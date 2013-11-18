#ifndef _UDI2_TUNER_H_PROXY
#define _UDI2_TUNER_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"
#include "../udi2/udi2_tuner.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDITunerConnect(format, ...) CSUDITunerConnect_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerConnect_PROXY(CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punDeliver, const char *pcFuncName, const int nLine);

#define CSUDITunerAddCallback(format, ...) CSUDITunerAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerAddCallback_PROXY(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData , const char *pcFuncName, const int nLine);

#define CSUDITunerDelCallback(format, ...) CSUDITunerDelCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerDelCallback_PROXY(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData, const char *pcFuncName, const int nLine);

#define CSUDITunerGetSignalInfo(format, ...) CSUDITunerGetSignalInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerGetSignalInfo_PROXY(CSUDI_UINT32 dwTunerId, CSUDI_UINT32 u32SignalInfoMask, CSUDITunerSignalInfo_S *psSignalInfo, const char *pcFuncName, const int nLine);

#define CSUDITunerGetDeviceInfo(format, ...) CSUDITunerGetDeviceInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerGetDeviceInfo_PROXY(CSUDI_UINT32 dwTunerId, CSUDITunerInfo_S *psTunerInfo, const char *pcFuncName, const int nLine);

#define CSUDITunerGetAllDeviceId(format, ...) CSUDITunerGetAllDeviceId_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerGetAllDeviceId_PROXY(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt, const char *pcFuncName, const int nLine);

#define CSUDITunerAddPID(format, ...) CSUDITunerAddPID_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerAddPID_PROXY(CSUDI_UINT32 dwTunerId ,int nPID, const char *pcFuncName, const int nLine);

#define CSUDITunerRemovePID(format, ...) CSUDITunerRemovePID_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerRemovePID_PROXY(CSUDI_UINT32 dwTunerId ,int nPID, const char *pcFuncName, const int nLine);

#define CSUDITunerRDIAddCallback(format, ...) CSUDITunerRDIAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerRDIAddCallback_PROXY(CSUDIRDICallback_F fnCallback,const void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDITunerRDIRemoveCallback(format, ...) CSUDITunerRDIRemoveCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerRDIRemoveCallback_PROXY(CSUDIRDICallback_F fnCallback,const void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDITunerRDIRemoveDev(format, ...) CSUDITunerRDIRemoveDev_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerRDIRemoveDev_PROXY(CSUDI_UINT32 dwTunerId, const char *pcFuncName, const int nLine);

#define CSUDITunerIORead(format, ...) CSUDITunerIORead_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerIORead_PROXY(CSUDI_UINT32 dwTunerId,void* pvData,int nBuffLength,int * pnReadLength, const char *pcFuncName, const int nLine);

#define CSUDITunerT2GetPlpInfo(format, ...) CSUDITunerT2GetPlpInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDITunerT2GetPlpInfo_PROXY(CSUDI_UINT32 dwTunerId, CSUDI_UINT8* pbPlpIds, int nPlpIdsCount, int* pnRetIdCount, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
