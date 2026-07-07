#include "ring_buffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static uint32_t _align_power_of_2(uint32_t n)
{
    if ( (n != 0) && (0 == (n & (n - 1))) )
    {
        return n;
    }
    /*limit max buffer size,0x1000 = 4*1024*/
    uint32_t tmp_val = 0x1000;
    while (0 == (tmp_val & n))
    {
        tmp_val = tmp_val >> 1;
    }

    return tmp_val << 1;
}

volatile RING_BUFFER_T * ring_buffer_init(uint32_t size)
{
    RING_BUFFER_T *ring_buffer = NULL;

    ring_buffer = (RING_BUFFER_T *)malloc(sizeof(RING_BUFFER_T));
    if (!ring_buffer)
    {
        return NULL;
    }
    size = _align_power_of_2(size);
    ring_buffer->buffer = (uint8_t *)malloc(size);
    if (!ring_buffer->buffer)
    {
        free(ring_buffer);
        return NULL;
    }
    ring_buffer->size = size;
    ring_buffer->in = 0;
    ring_buffer->out = 0;

    return ring_buffer;
}

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

    used_size = ring_buffer->in - ring_buffer->out;
    free_size = ring_buffer->size - used_size;
    len = len < free_size ? len : free_size;

    in_pos = ring_buffer->in & (ring_buffer->size - 1);
    len_to_end = ring_buffer->size - in_pos;
    len_to_end = len_to_end < len ? len_to_end : len;

    /*first put the data starting from ring_buffer->in to buffer end*/
    memcpy(ring_buffer->buffer + in_pos, buffer, len_to_end);
    /*then put the rest(if any) at the beginning of the buffer*/
    memcpy(ring_buffer->buffer, buffer + len_to_end, len - len_to_end);
    ring_buffer->in += len;

    return len;
}

uint32_t ring_buffer_get(volatile RING_BUFFER_T *ring_buffer, uint8_t *buffer, uint32_t len)
{
    uint32_t used_size = 0; /*used buffer size*/
    uint32_t len_to_end = 0; /*length to buffer end*/
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
    return len;
}

int32_t ring_buffer_is_empty(volatile RING_BUFFER_T *ring_buffer)
{
    if (!ring_buffer)
    {
        return -1;
    }

    if (ring_buffer->in - ring_buffer->out > 0)
    {
        return 0;
    }

    return -1;
}

uint32_t ring_buffer_get_with_del(volatile RING_BUFFER_T *ring_buffer, uint8_t *buffer, uint32_t len)
{
    uint32_t used_size = 0; /*used buffer size*/
    uint32_t len_to_end = 0; /*length to buffer end*/
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

    ring_buffer->out += len;

    return len;
}

uint32_t ring_buffer_delete(volatile RING_BUFFER_T *ring_buffer, uint32_t len)
{
    uint32_t used_size = 0; /*used buffer size*/

    if (!ring_buffer)
    {
        return 0;
    }

    used_size = ring_buffer->in - ring_buffer->out;
    len = len < used_size ? len : used_size;
    ring_buffer->out += len;

    return len;
}

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

int32_t ring_buffer_release(volatile RING_BUFFER_T *ring_buffer)
{
    if (!ring_buffer)
    {
        return -1;
    }
    if (!ring_buffer->buffer)
    {
        return -1;
    }
    free(ring_buffer->buffer);
    ring_buffer->buffer = NULL;
    free((void *)ring_buffer);
    ring_buffer = NULL;

    return 0;
}

int32_t ring_buffer_size_get(volatile RING_BUFFER_T *ring_buffer)
{
    if (!ring_buffer)
    {
        return -1;
    }

    return ring_buffer->in - ring_buffer->out;
}
