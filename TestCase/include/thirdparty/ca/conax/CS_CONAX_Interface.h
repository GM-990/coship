/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_CONAX_Interface.h  
* Author:      xiejianghua
* Date:        Dec 2005 
* Description: CONAX CA interface
*            
*******************************************************************************/

#include "udiplus_typedef.h"
#include "cs_cainterface.h"
#include "cs_cainterface_pvrext.h"

/*******************************************************************************
* 			                                  Revision History                                				     *
********************************************************************************
* - Revision 1.0  2005/12/31
*   Programmer:Xie jianghua
*   Create.
*
*- Revision 2.0 2007/4/17
*   Programmer:Xiejianghua
*   Refactored
*- Revision 3.0 2008/11/25
*   Programmer:heyingbo
*   Restructure
*- Revision 4.0 2010/11/29
*   Programmer:lihongqiang
*   Restructure
*******************************************************************************/
#ifndef _CS_CONAX_INTERFACE_H_
#define _CS_CONAX_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

//Memory Pairing mode
//#define VERIFIER_MODE		0

//Chipset Pairing mode
//#define CHIPSET_PAIRING		1


#define CWPK_WITH_LPK   (1)

#define	CA_ALARM		0   //  ALARM 消息类型
#define	CA_COMMAND	    1   //  COMMAND 消息类型
#define	CA_STATUS		2   //  STATUS 消息类型

#define CA_MSG_BASE                  0    //conax消息起始值

#if defined(SUPPORT_MAXFP)    //AVG项目对指纹长度要求很大
#define CA_MAX_FINGERPRINT_LEN		256/*32*/    //指纹信息的最大长度
#else
#define CA_MAX_FINGERPRINT_LEN		32/*32*/    //指纹信息的最大长度
#endif

#define CA_MAX_LABEL_NAME_LEN       32    //描述信息的最大长度
#define CA_MAX_DEBIT_NUM         	100   //DEBIT最大记录数
#define CA_MAX_CREDIT_NUM         	100   //credit最大记录数
#define CA_MAX_TEXT_SIZE			256   //短消息最大长度
#define CA_MAX_ES_NUM				16    //最多可解的ES个数
#define CA_MAX_CASYSTEMID_NUM       256   //最大CA系统ID个数
#define CA_MAX_CURRENCY_LEN			8     //CURRENCY最大长度

//父母级
typedef enum
{
	RAT_G,                //for the family
	RAT_PG,               //with parental guidance
	RAT_A,                //over 18
	RAT_X,                //erotic
    RAT_XXX               //erotic//hyb@090817
}T_CaMaturityRating;

typedef struct
{
	DWORD		   m_dwYear; 
	DWORD		   m_dwMonth;
	DWORD		   m_dwDay;
	DWORD		   m_dwHour;
	DWORD		   m_dwMinute;
	DWORD		   m_dwSecond;
	DWORD		   m_dwCentisecond;
}T_CaDateTime;

typedef struct
{   
    WORD                m_wLanguage;        //language,should be displayed as a decimal number with 1-3 digits
    BYTE         		m_cInterfaceVer;    //interface version
    BYTE         		m_cNumOfSession;    //number of session,should be displayed as a decimal number
    BYTE			    m_cSoftVersion[20]; //software version
    BYTE                m_szCardId[16];     //card number, should be displayed as a decimal number, like" XXX XXXX XXXX - X"
    BYTE			    m_acChipId[16];     //chip id,display as 3-4-4 format,interval space.eg:123 4567 8901
    WORD                m_awCaSysId[CA_MAX_CASYSTEMID_NUM];//ca system id, each one should be displayed as a hexadecimal number with four digits
    BYTE                m_cCaSysIdNum;      //number of ca system id
} T_CaConaxInfo;

typedef struct
{
	WORD		m_wMinutesLeft;
	BYTE		m_acLabel[CA_MAX_LABEL_NAME_LEN];//program name
	DWORD		m_dwProductRef;                  //no need to display for the moment
}T_CaAcceptViewInfo;

typedef struct
{
	WORD       m_wPurseBalance;         // 当前使用钱包的余额
	BYTE       m_cInsufficientTokens;   // 1: 卡内余额不足
	BYTE       m_cTokensMode;           // 0: ppv per event       1: ppv per time
	DWORD      m_dwEventcost;           // 节目价格
	DWORD      m_dwEventTag;
	BYTE       m_acProgramLabel[CA_MAX_LABEL_NAME_LEN];// 节目名称，字符串
	BYTE	   m_acPurseLabel[CA_MAX_LABEL_NAME_LEN]; //钱包名称，字符串
}T_CaPPVRequestInfo;

typedef struct
{
	DWORD 		m_dwProductRef; //no need to display for the moment
	float 		m_fPrice;       // 节目价格
    BYTE        m_acCurrency[CA_MAX_CURRENCY_LEN];
	BYTE  		m_acProductLabel[CA_MAX_LABEL_NAME_LEN];//名称
}T_CaOrderInfo;

//购买记录
typedef struct
{ 
    T_CaDateTime	  m_stStart;        //  年、月、日有效，其他无效
    T_CaDateTime	  m_stEnd;          //  年、月、日有效，其他无效
    DWORD	          m_dwEntitlement;  //标识卡的授权信息
} T_CaAuthPeriod;

typedef struct
{
    BYTE                m_szLabel[CA_MAX_LABEL_NAME_LEN];//名称
    WORD	            m_wRefID;           //节目代号
    T_CaAuthPeriod   	m_astAuthPeriod[2]; //授权信息
}T_CaSubscriptionInfo;

//ppv Event 记录
typedef struct
{   
    BYTE		   	m_szLabel[CA_MAX_LABEL_NAME_LEN];//名称
    DWORD	   		m_dwEventID;                     //节目代号
    T_CaDateTime	m_stStart;                       // 秒、厘秒无效。其他有效
    T_CaDateTime	m_stEnd;                         // 秒、厘秒无效。其他有效
    WORD           	m_MinutesOrCreditsLeft;          //剩余金额或点数
    WORD           	m_wAuthType;                     //0:time-limited access; 1:unlimited access; 2:credit-based access; 3:reserved
} T_CaPPVEventInfo;

typedef struct
{
	WORD			m_wPurseRef;//钱包代号
	DWORD			m_dwBalance;//余额
	BYTE			m_acLabel[CA_MAX_LABEL_NAME_LEN]; //名称
}T_CaPurseInfo;

typedef struct
{
	DWORD			m_dwEventTag;
	DWORD			m_dwConsumedTokens;              //已消费的金额
	BYTE			m_acLabel[CA_MAX_LABEL_NAME_LEN];//名称
	T_CaDateTime	m_stLastAccessTime;              //厘秒无效、其他有效
}T_CaDebitStatus;

typedef struct
{
	DWORD		         m_dwTotalNum;                     //记录个数
	T_CaDebitStatus      m_stDebitStatus[CA_MAX_DEBIT_NUM];//消费记录
}T_CaDebitInfo;

typedef struct
{
	DWORD		  	m_dwCreditTokens;                //点数
	BYTE		  	m_acLabel[CA_MAX_LABEL_NAME_LEN];//名称
}T_CaCreditStatus;

typedef struct
{
	DWORD		           m_dwTotalNum;                       //信息个数
	T_CaCreditStatus       m_stCreditStatus[CA_MAX_CREDIT_NUM];//交易信息
}T_CaCreditInfo;

typedef struct
{
	BYTE		    m_cId;                     //消息ID
	WORD		    m_wDuration;               //每次显示持续时间
	BYTE		    m_cRepetition;             //显示次数
	WORD		    m_wInterval;               //显示间隔时间
										//conax文档说明短消息间隔范围为:10s-2560s,原来间隔时间类型为BYTE型最大只支持255，
										//为了兼容间隔大于255时能正常显示短消息，改为WORD类型//hulan 2013.5.22
	T_CaDateTime    m_stShowTime;              //开始显示时间，厘秒无效、其他有效
	BYTE		    m_cTextLen;                //内容长度
	BYTE		    m_acText[CA_MAX_TEXT_SIZE];//短消息内容
}T_CaMessage;

typedef struct
{
	BOOL			m_bRead;            //已读标识
	BYTE			m_cMailId;          //邮件ID
	T_CaDateTime	m_stShowFlagTime;   //FLAG开始显示(UTC时间),邮件为直接显示邮件图标
	T_CaDateTime	m_stReceiveTime;    //接收时间
	BYTE			m_cTextLen;         //内容长度
	BYTE			m_acText[CA_MAX_TEXT_SIZE]; //邮件内容
}T_CaEmail;

typedef struct
{
	WORD			m_wDuration;    //持续时间,单位为厘秒（百分之一秒)
	WORD			m_wPosition_x;  //左上角X坐标
	WORD			m_wPosition_y;  //左上角X坐标
	BYTE			m_cHeight;      //左上角Y坐标
	T_CaDateTime	m_stShowTime;   //年、月、日无效，其他有效
	BYTE			m_acFingerText[CA_MAX_FINGERPRINT_LEN];//指纹内容
	INT             m_nLength;
}T_CaFingerprint;

typedef enum
{	
	CA_COMMAND_NEW_EMAIL = CA_MSG_BASE,//0 from Emm/ECM dwPram2 = (DWORD)&T_CaMessage	
	CA_COMMAND_SHORT_MSG,              //1 from Emm/ECM dwPram2 = (DWORD)&T_CaMessage
	CA_COMMAND_FINGER_PRINT,           //2 from Emm/ECM dwPram2 = (DWORD)&T_CaFingerprint
	CA_COMMAND_CLEAN_EMAIL_FLAG,       //3 notify to clean the email icon 
    CA_COMMAND_RESET_STB_PIN,          //4 just for France island project
    CA_COMMAND_STOP_FINGER_PRINT,      //5 这个消息应用不要处理了，由于显示时间精度原因需应用自己处理
    CA_COMMAND_STOP_SHORT_MSG,         //6 stop short-msg,conax5(包括conax5)后支持,之前版本不抛这个消息
    
	CA_COMMAND_MAX
}T_CaCommand;

//应用处理开音视频CA_ALARM_CW_SET_SUCCESS，关音视频CA_ALARM_CW_LOST，其它消息不需要处理开关音视频
typedef enum
{
	CA_ALARM_CANCEL = (CA_MSG_BASE+200),//200 used for cancel alarmmsg topbar except emailinco shortmsg fp
	CA_ALARM_OTHER,                     //201 "Other CA program"
	CA_ALARM_CARD_INSERT,               //202 "Card Checking… "
	CA_ALARM_CARD_REMOVE ,              //203 "No card inserted"
	CA_ALARM_CARD_INCORRECT,            //204 "Incorrect card:x",if dwParm2 != 0, it means the time the banner will be displayed
	CA_ALARM_CARD_INCORRECT_CONAX ,     //205 "Incorrect Conax card"
	CA_ALARM_CARD_COMMUNICATION_ERROR,  //206 "Card problem - check card"
	CA_ALARM_NO_RIGHT,                  //207 "No access"
	CA_ALARM_PPV_PREVIEW,               //208 "PPV Preview"
	CA_ALARM_PPV_EXPIRED,		        //209 "PPV Expired"
	CA_ALARM_GEOGRAPHICAL_BLACKOUT ,    //210 "No access - geographical blackout"
	CA_ALARM_NO_ACCESS_TO_NETWORK,      //211 "No access - network block"
	CA_ALARM_ACCEPT_VIEW,               //212 start viewing dialog box, para2 = T_CaAcceptViewInfo
	CA_ALARM_ORDER_INFO,                //213 ordering dialog box, para2 = T_CaOrderInfo
	CA_ALARM_PPV_REQUEST,               //214 token access dialog box, para2 = T_CaPPVRequestInfo
	CA_ALARM_MATURITY_RATING,           //215 maturity lock dialog box,para2 is the program maturity level
    CA_ALARM_PPV_INSUFFICIENT_TOKENS,   //216 ppv insufficient tokens,  (conax5后应用不会收到此消息) 
    CA_ALARM_SRV_FREE,                  //217 free service,应用不需要处理
    CA_ALARM_CONAX_SCRAMBLED,           //218 conax scramble service
    CA_ALARM_CW_SET_SUCCESS,            //219 open av;
    CA_ALARM_CARD_READY,                //220 smart card ok, no use to display,discard from conax5.0A(conax5后应用不会收到此消息)   
    CA_ALARM_CANCEL_MAT,                //221 cancel maturity dlg,discard from conax5.0A(conax5后应用不会收到此消息)
    CA_ALARM_CANCEL_PPV,                //222 cancel ppv dlg,discard from conax5.0A(conax5后应用不会收到此消息)
    CA_ALARM_CANCEL_ORDER,              //223 cancel order dlg,discard from conax5.0A(conax5后应用不会收到此消息)
    CA_ALARM_CANCEL_ACCEPT_VIEW,        //224 cancel accept view dlg,discard from conax5.0A(conax5后应用不会收到此消息)
    CA_ALARM_CW_LOST,                   //225 close dwParm3: audio or video;conax5(包括conax5)后支持,之前版本不抛这个消息
    
    CA_ALARM_MAX
}T_CaAlarm;

typedef enum 
{
	CA_STATUS_CARD_READY = (CA_MSG_BASE+300),//300 card ok, but needn't display
	CA_STATUS_CARD_INSERT ,                  //301 " Card Checking… "
	CA_STATUS_CARD_REMOVE ,                  //302 "No card inserted"
	CA_STATUS_CARD_INCORRECT,                //303 "Incorrect card",if dwParm2 != 0, it means the time the banner will be displayed
	CA_STATUS_CARD_INCORRECT_CONAX,          //304 "Incorrect Conax card"
	CA_STATUS_CARD_COMMUNICATION_ERROR,      //305 "Card problem - check card"
	CA_STATUS_ORDER_CONNECTING,              //306 connecting, please wait...
	CA_STATUS_ORDER_BUSY,                    //307 sorry, please try again
	CA_STATUS_ORDER_FAILED,                  //308 order failed, Error code:'PI_STATUS'//para2=PI_STATUS, should be displayed in hexadecimal
	CA_STATUS_ORDER_CLOSE,                   //309 no use this command at the moment
	CA_STATUS_ORDER_ACCEPTED,                //310 order accepted
	CA_STATUS_MAX
}T_CaStatus;

typedef enum
{
    CA_EMAIL_NO_SEQ = 0,        //无顺序要求
    CA_EMAIL_TIME_ASC,          //创建时间早的邮件放在前面
    CA_EMAIL_TIME_DESC,         //创建时间早的邮件放在后面
    CA_EMAIL_UNREAD_ASC,        //未读邮件放在前面
    CA_EMAIL_UNREAD_DESC,       //未读邮件放在后面    
    CA_EMAIL_UNREAD_TIME_ASC,   //未读邮件且时间晚的邮件放在前面
    CA_EMAIL_UNREAD_TIME_DESC,  //未读邮件且时间晚的邮件放在后面,第0封是未读的新来邮件
    CA_EMAIL_MAIL_ID_ASC,       //mail id升序排列  
    CA_EMAIL_MAIL_ID_DESC,      //mail id降序排列
    CA_EMAIL_SHOW_TIME_ASC,
    CA_EMAIL_MAX
}T_CaEmailSeq;

typedef enum _MENU_STATUS_EN    //conax5版本之前不支持,conax5版本后(包括conax5版本)可以使用
{
	CA_MENU_MAINMENU_STATUS,    //DVB主菜单         
	CA_MENU_CAMENU_STATUS,      //CA菜单 
	CA_MENU_MAX
}CONAX_MENU_STATUS;

/**************************************************************************
* Function Name: CS_CA_GetCaLibVer
*   获取CA 模块版本号
*
*Parameters	
*		pcCaVerBuf
*             [in] 存放版本号的Buffer
*		dwBufSize
*		 [in] Buffer的大小( >= 16)
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetCaLibVer(BYTE *pcCaVerBuf, DWORD dwBufSize);

/**************************************************************************
* Function Name: CS_CA_GetConaxInfo
*   获取CA信息
*
*Parameters	
*		pstCaReleaseInfo
*                  [out] CA信息结构
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*note: example menu of "About Conax CA"
*    //////////////////////
*      About Conax CA
*Software version: XX.XX.XX ----from CS_CA_GetCaLibVer
*Interface version: 0x40
*Card number: 019 0000 100x - y
*Number of seeions: 3
*Language:44
*CA_SYS_ID: 0x0B00
*      ///////////////////////
**************************************************************************/
INT CS_CA_GetConaxInfo(T_CaConaxInfo *pstConaxInfo);

/**************************************************************************
* Function Name: CS_CA_GetSubscriptionTotalNum
*   获取购买记录总数
*
*Parameters
*
*Return Values
*		成功返回记录总数，失败返回FAILURE
**************************************************************************/
INT CS_CA_GetSubscriptionTotalNum(void);

/**************************************************************************
* Function Name: CS_CA_GetSubscriptionInfo
*   获取指定序号购买记录的内容
*
*Parameters	
*	       dwRecordIndex
*                   [in]   序号(从1开始)
*          pstSubRecord
*                   [out]  内容
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetSubscriptionInfo(DWORD dwRecordIndex, T_CaSubscriptionInfo *pstSubRecord);

/**************************************************************************
* Function Name: CS_CA_GetPpvEventTotalNum
*   获取PPV EVENT 记录总数
*
*Parameters	
*	
*Return Values
*		成功返回记录总数，失败返回FAILURE
**************************************************************************/
INT CS_CA_GetPpvEventTotalNum(void);

/**************************************************************************
* Function Name: CS_CA_GetPpvEventInfo
*   获取指定序号PPV EVENT  记录的内容
*
*Parameters	
*	       dwRecordIndex
*                    [in]  序号(从1开始)
*          pstPpvRecord
*                    [out] 内容
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetPpvEventInfo(DWORD dwRecordIndex, T_CaPPVEventInfo* pstPpvRecord);

/**************************************************************************
* Function Name: CS_CA_GetPurseTotalNum
*   获取钱包总数
*
*Parameters	
*	
*Return Values
*		成功返回总数，失败返回FAILURE
**************************************************************************/
INT CS_CA_GetPurseTotalNum(void);

/**************************************************************************
* Function Name: CS_CA_GetPurseInfo
*   获取指定序号钱包 状态
*
*Parameters	
*	       dwRecordIndex
*                     [in]  钱包序号(从1开始)
*          pstCaPurseInfo
*                     [out] 状态内容
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetPurseInfo(DWORD dwPurseIndex,T_CaPurseInfo* pstCaPurseInfo);

/**************************************************************************
* Function Name: CS_CA_GetDebitInfo
*   获取指定序号钱包 的Debit 状态
*
*Parameters	
*	       dwRecordIndex
*                     [in]  钱包序号(从1开始)
*          pstCaDebitInfo
*                     [out] 状态内容
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetDebitInfo(DWORD dwPurseIndex,T_CaDebitInfo** pstCaDebitInfo);

/**************************************************************************
* Function Name: CS_CA_GetCreditInfo
*   获取指定序号钱包 的Credit 状态
*
*Parameters	
*	       dwRecordIndex
*                      [in]  钱包序号(从1开始)
*          pstCaCreditInfo
*                      [out] 状态内容
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetCreditInfo(DWORD dwPurseIndex,T_CaCreditInfo** pstCaCreditInfo);

/**************************************************************************
* Function Name: CS_CA_GetCardMaturityRating
*   获取卡当前的父母级
*
*Parameters	
*           pMat [out] 父母级别
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_GetCardMaturityRating(T_CaMaturityRating *pMat);

/**************************************************************************
* Function Name: CS_CA_ChangCardMaturityRating
*   修改卡的父母级
*
*Parameters	
*	       eMatRat
*                 [in]  父母级
*          pSzPin
*                 [in] 卡密码
*Return Values
*		SUCCESS(0)	修改成功
*		1			密码错误
*		OTHER       	修改失败
**************************************************************************/
INT CS_CA_ChangeCardMaturityRating(T_CaMaturityRating eMatRat,BYTE *pSzPin);

/**************************************************************************
* Function Name: CSCAAcceptMaturityRat
*   输入密码确认收看父母级限制节目
*
*Parameters	
*          pSzPin
*                 [in] 卡密码
*           hService
*                 [in]节目handle
*Return Values
*		SUCCESS(0)	成功
*		1			密码错误
*		OTHER	        失败
**************************************************************************/
INT CSCAAcceptMaturityRat( BYTE *pSzPin , HCSHANDLE hService);
INT CS_CA_AcceptMaturityRat( BYTE *pSzPin );


/**************************************************************************
* Function Name: CSCACancelMaturityRat
*   取消父母级限制提示框，不收看当前节目
*
*Parameters   hService
*                 [in]节目handle
*Return Values
*		SUCCESS	成功
*		FAILURE		错误
**************************************************************************/
INT CSCACancelMaturityRat( HCSHANDLE hService );
INT CS_CA_CancelMaturityRat(void );

/**************************************************************************
* Function Name: CSCAAcceptStartView
*   输入密码确认收看扣费
*
*Parameters	
*          pSzPin
*                 [in] 卡密码
*           hService
*                 [in]节目handle
*Return Values
*		SUCCESS(0)	成功
*		1			密码错误
*		OTHER	        失败
**************************************************************************/
INT CSCAAcceptStartView( BYTE *pSzPin , HCSHANDLE hService );
INT CS_CA_AcceptStartView( BYTE *pSzPin );

/**************************************************************************
* Function Name: CSCACancelStartView
*   取消扣费节目观看提示框，不收看当前节目
*
*Parameters	hService
*                 [in]节目handle
*Return Values
*		SUCCESS	成功
*		FAILURE		错误
**************************************************************************/
INT CSCACancelStartView( HCSHANDLE hService );
INT CS_CA_CancelStartView(void);

/**************************************************************************
* Function Name: CSCAChangCardMaturityRating
*   输入密码确认订购当前节目
*
*Parameters	
*          pSzPin
*                 [in] 卡密码
*           hService
*                 [in]节目handle
*Return Values
*		SUCCESS(0)	成功
*		1			密码错误
*		OTHER	        失败
**************************************************************************/
INT CSCAAcceptOrderInfo( BYTE *pSzPin ,HCSHANDLE hService );
INT CS_CA_AcceptOrderInfo( BYTE *pSzPin  );

/**************************************************************************
* Function Name: CSCACancelOrderInfo
*   取消节目订购提示框，不收看当前节目
*
*Parameters	
*           hService
*                 [in]节目handle

*Return Values
*		SUCCESS	成功
*		FAILURE		错误
**************************************************************************/
INT CSCACancelOrderInfo( HCSHANDLE hService  );
INT CS_CA_CancelOrderInfo(void);

/**************************************************************************
* Function Name: CSCAAcceptPPVRequest
*   输入密码确认购买PPV节目
*
*Parameters	
*          pSzPin
*                 [in] 卡密码
*           hService
*                 [in]节目handle

*Return Values
*		SUCCESS(0)	成功
*		1			密码错误
*		OTHER       	失败
**************************************************************************/
INT CSCAAcceptPPVRequest( BYTE *pSzPin , HCSHANDLE hService);
INT CS_CA_AcceptPPVRequest( BYTE *pSzPin );

/**************************************************************************
* Function Name: CSCACancelPPVRequest
*   取消PPV购买提示框，不收看当前节目
*
*Parameters	
*           hService
*                 [in]节目handle
*Return Values
*		SUCCESS	成功
*		FAILURE		错误
**************************************************************************/
INT CSCACancelPPVRequest( HCSHANDLE hService);
INT CS_CA_CancelPPVRequest(void);

/**************************************************************************
* Function Name: CS_CA_ChangeCardPin
*   修改卡密码
*
*Parameters	
*	       pSzPinOld
*                  [in]  旧密码
*          pSzPinNew
*                  [in]  新密码
*Return Values
*		SUCCESS(0)	修改成功
*		1			旧密码错误 "Incorrect old PIN"
*		OTHER       	修改失败"Change CA PIN failed"
*note: example dialogue:
*    //////////////////////////////////////////
*    Change PIN Please enter CA PIN   ****
*    Please enter new CA PIN  ****
*    Please confirm new CA PIN  ****
*    /////////////////////////////////////////
*if the user has entered two new PINs that do not match,"PIN confirmation does not match" should be displayed
**************************************************************************/
INT CS_CA_ChangeCardPin(BYTE* pSzPinOld, BYTE *pSzPinNew);

/**************************************************************************
* Function Name: CS_CA_Standby
*   通知CA 当前为待机 状态
*
*Parameters	
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_Standby(void);

/**************************************************************************
* Function Name: CS_CA_Resume
*   通知CA 从待机状态唤醒
*Parameters	
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_Resume(void);

/**************************************************************************
* Function Name: CS_CA_CheckCard
*   获取卡当前状态
*Parameters	
*Return Values
*		卡当前状态
**************************************************************************/
T_CaStatus CS_CA_GetCardStatus(void);

/**************************************************************************
* Function Name: CS_CA_EmailDel
*  删除一封邮件
*
*Parameters	
*	      dwIndex
*              [in]  序号(从0开始)
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*注: conax的邮件删除是通过前端来进行的。
本函数仅仅提供给某些运营商有特殊需求的项目。
认证项目，界面上不提供该处理
**************************************************************************/
INT CS_CA_EmailDel(DWORD dwIndex/* start with 0*/);

/**************************************************************************
* Function Name: CS_CA_EmailDelByID
*  删除一封邮件
*
*Parameters	
*	      ucMailId
*              [in]  邮件Id
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*注: 本函数通过邮件Id删除一封邮件，目前仅有kccl项目使用
**************************************************************************/
INT CS_CA_EmailDelByID(BYTE ucMailId);

/**************************************************************************
* Function Name: CS_CA_EmailDelAll
*  删除所有邮件
*
*Parameters	
*	       
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*注: conax的邮件删除是通过前端来进行的。
本函数仅仅提供给某些运营商有特殊需求的项目。
认证项目，界面上不提供该处理
**************************************************************************/
INT CS_CA_EmailDelAll(void);

/**************************************************************************
* Function Name: CS_CA_EmailGetNum
*  获取当前邮件总数
*
*Parameters	
*	       
*Return Values
*		返回总数
**************************************************************************/
INT CS_CA_EmailGetNum(void);

/**************************************************************************
* Function Name: CSCAEmailGetNum
*  获取当前邮件总数,以及未读邮件总数
*
*Parameters 
*          pnTotalNum :邮件总数
*          pnUnreadNum:未读邮件总数
*Return Values
*       SUCCESS 成功
*       FAILURE     失败

**************************************************************************/
INT CSCAEmailGetNum(  INT* pnTotalNum    , INT* pnUnreadNum);

/**************************************************************************
* Function Name: CS_CA_EmailGetInfo
*  根据序号获取某封邮件
*
*Parameters	
*	    dwIndex
*             [in] 序号(从0开始)
*		pstMsg
*             [out] 获得的短消息
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_EmailGetInfo(DWORD dwIndex/* start with 0*/, T_CaEmail *pstMail);

/**************************************************************************
* Function Name: CS_CA_EmailSetReadflag
*  将指定序号邮件设置为已读
*
*Parameters	
*	    dwIndex
*             [in] 序号(从0开始)
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_EmailSetReadflag(DWORD dwIndex/* start with 0*/);

/**************************************************************************
* Function Name: CS_CA_EmailSaveFlash
*  保存所有对邮件的修改
*
*Parameters	空
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
INT CS_CA_EmailSaveFlash(void);
/**************************************************************************
* Function Name: CS_CA_EmailSetSequence
*  设置email排列顺序，默认为无顺序
*
*Parameters 空
*Return Values
*       SUCCESS 成功
*       FAILURE     失败
**************************************************************************/
INT CS_CA_EmailSetSequence(T_CaEmailSeq emSeq);

/**************************************************************************
* Function Name: CSCAGetShortMsg
*  获取系统收到的最新的短消息
*
*Parameters	
*		dwMsgId
*			[in] 短消息ID
*		pstMessage
*			[out] 短消息内容
*           hService
*                 [in]节目handle
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*下面两个函数从conax5.0版本后(包括5.0版本)已经废弃,保留只是为了兼容老版本
**************************************************************************/
INT CSCAGetShortMsg(HCSHANDLE hService, DWORD dwMsgId, T_CaMessage *pstMessage);
INT CS_CA_GetShortMsg(DWORD dwMsgId, T_CaMessage *pstMessage);


/**************************************************************************
* Function Name: CSCAGetFingerPrint
*  获取系统收到的最新的指纹
*
*Parameters	
*		pstFingerPrint
*			[out] 指纹内容
*        hService
*             [in]节目handle
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*下面两个函数从conax5.0版本后(包括5.0版本)已经废弃,保留只是为了兼容老版本
**************************************************************************/
INT CSCAGetFingerPrint(HCSHANDLE hService, T_CaFingerprint* pstFingerPrint);
INT CS_CA_GetFingerPrint(T_CaFingerprint* pstFingerPrint);


/**************************************************************************
* Function Name: CSGetCaFlashAddress
*  CA 获取Flash空间地址,1个Block. 由应用实现该函数
*
*Parameters	空
*Return Values
*		CA FLASH 空间首地址
**************************************************************************/
//extern DWORD CSGetCaFlashAddress(void);
extern DWORD CS_CA_GetFlashAddress(void);


/**************************************************************************
* Function Name: CS_CA_GetChipId
*  获取机顶盒主芯片ID. ChipSet Pairing模式时使用
*
*Parameters	
*Return Values
*		成功则返回ID
*		失败返回0		
**************************************************************************/
DWORD CS_CA_GetChipId(void);

/**************************************************************************
* Function Name: CSCaGetCurrentTime
* CA 获取当前系统时间(UTC)
*
*Parameters	
*		stDateTime
*		[out] 日期和时间
*Return Values
*		SUCCESS	成功
*		FAILURE		失败
*注意:当应用没有获取到TDT表之前,请返回FAILURE
**************************************************************************/
extern INT CSCaGetCurrentTime(T_CaDateTime *stDateTime);

/**************************************************************************
* Function Name: CSGetCaCPMFlashAddress
*  CA 获取用于ChipSet Pairing key存储FLASH空间地址，大小1block.  由应用实现该函数。
* 此区域要相对比较安全。ChipSet Pairing 模式时使用，若无此功能，则返回0
*
*Parameters	空
*Return Values
*		CA FLASH 空间首地址
**************************************************************************/
extern DWORD CSGetCaCPMFlashAddress(void);

/**************************************************************************
* Function Name: CSGetCaOTPFlashAddress
*  CA获取存放 Verifier关键数据Flash地址,该区域需OTP，最少1K.
* 此区域要相对比较安全。verifier模式时使用，若无此功能，则返回0
*
*Parameters 空
*Return Values
*       CA FLASH 空间首地址
**************************************************************************/
extern DWORD CSGetCaOTPFlashAddress(void);

/**************************************************************************
* Function Name: CSGetCaVerifierFlashAddress
*  CA获取存放Verifier Key的Flash空间地址，1个Block.
*  verifier模式时使用，若无此功能，则返回0
*
*Parameters 空
*Return Values
*       Verifier Key的Flash空间地址
**************************************************************************/
extern DWORD CSGetCaVerifierFlashAddress(void);

/**************************************************************************
* Function Name: CSGetCaVerifierBackUpFlashAddress
* CA获取备份Verifier Key的Flash空间地址，1个Block.
*  verifier模式时使用，若无此功能，则返回0
*
*Parameters 空
*Return Values
*       Verifier Key的Flash空间地址
**************************************************************************/
extern DWORD CSGetCaVerifierBackUpFlashAddress(void);

/**************************************************************************
* Function Name: CSReturnPathConnectServer
* 连接到服务器，用于回传，若无此功能，则返回0
*
*Parameters 
*    pcIpAddress: IP地址
*    dwPort: 端口号
*    dwTimeout：超时时间
*Return Values
* *		SUCCESS	成功
*		FAILURE		失败      
**************************************************************************/
extern INT CSReturnPathConnectServer(char *pcIpAddress, DWORD dwPort, DWORD dwTimeout);

/**************************************************************************
* Function Name: CSReturnPathSend
* 发送数据，用于回传，若无此功能，则返回0
*Parameters 
*   pcData: 待发送的数据首地址
*   nLen: 数据长度
*   dwTimeout：超时时间
*Return Values
*      *		SUCCESS	成功
*		FAILURE		失败 
**************************************************************************/
extern INT CSReturnPathSend(char *pcData, INT nLen, DWORD dwTimeout);

/**************************************************************************
* Function Name: CSReturnPathRecv
* 接收数据，用于回传，若无此功能，则返回0
*  
*Parameters 
*       pcBuf: 接收数据首地址
*       cBufLen: 接收数据长度
*       dwTimeout :超时时间
*Return Values
*    	SUCCESS	成功
*		FAILURE		失败
**************************************************************************/
extern INT CSReturnPathRecv(char *pcBuf, INT cBufLen, DWORD dwTimeout);

/**************************************************************************
* Function Name: CSReturnPathClose

*  关闭连接，用于回传，若无此功能，则返回0
*
*Parameters 空
*Return Values
*		SUCCESS	成功
*		FAILURE		失败      
**************************************************************************/
extern INT CSReturnPathClose(void);

/**************************************************************************
* Function Name: CSCACheckandReportShortMsg
*  开机或退出待机播放节目，分析完EPG时间后检测是否有需要显示的短消息和指纹
*
*Parameters 空
*Return Values
*         
**************************************************************************/
void CSCACheckandReportShortMsg(void);

/**************************************************************************
* Function Name: CSCACancelCardProblem
* 用户主动按键,取消卡错误提示框
CA_ALARM_CARD_INCORRECT,
CA_ALARM_CARD_INCORRECT_CONAX,
CA_ALARM_CARD_COMMUNICATION_ERROR
*
*Parameters
*   dwParam:  单路节目请传入0
*             多路节目传入CSCAStartService()返回的handle
*Return Values
*注意: conax5版本之前不支持,conax5版本后(包括5版本)可以使用
**************************************************************************/
void CSCACancelCardBanner(DWORD dwParam);

/**************************************************************************
* Function Name: CSCACancelShortMsg
* 用户主动按键,取消短消息提示框
*
*Parameters
*   dwParam:  单路节目请传入0
*             多路节目传入CSCAStartService()返回的handle
*Return Values
*注意: conax5版本之前不支持,conax5版本后(包括5版本)可以使用
**************************************************************************/
void CSCACancelShortMsg(DWORD dwParam);

/**************************************************************************
*Function Name: CS_CA_EnterMenu
*  进入CA界面数据
*
*Parameters 
*	eMenuType:菜单类型
*   dwParam:  单路节目请传入0
*             多路节目传入CSCAStartService()返回的handle
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
*注意: conax5版本之前不支持,conax5版本后(包括5.0版本)可以使用
**************************************************************************/
INT CS_CA_EnterMenu( CONAX_MENU_STATUS eMenuType, DWORD dwParam);


/**************************************************************************
*Function Name: CS_CA_ExitMenu
*  退出CA界面数据
*
*Parameters 
*	eMenuType:菜单类型
*   dwParam:  单路节目请传入0
*             多路节目传入CSCAStartService()返回的handle
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
*注意: conax5版本之前不支持,conax5版本后(包括conax5版本)可以使用
**************************************************************************/
INT CS_CA_ExitMenu( CONAX_MENU_STATUS eMenuType, DWORD dwParam);

#ifdef  __cplusplus
}
#endif

#endif

