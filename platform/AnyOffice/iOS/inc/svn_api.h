#ifndef __SVN_API_H__
#define __SVN_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************************************************************************************************/
/*                        常规用法                                                                                                                                                                               */
/*                        SVN_API_InitEnv                                                          */
/*                               ||                                                                */
/*                               ||                                                                */
/*                               \/                                                                */
/*                        SVN_API_CreateTunnel                                                     */
/*                               ||                                                                */
/*                               ||                                                                */
/*                               \/                                                                */
/*                        SVN socket操作                                                                                                                                                              */
/*                               ||                                                                */
/*                               ||                                                                */
/*                               \/                                                                */
/*                        SVN_API_DestroyTunnel                                                    */
/*                               ||                                                                */
/*                               ||                                                                */
/*                               \/                                                                */
/*                        SVN_API_CleanEnv                                                         */
/***************************************************************************************************/


/***************************************************************************************************/
/*                        接口定义                                                                 */
/***************************************************************************************************/

/*
函数名称：SVN_API_InitEnv
函数原型：unsigned long SVN_API_InitEnv()
函数说明：初始化组件运行环境
输入参数：无
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
=========================================
使用说明：应用程序启动成功后调用此接口初始化组件运行环境，只有在此函数返回成功后，后续的接口才能正确执行。
一个应用程序实例只需调用此接口一次，在应用程序退出时调用SVN_API_CleanEnv()清理运行环境.
注意：此函数是同步函数，执行时间需要1-10秒左右，执行过程中会阻塞当前线程
*/
unsigned long SVN_API_InitEnv();

/*
函数名称：SVN_API_CleanEnv
函数原形：unsigned long SVN_API_CleanEnv()
函数说明：清理组件运行环境。
输入参数：无
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
=========================================
使用说明：应用程序关闭时调用此接口清理组件运行环境.
*/
unsigned long SVN_API_CleanEnv();


/*
函数名称：SVN_API_CreateTunnel
函数原形：unsigned long SVN_API_CreateTunnel(SVN_REGISTER_INFO_S *pstRegisterInfo)
函数说明：创建隧道。
输入参数：SVN_REGISTER_INFO_S *pstRegisterInfo –注册信息，包括SVN网关IP/端口、SVN用户名/密码
          代理类型/代理服务器地址/端口/账号/密码/域、APN信息（ID/Name）、使用的隧道模式(DTLS/TLS/ UDP/TLS+UDPS)、状态通知回调函数指针、日志输出回调函数指针。
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
=========================================
通过回调函数方式来通知应用程序是否已经创建成功，具体请参考组件状态通知接口；
调用失败，一般是入参不合法,包括网关URL、端口、用户名、密码不能为空，使用代理情况下代理URL、端口不能为空（手机平台不支持代理）。
使用说明：此函数是异步调用的，即调用这个函数不会阻塞应用程序的运行。当这个函数执行结束后，会调用应用程序注册的回调函数告知这个函数的执行结果。
*/
unsigned long SVN_API_CreateTunnel(SVN_REGISTER_INFO_S *pstRegisterInfo);


/*
函数名称：SVN_API_DestroyTunnel
函数原形：unsigned long SVN_API_DestroyTunnel()
函数说明：关闭加密隧道。
输入参数：无
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
=========================================
使用说明：需要删除隧道时使用，可以在连接过程中调用，立刻停止连接过程。
*/
unsigned long SVN_API_DestroyTunnel();


/*
函数名称：SVN_API_GetTunnelIP
函数原形：unsigned long SVN_API_GetTunnelIP (unsigned long *pulIPAddess, unsigned long *pulMask)
输入参数：无
输出参数：*pulIPAddess 用于安全通信的源IP（主机序）
          *pulMask 虚拟IP的子网掩码（主机序）
返回值  ：SVN_OK 表示成功
          SVN_ERR表示失败
=========================================
使用说明：创建多媒体隧道成功后，调用 SVN_API_GetTunnelIP 获取虚拟IP和子网掩码。可以取代GetLocalIpAddr 获取本机IP；还可以通过返
回的子网掩码判断是否某目的IP是否广播地址。对于需要封装加密和穿越的连接，必须使用此函数返回的隧道IP，才能确保媒体流的互通。

注意：如果返回的虚拟IP为0xFFFFFFFF，则表示连接SVN网关失败，应用程序必须再次调用SVN_API_CreateTunnel函数创建隧道。
*/
unsigned long SVN_API_GetTunnelIP(unsigned long *pulIPAddress, unsigned long *pulMask);


/*
函数名称：SVN_API_GetTunnelStatus
函数原形：unsigned long  SVN_API_GetTunnelStatus (unsigned int *puiStatus, int *piErrorCode)
函数说明：获取隧道状态以及具体信息
输入参数：无
输出参数：puiStatus 隧道状态(SVN_STATUS_OFFLINE, SVN_STATUS_CONNECTING, SVN_STATUS_ONLINE)
输出参数：piErrorCode     错误码
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
=========================================
使用说明：此函数可以获取隧道的实时状态和具体信息，通过不断轮询的方式可以获取组件隧道状态信息， 其状态结果与回调方式获取一致。
*/
unsigned long  SVN_API_GetTunnelStatus (unsigned int *puiStatus, int *piErrorCode);


/*
函数名称：SVN_API_GetVersion
函数原形：const char* SVN_API_GetVersion()
函数说明：获取组件库的版本。
输入参数：无
输出参数：
返回值  ： 返回版本号的字符串，如2.2.1051.1
*/
const char* SVN_API_GetVersion ();


/*
函数名称：SVN_API_SetLogParam
函数原形：unsigned long SVN_API_SetLogParam(char * pcLogPath, unsigned long ulLogLevel)
函数说明：为应用程序提供接口，设置日志级别和日志输出路径，如果不想将日志输出到文件，则pcLogPath需要置为NULL，默认输出error和warning级别日志。
输入参数：char * pcLogPath - 日志存储路径
          unsigned long ulLogLevel - 描述哪个等级的日志被打印输出
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
*/
unsigned long SVN_API_SetLogParam(char * pcLogPath, unsigned long ulLogLevel);

/*
函数名称：SVN_API_GetSessionID
函数原形：unsigned long SVN_API_GetSessionID(unsigned long *pulSessionIdLS, unsigned long *pulSessionIdMS)
函数说明：获取user id ， session id
输入参数：
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败

*/
unsigned long SVN_API_GetSessionID(unsigned char ** ppucUserID, unsigned char **ppucSesstionID);
char * SVN_API_GetTunnelToken();

/*
函数名称：SVN_API_SetStatisticSwitch
函数原形：unsigned long SVN_API_SetStatisticSwitch(unsigned long ulSwitch)
函数说明：设置统计与监控开关。
输入参数：unsigned long ulSwitch - 统计与监控开关
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
*/



unsigned long SVN_API_SetStatisticSwitch(unsigned long ulSwitch);

/*
函数名称：SVN_API_GetStatisticResult
函数原形：void SVN_API_GetStatisticResult(SVN_STATISTIC_INFO_S *pstStatInfo)
函数说明：获取统计与监控信息
输入参数：无
输出参数：SVN_STATISTIC_INFO_S *pstStatInfo - 统计的结果
返回值  ：无
*/
void SVN_API_GetStatisticResult(SVN_STATISTIC_INFO_S *pstStatInfo);

/*
函数名称：SVN_API_Ping
函数原形：unsigned long SVN_API_Ping (unsigned long ulIPAddr, unsigned long ulPacketSize,
                                      SVN_PingCallback  pfPingCallBack)
函数说明：设置Ping目的主机IP和Ping报文大小，并启动Ping功能。
输入参数：unsigned long ulIPAddr          - 被Ping目的主机IP
          unsigned long ulPacketSize      - ICMP回显请求报文中数据长度
          SVN_PingCallback pfPingCallBack  - ICMP结果的回调
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
*/
unsigned long SVN_API_Ping (unsigned long ulIPAddr, unsigned long ulPacketSize,
                            SVN_PingCallback  pfPingCallBack);

/*
函数名称：SVN_API_Traceroute
函数原形：unsigned long SVN_API_Traceroute (unsigned long ulIPAddr, SVN_TracertCallback  pfTracertCallBack)
函数说明：设置Traceroute目的主机IP，并启动Traceroute功能。
输入参数：unsigned long ulIPAddr                 - Traceroute目的主机IP
        ：SVN_TracertCallback  pfTracertCallBack - tracert结果的回调
输出参数：无
返回值  ： SVN_OK 表示成功
           SVN_ERR表示失败
*/
unsigned long SVN_API_Traceroute (unsigned long ulIPAddr, SVN_TracertCallback  pfTracertCallBack);




/*
函数名称：SVN_API_Encrypt
函数原形：unsigned long SVN_API_Encrypt(unsigned char *pucInBuffer, unsigned long  ulLen, unsigned char *pucOutBuffer, unsigned long *pulOutLen)
函数说明：提供加密接口。
输入参数：unsigned char *pucInBuffer - 待加密数据
          unsigned long  ulInLen     - 加密长度，最大长度1024
输出参数：unsigned char *pucOutBuffer - 加密后数据，分配空间必须大于明文长度，建议为明文长度加上32字节，内存空间不能为NULL
          unsigned long *pulOutLen    - 加密后长度，作为入参传进pucOutBuffer的长度，作为出参输出数据加密后的长度
返回值  ： SVN_OK 表示成功
           SVN_CRYPT_PARAMETER_ERR 表示加密入参错误
           SVN_CRYPT_MEMORY_ERR 表示加密后出参BUFF长度不足
*/
unsigned long SVN_API_Encrypt(unsigned char *pucInBuffer, unsigned long ulInLen,
                              unsigned char *pucOutBuffer, unsigned long *pulOutLen);
unsigned long SVN_API_EncryptLarge(unsigned char *pucInBuffer, unsigned long ulInLen,
    unsigned char **ppucOutBuffer, unsigned long *pulOutLen);

/*
函数名称：SVN_API_Decrypt
函数原形：long  SVN_API_Decrypt(unsigned char *pucInBuffer, unsigned long ulInLen,
                                unsigned char *pucOutBuffer, unsigned long *pulOutLen)
函数说明：提供解密接口。该接口与加密接口配套使用，只能够解密经加密接口加密后的密文。
输入参数：unsigned char * pucInBuffer - 待解密数据
          unsigned long  ulInLen - 解密长度，最大长度2048
输出参数：unsigned char * pucOutBuffer - 解密后数据，分配空间不能小于密文长度,内存空间不能为NULL
          unsigned long * pulOutLen - 解密后长度，内存空间不能为NULL，作为入参传进pucOutBuffer的长度，作为出参输出数据加密后的长度
返回值  ： SVN_OK 表示成功
           SVN_CRYPT_PARAMETER_ERR 表示解密入参错误
           SVN_CRYPT_MEMORY_ERR 表示解密后出参BUFF长度不足
*/
unsigned long SVN_API_Decrypt(unsigned char *pucInBuffer, unsigned long ulInLen,
                              unsigned char *pucOutBuffer, unsigned long *pulOutLen);

/*
加密并进行编码
*/
unsigned long SVN_API_EncryptAndEncode(unsigned char *pucInput, unsigned long ulInputLen, 
                              unsigned char **ppucOutPut);

/*
解码再解密
*/
unsigned long SVN_API_DecodeAndDecrypt(unsigned char *pucInput, unsigned char **ppucOutPut, 
                              unsigned long *pulOutLen);

/*
函数名称：SVN_API_MD5
函数原形：long SVN_API_MD5(unsigned char *pucInbuffer, unsigned long ulInLen,
                              unsigned char *pucOutbuffer, unsigned long *pulOutLen)
函数说明：摘要接口。
输入参数：unsigned char * pucInBuffer - 待摘要数据
          unsigned long ulInLen -待摘要数据长度，最大长度1024
输出参数：unsigned char * pucOutBuffer – 摘要值，内存空间不能为NULL
          unsigned long * pulOutLen - 摘要值长度，内存空间不能为NULL，作为入参传进pucOutBuffer的长度，作为出参输出摘要值的长度
返回值  ： SVN_OK 表示成功
           SVN_CRYPT_PARAMETER_ERR 表示摘要入参错误
           SVN_CRYPT_MEMORY_ERR 表示摘要后出参BUFF长度不足
*/
unsigned long SVN_API_MD5(unsigned char *pucInbuffer, unsigned long ulInLen,
                             unsigned char *pucOutbuffer, unsigned long *pulOutLen);
/*
 函数名称：unsigned long SVN_API_KeepAliveTimeout()
函数原型 ：unsigned long SVN_API_KeepAliveTimeout()
函数说明 ：保活接口。此接口用于iPhone后台场景下，应用程序调用此接口保活。
输入参数 ：无
输出参数 ：无
返回值     ：函数返回值类型为unsigned long,调用成功返回SVN_OK；调用失败返回SVN_ERR。
其它         ：此函数仅供iphone平台下调用,该函数启动前，环境初始化必须成功
*/
unsigned long SVN_API_KeepAliveTimeout( );

/*
函数名称：SVN_API_AddServer
函数原形：unsigned long SVN_API_AddServer(char *pcServerURL, unsigned short usServerPort)
函数说明：添加SVN地址接口。此接口用于负载均衡场景下，应用程序需要在初始化以后，调用隧道注册接口前调用此接口。
输入参数：char * pcServerURL - 网关URL
          unsigned short  usServerPort - 网关端口
输出参数：无
返回值：SVN_OK表示成功，SVN_ERR表示失败。
*/
unsigned long SVN_API_AddServer(char *pcServerURL, unsigned short usServerPort);

/*
函数名称：SVN_API_SetNetState
函数原形：void SVN_API_SetNetState(unsigned long ulNetState)
函数说明：设置网络当前连接状态.(注意，只在andriod平台下才需要)
输入参数：ulNetState - 当前的状态量
输出参数：无
返回值：无
*/
void SVN_API_SetNetState(unsigned long ulNetState);

/*
函数名称：SVN_API_SetWorkingDir
函数原形：void SVN_API_SetWorkingDir(char * pcWorkingDir)
函数说明：设置svn的工作目录，具体就是指定app可读可写的目录(注意，只在andriod平台下才需要).
            此函数必须在调用SVN_API_InitEnv之前执行。
输入参数：pcWorkingDir - app可读可写的目录名字
输出参数：无
返回值：无
*/
void SVN_API_SetWorkingDir(char * pcWorkingDir);
void SVN_API_GetWorkingDir(char * pcWorkingDir);
/*
函数名称：SVN_API_SuspendNetwork
函数原形：unsigned long SVN_API_SuspendNetwork()
函数说明：挂起svn隧道。
输入参数：无
输出参数：无
返回值：SVN_OK
*/
unsigned long SVN_API_SuspendNetwork();

/*
函数名称：SVN_API_ResumeNetwork
函数原形：unsigned long SVN_API_ResumeNetwork()
函数说明：恢复svn隧道。
输入参数：无
输出参数：无
返回值：SVN_OK
*/
unsigned long SVN_API_ResumeNetwork();


/*
函数名称：SVN_API_Base64Encode
函数原形：long SVN_API_Base64Encode(unsigned char *pszInBuf, int iInBufLen, unsigned char *pszOutBuf, int iOutBufLen);
函数说明：进行base64编码。
输入参数：unsigned char *pszInBuf 待编码数据
         int iInBufLen           待编码长度
         int iOutBufLen             编码后数据存贮内存长度
输出参数：unsigned char *pszOutBuf 编码后数据
返回值：0-错误，其余表示编码后长度
*/
long SVN_API_Base64Encode(unsigned char *pszInBuf, int iInBufLen, unsigned char *pszOutBuf, int iOutBufLen);

/*
函数名称：SVN_API_Base64Decode
函数原形：LONG SVN_API_Base64Decode(unsigned char *pszBuf, int iBufLen);
函数说明：进行base64解码。
输入参数：unsigned char *pszBuf 待解码数据
         int ibufLen           待解码长度
输出参数：unsigned char *pszBuf 解码后数据
返回值：0-错误，其余表示解码后长度
*/
long SVN_API_Base64Decode(unsigned char *pszBuf, int iBufLen);




/*
函数名称：SVN_API_AES256_Encrypt
函数原形：unsigned long SVN_API_AES256_Encrypt(const unsigned char *pucIn,
                                            const unsigned long ulInLength,
                                            unsigned char *pucOut,
                                            unsigned long *pulOutLength,
                                            const unsigned char *pucKeySeed,
                                            const unsigned char *pucIV)
函数说明：加密外部数据。
输入参数：  const unsigned char *pucIn  待加密数据
            const unsigned long ulLength    待加密长度
            const unsigned char *pucKeySeed 密钥种子二进制
            const unsigned char *pucIV      初始向量二进制
输出参数：unsigned char *pucOut       加密后数据
            unsigned long *pulOutLength, 加密后数据长度
返回值：SVN_OK表示成功，SVN_ERR表示失败
*/
unsigned long SVN_API_AES256_Encrypt(const unsigned char *pucIn,
                                            const unsigned long ulInLength,
                                            unsigned char *pucOut,
                                            unsigned long *pulOutLength,
                                            const unsigned char *pucKeySeed,
                                            const unsigned char *pucIV);

/*
函数名称：SVN_API_AES256_Decrypt
函数原形：unsigned long SVN_API_AES256_Decrypt(const unsigned char *pucIn,
                                            unsigned char *pucOut,
                                            const unsigned long ulLength,
                                            const unsigned char *pucKeySeed,
                                            const unsigned char *pucIV)
函数说明：解密外部加密配置文件。
输入参数：  const unsigned char *pucIn  待解密数据
            const unsigned long ulLength    待解密长度
            const unsigned char *pucKeySeed 密钥种子二进制
            const unsigned char *pucIV      初始向量二进制
输出参数：unsigned char *pucOut       解密后数据
输出参数：unsigned char *pucOut       解密后数据
返回值：SVN_OK表示成功，SVN_ERR表示失败
*/
unsigned long SVN_API_AES256_Decrypt(const unsigned char *pucIn,
                                            unsigned char *pucOut,
                                            const unsigned long ulLength,
                                            const unsigned char *pucKeySeed,
                                            const unsigned char *pucIV);

/*
函数名称：SVN_API_ImportCert
函数原形：unsigned long SVN_API_ImportCert(SVN_CLIENTCERT_INFO_S *pstClientCertInfo,
                                    SVN_SERVERCACERT_INFO_S *pstServerCACertInfo,
                                    const unsigned char *pszSubjectCN)
函数说明：导入客户端证书和设备CA证书。
输入参数：  SVN_CLIENTCERT_INFO_S *pstClientCertInfo        客户端证书、私钥信息结构
            SVN_SERVERCACERT_INFO_S *pstServerCACertInfo    指向验证网关设备证书的（多条）CA证书链
            const unsigned char *pszSubjectCN               指定的客户端证书subject cn(commonname)
输出参数：无
返回值  ：SVN_OK表示成功，其它表示失败
其它    : 1:导入的客户端证书可以为NULL,为NULL时不设置客户端证书。
          2:该接口目前不支持导入设备CA证书,对pstServerCACertInfo参数不处理。
          3:导入的客户端证书subject cn可以为NULL,为NULL时不校验客户端证书的commonname。
          4:客户端证书为PKCS12时，pszPrivateKeyPwd 为证书文件加密密钥；PEM或DER时为私钥文件加密密码
          5:若需要导入证书，该接口必须在登录网关前成功调用。
          6:导入的证书内容必须与证书类型一致
          7:PEM或DER格式的客户端证书导入时必须同时导入RSA私钥信息
          8:其它证书内容限制参照接口文档说明
*/
unsigned long SVN_API_ImportCert(SVN_CLIENTCERT_INFO_S *pstClientCertInfo,
                                    SVN_SERVERCACERT_INFO_S *pstServerCACertInfo,
                                    const unsigned char *pszSubjectCN);

unsigned long SVN_API_ParseCert(SVN_CLIENTCERT_INFO_S *pstClientCertInfo,    void **ppstX509Cert,
    void **ppstEvpPKey);
unsigned long SVN_API_FreeCert(void **ppstX509Cert,
    void **ppstEvpPKey);
/*
函数名称            ：SVN_API_LoadCACertFromFile 
函数原型            ：unsigned long SVN_API_LoadCACertFromFile(unsigned char *pucCertPath)
函数说明            ：导入设备CA证书
输入参数            ：unsigned char *pucCertPath                 证书文件路径 
输出参数            ：无
返回值              ：SVN_OK 表示调用成功、其他值表示失败
其它                ：1.该函数是导入设备CA证书的函数
                      

*/
unsigned long SVN_API_LoadCACertFromFile(unsigned char *pucCertPath);

unsigned long SVN_API_LoadCACertFromPem(unsigned char *pucCAContent);

/*
函数名称            ：SVN_API_ParseURL
函数原型            ：unsigned long  SVN_API_ParseURL(unsigned char* pucURL, unsigned long ulLen, SVN_ParseURLCallback pfParseURLCB, void *pvData)
函数说明            ：内网域名解析
输入参数            ：unsigned char* pucURL                 待解析域名
                      unsigned long ulLen                   待解析域名 长度
                      SVN_ParseURLCallback pfParseURLCB     解析后通知函数
                      void *pvData                          回调函数入参
输出参数            ：无
返回值              ：SVN_OK 表示调用成功、其他值表示失败
其它                ：1.该函数是异步函数,解析结果通过回调函数通知
                      2.对于解析后的IP，目前只支持最大10个解析后的ip返回。

*/
unsigned long  SVN_API_ParseURL(unsigned char* pucURL, unsigned long ulLen, SVN_ParseURLCallback pfParseURLCB, void *pvData);


/*
函数名称            ：SVN_API_CreateTunnelEx
函数原型            ：unsigned long SVN_API_CreateTunnelEx(PREGISTERINFO pstRegisterInfo)
函数说明            ：隧道创建扩展函数，使用消息通知回调函数，不使用原状态回调函数
输入参数            ：pstRegisterInfo 隧道注册信息
输出参数            ：无
返回值              ：SVN_OK 表示成功、其他值表示失败
其它                ：第三方调用此接口发起隧道创建，此过程为异步处理，隧道创建
                      完成后通过回调函数返回状态
*/
unsigned long SVN_API_CreateTunnelEx(SVN_REGISTER_INFO_S *pstRegisterInfo);


/*
函数名称            ：SVN_API_InitEnvEx
函数原型            ：unsigned long SVN_API_InitEnvEx(unsigned long ulControlTypeMask, void* pPara);
函数说明            ：组件初始化扩展函数，带控制参数初始化
输入参数            ：unsigned long ulControlTypeMask 控制参数掩码
                      void* pPara                     控制参数结构体
输出参数            ：无
返回值              ：SVN_OK 表示成功、其他值表示失败
其它                ：1.组件若使用带控制参数初始化内存，则必须使用该接口，不能使用原初始化接口SVN_API_InitEnv
                      2.目前支持以下控制参数
                            a.初始化内存大小控制，掩码位0x1
*/
unsigned long SVN_API_InitEnvEx(unsigned long ulControlTypeMask, void* pPara);
/*BEGIN: add by tianxin kf35598 2012-8-8*/
/*
函数名称：SVN_API_RegisterProcessFuction
函数原形：unsigned long SVN_API_RegisterProcessFuction(SVN_RegFuc pfFuction)
函数说明：注册MDM  消息处理函数到MTM 模块。
输入参数：SVN_RegFuc pfFuction        MDM 注册回调函数。        
输出参数：无
返回值  ：SVN_OK表示成功，其它表示失败
其它    : 
*/
unsigned long SVN_API_RegisterProcessFuction(SVN_RegFuc pfFuction);

/*END: add by tianxin kf35598 2012-8-8*/

/* BEGIN: Added by fengzhengyu, 2013/7/3 for:策略实时生效*/
/*
函数名称：SVN_API_RegisterProcessFuctionAnyOffice
函数原形：unsigned long SVN_API_RegisterProcessFuctionAnyOffice(SVN_RegFuc pfFuction)
函数说明：注册网关向AnyOffice发送消息的处理函数
输入参数：SVN_RegFuc pfFuction
输出参数：无
返回值  ：SVN_OK表示成功，其它表示失败
其它    :
*/
unsigned long SVN_API_RegisterProcessFuctionAnyOffice(SVN_RegFuc pfFuction);
/* END:   Added by fengzhengyu, 2013/7/3 */


 /* BEGIN: Added by liushangshu, 2012/4/16 for:内外网切换时，改变网关地址*/
/*****************************************************************************
* 函数名称            ：SVN_API_Resume 
* 函数原型            ：unsigned long SVN_API_Resume(char *pNewIpAddr);
* 描述                ：内外网切换时，改变连接的网关地址
*输入                 : 改变连接的网关地址
* 本函数调用的函数    ：无
* 调用本函数的函数    ：
* 本函数调用的全局变量：无
* 本函数影响的全局变量：
*****************************************************************************/    
unsigned long SVN_API_Resume(char *pNewIpAddr);
/* END: Added by liushangshu, 2012/4/16 for:内外网切换时，改变网关地址*/


/*****************************************************************************   
函数名称：SVN_API_UndoCAChecking
函数原形：unsigned long SVN_API_UndoCAChecking()
函数说明：设置与网关连接时，关闭校验CA。默认打开检验
输入参数：无
输出参数：无
返回值：SVN_OK为成功
*****************************************************************************/
unsigned long SVN_API_UndoCAChecking();
unsigned long SVN_API_DoCAChecking();
unsigned long SVN_API_GetServiceTicket(SVN_GetServiceTicketCallback getServiceTicketCallback);

unsigned long SVN_API_GetInfoByServiceTicket(const char * pcServiceTicket, SVN_GetInfoByServiceTicketCallBack serviceTicketValid);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SVN_API_H__ */
