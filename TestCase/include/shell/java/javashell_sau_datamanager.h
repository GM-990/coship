/******************************************************************************
FileName:  SAU_DATAMANAGER.H
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

#ifndef SAU_DATAMANAGER_H
#define SAU_DATAMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 *                                 Header File Include                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 Macro/Define/Structus                           *
 ******************************************************************************/
#define SAU_INVALID_SERVICE_HANDLE			(0x12345678)

/******************************************************************************
 *                                 Global Function Declare                                   *
 ******************************************************************************/
/******************************************************************************
Function Name:    SAU_DMInitialize

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
SAU_Error_t SAU_DMInitialize(WORD net_id, WORD ts_id, WORD sat_index, WORD tp_index);

/******************************************************************************
Function Name:    SAU_DMIsMonitorTS

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
BOOL SAU_DMIsMonitorTS(WORD ts_id);

/******************************************************************************
Function Name:    SAU_DMIsMonitorNetWork

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
BOOL SAU_DMIsMonitorNetWork(WORD net_id);

/******************************************************************************
Function Name:    SAU_DMSetMonitorTP

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
SAU_Error_t SAU_DMSetMonitorInfo(HCSHANDLE hTP, HCSHANDLE hSrv);

/******************************************************************************
Function Name:    SAU_DMGetServiceHandleFromDM

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
HCSHANDLE SAU_DMGetServiceHandleFromDM(WORD o_net_id, WORD ts_id, WORD srv_id);

/******************************************************************************
Function Name:    SAU_DMGetServiceHandleFromTS

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
HCSHANDLE SAU_DMGetServiceHandleFromTS(WORD o_net_id, WORD ts_id, WORD srv_id);

/******************************************************************************
Function Name:    SAU_DMFindServiceInDMList

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
HCSHANDLE SAU_DMFindServiceInDMList(WORD srv_id, HCSHANDLE hTP);
/******************************************************************************
Function Name:    SAU_DMFindServiceInTempList

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
HCSHANDLE SAU_DMFindServiceInTempList(WORD srv_id, HCSHANDLE hTP);

/******************************************************************************
Function Name:    SAU_CreateNewService

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
HCSHANDLE SAU_DMCreateNewService(WORD srv_id, HCSHANDLE hTP);

/******************************************************************************
Function Name:    SAU_DMGetMornitorServiceHandle

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
HCSHANDLE SAU_DMGetMonitorServiceHandle(void);

/******************************************************************************
Function Name:    SAU_UpdateServiceName

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
SAU_Error_t SAU_DMUpdateServiceName(HCSHANDLE hService, PBYTE pName, WORD nLen);

/******************************************************************************
Function Name:    SAU_DMRemoveServiceFromDM

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
SAU_Error_t SAU_DMRemoveServiceFromDM(HCSHANDLE hSrv, HCSHANDLE hTP);

/******************************************************************************
Function Name:    SAU_DMLoadTranspoder

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
SAU_Error_t SAU_DMLoadTranspoder(void);

/******************************************************************************
Function Name:    SAU_DMGetTPHandleFromDM

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
HCSHANDLE SAU_DMGetTPHandleFromDM(WORD o_net_id, WORD ts_id);

/******************************************************************************
Function Name:    SAU_DMGetTPHandleFromTS

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
HCSHANDLE SAU_DMGetTPHandleFromTS(WORD o_net_id, WORD ts_id);

/******************************************************************************
Function Name:    SAU_DMGetTPHandleFromTempTPList

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
HCSHANDLE SAU_DMGetTPHandleFromTempTPList(WORD o_net_id, WORD ts_id);

/******************************************************************************
Function Name:    SAU_DMFindTPInDMList

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
HCSHANDLE SAU_DMFindTPInDMList(WORD net_id, WORD ts_id);

/******************************************************************************
Function Name:    SAU_DMUpdateTPInDMAndTSFromTPKeyDefault

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
HCSHANDLE SAU_DMUpdateTPInDMAndTSFromTPKeyDefault(HCSHANDLE hTpInTs);

/******************************************************************************
Function Name:    SAU_CreateNewTransponder

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
HCSHANDLE SAU_DMCreateNewTransponder(WORD net_id, WORD o_net_id, WORD ts_id);

/******************************************************************************
Function Name:    SAU_DMUpdateTPData

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
SAU_Error_t SAU_DMUpdateTPData(HCSHANDLE hTP, Deliver *deliver_p);

/******************************************************************************
Function Name:    SAU_DMSetTransponderSModulation

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
SAU_Error_t SAU_DMSetTransponderSModulation(BYTE bModulation, BYTE bStandard, HCSHANDLE hTP);

/******************************************************************************
Function Name:    SAU_DMBuildServiceList

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
SAU_ServiceList_t* SAU_DMBuildServiceList(HCSHANDLE hTP);

/******************************************************************************
Function Name:    SAU_DMBuildNewServiceList

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
SAU_ServiceList_t* SAU_DMBuildNewServiceList(HCSHANDLE hTP);

/******************************************************************************
Function Name:    SAU_DMBuildRemoveServiceList

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
SAU_ServiceList_t* SAU_DMBuildRemoveServiceList(HCSHANDLE hTP);

/******************************************************************************
Function Name:    SAU_DMBuildNewTPList

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
SAU_TPList_t* SAU_DMBuildNewTPList(void);

/******************************************************************************
Function Name:    SAU_DMBuildRemoveTPList

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
SAU_TPList_t* SAU_DMBuildRemoveTPList(void);

/******************************************************************************
Function Name:    SAU_DMGetDMStatus

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
DWORD SAU_DMGetDMStatus(void);

/******************************************************************************
Function Name:    SAU_DMResetDMStatus

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
void SAU_DMResetDMStatus(void);

/* added by tengyuandao */
BOOL	SAU_DMAddLogicNumByServiceId(WORD	svrId,  WORD	logicNum);
WORD	SAU_DMGetLogNumByServiceId(WORD	svrId);
BOOL	SAU_DMInitLogicNumToServiceId();

/* 找到一个即在DM中又在TS的节目，着个节目是在删除当前频点或节目
	时可以播放的有效节目 
*/
HCSHANDLE	SAU_DMGetServiceDataInDMAndTS();

SAU_Error_t SAU_DMSynchronize();

SAU_Error_t SAU_DMLoadService(HCSHANDLE hTP);

SAU_Error_t SAU_DMAddServiceToDM(HCSHANDLE hSrv, HCSHANDLE hTP);

SAU_Error_t SAU_DMResetServiceStatus(HCSHANDLE hTP);

SAU_Error_t SAU_DMAddTransponderToDM(HCSHANDLE hTP);

SAU_Error_t SAU_DMRemoveTransponderFromDM(HCSHANDLE hTP);

SAU_Error_t SAU_DMResetTransponderStatus();

void SAU_DMSortNewAddedService(void);


SAU_TPList_t* SAU_DMBuildAllDMTPList(void);

SAU_Error_t SAU_ProcessAllNewService(void);
SAU_Error_t SAU_ProcessAllRemoveService(void);
SAU_Error_t SAU_ProcessAllNewTP(void);
SAU_Error_t SAU_ProcessAllRemoveTP(void);
BOOL  SAU_FreeAllNewServiceHandle(void);
BOOL  SAU_FreeAllNewTPHandle(void);
SAU_Error_t SAU_ProcessAllUpdateTP(void);
SAU_Error_t SAU_AddUpdateTPToTemp(HCSHANDLE hTP, Deliver *deliver_p);
void SAU_FilterLogicNumInDMAfterUpdate(void);

#ifdef __cplusplus
}
#endif

#endif  /* SAU_DATAMANAGER_H */

/* End of File */

