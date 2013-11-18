#ifndef _UDI2_IPCFG_H_PROXY
#define _UDI2_IPCFG_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_ipcfg.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIIPCFGAddEtherCallback(format, ...) CSUDIIPCFGAddEtherCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGAddEtherCallback_PROXY(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGDelEtherCallback(format, ...) CSUDIIPCFGDelEtherCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGDelEtherCallback_PROXY(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetEtherCount(format, ...) CSUDIIPCFGGetEtherCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetEtherCount_PROXY(int* pnCount, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetEtherName(format, ...) CSUDIIPCFGGetEtherName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetEtherName_PROXY(int nEthIndex,char * pcEthName,int nLen , const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetEtherChipInfo(format, ...) CSUDIIPCFGGetEtherChipInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetEtherChipInfo_PROXY(char * pcEthName, CSUDIEtherInfo_S * psChipInfo, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetMacByName(format, ...) CSUDIIPCFGGetMacByName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetMacByName_PROXY(char * pcEthName,unsigned char * pucMacAddress, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetEtherStatus(format, ...) CSUDIIPCFGGetEtherStatus_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetEtherStatus_PROXY(char * pcEthName, unsigned short * pushLinkState, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGSearchAP(format, ...) CSUDIIPCFGSearchAP_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGSearchAP_PROXY(char * pcEthName,CSUDIWLanApInfo_S * psWlanApInfo, int * pnWlanApInfoNum, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGConnectAP(format, ...) CSUDIIPCFGConnectAP_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGConnectAP_PROXY(CSUDIWLanConfig_S * psWLanConf, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGDisconnectAP(format, ...) CSUDIIPCFGDisconnectAP_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGDisconnectAP_PROXY(char * pcEthName, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetAPSignalQulityStrength(format, ...) CSUDIIPCFGGetAPSignalQulityStrength_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetAPSignalQulityStrength_PROXY(int * pnWlanApSigQulity,int * pnWlanApSigStrength, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGSetDNSServer(format, ...) CSUDIIPCFGSetDNSServer_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGSetDNSServer_PROXY(int nDNSIndex,char * pcDNS,int nLen, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetDNSServer(format, ...) CSUDIIPCFGGetDNSServer_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetDNSServer_PROXY(int nDNSIndex,char * pcDNS,int nLen, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGSetIpInfo(format, ...) CSUDIIPCFGSetIpInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGSetIpInfo_PROXY(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetIpInfo(format, ...) CSUDIIPCFGGetIpInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetIpInfo_PROXY(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay, const char *pcFuncName, const int nLine);

#define CSUDIRawSendPacket(format, ...) CSUDIRawSendPacket_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int  CSUDIRawSendPacket_PROXY(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag, const char *pcFuncName, const int nLine);

#define CSUDIRawSendPacketByMode(format, ...) CSUDIRawSendPacketByMode_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int  CSUDIRawSendPacketByMode_PROXY(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag, CSUDIRawSendType_E eRawSendMode, const char *pcFuncName, const int nLine);

#define CSUDIRawRecvPacket(format, ...) CSUDIRawRecvPacket_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIRawRecvPacket_PROXY(char *pszEthName, int  *fd, const char *pchBuf, int* pnBufLen, int nFlag, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGGetStatInfo(format, ...) CSUDIIPCFGGetStatInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGGetStatInfo_PROXY(char *pcName,CSUDIEtherStat_S *psNetStatus, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGNICUp(format, ...) CSUDIIPCFGNICUp_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGNICUp_PROXY(char *pcEthName, const char *pcFuncName, const int nLine);

#define CSUDIIPCFGNICDown(format, ...) CSUDIIPCFGNICDown_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIIPCFGNICDown_PROXY(char *pcEthName, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
