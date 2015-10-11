//
//  SecWebViewController.h
//  anyofficesdk
//
//  Created by fanjiepeng on 15-4-14.
//  Copyright (c) 2015年 fanjiepeng. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SecWebViewMainController.h"
@interface SecWebViewController : UIViewController<CloseWebAppDelegate>
@property (nonatomic, retain) NSURL *originUrl;
@property (nonatomic, copy) NSString *webAppName;

- (id)initWithURL:(NSURL*)url topTitle:(NSString *)title;
- (id)initWithURL:(NSURL*)url topTitle:(NSString *)title ssoFlag:(BOOL)isSingleSignOn;
-(void)setURLBarHidden:(BOOL)hidden;
-(void)dismissSecWebViewConttroller;
-(BOOL)registerShareToWeChatAppID:(NSString *)appID;
@end
