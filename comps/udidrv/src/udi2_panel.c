/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_panel.c
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

#include "udi2_panel.h"
/**
@brief 得到指定功能灯的能力信息

@param[in] nPanelIndex 显示设备的索引，默认从0开始索,直到没有更多的面板
@param[in] eName  显示设备类型名 
@param[out] psLightCapablity  保存读出的指定显示类型的属性结构信息的指针
@return CSUDI_SUCCESS表示成功,错误设备号返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID,不存在对应灯返回CSUDIPANEL_ERROR_NOT_SUPPORTED
@note eName不允许指定为EM_UDIPANEL_CTRLSTART、EM_UDIPANEL_CTRLALL、EM_UDIPANEL_TEXTSTART、
EM_UDIPANEL_TEXTALL、EM_UDIPANEL_SUITSTART、EM_UDIPANEL_SUITALL、EM_UDIPANEL_MAX枚举类型，否则返回CSUDIPANEL_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIPanelGetCapabilityByName(int nPanelIndex,CSUDIPanelLightName_E eName,CSUDIPanelLightCapability_S * psLightCapablity)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置指定显示类型的显示内容
 
@param[in] nPanelIndex  显示设备的索引，默认从0 开始索引,直到没有更多的面板
@param[in] psValue 指定灯的显示值
@note 该接口调用成功后，设置的内容立即在面板上显示
@return CSUDI_SUCCESS 表示成功,错误设备号返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID,不存在对应灯返回CSUDIPANEL_ERROR_NOT_SUPPORTED
@note psValue->m_eLightName不允许指定为EM_UDIPANEL_CTRLSTART、EM_UDIPANEL_CTRLALL、EM_UDIPANEL_TEXTSTART、
EM_UDIPANEL_TEXTALL、EM_UDIPANEL_SUITSTART、EM_UDIPANEL_SUITALL、EM_UDIPANEL_MAX枚举类型，否则返回CSUDIPANEL_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIPanelDisplay(int nPanelIndex,const CSUDIPanelLightValule_S * psValue)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 清除面板显示信息
@param[in] nPanelIndex  显示设备的索引，默认从0 开始索引,直到没有更多的面板
@param[in] eName   选择要清除的显示类型，详见CSUDIPanelLightName_E
@note 
    - 如果eName类型为EM_UDIPANEL_TEXTALL，则所有TEXT显示类型都会熄灭
    - 如果eName类型为EM_UDIPANEL_CTRLALL，则所有CTRL显示类型都会熄灭
    - 如果eName类型为EM_UDIPANEL_SUITALL，则所有SUIT显示类型都会熄灭
    - 如果eName类型为EM_UDIPANEL_MAX，则所有显示类型都会熄灭
@return CSUDI_SUCCESS 表示成功，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIPanelClear(int nPanelIndex,CSUDIPanelLightName_E eName)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

