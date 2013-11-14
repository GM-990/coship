/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_aout.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
		 This module is used to control RCA, SPDIF, HDMI audio output, such as: 
			 audio output mute;
			 audio output unmute;
			 audio output passthrough mode;
			 audio output decoded mode;
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_aout.h"

/**
@brief ����ָ����Ƶ���ͨ��Ϊ����

@param[in] eAudioDevice  ��Ƶ ���ͨ����������μ�CSUDIAOUTDevices_E
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û��趨��ָ����Ƶͨ����ʵ�־�������
@note ���Զ�ε��ã���ֻ�е�һ����Ч
*/
CSUDI_Error_Code CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ȡ��ָ����Ƶ���ͨ���ľ���״̬

@param[in] eAudioDevice ��Ƶ���ͨ����������μ�CSUDIAOUTDevices_E
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û��趨��ָ����Ƶ�豸��ʵ��ȡ�������Ĺ���
@note �ڷ�mute״̬�µ��ô˽ӿ���Ч
@note ���Զ�ε��ã���ֻ�е�һ����Ч
*/
CSUDI_Error_Code CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����ָ�����ͨ�������ģʽ

@param[in] eAudioDevice ��Ƶ���ͨ����������μ�CSUDIAOUTDevices_E
@param[in] eMode �豸�����ģʽ��������μ�CSUDIAOUTMode_E��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ȱʡ����£�������Ƶ���ͨ��Ϊ��������ģʽ��������������ֻ���ǽ�������ģʽ��
@note �����ǰ���ڲ���״̬�����øýӿ�������Ч�����������������ݵ�ֹͣ
*/
CSUDI_Error_Code CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
