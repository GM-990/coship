/**@defgroup  本模块对UDI2DHCPSERVER对外接口和数据结构进行了定义
@brief UDI2DHCPSERVER模块对应的头文件<udi2_dhcpserver.h>,定义了DHCPSERVER常用操作接口
1、本模块使用的ip地址表示方式是点分ip地址，定义最大长度为16个字节如:"192.168.1.1" 这种表示方式
2、本模块使用的MAC地址，定义的长度是6个字节如:{0x00,0x0e,0x09,0x11,0x12,0xe1}

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _UDI2_DHCPSERVER_H_
#define _UDI2_DHCPSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_error.h"

#define CSUDI_IP_LEN (16)				///<IP地址最大长度
#define CSUDI_MAC_LEN (6)				///<MAC地址长度

/**@brief  DHCP server错误类型枚举定义*/
enum
{    
	CSUDIDHCPS_ERROR_BAD_PARAMETER = 130 << 16, 		///<参数错
	CSUDIDHCPS_ERROR_NOT_CONFIG,  ///未进行基本DHCP server配置就启动DHCP server
	CSUDIDHCPS_ERROR_MORE_THAN_MAX ///设置超过驱动支持的最大值
};
/**@brief DHCP Server分配地址信息定义*/
typedef struct{
	char acIPStart[CSUDI_IP_LEN]; 	///<地址池开始IP地址
	char acIPEnd[CSUDI_IP_LEN];  		///<地址池结束IP地址,(起始地址&mask) < (结束地址&mask)
	char acMask[CSUDI_IP_LEN]; 		///<DHCP分配IP地址的子网掩码,字符串如"192.168.1.1"
	char acGatway[CSUDI_IP_LEN]; 	///<默认网关,字符串如"192.168.1.1",要求起始地址,结束地址,网关在同一个网段
	char acFirstDNS[CSUDI_IP_LEN]; ///首选DNS服务器IP地址,字符串格式,首选DNS必须配置
	char acSecondDNS[CSUDI_IP_LEN]; ///备用DNS服务器IP地址,字符串格式,备选DNS可以不配置,不配置时赋空串
	unsigned int   uLeaseTime;  				///<地址租约时间,单位为秒
}CSUDIDHCPSConfig_S;

/**@brief 静态配置的IP地址和MAC地址对*/
typedef struct{
	char acIP[CSUDI_IP_LEN]; 			///<IP地址,字符串如"192.168.1.1"
	unsigned char aucMac[CSUDI_MAC_LEN]; 		///<MAC地址,数值如{0x00,0x01,0x01,0x01,0x01,0x01}
}CSUDIDHCPSStaticIpCfg;

/**@brief DHCPServer已经分配的地址信息*/
typedef struct{
	char acIP[CSUDI_IP_LEN]; 			///<IP地址,字符串如"192.168.1.1"
	unsigned char aucMac[CSUDI_MAC_LEN];		 ///<MAC地址,数值如{0x00,0x01,0x01,0x01,0x01,0x01}
	unsigned int 	nLeftTime;  					///<子网client租约地址有效期的剩余时间
}CSUDIDHCPSLeaseInfo;

/**
@brief 启动DHCP服务器

@param[in] szIfName :网卡设备名称
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIDHCPSStart(const char* szIfName);

/**
@brief 停止DHCP服务器

@param[in] szIfName :网卡设备名称
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIDHCPSStop(const char* szIfName);

/**
@brief 配置DHCP服务器参数

@param[in] szIfName :网卡设备名称
@param[in] psDHCPCfg 指向配置参数的指针
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIDHCPSConfig(const char* szIfName,const CSUDIDHCPSConfig_S *psDHCPCfg);

/**
@brief 获取DHCP服务器的配置参数

@param[in] szIfName :网卡设备名称
@param[out] psOutDHCPCfg DHCP服务器的地址池和网关参数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIDHCPSGetConfig(const char* szIfName,CSUDIDHCPSConfig_S *psOutDHCPCfg);

/**
@brief 向DHCP服务器添加静态配置的IP地址和MAC地址对,这样被添加的MAC DHCP时将获得指定的IP地址

@param[in] szIfName :网卡设备名称
@param[in] psItem 指向MAC和IP地址对结构数组的指针
@param[in] nCount 指定pstItem指针内存包含的CSUDIDHCPSStaticIpCfg结构个数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
@note 该函数可以同时添加多个静态配置,具体个数由nCount参数指定
@note 如果添加的IP不在地址池范围内则，该地址映射不会在分配的时候生效
*/
CSUDI_Error_Code  CSUDIDHCPSAddStaticIPCfg(const char* szIfName,const CSUDIDHCPSStaticIpCfg *psItem,int nCount);

/**
@brief 向DHCP服务器删除静态配置的IP地址和MAC地址对

@param[in] szIfName :网卡设备名称
@param[in] ptItem 指向MAC和IP地址对结构数组的指针
@param[in] nCount 指定pstItem指针内存包含的CSUDIDHCPSStaticIpCfg结构个数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
@note 该函数可以同时删除多个静态配置,具体个数由nCount参数指定
*/
CSUDI_Error_Code  CSUDIDHCPSDelStaticIPCfg(const char* szIfName,const CSUDIDHCPSStaticIpCfg *pstItem,int nCount);

/**
@brief 获取DHCP服务器的配置参数

@param[in] szIfName :网卡设备名称
@param[out] psItem 静态配置的机器信息
@param[in/out] pnStaticCount 输入输出参数 ,输入传入数组大小，输出实际获取到的个数
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code  CSUDIDHCPSGetStaticIPCfgs(const char* szIfName,CSUDIDHCPSStaticIpCfg *psItem,int *pnStaticCount);

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
CSUDI_Error_Code  CSUDIDHCPSGetLeaseInfo(const char* szIfName,CSUDIDHCPSLeaseInfo *pstLeaseInfo,int *pnCount);

#ifdef __cplusplus
}
#endif

#endif


