#ifndef _UDI2_DISEQC_H_PROXY
#define _UDI2_DISEQC_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_diseqc.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIDSC12RotateTimes(format, ...) CSUDIDSC12RotateTimes_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12RotateTimes_PROXY(CSUDI_UINT32 dwTunerId, CSUDIRotateDirection_E eDirection, CSUDI_UINT8 ucTime, const char *pcFuncName, const int nLine);

#define CSUDIDSC12RotateCycles(format, ...) CSUDIDSC12RotateCycles_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12RotateCycles_PROXY(CSUDI_UINT32 dwTunerId, CSUDIRotateDirection_E eDirection,CSUDI_UINT8 ucCycles, const char *pcFuncName, const int nLine);

#define CSUDIDSC12StorePosition(format, ...) CSUDIDSC12StorePosition_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12StorePosition_PROXY(CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition, const char *pcFuncName, const int nLine);

#define CSUDIDSC12GotoPosition(format, ...) CSUDIDSC12GotoPosition_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12GotoPosition_PROXY(CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition, const char *pcFuncName, const int nLine);

#define CSUDIDSC12ResetPosition(format, ...) CSUDIDSC12ResetPosition_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12ResetPosition_PROXY(CSUDI_UINT32 dwTunerId, const char *pcFuncName, const int nLine);

#define CSUDIDSC12RotateWithoutStop(format, ...) CSUDIDSC12RotateWithoutStop_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12RotateWithoutStop_PROXY(CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucDirection, const char *pcFuncName, const int nLine);

#define CSUDIDSC12RotateStop(format, ...) CSUDIDSC12RotateStop_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12RotateStop_PROXY(CSUDI_UINT32 dwTunerId, const char *pcFuncName, const int nLine);

#define CSUDIDSC12DisableLimit(format, ...) CSUDIDSC12DisableLimit_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12DisableLimit_PROXY(CSUDI_UINT32 dwTunerId, const char *pcFuncName, const int nLine);

#define CSUDIDSC12SetLimit(format, ...) CSUDIDSC12SetLimit_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12SetLimit_PROXY(CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucDirection, const char *pcFuncName, const int nLine);

#define CSUDIDSC12AdjustTAB(format, ...) CSUDIDSC12AdjustTAB_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12AdjustTAB_PROXY(CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition, const char *pcFuncName, const int nLine);

#define CSUDIDSC12GotoSTABPosition(format, ...) CSUDIDSC12GotoSTABPosition_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12GotoSTABPosition_PROXY(CSUDI_UINT32 dwTunerId, double dlAngle, const char *pcFuncName, const int nLine);

#define CSUDIDSC12GetAntennaAngle(format, ...) CSUDIDSC12GetAntennaAngle_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12GetAntennaAngle_PROXY(double dlSatLongitude, double dlMyLongitude, double dlMyLatitude, double *pdlAngle, const char *pcFuncName, const int nLine);

#define CSUDIDSC12TurnSateWithUsals(format, ...) CSUDIDSC12TurnSateWithUsals_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSC12TurnSateWithUsals_PROXY(CSUDI_UINT32 dwTunerId, const CSUDISatLocationInfo_S *psSatLocationInfo, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
