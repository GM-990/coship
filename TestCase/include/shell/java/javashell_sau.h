/******************************************************************************
FileName:  SAU.H
Copyright (c) 2008, Shen Zhen Coship Electronics CO., LTD. 

Version:       Ver 1.0
Author:        Ryan Ren (renyan@coship.com)

File Description:
        Service auto upgrading(SAU) interface head file.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2008-12-01  Ryan Ren        Create
*******************************************************************************/

#ifndef _SAU_H_
#define _SAU_H_

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 *                                 Header File Include                                 *
 ******************************************************************************/
 #include "mmcp_typedef.h"
 #include "mmcp_os.h"
 #include "CSUSPDMCommonType.h"
 #include "SectionInterface2C.h"
 #include "SICommonType.h"

/******************************************************************************
 *                                 Macro/Define/Structus                           *
 ******************************************************************************/
#define SAU_DEBUG

#ifdef 	SAU_DEBUG
#define SAU_DBG 	USPPrint
#else
#define SAU_DBG 1?(void)0:USPPrint
#endif


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

typedef  DWORD 				LPARAM;
typedef  DWORD 				WPARAM;
 /*Front-end type*/
typedef enum _Frontend_type
{
	DVB_S = 1,
	DVB_C,
	DVB_T,
	DVB_S2,

	DVB_FE_TYPE_COUNT
}emFrontEndType;

typedef  enum  _ENMTestType
{
	SAU_ADDTP_TESTING,
	SAU_DELTP_TESTING,
	SAU_ADDSRV_TESTING,
	SAU_DELSRV_TESTING,
	SAU_CHANGETP_TESTING,
	SAU_CHANGESRVNAME_TESTING,
	SAU_OTHER_TESTING	
} enSAUTestType;

//Auto SI Update policy
#define SAU_SI_PAT_PMT				(0x00000001)
#define SAU_SI_SDT					(0x00000002)
#define SAU_SI_NIT					(0x00000004)
#define SAU_SI_BAT					(0x00000008)

/*
PMT Update policy
1. Only update playing service
2. Update all service in current TS
*/
#define PMT_UPDATE_SERVICE			(0x00000001)
#define PMT_UPDATE_TS				(0x00000002)

/*
SDT update policy
1. Update SDT of current TS
2. Update SDT of other TS
*/
#define SDT_UPDATE_ACT				(0x00000001)
#define SDT_UPDATE_OTHER			(0x00000002)

//NIT update policy
#define NIT_UPDATE_ACT				(0x00000001)
#define NIT_UPDATE_OTHER			(0x00000002)

/*
PMT Section request policy
1.
2.
*/
#define PMT_UPDATE_SERIAL			(0x00000001)
#define PMT_UPDATE_PARALLEL			(0x00000002)

//SDT Section request policy
#define SDT_OTHER_NO_TSID			(0x00000001)
#define SDT_OTHER_TSID				(0x00000002)

//DM modify Type
#define DM_SERVICE_ADD				(0x00000001)
#define DM_SERVICE_DELETE			(0x00000002)
#define DM_SERVICE_ATTR_CHANGE		(0x00000004)
#define DM_TP_ADD					(0x00000008)
#define DM_TP_DELETE				(0x00000010)
#define DM_TP_PARAM_CHANGE			(0x00000020)

typedef enum SAU_Error_type
{
	SAU_SUCCESS = 0,
	SAU_PARAM_INVALID = 1,
	SAU_NOT_INITIALIZED = 2,
	SAU_MEMORY_EXHAUST = 4,
	SAU_FATAL_ERROR = 8,

	SAU_NETWORK_NOT_MATCH = 0x10,
	SAU_APP_NO_ACTION = 0x20,
	
	SAU_ERROR_COUNT
}SAU_Error_t;

//SAU CALLBACK type
typedef enum Sau_Callback_t
{
	SAU_PAT_FINISH,
	SAU_PMT_FINISH,
	SAU_SDT_FINISH,
	SAU_SDT_FINISH_ADD_SERVICE,
	SAU_SDT_FINISH_DELETE_SERVICE,
	SAU_SERVICE_DEL_REPLAY,
	SAU_NIT_FINISH,
	SAU_NIT_FINISH_ADD_SERVICE,
	SAU_NIT_FINISH_DELETE_SERVICE,
	SAU_NIT_FINISH_ADD_TP,
	SAU_NIT_FINISH_DELETE_TP,
	SAU_TP_DEL_REPLAY,
	SAU_BAT_FINISH,

	SAU_PMTPID_CHANGED,

	SAU_DEL_GROUP,

	/* added by tengyuandao for topway special  */
	SAU_SDT_AND_NIT_OVER,

	SAU_CALLBACK_TYPE_COUNT
}SAUCALLBACK_t;

typedef SAU_Error_t (*SAUCALLBACK)(SAUCALLBACK_t emCallBackType, LPARAM lParam, WPARAM wParam);

typedef struct _SAU_InitParam
{
	//SAU thread parameter
	DWORD			m_dwThreadPriority;
	DWORD			m_dwStackSize;

	//FE type
	emFrontEndType	m_emFEType;

	//Upgrading policy
	DWORD			m_dwUpgradPolicy;
	DWORD			m_dwPMTPolicy;
	DWORD			m_dwNITPolicy;
	DWORD			m_dwSDTPolicy;
	DWORD			m_dwBATPolicy;

	//Section request policy
	DWORD			m_dwPMTSection;
	DWORD			m_dwSDTOtherSection;

	//Monitor Network Information, if not want to specify the network information, these 2 params are 0xFFFF
	WORD			m_dwTS_id;
	WORD			m_dwNet_id;

	//Monitor Satellite index, if it is Cable/Terrial system, let it be 0xFFFF
	WORD			m_dwSatIndex;

	//Monitor Deliver Index, If you want to monitor every deliver, let it be 0xFFFF
	WORD			m_dwTPIndex;

	SAUCALLBACK	m_pSAUCallBack;	
}SAU_InitParam_t;

typedef BOOL (*SAU_IsNeedSave)(HCSHANDLE hSrv);

//Private descriptor parse function callback in SDT, NIT, BAT
typedef void (*DescriptorCallBack)(HCSHANDLE hTP, PBYTE pDescriptorData, WORD dwDesLen);

typedef struct SAU_Service
{
	HCSHANDLE 	m_hService;
	HCSHANDLE	m_hTp;
	BOOL		m_bInDM;
}SAU_Service_t;

typedef struct SAU_ServiceList
{
	SAU_Service_t*	m_pService;
	int				m_nServiceCount;
	HCSHANDLE		m_hTP;
}SAU_ServiceList_t;

typedef struct SAU_TP
{
	HCSHANDLE 	m_hTP;
	BOOL		m_bInDM;
}SAU_TP_t;

typedef struct SAU_TPList
{
	SAU_TP_t*	m_pTP;
	int			m_nTPCount;
}SAU_TPList_t;

typedef struct _TPANDSERVICEHANDLE
{
	HCSHANDLE hTp;
	HCSHANDLE hService;
}TPANDSERVICEHANDLE;


typedef struct SAU_ServiceName
{
	HCSHANDLE hTp;
	DWORD dwSrvID;
	char * serviceName;
	BYTE  bType;
}SAU_ServiceName_t;


//Update Message type
#define SAU_MSG_BEGIN					(0x800000000)
#define SAU_MSG_SRVUPDATE				(SAU_MSG_BEGIN + 1)
#define SAU_MSG_TPUPDATE				(SAU_MSG_BEGIN + 2)
#define SAU_MSG_REPLAY_CURSERVICE	(SAU_MSG_BEGIN + 3)
#define SAU_MSG_CURSRV_REMOVED		(SAU_MSG_BEGIN + 4)
#define SAU_MSG_FATAL_ERROR			(SAU_MSG_BEGIN + 5)




/******************************************************************************
 *                                 Global Function Declare                                   *
 ******************************************************************************/

/******************************************************************************
Function Name:    SAUInitialize

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
SAU_Error_t SAUInitialize(SAU_InitParam_t* pInitParam);


/******************************************************************************
Function Name:    SAUStart

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
SAU_Error_t SAUStart(HCSHANDLE hTP, HCSHANDLE hSrv);

/******************************************************************************
Function Name:    SAUStop

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
SAU_Error_t SAUStop(void);


/******************************************************************************
Function Name:    SAU_RegisterSDTPrivateDesCallBack

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
SAU_Error_t SAU_RegisterSDTPrivateDesCallBack(DescriptorCallBack pCallBack, PBYTE TagArray_p, WORD dwCount);


/******************************************************************************
Function Name:    SAU_RegisterNITPrivateDesCallBack

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
SAU_Error_t SAU_RegisterNITPrivateDesCallBack(DescriptorCallBack pCallBack, PBYTE TagArray_p, WORD dwCount);


/******************************************************************************
Function Name:    SAU_RegisterBATPrivateDesCallBack

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
SAU_Error_t SAU_RegisterBATPrivateDesCallBack(DescriptorCallBack pCallBack, PBYTE TagArray_p, WORD dwCount);

/******************************************************************************
Function Name:    SAU_DMGetMonitorTP

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
HCSHANDLE SAU_DMGetMonitorTPHandle(void);

/******************************************************************************
Function Name:    SAU_DMGetServiceHandle

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
HCSHANDLE SAU_DMGetServiceHandle(WORD o_net_id, WORD ts_id, WORD srv_id, BOOL* pbInDM);

/******************************************************************************
Function Name:    SAU_DMGetTPHandle

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
HCSHANDLE SAU_DMGetTPHandle(WORD o_net_id, WORD ts_id, BOOL* pbInDM);


/******************************************************************************
Function Name:    SAU_GetVersion

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
SAU_Error_t SAU_GetVersion(char* const szVersion, INT nMaxLen);

/******************************************************************************
Function Name:    SAUInit

Description:
        节目自动更新的初始化

Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/

BOOL SAUInit(void);

/******************************************************************************
Function Name:   CSShellSAUGetUpdateMode

Description:
     获取节目更新方式。

Input:
    0:自动更新   1:提示消息手动更新

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/

int  CSShellSAUGetUpdateMode();

/******************************************************************************
Function Name:    SAU_setUpdateMode

Description:
     设置节目更新方式。

Input:
    0:自动更新   1:提示消息手动更新

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/

void CSShellSAUSetUpdateMode(INT mode);

/******************************************************************************
Function Name:    CSShellSAUGetSemaphore

Description:
        Function Description.
	获取对DM  操作的信号量
Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
CSUDI_HANDLE CSShellSAUGetSemaphore(void);


/******************************************************************************
Function Name:   CSShellSAUOpen

Description:
        Function Description.
     	 打开节目更新功能
Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
BOOL   CSShellSAUOpen();

/******************************************************************************
Function Name:    CSShellSAUClose

Description:
        Function Description.
     	关闭节目更新功能
Input:
    None

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/

BOOL   CSShellSAUClose();

void   CSShellSAUStoreNitHardVer(void);

void SAUNotifyServiceUpdate(int updateMsg);

void SAUNotifyServiceDatabase(void);

void SAUNotifyServiceDatabase(void);

SAU_Error_t SAU_SMFreeAllSectionHandle(void);

CSUSPDMServiceType   CSShellGetServiceType(BYTE type);


/******************************************************************************
Function Name:   CSShellSAUOpen

Description:
     	两个节目排序次序的比较函数
Input:
    hSrv1 service句柄
    hSrv2 service句柄

Output:
    None

Return Value:
    1:  hSrv1 >  hSrv2
    0:  hSrv1 =  hSrv2
  -1:  hSrv1 <  hSrv2

Other:
*******************************************************************************/
int  CSShellSAUCompareService(HCSHANDLE hSrv1,HCSHANDLE hSrv2);



/******************************************************************************
Function Name:   CSShellSAUIsServiceNeedFilter

Description:
     判断该节目是够需要被过滤掉
Input:
     hSrv service句柄

Output:
    None

Return Value:
    true:需要过滤   flase:不需要过滤

Other:
*******************************************************************************/
 BOOL CSShellSAUIsServiceNeedFilter(HCSHANDLE hSrv);

/******************************************************************************
Function Name:   CSShellIsServiceNeedFilter

Description:
     节目更新完成之后，对DM  中的 节目进行过滤
Input:
     hSrv service句柄

Output:
    None

Return Value:
    true:需要过滤   flase:不需要过滤

Other:
*******************************************************************************/
void CSShellSAUFilterAllService(void);


/******************************************************************************
Function Name:   CSShellIsServiceNeedFilter

Description:
   设置NIT  版本号，供NIT 监听时用
Input:
     version  NIT的版本号

Output:
    None

Return Value:
    None

Other:
*******************************************************************************/
void CSShellSAUSetNitVersion(int version);
#ifdef __cplusplus
}
#endif

#endif  /* SAU_H */
