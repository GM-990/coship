/**@defgroup SCREEN  SCREEN 定义视频播放后处理相关接口
@brief SCREEN模块对应头文件<udi2_screen.h>，定义视频播放后处理相关接口。

Screen的概念是多个Video（解码器）最终将视频混合到若干个（目前是SD/HD两个）Screen上输出（包括OSD），Screen再将内容输出到各个VOUT（输出接口如HDMI、分离等）上。很明显，最终输出到高清和标清上的内容，即使我们不考虑内容的变化也是有细微差异的，比如分辨率等等。Screen就是对这些内容进行抽象，并提出控制接口 。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.3 2009/09/24 增加获取Screen能力接口说明
@version 2.0.2 2009/08/25 增加Screen意义的说明
@version 2.0.1 2009/08/16 初稿
@{
*/
#ifndef _UDI2_SCREEN_H_
#define _UDI2_SCREEN_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif
 

/**@brief 视频播放相关错误代码值*/
enum
{
	CSUDISCREEN_ERROR_BAD_PARAMETER = CSUDI_SCREEN_ERROR_BASE,  ///< 错误参数类型
	CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED,      				 ///< 不支持此操作
	CSUDISCREEN_ERROR_ALREADY_ADDED							///< 该回调已经注册
};

/**@brief 视频输出通道类型*/
typedef enum
{
	EM_UDI_VOUT_DEVICE_SD	= 1,			///<标清输出通道
	EM_UDI_VOUT_DEVICE_HD	= 1 << 1		///<高清输出通道
}CSUDISCREENType_E;

/**@brief 视频输出分辨率*/
typedef enum 
{
	EM_UDISCREEN_RESOLUTION_INVALID,
	EM_UDISCREEN_RESOLUTION_NTSC,  			///<  SD: EM_UDICTL_MIX_ENCODING_MODE_NTSCM 
	EM_UDISCREEN_RESOLUTION_NTSC_443,		///<  NTSC M mode but with 4.43361875MHz color subcarrier frequency. 
	EM_UDISCREEN_RESOLUTION_NTSC_JAPAN,	///<  Japan NTSC, no pedestal level 
	EM_UDISCREEN_RESOLUTION_PAL, 		     	///<  SD: EM_UDICTL_MIX_ENCODING_MODE_PALBDGHI 
	EM_UDISCREEN_RESOLUTION_PAL_M,        	///<  PAL Brazil 
	EM_UDISCREEN_RESOLUTION_PAL_N,        	///< PAL_N 
	EM_UDISCREEN_RESOLUTION_PAL_NC,      	///<  PAL_N, Argentina (PAL N mode but with 3.58205625 MHz color subcarrier frequency and no pedestal (= 0 IRE blanking setup)) 
	EM_UDISCREEN_RESOLUTION_PAL_B,			///< Australia 
	EM_UDISCREEN_RESOLUTION_PAL_B1,		///< Hungary 
	EM_UDISCREEN_RESOLUTION_PAL_D,			///<  China ==EM_UDISCREEN_RESOLUTION_PAL 
	EM_UDISCREEN_RESOLUTION_PAL_D1,		///<  Poland 
	EM_UDISCREEN_RESOLUTION_PAL_G,			///< Europe. Same as EM_UDISCREEN_RESOLUTION_PAL. 
	EM_UDISCREEN_RESOLUTION_PAL_H,			///<  Europe 
	EM_UDISCREEN_RESOLUTION_PAL_K,	      		///< Europe 
	EM_UDISCREEN_RESOLUTION_PAL_I,			///<  U.K. 
	EM_UDISCREEN_RESOLUTION_SECAM,      		///< SECAM III B6 
	
	EM_UDISCREEN_RESOLUTION_480P,        ///< NTSC Progressive (27Mhz) 	
	EM_UDISCREEN_RESOLUTION_576P, 		///<  Australian mode 	
	EM_UDISCREEN_RESOLUTION_720P,  		///< HD: EIA770.3 #1 P60 = SMPTE 296M #1 P60 
	EM_UDISCREEN_RESOLUTION_720P_24HZ,    ///<  HD 720p 24Hz 	
	EM_UDISCREEN_RESOLUTION_720P_50HZ,	///< HD 720p 50Hz
	
	EM_UDISCREEN_RESOLUTION_1080I,  	   ///< HD: EIA770.3 #3 I60 = SMPTE274M #4 I60 		
	EM_UDISCREEN_RESOLUTION_1080I_50HZ, ///<  HD:  SMPTE 274M   I50 
	
	EM_UDISCREEN_RESOLUTION_1080P,     ///<  HD 1080 Progressive, 60Hz 
	EM_UDISCREEN_RESOLUTION_1080P_24HZ,///<  HD 1080 Progressive, 24Hz 
	EM_UDISCREEN_RESOLUTION_1080P_25HZ,	///<  HD 1080 Progressive, 25Hz 
	EM_UDISCREEN_RESOLUTION_1080P_30HZ,	///<  HD 1080 Progressive, 30Hz 
	EM_UDISCREEN_RESOLUTION_1080P_50HZ, ///<  HD 1080 Progressive, 50Hz 	
	EM_UDISCREEN_RESOLUTION_1250I_50HZ,   ///<  HD 1250 Interlaced, 50Hz 
	
	EM_UDISCREEN_RESOLUTION_VESA,			///<  PC monitor. Requires width, height and refresh rate parameters 
	EM_UDISCREEN_RESOLUTION_NUM
} CSUDISCREENResolution_E;

/**@brief 屏幕事件类型*/
typedef enum
{
    EM_UDISCREEN_RESOLUTION_CHANGED              ///<  屏幕分辨率改变事件
}CSUDISCREENEvent_E;

/**@brief 平台输出设备支持的分辨率设置能力信息
@note 数组中，如果碰到类型为EM_UDISCREEN_RESOLUTION_INVALID，则表示支持的分辨率类型已经结束，后续数据将被忽略。
*/
typedef struct
{
	  CSUDISCREENResolution_E   m_eResolution[EM_UDISCREEN_RESOLUTION_NUM];	   ///< 设备支持的视频分辨率格式
} CSUDISCREENCapability_S;

/**
@brief 获取指定输出设备的分辨率设置能力
@param[in] eScreenDevice 视频输出通道类型
@param[out] psCapabilityInfo  输出通道设备能力信息,详情请参见CSUDISCREENCapability_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psCapabilityInfo结构的内容未定义
@note 若平台不支持该输出通道(如不支持高清)该接口返回CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
@note 此接口一次只允许传入一个Screen设备，不得多个设备相或，否则返回CSUDISCREEN_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDISCREENGetCapability(CSUDISCREENType_E eScreenDevice,CSUDISCREENCapability_S * psCapabilityInfo);

/**
@brief 屏幕分辨率改变回调函数定义

@param[in] eEvt 屏幕事件类型，详见CSUDISCREENEvent_E定义
@param[in] eScreenDevice 视频输出接口，详见CSUDISCREENType_E定义
@param[in] pvUserData 用户数据
*/
typedef void (*CSUDISCREENCallback_F)(CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice,void * pvUserData);

 /**
@brief  注册指定事件类型的回调函数

@param[in] fnScreenCallback 回调函数指针，详见CSUDISCREENCallback_F定义
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息，同CSUDISCREENCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 回调函数的最小最大注册个数为32个，超过32个允许失败并返回错误代码值
@note fnScreenCallback和pvUserData唯一确定一个注册，当两次注册它们完全一样时，第二次将返回CSUDISCREEN_ERROR_ALREADY_ADDED，但只会通知一次；多次注册的两个参数不完全一样时则通知多次
*/
CSUDI_Error_Code CSUDISCREENAddCallback(CSUDISCREENCallback_F fnScreenCallback,void * pvUserData );

 /**
@brief 删除指定事件类型的回调函数

@param[in] fnScreenCallback 回调函数指针，详见CSUDISCREENCallback_F定义
@param[in] pvUserData 同CSUDISCREENCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回错误代码值
@note fnScreenCallback和pvUserData唯一确定一个注册，必须两者与注册时完全一样方能成功删除
*/
CSUDI_Error_Code CSUDISCREENDelCallback(CSUDISCREENCallback_F fnScreenCallback, void * pvUserData );
 
/**
@brief 设置输出分辨率及刷新频率

设置指定视频输出接口的分辨率
@param[in] peScreenDevice 需指定视频输出接口的数组
@param[in] peResolution 系统输出分辨率数组, 720*480,720*576, 1920*1080 etc，请参见CSUDISCREENResolution_E
@param[in] nCount peScreenDevice及peResolution数组的大小，要求两个数组必须一样大
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note Eastwin设置不同输出接口的分辨率在帧率上最好具有兼容性。如CVBS输出的PAL制和高清的1080i 50Hz兼容,同1080i 60Hz不兼容。若不兼容UDI应尽量处理。
@note Eastwin会尽量一次性设置多个Screen的分辨率，以方便UDI层进行合并处理
@note 若同时设置多个视频设备类型，有任何一个不支持时，该接口返回失败，并且其它支持的设置不生效
*/
CSUDI_Error_Code CSUDISCREENSetResolution(CSUDISCREENType_E* peScreenDevice, CSUDISCREENResolution_E* peResolution,int nCount);

/**
@brief 获取当前视频分辨率

@param[in] eScreenDevice 指定视频输出接口
@param[out] peResolution 当前屏幕的分辨率
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 此接口一次只允许传入一个Screen设备，不得多个设备相或，否则返回CSUDISCREEN_ERROR_BAD_PARAMETER
@note 屏幕分辨率与Player、播放等无关，无论当前是什么状态都需要能获取该信息。
*/
CSUDI_Error_Code CSUDISCREENGetResolution(CSUDISCREENType_E eScreenDevice,CSUDISCREENResolution_E * peResolution);

/**
@brief 设置Screen OSD透明度

@param[in] nTransparency 透明度，取值[0，100];
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置OSD层的透明度，范围为0-100。其中0为完全透明，100为不透明
*/
CSUDI_Error_Code  CSUDISCREENSetOSDTransparency(int nTransparency);

/**
@brief 设置视频输出的亮度

@param[in] nBrightness 视频输出的亮度，取值[0，100]
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置视频输出的亮度，0为全黑，100为亮度最高。
@note 亮度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
CSUDI_Error_Code CSUDISCREENSetBrightness(int nBrightness);

/**
@brief 设置视频输出的对比度

@param[in] nContrast 视频输出的对比度，取值[0，100]；
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置视频输出对比度，0为全黑，100为对比度最高。
@note 对比度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
CSUDI_Error_Code CSUDISCREENSetContrast( int nContrast);

/**
@brief 设置视频输出的饱和度

@param[in] nSaturation 视频输出端口的饱和度，取值[0，100]；
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置指定层的饱和度，0为全黑，100为饱和度最高。
@note 饱和度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
CSUDI_Error_Code CSUDISCREENSetSaturation( int nSaturation);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

