/**@defgroup PublicShellIrdeto2CA PublicShellIrdeto2CA 定义Irdeto2.26 CA对外接口

@version 2.0.0 2010/4/12 初稿
@{
*/
#ifndef CSSHELL_IRDETO2CA_H
#define CSSHELL_IRDETO2CA_H

#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,please check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "thirdparty/ca/irdeto/CS_IRDETOCA_Interface.h"

/**
@brief IRDETOCA信息及相关操作结构体
@note 目前mmcp提供了irdeto2.26 ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellIrdetoCaSetParam CSShellIrdetoCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellIrdeto2CaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_IRDETO2
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号

	INT (*m_fnPvrStartSrvRec)(int nServiceID,int nVideoPID,int nAudioPID,BYTE cDemux) ;///< CS_CA_PVR_StartSrvRecord
	INT (*m_fnPvrStopSrvRec)(int nServiceID,int nVideoPID,int nAudioPID,BYTE cDemux) ; ///< CS_CA_PVR_StopSrvRecord
	INT (*m_fnPvrRegisterKeyCallBack)(T_CBPvrKey hCBPvrKey); ///< CS_CA_PVR_RegisterKeyCallback
	INT (*m_fnPvrStartEsPlay)(WORD wPid, BYTE cDemux) ; ///< CS_CA_PVR_StartPvrEsPlay
	INT (*m_fnPvrStopEsPlay)(WORD wPid, BYTE cDemux);///< CS_CA_PVR_StopPvrEsPlay
	INT (*m_fnPvrSetEvtKey)(WORD wPid, BYTE cDemux, BYTE acKey[8]) ;///< CS_CA_PVR_SetEvenKey
	INT (*m_fnPvrSetOddKey)(WORD wPid, BYTE cDemux, BYTE acKey[8]);///< CS_CA_PVR_SetOddKey
	INT (*m_fnGetCaStatus)(void); ///< CS_CA_GetCaStatus
	INT (*m_fnStopCaGetStatus)(void); ///< CS_CA_StopGetCaStatus
	INT (*m_fnGetLoaderStatus)(void);///< CS_CA_GetLoaderStatus
	INT (*m_fnGetIrdStatus)(void); ///< CS_CA_GetIrdStatus'
	INT (*m_fnAddMailImpl)(T_CaMail* pstTxt); /// < CS_CA_TXT_Add
	INT (*m_fnDeleteMail)(DWORD dwIndex);///<CS_CA_TXT_Del
	INT (*m_fnDeleteAllMail)(void);///<CS_CA_TXT_DelAll
	INT (*m_fnGetMailNumber)(void);///<CS_CA_TXT_GetNum
	INT (*m_fnGetMailInfoByIndex)(DWORD dwIndex, T_CaMail** ppstTxt);///<CS_CA_TXT_GetInfo
	INT (*m_fnHaveNew)(T_TxtType eType);///<CS_CA_TXT_HaveNew
	INT (*m_fnGetNewMailNum)(void);///< CS_CA_TXT_HaveNewMailNum
	INT (*m_fnSaveMailtoFlash)(void);///<CS_CA_TXT_Save
	INT (*m_fnSetReadFlag)(DWORD dwIndex, BOOL bRead);	///<CS_CA_TXT_SetReadFlag
	INT (*m_fnSetLockFlag)(DWORD dwIndex, BOOL bLockFlag);///<CS_CA_TXT_SetLockFalg
	INT (*m_fnGetLockFlag)(DWORD dwIndex, BOOL* pbLockFlag);///<CS_CA_TXT_GetLockFalg  
}CSShellIrdeto2CaInitParam_S;

/**@brief Irdeto3 CA相关配置信息结构体*/
typedef struct
{
	T_CaFtaType m_eFtaType;
	DWORD m_dwFlashAddress; ///<Flash的地址
}CSShellIrdeto2CaConfig_S;
/**
@brief 设置爱迪德2.26CA相关操作接口给MMCP
@param[in] Irdeto2.26 CA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellIrdeto2CaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellIrdeto2CaSetParam(CSShellIrdeto2CaGetDefaulParam());
@endcode
*/
BOOL CSShellIrdeto2CaSetParam(const CSShellIrdeto2CaInitParam_S* psIrdeto2Ca);

	
/**
@brief 获取Irdeto2.26 CA默认实现参数
@return MMCP提供Irdeto2.26 CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得Irdeto2.26 CA的对象须使用csshell_irdeto2ca_private.h中的CSShellIrdeto2CaGetParam接口
@see CSShellIrdeto3CaGetParam
*/
const CSShellIrdeto2CaInitParam_S* CSShellIrdeto2CaGetDefaultParam(void);

/**
@brief 设置Irdeto2.26 CA的相关配置信息
@param[in] sConfig Irdeto2.26 CA相关配置信息的结构体实例
@note 该接口必须在CSShellIrdeto2CaSetParam之后调用
@see CSShellIrdeto2CaConfig_S
*/
void CSShellIrdeto2CaSetConfig(CSShellIrdeto2CaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


