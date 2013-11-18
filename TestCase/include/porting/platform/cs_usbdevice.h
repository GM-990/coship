#ifndef _CS_USB_DEVICE_H
#define _CS_USB_DEVICE_H

#include "cs_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 设备信息结构*/
typedef struct 
{
    	 int  m_nIsUsed;        ///<是否在用
    	 char m_szUSBType[128];     	 ///<设备类型
	 char m_szUSBDescription[256]; ///< 设备
}CSUSBInfo_S;	



/**
@brief 获取当前插入的USB 设备个数
@param[out] pdwUsbDeviceCount 当前插入的USB 设备个数

@return 成功获取则返回TRUE；失败则返回FALSE
*/
BOOL CSUSBGetCount(unsigned int * pdwUsbDeviceCount);

/**
@brief 根据设备索引获取USB 设备信息

@param[in] uIndex USB 设备索引ID ，取值范围在(0~31)
@param[out] psUsbInfo 指向返回USB 设备的信息
@return 成功获取返回TRUE，失败则返回FALSE
*/
BOOL CSUSBGetInfo(unsigned int uIndex,CSUSBInfo_S * psUsbInfo);


#ifdef __cplusplus
}
#endif

/** @} */

#endif
