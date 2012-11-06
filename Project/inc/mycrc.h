#ifndef _MYCRC_H_
#define _MYCRC_H_

#include "common.h"

typedef u8 crc;

#define POLYNOMIAL 0xD8  /* 11011 followed by 0's */
#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))

crc crcSlow(const u8 message[], u8 nBytes);
void crcInit(void);
crc crcFast(u8 const message[], u8 nBytes);


#endif //_MYCRC_H_
