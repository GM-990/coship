/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_hdmi.c
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

#include "udi2_hdmi.h"

/**
@brief 获取HDMI接口的总数
@param[out] pnHdmiDeviceCount 返回HDMI接口的总数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIHDMIGetCount(int * pnHdmiDeviceCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief	获取HDMI接收端设备的特性

@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in]	pCaps 	指向HDMI逻辑设备的句柄	暂时没有用到这个参数，直接传NULL
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置HDMI属性

用于设置HDMI的属性,进行HDMI的配置
@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in] ePropetyType 待设置的属性项目
@param[in] pvData 属性项值,其内容见CSUDIHDMIPropertyType_E枚举中对各种属性的描述
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
@note 数据解析出错会返回FALSE，但字符串数据超长时会自动截断
*/
CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief  注册指定事件类型的回调函数

@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in] fnHdmiCallback 回调函数指针，详见CSUDIHDMICallback_F定义
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息，同CSUDISCREENCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 回调函数最少支持8个。如果用户注册个数超过所支持个数，则允许失败并返回错误代码值。
@note fnHdmiCallback和pvUserData唯一确定一个注册，当两次注册它们完全一样时，第二次将返回CSUDIHDMI_ERROR_ALREADY_ADDED，但只会通知一次；多次注册的两个参数不完全一样时则通知多次
*/
CSUDI_Error_Code CSUDIHDMIAddCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 删除指定事件类型的回调函数

@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in] fnHdmiCallback 回调函数指针，详见CSUDIHDMICallback_F定义
@param[in] pvUserData 同CSUDISCREENCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIHDMI_ERROR_NO_CALLBACK
@note fnScreenCallback和pvUserData唯一确定一个注册，必须两者与注册时完全一样方能成功删除
*/
CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback, void * pvUserData )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
	
/**
@brief 获取HDMI属性

用于获取HDMI的属性
@param[in] nHdmiIndex HDMI设备索引，通过CSUDIHDMIGetCount可以得到索引范围
@param[in] ePropetyType 待获取的属性项目
@param[out] pvData 属性项值,其内容见CSUDIHDMIPropertyType_E枚举中对各种属性的描述
@param[out] pnLen 属性值长度
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
@note 如果CSUDIHDMIPropertyType_E为EM_UDIHDMI_LINK_STATUS:则pvData为CSUDIHDMILinkStatus_E，pnLen不使用;
*/
CSUDI_Error_Code CSUDIHDMIGetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData, int* pnLen)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

