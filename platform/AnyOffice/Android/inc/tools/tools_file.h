#ifndef __TOOLS_FILE_H__
#define __TOOLS_FILE_H__
/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : tools_file.h
  版 本 号   : 初稿
  作    者   : zhangyantao
  生成日期   : 2014年7月27日
  最近修改   :
  功能描述   : 文件操作相关工具
  函数列表   :
  修改历史   :
  1.日    期   : 2014年7月27日
    作    者   : zhangyantao 00288282
    修改内容   : 创建文件

******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "tools_common.h"


#ifndef PATH_MAX
#define PATH_MAX    512
#endif

INT32 Tools_File_Mkdir(CHAR *pcPath);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


