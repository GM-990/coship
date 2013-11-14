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
@brief Panelģ���ʼ��
@param[in] nIndex panel����
@param[in] psParms ��ʼ��Panelģ������Ҫ�Ĳ���
@return CSUDI_SUCCESS��ʾ�ɹ�,�����豸�ŷ���CSUDIPANEL_ERROR_INVALID_DEVICE_ID,��ʼ��ʧ�ܷ���CSUDI_FAILURE
@note: �ýӿ��Ѳ��Ƽ�ʹ��,��ʹ��CSUDIPanelInitExt
*/
CSUDI_Error_Code CSUDIPanelInit(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief Panelģ���ʼ��
@param[in] nIndex panel����
@param[in] ePanelType panel����
@param[in] pParms �ò�������ePanelTypeָ���Ĳ�ͬpanel���ͣ���ָ��ͬ�����ݽṹ����ϸ���£�
	EM_PANEL_TYPE_COMMON_LED��pParmsΪָ��CSUDIPanelCommonLedCfg_S��ָ��
	EM_PANEL_TYPE_SPI_LED��pParmsΪָ��CSUDIPanelCommonLedCfg_S��ָ��
	EM_PANEL_TYPE_COMMON_NO_LED��pParmsΪָ��CSUDIPanelCommonLedCfg_S��ָ��
	EM_PANEL_TYPE_MCU_LED��pParmsΪָ��CSUDIPanelMcuCfg_S��ָ��
	EM_PANEL_TYPE_GPIO_LED��pParmsΪָ��CSUDIPanelGpioCfg_S��ָ��
	EM_PANEL_TYPE_GPIO_NO_LED��pParmsΪָ��CSUDIPanelGpioCfg_S��ָ��
@return 
	-�ɹ����� CSUDI_SUCCESS
	-�����豸�ŷ��� CSUDIPANEL_ERROR_INVALID_DEVICE_ID
	-�������󷵻� CSUDIPANEL_ERROR_BAD_PARAMETER
	-��ʼ��ʧ�ܷ��� CSUDI_FAILURE
@note 
*/
CSUDI_Error_Code CSUDIPanelInitExt(int nIndex,CSUDIPanelType_E ePanelType,void *pParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ��ͨLED����ʼ��,���ô˽ӿڳ�ʼ�����������ɼ�С���ӿ�Ĵ�С
*/
CSUDI_Error_Code CSUDIPanelInitCommonLed(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ��ͨVFD����ʼ��,���ô˽ӿڳ�ʼ�����������ɼ�С���ӿ�Ĵ�С
*/
CSUDI_Error_Code CSUDIPanelInitCommonVfd(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ��Ƭ�����Ƶ�LED����ʼ��,���ó�ʼ�����������ɼ�С���ӿ�Ĵ�С
*/
CSUDI_Error_Code CSUDIPanelInitMCULed(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ��Ƭ�����Ƶ�VFD����ʼ��,���ó�ʼ�����������ɼ�С���ӿ�Ĵ�С
*/
CSUDI_Error_Code CSUDIPanelInitMCUVfd(int nIndex, CSUDIPanelParams_S *psParms)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


