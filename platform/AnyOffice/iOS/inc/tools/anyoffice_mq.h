/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : mq.h
  版 本 号   : 初稿
  作    者   : 刘国清 42408
  生成日期   : 2005年7月13日
  最近修改   :
  功能描述   : 消息队列管理头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2005年7月13日
    作    者   : 刘国清 42408
    修改内容   : 创建文件

******************************************************************************/

#ifndef __ANYOFFICE_MQ_H__
#define __ANYOFFICE_MQ_H__


#include <pthread.h>
//#include "svn_define.h"
//#include "secentry/common/secentry_pub.h"
#include "tools_common.h"
#include "anyoffice_thrsem.h"
#define     MQ_WAIT_FOREVER     0
#define     MQ_NO_WAIT          1

/*
    message queue struct;
*/
typedef struct tag_mq_s
{
    mutex_t   stmt;         /*队列访问锁*/
    thrsem_t  stsem;        /*计数器信号量、用来保存队列中有几个单元*/

    USHORT    b;            /*队列第一个单元的位置*/
    USHORT    e;            /*队列最后一个单元的位置*/
    USHORT    mask;         /*队列掩码*/
    USHORT    unit;         /*队列中每个单元的宽度*/

    UCHAR     aucdata[0];   /*队列元素头地址*/
}mq_t;

#define     mq_dump(pmq)    {   \
                                int  __value__;                   \
                                thrsem_value(&(pmq)->sem, &__value__); \
                                printf("sem_value:%d, b:%d, e:%d, mask:%d, unit:%d!\n",  \
                                    __value__, (pmq)->b, (pmq)->e, (pmq)->mask, (pmq)->unit);    \
                            }

/*
    for thread message queue;
*/
extern INT32    mq_init(mq_t *pstmq, USHORT len, USHORT unit);
extern mq_t*    mq_create(USHORT len, USHORT unit);
extern INT32    mq_destroy(mq_t *pstmq);
extern INT32    mq_put(mq_t *pstmq, UCHAR *e);
extern INT32    mq_get(mq_t *pstmq, UCHAR *e, INT32 wait);

#endif /*__MQ_H__*/
