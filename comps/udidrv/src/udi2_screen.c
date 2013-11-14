/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_screen.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_screen.h"
/**
@brief 获取指定输出设备的分辨率设置能力
@param[in] eScreenDevice 视频输出通道类型
@param[out] psCapabilityInfo  输出通道设备能力信息,详情请参见CSUDISCREENCapability_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psCapabilityInfo结构的内容未定义
@note 若平台不支持该输出通道(如不支持高清)该接口返回CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
@note 此接口一次只允许传入一个Screen设备，不得多个设备相或，否则返回CSUDISCREEN_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDISCREENGetCapability(CSUDISCREENType_E eScreenDevice,CSUDISCREENCapability_S * psCapabilityInfo)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

 /**
@brief  注册指定事件类型的回调函数

@param[in] fnScreenCallback 回调函数指针，详见CSUDISCREENCallback_F定义
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息，同CSUDISCREENCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 回调函数的最小最大注册个数为32个，超过32个允许失败并返回错误代码值
@note fnScreenCallback和pvUserData唯一确定一个注册，当两次注册它们完全一样时，第二次将返回CSUDISCREEN_ERROR_ALREADY_ADDED，但只会通知一次；多次注册的两个参数不完全一样时则通知多次
*/
CSUDI_Error_Code CSUDISCREENAddCallback(CSUDISCREENCallback_F fnScreenCallback,void * pvUserData )
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
@brief 删除指定事件类型的回调函数

@param[in] fnScreenCallback 回调函数指针，详见CSUDISCREENCallback_F定义
@param[in] pvUserData 同CSUDISCREENCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回错误代码值
@note fnScreenCallback和pvUserData唯一确定一个注册，必须两者与注册时完全一样方能成功删除
*/
CSUDI_Error_Code CSUDISCREENDelCallback(CSUDISCREENCallback_F fnScreenCallback, void * pvUserData )
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

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
CSUDI_Error_Code CSUDISCREENSetResolution(CSUDISCREENType_E* peScreenDevice, CSUDISCREENResolution_E* peResolution,int nCount)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取当前视频分辨率

@param[in] eScreenDevice 指定视频输出接口
@param[out] peResolution 当前屏幕的分辨率
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 此接口一次只允许传入一个Screen设备，不得多个设备相或，否则返回CSUDISCREEN_ERROR_BAD_PARAMETER
@note 屏幕分辨率与Player、播放等无关，无论当前是什么状态都需要能获取该信息。
*/
CSUDI_Error_Code CSUDISCREENGetResolution(CSUDISCREENType_E eScreenDevice,CSUDISCREENResolution_E * peResolution)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置Screen OSD透明度

@param[in] nTransparency 透明度，取值[0，100];
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置OSD层的透明度，范围为0-100。其中0为完全透明，100为不透明
*/
CSUDI_Error_Code  CSUDISCREENSetOSDTransparency(int nTransparency)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置视频输出的亮度

@param[in] nBrightness 视频输出的亮度，取值[0，100]
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置视频输出的亮度，0为全黑，100为亮度最高。
@note 亮度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
CSUDI_Error_Code CSUDISCREENSetBrightness(int nBrightness)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置视频输出的对比度

@param[in] nContrast 视频输出的对比度，取值[0，100]；
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置视频输出对比度，0为全黑，100为对比度最高。
@note 对比度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
CSUDI_Error_Code CSUDISCREENSetContrast( int nContrast)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置视频输出的饱和度

@param[in] nSaturation 视频输出端口的饱和度，取值[0，100]；
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置指定层的饱和度，0为全黑，100为饱和度最高。
@note 饱和度的默认值为50,即该值设置为50时,大多数节目呈现出最好效果
*/
CSUDI_Error_Code CSUDISCREENSetSaturation( int nSaturation)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

