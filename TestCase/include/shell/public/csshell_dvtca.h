/**@defgroup PublicShellDVTCA PublicShellDVTCA 定义数码视讯CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_DVTCA_H
#define CSSHELL_DVTCA_H


#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,pls check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "CS_SHIXUNCA_Interface.h"

#define CSSHELL_DVTCA_SMCID_MODE_EXTERNAL	0	//外部卡号。数码视讯ca使用的卡号默认为外部卡号
#define CSSHELL_DVTCA_SMCID_MODE_INTERNAL	1	//内部卡号

/**
@brief 数码视讯CA信息及相关操作结构体
@note 目前mmcp提供了dvt ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellDvtCaSetParam CSShellDvtCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(ShellDvtCa_S)
    CSShellCaType_E         m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_DVT
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口	

    int (*m_fnGetCoreVer)(char* pcVer, int* pnLen);///<CS_CA_GetCoreVer
    int (*m_fnTxtAdd)(T_CaMail* pstTxt);///<CS_CA_TXT_Add
    int (*m_fnTxtGetInfo)(int nIndex, T_CaMail* pstTxt);///<CS_CA_TXT_GetInfo
    int (*m_fnTxtDel)(int nIndex);///<CS_CA_TXT_Del
    int (*m_fnTxtDelAll)(void);///<CS_CA_TXT_DelAll
    int (*m_fnTxtGetNum)(void);///<CS_CA_TXT_GetNum
    int (*m_fnTxtSave)(void);///<CS_CA_TXT_Save
    int (*m_fnTxtSetReadFlag)(int nIndex, BOOL bRead);///<CS_CA_TXT_SetReadFlag
    int (*m_fnTxtHaveNewMailNum)(void);///<CS_CA_TXT_HaveNewMailNum
    int (*m_fnMMICheckPinCode)(DWORD dwPin);///<CS_CA_MMI_CheckPinCode
    int (*m_fnMMIModifyPinCode)(DWORD dwOldPin, DWORD dwNewPin);///<CS_CA_MMI_ModifyPinCode
    int (*m_fnMMIRequestBookPpvMenu)(void);///<CS_CA_MMI_RequestBookPpvMenu
    int (*m_fnMMIRequestViewedPpvManu)(void);///<CS_CA_MMI_RequestViewedPpvManu
    int (*m_fnMMIPpvBook)(T_IppDetailInfo* pstIppInfo);///<CS_CA_MMI_PpvBook
    int (*m_fnMMIPpvCancel)(WORD wEcmPid);///<CS_CA_MMI_PpvCancel
    int (*m_fnMMIRequestPPIDMoneyInfo)(WORD wPPID);///<CS_CA_MMI_RequestPPIDMoneyInfo
    int (*m_fnMMIRequestWorkTime)(void);	///<CS_CA_MMI_RequestWorkTime
    int (*m_fnMMIModifyWorkTime)(DWORD dwPin, T_WorkTime* pstWorkTime);///<CS_CA_MMI_ModifyWorkTime
    int (*m_fnMMIequestSmcInfo)(void);///<CS_CA_MMI_RequestSmcInfo
    int (*m_fnMMIRequestPPIDInfo)(void);///<CS_CA_MMI_RequestPPIDInfo
    int (*m_fnMMIRequestDetailPPIDInfo)(DWORD dwPin, DWORD dwPpId);///<CS_CA_MMI_RequestDetailPPIDInfo
    int (*m_fnMMINotifyCaShowOSDOver)(DWORD dwDuration);///<CS_CA_MMI_NotifyCaShowOSDOver
    int (*m_fnMMIRequestParentRating)(void);///<CS_CA_MMI_RequestParentRating
    int (*m_fnMMIModifyParentRating)(DWORD dwPin, BYTE cParentRatingValue);///<CS_CA_MMI_ModifyParentRating
    int (*m_fnMMIRequestPairInfo)(void);///<CS_CA_MMI_RequestPairInfo
    int (*m_fnMMIPairing)(T_PairInfo* pstPairInfo);	///<CS_CA_Pairing
    BOOL (*m_fnMMIIsPinLock)(void);///<CS_CA_MMI_IsPinLock
    int (*m_fnMMILockPin)(void);///<CS_CA_MMI_LockPin
    BOOL (*m_fnMMIIsPinPass)(void);///<CS_CA_MMI_IsPinPass
    int (*m_fnMMIGetSmcStatuss)(E_SmsStatus* pcSmcStatus);	///<CS_CA_MMI_GetSmcStatus
    int (*m_fnStartBoardCast)(BYTE* pcPmtData, int nPmtLen, BYTE* pcCatData, int nCatLen, WORD wTsId, WORD wOrgNetId);	///<CS_CA_StartBoardCast
    int (*m_fnsetFakeFlag)(BYTE cFakeFlag);///<CS_CA_SetFakeFlag
    BOOL (*m_fnStopBoardCast)(void);///<CS_CA_StopBoardCast
    int (*m_fnMMIRequestAreaBlockInfo)(void);///<CS_CA_MMI_RequestAreaBlockInfo
    int (*m_fnSetLanguageMode)(E_LanguageType eLanguageType);	///<CS_CA_SetLanguageMode
    int (*m_fnProcAreaLockCatInfo)(BYTE *pcData, int nLen, BYTE cDemux);///<CS_CA_ProcAreaLockCatInfo
    int (*m_fnMMIGetMotherInfo)(void);///<CS_CA_MMI_GetMotherInfo
    int (*m_fnSetLockFrequency)(int nFre);///<CS_Shell_SetLockFrequency
    int (*m_fnGetLockFrequency)();	//CS_Shell_GetLockFrequency
    void (*m_fnCACASDebugMsgAdd)(BOOL bOpen);//CSCACASDebugMsgAdd
    int (*m_fnPDSDProc)(DWORD dwPDSD);///CS_CA_PDSDProc    
    int (*m_fnGetCASpecificInfo)(HCSHANDLE hService, CASpecificInfo_S* pstCaInfo);///< CSCAGetCASpecificInfo
    int (*m_fnConfirmFreePreview)(BOOL bConfirmed);///CS_CA_MMI_ConfirmFreePreview
    int (*m_fnGetSMCIDMode)();//获取卡号的提供方式。CSSHELL_DVTCA_SMCID_MODE_EXTERNAL为外部卡号，CSSHELL_DVTCA_SMCID_MODE_INTERNAL为内部卡号。不注册该函数默认为外部卡号
}CSShellDvtCaInitParam_S;

/**@brief DVT CA相关配置信息结构体*/
typedef struct
{
	DWORD m_dwCaAddress;	///<CA flash的首地址
	DWORD m_dwCaAddressBak;	///<CA flash备份的首地址
}CSShellDvtCaConfig_S;

/**
@brief 设置数码视讯CA相关操作参数给MMCP
@param[in] psDvtCa 数码视讯CA信息及相关操作结构体实例
@return 成功返回TRUE，失败返回FALSE
@note 必须在调用CSShellDvtCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellDvtCaSetParam(CSShellDvtCaGetDefaultParam());
@endcode
*/
BOOL CSShellDvtCaSetParam(const CSShellDvtCaInitParam_S* psDvtCa);
	
/**
@brief 获取数码视讯CA默认实现接口
@return 成功返回MMCP提供数码视讯CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得数码视讯CA的对象须使用csshell_dvtca_private.h中的CSShellDvtCaGetParam接口。
@see CSShellDvtCaGetParam
*/
const CSShellDvtCaInitParam_S* CSShellDvtCaGetDefaultParam(void);

/**
@brief 设置Dvt CA的相关配置信息
@param[in] psConfig Dvt CA相关配置信息的结构体实例
@note 该接口必须在CSShellDvtCaSetParam之后调用
@see CSShellDvtCaConfig_S
*/
void CSShellDvtCaSetConfig(CSShellDvtCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

