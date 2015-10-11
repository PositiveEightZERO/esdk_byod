/******************************************************************************

                  版权所有 (C), 2001-2013, 华为技术有限公司

 ******************************************************************************
  文 件 名   : tools_common.h
  版 本 号   : 初稿
  作    者   : lifangxiang/00218420
  生成日期   : 2013年11月1日
  最近修改   :
  功能描述   : tools_common.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月1日
    作    者   : lifangxiang/00218420
    修改内容   : 创建文件

******************************************************************************/
#ifndef __TOOLS_COMMON_H__
#define __TOOLS_COMMON_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "tools_err.h"
 
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
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
#ifndef VOS_OK
#define VOS_OK 0
#endif 
    
#ifndef VOS_ERR
#define VOS_ERR 1
#endif 

#ifndef VOS_BOOL
#define VOS_BOOL unsigned long
#endif

#ifndef VOS_FALSE
#define VOS_FALSE 0
#endif
    
#ifndef VOS_TRUE
#define VOS_TRUE 1
#endif

#ifndef SIZE_T
#define SIZE_T unsigned long
#endif

/*----------------------------------------------*
 * 内部结构定义                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部结构定义                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 简单类型定义                                 *
 *----------------------------------------------*/
#ifndef INT32
#define INT32 int
#endif

#ifndef UINT32
#define UINT32 unsigned int        
#endif

#ifndef UINT
#define UINT unsigned int        
#endif

#ifndef LONG
#define LONG long
#endif

#ifndef ULONG
#define ULONG  unsigned long
#endif

#ifndef SHORT
#define SHORT short
#endif

#ifndef USHORT
#define  USHORT unsigned short
#endif

#ifndef CHAR
#define CHAR char
#endif

#ifndef UCHAR
#define UCHAR unsigned char 
#endif

#ifndef INT64
#define INT64 long long
#endif

#ifndef UINT64
#define UINT64 unsigned long long  
#endif

#ifndef FLOAT
typedef float               FLOAT;
#endif

#ifndef DOUBLE
typedef double              DOUBLE;
#endif

#ifndef VOID
typedef void                VOID;
#endif

#ifndef VOS_UINT32
#define VOS_UINT32 unsigned long
#endif

#ifndef VOS_INT32
#define VOS_INT32 long
#endif

#ifndef VOS_CHAR
#define VOS_CHAR char
#endif

#ifndef VOS_VOID
#define VOS_VOID void
#endif

#if defined(__APPLE__)
#else
#ifndef BOOL
#   ifdef __cplusplus
    typedef bool            BOOL;
#   else
    /*typedef UCHAR           BOOL;*/
#   endif
#endif
#endif
#ifndef CONST
#define CONST               const
#endif

#ifndef ECODE_E
typedef INT32               ECODE_E;
#endif

/*----------------------------------------------*
 * 常用常量宏定义                               *
 *----------------------------------------------*/
#ifndef NUM_8
#define NUM_8 8
#endif

#ifndef NUM_16
#define NUM_16 16
#endif

#ifndef NUM_32
#define NUM_32 32
#endif

#ifndef NUM_64
#define NUM_64 64
#endif

#ifndef NUM_128
#define NUM_128 128
#endif

#ifndef NUM_256
#define NUM_256 256
#endif

#ifndef NUM_512
#define NUM_512 512
#endif

#ifndef NUM_1024
#define NUM_1024 1024
#endif


/*----------------------------------------------*
 * 常用字节宏定义                               *
 *----------------------------------------------*/
#ifndef SIZE_BYTE_8
#define SIZE_BYTE_8 NUM_8
#endif

#ifndef SIZE_BYTE_16
#define SIZE_BYTE_16 NUM_16
#endif

#ifndef SIZE_BYTE_32
#define SIZE_BYTE_32 NUM_32
#endif

#ifndef SIZE_BYTE_64
#define SIZE_BYTE_64 NUM_64
#endif

#ifndef SIZE_BYTE_256
#define SIZE_BYTE_256 NUM_256
#endif

#ifndef SIZE_BYTE_512
#define SIZE_BYTE_512 NUM_512
#endif

#ifndef SIZE_BYTE_1024
#define SIZE_BYTE_1024 NUM_1024
#endif

#ifndef SIZE_KBYTE
#define SIZE_KBYTE SIZE_BYTE_1024
#endif

#ifndef SIZE_KBYTE_16
#define SIZE_KBYTE_16 (NUM_16 * SIZE_KBYTE)
#endif


/*----------------------------------------------*
 * 默认值定义                                   *
 *----------------------------------------------*/
#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef STATIC
#define STATIC static
#endif

/*----------------------------------------------*
 * BOOL值定义                                   *
 *----------------------------------------------*/
#ifndef TRUE
#   ifdef __cplusplus
#   define TRUE true
#   else
#   define TRUE (1)
#   endif
#endif

#ifndef FALSE
#   ifdef __cplusplus
#   define FALSE false
#   else
#   define FALSE (0)
#   endif
#endif
/*----------------------------------------------*
 * 安全释放宏定义                               *
 *----------------------------------------------*/
#ifndef TOOLS_HLP_FREE
#define TOOLS_HLP_FREE(ptr)\
            if ( NULL != (ptr) )\
            {\
                free(ptr);\
                (ptr) = NULL;\
            }
#endif


#ifndef TOOLS_HLP_FREE_EX
#define TOOLS_HLP_FREE_EX(ptr, freefunc)\
            if ( NULL != (ptr) )\
            {\
                if ( NULL != (freefunc) )\
                {\
                    freefunc(ptr);\
                }\
                (ptr) = NULL;\
            }
#endif
/*----------------------------------------------*
 * 内存申请                                     *
 *----------------------------------------------*/
#ifndef TOOLS_HLP_MALLOC
#define TOOLS_HLP_MALLOC(size) malloc(size)
#endif

#ifndef TOOLS_HLP_REALLOC
#define TOOLS_HLP_REALLOC(data, size)  realloc(data, size)
#endif

#ifndef TOOLS_HLP_CALLOC
#define TOOLS_HLP_CALLOC(count, size)  calloc(count, size)
#endif




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TOOLS_COMMON_H__ */

