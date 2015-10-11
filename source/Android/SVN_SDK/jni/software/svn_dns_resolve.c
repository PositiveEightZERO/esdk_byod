//
//  SvnDNSResolve.c
//  SvnSdk
//
//  Created by l00174413 on 14-2-25.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <sys/select.h>
#include <android/log.h>

#include "svn_dns_resolve.h"

/*jni层log输出函数*/
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "SDK", __VA_ARGS__))

typedef struct tagASYNC_DNS_RESOLVE
{
    char* pcUrl;
    unsigned long ulResolvedIP[SVN_MAX_URL_NUM];
    int iWaitResolved;
    int iAckResolved;
} ASYNC_DNS_RESOLVE_S;

ASYNC_DNS_RESOLVE_S* ASYNC_DNS_CreateAsyncResolveParam(const char* pcUrl,
        unsigned int ulLen);
void ASYNC_DNS_ReleaseAsyncResolveParam(ASYNC_DNS_RESOLVE_S* pstParam);
void ASYNC_DNS_AckResolvedFinished(ASYNC_DNS_RESOLVE_S* pstParam);

void ASYNC_DNS_ParseURLCallback(unsigned long ulIP[SVN_MAX_URL_NUM],
        void* pvData)
{
    ASYNC_DNS_RESOLVE_S* pstParam = NULL;

    if (NULL == (pstParam = pvData))
    {
        return;
    }

    int i = 0;
    for (i = 0; i < SVN_MAX_URL_NUM; i++)
    {
        if (0 == ulIP[i])
        {
            break;
        }
        pstParam->ulResolvedIP[i] = htonl(ulIP[i]);
        LOGE("parsed ip:%d", pstParam->ulResolvedIP[i]);
    }

    //pstParam->ulResolvedIP = htonl(ulIP[0]);

    //NSLog(@"parseIP result:%d", pstParam->ulResolvedIP);
    ASYNC_DNS_AckResolvedFinished(pstParam);

    return;
}

void ASYNC_DNS_AckResolvedFinished(ASYNC_DNS_RESOLVE_S* pstParam)
{
	if(NULL == pstParam)
	{
		return;
	}
    char cCtrl = 0;
    //LOGE("write to:%d", pstParam->iAckResolved);
    write(pstParam->iAckResolved, &cCtrl, sizeof(cCtrl));
}

int ASYNC_DNS_WaitResolvedFinished(ASYNC_DNS_RESOLVE_S* pstParam)
{
    fd_set fdsAckWait = { 0 };
    struct timeval  tmTimeout   = {0};
    int iSelectErr = 0;
    int iMaxFD = 0;

    FD_ZERO(&fdsAckWait);
    FD_SET(pstParam->iWaitResolved, &fdsAckWait);
    iMaxFD = pstParam->iWaitResolved;

    tmTimeout.tv_usec= (20000 % 1000) * 1000;
    tmTimeout.tv_sec = 20000 / 1000;

//iSelectErr = select(iMaxFD + 1, &fdsAckWait, NULL, NULL, &tmTimeout);
    //LOGE("ASYNC_DNS_WaitResolvedFinished select before!,fd= %d", pstParam->iWaitResolved);
    iSelectErr = select(iMaxFD + 1, &fdsAckWait, NULL, NULL, &tmTimeout);
    //LOGE("ASYNC_DNS_WaitResolvedFinished select return!");
    if (iSelectErr < 0)
    {
        //fail "select failed! err<%d><%s>", errno, strerror(errno)
        LOGE("ASYNC_DNS_WaitResolvedFinished select failed!");
        return 0;
    }

    if (0 == iSelectErr)
    {
        //timeout treat as failed
        LOGE("ASYNC_DNS_WaitResolvedFinished select timeout!");
        return 0;
    }

    if (FD_ISSET(pstParam->iWaitResolved, &fdsAckWait))
    {
        //LOGE("ASYNC_DNS_WaitResolvedFinished FD_ISSET!");
        return 1;
    }
    //LOGE("ASYNC_DNS_WaitResolvedFinished FD_ISSET false!");
    return 0;
}

ASYNC_DNS_RESOLVE_S* ASYNC_DNS_CreateAsyncResolveParam(const char* pcUrl,
        unsigned int ulLen)
{
    if (NULL == pcUrl || 0 == ulLen)
    {
        return NULL;
    }
    ASYNC_DNS_RESOLVE_S* pstParam = NULL;
    int iNotifyPipe[2] =
    { -1, -1 };

    pstParam = malloc(sizeof(ASYNC_DNS_RESOLVE_S));

    if (NULL == pstParam)
    {
        return NULL;
    }
    memset(pstParam, 0x00, sizeof(ASYNC_DNS_RESOLVE_S));

    pstParam->pcUrl = malloc(ulLen);

    if (NULL == pstParam->pcUrl)
    {
        return NULL;
    }

    memcpy(pstParam->pcUrl, pcUrl, ulLen);

    if (pipe(iNotifyPipe) < 0)
    {
        return NULL;
    }
    pstParam->iWaitResolved = iNotifyPipe[0];
    pstParam->iAckResolved = iNotifyPipe[1];

    //LOGE("pipe result:%d, %d", iNotifyPipe[0], iNotifyPipe[1]);
    return pstParam;

}

void ASYNC_DNS_ReleaseAsyncResolveParam(ASYNC_DNS_RESOLVE_S* pstParam)
{
    if (NULL == pstParam)
    {
        return;
    }

    close(pstParam->iWaitResolved);
    close(pstParam->iAckResolved);

    if (pstParam->pcUrl != NULL)
    {
        free(pstParam->pcUrl);
    }

    free(pstParam);
}

unsigned long ASYNC_DNS_ParseURL(const char *pcURL, unsigned int ulLen,
        unsigned long ulResolvedIP[SVN_MAX_URL_NUM])
{
    LOGE("ASYNC_DNS_ParseURL start!");
    unsigned long ulResolvedUrl = 0;
    ASYNC_DNS_RESOLVE_S* pstAsyncDNS = NULL;
    if(NULL == pcURL || 0 == ulLen || NULL == ulResolvedIP)
    {
        LOGE("ASYNC_DNS_ParseURL param error!");
        return 0;
    }

    memset(ulResolvedIP, 0, SVN_MAX_URL_NUM*sizeof(unsigned long));

    pstAsyncDNS = ASYNC_DNS_CreateAsyncResolveParam(pcURL, ulLen);

    if (NULL == pstAsyncDNS)
    {
        LOGE("ASYNC_DNS_CreateAsyncResolveParam fail!");
        return 0;
    }

    int iRet = SVN_API_ParseURL((unsigned char*) pcURL, ulLen,
            (SVN_ParseURLCallback) ASYNC_DNS_ParseURLCallback,
            (void*) pstAsyncDNS);

    if (iRet != SVN_OK)
    {
        LOGE("SVN_API_ParseURL fail!");
        return 0;
    }

    if (0 == ASYNC_DNS_WaitResolvedFinished(pstAsyncDNS))
    {
        ASYNC_DNS_ReleaseAsyncResolveParam(pstAsyncDNS);
        /* 解析失败 */
        LOGE("ASYNC_DNS_WaitResolvedFinished fail!");
        return 0;
    }

    /* 解析成功 */
    int i = 0;
    for (i = 0; i < SVN_MAX_URL_NUM; i++)
    {
        if (0 == pstAsyncDNS->ulResolvedIP[i])
        {
            LOGE("ResolvedIP ip num:%d!", i);
            break;
        }
        ulResolvedIP[i] = pstAsyncDNS->ulResolvedIP[i];
    }

    ulResolvedUrl = ulResolvedIP[0];
    ASYNC_DNS_ReleaseAsyncResolveParam(pstAsyncDNS);

    return ulResolvedUrl;
}

