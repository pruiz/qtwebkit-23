/*	
    IFWebController.mm
	Copyright 2001, Apple, Inc. All rights reserved.
*/

#import <WebKit/IFBaseWebController.h>

@class IFError;
@class IFLoadProgress;
@class IFWebFrame;

@interface IFBaseWebControllerPrivate : NSObject
{
    IFWebFrame *mainFrame;
}
@end

@interface IFBaseWebController (IFPrivate);
- (void)_receivedProgress: (IFLoadProgress *)progress forResource: (NSString *)resourceDescription fromDataSource: (IFWebDataSource *)dataSource;
- (void)_receivedError: (IFError *)error forResource: (NSString *)resourceDescription partialProgress: (IFLoadProgress *)progress fromDataSource: (IFWebDataSource *)dataSource;
- (void)_mainReceivedProgress: (IFLoadProgress *)progress forResource: (NSString *)resourceDescription fromDataSource: (IFWebDataSource *)dataSource;
- (void)_mainReceivedError: (IFError *)error forResource: (NSString *)resourceDescription partialProgress: (IFLoadProgress *)progress fromDataSource: (IFWebDataSource *)dataSource;
- (void)_didStartLoading: (NSURL *)url;
- (void)_didStopLoading: (NSURL *)url;
@end
