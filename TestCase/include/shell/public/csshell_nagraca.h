/**@defgroup PublicShellNagraCA PublicShellNagraCA 定义Nagra CA对外接口

@version 2.0.0 2010/01/24 初稿
@{
*/
#ifndef CSSHELL_NAGRACA_H
#define CSSHELL_NAGRACA_H

#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,please check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "CS_NAGRA_Interface.h"

/**
@brief NagraCA信息及相关操作结构体
@note 目前mmcp提供了nagra ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellNagraCaSetParam CSShellNagraCaGetDefaultParam*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellNagraCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_NAGRA
	
		CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号
	
		INT (*m_requestSystemInfo)(void); ///<CS_CA_RequestSystemInfo
		INT (*m_fnrequestSmartCardInfo)(void);///<CS_CA_RequestSmartCardInfo	
		INT (*m_fgetZoneId)(BYTE *pZoneId, INT nZoneIdLen);///<CS_CA_GetZoneId	
		void (*m_fsetCheckValidateIrd)(BOOL bSet);///<CSCASetCheckValidateIrd	
		INT (*m_fcheckPinCode)(E_CaPinIndex emPinIndex, DWORD dwPinCode);///<CSCACheckPinCode
		INT (*m_fchangePinCode)(E_CaPinIndex emPinIndex, DWORD dwOldPinCode, DWORD dwNewPinCode);///<CSCAChangePinCode
		BOOL (*m_fgetHotlineNumber)(BYTE *pucNumber, INT nLen);///<CSCAHDPlusGetHotlineNumber	
		BOOL (*m_fgetCallCharge)(BYTE *pucCharge, INT nLen);///<CSCAHDPlusGetCallCharge
		void (*m_fsaveEntitlementMessageToFlash)(void);///<CSCAHDPlusSaveEntitlementMessageToFlash
		INT (*m_fnaddMailImp)(NAGRA_CAMAIL_ST* pstTxt);///<CS_CA_TXT_Add	
		INT (*m_fndeleteMail)(DWORD dwIndex);///<CS_CA_TXT_Del		
		INT (*m_fndeleteAllMail)(void);///<CS_CA_TXT_DelAll
		INT (*m_fngetMailNumber)(void);///<CS_CA_TXT_GetNum	
		INT (*m_fngetMailInfoByIndex)(DWORD dwIndex, NAGRA_CAMAIL_ST** ppstTxt);///<CS_CA_TXT_GetInfo
		BOOL (*m_fnhaveNew)(NAGRA_TXTTYPE_ST eType);///<CS_CA_TXT_HaveNew	
		INT (*m_fngetNewMailNum)(void);///<CS_CA_TXT_HaveNewMailNum		
		INT (*m_fnsaveMailtoFlash)(void);///<CS_CA_TXT_Save	
		INT (*m_fnsetReadFlag)(DWORD dwIndex, BOOL bRead);	///<CS_CA_TXT_SetReadFlag
		INT (*m_fneraserAll)();///<CS_CA_TXT_EraserAll		
}CSShellNagraCaInitParam_S;

/**@brief Nagra CA相关配置信息结构体*/
typedef struct
{
	DWORD m_dwLib;///<存放Nagra Lib的首地址
	DWORD m_dwLibSize;///<存放Nagra Lib的大小
	DWORD m_dwPairing;///<存放Pairing的首地址
	DWORD m_dwPairingSize;///<存放Pairing的大小
	DWORD m_dwMail;///<存放Nagra 邮件的首地址
	DWORD m_dwMailSize;///<存放Nagra 邮件的大小
	DWORD m_dwEntitle;///<存放Entitle的首地址[HDPlus项目特有，其他项目可以不分配]
	DWORD m_dwEntitleSize;///<存放Entitle的大小[HDPlus项目特有，其他项目可以不分配]
}CSShellNagraCaConfig_S;

/**
@brief 设置Nagra CA相关操作接口给MMCP
@param[in] Nagra CA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellNagraCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellNagraCaSetParam(CSShellNagraCaGetDefaulParam());
@endcode
*/
BOOL CSShellNagraCaSetParam(const CSShellNagraCaInitParam_S* psNagraCa);

	
/**
@brief 获取Nagra CA默认实现参数
@return MMCP提供Nagra CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得同方3CA的对象须使用csshell_nagraca_private.h中的CSShellNagraCaGetParam接口
@see CSShellNagraCaGetParam
*/
const CSShellNagraCaInitParam_S* CSShellNagraCaGetDefaultParam(void);

/**
@brief 设置Nagra CA的相关配置信息
@param[in] sConfig Nagra CA相关配置信息的结构体实例
@note 该接口必须在CSShellNagraCaSetParam之后调用
@see CSShellNagraCaConfig_S
*/
void CSShellNagraCaSetConfig(CSShellNagraCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


