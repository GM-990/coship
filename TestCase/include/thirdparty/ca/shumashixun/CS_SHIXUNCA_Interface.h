/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_SHIXUNCA_Interface.h  
* Author:      
* Date:        June 2006
* Description: Sumashixun CA interface
*            
*****************************************************************************************************************************/
#include "udiplus_typedef.h"
#include "cs_cainterface.h"
/****************************************************************************************************************************
* 			                                     Revision History                                                           *
*****************************************************************************************************************************
*   Revision  : V1.0
*   Programmer: 
*   Create    : 
*****************************************************************************************************************************/
#ifndef _CS_SHIXUNCA_INTERFACE_H_
#define _CS_SHIXUNCA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define CS_SHIXUNCA_MESSAGE_BASE    		(0)
#define CS_SHIXUNCA_INFO_MESSAGE_BASE    	(100)
#define CS_SHIXUNCA_CMD_MESSAGE_BASE    	(200)
#define CS_SHIXUNCA_STATUS_MESSAGE_BASE    	(300)
#define CS_SHIXUNCA_ALARM_MESSAGE_BASE    	(400)
#define CS_SHIXUNCA_MMI_MESSAGE_BASE    	(500)


typedef enum _CaEventGroup
{
	CA_ALARM = CS_SHIXUNCA_MESSAGE_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,	
	CA_EVENTGROUP_MAX
}E_CaEventGroup;

typedef enum _CaInfo
{
	CA_INFO_START = CS_SHIXUNCA_INFO_MESSAGE_BASE,//(CS_SHIXUNCA_MESSAGE_BASE + CA_EVENTGROUP_MAX + 1), /*Information Start*/
	CA_INFO_SMC_STATUS,      /*dwParm2 = (DWORD)&T_SmcInfo*/
	CA_INFO_PPID,            /*dwParm2 = (DWORD)&T_PPIDList*/
	CA_INFO_PPID_DETAIL,     /*dwParm2 = (DWORD)&T_DetailPPIDInfo*/
	CA_INFO_TXT,             /*dwParm2 = (DWORD)&T_Msg*/

	CA_INFO_PG_LEVEL,        /*dwParm2 = (DWORD)&T_RatingValue*/
	CA_INFO_WORKTIME,        /*dwParm2 = (DWORD)&T_WorkTime*/
	CA_INFO_PAIR_INFO,       /*dwParm2 = (DWORD)&T_PairInfo*/
	CA_INFO_GET_BOOK_IPP,    /*dwParm2 = (DWORD)&T_IppsInfo*/
	CA_INFO_VIEWED_IPP,      /*dwParm2 = (DWORD)&T_IppsInfo*/
	
	CA_INFO_PPID_MONEY,      /*dwParm2 = (DWORD)&T_MoneyInfo*/
	CA_INFO_AREA_LOCK_INFO,  /*dwParam2 = (DWORD)&T_AreaInfo*/
	CA_INFO_CARD_MOTHER_INFO,/*dwParam2 = (DWORD)&SmcMotherInfo_S*///cxj add
	
	CA_INFO_MAX
}E_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_START = CS_SHIXUNCA_CMD_MESSAGE_BASE,//(CS_SHIXUNCA_MESSAGE_BASE + CA_INFO_MAX + 1), /*Command Start*/
	CA_COMMAND_IPP_REQUEST,          /*dwParm2 = (DWORD)&T_PpvDetailInfo*/
	CA_COMMAND_FORCE_MSG_HIDE,
	CA_COMMAND_FORCE_MSG_SHOW,       /*dwParm2 = (DWORD)&T_FingerPrintInfo*/
	CA_COMMAND_IPP_CONFIRM_REQUEST,  /*dwParm2 = (DWORD)&T_PpvDetailInfo*/
	CA_COMMAND_URGENCY_BROADCAST,    /*dwParm2 = (DWORD)&T_UrgencyBroadcast*/
	CA_COMMAND_CANCEL_URGENCY_BROADCAST,/*Cancel Urgency Broadcast*/
	CA_COMMAND_CLEAN_MAIL_FLAG,         /*Clean Mail Flag*/
	CA_COMMAND_CHANGESERVICE,			/*Param2 = CS_CA_GetLockFrequency() �ķ���ֵ*/
	CA_COMMAND_AREALOCK_OVER,			/*Param2 = E_AreaLockStatus */
	CA_COMMAND_CONFIRM_FREEPREVIEV,     /*Param2 = bConfirmed */
	CA_COMMAND_RESET_STBPIN,     		/*Param2 = (DWORD)&T_STBPin */

	CA_COMMAND_MAX	
}E_CaCommand;

typedef enum _CaStatus
{
	CA_STATUS_START = CS_SHIXUNCA_STATUS_MESSAGE_BASE,//(CS_SHIXUNCA_MESSAGE_BASE + CA_COMMAND_MAX + 1), /*Status Start*/
	CA_STATUS_PIN_ERROR,               /*Pin Code Error*/
	CA_STATUS_PIN_OK,                  /*Pin Code Ok*/
	CA_STATUS_MODIFY_PIN_SUCCESS,      /*Modify Pin Code Success*/
	CA_STATUS_MODIFY_PR_LEVEL_SUCCESS, /*Modify Parent Rating Success*/
	
	CA_STATUS_MODIFY_PR_LEVEL_FAIL,    /*Modify Parent Rating Fail*/
	CA_STATUS_GET_PPID_INFO_FAIL,      /*Get PPID Information Fail*/
	CA_STATUS_GET_DETAIL_PPID_INFO_FAIL, /*Get Detail PPID Information Fail*/
	CA_STATUS_PIN_LEN_ERROR,           /*Pin Code Date Length Error*/
	CA_STATUS_PIN_LOCK,                /*Pin Code Locked*/
	
	CA_STATUS_GET_SMC_INFO_ERROR,      /*10Get Smart Card Information Fail*/
	CA_STATUS_GET_RATING_ERROR,        /*Get Rating Fail*/
	CA_STATUS_GET_WORKTIME_FAIL,       /*Get Work Time Fail*/
	CA_STATUS_SET_WORKTIME_SUCCESS,    /*Set Work Time Success*/
	CA_STATUS_SET_WORKTIME_FAIL,       /*Set Work Time Fail*/
	
	CA_STATUS_GET_PAIR_INFO_FAIL,      /*Get Son And Mother Pair Information Fail*/
	CA_STATUS_PAIRING_SUCCESS,         /*Son And Mother Pair Success*/
	CA_STATUS_PAIRING_FAIL,            /*Son And Mother Pair Fail*/
	CA_STATUS_GET_PPID_MONEY_INFO_FAIL,/*Get PPID Money Information Fail*/
	CA_STATUS_NEED_CHECK_PIN_CODE,    /*Please Check Pin Code First*/
	
	CA_STATUS_PPID_INVALID,           /*20PPID Invalid*/
	CA_STATUS_PRODUCT_EXIST,          /*This Product Has Existed*/
	CA_STATUS_NO_SPACE,               /*There Is No Space*/	
	CA_STATUS_PRODUCT_NOT_FOUND,      /*This Product Not Found*/	
	CA_STATUS_PRICE_INVALID,          /*This Price Is Invalid*/
	
	CA_STATUS_CONFLICT,               /*Time Conflictû���õ�*/
	CA_STATUS_COMMUNION_WITH_IC_ERROR,/*Communion with IC card Error*/
	CA_STATUS_MONEY_PACKET_NOT_FOUND, /*Not Found Money Packet*/	
	CA_STATUS_IPPV_EXPIRED,           /*IPP Expired*/	
	CA_STATUS_IPPV_NO_ENOUGH_MONEY,   /*Not Enough Money To Purchase This Program*/
	
	CA_STATUS_SMC_OUTERROR,           /*30Smart Card Is Out Or Error*/
	CA_STATUS_GET_IPP_INFO_FAIL,      /*Get IPP Information Fail*/
	CA_STATUS_BOOK_IPP_OK,            /*Book IPP OK*/
	CA_STATUS_BOOK_IPP_FAIL,          /*Book IPP Failû���õ�*/
	CA_STATUS_GET_VIEWED_IPP_INFO_FAIL,/*Get Viewed IPP Inforamtion Fail*/
	CA_STATUS_GET_AREA_LOCK_INFO_FAIL,    /*Get Area Lock Information Fail*/
	CA_STATUS_GET_MOTHER_INFO_FAIL,    /*36Get Card Mother Info Fail*///cxj add
	CA_STATUS_SMC_OK,    			   /*smartcard ok */
	CA_STATUS_SMC_IN,					/*smartcard in*/
	CA_STATUS_SMC_ERROR,				/*smartcard error*/
	CA_STATUS_MAX
}E_CaStatus;

typedef enum _CaAlarm
{
	CA_ALARM_START = CS_SHIXUNCA_ALARM_MESSAGE_BASE,//(CS_SHIXUNCA_MESSAGE_BASE + CA_STATUS_MAX + 1),/*Alarm Start*/
	CA_ALARM_FREE_PROGRAME,       /*Free Programe*/
	CA_ALARM_SMC_OUT,             /*Please Insert Smart Card*/
	CA_ALARM_SMC_UNKNOW,          /*Unknow Smart Card*/
	CA_ALARM_SMC_CHECKING,        /*Checking Smart Card*/
	
	CA_ALARM_SMC_IN,              /*5Smart Card In*/	
	CA_ALARM_SMC_OK,              /*6Smart Card Ok*/
	CA_ALARM_SMC_ERROR,           /*7Smart Card Error*/	
	CA_ALARM_STB_SMC_NOT_MATCH,   /*STB and SC not Match*/
	CA_ALARM_PR_LEVEL_TOO_LOW,    /*Parental Rating too lowly*/
	
	CA_ALARM_OTHER_CA_ENCRYPT,      /*10Unknow encrypt Program*/
	CA_ALARM_WITH_SUMAVISION_CA,    /*Su Ma Shi Xun CA*/
	CA_ALARM_NO_RIGHT,              /*Have No Decrypt Right*/	
	CA_ALARM_NOT_IN_WATCH_TIME,      /*Program Not In Watch Time*/
	CA_ALARM_SMC_IS_PAIRED_OTHER,    /*Smart Card Is Paired With Other STB*/
	
	CA_ALARM_NO_ENTITLE_PROGRAM,     /*15Not Order This Program*/	
	CA_ALARM_PROGRAM_RESTRICT,       /*This Program Is Restricted��Ŀ�������տ�*/
	CA_ALARM_AREA_RESTRICT,          /*This Program Is Restricted Watching At This Area*/
	CA_ALARM_MOTHER_RESTRICT,        /*Please Match With Mother Card*/
	CA_ALARM_PPV_NOT_ENOUGH_TOKENS,  /*Not Enough Money To Purchase This Program*/
	
	CA_ALARM_IPPV_NO_CONFIRM,        /*20This is IPPV-P Program, Please Confirm This Program*/	
	CA_ALARM_IPPV_NO_BOOK,           /*This is IPPV-P Program, Please Book This Program*/
	CA_ALARM_IPPT_NO_CONFIRM,        /*This is IPPV-T Program, Please Confirm This Program*/
	CA_ALARM_IPPT_NO_BOOK,           /*This is IPPV-T Program, Please Book This Program*/
	CA_ALARM_IPPV_NO_ENOUGH_MONEY,   /*This is IPPV-P Program, Not Enough Money To Purchase This Program*/

	CA_ALARM_IPPT_NO_ENOUGH_MONEY,   /*25This is IPPV-T Program, Not Enough Money To Purchase This Program*/
	CA_ALARM_SMC_NOT_SERVER,         /*This Smart Card Is Forbided ServerIC������ֹ����*/
	CA_ALARM_NO_ROOM_FOR_NEW_MAIL,   /*There Is No Room To Store New Email*/
	CA_ALARM_CLEAN_MSG,              /*Clean All Alarm Message*/

	CA_ALARM_DATA_INVALID,           /*Data Invalid*/
	CA_ALARM_MONEY_PACKET_NOT_FOUND, /*30Money Packet Not Found*/
	CA_ALARM_TVS_NOT_FOUND,          /*TVS(��Ӫ��)Not Found*/
	CA_ALARM_KEY_NOT_FOUND,          /*Key(��Կ)Not Found*/
	CA_ALARM_IPPNEED_CALLBACK,       /*Ipp Need Callback(�ش�)*/
	CA_ALARM_GET_ARER_FAILED,		/*CA_ALARM_GET_ARER_FAILED */
	
	CA_ALARM_FREE_PREVIEWING,       /*This is FreePreview Program */
	CA_ALARM_CANCEL_FREE_PREVIEW,    /*Cancel FreePreview Program */
	
	CA_ALARM_ERROR_MAX
}E_CaAlarm;

typedef enum _SmsStatus
{
	CA_SMC_OUT,
	CA_SMC_INIT,
	CA_SMC_IN,
	CA_SMC_ERROR
}E_SmsStatus;

typedef enum _AreaLockStatus
{
	E_AREALCOK_SUCCESS,
	E_AREALCOK_TIMEOUT,
	E_AREALCOK_MAX
}E_AreaLockStatus;

typedef enum _LanguageType
{
	E_CHINESE_SIM_LANGUAGE = 1, //��������
	E_ENGLISH_LANGUAGE,     //Ӣ��
	E_LANGUAGE_MAX
}E_LanguageType;


#define CA_MAX_PPID_NAME_LEN       (21)
#define CA_MAX_PRODUCT_NAME_LEN    (21)
#define CA_MAX_SERVICE_NAME_LEN    (21)
#define CA_MAX_IPPV_NUM            (250)
#define CA_IPP_OTHER_INFO_LEN      (44)

typedef struct _CaDateTime
{
	DWORD    m_dwYear;
	BYTE     m_cMonty;
	BYTE     m_cDay;
	BYTE     m_cHour;
	BYTE     m_cMinute;
	BYTE     m_cSecond;
}T_CaDateTime;


typedef struct _IppDetailInfo
{
	BYTE     m_cProgramType; //1:IPPV, 0:IPPT
	WORD     m_cEcmPid;      //ECM PID��û��Ҫ����ʾ
	WORD     m_wPPID_Id;     //��Ӫ��ID
	BYTE	 m_cCurInterval; //ֻ��ΪIPPV-T��Ŀʱ�����ֶβ���Ч���շѼ��,��λ�� m_ucUnit����
	WORD     m_wProduceId;   //��ƷID
	BYTE     m_cSlotId;      //Ǯ��ID
	BYTE     m_acProductName[CA_MAX_PRODUCT_NAME_LEN]; //��Ʒ����
	BYTE     m_acPPID_Label[CA_MAX_PPID_NAME_LEN];     //��Ӫ������
	DWORD    m_dwStartTime;  //����Ԥ������ʱ�ش�����������ʱ���ݸ��ֶν��бȽ�
	T_CaDateTime m_stStartTime;
	DWORD    m_dwDuration;   //��������
	BYTE     m_acServiceName[CA_MAX_SERVICE_NAME_LEN];
	WORD     m_wTppvTapPrice;   //���ش�����¼��۸�
	WORD     m_wTppvNoTapPrice; //���ش�������¼��۸�
	WORD     m_wCppvTapPrice;   //Ҫ�ش�����¼��۸�
	WORD     m_wCppvNoTapPrice; //Ҫ�ش�������¼��۸�
	
	//begin �����ֶ����û�����Ԥ������ʱ��UI��䴫��CA�������Ѿ��������Ĺ���IPPT��Ŀ��ǰ�۸�����ʾ�ü۸�
	WORD     m_wBookedPrice;    //�Ѿ�Ԥ���ļ۸�*/
	BYTE     m_cBookedPriceType;//Ԥ���۸����ͣ�0:TppTap;1:TppNoTap;2:CppTap;3:CppNoTap;
	//end
	
	BYTE     m_cBookedInterval; //ֻ��ΪIPPV-T��Ŀʱ�����ֶβ���Ч
	
	BOOL     m_bConfirmed;        //�Ƿ���ȷ�Ϲ�
	BOOL     m_bBooked;           //�Ƿ���Ԥ��
	DWORD    m_dwPayTotalBalance; //��Ǯ�ܵ���
	BYTE     m_cIppStatus;
	//CAS5200����
	BYTE	 m_ucUnit;			  //�շѼ���ĵ�λ0 -����1-Сʱ2-��3-��4-��
	WORD	 m_wIpptPeriod; 	  //�û�����IPPT�Ĺۿ�������,for Philippines LongIPPT���û�����Ԥ������ʱ��UI��䴫��CA��
}T_IppDetailInfo;

typedef struct _IppsInfo
{
	DWORD    m_dwIppCount;
	T_IppDetailInfo m_astIppInfo[CA_MAX_IPPV_NUM];
}T_IppsInfo;

typedef struct _ViewedIppDetailInfo
{
	WORD	m_wPPID;                                    //��Ӫ�̱��
	BYTE    m_acPPID_Label[CA_MAX_PPID_NAME_LEN];       //��Ӫ������
	BYTE	m_acProductName[CA_MAX_PRODUCT_NAME_LEN]; //��Ʒ����
	DWORD	m_dwStartTime;                              //��ʼʱ�䣬time_t��ʽ
	T_CaDateTime m_stStartTime;
	DWORD	m_dwDuration;                                //��������
	WORD	m_wBookedPrice;                              //Ԥ���۸�(��)
	BYTE	m_cBookedPriceType;                          //Ԥ���۸����ͣ�0:TppTap;1:TppNoTap;2:CppTap;3:CppNoTap;
	BYTE	m_cBookedInterval;                           //Ԥ���շѼ����IPPVʱ��ֵΪ0
	BYTE    m_acOtherInfo[CA_IPP_OTHER_INFO_LEN];        //ippvʱΪ���˲�ƷΪippv��Ʒ����ipptʱΪ���ۿ���ʱ�䣺�����ӣ���Ǯ���������֡�
	//CAS5200����
	BYTE	 m_ucUnit;			  //�շѼ���ĵ�λ0 -����1-Сʱ2-��3-��4-��
}T_ViewedIppDetailInfo;

typedef struct _ViewedIppsInfo
{
	DWORD    m_dwViewedIppCount;
	T_ViewedIppDetailInfo m_astIppInfo[CA_MAX_IPPV_NUM];
}T_ViewedIppsInfo;

#define	 CA_MAX_FINGERPRINT_CONTENT_LEN	  (20)
typedef struct _FingerPrintInfo
{
	BOOL     m_bPriporityHigh; //���ȼ�������ָ����Ϣ�������ȼ����ߣ�ΪTRUE
	WORD     m_wDuration;      //ָ����ʾʱ�䳤�ȣ���λ:��
	BYTE     m_acMsg[CA_MAX_FINGERPRINT_CONTENT_LEN]; //ָ������
}T_FingerPrintInfo;//ָ����Ϣ

/*Mail*/
#define CA_MAIL_TITLE_LEN         (21)
#define CA_MAIL_SENDER_NAME_LEN   (11)
#define CA_MAIL_TXT_LEN           (1025)

typedef struct _Msg
{
	DWORD    m_dwVersion;       //��m_bPriporityHighΪTRUEʱ����ʾOSD�����ȼ�����m_bPriporityHighΪFALSEʱ����ʾCAS�ʼ��汾
	BOOL     m_bPriporityHigh;  //FALSE: Mail  TRUE: OSD
	WORD     m_wDuration;       //ֻ��m_bPriporityHighΪTRUE����Ч,if wDuration is 0, this means display once
	BYTE     m_szSenderName[CA_MAIL_SENDER_NAME_LEN];//����������
	DWORD    m_dwTitleLen;                 //���ⳤ��
	BYTE     m_szTitle[CA_MAIL_TITLE_LEN]; //����
	DWORD    m_dwContentLen;               //�ʼ����ݳ���
	BYTE     m_szContent[CA_MAIL_TXT_LEN]; //�ʼ�����
	T_CaDateTime    m_stSendTime;          //�ʼ�����ʱ��
}T_Msg;

typedef struct _CaMail
{	
	BOOL            m_bReadFlag;    //�Ѷ���־
	T_CaDateTime    m_stArriveTime; //�ʼ������ʱ��
	T_Msg           m_stMsg;
}T_CaMail;

#define CA_MAX_CAS_VER_LEN         	(10)
#define CA_MAX_COS_VER_LEN         	(10)
#define CA_MAX_SMC_NUMBER_LEN      	(17)
#define CA_MAX_SERCIAL_NO_LEN      	(64)
#define CA_MAX_CAS_NAME_LEN        	(21)
#define CA_MAX_STBPIN_LEN  	   		(8)

typedef struct _SmcInfo
{
	BYTE      m_cSmcStatus;//0:OUT, 1:IN
	WORD      m_wSystemId; //CAϵͳID��
	BOOL      m_bIsLock;   //TRUE:Lock FALSE:Unlock
	BYTE      m_acCasVer[CA_MAX_CAS_VER_LEN]; //���ܿ�CAS�汾��Ϣ
	BYTE      m_acCosVer[CA_MAX_COS_VER_LEN]; //���ܿ�COS�汾��Ϣ
	BYTE      m_acSmcNumber[CA_MAX_SMC_NUMBER_LEN]; //���ܿ���
	BYTE      m_acSercialNo[CA_MAX_SERCIAL_NO_LEN]; //���ܿ����к�
	BYTE      m_acCasManuName[CA_MAX_CAS_NAME_LEN]; //CAS��Ӧ������
	BYTE      m_acStbCasVer[CA_MAX_CAS_VER_LEN];    //CAS�汾��
	T_CaDateTime m_stSmcExpireTime; //���ܿ�����Ȩʱ��
	BYTE      m_cParentalRating;    //���ܿ��ۿ�����
}T_SmcInfo;

/*cxj add begin*/
typedef struct _SmcMotherInfo
{	
	T_CaDateTime m_sSetTime; //����ʱ��
	DWORD        m_dwMotherCardID;//��pdwMotherCardID��ֵΪ0�����ʾ��ǰ��Ϊĸ���� ���ֵ��Ϊ0����ʾ��ǰ��Ϊ�ӿ���ֵΪĸ������
	BYTE         m_ucFlag;//6bits������2bitsʹ�ñ�ʶ
	DWORD        m_dwDecryptTime;//����CW�Ĵ���
	DWORD        m_dwInterTime;//�տ����ʱ��
	T_CaDateTime m_sCorrespondTime;//�ӿ����ʱ��	
}SmcMotherInfo_S;
/*cxj add end*/

#define CA_MAX_PPID_NUM       (10)
typedef struct _PPID
{
	DWORD    m_dwPPIDId; //��Ӫ��ID��
	BYTE     m_acPPIDName[CA_MAX_PPID_NAME_LEN];//��Ӫ������
	BYTE     m_cServiceStatus;
}T_PPIDInfo;//��Ӫ�̻�����Ϣ

typedef struct _PPIDInfo
{
	BYTE         m_acCount;
	T_PPIDInfo   m_stPPIDInfo[CA_MAX_PPID_NUM];
}T_PPIDList;


#define CA_MAX_SERVICE_ENTITLE_NUM   (255)
typedef struct _ServiceEntitle
{
	WORD         m_wProductId;    //��ƷID��
	T_CaDateTime m_stEntitleTime; //��Ȩʱ��(����ʱ��)
	BOOL         m_bTapingFlag;   //¼�Ʊ�־
	T_CaDateTime m_stStartTime;   //��ʼʱ��
	T_CaDateTime m_stEndTime;     //����ʱ��
	BYTE         m_acProductName[CA_MAX_PRODUCT_NAME_LEN];//��Ʒ����
}T_ServiceEntitle;

typedef struct _DetailPPIDInfo
{
	DWORD    m_dwPPIDId;      //��Ӫ��ID��
	BYTE     m_cEntitleCount; //��Ȩ����
	T_ServiceEntitle m_stServicEntitle[CA_MAX_SERVICE_ENTITLE_NUM];
}T_DetailPPIDInfo;

typedef struct _RatingValue
{
	BYTE      m_cRatingValue; //�ۿ�����
}T_RatingValue;

typedef struct _WorkTime
{
	//����24Сʱ��ʱ��
	BYTE      m_cStartTimeHour;
	BYTE      m_cStartTimeMinute;
	BYTE      m_cEndTimeHour;
	BYTE      m_cEndTimeMinute;
}T_WorkTime;

typedef struct _UrgencyBroadcast
{
	WORD      m_wOriNetID;  //ԭʼ����ID��
	WORD      m_wTsID;      //��ID��
	WORD      m_wServiceID; //��ĿID��
	WORD      m_wDuration;  //Ӧ���㲥����ʱ��
}T_UrgencyBroadcast;

#define CA_MAX_PAIR_INFO_LEN   (250)
typedef struct _PairInfo
{
	BYTE      m_cLen;
	BYTE      m_acData[CA_MAX_PAIR_INFO_LEN]; //��ĸ�������Ϣ
}T_PairInfo;

typedef struct _MoneyInfo
{
	WORD      m_wPPID;        //��Ӫ��ID
	DWORD     m_dwAllBalance; //�ѻ���Ǯ��
	DWORD     m_dwRemainder;  //ʣ��Ǯ��
}T_MoneyInfo;//����ѻ���Ǯ����ʣ��Ǯ����Ϊ0�����ʾ��ȡǮ����Ϣʧ��

typedef struct _AreaInfo
{
	DWORD			m_dwCardArea;			//����������
	DWORD			m_dwSetCardAreaTime;	//�趨����ʱ�䡣
	BYTE			m_cStartFlag;			//������־��1:������0:û�С�
	DWORD			m_dwSetFlagTime;		//���ñ�־ʱ�䡣
	DWORD			m_dwSetStreamTime;		//��������������ʱ�䡣
	DWORD			m_dwIntervalTime;		//���е�ǰʱ���ȥ������ʱ������ֵ��
	DWORD			m_dwStreamMinArea;		//��������С������
	DWORD			m_dwStreamMaxArea;		//���������������
	T_CaDateTime    m_stSetCardAreaTime; 	//����ʱ��
}T_AreaInfo;

typedef struct _STBPin
{
	BYTE			m_acSTBPIN[CA_MAX_STBPIN_LEN];		//������PIN��
	BYTE			m_cLen;								//PIN��ĳ���
}T_STBPin;

typedef struct _CaShumaPrivate
{
    BOOL m_bFakeEeprom; //TRUE: ����αEEPROM ����
    DWORD m_dwFakeEepromAddr; //αEEPROM��ַ
    DWORD m_dwFakeEepromLength; //αEEPROM���� 
    DWORD m_dwFakeEepromBakAddr; //αEEPROM ���� ��ַ
    DWORD m_dwFakeEepromBakLength; //αEEPROM���ݳ��� 
    
    DWORD m_dwReseve1;
    DWORD m_dwReseve2;
    DWORD m_dwReseve3;
    DWORD m_dwReseve4;
}CaShumaPrivate_S;
/**************************************************************************
*Function Name: CS_CA_GetCoreVer
*  ��ȡ��ͨCA�ں˰汾��Ϣ
*
*Parameters 
*	pcVer[out]:
*		���ڴ洢CA�ں˰汾��Ϣ���� 
*   pnLen[in|out]:
*       �����ʼֵΪ30��������ʵֵ
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
*	0 :  ���ĳһ���ʼ��ɹ�
*	-1:  �����Ŵ����ɾ���ʼ�ʧ��
*    1:  ���Ǵ��û����ʼ�����ɾ��
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
*	0 :  ɾ�������ʼ��ɹ�
*	-1:  ɾ�������ʼ�ʧ��
*    1:  ���Ǵ��û����ʼ�����ɾ��
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
*	0 : ��ȡ�ʼ����ݳɹ�
*	-1: �����Ŵ����������󣬻��ȡ�ʼ����ݴ���
*    1: ���Ǵ��û����ʼ����ܲ�ѯ
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
*Function Name:CS_CA_MMI_CheckPinCode
* �����������
*
*Parameters 
*	pstPinCode[in]:��������
*
*Return Values: 
*	0 : SUCCESS �����Ϸ��������ͨ������Ϣ��UI
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_CheckPinCode(DWORD dwPin);

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
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_ModifyPinCode(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestBookPpvMenu
* ��ȡԤ��|�˶�PPV��Ϣ
*
*Parameters 
*   ��
*
*Return Values: 
*	0 : ������ͨ������Ϣ��UI
**************************************************************************/
INT CS_CA_MMI_RequestBookPpvMenu();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestViewedPpvManu
* ��ȡ�ѹۿ���PPV��Ϣ
*
*Parameters 
*   ��
*
*Return Values: 
*	0 : ������ͨ������Ϣ��UI
**************************************************************************/
INT CS_CA_MMI_RequestViewedPpvManu();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestViewedPpvManu
* Ԥ��|�˶�	IPP��Ŀ
*
*Parameters 
*   pstIppInfo[IN]
*
*Return Values: 
*	0 : ������ͨ������Ϣ��UI
*	    Ԥ��ʱ��Ҫ����������Ϣ:
*			CA_STATUS_BOOK_IPP_OK//Ԥ���ɹ�
*			CA_STATUS_IPPV_NO_ENOUGH_MONEY//Ԥ���ɹ��������࣬�뼰ʱ��ֵ
*			CA_STATUS_NEED_CHECK_PIN_CODE//��Ҫͨ��PIN��֤
*			CA_STATUS_COMMUNION_WITH_IC_ERROR//��IC��ͨѶ��������
*			CA_STATUS_PPID_INVALID//��Ӫ��ID��Ч
*			CA_STATUS_MONEY_PACKET_NOT_FOUND//Ǯ��û�з���
*			CA_STATUS_IPPV_EXPIRED//��Ʒ�Ѿ�����
*			CA_STATUS_PRODUCT_EXIST//��Ʒ�Ѿ����ڣ����ܲ���
*			CA_STATUS_NO_SPACE//û�пռ�
*			CA_STATUS_PRODUCT_NOT_FOUND//��Ʒû�з���
*			CA_STATUS_PRICE_INVALID//�۸���Ч
*			CA_STATUS_BOOK_IPP_FAIL//δ֪����
*		�˶�ʱ:
*			CA_STATUS_BOOK_IPP_OK//�˶��ɹ�
*			����:�˶�ʧ�ܣ�������
*  -1 : �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_PpvBook(T_IppDetailInfo* pstIppInfo);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestViewedPpvManu
* �˳��ڲ��Ž��浯���Ĺ���IPP��Ŀ����������
*
*Parameters 
*   wEcmPid[IN] : ��ǰIPP��Ŀ��ECMPID
*
*Return Values: 
*	0 : SUCCESS
*
*��ע: ��·���ſ�ʹ�ô˽ӿ�,��·���ſ���ʹ��CS_CA_MMI_PpvCancelEX
**************************************************************************/
INT CS_CA_MMI_PpvCancel(WORD wEcmPid);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvCancelEX
* �˳��ڲ��Ž��浯���Ĺ���IPP��Ŀ����������
*
*Parameters 
*   wEcmPid[IN] : ��ǰIPP��Ŀ��ECMPID
*   hService[IN]: ��ǰ��Ŀ���ŵľ��(CSCAStartService�õ��ľ��)
*
*Return Values: 
*	0 : SUCCESS
*
*��ע: ��·���ſ�ʹ�øýӿڡ�
**************************************************************************/
INT CS_CA_MMI_PpvCancelEX(WORD wEcmPid, HCSHANDLE hService);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPPIDMoneyInfo
* ��ȡ��Ӧ��Ӫ������Ǯ����ʣ��Ǯ����Ϣ
*
*Parameters 
*   wPPID[in]:��Ӫ��ID
*
*Return Values: 
*	0 : SUCCESS Ŀǰ�ú���ֻ�᷵�سɹ��������ͨ������Ϣ��UI
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_RequestPPIDMoneyInfo(WORD wPPID);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestWorkTime
* ��ȡ����ʱ��
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS Ŀǰ�ú���ֻ�᷵�سɹ��������ͨ������Ϣ��UI
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_RequestWorkTime();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyWorkTime
* ���ù���ʱ��
*
*Parameters 
*	dwPin[in]:����PinCode
*	pstWorkTime[in]:��ʼ�ͽ�������ʱ��
*
*Return Values: 
*	0 : SUCCESS �����Ϸ��������ͨ������Ϣ��UI
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_ModifyWorkTime(DWORD dwPin, T_WorkTime* pstWorkTime);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestSmcInfo
* �������ܿ���Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS Ŀǰ�ú���ֻ�᷵�سɹ��������ͨ������Ϣ��UI
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_RequestSmcInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPPIDInfo
* ����PPID��Ϣ(��Ӫ����Ϣ)
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS Ŀǰ�ú���ֻ�᷵�سɹ��������ͨ������Ϣ��UI
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_RequestPPIDInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestDetailPPIDInfo
* ����PPID��ϸ��Ϣ(��Ӫ����ϸ��Ϣ)
*
*Parameters 
*	pstPin[in]:����
*   dwPpId:��Ӫ��Id��
*
*Return Values: 
*	0 : SUCCESS �����Ϸ��������ͨ������Ϣ��UI
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_RequestDetailPPIDInfo(DWORD dwPin, DWORD dwPpId);

/**************************************************************************
*Function Name:CS_CA_MMI_NotifyCaShowOSDOver
* ֪ͨCAOSD��ʾ���
*
*Parameters 
*	dwDuration[in]:��ʾ�������ѵ�ʱ�䣬��λ����
*
*Return Values: 
*	0 : SUCCESS Ŀǰ�ú���ֻ�᷵�سɹ����Ƿ�ɹ�����֪ͨUI
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_NotifyCaShowOSDOver(DWORD dwDuration);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestParentRating
* ��ȡ�ۿ�����
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS Ŀǰ�ú���ֻ�᷵�سɹ�����ȡ������Ϣ���ȡ��Ϣʧ�ܽ�ͨ������Ϣ��
UI
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_RequestParentRating();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyParentRating
* �޸Ĺۿ�����
*
*Parameters 
*	pstPin[in]:��������
*   cParentRatingValue[in]:Ҫ�޸ĵļ���
*
*Return Values: 
*	0 : SUCCESS �����Ϸ���������ͨ������Ϣ��UI
*	-1: FAILURE �������Ϸ�
**************************************************************************/
INT CS_CA_MMI_ModifyParentRating(DWORD dwPin, BYTE cParentRatingValue);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPairInfo
* ��ȡ��ĸ�����������Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
Ŀǰ�ú���ֻ�᷵�سɹ�����ĸ�����������Ϣ���ȡ��Ϣʧ�ܽ�ͨ������Ϣ��UI
*	-1: FAILURE ���ã�Ŀǰ���᷵��ʧ��
**************************************************************************/
INT CS_CA_MMI_RequestPairInfo();

/**************************************************************************
*Function Name:CS_CA_Pairing
* ��ĸ�����
*
*Parameters 
*	pstPairInfo[in]: �����Ϣ
*
*Return Values: 
*	0 : SUCCESS ������ȷ������Ƿ�ɹ���ͨ������Ϣ��UI
*	-1: FAILURE ��������
**************************************************************************/
INT CS_CA_Pairing(T_PairInfo* pstPairInfo);

/**************************************************************************
*Function Name:CS_CA_MMI_IsPinLock
* ��ѯPIN���Ƿ���
*
*Parameters 
*	��
*
*Return Values: 
*	TRUE : PIN�뱻�� 
*	FALSE: PIN��δ����
**************************************************************************/
BOOL CS_CA_MMI_IsPinLock(void);

/**************************************************************************
*Function Name:CS_CA_MMI_LockPin
* ��סPIN��
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS(��PIN��ɹ�) 
*	-1: FAILURE(��PIN��ʧ��)
**************************************************************************/
INT CS_CA_MMI_LockPin(void);

/**************************************************************************
*Function Name:CS_CA_MMI_IsPinPass
* ��ѯ�Ƿ�ͨ��У������
*
*Parameters 
*	��
*
*Return Values: 
*	TRUE : ͨ��PIN��У�� 
*	FALSE: δͨ��PIN��У��
**************************************************************************/
BOOL CS_CA_MMI_IsPinPass(void);

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
INT CS_CA_MMI_GetSmcStatus(E_SmsStatus* pcSmcStatus);

/**************************************************************************
*Function Name:CS_CA_StartBoardCast
* �������ݹ㲥
*
*Parameters 
*	pcPmtData[In]:Pmt����
*	nPmtLen[In]:Pmt����
*	pcCatData[In]:Cat����
*	nCatLen[In]:Cat����
*	wTsId[In]:��ID
*	wOrginalNetworkID[In]:ԭʼ����ID
*
*Return Values: 
*	SUCCESS �������ݹ㲥�ɹ�
*	FAILURE �������ݹ㲥ʧ��
**************************************************************************/
INT CS_CA_StartBoardCast(BYTE* pcPmtData, INT nPmtLen, BYTE* pcCatData, INT nCatLen, WORD wTsId, WORD wOrgNetId);

/**************************************************************************
*Function Name:CS_CA_StopBoardCast
* ֹͣ��ǰ���ݹ㲥
*
*Parameters 
*	��
*
*Return Values: 
*	SUCCESS ֹͣ���ݹ㲥�ɹ�
*	FAILURE ֹͣ���ݹ㲥ʧ��
**************************************************************************/
INT CS_CA_StopBoardCast(void);

/************************************************************************
* s_cSupportFakeFlag = 1 ��ʾCA�����˼ټ���
* (���û�е��ô˺�����Ĭ��s_cSupportFakeFlag = 1����ʾ�����˼ټ������)
* s_cSupportFakeFlag = 0 ��ʾCA������ټ���
*************************************************************************/
INT CS_CA_SetFakeFlag(BYTE cFakeFlag);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestAreaBlockInfo
* ��ȡƵ������������Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : ����������Ϣ��ʽ�ϱ�UI
**************************************************************************/
INT CS_CA_MMI_RequestAreaBlockInfo();

/**************************************************************************
*Function Name:CS_CA_SetLanguageMode
* ������������
*
*Parameters 
*	eLanguageType[in]:��������
*
*Return Values: 
*	0 : SUCCESS
*	-1: FIAILURE
**************************************************************************/
INT CS_CA_SetLanguageMode(E_LanguageType eLanguageType);

/**************************************************************************
*Function Name:CS_CA_ProcAreaLockCatInfo
* ֪ͨ��������Ƶ���CAT��(������APP�ṩ��CS_CA_NotifyAppForbidSwitchChannel()����ʱ��APPҪ��
* <1>�洢��ǰ���ŵĽ�Ŀ��Ϣ; <2>��ת�����õ���������Ƶ��; <3>���������û�����; <4>����CAT��; 
* <5>������CAT������CS_CA_ProcAreaLockCatInfo()����, ��CAT�׸�CAģ��)
*
*Parameters 
*	pcSectionData[in]:
*		CAT ���ݻ�������ַ 
*     nLen [in]:
*		CAT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS ֪ͨCA��CAT���ݳɹ�
*	-1: FAILURE ���������֪ͨCA��CAT����ʧ��
**************************************************************************/
INT CS_CA_ProcAreaLockCatInfo(BYTE *pcData, INT nLen, BYTE cDemux);

/*cxj add begin*/
/**************************************************************************
*Function Name:CS_CA_MMI_GetMotherInfo
* ��ȡ���ܿ�����ĸ����Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : ����������Ϣ��ʽ�ϱ�UI
**************************************************************************/
INT CS_CA_MMI_GetMotherInfo(void);

/**************************************************************************
*Function Name:CSCACASDebugMsgAdd
* ����������Ѷ���д�ӡ�Ŀ���
*
*Parameters 
*	bOpen[in]:
*   TRUE,�򿪴�ӡ; FALSE,�رմ�ӡ���򿪴�ӡʱUSPPrint(ERROR_LEVEL,...)�Ĵ�ӡҪ�ܴ����
*
*Return Values: 
*	��
**************************************************************************/
void CSCACASDebugMsgAdd(BOOL bOpen);
/**************************************************************************
*Function Name:CS_CA_PDSDProc
* ���õ�ǰʹ�õ�PDSDֵ�������������
*
*Parameters
*       dwPDSD[in]     : ��ǰʹ�õ�PDSDֵ��������Ӫ�̵�����
*                        �����Ӧ��PDSDֵ��ͨ���ýӿڴ���CA
*
*Return Values: 
*	0 : SUCCESS ���óɹ�
*   -1: FAILURE ����ʧ��
**************************************************************************/
INT CS_CA_PDSDProc(DWORD dwPDSD);

/**************************************************************************
*Function Name:CS_CA_MMI_ConfirmFreePreview
* ֪ͨCA�Ƿ񻹼������Ԥ��
*
*Parameters
*       bConfirmed     : �Ƿ�ȷ�����Ԥ��
*                     
*Return Values: 
*	0 : SUCCESS ���óɹ�
*   -1: FAILURE ����ʧ��
**************************************************************************/
INT CS_CA_MMI_ConfirmFreePreview(BOOL bConfirmed);

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

/*cxj add end*/

#ifdef  __cplusplus
}
#endif

#endif //_CS_SHIXUNCA_INTERFACE_H_

