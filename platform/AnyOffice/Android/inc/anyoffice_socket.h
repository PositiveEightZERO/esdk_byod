/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_socket.h
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
#ifndef __ANYOFFICE_SOCKET_H__
#define __ANYOFFICE_SOCKET_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * 头文件                                       *
 *----------------------------------------------*/
#include "tools_socket.h"
#include "anyoffice_socket_impl.h"

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#ifndef sockaddr
#define sockaddr svn_sockaddr
#endif
 
#ifndef fd_set
#define fd_set svn_fd_set
#endif
 
#ifndef timeval
#define timeval svn_timeval
#endif
 
/*----------------------------------------------*
 * 数据结构定义                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
#define FD_SET       ANYOFFICE_FD_SET
#define FD_CLR       ANYOFFICE_FD_CLR
#define FD_ISSET     ANYOFFICE_FD_ISSET
#define FD_ZERO      ANYOFFICE_FD_ZERO
#define socket       anyoffice_socket
#define bind         anyoffice_bind
#define connect      anyoffice_connect
#define close        anyoffice_close
#define shutdown     anyoffice_shutdown
#define recv         anyoffice_recv
#define recvfrom     anyoffice_recvfrom
#define send         anyoffice_send
#define sendto       anyoffice_sendto
#define ioctl        anyoffice_ioctl
#define setsockopt   anyoffice_setsockopt
#define getsockopt   anyoffice_getsockopt
#define getsockname  anyoffice_getsockname
#define getpeername  anyoffice_getpeername
#define select       anyoffice_select
#define strerror     anyoffice_strerror

void anyoffice_set_usesdk(unsigned long ulUseSvnSocket);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif



