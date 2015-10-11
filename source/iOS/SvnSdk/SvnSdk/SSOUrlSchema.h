//
//  SSOUrlSchema.h
//  SvnSdk
//
//  Created by l00174413 on 14/10/29.
//  Copyright (c) 2014年 huawei. All rights reserved.
//

#import <Foundation/Foundation.h>


extern NSString* const Anyoffice_Login_Info_Version;//    @"version"
extern NSString* const Anyoffice_Login_Info_Source;//     @"source"
extern NSString* const Anyoffice_Login_Info_UrlEncoded;// @"UrlEncoded"

extern NSString* const Anyoffice_Login_Info_User_Name;//  @"user_name"
extern NSString* const Anyoffice_Login_Info_Password;//   @"password"
extern NSString* const Anyoffice_Login_Info_SVN_Server;// @"SvnServer"

extern NSString* const Anyoffice_Login_Info_SVN_Server_Backup;//  @"SvnServerBackup"
extern NSString* const Anyoffice_Login_Info_SVN_Params;// @"SvnParams"
extern NSString* const Anyoffice_Login_Info_App_Scheme;// @"SrcAppScheme"

extern NSString* const Anyoffice_Login_Info_Return_Code;//    @"ReturnCode"
extern NSString* const Anyoffice_Login_Info_TerminalID;// @"TerminalID"

/**
 * 应用拉起时的参数封装
 * 通过SDK拉起应用时，传递的参数由本类定义。传递方式由SDK定义，上层应用不应该对此存在依赖。
 */
@interface SSOUrlSchema: NSObject

/**
 * 拉起其他应用的urlSchema，传入参数
 */
-(id)initWithSchema:(NSString *)schema andParam:(NSDictionary*) params;


/**
 * 解析拉起本应用的urlSchema，获取单点登录信息
 */
-(id)initWithUrlSchema:(NSString *)urlSchema;

/**
 * urlScheme封装完成后，通过此接口获取封装好的urlScheme
 */
-(NSString *)getUrlSchema;


/**
 * 获取用户名
 * 获取应用拉起时传递的用户名信息
 */
-(NSString*)getUsername;
/**
 * 获取密码
 * 获取应用拉起时传递的密码信息。
 * @return
 * @see [类、类#方法、类#成员]
 */
-(NSString*)getPassword;
/**
 * 获取SVN网关地址
 * 获取应用拉起时传递的SVN网关地址信息
 * @return
 * @see [类、类#方法、类#成员]
 */
-(NSString*) getServerAddress;
/**
 * 获取单点登录的token
 * 获取应用拉起时传递的单点登录token信息
 * @return
 * @see [类、类#方法、类#成员]
 */
//-(SSOInfo*)getSsoInfo;

/**
 * 封装明文参数
 * 将参数明文封装到urlScheme中。
 * @name         参数key
 * @paramValue   参数值
 */
- (void) setParam: (NSString *) name paramValue: (NSString *)value;
/**
 * 获取明文参数
 * 从urlScheme中解析明文参数
 * @name    参数key
 * @return  参数值
 */
- (NSString *) getParam: (NSString *) name;

@end


