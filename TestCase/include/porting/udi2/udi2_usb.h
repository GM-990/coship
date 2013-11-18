/**@defgroup USB USB  ������USB������Ϣ��ؽӿ�
@brief usbģ���Ӧͷ�ļ�udi2_usb.h��������USB�豸��Ϣ��ؽӿ�

@version 2.0.2 2011/12/21 �����˴������ö�ٶ���
@version 2.0.1 2011/03/08 ����

@{
*/

#ifndef _UDI2_USB_H_
#define _UDI2_USB_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ���ش������ö�ٶ���*/
enum {
	CSUDIUSB_ERROR_BAD_PARAMETER = CSUDI_USB_ERROR_BASE,		///< �����쳣���� 
	CSUDIUSB_ERROR_FEATURE_NOT_SUPPORTED,						///< ��֧�ִ˲��� 
	CSUDIUSB_ERROR_INVALID_DEVICE_ID,							///< �Ƿ��豸ID
	CSUDIUSB_ERROR_UNKNOWN_ERROR								///< δ֪����
};

/**@brief �豸��Ϣ�ṹ*/
typedef struct
{
	CSUDI_BOOL   m_bIsUsed;        	///<�Ƿ�����
	char m_szUSBType[128];     	 	///<�豸����
	char m_szUSBDescription[256]; 	///<�豸����
}CSUDIUSBInfo_S;	

/**
@brief ��ȡ��ǰ�����USB �豸����
@param[out] pdwUsbDeviceCount ��ǰ�����USB �豸����
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIUSBGetCount(unsigned int* pdwUsbDeviceCount);

/**
@brief �����豸������ȡUSB �豸��Ϣ

@param[in] uIndex USB �豸����ID����0��ʼ���ֵΪCSUDIUSBGetCount��ȡ����count-1
@param[out] psUsbInfo ָ�򷵻�USB�豸����Ϣ
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIUSBGetInfo(unsigned int uIndex, CSUDIUSBInfo_S * psUsbInfo);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

