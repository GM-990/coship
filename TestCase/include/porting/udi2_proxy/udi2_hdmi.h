#ifndef _UDI2_HDMI_H_PROXY
#define _UDI2_HDMI_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_screen.h"
#include "../udi2/udi2_hdmi.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIHDMIGetCount(format, ...) CSUDIHDMIGetCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIHDMIGetCount_PROXY(int * pnHdmiDeviceCount, const char *pcFuncName, const int nLine);

#define CSUDIHDMIGetSinkCapabilities(format, ...) CSUDIHDMIGetSinkCapabilities_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIHDMIGetSinkCapabilities_PROXY(int nHdmiIndex, CSUDIHDMISinkCapabilities_S *pCaps, const char *pcFuncName, const int nLine);

#define CSUDIHDMISetProperty(format, ...) CSUDIHDMISetProperty_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIHDMISetProperty_PROXY(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData, const char *pcFuncName, const int nLine);

#define CSUDIHDMIAddCallback(format, ...) CSUDIHDMIAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIHDMIAddCallback_PROXY(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback,void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDIHDMIDelCallback(format, ...) CSUDIHDMIDelCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIHDMIDelCallback_PROXY(int nHdmiIndex, CSUDIHDMICallback_F fnHdmiCallback, void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDIHDMIGetProperty(format, ...) CSUDIHDMIGetProperty_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIHDMIGetProperty_PROXY(int nHdmiIndex, CSUDIHDMIPropertyType_E ePropetyType, void* pvData, int* pnLen, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
