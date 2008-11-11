/*
 * Copyright (C) 2005, 2007 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if ENABLE(NETSCAPE_PLUGIN_API)
#import <Cocoa/Cocoa.h>

#import "WebNetscapePluginPackage.h"

#import <wtf/RetainPtr.h>

@class DOMElement;
@class WebDataSource;
@class WebFrame;
@class WebView;

@interface WebBaseNetscapePluginView : NSView
{
    RetainPtr<WebNetscapePluginPackage> _pluginPackage;
    
    WebFrame *_webFrame;
    
    int _mode;
    
    BOOL _loadManually;
    BOOL _shouldFireTimers;
    BOOL _isStarted;
    BOOL _hasFocus;
    BOOL _isCompletelyObscured;
    
    RetainPtr<DOMElement> _element;
    RetainPtr<NSString> _MIMEType;
    RetainPtr<NSURL> _baseURL;
    RetainPtr<NSURL> _sourceURL;
    
    NSTrackingRectTag _trackingTag;
}

- (id)initWithFrame:(NSRect)r
      pluginPackage:(WebNetscapePluginPackage *)thePluginPackage
                URL:(NSURL *)URL
            baseURL:(NSURL *)baseURL
           MIMEType:(NSString *)MIME
      attributeKeys:(NSArray *)keys
    attributeValues:(NSArray *)values
       loadManually:(BOOL)loadManually
         DOMElement:(DOMElement *)anElement;

// Subclasses must override these.
- (void)handleMouseMoved:(NSEvent *)event;
- (void)setAttributeKeys:(NSArray *)keys andValues:(NSArray *)values;
- (void)focusChanged;

- (WebFrame *)webFrame;
- (WebDataSource *)dataSource;
- (WebView *)webView;
- (NSWindow *)currentWindow;

- (void)removeTrackingRect;
- (void)resetTrackingRect;

- (void)stopTimers;
- (void)startTimers;
- (void)restartTimers;

- (void)setHasFocus:(BOOL)hasFocus;
- (void)sendActivateEvent:(BOOL)activate;

@end

#endif

