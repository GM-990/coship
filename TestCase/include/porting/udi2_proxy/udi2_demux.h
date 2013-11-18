#ifndef _UDI2_DEMUX_H_PROXY
#define _UDI2_DEMUX_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_demux.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIDEMUXGetCount(format, ...) CSUDIDEMUXGetCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDEMUXGetCount_PROXY(int * pnDemuxCount, const char *pcFuncName, const int nLine);

#define CSUDIDEMUXGetCapability(format, ...) CSUDIDEMUXGetCapability_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDEMUXGetCapability_PROXY(int nDemuxIndex,CSUDIDEMUXCapability_S * psCapabilityInfo, const char *pcFuncName, const int nLine);

#define CSUDIDEMUXConnectTuner(format, ...) CSUDIDEMUXConnectTuner_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDEMUXConnectTuner_PROXY(CSUDI_UINT32 dwTunerId, int nDemuxIndex, const char *pcFuncName, const int nLine);

#define CSUDIDEMUXDisconnectTuner(format, ...) CSUDIDEMUXDisconnectTuner_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDEMUXDisconnectTuner_PROXY(CSUDI_UINT32 dwTunerId, int nDemuxIndex, const char *pcFuncName, const int nLine);

#define CSUDIDEMUXGetFreeFilterCount(format, ...) CSUDIDEMUXGetFreeFilterCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDEMUXGetFreeFilterCount_PROXY(int nDemuxIndex, int * pnFreeFilterCount, const char *pcFuncName, const int nLine);

#define CSUDIDEMUXGetTsPacketCount(format, ...) CSUDIDEMUXGetTsPacketCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDEMUXGetTsPacketCount_PROXY(int nDemuxIndex, unsigned int * puTsPacketCount, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
