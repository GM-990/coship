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
	CA_ALARM_CANCEL = 0,					//清除CA消息
	CA_ALARM_OTHER,							//Other CA
	CA_ALARM_CARD_INSERT,					//卡插入
	CA_ALARM_CARD_REMOVE,					//卡拔出
	CA_ALARM_CARD_INCORRECT,				//无效卡
	CA_ALARM_CARD_INCORRECT_COMVENIENT,		//无效的Comvenient卡	
	CA_ALARM_CARD_COMMUNICATION_ERROR,		//卡交互失败
	CA_ALARM_NO_RIGHT,						//没有权限
	CA_ALARM_NO_ECMS,						//没有接受到ECM
	CA_ALARM_BAD_PAIRING,					//配对错误
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
	CA_STATUS_CARD_READY,					//卡ready
	CA_STATUS_CARD_INSERT,					//卡插入
	CA_STATUS_CARD_REMOVE,					//卡拔出
	CA_STATUS_CARD_INCORRECT,				//无效卡
	CA_STATUS_CARD_INCORRECT_CONMVENIENT,	//无效的Comvenient卡
	CA_STATUS_CARD_COMMUNICATION_ERROR,		//卡交互失败
	CA_STATUS_MAX
}CaStatus_E;

typedef enum _CaPinIndex					// 要求用户输入的PIN类型
{
	PIN_INDEX_PC					= 0,	//0x00: Parental control PIN 2
	PIN_INDEX_IPPV,            				//0x01: IPPV/parental control PIN 1
	PIN_INDEX_STB_LOCK,        				//0x02: STB Lock PIN
	PIN_INDEX_HOME_SHOPPING,   				//0x03: Home shopping PIN	
	PIN_INDEX_NON_SMC 				= 0x80, //0x80: Check for non-smartcard PIN	
	PIN_INDEX_IPPV_PURSE_UNENOUGH	= 0xFF 	//0xFF: when IPPV but has no enough money 
}CaPinIndex_E;

typedef enum _CaPinText						//要求用户输入密码时需要显示的文本文档 
{
	PIN_TEXT_PC						= 0,    //0x00: check for parantal control
	PIN_TEXT_IPPV,            				//0x01: check for Impulse Pay Per View
	PIN_TEXT_PC_NONSMC 				= 3,   	//0x03: check for parental control using non-smartcard pin
	PIN_TEXT_RESUME,          				//0x04: resuming of an event
	PIN_TEXT_SELECT,           				//0x05: check  for selecting an event	
	PIN_TEXT_IPPV_PURSE_UNENOUGH 	= 0xFF 	//0xFF: when IPPV but has no enough money 
}CaPinText_E;

typedef enum _CaPinBlockType				//PINBlock类型
{
	SMC_INSERT 						= 0,	//0x00: pinblock for smartcard
	SMC_FREE								//0x01: pinblock for non-smartcard
}CaPinBlockType_E;

typedef struct _CaPinInfo					//要求输入PIN码时携带的信息
{
	BYTE	m_ucPinIndex;					//CaPinIndex_E
	BYTE	m_ucPinTextSelector;			//CaPinText_E
}CaPinInfo_S;

typedef struct _CaOrderInfo					//IPPV预定信息
{
	CaPinInfo_S	m_sPinInfo;							//要求用户输入的PIN类型
	BYTE		m_acPurseText[CA_COMMON_TEXT_SIZE];	//购买IPPV节目时，钱包余额信息
	BYTE		m_acCostText[CA_COMMON_TEXT_SIZE];	//购买IPPV节目时，节目费用信息
}CaOrderInfo_S;

typedef struct _CaPurseInfo					//钱包购买信息
{
	BYTE	m_ucPurseNumber;										//需要显示的钱包数目
	BYTE	m_acPurseInfo[CA_MAX_PURSE_NUM][CA_COMMON_TEXT_SIZE];	//钱包中的Token信息
}CaPurseInfo_S;   

typedef struct _CaOsmPosition				//OSM位置信息
{
	BYTE	m_bXOriginLeft;
	BYTE	m_bXAnchorCenter;
	WORD	m_wXPosition;
	BYTE	m_bYOriginTop;
	BYTE	m_bYAnchorCenter;
	WORD	m_wYPosition;
}CaOsmPosition_S;

typedef enum _CaAlignText					//文本对齐方式
{
	TEXT_LEFT_JUSTIFY			 	= 0,   	//left justified, ragged right
	TEXT_RIGHT_JUSTIFY,		 				//right justified, ragged left
	TEXT_CENTER_JUSTIFY,					//centered text, ragged left and right
	TEXT_BOTH_JUSTIFY						//justified text	
}CaAlignText_E;

typedef struct _CaOnScreenMessage			//OSM信息
{
	BOOL 			m_bIsMail;					//如果是邮件还需要存储
	BOOL			m_bIsRemoved;				//消息是否可以被用户移除
	WORD            m_wDuration;				//当消息不可移除时，该属性定义消息显示时间,单位秒，不为0。
	CaOsmPosition_S	m_tOsmPosition;				//消息显示位置
	CaAlignText_E   m_eTextAlign;				//消息文本对齐方式
	BYTE            m_bBgColor;					//背景色，1~15 表示15种颜色，0值表示不需要上色
	BYTE            m_ucText[CA_MAX_TEXT_SIZE];	//消息内容，以\0结束。
}CaOnScreenMessage_S;

typedef struct _CaQpskSwitch				//Qpsk信息
{
	BYTE	m_cSwitch0_12V;    		 		//SWITCH_0V | SWITCH_12V
	BYTE	m_c22K;             			//SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBPower;        			//SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBType;         			//SINGLE | UNIVERSAL
	DWORD	m_dwLNBLowFreq;     			//low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	DWORD	m_dwLNBMidFreq;     			//middle LNB frequency in Universal Mod  [unit:KHz]
	DWORD	m_dwLNBHighFreq;    			//high LNB frequency in Universal Mod   [unit:KHz]
	BYTE	m_cDiSEqC10;        			//DiSEqC1.0开关	( PORT_A | PORT_B | PORT_C | PORT_D )
	BYTE	m_cDiSEqC11;        			//DiSEqC1.0 ( 0-16 and 0 for not used )
	BYTE	m_cToneBurst;       			//Tone Burst开关 ( TONEBURST_NONE / TONEBURST_A / TONEBURST_B )
	BYTE	m_cDiSEqCRepeat;    			//重复次数 ( NoRepeat / OnceRepeat / TwiceRepeat )
	DWORD	m_dwFreqencyOffset; 			//channel frequency offset in KHz
	BYTE	m_cIQ;              			//0-Normal; 1-Invers
	BYTE 	m_cPilot;
	BYTE	m_cOption;        				//m_dwOption:2;
	BYTE	m_cModulation; 					//m_dwModulation:2;1:qpsk,2:8psk
	BYTE  	m_cStandard;    				//m_dwstandard:1; 0:s, 1:s2
	BYTE    m_cRev[3];
}CaQpskSwitch_S;

typedef struct _CaSatelliteSystem			//卫星频点信息
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

typedef struct _CaTerrestrialSystem			//地面频点信息
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

typedef struct _CaCableSystem				//有线频点信息
{
	DWORD	m_dwFrequency;
	BYTE	m_cFecOuter;
	BYTE	m_cModulation;
	DWORD	m_dwSymbolRate;
	BYTE	m_cFecInner;
}CaCableSystem_S;

typedef union _CaDeliverySystem				//频点信息
{
	CaCableSystem_S			m_stCableStytle;
	CaSatelliteSystem_S		m_stSatelliteStytle;
	CaTerrestrialSystem_S	m_stTerrestrialStytle;
}CaDeliverySystem_U;

typedef struct _CaForcedUpgrade				//ForceUpgrade trigger信息
{
	DWORD                  m_dwTriggerNum;				//Trigger number 对于每一种Trigger类别唯一
	CaDeliverySystem_U     m_stDlySys[CA_MAX_DES_NUM];	//升级所需的频点信息
	BYTE				   m_ucDesNum;					//命令中所带的描述符个数 	
}CaForcedUpgrade_S;

typedef enum _CaRescanMode					//重新搜索的模式
{
	RESCAN_ERASE_ALL_ADD_ALL		= 0, 	//0x00:erase all service list and add all services,FTA and CA-services.
	RESCAN_ERASE_ALL_ADD_CA,    			//0x01:erase all service list and add only CAservices for the actual CA-system.
	RESCAN_ADD_ALL ,            			//0x02:add all services, FTA and CA-services.
	RESCAN_ADD_CA              				//0x03:add only CA-services for the actual CAsystem.
}CaRescanMode_E;

typedef struct _CaRescanNow					//RescanNow trigger信息
{
	DWORD               m_dwTriggerNum;					//Trigger number 对于每一种Trigger类别唯一
	CaDeliverySystem_U	m_stDlySys[CA_MAX_DES_NUM];		//升级所需的频点信息
	CaRescanMode_E		m_eRescanMode;					//重新搜索的模式
	BYTE				m_ucDesNum;						//命令中所带的描述符个数 	
}CaRescanNow_S;

typedef struct _CaSwitchNow					//SwitchNow trigger信息
{
	DWORD	m_dwTriggerNum;					//Trigger number 对于每一种Trigger类别唯一
	WORD	m_wOriginalNetworkID;			//OriginalNetworkID
	WORD	m_wServiceID;					//ServiceID
}CaSwitchNow_S;

typedef enum _CaFtaType						//机顶盒类型
{
	E_CA_FTA_SAT   = 0x43,
	E_CA_FTA_CABLE = 0x44,
	E_CA_FTA_TER   = 0x5A
}CaFtaType_E;

typedef struct _CaBoxInfo					//机顶盒信息
{
	CaFtaType_E m_eSTBType;
	INT  		m_nManuCode;
	INT  		m_nCurSWVersion; 
	INT  		m_nCurHDVersion; 
	BYTE 		m_szSTBMode[CA_COMMON_TEXT_SIZE]; 
	BYTE 		m_szBoxID[CA_BOXID_SIZE];
}CaBoxInfo_S;

typedef struct _CaDateTime					//日期时间
{
	DWORD	m_dwYear;
	BYTE    m_cMonty;
	BYTE    m_cDay;
	BYTE    m_cHour;
	BYTE    m_cMinute;
}CaDateTime_S;

typedef struct _CaMail						//邮件信息
{	
	BOOL    		m_bReadFlag;				//已读标志
	CaDateTime_S  	m_stArriveTime;				//邮件到达时间
	BYTE    		m_ucText[CA_MAX_TEXT_SIZE];	//消息内容，以'\0'结束。
}CaMail_S;

typedef enum _CaCompLevelType				//加扰类型
{
	EM_CA_SHUT_BOTH_AV, 					//音视频加扰
	EM_CA_SHUT_AUDIO,     					//音频加扰
	EM_CA_SHUT_VIDEO						//视频加扰
}CaCompLevelType_E;

typedef struct _CaPinResetInfo				//PIN_RESET显示的PIN码
{
	BYTE	m_aucPin[4];
}CaPinResetInfo_S;

typedef struct _CaEASEvent					//EASEvent trigger信息
{
	DWORD	m_dwTriggerNum;					//Trigger number 对于每一种Trigger类别唯一
	WORD	m_wOriginalNetworkID;			//OriginalNetworkID
	WORD	m_wServiceID;					//ServiceID
	BYTE	m_aucCode[3];					//EAS-code,预留
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
    DWORD m_dwFirstMainAddr;        //第一块主Flash 地址
    DWORD m_dwFirstMainLength;      //第一块主Flash 长度 
    DWORD m_dwFirstBackupAddr;      //第一块备份Flash 
    DWORD m_dwFirstBackupLength;    //第一块备份Flash 长度 
    DWORD m_dwSecondMainAddr;        //第二块主Flash 地址
    DWORD m_dwSecondMainLength;      //第二块主Flash 长度 
    DWORD m_dwSecondBackupAddr;      //第二块备份Flash 
    DWORD m_dwSecondBackupLength;    //第二块备份Flash 长度 
    DWORD m_dwThirdMainAddr;      //第三块主Flash 地址
    DWORD m_dwThirdMainLength;    //第三块主Flash 长度 
    DWORD m_dwThirdBackupAddr;    //第三块备份Flash 
    DWORD m_dwThirdBackupLength;  //第三块备份Flash 长度 
    DWORD m_dwFourthMainAddr;      //第四块主Flash 地址
    DWORD m_dwFourthMainLength;    //第四块主Flash 长度 
    DWORD m_dwFourthBackupAddr;    //第四块备份Flash 
    DWORD m_dwFourthBackupLength;  //第四块备份Flash 长度 
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
    BOOL            m_bFixed;             //该值为TRUE时，m_ucX_percentage/m_ucY_percentage/m_eBackGroundColor/m_eFontColor才有效
	BYTE	        m_ucX_percentage;     //距离屏幕最左侧的距离占屏幕从左到右总长度的百分比(不带百分号)，m_bSpecify为TRUE时才有效				
	BYTE	        m_ucY_percentage;	  //距离屏幕最上侧的距离占屏幕从上到下总长度的百分比(不带百分号)，m_bSpecify为TRUE时才有效	
	CaFPBGColor_E   m_eBackGroundColor;   //m_bSpecify为TRUE时才有效
	CaFPFontColor_E m_eFontColor;         //m_bSpecify为TRUE时才有效
	BOOL            m_bCovert;            //是否闪烁显示，该值为TRUE时，m_wHideFrames/m_wShowFrames才有效
	WORD            m_wHideFrames;        //隐藏的帧数，m_bGlint为TRUE时才有效
	WORD            m_wShowFrames;        //显示的帧数，m_bGlint为TRUE时才有效
	BOOL            m_bEcmBased;          //是否是ECM指纹，该值为TRUE时，m_wServiceId才有效
	WORD            m_wServiceId;         //m_bEcmBased为TRUE时才有效
	DWORD           m_dwDuration;         //指纹显示持续的时间，单位为秒
	BYTE            m_aucContent[CA_FINGERPRINT_LEN]; //指纹内容
}CaFingerPrint_S;

/**************************************************************************
*  下面函数由CA调用，应用负责实现
**************************************************************************/
/**************************************************************************
* Function Name: CSCAGetBoxDes
*  CA 获取机顶盒相关信息
*
*Parameters:
*	pstSTBInfo[out]:	机顶盒信息
*
*Return Values:
*	无
*
*Note:
*	SN是10位,boxid是16位16进制数,在about ca界面可以看到,个性化完成后会升级进去sn的，
*	而boxid就是sn转成16进制数的,盒子没有序列号的话,此时boxid会取chipid来充数
*	新接口，如果老项目使用新库找不到该函数，请在该函数中直接调用CSGetCaBoxDes
**************************************************************************/
extern void CSCAGetBoxDes(CaBoxInfo_S *pstSTBInfo);

/**************************************************************************
* Function Name: CSCAGetFlashAddress
*  CA 获取Flash空间地址,1个Block.
*
*Parameters:
*	无
*
*Return Values:
*	CA FLASH 空间首地址
*
*Note:
*	新接口，如果老项目使用新库找不到该函数，请在该函数中直接调用CSGetCaFlashAddress
**************************************************************************/
extern DWORD CSCAGetFlashAddress(void);

/**************************************************************************
*Function Name:CSCAGetBackupFlashAddress
*  CA 获取备份数据区的Flash地址,1个Block.
*
*Parameters:
*	无
*
*Return Values: 
*	CA备份数据区 FLASH 首地址
**************************************************************************/
extern DWORD CSCAGetBackupFlashAddress(void);

/**************************************************************************
*Function Name:CSCASetFlashAddress
* 设置CA 内核存储数据所需的FLASH 地址,CA初始化之前调用该函数
*
*Parameters
*  pstFlashAddress: CA Flash 相关信息
                                
pstFlashAddress->m_dwFirstMainAddr: 分配给CA内核第一个主数据区地址,必须为一个非零值
pstFlashAddress->m_dwFirstMainLength: CA内核第一个主数据区大小,不小于64K
pstFlashAddress->m_dwFirstBackupAddr:分配给CA内核第一个备份数据区地址,必须为一个非零值
pstFlashAddress->m_dwFirstBackupLength:CA内核第一个备份数据区大小,不小于64K  

pstFlashAddress->m_dwSecondMainAddr: 分配给CA内核第二个主数据区地址,必须为一个非零值
pstFlashAddress->m_dwSecondMainLength: CA内核第二个主数据区大小,不小于64K
pstFlashAddress->m_dwSecondBackupAddr:分配给CA第二个内核备份数据区地址,必须为一个非零值
pstFlashAddress->m_dwSecondBackupLength:CA内核第二个备份数据区大小,不小于64K  

pstFlashAddress->m_dwThirdMainAddr: 分配给CA内核第三个主数据区地址,必须为一个非零值
pstFlashAddress->m_dwThirdMainLength: CA内核第三个主数据区大小,不小于64K
pstFlashAddress->m_dwThirdBackupAddr:分配给CA内核第三个备份数据区地址,必须为一个非零值
pstFlashAddress->m_dwThirdBackupLength:CA内核第三个备份数据区大小,不小于64K  

pstFlashAddress->m_dwFourthMainAddr: 分配给CA内核第四个主数据区地址,必须为一个非零值
pstFlashAddress->m_dwFourthMainLength: CA内核第四个主数据区大小,不小于64K
pstFlashAddress->m_dwFourthBackupAddr:分配给CA内核第四个备份数据区地址,必须为一个非零值
pstFlashAddress->m_dwFourthBackupLength:CA内核第四个备份数据区大小,不小于64K  
*                     
*Return Values: 
*	见CaFlashAddrErr_E定义

Note:
    若该函数返回错误，将导致CA初始化失败!!
**************************************************************************/
CaFlashAddrErr_E CSCASetFlashAddress(CaFlashAddress_S *pstFlashAddress);

/**************************************************************************
*Function Name:CSCAGetMailFlashAddress
* CA 获取邮件数据区的Flash地址, 1个Block
*
*Parameters:
*	无
*
*Return Values: 
*	邮件数据区 FLASH 首地址
*
*Note:
*	新接口，如果老项目使用新库找不到该函数，请在该函数中直接调用CSCAGetFlashMailAddress
**************************************************************************/
extern DWORD CSCAGetMailFlashAddress(void);

/**************************************************************************
* Function Name: CSCAGetBoxId
*  CA 获取STB的Box ID.由CA porting 实现
*
*Parameters:	
*	pcBoxId[out]:	存放机顶盒的BoxID.
*	pnLength[in&out]:in buffer的长度out实际boxid的长度
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:
*新的项目推荐用这个接口获取boxid 
*  
**************************************************************************/
INT CSCAGetBoxId(BYTE *pcBoxId, INT* pnLength);
/**************************************************************************
* Function Name: CSCAGetEpgTime
*  CA 获取STB 当前EPG 时间. 由应用实现该函数
*
*Parameters:
*	pstDataTime:	存放盒子的EPG 时间
*Return Values:
*	无
**************************************************************************/
extern void CSCAGetEpgTime(CaDateTime_S *pstDataTime);

/**************************************************************************
*  下面函数由应用调用，CA负责实现
**************************************************************************/
/**************************************************************************
*Function Name:CS_CA_TXT_Add
* 保存一封邮件
*
*Parameters: 
*	pstTxt[in]:		邮件内容
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:	
*	这个函数实现为空，不需要应用添加邮件
**************************************************************************/
INT CS_CA_TXT_Add(CaMail_S *pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Del
* 删除某一封邮件
*
*Parameters: 
*	dwIndex[in]:	邮件下标，从0开始
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Del(DWORD dwIndex);

/**************************************************************************
*Function Name:CS_CA_TXT_DelAll
* 删除所有保存过的邮件
*
*Parameters: 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_DelAll(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetNum
* 获取总邮件数
*
*Parameters: 
*	无
*
*Return Values: 
*	邮件总数
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* 获取具体某一封邮件的内容
*
*Parameters: 
*	nIndex[in]:		邮件下标，从0开始
*	ppstTxt[out]:	返回邮件的存储地址
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, CaMail_S **ppstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNew
* 判断是否有新邮件
*
*Parameters: 
*	无
*
*Return Values:
*	TRUE: 有新邮件 
*	FALSE:没有新邮件 
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(void);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* 获取未读邮件个数
*
*Parameters:
*	无
*
*Return Values: 
*	返回未读邮件个数
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* 将所有邮件保存到flash中
*
*Parameters:
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_Txt_SetReadFlag
* 设置邮件是否已读标志
*
*Parameters: 
*	dwIndex[in]:		邮件存储的下标，从0开始
*	bRead[in]:		已读(TRUE),未读(FALSE)
*
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
* Function Name: CS_CA_ChangePin
*   修改PIN码
*
*Parameters:
*	bOldPinLength[in]:	旧密码长度
*   pcOldPin[in]:		旧密码
*	ucNewPinLength[in]:	新密码长度
*	pcNewPin[in]:		新密码
*	ePinIndex[in]:		需要修改的密码类型
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE		密码长度不当或者BCLib正在处理一个check pin or change pin
*
*Note:
*	现在这个版本CA库，不支持修改PIN码功能，这个函数不再使用
**************************************************************************/
INT CS_CA_ChangePin(BYTE ucOldPinLength, BYTE *pcOldPin, BYTE ucNewPinLength, BYTE *pcNewPin, CaPinIndex_E ePinIndex);

/**************************************************************************
* Function Name: CS_CA_CheckCardPin
*   用户输入密码后，检测密码是否正确
*
*Parameters:
*   ucPinLength[in]:	密码长度
*   pcPin[in]:			PIN码
*	stPinInfo[in]:		输入的密码的相关信息(CA_COMMAND_INPUT_PIN消息携带的信息)
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE		密码长度不当或者BCLib正在处理一个check pin or change pin
**************************************************************************/
INT CS_CA_CheckCardPin(BYTE ucPinLength, BYTE *pcPin, CaPinInfo_S stPinInfo);

/**************************************************************************
* Function Name: CS_CA_GetCardNumber
*   获取CardNumber
*
*Parameters:
*	pcSmcNo[out]:	存放CardNumber
*	wLen[in]: 		CardNumber长度      
*       
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_GetCardNumber(BYTE *pcSmcNo, WORD wLen);

/**************************************************************************
* Function Name: CS_CA_GetPurseInfo
*   获取卡中的钱包信息
*
*Parameters:
*	无
*
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:
*	该函数为异步函数，通过消息CA_COMMAND_PURSE_INFO将购买信息抛给应用
**************************************************************************/
INT CS_CA_GetPurseInfo(void);

/**************************************************************************
* Function Name: CS_CA_GetCaSystemId
*   获取当前的CASystemID
*
*Parameters:
* 	无
*
*Return Values:
*	CASystemID
**************************************************************************/
WORD CS_CA_GetCaSystemId(void);

/**************************************************************************
* Function Name: CS_CA_GetBCLibVersion
*   获取当前的BC lib 的版本信息和创建时间
*
*Parameters:
*	pacVersion[out]:	获取库版本号
*	pacDate[out]:		获取库编译日期
*	pacTime[time]:		获取库编译时间
*
*Return Values:
*	无
**************************************************************************/
void CS_CA_GetBCLibVersion(BYTE *pacVersion, BYTE *pacDate, BYTE *pacTime);

/**************************************************************************
* Function Name: CS_CA_GetParingState
*   获取当前的机卡配对状态
*
*Parameters:
*          无
*
*Return Values:
*	0:	则菜单中显示:  NoPairing                         
*	1:	则菜单中显示:  PairingInProgress                 
*	2:	则菜单中显示:  PairingOk                         
*	3: 	则菜单中显示:  PairingBad                        
**************************************************************************/
INT CS_CA_GetParingState(void);

/**************************************************************************
*  下面几个函数专为无卡系统个性化 封装，接口说明请参考Comvenient相关文档。                     
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
*  下面结构为兼容老结构，新项目开发请不要使用
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

