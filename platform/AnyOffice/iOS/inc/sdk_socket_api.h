#ifndef _SDK_SOCKET_API_H_
#define _SDK_SOCKET_API_H_


#include "svn_socket_api.h"
#include "svn_socket_err.h"
#include "svn_define.h"
#include "svn_api.h"

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>


#include <unistd.h>
/*BEGIN: modified by wangyinan/wKF70505 at 2012/09/15*/
#ifdef ANYOFFICE_ANDROID
#include <jni.h>
#include <android/log.h>
#endif
/*END: modified by wangyinan/wKF70505 at 2012/09/15*/

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <pthread.h>
#include <time.h>//<sys/time.h>
#include <string.h>

#include <sys/errno.h>


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define CONN_TYPE_SSL             0          
#define CONN_TYPE_TCP             1   

#define LOW_LEVEL_SOCK_TYPE_SYS   0         /*底层socket是系统socket*/          
#define LOW_LEVEL_SOCK_TYPE_L4VPN 1         /*底层socket是4层VPN*/   


#define CONN_SSL_METHOD_TLSV1 1
#define CONN_SSL_METHOD_SSLV3 2
        
#define SVN_SDK_OK 0
#define SVN_SDK_ERR 1

/* 定义使用的宏，如shutdown,closesocket函数采用自定义的，该处使用系统的做例子 */
#define SHUTDOWN(fd)    { svn_sdk_shutdown((fd),0); svn_sdk_close(fd); }
#define SHUTDOWN2(fd)   { svn_sdk_shutdown((fd),2); svn_sdk_close(fd); }

/*BEGIN: modified by wangyinan at 2012/11/27 PN: A57D11352 for ios L4VPN*/
#define BIO_TYPE_SVN_SDK_SOCKET  (0x20|0x0400|0x0100) //#define BIO_TYPE_SDK_SOCKET  (0x1F|0x0400|0x0100)   /* BIO类型标识，不能与别的冲突 */
/*END: modified by wangyinan at 2012/11/27 PN: A57D11352 for ios L4VPN*/

#define SVN_SDK_FDSETLEN        97      
#define SVN_SDK_NONUSE          0
#define SVN_SDK_USE             1
#define SVN_SDK_STATUS_ONLINE   1

#define SVN_SDK_URL_MAX_LEN 512
//#ifndef SSL
//#define SSL void
//#endif

/* 描述符集结构 */
typedef struct svn_sdk_fd_set
{
    long fds_bits[SVN_SDK_FDSETLEN];
}svn_sdk_fd_set;

/* BEGIN:Add by zhongyongcai for 邮件刷新优化 DTS2013080804983,2013-08-22 */
/* 可用的连接池 */
typedef struct tagHIM_DNS_Info
{
    unsigned long ulGetHostbyname;
    unsigned long ulDnsServerNameLen;
    char* pcDNSServ;
}HIM_DNS_INFO_S;
/* END:Add by zhongyongcai for 邮件刷新优化 DTS2013080804983,2013-08-22 */

/* BEGIN: Modified by lifangxiang/00218420, 2013/12/7   PN:DTS2013112110693 - 对接老网关阻塞 */
typedef struct tagSVN_SDK_ASYNC_DNS_RESOLVE
{
    char*           pcUrl;
    unsigned long   ulResolvedIP;
    int             iWaitResolved;
    int             iAckResolved;
    int             iWaitReceived;
    int             iAckReceived;
}SVN_SDK_ASYNC_DNS_RESOLVE_S;
/* END:   Modified by lifangxiang/00218420, 2013/12/7   PN:DTS2013112110693 - 对接老网关阻塞 */


struct hostent * svn_sdk_gethostbyname_ex(char *server);


void SVN_SDK_FD_SET(int iFd, svn_sdk_fd_set* pstFdSet);
int SVN_SDK_FD_CLR(int iFd, svn_sdk_fd_set* pstFdSet);
int SVN_SDK_FD_ISSET(int iFd, svn_sdk_fd_set* pstFdSet);
void SVN_SDK_FD_ZERO(svn_sdk_fd_set* pstFdSet);
int svn_sdk_socket(int iFamily, int iType, int iProtocol);
int svn_sdk_bind(int iFd, struct sockaddr *pstSockAddr, int iAddrLen);
int svn_sdk_connect(int iFd, struct sockaddr *pstSockAddr, int iAddrLen);
int svn_sdk_close(int iFd);
int svn_sdk_shutdown(int iFd, int iHow);
int svn_sdk_recv(int iFd, char *pcBuf, int iLen, int iFlags);
int svn_sdk_recvfrom(int iFd, char *pcBuf, int iLen, int iFlags, struct sockaddr *pstFromAddr, int *piFromAddrLen);
int svn_sdk_send(int iFd, char *pcString, int iLen, int iFlags);
int svn_sdk_sendto(int iFd, char *pcString, int iLen, int iFlags, struct sockaddr *pstToAddr, int iToAddrLen);
int svn_sdk_ioctl(int iFd, int iCmd, int *piArg);
int svn_sdk_setsockopt(int iFd, int iLevel, int iOptName, char *pcOptVal, int iOptLen);
int svn_sdk_getsockopt(int iFd, int iLevel, int iOptName, char *pcOptVal, int *piOptLen);
int svn_sdk_getsockname(int iFd, struct sockaddr *pstAddr, int *piAddrLen);
int svn_sdk_getpeername(int iFd, struct sockaddr *pstAddr, int *piAddrLen);
int svn_sdk_select(int iMaxFd, struct svn_sdk_fd_set *pstIn, struct svn_sdk_fd_set *pstOut, struct svn_sdk_fd_set *pstEx, struct timeval *pstTvO);
char *svn_sdk_strerror(int iErrno);
int svn_sdk_geterror();
int svn_sdk_inprogress(int iErrNo);

void svn_sdk_set_usesdk(unsigned long ulUseSvnSocket);
/*BEGIN: added by wangyinan at 2012/11/27 PN: A57D11352 for ios L4VPN*/
unsigned long svn_sdk_get_usesdk();
/*END: added by wangyinan at 2012/11/27 PN: A57D11352 for ios L4VPN*/
void  SVN_SDK_FD_SET_EX(int iFd, 
                                    svn_sdk_fd_set* pstFdSet,
                                    svn_sdk_fd_set* pstsvnFdSet);

int SVN_SDK_FD_ISSET_EX(int iFd, 
                                                     svn_sdk_fd_set* pstFdSet,
                                                     svn_sdk_fd_set* pstsvnFdSet);
int svn_sdk_select_ex(int iMaxFd,
                            int svnFd, 
                            struct svn_sdk_fd_set *pstsvnIn, 
                            struct svn_sdk_fd_set *pstsvnOut, 
                            struct svn_sdk_fd_set *pstsvnEx, 
                            struct timeval *pstsvnTvO,
                            int Fd, 
                            struct svn_sdk_fd_set *pstIn, 
                            struct svn_sdk_fd_set *pstOut, 
                            struct svn_sdk_fd_set *pstEx, 
                            struct timeval *pstTvO,
                            struct timeval *pstTimeOut,
                            char *buf,
                            int *piBufLen);

int SSL_sdk_set_fd(void *s,int fd);

int svn_sdk_gethostbyname(char *name, int namelen);


/* BEGIN: Modified by lifangxiang/00218420, 2013/12/7   PN:DTSXXXXXXXXXX - 对接老网关阻塞 */
void*   svn_dns_GethostByNameThread(void *arg);
void    svn_dns_ParseURLCallback(unsigned long ulIP[], void* pvData);

SVN_SDK_ASYNC_DNS_RESOLVE_S*    svn_dns_CreateAsyncResolveParam(const char* pcUrl);
void                            svn_dns_DestroyAsyncResolveParam(SVN_SDK_ASYNC_DNS_RESOLVE_S* pstParam);

void    svn_dns_AckResolvedFinished(SVN_SDK_ASYNC_DNS_RESOLVE_S* pstParam);
int     svn_dns_WaitResolvedFinished(SVN_SDK_ASYNC_DNS_RESOLVE_S* pstParam, unsigned long ulTimeoutMS);
void    svn_dns_AckReceivedFinished(SVN_SDK_ASYNC_DNS_RESOLVE_S* pstParam);
int     svn_dns_WaitReceivedFinished(SVN_SDK_ASYNC_DNS_RESOLVE_S* pstParam, unsigned long ulTimeoutMS);
int     svn_dns_gethostbyname(const char* pcUrl, unsigned long ulUrl, int iL4VPN);
/* END:   Modified by lifangxiang/00218420, 2013/12/7   PN:DTSXXXXXXXXXX - 对接老网关阻塞 */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif 


