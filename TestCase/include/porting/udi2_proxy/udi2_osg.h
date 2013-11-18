#ifndef _UDI2_OSG_H_PROXY
#define _UDI2_OSG_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_osg.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIOSGCreateSurface(format, ...) CSUDIOSGCreateSurface_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGCreateSurface_PROXY(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight, CSUDI_HANDLE * phSurface, const char *pcFuncName, const int nLine);

#define CSUDIOSGDestroySurface(format, ...) CSUDIOSGDestroySurface_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGDestroySurface_PROXY(CSUDI_HANDLE hSurface, const char *pcFuncName, const int nLine);

#define CSUDIOSGGetDisplaySurface(format, ...) CSUDIOSGGetDisplaySurface_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGGetDisplaySurface_PROXY(int nIndex, CSUDI_HANDLE * phSurface, const char *pcFuncName, const int nLine);

#define CSUDIOSGGetSurfaceInfo(format, ...) CSUDIOSGGetSurfaceInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGGetSurfaceInfo_PROXY(CSUDI_HANDLE hSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo, const char *pcFuncName, const int nLine);

#define CSUDIOSGSetSurfaceInfo(format, ...) CSUDIOSGSetSurfaceInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGSetSurfaceInfo_PROXY(CSUDI_HANDLE hSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo, const char *pcFuncName, const int nLine);

#define CSUDIOSGBlit(format, ...) CSUDIOSGBlit_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGBlit_PROXY(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect,
                        CSUDI_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode, const char *pcFuncName, const int nLine);

#define CSUDIOSGFill(format, ...) CSUDIOSGFill_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGFill_PROXY(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel, const char *pcFuncName, const int nLine);

#define CSUDIOSGSync() CSUDIOSGSync_PROXY(__FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGSync_PROXY(const char *pcFuncName, const int nLine);

#define CSUDIOSGDecodeImage(format, ...) CSUDIOSGDecodeImage_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGDecodeImage_PROXY(const char * pszImageType, const void * pvImageMem, int nMemSize,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect,CSUDI_HANDLE * phSurface, const char *pcFuncName, const int nLine);

#define CSUDIOSGGetNativeDisplay(format, ...) CSUDIOSGGetNativeDisplay_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIOSGGetNativeDisplay_PROXY(CSUDI_HANDLE * phNativeDisplay, const char *pcFuncName, const int nLine);

#define CSUDIOSGGetNativeWindow(format, ...) CSUDIOSGGetNativeWindow_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGGetNativeWindow_PROXY(CSUDI_HANDLE hSurface, CSUDI_HANDLE *phNativeWindow, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
