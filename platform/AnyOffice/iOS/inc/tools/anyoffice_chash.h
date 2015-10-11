/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : anyoffice_pub.c
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2013年6月17日
  最近修改   :
  功能描述   : anyoffice客户端公共函数模块，与业务无关
  函数列表   :

  修改历史   :
  1.日    期   : 2013年6月17日
    作    者   : 
    修改内容   : 创建文件

******************************************************************************/
#ifndef CHASH_H
#define CHASH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  void * data;
  unsigned int len;
} chashdatum;

struct chash {
  unsigned int size;
  unsigned int count;
  int copyvalue;
  int copykey;
  struct chashcell ** cells; 
};

typedef struct chash chash;

struct chashcell {
  unsigned int func;
  chashdatum key;
  chashdatum value;
  struct chashcell * next;
};

typedef struct chashcell chashiter;

#define CHASH_COPYNONE    0
#define CHASH_COPYKEY     1
#define CHASH_COPYVALUE   2
#define CHASH_COPYALL     (CHASH_COPYKEY | CHASH_COPYVALUE)

#define CHASH_DEFAULTSIZE 13
  
/* Allocates a new (empty) hash using this initial size and the given flags,
   specifying which data should be copied in the hash.
    CHASH_COPYNONE  : Keys/Values are not copied.
    CHASH_COPYKEY   : Keys are dupped and freed as needed in the hash.
    CHASH_COPYVALUE : Values are dupped and freed as needed in the hash.
    CHASH_COPYALL   : Both keys and values are dupped in the hash.
 */

chash * chash_new(unsigned int size, int flags);

/* Frees a hash */

void chash_free(chash * hash);

/* Removes all elements from a hash */

void chash_clear(chash * hash);

/* Adds an entry in the hash table.
   Length can be 0 if key/value are strings.
   If an entry already exists for this key, it is replaced, and its value
   is returned. Otherwise, the data pointer will be NULL and the length
   field be set to TRUE or FALSe to indicate success or failure. */

int chash_set(chash * hash,
	      chashdatum * key,
	      chashdatum * value,
	      chashdatum * oldvalue);

/* Retrieves the data associated to the key if it is found in the hash table.
   The data pointer and the length will be NULL if not found*/

int chash_get(chash * hash,
	      chashdatum * key, chashdatum * result);

/* Removes the entry associated to this key if it is found in the hash table,
   and returns its contents if not dupped (otherwise, pointer will be NULL
   and len TRUE). If entry is not found both pointer and len will be NULL. */

int chash_delete(chash * hash,
		 chashdatum * key,
		 chashdatum * oldvalue);

/* Resizes the hash table to the passed size. */

int chash_resize(chash * hash, unsigned int size);

/* Returns an iterator to the first non-empty entry of the hash table */

chashiter * chash_begin(chash * hash);

/* Returns the next non-empty entry of the hash table */

chashiter * chash_next(chash * hash, chashiter * iter);

/* Some of the following routines can be implemented as macros to
   be faster. If you don't want it, define NO_MACROS */
#ifdef NO_MACROS
/* Returns the size of the hash table */

unsigned int          chash_size(chash * hash);

/* Returns the number of entries in the hash table */

unsigned int          chash_count(chash * hash);

/* Returns the key part of the entry pointed by the iterator */

void chash_key(chashiter * iter, chashdatum * result);

/* Returns the value part of the entry pointed by the iterator */

void chash_value(chashiter * iter, chashdatum * result);

#else

#ifndef INLINE
#ifdef _MSC_VER
#define INLINE __inline
#else
#define INLINE inline
#endif
#endif

static INLINE unsigned int chash_size(chash * hash)
{
  return hash->size;
}

static INLINE unsigned int chash_count(chash * hash)
{
  return hash->count;
}

static INLINE void chash_key(chashiter * iter, chashdatum * result)
{
  * result = iter->key;
}

static INLINE void chash_value(chashiter * iter, chashdatum * result)
{
  * result = iter->value;
}

#endif

#ifdef __cplusplus
}
#endif

#endif

