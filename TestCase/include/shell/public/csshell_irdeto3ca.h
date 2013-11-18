/**@defgroup PublicShellIrdeto3CA PublicShellIrdeto3CA ����Irdeto3 CA����ӿ�

@version 2.0.0 2009/12/16 ����
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
@brief IRDETO3CA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��irdeto3 ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellIrdeto3CaSetParam CSShellIrdeto3CaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellIrdeto3CaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_IRDETO3
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���
	
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

/**@brief Irdeto3 CA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD m_dwNVMAddr;///<���˽�����ݵ�Flash�׵�ַ
	DWORD m_dwNVMSize;///<���˽�����ݵ�Flash�Ĵ�С
	DWORD m_dwFlashMailAddr; ///<����ʼ���Flash�׵�ַ
	DWORD m_dwFlashMailSize; ///<����ʼ���Flash��С
}CSShellIrdeto3CaConfig_S;

/**
@brief ���ð��ϵ�3CA��ز����ӿڸ�MMCP
@param[in] Irdeto3 CA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellIrdeto3CaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellIrdeto3CaSetParam(CSShellIrdeto3CaGetDefaulParam());
@endcode
*/
BOOL CSShellIrdeto3CaSetParam(const CSShellIrdeto3CaInitParam_S* psIrdeto3Ca);

	
/**
@brief ��ȡIrdeto3 CAĬ��ʵ�ֲ���
@return MMCP�ṩIrdeto3 CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���ͬ��3CA�Ķ�����ʹ��csshell_irdeto3ca_private.h�е�CSShellIrdeto3CaGetParam�ӿ�
@see CSShellIrdeto3CaGetParam
*/
const CSShellIrdeto3CaInitParam_S* CSShellIrdeto3CaGetDefaultParam(void);

/**
@brief ����Irdeto3 CA�����������Ϣ
@param[in] sConfig Irdeto3 CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellIrdeto3CaSetParam֮�����
@see CSShellIrdeto3CaConfig_S
*/
void CSShellIrdeto3CaSetConfig(CSShellIrdeto3CaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


