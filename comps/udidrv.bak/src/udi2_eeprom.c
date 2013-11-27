/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_eeprom.c
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

#include "udi2_eeprom.h"
/**
@brief 向EEPROM存储器件中写数据

@param[in] dwStartAddress 待写入存储区的起始地址。
@param[in] pucData 待写入的数据指针。
@param[in] uDataLength 待写入的数据长度(单位：字节)。
@return CSUDI_SUCCESS表示EEPROM器件写成功，CSUDI_FAILURE表示EEPROM器件写失败。
*/
CSUDI_Error_Code CSUDIEPRWrite(CSUDI_UINT32 dwStartAddress, const unsigned char * pucData, unsigned int  uDataLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 从EEPROM存储器件中读数据

@param[in] dwStartAddress 待读出存储区的起始地址。
@param[out] pucData 存放读出数据的数据指针。
@param[in] uDataLength 待读出数据的长度(单位：字节)。
@return CSUDI_SUCCESS表示EEPROM器件读成功，CSUDI_FAILURE表示EEPROM器件读失败。
*/
CSUDI_Error_Code CSUDIEPRRead(CSUDI_UINT32 dwStartAddress, unsigned char * pucData, unsigned int  uDataLength )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取EEPROM芯片相关信息

@param[out] psEPRInfo 待写入EEPROM信息的指针参数。
@return 操作成功返回 CSUDI_SUCCESS;否则返回 CSUDI_FAILURE   
@note 该函数用来获取应用数据EEPROM结构的信息，此信息在进行数据读写时将会用到。
@note本函数是在进行EEPROM实际操作之前所应调用的第二个函数（第一个函数为eeprom初始化函数。
@note在调用时，psEPRInfo由调用者分配空间，该函数只需将相应的项赋值即可
*/
CSUDI_Error_Code CSUDIEPRGetInfo(CSUDIEPRInfo_S * psEPRInfo)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

