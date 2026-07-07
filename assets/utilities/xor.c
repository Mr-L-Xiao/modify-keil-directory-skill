#include "xor.h"
#include "stdio.h"

int32_t xor_value_calculate(uint8_t *pbuf, uint8_t len, uint8_t *pxor_val)
{
    uint8_t xor_val = 0x00;
    int32_t i = 0;

    if ((NULL == pbuf) || (NULL == pxor_val))
    {
        return -1;
    }

    for (i = 0; i < len; i++)
    {
        xor_val ^= pbuf[i];
    }

    *pxor_val = xor_val;

    return 0;
}
