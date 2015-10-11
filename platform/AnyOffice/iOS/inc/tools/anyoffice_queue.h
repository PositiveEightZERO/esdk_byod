/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : anyoffice_queue.h
  版 本 号   : 初稿
  作    者   : zhangwenhu
  生成日期   : 2013年6月24日
  最近修改   :
  功能描述   : 基于链表的队列实现
  函数列表   :
  修改历史   :
  1.日    期   : 2013年6月24日
    作    者   : zhangwenhu
    修改内容   : 创建文件

******************************************************************************/

#ifndef ANYOFFICE_QUEUE_H__
#define ANYOFFICE_QUEUE_H__

#include <pthread.h>
#include "anyoffice_dlist.h"
#include "tools_common.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef void (*ANYOFFICE_FREE_CB)(void *ptr);
typedef ULONG (*ANYOFFICE_QUEUENODE_OP_CB)(void* argv1/*自定义入参*/, void* argv2/*队列中结点的数据*/);

/*队列节点定义  */
typedef struct tagANYOFFICE_QueueNode
{
    SVN_DLIST_HEAD_S            stNode;         /*双链表节点  */
    VOID*                       pvData;         /*节点数据指针 */
}ANYOFFICE_QUEUENODE_S;

/*定长环状队列定义  */
typedef struct tagANYOFFICE_Queue
{
    ULONG               usNodeCount;        /*队列长度  */    
    pthread_mutex_t     mutexQueue;

    SVN_DLIST_HEAD_S    stDataList;         /*数据队列  */
    SVN_DLIST_HEAD_S    stIdleList;         /*空闲队列  */
} ANYOFFICE_QUEUE_S;

ANYOFFICE_QUEUE_S* ANYOFFICE_Queue_Create();
VOID ANYOFFICE_Queue_Release(ANYOFFICE_QUEUE_S* pstQueue, ANYOFFICE_FREE_CB cbFree);

LONG ANYOFFICE_Queue_Push(ANYOFFICE_QUEUE_S* pstQueue, VOID* pvData);
LONG ANYOFFICE_Queue_PushToHead(ANYOFFICE_QUEUE_S* pstQueue, VOID* pvData);
VOID* ANYOFFICE_Queue_Pop(ANYOFFICE_QUEUE_S* pstQueue);
VOID* ANYOFFICE_Queue_Peek(ANYOFFICE_QUEUE_S* pstQueue);
LONG ANYOFFICE_Queue_IsEmpty(ANYOFFICE_QUEUE_S* pstQueue);
LONG ANYOFFICE_Queue_Clear(ANYOFFICE_QUEUE_S* pstQueue, ANYOFFICE_FREE_CB cbFree);
LONG ANYOFFICE_Queue_Walk(ANYOFFICE_QUEUE_S* pstQueue, ANYOFFICE_QUEUENODE_OP_CB cbQueueNodeOP, VOID* pvData);
LONG ANYOFFICE_Queue_ItemCount(ANYOFFICE_QUEUE_S* pstQueue);

#ifdef  __cplusplus
}
#endif

#endif
