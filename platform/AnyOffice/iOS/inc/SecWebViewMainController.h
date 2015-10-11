//
//  SecWebViewMainController.h
//  anyofficesdk
//
//  Created by num1 on 14-7-14.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AnyOfficeShareToWeChatSceneSessionActivity.h"
#import "ChageFontSizeActivity.h"
#import "RefreshWebAppActivity.h"
#import "AnyOfficeWebView.h"

@protocol CloseWebAppDelegate <NSObject>

-(void)closeWebApp;

@end

@interface SecWebViewMainController : UIViewController<UIWebViewDelegate, ChangeFontSizeDelegate, RefreshWebAppDelegate, AnyOfficeWebViewDelegate,ShareToWeChatSceneSessionDelegate, UITextFieldDelegate,UIGestureRecognizerDelegate>{
}
@property (nonatomic, retain) NSURL *originUrl;
@property (nonatomic, copy) NSString *webAppName;

- (id)initWithURL:(NSURL*)url topTitle:(NSString *)title delegate:(id<CloseWebAppDelegate>)delegate;
- (id)initWithURL:(NSURL*)url topTitle:(NSString *)title ssoFlag:(BOOL)isSingleSignOn delegate:(id<CloseWebAppDelegate>)delegate;
-(void)setURLBarHidden:(BOOL)hidden;
- (void)closeClick;
-(BOOL)registerShareToWeChatAppID:(NSString *)appID;
@end
