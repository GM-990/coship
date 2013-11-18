#ifndef FlashStorageInterface_H
#define FlashStorageInterface_H


#include "CSUSPDMCommonType.h"

#ifdef __cplusplus
extern "C"
{
#endif

//分配给数据管理的Flash的初始化
HCSHANDLE CSUSPDMCreateFlashMediaEx( 
									HCSHANDLE hMemoryPool, 
									DATACONTAINERPAR * pFlashBlockDATACONTAINERPAR, 
									int nFlashBlockDATACONTAINERPARCount, 
									DATACONTAINERPAR * pFlashObjectDATACONTAINERPAR, 
									int nFlashObjectDATACONTAINERPARCount
									);

//从FlashMedia中获取指定类型（用途）的Flash部分
HCSHANDLE CSUSPDMGetFlashStorageBlock( int nFlashBlockType, HCSHANDLE hFlashMedia );

//设定由Flash所模拟的EEPROM的大小
BOOL CSUSPDMSetEEPROMSize( int nSize, HCSHANDLE hFLSForEEPROM );

//创建循环写Flash句柄（用于Flash模拟EEPROM）
//hMemoryPool: 这个Flash实例所用到的MemoryPool
//dwFLSAddr: Flash首地址(最好是一个Bolck块的开始)
//nFLSLen:Flash的长度(最好是一个Block的长度)
//nEEEPROMSize:循环写（也就是模拟的EEPROM）的大小，希望是nFLSLen长度的1/500以下（为了性能）
HCSHANDLE CSUSPDMCreateCycWirtableFlash( HCSHANDLE hMemoryPool, DWORD dwFLSAddr, int nFLSLen, int nEEPROMSize );

//释放由CSUSPDMCreateFlashMedia创建的句柄
BOOL CSUSPDMFreeFlashObject( HCSHANDLE hData );

//向 hStorageHandle 中写入一个BYTE(一个字节)
BOOL CSUSPDMWriteBYTEFlash( BYTE bLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//从 hStorageHandle 中读出一个BYTE(一个字节)
BOOL CSUSPDMReadBYTEFromFlash( BYTE * pbLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//向 hStorageHandle 中写入一个WORD(两个字节)
BOOL CSUSPDMWriteWORDFlash( WORD wLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//从 hStorageHandle 中读出一个WORD(两个字节)
BOOL CSUSPDMReadWORDFromFlash( WORD * pwLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//向 hStorageHandle 中写入一个DWORD(四个字节)
BOOL CSUSPDMWriteDWORDFlash( DWORD dwLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );
//从 hStorageHandle 中读出一个DWORD(四个字节)
BOOL CSUSPDMReadDWORDFromFlash( DWORD * pdwLhs, DWORD dwOffset, HCSHANDLE hStorageHandle );

//向 hStorageHandle 中写入指定长度的数据（字节为单位）
BOOL CSUSPDMWriteFlash( void * pBuf, DWORD dwOffset, int nLen, HCSHANDLE hStorageHandle );
//从 hStorageHandle 中读出指定长度的数据（字节为单位）
BOOL CSUSPDMReadFromFlash( void * pBuf, DWORD dwOffset, int nLen, HCSHANDLE hStorageHandle );

//循环写Flash操作
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


