/******************************************************************************

                  版权所有 (C), 2012-2020, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_list.h
  版 本 号   : 初稿
  作    者   : 张晓垒/zKF69242
  生成日期   : 2012年12月30日
  最近修改   :
  功能描述   : 单向链表实现
  函数列表   :
  修改历史   :
  1.日    期   : 2012年12月30日
    作    者   : 张晓垒/zKF69242
    修改内容   : 创建文件, for OR.EAS.007, for OR.EAS.008

******************************************************************************/

#ifndef _SECMAIL_LIST_H
#define _SECMAIL_LIST_H

//#include "vrpcfg.h"
#include "tools_common.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagListCell 
{
  VOID *pvData;
  struct tagListCell *pstPrev;
  struct tagListCell *pstNext;
} LIST_CELL_S;

typedef struct tagList {
  LIST_CELL_S * pstFirst;
  LIST_CELL_S * pstLast;
  ULONG ulCount;
} LIST_S;

typedef LIST_CELL_S LIST_ITER_S;

/* Allocate a new pointer list */
LIST_S* Tools_API_List_New();

/* Destroys a list. Data pointed by data pointers is NOT freed. */
VOID Tools_API_List_Free(LIST_S *);

/** BEGIN: Added by guofeng 00218442, 2013/3/29   PN:OR.EAS.007 增加和notes服务器对接时的push功能*/
VOID Tools_API_List_FreeEx(LIST_S * lst, void (*freeCallBack)(void *)) ;
/** END:   Added by guofeng 00218442, 2013/3/29 */

/* Inserts this data pointer before the element pointed by the iterator */
INT32 Tools_API_List_InsertBefore(LIST_S *, LIST_ITER_S *, void *);

/* Inserts this data pointer after the element pointed by the iterator */
INT32 Tools_API_List_InsertAfter(LIST_S *, LIST_ITER_S *, void *);

/* BEGIN: Added for PN:联系人应用拆分 by zhaopindong, 2014/7/4 */
INT32 Tools_API_List_MoveToFirst(LIST_S * lst, LIST_ITER_S * iter) ;

INT32 Tools_API_List_MoveToLast(LIST_S * lst, LIST_ITER_S * iter) ;
/* END:   Added for PN:联系人应用拆分 by zhaopindong, 2014/7/4 */

/* Some of the following routines can be implemented as macros to
   be faster. If you don't want it, define NO_MACROS */
#ifdef NO_MACROS

/* Returns TRUE if list is empty */
INT32 Tools_API_List_IsEmpty(LIST_S *);

/* Returns the number of elements in the list */
INT32 Tools_API_List_GetCount(LIST_S *);

/* Returns an iterator to the first element of the list */
LIST_ITER_S* Tools_API_List_Begin(LIST_S *);

/* Returns an iterator to the last element of the list */
LIST_ITER_S* Tools_API_List_End(LIST_S *);

/* Returns an iterator to the next element of the list */
LIST_ITER_S* Tools_API_List_Next(LIST_ITER_S *);

/* Returns an iterator to the previous element of the list */
LIST_ITER_S* Tools_API_List_Previous(LIST_ITER_S *);

/* Returns the data pointer of this element of the list */
VOID* Tools_API_List_GetContent(LIST_ITER_S *);

/* Inserts this data pointer at the beginning of the list */
INT32 Tools_API_List_Prepend(LIST_S *, void *);

/* Inserts this data pointer at the end of the list */
INT32 Tools_API_List_Append(LIST_S *, void *);
#else
/* BEGIN: Modify by denghui 00217247, 2013/4/12   PN:DTS2013041201302  增加list保护*/
#define     Tools_API_List_IsEmpty(lst)             ((NULL == (lst))? 1 : (((lst)->pstFirst==(lst)->pstLast) && ((lst)->pstLast==NULL)))
#define     Tools_API_List_GetCount(lst)            ((NULL == (lst))? 0 : ((lst)->ulCount))
#define     Tools_API_List_Begin(lst)               ((NULL == (lst))? NULL : ((lst)->pstFirst))
#define     Tools_API_List_End(lst)                 ((NULL == (lst))? NULL :((lst)->pstLast))
#define     Tools_API_List_Next(iter)               (iter ? (iter)->pstNext : NULL)
#define     Tools_API_List_Previous(iter)           (iter ? (iter)->pstPrev : NULL)
#define     Tools_API_List_GetContent(iter)         (iter ? (iter)->pvData : NULL)
#define     Tools_API_List_Prepend(lst, data)  ((NULL == (lst))? -1 : (Tools_API_List_InsertBefore(lst, (lst)->pstFirst, data)))
#define     Tools_API_List_Append(lst, data)   ((NULL == (lst))? -1 : (Tools_API_List_InsertAfter(lst, (lst)->pstLast, data)))
/* END: Modify by denghui 00217247, 2013/4/12   PN:DTS2013041201302  增加list保护*/
#endif


/* Deletes the element pointed by the iterator.
   Returns an iterator to the next element. */
LIST_ITER_S* Tools_API_List_Delete(LIST_S *, LIST_ITER_S *);

/** BEGIN: Added by guofeng 00218442, 2013/3/29   PN:OR.EAS.007 增加和notes服务器对接时的push功能*/
LIST_ITER_S * Tools_API_List_DeleteEx(LIST_S * lst, LIST_ITER_S * iter, void (*freeCallBack)(void *)) ;
/** END:   Added by guofeng 00218442, 2013/3/29 */


typedef VOID (* List_Func)(void *, void *);

VOID Tools_API_List_Foreach(LIST_S * lst, List_Func func, void * data);

VOID Tools_API_List_Concat(LIST_S * dest, LIST_S * src);

VOID* Tools_API_List_GetDataAtIndex(LIST_S * lst, int indx);

LIST_ITER_S* Tools_API_List_GetAtIndex(LIST_S * lst, int indx);

#ifdef __cplusplus
}
#endif

#endif
