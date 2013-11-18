/**@defgroup PublicShellIrdeto3CA PublicShellIrdeto3CA 定义Irdeto3 CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_IRDETO3CA_H
#define CSSHELL_IRDETO3CA_H

#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,please check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "thirdparty/ca/irdeto3/CS_IRDETOCA_Interface.h"

/**
@brief IRDETO3CA信息及相关操作结构体
@note 目前mmcp提供了irdeto3 ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellIrdeto3CaSetParam CSShellIrdeto3CaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellIrdeto3CaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_IRDETO3
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号
	
	INT (*m_fngetVersion)(BYTE* pacVerBuf, DWORD dwBufSize);///<CS_CA_GetVersion
	INT (*m_fnenterMenu)(IRDETO_MENU_STATUS_EN enumQueryDataType, DWORD dwParam);///<CS_CA_EnterIrdetoMenu
	INT (*m_fnexitMenu)(IRDETO_MENU_STATUS_EN enumQueryDataType);///<CS_CA_ExitIrdetoMenu
	DWORD (*m_fneraseAllFlash)();///<CS_CA_FAKE_NVM_EraseAllFlash
	INT (*m_fnpurchaseIppv)(IppvPurchaseInfo_S* pstIppvInfo);	///<CSCAPurchaseIppv
	INT (*m_fngetIppvSectorInfo)(DWORD dwResourceId, BYTE bSector);///<CSCAGetIppvSectorInfo
	INT (*m_fnsetIppvThreshold)(IppvThresholdInfo_S* pstIppvThrInfo);///<CSCASetIppvThreshold
	INT (*m_fngetIppvEventList)(DWORD dwResourceId, BYTE bSector, BYTE bDaysGrace);	///<CSCAGetIppvEventList
	INT (*m_fnsetSOLFlag)(BOOL bFlag);	///<CS_CA_SetSOLFlag
	INT (*m_fnaddMailImpl)(IRDETO_CAMAIL_ST* pstTxt);///<CS_CA_TXT_Add
	INT (*m_fndeleteMail)(DWORD dwIndex);///<CS_CA_TXT_Del
	INT (*m_fndeleteAllMail)(void);///<CS_CA_TXT_DelAll
	INT (*m_fngetMailNumber)(void);///<CS_CA_TXT_GetNum
	INT (*m_fngetMailInfoByIndex)(DWORD dwIndex, IRDETO_CAMAIL_ST** ppstTxt);///<CS_CA_TXT_GetInfo
	INT (*m_fnhaveNew)(IRDETO_TXTTYPE_ST eType);///<CS_CA_TXT_HaveNew
	INT (*m_fngetNewMailNum)(void);///<CS_CA_TXT_HaveNewMailNum
	INT (*m_fnsaveMailtoFlash)(void);///<CS_CA_TXT_Save
	INT (*m_fnsetReadFlag)(DWORD dwIndex, BOOL bRead);	///<CS_CA_TXT_SetReadFlag
	INT (*m_fnsetLockFlag)(DWORD dwIndex, BOOL bLockFlag);///<CS_CA_TXT_SetLockFalg
	INT (*m_fngetLockFlag)(DWORD dwIndex, BOOL* pbLockFlag);///<CS_CA_TXT_GetLockFalg  
	INT (*m_fngetCompLevelShutType)(DWORD dwGroupHandle);///<CSCAGetCompLevelShutType  
	INT (*m_fngetIppvSrvProviderName)(BYTE* pacNameBuf, DWORD dwBufSize);
	INT (*m_fnGetMailTitle)(DWORD dwIndex, BYTE* pcTitle, DWORD dwBufSize);
	
}CSShellIrdeto3CaInitParam_S;

/**@brief Irdeto3 CA相关配置信息结构体*/
typedef struct
{
	DWORD m_dwNVMAddr;///<存放私有数据的Flash首地址
	DWORD m_dwNVMSize;///<存放私有数据的Flash的大小
	DWORD m_dwFlashMailAddr; ///<存放邮件的Flash首地址
	DWORD m_dwFlashMailSize; ///<存放邮件的Flash大小
}CSShellIrdeto3CaConfig_S;

/**
@brief 设置爱迪德3CA相关操作接口给MMCP
@param[in] Irdeto3 CA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellIrdeto3CaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellIrdeto3CaSetParam(CSShellIrdeto3CaGetDefaulParam());
@endcode
*/
BOOL CSShellIrdeto3CaSetParam(const CSShellIrdeto3CaInitParam_S* psIrdeto3Ca);

	
/**
@brief 获取Irdeto3 CA默认实现参数
@return MMCP提供Irdeto3 CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得同方3CA的对象须使用csshell_irdeto3ca_private.h中的CSShellIrdeto3CaGetParam接口
@see CSShellIrdeto3CaGetParam
*/
const CSShellIrdeto3CaInitParam_S* CSShellIrdeto3CaGetDefaultParam(void);

/**
@brief 设置Irdeto3 CA的相关配置信息
@param[in] sConfig Irdeto3 CA相关配置信息的结构体实例
@note 该接口必须在CSShellIrdeto3CaSetParam之后调用
@see CSShellIrdeto3CaConfig_S
*/
void CSShellIrdeto3CaSetConfig(CSShellIrdeto3CaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


