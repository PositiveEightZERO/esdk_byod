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

#ifndef __ANYOFFICE_SOCKET_IMPL_H__
#define __ANYOFFICE_SOCKET_IMPL_H__
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define ANYOFFICE_FDSETLEN        97      

/* 用于设置Socket是否使用L4VPN*/
#define ANYOFFICE_USEL4VPN      0x20000001 

struct svn_sockaddr
{
  unsigned short sa_family;
  char  sa_data[14];
};

/* 描述符集结构 */
typedef struct svn_fd_set
{
    long fds_bits[ANYOFFICE_FDSETLEN];  /* we support for 3072 socket in a task */
} svn_fd_set;

struct svn_timeval
{
    long  tv_sec;       /* no. of seconds */
    long  tv_usec;      /* no. of micro seconds */
};


/*----------------------------------------------*
 * 头文件                                       *
 *----------------------------------------------*/


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
void  ANYOFFICE_FD_SET(int iFd, svn_fd_set* pstFdSet);

int ANYOFFICE_FD_CLR(int iFd, svn_fd_set* pstFdSet);

int ANYOFFICE_FD_ISSET(int iFd, svn_fd_set* pstFdSet);

void ANYOFFICE_FD_ZERO(svn_fd_set* pstFdSet);
 
int anyoffice_socket(int iFamily, int iType, int iProtocol);

int anyoffice_bind(int iFd, struct svn_sockaddr *pstsvn_sockaddr, int iAddrLen);

int anyoffice_connect(int iFd, struct svn_sockaddr *pstsvn_sockaddr, int iAddrLen);

int anyoffice_close(int iFd);

int anyoffice_shutdown(int iFd, int iHow);

int anyoffice_recv(int iFd, char *pcBuf, int iLen, int iFlags);

int anyoffice_recvfrom(int iFd, char *pcBuf, int iLen, int iFlags, struct svn_sockaddr *pstFromAddr, int *piFromAddrLen);

int anyoffice_send(int iFd, char *pcString, int iLen, int iFlags);

int anyoffice_sendto(int iFd, char *pcString, int iLen, int iFlags, struct svn_sockaddr *pstToAddr, int iToAddrLen);

int anyoffice_ioctl(int iFd, int iCmd, int *piArg);

int anyoffice_setsockopt(int iFd, int iLevel, int iOptName, char *pcOptVal, int iOptLen);

int anyoffice_getsockopt(int iFd, int iLevel, int iOptName, char *pcOptVal, int *piOptLen);

int anyoffice_getsockname(int iFd, struct svn_sockaddr *pstAddr, int *piAddrLen);

int anyoffice_getpeername(int iFd, struct svn_sockaddr *pstAddr, int *piAddrLen);

int anyoffice_select(int iMaxFd, struct svn_fd_set *pstIn, struct svn_fd_set *pstOut, struct svn_fd_set *pstEx, struct svn_timeval *pstTvO);

char* anyoffice_strerror(int iErrno);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */




