/*
 * Copyright (C) 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#include "DumpRenderTree.h"
#include "UIDelegate.h"

#include "DraggingInfo.h"
#include "EventSender.h"
#include "LayoutTestController.h"

#include <WebCore/COMPtr.h>
#include <wtf/Platform.h>
#include <JavaScriptCore/Assertions.h>
#include <JavaScriptCore/JavaScriptCore.h>
#include <WebKit/IWebFramePrivate.h>
#include <WebKit/IWebViewPrivate.h>
#include <stdio.h>

using std::wstring;

UIDelegate::UIDelegate()
    : m_refCount(1)
{
    m_frame.bottom = 0;
    m_frame.top = 0;
    m_frame.left = 0;
    m_frame.right = 0;
}

HRESULT STDMETHODCALLTYPE UIDelegate::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<IWebUIDelegate*>(this);
    else if (IsEqualGUID(riid, IID_IWebUIDelegate))
        *ppvObject = static_cast<IWebUIDelegate*>(this);
    else if (IsEqualGUID(riid, IID_IWebUIDelegatePrivate))
        *ppvObject = static_cast<IWebUIDelegatePrivate*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE UIDelegate::AddRef()
{
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE UIDelegate::Release()
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

    return newRef;
}

HRESULT STDMETHODCALLTYPE UIDelegate::hasCustomMenuImplementation( 
        /* [retval][out] */ BOOL *hasCustomMenus)
{
    *hasCustomMenus = TRUE;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::trackCustomPopupMenu( 
        /* [in] */ IWebView *sender,
        /* [in] */ OLE_HANDLE menu,
        /* [in] */ LPPOINT point)
{
    // Do nothing
    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::setFrame( 
        /* [in] */ IWebView* /*sender*/,
        /* [in] */ RECT* frame)
{
    m_frame = *frame;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::webViewFrame( 
        /* [in] */ IWebView* /*sender*/,
        /* [retval][out] */ RECT* frame)
{
    *frame = m_frame;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::runJavaScriptAlertPanelWithMessage( 
        /* [in] */ IWebView* /*sender*/,
        /* [in] */ BSTR message)
{
    printf("ALERT: %S\n", message ? message : L"");

    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::runJavaScriptConfirmPanelWithMessage( 
    /* [in] */ IWebView* sender,
    /* [in] */ BSTR message,
    /* [retval][out] */ BOOL* result)
{
    printf("CONFIRM: %S\n", message ? message : L"");
    *result = TRUE;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::runJavaScriptTextInputPanelWithPrompt( 
    /* [in] */ IWebView *sender,
    /* [in] */ BSTR message,
    /* [in] */ BSTR defaultText,
    /* [retval][out] */ BSTR *result)
{
    printf("PROMPT: %S, default text: %S\n", message ? message : L"", defaultText ? defaultText : L"");
    *result = SysAllocString(defaultText);

    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::webViewAddMessageToConsole( 
    /* [in] */ IWebView* sender,
    /* [in] */ BSTR message,
    /* [in] */ int lineNumber,
    /* [in] */ BSTR url,
    /* [in] */ BOOL isError)
{
    wstring newMessage;
    if (message) {
        newMessage = message;
        size_t fileProtocol = newMessage.find(L"file://");
        if (fileProtocol != wstring::npos)
            newMessage = newMessage.substr(0, fileProtocol) + urlSuitableForTestResult(newMessage.substr(fileProtocol));
    }

    printf("CONSOLE MESSAGE: line %d: %S\n", lineNumber, newMessage.c_str());
    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::doDragDrop( 
    /* [in] */ IWebView* sender,
    /* [in] */ IDataObject* object,
    /* [in] */ IDropSource* source,
    /* [in] */ DWORD okEffect,
    /* [retval][out] */ DWORD* performedEffect)
{
    if (!performedEffect)
        return E_POINTER;

    *performedEffect = 0;

    draggingInfo = new DraggingInfo(object, source);

    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::webViewGetDlgCode( 
    /* [in] */ IWebView* /*sender*/,
    /* [in] */ UINT /*keyCode*/,
    /* [retval][out] */ LONG_PTR *code)
{
    if (!code)
        return E_POINTER;
    *code = 0;
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE UIDelegate::createWebViewWithRequest( 
        /* [in] */ IWebView *sender,
        /* [in] */ IWebURLRequest *request,
        /* [retval][out] */ IWebView **newWebView)
{
    if (!::layoutTestController->canOpenWindows())
        return E_FAIL;
    *newWebView = createWebViewAndOffscreenWindow();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE UIDelegate::webViewClose( 
        /* [in] */ IWebView *sender)
{
    HWND hostWindow;
    sender->hostWindow(reinterpret_cast<OLE_HANDLE*>(&hostWindow));
    DestroyWindow(hostWindow);
    return S_OK;
}
