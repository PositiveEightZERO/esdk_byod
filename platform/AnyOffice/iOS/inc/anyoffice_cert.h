/******************************************************************************

                  版权所有 (C), 2006-2016, 华为赛门铁克科技有限公司

 ******************************************************************************
文 件 名  : cert.h
版 本 号  : V200R002C10
作    者  : 陈峰 90004813
生成日期  : 2014-11-5
最近修改  :
功能描述  : 与证书功能的相关接口
函数列表  : 无
修改历史  :
1.日    期:
  作    者:
  修改内容:
******************************************************************************/


#ifndef __ANYOFFICE_CERT_H__
#define __ANYOFFICE_CERT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "svn_define.h"
#include "anyoffice_csr.h"
#include <unistd.h>

ULONG AnyOffice_Cert_CreatPFX_by_X509AndEVPKEY(UCHAR *pucX509Cert ,ULONG ulX509Length,
        UCHAR *pucEVPKey,ULONG keyLength, UCHAR *pucKeyPassword,UCHAR **ppucPFXCert);
void AnyOffice_Cert_PrivateKey2Pkcs8(char *pcInPriKey, unsigned long ulInLen, char **ppcOutPriPkcs8Key, unsigned long *pulOutLen);
long AnyOffice_Cert_X509_cmp_time(char *pcInX509Buf, unsigned long ulInX509Len, time_t *cmp_time);
unsigned char **AnyOffice_Cert_Get_Issuer_Name(char *pcInX509Buf, unsigned long ulInX509Len);

/*****************************************************************************
 函 数 名  : AnyOffice_Cert_UseCert
 功能描述  : openssl设置客户端证书包装函数
 输入参数  : void * sslctx  
             void * x       
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月27日
    作    者   : suyaodong
    修改内容   : 新生成函数

*****************************************************************************/
int AnyOffice_Cert_UseCert(void * sslctx, void * x);

/*****************************************************************************
 函 数 名  : AnyOffice_Cert_UsePriv
 功能描述  : openssl设置客户端证书私钥包装函数
 输入参数  : void *sslctx  
             void * pkey   
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月27日
    作    者   : suyaodong
    修改内容   : 新生成函数

*****************************************************************************/
int AnyOffice_Cert_UsePriv(void *sslctx, void * pkey);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __CSR_H__ */