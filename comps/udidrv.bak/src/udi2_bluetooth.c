/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_bluetooth.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_bluetooth.h"
/**
@brief ע�������¼���������
@param[in] fnCb  �ص�����
@param[in] pUserData �û��Զ������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTAddListener(CSUDIBTCallback_F fnCb,void* pUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief ��ȡ��������������
@param[out] ahOutHandleArray ��������������������
@param[in] dwArraySize �����С
@param[out] pdwOutCount �����������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ʵ��������������������dwArraySizeʱ����������CSUDI_SUCCESS�����������*pdwOutCountΪdwArraySize
*/
CSUDI_Error_Code CSUDIBTGetAllAdapter(CSUDI_HANDLE *ahOutHandleArray,unsigned int dwArraySize,unsigned int* pdwOutCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief ��ȡ������������Ϣ
@param[in] hAdapter �������������
@param[out] psOutInfo ���������������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTGetAdapterInfo(CSUDI_HANDLE hAdapter,CSUDIBTAdapterInfo_S* psOutInfo)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ������������������
@param[in] hAdapter �������������
@param[in] szName ���õ���������������,�ַ������Ȳ��ܳ���CSUDIBT_STR_LEN
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTSetAdapterName(CSUDI_HANDLE hAdapter, const char *szName)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}
/**
@brief ��ȡ����������״̬
@param[in] hAdapter �������������
@param[out] peOutStatus �������������״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTGetAdapterStatus(CSUDI_HANDLE hAdapter,CSUDIBTAdapterStatus_E* peOutStatus)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ������������
@param[in] hAdapter �������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTTurnOn(CSUDI_HANDLE hAdapter)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �ر�����������
@param[in] hAdapter �������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTTurnOff(CSUDI_HANDLE hAdapter)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ���������豸,��������ɨ�赽���豸
@param[in] hAdapter �������������
@param[out] psOutDevicesArray ��������豸��Ϣ����
@param[in] dwArraySize �����豸��Ϣ�ṹ�������С
@param[out] pdwOutCount ��������豸����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ʵ�������豸��������nArraySizeʱ����������CSUDI_SUCCESS�����������*pdwOutCountΪdwArraySize
*/
CSUDI_Error_Code CSUDIBTGetDevices(CSUDI_HANDLE hAdapter,CSUDIBTDeviceInfo_S* psOutDevicesArray,unsigned int dwArraySize,unsigned int* pdwOutCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ɾ�������豸
@param[in] hAdapter �������������
@param[in] szDeviceAddress �����豸��ַ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �ɹ�ɾ�����豸,�������Ϣͬ��ɾ��.��������ɨ��ſ���ͨ��CSUDIBTGetDevices�ӿڻ�á�
*/
CSUDI_Error_Code CSUDIBTDeleteDevice(CSUDI_HANDLE hAdapter, const char* szDeviceAddress)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����ɨ�������豸
@param[in] hAdapter �������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ɨ������У�ÿɨ�赽һ�������豸������EM_UDIBT_EVENT_TYPE_FOUND_DEVICE��Ϣ��ɨ����ɻ���ɨ����ֹ����EM_UDIBT_EVENT_TYPE_SCAN_OK��Ϣ��
*/
CSUDI_Error_Code CSUDIBTStartScan(CSUDI_HANDLE hAdapter)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ֹͣɨ�������豸
@param[in] hAdapter �������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTStopScan(CSUDI_HANDLE hAdapter)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ���������豸���
@param[in] hAdapter �������������
@param[in] szDeviceAddress ����Ե������豸��ַ
@param[in] szPassword ��������ַ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTStartPair(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,const char* szPassword)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �������������豸
@param[in] hAdapter �������������
@param[in] szDeviceAddress �����ӵ������豸��ַ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ֻ������Ե��豸���ܽ������Ӳ���
*/
CSUDI_Error_Code CSUDIBTStartConnect(CSUDI_HANDLE hAdapter,const char* szDeviceAddress)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�����豸״̬
@param[in] hAdapter �������������
@param[in] szDeviceAddress �����豸��ַ
@param[out] peOutStatus �����豸״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTGetDeviceStatus(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,CSUDIBTDeviceStatus_E* peOutStatus)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ע��������������sdp����Э�麯��
@param[in] pcSerialID ���к�
@param[in] nPort �������������˿�
@param[in] pcName ����������sdp����������޹�
@param[in] pcDsc ��������������sdp����������޹�
@param[in] pcProv �����ṩ��������sdp����������޹�
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTSdpRegister(const char* pcSerialID, unsigned int dwPort, const char* pcName, const char* pcDsc, const char* pcProv)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


