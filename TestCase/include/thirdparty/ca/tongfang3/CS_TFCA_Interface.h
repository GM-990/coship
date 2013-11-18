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
* - Revision 1.1  2006/3/23
*   Programmer:Xiejianghua
*   Create.
*   Revision 1.2 2007/11/07
*   Programmer:pengshuangquan
*   Added & Modify
*   Revision 1.3 2008/08/01
*   Programmer:wansuanlin
*   Modify
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
	//�����ʼ�ͼ����ʾ(T_CaEmailIconType������ʾ��ʽ)
	CA_COMMAND_NEW_EMAIL,			//0  dwParm2 = (DWORD)T_CaEmailIconType
	
	//ָ��
	CA_COMMAND_FINGERPRINT,			// 1 dwParm2 = (DWORD)CardId

	//��ʾOSD��Ϣ
	CA_COMMAND_SHOW_OSD,			// 2 dwParm2 = (DWORD)&T_CaOsd
	
	//����OSD��Ϣ
	CA_COMMAND_HIDE_OSD,			// 3 dwParm2 = (WORD)T_CaOsdStyle

	//IPPV������Ϣ
	CA_COMMAND_IPPV_NOTIFY,			// 4 dwParm2 = (DWORD)&T_CaIppvNotify
	
	//ȡ��IPPV������ʾ
	CA_COMMAND_IPPV_CANCEL,			// 5 dwParm2 = 0
	
	//wOperaterID!=0ʱ��ʾ:"ĸ�����ݶ�ȡ�ɹ�������ӿ�"; wOperaterID==0ʱ��ʾ"ĸ�����ݶ�ȡʧ��"  
	CA_COMMAND_CHILD_INFO,			//6 dwParm2 = wOperaterID
	
	//Ƶ������
	CA_COMMAND_LOCK_SERVICE,		// 7 dwParm2=&T_CaLockServiceParam
	
	//Ƶ������
	CA_COMMAND_UNLOCK_SERVICE,		// 8 dwParm2 = NULL
	
	//������Ȩ��ͼ��(E_DeTitileStatus������ʾ��ʽ)
	CA_COMMAND_DETITLE_NOTIFY,		// 9 dwParm2 = E_DeTitileStatus 
	
	//ͬ���������ݲ�����
	CA_COMMAND_ENTITLE_CHANGE,		// 10 Reserve
	
	//��ʾ��������
	CA_COMMAND_SHOW_PROGRESS,		// 11 dwParm2 = &T_PrggressStrip
	
	//
	CA_COMMAND_MAX
}T_CaCommand;

typedef enum _CaAlarm
{	
	CA_ALARM_CANCEL = 0,				/* 0 ȡ����ǰ����ʾ*/
	CA_ALARM_BADCARD,					/* 1 �޷�ʶ��*/
	CA_ALARM_EXPICARD,					/* 2 ���ܿ����ڣ�������¿�*/
	CA_ALARM_INSERTCARD,				/* 3 ���Ž�Ŀ����������ܿ�*/
	CA_ALARM_NOOPER,					/* 4 ���в����ڽ�Ŀ��Ӫ��*/
	CA_ALARM_BLACKOUT,					/* 5 ��������*/
	CA_ALARM_OUTWORKTIME,				/* 6 ��ǰʱ�α��趨Ϊ���ܹۿ�*/
	CA_ALARM_WATCHLEVEL,				/* 7 ��Ŀ��������趨�Ĺۿ�����*/
	CA_ALARM_PAIRING,					/* 8 ���ܿ��뱾�����в���Ӧ*/
	CA_ALARM_NOENTITLE,					/* 9 û����Ȩ*/
	CA_ALARM_DECRYPTFAIL,				/* 10 ��Ŀ����ʧ��*/
	CA_ALARM_NOMONEY,					/* 11 ���ڽ���*/
    CA_ALARM_ERRREGION,					/* 12 ������ȷ*/
	CA_ALARM_NEEDFEED_TYPE,				/* 13 �ӿ���Ҫ��ĸ����Ӧ�������ĸ��*/
	CA_ALARM_ERRCARD_TYPE,				/* 14 ���ܿ�У��ʧ�ܣ�����ϵ��Ӫ��*/
	CA_ALARM_UPDATE_TYPE,				/* 15 ���ܿ������У��벻Ҫ�ο����߹ػ�*/
	CA_ALARM_LOWCARDVER_TYPE,			/* 16 ���������ܿ�*/
	CA_ALARM_VIEWLOCK_TYPE,				/* 17 ����Ƶ���л�Ƶ��*/
	CA_ALARM_MAXRESTART_TYPE,			/* 18 ���ܿ���ʱ���ߣ���5���Ӻ����¿���*/
	CA_ALARM_FREEZE_TYPE,				/* 19 ���ܿ��Ѷ��ᣬ����ϵ��Ӫ��*/
	CA_ALARM_CALLBACK_TYPE,				/* 20 ���ܿ�����ͣ����ش����Ӽ�¼����Ӫ��*/
	CA_ALARM_STBLOCKED_TYPE,            /* 21 ������������*/
	CA_ALARM_STBFRESS_TYPE,             /* 22 �����б�����*/

	//tf add in 0x0424 version
	CA_ALARM_CURTAIN_CANNOTPREVIEW,		/*23������Ŀ������Ԥ���׶�*/
	CA_ALARM_CARDUPGRADING, 			/*24�������Կ�������...*/
	CA_ALARM_CARDUPGRADE_FAIL,			/*25�������Կ�����ʧ�ܣ��������ͨѶģ��*/
	CA_ALARM_CARDUPGRADE_SUCCESS,		/*26�������Կ����Գɹ�*/
	CA_ALARM_NOOPER_INCARD,				/*27���в�������ֲ�ⶨ����Ӫ��*/

	//add by lgc 2013.04.30 
	TFCA_ALARM_CURTAIN_CANCLE,  /*28ȡ���߼�Ԥ����ʾ*/
	TFCA_ALARM_CURTAIN_OK,  /*29�߼�Ԥ����Ŀ��������*/
	TFCA_ALARM_CURTAIN_TOTTIME_ERROR, /*30�߼�Ԥ����Ŀ��ֹ���ܣ��Ѿ��ﵽ�ܹۿ�ʱ��*/			
	TFCA_ALARM_CURTAIN_WATCHTIME_ERROR, /*31�߼�Ԥ����Ŀ��ֹ���ܣ��Ѿ��ﵽWatchTime����*/
	TFCA_ALARM_CURTAIN_TOTCNT_ERROR, /*32�߼�Ԥ����Ŀ��ֹ���ܣ��Ѿ��ﵽ������ۿ�����*/
	TFCA_ALARM_CURTAIN_ROOM_ERROR, /*33�߼�Ԥ����Ŀ��ֹ���ܣ��߼�Ԥ����Ŀ��¼�ռ䲻��*/
	TFCA_ALARM_CURTAIN_PARAM_ERROR,/*34�߼�Ԥ����Ŀ��ֹ���ܣ���Ŀ��������*/
	TFCA_ALARM_CURTAIN_TIME_ERROR, /*35�߼�Ԥ����Ŀ��ֹ���ܣ����ݴ���*/



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

typedef enum _DeTitileStatus
{
	CA_DETITLE_ALL_READED,      /*ȫ���Ѷ� ,      ��Ӧ����:����ͼ��*/
	CA_DETITLE_RECEIVED,        /*�½��շ���Ȩ��, ��Ӧ����:��ʾͼ��*/
	CA_DETITLE_SPACE_SMALL,     /*�洢���� ,      ��Ӧ����:ͼ����˸*/
	CA_DETITLE_IGNORE,          /*������   */
	CA_DETITLE_MAX
}E_DeTitileStatus;

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
	BYTE		m_cCanTape;             /*�Ƿ��¼��*/
	WORD		m_wBeginDate;			/*��ʼʱ��*/
	WORD		m_wExpriData;			/*����ʱ��*/
}T_CaEntitle;

typedef struct _CaServiceEntitles		/*������Ȩ��Ϣ*/
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

typedef struct _PrggressStrip
{
	BYTE m_ucType;					            /* 1�������ݽ�����(������),2 ���ܿ�������*/
	BYTE m_ucProgress;				            /* �������Ȱٷֱ�*/	
}T_PrggressStrip;					            /*��ʾ���������� */
	
typedef struct _WorkingTime		   
{
	BYTE m_ucHour;
	BYTE m_ucMiniter;
	BYTE m_ucSecond;
}T_WorkingTime;                                 /*�������ܿ�����ʱ��*/

#define MAX_PAIR_NUM   (5)
#define PAIR_ELEM_LEN  (6)

typedef struct _PairElem
{
	BYTE m_acStdId[PAIR_ELEM_LEN];
}T_PairElem;                                   /*���������к�*/

typedef struct _PairList
{
	BYTE       m_ucListNum;						/*�����ܿ���ԵĻ����и���*/
	T_PairElem m_aucListBuff[MAX_PAIR_NUM];     /*�����ж�Ӧ�����к�*/
}T_PairList;

#define MAX_DETITLE_NUM (5)

typedef struct _DetitleInfo
{
	DWORD m_dwDetitleNum;                         /*����Ȩ����*/
	DWORD m_adwDetitleBuff[MAX_DETITLE_NUM];
}T_DetitleInfo;                                 /*����Ȩ��*/

typedef struct _EntitleInfo
{
	DWORD m_dwEntitleNum;
	DWORD m_adwEntitleBuff[MAX_ENTITL_NUM];//[MAX_DETITLE_NUM];
}T_EntitleInfo;                                 /*��Ȩ��*/

typedef enum _PairStatus
{
	E_PAIR_NEEDED_CHILD_CARD,		            /*������ӿ�*/
	E_PAIR_NEEDED_PARENT_CARD,                  /*�����ĸ��*/
	E_PAIR_FEED_SUCCESS,                        /*��ĸ��ι���ɹ�*/
	E_PAIR_FEED_NOT_NEED,						/*�ӿ�������Ҫι��*/
	E_PAIR_MAX
}E_PairStatus;	                                /*ι��״̬*/


#define MAX_PARENTCARDSN_LEN (17)
typedef struct _CaChildInfo
{
    BOOL        m_bIsChild;                     /*TRUE:�ӿ�FALSE:ĸ��*/
    DWORD       m_dwFeedPeri;                   /*ι������*/
    T_CaDate    m_stLastFeedTime;               /*�ϴ�ι��ʱ��*/
	CHAR        m_acParentCardSN[MAX_PARENTCARDSN_LEN];  /*ĸ������*/
	BOOL        m_bIsCanBeFeed;					/*�ӿ��ڵ�ǰʱ�����Ƿ��ܱ�ι����false���ӿ�ι��ʱ��δ�������ܱ�ι����true�����Ա�ι��*/
	BOOL        m_bNewVersion;                   /*TRUE:m_acParentCardSN��m_bIsCanBeFeed��Ч��FALSE:m_acParentCardSN��m_bIsCanBeFeed��Ч*/
}T_CaChildInfo;                                /*��ĸ��״̬*/

#define CA_MAXLEN_SMC_SN                (16)

//----------------CA ����ֵ--------
#define CA_RTN_FAILURE                  (0xffff)
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
#define	CA_RTN_CARD_NOTSUPPORT		    (0x0F)	/*- ��ǰ���ܿ���֧�ִ˹���*/
#define	CA_RTN_DATA_ERROR				(0x10)	/*- ���ݴ������ܿ��ܾ�*/
#define	CA_RTN_FEEDTIME_NOT_ARRIVE		(0x11)	/*- ι��ʱ��δ�����ӿ����ܱ�ι��*/
#define	CA_RTN_CARD_TYPEERROR			(0x12)	/*- ��ĸ��ι��ʧ�ܣ��������ܿ����ʹ���*/

#define CA_RTN_READFAILURE				(0xf0)	/*-��ȡĸ������ʧ��*/
#define CA_RTN_FEEDFAILURE				(0xf1)	/*-ι��ʧ��*/

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
* Function Name: CSCAGetPlatFormID
*   ��ȡƽ̨ID
*
*Parameters	
*	  pwPlatformID
*            [out] ���ƽ̨ID
*       
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CSCAGetPlatFormID(WORD* pwPlatformID);


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
*    pcCardId[in]:���ַ�������ʽ�������ܿ����ţ�
*    ����ΪCA_MAXLEN_SMC_SN +1
*         
*Return Values
*	  CA_RTN_OK : 		 		��ȷ����  
*	  CA_RTN_POINTER_INVALID: 	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	�����ܿ�����Ч��
**************************************************************************/
INT	CS_CA_GetSCCardID(char* pcCardId);

/**************************************************************************
* Function Name: CS_CA_IsPaired
*   �жϻ������뿨�Ƿ������
*Parameters	 
*	pstPairList[out]:�����ܿ��ͼ�����������ԣ�����Ի����е����к�
*	������ֵΪCA_RTN_OK����CA_RTN_CARD_PAIROTHER������Ч������������
*Return Values
*	  CA_RTN_OK:��ǰ������Ӧ,����дpstPairList
*	  CA_RTN_CARD_INVALID:	 �����ܿ�����Ч��
*	  CA_RTN_CARD_NOPAIR:���ܿ�û�ж�Ӧ�κλ����������û�����Ӧ
*	  CA_RTN_CARD_PAIROTHER:���ܿ��Ѿ���Ӧ����������,����дpstPairList
**************************************************************************/
WORD CS_CA_IsPaired(T_PairList * pstPairList);

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
*	  pstStartTime
*            [out]  ��ʼʱ��
*	  pstEndTime
*		[out] ����ʱ��
 *Return Values
*	  CA_RTN_OK : 				��ȷ����  
*	  CA_RTN_POINTER_INVALID: 	ָ��Ϊ��
*	  CA_RTN_CARD_INVALID:  	�����ܿ�����Ч��
*	  CA_RTN_UNKNOW:			δ֪����
**************************************************************************/
WORD CS_CA_GetWorkTime(T_WorkingTime* pstStartTime, T_WorkingTime* pstEndTime);

/**************************************************************************
* Function Name: CS_CA_SetWorkTime
 *  ���ù���ʱ��
*Parameters	
*	  dwPinCode
*		[in]	������
*	  pstStartTime
*            [in]  ��ʼʱ��
*	  pstEndTime
*		[in] ����ʱ��
 *Return Values
*	 CA_RTN_OK : 		    			��ȷ����  
*	 CA_RTN_POINTER_INVALID:    		ָ��Ϊ��
*	 CA_RTN_CARD_INVALID:  	    		�����ܿ�����Ч��
*	 CA_RTN_UNKNOW:		    		δ֪����
*	 CA_RTN_PIN_INVALID:	    			PIN�����
*	 CA_RTN_PIN_LOCK:	    			PIN���ѱ�����
**************************************************************************/
WORD CS_CA_SetWorkTime(DWORD dwPinCode,T_WorkingTime* pstStartTime, T_WorkingTime* pstEndTime);

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

#define     MAX_SLOT_NUM		20		/*���洢�����Ǯ����*/
typedef struct _CaSlotId				/*Ǯ��ID*/
{
	WORD	m_wSlotNum;
	BYTE	m_acSlotIds[MAX_SLOT_NUM];
}T_CaSlotId;

typedef struct _CaSlotInfo                       /*Ǯ����Ϣ*/
{
	DWORD	m_wCreditLimit;                      /*���ö�*/
	DWORD	m_wBalance;                          /*�ѻ��ĵ���*/
}T_CaSlotInfo;

#define  MAX_PRICE_NUM  (2)                      /*����IPPV�۸����*/

typedef struct _CaIppvPrice                      /*IPPV��Ŀ�۸�*/
{
	BYTE			m_cPriceCode;                /* �۸�����0: ֻ���տ�,����¼��; 1:���տ�,��¼��*/
	WORD			m_wPrice;			         /*�۸����*/
}T_CaIppvPrice;

typedef struct _CaIppvBuyInfo                  /*IPPV������Ϣ*/
{
	WORD			m_wOperatorId;               /*��Ӫ��ID*/
	BYTE			m_cSlotID;	                 /*Ǯ��ID*/
	DWORD 			m_dwProductId;               /*��Ŀ��ID*/
	BYTE			m_cPriceNum;                 /*��Ŀ�۸����*/
	T_CaIppvPrice  	m_stPrice[MAX_PRICE_NUM];    /*��Ŀ�۸�*/
	WORD			m_wExpiredDate;              /*��Ŀ����ʱ��*/
	DWORD			m_dwRemainCredit;             /*Ǯ��ʣ�����*/
}T_CaIppvBuyInfo;

typedef struct _CaIppvNotify                     /*IPPV����֪ͨ*/
{
	BYTE 				m_cMessageType;	         /*��ʾ����0: ���Ԥ���׶�.1: �շѽ׶�2  IPPT ����׶�*/
	WORD				m_wEcmPid;               /*����ʾ�Ի����־*/
	T_CaIppvBuyInfo		m_stIppvProgram;         /*Ippv��Ŀ��Ϣ*/
}T_CaIppvNotify;

#define		MAX_IPPVPROG_NUM       (300)	     /*���ܿ��������IPPV��Ŀ�ĸ���*/

typedef struct _CaIppvInfo					
{
	DWORD 			m_dwProductID;               /*��Ŀ��ID*/
	BYTE			m_cBookEdFlag;               /*��Ʒ״̬1: BOOKING 3: VIEWED*/ 
	BYTE			m_cCanTape;                  /*�Ƿ����¼��1������¼��0��������¼��*/
	WORD			m_wPrice;                      /*��Ŀ�۸�*/
	WORD			m_wExpiredDate;              /*��Ŀ����ʱ��*/
}T_CaIppvInfo;

typedef struct _CaIppvProgInfo                   /*IPPV ��Ŀ��Ϣ*/
{
	WORD			m_wProgramNum;
	T_CaIppvInfo    m_astIppvInfo[MAX_IPPVPROG_NUM];
}T_CaIppvProgInfo;

#define MAX_COMPONENT_NUM (5)

typedef struct _LockComponent
{
	WORD		m_CompType; 		/*�������1:MPEG_TYPE_VIDEO_11172  3:MPEG_TYPE_AUDIO_11172*/   
	WORD		m_wCompPID; 		/*���PID*/  
}T_LockComponent;

typedef struct _CaLockServiceParam
{
	BYTE m_wFecOuter;
	BYTE m_wFecInner;
	DWORD m_dwFrequency;
	DWORD m_wModulation;//0	Reserved, 1	QAM16, 2 QAM32, 3 QAM64, 4 QAM128, 5 QAM256, 6��255	Reserved
	DWORD m_wSymbolrate;
	DWORD m_wPcrPID;
	DWORD m_wComponentNum;
	T_LockComponent m_stComponent[MAX_COMPONENT_NUM];
}T_CaLockServiceParam;
  
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
	DWORD		m_dwEmailID;                                /*Email ID  */
	BYTE		m_cNewEmail;                                /*0 �������ʼ� 1�����ʼ� */
	BYTE   		m_acEmailHead[MAX_EMAIL_TITLE_LEN+1];  		/*�ʼ����⣬�Ϊ30 */
	WORD 		m_wImportance;								/*��Ҫ��, 0����ͨ��1����Ҫ */
	T_CaDate	m_dwCreateTime;                             /*EMAIL������ʱ��*/
}T_CaEmailHead;

typedef enum
{
	EMAIL_HIDE_ICON,							    /*�����ʼ�֪ͨͼ��*/
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

typedef enum _CaEmailSort 					    /*�ʼ�����ķ�ʽ*/
{
	CA_EMAIL_TIME_DESC,							/*����ʱ��������ں���*/	
	CA_EMAIL_TIME_ASC,							/*����ʱ���������ǰ��*/	
	CA_EMAIL_READSTATUS_DESC,                   /*�Ѷ������ں���*/
	CA_EMAIL_READSTATUS_ASC,                    /*�Ѷ�������ǰ��*/
	CA_EMAIL_COMSTATUS_ASC,						/*�Ѷ����Ҵ���ʱ���������ǰ��*/
	CA_EMAIL_COMSTATUS_DESC,                    /*�Ѷ����Ҵ���ʱ��������ں���*/
	
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

#define		MAX_OSD_LEN		184		/*OSD���ݵ���󳤶�*/

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

/**************************************************************************
* Function Name: CS_CA_GetDetitleInfo
*  ��ȡ����Ȩ��
* Parameters	
*	wOperatorsId[in]:��Ӫ��ID
*	pdwDetileNum[out]:����Ȩ��Ϣ
* Return Values
*	CA_RTN_OK �ɹ�
*	CA_RTN_POINTER_INVALID ָ��Ϊ��
*	CA_RTN_CARD_INVALID ���ܿ����ڻ����л�����Ч��
*	CA_RTN_DATA_NOT_FIND û���ҵ���Ӧ����Ӫ��
**************************************************************************/

WORD CS_CA_GetDetitleInfo(WORD wOperatorsId,T_DetitleInfo* pstDetileInfo);

/**************************************************************************
* Function Name: CS_CA_DelDetitleInfo
*  ɾ������Ȩ��Ϣ
* Parameters	
*	wOperatorsId[in]:��Ӫ��ID
*	dwDetileNum[in]:����Ȩ ID
* Return Values
*	   TRUE : �����ɹ�
*	   FALSE: ����ʧ��
**************************************************************************/

BOOL CS_CA_DelDetitleInfo(WORD wOperatorsId,DWORD dwDetileNum);

/**************************************************************************
* Function Name: CS_CA_GetEntitleInfo
* ��ȡ��Ȩ��Ϣ
* Parameters	
*	  wOperatorsId[IN]:��Ӫ��ID
*	  pdwEntileNum[out]:	��ȨID
* Return Values
*	CA_RTN_OK �ɹ�
*	CA_RTN_POINTER_INVALID ָ��Ϊ��
*	CA_RTN_CARD_INVALID ���ܿ����ڻ����л�����Ч��
*	CA_RTN_DATA_NOT_FIND û���ҵ���Ӧ����Ӫ��
**************************************************************************/
WORD CS_CA_GetEntitleInfo(WORD wOperatorsId,T_EntitleInfo* pstEntileInfo);

/**************************************************************************
* Function Name: CS_CA_GetPairCardInfo
*  ��ȡָ����Ӫ�����ܿ�����ĸ����Ϣ
* Parameters	
*	  	wOperatorsId[in]:  ��Ӫ��ID
*		pstChildInfo[out]: ���ܿ���Ϣ
* Return Values
*		CA_RTN_OK �ɹ�
*		CA_RTN_CARD_INVALID	 ���ܿ���Ч
*		CA_RTN_CARD_NOTSUPPORT:��ǰ���ܿ���֧�ִ˹���
*		CA_RTN_DATA_NOT_FIND:	 û���ҵ���Ҫ������
**************************************************************************/
WORD CS_CA_GetPairCardInfo(WORD wOperatorsId,T_CaChildInfo* pstChildInfo );

/**************************************************************************
* Function Name: CS_CA_SetParentPair
*  ������ĸ�����
* Parameters	
*	  wOperatorsId[in] :  ��Ӫ��ID
*	  bIsChild[in]: ���ܿ�����,TRUE:�ӿ� FALSE: ĸ��
*	  eStatus[out]:��ǰ����״̬
*		E_PAIR_NEEDED_CHILD_CARD:������ӿ�
*		E_PAIR_NEEDED_PARENT_CARD:�����ĸ��
*		E_PAIR_FEED_SUCCESS:��ĸ��ι���ɹ�
* Return Values
*		CA_RTN_OK �ɹ�
*		CA_RTN_CARD_INVALID	 ���ܿ���Ч
*		CA_RTN_CARD_NOTSUPPORT:��ǰ���ܿ���֧�ִ˹���
*		CA_RTN_DATA_NOT_FIND:	 û���ҵ���Ҫ������
*		CA_RTN_DATA_ERROR: ���ݴ������ܿ��ܾ�
*		CA_RTN_FEEDTIME_NOT_ARRIVE:ι��ʱ��δ�����ӿ����ܱ�ι��
*		CA_RTN_CARD_TYPEERROR:  ��ĸ��ι��ʧ�ܣ��������ܿ����ʹ���
*
*Notice:
*		Return Values Ϊ��һ���ȼ������Return Values ������CA_RTN_OK
*	����ʾ���ش������͡����Return Values ����CA_RTN_OK �ټ��
*	eStatus ���ͣEE_PAIR_FEED_SUCCESS, E_PAIR_MAX����ʾ
**************************************************************************/
WORD CS_CA_SetParentPair(WORD wOperatorsId, BOOL bIsChild, E_PairStatus* eStatus );


/**************************************************************************
* Function Name: CSCASetParentPair
*  ������ĸ����ԵĽӿ�(��Ҫ�ж��ӿ�ι��ʱ���Ƿ��ѵ�),�����ӵĽӿڣ�����Ŀ��Ҫ������ӿڣ�
*   ����Ҫʹ�ýӿ�CS_CA_SetParentPair
* Parameters	
*	  wOperatorsId[in] :  ��Ӫ��ID
*	  bIsChild[in]: ���ܿ�����,TRUE:�ӿ� FALSE: ĸ��
*       bIsCanBeFeed:��ǰ�ӿ��Ƿ��ܱ�ι��,TRUE:�ܹ���ι��, FALSE: ι��ʱ��δ�����ܱ�ι��
*	  eStatus[out]:��ǰ����״̬
*		E_PAIR_NEEDED_CHILD_CARD:������ӿ�
*		E_PAIR_NEEDED_PARENT_CARD:�����ĸ��
*		E_PAIR_FEED_SUCCESS:��ĸ��ι���ɹ�
* Return Values
*		CA_RTN_OK �ɹ�
*		CA_RTN_CARD_INVALID	 ���ܿ���Ч
*		CA_RTN_CARD_NOTSUPPORT:��ǰ���ܿ���֧�ִ˹���
*		CA_RTN_DATA_NOT_FIND:	 û���ҵ���Ҫ������
*		CA_RTN_DATA_ERROR: ���ݴ������ܿ��ܾ�
*		CA_RTN_FEEDTIME_NOT_ARRIVE:ι��ʱ��δ�����ӿ����ܱ�ι��
*		CA_RTN_CARD_TYPEERROR:  ��ĸ��ι��ʧ�ܣ��������ܿ����ʹ���
*
*Notice:
*		Return Values Ϊ��һ���ȼ������Return Values ������CA_RTN_OK
*	����ʾ���ش������͡����Return Values ����CA_RTN_OK �ټ��
*	eStatus ���ͣEE_PAIR_FEED_SUCCESS, E_PAIR_MAX����ʾ
**************************************************************************/
WORD CSCASetParentPair(WORD wOperatorsId, BOOL bIsChild, BOOL bIsCanBeFeed, E_PairStatus* eStatus);


/**************************************************************************
* Function Name: CS_CA_HaveNewDetitle
* ��������ʱ�����Ƿ����µķ���Ȩ����Ϣ
* Parameters	
*             ��
* Return Values
*		TRUE  : ���µķ���Ȩ��Ϣ������ʾ����Ȩͼ��
*		FALSE: û���µķ���Ȩ��Ϣ
**************************************************************************/
BOOL CS_CA_HaveNewDetitle(void);

/**************************************************************************
*Function Name:CS_CA_MMI_GetSmcStatus
* ��ȡ���ܿ���ǰ״̬
*
*Parameters 
*	pcSmcStatus[In/Out]:�洢���ܿ�״̬���������ֵΪ0����OUT��Ϊ1��IN
*
*Return Values: 
*	SUCCESS ��ȡ���ܿ���ǰ״̬�ɹ� 
*	FAILURE �����������
**************************************************************************/
INT CS_CA_MMI_GetSmcStatus(BYTE* pcSmcStatus);

/**************************************************************************
*Function Name:CS_CA_GetFlashAddress
* ��ȡͬ��CA   ����������Flash ��ַ
*Parameters 
*	pcSmcStatus[In/Out]:��
*
*Return Values: 
*һ������Block (64K) ����ʼ��ַ
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CS_CA_GetBackupFlashAddress
*  ��ȡͬ��CA  ������������Flash ��ַ(����������д��һ��������ͬһ��Block)
*Parameters 
*	pcSmcStatus[In/Out]:��
*
*Return Values: 
*һ������Block (64K) ����ʼ��ַ
**************************************************************************/
extern DWORD CS_CA_GetBackupFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CS_CA_GetStbInfo
* ��ȡ��ǰ�����е�ƽ̨�ź����к�
*Parameters 
*	pcSmcStatus[In/Out]:
*	pdwPlatformID [in]: ƽ̨ID, �̶�Ϊ0x1010;
*	pdwStbIDBCD [in]:   ���������кţ���ת��ΪBCD��. �������к�1234��Dec����ת��Ϊ0x1234��Hex�� 
*	Return Values: 
*	��
**************************************************************************/
extern void CS_CA_GetStbInfo(WORD* pdwPlatformID, ULONG* pdwStbIDBCD);

/**************************************************************************
* Function Name: CS_CA_ClearParentPairStatus
* �����ĸ�����ʱ�����״̬���˳���ĸ����Խ���ʱ����
* Parameters	
*             ��
* Return Values
*			  ��
**************************************************************************/
void CS_CA_ClearParentPairStatus(void);

/**************************************************************************
* Function Name: CSCAReadFeedDataFromParent
*  ��ĸ������, ��ǰ������ĸ��ʱ���ã���ȡĸ������Ҫι���ӿ�������
*  (�˽ӿ�Ϊ�м��Ϊ����㶫ʡ���������Ҫ���õĽӿ�,�����������Ҫ����)
* Parameters	
*	  	wOperatorsId[in]:  ��Ӫ��ID
*		pacData[out]:      ĸ������Ҫι���ӿ�������(����ʱbuffer��С���ٷ���256�ֽ�)
*       *wLen[out]:        ĸ������Ҫι���ӿ������ݵ�ʵ�ʳ���
* Return Values
*		CA_RTN_OK �ɹ�
*       CA_RTN_POINTER_INVALIDָ����Ч
*       CA_RTN_CARD_INVALID���ܿ���Ч
*       CA_RTN_CARD_NOTSUPPORT���ܿ���֧�ִ˹���
*       CA_RTN_DATA_NOT_FINDû���ҵ���Ҫ������
**************************************************************************/
WORD CSCAReadFeedDataFromParent(WORD wOperatorsId, BYTE* pacData, BYTE* ucLen);

/**************************************************************************
* Function Name: CSCAWriteFeedDataToChild
*  ��ĸ�����ܣ���ǰ�������ӿ�ʱ���ã��������Ϣд���ӿ�
*  (�˽ӿ�Ϊ�м��Ϊ����㶫ʡ���������Ҫ���õĽӿ�,�����������Ҫ����)
* Parameters
*       wOperatorsId[in]:  ��Ӫ��ID
*	  	pbyFeedData[in]:   Ҫд���ӿ�������buffer
*		byLen[in]:         Ҫд���ӿ�������ʵ�ʳ���
* Return Values
*		CA_RTN_OK �ɹ�
*       CA_RTN_POINTER_INVALIDָ����Ч
*       CA_RTN_CARD_INVALID���ܿ���Ч
*       CA_RTN_CARD_NOTSUPPORT���ܿ���֧�ִ˹���
*       CA_RTN_DATA_NOT_FINDû���ҵ���Ҫ������
**************************************************************************/
WORD CSCAWriteFeedDataToChild(WORD wOperatorsId, BYTE* pacFeedData, BYTE ucLen);

#ifdef  __cplusplus
}
#endif

#endif //_CS_TFCA_INTERFACE_H_

