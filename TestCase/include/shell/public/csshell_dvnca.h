/**@defgroup PublicShellDVNCA PublicShellDVTCA �������CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_DVNCA_H
#define CSSHELL_DVNCA_H


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
#include "thirdparty/ca/dvn/CS_DVNCA_Interface.h"


typedef int (*DvnCaCallback)(DWORD nUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);

/**
@brief ���CA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��dvn ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellDvnCaSetParam CSShellDvnCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellDvnCaInitParam_S)
    CSShellCaType_E         m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_DVN
    CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�	

    int (* m_fnGetVer)(BYTE *pucVer, DWORD nLen);///<CS_CA_GetVersion
    int (* m_fnGetCoreVer)(BYTE *pucVer, DWORD nLen);///<CS_CA_GetCoreVersion
    void (* m_fnSetDvnConfig)(int nMode);///<CSCASetDvnConfiguration
    int (* m_fnPerformPairing)(void);///<CS_CA_PerformPairing
    int (* m_fnGetInfoSTBAttribute)(BYTE *pucStbIDBuf, int nBufSize);///<CS_CA_GetInfo_STBAttribute
    int (* m_fnGetInfoSmcID)(BYTE *pucSmcIDBuf, int nBufSize);///<CS_CA_GetInfo_SmcID
    int (* m_fnGetInfoAccountNo)(BYTE *pucAccountNoBuf, int nBufSize);///<CS_CA_GetInfo_AccountNo
    int (* m_fnGetInfo_SysDescriptor)(int nSysDescriptorIndex, BYTE *pucSysDescriptorBuf, int nBufSize);///<CS_CA_GetInfo_SysDescriptor
    int (* m_fnTokenGetValue)(float *pfTokenValue);///<CS_CA_TokenGetValue
    int (* m_fnTokenConfirmChange)(BOOL bFlag, int nOperationID);///<CS_CA_TokenConfirmChange
    int (* m_fnTokenDeduct)(float fTokenValue, int nServiceType, int nOperationID);///<CS_CA_TokenDeduct
    int (* m_fnGetSubscriptionRecordNum)(int *pnLocalNum, int *pnCenterNum);///<CS_CA_GetSubscriptionRecordNum
    int (* m_fnGetSubscriptionRecordContent)(int nRecordIndex, int nSubRecordType, T_CaFtaSubscriptionService* pstSubRecord);///<CS_CA_GetSubscriptionRecordContent
    int (* m_fnGetTransactionRecordNum)(void);///<CS_CA_GetTransactionRecordNum
    int (* m_fnGetTransactionRecordContent)(int nIndex, T_CaFtaTransactionRecord *pstTracRecord);///<CS_CA_GetTransactionRecordContent
    int (* m_fnTxtAdd)(T_CaMail *pstTxt);///<CS_CA_TXT_Add
    int (* m_fnTxtDel)(DWORD dwIndex);///<CS_CA_TXT_Del
    int (* m_fnTxtDelAll)(void);///<CS_CA_TXT_DelAll
    int (* m_fnTxtGetNum)(void);///<CS_CA_TXT_GetNum
    int (* m_fnTxtGetInfo)(DWORD dwIndex, T_CaMail *ppstTxt);///<CS_CA_TXT_GetInfo
    int (* m_fnTxtHaveNew)(T_TxtType eType);///<CS_CA_TXT_HaveNew
    int (* m_fnTxtHaveNewMailNum)(void);///<CS_CA_TXT_HaveNewMailNum
    int (* m_fnTxtSave)(void);///<CS_CA_TXT_Save
    int (* m_fnTxtSetReadFlag)(DWORD dwIndex, BOOL bRead);///<CS_CA_TXT_SetReadFlag
    int (* m_fnTxtSetLockFalg)(DWORD dwIndex, BOOL bLockFlag);///<CS_CA_TXT_SetLockFalg
    int (* m_fnTxtGetLockFalg)(DWORD dwIndex, BOOL*pbLockFlag);///<CS_CA_TXT_GetLockFalg
    int (* m_fnGetSmcStatus)(BYTE *pucSmcStatus);///<CS_CA_GetSmcStatus
    int (* m_fnGetRating)(DWORD *pdwPGLevel);///<CS_CA_GetRating
    int (* m_fnSetRating)(DWORD dwPGLevel);///<CS_CA_SetRating
    int (* m_fnGetSmartcardVersion)(BYTE *pucVersion);///<CSCAGetSmartcardVersion
    int (* m_fnSetAreaBlock)(T_FtaCountryList *pstCountypList);///<CS_CA_SetAreaBlock
    int (* m_fnRegisterCallback)(DvnCaCallback fnCb); //ע��:�˺�������֧��ע�����ص�
    int (* m_fnGetPairedStatus)(BOOL *pbPaired); ///<CS_CA_GetPairedStatus
}CSShellDvnCaInitParam_S;


/**@brief Dvn CA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD m_dwFlashAddress;	///<CA flash���׵�ַ
	DWORD m_dwBakFlashAddress;	///<CA flash���ݵ��׵�ַ
}CSShellDvnCaConfig_S;


/**
@brief �������CA��ز���������MMCP
@param[in] psDvnCa ���CA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note �����ڵ���CSShellDvnCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellDvnCaSetParam(CSShellDvnCaGetDefaultParam());
@endcode
*/
BOOL CSShellDvnCaSetParam(const CSShellDvnCaInitParam_S* psDvnCa);
	
/**
@brief ��ȡ���CAĬ��ʵ�ֽӿ�
@return �ɹ�����MMCP�ṩ���CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ������CA�Ķ�����ʹ��csshell_dvnca_private.h�е�CSShellDvnCaGetParam�ӿڡ�
@see CSShellDvnCaGetParam
*/
const CSShellDvnCaInitParam_S* CSShellDvnCaGetDefaultParam(void);

/**
@brief ����Dvn CA�����������Ϣ
@param[in] psConfig Dvn CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellDvnCaSetParam֮�����
@see CSShellDvnCaConfig_S
*/
void CSShellDvnCaSetConfig(CSShellDvnCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

