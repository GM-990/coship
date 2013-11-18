/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd.,
*              All rights reserved.
* Filename:    CS_NAGRA7_interface.h
* Author:      zhangxing
* Date:        2011 12
* Description: 集成nagracak7版本
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
    CA_TXT_MAIL             = 0,//Text -- Mailbox,  邮件消息
    CA_TXT_ANNOUNCE         = 1,//Text -- Announcement,广播通知
    CA_TXT_CLUB             = 2 //Club number,会员号
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

typedef struct _CAMSG_ST        // 邮件属性
{
    BYTE        		m_cType;							//T_TxtType// 0: Text_Mail 1: Text_Announcement 
    BYTE        		m_cClass;               			//T_TxtShowModel// 0: Normal 1: Time 2: ForcedDisplay
    BYTE        		m_cPriority;
    DWORD       		m_dwLen;
    NAGRA_CADATETIME_ST	m_stDeleteTime;   					//在TIMED邮件中存在
    BYTE    			m_szContent[CA_MAX_SIZE_TXT_MAIL];
    WORD    			m_wDuration;						//for TMS mail
    BYTE    			m_bCoverPer; 						//for TMS mail
}NAGRA_CAMSG_ST;

typedef struct _CAMAIL_ST       //邮件结构体
{   
    BOOL    			m_bReadFlag;	//已读标志
    BOOL    			m_bLockFlag;	//邮件锁定标志
    NAGRA_CADATETIME_ST	m_stArriveTime; //邮件到达的时间
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
	CA_ALARM_CANCEL = 0,             //00 取消当前屏幕显示
	CA_ALARM_CARD_REMOVED,           //01 卡被拔出
	CA_ALARM_CARD_ERROR,             //02 智能卡错误
	CA_ALARM_CARD_MUTE,              //03 卡已插入，但无法通讯，卡可能插反了
	CA_ALARM_CARD_INVALID,           //04 卡无效，非Nagravision卡
	CA_ALARM_CARD_BLACKLISTED,       //05 卡被列入黑名单
	CA_ALARM_CARD_SUSPENDED,         //06 暂时取消卡的授权
	CA_ALARM_CARD_NEVER_PAIRED,      //07 卡与机顶盒未配对
	CA_ALARM_CARD_NOT_PAIRED,        //08 卡与当前机顶盒不配对
	CA_ALARM_CARD_EXPIRED,           //09 卡过期
	CA_ALARM_CARD_NOT_CERTIFIED,     //10 未鉴定的卡
	CA_ALARM_NO_ACCESS,              //11 没权限观看该节目
	CA_ALARM_RIGHT_SUSPENDED,        //12 授权被取消
	CA_ALARM_BLACK_OUT,              //13 节目区域禁播
	CA_ALARM_OTHER,                  //14 其他加扰节目
	CA_ALARM_ENTITLE_WILL_EXPIRE_RENEWABLE,		 //15 授权将要过期并可以恢复（支持profile功能的项目）
	CA_ALARM_ENTITLE_EXPIRED_RENEWABLE,	         //16 授权过期了并可以恢复（支持profile功能的项目）
	CA_ALARM_ENTITLE_WILL_EXPIRE_NOTRENEWABLE,   //17 授权将要过期并不可以恢复（支持profile功能的项目）
	CA_ALARM_ENTITLE_EXPIRED_NOTRENEWABLE,       //18 授权过期了并不可以恢复（支持profile功能的项目）
	CA_ALARM_ENTITLE_EXPIRE_TODAY_RENEWABLE,     //19 授权今天过期并可以恢复（支持profile功能的项目）
	CA_ALARM_ENTITLE_EXPIRE_TODAY_NOTRENEWABLE,	 //20 授权今天过期并不可以恢复（支持profile功能的项目）
	CA_ALARM_ENTITLE_SUCCESSFULLY_PROLONGED,     //21 授权已成功延长
	CA_ALARM_CHIPSET_PAIRING_REQUIRED,           //22 节目要求芯片级配对
	CA_ALARM_STRONG_PAIRING_REQUIRED, //23节目需要配对
	CA_ALARM_MAX
}E_CaAlarm;

typedef enum _CaCommand
{
    IRD_CMD_RESET_PIN_CODE = 0, 					//提示用户PIN码被重置为0000,可选
    IRD_CMD_MAIL, 									//新邮件到达，立即显示该邮件,可选
    IRD_CMD_FORCE_IDENTIFICATION_STANDARD,			//显示卡号指纹,可选
    IRD_CMD_FORCE_IDENTIFICATION_ZONEID,			//NOT USED YET
    IRD_CMD_SET_MACROVISION_CPS,					//显示MacrovisionCps,可选
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
    IRD_CMD_SET_NETWORK_ID,							//20 设置Networkid,可选
    IRD_CMD_MASTER_SLAVE_INIT,						//NOT USED YET
    IRD_CMD_MASTER_SLAVE_CANCELLATION,				//NOT USED YET
    IRD_CMD_MASTER_SLAVE_SINGLE_SHOT,				//NOT USED YET
    IRD_CMD_MASTER_SLAVE_AUTOMATIC,					//NOT USED YET
    IRD_CMD_SET_PIN_CODE,							//25 修改PIN,可选
    IRD_CMD_FORCE_STANDBY_STANDARD,					//强制待机,可选
    IRD_CMD_FORCE_STANDBY_ZONEID,					//NOT USED YET
    IRD_CMD_CONFIG_CAMLOCK,							//NOT USED YET
    IRD_CMD_COPY_PROTECTION_VALIDATE,				//NOT USED YET
    IRD_CMD_COPY_PROTECTION_REVOKE,					//30 NOT USED YET
    IRD_CMD_COPY_PROTECTION_FORCE_AUTHENTICATION,	//NOT USED YET
    IRD_CMD_COPY_PROTECTION_SET_KEY_SESSION_PERIOD,	//NOT USED YET
    IRD_CMD_RESTORE_FACTORY_SETTING,				//恢复出厂设置,可选
    IRD_CMD_FORCE_TUNE,								//Tune_t,强制切换频道,可选
    IRD_CMD_FORCE_TUNE_WITH_TIMEOUT,				//35 Tune_t,强制切换频道带超时时间,可选 
    IRD_CMD_FORCE_TUNE_ZONE,						//NOT USED YET
    IRD_CMD_FORCE_TUNE_WITH_TIMEOUT_IPTV,			//NOT USED YET
    IRD_CMD_FORCE_TUNE_ZONE_IPTV,					//NOT USED YET
    IRD_CMD_POP_UP,									//PopUp_t,显示popup,强制
    IRD_CMD_POP_UP_REMOVE,							//40 清除popup,强制
    IRD_CMD_MOVIE_KEY,								//NOT USED YET
    IRD_CMD_PUSH_VOD_CONTENT_CONFIG,				//NOT USED YET
    IRD_CMD_PUSH_VOD_PARTITION_FORMATTING,			//NOT USED YET
    IRD_CMD_PUSH_VOD_ERASE_ASSET,					//NOT USED YET
    IRD_CMD_PUSH_VOD_EARSE_METADATA_FILE,			//45 NOT USED YET
    IRD_CMD_PUSH_VOD_SET_DOWNLOADS_WAKEUP,			//NOT USED YET
    IRD_CMD_FORCE_SOFTWARE_DWONLOAD_STANDARD,		//强制OTA升级,可选
    IRD_CMD_FORCE_SOFTWARE_DWONLOAD_BY_ID,			//强制OTA升级通过ID,可选
    IRD_CMD_CHANGE_USAGE_ID_RESIDENT_SOFTWARE,		//NOT USED YET
    IRD_CMD_CHANGE_USAGE_ID_DOWNLOADABLE_APPS,		//50 NOT USED YET
    IRD_CMD_SET_COMMUNITY_TYPE,						//NOT USED YET
    IRD_CMD_FORMAT_LOGICAL_DISK,					//NOT USED YET
    IRD_CMD_USAGE_MONITORING,						//NOT USED YET
    IRD_CMD_BROADCAST_NETWORK_OPERATOR_LOCK,		//NOT USED YET
    IRD_CMD_SET_ZONE_ID,							//55,通知已设置zoneid,可选 
    IRD_CMD_CANCEL_ID,								//通知已取消设置zoneid,可选 
    IRD_CMD_EMM_WAKE_UP_DISABLING_FEATURE,			//NOT USED YET
    IRD_CMD_EMM_WAKE_UP_SETTING_PARAMETERS,			//NOT USED YET
    IRD_CMD_UPDATE_COHABITATION_TABLES,				//NOT USED YET
    IRD_CMD_REBOOT_STB,								//60,重启机顶盒,可选
    IRD_CMD_BGA_DISABLE,							//NOT USED YET
    IRD_CMD_BGA_ENABLE,								//NOT USED YET
    IRD_CMD_SET_PAIRING_CONFIG,						//Pairing_t,设置配对信息,可选
	IRD_CMD_DISPLAYPROFILE,							//通知已设置profile,可选 
	IRD_CMD_DISPLAYMESSAGE,							//65 通知已设置displaymessage,可选 
	IRD_CMD_FINGERPRINT,							//FingerPrint_S,可选
	IRD_CMD_SET_COUNTRYCODE,						//DWORD,设置国家码,可选
    IRD_CMD_NUM
}E_CaCommand;

typedef enum _CaInfo
{
	CA_INFO_SYSTEM_INFORMATION,			//带数据(DWORD)&T_CaSystemInfo
	CA_INFO_SMARTCARD_INFOMAITON,		//带数据(DWORD)&T_CaSmartCardInfo
	CA_INFO_SMARTCARD_DELTATIME,		//NOT USED YET
	CA_INFO_PINCODE_STATUS,				//带数据(DWORD)&E_CaPinCodeStatus，有PINCODE需求的项目
	CA_INFO_METADATA					//带数据(DWORD)&T_CaMetadataInfo，有Metadata需求的项目
}E_CaInfo;

typedef struct Mail_s
{
    WORD    wMailId;         //邮件ID
    BYTE    ucTotalSegment;  //邮件由几个section组成--应用不用关心
    BYTE    ucPriority;      //优先级---不知道代表什么
    BYTE    ucSegmentNum;    //这是第几个section------应用不用关心
    PBYTE   pMailPayload;    //数据头指针
    UINT   	unMailSize;      //数据的字节数
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
	BOOL 	bDisableWakeup;		//为TRUE,则关闭EmmWakeup功能,默认为FALSE,启用EmmWakeup功能,以下参数才有效
    WORD    wNetworkId;
    WORD    wTransportId;
    WORD    wServiceId;
	WORD	wWakeDuring;		//wake-up during,单位分钟
	WORD	wSleepDuring;		//sleep during,  单位分钟
	WORD 	wCheckFlag;			//CA内部标志,应用不必关心
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
  	PIN_INDEX_0 = 0,   //YQL@2012.9.21 对应nagra的pinid也是0--3       
  	PIN_INDEX_1 = 1,
  	PIN_INDEX_2,
  	PIN_INDEX_3,
  	PIN_INDEX_4,    //YQL@2012.9.21测试发现这个index是无效的
  	PIN_INDEX_LAST	//Last type. To be used only for consistency checks.
}E_CaPinIndex;

typedef enum _CaPinCodeStatus
{
	PIN_CODE_PIN_ACCEPTED,		//PIN码检测正确或修改PIN码成功
	PIN_CODE_PIN_REJECTED,		//PIN码检测错误或旧密码错误导致修改PIN码失败
	PIN_CODE_PIN_SUSPENDED,		//PIN码挂起
	PIN_CODE_PIN_INDEX_INVALID,	//PIN码索引无效
	PIN_CODE_NO_VALID_SMARTCARD,//未插卡
	PIN_CODE_NO_MANAGE,			//smart card is not able to manage PIN codes
	PIN_CODE_PIN_BLOCK,			//PIN code is blocked
	PIN_CODE_ERROR				//其他错误
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
	WORD m_wServiceID;					//这三者都为0时所有正在收看的节目都显示
	WORD m_wDisplayTime;				//每一次显示的时间，单位ms
	BYTE m_ucLocationX;					//percentage (0-100)
	BYTE m_ucLocationY;					//percentage (0-100),这两者都为0时位置随机
	BYTE m_ucForegroundcolor;			//字体颜色，0: black，1: white，2: red，3: blue，4: green，5: yellow，6: future use，7: transparent (100%)
	BYTE m_ucBackgroundcolor;			//背景颜色，0: black，1: white，2: red，3: blue，4: green，5: yellow，6: future use，7: transparent (100%)
	BOOL m_ucShowTime;					//是否显示当前时间，0不显示，1显示
	BOOL m_ucShowSTBSN;					//是否显示机顶盒序列号，0不显示，1显示
	BYTE m_ucLogo;						//0: no logo，1: logo 1 shown，2: logo 2 shown，3: logo 3 shown
	BYTE m_ucRepetTime;					//重复显示的次数,0时显示1次
	WORD m_wRepetInterval;				//每两次显示之间的时间间隔，单位s
	BYTE m_aucZipCodeInit[ZONE_ID_LEN];	//起始ZipCode值
	BYTE m_aucZipCodeFinal[ZONE_ID_LEN];//终止ZipCode值，ZipCode值在该范围内的机顶盒显示该指纹
}FingerPrint_S;

/**************************************************************************
* Function Name: CS_CA_RequestSystemInfo
*   获取CA  System Infomation. 
*   异步，数据由CA_INFO_SYSTEM_INFORMATION上报
*
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_RequestSystemInfo(void);

/**************************************************************************
* Function Name: CS_CA_RequestSmartCardInfo
*   获取CA  SmartCard Infomation. 
*   异步，数据由CA_INFO_SMARTCARD_INFORMATION上报
*
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_RequestSmartCardInfo(void);

/**************************************************************************
*Function Name: CS_CA_TXT_Add
*添加一封新邮件.
*
*Parameters 
*	pstTxt[in]:
*		邮件结构体
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Add(NAGRA_CAMAIL_ST* pstTxt);

/**************************************************************************
*Function Name: CS_CA_TXT_Del
*删除一封邮件.
*
*Parameters 
*	dwIndex[in]:
*		邮件索引号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Del(DWORD dwIndex);

/**************************************************************************
*Function Name: CS_CA_TXT_DelAll
*删除所有邮件.
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_DelAll(void);

/**************************************************************************
*Function Name: CS_CA_TXT_GetNum
*删除所有邮件.
*
*Return Values: 
*	 邮件个数
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name: CS_CA_TXT_GetInfo
*获取邮件详细信息.
*
*Parameters 
*	dwIndex[in]:
*		邮件索引号
*	ppstTxt[out]:
*		邮件信息
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, NAGRA_CAMAIL_ST** ppstTxt);

/**************************************************************************
*Function Name: CS_CA_TXT_HaveNew
*是否有某类型的新邮件
*
*Parameters 
*	eType[in]:
*		邮件类型
*Return Values: 
*	TRUE : 有新邮件
*	FALSE: 没有新邮件
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(NAGRA_TXTTYPE_ST eType);

/**************************************************************************
*Function Name: CS_CA_TXT_SetReadFlag
*设置邮件已读标志
*
*Parameters 
*	dwIndex[in]:
*		邮件索引号
*	bRead[in]:
*		已读/未读
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name: CS_CA_TXT_Save
*保存邮件到Flash中
*
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name: CS_CA_TXT_EraserAll
*擦除所有存在FLASH中的邮件
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_EraserAll(void);

/**************************************************************************
*Function Name: CS_CA_TXT_HaveNewMailNum
*获取新邮件个数.
*
*Return Values: 
*	未读 邮件个数
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
* Function Name: CSGetCAFlashAddress
*   获取CA  所需FLASH 空间地址. 
*
*Parameters 
*	FlashType[in]:
*		Flash区域类型,E_CaFlashType
*	pFlashMailAddr[in]:
*		起始地址
*	pFlashMailSize[in]:
*		区域大小
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
		//以下3个类型的FLASH，如果至少有一个存在，则需要分配一块独立的BLOCK，其划分为:
		//ENTITLE_FLASH(长度为CA_PROFILE_FLASH_SIZE+CA_MESSAGE_FLASH_SIZE):适用于HDPlus的项目
		//EMM_WAKEUP_FLASH(长度为CA_EMM_WAKEUP_FLASH_SIZE):适用于真待机的项目
		//PROLONG_FLASH(长度为CA_SCNUM_FLASH_SIZE+CA_PROLONG_FLASH_SIZE):适用于HDPlus的项目
		//如果哪个不存在，则将其对应的*pFlashMailAddr = 0;*pFlashMailSize = 0;
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
		//CSCDATA类型的FLASH，从Flash读取到的CSCData成功并且CRC校验通过，才传给CA有效地址和长度
		case CSC_DATA_FLASH:
			{
				if(CS_CSCData_CRC16(readcrcdata()))//从Flash读取到的CSCData成功并且CRC校验通过
				{
					*pFlashMailAddr = dwCscDataAddr;	//将实际的CSCData起始地址传给CA
					*pFlashMailSize = CSCDATA_LEN;		//将实际的CSCData数据长度传给CA
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
*   冻结所有事件响应(遥控器和前面板). 
*
*Parameters 
*Return Values
**************************************************************************/
extern void CS_CA_FreezeAllEvent(void);

/**************************************************************************
* Function Name: CS_CA_UnfreezeEvent
*   解除所有事件响应的冻结(遥控器和前面板). 
*
*Parameters 
*Return Values
**************************************************************************/
extern void CS_CA_UnfreezeEvent(void);

/**************************************************************************
* Function Name: CS_CA_ShowPopUp
*   显示Popup信息.
*
*Parameters 
*	ucLanguage[in]:
*		语言类型
*	unSize[in]:
*		显示内容大小
*	pContent[in]:
*		显示内容
*Return Values
**************************************************************************/
extern void CS_CA_ShowPopUp(BYTE ucLanguage[3], UINT unSize, PBYTE pContent);

/**************************************************************************
* Function Name: CS_CA_HidePopUp
*   隐藏Popup信息.
*
*Parameters 
*Return Values
**************************************************************************/
extern void CS_CA_HidePopUp(void);

/**************************************************************************
* Function Name: CS_CA_GetZoneId
*   获取机顶盒的ZoneId
*
*Parameters	
*		pZoneId
*            [out] ZoneId数组首地址
*       nLen
*            [in] ZoneId长度，为6  
*
*Return Values
*		SUCCESS	成功
*		FAILURE	失败
*Notes
*		未设置过ZoneId或者失败都会返回FAILURE，只有返回SUCCESS才有效
**************************************************************************/
INT CS_CA_GetZoneId(BYTE *pZoneId, INT nZoneIdLen);

/**************************************************************************
* Function Name: CSCASetCheckValidateIrd
*   设置是否校验IRD命令
*
*Parameters	
*		bSet
*            [in] TRUE:校验，FALSE:不校验
*
*Notes
*		默认都是需要校验，HDPlus要求Profile命令不校验，需要调用该接口
**************************************************************************/
void CSCASetCheckValidateIrd(BOOL bSet);

/**************************************************************************
* Function Name: CSCACheckPinCode
*   检测输入PIN码是否正确
*
*Parameters	
*		emPinIndex
*            [in] PIN码索引
*		dwPinCode
*			 [in] 输入的PIN码 
*
*Return Values
*		SUCCESS	成功
*		其他值失败
*Notes
*		该函数是异步的，结果会通过消息CA_INFO_PINCODE_STATUS上报
*		关于emPinIndex，nagra定义也不清楚，N8611I项目传PIN_INDEX_2
**************************************************************************/
INT CSCACheckPinCode(E_CaPinIndex emPinIndex, DWORD dwPinCode);

/**************************************************************************
* Function Name: CSCAChangePinCode
*   修改PIN码
*
*Parameters	
*		emPinIndex
*            [in] PIN码索引
*		dwOldPinCode
*			 [in] 输入的原始PIN码 
*		dwNewPinCode
*			 [in] 输入的新PIN码 
*Return Values
*		SUCCESS	成功
*		其他值失败
*Notes
*		该函数是异步的，结果会通过消息CA_INFO_PINCODE_STATUS上报
*		关于emPinIndex，nagra定义也不清楚，N8611I项目传PIN_INDEX_2
**************************************************************************/
INT CSCAChangePinCode(E_CaPinIndex emPinIndex, DWORD dwOldPinCode, DWORD dwNewPinCode);

/**************************************************************************
* Function Name: CSCAHDPlusGetHotlineNumber
*   获取HotlineNumber
*
*Parameters	
*		pucNumber
*            [out] HotlineNumber
*		nLen 
*			 [in] HotlineNumber长度,>=32
*
*Notes
*		有HDPlus的Entitlement需求的项目使用
**************************************************************************/
BOOL CSCAHDPlusGetHotlineNumber(BYTE *pucNumber, INT nLen);

/**************************************************************************
* Function Name: CSCAHDPlusGetCallCharge
*   获取CallCharge
*
*Parameters	
*		pucCharge
*            [out] CallCharge
*		nLen 
*			 [in] CallCharge长度,>=32
*
*Notes
*		有HDPlus的Entitlement需求的项目使用
**************************************************************************/
BOOL CSCAHDPlusGetCallCharge(BYTE *pucCharge, INT nLen);

/**************************************************************************
* Function Name: CSCAHDPlusSaveEntitlementMessageToFlash
*   保存显示的EntitlementMessage到Flash
*
*Notes
*		当有EntitlementMessage显示，并且用户选择清除显示的时候需要调用该接口
*		有HDPlus的Entitlement需求的项目使用
**************************************************************************/
void CSCAHDPlusSaveEntitlementMessageToFlash(void);

/**************************************************************************
* Function Name: CSCAHDPlusGetProductExpirationDate
*   获取产品的过期时间
*
*Parameters	
*		pnStatus
*            [out] -1:没有获取到过期时间,0:已经过期,1:节目未过期
*		pstExpirationDate 
*			 [out] 节目未过期时,pstExpirationDate才有效
*
*Notes
*		有HDPlus的Entitlement需求的项目使用
**************************************************************************/
void CSCAHDPlusGetProductExpirationDate(int *pnStatus, TDATE *pstExpirationDate);

/**************************************************************************
* Function Name: CSCAGetEmmWakeupParam
*   获取EmmWakeup参数
*
*Parameters	
*		pstEmmWakeupParam
*            [in] EmmWakeup参数结构体
*Return Values
*		TRUE	成功
*		FALSE	失败
*Notes
*		该函数在真待机项目中使用，目的是能够在待机的情况下接收EMM信息
**************************************************************************/
BOOL CSCAGetEmmWakeupParam(EmmWakeup_t *pstEmmWakeupParam);

#ifdef  __cplusplus
}
#endif

#endif //_CS_NAGRA7_INTERFACE_H_

