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
	ע��JS��չģ��
 
Input:
	hHandle : ��������,Ŀǰ���봫0
 
Output:
 
Return Value:
	
 
Other:
*******************************************************************************/
void CSJSExtRegisterJS(HCSHANDLE hHandle);

/******************************************************************************
Function Name:    CSJSExtRegisterSatelliteJS
 
Description:
	ע������JS��չģ��
 
Input:
	hHandle : ��������,Ŀǰ���봫0
 
Output:
 
Return Value:
	
 
Other:
*******************************************************************************/
void CSJSExtRegisterSatelliteJS(HCSHANDLE hHandle);

/******************************************************************************
Function Name:    CSJSExtRegisterTerrestrialJS
 
Description:
	ע�����JS��չģ��
 
Input:
	hHandle : ��������,Ŀǰ���봫0
 
Output:
 
Return Value:
	
 
Other:
*******************************************************************************/
void CSJSExtRegisterTerrestrialJS(HCSHANDLE hHandle);

/******************************************************************************
Function Name:    CSJSExtRegisterVOD
 
Description:
	ע��JS �㲥��չģ��
 
Input:
	hHandle : ��������,Ŀǰ���봫0
 
Output:
 
Return Value:
	
 
Other:
*******************************************************************************/
void CSJSExtRegisterVOD(HCSHANDLE hHandle);

#ifdef __cplusplus
}
#endif

#endif
