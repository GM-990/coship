#ifndef _UDI2_AOUT_H_PROXY
#define _UDI2_AOUT_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "../udi2/udi2_aout.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIAOUTMute(format, ...) CSUDIAOUTMute_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAOUTMute_PROXY(CSUDIAOUTDevices_E eAudioDevice, const char *pcFuncName, const int nLine);

#define CSUDIAOUTUnmute(format, ...) CSUDIAOUTUnmute_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAOUTUnmute_PROXY(CSUDIAOUTDevices_E eAudioDevice, const char *pcFuncName, const int nLine);

#define CSUDIAOUTSetOPMode(format, ...) CSUDIAOUTSetOPMode_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAOUTSetOPMode_PROXY(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
