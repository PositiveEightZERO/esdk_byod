//
//  LoginParam.h
//  anyofficesdk
//
//  Created by z00103873 on 14-7-4.
//  Copyright (c) 2014年 pangqi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AnyOfficeUserInfo.h"
#import "AnyOfficeSSOInfo.h"

typedef enum {
    AUTO_LOGIN_DISABLE = 0, //非自动登陆
    AUTO_LOGIN_ENABLE,      //自动登陆
    AUTO_LOGIN_DEFAULT = 999      //默认，由SDK内部决定是否自动登陆
}AUTO_LOGIN_TYPE;

typedef enum {
    CONNECT_TYPE_SDK = 0,
    CONNECT_TYPE_ANYOFFICE,
    CONNECT_TYPE_MTM,
    CONNECT_TYPE_E2E,
    CONNECT_TYPE_DEFAULT = 999
}CONNECT_TYPE;

@interface LoginParam : NSObject <NSCopying> {

}

@property(nonatomic, strong) AnyOfficeUserInfo* userInfo;
@property(nonatomic, strong) AnyOfficeSSOInfo* ssoInfo;
@property(strong, nonatomic) NSString* serviceType;
@property(nonatomic, assign) BOOL useSecureTransfer;
@property(nonatomic, strong) NSString* internetAddress;
@property(nonatomic, strong) NSString* intranetAddress;
@property(nonatomic, assign) BOOL loginBackgroud; //是否后台登陆
@property(nonatomic, assign) AUTO_LOGIN_TYPE loginType;  //登陆类型：自动登陆，非自动登陆，默认
@property(nonatomic, assign) CONNECT_TYPE connectType;
@property(nonatomic, assign) BOOL authGateway;

-(id)initWithServiceType:(NSString *) srvType andUseSecTrans:(BOOL)usel4;


@end
