/**@defgroup �����ӿڣ�udi_bluetooth��ģ���ƣ�CSUDIBT
	�����������豸��Ϊ����������(Adapter)�����������������豸��Ϊ�����豸(Device)
	
ģ����BT

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.1.0 2011/10/18 ���Ӷ���ص���Ϣ���ӿ�,���¶���CSUDIBTGetDevices�ӿ�
@version 2.0.1 2011/06/21 ��������Ϊ����client�Ľӿ�
@{
*/
#ifndef _UDI2_BLUETOOTH_H_
#define _UDI2_BLUETOOTH_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_tuner.h"

#ifdef  __cplusplus
extern "C" {
#endif


#define CSUDIBT_STR_LEN 64 				///< ����ģ��ʹ�õ��ַ������ȣ������ַ������ַ�ַ�����

typedef enum _CSUDIBTProfileType_E{
	CSUDIBT_PROFILE_A2DP = 0x01, ///< ����Advanced Audio Distribution Profile���߼���Ƶ����ģ�ͣ����������������������֣���
	CSUDIBT_PROFILE_HFP = 0x02,  ///< ����Headset profile���ֻ�ģ�ͣ������ֻ�����ͨ����
	CSUDIBT_PROFILE_HSP = 0x04,  ///< ����Hands Free Profile������ģ�ͣ���HFP��������չ��ĳЩ���ܣ���ʼ���ڳ���ϵͳ��
	CSUDIBT_PROFILE_HID = 0x08   ///< ����Human Interface Device Profile����ꡢ���̵������豸��ͨ�Ž������߻���
}CSUDIBTProfileType_E;

/**@brief ����������״̬ö��ֵ��*/
typedef enum _CSUDIBTAdapterStatus_E{
	EM_UDIBT_ADAPTER_STATUS_DOWN,		///< �ر�״̬
	EM_UDIBT_ADAPTER_STATUS_UP			///< ����״̬
}CSUDIBTAdapterStatus_E;


/**@brief ������Ϣ֪ͨ�¼�����ö��ֵ��*/
typedef enum _CSUDIBTEventType_E{
	EM_UDIBT_EVENT_TYPE_SCAN_OK,	///< ɨ���豸�ɹ�,�ص�pvEventDataΪNULL
	EM_UDIBT_EVENT_TYPE_SCAN_FAILED,///< ɨ���豸ʧ��,�ص�pvEventDataΪNULL
	EM_UDIBT_EVENT_TYPE_CONNECT_OK,	///< �豸���ӳɹ�,�ص�pvEventDataΪCSUDIBTDeviceInfo_Sָ��
	EM_UDIBT_EVENT_TYPE_CONNECT_FAILED,	///< �豸����ʧ��,�ص�pvEventDataΪCSUDIBTDeviceInfo_Sָ��
	EM_UDIBT_EVENT_TYPE_CONNECT_LOST, ///< ���Ӷ�ʧ,�ص�pvEventDataΪCSUDIBTDeviceInfo_Sָ��
	EM_UDIBT_EVENT_TYPE_FOUND_DEVICE,	///< �����豸,�ص�pvEventDataΪCSUDIBTDeviceInfo_Sָ��
	EM_UDIBT_EVENT_TYPE_PAIR_OK,	///< ��Գɹ�,�ص�pvEventDataΪCSUDIBTDeviceInfo_Sָ��
	EM_UDIBT_EVENT_TYPE_PAIR_FAILED	///< ���ʧ��,�ص�pvEventDataΪCSUDIBTDeviceInfo_Sָ��
}CSUDIBTEventType_E;

/**@brief �����豸״̬ö��ֵ��*/
typedef enum _CSUDIBTDeviceStatus_E{
	EM_UDIBT_DEVICE_STATUS_NOTMATCH,		///< δ���
	EM_UDIBT_DEVICE_STATUS_MATCHED,		///< ����Ե�δ����
	EM_UDIBT_DEVICE_STATUS_CONNECTED,	///< ������
	EM_UDIBT_DEVICE_STATUS_NOTEXIST		///< �豸�Ѳ�����
}CSUDIBTDeviceStatus_E;


/**@brief ������������Ϣ��*/
typedef struct _CSUDIBTAdapterInfo_S{   
    char szName[CSUDIBT_STR_LEN];		///< ��������������
    char szAddress[CSUDIBT_STR_LEN];	///< ������������ַ
}CSUDIBTAdapterInfo_S;

/**@brief �����豸��Ϣ��*/
typedef struct _CSUDIBTDeviceInfo_S{
	char szName[CSUDIBT_STR_LEN];		///< �����豸����
    char szAddress[CSUDIBT_STR_LEN];	///< �����豸��ַ
    unsigned int nProfileMask;			///< �����豸profile���ԣ�֧�ֶ���profile������
}CSUDIBTDeviceInfo_S;



/**@brief �����豸�������ֵ*/
enum
{
	CSUDIBluetooth_ERROR_BAD_PARAMETER = CSUDI_BLUETOOTH_ERROR_BASE,    ///< �������
	CSUDIBluetooth_ERROR_INVALID_DEVICE_ID,							///< �Ƿ��豸ID
	CSUDIBluetooth_ERROR_FEATURE_NOT_SUPPORTED,					///< ��֧�ִ˲���
	CSUDIBluetooth_ERROR_ADAPTER_NOT_UP,						///< ������δ����
	CSUDIBluetooth_ERROR_DEVICE_NOT_EXIST						///< �豸������
};
	
/**
@brief ����֪ͨ�ص�������
@param[in] hAdapter  �¼�֪ͨ�����������������
@param[in] eEvent �¼�����
@param[in] pvEventData �¼�����
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ޡ�
*/
typedef void (*CSUDIBTCallback_F)(CSUDI_HANDLE hAdapter,CSUDIBTEventType_E eEvent,void* pvEventData,void * pvUserData);

/**
@brief ע�������¼���������
@param[in] fnCb  �ص�����
@param[in] pUserData �û��Զ������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTAddListener(CSUDIBTCallback_F fnCb,void* pUserData);

/**
@brief ��ȡ��������������
@param[out] ahOutHandleArray ��������������������
@param[in] dwArraySize �����С
@param[out] pdwOutCount �����������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ʵ��������������������dwArraySizeʱ����������CSUDI_SUCCESS�����������*pdwOutCountΪdwArraySize
*/
CSUDI_Error_Code CSUDIBTGetAllAdapter(CSUDI_HANDLE *ahOutHandleArray,unsigned int dwArraySize,unsigned int* pdwOutCount);

/**
@brief ��ȡ������������Ϣ
@param[in] hAdapter �������������
@param[out] psOutInfo ���������������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTGetAdapterInfo(CSUDI_HANDLE hAdapter,CSUDIBTAdapterInfo_S* psOutInfo);

/**
@brief ������������������
@param[in] hAdapter �������������
@param[in] szName ���õ���������������,�ַ������Ȳ��ܳ���CSUDIBT_STR_LEN
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTSetAdapterName(CSUDI_HANDLE hAdapter, const char *szName);

/**
@brief ��ȡ����������״̬
@param[in] hAdapter �������������
@param[out] peOutStatus �������������״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTGetAdapterStatus(CSUDI_HANDLE hAdapter,CSUDIBTAdapterStatus_E* peOutStatus);

/**
@brief ������������
@param[in] hAdapter �������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTTurnOn(CSUDI_HANDLE hAdapter);

/**
@brief �ر�����������
@param[in] hAdapter �������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTTurnOff(CSUDI_HANDLE hAdapter);


/**
@brief ��ȡ���������豸,��������ɨ�赽���豸
@param[in] hAdapter �������������
@param[out] psOutDevicesArray ��������豸��Ϣ����
@param[in] dwArraySize �����豸��Ϣ�ṹ�������С
@param[out] pdwOutCount ��������豸����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ʵ�������豸��������nArraySizeʱ����������CSUDI_SUCCESS�����������*pdwOutCountΪdwArraySize
*/
CSUDI_Error_Code CSUDIBTGetDevices(CSUDI_HANDLE hAdapter,CSUDIBTDeviceInfo_S* psOutDevicesArray,unsigned int dwArraySize,unsigned int* pdwOutCount);


/**
@brief ɾ�������豸
@param[in] hAdapter �������������
@param[in] szDeviceAddress �����豸��ַ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �ɹ�ɾ�����豸,�������Ϣͬ��ɾ��.��������ɨ��ſ���ͨ��CSUDIBTGetDevices�ӿڻ�á�
*/
CSUDI_Error_Code CSUDIBTDeleteDevice(CSUDI_HANDLE hAdapter, const char* szDeviceAddress);


/**
@brief ����ɨ�������豸
@param[in] hAdapter �������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ɨ������У�ÿɨ�赽һ�������豸������EM_UDIBT_EVENT_TYPE_FOUND_DEVICE��Ϣ��ɨ����ɻ���ɨ����ֹ����EM_UDIBT_EVENT_TYPE_SCAN_OK��Ϣ��
*/
CSUDI_Error_Code CSUDIBTStartScan(CSUDI_HANDLE hAdapter);

/**
@brief ֹͣɨ�������豸
@param[in] hAdapter �������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTStopScan(CSUDI_HANDLE hAdapter);


/**
@brief ���������豸���
@param[in] hAdapter �������������
@param[in] szDeviceAddress ����Ե������豸��ַ
@param[in] szPassword ��������ַ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTStartPair(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,const char* szPassword);

/**
@brief �������������豸
@param[in] hAdapter �������������
@param[in] szDeviceAddress �����ӵ������豸��ַ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ֻ������Ե��豸���ܽ������Ӳ���
*/
CSUDI_Error_Code CSUDIBTStartConnect(CSUDI_HANDLE hAdapter,const char* szDeviceAddress);

/**
@brief ��ȡ�����豸״̬
@param[in] hAdapter �������������
@param[in] szDeviceAddress �����豸��ַ
@param[out] peOutStatus �����豸״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTGetDeviceStatus(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,CSUDIBTDeviceStatus_E* peOutStatus);

/**
@brief ע��������������sdp����Э�麯��
@param[in] pcSerialID ���к�
@param[in] nPort �������������˿�
@param[in] pcName ����������sdp����������޹�
@param[in] pcDsc ��������������sdp����������޹�
@param[in] pcProv �����ṩ��������sdp����������޹�
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIBTSdpRegister(const char* pcSerialID, unsigned int dwPort, const char* pcName, const char* pcDsc, const char* pcProv);


#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



