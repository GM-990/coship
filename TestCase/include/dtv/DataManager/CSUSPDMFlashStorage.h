#ifndef FlashStorageInterface_H
#define FlashStorageInterface_H


#include "CSUSPDMCommonType.h"

#ifdef __cplusplus
extern "C"
{
#endif

//��������ݹ����Flash�ĳ�ʼ��
HCSHANDLE CSUSPDMCreateFlashMediaEx( 
									HCSHANDLE hMemoryPool, 
									DATACONTAINERPAR * pFlashBlockDATACONTAINERPAR, 
									int nFlashBlockDATACONTAINERPARCount, 
									DATACONTAINERPAR * pFlashObjectDATACONTAINERPAR, 
									int nFlashObjectDATACONTAINERPARCount
									);

//��FlashMedia�л�ȡָ�����ͣ���;����Flash����
HCSHANDLE CSUSPDMGetFlashStorageBlock( int nFlashBlockType, HCSHANDLE hFlashMedia );

//�趨��Flash��ģ���EEPROM�Ĵ�С
BOOL CSUSPDMSetEEPROMSize( int nSize, HCSHANDLE hFLSForEEPROM );

//����ѭ��дFlash���������Flashģ��EEPROM��
//hMemoryPool: ���Flashʵ�����õ���MemoryPool
//dwFLSAddr: Flash�׵�ַ(�����һ��Bolck��Ŀ�ʼ)
//nFLSLen:Flash�ĳ���(�����һ��Block�ĳ���)
//nEEEPROMSize:ѭ��д��Ҳ����ģ���EEPROM���Ĵ�С��ϣ����nFLSLen���ȵ�1/500���£�Ϊ�����ܣ�
HCSHANDLE CSUSPDMCreateCycWirtableFlash( HCSHANDLE hMemoryPool, DWORD dwFLSAddr, int nFLSLen, int nEEPROMSize );

//�ͷ���CSUSPDMCreateFlashMedia�����ľ��
BOOL CSUSPDMFreeFlashObject( HCSHANDLE hData );

//�� hStorageHandle ��д��һ��BYTE(һ���ֽ�)
BOOL CSUSPDMWriteBYTEFlash( BYTE bLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//�� hStorageHandle �ж���һ��BYTE(һ���ֽ�)
BOOL CSUSPDMReadBYTEFromFlash( BYTE * pbLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//�� hStorageHandle ��д��һ��WORD(�����ֽ�)
BOOL CSUSPDMWriteWORDFlash( WORD wLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//�� hStorageHandle �ж���һ��WORD(�����ֽ�)
BOOL CSUSPDMReadWORDFromFlash( WORD * pwLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//�� hStorageHandle ��д��һ��DWORD(�ĸ��ֽ�)
BOOL CSUSPDMWriteDWORDFlash( DWORD dwLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//�� hStorageHandle �ж���һ��DWORD(�ĸ��ֽ�)
BOOL CSUSPDMReadDWORDFromFlash( DWORD * pdwLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );

//�� hStorageHandle ��д��ָ�����ȵ����ݣ��ֽ�Ϊ��λ��
BOOL CSUSPDMWriteFlash( void * pBuf, DWORD dwOffset, int nLen, HCSHANDLE hStorageHandle );
//�� hStorageHandle �ж���ָ�����ȵ����ݣ��ֽ�Ϊ��λ��
BOOL CSUSPDMReadFromFlash( void * pBuf, DWORD dwOffset, int nLen, HCSHANDLE hStorageHandle );

//ѭ��дFlash����
HCSHANDLE CSUSPDMCreateCycBlockWritableFlash( HCSHANDLE hMemoryPool, DWORD dwFLSAddr, int nFLSLen, int nSingleBlockSize, int nAllBlockCount );
BOOL CSUSPDMWriteCycBlockFlash( HCSHANDLE hCycBlockWritableFlash, int nWriteCycBlockIndex, int nWriteOffset, const BYTE * pucWriteBuf, int nWriteLen );
BOOL CSUSPDMReadCycBlockFlash( HCSHANDLE hCycBlockWritableFlash, int nReadCycBlockIndex, int nReadOffset, BYTE * pucOutputBuf, int nReadLen );

BOOL CSUSPDMSetFlashBlockSize(CSUSPDMFlashBlock nBlocksize);

//flash storage for flash for file_system
// the nNum is must set to 2; the pVoid is not used, may be seted NULL.
HCSHANDLE CSUSPDMCreateFileStorageBlock( DATACONTAINERPAR * pDATACONTAINERPAR, int nNum,  void* pVoid );

#ifdef __cplusplus
}
#endif

#endif /* FlashStorageInterface_H */


