//
//  LoginAgent.h
//  anyofficesdk
//
//  Created by z00103873 on 14-7-4.
//  Copyright (c) 2014年 huawei. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LoginParam.h"

@protocol LoginDelegate;

@interface LoginAgent : NSObject


@property (nonatomic, assign) BOOL hasLogin;
@property (nonatomic, assign) BOOL tunnelCreated;
@property (nonatomic, assign) BOOL appAuthenticated;


+(LoginAgent*)getInstance;

-(void)loginAsync:(LoginParam*)loginParam delegate:(id <LoginDelegate>) delegate;

-(NSInteger)doAppAuthentication: (NSString*)authServerInternetAddress authServerIntranetAddress:(NSString*)authServerIntranetAddress;

-(NSInteger)doAppAuthentication;

-(void)doGatewayAuthenticationAsync:(LoginParam*)loginParam delegate:(id <LoginDelegate>) delegate;

-(NSString*)getAppPolicySync;

-(NSString*)getAppPolicySync:(BOOL)useSecureTransfer;

-(AnyOfficeSSOInfo*)getSSOInfo;

-(AnyOfficeUserInfo*)getUserInfo;

-(LoginParam*)getLoginParam;

-(NSString*)getEmailAddress;

-(SecCertificateRef*)getCertficate;

-(NSMutableArray*)getTittleOfCustomViews;

-(NSMutableArray*)getCustomViewControllers;

-(BOOL)addCustomSetting:(NSString*)tittle viewController:(UIViewController*)viewController;

-(SecIdentityRef*)getPrivateKey;

-(NSInteger)hiworkLoginWithAutoLogin:(AUTO_LOGIN_TYPE)autoLogin LoginParam:(LoginParam*)loginParam;

- (void)setChiErrorID:(int)errorID andErrorMsg:(NSString *)errorMsg;

- (void)setEngErrorID:(int)errorID andErrorMsg:(NSString *)errorMsg;

-(void)clearSSOSession;

/**
 * 登录错误码定义
 * 定义SDK认证相关的所有错误码
 *
 * @author  y90004712
 * @version  [版本号, 2014-7-7]
 * @see  [相关类/方法]
 * @since  [产品/模块版本]
 */
typedef enum {
    /**
     * 登录成功
     */
    LOGIN_OK = 0,
    
    /**
     * 认证成功(网关认证、应用验证)
     */
    AUTH_OK = 0,
    
    /**
     * 网络连接错误
     */
    LOGIN_ERR_NETWORK = 0x1,
    
    /**
     * 用户名密码错误
     */
    LOGIN_ERR_PASSWORD = 0x2,
    
    /**
     * 应用未验证
     */
    LOGIN_ERR_APP_NOTAUTH = 0x3,
    
    /**
     * 参数不全，需要配置
     */
    LOGIN_ERR_PARAM_UNCOMPLETE = 0x4,
    
    /**
     * 未知错误
     */
    LOGIN_ERR_UNKNOWN = 0x111,
    
}LOGIN_ERR_CODE;


@end

@protocol LoginDelegate <NSObject>

@optional

//收到登陆网关认证的结果后，调用该函数
-(void)receiveGatewayAuthenticationResult:(int)result;

@end
