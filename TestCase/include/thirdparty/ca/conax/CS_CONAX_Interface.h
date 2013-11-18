/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_CONAX_Interface.h  
* Author:      xiejianghua
* Date:        Dec 2005 
* Description: CONAX CA interface
*            
*******************************************************************************/

#include "udiplus_typedef.h"
#include "cs_cainterface.h"
#include "cs_cainterface_pvrext.h"

/*******************************************************************************
* 			                                  Revision History                                				     *
********************************************************************************
* - Revision 1.0  2005/12/31
*   Programmer:Xie jianghua
*   Create.
*
*- Revision 2.0 2007/4/17
*   Programmer:Xiejianghua
*   Refactored
*- Revision 3.0 2008/11/25
*   Programmer:heyingbo
*   Restructure
*- Revision 4.0 2010/11/29
*   Programmer:lihongqiang
*   Restructure
*******************************************************************************/
#ifndef _CS_CONAX_INTERFACE_H_
#define _CS_CONAX_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

//Memory Pairing mode
//#define VERIFIER_MODE		0

//Chipset Pairing mode
//#define CHIPSET_PAIRING		1


#define CWPK_WITH_LPK   (1)

#define	CA_ALARM		0   //  ALARM ��Ϣ����
#define	CA_COMMAND	    1   //  COMMAND ��Ϣ����
#define	CA_STATUS		2   //  STATUS ��Ϣ����

#define CA_MSG_BASE                  0    //conax��Ϣ��ʼֵ

#if defined(SUPPORT_MAXFP)    //AVG��Ŀ��ָ�Ƴ���Ҫ��ܴ�
#define CA_MAX_FINGERPRINT_LEN		256/*32*/    //ָ����Ϣ����󳤶�
#else
#define CA_MAX_FINGERPRINT_LEN		32/*32*/    //ָ����Ϣ����󳤶�
#endif

#define CA_MAX_LABEL_NAME_LEN       32    //������Ϣ����󳤶�
#define CA_MAX_DEBIT_NUM         	100   //DEBIT����¼��
#define CA_MAX_CREDIT_NUM         	100   //credit����¼��
#define CA_MAX_TEXT_SIZE			256   //����Ϣ��󳤶�
#define CA_MAX_ES_NUM				16    //���ɽ��ES����
#define CA_MAX_CASYSTEMID_NUM       256   //���CAϵͳID����
#define CA_MAX_CURRENCY_LEN			8     //CURRENCY��󳤶�

//��ĸ��
typedef enum
{
	RAT_G,                //for the family
	RAT_PG,               //with parental guidance
	RAT_A,                //over 18
	RAT_X,                //erotic
    RAT_XXX               //erotic//hyb@090817
}T_CaMaturityRating;

typedef struct
{
	DWORD		   m_dwYear; 
	DWORD		   m_dwMonth;
	DWORD		   m_dwDay;
	DWORD		   m_dwHour;
	DWORD		   m_dwMinute;
	DWORD		   m_dwSecond;
	DWORD		   m_dwCentisecond;
}T_CaDateTime;

typedef struct
{   
    WORD                m_wLanguage;        //language,should be displayed as a decimal number with 1-3 digits
    BYTE         		m_cInterfaceVer;    //interface version
    BYTE         		m_cNumOfSession;    //number of session,should be displayed as a decimal number
    BYTE			    m_cSoftVersion[20]; //software version
    BYTE                m_szCardId[16];     //card number, should be displayed as a decimal number, like" XXX XXXX XXXX - X"
    BYTE			    m_acChipId[16];     //chip id,display as 3-4-4 format,interval space.eg:123 4567 8901
    WORD                m_awCaSysId[CA_MAX_CASYSTEMID_NUM];//ca system id, each one should be displayed as a hexadecimal number with four digits
    BYTE                m_cCaSysIdNum;      //number of ca system id
} T_CaConaxInfo;

typedef struct
{
	WORD		m_wMinutesLeft;
	BYTE		m_acLabel[CA_MAX_LABEL_NAME_LEN];//program name
	DWORD		m_dwProductRef;                  //no need to display for the moment
}T_CaAcceptViewInfo;

typedef struct
{
	WORD       m_wPurseBalance;         // ��ǰʹ��Ǯ�������
	BYTE       m_cInsufficientTokens;   // 1: ��������
	BYTE       m_cTokensMode;           // 0: ppv per event       1: ppv per time
	DWORD      m_dwEventcost;           // ��Ŀ�۸�
	DWORD      m_dwEventTag;
	BYTE       m_acProgramLabel[CA_MAX_LABEL_NAME_LEN];// ��Ŀ���ƣ��ַ���
	BYTE	   m_acPurseLabel[CA_MAX_LABEL_NAME_LEN]; //Ǯ�����ƣ��ַ���
}T_CaPPVRequestInfo;

typedef struct
{
	DWORD 		m_dwProductRef; //no need to display for the moment
	float 		m_fPrice;       // ��Ŀ�۸�
    BYTE        m_acCurrency[CA_MAX_CURRENCY_LEN];
	BYTE  		m_acProductLabel[CA_MAX_LABEL_NAME_LEN];//����
}T_CaOrderInfo;

//�����¼
typedef struct
{ 
    T_CaDateTime	  m_stStart;        //  �ꡢ�¡�����Ч��������Ч
    T_CaDateTime	  m_stEnd;          //  �ꡢ�¡�����Ч��������Ч
    DWORD	          m_dwEntitlement;  //��ʶ������Ȩ��Ϣ
} T_CaAuthPeriod;

typedef struct
{
    BYTE                m_szLabel[CA_MAX_LABEL_NAME_LEN];//����
    WORD	            m_wRefID;           //��Ŀ����
    T_CaAuthPeriod   	m_astAuthPeriod[2]; //��Ȩ��Ϣ
}T_CaSubscriptionInfo;

//ppv Event ��¼
typedef struct
{   
    BYTE		   	m_szLabel[CA_MAX_LABEL_NAME_LEN];//����
    DWORD	   		m_dwEventID;                     //��Ŀ����
    T_CaDateTime	m_stStart;                       // �롢������Ч��������Ч
    T_CaDateTime	m_stEnd;                         // �롢������Ч��������Ч
    WORD           	m_MinutesOrCreditsLeft;          //ʣ��������
    WORD           	m_wAuthType;                     //0:time-limited access; 1:unlimited access; 2:credit-based access; 3:reserved
} T_CaPPVEventInfo;

typedef struct
{
	WORD			m_wPurseRef;//Ǯ������
	DWORD			m_dwBalance;//���
	BYTE			m_acLabel[CA_MAX_LABEL_NAME_LEN]; //����
}T_CaPurseInfo;

typedef struct
{
	DWORD			m_dwEventTag;
	DWORD			m_dwConsumedTokens;              //�����ѵĽ��
	BYTE			m_acLabel[CA_MAX_LABEL_NAME_LEN];//����
	T_CaDateTime	m_stLastAccessTime;              //������Ч��������Ч
}T_CaDebitStatus;

typedef struct
{
	DWORD		         m_dwTotalNum;                     //��¼����
	T_CaDebitStatus      m_stDebitStatus[CA_MAX_DEBIT_NUM];//���Ѽ�¼
}T_CaDebitInfo;

typedef struct
{
	DWORD		  	m_dwCreditTokens;                //����
	BYTE		  	m_acLabel[CA_MAX_LABEL_NAME_LEN];//����
}T_CaCreditStatus;

typedef struct
{
	DWORD		           m_dwTotalNum;                       //��Ϣ����
	T_CaCreditStatus       m_stCreditStatus[CA_MAX_CREDIT_NUM];//������Ϣ
}T_CaCreditInfo;

typedef struct
{
	BYTE		    m_cId;                     //��ϢID
	WORD		    m_wDuration;               //ÿ����ʾ����ʱ��
	BYTE		    m_cRepetition;             //��ʾ����
	WORD		    m_wInterval;               //��ʾ���ʱ��
										//conax�ĵ�˵������Ϣ�����ΧΪ:10s-2560s,ԭ�����ʱ������ΪBYTE�����ֻ֧��255��
										//Ϊ�˼��ݼ������255ʱ��������ʾ����Ϣ����ΪWORD����//hulan 2013.5.22
	T_CaDateTime    m_stShowTime;              //��ʼ��ʾʱ�䣬������Ч��������Ч
	BYTE		    m_cTextLen;                //���ݳ���
	BYTE		    m_acText[CA_MAX_TEXT_SIZE];//����Ϣ����
}T_CaMessage;

typedef struct
{
	BOOL			m_bRead;            //�Ѷ���ʶ
	BYTE			m_cMailId;          //�ʼ�ID
	T_CaDateTime	m_stShowFlagTime;   //FLAG��ʼ��ʾ(UTCʱ��),�ʼ�Ϊֱ����ʾ�ʼ�ͼ��
	T_CaDateTime	m_stReceiveTime;    //����ʱ��
	BYTE			m_cTextLen;         //���ݳ���
	BYTE			m_acText[CA_MAX_TEXT_SIZE]; //�ʼ�����
}T_CaEmail;

typedef struct
{
	WORD			m_wDuration;    //����ʱ��,��λΪ���루�ٷ�֮һ��)
	WORD			m_wPosition_x;  //���Ͻ�X����
	WORD			m_wPosition_y;  //���Ͻ�X����
	BYTE			m_cHeight;      //���Ͻ�Y����
	T_CaDateTime	m_stShowTime;   //�ꡢ�¡�����Ч��������Ч
	BYTE			m_acFingerText[CA_MAX_FINGERPRINT_LEN];//ָ������
	INT             m_nLength;
}T_CaFingerprint;

typedef enum
{	
	CA_COMMAND_NEW_EMAIL = CA_MSG_BASE,//0 from Emm/ECM dwPram2 = (DWORD)&T_CaMessage	
	CA_COMMAND_SHORT_MSG,              //1 from Emm/ECM dwPram2 = (DWORD)&T_CaMessage
	CA_COMMAND_FINGER_PRINT,           //2 from Emm/ECM dwPram2 = (DWORD)&T_CaFingerprint
	CA_COMMAND_CLEAN_EMAIL_FLAG,       //3 notify to clean the email icon 
    CA_COMMAND_RESET_STB_PIN,          //4 just for France island project
    CA_COMMAND_STOP_FINGER_PRINT,      //5 �����ϢӦ�ò�Ҫ�����ˣ�������ʾʱ�侫��ԭ����Ӧ���Լ�����
    CA_COMMAND_STOP_SHORT_MSG,         //6 stop short-msg,conax5(����conax5)��֧��,֮ǰ�汾���������Ϣ
    
	CA_COMMAND_MAX
}T_CaCommand;

//Ӧ�ô�������ƵCA_ALARM_CW_SET_SUCCESS��������ƵCA_ALARM_CW_LOST��������Ϣ����Ҫ����������Ƶ
typedef enum
{
	CA_ALARM_CANCEL = (CA_MSG_BASE+200),//200 used for cancel alarmmsg topbar except emailinco shortmsg fp
	CA_ALARM_OTHER,                     //201 "Other CA program"
	CA_ALARM_CARD_INSERT,               //202 "Card Checking�� "
	CA_ALARM_CARD_REMOVE ,              //203 "No card inserted"
	CA_ALARM_CARD_INCORRECT,            //204 "Incorrect card:x",if dwParm2 != 0, it means the time the banner will be displayed
	CA_ALARM_CARD_INCORRECT_CONAX ,     //205 "Incorrect Conax card"
	CA_ALARM_CARD_COMMUNICATION_ERROR,  //206 "Card problem - check card"
	CA_ALARM_NO_RIGHT,                  //207 "No access"
	CA_ALARM_PPV_PREVIEW,               //208 "PPV Preview"
	CA_ALARM_PPV_EXPIRED,		        //209 "PPV Expired"
	CA_ALARM_GEOGRAPHICAL_BLACKOUT ,    //210 "No access - geographical blackout"
	CA_ALARM_NO_ACCESS_TO_NETWORK,      //211 "No access - network block"
	CA_ALARM_ACCEPT_VIEW,               //212 start viewing dialog box, para2 = T_CaAcceptViewInfo
	CA_ALARM_ORDER_INFO,                //213 ordering dialog box, para2 = T_CaOrderInfo
	CA_ALARM_PPV_REQUEST,               //214 token access dialog box, para2 = T_CaPPVRequestInfo
	CA_ALARM_MATURITY_RATING,           //215 maturity lock dialog box,para2 is the program maturity level
    CA_ALARM_PPV_INSUFFICIENT_TOKENS,   //216 ppv insufficient tokens,  (conax5��Ӧ�ò����յ�����Ϣ) 
    CA_ALARM_SRV_FREE,                  //217 free service,Ӧ�ò���Ҫ����
    CA_ALARM_CONAX_SCRAMBLED,           //218 conax scramble service
    CA_ALARM_CW_SET_SUCCESS,            //219 open av;
    CA_ALARM_CARD_READY,                //220 smart card ok, no use to display,discard from conax5.0A(conax5��Ӧ�ò����յ�����Ϣ)   
    CA_ALARM_CANCEL_MAT,                //221 cancel maturity dlg,discard from conax5.0A(conax5��Ӧ�ò����յ�����Ϣ)
    CA_ALARM_CANCEL_PPV,                //222 cancel ppv dlg,discard from conax5.0A(conax5��Ӧ�ò����յ�����Ϣ)
    CA_ALARM_CANCEL_ORDER,              //223 cancel order dlg,discard from conax5.0A(conax5��Ӧ�ò����յ�����Ϣ)
    CA_ALARM_CANCEL_ACCEPT_VIEW,        //224 cancel accept view dlg,discard from conax5.0A(conax5��Ӧ�ò����յ�����Ϣ)
    CA_ALARM_CW_LOST,                   //225 close dwParm3: audio or video;conax5(����conax5)��֧��,֮ǰ�汾���������Ϣ
    
    CA_ALARM_MAX
}T_CaAlarm;

typedef enum 
{
	CA_STATUS_CARD_READY = (CA_MSG_BASE+300),//300 card ok, but needn't display
	CA_STATUS_CARD_INSERT ,                  //301 " Card Checking�� "
	CA_STATUS_CARD_REMOVE ,                  //302 "No card inserted"
	CA_STATUS_CARD_INCORRECT,                //303 "Incorrect card",if dwParm2 != 0, it means the time the banner will be displayed
	CA_STATUS_CARD_INCORRECT_CONAX,          //304 "Incorrect Conax card"
	CA_STATUS_CARD_COMMUNICATION_ERROR,      //305 "Card problem - check card"
	CA_STATUS_ORDER_CONNECTING,              //306 connecting, please wait...
	CA_STATUS_ORDER_BUSY,                    //307 sorry, please try again
	CA_STATUS_ORDER_FAILED,                  //308 order failed, Error code:'PI_STATUS'//para2=PI_STATUS, should be displayed in hexadecimal
	CA_STATUS_ORDER_CLOSE,                   //309 no use this command at the moment
	CA_STATUS_ORDER_ACCEPTED,                //310 order accepted
	CA_STATUS_MAX
}T_CaStatus;

typedef enum
{
    CA_EMAIL_NO_SEQ = 0,        //��˳��Ҫ��
    CA_EMAIL_TIME_ASC,          //����ʱ������ʼ�����ǰ��
    CA_EMAIL_TIME_DESC,         //����ʱ������ʼ����ں���
    CA_EMAIL_UNREAD_ASC,        //δ���ʼ�����ǰ��
    CA_EMAIL_UNREAD_DESC,       //δ���ʼ����ں���    
    CA_EMAIL_UNREAD_TIME_ASC,   //δ���ʼ���ʱ������ʼ�����ǰ��
    CA_EMAIL_UNREAD_TIME_DESC,  //δ���ʼ���ʱ������ʼ����ں���,��0����δ���������ʼ�
    CA_EMAIL_MAIL_ID_ASC,       //mail id��������  
    CA_EMAIL_MAIL_ID_DESC,      //mail id��������
    CA_EMAIL_SHOW_TIME_ASC,
    CA_EMAIL_MAX
}T_CaEmailSeq;

typedef enum _MENU_STATUS_EN    //conax5�汾֮ǰ��֧��,conax5�汾��(����conax5�汾)����ʹ��
{
	CA_MENU_MAINMENU_STATUS,    //DVB���˵�         
	CA_MENU_CAMENU_STATUS,      //CA�˵� 
	CA_MENU_MAX
}CONAX_MENU_STATUS;

/**************************************************************************
* Function Name: CS_CA_GetCaLibVer
*   ��ȡCA ģ��汾��
*
*Parameters	
*		pcCaVerBuf
*             [in] ��Ű汾�ŵ�Buffer
*		dwBufSize
*		 [in] Buffer�Ĵ�С( >= 16)
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetCaLibVer(BYTE *pcCaVerBuf, DWORD dwBufSize);

/**************************************************************************
* Function Name: CS_CA_GetConaxInfo
*   ��ȡCA��Ϣ
*
*Parameters	
*		pstCaReleaseInfo
*                  [out] CA��Ϣ�ṹ
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*note: example menu of "About Conax CA"
*    //////////////////////
*      About Conax CA
*Software version: XX.XX.XX ----from CS_CA_GetCaLibVer
*Interface version: 0x40
*Card number: 019 0000 100x - y
*Number of seeions: 3
*Language:44
*CA_SYS_ID: 0x0B00
*      ///////////////////////
**************************************************************************/
INT CS_CA_GetConaxInfo(T_CaConaxInfo *pstConaxInfo);

/**************************************************************************
* Function Name: CS_CA_GetSubscriptionTotalNum
*   ��ȡ�����¼����
*
*Parameters
*
*Return Values
*		�ɹ����ؼ�¼������ʧ�ܷ���FAILURE
**************************************************************************/
INT CS_CA_GetSubscriptionTotalNum(void);

/**************************************************************************
* Function Name: CS_CA_GetSubscriptionInfo
*   ��ȡָ����Ź����¼������
*
*Parameters	
*	       dwRecordIndex
*                   [in]   ���(��1��ʼ)
*          pstSubRecord
*                   [out]  ����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetSubscriptionInfo(DWORD dwRecordIndex, T_CaSubscriptionInfo *pstSubRecord);

/**************************************************************************
* Function Name: CS_CA_GetPpvEventTotalNum
*   ��ȡPPV EVENT ��¼����
*
*Parameters	
*	
*Return Values
*		�ɹ����ؼ�¼������ʧ�ܷ���FAILURE
**************************************************************************/
INT CS_CA_GetPpvEventTotalNum(void);

/**************************************************************************
* Function Name: CS_CA_GetPpvEventInfo
*   ��ȡָ�����PPV EVENT  ��¼������
*
*Parameters	
*	       dwRecordIndex
*                    [in]  ���(��1��ʼ)
*          pstPpvRecord
*                    [out] ����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetPpvEventInfo(DWORD dwRecordIndex, T_CaPPVEventInfo* pstPpvRecord);

/**************************************************************************
* Function Name: CS_CA_GetPurseTotalNum
*   ��ȡǮ������
*
*Parameters	
*	
*Return Values
*		�ɹ�����������ʧ�ܷ���FAILURE
**************************************************************************/
INT CS_CA_GetPurseTotalNum(void);

/**************************************************************************
* Function Name: CS_CA_GetPurseInfo
*   ��ȡָ�����Ǯ�� ״̬
*
*Parameters	
*	       dwRecordIndex
*                     [in]  Ǯ�����(��1��ʼ)
*          pstCaPurseInfo
*                     [out] ״̬����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetPurseInfo(DWORD dwPurseIndex,T_CaPurseInfo* pstCaPurseInfo);

/**************************************************************************
* Function Name: CS_CA_GetDebitInfo
*   ��ȡָ�����Ǯ�� ��Debit ״̬
*
*Parameters	
*	       dwRecordIndex
*                     [in]  Ǯ�����(��1��ʼ)
*          pstCaDebitInfo
*                     [out] ״̬����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetDebitInfo(DWORD dwPurseIndex,T_CaDebitInfo** pstCaDebitInfo);

/**************************************************************************
* Function Name: CS_CA_GetCreditInfo
*   ��ȡָ�����Ǯ�� ��Credit ״̬
*
*Parameters	
*	       dwRecordIndex
*                      [in]  Ǯ�����(��1��ʼ)
*          pstCaCreditInfo
*                      [out] ״̬����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetCreditInfo(DWORD dwPurseIndex,T_CaCreditInfo** pstCaCreditInfo);

/**************************************************************************
* Function Name: CS_CA_GetCardMaturityRating
*   ��ȡ����ǰ�ĸ�ĸ��
*
*Parameters	
*           pMat [out] ��ĸ����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetCardMaturityRating(T_CaMaturityRating *pMat);

/**************************************************************************
* Function Name: CS_CA_ChangCardMaturityRating
*   �޸Ŀ��ĸ�ĸ��
*
*Parameters	
*	       eMatRat
*                 [in]  ��ĸ��
*          pSzPin
*                 [in] ������
*Return Values
*		SUCCESS(0)	�޸ĳɹ�
*		1			�������
*		OTHER       	�޸�ʧ��
**************************************************************************/
INT CS_CA_ChangeCardMaturityRating(T_CaMaturityRating eMatRat,BYTE *pSzPin);

/**************************************************************************
* Function Name: CSCAAcceptMaturityRat
*   ��������ȷ���տ���ĸ�����ƽ�Ŀ
*
*Parameters	
*          pSzPin
*                 [in] ������
*           hService
*                 [in]��Ŀhandle
*Return Values
*		SUCCESS(0)	�ɹ�
*		1			�������
*		OTHER	        ʧ��
**************************************************************************/
INT CSCAAcceptMaturityRat( BYTE *pSzPin , HCSHANDLE hService);
INT CS_CA_AcceptMaturityRat( BYTE *pSzPin );


/**************************************************************************
* Function Name: CSCACancelMaturityRat
*   ȡ����ĸ��������ʾ�򣬲��տ���ǰ��Ŀ
*
*Parameters   hService
*                 [in]��Ŀhandle
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		����
**************************************************************************/
INT CSCACancelMaturityRat( HCSHANDLE hService );
INT CS_CA_CancelMaturityRat(void );

/**************************************************************************
* Function Name: CSCAAcceptStartView
*   ��������ȷ���տ��۷�
*
*Parameters	
*          pSzPin
*                 [in] ������
*           hService
*                 [in]��Ŀhandle
*Return Values
*		SUCCESS(0)	�ɹ�
*		1			�������
*		OTHER	        ʧ��
**************************************************************************/
INT CSCAAcceptStartView( BYTE *pSzPin , HCSHANDLE hService );
INT CS_CA_AcceptStartView( BYTE *pSzPin );

/**************************************************************************
* Function Name: CSCACancelStartView
*   ȡ���۷ѽ�Ŀ�ۿ���ʾ�򣬲��տ���ǰ��Ŀ
*
*Parameters	hService
*                 [in]��Ŀhandle
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		����
**************************************************************************/
INT CSCACancelStartView( HCSHANDLE hService );
INT CS_CA_CancelStartView(void);

/**************************************************************************
* Function Name: CSCAChangCardMaturityRating
*   ��������ȷ�϶�����ǰ��Ŀ
*
*Parameters	
*          pSzPin
*                 [in] ������
*           hService
*                 [in]��Ŀhandle
*Return Values
*		SUCCESS(0)	�ɹ�
*		1			�������
*		OTHER	        ʧ��
**************************************************************************/
INT CSCAAcceptOrderInfo( BYTE *pSzPin ,HCSHANDLE hService );
INT CS_CA_AcceptOrderInfo( BYTE *pSzPin  );

/**************************************************************************
* Function Name: CSCACancelOrderInfo
*   ȡ����Ŀ������ʾ�򣬲��տ���ǰ��Ŀ
*
*Parameters	
*           hService
*                 [in]��Ŀhandle

*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		����
**************************************************************************/
INT CSCACancelOrderInfo( HCSHANDLE hService  );
INT CS_CA_CancelOrderInfo(void);

/**************************************************************************
* Function Name: CSCAAcceptPPVRequest
*   ��������ȷ�Ϲ���PPV��Ŀ
*
*Parameters	
*          pSzPin
*                 [in] ������
*           hService
*                 [in]��Ŀhandle

*Return Values
*		SUCCESS(0)	�ɹ�
*		1			�������
*		OTHER       	ʧ��
**************************************************************************/
INT CSCAAcceptPPVRequest( BYTE *pSzPin , HCSHANDLE hService);
INT CS_CA_AcceptPPVRequest( BYTE *pSzPin );

/**************************************************************************
* Function Name: CSCACancelPPVRequest
*   ȡ��PPV������ʾ�򣬲��տ���ǰ��Ŀ
*
*Parameters	
*           hService
*                 [in]��Ŀhandle
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		����
**************************************************************************/
INT CSCACancelPPVRequest( HCSHANDLE hService);
INT CS_CA_CancelPPVRequest(void);

/**************************************************************************
* Function Name: CS_CA_ChangeCardPin
*   �޸Ŀ�����
*
*Parameters	
*	       pSzPinOld
*                  [in]  ������
*          pSzPinNew
*                  [in]  ������
*Return Values
*		SUCCESS(0)	�޸ĳɹ�
*		1			��������� "Incorrect old PIN"
*		OTHER       	�޸�ʧ��"Change CA PIN failed"
*note: example dialogue:
*    //////////////////////////////////////////
*    Change PIN Please enter CA PIN   ****
*    Please enter new CA PIN  ****
*    Please confirm new CA PIN  ****
*    /////////////////////////////////////////
*if the user has entered two new PINs that do not match,"PIN confirmation does not match" should be displayed
**************************************************************************/
INT CS_CA_ChangeCardPin(BYTE* pSzPinOld, BYTE *pSzPinNew);

/**************************************************************************
* Function Name: CS_CA_Standby
*   ֪ͨCA ��ǰΪ���� ״̬
*
*Parameters	
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_Standby(void);

/**************************************************************************
* Function Name: CS_CA_Resume
*   ֪ͨCA �Ӵ���״̬����
*Parameters	
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_Resume(void);

/**************************************************************************
* Function Name: CS_CA_CheckCard
*   ��ȡ����ǰ״̬
*Parameters	
*Return Values
*		����ǰ״̬
**************************************************************************/
T_CaStatus CS_CA_GetCardStatus(void);

/**************************************************************************
* Function Name: CS_CA_EmailDel
*  ɾ��һ���ʼ�
*
*Parameters	
*	      dwIndex
*              [in]  ���(��0��ʼ)
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*ע: conax���ʼ�ɾ����ͨ��ǰ�������еġ�
�����������ṩ��ĳЩ��Ӫ���������������Ŀ��
��֤��Ŀ�������ϲ��ṩ�ô���
**************************************************************************/
INT CS_CA_EmailDel(DWORD dwIndex/* start with 0*/);

/**************************************************************************
* Function Name: CS_CA_EmailDelByID
*  ɾ��һ���ʼ�
*
*Parameters	
*	      ucMailId
*              [in]  �ʼ�Id
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*ע: ������ͨ���ʼ�Idɾ��һ���ʼ���Ŀǰ����kccl��Ŀʹ��
**************************************************************************/
INT CS_CA_EmailDelByID(BYTE ucMailId);

/**************************************************************************
* Function Name: CS_CA_EmailDelAll
*  ɾ�������ʼ�
*
*Parameters	
*	       
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*ע: conax���ʼ�ɾ����ͨ��ǰ�������еġ�
�����������ṩ��ĳЩ��Ӫ���������������Ŀ��
��֤��Ŀ�������ϲ��ṩ�ô���
**************************************************************************/
INT CS_CA_EmailDelAll(void);

/**************************************************************************
* Function Name: CS_CA_EmailGetNum
*  ��ȡ��ǰ�ʼ�����
*
*Parameters	
*	       
*Return Values
*		��������
**************************************************************************/
INT CS_CA_EmailGetNum(void);

/**************************************************************************
* Function Name: CSCAEmailGetNum
*  ��ȡ��ǰ�ʼ�����,�Լ�δ���ʼ�����
*
*Parameters 
*          pnTotalNum :�ʼ�����
*          pnUnreadNum:δ���ʼ�����
*Return Values
*       SUCCESS �ɹ�
*       FAILURE     ʧ��

**************************************************************************/
INT CSCAEmailGetNum(  INT* pnTotalNum    , INT* pnUnreadNum);

/**************************************************************************
* Function Name: CS_CA_EmailGetInfo
*  ������Ż�ȡĳ���ʼ�
*
*Parameters	
*	    dwIndex
*             [in] ���(��0��ʼ)
*		pstMsg
*             [out] ��õĶ���Ϣ
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_EmailGetInfo(DWORD dwIndex/* start with 0*/, T_CaEmail *pstMail);

/**************************************************************************
* Function Name: CS_CA_EmailSetReadflag
*  ��ָ������ʼ�����Ϊ�Ѷ�
*
*Parameters	
*	    dwIndex
*             [in] ���(��0��ʼ)
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_EmailSetReadflag(DWORD dwIndex/* start with 0*/);

/**************************************************************************
* Function Name: CS_CA_EmailSaveFlash
*  �������ж��ʼ����޸�
*
*Parameters	��
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_EmailSaveFlash(void);
/**************************************************************************
* Function Name: CS_CA_EmailSetSequence
*  ����email����˳��Ĭ��Ϊ��˳��
*
*Parameters ��
*Return Values
*       SUCCESS �ɹ�
*       FAILURE     ʧ��
**************************************************************************/
INT CS_CA_EmailSetSequence(T_CaEmailSeq emSeq);

/**************************************************************************
* Function Name: CSCAGetShortMsg
*  ��ȡϵͳ�յ������µĶ���Ϣ
*
*Parameters	
*		dwMsgId
*			[in] ����ϢID
*		pstMessage
*			[out] ����Ϣ����
*           hService
*                 [in]��Ŀhandle
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*��������������conax5.0�汾��(����5.0�汾)�Ѿ�����,����ֻ��Ϊ�˼����ϰ汾
**************************************************************************/
INT CSCAGetShortMsg(HCSHANDLE hService, DWORD dwMsgId, T_CaMessage *pstMessage);
INT CS_CA_GetShortMsg(DWORD dwMsgId, T_CaMessage *pstMessage);


/**************************************************************************
* Function Name: CSCAGetFingerPrint
*  ��ȡϵͳ�յ������µ�ָ��
*
*Parameters	
*		pstFingerPrint
*			[out] ָ������
*        hService
*             [in]��Ŀhandle
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*��������������conax5.0�汾��(����5.0�汾)�Ѿ�����,����ֻ��Ϊ�˼����ϰ汾
**************************************************************************/
INT CSCAGetFingerPrint(HCSHANDLE hService, T_CaFingerprint* pstFingerPrint);
INT CS_CA_GetFingerPrint(T_CaFingerprint* pstFingerPrint);


/**************************************************************************
* Function Name: CSGetCaFlashAddress
*  CA ��ȡFlash�ռ��ַ,1��Block. ��Ӧ��ʵ�ָú���
*
*Parameters	��
*Return Values
*		CA FLASH �ռ��׵�ַ
**************************************************************************/
//extern DWORD CSGetCaFlashAddress(void);
extern DWORD CS_CA_GetFlashAddress(void);


/**************************************************************************
* Function Name: CS_CA_GetChipId
*  ��ȡ��������оƬID. ChipSet Pairingģʽʱʹ��
*
*Parameters	
*Return Values
*		�ɹ��򷵻�ID
*		ʧ�ܷ���0		
**************************************************************************/
DWORD CS_CA_GetChipId(void);

/**************************************************************************
* Function Name: CSCaGetCurrentTime
* CA ��ȡ��ǰϵͳʱ��(UTC)
*
*Parameters	
*		stDateTime
*		[out] ���ں�ʱ��
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*ע��:��Ӧ��û�л�ȡ��TDT��֮ǰ,�뷵��FAILURE
**************************************************************************/
extern INT CSCaGetCurrentTime(T_CaDateTime *stDateTime);

/**************************************************************************
* Function Name: CSGetCaCPMFlashAddress
*  CA ��ȡ����ChipSet Pairing key�洢FLASH�ռ��ַ����С1block.  ��Ӧ��ʵ�ָú�����
* ������Ҫ��ԱȽϰ�ȫ��ChipSet Pairing ģʽʱʹ�ã����޴˹��ܣ��򷵻�0
*
*Parameters	��
*Return Values
*		CA FLASH �ռ��׵�ַ
**************************************************************************/
extern DWORD CSGetCaCPMFlashAddress(void);

/**************************************************************************
* Function Name: CSGetCaOTPFlashAddress
*  CA��ȡ��� Verifier�ؼ�����Flash��ַ,��������OTP������1K.
* ������Ҫ��ԱȽϰ�ȫ��verifierģʽʱʹ�ã����޴˹��ܣ��򷵻�0
*
*Parameters ��
*Return Values
*       CA FLASH �ռ��׵�ַ
**************************************************************************/
extern DWORD CSGetCaOTPFlashAddress(void);

/**************************************************************************
* Function Name: CSGetCaVerifierFlashAddress
*  CA��ȡ���Verifier Key��Flash�ռ��ַ��1��Block.
*  verifierģʽʱʹ�ã����޴˹��ܣ��򷵻�0
*
*Parameters ��
*Return Values
*       Verifier Key��Flash�ռ��ַ
**************************************************************************/
extern DWORD CSGetCaVerifierFlashAddress(void);

/**************************************************************************
* Function Name: CSGetCaVerifierBackUpFlashAddress
* CA��ȡ����Verifier Key��Flash�ռ��ַ��1��Block.
*  verifierģʽʱʹ�ã����޴˹��ܣ��򷵻�0
*
*Parameters ��
*Return Values
*       Verifier Key��Flash�ռ��ַ
**************************************************************************/
extern DWORD CSGetCaVerifierBackUpFlashAddress(void);

/**************************************************************************
* Function Name: CSReturnPathConnectServer
* ���ӵ������������ڻش������޴˹��ܣ��򷵻�0
*
*Parameters 
*    pcIpAddress: IP��ַ
*    dwPort: �˿ں�
*    dwTimeout����ʱʱ��
*Return Values
* *		SUCCESS	�ɹ�
*		FAILURE		ʧ��      
**************************************************************************/
extern INT CSReturnPathConnectServer(char *pcIpAddress, DWORD dwPort, DWORD dwTimeout);

/**************************************************************************
* Function Name: CSReturnPathSend
* �������ݣ����ڻش������޴˹��ܣ��򷵻�0
*Parameters 
*   pcData: �����͵������׵�ַ
*   nLen: ���ݳ���
*   dwTimeout����ʱʱ��
*Return Values
*      *		SUCCESS	�ɹ�
*		FAILURE		ʧ�� 
**************************************************************************/
extern INT CSReturnPathSend(char *pcData, INT nLen, DWORD dwTimeout);

/**************************************************************************
* Function Name: CSReturnPathRecv
* �������ݣ����ڻش������޴˹��ܣ��򷵻�0
*  
*Parameters 
*       pcBuf: ���������׵�ַ
*       cBufLen: �������ݳ���
*       dwTimeout :��ʱʱ��
*Return Values
*    	SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
extern INT CSReturnPathRecv(char *pcBuf, INT cBufLen, DWORD dwTimeout);

/**************************************************************************
* Function Name: CSReturnPathClose

*  �ر����ӣ����ڻش������޴˹��ܣ��򷵻�0
*
*Parameters ��
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��      
**************************************************************************/
extern INT CSReturnPathClose(void);

/**************************************************************************
* Function Name: CSCACheckandReportShortMsg
*  �������˳��������Ž�Ŀ��������EPGʱ������Ƿ�����Ҫ��ʾ�Ķ���Ϣ��ָ��
*
*Parameters ��
*Return Values
*         
**************************************************************************/
void CSCACheckandReportShortMsg(void);

/**************************************************************************
* Function Name: CSCACancelCardProblem
* �û���������,ȡ����������ʾ��
CA_ALARM_CARD_INCORRECT,
CA_ALARM_CARD_INCORRECT_CONAX,
CA_ALARM_CARD_COMMUNICATION_ERROR
*
*Parameters
*   dwParam:  ��·��Ŀ�봫��0
*             ��·��Ŀ����CSCAStartService()���ص�handle
*Return Values
*ע��: conax5�汾֮ǰ��֧��,conax5�汾��(����5�汾)����ʹ��
**************************************************************************/
void CSCACancelCardBanner(DWORD dwParam);

/**************************************************************************
* Function Name: CSCACancelShortMsg
* �û���������,ȡ������Ϣ��ʾ��
*
*Parameters
*   dwParam:  ��·��Ŀ�봫��0
*             ��·��Ŀ����CSCAStartService()���ص�handle
*Return Values
*ע��: conax5�汾֮ǰ��֧��,conax5�汾��(����5�汾)����ʹ��
**************************************************************************/
void CSCACancelShortMsg(DWORD dwParam);

/**************************************************************************
*Function Name: CS_CA_EnterMenu
*  ����CA��������
*
*Parameters 
*	eMenuType:�˵�����
*   dwParam:  ��·��Ŀ�봫��0
*             ��·��Ŀ����CSCAStartService()���ص�handle
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
*ע��: conax5�汾֮ǰ��֧��,conax5�汾��(����5.0�汾)����ʹ��
**************************************************************************/
INT CS_CA_EnterMenu( CONAX_MENU_STATUS eMenuType, DWORD dwParam);


/**************************************************************************
*Function Name: CS_CA_ExitMenu
*  �˳�CA��������
*
*Parameters 
*	eMenuType:�˵�����
*   dwParam:  ��·��Ŀ�봫��0
*             ��·��Ŀ����CSCAStartService()���ص�handle
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
*ע��: conax5�汾֮ǰ��֧��,conax5�汾��(����conax5�汾)����ʹ��
**************************************************************************/
INT CS_CA_ExitMenu( CONAX_MENU_STATUS eMenuType, DWORD dwParam);

#ifdef  __cplusplus
}
#endif

#endif

