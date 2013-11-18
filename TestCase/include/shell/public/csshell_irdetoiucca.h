/**@defgroup PublicShellIrdeto3CA PublicShellIrdeto3CA 定义Irdeto3 CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_IRDETOIUCCA_H
#define CSSHELL_IRDETOIUCCA_H

#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,please check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "thirdparty/ca/irdetoiuc/csiucinterface.h"

/**
@brief IRDETO3CA信息及相关操作结构体
@note 目前mmcp提供了irdeto3 ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellIrdetoIucCaSetParam CSShellIrdeto3IucCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellIrdetoIucCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_IRDETOIUC
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号
	
	INT (*m_fngetVersion)(BYTE* pacVerBuf, DWORD dwBufSize);///<CS_CA_GetVersion
	INT (*m_fnenterMenu)( CaStatusMenu_E enumQueryDataType);///<CSCAEnterIrdetoMenu
	INT (*m_fnexitMenu)(CaStatusMenu_E enumQueryDataType);///<CSCAExitIrdetoMenu
	DWORD (*m_fneraseAllFlash)();///<CS_CA_FAKE_NVM_EraseAllFlash
	INT (*m_fnsetSOLFlag)(BOOL bFlag);	///<CS_CA_SetSOLFlag
	INT (*m_fnaddMailImpl)(CaMail_S* pstTxt);///<CS_CA_TXT_Add
	INT (*m_fndeleteMail)(DWORD dwIndex);///<CS_CA_TXT_Del
	INT (*m_fndeleteAllMail)(void);///<CS_CA_TXT_DelAll
	INT (*m_fngetMailNumber)(void);///<CS_CA_TXT_GetNum
	INT (*m_fngetMailInfoByIndex)(DWORD dwIndex, CaMail_S** ppstTxt);///<CS_CA_TXT_GetInfo
	INT (*m_fnhaveNew)(void);///<CS_CA_TXT_HaveNew
	INT (*m_fngetNewMailNum)(void);///<CS_CA_TXT_HaveNewMailNum
	INT (*m_fnsaveMailtoFlash)(void);///<CS_CA_TXT_Save
	INT (*m_fnsetReadFlag)(DWORD dwIndex, BOOL bRead);	///<CS_CA_TXT_SetReadFlag
	INT (*m_fnsetLockFlag)(DWORD dwIndex, BOOL bLockFlag);///<CS_CA_TXT_SetLockFalg
	INT (*m_fngetLockFlag)(DWORD dwIndex, BOOL* pbLockFlag);///<CS_CA_TXT_GetLockFalg  
}CSShellIrdetoIucCaInitParam_S;

/**@brief IrdetoIuc CA相关配置信息结构体*/
typedef struct
{
	DWORD m_dwNVMAddr;///<存放私有数据的Flash首地址
	DWORD m_dwNVMSize;///<存放私有数据的Flash的大小
	DWORD m_dwBackupNVMAddr;///<存放私有数据的Flash首地址
	DWORD m_dwBackupNVMSize; ///<存放邮件的Flash大小
	DWORD m_dwFlashMailAddr; ///<存放邮件的Flash首地址
	DWORD m_dwFlashMailSize; ///<存放邮件的Flash大小
}CSShellIrdetoIucCaConfig_S;

/**
@brief 设置爱迪德IUC CA相关操作接口给MMCP
@param[in] Irdeto Iuc CA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellIrdetoIucCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellIrdetoIucCaSetParam(CSShellIrdetoIucCaGetDefaulParam());
@codeend
*/
BOOL CSShellIrdetoIucCaSetParam(const CSShellIrdetoIucCaInitParam_S* psIrdetoIucCa);

	
/**
@brief 获取IrdetoIUC CA默认实现参数
@return MMCP提供IrdetoIUC CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得同方3CA的对象须使用csshell_irdetoiucca_private.h中的CSShellIrdetoIucCaGetParam接口
@see CSShellIrdetoIucCaGetParam
*/
const CSShellIrdetoIucCaInitParam_S* CSShellIrdetoIucCaGetDefaultParam(void);

/**
@brief 设置Irdeto3 CA的相关配置信息
@param[in] sConfig Irdeto iuc CA相关配置信息的结构体实例
@note 该接口必须在CSShellIrdetoIucCaSetParam之后调用
@see CSShellIrdetoIucCaSetConfig
*/
void CSShellIrdetoIucCaSetConfig(CSShellIrdetoIucCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif
