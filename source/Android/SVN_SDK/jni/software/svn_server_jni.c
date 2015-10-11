#include <jni.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>  //for errno
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h> 
#include <pthread.h>

#include <android/log.h>

#include "svn_define.h"
#include "svn_api.h"
#include "svn_socket_api.h"
#include "mdmsdk_api.h"
#include "svn_dns_resolve.h"

#include "tools/tools_common.h"
#include "cert_api.h"


#define MAX_IP_LEN 256
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SDK", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "SDK", __VA_ARGS__))

/*jni 全局变量*/
JavaVM* g_JavaVM = NULL;
jmethodID g_CallbackMethodID;
jobject g_staticObj;
//jint g_parsedIP[SVN_MAX_URL_NUM];

extern unsigned long SVN_API_LoadCACertFile(unsigned char *pucCAFile, unsigned char *pucPwd);
extern unsigned long SVN_API_UndoCAChecking();
extern unsigned long SVN_API_DoCAChecking();



/* 域名解析回调函数指针，用于向上层传递解析后的域名 */
//void ParseURLCallback(unsigned long ulIP[SVN_MAX_URL_NUM], void *pvData)
//{
//    if(NULL == ulIP || NULL == pvData )
//    {
//        return;
//    }
//
//    jint *parsedIP = (jint *)pvData;
//    int i = 0;
//    for(i=0; i<SVN_MAX_URL_NUM; i++)
//    {
//        if(ulIP[i] == 0)
//        {
//            return;
//        }
//        parsedIP[i] = htonl(ulIP[i]);
//        LOGE("parsed ip:%d", parsedIP[i]);
//    }
//    //g_parsedIP[0] = (jint)ulIP[0];
//
//}




/*****************************************************************************
 Prototype    : WriteLogCallback
 Description  : 写日志回调函数
 Input        : None
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/6/28
    Author       : zhaixianqi 90006553
    Modification : Created function

*****************************************************************************/
int WriteLogCallback(unsigned char *pucLog, unsigned long ulLogLen,
        unsigned long ulLogLevel) {
    int iRet = SVN_ERR;
    iRet = JNI_Callback(pucLog, (int) ulLogLevel, 0, 0, 1);
    return iRet;
}


int TunnelStatusCallback(int iStatus, int iErrorCode) {
    int iRet = SVN_ERR;
    iRet = JNI_Callback(NULL, 0, iStatus, iErrorCode, 2);
    return iRet;
}

/*调用上层callback函数*/
int JNI_Callback(unsigned char *pucLog, int iLevel, int iStatus, int iErrorCode, int flag) 
{
    JNIEnv* staticEnv = NULL;
    jstring jstrLog = NULL;
    /*BEGIN:Modefied by liushangshu for 退出时崩溃，2013-8-9*/
    int iAttached = -1;
    //(*g_JavaVM)->AttachCurrentThread(g_JavaVM, (JNIEnv**) &staticEnv, NULL);
    if ( JNI_OK != (*g_JavaVM)->GetEnv(g_JavaVM,(JNIEnv**)&staticEnv,JNI_VERSION_1_4) )
    {
        iAttached = (*g_JavaVM)->AttachCurrentThread(g_JavaVM,(JNIEnv**)&staticEnv,NULL);  
    }
    /*END:Modefied by liushangshu for 退出时崩溃，2013-8-9*/
	
    if (1 == flag && NULL != pucLog) 
    {
        jstrLog = charTojstring(staticEnv, (char*) pucLog);
    }

    if (staticEnv) 
    {
        (*staticEnv)->CallVoidMethod(staticEnv, g_staticObj, g_CallbackMethodID,
                jstrLog, iLevel, iStatus, iErrorCode, flag);
        //(*g_JavaVM)->DetachCurrentThread(g_JavaVM);
    }
    else 
    {
        return SVN_ERR;
    }
    
    (*staticEnv)->DeleteLocalRef(staticEnv, jstrLog);
    /*BEGIN:Modefied by liushangshu for 退出时崩溃，2013-8-9*/
    if ( iAttached >= 0)
    {
       (*g_JavaVM)->DetachCurrentThread(g_JavaVM);
    }
    /*END:Modefied by liushangshu for 退出时崩溃，2013-8-9*/
    return SVN_OK;
}

/*获取回调函数全局对象变量和函数ID*/
void getJniCallBackInfo(JNIEnv* env, jobject obj, const char * cClassPath) 
{
    g_staticObj = (*env)->NewGlobalRef(env, obj);
    jclass clazz = (*env)->FindClass(env, cClassPath);
    g_CallbackMethodID = (*env)->GetMethodID(env, clazz, "callBack",
                "(Ljava/lang/String;IIII)V");

    return;
}



/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_setWorkingDir
 Description  : jni函数，设置工作目录
 Input        : JNIEnv* env   
                jobject obj   
                jstring wokp  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_setWorkingDir(JNIEnv* env,jobject obj,jstring WorkPath)
{
    char *pcWorkpath = (*env)->GetStringUTFChars(env,WorkPath,0) ;
    if(NULL != pcWorkpath)
    {
        SVN_API_SetWorkingDir(pcWorkpath);
        (*env)->ReleaseStringUTFChars(env,WorkPath,pcWorkpath);
    }
    return 0;
}

/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_initEnv
 Description  : jni函数，初始化sdk环境
 Input        : JNIEnv* env  
                jobject obj  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_initEnv(JNIEnv* env,jobject obj)   
{
    return SVN_API_InitEnv() ;
}
/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_initEnv
 Description  : jni函数，去初始化sdk环境
 Input        : JNIEnv* env  
                jobject obj  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_exitEnv(JNIEnv* env,jobject obj)   
{   
    return SVN_API_CleanEnv() ;
}
/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_getIpAddress
 Description  : jni函数，获取客户端获得的虚拟ip
 Input        : JNIEnv* env  
                jobject obj  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jstring JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_getIpAddress(JNIEnv* env,jobject obj)   
{   

    unsigned long ulIPAddess = 0;
    unsigned long ulMask = 0;
    char avIP[MAX_IP_LEN] = {0};
    struct    in_addr sin_addr ={0} ;
    
    SVN_API_GetTunnelIP(&ulIPAddess,&ulMask) ;
    sin_addr.s_addr = htonl(ulIPAddess);
    strcpy(avIP, inet_ntoa(sin_addr));
    
    return (*env)->NewStringUTF(env,avIP) ;
}

/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_getVPNStatus
 Description  : jni函数，获取vpn状态
 Input        : JNIEnv* env  
                jobject obj  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_getVPNStatus(JNIEnv* env,jobject obj)   
{   
    unsigned int ulVPNStatus = 0;
    unsigned int ulErrorCode = 0;
        
    SVN_API_GetTunnelStatus(&ulVPNStatus, &ulErrorCode);
    return ulVPNStatus ;
}

/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_setLogParam
 Description  : jni函数，设置日志保存路径，保存日志级别
 Input        : JNIEnv* env       
                jobject obj       
                jstring SavePath  
                jint LogLevel     
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_setLogParam(JNIEnv* env,jobject obj,jstring SavePath,jint LogLevel)
{
    jint iRet = -1;
    const char *pcSavePath = (*env)->GetStringUTFChars(env,SavePath,0) ;
    if(NULL != pcSavePath)
    {
        iRet = (int)SVN_API_SetLogParam(pcSavePath,(unsigned long)LogLevel);
        (*env)->ReleaseStringUTFChars(env,SavePath,pcSavePath);
    }
    return iRet;
}

/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_setCapPktParam
 Description  : jni函数，设置抓包开关，抓包文件保存路径
 Input        : JNIEnv* env       
                jobject obj       
                jlong IsOpen      
                jstring SavePath  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_setCapPktParam(JNIEnv* env,jobject obj,jlong IsOpen,jstring SavePath)
{
    jint iRet = -1;
//    const char *pcSavePath = (*env)->GetStringUTFChars(env,SavePath,0) ;
//    if(NULL != pcSavePath)
//    {
//        iRet = SVN_API_SetCapPktParam(IsOpen,pcSavePath);
//        (*env)->ReleaseStringUTFChars(env,SavePath,pcSavePath);
//    }
    return iRet;
}
/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_login
 Description  : jni函数，登陆网关
 Input        : JNIEnv* env   
                jobject obj   
                jobject _obj  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_login(JNIEnv* env,jobject obj,jobject _obj)   
{   
    jint iRet = -1;
    jbyteArray UserName;
    jbyte *jbUserName;
    jstring strencode = (*env)->NewStringUTF(env,"GBK");
    int length = 0;
    int count = 0;
    char * pcUserName = NULL;
    char * pcCerFile = NULL;
    char * pcCertContent = NULL;
    /*BEGIN: Add by liushangshu for SDK 连接，2013-7-26*/
	char *pcAppName  = NULL;
    char *pcAuthId = NULL;
    jbyteArray AppName ;
    jbyte *jbAppName;
     /*END: Add by liushangshu for SDK 连接，2013-7-26*/
	SVN_REGISTER_INFO_S stRegisterInfo;
	memset(&stRegisterInfo, 0, sizeof(stRegisterInfo));
    jclass string_class = (*env)->FindClass(env,"java/lang/String");
    if(NULL == string_class)
    {
        return iRet;   
    }
    jmethodID getbyte = (*env)->GetMethodID(env,string_class, "getBytes", "(Ljava/lang/String;)[B");
    
    
    jclass login_class = (*env)->GetObjectClass(env,_obj);
    if (NULL == login_class )
    {
        (*env)->DeleteLocalRef(env, string_class);
        return iRet;
    }
    
    jmethodID getGatewayUrl = (*env)->GetMethodID(env,login_class,"getGatewayUrl","()Ljava/lang/String;");
    jstring   gatewayUrl = (*env)->CallObjectMethod(env,_obj,getGatewayUrl) ;
    const char * pcgGatewayUrl = (*env)->GetStringUTFChars(env,gatewayUrl,0) ;
    if ( NULL == pcgGatewayUrl )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        return iRet;
    }
    jmethodID getUserName = (*env)->GetMethodID(env,login_class,"getUserName","()Ljava/lang/String;");
    jstring username = (*env)->CallObjectMethod(env,_obj,getUserName) ;
    UserName = (jbyteArray)(*env)->CallObjectMethod(env,username, getbyte, strencode);
    length = (*env)->GetArrayLength(env,UserName);
    pcUserName = (char*)malloc(length+1);
    if ( NULL ==  pcUserName )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        return iRet;
    }
    
    jbUserName = (*env)->GetByteArrayElements(env, UserName, NULL);
    for( count = 0; count < length; count++)
    {
        pcUserName[count] = (char)jbUserName[count];
    }
    pcUserName[length] = 0;

    jmethodID getPassword = (*env)->GetMethodID(env,login_class,"getPassword","()Ljava/lang/String;");
    jstring password = (*env)->CallObjectMethod(env,_obj,getPassword) ;
    const char * pcPassword = (*env)->GetStringUTFChars(env,password,0) ;
    if ( NULL ==  pcPassword )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        free(pcUserName);
        return iRet;
    }

    jmethodID getGatewayPort = (*env)->GetMethodID(env,login_class,"getGatewayPort","()Ljava/lang/String;");
    jstring   ServerPort = (*env)->CallObjectMethod(env,_obj,getGatewayPort) ;
    const char * pcServerPort = (*env)->GetStringUTFChars(env,ServerPort,0) ;
    if ( NULL ==  pcServerPort )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        free(pcUserName);
        return iRet;
    }

    jmethodID getProxyType = (*env)->GetMethodID(env,login_class,"getProxyType","()S");
    jshort proxyType = (*env)->CallShortMethod(env,_obj, getProxyType) ;
    
    jmethodID getProxyUrl = (*env)->GetMethodID(env,login_class,"getProxyUrl","()Ljava/lang/String;");
    jstring   proxyUrl = (*env)->CallObjectMethod(env,_obj,getProxyUrl) ;
    const char * pcProxyUrl = (*env)->GetStringUTFChars(env,proxyUrl,0) ;
    if ( NULL ==  pcProxyUrl )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        free(pcUserName);
        return iRet;
    }
    
    jmethodID getProxyPort = (*env)->GetMethodID(env,login_class,"getProxyPort","()S");
    jshort proxyPort = (*env)->CallShortMethod(env,_obj, getProxyPort) ;
    
    
    jmethodID getProxyUsername = (*env)->GetMethodID(env,login_class,"getProxyUsername","()Ljava/lang/String;");
    jstring   proxyUsername = (*env)->CallObjectMethod(env,_obj,getProxyUsername) ;
    const char * pcProxyUsername = (*env)->GetStringUTFChars(env,proxyUsername,0) ;
    if ( NULL ==  pcProxyUsername )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
        free(pcUserName);
        return iRet;
    }
    
    jmethodID getProxyPwd = (*env)->GetMethodID(env,login_class,"getProxyPwd","()Ljava/lang/String;");
    jstring   proxyPwd = (*env)->CallObjectMethod(env,_obj,getProxyPwd) ;
    const char * pcProxyPwd = (*env)->GetStringUTFChars(env,proxyPwd,0) ;
    if ( NULL ==  pcProxyPwd )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
        (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
        free(pcUserName);
        return iRet;
    }
    
    jmethodID getAttestDomain = (*env)->GetMethodID(env,login_class,"getAttestDomain","()Ljava/lang/String;");
    jstring   attestDomain = (*env)->CallObjectMethod(env,_obj,getAttestDomain) ;
    const char * pcAtTestDomain = (*env)->GetStringUTFChars(env,attestDomain,0) ;
    if ( NULL ==  pcAtTestDomain )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
        (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
        (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
        free(pcUserName);
        return iRet;
    }
    
    jmethodID getCertPath = (*env)->GetMethodID(env,login_class,"getCertPath","()Ljava/lang/String;");
    jstring   certPath = (*env)->CallObjectMethod(env,_obj, getCertPath) ;
    pcCerFile = (*env)->GetStringUTFChars(env,certPath,0) ;
    if ( NULL ==  pcCerFile )
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
        (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
        (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
        (*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
        free(pcUserName);
        return iRet;
    }
    
    
    (void)SVN_API_LoadCACertFromFile((unsigned char*)pcCerFile);
    
    jmethodID getCertContent = (*env)->GetMethodID(env,login_class,"getCertContent","()[B");
    jbyteArray   certContent = (*env)->CallObjectMethod(env,_obj, getCertContent) ;
    
    if (NULL != certContent)
    {
        pcCertContent = (*env)->GetByteArrayElements(env, certContent, NULL); ;
        if ( NULL !=  pcCertContent )
        {
            (void)SVN_API_LoadCACertFile((unsigned char*)pcCertContent, "certPassword");
            (*env)->ReleaseByteArrayElements(env, certContent, pcCertContent, 0);        
        }
    }
    
    jmethodID getClientCert = (*env)->GetMethodID(env,login_class,"getClientCert","()Ljava/lang/String;");
    jstring   clientCert = (*env)->CallObjectMethod(env,_obj, getClientCert) ;
    const char* pcClientCert = (*env)->GetStringUTFChars(env, clientCert,0) ;
    if (NULL == pcClientCert)
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
        (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
        (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
        (*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
        (*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
        free(pcUserName);
        return iRet;
    }
    
    jmethodID getClientKey = (*env)->GetMethodID(env,login_class,"getClientKey","()Ljava/lang/String;");
    jstring   clientKey = (*env)->CallObjectMethod(env,_obj, getClientKey) ;
    const char* pcClientKey = (*env)->GetStringUTFChars(env, clientKey,0) ;
    if (NULL == pcClientKey)
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
        (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
        (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
        (*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
        (*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
        (*env)->ReleaseStringUTFChars(env,clientCert,pcClientCert) ;
        free(pcUserName);
        return iRet;
    }
    
    jmethodID getClientPassword = (*env)->GetMethodID(env,login_class,"getClientPassword","()Ljava/lang/String;");
    jstring   clientPwd = (*env)->CallObjectMethod(env,_obj, getClientPassword) ;
    const char* pcClientPwd = NULL;
    if(NULL != clientPwd)
    {
        pcClientPwd = (*env)->GetStringUTFChars(env, clientPwd, 0) ;
        if (NULL == pcClientPwd)
        {
            (*env)->DeleteLocalRef(env, string_class);
            (*env)->DeleteLocalRef(env, login_class);
            (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
            (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
            (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
            (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
            (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
            (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
            (*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
            (*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
            (*env)->ReleaseStringUTFChars(env,clientCert,pcClientCert) ;
            (*env)->ReleaseStringUTFChars(env,clientKey,pcClientKey) ;
            free(pcUserName);
            return iRet;
        }
    }



    SVN_CLIENTCERT_INFO_S stClientCertInfo;
    //stClientCertInfo.pucClientCert = pcClientCert;
    //stClientCertInfo.pucPrivatekey = pcClientKey;

    stClientCertInfo.ulClientCertLen = strlen(pcClientCert);
    stClientCertInfo.ulPrivateKeyLen = strlen(pcClientKey);
    stClientCertInfo.pucClientCert = malloc(stClientCertInfo.ulClientCertLen + 1);
    if (NULL == stClientCertInfo.pucClientCert)
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
        (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
        (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
        (*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
        (*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
        (*env)->ReleaseStringUTFChars(env,clientCert,pcClientCert) ;
        (*env)->ReleaseStringUTFChars(env,clientKey,pcClientKey) ;
        if (NULL != pcClientPwd)
        {
            (*env)->ReleaseStringUTFChars(env,clientPwd,pcClientPwd) ;
        }
        free(pcUserName);
        return iRet;
    }
    memcpy(stClientCertInfo.pucClientCert, pcClientCert, stClientCertInfo.ulClientCertLen);
    stClientCertInfo.pucClientCert[stClientCertInfo.ulClientCertLen] = '\0';
    
    stClientCertInfo.pucPrivatekey = malloc(stClientCertInfo.ulPrivateKeyLen + 1);
    if (NULL == stClientCertInfo.pucPrivatekey)
    {
        (*env)->DeleteLocalRef(env, string_class);
        (*env)->DeleteLocalRef(env, login_class);
        (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
        (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
        (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
        (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
        (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
        (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
        (*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
        (*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
        (*env)->ReleaseStringUTFChars(env,clientCert,pcClientCert) ;
        (*env)->ReleaseStringUTFChars(env,clientKey,pcClientKey) ;
        if (NULL != pcClientPwd)
        {
            (*env)->ReleaseStringUTFChars(env,clientPwd,pcClientPwd) ;
        }
        free(pcUserName);
        return iRet;
    }
    memcpy(stClientCertInfo.pucPrivatekey, pcClientKey, stClientCertInfo.ulPrivateKeyLen);
    stClientCertInfo.pucPrivatekey[stClientCertInfo.ulPrivateKeyLen] = '\0';
    
    if(NULL != pcClientPwd)
    {
        int iPwdLen = strlen(pcClientPwd) + 1;
        stClientCertInfo.pszPrivateKeyPwd = malloc(iPwdLen + 1);
        if (NULL == stClientCertInfo.pszPrivateKeyPwd)
        {
            (*env)->DeleteLocalRef(env, string_class);
            (*env)->DeleteLocalRef(env, login_class);
            (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
            (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
            (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
            (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
            (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
            (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
            (*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
            (*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
            (*env)->ReleaseStringUTFChars(env,clientCert,pcClientCert) ;
            (*env)->ReleaseStringUTFChars(env,clientKey,pcClientKey) ;
            if (NULL != pcClientPwd)
            {
                (*env)->ReleaseStringUTFChars(env,clientPwd,pcClientPwd) ;
            }
            free(pcUserName);
            return iRet;
        }


        memcpy(stClientCertInfo.pszPrivateKeyPwd, pcClientPwd, iPwdLen);
        stClientCertInfo.pszPrivateKeyPwd[iPwdLen] = '\0';
    }
    
    stClientCertInfo.usClientCertType = SVN_CERT_PEM;
    stClientCertInfo.usPrivateKeyType = SVN_PRIVATEKEY_PEM;

    SVN_API_ImportCert(&stClientCertInfo, NULL, NULL);

    
    jmethodID getTunnelMode = (*env)->GetMethodID(env,login_class,"getTunnelMode","()S");
    jshort tunnelMode = (*env)->CallShortMethod(env,_obj, getTunnelMode) ;
    
    jmethodID getCAChecking = (*env)->GetMethodID(env, login_class, "getCAChecking", "()S");
    jshort sCAChecking = (*env)->CallShortMethod(env,_obj, getCAChecking) ;
    
    if (sCAChecking == 0)
    {
        SVN_API_UndoCAChecking();
    }
    else
    {
        SVN_API_DoCAChecking();
    }
	
	/*BEGIN: Add by liushangshu for SDK 连接，2013-7-26*/
    jmethodID getConnectType = (*env)->GetMethodID(env,login_class,"getConnectType","()I");
    int connecttype = (*env)->CallIntMethod(env,_obj,getConnectType) ;
    if ( SVN_CONNECT_TYPE_SDK == connecttype )
    {
        jmethodID getAppName = (*env)->GetMethodID(env,login_class,"getAppName","()Ljava/lang/String;");
        jstring appname = (*env)->CallObjectMethod(env,_obj,getAppName) ;
        AppName = (jbyteArray)(*env)->CallObjectMethod(env,appname, getbyte, strencode);
        length = (*env)->GetArrayLength(env,AppName);
        pcAppName = (char*)malloc(length+1);
        if ( NULL ==  pcAppName )
        {
           	(*env)->DeleteLocalRef(env, string_class);
			(*env)->DeleteLocalRef(env, login_class);
			(*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
			(*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
			(*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
			(*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
			(*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
			(*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
			(*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
			(*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
            (*env)->ReleaseStringUTFChars(env,clientCert,pcClientCert) ;
            (*env)->ReleaseStringUTFChars(env,clientKey,pcClientKey) ;
            if (NULL != pcClientPwd)
            {
                (*env)->ReleaseStringUTFChars(env,clientPwd,pcClientPwd) ;
            }
            free(pcUserName);
            return iRet;
        }
        
        jbAppName = (*env)->GetByteArrayElements(env, AppName, NULL);
        for( count = 0; count < length; count++)
        {
            pcAppName[count] = (char)jbAppName[count];
        }
        pcAppName[length] = 0;
        
        jmethodID getAuthId = (*env)->GetMethodID(env,login_class,"getAuthId","()Ljava/lang/String;");
        jstring   authid = (*env)->CallObjectMethod(env,_obj,getAuthId) ;
        pcAuthId = (*env)->GetStringUTFChars(env,authid,0) ;
        if ( NULL ==  pcAuthId )
        {
			(*env)->DeleteLocalRef(env, string_class);
			(*env)->DeleteLocalRef(env, login_class);
			(*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
			(*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
			(*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
			(*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
			(*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
			(*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
			(*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
			(*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
            (*env)->ReleaseStringUTFChars(env,clientCert,pcClientCert) ;
            (*env)->ReleaseStringUTFChars(env,clientKey,pcClientKey) ;
            if (NULL != pcClientPwd)
            {
                (*env)->ReleaseStringUTFChars(env,clientPwd,pcClientPwd) ;
            }
			free(pcUserName);
            free(pcAppName);
            return iRet;
        }

        memcpy(stRegisterInfo.acAPPName,pcAppName,strlen(pcAppName));
        memcpy(stRegisterInfo.acAuthId,pcAuthId,strlen(pcAuthId));
        (*env)->ReleaseStringUTFChars(env,authid,pcAuthId) ;
        free(pcAppName);
    }
    stRegisterInfo.ulConnectType = connecttype;
	/*END: Add by liushangshu 2013-7-27*/
	
	
    stRegisterInfo.stProxyInfo.usProxyType = proxyType;
    
    stRegisterInfo.stProxyInfo.usProxyPort = proxyPort;

    memcpy(stRegisterInfo.stProxyInfo.acProxyUrl, (const char*)pcProxyUrl, strlen(pcProxyUrl));    
    memcpy(stRegisterInfo.stProxyInfo.acProxyUserName, (const char*)pcProxyUsername, strlen(pcProxyUsername));
    memcpy(stRegisterInfo.stProxyInfo.acProxyPassword, (const char*)pcProxyPwd, strlen(pcProxyPwd));
    memcpy(stRegisterInfo.stProxyInfo.acProxyDomain,  (const char*)pcAtTestDomain, strlen(pcAtTestDomain));

    
    memcpy(stRegisterInfo.acServerURL, pcgGatewayUrl, strlen(pcgGatewayUrl));
    stRegisterInfo.usServerPort = (unsigned short) atol(pcServerPort);
    memcpy(stRegisterInfo.acUserName, pcUserName, strlen(pcUserName));
    memcpy(stRegisterInfo.acPassword, pcPassword, strlen(pcPassword));
    
    stRegisterInfo.usTunnelMode = tunnelMode;

    /* BEGIN: Added by zhaixianqi 90006553, 2013/6/28   PN:Coverity问题修改*/
    getJniCallBackInfo(env, obj, "com/huawei/svn/sdk/server/SvnClientApiImpl");
    stRegisterInfo.pfWriteLogCallback = (SVN_WriteLogCallback)WriteLogCallback;
    stRegisterInfo.pfStatusCallback = (SVN_StatusCallback) TunnelStatusCallback;
    /* END:   Added by zhaixianqi 90006553, 2013/6/28 */
    
    
    iRet = SVN_API_CreateTunnel(&stRegisterInfo);
    
    (*env)->DeleteLocalRef(env, string_class);
    (*env)->DeleteLocalRef(env, login_class);
    (*env)->ReleaseStringUTFChars(env,gatewayUrl,pcgGatewayUrl) ;
    (*env)->ReleaseStringUTFChars(env,password,pcPassword) ;
    (*env)->ReleaseStringUTFChars(env,ServerPort,pcServerPort) ;
    (*env)->ReleaseStringUTFChars(env,proxyUrl,pcProxyUrl) ;
    (*env)->ReleaseStringUTFChars(env,proxyUsername,pcProxyUsername) ;
    (*env)->ReleaseStringUTFChars(env,proxyPwd,pcProxyPwd) ;
    (*env)->ReleaseStringUTFChars(env,attestDomain,pcAtTestDomain) ;
    (*env)->ReleaseStringUTFChars(env,certPath,pcCerFile) ;
    (*env)->ReleaseStringUTFChars(env,clientCert,pcClientCert) ;
    (*env)->ReleaseStringUTFChars(env,clientKey,pcClientKey) ;
    if (NULL != pcClientPwd)
    {
        (*env)->ReleaseStringUTFChars(env,clientPwd,pcClientPwd) ;
    }

    free(pcUserName);
    
     return iRet;
}

/*****************************************************************************
 Prototype    : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_logout
 Description  : jni函数，删除隧道
 Input        : JNIEnv* env  
                jobject obj  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/11
    Author       : liushangshu WX80847
    Modification : Created function

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_logout(JNIEnv* env,jobject obj)   
{   
    return SVN_API_DestroyTunnel() ;
}




/*****************************************************************************
 函 数 名  : FSM_JNI_encryptLarge
 功能描述  : 对原始内容进行加密
 输入参数  : JNIEnv *env
             jobject jobj     
             jbyteArray oriContent  
 输出参数  : 无
 返 回 值  : JNIEXPORT jbyteArray JNICALL
 调用函数  :
 被调函数  :
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_com_huawei_svn_sdk_server_SvnBigStringOpterations_encryptLarge(JNIEnv *env, jobject jobj, jbyteArray oriContent)
{
    unsigned char *pucBuffer = NULL;
    unsigned char  * ppucOutBuffer =NULL;
    unsigned long pulOutLen =0;
    int enResult = 0;
    unsigned long ulBuffLen = 0;
    
    //赋值
    ulBuffLen = (unsigned long)((*env)->GetArrayLength(env, oriContent));
    pucBuffer = (*env)->GetByteArrayElements(env, oriContent, NULL);
    if (NULL == pucBuffer)
    {
        return (jstring)0;
    }
    
    //调用API
    enResult = SVN_API_EncryptLarge(pucBuffer, ulBuffLen,&ppucOutBuffer, &pulOutLen);

    //失败
    if (0 != enResult)
    {
        (*env)->ReleaseByteArrayElements(env, oriContent, pucBuffer, 0);
        return (jstring)0;        
    }

    /* 将加密后的数据拷贝到byte数组中 */
    jbyteArray result = (*env)->NewByteArray(env, pulOutLen);
    if (NULL != result)
    {
        (*env)->SetByteArrayRegion(env, result, 0, pulOutLen, (jbyte *)ppucOutBuffer);
    }

    //释放空间，并返回
    (*env)->ReleaseByteArrayElements(env, oriContent, pucBuffer, 0);

    return result;
}

/*****************************************************************************
 函 数 名  : FSM_JNI_decryptLarge
 功能描述  : 对加密内容进行解密
 输入参数  : JNIEnv *env
             jobject jobj     
             jbyteArray encContent  
 输出参数  : 无
 返 回 值  : JNIEXPORT jbyteArray JNICALL
 调用函数  :
 被调函数  :
 
 修改历史      :
  1.日    期   : 2012年9月3日
    作    者   : yangligang
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_com_huawei_svn_sdk_server_SvnBigStringOpterations_decryptLarge(JNIEnv *env, jobject jobj, jbyteArray encContent)
{
    unsigned char *pucBuffer = NULL;
    unsigned char  * ppucOutBuffer =NULL;
    unsigned long pulOutLen =0;
    int enResult = 0;
    unsigned long ulBuffLen = 0;
    
    //赋值
    ulBuffLen = (unsigned long)((*env)->GetArrayLength(env, encContent));
    pucBuffer = (*env)->GetByteArrayElements(env, encContent, NULL);
    if (NULL == pucBuffer)
    {
        return (jstring)0;
    }
    
    //调用API
    enResult = SVN_API_DecryptLarge(pucBuffer, ulBuffLen,&ppucOutBuffer, &pulOutLen);

    //失败
    if (0 != enResult)
    {
        (*env)->ReleaseByteArrayElements(env, encContent, pucBuffer, 0);
        return (jstring)0;        
    }

    /* 将加密后的数据拷贝到byte数组中 */
    jbyteArray result = (*env)->NewByteArray(env, pulOutLen);
    if (NULL != result)
    {
        (*env)->SetByteArrayRegion(env, result, 0, pulOutLen, (jbyte *)ppucOutBuffer);
    }

    //释放空间，并返回
    (*env)->ReleaseByteArrayElements(env, encContent, pucBuffer, 0);

    return result;
}


/*
 * Class:     com_huawei_svn_sdk_SvnApi
 * Method:    nativeParseURL
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jintArray JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_parseURL(JNIEnv* env, jobject obj,
        jstring strURL) {

    jintArray result = (*env)->NewIntArray(env, SVN_MAX_URL_NUM);
    if(NULL == result)
    {
        return NULL;
    }
    jint parsedIP[SVN_MAX_URL_NUM];
    memset(parsedIP, 0, SVN_MAX_URL_NUM*sizeof(jint));
    //g_parsedIP[0] = 0;
    char * cURL = (*env)->GetStringUTFChars(env, strURL, 0);

    if (cURL != NULL)
    {
        //iRet = SVN_API_SdkParseURL(cURL);

        //int iRet = SVN_API_ParseURL(cURL, strlen(cURL), &ParseURLCallback, (void *)parsedIP);
        int iRet = ASYNC_DNS_ParseURL(cURL, strlen(cURL), parsedIP);
        (*env)->ReleaseStringUTFChars(env, strURL, cURL);
        if(0 == iRet )
        {
            return NULL;
        }

//        if(0 == iRet)
//        {
//            int count = 0;
//
//            while(parsedIP[0] == 0 && count <30)
//            {
//                count++;
//                sleep(1);
//            }
//        }

    }
    (*env)->SetIntArrayRegion(env, result, 0, SVN_MAX_URL_NUM, parsedIP);
    return result;
}


JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_checkBind(JNIEnv* env, jobject obj, jstring strParam)
{
    jint iRet = -1;

    char *pcParam =  (*env)->GetStringUTFChars(env, strParam, 0);

    if(pcParam != NULL)
    {
        iRet = SVN_API_CheckBind(pcParam, strlen(pcParam));
        (*env)->ReleaseStringUTFChars(env, strParam, pcParam);
   }
    return iRet;
}



JNIEXPORT jstring JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_getMdmViolationResult(JNIEnv* env,jobject obj)
{

    unsigned long ulOutLen = 0;
    char* pMdmCheckResult = 0;

    int ret = SVN_API_GetMdmViolationResult(&pMdmCheckResult, &ulOutLen);

    LOGE("SVN_API_GetMdmViolationResult:%d", ret);

    if(ret != SVN_OK || NULL == pMdmCheckResult )
    {
        return (jstring)0;
    }
    jstring result =  (*env)->NewStringUTF(env, pMdmCheckResult) ;
    free(pMdmCheckResult);

    return result;
}



/*****************************************************************************
 函 数 名  : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_initSandbox
 功能描述  : 初始化当前应用使用沙盒功能
 输入参数  : JNIEnv *env
             jobject jobj
             jstring strAppIdentifier
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月25日
    作    者   : lizhiyong
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_initSandbox(JNIEnv* env, jobject obj, jstring strAppIdentifier)
{
    jint iRet = -1;
    char *pcAppId = (*env)->GetStringUTFChars(env, strAppIdentifier, 0);

    if (pcAppId != NULL)
    {
        iRet = SVN_API_FileInitSandbox(pcAppId);
        (*env)->ReleaseStringUTFChars(env, strAppIdentifier, pcAppId);
    }

    return iRet;
}

/*****************************************************************************
 函 数 名  : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_clearSandbox
 功能描述  : 清除当前应用的沙盒数据
 输入参数  : JNIEnv *env
             jobject jobj
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月25日
    作    者   : lizhiyong
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_clearSandbox(JNIEnv* env, jobject obj)
{
     return SVN_API_FileClearSandbox();
}

/*****************************************************************************
 函 数 名  : Java_com_huawei_svn_sdk_server_SvnClientApiImpl_eraseSandboxFile
 功能描述  : 删除指定应用沙盒中的所有数据
 输入参数  : JNIEnv *env
             jobject jobj
             jstring strAppIdentifier
 输出参数  : 无
 返 回 值  : JNIEXPORT jint JNICALL
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月25日
    作    者   : lizhiyong
    修改内容   : 新生成函数

*****************************************************************************/
JNIEXPORT jint JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_eraseSandboxFile(JNIEnv* env, jobject obj, jstring strAppIdentifier)
{
    jint iRet = -1;
    char *pcAppId = (*env)->GetStringUTFChars(env, strAppIdentifier, 0);

    if (pcAppId != NULL)
    {
        iRet = SVN_API_FileEraseSandboxFile(pcAppId);
        (*env)->ReleaseStringUTFChars(env, strAppIdentifier, pcAppId);
    }

    return iRet;
}


JNIEXPORT void JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_setNetState(JNIEnv* env, jobject obj, jint iNetState)
{
    SVN_API_SetNetState((unsigned long) iNetState);
}

JNIEXPORT jstring JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_getAccountName(JNIEnv* env, jobject obj)
{

//	LONG AnyOffice_API_GetCertificate(const CHAR *pcUsername, CHAR **ppcData, ULONG *pulDataLen, CHAR **ppcPassword);
//	LONG AnyOffice_API_GetAccountName( CHAR **ppcAccountName);


    CHAR *pcAccountName = malloc(SVN_MAX_USERNAME_LEN);

    if(pcAccountName == NULL)
    {
    	LOGI("AnyOffice_API_GetAccountName malloc error");
    	return (jstring)0;
    }

    memset(pcAccountName, 0, SVN_MAX_USERNAME_LEN);
    LONG ret = AnyOffice_API_GetAccountName(&pcAccountName);

    LOGI("AnyOffice_API_GetAccountName Result:%d", ret);

    if(ret != SVN_OK)
    {
    	free(pcAccountName);
        return (jstring)0;
    }
    jstring result =  (*env)->NewStringUTF(env, pcAccountName) ;
    free(pcAccountName);

    return result;
}


JNIEXPORT jobject JNICALL Java_com_huawei_svn_sdk_server_SvnClientApiImpl_getCertificate(JNIEnv* env, jobject obj, jstring username)
{
	CHAR *pcOutBuffer =NULL;
	ULONG pulOutLen =0;
	CHAR *pcPassword =NULL;
    int enResult = 0;
    unsigned long ulBuffLen = 0;


    char *pcUsername = (*env)->GetStringUTFChars(env,username,0) ;
	if(NULL == pcUsername)
	{
		return (jobject)0;
	}

    //调用API
	LONG ret = AnyOffice_API_GetCertificate(pcUsername, &pcOutBuffer, &pulOutLen, &pcPassword);


	(*env)->ReleaseStringUTFChars(env, username, pcUsername);
    //失败
    if (0 != ret)
    {
        return (jobject)0;
    }

	jclass certClass = (*env)->FindClass(env, "com/huawei/svn/sdk/server/CertificateInfo");


	jmethodID constrocMID = (*env)->GetMethodID(env, certClass, "<init>","([BLjava/lang/String;)V");

	 /* 将加密后的数据拷贝到byte数组中 */
	jbyteArray content = (*env)->NewByteArray(env, pulOutLen);
	if (NULL != content)
	{
		(*env)->SetByteArrayRegion(env, content, 0, pulOutLen, (jbyte *)pcOutBuffer);
	}

	jstring password = (*env)->NewStringUTF(env, pcPassword);

	jobject cert_ojb = (*env)->NewObject(env, certClass, constrocMID, content, password);

	return cert_ojb ;

}

