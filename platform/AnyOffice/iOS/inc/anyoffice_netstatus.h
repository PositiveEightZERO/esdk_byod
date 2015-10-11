/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_netstatus.h
  版 本 号   : 初稿
  作    者   : LiJingjin 90004727
  生成日期   : 2014年5月6日
  最近修改   :
  功能描述   : 系统接口获取网络设备状态发生变化时，调用接口触发网络切换。
  函数列表   :
*
*
  修改历史   :
  1.日    期   : 2014年5月6日
    作    者   : LiJingjin 90004727
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ANYOFFICE_NETSTATUS_H__
#define __ANYOFFICE_NETSTATUS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#include "tools_common.h"
#include "tools_string.h"
#include "anyoffice_def.h"


/*----------------------------------------------*
 * 头文件                                       *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/* 网络类型枚举定义(与原生层定义一致) */
typedef enum tagNetType
{
    NET_TYPE_WIFI = 0,
    NET_TYPE_3G = 1,
    NET_TYPE_4G,
    NET_TYPE_OTHER = 99,
    NET_TYPE_UNREACHABLE = 999,
} NET_TYPE_EN;

/* 网络状态枚举定义 */
typedef enum tagNetStatus
{
    NET_STATUS_OFFLINE = 0,
    NET_STATUS_ONLINE,
    NET_STATUS_CONNECTING
} NET_STATUS_EN;

/*----------------------------------------------*
 *                         宏定义                                       *
 *----------------------------------------------*/
/* 离线错误码定义 */
#define ANYOFFICE_NET_STATUS_ERR_UNREACHABLE  -1000   /* 终端网络已断开 */


/*----------------------------------------------*
 * 数据结构定义                                 *
 *----------------------------------------------*/
 typedef struct tagAnyOfficeDeviceNetStatus
 {
     NET_TYPE_EN   enNetType;  /* 4G/3G/Wifi */
     CHAR acSSID[ANYOFFICE_MAX_SSID_LEN];
 }ANYOFFICE_DEVICE_NET_STATUS_S;


/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/* 获取网络连接的状态，网络和隧道的状态 */
NET_STATUS_EN AnyOffice_API_GetNetStatus();

/* 获取网络设备的状态和属性 */
/* 连接/断开 3G/Wifi  SSID   */
ULONG AnyOffice_API_GetDeviceNetStatus(ANYOFFICE_DEVICE_NET_STATUS_S *pstStatus);


/* 设置网络状态变化的回调 */
typedef ULONG (*AnyOffice_API_NetChangeCallback)(NET_STATUS_EN enOldStatus, NET_STATUS_EN enNewStatus, INT32 iErrorCode);

ULONG AnyOffice_API_SetNetChangeCallback(AnyOffice_API_NetChangeCallback pfCallback);




#ifdef __cplusplus
#if __cplusplus
 }
#endif
#endif /* __cplusplus */

#endif

