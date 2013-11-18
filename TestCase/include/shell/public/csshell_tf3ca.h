/**@defgroup PublicShellTF3CA PublicShellTF3CA ����ͬ��3 CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_TF3CA_H
#define CSSHELL_TF3CA_H


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
#include "thirdparty/ca/tongfang3/CS_TFCA_Interface.h"
/**
@brief TF3CA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��tf3 ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellTf3CaSetParam CSShellTf3CaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellTf3CaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_TF3
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���

    int (*m_fnRestart)(void);	///<CS_CA_Restart
    int (*m_fnGetTFLibVersion)(BYTE* pacVerBuf,  DWORD dwBufSize);///<CS_CA_GetTFLibVersion
    WORD (*m_fnIsPinLocked)(BYTE* pbPinLock);///<CS_CA_IsPinLocked
    WORD (*m_fnChangePin)(DWORD dwOldPin, DWORD dwNewPin);///<CS_CA_ChangePin
    WORD (*m_fnGetParentRating)(BYTE* pucRating);///<CS_CA_GetRating
    WORD (*m_fnSetParentRating)(DWORD dwPinCode, BYTE ucRating);///<CS_CA_SetRating
    WORD (*m_fnGetWorkTime)(T_WorkingTime* pstStartTime, T_WorkingTime* pstEndTime);///<CS_CA_GetWorkTime
    WORD (*m_fnSetWorkTime)(DWORD dwPinCode,T_WorkingTime* pstStartTime, T_WorkingTime* pstEndTime);///<CS_CA_SetWorkTime
    WORD (*m_fnGetOperatorIds)(T_CaOperatorId* pstOperatorsId);///<CS_CA_GetOperatorIds
    WORD (*m_fnGetOperatorInfo)(WORD wOperatorsId, T_CaOperatorInfo* pstOperatorInfo);///<CS_CA_GetOperatorInfo
    WORD (*m_fnGetACList)(WORD wOperatorsId, T_CaOperatorAcList* pstAcList);///<CS_CA_GetACList
    WORD (*m_fnGetServiceEntitles)(WORD wOperatorsId, T_CaServiceEntitles* pstServiceEntitles);///<CS_CA_GetServiceEntitles     
    WORD (*m_fnGetSlotIDs)(WORD wOperatorsId, T_CaSlotId* pstSlotId);///<CS_CA_GetSlotIDs     
    WORD (*m_fnGetSlotInfo)(WORD wOperatorsId, BYTE bySlotID, T_CaSlotInfo* pstSlotInfo);///<CS_CA_GetSlotInfo
    WORD (*m_fnGetIPPVProgram)(WORD wOperatorsId, BYTE bySlotID, T_CaIppvProgInfo* pstIppvInfo);///<CS_CA_GetIPPVProgram
    WORD (*m_fnIPPVBuyConfirm)(BYTE cBuyFlag,WORD wEcmPid, DWORD dwPinCode, T_CaIppvPrice* pstPrice);///<CS_CA_IPPVBuyConfirm
    WORD (*m_fnDeleteIPPVProgram)(WORD wOperatorId, BYTE wSlotId, WORD	wProductId, WORD wExpireDate);///<CS_CA_DelIPPVProgram   
    void (*m_fnChangeTFDate)(WORD wdate, T_CaDate *pstDate);///<CS_CA_ChangeTFDate
    int (*m_fnGetEmailTotalNum)(BYTE* pcEmailNum, BYTE* pcEmptyNum); ///< CS_CA_GetEmailTotalNum
    WORD (*m_fnGetEmailTitle)(int nIndex, T_CaEmailHead *pstEmail);///<CS_CA_GetEmailTitle
    WORD (*m_fnGetEmailContent)(int nIndex, T_CaEmailContent* pstEmailContent);///<CS_CA_GetEmailContent
    void (*m_fnDeleteMail)(int index); ///<CS_CA_DelEmail
    void (*m_fnSortEmail)(T_CaEmailSortStyle eSortStyle);///<CS_CA_SortEmail    
    int (*m_fnStartStockPlay)(T_CaFtaService* pstServiceInfo);///<CS_CA_StartStockPlay  
    int (*m_fnStopStockPlay)(T_CaFtaService* pstServiceInfo);///<CS_CA_StopStockPlay
    WORD (*m_fnGetDetitleInfo)(WORD wOperatorsId,T_DetitleInfo* pstDetileInfo);///<CS_CA_GetDetitleInfo
    int (*m_fnDeleteDetitleInfo)(WORD wOperatorsId,DWORD dwDetileNum);///<CS_CA_DelDetitleInfo
    int (*m_fnHaveNewDetitle)(void);///<CS_CA_HaveNewDetitle  
    WORD (*m_fnGetEntitleInfo)(WORD wOperatorsId,T_EntitleInfo* pstEntileInfo);///<CS_CA_GetEntitleInfo     
    WORD (*m_fnGetPairCardInfo)(WORD wOperatorsId,T_CaChildInfo* pstChildInfo);///<CS_CA_GetPairCardInfo   
    WORD (*m_fnSetParentPair)(WORD wOperatorsId, BOOL bIsChild, E_PairStatus* eStatus);///<CS_CA_SetParentPair
    void (*m_fnClearParentPairStatus)(void);///<CS_CA_ClearParentPairStatus
    DWORD (*m_fnGetFlashAddress)(DWORD dwSize);///<CS_CA_GetFlashAddress
    DWORD (*m_fnGetBackupFlashAddress)(DWORD dwSize);///<CS_CA_GetBackupFlashAddress
    int (*m_fnGetSmartCardId)(char* pcCardId);///<CS_CA_GetSCCardID
    WORD (*m_fnIsPaired)(T_PairList * pstPairList);///<CS_CA_IsPaired
    int (*m_fnGetPlatFormID)(WORD* pwPlatformID);  
    WORD (*m_fnGetRegionCode)();
    BOOL (*m_fnGetSendName)(char *pucSendName, int* pnLen);
}CSShellTf3CaInitParam_S;

/**@brief ͬ��3CA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD  m_dwMailAddress;///<�ʼ����׵�ַ
	DWORD  m_dwMailAddressBak;///<�ʼ����ݵ��׵�ַ
}CSShellTf3CaConfig_S;
/**
@brief ����ͬ��3CA��ز����ӿڸ�MMCP
@param[in] TF3CA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellTf3CaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellTf3CaSetParam(CSShellTf3CaGetDefaulParam());
@endcode
*/
BOOL CSShellTf3CaSetParam(const CSShellTf3CaInitParam_S* psTf3Ca);

	
/**
@brief ��ȡͬ��3CAĬ��ʵ�ֲ���
@return MMCP�ṩͬ��3CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���ͬ��3CA�Ķ�����ʹ��csshell_tf3ca_private.h�е�CSShellTf3CaGetParam�ӿ�
@see CSShellTf3CaGetParam
*/
const CSShellTf3CaInitParam_S* CSShellTf3CaGetDefaulParam(void);

/**
@brief ����ͬ��3CA�����������Ϣ
@param[in] psConfig ͬ��3CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellTf3CaSetParam֮�����
@see CSShellTf3CaConfig_S
*/
void CSShellTf3CaSetConfig(CSShellTf3CaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


