/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_COMVENIENT_Interface.h  
* Author:      ZHENTIEJUN
* Date:        2 2008
* Description: COMVENIENT CA interface
*            
*******************************************************************************/

/*******************************************************************************
* 			                                  Revision History                                				     *
********************************************************************************
* - Revision 1.0  2008/2/26
*   Programmer:ZHENTIEJUN 
*   Create.
* - Revision 1.1 2010/12/21
*   Programmer:zhangxing
*   Refactoring.
*******************************************************************************/
#ifndef _CS_COMVENIENT_INTERFACE_H_
#define _CS_COMVENIENT_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define	CA_ALARM							(0)
#define	CA_COMMAND							(1)
#define	CA_STATUS							(2)

#define CA_MAX_TEXT_SIZE					(1024)
#define CA_MAX_PURSE_NUM					(17)
#define CA_MAX_DES_NUM                 		(8)
#define CA_COMMON_TEXT_SIZE					(32)
#define CA_BOXID_SIZE						(16)
#define CA_FINGERPRINT_LEN					(35)

typedef enum _CaAlarm
{
	CA_ALARM_CANCEL = 0,					//���CA��Ϣ
	CA_ALARM_OTHER,							//Other CA
	CA_ALARM_CARD_INSERT,					//������
	CA_ALARM_CARD_REMOVE,					//���γ�
	CA_ALARM_CARD_INCORRECT,				//��Ч��
	CA_ALARM_CARD_INCORRECT_COMVENIENT,		//��Ч��Comvenient��	
	CA_ALARM_CARD_COMMUNICATION_ERROR,		//������ʧ��
	CA_ALARM_NO_RIGHT,						//û��Ȩ��
	CA_ALARM_NO_ECMS,						//û�н��ܵ�ECM
	CA_ALARM_BAD_PAIRING,					//��Դ���
	CA_ALARM_MS_ERROR,						//Primary Secondary Error
	CA_ALARM_FTA,
	CA_ALARM_MAX
}CaAlarm_E;

typedef enum _CaCommand
{	  
	CA_COMMAND_ON_SCREEN_MSG,     			//notify  on screen message, dwPram2 = (DWORD)&CaOnScreenMessage_S
	CA_COMMAND_INPUT_PIN,         			//notify  to input pin, dwParm2 = (DWORD)&CaPinInfo_S
	CA_COMMAND_PIN_VERIFIED,      			//notify  that the PIN checking is verified
	CA_COMMAND_ORDER_INFO,		  			//notify  to get the order info, dwParm2 = (DWORD)&CaOrderInfo_S
	CA_COMMAND_PIN_CHANGED,       			//notify  that the PIN checking is changed
	CA_COMMAND_PIN_FAILURE,       			//notify  that the PIN checking is failure
	CA_COMMAND_PIN_BLOCKED,       			//notify  that the PIN checking is blocked, dwParm2 = (DWORD)CaPinBlockType_E
	CA_COMMAND_PIN_NOT_BE_CHANGED,			//notify  that the PIN checking is not be changed
	CA_COMMAND_PURSE_INFO,		  			//notify  to get the purse info, dwParm2 = (DWORD)&CaPurseInfo_S
	CA_COMMAND_FORCE_UPGRADE,	  			//notify  to upgrade forced, dwParm2 = (DWORD)&CaForcedUpgrade_S
	CA_COMMAND_RESCAN_NOW,		  			//notify  to rescan now, dwParm2 = (DWORD)&CaRescanNow_S
	CA_COMMAND_SWITCH_NOW,		  			//notify  to switch now, dwParm2 = (DWORD)&CaSwitchNow_S
	CA_COMMAND_WATERMARKING,		  		//notify  to display watermarking, dwParm2 = (DWORD)During
	CA_COMMAND_PIN_NUM_RESET,		  		//notify  to display pin number reset, dwParm2 = (DWORD)&CaPinResetInfo_S
	CA_COMMAND_EAS_EVENT_START,		  		//notify  EAS Event start, dwParm2 = (DWORD)&CaEASEvent_S
	CA_COMMAND_EAS_EVENT_END,		  		//notify  EAS Event end, dwParm2 = (DWORD)&CaEASEvent_S
	CA_COMMAND_FINGERPRINT,		  		    //notify  to display watermarking, dwParm2 = (DWORD)&CaFingerPrint_S
	CA_COMMAND_MAX
}CaCommand_E;

typedef enum _CaStatus
{
	CA_STATUS_CARD_READY,					//��ready
	CA_STATUS_CARD_INSERT,					//������
	CA_STATUS_CARD_REMOVE,					//���γ�
	CA_STATUS_CARD_INCORRECT,				//��Ч��
	CA_STATUS_CARD_INCORRECT_CONMVENIENT,	//��Ч��Comvenient��
	CA_STATUS_CARD_COMMUNICATION_ERROR,		//������ʧ��
	CA_STATUS_MAX
}CaStatus_E;

typedef enum _CaPinIndex					// Ҫ���û������PIN����
{
	PIN_INDEX_PC					= 0,	//0x00: Parental control PIN 2
	PIN_INDEX_IPPV,            				//0x01: IPPV/parental control PIN 1
	PIN_INDEX_STB_LOCK,        				//0x02: STB Lock PIN
	PIN_INDEX_HOME_SHOPPING,   				//0x03: Home shopping PIN	
	PIN_INDEX_NON_SMC 				= 0x80, //0x80: Check for non-smartcard PIN	
	PIN_INDEX_IPPV_PURSE_UNENOUGH	= 0xFF 	//0xFF: when IPPV but has no enough money 
}CaPinIndex_E;

typedef enum _CaPinText						//Ҫ���û���������ʱ��Ҫ��ʾ���ı��ĵ� 
{
	PIN_TEXT_PC						= 0,    //0x00: check for parantal control
	PIN_TEXT_IPPV,            				//0x01: check for Impulse Pay Per View
	PIN_TEXT_PC_NONSMC 				= 3,   	//0x03: check for parental control using non-smartcard pin
	PIN_TEXT_RESUME,          				//0x04: resuming of an event
	PIN_TEXT_SELECT,           				//0x05: check  for selecting an event	
	PIN_TEXT_IPPV_PURSE_UNENOUGH 	= 0xFF 	//0xFF: when IPPV but has no enough money 
}CaPinText_E;

typedef enum _CaPinBlockType				//PINBlock����
{
	SMC_INSERT 						= 0,	//0x00: pinblock for smartcard
	SMC_FREE								//0x01: pinblock for non-smartcard
}CaPinBlockType_E;

typedef struct _CaPinInfo					//Ҫ������PIN��ʱЯ������Ϣ
{
	BYTE	m_ucPinIndex;					//CaPinIndex_E
	BYTE	m_ucPinTextSelector;			//CaPinText_E
}CaPinInfo_S;

typedef struct _CaOrderInfo					//IPPVԤ����Ϣ
{
	CaPinInfo_S	m_sPinInfo;							//Ҫ���û������PIN����
	BYTE		m_acPurseText[CA_COMMON_TEXT_SIZE];	//����IPPV��Ŀʱ��Ǯ�������Ϣ
	BYTE		m_acCostText[CA_COMMON_TEXT_SIZE];	//����IPPV��Ŀʱ����Ŀ������Ϣ
}CaOrderInfo_S;

typedef struct _CaPurseInfo					//Ǯ��������Ϣ
{
	BYTE	m_ucPurseNumber;										//��Ҫ��ʾ��Ǯ����Ŀ
	BYTE	m_acPurseInfo[CA_MAX_PURSE_NUM][CA_COMMON_TEXT_SIZE];	//Ǯ���е�Token��Ϣ
}CaPurseInfo_S;   

typedef struct _CaOsmPosition				//OSMλ����Ϣ
{
	BYTE	m_bXOriginLeft;
	BYTE	m_bXAnchorCenter;
	WORD	m_wXPosition;
	BYTE	m_bYOriginTop;
	BYTE	m_bYAnchorCenter;
	WORD	m_wYPosition;
}CaOsmPosition_S;

typedef enum _CaAlignText					//�ı����뷽ʽ
{
	TEXT_LEFT_JUSTIFY			 	= 0,   	//left justified, ragged right
	TEXT_RIGHT_JUSTIFY,		 				//right justified, ragged left
	TEXT_CENTER_JUSTIFY,					//centered text, ragged left and right
	TEXT_BOTH_JUSTIFY						//justified text	
}CaAlignText_E;

typedef struct _CaOnScreenMessage			//OSM��Ϣ
{
	BOOL 			m_bIsMail;					//������ʼ�����Ҫ�洢
	BOOL			m_bIsRemoved;				//��Ϣ�Ƿ���Ա��û��Ƴ�
	WORD            m_wDuration;				//����Ϣ�����Ƴ�ʱ�������Զ�����Ϣ��ʾʱ��,��λ�룬��Ϊ0��
	CaOsmPosition_S	m_tOsmPosition;				//��Ϣ��ʾλ��
	CaAlignText_E   m_eTextAlign;				//��Ϣ�ı����뷽ʽ
	BYTE            m_bBgColor;					//����ɫ��1~15 ��ʾ15����ɫ��0ֵ��ʾ����Ҫ��ɫ
	BYTE            m_ucText[CA_MAX_TEXT_SIZE];	//��Ϣ���ݣ���\0������
}CaOnScreenMessage_S;

typedef struct _CaQpskSwitch				//Qpsk��Ϣ
{
	BYTE	m_cSwitch0_12V;    		 		//SWITCH_0V | SWITCH_12V
	BYTE	m_c22K;             			//SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBPower;        			//SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBType;         			//SINGLE | UNIVERSAL
	DWORD	m_dwLNBLowFreq;     			//low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	DWORD	m_dwLNBMidFreq;     			//middle LNB frequency in Universal Mod  [unit:KHz]
	DWORD	m_dwLNBHighFreq;    			//high LNB frequency in Universal Mod   [unit:KHz]
	BYTE	m_cDiSEqC10;        			//DiSEqC1.0����	( PORT_A | PORT_B | PORT_C | PORT_D )
	BYTE	m_cDiSEqC11;        			//DiSEqC1.0 ( 0-16 and 0 for not used )
	BYTE	m_cToneBurst;       			//Tone Burst���� ( TONEBURST_NONE / TONEBURST_A / TONEBURST_B )
	BYTE	m_cDiSEqCRepeat;    			//�ظ����� ( NoRepeat / OnceRepeat / TwiceRepeat )
	DWORD	m_dwFreqencyOffset; 			//channel frequency offset in KHz
	BYTE	m_cIQ;              			//0-Normal; 1-Invers
	BYTE 	m_cPilot;
	BYTE	m_cOption;        				//m_dwOption:2;
	BYTE	m_cModulation; 					//m_dwModulation:2;1:qpsk,2:8psk
	BYTE  	m_cStandard;    				//m_dwstandard:1; 0:s, 1:s2
	BYTE    m_cRev[3];
}CaQpskSwitch_S;

typedef struct _CaSatelliteSystem			//����Ƶ����Ϣ
{
	DWORD			m_dwFrequency;
	WORD       		m_wPrbitalPosition;
	BYTE       		m_cWestEastFlag;
	BYTE       		m_cPolarization;
	BYTE       		m_cModulation;
	DWORD      		m_dwSymbolRate;
	BYTE       		m_cFecInner;
	CaQpskSwitch_S	m_stQpskSwitch;
}CaSatelliteSystem_S;

typedef struct _CaTerrestrialSystem			//����Ƶ����Ϣ
{
	DWORD	m_dwCentreFrequency;
	BYTE    m_cBandWidth;
	BYTE    m_cConstellation;
	BYTE    m_cHierarchyInfo;
	BYTE    m_cCodeRateHpStream;
	BYTE    m_cCodeRateLpStream;
	BYTE    m_cGuardInterval;
	BYTE    m_cTransmissionMode;
	BYTE    m_cOtherFrequencyFlag;
}CaTerrestrialSystem_S;

typedef struct _CaCableSystem				//����Ƶ����Ϣ
{
	DWORD	m_dwFrequency;
	BYTE	m_cFecOuter;
	BYTE	m_cModulation;
	DWORD	m_dwSymbolRate;
	BYTE	m_cFecInner;
}CaCableSystem_S;

typedef union _CaDeliverySystem				//Ƶ����Ϣ
{
	CaCableSystem_S			m_stCableStytle;
	CaSatelliteSystem_S		m_stSatelliteStytle;
	CaTerrestrialSystem_S	m_stTerrestrialStytle;
}CaDeliverySystem_U;

typedef struct _CaForcedUpgrade				//ForceUpgrade trigger��Ϣ
{
	DWORD                  m_dwTriggerNum;				//Trigger number ����ÿһ��Trigger���Ψһ
	CaDeliverySystem_U     m_stDlySys[CA_MAX_DES_NUM];	//���������Ƶ����Ϣ
	BYTE				   m_ucDesNum;					//���������������������� 	
}CaForcedUpgrade_S;

typedef enum _CaRescanMode					//����������ģʽ
{
	RESCAN_ERASE_ALL_ADD_ALL		= 0, 	//0x00:erase all service list and add all services,FTA and CA-services.
	RESCAN_ERASE_ALL_ADD_CA,    			//0x01:erase all service list and add only CAservices for the actual CA-system.
	RESCAN_ADD_ALL ,            			//0x02:add all services, FTA and CA-services.
	RESCAN_ADD_CA              				//0x03:add only CA-services for the actual CAsystem.
}CaRescanMode_E;

typedef struct _CaRescanNow					//RescanNow trigger��Ϣ
{
	DWORD               m_dwTriggerNum;					//Trigger number ����ÿһ��Trigger���Ψһ
	CaDeliverySystem_U	m_stDlySys[CA_MAX_DES_NUM];		//���������Ƶ����Ϣ
	CaRescanMode_E		m_eRescanMode;					//����������ģʽ
	BYTE				m_ucDesNum;						//���������������������� 	
}CaRescanNow_S;

typedef struct _CaSwitchNow					//SwitchNow trigger��Ϣ
{
	DWORD	m_dwTriggerNum;					//Trigger number ����ÿһ��Trigger���Ψһ
	WORD	m_wOriginalNetworkID;			//OriginalNetworkID
	WORD	m_wServiceID;					//ServiceID
}CaSwitchNow_S;

typedef enum _CaFtaType						//����������
{
	E_CA_FTA_SAT   = 0x43,
	E_CA_FTA_CABLE = 0x44,
	E_CA_FTA_TER   = 0x5A
}CaFtaType_E;

typedef struct _CaBoxInfo					//��������Ϣ
{
	CaFtaType_E m_eSTBType;
	INT  		m_nManuCode;
	INT  		m_nCurSWVersion; 
	INT  		m_nCurHDVersion; 
	BYTE 		m_szSTBMode[CA_COMMON_TEXT_SIZE]; 
	BYTE 		m_szBoxID[CA_BOXID_SIZE];
}CaBoxInfo_S;

typedef struct _CaDateTime					//����ʱ��
{
	DWORD	m_dwYear;
	BYTE    m_cMonty;
	BYTE    m_cDay;
	BYTE    m_cHour;
	BYTE    m_cMinute;
}CaDateTime_S;

typedef struct _CaMail						//�ʼ���Ϣ
{	
	BOOL    		m_bReadFlag;				//�Ѷ���־
	CaDateTime_S  	m_stArriveTime;				//�ʼ�����ʱ��
	BYTE    		m_ucText[CA_MAX_TEXT_SIZE];	//��Ϣ���ݣ���'\0'������
}CaMail_S;

typedef enum _CaCompLevelType				//��������
{
	EM_CA_SHUT_BOTH_AV, 					//����Ƶ����
	EM_CA_SHUT_AUDIO,     					//��Ƶ����
	EM_CA_SHUT_VIDEO						//��Ƶ����
}CaCompLevelType_E;

typedef struct _CaPinResetInfo				//PIN_RESET��ʾ��PIN��
{
	BYTE	m_aucPin[4];
}CaPinResetInfo_S;

typedef struct _CaEASEvent					//EASEvent trigger��Ϣ
{
	DWORD	m_dwTriggerNum;					//Trigger number ����ÿһ��Trigger���Ψһ
	WORD	m_wOriginalNetworkID;			//OriginalNetworkID
	WORD	m_wServiceID;					//ServiceID
	BYTE	m_aucCode[3];					//EAS-code,Ԥ��
}CaEASEvent_S;

typedef enum _CaFlashAddrErr
{
	EM_FLASH_FATAL_ERROR = -4,        
    EM_FLASH_PARAM_ERROR = -3,
	EM_FLASH_LENGTH_ERROR = -2,
	EM_OTHER_FAILURE = -1,
	EM_FLASH_SET_SUCCESS = 0
}CaFlashAddrErr_E;

typedef struct _CaFlashAddress
{
    DWORD m_dwFirstMainAddr;        //��һ����Flash ��ַ
    DWORD m_dwFirstMainLength;      //��һ����Flash ���� 
    DWORD m_dwFirstBackupAddr;      //��һ�鱸��Flash 
    DWORD m_dwFirstBackupLength;    //��һ�鱸��Flash ���� 
    DWORD m_dwSecondMainAddr;        //�ڶ�����Flash ��ַ
    DWORD m_dwSecondMainLength;      //�ڶ�����Flash ���� 
    DWORD m_dwSecondBackupAddr;      //�ڶ��鱸��Flash 
    DWORD m_dwSecondBackupLength;    //�ڶ��鱸��Flash ���� 
    DWORD m_dwThirdMainAddr;      //��������Flash ��ַ
    DWORD m_dwThirdMainLength;    //��������Flash ���� 
    DWORD m_dwThirdBackupAddr;    //�����鱸��Flash 
    DWORD m_dwThirdBackupLength;  //�����鱸��Flash ���� 
    DWORD m_dwFourthMainAddr;      //���Ŀ���Flash ��ַ
    DWORD m_dwFourthMainLength;    //���Ŀ���Flash ���� 
    DWORD m_dwFourthBackupAddr;    //���Ŀ鱸��Flash 
    DWORD m_dwFourthBackupLength;  //���Ŀ鱸��Flash ���� 
}CaFlashAddress_S;

typedef enum _CaFPBGColor
{
	EM_BG_DARKGREEN = 0x0010,        
    EM_BG_BLUE = 0x0011, 
	EM_BG_SKYBLUE = 0x0012, 
	EM_BG_GREEN = 0x0013, 
	EM_BG_DULLORANGE = 0x0014,
	EM_BG_SLATE = 0x0015,
	EM_BG_ORANGE = 0x0016,
	EM_BG_TRANSPERENT = 0x0100
}CaFPBGColor_E;

typedef enum _CaFPFontColor
{
	EM_FONT_NAVYBLUE = 0x0001,        
    EM_FONT_BLACK = 0x0002, 
	EM_FONT_BROWN = 0x0003, 
	EM_FONT_BLUE = 0x0004, 
	EM_FONT_SLATE = 0x0005,
	EM_FONT_DULLORANGE = 0x0006,
	EM_FONT_LIGHTYELLOW = 0x0010,
	EM_FONT_GREEN = 0x0013,
	EM_FONT_ORANGE = 0x0014,
	EM_FONT_WHITE = 0x0025
}CaFPFontColor_E;

typedef struct _CaFingerPrint					
{
    BOOL            m_bFixed;             //��ֵΪTRUEʱ��m_ucX_percentage/m_ucY_percentage/m_eBackGroundColor/m_eFontColor����Ч
	BYTE	        m_ucX_percentage;     //������Ļ�����ľ���ռ��Ļ�������ܳ��ȵİٷֱ�(�����ٷֺ�)��m_bSpecifyΪTRUEʱ����Ч				
	BYTE	        m_ucY_percentage;	  //������Ļ���ϲ�ľ���ռ��Ļ���ϵ����ܳ��ȵİٷֱ�(�����ٷֺ�)��m_bSpecifyΪTRUEʱ����Ч	
	CaFPBGColor_E   m_eBackGroundColor;   //m_bSpecifyΪTRUEʱ����Ч
	CaFPFontColor_E m_eFontColor;         //m_bSpecifyΪTRUEʱ����Ч
	BOOL            m_bCovert;            //�Ƿ���˸��ʾ����ֵΪTRUEʱ��m_wHideFrames/m_wShowFrames����Ч
	WORD            m_wHideFrames;        //���ص�֡����m_bGlintΪTRUEʱ����Ч
	WORD            m_wShowFrames;        //��ʾ��֡����m_bGlintΪTRUEʱ����Ч
	BOOL            m_bEcmBased;          //�Ƿ���ECMָ�ƣ���ֵΪTRUEʱ��m_wServiceId����Ч
	WORD            m_wServiceId;         //m_bEcmBasedΪTRUEʱ����Ч
	DWORD           m_dwDuration;         //ָ����ʾ������ʱ�䣬��λΪ��
	BYTE            m_aucContent[CA_FINGERPRINT_LEN]; //ָ������
}CaFingerPrint_S;

/**************************************************************************
*  ���溯����CA���ã�Ӧ�ø���ʵ��
**************************************************************************/
/**************************************************************************
* Function Name: CSCAGetBoxDes
*  CA ��ȡ�����������Ϣ
*
*Parameters:
*	pstSTBInfo[out]:	��������Ϣ
*
*Return Values:
*	��
*
*Note:
*	SN��10λ,boxid��16λ16������,��about ca������Կ���,���Ի���ɺ��������ȥsn�ģ�
*	��boxid����snת��16��������,����û�����кŵĻ�,��ʱboxid��ȡchipid������
*	�½ӿڣ��������Ŀʹ���¿��Ҳ����ú��������ڸú�����ֱ�ӵ���CSGetCaBoxDes
**************************************************************************/
extern void CSCAGetBoxDes(CaBoxInfo_S *pstSTBInfo);

/**************************************************************************
* Function Name: CSCAGetFlashAddress
*  CA ��ȡFlash�ռ��ַ,1��Block.
*
*Parameters:
*	��
*
*Return Values:
*	CA FLASH �ռ��׵�ַ
*
*Note:
*	�½ӿڣ��������Ŀʹ���¿��Ҳ����ú��������ڸú�����ֱ�ӵ���CSGetCaFlashAddress
**************************************************************************/
extern DWORD CSCAGetFlashAddress(void);

/**************************************************************************
*Function Name:CSCAGetBackupFlashAddress
*  CA ��ȡ������������Flash��ַ,1��Block.
*
*Parameters:
*	��
*
*Return Values: 
*	CA���������� FLASH �׵�ַ
**************************************************************************/
extern DWORD CSCAGetBackupFlashAddress(void);

/**************************************************************************
*Function Name:CSCASetFlashAddress
* ����CA �ں˴洢���������FLASH ��ַ,CA��ʼ��֮ǰ���øú���
*
*Parameters
*  pstFlashAddress: CA Flash �����Ϣ
                                
pstFlashAddress->m_dwFirstMainAddr: �����CA�ں˵�һ������������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwFirstMainLength: CA�ں˵�һ������������С,��С��64K
pstFlashAddress->m_dwFirstBackupAddr:�����CA�ں˵�һ��������������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwFirstBackupLength:CA�ں˵�һ��������������С,��С��64K  

pstFlashAddress->m_dwSecondMainAddr: �����CA�ں˵ڶ�������������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwSecondMainLength: CA�ں˵ڶ�������������С,��С��64K
pstFlashAddress->m_dwSecondBackupAddr:�����CA�ڶ����ں˱�����������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwSecondBackupLength:CA�ں˵ڶ���������������С,��С��64K  

pstFlashAddress->m_dwThirdMainAddr: �����CA�ں˵���������������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwThirdMainLength: CA�ں˵���������������С,��С��64K
pstFlashAddress->m_dwThirdBackupAddr:�����CA�ں˵�����������������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwThirdBackupLength:CA�ں˵�����������������С,��С��64K  

pstFlashAddress->m_dwFourthMainAddr: �����CA�ں˵��ĸ�����������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwFourthMainLength: CA�ں˵��ĸ�����������С,��С��64K
pstFlashAddress->m_dwFourthBackupAddr:�����CA�ں˵��ĸ�������������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwFourthBackupLength:CA�ں˵��ĸ�������������С,��С��64K  
*                     
*Return Values: 
*	��CaFlashAddrErr_E����

Note:
    ���ú������ش��󣬽�����CA��ʼ��ʧ��!!
**************************************************************************/
CaFlashAddrErr_E CSCASetFlashAddress(CaFlashAddress_S *pstFlashAddress);

/**************************************************************************
*Function Name:CSCAGetMailFlashAddress
* CA ��ȡ�ʼ���������Flash��ַ, 1��Block
*
*Parameters:
*	��
*
*Return Values: 
*	�ʼ������� FLASH �׵�ַ
*
*Note:
*	�½ӿڣ��������Ŀʹ���¿��Ҳ����ú��������ڸú�����ֱ�ӵ���CSCAGetFlashMailAddress
**************************************************************************/
extern DWORD CSCAGetMailFlashAddress(void);

/**************************************************************************
* Function Name: CSCAGetBoxId
*  CA ��ȡSTB��Box ID.��CA porting ʵ��
*
*Parameters:	
*	pcBoxId[out]:	��Ż����е�BoxID.
*	pnLength[in&out]:in buffer�ĳ���outʵ��boxid�ĳ���
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:
*�µ���Ŀ�Ƽ�������ӿڻ�ȡboxid 
*  
**************************************************************************/
INT CSCAGetBoxId(BYTE *pcBoxId, INT* pnLength);
/**************************************************************************
* Function Name: CSCAGetEpgTime
*  CA ��ȡSTB ��ǰEPG ʱ��. ��Ӧ��ʵ�ָú���
*
*Parameters:
*	pstDataTime:	��ź��ӵ�EPG ʱ��
*Return Values:
*	��
**************************************************************************/
extern void CSCAGetEpgTime(CaDateTime_S *pstDataTime);

/**************************************************************************
*  ���溯����Ӧ�õ��ã�CA����ʵ��
**************************************************************************/
/**************************************************************************
*Function Name:CS_CA_TXT_Add
* ����һ���ʼ�
*
*Parameters: 
*	pstTxt[in]:		�ʼ�����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:	
*	�������ʵ��Ϊ�գ�����ҪӦ������ʼ�
**************************************************************************/
INT CS_CA_TXT_Add(CaMail_S *pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Del
* ɾ��ĳһ���ʼ�
*
*Parameters: 
*	dwIndex[in]:	�ʼ��±꣬��0��ʼ
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
*Parameters: 
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
*Parameters: 
*	��
*
*Return Values: 
*	�ʼ�����
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* ��ȡ����ĳһ���ʼ�������
*
*Parameters: 
*	nIndex[in]:		�ʼ��±꣬��0��ʼ
*	ppstTxt[out]:	�����ʼ��Ĵ洢��ַ
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, CaMail_S **ppstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNew
* �ж��Ƿ������ʼ�
*
*Parameters: 
*	��
*
*Return Values:
*	TRUE: �����ʼ� 
*	FALSE:û�����ʼ� 
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(void);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* ��ȡδ���ʼ�����
*
*Parameters:
*	��
*
*Return Values: 
*	����δ���ʼ�����
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* �������ʼ����浽flash��
*
*Parameters:
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
*Parameters: 
*	dwIndex[in]:		�ʼ��洢���±꣬��0��ʼ
*	bRead[in]:		�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
* Function Name: CS_CA_ChangePin
*   �޸�PIN��
*
*Parameters:
*	bOldPinLength[in]:	�����볤��
*   pcOldPin[in]:		������
*	ucNewPinLength[in]:	�����볤��
*	pcNewPin[in]:		������
*	ePinIndex[in]:		��Ҫ�޸ĵ���������
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE		���볤�Ȳ�������BCLib���ڴ���һ��check pin or change pin
*
*Note:
*	��������汾CA�⣬��֧���޸�PIN�빦�ܣ������������ʹ��
**************************************************************************/
INT CS_CA_ChangePin(BYTE ucOldPinLength, BYTE *pcOldPin, BYTE ucNewPinLength, BYTE *pcNewPin, CaPinIndex_E ePinIndex);

/**************************************************************************
* Function Name: CS_CA_CheckCardPin
*   �û���������󣬼�������Ƿ���ȷ
*
*Parameters:
*   ucPinLength[in]:	���볤��
*   pcPin[in]:			PIN��
*	stPinInfo[in]:		���������������Ϣ(CA_COMMAND_INPUT_PIN��ϢЯ������Ϣ)
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE		���볤�Ȳ�������BCLib���ڴ���һ��check pin or change pin
**************************************************************************/
INT CS_CA_CheckCardPin(BYTE ucPinLength, BYTE *pcPin, CaPinInfo_S stPinInfo);

/**************************************************************************
* Function Name: CS_CA_GetCardNumber
*   ��ȡCardNumber
*
*Parameters:
*	pcSmcNo[out]:	���CardNumber
*	wLen[in]: 		CardNumber����      
*       
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_GetCardNumber(BYTE *pcSmcNo, WORD wLen);

/**************************************************************************
* Function Name: CS_CA_GetPurseInfo
*   ��ȡ���е�Ǯ����Ϣ
*
*Parameters:
*	��
*
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:
*	�ú���Ϊ�첽������ͨ����ϢCA_COMMAND_PURSE_INFO��������Ϣ�׸�Ӧ��
**************************************************************************/
INT CS_CA_GetPurseInfo(void);

/**************************************************************************
* Function Name: CS_CA_GetCaSystemId
*   ��ȡ��ǰ��CASystemID
*
*Parameters:
* 	��
*
*Return Values:
*	CASystemID
**************************************************************************/
WORD CS_CA_GetCaSystemId(void);

/**************************************************************************
* Function Name: CS_CA_GetBCLibVersion
*   ��ȡ��ǰ��BC lib �İ汾��Ϣ�ʹ���ʱ��
*
*Parameters:
*	pacVersion[out]:	��ȡ��汾��
*	pacDate[out]:		��ȡ���������
*	pacTime[time]:		��ȡ�����ʱ��
*
*Return Values:
*	��
**************************************************************************/
void CS_CA_GetBCLibVersion(BYTE *pacVersion, BYTE *pacDate, BYTE *pacTime);

/**************************************************************************
* Function Name: CS_CA_GetParingState
*   ��ȡ��ǰ�Ļ������״̬
*
*Parameters:
*          ��
*
*Return Values:
*	0:	��˵�����ʾ:  NoPairing                         
*	1:	��˵�����ʾ:  PairingInProgress                 
*	2:	��˵�����ʾ:  PairingOk                         
*	3: 	��˵�����ʾ:  PairingBad                        
**************************************************************************/
INT CS_CA_GetParingState(void);

/**************************************************************************
*  ���漸������רΪ�޿�ϵͳ���Ի� ��װ���ӿ�˵����ο�Comvenient����ĵ���                     
**************************************************************************/
INT CSCAInitNsc(BYTE* pabPtr, WORD* pwLen);
INT CSCAInitNscComm(BYTE* pabPtr, WORD* pwLen,
	BYTE* pabBoxSerNo,
	BYTE* pabManuId,
	BYTE* pabManuData,
	BYTE* pabProvId,
	BYTE* pabProvData);

INT CSCAStartEmmFilter(void);
INT CSCAStopEmmFilter(void);
INT CSCAInitForNsc(void);
INT CSCACheckNsc(void);

/**************************************************************************
*  ����ṹΪ�����Ͻṹ������Ŀ�����벻Ҫʹ��
**************************************************************************/
#define CA_ON_SCREEN_MESSAGE            	(0x01)

typedef CaQpskSwitch_S 			QpskSwitch_S;
typedef CaSatelliteSystem_S 	SatelliteSystem_S;
typedef CaTerrestrialSystem_S 	TerrestrialSystem_S;
typedef CaCableSystem_S 		CableSystem_S;
typedef CaDeliverySystem_U 		DeliverySystem_S;
typedef CaRescanMode_E 			RescanMode_E;
typedef CaBoxInfo_S 			CABoxInfo_S;
typedef CaDateTime_S 			CADATETIME_S;
typedef CaMail_S 				CAMail_S;
typedef CaCompLevelType_E 		E_CaCompLevelType;

#ifdef  __cplusplus
}
#endif

#endif //_CS_COMVENIENT_INTERFACE_H_

