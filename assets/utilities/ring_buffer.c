#include "ring_buffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief	将输入值向上取整为2的幂，提高运算速度
 * @param	n - 用户期望的缓冲区字节数
 * @return	返回2的幂
 */
static uint32_t _align_power_of_2(uint32_t n)
{
    const uint32_t MAX_SIZE = 0x1000;	// 限制最大尺寸为2^12=4*4096字节

    if (n == 0) return 0;          		// 或 return 1
    if (n > MAX_SIZE) return MAX_SIZE;

    /* 经典向上取整 */
    uint32_t r = 1;
    while (r < n)
        r <<= 1;
    return r;
}

/**
 * @brief	初始化
 * @param	size - 用户期望的缓冲区字节数
 * @return	返回初始化后的环形缓冲区控制块
 */
volatile RING_BUFFER_T * ring_buffer_init(uint32_t size)
{
	/* 为控制块分配内存 */
    RING_BUFFER_T *ring_buffer = NULL;
    ring_buffer = (RING_BUFFER_T *)malloc(sizeof(RING_BUFFER_T));
    if (!ring_buffer)
    {
        return NULL;
    }
	/* 为真正缓冲区分配内存 */
    size = _align_power_of_2(size);
    ring_buffer->buffer = (uint8_t *)malloc(size);
    if (!ring_buffer->buffer)
    {
        free(ring_buffer);
        return NULL;
    }
	/* 控制块成员初始化 */
    ring_buffer->size = size;
    ring_buffer->in = 0;
    ring_buffer->out = 0;

    return ring_buffer;
}

/**
 * @brief	向环形缓冲区写入数据
 * @param	ring_buffer - 指向环形缓冲区控制块的指针
			buffer - 指向要写入的数据源数组的指针
			len - 请求写入的字节数
 * @return	实际写入的字节数
 */
uint32_t ring_buffer_add(volatile RING_BUFFER_T *ring_buffer, uint8_t *buffer, uint32_t len)
{
    uint32_t free_size = 0;  /*free buffer size*/
    uint32_t used_size = 0;  /*used buffer size*/
    uint32_t len_to_end = 0; /*length to buffer end*/
    uint32_t in_pos = 0;

    if (!ring_buffer || !buffer || len == 0)
    {
        return 0;
    }

	/* 计算当前空闲空间 */
    used_size = ring_buffer->in - ring_buffer->out;
    free_size = ring_buffer->size - used_size;
    len = len < free_size ? len : free_size;	// 防止写入超出空闲空间, 若超出则舍弃超出的部分

    /* 计算写入起始物理位置 */
    in_pos = ring_buffer->in & (ring_buffer->size - 1);
	
    /* 计算是否需要回绕写入 */
    len_to_end = ring_buffer->size - in_pos;
    len_to_end = len_to_end < len ? len_to_end : len;

    /* 从 in_pos 开始拷贝第一段数据 */
    memcpy(ring_buffer->buffer + in_pos, buffer, len_to_end);
    /* 如果还有剩余数据，从缓冲区头部（地址 0）继续拷贝 */
    memcpy(ring_buffer->buffer, buffer + len_to_end, len - len_to_end);
    ring_buffer->in += len;

    return len;
}

/**
 * @brief	从环形缓冲区中读取数据，但并不移除数据
 * @param	ring_buffer - 指向环形缓冲区控制块的指针
			buffer - 指向存放读出数据的目标数组的指针
			len - 请求读取的字节数
 * @return	len - 实际读取的字节数
 */
uint32_t ring_buffer_get(volatile RING_BUFFER_T *ring_buffer, uint8_t *buffer, uint32_t len)
{
    uint32_t used_size = 0; /*used buffer size*/
    uint32_t len_to_end = 0; /*length to buffer end*/
    uint32_t out_pos = 0;

    if (!ring_buffer || !buffer || len == 0)
    {
        return 0;
    }

	/* 计算已用的数据量，限制读取长度 */
    used_size = ring_buffer->in - ring_buffer->out;
    len = len < used_size ? len : used_size;

	/* 计算读取起始物理位置 */
    out_pos = ring_buffer->out & (ring_buffer->size - 1);
	
	/* 计算是否需要回绕读取 */
    len_to_end = ring_buffer->size - out_pos;
    len_to_end = len_to_end < len ? len_to_end : len;

    /*first get the data starting from ring_buffer->out to buffer end*/
    memcpy(buffer, ring_buffer->buffer + out_pos, len_to_end);
    /*then get the rest(if any) at the beginning of the buffer*/
    memcpy(buffer + len_to_end, ring_buffer->buffer, len - len_to_end);
    return len;
}

/**
 * @brief	判断环形缓冲区是否为空
 * @param	ring_buffer - 指向环形缓冲区控制块的指针
 * @return	-1：缓冲区无效，0：不为空，1：为空
 */
int32_t ring_buffer_is_empty(volatile RING_BUFFER_T *ring_buffer)
{
    if (!ring_buffer)
        return -1;                     // 错误
    return (ring_buffer->in == ring_buffer->out) ? 1 : 0;  // 1 表示为空
}

/**
 * @brief	从环形缓冲区中读取数据，同时移除数据
 * @param	ring_buffer - 指向环形缓冲区控制块的指针
			buffer - 指向存放读出数据的目标数组的指针
			len - 请求读取的字节数
 * @return	len - 实际读取的字节数
 * @note	和ring_buffer_get()的区别只在于是否移动out
 */
uint32_t ring_buffer_get_with_del(volatile RING_BUFFER_T *ring_buffer, uint8_t *buffer, uint32_t len)
{
    uint32_t used_size = 0; 	/*used buffer size*/
    uint32_t len_to_end = 0; 	/*length to buffer end*/
    uint32_t out_pos = 0;

    if (!ring_buffer || !buffer || len == 0)
    {
        return 0;
    }

    used_size = ring_buffer->in - ring_buffer->out;
    len = len < used_size ? len : used_size;

    out_pos = ring_buffer->out & (ring_buffer->size - 1);
    len_to_end = ring_buffer->size - out_pos;
    len_to_end = len_to_end < len ? len_to_end : len;

    /*first get the data starting from ring_buffer->out to buffer end*/
    memcpy(buffer, ring_buffer->buffer + out_pos, len_to_end);
    /*then get the rest(if any) at the beginning of the buffer*/
    memcpy(buffer + len_to_end, ring_buffer->buffer, len - len_to_end);

	/* 将out右移len,等效于删除数据 */
    ring_buffer->out += len;

    return len;
}

/**
 * @brief	从环形缓冲区中丢弃数据
 * @param	ring_buffer - 指向环形缓冲区控制块的指针
			len - 请求丢弃的字节数
 * @return	len - 实际丢弃的字节数
 * @note	从out位置开始丢弃
 */
uint32_t ring_buffer_delete(volatile RING_BUFFER_T *ring_buffer, uint32_t len)
{
    uint32_t used_size = 0; /*used buffer size*/

    if (!ring_buffer)
    {
        return 0;
    }

    used_size = ring_buffer->in - ring_buffer->out;
    len = len < used_size ? len : used_size;	// 限制丢弃长度不会超过已用长度
    ring_buffer->out += len;

    return len;
}

/**
 * @brief	清空环形缓冲区中的所有数据
 * @param	ring_buffer - 指向环形缓冲区控制块的指针
 * @return	-1：缓冲区无效，0：清空成功
 * @note	并不清空真实数据，只将in和out置0，等效于清空
 */
int32_t ring_buffer_clean(volatile RING_BUFFER_T *ring_buffer)
{
    if (!ring_buffer)
    {
        return -1;
    }

    ring_buffer->in = 0;
    ring_buffer->out = 0;

    return 0;
}

/**
 * @brief	完全释放环形缓冲区
 * @param	pp_ring_buffer - 指向环形缓冲区控制块的指针的指针（二级指针）
 * @return	-1：缓冲区无效，0：清空成功
 * @note	真实的将缓冲区的内存释放掉，并且将原指针置NULL
 */
int32_t ring_buffer_release(volatile RING_BUFFER_T **pp_ring_buffer)
{
    if (!pp_ring_buffer || !*pp_ring_buffer)
        return -1;
    RING_BUFFER_T *rb = (RING_BUFFER_T *)*pp_ring_buffer; // 去掉 volatile
    if (rb->buffer) {
        free(rb->buffer);
        rb->buffer = NULL;
    }
    free(rb);
    *pp_ring_buffer = NULL;   // 修改调用者的指针
    return 0;
}

/**
 * @brief	获取已用空间，只读函数，避免直接访问in和out
 * @param	ring_buffer - 指向环形缓冲区控制块的指针
 * @return	已用空间
 */
uint32_t ring_buffer_used(volatile RING_BUFFER_T *ring_buffer)
{
    if (!ring_buffer)
        return 0;   // 或通过额外参数返回错误
    return ring_buffer->in - ring_buffer->out;
}
