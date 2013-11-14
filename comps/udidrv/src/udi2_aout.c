/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_aout.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
		 This module is used to control RCA, SPDIF, HDMI audio output, such as: 
			 audio output mute;
			 audio output unmute;
			 audio output passthrough mode;
			 audio output decoded mode;
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_aout.h"

/**
@brief 设置指定音频输出通道为静音

@param[in] eAudioDevice  音频 输出通道，具体请参见CSUDIAOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户设定的指定音频通道上实现静音功能
@note 可以多次调用，但只有第一次有效
*/
CSUDI_Error_Code CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 取消指定音频输出通道的静音状态

@param[in] eAudioDevice 音频输出通道，具体请参见CSUDIAOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户设定的指定音频设备上实现取消静音的功能
@note 在非mute状态下调用此接口无效
@note 可以多次调用，但只有第一次有效
*/
CSUDI_Error_Code CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置指定输出通道的输出模式

@param[in] eAudioDevice 音频输出通道，具体请参见CSUDIAOUTDevices_E
@param[in] eMode 设备的输出模式，具体请参见CSUDIAOUTMode_E。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 缺省情况下，所有音频输出通道为解码后输出模式，并且左右声道只能是解码后输出模式。
@note 如果当前处于播放状态，调用该接口马上生效，可以允许声音短暂的停止
*/
CSUDI_Error_Code CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
