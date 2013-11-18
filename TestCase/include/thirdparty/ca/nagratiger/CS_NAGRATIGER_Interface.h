/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd.,
*              All rights reserved.
* Filename:    CS_NAGRATIGER_interface.h
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
#ifndef _CS_NAGRATIGER_INTERFACE_H_
#define _CS_NAGRATIGER_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define CA_MAX_ES_NUM					(8)

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
	CA_ALARM_CARD_BLACKLISTED,
	CA_ALARM_CARD_SUSPENDED,
	CA_ALARM_CARD_NEVER_PAIRED,
	CA_ALARM_CARD_NOT_PAIRED,
	CA_ALARM_CARD_EXPIRED,
	CA_ALARM_CARD_NOT_CERTIFIED,
	CA_ALARM_NO_ACCESS,
	CA_ALARM_RIGHT_SUSPENDED,
	CA_ALARM_BLACK_OUT,
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
    IRD_CMD_ENABLE_VIDEO,//TRUE/FALSE
    IRD_CMD_ENABLE_AUDIO,//TRUE/FALSE
    IRD_CMD_ENABLE_SMARTCARD1,//TRUE/FALSE
    IRD_CMD_ENABLE_SMARTCARD2,//TRUE/FALSE
    IRD_CMD_ENABLE_HARDDISC,//TRUE/FALSE
    IRD_CMD_ENABLE_DVD,//TRUE/FALSE
    IRD_CMD_ENABLE_SERIALPORT1,//TRUE/FALSE
    IRD_CMD_ENABLE_SERIALPORT2,//TRUE/FALSE
    IRD_CMD_ENABLE_PARALLELPORT,//TRUE/FALSE
    IRD_CMD_ENABLE_USBPORT,//TRUE/FALSE
    IRD_CMD_ENABLE_1394PORT,//TRUE/FALSE
    IRD_CMD_ENABLE_SPAREPORT1,//TRUE/FALSE
    IRD_CMD_ENABLE_SPAREPORT2,//TRUE/FALSE
    IRD_CMD_ENABLE_PERIPHERAL1,//TRUE/FALSE
    IRD_CMD_ENABLE_PERIPHERAL2,//TRUE/FALSE
    
    
    IRD_CMD_SET_NETWORK_ID,//NetWorkId_t

    //MASTER_SLAVE not used yet below
    IRD_CMD_MASTER_SLAVE_INIT,
    IRD_CMD_MASTER_SLAVE_CANCELLATION,
    IRD_CMD_MASTER_SLAVE_SINGLE_SHOT,
    IRD_CMD_MASTER_SLAVE_AUTOMATIC,
    
    IRD_CMD_SET_PIN_CODE,//PinCode_t
    
    IRD_CMD_FORCE_STANDBY_STANDARD,
    IRD_CMD_FORCE_STANDBY_ZONEID,//not used yet
    
    IRD_CMD_CONFIG_CAMLOCK,

    //COPY_PROTECTION not used yet
    IRD_CMD_COPY_PROTECTION_VALIDATE,
    IRD_CMD_COPY_PROTECTION_REVOKE,
    IRD_CMD_COPY_PROTECTION_FORCE_AUTHENTICATION,
    IRD_CMD_COPY_PROTECTION_SET_KEY_SESSION_PERIOD,
    
    IRD_CMD_RESTORE_FACTORY_SETTING,
    
    IRD_CMD_FORCE_TUNE,//Tune_t
    IRD_CMD_FORCE_TUNE_WITH_TIMEOUT,//Tune_t
    IRD_CMD_FORCE_TUNE_ZONE,//not used yet
    IRD_CMD_FORCE_TUNE_WITH_TIMEOUT_IPTV,//not used yet
    IRD_CMD_FORCE_TUNE_ZONE_IPTV,//not used yet
    
    IRD_CMD_POP_UP,//PopUp_t

    IRD_CMD_POP_UP_REMOVE,
    
    IRD_CMD_MOVIE_KEY,//not used yet
    
    //PUSH_VOD not used yet
    IRD_CMD_PUSH_VOD_CONTENT_CONFIG,
    IRD_CMD_PUSH_VOD_PARTITION_FORMATTING,
    IRD_CMD_PUSH_VOD_ERASE_ASSET,
    IRD_CMD_PUSH_VOD_EARSE_METADATA_FILE,
    IRD_CMD_PUSH_VOD_SET_DOWNLOADS_WAKEUP,

    //FORCE_SOFTWARE_DWONLOAD not used yet
    IRD_CMD_FORCE_SOFTWARE_DWONLOAD_STANDARD,
    IRD_CMD_FORCE_SOFTWARE_DWONLOAD_BY_ID,
    
    //CHANGE_USAGE_ID not used yet
    IRD_CMD_CHANGE_USAGE_ID_RESIDENT_SOFTWARE,
    IRD_CMD_CHANGE_USAGE_ID_DOWNLOADABLE_APPS,
    
    IRD_CMD_SET_COMMUNITY_TYPE,//not used yet

    IRD_CMD_FORMAT_LOGICAL_DISK,//not used yet

    IRD_CMD_USAGE_MONITORING,//not used yet

    IRD_CMD_BROADCAST_NETWORK_OPERATOR_LOCK,//not used yet

    IRD_CMD_SET_ZONE_ID,//not used yet
    IRD_CMD_CANCEL_ID,//not used yet

    IRD_CMD_EMM_WAKE_UP_DISABLING_FEATURE,//not used yet
    IRD_CMD_EMM_WAKE_UP_SETTING_PARAMETERS,//not used yet

    IRD_CMD_UPDATE_COHABITATION_TABLES,

    IRD_CMD_REBOOT_STB,
    
    IRD_CMD_BGA_DISABLE,//not used yet
    IRD_CMD_BGA_ENABLE,//not used yet

    IRD_CMD_SET_PAIRING_CONFIG,//Pairing_t

    IRD_CMD_NUM
}E_CaCommand;

typedef enum _CaInfo
{
	CA_INFO_SYSTEM_INFORMATION,			//带数据(DWORD)&T_CaSystemInfo
	CA_INFO_SMARTCARD_INFOMAITON,		//带数据(DWORD)&T_CaSmartCardInfo
	CA_INFO_SMARTCARD_DELTATIME			//带数据(DWORD)&g_CaDeltatime
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
	PAIRING_DATA_FLASH,		//for nagra pairing data
	CA_FLASH					// for ca email and ird command
}E_CaFlashType;

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

INT CS_CA_TXT_Add(NAGRA_CAMAIL_ST* pstTxt);
INT CS_CA_TXT_Del(DWORD dwIndex);
INT CS_CA_TXT_DelAll(void);
INT CS_CA_TXT_GetNum(void);
INT CS_CA_TXT_GetInfo(DWORD dwIndex, NAGRA_CAMAIL_ST** ppstTxt);
BOOL CS_CA_TXT_HaveNew(NAGRA_TXTTYPE_ST eType);
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);
INT CS_CA_TXT_Save(void);

INT CS_CA_TXT_EraserAll(void);
/**************************************************************************
* Function Name: CSGetCAFlashAddress
*   获取CA  所需FLASH 空间地址. 
*
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
			*pFlashMailSize = 64*1024;						  // should be large than (16+32+0.5)K byte
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

extern void CS_CA_FreezeAllEvent(void);
extern void CS_CA_UnfreezeEvent(void);
extern void CS_CA_ShowPopUp(BYTE ucLanguage[3], UINT unSize, PBYTE pContent);
extern void CS_CA_HidePopUp(void);

#ifdef  __cplusplus
}
#endif

#endif //_CS_NAGRATIGER_INTERFACE_H_

