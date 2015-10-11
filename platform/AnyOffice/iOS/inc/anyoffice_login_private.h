/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_login_private.h
  版 本 号   : 初稿
  作    者   : y90004712
  生成日期   : 2014年7月26日
  最近修改   :
  功能描述   : 登录相关的内部定义
  函数列表   :
*
*

  修改历史   :
  1.日    期   : 2014年7月26日
    作    者   : y90004712
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ANYOFFICE_LOGIN_PRIVATE_H__
#define __ANYOFFICE_LOGIN_PRIVATE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum tagAutoLoginType
{
    AUTO_LOGIN_ENABLE = 1,
    AUTO_LOGIN_DISABLE = 0,
    AUTO_LOGIN_DEFAULT = 999,
} AUTO_LOGIN_TYPE_EN;


/* 应用签名信息，用于应用验证 */
typedef struct tagAnyOfficeAppSignatureInfo
{
    CHAR *pcAppID;      /* 应用ID, Android平台对应于包名 */
    CHAR *pcPublicKey;  /* Android应用签名证书公钥 */
}ANYOFFICE_SIGNATURE_INFO_S;


typedef struct tagAnyOfficeInetAddress
{
    CHAR *pcHost;
    CHAR *pcIp;
    ULONG ulPort;
}ANYOFFICE_INET_ADDRESS;

/*
*  创建隧道接口
*/
INT32 AnyOffice_API_CreateTunnel();
INT32 AnyOffice_API_ReCreateTunnel();

/* 起线程创建隧道 */
VOID AnyOffice_API_CreateTunnelAsyn();



#ifdef __cplusplus
#if __cplusplus
  }
#endif
#endif /* __cplusplus */

#endif
