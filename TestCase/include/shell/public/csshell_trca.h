/**@defgroup PublicShellTRCA PublicShellTRCA �������� CA����ӿ�

@version  ���� 2013-3-1
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
@brief TRCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��TR ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellTRCaSetParam CSShellTRCaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellTRCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_TR
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���

	INT (*m_fnGetVersion)(BYTE* pacVerBuf,  DWORD dwBufSize); ///<CS_CA_GetVersion ��ȡCA�汾��Ϣ
    INT (*m_fnGetSmartCardVersion)(BYTE* pucVerBuf,  BYTE ucBufSize);///<CSCAGetSmartCardVersion ��ȡ���ܿ��汾��Ϣ
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

/**@brief ����CA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD  m_dwMailAddress;///<�ʼ����׵�ַ
	DWORD  m_dwMailAddressBak;///<�ʼ����ݵ��׵�ַ
}CSShellTRCaConfig_S;
/**
@brief ��������CA��ز����ӿڸ�MMCP
@param[in] TRCA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellTRCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellTRCaSetParam(CSShellTRCaGetDefaulParam());
@endcode
*/
BOOL CSShellTRCaSetParam(const CSShellTRCaInitParam_S* psTRCa);

	
/**
@brief ��ȡ����CAĬ��ʵ�ֲ���
@return MMCP�ṩ����CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ�������CA�Ķ�����ʹ��csshell_TRca_private.h�е�CSShellTRCaGetParam�ӿ�
@see CSShellTRCaGetParam
*/
const CSShellTRCaInitParam_S* CSShellTRCaGetDefaulParam(void);

/**
@brief ��������CA�����������Ϣ
@param[in] psConfig ����CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellTRCaSetParam֮�����
@see CSShellTRCaConfig_S
*/
void CSShellTRCaSetConfig(CSShellTRCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


