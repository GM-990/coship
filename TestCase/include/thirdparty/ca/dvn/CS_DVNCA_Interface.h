/****************************************************************************************************************************
* Notice:         Copyright Shenzhen Coship Electronics Co., Ltd., 
*                     All rights reserved.
* Filename:     CS_DVNCA_Interface.h  
* Author:        PengShuangquan
* Date:           Mar 2007
* Description: DVNCA interface
*            
*****************************************************************************************************************************/

#include "udiplus_typedef.h"
#include "cs_cainterface.h"
#include "cs_cainterface_pvrext.h"

#ifndef _CS_DVNCA_INTERFACE_H_
#define _CS_DVNCA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

//@deprecated
typedef enum _CaFactionConfig
{
	CA_SUPPORT_NONE 	= 0x0,
	CA_AUTO_PAIRING 	= 0x01,	/*auto pairing enable*/
	CA_SUPPORT_IPPV 	= 0x02,	/*IPPV mode enable*/
	CA_CHECK_AREAONCE	= 0x04,	/*check area code once*/
	CA_NO_PAIRING  		= 0x08,	/*no need pairing enable*/
	CA_ENABLE_TWOWAY 	= 0x10 	/*two way ca enable*/
}E_CaFactionConfig;

//~ MESSAGE
typedef enum _CaEventGroup
{
	CA_ALARM,				 /*  0  */
	CA_INFO,				 /*  1  */ 
	CA_COMMAND,				 /*  2  */
	CA_STATUS,				 /*  3  */
	CA_MAX	
}E_CaEventGroup;

typedef enum _CaAlarm
{
	CA_ALARM_DVN_CA,                /*  0  Progran encrypt with DNV CA */
	CA_ALARM_OTHER,                 /*  1  Progran encrypt with other CA */
	CA_ALARM_FREE,                  /*  2  Program free */
	CA_ALARM_NEGATIVE_TOKEN,        /*  3  Negative token */
	CA_ALARM_TOKEN_NOT_ENOUGH,      /*  4  Token not enough */
	CA_ALARM_AREA_CODE_ERROR, 	    /*  5  Area code not match */
	CA_ALARM_ECM_BLACKOUT, 		    /*  6  ECM is blackout */
	CA_ALARM_CAN_BE_DECRYPTED,      /*  7  Can be decrypted */
	CA_ALARM_SUBSCRIPTION_EXPIRED,  /*  8  Subscription expired during encryptedvideo playback */
	CA_ALARM_SUCSCRIPTION_REMOVED,  /*  9  Subscription removed during encryptedvideo playback */
	CA_ALARM_STB_LOCKED,		    /*  10 STB locked during encrypted videoplayback */
	CA_ALARM_PROGRAM_CHECKSUM_ERROR,/*  11 Program checksum error */
	CA_ALARM_ECM_DECRYPTION_ERROR,	/*  12 Ecm decryption error */
	CA_ALARM_SMC_NOT_PAIRED, 		/*  13 Smart card not paire */
	CA_ALARM_SMC_OUT,				/*  14 smartcard removed */
	CA_ALARM_SMC_INVALID,			/*  15 smartcard invalid */
	CA_ALARM_SMC_CHECKING,			/*  16 smartcard checking*/
	CA_ALARM_SMC_IN,	            /*  17 smartcard in */
	CA_ALARM_AREA_CODE_OK, 	   		/*  18 Area code ok */
	CA_ALARM_SMC_OK,                /*  19 smartcard ok */
	CA_ALARM_SMC_ACTIVATION_EXPIRED,/*  20 smart card activation  expired */
	CA_ALARM_ECM_DATA_ERROR,    	/*  21 ECM data error */
	CA_ALARM_SMC_SECURED_CHANNEL,   /*  22 smart card data error secured channel */
	CA_ALARM_CANCEL,                /*  23 cancel alarm msg on the screen */
	CA_ALARM_MAX
}E_CaAlarm;

typedef enum _CaStuaus
{
	CA_STATUS_PAIRING_PREPARE,    	 /*  0 Pairing prepare */
	CA_STATUS_PAIRING_START,         /*  1 Pairing start */
	CA_STATUS_SYSTEM_PAIRING,        /*  2 System pairing */
	CA_STATUS_SYSTEM_UPDATE,         /*  3 System updating */
	CA_STATUS_SET_PASSWORD,          /*  4 Set password */
	CA_STATUS_CHECK_PASSWORD,        /*  5 Check password */
	CA_STATUS_WRITE_USER_INFO,       /*  6 Write user information */
	CA_STATUS_PAIRING_COMPLETE,	     /*  7 Pairing complete */
	CA_STATUS_PAIRING_ERROR,	     /*  8 Pairing error(time out, no new version) */
	CA_STATUS_SMC_IN,				 /*  9 Smc in */
	CA_STATUS_SMC_OUT, 				 /*  10 Smc out */ 
	CA_STATUS_SMC_OK,                /*  11 smartcard ok */
	CA_STATUS_SMC_READY,             /*  12 Cak init ok,we can communicate with CA now */
	CA_STATUS_MAX
}E_CaStuaus;

typedef enum _CaCommand
{
	CA_COMMAND_STB_ON,				 /* 0  dwParm2 = NULL */
	CA_COMMAND_STB_OFF,				 /* 1  dwParm2 = NULL */
	CA_COMMAND_TOKEN_CHANGE,         /* 2  dwParm2 = (DWORD)&T_CaTokenChange */
	CA_COMMAND_EMAIL,				 /* 3  dwPram2  = (DWORD)&T_CaEmail */
	CA_COMMAND_SHORT_MSG,			 /* 4  dwParm2 = (DWORD)&T_CaShortMsg */
	CA_COMMAND_SMCID_MSG,			 /* 5  dwParm2 = NULL; *///@deprecated
	CA_COMMAND_FINGER_PRINT,		 /* 6  dwPram2  = E_FingerPrint; */  
	CA_COMMAND_RATING_CONTROL,		 /* 7  dwPram2  = Parent Rate*/
	CA_COMMAND_RATING_CONTROL_UNLOCK,/* 8  dwPram2 = NULL */
	CA_COMMAND_UPLOAD,				 /* 9  Reserver   */
	CA_COMMAND_OS_UPDATE,			 /* 10  Reserver   */
	CA_COMMAND_REBOOT,				 /* 11  dwPram2 = 0 App should reboot the STB */
	CA_COMMAND_MAX
}E_CaCommand;

typedef enum _FingerPrint
{
	E_FINGER_HIDE,
	E_FINGER_SHOW,	/*App Should display SMC ID  */
	E_FINGER_MAX
}E_FingerPrint;

typedef struct  _CaDate
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
}T_CaDateTime;

#define CA_MAX_SIZE_TXT_MAIL       (1024)

typedef enum _TxtType
{
	CA_TXT_MAIL         	= 0,/*Text -- Mailbox,	�ʼ���Ϣ */
	CA_TXT_ANNOUNCE     	= 1,/*Text -- Announcement,�㲥֪ͨ */
	CA_TXT_CLUB         	= 2 /*Club number,��Ա�� */
}T_TxtType;

typedef struct _CaMsg		/* �ʼ����� */
{
	BYTE    	m_cType;     			/*T_TxtType// 0: Text_Mail  1: Text_Announcement  */
	DWORD   	m_dwLen;
	BYTE    	m_szContent[CA_MAX_SIZE_TXT_MAIL];
}T_CaMsg;

typedef struct _CaMail		/*�ʼ��ṹ�� */
{	
    BOOL          m_bReadFlag;    /*�Ѷ���־ */
    BOOL          m_bLockFlag;	  /*�ʼ�������־ */
    T_CaDateTime  m_stArriveTime; /*�ʼ������ʱ�� */
    T_CaMsg       m_stMsg;
}T_CaMail;

#define MAX_SHORT_MSG_LEN (256)

typedef struct _CaShortMsg
{
    INT           m_nlen;	
    BYTE         m_acMessage[MAX_SHORT_MSG_LEN];
}T_CaShortMsg;

typedef struct _CaFtaSubscriptionService
{							  
	DWORD		   m_dwProductID;      		/*defined by the head-end system. */
	DWORD		   m_dwEventID;             /*Event ID (reserved) */
	T_CaDateTime   m_stStartTime;           /*Start Time */
	DWORD          m_dwReserved1;           /*Reserved */
	T_CaDateTime   m_stEndTime;             /*End Time */
	DWORD          m_dwReserved2;           /*Reserved */
}T_CaFtaSubscriptionService;

#define MAX_CONFIRMCODE_LEN (17)

typedef struct _CaFtaTransactionRecord
{
	DWORD		   m_dwType;
	DWORD          m_dwID;
	float          m_fPrice;
	T_CaDateTime   m_stDateTime;
	BYTE          m_acConfirmCode[MAX_CONFIRMCODE_LEN];
}T_CaFtaTransactionRecord;

typedef struct _DvnTokenChange
{
	float           m_fTokenChange;			/*Value to be added or deduct from current token value */
	INT             m_nOperationID;			/*Unique reference ID for the operation */
}T_CaTokenChange;

#define CA_MAX_COUNTRY_NUM         (20)

typedef struct _FtaCountry
{
	BYTE    m_cFlag;       		/*���ñ�־��1�����ã�0�������� */
	BYTE    m_acCountry[4]; 	/*���ҿ��ô��� */
}T_FtaCountry;

typedef struct _FtaCountryList
{
	WORD    		m_wCountryNum;    /*�ṹ�й��Ҵ������ */
	T_FtaCountry  	m_stCountryList[CA_MAX_COUNTRY_NUM]; /*��ŵĹ��Ҵ��� */
}T_FtaCountryList;

/**************************************************************************
*Function Name: CS_CA_GetVersion
*   ��ȡCA�汾��Ϣ
*
*Parameters
*   pacVerBuf[out]:ָ��洢CA�汾��Ϣ�Ŀռ�
*   dwBufSize[in]:����һ����󳤶�ֵ.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CA_GetCoreVersion
*   ��ȡDVN �ں˰汾
*
*Parameters
*   pacVerBuf[out]:ָ��洢CA�汾��Ϣ�Ŀռ�
*   dwBufSize[in]:����һ����󳤶�ֵ.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetCoreVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
* Function Name: CS_CA_SetDebugMode
*   ����CA�������ã�ע������ڵ���CS_CA_Init֮ǰ����
*
*Parameters	
*	  	nMode[in] ����ģʽ����E_CaFactionConfig
*Return Values: 
*	@deprecated
**************************************************************************/
void CSCASetDvnConfiguration(INT nMode);

/**************************************************************************
*Function Name:CS_CA_TransactionComfirm
* ȷ���Ƿ���
*
*Parameters 
*	bFlag:�Ƿ��ף�TRUE ȷ�� ,FALSE, ȡ��
* 	nIndex: ���׺�
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*	@deprecated
**************************************************************************/
INT CS_CA_TransactionComfirm(BOOL bFlag,INT nIndex);

/**************************************************************************
* Function Name: CS_CA_PerformPairing
*   �������
*
*Parameters	
*
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_PerformPairing(void);

/**************************************************************************
* Function Name: CS_CA_GetInfo_STBAttribute
*   ��ȡSTB ID
*
*Parameters	
*	  pcStbIDBuf
*            [out] STB ID����
*       nBufSize
*            [in] �����Buf����
*	 pnStbIDWidth
*		[out] STB ID���ݳ���
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetInfo_STBAttribute(BYTE* pcStbIDBuf, INT nBufSize);

/**************************************************************************
* Function Name: CS_CA_GetInfo_SmcID
*   ��ȡSMC ID
*
*Parameters	
*	  pcSmcIDBuf
*            [out] SMC ID����
*       nBufSize
*            [in] �����Buf����
*	 pnSmcIDWidth
*		[out] SMC ID���ݳ���
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetInfo_SmcID(BYTE* pcSmcIDBuf, INT nBufSize);

/**************************************************************************
* Function Name: CS_CA_GetInfo_AccountNo
*   ��ȡ�ʻ���
*
*Parameters	
*	  pcAccountNoBuf
*            [out] Accout No����
*       nBufSize
*            [in] �����Buf����
*	 pnAccountNoWidth
*		[out] Accout No���ݳ���
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetInfo_AccountNo(BYTE* pcAccountNoBuf, INT nBufSize);

/**************************************************************************
* Function Name: CS_CA_GetInfo_SysDescriptor
*   ��ȡָ�����ϵͳ������
*
*Parameters	
*	  nSysDescriptorIndex
* 	  	[in] ���������(0~4)
*	  pcSysDescriptorBuf
*            [out] ����������
*       nBufSize
*            [in] �����Buf����
*	 pnSysDescriptorWidth
*		[out] ���������ݳ���
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetInfo_SysDescriptor(INT nSysDescriptorIndex, BYTE* pcSysDescriptorBuf, INT nBufSize);

/**************************************************************************
* Function Name: CS_CA_TokenGetValue
*   ��ȡ�ʻ����
*
*Parameters	
*	       pfTokenValue
*                   [out]   ���
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_TokenGetValue(float* pfTokenValue);

/**************************************************************************
* Function Name: CS_CA_TokenConfirmChange
*   ȷ���ʻ����仯
*
*Parameters	
*	       nOperationID
*                   [in]   ������  (Ĭ��ֵ0)
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_TokenConfirmChange(BOOL bFlag,INT nOperationID);

/**************************************************************************
* Function Name: CS_CA_TokenDeduct
*   �۳�Ǯ�����
*
*Parameters	
*	       fTokenValue
*                   [in]   ĳ����Ŀ/����ļ۸�
*            nServiceType
*                   [in]   ��Ŀ/���������
* 			nOperationID
*					[in]   ��Ŀ/�����ID
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_TokenDeduct(float fTokenValue, INT nServiceType, INT nOperationID);

/**************************************************************************
* Function Name: CS_CA_GetSubscriptionRecordNum
*   ��ȡ�����¼��Ŀ
*
*Parameters
*	       pnLocalNum
*                   [out]   ���򱾵���Ӫ�̽�Ŀ��¼����
*            pnCenterNum
*                   [out]   ����CCTV��Ӫ�̽�Ŀ��¼����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetSubscriptionRecordNum( INT *pnLocalNum,INT *pnCenterNum );

/**************************************************************************
* Function Name: CS_CA_GetSubscriptionRecordContent
*   ��ȡָ����Ź����¼������
*
*Parameters	
*	       nRecordIndex
*                   [in]   ���(��0��ʼ)
*            pstSubRecord
*                   [out]  ��¼����
*            nSubRecordType
*                   [in]  �����¼����:0 CCTV��Ӫ�̣� 1 ������Ӫ��
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetSubscriptionRecordContent(INT nRecordIndex, INT nSubRecordType, T_CaFtaSubscriptionService* pstSubRecord);

/**************************************************************************
* Function Name: CS_CA_GetTransactionRecordNum
*   ��ȡ���׼�¼����
*
*Parameters
*
*Return Values
*		�ɹ����ؼ�¼������ʧ�ܷ���FAILURE
**************************************************************************/
INT CS_CA_GetTransactionRecordNum(void);

/**************************************************************************
* Function Name: CS_CA_GetTransactionRecordContent
*   ��ȡָ����Ž��׼�¼������
*
*Parameters	
*	       nIndex
*                   [in]   ���(��0��ʼ)
*            pstTracRecord
*                   [out]  ��¼����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetTransactionRecordContent(INT nIndex, T_CaFtaTransactionRecord* pstTracRecord);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* ����һ���ʼ�
*
*Parameters 
*	pstTxt[in]:�ʼ�����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Add(T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Del
* ɾ��ĳһ���ʼ�
*
*Parameters 
*	nIndex[in]:�ʼ��±꣬��0��ʼ
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Del(DWORD dwIndex);

/**************************************************************************
*Function Name:CS_CA_TXT_DelAll
* ɾ�����б�������ʼ�
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_DelAll(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetNum
* ��ȡ���ʼ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* ��ȡ����ĳһ���ʼ�������
*
*Parameters 
*	nIndex[in]:�ʼ��±꣬��0��ʼ
*	ppstTxt[out]:�����ʼ��Ĵ洢��ַ
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, T_CaMail* ppstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNew
* ��ѯĳһ���ʼ��Ƿ���δ���ʼ�
*
*Parameters 
*	eType[in]:�ʼ�����
*
*Return Values: 
*	1: TRUE 
*	0: FALSE
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(T_TxtType eType);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* ��ȡδ���ʼ�����
*
*Parameters 
*	��
*
*Return Values: 
*	����δ���ʼ�����
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* �������ʼ����浽flash�У�һ���ǹػ���ϵ�ǰ���еĲ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_Txt_SetReadFlag
* �����ʼ��Ƿ��Ѷ���־
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	bRead[in]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_SetLockFalg
* �����ʼ��Ƿ�����
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	bLockFlag[in]:�Ѽ���(TRUE),δ����(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_SetLockFalg(DWORD dwIndex, BOOL bLockFlag);

/**************************************************************************
*Function Name:CS_CA_TXT_GetLockFalg
* �õ�ָ���ʼ�������״̬
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	pbLockFlag[out]:�Ѽ���(TRUE),δ����(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_GetLockFalg(DWORD dwIndex, BOOL* pbLockFlag);

/**************************************************************************
*Function Name:CS_CA_GetSmcStatus
* ��ȡ���ܿ���ǰ״̬
*
*Parameters 
*	pcSmcStatus[In/Out]:�洢���ܿ�״̬���������ֵΪ0����OUT��Ϊ1��IN
*
*Return Values: 
*	SUCCESS ��ȡ���ܿ���ǰ״̬�ɹ� 
*	FAILURE �����������
**************************************************************************/
INT CS_CA_GetSmcStatus(BYTE* pcSmcStatus);

/**************************************************************************
*Function Name:CS_CA_GetRating
* ��ȡ���˼���
*
*Parameters 
*	pcSmcStatus[In/Out]:pdwPGLevel
*
*Return Values: 
*	SUCCESS :�������سɹ�
*	FAILURE: ��������ʧ��
**************************************************************************/
INT CS_CA_GetRating(DWORD* pdwPGLevel);

/**************************************************************************
*Function Name:CS_CA_SetRating
* ���û����г��˼���
*
*Parameters 
*	pcSmcStatus[In/Out]:pdwPGLevel 0~15 ;
*
*Return Values: 
*	SUCCESS :�������سɹ�
*	FAILURE: ��������ʧ��

**************************************************************************/
INT CS_CA_SetRating(DWORD pdwPGLevel);

/**************************************************************************
*Function Name:CS_CA_GetSmcStatus
* ��ȡ���ܿ��汾
*
*Parameters 
*	pucVersion[In/Out]:�洢smartcard�汾����Ҫ3���ֽ����洢���Ҹ�ʽΪASCII��
*
*Return Values: 
*	SUCCESS ��ȡ���ܿ���ǰ�汾�ɹ� 
*	FAILURE �����������
**************************************************************************/
INT CSCAGetSmartcardVersion(BYTE * pucVersion);

/**************************************************************************
*Function Name: CS_CA_SetAreaBlock
*  �����������ޣ�����NIT�����ṹ��
*
*Parameters 
*	pstCountypList[in]:���Ҵ�������͹��Ҵ����
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_SetAreaBlock(T_FtaCountryList* pstCountypList);

/**************************************************************************
*Function Name: CS_CA_GetPairedStatus
*  ��ȡ���ܿ��������е����״̬
*
*Parameters 
*	pbPaired[In/Out]:���״̬��=1����ԣ�=0δ���
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetPairedStatus(BOOL *pbPaired);

/**************************************************************************
*Function Name:CS_CA_GetFlashAddress
*  ��ȡCA��ҪFLASH�������ʼ��ַ��CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ�����
*
*Parameters 
*	dwFlashSize: ָ��Ҫ����FLASH��Ĵ�С
*
*Return Values:
*	���ط������ʼ��ַ������Ƿ���һ��Block��Flash�����Բ���dwFlashSize
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(DWORD dwFlashSize);

/**************************************************************************
*Function Name:CS_CA_GetBackupFlashAddress
*  ��ȡCA��Ҫ����FLASH�������ʼ��ַ��CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ�����
*
*Parameters 
*	dwFlashSize: ָ���������������FLASH��Ĵ�С
*
*Return Values:
*	���ط������ʼ��ַ������Ƿ���һ��Block��Flash�����Բ���dwFlashSize
**************************************************************************/
extern DWORD CS_CA_GetBackupFlashAddress(DWORD dwFlashSize);

#ifdef  __cplusplus
}
#endif

#endif /*_CS_DVNCA_INTERFACE_H_ */

