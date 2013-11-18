/*************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_NSCA_Interface.h  
* Author:      Wan suanlin
* Date:        
* Description: NDS CA interface
*            
**************************************************************************************************************/
#include "udiplus_typedef.h"

#ifndef _CS_NDSCA_INTERFACE_H
#define _CS_NDSCA_INTERFACE_H

#ifdef  __cplusplus
extern "C" {
#endif

#define CS_NDSCA_MESSAGE_BASE    (0)

typedef enum _CaEventGroup
{
	CA_ALARM = CS_NDSCA_MESSAGE_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	
	CA_EVENTGROUP_MAX
}E_CaEventGroup;

typedef enum _CaInfo
{
	CA_INFO_START = (CS_NDSCA_MESSAGE_BASE + CA_EVENTGROUP_MAX + 1),//6
	
	CA_INFO_TXT,          //dwParm2 = (DWORD)&T_CaMail(�����ʼ�)
	CA_INFO_TXT_CHANGE,   //dwParm2 = (DWORD)&T_MailNum
	CA_INFO_VIEWCARDCFG,  //dwParm2 = (DWORD)&T_CaViewCardCfg
	CA_INFO_SICFG,        //dwParm2 = (DWORD)&T_SICfg(service information config)
	CA_INFO_CADIAG_DATA,  //dwParm2 = (DWORD)&T_CaDiagdata
	CA_INFO_OSD,          //dwParm2 = (DWORD)&T_CaExceptionMsg:barker show 12
	CA_INFO_SCREGIONID,   //dwParm2 = (DWORD)&T_SCRegionInfor
	CA_INFO_CASTBLABEL,   //dwParm2 = (DWORD)&T_CaStbLabelInfo
	
	CA_INFO_MAX	
}E_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_START = (CS_NDSCA_MESSAGE_BASE + CA_INFO_MAX + 1),//16
	
	CA_COMMAND_FORCE_MSG_HIDE,
	CA_COMMAND_FORCE_MSG_SHOW, //dwParm2 = (DWORD)&T_CaFpInfo
	CA_COMMAND_SMC_STATUS,     //dwParm2 = (DWORD)&T_SmcStatus
	CA_COMMAND_BOUQETID_CHANGE,/*dwParm2 is original data,you should get bouquet_id by 
	"bouquet_id = (dwParm2 & 0xFF) + 0x6000". Notice the calculation of bouquet_id is 
	according to the project*/

	//��������Ƶ������Decoder��
	CA_COMMAND_AV_TURN_ON,        //Open AV: dwParm2  = &T_OperateAVInfo 21
	CA_COMMAND_AV_TURN_OFF,       //Close AV:dwparm2 = &T_OperateAVInfo  22
	
	CA_COMMAND_MAX
}E_CaCommand;

typedef enum _CaStatus
{
	CA_STATUS_START = (CS_NDSCA_MESSAGE_BASE + CA_COMMAND_MAX + 1),//24

	CA_STATUS_MODIFY_PIN_SUCCESS,             /*Modify Pin Code Success*/
	CA_STATUS_MODIFY_PR_LEVEL_SUCCESS,        /*Modify Parent Rating Success*/	
	CA_STATUS_MODIFY_STANDBY_STATUS_SUCCESS,  /*Set STB Standby Status Success*/
	CA_STATUS_GET_CUR_LANGUAGE_SUCCESS,       /*Get Current Language Success, dwParm2 = E_CaLanguage*/
	CA_STATUS_MODIFY_CUR_LANGUAGE_SUCCESS,    /*Modify Current Language Success*/

	CA_STATUS_GET_PR_LEVEL_SUCCESS,           /*Get Current Card Limit Value, dwPara2 = Limit Valut(3-18)*/
	CA_STATUS_GET_PIN_STATUS_SUCCESS,         /*Get Pin Status Success, dwParm2 = (DWORD)&T_CaPinLockStaus*/
	CA_STATUS_MODIFY_PIN_STATUS_SUCCESS,      /*Modify Pin Status Success*/

	CA_STATUS_SMC_ERROR,                      /*Smart card error 33*/
	CA_STATUS_INVALID_USERID,                 /*Invalid user id*/
	CA_STATUS_INVALID_PIN,                    /*Invalid pin*/
	CA_STATUS_INVALID_LIMITID,                /*Invalid limit value*/
	CA_STATUS_ACTION_FAILED,                  /*Action failed*/
	CA_STATUS_WRONG_LANGUAGE,                 /*Wrong language*/

	CA_STATUS_MODIFY_STANDBY_STATUS_FAILURE,  /*Set STB Standby Status failure*/
	
	CA_STATUS_MAX
}E_CaStatus;

typedef enum _CaAlarm
{
	CA_ALARM_START = (CS_NDSCA_MESSAGE_BASE + CA_STATUS_MAX + 1),//41

	CA_ALARM_FREE_PROGRAME,       
	CA_ALARM_WITH_NDS_CA,         
	CA_ALARM_OTHER_CA_ENCRYPT,    /*E128 Other CA Service(E128 �������Ž�Ŀ)*/
	CA_ALARM_SMC_OUT,             /*E01 Please insert Your Card(E01 ���ܽ�Ŀ����忨)*/
	CA_ALARM_SMC_UNKNOW,          /*E50 Unknow Card(E50 δ��ʶ������ܿ�)--Ԥ�� 45*/
	
	CA_ALARM_SMC_CHECKING,        /*47(Ԥ��)*/
	CA_ALARM_SMC_IN,              
	CA_ALARM_SMC_MUTE,            /*E02 Card is invalid. Please call Customer service (����������)(E02��Ч�����ܿ�)*/
	CA_ALARM_SMC_TRANSTION_ERROR, /*E51 Card Transtion is Error(E51 ���ܿ�ͨѶ����)--Ԥ�� 50*/
	CA_ALARM_SMC_OK,              
	
	CA_ALARM_SMC_ERROR,           /*E52 Card is Error(E52 ���ܿ�����, Ԥ��)*/
	CA_ALARM_SMC_INVALID,         /*E25 Inserted card is not a TV Viewing card(E25 ���ܿ���Ч)*/
	CA_ALARM_SMC_MATCH,           /*Ԥ��*/
	CA_ALARM_STB_SMC_NOT_MATCH,   /*E14 This is the wrong viewing card for this CableCard(E14 ���������)*/
	CA_ALARM_SMC_CHANGED,         /*E53 Card has been Changed(E53 �ۿ������ܿ������ı�)--Ԥ�� 55*/

	CA_ALARM_SMC_CLOSED,          /*E11 Please call Customer service (����������) (Security problem)(E11 ��ȫ���⣬�رտ�)*/
	CA_ALARM_SMC_NEW_TRANSITION,  /* New smart card in preparation stage */
	CA_ALARM_SMC_NOT_AUTHORIZED,  /*E06 This viewing card is not authorized. Please call Customer service (����������)(E06 ��δ��Ȩ)*/
	CA_ALARM_CW_INVALID,          /*E04 This Service is not authorized(E04 ��Ŀδ��Ȩ)*/
	CA_ALARM_CW_VALID,            //61 valid cw key*/
	
	CA_ALARM_SRV_INFO_ERROR,      /*E05 Invalid Viewing CA Packet(E05 ��Ŀ��Ϣ�������Ե�)*/	
	CA_ALARM_AREA_RESTRICT,       /*E09 The Service is Blacked Out(E09 ��Ŀ��������)*/	
	CA_ALARM_PR_LEVEL_TOO_LOW,    /*E54 Parent's Level is too Low(E54 �ۿ�����̫��)--Ԥ��*/
	CA_ALARM_CLEAN_MSG,           

	CA_ALARM_MAX
	
}E_CaAlarm;

typedef enum _CaOsdType
{
	CA_OSD_CLEAN_SCREEN,        /*(Clean screen)*/
	CA_OSD_SMC_OUT,             /*Please insert card*/
	CA_OSD_SMC_MUTE,            /*Card communication Failure*/
	CA_OSD_SERVICE_NOT_AUTHORIZED,  /*This service is not authorized*/
	CA_OSD_INVALID_PACKET,      /*Invalid packet*/

	CA_OSD_SMC_NOT_AUTHORIZED,  /*This card is not authorized*/
	CA_OSD_SERVICE_BLACKED_OUT, /*This service is blackout*/
	CA_OSD_SECURITY_PROBLEM,    /*Please call(security problem)*/
	CA_OSD_NO_EMPTY_IPPV_SLOTS, /*No empty IPPV slots*/
	CA_OSD_SMC_LACK,            /*Insufficient credit in card*/

	CA_OSD_STB_SMC_NOT_MATCH,   /*This is wrong card for this STB*/
	CA_OSD_PURCHASE_BLOCK,      /*Purchase not permitted without callback ability*/
	CA_OSD_USE_NEW_CARD,        /*Please Use new card(old card already chained)*/
	CA_OSD_END_PURCHASE_WINDOW, /*End of purchase window*/
	CA_OSD_BAD_IPPV_PACKET,     /*Please call(bad IPPV packet, purchase failed)*/
	
	CA_OSD_SMC_NOT_TVCARD,      /*Insert card is not TV viewing card*/
	CA_OSD_SMC_CHECKING,        /*Checking card, please wait*/
	CA_OSD_VOD_NOT_VIEWED,      /*This VOD service isn't addressed to your viewing card*/

	CA_OSD_MSG_TXT,             /*Message Text*/
	
	CA_OSD_TYPE_MAX
}E_CaOsdType;

typedef enum _CaLanguage
{
	CA_LAN_ENG,      //English
	CA_LAN_SPA,      //Spanish
	CA_LAN_POR,      //Portuguese
	CA_LAN_ARA,      //Arabic
	CA_LAN_BEN,      //Bengali

	CA_LAN_BUR,      //Burmese
	CA_LAN_GRC2,     //Cantonese Chinese
	CA_LAN_HEB,      //Hebrew
	CA_LAN_HIN,      //Hindi
	CA_LAN_IND,      //Indonesian (=Bhasa3)

	CA_LAN_JPN,      //Japanese
	CA_LAN_KOR,      //Korean
	CA_LAN_MAY,      //Malay(=Bhasa)
	CA_LAN_TGL,      //Tagalog
	CA_LAN_TAM,      //Tamil

	CA_LAN_THA,      //Thai
	CA_LAN_URD,      //Urdu
	CA_LAN_CHI,      //Chinese complicated
	CA_LAN_MIS2,     //Chinese simplified
	CA_LAN_GOH2,     //Mandarin Chinese

	CA_LAN_FRO2,     //Mandarin Taiwan
	CA_LAN_INVALID = 0xFF,
	
	CS_CA_LAN_MAX
}E_CaLanguage;

typedef enum _AVStreamType
{
	E_AVTYPE_AUDIO,
	E_AVTYPE_VIDEO,

	E_AVTYPE_MAX
}E_AVStreamType;

/* Smardcard Region data*/
#define CA_MAX_REGION_DATA_LEN  (64)

typedef enum _SCRegionStatus
{
	E_SCREGION_OK,
	E_SCREGION_CARD_ERROR,
	E_SCREGION_ACTION_FAILED,
	E_SCREGION_INVALID_PARAM,
	/*E_SCREGION_INSUFFICIENT_BUFFER,*/

	E_SCREGION_MAX
	
}E_SCRegionStatus;

typedef struct _SCRegionInfor
{
	E_SCRegionStatus  m_eStatus;
	BYTE                     m_ucSmcRegionData[CA_MAX_REGION_DATA_LEN];
}T_SCRegionInfor;

typedef struct _OperateAVInfo
{
	HCSHANDLE          m_hService;
	E_AVStreamType   m_eAVType;
}T_OperateAVInfo;

typedef enum _OpenAVtype
{
	E_CW_FLAG,
	E_OSD_FLAG,
	E_SMC_FLAG,
	E_NDSCA_FLAG,
	E_REGION_FLAG,

	E_AVSTATUS_MAX
}E_OpenAVtype;

typedef struct _StreamInfo
{
	E_AVStreamType m_eStreamType;
}T_StreamInfo;

typedef struct _CaDateTime
{
	DWORD   m_dwYear;
	BYTE    m_cMonth;
	BYTE    m_cDay;
	BYTE    m_cWeekDay;
	BYTE    m_cHour;
	BYTE    m_cMinute;
	BYTE    m_cSecond;
	BYTE    m_cRec[2];
}T_CaDateTime;

//MAIL
#define CA_MAIL_TITLE_LEN         (60)
#define CA_MAIL_SENDER_LEN        (60)
#define MAX_MAIL_TXT_LEN          (4096)

typedef struct _MailNum
{
	DWORD   m_dwTotalNum;
	DWORD   m_dwUnReadNum;
}T_MailNum;

typedef struct _Msg
{
	WORD    m_wEmailId;        //�ʼ���ʶ��
	BOOL    m_bPriporityHigh;  //FALSE: Nomal Mail  TRUE: Urgency Mail
	BOOL    m_bDelete;         //TRUE:  User can not delete
	DWORD   m_dwSenderLen;                //��������������
	BYTE    m_acSenderName[CA_MAIL_SENDER_LEN];//����������

	BYTE    m_cTitleFormatCode;
	BYTE    m_cRec1[3];
	DWORD   m_dwTitleLen;                 //���ⳤ��
	BYTE    m_acTitle[CA_MAIL_TITLE_LEN]; //����

	BYTE    m_cLanguage;                  //0x11-Unicode����, 0x13-Chinese
	BYTE    m_cCompress;                  //Ŀǰ��֧��

	BYTE    m_cContFormatCode;            //0x11 Unicode���� , 0x13-GB2312����
	BYTE    m_cRec2[1];
	DWORD   m_dwContentLen;               //�ʼ����ݳ���
	BYTE    *pcContext;                   //�ʼ�����
	
	T_CaDateTime    m_stSendTime;         //�ʼ�����ʱ��
	T_CaDateTime    m_stDeleteTime;       //�ʼ�����ʱ��
}T_Msg;

typedef struct _CaMail
{	
	BOOL    m_bReadFlag;            //�Ѷ���־
	
	T_CaDateTime    m_stArriveTime; //�ʼ������ʱ��
	T_Msg           m_stMsg;
}T_CaMail;


//OSD
#define CA_MAX_OSD_LEN          (512)

typedef enum _CaOsdOp
{
	CA_OSD_SHOW_GENERAL,   /*User cant not remove the message*/
	CA_OSD_SHOW_INFO,      /*User can remove the message*/
	CA_OSD_IDLE_EXCEPTION  /*Remove Pre-show message*/
}E_CaOsdOp;

typedef struct _CaOsdTxt
{
	DWORD   m_dwLen;
	BYTE    m_acText[CA_MAX_OSD_LEN];
}T_CaOsdTxt;

//Exception Descriptor
typedef struct _CaExceptionMsg
{
	BYTE    m_cExceprionCode;  //E_CaOsdType
	BYTE    m_cLangue;         //E_CaLanguage
	WORD    m_wLangueCode;     //0x11:unicode 0x13:GB232
	DWORD   m_dwMsgHandle;     //when app cancel this message, call cancel function will use
	
	T_CaOsdTxt m_stOsdTxt;
}T_CaExceptionMsg;

//Exception Extra Data Descriptor
typedef struct _CaExtraMsg
{
	DWORD   m_dwDataSouce;
	BYTE    m_cExceptionSouce;//0:ECM OSD 1:EMM OSD 2:Verifier OSD
	BYTE    m_cPriority;
	BYTE    m_cRec[2];
}T_CaExtraMsg;

//SMART CARD
typedef enum _CaCardType
{
	CA_UNKNOWN_CARD,      //Unknown card type
	CA_VIEWING_CARD,      //Card designated for NDS CA
	CA_OLD_VIEWING_CARD,  //Old viewing Card(���ڵ�)
	CA_INTERACTIVE_CARD,  //Card designated for interactive TV viewing
	CA_AUXILIARY_CARD,    //Card for other purpose

	CA_CARD_TYPE_MAX
}E_CaCardType;

typedef enum _CaCardStatus
{
	CA_CARD_REMOVE,     //The card is removed from STB
	CA_CARD_READY,      //The card is inserted and ready for operation
	CA_CARD_FAILURE,    //The card does not answer
	CA_CARD_INVALID,    //The inserted card is not a valid viewing card
	CA_CARD_CLOSED,     //Security problem, card failure on authentication
	
	CA_CARD_TRANSITION, //New card in preparation stage
	CA_CARD_INSERT,     //Inserted card is undergoing initialization

	CA_CARD_STATUS_MAX
}E_CaCardStatus;

//Card Status Descriptor:When smart card is changed, the verifier will send this status to app
typedef struct _SmcStatus
{
	BYTE    m_cSmcType;   //E_CaCardType
	BYTE    m_cSmcStatus; //E_CaCardStatus
	BYTE    m_cRec[2];
}T_SmcStatus;

//-->Ŀǰδʹ��
//SI Descriptor
typedef struct _SICfg
{
	WORD    m_wCaSystemId;
	WORD    m_wSiFiltering; //Maybe this is Bouquet ID
}T_SICfg;
//<--Ŀǰδʹ��

//Viewing Card config descriptor
typedef struct _CaViewCardCfg
{
	DWORD   m_dwCardID;       //���ܿ�ID
	DWORD   m_dwSubscriberID; //Ԥ���߱�ʶID
	BYTE    m_cCardPaired;    //0��������ƥ�䣬1������ƥ��
	BYTE    m_cCardIDCheck;   //���ܿ�У��λ����ʾʱ���������ʾ���ܿ��ŵĺ���
	BYTE    m_cCardAuthorized;//0�����ܿ�δ��Ȩ��1�����ܿ�����Ȩ
	BYTE    m_cRec[1];
}T_CaViewCardCfg;

//Finger Print
typedef enum _CaOsdCol
{
	CA_COL_TRANSPARENT, //͸��ɫ
	CA_COL_WHITE,       //��ɫ
	CA_COL_RED,         //��ɫ
	CA_COL_BLUE,        //��ɫ
	
	CA_COL_MAX
}E_CaOsdCol;

#define CA_MAX_FINGERPRINT_LEN    (20)

typedef struct _CaFpInfo
{
	BYTE    m_cTxtColIndex;    //E_CaOsdCol(�ı�������ɫ)
	BYTE    m_cRegionColIndex; //E_CaOsdCol(�������ɫ)
	WORD    m_wRegionHeight;   //�ı��߶�
	WORD    m_wRegionWidth;    //�ı����
	WORD    m_wRegionXOffset;  //�ı�ƫ�ƺ�����ֵ(�����Ļ)
	
	WORD    m_wRegionYOffset;  //�ı�ƫ��������ֵ(�����Ļ)
	WORD    m_wTxtXOffset;     //��ʾ����ƫ�ƺ�����ֵ(����ı�)
	WORD    m_wTxtYOffset;     //��ʾ����ƫ��������ֵ(����ı�)
	WORD    m_wDuration;       //Time Out, Hide msg; if the value is 0, Ca will send CA_COMMAND_FORCE_MSG_SHOW to Hide this msg

	BYTE    m_cLen;
	BYTE    m_acTxt[CA_MAX_FINGERPRINT_LEN];
}T_CaFpInfo;

//Lock Status
typedef struct _CaPinLockStatus
{
	BYTE    m_cSystemLocked;    //TRUE: Locked; FALSE: Unlocked(Ŀǰֻ������һ�ֶ�)
	BYTE    m_cSystemtmpLocked; //TRUE: Locked; FALSE: Unlocked
	BYTE    m_cMasterPinDefined;//TRUE: Master pin defined; FALSE: Master pin undefine
	BYTE    m_cRec[1];
}T_CaPinLockStaus;

#define CA_MAX_SMC_VERSION_LEN   (8)
//Ca DiagData Information
typedef struct _CaDiagdata
{
	DWORD   m_dwLen;
	BYTE    m_acDiagInfo[CA_MAX_OSD_LEN];
	BYTE    m_acSmcVer[CA_MAX_SMC_VERSION_LEN];
}T_CaDiagdata;

#define CA_MAX_CASTBLABEL_LEN    (20)
#define CA_MAX_CHIPID_LEN        (20)
typedef struct _CaStbLabelInfo
{
	DWORD    m_dwCastblabelLen;
	BYTE     m_aucCastblabel[CA_MAX_CASTBLABEL_LEN];
	DWORD    m_dwChipidLen;
	BYTE     m_aucChipid[CA_MAX_CHIPID_LEN];//����ΪCA�����STB ID�����ֵΪоƬ������chip id+0x80000000���Ҽ���У��λ
	
}T_CaStbLabelInfo;

/**************************************************************************
*Function Name: CS_CA_GetCoreVer
*  ��ȡCA�ں˰汾��Ϣ
*
*Parameters 
*	pcVer[out]:
*		���ڴ洢CA�ں˰汾��Ϣ���� 
*   pnLen[in|out]:
*       �����ʼֵΪ64��������ʵֵ
*
*Return Values: 
*	0 : SUCCESS ��ȡCA�ں˰汾��Ϣ�ɹ�
*	-1: FAILURE ����������ȡCA�ں˰汾ʧ��
**************************************************************************/
INT CS_CA_GetCoreVer(char* pcVer, INT* pnLen);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* ����һ���ʼ�
*
*Parameters 
*	pstTxt[in]:�ʼ�����
*
*Return Values: 
*	0 : SUCCESS ����һ���ʼ����ڴ��гɹ�
*	-1: FAILURE ��������򱣴��ʼ�ʧ��
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
*	0 : SUCCESS ���ĳһ���ʼ��ɹ�
*	-1: FAILURE �����Ŵ����ɾ���ʼ�ʧ��
**************************************************************************/
INT CS_CA_TXT_Del(INT nIndex);

/**************************************************************************
*Function Name:CS_CA_TXT_DelAll
* ɾ�����б�������ʼ�
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS ɾ�������ʼ��ɹ�
*	-1: FAILURE ɾ�������ʼ�ʧ��
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
*	0 : û���ʼ�
*	����: �ʼ��ܸ���
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* ��ȡ����ĳһ���ʼ�������
*
*Parameters 
*	nIndex[in]:�ʼ��±꣬��0��ʼ
*	pstTxt[out]:CA���������ʼ����ݴ洢���ϲ����õĿռ�
*
*Return Values: 
*	0 : SUCCESS ��ȡ�ʼ����ݳɹ�
*	-1: FAILURE �����Ŵ����������󣬻��ȡ�ʼ����ݴ���
**************************************************************************/
INT CS_CA_TXT_GetInfo(INT nIndex, T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* �������ʼ����浽flash�У�һ���ǹػ���ϵ�ǰ���еĲ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS ͬ��FLASH�ɹ�
*	-1: FAILURE ͬ��FLASHʧ��
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_TXT_SetReadFlag
* �����ʼ��Ƿ��Ѷ���־
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	bRead[in]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS ���ö���־�ɹ�
*	-1: FAILURE �����Ŵ�������ö���־ʧ��
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(INT nIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* ���ص�ǰ���ж��ٷ�δ���ʼ�
*
*Parameters 
*	��
*
*Return Values: 
*   0:û��δ���ʼ�
*   ������δ���ʼ�����
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyPinCode
* �����޸�����
*
*Parameters 
*	pstOldPin[in]:������
*   pstNewPin[in]:������
*
*Return Values: 
*	0 : SUCCESS �����Ϸ��������ͨ������Ϣ��UI
*		[��Ϣ���ͣ�CA_STATUS_MODIFY_PIN_SUCCESS | CA_STATUS_SMC_ERROR | CA_STATUS_INVALID_USERID
*		           CA_STATUS_INVALID_PIN | CA_STATUS_INVALID_LIMITID | CA_STATUS_ACTION_FAILED]
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_ModifyPinCode(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestSmcInfo
* �������ܿ���Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS Ŀǰ�ú���ֻ�᷵�سɹ��������ͨ������Ϣ��UI
*		[��Ϣ���ͣ�CA_INFO_VIEWCARDCFG, dwParm2 = (DWORD)&T_CaViewCardCfg]
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_RequestSmcInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestParentRating
* ��ȡ�ۿ�����(Ŀǰ�˹����ڹ�����Ŀ��δʹ��)
*
*Parameters 
*	dwPin[IN]:�û�����
*
*Return Values: 
*	0 : SUCCESS Ŀǰ�ú���ֻ�᷵�سɹ�����ȡ������Ϣ���ȡ��Ϣʧ�ܽ�ͨ������Ϣ��UI
*		[��Ϣ���ͣ�CA_STATUS_GET_PR_LEVEL_SUCCESS(dwParm2�����Ĳ���Ϊ��ǰ���ܿ��ļ���) | CA_STATUS_SMC_ERROR |
*		           CA_STATUS_INVALID_USERID | CA_STATUS_INVALID_PIN | CA_STATUS_INVALID_LIMITID]
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_RequestParentRating(DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyParentRating
* �޸Ĺۿ�����(Ŀǰ�˹����ڹ�����Ŀ��δʹ��)
*
*Parameters 
*	pstPin[in]:��������
*   cParentRatingValue[in]:Ҫ�޸ĵļ���
*
*Return Values: 
*	0 : SUCCESS �����Ϸ���������ͨ������Ϣ��UI
*		[��Ϣ���ͣ�CA_STATUS_MODIFY_PR_LEVEL_SUCCESS | CA_STATUS_SMC_ERROR | CA_STATUS_INVALID_USERID
*		           CA_STATUS_INVALID_PIN | CA_STATUS_INVALID_LIMITID | CA_STATUS_ACTION_FAILED]
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_ModifyParentRating(DWORD dwPin, BYTE cParentRatingValue);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyStandbyStatus
* ���û�����״̬
*
*Parameters 
*	cStandbyStaus[in]:����STB״̬[0:���Ѵ�����1:�������]
*
*Return Values: 
*	0 : SUCCESS �����Ϸ���������ͨ������Ϣ��UI
*		[��Ϣ���ͣ�CA_STATUS_MODIFY_STANDBY_STATUS_SUCCESS]
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_ModifyStandbyStatus(BYTE cStandbyStaus);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCurLanguage
* ��ȡ��ǰ��������
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS (������Ĭ�ϳɹ�)������ͨ����Ϣ��UI
*		[��Ϣ���ͣ�CA_STATUS_GET_CUR_LANGUAGE_SUCCESS(dwParm2�����Ĳ���Ϊ��ǰ��������) |
*		           CA_STATUS_WRONG_LANGUAGE]
*	-1: FAILURE (Ŀǰû�������������)
**************************************************************************/
INT CS_CA_MMI_RequestCurLanguage(void);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyCurLanguage
* ����ĳһ��������
*
*Parameters 
*	eLan[in]:��Ҫ���õ�����
*
*Return Values: 
*	0 : SUCCESS �����Ϸ���������ͨ������Ϣ��UI
*	    [��Ϣ���ͣ�CA_STATUS_MODIFY_CUR_LANGUAGE_SUCCESS | 
*	               CA_STATUS_WRONG_LANGUAGE]
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_ModifyCurLanguage(E_CaLanguage eLan);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestLockStatus
* ��ȡPIN��״̬(Ŀǰ�˹����ڹ�����Ŀ��δʹ��)
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS (������Ĭ�ϳɹ�)������ͨ����Ϣ��UI
*	    [��Ϣ���ͣ�CA_STATUS_GET_PIN_STATUS_SUCCESS |  
*	               CA_STATUS_SMC_ERROR]
*	-1: FAILURE (Ŀǰû�������������)
**************************************************************************/
INT CS_CA_MMI_RequestLockStatus();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyLockStatus
* ����PIN��״̬(Ŀǰ�˹����ڹ�����Ŀ��δʹ��)
*
*Parameters 
*	dwPin[in]: 
*
*Return Values: 
*	0 : SUCCESS �����Ϸ���������ͨ����Ϣ��UI
*	    [��Ϣ���ͣ�CA_STATUS_MODIFY_PIN_STATUS_SUCCESS | CA_STATUS_SMC_ERROR | 
*	               CA_STATUS_INVALID_PIN | CA_STATUS_ACTION_FAILED]
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_ModifyLockStatus(DWORD dwPin, BYTE cLockMode);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCaDiagData
* ��ȡCA�����Ϣ
*
*Parameters 
*	�� 
*
*Return Values: 
*	0 : SUCCESS �����Ϸ���������ͨ����Ϣ��UI
*	    [��Ϣ���ͣ�CA_INFO_CADIAG_DATA, dwParm2 = (DOWRD)&T_CaDiagdata]
*	-1: FAILURE
**************************************************************************/
INT CS_CA_MMI_RequestCaDiagData(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCardId
* ��ȡ����(NDS verify�ϱ���Ϣʱ���������Ŀ��ţ��������1λУ��λ)
*
*Parameters 
*	pdwCardId[out]:����(unsined int buffer�洢)
*
*Return Values: 
*	0 : SUCCESS �ɹ���ȡ������
*   -1: FAILURE ��������
**************************************************************************/
INT CS_CA_MMI_RequestCardId(DWORD* pdwCardId);

/**************************************************************************
*Function Name:CS_CA_GetCardIdFormateString
* ��ȡ����(���ַ�����ʽ�ṩ��Ӧ��ʹ��)
*
*Parameters 
*		pacCardIdStr[in/out]:APP����ռ䱣���ַ�����ʽ�Ŀ��ţ�
*							�ó��ȱ������12 bytes*    
*            pnLen[in/out]:APP֪ͨCA����Ŀռ�Ĵ�С��
                            ��ȡ���ź󣬸����ݱ����������ݵĳ��ȣ���������ʾ��ȫ�������Ǹ����鿪��̫С
*
*Return Values: 
*	0 : SUCCESS �ɹ���ȡ������
*   -1: FAILURE ��������
**************************************************************************/
INT CS_CA_GetCardIdFormateString(CHAR* pacCardIdStr, INT* pnLen);

/**************************************************************************
*Function Name:CS_CA_GetChipId
* ��ȡоƬChip Id
*
*Parameters 
*		pacChipId[in/out]:APP����ռ䱣���ַ�����ʽ��оƬid�ţ�
*							�ó��ȱ�����ڵ���8 bytes��ǰ4��bytesΪChip Id
*            pnLen[in/out]:APP֪ͨCA����Ŀռ�Ĵ�С
*
*Return Values: 
*	0 : SUCCESS �ɹ���ȡ��оƬId��
*   -1: FAILURE ��������
**************************************************************************/
INT CS_CA_GetChipId(BYTE* pacChipId, INT* pnLen);

/**************************************************************************
*Function Name:CS_CA_GetClearMode
* ��ȡclear mode λ
*
*Parameters 
*
*Return Values: 
*	clear mode λ
**************************************************************************/
INT CS_CA_GetClearMode(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestBouquetId
* ��ȡ�û�����
*
*Parameters 
*	pdwBoqueId[in/out]:(unsigned int buffer,���ڴ洢BouquetIdֵ)
*
*Return Values: 
*	0 : SUCCESS ��ȡBouquetId�ɹ�
*	-1: FAILURE ��ȡBouquetIdʧ��
**************************************************************************/
INT CS_CA_MMI_RequestBouquetId(DWORD* pdwBouquetId);

/**************************************************************************
*Function Name:CS_CA_SetCustomerId
* ������Ӫ��ID(NDS����ģ�������Ӫ�̵�CustomerId��ͬ)
* ���ڵ���CS_CA_Init()����֮ǰ�ȵ��ñ�����
*
*Parameters 
*	dwCustomerId[in]:����Ŀȷ��ʱ����NDS CA�������Ӫ�̵�ID
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_SetCustomerId(DWORD dwCustomerId);

/**************************************************************************
*Function Name:CS_CA_MMI_GetSmcStatus
* ��ȡ���ܿ���ǰ״̬
*
*Parameters 
*	pcSmcStatus[In/Out]:�洢���ܿ�״̬���������ֵΪ0����OUT��Ϊ2��ok�����������error
*
*Return Values: 
*	SUCCESS ��ȡ���ܿ���ǰ״̬�ɹ� 
*	FAILURE �����������
**************************************************************************/
INT CS_CA_MMI_GetSmcStatus(BYTE* pcSmcStatus);

/**************************************************************************
*Function Name:CS_CA_MsgCancelOsd
* ֪ͨCAȡ��OSD��ʾ(����������Ŀ,����������Ŀ
* Verifier��û�������������OSD����Ϣ����ҪAPPǿ���˳�����֪ͨCA)
*
*Parameters 
*	��
*
*Return Values: 
*	SUCCESS ֪ͨCA�ɹ� 
*	FAILURE ֪ͨCAʧ��
**************************************************************************/
DWORD CS_CA_MsgCancelOsd(void);

/**************************************************************************
*Function Name:CSCAGetIrdSerialNum
* ��ȡCA Identify Num(����֧�ָ߼���ȫ��оƬ��ֵ
* ΪChip ID+0x80000000)
*
*Parameters 
*		pdwNum[in/out]:CA Identify Num
*            
*
*Return Values: 
*	0 : SUCCESS �ɹ���ȡ��CA Identify Num
*   -1: FAILURE ��������
**************************************************************************/
INT CSCAGetIrdSerialNum(DWORD *pdwNum);

/**************************************************************************
*Function Name:CSCAGetDeviceId
* ��ȡNDS SVP Device Id
*
*Parameters 
*		pacDeviceId[in/out]:APP����ռ䱣���ַ�����ʽ��Device id�ţ�
*							�ó��ȱ������8 bytes*    
*            pnLen[in/out]:APP֪ͨCA����Ŀռ�Ĵ�С
*
*Return Values: 
*	0 : SUCCESS �ɹ���ȡ��Device Id��
*   -1: FAILURE ��������
**************************************************************************/
INT CSCAGetDeviceId(BYTE* pacDeviceId, INT* pnLen);

/**************************************************************************
*Function Name:CSCAGetCaStbLabel
* ��ȡCA STB LABEL��Ҫ��ʾ�������Ϣ������
*
*Parameters 
*		pstCaStbLabelInfo[in/out]:�洢CA STB LABEL��CHIP ID    
*
*Return Values: 
*	0 : SUCCESS ��ȡ��Ϣ�ɹ�
*   -1: FAILURE ��������
**************************************************************************/
INT CSCAGetCaStbLabel(T_CaStbLabelInfo* pstCaStbLabelInfo);

#define CA_ESPID_MAX_NUM     (8)
#define CA_EMMPID_MAX_NUM    (8)

typedef struct _CSCAEcmPidInfo
{
	WORD     m_wEsPid;
	WORD     m_wEcmPid;
}CSCAEcmPidInfo_S;

typedef struct _CSCAEmmPidInfo
{
	WORD     m_wDmxId;
	WORD     m_wEmmPid;
}CSCAEmmPidInfo_S;

typedef struct _CASpecificInfo
{
	WORD     m_wEmmPidNum;
	WORD     m_wEsNum;
	CSCAEcmPidInfo_S m_astEcmPidInfo[CA_ESPID_MAX_NUM];
	CSCAEmmPidInfo_S m_astEmmPidInfo[CA_EMMPID_MAX_NUM];
}CASpecificInfo_S;

/**************************************************************************
*Function Name:CSCAGetCASpecificInfo
* ��ȡ��ǰ���Ž�Ŀ��ECM/EMM PID��Ϣ
*
*Parameters
*       hService[in]     : ��������ʱCA���ص�Handel���������CA��USP���ƵĻ�
*          �����õ���CS_CA_StartSrvPlay���׽ӿڣ���hService��0xFFFFFFFF
*		pstCaInfo[in/out]: �洢��ǰ���Ž�Ŀ��ECM/EMM PID��Ϣ
*
*Return Values: 
*	0 : SUCCESS ��ȡ��Ϣ�ɹ�
*   -1: FAILURE ��������
**************************************************************************/
INT CSCAGetCASpecificInfo(HCSHANDLE hService, CASpecificInfo_S* pstCaInfo);


#ifdef  __cplusplus
}
#endif

#endif //_CS_NDSCA_INTERFACE_H


