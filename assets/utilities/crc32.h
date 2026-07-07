#ifndef _CRC32_H_
#define _CRC32_H_

#include <stdint.h>

uint32_t crc32_value_update(uint8_t *buf, int32_t len, uint32_t last_crc);

#endif // CRC32_H
