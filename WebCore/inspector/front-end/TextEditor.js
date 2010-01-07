/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

WebInspector.TextEditor = function(platform)
{
    this._textModel = new WebInspector.TextEditorModel(this._textChanged.bind(this));
    this._highlighter = new WebInspector.TextEditorHighlighter(this._textModel);

    this.element = document.createElement("div");
    this.element.className = "text-editor";
    this.element.tabIndex = 0;

    this._canvas = document.createElement("canvas");
    this._canvas.className = "text-editor-canvas";
    this.element.appendChild(this._canvas);

    this._container = document.createElement("div");
    this._container.className = "text-editor-container";
    this.element.appendChild(this._container);

    this._sheet = document.createElement("div");
    this._sheet.className = "text-editor-sheet";
    this._container.appendChild(this._sheet);

    var cursorElement = document.createElement("div");
    cursorElement.className = "text-editor-cursor";
    this._container.appendChild(cursorElement);
    this._cursor = new WebInspector.TextCursor(cursorElement);

    this._container.addEventListener("scroll", this._scroll.bind(this));
    this._sheet.addEventListener("mouseup", this._mouseUp.bind(this));
    this._sheet.addEventListener("mousedown", this._mouseDown.bind(this));
    this._sheet.addEventListener("mousemove", this._mouseMove.bind(this));
    this._sheet.addEventListener("mouseout", this._mouseOut.bind(this));
    this._sheet.addEventListener("dblclick", this._dblClick.bind(this));
    this.element.addEventListener("keydown", this._keyDown.bind(this));
    this.element.addEventListener("textInput", this._textInput.bind(this));
    this.element.addEventListener("beforecopy", this._beforeCopy.bind(this));
    this.element.addEventListener("copy", this._copy.bind(this));
    this.element.addEventListener("beforecut", this._beforeCut.bind(this));
    this.element.addEventListener("cut", this._cut.bind(this));
    this.element.addEventListener("beforepaste", this._beforePaste.bind(this));
    this.element.addEventListener("paste", this._paste.bind(this));

    this._desiredCaretColumn = 0;
    this._scrollLeft = 0;
    this._scrollTop = 0;

    this._ctx = this._canvas.getContext("2d");
    this._selection = new WebInspector.TextSelectionModel(this._selectionChanged.bind(this));

    this._isMacTiger = platform === "mac-tiger";
    this._isMacLeopard = platform === "mac-leopard";
    this._isMac = this._isMacTiger || this._isMacLeopard;
    this._isWin = platform === "windows";

    this._initFont();

    this._paintCoalescingLevel = 0;

    this._registerShortcuts();
    // Debugging flags, allow disabling / enabling highlights and track repaints.
    this._highlightingEnabled = true;
    this._debugMode = false;
    this._lineAlignmentOffset = 0;

    this._textWidth = 0;
    this._longestLineNumber = 0;
}

WebInspector.TextEditor.prototype = {
    set text(text)
    {
        var lastLine = this._textModel.linesCount - 1;
        this._textModel.setText(null, text);
        this._textModel.resetUndoStack();
        this._setCaretLocation(0, 0);
    },

    setSelection: function(startLine, startColumn, endLine, endColumn)
    {
        var start = this._fit(startLine, startColumn);
        this._selection.setStart(start.line, start.column);
        this._setSelectionEnd(endLine, endColumn);
    },

    reveal: function(line, column) {
        var firstLine = this._scrollTop / this._lineHeight;
        var visibleLines = this._canvas.height / this._lineHeight;
        var minTop = (line - visibleLines + 1) * this._lineHeight;
        var maxTop = line * this._lineHeight;
        if (this._scrollTop < minTop) {
            this._lineAlignmentOffset = this._canvas.height % this._lineHeight;
            this._container.scrollTop = minTop;
        } else if (this._scrollTop > maxTop) {
            this._lineAlignmentOffset = 0;
            this._container.scrollTop = maxTop;
        }

        var firstColumn = this._columnForOffset(line, this._scrollLeft);
        var maxLeft = this._columnToOffset(line, column);
        var minLeft = maxLeft - this._container.clientWidth + this._lineNumberOffset;
        if (this._scrollLeft < minLeft)
            this._container.scrollLeft = minLeft + 100;
        if (this._scrollLeft > maxLeft)
            this._container.scrollLeft = maxLeft;
    },

    // WebInspector.TextModel listener
    _textChanged: function(oldRange, newRange, oldText, newText)
    {
        if (newRange.linesCount == oldRange.linesCount)
            this._invalidateLines(newRange.startLine, newRange.endLine + 1);
        else
            // Lines shifted, invalidate all under start line. Also clear lines that now are outside model range.
            this._invalidateLines(newRange.startLine, this._textModel.linesCount + Math.max(0, oldRange.endLine - newRange.endLine));

        this._invalidateHighlight(newRange.startLine);
        this._updateSize(newRange.startLine, Math.max(newRange.endLine, oldRange.endLine));
        this._paint();
    },

    // WebInspector.TextSelectionModel listener
    _selectionChanged: function(oldRange, newRange)
    {
        this._invalidateLines(oldRange.startLine, oldRange.endLine + 1);
        this._invalidateLines(newRange.startLine, newRange.endLine + 1);
        this._paint();
    },

    _updateSize: function(startLine, endLine)
    {
        var newLineNumberDigits = this._decimalDigits(this._textModel.linesCount);
        this._lineNumberOffset = (newLineNumberDigits + 2) * this._digitWidth;

        var guardedEndLine = Math.min(this._textModel.linesCount, endLine + 1);
        var newMaximum = false;
        for (var i = startLine; i < guardedEndLine; ++i) {
            var lineWidth = this._ctx.measureText(this._textModel.line(i)).width;
            if (lineWidth > this._textWidth) {
                this._textWidth = lineWidth;
                this._longestLineNumber = i;
                newMaximum = true;
            }
        }

        if (!newMaximum && startLine <= this._longestLineNumber && this._longestLineNumber <= endLine) {
            this._textWidth = 0;
            this._longestLineNumber = 0;
            for (var i = 0; i < this._textModel.linesCount; ++i) {
                var lineWidth = this._ctx.measureText(this._textModel.line(i)).width;
                if (lineWidth > this._textWidth) {
                    this._textWidth = lineWidth;
                    this._longestLineNumber = i;
                }
            }
        }

        this._sheet.style.width = this._textWidth + this._lineNumberOffset + "px";
        this._sheet.style.height = this._lineHeight * this._textModel.linesCount + "px";

        if (this._canvas.width !== this._container.clientWidth || this._canvas.height !== this._container.clientHeight || newLineNumberDigits !== this._lineNumberDigits) {
            this._canvas.width = this._container.clientWidth;
            this._canvas.height = this._container.clientHeight;
            this._lineNumberDigits = newLineNumberDigits;
            this._repaintAll();
        }
    },

    _repaintAll: function()
    {
        this._invalidateLines(0, this._textModel.linesCount);
        this._paint();
        this._updateCursor(this._selection.endLine, this._selection.endColumn);
    },

    _invalidateLines: function(startLine, endLine)
    {
        if (!this._damage)
            this._damage = [ { startLine: startLine, endLine: endLine } ];
        else {
            for (var i = 0; i < this._damage.length; ++i) {
                var chunk = this._damage[i];
                if (chunk.startLine <= endLine && chunk.endLine >= startLine) {
                    chunk.startLine = Math.min(chunk.startLine, startLine);
                    chunk.endLine = Math.max(chunk.endLine, endLine);
                    return;
                }
            }
            this._damage.push({ startLine: startLine, endLine: endLine });
        }
    },

    _paint: function()
    {
        if (this._paintCoalescingLevel)
            return;

        for (var i = 0; this._damage && i < this._damage.length; ++i)
            this._paintLines(this._damage[i].startLine, this._damage[i].endLine);
        delete this._damage;
    },

    _paintLines: function(firstLine, lastLine)
    {
        this._ctx.font = this._font;
        this._ctx.textBaseline = "bottom";

        firstLine = Math.max(firstLine, Math.floor(this._scrollTop / this._lineHeight) - 1);
        lastLine = Math.min(lastLine, Math.ceil((this._scrollTop + this._canvas.height) / this._lineHeight) + 1);
        if (firstLine > lastLine)
            return;

        if (this._debugMode) {
            WebInspector.log("Repaint %d:%d", firstLine, lastLine);
            this._ctx.fillStyle = "rgb(255,255,0)";
            var height = (lastLine - firstLine) * this._lineHeight;
            this._ctx.fillRect(0, this._lineHeight * firstLine - this._scrollTop, this._canvas.width, height);
            setTimeout(this._paintLinesContinuation.bind(this, firstLine, lastLine), 100);
        } else
            this._paintLinesContinuation(firstLine, lastLine);
    },

    _paintLinesContinuation: function(firstLine, lastLine) {
        // Clear all.
        var height = (lastLine - firstLine) * this._lineHeight;
        this._ctx.fillStyle = "rgb(255,255,255)";
        this._ctx.fillRect(0, this._lineHeight * firstLine - this._scrollTop, this._canvas.width, height);

        lastLine = Math.min(lastLine, this._textModel.linesCount);

        // Paint line numbers and separator.
        this._ctx.fillStyle = "rgb(235,235,235)";
        this._ctx.fillRect(this._lineNumberOffset - 2, 0, 1, this._canvas.width);
        this._ctx.fillStyle = "rgb(155,155,155)";
        for (var i = firstLine; i < lastLine; ++i)
           this._ctx.fillText(i + 1, (this._lineNumberDigits - this._decimalDigits(i + 1) + 1) * this._digitWidth, this._lineHeight * (i + 1) - this._scrollTop);

        // Clip editor area.
        this._ctx.save();
        this._ctx.beginPath();
        this._ctx.rect(this._lineNumberOffset - 1, 0, this._canvas.width - this._lineNumberOffset + 1, this._canvas.height);
        this._ctx.clip();

        if (this._selection.startLine === this._selection.endLine && firstLine <= this._selection.startLine && this._selection.startLine < lastLine)
            this._paintCurrentLine(this._selection.startLine);

        if (this._highlightingEnabled)
            this._highlighter.highlight(firstLine, lastLine);

        this._paintSelection(firstLine, lastLine);

        for (var i = firstLine; i < lastLine; ++i) {
            var line = this._textModel.line(i);
            if (!this._highlightingEnabled) {
                this._ctx.fillText(line, this._lineNumberOffset - this._scrollLeft, this._lineHeight * (i + 1) - this._scrollTop);
                continue;
            }

            var attributes = this._textModel.getAttributes(i, "highlight");
            var plainTextStart = -1;
            for (var j = 0; j < line.length;) {
                var attribute = attributes && attributes[j];
                if (!attribute || !attribute.style) {
                    if (plainTextStart === -1)
                        plainTextStart = j;
                    j++;
                } else {
                    if (plainTextStart !== -1) {
                        this._ctx.fillStyle = "rgb(0,0,0)";
                        this._ctx.fillText(line.substring(plainTextStart, j), this._lineNumberOffset - this._scrollLeft + this._columnToOffset(i, plainTextStart), this._lineHeight * (i + 1) - this._scrollTop);
                        plainTextStart = -1;
                    }
                    this._ctx.fillStyle = attribute.style;
                    this._ctx.fillText(line.substring(j, j + attribute.length), this._lineNumberOffset - this._scrollLeft + this._columnToOffset(i, j), this._lineHeight * (i + 1) - this._scrollTop);
                    j += attribute.length;
                }
            }
            if (plainTextStart !== -1) {
                this._ctx.fillStyle = "rgb(0,0,0)";
                this._ctx.fillText(line.substring(plainTextStart, line.length), this._lineNumberOffset - this._scrollLeft + this._columnToOffset(i, plainTextStart), this._lineHeight * (i + 1) - this._scrollTop);
            }
        }
        this._ctx.restore();
    },

    _paintCurrentLine: function(line)
    {
        this._ctx.fillStyle = "rgb(232, 242, 254)";
        this._ctx.fillRect(0, this._lineHeight * line - this._scrollTop, this._canvas.width, this._lineHeight);
        this._ctx.fillStyle = "rgb(0, 0, 0)";
    },

    _scroll: function(e)
    {
        if (this._muteScroll)
            return;

        if (this._container.scrollTop === 0)
            this._lineAlignmentOffset = 0;
        else if (this._container.scrollTop + this._container.clientHeight === this._textModel.linesCount * this._lineHeight)
            this._lineAlignmentOffset = this._container.scrollTop % this._lineHeight;

        // Enforce line alignment.
        if (this._container.scrollTop % this._lineHeight !== this._lineAlignmentOffset) {
            var linesOffset = Math.floor(this._container.scrollTop / this._lineHeight);
            this._muteScroll = true;
            this._container.scrollTop = linesOffset * this._lineHeight + this._lineAlignmentOffset;
            delete this._muteScroll;
        }

        // Hide div-based cursor first.
        this._cursor._cursorElement.style.display = "none";
        setTimeout(this._repaintOnScroll.bind(this), 0);
    },

    _repaintOnScroll: function()
    {
        var linesOffset = Math.floor(this._container.scrollTop / this._lineHeight);
        if (this._scrollTop !== this._container.scrollTop || this._scrollLeft !== this._container.scrollLeft) {
            this._scrollTop = this._container.scrollTop;
            this._scrollLeft = this._container.scrollLeft;
            this._repaintAll();
        }
    },

    _mouseUp: function(e)
    {
        this._isDragging = false;
    },

    _mouseDown: function(e)
    {
        var location = this._caretForMouseEvent(e);
        if (e.shiftKey)
            this._setSelectionEnd(location.line, location.column);
        else
            this._setCaretLocation(location.line, location.column);
        this._isDragging = true;
        this._textModel.markUndoableState();
    },

    _mouseMove: function(e)
    {
        if (!this._isDragging)
            return;
        var location = this._caretForMouseEvent(e);
        this._setSelectionEnd(location.line, location.column)
    },

    _mouseOut: function(e)
    {
        this._isDragging = false;
    },

    _dblClick: function(e)
    {
        var location = this._caretForMouseEvent(e);
        var range = this._textModel.wordRange(location.line, location.column);
        this.setSelection(range.startLine, range.startColumn, range.endLine, range.endColumn);
    },

    _caretForMouseEvent: function(e)
    {
        var lineNumber = Math.floor((e.y + this._scrollTop - 4) / this._lineHeight);
        var line = this._textModel.line(lineNumber);
        var offset = e.x + this._scrollLeft - this._lineNumberOffset - this._digitWidth;
        return { line: lineNumber, column: this._columnForOffset(lineNumber, offset) };
    },

    _columnForOffset: function(lineNumber, offset)
    {
        var line = this._textModel.line(lineNumber);
        for (var column = 0; column < line.length; ++column) {
            if (this._ctx.measureText(line.substring(0, column)).width > offset)
                break;
        }
        return column;
    },

    _columnToOffset: function(lineNumber, column)
    {
        var line = this._textModel.line(lineNumber);
        return this._ctx.measureText(line.substring(0, column)).width;
    },

    _keyDown: function(e)
    {
        var shortcutKey = WebInspector.KeyboardShortcut.makeKeyFromEvent(e);
        var handler = this._shortcuts[shortcutKey];
        if (handler) {
            handler.call(this);
            e.preventDefault();
            e.stopPropagation();
            return;
        }

        if (this._handleNavigationKey(e)) {
            e.preventDefault();
            e.stopPropagation();
            return;
        }

        var keyCodes = WebInspector.KeyboardShortcut.KeyCodes;
        switch (e.keyCode) {
            case keyCodes.Backspace:
                this._handleBackspaceKey();
                break;
            case keyCodes.Delete:
                this._handleDeleteKey();
                break;
            case keyCodes.Tab:
                this._replaceSelectionWith("\t");
                break;
            case keyCodes.Enter:
                this._replaceSelectionWith("\n");
                break;
            default:
                return;
        }

        e.preventDefault();
        e.stopPropagation();
    },

    _handleNavigationKey: function(e)
    {
        var caretLine = this._selection.endLine;
        var caretColumn = this._selection.endColumn;
        var arrowAction = e.shiftKey ? this._setSelectionEnd : this._setCaretLocation;

        var keyCodes = WebInspector.KeyboardShortcut.KeyCodes;
        switch (e.keyCode) {
            case keyCodes.Up:
            case keyCodes.PageUp:
                if (e.metaKey)
                    arrowAction.call(this, 0, 0, true);
                else if (e.ctrlKey)
                    this._container.scrollTop -= this._lineHeight;
                else {
                    var jump = e.keyCode === keyCodes.Up ? 1 : Math.floor(this._canvas.height / this._lineHeight);
                    arrowAction.call(this, caretLine - jump, this._desiredCaretColumn, true);
                }
                break;
            case keyCodes.Down:
            case keyCodes.PageDown:
                if (e.metaKey)
                    arrowAction.call(this, this._textModel.linesCount - 1, this._textModel.lineLength(this._textModel.linesCount - 1), true);
                else if (e.ctrlKey)
                    this._container.scrollTop += this._lineHeight;
                else {
                    var jump = e.keyCode === keyCodes.Down ? 1 : Math.floor(this._canvas.height / this._lineHeight);
                    arrowAction.call(this, caretLine + jump, this._desiredCaretColumn, true);
                }
                break;
            case keyCodes.Left:
                if (!e.shiftKey && !e.metaKey && !this._isAltCtrl(e) && !this._selection.isEmpty()) {
                    // Reset selection
                    var range = this._selection.range();
                    this._setCaretLocation(range.startLine, range.startColumn);
                } else if (e.metaKey)
                    arrowAction.call(this, this._selection.endLine, 0);
                else if (caretColumn === 0 && caretLine > 0)
                    arrowAction.call(this, caretLine - 1, this._textModel.lineLength(caretLine - 1));
                else if (this._isAltCtrl(e)) {
                    caretColumn = this._textModel.wordStart(this._selection.endLine, this._selection.endColumn);
                    if (caretColumn === this._selection.endColumn)
                        caretColumn = 0;
                    arrowAction.call(this, caretLine, caretColumn);
                } else
                    arrowAction.call(this, caretLine, caretColumn - 1);
                break;
            case keyCodes.Right:
                var line = this._textModel.line(caretLine);
                if (!e.shiftKey && !e.metaKey && !this._isAltCtrl(e) && !this._selection.isEmpty()) {
                    // Reset selection
                    var range = this._selection.range();
                    this._setCaretLocation(range.endLine, range.endColumn);
                } else if (e.metaKey)
                    arrowAction.call(this, this._selection.endLine, this._textModel.lineLength(this._selection.endLine));
                else if (caretColumn === line.length && caretLine < this._textModel.linesCount - 1)
                    arrowAction.call(this, caretLine + 1, 0);
                else if (this._isAltCtrl(e)) {
                    caretColumn = this._textModel.wordEnd(this._selection.endLine, this._selection.endColumn);
                    if (caretColumn === this._selection.endColumn)
                        caretColumn = line.length;
                    arrowAction.call(this, caretLine, caretColumn);
                } else
                    arrowAction.call(this, caretLine, caretColumn + 1);
                break;
            default:
                return false;
        }
        this._textModel.markUndoableState();
        return true;
    },

    _textInput: function(e)
    {
        if (e.data && !e.altKey && !e.ctrlKey && !e.metaKey) {
            this._replaceSelectionWith(e.data);
            e.preventDefault();
            e.stopPropagation();
        }
    },

    _setCaretLocation: function(line, column, updown)
    {
        this.setSelection(line, column, line, column, updown);
    },

    _setSelectionEnd: function(line, column, updown)
    {
        if (!updown)
            this._desiredCaretColumn = column;

        var end = this._fit(line, column);
        this._selection.setEnd(end.line, end.column);
        this.reveal(this._selection.endLine, this._selection.endColumn);
        this._updateCursor(end.line, end.column);
    },

    _updateCursor: function(line, column)
    {
        if (line > this._textModel.linesCount)
            return;
        var offset = this._columnToOffset(line, column);
        if (offset >= this._container.scrollLeft)
            this._cursor.setLocation(this._lineNumberOffset + offset - 1, line * this._lineHeight);
        else
            this._cursor.setLocation(0, 0);
    },

    _fit: function(line, column)
    {
        line = Math.max(0, Math.min(line, this._textModel.linesCount - 1));
        var lineLength = this._textModel.lineLength(line);
        column = Math.max(0, Math.min(column, lineLength));
        return { line: line, column: column };
    },

    _invalidateHighlight: function(startLine)
    {
        if (!this._highlightingEnabled)
            return;
        var firstVisibleLine = Math.max(0, Math.floor(this._scrollTop / this._lineHeight) - 1);
        var lastVisibleLine = Math.min(this._textModel.linesCount, Math.ceil(firstVisibleLine + this._canvas.height / this._lineHeight + 1));

        var damage = this._highlighter.highlight(startLine, lastVisibleLine);
        for (var line in damage)
            this._invalidateLines(line, parseInt(line) + 1);
    },

    _paintSelection: function(firstLine, lastLine)
    {
        if (this._selection.isEmpty())
            return;
        var range = this._selection.range();
        this._ctx.fillStyle = "rgb(181, 213, 255)";

        firstLine = Math.max(firstLine, range.startLine);
        endLine = Math.min(lastLine, range.endLine + 1);

        for (var i = firstLine; i < endLine; ++i) {
            var line = this._textModel.line(i);
            var from, to;

            if (i === range.startLine) {
                var offset = this._columnToOffset(range.startLine, range.startColumn);
                from = offset - this._scrollLeft + this._lineNumberOffset - 1;
            } else
                from = 0;

            if (i === range.endLine) {
                var offset = this._columnToOffset(range.endLine, range.endColumn);
                to = offset - this._scrollLeft + this._lineNumberOffset - 1;
            } else
                to = this._canvas.width;

            this._ctx.fillRect(from, this._lineHeight * i - this._scrollTop, to - from, this._lineHeight);
        }
        this._ctx.fillStyle = "rgb(0, 0, 0)";
    },

    _beforeCopy: function(e)
    {
        if (!this._selection.isEmpty())
            e.preventDefault();
    },

    _copy: function(e)
    {
        var range = this._selection.range();
        var text = this._textModel.copyRange(range);

        e.preventDefault();
        e.clipboardData.clearData();
        e.clipboardData.setData("text/plain", text);
    },

    _beforeCut: function(e)
    {
        if (!this._selection.isEmpty())
            e.preventDefault();
    },

    _cut: function(e)
    {
        this._textModel.markUndoableState();
        this._copy(e);
        this._replaceSelectionWith("");
    },

    _beforePaste: function(e)
    {
        e.preventDefault();
    },

    _paste: function(e)
    {
        var text = e.clipboardData.getData("Text");
        if (!text)
            return;

        this._textModel.markUndoableState();
        this._replaceSelectionWith(text);
        e.preventDefault();
    },

    _replaceSelectionWith: function(newText, overrideRange)
    {
        var range = overrideRange || this._selection.range();

        this._setCoalescingUpdate(true);
        var newRange = this._textModel.setText(range, newText);
        this._setCaretLocation(newRange.endLine, newRange.endColumn);
        this._setCoalescingUpdate(false);
    },

    _setCoalescingUpdate: function(enabled)
    {
        if (enabled)
            this._paintCoalescingLevel++;
        else
            this._paintCoalescingLevel--;
        if (!this._paintCoalescingLevel)
            this._paint();
    },

    _selectAll: function()
    {
        // No need to reveal last selection line in select all.
        this._selection.setStart(0, 0);
        var lastLineNum = this._textModel.linesCount - 1;
        this._selection.setEnd(lastLineNum, this._textModel.lineLength(lastLineNum));
        this._updateCursor(this._selection.endLine, this._selection.endColumn);
    },

    _initFont: function(sansSerif, fontSize)
    {
        if (sansSerif) {
            this._isMonospace = false;
            this._fontSize = fontSize || 11;
            this._font = this._fontSize + "px sans-serif";
        } else {
            this._isMonospace = true;
            this._fontSize = fontSize || 10;
            this._font = this._fontSize + "px monospace";
            if (this._isMac)
                this._font = this._fontSize + "px Monaco";
            else if (this._isWin)
                this._font = (this._fontSize + 1) + "px Courier New";
        }
        this._ctx.font = this._font;
        this._digitWidth = this._ctx.measureText("0").width;

        this._lineHeight = Math.floor(this._fontSize * 1.4);
        this._cursor.setLineHeight(this._lineHeight);
    },

    _registerShortcuts: function()
    {
        var modifiers = WebInspector.KeyboardShortcut.Modifiers;
        this._shortcuts = {};
        this._shortcuts[WebInspector.KeyboardShortcut.makeKey("z", this._isMac ? modifiers.Meta : modifiers.Ctrl)] = this._handleUndo.bind(this);
        this._shortcuts[WebInspector.KeyboardShortcut.makeKey("z", modifiers.Shift | (this._isMac ? modifiers.Meta : modifiers.Ctrl))] = this._handleRedo.bind(this);
        this._shortcuts[WebInspector.KeyboardShortcut.makeKey("a", this._isMac ? modifiers.Meta : modifiers.Ctrl)] = this._selectAll.bind(this);
        this._shortcuts[WebInspector.KeyboardShortcut.makeKey(WebInspector.KeyboardShortcut.KeyCodes.Plus, this._isMac ? modifiers.Meta : modifiers.Ctrl)] = this._handleZoomIn.bind(this);
        this._shortcuts[WebInspector.KeyboardShortcut.makeKey(WebInspector.KeyboardShortcut.KeyCodes.Minus, this._isMac ? modifiers.Meta : modifiers.Ctrl)] = this._handleZoomOut.bind(this);
        this._shortcuts[WebInspector.KeyboardShortcut.makeKey(WebInspector.KeyboardShortcut.KeyCodes.Zero, this._isMac ? modifiers.Meta : modifiers.Ctrl)] = this._handleZoomReset.bind(this);
        if (this._isMac)
            this._shortcuts[WebInspector.KeyboardShortcut.makeKey("d", modifiers.Ctrl)] = this._handleDeleteKey.bind(this);

        this._shortcuts[WebInspector.KeyboardShortcut.makeKey("d", modifiers.Ctrl | modifiers.Alt)] = this._handleToggleDebugMode.bind(this);
        this._shortcuts[WebInspector.KeyboardShortcut.makeKey("h", modifiers.Ctrl | modifiers.Alt)] = this._handleToggleHighlightMode.bind(this);
        this._shortcuts[WebInspector.KeyboardShortcut.makeKey("m", modifiers.Ctrl | modifiers.Alt)] = this._handleToggleMonospaceMode.bind(this);
    },

    _handleUndo: function()
    {
        this._setCoalescingUpdate(true);
        var range = this._textModel.undo();
        if (range)
            this._setCaretLocation(range.endLine, range.endColumn);
        this._setCoalescingUpdate(false);
    },

    _handleRedo: function()
    {
        this._setCoalescingUpdate(true);
        var range = this._textModel.redo();
        if (range)
            this._setCaretLocation(range.endLine, range.endColumn);
        this._setCoalescingUpdate(false);
    },

    _handleDeleteKey: function()
    {
        var range = this._selection.range();
        if (range.isEmpty()) {
            if (range.endColumn < this._textModel.lineLength(range.startLine))
                range.endColumn++;
            else if (range.endLine < this._textModel.linesCount) {
                range.endLine++;
                range.endColumn = 0;
            } else
                return;
        } else
            this._textModel.markUndoableState();
        this._replaceSelectionWith("", range);
    },

    _handleBackspaceKey: function()
    {
        var range = this._selection.range();
        if (range.isEmpty()) {
            if (range.startColumn > 0)
                range.startColumn--;
            else if (range.startLine > 0) {
                range.startLine--;
                range.startColumn = this._textModel.lineLength(range.startLine);
            } else
                return;
        } else
            this._textModel.markUndoableState();
        this._replaceSelectionWith("", range);
    },

    _handleToggleDebugMode: function()
    {
        this._debugMode = !this._debugMode;
    },

    _handleZoomIn: function()
    {
        if (this._fontSize < 25)
            this._changeFont(!this._isMonospace, this._fontSize + 1);
    },

    _handleZoomOut: function()
    {
        if (this._fontSize > 1)
            this._changeFont(!this._isMonospace, this._fontSize - 1);
    },

    _handleZoomReset: function()
    {
        this._changeFont(!this._isMonospace);
    },

    _handleToggleMonospaceMode: function()
    {
        this._changeFont(this._isMonospace, this._fontSize);
    },

    _changeFont: function(sansSerif, fontSize) {
        this._initFont(sansSerif, fontSize);
        this._updateSize(0, this._textModel.linesCount);
        this._repaintAll();
    },

    _handleToggleHighlightMode: function()
    {
        this._highlightingEnabled = !this._highlightingEnabled;
    },

    _isMetaCtrl: function(e)
    {
        return this._isMac ? e.metaKey : e.ctrlKey;
    },

    _isAltCtrl: function(e)
    {
        return this._isMac ? e.altKey : e.ctrlKey;
    },

    _decimalDigits: function(number)
    {
        return Math.ceil(Math.log(number + 1) / Math.log(10));
    }
}

WebInspector.TextSelectionModel = function(changeListener)
{
    this.startLine = 0;
    this.startColumn = 0;
    this.endLine = 0;
    this.endColumn = 0;
    this._changeListener = changeListener;
}

WebInspector.TextSelectionModel.prototype = {
    setStart: function(line, column)
    {
        var oldRange = this.range();

        this.startLine = line;
        this.startColumn = column;
        this.endLine = line;
        this.endColumn = column;

        this._changeListener(oldRange, this.range());
    },

    setEnd: function(line, column)
    {
        var oldRange = this.range();

        this.endLine = line;
        this.endColumn = column;

        this._changeListener(oldRange, this.range(), this.endLine, this.endColumn);
    },

    range: function()
    {
        if (this.startLine < this.endLine || (this.startLine === this.endLine && this.startColumn <= this.endColumn))
            return new WebInspector.TextRange(this.startLine, this.startColumn, this.endLine, this.endColumn);
        else
            return new WebInspector.TextRange(this.endLine, this.endColumn, this.startLine, this.startColumn);
    },

    isEmpty: function()
    {
        return this.startLine === this.endLine && this.startColumn === this.endColumn;
    }
}

WebInspector.TextCursor = function(cursorElement)
{
    this._visible = false;
    this._cursorElement = cursorElement;
}

WebInspector.TextCursor.prototype = {
    setLocation: function(x, y)
    {
        this._x = x;
        this._y = y;
        if (this._paintInterval) {
            window.clearInterval(this._paintInterval);
            delete this._paintInterval;
        }
        this._paintInterval = window.setInterval(this._paint.bind(this, false), 500);
        this._paint(true);
    },

    setLineHeight: function(lineHeight)
    {
        this._cursorElement.style.height = lineHeight + "px";
    },

    _paint: function(force)
    {
        if (force)
            this._visible = true;
        else
            this._visible = !this._visible;
        this._cursorElement.style.left = this._x + "px";
        this._cursorElement.style.top = this._y + "px";
        this._cursorElement.style.display = this._visible ? "block" : "none";
    }
}
