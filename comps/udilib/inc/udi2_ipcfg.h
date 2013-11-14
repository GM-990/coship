/**@defgroup IPCFG IPCFG 本模块描述了网卡及协议栈相关接口函数和数据定义。
@brief 网络配置模块对应头文件<udi2_ipcfg.h>，包括获取有线、无线网卡的信息及设置IP设置 等操作。在MMCP（CloudTV ）中间件平台等部分功能简单些的产品中，MMCP（CloudTV ）中间件平台仅需使用基础网络部分的接口进行网络连接，获取数据等操作。在功能更强大的MMCP（CloudTV ）中间件平台产品中，MMCP（CloudTV ）中间件平台会使用该部分的接口对网络进行设置，如设置IP地址，获取网络连接状态等。

在本模块中所有网卡已通过与网卡名称进行唯一标识，操作也通过网卡名称参数进行控制。部分操作系统如Linux支持一个网卡设置多个IP，这种情况下可将该网卡抽象成两个完全独立的网卡，如eth0:1，eth0:2。和两个真正独立的网卡相比，其区别在于它们的Mac地址和他们的插入、拔出状态完全一样。

所有网卡的命名规则由平台驱动决定，但其长度不得超出接口文档中描述的最大长度。

UDI不提供设置网卡MAC地址的接口，如果网卡芯片没有内置MAC地址，终端需在进入MMCP（CloudTV ）中间件平台之前将其配置好，如在UDI接口初始化的时候，从Flash中读取出厂时设置到其中的MAC地址，并将其设置到网卡中。

- 如果系统中没有无线网卡，在获取网卡时不需要提供无线网卡即可。
- 如果系统中没有任何网卡，本模块获取不到任何网卡。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.6 2012/11/23 增加可拔插网卡消息监听接口
@version 2.0.5 2009/08/31 删除无线网卡的认证消息结构信息
@version 2.0.4 2009/08/25 增加了无线网卡的注册回调接口
@version 2.0.3 2009/08/25 删除设置Mac地址接口,因为该接口属于配置项
@version 2.0.2 2009/08/24 综合评审后的版本
@version 0.0.1 2009/08/12 演示版本
@{
*/
#ifndef _UDI2_IPCFG_H_
#define _UDI2_IPCFG_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"

#ifdef __cplusplus
extern "C" {
#endif


/**@brief 有线和无线网卡连接断开

@note bit [7] ... [3]... [2] [1] ...[0]
@note ...............................0 ..: link up
@note ...............................1 ..: link down  
*/
#define CSUDI_ETHMASK_UP_DOWN       		  	(unsigned short)0x1U   


/**@brief 有线和无线网卡传输速率

@note bit [7] ... [3]... [2] [1] ...[0]
@note ................... 0   0 .......................: 100BASE
@note ................... 0   1 .......................: 10BASE
@note ................... 1   0 .......................: 54M无线网卡
@note ................... 1   1 .......................: 未知传输速率
*/
#define CSUDI_ETHMASK_LINKSPEED     			(unsigned short)0x6U   


/**@brief 有线和无线网卡全半双工定义

@note bit [7] ... [3]... [2] [1] ...[0]
@note .............0 ................................: FULL
@note .............1 ................................: HALF
*/
#define CSUDI_ETHMASK_FULL_HALF     			((unsigned short)0x8U)   

#define CSUDI_ETHER_NETIFNAME_MAX_LEN   		(16)      			///< 网卡名称字符串的最大长度
#define CSUDI_ETHER_IPADDR_MAX_LEN   		       (16)      			///< IP地址字符串的最大长度
#define CSUDI_ETHER_DNS_MAX_SUPPORT   		(4)      			///< DNS支持的最大个数

#define CSUDI_MAX_LAN_CHIP_NUMBER             		(2)     			///< 机顶盒支持的最大网卡数  
#define CSUDI_WLAN_ESSID_MAX_LEN 				(32)    			///< 无线网卡认证的ESSID字符串的长度
#define CSUDI_WLAN_ENCKEY_MAX_LEN 			(64)    			///< 无线网卡认证的密钥的字符串的长度
#define CSUDI_WLAN_WPA_MSG_LEN 				(128)  			///< WPA认证程序消息的长度
#define CSUDI_WLAN_APNAME_MSX_LEN 			(32)   			///< AP ESSID的长度
#define CSUDI_WLAN_APMAC_MAX_LEN                     (32)                      ///< AP MAC的长度


/**@brief IPCFG模块错误代码值 */
enum
{
   CSUDIIPCFG_ERROR_BAD_PARAMETER = CSUDI_IPCFG_ERROR_BASE, ///< 参数错误
   CSUIDIPCFG_ERROR_FEATURE_NOT_SUPPORTED,                  		///< 操作不支持
   CSUDIIPCFG_ERROR_UNKNOWN_ERROR,							///< 未知错误
   CSUDIIPCFG_ERROR_NO_MEMORY,								///< 无内存可用错误
   CSUDIIPCFG_ERROR_NETIFUP_FAIL ,								///< 无线网卡启动失败
   CSUDIIPCFG_ERROR_NETIFDOWN_FAIL,							///< 无线网卡关闭失败
   CSUDIIPCFG_ERROR_CONNECTAP_ERROR,       						///< 连接错误
   CSUDIIPCFG_ERROR_SETKEY_ERROR,              						///< 设置密钥错误	
   CSUDIIPCFG_ERROR_CLEANKEY_ERROR,         						///< 清除认证信息错误
   CSUDIIPCFG_ERROR_GETINFO_ERROR,							///< 获得信息错误
   CSUDIIPCFG_ERROR_GETAPNAME_ERROR,	   						///< 没有获取到AP名称
   CSUDIIPCFG_ERROR_ALREADY_ADDED	                       			 ///< 回调函数已经注册
};

typedef enum
{
	EM_UDIRAWSENDTYPE_UNICAST = 0,//单播
	EM_UDIRAWSENDTYPE_BROADCAST,//广播
	EM_UDIRAWSENDTYPE_UNDEFINED
} CSUDIRawSendType_E;

/**@brief 网卡类型*/
typedef enum 
{
   EM_UDIETHER_TYPE_LAN = 0,  									///< 有线网卡
   EM_UDIETHER_TYPE_WLAN,										///< 无线网卡
   EM_UDIEHTER_TYPE_PPPOE,                                      //pppoe
   EM_UDIEHTER_TYPE_AP,										///无线AP
   EM_UDIEHTER_TYPE_UNKNOWN                                       ///未知类型网卡
}CSUDIEtherType_E;


/**@brief 无线网卡认证的加密类型*/
typedef enum 
{
   EM_UDIWLAN_ENC_NONE = 0,  									///< 无加密
   EM_UDIWLAN_ENC_WEPOPENAUTO,								///< WEPOpen加密
   EM_UDIWLAN_ENC_WEPSHARE,									///< shared共享模式来加密WEP信息
   EM_UDIWLAN_ENC_WPAPSK_TKIP,								///<WPAPSK 认证方式和TKIP加密算法
   EM_UDIWLAN_ENC_WPAPSK_AES,								///<WPAPSK 认证方式和AES加密算法
   EM_UDIWLAN_ENC_WPA2PSK_TKIP,								///<WPA2PSK 认证方式和TKIP加密算法
   EM_UDIWLAN_ENC_WPA2PSK_AES,								///<WPA2PSK 认证方式和AES加密算法
   EM_UDIWLAN_ENC_UNKNOW
}CSUDIWLanEncType_E;


/**@brief 无线网卡密钥的数据类型*/
typedef enum     
{	
    EM_UDIWLAN_KEY_HEX = 0,   									///< 16进制
    EM_UDIWLAN_KEY_ANSI,       									///< ASCII码
    EM_UDIWLAN_KEY_STRING    									///< 保留字
 }CSUDIWLanKeyType_E;


/**@brief AP的WPA认证的状态*/
typedef enum 
{
    EM_UDIWLAN_WPA_STATUS_UP = 0,								///< WPA认证成功
    EM_UDIWLAN_WPA_STATUS_DOWN 								///< WPA认证失败
}CSUDIWLanWPAStatus_E;


/**@brief 无线网卡WPA认证返回的消息以及回调函数返回给应用的消息类型*/
typedef enum
{
    EM_UDIWLAN_WPA_DISCONNECTED = 0,  						///< WPA断开连接
    EM_UDIWLAN_WPA_INACTIVE,	 								///< WPA处于非激活状态
    EM_UDIWLAN_WPA_SCANNING, 									///< 扫描网络
    EM_UDIWLAN_WPA_ASSOCIATING, 								///< 试图连上BSS/SSID
    EM_UDIWLAN_WPA_ASSOCIATED, 								///< 连接完成
    EM_UDIWLAN_WPA_4WAY_HANDSHAKE, 							///< WPA4的组密钥握手
    EM_UDIWLAN_WPA_GROUP_HANDSHAKE, 						///< WPA的组密钥握手
    EM_UDIWLAN_WPA_COMPLETED, 								///< 鉴定完成
    EM_UDIWLAN_WPA_SET_ERR,									///< 设置错误
    EM_UDIWLAN_WPA_KEY_ERR, 									///< 密钥错误
    EM_UDIWLAN_WPA_NO_FOUND_AP, 								///< 未找到AP
    EM_UDIWLAN_WPA_FINISH,									///< WPA完成
    EM_UDIWLAN_WEP_STATE_UP,									///< 状态启动
    EM_UDIWLAN_WEP_STATE_ERROR,								///< 状态错误
    EM_UDIWLAN_NONE_ENC_UP,									///< 不加密
    EM_UDIWLAN_CONN_TIMEOUT,									///< 连接超时
    EM_UDIWLAN_DOWN,											///< 网络断开
    EM_UDIWLAN_UP,												///< 网络连接成功
    EM_UDIWLAN_APLOST,											///< AP缺失
    EM_UDIWLAN_APCONNECT										///< AP成功连接
}CSUDIWLanMsgInfo_E;



/**@brief 无线网卡的配置信息*/
typedef struct 
{
    char szEthName[CSUDI_ETHER_NETIFNAME_MAX_LEN+4];		///< 无线网卡的名称
    char szEssId[CSUDI_WLAN_ESSID_MAX_LEN+4];			///< 认证的ESSID
    char szKey[CSUDI_WLAN_ENCKEY_MAX_LEN+4];				///< 认证的密钥
    CSUDIWLanEncType_E 	eEncType;                     			///< 认证的加密类型
    CSUDIWLanKeyType_E 	eKeyType;                     			///< 认证的密钥的数据类型
}CSUDIWLanConfig_S;


/**@brief 无线AP的信息*/
typedef struct 
{
    char szApName[CSUDI_WLAN_APNAME_MSX_LEN]; 			///< AP ESSID
    int    nSignalStrength;                             ///< AP信号强度（0～100）
    int    nLinkQuality;                                ///< AP连接质量（0～100）
    CSUDIWLanEncType_E eApEncType;                      			///< AP的加密方式
    char szApMac[CSUDI_WLAN_APMAC_MAX_LEN+4];                  ///< AP的MAC
}CSUDIWLanApInfo_S;

/**@brief 网卡的芯片信息*/
typedef struct
{
    unsigned int      		uLanManuFactory;   					///< 生产厂家ID                   
    CSUDIEtherType_E    eEtherType ;	          				///< 网卡类型
}CSUDIEtherInfo_S;

/**@brief 网卡的统计信息*/
typedef struct
{
    unsigned int uConnectTime;          ///< 连接持续时间
    unsigned int uTotalBytesSend;       ///< 发送包字节数
    unsigned int uTotalBytesReceived;   ///< 接收包字节数
    unsigned int uTotalPacketSend;      ///< 发送包个数
    unsigned int uTotalPacketReceived;  ///< 接发包个数
}CSUDIEtherStat_S;

/**@brief 无线网卡回调函数

@param[in] eMsgType 无线网卡消息类型，详见CSUDIWLanMsgInfo_E定义
@param[in] pcEthName 无线网卡的名字
@param[in] pvUserData 用户数据类型
*/
typedef void (*CSUDIEtherCallback_F)(CSUDIWLanMsgInfo_E eMsgType,char * pcEthName,void * pvUserData);


 /**
@brief 注册无线网卡的回调函数

@param[in] fnEtherCallback 回调函数指针，详见CSUDIEtherCallback_F定义
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 回调函数句柄和用户数据唯一确定一个回调函数，当两次注册它们完全一样时，第二次将返回CSUDIIPCFG_ERROR_ALREADY_ADDED
@note 至少支持注册的回调函数个数为32个，最大不限
*/
CSUDI_Error_Code CSUDIIPCFGAddEtherCallback(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData);


 /**
@brief 删除无线网卡的回调函数

@param[in] fnEtherCallback 回调函数指针，详见CSUDIEtherCallback_F定义
@param[in] pvUserData 同CSUDIIPCFGAddEtherCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGDelEtherCallback(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData);


/**
@brief 获得网卡数，支持有线和无线网卡和pppoe。如果带有pppoe功能，数量应该是实际的网卡数加上ppp0
@param[out] pnCount 获取到的网卡数目
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/ 
CSUDI_Error_Code CSUDIIPCFGGetEtherCount(int* pnCount);


/**
@brief 获取网卡设备名，支持有线和无线网卡和pppoe。

@param[in] nEthIndex 网卡设备索引号，如 0，1 索引号
@param[in] nLen pcEthName所指空间的长度，必须大于0小于等于16
@param[out] pcEthName 获取到的网卡设备名，由用户申请空间，长度必须大于0小于等于16
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherName(int nEthIndex,char * pcEthName,int nLen );


/**
@brief 获得网卡芯片的信息，包括制造商和类型等。

@param[in] pcEthName 网卡名，例如:eth0
@param[out] psChipInfo 见CSUDIEtherInfo_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherChipInfo(char * pcEthName, CSUDIEtherInfo_S * psChipInfo);


/**
@brief 通过设备名称来获取网卡物理地址，支持有线和无线网卡。

@param[in] pcEthName 网卡名，例如:eth0
@param[out] pucMacAddress 指向一个MAC地址的字符型数组, 格式为{0x00, 0x0E, 0xDB, 0x11, 0x22, 0x33}类
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 1.udi2没有制定设置mac地址的接口，因为设置mac地址不应该是udi2关心的事情，而应该是系统在启动时就执行的动作。
@note 换句话说，mac地址在调用该接口之前就已经确定。这里获取的mac地址，也必须与系统启动时设置进的保持一致。
*/
CSUDI_Error_Code CSUDIIPCFGGetMacByName(char * pcEthName,unsigned char * pucMacAddress);


/**
@brief 获取有线和无线网卡的连接状态。

@param[in] pcEthName 指向一个代表网卡的字符串，如"eth0"
@param[out] pushLinkState 连接状态
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 有线网卡的连接状态:
@note 1、pushLinkState & CSUDI_ETHMASK_UP_DOWN : 为0代表已经连接，为1代表未连接；
@note 2、pushLinkState & CSUDI_ETHMASK_100BASE_10BASE:为00代表100Mbps，为01代表10Mbps，为10代表无线54Mbps，为11代表未知速率；
@note 3、pushLinkState & CSUDI_ETHMASK_FULL_HALF: 为0代表全双工，为1代表半双工。
@note 无线网卡的连接状态:
@note 1、pushLinkState & CSUDI_ETHMASK_UP_DOWN : 为0代表已经连接，为1代表未连接。
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherStatus(char * pcEthName, unsigned short * pushLinkState);


/**
@brief 搜索网络内存在的所有AP的信息，只用于无线网卡。

@param[in] pcEthName 无线网卡的名称
@param[in] psWlanApInfo 存储AP信息的结构体数组
@param[in] pnWlanApInfoNum psWlanApInfo结构体数组的个数
@param[out] psWlanApInfo 存储实际获取到的AP信息的结构体数组
@param[out] pnWlanApInfoNum 实际获取到的AP信息的有效个数 	
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGSearchAP(char * pcEthName,CSUDIWLanApInfo_S * psWlanApInfo, int * pnWlanApInfoNum);


/**
@brief 与AP建立连接并且进行认证，只用于无线网卡。

@param[in] psWLanConf 无线网卡的配置参数数据结构，包括无线网卡的名称，认证的ESSID字符串，认证的密钥的字符串和长度，认证的类型，密钥的数据类型
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGConnectAP(CSUDIWLanConfig_S * psWLanConf);


/**
@brief 断开与AP的连接，只用于无线网卡。

@param[in] pcEthName 无线网卡的名称
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGDisconnectAP(char * pcEthName);


/**
@brief 获取AP信号的质量和强度。

@param[out] pnWlanApSigQulity AP信号的质量
@param[out] pnWlanApSigStrength AP信号的强度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 此接口建议废弃掉，若需获取信息，应该在CSUDIIPCFGSearchAP 的时候保存信息
*/
CSUDI_Error_Code CSUDIIPCFGGetAPSignalQulityStrength(int * pnWlanApSigQulity,int * pnWlanApSigStrength);


/**
@brief 设置DNS的地址。

@param[in] nDNSIndex DNS的索引号，如 0，1 ，最多支持4个
@param[in] pcDNS DNS的IP地址（如"192.168.99.5"的点分十进制IP地址)
@param[in] nLen DNS的IP地址字符串长度，不小于16 
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGSetDNSServer(int nDNSIndex,char * pcDNS,int nLen);


/**  
@brief 获得DNS的地址。

@param[in] nDNSIndex DNS的索引号，如 0，1，最多支持4个
@param[in] nLen DNS的IP地址字符串长度，不小于16 
@param[out] pcDNS DNS的IP地址(如"192.168.99.5"的点分十进制IP地址），pcDNS的空间由用户分配
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGGetDNSServer(int nDNSIndex,char * pcDNS,int nLen);


/**
@brief 设置网卡的IP地址，子网掩码和网关地址，支持有线和无线网卡。

@param[in] pcEthName 网卡设备名，如:"eth0"，若为CSUDI_NULL，则返回错误
@param[in] pcIp IP地址字符串，如"192.168.1.1" ，若为CSUDI_NULL，则返回错误    
@param[in] pcMask 子网掩码字符串，如"255.255.255.0"，可以为CSUDI_NULL
@param[in] pcGateWay 网关地址字符串，如"192.168.1.254"，可以为CSUDI_NULL
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGSetIpInfo(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay);


/**
@brief 获取网卡的IP地址、子网掩码和网关地址，支持有线和无线网卡。

@param[in] pcEthName 网卡设备名
@param[out] pcIp 获取到的IP地址，长度必须不小于16
@param[out] pcMask 获取到的mask地址，长度必须不小于16
@param[out] pcGateWay 获取到的gateway地址，长度必须不小于16
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note: 
         以上三个参数只要有一个不为空，就获取相应的值，成功返回CSUDI_SUCCESS 
         如果pcIp==pcMask==pcGateWay==CSUDI_NULL，会返回CSUDIIPCFG_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIIPCFGGetIpInfo(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay);


/**
@brief 将应用层传下来的包经过处理交给网卡驱动层发送出去。

@param[in] pszEthName 网卡设备名称
@param[in] fd socket描述符。(*fd==-1的时候，在此函数内创建socket,否则与sosket无关)
@param[in] pcDhcpBuf  应用层数据/以太网包数据
@param[in] nBufLen buffer的长度
@param[in] nFlag 是否需要此函数添加udp/ip/ethernet头 nFlag =1,需要添加，nFlag =0,直接发送
@return 成功返回 0,失败返回 -1
*/
int  CSUDIRawSendPacket(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag);


/**
@brief 将应用层传下来的包经过处理交给网卡驱动层发送出去,根据用户设置的参数eRawSendType决定发送的包是单播包还是广播包。

@param[in] pszEthName 网卡设备名称
@param[in] fd socket描述符。(*fd==-1的时候，在此函数内创建socket,否则与sosket无关)
@param[in] pcDhcpBuf  应用层数据/以太网包数据
@param[in] nBufLen buffer的长度
@param[in] nFlag 是否需要此函数添加udp/ip/ethernet头 nFlag =1,需要添加，nFlag =0,直接发送
@param[in] eRawSendType 定义发送包时是单播，还是广播
@return 成功返回 0,失败返回 -1
*/
int  CSUDIRawSendPacketByMode(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag, CSUDIRawSendType_E eRawSendMode);
/**
@brief 接收从网卡驱动传上来的包，交给应用层

@param[in] pszEthName 网卡设备名称
@param[in] fd socket描述符。(*fd==-1的时候，在此函数内创建socket,否则与sosket无关)
@param[out] pchBuf  需要接收的数据
@param[in,out] pnBufLen buffer的长度.输入参数时为pchBuf的缓冲区长度，输出参数为pchBuf的有效数据长度
@param[in] nFlag  是否需要此函数去掉udp/ip/ethernet头  nFlag =1,需要去掉，nFlag =0,直接交给上一层
@return 成功返回 实际收到的字节数,失败返回 -1
*/
int CSUDIRawRecvPacket (char *pszEthName, int  *fd, const char *pchBuf, int* pnBufLen, int nFlag);


/**
@brief 获取网卡统计信息
@param[in] pcName 网卡设备名称
@param[out] psNetStatus 存储实际获取到的网卡统计信息的结构体
@return 成功返回CSUDI_SUCCESS 失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGGetStatInfo(char *pcName,CSUDIEtherStat_S *psNetStatus);


/**
@brief 启用网卡
@param[in] pcEthName 网卡设备名称
@param[out] 无
@return 成功返回CSUDI_SUCCESS 失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGNICUp(char *pcEthName);


/**
@brief 禁用网卡
@param[in] pcEthName 网卡设备名称
@param[out] 无
@return 成功返回CSUDI_SUCCESS 失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGNICDown(char *pcEthName);

/*-----------------以下接口定义可插拔网卡的相关接口---------------------*/
/**
@brief 添加可插拔网卡通知回调函数

详见RDI模块CSUDIRDIAddCallback_F接口定义，仅用于处理“网卡”类型的可移动设备
@param[in] fnCallback 回调函数,当设备状态发生变化时调用该函数
@param[in] pvUserData 用户数据,将作为回调函数的pvUserData参数返回
@return 成功添加返回CSUDI_SUCCESS,失败返回错误代码值
@note 添加回调前产生的设备变化信息,不会补充通知.即若开机时这些设备就存在,调用本函数时不会得到通知.
@note 若平台不需要支持任何可插拔网卡，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
@note 在CSUDIRDICallback_F回调中，可插拔网卡设备的EM_UDIRDI_EVENT_READY消息中，pEventData为ethname (网卡名称字符串)，可用于本头文件上面定义的使用网卡名的接口
*/
CSUDI_Error_Code CSUDIIPCFGRdiAddCallback(CSUDIRDICallback_F fnCallback, void* pvUserData);

/**
@brief 删除可插拔网卡通知回调函数

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“网卡”类型的可移动设备
@param[in] fnCallback 回调函数,之前注册的函数
@param[in] pvUserData 用户数据,必须与注册时的一致
@return 成功删除返回CSUDI_SUCCESS,失败返回错误代码值
@note fnCallback和pUserData一起唯一确定一个回调函数
@note 若平台不需要支持任何可插拔网卡，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDIIPCFGRdiRemoveCallback(CSUDIRDICallback_F fnCallback, void* pvUserData);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

