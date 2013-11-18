/**@defgroup PublicShellSTCA PublicShellSTCA 定义算通 CA对外接口

@version 2.0.0 2009/12/16 初稿
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
@brief 算通CA信息及相关操作结构体
@note 目前mmcp提供了stca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellStCaSetParam CSShellStCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellStCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_SUANTONG
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口

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

/**@brief 算通CA相关配置信息结构体*/
typedef struct
{
	DWORD  m_dwFlsAddress;///<Flash的首地址
	DWORD  m_dwCaMailAddress;///<邮件的首地址
}CSShellStCaConfig_S;

/**
@brief 设置算通CA相关操作参数给MMCP
@param[in] pStCa 算通CA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellStCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellStCaSetParam(CSShellStCaGetDefaultParam());
@endcode
*/
BOOL CSShellStCaSetParam(const CSShellStCaInitParam_S* psStCa);

/**
@brief 获取算通CA默认实现接口的参数信息
@return 成功返回默认的参数信息，失败返回NULL
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP不得直接使用本接口，若MMCP需要获得算通CA的对象须使用csshell_stca_private.h中的CSShellTf3CaGetParam接口
@see CSShellStCaGetParam
*/
const CSShellStCaInitParam_S* CSShellStCaGetDefaultParam(void);

/**
@brief 设置算通CA的相关配置信息
@param[in] psConfig 算通CA相关配置信息的结构体实例
@note 该接口必须在CSShellStCaSetParam之后调用
@see CSShellStCaConfig_S
*/
void CSShellStCaSetConfig(CSShellStCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

