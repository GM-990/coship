#ifndef _UDI2_VIDEO_H_PROXY
#define _UDI2_VIDEO_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_screen.h"
#include "udi2_osg.h"
#include "udi2_error.h"
#include "../udi2/udi2_video.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIVIDEOAddCallback(format, ...) CSUDIVIDEOAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOAddCallback_PROXY(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent,void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDIVIDEODelCallback(format, ...) CSUDIVIDEODelCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEODelCallback_PROXY(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent, void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDIVIDEOSetVBIEnable(format, ...) CSUDIVIDEOSetVBIEnable_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOSetVBIEnable_PROXY(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL bEnable, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetVBIEnable(format, ...) CSUDIVIDEOGetVBIEnable_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetVBIEnable_PROXY(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL* pbEnable, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetCount(format, ...) CSUDIVIDEOGetCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetCount_PROXY(int * pnVideoDecCount, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetCapability(format, ...) CSUDIVIDEOGetCapability_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetCapability_PROXY(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOSnapshot(format, ...) CSUDIVIDEOSnapshot_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOSnapshot_PROXY(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect, CSUDI_HANDLE* phSurface, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetStreamStatus(format, ...) CSUDIVIDEOGetStreamStatus_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetStreamStatus_PROXY(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOShow(format, ...) CSUDIVIDEOShow_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOShow_PROXY(int nVideoIndex, CSUDI_BOOL bIsShow, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOSetAspectRatio(format, ...) CSUDIVIDEOSetAspectRatio_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOSetAspectRatio_PROXY(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetAspectRatio(format, ...) CSUDIVIDEOGetAspectRatio_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetAspectRatio_PROXY(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E *peAspectRatio, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOSetMatchMethod(format, ...) CSUDIVIDEOSetMatchMethod_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOSetMatchMethod_PROXY(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E eMatchMethod, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetMatchMethod(format, ...) CSUDIVIDEOGetMatchMethod_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetMatchMethod_PROXY(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E *peMatchMethod, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOSetStopMode(format, ...) CSUDIVIDEOSetStopMode_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOSetStopMode_PROXY(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetStopMode(format, ...) CSUDIVIDEOGetStopMode_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetStopMode_PROXY(int nVideoIndex, CSUDIVIDEOStopMode_E *peStopMode, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOSetWindowSize(format, ...) CSUDIVIDEOSetWindowSize_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOSetWindowSize_PROXY(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect , const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetWindowSize(format, ...) CSUDIVIDEOGetWindowSize_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetWindowSize_PROXY(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * pstRect , const char *pcFuncName, const int nLine);

#define CSUDIVIDEOSetWinClip(format, ...) CSUDIVIDEOSetWinClip_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOSetWinClip_PROXY(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect, const char *pcFuncName, const int nLine);

#define CSUDIVIDEOGetWinClip(format, ...) CSUDIVIDEOGetWinClip_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIVIDEOGetWinClip_PROXY(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * psRect, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
