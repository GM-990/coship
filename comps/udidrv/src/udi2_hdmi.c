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
@brief ��ȡHDMI�ӿڵ�����
@param[out] pnHdmiDeviceCount ����HDMI�ӿڵ�����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIHDMIGetCount(int * pnHdmiDeviceCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief	��ȡHDMI���ն��豸������

@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in]	pCaps 	ָ��HDMI�߼��豸�ľ��	��ʱû���õ����������ֱ�Ӵ�NULL
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����HDMI����

��������HDMI������,����HDMI������
@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in] ePropetyType �����õ�������Ŀ
@param[in] pvData ������ֵ,�����ݼ�CSUDIHDMIPropertyType_Eö���жԸ������Ե�����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
@note ���ݽ�������᷵��FALSE�����ַ������ݳ���ʱ���Զ��ض�
*/
CSUDI_Error_Code CSUDIHDMISetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief  ע��ָ���¼����͵Ļص�����

@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in] fnHdmiCallback �ص�����ָ�룬���CSUDIHDMICallback_F����
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ��ͬCSUDISCREENCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note �ص���������֧��8��������û�ע�����������֧�ָ�����������ʧ�ܲ����ش������ֵ��
@note fnHdmiCallback��pvUserDataΨһȷ��һ��ע�ᣬ������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIHDMI_ERROR_ALREADY_ADDED����ֻ��֪ͨһ�Σ����ע���������������ȫһ��ʱ��֪ͨ���
*/
CSUDI_Error_Code CSUDIHDMIAddCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ɾ��ָ���¼����͵Ļص�����

@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in] fnHdmiCallback �ص�����ָ�룬���CSUDIHDMICallback_F����
@param[in] pvUserData ͬCSUDISCREENCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIHDMI_ERROR_NO_CALLBACK
@note fnScreenCallback��pvUserDataΨһȷ��һ��ע�ᣬ����������ע��ʱ��ȫһ�����ܳɹ�ɾ��
*/
CSUDI_Error_Code CSUDIHDMIDelCallback(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback, void * pvUserData )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
	
/**
@brief ��ȡHDMI����

���ڻ�ȡHDMI������
@param[in] nHdmiIndex HDMI�豸������ͨ��CSUDIHDMIGetCount���Եõ�������Χ
@param[in] ePropetyType ����ȡ��������Ŀ
@param[out] pvData ������ֵ,�����ݼ�CSUDIHDMIPropertyType_Eö���жԸ������Ե�����
@param[out] pnLen ����ֵ����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
@note ���CSUDIHDMIPropertyType_EΪEM_UDIHDMI_LINK_STATUS:��pvDataΪCSUDIHDMILinkStatus_E��pnLen��ʹ��;
*/
CSUDI_Error_Code CSUDIHDMIGetProperty(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData, int* pnLen)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

