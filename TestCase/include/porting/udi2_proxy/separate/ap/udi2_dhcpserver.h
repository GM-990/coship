#ifndef _UDI2_DHCPSERVER_H_PROXY
#define _UDI2_DHCPSERVER_H_PROXY

#include "udi2_error.h"
#include "../udi2/separate/ap/udi2_dhcpserver.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIDHCPSStart(format, ...) CSUDIDHCPSStart_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDHCPSStart_PROXY(const char* szIfName, const char *pcFuncName, const int nLine);

#define CSUDIDHCPSStop(format, ...) CSUDIDHCPSStop_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDHCPSStop_PROXY(const char* szIfName, const char *pcFuncName, const int nLine);

#define CSUDIDHCPSConfig(format, ...) CSUDIDHCPSConfig_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDHCPSConfig_PROXY(const char* szIfName,const CSUDIDHCPSConfig_S *psDHCPCfg, const char *pcFuncName, const int nLine);

#define CSUDIDHCPSGetConfig(format, ...) CSUDIDHCPSGetConfig_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDHCPSGetConfig_PROXY(const char* szIfName,CSUDIDHCPSConfig_S *psOutDHCPCfg, const char *pcFuncName, const int nLine);

#define CSUDIDHCPSAddStaticIPCfg(format, ...) CSUDIDHCPSAddStaticIPCfg_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIDHCPSAddStaticIPCfg_PROXY(const char* szIfName,const CSUDIDHCPSStaticIpCfg *psItem,int nCount, const char *pcFuncName, const int nLine);

#define CSUDIDHCPSDelStaticIPCfg(format, ...) CSUDIDHCPSDelStaticIPCfg_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIDHCPSDelStaticIPCfg_PROXY(const char* szIfName,const CSUDIDHCPSStaticIpCfg *pstItem,int nCount, const char *pcFuncName, const int nLine);

#define CSUDIDHCPSGetStaticIPCfgs(format, ...) CSUDIDHCPSGetStaticIPCfgs_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIDHCPSGetStaticIPCfgs_PROXY(const char* szIfName,CSUDIDHCPSStaticIpCfg *psItem,int *pnStaticCount, const char *pcFuncName, const int nLine);

#define CSUDIDHCPSGetLeaseInfo(format, ...) CSUDIDHCPSGetLeaseInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIDHCPSGetLeaseInfo_PROXY(const char* szIfName,CSUDIDHCPSLeaseInfo *pstLeaseInfo,int *pnCount, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
