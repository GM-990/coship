/** @addtogroup CSVideo
@{
*/

#ifndef _CS_SYSVIDEO_H_
#define _CS_SYSVIDEO_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "udi_video.h"
#include "udi_osg.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
@name 视频输出分辨率重定义
@deprecated 本组定义仅为保持兼容而保留，推荐直接使用CSVIDTimingMode
@{*/ 
#define CSVID_TIMING_MODE_480I59940_13500		CSVID_TIMING_MODE_NTSC
#define CSVID_TIMING_MODE_576I50000_13500		CSVID_TIMING_MODE_PAL
#define CSVID_TIMING_MODE_576P50000_27000		CSVID_TIMING_MODE_576P
#define CSVID_TIMING_MODE_720P60000_74250		CSVID_TIMING_MODE_720P
#define CSVID_TIMING_MODE_720P50000_74250		CSVID_TIMING_MODE_720P_50HZ
#define CSVID_TIMING_MODE_1080I60000_74250		CSVID_TIMING_MODE_1080I
#define CSVID_TIMING_MODE_1080I50000_74250_1 		CSVID_TIMING_MODE_1080I_50HZ
/*@}*/

/**@brief 重定义视频输出宽高比转换机制
@deprecated 本定义仅为保持兼容而保留，推荐直接使用CSVIDMatchMethod
*/
#define CSVIDDisplayARConversionType  			CSVIDMatchMethod

/**@brief 视频输出模式*/
typedef enum _CSVIDOutputMode
{
	CSVID_OUTPUT_SD_MODE 	=0x00000001,	/**< SD mode. 576i/480i*/
	CSVID_OUTPUT_ED_MODE	=0x00000002,	/**< ED mode. 576p/480p*/
	CSVID_OUTPUT_HD_MODE	=0x00000004	    /**< HD mode. 1080p/1080i/720p etc.*/
}CSVIDOutputMode;

/**@brief 视频输出端口的颜色模式 */
typedef enum
{
	CSVID_COLOR_MODE_YUV,	/**< YUV模式 */
	CSVID_COLOR_MODE_RGB, 	/**< RGB模式 */
	CSVID_COLOR_MODE_NUM	 /**< 模式计数 */
}CSSYSVIDOutputColorMode;

/**@brief 需添加注释*/
typedef struct _CSOnVideoNeedData_S
{ 
	int     nJumpTimeMs;		/**< 需添加注释*/
	UINT    uNeedDataSize; 		/**< 需添加注释*/
	void *  pUserData;		    /**< 需添加注释*/
}CSOnVideoNeedData_S; 

/**@brief Image Quality Improvement  图像质量提升级别; 注意 仅ST7111  ST7105 有效*/
typedef enum _CSVIDIQILevel
{
	CSVID_IQI_LEVEL_DISABLE,			/**< Disable  Level */
	CSVID_IQI_LEVEL_LOW,				/**< Low	Level  */
	CSVID_IQI_LEVEL_MID,				/**< Middle 	Level  */
	CSVID_IQI_LEVEL_HIGH,				/**< High 	Level  */
	CSVID_IQI_LEVEL_MANUAL,			/**< Manual Level */
	CSVID_IQI_LEVEL_NUM
}CSVIDIQILevel;

/**@brief Image Quality Improvement  图像质量提升类型; 注意 仅ST7111  ST7105 有效*/
typedef enum _CSVIDIQIMode
{
	CSVID_IQI_MODE_LTI,					/**< LTI Mode 瞬时亮度增强 	*/
	CSVID_IQI_MODE_CTI,					/**< CTI Mode 瞬时色彩增强	*/
	CSVID_IQI_MODE_PEAKING,				/**< Peaking Mode 亮度峰化	*/
	CSVID_IQI_MODE_LUMA_ENHANCER,		/**< Luma 	Mode  亮度增强	*/
	CSVID_IQI_MODE_DEMO,				/**< Demo 来区别IQI 与非IQI的图像差异，分别为左边，右边，中间图像质量提升	*/
	CSVID_IQI_MODE_ZOOM_FILTERSET,		/**< 图像平滑度*/
	CSVID_IQI_MODE_NUM
}CSVIDIQIMode;

/**
@brief 视频播放事件回调函数

当底层播放视频时发生相应事件会调用该函数通知，主要用于满足特殊的视频控制功能
该接口目前暂未实现，推荐有新需求且CSSYSShowVideoCallback无法满足时使用本接口进行扩展，并逐步替代CSSYSShowVideoCallback
@param[in] dwEvent 发生的视频事件，为CSSYSVidEvent_E中的任意值
@param[in] pvParam 对应的回调数据结构结构,目前均为NULL
@param[in] pvUserData 用户数据,注册时传入
*/
typedef void (*FVideoCallback)(DWORD dwEvent,PVOID pvParam,PVOID pvUserData);

/**
@brief 注册视频事件回调函数

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] fnCallback 回调函数，可以为空
@param[in] pvUserData 用户自定义数据，会在fnCallback函数调用时用pvUserData返回给用户
@note 可被多次调用，以最后一次调用结果为准，fnCallback为NULL为反注册
@see CSSYSVIDSetEventMask CSSYSVIDGetEventMask
*/
BOOL CSSYSVIDRegisterEvent(HCSHANDLE hHandle,FVideoCallback fnCallback,PVOID pvUserData);

/**
@brief  设置视频事件有效性

即当前是否关注某个视频事件
@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[in] dwEventMask 设置当前video模块要监控的事件,为CSSYSVidEvent_E中枚举变量的组合或单值
@note 举例说明:如果dwEventMask=EM_VID_SOURCE_CHANGED_EVT|EM_VID_OVERFLOW_EVT
那么，当节目源发生变化或者video buffer ovferlow，CSSYSVIDRegisterEvent函数注册的回调函数会被调用
@see CSSYSVIDRegisterEvent CSSYSVIDGetEventMask
*/
BOOL CSSYSVIDSetEventMask(HCSHANDLE hHandle,DWORD dwEventMask);

/**
@brief 获取视频事件有效性

即获取CSSYSVIDSetEventMask函数的设置情况
@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[out] pdwEventMask 当前video模块监控的事件，为CSSYSVidEvent_E中枚举变量的组合或单值
@return 成功返回TRUE,失败返回FALSE
@see CSSYSVIDRegisterEvent CSSYSVIDSetEventMask
*/
BOOL CSSYSVIDGetEventMask(HCSHANDLE hHandle,DWORD* pdwEventMask);

/**
@brief 新帧事件回调

当有新帧解出来时调用该函数通知
@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] pUserData 用户自定义参数
@deprecated 已被FVideoCallback回调替代，不再建议使用
*/
typedef void(*FOnVidFrameCome)(HCSHANDLE hHandle, PVOID pUserData);

/**
@brief 注册新帧事件回调

@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来。具体参看设备关联接口文档定义。
@param[in] fnCallback 回调函数
@param[in] pUserData 用户自定义参数
@deprecated 已被FVideoCallback回调替代，不再建议使用
*/
void CSSYSVIDRegNewFrameCome(HCSHANDLE hHandle, FOnVidFrameCome fnCallback, PVOID pUserData);

/**
@brief 判断设备(链)是否支持H.264，MPEG-4格式的高清视频解码

@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@return 支持返回TRUE,不支持返回FALSE
@note 如果设备handle非法，返回FALSE(一个非法的设备不能解码H.264)
*/
BOOL CSSYSVIDIsSupportHD_H264(HCSHANDLE hHandle);

/**
@brief 判断设备(链)是否支持MPEG-2格式的高清视频解码

@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@return 支持返回TRUE,不支持返回FALSE
@note 如果设备handle非法，返回FALSE(一个非法的设备不能解码H.264)
*/
BOOL CSSYSVIDIsSupportHD(HCSHANDLE hHandle);

/**
@brief 清除video picture buffer的最后一帧

@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[in] hUserData 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@deprecated 请使用CSSYSVIDClearDecodeBuffer
*/
BOOL CSSYSVIDClearLastPicture(HCSHANDLE hHandle, HCSHANDLE hUserData );

/**
@brief 设置视频输出分辨率自动选项

当CSSYSVIDSetResolution设置当前分辨率为自动时，自动切换到本函数指定的分辨率
@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] enmDefaultPal 当检测到视频为PAL制时，切换到本参数指定的分辨率，建议是一个PAL分辨率如PALN,PALD等
@param[in] enmDefaultNtsc 当检测到视频为NTSC制时，切换到本参数指定的分辨率，建议是一个NTSC分辨率如NTSCM,NTSC_443,NTSC_J等
@return 成功返回TRUE,失败返回FALSE
@see CSSYSVIDSetResolution
*/
BOOL CSSYSVIDSetDefaultAutoResolution(HCSHANDLE hHandle, CSVIDTimingMode enmDefaultPal, CSVIDTimingMode enmDefaultNtsc);

/**
@brief 水平移动视频

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nXoffset 水平偏移量, 取值范围[-50, 50], 默认值为0;正数表示右移,负数表示左移
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 该接口主要用来对视频进行微调，使得不同层间的坐标趋近一致
*/
BOOL CSSYSVIDSetPositionH(HCSHANDLE hHandle, int  nXoffset, HCSHANDLE hReserved );
 
/**
@brief 垂直移动视频

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nYoffset 垂直偏移量, 取值范围[-50, 50], 默认值为0;正数表示下移,负数表示上移
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 该接口主要用来对视频进行微调，使得不同层间的坐标趋近一致
*/
BOOL CSSYSVIDSetPositionV(HCSHANDLE hHandle, int  nYoffset, HCSHANDLE hReserved );

/**
@brief 设置视频输出设备

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] dwOutputDevice 视频输出端口，为CSVIDOutputDevice中各项相或
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 供用户设置视频输出设备,未设置的设备将不会有视频输出。
@note 如果dwOutputDevice中存在任何不支持或者不能同时支持的设备,
则返回FALSE,并且不进行任何设置(包括dwOutputDevice中指定的支持的设备)
@note 这是一个平台配置性的接口,一般来说只需要在开机时调用一次
*/
BOOL CSSYSVIDSetOutputDevice(HCSHANDLE hHandle, DWORD dwOutputDevice,HCSHANDLE hReserved );

/**
@brief 设置视频输出端口的颜色模式

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] eDevice 视频输出设备
@param[in] eMode 颜色模式，RGB或是YUV
@return 成功返回TRUE,失败返回FALSE
@note 一般只支持CSVID_OUTPUT_RGB/CSVID_OUTPUT_YUV/CSVID_OUTPUT_HDMI，
其中CSVID_OUTPUT_RGB/CSVID_OUTPUT_YUV都是指分量输出，
设置立即生效
*/
BOOL CSSYSVIDSetOutputDeviceColorMode(HCSHANDLE hHandle, CSVIDOutputDevice eDevice, CSSYSVIDOutputColorMode eMode);

/*----------------------------------ES注入接口----------------------------------*/
/**
@brief VideoDecoder需要数据的回调定义 

ES注入回调
@param[in] hHandle 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来
@param[in] pvParam 对应的回调数据结构结构
@note 该接口实际是视频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放此函数无效。
@note pvParam指向一个CSOnVideoNeedData_S结构，
其中nJumpTimeMs是指上一次注入的数据尾距离本次注入数据头中间的时间间隔，
对于回放等Trick操作将用该数据决定读取数据的位置，该为值为0表示是连续的注入
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
typedef void(*FOnVideoNeedData)(HCSHANDLE hHandle,  PVOID pvParam);

/**
@brief 打开一个ES视频注入通道

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] pstESBufferInfo ES注入的Buffer的相关的信息，例如视频Buffer的地址和大小等
@return 成功返回TRUE,失败返回FALSE
@note 初始化ES注入所需要的Buffer的相关的信息，并分配buffer，大小通过初始化参数传入，默认视频大小为2兆
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
BOOL CSVIDESInit(HCSHANDLE hHandle, CSESBufferInfo* pstESBufferInfo);

/**
@brief 关闭一个ES注入通道，回收资源

终止ES的注入，并回收资源
@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@return 成功返回TRUE,失败返回FALSE
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
BOOL CSVIDESTerm(HCSHANDLE hHandle);

/**
@brief 获得ES注入VideoBuffer的空闲空间的大小

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[out] pnBufSize ES注入所需VideoBuffer的大小。
@return 成功返回TRUE,失败返回FALSE
@note 获得ES注入VideoBuffer的空闲空间的大小
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
BOOL CSESGetVideoBufferFreeSize(HCSHANDLE hHandle, int * pnBufSize);

/**
@brief 获得Video ES注入Buffer的起始地址和大小

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nBufSize 要获取的VideoBuffer的大小
@param[out] ppucBuffer ES注入所需的VideoBuffer的地址
@return 成功返回TRUE,失败返回FALSE
@note 获得ES注入VideoBuffer的空闲空间的大小
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
BOOL CSESGetAvailVideoBuffer(HCSHANDLE  hHandle, int nBufSize, PBYTE *ppucBuffer);

/**
@brief 清空ES注入的整个VideoBuffe

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@return 成功返回TRUE,失败返回FALSE
@note 清空ES注入的整个VideoBuffer
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
BOOL CSESVideoBufferReset(HCSHANDLE hHandle);

/**
@brief 往Videobuffer中写入数据

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] pucBuffer 要进行ES注入的video数据的存放的地址。
@param[in] nBufSize 要进行ES注入的video数据的大小。
@return 成功返回TRUE,失败返回FALSE
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
@note 往Videobuffer中写入数据。
*/
BOOL CSESVideoBufferPut(HCSHANDLE hHandle,  PBYTE pucBuffer, int nBufSize);

/**
@brief 设置Video数据注入的同步信息

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nFirstInjectSize 第一次进行ES注入的video数据的大小
@param[in] nFirstMilliSeconds 第一次进行video数据ES注入的时间
@return 成功返回TRUE,失败返回FALSE
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
@note 往Videobuffer中写入数据。
*/
BOOL CSESSetVideoBufferSyncInfo(HCSHANDLE  hHandle,  int  nFirstInjectSize, int  nFirstMilliSeconds);

/**
@brief 获得注入的AV ES流PTS信息

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[out] syncInfo 返回的的包含ES流的PTS数据。
@return 成功返回TRUE,失败返回FALSE
@note 该函数接口用于IPTV的ＥＳ流注入同步管理。
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
BOOL CSESGetVideoCurrentPTS(HCSHANDLE hHandle, CSESSyncInfoEx* syncInfo);

/**
@brief 设置ES流注入的视频解码器的ＰＴＳ

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] un64PTS 欲设置的以clock_t类型的视频ＰＴＳ。
@return 成功返回TRUE,失败返回FALSE
@note 该函数接口用于IPTV的ＥＳ流注入同步管理。
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
BOOL CSESSetVideoPTSEx(HCSHANDLE hHandle, U64CLock_t un64PTS);

/**
@brief 告知视频解码器本次注入的ES数据头与上次注入的数据尾是否是不连续注入

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] bDiscontinuity TRUE不连续，FALSE连续
@return 成功返回TRUE,失败返回FALSE
@note 该接口实际是视频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放此函数无效
@deprecated 仅为保持兼容而保留，不推荐将视频从本通道注入，也不推荐使用此格式视频
*/
BOOL CSSYSVIDDiscontinuity(HCSHANDLE hHandle, BOOL bDiscontinuity);

/**
@brief 在接收到FOnVideoNeedData回调注入数据后，调用该函数告知视频解码器ES注入完成

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@return 成功返回TRUE,失败返回FALSE
@note 该接口实际是视频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放此函数无效。
*/
BOOL CSSYSVIDInjectComplete(HCSHANDLE hHandle);

/**
@brief 设置ES数据注入的视频回调函数

@param[in] hHandle 视频解码器设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] pfnCallback 上层注册的用于获得decoder 信息回调函数指针。
@param[in] pvUserData 提供给回调函数的参数指针。
@return 成功返回TRUE,失败返回FALSE
@note 该回调函数用于IPTV应用，主要用来通知 STB IP端客户器ES流Decoder的数据状态，
	如数据上溢，下溢等，以便IP客户端与前端IPTV服务器进行协调，更新前端IP数据发送的速率，达到完美的播放效果。
*/
BOOL  CSESSetVideoDataInjectCallback(HCSHANDLE hHandle, CSESDataInjectCallback pfnCallback, PVOID pvUserData);

/**
@brief 注册VideoDecoder需要数据的回调

@param[in] hHandle  底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来。具体参看设备关联接口文档定义。
@param[in] fnCallback 回调
@param[in] pUserData 用户自定义参数
@return 成功返回TRUE,失败返回FALSE
@note 该接口实际是视频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放此函数无效。
*/
void CSSYSVIDRegNeedDataCallback(HCSHANDLE hHandle, FOnVideoNeedData fnCallback, PVOID pUserData);

/**@brief 清除Video Decode Buffer中的数据

调用该接口后，在没有新的视频图像解码出来之前，即使video显示是打开的，画面也应该是黑屏/静帧效果\n
主要用于解决视频流中断后恢复的情况下，未遇到I帧之前，不可用中断前最后一副I帧进行辅助解码，会产生播放马赛克问题
常见的最后一帧出现在如下情况：
- 加密节目拔掉智能卡
- 拔掉信号线等造成video画面静止的异常事件后
- 正常停止当前有视频节目后
@param[in] hHandle[in]: 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来。具体参看设备关联接口文档定义
@param[in] hReserved[in]: 该参数保留，以便日后的功能扩展，目前不使用，必须设为NULL
@return 操作成功返回TRUE，操作失败返回FALSE
*/
BOOL CSSYSVIDClearDecodeBuffer(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**@brief 设置视频的图像质量增强(IQI)参数，目前仅ST7111、ST7105平台支持该特性
@param[in] hHandle[in]: 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来。具体参看设备关联接口文档定义
@param[in] enmIQIMode[in]:    CSVID_IQI_MODE_DEMO 为客户演示模式，该模式打开后，enmIQILevel 分别对应画面的左边，右边，中间的打开IQI处理，可以根据项目设置
@param[in] enmIQILevel[in]: 该参数设置IQI 的LEVEL,有4个等级。
@param[in] dwReserve[in]: 该参数保留，以便日后的功能扩展，目前不使用，必须设为0
@return 操作成功返回TRUE，操作失败返回FALSE
*/
BOOL CSSYSVIDSetIQIMode(HCSHANDLE hHandle, CSVIDIQIMode enmIQIMode,CSVIDIQILevel enmIQILevel, DWORD dwReserve);

/**@brief 视频截图(快照)接口，将当前正在显示的视频图像的部分(或全部)截取并输出到目标图像的指定区域。
@param[in] hHandle[in]: 底层视频设备操作句柄，由应用通过调CSRMConnect() 分配得来。具体参看设备关联接口文档定义
@param[in] pstInputRect[in]:	输入窗口大小，表示要截取当前视频图像的哪一部分，为NULL表示整个图像，注意不能超出当前视频图像大小。
@param[in] pstOutputRect[in]:	输出窗口大小，表示输出到pstDestImage的哪一部分，为NULL表示整个pstDestImage，注意不能超出pstDestImage大小。
@param[in] pstDestImage[in]: 视频截图将输出到pstDestImage的指定区域，目前只支持RGB888 24位色格式，CSGraphPixmap::imagebits指向的buffer由用户分配，4字节对齐，这与CSGraphPixmap结构体定义的要求一致
@return 操作成功返回TRUE，操作失败返回FALSE
*/
BOOL CSSYSVIDSnapshot(HCSHANDLE hHandle, const CSWINRECT* pstInputRect, const CSWINRECT* pstOutputRect, CSGraphPixmap * pstDestImage);

#ifdef  __cplusplus
}
#endif
/** @} */
#endif


