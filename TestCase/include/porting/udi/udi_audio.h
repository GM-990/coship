/**@defgroup CSAudio audio 提供统一的音频相关接口,如音频播放,音频相关属性的设置及设置音频事件监听等操作
@brief 提供统一的音频相关接口,如音频播放,音频相关属性的设置及设置音频事件监听等操作

@version 0.2 2008/11/04 添加详细描述
@version 0.1 2008/10/21 初始版本
@{
*/
#ifndef _UDI_SYSAUDIO_H_
#define _UDI_SYSAUDIO_H_

#include "udi_typedef.h"
#include "udi_public.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 音频流编码格式，需注意与打包格式(TS、AVI、ASF等区别)*/
typedef enum _CSAUDStreamType
{
	CSAUD_STREAM_AC3,			/**< Dolby Digital AC3 audio */
	CSAUD_STREAM_DTS,			/**< Digital Surround sound. */
	CSAUD_STREAM_MPEG1,			/**< MPEG1/2, layer 1/2. This does not support layer 3 (mp3). */
	CSAUD_STREAM_MPEG2,			/**< MPEG1/2, layer 1/2. This does not support layer 3 (mp3). */
	CSAUD_STREAM_CDDA,			/**< 1979年，Philips和Sony公司结盟联合开发CD-DA Compact Disc-Digital Audio,精密光盘数字音频）标准*/
	CSAUD_STREAM_PCM,			
	CSAUD_STREAM_LPCM,			
  	CSAUD_STREAM_MP3,			/**< MPEG1/2, layer 3. */
  	CSAUD_STREAM_MPEG_AAC,		/**< Advanced audio coding. Part of MPEG-4 */
  	CSAUD_STREAM_HE_AAC,		/**< AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE) */
  	CSAUD_STREAM_RAW_AAC,	
	CSAUD_STREAM_AAC_PLUS_ADTS, /**< AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format) */
	CSAUD_STREAM_AAC_PLUS_LOAS, /**< AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream) */
	CSAUD_STREAM_AC3_PLUS,		/**< Dolby Digital Plus (AC3+ or DDP) audio */
	CSAUD_STREAM_DTS_HD,		/**< Digital Surround sound, HD */
  	CSAUD_STREAM_WMA_STD,		/**< WMA Standard */
  	CSAUD_STREAM_WMA_PRO,		/**< WMA Professional */ 
 	CSAUD_STREAM_DRA,           /**< Digital Rise Audio */
  	CSAUD_STREAMTYPE_NUM	
}CSAUDStreamType;

/**@brief 音频打包类型*/
typedef enum _CSAUDStreamPacketType
{
	CSAUD_STREAM_PACKET_TYPE_PES,
	CSAUD_STREAM_PACKET_TYPE_ES,
  	CSAUD_STREAM_PACKET_TYPE_NUM
}CSAUDStreamPacketType;

/**@brief 音频输入信息*/
typedef struct _CSSYSAUDInputInfo
{
	CSAUDStreamType     StreamType;	    /**< 流类型:Mpeg2/Mp3/AAC/AC3*/
	int 			    nSampleRate;	/**< 音频采样率:44100, 48000, 32000 etc.,0表示未知*/
	int 			    nPID;		    /**< 数据所在PID*/
	CSSYSServiceType    ServiceType;    /**< 数据打包类型:PES or ES*/
	int 			    nBitWidth;	    /**< 采样位数：8，16,0表示未知*/
	int 			    nChannel;	    /**< 通道数:1-mono,2-stereo,0表示未知*/
}CSSYSAUDInputInfo;


/**@brief 定义音频输出接口类型*/
typedef enum _CSAUDOutputDevice
{
	CSAUD_OUTPUT_RCA	=0x00000001,	/**< 常规音频左右声道输出接口*/
	CSAUD_OUTPUT_SPDIF	=0x00000002,	/**< 数字音频接口*/
	CSAUD_OUTPUT_RF		=0x00000004,	/**< RF调制输出*/
	CSAUD_OUTPUT_SCART	=0x00000008,	/**< scart接口*/
	CSAUD_OUTPUT_HDMI  	=0x00000010	    /**< hdmi 接口*/
}CSAUDOutputDevice;	

/**@brief 声道*/
typedef enum _CSAUDChannelType
{
  	CSAUD_CHANNEL_STEREO,			/**< 立体声*/
  	CSAUD_CHANNEL_LEFT,			    /**< 左声道*/
  	CSAUD_CHANNEL_RIGHT,			/**< 右声道*/
  	CSAUD_CHANNEL_MIXED_MONO		/**< 混合声*/
}CSAUDChannelType;	
 
/**@brief 音频状态结构体 */
typedef struct _CSSYSAUDStatus_S
{
	BOOL 			m_bMute;		/**< 当静音时为true*/
	CSAUDStreamType 	m_eAudStreamType;	/**< 音频流类型*/
	CSAUDChannelType 	m_eChannelType;		/**< 声道*/
	int 			m_nSampleRate;		/**< 音频采样率*/
	int 			m_nBitWidth;		/**< 音频采样位数*/
	DWORD			m_dwFrameCount;		/**< 解码后的帧数*/
	DWORD 			m_dwReserved1;		/**< 保留参数，须填0*/
	DWORD			m_dwReserved2;		/**< 保留参数，须填0*/
}CSSYSAUDStatus_S;

/**@brief PCM注入描述参数*/
typedef struct _CSPcmStartParam_S
{
	unsigned uBitsSample;	/**< 采样位数:8 or 16*/
	unsigned uSampleRate;	/**< 采样率:32000,44100,48000*/
	unsigned uChannels;  	/**< 通道数:1-mono,2-stereo*/
}CSPcmStartParam_S;

/**@brief PCM字节序参数*/
typedef enum _CSPcmEndian_e
{
	CSPCM_ERROR_ENDIAN,
	CSPCM_DEFAULT_ENDIAN,	/**< 默认字节序，即平台推荐使用的字节序*/
	CSPCM_LITTLE_ENDIAN,	/**< 小端字节序*/
	CSPCM_BIG_ENDIAN	/**< 大端字节序*/
}CSPcmEndian;

/**@brief PCM注入状态*/
typedef struct _CSPcmStatusParam_S
{	
	DWORD m_dwQueuedByte;        /**< 注入空间中剩余的未播放的数据字节数*/
	DWORD m_ulFifoSize;          /**< 注入空间总大小*/
}CSPcmStatusParam_S;

/**@brief 输出音频内容信息结构体*/
typedef struct _CSSYSAUDOutputInfo
{
	CSAUDOutputDevice	OutputDevice;	/**< 输出接口: AV/spdif/hdmi*/
	BOOL 			    bMute;		    /**< 输出是否静音 */
	int 			    nVolume;	    /**< 输出音量, 预置  32 */
	CSAUDChannelType	ChannelType;	/**< 输出声道 */
}CSSYSAUDOutputInfo;

/**
@brief 用户设置Audio/pcr等节目信息

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] pstAUDInfo 输入节目的音频信息
@param[in] pstPCRInfo 输入节目的PCR信息，如果无PCR信息可以留空，PCRPID非法相当于无PCR，仍可正常设置
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 函数运行时不能启动音频的解码
*/
BOOL CSSYSAUDSetInput(HCSHANDLE hHandle, CSSYSAUDInputInfo *pstAUDInfo, CSSYSPCRInputInfo* pstPCRInfo,HCSHANDLE hReserved );

/**
@brief 设置当前音视频数据的打包类型

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] ePacketType 音视频数据打包类型，如ES或PES
@return 成功返回TRUE,失败返回FALSE
@note 在CSSYSAUDSetInput之后且启动播放之前调用，启动播放时生效，如果不调用则默认为PES格式
*/
BOOL CSSYSAUDSetStreamPacketType(HCSHANDLE hHandle, CSAUDStreamPacketType ePacketType);

/**
@brief 设置音频输出设备的输出数据格式（压缩或非压缩格式）

主要用来设置AC3等音频格式是否要在机顶盒里进行解码，还是直接输出到音响等设备以获得更好的效果
@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] eDevice 音频输出端口，可为CSAUDOutputDevice中各项相或
@param[in] bCompressed 是否为压缩格式
@note 如果dwOutputDevice中存在任何不支持或者不能同时支持的设备,
则返回FALSE,并且不进行任何设置(包括eDevice中指定的支持的设备)
@note 非压缩格式实际上就是原始的PCM音频格式，而其它任何形式均认为是压缩格式(如AC3)
@note 因为设备一般不具备编码功能，所以只能选择解码或者不解码，而不能选择以特定的格式输出
*/
BOOL CSSYSAUDSetOutputDeviceDataFormat(HCSHANDLE hHandle, CSAUDOutputDevice eDevice, BOOL bCompressed);

/**
@brief 启动音频的解码、播放和输出

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 按照CSSYSAUDsudInput()等设置函数设置音频pid，以及音频的相关特性参数启动音频的播放
@note 非Stop状态下再次调用Play返回TRUE，但不重新开始播放,新设置的Pid也不生效
*/
BOOL CSSYSAUDPlay(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief 停止音频的播放和解码

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note Stop状态下再次调用Stop返回TRUE
*/
BOOL CSSYSAUDStop(HCSHANDLE hHandle,HCSHANDLE hReserved);

/**
@brief 暂停音频的播放和解码

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 暂停与hHandle相关的音频的播放。只有正在播放音频时调用才有效,又即已Pause时再次调用返回TRUE
*/
BOOL CSSYSAUDPause(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief 恢复已暂停的音频

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 恢复已暂停的音频，与CSSYSAUDPause成对使用
@note 已Resume状态下再次Resume仍返回TRUE
*/
BOOL CSSYSAUDResume(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief 音频设备静音

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 在用户设定的指定音频设备上实现静音功能
@note 可以多次调用，但只有Unmute后的第一次有效
*/
BOOL CSSYSAUDMute(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief 取消音频设备静音

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] hReserved  保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 在用户设定的指定音频设备上实现取消静音的功能
@note 可以多次调用，但只有Mute后的第一次有效
@note 虽然应用上大量使用，但调整音量不会自动Unmute
*/
BOOL CSSYSAUDUnmute( HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief 设置音频音量

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] nVolume 用户需要输出的音量值，按[0,31]阶音量计算,0为无声,31最大
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 所有输出设备上统一设置音量
@note 音量超过有效范围返回错误,并不生效
*/
BOOL CSSYSAUDSetVolume(HCSHANDLE hHandle, int nVolume, HCSHANDLE hReserved);

/**
@brief 设置音频声道

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] enmChannelType 用户需要输出的声道, 如LEFT, RIGHT, STEREO
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 在用户指定的音频设备上，切换模拟音频输出的声道
*/
BOOL CSSYSAUDSetChannel(HCSHANDLE hHandle,CSAUDChannelType enmChannelType,HCSHANDLE hReserved);

/**
@brief 当前系统音频的状态

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[out] pstAUDStatus 指向CSSYSAUDStatus_S 结构的指针
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSYSAUDGetStatus(HCSHANDLE hHandle,CSSYSAUDStatus_S *pstAUDStatus);

/*---------------PCM注入相关接口---------------*/
/**
@brief 初始化PCM注入

为PCM播放做好准备
@return PCM播放句柄
@note 暂不支持选择设备
*/
HCSHANDLE CSPcmPlay_Open(void);

/**
@brief 开始播放PCM音频

@param[in] hPcm PCM播放句柄，由CSPcmPlay_Open返回
@param[in] psPcmStart 播放选项参数，指示待注入的PCM数据的属性
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSPcmPlay_Start(HCSHANDLE hPcm, CSPcmStartParam_S * psPcmStart);

/**
@brief 获取PCM播放数据Buffer

获取用于存放待播放PCM播放数据的的内存，如获取成功则会将接下来要播放的PCM数据拷贝到其中
否则表示缓冲区已满，暂时无需放入新数据
上层需不断的调用该接口获取Buffer，以便及时填充待播放数据
@param[in] hPcm PCM播放句柄
@param[out] ppvBuffer 返回获取到的内存地址指针
@param[out] puLength 返回获取到的内存长度
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSPcmPlay_GetFreeBuffer(HCSHANDLE hPcm,void ** ppvBuffer, unsigned  * puLength );

/**
@brief 通知本次注入的数据

通过CSPcmPlay_GetFreeBuffer获取到内存后，将待播放数据拷贝到其中后，使用本函数通知底层模块数据拷贝完成及拷贝了多少
本函数调用后，此次拷贝的数据即可用于播放了
@param[in] hPcm PCM播放句柄
@param[in] uWrittenSize 已拷贝数据的大小
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSPcmPlay_WriteComplete(HCSHANDLE hPcm, unsigned uWrittenSize);

/**
@brief 停止播放

@param[in] hPcm PCM播放句柄
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSPcmPlay_Stop(HCSHANDLE hPcm);

/**
   @brief 暂停播放PCM音频

   @param[in]hPcm PCM 播放句柄，由CSPcmPlay_Open返回
   @return 成功返回TRUE；失败返回FALSE
   @note 已Pause状态下再次Pause仍然返回TRUE
*/
BOOL CSPcmPlay_Pause(HCSHANDLE hPcm);

/**
   @brief 恢复已经暂停播放PCM音频

   @param[in]hPcm PCM 播放句柄，由CSPcmPlay_Open返回
   @return 成功返回TRUE；失败返回FALSE
   @note 已Resume状态下再次Resume仍然返回TRUE
*/
BOOL CSPcmPlay_Resume(HCSHANDLE hPcm);

/**
@brief 关闭PCM播放

释放相关资源
@param[in] hPcm PCM播放句柄
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSPcmPlay_Close(HCSHANDLE hPcm);

/**
@brief 将缓存中的pcm数据播放完全

本函数会堵塞，一直等到缓存中的数据播放完成了才返回
一般用于待播放数据全部注入完毕后，等待播放结束
@param[in] hPcm PCM播放句柄
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSPcmPlay_Flush(HCSHANDLE hPcm);

/**
@brief 设置并返回PCM字节序

重复设置仅返回不生效
@param[in] hPcm PCM播放句柄
@param[in] enmEndian 目标字节序
- PCM_LITTLE_ENDIAN 期望使用小字节序
	- 若平台仅支持PCM_BIG_ENDIAN则返回PCM_BIG_ENDIAN
	- 若平台支持PCM_LITTLE_ENDIAN，则返回PCM_LITTLE_ENDIAN
- PCM_BIG_ENDIAN 期望使用大字节序
	- 若平台仅支持PCM_LITTLE_ENDIAN则返回PCM_LITTLE_ENDIAN
	- 若平台支持PCM_BIG_ENDIAN，则返回PCM_BIG_ENDIAN
- PCM_DEFAULT_ENDIAN 返回平台推荐使用的字节序，如效率更高的字节序
@return 返回实际字节序状态
@note 因为平台不一定支持两种字节序，所以实际返回的字节序与设置的字节序不一定相同
*/
CSPcmEndian CSPcmPlaySetEndian(HCSHANDLE hPcm, CSPcmEndian enmEndian);

/**
@brief 获取PCM注入的状态信息

用于获取"精确"的注入流播放位置，因为数据从注入到播放还需要一些时间，这些数据就是用来计算这段时间的
@param[in] hPcm PCM播放句柄
@param[out] psPcmStatus 指向CSPcmStatusParam_S结构的指针
@return 成功返回TRUE,失败返回FALSE
@note 实际上由于该模块的下层模块可能仍有缓存，这个时间也不能保证100%精确，但基本上已经可用
*/
BOOL CSPcmPlayGetStatus(HCSHANDLE hPcm, CSPcmStatusParam_S *psPcmStatus);

/**
@brief 清除已注入的所有PCM数据

主要用于快进、快退、定位等操作时清空已注入的数据，已防止这些操作的延迟
@param[in] hPcm PCM播放句柄
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSPcmPlayClearBuffer(HCSHANDLE hPcm);



/*------------------以下接口实际上PVR未调用，但本地媒体播放却可能用到--------------*/

/**
@brief 音频模块进入快进,快退,定位模式

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来 
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSYSAUDTrickModeStart(HCSHANDLE hHandle,HCSHANDLE hReserved);

/**
@brief 退出音频的快进,快退,定位模式

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来 
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSYSAUDTrickModeStop(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief 设置Audio的播放速度

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] nSpeed  	播放的速度
@return 成功返回TRUE,失败返回FALSE
@note 该接口实际是音频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放此函数无效。
@note 1000为常速播放,2000表示2倍速，500表示正向半速0为pause,负数为反向播放,例如-1000表示反向一倍速。
*/
BOOL CSSYSAUDSetSpeed(HCSHANDLE hHandle, int nSpeed);



#ifdef  __cplusplus
}
#endif

/** @} */

#endif


