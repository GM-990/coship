/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_audio.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
		 This module is used to get the audio decoders' properities supported by platform:
			 get the number of audio decoders;
			 get the capabilities of audio decoders;
			 get the status of audio decoders;
		 and do some audio decoder output control, such as:
			 audio decoding output stereo mode;
			 audio decoding output mute/unmute;
			 audio decoding output volume control;
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_audio.h"


/**
@brief ��ȡAudio Decoder�豸������
@param[out] pnAudioDecCount ϵͳ�� ����Audio Decoder�豸�ĸ���
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����pnAudioDecCount������δ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetCount(int * pnAudioDecCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief ��ȡָ��Audio Decoder�豸������

���۸��豸�ǿ��л��߱�ռ��
@param[in] nAudioIndex Audio Decoder�豸������
@param[out] psCapabilityInfo Audio Decoder�豸������Ϣ,������μ�CSUDIAUDIOCapability_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psCapabilityInfo�ṹ������δ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief ��ȡָ��Audio Decode�豸��״̬��Ϣ

@param[in] nAudioIndex Audio Decoder�豸������
@param[out] psStatus ���ڴ��Audio Decoder ״̬��Ϣ�����ݽṹ��ַָ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psStatus�ṹ������δ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief ����ָ����Ƶ������������

@param[in] nAudioIndex Audio Decoder�豸������
@param[in] eChannelType �û���Ҫ���������, ��LEFT, RIGHT, STEREO
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û�ָ������Ƶ�������ϣ��л�ģ����Ƶ���������
@note Ŀǰֻ֧��RCAͨ��������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief ����ָ����Ƶ������Ϊ����

@param[in] nAudioIndex  Audio Decoder�豸������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û�ָ����Ƶ��������ʵ�־�������
@note ���Զ�ε��ã���ֻ�е�һ����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOMute(int nAudioIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief ȡ��ָ����Ƶ�������ľ���״̬

@param[in] nAudioIndex  Audio Decoder�豸������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û�ָ����Ƶ��������ʵ��ȡ�������Ĺ���
@note �ڷ�mute״̬�µ��ô˽ӿ���Ч
@note ���Զ�ε��ã���ֻ�е�һ����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOUnmute(int nAudioIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief ����ָ����Ƶ������������

@param[in] nAudioIndex  Audio Decoder�豸������
@param[in] nVolume �û���Ҫ���������ֵ����[0,CSUDI_MAX_VOLUME]����������,0Ϊ����,CSUDI_MAX_VOLUME���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��������豸��ͳһ��������
@note ����������Ч��Χ���ش���,������Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


