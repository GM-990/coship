/****************************************************************************************************************************
* Notice:         Copyright Shenzhen Coship Electronics Co., Ltd., 
*                    All rights reserved.
* Filename:     CS_CRYPT_Interface.h  
* Author:        caoxiaojing
* Date:           June 2012
* Description: Cryptguard CA interface
*            
*****************************************************************************************************************************/
#ifndef _CS_CRYPT_INTERFACE_H_
#define _CS_CRYPT_INTERFACE_H_

#include "udiplus_typedef.h"
#include "cs_cainterface.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum _CAMessage
{
	CA_ALARM,
	CA_COMMAND,
	CA_INFO,
	CA_STATUS
}CAMessage_E;

typedef enum _CAAlarm
{
	CA_ALARM_CANCEL,                

	CA_ALARM_SMC_OUT, 			      
	CA_ALARM_SMC_ERR, 			      

	CA_ALARM_NO_RIGHT, //无权限
	CA_ALARM_SRV_OTHER,//其他加扰节目		
	CA_ALARM_CHAL_LOCKED,//节目被锁定
	CA_ALARM_NO_PAIR,//机卡不匹配
	CA_ALARM_BLACK_OUT,//value rules,提示"This service is blocked"
	
	CA_ALARM_MAX	
}CAAlarm_E;

typedef enum  _CACommand
{
	CA_COMMAND_PINCHECK,//应用收到该消息后需要弹出PIN码校验框，输入PIN码后调用CSCACheckPinCode()
	CA_COMMAND_FINGERPRINT,//该消息附带参数dwParm2 =(DWORD)&CSCAFingerPrint_S
	CA_COMMAND_OSD,//该消息附带参数dwParm2 =(DWORD)&CAOSDMsg_S
	
	CA_COMMAND_MAX
}CACommand_E;

typedef enum  _CAInfo
{
	CA_INFO_SUBSCRIPTION_STATUS,//Subscription status,dwParm2 =(DWORD)&CAStatus_S
	CA_INFO_PPV_STATUS,//Pay_Per_View status,dwParm2 =(DWORD)&CAStatus_S
	CA_INFO_MAX
}CAInfo_E;

typedef enum _CAStatus
{
	CA_STATUS_SMC_IN, 	
	CA_STATUS_SMC_OUT,    
	CA_STATUS_SMC_OK,
	CA_STATUS_SMC_ERR, 

	CA_STATUS_PIN_RIGHT,//PIN码错误
	CA_STATUS_PIN_ERROR,//PIN码正确

	CA_STATUS_DSM_OK,//解扰成功

	CA_STATUS_MAX
}CAStatus_E;

//收到需要关闭音视频的ALARM消息时判断需要关闭的音视频类型，由dwParm3给出
typedef enum _CAAVStop
{
	CA_STOP_AV,      ///关闭音视频
	CA_STOP_AUDIO,   ///关闭音频
	CA_STOP_VIDEO,    ///关闭视频

	CA_STOP_MAX
}CAAVStop_E;

typedef struct  _CATime
{
	WORD   m_wYear;
	BYTE   m_ucMonth;
	BYTE   m_ucDay;
	BYTE   m_ucHour;
	BYTE   m_ucMinute;
}CATime_S;

typedef struct _CAFingerprint 
{
	BYTE   m_ucFPIRD; ///// 0: No IRD number display  // 1: Fingerprint IRD number display
	BYTE   m_ucFPCardNum;// 0: No cardnumber display // 1: Fingerprint Cardnumber display
	BYTE   m_ucFPRandom;// 0: Fingerprint placement is controlled by X and Y.// 1: Fingerprint placement should be randomized on each display.
	BYTE   m_ucFPRepeat;// 0: No repeat  // 其他值x: Repeat fingerprint displaying every x minutes
	BYTE   m_ucLocationX; // 0-15: Placement X cordinate    
    	BYTE   m_ucLocationY; // 0-15: Placement Y cordinate
	WORD   m_wDuration; // 0-65535: Duration in ms for display of fingerprint.
	BYTE   m_ucFPColor;// 0: Random fingerprint color. // 1: Fingerprint color Grey.// 2: Fingerprint color Black.// 3: Fingerprint color White.
						// 4: Fingerprint color Blue.// 5: Fingerprint color Green.// 6: Fingerprint color Red.// 7: Fingerprint color Yellow.
}CAFingerPrint_S;

typedef struct _CASMCInfo
{
	DWORD m_dwCardMainNumber;//Card main identification number
	DWORD m_dwCardAltNumber;//Alternative card  identification number
	WORD m_wCASystemID;//CA System ID
}CASMCInfo_S;

#define CA_MAX_OSD_LEN 255
typedef struct _CAOSDMsg
{
	BYTE  m_ucDisplayTime;//单位秒,0时一直显示
	BYTE  m_aucMsg[CA_MAX_OSD_LEN + 1];//字符串
} CAOSDMsg_S;

#define MAX_STATUS_LEN (1024)
typedef struct _CAStatusMsg
{
	WORD  m_wLen;
	BYTE  m_aucStatus[MAX_STATUS_LEN + 1];//字符串
} CAStatusMsg_S;//subscription status and ppv view status


#define STB_KEY_LEN 16
typedef struct _CASTBKeyInfo
{
	DWORD m_dwSTBID;
	BYTE m_aucSysGlobalKey[STB_KEY_LEN];
	BYTE m_aucManGlobalKey[STB_KEY_LEN];
	BYTE m_aucStbGroupKey[STB_KEY_LEN];
	BYTE m_aucStbUniqueKey[STB_KEY_LEN];
}CASTBKeyInfo_S;

/**************************************************************************
*Function Name: CSCASMCGetInfo
*
* 查询智能卡信息
*
*Parameters:
*	pstSMCInfo[out]:保存查询到的智能卡信息
*   		           
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCASMCGetInfo(CASMCInfo_S* pstSMCInfo);

/**************************************************************************
*Function Name: CSCACheckPinCode
*
* 校验智能卡PIN码
*
*Parameters:
*	pucPinCode[in]:输入的PIN码
*	ucLen[in]:输入的PIN码的长度		
*
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCACheckPinCode(BYTE *pucPinCode, BYTE ucLen);

/**************************************************************************
*Function Name: CSCAGetProtocolVersion
*
* 查询card  Protocol Version
*
*Parameters:
*	pwPrtlVer[out]:保存智能卡 Protocol Version
*   		           
*Return Values:
*	 0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCAGetProtocolVersion(WORD *pwPrtlVer);

/**************************************************************************
*Function Name: CSCAGetSubscriptionStatus
*
* 查询Subscription状态，收到消息CA_INFO_SUBSCRIPTION_STATUS后将参数中的字符串显示在界面上
*
*
*Parameters:
*	void
*   		           
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCAGetSubscriptionStatus(void);

/**************************************************************************
*Function Name: CSCAGetPPVStatus
*
* 查询PPV状态，收到消息CA_INFO_PPV_STATUS后将参数中的字符串显示在界面上
*
*
*Parameters:
*	void
*   		           
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCAGetPPVStatus(void);

/**************************************************************************
*Function Name:CSCAGetFlashAddress
*
* 获取主数据区的Flash 地址，应用实现
*
*Parameters: 
*	 dwSize[In]: 所需数据区的大小
*
*Return Values: 
*	一个连续区域的起始地址
**************************************************************************/
extern DWORD CSCAGetFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetBackupFlashAddress
*
*  获取备份数据区的Flash 地址(备份区与主写区一定不能在同一个Block)，应用实现
*
*Parameters: 
*	 dwSize[In]: 所需数据区的大小
*
*Return Values: 
*	一个连续区域的起始地址
**************************************************************************/
extern DWORD CSCAGetBackupFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetStbKeyInfo
*
* 获取当前机顶盒的ID及四个Key信息,每个机顶盒唯一,应用实现
*
*Parameters: 
*	psSTBKeyInfo [out]:保存机顶盒信息的结构体地址
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
extern INT CSCAGetStbKeyInfo(CASTBKeyInfo_S* psSTBKeyInfo);

#ifdef  __cplusplus
}
#endif
#endif //_CS_CRYPT_INTERFACE_H_

