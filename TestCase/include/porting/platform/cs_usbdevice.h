#ifndef _CS_USB_DEVICE_H
#define _CS_USB_DEVICE_H

#include "cs_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief �豸��Ϣ�ṹ*/
typedef struct 
{
    	 int  m_nIsUsed;        ///<�Ƿ�����
    	 char m_szUSBType[128];     	 ///<�豸����
	 char m_szUSBDescription[256]; ///< �豸
}CSUSBInfo_S;	



/**
@brief ��ȡ��ǰ�����USB �豸����
@param[out] pdwUsbDeviceCount ��ǰ�����USB �豸����

@return �ɹ���ȡ�򷵻�TRUE��ʧ���򷵻�FALSE
*/
BOOL CSUSBGetCount(unsigned int * pdwUsbDeviceCount);

/**
@brief �����豸������ȡUSB �豸��Ϣ

@param[in] uIndex USB �豸����ID ��ȡֵ��Χ��(0~31)
@param[out] psUsbInfo ָ�򷵻�USB �豸����Ϣ
@return �ɹ���ȡ����TRUE��ʧ���򷵻�FALSE
*/
BOOL CSUSBGetInfo(unsigned int uIndex,CSUSBInfo_S * psUsbInfo);


#ifdef __cplusplus
}
#endif

/** @} */

#endif
