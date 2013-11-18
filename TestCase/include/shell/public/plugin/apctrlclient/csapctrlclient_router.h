/**@defgroup  本模块对UDI2RT对外接口和数据结构进行了定义
@brief UDI2RT模块对应的头文件<udi2_router.h>,定义了router常用操作接口

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _CS_APCTRL_CLIENT_ROUTER_H_
#define _CS_APCTRL_CLIENT_ROUTER_H_

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	CSAPCTRLCRT_ERROR_BAD_PARAMETER = 130 << 16, 			///<参数错误
	CSAPCTRLCRT_ERROR_UNKNOW_SERVICE,					///<未知服务
	CSAPCTRLCRT_ERROR_EXCEED_MAX_NUMBER,				///<超过最大个数
	CSAPCTRLCRT_ERROR_SERVICE_IS_STOPED				///当前服务处于关闭状态
};

typedef enum{
	EM_APCTRLCRT_SERVICETYPE_DNS_PROXY, 	///< DNS代理服务， 使用LAN口启动DNS服务
	EM_APCTRLCRT_SERVICETYPE_IGMP_PROXY, 	///< IGMP代理服务，上行为WAN口，下行为LAN口
	EM_APCTRLCRT_SERVICETYPE_UPNP 			///< UPNP服务，上行为WAN口，下行为LAN口
}CSAPCTRLCRTServiceType_E;

/**
@brief 设置wan口和lan口的名称

@param[in] szWan wan口名称
@param[in] szLan lan口名称
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSAPCTRLCRTSetInterfaces(const char* szWan,const char* szLan);

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
CSUDI_Error_Code CSAPCTRLCRTGetInterfaces(char* pcWan,int nWanLen,char* pcLan,int nLanLen);

/**
@brief 设置LAN口IP地址

@param[in] szLan LAN口名称
@param[in] szIp  IP地址
@param[in] szMask  子网掩码
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSAPCTRLCRTSetLanIp(const char* szLan,const char* szIp,const char* szMask);

/**
@brief 启动指定网络服务

@param[in] eService 需要启动的网络服务类型
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSAPCTRLCRTStartService(CSAPCTRLCRTServiceType_E eService);

/**
@brief 停止指定网络服务

@param[in] eService 需要停止的网络服务类型
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSAPCTRLCRTStopService(CSAPCTRLCRTServiceType_E eService);

/**
@brief 获取指定网络服务的状态

@param[in] eService 指定网络服务
@param[out] pbIsRunning 返回该网络服务的状态,TRUE为运行状态,FALSE为未启动状态
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSAPCTRLCRTGetServiceStatus(CSAPCTRLCRTServiceType_E eService,CSUDI_BOOL *pbIsRunning);

/**
@brief 执行iptable命令规则

@param[in] pszcmd iptable命令规则字符串
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code  CSAPCTRLCRTExecuteIptableCmd(const char *pszcmd);

/**
@brief 设置DNS服务器地址

@param[in] szFirstDnsServerIp 首选DNS服务器地址。参数应为字符串格式如:"192.168.1.1"
@param[in] szSecondDnsServerIp 备选DNS服务器地址。参数应为字符串格式如:"192.168.1.1"
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
@note
1、szFirstDnsServerIp不能为NULL
2、szSecondDnsServerIp为NULL时，表示未设置备选DNS服务器地址
*/
CSUDI_Error_Code CSAPCTRLRTSetDnsServer(const char* szFirstDnsServerIp,const char* szSecondDnsServerIp);

/**
@brief 设置wan口地址

@param[in] szWanIp wan口地址。参数应为字符串格式如:"192.168.1.1"
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回 错误代码
*/
CSUDI_Error_Code CSAPCTRLRTSetWanIp(const char* szWanIp);


#ifdef __cplusplus
}
#endif
#endif /* __CSLANPLUS_H__ */


