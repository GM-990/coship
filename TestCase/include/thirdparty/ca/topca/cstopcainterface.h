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
	CA_ALARM_CANCEL,             //���ALAMR��Ϣ��������Ƶ
	CA_ALARM_CLEAR_EVENT,        //��������¼���Ϣ�����ô�����Ƶ
	
	//����ALARM��Ϣ��Я������Ϣ�ṹ��Ϊ��CaEventInfo_S(��dw2 = &CaEventInfo_S)
	
	CA_ALARM_FLASH_REDEPLOYED,   //�����д洢�ṹ���·�����
	CA_ALARM_START_REGISTER,     //��ʼע��
	CA_ALARM_STOP_REGISTER,      //����ע�ᣬTop Core���������в���ʱ��Alarm��ʾ����ͨ������Ϣ�����
	CA_ALARM_REGISTER_FAILED,    //ע��ʧ��
	CA_ALARM_REGISTER_SUCCESS,   //ע��ɹ�
	CA_ALARM_GRANT_ENTITLEMENT,  //��ȡ��Ȩ(���ڻ�ȡ)
	CA_ALARM_ENTITLEMENT_SUCCESS,//��ȡ��Ȩ�ɹ���������Ƶ
	CA_ALAMR_REVOKE_ENTITLEMENT, //ȡ����Ȩ
	CA_ALARM_NOENTITLE,          //û����Ȩ
	CA_ALARM_PASSWORD_FAILED,    //��֤����ʧ��
	CA_ALARM_INTERNAL_ERROR,     //�ڲ�����	
	CA_ALARM_SWITCH_CHANNEL_GROUP, //�л�Ƶ��Ⱥ��
	CA_ALARM_UPDATE_CHANNEL_GROUP, //����Ƶ��Ⱥ��	
	CA_ALARM_OTHER_MESSAGE,      //����ѶϢ
	CA_ALARM_MAIL_SPACE_FULL,    //����ռ���������ʾ��Ϣ��ʾ����Ļ���Ϸ�
	CA_ALARM_NEWMAIL_NOTIFY,     //���ʼ�֪ͨ����ʾ��Ϣ��ʾ����Ļ���Ϸ�
	CA_ALARM_URGENCYMAIL_NOTIFY, //�����ʼ�֪ͨ����ʾ��Ϣ��ʾ����Ļ���Ϸ�
	
	CA_ALARM_MAX
}E_CaAlarm;

typedef enum _CaInfo
{
	CA_INFO_START = (CS_TOPCA_MESSAGE_BASE + CA_ALARM_MAX + 1),//
	CA_INFO_OSD,                //Ŀǰû��ʹ�õ�����Ϣ
	CA_INFO_SHOW_DIALOG,        //Ҫ��ʾ�ĶԻ���dwParm2 = &UiDialogConfig
	CA_INFO_HIDE_DIALOG,        //�����Ի���
	
	CA_INFO_MAX	
}E_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_START = (CS_TOPCA_MESSAGE_BASE + CA_INFO_MAX + 1),//
	CA_COMMAND_ACTIVE,   //dwParm2 = n second(��ʾn���ָ�����)
	CA_COMMAND_STANDBY,  //dwParm2 = n second(��ʾn���������)
	CA_COMMAND_REBOOT,   //dwParm2 = n second(��ʾn�������������)
	/*������app�յ�CA_COMMAND_START_CHANNELLIST��Ϣ�󣬶����л�Ƶ��ʱҪ�Ƚϣ�
	  ���Ҫ�л���Ƶ��serviceid����CSChannelList_S�ṹ��Ӧ��list�ڣ�����̨���ɹ�*/
	CA_COMMAND_START_CHANNELLIST,  //dwParm2 = &CSChannelList_S
	CA_COMMAND_STOP_CHANNELLIST,  /*ֹͣ��channellist�Ƚϣ������л�����Ӧ��Ƶ��*/
	CA_COMMAND_TUNER_CHANNELID,   /*�л���ָ����channel idƵ����*/
	CA_COMMAND_TUNER_UP,          /*�����л�̨*/
	CA_COMMAND_TUNER_DOWN,        /*�����л�̨*/
	CA_COMMAND_OSD_SHOW,          /*��ʾOSD����Ϣ�ṹ��OsdInfo_S*/
	CA_COMMAND_OSD_HID,           /*����OSD*/
	CA_COMMAND_PORTAL,            /*֪ͨAPP��������СӦ��, ���ݽṹAppPortal_S*/
	CA_COMMAND_PCCTROL,           /*���������ƣ���Ϣ�ṹ�壺PctrlPasswordList_S*/
	CA_COMMAND_OTA,               /*֪ͨAPP������Ϣ���棬������������Ƶ���ƣ���Ϣ�ṹ�壺SSUInfo_S*/
	CA_COMMAND_SSU_SHOW,          /*OTA������ʾ��Ϣ�����ڽ�����ʾ�� ��Ϣ�ṹ�壺SSUDialog_S*/
	CA_COMMAND_RESET_PWD,         /*����ϵͳ�򹤳����룬��Ϣ�ṹ�壺UiPwdReset_S*/
	
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
	EM_SYSTEM_LANGUAGE_CHI = 1, //��������
	EM_SYSTEM_LANGUAGE_ENG = 2  //Ӣ��
}EM_LanguageType;

typedef enum
{
	EM_SYSTEM_ROLE_ADMIN = 1,    //�����ʺ�
	EM_SYSTEM_ROLE_ENGINEER = 2  //�����ʺ�
}EM_SystemRole;

typedef enum
{
	EM_SYSTEM_POWER_MODE_UNKNOWN = 0,
	EM_SYSTEM_POWER_MODE_ACTIVE = 1,  //����
	EM_SYSTEM_POWER_MODE_STANDBY = 2  //����
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
	EM_UI_EVENT_TYPE_NOTIFICATION = 0, //�¼�֪ͨ
	EM_UI_EVENT_TYPE_INFO = 1,         //��Ѷ������Ļ���Ϸ���ʾ
	EM_UI_EVENT_TYPE_WARNING = 2,      //������Ϣ������Ļ�м���ʾ
	EM_UI_EVENT_TYPE_ERROR = 3         //������Ϣ������Ļ�м���ʾ
}EM_UiEventType;

typedef enum
{
	EM_UI_EVENT_MESSAGE_FLASH_REDEPLOYED = 1,      //�����д洢�ṹ��������
	EM_UI_EVENT_MESSAGE_START_REGISTER = 10,       //��ʼע��
	EM_UI_EVENT_MESSAGE_STOP_REGISTER = 11,        //����ע��
	EM_UI_EVENT_MESSAGE_REGISTER_FAILED = 12,      //ע��ʧ��
	EM_UI_EVENT_MESSAGE_REGISTER_SUCCESSFUL = 13,  //ע��ɹ�
	EM_UI_EVENT_MESSAGE_GRANT_ENTITLEMENT = 20,    //��ȡ��Ȩ
	EM_UI_EVENT_MESSAGE_REVOKE_ENTITLEMENT = 21,   //ȡ����Ȩ
	EM_UI_EVENT_MESSAGE_NO_ENTITLEMENT = 22,       //δ�����Ȩ
	EM_UI_EVENT_MESSAGE_NEW_MAIL = 30,             //��ȡ�µ��ʼ�
	EM_UI_EVENT_MESSAGE_HIGHT_GRADE_MAIL = 31,     //��ȡһ��ߵȼ��ʼ�����ʾѶϢ�������������ʼ�
	EM_UI_EVENT_MESSAGE_MAIL_SPACE_FULL = 32,      //�ʼ��ռ�����
	EM_UI_EVENT_MESSAGE_SWITCH_CHANNEL_GROUP = 40, //�л�Ƶ��Ⱥ��
	EM_UI_EVENT_MESSAGE_PASSWORD_FAILED = 41,      //��֤����ʧ��
	EM_UI_EVENT_MESSAGE_INTERNAL_ERROR = 90,       //�ڲ�����
	EM_UI_EVENT_MESSAGE_OTHER = 91                 //����ѶϢ
}EM_UiEventMessage;

typedef struct
{
	INT     m_nMessageType; //ΪEM_UiEventType����
	INT     m_nMessage;     //ΪEM_UiEventMessage����
	INT     m_nP1;  //����P1 ��p1=0:��ʾ��ʾ��Ϣ�����ñ�����ֱ�������Ϣ����  ��p1=n:��ʾ��ʾ��Ϣ����ʾn���Ӻ���ʧ
	INT     m_nP2;  //����ϢΪCA_ALARM_URGENCYMAIL_NOTIFYʱ�����ֶ���Ч��m_nP2ΪEmailId��ͨ��EmailId��ȡ�ʼ����ݣ�����������ʾ
	BYTE    m_aucText[CA_MAX_EVENT_TEXT_LEN];  //��ʾ������Ϣ
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
typedef struct _ServiceInfo  //Ƶ��(��Ŀ)��Ϣ
{
	WORD     m_wChannelId;
	WORD     m_wServiceId;
}CSServiceInfo_S;

typedef struct _ChannelList
{
	DWORD    m_dwChannelCount; //Ƶ������
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
	EM_APP_TYPE_DEVICE_INFO = 1,            //�豸��Ѷ
	EM_APP_TYPE_COMMON_SETTING = 2,         //һ���趨
	EM_APP_TYPE_ADVANCE_SETTING = 3,        //�����趨
	EM_APP_TYPE_ENGINEERING_SETTING = 4,    //�����趨
	EM_APP_TYPE_EPG = 5,                    //��Ŀָ�ϣ�����ο�Bouquet_id
	EM_APP_TYPE_TV_SERVICE = 6,             //����ֱ��
	EM_APP_TYPE_RADIO_SERVICE = 7,          //�㲥��̨
	EM_APP_TYPE_FAVORITE_CHANNEL = 8,       //ϲ��Ƶ��
	EM_APP_TYPE_PROGRAM_BOOKING = 9,        //ԤԼ����
	EM_APP_TYPE_CA_MAIL = 10,               //CA�ʼ�
	EM_APP_TYPE_CHANNEL_GROUP = 11,         //Ƶ����Ⱥ��
	EM_APP_TYPE_WEB_INFO = 12,              //��ʱ��Ѷ
	EM_APP_TYPE_LOCAL_TIME_SHIFT = 13,      //ʱ�Ƶ���
	EM_APP_TYPE_LOCAL_PVR = 14,             //��Ŀ¼Ӱ
	EM_APP_TYPE_LOCAL_MP3_PLAYER = 15,      //MP3����
	EM_APP_TYPE_LOCAL_PICTURE_BROWSER = 16, //��Ƭ���
	EM_APP_TYPE_LOCAL_MEDIA_PLAYER = 17,    //Ӱ������
	EM_APP_TYPE_LOCAL_PUSH_VOD = 18         //PUSH VOD
}AppType_E;

typedef struct
{
    AppType_E  m_eType;      //����
    WORD       m_wItemId;    //���
    WORD       m_wItemIcon;  //ͼʾ
    BYTE       m_aucItemNname[MAX_APP_ITEM_NAME_LEN]; //��Ŀ����
    WORD       m_wBouquetId;
    BOOL       m_bCharged;  //�Ƿ�Ϊ�շ�Ӧ�ã����Ϊ�շ�Ӧ�ã������ʾ�շ�ͼʾ
}AppItem_S;

typedef struct
{
	BYTE       m_cCategoryId;   //���
	BYTE       m_cCategoryIcon; //ͼʾ
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
	EM_MAIL_ADDRESS_GLOBAL = 1,  //�������л����е��ʼ�
	EM_MAIL_ADDRESS_GROUP = 2,   //����Ⱥ������е��ʼ�
	EM_MAIL_ADDRESS_SINGLE = 3   //����ָ�������е��ʼ�
}EM_MAILADDRESS;

typedef enum
{
	EM_MAIL_DEGREE_HIGH = 1,   //�ʼ��б��� ����һ����ʾΪ ��
	EM_MAIL_DEGREE_NORMAL = 2, //�ʼ��б��� ����һ����ʾΪ ��
	EM_MAIL_DEGREE_LOW = 3     //�ʼ��б��� ����һ����ʾΪ ��
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
	DWORD   m_dwMailId;         //�ʼ���ʶ��
	EM_MAILADDRESS m_emAddr;    //�ʼ�Ѱַ
	EM_MAILDEGREE  m_emDegree;  //�ʼ��ȼ�
	BOOL           m_bIsRead;   //�Ѷ���־
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
	BYTE    m_ucTotalCount;   //�ʼ��ܸ���
	BYTE    m_ucHadReadCount; //�Ѷ��ʼ�����
	BYTE    m_ucNoReadCount;  //δ���ʼ�����
	BYTE    m_ucFreeCount;    //ʣ��ռ����
}MailInfo_S;

typedef struct
{
	INT     m_nCount;  //�ʼ�����
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
	EM_UI_KEY_0 = 0,       //���ּ� 0
	EM_UI_KEY_1 = 1,       //���ּ� 1
	EM_UI_KEY_2 = 2,       //���ּ� 2
	EM_UI_KEY_3 = 3,       //���ּ� 3
	EM_UI_KEY_4 = 4,       //���ּ� 4
	EM_UI_KEY_5 = 5,       //���ּ� 5
	EM_UI_KEY_6 = 6,       //���ּ� 6
	EM_UI_KEY_7 = 7,       //���ּ� 7
	EM_UI_KEY_8 = 8,       //���ּ� 8
	EM_UI_KEY_9 = 9,       //���ּ� 9
	EM_UI_KEY_OK = 10,     //OK ��
	EM_UI_KEY_RED = 11,    //��ɫ��
	EM_UI_KEY_GREEN = 12,  //��ɫ��
	EM_UI_KEY_YELLOW = 13, //��ɫ��
	EM_UI_KEY_BLUE = 14,   //��ɫ��
	EM_UI_KEY_EXIT = 15,   //�˳�������ʾ��ѡ��
	EM_UI_KEY_NONE = 99    //�ް���
}EM_UiKey;

typedef struct
{
	BYTE      m_ucItemId;  //ѡ����Ŀ���룬����>0
	EM_UiKey  m_emKey;     //��ݼ�ֵ
	BYTE      m_aucCaption[MAX_CA_UI_ITEM_CAPTION_LEN];
}UiChoiceItem_S;

typedef struct
{
	BYTE     m_aucCaption[MAX_CA_UI_DIALOG_CAPTION_LEN];
	WORD     m_wTimeout;                      //�ȴ�ʱ�䣬��λ����  0 ��ʾ���õȴ���ֱ�����յ�CA_INFO_HIDE_DIALOG��Ϣ�����
	BYTE     m_aucPassword[MAX_PASSWORD_LEN]; //��֤����
	BOOL     m_bVerified;                     //�Ƿ���Ҫ�����������֤
	INT      m_nItemCount;                    //ѡ����Ŀ����
	UiChoiceItem_S m_astItem[MAX_CA_UI_DIALOG_ITEM_COUNT];  //ѡ����Ŀ
}UiDialogConfig;

typedef struct
{
	BYTE    m_ucItemId;                      //ѡ����Ŀ���룬���=0����ʾδѡ���κ���Ŀ
	BYTE    m_aucPassword[MAX_PASSWORD_LEN]; //ʹ�������������
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
	EM_PWD_ADMIN = 1,      //ϵͳ��������
	EM_PWD_ENGINEER       //��������
}EM_PwdType;

typedef struct
{
	EM_PwdType   m_emRole;    //��������
	BYTE         m_aucPwd[MAX_PCTRL_PASSWORD_LEN];  //��Ҫ���õ�����
}UiPwdReset_S;

/**************************************************************************************************
 *FunctionName: SetTopcaConfig
 *Descriptor  : ���ڿ��Ʋ����
 *	��pstConfig->m_nMode = 1:
 *	  pstConfig->nTestId = 0  ����TOP CA��������в��Գ���
 *
 *	��pstConfig->m_nMode = 2: Ϊ����ģʽ
 *	  pstConfig->nTestId = 0   �г�������
 *	  pstConfig->nTestId = 1   ��ʾ����baseģ��
 *	  pstConfig->nTestId = 2   ��ʾ����tbaseģ��
 *	  pstConfig->nTestId = 3   ��ʾ����taskģ��
 *	  pstConfig->nTestId = 4   ��ʾ����system(manu1)ģ��
 *	  pstConfig->nTestId = 5   ��ʾ����system(manu2)ģ��
 *	  pstConfig->nTestId = 6   ��ʾ����flaseģ��
 *	  pstConfig->nTestId = 7   ��ʾ����socket TCPģ��
 *	  pstConfig->nTestId = 8   ��ʾ����socket UDPģ��
 *	  pstConfig->nTestId = 9   ��ʾ����psisi callbackģ��
 *	  pstConfig->nTestId = 10  ��ʾ����psisi(manu1)ģ��
 *	  pstConfig->nTestId = 11  ��ʾ����tuner callbackģ��
 *	  pstConfig->nTestId = 12  ��ʾ����tuner present (manual 2)ģ��
 *	  pstConfig->nTestId = 13  ��ʾ����tuner ch_mapping (manual 3)ģ��
 *	  pstConfig->nTestId = 14  ��ʾ����demux (manual)ģ��
 *	  pstConfig->nTestId = 15  ��ʾ����descramble (manual)ģ��
 *	  pstConfig->nTestId = 16  ��ʾ����ui (manual)ģ��
 *	  pstConfig->nTestId = 17  ��ʾ����scrolling (manual)ģ��
 *	  pstConfig->nTestId = 18  ��ʾ����app (manual)ģ��

 *����ʱ����CA��ʼ��֮ǰ���ã����û�е��ô˺�����Ĭ�������release�汾(��nTestId = 0)
 **************************************************************************************************/
void SetTopcaConfig(TopcaConfig_S *pstConfig);

/*********************************************************************************************************
 *Function: ��ȡ�ʼ��ܸ������Ѷ��ʼ�������δ���ʼ�����������ʣ��ռ����
 *
 *FunctionName : CSCAGetMailNum
 *
 *Parameters   :
 *	pstMailInfo[IN/OUT] : APP����ռ䣬���ڴ洢�ʼ���
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetMailNum(MailInfo_S *pstMailInfo);

/*********************************************************************************************************
 *Function: ��ȡ�ʼ�������Ϣ
 *
 *FunctionName : CSCAGetMailTitle
 *
 *Parameters   :
 *	nIndex               : �ʼ�������(��0��ʼ)
 *	pstMailTitle[IN/OUT] : APP����ռ䣬���ڴ洢�ʼ�������Ϣ
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetMailTitle(INT nIndex, MailTitle_S *pstMailTitle);

/*********************************************************************************************************
 *Function: ��ȡ�ʼ�������Ϣ
 *
 *FunctionName : CSCAGetMailContent
 *
 *Parameters   :
 *	nIndex                 : �ʼ�������(��0��ʼ)
 *	pstMailContent[IN/OUT] : APP����ռ䣬���ڴ洢�ʼ�������Ϣ
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetMailContent(INT nIndex, MailContent_S *pstMailContent);

/*********************************************************************************************************
 *Function: ͨ��EmailId��ȡ�ʼ���Ϣ
 *
 *FunctionName : CSCAGetMailIntegerInfoByEmailId
 *
 *Parameters   :
 *	wEmailId                   : �ʼ�ID��
 *	pstMailIntegerInfo[IN/OUT] : APP����ռ䣬���ڴ洢�ʼ���Ϣ
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetMailIntegerInfoByEmailId(WORD wEmailId, MailInteger_S *pstMailIntegerInfo);

/*********************************************************************************************************
 *Function: ɾ��һ���ʼ�
 *
 *FunctionName : CSCADelMail
 *
 *Parameters   :
 *	nIndex                 : �ʼ�������(��0��ʼ)
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCADelMail(INT nIndex);

/*********************************************************************************************************
 *Function: ɾ�������ʼ�
 *
 *FunctionName : CSCADelMailAll
 *
 *Parameters   :
 *	��
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCADelMailAll();

/*********************************************************************************************************
 *Function: ���������������ʼ��Ѷ���δ����־λ
 *
 *FunctionName : CSCASetMailFlagByIndex
 *
 *Parameters   :
 *	nIndex : �ʼ������ţ���0��ʼ
 *	bFlag  : ���ʼ���־λ,TRUE��ʾ��Ϊ�Ѷ���FALSE��ʾ��Ϊδ��
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCASetMailFlagByIndex(INT nIndex, BOOL bFlag);

/*********************************************************************************************************
 *Function: ����Email ID�����ʼ��Ѷ���δ����־λ
 *
 *FunctionName : CSCASetMailFlagByEmailId
 *
 *Parameters   :
 *	nIndex : wEmailId�ʼ�ID��
 *	bFlag  : ���ʼ���־λ,TRUE��ʾ��Ϊ�Ѷ���FALSE��ʾ��Ϊδ��
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
 *Function: ��ȡ����СӦ���Ƿ�����Ȩ
 *
 *FunctionName : CSCAGetEntitledForEnterApp
 *
 *Parameters   :
 *	nAppType[IN]    : Ӧ������-->AppType_E
 *	pbEntitled[OUT] : ��Ӧ���Ƿ���Խ���
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetEntitledForEnterApp(INT nAppType, BOOL *pbEntitled);

/*********************************************************************************************************
 *Function: ��ȡPortal��Ϣ
 *
 *FunctionName : CSCAGetPortalInfo
 *
 *Parameters   :
 *	pstPortalInfo[IN/OUT]    : Ӧ�÷���ռ�洢Portal��Ϣ
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetPortalInfo(AppPortal_S *pstPortalInfo);

/*********************************************************************************************************
 *Function: ��ȡ�ı���Ϣ
 *
 *FunctionName : CSCAGetTextInfo
 *
 *Parameters   :
 *	pstTextInfo[IN/OUT]    : Ӧ�÷���ÿռ�, CAģ�齫������
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetTextInfo(TextInfo_S *pstTextInfo);

/*********************************************************************************************************
 *Function: ��ȡƵ�������б�
 *
 *FunctionName : CSCAGetChannelGroupList
 *
 *Parameters   :
 *	pstChannelGroupList[IN/OUT]    : Ӧ�÷���ÿռ�, CAģ�齫������
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetChannelGroupList(GroupList_S *pstChannelGroupList);

/*********************************************************************************************************
 *Function: ��ȡƵ����������
 *
 *FunctionName : CSCAGetChannelGroupList
 *
 *Parameters   :
 *	dwGroupId[IN]               : ����Gropu id(��CSCAGetChannelGroupList��ȡ��)
 *	pstChannelGroupList[IN/OUT] : Ӧ�÷���ÿռ�, CAģ�齫������
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetChannelGroupContent(DWORD dwGroupId, CHGroupContent_S *pstChannelGroupContent);

/*********************************************************************************************************
 *Function: ��ȡ���������б�
 *
 *FunctionName : CSCAGetGroupPasswordList
 *
 *Parameters   :
 *	dwGroupId[IN]           : ����Gropu id(��CSCAGetChannelGroupList��ȡ��)
 *	pstPasswordList[IN/OUT] : Ӧ�÷���ÿռ�, CAģ�齫������
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetGroupPasswordList(DWORD dwGroupId, CHGroupPwdList_S *pstPasswordList);

/*********************************************************************************************************
 *Function: ���÷��������б�
 *
 *FunctionName : CSCASetGroupPasswordList
 *
 *Parameters   :
 *	dwGroupId[IN]       : ����Gropu id(��CSCAGetChannelGroupList��ȡ��)
 *	pstPasswordList[IN] : Ӧ�÷���ÿռ�, CAģ�齫������
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCASetGroupPasswordList(DWORD dwGroupId, CHGroupPwdList_S *pstPasswordList);

/*********************************************************************************************************
 *Function: ���÷��������б�
 *
 *FunctionName : CSCAReSetGroupPasswordList
 *
 *Parameters   :
 *	dwGroupId[IN]       : ����Gropu id(��CSCAGetChannelGroupList��ȡ��)
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAReSetGroupPasswordList(DWORD dwGroupId);

/*********************************************************************************************************
 *Function: �л�Ƶ�������
 *
 *FunctionName : CSCAChannelGroupSwitch
 *
 *Parameters   :
 *	dwGroupId[IN]       : ����Gropu id(��CSCAGetChannelGroupList��ȡ��)
 *	pstPassword[IN]     : ��������ֵ
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAChannelGroupSwitch(DWORD dwGroupId, CHGroupPwd_S *pstPassword);

/*********************************************************************************************************
 *Function: �趨Ԥ��Ƶ����Ⱥ��
 *
 *FunctionName : CSCAChannelGroupSetDefault
 *
 *Parameters   :
 *	dwGroupId[IN]       : ����Gropu id(��CSCAGetChannelGroupList��ȡ��)
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAChannelGroupSetDefault(DWORD dwGroupId);

/*********************************************************************************************************
 *Function: ֪ͨCAģ�飬ֹͣ������ʾ
 *
 *FunctionName : CSCAStopScrolling
 *
 *Parameters   :
 *	None       : ��
 *
 *ReturnValues :
 *	��
 *********************************************************************************************************/
void CSCAStopScrolling(void);

/*********************************************************************************************************
 *Function: ֪ͨCAģ�飬ֹͣSubtitle��ʾ
 *
 *FunctionName : CSCAStopScrolling
 *
 *Parameters   :
 *	None       : ��
 *
 *ReturnValues :
 *	��
 *********************************************************************************************************/
void CSCAStopSubtitle(void);

/*********************************************************************************************************
 *Function: �������������й����иı����״̬ʱ��֪ͨCAģ��
 *
 *FunctionName : CSCASetStandbyMode
 *
 *Parameters   :
 *	emStandbyMode[IN] : EM_SYSTEM_POWER_MODE_ACTIVE : ��ʾActive(����ģʽ)  
 *						EM_SYSTEM_POWER_MODE_STANDBY: ��ʾStandby(����ģʽ)
 *
 *ReturnValues :
 *	��
 *********************************************************************************************************/
void CSCASetStandbyMode(EM_SystemPowerMode emStandbyMode);

/*********************************************************************************************************
 *Function: �������������й����иı���������ʱ��֪ͨCAģ��
 *
 *FunctionName : CSCASetLanguageType
 *
 *Parameters   :
 *	emLanguageType[IN] : EM_SYSTEM_LANGUAGE_CHI : ��ʾ��������  
 *						 EM_SYSTEM_LANGUAGE_ENG : ��ʾӢ��
 *
 *ReturnValues :
 *	��
 *********************************************************************************************************/
void CSCASetLanguageType(EM_LanguageType emLanguageType);

/*********************************************************************************************************
 *Function: ����������֪ͨCAģ��
 *
 *FunctionName : CSCAReboot
 *
 *Parameters   :
 *	��
 *
 *ReturnValues :
 *	��
 *********************************************************************************************************/
void CSCAReboot(void);

/*********************************************************************************************************
 *Function: ��ȡƵ��ӳ����Ϣ
 *
 *FunctionName : CSCAGetChannelMappingInfo
 *
 *Parameters   :
 *	pstChannelListInfo[IN/OUT] : App����洢�ռ�
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAGetChannelMappingInfo(CSChannelList_S *pstChannelListInfo);

/*********************************************************************************************************
 *Function: Ӧ����Ӧ�Ի����磺Ҫ����IPPV...
 *
 *FunctionName : CSCAUiDialogRespose
 *
 *Parameters   :
 *	pstUiDialogResponse[IN] : App��ѡ����뼰�����CAģ��
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE
 *********************************************************************************************************/
INT CSCAUiDialogRespose(UiDialogResponse *pstUiDialogResponse);

/*********************************************************************************************************
 *Function: ��ȡȺ��ID��CA systemid�����ڽ�Ŀ�����OTA����
 *
 *FunctionName : CSCAGetGroupSystemInfo
 *
 *Parameters   :
 *	pstGroupSystemInfo[IN] : APP����ռ����ڴ洢Group id��system id
 *
 *ReturnValues :
 *	0 : SUCCESS
 *	-1: FAILURE-->ָ��Ϊ��
 *********************************************************************************************************/
INT CSCAGetGroupSystemInfo(UiGroupSystemInfo_S* pstGroupSystemInfo);

/*********************************************************************************************************
 *Function Name: CSCASearchCb
 *	��[ȫƵ��]������ϣ���NIT�����仯�����[ȫƵ��]������ϣ����ô˺���֪ͨCAģ��
 *
 *Parameter: None
 *Return   : None
 *********************************************************************************************************/
void CSCASearchCb(void);

/*********************************************************************************************************
 *Function Name: CSCAGetCurrentPpvInfo
 *	��ȡ��ǰPpv��Ϣ
 *
 *Parameter: 
 *	pstUiDialogResponse[IN] : APP����ռ����ڴ洢��ǰPpv��Ϣ
 *
 *Return   : 
 *	0 : SUCCESS
 *	-1: FAILURE-->ָ��Ϊ��
 *********************************************************************************************************/
INT CSCAGetCurrentPpvInfo(UiDialogConfig *pstUiDialogResponse);

//dependence app interface
//��Ӧ��ʵ�֣���CAģ��ʹ�ã����ص�ǰϵͳʱ��
extern INT CSTopCaGetSysTime(CaDate_S* pstDate);

//��Ӧ��ʵ�֣���CAģ��ʹ�ã�����1��Block���������ؿɲ�������ʼ��ַ;
extern DWORD CS_CA_GetFlashAddress(DWORD dwSize);

//��Ӧ��ʵ�֣���CAģ��ʹ�ã����ڴ洢GroupId��CasystemId, �����С����С��8���ֽ�,pdwStartAddr�洢���ص���ʼ��ַ,pdwSize�洢����Ĵ�С;
extern void CSCAGetAddressInfoForGroupAndSystemId(DWORD *pdwStartAddr, DWORD *pdwSize);

typedef enum
{
	EM_TUNE_CABLE,
	EM_TUNE_SATELITE,
	EM_TUNE_TERRESTRIAL
}EM_TuneType;
//��Ӧ��ʵ�֣���CAģ��ʹ�ã����ڸ�֮CAģ�鵱ǰ������(EM_TUNE_CABLE)������(EM_TUNE_SATELITE)���ǵ���(EM_TUNE_TERRESTRIAL)����
EM_TuneType CSCAGetTuneType();

#ifdef  __cplusplus
}
#endif

#endif //_CS_TOPCA_INTERFACE_H


