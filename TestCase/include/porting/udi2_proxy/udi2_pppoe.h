#ifndef _UDI2_PPPOE_H_PROXY
#define _UDI2_PPPOE_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_pppoe.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIPPPOEInit(format, ...) CSUDIPPPOEInit_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOEInit_PROXY(CSUDIPPPOEInitPara_S *psInitPara, const char *pcFuncName, const int nLine);

#define CSUDIPPPOECleanup(format, ...) CSUDIPPPOECleanup_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOECleanup_PROXY(int nFlag, const char *pcFuncName, const int nLine);

#define CSUDIPPPOEConnect(format, ...) CSUDIPPPOEConnect_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOEConnect_PROXY(const char *pcUserName, const char *pcPasswd, char *pcServiceName, char *pcEthName, int nTimeout, const char *pcFuncName, const int nLine);

#define CSUDIPPPOEDisconnect(format, ...) CSUDIPPPOEDisconnect_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOEDisconnect_PROXY(int nTimeout , const char *pcFuncName, const int nLine);

#define CSUDIPPPOEGetServiceName(format, ...) CSUDIPPPOEGetServiceName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOEGetServiceName_PROXY(char *pcSevicename, const char *pcFuncName, const int nLine);

#define CSUDIPPPOEGetState(format, ...) CSUDIPPPOEGetState_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOEGetState_PROXY(int *pnState, const char *pcFuncName, const int nLine);

#define CSUDIPPPOEGetACMacAddr(format, ...) CSUDIPPPOEGetACMacAddr_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOEGetACMacAddr_PROXY(char *pcMAC, const char *pcFuncName, const int nLine);

#define CSUDIPPPOEGetSessionID(format, ...) CSUDIPPPOEGetSessionID_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOEGetSessionID_PROXY(unsigned short *pusSessionID, const char *pcFuncName, const int nLine);

#define CSUDIPPPOEGetVersion(format, ...) CSUDIPPPOEGetVersion_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIPPPOEGetVersion_PROXY(char *pcVer, const char *pcFuncName, const int nLine);

#define CSUDIPPPOEAddCallback(format, ...) CSUDIPPPOEAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPPPOEAddCallback_PROXY(CSUDIPPPOECallback_F fnCallback,void *pvUserData, const char *pcFuncName, const int nLine);

#define CSUDIPPPOERemoveCallback(format, ...) CSUDIPPPOERemoveCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPPPOERemoveCallback_PROXY(CSUDIPPPOECallback_F fnCallback,void *pvUserData, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
