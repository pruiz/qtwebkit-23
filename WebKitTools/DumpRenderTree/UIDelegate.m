/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
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

#import "UIDelegate.h"

#import "DumpRenderTree.h"
#import "DumpRenderTreeDraggingInfo.h"
#import "EventSendingController.h"
#import <WebKit/WebFramePrivate.h>
#import <WebKit/WebHTMLViewPrivate.h>
#import <WebKit/WebView.h>

DumpRenderTreeDraggingInfo *draggingInfo = nil;

@implementation UIDelegate

- (void)webView:(WebView *)sender addMessageToConsole:(NSDictionary *)dictionary
{
    NSString *message = [dictionary objectForKey:@"message"];
    NSNumber *lineNumber = [dictionary objectForKey:@"lineNumber"];
    
    printf ("CONSOLE MESSAGE: line %d: %s\n", [lineNumber intValue], [message UTF8String]);
}

- (void)webView:(WebView *)sender runJavaScriptAlertPanelWithMessage:(NSString *)message
{
    if (!done)
        printf("ALERT: %s\n", [message UTF8String]);
}

- (void)webView:(WebView *)sender dragImage:(NSImage *)anImage at:(NSPoint)viewLocation offset:(NSSize)initialOffset event:(NSEvent *)event pasteboard:(NSPasteboard *)pboard source:(id)sourceObj slideBack:(BOOL)slideFlag forView:(NSView *)view
{
     assert(!draggingInfo);
     if (addFileToPasteboardOnDrag) {
         [pboard declareTypes:[NSArray arrayWithObject:NSFilenamesPboardType] owner:nil];
         [pboard setPropertyList:[NSArray arrayWithObject:@"DRTFakeFile"] forType:NSFilenamesPboardType];
     }
     draggingInfo = [[DumpRenderTreeDraggingInfo alloc] initWithImage:anImage offset:initialOffset pasteboard:pboard source:sourceObj];
     [EventSendingController replaySavedEvents];
}

- (void)webViewFocus:(WebView *)webView
{
    windowIsKey = YES;
    NSView *documentView = [[mainFrame frameView] documentView];
    if ([documentView isKindOfClass:[WebHTMLView class]])
        [(WebHTMLView *)documentView _updateActiveState];
}

- (WebView *)webView:(WebView *)sender createWebViewWithRequest:(NSURLRequest *)request
{
    if (!canOpenWindows)
        return nil;
    
    // Make sure that waitUntilDone has been called.
    assert(waitToDump);

    WebView *webView = createWebView();
    
    return [webView autorelease];
}

- (void)webViewClose:(WebView *)sender
{
    NSWindow* window = [sender window];
 
    if (closeWebViews)
        [sender close];
    
    [window close];
}

- (void)dealloc
{
    [draggingInfo release];
    draggingInfo = nil;

    [super dealloc];
}

@end
