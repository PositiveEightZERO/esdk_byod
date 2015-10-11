//
//  AnyOfficeShareToWeChatSceneSessionActivity.h
//  anyofficesdk
//
//  Created by f00291727 on 07/15/15.
//  Copyright (c) 2015 fanjiepeng. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AnyOfficeShareToWeChatAlertView.h"
#include "AnyOfficeWebView.h"
#import "WXApi.h"
#import "WXApiObject.h"
@protocol ShareToWeChatSceneSessionDelegate <NSObject>

-(void)performAnyOfficeShareToWeChatSceneSessionActivity;

@end

@interface AnyOfficeShareToWeChatSceneSessionActivity : UIActivity<UITextViewDelegate, AnyOfficeShareToWeChatAlertViewDelegate,WXApiDelegate>

- (id) initWithURL:(NSURL *)url webView:(AnyOfficeWebView *)web delegate:(id<ShareToWeChatSceneSessionDelegate>)delegate;

@end
