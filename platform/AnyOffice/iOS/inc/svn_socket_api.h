#ifndef __SVN_SOCKET_API_H__
#define __SVN_SOCKET_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************************************************************/
/* 类型定义  */
/***************************************************************/
#ifndef LONG
#define LONG     long
#endif

#ifndef ULONG
#define ULONG    unsigned long
#endif

#ifndef SHORT   
#define SHORT    short
#endif

#ifndef CHAR
#define CHAR     char
#endif

#define SVN_FD_SETSIZE      3072            
#define SVN_FDSETLEN        97              
#define SVN_NFDBITS         0x20            

/* 描述符集结构 */
typedef struct svn_fd_set
{
    LONG fds_bits[SVN_FDSETLEN];
}svn_fd_set;

#define SVN_FD_SET(n, p)   \
{\
    if ( (n) > 0 ) {\
        ((p)->fds_bits[(n)/SVN_NFDBITS] |= \
        (((ULONG) 0x80000000) >> ((n) % SVN_NFDBITS)));\
    }\
}

#define SVN_FD_CLR(n, p)    ((p)->fds_bits[(n)/SVN_NFDBITS] &= \
    ~(((ULONG) 0x80000000) >> ((n) % SVN_NFDBITS)))
    
#define SVN_FD_ISSET(n, p)  ((p)->fds_bits[(n)/SVN_NFDBITS] & \
    (((ULONG) 0x80000000) >> ((n) % SVN_NFDBITS)))
    
#define SVN_FD_ZERO(p)  { \
    LONG    lIter; \
    for ( lIter = 0; lIter < SVN_FDSETLEN; lIter++ ) { \
        ((p)->fds_bits[lIter] = 0); \
        } \
}
/***************************************************************/
/* 接口函数  */
/***************************************************************/

LONG svn_socket(LONG lFamily, LONG lType, LONG lProtocol);

LONG svn_bind(LONG lFd, struct sockaddr_in *pstSockAddr, LONG lAddrLen);

LONG svn_connect(LONG lFd, struct sockaddr_in *pstSockAddr, LONG lAddrLen);

LONG svn_listen(LONG lFd, LONG lBackLog);

LONG svn_accept(LONG lFd, struct sockaddr_in *pstSockAddr, LONG *plAddrLen);

LONG svn_close(LONG lFd);

LONG svn_shutdown(LONG lFd, LONG lHow);

LONG svn_recv(LONG lFd, CHAR *pcBuf, LONG lLen, LONG lFlags);

LONG svn_recvfrom(LONG lFd, CHAR *pcBuf, LONG lLen, LONG lFlags, struct sockaddr_in *pstFromAddr, LONG *plFromAddrLen);

LONG svn_send(LONG lFd, CHAR *pcString, LONG lLen, LONG lFlags);

LONG svn_sendto(LONG lFd, CHAR *pcString, LONG lLen, LONG lFlags, struct sockaddr_in *pstToAddr, LONG lToAddrLen);

LONG svn_ioctl(LONG lFd, LONG lCmd, LONG *plArg);

LONG svn_setsockopt(LONG lFd, LONG lLevel, LONG lOptName, CHAR *pcOptVal, LONG lOptLen);

LONG svn_getsockopt(LONG lFd, LONG lLevel, LONG lOptName, CHAR *pcOptVal, LONG *plOptLen);

LONG svn_getsockname(LONG lFd, struct sockaddr_in *pstAddr, LONG *plAddrLen);

LONG svn_getpeername(LONG lFd, struct sockaddr_in *pstAddr, LONG *plAddrLen);

LONG svn_select(LONG lMaxFd, struct svn_fd_set *pstIn, struct svn_fd_set *pstOut, struct svn_fd_set *pstEx, struct timeval *pstTvO);

CHAR *svn_strerror(LONG lErrno);

/* BEGIN: Added for PN:增加TCP over UDP模式 by y90004712, 2013/8/26 */
LONG svn_setusetls(LONG lFd);
/* END:   Added for PN:增加TCP over UDP模式 by y90004712, 2013/8/26 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
