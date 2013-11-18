/*************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    cstopcainterface.h  
* Author:      Wan suanlin
* Date:        
* Description: top cas interface
*            
**************************************************************************************************************/
#include "udiplus_typedef.h"

#ifndef _CS_TOPCA_INTERFACE_H
#define _CS_TOPCA_INTERFACE_H

#ifdef  __cplusplus
extern "C" {
#endif

#define CS_TOPCA_MESSAGE_BASE    (0)

typedef enum _CaEventGroup
{
	CA_ALARM = CS_TOPCA_MESSAGE_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	
	CA_EVENTGROUP_MAX
}E_CaEventGroup;

typedef enum
{
	CA_ALARM_START = (CS_TOPCA_MESSAGE_BASE + CA_EVENTGROUP_MAX + 1),
	CA_ALARM_CANCEL,             //清除ALAMR消息，打开音视频
	CA_ALARM_CLEAR_EVENT,        //清除界面事件消息，不用打开音视频
	
	//以下ALARM消息所携带的消息结构体为：CaEventInfo_S(即dw2 = &CaEventInfo_S)
	
	CA_ALARM_FLASH_REDEPLOYED,   //机顶盒存储结构重新分配置
	CA_ALARM_START_REGISTER,     //开始注册
	CA_ALARM_STOP_REGISTER,      //结束注册，Top Core发出的所有不超时的Alarm提示都会通过此消息来清除
	CA_ALARM_REGISTER_FAILED,    //注册失败
	CA_ALARM_REGISTER_SUCCESS,   //注册成功
	CA_ALARM_GRANT_ENTITLEMENT,  //获取授权(正在获取)
	CA_ALARM_ENTITLEMENT_SUCCESS,//获取授权成功，打开音视频
	CA_ALAMR_REVOKE_ENTITLEMENT, //取消授权
	CA_ALARM_NOENTITLE,          //没有授权
	CA_ALARM_PASSWORD_FAILED,    //验证密码失败
	CA_ALARM_INTERNAL_ERROR,     //内部错误	
	CA_ALARM_SWITCH_CHANNEL_GROUP, //切换频道群组
	CA_ALARM_UPDATE_CHANNEL_GROUP, //更新频道群组	
	CA_ALARM_OTHER_MESSAGE,      //其它讯息
	CA_ALARM_MAIL_SPACE_FULL,    //邮箱空间已满，提示信息显示在屏幕左上方
	CA_ALARM_NEWMAIL_NOTIFY,     //新邮件通知，提示信息显示在屏幕左上方
	CA_ALARM_URGENCYMAIL_NOTIFY, //紧急邮件通知，提示信息显示在屏幕左上方
	
	CA_ALARM_MAX
}E_CaAlarm;

typedef enum _CaInfo
{
	CA_INFO_START = (CS_TOPCA_MESSAGE_BASE + CA_ALARM_MAX + 1),//
	CA_INFO_OSD,                //目前没有使用到此消息
	CA_INFO_SHOW_DIALOG,        //要显示的对话框，dwParm2 = &UiDialogConfig
	CA_INFO_HIDE_DIALOG,        //消除对话框
	
	CA_INFO_MAX	
}E_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_START = (CS_TOPCA_MESSAGE_BASE + CA_INFO_MAX + 1),//
	CA_COMMAND_ACTIVE,   //dwParm2 = n second(表示n秒后恢复待机)
	CA_COMMAND_STANDBY,  //dwParm2 = n second(表示n秒后进入待机)
	CA_COMMAND_REBOOT,   //dwParm2 = n second(表示n秒后重启机顶盒)
	/*当启动app收到CA_COMMAND_START_CHANNELLIST消息后，对于切换频道时要比较，
	  如果要切换的频道serviceid不在CSChannelList_S结构对应的list内，则切台不成功*/
	CA_COMMAND_START_CHANNELLIST,  //dwParm2 = &CSChannelList_S
	CA_COMMAND_STOP_CHANNELLIST,  /*停止与channellist比较，可以切换到对应的频道*/
	CA_COMMAND_TUNER_CHANNELID,   /*切换到指定的channel id频道上*/
	CA_COMMAND_TUNER_UP,          /*向上切换台*/
	CA_COMMAND_TUNER_DOWN,        /*向下切换台*/
	CA_COMMAND_OSD_SHOW,          /*显示OSD，消息结构：OsdInfo_S*/
	CA_COMMAND_OSD_HID,           /*隐藏OSD*/
	CA_COMMAND_PORTAL,            /*通知APP重新排序小应用, 数据结构AppPortal_S*/
	CA_COMMAND_PCCTROL,           /*亲子锁控制，消息结构体：PctrlPasswordList_S*/
	CA_COMMAND_OTA,               /*通知APP将此信息保存，用于重启后锁频调制，消息结构体：SSUInfo_S*/
	CA_COMMAND_SSU_SHOW,          /*OTA升级显示信息，用于界面提示， 消息结构体：SSUDialog_S*/
	CA_COMMAND_RESET_PWD,         /*重置系统或工程密码，消息结构体：UiPwdReset_S*/
	
	CA_COMMAND_MAX
}E_CaCommand;

typedef enum _CaStatus
{
	CA_STATUS_START = (CS_TOPCA_MESSAGE_BASE + CA_COMMAND_MAX + 1),//

	CA_STATUS_MAX
}E_CaStatus;

#define MAX_SUBTITLE_LEN             (516)

typedef struct _OsdInfo
{
	DWORD  m_dwDuration;
	INT    m_nLen;
	CHAR   m_acText[MAX_SUBTITLE_LEN];
}OsdInfo_S;

typedef enum
{
	EM_SYSTEM_LANGUAGE_CHI = 1, //繁体中文
	EM_SYSTEM_LANGUAGE_ENG = 2  //英文
}EM_LanguageType;

typedef enum
{
	EM_SYSTEM_ROLE_ADMIN = 1,    //管理帐号
	EM_SYSTEM_ROLE_ENGINEER = 2  //工程帐号
}EM_SystemRole;

typedef enum
{
	EM_SYSTEM_POWER_MODE_UNKNOWN = 0,
	EM_SYSTEM_POWER_MODE_ACTIVE = 1,  //正常
	EM_SYSTEM_POWER_MODE_STANDBY = 2  //待机
}EM_SystemPowerMode;

typedef enum _Provider
{
	EM_PROVIDER_UNKNOWN = 0,
	EM_PROVIDER_COSHIP = 1,
	EM_PROVIDER_SKYWORTH = 2,
	EM_PROVIDER_YUXING = 3,
	EM_PROVIDER_DAHUA = 4,
	EM_PROVIDER_IPANEL = 21
}EM_Provider;

typedef enum _ChipType
{
	EM_CHIPSET_MODEL_UNKNOWN = 0,
	EM_CHIPSET_MODEL_BCM_7401 = 1,
	EM_CHIPSET_MODEL_BCM_7402 = 2,
	EM_CHIPSET_MODEL_ST_7101 = 11
}EM_ChipType;

#define CA_MAX_EVENT_TEXT_LEN           (128)

typedef enum
{
	EM_UI_EVENT_TYPE_NOTIFICATION = 0, //事件通知
	EM_UI_EVENT_TYPE_INFO = 1,         //资讯，在屏幕左上方显示
	EM_UI_EVENT_TYPE_WARNING = 2,      //警告消息，在屏幕中间显示
	EM_UI_EVENT_TYPE_ERROR = 3         //错误消息，在屏幕中间显示
}EM_UiEventType;

typedef enum
{
	EM_UI_EVENT_MESSAGE_FLASH_REDEPLOYED = 1,      //机顶盒存储结构重新配置
	EM_UI_EVENT_MESSAGE_START_REGISTER = 10,       //开始注册
	EM_UI_EVENT_MESSAGE_STOP_REGISTER = 11,        //结束注册
	EM_UI_EVENT_MESSAGE_REGISTER_FAILED = 12,      //注册失败
	EM_UI_EVENT_MESSAGE_REGISTER_SUCCESSFUL = 13,  //注册成功
	EM_UI_EVENT_MESSAGE_GRANT_ENTITLEMENT = 20,    //获取授权
	EM_UI_EVENT_MESSAGE_REVOKE_ENTITLEMENT = 21,   //取消授权
	EM_UI_EVENT_MESSAGE_NO_ENTITLEMENT = 22,       //未获得授权
	EM_UI_EVENT_MESSAGE_NEW_MAIL = 30,             //获取新的邮件
	EM_UI_EVENT_MESSAGE_HIGHT_GRADE_MAIL = 31,     //获取一封高等级邮件，显示讯息后，立即开启该邮件
	EM_UI_EVENT_MESSAGE_MAIL_SPACE_FULL = 32,      //邮件空间已满
	EM_UI_EVENT_MESSAGE_SWITCH_CHANNEL_GROUP = 40, //切换频道群组
	EM_UI_EVENT_MESSAGE_PASSWORD_FAILED = 41,      //验证密码失败
	EM_UI_EVENT_MESSAGE_INTERNAL_ERROR = 90,       //内部错误
	EM_UI_EVENT_MESSAGE_OTHER = 91                 //其它讯息
}EM_UiEventMessage;

typedef struct
{
	INT     m_nMessageType; //为EM_UiEventType类型
	INT     m_nMessage;     //为EM_UiEventMessage类型
	INT     m_nP1;  //参数P1 当p1=0:表示显示信息框永久保留，直到清除消息到来  当p1=n:表示显示信息框显示n秒钟后消失
	INT     m_nP2;  //当消息为CA_ALARM_URGENCYMAIL_NOTIFY时，此字段有效，m_nP2为EmailId，通过EmailId读取邮件内容，用于立即显示
	BYTE    m_aucText[CA_MAX_EVENT_TEXT_LEN];  //显示文字信息
}CaEventInfo_S;

#define MAX_IPADDR_LEN           (4)
#define MAX_MACADDR_LEN          (6)
#define MAX_MIDDLEWARE_VER_LEN   (8)

typedef struct
{
	DWORD   m_dwTotal;
	DWORD   m_dwUsed;
	DWORD   m_dwFree;
}TopcasSystemInfo_S;

typedef struct
{
	BYTE    m_ucVer1;
	BYTE    m_ucVer2;
	BYTE    m_ucVer3;
	BYTE    m_ucVer4;
}Version_S;

typedef struct _TopcasConfig
{
	EM_Provider   m_emMiddleWareProvider;
	EM_Provider   m_emStbProvider;
	EM_ChipType   m_emChipType;
	BYTE          m_aucMiddleWareVersion[MAX_MIDDLEWARE_VER_LEN];
	BYTE          m_aucIpAddr[MAX_IPADDR_LEN];
	BYTE          m_aucMacAddr[MAX_MACADDR_LEN];
	Version_S          m_stCurrentProductVer;
	TopcasSystemInfo_S m_stRamInfo;
	TopcasSystemInfo_S m_stFlashInfo;
}TopcasConfig_S;

#define MAX_CHANNEL_NUM         (480)
typedef struct _ServiceInfo  //频道(节目)信息
{
	WORD     m_wChannelId;
	WORD     m_wServiceId;
}CSServiceInfo_S;

typedef struct _ChannelList
{
	DWORD    m_dwChannelCount; //频道个数
	CSServiceInfo_S   m_astServiceInfo[MAX_CHANNEL_NUM];
}CSChannelList_S;


//Channel group
#define MAX_PASSWORD_LEN                     (4)
#define MAX_CHANNELGROUP_PASSWORD_COUNT      (8)
#define MAX_CHANNELGROPU_NAME_LEN            (32)
#define MAX_GRROUP_COUNT                     (16)
typedef enum
{
    EM_CHGROUP_TYPE_GLOBAL = 1,
    EM_CHGROUP_TYPE_GROUP  = 2,
    EM_CHGROUP_TYPE_SINGLE = 3
}EM_ChGroupType;

typedef struct
{
    BOOL    m_bEnabled;
    BYTE    m_aucPwd[MAX_PASSWORD_LEN];
}CHGroupPwd_S;

typedef struct
{
    INT     m_nCount;
    CHGroupPwd_S m_astPassword[MAX_CHANNELGROUP_PASSWORD_COUNT];
}CHGroupPwdList_S;

typedef struct
{
    BOOL    m_bIsDefault;
    BOOL    m_bIsActive;
    EM_ChGroupType  m_enType;
    DWORD   m_dwGroupId;
    BYTE    m_aucGroupName[MAX_CHANNELGROPU_NAME_LEN];
    BOOL    m_bPassword;
}CHGroupContent_S;

typedef struct
{
    INT     m_nCount;
    DWORD   m_dwGroupId[MAX_GRROUP_COUNT];
}GroupList_S;

#define MAX_APP_ITEM_NAME_LEN        (32)
#define MAX_APP_ITEM_PAGE_LEN        (32)
#define MAX_APP_CATEGORY_NAME_LEN    (32)
#define MAX_APP_ITEM_COUNT           (32)
#define MAX_APP_CATEGORY_COUNT       (16)

typedef enum
{
	EM_APP_TYPE_DEVICE_INFO = 1,            //设备资讯
	EM_APP_TYPE_COMMON_SETTING = 2,         //一般设定
	EM_APP_TYPE_ADVANCE_SETTING = 3,        //进阶设定
	EM_APP_TYPE_ENGINEERING_SETTING = 4,    //工程设定
	EM_APP_TYPE_EPG = 5,                    //节目指南，必须参考Bouquet_id
	EM_APP_TYPE_TV_SERVICE = 6,             //电视直播
	EM_APP_TYPE_RADIO_SERVICE = 7,          //广播电台
	EM_APP_TYPE_FAVORITE_CHANNEL = 8,       //喜爱频道
	EM_APP_TYPE_PROGRAM_BOOKING = 9,        //预约收视
	EM_APP_TYPE_CA_MAIL = 10,               //CA邮件
	EM_APP_TYPE_CHANNEL_GROUP = 11,         //频道表群组
	EM_APP_TYPE_WEB_INFO = 12,              //即时资讯
	EM_APP_TYPE_LOCAL_TIME_SHIFT = 13,      //时移电视
	EM_APP_TYPE_LOCAL_PVR = 14,             //节目录影
	EM_APP_TYPE_LOCAL_MP3_PLAYER = 15,      //MP3播放
	EM_APP_TYPE_LOCAL_PICTURE_BROWSER = 16, //照片浏览
	EM_APP_TYPE_LOCAL_MEDIA_PLAYER = 17,    //影音播放
	EM_APP_TYPE_LOCAL_PUSH_VOD = 18         //PUSH VOD
}AppType_E;

typedef struct
{
    AppType_E  m_eType;      //类型
    WORD       m_wItemId;    //编号
    WORD       m_wItemIcon;  //图示
    BYTE       m_aucItemNname[MAX_APP_ITEM_NAME_LEN]; //项目名称
    WORD       m_wBouquetId;
    BOOL       m_bCharged;  //是否为收费应用，如果为收费应用，必须标示收费图示
}AppItem_S;

typedef struct
{
	BYTE       m_cCategoryId;   //编号
	BYTE       m_cCategoryIcon; //图示
	BYTE       m_cCategoryName[MAX_APP_CATEGORY_NAME_LEN];
	BYTE       m_cItemCount;
	AppItem_S  m_astItem[MAX_APP_ITEM_COUNT];
}AppCategory_S;

typedef struct
{
	INT        m_cCategoryCount;
	AppCategory_S m_astCategory[MAX_APP_CATEGORY_COUNT];
}AppPortal_S;

#define MAX_PCTRL_PASSWORD_LEN       (4)
#define MAX_PCTRL_PASSWORD_COUNT     (16)

typedef struct
{
	CHAR    m_acPassword[MAX_PCTRL_PASSWORD_LEN];
}PctrlPassword_S;

typedef struct
{
	BOOL    m_bEnabled;
	BOOL    m_bLocked;
	BYTE    m_ucPasswordCount;
	PctrlPassword_S m_astPassword[MAX_PCTRL_PASSWORD_COUNT];
}PctrlPasswordList_S;

#define MAX_INFO_TEXT_LEN            (32)
typedef struct
{
	CHAR    m_acSystemId[MAX_INFO_TEXT_LEN];
	CHAR    m_acProvider[MAX_INFO_TEXT_LEN];
	CHAR    m_acVersion[MAX_INFO_TEXT_LEN];
	CHAR    m_acClientId[MAX_INFO_TEXT_LEN];
	CHAR    m_acFingerPrint[MAX_INFO_TEXT_LEN];
	CHAR    m_acEntitlement[MAX_INFO_TEXT_LEN];
	CHAR    m_acEmoney[MAX_INFO_TEXT_LEN];
}TextInfo_S;

typedef enum
{
	EM_MAIL_ADDRESS_GLOBAL = 1,  //发给所有机顶盒的邮件
	EM_MAIL_ADDRESS_GROUP = 2,   //发给群组机顶盒的邮件
	EM_MAIL_ADDRESS_SINGLE = 3   //发给指定机顶盒的邮件
}EM_MAILADDRESS;

typedef enum
{
	EM_MAIL_DEGREE_HIGH = 1,   //邮件列表中 级等一列显示为 高
	EM_MAIL_DEGREE_NORMAL = 2, //邮件列表中 级等一列显示为 中
	EM_MAIL_DEGREE_LOW = 3     //邮件列表中 级等一列显示为 低
}EM_MAILDEGREE;

#define MAX_MAIL_TITLE_LEN          (64)
#define MAX_MAIL_CONTENT_LEN        (604)
#define MAX_MAIL_COUNT              (30)

typedef struct  _CaDate
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
	DWORD   m_dwMs;
}CaDate_S;

typedef struct 
{
	DWORD   m_dwMailId;         //邮件标识号
	EM_MAILADDRESS m_emAddr;    //邮件寻址
	EM_MAILDEGREE  m_emDegree;  //邮件等级
	BOOL           m_bIsRead;   //已读标志
	BYTE    m_aucTitle[MAX_MAIL_TITLE_LEN];
	CaDate_S m_stDate;
}MailTitle_S;

typedef struct
{
	BYTE    m_aucContent[MAX_MAIL_CONTENT_LEN];
}MailContent_S;

typedef struct
{
	MailTitle_S    m_stMailTitel;
	MailContent_S  m_stMailContent;
}MailInteger_S;

typedef struct
{
	BYTE    m_ucTotalCount;   //邮件总个数
	BYTE    m_ucHadReadCount; //已读邮件个数
	BYTE    m_ucNoReadCount;  //未读邮件个数
	BYTE    m_ucFreeCount;    //剩余空间个数
}MailInfo_S;

typedef struct
{
	INT     m_nCount;  //邮件个数
	BYTE    m_aucMailId[MAX_MAIL_COUNT];
}MailList_S;

typedef struct
{
	Version_S     m_stVersion;
	DWORD         m_dwFrequency; //integer type, not BCD
	BYTE          m_ucOuterFEC;
	BYTE          m_ucModulation;
	DWORD         m_dwSymbolRate; // integer type, not BCD
	BYTE          m_ucInterFEC;
	WORD          m_wDataPid;
}SSUInfo_S;

typedef struct
{
	Version_S     m_stPresentVersion;
	Version_S     m_stNextVersion;
	BYTE          m_ucMaxDelayTime; // 0: immediate, 1: delay one hour 2: delay two hour ..
	BYTE          m_ucWaitTime;
}SSUDialog_S;

#define MAX_CA_UI_ITEM_CAPTION_LEN          (32)
#define MAX_CA_UI_DIALOG_CAPTION_LEN        (64)
#define MAX_CA_UI_DIALOG_ITEM_COUNT         (8)

typedef enum
{
	EM_UI_KEY_0 = 0,       //数字键 0
	EM_UI_KEY_1 = 1,       //数字键 1
	EM_UI_KEY_2 = 2,       //数字键 2
	EM_UI_KEY_3 = 3,       //数字键 3
	EM_UI_KEY_4 = 4,       //数字键 4
	EM_UI_KEY_5 = 5,       //数字键 5
	EM_UI_KEY_6 = 6,       //数字键 6
	EM_UI_KEY_7 = 7,       //数字键 7
	EM_UI_KEY_8 = 8,       //数字键 8
	EM_UI_KEY_9 = 9,       //数字键 9
	EM_UI_KEY_OK = 10,     //OK 键
	EM_UI_KEY_RED = 11,    //红色键
	EM_UI_KEY_GREEN = 12,  //绿色键
	EM_UI_KEY_YELLOW = 13, //黄色键
	EM_UI_KEY_BLUE = 14,   //蓝色键
	EM_UI_KEY_EXIT = 15,   //退出键，表示不选择
	EM_UI_KEY_NONE = 99    //无按键
}EM_UiKey;

typedef struct
{
	BYTE      m_ucItemId;  //选择项目代码，必须>0
	EM_UiKey  m_emKey;     //快捷键值
	BYTE      m_aucCaption[MAX_CA_UI_ITEM_CAPTION_LEN];
}UiChoiceItem_S;

typedef struct
{
	BYTE     m_aucCaption[MAX_CA_UI_DIALOG_CAPTION_LEN];
	WORD     m_wTimeout;                      //等待时间，单位：秒  0 表示永久等待，直到接收到CA_INFO_HIDE_DIALOG消息才清除
	BYTE     m_aucPassword[MAX_PASSWORD_LEN]; //验证密码
	BOOL     m_bVerified;                     //是否需要对密码进行验证
	INT      m_nItemCount;                    //选择项目个数
	UiChoiceItem_S m_astItem[MAX_CA_UI_DIALOG_ITEM_COUNT];  //选择项目
}UiDialogConfig;

typedef struct
{
	BYTE    m_ucItemId;                      //选择项目代码，如果=0，表示未选择任何项目
	BYTE    m_aucPassword[MAX_PASSWORD_LEN]; //使用者输入的密码
}UiDialogResponse;

#define MAIL_INFO

typedef struct
{
	int     m_nMode;
	int     m_nTestId;
}TopcaConfig_S;

typedef struct
{
	WORD    m_wSystemId;  //CA System Id
	WORD    m_wGroupId;   //Group Id
}UiGroupSystemInfo_S;

typedef enum
{
	EM_PWD_ADMIN = 1,      //系统管理密码
	EM_PWD_ENGINEER       //工程密码
}EM_PwdType;

typedef struct
{
	EM_PwdType   m_emRole;    //密码类型
	BYTE         m_aucPwd[MAX_PCTRL_PASSWORD_LEN];  //将要重置的密码
}UiPwdReset_S;

/**************************************************************************************************
 *FunctionName: SetTopcaConfig
 *Descriptor  : 用于控制测试项，
 *	当pstConfig->m_nMode = 1:
 *	  pstConfig->nTestId = 0  启动TOP CA，不会进行测试程序；
 *
 *	当pstConfig->m_nMode = 2: 为测试模式
 *	  pstConfig->nTestId = 0   列出测试项
 *	  pstConfig->nTestId = 1   表示测试base模块
 *	  pstConfig->nTestId = 2   表示测试tbase模块
 *	  pstConfig->nTestId = 3   表示测试task模块
 *	  pstConfig->nTestId = 4   表示测试system(manu1)模块
 *	  pstConfig->nTestId = 5   表示测试system(manu2)模块
 *	  pstConfig->nTestId = 6   表示测试flase模块
 *	  pstConfig->nTestId = 7   表示测试socket TCP模块
 *	  pstConfig->nTestId = 8   表示测试socket UDP模块
 *	  pstConfig->nTestId = 9   表示测试psisi callback模块
 *	  pstConfig->nTestId = 10  表示测试psisi(manu1)模块
 *	  pstConfig->nTestId = 11  表示测试tuner callback模块
 *	  pstConfig->nTestId = 12  表示测试tuner present (manual 2)模块
 *	  pstConfig->nTestId = 13  表示测试tuner ch_mapping (manual 3)模块
 *	  pstConfig->nTestId = 14  表示测试demux (manual)模块
 *	  pstConfig->nTestId = 15  表示测试descramble (manual)模块
 *	  pstConfig->nTestId = 16  表示测试ui (manual)模块
 *	  pstConfig->nTestId = 17  表示测试scrolling (manual)模块
 *	  pstConfig->nTestId = 18  表示测试app (manual)模块

 *调用时机：CA初始化之前调用，如果没有调用此函数，默认情况是release版本(即nTestId = 0)
 **************************************************************************************************/
void SetTopcaConfig(TopcaConfig_S *pstConfig);

/*********************************************************************************************************
 *Function: 获取邮件总个数，已读邮件个数，未读邮件个数，邮箱剩余空间个数
 *
 *FunctionName : CSCAGetMailNum
 *
 *Parameters   :
 *	pstMailInfo[IN/OUT] : APP分配空间，用于存储邮件数
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetMailNum(MailInfo_S *pstMailInfo);

/*********************************************************************************************************
 *Function: 获取邮件标题信息
 *
 *FunctionName : CSCAGetMailTitle
 *
 *Parameters   :
 *	nIndex               : 邮件索引号(从0开始)
 *	pstMailTitle[IN/OUT] : APP分配空间，用于存储邮件标题信息
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetMailTitle(INT nIndex, MailTitle_S *pstMailTitle);

/*********************************************************************************************************
 *Function: 获取邮件内容信息
 *
 *FunctionName : CSCAGetMailContent
 *
 *Parameters   :
 *	nIndex                 : 邮件索引号(从0开始)
 *	pstMailContent[IN/OUT] : APP分配空间，用于存储邮件内容信息
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetMailContent(INT nIndex, MailContent_S *pstMailContent);

/*********************************************************************************************************
 *Function: 通过EmailId获取邮件信息
 *
 *FunctionName : CSCAGetMailIntegerInfoByEmailId
 *
 *Parameters   :
 *	wEmailId                   : 邮件ID号
 *	pstMailIntegerInfo[IN/OUT] : APP分配空间，用于存储邮件信息
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetMailIntegerInfoByEmailId(WORD wEmailId, MailInteger_S *pstMailIntegerInfo);

/*********************************************************************************************************
 *Function: 删除一封邮件
 *
 *FunctionName : CSCADelMail
 *
 *Parameters   :
 *	nIndex                 : 邮件索引号(从0开始)
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCADelMail(INT nIndex);

/*********************************************************************************************************
 *Function: 删除所有邮件
 *
 *FunctionName : CSCADelMailAll
 *
 *Parameters   :
 *	无
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCADelMailAll();

/*********************************************************************************************************
 *Function: 根据索引号设置邮件已读、未读标志位
 *
 *FunctionName : CSCASetMailFlagByIndex
 *
 *Parameters   :
 *	nIndex : 邮件索引号，从0开始
 *	bFlag  : 置邮件标志位,TRUE表示置为已读，FALSE表示置为未读
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCASetMailFlagByIndex(INT nIndex, BOOL bFlag);

/*********************************************************************************************************
 *Function: 根据Email ID设置邮件已读、未读标志位
 *
 *FunctionName : CSCASetMailFlagByEmailId
 *
 *Parameters   :
 *	nIndex : wEmailId邮件ID号
 *	bFlag  : 置邮件标志位,TRUE表示置为已读，FALSE表示置为未读
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCASetMailFlagByEmailId(WORD wEmailId, BOOL bFlag);


#define TOP_CORE

/*********************************************************************************************************
 *Function: set the middle ware provider, stb provider, chipset type, there be used to notify ca system
 *
 *FunctionName : CSCAConfigForTopcas
 *
 *Parameters   :
 *	pstTopcasConfigInfo[IN] : TopcasConfig_S struct, it include middle ware provider, stb provider, 
 *                            chipset type
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAConfigForTopcas(TopcasConfig_S* pstTopcasConfigInfo);

/*********************************************************************************************************
 *Function: 获取进入小应用是否有授权
 *
 *FunctionName : CSCAGetEntitledForEnterApp
 *
 *Parameters   :
 *	nAppType[IN]    : 应用类型-->AppType_E
 *	pbEntitled[OUT] : 该应用是否可以进入
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetEntitledForEnterApp(INT nAppType, BOOL *pbEntitled);

/*********************************************************************************************************
 *Function: 获取Portal信息
 *
 *FunctionName : CSCAGetPortalInfo
 *
 *Parameters   :
 *	pstPortalInfo[IN/OUT]    : 应用分配空间存储Portal信息
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetPortalInfo(AppPortal_S *pstPortalInfo);

/*********************************************************************************************************
 *Function: 获取文本信息
 *
 *FunctionName : CSCAGetTextInfo
 *
 *Parameters   :
 *	pstTextInfo[IN/OUT]    : 应用分配好空间, CA模块将结果输出
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetTextInfo(TextInfo_S *pstTextInfo);

/*********************************************************************************************************
 *Function: 获取频道分组列表
 *
 *FunctionName : CSCAGetChannelGroupList
 *
 *Parameters   :
 *	pstChannelGroupList[IN/OUT]    : 应用分配好空间, CA模块将结果输出
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetChannelGroupList(GroupList_S *pstChannelGroupList);

/*********************************************************************************************************
 *Function: 获取频道分组内容
 *
 *FunctionName : CSCAGetChannelGroupList
 *
 *Parameters   :
 *	dwGroupId[IN]               : 传入Gropu id(由CSCAGetChannelGroupList获取到)
 *	pstChannelGroupList[IN/OUT] : 应用分配好空间, CA模块将结果输出
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetChannelGroupContent(DWORD dwGroupId, CHGroupContent_S *pstChannelGroupContent);

/*********************************************************************************************************
 *Function: 获取分组密码列表
 *
 *FunctionName : CSCAGetGroupPasswordList
 *
 *Parameters   :
 *	dwGroupId[IN]           : 传入Gropu id(由CSCAGetChannelGroupList获取到)
 *	pstPasswordList[IN/OUT] : 应用分配好空间, CA模块将结果输出
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetGroupPasswordList(DWORD dwGroupId, CHGroupPwdList_S *pstPasswordList);

/*********************************************************************************************************
 *Function: 设置分组密码列表
 *
 *FunctionName : CSCASetGroupPasswordList
 *
 *Parameters   :
 *	dwGroupId[IN]       : 传入Gropu id(由CSCAGetChannelGroupList获取到)
 *	pstPasswordList[IN] : 应用分配好空间, CA模块将结果输出
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCASetGroupPasswordList(DWORD dwGroupId, CHGroupPwdList_S *pstPasswordList);

/*********************************************************************************************************
 *Function: 重置分组密码列表
 *
 *FunctionName : CSCAReSetGroupPasswordList
 *
 *Parameters   :
 *	dwGroupId[IN]       : 传入Gropu id(由CSCAGetChannelGroupList获取到)
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAReSetGroupPasswordList(DWORD dwGroupId);

/*********************************************************************************************************
 *Function: 切换频道表分组
 *
 *FunctionName : CSCAChannelGroupSwitch
 *
 *Parameters   :
 *	dwGroupId[IN]       : 传入Gropu id(由CSCAGetChannelGroupList获取到)
 *	pstPassword[IN]     : 传入密码值
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAChannelGroupSwitch(DWORD dwGroupId, CHGroupPwd_S *pstPassword);

/*********************************************************************************************************
 *Function: 设定预设频道表群组
 *
 *FunctionName : CSCAChannelGroupSetDefault
 *
 *Parameters   :
 *	dwGroupId[IN]       : 传入Gropu id(由CSCAGetChannelGroupList获取到)
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAChannelGroupSetDefault(DWORD dwGroupId);

/*********************************************************************************************************
 *Function: 通知CA模块，停止滚动显示
 *
 *FunctionName : CSCAStopScrolling
 *
 *Parameters   :
 *	None       : 无
 *
 *ReturnValues :
 *	无
 *********************************************************************************************************/
void CSCAStopScrolling(void);

/*********************************************************************************************************
 *Function: 通知CA模块，停止Subtitle显示
 *
 *FunctionName : CSCAStopScrolling
 *
 *Parameters   :
 *	None       : 无
 *
 *ReturnValues :
 *	无
 *********************************************************************************************************/
void CSCAStopSubtitle(void);

/*********************************************************************************************************
 *Function: 当开机或在运行过程中改变待机状态时，通知CA模块
 *
 *FunctionName : CSCASetStandbyMode
 *
 *Parameters   :
 *	emStandbyMode[IN] : EM_SYSTEM_POWER_MODE_ACTIVE : 表示Active(正常模式)  
 *						EM_SYSTEM_POWER_MODE_STANDBY: 表示Standby(待机模式)
 *
 *ReturnValues :
 *	无
 *********************************************************************************************************/
void CSCASetStandbyMode(EM_SystemPowerMode emStandbyMode);

/*********************************************************************************************************
 *Function: 当开机或在运行过程中改变语言设置时，通知CA模块
 *
 *FunctionName : CSCASetLanguageType
 *
 *Parameters   :
 *	emLanguageType[IN] : EM_SYSTEM_LANGUAGE_CHI : 表示繁体中文  
 *						 EM_SYSTEM_LANGUAGE_ENG : 表示英文
 *
 *ReturnValues :
 *	无
 *********************************************************************************************************/
void CSCASetLanguageType(EM_LanguageType emLanguageType);

/*********************************************************************************************************
 *Function: 重启机顶盒通知CA模块
 *
 *FunctionName : CSCAReboot
 *
 *Parameters   :
 *	无
 *
 *ReturnValues :
 *	无
 *********************************************************************************************************/
void CSCAReboot(void);

/*********************************************************************************************************
 *Function: 获取频道映射信息
 *
 *FunctionName : CSCAGetChannelMappingInfo
 *
 *Parameters   :
 *	pstChannelListInfo[IN/OUT] : App分配存储空间
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetChannelMappingInfo(CSChannelList_S *pstChannelListInfo);

/*********************************************************************************************************
 *Function: 应用响应对话框，如：要购买IPPV...
 *
 *FunctionName : CSCAUiDialogRespose
 *
 *Parameters   :
 *	pstUiDialogResponse[IN] : App传选择代码及密码给CA模块
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAUiDialogRespose(UiDialogResponse *pstUiDialogResponse);

/*********************************************************************************************************
 *Function: 获取群组ID及CA systemid，用于节目分组和OTA升级
 *
 *FunctionName : CSCAGetGroupSystemInfo
 *
 *Parameters   :
 *	pstGroupSystemInfo[IN] : APP分配空间用于存储Group id和system id
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE-->指针为空
 *********************************************************************************************************/
INT CSCAGetGroupSystemInfo(UiGroupSystemInfo_S* pstGroupSystemInfo);

/*********************************************************************************************************
 *Function Name: CSCASearchCb
 *	当[全频段]搜索完毕，或NIT发生变化后进行[全频段]搜索完毕，调用此函数通知CA模块
 *
 *Parameter: None
 *Return   : None
 *********************************************************************************************************/
void CSCASearchCb(void);

/*********************************************************************************************************
 *Function Name: CSCAGetCurrentPpvInfo
 *	获取当前Ppv信息
 *
 *Parameter: 
 *	pstUiDialogResponse[IN] : APP分配空间用于存储当前Ppv信息
 *
 *Return   : 
 *	0 : SUCCESS
 *	-1: FAILURE-->指针为空
 *********************************************************************************************************/
INT CSCAGetCurrentPpvInfo(UiDialogConfig *pstUiDialogResponse);

//dependence app interface
//由应用实现，供CA模块使用：返回当前系统时间
extern INT CSTopCaGetSysTime(CaDate_S* pstDate);

//由应用实现，供CA模块使用：分配1个Block，函数返回可操作的起始地址;
extern DWORD CS_CA_GetFlashAddress(DWORD dwSize);

//由应用实现，供CA模块使用：用于存储GroupId和CasystemId, 分配大小不能小于8个字节,pdwStartAddr存储返回的起始地址,pdwSize存储分配的大小;
extern void CSCAGetAddressInfoForGroupAndSystemId(DWORD *pdwStartAddr, DWORD *pdwSize);

typedef enum
{
	EM_TUNE_CABLE,
	EM_TUNE_SATELITE,
	EM_TUNE_TERRESTRIAL
}EM_TuneType;
//由应用实现，供CA模块使用：用于告之CA模块当前是有线(EM_TUNE_CABLE)，卫星(EM_TUNE_SATELITE)还是地面(EM_TUNE_TERRESTRIAL)机；
EM_TuneType CSCAGetTuneType();

#ifdef  __cplusplus
}
#endif

#endif //_CS_TOPCA_INTERFACE_H


