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
@brief �õ�ָ�����ܵƵ�������Ϣ

@param[in] nPanelIndex ��ʾ�豸��������Ĭ�ϴ�0��ʼ��,ֱ��û�и�������
@param[in] eName  ��ʾ�豸������ 
@param[out] psLightCapablity  ���������ָ����ʾ���͵����Խṹ��Ϣ��ָ��
@return CSUDI_SUCCESS��ʾ�ɹ�,�����豸�ŷ���CSUDIPANEL_ERROR_INVALID_DEVICE_ID,�����ڶ�Ӧ�Ʒ���CSUDIPANEL_ERROR_NOT_SUPPORTED
@note eName������ָ��ΪEM_UDIPANEL_CTRLSTART��EM_UDIPANEL_CTRLALL��EM_UDIPANEL_TEXTSTART��
EM_UDIPANEL_TEXTALL��EM_UDIPANEL_SUITSTART��EM_UDIPANEL_SUITALL��EM_UDIPANEL_MAXö�����ͣ����򷵻�CSUDIPANEL_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIPanelGetCapabilityByName(int nPanelIndex,CSUDIPanelLightName_E eName,CSUDIPanelLightCapability_S * psLightCapablity)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����ָ����ʾ���͵���ʾ����
 
@param[in] nPanelIndex  ��ʾ�豸��������Ĭ�ϴ�0 ��ʼ����,ֱ��û�и�������
@param[in] psValue ָ���Ƶ���ʾֵ
@note �ýӿڵ��óɹ������õ������������������ʾ
@return CSUDI_SUCCESS ��ʾ�ɹ�,�����豸�ŷ���CSUDIPANEL_ERROR_INVALID_DEVICE_ID,�����ڶ�Ӧ�Ʒ���CSUDIPANEL_ERROR_NOT_SUPPORTED
@note psValue->m_eLightName������ָ��ΪEM_UDIPANEL_CTRLSTART��EM_UDIPANEL_CTRLALL��EM_UDIPANEL_TEXTSTART��
EM_UDIPANEL_TEXTALL��EM_UDIPANEL_SUITSTART��EM_UDIPANEL_SUITALL��EM_UDIPANEL_MAXö�����ͣ����򷵻�CSUDIPANEL_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIPanelDisplay(int nPanelIndex,const CSUDIPanelLightValule_S * psValue)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��������ʾ��Ϣ
@param[in] nPanelIndex  ��ʾ�豸��������Ĭ�ϴ�0 ��ʼ����,ֱ��û�и�������
@param[in] eName   ѡ��Ҫ�������ʾ���ͣ����CSUDIPanelLightName_E
@note 
    - ���eName����ΪEM_UDIPANEL_TEXTALL��������TEXT��ʾ���Ͷ���Ϩ��
    - ���eName����ΪEM_UDIPANEL_CTRLALL��������CTRL��ʾ���Ͷ���Ϩ��
    - ���eName����ΪEM_UDIPANEL_SUITALL��������SUIT��ʾ���Ͷ���Ϩ��
    - ���eName����ΪEM_UDIPANEL_MAX����������ʾ���Ͷ���Ϩ��
@return CSUDI_SUCCESS ��ʾ�ɹ���ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIPanelClear(int nPanelIndex,CSUDIPanelLightName_E eName)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

