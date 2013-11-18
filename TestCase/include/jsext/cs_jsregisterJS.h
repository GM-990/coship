#ifndef __CS_JSREGISTERJS_H__
#define __CS_JSREGISTERJS_H__

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
void CSJSExtRegisterJS(HCSHANDLE hHandle);

/******************************************************************************
Function Name:    CSJSExtRegisterSatelliteJS
 
Description:
	注册卫星JS扩展模块
 
Input:
	hHandle : 保留参数,目前必须传0
 
Output:
 
Return Value:
	
 
Other:
*******************************************************************************/
void CSJSExtRegisterSatelliteJS(HCSHANDLE hHandle);

/******************************************************************************
Function Name:    CSJSExtRegisterTerrestrialJS
 
Description:
	注册地面JS扩展模块
 
Input:
	hHandle : 保留参数,目前必须传0
 
Output:
 
Return Value:
	
 
Other:
*******************************************************************************/
void CSJSExtRegisterTerrestrialJS(HCSHANDLE hHandle);

/******************************************************************************
Function Name:    CSJSExtRegisterVOD
 
Description:
	注册JS 点播扩展模块
 
Input:
	hHandle : 保留参数,目前必须传0
 
Output:
 
Return Value:
	
 
Other:
*******************************************************************************/
void CSJSExtRegisterVOD(HCSHANDLE hHandle);

#ifdef __cplusplus
}
#endif

#endif
