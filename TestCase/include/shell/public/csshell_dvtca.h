/**@defgroup PublicShellDVTCA PublicShellDVTCA ����������ѶCA����ӿ�

@version 2.0.0 2009/12/16 ����
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

#define CSSHELL_DVTCA_SMCID_MODE_EXTERNAL	0	//�ⲿ���š�������Ѷcaʹ�õĿ���Ĭ��Ϊ�ⲿ����
#define CSSHELL_DVTCA_SMCID_MODE_INTERNAL	1	//�ڲ�����

/**
@brief ������ѶCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��dvt ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellDvtCaSetParam CSShellDvtCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(ShellDvtCa_S)
    CSShellCaType_E         m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_DVT
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�	

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
    int (*m_fnGetSMCIDMode)();//��ȡ���ŵ��ṩ��ʽ��CSSHELL_DVTCA_SMCID_MODE_EXTERNALΪ�ⲿ���ţ�CSSHELL_DVTCA_SMCID_MODE_INTERNALΪ�ڲ����š���ע��ú���Ĭ��Ϊ�ⲿ����
}CSShellDvtCaInitParam_S;

/**@brief DVT CA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD m_dwCaAddress;	///<CA flash���׵�ַ
	DWORD m_dwCaAddressBak;	///<CA flash���ݵ��׵�ַ
}CSShellDvtCaConfig_S;

/**
@brief ����������ѶCA��ز���������MMCP
@param[in] psDvtCa ������ѶCA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note �����ڵ���CSShellDvtCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellDvtCaSetParam(CSShellDvtCaGetDefaultParam());
@endcode
*/
BOOL CSShellDvtCaSetParam(const CSShellDvtCaInitParam_S* psDvtCa);
	
/**
@brief ��ȡ������ѶCAĬ��ʵ�ֽӿ�
@return �ɹ�����MMCP�ṩ������ѶCA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���������ѶCA�Ķ�����ʹ��csshell_dvtca_private.h�е�CSShellDvtCaGetParam�ӿڡ�
@see CSShellDvtCaGetParam
*/
const CSShellDvtCaInitParam_S* CSShellDvtCaGetDefaultParam(void);

/**
@brief ����Dvt CA�����������Ϣ
@param[in] psConfig Dvt CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellDvtCaSetParam֮�����
@see CSShellDvtCaConfig_S
*/
void CSShellDvtCaSetConfig(CSShellDvtCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

