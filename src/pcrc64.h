#ifndef CRC64_H
#define CRC64_H

#include <stdint.h>

unsigned long long pcrc64(unsigned long long crc, const unsigned char *s, unsigned long long l);

#ifdef REDIS_TEST
int crc64Test(int argc, char *argv[]);
#endif

#endif
