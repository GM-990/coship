/**@defgroup PUBLIC PUBLIC  定义了音视频以及图片解码相关接口用到的公共类型
@brief Public模块对应头文件udi2_public.h，定义了音视频以及图片解码相关接口用到的公共类型

音视频模块对设备,功能的抽象请参考下图(其中虚线部分为未抽象的模块):
@image HTML udi_av.png

- 其中第一排(player,inject,record)为混合服务抽象，提供了一些利用多种设备完成的服务。
- 第二排(demux,video,audio)为设备抽象，提供了一些设备及它们能力的抽象。
- 第三排及以下为独立服务的抽象，提供了利用对应设备的独自完成的功能。

@version 2.0.4 2009/11/04 去掉音视频数据打包格式及来源,inject模块已经包含
@version 2.0.3 2009/08/28 增加了矩形区域结构体
@version 2.0.2 2009/08/25 增加了对整个音视频模块的抽象说明
@version 2.0.1 2009/08/17 初稿

@{
*/
#ifndef _UDI2_PUBLIC_H_
#define _UDI2_PUBLIC_H_


#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDI_MAX_FILE_NAME_LEN (256)       ///< 文件夹名称的最大长度

#define CSUDI_MAX_URL_LEN (1024)       ///< URL 最大长度

#define CSUDI_MAX_SECURITYKEY_LEN (128)  ///< 最大密码长度 单位:BYTES,用于指定AED,DES加密密钥长度

#define CSUDI_INVALID_PID (0xe000)          ///< 非法的音视频PID值

#define CSUDI_INVALID_INDEX (-1)   	        ///< 非法的设备id


/**@brief 音频流编码格式，需注意与打包格式(TS、AVI、ASF等区别)*/
typedef enum 
{
	EM_UDI_AUD_STREAM_UNKNOWN,      ///< 未知类型，这种情况下，驱动应尽量识别，主要用于解AVI等容器中的音视频
	EM_UDI_AUD_STREAM_AC3,			///< Dolby Digital AC3 audio  
	EM_UDI_AUD_STREAM_DTS,			///< Digital Surround sound 
	EM_UDI_AUD_STREAM_MPEG1,		///< MPEG1/2, layer 1/2. This does not support layer 3 (mp3) 
	EM_UDI_AUD_STREAM_MPEG2,		///< MPEG1/2, layer 1/2. This does not support layer 3 (mp3) 
	EM_UDI_AUD_STREAM_CDDA,			///< 1979年，Philips和Sony公司结盟联合开发CD-DA Compact Disc-Digital Audio,精密光盘数字音频）标准 
	EM_UDI_AUD_STREAM_PCM,			///< PCM data
	EM_UDI_AUD_STREAM_LPCM,		
  	EM_UDI_AUD_STREAM_MP3,			///<  MPEG1/2, layer 3 
  	EM_UDI_AUD_STREAM_MPEG_AAC,		///<  Advanced audio coding. Part of MPEG-4  
  	EM_UDI_AUD_STREAM_HE_AAC,		///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE)  
  	EM_UDI_AUD_STREAM_RAW_AAC,	    
	EM_UDI_AUD_STREAM_AAC_PLUS_ADTS, ///<   AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format)  
	EM_UDI_AUD_STREAM_AAC_PLUS_LOAS, ///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream)  
	EM_UDI_AUD_STREAM_AC3_PLUS,		///<   Dolby Digital Plus (AC3+ or DDP) audio 
	EM_UDI_AUD_STREAM_DTS_HD,		///<   Digital Surround sound, HD 
  	EM_UDI_AUD_STREAM_WMA_STD,		///<   WMA Standard  
  	EM_UDI_AUD_STREAM_WMA_PRO,		///<   WMA Professional 
  	EM_UDI_AUD_STREAM_DRA,			///< DRA1
  	EM_UDI_AUD_STREAMTYPE_NUM				
}CSUDIAUDStreamType_E;							

/**@brief 视频流编码格式，需注意与打包格式(TS、ES、AVI、ASF等区别)*/
typedef enum 
{
	EM_UDI_VID_STREAM_UNKNOWN,      ///< 未知类型，这种情况下，驱动应尽量识别，主要用于解AVI等容器中的音视频
	EM_UDI_VID_STREAM_MPEG1,		///< MPEG-1 Video (ISO/IEC 11172-2) 
	EM_UDI_VID_STREAM_MPEG2,		///< MPEG-2 Video (ISO/IEC 13818-2)  
	EM_UDI_VID_STREAM_MPEG4,		///< MPEG-4 Part 2 Video  
	EM_UDI_VID_STREAM_H264,			///< H.264 (ITU-T) or ISO/IEC 14496-10/MPEG-4 AVC 
	EM_UDI_VID_STREAM_H263,			///< H.263 Video. The value of the enum is not based on PSI standards 
	EM_UDI_VID_STREAM_VC1,			///< VC-1 Advanced Profile  
	EM_UDI_VID_STREAM_VC1_SM,		///< VC-1 Simple&Main Profile 
	EM_UDI_VID_STREAM_DIVX_311,		///< DivX 3.11 coded video 
  	EM_UDI_VID_STREAMTYPE_NUM
}CSUDIVIDStreamType_E;


/**@brief 流类型*/
typedef enum 
{
	EM_UDI_CONTENT_VIDEO = 0,	///< 视频类型	 						
	EM_UDI_CONTENT_AUDIO,  	    ///< 音频类型						
	EM_UDI_CONTENT_SUBTITLE,    ///< 字幕类型
	EM_UDI_CONTENT_TELTEXT, 	///< 图文电视类型
	EM_UDI_CONTENT_PCR 		///< PCR类型	
}CSUDIContentType_E;

/**@brief 播放信息类型*/
typedef struct
{
	int                         m_nPid;                        ///< 数据所在PID.pid值的范围遵循13818-1。但是，如果此stream在数据注入播放模式中使用时，请赋值m_nPid=-1
	CSUDIContentType_E       	m_eContentType;  	           ///< 流类型，见CSUDIContentType_E定义
	union 
	{
		CSUDIVIDStreamType_E m_eVideoType; ///< 视频流类型
		CSUDIAUDStreamType_E m_eAudioType; ///< 音频流类型
	} m_uStreamType;   	///< 数据的流类型，有如下两种，并且根据m_eContentType的类型确定使用以下哪种数据流。-CSUDIVIDStreamType_E m_eVideoType：视频流类型。-CSUDIAUDStreamType_E m_eAudioType：音频流类型。
	
}CSUDIStreamInfo_S;

/**@brief 矩形区域结构定义 */
typedef struct 
{
	int m_nX;                ///< 矩形区域左上角横坐标 
	int m_nY;                ///< 矩形区域左上角纵坐标 
	int m_nWidth;            ///< 矩形区域宽度 > 0
	int m_nHeight;           ///< 矩形区域高度 > 0
} CSUDIWinRect_S;


#ifdef  __cplusplus
}
#endif
/** @} */
#endif


