/**@defgroup PublicShellNDSCA PublicShellNDSCA 定义NDS CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_NDSCA_H
#define CSSHELL_NDSCA_H



#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,pls check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "CS_NDSCA_Interface.h"
#include "udi2_ota.h"
#include "udi2_error.h"

/**
@brief NDS CA信息及相关操作结构体
@note 目前mmcp提供了nds ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellNdsCaSetParam CSShellNdsCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellNdsCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_NDS
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口

    int (*m_fnGetCoreVer)(char* pcVer, int* pnLen);	///< CS_CA_GetCoreVer
    int (*m_fnGetSmcInfo)(); ///< CS_CA_MMI_RequestSmcInfo
    int (*m_fnGetParentRating) (DWORD dwPin); ///< CS_CA_MMI_RequestParentRating
    int (*m_fnGetCurrentLanguage)(void); ///< CS_CA_MMI_RequestCurLanguage
    int (*m_fnGetPinLockStatus)(void); ///< CS_CA_MMI_RequestLockStatus
    int (*m_fnGetCaDiagData)(void); ///< CS_CA_MMI_RequestCaDiagData
    int (*m_fnGetClearMode)(void); ///< CS_CA_GetClearMode
    int (*m_fnGetBouquetID)(DWORD* pdwBouquetId); ///< CS_CA_MMI_RequestBouquetId
    int (*m_fnGetIdentifyNum)(DWORD *pdwNum);  ///< CSCAGetIrdSerialNum
    int (*m_fnGetStbLabelInfo)(T_CaStbLabelInfo* pstCaStbLabelInfo); ///< CSCAGetCaStbLabel
    int (*m_fnGetCaSpecificInfo)(HCSHANDLE hService, CASpecificInfo_S* pstCaInfo); ///< CSCAGetCASpecificInfo
    int (*m_fnChangeParentRating)(DWORD dwPin, BYTE cParentRatingValue); ///< CS_CA_MMI_ModifyParentRating
    int (*m_fnChangeStandbyStatus)(BYTE cStandbyStaus); ///< CS_CA_MMI_ModifyStandbyStatus
    int (*m_fnChangeCurrentLanguage)(E_CaLanguage eLan); ///< CS_CA_MMI_ModifyCurLanguage
    DWORD (*m_fnCancelOSD)(void); ///< CS_CA_MsgCancelOsd
    int (*m_fnGetMailCount)(void); ///< CS_CA_TXT_GetNum
    int (*m_fnGetNewMailCout)(void); ///< CS_CA_TXT_HaveNewMailNum
    int (*m_fnGetMailInfo)(int nIndex, T_CaMail* pstTxt); ///< CS_CA_TXT_GetInfo
    int (*m_fnAddMail)(T_CaMail *pstTxt); ///< CS_CA_TXT_Add
    INT (*m_fnDeleteMail)(INT index);	///< CS_CA_TXT_Del
    int (*m_fnDeleteAllMail)(void); ///< CS_CA_TXT_DelAll
    int (*m_fnSaveMail)(void); ///< CS_CA_TXT_Save
    int (*m_fnSetMailReadFlag)(int index, BOOL bFlag); ///< CS_CA_TXT_SetReadFlag
   	int (*m_fnGetCardID)(char* pacCardIdStr, int* pnLen);	//CS_CA_GetCardIdFormateString
   	int (*m_fnSetCustomerId)(DWORD dwCustomerId );	//CS_CA_SetCustomerId
  	int (*m_fnModifyPinCode)(DWORD dwOldPin, DWORD dwNewPin);// CS_CA_MMI_ModifyPinCode
  	CSUDI_Error_Code (*m_fnGetOTAInfo)(CSUDIOTAInfoType_E eType, void* value, int length);///<OTA信息读取成功返回0，否则返回非0;m_fnGetOTAInfo默认为NULL,
}CSShellNdsCaInitParam_S;

/**@brief NDS CA相关配置信息结构体*/
typedef struct
{
	DWORD m_dwEmail; ///<
	DWORD m_dwCAEEProm; ///<
	DWORD m_dwBCA; ///<
	DWORD m_dwFlashHead;	///<
	DWORD m_dwCustomerID;///<
}CSShellNdsCaConfig_S;

/**
@brief 设置NDS CA相关操作接口给MMCP
@param[in] pNDS3Ca NDS CA信息及相关操作结构体实例
@return 成功返回TRUE,否则返回FALSE
@note CSShellNdsCaSetParam要在CSShellNdsCaSetConfig之前调用
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellNdsCaSetParam(CSShellNdsCaGetDefaultParam());
@endcode
*/
BOOL CSShellNdsCaSetParam(const CSShellNdsCaInitParam_S* psNdsCa);
	
/**
@brief 获取NDS CA默认实现接口
@return 成功返回MMCP提供NDS CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得NDS CA的对象须使用csshell_ndsca_private.h中的CSShellNdsCaGetParam接口
@see CSShellNdsCaGetParam
*/
const CSShellNdsCaInitParam_S* CSShellNdsCaGetDefaultParam(void);

/**
@brief 设置NDS CA的相关配置信息
@param[in] psConfig NDS CA相关配置信息的结构体实例
@note 该接口必须在CSShellNdsCaSetParam之后调用
@see CSShellStCaConfig_S
*/
void CSShellNdsCaSetConfig(CSShellNdsCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

