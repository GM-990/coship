/**@defgroup MDHCP 本模块对DHCP对外接口和数据结构进行了定义
@brief MDHCP模块对应的头文件<cs_mdhcpc.h>,定义了DHCP常用操作接口，支持DHCP RFC2131,
RFC2132和RFC1533标准

1、本接口支持多实例，网卡设备名称作为唯一标识符
2、CSMDHCPInit函数会创建一个实例，所有的接口的调用必须在CSMDHCPInit函数之后
3、本模块的实现依据RFC2131、RFC1533和RFC2132，获取到IP地址、子网掩码和网关
4、本模块所除常见option之外，还支持几种特殊的option选项:

        操作码           描述                                                     用途      
        
        60                      Class-identifier                                         用来做终端、服务端双方校验、DHCP服务器划分子网等
        43                      Vendor specific information                         下发某个服务器地址,用来收视率调查等
        151-155              private                                                     Coship标准，用来存放应用下载服务器地址，现在用于广东省网、天威
        125                     Vendor-Identifying Vendor-Specific.             终端对服务器端进行校验
如果客户需要支持上面描述的这几个option选项，必须调用CSMDHCPSetSendOption来设置，如果需要客户端也验证，则还需要
调用CSMDHCPSetRecvOption来设置客户端校验字段

5、常见option对照表，这些option不需要用户设置，DHCP 模块发包就会带上这些option

        操作码     			      描述                                                           
		1			 		      掩码
		3						网关
		6						DNS服务器
		51						DHCP租期信息
		53						DHCP服务器 IP地址
		55						DHCP 请求选项列表
5、用户从自动获取ip地址切换到静态设置IP地址，必须调用CSMDHCPStop来停止DHCP后再设置静态IP地址
6、用户从静态IP地址切换到自动获取IP,直接调用CSMDHCPStart来获取IP地址

@version 2.0.1 2011/11/14 
@{
*/

#ifndef __MDHCPC_H_INCLUDED__
#define __MDHCPC_H_INCLUDED__

#include "mmcp_typedef.h"
#include "csshell_mdhcpc.h"

#if defined(__cplusplus) || defined(__CPLUSPLUS)
extern "C" {
#endif 

/**
@brief 初始化DHCP

@param[in] pcEthName:网卡设备名称
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
@note 该接口不会启动dhcp，仅仅是初始化动作
*/
int  CSMDHCPInit(char * pcEthName,CSMDHCPInitPara_S *psInitPara);

/**
@brief 设置是否采用DHCP获取的DNS，gateway，默认为采用。

@param[in] pcEthName : 对应设置的网卡名。
@param[in] bEnableDNS : 设置是否采用DHCP服务器分配的DNS，TRUE表示采用，FALSE表示不采用
@param[in] bEnableGW : 设置是否采用DHCP服务器分配的gateway,TRUE表示采用，FALSE表示不采用
@return 
- 如果成功返回CSUDI_SUCCESS
- 如果失败返回CSUDI_FAILURE
@note 此接口在CSMDHCPInit之后，CSMDHCPStart之前调用
*/
int CSMDHCPDnsGwSet(char * pcEthName, BOOL bEnableDNS, BOOL bEnableGW);


/**
@brief 启动DHCP

@param[in] pcEthName:网卡设备名称
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
@note 本接口异步执行，返回成功仅表示命令已接收，不表示命令执行结果。
*/
int  CSMDHCPStart(char * pcEthName);

/**
@brief 停止DHCP

@param[in] pcEthName:网卡设备名称
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
@note 本接口异步执行，返回成功仅表示命令已接收，不表示命令执行结果。
*/
int  CSMDHCPStop(char * pcEthName);

/**
@brief 设置DHCP option选项,设置之后，发包就会带上所设置的选项

@param[in] pcEthName:网卡设备名称
@param[in] nOptCode:Option选项的操作码,操作码范围0-255，各个操作码代表的含义请参见RFC2132
@param[in] pOptVal:Option选项的值
@param[in] nOptLen:Option选项的长度
@return 
- 成功返回CSUDI_SUCCESS。
- 失败返回CSUDI_FAILURE
*/
int CSMDHCPSetSendOption(char * pcEthName,  int nOptCode, void *pOptVal,int nOptLen);

/**
@brief 设置DHCP option选项，设置之后，客户端将会对DHCP服务器应答的数据包做相应的校验

@param[in] pcEthName:网卡设备名称
@param[in] nOptCode:Option选项的操作码,操作码范围0-255，各个操作码代表的含义请参见RFC2132
@param[in] pOptVal:Option选项的值
@param[in] nOptLen:Option选项的长度
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
*/
int CSMDHCPSetRecvOption(char * pcEthName,  int nOptCode, void *pOptVal,int nOptLen);

/**
@brief 获取DHCP option选项

@param[in] pcEthName:网卡设备名称
@param[in] nOptCode:Option选项的操作码,操作码范围0-255，各个操作码代表的含义请参见RFC2132
@param[out] pOptVal:用来存放获取的option 选项的值，buffer的长度不能小于256
@param[out] nOptLen:pOptVal所指向的存储空间的大小
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
*/
int CSMDHCPGetOption(char * pcEthName,  int nOptCode,void *pOptVal, int nOptLen);

/**
@brief 设置DHCP报文的发包间隔

@param[in] pcEthName 网卡设备名称
@param[in] ePacketType: 包类型，详见CSMDHCPPacketType_E 定义
@param[in] fnGetSendInterval: 函数指针，获取发包时间间隔，该函数由用户实现，返回值单位:秒(s),如果不设置，将采用默认值序列发包
@return
- 如果成功返回0。
- 如果失败返回-1
@note  不设置，DHCPDISCOVER报文将采用默认时间间隔，默认时间间隔[1,2,4,8,16,32,64,128,256],循环
*/
int CSMDHCPSetPacketInterval(char * pcEthName,CSMDHCPPacketType_E ePacketType, CSOnMDHCPGetSendInterval_F fnGetSendInterval);

/**
@brief 增加DHCP续约模式

@param[in] pcEthName 网卡设备名称
@param[in] nSendTime: 发送RENEW报文的时间，租约的万分比，例如:天威是75%续约，需要传7500
@param[in] eSendType: 发包模式，详见CSMDHCPPacketMode_E定义
@return
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
@note  按照RFC2131标准，DHCP模块在50%租约发送单播包续约，续约不成功，在87.5%发送广播包续约，\n
续约不成功，等待租约满的时候重新发送DHCPDISCOVER 报文
@note  各个项目在标准的基础上加上了私有的发包方式，目前有两种需求
@note  天威需求:50%续约不成功的时候，增加在75%租约的时候发送单播包续约
@note 上海电信需求:50%续约不成功的时候，增加在68.75%发送单播包续约，87.5%续约不成功的时候，增加在93.75%发送广播包续约
*/
int CSMDHCPAddRenewPacketSendMode(char * pcEthName,int nSendTime,CSMDHCPPacketMode_E eSendType); 

/**
@brief  获取DHCP服务器主机名字段

@param[in] pcEthName 网卡设备名称
@param[out] pucServerHostName: 用户设置的服务器主机名字段的值，长度不超过64字节
@param[in] dwLen pucServerHostName所指向的存储空间的大小
@return 
- 成功返回CSUDI_SUCCESS
- 失败返回CSUDI_FAILURE
@note:有的局方将VOD服务器的地址放在这个字段中下发下来，应用需要调用该接口获取vod服务器的地址去点播
*/
int CSMDHCPGetServerHostName(char * pcEthName,BYTE *pucServerHostName,DWORD dwLen);

/**
@brief 添加DHCP通知回调函数

@param[in] fnCallback 回调函数,当DHCP状态发生变化时调用该函数
@param[in] pvUserData 用户数据,将作为回调函数的pvUserData参数返回
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
@note 添加回调前产生的DHCP状态变化不会补充通知
*/
int CSMDHCPAddCallback(char * pcEthName,CSOnMDHCPChange_F fnCallback,void* pvUserData);

/**
@brief 删除DHCP回调函数

@param[in] fnCallback 回调函数,之前注册的函数
@param[in] pvUserData 用户数据,必须与注册时的一致
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
@note fnCallback和pUserData一起唯一确定一个回调函数
*/
int CSMDHCPRemoveCallback(char * pcEthName,CSOnMDHCPChange_F fnCallback);

#if defined(__cplusplus) || defined(__CPLUSPLUS)
}
#endif

#endif 

