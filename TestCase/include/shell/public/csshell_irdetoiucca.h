/**@defgroup PublicShellIrdeto3CA PublicShellIrdeto3CA ����Irdeto3 CA����ӿ�

@version 2.0.0 2009/12/16 ����
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
@brief IRDETO3CA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��irdeto3 ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellIrdetoIucCaSetParam CSShellIrdeto3IucCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellIrdetoIucCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_IRDETOIUC
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���
	
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

/**@brief IrdetoIuc CA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD m_dwNVMAddr;///<���˽�����ݵ�Flash�׵�ַ
	DWORD m_dwNVMSize;///<���˽�����ݵ�Flash�Ĵ�С
	DWORD m_dwBackupNVMAddr;///<���˽�����ݵ�Flash�׵�ַ
	DWORD m_dwBackupNVMSize; ///<����ʼ���Flash��С
	DWORD m_dwFlashMailAddr; ///<����ʼ���Flash�׵�ַ
	DWORD m_dwFlashMailSize; ///<����ʼ���Flash��С
}CSShellIrdetoIucCaConfig_S;

/**
@brief ���ð��ϵ�IUC CA��ز����ӿڸ�MMCP
@param[in] Irdeto Iuc CA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellIrdetoIucCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellIrdetoIucCaSetParam(CSShellIrdetoIucCaGetDefaulParam());
@codeend
*/
BOOL CSShellIrdetoIucCaSetParam(const CSShellIrdetoIucCaInitParam_S* psIrdetoIucCa);

	
/**
@brief ��ȡIrdetoIUC CAĬ��ʵ�ֲ���
@return MMCP�ṩIrdetoIUC CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���ͬ��3CA�Ķ�����ʹ��csshell_irdetoiucca_private.h�е�CSShellIrdetoIucCaGetParam�ӿ�
@see CSShellIrdetoIucCaGetParam
*/
const CSShellIrdetoIucCaInitParam_S* CSShellIrdetoIucCaGetDefaultParam(void);

/**
@brief ����Irdeto3 CA�����������Ϣ
@param[in] sConfig Irdeto iuc CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellIrdetoIucCaSetParam֮�����
@see CSShellIrdetoIucCaSetConfig
*/
void CSShellIrdetoIucCaSetConfig(CSShellIrdetoIucCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif
