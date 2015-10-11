/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_sdk.h
  版 本 号   : 初稿
  作    者   : zhangyantao 00103873
  生成日期   : 2014年7月9日
  最近修改   :
  功能描述   : AnyOffice SDK 初始化和去初始化
  函数列表   :
*
*
  修改历史   :
  1.日    期   : 2014年7月9日
    作    者   : zhangyantao 00103873
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ANYOFFICE_SDK_H__
#define __ANYOFFICE_SDK_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * 头文件                                       *
 *----------------------------------------------*/
#include "tools_common.h"
#include "svn_define.h"
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define ANYOFFICE_COMMON_OK   0
#define ANYOFFICE_ERROR_COMMON_UNDEFINED       1
#define ANYOFFICE_ERROR_COMMON_PARAMETER       2
#define ANYOFFICE_ERROR_COMMON_OUTOFMEMORY     3

#define ANYOFFICE_ERROR_COMMON_TCP_CONNECT_FAILED   -1
#define ANYOFFICE_ERROR_COMMON_PROXY_CONNECT_FAILED -2
#define ANYOFFICE_ERROR_COMMON_INVALID_PROXY        -3
#define ANYOFFICE_ERROR_CONMON_TLS_SHAKEHAND_FAILED -4
#define ANYOFFICE_ERROR_COMMON_UNAUTHORIED          -5
#define ANYOFFICE_ERROR_COMMON_GATEWAY_EXCEPTION    -99
#define ANYOFFICE_ERROR_COMMON_INTERNAL_EXCEPTION   -100

#define ANYOFFICE_ERROR_COMMON_UNINITIALIZED        -1001
#define ANYOFFICE_ERROR_COMMON_REQUIRE_APPAUTHEN    -1002  /* 应用验证失败,应用非法 */
#define ANYOFFICE_ERROR_COMMON_REQUIRE_USERAUTHEN   -1003
#define ANYOFFICE_ERROR_COMMON_REQUIRE_GET_POLICY   -1004
#define ANYOFFICE_ERROR_COMMON_REQUIRE_LOGIN        -1005
#define ANYOFFICE_ERROR_COMMON_VERSION_MISSMATCH    -1006
#define ANYOFFICE_ERROR_COMMON_INVALID_PARAMETER    -1007
#define ANYOFFICE_ERROR_COMMON_NOFUNCTION           -1008  /*no license, or no function is enabled*/
#define ANYOFFICE_ERROR_COMMON_CHK_CERT_BIND_FAIL       -1009  /*ckcert fail 由网关返回*/
#define ANYOFFICE_ERROR_COMMON_CHK_CERT_UNKNOW_FAIL       -1010  /*未知错误 属于证书校验失败，可能是网络异常导致，由客户端适配*/
#define ANYOFFICE_ERROR_APP_INVALID_APPID           -2001
#define ANYOFFICE_ERROR_APP_INVALID_SIGNATURE       -2002

#define ANYOFFICE_ERROR_GATEWAY_INVALID_TOKEN       -3001
#define ANYOFFICE_ERROR_GATEWAY_REQUIRE_USER        -3006
#define ANYOFFICE_ERROR_GATEWAY_REQUIRE_CONFIG      -3007

#define ANYOFFICE_ERROR_LOGIN_MISTAKE_ERR            -1       /* 其他错误:LOGIN_FAILLOG_ANYOFFICE_MISTAKE */
#define ANYOFFICE_ERROR_LOGIN_SYSTEM_BUSY            -3       /* 连接网关失:LOGIN_FAILLOG_ANYOFFICE_NOGATEWAY */
#define ANYOFFICE_ERROR_LOGIN_GATEWAY_CONNECT_ERR   -4       /* 连接网关失:LOGIN_FAILLOG_ANYOFFICE_NOGATEWAY */
#define ANYOFFICE_ERROR_LOGIN_USERINFO_ERR           -5       /* 用户名密码错误:LOGIN_FAILLOG_ANYOFFICE_ERRORPASSWORD */
#define ANYOFFICE_ERROR_LOGIN_USERINFO_NO_PASSWORD  -9       /*没有密码*/
#define ANYOFFICE_ERROR_LOGIN_GATEWAY_PORT_ERR       -97     /* 端口号为空 */
#define ANYOFFICE_ERROR_LOGIN_GATEWAY_ADDRESS_ERR   -98      /* 网关地址为空 */
#define ANYOFFICE_ERROR_LOGIN_MAILINFO_ERR           -99       /* 邮件地址异常:LOGIN_FAILLOG_ANYOFFICE_ERROREMAIL */
#define ANYOFFICE_ERROR_LOGIN_GATEWAYINFO_ERR        -100       /* 服务器配置策略异常:LOGIN_FAILLOG_ANYOFFICE_ERRORSEVER */
#define ANYOFFICE_ERROR_LOGIN_CANOFFLINELOGIN_ERR        -100       /* 服务器配置策略异常:LOGIN_FAILLOG_ANYOFFICE_ERRORSEVER */
#define ANYOFFICE_ERROR_LOGIN_EXCEPTION              -101   /* 未知异常:LOGIN_FAILLOG_ANYOFFICE_EXCEPTION */
#define ANYOFFICE_ERROR_LOGIN_LICENSE_VISIT_LIMIT_ERR  -102/*license未激活或过期*/
#define ANYOFFICE_ERROR_LOGIN_POLICY_ENC_CHANGED  -103     /* 加解密策略切换 */
#define ANYOFFICE_ERROR_LOGIN_DB_CORRUPT -104            /* 数据库磁盘文件损坏 */
#define ANYOFFICE_ERROR_LOGIN_MAILISLOGIN_ANDCFGISCHANGED -105            /*未杀进程邮箱已经成功登录过，本次登录网关地址\邮件地址\用户名有修改,需要用户退出重登录(杀进程)*/
#define ANYOFFICE_ERROR_LOGINE_OFFLINE              -95
#define ANYOFFICE_ERROR_LOGIN_CHANGE_ACCOUNT -96

#define ANYOFFICE_SDK_ENTRY_VERSION_MAX_LEN 256
#define ANYOFFICE_ERROR_AUTH_NO_CLIENT_CERT -4000 /*双向校验函数，如果不存在证书，则返回错误*/

#define CSR_NO_MEMORY               1
#define CSR_ARG_ERR                 2
#define CSR_ERR_DN_TEMPLATE         3
#define CSR_ERR_GET_CONFIG          4
#define CSR_HTTP_CON_ERR            5
#define CSR_HTTP_RESPONSE_ERR       6

const unsigned char* AnyOffice_API_getEntryVersion();

void AnyOffice_API_setEntryVersion(unsigned char *pucEntryVersion);

INT32 AnyOffice_API_InitEnv(CHAR *pcUserName, CHAR *pcWorkPath);

INT32 AnyOffice_API_ResetEnv(void);

ULONG AnyOffice_API_CleanEnv(void);

VOID AnyOffice_API_SetLogCallBack(SVN_WriteLogCallback pfCallBackWriteLog);
typedef VOID (*TreatCACallback)();
VOID AnyOffice_API_SetCATreatCallback(TreatCACallback pfCallbackTreatCA);

/* BEGIN:Add by zhongyongcai for webos,2015-05-5*/
#ifdef ANYOFFICE_ATELIEROS
void AnyOffice_SDK_SetPackageName(char* pcPackageName);
#endif
/* END:Add by zhongyongcai for webos,2015-05-5*/


/*****************************************************************************
 函 数 名  : AnyOffice_API_Apply_Certificate
 功能描述  : 对外提供证书申请功能
 输入参数  : CSR_PRE_INFO_S *pstCsrPreInfo           
             SVN_CLIENTCERT_INFO_S **ppstClientCert  
 输出参数  : CSR_PRE_INFO_S *pstCsrPreInfo    
 返 回 值  : CSR_NO_MEMORY               1 内存申请失败
             CSR_ARG_ERR                 2 参数错误
             CSR_ERR_DN_TEMPLATE         3 DN模板错误
             CSR_ERR_GET_CONFIG          4 获取配置文件失败
             CSR_HTTP_CON_ERR            5 http链接失败
             CSR_HTTP_RESPONSE_ERR       6 http response失败
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月10日
    作    者   : suyaodong
    修改内容   : 新生成函数

*****************************************************************************/
struct tag_CSR_PRE_INFO_S;
int AnyOffice_API_Apply_Certificate(struct tag_CSR_PRE_INFO_S  *pstCsrPreInfo,SVN_CLIENTCERT_INFO_S **pstClientCert );

/*****************************************************************************
 函 数 名  : AnyOffice_API_Check_Cert_Bind
 功能描述  : 检查客户端证书是否绑定
 输出参数  : 无
 返 回 值  : LONG
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月30日
    作    者   : suyaodong
    修改内容   : 新生成函数

*****************************************************************************/
LONG AnyOffice_API_Check_Cert_Bind();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

