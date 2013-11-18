
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
@brief UDRMCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��udrm ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellUDRMCaSetParam CSShellUDRMCaGetDefaulParam
*/
typedef struct
{
	int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellUDRMCaInitParam_S)
	CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_UDRM
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���

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
@brief ����NDS CA��ز����ӿڸ�MMCP
@param[in] pNDS3Ca NDS CA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,���򷵻�FALSE
@note CSShellNdsCaSetParamҪ��CSShellNdsCaSetConfig֮ǰ����
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellNdsCaSetParam(CSShellNdsCaGetDefaultParam());
@codeend
*/
BOOL CSShellUdrmCaSetParam(const CSShellUDRMCaInitParam_S* psNdsCa);
	
/**
@brief ��ȡNDS CAĬ��ʵ�ֽӿ�
@return �ɹ�����MMCP�ṩNDS CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���NDS CA�Ķ�����ʹ��csshell_ndsca_private.h�е�CSShellNdsCaGetParam�ӿ�
@see CSShellNdsCaGetParam
*/
const CSShellUDRMCaInitParam_S* CSShellUdrmCaGetDefaultParam(void);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


