/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_SUANTONGCA_Interface.h  
* Author:      
* Date:        June 2007
* Description: SuanTong CA interface
*            
*****************************************************************************************************************************/

/****************************************************************************************************************************
* 			                                     Revision History                                                           *
*****************************************************************************************************************************
*   Revision  : V1.0
*   Programmer: 
*   Create    : Harry Peng
*****************************************************************************************************************************/
#ifndef _CS_SUANTONGCA_INTERFACE_H_
#define _CS_SUANTONGCA_INTERFACE_H_


#include "udiplus_typedef.h"
#include "cs_cainterface_pvrext.h"

#ifdef  __cplusplus
extern "C" {
#endif


#define CS_SUANTONGCA_MESSAGE_BASE    (0)
//hyb@090511
#define CA_MAX_TEXT_LEN                 (100)

typedef enum _CaEventGroup
{
	CA_ALARM = CS_SUANTONGCA_MESSAGE_BASE,    
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	CA_MAX_STATUS
}E_CaEventGroup;

typedef enum _CaInfo
{
	CA_INFO_SMC_STATUS = (CS_SUANTONGCA_MESSAGE_BASE + CA_MAX_STATUS + 1),     /*6 dwParm2 = (DWORD)&T_SmcInfo*/
	CA_INFO_SYSTEMID,        	/*dwParm2 = (DWORD)&T_CaSystemIdInfo,compliant with old version,not used now */
	CA_INFO_PPID,            	/*dwParm2 = (DWORD)&T_PPIDList*/
	CA_INFO_PPID_DETAIL,     	/*dwParm2 = (DWORD)&T_DetailPPIDInfo*/
	CA_INFO_TXT,             	/*10dwParm2 = (DWORD)&T_Msg*/
	CA_INFO_PARENTLEVEL,		/*dwParm2 = (DWORD)dwParentLevel*/
	CA_INFO_USER_VIEW,          /*dwParm2 = (DWORD)&CaUserView_S*///hyb@090511
	CA_INFO_MAX
}E_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_FINGER_PRINT  = (CS_SUANTONGCA_MESSAGE_BASE + CA_INFO_MAX + 1),   /*14 dwParm2 = (DWORD)&T_CaSpecInfo*/
	CA_COMMAND_CHANGE_PGPIN,   /*15dwParm2 = (DWORD)&T_CaPin , compliant with old version,not used now*/
	CA_COMMAND_IPPV_REQUEST,   /*16dwParm2 = (DWORD)&T_PpvInfo*/
	CA_COMMAND_PAIRING,        /*dwParm2 = (DWORD)&T_SercialInfo , compliant with old version,not used now*/
	CA_COMMAND_OTA,			   /*dwParm2 = (DWORD)&T_OtaInfo*/
    CA_COMMAND_CHNL_SWITCH,/*dwParm2 = (DWORD)&CaChnlSwitch_S*///hyb@090511

	CA_COMMAND_MAX	
}E_CaCommand;

typedef enum _CaStatus
{
	CA_STATUS_PPID_ERROR  = (CS_SUANTONGCA_MESSAGE_BASE + CA_COMMAND_MAX + 1),      //21        /*20 PPID Id Error*/
	CA_STATUS_READ_INFO_ERROR,         		/*Read Information Error*/
	CA_STATUS_PIN_ERROR,               		/*Pin Code Error*/
	CA_STATUS_PIN_OK,                  		/*Pin Code Ok*/
	CA_STATUS_MODIFY_PIN_SUCCESS,      		/*25Modify Pin Code Success*/
	CA_STATUS_MODIFY_PIN_FAIL,         		/*Modify Pin Code Fail*/
	CA_STATUS_SERIAL_ERROR,            		/*Serial Number Error*/
	CA_STATUS_SET_SERIAL_SUCCESS,      		/*28Set Serial Number Success*/
	CA_STATUS_SET_SERIAL_FAIL,         		/*29Set Serial Number Fail*/
	CA_STATUS_RECORD_FAIL,             		/*30Request Record Flag Fail*/
	CA_STATUS_ALLOW_RECORD,            		/*Allow Record*/
	CA_STATUS_REJECT_RECORD,           		/*Reject Record*/
	CA_STATUS_MODIFY_PR_LEVEL_SUCCESS, 		/*Modify Parent Rating Success*/
	CA_STATUS_MODIFY_PR_LEVEL_FAIL,    		/*Modify Parent Rating Fail*/
	CA_STATUS_GET_PPID_INFO_FAIL,      		/*35Get PPID Information Fail*/
	CA_STATUS_GET_DETAIL_PPID_INFO_FAIL, 	/*Get Detail PPID Information Fail*/
	
	CA_STATUS_MAX
	
}E_CaStatus;

typedef enum _CaAlarm
{
	CA_ALARM_FREE_PROGRAME  = (CS_SUANTONGCA_MESSAGE_BASE + CA_STATUS_MAX + 1),   /*38 E00 Free Programe*/
	CA_ALARM_SMC_OUT,             			/*E01 Please Insert Smart Card*/
	CA_ALARM_SMC_UNKNOW,          			/*40E02 Unknow Smart Card*/
	CA_ALARM_SMC_CHECKING,        			/*E03 Checking Smart Card*/
	
	CA_ALARM_SMC_OK,              			/*Smart Card Ok 04*/
	CA_ALARM_SMC_ERROR,          			/*Smart Card Error*/
	CA_ALARM_SMC_INFO_ERROR,     			/*44 Smart Card Information Error:SC_HARD_ERR*/
	CA_ALARM_SMC_STATUS_ERROR,   			/*45Smart Card Statuwords Error:SC_SOFT_ERR*/
	
	CA_ALARM_STB_SMC_NOT_MATCH  , 			/*E08 STB and SC not Match*/
	CA_ALARM_PR_LEVEL_TOO_LOW,      		/*E09 Parental Rating too lowly*/
	CA_ALARM_OTHER_CA_ENCRYPT,      		/*E10 Unknow encrypt Program*/
	
	CA_ALARM_ALLOCATE_ECM_CHANNEL_FAIL  , 	/*E11 Allocate ECM Channel/Filter Fail*/
	CA_ALARM_OPEN_DECRABLER_FAIL,           /*50E12 Open Descrambler Fail*/
	CA_ALARM_SMC_ERROR_DESCRABLING,         /*E13 SC Error when Descraming*/
	CA_ALARM_CW_ERROR,                      /*E14 CW Error*/
	CA_ALARM_NO_RIGHT,                      /*E15 Have No Decrypt Right*/
	CA_ALARM_DESCRAMBLE_SUCCESS,            /*E16 Descramble Success*/
	CA_ALARM_ALLOCATE_EMM_CHANNEL_FAIL,     /*55Alloc Emm Channel or Filter fail*/
	
	CA_ALARM_IPPV_PROGRAME ,  				/*E20 IPPV Program*/
	CA_ALARM_IPPV_PIN_ERROR,      			/*E21 IPPV PIN Error*/
	CA_ALARM_IPPV_EXPIRED,        			/*E22 IPPV Expired*/
	CA_ALARM_ORDER_IPPVP_FAIL,    			/*E23 Order IPPVP Fail*/
	CA_ALARM_ORDER_IPPVP_SUCCESS, 			/*60E24 Order IPPVP Success*/
	CA_ALARM_START_IPPVT_FAIL,    			/*E27 Order IPPV-T Fail*/
	CA_ALARM_START_IPPVT_SUCCESS, 			/*E26 Order IPPV-T Success*/
	CA_ALARM_PAY_IPPVT_FAIL,      			/*IPPVT Pay Fail*/
	CA_ALARM_STOP_IPPVT_FAIL,     			/*E29 End IPPV_T Fail*/
	CA_ALARM_STOP_IPPVT_SUCCESS,  			/*65E28 End IPPV-T Success*/

	CA_ALARM_NO_SIGNAL ,      				/*No Signal*/
	CA_ALARM_UNKNOW_SERVICE,      			/*Service Unknow*/

	CA_ALARM_TABLE_STATUS_OK ,   			/*Cat Or Pmt Status Ok*/
	CA_ALARM_CAT_STATUS_ERROR,       		/*Cat Status error*/
	CA_ALARM_STOP_PROGRAME_SUCCESS,  		/*70Stop program success*/

	CA_ALARM_STB_SMC_MATCH,          		/*STB and SC Match*/
	CA_ALARM_SMC_IN,          				/*Smart Card Insert*/

	CA_ALARM_OTA_AVAILABLE,					/*OTA Infomation Detect*/
	CA_ALARM_OTA_FAIL,						/*OTA Upgrate Failed */

	CA_ALARM_ST_PROGRAM, 					/*75If SUANTONG CA Encrypt*/

	CA_ALARM_ANTI_MOVE_CTRL,				/*E07 Anti move ctrl*/
	CA_ALARM_SON_CARD_OVERDUE,				/*E17 Son Card Authentication Overdue*/	
	CA_ALARM_USERVIEW_CONTROL_ERROR,		/*78E18 User View Control Error*/
	CA_ALARM_REGIONVIEW_CONTROL_ERROR,		/*E19 Region View Control Error*/
	CA_ALARM_CARD_PAUSE,					/*80E25 Smart Card Pause*/
	CA_ALARM_USERVIEW_SUCCESS,				/*E30 User View Success*/
	CA_ALARM_USERVIEW_FAIL,					/*E31 User View Fail*/
	CA_ALARM_SYSTEMCHECK,                   /*System checking, keep card in and power on*/
	CA_ALARM_SYSTEMCHECK_OVER,              /*System checking finished*/

	CA_ALARM_ERROR_MAX
}E_CaAlarm;
//hyb@090511
typedef enum _CaChnlSwitchType
{
    EM_CA_CHNL_FORCE_SWITCH_IMMEDIATELY= 0,      /*force switch the specify channel immediately */
    EM_CA_CHNL_FORCE_SWITCH_SPECIFY_TIME,        /*force switch the specify channel in the specify time*/
    EM_CA_CHNL_USER_SWITCH_IMMEDIATELY,          /*switch the specify channel immediately according to  user's choice*/
    EM_CA_CHNL_USER_SWITCH_SPECIFY_TIME          /*switch the specify channel in the specify time according to user's choice*/
}E_CaChnlSwitchType;
typedef enum _CaModulation
{
    EM_CA_MODULATION_QAM= 1,      /*QAM */
    EM_CA_MODULATION_QPSK,        /*QPSK*/
    EM_CA_MODULATION_BROADCAST,/*broadcast*/
    EM_CA_MODULATION_MOBILE/*mobile net*/
}E_CaModulation;
typedef enum _CaUserViewOp
{
    EM_CA_USER_VIEW_GET_INFO= 0,      /*get the infomation of user view*/
    EM_CA_USER_VIEW_SET_PARA,        /*set the parameters of user view*/
    EM_CA_USER_VIEW_CANCEL_SETTING/*cancel the settings*/
}E_CaUserViewOp;

typedef enum _CaUserViewStatus
{
    EM_CA_ALLOW_VIEW_EVERYDAY = 0,/*allow user view in a specifiedl time everyday*/
    EM_CA_ALLOW_VIEW_INTRADAY,        /*allow user view in a specifiedl time only today*/
    EM_CA_FORBIDDEN_VIEW_EVERYDAY,/*forbidden user view in a specifiedl time everyday*/
    EM_CA_FORBIDDEN_VIEW_INTRADAY       /*allow user view in a specifiedl time only today*/
}E_CaUserViewStatus;

/*not used in CAS Version 3 or above,start */
#define CA_MAX_SYSTEMID_COUNT_LEN  (16)

typedef struct _CurrentCaSystemInfo
{
	WORD     m_wSystemId;//��ͨCAϵͳID
	WORD     m_wEcmSubsystemId;//��ͨCA��ϵͳID(for ecm)
	WORD     m_wEmmSubsystemId;//��ͨCA��ӵͳID(for emm)
}T_CurrentCaSystemInfo;

typedef struct _CaSystemIdInfo
{
	BYTE     m_cCount;
	T_CurrentCaSystemInfo  m_stCaSystemInfo[CA_MAX_SYSTEMID_COUNT_LEN];
}T_CaSystemIdInfo;
/*not used in CAS Version 3 or above,end */

#define CA_MAX_PPID_NAME_LEN       (17)
typedef struct _PpvInfo
{
	INT      m_nProgramNo;//��ǰ��Ŀ��
	BYTE     m_cStatus;//0:Credit and OverCredit are Valid; 1: Credit and OverCredit are Invalid
	DWORD    m_dwPPID_Id;//��ǰIPPV��Ŀ�ṩ�̵�ID
	BYTE     m_acPPID_Label[CA_MAX_PPID_NAME_LEN];//��Ŀ�ṩ�̵�����,�ַ�������
	FLOAT    m_fCredit;     /*���*/
	FLOAT    m_fOverCredit; /*��͸֧��:�������ֶζ�������ʾ���û�������Ҫ�����ж�*/
	DWORD    m_dwIppvNo;//IPPV��Ŀ��
	FLOAT    m_fIppv_P_Price;//����Ŀ�㲥IPPV�ļ۸�
	FLOAT    m_fIppv_T_Price;//��ʱ��㲥��IPPV�۸�
}T_PpvInfo;

#define CA_MAX_PINCODE_LEN     (4)
typedef struct _PinCodeInfo
{
	BYTE     m_cPinLen;//pin�볤��
	BYTE     m_acPinCode[CA_MAX_PINCODE_LEN];//pin�룬16��������
}T_PinCodeInfo;

#define	 CA_MAX_FINGERPRINT_LEN		  (10)
typedef struct _CaSpecInfo
{
	BOOL     m_bPriporityHigh;//not used 
	WORD     m_wDuration; //��ʾʱ�� ��λ:second
	BYTE     m_acMsg[CA_MAX_FINGERPRINT_LEN];//����,16�������ݣ�Ӧ��ֱ����ʾ16�������ݼ���
}T_CaSpecInfo;

/*Mail*/
#define CA_MAIL_FROM_LEN     (200)
#define CA_MAIL_TITLE_LEN    (200)
#define CA_MAIL_TXT_LEN      (1024)

typedef struct _CaDateTime
{
	DWORD    m_dwYear;
	BYTE     m_cMonty;
	BYTE     m_cDay;
	BYTE     m_cHour;
	BYTE     m_cMinute;
    //hyb@090511, add two parameters for force switch channel    
    BYTE     m_cSecond;
    BYTE     m_cFiveMillisecs;//five milliseconds
}T_CaDateTime;

typedef struct _Msg
{
	BOOL     m_bPriporityHigh;  //FALSE: Mail  TRUE: Subtitle
	DWORD    m_dwFromLen;//�ʼ������߳���
	BYTE     m_szFrom[CA_MAIL_FROM_LEN];//�ʼ������ߣ��ַ�������
	DWORD    m_dwTitleLen;//�ʼ����ⳤ��
	BYTE     m_szTitle[CA_MAIL_TITLE_LEN];//�ʼ����⣬�ַ�������
	DWORD    m_dwContentLen;//�ʼ����ݳ���
	BYTE     m_szContent[CA_MAIL_TXT_LEN];//�ʼ����ݣ��ַ�������
	T_CaDateTime    m_stCreatTime;  //�ʼ�����ʱ��
	T_CaDateTime    m_stSendTime;   //�ʼ�����ʱ��
}T_Msg;

typedef struct _CaMail
{	
	BOOL            m_bReadFlag;    //�Ѷ���־
	T_CaDateTime    m_stArriveTime; //�ʼ������ʱ��
	T_Msg           m_stMsg;//�ʼ�
}T_CaMail;

#define CA_MAX_ECM_LEN         (64)
#define CA_MAX_EMM_LEN         (64)
#define CA_MAX_SMC_VER_LEN     (9)
#define CA_MAX_SMC_NUMBER_LEN  (22)
#define CA_MAX_SMC_NAME_LEN    (17)
#define CA_MAX_SMC_ISSURE_TIME (17)
#define CA_MAX_SMC_EXPIRE_TIME (17)
#define CA_MAX_SMC_ANTIMOVE_LEN (3)

typedef struct _SmcInfo
{
	BYTE      m_cSmcStatus;//���ܿ�״̬:0 SC_OK,1 SC_OUT,2 SC_HARD_ERR,3 SC_SOFT_ERR
	WORD      m_wSystemId;//��ͨCAϵͳID
	WORD      m_wEcmSubSystemId;//��ͨCA��ϵͳID(for ecm)
	WORD      m_wEmmSubSystemId;//��ͨCA��ϵͳID(for emm)
	BYTE      m_acSmcVer[CA_MAX_SMC_VER_LEN];//���ܿ��汾�ţ��ַ�������
	BYTE      m_acSmcNumber[CA_MAX_SMC_NUMBER_LEN];//���ܿ��ţ�ʮ�����ַ���
	BYTE      m_acSmcName[CA_MAX_SMC_NAME_LEN];//���ܿ������������ַ�������
	BYTE      m_acSmcIssureTime[CA_MAX_SMC_ISSURE_TIME];//���ܿ�����ʱ�䣬�ַ�������
	BYTE      m_acSmcExpireTime[CA_MAX_SMC_EXPIRE_TIME];//���ܿ���Ч�ڽ�ֹʱ�䣬�ַ�������
	BYTE      m_cParentalRating;//���ܿ��ڸ�ĸ������
	BYTE      m_cPairStatus;//0:δƥ��,1:��ƥ��,2:ƥ��
	BYTE      m_cGroupControl;//��Ŀ��������������
	BYTE      m_acAntiMove[CA_MAX_SMC_ANTIMOVE_LEN];//��ֹ���ܿ�������ο��ƴ��룬16��������
}T_SmcInfo;

#define CA_MAX_PPID_NUMBER_LEN       (10)
typedef struct _PPIDInfo
{
	DWORD    m_dwPPIDId;//��Ŀ�ṩ��ID
	BYTE     m_acPPIDName[CA_MAX_PPID_NAME_LEN];//��Ŀ�ṩ�����ƣ��ַ�������
}T_PPIDInfo;

typedef struct _PPIDList
{
	BYTE         m_cCount;//��Ŀ�ṩ�̸���
	T_PPIDInfo   m_stPPIDInfo[CA_MAX_PPID_NUMBER_LEN];//��Ŀ�ṩ����Ϣ
}T_PPIDList;

#define CA_PPID_LAST_CHANGE_CREDIT_DATE_LEN  (11)
#define CA_PPID_START_AUTHOR_DATE_LEN        (11)
#define CA_PPID_END_AUTHOR_DATE_LEN          (11)
#define CA_PPID_PROGRAME_NUMBER_LEN          (8)
#define CA_PPID_IPPV_NUMBER_LEN              (32)

typedef struct _PpvProgram
{
	DWORD    m_dwStartPpvNo;//PPV��ʼ��Ŀ�ţ����Ϊ0xffffffff����Ϊ20x00000000ʱ��Ч
	DWORD    m_dwEndPpvNo;//PPV��ֹ��Ŀ�ţ����Ϊ0xffffffff����Ϊ20x00000000ʱ��Ч
}T_PpvProgram;

typedef struct _DetailPPIDInfo
{
	DWORD    m_dwPPIDId;//��Ŀ�ṩ��ID
	FLOAT    m_fCredit;//ĳһ��Ŀ�ṩ���ڿ��ڵĽ��
	BYTE     m_acCreditDate[CA_PPID_LAST_CHANGE_CREDIT_DATE_LEN];//ĳһ��Ŀ�ṩ�����һ�α䶯���ڽ������ڣ��ַ�������
	BYTE     m_cClassNumber;//�Ѿ���Ȩ�Ľ�Ŀ������
	DWORD 	 m_adwRightInfo[CA_PPID_PROGRAME_NUMBER_LEN];//��Ȩ��Ϣ
	BYTE     m_acProgStartDate[CA_PPID_START_AUTHOR_DATE_LEN];//��Ȩ��ʼ���ڣ��ַ�������
	BYTE     m_acProgEndDate[CA_PPID_END_AUTHOR_DATE_LEN];//��Ȩ��ֹ���ڣ��ַ�������
	BYTE     m_cPpvCount;//Ԥ����ppv��Ŀ�Ľ�Ŀ���������Ϊ256(�����ܿ�����)
	T_PpvProgram m_stPpvProgram[CA_PPID_IPPV_NUMBER_LEN];//ppv��Ŀ��Ϣ
	BYTE     m_cPpvtCount;//����Ŀ�㲥��ippv��Ŀ�ĸ���
	DWORD    m_adwPpvtNo[CA_PPID_IPPV_NUMBER_LEN];//����Ŀ�㲥��ippv��Ŀ�Ľ�Ŀ�ţ����Ϊ0xffffffff����Ϊ20x00000000ʱ��Ч
}T_DetailPPIDInfo;

#define CA_PAIRING_SERCIAL_NUMBER_LEN     (8)
typedef struct _SercialInfo
{
	INT      m_nLen;//���������кų���
	BYTE     m_acSercialNum[CA_PAIRING_SERCIAL_NUMBER_LEN];//���������кţ�16��������
}T_SercialInfo;

typedef struct _OtaInfo
{
	BYTE   m_cSoftVer;//����汾
	DWORD   m_dwDownLoadFrequency;//�����������ݵ�Ƶ��
	WORD   m_wDownLoadSymbol;//����������������Ƶ��ķ�����
	BYTE   m_cDownLoadModulation;//����������������Ƶ��ĵ��Ʒ�ʽ
	WORD   m_wDownLoadPid;//�����������ݵ�PID
	BYTE   m_cDownLoadTableid;//�����������ݵ�TableID
	BYTE   m_cDownLoadDuration;//
	T_CaDateTime m_stIssueDate;//
}T_OtaInfo;
//hyb@090511
typedef struct
{    
    WORD    m_wNetworkID; 
    WORD    m_wTsID;           
    WORD    m_wServiceID;           
    WORD    m_wEsPid; 
}CaSwitchByChnlPara_S;
typedef struct
{    
    WORD    m_wFreq; //frequency :MHZ
    E_CaModulation    m_emModulation;           
    WORD    m_wSymRate;           //symbol rate :Ksps
    WORD    m_wServiceID; 
}CaSwitchByTunerPara_S;

typedef union 
{
    CaSwitchByChnlPara_S              m_stSwitchByChnlPara;//����Ŀ��ת�Ĳ���
    CaSwitchByTunerPara_S           m_stSwitchByTunerPara;//��Ƶ����ת�Ĳ���
}CaChnlSwitchPara_U;

typedef struct
{
    BOOL                   m_bIsCmdByTuner;/*false: switch the channel by CaSwitchByChnlPara_S; true:CaSwitchByTunerPara_S*/
    E_CaChnlSwitchType     m_emSwitchType;//��ת����
    T_CaDateTime           m_stStartDate;//Ƶ����ת��ʼʱ��
    T_CaDateTime           m_stExpireDate; // Ƶ����ת��ֹʱ��  
    BYTE                   m_acText[CA_MAX_TEXT_LEN]; //  �ı��ַ��������ַ�������
    CaChnlSwitchPara_U     m_uPara;//Ƶ����ת����
}CaChnlSwitch_S;

typedef struct
{
    E_CaUserViewOp        m_emOperation  ;//�û�����ģʽ
    E_CaUserViewStatus    m_emStatus;//׼����״̬
    WORD                   m_wStartChnl;    //��ʼ����ͨ����
    WORD                   m_wEndChnl;//��������ͨ����
    T_CaDateTime           m_stStartDate;//׼������ʼʱ��
    T_CaDateTime           m_stExpireDate; // ׼������ֹʱ��

}CaUserView_S;


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
*	0 : SUCCESS 
*	-1: FAILURE
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
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:��������Ŀ������Ŀ����Ҫ���ô˽ӿ�
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
*	pstTxt[out]:CA���������ʼ����ݴ洢���ϲ����õĿռ�
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(INT nIndex, T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* �������ʼ��첽���浽flash�У���CS_CA_TXT_Add�����
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:��������Ŀ������Ŀ����Ҫ���ô˽ӿ�
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Save_Asyn
* �������ʼ��첽���浽flash�У���CS_CA_TXT_Add�����
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:��������Ŀ������Ŀ����Ҫ���ô˽ӿ�
**************************************************************************/
int CS_CA_TXT_Save_Asyn(void);

/**************************************************************************
*Function Name:CS_CA_TXT_SetReadFlag
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
INT CS_CA_TXT_SetReadFlag(INT nIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* ���ص�ǰ���ж��ٷ�δ���ʼ�
*
*Parameters 
*	��
*
*Return Values: 
*   nCount:δ���ʼ�����
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum( );

/**************************************************************************
*Function Name:CS_CA_MMI_CheckPinCode
* ����У������
*
*Parameters 
*	pstPinCode[in]:��������
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
*		CA_STATUS_PIN_OK   У��pin��ɹ�
*		CA_STATUS_PIN_ERROR pin�����
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
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
*		CA_STATUS_MODIFY_PIN_SUCCESS   �޸�pin��ɹ�
*		CA_STATUS_MODIFY_PIN_FAIL      �޸�pin��ʧ��
**************************************************************************/
INT CS_CA_MMI_ModifyPinCode(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvConfirm
* �㲥PPV��Ŀ
*
*Parameters 
*	dwIppvNo[in]:��Ŀ��
*   pstPinCode[in]:����PinCode
*   bIsPpvp[in]:TRUE:����PPVP��FALSE����PPVT
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
* �����PPVP
*		CA_ALARM_ORDER_IPPVP_SUCCESS   IPPVP�㲥�ɹ�
*		CA_ALARM_ORDER_IPPVP_FAIL      IPPVP�㲥ʧ��
*		CA_ALARM_IPPV_PIN_ERROR        pin�����
*		CA_ALARM_IPPV_EXPIRED          IPPVP��Ŀ����
*�����PPVT
*		CA_ALARM_START_IPPVT_SUCCESS   IPPVT�㲥�ɹ�
*		CA_ALARM_ORDER_IPPVP_FAIL      IPPVT�㲥ʧ��
*		CA_ALARM_IPPV_PIN_ERROR        pin�����
*		CA_ALARM_IPPV_EXPIRED          IPPVP��Ŀ����
**************************************************************************/
INT CS_CA_MMI_PpvConfirm(DWORD dwIppvNo, DWORD dwPin, BOOL bIsPpvp);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvCancel
* ����PPVT��Ŀ�㲥
*
*Parameters 
*	dwIppvNo[in]:��Ŀ��
*   pstPinCode[in]:����PinCode
*  
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:���ӿ�ֻ�ǽ���IPPVT��Ŀ�ĵ㲥��IPPVP�㲥���ǲ���ȡ���ġ�
* �첽��ʽʵ��:
*		CA_ALARM_IPPV_PIN_ERROR        pin�����
*		CA_ALARM_STOP_IPPVT_SUCCESS    ����IPPVT�ɹ�
*		CA_ALARM_STOP_IPPVT_FAIL       ����IPPVTʧ��
**************************************************************************/
INT CS_CA_MMI_PpvCancel(DWORD dwIppvNo, DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestSmcInfo
* �������ܿ���Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
*		CA_INFO_SMC_STATUS   dwParm2 = (DWORD)&T_SmcInfo
*       T_SmcInfo.m_cSmcStatusΪ0��ʱ��ɻ�ȡ���ܿ���Ϣ��Ϊ����ֵ��ʱ�������ܿ���Ϣ��ȡ����
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
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�CA_INFO_PPID   dwParm2 = (DWORD)&T_PPIDList
*       �����ʱ�򷵻�CA_STATUS_GET_PPID_INFO_FAIL
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
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�CA_INFO_PPID_DETAIL   dwParm2 = (DWORD)&T_DetailPPIDInfo
*       �����ʱ�򷵻�
* 					CA_STATUS_READ_INFO_ERROR
*                   CA_STATUS_PIN_ERROR
*                   CA_STATUS_PPID_ERROR
*                   CA_STATUS_GET_DETAIL_PPID_INFO_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestDetailPPIDInfo(DWORD dwPin, DWORD dwPpId);

/**************************************************************************
*Function Name:CA_CA_MMI_RequestRecordSrvAllowedFlag
* �����¼�Ʊ�־
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�
*					CA_STATUS_ALLOW_RECORD   
*					CA_STATUS_REJECT_RECORD   
*       �����ʱ�򷵻�
* 					CA_STATUS_RECORD_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestRecordSrvAllowedFlag();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyParentRating
* �����޸ĸ�ĸ��
*
*Parameters 
*	pstPin[in]:��������
*   cParentRatingValue[in]:Ҫ�޸ĵļ���
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�
*					CA_STATUS_MODIFY_PR_LEVEL_SUCCESS   
*       �����ʱ�򷵻�
*					CA_STATUS_MODIFY_PR_LEVEL_FAIL   
* 					CA_STATUS_PIN_ERROR
**************************************************************************/
INT CS_CA_MMI_ModifyParentRating(DWORD dwPin, BYTE cParentRatingValue);


/**************************************************************************
*Function Name:CS_CA_MMI_RequestCurrentParentLevel
* �����޸ĸ�ĸ������
*
*Parameters 
	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE�����÷���ֵ��Ŀǰû���õ�
* ��ע:�첽��ʽʵ��:
*		CA_INFO_PARENTLEVEL   dwParm2 = (DWORD)dwParentLevel
**************************************************************************/

INT CS_CA_MMI_RequestCurrentParentLevel();

/**************************************************************************
*Function Name:CS_CA_GetCurrentProgramNo
* ��ȡ��ǰ��Ŀ�ţ����ڱȽ��Ƿ��ѻ�̨
*
*Parameters 
*	��
*
*Return Values: 
*	��Ŀ�� 
**************************************************************************/
INT CS_CA_GetCurrentProgramNo();

/**************************************************************************
*Function Name:CS_CA_MMI_CheckIsNewPPV
*�жϵ�ǰ��PPV �Ƿ�Ϊ��
*Parameters 
*	pstOldPPV  ���Ƚϵ���PPV ��Ϣ
*	pstNewPPV ���Ƚϵ���PPV ��Ϣ

*Return Values:
	TRUE : PPV ��Ŀ�и���
	FALSE :PPV ��Ŀδ����
*
**************************************************************************/
BOOL CS_CA_MMI_CheckIsNewPPV(T_PpvInfo * pstOldPPV, T_PpvInfo * pstNewPPV);


/**************************************************************************
*Function Name:CS_CA_TXT_SetRepeatCheck
*�Ƿ�����ظ��ʼ��ж�
*Parameters 
*	bIsCheck 
*	TRUE ,�����ظ��ʼ��ж�.
*	FALSE ,�����ظ��ʼ��ж�.

*Return Values:
	SUCCESS :�ɹ�
	FAILURE :ʧ��
*ע��CA ��Ĭ�ϲ����ǽ����ظ��жϣ��������ظ������ʼ�
*����Ҫ�����ظ��ʼ�������Ե��ô˺��������޸ġ�
**************************************************************************/
INT CS_CA_TXT_SetRepeatCheck(BOOL bIsCheck);

/**************************************************************************
*Function Name:CS_CA_GetFlashAddress
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ�����
*Parameters 
*	dwFlashSize: ָ��Ҫ����FLASH��Ĵ�С
*Return Values:
*	���ط������ʼ��ַ
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(DWORD dwFlashSize);

/**************************************************************************
*Function Name:get_software_version
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ����ã���ȡ��ǰ����汾
*Parameters 
*	chSWVision: ���ڴ洢��ǰ����汾��
*   nsizeSW   : ����汾����
*Return Values:
*	SUCCESS: �ɹ�
*   ����   : ʧ��
**************************************************************************/
DWORD get_software_version (char* chSWVision, int nsizeSW);

/**************************************************************************
*Function Name:get_hardware_version
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ����ã���ȡ��ǰӲ���汾
*Parameters 
*	chHWVision: ���ڴ洢��ǰӲ���汾��
*   nsizeHW   : Ӳ���汾����
*Return Values:
*	SUCCESS: �ɹ�
*   ����   : ʧ��
**************************************************************************/
DWORD get_hardware_version (char* chHWVision, int nsizeHW);
/**************************************************************************
*Function Name:CS_CA_MMI_UserViewOp
*�����û�׼����������Ŀ
*Parameters 
*   stUserView: CaUserView_S�ṹ��
*   dwPin   : ����
*Return Values:
*   SUCCESS: �ɹ�
*   FAILURE: ��������
* ��ע:�첽��ʽʵ��:
*		CA_INFO_USER_VIEW   dwParm2 = (DWORD)&CaUserView_S
**************************************************************************/
INT CS_CA_MMI_UserViewOp(CaUserView_S stUserView,DWORD dwPin);
/**************************************************************************
*Function Name:CSCAFactoryDefault
*�ָ���������ʱ��CA���������
*Parameters 
*  ��
*Return Values:
*  ��
**************************************************************************/

void CSCAFactoryDefault(void);
/**************************************************************************
*Function Name:CS_CA_GetMailFlashAddress
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ�����
*Parameters 
*   ��
*Return Values:
*   ���ط������ʼ��ַ
**************************************************************************/
extern DWORD CS_CA_GetMailFlashAddress(void);
/**************************************************************************
*Function Name:CSCAGetTSAntiMoveCtrlCode
*��ȡ�������������
*Parameters 
*   pcAntiMoveCtrl: �������������
*Return Values:
*   SUCCESS: �ɹ�
*   ����   : ʧ��
**************************************************************************/

INT CSCAGetTSAntiMoveCtrlCode(BYTE* pcAntiMoveCtrl);

/**************************************************************************
*Function Name:CSCaGetCurrentTime
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ�����
*Parameters 
*   [out]pstDateTime:���浱ǰ��EPGʱ��
*Return Values:
*   SUCCESS:0�����EPGʱ��ɹ�
*   FAILURE:-1�����EPGʱ��ʧ��
**************************************************************************/
extern INT CSCaGetCurrentTime(T_CaDateTime *pstDateTime);


#ifdef  __cplusplus
}
#endif

#endif //_CS_SUANTONG_INTERFACE_H_

