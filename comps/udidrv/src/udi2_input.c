/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_input.c
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

#include "udi2_input.h"

/**
@brief ע�������豸�����ص�����

�κ������豸������Ч�İ�����Ϣ��ͨ��ע��Ļص�����֪ͨӦ�ò�\n
@param[in] fnInputCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�������Դ��״̬�͹�����
@param[in] pvUserData �û����ݣ�����ΪCSUDI_NULL
@return ע��ɹ��򷵻�CSUDI_SUCCESS ;ע��ʧ���򷵻���ش������
@note 
- ֧��ע��Ļص�����������֧��8���������
- fnInputCallback��pvUserDataΨһ��ʶһ���ص����ͣ�������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIINPUT_ERROR_ALREADY_ADDED����

@note Booter��ص������������ж��е��ã��ϲ�ᰴ�ж�ʹ��ԭ��ʹ�ûص�
*/
CSUDI_Error_Code CSUDIINPUTAddCallback( CSUDIInputCallback_F fnInputCallback ,void * pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}



/**
@brief ɾ�������豸�����ص�����

@param[in] fnInputCallback ָ��ص������ĺ���ָ��
@param[in] pvUserData �û�����
@return ɾ���ɹ��򷵻�CSUDI_SUCCESS;ɾ��ʧ���򷵻���ش������
@note ���Ҫɾ��һ�������ڵĻص����򷵻�CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST
@note fnInputCallback��pvUserDataΨһ��ʶһ���ص�����

@note Booter��ص������������ж��е��ã��ϲ�ᰴ�ж�ʹ��ԭ��ʹ�ûص�
*/
CSUDI_Error_Code CSUDIINPUTDelCallback(  CSUDIInputCallback_F fnInputCallback ,void * pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

