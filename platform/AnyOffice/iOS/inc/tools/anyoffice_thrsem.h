/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : thrsem.h
  版 本 号   : 初稿
  作    者   : 刘国清 42408
  生成日期   : 2005年7月13日
  最近修改   :
  功能描述   : 信号量管理头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2005年7月13日
    作    者   : 刘国清 42408
    修改内容   : 创建文件

******************************************************************************/

#ifndef __THRSEM_H__
#define __THRSEM_H__

#include <semaphore.h>
#include <pthread.h>
#include <errno.h>


extern int pthread_mutexattr_settype (pthread_mutexattr_t *pstattr, int __kind);
/*
    for mutex lock;
*/
#define    mutex_t               pthread_mutex_t
#define    mutex_attr_t          pthread_mutexattr_t

#ifdef ANYOFFICE_IPHONE
#define    mutex_init(pmt)        {mutex_attr_t attr; \
                                    (void)pthread_mutexattr_init(&attr);\
                                    (void)pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);\
                                    (void)pthread_mutex_init((pmt), &attr);\
}
#else
/*#define    mutex_init(pmt)       pthread_mutex_init(pmt, NULL)*/
#define    mutex_init(pmt)        {mutex_attr_t attr; \
                                    (void)pthread_mutexattr_init(&attr);\
                                    (void)pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);\
                                    (void)pthread_mutex_init((pmt), &attr);\
                                    }
#endif

/*#define    mutex_lock(pmt)       pthread_mutex_lock(pmt)*/
#define    mutex_lock(pmt)       {if (EDEADLK == pthread_mutex_lock(pmt))\
                                  {\
                                    exit(0);\
                                  }\
                                 }
#define    mutex_unlock(pmt)     pthread_mutex_unlock(pmt)
#define    mutex_destroy(pmt)    pthread_mutex_destroy(pmt)
#define    mutex_trylock(pmt)    pthread_mutex_trylock(pmt) /*Add by zhangzuotao 44842 2006-11-28*/

/*
    for count semphore
*/
#define    thrsem_t                 sem_t

#define    thrsem_init(psem, val)   sem_init((psem), 0, (val))
#define    thrsem_dec(psem)         sem_wait(psem)
#define    thrsem_trydec(psem)      sem_trywait(psem)
#define    thrsem_inc(psem)         sem_post(psem)
#define    thrsem_value(psem, pval) sem_getvalue((psem), (pval))
#define    thrsem_destroy(psem)     sem_destroy(psem)

/*share lock and exclusive lock*/
typedef struct tag_rwlock_s
{
    mutex_t   stmt_write;       /*write lock*/
    mutex_t   stmt_read;        /*read lock*/
    int       count;            /*count read lock*/

    mutex_t   stmt_s;           /*share lock, mutex rwlock_s*/
}rwlock_t;

#define rwlock_init(rwlock)         {\
                                        mutex_init(&(rwlock)->mt_write);\
                                        mutex_init(&(rwlock)->mt_read);\
                                        mutex_init(&(rwlock)->mt_s);\
                                        (rwlock)->count = 0;\
                                    }

#define rwlock_read_lock(rwlock)    {\
                                        mutex_lock(&(rwlock)->mt_s);\
                                        if (0 == (rwlock)->count)\
                                        {\
                                            mutex_lock(&(rwlock)->mt_read);\
                                        }\
                                        (rwlock)->count ++;\
                                        mutex_unlock(&(rwlock)->mt_s);\
                                    }

#define rwlock_read_unlock(rwlock)  {\
                                        mutex_lock(&(rwlock)->mt_s);\
                                        if (((rwlock)->count > 0) && (0 == --(rwlock)->count))\
                                        {\
                                            mutex_unlock(&(rwlock)->mt_read);\
                                        }\
                                        mutex_unlock(&(rwlock)->mt_s);\
                                    }

#define rwlock_write_lock(rwlock)   {\
                                        mutex_lock(&(rwlock)->mt_read);\
                                        mutex_lock(&(rwlock)->mt_write);\
                                    }

#define rwlock_write_unlock(rwlock) {\
                                        mutex_unlock(&(rwlock)->mt_write);\
                                        mutex_unlock(&(rwlock)->mt_read);\
                                    }

#define rwlock_destroy(rwlock)      {\
                                        mutex_destroy(&(rwlock)->mt_write);\
                                        mutex_destroy(&(rwlock)->mt_read);\
                                        mutex_destroy(&(rwlock)->mt_s);\
                                        (rwlock)->count = 0;\
                                    }
#endif /*__THRSEM_H__*/
