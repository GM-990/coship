#ifndef _CRC_H_
#define _CRC_H_
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
CSUSPCRC16
  ����������ݶε�CRC16ֵ

Parameters	
        pvStartAddress, ���ݶε���ʼ��ַ
        dwSizeInBytes,���ݶεĳ���

Return Values
	���ݵ�CRC16ֵ
*/
WORD CSUSPCRC16(PVOID pvStartAddress, DWORD dwSizeInBytes);

/*
CSUSPCRC16Add
  ������CRC16ֵ�Ļ�������������������ݶε�CRC16ֵ

Parameters	
        pvStartAddress, ���ݶε���ʼ��ַ
        dwSizeInBytes,���ݶεĳ���
        wOrgCrc16, ���е�CRC16ֵ

Return Values
	���ݵ�CRC16ֵ
*/
WORD CSUSPCRC16Add(PVOID pvStartAddress, DWORD dwSizeInBytes,WORD wOrgCrc16);//0x0

/*
CSUSPCRC16
  ����������ݶε�CRC32ֵ

Parameters	
        pvStartAddress, ���ݶε���ʼ��ַ
        dwSizeInBytes,���ݶεĳ���

Return Values
	���ݵ�CRC32ֵ
*/
DWORD CSUSPCRC32(PVOID pvStartAddress, DWORD dwSizeInBytes);

DWORD CSUSPMPEGCRC32(PVOID pvStartAddress, DWORD dwSizeInBytes);

DWORD CSUSPMPEGCRC32Add(PVOID pvStartAddress, DWORD dwSizeInBytes,DWORD dwOrgCrc32);//0xffffff

#ifdef __cplusplus
}
#endif

#endif

