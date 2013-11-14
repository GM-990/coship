/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_flash.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_flash.h"
/**
@brief 从Flash中读取保存的数据 

@param[in] dwStartAddress 存储数据的Flash空间起始地址
@param[out] pucBuf 保存读出数据的缓冲区起始地址
@param[in] uDataLength 要读取的数据长度(以字节为单位)
@return CSUDI_SUCCESS表示读取成功，CSUDI_FAILURE表示读取失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashRead (CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf,  unsigned int uDataLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 往Flash中写入需要保存的数据 ,内建擦除

@param[in] dwStartAddress 存储数据的Flash空间地址
@param[in] pucBuf 要写入数据的缓冲区起始地址
@param[in] uDataLength 要写入的数据长度(以字节为单位)
@return CSUDI_SUCCESS表示写入成功，CSUDI_FAILURE表示写入失败
@note 已经在待机状态下调用本接口仍返回TRUE

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashWrite(CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf, unsigned int uDataLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取Flash分区集合信息
@param[in] eSuitType 需获取的Flash分区集合类型
@param[out] psFlashSuit 指向flash相关信息结构体的指针，详细定义见CSUDIFLASHRegionSuit_S
@return CSUDI_SUCCESS表示读取成功，CSUDI_FAILURE表示读取失败
@note 若平台不支持获取详细的Block信息，可将所有Flash划分为同一个Block，但不建议这样做。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashGetInfo(CSUDIFLASHRegionSuitType_E eSuitType,CSUDIFLASHRegionSuit_S * psFlashSuit)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 擦除指定空间。

@param[in] dwStartAddress 待擦除空间的起始地址（绝对地址）
@param[in] uSpaceSize 待擦除空间大小（以字节为单位）
@return CSUDI_SUCCESS表示擦除操作成功,CSUDI_FAILURE表示擦除操作失败
@note 
- 擦除Flash仅要求按整Block进行，若传入的地址及大小不是整Block，可以返回CSUDIFLASH_ERROR_BAD_PARAMETER，但也
可以正常执行，但如果正常执行，需保证未指定擦除部分的内容不得被破坏。
- 如0-0x10000为一个Block，若要求擦除0-0x8000区域则可以返回CSUDIFLASH_ERROR_BAD_PARAMETER这种情况下Flash存储
的内容不发生任何变化；也可也返回CSUDI_SUCCESS，并且Flash0-0x7fff的内容被擦除,但其它Flash区域的内容，特别是
0x8000-0x10000的内容不得发生变化。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashEraseSpace(CSUDI_UINT32 dwStartAddress, unsigned int uSpaceSize)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 往FLASH中写入需要保存的数据 ,为了提高效率实现时不带擦除

@param[in] dwStartAddress 待写入空间的起始地址（绝对地址）
@param[in] pucBuf 要写入数据的缓冲区起始地址
@param[in] uDataLength 待写入数据长度（以字节为单位）
@return CSUDI_SUCCESS 表示写操作成功，CSUDI_FAILURE表示写操作失败
@note 
- 若所写区域未进行事先擦除则等效于CSUDIFlashWrite
- 中间件根据flash的特性保证写入地址的数据都是从1->0而不会出现从0变为1的操作，因此该接口只需要在写入的地方直接按位修改数据即可
- 例如:会把flash中某地址对应的数据: 0x31 0x39 0x38 0x34 写为: 0x31 0x39 0x38 0x30,只需要把0x34中的一位1,变为0即可。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashWriteWithoutErase( CSUDI_UINT32 dwStartAddress, const unsigned char * pucBuf, unsigned int uDataLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

