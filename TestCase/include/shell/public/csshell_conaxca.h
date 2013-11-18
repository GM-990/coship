
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
@brief conaxCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��conax ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellConaxCaSetParam CSShellConaxCaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellConaxCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_Conax
	
    CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���
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

/**@brief ConaxCA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD  m_dwCaMailAddress;///<�ʼ����׵�ַ	
}CSShellConaxCaConfig_S;

/**
@brief ����ConaxCA��ز����ӿڸ�MMCP
@param[in] ConaxCA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellConaxCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellConaxCaSetParam(CSShellConaxCaGetDefaulParam());
@endcode
*/
BOOL CSShellConaxCaSetParam(const CSShellConaxCaInitParam_S* psConaxCa);

	
/**
@brief ��ȡͬ��3CAĬ��ʵ�ֲ���
@return MMCP�ṩͬ��3CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���ͬ��3CA�Ķ�����ʹ��csshell_Conaxca_private.h�е�CSShellConaxCaGetParam�ӿ�
@see CSShellConaxCaGetParam
*/
const CSShellConaxCaInitParam_S* CSShellConaxCaGetDefaulParam(void);

/**
@brief ����ͬ��3CA�����������Ϣ
@param[in] psConfig ͬ��3CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellConaxCaSetParam֮�����
@see CSShellConaxCaConfig_S
*/
void CSShellConaxCaSetConfig(CSShellConaxCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


