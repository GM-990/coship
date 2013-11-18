#ifndef _UDI2_EW200SERVICE_H_PROXY
#define _UDI2_EW200SERVICE_H_PROXY

#include "udi2_typedef.h"
#include "udi2_tuner.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "../udi2/udi2_EW200Service.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIEW200SReadClientInfo(format, ...) CSUDIEW200SReadClientInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIEW200SReadClientInfo_PROXY(CSUDIEW200SClientInfoType_E eType, void* value, int length, const char *pcFuncName, const int nLine);

#define CSUDIEW200SWriteClientInfo(format, ...) CSUDIEW200SWriteClientInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIEW200SWriteClientInfo_PROXY(CSUDIEW200SClientInfoType_E eType, void* value, int length, const char *pcFuncName, const int nLine);

#define CSUDIEW200SGetTuneParam(format, ...) CSUDIEW200SGetTuneParam_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEW200SGetTuneParam_PROXY(int nOrNetId,int nTsId,int nServiceId,CSUDIEW200SSourceParam_S * psSourceParam, const char *pcFuncName, const int nLine);

#define CSUDIEW200SGetIPQamDomainInfo(format, ...) CSUDIEW200SGetIPQamDomainInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_BOOL CSUDIEW200SGetIPQamDomainInfo_PROXY(const CSUDITunerSRCDeliver_U* punDeliverList,
										int nDeliverCount,
										int nPid,
										unsigned char ucTableId,
										char* pIpQamDataBuffer,
										int nBufferLength,
										void* pvReserved
									   , const char *pcFuncName, const int nLine);

#define CSUDIEW200SPlayStream(format, ...) CSUDIEW200SPlayStream_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
void CSUDIEW200SPlayStream_PROXY(char* pcURL, int nPlayId, const char *pcFuncName, const int nLine);

#define CSUDIEW200SStopPlay(format, ...) CSUDIEW200SStopPlay_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
void CSUDIEW200SStopPlay_PROXY(int nPlayId, const char *pcFuncName, const int nLine);

#define CSUDIEW200SSetPlayRect(format, ...) CSUDIEW200SSetPlayRect_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
void CSUDIEW200SSetPlayRect_PROXY(CSUDIWinRect_S* psRect,int nPlayId, const char *pcFuncName, const int nLine);

#define CSUDIEW200SResume(format, ...) CSUDIEW200SResume_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEW200SResume_PROXY(int nPlayId, const char *pcFuncName, const int nLine);

#define CSUDIEW200SPause(format, ...) CSUDIEW200SPause_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEW200SPause_PROXY(int nPlayId, const char *pcFuncName, const int nLine);

#define CSUDIEW200STrickMode(format, ...) CSUDIEW200STrickMode_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEW200STrickMode_PROXY(int nPlayId,CSUDI_BOOL bTrickMode, const char *pcFuncName, const int nLine);

#define CSUDIEW200SAddCallback(format, ...) CSUDIEW200SAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEW200SAddCallback_PROXY(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata, const char *pcFuncName, const int nLine);

#define CSUDIEW200SDelCallback(format, ...) CSUDIEW200SDelCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEW200SDelCallback_PROXY(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
