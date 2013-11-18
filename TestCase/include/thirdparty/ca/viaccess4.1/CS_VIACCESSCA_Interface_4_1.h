/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_VIACCESSCA_Interface_4_1.h  
* Author:      Yang Wande
* Date:        July 2012
* Description: VIACCESS CA interface     
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
* - Revision 2.0  2012/4/23
*   Programmer:Yang Wande
*   Modify.
*****************************************************************************************************************************/
#ifndef _CS_VIACCESSCA_INTERFACE_H_
#define _CS_VIACCESSCA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define CS_VIACCESSCA_MESSAGE_BASE				(0)

typedef enum _CaEventGroup
{
	CA_ALARM = CS_VIACCESSCA_MESSAGE_BASE,
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	CA_EVENTGROUP_MAX
}CaEventGroup_E;

typedef enum _CaCommand
{
	//Cancel force message.
	CA_COMMAND_FORCE_MSG_HIDE = 10,	
	
	//Display force message.
	CA_COMMAND_FORCE_MSG_SHOW,			/*dwParm2=(DWORD)&CaMsg_S*/

	//Notify the app to input the PIN code.
	CA_COMMAND_KEY_REQUEST,     		/*dwParm2=(DWORD)&CaKeyRequest_S or 0*/

	//Notify the app PPV request.          
	CA_COMMAND_PPV_REQUEST,             /*dwParm2=(DWORD)&CaPpvRequest_S*/

	//Proctect the digital outputs
	CA_COMMAND_DIGITAL_OUTPUT_PROT,     /*dwParm2=(DWORD)&CaDigitalOutputProt_S*/
	
	CA_COMMAND_COPY_PROT,				/*dwParm2=(DWORD)&CopyProtection_S*/

	//Notify the app to suspend all analog channels
	CA_COMMAND_ANALOGCHANNEL_SUSPEND,

	//Notify the app to resume all analog channels
	CA_COMMAND_ANALOGCHANNEL_RESUME,

	//Notify the app to suspend all free channels
	CA_COMMAND_FREECHANNEL_SUSPEND,

	//Notify the app to resume all free channels
	CA_COMMAND_FREECHANNEL_RESUME,

	CA_COMMAND_MAX
}CaCommand_E;

typedef enum _CaAlarm
{
	CA_ALARM_VA_CA = 100,  				/*Don't need to display any msg.*/ 
	/*The program is scrambled by other CA.*/
	CA_ALARM_OTHER,			            /*Display the message.*/ 	
	CA_ALARM_FREE,                      /*Open AV, don't need to display any msg.*/

	CA_ALARM_SMC_IN,                    /*Don't need to display any msg.*/
	/*Please insert your smartcard.*/
	CA_ALARM_SMC_OUT,                   /*Display the message.*/  
	/*"Checking the card !"*/			
	CA_ALARM_SMC_CHECKING = 105,        /*Display the message.*/ 
	CA_ALARM_SMC_OK,                    /*Don't need to display any msg.*/  
	/*Please insert the smartcard to access the program.*/
	CA_ALARM_SMC_NOT_FOUND,	            /*Display the message.*/
    /*Please insert the smartcard.*/
	CA_ALARM_SMC_NEEDED,	            /*Display the message.*/
	/*Smartcard memory is full.*/
	CA_ALARM_SMC_INSUFFICIENT_MEMORY,   /*Display the message.*/
    /*Invalid smartcard.*/
	CA_ALARM_SMC_NOT_ACCEPTED = 110,    /*Display the message.*/
	/*Smartcard error.*/
	CA_ALARM_SMC_FAILED,    		    /*Display the message.*/
	/*The smartcard has been invalidated by the operator. Please contact the operator for further information.*/
	CA_ALARM_SMC_INVALIDATED,           /*Display the message.*/
	/*Your smartcard is locked.Please contact your operator.*/
	CA_ALARM_SMC_LOCKED,	            /*Display the message.*/
	/*Wrong Card inserted.Please contact the operator for further information.*/
	CA_ALARM_PAIRING_MISMATCH,	        /*Display the message.*/
	
	/*The terminal configuration is full. Please contact the operator for further information.*/
	CA_ALARM_TERMINAL_INSUFFICIENT_MEMORY = 115,  /*Display the message.*/
	/*The terminal configuration is corrupted. Please contact the operator for further information.*/
	CA_ALARM_TERMINAL_CONFIGURATION_ERROR,        /*Display the message.*/
    /*The terminal is not activated. Please contact the operator for further information.*/
	CA_ALARM_TERMINAL_NOT_ACTIVATED,    /*Display the message.*/
	/*The terminal has been invalidated by the operator. Please contact the operator for further information.*/
	CA_ALARM_TERMINAL_INVALIDATED,      /*Display the message.*/
	/*Personalization of the terminal failed. Please contact your operator.*/
	CA_ALARM_PERSONALIZATION_KO,        /*Display the message.*/
	
	CA_ALARM_RIGHT_OK = 120,			/*Open AV, dwParm2(0:program,1:video,2:audio),when "program", don't need to display any msg.*/
	CA_ALARM_PREVIEW_OK,				/*Open AV, dwParm2 concerns address of CaPreviewProgramInfo_S.*/
	CA_ALARM_PREVIEW_AND_IPPV_OFFER_OK, /*Open AV, dwParm2 concerns address of CaPreviewProgramInfo_S.*/
	
	/*You do not have the rights to access the <specific text>.*/
	CA_ALARM_NO_RIGHT,					/*<specific text> should be replaced by "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*Geographical blackout for the <specific text>.*/
	CA_ALARM_GEOGRAPHICAL_BLACKOUT,		/*<specific text> should be replaced by "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*Access not granted because the key is not present. If this <specific text> is in your subscription please contact your operator.*/
	CA_ALARM_MISSING_KEY = 125,		    /*<specific text> should be replaced by "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*Purchase is not possible for the <specific text>.*/
	CA_ALARM_IPPV_OFFER_EXPIRED,		/*<specific text> should be replaced by "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*Access to this <specific text> has been forbidden by the operator.*/
	CA_ALARM_FORBIDDEN_MODE,		    /*<specific text> should be replaced by "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*You do not have sufficient credit to purchase this <specific text>.*/
	CA_ALARM_INSUFFICIENT_CREDIT,		/*<specific text> should be replaced by "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*You cannot view this <specific text> because the visualization date has expired.*/
	CA_ALARM_RIGHT_EXPIRED,		        /*<specific text> should be replaced by "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*You cannot view this <specific text> because the maximum number of playbacks has been reached.*/
	CA_ALARM_RIGHT_CONSUMED = 130,		/*<specific text> should be replaced by "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*Terminal configuration is not accurate. Please contact your operator for further information.*/
	CA_ALARM_CW_PROTECTION_MISMATCH,	/*It is "program" or by "video" or "audio" concerns dwParm2(0:program,1:video,2:audio), when "audio", video should display.*/
	/*You do not have the rights to record this <specific text>.*/
	CA_ALARM_RECORD_RIGHT_KO,	        /*<specific text> should be replaced by "program"or by "video" concerns dwParm2(0:program,1:video,2:audio)*/
	/*You do not have timeshift rights.*/
	CA_ALARM_TIMESHIFT_RIGHT_KO,	    /*Display the message.*/
    /*You have reached your credit threshold.*/
	CA_ALARM_THRESHOLD_REACHED,         /*Display the message. Descrambled or clear Video should be displayed*/
	/*You have exceeded your credit overdraft.*/
	CA_ALARM_OVERDRAFT_REACHED = 135,   /*Display the message. Descrambled or clear Video should be displayed*/
    /*The maturity rating of this program exceeds your setting.*/
	CA_ALARM_MATURITY_RATING,			/*Display the message.*/
	/*This program is IPPV program.*/
	CA_ALARM_IPPV_PROGRAM,			    /*Display the message.*/
	/*A security restriction has been activated on this STB! Please contact your operator.*/
	CA_ALARM_START_SECURITY_RESTRICTION,/*Display the message. Descrambled or clear Video should be displayed*/
	CA_ALARM_STOP_SECURITY_RESTRICTION, /*Don't need to display any msg.*/
	CA_ALARM_CAMLOCK_NOT_ACTIVATED = 140,     /*Don't need to display any msg.*/
	CA_ALARM_CAMLOCK_DISABLED,			/*Don't need to display any msg.*/
	CA_ALARM_CAMLOCK_ACTIVATED_OK,	    /*Don't need to display any msg.*/
	CA_ALARM_RECORD_RIGHT_OK,			/*Don't need to display any msg.*/
	CA_ALARM_TIMESHIFT_RIGHT_OK,		/*Don't need to display any msg.*/
	CA_ALARM_RECORD_INFO_CHANGE = 145,	/*Don't need to display any msg.*/
	/*The inserted card is not compliant with this STB! Please contact your operator.*/
	CA_ALARM_CAMLOCK_ACTIVATED_KO,	    /*Close AV. Display the message. Lock STB if camlock level is high.*/
	/*Please insert your smartcard.*/
	CA_ALARM_CAMLOCK_ACTIVATED_NO_CARD, /*Close AV. Display the message.*/
						
	CA_ALARM_MAX
}CaAlarm_E;

typedef enum _CaStatus
{					
	CA_STATUS_OK = 200,                       
    CA_STATUS_ARRAY_TOO_SMALL,    
    CA_STATUS_WRONG_PIN_CODE,       
    CA_STATUS_LOCKED_PIN_CODE,    
    CA_STATUS_MAX_CHANGES_PIN_CODE, 
    
    CA_STATUS_FORBIDDEN_PIN_CODE = 205,   
    CA_STATUS_NO_PIN_CODE,         
    CA_STATUS_INVALID_MORALITY_LEVEL_VALUE,
    CA_STATUS_NOT_ENOUGH_MEMORY,   
    CA_STATUS_NO_CARD,
    
    CA_STATUS_CARD_ERROR = 210,
    CA_STATUS_BAD_SOID, 
    CA_STATUS_OPERATION_NOT_ACCEPTED, 
    CA_STATUS_PRESELECTION_AREA_FULL, 
    CA_STATUS_PROG_ALREADY_STORED_IN_PRESELECTION_AREA, 
    
    CA_STATUS_PROG_NOT_FOUND_IN_PRESELECTION_AREA = 215,  
    CA_STATUS_PRESELECTION_MODE_NOT_ACCEPTED,  
    CA_STATUS_CONSULTATION_LOCKED, 
    CA_STATUS_BAD_HANDLE, 
    CA_STATUS_NO_IPPV_OFFER_AVAILABLE,
    
    CA_STATUS_NO_ITEM_FOUND = 220,
    CA_STATUS_INSUFFICIENT_MEMORY,
    CA_STATUS_INFO_NOT_AVAILABLE,
    CA_STATUS_WRONG_ACS_ID,
    CA_STATUS_RESOURCE_BUSY,
    
    CA_STATUS_OPERATING_ERROR = 225,
    CA_STATUS_MODE_NOT_SETUP,
    CA_STATUS_WRONG_MODE,
    CA_STATUS_NO_STB_ID,
    CA_STATUS_PVR_NO_RIGHTS,
    
    CA_STATUS_CONNECTION_ERROR = 230,
    CA_STATUS_WRONG_ACTIVATION_CODE,
    CA_STATUS_ACTIVATION_ERROR,
    CA_STATUS_TERMINAL_NOT_ACTIVATED,
    CA_STATUS_TERMINAL_LOCKED,
    
    CA_STATUS_ERROR = 235,
    CA_STATUS_SMC_IN, 
    CA_STATUS_SMC_OUT,
    CA_STATUS_SMC_OK,
    /* Used to know the end */
    CA_STATUS_LAST,
    CA_SECURITY_STATUS = 240,                  /*dwParm2 = CSCaSecurityStatus_S*/

	CA_STATUS_MAX
}CaSatus_E;

typedef enum _CaSecurityStatus_E
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
}CaSecurityStatus_E;

typedef enum _CaInfo
{
	//mail content message     
	CA_INFO_TXT = 300,              	/*dwParm2 = (DWORD)&CaMail_S;*/

	//SOID information list  
	CA_INFO_SOID_LIST,        			/*dwParm2 = (DWORD)&CaSoidList_S*/
	 
	CA_INFO_SUBLIST_THEME,    			/*dwParm2 = (DWORD)&CaSubListTheme_S*/
 
	CA_INFO_SUBLIST_CLASS,    			/*dwParm2 = (DWORD)&CaSubListClass_S*/
	            
	CA_INFO_PREBOOKLIST,      			/*dwParm2 = (DWORD)&CaPrebookedList_S*/

	//SOID name
	CA_INFO_SOID_LABEL = 305,       	/*dwParm2 = (DWORD)&CaSoidLabel_S*/
                                            
	CA_INFO_SMC_ID,           			/*dwParm2 = (DWORD)&CaSmcID_S or NULL*/

	//Not used now.
	CA_INFO_LOCK_STATUS,      			/*dwParm2 = (DWORD)&T_LockStatus*/
                                   
	CA_INFO_PG_LEVEL,         			/*dwParm2 = (DWORD)&CaPgLevel_S*/
                
	CA_INFO_PPVP_LIST,        			/*dwParm2 = (DWORD)&CaIppvpListValue_S*/
                     
	CA_INFO_PPVT_LIST = 310,        	/*dwParm2 = (DWORD)&CaIppvtListValue_S*/
                  
	CA_INFO_PPV_CREDIT,       			/*dwParm2 = (DWORD)&CaCreditValue_S*/
                                   
	CA_INFO_THRESHOLD,        			/*dwParm2 = (DWORD)&CaThreshold_S*/
	                                         
	CA_INFO_CA_VERSION,	      			/*dwParm2 = (DWORD)&CaVersion_S*/
                                 
	CA_INFO_CA_VERSION_DETAIL,			/*dwParm2 = (DWORD)&CaVersionDetail_S*/

	CA_INFO_STB_ID = 315,				/*dwParm2 = (DWORD)&CaSTBId_S*/

	CA_INFO_TERMINAL_ID,				/*dwParm2 = (DWORD)&CaTerminalId_S*/

	CA_INFO_CAMLOCK_VALUE,				/*dwParm2 = (DWORD)&CaCamlock_S*/

	CA_INFO_CARD_PARING_MODE,			/*dwParm2 = (DWORD)CaCardParingMode_E*/

	CA_INFO_PARING_CARD_NUM,			/*dwParm2 = (DWORD)&CaCardNumForParing_S*/

	CA_INFO_PARING_CARD_LIST = 320,		/*dwParm2 = (DWORD)&CaParingCardList_S*/
	
    CA_INFO_PRESELECTIONLIST,			/*dwParm2 = (DWORD)&CaPreSelectionList_S*/
    
    CA_INFO_NULL,

    CA_INFO_ATTEMPTSFORPINCODE,			/*dwParm2 = (DWORD)&CaAttemptForPincode_S*/

    CA_INFO_PREVIEWPROGRESS,			/*dwParm2 = (DWORD)&CaPreviewProgress_S*/
    	
    //the attemps for pin code is unlimited
    CA_INFO_UNLIMITED_ATTEMPTSFORPINCODE = 325,
    
    CA_INFO_SECURITY_PAIRING_SANCTION,	/*dwParm2 = (DWORD)&T_PairingSanctionItem*/
    
    CA_INFO_SECURITY_STATUS,			/*dwParm2 = (DWORD)&T_SecStatus*/
    
    CA_INFO_SECURITY_FORBIDDEN_SOID_LIST,	/*dwParm2 = (DWORD)&T_ForbiddenSoidList*/
    
    CA_INFO_SECURITY_DETECTION_CONFIG,	/*dwParm2 = (DWORD)&T_DetectionConfigItem*/

     CA_INFO_MAX
}CaInfo_E;

typedef enum _AppAction
{
    CA_APP_ACTION_CANCEL,
    CA_APP_ACTION_CONFIRM,    
    CA_APP_ACTION_MAX
}CaAppAction_E;

typedef enum _PreselectionType
{
    E_PRESELECTION_RESERVED,
    E_PRESELECTION_THEMELEVEL,
    E_PRESELECTION_CLASS,
    E_PRESELECTION_PREBOOKED,
    E_PRESELECTION_PPVP,
    E_PRESELECTION_PPVT
}CaPreselectionType_E;

typedef enum _CAMLockLevel
{
    E_LOW_LEVEL,
    E_INTERMEDIATE_LEVEL,
    E_HIGH_LEVEL
}CaCAMLockLevel_E;

typedef struct _CSCaSecurityStatus_S
{
   CaSecurityStatus_E m_emSecurityStatus;
}CSCaSecurityStatus_S;

typedef struct _KeyRequest
{
	unsigned char m_dwPgLevel;
	BOOL m_dwLockStatus;
	DWORD m_dwAcsId;
}CaKeyRequest_S;

typedef enum _PpvMode
{
	E_PPV_NO = 0,
	E_PPV_P  = 0x01,
	E_PPV_T  = 0x02,
	E_PPV_TP = 0x3,
	E_PPV_MAX
}CaPpvMode_E;

typedef enum _CardParingMode
{   
    CARD_NO_MODE,
    CARD_AUTOPAIRING_MODE,
    CARD_PAIRING_MODE     
}CaCardParingMode_E;

typedef struct _CardNumForParing
{
    DWORD m_dwMaxNum;
    DWORD m_dwSmcNum;   
}CaCardNumForParing_S;

#define MAX_PAIRING_SMARTCARD_NUMBER     (8)
#define CA_MAX_SMCID_LEN                 (14)
#define CA_CURRENCY_LENGTH               (17)

typedef struct _ParingCardList
{
    DWORD  m_dwCardNum;
    BYTE   m_dwCardList[MAX_PAIRING_SMARTCARD_NUMBER][CA_MAX_SMCID_LEN];
}CaParingCardList_S;

typedef struct _PreviewProgramInfo
{
    BYTE  m_ucPreviewIndex;
    WORD  m_wPreviewNumber;
	DWORD m_dwSoid;
}CaPreviewProgramInfo_S;

typedef struct _Price
{
	BOOL  m_bToken;
	float m_fPpvPrice;
	BYTE  m_szCurrency[CA_CURRENCY_LENGTH];
}CaPrice_S;

typedef struct _PpvPrice
{
	CaPrice_S m_stPpvp;
	CaPrice_S m_stPpvt;	
}CaPpvPrice_S;

typedef struct _PpvSrvID
{
	DWORD m_dwPpvpSrvID;
	DWORD m_dwPpvtSrvID;
}CaPpvSrvID_S;

typedef enum _PaymentMode
{
    POST_PAYMENT_MODE,
    PRE_PAYMENT_MODE,
    UNKNOWN_PAYMENT_MODE
}CaPaymentMode_E;

typedef struct _PpvRequest
{
	DWORD 			    m_dwAcsId;
    DWORD 			    m_dwSoid;
    DWORD               m_dwPpvHandle;
    CaPaymentMode_E     m_PayMode;
    CaPrice_S           m_stCeiling;
    CaPpvMode_E         m_ePpvMode;

    CaPpvPrice_S        m_stPpvPrice;
    CaPpvSrvID_S        m_stPpvSrvID;
    CaPrice_S           m_stRemainCredit;
    BOOL                m_bWithoutModem;

	BOOL                m_bPreview;
	BOOL                m_bMaturity;
}CaPpvRequest_S;

typedef enum _EncodeFormat
{
    ENCODING_ASCII         = 0,
    ENCODING_UTF_8         = 1,
    ENCODING_UTF_16        = 2,
    ENCODING_UTF_32        = 3,
    ENCODING_GB2312_80     = 4
}CaEncodeFormat_E;

typedef struct _Msg
{
	BOOL             m_bPriporityHigh; //TRUE:短消息, FLASE:普通邮件
	DWORD            m_dwSoid;
	CaEncodeFormat_E m_Encoding;
	DWORD            m_dwLen;
	BYTE*            m_pszContent;
}CaMsg_S;

typedef struct _SoidList
{
	DWORD  m_dwSoidNum;
	DWORD* m_padwSoidList; //Service operator id list concerns soid.                 
}CaSoidList_S;

typedef struct _AttemptForPincode
{
    UINT8 RemainingCount;
    UINT8 MaxCount;
}CaAttemptForPincode_S;

typedef struct _Date
{
	DWORD m_dwYear;
	BYTE  m_cMonth;
	BYTE  m_cDay;
}CaDate_S;

typedef struct _ThemeSubList		
{
	CaDate_S m_stBegin;
	CaDate_S m_stEnd;
	BYTE     m_cTheme;
	BYTE     m_cLevel;	
}CaThemeSubList_S;
	
typedef struct _SubListTheme
{
	DWORD             m_dwNum;
	CaThemeSubList_S* m_pastSubList;
}CaSubListTheme_S;

typedef struct _ClassSubList
{
    CaDate_S m_stBegin;
    CaDate_S m_stEnd;
    DWORD    m_dwClassNum;
    BYTE     m_pacClass[256];
}CaClassSubList_S;

typedef struct _SubListClass
{
	DWORD         	  m_dwNum;
	CaClassSubList_S* m_pastSubList;
}CaSubListClass_S;

typedef struct _PrebookedRange
{
	DWORD m_dwBeginProgNum;
	DWORD m_dwEndProgNum;
}CaPrebookedRange_S;

typedef struct _PrebookedList
{
	DWORD               m_dwNum;
	CaPrebookedRange_S* m_pastPreBookedList;
}CaPrebookedList_S;

typedef struct _PreselectionPrice
{
    INT32   iInteger; 
    UINT16  uiFraction; 
}CaPreselectionPrice_S;

typedef struct _Thle
{
    CaDate_S m_Date;
    BYTE     m_Theme;
    BYTE     m_Level;
}CaThemeLevel_S;

typedef struct _Link
{
    CaDate_S m_Date;
    BYTE     m_Link;
}CaLink_S;

typedef struct _IppvP
{
    DWORD       		  m_dwNumb;
    CaPreselectionPrice_S m_dwCost;
}CaIppvP_S;

typedef struct _IppvT
{
    DWORD       		  m_dwNumb;
    CaPreselectionPrice_S m_dwCost;
}CaIppvT_S;

typedef union _InfoPreselection
{
    CaThemeLevel_S      m_Thle;
    CaLink_S            m_Link;
    DWORD              	m_dwPnumb;
    CaIppvP_S           m_Ppvp;
    CaIppvT_S           m_Ppvt;
}CaPreselectionInfo_S;

typedef struct _Preselection
{
    DWORD                m_dwSoid;
    CaPreselectionType_E m_Type;
    CaPreselectionInfo_S m_Info;
}CaPreselection_S;

typedef struct _PreSelectionList
{
	DWORD            m_dwNum;
	CaPreselection_S *m_pastPreSelectionList;
}CaPreSelectionList_S;

typedef struct _SoidLabel
{
	DWORD m_dwSoid;
	DWORD m_dwLabelLen;
	BYTE* m_pacLabel;
}CaSoidLabel_S;

typedef struct _SmcID
{
	DWORD m_dwLen;
	BYTE* m_pacID;
}CaSmcID_S;

typedef struct _PgLevel
{
	BYTE m_cPgLevel;
	BYTE m_cPgLevelMaxAllowed;
}CaPgLevel_S;

typedef struct _PreviewProgress
{
    unsigned char UsedCWNum;
    unsigned char MaxCWNum;
}CaPreviewProgress_S;

typedef struct _PpvRecordValue
{
    DWORD m_dwProgNum;
    FLOAT m_fCostTokens;
    BYTE  m_szCurrency[CA_CURRENCY_LENGTH];
}CaPpvRecordValue_S;

typedef struct _IppvpListValue
{
    DWORD               m_dwNum;
    CaPpvRecordValue_S* m_pastPpvRecord;
}CaIppvpListValue_S;

typedef struct _IppvtListValue
{
    DWORD               m_dwNum;
    CaPpvRecordValue_S* m_pastPpvRecord;
}CaIppvtListValue_S;

typedef struct _CreaditValue
{
    FLOAT m_fRemain;/* Remaining credit: Set with the remaining credit available in the smartcard for a given program */
	BYTE  m_szCurrency[CA_CURRENCY_LENGTH];/* add at 2006-05-19*/
}CaCreditValue_S;

typedef struct _Threshold
{
	FLOAT m_fThresholdToken;
}CaThreshold_S;

typedef struct _Version
{
	DWORD m_dwLen;
	BYTE* m_pszVersion;
}CaVersion_S;

typedef struct _DateTime
{
    DWORD m_dwYear;
    BYTE  m_cMonth;
    BYTE  m_cDay;
    BYTE  m_cHour;
    BYTE  m_cMinute;
}CaDateTime_S;

typedef struct _STBId
{
    BOOL  m_bIsIdValid;
    BYTE* m_pSTBId;
    DWORD m_dwLen;        
}CaSTBId_S;

typedef enum _CamlockDynamicState
{
    CAMLOCK_ACTIVATED_OK      = 0x01,
    CAMLOCK_ACTIVATED_KO,
    CAMLOCK_ACTIVATED_NO_CARD,
    CAMLOCK_NOT_ACTIVATED,
    CAMLOCK_DISABLED
}CaCamlockDynamicState_E;

typedef struct _CAMLOCK
{
    CaCamlockDynamicState_E m_Camlockstatus;
	DWORD                   m_dwLen;
    BYTE*                   m_pCamlockValue;        
}CaCamlock_S;

typedef struct _TerminalId
{
	DWORD   m_dwLen;
    BYTE*   m_pTerminalId;      
}CaTerminalId_S;

typedef struct _VersionDetail
{
	DWORD   m_dwDelVerNum;
	DWORD   m_dwMajorVerNum;
	DWORD   m_dwMinorVerNum;
	DWORD   m_dwEditionVerNum;
	DWORD   m_dwRevisionVerNum;	
	DWORD   m_dwOptionMask;//no use
	BYTE*   m_pszCasVariant;	
	BYTE*	m_psztOperatorName; 	
}CaVersionDetail_S;

typedef struct _Txt
{
	BOOL           m_bReadFlag;
	CaDateTime_S   m_stTime;
	CaMsg_S        m_stMsg;
}CaMail_S;

typedef struct _DigitalOutputProt
{
    BOOL           m_bSPDIFActive;
    BOOL           m_bEthernetActive;  
    BOOL           m_bUSBActive;  
    BOOL           m_bILinkActive;    
    BOOL           m_bDvbCiActive;    
    BOOL           m_bHddActive;   
    BOOL           m_bDvdActive;
}CaDigitalOutputProt_S;

typedef enum _Macrovision_E
{
	MACROVISION_ACTIVED   =0,
	MACROVISION_DEACTIVED
}Macrovision_E;

typedef enum _CGMS_A_E
{
	CGMS_A_COPY_FREELY  = 0,
	CGMS_A_COPY_NOMORE,
	CGMS_A_COPY_ONECE,
	CGMS_A_COPY_NEVER	
}CGMS_A_E;

typedef enum _DTCP_IP_Redistribution_E
{
	DTCP_IP_REDISTRIBUTION_ACTIVED   =0,
	DTCP_IP_REDISTRIBUTION_DEACTIVED
}DTCP_IP_Redistribution_E;

typedef enum _DTCP_IP_configuration_E
{
	DTCP_IP_COPY_FREELY  = 0,
	DTCP_IP_COPY_NOMORE,
	DTCP_IP_COPY_ONEGENERATION,
	DTCP_IP_COPY_NEVER	
}DTCP_IP_configuration_E;

typedef struct _CopyProtection_S
{
	Macrovision_E               m_emMacrovision;
	CGMS_A_E                    m_emCGMS_A;
	DTCP_IP_Redistribution_E 	m_emDTCP_IP_Redistribution;
	DTCP_IP_configuration_E  	m_emDTCP_IP_configuration;
}CopyProtection_S;

typedef struct _StbInformation_S
{
	BYTE  m_ucManufacturerId;
    BYTE  m_ucModelId;
    BYTE  m_ucHardwareType;
    BYTE  m_ucHardwareVer;
    BYTE  m_ucSoftwareType;
    BYTE  m_ucSoftwareVer;
}CaStbInformation_S;

/**************************************************************************
*Function Name: CS_CA_MMI_PpvpConfirm
*  购买PPV节目
*
*Parameters 
*	dwAcsId[in]:通过CA_COMMAND_PPV_REQUEST所带CaPpvRequest_S结构体获取
*	dwStbStreamHandle[in]:通过CA_COMMAND_PPV_REQUEST所带CaPpvRequest_S结构体获取,
*		等同于m_dwPpvHandle.
*	dwProgNum[in]:通过CA_COMMAND_PPV_REQUEST所带CaPpvRequest_S结构体获取,
*		等同于m_stPpvSrvID.m_dwPpvpSrvID
*	dwPinCode[in]:
*		PIN code
*   nActionType[in]:
*       CA_APP_ACTION_CANCEL or CA_APP_ACTION_CONFIRM
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给应用
*		当选择操作类型为CA_APP_ACTION_CANCEL时，将上抛CA_ALARM_IPPV_PROGRAM.
*		当选择操作类型为CA_APP_ACTION_CONFIRM时，将上抛
*		CA_STATUS_OK | CA_STATUS_WRONG_ACS_ID | CA_STATUS_BAD_HANDLE |
*		CA_STATUS_WRONG_PIN_CODE | CA_STATUS_LOCKED_PIN_CODE |
*		CA_STATUS_NO_PIN_CODE | CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | 
*		CA_STATUS_NO_IPPV_OFFER_AVAILABLE | CA_STATUS_ERROR | CA_STATUS_TERMINAL_LOCKED
*		| CA_STATUS_INSUFFICIENT_MEMORY
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_PpvpConfirm(DWORD dwAcsId, DWORD dwStbStreamHandle, DWORD dwProgNum, DWORD dwPinCode, CaAppAction_E nActionType);

/**************************************************************************
*Function Name: CS_CA_MMI_PpvtConfirm
*  购买PPV节目
*
*Parameters 
*	dwAcsId[in]:通过CA_COMMAND_PPV_REQUEST所带CaPpvRequest_S结构体获取
*	dwStbStreamHandle[in]:通过CA_COMMAND_PPV_REQUEST所带CaPpvRequest_S结构体获取,
*		等同于m_dwPpvHandle.
*	dwProgNum[in]:通过CA_COMMAND_PPV_REQUEST所带CaPpvRequest_S结构体获取,
*		等同于m_stPpvSrvID.m_dwPpvtSrvID
*	dwPinCode[in]:
*		PIN code
*   fNewCeiling[in]:购买上限,通过CA_COMMAND_PPV_REQUEST所带CaPpvRequest_S结构体获取;
*   eActionType[in]:
*       CA_APP_ACTION_CANCEL or CA_APP_ACTION_CONFIRM
*
*Return Values: 
*	0 : SUCCESS 参数正确,其购买结果将通过消息上报给应用
*		当选择操作类型为CA_APP_ACTION_CANCEL时，将上抛CA_ALARM_IPPV_PROGRAM.
*		当选择操作类型为CA_APP_ACTION_CONFIRM时，将上抛
*		CA_STATUS_OK | CA_STATUS_WRONG_ACS_ID | CA_STATUS_BAD_HANDLE |
*		CA_STATUS_WRONG_PIN_CODE | CA_STATUS_LOCKED_PIN_CODE |
*		CA_STATUS_NO_PIN_CODE | CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | 
*		CA_STATUS_NO_IPPV_OFFER_AVAILABLE | CA_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_PpvtConfirm(DWORD dwAcsId, DWORD dwStbStreamHandle, DWORD dwProgNum, DWORD dwPinCode, float fNewCeiling, CaAppAction_E eActionType);

/**************************************************************************
*Function Name: CS_CA_MMI_UnlockMaturity
*	Unlock the maturity rating
*
*Parameters 
*	dwAcdId[in]:
*       ACS ID, 通过CA_COMMAND_KEY_REQUEST所带CaKeyRequest_S结构体获取
*   dwPinCode[in]:
*		PIN code
*   eActionType[in]:
*		Action type

*Return Values: 
*	0 : SUCCESS 参数正确,其结果将通过消息上报给应用
*		当选择操作类型为CA_APP_ACTION_CANCEL时，将上抛CA_ALARM_MATURITY_RATING.
*		当选择操作类型为CA_APP_ACTION_CONFIRM时，将上抛
*		CA_STATUS_OK | CA_STATUS_WRONG_ACS_ID | CA_STATUS_WRONG_PIN_CODE | 
*		CA_STATUS_LOCKED_PIN_CODE | CA_STATUS_NO_PIN_CODE | CA_STATUS_NO_CARD | 
*		CA_STATUS_CARD_ERROR | CA_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_UnlockMaturity(DWORD dwAcdId, DWORD dwPincode, CaAppAction_E eActionType);

/**************************************************************************
*Function Name: CS_CA_MMI_UnlockConsultation
*	Unlock consultation restriction
*
*Parameters 
*	dwPinCode[in]:
*		PIN code
*
*Return Values: 
*	0 : SUCCESS 参数正确,其执行结果将通过消息上报给应用
*		CA_STATUS_OK | CA_STATUS_BAD_HANDLE | CA_STATUS_WRONG_PIN_CODE | 
*		CA_STATUS_LOCKED_PIN_CODE | CA_STATUS_NO_PIN_CODE | CA_STATUS_NO_CARD | 
*		CA_STATUS_CARD_ERROR | CA_STATUS_ERROR | CA_STATUS_TERMINAL_LOCKED
*		| CA_STATUS_INSUFFICIENT_MEMORY
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_UnlockConsultation(DWORD dwPincode);
INT CS_CA_MMI_UnlockConsultationEx(DWORD dwPincode, CaAppAction_E nActionType);

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
*	0 : SUCCESS 参数正确,其结果将通过消息上报给应用
*		CA_STATUS_OK | CA_STATUS_MAX_CHANGES_PIN_CODE | CA_STATUS_FORBIDDEN_PIN_CODE |
*		CA_STATUS_WRONG_PIN_CODE | CA_STATUS_LOCKED_PIN_CODE |
*		CA_STATUS_NO_PIN_CODE | CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | 
*		CA_STATUS_ERROR | CA_STATUS_TERMINAL_LOCKED | CA_STATUS_NOT_ENOUGH_MEMORY
*		| CA_STATUS_INSUFFICIENT_MEMORY
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
*	0 : SUCCESS 参数正确,其结果将通过消息上报给应用
*		CA_STATUS_OK | CA_STATUS_WRONG_PIN_CODE | CA_LOCKED_PIN_CODE |
*		CA_NO_PIN_CODE | CA_INVALID_MATURITY_RATING | CA_STATUS_NOT_ENOUGH_MEMORY
*		| CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR |
*		CA_STATUS_TERMINAL_LOCKED | CA_STATUS_INSUFFICIENT_MEMORY
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
*	fNewThreshold[in]:
*		余额下限值
*
*Return Values: 
*	0 : SUCCESS 参数正确,其结果将通过消息上报给应用
*		CA_STATUS_OK | CA_STATUS_WRONG_PIN_CODE | CA_LOCKED_PIN_CODE |
*		CA_NO_PIN_CODE | CA_STATUS_NOT_ENOUGH_MEMORY
*		| CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR |
*		CA_STATUS_TERMINAL_LOCKED | CA_STATUS_INSUFFICIENT_MEMORY
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_ModifyThreshold(DWORD dwPin, float fNewThreshold);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubSoidList
*	请求运营商信息列表
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_SOID_LIST;否则上抛 
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR 
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubSoidList(void);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubLabel
*	请求运营商名称
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_SOID_LABEL;否则上抛 CA_STATUS_BAD_SOID |
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR 
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubLabel(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubThemeSubscriptionList
*	请求授权列表信息
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_SUBLIST_CLASS;否则上抛 CA_STATUS_BAD_SOID |
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR | 
*		CA_STATUS_CONSULTATION_LOCKED | CA_STATUS_INFO_NOT_AVAILABLE
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubClassSubscriptionList(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubThemeSubscriptionList
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_SUBLIST_THEME;否则上抛 CA_STATUS_BAD_SOID |
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR | 
*		CA_STATUS_CONSULTATION_LOCKED | CA_STATUS_INFO_NOT_AVAILABLE
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubThemeSubscriptionList(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSubPreBookedList
*	请求Prebooked program信息列表
*
*Parameters 
*	dwSoid[in]
*
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PREBOOKLIST;否则上抛 CA_STATUS_BAD_SOID |
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR | 
*		CA_STATUS_CONSULTATION_LOCKED | CA_STATUS_INFO_NOT_AVAILABLE
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestSubPreBookedList(DWORD dwSoid);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestPreselectionList
*	请求Preselection信息列表
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PRESELECTIONLIST;否则上抛 
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR | 
*		CA_STATUS_CONSULTATION_LOCKED
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestPreselectionList(void);

/**************************************************************************
*Function Name: CS_CA_MMI_RequestSmcID
*	请求智能卡号
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_SMC_ID;否则上抛 
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR 
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
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PG_LEVEL;否则上抛 
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR
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
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PPVP_LIST;否则上抛 CA_STATUS_BAD_SOID 
*		| CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR |
*		CA_STATUS_CONSULTATION_LOCKED
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
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PPVT_LIST;否则上抛 CA_STATUS_BAD_SOID 
*		| CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR |
*		CA_STATUS_CONSULTATION_LOCKED
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
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PPV_CREDIT;否则上抛 CA_STATUS_BAD_SOID 
*		| CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR |
*		CA_STATUS_CONSULTATION_LOCKED | CA_STATUS_INFO_NOT_AVAILABLE
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
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_THRESHOLD;否则上抛 
*		CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | CA_STATUS_ERROR 
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
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_CA_VERSION;否则上抛 
*		CA_STATUS_ERROR
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
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_CA_VERSION_DETAIL;否则上抛 
*		CA_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestCaVersionDetail(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* 保存一封邮件.收到CA_INFO_TXT后调用此接口进行保存.保存前需填充邮件接收时间.
*
*Parameters 
*	pstTxt[in]:邮件内容
*
*Return Values: 
*	0 : SUCCESS 保存一封邮件到内存中成功
*	-1: FAILURE 参数错误或保存邮件失败
**************************************************************************/
INT CS_CA_TXT_Add(CaMail_S *pstTxt);

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
INT CS_CA_TXT_GetInfo(INT nIndex, CaMail_S* pstTxt);

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
*	pstTxt[out]:CA将读出的邮件内容存储到上层分配好的空间
*
*Return Values: 
*	0 : SUCCESS 设置读标志成功
*	-1: FAILURE 索引号错误或设置读标志失败
**************************************************************************/
INT CS_CA_TXT_SetRead(INT nIndex, CaMail_S* pstTxt);

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
INT CS_CA_GetUnreadNum(void);

/**************************************************************************
*Function Name:CS_CA_MMI_AddSubPreselection
* Add preselection information
*
*Parameters 
*	dwSoid[in]:
*           SOID
*   dwPinCode[in]:
*           PIN code
*   eReferenceType[in]:
*           Type of preselection
*   uInfo[in]:
*           Preselection information
*Return Values: 
*	0 : SUCCESS 参数正确,其执行结果将通过消息上报给应用
*		CA_STATUS_OK | CA_STATUS_BAD_SOID | CA_STATUS_OPERATION_NOT_ACCEPTED |
*		CA_STATUS_WRONG_PIN_CODE | CA_STATUS_LOCKED_PIN_CODE |
*		CA_STATUS_NO_PIN_CODE | CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | 
*		CA_STATUS_PRESELECTION_AREA_FULL | CA_STATUS_ERROR |
*		CA_STATUS_PROG_ALREADY_STORED_IN_PRESELECTION_AREA |
*		CA_STATUS_PROG_NOT_FOUND_IN_PRESELECTION_AREA | CA_STATUS_PRESELECTION_MODE_NOT_ACCEPTED
*		CA_STATUS_TERMINAL_LOCKED | CA_STATUS_INSUFFICIENT_MEMORY
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_AddSubPreselection(DWORD dwSoid, DWORD dwPinCode, CaPreselectionType_E eReferenceType, CaPreselectionInfo_S* uInfo);

/**************************************************************************
*Function Name:CS_CA_MMI_DeleteSubPreselection
* Delete preselection information
*
*Parameters 
*	dwSoid[in]:
*            SOID
*   dwPinCode[in]:
*           PIN code
*   eReferenceType[in]:
*           Type of preselection
*   uInfo[in]:
*           Preselection information
*Return Values: 
*	0 : SUCCESS 参数正确,其执行结果将通过消息上报给应用
*		CA_STATUS_OK | CA_STATUS_BAD_SOID | CA_STATUS_OPERATION_NOT_ACCEPTED |
*		CA_STATUS_WRONG_PIN_CODE | CA_STATUS_LOCKED_PIN_CODE |
*		CA_STATUS_NO_PIN_CODE | CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR | 
*		CA_STATUS_PRESELECTION_AREA_FULL | CA_STATUS_ERROR |
*		CA_STATUS_PROG_ALREADY_STORED_IN_PRESELECTION_AREA |
*		CA_STATUS_PROG_NOT_FOUND_IN_PRESELECTION_AREA | CA_STATUS_PRESELECTION_MODE_NOT_ACCEPTED
*		CA_STATUS_TERMINAL_LOCKED | CA_STATUS_INSUFFICIENT_MEMORY
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_DeleteSubPreselection(DWORD dwSoid, DWORD dwPinCode, CaPreselectionType_E eReferenceType, CaPreselectionInfo_S* uInfo);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCardParingMode
* Get smartcard paring mode
*
*Parameters 
            no
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_CARD_PARING_MODE;否则上抛 
*		CA_STATUS_ERROR | CA_STATUS_MODE_NOT_SETUP
*	-1: FAILURE 失败
**************************************************************************/
INT CS_CA_MMI_RequestCardParingMode(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCardParingNum
* Get number of the paring smartcard
*
*Parameters 
            no
*Return Values:
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PARING_CARD_NUM;否则上抛 
*		CA_STATUS_ERROR | CA_STATUS_MODE_NOT_SETUP
*	-1: FAILURE 失败
**************************************************************************/
INT CS_CA_MMI_RequestCardParingNum(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestParingCardList
* Get the paring card list
*
*Parameters 
            no
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PARING_CARD_LIST;否则上抛 
*		CA_STATUS_ERROR | CA_STATUS_MODE_NOT_SETUP
*	-1: FAILURE 失败
**************************************************************************/
INT CS_CA_MMI_RequestParingCardList(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCamLockValue
* Get the CAMLOCK value
*
*Parameters 
            no
*Return Values: 
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_CAMLOCK_VALUE;否则上抛 
*		CA_STATUS_ERROR | CA_STATUS_MODE_NOT_SETUP
*	-1: FAILURE 失败
**************************************************************************/
INT CS_CA_MMI_RequestCamLockValue(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestSTBId
* Get the STB Id
*
*Parameters 
            no
*Return Values:
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_STB_ID;否则上抛 
*		CA_STATUS_ERROR | CA_STATUS_NO_STB_ID
*	-1: FAILURE 失败
**************************************************************************/
INT CS_CA_MMI_RequestSTBId(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestTerminalId
* Get the Terminal Id
*
*Parameters 
            no
*Return Values:
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_TERMINAL_ID;否则上抛 
*		CA_STATUS_ERROR 
*	-1: FAILURE 失败
**************************************************************************/
INT CS_CA_MMI_RequestTerminalId(void);

/**************************************************************************
* Function Name: CS_CA_GetSmcStatus
*  获取卡状态,应用进入菜单时调用
*
*Parameters 空
*Return Values
*       返回卡状态对应的CaAlarm_E枚举值
**************************************************************************/
INT CS_CA_GetSmcStatus();

/**************************************************************************
*Function Name:CS_CA_MMI_GetLeftAttemptsForPincode
* Get the left number of attempt for inputting pincode
*
*Parameters 
            no
*Return Values:
*	0 : SUCCESS 参数正确,其请求结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_UNLIMITED_ATTEMPTSFORPINCODE或CA_INFO_ATTEMPTSFORPINCODE;
*		否则上抛 CA_STATUS_NO_PIN_CODE | CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR 
*		| CA_STATUS_ERROR 
*	-1: FAILURE 失败
**************************************************************************/
INT CS_CA_MMI_GetLeftAttemptsForPincode(void);

/**************************************************************************
*Function Name:CS_CA_MMI_GetPreviewProgress
* Get the progress information of a preview program
*
*Parameters 
*   dwSoid[in]: Service Operator Identifier.
*   ucPreviewIndex[in]: Specifies the index (or channel number) of the preview
*                       contained in the stream.
*   wPreviewNumber[in]: Specifies the preview number of the preview contained
*                       in the stream.
*Return Values: 
*	0 : SUCCESS 参数正确,其结果将通过消息上报给应用
*		请求成功，将上抛CA_INFO_PREVIEWPROGRESS;否则上抛 CA_STATUS_CONSULTATION_LOCKED
*		| CA_STATUS_BAD_HANDLE | CA_STATUS_NO_CARD | CA_STATUS_CARD_ERROR |
*		CA_STATUS_INFO_NOT_AVAILABLE | CA_STATUS_ERROR
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_GetPreviewProgress(DWORD dwSoid, BYTE ucPreviewIndex, WORD wPreviewNumber);

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
* Function Name: CSCAGetSerialNumber
*  获取序列号,VIACCESS的序列号格式为12byte的BCD码
*
*Parameters 
*	pData: 12BYTE BCD码存储空间地址
*Return Values:成功SUCCESS
*           失败FAILURE
**************************************************************************/
extern INT CSCAGetSerialNumber(BYTE* pData);

/**************************************************************************
* Function Name: CS_CA_GetStbInformation
*  获取机顶盒信息
*
*Parameters 
*  pstCaStbInfo 机顶盒信息结构体.
*Return Values:成功SUCCESS
*           失败FAILURE
**************************************************************************/
extern INT CS_CA_GetStbInformation(CaStbInformation_S* pstCaStbInfo);

/**************************************************************************
* Function Name: CS_CA_GetCAMLockLevel
*  获取CAMLock等级
*
*Parameters 
*  peCAMLockLevel:存放CAMLockLevel的地址(CAMLockLevel是项目立项时，与viaccess确定)
*Return Values:成功SUCCESS
*           失败FAILURE
**************************************************************************/
extern INT CS_CA_GetCAMLockLevel(CaCAMLockLevel_E* peCAMLockLevel);

/*************************************************************************
*Function Name:CS_CA_GetProtectionCapabilities
* 获取机顶盒支持的保护功能的类型
*Parameters:
*    pOutputProtection[in/out]:
                    支持保护功能的数字输出设备类型，定义了低8位，如果支持某一种保护，将相应的bit设置为1，否则
                    设置成0
                    b0:SPDIF (Digital Audio out)
                    b1:Ethernet
                    b2:USB
                    b3:I-Link (FireWire - IEEE1394)
                    b4:DVB-CI
                    b5:HDD
                    b6:DVD
                    b7:CI+
                    other:set 0
*	pCopyProtection[in/out]:
					支持的copy protection的类型，定义了低4位
					b0:Macrovision?
					b1:Fingerprinting
					b2:CGMS-A
					b3:DTCP-IP
					other:set 0
*Return Values:成功SUCCESS
*				失败FAILURE
**************************************************************************/
extern INT CS_CA_GetProtectionCapabilities(WORD *pOutputProtection, WORD *pCopyProtection);

/**************************************************************************
*  下面结构为兼容老结构，新项目开发请不要使用
**************************************************************************/
typedef CaSatus_E					E_CaSatus;
typedef CaPreselectionType_E        T_PreselectionType;
typedef CaKeyRequest_S		  	    T_KeyRequest;
typedef CaCardParingMode_E 			T__CardParingMode;
typedef CaCardNumForParing_S		T__CardNumForParing;
typedef CaParingCardList_S		    T__ParingCardList;
typedef CaPreviewProgramInfo_S      PreviewProgramInfo_S;
typedef CaPrice_S      				T_Price;
typedef CaPpvPrice_S                T_PpvPrice;
typedef CaPpvSrvID_S  				T_PpvSrvID;
typedef CaPaymentMode_E             T_PaymentMode;
typedef CaPpvRequest_S              T_PpvRequest;
typedef CaEncodeFormat_E            T_EncodeFormat;
typedef CaMsg_S        				T_Msg;
typedef CaSoidList_S 				T_SoidList;
typedef CaAttemptForPincode_S       T_AttemptForPincode;
typedef CaDate_S  					T_Date;
typedef CaThemeSubList_S            T_ThemeSubList;
typedef CaSubListTheme_S            T_SubListTheme;
typedef CaClassSubList_S            T_ClassSubList;
typedef CaSubListClass_S            T_SubListClass;
typedef CaPrebookedRange_S          T_PrebookedRange;
typedef CaPrebookedList_S           T_PrebookedList;
typedef CaPreselectionPrice_S       T_PreselectionPrice;
typedef CaThemeLevel_S              T_Thle;
typedef CaLink_S                    T_Link;
typedef CaIppvP_S                   T_IppvP;
typedef CaIppvT_S                   T_IppvT;
typedef CaPreselectionInfo_S        T_InfoPreselection;
typedef CaPreselection_S            T_Preselection;
typedef CaPreSelectionList_S        T_PreSelectionList;
typedef CaSoidLabel_S               T_SoidLabel;
typedef CaSmcID_S 					T_SmcID;
typedef CaPgLevel_S                 T_PgLevel;
typedef CaPreviewProgress_S         T_PreviewProgress;
typedef CaPpvRecordValue_S          T_PpvRecordValue;
typedef CaIppvpListValue_S          T_IppvpListValue;
typedef CaIppvtListValue_S          T_IppvtListValue;
typedef CaCreditValue_S             T_CreditValue;
typedef CaThreshold_S               T_Threshold;
typedef CaVersion_S 				T_Version;
typedef CaDateTime_S                T_DateTime;
typedef CaSTBId_S 					T_STBId;
typedef CaCamlockDynamicState_E     T_CamlockDynamicState;
typedef CaCamlock_S   				T_CAMLOCK;
typedef CaTerminalId_S              T_TerminalId;
typedef CaVersionDetail_S           T_VersionDetail;
typedef CaMail_S 					T_Txt;
typedef CaDigitalOutputProt_S       T_DigitalOutputProt;

#ifdef  __cplusplus
}
#endif

#endif //_CS_VIACCESSCA_INTERFACE_H_


