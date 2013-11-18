/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_VIACCESSCA_Interface.h  
* Author:      tuhuajiang
* Date:        March 2005 
* Description: VIACCESS CA interface
*              
*              Delivery Reference: 492-PPV-1.1-ShenzeCable-0213
*              Library Version   : 1.1 R492
*              Delivery date     : 2005-04-05
*            
*****************************************************************************************************************************/
#include "udiplus_typedef.h"
#include "cs_cainterface.h"
#include "cs_cainterface_pvrext.h"

/****************************************************************************************************************************
* 			                Revision History                                *
*****************************************************************************************************************************
* - Revision 1.0  2005/3/7
*   Programmer:Tu huajiang
*   Create.
*****************************************************************************************************************************/
#ifndef _CS_VIACCESSCA_INTERFACE_H_
#define _CS_VIACCESSCA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif
//{HYB080606
#define PURPLE_BOX                     (0)
//}

#define MAX_SRV_LABEL_SIZE                  (32)
#define MAX_PPV_LABEL_SIZE                  (32)
#define MAX_SMC_ADDRESS_SIZE                (16)
#define MAX_ISSUER_LABEL_SIZE               (32)
#define MAX_CURRENCY_UNIT_SIZE              (32)
#define MAX_PAIRING_SMARTCARD_NUMBER     (8)
#define SMARTCARD_UA_SIZE                   (5)
typedef enum _AppAction
{
    CA_APP_ACTION_CANCEL,
    CA_APP_ACTION_CONFIRM,    
    CA_APP_ACTION_MAX
}E_AppAction;

typedef enum _CaEventGroup
{
	CA_ALARM,
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	
	CA_EVENTGROUP_MAX
}E_CaEventGroup;

//ca message content
/*---------------------------------------------------------------------------------------------------------------------------
*       first  doubleword = CA_COMMAND | CA_ALARM | CA_INFO //defined in event.h
*       second doubleword = eX                              //define in T_CaCommand
*       third  doubleword = certaion information address
*       fourth doubleword   reserved                        //gui can only deal with double parameters
----------------------------------------------------------------------------------------------------------------------------*/
typedef enum _CaCommand
{
	//notify the app to erasering the forced showing message
	CA_COMMAND_FORCE_MSG_HIDE,  //0:
	//notify the app to force showing a message  dwParm2=(DWORD)&T_Msg
	CA_COMMAND_FORCE_MSG_SHOW,  //1:

	//notify the app to input the PIN code.dwParm2=(DWORD)&T_KeyRequest or 0
	CA_COMMAND_KEY_REQUEST,     //2:

	//notify the app PPV request.          dwParm2=(DWORD)&T_PpvRequest
	CA_COMMAND_PPV_REQUEST,     //3:

	//notify cancelling the PPV request    dwParm2=chPpv
	CA_COMMAND_PPV_REQUEST_CANCEL, //4:

	//notify the app preview information   dwParm2=(DWORD)&T_CaPreview
	//CA_COMMAND_PREVIEW,	

	//notify the app to replay the current service
	//CA_COMMAND_REPLAY,
	//proctect the digital outputs
	CA_COMMAND_DIGITAL_OUTPUT_PROT,
	CA_COMMAND_START_MACROVISION,
	CA_COMMAND_STOP_MACROVISION,

	CA_COMMAND_MAX
}T_CaCommand;

typedef enum _CaAlarm
{
	//CA INIT                          -> Send by CAK-APP
	//"Smart card communication error !"
	CA_ALARM_SMC_ACCESS_ERROR = 0,           //0: Smart Card Access Error during cak_Init
	CA_ALARM_SMC_ACCESS_OK,              //1: Smart Card Access OK during cak_Init

	//Service Status:
	//CA TYPE                          -> Send by CAK-APP
	CA_ALARM_VA_CA,                      //2: Program encrypt with VA CA
	//"The service is encrypted with other CA !"
	CA_ALARM_OTHER,			             //3: Program encrypt with other CA
	CA_ALARM_FREE,                       //4: Program free

	//Smc Status:
	CA_ALARM_SMC_IN = 5,                    //5: 31 Dectect smartcard
	//"Please insert the smart card !"
	CA_ALARM_SMC_OUT,                   //6: 32 Smartcard removed   

	//"Checking the card !"
	CA_ALARM_SMC_CHECKING,              //7:
	CA_ALARM_SMC_OK,                    //8: 33 The smart card is communicating well with the CA 
	//"Card error !"
	CA_ALARM_SMC_COM_ERROR,             //9: 34 Communication error   //CA_ALARM_SMC_ACCESS_ERROR
	//"Invalid smartcard !"   
	CA_ALARM_SMC_INVALID = 10,	            //10: 35 Smart card invalid  
	//"The smartcard has been invalidated bythe operator. Please contact the operator for further information."
        CA_ALARM_SMC_ILLEGAL,               //11:
	//"Smartcard memory is full."
	CA_ALARM_SMC_MEMORY_FULL,           //12:
	//"Smartcard has no pin code. Please contact your operator. "
	CA_ALARM_SMC_NO_PIN,                //13:
        //"Wrong Card inserted.Please contact the operator for further information."
        CA_ALARM_SMC_MISMATCH,  //14

	//Es Status:
	CA_ALARM_ES_STATUS = 15,                 //15: dwParm2 = T_CaFtaEsState
	CA_ALARM_PPV_DESCRAMBLED_OK,        //16:
	//"Purchase is not possible."
	CA_ALARM_PPV_NORIGHT,               //17:
	//"PPV is impossible because the smartcard is in an unauthorized geographical area."
	CA_ALARM_PPV_GEOGRAPHICAL_BLACKOUT, //18:

	//"The threshold has been reached !"
	CA_ALARM_PPV_THRESHOLD_REACHED,     //19:
	//"Overdraft  detected !"
	CA_ALARM_PPV_OVERDRAFT_REACHED = 20,     //20:
	//"The key is not present in the smartcard ! "
	CA_ALARM_PPV_WRONG_REFERENCE,       //21:
	//"There is not enough money to purchase this program ! "
	CA_ALARM_PPV_NOT_ENOUGH_TOKENS,     //22:
	//"PPV Programe no more available"
	CA_ALARM_PPV_NOT_POSSIBLE_ANY_MORE_MESSAGE, //23

	CA_ALARM_QEV_ACTIVATED_OK,          //24:
	//"Query activated not OK !"
	//"The inserted smartcard is not compliant with this decoder, Please contact your operator !"
	CA_ALARM_QEV_ACTIVATED_NOT_OK = 25,      //25:
	//"Query activated no card !"
	CA_ALARM_QEV_ACTIVATED_NO_CARD,     //26:
	//"Query not activated !"
	CA_ALARM_QEV_NOT_ACTIVATED,         //27:
	//"Query wrong STB parameter !"
	CA_ALARM_QEV_WRONG_STB_PARAMETER,   //28:

    //for recorder
        CA_ALARM_NO_RECORD_RIGHT,
        CA_ALARM_NO_TIMESHIFT_RIGHT = 30,
        
    //for terminal
        CA_ALARM_TERMINAL_NOT_ACTIVATED,
        CA_ALARM_TERMINAL_CONFIGURATION_ERROR,
        CA_ALARM_TERMINAL_INSUFFICIENT_MEMORY,
        CA_ALARM_TERMINAL_INVALIDATED,
        CA_ALARM_TERMINAL_PERSONALIZATION_FAILED = 35,

    //about the maturity screen
    //"A security restriction has been activated on this STB!Please contact with your operator."
    CA_ALARM_START_SECURITY_RESTRICTION,//be displayed a full screen pop-up
    CA_ALARM_STOP_SECURITY_RESTRICTION,

    CA_ALARM_CAMLOCK_OK,
    //应用关闭音视频，显示"The inserted smartcard is not compliant with this STB!Please contact your operator."
    CA_ALARM_CAMLOCK_FAILED,
						
	//status no need to show some OSD
	CA_ALARM_STATUS_OK = 40,                       
    CA_ALARM_STATUS_ARRAY_TOO_SMALL,    

    CA_ALARM_STATUS_BAD_PIN_CODE,       
    CA_ALARM_STATUS_LOCKED_PIN_CODE,    
    CA_ALARM_STATUS_MAX_CHANGES_PIN_CODE, 
    CA_ALARM_STATUS_INVALID_PIN_CODE = 45,   
    CA_ALARM_STATUS_NO_PIN_CODE,         
    CA_ALARM_STATUS_INVALID_MORALITY_LEVEL_VALUE,
    CA_ALARM_STATUS_NOT_ENOUGH_MEMORY,    //36:
    CA_ALARM_STATUS_NO_CARD,
    CA_ALARM_STATUS_CARD_ERROR = 50,
    CA_ALARM_STATUS_BAD_SOID,             //37:
    CA_ALARM_STATUS_OPERATION_NOT_ACCEPTED, //38:
    CA_ALARM_STATUS_PRESELECTION_AREA_FULL, //39:
    CA_ALARM_STATUS_PROG_ALREADY_STORED_IN_PRESELECTION_AREA, //40:
    CA_ALARM_STATUS_PROG_NOT_FOUND_IN_PRESELECTION_AREA = 55,      //41:
    CA_ALARM_STATUS_PRESELECTION_MODE_NOT_ACCEPTED,           //42:
    CA_ALARM_STATUS_CONSULTATION_LOCKED,            //43:

    CA_ALARM_STATUS_BAD_HANDLE,                 //
    CA_ALARM_STATUS_NO_IPPV_OFFER_AVAILABLE,
    CA_ALARM_STATUS_NO_ITEM_FOUND = 60,
    CA_ALARM_STATUS_INSUFFICIENT_MEMORY,
    CA_ALARM_STATUS_INFO_NOT_AVAILABLE,
    CA_ALARM_STATUS_WRONG_ACS_ID,
    CA_ALARM_STATUS_RESOURCE_BUSY,
    CA_ALARM_STATUS_OPERATING_ERROR = 65,
    CA_ALARM_STATUS_MODE_NOT_SETUP,
    CA_ALARM_STATUS_WRONG_MODE,
    CA_ALARM_STATUS_NO_STB_ID,
    CA_ALARM_STATUS_PVR_NO_RIGHTS,
    CA_ALARM_STATUS_CONNECTION_ERROR = 70,
    CA_ALARM_STATUS_WRONG_ACTIVATION_CODE,
    CA_ALARM_STATUS_ACTIVATION_ERROR,
    CA_ALARM_STATUS_TERMINAL_NOT_ACTIVATED,
    CA_ALARM_STATUS_TERMINAL_LOCKED,
    
    CA_ALARM_STATUS_ERROR = 75,
        /* Used to know the end */
    CA_ALARM_STATUS_LAST,
    CA_ALARM_SECURITY_STATUS,// dwParm2 = CSCaSecurityStatus_S

	CA_ALARM_MAX
}T_CaAlarm;
typedef enum _CSCaSecurityStatus_E
{
    CA_ALARM_SECURITY_STATUS_OK,
    CA_ALARM_SECURITY_STATUS_WRONG_MODE_STATUS,
    CA_ALARM_SECURITY_STATUS_MODE_NOT_SETUP_STATUS,
    CA_ALARM_SECURITY_STATUS_SAME_MODE_STATUS,
    CA_ALARM_SECURITY_STATUS_WRONG_CONFIG_STATUS,
    CA_ALARM_SECURITY_STATUS_TOO_MANY_SMARTCARD_STATUS,
    CA_ALARM_SECURITY_STATUS_ARRAY_TOO_SMALL_STATUS,
    CA_ALARM_SECURITY_STATUS_LOCKED_STATUS,
    CA_ALARM_SECURITY_STATUS_ERROR_STATUS,
    CA_ALARM_SECURITY_STATUS_SECURITY_ERROR,
    CA_ALARM_SECURITY_STATUS_MAX
}CSCaSecurityStatus_E;

typedef enum _CaEsAlarm
{
	CA_ALARM_STREAM_DESCRAMBLED = 0,   //0: 23:
	
	//"You don't have the right to access the program !"
	CA_ALARM_STREAM_NO_RIGHT,         //1: 24:

    //"Geographical blackout !"
	CA_ALARM_STREAM_GEOGRAPHICAL_BLACKOUT,//2: 25:
	
       //"Access not granted because the key is not present.If this <video/program> is in your subscription please contact your operator."
       CA_ALARM_STREAM_MISSING_KEY,
        //purchase is not possible for the <>.
        CA_ALARM_STREAM_IPPV_OFFER=4 ,
        //"Access to this <>has been forbidden by the operator"
        CA_ALARM_STREAM_FORBIDDEN_MODE,
        //"You don't have sufficient credit to purchase this<>"
        CA_ALARM_STREAM_INSUFFICIENT_CREDIT,
        //"You can not view this<>because the visualization date has expired"
        CA_ALARM_STREAM_RIGHT_EXPIRED,
        //"You can not view this<>because the maximum number of playbacks has been reached"
        CA_ALARM_STREAM_RIGHT_CONSUMED,
        //"Terminal configuration is not accurate. Please contact your operator for further information"
        CA_ALARM_STREAM_CW_PROTECTION_MISMATCH =9,
        CA_ALARM_STREAM_PREVIEW_ONLY_AVAILABLE_IN_PREBOOK,
	//"No right !"
	//CA_ALARM_STREAM_QUERY_NO_RIGHT,//29:
	//CA_ALARM_STREAM_QUERY_RIGHT,       //30:
	
	CA_ALARM_STREAM_MAX = 15
}T_CaEsAlarm;

typedef enum _CaInfo
{
	//receive text Content message     dwParm2 = (DWORD)&T_Msg;
	CA_INFO_TXT = 0,              //0:

	//<-INT CS_CA_MMI_RequestSubSoidList(void);   dwParm2 = (DWORD)&T_SoidList
	CA_INFO_SOID_LIST,        //1:
	//<-INT CS_CA_MMI_RequestSubThemeSubscriptionList(DWORD dwSoid); dwParm2 = (DWORD)&T_SubListTheme
	CA_INFO_SUBLIST_THEME,    //2:
	//<-INT CS_CA_MMI_RequestSubClassSubscriptionList(DWORD dwSoid);   dwParm2 = (DWORD)&T_SubListClass
	CA_INFO_SUBLIST_CLASS,    //3:
	//<-INT CS_CA_MMI_RequestSubPreBookedList(DWORD dwSoid);             dwParm2 = (DWORD)&T_PrebookedList
	CA_INFO_PREBOOKLIST,      //4:
	//<-INT CS_CA_MMI_RequestSubLabel(DWORD dwSoid);                          dwParm2 = (DWORD)&T_SoidLabel
	CA_INFO_SOID_LABEL = 5,       //5:
	//<-INT CS_CA_MMI_RequestSmcID(void);                                              dwParm2 = (DWORD)&T_SmcID;
	CA_INFO_SMC_ID,           //6:
	//<-INT CS_CA_MMI_RequestLockStatus(void);                                        dwParm2 = (DWORD)&T_LockStatus;
	CA_INFO_LOCK_STATUS,      //7:
	//<-INT CS_CA_MMI_RequestMoralityLevel(void);                                     dwParm2 = (DWORD)&T_PgLevel;
	CA_INFO_PG_LEVEL,         //8:
	//<-INT CS_CA_MMI_RequestPpvIppvpList(DWORD dwSoid);                     dwParm2 = (DWORD)&T_IppvpListValue;  
	CA_INFO_PPVP_LIST,        //9:
	//<-INT CS_CA_MMI_RequestPpvIppvtList(DWORD dwSoid);                      dwParm2 = (DWORD)&T_IppvtListValue;
	CA_INFO_PPVT_LIST = 10,        //10:
	//<-INT CS_CA_MMI_RequestPpvInfoCredit(DWORD dwSoid);                    dwParm2 = (DWORD)&T_Credit;
	CA_INFO_PPV_CREDIT,       //11:
	//<-INT CS_CA_MMI_RequestPpvCeiling(void);                                          dwParm2 = (DWORD)&T_CeilingValue
	CA_INFO_PPV_CEILING,      //12:
	//<-INT CS_CA_MMI_RequestPpvThreshold(void);                                      dwParm2 = (DWORD)&T_Threshold
	CA_INFO_THRESHOLD,        //13:
	//<-INT CS_CA_MMI_RequestCaVersion(void);                                           dwParm2 = (DWORD)&T_Version
	CA_INFO_CA_VERSION,	      //14:
	//<-INT CS_CA_MMI_RequestCaVersionDetail(void);                                   dwParm2 = (DWORD)&T_VersionDetail
	CA_INFO_CA_VERSION_DETAIL = 15,//15:
	//to get the STB Id
	CA_INFO_STB_ID,
	//to get the terminal Id
	CA_INFO_TERMINAL_ID,
	//to get the camlock value
	CA_INFO_CAMLOCK_VALUE,
	//to get the paring mode
	CA_INFO_CARD_PARING_MODE,
	//to get the number of paring cards
	CA_INFO_PARING_CARD_NUM = 20,
	//to get the UA of the paring card
	CA_INFO_PARING_CARD_LIST,
	
    CA_INFO_PRESELECTIONLIST,//16
    CA_INFO_NULL,
    //To get the left attempts for inputting pincode
    CA_INFO_ATTEMPTSFORPINCODE,
    //to get the progress of the preview program
    CA_INFO_PREVIEWPROGRESS = 25,
    CA_INFO_OP_DATA,
    //the attemps for pin code is unlimited
    CA_INFO_UNLIMITED_ATTEMPTSFORPINCODE,

    
    CA_INFO_SECURITY_PAIRING_SANCTION,
    CA_INFO_SECURITY_STATUS,
    CA_INFO_SECURITY_FORBIDDEN_SOID_LIST = 30,
    CA_INFO_SECURITY_DETECTION_CONFIG,
 //    CA_INFO_TRACE_LIST,
     CA_INFO_MAX
}T_CaInfo;


typedef enum _PreselectionType
{
    E_PRESELECTION_RESERVED,
    E_PRESELECTION_THEMELEVEL,
    E_PRESELECTION_CLASS,
    E_PRESELECTION_PREBOOKED,
    E_PRESELECTION_PPVP,
    E_PRESELECTION_PPVT
}T_PreselectionType;




typedef struct _CSCaSecurityStatus_S
{
   CSCaSecurityStatus_E m_emSecurityStatus;
}CSCaSecurityStatus_S;

typedef struct _KeyRequest
{
	unsigned char m_dwPgLevel;
	BOOL m_dwLockStatus;
}T_KeyRequest;


typedef struct _EsState
{
	T_CaEsAlarm eUndefined;
	T_CaEsAlarm eVideo;
	T_CaEsAlarm eAudio;
	T_CaEsAlarm eTtx;
	T_CaEsAlarm eSub;
	T_CaEsAlarm eDb;
}T_CaFtaEsState;

typedef enum _PpvMode
{
	E_PPV_NO = 0,
	E_PPV_P  = 0x01,
	E_PPV_T  = 0x02,
	E_PPV_TP = 0x3,
	E_PPV_MAX
}T_PpvMode;

typedef enum _CardParingMode
{
    
    CARD_NO_MODE,
    CARD_AUTOPAIRING_MODE,
    CARD_PAIRING_MODE 
    
}T__CardParingMode;

typedef struct _CardNumForParing
{
    DWORD m_dwMaxNum;
    DWORD m_dwSmcNum;
    
}T__CardNumForParing;

typedef struct _ParingCardList
{
    DWORD  m_dwCardNum;
    BYTE  m_dwCardList[MAX_PAIRING_SMARTCARD_NUMBER][SMARTCARD_UA_SIZE];
}T__ParingCardList;

typedef struct _Price
{
	BOOL                m_bToken;
	float               m_fPpvPrice;
	BYTE                m_szCurrency[16+2];
}T_Price;

typedef struct _PpvPrice
{
	T_Price            m_stPpvp;
	T_Price            m_stPpvt;	
}T_PpvPrice;

typedef struct _PpvSrvID
{
	DWORD      m_dwPpvpSrvID;
	DWORD      m_dwPpvtSrvID;
}T_PpvSrvID;

typedef enum _PaymentMode
{
     POST_PAYMENT_MODE,
     PRE_PAYMENT_MODE,
     UNKNOWN_PAYMENT_MODE

} T_PaymentMode;

typedef struct _PpvRequest
{
    DWORD 			    m_dwSoid;
    
    //{   hyb@080523
       DWORD               m_dwPpvHandle;
    //}
   T_PaymentMode    m_PayMode;
    T_Price             m_stCeiling;
    T_PpvMode           m_ePpvMode;

    T_PpvPrice          m_stPpvPrice;
    T_PpvSrvID          m_stPpvSrvID;
    T_Price             m_stRemainCredit;
    BOOL                m_bWithoutModem;

	BOOL                m_bPreview;
	BOOL                m_bMaturity;
} T_PpvRequest;

typedef enum _EncodeFormat
{
    ENCODING_ASCII         = 0,
    ENCODING_UTF_8         = 1,
    ENCODING_UTF_16        = 2,
    ENCODING_UTF_32        = 3,
    ENCODING_GB2312_80     = 4

} T_EncodeFormat;

typedef struct _Msg
{
	BOOL      m_bPriporityHigh;//TRUE:短消息, FLASE:普通邮件
	DWORD   m_dwSoid;
	T_EncodeFormat      m_Encoding;
	DWORD     m_dwLen;
	BYTE*     m_pszContent;
}T_Msg;

typedef struct _SoidList
{
	DWORD     m_dwSoidNum;
	DWORD*    m_padwSoidList;                 
}T_SoidList;

typedef struct _AttemptForPincode
{
    UINT8 RemainingCount;
    UINT8 MaxCount;
}T_AttemptForPincode;

typedef struct _Date
{
	DWORD     m_dwYear;
	BYTE      m_cMonth;
	BYTE      m_cDay;
}T_Date;

typedef struct _ThemeSubList		
{
	T_Date    m_stBegin;
	T_Date    m_stEnd;
	BYTE      m_cTheme;
	BYTE      m_cLevel;	
}T_ThemeSubList;
	
typedef struct _SubListTheme
{
	DWORD             m_dwNum;
	T_ThemeSubList*   m_pastSubList;
}T_SubListTheme;

typedef struct _ClassSubList
{
    T_Date    		m_stBegin;
    T_Date    		m_stEnd;
    DWORD    		m_dwClassNum;
    BYTE          	m_pacClass[256];
} T_ClassSubList;

typedef struct _SubListClass
{
	DWORD         	  m_dwNum;
	T_ClassSubList*   m_pastSubList;
}T_SubListClass;


typedef struct _PrebookedRange
{
	DWORD             m_dwBeginProgNum;
	DWORD             m_dwEndProgNum;
}T_PrebookedRange;

typedef struct _PrebookedList
{
	DWORD             m_dwNum;
	T_PrebookedRange* m_pastPreBookedList;
}T_PrebookedList;

typedef struct _PreselectionPrice
{
    INT32   iInteger; 
    UINT16  uiFraction; 
} T_PreselectionPrice;

typedef struct _Thle
    {
    T_Date                m_Date;
    BYTE                   m_Theme;
    BYTE                   m_Level;
} T_Thle;

typedef struct _Link
    {
    T_Date                m_Date;
    BYTE                   m_Link;
} T_Link;

typedef struct _IppvP
    {
    DWORD       m_dwNumb;
    T_PreselectionPrice        m_dwCost;
} T_IppvP;

typedef struct _IppvT
    {
    DWORD       m_dwNumb;
    T_PreselectionPrice        m_dwCost;
} T_IppvT;


typedef union _InfoPreselection
  {
    T_Thle                m_Thle;
    T_Link                m_Link;
    DWORD              m_dwPnumb;
    T_IppvP               m_Ppvp;
    T_IppvT               m_Ppvt;
} T_InfoPreselection;

typedef struct _Preselection
{
    DWORD                m_dwSoid;
    T_PreselectionType   m_Type;
    T_InfoPreselection    m_Info;
} T_Preselection;

typedef struct _PreSelectionList
{
	DWORD             m_dwNum;
	T_Preselection  *m_pastPreSelectionList;
}T_PreSelectionList;

typedef struct _SoidLabel
{
	DWORD         m_dwSoid;
	DWORD         m_dwLabelLen;
	BYTE*         m_pacLabel;
}T_SoidLabel;

typedef struct _SmcID
{
	DWORD         m_dwLen;
	BYTE*         m_pacID;
}T_SmcID;

typedef struct _LockStatus
{
	INT               m_bPreselectionLocked;     //preselection area
	INT               m_bConsultationLocled;     //read smc information(eg:Sub, ppv, remain credit)
	INT               m_bMaturityRatingLocked;   //pg
	INT               m_bImpulsePurchaseLocked;  //IPPV
}T_LockStatus;

typedef struct _PgLevel
{
	BYTE             m_cPgLevel;
	BYTE             m_cPgLevelMaxAllowed;
}T_PgLevel;
typedef struct _PreviewProgress
{
    unsigned char UsedCWNum;
    unsigned char MaxCWNum;
}T_PreviewProgress;

typedef struct  _SignedCredit
{
   	 INT                m_nToken;
}T_SignedCredit;

typedef struct _PpvRecordValue
{
    DWORD               m_dwProgNum;
    FLOAT               m_fCostTokens;
    BYTE                m_szCurrency[16];
}T_PpvRecordValue;

typedef struct _IppvpListValue
{
    DWORD               m_dwNum;
    T_PpvRecordValue*   m_pastPpvRecord;
}T_IppvpListValue;

typedef struct _IppvtListValue
{
    DWORD               m_dwNum;
    T_PpvRecordValue*   m_pastPpvRecord;
}T_IppvtListValue;

typedef struct _InfoCreditValue
{
    T_Date              m_stDate;
    FLOAT               m_fToken;
}T_InfoCreditValue;

typedef struct _CreaditValue
{
    FLOAT               m_fRemain;/* Remaining credit: Set with the remaining credit available in the smartcard for a given program */
	BYTE                m_szCurrency[17];/* add at 2006-05-19*/
}T_CreditValue;

typedef struct _CeilingValue
{
    FLOAT         m_fCeilingToken;
}T_CeilingValue;

typedef struct _Threshold
{
	FLOAT         m_dwThresholdToken;
}T_Threshold;

typedef struct _Version
{
	DWORD         m_dwLen;
	BYTE*         m_pszVersion;
}T_Version;
typedef struct _DateTime
{
    DWORD          m_dwYear;
    BYTE           m_cMonth;
    BYTE           m_cDay;
    BYTE           m_cHour;
    BYTE           m_cMinute;
    

}T_DateTime;

typedef struct _STBId
{
    BOOL m_bIsIdValid;
    BYTE* m_pSTBId;
    
       // {hyb@080516
    DWORD         m_dwLen;        
  //  T_DateTime   m_stDate;
       //}
}T_STBId;

typedef enum _CamlockDynamicState
{
    CAMLOCK_ACTIVATED_OK      = 0x01,
    CAMLOCK_ACTIVATED_KO,
    CAMLOCK_ACTIVATED_NO_CARD,
    CAMLOCK_NOT_ACTIVATED,
    CAMLOCK_DISABLED
}T_CamlockDynamicState;

typedef struct _CAMLOCK
{
    T_CamlockDynamicState  m_Camlockstatus;
    BYTE* m_pCamlockValue;
    //{hyb@080516
    DWORD         m_dwLen;        

//}
}T_CAMLOCK;
//{hyb@080516
typedef struct _TerminalId
{
    BYTE*   m_pTerminalId;
    DWORD         m_dwLen;        

} T_TerminalId;
//}

typedef struct _VersionDetail
{
	DWORD          m_dwDelVerNum;
	DWORD          m_dwMajorVerNum;
	DWORD          m_dwMinorVerNum;
	DWORD          m_dwEditionVerNum;
	DWORD          m_dwRevisionVerNum;	
	DWORD          m_dwOptionMask;//no use
	BYTE*          m_pszCasVariant;	
	BYTE*	       m_psztOperatorName; 	
}T_VersionDetail;


typedef struct _Txt
{
	BOOL           m_bReadFlag;
	T_DateTime     m_stTime;
	T_Msg          m_stMsg;
}T_Txt;
//{add by hyb@080513

typedef struct _DigitalOutputProt
{
    BOOL           m_bSPDIFActive;
    BOOL           m_bEthernetActive;  
    BOOL           m_bUSBActive;  
    BOOL           m_bILinkActive;    
    BOOL           m_bDvbCiActive;    
    BOOL           m_bHddActive;   
    BOOL           m_bDvdActive;
}T_DigitalOutputProt;

//}





#if 0
#define MAX_FILENAME_LENGTH 64 + 1
#define MAX_ADDRESS_SIZE 20 + 2
#define MAX_MODULE_NAME_LENGTH           64 + 1
typedef enum 
{
    OUTPUT_NONE,
    OUTPUT_STDOUT,
    OUTPUT_STBOUT,
    OUTPUT_IP,
    OUTPUT_FILE
} T_TraceOutputType;

typedef enum 
{
    HEADER_NONE,
    HEADER_DATE,
    HEADER_RUNTIME
} T_TraceHeaderType;

typedef enum 
{
    TRACE_MODE_LIVE,
    TRACE_MODE_AUTOFLUSH,
    TRACE_MODE_MANUAL_FLUSH
} T_TraceMode;
typedef struct _IpAddr
{
    char        szAddr[MAX_ADDRESS_SIZE];
    UINT16      uiPort;

} T_IpAddr;

typedef union 
{
    T_IpAddr    stIpAddr;
    char                szFilename[MAX_FILENAME_LENGTH];
} T_TraceOutputPara;

typedef struct _CurConfiguration
{
    T_TraceMode               m_eTraceMode ;
   T_TraceHeaderType    m_eHeaderType;
    T_TraceOutputType             m_eOutputType ;
    DWORD           m_dwWidth;
    T_TraceOutputPara m_unPara;
} T_CurConfiguration;
typedef enum 
{
    TRACE_LEVEL_NONE,
    TRACE_LEVEL_ERROR,
    TRACE_LEVEL_INFO,
    TRACE_LEVEL_VERBOSE
} T_TraceLevel;

typedef struct _TraceModule
{
    char                m_szModuleName[MAX_MODULE_NAME_LENGTH];
    T_TraceLevel   m_eTraceLevel;
} T_TraceModule;
typedef struct _TraceList
{
    T_TraceModule   m_stModule;
     INT m_nNum;
} T_TraceList;

#endif

/**************************************************************************
*Function Name: CS_CAConfig
*  配置CA参数
*Parameters 
*   
*    m_bCamlock: 是否启用camlock功能
*    m_bIsDrvTest: 是否为驱动测试 (FALSE为认证测试)   
*
*Return Values: 
*   0 : SUCCESS 函数成功
*   -1: FAILURE 函数失败
**************************************************************************/


INT CSCAConfig(CaConfig_S* pstCaConfig);


/**************************************************************************
*Function Name: CS_CA_MMI_PpvConfirm
*  购买PPV节目
*
*Parameters 
*	dwStbStreamHandle[in]:
*		1:Audio
             2:Video
*	dwProgNum[in]:
*		program number
*	dwPinCode[in]:
*		PIN code
       stNewCeiling[in]:
            ceiling for PPVT
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_ALARM_STATUS_OK
*		CA_ALARM_STATUS_BAD_HANDLE | CA_ALARM_STATUS_ERROR | CA_ALARM_STATUS_NO_PURCHASE_PROPOSED
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_PpvpConfirm(DWORD dwStbStreamHandle, DWORD dwProgNum,DWORD dwPinCode);
INT CS_CA_MMI_PpvpConfirmEx(DWORD dwStbStreamHandle, DWORD dwProgNum,DWORD dwPinCode,E_AppAction nActionType);

INT CS_CA_MMI_PpvtConfirm(DWORD dwStbStreamHandle,DWORD dwProgNum,DWORD dwPinCode,DWORD stNewCeiling);
INT CS_CA_MMI_PpvtConfirmEx(DWORD dwStbStreamHandle,DWORD dwProgNum,DWORD dwPinCode,DWORD stNewCeiling,E_AppAction nActionType);

/**************************************************************************
*Function Name: CS_CA_MMI_UnlockMaturity
*	Unlock the maturity rating
*
*Parameters 
*	dwPinCode[in]:
*		PIN code

*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		CA_ALARM_STATUS_BAD_PIN_CODE | CA_ALARM_STATUS_LOCKED_PIN_CODE | CA_ALARM_STATUS_MAX_CHANGES_PIN_CODE
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_UnlockMaturity(DWORD dwPincode);
INT CS_CA_MMI_UnlockMaturityEx(DWORD dwPincode, E_AppAction nActionType);

/**************************************************************************
*Function Name: CS_CA_MMI_UnlockConsultation
*	Unlock consultation restriction
*
*Parameters 
*	dwPinCode[in]:
*		PIN code
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		CA_ALARM_STATUS_BAD_PIN_CODE | CA_ALARM_STATUS_LOCKED_PIN_CODE | CA_ALARM_STATUS_MAX_CHANGES_PIN_CODE
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_UnlockConsultation(DWORD dwPincode);
INT CS_CA_MMI_UnlockConsultationEx(DWORD dwPincode, E_AppAction nActionType);

/**************************************************************************
*Function Name: CS_CA_MMI_ModifyPinCode
*	修改PIN码
*
*Parameters 
*	dwPinOld[in]:
*		旧PIN码
*	dwPinNew[in]:
*		新PIN码
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_ALARM_STATUS_OK
*		CA_ALARM_STATUS_BAD_PIN_CODE| CA_ALARM_STATUS_LOCKED_PIN_CODE | CA_ALARM_STATUS_MAX_CHANGES_PIN_CODE
*		CA_ALARM_STATUS_NO_PIN_CODE
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_ModifyPinCode(DWORD dwPinOld, DWORD dwPinNew);

/**************************************************************************
*Function Name: CS_CA_MMI_ModifyMaturityLevel
*	修改成人级别
*
*Parameters 
*	dwPin[in]:
*		PIN码
*	dwMaturityLevel[in]:
*		成人级别
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_ALARM_STATUS_OK | CA_ALARM_STATUS_NO_PIN
*		CA_ALARM_STATUS_BAD_PIN_CODE | CA_ALARM_STATUS_LOCKED_PIN_CODE | CA_ALARM_STATUS_MAX_CHANGES_PIN_CODE
*		CA_ALARM_STATUS_ERROR | CA_ALARM_STATUS_INVALID_MORALITY_LEVEL_VALUE
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_ModifyMaturityLevel(DWORD dwPin, DWORD dwMaturityLevel);

/**************************************************************************
*Function Name: CS_CA_MMI_ModifyThreshold
*	修改余额下限
*
*Parameters 
*	dwPin[in]:
*		PIN码
*	dwNewThreshold[in]:
*		余额下限值
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_ALARM_STATUS_OK | CA_ALARM_STATUS_NO_PIN
*		CA_ALARM_STATUS_BAD_PIN_CODE | CA_ALARM_STATUS_LOCKED_PIN_CODE | CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_ModifyThreshold(DWORD dwPin, DWORD dwNewThreshold);


/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubSoidList
*	请求运营商信息列表
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_SOID_LIST(INFO消息，结构体T_SoidList)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/

INT CS_CA_MMI_RequestSubSoidList(void);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubThemeSubscriptionList
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_SUBLIST_THEME(INFO消息，结构体T_SubListTheme)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubThemeSubscriptionList(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubThemeSubscriptionList
*	请求授权列表信息
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_SUBLIST_CLASS(INFO消息，结构体T_SubListClass)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubClassSubscriptionList(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubPreBookedList
*	请求可预订信息列表
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_PREBOOKLIST(INFO消息，结构体T_PrebookedList)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubPreBookedList(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestPreselectionList
*	请求预定信息列表
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_PRESELECTIONLIST(INFO消息，结构体T_PreSELECTIONList)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestPreselectionList( );


/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubLabel
*	请求运营商名称
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_SOID_LABEL(INFO消息，结构体T_SoidLabel)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubLabel(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSmcID
*	请求智能卡号
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_SMC_ID(INFO消息，结构体T_SmcID)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSmcID(void);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestMoralityLevel
*	请求智能卡级别
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_PG_LEVEL(INFO消息，结构体T_PgLevel)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestMoralityLevel(void);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestPpvIppvpList
*	请求已购买过的IPPVP节目
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_PPVP_LIST(INFO消息，结构体T_IppvpListValue)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestPpvIppvpList(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestPpvIppvtList
*	请求已购买过的IPPVT节目
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_PPVT_LIST(INFO消息，结构体T_IppvtListValue)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestPpvIppvtList(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestPpvInfoCredit
*	请求总余额信息
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_PPV_CREDIT(INFO消息，结构体T_CreditValue)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestPpvInfoCredit(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestPpvThreshold
*	请求余额下限信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_THRESHOLD(INFO消息，结构体T_Threshold)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestPpvThreshold(void);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestCaVersion
*	请求CA内核信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*		MSG:CA_INFO_CA_VERSION(INFO消息，结构体T_Version)
*		CA_ALARM_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestCaVersion(void);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestCaVersionDetail
*	请求CA内核详细信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给UI
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestCaVersionDetail(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* 保存一封邮件
*
*Parameters 
*	pstTxt[in]:邮件内容
*
*Return Values: 
*	0 : SUCCESS 保存一封邮件到内存中成功
*	-1: FAILURE 参数错误或保存邮件失败
**************************************************************************/
INT CS_CA_TXT_Add(T_Txt *pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_GetNum
* 获取总邮件数
*
*Parameters 
*	无
*
*Return Values: 
*	0 : 没有邮件
*	其它: 邮件总个数
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* 获取具体某一封邮件的内容
*
*Parameters 
*	nIndex[in]:邮件下标，从0开始
*	pstTxt[out]:CA将读出的邮件内容存储到上层分配好的空间
*
*Return Values: 
*	0 : SUCCESS 获取邮件内容成功
*	-1: FAILURE 索引号错误或参数错误，或获取邮件内容错误
**************************************************************************/
INT CS_CA_TXT_GetInfo(INT nIndex, T_Txt* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Del
* 删除某一封邮件
*
*Parameters 
*	nIndex[in]:邮件下标，从0开始
*
*Return Values: 
*	0 : SUCCESS 册除某一封邮件成功
*	-1: FAILURE 索引号错误或删除邮件失败
**************************************************************************/
INT CS_CA_TXT_Del(DWORD dwIndex);

/**************************************************************************
*Function Name:CS_CA_TXT_DelAll
* 删除所有保存过的邮件
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 删除所有邮件成功
*	-1: FAILURE 删除所有邮件失败
**************************************************************************/
INT CS_CA_TXT_DelAll(void);

/**************************************************************************
*Function Name:CS_CA_TXT_SetRead
* 设置邮件是否已读标志
*
*Parameters 
*	nIndex[in]:邮件存储的下标，从0开始
*	bRead[in]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 设置读标志成功
*	-1: FAILURE 索引号错误或设置读标志失败
**************************************************************************/
INT CS_CA_TXT_SetRead(INT nIndex, T_Txt* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* 将所有邮件保存到flash中，一般是关机或断电前进行的操作
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 同步FLASH成功
*	-1: FAILURE 同步FLASH失败
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_GetUnreadNum
* 获取未读邮件的个数
*
*Parameters 
*	无
*
*Return Values: 
*	未读邮件的个数
**************************************************************************/
INT CS_CA_GetUnreadNum( void );

/**************************************************************************
*Function Name:CS_CA_MMI_AddSubPreselection
* Add preselection information
*
*Parameters 
*	dwSoid[in]:
            SOID
      dwPinCode[in]:
            PIN code
*    eReferenceType[in]:
            Type of preselection
      uInfo[in]:
            Preselection information
*Return Values: 
*	SUCCESS : Add successfully
**************************************************************************/
INT CS_CA_MMI_AddSubPreselection(DWORD dwSoid,DWORD dwPinCode,T_PreselectionType eReferenceType,T_InfoPreselection* uInfo);

/**************************************************************************
*Function Name:CS_CA_MMI_DeleteSubPreselection
* Add preselection information
*
*Parameters 
*	dwSoid[in]:
            SOID
      dwPinCode[in]:
            PIN code
*    eReferenceType[in]:
            Type of preselection
      uInfo[in]:
            Preselection information
*Return Values: 
*	SUCCESS : Delete successfully
**************************************************************************/
INT CS_CA_MMI_DeleteSubPreselection(DWORD dwSoid,DWORD dwPinCode,T_PreselectionType eReferenceType,T_InfoPreselection* uInfo);
/**************************************************************************
*Function Name:CS_CA_MMI_RequestCardParingMode
* Get smartcard paring mode
*
*Parameters 
            no
*Return Values: 
*	SUCCESS : Get mode successfully
**************************************************************************/
INT CS_CA_MMI_RequestCardParingMode();
/**************************************************************************
*Function Name:CS_CA_MMI_RequestCardParingNum
* Get number of the paring smartcard
*
*Parameters 
            no
*Return Values: 
*	SUCCESS : Get number successfully
**************************************************************************/
INT CS_CA_MMI_RequestCardParingNum();
/**************************************************************************
*Function Name:CS_CA_MMI_RequestParingCardList
* Get the paring card list
*
*Parameters 
            no
*Return Values: 
*	SUCCESS : Get the list successfully
**************************************************************************/
INT CS_CA_MMI_RequestParingCardList();
/**************************************************************************
*Function Name:CS_CA_MMI_RequestCamLockValue
* Get the CAMLOCK value
*
*Parameters 
            no
*Return Values: 
*	SUCCESS : Get the value successfully
**************************************************************************/
INT CS_CA_MMI_RequestCamLockValue();
/**************************************************************************
*Function Name:CS_CA_MMI_RequestSTBId
* Get the STB Id
*
*Parameters 
            no
*Return Values: 
*	SUCCESS : Get the ID successfully
**************************************************************************/
INT CS_CA_MMI_RequestSTBId(void);
#if PURPLE_BOX
/**************************************************************************
*Function Name:CS_CA_MMI_RequestTerminalId
* Get the Terminal Id
*
*Parameters 
            no
*Return Values: 
*	SUCCESS : Get the Terminal Id successfully
**************************************************************************/
INT CS_CA_MMI_RequestTerminalId(void);
#endif
/**************************************************************************
*Function Name:CS_CA_MMI_GetLeftAttemptsForPincode
* Get the left number of attempt for inputting pincode
*
*Parameters 
            no
*Return Values: 
*	SUCCESS : Get the left number successfully
**************************************************************************/
INT CS_CA_MMI_GetLeftAttemptsForPincode();

/**************************************************************************
* Function Name: CS_CA_GetFlashAddress
*  CA 获取Flash空间地址,为2*64K. 由应用实现该函数
*
*Parameters 空
*Return Values
*       CA FLASH 空间首地址
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(void);
/**************************************************************************
* Function Name: CS_CA_GetSmcStatus
*  获取卡状态,应用进入菜单时调用
*
*Parameters 空
*Return Values
*       卡状态,该函数直接返回卡状态
CA_ALARM_SMC_OUT|CA_ALARM_SMC_CHECKING|CA_ALARM_SMC_OK|CA_ALARM_SMC_COM_ERROR
|CA_ALARM_SMC_INVALID|CA_ALARM_SMC_MEMORY_FULL|CA_ALARM_SMC_ILLEGAL

**************************************************************************/

INT CS_CA_GetSmcStatus();

/**************************************************************************
* Function Name: CS_CA_GetUartHandle
*  获取串口的索引
*
*Parameters 空
*Return Values:串口索引
*      
**************************************************************************/

extern DWORD CS_CA_GetUartHandle();
/**************************************************************************
* Function Name: CSCAGetSerialNumber
*  获取序列号,VIACCESS的序列号格式为12byte的BCD码
*
*Parameters 空
*Return Values:成功SUCCESS
*           失败FAILURE
**************************************************************************/

extern INT CSCAGetSerialNumber(BYTE* pData);


#ifdef  __cplusplus
}
#endif

#endif //_CS_VIACCESSCA_INTERFACE_H_


