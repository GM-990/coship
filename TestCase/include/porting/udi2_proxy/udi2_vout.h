#ifndef _UDI2_VOUT_H_PROXY
#define _UDI2_VOUT_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "../udi2/udi2_vout.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIVOUTEnable(format, ...) CSUDIVOUTEnable_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVOUTEnable_PROXY(CSUDIVOUTDevices_E eVoutDevice, const char *pcFuncName, const int nLine);

#define CSUDIVOUTDisable(format, ...) CSUDIVOUTDisable_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVOUTDisable_PROXY(CSUDIVOUTDevices_E eVoutDevice, const char *pcFuncName, const int nLine);

#define CSUDIVOUTSetOutputType(format, ...) CSUDIVOUTSetOutputType_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVOUTSetOutputType_PROXY(CSUDIVOUTDevices_E eVoutDevice, CSUDIVOUTOutputType_E eType, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
