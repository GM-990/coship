/**@defgroup INJECTER INJECTER 定义了驱动数据注入相关接口
@brief 本模块主要定义了定义了各种数据注入的相关接口。
-注入播放必须保证以下面接口调用顺序调用正常工作
1.开始注入播放： CSUDIINJECTEROpen -> CSUDIPLAYEROpen -> CSUDIINJECTERStart -> CSUDIPLAYERStart
2.停止注入播放： CSUDIPLAYERStop ->	CSUDIINJECTERStop -> CSUDIPLAYERClose -> CSUDIINJECTERClose
详细描述参见PLAYER模块。


@version 2.0.2 2010/11/24 增加注入播放顺序描述注释
@version 2.0.1 2009/08/15 
@{
*/
#ifndef _UDI2_INJECTER_H_
#define _UDI2_INJECTER_H_

#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif 

/**@brief 数据注入相关错误代码值*/
enum
{
	CSUDIINJECTER_ERROR_BAD_PARAMETER = CSUDI_INJECT_ERROR_BASE,  ///< 错误参数
	CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED,                    ///< 不支持操作
	CSUDIINJECTER_ERROR_UNKNOWN_ERROR,							  ///< 一般错误 
	CSUDIINJECTER_ERROR_NO_MEMORY,								  ///< 无可用内存
	CSUDIINJECTER_ERROR_INVALID_DEVICE_ID,						  ///< 非法设备ID
	CSUDIINJECTER_ERROR_INVALID_HANDLE,							  ///< 非法句柄	
	CSUDIINJECTER_ERROR_INVALID_STATUS,							  ///< 非法状态
	CSUDIINJECTER_ERROR_DEVICE_BUSY,							  ///< 设备繁忙
	CSUDIINJECTER_ERROR_DEVICE_DISORDER,						 ///<设备错误，比如说注入TS的时候demux=-1,参数是对的，但逻辑不对
	CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE,	///<注入类型与注入内容不配
	CSUDIINJECTER_ERROR_ALREADY_STARTED,								///< 该注入已经启动
	CSUDIINJECTER_ERROR_ALREADY_STOPPED,								///< 该注入已经停止
	CSUDIINJECTER_ERROR_NOT_STARTED									///< 该注入已经停止

};

/**@brief Injecter枚举类型的定义*/
typedef enum 
{
	EM_INJECTER_PCM, 	      ///<  PCM注入播放器 
	EM_INJECTER_ES,		      ///<  ES注入播放器 
	EM_INJECTER_PES,	      ///<  PES注入播放器 
	EM_INJECTER_TS,		      ///<  TS注入播放器 
	EM_INJECTER_AVI,	      ///<  AVI注入播放器 
	EM_INJECTER_RM,	          ///<  RM注入播放器 
	EM_INJECTER_RMVB,	      ///<  RMVB注入播放器 
	EM_INJECTER_PS,			  ///<  PS注入播放器 
	EM_INJECTER_MAX               ///<上限 。
}CSUDIINJECTERType_E;


/**@brief Injecter 注入的媒体内容的枚举定义*/
typedef enum 
{
	EM_UDIINJECTER_CONTENT_DEFAULT,		///<  Injecter注入的内容，default为缺省类型 ，指TS等不需要或没有办法分辨出是音频、视频还是图文字幕信息
	EM_UDIINJECTER_CONTENT_AUDIO,		///<   Injecter注入的内容为音频数据
	EM_UDIINJECTER_CONTENT_VIDEO,		///<   Injecter注入的内容为视频数据 
	EM_UDIINJECTER_CONTENT_SUBTITLE,		///<   Injecter注入的内容为subtitle数据 
	EM_UDIINJECTER_CONTENT_TELETEXT,	///<   Injecter注入的内容为teletext数据 
	EM_UDIINJECTER_CONTENT_IFRAME,	///<   Injecter注入的内容为视频IFrame数据 
	EM_UDIINJECTER_CONTENT_MAX             ///< 上限。
}CSUDIINJECTERContentType_E;


/**@brief 描述PCM数据参数的结构定义*/
typedef struct
{
	unsigned int m_uBitsSample;	///< 采样位数:8,16,32
	unsigned int m_uSampleRate;	///< 采样率:32000,44100,48000 
	unsigned int m_uChannels;  	///< 通道数:1-mono,2-stereo 
}CSUDIINJECTERPcmStartParam_S;


/**@brief 平台支持的PCM字节序参数*/
typedef enum 
{
	EM_UDIINJECTER_ERROR_ENDIAN,             ///<  上限 。
	EM_UDIINJECTER_LITTLE_ENDIAN,		///< 小端字节序 
	EM_UDIINJECTER_BIG_ENDIAN			///< 大端字节序 
	
}CSUDIINJECTERPcmEndian_E;


/**@brief 同步方式选择的枚举类型定义*/
typedef enum 
{
	EM_UDIINJECTER_SYNC_PCR,			///<PCR同步模式
	EM_UDIINJECTER_SYNC_AUDIO_FIRST,	///<音频PTS为时钟基准的同步模式
	EM_UDIINJECTER_SYNC_VIDEO_FIRST,	///<视频PTS为时钟基准的同步模式
	EM_UDIINJECTER_SYNC_NONE			///<不需要同步
}CSUDISYNCMode_E;

/**@brief Injecter 同步参数结构定义*/
typedef struct
{

	CSUDI_HANDLE m_hSyncHandle;	    ///< 作为同步基准的注入句柄，该句柄由CSUDIINJECTEROpen得到
	CSUDISYNCMode_E m_eSyncMode;	///< 指定同步模式，同步模式的内容，请参见CSUDISYNCMode_E
	
}CSUDIPESSYNCMode_S;


/**@brief Injecter Buffer参数结构定义*/
typedef struct
{
	int     m_nFIFOBufferSize;		///< Injecter的目标缓冲区或解码器缓冲区长度，如果该值为0则表示应用不关心长度，由驱动自行决定
}CSUDIINJECTERBuffer_S;

/**@brief Injecter Buffer状态信息结构定义*/
typedef struct
{
	int 	  m_nAvailableDataSize;			///< Injecter已经注入到目标缓冲区或解码器缓冲区中，但尚未被消耗掉的数据长度(单位字节)。这个数据初始值为0，能够在注入、解码过程中被查询
}CSUDIINJECTERStatus_S;

/**@brief Injecter 属性参数类型的枚举定义*/
typedef enum
{
	EM_UDIINJECTER_STATUS,		   ///< 获取INJECTER缓冲状态信息 具体参考CSUDIINJECTERStatus_S，该值为只读
	EM_UDIINJECTER_BUFFER,    	   ///< Injecter启动的buffer信息，具体请参见CSUDIINJECTERBuffer_S，该值为只读
	EM_UDIINJECTER_CALLBACK,	   ///< 设置Injecter回调函数，暂不用实现
	EM_UDIINJECTER_PES_SYNC,	   ///< 多个注入器之间的同步信息，具体请参见CSUDIPESSYNCMode_S，该值可读写	
	EM_UDIINJECTER_PCM_PARAMS,     ///< PCM注入播放器启动参数，具体请参见CSUDIINJECTERPcmStartParam_S，该值可读写
	EM_UDIINJECTER_PCM_ENDIAN,     ///< PCM数据注入数据的大小端设定，具体请参见CSUDIINJECTERPcmEndian_E，其默认值为平台推荐值，该值可读写
	EM_UDIINJECTER_IFRAME_PARAMS   ///< IFrame数据参数，具体请参见CSUDIINJECTERIFrameParam_S，该值可读写
}CSUDIINJECTERAttributeType_E;

/**@brief Injecter 注入通道的资源数据结构定义*/
typedef struct
{
	int m_nDemux;		    ///< Injecter用到的demux ID信息，在TS流注入的情况下，需要指明demux ID。-1代表无效值，如果没有用到Demux，则该参数可为-1
	int m_nVideoDecoder;        ///< Injecter用到的视频解码器ID信息，-1代表无效值，如果没有用到Video decoder，则该参数可为-1
	int m_nAudioDecoder;        ///< Injecter用到的音频解码器ID信息，-1代表无效值，如果没有用到Audio decoder，则该参数可为-1
	
}CSUDIINJECTERChnl_S;

/**@brief Injecter 注入IFrame数据参数 */
typedef struct
{
	const char *m_pcIFrameData;		    ///< I帧数据的地址
	int m_nDataLen;        							///< I帧数据的地址的长度
}CSUDIINJECTERIFrameParam_S;


/**@brief Injecter 注入通道类型结构定义*/
typedef struct 
{
 	CSUDIINJECTERType_E 		m_eInjecterType;  ///< Injecter实例类型，详见CSUDIINJECTERType_E
	CSUDIINJECTERContentType_E 	m_eContentType;  ///< Injecter 注入的媒体内容类型
}CSUDIINJECTEROpenParam_S;

/**
@brief 创建一个INJECTER注入实例,为INJECTER注入做好准备
@param[in] psInjecterChnl 数据注入通道数据结构指针。
@param[in] psOpenParams 将要创建的INJECTER实例的类型，主要包括PCM,TS,PES,ES等，详情请参见CSUDIINJECTERType_E
@param[out] phINJECTER 本次打开的INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note psInjecterChnl结构体中的设备索引可以为无效值(-1),代表该设备不需要或不存在
@note Demux必须先于Tuner断开连接，才能用于创建Inject，即一个Demux不能同时拥有两个源设备
@note 创建Inject须尽量使用最少的设备，如：
- 注入TS流的时候，注入内容只能是EM_UDIINJECTER_CONTENT_DEFAULT，否则返回CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- 注入PCM的时候，注入内容只能是EM_UDIINJECTER_CONTENT_AUDIO，否则返回CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- 注入TS流的时候，注入通道只能选DemuxID，音视频ID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- 注入PCM的时候，注入通道只能选AudioID，视频ID与DemuxID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- 注入I帧的时候，注入通道只能选VideoID,音频ID与DemuxID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- AVI,RM,RMVB, 等涉及播放多媒体文件播放的注入可不支持。如果芯片驱动对多媒体文件播放有更好的API，请告知同洲，
	同洲可对此API进行评估，如果合适，则封装上层接口直接调用其API.就不用再实现底层的注入支持。
*/
CSUDI_Error_Code  CSUDIINJECTEROpen(const CSUDIINJECTERChnl_S * psInjecterChnl, const CSUDIINJECTEROpenParam_S * psOpenParams, CSUDI_HANDLE * phINJECTER);

/**
@brief 关闭一个INJECTER注入实例,释放相关资源
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code  CSUDIINJECTERClose(CSUDI_HANDLE hINJECTER);


/**
@brief 设置INJECTER注入实例的属性信息，本函数调整INJECTER实例的属性、相关参数、数据源等信息。
@param[in] hINJECTER 注入实例句柄，由CSUDIINJECTEROpen返回
@param[in] eAttrType ，将要调整的属性类型，请参见CSUDIINJECTERAttributeType_E的详细说明
@param[in] pvData 具体的属性数据结构指针，内容请参见CSUDIINJECTERAttributeType_E的详细说明。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 如果已经调用CSUDIINJECTERStart启动数据注入，不允许再调用此接口对回调函数进行设置。
@note 如果不支持设置回调函数，调用此接口将返回不支持操作
*/
CSUDI_Error_Code CSUDIINJECTERSetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, const void * pvData);


/**
@brief 获取INJECTER注入实例的各属性信息。
@param[in] hINJECTER 注入实例句柄，由CSUDIINJECTEROpen返回
@param[in] eAttrType ，将要调整的属性类型，请参见CSUDIINJECTERAttributeType_E的详细说明
@param[out] pvData 具体的属性数据结构指针，内容请参见CSUDIINJECTERAttributeType_E的详细说明。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIINJECTERGetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, void * pvData);

/**
@brief  开始一个INJECTER 数据 注入
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果注入实例已经开始，则返回CSUDIREC_ERROR_ALREADY_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStart(CSUDI_HANDLE hINJECTER);


/**
@brief 停止一个INJECTER 数据注入
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果注入实例已经被停止，则返回CSUDIREC_ERROR_ALREADY_STOPPED
@note  如果注入实例没有启动，则返回CSUDIREC_ERROR_NOT_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStop(CSUDI_HANDLE hINJECTER);

/**
@brief等待Inecter已经注入到目标缓存区中的数据播放完成

本函数会堵塞，一直等到缓存中的数据播放完成了才返回CSUDI_SUCCESS，或下层驱动超时返回CSUDI_FAILURE
一般用于待播放数据全部注入完毕后，等待播放结束
@param[in] hINJECTER INJECTER数据注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  
*/
CSUDI_Error_Code CSUDIINJECTERFlush(CSUDI_HANDLE hINJECTER);

/**
@brief 清除Inecter已经注入到目标缓存区中的，尚没有播放完的全部数据

主要用于快进、快退、定位等操作时清空已注入的数据，已防止这些操作的延迟
@param[in] hINJECTER INJECTER数据注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  
*/
CSUDI_Error_Code CSUDIINJECTERClear(CSUDI_HANDLE hINJECTER);

/**
@brief 获取下次注入到Injecter目标缓冲区中的空闲Buffer地址指针和连续空闲Buffer的大小。
@param[in] hINJECTER INJECTER数据注入实例句柄
@param[out] ppvBuffer 返回获取到的内存地址指针
@param[out] puLength 返回获取到的内存长度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果buffer已经满，再去调用此接口应该返回失败，且puLength长度为0
@note  该函数必须与CSUDIINJECTERWriteComplete配对使用，对该函数的调用要考虑线程安全
@code 
	char  	*pcBuf = NULL; 
	unsigned int unBufSize = 0;
	...
	if( (CSUDIINJECTERGetFreeBuffer(hInject, (void *)&pcBuf, &unBufSize) == CSUDI_SUCCESS) && (unBufSize > 0))
	{
		//往buffer中写入内容,  此处假设unWriteLen <= unBufSize
		memcpy(pcBuf, pcDataTobeWrite, unWriteLen );		
		CSUDIINJECTERWriteComplete(hInject, unWriteLen);
	}
	...
@endcode
*/
CSUDI_Error_Code CSUDIINJECTERGetFreeBuffer(CSUDI_HANDLE hINJECTER, void ** ppvBuffer, unsigned  int * puLength );

/**
@brief 通知本次注入的数据
@param[in] hINJECTER INJECTER数据注入实例句柄
@param[in] uWrittenSize 已拷贝数据的大小
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 如果buffer长度为nLength, 注入长度大于nLength,则该接口返回CSUDIINJECTER_ERROR_NO_MEMORY
@note  该函数必须与CSUDIINJECTERGetFreeBuffer配对使用，对该函数的调用要考虑线程安全
*/
CSUDI_Error_Code CSUDIINJECTERWriteComplete(CSUDI_HANDLE hINJECTER, unsigned int uWrittenSize);



#ifdef  __cplusplus
}
#endif
/**@}*/
#endif
