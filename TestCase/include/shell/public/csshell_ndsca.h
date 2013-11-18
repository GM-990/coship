/**@defgroup PublicShellNDSCA PublicShellNDSCA ����NDS CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_NDSCA_H
#define CSSHELL_NDSCA_H



#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,pls check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "CS_NDSCA_Interface.h"
#include "udi2_ota.h"
#include "udi2_error.h"

/**
@brief NDS CA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��nds ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellNdsCaSetParam CSShellNdsCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellNdsCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_NDS
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�

    int (*m_fnGetCoreVer)(char* pcVer, int* pnLen);	///< CS_CA_GetCoreVer
    int (*m_fnGetSmcInfo)(); ///< CS_CA_MMI_RequestSmcInfo
    int (*m_fnGetParentRating) (DWORD dwPin); ///< CS_CA_MMI_RequestParentRating
    int (*m_fnGetCurrentLanguage)(void); ///< CS_CA_MMI_RequestCurLanguage
    int (*m_fnGetPinLockStatus)(void); ///< CS_CA_MMI_RequestLockStatus
    int (*m_fnGetCaDiagData)(void); ///< CS_CA_MMI_RequestCaDiagData
    int (*m_fnGetClearMode)(void); ///< CS_CA_GetClearMode
    int (*m_fnGetBouquetID)(DWORD* pdwBouquetId); ///< CS_CA_MMI_RequestBouquetId
    int (*m_fnGetIdentifyNum)(DWORD *pdwNum);  ///< CSCAGetIrdSerialNum
    int (*m_fnGetStbLabelInfo)(T_CaStbLabelInfo* pstCaStbLabelInfo); ///< CSCAGetCaStbLabel
    int (*m_fnGetCaSpecificInfo)(HCSHANDLE hService, CASpecificInfo_S* pstCaInfo); ///< CSCAGetCASpecificInfo
    int (*m_fnChangeParentRating)(DWORD dwPin, BYTE cParentRatingValue); ///< CS_CA_MMI_ModifyParentRating
    int (*m_fnChangeStandbyStatus)(BYTE cStandbyStaus); ///< CS_CA_MMI_ModifyStandbyStatus
    int (*m_fnChangeCurrentLanguage)(E_CaLanguage eLan); ///< CS_CA_MMI_ModifyCurLanguage
    DWORD (*m_fnCancelOSD)(void); ///< CS_CA_MsgCancelOsd
    int (*m_fnGetMailCount)(void); ///< CS_CA_TXT_GetNum
    int (*m_fnGetNewMailCout)(void); ///< CS_CA_TXT_HaveNewMailNum
    int (*m_fnGetMailInfo)(int nIndex, T_CaMail* pstTxt); ///< CS_CA_TXT_GetInfo
    int (*m_fnAddMail)(T_CaMail *pstTxt); ///< CS_CA_TXT_Add
    INT (*m_fnDeleteMail)(INT index);	///< CS_CA_TXT_Del
    int (*m_fnDeleteAllMail)(void); ///< CS_CA_TXT_DelAll
    int (*m_fnSaveMail)(void); ///< CS_CA_TXT_Save
    int (*m_fnSetMailReadFlag)(int index, BOOL bFlag); ///< CS_CA_TXT_SetReadFlag
   	int (*m_fnGetCardID)(char* pacCardIdStr, int* pnLen);	//CS_CA_GetCardIdFormateString
   	int (*m_fnSetCustomerId)(DWORD dwCustomerId );	//CS_CA_SetCustomerId
  	int (*m_fnModifyPinCode)(DWORD dwOldPin, DWORD dwNewPin);// CS_CA_MMI_ModifyPinCode
  	CSUDI_Error_Code (*m_fnGetOTAInfo)(CSUDIOTAInfoType_E eType, void* value, int length);///<OTA��Ϣ��ȡ�ɹ�����0�����򷵻ط�0;m_fnGetOTAInfoĬ��ΪNULL,
}CSShellNdsCaInitParam_S;

/**@brief NDS CA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD m_dwEmail; ///<
	DWORD m_dwCAEEProm; ///<
	DWORD m_dwBCA; ///<
	DWORD m_dwFlashHead;	///<
	DWORD m_dwCustomerID;///<
}CSShellNdsCaConfig_S;

/**
@brief ����NDS CA��ز����ӿڸ�MMCP
@param[in] pNDS3Ca NDS CA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,���򷵻�FALSE
@note CSShellNdsCaSetParamҪ��CSShellNdsCaSetConfig֮ǰ����
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellNdsCaSetParam(CSShellNdsCaGetDefaultParam());
@endcode
*/
BOOL CSShellNdsCaSetParam(const CSShellNdsCaInitParam_S* psNdsCa);
	
/**
@brief ��ȡNDS CAĬ��ʵ�ֽӿ�
@return �ɹ�����MMCP�ṩNDS CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���NDS CA�Ķ�����ʹ��csshell_ndsca_private.h�е�CSShellNdsCaGetParam�ӿ�
@see CSShellNdsCaGetParam
*/
const CSShellNdsCaInitParam_S* CSShellNdsCaGetDefaultParam(void);

/**
@brief ����NDS CA�����������Ϣ
@param[in] psConfig NDS CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellNdsCaSetParam֮�����
@see CSShellStCaConfig_S
*/
void CSShellNdsCaSetConfig(CSShellNdsCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

