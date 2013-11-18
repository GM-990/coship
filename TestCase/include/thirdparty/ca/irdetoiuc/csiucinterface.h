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

/*״̬���ַ�������󳤶�*/
#define CA_STATUS_STRING_SIZE (16)

/*private data ����*/
#define CA_PRIVATEDATA_LEN (16)

/*CA �ϴ�Ӧ�õ���Ϣ���*/
typedef enum _CaEvent
{
    CA_ALARM,
    CA_INFO,
    CA_COMMAND,
    CA_STATUS
}CaEvent_E;

typedef enum _CaInfo
{
    /*����CA_MENU_SERVICE_STATUS����ʱ����ȥ����Ϣ,dwParm2 = (DWORD)&CaServiceInfo_S*/
    CA_INFO_SERVICE_STATUS,

    /*����CA_MENU_LOADER_STATUS����ʱ����ȥ����Ϣ,dwParm2 = (DWORD)&CaLoaderState_S*/
    CA_INFO_LOADER_STATUS,

    /*����CA_MENU_PRODUCT_STATUS����ʱ����ȥ����Ϣ,dwParm2 = (DWORD)&CaProductList_S*/
    CA_INFO_PRODUCT_STATUS,

    /*����CA_MENU_CLIENT_STATUS����ʱ����ȥ����Ϣ,dwParm2 = (DWORD)&CaClientInfo_S*/
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

     /*����ȥ������Ƶ����Ϣ,dwParm2 = (DWORD)&T_OtaCfg */
    CA_COMMAND_OTA_NEXT, 

     /*�Ƿ�����˵�����,dwParm2 = TRUE/FALSE,TRUEʱ���������˵�,FALSEʱ�û������˵� */
    CA_COMMAND_OTA_DETECT,             

     /*Attributed_Display ��Ϣ,�����ṹ��dwParm2 = (DWORD)&CaAttribCovert_S */
    CA_COMMAND_ATTRIB, 

     /*TEXT MESSAGES,dwParm2 = (DWORD)&CaMsg_S*/
    CA_COMMAND_MAIL,

     /*����CSCAEraseCAFlash()�ӿڣ�ͬʱ�ָ��������� */
    CA_COMMAND_RESETTOFACTORY,

     /*����Service Status �����ECM\EMM Monitor��Ϣ, dwParm2 = (DWORD)&CaECMEMMMonitor_S */
    CA_COMMAND_MONITOR,

     /*����Service Status �����EMM\DES ״̬,dwParm2 = (DWORD)&CaServiceStatus_S*/	 
    CA_COMMAND_EMMDES_STATUS,

     /*����Service Status �����ECM״̬,dwParm2 =  (DWORD)&CaECMStatus_S */
    CA_COMMAND_ECM_STATUS,

      /*ǿ����̨��һ��ָ����Ŀ, dwParm2 = (DWORD)&CaIRDServiceInfo_S */
    CA_COMMAND_FORCE_TUNE_TO_SRV,

     /*����pin��, dwParm2 = (DWORD)&(CaPin_S) */
    CA_COMMAND_INSTALLER_PINCODE,

     /*SHTUNE���� ��������+�Զ����� dwParm2 = (DWORD)&T_DeliverySystem */
    CA_COMMAND_HOMECHANNEL_SCAN,

     /*�ı�PIN ������,dwParm2 = (DWORD)&CaPin_S */
    CA_COMMAND_CHANGE_PIN,
	
    CA_COMMAND_MAX
}CaCommand_E;

typedef enum _CaStatus
{
     /*��ʾ��ǰ��OTA������������,�����ṹ��dwParm2 = (DWORD)&T_OtaCfg*/
    CA_STATUS_OTA_OK, 
    
     /*��ʾ��ǰOTA�����׶�,�����ṹ��dwParm2 = (DWORD)&CaOtaStatus_S*/ 
    CA_STATUS_OTA_STATUS,
    
     /*��ʾOTA����ʧ�ܼ�ʧ��ԭ��,�����ṹ��dwParm2 = CaOtaError_E*/
    CA_STATUS_OTA_FAILURE,
    
    CA_STATUS_MAX
}CaStatus_E;

/*ALARM��Ϣ״̬������Ϣ*/
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

/*������Ƶ����Ϣ,����E_SrvEsType��1\2\3��Ҫ��ʾ��ʾ��Ϣ,�ص���Ӧ������Ƶ;
������Ƶ����Ϣ,����Ӧ������Ƶ,ֻ��E_SrvEsType��3������Ļ��Ϣ*/
typedef enum _CaSrvEsType
{
    CA_UNKNOWN,
    CA_AUDIO, /*��Ƶ*/
    CA_VIDEO, /*��Ƶ*/
    CA_AV,    /*����Ƶ*/
    CA_SUBTITLE,  /*SUBTITLE*/    
    CA_CRYPT_MAX = 0xff
} CaSrvEsType_E;


/*ird ����ǿ����̨��һ����Ŀ��3Ҫ�ؽṹ*/
typedef struct _CaIRDServiceInfo
{
	WORD m_wOrigNetworkID;//ԭʼ����ID
	WORD m_wTranStreamID;//������ID
	WORD m_wServiceID;//��ĿID
} CaIRDServiceInfo_S;

typedef struct _CaSrvErrorInfo
{
    BYTE m_aucStatus[CA_STATUS_STRING_SIZE+1];//״̬��,�ַ���
    CaMsgText_E m_eTextnum;                    /*״̬������Ϣ����*/
} CaSrvErrorInfo_S;

typedef struct _CaErrorInfo
{
    INT m_nIndex;                              /*��Ŀ������*/
    BYTE m_aucStatus[CA_STATUS_STRING_SIZE+1];//״̬��,�ַ���
    CaMsgText_E m_eTextnum;                    /*״̬������Ϣ����*/
    CaSrvEsType_E m_eSerEsType;                /*��Ҫ���ص�����Ƶ����*/
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

/*CA�˵���*/
typedef enum _CaStatusMenu
{
    CA_MENU_SERVICE_STATUS,
    CA_MENU_LOADER_STATUS,
    CA_MENU_PRODUCT_STATUS,
    CA_MENU_CLIENT_STATUS,
    
    CA_MENU_MAX
}CaStatusMenu_E;

/*EM_MENU_LOADER_STATUS�˵�������Ϣ*/
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
    BYTE m_aucFirmVersion[CA_MAX_SIZE_FIRMWARE+1]; /*����������Ӧ�ø�����Ŀʵ�������ʾThe manufacturer downloaded image/application version*/
    BYTE m_aucLoadVersion[CA_MAX_SIZE_LOASER+1];   /*�ַ���*/
    BYTE m_aucSerialNO[CA_MAX_SIZE_IRD_SERIALNUMBER+1];/*�ַ���*/
    WORD m_wSpecInfo;
    CaDateTime_S m_stSignDateTime;
    CaDateTime_S m_stDownloadDateTime;
}CaLoaderState_S;

typedef enum _CaServiceType
{
    CA_SERVICE_EMM,  /*Service�˵�����ʾ����ΪEMM service*/
    CA_SERVICE_DES,  /*Service�˵�����ʾ����ΪDescramble Service*/

    CA_SERVICE_MAX
}CaServiceType_E;

/*ÿ��stream�����ES����*/
#define CA_MAXES_PERSTREAM (16)
#define CA_STREAM_STATUS_LENGTH (16)//״̬��Ϣ�ĳ���

typedef struct _CaStreamStatus
{
    BOOL m_bIsECM;                                  /*�Ƿ�ECM ,TRUE:ECM,  FALSE:EMM*/
    WORD m_wCAPID;                                  /*ECM\EMM PID*/
    BOOL m_bCanRecord;                              /*Whether the service can be recorded*/
    BYTE m_ucstreamStatus[CA_STREAM_STATUS_LENGTH+1];//stream status��ʾ״̬��
    DWORD m_dwESCount;                              /*��m_bIsECM ΪTURE���ʾ��ECM ��Ӧ��ES����*/
    WORD m_wESPID[CA_MAXES_PERSTREAM];              /*��m_bIsECM ΪTURE���ʾ��ECM ��Ӧ��ES��PID*/
}CaStreamStatus_S;

#define CA_MAX_STREAMNUMBER_PERHANDLE (5)

typedef struct _CaServiceInfo
{
    DWORD m_dwClientHandle;            /*service handle*/
    CaServiceType_E  m_eServiceType;   /*service type*/
    CaSrvErrorInfo_S m_stServiceStatus;/*service status��ʾ״̬���������Ϣ*/
    DWORD m_dwStreamCount;             /*service ������stream����*/
    CaStreamStatus_S m_stStreamStatus[CA_MAX_STREAMNUMBER_PERHANDLE];/*ÿ��stream����ϸ��Ϣ*/
}CaServiceInfo_S;

/*EM_MENU_SERVICE_STATUS�˵�������Ϣ*/
typedef struct _CaSvrInfoList
{
    BYTE m_ucServiceCount;                                   /*service ����*/
    CaServiceInfo_S m_stServiceInfo[CA_DEMUX_MAX+CA_SRV_MAX];/*ÿ��service ��ϸ��Ϣ*/
} CaSvrInfoList_S;	

 /*Product ID����*/
#define CA_PRODUCT_ID_LENGTH (2)

typedef struct _CaProductInfo
{
    BYTE m_ucSectorNum;                       /*��10������ʾ*/
    BOOL m_bEntitled;                         /*��ʾTRUE��FALSE*/
    BYTE m_aucProductId[CA_PRODUCT_ID_LENGTH];/*����λ16��������ʽ��ʾ*/
    CaDateTime_S  m_ststartDate;              /*��2010/01/15��ʽ��ʾ*/
    BYTE m_ucduration;                        /*��10������ʾ*/
    DWORD m_dwproductType;                    /*0 Normal Product, 1 PVR Product, 2 Push VOD Product, 3 Pre Enablement Product.*/
}CaProductInfo_S;

/*Product ������*/
#define CA_MAX_PRODUCT_COUNT (400)

/*EM_MENU_PRODUCT_STATUS�˵�������Ϣ*/
typedef struct _CaProductList
{
    WORD m_wProductCount;                                 /*����*/
    CaProductInfo_S m_stProductInfo[CA_MAX_PRODUCT_COUNT];/*ÿ��Product��ϸ��Ϣ*/
}CaProductList_S;

#define CA_IUC_GENERIC_LENGTH (512)

typedef struct _CaTMSData
{
    WORD m_wMsgLength;                        /*TMS ��Ϣ����*/
    BYTE  m_aucTmsData[CA_IUC_GENERIC_LENGTH];/*16������ʾ*/
}CaTMSData_S;

/*EM_MENU_CLIENT_STATUS �˵�������Ϣ*/
typedef struct _CaClientInfo
{
    BYTE m_aucIucVersion[CA_IUC_GENERIC_LENGTH+1];/*CA�ں˰汾�ţ��ַ���(����:version 2.5.0-1/1.0)*/
    DWORD m_dwSN;                                 /*Serial Number of the secure client��10������ʾ(����:4080000000)*/
    CaTMSData_S m_stTmsData;                      /*TMS data available on the persistent storage (����:00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF)*/
    BYTE m_aucCapability[CA_IUC_GENERIC_LENGTH+1];/*�ַ���(����:TMS: Y Fingerprint: Y Copycontrol: Y Macrovision: Y CI+: Y ..�˴�ʡ��...PRODUCTFILTER: Y)*/
    BYTE m_ucSecureTye;                           /*Ϊ0ʱ��ʾ"Secure Chipset" ,Ϊ1ʱ��ʾ "Security ID".*/
    WORD m_wLocKId;                               /*Lock ID/Manufacturer ID.16������ʾ(����:23)*/
    DWORD m_dwCaSystemId;                         /*16������ʾ(����:0x624)*/
    DWORD m_dwEcmCount;                           /*Count of ECMS received after start up*/
    DWORD m_dwEmmCount;                           /*Count of EMMs  received after start up*/
    DWORD m_dwCSSN;                               /*Secure chipset ID��16������ʾ(����:AABBCCDD)*/
    BYTE m_aucBuildInfo[CA_IUC_GENERIC_LENGTH+1]; /*CA�ں˿������Ϣ���ַ���(����:Built 2012-02-21 15:35)*/
    BYTE m_aucNationality[CA_IUC_GENERIC_LENGTH+1];/*һ����3����ĸ���ַ����(����:USA)*/
    BYTE m_aucClientID[CA_IUC_GENERIC_LENGTH+1];  /*client's serial number,�ַ���(����:4080000000(8))*/
}CaClientInfo_S;

/*CA_COMMAND_MONITOR��Ϣ������Ϣ������Service status�����Monitor��Ϣ*/
#define CA_MAX_MONITORMSG_LEN (256)

typedef struct _CaECMEMMMonitor
{
    DWORD m_dwClientHandle;                       /*monitor��Ϣ��Ӧ��Service��Handle*/
    BYTE m_ucMonitorMsg[CA_MAX_MONITORMSG_LEN+1]; /*�ַ���*/
}CaECMEMMMonitor_S;

/*EM_CA_INFO_EMMDES_STATUS��Ϣ������Ϣ������Service status�����Service Status��Ϣ*/
typedef struct _CaServiceStatus
{
    DWORD m_dwClientHandle;                    /*״̬�����仯��Service��Handle*/
    BYTE m_aucStatus[CA_STATUS_STRING_SIZE+1];//״̬��
    CaMsgText_E m_eTextnum;                    /*״̬��Ϣ����*/
}CaServiceStatus_S;

/*EM_CA_INFO_ECM_STATUS��Ϣ�����ṹ�壬����Service status�����ECM״̬��Ϣ*/
typedef struct _CaECMStatus
{
    BYTE m_aucStatus[CA_STATUS_STRING_SIZE+1];//״̬��
    WORD m_wECMPID;                              /*״̬�����仯��ECM ��PID*/
}CaECMStatus_S;

/*TEXT Messsge ������󳤶�*/   
#define CA_MAX_SIZE_TXT_MAIL (511) 

/*TEXT Messsge����*/
typedef enum _CaTxtType
{
    CA_TXT_TYPE_MAIL,     /*Text -- Mailbox*/
    CA_TXT_TYPE_ANNOUNCE, /*Text -- Announcement*/
    CA_TXT_TYPE_CLUBNUM,  /*Text --Club Numbers(�ݲ�֧��)*/

    CA_TXT_TYPE_MAX
}CaTxtType_E;

/*TEXT Messsge��ʾ��ʽ*/
typedef enum _CaTxtShowMode
{
    CA_TXT_SHOW_NORMAL,  /*Normal����ʾ�ʼ�ͼ�꣬������*/
    CA_TXT_SHOW_TIMED,	 /*Timed(automatic erase after date/time)*/
    CA_TXT_SHOW_FORCED,  /*Forced Display������Ļ�м䵯��*/

    CA_TXT_SHOW_MAX
}CaTxtShowMode_E;

/*TEXT Messsge����,forced mail /announcement ������ʾ���ݲ���ʾ�ʼ�ͼ�꣬normal mail ֻ��ʾ�ʼ�ͼ�꣬û��normal announcement*/
typedef struct _CaMsg
{ 
    CaTxtType_E m_eType; 
    CaTxtShowMode_E m_eClass;                 /*��m_eTypeΪ 0: Text_Mail ��1: Text_Announcementʱ��Ч*/
    BYTE m_ucPriority;                        /*���ȼ�*/
    WORD m_wLen;                              /*TEXT Message����*/
    CaDateTime_S  m_stDeleteTime;             /*ɾ��ʱ��,��m_cClassֵΪ1 ʱ��Ч*/
    BYTE m_aucContent[CA_MAX_SIZE_TXT_MAIL+1];/*TEXT Message����,�ַ���*/
}CaMsg_S;

/*Attributed_Display��Ϣ������ֽ���*/
#define MAX_ATTRIBDATA_LEN (511)

/*Attributed_Display��Ϣ����*/
typedef enum _CaTmsType
{
    CA_TMS_TYPE_NORMALMAIL ,      /*Normal*/
    CA_TMS_TYPE_FORCEDMAIL ,      /*Forced Text*/
    CA_TMS_TYPE_FINGERPRT ,       /*Fingerprint*/
    CA_TMS_TYPE_FINGERPRTOPTION,  /*Fingerprinting options*/
    
    CA_TMS_TYPE_MAX
}CaTmsType_E;

/*Attributed_Display��Ϣ
IucAttribCovert_S:m_eType == 0 ��ͬ�� IucCaMsg_S: m_eType == 0 && m_eClass == 0;ֻ��ʾ�ʼ�ͼ��
IucAttribCovert_S:m_eType == 1 ��ͬ�� IucCaMsg_S: m_eClass == 2;��Ҫ������ʾ,����ʾ�ʼ�ͼ��
IucAttribCovert_S:m_eType == 1 || m_eType == 2  �����밴��Ҫ����ʾ(flash ,banner,covert,duration);
*/
typedef struct _CaAttribCovert
{
    WORD m_wMsgLength;                          /*Attributed_Display ��Ϣ����*/
    BYTE m_aucMessageData[MAX_ATTRIBDATA_LEN+1];/*Attributed_Display ��Ϣ����,�ַ�������*/
    CaTmsType_E m_eType;
    
    //�����ĸ�����ֻ��forced text �� fingerprint ��Ч*/
    BOOL m_bFlash;     /*�Ƿ���˸,����˸ΪTRUEʱ,����Overted ָ������һ�����˸����.����Covertָ��,��Ҫ��ʾֻ��Ҫ��ʾ����֡����*/
    BOOL m_bBanner;    /*ΪTRUEʱ,��Ҫ��ʾ��Ϣ ��������, ������Ҫ������,ĿǰIrdeto�Ա���û��Ҫ��*/
    BYTE m_ucCoverPer; /*������,forced text ʱ����Ϣ��ռ������ʾ��Ļ������ٷֱ�, fingerprintʱ��ʾָ�ƿ��ռ��Ļ��ȵİٷֱȣ�ָ�Ƹ߶��ǿ�ȳ���ָ�Ƹ���*/
    WORD m_wDuration;  /*��ʱ����,��λ��.Ϊ0 ʱFingerprintһֱ��ʾ����һ��Fingerprint������Forced Text��ʾ���û�ȡ����ʾ*/

    BOOL m_bCovert;    /*������m_bTypeΪָ��ʱ,m_bCovert��ʾָ������,ΪTRUEʱΪCovert ָ��. ΪFALSEʱΪOvertָ��*/

    /*----����EOF���� �����Ķ���ṹ��Ϣ----*/
    BYTE m_ucLocationX; //������λ��
    BYTE m_ucLocationY; //������λ��
    BYTE m_ucBgTransAlpha; // ����͸����
    DWORD m_dwBgColour; //������ɫ
    BYTE m_ucFontTransAlpha; // ����͸����
    DWORD m_dwFontColour; //������ɫ
    BYTE m_ucFontType; //��������
}CaAttribCovert_S;

/*Irdeto CA PIN ��*/
typedef struct  _CaPin
{
    BYTE* m_pucPin; //PIN ��
    BYTE m_ucPinLen;//PIN �볤��
}CaPin_S;

/*���浽������ʼ��ṹ��Ϣ*/
typedef struct _CaMail		
{
    BOOL  m_bReadFlag;          /*�Ѷ���־*/
    CaDateTime_S m_stArriveTime;/*�յ��ʼ�ʱ��*/
    CaMsg_S m_stMail;           /*��Ϣ����*/
} CaMail_S;

/*OTA���������׶�,���ý׶λ��ж�����ʣ��ʱ��ͻᳬʱ.
Stepֵ:
0  ��������NIT��
1  ��������PAT��
2  ��������PMT��
3  ��������PID
*/
typedef struct _CaOtaStatus
{
    BYTE m_ucStep;     /*OTA�׶�*/
    BYTE m_ucTimeLeft; /*ʣ��ʱ��*/
}CaOtaStatus_S;

/* OTA ʧ������*/
typedef enum _CaOtaError
{
    CA_OTA_ERROR_1,  /*NIT��û��������*/
    CA_OTA_ERROR_2,  /*PAT,PMT ������PIDû��������*/

    CA_OTA_ERROR_MAX
}CaOtaError_E;

/*EMM ǿ����������*/
typedef struct _CaEmmOta
{
    BOOL m_bDownLoadAllowed; /*�Ƿ���������*/
    BOOL m_bForced;          /*�Ƿ�Ϊǿ��*/
}CaEmmOta_S;

/*�ʼ�����ķ�ʽ*/
typedef enum _CaEmailSortStyle 					    
{
    CA_EMAIL_TIME_DESC,	/*����ʱ��������ں���*/	
    CA_EMAIL_TIME_ASC,/*����ʱ���������ǰ��*/	
    CA_EMAIL_SORT_MAX
}CaEmailSortStyle_E;

/**************************************************************************
*Function Name: CSCAGetCACompileTime
*��ȡCA�汾��Ϣ
*
*Parameters:
*    pacVerBuf[out]: �洢CA Porting�����ʱ��Ŀռ��ַ
*    dwBufSize[in]: �洢CA Porting�����ʱ��Ŀռ䳤��
*
*Return Values
*    0: SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAGetCACompileTime(char* pcComTimeBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name:CSCATXTDel
* ɾ��ĳһ���ʼ�
*
*Parameters:
*    nIndex[in]: �ʼ��±꣬��0��ʼ
*
*Return Values:
*	0: SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCATXTDel(DWORD dwIndex);

/**************************************************************************
*Function Name:CSCATXTDelAll
* ɾ������������ʼ�
*
*Parameters:
*    ��
*
*Return Values:
*    0: SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCATXTDelAll(void);

/**************************************************************************
*Function Name:CSCATXTGetNum
* ��ȡ�����е����ʼ���
*
*Parameters:
*	��
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
************************************************************************/
INT CSCATXTGetNum(void);

/**************************************************************************
*Function Name:CSCATXTGetInfo
* ��ȡĳһ���ʼ�������
*
*Parameters:
*	nIndex[in]: �ʼ��±꣬��0��ʼ
*	ppstTxt[out]: �����ʼ��Ĵ洢��ַ
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCATXTGetInfo(DWORD dwIndex, CaMail_S** ppstTxt);

/**************************************************************************
*Function Name:CSCATXTHaveNew
* ��ѯ�������Ƿ���δ���ʼ�
*
*Parameters:
*	��
*
*Return Values:
*	1: TRUE
*	0: FALSE
**************************************************************************/
BOOL CSCATXTHaveNew(void);

/**************************************************************************
*Function Name:CSCATXTHaveNewMailNum
* ��ȡ������δ���ʼ�����
*
*Parameters:
*	��
*
*Return Values:
*	����δ���ʼ�����
**************************************************************************/
INT CSCATXTHaveNewMailNum(void);

/**************************************************************************
*Function Name:CSCATxtSetReadFlag
* �����ʼ��Ƿ��Ѷ�
*
*Parameters:
*	nIndex[in]: �ʼ���������0��ʼ
*	bRead[in]: �Ѷ�(TRUE),δ��(FALSE)
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCATXTSetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name:CSCATXTSort
* �ʼ�����
*
*Parameters:
*	emStyle[in]: ����ʽ ��CaEmailSortStyle_E��˵��
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCATXTSort(CaEmailSortStyle_E emStyle);

/**************************************************************************
*Function Name:CSCAOTAGetTunerParm
* ��ȡ����ʱ�����Ƶ�������ͨ��CA_COMMAND_OTA_NEXT��Ϣ�첽���
*
*Parameters:
*	��
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCAOTAGetTunerParm(void);

/**************************************************************************
*Function Name:CSCAOTAGetUpdateInfo
* ��ȡ�������ݵ���Ϣ��ͨ��CA_STATUS_OTA_OK��Ϣ�첽���
*
*Parameters:
*	��
*
*Return Values:
*	0 : SUCCESS
*	-1: FAILURE
**************************************************************************/
INT CSCAOTAGetUpdateInfo(void);

/**************************************************************************
*Function Name:CSCAOTASaveCfg
* ��������Ƶ���������Ϣ
*
*Parameters:
*    pstOtaCfg[in]: �������Ƶ���������Ϣ
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT  CSCAOTASaveCfg(T_OtaCfg* pstOtaCfg);

/**************************************************************************
*Function Name: CSCAEnterIrdetoMenu
*APP����CA�˵��������ݣ��ֱ�ͨ�����Ե�INFO��Ϣ�첽���
*
*Parameters:
*    eQueryDataType[in]: ����˵�����
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAEnterIrdetoMenu( CaStatusMenu_E eQueryDataType);

/**************************************************************************
*Function Name: CSCAEraseCAFlash
*����CA��Ӧ��FLASH���������ʼ����򣬽��ָ������˵����յ�
*CA_COMMAND_RESETTOFACTORY����ʱ���øýӿڣ� ͬʱ�������ָ������Ĳ���
*
*Parameters:
*    ��
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAEraseCAFlash(void);

/**************************************************************************
*Function Name: CSCAResetCAData
* ����CA�ںˣ����ָ������˵�ʱ���øýӿ�
* ͬʱ�������ָ������Ĳ���
*
*Parameters:
*    ��
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAResetCAData(void);

/**************************************************************************
*Function Name: CSCAMonitorControl
*  �򿪻�ر�Monitor��Ϣ��ʾ
*
*Parameters:
*	dwServiceHandle[in]: servicehandle, ������ServiceStatus����ʱ��õ�IucSvrInfoList_S�ṹ����Ϣ�е�m_dwClientHandle
*  	ucIsEnable[in]: ����servicehandle��Ӧ�˵�ʱTRUE,��;�˳�servicehandle��Ӧ�˵�ʱFALSE,�ر�
*
*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
INT CSCAMonitorControl( DWORD dwServiceHandle, BOOL bIsEnable);
/**************************************************************************
*Function Name: CSCAPIDChange
* �л���Ƶ�򲥷�subtitle
*
*Parameters:
*	hService[in]:  ����CSCAStartService���ص�HCSHANDLE
       eStreamType[in]: ��Ƶ�л���CA_AUDIO, ����subtitle���Ŵ�CA_SUBTITLE
*
*Return Values:
*    ��
**************************************************************************/
void CSCAPIDChange(HCSHANDLE hService,CaSrvEsType_E eStreamType);

/**************************************************************************
*Function Name: CSCAGetPrivateData
* ���STB��PrivateData��ÿ������Ψһ����Ӧ��ʵ�ָú���
*
*Parameters:
*    pucPrivateData[in]: ���PrivateData��Buffer��ַ
*    pdwDataLen[in]: ���PrivateData��Buffer��С
*			  [out]: PrivateData��ʵ�ʳ���

*Return Values:
*    0 : SUCCESS
*    -1: FAILURE
**************************************************************************/
extern INT CSCAGetPrivateData(BYTE* pucPrivateData, DWORD* pdwDataLen);

/**************************************************************************
*Function Name: CSCAGetFTAType
* ��ȡ�������źŴ������ͣ���Ӧ��ʵ�ָú���
*
*Parameters:
*    ��

*Return Values:
*    �������źŴ�������T_CaFtaType
**************************************************************************/
extern T_CaFtaType  CSCAGetFTAType(void);

/**************************************************************************
*Function Name:CsCaGetFlashAddress
* ��ȡIUC   ����������Flash ��ַ����Ӧ��ʵ�ָú���
*
*Parameters:
*    dwSize[In]: �����������Ĵ�С.
*
*Return Values:
*     һ�������������ʼ��ַ
**************************************************************************/
extern DWORD CSCAGetFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetBackupFlashAddress
*  ��ȡIUC  ������������Flash ��ַ����Ӧ��ʵ�ָú���
*
*Parameters:
*    dwSize[In]: �����������Ĵ�С
*
*Return Values:
*    һ�������������ʼ��ַ
**************************************************************************/
extern DWORD CSCAGetBackupFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetFlashMailAddress
*  ��ȡIUC  �ʼ���������Flash ��ַ����Ӧ��ʵ�ָú���
*
*Parameters:
*    dwSize[In]: ������������С
*
*Return Values:
*    һ�������������ʼ��ַ
**************************************************************************/
extern DWORD CSCAGetFlashMailAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetFlashBlockSize
*  ��ȡ��ʹ�õ�Flash һ��Block�Ĵ�С����λByte����Ӧ��ʵ�ָú���
*
*Parameters:
*    ��
*
*Return Values:
*    ��ʹ�õ�Flash һ��Block�Ĵ�С����λByte
**************************************************************************/
extern DWORD CSCAGetFlashBlockSize(void);

/**************************************************************************
* Function Name: CSCAGetEpgTime
* ��ȡSTB ��ǰEPG ʱ��. ��Ӧ��ʵ�ָú���
*
*Parameters:
*    pstDataTime[out]: ���STB ��ǰEPG  ʱ��
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

