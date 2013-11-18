/**@defgroup CSVideo video 提供统一的视频解码相关接口
@brief 提供统一的视频解码相关接口

@version 0.3 2009/04/27 增加CSVID_TIMING_MODE_1080P_50HZ
@version 0.2 2008/10/30 添加详细描述，分离剩余的初始化接口
@version 0.1 2008/10/23 初始版本
@{
*/

#ifndef _UDI_SYSVIDEO_H_
#define _UDI_SYSVIDEO_H_

#include "udi_typedef.h"
#include "udi_public.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 定义视频正常播放速度*/
#define CS_VID_NORMAL_SPEED 		(1000)

/**@brief 视频宽高比*/
typedef enum _CSVIDAspectRatioType
{
	CSVID_ASPECT_RATIO_UNKNOWN,
	CSVID_ASPECT_RATIO_4TO3,	/**< 4：3*/
	CSVID_ASPECT_RATIO_16TO9,	/**< 16：9*/
	CSVID_ASPECT_RATIO_AUTO		/**< 自动选择*/
}CSVIDAspectRatioType;

/**@brief 视频输出宽高比转换机制
@image HTML VIDAspectRatioType.png
*/
typedef enum _CSVIDMatchMethod
{
	CSVID_MATCH_METHOD_LETTER_BOX,	/**< Letter Box*/
	CSVID_MATCH_METHOD_PAN_SCAN,	/**< Pan Scan*/
	CSVID_MATCH_METHOD_COMBINED,	/**< ComBined*/
	CSVID_MATCH_METHOD_IGNORE,	    /**< Ignore*/
	CSVID_MATCH_METHOD_NUM
}CSVIDMatchMethod;

/**@brief 视频流编码格式，需注意与打包格式(TS、ES、AVI、ASF等区别)*/
typedef enum _CSVIDStreamType
{
	CSVID_STREAM_MPEG1,			/**< MPEG-1 Video (ISO/IEC 11172-2) */
	CSVID_STREAM_MPEG2,			/**< MPEG-2 Video (ISO/IEC 13818-2) */
	CSVID_STREAM_MPEG4,			/**< MPEG-4 Part 2 Video */
	CSVID_STREAM_H264,			/**< H.264 (ITU-T) or ISO/IEC 14496-10/MPEG-4 AVC */
	CSVID_STREAM_H263,			/**< H.263 Video. The value of the enum is not based on PSI standards. */
	CSVID_STREAM_VC1,			/**< VC-1 Advanced Profile */
	CSVID_STREAM_VC1_SM,		/**< VC-1 Simple&Main Profile */
	CSVID_STREAM_DIVX_311,		/**< DivX 3.11 coded video */
  	CSVID_STREAMTYPE_NUM
}CSVIDStreamType;

/**@brief 视频输出接口*/
typedef enum _CSVIDOutputDevice
{
	CSVID_OUTPUT_CVBS	=0x00000001,	/**< video cvbs BNC 输出接口*/
	CSVID_OUTPUT_YC		=0x00000002,	/**< S端子输出接口*/
	CSVID_OUTPUT_RGB	=0x00000004,	/**< video R,G, B, component output interface*/
	CSVID_OUTPUT_YUV	=0x00000008,	/**< video Y, Cb, Cr output interface*/
	CSVID_OUTPUT_RF		=0x00000010,	/**< 保留*/
	CSVID_OUTPUT_SCART	=0x00000020,	/**< 保留*/
	CSVID_OUTPUT_HDMI	=0x00000040,	/**< hdmi 接口*/
	CSVID_OUTPUT_DVO	=0x00000080	    /**< 数字视频输出*/
}CSVIDOutputDevice;

/**@brief 视频输出分辨率*/
typedef enum _CSVIDTimingMode
{
	CSVID_TIMING_MODE_SLAVE,
	CSVID_TIMING_MODE_NTSC,  		/**< SD: CSMIX_ENCODING_MODE_NTSCM*/ 
	CSVID_TIMING_MODE_PAL, 			/**< SD: CSMIX_ENCODING_MODE_PALBDGHI*/
	CSVID_TIMING_MODE_576P, 		/**< Australian mode*/
	CSVID_TIMING_MODE_720P,  		/**< HD: EIA770.3 #1 P60 = SMPTE 296M #1 P60*/
	CSVID_TIMING_MODE_720P_50HZ,
	CSVID_TIMING_MODE_1080I,  		/**< HD: EIA770.3 #3 I60 = SMPTE274M #4 I60*/ 		
	CSVID_TIMING_MODE_1080I_50HZ,	/**< HD:  SMPTE 274M   I50*/
	CSVID_TIMING_MODE_AUTO,			/**< new add, ref 7710. zzp, 2006.9.18*/
	CSVID_TIMING_MODE_NTSC_443,		/**< NTSC M mode but with 4.43361875MHz color subcarrier frequency.*/
	CSVID_TIMING_MODE_NTSC_JAPAN,	/**< Japan NTSC, no pedestal level*/
	CSVID_TIMING_MODE_PAL_M,        /**< PAL Brazil*/
	CSVID_TIMING_MODE_PAL_N,        /**< PAL_N*/
	CSVID_TIMING_MODE_PAL_NC,       /**< PAL_N, Argentina (PAL N mode but with 3.58205625 MHz color subcarrier frequency and no pedestal (= 0 IRE blanking setup))*/
	CSVID_TIMING_MODE_PAL_B,		/**< Australia*/
	CSVID_TIMING_MODE_PAL_B1,		/**< Hungary*/
	CSVID_TIMING_MODE_PAL_D,		/**< China ==CSVID_TIMING_MODE_PAL*/
	CSVID_TIMING_MODE_PAL_D1,		/**< Poland*/
	CSVID_TIMING_MODE_PAL_G,		/**< Europe. Same as CSVID_TIMING_MODE_PAL.*/
	CSVID_TIMING_MODE_PAL_H,		/**< Europe*/
	CSVID_TIMING_MODE_PAL_K,		/**< Europe*/
	CSVID_TIMING_MODE_PAL_I,		/**< U.K.*/
	CSVID_TIMING_MODE_SECAM,        /**< SECAM III B6*/
	CSVID_TIMING_MODE_480P,         /**< NTSC Progressive (27Mhz)*/
	CSVID_TIMING_MODE_1080P,        /**< HD 1080 Progressive, 60Hz*/
	CSVID_TIMING_MODE_1080P_24HZ,	/**< HD 1080 Progressive, 24Hz*/
	CSVID_TIMING_MODE_1080P_25HZ,	/**< HD 1080 Progressive, 25Hz*/
	CSVID_TIMING_MODE_1080P_30HZ,	/**< HD 1080 Progressive, 30Hz*/
	CSVID_TIMING_MODE_1250I_50HZ,   /**< HD 1250 Interlaced, 50Hz*/
	CSVID_TIMING_MODE_720P_24HZ,    /**< HD 720p 24Hz*/
	CSVID_TIMING_MODE_VESA,			/**< PC monitor. Requires width, height and refresh rate parameters*/
	CSVID_TIMING_MODE_1080P_50HZ, /**< HD 1080 Progressive, 50Hz*/
	CSVID_TIMING_MODE_NUM
} CSVIDTimingMode;

/**@brief 视频输出层枚举*/
typedef enum _CSVIDWindow
{
	CSVID_WINDOW_VIDEO,			/**< 视频窗口*/
	CSVID_WINDOW_OSD,			/**< osd 窗口*/
	CSVID_WINDOW_BACKGROUND,	/**< 背景窗口*/
	CSVID_WINDOW_ALLWINDOW
}CSVIDWindow;


/**@brief 视频显示模式*/
typedef enum _CSSYSVideoShowType
{
	CS_MODE_NORMAL = 0,	/**< 正在播放当前节目*/
	CS_MODE_BLACK,  	/**< 黑屏*/
	CS_MODE_FREEZE		/**< 静帧*/
}CSSYSVideoShowType;

/**@brief 视频停止模式*/
typedef enum _CSVIDStopMode	
{	
	CSVID_STOPMODE_BLACK,	/**< 黑屏*/
	CSVID_STOPMODE_FREEZE,	/**< 静帧*/	
	CSVID_STOPMODE_FIFO	    /**< 淡入淡出*/
}CSVIDStopMode;

/**@brief 视频事件*/
typedef enum _CSSYSVidEvent_E
{
	EM_VID_NO_EVT	=0x00000000,			    /**< 没有任何事发生*/
	EM_VID_SOURCE_CHANGED_EVT=0x00000001,		/**< 视频源变化*/
	EM_VID_ASPACTRADIO_CHANGED_EVT=0x00000002,	/**< 宽高比转换机制变化*/
	EM_VID_TIMINGMODE_CHANGED_EVT=0x00000004,	/**< 输出分辨率变化*/
	EM_VID_OVERFLOW_EVT=0x00000008,			    /**< 缓存溢出*/
	EM_VID_UNDERFLOW_EVT=0x00000010,		    /**< 缓存数据耗尽，即没有数据播了*/
	EM_VID_PAKET_START =  0x000000020,		    /**< 需添加注释*/
	EM_VID_PAKET_STOP = 0x000000040,		    /**< 需添加注释*/
	EM_VID_ALL_EVT=0x0000007f			        /**< 事件上限*/
}CSSYSVidEvent_E;

/**@brief 视频状态*/
typedef struct _CSSYSVIDStatus_S
{
	BOOL 			m_bWindowEnable;	    /**< 视频显示窗口是否打开*/	
	CSVIDStreamType		m_eVidStreamType;	/**< 视频流类型*/
	int 			m_nSourceWidth;		    /**< 源视频流宽度*/
	int 			m_nSourceHight;		    /**< 源视频流高度*/
	int 			m_nFrameRate;		    /**< 源视频流码率*/
	BOOL			m_bInterlaced;		    /**< TRUE表示隔行;FALSE表示逐行*/
	
	CSVIDAspectRatioType 	m_eAspectRatio;		/**< 视频宽高比*/
	CSVIDMatchMethod 	m_eConversion;		    /**< 视频输出宽高比转换机制*/
	CSVIDTimingMode		m_eTimingModeSD;	    /**< 标清设备视频输出分辨率*/
	CSVIDTimingMode		m_eTimingModeHD;	    /**< 高清设备视频输出分辨率*/
	DWORD			m_dwFrameCount;		        /**< 视频已播放的总帧数*/
	DWORD 			m_dwReserved1;		        /**< 保留参数，须填0*/
	DWORD			m_dwReserved2;		        /**< 保留参数，须填0*/
}CSSYSVIDStatus_S;

/**@brief 视频输入信息*/
typedef struct _CSSYSVIDInputInfo
{
	CSVIDStreamType	StreamType;		    /**< 视频流类型*/
	int 			nFrameRate;		    /**< 视频码率: 25/30,0表示未知*/
	int 			nWidth;			    /**< 视频宽度:702/720/1920,0表示未知*/
	int 			nHight;			    /**< 视频高度: 480/576/1080,0表示未知*/
	CSVIDAspectRatioType 	AspectRatio;/**< 视频宽高比: 4:3/16:9,0表示未知*/
	int 			        nPID;		/**< 视频流PID*/
	CSSYSServiceType	    ServiceType;/**< 视频数据类型：PES or ES*/							
	int                     nFirstSpeed;/**< 启动播放的初始速度, CS_VID_NORMAL_SPEED 为正常速度*/
}CSSYSVIDInputInfo;	

/**@brief 视频区域矩形*/
typedef struct _CSWINRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
}CSWINRECT;

/**@brief 视频层信息*/
typedef struct _CSVIDWindowInfo
{
	CSVIDWindow		nWindowIndex;	/**< 视频所在层*/
 	BOOL 			nShow;			/**< 视频显示状态*/
	BOOL 			nFullSize;		/**< 是否全屏*/
	CSWINRECT*		pstRect;		/**< 显示区域*/
	int 			nalpha;			/**< 视频输出透明度（范围？）*/
}CSVIDWindowInfo;

/**@brief 视频输出信息*/
typedef struct _CSSYSTVOutputInfo
{
	CSVIDOutputDevice	OutputDevice;		/**< 视频输出设备*/
	CSVIDTimingMode		Resolution;		    /**< 视频输出分辨率*/
	CSVIDWindowInfo*	pstWindowInfo;		/**< 视频层信息*/
	int 			    nBrightness;		/**< 亮度*/
	int 			    nContrast;		    /**< 对比度*/
	int 			    nSaturation;		/**< 饱和度*/
	int 			    nPositionH;		    /**< tv output PositionH*/
	int 			    nPositionY;		    /**< tv output PositionV*/
}CSSYSTVOutputInfo;

/**
@brief 视频显示事件回调函数

该函数主要用于通知视频播放过程中的事件，目前仅会通知CSSYSEVENT_FRONTTS_RUN和CSSYSEVENT_FRONTTS_STOP，
用于停播检测功能，停播以音视频同时为准，暂时不能调整
只在播放时有效,即如果主动调用Stop停止播放,不会来CSSYSEVENT_FRONTTS_STOP消息
@param[in] enmEventMsg 发生的事件
@param[in] pvParam 用户数据，注册时转入的数据
*/
typedef void (*CSSYSShowVideoCallback)(CSSYSEventMsg enmEventMsg, PVOID pvUserData );

/**
@brief 注册视频显示事件回调函数

用于在发生显示相关的事件时通知用于，如前端停播等
@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来。具体参看设备关联接口文档定义。
@param[in] enmEventMsg 注册的消息，目前仅支持CSSYS_MSG_FRONT_TS
@param[in] pfnCallback 注册的回调函数。
@param[in] pvUserData 用户自定义的数据。
@return 成功返回TRUE,失败返回FALSE
@see CSSYSShowVideoByEventMsg CSSYSShowVideoSetEventMask CSSYSShowVideoGetEventMask
*/
BOOL CSSYSVIDRegisterVideoShowCallback(HCSHANDLE hHandle,  CSSYSRegisterMsg enmEventMsg,CSSYSShowVideoCallback pfnCallback, PVOID pvUserData);

/**
@brief 通知底层，应用欲操作视频显示功能（如黑屏、静桢还是正常显示）对应的事件类型

当上层检测到某个事件后(通过CSSYSShowVideoCallback回调或者CA、高频头等其它模块)调用本函数用于控制视频的状态
本模块需要控制各种不同消息的优先级和状态
@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[in] enmEventMsg 对应的事件类型
@param[in] enmVideoShowType 发生该事件时的处理方式，如黑屏等
@return 成功返回TRUE,失败返回FALSE.
@see CSSYSVIDRegisterVideoShowCallback CSSYSShowVideoSetEventMask CSSYSShowVideoGetEventMask
*/
BOOL CSSYSShowVideoByEventMsg( HCSHANDLE hHandle, CSSYSEventMsg enmEventMsg, CSSYSVideoShowType enmVideoShowType );

/**
@brief  设置对video开关起控制作用的事件的Mask

用于控制模块当前对某个事件的响应情况，某些情况下如某个特殊界面，不需要响应某些消息
@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[in] nEvent 对应的事件类型，在CSSYSEventMsg中的取值
@param[in] dwMask 事件的Mask（或者说是属性），如该事件是否有效等，为CSSYSEventMask
@return 成功返回TRUE,失败返回FALSE
@note 使用该接口可以实现禁用或启用一个Event类型对video开关的控制，也可以设置该Event类型是否节目相关。
一个Event类型禁用之后，如果再针对该Event调用CSSYSShowVideoByEventMsg
这些事件的状态仍然会被记录，但不参与视频的开关控制。
当有一个事件禁用或重新使用，视频的开关状态并不会立即改变，
直到CSSYSShowVideoByEventMsg被调用。

@code 
//禁用CSSYSEVENT_SIGNAL_LOCK对video的控制：
DWORD dwMask = CSSYSShowVideoGetEventMask(CSSYSGetNexyHandle(0), CSSYSEVENT_SIGNAL_LOCK);
dwMask &= ~CSSYSEVENT_MASK_ACTIVE;
CSSYSShowVideoSetEventMask(CSSYSGetNextHandle(0), CSSYSEVENT_SIGNAL_LOCK, dwMask);

//启用CSSYSEVENT_SIGNAL_LOCK对video的控制：
DWORD dwMask = CSSYSShowVideoGetEventMask(CSSYSGetNexyHandle(0), CSSYSEVENT_SIGNAL_LOCK);
dwMask |= CSSYSEVENT_MASK_ACTIVE;
CSSYSShowVideoSetEventMask(CSSYSGetNextHandle(0), CSSYSEVENT_SIGNAL_LOCK, dwMask);
@endcode
@see CSSYSVIDRegisterVideoShowCallback CSSYSShowVideoByEventMsg CSSYSShowVideoGetEventMask
*/
BOOL CSSYSShowVideoSetEventMask(HCSHANDLE hHandle, int nEvent, DWORD dwMask); 

/**
@brief  获取对video开关起控制作用的事件的Mask

@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[in] nEvent 对应的事件类型，在CSSYSEventMsg中的取值
@return 事件的Mask（或者说是属性），如该事件是否有效等，为CSSYSEventMask
@see CSSYSVIDRegisterVideoShowCallback CSSYSShowVideoByEventMsg CSSYSShowVideoSetEventMask
*/
DWORD CSSYSShowVideoGetEventMask(HCSHANDLE hHandle, int nEvent); 

/**
@brief 设置节目video/pcr信息

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] pstVIDInfo 输入节目的视频信息
@param[in] pstPCRInfo 输入节目的PCR信息，如果无PCR信息可以留空，PCRPID非法相当于无PCR，仍可正常设置
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 供用户设置video/pcr节目信息，包括流的类型，格式，pid等，如果不设置诸如视频类型，则按照默认类型操作；	
*/
BOOL CSSYSVIDSetInput(HCSHANDLE hHandle, CSSYSVIDInputInfo *pstVIDInfo,CSSYSPCRInputInfo*pstPCRInfo,HCSHANDLE hReserved );

/**
@brief 启动视频播放

@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 按照CSVIDSetInput()等设置函数设置的视频pid及视频的相关特性参数启动视频的播放.
@note 非Stop状态下再次调用Play返回TRUE，但不重新开始播放,新设置的Pid也不生效
*/
BOOL CSSYSVIDPlay(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief 停止视频播放

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 停止视频播放；与CSVIDPlay成对使用。
@note Stop状态下再次调用Stop返回TRUE
*/
BOOL CSSYSVIDStop(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief 暂停视频解码

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 暂停与hHandle相关的视频的播放，包括解码和显示。只有正在非Stop状态下调用才有效,又即已Pause时再次调用返回TRUE
@note 如果是实时流播放则静帧,恢复后不需要原地继续播放;如果是注入播放则暂停并可在原地恢复
@note VOD系统一般会在前端进行暂停操着，此情况下无需调用本系列接口
*/
BOOL CSSYSVIDPause(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief 恢复暂停模式，播放从暂停刻开始起的视频流

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 恢复暂停模式;与CSSYSVIDPause成对使用
@note 已Resume状态下再次Resume仍返回TRUE
*/
BOOL CSSYSVIDResume(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief 设置指定层的屏幕宽高比

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] enmAspectRatio 视频输出端口的宽高比，如4:3 或 16:9
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 自动表示根据节目的宽高比自动设置屏幕宽高比，也即将宽高比效果交给显示设备处理
@note 设置指定层的屏幕宽高比，目前只支持16:9/4:3。具体效果需要根据所播放的节目的宽高比，
以及设置的适应模式，以及电视机是否有自适应功能来确定。目前只支持video层。
@note 一般来说,电视机遇到与自身屏幕宽高比不一致的节目源时只会以缩放模式进行处理。
按此思路即可得到各种组合应有的显示效果,但也有少量电视有调整功能,需额外注意期望效果
*/
BOOL CSSYSVIDSetAspectRatio(HCSHANDLE hHandle, CSVIDAspectRatioType enmAspectRatio, HCSHANDLE hReserved );

/**
@brief 设置停止视频的模式

即换台时的视频切换效果，如静帧、黑屏
@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] eStopMode 停止模式
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSYSVIDSetStopMode(HCSHANDLE hHandle, CSVIDStopMode eStopMode);

/**
@brief 设置指定层的屏幕的适应模式

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] MatchMethod 视频输出端口的适应模式，如PAN_SCAN,LETTER_BOX,等
@param[in] hReserved  保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 设置指定层的屏幕的适应模式，具体效果需要根据所播放的节目的宽高比，
以及设置的适应模式，以及电视机是否有自适应功能来确定。目前只支持video层。
@see CSVIDMatchMethod
*/
BOOL CSSYSVIDSetMatchMethod(HCSHANDLE hHandle, CSVIDMatchMethod MatchMethod, HCSHANDLE hReserved);

/**
@brief 设置输出分辨率及刷新频率

设置指定解码器的输出分辨率
@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] enmResolution 系统输出分辨率, 720*480,720*576, 1920*1080 etc
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 所有输出设备(接口)一起设置
@note 这里设置的是最终输出的分辨率,包括OSD等，而非仅仅是视频
@note 如果平台支持OSD等的缩放则须将OSD缩放到该分辨率下;否则按居中处理
@note 需特别注意视频按非原始刷新频率播放可能会产生异常,所以这个设置一般来说建议按视频节目的值进行设置
*/
BOOL CSSYSVIDSetResolution(HCSHANDLE hHandle, CSVIDTimingMode enmResolution, HCSHANDLE hReserved );

/**
@brief 设置指定层的窗口大小，实现缩放功能

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] enmWindow 指定视屏输出到图形层
@param[in] pstRect 窗口的大小以及窗口位置，参看CSWINRECT结构。
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 如果pstRect为NULL，则显示为全屏。目前只支持video层
*/
BOOL CSSYSVIDSetWindowSize(HCSHANDLE hHandle, CSVIDWindow enmWindow, CSWINRECT* pstRect, HCSHANDLE hReserved );

/**
@brief 显示/隐藏视频输出层

给用户抽象三层，osd层，video层，iframe层。该接口实现对指定层的可见与不可见，
由于目前定义的为osd最上，video其次，iframe在最底层，所以osd会挡住video，video会挡住iframe
@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] enmWindow 指定视屏输出到图形层
@param[in] bIsShow TRUE:显示该窗口;FALSE:隐藏该窗口
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 
*/
BOOL CSSYSVIDShow(HCSHANDLE hHandle, CSVIDWindow enmWindow, BOOL bIsShow, HCSHANDLE hReserved );

/**
@brief 设置指定层的透明度

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] enmWindow 指定视屏输出到图形层
@param[in] nAlpha 窗口的透明度特性，取值[0，100];
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 设置指定层的透明度，范围为0-100。其中0为完全透明，100为不透明
*/
BOOL CSSYSVIDSetAlpha(HCSHANDLE hHandle,CSVIDWindow enmWindow, int nAlpha, HCSHANDLE hReserved );

/**
@brief 设置指定层的亮度

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nBrightness 视频输出的亮度，取值[0，100]
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 设置指定层的亮度，0为全黑，100为亮度最高。目前只支持video层。
@note 亮度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
BOOL CSSYSVIDSetBrightness(HCSHANDLE hHandle, int nBrightness, HCSHANDLE hReserved );

/**
@brief 设置指定层的对比度

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nContrast 视频输出的对比度，取值[0，100]；
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 设置指定层的对比度，0为全黑，100为对比度最高。目前只支持video层。
@note 对比度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
BOOL CSSYSVIDSetContrast(HCSHANDLE hHandle, int nContrast, HCSHANDLE hReserved );

/**
@brief 获取当前视频状态

@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[out] pstVIDStatus 指向存储当前视频状态的内存空间
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSYSVIDGetStatus(HCSHANDLE hHandle,CSSYSVIDStatus_S* pstVIDStatus);	

/**
@brief 设置指定层的饱和度

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nSaturation 视频输出端口的饱和度，取值[0，100]；
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 设置指定层的饱和度，0为全黑，100为饱和度最高。目前只支持video层
@note 饱和度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
BOOL CSSYSVIDSetSaturation(HCSHANDLE hHandle, int nSaturation, HCSHANDLE hReserved );

/**
@brief 设置窗口剪切位置和尺寸

设置后仅显示区域的视频，用于实现局部放大缩小等操作
@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] psRect 指定剪切的区域。
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSYSVIDSetWinClip(HCSHANDLE hHandle, const CSWINRECT psRect);

/*------------------以下接口实际上PVR未调用，但本地媒体播放却可能用到--------------*/

/**
@brief 进入快进,快退,定位模式

为快进、快退、定位做好准备，必须在其它定位接口前调用
@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 该接口一般用于从Demux注入的数据以控制播放速度等
*/
BOOL CSSYSVIDTrickModeStart(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief 退出快进,快退,定位模式

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 该接口一般用于从Demux注入的数据以控制播放速度等
*/
BOOL CSSYSVIDTrickModeStop(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief 设置Video的播放速度

用于本地媒体播放时，调整播放速度之用
@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nSpeed 播放的速度
@return 成功返回TRUE,失败返回FALSE
@note 播放速度:1000为常速播放,2000表示2倍速，500表示正向半速0为pause,负数为反向播放,例如-1000表示反向一倍速
@note 负数用于PVR等应用(待讨论);但超过平台可支持最大倍数时如何处理(待讨论)
@note VOD系统一般会在前端进行播放速度的设置，此情况下无需调用本系列接口
@note 该接口一般用于从Demux注入的数据以控制播放速度
*/
BOOL CSSYSVIDSetSpeed(HCSHANDLE hHandle, int nSpeed);

#ifdef  __cplusplus
}
#endif
/** @} */
#endif


