/**@defgroup PublicShellDVNCA PublicShellDVTCA 定义天柏CA对外接口

@version 2.0.0 2009/12/16 初稿
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
@brief 天柏CA信息及相关操作结构体
@note 目前mmcp提供了dvn ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellDvnCaSetParam CSShellDvnCaGetDefaultParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellDvnCaInitParam_S)
    CSShellCaType_E         m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_DVN
    CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口	

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
    int (* m_fnRegisterCallback)(DvnCaCallback fnCb); //注意:此函数必须支持注册多个回调
    int (* m_fnGetPairedStatus)(BOOL *pbPaired); ///<CS_CA_GetPairedStatus
}CSShellDvnCaInitParam_S;


/**@brief Dvn CA相关配置信息结构体*/
typedef struct
{
	DWORD m_dwFlashAddress;	///<CA flash的首地址
	DWORD m_dwBakFlashAddress;	///<CA flash备份的首地址
}CSShellDvnCaConfig_S;


/**
@brief 设置天柏CA相关操作参数给MMCP
@param[in] psDvnCa 天柏CA信息及相关操作结构体实例
@return 成功返回TRUE，失败返回FALSE
@note 必须在调用CSShellDvnCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellDvnCaSetParam(CSShellDvnCaGetDefaultParam());
@endcode
*/
BOOL CSShellDvnCaSetParam(const CSShellDvnCaInitParam_S* psDvnCa);
	
/**
@brief 获取天柏CA默认实现接口
@return 成功返回MMCP提供天柏CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得天柏CA的对象须使用csshell_dvnca_private.h中的CSShellDvnCaGetParam接口。
@see CSShellDvnCaGetParam
*/
const CSShellDvnCaInitParam_S* CSShellDvnCaGetDefaultParam(void);

/**
@brief 设置Dvn CA的相关配置信息
@param[in] psConfig Dvn CA相关配置信息的结构体实例
@note 该接口必须在CSShellDvnCaSetParam之后调用
@see CSShellDvnCaConfig_S
*/
void CSShellDvnCaSetConfig(CSShellDvnCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

