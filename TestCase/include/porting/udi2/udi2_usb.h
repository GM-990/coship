/**@defgroup USB USB  定义了USB设置信息相关接口
@brief usb模块对应头文件udi2_usb.h，定义了USB设备信息相关接口

@version 2.0.2 2011/12/21 增加了错误代码枚举定义
@version 2.0.1 2011/03/08 初稿

@{
*/

#ifndef _UDI2_USB_H_
#define _UDI2_USB_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 返回错误代码枚举定义*/
enum {
	CSUDIUSB_ERROR_BAD_PARAMETER = CSUDI_USB_ERROR_BASE,		///< 参数异常错误 
	CSUDIUSB_ERROR_FEATURE_NOT_SUPPORTED,						///< 不支持此操作 
	CSUDIUSB_ERROR_INVALID_DEVICE_ID,							///< 非法设备ID
	CSUDIUSB_ERROR_UNKNOWN_ERROR								///< 未知错误
};

/**@brief 设备信息结构*/
typedef struct
{
	CSUDI_BOOL   m_bIsUsed;        	///<是否在用
	char m_szUSBType[128];     	 	///<设备类型
	char m_szUSBDescription[256]; 	///<设备描述
}CSUDIUSBInfo_S;	

/**
@brief 获取当前插入的USB 设备个数
@param[out] pdwUsbDeviceCount 当前插入的USB 设备个数
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIUSBGetCount(unsigned int* pdwUsbDeviceCount);

/**
@brief 根据设备索引获取USB 设备信息

@param[in] uIndex USB 设备索引ID，从0开始最大值为CSUDIUSBGetCount获取到的count-1
@param[out] psUsbInfo 指向返回USB设备的信息
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIUSBGetInfo(unsigned int uIndex, CSUDIUSBInfo_S * psUsbInfo);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

