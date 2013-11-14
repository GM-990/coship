/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_audio.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
		 This module is used to get the audio decoders' properities supported by platform:
			 get the number of audio decoders;
			 get the capabilities of audio decoders;
			 get the status of audio decoders;
		 and do some audio decoder output control, such as:
			 audio decoding output stereo mode;
			 audio decoding output mute/unmute;
			 audio decoding output volume control;
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_audio.h"


/**
@brief 获取Audio Decoder设备的总数
@param[out] pnAudioDecCount 系统中 存在Audio Decoder设备的个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pnAudioDecCount的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetCount(int * pnAudioDecCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 获取指定Audio Decoder设备的能力

无论该设备是空闲或者被占用
@param[in] nAudioIndex Audio Decoder设备的索引
@param[out] psCapabilityInfo Audio Decoder设备能力信息,详情请参见CSUDIAUDIOCapability_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psCapabilityInfo结构的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief 获取指定Audio Decode设备的状态信息

@param[in] nAudioIndex Audio Decoder设备的索引
@param[out] psStatus 用于存放Audio Decoder 状态信息的数据结构地址指针
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psStatus结构的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief 设置指定音频解码器的声道

@param[in] nAudioIndex Audio Decoder设备的索引
@param[in] eChannelType 用户需要输出的声道, 如LEFT, RIGHT, STEREO
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定的音频解码器上，切换模拟音频输出的声道
@note 目前只支持RCA通道的设置

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief 设置指定音频解码器为静音

@param[in] nAudioIndex  Audio Decoder设备的索引
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定音频解码器上实现静音功能
@note 可以多次调用，但只有第一次有效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOMute(int nAudioIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief 取消指定音频解码器的静音状态

@param[in] nAudioIndex  Audio Decoder设备的索引
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定音频解码器上实现取消静音的功能
@note 在非mute状态下调用此接口无效
@note 可以多次调用，但只有第一次有效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOUnmute(int nAudioIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief 设置指定音频解码器的音量

@param[in] nAudioIndex  Audio Decoder设备的索引
@param[in] nVolume 用户需要输出的音量值，按[0,CSUDI_MAX_VOLUME]阶音量计算,0为无声,CSUDI_MAX_VOLUME最大
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 所有输出设备上统一设置音量
@note 音量超过有效范围返回错误,并不生效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


