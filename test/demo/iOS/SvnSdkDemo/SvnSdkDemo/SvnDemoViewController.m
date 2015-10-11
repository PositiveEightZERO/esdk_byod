//
//  SvnDemoViewController.m
//  SvnSdkDemo
//
//  Created by l00174413 on 13-6-19.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#include <sys/socket.h>
#include <arpa/inet.h>


//引入SDK头文件

#import <SvnSdk/SDKContext.h>
#import <SvnSdk/LoginParam.h>
#import <SvnSdk/LoginAgent.h>
#import <SvnSdk/NetStatusManager.h>
#import <SvnSdk/MdmDeviceIdInfo.h>
#import <SvnSdk/AppManager.h>
#import <SvnSdk/AppInfo.h>

#import <SvnSdk/svn_define.h>
#import <SvnSdk/svn_api.h>

#import <SvnSdk/anyoffice_keyspace.h>

#import "SvnDemoViewController.h"

#import "UIHelper.h"

#import "MenuSectionInfo.h"
#import "MenuViewController.h"
#import "FaqsViewController.h"
#import "UIView+Toast.h"


@implementation SvnDemoViewController
{
    CGRect operateViewFrame;
}

-(void)onNetChangedWithOldStatus:(NET_STATUS_EN)oldStatus newStatus:(NET_STATUS_EN)newStatus errCode:(int)error
{
    NSLog(@"old status:%d, new status:%d, errorcode:%d", oldStatus, newStatus, error);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [self updateTunnelStatus:newStatus errorCode:error];
    });
    

}

- (void)updateTunnelStatus:(int) uiStatus errorCode:(int)err
{
    
    
    /*----------------------------SVN隧道状态定义-----------------------------*/
    /* SVN隧道状态标准定义（适用于除Windows外的各平台） */
    //#define SVN_STATUS_OFFLINE              0x0       /* 表示隧道不可用 */
    //#define SVN_STATUS_ONLINE               0x1       /* 表示隧道在线   */
    //#define SVN_STATUS_CONNECTING           0x2       /* 表示处于登录或注销隧道过程中，或正在进行隧道重连 */
    
    if(uiStatus == SVN_STATUS_ONLINE)
    {
        [self showLoginSuccess];
        NSLog(@"tunnel online");
        unsigned long ipAddress = 0;
        unsigned long mask = 0;
        int iret = SVN_API_GetTunnelIP(&ipAddress, &mask);
        if(iret == SVN_OK)
        {
            int ip32 = ((ipAddress &0xff000000) >> 24) & 0x000000ff;
            int ip24 = (ipAddress &0x00ff0000) >> 16;
            int ip16 = (ipAddress &0x0000ff00) >> 8;
            int ip8 = ipAddress &0x000000ff;
            
            NSString * ip = [NSString stringWithFormat:@"%d.%d.%d.%d", ip32, ip24, ip16, ip8];
            
            NSLog(@"%@", ip);
            
            [self.tunnelIPTextFiled setText:ip];
            
        }
    }
    else
    {
        [self.tunnelIPTextFiled  setText:@"tunnel offline"];
        [self showTunnelOffline];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
//    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:self.scrollView
//                                                                      attribute:NSLayoutAttributeLeading
//                                                                      relatedBy:0
//                                                                         toItem:self.view
//                                                                      attribute:NSLayoutAttributeLeft
//                                                                     multiplier:1.0
//                                                                       constant:0];
//    [self.view addConstraint:leftConstraint];
//    
//    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:self.scrollView
//                                                                       attribute:NSLayoutAttributeTrailing
//                                                                       relatedBy:0
//                                                                          toItem:self.view
//                                                                       attribute:NSLayoutAttributeRight
//                                                                      multiplier:1.0
//                                                                        constant:0];
//    [self.view addConstraint:rightConstraint];
//    
    
    float height = self.view.frame.size.height > self.view.frame.size.width ? self.view.frame.size.height: self.view.frame.size.width;
    
    height -= self.navigationController.navigationBar.frame.size.height;
    height -= [UIApplication sharedApplication].statusBarFrame.size.height;
    
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:self.contentView
                                                                        attribute:NSLayoutAttributeBottom
                                                                        relatedBy:0
                                                                           toItem:self.scrollView
                                                                        attribute:NSLayoutAttributeTop
                                                                       multiplier:1.0
                                                                         constant:height];
    [self.scrollView addConstraint:bottomConstraint];


    
    UIEdgeInsets insets = UIEdgeInsetsMake(20, 10, 20, 10);
    // 指定为拉伸模式，伸缩后重新赋值

    UIImage *imageEmpty = [[UIImage imageNamed:@"btn_background_empty.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    
    UIImage *imageSelected = [[UIImage imageNamed:@"btn_background_selected.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    [self.loginButton setBackgroundImage:imageEmpty forState:UIControlStateNormal];
    [self.loginButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.faqsButton setBackgroundImage:imageEmpty forState:UIControlStateNormal];
    [self.faqsButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    UIImage *imageEncrypt = [UIImage imageNamed:@"btn_encrypt_decrypt.png"] ;
    UIImage *imageEncryptelected= [UIImage imageNamed:@"btn_encrypt_decrypt_selected.png"] ;
    [self.encryptButton setImage:imageEncrypt forState:UIControlStateNormal];
    [self.encryptButton setImage:imageEncryptelected forState:UIControlStateHighlighted];

  
	//tapRecognizer for dismiss keyboard.
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc]
                                             initWithTarget:self action:@selector(tapOnView:)];
    [self.view addGestureRecognizer: tapRecognizer];
    
    
    
//    [self.gatewayTextFiled setText:@"10.170.119.67"];
//    [self.usernameTextFiled setText:@"lzy"];
//    [self.passwordTextFiled setText:@"Admin@123"];
    

//    [self.gatewayTextFiled setText:@"58.60.106.145"];
//    [self.usernameTextFiled setText:@"lzy"];
//    [self.passwordTextFiled setText:@"Admin@123"];
    
    //[self loginButtonClicked:self.loginButton];

    
}


-(void)tapOnView:(UITapGestureRecognizer *)tapRecognizer
{
    [[self view] endEditing: YES];
}


- (void)viewDidUnload
{
    
    
    [self setScrollView:nil];
    
    [self setGatewayTextFiled:nil];
    [self setUsernameTextFiled:nil];
    [self setPasswordTextFiled:nil];
    [self setTunnelIPTextFiled:nil];
    
    
    
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

-(void)viewDidLayoutSubviews
{
    
    
    self.contentView.frame = CGRectMake(self.contentView.frame.origin.x
                                        , self.contentView.frame.origin.y, self.contentView.frame.size.width, self.view.frame.size.height);
    
    //NSLog(@"view size:%f, %f", self.view.frame.size.width, self.view.frame.size.height);
    
    NSLog(@"contentView size:%f, %f, origin:%f, %f", self.contentView.frame.size.width, self.contentView.frame.size.height, self.contentView.frame.origin.x, self.contentView.frame.origin.y);
    
    [self.contentView layoutIfNeeded];
    
    [super viewDidLayoutSubviews];

    
}

- (void)viewDidAppear:(BOOL)animated
{
    
    [super viewDidAppear:animated];
    
    
    self.contentView.frame = CGRectMake(self.contentView.frame.origin.x
                                        , self.contentView.frame.origin.y, self.contentView.frame.size.width, self.view.frame.size.height);

    
    [self.contentView layoutIfNeeded];
    
   
    NSLog(@"viewDidAppear contentView size:%f, %f, origin:%f, %f", self.contentView.frame.size.width, self.contentView.frame.size.height, self.contentView.frame.origin.x, self.contentView.frame.origin.y);
    
    
}

- (void)viewWillAppear:(BOOL)animated
{
//    self.contentView.frame = CGRectMake(self.contentView.frame.origin.x
//                                        , self.contentView.frame.origin.y, self.contentView.frame.size.width, self.view.frame.size.height);
//    
//    NSLog(@"view size:%f, %f", self.view.frame.size.width, self.view.frame.size.height);
//    
//    NSLog(@"contentView size:%f, %f, origin:%f, %f", self.contentView.frame.size.width, self.contentView.frame.size.height, self.contentView.frame.origin.x, self.contentView.frame.origin.y);
//    
//    [self.contentView layoutIfNeeded];
}

- (IBAction)loginButtonClicked:(id)sender {
    
    NSString* gatewayIP = [self.gatewayTextFiled text];
    NSString* username = [self.usernameTextFiled text];
    NSString* pwd = [self.passwordTextFiled text];
    
    
    NSArray* tmpPathArray = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* tmpDocumentsDir = [tmpPathArray objectAtIndex:0];
    NSLog(@"Document Path:%@", tmpDocumentsDir);
    
    
    [self showLoginProgress];
   
    
    //注册网络状态通知
    [[NetStatusManager getInstance] initWithDelegate:self];
    
    //初始化SDK
    [[SDKContext getInstance] init:tmpDocumentsDir];
 

    
    //设置登录验证参数
    
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    LoginParam *loginParam = [[LoginParam alloc] initWithServiceType:bundleIdentifier andUseSecTrans:YES];
    //不设置登录参数，从AnyOffce获取，获取不到将界面输入
//    [loginParam setInternetAddress:gatewayIP];
//    loginParam.userInfo = [[AnyOfficeUserInfo alloc] initWithDomain:nil username:username password:pwd];
    loginParam.loginBackgroud = NO;
    loginParam.loginType = AUTO_LOGIN_ENABLE;
    
    //登录验证
    [[LoginAgent getInstance] loginAsync:loginParam delegate:self];
    
    
    //[[LoginAgent getInstance] doGatewayAuthenticationAsync:loginParam delegate:self];

    
//        NSString* deviceId = [MdmDeviceIdInfo getDeviceID];
//        NSLog(@"deviceId:%@", deviceId);
    
}

- (IBAction)logoutButtonClicked:(id)sender {
    
    
     SVN_API_DestroyTunnel();
    
     [[SDKContext getInstance] reset];
    
     [self showTunnelOffline];
}


//收到登陆网关认证的结果后，调用该函数
-(void)receiveGatewayAuthenticationResult:(int)result
{
    NSLog(@"receiveGatewayAuthenticationResult:%d", result);
    
    NSLog(@"tunnelCreated:%d", [[LoginAgent getInstance] tunnelCreated]);
    
    if(result != 0)
    {
        [self showTunnelOffline];
    }
    
    NSString *msg = [NSString stringWithFormat:@"GatewayAuthentication Result:%d", result ];
    

    [self.view makeToast:msg duration:3.0 position:CSToastPositionBottom];
    
    
    //获取SSO信息
  
    
    LoginParam* loginParam = [[LoginAgent getInstance] getLoginParam];
    if(loginParam)
    {
        NSLog(@"svn gateway address :%@", loginParam.internetAddress);
        
        
        AnyOfficeUserInfo* userInfo = loginParam.userInfo;
        if(userInfo)
        {
            NSLog(@"userInfo username:%@, password:%@", userInfo.userName, userInfo.password);
        }
        else
        {
            NSLog(@"userInfo empty");
        }
    }
    else
    {
        NSLog(@"loginParam empty");
    }
    
    

    
    
    AnyOffice_KeySpace_API_WriteItem("com.huawei.anyoffice.keyspace", "name1", "value1");
    
    AnyOffice_KeySpace_API_WriteItem("com.huawei.anyoffice.keyspace", "name2", "value2");
 
}

- (IBAction)checkUpdateButtonClicked:(id)sender {
    
    
    NSString *bundleVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString*)kCFBundleVersionKey];
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    //检查是否有更新
    AppInfo * appInfo = [[AppManager getInstance] checkUpdateWithAppid:bundleIdentifier andCurrentVersion:bundleVersion];
    NSLog(@"appInfo:%@", appInfo);//appInfo.packageURL
    
    //有更新，应用商店安装
    if(appInfo)
    {
        NSLog(@"app:%@ has update, new version:%@!, packageURL:%@", bundleIdentifier, appInfo.appVersion, appInfo.packageURL);
        //anyoffice://main?srcUrl=AnyMail://&pageName=AppStore&detailId=com.huawei.anyoffice.anymail
        
        NSString* packageURL = [NSString stringWithFormat:@"anyoffice://main?srcUrl=%@://&pageName=AppStore&detailId=%@", @"esdkdemo", bundleIdentifier ];
        [[AppManager getInstance] updateAppWithPackageUrl:packageURL andAppid:bundleIdentifier];
    
    }
    
    
}







- (void) handleLoginWithParam:(NSDictionary *)params
{
    //@"esdkdemo://auth?source=Huawei&user_name=xxx&password=xxx&SvnServer=xxx&SvnServerBackup=xxx&SrcAppScheme=anyoffice%3A%2F%2F";
//    NSString *server = [params valueForKey:@"SvnServer"];
//    NSString *username = [params valueForKey:@"user_name"];
//    NSString *password = [params valueForKey:@"password"];
//    
//    if(server && username && password)
//    {
//        [self.usernameTextFiled setText:username];
//        [self.passwordTextFiled setText:password];
//        [self.gatewayTextFiled setText:server];
//        
//        [self loginButtonClicked:nil];
//    }
    
}


- (void) showLoginProgress
{
    [[NSBundle mainBundle] loadNibNamed:@"LogingView" owner:self options:nil];

    self.logingView.frame = self.operateView.frame;
    
    self.logingView.hidden = NO;
  
    self.operateView.hidden = YES;
    
    [self.contentView addSubview:self.logingView];
    
}


- (void) showLoginSuccess;
{
    self.inputView.hidden = TRUE;
    self.operateView.hidden = TRUE;
    
    [self.logingView removeFromSuperview];
     
    [[NSBundle mainBundle] loadNibNamed:@"LoginSuccessView" owner:self options:nil];
    
    
    self.loginSuccessView.frame = CGRectMake(self.loginSuccessView.frame.origin.x
                                             , self.loginSuccessView.frame.origin.y, self.inputView.frame.size.width, self.loginSuccessView.frame.size.height);
    
    UIEdgeInsets insets = UIEdgeInsetsMake(20, 10, 20, 10);
    // 指定为拉伸模式，伸缩后重新赋值
    
    UIImage *imageEmpty = [[UIImage imageNamed:@"btn_background_empty.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    
    UIImage *imageSelected = [[UIImage imageNamed:@"btn_background_selected.png"] resizableImageWithCapInsets:insets resizingMode:UIImageResizingModeStretch];
    [self.enterButton setBackgroundImage:imageEmpty forState:UIControlStateNormal];
    [self.enterButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    
    [self.logoutButton setBackgroundImage:imageEmpty forState:UIControlStateNormal];
    [self.logoutButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];
    
    [self.checkUpdateButton setBackgroundImage:imageEmpty forState:UIControlStateNormal];
    [self.checkUpdateButton setBackgroundImage:imageSelected forState:UIControlStateHighlighted];

    [self.contentView addSubview:self.loginSuccessView];
    


}


- (void) showTunnelOffline
{
    self.inputView.hidden = NO;
    self.operateView.hidden = NO;
    
    [self.logingView removeFromSuperview];
    self.logingView.hidden = YES;
    
    [self.loginSuccessView removeFromSuperview];
    

  
}


- (IBAction)enterButtonClicked:(id)sender {
    [self performSegueWithIdentifier:@"display_menu" sender:sender];
}

- (IBAction)encryptButtonClicked:(id)sender {
    
    [self performSegueWithIdentifier:@"display_menu_encrypt" sender:sender];
}


- (void)initEnctyptEnv
{
    static BOOL hasInitEnctyptEnv = NO;
    if (hasInitEnctyptEnv) {
        return;
    }

    
    NSString* gatewayIP = [self.gatewayTextFiled text];
    NSString* username = [self.usernameTextFiled text];

    
    NSString *documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSLog(@"Document Path:%@",documentPath);

    [[SDKContext getInstance] init:username andWorkPath:documentPath];

    [[LoginAgent getInstance] doAppAuthentication:gatewayIP authServerIntranetAddress:nil];
    
    hasInitEnctyptEnv = YES;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    
    if ([[segue identifier] isEqualToString:@"faqs"])
    {
        FaqsViewController *faqsViewController =  [segue destinationViewController];
        faqsViewController.faqsType = 1;
        return;
        
    }
    
    
    [self initEnctyptEnv];
    
    NSMutableArray *sections = [[NSMutableArray alloc] init];
        
    NSMutableArray * menuItems;
    MenuItem *item;
      
    if ([[segue identifier] isEqualToString:@"display_menu"])
    {
        
        MenuSectionInfo *httpSection = [[MenuSectionInfo alloc] init];
        httpSection.sectionName = @"HTTP Test";
        menuItems =[[NSMutableArray alloc] init];
        
        item = [[MenuItem alloc] init];
        item.itemName = @"NSURLConnection Test";
        item.itemAction = @"menu_nsurlconnection";
        [menuItems addObject:item];
        
        item = [[MenuItem alloc] init];
        item.itemName = @"ASIHTTPRequest Test";
        item.itemAction = @"menu_asihttprequest";
        [menuItems addObject:item];
        
        item = [[MenuItem alloc] init];
        item.itemName = @"UIWebView Test";
        item.itemAction = @"menu_webview";
        [menuItems addObject:item];
        
        item = [[MenuItem alloc] init];
        item.itemName = @"Speed Compare Test";
        item.itemAction = @"menu_speed_compare";
        [menuItems addObject:item];
        
        httpSection.menuItems = menuItems;
        [sections addObject:httpSection];
        
    }
    
    
    MenuSectionInfo *encrpytSection = [[MenuSectionInfo alloc] init];
    encrpytSection.sectionName = @"Encrypt/Decrpty Test";
    menuItems =[[NSMutableArray alloc] init];
    
    item = [[MenuItem alloc] init];
    item.itemName = @"File Encrypt/Decrypt";
    item.itemAction = @"menu_file_encrypt";
    [menuItems addObject:item];
    
    item = [[MenuItem alloc] init];
    item.itemName = @"Data Encrypt/Decrypt";
    item.itemAction = @"menu_data_encrypt";
    [menuItems addObject:item];
    
    item = [[MenuItem alloc] init];
    item.itemName = @"SQLite Encrypt/Decrypt Test";
    item.itemAction = @"menu_sqlite";
    [menuItems addObject:item];
    
    
    item = [[MenuItem alloc] init];
    item.itemName = @"Other Encrypt/Decrypt Test";
    item.itemAction = @"menu_other_encrypt";
    [menuItems addObject:item];
    
    encrpytSection.menuItems = menuItems;
    [sections addObject:encrpytSection];
    
    if ([[segue identifier] isEqualToString:@"display_menu"])
    {
    
        MenuSectionInfo *mdmSection = [[MenuSectionInfo alloc] init];
        mdmSection.sectionName = @"MDM Test";
        menuItems =[[NSMutableArray alloc] init];
        
        item = [[MenuItem alloc] init];
        item.itemName = @"MDM Check Test";
        item.itemAction = @"menu_mdm_check";
        [menuItems addObject:item];
        
      
        mdmSection.menuItems = menuItems;
        [sections addObject:mdmSection];
        
    }
    
    
    MenuViewController *menuViewController =  [segue destinationViewController];
    menuViewController.sections = sections;
    
}
@end
