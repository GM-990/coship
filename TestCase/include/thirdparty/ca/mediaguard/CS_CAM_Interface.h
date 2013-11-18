/*****************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*                 All rights reserved.
* Filename:    CS_CAM_Interface.h  
* Author:       Xiejianghua
* Date:          2006-10 
* Description: MediaGuard CA Interface
*            
******************************************************************************/
#include "udiplus_typedef.h"

/*****************************************************************************
* 			                Revision History                                                                          *
******************************************************************************
* - Revision 1.0  2006/10/31
*   Programmer:Xiejianghua
*   Create.

******************************************************************************/
#ifndef _CS_CAM_INTERFACE_H_
#define _CS_CAM_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define	MAX_OPERATOR_NUM         (64)
#define MAX_MAIL_DATA_LENGTH     (256)

typedef enum _CaMessage
{
	CA_ALARM,
	CA_COMMAND,
	CA_STATUS,
	CA_INFO,
	CA_MMI
}T_CaMessage;

typedef enum _CaAlarm
{
	CA_ALARM_PROGRAM_ACCESS_OK,        /*0节目正常解扰*/
	CA_ALARM_PROGRAM_PREVIEW,          /*1节目预览*/
	CA_ALARM_PROGRAM_EXPIRED,          /*2节目授权过期*/
	CA_ALARM_NO_CARD,                  /*3没有智能卡*/
	CA_ALARM_ERROR_CARD,               /*4错误的智能卡*/
	CA_ALARM_CARD_OK,                  /*5正确的智能卡*/
	CA_ALARM_UNKNOWN_OPERATOR,         /*6未知运营商*/
	CA_ALARM_UNKNOWN_OFFER,            /*7未知服务提供商*/
	CA_ALARM_GEOGRAPHICAL_BLACKOUT,    /*8区域禁播*/   
	CA_ALARM_RATING_LIMITED,           /*9父母级限制*/
	CA_ALARM_NO_CREDIT,                /*10余额不足*/
	CA_ALARM_NO_RIGHTS,                /*11 节目没有授权dwParm2 确定种类
												dwParm2 = 0:A/V 都没授权；
												dwParm2 = 1:音频没授权；
												dwParm2 = 2:视频没授权
										*/
	CA_ALARM_NOT_PAIRED,               /*12机卡未配对*/
	CA_ALARM_FREE_PROGRAME,            /*13清流*/
	CA_ALARM_OTHER_CA_ENCRYPT,         /*14非MG CA加扰节目*/
	CA_ALARM_MG_CA_ENCRYPT,            /*15MG CA 加扰节目*/
	CA_ALARM_PAIRED,                   /*16机卡配对*/	
	CA_ALARM_MAX
}T_CaAlarm;

typedef enum _CaCommand
{
	CA_COMMAND_INHIBIT_STB,
	CA_COMMAND_SHOW_TEXT,
	CA_COMMAND_NEW_EMAIL,				//dwParm2 = (DWORD)&T_CaEmail
	CA_COMMAND_OTA,						//dwparm2 = (DWORD)&T_CAOtaParameter
	CA_COMMAND_RESETPIN,
	CA_COMMAND_MAX
}T_CaCommand;

typedef enum _Castatus
{
	CA_STATUS_SRV_FREE,	              //免费节目
	CA_STATUS_SRV_OTHER,              //其他CA加桡节目
	CA_STATUS_CARD_IN,
	CA_STATUS_CARD_OUT,
	CA_STATUS_SRV_RIGHT,             //NVOD等服务有权限
	CA_STATUS_SRV_NO_RIGHT,          //NVOD等服务没权限
	CA_STATUS_PAIRED,
	CA_STATUS_NOT_PAIRED,
     	CA_STATUS_MAX
}T_CaStatus;

typedef enum _CaInfo
{
	CA_INFO_CA_LIB_VERSION,				//dwparm2 = (DWORD)&T_CaLibVersion
	CA_INFO_CARD_ID,						//dwparm2 = (DWORD)dwCardId
	CA_INFO_RATING,						//dwparm2 = (DWORD)dwRating
	CA_INFO_OPERATOR_INFO,				//dwparm2 = (DWORD)&T_CaOperatorInfoArray
     	CA_INFO_MAX
}T_CaInfo;

typedef enum _CaMmi
{
	CA_MMI_PIN_OK,						//Check Pin ==> PIN OK
	CA_MMI_PIN_ERROR,					//Chcek Pin ==> PIN Error

	CA_MMI_CHANGE_PIN_SUCCESS,			//Change Pin SUCCESS
	CA_MMI_CHANGE_PIN_FAIL,				//Change Pin Fail

	CA_MMI_ENABLE_RATING_SUCCESS,		//Enable Rating check SUCCESS
	CA_MMI_ENABLE_RATING_FAIL,			//Enable Rating check FAIL

	CA_MMI_DISABLE_RATING_SUCCESS,		//Disable Rating check SUCCESS
	CA_MMI_DISABLE_RATING_FAIL,			//Disable Rating check FAIL

	CA_MMI_SET_RATING_SUCCESS,			//Set Rating SUCCESS
	CA_MMI_SET_RATING_FAIL				//Set Rating FAIL

}T_CaMmi;

typedef struct
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
}T_CaDate;

typedef struct _CaLibVersion					/*MediaGuard LIB 版本*/
{
	DWORD	m_dwCoreLibVersionLength;
	DWORD	m_dwPairingLibVersionLength;
	BYTE	m_acCoreLibVersion[15];
	BYTE	m_acPairingLibVersion[15];
}T_CaLibVersion;

typedef struct _CaOperatorInfo
{
	BYTE		m_acOperatorName[16];
	BYTE		m_acOffers[8];
	DWORD		m_dwOperatorId;
	DWORD		m_dwGeographicalId;
	T_CaDate	m_stDate;     					/* 年、月、日有效*/
	DWORD       m_dwDate; //add at 2008-04-03
}T_CaOperatorInfo;

typedef struct _CaOperatorInfoArray			/*所有运营商信息*/
{
	DWORD			m_dwOperatorNum;		/*运营商信息个数*/
	T_CaOperatorInfo	m_stOperatorInfo[MAX_OPERATOR_NUM];
}T_CaOperatorInfoArray;

typedef struct _CaOtaParameter		/*OTA 升级数据*/
{
	BYTE	m_cNewSwVerMajor;
	BYTE	m_cNewSwVerMinor;
	BYTE	m_cModulation;			/*0:undefine;  1:16QAM;  2: 32QAM;  3: 64QAM;  4:128QAM;  5: 256QAM*/
	WORD	m_wDownLoadPid;
	WORD	m_wSymbRate;
	WORD     m_wVersionNum;			/*CDT 表版本号*/
	DWORD	m_dwBackUpFrequency;
	DWORD	m_dwMainFrequency;
}T_CAOtaParameter;

typedef enum _CaEmailSort				/*邮件排序的方式*/
{
	CA_EMAIL_TIME_DESC,					/*创建时间早的排在后面*/	
	CA_EMAIL_READSTATUS_DESC,			/*已读的排在后面*/
	CA_EMAIL_COMSTATUS_DESC,			/*已读的且创建时间早的排在后面*/
	
	CA_EMAIL_SORT_MAX
}T_CaEmailSortStyle;

typedef struct							/*邮件*/
{
	BOOL  			m_bNewEmail;
	BYTE			m_cEmailId;
	BYTE			m_cTitleLen;
	BYTE			m_cSenderLen;
	BYTE			m_cContentLen; 	
	BYTE			m_acEmailData[MAX_MAIL_DATA_LENGTH];	
	/*邮件格式为 Title  0 Sender 0 Content 即三个字符串以0 间隔
	   若m_cTitleLen = 0;  则m_acEmailData 为Sender 0 Content      */
	T_CaDate		m_stDate;	
}T_CaEmail;


/**************************************************************************
* Function Name: CS_CA_Restart
*   重新启动CA服务
*
*Parameters	
*		
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_Restart(void);

/**************************************************************************
* Function Name: CS_CA_ResetAudioPid
*   重新设置音频PID
*
*Parameters	
*		wAudioPid
*			音频PID
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_ResetAudioPid(WORD		wAudioPid);


/**************************************************************************
* Function Name: CS_CA_MMI_ChangePin
*   修改智能卡密码
*
*Parameters	
*		dwOldPin[in] 
*			旧密码
*       	dwNewPin[in] 
*			新密码
*Return Values:SUCCESS
*	结果会通过CA_MMI_CHANGE_PIN_SUCCESS或CA_MMI_CHANGE_PIN_FAIL上报
**************************************************************************/
INT CS_CA_MMI_ChangePin(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
* Function Name: CS_CA_MMI_CheckPin
*   校验智能卡密码
*
*Parameters	
*		dwPinCode[in] 
*			要校验的密码
*Return Values:SUCCESS
*	结果会通过CA_MMI_PIN_OK或CA_MMI_PIN_ERROR上报
**************************************************************************/
INT CS_CA_MMI_CheckPin(DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestCaLibVersion
*   请求获取MediaGuard CA库版本
*
*Parameters	
*
*Return Values:SUCCESS
*	数据会通过CA_INFO_CA_LIB_VERSION
**************************************************************************/
INT CS_CA_MMI_RequestCaLibVersion(void);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestPairStatus
*   请求获取机卡配对状态
*
*Parameters	
*
*Return Values:SUCCESS
*	数据会通过CA_INFO_PAIR_STATUS
**************************************************************************/
INT CS_CA_MMI_RequestPairStatus(void);

/**************************************************************************
* Function Name: CS_CA_MMI_EnableRatingLock
*   打开父母级控制
*
*Parameters	
*		dwPinCode[in] 
*			智能卡密码
*Return Values:SUCCESS
*	结果会通过CA_MMI_ENABLE_RATING_SUCCESS或CA_MMI_ENABLE_RATING_FAIL上报
**************************************************************************/
INT CS_CA_MMI_EnableRatingLock(DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_MMI_DisableRatingLock
*  关闭父母级控制
*
*Parameters	
*		dwPinCode[in] 
*			智能卡密码
*Return Values:SUCCESS
*	结果会通过CA_MMI_DISABLE_RATING_SUCCESS或CA_MMI_DISABLE_RATING_FAIL上报
**************************************************************************/
INT CS_CA_MMI_DisableRatingLock(DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_MMI_SetRating
*  设置父母级别
*
*Parameters	
*		dwRating[in] 
*			要设置的父母级
*		dwPinCode[in] 
*			智能卡密码
*Return Values:SUCCESS
*	结果会通过CA_MMI_SET_RATING_SUCCESS或CA_MMI_SET_RATING_FAIL上报
**************************************************************************/
INT CS_CA_MMI_SetRating(DWORD dwRating, DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestRating
*   请求获取父母级别
*
*Parameters	
*
*Return Values:SUCCESS
*	数据会通过CA_INFO_RATING
**************************************************************************/
INT CS_CA_MMI_RequestRating(void);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestCardId
*   请求获取智能卡号
*
*Parameters	
*
*Return Values:SUCCESS
*	数据会通过CA_INFO_CARD_ID
**************************************************************************/
INT CS_CA_MMI_RequestCardId(void);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestOperatorInfo
*   请求获取运营商信息
*
*Parameters	
*
*Return Values:SUCCESS
*	数据会通过CA_INFO_OPERATOR_INFO
**************************************************************************/
INT CS_CA_MMI_RequestOperatorInfo(void);

/**************************************************************************
*Function Name:CS_CA_MMI_GetSmcStatus
* 获取智能卡当前状态
*
*Parameters 
*	pcSmcStatus[In/Out]:存储智能卡状态，如果返回值为0，则卡OUT，为1则卡IN
*
*Return Values: 
*	SUCCESS 获取智能卡当前状态成功 
*	FAILURE 输入参数错误
**************************************************************************/
INT CS_CA_MMI_GetSmcStatus(BYTE* pcSmcStatus);

/**************************************************************************
* Function Name: CS_CA_SetSortEmailStyle
*   设置邮件的排序和删除类型，在CA初始化时设置
*   排序缺省值(未通过该函数设置)为: 创建时间早的排在后面
*   邮箱满时删除类型缺省值(未通过该函数设置):删除创建时间早的邮件
*
*Parameters	
*	eEmailSortStyle[In]:邮件的排序类型
*	eEmailDeleteStyle[In]:删除邮件的类型
*
*Return Values:
*		SUCCESS:设置排序类型成功
*		FAILURE:传入的参数不正确
**************************************************************************/
INT CS_CA_SetSortEmailStyle(T_CaEmailSortStyle eEmailSortStyle, T_CaEmailSortStyle eEmailDeleteStyle);

/**************************************************************************
* Function Name: CS_CA_GetEmailTotalNum
*   获取邮件总数
*
*Parameters	
*
*Return Values:当前邮件总数
**************************************************************************/
INT CS_CA_GetEmailTotalNum(void);

/**************************************************************************
* Function Name: CS_CA_GetEmailInfo
*   获取指定序号邮件内容
*
*Parameters	
*		dwIndex[in]
*			邮件序号(从1开始)
*		pstMsg[out]
*			邮件内容
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetEmailInfo(DWORD dwIndex/*从1开始*/, T_CaEmail* pstMsg);

/**************************************************************************
* Function Name: CS_CA_SetEmailReadFlag
*   设置指定邮件为已读
*
*Parameters	
*		dwIndex[in]
*			邮件序号(从1开始)
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_SetEmailReadFlag(DWORD dwIndex/*从1开始*/);

/**************************************************************************
* Function Name: CS_CA_SetEmailReadFlagEx
*   扩展CS_CA_SetEmailReadFlag 函数，根据EmailId 设置
*   指定邮件为已读
*
*Parameters	
*		cEmailId[in]
*			邮件Id
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_SetEmailReadFlagEx(BYTE  cEmailId);

/**************************************************************************
* Function Name: CS_CA_DelEmail
*   删除邮件
*
*Parameters	
*		dwIndex[in]
*			邮件序号(=0:删除所有邮件；>0:删除对应序号邮件)
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_DelEmail(DWORD dwIndex/*从1开始，0: 删除全部*/);

/**************************************************************************
* Function Name: CS_CA_SaveEmailChange
*   保存对邮件的修改
*
*Parameters	
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_SaveEmailChange(void);

/**************************************************************************
* Function Name: CS_CA_CheckServiceRight
*   获取APP进入Service 的权限( 歌华特殊需求)
*
*Parameters	
*Return Values
**************************************************************************/
BOOL CS_CA_CheckServiceRight(WORD wServiceId, DWORD dwPid);

/**************************************************************************
* Function Name: CS_CA_StartCdtMonitor
*   开始CDT 监听( 歌华特殊需求)
*
*Parameters	
*		dwPid[in]
*			CDT表的Pid
*		wVersionNum[in]
*			CDT表的版本号*			
*Return Values
*		SUCCESS. 
**************************************************************************/
INT CS_CA_StartCdtMonitor(DWORD dwPid, WORD wVersionNum);/* wVersionNUm = 0xFF 时不判断版本号*/

/**************************************************************************
* Function Name: CS_CA_StartSmtMonitor
*   开始SMT 监听( 歌华特殊需求)
*
*Parameters	
*		dwPid[in]
*			SMT表的Pid
*		wVersionNum[in]
*			SMT表的版本号
*Return Values
*		SUCCESS. 
**************************************************************************/
INT CS_CA_StartSmtMonitor(DWORD dwPid, WORD wVersionNum);/* wVersionNUm = 0xFF 时不判断版本号*/

/**************************************************************************
* Function Name: CS_CA_StopCdtMonitor
* 停止CDT 监听( 歌华特殊需求)
*
*Parameters	

*Return Values
*		SUCCESS. 
**************************************************************************/
INT CS_CA_StopCdtMonitor(void);

/**************************************************************************
* Function Name: CS_CA_StopSmtMonitor
* 停止SMT  监听( 歌华特殊需求)
*
*Parameters	

*Return Values
*		SUCCESS. 
**************************************************************************/
INT CS_CA_StopSmtMonitor(void);

/**************************************************************************
* Function Name: CS_CA_GetFlashAddress
*   CA获取FLASH空间地址(由APP实现)
*
*Parameters	
*Return Values
*		分配给CA的FLASH空间起始地址,大小为5K
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(void);

/**************************************************************************
* Function Name: CS_CA_GetCALibFlashAddress
*   CA获取FLASH空间地址(由APP实现)
*
*Parameters	
*Return Values
*		分配给CA Lib的FLASH空间起始地址,大小为8K
**************************************************************************/
extern DWORD CS_CA_GetCALibFlashAddress(void);

/**************************************************************************
* Function Name: CS_CA_EmailReadFlash
*   CA 读FLASH (由APP实现) ( 歌华特殊需求)
*
*Parameters	
*		dwStartAddress[in]
*				FLASH 的起始地址
*		pcData[in]
*				读出数据的地址
*		dwSize[in] 
*				读出数据的长度
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*		
**************************************************************************/
extern INT CS_CA_EmailReadFlash(DWORD dwStartAddress, BYTE* pcData, DWORD dwSize);

/**************************************************************************
* Function Name: CS_CA_EmailWriteFlash
*   CA 写FLASH (由APP实现) ( 歌华特殊需求)
*
*Parameters	
*		dwStartAddress[in]
*				FLASH 的起始地址
*		pcData[in]
*				写入数据的地址
*		dwSize[in] 
*				写入数据的长度
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*
**************************************************************************/
extern INT CS_CA_EmailWriteFlash(DWORD dwStartAddress, BYTE* pcData, DWORD dwSize);

typedef struct _StbInfo
{
	BYTE	m_cManufacturorId;
	BYTE	m_cHardwareId;
	BYTE	m_cSWVerMajor;
	BYTE	m_cSWVerMinor;
	BYTE	m_acSN[8];
	
}T_StbInfo;

/**************************************************************************
* Function Name: CS_CA_GetSTBInfo
*   CA获取机顶盒相关信息(由APP实现)
*
*Parameters	
*		pStbInfo[out]
*			机顶盒信息
*Return Values
*		SUCCESS. 
**************************************************************************/
extern INT CS_CA_GetSTBInfo(T_StbInfo* pStbInfo);

/**************************************************************************
* Function Name: CS_CA_GetCurrentDateTime
*   CA获取当前码流时间(由APP实现)
*
*Parameters	
*		pDatetime[out]
*			码流时间
*Return Values
*		SUCCESS. 
**************************************************************************/
extern INT CS_CA_GetCurrentDateTime(T_CaDate* pDatetime);

/**************************************************************************
* Function Name: CS_CA_TunerChangeNotify
*   通知CA Tuner状态发生改变
*Parameters	
*		eCaTunerStatus[in]
*			Tuner的状态变化
*Return Values
*		FAILURE
*		SUCCESS. 
**************************************************************************/
typedef enum _CaTunerStatus
{
	CA_TUNER_CONNECTED,
	CA_TUNER_DISCONNECTED,
	CA_TUNER_CHANGED,

	CA_TUNER_MAX
}T_CaTunerStatus;

INT CS_CA_TunerChangeNotify(T_CaTunerStatus eCaTunerStatus);


#ifdef  __cplusplus
}
#endif

#endif 

