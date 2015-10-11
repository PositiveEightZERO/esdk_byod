/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

******************************************************************************
  文 件 名   : mdmsdk_chash.c
  版 本 号   : 初稿
  作    者   : kongsheng
  生成日期   : 2014年7月31日
  最近修改   :
  功能描述   :
  函数列表   :

  修改历史   :
  1.日    期   : 2014年7月14日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/
#ifndef CERT_API_H
#define CERT_API_H
#ifdef __cplusplus
extern "C" {
#endif

INT32 AnyOffice_API_GetCertificate(CHAR *pcUsername, CHAR **ppcData, ULONG *pulDataLen, CHAR **ppcPassword);
INT32 AnyOffice_API_GetAccountName( CHAR **ppcAccountName);


#ifdef __cplusplus
}
#endif

#endif
