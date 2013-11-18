/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_SHIXUNCA_Interface.h  
* Author:      
* Date:        June 2006
* Description: Sumashixun CA interface
*            
*****************************************************************************************************************************/
#include "udiplus_typedef.h"
#include "cs_cainterface.h"
/****************************************************************************************************************************
* 			                                     Revision History                                                           *
*****************************************************************************************************************************
*   Revision  : V1.0
*   Programmer: 
*   Create    : 
*****************************************************************************************************************************/
#ifndef _CS_SHIXUNCA_INTERFACE_H_
#define _CS_SHIXUNCA_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define CS_SHIXUNCA_MESSAGE_BASE    		(0)
#define CS_SHIXUNCA_INFO_MESSAGE_BASE    	(100)
#define CS_SHIXUNCA_CMD_MESSAGE_BASE    	(200)
#define CS_SHIXUNCA_STATUS_MESSAGE_BASE    	(300)
#define CS_SHIXUNCA_ALARM_MESSAGE_BASE    	(400)
#define CS_SHIXUNCA_MMI_MESSAGE_BASE    	(500)


typedef enum _CaEventGroup
{
	CA_ALARM = CS_SHIXUNCA_MESSAGE_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,	
	CA_EVENTGROUP_MAX
}E_CaEventGroup;

typedef enum _CaInfo
{
	CA_INFO_START = CS_SHIXUNCA_INFO_MESSAGE_BASE,//(CS_SHIXUNCA_MESSAGE_BASE + CA_EVENTGROUP_MAX + 1), /*Information Start*/
	CA_INFO_SMC_STATUS,      /*dwParm2 = (DWORD)&T_SmcInfo*/
	CA_INFO_PPID,            /*dwParm2 = (DWORD)&T_PPIDList*/
	CA_INFO_PPID_DETAIL,     /*dwParm2 = (DWORD)&T_DetailPPIDInfo*/
	CA_INFO_TXT,             /*dwParm2 = (DWORD)&T_Msg*/

	CA_INFO_PG_LEVEL,        /*dwParm2 = (DWORD)&T_RatingValue*/
	CA_INFO_WORKTIME,        /*dwParm2 = (DWORD)&T_WorkTime*/
	CA_INFO_PAIR_INFO,       /*dwParm2 = (DWORD)&T_PairInfo*/
	CA_INFO_GET_BOOK_IPP,    /*dwParm2 = (DWORD)&T_IppsInfo*/
	CA_INFO_VIEWED_IPP,      /*dwParm2 = (DWORD)&T_IppsInfo*/
	
	CA_INFO_PPID_MONEY,      /*dwParm2 = (DWORD)&T_MoneyInfo*/
	CA_INFO_AREA_LOCK_INFO,  /*dwParam2 = (DWORD)&T_AreaInfo*/
	CA_INFO_CARD_MOTHER_INFO,/*dwParam2 = (DWORD)&SmcMotherInfo_S*///cxj add
	
	CA_INFO_MAX
}E_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_START = CS_SHIXUNCA_CMD_MESSAGE_BASE,//(CS_SHIXUNCA_MESSAGE_BASE + CA_INFO_MAX + 1), /*Command Start*/
	CA_COMMAND_IPP_REQUEST,          /*dwParm2 = (DWORD)&T_PpvDetailInfo*/
	CA_COMMAND_FORCE_MSG_HIDE,
	CA_COMMAND_FORCE_MSG_SHOW,       /*dwParm2 = (DWORD)&T_FingerPrintInfo*/
	CA_COMMAND_IPP_CONFIRM_REQUEST,  /*dwParm2 = (DWORD)&T_PpvDetailInfo*/
	CA_COMMAND_URGENCY_BROADCAST,    /*dwParm2 = (DWORD)&T_UrgencyBroadcast*/
	CA_COMMAND_CANCEL_URGENCY_BROADCAST,/*Cancel Urgency Broadcast*/
	CA_COMMAND_CLEAN_MAIL_FLAG,         /*Clean Mail Flag*/
	CA_COMMAND_CHANGESERVICE,			/*Param2 = CS_CA_GetLockFrequency() 的返回值*/
	CA_COMMAND_AREALOCK_OVER,			/*Param2 = E_AreaLockStatus */
	CA_COMMAND_CONFIRM_FREEPREVIEV,     /*Param2 = bConfirmed */
	CA_COMMAND_RESET_STBPIN,     		/*Param2 = (DWORD)&T_STBPin */

	CA_COMMAND_MAX	
}E_CaCommand;

typedef enum _CaStatus
{
	CA_STATUS_START = CS_SHIXUNCA_STATUS_MESSAGE_BASE,//(CS_SHIXUNCA_MESSAGE_BASE + CA_COMMAND_MAX + 1), /*Status Start*/
	CA_STATUS_PIN_ERROR,               /*Pin Code Error*/
	CA_STATUS_PIN_OK,                  /*Pin Code Ok*/
	CA_STATUS_MODIFY_PIN_SUCCESS,      /*Modify Pin Code Success*/
	CA_STATUS_MODIFY_PR_LEVEL_SUCCESS, /*Modify Parent Rating Success*/
	
	CA_STATUS_MODIFY_PR_LEVEL_FAIL,    /*Modify Parent Rating Fail*/
	CA_STATUS_GET_PPID_INFO_FAIL,      /*Get PPID Information Fail*/
	CA_STATUS_GET_DETAIL_PPID_INFO_FAIL, /*Get Detail PPID Information Fail*/
	CA_STATUS_PIN_LEN_ERROR,           /*Pin Code Date Length Error*/
	CA_STATUS_PIN_LOCK,                /*Pin Code Locked*/
	
	CA_STATUS_GET_SMC_INFO_ERROR,      /*10Get Smart Card Information Fail*/
	CA_STATUS_GET_RATING_ERROR,        /*Get Rating Fail*/
	CA_STATUS_GET_WORKTIME_FAIL,       /*Get Work Time Fail*/
	CA_STATUS_SET_WORKTIME_SUCCESS,    /*Set Work Time Success*/
	CA_STATUS_SET_WORKTIME_FAIL,       /*Set Work Time Fail*/
	
	CA_STATUS_GET_PAIR_INFO_FAIL,      /*Get Son And Mother Pair Information Fail*/
	CA_STATUS_PAIRING_SUCCESS,         /*Son And Mother Pair Success*/
	CA_STATUS_PAIRING_FAIL,            /*Son And Mother Pair Fail*/
	CA_STATUS_GET_PPID_MONEY_INFO_FAIL,/*Get PPID Money Information Fail*/
	CA_STATUS_NEED_CHECK_PIN_CODE,    /*Please Check Pin Code First*/
	
	CA_STATUS_PPID_INVALID,           /*20PPID Invalid*/
	CA_STATUS_PRODUCT_EXIST,          /*This Product Has Existed*/
	CA_STATUS_NO_SPACE,               /*There Is No Space*/	
	CA_STATUS_PRODUCT_NOT_FOUND,      /*This Product Not Found*/	
	CA_STATUS_PRICE_INVALID,          /*This Price Is Invalid*/
	
	CA_STATUS_CONFLICT,               /*Time Conflict没有用到*/
	CA_STATUS_COMMUNION_WITH_IC_ERROR,/*Communion with IC card Error*/
	CA_STATUS_MONEY_PACKET_NOT_FOUND, /*Not Found Money Packet*/	
	CA_STATUS_IPPV_EXPIRED,           /*IPP Expired*/	
	CA_STATUS_IPPV_NO_ENOUGH_MONEY,   /*Not Enough Money To Purchase This Program*/
	
	CA_STATUS_SMC_OUTERROR,           /*30Smart Card Is Out Or Error*/
	CA_STATUS_GET_IPP_INFO_FAIL,      /*Get IPP Information Fail*/
	CA_STATUS_BOOK_IPP_OK,            /*Book IPP OK*/
	CA_STATUS_BOOK_IPP_FAIL,          /*Book IPP Fail没有用到*/
	CA_STATUS_GET_VIEWED_IPP_INFO_FAIL,/*Get Viewed IPP Inforamtion Fail*/
	CA_STATUS_GET_AREA_LOCK_INFO_FAIL,    /*Get Area Lock Information Fail*/
	CA_STATUS_GET_MOTHER_INFO_FAIL,    /*36Get Card Mother Info Fail*///cxj add
	CA_STATUS_SMC_OK,    			   /*smartcard ok */
	CA_STATUS_SMC_IN,					/*smartcard in*/
	CA_STATUS_SMC_ERROR,				/*smartcard error*/
	CA_STATUS_MAX
}E_CaStatus;

typedef enum _CaAlarm
{
	CA_ALARM_START = CS_SHIXUNCA_ALARM_MESSAGE_BASE,//(CS_SHIXUNCA_MESSAGE_BASE + CA_STATUS_MAX + 1),/*Alarm Start*/
	CA_ALARM_FREE_PROGRAME,       /*Free Programe*/
	CA_ALARM_SMC_OUT,             /*Please Insert Smart Card*/
	CA_ALARM_SMC_UNKNOW,          /*Unknow Smart Card*/
	CA_ALARM_SMC_CHECKING,        /*Checking Smart Card*/
	
	CA_ALARM_SMC_IN,              /*5Smart Card In*/	
	CA_ALARM_SMC_OK,              /*6Smart Card Ok*/
	CA_ALARM_SMC_ERROR,           /*7Smart Card Error*/	
	CA_ALARM_STB_SMC_NOT_MATCH,   /*STB and SC not Match*/
	CA_ALARM_PR_LEVEL_TOO_LOW,    /*Parental Rating too lowly*/
	
	CA_ALARM_OTHER_CA_ENCRYPT,      /*10Unknow encrypt Program*/
	CA_ALARM_WITH_SUMAVISION_CA,    /*Su Ma Shi Xun CA*/
	CA_ALARM_NO_RIGHT,              /*Have No Decrypt Right*/	
	CA_ALARM_NOT_IN_WATCH_TIME,      /*Program Not In Watch Time*/
	CA_ALARM_SMC_IS_PAIRED_OTHER,    /*Smart Card Is Paired With Other STB*/
	
	CA_ALARM_NO_ENTITLE_PROGRAM,     /*15Not Order This Program*/	
	CA_ALARM_PROGRAM_RESTRICT,       /*This Program Is Restricted节目被限制收看*/
	CA_ALARM_AREA_RESTRICT,          /*This Program Is Restricted Watching At This Area*/
	CA_ALARM_MOTHER_RESTRICT,        /*Please Match With Mother Card*/
	CA_ALARM_PPV_NOT_ENOUGH_TOKENS,  /*Not Enough Money To Purchase This Program*/
	
	CA_ALARM_IPPV_NO_CONFIRM,        /*20This is IPPV-P Program, Please Confirm This Program*/	
	CA_ALARM_IPPV_NO_BOOK,           /*This is IPPV-P Program, Please Book This Program*/
	CA_ALARM_IPPT_NO_CONFIRM,        /*This is IPPV-T Program, Please Confirm This Program*/
	CA_ALARM_IPPT_NO_BOOK,           /*This is IPPV-T Program, Please Book This Program*/
	CA_ALARM_IPPV_NO_ENOUGH_MONEY,   /*This is IPPV-P Program, Not Enough Money To Purchase This Program*/

	CA_ALARM_IPPT_NO_ENOUGH_MONEY,   /*25This is IPPV-T Program, Not Enough Money To Purchase This Program*/
	CA_ALARM_SMC_NOT_SERVER,         /*This Smart Card Is Forbided ServerIC卡被禁止服务*/
	CA_ALARM_NO_ROOM_FOR_NEW_MAIL,   /*There Is No Room To Store New Email*/
	CA_ALARM_CLEAN_MSG,              /*Clean All Alarm Message*/

	CA_ALARM_DATA_INVALID,           /*Data Invalid*/
	CA_ALARM_MONEY_PACKET_NOT_FOUND, /*30Money Packet Not Found*/
	CA_ALARM_TVS_NOT_FOUND,          /*TVS(运营商)Not Found*/
	CA_ALARM_KEY_NOT_FOUND,          /*Key(密钥)Not Found*/
	CA_ALARM_IPPNEED_CALLBACK,       /*Ipp Need Callback(回传)*/
	CA_ALARM_GET_ARER_FAILED,		/*CA_ALARM_GET_ARER_FAILED */
	
	CA_ALARM_FREE_PREVIEWING,       /*This is FreePreview Program */
	CA_ALARM_CANCEL_FREE_PREVIEW,    /*Cancel FreePreview Program */
	
	CA_ALARM_ERROR_MAX
}E_CaAlarm;

typedef enum _SmsStatus
{
	CA_SMC_OUT,
	CA_SMC_INIT,
	CA_SMC_IN,
	CA_SMC_ERROR
}E_SmsStatus;

typedef enum _AreaLockStatus
{
	E_AREALCOK_SUCCESS,
	E_AREALCOK_TIMEOUT,
	E_AREALCOK_MAX
}E_AreaLockStatus;

typedef enum _LanguageType
{
	E_CHINESE_SIM_LANGUAGE = 1, //简体中文
	E_ENGLISH_LANGUAGE,     //英文
	E_LANGUAGE_MAX
}E_LanguageType;


#define CA_MAX_PPID_NAME_LEN       (21)
#define CA_MAX_PRODUCT_NAME_LEN    (21)
#define CA_MAX_SERVICE_NAME_LEN    (21)
#define CA_MAX_IPPV_NUM            (250)
#define CA_IPP_OTHER_INFO_LEN      (44)

typedef struct _CaDateTime
{
	DWORD    m_dwYear;
	BYTE     m_cMonty;
	BYTE     m_cDay;
	BYTE     m_cHour;
	BYTE     m_cMinute;
	BYTE     m_cSecond;
}T_CaDateTime;


typedef struct _IppDetailInfo
{
	BYTE     m_cProgramType; //1:IPPV, 0:IPPT
	WORD     m_cEcmPid;      //ECM PID，没有要求显示
	WORD     m_wPPID_Id;     //运营商ID
	BYTE	 m_cCurInterval; //只有为IPPV-T节目时，此字段才有效：收费间隔,单位由 m_ucUnit决定
	WORD     m_wProduceId;   //产品ID
	BYTE     m_cSlotId;      //钱包ID
	BYTE     m_acProductName[CA_MAX_PRODUCT_NAME_LEN]; //产品名称
	BYTE     m_acPPID_Label[CA_MAX_PPID_NAME_LEN];     //运营商名称
	DWORD    m_dwStartTime;  //用于预定或购买时回传，还有排序时根据该字段进行比较
	T_CaDateTime m_stStartTime;
	DWORD    m_dwDuration;   //持续秒数
	BYTE     m_acServiceName[CA_MAX_SERVICE_NAME_LEN];
	WORD     m_wTppvTapPrice;   //不回传、能录像价格
	WORD     m_wTppvNoTapPrice; //不回传、不可录像价格
	WORD     m_wCppvTapPrice;   //要回传、能录像价格
	WORD     m_wCppvNoTapPrice; //要回传、不可录像价格
	
	//begin 以下字段是用户进行预定或购买时由UI填充传给CA，若是已经订购过的过期IPPT节目当前价格中显示该价格
	WORD     m_wBookedPrice;    //已经预订的价格*/
	BYTE     m_cBookedPriceType;//预订价格类型：0:TppTap;1:TppNoTap;2:CppTap;3:CppNoTap;
	//end
	
	BYTE     m_cBookedInterval; //只有为IPPV-T节目时，此字段才有效
	
	BOOL     m_bConfirmed;        //是否已确认购
	BOOL     m_bBooked;           //是否已预订
	DWORD    m_dwPayTotalBalance; //扣钱总点数
	BYTE     m_cIppStatus;
	//CAS5200新增
	BYTE	 m_ucUnit;			  //收费间隔的单位0 -分钟1-小时2-天3-月4-年
	WORD	 m_wIpptPeriod; 	  //用户订购IPPT的观看周期数,for Philippines LongIPPT，用户进行预定或购买时由UI填充传给CA。
}T_IppDetailInfo;

typedef struct _IppsInfo
{
	DWORD    m_dwIppCount;
	T_IppDetailInfo m_astIppInfo[CA_MAX_IPPV_NUM];
}T_IppsInfo;

typedef struct _ViewedIppDetailInfo
{
	WORD	m_wPPID;                                    //运营商编号
	BYTE    m_acPPID_Label[CA_MAX_PPID_NAME_LEN];       //运营商名称
	BYTE	m_acProductName[CA_MAX_PRODUCT_NAME_LEN]; //产品名称
	DWORD	m_dwStartTime;                              //开始时间，time_t格式
	T_CaDateTime m_stStartTime;
	DWORD	m_dwDuration;                                //持续秒数
	WORD	m_wBookedPrice;                              //预订价格(分)
	BYTE	m_cBookedPriceType;                          //预订价格类型：0:TppTap;1:TppNoTap;2:CppTap;3:CppNoTap;
	BYTE	m_cBookedInterval;                           //预订收费间隔，IPPV时此值为0
	BYTE    m_acOtherInfo[CA_IPP_OTHER_INFO_LEN];        //ippv时为“此产品为ippv产品”，ippt时为“观看总时间：？分钟，扣钱总数：？分”
	//CAS5200新增
	BYTE	 m_ucUnit;			  //收费间隔的单位0 -分钟1-小时2-天3-月4-年
}T_ViewedIppDetailInfo;

typedef struct _ViewedIppsInfo
{
	DWORD    m_dwViewedIppCount;
	T_ViewedIppDetailInfo m_astIppInfo[CA_MAX_IPPV_NUM];
}T_ViewedIppsInfo;

#define	 CA_MAX_FINGERPRINT_CONTENT_LEN	  (20)
typedef struct _FingerPrintInfo
{
	BOOL     m_bPriporityHigh; //优先级，对于指纹信息，其优先级都高，为TRUE
	WORD     m_wDuration;      //指纹显示时间长度，单位:秒
	BYTE     m_acMsg[CA_MAX_FINGERPRINT_CONTENT_LEN]; //指纹内容
}T_FingerPrintInfo;//指纹信息

/*Mail*/
#define CA_MAIL_TITLE_LEN         (21)
#define CA_MAIL_SENDER_NAME_LEN   (11)
#define CA_MAIL_TXT_LEN           (1025)

typedef struct _Msg
{
	DWORD    m_dwVersion;       //当m_bPriporityHigh为TRUE时，表示OSD的优先级，当m_bPriporityHigh为FALSE时，表示CAS邮件版本
	BOOL     m_bPriporityHigh;  //FALSE: Mail  TRUE: OSD
	WORD     m_wDuration;       //只有m_bPriporityHigh为TRUE才有效,if wDuration is 0, this means display once
	BYTE     m_szSenderName[CA_MAIL_SENDER_NAME_LEN];//发送者姓名
	DWORD    m_dwTitleLen;                 //标题长度
	BYTE     m_szTitle[CA_MAIL_TITLE_LEN]; //标题
	DWORD    m_dwContentLen;               //邮件内容长度
	BYTE     m_szContent[CA_MAIL_TXT_LEN]; //邮件内容
	T_CaDateTime    m_stSendTime;          //邮件发送时间
}T_Msg;

typedef struct _CaMail
{	
	BOOL            m_bReadFlag;    //已读标志
	T_CaDateTime    m_stArriveTime; //邮件到达的时间
	T_Msg           m_stMsg;
}T_CaMail;

#define CA_MAX_CAS_VER_LEN         	(10)
#define CA_MAX_COS_VER_LEN         	(10)
#define CA_MAX_SMC_NUMBER_LEN      	(17)
#define CA_MAX_SERCIAL_NO_LEN      	(64)
#define CA_MAX_CAS_NAME_LEN        	(21)
#define CA_MAX_STBPIN_LEN  	   		(8)

typedef struct _SmcInfo
{
	BYTE      m_cSmcStatus;//0:OUT, 1:IN
	WORD      m_wSystemId; //CA系统ID号
	BOOL      m_bIsLock;   //TRUE:Lock FALSE:Unlock
	BYTE      m_acCasVer[CA_MAX_CAS_VER_LEN]; //智能卡CAS版本信息
	BYTE      m_acCosVer[CA_MAX_COS_VER_LEN]; //智能卡COS版本信息
	BYTE      m_acSmcNumber[CA_MAX_SMC_NUMBER_LEN]; //智能卡号
	BYTE      m_acSercialNo[CA_MAX_SERCIAL_NO_LEN]; //智能卡序列号
	BYTE      m_acCasManuName[CA_MAX_CAS_NAME_LEN]; //CAS供应商名称
	BYTE      m_acStbCasVer[CA_MAX_CAS_VER_LEN];    //CAS版本号
	T_CaDateTime m_stSmcExpireTime; //智能卡被授权时间
	BYTE      m_cParentalRating;    //智能卡观看级别
}T_SmcInfo;

/*cxj add begin*/
typedef struct _SmcMotherInfo
{	
	T_CaDateTime m_sSetTime; //设置时间
	DWORD        m_dwMotherCardID;//当pdwMotherCardID的值为0，则表示当前卡为母卡， 如果值不为0，表示当前卡为子卡，值为母卡卡号
	BYTE         m_ucFlag;//6bits保留，2bits使用标识
	DWORD        m_dwDecryptTime;//解密CW的次数
	DWORD        m_dwInterTime;//收看间隔时间
	T_CaDateTime m_sCorrespondTime;//子卡配对时间	
}SmcMotherInfo_S;
/*cxj add end*/

#define CA_MAX_PPID_NUM       (10)
typedef struct _PPID
{
	DWORD    m_dwPPIDId; //运营商ID号
	BYTE     m_acPPIDName[CA_MAX_PPID_NAME_LEN];//运营商名称
	BYTE     m_cServiceStatus;
}T_PPIDInfo;//运营商基本信息

typedef struct _PPIDInfo
{
	BYTE         m_acCount;
	T_PPIDInfo   m_stPPIDInfo[CA_MAX_PPID_NUM];
}T_PPIDList;


#define CA_MAX_SERVICE_ENTITLE_NUM   (255)
typedef struct _ServiceEntitle
{
	WORD         m_wProductId;    //产品ID号
	T_CaDateTime m_stEntitleTime; //授权时间(购买时间)
	BOOL         m_bTapingFlag;   //录制标志
	T_CaDateTime m_stStartTime;   //开始时间
	T_CaDateTime m_stEndTime;     //结束时间
	BYTE         m_acProductName[CA_MAX_PRODUCT_NAME_LEN];//产品名称
}T_ServiceEntitle;

typedef struct _DetailPPIDInfo
{
	DWORD    m_dwPPIDId;      //运营商ID号
	BYTE     m_cEntitleCount; //授权数量
	T_ServiceEntitle m_stServicEntitle[CA_MAX_SERVICE_ENTITLE_NUM];
}T_DetailPPIDInfo;

typedef struct _RatingValue
{
	BYTE      m_cRatingValue; //观看级别
}T_RatingValue;

typedef struct _WorkTime
{
	//采用24小时计时制
	BYTE      m_cStartTimeHour;
	BYTE      m_cStartTimeMinute;
	BYTE      m_cEndTimeHour;
	BYTE      m_cEndTimeMinute;
}T_WorkTime;

typedef struct _UrgencyBroadcast
{
	WORD      m_wOriNetID;  //原始网络ID号
	WORD      m_wTsID;      //流ID号
	WORD      m_wServiceID; //节目ID号
	WORD      m_wDuration;  //应急广播持续时间
}T_UrgencyBroadcast;

#define CA_MAX_PAIR_INFO_LEN   (250)
typedef struct _PairInfo
{
	BYTE      m_cLen;
	BYTE      m_acData[CA_MAX_PAIR_INFO_LEN]; //子母卡配对信息
}T_PairInfo;

typedef struct _MoneyInfo
{
	WORD      m_wPPID;        //运营商ID
	DWORD     m_dwAllBalance; //已花费钱数
	DWORD     m_dwRemainder;  //剩余钱数
}T_MoneyInfo;//如果已花费钱数和剩余钱数都为0，则表示获取钱包信息失败

typedef struct _AreaInfo
{
	DWORD			m_dwCardArea;			//卡的区域码
	DWORD			m_dwSetCardAreaTime;	//设定区域时间。
	BYTE			m_cStartFlag;			//启动标志。1:启动；0:没有。
	DWORD			m_dwSetFlagTime;		//设置标志时间。
	DWORD			m_dwSetStreamTime;		//设置特征码流的时间。
	DWORD			m_dwIntervalTime;		//卡中当前时间减去特征码时间的最大值。
	DWORD			m_dwStreamMinArea;		//码流的最小区域码
	DWORD			m_dwStreamMaxArea;		//码流的最大区域码
	T_CaDateTime    m_stSetCardAreaTime; 	//区域时间
}T_AreaInfo;

typedef struct _STBPin
{
	BYTE			m_acSTBPIN[CA_MAX_STBPIN_LEN];		//机顶盒PIN码
	BYTE			m_cLen;								//PIN码的长度
}T_STBPin;

typedef struct _CaShumaPrivate
{
    BOOL m_bFakeEeprom; //TRUE: 开启伪EEPROM 功能
    DWORD m_dwFakeEepromAddr; //伪EEPROM地址
    DWORD m_dwFakeEepromLength; //伪EEPROM长度 
    DWORD m_dwFakeEepromBakAddr; //伪EEPROM 备份 地址
    DWORD m_dwFakeEepromBakLength; //伪EEPROM备份长度 
    
    DWORD m_dwReseve1;
    DWORD m_dwReseve2;
    DWORD m_dwReseve3;
    DWORD m_dwReseve4;
}CaShumaPrivate_S;
/**************************************************************************
*Function Name: CS_CA_GetCoreVer
*  获取算通CA内核版本信息
*
*Parameters 
*	pcVer[out]:
*		用于存储CA内核版本信息缓存 
*   pnLen[in|out]:
*       输入初始值为30，返回真实值
*
*Return Values: 
*	0 : SUCCESS 获取CA内核版本信息成功
*	-1: FAILURE 参数错误或获取CA内核版本失败
**************************************************************************/
INT CS_CA_GetCoreVer(char* pcVer, INT* pnLen);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* 保存一封邮件
*
*Parameters 
*	pstTxt[in]:邮件内容
*
*Return Values: 
*	0 : SUCCESS 保存一封邮件到内存中成功
*	-1: FAILURE 参数错误或保存邮件失败
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
*	0 :  册除某一封邮件成功
*	-1:  索引号错误或删除邮件失败
*    1:  不是此用户的邮件不能删除
**************************************************************************/
INT CS_CA_TXT_Del(INT nIndex);

/**************************************************************************
*Function Name:CS_CA_TXT_DelAll
* 删除所有保存过的邮件
*
*Parameters 
*	无
*
*Return Values: 
*	0 :  删除所有邮件成功
*	-1:  删除所有邮件失败
*    1:  不是此用户的邮件不能删除
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
*	0 : 没有邮件
*	其它: 邮件总个数
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* 获取具体某一封邮件的内容
*
*Parameters 
*	nIndex[in]:邮件下标，从0开始
*	pstTxt[out]:CA将读出的邮件内容存储到上层分配好的空间
*
*Return Values: 
*	0 : 获取邮件内容成功
*	-1: 索引号错误或参数错误，或获取邮件内容错误
*    1: 不是此用户的邮件不能查询
**************************************************************************/
INT CS_CA_TXT_GetInfo(INT nIndex, T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* 将所有邮件保存到flash中，一般是关机或断电前进行的操作
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 同步FLASH成功
*	-1: FAILURE 同步FLASH失败
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_TXT_SetReadFlag
* 设置邮件是否已读标志
*
*Parameters 
*	nIndex[in]:邮件存储的下标，从0开始
*	bRead[in]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 设置读标志成功
*	-1: FAILURE 索引号错误或设置读标志失败
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(INT nIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* 返回当前还有多少封未读邮件
*
*Parameters 
*	无
*
*Return Values: 
*   0:没有未读邮件
*   其它：未读邮件个数
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_MMI_CheckPinCode
* 请求较验密码
*
*Parameters 
*	pstPinCode[in]:输入密码
*
*Return Values: 
*	0 : SUCCESS 参数合法，结果将通过发消息给UI
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_CheckPinCode(DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyPinCode
* 请求修改密码
*
*Parameters 
*	pstOldPin[in]:旧密码
*   pstNewPin[in]:新密码
*
*Return Values: 
*	0 : SUCCESS 参数合法，结果将通过发消息给UI
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_ModifyPinCode(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestBookPpvMenu
* 获取预订|退订PPV信息
*
*Parameters 
*   无
*
*Return Values: 
*	0 : 其结果将通过发消息给UI
**************************************************************************/
INT CS_CA_MMI_RequestBookPpvMenu();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestViewedPpvManu
* 获取已观看的PPV信息
*
*Parameters 
*   无
*
*Return Values: 
*	0 : 其结果将通过发消息给UI
**************************************************************************/
INT CS_CA_MMI_RequestViewedPpvManu();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestViewedPpvManu
* 预订|退订	IPP节目
*
*Parameters 
*   pstIppInfo[IN]
*
*Return Values: 
*	0 : 其结果将通过发消息给UI
*	    预定时需要处理以下消息:
*			CA_STATUS_BOOK_IPP_OK//预订成功
*			CA_STATUS_IPPV_NO_ENOUGH_MONEY//预订成功，但余额不多，请及时充值
*			CA_STATUS_NEED_CHECK_PIN_CODE//需要通过PIN验证
*			CA_STATUS_COMMUNION_WITH_IC_ERROR//与IC卡通讯参数错误
*			CA_STATUS_PPID_INVALID//运营商ID无效
*			CA_STATUS_MONEY_PACKET_NOT_FOUND//钱包没有发现
*			CA_STATUS_IPPV_EXPIRED//产品已经过期
*			CA_STATUS_PRODUCT_EXIST//产品已经存在，不能操作
*			CA_STATUS_NO_SPACE//没有空间
*			CA_STATUS_PRODUCT_NOT_FOUND//产品没有发现
*			CA_STATUS_PRICE_INVALID//价格无效
*			CA_STATUS_BOOK_IPP_FAIL//未知错误
*		退订时:
*			CA_STATUS_BOOK_IPP_OK//退订成功
*			其他:退订失败，请重试
*  -1 : 参数不合法
**************************************************************************/
INT CS_CA_MMI_PpvBook(T_IppDetailInfo* pstIppInfo);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestViewedPpvManu
* 退出在播放界面弹出的购买IPP节目，即不购买
*
*Parameters 
*   wEcmPid[IN] : 当前IPP节目的ECMPID
*
*Return Values: 
*	0 : SUCCESS
*
*备注: 单路解扰库使用此接口,多路解扰库请使用CS_CA_MMI_PpvCancelEX
**************************************************************************/
INT CS_CA_MMI_PpvCancel(WORD wEcmPid);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvCancelEX
* 退出在播放界面弹出的购买IPP节目，即不购买
*
*Parameters 
*   wEcmPid[IN] : 当前IPP节目的ECMPID
*   hService[IN]: 当前节目播放的句柄(CSCAStartService得到的句柄)
*
*Return Values: 
*	0 : SUCCESS
*
*备注: 多路解扰库使用该接口。
**************************************************************************/
INT CS_CA_MMI_PpvCancelEX(WORD wEcmPid, HCSHANDLE hService);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPPIDMoneyInfo
* 获取对应运营商所花钱数和剩余钱数信息
*
*Parameters 
*   wPPID[in]:运营商ID
*
*Return Values: 
*	0 : SUCCESS 目前该函数只会返回成功，结果将通过发消息给UI
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_RequestPPIDMoneyInfo(WORD wPPID);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestWorkTime
* 获取工作时段
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 目前该函数只会返回成功，结果将通过发消息给UI
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_RequestWorkTime();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyWorkTime
* 设置工作时段
*
*Parameters 
*	dwPin[in]:输入PinCode
*	pstWorkTime[in]:开始和结束工作时间
*
*Return Values: 
*	0 : SUCCESS 参数合法，结果将通过发消息给UI
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_ModifyWorkTime(DWORD dwPin, T_WorkTime* pstWorkTime);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestSmcInfo
* 请求智能卡信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 目前该函数只会返回成功，结果将通过发消息给UI
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_RequestSmcInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPPIDInfo
* 请求PPID信息(运营商信息)
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 目前该函数只会返回成功，结果将通过发消息给UI
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_RequestPPIDInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestDetailPPIDInfo
* 请求PPID详细信息(运营商详细信息)
*
*Parameters 
*	pstPin[in]:密码
*   dwPpId:运营商Id号
*
*Return Values: 
*	0 : SUCCESS 参数合法，结果将通过发消息给UI
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_RequestDetailPPIDInfo(DWORD dwPin, DWORD dwPpId);

/**************************************************************************
*Function Name:CS_CA_MMI_NotifyCaShowOSDOver
* 通知CAOSD显示完成
*
*Parameters 
*	dwDuration[in]:显示完所花费的时间，单位：秒
*
*Return Values: 
*	0 : SUCCESS 目前该函数只会返回成功，是否成功不会通知UI
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_NotifyCaShowOSDOver(DWORD dwDuration);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestParentRating
* 获取观看级别
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 目前该函数只会返回成功，获取级别信息或获取信息失败将通过发消息给
UI
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_RequestParentRating();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyParentRating
* 修改观看级别
*
*Parameters 
*	pstPin[in]:输入密码
*   cParentRatingValue[in]:要修改的级别
*
*Return Values: 
*	0 : SUCCESS 参数合法，其结果将通过发消息给UI
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_ModifyParentRating(DWORD dwPin, BYTE cParentRatingValue);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPairInfo
* 获取子母卡配对所需信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
目前该函数只会返回成功，子母卡配对所需信息或获取信息失败将通过发消息给UI
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_RequestPairInfo();

/**************************************************************************
*Function Name:CS_CA_Pairing
* 子母卡配对
*
*Parameters 
*	pstPairInfo[in]: 配对信息
*
*Return Values: 
*	0 : SUCCESS 参数正确，配对是否成功将通过发消息给UI
*	-1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_Pairing(T_PairInfo* pstPairInfo);

/**************************************************************************
*Function Name:CS_CA_MMI_IsPinLock
* 查询PIN码是否被锁
*
*Parameters 
*	无
*
*Return Values: 
*	TRUE : PIN码被锁 
*	FALSE: PIN码未被锁
**************************************************************************/
BOOL CS_CA_MMI_IsPinLock(void);

/**************************************************************************
*Function Name:CS_CA_MMI_LockPin
* 锁住PIN码
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS(锁PIN码成功) 
*	-1: FAILURE(锁PIN码失败)
**************************************************************************/
INT CS_CA_MMI_LockPin(void);

/**************************************************************************
*Function Name:CS_CA_MMI_IsPinPass
* 查询是否通过校验密码
*
*Parameters 
*	无
*
*Return Values: 
*	TRUE : 通过PIN码校验 
*	FALSE: 未通过PIN码校验
**************************************************************************/
BOOL CS_CA_MMI_IsPinPass(void);

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
INT CS_CA_MMI_GetSmcStatus(E_SmsStatus* pcSmcStatus);

/**************************************************************************
*Function Name:CS_CA_StartBoardCast
* 启动数据广播
*
*Parameters 
*	pcPmtData[In]:Pmt数据
*	nPmtLen[In]:Pmt长度
*	pcCatData[In]:Cat数据
*	nCatLen[In]:Cat长度
*	wTsId[In]:流ID
*	wOrginalNetworkID[In]:原始网络ID
*
*Return Values: 
*	SUCCESS 启动数据广播成功
*	FAILURE 启动数据广播失败
**************************************************************************/
INT CS_CA_StartBoardCast(BYTE* pcPmtData, INT nPmtLen, BYTE* pcCatData, INT nCatLen, WORD wTsId, WORD wOrgNetId);

/**************************************************************************
*Function Name:CS_CA_StopBoardCast
* 停止当前数据广播
*
*Parameters 
*	无
*
*Return Values: 
*	SUCCESS 停止数据广播成功
*	FAILURE 停止数据广播失败
**************************************************************************/
INT CS_CA_StopBoardCast(void);

/************************************************************************
* s_cSupportFakeFlag = 1 表示CA处理了假加密
* (如果没有调用此函数，默认s_cSupportFakeFlag = 1，表示处理了假加密情况)
* s_cSupportFakeFlag = 0 表示CA不处理假加密
*************************************************************************/
INT CS_CA_SetFakeFlag(BYTE cFakeFlag);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestAreaBlockInfo
* 获取频点区域锁定信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : 其结果将以消息形式上报UI
**************************************************************************/
INT CS_CA_MMI_RequestAreaBlockInfo();

/**************************************************************************
*Function Name:CS_CA_SetLanguageMode
* 设置语言种类
*
*Parameters 
*	eLanguageType[in]:语言类型
*
*Return Values: 
*	0 : SUCCESS
*	-1: FIAILURE
**************************************************************************/
INT CS_CA_SetLanguageMode(E_LanguageType eLanguageType);

/**************************************************************************
*Function Name:CS_CA_ProcAreaLockCatInfo
* 通知区域锁定频点的CAT表(当调用APP提供的CS_CA_NotifyAppForbidSwitchChannel()函数时，APP要做
* <1>存储当前播放的节目信息; <2>跳转到设置的区域锁定频点; <3>不允许用用户操作; <4>请求CAT表; 
* <5>当请求到CAT表后调用CS_CA_ProcAreaLockCatInfo()函数, 将CAT抛给CA模块)
*
*Parameters 
*	pcSectionData[in]:
*		CAT 数据缓存区地址 
*     nLen [in]:
*		CAT数据长度
*	cDemux[in]:
*		通道号
*
*Return Values: 
*	0 : SUCCESS 通知CA新CAT数据成功
*	-1: FAILURE 参数错误或通知CA新CAT数据失败
**************************************************************************/
INT CS_CA_ProcAreaLockCatInfo(BYTE *pcData, INT nLen, BYTE cDemux);

/*cxj add begin*/
/**************************************************************************
*Function Name:CS_CA_MMI_GetMotherInfo
* 获取智能卡的字母卡信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : 其结果将以消息形式上报UI
**************************************************************************/
INT CS_CA_MMI_GetMotherInfo(void);

/**************************************************************************
*Function Name:CSCACASDebugMsgAdd
* 控制数码视讯库中打印的开关
*
*Parameters 
*	bOpen[in]:
*   TRUE,打开打印; FALSE,关闭打印，打开打印时USPPrint(ERROR_LEVEL,...)的打印要能打出来
*
*Return Values: 
*	无
**************************************************************************/
void CSCACASDebugMsgAdd(BOOL bOpen);
/**************************************************************************
*Function Name:CS_CA_PDSDProc
* 设置当前使用的PDSD值，用于区域控制
*
*Parameters
*       dwPDSD[in]     : 当前使用的PDSD值，根据运营商的需求，
*                        获得相应的PDSD值后，通过该接口传给CA
*
*Return Values: 
*	0 : SUCCESS 设置成功
*   -1: FAILURE 设置失败
**************************************************************************/
INT CS_CA_PDSDProc(DWORD dwPDSD);

/**************************************************************************
*Function Name:CS_CA_MMI_ConfirmFreePreview
* 通知CA是否还继续免费预览
*
*Parameters
*       bConfirmed     : 是否确认免费预览
*                     
*Return Values: 
*	0 : SUCCESS 设置成功
*   -1: FAILURE 设置失败
**************************************************************************/
INT CS_CA_MMI_ConfirmFreePreview(BOOL bConfirmed);

#define CA_ESPID_MAX_NUM     (8)
#define CA_EMMPID_MAX_NUM    (8)

typedef struct _CSCAEcmPidInfo
{
	WORD     m_wEsPid;
	WORD     m_wEcmPid;
}CSCAEcmPidInfo_S;

typedef struct _CSCAEmmPidInfo
{
	WORD     m_wDmxId;
	WORD     m_wEmmPid;
}CSCAEmmPidInfo_S;

typedef struct _CASpecificInfo
{
	WORD     m_wEmmPidNum;
	WORD     m_wEsNum;
	CSCAEcmPidInfo_S m_astEcmPidInfo[CA_ESPID_MAX_NUM];
	CSCAEmmPidInfo_S m_astEmmPidInfo[CA_EMMPID_MAX_NUM];
}CASpecificInfo_S;

/**************************************************************************
*Function Name:CSCAGetCASpecificInfo
* 获取当前播放节目的ECM/EMM PID信息
*
*Parameters
*       hService[in]     : 启动播放时CA返回的Handel，如果启动CA是USP机制的话
*          即调用的是CS_CA_StartSrvPlay这套接口，那hService传0xFFFFFFFF
*		pstCaInfo[in/out]: 存储当前播放节目的ECM/EMM PID信息
*
*Return Values: 
*	0 : SUCCESS 获取信息成功
*   -1: FAILURE 参数错误
**************************************************************************/
INT CSCAGetCASpecificInfo(HCSHANDLE hService, CASpecificInfo_S* pstCaInfo);

/*cxj add end*/

#ifdef  __cplusplus
}
#endif

#endif //_CS_SHIXUNCA_INTERFACE_H_

