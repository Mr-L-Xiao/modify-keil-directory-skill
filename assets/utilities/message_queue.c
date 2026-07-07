#include "message_queue.h"

// 消息队列结构体
struct _message_queue_struct
{
    uint8_t *buffer;       // 消息缓冲区
    uint16_t queue_size;   // 队列容量（最大消息数量）
    uint16_t item_size;    // 每个消息的大小
    uint16_t head;         // 队头指针
    uint16_t tail;         // 队尾指针
    uint16_t count;        // 当前消息数量
};

/* 创建消息队列 */
MESSAGE_QUEUE_STRUCT *message_queue_create(uint16_t queue_size, uint16_t item_size)
{
    MESSAGE_QUEUE_STRUCT *pqueue = {0};

    /* 分配消息队列结构体 */
    pqueue = (MESSAGE_QUEUE_STRUCT *)malloc(sizeof(MESSAGE_QUEUE_STRUCT));
    if (pqueue == NULL)
    {
        return NULL;  // 分配失败
    }

    /* 分配消息缓冲区 */
    pqueue->buffer = (uint8_t *)malloc(queue_size * item_size);
    if (NULL == pqueue->buffer)
    {
        free(pqueue);  // 释放队列结构体

        return NULL;  // 分配失败
    }

    /* 初始化队列属性 */
    pqueue->queue_size = queue_size;
    pqueue->item_size = item_size;
    pqueue->head = 0;
    pqueue->tail = 0;
    pqueue->count = 0;

    return pqueue;
}

/* 发送消息 */
bool message_queue_send(MESSAGE_QUEUE_STRUCT *pqueue, const void *pitem)
{
    if ((NULL == pqueue) || (NULL == pitem))
    {
        return false;
    }

    /* 检查队列是否已满 */
    if (pqueue->count == pqueue->queue_size)
    {
        return false;  // 队列已满，发送失败
    }

    /* 将消息复制到队尾 */
    memcpy(&pqueue->buffer[pqueue->tail * pqueue->item_size], pitem, pqueue->item_size);

    /* 更新队尾指针 */
    pqueue->tail = (pqueue->tail + 1) % pqueue->queue_size;

    /* 增加消息数量 */
    pqueue->count++;

    return true;  // 发送成功
}

/* 接收消息 */
bool message_queue_receive(MESSAGE_QUEUE_STRUCT *pqueue, void *pitem)
{
    if ((NULL == pqueue) || (NULL == pitem))
    {
        return false;
    }

    /* 检查队列是否为空 */
    if (0 == pqueue->count)
    {
        return false;  // 队列为空，接收失败
    }

    /* 从队头取出消息 */
    memcpy(pitem, &pqueue->buffer[pqueue->head * pqueue->item_size], pqueue->item_size);

    /* 更新队头指针 */
    pqueue->head = (pqueue->head + 1) % pqueue->queue_size;

    /* 减少消息数量 */
    pqueue->count--;

    return true;  // 接收成功
}

/* 销毁消息队列 */
void message_queue_delete(MESSAGE_QUEUE_STRUCT *pqueue)
{
    if (pqueue != NULL)
    {
        if (pqueue->buffer != NULL)
        {
            free(pqueue->buffer);  // 释放消息缓冲区
        }
        free(pqueue);  // 释放队列结构体
    }
}
