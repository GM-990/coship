#ifndef _UDI2_BLUETOOTH_H_PROXY
#define _UDI2_BLUETOOTH_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_tuner.h"
#include "../udi2/udi2_bluetooth.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIBTAddListener(format, ...) CSUDIBTAddListener_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTAddListener_PROXY(CSUDIBTCallback_F fnCb,void* pUserData, const char *pcFuncName, const int nLine);

#define CSUDIBTGetAllAdapter(format, ...) CSUDIBTGetAllAdapter_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTGetAllAdapter_PROXY(CSUDI_HANDLE *ahOutHandleArray,unsigned int dwArraySize,unsigned int* pdwOutCount, const char *pcFuncName, const int nLine);

#define CSUDIBTGetAdapterInfo(format, ...) CSUDIBTGetAdapterInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTGetAdapterInfo_PROXY(CSUDI_HANDLE hAdapter,CSUDIBTAdapterInfo_S* psOutInfo, const char *pcFuncName, const int nLine);

#define CSUDIBTSetAdapterName(format, ...) CSUDIBTSetAdapterName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTSetAdapterName_PROXY(CSUDI_HANDLE hAdapter, const char *szName, const char *pcFuncName, const int nLine);

#define CSUDIBTGetAdapterStatus(format, ...) CSUDIBTGetAdapterStatus_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTGetAdapterStatus_PROXY(CSUDI_HANDLE hAdapter,CSUDIBTAdapterStatus_E* peOutStatus, const char *pcFuncName, const int nLine);

#define CSUDIBTTurnOn(format, ...) CSUDIBTTurnOn_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTTurnOn_PROXY(CSUDI_HANDLE hAdapter, const char *pcFuncName, const int nLine);

#define CSUDIBTTurnOff(format, ...) CSUDIBTTurnOff_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTTurnOff_PROXY(CSUDI_HANDLE hAdapter, const char *pcFuncName, const int nLine);

#define CSUDIBTGetDevices(format, ...) CSUDIBTGetDevices_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTGetDevices_PROXY(CSUDI_HANDLE hAdapter,CSUDIBTDeviceInfo_S* psOutDevicesArray,unsigned int dwArraySize,unsigned int* pdwOutCount, const char *pcFuncName, const int nLine);

#define CSUDIBTDeleteDevice(format, ...) CSUDIBTDeleteDevice_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTDeleteDevice_PROXY(CSUDI_HANDLE hAdapter, const char* szDeviceAddress, const char *pcFuncName, const int nLine);

#define CSUDIBTStartScan(format, ...) CSUDIBTStartScan_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTStartScan_PROXY(CSUDI_HANDLE hAdapter, const char *pcFuncName, const int nLine);

#define CSUDIBTStopScan(format, ...) CSUDIBTStopScan_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTStopScan_PROXY(CSUDI_HANDLE hAdapter, const char *pcFuncName, const int nLine);

#define CSUDIBTStartPair(format, ...) CSUDIBTStartPair_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTStartPair_PROXY(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,const char* szPassword, const char *pcFuncName, const int nLine);

#define CSUDIBTStartConnect(format, ...) CSUDIBTStartConnect_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTStartConnect_PROXY(CSUDI_HANDLE hAdapter,const char* szDeviceAddress, const char *pcFuncName, const int nLine);

#define CSUDIBTGetDeviceStatus(format, ...) CSUDIBTGetDeviceStatus_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTGetDeviceStatus_PROXY(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,CSUDIBTDeviceStatus_E* peOutStatus, const char *pcFuncName, const int nLine);

#define CSUDIBTSdpRegister(format, ...) CSUDIBTSdpRegister_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBTSdpRegister_PROXY(const char* pcSerialID, unsigned int dwPort, const char* pcName, const char* pcDsc, const char* pcProv, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
