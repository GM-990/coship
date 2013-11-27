/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_vout.c
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

#include "udi2_vout.h"
/**
@brief ��ָ������Ƶ����豸

@param[in] eVoutDevice  ��Ƶ����豸��������μ�CSUDIVOUTDevices_E
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVOUTEnable(CSUDIVOUTDevices_E eVoutDevice)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �ر�ָ������Ƶ����豸

@param[in] eVoutDevice	��Ƶ����豸��������μ�CSUDIVOUTDevices_E
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVOUTDisable(CSUDIVOUTDevices_E eVoutDevice)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	 
	return Retcode;
}

/**
@brief ������Ƶ����˿ڵ���ɫ����

@param[in] eVoutDevice ��Ƶ����豸,ֻ֧�ַ���(EM_UDIVOUT_YUV)/HDMI(EM_UDIVOUT_HDMI)
@param[in] eType �������ɫ���ͣ�RGB����YUV
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����������Ч
*/
CSUDI_Error_Code CSUDIVOUTSetOutputType(CSUDIVOUTDevices_E eVoutDevice, CSUDIVOUTOutputType_E eType)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

