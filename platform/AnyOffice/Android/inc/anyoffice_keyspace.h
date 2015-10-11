#ifndef __ANYOFFICE_KEYSPACE_H__
#define __ANYOFFICE_KEYSPACE_H__
/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_keyspace.h
  版 本 号   : 初稿
  作    者   : LiJingjin 90004727
  生成日期   : 2014年5月6日
  最近修改   :
  功能描述   : 系统安全存储区接口定义
  函数列表   :
*
*
  修改历史   :
  1.日    期   : 2014年5月6日
    作    者   : LiJingjin 90004727
    修改内容   : 创建文件

******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*----------------------------------------------*
 * 头文件                                       *
 *----------------------------------------------*/
#include "svn_define.h"
#include "tools_hash.h"
#include "anyoffice_login.h"

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 数据结构定义                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

INT32 AnyOffice_KeySpace_API_WriteItem(const char *pcGroup, const char *pcName, const char *pcValue);

INT32 AnyOffice_KeySpace_API_WriteGroup(const char *pcGroup, HASH_S* pstHash);

INT32 AnyOffice_KeySpace_API_ReadItem(const char *pcGroup, const char *pcName, char **ppcValue);

INT32 AnyOffice_KeySpace_API_ReadGroup(const char *pcGroup, HASH_S** ppstHash);

INT32 AnyOffice_KeySpace_API_DeleteItem(const char *pcGroup, const char *pcName);

INT32 AnyOffice_KeySpace_API_DeleteGroup(const char *pcGroup);

INT32 AnyOffice_KeySpace_API_WriteDeviceInfoGroup(HASH_S *pstHash);

INT32 AnyOffice_KeySpace_API_ReadDeviceInfoGroup(HASH_S **ppstHash);

INT32 AnyOffice_KeySpace_API_WriteDeviceInfoGroupItem(CHAR *pcItemName, CHAR *pcItemValue);

INT32 AnyOffice_KeySpace_API_ReadDeviceInfoGroupItem(CHAR *pcItemName, CHAR **ppcItemValue);

INT32 AnyOffice_KeySpace_API_DeleteDeviceInfoGroup();



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __KEYSPACE_API_H__ */
