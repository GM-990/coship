
#ifndef CSSHELL_UDRMCA_H
#define CSSHELL_UDRMCA_H


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
#include "cs_udrm_interface.h"

typedef int (*FOnUdrmCaCallback)(DWORD nUserData,int nMsg,DWORD dwPara1,DWORD dwPara2);

/**
@brief UDRMCA信息及相关操作结构体
@note 目前mmcp提供了udrm ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellUDRMCaSetParam CSShellUDRMCaGetDefaulParam
*/
typedef struct
{
	int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellUDRMCaInitParam_S)
	CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_UDRM
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号

	int (* m_fnGetMailNum)(int *pnTotalEmailNum, int *pnNoReadEmailNum);
	int (* m_fnGetMailByIndex)(int nIndex, CaMail_S *pstMailInfo);
	int (* m_fnGetMailByEmailId)(DWORD wEmailId, CaMail_S *pstMailInfo);
	int (* m_fnSetMailReadedFlag)(DWORD nMailId, BOOL bReadFlag);
	int (* m_fnDelMail)(DWORD nMailId);
	int (* m_fnDelMailAll)(void);
	int (* m_fnGetCaInfo)(CaInfo_S *pstCaInfo);
	int (* m_fnGetOperatorNumber)(DWORD *pdwOperatorNum);
	int (* m_fnGetSectorNumber)(DWORD dwOperatorIndex, DWORD *pdwSectorNum);
	int (* m_fnGetProductNumber)(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwProductNum);
	int (* m_fnGetProductInfo)(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD dwProductIndex, CaProductInfo_S* pstProductInfo);
	int (* m_fnGetIPPVTNumber)(DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD *pdwIPPVTNum);
	int (* m_fnGetIPPVTInfo)(DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD dwIPPVTIndex, CaIPPVTInfo_S* pstIPPVTInfo);
	int (* m_fnOrderIPPVT)(BOOL bOrder,DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD dwEventId);
	int (* m_fnGetViewLevel)(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwLevel);
	int (* m_fnSetViewLevel)(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD dwLevel, BYTE *paucPin);
	int (* m_fnCheckPin)(DWORD dwOperatorIndex, DWORD dwSectorIndex, BYTE *paucPin);
	int (* m_fnSetPin)(DWORD dwOperatorIndex, DWORD dwSectorIndex, BYTE *paucOldPin, BYTE *paucNewPin);
	int (* m_fnGetWalletInfo)(DWORD dwOperatorIndex, DWORD dwSectorIndex, CaWalletInfo_S *pstWalletInfo);
	void (* m_fnRegistCallback)(FOnUdrmCaCallback fnCb);
	int (* m_fnGetCaOTAInfo)(CaOtaInfo_S * sOTAInfo);
	int (* m_fnGetUDRMBuildInformation)(BYTE *paucBuff, int nBuffSize);
	int (*m_fnGetUDRMAreaValue)(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwAreaValue);
	int (*m_fnGetSNFromTestMode)(CHAR* pacSN, int* pnLen);
}CSShellUDRMCaInitParam_S;


/**
@brief 设置NDS CA相关操作接口给MMCP
@param[in] pNDS3Ca NDS CA信息及相关操作结构体实例
@return 成功返回TRUE,否则返回FALSE
@note CSShellNdsCaSetParam要在CSShellNdsCaSetConfig之前调用
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellNdsCaSetParam(CSShellNdsCaGetDefaultParam());
@codeend
*/
BOOL CSShellUdrmCaSetParam(const CSShellUDRMCaInitParam_S* psNdsCa);
	
/**
@brief 获取NDS CA默认实现接口
@return 成功返回MMCP提供NDS CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得NDS CA的对象须使用csshell_ndsca_private.h中的CSShellNdsCaGetParam接口
@see CSShellNdsCaGetParam
*/
const CSShellUDRMCaInitParam_S* CSShellUdrmCaGetDefaultParam(void);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


