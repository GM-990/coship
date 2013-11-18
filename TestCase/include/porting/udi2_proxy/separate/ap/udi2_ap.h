#ifndef _UDI2_AP_H_PROXY
#define _UDI2_AP_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/separate/ap/udi2_ap.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIAPSetSignal(format, ...) CSUDIAPSetSignal_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPSetSignal_PROXY(const char* szApName,CSUDI_BOOL bEnable, const char *pcFuncName, const int nLine);

#define CSUDIAPSetFreqChannel(format, ...) CSUDIAPSetFreqChannel_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPSetFreqChannel_PROXY(const char* szApName,CSUDIAPFreqChannel_E eChannel, const char *pcFuncName, const int nLine);

#define CSUDIAPSetSSIDSeparate(format, ...) CSUDIAPSetSSIDSeparate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPSetSSIDSeparate_PROXY(const char* szApName,CSUDI_BOOL bEnable, const char *pcFuncName, const int nLine);

#define CSUDIAPGetConfig(format, ...) CSUDIAPGetConfig_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPGetConfig_PROXY(const char* szApName,CSUDIAPConfig_S* pstOutConfig, const char *pcFuncName, const int nLine);

#define CSUDIAPGetSSIDCapbility(format, ...) CSUDIAPGetSSIDCapbility_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPGetSSIDCapbility_PROXY(const char* szApName,int *pnMaxCount, const char *pcFuncName, const int nLine);

#define CSUDIAPGetSSIDs(format, ...) CSUDIAPGetSSIDs_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPGetSSIDs_PROXY(const char* szApName,CSUDIAPSSID_S* psOutSsids,int* pnCount, const char *pcFuncName, const int nLine);

#define CSUDIAPSetSSIDs(format, ...) CSUDIAPSetSSIDs_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPSetSSIDs_PROXY(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount, const char *pcFuncName, const int nLine);

#define CSUDIAPGetStationsInfo(format, ...) CSUDIAPGetStationsInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPGetStationsInfo_PROXY(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount, const char *pcFuncName, const int nLine);

#define CSUDIAPSetAdvancedInfo(format, ...) CSUDIAPSetAdvancedInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPSetAdvancedInfo_PROXY(const char* szApName,const CSUDIAPAdvancedInfo_S *psInfo, const char *pcFuncName, const int nLine);

#define CSUDIAPGetAdvancedInfo(format, ...) CSUDIAPGetAdvancedInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIAPGetAdvancedInfo_PROXY(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
