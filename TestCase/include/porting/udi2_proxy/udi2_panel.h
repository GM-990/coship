#ifndef _UDI2_PANEL_H_PROXY
#define _UDI2_PANEL_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_panel.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIPanelGetCapabilityByName(format, ...) CSUDIPanelGetCapabilityByName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPanelGetCapabilityByName_PROXY(int nPanelIndex,CSUDIPanelLightName_E eName,CSUDIPanelLightCapability_S * psLightCapablity, const char *pcFuncName, const int nLine);

#define CSUDIPanelDisplay(format, ...) CSUDIPanelDisplay_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPanelDisplay_PROXY(int nPanelIndex,const CSUDIPanelLightValule_S * psValue, const char *pcFuncName, const int nLine);

#define CSUDIPanelClear(format, ...) CSUDIPanelClear_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPanelClear_PROXY(int nPanelIndex,CSUDIPanelLightName_E eName, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
