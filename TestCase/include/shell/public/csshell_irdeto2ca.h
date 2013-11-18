/**@defgroup PublicShellIrdeto2CA PublicShellIrdeto2CA ����Irdeto2.26 CA����ӿ�

@version 2.0.0 2010/4/12 ����
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
@brief IRDETOCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��irdeto2.26 ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellIrdetoCaSetParam CSShellIrdetoCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellIrdeto2CaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_IRDETO2
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���

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

/**@brief Irdeto3 CA���������Ϣ�ṹ��*/
typedef struct
{
	T_CaFtaType m_eFtaType;
	DWORD m_dwFlashAddress; ///<Flash�ĵ�ַ
}CSShellIrdeto2CaConfig_S;
/**
@brief ���ð��ϵ�2.26CA��ز����ӿڸ�MMCP
@param[in] Irdeto2.26 CA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellIrdeto2CaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellIrdeto2CaSetParam(CSShellIrdeto2CaGetDefaulParam());
@endcode
*/
BOOL CSShellIrdeto2CaSetParam(const CSShellIrdeto2CaInitParam_S* psIrdeto2Ca);

	
/**
@brief ��ȡIrdeto2.26 CAĬ��ʵ�ֲ���
@return MMCP�ṩIrdeto2.26 CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���Irdeto2.26 CA�Ķ�����ʹ��csshell_irdeto2ca_private.h�е�CSShellIrdeto2CaGetParam�ӿ�
@see CSShellIrdeto3CaGetParam
*/
const CSShellIrdeto2CaInitParam_S* CSShellIrdeto2CaGetDefaultParam(void);

/**
@brief ����Irdeto2.26 CA�����������Ϣ
@param[in] sConfig Irdeto2.26 CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellIrdeto2CaSetParam֮�����
@see CSShellIrdeto2CaConfig_S
*/
void CSShellIrdeto2CaSetConfig(CSShellIrdeto2CaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


