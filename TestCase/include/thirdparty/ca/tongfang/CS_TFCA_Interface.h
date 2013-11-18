/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_TFCA_Interface.h  
* Author:      Xiejianghua
* Date:        Mar 2006 
* Description: TF CA interface
*            
*****************************************************************************************************************************/
#include "udiplus_typedef.h"
#include "cs_cainterface.h"

/****************************************************************************************************************************
* 			                Revision History                                *
*****************************************************************************************************************************
* - Revision 1.0  2006/3/23
*   Programmer:Xiejianghua
*   Create.
*****************************************************************************************************************************/
#ifndef _CS_TFCA_INTERFACE_H_
#define _CS_TFCA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum _CaMessage
{
	CA_COMMAND,
	CA_ALARM,
	CA_STATUS,
	CA_INFO,
	CA_MMI
}T_CaMessage;

typedef enum  _CaCommand
{

	//新邮件到达
	CA_COMMAND_NEW_EMAIL,			 // dwParm2 = (DWORD)T_CaEmailIconType

	//指纹
	CA_COMMAND_FINGERPRINT,			//dwParm2 = (DWORD)CardId

	//显示OSD信息
	CA_COMMAND_SHOW_OSD,			//dwParm2 = (DWORD)&T_CaOsd
	//隐藏OSD信息
	CA_COMMAND_HIDE_OSD,			//dwParm2 = (WORD)T_CaOsdStyle

	//IPPV购买信息
	CA_COMMAND_IPPV_NOTIFY,			//dwParm2 = (DWORD)&T_CaIppvNotify
	//取消IPPV购买提示
	CA_COMMAND_IPPV_CANCEL,			//dwParm2 = (DWORD)EcmPid

	CA_COMMAND_MAX
}T_CaCommand;

typedef enum _CaAlarm
{	
	CA_ALARM_CANCEL = 0,       /* 0 取消当前的显示*/
	CA_ALARM_BADCARD,          /* 1 无法识别卡，不能使用*/
	CA_ALARM_EXPICARD,         /* 2 智能卡已经过期，请更换新卡*/
	CA_ALARM_INSERTCARD,       /* 3 加扰节目，请插入智能卡*/
	CA_ALARM_NOOPER,           /* 4 卡中不存在节目运营商*/
	CA_ALARM_BLACKOUT,         /* 5 条件禁播*/
	CA_ALARM_OUTWORKTIME,      /* 6 不在工作时段内*/
	CA_ALARM_WATCHLEVEL,       /* 7 节目级别高于设定观看级别*/
	CA_ALARM_PAIRING,          /* 8 节目要求机卡对应*/
	CA_ALARM_NOENTITLE,        /* 9 没有授权*/
	CA_ALARM_DECRYPTFAIL,      /* 10 节目解密失败*/
	CA_ALARM_NOMONEY,          /* 11 卡内金额不足*/
    CA_ALARM_ERRREGION,        /* 12 区域不正确*/
    CA_ALARM_MAX
}T_CaAlarm;

typedef enum _CaStatus
{
	CA_STATUS_SMC_IN,
	CA_STATUS_SMC_OUT,
	CA_STATUS_DSM_OK,
	CA_STATUS_SRV_FREE,
	CA_STATUS_SRV_OTHER,
	CA_STATUS_MAX
}T_CaStatus;

#define MAX_OPERATOR_NUM       (4)         /*最多的运营商个数*/
typedef struct _CaOperatorId               /*运营商ID*/
{
	WORD	m_wOperatorIdNum;
	WORD	m_awOperatorIds[MAX_OPERATOR_NUM];
}T_CaOperatorId;

#define MAX_OPERATORINFO_LEN    (20)       /*最长的运营商名称*/
typedef struct _CaOperatorInfo
{
	BYTE	m_acOperatorInfo[MAX_OPERATORINFO_LEN+1];   /*运营商私有信息*/
}T_CaOperatorInfo;

#define MAX_ACLIST_NUM          (18)        /*智能卡内保存每个运营商的AC的个数*/
typedef struct _CaOperatorAcList            /*运营商的AC信息*/
{
	WORD	m_wAcListNum;
	DWORD	m_dwOperatorAcList[MAX_ACLIST_NUM];
}T_CaOperatorAcList;

#define	MAX_ENTITL_NUM           (300)      /*智能卡保存最多授权产品的个数*/
typedef struct _CaEntitle
{
	DWORD 		m_dwProductID;
	BYTE		m_cCanTape;			/*是否可录象*/
	WORD		m_wExpriData;		/*过期时间*/
}T_CaEntitle;

typedef struct _CaServiceEntitles  /*服务授权信息*/
{
	WORD			m_wProductCount;
	T_CaEntitle		m_astEntitles[MAX_ENTITL_NUM];
}T_CaServiceEntitles;

typedef struct  _CaDate
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
}T_CaDate;

//----------------CA 返回值--------
#define CA_RTN_FAILURE                  (-1)
#define CA_RTN_OK                       (0x00)
#define CA_RTN_UNKNOWN                  (0x01)    /*- 未知错误*/
#define CA_RTN_POINTER_INVALID          (0x02)    /*- 指针无效*/
#define CA_RTN_CARD_INVALID             (0x03)    /*- 智能卡无效*/
#define CA_RTN_PIN_INVALID              (0x04)    /*- PIN码无效*/
#define CA_RTN_PIN_LOCK                 (0x05)    /*- PIN码锁定*/
#define CA_RTN_DATASPACE_SMALL          (0x06)    /*- 所给的空间不足*/
#define CA_RTN_CARD_PAIROTHER           (0x07)    /*- 智能卡已经对应别的机顶盒*/
#define CA_RTN_DATA_NOT_FIND            (0x08)    /*- 没有找到所要的数据*/
#define CA_RTN_PROG_STATUS_INVALID      (0x09)    /*- 要购买的节目状态无效*/
#define CA_RTN_CARD_NO_ROOM             (0x0A)    /*- 智能卡没有空间存放购买的节目*/
#define CA_RTN_WORKTIME_INVALID         (0x0B)    /*- 设定的工作时段无效 0～24*/
#define CA_RTN_IPPV_CANNTDEL            (0x0C)    /*- IPPV节目不能被删除*/
#define CA_RTN_CARD_NOPAIR              (0x0D)    /*- 智能卡没有对应任何的机顶盒*/
#define CA_RTN_WATCHRATING_INVALID      (0x0E)    /*- 设定的观看级别无效 4－18*/

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
* Function Name: CS_CA_GetTFLibVersion
*   获取同方库版本
*
*Parameters	
*	  pacVerBuf
*            [out] 存放版本信息的Buffer
*       dwBufSize
*            [in] Buffer 长度(最少16字节)
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetTFLibVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
* Function Name: CS_CA_IsPinLocked
*   判断智能卡是否琐定
*
*Parameters	
*	   pbPinLock
*            [out] 是否琐定标志
*Return Values
*	  CA_RTN_OK : 				正确返回  
*	  CA_RTN_POINTER_INVALID: 	指针为空
*	  CA_RTN_CARD_INVALID:  		无智能卡或无效卡
**************************************************************************/
WORD CS_CA_IsPinLocked(BYTE* pbPinLock);

/**************************************************************************
* Function Name: CS_CA_ChangePin
*   修改智能卡密码
*Parameters	
*	  dwOldPin
*            [in] 旧密码
*       dwNewPin
*            [in] 新密码
*Return Values
*	  CA_RTN_OK : 		 		正确返回  
*	  CA_RTN_POINTER_INVALID: 	指针为空
*       CA_RTN_CARD_INVALID:  		无智能卡或无效卡
*	  CA_RTN_PIN_INVALID:		旧PIN码错误
*	  CA_RTN_PIN_LOCK:			PIN码已被锁定
*	  CA_RTN_UNKNOW:			未知错误
**************************************************************************/
WORD CS_CA_ChangePin(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
* Function Name: CS_CA_GetSCCardID
*   获取智能卡的卡号
*
*Parameters	
*	  pacSCIDBuf
*            [out] 智能卡的卡号
*Return Values
*	  CA_RTN_OK : 		 		正确返回  
*	  CA_RTN_POINTER_INVALID: 	指针为空
*	  CA_RTN_CARD_INVALID:  		无智能卡或无效卡
**************************************************************************/
INT	CS_CA_GetSCCardID(DWORD* pdwCardId);

/**************************************************************************
* Function Name: CS_CA_IsPaired
*   判断机顶盒与卡是否已配对
*Parameters	 
*Return Values
*	  CA_RTN_OK : 		 		 智能卡对应当前机顶盒
*	  CA_RTN_CARD_INVALID:  		 无智能卡或无效卡
*	  CA_RTN_CARD_NOPAIR:		 智能卡没有对应任何机顶盒
*	  CA_RTN_CARD_PAIROTHER:  	 智能卡已对应别的机顶盒
**************************************************************************/
WORD CS_CA_IsPaired( void );

/**************************************************************************
* Function Name: CS_CA_SetPaired
*   将当间机顶盒与智能卡配对
*Parameters	
*	  dwPinCode
*            [in]卡密码
 *Return Values
*	  CA_RTN_OK : 		 		配对成功
*	  CA_RTN_POINTER_INVALID: 	指针为空
*	  CA_RTN_CARD_INVALID:  		无智能卡或无效卡
*	  CA_RTN_PIN_INVALID:		PIN码错误
*	  CA_RTN_PIN_LOCK:			PIN码已被锁定
*	  CA_RTN_UNKNOW:			未知错误
*	  CA_RTN_CARD_PAIROTHER:  	智能卡已对应别的机顶盒
**************************************************************************/
WORD CS_CA_SetPaired(DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_GetRating
 *   获取父母级别
*Parameters	
*	  pbyRating
*            [out]  父母级[4-18]
 *Return Values
*	  CA_RTN_OK : 		 		正确返回  
*	  CA_RTN_POINTER_INVALID: 	指针为空
*	  CA_RTN_CARD_INVALID:  	 	无智能卡或无效卡
*	  CA_RTN_UNKNOW:		 	未知错误
**************************************************************************/
WORD CS_CA_GetRating(BYTE* pbyRating);

/**************************************************************************
* Function Name: CS_CA_SetRating
 *  设置父母级别
*Parameters	
*	  dwPinCode
*            [out]  父母级
 *Return Values
*	  CA_RTN_OK : 		    			正确返回 
*	  CA_RTN_POINTER_INVALID:      	指针为空
*	  CA_RTN_CARD_INVALID:  	    		无智能卡或无效卡
*	  CA_RTN_UNKNOW:		   	 	未知错误
*	  CA_RTN_PIN_INVALID:	    		PIN码错误
*	  CA_RTN_PIN_LOCK:	    			PIN码已被锁定
*	  CA_RTN_WATCHRATING_INVALID:   输入的父母级无效。必须是[4-18]
**************************************************************************/
WORD CS_CA_SetRating(DWORD dwPinCode, BYTE byRating);

/**************************************************************************
* Function Name: CS_CA_GetWorkTime
 *  获取工作时段
*Parameters	
*	  pbyStartHour
*            [out]  开始时间
*	  pbyEndHour
*		[out] 结束时间
 *Return Values
*	  CA_RTN_OK : 				正确返回  
*	  CA_RTN_POINTER_INVALID: 	指针为空
*	  CA_RTN_CARD_INVALID:  		无智能卡或无效卡
*	  CA_RTN_UNKNOW:			未知错误
**************************************************************************/
WORD CS_CA_GetWorkTime(BYTE* pbyStartHour, BYTE* pbyEndHour);

/**************************************************************************
* Function Name: CS_CA_SetWorkTime
 *  设置工作时段
*Parameters	
*	  dwPinCode
*		[in]	卡密码
*	  pbyStartHour
*            [in]  开始时间
*	  pbyEndHour
*		[in] 结束时间
 *Return Values
*	 CA_RTN_OK : 		    			正确返回  
*	 CA_RTN_POINTER_INVALID:    		指针为空
*	 CA_RTN_CARD_INVALID:  	    		无智能卡或无效卡
*	 CA_RTN_UNKNOW:		    		未知错误
*	 CA_RTN_PIN_INVALID:	    			PIN码错误
*	 CA_RTN_PIN_LOCK:	    			PIN码已被锁定
**************************************************************************/
WORD CS_CA_SetWorkTime(DWORD dwPinCode, BYTE byStartHour, BYTE byEndHour);

/**************************************************************************
* Function Name: CS_CA_GetOperatorIds
 *  获取运营商ID
*Parameters	
*	  pstOperatorsId
*		[out] 运营商ID
 *Return Values
*	  CA_RTN_OK : 		    		正确返回  
*	  CA_RTN_POINTER_INVALID:    	指针为空
*	  CA_RTN_CARD_INVALID:  	   	无智能卡或无效卡
**************************************************************************/
WORD CS_CA_GetOperatorIds(T_CaOperatorId* pstOperatorsId);

/**************************************************************************
* Function Name: CS_CA_GetOperatorInfo
 *  获取运营商信息
*Parameters	
*	  wOperatorsId
*		[in]	运营商ID
*	  T_CaOperatorInfo
*            [out]  运营商信息
 *Return Values
*	  CA_RTN_OK : 		    		正确返回  
*	  CA_RTN_POINTER_INVALID:    	指针为空
*	  CA_RTN_CARD_INVALID:  	    	无智能卡或无效卡
*	  CA_RTN_DATA_NOT_FIND:	    	没有找到对应运营商
**************************************************************************/
WORD CS_CA_GetOperatorInfo(WORD wOperatorsId, T_CaOperatorInfo* pstOperatorInfo);

/**************************************************************************
* Function Name: CS_CA_GetACList
 *  获取运营商AC信息
*Parameters	
*	  wOperatorsId
*		[in]	运营商ID
*	  pstAcList
*            [out]  运营商AC信息
 *Return Values
*	 CA_RTN_OK : 		    		正确返回  
*	 CA_RTN_POINTER_INVALID:    	指针为空
*	 CA_RTN_CARD_INVALID:  	    	无智能卡或无效卡
*	 CA_RTN_DATA_NOT_FIND:	    	没有找到对应运营商
**************************************************************************/
WORD CS_CA_GetACList(WORD wOperatorsId, T_CaOperatorAcList* pstAcList );

/**************************************************************************
* Function Name: CS_CA_GetServiceEntitles
 *  获取运营商权限
*Parameters	
*	  wOperatorsId
*		[in]	运营商ID
*	  pstServiceEntitles
*            [out]  运营商权限
 *Return Values
*	  CA_RTN_OK : 		    			正确返回  
*	  CA_RTN_POINTER_INVALID:    		指针为空
*	  CA_RTN_CARD_INVALID:  	    		无智能卡或无效卡
*	  CA_RTN_DATA_NOT_FIND:	   		 没有找到对应运营商
**************************************************************************/
WORD CS_CA_GetServiceEntitles(WORD wOperatorsId, T_CaServiceEntitles* pstServiceEntitles);

#define MAX_SLOT_NUM       (20)   /*卡存储的最大钱包数*/
typedef struct _CaSlotId          /*钱包ID*/
{
	WORD	m_wSlotNum;
	BYTE	m_acSlotIds[MAX_SLOT_NUM];
}T_CaSlotId;

typedef struct _CaSlotInfo         /*钱包信息*/
{
	WORD	m_wCreditLimit;        /*信用度*/
	WORD	m_wBalance;            /*已花的点数*/
}T_CaSlotInfo;

#define MAX_PRICE_NUM     (2)      /*最多的IPPV价格个数*/

typedef struct _CaIppvPrice        /*IPPV节目价格*/
{
	BYTE			m_cPriceCode;  /*价格类型0: 只能收看,不能录象; 1:能收看,能录象*/
	WORD			m_wPrice;      /*价格点数*/
}T_CaIppvPrice;

typedef struct _CaIppvBuyInfo				/*IPPV购买信息*/
{
	WORD			m_wOperatorId;			/*运营商ID*/
	BYTE			m_cSlotID;				/*钱包ID*/
	DWORD 			m_dwProductId;          /*节目的ID*/
	BYTE			m_cPriceNum;			/*节目价格个数*/
	T_CaIppvPrice  	m_stPrice[MAX_PRICE_NUM]; 		/*节目价格*/
	WORD			m_wExpiredDate;				/*节目过期时间*/
	WORD			m_wRemainCredit;			/*钱包剩余点数*/
}T_CaIppvBuyInfo;

typedef struct _CaIppvNotify						/*IPPV购买通知*/
{
	BYTE 				m_cMessageType;	/*显示类型0: 免费预览阶段.1: 收费阶段*/
	WORD				m_wEcmPid;			/*需显示对话框标志*/
	T_CaIppvBuyInfo		m_stIppvProgram;	/*Ippv节目信息*/
}T_CaIppvNotify;

#define	MAX_IPPVPROG_NUM    (300)      /*智能卡保存最多IPPV节目的个数*/

typedef struct _CaIppvInfo					
{
	DWORD 			m_dwProductID;               /*节目的ID*/
	BYTE			m_cBookEdFlag;			/*产品状态1: BOOKING 3: VIEWED*/ 
	BYTE			m_cCanTape;			/*是否可以录像，1：可以录像；0：不可以录像*/
	WORD			m_wPrice;				/*节目价格*/
	WORD			m_wExpiredDate;		/*节目过期时间*/
}T_CaIppvInfo;

typedef struct _CaIppvProgInfo			/*IPPV 节目信息*/
{
	WORD			m_wProgramNum;
	T_CaIppvInfo		m_astIppvInfo[MAX_IPPVPROG_NUM];
}T_CaIppvProgInfo;

/**************************************************************************
* Function Name: CS_CA_GetSlotIDs
 *  获取运营商钱包ID
*Parameters	
*	  wOperatorsId
*		[in]	运营商ID
*	  pstSlotId
*            [out]  钱包ID
 *Return Values
*	  CA_RTN_OK : 		    		正确返回  
*	  CA_RTN_POINTER_INVALID:    	指针为空
*	  CA_RTN_CARD_INVALID:  	    	无智能卡或无效卡
*	  CA_RTN_DATA_NOT_FIND:	    	没有找到对应运营商
**************************************************************************/
WORD CS_CA_GetSlotIDs(WORD wOperatorsId, T_CaSlotId* pstSlotId);

/**************************************************************************
* Function Name: CS_CA_GetSlotInfo
 *  获取运营商钱包信息
*Parameters	
*	  wOperatorsId
*		[in]	运营商ID
*	  bySlotID
*            [in]  钱包ID
*	  pstSlotInfo
*            [out]  钱包信息
 *Return Values
*	  CA_RTN_OK : 		    		正确返回  
*	  CA_RTN_POINTER_INVALID:    	指针为空
*	  CA_RTN_CARD_INVALID:  	    	无智能卡或无效卡
*	  CA_RTN_DATA_NOT_FIND:	    	没有找到对应运营商
**************************************************************************/
WORD CS_CA_GetSlotInfo(WORD wOperatorsId, BYTE bySlotID, T_CaSlotInfo* pstSlotInfo);

/**************************************************************************
* Function Name: CS_CA_GetIPPVProgram
 *  获取运营商PPV节目信息
*Parameters	
*	  wOperatorsId
*		[in]	运营商ID
*	  bySlotID
*            [in]  钱包ID
*	  pstIppvInfo
*            [out]  PPV节目信息
 *Return Values
*	  CA_RTN_OK : 		    		正确返回  
*	  CA_RTN_POINTER_INVALID:    	指针为空
*	  CA_RTN_CARD_INVALID:  	   	无智能卡或无效卡
*	  CA_RTN_DATA_NOT_FIND:	    	没有找到对应运营商
**************************************************************************/
WORD CS_CA_GetIPPVProgram(WORD wOperatorsId, BYTE bySlotID, T_CaIppvProgInfo* pstIppvInfo);

/**************************************************************************
* Function Name: CS_CA_IPPVBuyConfirm
 *  确认购买或取消购买IPPV节目
*Parameters	
*	  cBuyFlag
*		[in]	TRUE :购买。FALSE:不购买
*	  wEcmPid
*            [in]  对应节目ECMPID
*	  dwPinCode
*            [in]  卡密码
*	  pstPrice
*            [in]  节目价格 
 *Return Values
*	  CA_RTN_OK : 		    			正确返回  
*	  CA_RTN_POINTER_INVALID:    		价格指针为空
*	  CA_RTN_CARD_INVALID:  	    		无智能卡或无效卡
*	  CA_RTN_DATA_NOT_FIND:	    		没有找到对应运营商
*	  CA_RTN_PIN_INVALID:	    		PIN码错误
*	  CA_RTN_PIN_LOCK:	    			PIN码已被锁定
*	  CA_RTN_CARD_NO_ROOM:	    		智能卡没有空间
*	  CA_RTN_PROG_STATUS_INVALID:	节目状态无效
**************************************************************************/
WORD CS_CA_IPPVBuyConfirm(BYTE cBuyFlag,WORD wEcmPid, DWORD dwPinCode, T_CaIppvPrice* pstPrice);

/**************************************************************************
* Function Name: CS_CA_DelIPPVProgram
 *  删除运营商IPPV节目
*Parameters	
*	  wOperatorId
*		[in]	运营商ID
*	  wSlotId
*            [in]  钱包ID
*	  wProductId
*            [in]  节目ID
*	  wExpireDate
*            [in]  节目过期时间
 *Return Values
*	  CA_RTN_OK : 		    		正确返回  
*	  CA_RTN_CARD_INVALID:  	   	无智能卡或无效卡
*	  CA_RTN_DATA_NOT_FIND:	    	没有找到相应的节目
*	  CA_RTN_IPPV_CANNTDEL:	    	节目不能被删除
***************************************************************************/
WORD CS_CA_DelIPPVProgram(WORD wOperatorId, BYTE wSlotId, WORD	wProductId, WORD wExpireDate);

/**************************************************************************
* Function Name: CS_CA_ChangeTFDate
*  时间转换，将同方定义的时间转换为
* CA定义的时间
* 同方定义的时间 以2000-1-1为0，每多一天加1
*Parameters	
*	  wdate
*		[in]	同方定义的时间整数
*	  pstDate
*            [out]  转换后的时间表示
**************************************************************************/
void CS_CA_ChangeTFDate(WORD wdate, T_CaDate *pstDate);

#define MAX_EMAIL_TITLE_LEN	     (30)			/*邮件标题的长度*/
#define MAX_EMAIL_NUM            (100)			/*机顶盒保存的最大邮件个数*/
#define MAX_EMAIL_CONTENT_LEN    (160)			/*邮件内容的长度*/

typedef struct _CaEmailContent
{
	BYTE		m_acEmailContent[MAX_EMAIL_CONTENT_LEN+1];	/*Email的正文*/
}T_CaEmailContent;

typedef struct _CaEmailTitle                       
{
	DWORD		m_dwEmailID;            						/*Email ID  							  */
	BYTE		m_cNewEmail;            						/*0 不是新邮件 1是新邮件	  */
	BYTE   		m_acEmailHead[MAX_EMAIL_TITLE_LEN+1];  		/*邮件标题，最长为30		  */
	WORD 		m_wImportance;								/*重要性, 0：普通，1：重要 */
	T_CaDate	m_dwCreateTime;          						/*EMAIL创建的时间				  */
}T_CaEmailHead;

typedef enum
{
	EMAIL_HIDE_ICON,									/*隐藏邮件通知图标*/
	EMAIL_NEW_ICON,									/*新邮件通知，显示新邮件图标*/
	EMAIL_FULL_ICON									/*磁盘空间以满，图标闪烁。*/	
}T_CaEmailIconType;

/*
typedef struct _CaEmailNotify
{
	T_CaEmailIconType	m_stShowType;
	DWORD				m_dwEmailId;
}T_CaEmailNotify;
*/

typedef enum _CaEmailSort					/*邮件排序的方式*/
{
	CA_EMAIL_TIME_DESC,							/*创建时间早的排在后面*/	
	CA_EMAIL_TIME_ASC,							/*创建时间早的排在前面*/	
	CA_EMAIL_READSTATUS_DESC,					/*已读的排在后面*/
	CA_EMAIL_READSTATUS_ASC,						/*已读的排在前面*/
	CA_EMAIL_COMSTATUS_ASC,						/*已读的且创建时间早的排在前面*/
	CA_EMAIL_COMSTATUS_DESC,						/*已读的且创建时间早的排在后面*/
	
	CA_EMAIL_SORT_MAX
}T_CaEmailSortStyle;
	
/**************************************************************************
* Function Name: CS_CA_GetEmailTotalNum
 *  获取邮件总数
*Parameters	
*		EmailNum: 现存邮件数
*		EmptyNum: 剩余邮件空间
 *Return Values
*		SUCCESS:	成功
*		FAILURE:		失败
**************************************************************************/
INT CS_CA_GetEmailTotalNum(BYTE* pcEmailNum, BYTE* pcEmptyNum);

/**************************************************************************
* Function Name: CS_CA_GetEmailTitle
 *  获取邮件标题
*Parameters	
*	  nIndex
*		[in]	序号(从1开始)
*	  pstEmail
*            [out]  邮件标题
 *Return Values
*	  CA_RTN_OK : 成功 
*	  CA_RTN_POINTER_INVALID: 指针为空
*	  FAILURE: 序号错误
**************************************************************************/
WORD CS_CA_GetEmailTitle(INT nIndex, T_CaEmailHead *pstEmail);

/**************************************************************************
* Function Name: CS_CA_GetEmailContent
 *  获取某一封邮件内容
*Parameters	
*	  nIndex
*		[in]	邮件序号(从1开始)
*	  pstEmailContent
*            [out]  邮件内容
 *Return Values
*	  CA_RTN_OK : 获取成功  
*	  CA_RTN_POINTER_INVALID: 指针为空
*	  CA_RTN_DATA_NOT_FIND:  数据未找到
**************************************************************************/
WORD CS_CA_GetEmailContent(INT nIndex, T_CaEmailContent* pstEmailContent);

/**************************************************************************
* Function Name: CS_CA_GetEmailContentById
 *  根据邮件ID获取一封邮件内容
*Parameters	
*	  dwEmailId
*		[in]	邮件ID
*	  pstEmailContent
*            [out]  邮件内容
 *Return Values
*	  CA_RTN_OK : 获取成功  
*	  CA_RTN_POINTER_INVALID: 指针为空
*	  CA_RTN_DATA_NOT_FIND:  数据未找到
**************************************************************************/
WORD CS_CA_GetEmailContentById(DWORD dwEmailId, T_CaEmailContent* pstEmailContent);

/**************************************************************************
* Function Name: CS_CA_DelEmail
 *  删除一封邮件
*Parameters	
*	  nIndex
*		[in]	邮件序号(0:全部删除; >0: 删除对应序号邮件)
 *Return Values
*	   无
**************************************************************************/
void    CS_CA_DelEmail(INT nIndex);

/**************************************************************************
* Function Name: CS_CA_SortEmail
 *  对邮件进行排序
*Parameters	
*	  eSortStyle
		[in]	排序方式
 *Return Values
*	   无
**************************************************************************/
void CS_CA_SortEmail( T_CaEmailSortStyle eSortStyle);

#define		MAX_OSD_LEN		180		/*OSD内容的最大长度*/

typedef enum _CaOsdStyle
{
	CA_OSD_TOP,					/*在屏幕上方显示*/
	CA_OSD_BOTTOM
}T_CaOsdStyle;

typedef struct _CaOsd
{
	T_CaOsdStyle 	m_eStyle;
	BYTE			m_acMessage[MAX_OSD_LEN];
}T_CaOsd;

/**************************************************************************
* Function Name: CS_CA_StartStockPlay
*  启动股票加密
* Parameters	
*	  pstServiceInfo[IN]:

* Return Values
*	   无
**************************************************************************/
INT CS_CA_StartStockPlay(T_CaFtaService* pstServiceInfo);

/**************************************************************************
* Function Name: CS_CA_StartStockPlay
*  退出股票时调用
* Parameters	
*	  pstServiceInfo[IN]:

* Return Values
*	   无
**************************************************************************/
INT CS_CA_StopStockPlay(T_CaFtaService* pstServiceInfo);


#ifdef  __cplusplus
}
#endif

#endif //_CS_TFCA_INTERFACE_H_

