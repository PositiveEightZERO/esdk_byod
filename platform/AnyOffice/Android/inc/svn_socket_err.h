#ifndef _SVN_SOCKET_ERR_H_
#define _SVN_SOCKET_ERR_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define SVN_EPERM           -1      /* Operation not permitted */
#define SVN_EINVAL          -1      /* operation had invalid err */
#define SVN_ENOENT          -2      /* No such file or directory */
#define SVN_ESRCH           -3      /* No such process */
#define SVN_EINTR           -4      /* Interrupted system call */
#define SVN_EIO             -5      /* Input/output error */
#define SVN_ENXIO           -6      /* Device not configured */
#define SVN_E2BIG           -7      /* Argument list too long */
#define SVN_ENOEXEC         -8      /* Exec format error */
#define SVN_EBADF           -9      /* Bad file descriptor */
#define SVN_ECHILD          -10     /* No child processes */
#define SVN_EDEADLK         -11     /* Resource deadlock avoided */
#define SVN_ENOMEM          -12     /* Cannot allocate memory */
#define SVN_EACCES          -13     /* Permission denied */
#define SVN_EFAULT          -14     /* Bad address */
#define SVN_ENOTBLK         -15     /* Block device required */
#define SVN_EBUSY           -16     /* Device busy */
#define SVN_EEXIST          -17     /* File exists */
#define SVN_EXDEV           -18     /* Cross-device link */
#define SVN_ENODEV          -19     /* Operation not supported by device */
#define SVN_ENOTDIR         -20     /* Not a directory */
#define SVN_EISDIR          -21     /* Is a directory */
#define SVN_EINVALID        -22     /* Invalid argument */
#define SVN_ENFILE          -23     /* Too many open files in system */
#define SVN_EMFILE          -24     /* Too many open files */
#define SVN_ENOTTY          -25     /* Inappropriate ioctl for device */
#define SVN_ETXTBSY         -26     /* Text file busy */
#define SVN_EFBIG           -27     /* File too large */
#define SVN_ENOSPC          -28     /* No space left on device */
#define SVN_ESPIPE          -29     /* Illegal seek */
#define SVN_EROFS           -30     /* Read-only file system */
#define SVN_EMLINK          -31     /* Too many links */
#define SVN_EPIPE           -32     /* Broken pipe */

/* math software */
#define SVN_EDOM            -33     /* Numerical argument out of domain */
#define SVN_ERANGEE         -34     /* Result too large */

/* non-blocking and interrupt i/o */
#define SVN_EAGAIN          -35     /* Resource temporarily unavailable */
#define SVN_EWOULDBLOCK     SVN_EAGAIN  /* Operation would block */
#define SVN_EINPROGRESS     -36     /* Operation now in progress */
#define SVN_EALREADY        -37     /* Operation already in progress */

/* ipc/network software -- argument errors */
#define SVN_ENOTSOCK        -38     /* Socket operation on non-socket */
#define SVN_EDESTADDRREQ    -39     /* Destination address required */
#define SVN_EMSGSIZE        -40     /* Message too LONG */
#define SVN_EPROTOTYPE      -41     /* Protocol wrong type for socket */
#define SVN_ENOPROTOOPT     -42     /* Protocol not available */
#define SVN_EPROTONOSUPPORT -43     /* Protocol not supported */
#define SVN_ESOCKTNOSUPPORT -44     /* Socket type not supported */
#define SVN_EOPNOTSUPP      -45     /* Operation not supported */
#define SVN_EPFNOSUPPORT    -46     /* Protocol family not supported */
#define SVN_EAFNOSUPPORT    -47     /* Address family not supported by protocol family */
#define SVN_EADDRINUSE      -48     /* Address already in use */
#define SVN_EADDRNOTAVAIL   -49     /* Can't assign requested address */

/* ipc/network software -- operational errors */
#define SVN_ENETDOWN        -50     /* Network is down */
#define SVN_ENETUNREACH     -51     /* Network is unreachable */
#define SVN_ENETRESET       -52     /* Network dropped connection on reset */
#define SVN_ECONNABORTED    -53     /* Software caused connection abort */
#define SVN_ECONNRESET      -54     /* Connection reset by peer */
#define SVN_ENOBUFS         -55     /* No buffer space available */
#define SVN_EQDROP          -56     /* Drops a packet because queue us full */
#define SVN_EISCONN         -57     /* Socket is already connected */
#define SVN_ENOTCONN        -58     /* Socket is not connected */
#define SVN_ESHUTDOWN       -59     /* Can't send after socket shutdown */
#define SVN_ETOOMANYREFS    -60     /* Too many references: can't splice */
#define SVN_ETIMEDOUT       -61     /* Operation timed out */
#define SVN_ECONNREFUSED    -62     /* Connection refused */
#define SVN_ELOOP           -63     /* Too many levels of symbolic links */
#define SVN_ENAMETOOLONG    -64     /* File name too long */

/* should be rearranged */
#define SVN_EHOSTDOWN       -65     /* Host is down */
#define SVN_EHOSTUNREACH    -66     /* No route to host */
#define SVN_ENOTEMPTY       -67     /* Directory not empty */

/* quotas & mush */
#define SVN_EPROCLIM        -68     /* Too many processes */
#define SVN_EUSERS          -69     /* Too many users */
#define SVN_EDQUOT          -70     /* Disc quota exceeded */

/* Network File System */
#define SVN_ESTALE          -71     /* Stale NFS file handle */
#define SVN_EREMOTE         -72     /* Too many levels of remote in path */
#define SVN_EBADRPC         -73     /* RPC struct is bad */
#define SVN_ERPCMISMATCH    -74     /* RPC version wrong */
#define SVN_EPROGUNAVAIL    -75     /* RPC prog. not avail */
#define SVN_EPROGMISMATCH   -76     /* Program version wrong */
#define SVN_EPROCUNAVAIL    -77     /* Bad procedure for program */
#define SVN_ENOLCK          -78     /* No locks available */
#define SVN_ENOSYS          -79     /* Function not implemented */
#define SVN_EFTYPE          -80     /* Inappropriate file type or format */
#define SVN_EAUTH           -81     /* Authentication error */
#define SVN_ENEEDAUTH       -82     /* Need authenticator */
#define SVN_ELAST           -83     /* Must be equal largest errno */

/* pseudo-errors returned inside kernel to modify return to process */
#define SVN_ERESTART        -84     /* restart syscall */
#define SVN_EJUSTRETURN     -85     /* don't modify regs, just return */
#define SVN_ETASKCREATE     -86     /* don't create task */
#define SVN_ETASKDELETE     -87     /* don't delete task */
#define SVN_ETASKGETID      -88     /* have not task */
#define SVN_EPRISET         -89     /* can't set pritioy */
#define SVN_EEVRECEIVE      -90     /* time is out */
#define SVN_EEVSEND         -91     /* ev_nowait is select */
#define SVN_EQUCREATE       -92     /* node's object table full */
#define SVN_EQUDELETE       -93     /* delete queue fail */
#define SVN_EQUSEND         -94     /* send message fail */
#define SVN_EQURECEIVE      -95     /* receive message fail */
#define SVN_ESMCREATE       -96     /* sm create fail */
#define SVN_ESMDELETE       -97     /* sm delete fail */
#define SVN_ESMP            -98     /* sm p fail */
#define SVN_ESMV            -99     /* sm v fail */
#define SVN_ETCBCHECK       -100    /* has not find the tcb */
#define SVN_ETCBCREATE      -101    /* has not create tcb */
#define SVN_ETCBDELETE      -102    /* tcb has not delete */
#define SVN_ENONBIO         -103    /* SVN_ENONBIO */
#define SVN_ENOASYNC        -104    /* SVN_ENOASYNC */
#define SVN_ENOSETOWN       -105    /* SVN_ENOSETOWN */
#define SVN_ENOGETOWN       -106    /* SVN_ENOGETOWN */
#define SVN_ENOFIONBIO      -107    /* SVN_ENOFIONBIO */
#define SVN_ENOFIOASYNC     -108    /* SVN_ENOFIOASYNC */
#define SVN_ENOFIOBACKCALL  -109    /* SVN_ENOFIOBACKCALL */
#define SVN_ECANTTCB        -107    /* SVN_ECANTTCB */
#define SVN_EUIO            -108    /* SVN_EUIO */
#define SVN_ENOPROTO        -109    /* SVN_ENOPROTO */
#define SVN_ENOMG           -110    /* SVN_ENOMG */
#define SVN_EBLOCKING       -111    /* SVN_EBLOCKING */
#define SVN_ENOTUDPSOCK     -112    /* SVN_ENOTUDPSOCK */
#define SVN_ETCBINUSE       -113    /* SVN_ETCBINUSE */
#define SVN_ESNDBUFHIWAT    -114    /* SVN_ESNDBUFHIWAT */
#define SVN_EMORECONTROL    -120    /* SVN_EMORECONTROL */
#define SVN_EVOIDINPCB      -121    /* SVN_EVOIDINPCB */
#define SVN_EVOIDTCPCB      -122    /* SVN_EVOIDTCPCB */

#define SVN_ENOREMOTEHOST   1000    /* SVN_ENOREMOTEHOST */
#define SVN_ENOREMOTEADDR   1001    /* SVN_ENOREMOTEADDR */
#define SVN_ENOBUF          1002    /* SVN_ENOBUF */
#define SVN_ENOTBIND        1003    /* call unbind return this error for not bind yes */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
