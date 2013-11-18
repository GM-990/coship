#ifndef _CRC_H_
#define _CRC_H_
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
CSUSPCRC16
  计算给定数据段的CRC16值

Parameters	
        pvStartAddress, 数据段的起始地址
        dwSizeInBytes,数据段的长度

Return Values
	数据的CRC16值
*/
WORD CSUSPCRC16(PVOID pvStartAddress, DWORD dwSizeInBytes);

/*
CSUSPCRC16Add
  在已有CRC16值的基础上增量计算给定数据段的CRC16值

Parameters	
        pvStartAddress, 数据段的起始地址
        dwSizeInBytes,数据段的长度
        wOrgCrc16, 已有的CRC16值

Return Values
	数据的CRC16值
*/
WORD CSUSPCRC16Add(PVOID pvStartAddress, DWORD dwSizeInBytes,WORD wOrgCrc16);//0x0

/*
CSUSPCRC16
  计算给定数据段的CRC32值

Parameters	
        pvStartAddress, 数据段的起始地址
        dwSizeInBytes,数据段的长度

Return Values
	数据的CRC32值
*/
DWORD CSUSPCRC32(PVOID pvStartAddress, DWORD dwSizeInBytes);

DWORD CSUSPMPEGCRC32(PVOID pvStartAddress, DWORD dwSizeInBytes);

DWORD CSUSPMPEGCRC32Add(PVOID pvStartAddress, DWORD dwSizeInBytes,DWORD dwOrgCrc32);//0xffffff

#ifdef __cplusplus
}
#endif

#endif

