/**@defgroup AOUT AOUT 定义音频播放后处理相关接口
@brief AOUT模块对应头文件<udi2_aout.h>，包括了音频播放后处理相关接口。

多个Audio（解码器）将多路声音通过mixer混合后的声音输出到Aout，其大致相当于输出接口的抽象，如hdmi、spdif等。Mixer由于暂无相关的控制需要，未进行抽象。

Aout和Audio模块的区别在于Audio控制解码器对声音的处理，好比自来水厂，它生产声音；而本模块控制声音的输出端口，好比家里的水龙头，它控制声音的输出 。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
		This module is used to control RCA, SPDIF, HDMI audio output, such as: 
			audio output mute;
			audio output unmute;
			audio output passthrough mode;
			audio output decoded mode;
-------------------------------------------------------------------------------

@version 2.0.2 2009/08/25 增加Aout本身意义及位置说明
@version 2.0.1 2009/08/16 初稿
@{
*/
#ifndef _UDI2_AOUT_H_
#define _UDI2_AOUT_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 音频控制错误代码值*/
enum
{
	CSUDIAOUT_ERROR_BAD_PARAMETER = CSUDI_AOUT_ERROR_BASE,  ///< 错误参数
	CSUDIAOUT_ERROR_INVALID_DEVICE_ID,						///< 非法设备ID
	CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED					///< 不支持此操作
};

/**@brief 音频通道类型*/
typedef enum
{
	EM_UDIAOUT_RCA		= 1,			///<音频左右声道输出。
	EM_UDIAOUT_SPDIF	= 1 << 1,		///<音频SPDIF输出。
	EM_UDIAOUT_HDMI 	= 1 << 2		///< HDMI音频输出
}CSUDIAOUTDevices_E;


/**@brief 音频通道输出的模式*/
typedef enum
{
	EM_UDIAOUT_DECODE,	///<音频码流需要经过解码，然后输出
	EM_UDIAOUT_BYPASS	///<音频码流不需要解码而是直接输出，由后端输出设备（如：带SPDIF输出的功放）完成解码和播放。
}CSUDIAOUTMode_E;

/**
@brief 设置指定音频输出通道为静音

@param[in] eAudioDevice  音频 输出通道，具体请参见CSUDIAOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户设定的指定音频通道上实现静音功能
@note 可以多次调用，但只有第一次有效
*/
CSUDI_Error_Code CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice);

/**
@brief 取消指定音频输出通道的静音状态

@param[in] eAudioDevice 音频输出通道，具体请参见CSUDIAOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户设定的指定音频设备上实现取消静音的功能
@note 在非mute状态下调用此接口无效
@note 可以多次调用，但只有第一次有效
*/
CSUDI_Error_Code CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice);

/**
@brief 设置指定输出通道的输出模式

@param[in] eAudioDevice 音频输出通道，具体请参见CSUDIAOUTDevices_E
@param[in] eMode 设备的输出模式，具体请参见CSUDIAOUTMode_E。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 缺省情况下，所有音频输出通道为解码后输出模式，并且左右声道只能是解码后输出模式。
@note 如果当前处于播放状态，调用该接口马上生效，可以允许声音短暂的停止
*/
CSUDI_Error_Code CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

