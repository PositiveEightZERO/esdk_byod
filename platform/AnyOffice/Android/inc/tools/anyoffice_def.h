/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_def.h
  版 本 号   : 初稿
  作    者   : chenxingcheng
  生成日期   : 2012年9月12日
  最近修改   :
  功能描述   : 定义Anyoffice级别的宏定义、数据结构等。
               这些宏定义和数据结构可以被Anyoffice的所有组件引用，如SecMail, MDM, PushService等。
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月12日
    作    者   : chenxingcheng
    修改内容   : 创建文件

******************************************************************************/

#ifndef __ANYOFFICE_DEF_H__
#define __ANYOFFICE_DEF_H__


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/* BEGIN: Added by hexinxin, 2014/4/22   PN:迭代九网关支持端口可配置，客户端请求时增加端口*/
#define HIWORK_SSL_DEFAULT_PORT 443
/* END:   Added by hexinxin, 2014/4/22 */
/* BEGIN: Modified for 问题单号:DTS2013030503243 安全红线修改 by chenxingcheng, 2013/3/6 */
#define SAFE_LVL_REDLINE    0x10
/* BEGIN: Modified for 问题单号:DTS2013080100226 客户端日志中含有敏感信息 by wangyinan wKF70505, 2013/8/1 */
#define SAFE_LVL_DEV        0x20
/* END:   Modified by wangyinan wKF70505, 2013/8/1 */
/*
    说明及注意事项:
    1. 安全等级大于等于开发者等级时，可以打印用户名密码等敏感信息
    2. 开发人员本地调试时，可以使用 SAFE_LVL_DEV
    3. !!! 开发者等级不符合安全红线要求的，所以代码上库时保持安全等级为 SAFE_LVL_REDLINE !!!
    4. 后续开发约束: 打印敏感信息的代码都应该放到开发者等级的宏范围中，例如:
        #if (ANYOFFICE_SAFE_LVL >= SAFE_LVL_DEV)
            打印敏感信息
        #endif
    5. 当需要使用 #if (ANYOFFICE_SAFE_LVL >= SAFE_LVL_DEV) 时， 请直接或者间接包含此头文件，否则判断可能恒成立!!
        #include "anyoffice_def.h"
    6. 不要在其他文件中重定义该宏
*/
#define ANYOFFICE_SAFE_LVL   SAFE_LVL_REDLINE
/* END:   Modified by chenxingcheng, 2013/3/6 */

/* BEGIN: Added by chenxingcheng, 2012/9/5
使用Curl库进行Http请求时,不使用绿色组件定义的SVN_MAX_URL_LEN(128)，自定义更大的URL长度*/
#define ANYOFFICE_MAX_CURL_URL_LEN           1024

/* END:   Added by chenxingcheng, 2012/9/5 */

/* BEGIN: Added by chenxingcheng, 2012/10/15   PN:定义字节序*/
#define ANYOFFICE_LITTLE_ENDIAN      0
#define ANYOFFICE_BIG_ENDIAN         1
#define ANYOFFICE_UNKNOWN_ENDIAN    2
/* END:   Added by chenxingcheng, 2012/10/15 */

#define ANYOFFICE_ENABLE  1     /* 使能 */
#define ANYOFFICE_DISABLE 0     /* 未使能 */

#define ANYOFFICE_IPADDR_LEN      32   /* 最大地址长度 */
#define ANYOFFICE_MAX_SSID_LEN    128  /* 最大SSID长度 */

/* BEGIN: Added by ljj, 2014/9/18   PN:sdk对外提供获取ip/域名的接口*/
#define ANYOFFICE_MAX_DOM_LEN     128  /* 网关域名最大长度 */
/* END:   Added by ljj, 2014/9/18 */




/* BEGIN: Added by chenxingcheng, 2012/11/21   PN:当applist为空时，返回给UI的字符串*/
#define STRING_APPLIST_EMPTY    "{\"start\":0,\"count\":0,\"totalCnt\":0,\"items\":[]}" 
/* END:   Added by chenxingcheng, 2012/11/21 */
#define MDM_SAVE_APK_DIR        "/mnt/sdcard/AnyofficeDownload"

/* BEGIN: Added by xingxiaofeng, 2012/6/19 */
#define MID_SECENTRY_HIWORK     0xddcc0000
/* END:   Added by xingxiaofeng, 2012/6/19 */


#define ANYOFFICE_MALLOC(length) malloc(length)
#define ANYOFFICE_REMALLOC(pOldMemPtr,ulNewSize) realloc(pOldMemPtr,ulNewSize)
#ifndef ANYOFFICE_SAFE_FREE
#define ANYOFFICE_SAFE_FREE(data) \
    if(data) \
    {\
        free(data);\
        (data) = NULL; \
    }
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __ANYOFFICE_DEF_H__ */
