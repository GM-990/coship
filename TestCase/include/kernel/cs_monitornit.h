#ifndef _CS_MONITOR_NIT_H_
#define _CS_MONITOR_NIT_H_



#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

#define 	EM_MSG_NIT_VERSION_CHANGE  0x11
#define     EM_MSG_SDT_VERSION_CHANGE  0x12
#define     EM_MSG_BAT_VERSION_CHANGE  0x13

typedef void (*CSOnMonitorNitCallback_F)(int nMsg ,int nParam, DWORD dwUserData);

typedef void (*CSOnMonitorSDTCallback_F)(int nMsg ,int nParam, DWORD dwUserData);

typedef void (*CSOnMonitorBATCallback_F)(int nMsg ,int nParam, DWORD dwUserData);

/**
@brief 	����NIT version ���Ҽ���NIT version�仯
@return �ɹ�����TRUE,ʧ���򷵻�FALSE
*/
BOOL CSNITMStartRequestAndMonitor();
BOOL CSSDTMStartRequestAndMonitor();
BOOL CSBATMStartRequestAndMonitor();
/**
@brief 	����version ����NIT version�Ƿ����仯
@param[in] ucVersion
@return �ɹ�����TRUE,ʧ���򷵻�FALSE
*/
BOOL CSNITMStartMonitor(BYTE ucVersion);
BOOL CSSDTMStartMonitor(BYTE ucVersion);
BOOL CSBATMStartMonitor(BYTE ucVersion);
/**
@brief 	ȡ������NIT version
@return �ɹ�����TRUE,ʧ���򷵻�FALSE
*/
BOOL CSNITMStop();
/**
@brief 	ע��ص�
@param[in] fnCallback  �ص�����ָ��
@param[in] dwUserData
@return �ɹ�����TRUE,ʧ���򷵻�FALSE
*/
BOOL CSNITMAddCallback(CSOnMonitorNitCallback_F fnCallback, DWORD dwUserData);
BOOL CSSDTMAddCallback(CSOnMonitorNitCallback_F fnCallback, DWORD dwUserData);
BOOL CSBATMAddCallback(CSOnMonitorNitCallback_F fnCallback, DWORD dwUserData);
/**
@brief 	�Ƴ��ص�
@param[in] fnCallback  �ص�����ָ��
@return �ɹ�����TRUE,ʧ���򷵻�FALSE
*/

BOOL CSNITMRemoveCallback(CSOnMonitorNitCallback_F fnCallback);


#ifdef	__cplusplus
}
#endif

#endif 
