/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_strconv.h
  版 本 号   : 初稿
  作    者   : chenxingcheng
  生成日期   : 2012年10月13日
  最近修改   :
  功能描述   : anyoffice_strconv 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年10月13日
    作    者   : chenxingcheng
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ANYOFFICE_STRCONV_H__
#define __ANYOFFICE_STRCONV_H__

#include "tools_common.h"

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

/* BEGIN: Added by huangyiyuan hkf60364, 2013/5/28   PN:DTS2013052502935*/
typedef struct tagPtmCodeTable
{
    USHORT usFrom;
    USHORT usTo;
}PTM_CodeTable;


#define    PTM_TABLE_LEN 23940          /*gbk unicode对应表的长度*/ 



extern int AnyOffice_StrConv_Gbk2Unicode(UCHAR *gbk_buf, USHORT *unicode_buf, int max_unicode_buf_size, int endian);
extern int AnyOffice_StrConv_Unicode2Gbk(USHORT *unicode_buf, UCHAR *gbk_buf, int max_gbk_buf_size, int endian);

extern int AnyOffice_StrConv_Unicode2Utf8(USHORT *unicode_buf, UCHAR *utf8_buf, int max_utf8_buf_size, int endian);
extern int AnyOffice_StrConv_Utf82Unicode(UCHAR *utf8_buf, USHORT *unicode_buf, int max_unicode_buf_size, int endian);

/* END:   Added by huangyiyuan hkf60364, 2013/5/28 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __ANYOFFICE_STRCONV_H__ */




