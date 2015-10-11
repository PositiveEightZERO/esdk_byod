//
//  SSOUrlSchema.m
//  SvnSdk
//
//  Created by l00174413 on 14/10/29.
//  Copyright (c) 2014年 huawei. All rights reserved.
//





#import "SSOUrlSchema.h"
#import "LoginAgent.h"


 NSString* const Anyoffice_Login_Info_Version           = @"version";
 NSString* const Anyoffice_Login_Info_Source            = @"source";
 NSString* const Anyoffice_Login_Info_UrlEncoded        = @"UrlEncoded";

 NSString* const Anyoffice_Login_Info_User_Name         = @"user_name";
 NSString* const Anyoffice_Login_Info_Password          = @"password";
 NSString* const Anyoffice_Login_Info_SVN_Server        = @"SvnServer";

 NSString* const Anyoffice_Login_Info_SVN_Server_Backup = @"SvnServerBackup";
 NSString* const Anyoffice_Login_Info_SVN_Params        = @"SvnParams";
 NSString* const Anyoffice_Login_Info_App_Scheme        = @"SrcAppScheme";

 NSString* const Anyoffice_Login_Info_Return_Code       = @"ReturnCode";
 NSString* const Anyoffice_Login_Info_TerminalID        = @"TerminalID";



@interface SSOUrlSchema()
{
    
//    NSString* _source;
//
//    NSString* _urlEncoded;
//
//    NSString* _username;
//
//    NSString* _password;
//
//    NSString* _svnServer;
//
//    NSString* _svnServerBackup;
//
//    NSString* _svnParams;
//
//    NSString* _srcAppScheme;
//
//    NSString* _returnCode;
//
//    NSString* _terminalID;
//
//    NSString* _version;
    
    NSString* _scheme;
    
    NSMutableDictionary *_params;
}

@end



@implementation SSOUrlSchema



#pragma mark
#pragma mark interfaces

/**
 * 拉起其他应用的urlSchema，传入参数
 */
-(id)initWithSchema:(NSString *)schema andParam:(NSDictionary*) params
{
    self = [super init];
    
    if(self)
    {
        _scheme = schema;
        _params = [[NSMutableDictionary alloc] initWithDictionary:params];
        
        LoginParam * loginParam = [[LoginAgent getInstance] getLoginParam];
        if(loginParam)
        {
            [_params setObject:loginParam.internetAddress forKey:Anyoffice_Login_Info_SVN_Server];
            
            if(loginParam.userInfo)
            {
                [_params setObject:loginParam.userInfo.userName forKey:Anyoffice_Login_Info_User_Name];
                [_params setObject:loginParam.userInfo.password forKey:Anyoffice_Login_Info_Password];
            }
            
        }
       
    }
   
    return self;
}

/**
 * 解析拉起本应用的urlSchema，获取单点登录信息
 */
-(id)initWithUrlSchema:(NSString *)urlSchema
{
    self = [super init];
    
    if(self)
    {
        _params = [[NSMutableDictionary alloc] initWithCapacity:10];
        // 传递的参数编解码处理
        NSURL *url = [NSURL URLWithString:urlSchema];
        
        //解析从Anyoffice传过来的url
        _scheme = [url scheme];
        NSString *strQuery = [url query];
        NSArray *arrayParam = [strQuery componentsSeparatedByString:@"&"];
        
        if ([arrayParam count] > 0)
        {
            //将参数保存到结构体中
            NSRange range;
            NSString *key = nil;
            NSString *value = nil;
            
            
            for (NSString* obj in arrayParam)
            {
                //区分出key和value
                range = [obj rangeOfString:@"=" options:NSLiteralSearch];
                if (range.location != NSNotFound)
                {
                    key = [obj substringToIndex:range.location];
                    value = [[obj substringFromIndex:range.location+1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                    
                    [_params setObject:value forKey:key];
                    
//                    if ([key isEqualToString:Anyoffice_Login_Info_Source]) {
//                        _source = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_UrlEncoded]){
//                        _urlEncoded = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_User_Name]){
//                        _username = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_Password]){
//                        _password = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_SVN_Server]){
//                        _svnServer = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_SVN_Server_Backup]){
//                        _svnServerBackup = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_SVN_Params]){
//                        _svnParams = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_App_Scheme]){
//                        _srcAppScheme = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_Return_Code]){
//                        _returnCode = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_TerminalID]){
//                        _terminalID = value;
//                    }
//                    else if ([key isEqualToString:Anyoffice_Login_Info_Version]){
//                        _version = value;
//                    }
//                    else
//                    {
//                        NSLog(@"unknown param:%@", key);
//                    }
                }
                
            }
            
            
        }
        

    }
    
    return self;
}


/**
 * urlScheme封装完成后，通过此接口获取封装好的urlScheme
 */
-(NSString *)getUrlSchema;
{
    NSMutableString * urlScheme = [[NSMutableString alloc] init];
  
    [urlScheme appendString:_scheme];
    
    if(![_scheme hasSuffix:@"?"])
    {
        NSRange range = [_scheme rangeOfString:@"://"];
        if(range.location == NSNotFound)
        {
            [urlScheme appendString:@"://"];
        }
        
        [urlScheme appendString:@"?"];
        
    }
    
    
    
    
    for(NSString * key in [_params allKeys])
    {
        
        [urlScheme appendFormat:@"%@=%@&", key, [_params objectForKey:key] ];
        
        
    }
    
    return [urlScheme stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
}


/**
 * 获取用户名
 * 获取应用拉起时传递的用户名信息
 */
-(NSString*)getUsername
{
    return [_params objectForKey:Anyoffice_Login_Info_User_Name];
}
/**
 * 获取密码
 * 获取应用拉起时传递的密码信息。
 * @return
 * @see [类、类#方法、类#成员]
 */
-(NSString*)getPassword
{
    return [_params objectForKey:Anyoffice_Login_Info_Password];
}
/**
 * 获取SVN网关地址
 * 获取应用拉起时传递的SVN网关地址信息
 * @return
 * @see [类、类#方法、类#成员]
 */
-(NSString*) getServerAddress
{
    return [_params objectForKey:Anyoffice_Login_Info_SVN_Server];
}

/**
 * 封装明文参数
 * 将参数明文封装到urlScheme中。
 * @name         参数key
 * @paramValue   参数值
 */
- (void) setParam: (NSString *) name paramValue: (NSString *)value
{
    [_params setObject:value forKey:name];
}
/**
 * 获取明文参数
 * 从urlScheme中解析明文参数
 * @name    参数key
 * @return  参数值
 */
- (NSString *) getParam: (NSString *) name
{
    return [_params objectForKey:name];
}
@end
