/*************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_NSCA_Interface.h  
* Author:      Wan suanlin
* Date:        
* Description: NDS CA interface
*            
**************************************************************************************************************/
#include "udiplus_typedef.h"

#ifndef _CS_NDSCA_INTERFACE_H
#define _CS_NDSCA_INTERFACE_H

#ifdef  __cplusplus
extern "C" {
#endif

#define CS_NDSCA_MESSAGE_BASE    (0)

typedef enum _CaEventGroup
{
	CA_ALARM = CS_NDSCA_MESSAGE_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	
	CA_EVENTGROUP_MAX
}E_CaEventGroup;

typedef enum _CaInfo
{
	CA_INFO_START = (CS_NDSCA_MESSAGE_BASE + CA_EVENTGROUP_MAX + 1),//6
	
	CA_INFO_TXT,          //dwParm2 = (DWORD)&T_CaMail(紧急邮件)
	CA_INFO_TXT_CHANGE,   //dwParm2 = (DWORD)&T_MailNum
	CA_INFO_VIEWCARDCFG,  //dwParm2 = (DWORD)&T_CaViewCardCfg
	CA_INFO_SICFG,        //dwParm2 = (DWORD)&T_SICfg(service information config)
	CA_INFO_CADIAG_DATA,  //dwParm2 = (DWORD)&T_CaDiagdata
	CA_INFO_OSD,          //dwParm2 = (DWORD)&T_CaExceptionMsg:barker show 12
	CA_INFO_SCREGIONID,   //dwParm2 = (DWORD)&T_SCRegionInfor
	CA_INFO_CASTBLABEL,   //dwParm2 = (DWORD)&T_CaStbLabelInfo
	
	CA_INFO_MAX	
}E_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_START = (CS_NDSCA_MESSAGE_BASE + CA_INFO_MAX + 1),//16
	
	CA_COMMAND_FORCE_MSG_HIDE,
	CA_COMMAND_FORCE_MSG_SHOW, //dwParm2 = (DWORD)&T_CaFpInfo
	CA_COMMAND_SMC_STATUS,     //dwParm2 = (DWORD)&T_SmcStatus
	CA_COMMAND_BOUQETID_CHANGE,/*dwParm2 is original data,you should get bouquet_id by 
	"bouquet_id = (dwParm2 & 0xFF) + 0x6000". Notice the calculation of bouquet_id is 
	according to the project*/

	//操作音视频，操作Decoder层
	CA_COMMAND_AV_TURN_ON,        //Open AV: dwParm2  = &T_OperateAVInfo 21
	CA_COMMAND_AV_TURN_OFF,       //Close AV:dwparm2 = &T_OperateAVInfo  22
	
	CA_COMMAND_MAX
}E_CaCommand;

typedef enum _CaStatus
{
	CA_STATUS_START = (CS_NDSCA_MESSAGE_BASE + CA_COMMAND_MAX + 1),//24

	CA_STATUS_MODIFY_PIN_SUCCESS,             /*Modify Pin Code Success*/
	CA_STATUS_MODIFY_PR_LEVEL_SUCCESS,        /*Modify Parent Rating Success*/	
	CA_STATUS_MODIFY_STANDBY_STATUS_SUCCESS,  /*Set STB Standby Status Success*/
	CA_STATUS_GET_CUR_LANGUAGE_SUCCESS,       /*Get Current Language Success, dwParm2 = E_CaLanguage*/
	CA_STATUS_MODIFY_CUR_LANGUAGE_SUCCESS,    /*Modify Current Language Success*/

	CA_STATUS_GET_PR_LEVEL_SUCCESS,           /*Get Current Card Limit Value, dwPara2 = Limit Valut(3-18)*/
	CA_STATUS_GET_PIN_STATUS_SUCCESS,         /*Get Pin Status Success, dwParm2 = (DWORD)&T_CaPinLockStaus*/
	CA_STATUS_MODIFY_PIN_STATUS_SUCCESS,      /*Modify Pin Status Success*/

	CA_STATUS_SMC_ERROR,                      /*Smart card error 33*/
	CA_STATUS_INVALID_USERID,                 /*Invalid user id*/
	CA_STATUS_INVALID_PIN,                    /*Invalid pin*/
	CA_STATUS_INVALID_LIMITID,                /*Invalid limit value*/
	CA_STATUS_ACTION_FAILED,                  /*Action failed*/
	CA_STATUS_WRONG_LANGUAGE,                 /*Wrong language*/

	CA_STATUS_MODIFY_STANDBY_STATUS_FAILURE,  /*Set STB Standby Status failure*/
	
	CA_STATUS_MAX
}E_CaStatus;

typedef enum _CaAlarm
{
	CA_ALARM_START = (CS_NDSCA_MESSAGE_BASE + CA_STATUS_MAX + 1),//41

	CA_ALARM_FREE_PROGRAME,       
	CA_ALARM_WITH_NDS_CA,         
	CA_ALARM_OTHER_CA_ENCRYPT,    /*E128 Other CA Service(E128 其他加扰节目)*/
	CA_ALARM_SMC_OUT,             /*E01 Please insert Your Card(E01 加密节目，请插卡)*/
	CA_ALARM_SMC_UNKNOW,          /*E50 Unknow Card(E50 未法识别的智能卡)--预留 45*/
	
	CA_ALARM_SMC_CHECKING,        /*47(预留)*/
	CA_ALARM_SMC_IN,              
	CA_ALARM_SMC_MUTE,            /*E02 Card is invalid. Please call Customer service (×××××)(E02无效的智能卡)*/
	CA_ALARM_SMC_TRANSTION_ERROR, /*E51 Card Transtion is Error(E51 智能卡通讯错误)--预留 50*/
	CA_ALARM_SMC_OK,              
	
	CA_ALARM_SMC_ERROR,           /*E52 Card is Error(E52 智能卡错误, 预留)*/
	CA_ALARM_SMC_INVALID,         /*E25 Inserted card is not a TV Viewing card(E25 智能卡无效)*/
	CA_ALARM_SMC_MATCH,           /*预留*/
	CA_ALARM_STB_SMC_NOT_MATCH,   /*E14 This is the wrong viewing card for this CableCard(E14 机卡不配对)*/
	CA_ALARM_SMC_CHANGED,         /*E53 Card has been Changed(E53 观看的智能卡发生改变)--预留 55*/

	CA_ALARM_SMC_CLOSED,          /*E11 Please call Customer service (×××××) (Security problem)(E11 安全问题，关闭卡)*/
	CA_ALARM_SMC_NEW_TRANSITION,  /* New smart card in preparation stage */
	CA_ALARM_SMC_NOT_AUTHORIZED,  /*E06 This viewing card is not authorized. Please call Customer service (×××××)(E06 卡未授权)*/
	CA_ALARM_CW_INVALID,          /*E04 This Service is not authorized(E04 节目未授权)*/
	CA_ALARM_CW_VALID,            //61 valid cw key*/
	
	CA_ALARM_SRV_INFO_ERROR,      /*E05 Invalid Viewing CA Packet(E05 节目信息错误，请稍等)*/	
	CA_ALARM_AREA_RESTRICT,       /*E09 The Service is Blacked Out(E09 节目区域限制)*/	
	CA_ALARM_PR_LEVEL_TOO_LOW,    /*E54 Parent's Level is too Low(E54 观看级别太低)--预留*/
	CA_ALARM_CLEAN_MSG,           

	CA_ALARM_MAX
	
}E_CaAlarm;

typedef enum _CaOsdType
{
	CA_OSD_CLEAN_SCREEN,        /*(Clean screen)*/
	CA_OSD_SMC_OUT,             /*Please insert card*/
	CA_OSD_SMC_MUTE,            /*Card communication Failure*/
	CA_OSD_SERVICE_NOT_AUTHORIZED,  /*This service is not authorized*/
	CA_OSD_INVALID_PACKET,      /*Invalid packet*/

	CA_OSD_SMC_NOT_AUTHORIZED,  /*This card is not authorized*/
	CA_OSD_SERVICE_BLACKED_OUT, /*This service is blackout*/
	CA_OSD_SECURITY_PROBLEM,    /*Please call(security problem)*/
	CA_OSD_NO_EMPTY_IPPV_SLOTS, /*No empty IPPV slots*/
	CA_OSD_SMC_LACK,            /*Insufficient credit in card*/

	CA_OSD_STB_SMC_NOT_MATCH,   /*This is wrong card for this STB*/
	CA_OSD_PURCHASE_BLOCK,      /*Purchase not permitted without callback ability*/
	CA_OSD_USE_NEW_CARD,        /*Please Use new card(old card already chained)*/
	CA_OSD_END_PURCHASE_WINDOW, /*End of purchase window*/
	CA_OSD_BAD_IPPV_PACKET,     /*Please call(bad IPPV packet, purchase failed)*/
	
	CA_OSD_SMC_NOT_TVCARD,      /*Insert card is not TV viewing card*/
	CA_OSD_SMC_CHECKING,        /*Checking card, please wait*/
	CA_OSD_VOD_NOT_VIEWED,      /*This VOD service isn't addressed to your viewing card*/

	CA_OSD_MSG_TXT,             /*Message Text*/
	
	CA_OSD_TYPE_MAX
}E_CaOsdType;

typedef enum _CaLanguage
{
	CA_LAN_ENG,      //English
	CA_LAN_SPA,      //Spanish
	CA_LAN_POR,      //Portuguese
	CA_LAN_ARA,      //Arabic
	CA_LAN_BEN,      //Bengali

	CA_LAN_BUR,      //Burmese
	CA_LAN_GRC2,     //Cantonese Chinese
	CA_LAN_HEB,      //Hebrew
	CA_LAN_HIN,      //Hindi
	CA_LAN_IND,      //Indonesian (=Bhasa3)

	CA_LAN_JPN,      //Japanese
	CA_LAN_KOR,      //Korean
	CA_LAN_MAY,      //Malay(=Bhasa)
	CA_LAN_TGL,      //Tagalog
	CA_LAN_TAM,      //Tamil

	CA_LAN_THA,      //Thai
	CA_LAN_URD,      //Urdu
	CA_LAN_CHI,      //Chinese complicated
	CA_LAN_MIS2,     //Chinese simplified
	CA_LAN_GOH2,     //Mandarin Chinese

	CA_LAN_FRO2,     //Mandarin Taiwan
	CA_LAN_INVALID = 0xFF,
	
	CS_CA_LAN_MAX
}E_CaLanguage;

typedef enum _AVStreamType
{
	E_AVTYPE_AUDIO,
	E_AVTYPE_VIDEO,

	E_AVTYPE_MAX
}E_AVStreamType;

/* Smardcard Region data*/
#define CA_MAX_REGION_DATA_LEN  (64)

typedef enum _SCRegionStatus
{
	E_SCREGION_OK,
	E_SCREGION_CARD_ERROR,
	E_SCREGION_ACTION_FAILED,
	E_SCREGION_INVALID_PARAM,
	/*E_SCREGION_INSUFFICIENT_BUFFER,*/

	E_SCREGION_MAX
	
}E_SCRegionStatus;

typedef struct _SCRegionInfor
{
	E_SCRegionStatus  m_eStatus;
	BYTE                     m_ucSmcRegionData[CA_MAX_REGION_DATA_LEN];
}T_SCRegionInfor;

typedef struct _OperateAVInfo
{
	HCSHANDLE          m_hService;
	E_AVStreamType   m_eAVType;
}T_OperateAVInfo;

typedef enum _OpenAVtype
{
	E_CW_FLAG,
	E_OSD_FLAG,
	E_SMC_FLAG,
	E_NDSCA_FLAG,
	E_REGION_FLAG,

	E_AVSTATUS_MAX
}E_OpenAVtype;

typedef struct _StreamInfo
{
	E_AVStreamType m_eStreamType;
}T_StreamInfo;

typedef struct _CaDateTime
{
	DWORD   m_dwYear;
	BYTE    m_cMonth;
	BYTE    m_cDay;
	BYTE    m_cWeekDay;
	BYTE    m_cHour;
	BYTE    m_cMinute;
	BYTE    m_cSecond;
	BYTE    m_cRec[2];
}T_CaDateTime;

//MAIL
#define CA_MAIL_TITLE_LEN         (60)
#define CA_MAIL_SENDER_LEN        (60)
#define MAX_MAIL_TXT_LEN          (4096)

typedef struct _MailNum
{
	DWORD   m_dwTotalNum;
	DWORD   m_dwUnReadNum;
}T_MailNum;

typedef struct _Msg
{
	WORD    m_wEmailId;        //邮件标识符
	BOOL    m_bPriporityHigh;  //FALSE: Nomal Mail  TRUE: Urgency Mail
	BOOL    m_bDelete;         //TRUE:  User can not delete
	DWORD   m_dwSenderLen;                //发送者姓名长度
	BYTE    m_acSenderName[CA_MAIL_SENDER_LEN];//发送者姓名

	BYTE    m_cTitleFormatCode;
	BYTE    m_cRec1[3];
	DWORD   m_dwTitleLen;                 //标题长度
	BYTE    m_acTitle[CA_MAIL_TITLE_LEN]; //标题

	BYTE    m_cLanguage;                  //0x11-Unicode编码, 0x13-Chinese
	BYTE    m_cCompress;                  //目前不支持

	BYTE    m_cContFormatCode;            //0x11 Unicode编码 , 0x13-GB2312编码
	BYTE    m_cRec2[1];
	DWORD   m_dwContentLen;               //邮件内容长度
	BYTE    *pcContext;                   //邮件内容
	
	T_CaDateTime    m_stSendTime;         //邮件发送时间
	T_CaDateTime    m_stDeleteTime;       //邮件过期时间
}T_Msg;

typedef struct _CaMail
{	
	BOOL    m_bReadFlag;            //已读标志
	
	T_CaDateTime    m_stArriveTime; //邮件到达的时间
	T_Msg           m_stMsg;
}T_CaMail;


//OSD
#define CA_MAX_OSD_LEN          (512)

typedef enum _CaOsdOp
{
	CA_OSD_SHOW_GENERAL,   /*User cant not remove the message*/
	CA_OSD_SHOW_INFO,      /*User can remove the message*/
	CA_OSD_IDLE_EXCEPTION  /*Remove Pre-show message*/
}E_CaOsdOp;

typedef struct _CaOsdTxt
{
	DWORD   m_dwLen;
	BYTE    m_acText[CA_MAX_OSD_LEN];
}T_CaOsdTxt;

//Exception Descriptor
typedef struct _CaExceptionMsg
{
	BYTE    m_cExceprionCode;  //E_CaOsdType
	BYTE    m_cLangue;         //E_CaLanguage
	WORD    m_wLangueCode;     //0x11:unicode 0x13:GB232
	DWORD   m_dwMsgHandle;     //when app cancel this message, call cancel function will use
	
	T_CaOsdTxt m_stOsdTxt;
}T_CaExceptionMsg;

//Exception Extra Data Descriptor
typedef struct _CaExtraMsg
{
	DWORD   m_dwDataSouce;
	BYTE    m_cExceptionSouce;//0:ECM OSD 1:EMM OSD 2:Verifier OSD
	BYTE    m_cPriority;
	BYTE    m_cRec[2];
}T_CaExtraMsg;

//SMART CARD
typedef enum _CaCardType
{
	CA_UNKNOWN_CARD,      //Unknown card type
	CA_VIEWING_CARD,      //Card designated for NDS CA
	CA_OLD_VIEWING_CARD,  //Old viewing Card(过期的)
	CA_INTERACTIVE_CARD,  //Card designated for interactive TV viewing
	CA_AUXILIARY_CARD,    //Card for other purpose

	CA_CARD_TYPE_MAX
}E_CaCardType;

typedef enum _CaCardStatus
{
	CA_CARD_REMOVE,     //The card is removed from STB
	CA_CARD_READY,      //The card is inserted and ready for operation
	CA_CARD_FAILURE,    //The card does not answer
	CA_CARD_INVALID,    //The inserted card is not a valid viewing card
	CA_CARD_CLOSED,     //Security problem, card failure on authentication
	
	CA_CARD_TRANSITION, //New card in preparation stage
	CA_CARD_INSERT,     //Inserted card is undergoing initialization

	CA_CARD_STATUS_MAX
}E_CaCardStatus;

//Card Status Descriptor:When smart card is changed, the verifier will send this status to app
typedef struct _SmcStatus
{
	BYTE    m_cSmcType;   //E_CaCardType
	BYTE    m_cSmcStatus; //E_CaCardStatus
	BYTE    m_cRec[2];
}T_SmcStatus;

//-->目前未使用
//SI Descriptor
typedef struct _SICfg
{
	WORD    m_wCaSystemId;
	WORD    m_wSiFiltering; //Maybe this is Bouquet ID
}T_SICfg;
//<--目前未使用

//Viewing Card config descriptor
typedef struct _CaViewCardCfg
{
	DWORD   m_dwCardID;       //智能卡ID
	DWORD   m_dwSubscriberID; //预订者标识ID
	BYTE    m_cCardPaired;    //0：机卡不匹配，1：机卡匹配
	BYTE    m_cCardIDCheck;   //智能卡校验位，显示时将其放在显示智能卡号的后面
	BYTE    m_cCardAuthorized;//0：智能卡未授权，1：智能卡已授权
	BYTE    m_cRec[1];
}T_CaViewCardCfg;

//Finger Print
typedef enum _CaOsdCol
{
	CA_COL_TRANSPARENT, //透明色
	CA_COL_WHITE,       //白色
	CA_COL_RED,         //红色
	CA_COL_BLUE,        //蓝色
	
	CA_COL_MAX
}E_CaOsdCol;

#define CA_MAX_FINGERPRINT_LEN    (20)

typedef struct _CaFpInfo
{
	BYTE    m_cTxtColIndex;    //E_CaOsdCol(文本字体颜色)
	BYTE    m_cRegionColIndex; //E_CaOsdCol(区域框颜色)
	WORD    m_wRegionHeight;   //文本高度
	WORD    m_wRegionWidth;    //文本宽度
	WORD    m_wRegionXOffset;  //文本偏移横坐标值(相对屏幕)
	
	WORD    m_wRegionYOffset;  //文本偏移纵坐标值(相对屏幕)
	WORD    m_wTxtXOffset;     //显示内容偏移横坐标值(相对文本)
	WORD    m_wTxtYOffset;     //显示内容偏移纵坐标值(相对文本)
	WORD    m_wDuration;       //Time Out, Hide msg; if the value is 0, Ca will send CA_COMMAND_FORCE_MSG_SHOW to Hide this msg

	BYTE    m_cLen;
	BYTE    m_acTxt[CA_MAX_FINGERPRINT_LEN];
}T_CaFpInfo;

//Lock Status
typedef struct _CaPinLockStatus
{
	BYTE    m_cSystemLocked;    //TRUE: Locked; FALSE: Unlocked(目前只考虑这一字段)
	BYTE    m_cSystemtmpLocked; //TRUE: Locked; FALSE: Unlocked
	BYTE    m_cMasterPinDefined;//TRUE: Master pin defined; FALSE: Master pin undefine
	BYTE    m_cRec[1];
}T_CaPinLockStaus;

#define CA_MAX_SMC_VERSION_LEN   (8)
//Ca DiagData Information
typedef struct _CaDiagdata
{
	DWORD   m_dwLen;
	BYTE    m_acDiagInfo[CA_MAX_OSD_LEN];
	BYTE    m_acSmcVer[CA_MAX_SMC_VERSION_LEN];
}T_CaDiagdata;

#define CA_MAX_CASTBLABEL_LEN    (20)
#define CA_MAX_CHIPID_LEN        (20)
typedef struct _CaStbLabelInfo
{
	DWORD    m_dwCastblabelLen;
	BYTE     m_aucCastblabel[CA_MAX_CASTBLABEL_LEN];
	DWORD    m_dwChipidLen;
	BYTE     m_aucChipid[CA_MAX_CHIPID_LEN];//该项为CA界面的STB ID项，其数值为芯片真正的chip id+0x80000000并且加上校验位
	
}T_CaStbLabelInfo;

/**************************************************************************
*Function Name: CS_CA_GetCoreVer
*  获取CA内核版本信息
*
*Parameters 
*	pcVer[out]:
*		用于存储CA内核版本信息缓存 
*   pnLen[in|out]:
*       输入初始值为64，返回真实值
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
*	0 : SUCCESS 册除某一封邮件成功
*	-1: FAILURE 索引号错误或删除邮件失败
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
*	0 : SUCCESS 删除所有邮件成功
*	-1: FAILURE 删除所有邮件失败
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
*	0 : SUCCESS 获取邮件内容成功
*	-1: FAILURE 索引号错误或参数错误，或获取邮件内容错误
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
*Function Name:CS_CA_MMI_ModifyPinCode
* 请求修改密码
*
*Parameters 
*	pstOldPin[in]:旧密码
*   pstNewPin[in]:新密码
*
*Return Values: 
*	0 : SUCCESS 参数合法，结果将通过发消息给UI
*		[消息类型：CA_STATUS_MODIFY_PIN_SUCCESS | CA_STATUS_SMC_ERROR | CA_STATUS_INVALID_USERID
*		           CA_STATUS_INVALID_PIN | CA_STATUS_INVALID_LIMITID | CA_STATUS_ACTION_FAILED]
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_ModifyPinCode(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestSmcInfo
* 请求智能卡信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 目前该函数只会返回成功，结果将通过发消息给UI
*		[消息类型：CA_INFO_VIEWCARDCFG, dwParm2 = (DWORD)&T_CaViewCardCfg]
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_RequestSmcInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestParentRating
* 获取观看级别(目前此功能在国内项目还未使用)
*
*Parameters 
*	dwPin[IN]:用户密码
*
*Return Values: 
*	0 : SUCCESS 目前该函数只会返回成功，获取级别信息或获取信息失败将通过发消息给UI
*		[消息类型：CA_STATUS_GET_PR_LEVEL_SUCCESS(dwParm2所带的参数为当前智能卡的级别) | CA_STATUS_SMC_ERROR |
*		           CA_STATUS_INVALID_USERID | CA_STATUS_INVALID_PIN | CA_STATUS_INVALID_LIMITID]
*	-1: FAILURE 备用，目前不会返回失败
**************************************************************************/
INT CS_CA_MMI_RequestParentRating(DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyParentRating
* 修改观看级别(目前此功能在国内项目还未使用)
*
*Parameters 
*	pstPin[in]:输入密码
*   cParentRatingValue[in]:要修改的级别
*
*Return Values: 
*	0 : SUCCESS 参数合法，其结果将通过发消息给UI
*		[消息类型：CA_STATUS_MODIFY_PR_LEVEL_SUCCESS | CA_STATUS_SMC_ERROR | CA_STATUS_INVALID_USERID
*		           CA_STATUS_INVALID_PIN | CA_STATUS_INVALID_LIMITID | CA_STATUS_ACTION_FAILED]
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_ModifyParentRating(DWORD dwPin, BYTE cParentRatingValue);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyStandbyStatus
* 设置机顶盒状态
*
*Parameters 
*	cStandbyStaus[in]:设置STB状态[0:唤醒待机，1:进入待机]
*
*Return Values: 
*	0 : SUCCESS 参数合法，其结果将通过发消息给UI
*		[消息类型：CA_STATUS_MODIFY_STANDBY_STATUS_SUCCESS]
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_ModifyStandbyStatus(BYTE cStandbyStaus);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCurLanguage
* 获取当前语言类型
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS (本函数默认成功)其结果将通过消息给UI
*		[消息类型：CA_STATUS_GET_CUR_LANGUAGE_SUCCESS(dwParm2所带的参数为当前语言类型) |
*		           CA_STATUS_WRONG_LANGUAGE]
*	-1: FAILURE (目前没有这种情况发生)
**************************************************************************/
INT CS_CA_MMI_RequestCurLanguage(void);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyCurLanguage
* 设置某一特殊语言
*
*Parameters 
*	eLan[in]:将要设置的语种
*
*Return Values: 
*	0 : SUCCESS 参数合法，其结果将通过发消息给UI
*	    [消息类型：CA_STATUS_MODIFY_CUR_LANGUAGE_SUCCESS | 
*	               CA_STATUS_WRONG_LANGUAGE]
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_ModifyCurLanguage(E_CaLanguage eLan);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestLockStatus
* 获取PIN码状态(目前此功能在国内项目还未使用)
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS (本函数默认成功)其结果将通过消息给UI
*	    [消息类型：CA_STATUS_GET_PIN_STATUS_SUCCESS |  
*	               CA_STATUS_SMC_ERROR]
*	-1: FAILURE (目前没有这种情况发生)
**************************************************************************/
INT CS_CA_MMI_RequestLockStatus();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyLockStatus
* 设置PIN码状态(目前此功能在国内项目还未使用)
*
*Parameters 
*	dwPin[in]: 
*
*Return Values: 
*	0 : SUCCESS 参数合法，其结果将通过消息给UI
*	    [消息类型：CA_STATUS_MODIFY_PIN_STATUS_SUCCESS | CA_STATUS_SMC_ERROR | 
*	               CA_STATUS_INVALID_PIN | CA_STATUS_ACTION_FAILED]
*	-1: FAILURE 参数不合法
**************************************************************************/
INT CS_CA_MMI_ModifyLockStatus(DWORD dwPin, BYTE cLockMode);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCaDiagData
* 获取CA诊断消息
*
*Parameters 
*	无 
*
*Return Values: 
*	0 : SUCCESS 参数合法，其结果将通过消息给UI
*	    [消息类型：CA_INFO_CADIAG_DATA, dwParm2 = (DOWRD)&T_CaDiagdata]
*	-1: FAILURE
**************************************************************************/
INT CS_CA_MMI_RequestCaDiagData(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCardId
* 获取卡号(NDS verify上报消息时解析出来的卡号，包括最后1位校验位)
*
*Parameters 
*	pdwCardId[out]:卡号(unsined int buffer存储)
*
*Return Values: 
*	0 : SUCCESS 成功获取到卡号
*   -1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_MMI_RequestCardId(DWORD* pdwCardId);

/**************************************************************************
*Function Name:CS_CA_GetCardIdFormateString
* 获取卡号(以字符串形式提供给应用使用)
*
*Parameters 
*		pacCardIdStr[in/out]:APP申请空间保存字符串形式的卡号，
*							该长度必须大于12 bytes*    
*            pnLen[in/out]:APP通知CA申请的空间的大小，
                            获取卡号后，该数据表明卡号数据的长度，若出现显示不全，可能是该数组开辟太小
*
*Return Values: 
*	0 : SUCCESS 成功获取到卡号
*   -1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_GetCardIdFormateString(CHAR* pacCardIdStr, INT* pnLen);

/**************************************************************************
*Function Name:CS_CA_GetChipId
* 获取芯片Chip Id
*
*Parameters 
*		pacChipId[in/out]:APP申请空间保存字符串形式的芯片id号，
*							该长度必须大于等于8 bytes，前4个bytes为Chip Id
*            pnLen[in/out]:APP通知CA申请的空间的大小
*
*Return Values: 
*	0 : SUCCESS 成功获取到芯片Id号
*   -1: FAILURE 参数错误
**************************************************************************/
INT CS_CA_GetChipId(BYTE* pacChipId, INT* pnLen);

/**************************************************************************
*Function Name:CS_CA_GetClearMode
* 获取clear mode 位
*
*Parameters 
*
*Return Values: 
*	clear mode 位
**************************************************************************/
INT CS_CA_GetClearMode(void);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestBouquetId
* 获取用户类型
*
*Parameters 
*	pdwBoqueId[in/out]:(unsigned int buffer,用于存储BouquetId值)
*
*Return Values: 
*	0 : SUCCESS 获取BouquetId成功
*	-1: FAILURE 获取BouquetId失败
**************************************************************************/
INT CS_CA_MMI_RequestBouquetId(DWORD* pdwBouquetId);

/**************************************************************************
*Function Name:CS_CA_SetCustomerId
* 设置运营商ID(NDS分配的，各个运营商的CustomerId不同)
* 请在调用CS_CA_Init()函数之前先调用本函数
*
*Parameters 
*	dwCustomerId[in]:当项目确定时，由NDS CA分配给运营商的ID
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_SetCustomerId(DWORD dwCustomerId);

/**************************************************************************
*Function Name:CS_CA_MMI_GetSmcStatus
* 获取智能卡当前状态
*
*Parameters 
*	pcSmcStatus[In/Out]:存储智能卡状态，如果返回值为0，则卡OUT，为2则卡ok，其他情况卡error
*
*Return Values: 
*	SUCCESS 获取智能卡当前状态成功 
*	FAILURE 输入参数错误
**************************************************************************/
INT CS_CA_MMI_GetSmcStatus(BYTE* pcSmcStatus);

/**************************************************************************
*Function Name:CS_CA_MsgCancelOsd
* 通知CA取消OSD显示(用于天威项目,由于天威项目
* Verifier库没有主动发送清除OSD的消息，需要APP强制退出，并通知CA)
*
*Parameters 
*	无
*
*Return Values: 
*	SUCCESS 通知CA成功 
*	FAILURE 通知CA失败
**************************************************************************/
DWORD CS_CA_MsgCancelOsd(void);

/**************************************************************************
*Function Name:CSCAGetIrdSerialNum
* 获取CA Identify Num(对于支持高级安全的芯片其值
* 为Chip ID+0x80000000)
*
*Parameters 
*		pdwNum[in/out]:CA Identify Num
*            
*
*Return Values: 
*	0 : SUCCESS 成功获取到CA Identify Num
*   -1: FAILURE 参数错误
**************************************************************************/
INT CSCAGetIrdSerialNum(DWORD *pdwNum);

/**************************************************************************
*Function Name:CSCAGetDeviceId
* 获取NDS SVP Device Id
*
*Parameters 
*		pacDeviceId[in/out]:APP申请空间保存字符串形式的Device id号，
*							该长度必须大于8 bytes*    
*            pnLen[in/out]:APP通知CA申请的空间的大小
*
*Return Values: 
*	0 : SUCCESS 成功获取到Device Id号
*   -1: FAILURE 参数错误
**************************************************************************/
INT CSCAGetDeviceId(BYTE* pacDeviceId, INT* pnLen);

/**************************************************************************
*Function Name:CSCAGetCaStbLabel
* 获取CA STB LABEL，要显示在诊断信息界面上
*
*Parameters 
*		pstCaStbLabelInfo[in/out]:存储CA STB LABEL和CHIP ID    
*
*Return Values: 
*	0 : SUCCESS 获取信息成功
*   -1: FAILURE 参数错误
**************************************************************************/
INT CSCAGetCaStbLabel(T_CaStbLabelInfo* pstCaStbLabelInfo);

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


#ifdef  __cplusplus
}
#endif

#endif //_CS_NDSCA_INTERFACE_H


