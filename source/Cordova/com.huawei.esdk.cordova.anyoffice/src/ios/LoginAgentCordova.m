#import "LoginAgentCordova.h"

#import <SvnSdk/LoginAgent.h>
#import <SvnSdk/LoginParam.h>
#import <SvnSdk/AnyOfficeUserInfo.h>
@interface LoginAgentCordova()<LoginDelegate>
{
    dispatch_semaphore_t sema;
    NSString* callbackId;
    int loginResult;
}

@end

@implementation LoginAgentCordova


- (void)login:(CDVInvokedUrlCommand*)command
{

    callbackId = [command callbackId];
    
    NSString* username = [[command arguments] objectAtIndex:0];
    NSString* password = [[command arguments] objectAtIndex:1];

    NSString* gateway = [[command arguments] objectAtIndex:2];
    
    
    
    //设置登录验证参数
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    LoginParam *loginParam = [[LoginParam alloc] initWithServiceType:bundleIdentifier andUseSecTrans:YES];
    [loginParam setInternetAddress:gateway];
    loginParam.userInfo = [[AnyOfficeUserInfo alloc] initWithDomain:nil username:username password:password];
    loginParam.loginBackgroud = YES;
    loginParam.loginType = AUTO_LOGIN_ENABLE;
    
    loginResult = -1;
    //sema = dispatch_semaphore_create(0);

    
    //登录验证
    [[LoginAgent getInstance] loginAsync:loginParam delegate:self];
    
    //dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);

    //sema = NULL;

   
    
    
}


//收到登陆网关认证的结果后，调用该函数
-(void)receiveGatewayAuthenticationResult:(int)result;
{
    NSLog(@"Cordova receiveGatewayAuthenticationResult:%d", result);
    loginResult = result;
    
    
    CDVPluginResult* pluginResult;
    
    if(loginResult == 0)
    {
        pluginResult = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_OK
                  messageAsInt:0];
        
        
    }
    else
    {
        pluginResult = [CDVPluginResult
                  resultWithStatus:CDVCommandStatus_ERROR
                  messageAsInt:loginResult ];
        
    }
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:callbackId];
    
    
    //dispatch_semaphore_signal(sema);
}
@end