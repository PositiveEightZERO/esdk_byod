//
//  SvnDNSResolve.h
//  SvnSdk
//
//  Created by l00174413 on 14-2-25.
//
//

#ifndef SvnSdk_SvnDNSResolve_h
#define SvnSdk_SvnDNSResolve_h


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "svn_define.h"
#include "svn_api.h"
#include "svn_socket_api.h"
#include "svn_socket_err.h"
/**
 * 域名解析回调函数
 */
extern unsigned long ASYNC_DNS_ParseURL(const char* pucURL, unsigned int ulLen);

    
    
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
