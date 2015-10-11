/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : tools_socket.h
  版 本 号   : 初稿
  作    者   : y90004712
  生成日期   : 2014年7月11日
  最近修改   :
  功能描述   : VOS socket申明
  函数列表   :
*
*

  修改历史   :
  1.日    期   : 2014年7月11日
    作    者   : y90004712
    修改内容   : 创建文件

******************************************************************************/
#ifndef __TOOLS_SOCKET_H__
#define __TOOLS_SOCKET_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include "tools_common.h"



/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
typedef long VOS_SOCKET;

#ifndef VOS_INVALID_SOCKET
#define VOS_INVALID_SOCKET (-1)
#endif


/*----------------------------------------------*/
/*               ioctl相关常量定义                           */
/*----------------------------------------------*/
#ifndef FIONBIO
#define FIOGETOWN       0x4004667C      /* Get owner of the socket */
/* added by mayun 获得发送缓冲区空闲空间大小 */
#define FIOSBFREE       0x4004667B      /* Get bytes of socket send buffer's free space */
/* end of mayun */
#define FIOREAD         0x4004667F      /* Get bytes in socket read buffer */
#define FIOSETOWN       0x8004667C      /* Set owner of the socket */
#define FIOASYNC        0x8004667D      /* Set/Unset socket to receive */                                      /* Asyncronous signal */
#define FIONBIO         0x8004667E      /* Set/Unset socket non-blocking */
#define FIORECALL       0x8004668D      /* Set/Unset socket to call back */
#define	FSENDDATAIF	    0x80046684      /* Set/Unset socket send data out from a ifnet */

#define F_GETFL         1               /* get file status flags */
#define F_SETFL         2               /* set file status flags */
#define F_GETOWN        3               /* get SIGIO/SIGURG proc/pgrp */

#define FASYNC          0x04            /* kernel/compat */
#define FNONBLOCK       0x08            /* kernel */
#define FNDELAY         0x08            /* compat */

#define FCNTLFLAGS      (FASYNC|FNONBLOCK)
#endif


/*----------------------------------------------*
 *              socket错误码定义                              *
 *----------------------------------------------*/
#ifndef EWOULDBLOCK
#define	EPERM	    	-1		/* Operation not permitted */
#define	ENOENT		-2		/* No such file or directory */
#define	ESRCH		-3		/* No such process */
#define	EINTR		-4		/* Interrupted system call */
#define	EIO		-5		/* Input/output error */
#define	ENXIO		-6		/* Device not configured */
#define	E2BIG		-7		/* Argument list too long */
#define	ENOEXEC		-8		/* Exec format error */
#define	EBADF		-9		/* Bad file descriptor */
#define	ECHILD		-10		/* No child processes */
#define	EDEADLK		-11		/* Resource deadlock avoided */
					            /* 11 was EAGAIN */
#define	ENOMEM		-12		/* Cannot allocate memory */
#define	EACCES		-13		/* Permission denied */
#define	EFAULT		-14		/* Bad address */
#define	ENOTBLK		-15		/* Block device required */
#define	EBUSY		-16		/* Device busy */
#define	EEXIST		-17		/* File exists */
#define	EXDEV		-18		/* Cross-device link */
#define	ENODEV		-19		/* Operation not supported by device */
#define	ENOTDIR		-20		/* Not a directory */
#define	EISDIR		-21		/* Is a directory */
#define	EINVALID	-22		/* Invalid argument */
#define	ENFILE		-23		/* Too many open files in system */
#define	EMFILE		-24		/* Too many open files */
#define	ENOTTY		-25		/* Inappropriate ioctl for device */
#define	ETXTBSY		-26		/* Text file busy */
#define	EFBIG		-27		/* File too large */
#define	ENOSPC		-28		/* No space left on device */
#define	ESPIPE		-29		/* Illegal seek */
#define	EROFS		-30		/* Read-only file system */
#define	EMLINK		-31		/* Too many links */
#define	EPIPE		-32		/* Broken pipe */

/* math software */
#define	EDOM		-33		/* Numerical argument out of domain */
#define	ERANGEE		-34		/* Result too large */

/* non-blocking and interrupt i/o */
#define	EAGAIN		-35		/* Resource temporarily unavailable */
#define	EWOULDBLOCK	EAGAIN		/* Operation would block */
#define	EINPROGRESS	-36		/* Operation now in progress */
#define	EALREADY	-37		/* Operation already in progress */

/* ipc/network software -- argument errors */
#define	ENOTSOCK	-38		/* Socket operation on non-socket */
#define	EDESTADDRREQ	-39		/* Destination address required */
#define	EMSGSIZE	-40		/* Message too LONG */
#define	EPROTOTYPE	-41		/* Protocol wrong type for socket */
#define	ENOPROTOOPT	-42		/* Protocol not available */
#define	EPROTONOSUPPORT	-43		/* Protocol not supported */
#define	ESOCKTNOSUPPORT	-44		/* Socket type not supported */
#define	EOPNOTSUPP	-45		/* Operation not supported */
#define	EPFNOSUPPORT	-46		/* Protocol family not supported */
#define	EAFNOSUPPORT	-47		/* Address family not supported by protocol family */
#define	EADDRINUSE	-48		/* Address already in use */
#define	EADDRNOTAVAIL	-49		/* Can't assign requested address */

/* ipc/network software -- operational errors */
#define	ENETDOWN	-50		/* Network is down */
#define	ENETUNREACH	-51		/* Network is unreachable */
#define	ENETRESET	-52		/* Network dropped connection on reset */
#define	ECONNABORTED	-53		/* Software caused connection abort */
#define	ECONNRESET	-54		/* Connection reset by peer */
#define	ENOBUFS		-55		/* No buffer space available */
#define EQDROP          -56      /* Drops a packet because queue us full */
#define	EISCONN		-57		/* Socket is already connected */
#define	ENOTCONN	-58		/* Socket is not connected */
#define	ESHUTDOWN	-59		/* Can't send after socket shutdown */
#define	ETOOMANYREFS	-60		/* Too many references: can't splice */
#define	ETIMEDOUT	-61		/* Operation timed out */
#define	ECONNREFUSED	-62		/* Connection refused */

#define	ELOOP		-63		/* Too many levels of symbolic links */

#define	ENAMETOOLONG	-64		/* File name too long */

/* should be rearranged */
#define	EHOSTDOWN	-65		/* Host is down */
#define	EHOSTUNREACH	-66		/* No route to host */
#define	ENOTEMPTY	-67		/* Directory not empty */

/* quotas & mush */
#define	EPROCLIM	-68		/* Too many processes */
#define	EUSERS		-69		/* Too many users */
#define	EDQUOT		-70  		/* Disc quota exceeded */

/* Network File System */
#define	ESTALE		-71		/* Stale NFS file handle */
#define	EREMOTE		-72		/* Too many levels of remote in path */
#define	EBADRPC		-73		/* RPC struct is bad */
#define	ERPCMISMATCH	-74		/* RPC version wrong */
#define	EPROGUNAVAIL	-75		/* RPC prog. not avail */
#define	EPROGMISMATCH	-76		/* Program version wrong */
#define	EPROCUNAVAIL	-77		/* Bad procedure for program */

#define	ENOLCK		-78		/* No locks available */
#define	ENOSYS		-79		/* Function not implemented */

#define	EFTYPE		-80		/* Inappropriate file type or format */
#define	EAUTH		-81		/* Authentication error */
#define	ENEEDAUTH	-82		/* Need authenticator */
#define	ELAST		-83		/* Must be equal largest errno */


/* pseudo-errors returned inside kernel to modify return to process */
#define	ERESTART	-84		/* restart syscall */
#define	EJUSTRETURN	-85		/* don't modify regs, just return	*/

#define ETASKCREATE	-86		/*	don't create task				*/
#define	ETASKDELETE	-87		/*	don't delete task				*/
#define	ETASKGETID	-88		/*	have not task					*/
#define	EPRISET		-89		/*	can't set pritioy				*/

#define	EEVRECEIVE	-90		/*	time is out						*/
#define	EEVSEND		-91		/*	ev_nowait is select				*/

#define	EQUCREATE	-92		/*	node's object table full		*/
#define	EQUDELETE	-93		/*	delete queue fail				*/
#define	EQUSEND		-94		/*	send message fail				*/
#define	EQURECEIVE	-95		/*	receive message fail			*/

#define	ESMCREATE	-96		/*	sm create fail					*/
#define	ESMDELETE	-97		/*	sm delete fail					*/
#define	ESMP		-98		/*	sm p fail						*/
#define	ESMV		-99		/*	sm v fail						*/

#define ETCBCHECK	-100		/*	has not find the tcb			*/
#define ETCBCREATE	-101		/*	has not create tcb				*/
#define ETCBDELETE	-102		/*	tcb has not delete				*/

#define	ENONBIO		-103
#define	ENOASYNC	-104
#define	ENOSETOWN	-105
#define	ENOGETOWN	-106
#define	ENOFIONBIO	-107
#define	ENOFIOASYNC	-108
#define ENOFIOBACKCALL  -109

#define	ECANTTCB	-107

#define	EUIO		-108

#define	ENOPROTO	-109
#define	ENOMG		-110

#define	EBLOCKING	-111
#define ENOTUDPSOCK -112
#define ETCBINUSE   -113

#define ESNDBUFHIWAT -114

/* Add new return value, zhangfeng, 2004,02,27 */
#define EMORECONTROL        -120
#define EVOIDINPCB              -121
#define EVOIDTCPCB              -122
/* zhangfeng, 2004,02,27 */

#define EINVAL		    -1		

#define EFULLSOCK    -200

#define ENOREMOTEHOST 1000
#define ENOREMOTEADDR 1001
#define ENOBUF        1002
#define ENOTBIND      1003  /*call unbind return this error for not bind yes*/
#endif

#define VOS_NTOHL ntohl
#define VOS_NTOHS ntohs
#define VOS_HTONL htonl
#define VOS_HTONS htons


/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
extern VOS_UINT32 VOS_SocketInit(VOID);
 
extern VOS_SOCKET VOS_Accept(VOS_SOCKET s, struct sockaddr *paddr, VOS_INT32 * paddrlen);
 
extern VOS_UINT32 VOS_Inet_addr(VOS_CHAR* pString);
 
extern VOS_UINT32 VOS_Bind(VOS_SOCKET s, struct sockaddr *paddr, VOS_INT32 addrlen);
 
extern VOS_UINT32 VOS_Connect(VOS_SOCKET s, struct sockaddr *paddr, VOS_INT32 addrlen);
 
extern VOS_UINT32 VOS_Listen(VOS_SOCKET s, VOS_INT32 backlog);
 
extern VOS_UINT32 VOS_Recv(VOS_SOCKET s, VOS_CHAR * buf, VOS_INT32 len, VOS_INT32 flags);
 
extern VOS_UINT32 VOS_Recvfrom(VOS_SOCKET s, VOS_CHAR * buf, VOS_INT32 len, VOS_INT32 flags,
 
                      struct sockaddr * pfrom, VOS_INT32 * pfromlen);
 
extern VOS_UINT32 VOS_Send(VOS_SOCKET s, VOS_CHAR * buf, VOS_INT32 len, VOS_INT32 flags);
 
extern VOS_UINT32 VOS_Sendto(VOS_SOCKET s, VOS_CHAR * buf, VOS_INT32 len, VOS_INT32 flags,
                       struct sockaddr * pto, VOS_INT32 tolen);
 
extern VOS_SOCKET VOS_Socket(VOS_INT32 af, VOS_INT32 type, VOS_INT32 protocol);
 
extern VOS_UINT32 VOS_Shutdown(VOS_SOCKET s, VOS_INT32 how);
 
extern VOS_UINT32 VOS_CloseSocket(VOS_SOCKET s);
 
extern VOS_UINT32 VOS_Getpeername(VOS_SOCKET s, struct sockaddr * paddr, VOS_INT32 * paddrlen);
 
extern VOS_UINT32 VOS_Getsockname(VOS_SOCKET s, struct sockaddr * paddr, VOS_INT32 * paddrlen);
 
extern VOS_UINT32 VOS_Getsockopt(VOS_SOCKET s, VOS_INT32 level, VOS_INT32 optname, 
                        VOS_CHAR * poptval, VOS_INT32 * poptlen);
 
extern VOS_UINT32 VOS_Setsockopt(VOS_SOCKET s, VOS_INT32 level, VOS_INT32 optname, 
                        VOS_CHAR * poptval, VOS_INT32 poptlen);
 
extern VOS_UINT32 VOS_IoctlSocket(VOS_SOCKET s, VOS_INT32 cmd, VOS_INT32* parg);
 
extern VOS_UINT32 VOS_Select(VOS_INT32 width, fd_set * preadfds, fd_set * pwritefds, 
                        fd_set * pexceptfds, struct timeval * ptimeout  );
 
extern struct hostent * VOS_Gethostbyname( CHAR *pszHostName);
 
extern VOS_UINT32 VOS_GetNetState();
 
extern VOS_VOID VOS_NetConnected();
 
extern VOS_VOID VOS_Inet_ntoa_b(VOS_UINT32 ulAddress, VOS_CHAR * pString);
 
extern VOS_UINT32 VOS_SetApnNameAndID( VOS_UINT32 ulApnID,VOS_CHAR cApnName[128] );


#ifdef __cplusplus
    }
#endif


#endif
