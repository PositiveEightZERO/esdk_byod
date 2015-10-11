#ifndef __ANYOFFICE_OS_H__
#define __ANYOFFICE_OS_H__
/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_os.h
  版 本 号   : 初稿
  作    者   : zhangyantao 00103873
  生成日期   : 2014年7月18日
  最近修改   :
  功能描述   : 处理系统差异
  函数列表   :
  修改历史   :
  1.日    期   : 2014年7月18日
    作    者   : zhangyantao 00103873
    修改内容   : 创建文件
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "tools_common.h"

#define DEVICE_FAMILY_TYPE_LEN 10

CHAR* AnyOffice_OS_API_GetAppID();

CHAR* AnyOffice_OS_API_GetDeviceID();

CHAR* AnyOffice_OS_API_GetSystemVersion();

CHAR *AnyOffice_OS_API_GetUUID();

CHAR* AnyOffice_OS_API_GetOsFamilyType();


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



