/******************************************************************************
FileName:  SAU_CONTROL.H
Copyright (c) 2008, Shen Zhen Coship Electronics CO., LTD. 

Version:       Ver 1.0
Author:        Ryan Ren (renyan@coship.com)

File Description:
        File Description, Please add you description here.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2008-12-24  Ryan Ren        Create
*******************************************************************************/

#ifndef SAU_CONTROL_H
#define SAU_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 *                                 Header File Include                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 Macro/Define/Structus                           *
 ******************************************************************************/
typedef enum SAU_Status_
{
	SAU_IDLE = 1,
	SAU_RUNNING,
	SAU_STORE,

	SAU_STATUS_COUNT
}SAU_Status_t;
/******************************************************************************
 *                                 Global Function Declare                                   *
 ******************************************************************************/

/******************************************************************************
Function Name:    SAUCALLBACK

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
SAU_Error_t SAU_CallBack(SAUCALLBACK_t emCallBackType, LPARAM lParam, WPARAM wParam);

/******************************************************************************
Function Name:    SAU_GetMonitorFEType

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
emFrontEndType SAU_GetMonitorFEType(void);

/******************************************************************************
Function Name:    SAU_GetPMTPolicy

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
DWORD SAU_GetPMTPolicy(void);

/******************************************************************************
Function Name:    SAU_GetSDTPolicy

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
DWORD SAU_GetSDTPolicy(void);

/******************************************************************************
Function Name:    SAU_GetSDTPolicy

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
DWORD SAU_GetSDTOtherRequestPolicy(void);

/******************************************************************************
Function Name:    SAU_GetPMTSectionPolicy

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
DWORD SAU_GetPMTSectionPolicy(void);


/******************************************************************************
Function Name:    SAU_GetRuningStatus

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
SAU_Status_t SAU_GetSAUProcessStatus(void);

/******************************************************************************
Function Name:    SAU_StartAutoTesting

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
void  SAU_StartAutoTesting(enSAUTestType nTestType);
/******************************************************************************
Function Name:    MonitorNITSectionCallBack

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
void MonitorNITSectionCallBack(SectionEventType event, Section* pSection, void* pUserData, HCSHANDLE hRequest);
/******************************************************************************
Function Name:    SAUNITCheckSection

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
BOOL  SAUNITCheckSection(PBYTE pbNITData, DWORD nLength);
/******************************************************************************
Function Name:    SAUNITCheckDescriptors

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
BOOL SAUNITCheckDescriptors(PBYTE pData, DWORD nLength);
/******************************************************************************
Function Name:    SAU_SetRuningStatus

Description:
        Function Description.

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
void SAU_SetSAUProcessStatus(SAU_Status_t status);

void SAU_SetProcessAvailable(int i);

int SAU_GetProcessAvailable(void);

DWORD CSShellSAULockDM();

DWORD  CSShellSAUUnlockDM();

void SAU_ResetFinishFalg(void);

#ifdef __cplusplus
}
#endif

#endif  /* SAU_CONTROL_H */

/* End of File */

