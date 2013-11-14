/**@defgroup AOUT AOUT ������Ƶ���ź�����ؽӿ�
@brief AOUTģ���Ӧͷ�ļ�<udi2_aout.h>����������Ƶ���ź�����ؽӿڡ�

���Audio��������������·����ͨ��mixer��Ϻ�����������Aout��������൱������ӿڵĳ�����hdmi��spdif�ȡ�Mixer����������صĿ�����Ҫ��δ���г���

Aout��Audioģ�����������Audio���ƽ������������Ĵ����ñ�����ˮ��������������������ģ���������������˿ڣ��ñȼ����ˮ��ͷ����������������� ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
		This module is used to control RCA, SPDIF, HDMI audio output, such as: 
			audio output mute;
			audio output unmute;
			audio output passthrough mode;
			audio output decoded mode;
-------------------------------------------------------------------------------

@version 2.0.2 2009/08/25 ����Aout�������弰λ��˵��
@version 2.0.1 2009/08/16 ����
@{
*/
#ifndef _UDI2_AOUT_H_
#define _UDI2_AOUT_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ��Ƶ���ƴ������ֵ*/
enum
{
	CSUDIAOUT_ERROR_BAD_PARAMETER = CSUDI_AOUT_ERROR_BASE,  ///< �������
	CSUDIAOUT_ERROR_INVALID_DEVICE_ID,						///< �Ƿ��豸ID
	CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED					///< ��֧�ִ˲���
};

/**@brief ��Ƶͨ������*/
typedef enum
{
	EM_UDIAOUT_RCA		= 1,			///<��Ƶ�������������
	EM_UDIAOUT_SPDIF	= 1 << 1,		///<��ƵSPDIF�����
	EM_UDIAOUT_HDMI 	= 1 << 2		///< HDMI��Ƶ���
}CSUDIAOUTDevices_E;


/**@brief ��Ƶͨ�������ģʽ*/
typedef enum
{
	EM_UDIAOUT_DECODE,	///<��Ƶ������Ҫ�������룬Ȼ�����
	EM_UDIAOUT_BYPASS	///<��Ƶ��������Ҫ�������ֱ��������ɺ������豸���磺��SPDIF����Ĺ��ţ���ɽ���Ͳ��š�
}CSUDIAOUTMode_E;

/**
@brief ����ָ����Ƶ���ͨ��Ϊ����

@param[in] eAudioDevice  ��Ƶ ���ͨ����������μ�CSUDIAOUTDevices_E
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û��趨��ָ����Ƶͨ����ʵ�־�������
@note ���Զ�ε��ã���ֻ�е�һ����Ч
*/
CSUDI_Error_Code CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice);

/**
@brief ȡ��ָ����Ƶ���ͨ���ľ���״̬

@param[in] eAudioDevice ��Ƶ���ͨ����������μ�CSUDIAOUTDevices_E
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���û��趨��ָ����Ƶ�豸��ʵ��ȡ�������Ĺ���
@note �ڷ�mute״̬�µ��ô˽ӿ���Ч
@note ���Զ�ε��ã���ֻ�е�һ����Ч
*/
CSUDI_Error_Code CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice);

/**
@brief ����ָ�����ͨ�������ģʽ

@param[in] eAudioDevice ��Ƶ���ͨ����������μ�CSUDIAOUTDevices_E
@param[in] eMode �豸�����ģʽ��������μ�CSUDIAOUTMode_E��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ȱʡ����£�������Ƶ���ͨ��Ϊ��������ģʽ��������������ֻ���ǽ�������ģʽ��
@note �����ǰ���ڲ���״̬�����øýӿ�������Ч�����������������ݵ�ֹͣ
*/
CSUDI_Error_Code CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

