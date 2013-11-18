
#ifndef CSSHELL_conaxCA_H
#define CSSHELL_conaxCA_H


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
#include "thirdparty/ca/conax/CS_CONAX_Interface.h"
/**
@brief conaxCA信息及相关操作结构体
@note 目前mmcp提供了conax ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellConaxCaSetParam CSShellConaxCaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellConaxCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_Conax
	
    CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号
    INT (*m_fnSaveMailtoFlash)(void);///<CS_CA_EmailSaveFlash
    INT (*m_fnGetEmailNum)(INT* pnTotalNum    , INT* pnUnreadNum);//CSCAEmailGetNum	
    INT (*m_fnDelMail)(DWORD dwIndex/* start with 0*/);//CS_CA_EmailDel
    INT (*m_fnDelMailAll)(void);//CS_CA_EmailDelAll
    INT (*m_fnGetMailInfo)(DWORD dwIndex/* start with 0*/, T_CaEmail *pstMail);//CS_CA_EmailGetInfo
    INT (*m_fnSetMailReadFlag)(DWORD dwIndex/* start with 0*/);//CS_CA_EmailSetReadflag
    INT (*m_fnGetSubscriptionTotalNum)(void);//CS_CA_GetSubscriptionTotalNum
    INT (*m_fnGetSubscriptionInfo)(DWORD dwRecordIndex, T_CaSubscriptionInfo *pstSubRecord);//CS_CA_GetSubscriptionInfo
    INT (*m_fnGetPpvEventTotalNum)(void);//CS_CA_GetPpvEventTotalNum
    INT (*m_fnGetPpvEventInfo)(DWORD dwRecordIndex, T_CaPPVEventInfo* pstPpvRecord);//CS_CA_GetPpvEventInfo
    INT (*m_fnGetPurseTotalNum)(void);//CS_CA_GetPurseTotalNum
    INT (*m_fnGetPurseInfo)(DWORD dwPurseIndex,T_CaPurseInfo* pstCaPurseInfo);//CS_CA_GetPurseInfo
    INT (*m_fnGetDebitInfo)(DWORD dwPurseIndex,T_CaDebitInfo** pstCaDebitInfo);//CS_CA_GetDebitInfo
    INT (*m_fnGetCreditInfo)(DWORD dwPurseIndex,T_CaCreditInfo** pstCaCreditInfo);//CS_CA_GetCreditInfo
    INT (*m_fnGetConaxInfo)(T_CaConaxInfo *pstConaxInfo);//CS_CA_GetConaxInfo
    INT (*m_fnChangeCardPin)(BYTE* pSzPinOld, BYTE *pSzPinNew);//CS_CA_ChangeCardPin
    INT (*m_fnGetCardMaturityRating)(T_CaMaturityRating *pMat);//CS_CA_GetCardMaturityRating
    INT (*m_fnChangeCardMaturityRating)(T_CaMaturityRating eMatRat,BYTE *pSzPin);//CS_CA_ChangeCardMaturityRating
    INT (*m_fnAcceptMaturityRat)( BYTE *pSzPin );//CS_CA_AcceptMaturityRat
    INT (*m_fnCAAcceptMaturityRat)( BYTE *pSzPin , HCSHANDLE hService);//CSCAAcceptMaturityRat
    INT (*m_fnCancelMaturityRat)(void );//CS_CA_CancelMaturityRat
    INT (*m_fnCACancelMaturityRat)( HCSHANDLE hService );//CSCACancelMaturityRat
    INT (*m_fnAcceptStartView)( BYTE *pSzPin );//CS_CA_AcceptStartView
    INT (*m_fnCAAcceptStartView)( BYTE *pSzPin , HCSHANDLE hService );//CSCAAcceptStartView
    INT (*m_fnCancelStartView)(void);//CS_CA_CancelStartView
    INT (*m_fnCACancelStartView)( HCSHANDLE hService );//CSCACancelStartView
    T_CaStatus (*m_fnGetCardStatus)(void);//CS_CA_GetCardStatus
    INT (*m_fnAcceptOrderInfo)( BYTE *pSzPin  );//CS_CA_AcceptOrderInfo
    INT (*m_fnCAAcceptOrderInfo)( BYTE *pSzPin ,HCSHANDLE hService );//CSCAAcceptOrderInfo
    INT (*m_fnCancelOrderInfo)(void);//CS_CA_CancelOrderInfo
    INT (*m_fnCACancelOrderInfo)( HCSHANDLE hService  );//CSCACancelOrderInfo
    INT (*m_fnAcceptPPVRequest)( BYTE *pSzPin );//CS_CA_AcceptPPVRequest
    INT (*m_fnCAAcceptPPVRequest)( BYTE *pSzPin , HCSHANDLE hService);//CSCAAcceptPPVRequest
    INT (*m_fnCancelPPVRequest)(void);//CS_CA_CancelPPVRequest
    INT (*m_fnCACancelPPVRequest)( HCSHANDLE hService);//CSCACancelPPVRequest
    void (*m_fnCACheckandReportShortMsg)(void);//CSCACheckandReportShortMsg
    void (*m_fnCACancelCardBanner)(DWORD dwParam);//CSCACancelCardBanner
    void (*m_fnCACancelShortMsg)(DWORD dwParam);//CSCACancelShortMsg
    INT (*m_fnEnterMenu)( CONAX_MENU_STATUS eMenuType, DWORD dwParam );//CS_CA_EnterMenu
    INT (*m_fnExitMenu)( CONAX_MENU_STATUS eMenuType , DWORD dwParam);//CS_CA_ExitMenu
}CSShellConaxCaInitParam_S;

/**@brief ConaxCA相关配置信息结构体*/
typedef struct
{
	DWORD  m_dwCaMailAddress;///<邮件的首地址	
}CSShellConaxCaConfig_S;

/**
@brief 设置ConaxCA相关操作接口给MMCP
@param[in] ConaxCA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellConaxCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellConaxCaSetParam(CSShellConaxCaGetDefaulParam());
@endcode
*/
BOOL CSShellConaxCaSetParam(const CSShellConaxCaInitParam_S* psConaxCa);

	
/**
@brief 获取同方3CA默认实现参数
@return MMCP提供同方3CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得同方3CA的对象须使用csshell_Conaxca_private.h中的CSShellConaxCaGetParam接口
@see CSShellConaxCaGetParam
*/
const CSShellConaxCaInitParam_S* CSShellConaxCaGetDefaulParam(void);

/**
@brief 设置同方3CA的相关配置信息
@param[in] psConfig 同方3CA相关配置信息的结构体实例
@note 该接口必须在CSShellConaxCaSetParam之后调用
@see CSShellConaxCaConfig_S
*/
void CSShellConaxCaSetConfig(CSShellConaxCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


