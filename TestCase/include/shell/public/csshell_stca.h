/**@defgroup PublicShellSTCA PublicShellSTCA ������ͨ CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_STCA_H
#define CSSHELL_STCA_H

#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,pls check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "csshell_ca.h"

#include "mmcp_typedef.h"
#include "thirdparty/ca/suantong/CS_SUANTONGCA_Interface.h"

/**
@brief ��ͨCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��stca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellStCaSetParam CSShellStCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellStCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_SUANTONG
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�

    int (*m_fnGetCoreVersion)(char* pcVer, int* pnLen);///<CS_CA_GetCoreVer
    int (*m_fnCheckPinCode)(DWORD dwPin);///<CS_CA_MMI_CheckPinCode
    int (*m_fnModifyPinCode)(DWORD dwOldPin, DWORD dwNewPin);///<CS_CA_MMI_ModifyPinCode
    int (*m_fnPpvConfirm)(DWORD dwIppvNo, DWORD dwPin, BOOL bIsPpvp);///<CS_CA_MMI_PpvConfirm
    int (*m_fnPpvCancel)(DWORD dwIppvNo, DWORD dwPin);///<CS_CA_MMI_PpvCancel
    int (*m_fnRequestSmcInfo)();///<CS_CA_MMI_RequestSmcInfo
    int (*m_fnRequestPPIDInfo)();///<CS_CA_MMI_RequestPPIDInfo
    int (*m_fnRequestDetailPPIDInfo)(DWORD dwPin, DWORD dwPpId);///<CS_CA_MMI_RequestDetailPPIDInfo
    int (*m_fnRequestRecordSrvAllowedFlag)();///<CS_CA_MMI_RequestRecordSrvAllowedFlag
    int (*m_fnModifyParentRating)(DWORD dwPin, BYTE cParentRatingValue);///<CS_CA_MMI_ModifyParentRating
    int (*m_fnRequestCurrentParentLevel)();///<CS_CA_MMI_RequestCurrentParentLevel
    int (*m_fnGetCurrentProgramNo)();///<CS_CA_GetCurrentProgramNo
    int (*m_fnCheckIsNewPPV)(T_PpvInfo * pstOldPPV, T_PpvInfo * pstNewPPV);///<CS_CA_MMI_CheckIsNewPPV
    int (*m_fnSetUserViewOpImpl)(CaUserView_S stUserView,DWORD dwPin);///<CS_CA_MMI_UserViewOp
    void (*m_fnSetFactoryDefault)(void);///<CSCAFactoryDefault
    int (*m_fnGetTSAntiMoveCtrlCode)(BYTE* pcAntiMoveCtrl);///<CSCAGetTSAntiMoveCtrlCode
    int (*m_fnAddMailImpl)(T_CaMail* pstTxt);///<CS_CA_TXT_Add
    int (*m_fnDeleteMail)(DWORD dwIndex);///<CS_CA_TXT_Del
    int (*m_fnDeleteAllMail)(void);///<CS_CA_TXT_DelAll
    int (*m_fnGetMailNumber)(void);///<CS_CA_TXT_GetNum
    int (*m_fnGetMailInfoByIndex)(int nIndex, T_CaMail* pstTxt);///<CS_CA_TXT_GetInfo
    int (*m_fnSaveFlash)(void);///<CS_CA_TXT_Save
    int (*m_fnSetReadFlag)(int nIndex, BOOL bRead);///<CS_CA_TXT_SetReadFlag
    int (*m_fnGetNoReadMailNumber)(void);///<CS_CA_TXT_HaveNewMailNum
    int (*m_fnSetRepeatCheck)(int isCheck);///<CS_CA_TXT_SetRepeatCheck
}CSShellStCaInitParam_S;

/**@brief ��ͨCA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD  m_dwFlsAddress;///<Flash���׵�ַ
	DWORD  m_dwCaMailAddress;///<�ʼ����׵�ַ
}CSShellStCaConfig_S;

/**
@brief ������ͨCA��ز���������MMCP
@param[in] pStCa ��ͨCA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellStCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellStCaSetParam(CSShellStCaGetDefaultParam());
@endcode
*/
BOOL CSShellStCaSetParam(const CSShellStCaInitParam_S* psStCa);

/**
@brief ��ȡ��ͨCAĬ��ʵ�ֽӿڵĲ�����Ϣ
@return �ɹ�����Ĭ�ϵĲ�����Ϣ��ʧ�ܷ���NULL
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ�����ͨCA�Ķ�����ʹ��csshell_stca_private.h�е�CSShellTf3CaGetParam�ӿ�
@see CSShellStCaGetParam
*/
const CSShellStCaInitParam_S* CSShellStCaGetDefaultParam(void);

/**
@brief ������ͨCA�����������Ϣ
@param[in] psConfig ��ͨCA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellStCaSetParam֮�����
@see CSShellStCaConfig_S
*/
void CSShellStCaSetConfig(CSShellStCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

