#ifndef _UDI2_AUDIO_H_PROXY
#define _UDI2_AUDIO_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_aout.h"
#include "udi2_error.h"
#include "../udi2/udi2_audio.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIAUDIOGetCount(format, ...) CSUDIAUDIOGetCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAUDIOGetCount_PROXY(int * pnAudioDecCount, const char *pcFuncName, const int nLine);

#define CSUDIAUDIOGetCapability(format, ...) CSUDIAUDIOGetCapability_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAUDIOGetCapability_PROXY(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo, const char *pcFuncName, const int nLine);

#define CSUDIAUDIOGetStatus(format, ...) CSUDIAUDIOGetStatus_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAUDIOGetStatus_PROXY(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus, const char *pcFuncName, const int nLine);

#define CSUDIAUDIOSetChannel(format, ...) CSUDIAUDIOSetChannel_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAUDIOSetChannel_PROXY(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType, const char *pcFuncName, const int nLine);

#define CSUDIAUDIOMute(format, ...) CSUDIAUDIOMute_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAUDIOMute_PROXY(int nAudioIndex, const char *pcFuncName, const int nLine);

#define CSUDIAUDIOUnmute(format, ...) CSUDIAUDIOUnmute_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAUDIOUnmute_PROXY(int nAudioIndex, const char *pcFuncName, const int nLine);

#define CSUDIAUDIOSetVolume(format, ...) CSUDIAUDIOSetVolume_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAUDIOSetVolume_PROXY(int nAudioIndex,int nVolume, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
