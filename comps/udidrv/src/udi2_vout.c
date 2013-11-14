/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_vout.c
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

#include "udi2_vout.h"
/**
@brief 打开指定的视频输出设备

@param[in] eVoutDevice  视频输出设备，具体请参见CSUDIVOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVOUTEnable(CSUDIVOUTDevices_E eVoutDevice)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 关闭指定的视频输出设备

@param[in] eVoutDevice	视频输出设备，具体请参见CSUDIVOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVOUTDisable(CSUDIVOUTDevices_E eVoutDevice)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	 
	return Retcode;
}

/**
@brief 设置视频输出端口的颜色类型

@param[in] eVoutDevice 视频输出设备,只支持分量(EM_UDIVOUT_YUV)/HDMI(EM_UDIVOUT_HDMI)
@param[in] eType 输出的颜色类型，RGB或是YUV
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置立即生效
*/
CSUDI_Error_Code CSUDIVOUTSetOutputType(CSUDIVOUTDevices_E eVoutDevice, CSUDIVOUTOutputType_E eType)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

