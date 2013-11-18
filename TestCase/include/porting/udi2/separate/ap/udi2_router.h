/**@defgroup  本模块对UDI2RT对外接口和数据结构进行了定义
@brief UDI2RT模块对应的头文件<udi2_router.h>,定义了router常用操作接口

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _UDI2_ROUTER_H_
#define _UDI2_ROUTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_error.h"

#define CSUDI_APRT_IP_LEN (16)				///<IP地址最大长度


enum
{
	CSUDIRT_ERROR_BAD_PARAMETER = 129 << 16, 			///<参数错误
	CSUDIRT_ERROR_NOT_SET_WAN_LAN,					///<未设置WAN口和LAN口
	CSUDIRT_ERROR_UNKNOW_SERVICE,					///<未知服务
	CSUDIRT_ERROR_EXCEED_MAX_NUMBER,				///<超过最大个数
	CSUDIRT_ERROR_SERVICE_IS_STOPED				///当前服务处于关闭状态
};

typedef enum{
	EM_RT_SERVICETYPE_DNS_PROXY, 	///< DNS代理服务， 使用LAN口启动DNS服务,pvStartParam为CSUDIRTDnsServers_S*类型，指定dns服务器地址
	EM_RT_SERVICETYPE_IGMP_PROXY, 	///< IGMP代理服务，上行为WAN口，下行为LAN口，pvStartParam暂未定义
	EM_RT_SERVICETYPE_UPNP,			///< UPNP服务，上行为WAN口，下行为LAN口
	EM_RT_SERVICETYPE_MAX
}CSUDIRTServiceType_E;

typedef struct{
	char acFirstDNS[CSUDI_APRT_IP_LEN]; 	 		///< IP地址。空字符串表示未设置该IP地址
	char acSecondDNS[CSUDI_APRT_IP_LEN]; 	 		///< IP地址。空字符串表示未设置该IP地址
}CSUDIRTDnsServers_S;


/**
@brief 设置wan口和lan口的名称

@param[in] szWan wan口名称
@param[in] szLan lan口名称
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSUDIRTSetInterfaces(const char* szWan,const char* szLan);

/**
@brief 获取wan口和lan口的名称

@param[in] pcWan : wan口名称
@param[in] nWanLen :pcWan数组长度
@param[in] pcLan  :Lan口名称
@param[in] nLanLen :pcLan数组长度
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSUDIRTGetInterfaces(char* pcWan,int nWanLen,char* pcLan,int nLanLen);

/**
@brief 设置LAN口IP地址

@param[in] szLan LAN口名称
@param[in] szIp  IP地址
@param[in] szMask  子网掩码
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSUDIRTSetLanIp(const char* szLan,const char* szIp,const char* szMask);

/**
@brief 启动指定网络服务

@param[in] eService 需要启动的网络服务类型
@param[in] pvUserdata 服务启动参数，见各个服务类型定义说明。
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSUDIRTStartService(CSUDIRTServiceType_E eService,void* pvStartParam);

/**
@brief 停止指定网络服务

@param[in] eService 需要停止的网络服务类型
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSUDIRTStopService(CSUDIRTServiceType_E eService);

/**
@brief 获取指定网络服务的状态

@param[in] eService 指定网络服务
@param[out] pbIsRunning 返回该网络服务的状态,TRUE为运行状态,FALSE为未启动状态
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSUDIRTGetServiceStatus(CSUDIRTServiceType_E eService,CSUDI_BOOL *pbIsRunning);

/**
@brief 执行iptable命令规则

@param[in] pszcmd iptable命令规则字符串
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code  CSUDIRTExecuteIptableCmd(const char *pszcmd);

#ifdef __cplusplus
}
#endif
#endif /* __CSLANPLUS_H__ */


