/**@defgroup CSSMC [CSSMC]Smartcard drvier
@brief �ṩ���ܿ�ͳһ�ӿ�

@version 0.4 2009/02/26 �����˻�ȡ���ܿ�ʱ��Ƶ�ʽӿ�
@version 0.3 2009/01/06 ������һЩ���ܽӿ�
@version 0.2 2008/11/5  ��һ��ϸ���汾
@version 0.1 2008/10/23 ��ʼ�汾
@{
*/

#ifndef __CS_SMC_H
#define __CS_SMC_H

#include "cs_typedef.h"
#include "CS_OS.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ���ܿ��豸����*/
#define CS_SMC_DEVICE_COUNT 	(2)       /* 1 or 2  Total Smart Card Device Number */


/**@brief ���ܿ�Э��*/
typedef enum _SmcProtocol
{
	CS_SMC_PROTOCOL_UNKNOWN,		/**< δ֪Э��*/
	CS_SMC_PROTOCOL_T0,				/**< ������*/
	CS_SMC_PROTOCOL_T1,				/**< ������*/
	CS_SMC_PROTOCOL_T14,			/**< ������*/

	CS_SMC_PROTOCOL_MAX				/**<  ���� */
}E_SmcPrococol;

/**@brief ���ܿ���׼*/
typedef enum _SmcStandard
{
	CS_SMC_STANDARD_NDS,			/**< NDS��׼*/
	CS_SMC_STANDARD_ISO,			/**< ISO��׼*/
	CS_SMC_STANDARD_EMV2000,		/**< EMV2000��׼*/
	CS_SMC_STANDARD_IRDETO,			/**< ���ϵ±�׼*/

	CS_SMC_STANDARD_MAX				/**<  ���� */
}E_SmcStandard;

/**@brief CA����*/
typedef enum _CaType
{
	SMC_UNKNOWN_CA = 0,
	SMC_CONAX_CA,  
	SMC_VIACCESS_CA , 
	SMC_TONGFANG_CA ,
	SMC_DVN_CA , 
	SMC_CRYPTOWORKS_CA , 
	SMC_MEDIAGUARD_CA , 
	SMC_SUMAVISION_CA ,
	SMC_ST_CA,
	SMC_NAGRAVISON_CA , 
	SMC_IRDETO_CA, 
	SMC_COMVENIENT_CA,
	SMC_ABV_CA

}E_SMC_CaType;

/**@brief ���󷵻�ֵ*/
typedef enum _DrvReturnType
{
	CS_SMC_FAILURE = -1,
	CS_SMC_SUCCESS,
	CS_SMC_TIMEOUT,
	CS_SMC_ERROR_PARITY,
	CS_SMC_NO_ANSWER,
	
	CS_SMC_OUT,
	CS_SMC_IN,	
	CS_SMC_BUSY,
	CS_SMC_NOT_LOCK_OWNER,
	CS_SMC_READY,

	CS_SMC_MUTE,
	CS_SMC_ABORT,
	CS_SMC_INVALID,
	CS_SMC_INVALID_PROTOCOL,
	CS_SMC_NOT_RESET,
	CS_SMC_NOT_SUPPORT,

	CS_SMC_MAX	
}E_DrvReturnType;

/**
@brief ���ܿ��ص�����

��CA�����ṩ�������ܿ�������߰γ���ʱ����Coship���ܿ�API����
@param[in] dCardIndex ���ܿ�����
@param[in] bStatus ���ܿ�״̬
*/
typedef void (*T_CSSMCTaskCallback)(DWORD dwCardNumber, INT nStatus);


/**
@brief ���ܿ���ʼ��

*��ϵͳ������ʱ����ã�����ֻ����һ��
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE;
*/
INT CSSMCInit(void);

/**
@brief �����ܿ�

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] CallBack �ص�����
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE;���������Ч����CS_SMC_INVALID_INDEX��
*/
INT CSSMCOpen(DWORD dCardIndex, T_CSSMCTaskCallback CallBack);

/**
@brief �ر����ܿ�

@param[in] dCardIndex ���ܿ����ۺ�
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE;���������Ч����CS_SMC_INVALID_INDEX��
*/
INT CSSMCClose(DWORD dCardIndex);

/**
@brief �豸�����ܿ�֮������ݴ���

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] WriteData д�����ܿ�������
@param[in] NumberToWrite д�����ݳ���
@param[out] ResponseData ��ȡ���ݵĴ洢λ��
@param[out] NumberRead ��ȡ���ݵĳ���
@param[out] StatusWord �����ṩ��״̬��
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE;���û�п�����CS_SMC_NO_CARD��������ܿ���æ�򷵻�CS_SMC_CARD_BUSY�������ʱ����CS_SMC_TIMEOUT;���������Ч����CS_SMC_INVALID_INDEX�������������г��ִ��󷵻�CS_SMC_ERROR_PARITY
*/
INT CSSMCDataExchange
(
	DWORD  dCardIndex,
	BYTE  *WriteData,
	DWORD  NumberToWrite,
	BYTE  *ResponseData,
	DWORD *NumberRead,
	BYTE  *StatusWord
);

/**
@brief �豸�����ܿ�֮������ݴ���

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] WriteData д�����ܿ�������
@param[out] ResponseData �����ܿ��ж�ȡ������
@note:
- case  ((WriteData[0] & 0x10) != 0)    WriteData[1] valid
- case  ((WriteData[0] & 0x20) != 0)    WriteData[2] valid    
- case  ((WriteData[0] & 0x40) != 0)    WriteData[3] valid
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE;���û�п�����CS_SMC_NO_CARD��������ܿ���æ�򷵻�CS_SMC_CARD_BUSY�������ʱ����CS_SMC_TIMEOUT;���������Ч����CS_SMC_INVALID_INDEX
*/
INT CSSMCSetProtocol
(
	DWORD  dCardIndex,
	BYTE  *WriteData,
	BYTE  *ResponseData
);

/**
@brief ���ܿ���Դ�ر�

@param[in] dCardIndex ���ܿ����ۺ�
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE;
*/
INT CSSMCPowerDown(DWORD dCardIndex);

/**
@brief �������ܿ�

@param[in] dCardIndex ���ܿ����ۺ�
@param[out] pATR �������ܿ���Ӧ���ź�
@param[out] pAtrLength pATR�ĳ���
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE;���û�п�����CS_SMC_NO_CARD��������ܿ���æ�򷵻�CS_SMC_CARD_BUSY
*/
INT CSSMCResetSmartCard
(
	DWORD dCardIndex,
	BYTE* pATR,
	DWORD* pAtrLength
);

/**
@brief ������ܿ�

@param[in] dCardIndex ���ܿ����ۺ�
@return ����ɹ�����CS_SMC_SUCCESS;���û�п�����CS_SMC_NO_CARD�������Ч�Ŀ����򷵻�CS_SMC_INVALID_INDEX
*/
INT CSSMCDetectSmartCard(DWORD dCardIndex);

/**
@brief ��ȡ���ܿ���ʷ�ֽ�����

@param[in] dCardIndex ���ܿ����ۺ�
@param[in/out] historyData �洢��ʷ�ֽ����ݿռ�
@param[in/out] pSize ������ʷ�ֽڸ���
@return ����ɹ�����CS_SMC_SUCCESS;���û�п�����CS_SMC_NO_CARD�������Ч�Ŀ����򷵻�CS_SMC_INVALID_INDEX
*/
INT CSSMCGetHistorical(DWORD  dCardIndex, unsigned char* historyData ,DWORD* pSize);

/**
@brief �������ܿ��Ĳ�����

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] BaudRate ������
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE��
*/
INT CSSMCSetBaudRate(DWORD dCardIndex, DWORD BaudRate);

/**
@brief �����ܿ�д����

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] pWriteBuf ׼��д���ݵĴ洢��
@param[in] NumberToWrite ׼��д���ݵĳ���
@param[out] pNumberWrite ��д�����ݳ���
@param[in] Timeout ��ʱʱ��
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE��
*/
INT CSSMCSend(DWORD dCardIndex,
							BYTE  *pWriteBuf,
							DWORD NumberToWrite,
							DWORD *pNumberWrite,
							DWORD  Timeout);

/**
@brief �����ܿ�������

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] pReadBuf ��ȡ���ݴ洢��
@param[in] NumberToRead ��ȡ�ռ��ڴ泤��
@param[out] pNumberRead �Ѷ�ȡ�����ݳ���
@param[in] Timeout ��ʱʱ��
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE��
*/
INT CSSMCReceive(DWORD dCardIndex,
								BYTE  *pReadBuf,
								DWORD NumberToRead,
								DWORD *pNumberRead,
								DWORD  Timeout);

/**
@brief ��ȡ���ܿ�Э��Ͳ�����

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] protocol Э������
@param[in] rate �������
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE�����δ��⵽���ܿ�����CS_SMC_NO_CARD
*/
INT CSSMCGetSCCapability(DWORD dCardIndex, DWORD *protocol, DWORD* rate);

/**
@brief ��ȡ���ܿ�������

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] protocol Э������
@param[in] rate �������
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE�����δ��⵽���ܿ�����CS_SMC_NO_CARD
*/
INT CSSMCGetSCProtocol(DWORD dCardIndex, DWORD *protocol, DWORD *rate);

/**
@brief ���ܿ�����

@param[in] dCardIndex ���ܿ����ۺ�
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE�����δ��⵽���ܿ�����CS_SMC_NO_CARD
*/
INT CSSMCAbortSmartCard(DWORD dCardIndex);

/**
@brief �������ܿ�

@param[in] dCardIndex ���ܿ����ۺ�
@param[in/out] pnCardStatus ���ܿ�״̬
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE�����δ��⵽���ܿ�����CS_SMC_NO_CARD
*/
INT CSSMCLockSmartCard(DWORD dCardIndex, INT *pnCardStatus);

/**
@brief �������ܿ�

@param[in] dCardIndex ���ܿ�����
@param[in/out] pnCardStatus ���ܿ�״̬
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE�����δ��⵽���ܿ�����CS_SMC_NO_CARD
*/
INT CSSMCUnlockSmartCard(DWORD dCardIndex, INT *pnCardStatus);

/**
@brief �������ܿ���׼��Э������

@param[in] dCardIndex ���ܿ����ۺ�
@param[in] eProtocol  Э��T0/T1/T14
@param[in] eStandard  ���ܿ�ͨѶ��׼
@param[in] dwUserData Ԥ��
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE
*/
INT CSSMCConfig(DWORD dCardIndex,E_SmcPrococol eProtocol,E_SmcStandard eStandard,DWORD dwUserData);

/**
@brief �������ܿ�

@param[in] dCardIndex ���ܿ����ۺ�
@param[in/out] pnCardStatus  ���ܿ�״̬
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE
*/
INT CSSMCActiveSmartCard(DWORD dCardIndex, INT* pnCardStatus);

/**
@brief ������ܿ�

@param[in] dCardIndex ���ܿ����ۺ�
@param[in/out] pnCardStatus  ���ܿ�״̬
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE
*/
INT CSSMCDeactiveSmartCard(DWORD dCardIndex, INT* pnCardStatus);

/**
@brief ��λ���ܿ�

@param[in] dCardIndex ���ܿ����ۺ�
@param[in/out] pATR  �洢���ܿ���λӦ������
@param[in/out] pAtrLength ��λӦ�����ݳ���
@param[in] bColdReset ��λ��ʽ(�ȸ�λ�����临λ)
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE
*/
INT CSSMCResetSmartCardEx
(
	DWORD dCardIndex,
	BYTE* pATR,
	DWORD* pAtrLength,
	BOOL bColdReset
);

/**
@brief ��ȡ���ܿ�ʱ��Ƶ��

@param[in] dCardIndex ���ܿ����ۺ�
@param[in/out] dwclockfrequency  �洢��ȡ����ʱ��Ƶ��
@return ����ɹ�����CS_SMC_SUCCESS;���ʧ�ܷ���CS_SMC_FAILURE
*/
INT CSSMCGetSCClockFrequency(DWORD dCardIndex, DWORD *dwclockfrequency);

/**
@brief ���õ�ǰ�����õ������򿨰廹�Ƿ��򿨰�

@param[in] bIsReverseDir TURE:Ϊ���򿨰壬FALSE��Ϊ���򿨰�
@                        Ĭ�����Ϊ���򿨰�
@return None
*/
void CSSMCSetCardReaderResetDir(BOOL bIsReverseDir);
#ifdef __cplusplus
}
#endif
#endif

