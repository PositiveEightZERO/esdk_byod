
/******************************************************************************
*    文件名称 :    svn_http.h
*    作    者 :    华为技术有限公司(Huawei Tech.Co.,Ltd)
*    文件描述 :    HTTP模块对外API头文件
*    创建时间 :    2012-12-11
*    版权声明 :    Copyright (C) 2012
*                  华为技术有限公司(Huawei Tech.Co.,Ltd)
*    修订历史 :    2012-12-11    1.0.1
******************************************************************************/

#ifndef __svnhttp_h_
#define __svnhttp_h_


#include <stdio.h>

#ifdef __cplusplus
extern "C" {    /* Assume C declarations for C++ */
#endif  /* __cplusplus */


#if (defined(WIN32) || defined(_WIN32))
#define HTTP_API  __declspec(dllexport)
#else

#define HTTP_API   __attribute__ ((__visibility__ ("default")))
#endif


typedef unsigned long HTTPHANDLE;
typedef unsigned long HTTPULong;
typedef long LONG;
typedef int HTTPInt;
typedef unsigned int HTTPUInt;

typedef char HTTPChar;

typedef HTTPULong HTTPBOOL;

#define HTTP_TRUE 1
#define HTTP_FALSE 0

#define HTTP_FAILURE (-1)
#define HTTP_SUCCESS (0)

#ifdef __cplusplus
#define HTTP_NULL    0
#else
#define HTTP_NULL    ((void *)0)
#endif

#define     EN_HTTP_CURLE_ERR_START       100    /*返回的错误码映射到HTTP错误码基准值*/

#define     HTTP_DEFAULT_HANDLE_NUM       16     /*默认支持的handle个数*/
#define     HTTP_MAX_HANDLE_NUM           500    /*最大支持的handle个数*/
#define     HTTP_MIN_HANDLE_NUM           1      /*最小支持的handle个数*/


/* 错误码 */
typedef enum{
    EN_HTTP_OK,
    EN_HTTP_INIT_FAIL,                      /* 1 初始化失败 */
    EN_HTTP_UNINIT,                         /* 2 未初始化 */
    EN_HTTP_INVALID_PARAM,                  /* 3 无效参数 */
    EN_HTTP_INVALID_HANDLE,                 /* 4 无效句柄 */ 
    EN_HTTP_HANDLE_INUSE,                   /* 5 当前句柄正在使用中，不可设置 */ 
    EN_HTTP_HANDLE_STATE_ERROR,             /* 6 当前句柄状态不支持对应操作 */
    EN_HTTP_ALLOC_MEM_FAIL,                 /* 7 内存分配失败 */
    EN_HTTP_CREATE_HANDLE_FAIL,             /* 8 分配handle失败 */
    EN_HTTP_INVALID_TLS_MODE,               /* 9 tls模式错误 */
    EN_HTTP_INVALID_URL,                    /* 10 无效的URL */
    EN_HTTP_INVALID_HEAD_TYPE,              /* 11 无效的头域类型 */
    EN_HTTP_OPEN_LOG_FAIL,                  /* 12 打开日志失败 */
    EN_HTTP_WRITE_LOG_FAIL,                 /* 13 写日志失败 */
    EN_HTTP_INVALID_FILEPATH,               /* 14 无效的文件路径 */
    EN_HTTP_HANDLE_RELEASED,                /* 15 handle已经被释放 */
    EN_HTTP_CREATE_MUTEX_FAIL,              /* 16 创建mutex失败 */
    EN_HTTP_LOCK_MUTEX_FAIL,                /* 17 LOCK mutex失败 */
    EN_HTTP_UNLOCK_MUTEX_FAIL,              /* 18 UNLOCK mutex失败 */
    EN_HTTP_CREATE_THREAD_FAIL,             /* 19 创建线程失败 */
    EN_HTTP_NOT_SUPPORT,                    /*20 不支持该功能 */   
    EN_HTTP_GETHOSTNAME_FAIL,               /* 26 获取主机名失败 */
    EN_HTTP_NET_NTOA_FAIL,                  /*27 IP 地址转换失败*/
    EN_HTTP_RESPONSE_PARSE_ERR,             /* 28 响应数据解析失败 */


    EN_HTTP_CURLE_UNSUPPORTED_PROTOCOL = EN_HTTP_CURLE_ERR_START + 1,    /* 101 */
    EN_HTTP_CURLE_FAILED_INIT,             /* 102 */
    EN_HTTP_CURLE_URL_MALFORMAT,           /* 103 */
    EN_HTTP_CURLE_NOT_BUILT_IN,            /* 104 */
    EN_HTTP_CURLE_COULDNT_RESOLVE_PROXY,   /* 105 */
    EN_HTTP_CURLE_COULDNT_RESOLVE_HOST,    /* 106 */
    EN_HTTP_CURLE_COULDNT_CONNECT,         /* 107 */
    EN_HTTP_CURLE_FTP_WEIRD_SERVER_REPLY,  /* 108 */
    EN_HTTP_CURLE_REMOTE_ACCESS_DENIED,    /* 109 a service was denied by the server
                                              due to lack of access - when login fails
                                              this is not returned. */
    EN_HTTP_CURLE_FTP_ACCEPT_FAILED,       /* 110 */
    EN_HTTP_CURLE_FTP_WEIRD_PASS_REPLY,    /* 111 */
    EN_HTTP_CURLE_FTP_ACCEPT_TIMEOUT,      /* 112 - timeout occurred accepting server*/
    EN_HTTP_CURLE_FTP_WEIRD_PASV_REPLY,    /* 113 */
    EN_HTTP_CURLE_FTP_WEIRD_227_FORMAT,    /* 114 */
    EN_HTTP_CURLE_FTP_CANT_GET_HOST,       /* 115 */
    EN_HTTP_CURLE_OBSOLETE16,              /* 116 - NOT USED */
    EN_HTTP_CURLE_FTP_COULDNT_SET_TYPE,    /* 117 */
    EN_HTTP_CURLE_PARTIAL_FILE,            /* 118 */
    EN_HTTP_CURLE_FTP_COULDNT_RETR_FILE,   /* 119 */
    EN_HTTP_CURLE_OBSOLETE20,              /* 120 - NOT USED */
    EN_HTTP_CURLE_QUOTE_ERROR,             /* 121 - quote command failure */
    EN_HTTP_CURLE_HTTP_RETURNED_ERROR,     /* 122 */
    EN_HTTP_CURLE_WRITE_ERROR,             /* 123 */
    EN_HTTP_CURLE_OBSOLETE24,              /* 124 - NOT USED */
    EN_HTTP_CURLE_UPLOAD_FAILED,           /* 125 - failed upload "command" */
    EN_HTTP_CURLE_READ_ERROR,              /* 126 - couldn't open/read from file */
    EN_HTTP_CURLE_OUT_OF_MEMORY,           /* 127 */
                                           /* Note: CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error
                                              instead of a memory allocation error if CURL_DOES_CONVERSIONS
                                              is defined*/
    EN_HTTP_CURLE_OPERATION_TIMEDOUT,      /* 128 - the timeout time was reached */
    EN_HTTP_CURLE_OBSOLETE29,              /* 129 - NOT USED */
    EN_HTTP_CURLE_FTP_PORT_FAILED,         /* 130 - FTP PORT operation failed */
    EN_HTTP_CURLE_FTP_COULDNT_USE_REST,    /* 131 - the REST command failed */
    EN_HTTP_CURLE_OBSOLETE32,              /* 132 - NOT USED */
    EN_HTTP_CURLE_RANGE_ERROR,             /* 133 - RANGE "command" didn't work */
    EN_HTTP_CURLE_HTTP_POST_ERROR,         /* 134 */
    EN_HTTP_CURLE_SSL_CONNECT_ERROR,       /* 135 - wrong when connecting with SSL */
    EN_HTTP_CURLE_BAD_DOWNLOAD_RESUME,     /* 136 - couldn't resume download */
    EN_HTTP_CURLE_FILE_COULDNT_READ_FILE,  /* 137 */
    EN_HTTP_CURLE_LDAP_CANNOT_BIND,        /* 138 */
    EN_HTTP_CURLE_LDAP_SEARCH_FAILED,      /* 139 */
    EN_HTTP_CURLE_OBSOLETE40,              /* 140 - NOT USED */
    EN_HTTP_CURLE_FUNCTION_NOT_FOUND,      /* 141 */
    EN_HTTP_CURLE_ABORTED_BY_CALLBACK,     /* 142 */
    EN_HTTP_CURLE_BAD_FUNCTION_ARGUMENT,   /* 143 */
    EN_HTTP_CURLE_OBSOLETE44,              /* 144 - NOT USED */
    EN_HTTP_CURLE_INTERFACE_FAILED,        /* 145 - CURLOPT_INTERFACE failed */
    EN_HTTP_CURLE_OBSOLETE46,              /* 146 - NOT USED */
    EN_HTTP_CURLE_TOO_MANY_REDIRECTS ,     /* 147 - catch endless re-direct loops */
    EN_HTTP_CURLE_UNKNOWN_OPTION,          /* 148 - User specified an unknown option */
    EN_HTTP_CURLE_TELNET_OPTION_SYNTAX ,   /* 149 - Malformed remote login option */
    EN_HTTP_CURLE_OBSOLETE50,              /* 150 - NOT USED */
    EN_HTTP_CURLE_PEER_FAILED_VERIFICATION, /* 151 - peer's certificate or fingerprint
                                               wasn't verified fine */
    EN_HTTP_CURLE_GOT_NOTHING,             /* 152 - when this is a specific error */
    EN_HTTP_CURLE_SSL_ENGINE_NOTFOUND,     /* 153 - SSL crypto engine not found */
    EN_HTTP_CURLE_SSL_ENGINE_SETFAILED,    /* 154 - can not set SSL crypto engine as default */
    EN_HTTP_CURLE_SEND_ERROR,              /* 155 - failed sending network data */
    EN_HTTP_CURLE_RECV_ERROR,              /* 156 - failure in receiving network data */
    EN_HTTP_CURLE_OBSOLETE57,              /* 157 - NOT IN USE */
    EN_HTTP_CURLE_SSL_CERTPROBLEM,         /* 158 - problem with the local certificate */
    EN_HTTP_CURLE_SSL_CIPHER,              /* 159 - couldn't use specified cipher */
    EN_HTTP_CURLE_SSL_CACERT,              /* 160 - problem with the CA cert (path?) */
    EN_HTTP_CURLE_BAD_CONTENT_ENCODING,    /* 161 - Unrecognized/bad encoding */
    EN_HTTP_CURLE_LDAP_INVALID_URL,        /* 162 - Invalid LDAP URL */
    EN_HTTP_CURLE_FILESIZE_EXCEEDED,       /* 163 - Maximum file size exceeded */
    EN_HTTP_CURLE_USE_SSL_FAILED,          /* 164 - Requested FTP SSL level failed */
    EN_HTTP_CURLE_SEND_FAIL_REWIND,        /* 165 - Sending the data requires a rewind that failed */
    EN_HTTP_CURLE_SSL_ENGINE_INITFAILED,   /* 166 - failed to initialise ENGINE */
    EN_HTTP_CURLE_LOGIN_DENIED,            /* 167 - user, password or similar was not
                                              accepted and we failed to login */
    EN_HTTP_CURLE_TFTP_NOTFOUND,           /* 168 - file not found on server */
    EN_HTTP_CURLE_TFTP_PERM,               /* 169 - permission problem on server */
    EN_HTTP_CURLE_REMOTE_DISK_FULL,        /* 170 - out of disk space on server */
    EN_HTTP_CURLE_TFTP_ILLEGAL,            /* 171 - Illegal TFTP operation */
    EN_HTTP_CURLE_TFTP_UNKNOWNID,          /* 172 - Unknown transfer ID */
    EN_HTTP_CURLE_REMOTE_FILE_EXISTS,      /* 173 - File already exists */
    EN_HTTP_CURLE_TFTP_NOSUCHUSER,         /* 174 - No such user */
    EN_HTTP_CURLE_CONV_FAILED,             /* 175 - conversion failed */
    EN_HTTP_CURLE_CONV_REQD,               /* 176 - caller must register conversion
                                                callbacks using curl_easy_setopt options
                                                CURLOPT_CONV_FROM_NETWORK_FUNCTION,
                                                CURLOPT_CONV_TO_NETWORK_FUNCTION, and
                                                CURLOPT_CONV_FROM_UTF8_FUNCTION */
    EN_HTTP_CURLE_SSL_CACERT_BADFILE,      /* 177 - could not load CACERT file, missing
                                                or wrong format */
    EN_HTTP_CURLE_REMOTE_FILE_NOT_FOUND,   /* 178 - remote file not found */
    EN_HTTP_CURLE_SSH,                     /* 179 - error from the SSH layer, somewhat
                                                generic so the error message will be of
                                                interest when this has happened */
    EN_HTTP_CURLE_SSL_SHUTDOWN_FAILED,     /* 180 - Failed to shut down the SSL
                                                connection */
    EN_HTTP_CURLE_AGAIN,                   /* 181 - socket is not ready for send/recv,
                                                wait till it's ready and try again (Added
                                                in 7.18.2) */
    EN_HTTP_CURLE_SSL_CRL_BADFILE,         /* 182 - could not load CRL file, missing or
                                                wrong format (Added in 7.19.0) */
    EN_HTTP_CURLE_SSL_ISSUER_ERROR,        /* 183 - Issuer check failed. */
    EN_HTTP_CURLE_FTP_PRET_FAILED,         /* 184 - a PRET command failed */
    EN_HTTP_CURLE_RTSP_CSEQ_ERROR,         /* 185 - mismatch of RTSP CSeq numbers */
    EN_HTTP_CURLE_RTSP_SESSION_ERROR,      /* 186 - mismatch of RTSP Session Ids */
    EN_HTTP_CURLE_FTP_BAD_FILE_LIST,       /* 187 - unable to parse FTP file list */
    EN_HTTP_CURLE_CHUNK_FAILED,            /* 188 - chunk callback reported error */

    EN_HTTP_CURL_CREATE_HANDLE_FAIL = 201,   /* 201 */
    EN_HTTP_CURL_SET_HEAD_FAIL,              /* 202 */
    
    EN_HTTP_ERRORCODE_BUTT                   /* never used */

}EN_HTTP_ERRORCODE;


/* TLS模式 */
typedef enum{
    EN_TLS_CLOSE              = 0, /*关闭不使用TLS*/
    EN_TLS_VERIFY_NONE        = 1, /*开启TLS，不校验*/
    EN_TLS_VERIFY_SERVER      = 2, /*开启TLS，仅校验服务器*/
    EN_TLS_VERIFY_CLIENT      = 3, /*开启TLS，仅校验客户端*/
    EN_TLS_VERIFY_BOTH        = 4, /*开启TLS，服务器、客户端均校验*/

    EN_TLS_MODE_BUTT

}EN_TLS_MODE;

/* TLS校验服务器方式 */
typedef enum{
    EN_VERIFY_SERVER_PEER               = 0, /*仅校验证书(默认方式）*/
    EN_VERIFY_SERVER_PEER_AND_HOST      = 1, /*校验证书和主机*/
    
    EN_VERIFY_SERVER_BUTT

}EN_VERIFY_SERVER_MODE;


/* TLS参数 */
typedef struct tagHTTP_TLS_PARA
{
    HTTPChar    *pcCaCertPath;                   /*CA证书 */
    HTTPChar    *pcClientCertPath;               /*客户端证书 */
    HTTPChar    *pcClientCertType;               /*客户端证书类型，"PEM"/"DER" */
    HTTPChar    *pcClientKeyPath;                /*客户端密钥*/
    HTTPChar    *pcClientKeyType;                /*客户端密钥类型，"PEM"/"DER"/"ENG" */
    HTTPChar    *pcClientPrivKeyPw;              /*客户端私钥密码 */
    EN_VERIFY_SERVER_MODE  enVerifyServerMode;   /*校验服务端模式(默认为证书和主机都校验）*/
}ST_HTTP_TLS_PARA;

/* http头域链表 */
typedef struct tagST_HTTP_HEAD_LIST
{
    HTTPChar                   *pcHeadType;
    HTTPChar                   *pcHeadValue;
    struct tagST_HTTP_HEAD_LIST     *pNext;

}ST_HTTP_HEAD_LIST;


/* http body结构 */
typedef struct tagST_HTTP_BODY
{
    HTTPChar       *pcbody;
    HTTPULong       ulBodyLen;
}ST_HTTP_BODY;

/* http响应参数 */
typedef struct tagST_HTTP_RSP
{
    HTTPULong              ulStatusCode;
    ST_HTTP_HEAD_LIST      *pstHeadList;
    ST_HTTP_BODY           stHttpBody;
}ST_HTTP_RSP;


/* http请求响应的回调函数指针 */
typedef int (*HttpRspNotify)(HTTPHANDLE handle, LONG lCode, ST_HTTP_RSP *pstHttpRsp);

/* http配置参数 */
typedef struct tagST_HTTP_HANDLE_CONFIG
{

    HTTPChar               *pcHostIP;
    HTTPUInt                hostPort;
    EN_TLS_MODE             enTlsMode;
    ST_HTTP_TLS_PARA        stTLSPara; 
 
}ST_HTTP_HANDLE_CONFIG;

/* HTTP 请求行 */
typedef struct tagST_HTTP_REQUEST_LINE
{
    HTTPChar          *pcMethod;
    HTTPChar          *pcUrl;

}ST_HTTP_REQUEST_LINE;


/* HTTP 日志级别 */
typedef enum
{
    EN_HTTP_LOG_NONE         = 0,    /*关闭日志*/
    EN_HTTP_LOG_ERROR        = 1,    /*错误(运行日志)*/
    EN_HTTP_LOG_WARNING      = 2,    /*告警(运行日志)*/
    EN_HTTP_LOG_INFO         = 3,    /*提示(运行日志)*/
    EN_HTTP_LOG_DEBUG        = 4     /*调试(运行日志)*/

}EN_HTTP_LOG_LEVEL;


/* HTTP初始化参数 */
typedef struct tagST_HTTP_CONFIG
{
    EN_HTTP_LOG_LEVEL           enLevel;            /*日志级别*/
    HTTPUInt                    uiLogFileSize;      /*日志文件大小(当日志级别为EN_HTTP_LOG_NONE时无效)*/
    HTTPChar                    *pcFilePath;        /*日志文件路径(当日志级别为EN_HTTP_LOG_NONE时无效)*/
    HTTPUInt                    uiHandleNum;        /*支持的handle个数*/
}ST_HTTP_CONFIG;



/*
 *****************************************************************************
 * 函 数 名  : svn_http_init
 * 功能描述  : HTTP sdk初始化
 * 输入参数  : 无
 *
 * 输出参数  : 无
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 只需要一次环境初始化
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 *
 *****************************************************************************
 */
HTTP_API LONG svn_http_init();


/*
 *****************************************************************************
 * 函 数 名  : svn_http_cleanup
 * 功能描述  : HTTP sdk环境清理
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 *
 *****************************************************************************
 */
HTTP_API LONG svn_http_cleanup();


/*
 *****************************************************************************
 * 函 数 名  : svn_http_createhandle
 * 功能描述  : 创建HTTP请求的句柄，并设置该handle配置参数
 * 输入参数  : pstHttpHandleConfig
 *             pstHttpHandleConfig： 创建句柄时需使用的参数，包含以下项：
               HTTPChar               *pcHostIP : HTTP服务IP
               HTTPUInt                hostPort:  HTTP服务端口
               EN_TLS_MODE             enTlsMode：TLS安全模式，参见头文件定义
               ST_HTTP_TLS_PARA        stTLSPara：设置TLS时需要的参数，参见头文件
             
 * 输出参数  : pHandle 返回HTTP请求的句柄，用于后续的请求响应操作
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 *
 *****************************************************************************
 */
HTTP_API LONG svn_http_createhandle(const ST_HTTP_HANDLE_CONFIG *pstHttpHandleConfig, HTTPHANDLE *pHandle);


/*
 *****************************************************************************
 * 函 数 名  : svn_http_closehandle
 * 功能描述  : 关闭HTTP请求的句柄
 * 输入参数  : handle  
 * 输出参数  : 无
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 *
 *****************************************************************************
 */
HTTP_API LONG svn_http_closehandle(HTTPHANDLE handle);


/*
 *****************************************************************************
 * 函 数 名  : svn_http_setrequestline
 * 功能描述  : 设置HTTP请求行
 * 输入参数  : handle            
 *             pstRequestLine  
 *
 * 输出参数  : 无
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 *
 *****************************************************************************
 */
HTTP_API LONG svn_http_setrequestline(HTTPHANDLE handle, const ST_HTTP_REQUEST_LINE *pstRequestLine);


/*
 *****************************************************************************
 * 函 数 名  : svn_http_addheadcontent
 * 功能描述  : 添加http头域
 * 输入参数  : handle           
 *             pcHeadType 
 *             pcHeadValue 
 *              
 * 输出参数  : 无
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 *
 *****************************************************************************
 */
HTTP_API LONG svn_http_addheadcontent(HTTPHANDLE handle, const HTTPChar *pcHeadType, const HTTPChar *pcHeadValue);


/*
 *****************************************************************************
 * 函 数 名  : svn_http_setbody
 * 功能描述  : 设置http请求的body
 * 输入参数  : handle  
 *             pstBody  
 * 输出参数  : 无
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 *
 *****************************************************************************
 */
HTTP_API LONG svn_http_setbody(HTTPHANDLE handle, const ST_HTTP_BODY* pstBody);


/*
 *****************************************************************************
 * 函 数 名  : svn_http_reqasynsend
 * 功能描述  : 异步发送通用的http请求
 * 输入参数  : handle 
 *             httpRspNotifyFunc 回调函数
 *
 * 输出参数  : 
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 异步发送
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 *
 *****************************************************************************
 */
HTTP_API LONG svn_http_reqasynsend(HTTPHANDLE handle, HttpRspNotify httpRspNotifyFunc);


/*
 *****************************************************************************
 * 函 数 名  : svn_http_reqsynsend
 * 功能描述  : 同步发送通用的http请求
 * 输入参数  : handle 
 *             httpRspNotifyFunc 回调函数
 *
 * 输出参数  : 
 * 返 回 值  : 成功返回EN_HTTP_OK，失败返回对应错误码
 * 其它说明  : 
 * 
 * 修改历史      :
 *  1.日    期   : 2012年12月24日
 *    作    者   : 
 *    修改内容   : 新生成函数
 * 2.add by liushangshu for change function name.
 *****************************************************************************
 */
HTTP_API EN_HTTP_ERRORCODE svn_http_reqsynsend(HTTPHANDLE handle, ST_HTTP_RSP **ppstHttpRsp);

/*
 *****************************************************************************
 * 函 数 名  : svn_http_releasesynrsp
 * 功能描述  : 释放同步发送请求的响应
 * 输入参数  : pstHttpRsp  
 *
 * 输出参数  : 
 * 返 回 值  : EN_HTTP_ERRORCODE
 * 其它说明  : 
 * 
 * 修改历史      :
 *  1.日    期   : 
 *    作    者   : ***
 *    修改内容   : 新生成函数
 *  2.日    期   : 2013年5月14日
 *    作    者   : add by liushangshu 
 *    修改内容   : 实现代码逻辑
 *
 *****************************************************************************
 */
HTTP_API EN_HTTP_ERRORCODE svn_http_releasesynrsp(ST_HTTP_RSP *pstHttpRsp);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__svnhttp_h_
