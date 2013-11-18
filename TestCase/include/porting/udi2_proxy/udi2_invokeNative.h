#ifndef _UDI2_INVOKENATIVE_H_PROXY
#define _UDI2_INVOKENATIVE_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_invokeNative.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIInvokeNative(format, ...) CSUDIInvokeNative_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIInvokeNative_PROXY(void * pfFunction, CSUDIInvokeArg_S * psArgs, int nArgsSize, 
	CSUDIInvokeValueType_E eReturnType, CSUDIInvokeValue_S* psReturnValue, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
