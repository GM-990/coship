/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd.,
*              All rights reserved.
* Filename:    CS_NAGRA7_interface.h
* Author:      zhangxing
* Date:        2011 12
* Description: ����nagracak7�汾
*******************************************************************************/
#include "udiplus_typedef.h"

/*******************************************************************************
* 			                Revision History                               						     *
********************************************************************************
* - Revision 1.0  2008/6
*   Programmer:Xie jianghua
*   Create.
*******************************************************************************/
#ifndef _CS_NAGRA7_INTERFACE_H_
#define _CS_NAGRA7_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define CA_MAX_ES_NUM				(8)
#define MAX_PROFILE_NUM				(4)
#define MAX_PERIOD_NUM				(5)
#define MAX_DIS_MSG_NUM				(8)
#define MAX_DIS_MSG_LEN				(32)
#define MAX_SCNUM_LEN 				(64)

typedef enum _TXTTYPE_ST
{
    CA_TXT_MAIL             = 0,//Text -- Mailbox,  �ʼ���Ϣ
    CA_TXT_ANNOUNCE         = 1,//Text -- Announcement,�㲥֪ͨ
    CA_TXT_CLUB             = 2 //Club number,��Ա��
}NAGRA_TXTTYPE_ST;

typedef struct _CADATETIME_ST
{
    DWORD   m_dwYear;
    BYTE    m_cMonty;
    BYTE    m_cDay;
    BYTE    m_cHour;
    BYTE    m_cMinute;
}NAGRA_CADATETIME_ST;

#define CA_MAX_SIZE_TXT_MAIL       	(3*1024)

typedef struct _CAMSG_ST        // �ʼ�����
{
    BYTE        		m_cType;							//T_TxtType// 0: Text_Mail 1: Text_Announcement 
    BYTE        		m_cClass;               			//T_TxtShowModel// 0: Normal 1: Time 2: ForcedDisplay
    BYTE        		m_cPriority;
    DWORD       		m_dwLen;
    NAGRA_CADATETIME_ST	m_stDeleteTime;   					//��TIMED�ʼ��д���
    BYTE    			m_szContent[CA_MAX_SIZE_TXT_MAIL];
    WORD    			m_wDuration;						//for TMS mail
    BYTE    			m_bCoverPer; 						//for TMS mail
}NAGRA_CAMSG_ST;

typedef struct _CAMAIL_ST       //�ʼ��ṹ��
{   
    BOOL    			m_bReadFlag;	//�Ѷ���־
    BOOL    			m_bLockFlag;	//�ʼ�������־
    NAGRA_CADATETIME_ST	m_stArriveTime; //�ʼ������ʱ��
    NAGRA_CAMSG_ST 		m_stMsg;
}NAGRA_CAMAIL_ST;

typedef enum _CaMessage
{
	CA_ALARM,// 0
	CA_COMMAND,// 1
	CA_INFO	// 2
}E_CaMessage;

typedef enum _CaAlarm
{
	CA_ALARM_CANCEL = 0,             //00 ȡ����ǰ��Ļ��ʾ
	CA_ALARM_CARD_REMOVED,           //01 �����γ�
	CA_ALARM_CARD_ERROR,             //02 ���ܿ�����
	CA_ALARM_CARD_MUTE,              //03 ���Ѳ��룬���޷�ͨѶ�������ܲ巴��
	CA_ALARM_CARD_INVALID,           //04 ����Ч����Nagravision��
	CA_ALARM_CARD_BLACKLISTED,       //05 �������������
	CA_ALARM_CARD_SUSPENDED,         //06 ��ʱȡ��������Ȩ
	CA_ALARM_CARD_NEVER_PAIRED,      //07 ���������δ���
	CA_ALARM_CARD_NOT_PAIRED,        //08 ���뵱ǰ�����в����
	CA_ALARM_CARD_EXPIRED,           //09 ������
	CA_ALARM_CARD_NOT_CERTIFIED,     //10 δ�����Ŀ�
	CA_ALARM_NO_ACCESS,              //11 ûȨ�޹ۿ��ý�Ŀ
	CA_ALARM_RIGHT_SUSPENDED,        //12 ��Ȩ��ȡ��
	CA_ALARM_BLACK_OUT,              //13 ��Ŀ�������
	CA_ALARM_OTHER,                  //14 �������Ž�Ŀ
	CA_ALARM_ENTITLE_WILL_EXPIRE_RENEWABLE,		 //15 ��Ȩ��Ҫ���ڲ����Իָ���֧��profile���ܵ���Ŀ��
	CA_ALARM_ENTITLE_EXPIRED_RENEWABLE,	         //16 ��Ȩ�����˲����Իָ���֧��profile���ܵ���Ŀ��
	CA_ALARM_ENTITLE_WILL_EXPIRE_NOTRENEWABLE,   //17 ��Ȩ��Ҫ���ڲ������Իָ���֧��profile���ܵ���Ŀ��
	CA_ALARM_ENTITLE_EXPIRED_NOTRENEWABLE,       //18 ��Ȩ�����˲������Իָ���֧��profile���ܵ���Ŀ��
	CA_ALARM_ENTITLE_EXPIRE_TODAY_RENEWABLE,     //19 ��Ȩ������ڲ����Իָ���֧��profile���ܵ���Ŀ��
	CA_ALARM_ENTITLE_EXPIRE_TODAY_NOTRENEWABLE,	 //20 ��Ȩ������ڲ������Իָ���֧��profile���ܵ���Ŀ��
	CA_ALARM_ENTITLE_SUCCESSFULLY_PROLONGED,     //21 ��Ȩ�ѳɹ��ӳ�
	CA_ALARM_CHIPSET_PAIRING_REQUIRED,           //22 ��ĿҪ��оƬ�����
	CA_ALARM_STRONG_PAIRING_REQUIRED, //23��Ŀ��Ҫ���
	CA_ALARM_MAX
}E_CaAlarm;

typedef enum _CaCommand
{
    IRD_CMD_RESET_PIN_CODE = 0, 					//��ʾ�û�PIN�뱻����Ϊ0000,��ѡ
    IRD_CMD_MAIL, 									//���ʼ����������ʾ���ʼ�,��ѡ
    IRD_CMD_FORCE_IDENTIFICATION_STANDARD,			//��ʾ����ָ��,��ѡ
    IRD_CMD_FORCE_IDENTIFICATION_ZONEID,			//NOT USED YET
    IRD_CMD_SET_MACROVISION_CPS,					//��ʾMacrovisionCps,��ѡ
    IRD_CMD_ENABLE_VIDEO,							//5 NOT USED YET
    IRD_CMD_ENABLE_AUDIO,							//NOT USED YET
    IRD_CMD_ENABLE_SMARTCARD1,						//NOT USED YET
    IRD_CMD_ENABLE_SMARTCARD2,						//NOT USED YET
    IRD_CMD_ENABLE_HARDDISC,						//NOT USED YET
    IRD_CMD_ENABLE_DVD,								//10 NOT USED YET
    IRD_CMD_ENABLE_SERIALPORT1,						//NOT USED YET
    IRD_CMD_ENABLE_SERIALPORT2,						//NOT USED YET
    IRD_CMD_ENABLE_PARALLELPORT,					//NOT USED YET
    IRD_CMD_ENABLE_USBPORT,							//NOT USED YET
    IRD_CMD_ENABLE_1394PORT,						//15 NOT USED YET
    IRD_CMD_ENABLE_SPAREPORT1,						//NOT USED YET
    IRD_CMD_ENABLE_SPAREPORT2,						//NOT USED YET
    IRD_CMD_ENABLE_PERIPHERAL1,						//NOT USED YET
    IRD_CMD_ENABLE_PERIPHERAL2,						//NOT USED YET
    IRD_CMD_SET_NETWORK_ID,							//20 ����Networkid,��ѡ
    IRD_CMD_MASTER_SLAVE_INIT,						//NOT USED YET
    IRD_CMD_MASTER_SLAVE_CANCELLATION,				//NOT USED YET
    IRD_CMD_MASTER_SLAVE_SINGLE_SHOT,				//NOT USED YET
    IRD_CMD_MASTER_SLAVE_AUTOMATIC,					//NOT USED YET
    IRD_CMD_SET_PIN_CODE,							//25 �޸�PIN,��ѡ
    IRD_CMD_FORCE_STANDBY_STANDARD,					//ǿ�ƴ���,��ѡ
    IRD_CMD_FORCE_STANDBY_ZONEID,					//NOT USED YET
    IRD_CMD_CONFIG_CAMLOCK,							//NOT USED YET
    IRD_CMD_COPY_PROTECTION_VALIDATE,				//NOT USED YET
    IRD_CMD_COPY_PROTECTION_REVOKE,					//30 NOT USED YET
    IRD_CMD_COPY_PROTECTION_FORCE_AUTHENTICATION,	//NOT USED YET
    IRD_CMD_COPY_PROTECTION_SET_KEY_SESSION_PERIOD,	//NOT USED YET
    IRD_CMD_RESTORE_FACTORY_SETTING,				//�ָ���������,��ѡ
    IRD_CMD_FORCE_TUNE,								//Tune_t,ǿ���л�Ƶ��,��ѡ
    IRD_CMD_FORCE_TUNE_WITH_TIMEOUT,				//35 Tune_t,ǿ���л�Ƶ������ʱʱ��,��ѡ 
    IRD_CMD_FORCE_TUNE_ZONE,						//NOT USED YET
    IRD_CMD_FORCE_TUNE_WITH_TIMEOUT_IPTV,			//NOT USED YET
    IRD_CMD_FORCE_TUNE_ZONE_IPTV,					//NOT USED YET
    IRD_CMD_POP_UP,									//PopUp_t,��ʾpopup,ǿ��
    IRD_CMD_POP_UP_REMOVE,							//40 ���popup,ǿ��
    IRD_CMD_MOVIE_KEY,								//NOT USED YET
    IRD_CMD_PUSH_VOD_CONTENT_CONFIG,				//NOT USED YET
    IRD_CMD_PUSH_VOD_PARTITION_FORMATTING,			//NOT USED YET
    IRD_CMD_PUSH_VOD_ERASE_ASSET,					//NOT USED YET
    IRD_CMD_PUSH_VOD_EARSE_METADATA_FILE,			//45 NOT USED YET
    IRD_CMD_PUSH_VOD_SET_DOWNLOADS_WAKEUP,			//NOT USED YET
    IRD_CMD_FORCE_SOFTWARE_DWONLOAD_STANDARD,		//ǿ��OTA����,��ѡ
    IRD_CMD_FORCE_SOFTWARE_DWONLOAD_BY_ID,			//ǿ��OTA����ͨ��ID,��ѡ
    IRD_CMD_CHANGE_USAGE_ID_RESIDENT_SOFTWARE,		//NOT USED YET
    IRD_CMD_CHANGE_USAGE_ID_DOWNLOADABLE_APPS,		//50 NOT USED YET
    IRD_CMD_SET_COMMUNITY_TYPE,						//NOT USED YET
    IRD_CMD_FORMAT_LOGICAL_DISK,					//NOT USED YET
    IRD_CMD_USAGE_MONITORING,						//NOT USED YET
    IRD_CMD_BROADCAST_NETWORK_OPERATOR_LOCK,		//NOT USED YET
    IRD_CMD_SET_ZONE_ID,							//55,֪ͨ������zoneid,��ѡ 
    IRD_CMD_CANCEL_ID,								//֪ͨ��ȡ������zoneid,��ѡ 
    IRD_CMD_EMM_WAKE_UP_DISABLING_FEATURE,			//NOT USED YET
    IRD_CMD_EMM_WAKE_UP_SETTING_PARAMETERS,			//NOT USED YET
    IRD_CMD_UPDATE_COHABITATION_TABLES,				//NOT USED YET
    IRD_CMD_REBOOT_STB,								//60,����������,��ѡ
    IRD_CMD_BGA_DISABLE,							//NOT USED YET
    IRD_CMD_BGA_ENABLE,								//NOT USED YET
    IRD_CMD_SET_PAIRING_CONFIG,						//Pairing_t,���������Ϣ,��ѡ
	IRD_CMD_DISPLAYPROFILE,							//֪ͨ������profile,��ѡ 
	IRD_CMD_DISPLAYMESSAGE,							//65 ֪ͨ������displaymessage,��ѡ 
	IRD_CMD_FINGERPRINT,							//FingerPrint_S,��ѡ
	IRD_CMD_SET_COUNTRYCODE,						//DWORD,���ù�����,��ѡ
    IRD_CMD_NUM
}E_CaCommand;

typedef enum _CaInfo
{
	CA_INFO_SYSTEM_INFORMATION,			//������(DWORD)&T_CaSystemInfo
	CA_INFO_SMARTCARD_INFOMAITON,		//������(DWORD)&T_CaSmartCardInfo
	CA_INFO_SMARTCARD_DELTATIME,		//NOT USED YET
	CA_INFO_PINCODE_STATUS,				//������(DWORD)&E_CaPinCodeStatus����PINCODE�������Ŀ
	CA_INFO_METADATA					//������(DWORD)&T_CaMetadataInfo����Metadata�������Ŀ
}E_CaInfo;

typedef struct Mail_s
{
    WORD    wMailId;         //�ʼ�ID
    BYTE    ucTotalSegment;  //�ʼ��ɼ���section���--Ӧ�ò��ù���
    BYTE    ucPriority;      //���ȼ�---��֪������ʲô
    BYTE    ucSegmentNum;    //���ǵڼ���section------Ӧ�ò��ù���
    PBYTE   pMailPayload;    //����ͷָ��
    UINT   	unMailSize;      //���ݵ��ֽ���
}Mail_t;

typedef struct PopUp_s
{
    WORD    wPopUpId;
    BYTE    ucTotalSegment;
    BYTE    ucPersistence;
    BYTE    ucSegmentNum;
    PBYTE   pPopUpPayload;
    UINT    unPopUpSize;
}PopUp_t;

typedef struct NetworkOperator_s
{
    UINT    unListNum;
    USHORT* pOperatorList;
    BOOL    bActive;
}NetworkOperator_t;

typedef struct MacrovisionCps_s
{
    PBYTE   pCpsPayload;
    INT     unCpsSize;
}MacrovisionCps_t;

typedef struct NetWorkId_s
{
    WORD     wNetWorkId;
    WORD     wOriginalNetWorkId;
}NetWorkId_t;

typedef struct PinCode_s
{
    PBYTE   pPinCode;
    BYTE    ucPinLen;
}PinCode_t;

typedef struct Tune_s
{
    WORD    wNetworkId;
    WORD    wTransportId;
    WORD    wServiceId;
    WORD    wTimeOut;
}Tune_t;

typedef struct CaCnRang_s
{
    WORD    wCaCnMin;
    WORD    wCaCnMax;
}CaCnRange_t;

typedef struct Pairing_s
{
    BOOL          	bStrongPairingRequired;
    BYTE        	ucCaCnNum;
    PBYTE       	pucCaCnList;
    BYTE        	ucCaCnRangeNum;
    CaCnRange_t*	pstCaCnRangeList;
}Pairing_t;

typedef struct Period_s
{
	BYTE ucBeginOfPeriod;
	BYTE ucEndOfPeriod;
	BYTE ucFrequency;	
}Period_t;

typedef struct Profile_s
{
	BYTE 		ucProfileId;
	BYTE 		ucPeriodNum;
	Period_t 	stPeriodList[MAX_PERIOD_NUM];
	TDATE 		stLastDisplayDate;
	BYTE 		ucDisplayFlag;
	BYTE 		aucPeriod[2];
}Profile_t;

typedef struct DisplayProfile_s
{
	BYTE 		ucProfileNum;
	Profile_t 	stProfileList[MAX_PROFILE_NUM];
	WORD 		wCheckFlag;
}DisPlayProfile_t;

typedef struct Message_s
{
	BYTE ucMessageType;
	BYTE ucMessageLen;
	BYTE ucMessageContent[MAX_DIS_MSG_LEN];
}Message_t;

typedef struct DisplayMessage_s
{
	BYTE 		ucMessageNum;
	Message_t 	stMessageList[MAX_DIS_MSG_NUM];
	WORD 		wCheckFlag;
}DisplayMessage_t;

typedef struct EmmWakeup_s
{
	BOOL 	bDisableWakeup;		//ΪTRUE,��ر�EmmWakeup����,Ĭ��ΪFALSE,����EmmWakeup����,���²�������Ч
    WORD    wNetworkId;
    WORD    wTransportId;
    WORD    wServiceId;
	WORD	wWakeDuring;		//wake-up during,��λ����
	WORD	wSleepDuring;		//sleep during,  ��λ����
	WORD 	wCheckFlag;			//CA�ڲ���־,Ӧ�ò��ع���
}EmmWakeup_t;

#define CA_PROFILE_FLASH_SIZE		sizeof(DisPlayProfile_t) 
#define CA_MESSAGE_FLASH_SIZE		sizeof(DisplayMessage_t)
#define CA_EMM_WAKEUP_FLASH_SIZE	sizeof(EmmWakeup_t)
#define CA_SCNUM_FLASH_SIZE			MAX_SCNUM_LEN
#define CA_PROLONG_FLASH_SIZE 		sizeof(TDATE)

#define MAX_CHIPNUID_LEN 			64
#define MAX_CHIPTYPE_LEN 			20
#define MAX_CHIPREV_LEN  			20

typedef struct _CaSystemInfo
{
	BYTE 	m_acCAKVersion[64];
	BYTE 	m_acIrdSerialNumber[64];
	BYTE 	m_acProjectInfo[64];
	BYTE 	m_acChipNuid[MAX_CHIPNUID_LEN];     //how to show
	BYTE 	m_acChipType[MAX_CHIPTYPE_LEN];
	BYTE 	m_acChipRevision[MAX_CHIPREV_LEN];
	INT  	m_nCscMaxIndex;	//used for csc download
}T_CaSystemInfo;

typedef enum _CaFlashType
{
	NAGRA_FLASH,				//for nagra lib
	PAIRING_DATA_FLASH,			//for nagra pairing data
	CA_FLASH,					//for ca email and ird command
	ENTITLE_FLASH,				//for entitlement profile
	EMM_WAKEUP_FLASH,			//for emm wake-up
	PROLONG_FLASH,				//for 136ENTITLEMENT Msg
	CSC_DATA_FLASH				//for CSC Data
}E_CaFlashType;

typedef enum _CaPinIndex
{
  	PIN_INDEX_0 = 0,   //YQL@2012.9.21 ��Ӧnagra��pinidҲ��0--3       
  	PIN_INDEX_1 = 1,
  	PIN_INDEX_2,
  	PIN_INDEX_3,
  	PIN_INDEX_4,    //YQL@2012.9.21���Է������index����Ч��
  	PIN_INDEX_LAST	//Last type. To be used only for consistency checks.
}E_CaPinIndex;

typedef enum _CaPinCodeStatus
{
	PIN_CODE_PIN_ACCEPTED,		//PIN������ȷ���޸�PIN��ɹ�
	PIN_CODE_PIN_REJECTED,		//PIN�������������������޸�PIN��ʧ��
	PIN_CODE_PIN_SUSPENDED,		//PIN�����
	PIN_CODE_PIN_INDEX_INVALID,	//PIN��������Ч
	PIN_CODE_NO_VALID_SMARTCARD,//δ�忨
	PIN_CODE_NO_MANAGE,			//smart card is not able to manage PIN codes
	PIN_CODE_PIN_BLOCK,			//PIN code is blocked
	PIN_CODE_ERROR				//��������
}E_CaPinCodeStatus;

typedef struct _CaSmartCardInfo
{
	BYTE	m_cSCState;
	BYTE	m_acSCVersion[64];
	BYTE	m_acSCNumber[64];
	WORD	m_wECMCASystemID;	//only used for csc download on HDPlus project
	BYTE	m_acZipCode[64];	//NOT USED YET
	INT		m_nZipCodeSize;		//NOT USED YET
}T_CaSmartCardInfo;

typedef struct _Metadata
{
	BYTE		m_acMetadata[3];
	HCSHANDLE	m_hService;
}Metadata_t;

typedef struct _CaMetadataInfo
{
	INT			m_nMetadataCount;
	Metadata_t	m_stMetadata[CA_MAX_ES_NUM];
}T_CaMetadataInfo;

#define ZONE_ID_LEN 			(6)
typedef struct _FingerPrint
{
	WORD m_wNetworkID;
	WORD m_wTransportID;
	WORD m_wServiceID;					//�����߶�Ϊ0ʱ���������տ��Ľ�Ŀ����ʾ
	WORD m_wDisplayTime;				//ÿһ����ʾ��ʱ�䣬��λms
	BYTE m_ucLocationX;					//percentage (0-100)
	BYTE m_ucLocationY;					//percentage (0-100),�����߶�Ϊ0ʱλ�����
	BYTE m_ucForegroundcolor;			//������ɫ��0: black��1: white��2: red��3: blue��4: green��5: yellow��6: future use��7: transparent (100%)
	BYTE m_ucBackgroundcolor;			//������ɫ��0: black��1: white��2: red��3: blue��4: green��5: yellow��6: future use��7: transparent (100%)
	BOOL m_ucShowTime;					//�Ƿ���ʾ��ǰʱ�䣬0����ʾ��1��ʾ
	BOOL m_ucShowSTBSN;					//�Ƿ���ʾ���������кţ�0����ʾ��1��ʾ
	BYTE m_ucLogo;						//0: no logo��1: logo 1 shown��2: logo 2 shown��3: logo 3 shown
	BYTE m_ucRepetTime;					//�ظ���ʾ�Ĵ���,0ʱ��ʾ1��
	WORD m_wRepetInterval;				//ÿ������ʾ֮���ʱ��������λs
	BYTE m_aucZipCodeInit[ZONE_ID_LEN];	//��ʼZipCodeֵ
	BYTE m_aucZipCodeFinal[ZONE_ID_LEN];//��ֹZipCodeֵ��ZipCodeֵ�ڸ÷�Χ�ڵĻ�������ʾ��ָ��
}FingerPrint_S;

/**************************************************************************
* Function Name: CS_CA_RequestSystemInfo
*   ��ȡCA  System Infomation. 
*   �첽��������CA_INFO_SYSTEM_INFORMATION�ϱ�
*
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_RequestSystemInfo(void);

/**************************************************************************
* Function Name: CS_CA_RequestSmartCardInfo
*   ��ȡCA  SmartCard Infomation. 
*   �첽��������CA_INFO_SMARTCARD_INFORMATION�ϱ�
*
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_RequestSmartCardInfo(void);

/**************************************************************************
*Function Name: CS_CA_TXT_Add
*���һ�����ʼ�.
*
*Parameters 
*	pstTxt[in]:
*		�ʼ��ṹ��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Add(NAGRA_CAMAIL_ST* pstTxt);

/**************************************************************************
*Function Name: CS_CA_TXT_Del
*ɾ��һ���ʼ�.
*
*Parameters 
*	dwIndex[in]:
*		�ʼ�������
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Del(DWORD dwIndex);

/**************************************************************************
*Function Name: CS_CA_TXT_DelAll
*ɾ�������ʼ�.
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_DelAll(void);

/**************************************************************************
*Function Name: CS_CA_TXT_GetNum
*ɾ�������ʼ�.
*
*Return Values: 
*	 �ʼ�����
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name: CS_CA_TXT_GetInfo
*��ȡ�ʼ���ϸ��Ϣ.
*
*Parameters 
*	dwIndex[in]:
*		�ʼ�������
*	ppstTxt[out]:
*		�ʼ���Ϣ
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, NAGRA_CAMAIL_ST** ppstTxt);

/**************************************************************************
*Function Name: CS_CA_TXT_HaveNew
*�Ƿ���ĳ���͵����ʼ�
*
*Parameters 
*	eType[in]:
*		�ʼ�����
*Return Values: 
*	TRUE : �����ʼ�
*	FALSE: û�����ʼ�
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(NAGRA_TXTTYPE_ST eType);

/**************************************************************************
*Function Name: CS_CA_TXT_SetReadFlag
*�����ʼ��Ѷ���־
*
*Parameters 
*	dwIndex[in]:
*		�ʼ�������
*	bRead[in]:
*		�Ѷ�/δ��
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name: CS_CA_TXT_Save
*�����ʼ���Flash��
*
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name: CS_CA_TXT_EraserAll
*�������д���FLASH�е��ʼ�
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_EraserAll(void);

/**************************************************************************
*Function Name: CS_CA_TXT_HaveNewMailNum
*��ȡ���ʼ�����.
*
*Return Values: 
*	δ�� �ʼ�����
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
* Function Name: CSGetCAFlashAddress
*   ��ȡCA  ����FLASH �ռ��ַ. 
*
*Parameters 
*	FlashType[in]:
*		Flash��������,E_CaFlashType
*	pFlashMailAddr[in]:
*		��ʼ��ַ
*	pFlashMailSize[in]:
*		�����С
*Return Values
**************************************************************************/
extern void CSGetCAFlashAddress(WORD FlashType, DWORD* pFlashMailAddr, DWORD* pFlashMailSize);

/* -- example --
void CSGetCAFlashAddress(WORD FlashType, DWORD* pFlashMailAddr, DWORD* pFlashMailSize)
{
	switch(FlashType)
	{
		case NAGRA_FLASH:
			*pFlashMailAddr = NAGRA_CA_FLASH_ADDRESS;	  	//one separate block
			*pFlashMailSize = 64*1024;						// should be large than 48Kbyte
			break;
		case PAIRING_DATA_FLASH:
			*pFlashMailAddr = PAIRING_DATA_FLASH_ADDRESS;  	//OTP area or chipset inner ram
			*pFlashMailSize = 602; 							// should be large than 602 byte
			break;
		case CA_FLASH:
			*pFlashMailAddr = CA_FLASH_ADDRESS; 			//one separate block
			*pFlashMailSize = 64*1024; 				  		// should be large than 64K byte
			break;
		//����3�����͵�FLASH�����������һ�����ڣ�����Ҫ����һ�������BLOCK���仮��Ϊ:
		//ENTITLE_FLASH(����ΪCA_PROFILE_FLASH_SIZE+CA_MESSAGE_FLASH_SIZE):������HDPlus����Ŀ
		//EMM_WAKEUP_FLASH(����ΪCA_EMM_WAKEUP_FLASH_SIZE):���������������Ŀ
		//PROLONG_FLASH(����ΪCA_SCNUM_FLASH_SIZE+CA_PROLONG_FLASH_SIZE):������HDPlus����Ŀ
		//����ĸ������ڣ������Ӧ��*pFlashMailAddr = 0;*pFlashMailSize = 0;
		case ENTITLE_FLASH:
			*pFlashMailAddr = CA_DATA_NVRAM + 128*KBYTE; 	//one separate block
			*pFlashMailSize = CA_PROFILE_FLASH_SIZE + CA_MESSAGE_FLASH_SIZE;
			break;
		case EMM_WAKEUP_FLASH:
			*pFlashMailAddr = CA_DATA_NVRAM + 128*KBYTE + CA_PROFILE_FLASH_SIZE + CA_MESSAGE_FLASH_SIZE;
			*pFlashMailSize = CA_EMM_WAKEUP_FLASH_SIZE;
			break;
		case PROLONG_FLASH:
			*pFlashMailAddr = CA_DATA_NVRAM + 128*KBYTE + CA_PROFILE_FLASH_SIZE + CA_MESSAGE_FLASH_SIZE + CA_EMM_WAKEUP_FLASH_SIZE;
			*pFlashMailSize = CA_SCNUM_FLASH_SIZE + CA_PROLONG_FLASH_SIZE;
			break;
		//CSCDATA���͵�FLASH����Flash��ȡ����CSCData�ɹ�����CRCУ��ͨ�����Ŵ���CA��Ч��ַ�ͳ���
		case CSC_DATA_FLASH:
			{
				if(CS_CSCData_CRC16(readcrcdata()))//��Flash��ȡ����CSCData�ɹ�����CRCУ��ͨ��
				{
					*pFlashMailAddr = dwCscDataAddr;	//��ʵ�ʵ�CSCData��ʼ��ַ����CA
					*pFlashMailSize = CSCDATA_LEN;		//��ʵ�ʵ�CSCData���ݳ��ȴ���CA
				}
				else
				{
					*pFlashMailAddr = 0;
					*pFlashMailSize = 0;
				}
			}
			break;
		default:
			*pFlashMailAddr = 0;
			*pFlashMailSize = 0;
			break;
	}
}*/


/**************************************************************************
* Function Name: CS_CA_FreezeAllEvent
*   ���������¼���Ӧ(ң������ǰ���). 
*
*Parameters 
*Return Values
**************************************************************************/
extern void CS_CA_FreezeAllEvent(void);

/**************************************************************************
* Function Name: CS_CA_UnfreezeEvent
*   ��������¼���Ӧ�Ķ���(ң������ǰ���). 
*
*Parameters 
*Return Values
**************************************************************************/
extern void CS_CA_UnfreezeEvent(void);

/**************************************************************************
* Function Name: CS_CA_ShowPopUp
*   ��ʾPopup��Ϣ.
*
*Parameters 
*	ucLanguage[in]:
*		��������
*	unSize[in]:
*		��ʾ���ݴ�С
*	pContent[in]:
*		��ʾ����
*Return Values
**************************************************************************/
extern void CS_CA_ShowPopUp(BYTE ucLanguage[3], UINT unSize, PBYTE pContent);

/**************************************************************************
* Function Name: CS_CA_HidePopUp
*   ����Popup��Ϣ.
*
*Parameters 
*Return Values
**************************************************************************/
extern void CS_CA_HidePopUp(void);

/**************************************************************************
* Function Name: CS_CA_GetZoneId
*   ��ȡ�����е�ZoneId
*
*Parameters	
*		pZoneId
*            [out] ZoneId�����׵�ַ
*       nLen
*            [in] ZoneId���ȣ�Ϊ6  
*
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE	ʧ��
*Notes
*		δ���ù�ZoneId����ʧ�ܶ��᷵��FAILURE��ֻ�з���SUCCESS����Ч
**************************************************************************/
INT CS_CA_GetZoneId(BYTE *pZoneId, INT nZoneIdLen);

/**************************************************************************
* Function Name: CSCASetCheckValidateIrd
*   �����Ƿ�У��IRD����
*
*Parameters	
*		bSet
*            [in] TRUE:У�飬FALSE:��У��
*
*Notes
*		Ĭ�϶�����ҪУ�飬HDPlusҪ��Profile���У�飬��Ҫ���øýӿ�
**************************************************************************/
void CSCASetCheckValidateIrd(BOOL bSet);

/**************************************************************************
* Function Name: CSCACheckPinCode
*   �������PIN���Ƿ���ȷ
*
*Parameters	
*		emPinIndex
*            [in] PIN������
*		dwPinCode
*			 [in] �����PIN�� 
*
*Return Values
*		SUCCESS	�ɹ�
*		����ֵʧ��
*Notes
*		�ú������첽�ģ������ͨ����ϢCA_INFO_PINCODE_STATUS�ϱ�
*		����emPinIndex��nagra����Ҳ�������N8611I��Ŀ��PIN_INDEX_2
**************************************************************************/
INT CSCACheckPinCode(E_CaPinIndex emPinIndex, DWORD dwPinCode);

/**************************************************************************
* Function Name: CSCAChangePinCode
*   �޸�PIN��
*
*Parameters	
*		emPinIndex
*            [in] PIN������
*		dwOldPinCode
*			 [in] �����ԭʼPIN�� 
*		dwNewPinCode
*			 [in] �������PIN�� 
*Return Values
*		SUCCESS	�ɹ�
*		����ֵʧ��
*Notes
*		�ú������첽�ģ������ͨ����ϢCA_INFO_PINCODE_STATUS�ϱ�
*		����emPinIndex��nagra����Ҳ�������N8611I��Ŀ��PIN_INDEX_2
**************************************************************************/
INT CSCAChangePinCode(E_CaPinIndex emPinIndex, DWORD dwOldPinCode, DWORD dwNewPinCode);

/**************************************************************************
* Function Name: CSCAHDPlusGetHotlineNumber
*   ��ȡHotlineNumber
*
*Parameters	
*		pucNumber
*            [out] HotlineNumber
*		nLen 
*			 [in] HotlineNumber����,>=32
*
*Notes
*		��HDPlus��Entitlement�������Ŀʹ��
**************************************************************************/
BOOL CSCAHDPlusGetHotlineNumber(BYTE *pucNumber, INT nLen);

/**************************************************************************
* Function Name: CSCAHDPlusGetCallCharge
*   ��ȡCallCharge
*
*Parameters	
*		pucCharge
*            [out] CallCharge
*		nLen 
*			 [in] CallCharge����,>=32
*
*Notes
*		��HDPlus��Entitlement�������Ŀʹ��
**************************************************************************/
BOOL CSCAHDPlusGetCallCharge(BYTE *pucCharge, INT nLen);

/**************************************************************************
* Function Name: CSCAHDPlusSaveEntitlementMessageToFlash
*   ������ʾ��EntitlementMessage��Flash
*
*Notes
*		����EntitlementMessage��ʾ�������û�ѡ�������ʾ��ʱ����Ҫ���øýӿ�
*		��HDPlus��Entitlement�������Ŀʹ��
**************************************************************************/
void CSCAHDPlusSaveEntitlementMessageToFlash(void);

/**************************************************************************
* Function Name: CSCAHDPlusGetProductExpirationDate
*   ��ȡ��Ʒ�Ĺ���ʱ��
*
*Parameters	
*		pnStatus
*            [out] -1:û�л�ȡ������ʱ��,0:�Ѿ�����,1:��Ŀδ����
*		pstExpirationDate 
*			 [out] ��Ŀδ����ʱ,pstExpirationDate����Ч
*
*Notes
*		��HDPlus��Entitlement�������Ŀʹ��
**************************************************************************/
void CSCAHDPlusGetProductExpirationDate(int *pnStatus, TDATE *pstExpirationDate);

/**************************************************************************
* Function Name: CSCAGetEmmWakeupParam
*   ��ȡEmmWakeup����
*
*Parameters	
*		pstEmmWakeupParam
*            [in] EmmWakeup�����ṹ��
*Return Values
*		TRUE	�ɹ�
*		FALSE	ʧ��
*Notes
*		�ú������������Ŀ��ʹ�ã�Ŀ�����ܹ��ڴ���������½���EMM��Ϣ
**************************************************************************/
BOOL CSCAGetEmmWakeupParam(EmmWakeup_t *pstEmmWakeupParam);

#ifdef  __cplusplus
}
#endif

#endif //_CS_NAGRA7_INTERFACE_H_

