#ifndef _UDI2_DESCRAMBLE_H_PROXY
#define _UDI2_DESCRAMBLE_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_descramble.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIDSMAllocate(format, ...) CSUDIDSMAllocate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSMAllocate_PROXY(int nDemuxIndex, CSUDI_HANDLE * phDSM, const char *pcFuncName, const int nLine);

#define CSUDIDSMFree(format, ...) CSUDIDSMFree_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSMFree_PROXY(CSUDI_HANDLE hDSM , const char *pcFuncName, const int nLine);

#define CSUDIDSMSetPID(format, ...) CSUDIDSMSetPID_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSMSetPID_PROXY(CSUDI_HANDLE hDSM,  unsigned short ushPID , const char *pcFuncName, const int nLine);

#define CSUDIDSMClearPID(format, ...) CSUDIDSMClearPID_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSMClearPID_PROXY(CSUDI_HANDLE hDSM, unsigned short ushPID , const char *pcFuncName, const int nLine);

#define CSUDIDSMSetOddKey(format, ...) CSUDIDSMSetOddKey_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSMSetOddKey_PROXY(CSUDI_HANDLE hDSM, const unsigned char * pucOddKey, unsigned char ucOddLength, const char *pcFuncName, const int nLine);

#define CSUDIDSMSetEvenKey(format, ...) CSUDIDSMSetEvenKey_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIDSMSetEvenKey_PROXY(CSUDI_HANDLE hDSM, const unsigned char * pucEvenKey, unsigned char ucEvenLength, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
