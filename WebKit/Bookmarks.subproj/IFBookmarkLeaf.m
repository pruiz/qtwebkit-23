//
//  IFBookmarkLeaf.m
//  WebKit
//
//  Created by John Sullivan on Tue Apr 30 2002.
//  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
//

#import <WebKit/IFBookmarkLeaf.h>
#import <WebKit/IFBookmark_Private.h>
#import <WebKit/IFBookmarkGroup.h>
#import <WebKit/IFBookmarkGroup_Private.h>
#import <WebKit/IFURIEntry.h>
#import <WebKit/WebKitDebug.h>

@implementation IFBookmarkLeaf

- (id)initWithURLString:(NSString *)URLString
                  title:(NSString *)title
                  image:(NSImage *)image
                  group:(IFBookmarkGroup *)group;
{
    WEBKIT_ASSERT_VALID_ARG (group, group != nil);
    
    [super init];

    // Since our URLString may not be valid for creating an NSURL object,
    // just hang onto the string separately and don't bother creating
    // an NSURL object for the IFURIEntry.
    _entry = [[IFURIEntry alloc] initWithURL:nil title:title image:image];
    _URLString = [URLString retain];
    [self _setGroup:group];

    return self;
}

- (void)dealloc
{
    [_entry release];
    [_URLString release];
    [super dealloc];
}

- (NSString *)title
{
    return [_entry title];
}

- (void)setTitle:(NSString *)title
{
    [_entry setTitle:title];

    [[self _group] _bookmarkDidChange:self];    
}

- (NSImage *)image
{
    return [_entry image];
}

- (void)setImage:(NSImage *)image
{
    [_entry setImage:image];

    [[self _group] _bookmarkDidChange:self];    
}

- (BOOL)isLeaf
{
    return YES;
}

- (NSString *)URLString
{
    return _URLString;
}

- (void)setURLString:(NSString *)URLString
{
    if ([URLString isEqualToString:_URLString]) {
        return;
    }

    [_URLString release];
    _URLString = [[NSString stringWithString:URLString] retain];

    [[self _group] _bookmarkDidChange:self];    
}

@end
