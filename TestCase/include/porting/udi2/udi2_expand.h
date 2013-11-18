/**@defgroup 该模块主要定义了与机顶盒芯片相连了外围扩展设备
例如: 1、硬AP 的wifi 设备
             2、cable modem
             3、EOC
@{
*/

#ifndef _UDI2_EXPAND_H_
#define _UDI2_EXPAND_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_ipcfg.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
@brief 获取扩展网络设备的连接状态，例如: 硬AP，cable modem，EOC等

@param[in] pcEthName 与扩展网络设备相连的网卡名字
@param[out] pushLinkState 连接状态
@return 成功返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
@note 无线网卡的连接状态:
@note 1、pushLinkState & CSUDI_ETHMASK_UP_DOWN : 为0代表已经连接，为1代表未连接。
@note 2、集成厂商可以自行决定获取连接状态的方法，例如: SNMP 查询协议
*/
CSUDI_Error_Code CSUDIExpandGetStatus(char * pcEthName, unsigned short * pushLinkState);

#ifdef	__cplusplus
}
#endif

/**@}*/

#endif 


