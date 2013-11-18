
#ifndef CSSHELL_ComvenientCA_H
#define CSSHELL_ComvenientCA_H


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
#include "thirdparty/ca/comvenient/CS_COMVENIENT_Interface.h"
#include "CSUSPDMCommonType.h"

/**
@brief ComvenientCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��Comvenient ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellComvenientCaSetParam CSShellComvenientCaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellComvenientCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_Comvenient
    CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���
    INT (*m_fnGetCaBoxId)(BYTE* pcBoxId,INT *pnLength);  //CSGetCaBoxId  //�˴�����CAͷ�ļ��䶯��������󣬸�ͷ�ļ�����һ��
    void (*m_fnGetEpgTime)(CaDateTime_S* pstDataTime);  //CSCAGetEpgTime
    INT  (*m_fnChangeCardPin)(BYTE ucOldPinLength,BYTE* pcOldPin,BYTE ucNewPinLength,BYTE* pcNewPin,CaPinIndex_E ePinIndex);  //CS_CA_ChangePin
    INT  (*m_fnCheckCardPin)(BYTE  ucPinLength, BYTE* pcPin,CaPinInfo_S sPinInfo);   //CS_CA_CheckCardPin
    INT  (*m_fnGetCardNumber)(BYTE* pCSmcNo, WORD wLen);  //CS_CA_GetCardNumber
    INT  (*m_fnGetPurseInfo)(void);   //CS_CA_GetPurseInfo
    WORD  (*m_fnGetCaSystemId)(void);   //CA_INF_GetCaSystemId
    void  (*m_fnGetBCLibVersion)(BYTE* pacVersion, BYTE* pacDate, BYTE* pacTime) ;    //CS_CA_GetBCLibVersion
    INT  (*m_fnGetParingState)(void);   //CA_CA_GetParingState
    INT (*m_fnAddMail)(CaMail_S* pstTxt);  //CS_CA_TXT_Add
    INT (*m_fnDelMail)(DWORD dwIndex);  //CS_CA_TXT_Del
    INT (*m_fnDelMailAll)(void);  //CS_CA_TXT_DelAll
    INT (*m_fnGetEmailNum)(void); //CS_CA_TXT_GetNum
    INT (*m_fngetMailInfoByIndex)(DWORD dwIndex, CaMail_S** ppstTxt);  //CS_CA_TXT_GetInfo
    BOOL (*m_fnhaveNew)(void);   //CS_CA_TXT_HaveNew
    INT (*m_fngetNewMailNum)(void);  //CS_CA_TXT_HaveNewMailNum
    INT (*m_fnsaveMailtoFlash)(void);   //CS_CA_TXT_Save
    INT (*m_fnsetReadFlag)(DWORD dwIndex, BOOL bRead);  //CS_CA_TXT_SetReadFlag
    BOOL (*m_fnGetIntersectionDeliverListByDM)(Deliver* pDeliverFromCA, int nDeliverCACount, Deliver *pDeliverRet, int* nDeliverRetCount);  //Ƶ���� 
}CSShellComvenientCaInitParam_S;

/**@brief ComvenientCA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD  m_dwCaMailAddress;///<�ʼ����׵�ַ	
	DWORD  m_dwCaFlashAddress;	
	DWORD  m_dwCaBackupFlashAddress;	
}CSShellComvenientCaConfig_S;


/**
@brief ����ComvenientCA��ز����ӿڸ�MMCP
@param[in] ComvenientCA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellComvenientCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellComvenientCaSetParam(CSShellComvenientCaGetDefaulParam());
@code end
*/
BOOL CSShellComvenientCaSetParam(const CSShellComvenientCaInitParam_S* psComvenientCa);

	
/**
@brief ��ȡComvenient CAĬ��ʵ�ֲ���
@return MMCP�ṩComvenient CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���ͬ��3CA�Ķ�����ʹ��csshell_Comvenientca_private.h�е�CSShellComvenientCaGetParam�ӿ�
@see CSShellComvenientCaGetParam
*/
const CSShellComvenientCaInitParam_S* CSShellComvenientCaGetDefaulParam(void);

/**
@brief ����Comvenient CA�����������Ϣ
@param[in] psConfig Comvenient CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellComvenientCaSetParam֮�����
@see CSShellComvenientCaConfig_S
*/
void CSShellComvenientCaSetConfig(CSShellComvenientCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


