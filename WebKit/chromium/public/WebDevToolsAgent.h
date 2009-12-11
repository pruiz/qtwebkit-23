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

#ifndef WebDevToolsAgent_h
#define WebDevToolsAgent_h

#include "WebCommon.h"

namespace WebKit {
class WebDevToolsAgentClient;
class WebFrame;
class WebString;
class WebURLRequest;
class WebURLResponse;
class WebView;
struct WebDevToolsMessageData;
struct WebPoint;
struct WebURLError;

class WebDevToolsAgent {
public:
    WEBKIT_API static WebDevToolsAgent* create(WebView*, WebDevToolsAgentClient*);

    virtual ~WebDevToolsAgent() {}

    virtual void attach() = 0;
    virtual void detach() = 0;

    virtual void didNavigate() = 0;

    virtual void dispatchMessageFromFrontend(const WebDevToolsMessageData&) = 0;

    virtual void inspectElementAt(const WebPoint&) = 0;

    virtual void setRuntimeFeatureEnabled(const WebString& feature, bool enabled) = 0;

    // Exposed for LayoutTestController.
    virtual void evaluateInWebInspector(long callId, const WebString& script) = 0;
    virtual void setTimelineProfilingEnabled(bool enabled) = 0;

    // Asynchronously executes debugger command in the render thread.
    // |callerIdentifier| will be used for sending response.
    WEBKIT_API static void executeDebuggerCommand(
        const WebString& command, int callerIdentifier);

    // Asynchronously request debugger to pause immediately.
    WEBKIT_API static void debuggerPauseScript();

    WEBKIT_API static bool dispatchMessageFromFrontendOnIOThread(const WebDevToolsMessageData&);

    typedef void (*MessageLoopDispatchHandler)();

    // Installs dispatch handle that is going to be called periodically
    // while on a breakpoint.
    WEBKIT_API static void setMessageLoopDispatchHandler(MessageLoopDispatchHandler);

    virtual void identifierForInitialRequest(unsigned long resourceId, WebFrame*, const WebURLRequest&) = 0;
    virtual void willSendRequest(unsigned long resourceId, const WebURLRequest&) = 0;
    virtual void didReceiveData(unsigned long resourceId, int length) = 0;
    virtual void didReceiveResponse(unsigned long resourceId, const WebURLResponse&) = 0;
    virtual void didFinishLoading(unsigned long resourceId) = 0;
    virtual void didFailLoading(unsigned long resourceId, const WebURLError&) = 0;
};

} // namespace WebKit

#endif
