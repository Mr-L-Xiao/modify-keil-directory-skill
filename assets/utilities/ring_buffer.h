#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#ifndef __IO
#define     __IO    volatile
#endif

#include <stdint.h>

typedef struct _ring_buffer_struct
{
    unsigned char   *buffer;  /*the buffer holding the data*/
    unsigned int    size;    /*the size(power of 2) of the allocated buffer*/
    __IO unsigned int    in;    /*data is added at offset(in % size)*/
    __IO unsigned int    out;       /*data is extracted from offset(out % size)*/
}RING_BUFFER_T;

volatile RING_BUFFER_T * ring_buffer_init(unsigned int size);
unsigned int ring_buffer_add(volatile RING_BUFFER_T *ring_buffer, unsigned char *buffer, unsigned int len);
unsigned int ring_buffer_get(volatile RING_BUFFER_T *ring_buffer, unsigned char *buffer, unsigned int len);
unsigned int ring_buffer_delete(volatile RING_BUFFER_T *ring_buffer, unsigned int len);
int ring_buffer_release(volatile RING_BUFFER_T *ring_buffer);
int ring_buffer_clean(volatile RING_BUFFER_T *ring_buffer);
int ring_buffer_is_empty(volatile RING_BUFFER_T *ring_buffer);
unsigned int ring_buffer_get_with_del(volatile RING_BUFFER_T *ring_buffer, unsigned char *buffer, unsigned int len);
int ring_buffer_size_get(volatile RING_BUFFER_T *ring_buffer);

#endif