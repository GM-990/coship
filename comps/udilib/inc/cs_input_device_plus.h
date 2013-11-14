/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

#ifndef __CS_INPUT_DEVICE_PLUS_H__
#define __CS_INPUT_DEVICE_PLUS_H__

#include "cs_hdicommon_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief �����豸����ö�ٶ��� */
typedef enum {
	CS_INPUT_DEVICE_MOUSE=1<<0,                //���
	CS_INPUT_DEVICE_KEYBOARD=1<<1,		//����
	CS_INPUT_DEVICE_JOYSTICK=1<<2,		//��Ϸ�ֱ�
	CS_INPUT_DEVICE_MAX	=1<<3
}CSInputDeviceType_E;

/**@brief �����豸���ݽṹ���� */
typedef struct  {
	char	m_caName[64];	 //�����豸����
	CSInputDeviceType_E	m_eInputType; //�����豸����
	char m_caEventName[64]; //�����豸��Ӧ���¼����ƣ�����·�������ڴ򿪸��豸������/dev/input/mice
	int m_nVendorId;	//�����豸����������ID
	int m_nProductId; // �����豸�Ĳ�ƷID
	int m_nDevFd; //linuxϵͳ�����豸��Ӧ���ļ������ͨ��nDevfd =open(caEvenNamet) ��ȡ�������Ը��豸�Ĳ�������Ҫ�þ��	
}InputDevice_S;

 /**@brief ���������Ϸ�ֱ�ʱ�䶨�� */
typedef struct {
     unsigned long    tv_sec;
     unsigned long    tv_usec;
 }CSInputDeviceTimeval_S;

/**@brief ���������Ϸ�ֱ����������¼����������Ͷ��� */
typedef struct {
    unsigned int m_uDeviceIndex;   ///<�¼��������豸��ţ���������ͬһ���͵Ķ���豸
    CSInputDeviceTimeval_S m_stTime;      ///<�¼�������ʱ��
    unsigned short m_ushEventType;      ///< �¼�����
    unsigned short m_ushCode;      ///< �¼���Ӧ������
    signed int m_nValue;///<�¼�Я����״̬����. ����m_ushEventType�Ĳ�ͬ����������
}CSInputDeviceParam_S ;

/**
@brief Input device module ��ʼ����
@return CSHDI_SUCCESS ��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ�
*/
CSHDI_Error_t CSInputDeviceInit(void);

/**
@briefע����ꡢ���̡��ֱ���input device �ص�������input device ����������������λ�õ���Ϣͨ���ص��������ݸ�Ӧ��

@param[in] fnCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�input device �������ֵ������λ�õ���Ϣ�����CSHDICallback���塣
@return CSHDI_SUCCESS��ע��ɹ�;CSHDI_ERROR_BAD_PARAMETER�������������
@note
*/
CSHDI_Error_t CSInputDeviceRegisterKeySendCallback(CSHDICallback fnCallback);


#ifdef __cplusplus
}
#endif

#endif

