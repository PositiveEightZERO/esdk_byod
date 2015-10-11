/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_list.h
  版 本 号   : 初稿
  作    者   : tianxin
  生成日期   : 2012年9月11日
  最近修改   :
  功能描述   : 双链表的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月11日
    作    者   : tianxin
    修改内容   : 创建文件

******************************************************************************/
/* BEGIN: Modified by chenxingcheng, 2012/10/12   PN:指定__ANYOFFICE_LIST_H__*/
#ifndef __ANYOFFICE_LIST_H__
#define __ANYOFFICE_LIST_H__

#include "svn_define.h"
#include "tools_common.h"
/* END:   Modified by chenxingcheng, 2012/10/12 */


#ifdef __cplusplus
extern "C"{
#endif

#ifndef offset_of
#define offset_of(type, memb) \
    (/*lint -e(413) */ (unsigned long)(&((type *)0)->memb))
#endif

#ifndef container_of
#define container_of(obj, type, memb) \
    ((type *)(((char *)obj) - offset_of(type, memb)))
#endif

typedef struct tagSVN_DList_Head
{
    struct tagSVN_DList_Head *pstNext;
    struct tagSVN_DList_Head *pstPrev;
}SVN_DLIST_HEAD_S;

#define SVN_DLIST_HEAD_ASSIGN(name) { &(name), &(name) }

#define SVN_DLIST_HEAD(name) \
    SVN_DLIST_HEAD_S name = SVN_DLIST_HEAD_ASSIGN(name)

#define SVN_DLIST_ENTRY(ptr, type, member) \
    container_of(ptr, type, member)

#define SVN_DLIST_FIRST_ENTRY(ptr, type, member) \
    SVN_DLIST_ENTRY((ptr)->pstNext, type, member)

#define SVN_DLIST_NEXT_ENTRY(ptr, type, member) \
    SVN_DLIST_ENTRY((ptr)->pstNext, type, member)

#define SVN_DLIST_LAST_ENTRY(ptr, type, member) \
    SVN_DLIST_ENTRY((ptr)->pstPrev, type, member)

#define SVN_DLIST_FOR_EACH(pos, head) \
    /*lint -save -e505 -e613 */ \
    for (pos = (head)->pstNext; (pos) != (head); pos = pos->pstNext)
    /*lint -restore*/

#define SVN_DLIST_FOR_EACH_ENTRY(pos, pstHead, type, member) \
    /*lint -save -e505 -e613*/ \
    for (pos = SVN_DLIST_ENTRY((pstHead)->pstNext,type,member); \
        &pos->member != (pstHead); \
        pos = SVN_DLIST_ENTRY(pos->member.pstNext,type,member)) \
    /*lint -restore */

#define SVN_DLIST_FOR_EACH_ENTRY_SAFE(pos, tmp, pstHead, type, member) \
    for (pos = SVN_DLIST_FIRST_ENTRY(pstHead, type, member), \
        tmp = SVN_DLIST_ENTRY(pos->member.pstNext, type, member); \
        &pos->member != (pstHead); \
        pos = tmp, tmp = SVN_DLIST_ENTRY(tmp->member.pstNext, type, member))

#define SVN_DLIST_EXCHANGE(pstListDst, pstListSrc) \
    do { \
       SVN_DLIST_HEAD_S stListTmp = {0}; \
       (VOID)TOOLS_MEMCPY_S(&stListTmp, sizeof(SVN_DLIST_HEAD_S), pstListDst, sizeof(SVN_DLIST_HEAD_S)); \
       (VOID)TOOLS_MEMCPY_S(pstListDst, sizeof(SVN_DLIST_HEAD_S), pstListSrc, sizeof(SVN_DLIST_HEAD_S)); \
       (VOID)TOOLS_MEMCPY_S(pstListSrc, sizeof(SVN_DLIST_HEAD_S), &stListTmp, sizeof(SVN_DLIST_HEAD_S)); \
       if ((pstListSrc == (pstListDst)->pstPrev) \
        && (pstListSrc == (pstListDst)->pstNext))\
       { \
          (pstListDst)->pstPrev = pstListDst;  \
          (pstListDst)->pstNext = pstListDst;  \
       } \
       if ((pstListDst == (pstListSrc)->pstPrev) \
        && (pstListDst == (pstListSrc)->pstNext))\
       { \
          (pstListSrc)->pstPrev = pstListSrc;  \
          (pstListSrc)->pstNext = pstListSrc;  \
       } \
       (pstListSrc)->pstNext->pstPrev = pstListSrc;   \
       (pstListSrc)->pstPrev->pstNext = pstListSrc;   \
       (pstListDst)->pstNext->pstPrev = pstListDst;   \
       (pstListDst)->pstPrev->pstNext = pstListDst;   \
    }while(0)

#define SVN_DLIST_FREENODES(ptr, type, member) \
    do{\
        type *pstNode2Del = NULL;\
        while(VOS_FALSE == SVN_DList_Empty(ptr))\
        {\
            pstNode2Del = SVN_DLIST_ENTRY((ptr)->pstNext, type, member);\
            SVN_DList_DelInit(&pstNode2Del->member);\
            MDM_Free(pstNode2Del);\
        }\
    }while(0);


#ifndef ANYOFFICE_GTEST
 VOID SVN_DList_HeadInit(SVN_DLIST_HEAD_S *pstList);

 VOID SVN_DList_AddNext(SVN_DLIST_HEAD_S *pstNew, SVN_DLIST_HEAD_S *pstHead);
 VOID SVN_DList_AddPrev(SVN_DLIST_HEAD_S *pstNew, SVN_DLIST_HEAD_S *pstHead);

 VOID SVN_DList_DelInit(SVN_DLIST_HEAD_S *pstEntry);
 VOID SVN_DList_DelSafe(SVN_DLIST_HEAD_S *pstEntry);
 LONG SVN_DList_Empty(const SVN_DLIST_HEAD_S *pstHead);
 LONG SVN_DList_IsLinked(const SVN_DLIST_HEAD_S *pstHead);
#endif
 SVN_DLIST_HEAD_S* SVN_DList_Remove(SVN_DLIST_HEAD_S *pstHead);
 SVN_DLIST_HEAD_S* SVN_DList_RemoveTail(SVN_DLIST_HEAD_S *pstHead);


 LONG SVN_DList_IsLast(const SVN_DLIST_HEAD_S *pstList, const SVN_DLIST_HEAD_S *pstHead);
 LONG SVN_DList_IsSingular(const SVN_DLIST_HEAD_S *pstHead);
 LONG SVN_DList_IsBeyondSingular(const SVN_DLIST_HEAD_S *pstHead);


 VOID SVN_DList_SpliceInit(SVN_DLIST_HEAD_S *pstList, SVN_DLIST_HEAD_S *pstHead);

VOID *SVN_DList_GetNext(VOID *pCure,VOID **ppNext);


#ifdef __cplusplus
}
#endif

#endif /* __MDM_LIST_H__ */
