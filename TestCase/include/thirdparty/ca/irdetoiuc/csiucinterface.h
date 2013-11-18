/****************************************************************************************************************************
* Notice:    Copyright Shenzhen Coship Electronics Co., Ltd.,
*                 All rights reserved.
* Filename:  csiucinterface.h
* Author:    zhentiejun
* Date:      Sep 2009
* Des:       IUC interface
*
*****************************************************************************************************************************/
#include "mmcp_typedef.h"
#include "csiucloader.h"
#include "cs_cainterface_pvrext.h"
#include "cs_cainterface.h"

/****************************************************************************************************************************
* 			                Revision History                                                                                *
*****************************************************************************************************************************
*  Revision  : 2.0  2012/7/10
*  Programmer: CaoXiaojing
*  Restructure

*  Revision  : 1.0  2009/9/7
*  Programmer: Zhentiejun
*  Create
*****************************************************************************************************************************/
#ifndef _CS_IUC_INTERFACE_H_
#define _CS_IUC_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define CA_DEMUX_MAX (4)
#define CA_SRV_MAX (8)

/*状态码字符串的最大长度*/
#define CA_STATUS_STRING_SIZE (16)

/*private data 长度*/
#define CA_PRIVATEDATA_LEN (16)

/*CA 上传应用的消息类别*/
typedef enum _CaEvent
{
    CA_ALARM,
    CA_INFO,
    CA_COMMAND,
    CA_STATUS
}CaEvent_E;

typedef enum _CaInfo
{
    /*进入CA_MENU_SERVICE_STATUS界面时抛上去的消息,dwParm2 = (DWORD)&CaServiceInfo_S*/
    CA_INFO_SERVICE_STATUS,

    /*进入CA_MENU_LOADER_STATUS界面时抛上去的消息,dwParm2 = (DWORD)&CaLoaderState_S*/
    CA_INFO_LOADER_STATUS,

    /*进入CA_MENU_PRODUCT_STATUS界面时抛上去的消息,dwParm2 = (DWORD)&CaProductList_S*/
    CA_INFO_PRODUCT_STATUS,

    /*进入CA_MENU_CLIENT_STATUS界面时抛上去的消息,dwParm2 = (DWORD)&CaClientInfo_S*/
    CA_INFO_CLIENT_STATUS,
    
    CA_INFO_MAX
}CaInfo_E;

typedef enum _CaCommand
{
     /*Re-Scan Service Information and Reset Channel Order,dwParm2 = 0 */
    CA_COMMAND_APP_HOMECHANNEL_SCAN,

     /*Reconnect to current service,dwParm2 = 0*/
    CA_COMMAND_RECONNECT_CURRENT_SERVICE,

     /*Reset IRD,dwParm2 = 0  */
    CA_COMMAND_REBOOT,             

     /*EMM force download,dwParm2 = (DWORD)&CaEmmOta_S*/	 
    CA_COMMAND_EMM_OTA,

     /*传上去升级流频点信息,dwParm2 = (DWORD)&T_OtaCfg */
    CA_COMMAND_OTA_NEXT, 

     /*是否允许菜单升级,dwParm2 = TRUE/FALSE,TRUE时点亮升级菜单,FALSE时置灰升级菜单 */
    CA_COMMAND_OTA_DETECT,             

     /*Attributed_Display 消息,所带结构体dwParm2 = (DWORD)&CaAttribCovert_S */
    CA_COMMAND_ATTRIB, 

     /*TEXT MESSAGES,dwParm2 = (DWORD)&CaMsg_S*/
    CA_COMMAND_MAIL,

     /*调用CSCAEraseCAFlash()接口，同时恢复出厂设置 */
    CA_COMMAND_RESETTOFACTORY,

     /*更新Service Status 界面的ECM\EMM Monitor信息, dwParm2 = (DWORD)&CaECMEMMMonitor_S */
    CA_COMMAND_MONITOR,

     /*更新Service Status 界面的EMM\DES 状态,dwParm2 = (DWORD)&CaServiceStatus_S*/	 
    CA_COMMAND_EMMDES_STATUS,

     /*更新Service Status 界面的ECM状态,dwParm2 =  (DWORD)&CaECMStatus_S */
    CA_COMMAND_ECM_STATUS,

      /*强制切台到一个指定节目, dwParm2 = (DWORD)&CaIRDServiceInfo_S */
    CA_COMMAND_FORCE_TUNE_TO_SRV,

     /*重置pin码, dwParm2 = (DWORD)&(CaPin_S) */
    CA_COMMAND_INSTALLER_PINCODE,

     /*SHTUNE命令 重新启动+自动搜索 dwParm2 = (DWORD)&T_DeliverySystem */
    CA_COMMAND_HOMECHANNEL_SCAN,

     /*改变PIN 码命令,dwParm2 = (DWORD)&CaPin_S */
    CA_COMMAND_CHANGE_PIN,
	
    CA_COMMAND_MAX
}CaCommand_E;

typedef enum _CaStatus
{
     /*表示当前有OTA升级码流存在,所带结构体dwParm2 = (DWORD)&T_OtaCfg*/
    CA_STATUS_OTA_OK, 
    
     /*表示当前OTA所处阶段,所带结构体dwParm2 = (DWORD)&CaOtaStatus_S*/ 
    CA_STATUS_OTA_STATUS,
    
     /*表示OTA升级失败及失败原因,所带结构体dwParm2 = CaOtaError_E*/
    CA_STATUS_OTA_FAILURE,
    
    CA_STATUS_MAX
}CaStatus_E;

/*ALARM消息状态描述信息*/
typedef enum _CaMsgText
{
    CA_BAKMSG_TEXT000 = 0 , /*Being Descrambled*/
    CA_BAKMSG_TEXT001 = 1 , /*EMM service OK or FTA service*/
    CA_BAKMSG_TEXT002 = 2 , /*PVR Record Request OK*/
    CA_BAKMSG_TEXT003 = 3 , /*PVR Playback Request OK*/
    CA_BAKMSG_TEXT004 = 4 , /*Incomplete Definition*/
    CA_BAKMSG_TEXT005 = 5 , /*No ECM/EMM data received*/
    CA_BAKMSG_TEXT006 = 6 , /*No valid ECM found*/
    CA_BAKMSG_TEXT007 = 7 , /*Invalid EMM received*/
    CA_BAKMSG_TEXT008 = 8 , /*No valid entitlement found*/
    CA_BAKMSG_TEXT009 = 9 , /* No valid sector found*/
    CA_BAKMSG_TEXT010 = 10 , /*P-Key mismatch*/
    CA_BAKMSG_TEXT011 = 11 , /*G-Key mismatch*/
    CA_BAKMSG_TEXT012 = 12 , /*TG mismatch*/
    CA_BAKMSG_TEXT013 = 13 , /*CWDK mismatch*/
    CA_BAKMSG_TEXT014 = 14 , /*Macrovision blocking enforced*/
    CA_BAKMSG_TEXT015 = 15 , /*No valid CA Regional Information found*/
    CA_BAKMSG_TEXT016 = 16 , /*Region or Sub-Region mismatch*/
    CA_BAKMSG_TEXT017 = 17 , /*PVR Record is not allowed*/
    CA_BAKMSG_TEXT018 = 18 , /*Not entitled to perform PVR operation*/
    CA_BAKMSG_TEXT019 = 19 , /*The PVR content is expired*/
    CA_BAKMSG_TEXT020 = 20 , /*Invalid PVR metadata*/
    CA_BAKMSG_TEXT021 = 21 , /*No PVR Session Key to perform PVR operation*/
    CA_BAKMSG_TEXT022 = 22 , /*No matching CA System ID in PMT*/
    CA_BAKMSG_TEXT023 = 23 , /*No matching CA System ID in CAT*/
    CA_BAKMSG_TEXT024 = 24 , /*No PVR Master Session Key to perform PVR operation*/
    CA_BAKMSG_TEXT025 = 25 , /*Initialization has not been performed yet*/
    CA_BAKMSG_TEXT026 = 26 , /*The Pre-Enablement product is expired*/
    CA_BAKMSG_TEXT027 = 27 , /*PESK mismatch*/
    CA_BAKMSG_TEXT028 = 28 , /* Product blackout*/
    CA_BAKMSG_TEXT029 = 29 , /*Product Overwrite EMM handled successfully*/
    CA_BAKMSG_TEXT030 = 30 , /*Product Delete EMM handled successfully*/
    CA_BAKMSG_TEXT031 = 31 , /*Service not allowed*/ 
    CA_BAKMSG_TEXT032 = 32 , /*New CG for FSU product has been received*/
    CA_BAKMSG_TEXT033 = 33 , /*New CG for Push VOD product has been received*/
    CA_BAKMSG_TEXT034 = 34 , /*CG Mismatch*/
    CA_BAKMSG_TEXT035 = 35 , /*SG Mismatch*/
    CA_BAKMSG_TEXT036 = 36 , /*Invalid P-Key Index*/
    CA_BAKMSG_TEXT037 = 37 , /*Invalid G-Key Index*/
    CA_BAKMSG_TEXT038 = 38 , /*Invalid Timestamp received*/
    CA_BAKMSG_TEXT039 = 39 , /*Variant Mismatch*/
    CA_BAKMSG_TEXT040 = 40 , /*P-Key Hash Mismatch*/
    CA_BAKMSG_TEXT041 = 41 , /*Opcode Restricted*/
    CA_BAKMSG_TEXT042 = 42 , /*Invalid VOD Nonce*/
    CA_BAKMSG_TEXT043 = 43 , /*TMS Failed*/
    CA_BAKMSG_TEXT044 = 44 , /*Homing Channel Failed*/
    CA_BAKMSG_TEXT045 = 45 , /*Invalid Middleware User Data*/
    CA_BAKMSG_TEXT046 = 46 , /*Flexi Flash Failed*/
    CA_BAKMSG_TEXT047 = 47 , /*SN Mapping EMM handled successfully*/
    CA_BAKMSG_TEXT048 = 48 , /*Sector Overwrite EMM handled successfully*/
    CA_BAKMSG_TEXT049 = 49 , /*Pairing EMM handled successfully*/
    CA_BAKMSG_TEXT050 = 50 , /*Product Key EMM handled successfully*/
    CA_BAKMSG_TEXT051 = 51 , /*Region Control EMM handled successfully*/
    CA_BAKMSG_TEXT052 = 52 , /*PVR MSK EMM handled successfully*/
    CA_BAKMSG_TEXT053 = 53 , /*VOD Product Overwrite EMM handled successfully*/
    CA_BAKMSG_TEXT054 = 54 , /*VOD Asset ID EMM handled successfully*/
    CA_BAKMSG_TEXT055 = 55 , /*Nationality EMM handled successfully*/
    CA_BAKMSG_TEXT056 = 56 , /*Entitlement Property EMM handled successfully*/
    CA_BAKMSG_TEXT057 = 57 , /*Timestamp EMM handled successfully*/
    CA_BAKMSG_TEXT058 = 58 , /*User Area EMM handled successfully*/
    CA_BAKMSG_TEXT059 = 59 , /*Application Data EMM handled successfully*/
    CA_BAKMSG_TEXT060 = 60 , /*Filter Criteria EMM handled successfully*/
    CA_BAKMSG_TEXT061 = 61 , /*Package EMM handled successfully*/
    CA_BAKMSG_TEXT062 = 62 , /*Block Download EMM handled successfully*/
    CA_BAKMSG_TEXT063 = 63 , /*IRD EMM handled successfully*/
    CA_BAKMSG_TEXT064 = 64 , /*Unique SN Mapping EMM handled successfully*/
    CA_BAKMSG_TEXT065 = 65 , /*Signed CCP CAM EMM handled successfully*/
    CA_BAKMSG_TEXT066 = 66 , /*TM Message EMM handled successfully*/
    CA_BAKMSG_TEXT067 = 67 , /*Macrovision Configuration EMM handled successfully*/
    CA_BAKMSG_TEXT068 = 68 , /*Extended TMS Message EMM handled successfully*/
    CA_BAKMSG_TEXT069 = 69 , /*Reset To Factory State EMM handled successfully*/
    
    CA_BAKMSG_TEXT_MAX
}CaMsgText_E;

/*关音视频的消息,不管E_SrvEsType是1\2\3都要显示提示信息,关掉相应的音视频;
开音视频的消息,打开相应的音视频,只有E_SrvEsType是3才清屏幕消息*/
typedef enum _CaSrvEsType
{
    CA_UNKNOWN,
    CA_AUDIO, /*音频*/
    CA_VIDEO, /*视频*/
    CA_AV,    /*音视频*/
    CA_SUBTITLE,  /*SUBTITLE*/    
    CA_CRYPT_MAX = 0xff
} CaSrvEsType_E;


/*ird 命令强制切台到一个节目的3要素结构*/
typedef struct _CaIRDServiceInfo
{
	WORD m_wOrigNetworkID;//原始网络ID
	WORD m_wTranStreamID;//传输流ID
	WORD m_wServiceID;//节目ID
} CaIRDServiceInfo_S;

typedef struct _CaSrvErrorInfo
{
    BYTE m_aucStatus[CA_STATUS_STRING_SIZE+1];//状态码,字符串
    CaMsgText_E m_eTextnum;                    /*状态描述信息索引*/
} CaSrvErrorInfo_S;

typedef struct _CaErrorInfo
{
    INT m_nIndex;                              /*节目索引号*/
    BYTE m_aucStatus[CA_STATUS_STRING_SIZE+1];//状态码,字符串
    CaMsgText_E m_eTextnum;                    /*状态描述信息索引*/
    CaSrvEsType_E m_eSerEsType;                /*需要开关的音视频类型*/
}CaErrorInfo_S;

typedef struct _CaDateTime
{
    WORD m_wYear;
    BYTE m_ucMonth;
    BYTE m_ucDay;
    BYTE m_ucHour;
    BYTE m_ucMinute;
    BYTE m_ucSecond;
}CaDateTime_S;

/*CA菜单项*/
typedef enum _CaStatusMenu
{
    CA_MENU_SERVICE_STATUS,
    CA_MENU_LOADER_STATUS,
    CA_MENU_PRODUCT_STATUS,
    CA_MENU_CLIENT_STATUS,
    
    CA_MENU_MAX
}CaStatusMenu_E;

/*EM_MENU_LOADER_STATUS菜单返回信息*/
typedef struct _CaLoaderState
{
    BYTE m_ucManufacturerId;
    BYTE m_ucHardwareVersion;
    WORD m_wVariant;
    WORD m_wSystemId;
    WORD m_wKeyVersion;
    WORD m_wSignatureVersion;
    DWORD m_dwLdSECSSN;
    BYTE m_ucDownLoadID;
    BYTE m_aucFirmVersion[CA_MAX_SIZE_FIRMWARE+1]; /*该项内容由应用根据项目实际情况显示The manufacturer downloaded image/application version*/
    BYTE m_aucLoadVersion[CA_MAX_SIZE_LOASER+1];   /*字符串*/
    BYTE m_aucSerialNO[CA_MAX_SIZE_IRD_SERIALNUMBER+1];/*字符串*/
    WORD m_wSpecInfo;
    CaDateTime_S m_stSignDateTime;
    CaDateTime_S m_stDownloadDateTime;
}CaLoaderState_S;

typedef enum _CaServiceType
{
    CA_SERVICE_EMM,  /*Service菜单中显示名字为EMM service*/
    CA_SERVICE_DES,  /*Service菜单中显示名字为Descramble Service*/

    CA_SERVICE_MAX
}CaServiceType_E;

/*每个stream的最大ES个数*/
#define CA_MAXES_PERSTREAM (16)
#define CA_STREAM_STATUS_LENGTH (16)//状态信息的长度

typedef struct _CaStreamStatus
{
    BOOL m_bIsECM;                                  /*是否ECM ,TRUE:ECM,  FALSE:EMM*/
    WORD m_wCAPID;                                  /*ECM\EMM PID*/
    BOOL m_bCanRecord;                              /*Whether the service can be recorded*/
    BYTE m_ucstreamStatus[CA_STREAM_STATUS_LENGTH+1];//stream status显示状态码
    DWORD m_dwESCount;                              /*若m_bIsECM 为TURE则表示该ECM 对应的ES个数*/
    WORD m_wESPID[CA_MAXES_PERSTREAM];              /*若m_bIsECM 为TURE则表示该ECM 对应的ES的PID*/
}CaStreamStatus_S;

#define CA_MAX_STREAMNUMBER_PERHANDLE (5)

typedef struct _CaServiceInfo
{
    DWORD m_dwClientHandle;            /*service handle*/
    CaServiceType_E  m_eServiceType;   /*service type*/
    CaSrvErrorInfo_S m_stServiceStatus;/*service status显示状态码加描述信息*/
    DWORD m_dwStreamCount;             /*service 包含的stream个数*/
    CaStreamStatus_S m_stStreamStatus[CA_MAX_STREAMNUMBER_PERHANDLE];/*每个stream的详细信息*/
}CaServiceInfo_S;

/*EM_MENU_SERVICE_STATUS菜单返回信息*/
typedef struct _CaSvrInfoList
{
    BYTE m_ucServiceCount;                                   /*service 个数*/
    CaServiceInfo_S m_stServiceInfo[CA_DEMUX_MAX+CA_SRV_MAX];/*每个service 详细信息*/
} CaSvrInfoList_S;	

 /*Product ID长度*/
#define CA_PRODUCT_ID_LENGTH (2)

typedef struct _CaProductInfo
{
    BYTE m_ucSectorNum;                       /*以10进制显示*/
    BOOL m_bEntitled;                         /*显示TRUE或FALSE*/
    BYTE m_aucProductId[CA_PRODUCT_ID_LENGTH];/*以四位16进制数形式显示*/
    CaDateTime_S  m_ststartDate;              /*以2010/01/15格式显示*/
    BYTE m_ucduration;                        /*以10进制显示*/
    DWORD m_dwproductType;                    /*0 Normal Product, 1 PVR Product, 2 Push VOD Product, 3 Pre Enablement Product.*/
}CaProductInfo_S;

/*Product 最大个数*/
#define CA_MAX_PRODUCT_COUNT (400)

/*EM_MENU_PRODUCT_STATUS菜单返回信息*/
typedef struct _CaProductList
{
    WORD m_wProductCount;                                 /*个数*/
    CaProductInfo_S m_stProductInfo[CA_MAX_PRODUCT_COUNT];/*每个Product详细信息*/
}CaProductList_S;

#define CA_IUC_GENERIC_LENGTH (512)

typedef struct _CaTMSData
{
    WORD m_wMsgLength;                        /*TMS 消息长度*/
    BYTE  m_aucTmsData[CA_IUC_GENERIC_LENGTH];/*16进制显示*/
}CaTMSData_S;

/*EM_MENU_CLIENT_STATUS 菜单返回信息*/
typedef struct _CaClientInfo
{
    BYTE m_aucIucVersion[CA_IUC_GENERIC_LENGTH+1];/*CA内核版本号，字符串(例如:version 2.5.0-1/1.0)*/
    DWORD m_dwSN;                                 /*Serial Number of the secure client，10进制显示(例如:4080000000)*/
    CaTMSData_S m_stTmsData;                      /*TMS data available on the persistent storage (例如:00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF)*/
    BYTE m_aucCapability[CA_IUC_GENERIC_LENGTH+1];/*字符串(例如:TMS: Y Fingerprint: Y Copycontrol: Y Macrovision: Y CI+: Y ..此处省略...PRODUCTFILTER: Y)*/
    BYTE m_ucSecureTye;                           /*为0时显示"Secure Chipset" ,为1时显示 "Security ID".*/
    WORD m_wLocKId;                               /*Lock ID/Manufacturer ID.16进制显示(例如:23)*/
    DWORD m_dwCaSystemId;                         /*16进制显示(例如:0x624)*/
    DWORD m_dwEcmCount;                           /*Count of ECMS received after start up*/
    DWORD m_dwEmmCount;                           /*Count of EMMs  received after start up*/
    DWORD m_dwCSSN;                               /*Secure chipset ID，16进制显示(例如:AABBCCDD)*/
    BYTE m_aucBuildInfo[CA_IUC_GENERIC_LENGTH+1]; /*CA内核库编译信息，字符串(例如:Built 2012-02-21 15:35)*/
    BYTE m_aucNationality[CA_IUC_GENERIC_LENGTH+1];/*一般是3个字母的字符串澹(例如:USA)*/
    BYTE m_aucClientID[CA_IUC_GENERIC_LENGTH+1];  /*client's serial number,字符串(例如:4080000000(8))*/
}CaClientInfo_S;

/*CA_COMMAND_MONITOR消息所带信息，更新Service status界面的Monitor信息*/
#define CA_MAX_MONITORMSG_LEN (256)

typedef struct _CaECMEMMMonitor
{
    DWORD m_dwClientHandle;                       /*monitor信息对应的Service的Handle*/
    BYTE m_ucMonitorMsg[CA_MAX_MONITORMSG_LEN+1]; /*字符串*/
}CaECMEMMMonitor_S;

/*EM_CA_INFO_EMMDES_STATUS消息所带信息，更新Service status界面的Service Status信息*/
typedef struct _CaServiceStatus
{
    DWORD m_dwClientHandle;                    /*状态发生变化的Service的Handle*/
    BYTE m_aucStatus[CA_STATUS_STRING_SIZE+1];//状态字
    CaMsgText_E m_eTextnum;                    /*状态信息索引*/
}CaServiceStatus_S;

/*EM_CA_INFO_ECM_STATUS消息所带结构体，更新Service status界面的ECM状态信息*/
typedef struct _CaECMStatus
{
    BYTE m_aucStatus[CA_STATUS_STRING_SIZE+1];//状态字
    WORD m_wECMPID;                              /*状态发生变化的ECM 的PID*/
}CaECMStatus_S;

/*TEXT Messsge 内容最大长度*/   
#define CA_MAX_SIZE_TXT_MAIL (511) 

/*TEXT Messsge类型*/
typedef enum _CaTxtType
{
    CA_TXT_TYPE_MAIL,     /*Text -- Mailbox*/
    CA_TXT_TYPE_ANNOUNCE, /*Text -- Announcement*/
    CA_TXT_TYPE_CLUBNUM,  /*Text --Club Numbers(暂不支持)*/

    CA_TXT_TYPE_MAX
}CaTxtType_E;

/*TEXT Messsge显示方式*/
typedef enum _CaTxtShowMode
{
    CA_TXT_SHOW_NORMAL,  /*Normal，显示邮件图标，不弹出*/
    CA_TXT_SHOW_TIMED,	 /*Timed(automatic erase after date/time)*/
    CA_TXT_SHOW_FORCED,  /*Forced Display，在屏幕中间弹出*/

    CA_TXT_SHOW_MAX
}CaTxtShowMode_E;

/*TEXT Messsge属性,forced mail /announcement 立即显示内容不显示邮件图标，normal mail 只显示邮件图标，没有normal announcement*/
typedef struct _CaMsg
{ 
    CaTxtType_E m_eType; 
    CaTxtShowMode_E m_eClass;                 /*当m_eType为 0: Text_Mail 或1: Text_Announcement时有效*/
    BYTE m_ucPriority;                        /*优先级*/
    WORD m_wLen;                              /*TEXT Message长度*/
    CaDateTime_S  m_stDeleteTime;             /*删除时间,当m_cClass值为1 时有效*/
    BYTE m_aucContent[CA_MAX_SIZE_TXT_MAIL+1];/*TEXT Message内容,字符串*/
}CaMsg_S;

/*Attributed_Display消息内容最长字节数*/
#define MAX_ATTRIBDATA_LEN (511)

/*Attributed_Display消息类型*/
typedef enum _CaTmsType
{
    CA_TMS_TYPE_NORMALMAIL ,      /*Normal*/
    CA_TMS_TYPE_FORCEDMAIL ,      /*Forced Text*/
    CA_TMS_TYPE_FINGERPRT ,       /*Fingerprint*/
    CA_TMS_TYPE_FINGERPRTOPTION,  /*Fingerprinting options*/
    
    CA_TMS_TYPE_MAX
}CaTmsType_E;

/*Attributed_Display消息
IucAttribCovert_S:m_eType == 0 等同于 IucCaMsg_S: m_eType == 0 && m_eClass == 0;只显示邮件图标
IucAttribCovert_S:m_eType == 1 等同于 IucCaMsg_S: m_eClass == 2;需要立即显示,不显示邮件图标
IucAttribCovert_S:m_eType == 1 || m_eType == 2  还必须按照要求显示(flash ,banner,covert,duration);
*/
typedef struct _CaAttribCovert
{
    WORD m_wMsgLength;                          /*Attributed_Display 消息长度*/
    BYTE m_aucMessageData[MAX_ATTRIBDATA_LEN+1];/*Attributed_Display 消息内容,字符串数组*/
    CaTmsType_E m_eType;
    
    //以下四个属性只对forced text 和 fingerprint 有效*/
    BOOL m_bFlash;     /*是否闪烁,当闪烁为TRUE时,对于Overted 指纹则是一般的闪烁即可.对于Covert指纹,则要显示只需要显示部分帧画面*/
    BOOL m_bBanner;    /*为TRUE时,需要显示消息 不带标题, 否则需要带标题,目前Irdeto对标题没有要求*/
    BYTE m_ucCoverPer; /*覆盖率,forced text 时是消息框占整个显示屏幕的面积百分比, fingerprint时表示指纹宽度占屏幕宽度的百分比，指纹高度是宽度除以指纹个数*/
    WORD m_wDuration;  /*超时限制,单位秒.为0 时Fingerprint一直显示到下一条Fingerprint到来，Forced Text显示到用户取消显示*/

    BOOL m_bCovert;    /*当属性m_bType为指纹时,m_bCovert表示指纹类型,为TRUE时为Covert 指纹. 为FALSE时为Overt指纹*/

    /*----以下EOF需求 给出的额外结构信息----*/
    BYTE m_ucLocationX; //横坐标位置
    BYTE m_ucLocationY; //纵坐标位置
    BYTE m_ucBgTransAlpha; // 背景透明度
    DWORD m_dwBgColour; //背景颜色
    BYTE m_ucFontTransAlpha; // 字体透明度
    DWORD m_dwFontColour; //字体颜色
    BYTE m_ucFontType; //字体类型
}CaAttribCovert_S;

/*Irdeto CA PIN 码*/
typedef struct  _CaPin
{
    BYTE* m_pucPin; //PIN 码
    BYTE m_ucPinLen;//PIN 码长度
}CaPin_S;

/*保存到邮箱的邮件结构信息*/
typedef struct _CaMail		
{
    BOOL  m_bReadFlag;          /*已读标志*/
    CaDateTime_S m_stArriveTime;/*收到邮件时间*/
    CaMsg_S m_stMail;           /*消息内容*/
} CaMail_S;

/*OTA升级所处阶段,及该阶段还有多少秒剩余时间就会超时.
Step值:
0  正在搜索NIT表
1  正在搜索PAT表
2  正在搜索PMT表
3  正在搜索PID
*/
typedef struct _CaOtaStatus
{
    BYTE m_ucStep;     /*OTA阶段*/
    BYTE m_ucTimeLeft; /*剩余时间*/
}CaOtaStatus_S;

/* OTA 失败类型*/
typedef enum _CaOtaError
{
    CA_OTA_ERROR_1,  /*NIT表没有搜索到*/
    CA_OTA_ERROR_2,  /*PAT,PMT 或者是PID没有搜索到*/

    CA_OTA_ERROR_MAX
}CaOtaError_E;

/*EMM 强制升级命令*/
typedef struct _CaEmmOta
{
    BOOL m_bDownLoadAllowed; /*是否允许下载*/
    BOOL m_bForced;          /*是否为强制*/
}CaEmmOta_S;

/*邮件排序的方式*/
typedef enum _CaEmailSortStyle 					    
{
    CA_EMAIL_TIME_DESC,	/*接收时间早的排在后面*/	
    CA_EMAIL_TIME_ASC,/*接收时间早的排在前面*/	
    CA_EMAIL_SORT_MAX
}CaEmailSortStyle_E;

/**************************************************************************
*Function Name: CSCAGetCACompileTime
*获取CA版本信息
*
*Parameters:
*    pacVerBuf[out]: 存储CA Porting库编译时间的空间地址
*    dwBufSize[in]: 存储CA Porting库编译时间的空间长度
*
*Return Values
*    0: SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAGetCACompileTime(char* pcComTimeBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name:CSCATXTDel
* 删除某一封邮件
*
*Parameters:
*    nIndex[in]: 邮件下标，从0开始
*
*Return Values:
*	0: SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCATXTDel(DWORD dwIndex);

/**************************************************************************
*Function Name:CSCATXTDelAll
* 删除邮箱的所有邮件
*
*Parameters:
*    无
*
*Return Values:
*    0: SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCATXTDelAll(void);

/**************************************************************************
*Function Name:CSCATXTGetNum
* 获取邮箱中的总邮件数
*
*Parameters:
*	无
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
************************************************************************/
INT CSCATXTGetNum(void);

/**************************************************************************
*Function Name:CSCATXTGetInfo
* 获取某一封邮件的内容
*
*Parameters:
*	nIndex[in]: 邮件下标，从0开始
*	ppstTxt[out]: 返回邮件的存储地址
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCATXTGetInfo(DWORD dwIndex, CaMail_S** ppstTxt);

/**************************************************************************
*Function Name:CSCATXTHaveNew
* 查询邮箱中是否有未读邮件
*
*Parameters:
*	无
*
*Return Values:
*	1: TRUE
*	0: FALSE
**************************************************************************/
BOOL CSCATXTHaveNew(void);

/**************************************************************************
*Function Name:CSCATXTHaveNewMailNum
* 获取邮箱中未读邮件个数
*
*Parameters:
*	无
*
*Return Values:
*	返回未读邮件个数
**************************************************************************/
INT CSCATXTHaveNewMailNum(void);

/**************************************************************************
*Function Name:CSCATxtSetReadFlag
* 设置邮件是否已读
*
*Parameters:
*	nIndex[in]: 邮件索引，从0开始
*	bRead[in]: 已读(TRUE),未读(FALSE)
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCATXTSetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name:CSCATXTSort
* 邮件排序
*
*Parameters:
*	emStyle[in]: 排序方式 见CaEmailSortStyle_E的说明
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCATXTSort(CaEmailSortStyle_E emStyle);

/**************************************************************************
*Function Name:CSCAOTAGetTunerParm
* 获取升级时所需的频点参数，通过CA_COMMAND_OTA_NEXT消息异步获得
*
*Parameters:
*	无
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCAOTAGetTunerParm(void);

/**************************************************************************
*Function Name:CSCAOTAGetUpdateInfo
* 获取升级数据的信息，通过CA_STATUS_OTA_OK消息异步获得
*
*Parameters:
*	无
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCAOTAGetUpdateInfo(void);

/**************************************************************************
*Function Name:CSCAOTASaveCfg
* 保存升级频点和数据信息
*
*Parameters:
*    pstOtaCfg[in]: 存放升级频点和数据信息
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT  CSCAOTASaveCfg(T_OtaCfg* pstOtaCfg);

/**************************************************************************
*Function Name: CSCAEnterIrdetoMenu
*APP请求CA菜单界面数据，分别通过各自的INFO消息异步获得
*
*Parameters:
*    eQueryDataType[in]: 请求菜单类型
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAEnterIrdetoMenu( CaStatusMenu_E eQueryDataType);

/**************************************************************************
*Function Name: CSCAEraseCAFlash
*擦除CA对应的FLASH，不包含邮件区域，进恢复出厂菜单和收到
*CA_COMMAND_RESETTOFACTORY命令时调用该接口， 同时做其他恢复出厂的操作
*
*Parameters:
*    无
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAEraseCAFlash(void);

/**************************************************************************
*Function Name: CSCAResetCAData
* 重置CA内核，进恢复出厂菜单时调用该接口
* 同时做其他恢复出厂的操作
*
*Parameters:
*    无
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAResetCAData(void);

/**************************************************************************
*Function Name: CSCAMonitorControl
*  打开或关闭Monitor信息显示
*
*Parameters:
*	dwServiceHandle[in]: servicehandle, 即进入ServiceStatus界面时获得的IucSvrInfoList_S结构体信息中的m_dwClientHandle
*  	ucIsEnable[in]: 进入servicehandle对应菜单时TRUE,打开;退出servicehandle对应菜单时FALSE,关闭
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAMonitorControl( DWORD dwServiceHandle, BOOL bIsEnable);
/**************************************************************************
*Function Name: CSCAPIDChange
* 切换音频或播放subtitle
*
*Parameters:
*	hService[in]:  调用CSCAStartService返回的HCSHANDLE
       eStreamType[in]: 音频切换传CA_AUDIO, 启动subtitle播放传CA_SUBTITLE
*
*Return Values:
*    无
**************************************************************************/
void CSCAPIDChange(HCSHANDLE hService,CaSrvEsType_E eStreamType);

/**************************************************************************
*Function Name: CSCAGetPrivateData
* 获得STB的PrivateData，每个盒子唯一，由应用实现该函数
*
*Parameters:
*    pucPrivateData[in]: 存放PrivateData的Buffer地址
*    pdwDataLen[in]: 存放PrivateData的Buffer大小
*			  [out]: PrivateData的实际长度

*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
extern INT CSCAGetPrivateData(BYTE* pucPrivateData, DWORD* pdwDataLen);

/**************************************************************************
*Function Name: CSCAGetFTAType
* 获取机顶盒信号传输类型，由应用实现该函数
*
*Parameters:
*    无

*Return Values:
*    机顶盒信号传输类型T_CaFtaType
**************************************************************************/
extern T_CaFtaType  CSCAGetFTAType(void);

/**************************************************************************
*Function Name:CsCaGetFlashAddress
* 获取IUC   主数据区的Flash 地址，由应用实现该函数
*
*Parameters:
*    dwSize[In]: 所需数据区的大小.
*
*Return Values:
*     一个连续区域的起始地址
**************************************************************************/
extern DWORD CSCAGetFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetBackupFlashAddress
*  获取IUC  备份数据区的Flash 地址，由应用实现该函数
*
*Parameters:
*    dwSize[In]: 所需数据区的大小
*
*Return Values:
*    一个连续区域的起始地址
**************************************************************************/
extern DWORD CSCAGetBackupFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetFlashMailAddress
*  获取IUC  邮件数据区的Flash 地址，由应用实现该函数
*
*Parameters:
*    dwSize[In]: 所需数据区大小
*
*Return Values:
*    一个连续区域的起始地址
**************************************************************************/
extern DWORD CSCAGetFlashMailAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetFlashBlockSize
*  获取所使用的Flash 一个Block的大小，单位Byte，由应用实现该函数
*
*Parameters:
*    无
*
*Return Values:
*    所使用的Flash 一个Block的大小，单位Byte
**************************************************************************/
extern DWORD CSCAGetFlashBlockSize(void);

/**************************************************************************
* Function Name: CSCAGetEpgTime
* 获取STB 当前EPG 时间. 由应用实现该函数
*
*Parameters:
*    pstDataTime[out]: 存放STB 当前EPG  时间
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
extern INT CSCAGetEpgTime(CaDateTime_S* pstDataTime);

#ifdef  __cplusplus
}
#endif

#endif //_CS_IUC_INTERFACE_H_

