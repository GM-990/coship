#ifndef JAVASHELL_SMC_H
#define JAVASHELL_SMC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

typedef enum CSShell_SMC_CardChangedEvent_E
{
	EM_CSSHELL_CARD_VALID,
	EM_CSSHELL_CARD_INVALID
}CSShell_SMC_CardChangedEvent_E;

typedef struct _SmartCardFunc
{
	int (*fnGetSmartCardID)(char* pacCardIdStr, int* pnLen);
}SmartCardFunc_S;


void RegSmartCardFunc(SmartCardFunc_S smcfunc);
/******************************************************************************
Function Name:    FOnCSShellNDSCACardChanged
	
Description:
	smc状态变化回调函数原型。
Input:
    dwUserData:用户数据
    eCardStatus:smartcard的状态
    nPara1:状态参数1，一般用于传卡号
    nPara2:状态参数1，预留
Output:
    None

Return Value:
	成功:0
	失败:其他
Other:
*******************************************************************************/

typedef int (*FOnCSShellNDSCACardChanged)(DWORD nUserData,CSShell_SMC_CardChangedEvent_E eCardStatus,int nPara1,int nPara2);

/******************************************************************************
Function Name:    CSShell_SMC_AddCardChangedCallback
	
Description:
	增加smc状态变化的侦听
Input:
	dwUserData:用户数据
	fnCallback:回调函数
Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
void CSShell_SMC_AddCardChangedCallback(DWORD dwUserData,FOnCSShellNDSCACardChanged fnCallback);
/******************************************************************************
Function Name:    CSShell_SMC_RemoveCardChangedCallback
	
Description:
	增加smc状态变化的侦听
Input:
	dwUserData:用户数据
	fnCallback:回调函数
Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
void CSShell_SMC_RemoveCardChangedCallback(DWORD dwUserData,FOnCSShellNDSCACardChanged fnCallback);

#ifdef __cplusplus
}
#endif

#endif  /* JAVASHELL_SMC_H */

