/**@defgroup AUDIO AUDIO 定义了平台无关的音频驱动接口
@brief AUDIO模块对应头文件<udi2_audio.h>，包括音频解码器属性及控制接口。

跟Video一样，一个可以并且需要独立工作的音频解码器抽象为一个Audio解码器。其索引一般从0开始，一般来说0表示主解码器。如果芯片支持同时播放两路独立音频，则可以抽象出两个Audio解码器，这一点上跟pip是一致的，虽然一般情况下，不会有这样的需求。

如果芯片支持在播放视频的同时播放PCM，也可以独立出一个PCM的Audio解码器 。

@version 2.0.3 2009/10/12 修改描述错误以及增加输出模式设置生效规则
@version 2.0.2 2009/08/16 增加Audio抽象规则说明
@version 2.0.1 2009/08/16 初稿
@{
*/
#ifndef _UDI2_AUDIO_H_
#define _UDI2_AUDIO_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_aout.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDI_MAX_VOLUME		(31)  ///< 音量最大值

/**@brief 音频错误代码值*/
enum
{
	CSUDIAUDIO_ERROR_BAD_PARAMETER = CSUDI_AUDIO_ERROR_BASE,    ///< 错误参数
	CSUDIAUDIO_ERROR_INVALID_DEVICE_ID,							///< 非法设备ID
	CSUDIAUDIO_ERROR_FEATURE_NOT_SUPPORTED						///< 不支持此操作
};

/**@brief 音频声道*/
typedef enum 
{
  	EM_UDIAUDIO_CHANNEL_STEREO,			///< 立体声 
  	EM_UDIAUDIO_CHANNEL_LEFT,			    ///< 左声道 
  	EM_UDIAUDIO_CHANNEL_RIGHT,				///< 右声道 
  	EM_UDIAUDIO_CHANNEL_MIXED_MONO			///< 混合声 
}CSUDIAUDIOChannelType_E;	

/**@brief Audio Decoder设备能力信息
@note 数组中，如果碰到音频类型为EM_UDI_AUD_STREAM_INVALID，则表示支持的音频格式类型已经结束，后续数据将被忽略。
*/
typedef struct  
{
	CSUDIAUDStreamType_E m_sSupportDecode[EM_UDI_AUD_STREAMTYPE_NUM];   ///< 音频解码器支持的音频格式类型数组
	CSUDIAUDStreamType_E m_sSupportByPass[EM_UDI_AUD_STREAMTYPE_NUM];   ///< 支持直接输出的音频个数类型数组
} CSUDIAUDIOCapability_S;


/**@brief 音频状态结构体 */
typedef struct 
{
	CSUDIAUDStreamType_E			m_eStreamType;	///<  音频流类型 
	int								m_nSampleRate;  ///<  音频采样率 
	int								m_nBitWidth;	///<  音频采样位数 
	int								m_nChannels;  	///<  通道数:1-mono,2-stereo 
	unsigned int					m_uFrameCount;  ///<  已解码的帧数 
}CSUDIAUDIOStatus_S;

/**
@brief 获取Audio Decoder设备的总数
@param[out] pnAudioDecCount 系统中 存在Audio Decoder设备的个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pnAudioDecCount的内容未定义
*/
CSUDI_Error_Code CSUDIAUDIOGetCount(int * pnAudioDecCount);

/**
@brief 获取指定Audio Decoder设备的能力

无论该设备是空闲或者被占用
@param[in] nAudioIndex Audio Decoder设备的索引
@param[out] psCapabilityInfo Audio Decoder设备能力信息,详情请参见CSUDIAUDIOCapability_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psCapabilityInfo结构的内容未定义
*/
CSUDI_Error_Code CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo);

/**
@brief 获取指定Audio Decode设备的状态信息

@param[in] nAudioIndex Audio Decoder设备的索引
@param[out] psStatus 用于存放Audio Decoder 状态信息的数据结构地址指针
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psStatus结构的内容未定义
*/
CSUDI_Error_Code CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus);

/**
@brief 设置指定音频解码器的声道

@param[in] nAudioIndex Audio Decoder设备的索引
@param[in] eChannelType 用户需要输出的声道, 如LEFT, RIGHT, STEREO
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定的音频解码器上，切换模拟音频输出的声道
@note 目前只支持RCA通道的设置
*/
CSUDI_Error_Code CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType);

/**
@brief 设置指定音频解码器为静音

@param[in] nAudioIndex  Audio Decoder设备的索引
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定音频解码器上实现静音功能
@note 可以多次调用，但只有第一次有效
*/
CSUDI_Error_Code CSUDIAUDIOMute(int nAudioIndex);

/**
@brief 取消指定音频解码器的静音状态

@param[in] nAudioIndex  Audio Decoder设备的索引
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定音频解码器上实现取消静音的功能
@note 在非mute状态下调用此接口无效
@note 可以多次调用，但只有第一次有效
*/
CSUDI_Error_Code CSUDIAUDIOUnmute(int nAudioIndex);

/**
@brief 设置指定音频解码器的音量

@param[in] nAudioIndex  Audio Decoder设备的索引
@param[in] nVolume 用户需要输出的音量值，按[0,CSUDI_MAX_VOLUME]阶音量计算,0为无声,CSUDI_MAX_VOLUME最大
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 所有输出设备上统一设置音量
@note 音量超过有效范围返回错误,并不生效
*/
CSUDI_Error_Code CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

