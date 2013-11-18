/****************************************************************************************************************************
* Notice:         Copyright Shenzhen Coship Electronics Co., Ltd., 
*                     All rights reserved.
* Filename:     CS_DVNCA_Interface.h  
* Author:        PengShuangquan
* Date:           Mar 2007
* Description: DVNCA interface
*            
*****************************************************************************************************************************/

#include "udiplus_typedef.h"
#include "cs_cainterface.h"
#include "cs_cainterface_pvrext.h"

#ifndef _CS_DVNCA_INTERFACE_H_
#define _CS_DVNCA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

//@deprecated
typedef enum _CaFactionConfig
{
	CA_SUPPORT_NONE 	= 0x0,
	CA_AUTO_PAIRING 	= 0x01,	/*auto pairing enable*/
	CA_SUPPORT_IPPV 	= 0x02,	/*IPPV mode enable*/
	CA_CHECK_AREAONCE	= 0x04,	/*check area code once*/
	CA_NO_PAIRING  		= 0x08,	/*no need pairing enable*/
	CA_ENABLE_TWOWAY 	= 0x10 	/*two way ca enable*/
}E_CaFactionConfig;

//~ MESSAGE
typedef enum _CaEventGroup
{
	CA_ALARM,				 /*  0  */
	CA_INFO,				 /*  1  */ 
	CA_COMMAND,				 /*  2  */
	CA_STATUS,				 /*  3  */
	CA_MAX	
}E_CaEventGroup;

typedef enum _CaAlarm
{
	CA_ALARM_DVN_CA,                /*  0  Progran encrypt with DNV CA */
	CA_ALARM_OTHER,                 /*  1  Progran encrypt with other CA */
	CA_ALARM_FREE,                  /*  2  Program free */
	CA_ALARM_NEGATIVE_TOKEN,        /*  3  Negative token */
	CA_ALARM_TOKEN_NOT_ENOUGH,      /*  4  Token not enough */
	CA_ALARM_AREA_CODE_ERROR, 	    /*  5  Area code not match */
	CA_ALARM_ECM_BLACKOUT, 		    /*  6  ECM is blackout */
	CA_ALARM_CAN_BE_DECRYPTED,      /*  7  Can be decrypted */
	CA_ALARM_SUBSCRIPTION_EXPIRED,  /*  8  Subscription expired during encryptedvideo playback */
	CA_ALARM_SUCSCRIPTION_REMOVED,  /*  9  Subscription removed during encryptedvideo playback */
	CA_ALARM_STB_LOCKED,		    /*  10 STB locked during encrypted videoplayback */
	CA_ALARM_PROGRAM_CHECKSUM_ERROR,/*  11 Program checksum error */
	CA_ALARM_ECM_DECRYPTION_ERROR,	/*  12 Ecm decryption error */
	CA_ALARM_SMC_NOT_PAIRED, 		/*  13 Smart card not paire */
	CA_ALARM_SMC_OUT,				/*  14 smartcard removed */
	CA_ALARM_SMC_INVALID,			/*  15 smartcard invalid */
	CA_ALARM_SMC_CHECKING,			/*  16 smartcard checking*/
	CA_ALARM_SMC_IN,	            /*  17 smartcard in */
	CA_ALARM_AREA_CODE_OK, 	   		/*  18 Area code ok */
	CA_ALARM_SMC_OK,                /*  19 smartcard ok */
	CA_ALARM_SMC_ACTIVATION_EXPIRED,/*  20 smart card activation  expired */
	CA_ALARM_ECM_DATA_ERROR,    	/*  21 ECM data error */
	CA_ALARM_SMC_SECURED_CHANNEL,   /*  22 smart card data error secured channel */
	CA_ALARM_CANCEL,                /*  23 cancel alarm msg on the screen */
	CA_ALARM_MAX
}E_CaAlarm;

typedef enum _CaStuaus
{
	CA_STATUS_PAIRING_PREPARE,    	 /*  0 Pairing prepare */
	CA_STATUS_PAIRING_START,         /*  1 Pairing start */
	CA_STATUS_SYSTEM_PAIRING,        /*  2 System pairing */
	CA_STATUS_SYSTEM_UPDATE,         /*  3 System updating */
	CA_STATUS_SET_PASSWORD,          /*  4 Set password */
	CA_STATUS_CHECK_PASSWORD,        /*  5 Check password */
	CA_STATUS_WRITE_USER_INFO,       /*  6 Write user information */
	CA_STATUS_PAIRING_COMPLETE,	     /*  7 Pairing complete */
	CA_STATUS_PAIRING_ERROR,	     /*  8 Pairing error(time out, no new version) */
	CA_STATUS_SMC_IN,				 /*  9 Smc in */
	CA_STATUS_SMC_OUT, 				 /*  10 Smc out */ 
	CA_STATUS_SMC_OK,                /*  11 smartcard ok */
	CA_STATUS_SMC_READY,             /*  12 Cak init ok,we can communicate with CA now */
	CA_STATUS_MAX
}E_CaStuaus;

typedef enum _CaCommand
{
	CA_COMMAND_STB_ON,				 /* 0  dwParm2 = NULL */
	CA_COMMAND_STB_OFF,				 /* 1  dwParm2 = NULL */
	CA_COMMAND_TOKEN_CHANGE,         /* 2  dwParm2 = (DWORD)&T_CaTokenChange */
	CA_COMMAND_EMAIL,				 /* 3  dwPram2  = (DWORD)&T_CaEmail */
	CA_COMMAND_SHORT_MSG,			 /* 4  dwParm2 = (DWORD)&T_CaShortMsg */
	CA_COMMAND_SMCID_MSG,			 /* 5  dwParm2 = NULL; *///@deprecated
	CA_COMMAND_FINGER_PRINT,		 /* 6  dwPram2  = E_FingerPrint; */  
	CA_COMMAND_RATING_CONTROL,		 /* 7  dwPram2  = Parent Rate*/
	CA_COMMAND_RATING_CONTROL_UNLOCK,/* 8  dwPram2 = NULL */
	CA_COMMAND_UPLOAD,				 /* 9  Reserver   */
	CA_COMMAND_OS_UPDATE,			 /* 10  Reserver   */
	CA_COMMAND_REBOOT,				 /* 11  dwPram2 = 0 App should reboot the STB */
	CA_COMMAND_MAX
}E_CaCommand;

typedef enum _FingerPrint
{
	E_FINGER_HIDE,
	E_FINGER_SHOW,	/*App Should display SMC ID  */
	E_FINGER_MAX
}E_FingerPrint;

typedef struct  _CaDate
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
}T_CaDateTime;

#define CA_MAX_SIZE_TXT_MAIL       (1024)

typedef enum _TxtType
{
	CA_TXT_MAIL         	= 0,/*Text -- Mailbox,	邮件消息 */
	CA_TXT_ANNOUNCE     	= 1,/*Text -- Announcement,广播通知 */
	CA_TXT_CLUB         	= 2 /*Club number,会员号 */
}T_TxtType;

typedef struct _CaMsg		/* 邮件属性 */
{
	BYTE    	m_cType;     			/*T_TxtType// 0: Text_Mail  1: Text_Announcement  */
	DWORD   	m_dwLen;
	BYTE    	m_szContent[CA_MAX_SIZE_TXT_MAIL];
}T_CaMsg;

typedef struct _CaMail		/*邮件结构体 */
{	
    BOOL          m_bReadFlag;    /*已读标志 */
    BOOL          m_bLockFlag;	  /*邮件锁定标志 */
    T_CaDateTime  m_stArriveTime; /*邮件到达的时间 */
    T_CaMsg       m_stMsg;
}T_CaMail;

#define MAX_SHORT_MSG_LEN (256)

typedef struct _CaShortMsg
{
    INT           m_nlen;	
    BYTE         m_acMessage[MAX_SHORT_MSG_LEN];
}T_CaShortMsg;

typedef struct _CaFtaSubscriptionService
{							  
	DWORD		   m_dwProductID;      		/*defined by the head-end system. */
	DWORD		   m_dwEventID;             /*Event ID (reserved) */
	T_CaDateTime   m_stStartTime;           /*Start Time */
	DWORD          m_dwReserved1;           /*Reserved */
	T_CaDateTime   m_stEndTime;             /*End Time */
	DWORD          m_dwReserved2;           /*Reserved */
}T_CaFtaSubscriptionService;

#define MAX_CONFIRMCODE_LEN (17)

typedef struct _CaFtaTransactionRecord
{
	DWORD		   m_dwType;
	DWORD          m_dwID;
	float          m_fPrice;
	T_CaDateTime   m_stDateTime;
	BYTE          m_acConfirmCode[MAX_CONFIRMCODE_LEN];
}T_CaFtaTransactionRecord;

typedef struct _DvnTokenChange
{
	float           m_fTokenChange;			/*Value to be added or deduct from current token value */
	INT             m_nOperationID;			/*Unique reference ID for the operation */
}T_CaTokenChange;

#define CA_MAX_COUNTRY_NUM         (20)

typedef struct _FtaCountry
{
	BYTE    m_cFlag;       		/*可用标志：1：可用；0：不可用 */
	BYTE    m_acCountry[4]; 	/*国家可用代码 */
}T_FtaCountry;

typedef struct _FtaCountryList
{
	WORD    		m_wCountryNum;    /*结构中国家代码个数 */
	T_FtaCountry  	m_stCountryList[CA_MAX_COUNTRY_NUM]; /*存放的国家代码 */
}T_FtaCountryList;

/**************************************************************************
*Function Name: CS_CA_GetVersion
*   获取CA版本信息
*
*Parameters
*   pacVerBuf[out]:指向存储CA版本信息的空间
*   dwBufSize[in]:给定一个最大长度值.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CA_GetCoreVersion
*   获取DVN 内核版本
*
*Parameters
*   pacVerBuf[out]:指向存储CA版本信息的空间
*   dwBufSize[in]:给定一个最大长度值.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetCoreVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
* Function Name: CS_CA_SetDebugMode
*   设置CA功能配置，注意必须在调用CS_CA_Init之前调用
*
*Parameters	
*	  	nMode[in] 功能模式，见E_CaFactionConfig
*Return Values: 
*	@deprecated
**************************************************************************/
void CSCASetDvnConfiguration(INT nMode);

/**************************************************************************
*Function Name:CS_CA_TransactionComfirm
* 确定是否交易
*
*Parameters 
*	bFlag:是否交易，TRUE 确定 ,FALSE, 取消
* 	nIndex: 交易号
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*	@deprecated
**************************************************************************/
INT CS_CA_TransactionComfirm(BOOL bFlag,INT nIndex);

/**************************************************************************
* Function Name: CS_CA_PerformPairing
*   机卡配对
*
*Parameters	
*
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_PerformPairing(void);

/**************************************************************************
* Function Name: CS_CA_GetInfo_STBAttribute
*   获取STB ID
*
*Parameters	
*	  pcStbIDBuf
*            [out] STB ID数据
*       nBufSize
*            [in] 传入的Buf长度
*	 pnStbIDWidth
*		[out] STB ID数据长度
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetInfo_STBAttribute(BYTE* pcStbIDBuf, INT nBufSize);

/**************************************************************************
* Function Name: CS_CA_GetInfo_SmcID
*   获取SMC ID
*
*Parameters	
*	  pcSmcIDBuf
*            [out] SMC ID数据
*       nBufSize
*            [in] 传入的Buf长度
*	 pnSmcIDWidth
*		[out] SMC ID数据长度
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetInfo_SmcID(BYTE* pcSmcIDBuf, INT nBufSize);

/**************************************************************************
* Function Name: CS_CA_GetInfo_AccountNo
*   获取帐户号
*
*Parameters	
*	  pcAccountNoBuf
*            [out] Accout No数据
*       nBufSize
*            [in] 传入的Buf长度
*	 pnAccountNoWidth
*		[out] Accout No数据长度
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetInfo_AccountNo(BYTE* pcAccountNoBuf, INT nBufSize);

/**************************************************************************
* Function Name: CS_CA_GetInfo_SysDescriptor
*   获取指定序号系统描述符
*
*Parameters	
*	  nSysDescriptorIndex
* 	  	[in] 描述符序号(0~4)
*	  pcSysDescriptorBuf
*            [out] 描述符数据
*       nBufSize
*            [in] 传入的Buf长度
*	 pnSysDescriptorWidth
*		[out] 描述符数据长度
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetInfo_SysDescriptor(INT nSysDescriptorIndex, BYTE* pcSysDescriptorBuf, INT nBufSize);

/**************************************************************************
* Function Name: CS_CA_TokenGetValue
*   获取帐户余额
*
*Parameters	
*	       pfTokenValue
*                   [out]   余额
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_TokenGetValue(float* pfTokenValue);

/**************************************************************************
* Function Name: CS_CA_TokenConfirmChange
*   确认帐户金额变化
*
*Parameters	
*	       nOperationID
*                   [in]   索引号  (默认值0)
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_TokenConfirmChange(BOOL bFlag,INT nOperationID);

/**************************************************************************
* Function Name: CS_CA_TokenDeduct
*   扣除钱包金额
*
*Parameters	
*	       fTokenValue
*                   [in]   某个节目/服务的价格
*            nServiceType
*                   [in]   节目/服务的类型
* 			nOperationID
*					[in]   节目/服务的ID
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_TokenDeduct(float fTokenValue, INT nServiceType, INT nOperationID);

/**************************************************************************
* Function Name: CS_CA_GetSubscriptionRecordNum
*   获取购买记录数目
*
*Parameters
*	       pnLocalNum
*                   [out]   购买本地运营商节目记录总数
*            pnCenterNum
*                   [out]   购买CCTV运营商节目记录总数
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetSubscriptionRecordNum( INT *pnLocalNum,INT *pnCenterNum );

/**************************************************************************
* Function Name: CS_CA_GetSubscriptionRecordContent
*   获取指定序号购买记录的内容
*
*Parameters	
*	       nRecordIndex
*                   [in]   序号(从0开始)
*            pstSubRecord
*                   [out]  记录内容
*            nSubRecordType
*                   [in]  购买记录类型:0 CCTV运营商， 1 本地运营商
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetSubscriptionRecordContent(INT nRecordIndex, INT nSubRecordType, T_CaFtaSubscriptionService* pstSubRecord);

/**************************************************************************
* Function Name: CS_CA_GetTransactionRecordNum
*   获取交易记录总数
*
*Parameters
*
*Return Values
*		成功返回记录总数，失败返回FAILURE
**************************************************************************/
INT CS_CA_GetTransactionRecordNum(void);

/**************************************************************************
* Function Name: CS_CA_GetTransactionRecordContent
*   获取指定序号交易记录的内容
*
*Parameters	
*	       nIndex
*                   [in]   序号(从0开始)
*            pstTracRecord
*                   [out]  记录内容
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetTransactionRecordContent(INT nIndex, T_CaFtaTransactionRecord* pstTracRecord);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* 保存一封邮件
*
*Parameters 
*	pstTxt[in]:邮件内容
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Add(T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Del
* 删除某一封邮件
*
*Parameters 
*	nIndex[in]:邮件下标，从0开始
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
*Parameters 
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
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* 获取具体某一封邮件的内容
*
*Parameters 
*	nIndex[in]:邮件下标，从0开始
*	ppstTxt[out]:返回邮件的存储地址
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, T_CaMail* ppstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNew
* 查询某一类邮件是否有未读邮件
*
*Parameters 
*	eType[in]:邮件类型
*
*Return Values: 
*	1: TRUE 
*	0: FALSE
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(T_TxtType eType);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* 获取未读邮件个数
*
*Parameters 
*	无
*
*Return Values: 
*	返回未读邮件个数
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* 将所有邮件保存到flash中，一般是关机或断电前进行的操作
*
*Parameters 
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
*Parameters 
*	nIndex[in]:邮件存储的下标，从0开始
*	bRead[in]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_SetLockFalg
* 设置邮件是否锁定
*
*Parameters 
*	nIndex[in]:邮件存储的下标，从0开始
*	bLockFlag[in]:已加锁(TRUE),未加锁(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_SetLockFalg(DWORD dwIndex, BOOL bLockFlag);

/**************************************************************************
*Function Name:CS_CA_TXT_GetLockFalg
* 得到指定邮件的锁定状态
*
*Parameters 
*	nIndex[in]:邮件存储的下标，从0开始
*	pbLockFlag[out]:已加锁(TRUE),未加锁(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_GetLockFalg(DWORD dwIndex, BOOL* pbLockFlag);

/**************************************************************************
*Function Name:CS_CA_GetSmcStatus
* 获取智能卡当前状态
*
*Parameters 
*	pcSmcStatus[In/Out]:存储智能卡状态，如果返回值为0，则卡OUT，为1则卡IN
*
*Return Values: 
*	SUCCESS 获取智能卡当前状态成功 
*	FAILURE 输入参数错误
**************************************************************************/
INT CS_CA_GetSmcStatus(BYTE* pcSmcStatus);

/**************************************************************************
*Function Name:CS_CA_GetRating
* 获取成人级别
*
*Parameters 
*	pcSmcStatus[In/Out]:pdwPGLevel
*
*Return Values: 
*	SUCCESS :函数返回成功
*	FAILURE: 函数返回失败
**************************************************************************/
INT CS_CA_GetRating(DWORD* pdwPGLevel);

/**************************************************************************
*Function Name:CS_CA_SetRating
* 设置机顶盒成人级别
*
*Parameters 
*	pcSmcStatus[In/Out]:pdwPGLevel 0~15 ;
*
*Return Values: 
*	SUCCESS :函数返回成功
*	FAILURE: 函数返回失败

**************************************************************************/
INT CS_CA_SetRating(DWORD pdwPGLevel);

/**************************************************************************
*Function Name:CS_CA_GetSmcStatus
* 获取智能卡版本
*
*Parameters 
*	pucVersion[In/Out]:存储smartcard版本，需要3个字节来存储，且格式为ASCII码
*
*Return Values: 
*	SUCCESS 获取智能卡当前版本成功 
*	FAILURE 输入参数错误
**************************************************************************/
INT CSCAGetSmartcardVersion(BYTE * pucVersion);

/**************************************************************************
*Function Name: CS_CA_SetAreaBlock
*  设置区域受限：解析NIT表，填充结构体
*
*Parameters 
*	pstCountypList[in]:国家代码个数和国家代码号
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_SetAreaBlock(T_FtaCountryList* pstCountypList);

/**************************************************************************
*Function Name: CS_CA_GetPairedStatus
*  获取智能卡跟机顶盒的配对状态
*
*Parameters 
*	pbPaired[In/Out]:配对状态，=1已配对，=0未配对
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetPairedStatus(BOOL *pbPaired);

/**************************************************************************
*Function Name:CS_CA_GetFlashAddress
*  获取CA需要FLASH区域的起始地址，CA模块依赖接口，项目集成时实现此函数供CA模块调用
*
*Parameters 
*	dwFlashSize: 指定要分配FLASH块的大小
*
*Return Values:
*	返回分配的起始地址，最好是分配一个Block的Flash，忽略参数dwFlashSize
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(DWORD dwFlashSize);

/**************************************************************************
*Function Name:CS_CA_GetBackupFlashAddress
*  获取CA需要备份FLASH区域的起始地址，CA模块依赖接口，项目集成时实现此函数供CA模块调用
*
*Parameters 
*	dwFlashSize: 指定给备份区域分配FLASH块的大小
*
*Return Values:
*	返回分配的起始地址，最好是分配一个Block的Flash，忽略参数dwFlashSize
**************************************************************************/
extern DWORD CS_CA_GetBackupFlashAddress(DWORD dwFlashSize);

#ifdef  __cplusplus
}
#endif

#endif /*_CS_DVNCA_INTERFACE_H_ */

