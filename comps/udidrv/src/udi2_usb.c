/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_usb.c
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

#include "udi2_usb.h"
/**
@brief 获取当前插入的USB 设备个数
@param[out] pdwUsbDeviceCount 当前插入的USB 设备个数
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIUSBGetCount(unsigned int* pdwUsbDeviceCount)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 根据设备索引获取USB 设备信息

@param[in] uIndex USB 设备索引ID，从0开始最大值为CSUDIUSBGetCount获取到的count-1
@param[out] psUsbInfo 指向返回USB设备的信息
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIUSBGetInfo(unsigned int uIndex, CSUDIUSBInfo_S * psUsbInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

