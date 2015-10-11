//
//  MdmCheck.m
//  MdmCheck
//
//  Created by Lion User on 27/11/2013.
//  Copyright (c) 2013 Lion User. All rights reserved.
//

#import "MdmCheck.h"

#import "svn_define.h"
#import "svn_socket_api.h"
#import "mdmsdk_api.h"

#define ASSERT_UNKNOWN 65535

enum
{
    MDMCONFIG_INSTALL_NO = 0,
    MDMCONFIG_INSTALL_YES
};

@implementation MdmCheck

/*****************************************************************************
 * 描述              ：根据证书，判断MDM配置文件是否安装
 * 本函数调用的函数    ：
 * 调用本函数的函数    ： 
 
 * 输入                ：void
 * 输出                ：无
 * 返回                ：MDMCONFIG_INSTALL_YES---MDM配置文件已经安装
                        MDMCONFIG_INSTALL_NO---MDM配置文件没有安装
 
 *****************************************************************************/
+ (NSInteger) isMdmConfigInstalled
{
    /* 获取客户端app中内置的client证书信息 */
    NSString * certPath = [[NSBundle mainBundle]
                           pathForResource:@"MDMCheckClient" ofType:@"cer"];
    NSData * certData = [NSData dataWithContentsOfFile:certPath];
    
    if ( nil == certData )
    {
        return MDMCONFIG_INSTALL_NO;
    }
    SecCertificateRef cert = SecCertificateCreateWithData(NULL, (CFDataRef) certData);
    SecPolicyRef policy = SecPolicyCreateBasicX509();
    
    /* 通过证书校验结果来验证MDM主配置文件中的CA证书是否存在 */
    SecTrustRef trust;
    SecTrustCreateWithCertificates(
                                                  (CFArrayRef) [NSArray arrayWithObject:(id)cert],
                                                  policy, &trust);
    
    
    SecTrustResultType trustResult = -1;
    SecTrustEvaluate(trust, &trustResult);
    
    CFRelease(trust);
    CFRelease(policy);
    CFRelease(cert);
    
    /* 如果trustResult == 4 说明证书存在 进而说明mdm主配置文件存在 */
    if(4 == trustResult)
    {
        // Profile installed
        return MDMCONFIG_INSTALL_YES;
    }
    else
    {
        // Profile not installed
        return MDMCONFIG_INSTALL_NO;
    }

}



+ (MDMCheckResult) checkMdmSpecific
{
    MDMCheckResult result;
    memset(&result, 0, sizeof(MDMCheckResult));
    result.isSuccess = SVN_OK;
    
    int mdmConfigInstalled = [MdmCheck isMdmConfigInstalled];
    if(MDMCONFIG_INSTALL_NO == mdmConfigInstalled)
    {
        result.isMDMEnabled = false;
        NSLog(@"checkMdmSpecific success:mdm not enabled");
        return result;
    }
    char *param = "MDM";
    int iRet = SVN_API_CheckBind(param, strlen(param));
    
    if (ASSERT_UNKNOWN == iRet)
    {
        NSLog(@"checkMdmSpecific failed:ASSERT_UNKNOWN");
        result.isSuccess = SVN_ERR;
        return result;
    }
    

    
    result.isMDMEnabled = true;
    result.bindResult = iRet;
    
    if(ASSERT_BINDED_BY_USER != iRet && ASSERT_BINDED_MULTIUSER != iRet)
    {
        return result;
    }
    
    char* pcViolationResult = NULL;
    unsigned long ulOutLen = 0;
    
    iRet = SVN_API_GetMdmViolationResult(&pcViolationResult, &ulOutLen);
    
    if (iRet != SVN_OK || NULL == pcViolationResult)
    {
        result.isSuccess = SVN_ERR;
        NSLog(@"checkMdmSpecific failed:violationResult null");
        return result;
    }
    
    NSError *error = nil;
    
    NSData *resultData = [NSData dataWithBytes:pcViolationResult length:ulOutLen];
    free(pcViolationResult);
    //{"webInfo":{"osverFlag":"","nopasswdFlag":"","decryptFlag":"","usbFlag":"","appNormalFlag":"","rootFlag":"","appNeedFlag":"","loginFlag":"","mdmcfgFlag":""},"vgInfo":{"errorCode":"2","dc":null}}
    
    
    // {"webInfo":{"osverFlag":false,"nopasswdFlag":false,"decryptFlag":false,"usbFlag":false,"appNormalFlag":false,"rootFlag":false,"appNeedFlag":false,"loginFlag":false,"mdmcfgFlag":false},"vgInfo":{"errorCode":"0","dc":null}}
    

    NSDictionary *resultDic = nil;
    NSDictionary *vgInfo = nil;
    NSDictionary *webInfo = nil;
    resultDic = [NSJSONSerialization JSONObjectWithData:resultData options:NSJSONReadingMutableLeaves error:&error];
    if(resultDic)
    {
        vgInfo = [resultDic objectForKey:@"vgInfo"];
        webInfo = [resultDic objectForKey:@"webInfo"];
    }
    
    if(vgInfo != nil && webInfo != nil)
    {
        
        NSString *errorCode = [vgInfo objectForKey:@"errorCode"];//getString("errorCode");
        
        if(errorCode != nil && [errorCode isEqualToString:@"0"])
        {
            NSString *osverFlag = [webInfo objectForKey:@"osverFlag"];
            NSString *nopasswdFlag = [webInfo objectForKey:@"nopasswdFlag"];
            NSString *decryptFlag = [webInfo objectForKey:@"decryptFlag"];
            NSString *usbFlag = [webInfo objectForKey:@"usbFlag"];
            NSString *appNormalFlag = [webInfo objectForKey:@"appNormalFlag"];
            NSString *rootFlag = [webInfo objectForKey:@"rootFlag"];
            NSString *appNeedFlag = [webInfo objectForKey:@"appNeedFlag"];
            //NSString *loginFlag = [webInfo objectForKey:@"loginFlag"];
            //boolean mdmcfgFlag = webInfo.getBoolean("mdmcfgFlag");
            if(osverFlag && nopasswdFlag && decryptFlag && usbFlag && appNormalFlag && rootFlag && appNeedFlag)
            {
                result.isRoot = [rootFlag boolValue];
                result.isPwdCheckOK = (![nopasswdFlag boolValue]);
                result.isAppCheckOK = (![appNormalFlag boolValue] && ![appNeedFlag boolValue]);
                //result.isLongTimeNoLogin = ([loginFlag boolValue]);
                result.isOtherCheckOK = (![osverFlag boolValue] && ![decryptFlag boolValue] && ![usbFlag boolValue]);
                NSLog(@"checkMdmSpecific violationResult:%d, %d, %d, %d, %d, %d,%d, %d", result.isSuccess, result.isMDMEnabled, result.bindResult, result.isRoot, result.isPwdCheckOK, result.isAppCheckOK, result.isLongTimeNoLogin, result.isOtherCheckOK);
                
                return result;
                
            }
            
          
        }

        
    }
 
    result.isSuccess = SVN_ERR;
    NSLog(@"checkMdmSpecific failed: info error");
    return result;
      
   
    

}

@end
