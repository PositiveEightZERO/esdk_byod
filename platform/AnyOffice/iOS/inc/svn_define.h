#ifndef __SVN_DEFINE_H__
#define __SVN_DEFINE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* BEGIN: Added for PN:内存优化 by huangyiyuan hkf60364, 2013/5/10 */
/* BEGIN: Added for PN:xxxxx by zhaixianqi 90006553, 2012/9/13 */
#define SVN_MAX_MEMORY 6
/* END:   Added for PN:xxxxx by zhaixianqi 90006553, 2012/9/13 */
/* END:   Added for PN:内存优化 by huangyiyuan hkf60364, 2012/5/10 */



/* 客户端支持的最大代理相关信息 */
#define SVN_MAX_PROXY_URL_LEN               128     /* 代理服务器IP最大长度 */
#define SVN_MAX_PROXY_USERNAME_LEN          256     /* 代理用户名最大长度 */
#define SVN_MAX_PROXY_USERPWD_LEN           256     /* 代理密码最大长度 */
#define SVN_MAX_PROXY_DOMAIN_LEN            128     /* 域名最大长度 */

/* APN名字的最大长度 */
#define SVN_MAX_APNNAME_LEN                 128     /* APN名最大长度 */

/* 用户认证id的最大长度 */
/* BEGIN: Modefied by liushangshu WX80847 for 新需求增加SDK连接, 2013/8/1 */
#define SVN_MAX_AUTH_ID_LEN                 64    
/* END: Modefied by liushangshu WX80847 for 新需求增加SDK连接, 2013/8/1 */


/* SVN网关支持的最大用户名和密码长度 */
/* BEGIN: Modified by zhaixianqi 90006553, 2013/11/30 for 
    GBK转UTF8编码后再进行URL编码，字符转码后最大长度为 62/2*3*3 + 3 = 282
    问题单号:DTS2013112611858 */
#define SVN_MAX_USERNAME_LEN            ((64 / 2) * 3 * 3)      /* SVN网关用户名最大长度 288 */
/* END:   Modified by zhaixianqi 90006553, 2013/11/30 */
#define SVN_MAX_USERPWD_LEN             32      /* SVN网关密码最大长度 */
#define SVN_MAX_URL_LEN                 128     /* 虚拟网关最大长度 */
/* BEGIN: Added by liushangshu WX80847 for 新需求增加SDK连接, 2013/5/7 */
#define SVN_MAX_APP_NAME_LEN            32     /*客户应用名最大长度*/
#define SVN_MAX_DEVICE_ID_LEN           64     /*客户deviceid最大长度*/
/* END: Added by liushangshu WX80847 for 新需求增加SDK连接, 2013/5/7 */
#define SVN_MAX_ICON_LEN                256

#define SVN_TOKEN_MAX_LEN 68


/* BEGIN: Added by liushangshu WX80847 for 新需求增加SDK连接, 2013/7/25 */
#define SVN_CONNECT_TYPE_E2E           3      /*端到端类型链接*/
#define SVN_CONNECT_TYPE_MTM           2      /*正常的多媒体隧道链接*/
#define SVN_CONNECT_TYPE_ANYOFFICE     1      /*Anyoffice 链接*/
#define SVN_CONNECT_TYPE_SDK           0      /*SDK 链接*/
/* END: Added by liushangshu WX80847 for 新需求增加SDK连接, 2013/7/25 */


/* 解析域名时，解析结果最大返回数为10个 */
#define SVN_MAX_URL_NUM                 10

/* 隧道传输模式*/
#define SVN_TUNNEL_MODE_DTLS            0       /* 隧道模式DTLS */
#define SVN_TUNNEL_MODE_TLS             1       /* 隧道模式TLS */
#define SVN_TUNNEL_MODE_UDP             2       /* 隧道模式UDP */
#define SVN_TUNNEL_MODE_UDPS            3       /* 隧道模式TLS + UDPS */

/*BEGIN:Add by liushangshu for Tcp over Udp.2013.06.06*/
/*TCP OVERUDP开关*/
#define SVN_UDP_OVER_UDP_OFF            0       /* TCP OVERUDP关 */
#define SVN_UDP_OVER_UDP_ON             1       /* TCP OVERUDP开 */
/*END:Add by liushangshu for Tcp over Udp.2013.06.06*/

/* 组件初始化控制参数掩码 */
#define SVN_ENV_CONTROL_TYPE_MEN_SIZE   1       /* 组件初始化时，申请的内存大小参数类型值 */
#define SVN_ENV_CONTROL_TYEP_SSL_TIME    2      /*SSL握手超时时间，单位S*/

/* 组件初始化控制参数结构体，后续若是新增控制参数的对应值，则在该结构体内扩展，具体使用时，根据控制参数掩码掩码取值 */
typedef struct tagSVNInitConctrolInfo
{
    unsigned  long ulMemorySize;                /* 初始化时需要申请的内存大小，单位MB，最小3，最大6 */
    int iSslConnTimeout;         /*SSL握手超时时间设置*/
} SVN_INIT_CONTROL_S;


#define WM_SVN_MESSAGE_TYPE_BASE         0x1000    /* Windows状态通知消息类型的基础偏移值，用于计算Windows消息通知类型与隧道状态的对应关系 */

/*----------------------------SVN隧道状态定义-----------------------------*/
/* SVN隧道状态标准定义（适用于除Windows外的各平台） */
#define SVN_STATUS_OFFLINE              0x0       /* 表示隧道不可用 */
#define SVN_STATUS_ONLINE               0x1       /* 表示隧道在线   */
#define SVN_STATUS_CONNECTING           0x2       /* 表示处于登录或注销隧道过程中，或正在进行隧道重连 */

/* Windows平台下SVN隧道状态定义 （Windows平台下必须使用以下定义）*/
#define WM_SVN_STATUS_OFFLINE           (WM_SVN_MESSAGE_TYPE_BASE + SVN_STATUS_OFFLINE)        /* windows平台下，隧道不可用 */
#define WM_SVN_STATUS_ONLINE            (WM_SVN_MESSAGE_TYPE_BASE + SVN_STATUS_ONLINE)         /* windows平台下，隧道在线 */
#define WM_SVN_STATUS_CONNECTING        (WM_SVN_MESSAGE_TYPE_BASE + SVN_STATUS_CONNECTING)     /* windows平台下，表示处于登录或注销隧道过程中，或正在进行隧道重连 */
/*-----------------------------------------------------------------------*/


/* 定义组件通知消息的类型,组件消息通知回调函数处理时使用 */
typedef enum enNEM_NotifyState 
{
    SVN_NOTIFY_STATUS                 = 0,  /* SVN隧道状态通知消息 ,隧道状态发生变化时通知当前隧道状态及相应错误码 */
    SVN_NOTIFY_USER_LOCK              = 1   /* 用户登录密码错误时，若后续输错会被SVN安全设备锁定用户，则通知上层，附带剩余尝试次数 */

}SVN_NOTIFY_STATE_EN;


/* 隧道不可用的几种错误码 */
#define SVN_TCP_CONNECT_ERR             -1      /* 创建TCP连接失败，请检查网络连接和网关的地址和端口是否正确。对于移动终端需要确保网络已激活 */
#define SVN_PROXY_CONNECT_ERR           -2      /* 与代理服务建立连接失败，请检查网络和代理服务器地址和端口是否正确 */
#define SVN_PROXY_INFO_ERR              -3      /* 代理信息错误，请检查代理用户名、密码、域信息是否正确 */
#define SVN_TLS_HANDSHAKE_ERR           -4      /* TLS握手失败*/
#define SVN_USER_INFO_ERR               -5      /* 登录SVN的用户名、密码错误 */
#define SVN_VIP_UNAVAILABLE             -6      /* 无法获取虚拟IP */
#define SVN_USER_EXCEED_LIMIT           -7      /* 用户数达到上线 */
#define SVN_USER_IP_DENY                -8      /* 用户IP受限 */
#define SVN_TUNNEL_DISABLED             -9      /* 多媒体隧道功能未开启 */
#define SVN_USERID_INVALID              -10     /* 用户ID无效 */
#define SVN_TUNNEL_CLOSED               -11     /* 隧道关闭，用户被踢下线*/
#define SVN_UDPS_TUNNEL_BLOCK           -12     /* 登录SVN网关时,UDP隧道探测超时失败,请检查网络状况 */
#define SVN_SERVER_VERIFY_FAILED        -13     /* 登录SVN网关时,服务器证书的CA不匹配,校验失败 */
#define SVN_VERIFY_CLIENT_CERT_ERR      -14     /* 登录SVN网关时,客户端证书不匹配,校验失败 */
#define SVN_USER_LOCKED                 -15     /* 用户被锁定，无法登录 */
#define SVN_USER_AUTH_ID_ERR            -16     /* auth id方式登录方式，用户名与auth id不匹配，无法登录 */
#define SVN_USER_AUTH_CERT_NOT_MATCH    -17     /* 客户端证书和绑定证书不匹配 */
#define SVN_USER_AUTH_MDM_SERVER_ERR    -18    /* MDM Server 错误 */
#define SVN_GATEWAY_EXCEPTION           -99     /* 网关运行异常 */
#define SVN_SYSTEM_EXCEPTION            -100    /* 组件运行异常 */



/* 加解密接口错误码定义 */
#define SVN_CRYPT_PARAMETER_ERR         2000    /* 加解密入参错误，ULONG值 */
#define SVN_CRYPT_MEMORY_ERR            2001    /* 加解密后出参BUFF长度不足，ULONG值 */



/* 错误代码定义 */
#define SVN_OK              0       /* 零 */
#define SVN_ERR             1       /* 非零 */
#define SVN_ERR_PARAM       2       /* 输入参数错误 */
#define SVN_ERR_EXIST       3       /* 已经有业务在运行，DLL与控件、独立客户端互斥 */

/* BOOL类型定义 */
#define SVN_TRUE            1
#define SVN_FALSE           0



/* 代理类型 */
#define SVN_PROXY_NONE      0      /* 不使用代理　*/
#define SVN_PROXY_HTTP      1      /* 使用HTTP代理 */
#define SVN_PROXY_SOCKS5    2      /* 使用SOCK5代理 */
#define SVN_PROXY_UNKNOWN   3      /* 使用未知代理 */



/* 代理信息 */
typedef struct tagSVNProxyInfo
{
    char            acProxyUrl[SVN_MAX_PROXY_URL_LEN];              /* 代理服务器IP或域名 */
    unsigned short  usProxyPort;                                    /* 代理服务器端口 */
    unsigned short  usProxyType;                                    /* 代理类型，SYS_PROXY_NO/SYS_PROXY_HTTP/SYS_PROXY_SOCKS5 */
    char            acProxyUserName[SVN_MAX_PROXY_USERNAME_LEN];    /* 代理用户名 */
    char            acProxyPassword[SVN_MAX_PROXY_USERPWD_LEN];     /* 代理密码 */
    char            acProxyDomain[SVN_MAX_PROXY_DOMAIN_LEN];        /* 认证域名 */
} SVN_PROXY_INFO_S;

#define MAX_INFO_ITEM_LEN 64
typedef struct enInfoByServiceTicket
{
    char acEmpid[MAX_INFO_ITEM_LEN];
    char acOdlempid[MAX_INFO_ITEM_LEN];
    char acUid[MAX_INFO_ITEM_LEN];
}INFO_BY_SERVICETICKET;

/*注册响应回调函数*/
typedef int (*SVN_StatusCallback)(unsigned int uiStatus, int iErrorCode);

/*注册svn版本号回调函数*/
typedef void (*SVN_EntryVersionCallback)(unsigned char *pucEntryVersion);

/* 组件消息通知回调函数 */
typedef int (*SVN_StatusNotifyCallback)(unsigned int iMessageType, void* pWPara, void* pLPara);


/* 日志回调函数指针 */
typedef int (*SVN_WriteLogCallback) (unsigned char *pucLog, unsigned long ulLogLen, unsigned long ulLogLevel);

/* Ping回调函数指针 */
typedef void (*SVN_PingCallback) (unsigned char *pucPingBuffer); 

/* Traceroute回调函数指针 */
typedef void (*SVN_TracertCallback) (unsigned char *pucTracertBuffer);

/* BEGIN: Modified for 问题单号:L4VPN支持DNS域名解析并发 by y90004712, 2013/11/26 */
/* 域名解析回调函数指针，用于向上层传递解析后的域名 */
typedef void (*SVN_ParseURLCallback) (unsigned long ulIP[SVN_MAX_URL_NUM], void *pvData);
/* END:   Modified by y90004712, 2013/11/26 */
typedef  void (*SVN_GetServiceTicketCallback) (unsigned long ret, const unsigned char * serviceTicket);
typedef void (*SVN_GetInfoByServiceTicketCallBack)(INFO_BY_SERVICETICKET  stInfoByServiceTicket);
/*注册信息*/
typedef struct tagSVNRegisterInfo
{
    SVN_StatusCallback    pfStatusCallback;                   /* 在windows平台为接收状态通知的窗口句柄，在其他平台为状态通知回调函数指针 */
    SVN_WriteLogCallback  pfWriteLogCallback;                 /* 日志输出回调函数指针 */
    char                  acServerURL[SVN_MAX_URL_LEN];       /* 网关地址，例如：9.1.1.3 或 www.abc.com */
    unsigned short        usServerPort;                       /* 网关的端口 */
    unsigned short        usTunnelMode;                       /* 隧道模式 */
    char                  acUserName[SVN_MAX_USERNAME_LEN];   /* 注册SVN网关用户名 */
    char                  acPassword[SVN_MAX_USERPWD_LEN];    /* 注册SVN网关密码  */
    unsigned long         ulAPNID;                            /* APN的ID，只有在symbian平台有效 */
    char                  acAPNName[SVN_MAX_APNNAME_LEN];     /* APN名字，只有在symbian平台有效 */
    char                  acAuthId[SVN_MAX_AUTH_ID_LEN];      /* 用户认证id */
    SVN_PROXY_INFO_S      stProxyInfo;                        /* 代理信息结构 */
    /* BEGIN: Added by liushangshu WX80847 for 新需求增加SDK连接, 2013/5/7 */
    char                  acAPPName[SVN_MAX_APP_NAME_LEN];    /*应用名*/     
    char                  acDeviceID[SVN_MAX_DEVICE_ID_LEN];       /*设备ID*/        
    /* END: Added by liushangshu WX80847 for 新需求增加SDK连接, 2013/5/7 */

    unsigned char         aucToken[SVN_TOKEN_MAX_LEN];
    
    /* BEGIN: Added by liushangshu WX80847 for 新需求增加SDK连接, 2013/7/25 */
    unsigned long         ulConnectType ;  
    /* END: Added by liushangshu WX80847 for 新需求增加SDK连接, 2013/7/25 */
    /* BEGIN: Added by chenfeng 90004813 for 增加svn设备版本号, 2015-4-17 */
    SVN_EntryVersionCallback pfEntryVersionCallback;
    /* END: Added by chenfeng 90004813 for 增加svn设备版本号, 2015-4-17 */
} SVN_REGISTER_INFO_S;



/* 诊断功能所需宏定义 */
#define SVN_SWITCH_STATE_ON            1  /* 诊断开关为开 */
#define SVN_SWITCH_STATE_OFF           0  /* 诊断开关为关 */



/* 定义日志级别 */
#define SVN_ERR_LOG         1    /* 程序无法正常完成操作，比如网络异常断开、数据收发异常、内存申请失败、入参检查错误等 */

#define SVN_WARN_LOG        2    /* 进入一个异常分支，但并不会引起程序错误，例如用户名或密码错误、终端安全检查错误、线程正常退出、被踢下线等 */

#define SVN_NOTICE_LOG      3    /* 日常运行提示信息，比如客户端登录、退出日志 */

#define SVN_DEBUG_LOG       4    /* 调试信息，打印比较频繁，打印内容多，例如报文内容打印、函数入口出口追踪日志等 */


/* 统计与监控模块结构体定义 */
typedef struct tagSVNStatisticInfo
{    
    unsigned long ulVSockRecvTCPLen;        /*接收的TCP报文数据长度，在组件TCP加密通信接口处统计*/
   
    unsigned long ulVSockSendTCPLen;        /*发送的TCP报文数据长度，在组件TCP加密通信接口处统计*/
   
    unsigned long ulVSockRecvUDPPktNum;     /*接收的UDP报文个数，在组件UDP加密通信接口中统计*/
    
    unsigned long ulVSockSendUDPPktNum;     /*发送的UDP报文个数，在组件UDP加密通信接口中统计*/
   
    unsigned long ulSockRecvUDPPktNum;      /*接收的UDP报文个数，在调用操作系统UDP通信接口处统计*/
    
    unsigned long ulSockSendUDPPktNum;      /*发送的UDP报文个数，在调用操作系统UDP通信接口处统计*/
    
    unsigned long ulSockSendUDPSpeed;       /*发送UDP报文的速率，在调用操作系统UDP通信接口处统计*/
   
    unsigned long ulSockRecvUDPSpeed;       /*接收UDP报文的速率，在调用操作系统UDP通信接口处统计*/
   
    unsigned long ulTLSConnTimes;           /* TLS连接建立请求次数 */
    
    unsigned long ulTLSConnFailTimes;       /* TLS连接失败次数 */
    
    unsigned long ulTLSReConnTimes;         /* TLS重建次数 */
   
    unsigned long ulTLSReConnFailTimes;     /* TLS重建失败的次数 */
    
    unsigned long ulRecvRTPDelay;           /* RTP报文组件接收时延统计 单位：毫秒 */
    
    unsigned long ulSendRTPDelay;           /* RTP报文组件发送时延统计 单位：毫秒 */
   
    unsigned long ulFreeMem;                /* 组件空闲内存 单位 KByte */
    
    unsigned long ulUsedMem;                /* 组件使用内存 单位 KByte */

}SVN_STATISTIC_INFO_S;

/* 证书导入错误码定义 */
#define SVN_ERR_NO_KEY              21     /* 没有私钥 */
#define SVN_ERR_TYPE_MISMATCH       22     /* 证书和私钥类型不匹配 */
#define SVN_ERR_VALUE_MISMATCH      23     /* 证书和私钥内容不匹配 */
#define SVN_ERR_UNKNOWN_TYPE        24     /* 不可识别的类型 */
#define SVN_ERR_CN_MISMATCH         25     /* 客户端证书中的CN和传入的CN不匹配 */

#define SVN_ERR_CA_IMPORT_SIZE_ERROR        26 /* 导入文件中证书大小超过3k */
#define SVN_ERR_CA_IMPORT_ERROR             27 /* 导入证书失败 */
#define SVN_ERR_CA_IMPORT_NUM_LIMIT_ERROR   28 /* 系统CA证书数已达上限 */
#define SVN_ERR_CA_IMPORT_LEVEL_LIMIT_ERROR 29 /* 导入证书级数超过10 */
#define SVN_ERR_CA_IMPORT_VALUE_ERROR       30 /* 导入证书内容非法*/


/* 支持的客户端证书类型:PEM.DER.PKCS12 */
#define SVN_CERT_PEM           1    
#define SVN_CERT_DER           2
#define SVN_CERT_PKCS12        3

/* 支持的客户端私钥类型:PEM.DER */
#define SVN_PRIVATEKEY_PEM     1 
#define SVN_PRIVATEKEY_DER     2

/* 支持的服务端CA证书类型:PEM.DER.PKCS7 */
#define SVN_CACERT_PEM         1
#define SVN_CACERT_DER         2
#define SVN_CACERT_PKCS7       3

/* RSA私钥加密密钥的最大长度(DES_CBC加密算法) */
#define SVN_MAX_RSA_PWD_LEN     256

/* CA证书的级数上限 */
#define SVN_CA_LEVEL_NUM        9

/* 导入的设备CA证书个数限定 */
#define SVN_CERTIFICATE_NUM     80

/* 单张证书（包括客户端证书以及CA证书）内容长度限定 */
#define SVN_CERTIFICATE_MAX_LEN     (3*1024)


/* RSA密钥位数:字节 */
#define SVN_RSA_KeyBits_512     512
#define SVN_RSA_KeyBits_1024    1024
#define SVN_RSA_KeyBits_2048    2048
#define SVN_RSA_KeyBits_4096    4096

/* 绿色组件:数字证书各项内容结构体定义 */
typedef struct tagEntry
{
    char *key;      /* 关键字：用于导入内容时检索实体项 */
    char *value;    /* 内容：用于填充到实体项 */
} SVN_CERT_ENTRY_S;


/* 绿色组件:客户端证书结构体定义 */
typedef struct tagSVNClientCertInfoInput
{
    unsigned char          *pucClientCert;       /* 绿色客户端证书内容Buffer */
    unsigned char          *pucPrivatekey;       /* 绿色客户端证书对应的私钥内容Buffer */
    unsigned long          ulClientCertLen;      /* 证书长度（字节数）*/
    unsigned long          ulPrivateKeyLen;      /* 私钥长度（字节数）*/
    unsigned short         usClientCertType;     /* 证书格式类型 */
    unsigned short         usPrivateKeyType;     /* 私钥格式类型 */
    unsigned char          *pszPrivateKeyPwd;    /* 加密密码（如果有则设置此变量，否则入参NULL）*/
}SVN_CLIENTCERT_INFO_S;


/* 绿色组件:设备CA证书结构体定义 */
typedef struct tagSVNServerCACertInfoInput
{
    struct tagSVNServerCACertInfoInput  *pstServerCACertNext;  /* 指向下一条CA证书链 */
    unsigned char                       *pszServerCACert;      /* 服务端CA证书（链）内容Buffer */
    unsigned long                       ulServerCACertLen;     /* Buffer长度（字节数）*/
    unsigned short                      usServerCACertType;    /* CA证书格式类型 */
    unsigned short                      usReserve;             /* 为字节对齐，设置保留位 */
}SVN_SERVERCACERT_INFO_S;

/* 隧道状态错误码结构体 */
typedef struct tagSVNTunnelStatusInfo
{
    unsigned int    uiStatus;       /* 隧道状态 */
    int             iErrorCode;     /* 错误码 */
} SVN_TUNNEL_STATUS_NOTIFY_S;

/* 用户锁定信息结构体定义 */
typedef struct tagSVNUserLockInfo
{
    unsigned char ucRemanentTimes;  /* 用户密码剩余尝试次数 */
} SVN_USER_LOCK_NOTIFY_S;
/*BEGIN: add by tianxin KF35598 2012-8-7*/
typedef unsigned long (*SVN_RegFuc)(unsigned char* , unsigned short);
typedef struct tagSvnCallbackOps
{
    unsigned char ucRegFlags;
    SVN_RegFuc pfProcessFuc;
}CMTM_SVN_CALLBACKOPS_S;
/*END: add by tianxin KF35598 2012-8-7*/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SVN_DEFINE_H__ */
