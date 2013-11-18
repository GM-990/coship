/**@defgroup PublicShellTRCA PublicShellTRCA 定义滕锐 CA对外接口

@version  初稿 2013-3-1
@{
*/
#ifndef CSSHELL_TRCA_H
#define CSSHELL_TRCA_H


#ifdef CSSHELL_CA_INCLUDED
//@error Error! more then one CA Shell file included in the same file,pls check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "CS_TRCA_Interface.h"
#include "cs_cainterface.h"
/**
@brief TRCA信息及相关操作结构体
@note 目前mmcp提供了TR ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellTRCaSetParam CSShellTRCaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellTRCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_TR
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号

	INT (*m_fnGetVersion)(BYTE* pacVerBuf,  DWORD dwBufSize); ///<CS_CA_GetVersion 获取CA版本信息
    INT (*m_fnGetSmartCardVersion)(BYTE* pucVerBuf,  BYTE ucBufSize);///<CSCAGetSmartCardVersion 获取智能卡版本信息
    INT (*m_fnDelOneTxtMsg)(TRCaMsgType_E eTxtMsgType, WORD wTxtMsgIndex); ///<CSCADelOneTxtMsg
    INT (*m_fnDelAllTxtMsg)(TRCaMsgType_E eTxtMsgType); ///<CSCADelAllTxtMsg
    INT (*m_fnGetTxtMsgInfo)(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, TRCaTxtMsg_S *psTxtMsg); ///<CSCAGetTxtMsgInfo
    INT (*m_fnGetTxtMsgNum)(TRCaMsgType_E eTxtMsgType, BYTE *pucTotalNum, BYTE *pucEmptyNum);  ///<CSCAGetTxtMsgNum
    INT (*m_fnSetTxtMsgReadFlag)(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, BOOL bRead); ///<CSCASetTxtMsgReadFlag
    INT (*m_fnGetTxtMsgReadFlag)(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, BOOL *pbRead); ///<CSCAGetTxtMsgReadFlag
    INT (*m_fnHaveNewTxtMsgNum)(TRCaMsgType_E eTxtMsgType, BYTE *pucNotReadNum); ///<CSCAHaveNewTxtMsgNum
    INT (*m_fnGetEntitleTotalNum)(WORD *pwTotalNum); ///<CSCAGetEntitleTotalNum
    INT (*m_fnGetEntitleInfo)(WORD wIndex, TREntitleInfo_S *psEntitleInfo); ///<CSCAGetEntitleInfo
    INT (*m_fnGetRating)(BYTE *pucRating); ///<CSCAGetRating
    INT (*m_fnSetRating)(BYTE* pucPin, BYTE ucPinLen, BYTE ucRating);///<CSCASetRating
    INT (*m_fnOpenParentalControl)(BYTE* pucPin, BYTE ucPinLen); ///<CSCAOpenParentalControl
    INT (*m_fnChangePin)(BYTE* pucOldPin, BYTE* pucNewPin, BYTE ucPinLen);///<CSCAChangePin
    INT (*m_fnCSCAGetSmartCardID)(char* pcCardId, BYTE ucCardIdBuff); ///<CSCAGetSmartCardID
    TRCaReadMasterRet_E (*m_fnReadDataFromMaster)(void);///<CSCAReadDataFromMaster
    TRCaWriteSlaverRet_E (*m_fnWriteDataToSlaver)(void);///CSCAWriteDataToSlaver
 
}CSShellTRCaInitParam_S;

/**@brief 滕锐CA相关配置信息结构体*/
typedef struct
{
	DWORD  m_dwMailAddress;///<邮件的首地址
	DWORD  m_dwMailAddressBak;///<邮件备份的首地址
}CSShellTRCaConfig_S;
/**
@brief 设置滕锐CA相关操作接口给MMCP
@param[in] TRCA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellTRCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellTRCaSetParam(CSShellTRCaGetDefaulParam());
@endcode
*/
BOOL CSShellTRCaSetParam(const CSShellTRCaInitParam_S* psTRCa);

	
/**
@brief 获取滕锐CA默认实现参数
@return MMCP提供滕锐CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得滕锐CA的对象须使用csshell_TRca_private.h中的CSShellTRCaGetParam接口
@see CSShellTRCaGetParam
*/
const CSShellTRCaInitParam_S* CSShellTRCaGetDefaulParam(void);

/**
@brief 设置滕锐CA的相关配置信息
@param[in] psConfig 滕锐CA相关配置信息的结构体实例
@note 该接口必须在CSShellTRCaSetParam之后调用
@see CSShellTRCaConfig_S
*/
void CSShellTRCaSetConfig(CSShellTRCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


