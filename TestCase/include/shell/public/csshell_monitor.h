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
	int m_nSize;	///<本结构大小，一定等于sizeof(CSShellMonitorInitParam)

	/**
	@brief 	请求NIT version 并且监听NIT version变化
	@return 成功返回TRUE,失败则返回FALSE
	*/
	BOOL (* m_fnStartRequestAndMonitor)();
	/**
	@brief 	根据version 监听NIT version是否发生变化
	@param[in] ucVersion
	@return 成功返回TRUE,失败则返回FALSE
	*/
	BOOL (* m_fnStartMonitor)(BYTE ucVersion);
	/**
	@brief 	取消监听NIT version
	@return 成功返回TRUE,失败则返回FALSE
	*/
	BOOL (* m_fnStopMonitor)();
	/**
	@brief 	注册回调
	@param[in] fnCallback  回调函数指针
	@param[in] dwUserData
	@return 成功返回TRUE,失败则返回FALSE
	*/
	BOOL (* m_fnAddMonitorCallback)(CSShellMonitorNitCallback_F fnCallback, DWORD dwUserData);
	/**
	@brief 	移除回调
	@param[in] fnCallback  回调函数指针
	@return 成功返回TRUE,失败则返回FALSE
	*/

	BOOL (* m_fnRemoveMonitorCallback)(CSShellMonitorNitCallback_F fnCallback);

}CSShellMonitorInitParam_S;

/**
@brief 设置 Monitor 接口参数
@param psParam  Monitor  Shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellMonitorSetParam(CSShellMonitorInitParam_S * psParam);

/**
@brief 获取 Monitor 接口参数
@param 无
@成功返回CSShellMonitorInitParam_S,失败返回NULL
*/
const CSShellMonitorInitParam_S* CSShellMonitorGetParam();


#ifdef __cplusplus
}
#endif

#endif

