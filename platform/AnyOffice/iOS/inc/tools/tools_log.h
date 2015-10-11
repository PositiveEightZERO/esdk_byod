/******************************************************************************

                  版权所有 (C), 2001-2013, 华为技术有限公司

 ******************************************************************************
  文 件 名   : tools_log.h
  版 本 号   : 初稿
  作    者   : lifangxiang/00218420
  生成日期   : 2013年11月5日
  最近修改   :
  功能描述   : tools_log.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月5日
    作    者   : lifangxiang/00218420
    修改内容   : 创建文件

******************************************************************************/
#ifndef __TOOLS_LOG_H__
#define __TOOLS_LOG_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <pthread.h>

#ifdef _lint
#ifndef __FUNCTION__
#define __FUNCTION__ ""
#endif /* end of __FUNCTION__ */
#endif

#define ANYOFFICE_SDK_ERR_LOG      1      
#define ANYOFFICE_SDK_WARN_LOG     2      
#define ANYOFFICE_SDK_NOTICE_LOG   3      
#define ANYOFFICE_SDK_DEBUG_LOG    4      

/* 日志模块ID */
#define TOOLS_MOD_NAME_SDK         0x1000



/*----------------------------------------------*
 * 自定义模块日志打印                           *
 *----------------------------------------------*/
extern void WriteLog(unsigned long ulInfoID, unsigned long ulErrorLevel, const char *pcFuncName, unsigned long ulLineNum, char *pcFormat, ...);
#define TOOLS_LOGD(mod, format, ...) WriteLog(mod, ANYOFFICE_SDK_DEBUG_LOG, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define TOOLS_LOGN(mod, format, ...) WriteLog(mod, ANYOFFICE_SDK_NOTICE_LOG, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define TOOLS_LOGW(mod, format, ...) WriteLog(mod, ANYOFFICE_SDK_WARN_LOG, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define TOOLS_LOGE(mod, format, ...) WriteLog(mod, ANYOFFICE_SDK_ERR_LOG, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

/*----------------------------------------------*
 * 本模块日志打印                               *
 *----------------------------------------------*/
#define _TOOLS_LOGD(format, ...) TOOLS_LOGD(TOOLS_MOD_NAME_SDK, format, ##__VA_ARGS__)
#define _TOOLS_LOGE(format, ...) TOOLS_LOGE(TOOLS_MOD_NAME_SDK, format, ##__VA_ARGS__)
#define _TOOLS_LOGN(format, ...) TOOLS_LOGN(TOOLS_MOD_NAME_SDK, format, ##__VA_ARGS__)
#define _TOOLS_LOGW(format, ...) TOOLS_LOGW(TOOLS_MOD_NAME_SDK, format, ##__VA_ARGS__)

#endif /* __TOOLS_LOG_H__ */
