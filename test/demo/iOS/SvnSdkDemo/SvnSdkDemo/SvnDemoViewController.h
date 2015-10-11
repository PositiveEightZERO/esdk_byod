//
//  SvnDemoViewController.h
//  SvnSdkDemo
//
//  Created by l00174413 on 13-6-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <SvnSdk/SDKContext.h>
#import <SvnSdk/LoginParam.h>
#import <SvnSdk/LoginAgent.h>
#import <SvnSdk/NetStatusManager.h>
#import <SvnSdk/MdmDeviceIdInfo.h>
#import <SvnSdk/AppManager.h>
#import <SvnSdk/AppInfo.h>

@interface SvnDemoViewController : UIViewController<UITextFieldDelegate, NetChangeCallbackDelegate>

@property (weak, nonatomic) IBOutlet UITextField *gatewayTextFiled;
@property (weak, nonatomic) IBOutlet UITextField *usernameTextFiled;
@property (weak, nonatomic) IBOutlet UITextField *passwordTextFiled;
@property (weak, nonatomic) IBOutlet UITextField *tunnelIPTextFiled;
@property (weak, nonatomic) IBOutlet UIButton *loginButton;
@property (weak, nonatomic) IBOutlet UIButton *encryptButton;

@property (weak, nonatomic) IBOutlet UIButton *faqsButton;
@property (weak, nonatomic) IBOutlet UIButton *enterButton;

@property (weak, nonatomic) IBOutlet UIButton *logoutButton;



@property (weak, nonatomic) IBOutlet UIButton *checkUpdateButton;

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UIView *contentView;
@property (weak, nonatomic) IBOutlet UIView *inputView;

@property (weak, nonatomic) IBOutlet UIView *operateView;


@property (nonatomic, weak) IBOutlet UIView *logingView;
@property (nonatomic, weak) IBOutlet UIView *loginSuccessView;



- (IBAction)loginButtonClicked:(id)sender;

- (IBAction)logoutButtonClicked:(id)sender;

- (IBAction)checkUpdateButtonClicked:(id)sender;
- (IBAction)enterButtonClicked:(id)sender;
- (IBAction)encryptButtonClicked:(id)sender;


- (void) updateTunnelStatus:(int)ip errorCode:(int) err;

- (void) handleLoginWithParam:(NSDictionary *)params;


- (void) showLoginProgress;

- (void) showLoginSuccess;

- (void) showTunnelOffline;

@end
