#include "crc32.h"

static uint32_t _crc32_value_calculate(uint16_t in_crc)
{
    uint16_t j;
    uint32_t ulCRC = in_crc;
    const uint32_t CRC32_POLYNOMIAL = 0xEDB88320;

    for (j = 8; j; j--)
    {
        if (ulCRC & 1)
            ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
        else
            ulCRC >>= 1;
    }

    return ulCRC;
}

uint32_t crc32_value_update(uint8_t *buf, int32_t len, uint32_t last_crc)
{
    uint16_t i = 0;
    uint32_t ulTemp1, ulTemp2;
    uint32_t new_crc = last_crc;

    for (i = 0; i < len; i++)
    {
        ulTemp1 = (new_crc >> 8) & 0x00FFFFFFL;
        ulTemp2 = _crc32_value_calculate(((uint16_t)new_crc ^ buf[i]) & 0xFF);
        new_crc   = ulTemp1 ^ ulTemp2;
    }

    return new_crc;
}
