/**@defgroup VIDEO VIDEO  定义了平台无关的视频驱动接口
@brief VIDEO模块对应头文件<udi2_video.h>，包括视频解码器属性及控制接口 。

一个可以并且需要独立工作的视频解码器抽象为一个Video解码器。如pip系统抽象出两个解码器，而在非pip系统中，即使Mpeg2和Mpeg4是两个解码器在工作，也仅抽象成一个Video解码器。

I帧的解码通过创建一个inject，一个player，然后将数据从inject注入，用player进行播放, - 再将其Snapshot，如需获得其大小信息可通过GetStreamStatus.

Video解码器的索引从0开始,一般来说0表示主解码器。副解码器的视频总是叠在主解码器视频的上面，然后共同混合到Screen上，除非它被Hide。

除此之外，视频在解码时还会进行一些内容上的变化处理，如拉升缩放、改变分辨率、增加黑边等等。这些都由视频解码器完成。

@version 2.0.4 2009/09/24 增加设置裁剪等矩形的参考系说明
@version 2.0.3 2009/09/15 增加屏幕适应模式的图示说明
@version 2.0.2 2009/08/27 去掉对播放jpg图片的支持
@version 2.0.2 2009/08/25 增加Video抽象规则及播放jpg等功能说明
@version 2.0.1 2009/08/16 初稿
@{
*/
#ifndef _UDI2_VIDEO_H_
#define _UDI2_VIDEO_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_screen.h"
#include "udi2_osg.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 视频设备操作相关错误代码值*/
enum
{
	CSUDIVIDEO_ERROR_BAD_PARAMETER = CSUDI_VIDEO_ERROR_BASE,  ///< 参数错误
	CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED,					///< 不支持操作
	CSUDIVIDEO_ERROR_UNKNOWN_ERROR,					   		///< 一般错误
	CSUDIVIDEO_ERROR_NO_MEMORY,					    		///< 无可用内存
	CSUDIVIDEO_ERROR_INVALID_DEVICE_ID,					    ///< 非法设备ID
	CSUDIVIDEO_ERROR_CALLBACK_EXIST,         					///< 要注册的回调已经存在
	CSUDIVIDEO_ERROR_CALLBACK_NOT_EXIST 					///< 要删除的回调不存在
};

/**@brief VIDEO设备能力信息
@note 数组中，如果碰到类型为EM_UDI_VID_STREAM_INVALID，则表示支持的视频类型已经结束，后续数据将被忽略。
*/
typedef struct
{
	  CSUDIVIDStreamType_E   m_eStreamType[EM_UDI_VID_STREAMTYPE_NUM];	   ///< 设备支持的视频码流格式
} CSUDIVIDEOCapability_S;

/**@brief 视频停止模式*/
typedef enum
{	
	EM_UDIVIDEO_STOPMODE_BLACK,	///< 黑屏 
	EM_UDIVIDEO_STOPMODE_FREEZE	///< 静帧 
	
}CSUDIVIDEOStopMode_E;

/**@brief 视频解码信息*/
typedef struct 
{
	CSUDIVIDStreamType_E 	m_eVidStreamType;		///< 视频流类型
	int 			                	m_nSourceWidth;		///< 源视频流宽度
	int 			                	m_nSourceHight;		    	///< 源视频流高度
	int 			                	m_nFrameRate;		    	///< 源视频流码率
	CSUDI_BOOL	                	m_bInterlaced;		    	///< CSUDI_TRUE表示隔行;CSUDI_FALSE表示逐行
	int			                	m_dwFrameCount;		///< 视频已播放的总帧数
}CSUDIVIDEOStreamStatus_S;

/**@brief 视频宽高比*/
typedef enum 
{
	EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN,  
	EM_UDIVIDEO_ASPECT_RATIO_4_3,	        ///< 4：3
	EM_UDIVIDEO_ASPECT_RATIO_16_9,	        ///< 16：9 
	EM_UDIVIDEO_ASPECT_RATIO_AUTO		 ///<  自动选择 
}CSUDIVIDEOAspectRatioType_E;

/**
@brief 视频输出宽高比转换机制,请参考视频输出宽高比转换示例图
@see CSUDIVIDEOSetMatchMethod
*/
typedef enum 
{
	EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,	///<  Letter Box 
	EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,	///<  Pan Scan 
	EM_UDIVIDEO_MATCH_METHOD_COMBINED,	///<  ComBined 
	EM_UDIVIDEO_MATCH_METHOD_IGNORE,	      ///< Ignore 
	EM_UDIVIDEO_MATCH_METHOD_NUM                 ///< 上限。
}CSUDIVIDEOMatchMethod_E;

/**@brief VBI 功能类型*/
typedef enum 
{   
    EM_UDIVIDEO_VBI_TELTEXT=(1<<0),   ///< teltext
    EM_UDIVIDEO_VBI_CLOSECAPTION=(1<<1),///<VBI CLOSE CAPTION
    EM_UDIVIDEO_VBI_MACROVISION=(1<<2), ///< Macrovision
    EM_UDIVIDEO_VBI_CGMS=(1<<3) 		   ///< CGMS
}CSUDIVIDEOVBIType_E;

/**@brief 视频事件枚举定义*/
typedef enum 
{	
	EM_UDIVIDEO_USERDATA,		    	///< 视频解码器user data事件，回调中pvEventData指向CSUDIVIDEOUserData_S结构体
	EM_UDIVIDEO_MAXEVENTTYPE		///< 边界值
}CSUDIVIDEOEventType_E; 

/**@brief video中的user data数据结构定义*/
typedef struct
{
	void *			m_pVideoUserdata;				///< UserData数据Buffer地址
	unsigned int		m_uLength;						///< UserData长度
	CSUDI_UINT64 	m_u64Pts;						///< PTS 
	CSUDI_BOOL		m_bIsRegistered;					///< 表示是否为已注册的UserData，为TRUE则结构体成员(m_ucCountryCode, m_ucCountryCodeExtensionByte & m_uProviderCode)有效
	CSUDI_UINT8		m_ucCountryCode;				///< Country code following ITU-T recommendation T.35
	CSUDI_UINT8		m_ucCountryCodeExtensionByte;	///< Country code extension byte. 注意：仅当m_ucCountryCode为0xFF有效
	CSUDI_UINT16	m_uProviderCode;				///< manufacturers code
	unsigned int		m_adwReserved[4];				///< 保留字段
} CSUDIVIDEOUserData_S;

 /**
@brief video回调函数的定义

@param[in] nVideoIndex 视频解码器的索引；
@param[in] eEvent 回调消息类型，详见CSUDIVIDEOEventType_E定义；
@param[in] pvEventData回调消息数据，详见CSUDIVIDEOEventType_E各枚举具体定义；
@param[in] pvUserData  用户数据类型
*/
typedef  void ( *CSUDIVIDEOCallback_F  )(int nVideoIndex, CSUDIVIDEOEventType_E eEvent, void * pvEventData, void * pvUserData);

 /**
@brief  注册指定事件类型的回调函数

@param[in] nVideoIndex 视频解码器的索引
@param[in] fnVideoCallback 回调函数指针，详见CSUDIVIDEOCallback_F定义
@param[in] eEvent 回调函数类型，详见CSUDIVIDEOEventType_E
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 至少支持注册的回调函数个数为32个，最大不限
@note 不允许注册EM_UDIVIDEO_MAXEVENTTYPE类型的回调
@note 回调函数指针+回调函数类型+用户数据唯一标识一个回调,当两次注册它们完全一样时，第二次将返回CSUDIVIDEO_ERROR_CALLBACK_EXIST
*/
CSUDI_Error_Code CSUDIVIDEOAddCallback(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent,void * pvUserData );

 /**
@brief 删除指定事件类型的回调函数

@param[in] nVideoIndex 视频解码器的索引
@param[in] fnVideoCallback 回调函数指针，详见CSUDIVIDEOCallback_F定义
@param[in] eEvent 回调函数类型，详见CSUDIVIDEOEventType_E
@param[in] pvUserData 同CSUDIVIDEOAddCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIVIDEO_ERROR_CALLBACK_NOT_EXIST
@note 允许在回调函数中删除回调
*/
CSUDI_Error_Code CSUDIVIDEODelCallback(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent, void * pvUserData );
 
/**
@brief 控制VBI方式的teletext\close caption\Macrovision 功能开关

@param[in] nVideoIndex 视频解码器的索引
@param[in] eVideoVBIType VBI功能类型，可以组合控制
@param[in] bEnable 打开或关闭eVideoVBIType对应的功能
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 系统启动时所有功能默认关闭，只有调用本接口才能打开或关闭对应功能，设置一次永久生效
@note 当功能为EM_UDIVIDEO_VBI_TELTEXT时，需要同player接口CSUDIPLAYERSetStream配合使用,eg:   
@note 启动VBI teltext 功能
@note  sStreamInfo.m_eContentType=EM_UDI_CONTENT_TELTEXT;
@note  sStreamInfo.m_nPid=teltext_pid;
@note  CSUDIPLAYERSetStream(hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL);
@note  CSUDIVIDEOSetVBIEnable(nVideoIndex,EM_UDIVIDEO_VBI_TELTEXT, CSUDI_TRUE);
@note  其它功能因不需要设置PID,故只需调用CSUDIVIDEOSetVBIEnable接口
*/
CSUDI_Error_Code CSUDIVIDEOSetVBIEnable(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL bEnable);

/**
@brief 获取VBI各个功能teletext\close caption\Macrovision 开关状态

@param[in] nVideoIndex 视频解码器的索引
@param[in] eVideoVBIType VBI功能类型，每次只能查询一个功能
@param[out] pbEnable ,eVideoVBIType对应的功能开关状态
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVIDEOGetVBIEnable(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL* pbEnable);

/**
@brief 获取Video Decoder设备的总数

@param[out] pnVideoDecCount 系统中 存在Video Decoder设备的个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，这种情况下pnVideoDecCount的内容不确定
*/
CSUDI_Error_Code CSUDIVIDEOGetCount(int * pnVideoDecCount);

/**
@brief 获取指定Video Decoder设备的能力信息

无论空闲或者被占用
@param[in] nVideoIndex Video Decoder设备的索引
@param[out] psCapabilityInfo Video Decoder设备能力信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，这种情况下psCapabilityInfo结构的内容不确定
*/
CSUDI_Error_Code CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo);

/**@brief 视频截图(快照)接口，将当前正在显示的视频图像的部分(或全部)截取并输出到目标图像的指定区域。

@param[in] nVideoIndex  Video Decoder设备的索引
@param[in] psSrcRect 输入窗口大小，表示要截取当前视频图像的哪一部分，为CSUDI_NULL表示整个图像，注意不能超出当前视频图像大小。
@param[in] psDstRect 输出窗口大小，表示输出到hSurface的哪一部分，为CSUDI_NULL表示整个hSurface，注意不能超出hSurface大小。
@param[out] phSurface 视频截图将输出到hSurface的指定区域，该Surface是调用OSG接口创建的缓存或是直接获取显存；
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且hSurface结构的内容未定义。
@note 在截取I帧图片时，通过调用CSUDIVIDEOGetStreamStatus获取当前I帧信息，例如图像宽高信息。
@note 如果产品不需要视频快照功能或者硬件不支持，该接口可以不实现。此时需返回CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED。
*/
CSUDI_Error_Code CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect, CSUDI_HANDLE* phSurface);

/**
@brief 获取解码器播放状态信息

如当前播放的视频大小等
@param[in]  nVideoIndex Video Decoder设备资源的索引
@param[out] psStreamStatus 捕获IFRAME信息 见CSUDIVIDStreamStatus_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psStreamStatus结构的内容未定义
*/
CSUDI_Error_Code CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus);

/**
@brief 显示/隐藏视频解码器的输出

@param[in] nVideoIndex  视频解码器的索引
@param[in] bIsShow 为CSUDI_TRUE则表示显示该窗口;为CSUDI_FALSE则表示隐藏该窗口
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值 
@note 系统初始化时应为显示状态
*/
CSUDI_Error_Code CSUDIVIDEOShow(int nVideoIndex, CSUDI_BOOL bIsShow);

/**
@brief 设置指定视频输出的屏幕宽高比

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 指定视频输出屏幕
@param[in] eAspectRatio 视频输出接口的宽高比，如4:3 或 16:9
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note auto表示根据节目的宽高比自动设置屏幕宽高比，也即将宽高比效果交给底层驱动处理
@note 设置指定视频输出接口输出画面的宽高比，目前只支持16:9/4:3/auto等模式。
具体效果依赖于所播放的节目的宽高比以及指定视频输出接口的适应模式(MatchMethod)等因素
@note 此接口设置的显示效果只在视频全屏显示时有效，在视频小窗口显示时不起作用，即无论此接口如何设置，视频小窗口显示时始终是填满小窗口的
*/
CSUDI_Error_Code CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio);

/**
@brief获取指定视频输出的屏幕宽高比

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 指定视频输出屏幕
@param[out] peAspectRatio 视频输出接口的宽高比，如4:3 或 16:9
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 本接口用于获取当前设置的屏幕宽高比状态，如果设置的是AUTO，则返回AUTO，
流的情况由CSUDIVIDEOGetStreamStatus接口获得
*/
CSUDI_Error_Code CSUDIVIDEOGetAspectRatio(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E *peAspectRatio);


/**
@brief设置指定视频输出接口的适应模式

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 指定视频输出屏幕
@param[in] eMatchMethod 指定视频输出端口的适应模式，如PAN_SCAN,LETTER_BOX,等
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置指定视频输出端口的屏幕适应模式，
具体效果依赖于所播放的节目的宽高比以及指定视频输出接口的画面比(AspectRatio)等因素，请参考下图：
@image HTML VIDAspectRatioType.png
@note 此接口设置的显示效果只在视频全屏显示时有效，在视频小窗口显示时不起作用，即无论此接口如何设置，视频小窗口显示时始终是填满小窗口的
*/
CSUDI_Error_Code CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E eMatchMethod);

/**
@brief获取指定视频输出接口的适应模式

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 指定视频输出屏幕
@param[out] peMatchMethod 指定视频输出端口的适应模式，如PAN_SCAN,LETTER_BOX,等
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVIDEOGetMatchMethod(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E *peMatchMethod);

/**
@brief 设置停止视频的模式

即节目切换时，视频切换效果，如静帧、黑屏
@param[in] nVideoIndex  Video Decoder设备资源的索引
@param[in] eStopMode 停止模式，详情请参考CSUDIVIDEOStopMode_E。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode);

/**
@brief获取停止视频的模式

即节目切换时，视频切换效果，如静帧、黑屏
@param[in] nVideoIndex  Video Decoder设备资源的索引
@param[out] peStopMode 停止模式，详情请参考CSUDIVIDEOStopMode_E。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVIDEOGetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E *peStopMode);

/**
@brief 设置指定视频解码输出的窗口大小，实现缩放功能

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 设置在哪个屏幕上的视频窗口大小
@param[in] pstRect 窗口的大小以及窗口位置，参看CSUDIWinRect_S结构；如果pstRect为NULL，则显示为全屏。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note pstRect为相对与当前视频分辨率的矩形,该信息可以从CSUDISCREENGetResolution接口获取
@note 不考虑裁剪矩形为负数的情况
*/
CSUDI_Error_Code CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect );

/**
@brief 获取指定视频解码输出的窗口大小

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 设置在哪个屏幕上的视频窗口大小
@param[out] pstRect 窗口的大小以及窗口位置，参看CSUDIWinRect_S结构；
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVIDEOGetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * pstRect );

/**
@brief 设置窗口剪切位置和尺寸

设置后整个视频窗口仅显示设置区域的视频，用于实现局部放大缩小等操作，
即设置后将制定区域在整个Screen上显示(如果没有设置过CSUDIVIDEOSetWindowSize则为全屏显示)
@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 设置在哪个屏幕上的视频剪切区域
@param[in] psRect 指定剪切的区域，详见CSUDIWinRect_S定义。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 如果psRect为CSUDI_NULL，则表示全屏显示
@note pstRect为相对与当前视频分辨率的矩形,该信息可以从CSUDISCREENGetResolution接口获取
*/
CSUDI_Error_Code CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect);

/**
@brief 获取窗口剪切位置和尺寸

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 设置在哪个屏幕上的视频剪切区域
@param[out] psRect 剪切的区域，详见CSUDIWinRect_S定义。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVIDEOGetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * psRect);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif
