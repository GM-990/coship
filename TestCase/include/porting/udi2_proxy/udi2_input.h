#ifndef _UDI2_INPUT_H_PROXY
#define _UDI2_INPUT_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_input.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIINPUTAddCallback(format, ...) CSUDIINPUTAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINPUTAddCallback_PROXY(CSUDIInputCallback_F fnInputCallback ,void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDIINPUTDelCallback(format, ...) CSUDIINPUTDelCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINPUTDelCallback_PROXY(CSUDIInputCallback_F fnInputCallback ,void * pvUserData, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
