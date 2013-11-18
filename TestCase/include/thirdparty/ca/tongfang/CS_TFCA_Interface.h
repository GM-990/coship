/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_TFCA_Interface.h  
* Author:      Xiejianghua
* Date:        Mar 2006 
* Description: TF CA interface
*            
*****************************************************************************************************************************/
#include "udiplus_typedef.h"
#include "cs_cainterface.h"

/****************************************************************************************************************************
* 			                Revision History                                *
*****************************************************************************************************************************
* - Revision 1.0  2006/3/23
*   Programmer:Xiejianghua
*   Create.
*****************************************************************************************************************************/
#ifndef _CS_TFCA_INTERFACE_H_
#define _CS_TFCA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum _CaMessage
{
	CA_COMMAND,
	CA_ALARM,
	CA_STATUS,
	CA_INFO,
	CA_MMI
}T_CaMessage;

typedef enum  _CaCommand
{

	//���ʼ�����
	CA_COMMAND_NEW_EMAIL,			 // dwParm2 = (DWORD)T_CaEmailIconType

	//ָ��
	CA_COMMAND_FINGERPRINT,			//dwParm2 = (DWORD)CardId

	//��ʾOSD��Ϣ
	CA_COMMAND_SHOW_OSD,			//dwParm2 = (DWORD)&T_CaOsd
	//����OSD��Ϣ
	CA_COMMAND_HIDE_OSD,			//dwParm2 = (WORD)T_CaOsdStyle

	//IPPV������Ϣ
	CA_COMMAND_IPPV_NOTIFY,			//dwParm2 = (DWORD)&T_CaIppvNotify
	//ȡ��IPPV������ʾ
	CA_COMMAND_IPPV_CANCEL,			//dwParm2 = (DWORD)EcmPid

	CA_COMMAND_MAX
}T_CaCommand;

typedef enum _CaAlarm
{	
	CA_ALARM_CANCEL = 0,       /* 0 ȡ����ǰ����ʾ*/
	CA_ALARM_BADCARD,          /* 1 �޷�ʶ�𿨣�����ʹ��*/
	CA_ALARM_EXPICARD,         /* 2 ���ܿ��Ѿ����ڣ�������¿�*/
	CA_ALARM_INSERTCARD,       /* 3 ���Ž�Ŀ����������ܿ�*/
	CA_ALARM_NOOPER,           /* 4 ���в����ڽ�Ŀ��Ӫ��*/
	CA_ALARM_BLACKOUT,         /* 5 ��������*/
	CA_ALARM_OUTWORKTIME,      /* 6 ���ڹ���ʱ����*/
	CA_ALARM_WATCHLEVEL,       /* 7 ��Ŀ��������趨�ۿ�����*/
	CA_ALARM_PAIRING,          /* 8 ��ĿҪ�������Ӧ*/
	CA_ALARM_NOENTITLE,        /* 9 û����Ȩ*/
	CA_ALARM_DECRYPTFAIL,      /* 10 ��Ŀ����ʧ��*/
	CA_ALARM_NOMONEY,          /* 11 ���ڽ���*/
    CA_ALARM_ERRREGION,        /* 12 ������ȷ*/
    CA_ALARM_MAX
}T_CaAlarm;

typedef enum _CaStatus
{
	CA_STATUS_SMC_IN,
	CA_STATUS_SMC_OUT,
	CA_STATUS_DSM_OK,
	CA_STATUS_SRV_FREE,
	CA_STATUS_SRV_OTHER,
	CA_STATUS_MAX
}T_CaStatus;

#define MAX_OPERATOR_NUM       (4)         /*������Ӫ�̸���*/
typedef struct _CaOperatorId               /*��Ӫ��ID*/
{
	WORD	m_wOperatorIdNum;
	WORD	m_awOperatorIds[MAX_OPERATOR_NUM];
}T_CaOperatorId;

#define MAX_OPERATORINFO_LEN    (20)       /*�����Ӫ������*/
typedef struct _CaOperatorInfo
{
	BYTE	m_acOperatorInfo[MAX_OPERATORINFO_LEN+1];   /*��Ӫ��˽����Ϣ*/
}T_CaOperatorInfo;

#define MAX_ACLIST_NUM          (18)        /*���ܿ��ڱ���ÿ����Ӫ�̵�AC�ĸ���*/
typedef struct _CaOperatorAcList            /*��Ӫ�̵�AC��Ϣ*/
{
	WORD	m_wAcListNum;
	DWORD	m_dwOperatorAcList[MAX_ACLIST_NUM];
}T_CaOperatorAcList;

#define	MAX_ENTITL_NUM           (300)      /*���ܿ����������Ȩ��Ʒ�ĸ���*/
typedef struct _CaEntitle
{
	DWORD 		m_dwProductID;
	BYTE		m_cCanTape;			/*�Ƿ��¼��*/
	WORD		m_wExpriData;		/*����ʱ��*/
}T_CaEntitle;

typedef struct _CaServiceEntitles  /*������Ȩ��Ϣ*/
{
	WORD			m_wProductCount;
	T_CaEntitle		m_astEntitles[MAX_ENTITL_NUM];
}T_CaServiceEntitles;

typedef struct  _CaDate
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
}T_CaDate;

//----------------CA ����ֵ--------
#define CA_RTN_FAILURE                  (-1)
#define CA_RTN_OK                       (0x00)
#define CA_RTN_UNKNOWN                  (0x01)    /*- δ֪����*/
#define CA_RTN_POINTER_INVALID          (0x02)    /*- ָ����Ч*/
#define CA_RTN_CARD_INVALID             (0x03)    /*- ���ܿ���Ч*/
#define CA_RTN_PIN_INVALID              (0x04)    /*- PIN����Ч*/
#define CA_RTN_PIN_LOCK                 (0x05)    /*- PIN������*/
#define CA_RTN_DATASPACE_SMALL          (0x06)    /*- �����Ŀռ䲻��*/
#define CA_RTN_CARD_PAIROTHER           (0x07)    /*- ���ܿ��Ѿ���Ӧ��Ļ�����*/
#define CA_RTN_DATA_NOT_FIND            (0x08)    /*- û���ҵ���Ҫ������*/
#define CA_RTN_PROG_STATUS_INVALID      (0x09)    /*- Ҫ����Ľ�Ŀ״̬��Ч*/
#define CA_RTN_CARD_NO_ROOM             (0x0A)    /*- ���ܿ�û�пռ��Ź���Ľ�Ŀ*/
#define CA_RTN_WORKTIME_INVALID         (0x0B)    /*- �趨�Ĺ���ʱ����Ч 0��24*/
#define CA_RTN_IPPV_CANNTDEL            (0x0C)    /*- IPPV��Ŀ���ܱ�ɾ��*/
#define CA_RTN_CARD_NOPAIR              (0x0D)    /*- ���ܿ�û�ж�Ӧ�κεĻ�����*/
#define CA_RTN_WATCHRATING_INVALID      (0x0E)    /*- �趨�Ĺۿ�������Ч 4��18*/

/**************************************************************************
* Function Name: CS_CA_Restart
*   ��������CA����
*
*Parameters	
*		
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_Restart(void);

/**************************************************************************
* Function Name: CS_CA_GetTFLibVersion
*   ��ȡͬ����汾
*
*Parameters	
*	  pacVerBuf
*            [out] ��Ű汾��Ϣ��Buffer
*       dwBufSize
*            [in] Buffer ����(����16�ֽ�)
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetTFLibVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
* Function Name: CS_CA_IsPinLocked
*   �ж����ܿ��Ƿ�����
*
*Parameters	
*	   pbPinLock
*            [out] �Ƿ�������־
*Return Values
*	  CA_RTN_OK : 				��ȷ����  
*	  CA_RTN_POINTER_INVALID: 	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  		�����ܿ�����Ч��
**************************************************************************/
WORD CS_CA_IsPinLocked(BYTE* pbPinLock);

/**************************************************************************
* Function Name: CS_CA_ChangePin
*   �޸����ܿ�����
*Parameters	
*	  dwOldPin
*            [in] ������
*       dwNewPin
*            [in] ������
*Return Values
*	  CA_RTN_OK : 		 		��ȷ����  
*	  CA_RTN_POINTER_INVALID: 	ָ��Ϊ��
*       CA_RTN_CARD_INVALID:  		�����ܿ�����Ч��
*	  CA_RTN_PIN_INVALID:		��PIN�����
*	  CA_RTN_PIN_LOCK:			PIN���ѱ�����
*	  CA_RTN_UNKNOW:			δ֪����
**************************************************************************/
WORD CS_CA_ChangePin(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
* Function Name: CS_CA_GetSCCardID
*   ��ȡ���ܿ��Ŀ���
*
*Parameters	
*	  pacSCIDBuf
*            [out] ���ܿ��Ŀ���
*Return Values
*	  CA_RTN_OK : 		 		��ȷ����  
*	  CA_RTN_POINTER_INVALID: 	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  		�����ܿ�����Ч��
**************************************************************************/
INT	CS_CA_GetSCCardID(DWORD* pdwCardId);

/**************************************************************************
* Function Name: CS_CA_IsPaired
*   �жϻ������뿨�Ƿ������
*Parameters	 
*Return Values
*	  CA_RTN_OK : 		 		 ���ܿ���Ӧ��ǰ������
*	  CA_RTN_CARD_INVALID:  		 �����ܿ�����Ч��
*	  CA_RTN_CARD_NOPAIR:		 ���ܿ�û�ж�Ӧ�κλ�����
*	  CA_RTN_CARD_PAIROTHER:  	 ���ܿ��Ѷ�Ӧ��Ļ�����
**************************************************************************/
WORD CS_CA_IsPaired( void );

/**************************************************************************
* Function Name: CS_CA_SetPaired
*   ����������������ܿ����
*Parameters	
*	  dwPinCode
*            [in]������
 *Return Values
*	  CA_RTN_OK : 		 		��Գɹ�
*	  CA_RTN_POINTER_INVALID: 	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  		�����ܿ�����Ч��
*	  CA_RTN_PIN_INVALID:		PIN�����
*	  CA_RTN_PIN_LOCK:			PIN���ѱ�����
*	  CA_RTN_UNKNOW:			δ֪����
*	  CA_RTN_CARD_PAIROTHER:  	���ܿ��Ѷ�Ӧ��Ļ�����
**************************************************************************/
WORD CS_CA_SetPaired(DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_GetRating
 *   ��ȡ��ĸ����
*Parameters	
*	  pbyRating
*            [out]  ��ĸ��[4-18]
 *Return Values
*	  CA_RTN_OK : 		 		��ȷ����  
*	  CA_RTN_POINTER_INVALID: 	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	 	�����ܿ�����Ч��
*	  CA_RTN_UNKNOW:		 	δ֪����
**************************************************************************/
WORD CS_CA_GetRating(BYTE* pbyRating);

/**************************************************************************
* Function Name: CS_CA_SetRating
 *  ���ø�ĸ����
*Parameters	
*	  dwPinCode
*            [out]  ��ĸ��
 *Return Values
*	  CA_RTN_OK : 		    			��ȷ���� 
*	  CA_RTN_POINTER_INVALID:      	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	    		�����ܿ�����Ч��
*	  CA_RTN_UNKNOW:		   	 	δ֪����
*	  CA_RTN_PIN_INVALID:	    		PIN�����
*	  CA_RTN_PIN_LOCK:	    			PIN���ѱ�����
*	  CA_RTN_WATCHRATING_INVALID:   ����ĸ�ĸ����Ч��������[4-18]
**************************************************************************/
WORD CS_CA_SetRating(DWORD dwPinCode, BYTE byRating);

/**************************************************************************
* Function Name: CS_CA_GetWorkTime
 *  ��ȡ����ʱ��
*Parameters	
*	  pbyStartHour
*            [out]  ��ʼʱ��
*	  pbyEndHour
*		[out] ����ʱ��
 *Return Values
*	  CA_RTN_OK : 				��ȷ����  
*	  CA_RTN_POINTER_INVALID: 	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  		�����ܿ�����Ч��
*	  CA_RTN_UNKNOW:			δ֪����
**************************************************************************/
WORD CS_CA_GetWorkTime(BYTE* pbyStartHour, BYTE* pbyEndHour);

/**************************************************************************
* Function Name: CS_CA_SetWorkTime
 *  ���ù���ʱ��
*Parameters	
*	  dwPinCode
*		[in]	������
*	  pbyStartHour
*            [in]  ��ʼʱ��
*	  pbyEndHour
*		[in] ����ʱ��
 *Return Values
*	 CA_RTN_OK : 		    			��ȷ����  
*	 CA_RTN_POINTER_INVALID:    		ָ��Ϊ��
*	 CA_RTN_CARD_INVALID:  	    		�����ܿ�����Ч��
*	 CA_RTN_UNKNOW:		    		δ֪����
*	 CA_RTN_PIN_INVALID:	    			PIN�����
*	 CA_RTN_PIN_LOCK:	    			PIN���ѱ�����
**************************************************************************/
WORD CS_CA_SetWorkTime(DWORD dwPinCode, BYTE byStartHour, BYTE byEndHour);

/**************************************************************************
* Function Name: CS_CA_GetOperatorIds
 *  ��ȡ��Ӫ��ID
*Parameters	
*	  pstOperatorsId
*		[out] ��Ӫ��ID
 *Return Values
*	  CA_RTN_OK : 		    		��ȷ����  
*	  CA_RTN_POINTER_INVALID:    	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	   	�����ܿ�����Ч��
**************************************************************************/
WORD CS_CA_GetOperatorIds(T_CaOperatorId* pstOperatorsId);

/**************************************************************************
* Function Name: CS_CA_GetOperatorInfo
 *  ��ȡ��Ӫ����Ϣ
*Parameters	
*	  wOperatorsId
*		[in]	��Ӫ��ID
*	  T_CaOperatorInfo
*            [out]  ��Ӫ����Ϣ
 *Return Values
*	  CA_RTN_OK : 		    		��ȷ����  
*	  CA_RTN_POINTER_INVALID:    	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	    	�����ܿ�����Ч��
*	  CA_RTN_DATA_NOT_FIND:	    	û���ҵ���Ӧ��Ӫ��
**************************************************************************/
WORD CS_CA_GetOperatorInfo(WORD wOperatorsId, T_CaOperatorInfo* pstOperatorInfo);

/**************************************************************************
* Function Name: CS_CA_GetACList
 *  ��ȡ��Ӫ��AC��Ϣ
*Parameters	
*	  wOperatorsId
*		[in]	��Ӫ��ID
*	  pstAcList
*            [out]  ��Ӫ��AC��Ϣ
 *Return Values
*	 CA_RTN_OK : 		    		��ȷ����  
*	 CA_RTN_POINTER_INVALID:    	ָ��Ϊ��
*	 CA_RTN_CARD_INVALID:  	    	�����ܿ�����Ч��
*	 CA_RTN_DATA_NOT_FIND:	    	û���ҵ���Ӧ��Ӫ��
**************************************************************************/
WORD CS_CA_GetACList(WORD wOperatorsId, T_CaOperatorAcList* pstAcList );

/**************************************************************************
* Function Name: CS_CA_GetServiceEntitles
 *  ��ȡ��Ӫ��Ȩ��
*Parameters	
*	  wOperatorsId
*		[in]	��Ӫ��ID
*	  pstServiceEntitles
*            [out]  ��Ӫ��Ȩ��
 *Return Values
*	  CA_RTN_OK : 		    			��ȷ����  
*	  CA_RTN_POINTER_INVALID:    		ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	    		�����ܿ�����Ч��
*	  CA_RTN_DATA_NOT_FIND:	   		 û���ҵ���Ӧ��Ӫ��
**************************************************************************/
WORD CS_CA_GetServiceEntitles(WORD wOperatorsId, T_CaServiceEntitles* pstServiceEntitles);

#define MAX_SLOT_NUM       (20)   /*���洢�����Ǯ����*/
typedef struct _CaSlotId          /*Ǯ��ID*/
{
	WORD	m_wSlotNum;
	BYTE	m_acSlotIds[MAX_SLOT_NUM];
}T_CaSlotId;

typedef struct _CaSlotInfo         /*Ǯ����Ϣ*/
{
	WORD	m_wCreditLimit;        /*���ö�*/
	WORD	m_wBalance;            /*�ѻ��ĵ���*/
}T_CaSlotInfo;

#define MAX_PRICE_NUM     (2)      /*����IPPV�۸����*/

typedef struct _CaIppvPrice        /*IPPV��Ŀ�۸�*/
{
	BYTE			m_cPriceCode;  /*�۸�����0: ֻ���տ�,����¼��; 1:���տ�,��¼��*/
	WORD			m_wPrice;      /*�۸����*/
}T_CaIppvPrice;

typedef struct _CaIppvBuyInfo				/*IPPV������Ϣ*/
{
	WORD			m_wOperatorId;			/*��Ӫ��ID*/
	BYTE			m_cSlotID;				/*Ǯ��ID*/
	DWORD 			m_dwProductId;          /*��Ŀ��ID*/
	BYTE			m_cPriceNum;			/*��Ŀ�۸����*/
	T_CaIppvPrice  	m_stPrice[MAX_PRICE_NUM]; 		/*��Ŀ�۸�*/
	WORD			m_wExpiredDate;				/*��Ŀ����ʱ��*/
	WORD			m_wRemainCredit;			/*Ǯ��ʣ�����*/
}T_CaIppvBuyInfo;

typedef struct _CaIppvNotify						/*IPPV����֪ͨ*/
{
	BYTE 				m_cMessageType;	/*��ʾ����0: ���Ԥ���׶�.1: �շѽ׶�*/
	WORD				m_wEcmPid;			/*����ʾ�Ի����־*/
	T_CaIppvBuyInfo		m_stIppvProgram;	/*Ippv��Ŀ��Ϣ*/
}T_CaIppvNotify;

#define	MAX_IPPVPROG_NUM    (300)      /*���ܿ��������IPPV��Ŀ�ĸ���*/

typedef struct _CaIppvInfo					
{
	DWORD 			m_dwProductID;               /*��Ŀ��ID*/
	BYTE			m_cBookEdFlag;			/*��Ʒ״̬1: BOOKING 3: VIEWED*/ 
	BYTE			m_cCanTape;			/*�Ƿ����¼��1������¼��0��������¼��*/
	WORD			m_wPrice;				/*��Ŀ�۸�*/
	WORD			m_wExpiredDate;		/*��Ŀ����ʱ��*/
}T_CaIppvInfo;

typedef struct _CaIppvProgInfo			/*IPPV ��Ŀ��Ϣ*/
{
	WORD			m_wProgramNum;
	T_CaIppvInfo		m_astIppvInfo[MAX_IPPVPROG_NUM];
}T_CaIppvProgInfo;

/**************************************************************************
* Function Name: CS_CA_GetSlotIDs
 *  ��ȡ��Ӫ��Ǯ��ID
*Parameters	
*	  wOperatorsId
*		[in]	��Ӫ��ID
*	  pstSlotId
*            [out]  Ǯ��ID
 *Return Values
*	  CA_RTN_OK : 		    		��ȷ����  
*	  CA_RTN_POINTER_INVALID:    	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	    	�����ܿ�����Ч��
*	  CA_RTN_DATA_NOT_FIND:	    	û���ҵ���Ӧ��Ӫ��
**************************************************************************/
WORD CS_CA_GetSlotIDs(WORD wOperatorsId, T_CaSlotId* pstSlotId);

/**************************************************************************
* Function Name: CS_CA_GetSlotInfo
 *  ��ȡ��Ӫ��Ǯ����Ϣ
*Parameters	
*	  wOperatorsId
*		[in]	��Ӫ��ID
*	  bySlotID
*            [in]  Ǯ��ID
*	  pstSlotInfo
*            [out]  Ǯ����Ϣ
 *Return Values
*	  CA_RTN_OK : 		    		��ȷ����  
*	  CA_RTN_POINTER_INVALID:    	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	    	�����ܿ�����Ч��
*	  CA_RTN_DATA_NOT_FIND:	    	û���ҵ���Ӧ��Ӫ��
**************************************************************************/
WORD CS_CA_GetSlotInfo(WORD wOperatorsId, BYTE bySlotID, T_CaSlotInfo* pstSlotInfo);

/**************************************************************************
* Function Name: CS_CA_GetIPPVProgram
 *  ��ȡ��Ӫ��PPV��Ŀ��Ϣ
*Parameters	
*	  wOperatorsId
*		[in]	��Ӫ��ID
*	  bySlotID
*            [in]  Ǯ��ID
*	  pstIppvInfo
*            [out]  PPV��Ŀ��Ϣ
 *Return Values
*	  CA_RTN_OK : 		    		��ȷ����  
*	  CA_RTN_POINTER_INVALID:    	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	   	�����ܿ�����Ч��
*	  CA_RTN_DATA_NOT_FIND:	    	û���ҵ���Ӧ��Ӫ��
**************************************************************************/
WORD CS_CA_GetIPPVProgram(WORD wOperatorsId, BYTE bySlotID, T_CaIppvProgInfo* pstIppvInfo);

/**************************************************************************
* Function Name: CS_CA_IPPVBuyConfirm
 *  ȷ�Ϲ����ȡ������IPPV��Ŀ
*Parameters	
*	  cBuyFlag
*		[in]	TRUE :����FALSE:������
*	  wEcmPid
*            [in]  ��Ӧ��ĿECMPID
*	  dwPinCode
*            [in]  ������
*	  pstPrice
*            [in]  ��Ŀ�۸� 
 *Return Values
*	  CA_RTN_OK : 		    			��ȷ����  
*	  CA_RTN_POINTER_INVALID:    		�۸�ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	    		�����ܿ�����Ч��
*	  CA_RTN_DATA_NOT_FIND:	    		û���ҵ���Ӧ��Ӫ��
*	  CA_RTN_PIN_INVALID:	    		PIN�����
*	  CA_RTN_PIN_LOCK:	    			PIN���ѱ�����
*	  CA_RTN_CARD_NO_ROOM:	    		���ܿ�û�пռ�
*	  CA_RTN_PROG_STATUS_INVALID:	��Ŀ״̬��Ч
**************************************************************************/
WORD CS_CA_IPPVBuyConfirm(BYTE cBuyFlag,WORD wEcmPid, DWORD dwPinCode, T_CaIppvPrice* pstPrice);

/**************************************************************************
* Function Name: CS_CA_DelIPPVProgram
 *  ɾ����Ӫ��IPPV��Ŀ
*Parameters	
*	  wOperatorId
*		[in]	��Ӫ��ID
*	  wSlotId
*            [in]  Ǯ��ID
*	  wProductId
*            [in]  ��ĿID
*	  wExpireDate
*            [in]  ��Ŀ����ʱ��
 *Return Values
*	  CA_RTN_OK : 		    		��ȷ����  
*	  CA_RTN_CARD_INVALID:  	   	�����ܿ�����Ч��
*	  CA_RTN_DATA_NOT_FIND:	    	û���ҵ���Ӧ�Ľ�Ŀ
*	  CA_RTN_IPPV_CANNTDEL:	    	��Ŀ���ܱ�ɾ��
***************************************************************************/
WORD CS_CA_DelIPPVProgram(WORD wOperatorId, BYTE wSlotId, WORD	wProductId, WORD wExpireDate);

/**************************************************************************
* Function Name: CS_CA_ChangeTFDate
*  ʱ��ת������ͬ�������ʱ��ת��Ϊ
* CA�����ʱ��
* ͬ�������ʱ�� ��2000-1-1Ϊ0��ÿ��һ���1
*Parameters	
*	  wdate
*		[in]	ͬ�������ʱ������
*	  pstDate
*            [out]  ת�����ʱ���ʾ
**************************************************************************/
void CS_CA_ChangeTFDate(WORD wdate, T_CaDate *pstDate);

#define MAX_EMAIL_TITLE_LEN	     (30)			/*�ʼ�����ĳ���*/
#define MAX_EMAIL_NUM            (100)			/*�����б��������ʼ�����*/
#define MAX_EMAIL_CONTENT_LEN    (160)			/*�ʼ����ݵĳ���*/

typedef struct _CaEmailContent
{
	BYTE		m_acEmailContent[MAX_EMAIL_CONTENT_LEN+1];	/*Email������*/
}T_CaEmailContent;

typedef struct _CaEmailTitle                       
{
	DWORD		m_dwEmailID;            						/*Email ID  							  */
	BYTE		m_cNewEmail;            						/*0 �������ʼ� 1�����ʼ�	  */
	BYTE   		m_acEmailHead[MAX_EMAIL_TITLE_LEN+1];  		/*�ʼ����⣬�Ϊ30		  */
	WORD 		m_wImportance;								/*��Ҫ��, 0����ͨ��1����Ҫ */
	T_CaDate	m_dwCreateTime;          						/*EMAIL������ʱ��				  */
}T_CaEmailHead;

typedef enum
{
	EMAIL_HIDE_ICON,									/*�����ʼ�֪ͨͼ��*/
	EMAIL_NEW_ICON,									/*���ʼ�֪ͨ����ʾ���ʼ�ͼ��*/
	EMAIL_FULL_ICON									/*���̿ռ�������ͼ����˸��*/	
}T_CaEmailIconType;

/*
typedef struct _CaEmailNotify
{
	T_CaEmailIconType	m_stShowType;
	DWORD				m_dwEmailId;
}T_CaEmailNotify;
*/

typedef enum _CaEmailSort					/*�ʼ�����ķ�ʽ*/
{
	CA_EMAIL_TIME_DESC,							/*����ʱ��������ں���*/	
	CA_EMAIL_TIME_ASC,							/*����ʱ���������ǰ��*/	
	CA_EMAIL_READSTATUS_DESC,					/*�Ѷ������ں���*/
	CA_EMAIL_READSTATUS_ASC,						/*�Ѷ�������ǰ��*/
	CA_EMAIL_COMSTATUS_ASC,						/*�Ѷ����Ҵ���ʱ���������ǰ��*/
	CA_EMAIL_COMSTATUS_DESC,						/*�Ѷ����Ҵ���ʱ��������ں���*/
	
	CA_EMAIL_SORT_MAX
}T_CaEmailSortStyle;
	
/**************************************************************************
* Function Name: CS_CA_GetEmailTotalNum
 *  ��ȡ�ʼ�����
*Parameters	
*		EmailNum: �ִ��ʼ���
*		EmptyNum: ʣ���ʼ��ռ�
 *Return Values
*		SUCCESS:	�ɹ�
*		FAILURE:		ʧ��
**************************************************************************/
INT CS_CA_GetEmailTotalNum(BYTE* pcEmailNum, BYTE* pcEmptyNum);

/**************************************************************************
* Function Name: CS_CA_GetEmailTitle
 *  ��ȡ�ʼ�����
*Parameters	
*	  nIndex
*		[in]	���(��1��ʼ)
*	  pstEmail
*            [out]  �ʼ�����
 *Return Values
*	  CA_RTN_OK : �ɹ� 
*	  CA_RTN_POINTER_INVALID: ָ��Ϊ��
*	  FAILURE: ��Ŵ���
**************************************************************************/
WORD CS_CA_GetEmailTitle(INT nIndex, T_CaEmailHead *pstEmail);

/**************************************************************************
* Function Name: CS_CA_GetEmailContent
 *  ��ȡĳһ���ʼ�����
*Parameters	
*	  nIndex
*		[in]	�ʼ����(��1��ʼ)
*	  pstEmailContent
*            [out]  �ʼ�����
 *Return Values
*	  CA_RTN_OK : ��ȡ�ɹ�  
*	  CA_RTN_POINTER_INVALID: ָ��Ϊ��
*	  CA_RTN_DATA_NOT_FIND:  ����δ�ҵ�
**************************************************************************/
WORD CS_CA_GetEmailContent(INT nIndex, T_CaEmailContent* pstEmailContent);

/**************************************************************************
* Function Name: CS_CA_GetEmailContentById
 *  �����ʼ�ID��ȡһ���ʼ�����
*Parameters	
*	  dwEmailId
*		[in]	�ʼ�ID
*	  pstEmailContent
*            [out]  �ʼ�����
 *Return Values
*	  CA_RTN_OK : ��ȡ�ɹ�  
*	  CA_RTN_POINTER_INVALID: ָ��Ϊ��
*	  CA_RTN_DATA_NOT_FIND:  ����δ�ҵ�
**************************************************************************/
WORD CS_CA_GetEmailContentById(DWORD dwEmailId, T_CaEmailContent* pstEmailContent);

/**************************************************************************
* Function Name: CS_CA_DelEmail
 *  ɾ��һ���ʼ�
*Parameters	
*	  nIndex
*		[in]	�ʼ����(0:ȫ��ɾ��; >0: ɾ����Ӧ����ʼ�)
 *Return Values
*	   ��
**************************************************************************/
void    CS_CA_DelEmail(INT nIndex);

/**************************************************************************
* Function Name: CS_CA_SortEmail
 *  ���ʼ���������
*Parameters	
*	  eSortStyle
		[in]	����ʽ
 *Return Values
*	   ��
**************************************************************************/
void CS_CA_SortEmail( T_CaEmailSortStyle eSortStyle);

#define		MAX_OSD_LEN		180		/*OSD���ݵ���󳤶�*/

typedef enum _CaOsdStyle
{
	CA_OSD_TOP,					/*����Ļ�Ϸ���ʾ*/
	CA_OSD_BOTTOM
}T_CaOsdStyle;

typedef struct _CaOsd
{
	T_CaOsdStyle 	m_eStyle;
	BYTE			m_acMessage[MAX_OSD_LEN];
}T_CaOsd;

/**************************************************************************
* Function Name: CS_CA_StartStockPlay
*  ������Ʊ����
* Parameters	
*	  pstServiceInfo[IN]:

* Return Values
*	   ��
**************************************************************************/
INT CS_CA_StartStockPlay(T_CaFtaService* pstServiceInfo);

/**************************************************************************
* Function Name: CS_CA_StartStockPlay
*  �˳���Ʊʱ����
* Parameters	
*	  pstServiceInfo[IN]:

* Return Values
*	   ��
**************************************************************************/
INT CS_CA_StopStockPlay(T_CaFtaService* pstServiceInfo);


#ifdef  __cplusplus
}
#endif

#endif //_CS_TFCA_INTERFACE_H_

