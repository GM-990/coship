#ifndef _PERFORMANCE_CRC_H_
#define _PERFORMANCE_CRC_H_
#include "cs_typedef.h"

WORD CSUSPCRC16_UT(PVOID pvStartAddress, DWORD dwSizeInBytes);

DWORD CSUSPCRC32_UT(PVOID pvStartAddress, DWORD dwSizeInBytes);

DWORD CSUSPMPEGCRC32_UT(PVOID pvStartAddress, DWORD dwSizeInBytes);

#endif

