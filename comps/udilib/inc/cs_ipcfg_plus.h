/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

#ifndef _CS_IPCFG_PLUS_H_
#define _CS_IPCFG_PLUS_H_

#include "udi2_ipcfg.h"
#ifdef __cplusplus
extern "C" {
#endif

/**@brief 网卡信息*/
typedef struct 
{
    char szEthName[CSUDI_ETHER_NETIFNAME_MAX_LEN];		///< 网卡的名称
    CSUDIEtherType_E eEthType;  //网卡类型：包括无线网卡、有线网卡、pppoe等
}CSUDIEthInfo_S;

/***************************************************************************
* 函数: CSIPCFG_TCPSetup
* 功能: 初始化并启动协议栈
* 输入:  
*       pBuffer :  为协议栈分配的空间，由用户申请空间。
*       nSize :为协议栈分配的空间大小，不小于0x10000（64K）字节。     
*	  nInputThreadPrio: 协议栈input线程优先级，必须大于0
*       nInputThreadStackSize : 协议栈input线程堆栈大小，不小于32*1024字节
*       nTimerThreadPrio: 协议栈timer线程优先级，必须大于0
*       nInputThreadStackSize : 协议栈timer线程堆栈大小，不小于32*1024字节
*	  usConfig :	当usConfig =CS_CONFIG_DHCPC时表示协议栈需要加载DHCP协议。
*                   当usConfig = CS_CONFIG_PPPOE时表示协议栈需要加载PPPOE协议
*
*                   
* 输出: 
*     
* 返回值: 
*   成功：0
*   失败：非0，如果是-1通常是参数错误
********************************************************************************/
int CSIPCFG_TCPSetup (
		char*          pchBuffer,
		unsigned int   unSize,
		int            nInputThreadPrio,
		int      stInputThreadStackSize,
		int            nTimerThreadPrio,
		int      stTimerThreadStackSize,
		unsigned short usConfig );

/**
@brief 添加网卡

@param[in] pcEthName 网卡设备名称
@param[in] eType 网卡类型
@return 成功返回 CSUDI_SUCCESS,失败返回 CSUDI_FAILURE
@备注:用户如果有pppoe，必须要添加
*/
CSUDI_Error_Code  CSIPCFGGAddEth(char * pcEthName,int nLen, CSUDIEtherType_E eType);

/**
@brief 删除网卡,主要用于可插拔网络设备
@param[in] pcEthName 需要删除的网络设备名称
@return 成功返回 CSUDI_SUCCESS,失败返回 CSUDI_FAILURE
*/
CSUDI_Error_Code  CSIPCFGGRemoveEth(char * pcEthName);

#ifdef __cplusplus
}
#endif
#endif /* __CSIPCFGPLUS_H__ */


