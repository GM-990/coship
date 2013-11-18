#ifndef _UDI2_USB_H_PROXY
#define _UDI2_USB_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"
#include "../udi2/udi2_usb.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIUSBGetCount(format, ...) CSUDIUSBGetCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIUSBGetCount_PROXY(unsigned int* pdwUsbDeviceCount, const char *pcFuncName, const int nLine);

#define CSUDIUSBGetInfo(format, ...) CSUDIUSBGetInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIUSBGetInfo_PROXY(unsigned int uIndex, CSUDIUSBInfo_S * psUsbInfo, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
