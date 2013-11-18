/**@defgroup PublicShellTF3CA PublicShellTF3CA 定义同方3 CA对外接口

@version 2.0.0 2009/12/16 初稿
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
@brief TF3CA信息及相关操作结构体
@note 目前mmcp提供了tf3 ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellTf3CaSetParam CSShellTf3CaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellTf3CaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_TF3
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号

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

/**@brief 同方3CA相关配置信息结构体*/
typedef struct
{
	DWORD  m_dwMailAddress;///<邮件的首地址
	DWORD  m_dwMailAddressBak;///<邮件备份的首地址
}CSShellTf3CaConfig_S;
/**
@brief 设置同方3CA相关操作接口给MMCP
@param[in] TF3CA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellTf3CaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellTf3CaSetParam(CSShellTf3CaGetDefaulParam());
@endcode
*/
BOOL CSShellTf3CaSetParam(const CSShellTf3CaInitParam_S* psTf3Ca);

	
/**
@brief 获取同方3CA默认实现参数
@return MMCP提供同方3CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得同方3CA的对象须使用csshell_tf3ca_private.h中的CSShellTf3CaGetParam接口
@see CSShellTf3CaGetParam
*/
const CSShellTf3CaInitParam_S* CSShellTf3CaGetDefaulParam(void);

/**
@brief 设置同方3CA的相关配置信息
@param[in] psConfig 同方3CA相关配置信息的结构体实例
@note 该接口必须在CSShellTf3CaSetParam之后调用
@see CSShellTf3CaConfig_S
*/
void CSShellTf3CaSetConfig(CSShellTf3CaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


