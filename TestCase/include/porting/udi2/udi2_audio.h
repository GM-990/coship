/**@defgroup AUDIO AUDIO ������ƽ̨�޹ص���Ƶ�����ӿ�
@brief AUDIOģ���Ӧͷ�ļ�<udi2_audio.h>��������Ƶ���������Լ����ƽӿڡ�

��Videoһ����һ�����Բ�����Ҫ������������Ƶ����������Ϊһ��Audio��������������һ���0��ʼ��һ����˵0��ʾ�������������оƬ֧��ͬʱ������·������Ƶ������Գ��������Audio����������һ���ϸ�pip��һ�µģ���Ȼһ������£�����������������

���оƬ֧���ڲ�����Ƶ��ͬʱ����PCM��Ҳ���Զ�����һ��PCM��Audio������ ��

@version 2.0.3 2009/10/12 �޸����������Լ��������ģʽ������Ч����
@version 2.0.2 2009/08/16 ����Audio�������˵��
@version 2.0.1 2009/08/16 ����
@{
*/
#ifndef _UDI2_AUDIO_H_
#define _UDI2_AUDIO_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_aout.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDI_MAX_VOLUME		(31)  ///< �������ֵ

/**@brief ��Ƶ�������ֵ*/
enum
{
	CSUDIAUDIO_ERROR_BAD_PARAMETER = CSUDI_AUDIO_ERROR_BASE,    ///< �������
	CSUDIAUDIO_ERROR_INVALID_DEVICE_ID,							///< �Ƿ��豸ID
	CSUDIAUDIO_ERROR_FEATURE_NOT_SUPPORTED						///< ��֧�ִ˲���
};

/**@brief ��Ƶ����*/
typedef enum 
{
  	EM_UDIAUDIO_CHANNEL_STEREO,			///< ������ 
  	EM_UDIAUDIO_CHANNEL_LEFT,			    ///< ������ 
  	EM_UDIAUDIO_CHANNEL_RIGHT,				///< ������ 
  	EM_UDIAUDIO_CHANNEL_MIXED_MONO			///< ����� 
}CSUDIAUDIOChannelType_E;	

/**@brief Audio Decoder�豸������Ϣ
@note �����У����������Ƶ����ΪEM_UDI_AUD_STREAM_INVALID�����ʾ֧�ֵ���Ƶ��ʽ�����Ѿ��������������ݽ������ԡ�
*/
typedef struct  
{
	CSUDIAUDStreamType_E m_sSupportDecode[EM_UDI_AUD_STREAMTYPE_NUM];   ///< ��Ƶ������֧�ֵ���Ƶ��ʽ��������
	CSUDIAUDStreamType_E m_sSupportByPass[EM_UDI_AUD_STREAMTYPE_NUM];   ///< ֧��ֱ���������Ƶ������������
} CSUDIAUDIOCapability_S;


/**@brief ��Ƶ״̬�ṹ�� */
typedef struct 
{
	CSUDIAUDStreamType_E			m_eStreamType;	///<  ��Ƶ������ 
	int								m_nSampleRate;  ///<  ��Ƶ������ 
	int								m_nBitWidth;	///<  ��Ƶ����λ�� 
	int								m_nChannels;  	///<  ͨ����:1-mono,2-stereo 
	unsigned int					m_uFrameCount;  ///<  �ѽ����֡�� 
}CSUDIAUDIOStatus_S;

/**
@brief ��ȡAudio Decoder�豸������
@param[out] pnAudioDecCount ϵͳ�� ����Audio Decoder�豸�ĸ���
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����pnAudioDecCount������δ����
*/
CSUDI_Error_Code CSUDIAUDIOGetCount(int * pnAudioDecCount);

/**
@brief ��ȡָ��Audio Decoder�豸������

���۸��豸�ǿ��л��߱�ռ��
@param[in] nAudioIndex Audio Decoder�豸������
@param[out] psCapabilityInfo Audio Decoder�豸������Ϣ,������μ�CSUDIAUDIOCapability_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psCapabilityInfo�ṹ������δ����
*/
CSUDI_Error_Code CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo);

/**
@brief ��ȡָ��Audio Decode�豸��״̬��Ϣ

@param[in] nAudioIndex Audio Decoder�豸������
@param[out] psStatus ���ڴ��Audio Decoder ״̬��Ϣ�����ݽṹ��ַָ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psStatus�ṹ������δ����
*/
CSUDI_Error_Code CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus);

/**
@brief ����ָ����Ƶ������������

@param[in] nAudioIndex Audio Decoder�豸������
@param[in] eChannelType �û���Ҫ���������, ��LEFT, RIGHT, STEREO
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û�ָ������Ƶ�������ϣ��л�ģ����Ƶ���������
@note Ŀǰֻ֧��RCAͨ��������
*/
CSUDI_Error_Code CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType);

/**
@brief ����ָ����Ƶ������Ϊ����

@param[in] nAudioIndex  Audio Decoder�豸������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û�ָ����Ƶ��������ʵ�־�������
@note ���Զ�ε��ã���ֻ�е�һ����Ч
*/
CSUDI_Error_Code CSUDIAUDIOMute(int nAudioIndex);

/**
@brief ȡ��ָ����Ƶ�������ľ���״̬

@param[in] nAudioIndex  Audio Decoder�豸������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û�ָ����Ƶ��������ʵ��ȡ�������Ĺ���
@note �ڷ�mute״̬�µ��ô˽ӿ���Ч
@note ���Զ�ε��ã���ֻ�е�һ����Ч
*/
CSUDI_Error_Code CSUDIAUDIOUnmute(int nAudioIndex);

/**
@brief ����ָ����Ƶ������������

@param[in] nAudioIndex  Audio Decoder�豸������
@param[in] nVolume �û���Ҫ���������ֵ����[0,CSUDI_MAX_VOLUME]����������,0Ϊ����,CSUDI_MAX_VOLUME���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��������豸��ͳһ��������
@note ����������Ч��Χ���ش���,������Ч
*/
CSUDI_Error_Code CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

