#ifndef _UDI2_TOOLSET_H_PROXY
#define _UDI2_TOOLSET_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "../udi2/udi2_toolset.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIGetChipID(format, ...) CSUDIGetChipID_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIGetChipID_PROXY(CSUDICHIPID_S * psChipID, const char *pcFuncName, const int nLine);

#define CSUDIGetSerialKey(format, ...) CSUDIGetSerialKey_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIGetSerialKey_PROXY(CSUDISerialKey_S * psSerialKey, const char *pcFuncName, const int nLine);

#define CSUDISYSReboot() CSUDISYSReboot_PROXY(__FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISYSReboot_PROXY(const char *pcFuncName, const int nLine);

#define CSUDISYSHalt() CSUDISYSHalt_PROXY(__FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISYSHalt_PROXY(const char *pcFuncName, const int nLine);

#define CSUDISYSStandby(format, ...) CSUDISYSStandby_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISYSStandby_PROXY(CSUDI_BOOL bStandby,int nStandbyTime,CSUDI_BOOL *pbReal, const char *pcFuncName, const int nLine);

#define CSUDIToolsetExec(format, ...) CSUDIToolsetExec_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
void CSUDIToolsetExec_PROXY(CSUDIToolsetExecInfo_S *psExecInfo, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
