#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* 消息队列句柄 */
typedef struct _message_queue_struct MESSAGE_QUEUE_STRUCT;

/* 创建消息队列 */
MESSAGE_QUEUE_STRUCT *message_queue_create(uint16_t queue_size, uint16_t item_size);

/* 发送消息 */
bool message_queue_send(MESSAGE_QUEUE_STRUCT *pqueue, const void *pitem);

/* 接收消息 */
bool message_queue_receive(MESSAGE_QUEUE_STRUCT *pqueue, void *pitem);

/* 销毁消息队列 */
void message_queue_delete(MESSAGE_QUEUE_STRUCT *pqueue);

#endif // MESSAGE_QUEUE_H
