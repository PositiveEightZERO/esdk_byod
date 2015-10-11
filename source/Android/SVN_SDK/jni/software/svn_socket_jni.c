#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>

#include "svn_define.h"
#include "svn_api.h"
#include "svn_socket_api.h"
#include "svn_socket_err.h"

/*jni层log输出函数*/
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "SDK", __VA_ARGS__))

/* 系统定义与组件内部定义值不同，如SOL_SOCKET系统为1；需要自定义 */
#define SVN_SOL_SOCKET      0xffff

#define SVN_SO_SNDBUF       0x1001          /* send buffer size */
#define SVN_SO_RCVBUF       0x1002          /* receive buffer size */
#define SVN_SO_SNDLOWAT     0x1003          /* send low-water mark */
#define SVN_SO_RCVLOWAT     0x1004          /* receive low-water mark */
#define SVN_SO_SNDTIMEO     0x1005          /* send timeout */
#define SVN_SO_RCVTIMEO     0x1006          /* receive timeout */
#define SVN_SO_ERROR        0x1007          /* get error status and clear */
#define SVN_SO_TYPE         0x1008          /* get socket type */
#define SVN_SO_RCVVLANID    0x1009          /* get vlan info from mbuf */
#define SVN_SO_RCVPVCID     0x1010          /* get pvc info from mbuf */

//转换函数jstring to char*
char* jstringTochar(JNIEnv * env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");

    jstring strencode = (*env)->NewStringUTF(env, "utf-8");

    jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes",
            "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid,
            strencode);
    jsize alen = (*env)->GetArrayLength(env, barr);
    jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);
    (*env)->DeleteLocalRef(env, clsstring);
    (*env)->DeleteLocalRef(env, strencode);
    (*env)->DeleteLocalRef(env, ba);
    return rtn;
}

//转换函数char* to jstring
jstring charTojstring(JNIEnv* env, const char* pat) {
    jstring strRetData = NULL;
    jclass strClass = (*env)->FindClass(env, "java/lang/String");

    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>",
            "([BLjava/lang/String;)V");

    jbyteArray bytes = (*env)->NewByteArray(env, strlen(pat));

    (*env)->SetByteArrayRegion(env, bytes, 0, strlen(pat), (jbyte*) pat);

    jstring encoding = (*env)->NewStringUTF(env, "utf-8");

    strRetData = (jstring)(*env)->NewObject(env, strClass, ctorID, bytes,
            encoding);
    (*env)->DeleteLocalRef(env, encoding);
    (*env)->DeleteLocalRef(env, bytes);
    (*env)->DeleteLocalRef(env, strClass);
    return strRetData;
}

/*调用返回值为整数的jni函数*/
int getJniIntData(JNIEnv* env, jobject obj, const char*cMethodName,
        char*cMethodType) {
    int iRetData = 0;
    jclass tempClass = (*env)->GetObjectClass(env, obj);
    jmethodID getTempMethodId = (*env)->GetMethodID(env, tempClass, cMethodName,
            cMethodType);
    iRetData = (*env)->CallIntMethod(env, obj, getTempMethodId);
    (*env)->DeleteLocalRef(env, tempClass);
    return iRetData;
}

/*调用返回值为String的jni函数*/
jstring getJniStringData(JNIEnv* env, jobject obj, const char*cMethodName,
        char*cMethodType) {

    if (NULL != env && NULL != obj && NULL != cMethodName
            && NULL != cMethodType) {

        jstring strRetData = NULL;

        jclass tempClass = (*env)->GetObjectClass(env, obj);

        jmethodID getTempMethodId = (*env)->GetMethodID(env, tempClass,
                cMethodName, cMethodType);

        strRetData = (*env)->CallObjectMethod(env, obj, getTempMethodId);

        (*env)->DeleteLocalRef(env, tempClass);
        return strRetData;
    }
    return NULL;
}

/*获取静态变量值*/
int GetStaticIntData(JNIEnv * env, jobject obj, const char * cStaticFieldName) {
    int iRet = 0;

    jclass tempClass = (*env)->GetObjectClass(env, obj);

    jfieldID statciFieldId = (*env)->GetStaticFieldID(env, tempClass,
            cStaticFieldName, "I");

    iRet = (*env)->GetStaticIntField(env, tempClass, statciFieldId);

    (*env)->DeleteLocalRef(env, tempClass);
    return iRet;
}

/* svn socket api */

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnSocket
 * Signature: (III)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnSocket(JNIEnv* env,
        jobject obj, jint iFamily, jint iType, jint iProtocol) {
    int iRet = 0;
    //iRet = svn_sdk_socket(iFamily, iType, iProtocol);
    iRet = svn_socket(iFamily, iType, iProtocol);
    return iRet;
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnBind
 * Signature: (ILcom/huawei/svn/sdk/SvnSockaddrIn;)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnBind(JNIEnv* env,
        jobject obj, jint iFd, jobject objSockAddr) {
    int iAddrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in stSockAddr = { 0 };

    jclass find_class = (*env)->GetObjectClass(env, objSockAddr);

    jmethodID getSin_family_ID = (*env)->GetMethodID(env, find_class,
            "getSinFamily", "()I");
    stSockAddr.sin_family = (*env)->CallIntMethod(env, objSockAddr,
            getSin_family_ID);

    jmethodID getSin_addr_ID = (*env)->GetMethodID(env, find_class,
            "getSinAddr", "()Ljava/lang/String;");
    jstring jstrSinAddr = (*env)->CallObjectMethod(env, objSockAddr,
            getSin_addr_ID);
    char * cSinAddr = (char*) (*env)->GetStringUTFChars(env, jstrSinAddr, 0);
    stSockAddr.sin_addr.s_addr = inet_addr(cSinAddr);
    (*env)->ReleaseStringUTFChars(env, jstrSinAddr, cSinAddr);

    jmethodID getSin_port_ID = (*env)->GetMethodID(env, find_class,
            "getSinPort", "()I");
    stSockAddr.sin_port = htons(
            (*env)->CallIntMethod(env, objSockAddr, getSin_port_ID));

    //return svn_sdk_bind(iFd, (struct sockaddr *) &stSockAddr, iAddrLen);
    return svn_bind(iFd, (struct sockaddr *) &stSockAddr, iAddrLen);
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnShutdown
 * Signature: (II)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnShutdown(JNIEnv* env,
        jobject obj, jint iFd, jint iHow) {
    //return svn_sdk_shutdown(iFd, iHow);
    return svn_shutdown(iFd, iHow);
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnClose
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnClose(JNIEnv* env,
        jobject obj, jint iFd) {
    //return svn_sdk_close(iFd);
    return svn_close(iFd);
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnConnect
 * Signature: (ILcom/huawei/svn/sdk/SvnSockaddrIn;)II
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnConnect(JNIEnv* env,
        jobject obj, jint iFd, jobject objServerAddr, jint iTimeout) {
    int iAddrLen = sizeof(struct sockaddr_in);
    int iRet = 0;
    int iOne = 1;
    int iZero = 0;
    //svn_sdk_fd_set readfd;
    //svn_sdk_fd_set writefd;
    svn_fd_set readfd;
    svn_fd_set writefd;

    struct timeval timeout = { 0 };

    struct sockaddr_in stSockAddr = { 0 };

    jclass find_class = (*env)->GetObjectClass(env, objServerAddr);

    jmethodID getSin_family_ID = (*env)->GetMethodID(env, find_class,
            "getSinFamily", "()I");
    stSockAddr.sin_family = (*env)->CallIntMethod(env, objServerAddr,
            getSin_family_ID);

    jmethodID getSin_addr_ID = (*env)->GetMethodID(env, find_class,
            "getSinAddr", "()Ljava/lang/String;");
    jstring jstrSinAddr = (*env)->CallObjectMethod(env, objServerAddr,
            getSin_addr_ID);
    char * cSinAddr = (char*) (*env)->GetStringUTFChars(env, jstrSinAddr, 0);
    stSockAddr.sin_addr.s_addr = inet_addr(cSinAddr);
    (*env)->ReleaseStringUTFChars(env, jstrSinAddr, cSinAddr);

    jmethodID getSin_port_ID = (*env)->GetMethodID(env, find_class,
            "getSinPort", "()I");
    stSockAddr.sin_port = htons(
            (*env)->CallIntMethod(env, objServerAddr, getSin_port_ID));

    if (0 != iTimeout) {
        timeout.tv_sec = iTimeout / 1000;
        timeout.tv_usec = (iTimeout % 1000) * 1000;
        LOGE("connect out of time is %d.%d", timeout.tv_sec, timeout.tv_usec);
//        SVN_SDK_FD_ZERO(&readfd);
//        SVN_SDK_FD_ZERO(&writefd);
//        SVN_SDK_FD_SET(iFd, &readfd);
//        SVN_SDK_FD_SET(iFd, &writefd);

        //SVN_FD_ZERO(&readfd);
        SVN_FD_ZERO(&writefd);
        //SVN_FD_SET(iFd, &readfd);
        SVN_FD_SET(iFd, &writefd);

        iRet = svn_ioctl(iFd, FIONBIO, &iOne);
        //iRet = svn_sdk_ioctl(iFd, FIONBIO, &iOne);
        if (0 == iRet) {
            LOGE("set nonblock success");
        } else {
            LOGE("set nonblock failed");
        }
    }
    else
    {
        if (0 == svn_ioctl(iFd, FIONBIO, &iZero)) {
            LOGE("set block success");
        } else {
            LOGE("set block failed");
        }

    }



    //iRet = svn_sdk_connect(iFd, (struct sockaddr *) &stSockAddr, iAddrLen);
    iRet = svn_connect(iFd, (struct sockaddr *) &stSockAddr, iAddrLen);
    if (SVN_OK == iRet) {
        LOGE("connect return 0,connect ok");
        return iRet;
    }
    if (SVN_EINPROGRESS == iRet) {
        //if (svn_sdk_select(iFd + 1, &readfd, &writefd, NULL, &timeout) > 0) {
        if (svn_select(iFd + 1, NULL, &writefd, NULL, &timeout) > 0) {
//            if (SVN_SDK_FD_ISSET(iFd, &writefd)) {
//                if (SVN_SDK_FD_ISSET(iFd, &readfd)) {
            if (SVN_FD_ISSET(iFd, &writefd)) {
                //if (SVN_FD_ISSET(iFd, &readfd)) {
                    //LOGE("connect error,fd is can RW");
                   // iRet = -1;
                //} else {
                    LOGE("socket connect success");
                    iRet = SVN_OK;
                //}

            }
        } else {
            iRet = -1;
            LOGE("connect timeout ,connect error");
        }
    }
    //else if(SVN_EAGAIN == iRet)
    //{
    //iRet = svn_connect(iFd, (struct sockaddr *) &stSockAddr, iAddrLen);
    //if(iRet != SVN_OK)
    //{
    //LOGE("reconnect error,errno is %d", iRet);
    //iRet = -1;
    //}
    //}
    else {
        LOGE("connect error,errno is %d", iRet);
        iRet = -1;
    }

    //if (0 == svn_sdk_ioctl(iFd, FIONBIO, &iZero)) {
    if (0 == svn_ioctl(iFd, FIONBIO, &iZero)) {
        LOGE("set block success");
    } else {
        LOGE("set block failed");
    }
    return iRet;
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnGetlocalport
 * Signature: (I)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnGetlocalport(
        JNIEnv* env, jobject obj, jint iFd) {
    //用svn_getsockname替换，里面能得到port
    return 0;
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnGetsockopt
 * Signature: (IIILjava/lang/Object;Ljava/lang/Object;)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnGetsockopt(
        JNIEnv* env, jobject obj, jint iFd, jint iLevel, jint iOptName,
        jintArray iArrayOptVal, jintArray iArrayOptLen) {
    int iRet = 0;
    char* pcOptVal = NULL;
    long lOptLen = 0;
    int iOptVal = 0;

    int* piOptValTmp = NULL;
    int* piOptLenTmp = NULL;

    struct timeval stTimeVal = { 0 };

    lOptLen = 8;   //预先把8字节的内存做出参，目前出参最大为时间相关属性的设置
    pcOptVal = (char*) malloc(lOptLen);

    //iRet = svn_sdk_getsockopt(iFd, iLevel, iOptName, pcOptVal, &lOptLen);
    iRet = svn_getsockopt(iFd, iLevel, iOptName, pcOptVal, &lOptLen);
    if (0 <= iRet) {
        if ((SVN_SOL_SOCKET == iLevel)
                && ((SVN_SO_RCVTIMEO == iOptName)
                        || (SVN_SO_SNDTIMEO == iOptName))) {
            stTimeVal.tv_sec = (*((int*) (pcOptVal)));
            stTimeVal.tv_usec = (*((int*) (pcOptVal) + 1));

            iOptVal = (stTimeVal.tv_sec) * 1000 + (stTimeVal.tv_usec) / 1000;
            lOptLen = sizeof(int);

        } else {
            if (sizeof(char) == lOptLen) {
                iOptVal = (int) (*pcOptVal);
                lOptLen = sizeof(char);
            } else {
                iOptVal = *((int*) (pcOptVal));
                lOptLen = sizeof(int);
            }
        }

    } else {
        return iRet;
    }

    piOptValTmp = (int*) (*env)->GetIntArrayElements(env, iArrayOptVal, NULL);
    *piOptValTmp = iOptVal;
    (*env)->ReleaseIntArrayElements(env, iArrayOptVal, piOptValTmp, 0);

    piOptLenTmp = (int*) (*env)->GetIntArrayElements(env, iArrayOptLen, NULL);
    *piOptLenTmp = lOptLen;
    (*env)->ReleaseIntArrayElements(env, iArrayOptLen, piOptLenTmp, 0);

    return iRet;

}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnSend
 * Signature: (I[BII)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnSend(JNIEnv* env,
        jobject obj, jint iFd, jbyteArray byteBuf, jint iLen, jint iFlags) {
    int iRet = 0;
    char* pcBuf = NULL;

    pcBuf = (unsigned char*) (*env)->GetByteArrayElements(env, byteBuf, NULL);

    //iRet = svn_sdk_send(iFd, pcBuf, iLen, iFlags);
    iRet = svn_send(iFd, pcBuf, iLen, iFlags);

    //LOGE("svn_send buffer len is %d, iRet is %d", iLen, iRet);

    (*env)->ReleaseByteArrayElements(env, byteBuf, pcBuf, 0);

    return iRet;
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnSendto
 * Signature: (I[BIILcom/huawei/svn/sdk/SvnSockaddrIn;)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnSendto(JNIEnv* env,
        jobject obj, jint iFd, jbyteArray byteBuf, jint iLen, jint iFlags,
        jobject objToAddr) {
    int iRet = 0;
    char* pcBuf = NULL;
    int iAddrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in stSockAddr = { 0 };

    jclass find_class = (*env)->GetObjectClass(env, objToAddr);

    jmethodID getSin_family_ID = (*env)->GetMethodID(env, find_class,
            "getSinFamily", "()I");
    stSockAddr.sin_family = (*env)->CallIntMethod(env, objToAddr,
            getSin_family_ID);

    jmethodID getSin_addr_ID = (*env)->GetMethodID(env, find_class,
            "getSinAddr", "()Ljava/lang/String;");
    jstring jstrSinAddr = (*env)->CallObjectMethod(env, objToAddr,
            getSin_addr_ID);
    char * cSinAddr = (*env)->GetStringUTFChars(env, jstrSinAddr, 0);
    stSockAddr.sin_addr.s_addr = inet_addr(cSinAddr);

    jmethodID getSin_port_ID = (*env)->GetMethodID(env, find_class,
            "getSinPort", "()I");
    stSockAddr.sin_port = htons(
            (*env)->CallIntMethod(env, objToAddr, getSin_port_ID));

    pcBuf = (unsigned char*) (*env)->GetByteArrayElements(env, byteBuf, NULL);

    //LOGE("Sendto sockFD is %d,buffer is %s,iLen is %d,iFlags is %d",iFd,pcBuf,iLen,iFlags);
    //LOGE("server ip address is %s:%d",cSinAddr,ntohs(stSockAddr.sin_port));

//    iRet = svn_sdk_sendto(iFd, pcBuf, iLen, iFlags,
//            (struct sockaddr *) &stSockAddr, iAddrLen);
    iRet = svn_sendto(iFd, pcBuf, iLen, iFlags, (struct sockaddr *) &stSockAddr,
            iAddrLen);

    (*env)->ReleaseByteArrayElements(env, byteBuf, pcBuf, 0);
    (*env)->ReleaseStringUTFChars(env, jstrSinAddr, cSinAddr);
    (*env)->DeleteLocalRef(env, find_class);
    //free(cSinAddr);
    return iRet;

}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnRecv
 * Signature: (I[BII)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnRecv(JNIEnv* env,
        jobject obj, jint iFd, jbyteArray byteBuf, jint iLen, jint iFlags) {
    int iRet = 0;
    char* pcBuf = NULL;

    pcBuf = (unsigned char*) (*env)->GetByteArrayElements(env, byteBuf, NULL);

    //iRet = svn_sdk_recv(iFd, pcBuf, iLen, iFlags);
    iRet = svn_recv(iFd, pcBuf, iLen, iFlags);

    (*env)->ReleaseByteArrayElements(env, byteBuf, pcBuf, 0);

    return iRet;
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnRecvfrom
 * Signature: (I[BIILcom/huawei/svn/sdk/SvnSockaddrIn;)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnRecvfrom(JNIEnv* env,
        jobject obj, jint iFd, jbyteArray byteBuf, jint iLen, jint iFlags,
        jobject objFromAddr) {
    int iRet = 0;
    char* pcBuf = NULL;
    int iAddrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in stSockAddr = { 0 };

    pcBuf = (unsigned char*) (*env)->GetByteArrayElements(env, byteBuf, NULL);

//    iRet = svn_sdk_recvfrom(iFd, pcBuf, iLen, iFlags,
//            (struct sockaddr *) &stSockAddr, (long*) (&iAddrLen));
    iRet = svn_recvfrom(iFd, pcBuf, iLen, iFlags,
            (struct sockaddr *) &stSockAddr, (long*) (&iAddrLen));

    (*env)->ReleaseByteArrayElements(env, byteBuf, pcBuf, 0);

    if (0 < iRet) {
        jclass find_class = (*env)->GetObjectClass(env, objFromAddr);

        char * cSinAddr = inet_ntoa(stSockAddr.sin_addr);
        jstring jstrSinAddr = charTojstring(env, cSinAddr); //change char to string

        jmethodID setSin_family_ID = (*env)->GetMethodID(env, find_class,
                "setSinFamily", "(I)V");
        (*env)->CallVoidMethod(env, objFromAddr, setSin_family_ID,
                stSockAddr.sin_family);

        jmethodID setSin_addr_ID = (*env)->GetMethodID(env, find_class,
                "setSinAddr", "(Ljava/lang/String;)V");
        (*env)->CallVoidMethod(env, objFromAddr, setSin_addr_ID, jstrSinAddr);

        jmethodID setSin_port_ID = (*env)->GetMethodID(env, find_class,
                "setSinPort", "(I)V");
        (*env)->CallVoidMethod(env, objFromAddr, setSin_port_ID,
                stSockAddr.sin_port);

        jmethodID setSin_addrlen_ID = (*env)->GetMethodID(env, find_class,
                "setSinAddrlen", "(I)V");
        (*env)->CallVoidMethod(env, objFromAddr, setSin_addrlen_ID, iAddrLen);

        (*env)->DeleteLocalRef(env, jstrSinAddr);
    }

    return iRet;
}

/*
 * Class:     com_huawei_svn_sdk_socket_SvnSocketApiImpl
 * Method:    svnSetsockopt
 * Signature: (IIIII)I
 */
JNIEXPORT jint
JNICALL Java_com_huawei_svn_sdk_socket_SvnSocketApiImpl_svnSetsockopt(
        JNIEnv* env, jobject obj, jint iFd, jint iLevel, jint iOptName,
        jint iOptVal, jint iOptLen) {
    char* pcOptVal = NULL;
    long lOptLen = 0;
    struct timeval stTimeVal = { 0 };

    pcOptVal = (char*) (&iOptVal);
    lOptLen = sizeof(int);

    //时间属性需要转换，外部强制传入毫秒
    //该处SOL_SOCKET等属性与组件内的不同，需要将组件内的设置
    if ((SVN_SOL_SOCKET == iLevel)
            && ((SVN_SO_RCVTIMEO == iOptName) || (SVN_SO_SNDTIMEO == iOptName))) {
        stTimeVal.tv_sec = (iOptVal / 1000);
        stTimeVal.tv_usec = (iOptVal % 1000) * 1000;

        pcOptVal = (char*) (&stTimeVal);
        lOptLen = sizeof(struct timeval);
    }

    //return svn_sdk_setsockopt(iFd, iLevel, iOptName, pcOptVal, lOptLen);
    return svn_setsockopt(iFd, iLevel, iOptName, pcOptVal, lOptLen);
}

