/****************************************************************************/
/*
 * Filename:        cs_panel_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_panel_plus.h"

/**
@brief Panel模块初始化
@param[in] nIndex panel索引
@param[in] psParms 初始化Panel模块所需要的参数
@return CSUDI_SUCCESS表示成功,错误设备号返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID,初始化失败返回CSUDI_FAILURE
@note: 该接口已不推荐使用,请使用CSUDIPanelInitExt
*/
CSUDI_Error_Code CSUDIPanelInit(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief Panel模块初始化
@param[in] nIndex panel索引
@param[in] ePanelType panel类型
@param[in] pParms 该参数根据ePanelType指定的不同panel类型，而指向不同的数据结构，详细如下：
	EM_PANEL_TYPE_COMMON_LED：pParms为指向CSUDIPanelCommonLedCfg_S的指针
	EM_PANEL_TYPE_SPI_LED：pParms为指向CSUDIPanelCommonLedCfg_S的指针
	EM_PANEL_TYPE_COMMON_NO_LED：pParms为指向CSUDIPanelCommonLedCfg_S的指针
	EM_PANEL_TYPE_MCU_LED：pParms为指向CSUDIPanelMcuCfg_S的指针
	EM_PANEL_TYPE_GPIO_LED：pParms为指向CSUDIPanelGpioCfg_S的指针
	EM_PANEL_TYPE_GPIO_NO_LED：pParms为指向CSUDIPanelGpioCfg_S的指针
@return 
	-成功返回 CSUDI_SUCCESS
	-错误设备号返回 CSUDIPANEL_ERROR_INVALID_DEVICE_ID
	-参数错误返回 CSUDIPANEL_ERROR_BAD_PARAMETER
	-初始化失败返回 CSUDI_FAILURE
@note 
*/
CSUDI_Error_Code CSUDIPanelInitExt(int nIndex,CSUDIPanelType_E ePanelType,void *pParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 普通LED面板初始化,调用此接口初始化此类型面板可减小链接库的大小
*/
CSUDI_Error_Code CSUDIPanelInitCommonLed(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 普通VFD面板初始化,调用此接口初始化此类型面板可减小链接库的大小
*/
CSUDI_Error_Code CSUDIPanelInitCommonVfd(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 单片机控制的LED面板初始化,调用初始化此类型面板可减小链接库的大小
*/
CSUDI_Error_Code CSUDIPanelInitMCULed(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 单片机控制的VFD面板初始化,调用初始化此类型面板可减小链接库的大小
*/
CSUDI_Error_Code CSUDIPanelInitMCUVfd(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


