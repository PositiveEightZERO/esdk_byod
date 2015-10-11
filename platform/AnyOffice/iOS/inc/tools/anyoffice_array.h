/******************************************************************************

                  版权所有 (C), 2012-2020, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_array.h
  版 本 号   : 初稿
  作    者   : 张晓垒/zKF69242
  生成日期   : 2012年12月30日
  最近修改   :
  功能描述   :动态指针数组实现 
  函数列表   :
              Tools_API_Array_GetCount
              Tools_API_Array_GetData
              Tools_API_Array_GetDataAtIndex
              Tools_API_Array_SetDataAtIndex
  修改历史   :
  1.日    期   : 2012年12月30日
    作    者   : 张晓垒/zKF69242
    修改内容   : 创建文件, for OR.EAS.007, for OR.EAS.008

******************************************************************************/

#ifndef __ANYOFFICE_ARRAY_H__
#define __ANYOFFICE_ARRAY_H__

#ifdef __cplusplus
extern "C" {
#endif


/* array结构体定义 */
typedef struct tagArray {
  void **ppvArray;
  unsigned long ulLen;
  unsigned long ulMax;
} ARRAY_S;

ARRAY_S* Tools_API_Array_New(unsigned long ulInitSize);

int Tools_API_Array_Add(ARRAY_S *pstArray, void *pvData, unsigned long *ulIndx);

int Tools_API_Array_SetSize(ARRAY_S *pstArray, unsigned long ulNewSize);

int Tools_API_Array_Delete(ARRAY_S *pstArray, unsigned long ulIndx);

int Tools_API_Array_DeleteSlow(ARRAY_S *pstArray, unsigned long ulIndx);

int Tools_API_Array_DeleteFast(ARRAY_S *pstArray, unsigned long ulIndx);

/* Some of the following routines can be implemented as macros to
   be faster. If you don't want it, define NO_MACROS */

#ifdef NO_MACROS
/* Returns the array itself */
void** Tools_API_Array_GetData(ARRAY_S *);

/* Returns the number of elements in the array */
unsigned long Tools_API_Array_GetCount(ARRAY_S *);

/* Returns the contents of one cell */
void* Tools_API_Array_GetDataAtIndex(ARRAY_S *pstArray, unsigned long ulIndx);

/* Sets the contents of one cell */
VOID Tools_API_Array_SetDataAtIndex(ARRAY_S *pstArray, unsigned long ulIndx, void* pvValue);

#else
static void** Tools_API_Array_GetData(ARRAY_S *pstArray);
static unsigned long Tools_API_Array_GetCount(ARRAY_S *pstArray);
static void* Tools_API_Array_GetDataAtIndex(ARRAY_S *pstArray, unsigned long ulIndx);
static void Tools_API_Array_SetDataAtIndex(ARRAY_S *pstArray, unsigned long ulIndx, void *pvValue);
#endif

void Tools_API_Array_Free(ARRAY_S *pstArray);

#ifdef __cplusplus
}
#endif

#endif
