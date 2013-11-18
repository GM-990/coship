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
* - Revision 1.1  2006/3/23
*   Programmer:Xiejianghua
*   Create.
*   Revision 1.2 2007/11/07
*   Programmer:pengshuangquan
*   Added & Modify
*   Revision 1.3 2008/08/01
*   Programmer:wansuanlin
*   Modify
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
	//处理邮件图标显示(T_CaEmailIconType给出显示方式)
	CA_COMMAND_NEW_EMAIL,			//0  dwParm2 = (DWORD)T_CaEmailIconType
	
	//指纹
	CA_COMMAND_FINGERPRINT,			// 1 dwParm2 = (DWORD)CardId

	//显示OSD信息
	CA_COMMAND_SHOW_OSD,			// 2 dwParm2 = (DWORD)&T_CaOsd
	
	//隐藏OSD信息
	CA_COMMAND_HIDE_OSD,			// 3 dwParm2 = (WORD)T_CaOsdStyle

	//IPPV购买信息
	CA_COMMAND_IPPV_NOTIFY,			// 4 dwParm2 = (DWORD)&T_CaIppvNotify
	
	//取消IPPV购买提示
	CA_COMMAND_IPPV_CANCEL,			// 5 dwParm2 = 0
	
	//wOperaterID!=0时提示:"母卡数据读取成功，请插子卡"; wOperaterID==0时提示"母卡数据读取失败"  
	CA_COMMAND_CHILD_INFO,			//6 dwParm2 = wOperaterID
	
	//频道锁定
	CA_COMMAND_LOCK_SERVICE,		// 7 dwParm2=&T_CaLockServiceParam
	
	//频道解锁
	CA_COMMAND_UNLOCK_SERVICE,		// 8 dwParm2 = NULL
	
	//处理反授权码图标(E_DeTitileStatus给出显示方式)
	CA_COMMAND_DETITLE_NOTIFY,		// 9 dwParm2 = E_DeTitileStatus 
	
	//同方保留，暂不处理
	CA_COMMAND_ENTITLE_CHANGE,		// 10 Reserve
	
	//显示升级进度
	CA_COMMAND_SHOW_PROGRESS,		// 11 dwParm2 = &T_PrggressStrip
	
	//
	CA_COMMAND_MAX
}T_CaCommand;

typedef enum _CaAlarm
{	
	CA_ALARM_CANCEL = 0,				/* 0 取消当前的显示*/
	CA_ALARM_BADCARD,					/* 1 无法识别卡*/
	CA_ALARM_EXPICARD,					/* 2 智能卡过期，请更换新卡*/
	CA_ALARM_INSERTCARD,				/* 3 加扰节目，请插入智能卡*/
	CA_ALARM_NOOPER,					/* 4 卡中不存在节目运营商*/
	CA_ALARM_BLACKOUT,					/* 5 条件禁播*/
	CA_ALARM_OUTWORKTIME,				/* 6 当前时段被设定为不能观看*/
	CA_ALARM_WATCHLEVEL,				/* 7 节目级别高于设定的观看级别*/
	CA_ALARM_PAIRING,					/* 8 智能卡与本机顶盒不对应*/
	CA_ALARM_NOENTITLE,					/* 9 没有授权*/
	CA_ALARM_DECRYPTFAIL,				/* 10 节目解密失败*/
	CA_ALARM_NOMONEY,					/* 11 卡内金额不足*/
    CA_ALARM_ERRREGION,					/* 12 区域不正确*/
	CA_ALARM_NEEDFEED_TYPE,				/* 13 子卡需要和母卡对应，请插入母卡*/
	CA_ALARM_ERRCARD_TYPE,				/* 14 智能卡校验失败，请联系运营商*/
	CA_ALARM_UPDATE_TYPE,				/* 15 智能卡升级中，请不要拔卡或者关机*/
	CA_ALARM_LOWCARDVER_TYPE,			/* 16 请升级智能卡*/
	CA_ALARM_VIEWLOCK_TYPE,				/* 17 请勿频繁切换频道*/
	CA_ALARM_MAXRESTART_TYPE,			/* 18 智能卡暂时休眠，请5分钟后重新开机*/
	CA_ALARM_FREEZE_TYPE,				/* 19 智能卡已冻结，请联系运营商*/
	CA_ALARM_CALLBACK_TYPE,				/* 20 智能卡已暂停，请回传收视记录给运营商*/
	CA_ALARM_STBLOCKED_TYPE,            /* 21 请重启机顶盒*/
	CA_ALARM_STBFRESS_TYPE,             /* 22 机顶盒被冻结*/

	//tf add in 0x0424 version
	CA_ALARM_CURTAIN_CANNOTPREVIEW,		/*23窗帘节目，不可预览阶段*/
	CA_ALARM_CARDUPGRADING, 			/*24升级测试卡测试中...*/
	CA_ALARM_CARDUPGRADE_FAIL,			/*25升级测试卡测试失败，请检查机卡通讯模块*/
	CA_ALARM_CARDUPGRADE_SUCCESS,		/*26升级测试卡测试成功*/
	CA_ALARM_NOOPER_INCARD,				/*27卡中不存在移植库定制运营商*/

	//add by lgc 2013.04.30 
	TFCA_ALARM_CURTAIN_CANCLE,  /*28取消高级预览显示*/
	TFCA_ALARM_CURTAIN_OK,  /*29高级预览节目正常解密*/
	TFCA_ALARM_CURTAIN_TOTTIME_ERROR, /*30高级预览节目禁止解密：已经达到总观看时长*/			
	TFCA_ALARM_CURTAIN_WATCHTIME_ERROR, /*31高级预览节目禁止解密：已经达到WatchTime限制*/
	TFCA_ALARM_CURTAIN_TOTCNT_ERROR, /*32高级预览节目禁止解密：已经达到总允许观看次数*/
	TFCA_ALARM_CURTAIN_ROOM_ERROR, /*33高级预览节目禁止解密：高级预览节目记录空间不足*/
	TFCA_ALARM_CURTAIN_PARAM_ERROR,/*34高级预览节目禁止解密：节目参数错误*/
	TFCA_ALARM_CURTAIN_TIME_ERROR, /*35高级预览节目禁止解密：数据错误*/



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

typedef enum _DeTitileStatus
{
	CA_DETITLE_ALL_READED,      /*全部已读 ,      对应操作:隐藏图标*/
	CA_DETITLE_RECEIVED,        /*新接收反授权码, 对应操作:显示图标*/
	CA_DETITLE_SPACE_SMALL,     /*存储已满 ,      对应操作:图标闪烁*/
	CA_DETITLE_IGNORE,          /*不处理   */
	CA_DETITLE_MAX
}E_DeTitileStatus;

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
	BYTE		m_cCanTape;             /*是否可录象*/
	WORD		m_wBeginDate;			/*起始时间*/
	WORD		m_wExpriData;			/*过期时间*/
}T_CaEntitle;

typedef struct _CaServiceEntitles		/*服务授权信息*/
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

typedef struct _PrggressStrip
{
	BYTE m_ucType;					            /* 1升级数据接收中(测试用),2 智能卡升级中*/
	BYTE m_ucProgress;				            /* 升级进度百分比*/	
}T_PrggressStrip;					            /*显示升级进度条 */
	
typedef struct _WorkingTime		   
{
	BYTE m_ucHour;
	BYTE m_ucMiniter;
	BYTE m_ucSecond;
}T_WorkingTime;                                 /*设置智能卡工作时间*/

#define MAX_PAIR_NUM   (5)
#define PAIR_ELEM_LEN  (6)

typedef struct _PairElem
{
	BYTE m_acStdId[PAIR_ELEM_LEN];
}T_PairElem;                                   /*机顶盒序列号*/

typedef struct _PairList
{
	BYTE       m_ucListNum;						/*与智能卡配对的机顶盒个数*/
	T_PairElem m_aucListBuff[MAX_PAIR_NUM];     /*机顶盒对应的序列号*/
}T_PairList;

#define MAX_DETITLE_NUM (5)

typedef struct _DetitleInfo
{
	DWORD m_dwDetitleNum;                         /*反授权个数*/
	DWORD m_adwDetitleBuff[MAX_DETITLE_NUM];
}T_DetitleInfo;                                 /*反授权码*/

typedef struct _EntitleInfo
{
	DWORD m_dwEntitleNum;
	DWORD m_adwEntitleBuff[MAX_ENTITL_NUM];//[MAX_DETITLE_NUM];
}T_EntitleInfo;                                 /*授权码*/

typedef enum _PairStatus
{
	E_PAIR_NEEDED_CHILD_CARD,		            /*请插入子卡*/
	E_PAIR_NEEDED_PARENT_CARD,                  /*请插入母卡*/
	E_PAIR_FEED_SUCCESS,                        /*子母卡喂养成功*/
	E_PAIR_FEED_NOT_NEED,						/*子卡还不需要喂养*/
	E_PAIR_MAX
}E_PairStatus;	                                /*喂养状态*/


#define MAX_PARENTCARDSN_LEN (17)
typedef struct _CaChildInfo
{
    BOOL        m_bIsChild;                     /*TRUE:子卡FALSE:母卡*/
    DWORD       m_dwFeedPeri;                   /*喂养周期*/
    T_CaDate    m_stLastFeedTime;               /*上次喂养时间*/
	CHAR        m_acParentCardSN[MAX_PARENTCARDSN_LEN];  /*母卡卡号*/
	BOOL        m_bIsCanBeFeed;					/*子卡在当前时间内是否能被喂养。false：子卡喂养时间未到，不能被喂养；true：可以被喂养*/
	BOOL        m_bNewVersion;                   /*TRUE:m_acParentCardSN和m_bIsCanBeFeed有效，FALSE:m_acParentCardSN和m_bIsCanBeFeed无效*/
}T_CaChildInfo;                                /*子母卡状态*/

#define CA_MAXLEN_SMC_SN                (16)

//----------------CA 返回值--------
#define CA_RTN_FAILURE                  (0xffff)
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
#define	CA_RTN_CARD_NOTSUPPORT		    (0x0F)	/*- 当前智能卡不支持此功能*/
#define	CA_RTN_DATA_ERROR				(0x10)	/*- 数据错误，智能卡拒绝*/
#define	CA_RTN_FEEDTIME_NOT_ARRIVE		(0x11)	/*- 喂养时间未到，子卡不能被喂养*/
#define	CA_RTN_CARD_TYPEERROR			(0x12)	/*- 子母卡喂养失败，插入智能卡类型错误*/

#define CA_RTN_READFAILURE				(0xf0)	/*-读取母卡数据失败*/
#define CA_RTN_FEEDFAILURE				(0xf1)	/*-喂养失败*/

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
* Function Name: CSCAGetPlatFormID
*   获取平台ID
*
*Parameters	
*	  pwPlatformID
*            [out] 存放平台ID
*       
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CSCAGetPlatFormID(WORD* pwPlatformID);


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
*    pcCardId[in]:已字符串的形式返回智能卡卡号，
*    长度为CA_MAXLEN_SMC_SN +1
*         
*Return Values
*	  CA_RTN_OK : 		 		正确返回  
*	  CA_RTN_POINTER_INVALID: 	指针为空
*	  CA_RTN_CARD_INVALID:  	无智能卡或无效卡
**************************************************************************/
INT	CS_CA_GetSCCardID(char* pcCardId);

/**************************************************************************
* Function Name: CS_CA_IsPaired
*   判断机顶盒与卡是否已配对
*Parameters	 
*	pstPairList[out]:该智能卡和几个机顶盒配对，及配对机顶盒的序列号
*	当返回值为CA_RTN_OK或者CA_RTN_CARD_PAIROTHER是有有效输出，否则清空
*Return Values
*	  CA_RTN_OK:当前机卡对应,并改写pstPairList
*	  CA_RTN_CARD_INVALID:	 无智能卡或无效卡
*	  CA_RTN_CARD_NOPAIR:智能卡没有对应任何机顶盒请设置机卡对应
*	  CA_RTN_CARD_PAIROTHER:智能卡已经对应其它机顶盒,并改写pstPairList
**************************************************************************/
WORD CS_CA_IsPaired(T_PairList * pstPairList);

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
*	  pstStartTime
*            [out]  开始时间
*	  pstEndTime
*		[out] 结束时间
 *Return Values
*	  CA_RTN_OK : 				正确返回  
*	  CA_RTN_POINTER_INVALID: 	指针为空
*	  CA_RTN_CARD_INVALID:  	无智能卡或无效卡
*	  CA_RTN_UNKNOW:			未知错误
**************************************************************************/
WORD CS_CA_GetWorkTime(T_WorkingTime* pstStartTime, T_WorkingTime* pstEndTime);

/**************************************************************************
* Function Name: CS_CA_SetWorkTime
 *  设置工作时段
*Parameters	
*	  dwPinCode
*		[in]	卡密码
*	  pstStartTime
*            [in]  开始时间
*	  pstEndTime
*		[in] 结束时间
 *Return Values
*	 CA_RTN_OK : 		    			正确返回  
*	 CA_RTN_POINTER_INVALID:    		指针为空
*	 CA_RTN_CARD_INVALID:  	    		无智能卡或无效卡
*	 CA_RTN_UNKNOW:		    		未知错误
*	 CA_RTN_PIN_INVALID:	    			PIN码错误
*	 CA_RTN_PIN_LOCK:	    			PIN码已被锁定
**************************************************************************/
WORD CS_CA_SetWorkTime(DWORD dwPinCode,T_WorkingTime* pstStartTime, T_WorkingTime* pstEndTime);

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

#define     MAX_SLOT_NUM		20		/*卡存储的最大钱包数*/
typedef struct _CaSlotId				/*钱包ID*/
{
	WORD	m_wSlotNum;
	BYTE	m_acSlotIds[MAX_SLOT_NUM];
}T_CaSlotId;

typedef struct _CaSlotInfo                       /*钱包信息*/
{
	DWORD	m_wCreditLimit;                      /*信用度*/
	DWORD	m_wBalance;                          /*已花的点数*/
}T_CaSlotInfo;

#define  MAX_PRICE_NUM  (2)                      /*最多的IPPV价格个数*/

typedef struct _CaIppvPrice                      /*IPPV节目价格*/
{
	BYTE			m_cPriceCode;                /* 价格类型0: 只能收看,不能录象; 1:能收看,能录象*/
	WORD			m_wPrice;			         /*价格点数*/
}T_CaIppvPrice;

typedef struct _CaIppvBuyInfo                  /*IPPV购买信息*/
{
	WORD			m_wOperatorId;               /*运营商ID*/
	BYTE			m_cSlotID;	                 /*钱包ID*/
	DWORD 			m_dwProductId;               /*节目的ID*/
	BYTE			m_cPriceNum;                 /*节目价格个数*/
	T_CaIppvPrice  	m_stPrice[MAX_PRICE_NUM];    /*节目价格*/
	WORD			m_wExpiredDate;              /*节目过期时间*/
	DWORD			m_dwRemainCredit;             /*钱包剩余点数*/
}T_CaIppvBuyInfo;

typedef struct _CaIppvNotify                     /*IPPV购买通知*/
{
	BYTE 				m_cMessageType;	         /*显示类型0: 免费预览阶段.1: 收费阶段2  IPPT 购买阶段*/
	WORD				m_wEcmPid;               /*需显示对话框标志*/
	T_CaIppvBuyInfo		m_stIppvProgram;         /*Ippv节目信息*/
}T_CaIppvNotify;

#define		MAX_IPPVPROG_NUM       (300)	     /*智能卡保存最多IPPV节目的个数*/

typedef struct _CaIppvInfo					
{
	DWORD 			m_dwProductID;               /*节目的ID*/
	BYTE			m_cBookEdFlag;               /*产品状态1: BOOKING 3: VIEWED*/ 
	BYTE			m_cCanTape;                  /*是否可以录像，1：可以录像；0：不可以录像*/
	WORD			m_wPrice;                      /*节目价格*/
	WORD			m_wExpiredDate;              /*节目过期时间*/
}T_CaIppvInfo;

typedef struct _CaIppvProgInfo                   /*IPPV 节目信息*/
{
	WORD			m_wProgramNum;
	T_CaIppvInfo    m_astIppvInfo[MAX_IPPVPROG_NUM];
}T_CaIppvProgInfo;

#define MAX_COMPONENT_NUM (5)

typedef struct _LockComponent
{
	WORD		m_CompType; 		/*组件类型1:MPEG_TYPE_VIDEO_11172  3:MPEG_TYPE_AUDIO_11172*/   
	WORD		m_wCompPID; 		/*组件PID*/  
}T_LockComponent;

typedef struct _CaLockServiceParam
{
	BYTE m_wFecOuter;
	BYTE m_wFecInner;
	DWORD m_dwFrequency;
	DWORD m_wModulation;//0	Reserved, 1	QAM16, 2 QAM32, 3 QAM64, 4 QAM128, 5 QAM256, 6～255	Reserved
	DWORD m_wSymbolrate;
	DWORD m_wPcrPID;
	DWORD m_wComponentNum;
	T_LockComponent m_stComponent[MAX_COMPONENT_NUM];
}T_CaLockServiceParam;
  
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
	DWORD		m_dwEmailID;                                /*Email ID  */
	BYTE		m_cNewEmail;                                /*0 不是新邮件 1是新邮件 */
	BYTE   		m_acEmailHead[MAX_EMAIL_TITLE_LEN+1];  		/*邮件标题，最长为30 */
	WORD 		m_wImportance;								/*重要性, 0：普通，1：重要 */
	T_CaDate	m_dwCreateTime;                             /*EMAIL创建的时间*/
}T_CaEmailHead;

typedef enum
{
	EMAIL_HIDE_ICON,							    /*隐藏邮件通知图标*/
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

typedef enum _CaEmailSort 					    /*邮件排序的方式*/
{
	CA_EMAIL_TIME_DESC,							/*创建时间早的排在后面*/	
	CA_EMAIL_TIME_ASC,							/*创建时间早的排在前面*/	
	CA_EMAIL_READSTATUS_DESC,                   /*已读的排在后面*/
	CA_EMAIL_READSTATUS_ASC,                    /*已读的排在前面*/
	CA_EMAIL_COMSTATUS_ASC,						/*已读的且创建时间早的排在前面*/
	CA_EMAIL_COMSTATUS_DESC,                    /*已读的且创建时间早的排在后面*/
	
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

#define		MAX_OSD_LEN		184		/*OSD内容的最大长度*/

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

/**************************************************************************
* Function Name: CS_CA_GetDetitleInfo
*  获取反授权码
* Parameters	
*	wOperatorsId[in]:运营商ID
*	pdwDetileNum[out]:反授权信息
* Return Values
*	CA_RTN_OK 成功
*	CA_RTN_POINTER_INVALID 指针为空
*	CA_RTN_CARD_INVALID 智能卡不在机顶盒或者无效卡
*	CA_RTN_DATA_NOT_FIND 没有找到相应的运营商
**************************************************************************/

WORD CS_CA_GetDetitleInfo(WORD wOperatorsId,T_DetitleInfo* pstDetileInfo);

/**************************************************************************
* Function Name: CS_CA_DelDetitleInfo
*  删除反授权信息
* Parameters	
*	wOperatorsId[in]:运营商ID
*	dwDetileNum[in]:反授权 ID
* Return Values
*	   TRUE : 操作成功
*	   FALSE: 操作失败
**************************************************************************/

BOOL CS_CA_DelDetitleInfo(WORD wOperatorsId,DWORD dwDetileNum);

/**************************************************************************
* Function Name: CS_CA_GetEntitleInfo
* 获取授权信息
* Parameters	
*	  wOperatorsId[IN]:运营商ID
*	  pdwEntileNum[out]:	授权ID
* Return Values
*	CA_RTN_OK 成功
*	CA_RTN_POINTER_INVALID 指针为空
*	CA_RTN_CARD_INVALID 智能卡不在机顶盒或者无效卡
*	CA_RTN_DATA_NOT_FIND 没有找到相应的运营商
**************************************************************************/
WORD CS_CA_GetEntitleInfo(WORD wOperatorsId,T_EntitleInfo* pstEntileInfo);

/**************************************************************************
* Function Name: CS_CA_GetPairCardInfo
*  获取指定运营商智能卡的子母卡信息
* Parameters	
*	  	wOperatorsId[in]:  运营商ID
*		pstChildInfo[out]: 智能卡信息
* Return Values
*		CA_RTN_OK 成功
*		CA_RTN_CARD_INVALID	 智能卡无效
*		CA_RTN_CARD_NOTSUPPORT:当前智能卡不支持此功能
*		CA_RTN_DATA_NOT_FIND:	 没有找到所要的数据
**************************************************************************/
WORD CS_CA_GetPairCardInfo(WORD wOperatorsId,T_CaChildInfo* pstChildInfo );

/**************************************************************************
* Function Name: CS_CA_SetParentPair
*  设置子母卡配对
* Parameters	
*	  wOperatorsId[in] :  运营商ID
*	  bIsChild[in]: 智能卡类型,TRUE:子卡 FALSE: 母卡
*	  eStatus[out]:当前操作状态
*		E_PAIR_NEEDED_CHILD_CARD:请插入子卡
*		E_PAIR_NEEDED_PARENT_CARD:请插入母卡
*		E_PAIR_FEED_SUCCESS:子母卡喂养成功
* Return Values
*		CA_RTN_OK 成功
*		CA_RTN_CARD_INVALID	 智能卡无效
*		CA_RTN_CARD_NOTSUPPORT:当前智能卡不支持此功能
*		CA_RTN_DATA_NOT_FIND:	 没有找到所要的数据
*		CA_RTN_DATA_ERROR: 数据错误，智能卡拒绝
*		CA_RTN_FEEDTIME_NOT_ARRIVE:喂养时间未到，子卡不能被喂养
*		CA_RTN_CARD_TYPEERROR:  子母卡喂养失败，插入智能卡类型错误
*
*Notice:
*		Return Values 为第一优先级，如果Return Values 不等于CA_RTN_OK
*	这显示返回错误类型。如果Return Values 等于CA_RTN_OK 再检查
*	eStatus 类型E_PAIR_FEED_SUCCESS, E_PAIR_MAX不显示
**************************************************************************/
WORD CS_CA_SetParentPair(WORD wOperatorsId, BOOL bIsChild, E_PairStatus* eStatus );


/**************************************************************************
* Function Name: CSCASetParentPair
*  设置子母卡配对的接口(需要判断子卡喂养时间是否已到),新增加的接口，新项目需要用这个接口，
*   不需要使用接口CS_CA_SetParentPair
* Parameters	
*	  wOperatorsId[in] :  运营商ID
*	  bIsChild[in]: 智能卡类型,TRUE:子卡 FALSE: 母卡
*       bIsCanBeFeed:当前子卡是否能被喂养,TRUE:能够被喂养, FALSE: 喂养时间未到不能被喂养
*	  eStatus[out]:当前操作状态
*		E_PAIR_NEEDED_CHILD_CARD:请插入子卡
*		E_PAIR_NEEDED_PARENT_CARD:请插入母卡
*		E_PAIR_FEED_SUCCESS:子母卡喂养成功
* Return Values
*		CA_RTN_OK 成功
*		CA_RTN_CARD_INVALID	 智能卡无效
*		CA_RTN_CARD_NOTSUPPORT:当前智能卡不支持此功能
*		CA_RTN_DATA_NOT_FIND:	 没有找到所要的数据
*		CA_RTN_DATA_ERROR: 数据错误，智能卡拒绝
*		CA_RTN_FEEDTIME_NOT_ARRIVE:喂养时间未到，子卡不能被喂养
*		CA_RTN_CARD_TYPEERROR:  子母卡喂养失败，插入智能卡类型错误
*
*Notice:
*		Return Values 为第一优先级，如果Return Values 不等于CA_RTN_OK
*	这显示返回错误类型。如果Return Values 等于CA_RTN_OK 再检查
*	eStatus 类型E_PAIR_FEED_SUCCESS, E_PAIR_MAX不显示
**************************************************************************/
WORD CSCASetParentPair(WORD wOperatorsId, BOOL bIsChild, BOOL bIsCanBeFeed, E_PairStatus* eStatus);


/**************************************************************************
* Function Name: CS_CA_HaveNewDetitle
* 供开机的时候检测是否有新的反授权码信息
* Parameters	
*             无
* Return Values
*		TRUE  : 有新的反授权信息，需显示反授权图标
*		FALSE: 没有新的反授权信息
**************************************************************************/
BOOL CS_CA_HaveNewDetitle(void);

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
*Function Name:CS_CA_GetFlashAddress
* 获取同方CA   主数据区的Flash 地址
*Parameters 
*	pcSmcStatus[In/Out]:无
*
*Return Values: 
*一个连续Block (64K) 的起始地址
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CS_CA_GetBackupFlashAddress
*  获取同方CA  备份数据区的Flash 地址(备份区与主写区一定不能在同一个Block)
*Parameters 
*	pcSmcStatus[In/Out]:无
*
*Return Values: 
*一个连续Block (64K) 的起始地址
**************************************************************************/
extern DWORD CS_CA_GetBackupFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CS_CA_GetStbInfo
* 获取当前机顶盒的平台号和序列号
*Parameters 
*	pcSmcStatus[In/Out]:
*	pdwPlatformID [in]: 平台ID, 固定为0x1010;
*	pdwStbIDBCD [in]:   机顶盒序列号，需转换为BCD码. 例如序列号1234（Dec）需转换为0x1234（Hex） 
*	Return Values: 
*	无
**************************************************************************/
extern void CS_CA_GetStbInfo(WORD* pdwPlatformID, ULONG* pdwStbIDBCD);

/**************************************************************************
* Function Name: CS_CA_ClearParentPairStatus
* 清除字母卡配对时保存的状态，退出子母卡配对界面时调用
* Parameters	
*             无
* Return Values
*			  无
**************************************************************************/
void CS_CA_ClearParentPairStatus(void);

/**************************************************************************
* Function Name: CSCAReadFeedDataFromParent
*  子母卡功能, 当前插入是母卡时调用，读取母卡中需要喂养子卡的数据
*  (此接口为中间件为满足广东省网需求而需要调用的接口,其它情况不需要调用)
* Parameters	
*	  	wOperatorsId[in]:  运营商ID
*		pacData[out]:      母卡中需要喂养子卡的数据(传入时buffer大小至少分配256字节)
*       *wLen[out]:        母卡中需要喂养子卡的数据的实际长度
* Return Values
*		CA_RTN_OK 成功
*       CA_RTN_POINTER_INVALID指针无效
*       CA_RTN_CARD_INVALID智能卡无效
*       CA_RTN_CARD_NOTSUPPORT智能卡不支持此功能
*       CA_RTN_DATA_NOT_FIND没有找到所要的数据
**************************************************************************/
WORD CSCAReadFeedDataFromParent(WORD wOperatorsId, BYTE* pacData, BYTE* ucLen);

/**************************************************************************
* Function Name: CSCAWriteFeedDataToChild
*  子母卡功能，当前插入是子卡时调用，将配对信息写入子卡
*  (此接口为中间件为满足广东省网需求而需要调用的接口,其它情况不需要调用)
* Parameters
*       wOperatorsId[in]:  运营商ID
*	  	pbyFeedData[in]:   要写入子卡的数据buffer
*		byLen[in]:         要写入子卡的数据实际长度
* Return Values
*		CA_RTN_OK 成功
*       CA_RTN_POINTER_INVALID指针无效
*       CA_RTN_CARD_INVALID智能卡无效
*       CA_RTN_CARD_NOTSUPPORT智能卡不支持此功能
*       CA_RTN_DATA_NOT_FIND没有找到所要的数据
**************************************************************************/
WORD CSCAWriteFeedDataToChild(WORD wOperatorsId, BYTE* pacFeedData, BYTE ucLen);

#ifdef  __cplusplus
}
#endif

#endif //_CS_TFCA_INTERFACE_H_

