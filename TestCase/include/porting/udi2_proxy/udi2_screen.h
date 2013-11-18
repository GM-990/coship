#ifndef _UDI2_SCREEN_H_PROXY
#define _UDI2_SCREEN_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "../udi2/udi2_screen.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDISCREENGetCapability(format, ...) CSUDISCREENGetCapability_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISCREENGetCapability_PROXY(CSUDISCREENType_E eScreenDevice,CSUDISCREENCapability_S * psCapabilityInfo, const char *pcFuncName, const int nLine);

#define CSUDISCREENAddCallback(format, ...) CSUDISCREENAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISCREENAddCallback_PROXY(CSUDISCREENCallback_F fnScreenCallback,void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDISCREENDelCallback(format, ...) CSUDISCREENDelCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISCREENDelCallback_PROXY(CSUDISCREENCallback_F fnScreenCallback, void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDISCREENSetResolution(format, ...) CSUDISCREENSetResolution_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISCREENSetResolution_PROXY(CSUDISCREENType_E* peScreenDevice, CSUDISCREENResolution_E* peResolution,int nCount, const char *pcFuncName, const int nLine);

#define CSUDISCREENGetResolution(format, ...) CSUDISCREENGetResolution_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISCREENGetResolution_PROXY(CSUDISCREENType_E eScreenDevice,CSUDISCREENResolution_E * peResolution, const char *pcFuncName, const int nLine);

#define CSUDISCREENSetOSDTransparency(format, ...) CSUDISCREENSetOSDTransparency_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDISCREENSetOSDTransparency_PROXY(int nTransparency, const char *pcFuncName, const int nLine);

#define CSUDISCREENSetBrightness(format, ...) CSUDISCREENSetBrightness_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISCREENSetBrightness_PROXY(int nBrightness, const char *pcFuncName, const int nLine);

#define CSUDISCREENSetContrast(format, ...) CSUDISCREENSetContrast_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISCREENSetContrast_PROXY(int nContrast, const char *pcFuncName, const int nLine);

#define CSUDISCREENSetSaturation(format, ...) CSUDISCREENSetSaturation_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISCREENSetSaturation_PROXY(int nSaturation, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
