/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_login.h
  版 本 号   : 初稿
  作    者   : LiJingjin 90004727
  生成日期   : 2014年5月6日
  最近修改   :
  功能描述   : 连接管理对外接口
  函数列表   :
*
*
  修改历史   :
  1.日    期   : 2014年5月6日
    作    者   : LiJingjin 90004727
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ANYOFFICE_LOGIN_H__
#define __ANYOFFICE_LOGIN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*----------------------------------------------*
 * 头文件                                       *
 *----------------------------------------------*/
#include "tools_hash.h"
#include "tools_common.h"
#include "svn_define.h"


/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define ANYOFFICE_SDK_APPMODULE_ANYOFFICE   "home"
#define ANYOFFICE_SDK_APPMODULE_MAIL        "mail"
#define ANYOFFICE_SDK_APPMODULE_BROWSER     "browser"


/*----------------------------------------------*
 * 数据结构定义                                 *
 *----------------------------------------------*/

/* 初始化业务时传入的单点登陆信息，该结构中password为明文，不允许在内存中长驻 */
typedef struct tagAnyOfficeUserInfo
{
    CHAR *pcDomainName;
    CHAR *pcUserName;
    CHAR *pcPassword;
}ANYOFFICE_USER_INFO_S;

/* 初始化业务时传入的单点登陆信息,该结构中token未明文，不允许在内存中长驻 */
typedef struct tagAnyOfficeSSOInfo
{
    CHAR *pcToken;
}ANYOFFICE_SSO_INFO_S;

typedef struct tagAnyOfficeGatewayInfo
{
    CHAR *pcInternetAddress;
    ULONG ulInternetPort;
    CHAR *pcIntranetAddress;
    ULONG ulIntranetPort;
}ANYOFFICE_GATEWAY_INFO_S;


#define ANYOFFICE_AUTOLOGIN_DEFAULT 999
#define ANYOFFICE_CONNECTTYPE_DEFAULT 999
typedef struct tagAnyofficeLoginParam
{
    CHAR  *pcServiceType;
    ULONG ulUseSecureTransfer;
    ULONG ulAutoLoginType;   /* 1 enable ANYOFFICE_ENABLE , 0 disable  ANYOFFICE_DISABLE */
    ANYOFFICE_GATEWAY_INFO_S *pstGatewayInfo;
    ANYOFFICE_USER_INFO_S    *pstUserInfo;
    ANYOFFICE_SSO_INFO_S     *pstSSOInfo;
	ULONG ulConnectType;
    ULONG ulAuthGateway;
}ANYOFFICE_LOGIN_PARAM_S;

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
/*
    同步接口，实现对集成SDK的应用的验证，只有通过验证才能正常使用SDK功能
*/
INT32 AnyOffice_API_DoAppAuthentication(ANYOFFICE_GATEWAY_INFO_S *pstGateway);

INT32 AnyOffice_API_DoGatewayAuthentication_Sync(
            ULONG ulAutoLogin,
            ANYOFFICE_LOGIN_PARAM_S *pstLoginParam);

INT32 AnyOffice_API_GetGatewayAuthenticationResult(HASH_S **ppstHash);

/*
* 到网关上查询用户策略，根据是否使用安全传输的配置和策略决定是否创建隧道
*/
#define ANYOFFICE_SECURE_TRANSFER_DISABLE 0
#define ANYOFFICE_SECURE_TRANSFER_ENABLE  1
INT32 AnyOffice_API_GetAppPolicy_Sync(
            ULONG ulUseSecureTransfer, CHAR **ppcPolicy);

INT32 AnyOffice_API_GetEmailAddress(CHAR **ppcEnailAddress);

/*
*  查询会话信息
*/
INT32 AnyOffice_API_GetSSOInfo(ANYOFFICE_SSO_INFO_S **ppstSSOInfo);

/*
*  查询用户信息
*/
INT32 AnyOffice_API_GetUserInfo(ANYOFFICE_USER_INFO_S **pstUserInfo);

/*
*  查询网关信息
*/
INT32 AnyOffice_API_GetGatewayInfo(ANYOFFICE_GATEWAY_INFO_S **ppstGatewayInfo);

/*
*  获取网络接入类型， 返回当前是内网还是外网
*/
#define ANYOFFICE_INNER_NETWORK     1      /* 当前终端处于内网 */
#define ANYOFFICE_OUTER_NETWORK     2      /* 当前终端处于外网 */
INT32 AnyOffice_API_GetNetworkAccessType();

/*
*  查询SDK保存的登录参数
*/
INT32 AnyOffice_API_GetLoginParam(ANYOFFICE_LOGIN_PARAM_S** ppstLoginParam);

VOID AnyOffice_API_FreeLoginParam(ANYOFFICE_LOGIN_PARAM_S* pstLoginParam);


/*
*  策略解析
*/
typedef long (*ANYOFFICE_SDK_POLICY_CALL_BACK)(CHAR *pLableValue,int iValueLen);

typedef struct tagAnyOfficeSDKLableTOFunc
{
    ANYOFFICE_SDK_POLICY_CALL_BACK pPolicyCallback;         /*回调函数*/
    CHAR *pcLableName;                                              /*标签名*/
}ANYOFFICE_SDK_POLICY_FUNC;

INT32 AnyOffice_SDK_API_ParseAndSavePolicy(const CHAR* pcXmlStr,
                        ANYOFFICE_SDK_POLICY_FUNC CALL_BACK[], int iCallFuncCnt);

typedef long (*ANYOFFICE_SDK_SESSION_UPDATE_CALL_BACK)(UCHAR * pucUserID, UCHAR * pucSessionID);

VOID AnyOffice_API_SetSessionUpdate_CAllback(
    ANYOFFICE_SDK_SESSION_UPDATE_CALL_BACK callbackSessionUpdate);

INT32 AnyOffice_API_LoginWithoutGatewayAuthentication(CHAR *pcUserName);

INT32 AnyOffice_API_RedoGatewayAuthentication();

INT32 AnyOffice_API_RefreshUser();

typedef struct tagAnyOfficeAppInfo
{
    CHAR *pcAppVersion;
    CHAR *pcIdentifier;
    CHAR *pcName;
    CHAR *pcAppSize;
    CHAR *pcPackageUrl;
    CHAR *pcIconUrl;
    CHAR *pcDescription;
    ULONG ulPackageSize;
}ANYOFFICE_APP_INFO_S;

INT32 AnyOffice_API_CheckAppVersionUpdate(
    CHAR *pcAppID, CHAR *pcVersionNO, int languageType,char ** jsonAppInfo);

INT32 AnyOffice_API_GetCertificate(
    CHAR *pcUserName, CHAR **cert, ULONG *puSize, CHAR **ppcPassword);

/* BEGIN: Modified by zhangtailei 00218689, 2015/4/2   PN:修改密码后iOS没有跳转到网关登陆界面*/
void AnyOffice_API_ClearToKen(void);
/* END:   Modified by zhangtailei 00218689, 2015/4/2 */

/* BEGIN:Add by zhongyongcai for webos,2015-04-29 */
#ifdef ANYOFFICE_ATELIEROS
VOID AnyOffice_Login_FreeUserInfo(ANYOFFICE_USER_INFO_S* pstUserInfo);
#endif
/* END:Add by zhongyongcai for webos,2015-04-29 */

/*****************************************************************************
 函 数 名  : AnyOffice_API_SetClientCert
 功能描述  : 保存在线申请的客户端证书到keyspace
 输入参数  : SVN_CLIENTCERT_INFO_S *pstClientCert  
 输出参数  : 无
 返 回 值  : LONG
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月17日
    作    者   : suyaodong
    修改内容   : 新生成函数

*****************************************************************************/
LONG  AnyOffice_API_SetClientCert(SVN_CLIENTCERT_INFO_S *pstClientCert );

/*****************************************************************************
 函 数 名  : AnyOffice_API_GetClientCert
 功能描述  : 获取sandbox中保存的客户端证书
 输入参数  : SVN_CLIENTCERT_INFO_S * pstClientCert  
 输出参数  : 无
 返 回 值  : ULONG
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月17日
    作    者   : suyaodong
    修改内容   : 新生成函数

*****************************************************************************/
ULONG  AnyOffice_API_GetClientCert(SVN_CLIENTCERT_INFO_S * pstClientCert);

/*****************************************************************************
 函 数 名  : AnyOffice_API_DeleteClientCert
 功能描述  : 删除在线申请的证书（场景：设备解绑）
 输出参数  : 无
 返 回 值  : ULONG
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月17日
    作    者   : suyaodong
    修改内容   : 新生成函数

*****************************************************************************/
ULONG  AnyOffice_API_DeleteClientCert();
/*释放证书内部存储数据*/
#define AnyOffice_Free_Client_Cert(cert) \
    do \
    {    \
        ANYOFFICE_SAFE_FREE(cert.pucClientCert);\
        ANYOFFICE_SAFE_FREE(cert.pucPrivatekey);\
        ANYOFFICE_SAFE_FREE(cert.pszPrivateKeyPwd);\
    }\
    while(0)
#ifdef __cplusplus
#if __cplusplus
  }
#endif
#endif /* __cplusplus */

#endif


