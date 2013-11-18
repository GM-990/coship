#ifndef _UDI2_FLASH_H_PROXY
#define _UDI2_FLASH_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_flash.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIFlashRead(format, ...) CSUDIFlashRead_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFlashRead_PROXY(CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf,  unsigned int uDataLength, const char *pcFuncName, const int nLine);

#define CSUDIFlashWrite(format, ...) CSUDIFlashWrite_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFlashWrite_PROXY(CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf, unsigned int uDataLength, const char *pcFuncName, const int nLine);

#define CSUDIFlashGetInfo(format, ...) CSUDIFlashGetInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFlashGetInfo_PROXY(CSUDIFLASHRegionSuitType_E eSuitType,CSUDIFLASHRegionSuit_S * psFlashSuit, const char *pcFuncName, const int nLine);

#define CSUDIFlashEraseSpace(format, ...) CSUDIFlashEraseSpace_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFlashEraseSpace_PROXY(CSUDI_UINT32 dwStartAddress, unsigned int uSpaceSize, const char *pcFuncName, const int nLine);

#define CSUDIFlashWriteWithoutErase(format, ...) CSUDIFlashWriteWithoutErase_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFlashWriteWithoutErase_PROXY(CSUDI_UINT32 dwStartAddress, const unsigned char * pucBuf, unsigned int uDataLength, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
