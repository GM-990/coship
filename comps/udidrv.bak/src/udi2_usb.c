/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_usb.c
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

#include "udi2_usb.h"
/**
@brief ��ȡ��ǰ�����USB �豸����
@param[out] pdwUsbDeviceCount ��ǰ�����USB �豸����
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIUSBGetCount(unsigned int* pdwUsbDeviceCount)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �����豸������ȡUSB �豸��Ϣ

@param[in] uIndex USB �豸����ID����0��ʼ���ֵΪCSUDIUSBGetCount��ȡ����count-1
@param[out] psUsbInfo ָ�򷵻�USB�豸����Ϣ
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIUSBGetInfo(unsigned int uIndex, CSUDIUSBInfo_S * psUsbInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

