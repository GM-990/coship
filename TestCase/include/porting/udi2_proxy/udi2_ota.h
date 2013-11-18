#ifndef _UDI2_OTA_H_PROXY
#define _UDI2_OTA_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_ota.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIOTAGetInfo(format, ...) CSUDIOTAGetInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOTAGetInfo_PROXY(CSUDIOTAInfoType_E eType, void* value, int length, const char *pcFuncName, const int nLine);

#define CSUDIOTASetInfo(format, ...) CSUDIOTASetInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOTASetInfo_PROXY(CSUDIOTAInfoType_E eType, void* value, int length, const char *pcFuncName, const int nLine);

#define CSUDIOTASetOta(format, ...) CSUDIOTASetOta_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOTASetOta_PROXY(const char* pUrl, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
