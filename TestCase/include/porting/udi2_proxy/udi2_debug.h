#ifndef _UDI2_DEBUG_H_PROXY
#define _UDI2_DEBUG_H_PROXY

#include "../udi2/udi2_debug.h"

#ifdef __cplusplus
extern "C"
{
#endif

//#define CSDebugDefaultOutputString(format, ...) CSDebugDefaultOutputString_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)



void CSDebugDefaultOutputString_PROXY(const char * pStr , const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
