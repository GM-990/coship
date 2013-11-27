/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_ipcfg.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_ipcfg.h"
 /**
@brief 注册无线网卡的回调函数

@param[in] fnEtherCallback 回调函数指针，详见CSUDIEtherCallback_F定义
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 回调函数句柄和用户数据唯一确定一个回调函数，当两次注册它们完全一样时，第二次将返回CSUDIIPCFG_ERROR_ALREADY_ADDED
@note 至少支持注册的回调函数个数为32个，最大不限
*/
CSUDI_Error_Code CSUDIIPCFGAddEtherCallback(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


 /**
@brief 删除无线网卡的回调函数

@param[in] fnEtherCallback 回调函数指针，详见CSUDIEtherCallback_F定义
@param[in] pvUserData 同CSUDIIPCFGAddEtherCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGDelEtherCallback(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获得网卡数，支持有线和无线网卡和pppoe。如果带有pppoe功能，数量应该是实际的网卡数加上ppp0
@param[out] pnCount 获取到的网卡数目
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/ 
CSUDI_Error_Code CSUDIIPCFGGetEtherCount(int* pnCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取网卡设备名，支持有线和无线网卡和pppoe。

@param[in] nEthIndex 网卡设备索引号，如 0，1 索引号
@param[in] nLen pcEthName所指空间的长度，必须大于0小于等于16
@param[out] pcEthName 获取到的网卡设备名，由用户申请空间，长度必须大于0小于等于16
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherName(int nEthIndex,char * pcEthName,int nLen )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获得网卡芯片的信息，包括制造商和类型等。

@param[in] pcEthName 网卡名，例如:eth0
@param[out] psChipInfo 见CSUDIEtherInfo_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherChipInfo(char * pcEthName, CSUDIEtherInfo_S * psChipInfo)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 通过设备名称来获取网卡物理地址，支持有线和无线网卡。

@param[in] pcEthName 网卡名，例如:eth0
@param[out] pucMacAddress 指向一个MAC地址的字符型数组, 格式为{0x00, 0x0E, 0xDB, 0x11, 0x22, 0x33}类
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 1.udi2没有制定设置mac地址的接口，因为设置mac地址不应该是udi2关心的事情，而应该是系统在启动时就执行的动作。
@note 换句话说，mac地址在调用该接口之前就已经确定。这里获取的mac地址，也必须与系统启动时设置进的保持一致。
*/
CSUDI_Error_Code CSUDIIPCFGGetMacByName(char * pcEthName,unsigned char * pucMacAddress)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


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
CSUDI_Error_Code CSUDIIPCFGGetEtherStatus(char * pcEthName, unsigned short * pushLinkState)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 搜索网络内存在的所有AP的信息，只用于无线网卡。

@param[in] pcEthName 无线网卡的名称
@param[in] psWlanApInfo 存储AP信息的结构体数组
@param[in] pnWlanApInfoNum psWlanApInfo结构体数组的个数
@param[out] psWlanApInfo 存储实际获取到的AP信息的结构体数组
@param[out] pnWlanApInfoNum 实际获取到的AP信息的有效个数 	
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGSearchAP(char * pcEthName,CSUDIWLanApInfo_S * psWlanApInfo, int * pnWlanApInfoNum)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 与AP建立连接并且进行认证，只用于无线网卡。

@param[in] psWLanConf 无线网卡的配置参数数据结构，包括无线网卡的名称，认证的ESSID字符串，认证的密钥的字符串和长度，认证的类型，密钥的数据类型
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGConnectAP(CSUDIWLanConfig_S * psWLanConf)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 断开与AP的连接，只用于无线网卡。

@param[in] pcEthName 无线网卡的名称
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGDisconnectAP(char * pcEthName)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取AP信号的质量和强度。

@param[out] pnWlanApSigQulity AP信号的质量
@param[out] pnWlanApSigStrength AP信号的强度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 此接口建议废弃掉，若需获取信息，应该在CSUDIIPCFGSearchAP 的时候保存信息
*/
CSUDI_Error_Code CSUDIIPCFGGetAPSignalQulityStrength(int * pnWlanApSigQulity,int * pnWlanApSigStrength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 设置DNS的地址。

@param[in] nDNSIndex DNS的索引号，如 0，1 ，最多支持4个
@param[in] pcDNS DNS的IP地址（如"192.168.99.5"的点分十进制IP地址)
@param[in] nLen DNS的IP地址字符串长度，不小于16 
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGSetDNSServer(int nDNSIndex,char * pcDNS,int nLen)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**  
@brief 获得DNS的地址。

@param[in] nDNSIndex DNS的索引号，如 0，1，最多支持4个
@param[in] nLen DNS的IP地址字符串长度，不小于16 
@param[out] pcDNS DNS的IP地址(如"192.168.99.5"的点分十进制IP地址），pcDNS的空间由用户分配
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGGetDNSServer(int nDNSIndex,char * pcDNS,int nLen)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置网卡的IP地址，子网掩码和网关地址，支持有线和无线网卡。

@param[in] pcEthName 网卡设备名，如:"eth0"，若为CSUDI_NULL，则返回错误
@param[in] pcIp IP地址字符串，如"192.168.1.1" ，若为CSUDI_NULL，则返回错误    
@param[in] pcMask 子网掩码字符串，如"255.255.255.0"，可以为CSUDI_NULL
@param[in] pcGateWay 网关地址字符串，如"192.168.1.254"，可以为CSUDI_NULL
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGSetIpInfo(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


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
CSUDI_Error_Code CSUDIIPCFGGetIpInfo(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 将应用层传下来的包经过处理交给网卡驱动层发送出去。

@param[in] pszEthName 网卡设备名称
@param[in] fd socket描述符。(*fd==-1的时候，在此函数内创建socket,否则与sosket无关)
@param[in] pcDhcpBuf  应用层数据/以太网包数据
@param[in] nBufLen buffer的长度
@param[in] nFlag 是否需要此函数添加udp/ip/ethernet头 nFlag =1,需要添加，nFlag =0,直接发送
@return 成功返回 0,失败返回 -1
*/
int  CSUDIRawSendPacket(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


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
int  CSUDIRawSendPacketByMode(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag, CSUDIRawSendType_E eRawSendMode)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief 接收从网卡驱动传上来的包，交给应用层

@param[in] pszEthName 网卡设备名称
@param[in] fd socket描述符。(*fd==-1的时候，在此函数内创建socket,否则与sosket无关)
@param[out] pchBuf  需要接收的数据
@param[in,out] pnBufLen buffer的长度.输入参数时为pchBuf的缓冲区长度，输出参数为pchBuf的有效数据长度
@param[in] nFlag  是否需要此函数去掉udp/ip/ethernet头  nFlag =1,需要去掉，nFlag =0,直接交给上一层
@return 成功返回 实际收到的字节数,失败返回 -1
*/
int CSUDIRawRecvPacket (char *pszEthName, int  *fd, const char *pchBuf, int* pnBufLen, int nFlag)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取网卡统计信息
@param[in] pcName 网卡设备名称
@param[out] psNetStatus 存储实际获取到的网卡统计信息的结构体
@return 成功返回CSUDI_SUCCESS 失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGGetStatInfo(char *pcName,CSUDIEtherStat_S *psNetStatus)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 启用网卡
@param[in] pcEthName 网卡设备名称
@param[out] 无
@return 成功返回CSUDI_SUCCESS 失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGNICUp(char *pcEthName)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 禁用网卡
@param[in] pcEthName 网卡设备名称
@param[out] 无
@return 成功返回CSUDI_SUCCESS 失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIIPCFGNICDown(char *pcEthName)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

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
CSUDI_Error_Code CSUDIIPCFGRdiAddCallback(CSUDIRDICallback_F fnCallback, void* pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 删除可插拔网卡通知回调函数

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“网卡”类型的可移动设备
@param[in] fnCallback 回调函数,之前注册的函数
@param[in] pvUserData 用户数据,必须与注册时的一致
@return 成功删除返回CSUDI_SUCCESS,失败返回错误代码值
@note fnCallback和pUserData一起唯一确定一个回调函数
@note 若平台不需要支持任何可插拔网卡，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDIIPCFGRdiRemoveCallback(CSUDIRDICallback_F fnCallback, void* pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

