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
	smc״̬�仯�ص�����ԭ�͡�
Input:
    dwUserData:�û�����
    eCardStatus:smartcard��״̬
    nPara1:״̬����1��һ�����ڴ�����
    nPara2:״̬����1��Ԥ��
Output:
    None

Return Value:
	�ɹ�:0
	ʧ��:����
Other:
*******************************************************************************/

typedef int (*FOnCSShellNDSCACardChanged)(DWORD nUserData,CSShell_SMC_CardChangedEvent_E eCardStatus,int nPara1,int nPara2);

/******************************************************************************
Function Name:    CSShell_SMC_AddCardChangedCallback
	
Description:
	����smc״̬�仯������
Input:
	dwUserData:�û�����
	fnCallback:�ص�����
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
	����smc״̬�仯������
Input:
	dwUserData:�û�����
	fnCallback:�ص�����
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

