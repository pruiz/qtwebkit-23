/*
        WebNetscapePluginStream.h
	Copyright 2002, Apple, Inc. All rights reserved.
*/

#import <Foundation/Foundation.h>

#import <WebKit/npapi.h>
#import <WebKit/WebBaseNetscapePluginStream.h>

@class WebNetscapePluginEmbeddedView;
@class WebResourceHandle;
@class WebResourceRequest;


@interface WebNetscapePluginStream : WebBaseNetscapePluginStream 
{
    WebNetscapePluginEmbeddedView *view;
    
    WebResourceRequest *request;
    WebResourceHandle *resource;
    
    NSMutableData *resourceData;

    NSURL *currentURL;
}

- initWithRequest:(WebResourceRequest *)theRequest
    pluginPointer:(NPP)thePluginPointer
       notifyData:(void *)theNotifyData;

- (void)start;

- (void)stop;

@end
