#ifndef __ANYOFFICE_HTTP_CLIENT_H__#define __ANYOFFICE_HTTP_CLIENT_H__/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司
******************************************************************************
  文 件 名   : anyoffice_httpclient.h
  版 本 号   : 初稿
  作    者   : zhangyantao 00103873
  生成日期   : 2014年7月18日
  最近修改   :
  功能描述   : HTTP 客户端，处理和网关HTTPS通信
  函数列表   :
  修改历史   :
  1.日    期   : 2014年7月18日
    作    者   : zhangyantao 00103873
    修改内容   : 创建文件******************************************************************************/#include "tools_common.h"
#include "tools_string.h"#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_CLIENT_URL_MAX_LENGTH  1024
#define HTTP_CLIENT_METHOD_POST     0 #define HTTP_CLIENT_METHOD_OPTION   1#define HTTP_CLIENT_METHOD_PUT      2#define HTTP_CLIENT_METHOD_GET      3
#define HTTP_CLIENT_HEAD_TYPE_CONTENTTYPE           "Content-Type"#define HTTP_CLIENT_HEAD_TYPE_CONTENTLENGTH         "Content-Length"#define HTTP_CLIENT_HEAD_TYPE_AUTHORIZATION         "Authorization"#define HTTP_CLIENT_HEAD_TYPE_USERAGENT             "User-Agent"#define HTTP_CLIENT_HEAD_TYPE_HOST                  "Host"#define HTTP_CLIENT_HEAD_TYPE_CONNECTION            "Connection"#define HTTP_CLIENT_HEAD_VALUE_TEXT                 "txt/html"
//双向校验操作系统标识
#define HTTP_CLIENT_OS_VALUE_ANDROID                 1#define HTTP_CLIENT_OS_VALUE_IOS                      0//用户证书，设备证书#define HTTP_CLIENT_OS_DEVICE_CERT                 0#define HTTP_CLIENT_OS_USER_CERT                      1#define HTTP_CLIENT_STATUS_OK                      200#define HTTP_CLIENT_STATUS_LOCATION_RESPONSE       301        /*返回重定向*/#define HTTP_CLIENT_STATUS_TEMPLOCATION_RESPONSE   302        /*返回重定向*/#define HTTP_CLIENT_STATUS_PAGE_NOTFOUND           404        /*页面不存在*/

/* HTTP头域链表，请求头，响应头均使用该结构 */
typedef struct tagHttpClientVRBList
{
    CHAR                        *pcType;  /* 头类型，如Content-Type */
    CHAR                        *pcValue; /* 头的值，如application/vnd.ms-sync.wbxml */
    struct tagHttpClientVRBList *pNext;       /* 串联下一个头节点 */
}HTTP_CLIENT_VRB_LIST_S;

/* HTTP 请求行 */
typedef struct tagHttpClientRequestLine
{
    ULONG ulMethod;     /* 请求方法，如POST,OPTIONS */
    ULONG ulSSLEnable;
    ULONG ulPort;     ULONG ulAppendSessionInfo;  /*URL中是否添加*/
    CHAR  *pcURL;        /* 完整的URL用来支持重定向的响应直接进行处理 */
    CHAR  *pcHost;       /* Host, */
    CHAR  *pcPath;     CHAR  *pcServiceType;  /* 业务类型 */ 
    HTTP_CLIENT_VRB_LIST_S *pstParamList;  /* 参数列表 */
}HTTP_CLIENT_REQUEST_LINE_S;


/* HTTP消息体结构 */
typedef struct tagHttpClientBody
{
    ULONG      ulContentLen;   /* http响应头中的content-length长度 */
    ULONG      ulBodyLen;      /* 消息体长度 */
    CHAR       *pcbody;        /* 消息体内容 */
}HTTP_CLIENT_BODY_S;
    /* HTTP请求消息结构 */
typedef struct tagHttpClientRequest
{
    HTTP_CLIENT_REQUEST_LINE_S    *pstReqLine;   /* HTTP 请求行 */
    HTTP_CLIENT_VRB_LIST_S        *pstHeadList;  /* HTTP头域链表 */
    HTTP_CLIENT_BODY_S            stHttpBody;    /* HTTP消息体 */}HTTP_CLIENT_REQUEST_S;

typedef struct tagHttpClientResponse
{
    ULONG                        ulStatusCode;   /* HTTP状态码 */
    CHAR                         *pcRedirectURL;
    HTTP_CLIENT_VRB_LIST_S       *pstHeadList;   /* HTTP头域链表 */ 
    struct http_slist            *pstCookies;  /*响应cookie*/
    HTTP_CLIENT_BODY_S           stHttpBody;     /* HTTP消息体 */
}HTTP_CLIENT_RESPONSE_S;

 typedef ULONG (*HTTP_Client_IOCallBack)(    UCHAR *pucData, ULONG ulSize, ULONG ulNmemb,void *pvUserData); typedef INT32 (*HTTP_Client_ProgressCallBack)(    VOID *pvClientp, double dltotal, double dlnow, double ultotal, double ulnow);typedef struct tagHttpClientOption
{
    ULONG ulL4VPNFlag;    ULONG ulAcceptGzip;    ULONG ulTimeOut;    ULONG ulSSLVerifyPeer;    ULONG ulSSLVerifyHost;    ULONG ulCurlLogVerbose;    ULONG ulHttpAuthenticateEnable;     ULONG ulPostFieldSize;     ULONG ulNoProgress;     ULONG ulResumeFrom;     ULONG ulCaCheckFlg;     CHAR* pcCaFileFullPath;         HTTP_Client_IOCallBack callbackBodyRead;     VOID *pvcallbackBodyReadUserData;     HTTP_Client_IOCallBack callbackBodyWrite;     VOID *pvcallbackBodyWriteUserData;     HTTP_Client_IOCallBack callbackHeadWrite;     VOID *pvcallbackHeadWriteUserData;     HTTP_Client_ProgressCallBack callbackProgressFunction;     VOID *pvcallbackProgressData;     CHAR * pcHostName;     ULONG ulAuthEnable;    /*是否校验服务器*/     ULONG diableMultiplex;  /*禁用socket复用*/}HTTP_CLIENT_OPTION_S;HTTP_CLIENT_VRB_LIST_S *AnyOffice_HttpClient_API_AppendHeadVRB(    HTTP_CLIENT_VRB_LIST_S *pstHeadList, CHAR *pcType, CHAR *pcValue);VOID AnyOffice_HttpClient_API_FreeVRBList(            HTTP_CLIENT_VRB_LIST_S    *pstVRBList);VOID AnyOffice_HttpClient_API_FreeResponse(HTTP_CLIENT_RESPONSE_S *pstResponse);void AnyOffice_HttpClient_API_FreeRequest(HTTP_CLIENT_REQUEST_S *pstRequest);INT32 AnyOffice_HttpClient_API_Connect(     HTTP_CLIENT_OPTION_S *pstOptions, 
    HTTP_CLIENT_REQUEST_S *pstRequest, 
    HTTP_CLIENT_RESPONSE_S *pstResponse);INT32 AnyOffice_HttpClient_API_Init();VOID AnyOffice_HttpClient_API_Fini();

#ifdef __cplusplus
}
#endif




#endif

