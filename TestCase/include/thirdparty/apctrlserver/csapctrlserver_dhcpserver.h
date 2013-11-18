/**@defgroup  
@brief 
1、本模块定义的ip地址采用形如"192.168.1.1"的ASII码字符串方式表示，定义最大长度为16个字节(最后一个字节为\0)
2、本模块定义的ip地址范围采用形如"192.168.1.1~255"的字符串方式表示，定义最大长度为20(最后一个字节为\0)
2、本模块使用的MAC地址采用形如{0x00,0x0e,0x09,0x11,0x12,0xe1}的无符号BYTE数组方式表示，定义的长度是6个字节

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _CS_APCTRL_SERVER_DHCPSERVER_H_
#define _CS_APCTRL_SERVER_DHCPSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csapctrlserver.h"

/**@brief DHCP Server分配地址信息定义*/
typedef struct{
	char aucIPStart[CSAPCTRLS_IP_LEN]; 	///<地址池开始IP地址
	char aucIPEnd[CSAPCTRLS_IP_LEN];  		///<地址池结束IP地址,(起始地址&mask) < (结束地址&mask)
	char aucMask[CSAPCTRLS_IP_LEN]; 		///<DHCP分配IP地址的子网掩码,字符串如"192.168.1.1"
	char aucGatway[CSAPCTRLS_IP_LEN]; 	///<默认网关,字符串如"192.168.1.1",要求起始地址,结束地址,网关在同一个网段
	unsigned int   uLeaseTime;  				///<地址租约时间,单位为秒
}CSAPCTRLSDHCPSConfig_S;

/**@brief 静态配置的IP地址和MAC地址对*/
typedef struct{
	char aucIP[CSAPCTRLS_IP_LEN]; 			///<IP地址,字符串如"192.168.1.1"
	unsigned char aucMac[CSAPCTRLS_MAC_LEN]; 		///<MAC地址,数值如{0x00,0x01,0x01,0x01,0x01,0x01}
}CSAPCTRLSDHCPSStaticIpCfg;

/**@brief DHCPServer已经分配的地址信息*/
typedef struct{
	char aucIP[CSAPCTRLS_IP_LEN]; 			///<IP地址,字符串如"192.168.1.1"
	unsigned char aucMac[CSAPCTRLS_MAC_LEN];		 ///<MAC地址,数值如{0x00,0x01,0x01,0x01,0x01,0x01}
	unsigned int 	nLeftTime;  					///<子网client租约地址有效期的剩余时间
}CSAPCTRLSDHCPSLeaseInfo;

/**
@brief 启动DHCP服务器

@param[in] szIfName :网卡设备名称
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSStart(const char* szIfName);

/**
@brief 停止DHCP服务器

@param[in] szIfName :网卡设备名称
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSStop(const char* szIfName);

/**
@brief DHCP服务是否运行中

@param[in] szIfName :网卡设备名称
@param[out] pbOutIsRunning DHCP服务是否运行中
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSIsRunning(const char* szIfName,CSUDI_BOOL* pbOutIsRunning);

/**
@brief 配置DHCP服务器参数

@param[in] szIfName :网卡设备名称
@param[in] psDHCPCfg 指向配置参数的指针
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSConfig(const char* szIfName,const CSAPCTRLSDHCPSConfig_S *psDHCPCfg);

/**
@brief 获取DHCP服务器的配置参数

@param[in] szIfName :网卡设备名称
@param[out] psOutDHCPCfg DHCP服务器的地址池和网关参数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSGetConfig(const char* szIfName,CSAPCTRLSDHCPSConfig_S *psOutDHCPCfg);

/**
@brief 向DHCP服务器添加静态配置的IP地址和MAC地址对,这样被添加的MAC DHCP时将获得指定的IP地址

@param[in] szIfName :网卡设备名称
@param[in] psItem 指向MAC和IP地址对结构数组的指针
@param[in] nCount 指定pstItem指针内存包含的CSAPCTRLSDHCPSStaticIpCfg结构个数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
@note 该函数可以同时添加多个静态配置,具体个数由nCount参数指定
*/
CSUDI_Error_Code  CSAPCTRLSDHCPSAddStaticIPCfg(const char* szIfName,const CSAPCTRLSDHCPSStaticIpCfg *psItem,int nCount);

/**
@brief 向DHCP服务器删除静态配置的IP地址和MAC地址对

@param[in] szIfName :网卡设备名称
@param[in] ptItem 指向MAC和IP地址对结构数组的指针
@param[in] nCount 指定pstItem指针内存包含的CSAPCTRLSDHCPSStaticIpCfg结构个数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
@note 该函数可以同时删除多个静态配置,具体个数由nCount参数指定
*/
CSUDI_Error_Code  CSAPCTRLSDHCPSDelStaticIPCfg(const char* szIfName,const CSAPCTRLSDHCPSStaticIpCfg *pstItem,int nCount);

/**
@brief 获取DHCP服务器的配置参数

@param[in] szIfName :网卡设备名称
@param[out] psItem 静态配置的机器信息
@param[in/out] pnStaticCount 输入输出参数 ,输入传入数组大小，输出实际获取到的个数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code  CSAPCTRLSDHCPSGetStaticIPCfgs(const char* szIfName,CSAPCTRLSDHCPSStaticIpCfg *psItem,int *pnStaticCount);

/**
@brief 获取DHCP服务器的已分配地址信息

@param[in] szIfName :网卡设备名称
@param[out] pstLeaseInfo 用户传入的保存已分配地址信息的指针，最大支持分配CSNET_MAX_DHCPS_LEASE_COUNT 个
@param[in/out] pnCount 作为输入参数时该指针保存参数psLeaseInfo数组的大小,作为输出参数时返回实际获取的个数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
@note 调用该函数时pnCount所指的对象一定要赋初值,否则运行结果不可预料
*/
CSUDI_Error_Code  CSAPCTRLSDHCPSGetLeaseInfo(const char* szIfName,CSAPCTRLSDHCPSLeaseInfo *pstLeaseInfo,int *pnCount);

#ifdef __cplusplus
}
#endif

#endif


