/**@defgroup �����ӿڣ�csshell_bluetooth��ģ���ƣ�CSSHELLBT
	�����������豸��Ϊ����������(Adapter)�����������������豸��Ϊ�����豸(Device)
	
ģ����BT
@version 2.0.1 2011/09/14 Ϊ�˱�֤�м����ͳһ�ԣ���������Ϊ����client�Ľӿ�,��һ�׽ӿڵĶ���ʹ��
��ʹ������udi�ӿ���ĳЩƽ̨��û��ʵ�֣�Ҳ����������Ӳ��������⡣
@{
*/
#ifndef _CSSHELL_BLUETOOTH_H_
#define _CSSHELL_BLUETOOTH_H_

#include "udi2_bluetooth.h"
#include "mmcp_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct 
{
	int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellBTInitParam_S)


	/**
	@brief ע�������¼���������
	@param[in] fnCb  �ص�����
	@param[in] pUserData �û��Զ������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	// ����ע���ص�
	CSUDI_Error_Code (* m_fnAddListener)(CSUDIBTCallback_F  fnCb, void* pUserData);
	
	/**
	@brief	 ɾ�������¼��ص�������
	@param[in] fnCb  �ص�����
	@param[in] pUserData �û��Զ������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnRemoveListener)(CSUDIBTCallback_F fnCb,void* pUserData);

	/**
	@brief ��ȡ��������������
	@param[out] ahOutHandleArray ��������������������
	@param[in] arraySize �����С
	@param[out] pnOutCount �����������������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note ʵ��������������������arraySizeʱ����������CSUDI_SUCCESS�����������*pnOutCountΪarraySize
	*/
	CSUDI_Error_Code (* m_fnGetAllAdapter)(CSUDI_HANDLE *ahOutHandleArray,unsigned int arraySize,unsigned int* pnOutCount);
	
	/**
	@brief ��ȡ������������Ϣ
	@param[in] hAdapter �������������
	@param[out] psOutInfo ���������������Ϣ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnGetAdapterInfo)(CSUDI_HANDLE hAdapter,CSUDIBTAdapterInfo_S* psOutInfo);

	/**
	@brief ��ȡ����������״̬
	@param[in] hAdapter �������������
	@param[out] peOutStatus �������������״̬
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnGetAdapterStatus)(CSUDI_HANDLE hAdapter ,  CSUDIBTAdapterStatus_E* peOutStatus);
	
	/**
	@brief ������������
	@param[in] hAdapter �������������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnTurnOn)(CSUDI_HANDLE hAdapter);
	
	/**
	@brief �ر�����������
	@param[in] hAdapter �������������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnTurnOff)(CSUDI_HANDLE hAdapter);
	
	/**
	@brief ��ȡ��ɨ�赽�����������豸
	@param[in] hAdapter �������������
	@param[out] asOutDevicesArray ��������豸��Ϣ����
	@param[in] nArrayCount �����豸��Ϣ�ṹ�������С
	@param[out] pnOutCount ��������豸����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note ʵ�������豸��������nArraySizeʱ����������CSUDI_SUCCESS�����������*pnOutCountΪarraySize
	*/
	CSUDI_Error_Code (* m_fnGetDevices)(CSUDI_HANDLE hAdapter,CSUDIBTDeviceInfo_S* asOutDevicesArray,unsigned int nArraySize,unsigned int* pnOutCount);
	
	/**
	@brief ����ɨ�������豸,ͬ���ӿ�
	@param[in] hAdapter �������������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻�CSUDIBluetooth_ERROR_SCAN_FAILED
	*/
	CSUDI_Error_Code (* m_fnStartScan)(CSUDI_HANDLE hAdapter);
	
	/**
	@brief ֹͣɨ�������豸
	@param[in] hAdapter �������������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnStopScan)(CSUDI_HANDLE hAdapter);

	/**
	@brief ���������豸���
	@param[in] hAdapter �������������
	@param[in] szDeviceAddress ����Ե������豸��ַ
	@param[in] szPassword ��������ַ���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnStartPair)(CSUDI_HANDLE  hAdapter,const char* szDeviceAddress,const char* szPassword);

	/**
	@brief �������������豸
	@param[in] hAdapter �������������
	@param[in] szDeviceAddress �����ӵ������豸��ַ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note ֻ������Ե��豸���ܽ������Ӳ���
	*/
	CSUDI_Error_Code (* m_fnStartConnect)(CSUDI_HANDLE  hAdapter,  const char* szDeviceAddress);

	/**
	@brief ɾ�������豸
	@param[in] hAdapter �������������
	@param[in] szDeviceAddress �����豸��ַ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note �ɹ�ɾ�����豸,�������Ϣͬ��ɾ��.��������ɨ��ſ���ͨ��CSUDIBTGetDevices�ӿڻ�á�
	*/
	CSUDI_Error_Code (* m_fnStartDelete)(CSUDI_HANDLE hAdapter, const char* szDeviceAddress);

	/**
	@brief ��ȡ�����豸״̬
	@param[in] hAdapter �������������
	@param[in] szDeviceAddress �����豸��ַ
	@param[out] peOutStatus �����豸״̬
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnGetDeviceStatus)(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,CSUDIBTDeviceStatus_E* peOutStatus);
	
	/**
	@brief ע��������������sdp����Э�麯��
	@param[in] pcSerialID ���к�
	@param[in] nPort �������������˿�
	@param[in] pcName ����������sdp����������޹�
	@param[in] pcDsc ��������������sdp����������޹�
	@param[in] pcProv �����ṩ��������sdp����������޹�
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnSdpRegister) (const char* pcSerialID, unsigned int nPort, const char* pcName, const char* pcDsc, const char* pcProv);

}CSShellBTInitParam_S;

/**
@brief ���� BLUETOOTH �ӿڲ���
@param psParam  BLUETOOTH  Shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellBTSetParam(CSShellBTInitParam_S * psParam);

/**
@brief ��ȡ BLUETOOTH �ӿڲ���
@param ��
@�ɹ�����CSShellBTInitParam_S,ʧ�ܷ���NULL
*/
const CSShellBTInitParam_S* CSShellBTgetParam();

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



