#ifndef _UDI2_EEPROM_H_PROXY
#define _UDI2_EEPROM_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_eeprom.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIEPRWrite(format, ...) CSUDIEPRWrite_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEPRWrite_PROXY(CSUDI_UINT32 dwStartAddress, const unsigned char * pucData, unsigned int  uDataLength, const char *pcFuncName, const int nLine);

#define CSUDIEPRRead(format, ...) CSUDIEPRRead_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEPRRead_PROXY(CSUDI_UINT32 dwStartAddress, unsigned char * pucData, unsigned int  uDataLength , const char *pcFuncName, const int nLine);

#define CSUDIEPRGetInfo(format, ...) CSUDIEPRGetInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIEPRGetInfo_PROXY(CSUDIEPRInfo_S * psEPRInfo, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
