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
@brief 	请求NIT version 并且监听NIT version变化
@return 成功返回TRUE,失败则返回FALSE
*/
BOOL CSNITMStartRequestAndMonitor();
BOOL CSSDTMStartRequestAndMonitor();
BOOL CSBATMStartRequestAndMonitor();
/**
@brief 	根据version 监听NIT version是否发生变化
@param[in] ucVersion
@return 成功返回TRUE,失败则返回FALSE
*/
BOOL CSNITMStartMonitor(BYTE ucVersion);
BOOL CSSDTMStartMonitor(BYTE ucVersion);
BOOL CSBATMStartMonitor(BYTE ucVersion);
/**
@brief 	取消监听NIT version
@return 成功返回TRUE,失败则返回FALSE
*/
BOOL CSNITMStop();
/**
@brief 	注册回调
@param[in] fnCallback  回调函数指针
@param[in] dwUserData
@return 成功返回TRUE,失败则返回FALSE
*/
BOOL CSNITMAddCallback(CSOnMonitorNitCallback_F fnCallback, DWORD dwUserData);
BOOL CSSDTMAddCallback(CSOnMonitorNitCallback_F fnCallback, DWORD dwUserData);
BOOL CSBATMAddCallback(CSOnMonitorNitCallback_F fnCallback, DWORD dwUserData);
/**
@brief 	移除回调
@param[in] fnCallback  回调函数指针
@return 成功返回TRUE,失败则返回FALSE
*/

BOOL CSNITMRemoveCallback(CSOnMonitorNitCallback_F fnCallback);


#ifdef	__cplusplus
}
#endif

#endif 
