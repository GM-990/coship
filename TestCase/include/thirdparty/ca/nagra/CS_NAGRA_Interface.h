/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd.,
*              All rights reserved.
* Filename:    CS_NAGRA_interface.h
* Author:      xiejianghua
* Date:        2008 6
* Description:
*******************************************************************************/
#include "udiplus_typedef.h"

/*******************************************************************************
* 			                Revision History                               						     *
********************************************************************************
* - Revision 1.0  2008/6
*   Programmer:Xie jianghua
*   Create.
*******************************************************************************/
#ifndef _CS_NAGRA_INTERFACE_H_
#define _CS_NAGRA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define CA_MAX_ES_NUM				(8)
#define MAX_PROFILE_NUM				(4)
#define MAX_PERIOD_NUM				(5)
#define MAX_DIS_MSG_NUM				(8)
#define MAX_DIS_MSG_LEN				(32)

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

#define CA_MAX_SIZE_TXT_MAIL       (3*1024)

typedef struct _CAMSG_ST        // 邮件属性
{
    BYTE        m_cType;                //T_TxtType// 0: Text_Mail  1: Text_Announcement 
    BYTE        m_cClass;               //T_TxtShowModel// 0: Normal     1: Time              2: ForcedDisplay
    BYTE        m_cPriority;
    DWORD       m_dwLen;
    NAGRA_CADATETIME_ST  m_stDeleteTime;   //在TIMED邮件中存在
    BYTE    m_szContent[CA_MAX_SIZE_TXT_MAIL];

    WORD    m_wDuration;//for TMS mail
    BYTE    m_bCoverPer; //for TMS mail
}NAGRA_CAMSG_ST;

typedef struct _CAMAIL_ST       //邮件结构体
{   
    BOOL    m_bReadFlag;            //已读标志
    BOOL    m_bLockFlag;             //邮件锁定标志
    NAGRA_CADATETIME_ST  m_stArriveTime; //邮件到达的时间
    NAGRA_CAMSG_ST m_stMsg;
}NAGRA_CAMAIL_ST;

typedef enum _CaMessage
{
	CA_ALARM,
	CA_COMMAND,
	CA_INFO	
}E_CaMessage;

typedef enum _CaAlarm
{
	CA_ALARM_CANCEL = 0,
	CA_ALARM_CARD_REMOVED,
	CA_ALARM_CARD_ERROR,
	CA_ALARM_CARD_MUTE,
	CA_ALARM_CARD_INVALID,
	CA_ALARM_CARD_BLACKLISTED,//5
	CA_ALARM_CARD_SUSPENDED,
	CA_ALARM_CARD_NEVER_PAIRED,
	CA_ALARM_CARD_NOT_PAIRED,
	CA_ALARM_CARD_EXPIRED,
	CA_ALARM_CARD_NOT_CERTIFIED,//10
	CA_ALARM_NO_ACCESS,
	CA_ALARM_RIGHT_SUSPENDED,
	CA_ALARM_BLACK_OUT,
	CA_ALARM_OTHER,
	CA_ALARM_ENTITLE_WILL_EXPIRE_RENEWABLE,//15 130
	CA_ALARM_ENTITLE_EXPIRED_RENEWABLE,// 131
	CA_ALARM_ENTITLE_WILL_EXPIRE_NOTRENEWABLE,//132
	CA_ALARM_ENTITLE_EXPIRED_NOTRENEWABLE,//133
	CA_ALARM_ENTITLE_EXPIRE_TODAY_RENEWABLE,//134
	CA_ALARM_ENTITLE_EXPIRE_TODAY_NOTRENEWABLE,//20 135	
	CA_ALARM_MAX
}E_CaAlarm;

typedef enum _CaCommand
{
    IRD_CMD_RESET_PIN_CODE, 
    
    IRD_CMD_MAIL, //Mail_t
    
    IRD_CMD_FORCE_IDENTIFICATION_STANDARD,
    IRD_CMD_FORCE_IDENTIFICATION_ZONEID,//NOT USED YET
    
    IRD_CMD_SET_MACROVISION_CPS,//MacrovisionCps_t

    //IRD_CMD_CONFIG_STB,//too many functions as below
    IRD_CMD_ENABLE_VIDEO,//TRUE/FALSE 5
    IRD_CMD_ENABLE_AUDIO,//TRUE/FALSE
    IRD_CMD_ENABLE_SMARTCARD1,//TRUE/FALSE
    IRD_CMD_ENABLE_SMARTCARD2,//TRUE/FALSE
    IRD_CMD_ENABLE_HARDDISC,//TRUE/FALSE
    IRD_CMD_ENABLE_DVD,//TRUE/FALSE 10
    IRD_CMD_ENABLE_SERIALPORT1,//TRUE/FALSE
    IRD_CMD_ENABLE_SERIALPORT2,//TRUE/FALSE
    IRD_CMD_ENABLE_PARALLELPORT,//TRUE/FALSE
    IRD_CMD_ENABLE_USBPORT,//TRUE/FALSE
    IRD_CMD_ENABLE_1394PORT,//TRUE/FALSE 15
    IRD_CMD_ENABLE_SPAREPORT1,//TRUE/FALSE
    IRD_CMD_ENABLE_SPAREPORT2,//TRUE/FALSE
    IRD_CMD_ENABLE_PERIPHERAL1,//TRUE/FALSE
    IRD_CMD_ENABLE_PERIPHERAL2,//TRUE/FALSE
    
    
    IRD_CMD_SET_NETWORK_ID,//NetWorkId_t 20

    //MASTER_SLAVE not used yet below
    IRD_CMD_MASTER_SLAVE_INIT,
    IRD_CMD_MASTER_SLAVE_CANCELLATION,
    IRD_CMD_MASTER_SLAVE_SINGLE_SHOT,
    IRD_CMD_MASTER_SLAVE_AUTOMATIC,
    
    IRD_CMD_SET_PIN_CODE,//PinCode_t 25
    
    IRD_CMD_FORCE_STANDBY_STANDARD,
    IRD_CMD_FORCE_STANDBY_ZONEID,//not used yet
    
    IRD_CMD_CONFIG_CAMLOCK,

    //COPY_PROTECTION not used yet
    IRD_CMD_COPY_PROTECTION_VALIDATE,
    IRD_CMD_COPY_PROTECTION_REVOKE,//30
    IRD_CMD_COPY_PROTECTION_FORCE_AUTHENTICATION,
    IRD_CMD_COPY_PROTECTION_SET_KEY_SESSION_PERIOD,
    
    IRD_CMD_RESTORE_FACTORY_SETTING,
    
    IRD_CMD_FORCE_TUNE,//Tune_t
    IRD_CMD_FORCE_TUNE_WITH_TIMEOUT,//Tune_t 35
    IRD_CMD_FORCE_TUNE_ZONE,//not used yet
    IRD_CMD_FORCE_TUNE_WITH_TIMEOUT_IPTV,//not used yet
    IRD_CMD_FORCE_TUNE_ZONE_IPTV,//not used yet
    
    IRD_CMD_POP_UP,//PopUp_t

    IRD_CMD_POP_UP_REMOVE,//40
    
    IRD_CMD_MOVIE_KEY,//not used yet
    
    //PUSH_VOD not used yet
    IRD_CMD_PUSH_VOD_CONTENT_CONFIG,
    IRD_CMD_PUSH_VOD_PARTITION_FORMATTING,
    IRD_CMD_PUSH_VOD_ERASE_ASSET,
    IRD_CMD_PUSH_VOD_EARSE_METADATA_FILE,//45
    IRD_CMD_PUSH_VOD_SET_DOWNLOADS_WAKEUP,

    //FORCE_SOFTWARE_DWONLOAD not used yet
    IRD_CMD_FORCE_SOFTWARE_DWONLOAD_STANDARD,
    IRD_CMD_FORCE_SOFTWARE_DWONLOAD_BY_ID,
    
    //CHANGE_USAGE_ID not used yet
    IRD_CMD_CHANGE_USAGE_ID_RESIDENT_SOFTWARE,
    IRD_CMD_CHANGE_USAGE_ID_DOWNLOADABLE_APPS,//50
    
    IRD_CMD_SET_COMMUNITY_TYPE,//not used yet

    IRD_CMD_FORMAT_LOGICAL_DISK,//not used yet

    IRD_CMD_USAGE_MONITORING,//not used yet

    IRD_CMD_BROADCAST_NETWORK_OPERATOR_LOCK,//not used yet

    IRD_CMD_SET_ZONE_ID,//not used yet 55
    IRD_CMD_CANCEL_ID,//not used yet

    IRD_CMD_EMM_WAKE_UP_DISABLING_FEATURE,//not used yet
    IRD_CMD_EMM_WAKE_UP_SETTING_PARAMETERS,//not used yet

    IRD_CMD_UPDATE_COHABITATION_TABLES,

    IRD_CMD_REBOOT_STB,//60
    
    IRD_CMD_BGA_DISABLE,//not used yet
    IRD_CMD_BGA_ENABLE,//not used yet

    IRD_CMD_SET_PAIRING_CONFIG,//Pairing_t

	IRD_CMD_DISPLAYPROFILE,
	IRD_CMD_DISPLAYMESSAGE,//65

//cxj add
	IRD_CMD_FINGERPRINT,//66
	IRD_CMD_SET_COUNTRYCODE,
	
    IRD_CMD_NUM
}E_CaCommand;

typedef enum _CaInfo
{
	CA_INFO_SYSTEM_INFORMATION,			//带数据(DWORD)&T_CaSystemInfo
	CA_INFO_SMARTCARD_INFOMAITON,		//带数据(DWORD)&T_CaSmartCardInfo
	CA_INFO_SMARTCARD_DELTATIME,		//带数据(DWORD)&g_CaDeltatime
	CA_INFO_PINCODE_STATUS,				//带数据(DWORD)&E_CaPinCodeStatus
	CA_INFO_METADATA					//带数据(DWORD)&T_CaMetadata
}E_CaInfo;

typedef struct Mail_s
{
    WORD    wMailId;
    BYTE    ucTotalSegment;
    BYTE    ucPriority;
    BYTE    ucSegmentNum;
    PBYTE   pMailPayload;
    UINT     unMailSize;
    
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

typedef struct 
{
    WORD    wCaCnMin;
    WORD    wCaCnMax;
}CaCnRange_t;

typedef struct Pairing_s
{
    BOOL            	bStrongPairingRequired;
    BYTE            	ucCaCnNum;
    PBYTE           	pucCaCnList;
    BYTE            	ucCaCnRangeNum;
    CaCnRange_t*   pstCaCnRangeList;
}Pairing_t;

typedef struct Period_s
{
	BYTE ucBeginOfPeriod;
	BYTE ucEndOfPeriod;
	BYTE ucFrequency;	
}Period_t;

typedef struct Profile_s
{
	BYTE ucProfileId;
	BYTE ucPeriodNum;
	Period_t stPeriodList[MAX_PERIOD_NUM];
	TDATE stLastDisplayDate;
	BYTE ucDisplayFlag;
	BYTE aucPeriod[2];
}Profile_t;

typedef struct DisplayProfile_s
{
	BYTE ucProfileNum;
	Profile_t stProfileList[MAX_PROFILE_NUM];
	WORD wCheckFlag;
}DisPlayProfile_t;

typedef struct Message_s
{
	BYTE ucMessageType;
	BYTE ucMessageLen;
	BYTE ucMessageContent[MAX_DIS_MSG_LEN];
}Message_t;

typedef struct DisplayMessage_s
{
	BYTE ucMessageNum;
	Message_t stMessageList[MAX_DIS_MSG_NUM];
	WORD wCheckFlag;
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

typedef struct
{
	INT				m_nOriNetworkID;
	INT				m_nServiceId;
	INT           		m_nTsID;
	WORD			m_wEsNum;
	WORD			m_acEsPid[CA_MAX_ES_NUM];
}T_CaServiceInfo;

typedef struct
{
	BYTE           		m_acCAKVersion[64];
	BYTE           		m_acIrdSerialNumber[64];
	BYTE           		m_acProjectInfo[64];
}T_CaSystemInfo;

typedef enum _CaFlashType
{
	NAGRA_FLASH,				//for nagra lib
	PAIRING_DATA_FLASH,			//for nagra pairing data
	CA_FLASH,					//for ca email and ird command
	ENTITLE_FLASH,				//for entitlement profile
	EMM_WAKEUP_FLASH			//for emm wake-up
}E_CaFlashType;

typedef enum _CaPinIndex
{
  	PIN_INDEX_1 = 1,
  	PIN_INDEX_2,
  	PIN_INDEX_3,
  	PIN_INDEX_4,
  	PIN_INDEX_LAST	//Last type. To be used only for consistency checks.
}E_CaPinIndex;

typedef enum _CaPinCodeStatus
{
	PIN_CODE_PIN_ACCEPTED,		//PIN码检测正确或修改PIN码成功
	PIN_CODE_PIN_REJECTED,		//PIN码检测错误或旧密码错误导致修改PIN码失败
	PIN_CODE_PIN_SUSPENDED,		//PIN码挂起
	PIN_CODE_PIN_INDEX_INVALID,	//PIN码索引无效
	PIN_CODE_NO_VALID_SMARTCARD,//未插卡
	PIN_CODE_ERROR				//其他错误
}E_CaPinCodeStatus;

/*
typedef enum _CaSmartState
{
  CA_SMARTCARD_STATE_OK,
  CA_SMARTCARD_STATE_ERROR,
  CA_SMARTCARD_STATE_MUTE,
  CA_SMARTCARD_STATE_INVALID,
  CA_SMARTCARD_STATE_BLACKLISTED,
  CA_SMARTCARD_STATE_SUSPENDED,
  CA_SMARTCARD_STATE_NEVER_PAIRED,
  CA_SMARTCARD_STATE_NOT_PAIRED,
  CA_SMARTCARD_STATE_EXPIRED,
  CA_SMARTCARD_STATE_NOT_CERTIFIED,
  CA_SMARTCARD_STATE_REMOVED
};
*/

typedef struct
{
	BYTE           		m_cSCState;
	BYTE           		m_acSCVersion[64];
	BYTE           		m_acSCNumber[64];
}T_CaSmartCardInfo;

typedef struct
{
	BYTE 				m_acMetadata[3];
	HCSHANDLE 			m_hService;
}Metadata_t;

typedef struct
{
	INT 				m_nMetadataCount;
	Metadata_t			m_stMetadata[CA_MAX_ES_NUM];
}T_CaMetadataInfo;

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
			*pFlashMailAddr = NAGRA_CA_FLASH_ADDRESS;	  //one separate block
			*pFlashMailSize = 16*1024;						  // should be large than 16K byte
			break;
		case PAIRING_DATA_FLASH:
			*pFlashMailAddr = PAIRING_DATA_FLASH_ADDRESS;  //OTP area or chipset inner ram
			*pFlashMailSize = 602; 							   // should be large than 602 byte
			break;
		case CA_FLASH:
			*pFlashMailAddr = CA_FLASH_ADDRESS; 			//one separate block
			*pFlashMailSize = 64*1024; 				  		// should be large than 64K byte
			break;
	}
}
*/

/**************************************************************************
* Function Name: CSCACSCGetProtectedKey
*   获取CSC 测试需要的ProtectedKey
*
*Parameters 
*	pucData[out]:
*		ProtectedKey数据，16字节数组
*	nLen[in]:
*		ProtectedKey长度，等于16
*Return Values
*	TRUE : SUCCESS 
*	FALSE: FAILURE
*本函数需要应用实现，读取FLASH中存储的CSCData，
*从CSCData的第18个字节开始，长度16字节就是ProtectedKey
**************************************************************************/
BOOL CSCACSCGetProtectedKey(BYTE *pucData, int nLen);

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

#endif //_CS_TFCA_INTERFACE_H_

