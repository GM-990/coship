#ifndef CSSHELL_MONITOR_H
#define CSSHELL_MONITOR_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define 	EM_MSG_NIT_VERSION_CHANGE  0x11

typedef void (*CSShellMonitorNitCallback_F)(int nMsg ,int nParam, DWORD dwUserData);

typedef struct 
{
	int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellMonitorInitParam)

	/**
	@brief 	����NIT version ���Ҽ���NIT version�仯
	@return �ɹ�����TRUE,ʧ���򷵻�FALSE
	*/
	BOOL (* m_fnStartRequestAndMonitor)();
	/**
	@brief 	����version ����NIT version�Ƿ����仯
	@param[in] ucVersion
	@return �ɹ�����TRUE,ʧ���򷵻�FALSE
	*/
	BOOL (* m_fnStartMonitor)(BYTE ucVersion);
	/**
	@brief 	ȡ������NIT version
	@return �ɹ�����TRUE,ʧ���򷵻�FALSE
	*/
	BOOL (* m_fnStopMonitor)();
	/**
	@brief 	ע��ص�
	@param[in] fnCallback  �ص�����ָ��
	@param[in] dwUserData
	@return �ɹ�����TRUE,ʧ���򷵻�FALSE
	*/
	BOOL (* m_fnAddMonitorCallback)(CSShellMonitorNitCallback_F fnCallback, DWORD dwUserData);
	/**
	@brief 	�Ƴ��ص�
	@param[in] fnCallback  �ص�����ָ��
	@return �ɹ�����TRUE,ʧ���򷵻�FALSE
	*/

	BOOL (* m_fnRemoveMonitorCallback)(CSShellMonitorNitCallback_F fnCallback);

}CSShellMonitorInitParam_S;

/**
@brief ���� Monitor �ӿڲ���
@param psParam  Monitor  Shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellMonitorSetParam(CSShellMonitorInitParam_S * psParam);

/**
@brief ��ȡ Monitor �ӿڲ���
@param ��
@�ɹ�����CSShellMonitorInitParam_S,ʧ�ܷ���NULL
*/
const CSShellMonitorInitParam_S* CSShellMonitorGetParam();


#ifdef __cplusplus
}
#endif

#endif

