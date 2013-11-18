#ifndef __CS_JSREGISTERGTOOL_H__
#define __CS_JSREGISTERGTOOL_H__

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
Function Name:    CSJSExtRegister
 
Description:
	注册JS扩展模块
 
Input:
	hHandle : 保留参数,目前必须传0
 
Output:
 
Return Value:
	
 
Other:
*******************************************************************************/
void CSJSExtRegisterGTool(HCSHANDLE hHandle);

#ifdef __cplusplus
}
#endif

#endif
