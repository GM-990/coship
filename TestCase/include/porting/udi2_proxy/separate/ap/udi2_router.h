#ifndef _UDI2_ROUTER_H_PROXY
#define _UDI2_ROUTER_H_PROXY

#include "udi2_error.h"
#include "../udi2/separate/ap/udi2_router.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIRTSetInterfaces(format, ...) CSUDIRTSetInterfaces_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRTSetInterfaces_PROXY(const char* szWan,const char* szLan, const char *pcFuncName, const int nLine);

#define CSUDIRTGetInterfaces(format, ...) CSUDIRTGetInterfaces_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRTGetInterfaces_PROXY(char* pcWan,int nWanLen,char* pcLan,int nLanLen, const char *pcFuncName, const int nLine);

#define CSUDIRTSetLanIp(format, ...) CSUDIRTSetLanIp_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRTSetLanIp_PROXY(const char* szLan,const char* szIp,const char* szMask, const char *pcFuncName, const int nLine);

#define CSUDIRTStartService(format, ...) CSUDIRTStartService_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRTStartService_PROXY(CSUDIRTServiceType_E eService,void* pvStartParam, const char *pcFuncName, const int nLine);

#define CSUDIRTStopService(format, ...) CSUDIRTStopService_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRTStopService_PROXY(CSUDIRTServiceType_E eService, const char *pcFuncName, const int nLine);

#define CSUDIRTGetServiceStatus(format, ...) CSUDIRTGetServiceStatus_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRTGetServiceStatus_PROXY(CSUDIRTServiceType_E eService,CSUDI_BOOL *pbIsRunning, const char *pcFuncName, const int nLine);

#define CSUDIRTExecuteIptableCmd(format, ...) CSUDIRTExecuteIptableCmd_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIRTExecuteIptableCmd_PROXY(const char *pszcmd, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
