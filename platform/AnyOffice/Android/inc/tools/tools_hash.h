/******************************************************************************

                  版权所有 (C), 2001-2013, 华为技术有限公司

 ******************************************************************************
  文 件 名   : tools_hash.h
  版 本 号   : 初稿
  作    者   : lifangxiang/00218420
  生成日期   : 2013年11月4日
  最近修改   :
  功能描述   : hash表实现，移植自libetpan/chash
  函数列表   :
              Tools_API_Hash_GetCount
              Tools_API_Hash_GetKey
              Tools_API_Hash_GetSize
              Tools_API_Hash_GetValue
  修改历史   :
  1.日    期   : 2013年11月4日
    作    者   : lifangxiang/00218420
    修改内容   : 创建文件

******************************************************************************/
#ifndef __TOOLS_HASH_H__
#define __TOOLS_HASH_H__

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include "tools_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#ifndef CHASH_COPYNONE
#define CHASH_COPYNONE 0
#endif

#ifndef CHASH_COPYKEY
#define CHASH_COPYKEY 1
#endif

#ifndef CHASH_COPYVALUE
#define CHASH_COPYVALUE 2
#endif

#ifndef CHASH_COPYALL
#define CHASH_COPYALL (CHASH_COPYKEY | CHASH_COPYVALUE)
#endif

#ifndef CHASH_DEFAULTSIZE
#define CHASH_DEFAULTSIZE 13
#endif
/*----------------------------------------------*
 * 外部结构定义                                 *
 *----------------------------------------------*/
typedef struct tagHashDatum
{
    VOID *       pvData;
    UINT32       uiLen;
} HASH_DATUM_S;

typedef struct tagHashCell
{
    UINT32       uiFunc;
    HASH_DATUM_S stKey;
    HASH_DATUM_S stVal;
    struct tagHashCell* pstNext;
} HASH_CELL_S;

typedef struct tagHash
{
    UINT32        uiSize;
    UINT32        uiCount;
    INT32         iCopyValue;
    INT32         iCopyKey;
    HASH_CELL_S** pstCells;
}HASH_S;

typedef HASH_CELL_S HASH_ITER_S;


/*----------------------------------------------*
 * 函数声明                                     *
 *----------------------------------------------*/

 
/* Allocates a new (empty) hash using this initial size and the given flags,
   specifying which data should be copied in the hash.
    CHASH_COPYNONE  : Keys/Values are not copied.
    CHASH_COPYKEY   : Keys are dupped and freed as needed in the hash.
    CHASH_COPYVALUE : Values are dupped and freed as needed in the hash.
    CHASH_COPYALL   : Both keys and values are dupped in the hash.
 */
HASH_S *				Tools_API_Hash_New(UINT32 uiSize, INT32 uiFlags);

/* Frees a hash */
VOID					Tools_API_Hash_Free(HASH_S * pstHash);

/* Frees a hash safely */
VOID Tools_API_Hash_FreeSafe(HASH_S *pstHash);

/* Removes all elements from a hash */
VOID					Tools_API_Hash_Clear(HASH_S * pstHash);

/* Adds an entry in the hash table.
   Length can be 0 if key/value are strings.
   If an entry already exists for this key, it is replaced, and its value
   is returned. Otherwise, the data pointer will be NULL and the length
   field be set to TRUE or FALSe to indicate success or failure. */
INT32					Tools_API_Hash_Set(HASH_S *                    pstHash,
                                           HASH_DATUM_S *              pstKey,
                                           HASH_DATUM_S *              pstValue,
                                           HASH_DATUM_S *              pstOldvalue);

/* Retrieves the data associated to the key if it is found in the hash table.
   The data pointer and the length will be NULL if not found*/
INT32					Tools_API_Hash_Get(HASH_S * pstHash,
                                           HASH_DATUM_S * pstKey, HASH_DATUM_S * pstResult);

/* Removes the entry associated to this key if it is found in the hash table,
   and returns its contents if not dupped (otherwise, poINT32er will be NULL
   and len TRUE). If entry is not found both pointer and len will be NULL. */
INT32					Tools_API_Hash_Delete(HASH_S *                    pstHash,
                                              HASH_DATUM_S *              pstKey,
                                              HASH_DATUM_S *              pstOldvalue);

/* Resizes the hash table to the passed size. */
INT32					Tools_API_Hash_Resize(HASH_S * pstHash, UINT32 uiSize);

/* Returns an iterator to the first non-empty entry of the hash table */
HASH_ITER_S *			Tools_API_Hash_Begin(const HASH_S * pstHash);

/* Returns the next non-empty entry of the hash table */
HASH_ITER_S *			Tools_API_Hash_Next(const HASH_S * pstHash, HASH_ITER_S * pstIter);

long Tools_API_Hash_AddKeyValue(HASH_S *pstHash, const char *pcName, const char* pcValue);

long Tools_API_Hash_AddKeyNum(HASH_S *pstHash, const char *pcName, unsigned long ulNum);

long Tools_API_Hash_IsExist(HASH_S *pstHash, VOID *pvKey, unsigned long ulKeyLen);

long Tools_API_Hash_DeleteKey(HASH_S *pstHash, VOID *pvKey, unsigned long ulKeyLen);

HASH_S *Tools_API_Hash_Dump(HASH_S *pstHash);

/* Some of the following routines can be implemented as macros to
   be faster. If you don't want it, define NO_MACROS */
#ifdef NO_MACROS

/* Returns the size of the hash table */
UINT32					Tools_API_Hash_GetSize(HASH_S * pstHash);

/* Returns the number of entries in the hash table */
UINT32					Tools_API_Hash_GetCount(HASH_S * pstHash);

/* Returns the key part of the entry pointed by the iterator */
VOID					Tools_API_Hash_GetKey(HASH_ITER_S * pstIter, HASH_DATUM_S * pstResult);

/* Returns the value part of the entry pointed by the iterator */
VOID					Tools_API_Hash_GetValue(HASH_ITER_S * pstIter, HASH_DATUM_S * pstResult);

#else

#ifndef INLINE
#   ifdef _MSC_VER
#       define INLINE __inline
#   else
#       define INLINE inline
#   endif
#endif

static INLINE UINT32 Tools_API_Hash_GetSize(HASH_S * pstHash)
{
    return pstHash->uiSize;
}

static INLINE UINT32 Tools_API_Hash_GetCount(HASH_S * pstHash)
{
    return pstHash->uiCount;
}

static INLINE VOID Tools_API_Hash_GetKey(HASH_ITER_S * pstIter, HASH_DATUM_S * pstResult)
{
    *pstResult = pstIter->stKey;
}

static INLINE VOID Tools_API_Hash_GetValue(HASH_ITER_S * pstIter, HASH_DATUM_S * pstResult)
{
    *pstResult = pstIter->stVal;
}

#endif

#ifdef __cplusplus
}
#endif

#endif
