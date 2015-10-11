//
//  SvnDNSResolve.c
//  SvnSdk
//
//  Created by l00174413 on 14-2-25.
//
//

#import <Foundation/Foundation.h>

#include "SvnDNSResolve.h"




typedef struct tagASYNC_DNS_RESOLVE
{
    char*           pcUrl;
    unsigned long   ulResolvedIP;
    int             iWaitResolved;
    int             iAckResolved;
}ASYNC_DNS_RESOLVE_S;

ASYNC_DNS_RESOLVE_S* ASYNC_DNS_CreateAsyncResolveParam(const char* pcUrl, unsigned int ulLen);
void ASYNC_DNS_ReleaseAsyncResolveParam(ASYNC_DNS_RESOLVE_S* pstParam);
void ASYNC_DNS_AckResolvedFinished(ASYNC_DNS_RESOLVE_S* pstParam);

void ASYNC_DNS_ParseURLCallback(unsigned long ulIP[SVN_MAX_URL_NUM], void* pvData)
{
    ASYNC_DNS_RESOLVE_S* pstParam = NULL;
    
    if ( NULL == (pstParam = pvData) )
    {
        return;
    }
    
    pstParam->ulResolvedIP = htonl(ulIP[0]);
    
    NSLog(@"parseIP result:%d", pstParam->ulResolvedIP);
    ASYNC_DNS_AckResolvedFinished(pstParam);
    
    return;
}


void ASYNC_DNS_AckResolvedFinished(ASYNC_DNS_RESOLVE_S* pstParam)
{
    char cCtrl = 0;
    
    write(pstParam->iAckResolved, &cCtrl, sizeof(cCtrl));
}

int ASYNC_DNS_WaitResolvedFinished(ASYNC_DNS_RESOLVE_S* pstParam)
{
    fd_set          fdsAckWait  = {0};
    //struct timeval  tmTimeout   = {0};
    int             iSelectErr  = 0;
    int             iMaxFD      = 0;
    
    FD_ZERO(&fdsAckWait);
    FD_SET(pstParam->iWaitResolved, &fdsAckWait);
    iMaxFD = pstParam->iWaitResolved;
    
//    tmTimeout.tv_usec= (ulTimeoutMS % 1000) * 1000;
//    tmTimeout.tv_sec = ulTimeoutMS / 1000;
    
    //iSelectErr = select(iMaxFD + 1, &fdsAckWait, NULL, NULL, &tmTimeout);
    iSelectErr = select(iMaxFD + 1, &fdsAckWait, NULL, NULL, NULL);
    if ( iSelectErr < 0 )
    {
        //fail "select failed! err<%d><%s>", errno, strerror(errno)
        return FALSE;
    }
    
    if ( 0 == iSelectErr )
    {
        //timeout treat as failed
        return FALSE;
    }
    
    if ( FD_ISSET(pstParam->iWaitResolved, &fdsAckWait) )
    {
        return TRUE;
    }
    
    return FALSE;
}



ASYNC_DNS_RESOLVE_S* ASYNC_DNS_CreateAsyncResolveParam(const char* pcUrl, unsigned int ulLen)
{
    if(NULL == pcUrl || 0 == ulLen)
    {
        return NULL;
    }
    ASYNC_DNS_RESOLVE_S* pstParam = NULL;
    int iNotifyPipe[2] = {-1, -1};
    
    pstParam = malloc(sizeof(ASYNC_DNS_RESOLVE_S));
    
    if ( NULL == pstParam )
    {
        return NULL;
    }
    memset(pstParam, 0x00, sizeof(ASYNC_DNS_RESOLVE_S));
    
    pstParam->pcUrl = malloc(ulLen);
    
    if ( NULL == pstParam->pcUrl)
    {
        return NULL;
    }
    
    memcpy(pstParam->pcUrl, pcUrl, ulLen);
    
    if (pipe(iNotifyPipe) < 0 )
    {
        return NULL;
    }
    pstParam->iWaitResolved = iNotifyPipe[0];
    pstParam->iAckResolved = iNotifyPipe[1];
    
    return pstParam;
    
}


void ASYNC_DNS_ReleaseAsyncResolveParam(ASYNC_DNS_RESOLVE_S* pstParam)
{
    if(NULL == pstParam)
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


unsigned long ASYNC_DNS_ParseURL(const char *pcURL, unsigned int ulLen)
{
    unsigned long   ulResolvedUrl   = 0;
	ASYNC_DNS_RESOLVE_S* pstAsyncDNS = NULL;
    
	pstAsyncDNS = ASYNC_DNS_CreateAsyncResolveParam(pcURL, ulLen);
    
    if(NULL == pstAsyncDNS)
    {
        return 0;
    }
	
	int iRet = SVN_API_ParseURL((unsigned char*)pcURL,
                            ulLen,
                            (SVN_ParseURLCallback)ASYNC_DNS_ParseURLCallback,
                            (void*)pstAsyncDNS);
    
    if(iRet != SVN_OK)
    {
        return 0;
    }
    
    
    if (FALSE == ASYNC_DNS_WaitResolvedFinished(pstAsyncDNS))
	{
        ASYNC_DNS_ReleaseAsyncResolveParam(pstAsyncDNS);
	    /* 解析失败 */
	    return 0;
	}
	
	/* 解析成功 */
	ulResolvedUrl = pstAsyncDNS->ulResolvedIP;
    ASYNC_DNS_ReleaseAsyncResolveParam(pstAsyncDNS);
	
    return ulResolvedUrl;
}

