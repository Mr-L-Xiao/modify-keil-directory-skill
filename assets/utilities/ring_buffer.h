#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#ifndef __IO
#define __IO    volatile
#endif

#include <stdint.h>

/* 环形缓冲区控制块 */
typedef struct _ring_buffer_struct
{
    uint8_t   *buffer;  	// the buffer holding the data，指向真正缓冲区的指针
    uint32_t    size;    	// the size(power of 2) of the allocated buffer
    __IO uint32_t    in;    // data is added at offset(in % size)
    __IO uint32_t    out;   // data is extracted from offset(out % size)
}RING_BUFFER_T;

volatile RING_BUFFER_T * ring_buffer_init(uint32_t size);
uint32_t ring_buffer_add(volatile RING_BUFFER_T *ring_buffer, uint8_t *buffer, uint32_t len);
uint32_t ring_buffer_get(volatile RING_BUFFER_T *ring_buffer, uint8_t *buffer, uint32_t len);
uint32_t ring_buffer_delete(volatile RING_BUFFER_T *ring_buffer, uint32_t len);
int32_t ring_buffer_release(volatile RING_BUFFER_T **pp_ring_buffer);
int32_t ring_buffer_clean(volatile RING_BUFFER_T *ring_buffer);
int32_t ring_buffer_is_empty(volatile RING_BUFFER_T *ring_buffer);
uint32_t ring_buffer_get_with_del(volatile RING_BUFFER_T *ring_buffer, uint8_t *buffer, uint32_t len);
uint32_t ring_buffer_used(volatile RING_BUFFER_T *ring_buffer);

#endif
