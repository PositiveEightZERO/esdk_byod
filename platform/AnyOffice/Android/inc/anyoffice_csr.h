/******************************************************************************

                  版权所有 (C), 2006-2016, 华为赛门铁克科技有限公司

 ******************************************************************************
文 件 名  : csr.h
版 本 号  : V200R002C10
作    者  : 陈峰 90004813
生成日期  : 2014-11-5
最近修改  :
功能描述  : 产生证书申请签名CSR的接口
函数列表  : 无
修改历史  :
1.日    期:
  作    者:
  修改内容:
******************************************************************************/


#ifndef __ANYOFFICE_CSR_H__
#define __ANYOFFICE_CSR_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "svn_define.h"

#define VOS_ERR 1
#define VOS_OK 0
#define VOID void
#define ULONG unsigned long
#define LONG long
#define CHAR char
#define UCHAR unsigned char

#define MDM_PRIVATE_PEM_HEAD "-----BEGIN RSA PRIVATE KEY-----"
#define MDM_PRIVATE_PEM_TAIL "-----END RSA PRIVATE KEY-----"

/**//*版本号*/
#define MA_X509_V1 0
#define MA_X509_V2 1
#define MA_X509_V3 2

#define CERT_DN_T  "t="
#define CERT_DN_CN "cn="
#define CERT_DN_C  "c="
#define CERT_DN_OU "ou="
#define CERT_DN_O  "o="
#define CERT_DN_DC "dc="
#define CERT_DN_L  "l="
#define CERT_DN_ST "st="

#define ANYOFFICE_ASYMMETRIC_TYPE_RSA  0    /* 非对称算法类型:RSA */
#define ANYOFFICE_ASYMMETRIC_TYPE_DSA  1    /* 非对称算法类型:DSA */

/* 非对称密钥信息 */
typedef struct tagAsymmetricKeyInfo
{
    unsigned long  ulAlgType;   /* 非对称算法类型 */
    char *pcPrivateKeyInfo;         /* 非对称算法密钥信息 */
    unsigned long ulKeyInfoLen;     /* 非对称算法密钥信息长度 */
} ASYMMETRIC_KEY_INFO_S;

#define ANYOFFICE_ASYMMETRIC_LEN_512    512
#define ANYOFFICE_ASYMMETRIC_LEN_1024   1024
#define ANYOFFICE_ASYMMETRIC_LEN_2048   2048
#define ANYOFFICE_ASYMMETRIC_LEN_4096   2096

/* 非对称算法信息 */
typedef struct tagAsymmetricAlgorithmInfo
{
    unsigned long  ulAlgType;   /* 非对称算法类型 */
    unsigned long  ulKeyLen;    /* 非对称算法密钥信息长度 */
} ASYMMETRIC_ALGORITHM_INFO_S;

#define ANYOFFICE_SIGNATURE_ALG_MD2     0
#define ANYOFFICE_SIGNATURE_ALG_MD5     1
#define ANYOFFICE_SIGNATURE_ALG_SHA1    2
#define ANYOFFICE_SIGNATURE_ALG_SHA256  3
#define ANYOFFICE_SIGNATURE_ALG_SHA512  4

typedef struct tag_CSR_SOURCE_S
{
    char *pcChallengePassword;
    unsigned long ulSignatureAlgorithmType; /* 签名算法类型 */
    ASYMMETRIC_KEY_INFO_S       *pstKeyInfo;
    ASYMMETRIC_ALGORITHM_INFO_S *pstAlgInfo;
}CSR_SOURCE_S;

typedef struct tag_CSR_SUBJECT_S
{
    char *pcDn; /*style: "CN=xxx,O=xxx,OU=xxx"*/
}CSR_SUBJECT_S;


#define ANYOFFICE_CSR_EXT_TYPE_TEXT  0  /* 扩展属性值类型:字符串 */

/* 扩展属性值定义 */
typedef struct tagCSR_Extension
{
    unsigned long ulType;
    void *pvValue;
};

#define ANYOFFICE_CSR_EXT_SUBJECT_ALTERNATIVE_NAME  "SubjectAlternativeName"

typedef struct tag_CSR_EXTENSIONS_S
{
    char *pcSubjectAlternativeName;
}CSR_EXTENSIONS;

/* not support the moment (2014-11-3) */
typedef struct tag_CSR_KEY_USAGE_S
{
    /* by bit: 
             1. digital signature:                0x1>>0;
             2. Non Repudiation:               0x1>>1
             3. Key Encipherment:            0x1>>2;
             4: Data Encipherment;
             5: Key Agreement;
             6: Certificate Sign;
             7: CRL Sign;
             8: Encipher Only;
             9: Decipher Only;
    */
    unsigned long ulKeyUsage;
    /* by bit:
             1: TLS Web Server Authentication;
             2: TLS Web client Authentication;
             3: Code Signing;
             4: E-mail Protection;
             5: Time stamping;
             6: Microsoft Individual Code Signing;
             7: Microsoft Commerical Code Signing;
             8: Microsoft Trust List Signing;
             9: Microsoft Server Gated Crypto;
             10: Microsoft Encrypted File System;
             11: Netscape Server Gated Crypto;
             12: Microsoft EFS File Recovery;
             13: IPSec End System;
             14: IPSec Tunnel;
             15: IPSec User;
             16: IP security end entity;
             17: Smart Card Logon;

    */
    unsigned long ulExtendedKeyUsage;
}CSR_KEY_USAGE_S;

/* not support the moment (2014-11-3) */
typedef struct tag_CSR_NETSCAPE_S
{
    
}CSR_NETSCAPE_S;


typedef struct tag_CSR_PRE_INFO_S
{
    CSR_SOURCE_S    stCsrSource;
    CSR_SUBJECT_S   stCsrSubject;    /*must not null*/
    CSR_EXTENSIONS  stCsrExtensions;
    CSR_KEY_USAGE_S stCsrKeyusage;
    CSR_NETSCAPE_S  stNetscape;      /*not support this moment (2014-11-3)*/
}CSR_PRE_INFO_S;

typedef struct tag_CERT_REQ_S
{
    char *pcX509ReqData;     /* CSR data */
    unsigned long ulX509ReqLen;
    char *pcPrivateKeyData;  /* private key data */
    unsigned long ulPrivateKeyLen;
    char *pcPkcs8PrivateKeyData; /* pkcs8 style private key data */
    unsigned long ulPkcs8PrivateKeyLen;
    char *pcPublicKeyData;   /* public key data */
    unsigned long ulPublicKeyLen;
}CERT_REQ_S;

CSR_PRE_INFO_S *AnyOffice_API_Cert_CsrPreInfo_New();
void AnyOffice_API_Cert_ReleaseCsrPreInfo(CSR_PRE_INFO_S *pstCsrPreInfo);

/* Create CSR and Return the data certificate request need */
CERT_REQ_S *AnyOffice_API_Cert_CreateCertReq(CSR_PRE_INFO_S *pstCsrPreInfo);
void AnyOffice_API_Cert_ReleaseCertReq(CERT_REQ_S *pstCertReq);





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __CSR_H__ */