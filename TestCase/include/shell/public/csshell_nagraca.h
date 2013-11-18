/**@defgroup PublicShellNagraCA PublicShellNagraCA ����Nagra CA����ӿ�

@version 2.0.0 2010/01/24 ����
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
@brief NagraCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��nagra ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellNagraCaSetParam CSShellNagraCaGetDefaultParam*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellNagraCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_NAGRA
	
		CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���
	
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

/**@brief Nagra CA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD m_dwLib;///<���Nagra Lib���׵�ַ
	DWORD m_dwLibSize;///<���Nagra Lib�Ĵ�С
	DWORD m_dwPairing;///<���Pairing���׵�ַ
	DWORD m_dwPairingSize;///<���Pairing�Ĵ�С
	DWORD m_dwMail;///<���Nagra �ʼ����׵�ַ
	DWORD m_dwMailSize;///<���Nagra �ʼ��Ĵ�С
	DWORD m_dwEntitle;///<���Entitle���׵�ַ[HDPlus��Ŀ���У�������Ŀ���Բ�����]
	DWORD m_dwEntitleSize;///<���Entitle�Ĵ�С[HDPlus��Ŀ���У�������Ŀ���Բ�����]
}CSShellNagraCaConfig_S;

/**
@brief ����Nagra CA��ز����ӿڸ�MMCP
@param[in] Nagra CA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellNagraCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellNagraCaSetParam(CSShellNagraCaGetDefaulParam());
@endcode
*/
BOOL CSShellNagraCaSetParam(const CSShellNagraCaInitParam_S* psNagraCa);

	
/**
@brief ��ȡNagra CAĬ��ʵ�ֲ���
@return MMCP�ṩNagra CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���ͬ��3CA�Ķ�����ʹ��csshell_nagraca_private.h�е�CSShellNagraCaGetParam�ӿ�
@see CSShellNagraCaGetParam
*/
const CSShellNagraCaInitParam_S* CSShellNagraCaGetDefaultParam(void);

/**
@brief ����Nagra CA�����������Ϣ
@param[in] sConfig Nagra CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellNagraCaSetParam֮�����
@see CSShellNagraCaConfig_S
*/
void CSShellNagraCaSetConfig(CSShellNagraCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


