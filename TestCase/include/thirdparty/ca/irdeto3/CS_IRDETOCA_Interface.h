/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_IRDETO_Interface.h  
* Author:      Xiejianghua
* Date:        Mar 2006
* Description: IRDETO CA interface
*            
*****************************************************************************************************************************/
#include "mmcp_typedef.h"
#include "CS_IRDETOCA_Loader.h"
#include "cs_cainterface_pvrext.h"
#include "cs_cainterface.h"

/****************************************************************************************************************************
* 			                Revision History                                                                                *
*****************************************************************************************************************************
* - Revision  : 1.0  2006/3/22
*   Programmer: Xie jianghua
*   Create
*   Modify by : Wan suanlin
* - Revision  : 1.1  2007/4/12
*   Modify by : Pengshungquan
*****************************************************************************************************************************/
#ifndef _CS_IRDETO_INTERFACE_H_
#define _CS_IRDETO_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*for Irdetoca Smartcard Status*/
#define     MAX_SC_NUMBER       2 //菜单中最多可以显示的智能卡数
#define     SC_NUMBER_LEN       16 //智能卡号最大长度
#define     SC_TYPE_LEN         8 //智能卡类型的描述字节的最大长度
#define     SC_VER_LEN          8 //智能卡版本描述字节的最大长度
#define     SC_NATION_LEN       8 //智能卡国家码描述字节的最大长度
#define     SOURCE_SMARTCARD    4 // Error 信息中的Source 属性的一个取值,表示该错误信息源于智能卡
#define     SOURCE_APPLICATION  32 // Error 信息中的Source 属性的一个取值,表示该错误信息源于应用

#define     SOURCE_MOD_EMM_SOURCE 6
#define     SOURCE_MOD_ECM_SOURCE 9
								   
#define     SOURCE_EMM_SOURCE 2// Error 信息中的Source 属性的一个取值,表示该错误信息源于EMM
#define     SOURCE_ECM_SOURCE 1// Error 信息中的Source 属性的一个取值,表示该错误信息源于ECM
								   // 如:E16-4 Severice is not available. "4"表示该错误源于智能卡							

#define 	CA_DEMUX_MAX  2 // 可支持的Demux最大数
#define 	CA_SRV_MAX 		3//多路解扰的可支持的节目最大数
#define     CA_IPPV_EVENT_MAX    32//IPPV 中可以支持的EVENT最大个数

/* Error 信息中的Severity 属性的取值,表示该错误类型
当应用收到Error信息时,要将收到的Severity值转化为相应的字母显示.
1: 'F' 2: 'E' 3: 'W' 4: 'I' 5: 'D'
*/
typedef enum _STATUS_SEVERITY_EN
{
	IRDETO_STATUS_FATAL = 1,             
	IRDETO_STATUS_ERROR = 2,             
	IRDETO_STATUS_WARNING = 3,           
	IRDETO_STATUS_INFO = 4,             
	IRDETO_STATUS_INFO_DND = 5          
} IRDETO_STATUS_SEVERITY_EN;

/* Error 信息中的Textnum 属性的取值,表示该错误需要显示的文本信息
当应用收到Error信息时,要将收到的m_wTextnum值转化为相应注释的文本信息.
*/
typedef enum _BAKMSG_TEXT_EN
{
    CA_BAKMSG_TEXT000 = 0 ,           /*Service not scrambled*/                                             
    CA_BAKMSG_TEXT001 = 1 ,           /*Please insert CA Module*/                                           
    CA_BAKMSG_TEXT002 = 2 ,           /*CA Module EEPROM Failure*/                                          
    CA_BAKMSG_TEXT003 = 3 ,           /*CA Module Failure*/                                                 
    CA_BAKMSG_TEXT004 = 4 ,            /*Please insert Smartcard*/                                          
    CA_BAKMSG_TEXT005 = 5 ,            /*Unknown Smartcard*/                                                
    CA_BAKMSG_TEXT006 = 6 ,            /*Smartcard Failure*/                                                
    CA_BAKMSG_TEXT007 = 7 ,           /*Service is currently scrambled*/                                    
    CA_BAKMSG_TEXT008 = 8 ,             /*Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT009 = 9 ,            /* Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT010 = 10,             /*Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT011 = 11 ,             /*Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT012 = 12 ,             /*Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT013 = 13 ,             /*Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT014 = 14 ,            /* Preview Period Expired*/                                          
    CA_BAKMSG_TEXT015 = 15 ,            /* No further purchases possible*/                                   
    CA_BAKMSG_TEXT016 = 16 ,            /* Credit Limit Reached*/                                            
    CA_BAKMSG_TEXT017 = 17 ,             /*Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT018 = 18 ,             /*Viewing is temporarily blocked,please stay tuned*/                
    CA_BAKMSG_TEXT019 = 19 ,             /*Invalid smartcard*/                                               
    CA_BAKMSG_TEXT020 = 20 ,           /*Service is cureently scrambled*/                                    
    CA_BAKMSG_TEXT021 = 21 ,           /*Service is currently scrambled*/                                    
    CA_BAKMSG_TEXT022 = 22 ,           /*Smartcard Not Compatible*/                                          
    CA_BAKMSG_TEXT023 = 23 ,           /*Service Unknow*/                                                    
    CA_BAKMSG_TEXT024 = 24 ,           /*Service is not currently running */                                 
    CA_BAKMSG_TEXT025 = 25 ,           /*Locating Serivce*/                                                  
    CA_BAKMSG_TEXT026 = 26 ,           /*Decoder Memory Full*/                                               
    CA_BAKMSG_TEXT027 = 27 ,           /* Service is not currently available*/                               
    CA_BAKMSG_TEXT028 = 28 ,           /*Parental Control Lock and/or Ask for Parental PIN*/                 
    CA_BAKMSG_TEXT029 = 29 ,           /*Not allowed in this country*/                                       
    CA_BAKMSG_TEXT030 = 30 ,           /*No Event Information*/                                              
    CA_BAKMSG_TEXT031 = 31 ,           /*Service not allowed*/                                               
    CA_BAKMSG_TEXT032 = 32 ,           /*Reading Satellite Information*/                                     
    CA_BAKMSG_TEXT033 = 33 ,           /*Bouquet Block*/                                                     
    CA_BAKMSG_TEXT034 = 34 ,           /*No Signal or E48 Searching for signal*/                             
    CA_BAKMSG_TEXT035 = 35 ,           /*LNB Overload*/                                                      
    CA_BAKMSG_TEXT036 = 36 ,           /*No Service available*/                                              
    CA_BAKMSG_TEXT037 = 37 ,           /*Searching for signal*/                                              
    CA_BAKMSG_TEXT038 = 38 ,           /*Incorrect Pin*/                                                     
    CA_BAKMSG_TEXT039 = 39 ,           /*IPPV Ok*/                                                           
    CA_BAKMSG_TEXT040 = 40 ,           /*CAM not compatible*/                                                
    CA_BAKMSG_TEXT041 = 41 ,           /*Unknow Channel*/                                                    
    CA_BAKMSG_TEXT042 = 42 ,           /*channel not currently available*/                                   
    CA_BAKMSG_TEXT043 = 43 ,           /*Invalid tuning parametes*/                                          
    CA_BAKMSG_TEXT044 = 44 ,           /*Service is currently blocked*/                                      
    CA_BAKMSG_TEXT045 = 45 ,           /*Please insert Nagravision smart card*/                              
    CA_BAKMSG_TEXT046 = 46 ,           /*Nagravison smart card not inserted correctly*/                      
    CA_BAKMSG_TEXT047 = 47 ,           /*Free-to-air service is blocked*/                                    
    CA_BAKMSG_TEXT048 = 48 ,           /*This service is unavailable on your second TV*/                     
    CA_BAKMSG_TEXT049 = 49 ,           /*Song title and Artist information cannot be viewed*/                
    CA_BAKMSG_TEXT050 = 50 ,           /*Rescan in progress. Please wait*/                                   
    CA_BAKMSG_TEXT051 = 51 ,           /*TVLink Overload*/                                                   
    CA_BAKMSG_TEXT052 = 52 ,           /*LNB1 Overload*/                                                     
    CA_BAKMSG_TEXT053 = 53 ,           /*LNB2 Overload*/                                                     
    CA_BAKMSG_TEXT054 = 54 ,           /*TV2 Disabled. Please contact customer services*/                    
    CA_BAKMSG_TEXT055 = 55 ,             /*Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT056 = 56 ,             /*Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT057 = 57 ,           /*Service is currently scrambled*/                                    
    CA_BAKMSG_TEXT058 = 58 ,           /*Service is currently scrambled*/                                    
    CA_BAKMSG_TEXT059 = 59 ,           /* No MSK*/                                                           
    CA_BAKMSG_TEXT060 = 60 ,           /* No Subscription*/                                                  
    CA_BAKMSG_TEXT061 = 61 ,           /* Session Record Storage Failure*/                                   
    CA_BAKMSG_TEXT062 = 62 ,           /* Session Record Data Error*/                                        
    CA_BAKMSG_TEXT063 = 63 ,           /*Bad Op-code or Sequence*/                                           
    CA_BAKMSG_TEXT064 = 64 ,           /*Incomplete Definition*/                                             
    CA_BAKMSG_TEXT065 = 65 ,           /* Cannot allocate descrambler*/                                      
    CA_BAKMSG_TEXT066 = 66 ,           /*Not supported for this service*/                                    
    CA_BAKMSG_TEXT067 = 67 ,          /*PVR Encryption not available*/                                       
    CA_BAKMSG_TEXT068 = 68 ,          /*Resource unavailable*/                                               
    CA_BAKMSG_TEXT069 = 69 ,           /*Out of EMM Filters*/                                                
    CA_BAKMSG_TEXT070 = 70 ,            /* Out of ECM Filters*/                                              
    CA_BAKMSG_TEXT071 = 71 ,           /*Out of Channels*/                                                   
    CA_BAKMSG_TEXT072 = 72 ,            /* Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT073 = 73 ,            /*Service is currently scrambled*/                                   
    CA_BAKMSG_TEXT074 = 74 ,            /* Service is currently scrambled*/                                  
    CA_BAKMSG_TEXT075 = 75 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT076 = 76 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT077 = 77 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT078 = 78 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT079 = 79 ,           /*Reserved smartcard retun code*/                                     
    CA_BAKMSG_TEXT080 = 80 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT081 = 81 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT082 = 82 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT083 = 83 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT084 = 84 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT085 = 85 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT086 = 86 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT087 = 87 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT088 = 88 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT089 = 89 ,            /*Reserved smartcard retun code*/                                    
    CA_BAKMSG_TEXT090 = 90 ,            /*Service is currently scrambled*/                                   
    CA_BAKMSG_TEXT091 = 91 ,            /*Service is currently scrambled*/                                   
    CA_BAKMSG_TEXT092 = 92 ,            /*Service is currently scrambled*/
#if 1                                  
    CA_BAKMSG_TEXT093 = 93 ,           /* Unknown Ireto PVR phase*/               
    CA_BAKMSG_TEXT094 = 94 ,           /* Recording key error*/                   
    CA_BAKMSG_TEXT095 = 95 ,           /* PVR not allowed*/                       
    CA_BAKMSG_TEXT096 = 96 ,           /* Acquiring initial content rights*/      
    CA_BAKMSG_TEXT097 = 97 ,           /* Acquiring initial content rights*/      
    CA_BAKMSG_TEXT098 = 98 ,           /* Chain recordings failure*/              
    CA_BAKMSG_TEXT099 = 99 ,           /* No more playback session available*/    
    CA_BAKMSG_TEXT100 = 100 ,           /*Permanent recording is not allowed*/     
    CA_BAKMSG_TEXT101 = 101 ,           /*Playback is not allowed*/              
    CA_BAKMSG_TEXT102 = 102 ,           /*DRM authentication failure*/           
    CA_BAKMSG_TEXT103 = 103 ,           /* Invalid smartcard*/   
                    
    CA_BAKMSG_TEXT104 = 104 ,           /*PVR DRM Ok*/

    CA_BAKMSG_TEXT105 = 105 ,           /* Playback is not allowed*/                   
    CA_BAKMSG_TEXT106 = 106 ,           /* No PVR subscription*/                       
    CA_BAKMSG_TEXT107 = 107 ,           /* Outside Viewing window*/                    
    CA_BAKMSG_TEXT108 = 108 ,           /* PVR is not allowed*/                        
    CA_BAKMSG_TEXT109 = 109 ,           /* No PVR MSK*/                                
    CA_BAKMSG_TEXT110 = 110 ,           /* No more playback session available*/        
    CA_BAKMSG_TEXT111 = 111 ,           /* Number-of-playbacks limit exceeded*/        
    CA_BAKMSG_TEXT112 = 112 ,           /* Record is not allowed*/   
#endif
     /*----------------------FFFFFFF----------------------*/                 
    CA_BAKMSG_TEXT113 = 113 ,         /*Incomplete Definiton*/      
    CA_BAKMSG_TEXT114 = 114 ,         /*Format Error*/   

    /*-----------------------DDDDDDDDDD--------------------*/           
    CA_BAKMSG_TEXT115 = 115 ,            /*Card In*/                                                                           
    CA_BAKMSG_TEXT116 = 116 ,            /* Event Purchased*/                                                                 
    CA_BAKMSG_TEXT117 = 117 ,          /* Preview Period*/                                                                  
    CA_BAKMSG_TEXT118 = 118 ,          /* Event already purchased*/                                                         
    CA_BAKMSG_TEXT119 = 119 ,          /* Event is recorded*/                                                               
    CA_BAKMSG_TEXT120 = 120 ,          /*Service is currently de-scrambled*/                                               
    CA_BAKMSG_TEXT121 = 121 ,          /*Service ok*/                                                                        
    CA_BAKMSG_TEXT122 = 122 ,          /*DVB EMM Service Ok*/                                                                
    CA_BAKMSG_TEXT123 = 123 ,          /*Descrambler Service Ok*/                                                            
    CA_BAKMSG_TEXT124 = 124 ,          /*EMM source ok*/                                                                     
    CA_BAKMSG_TEXT125 = 125 ,          /*ECMSource Ok*/                                                                      
    CA_BAKMSG_TEXT126 = 126 ,          /* Descrambler Ok*/                                                                   
    CA_BAKMSG_TEXT127 = 127 ,          /*Monitoring Enabled*/                                                                
    CA_BAKMSG_TEXT128 = 128 ,          /*PVR Record Service Ok*/                                                             
    CA_BAKMSG_TEXT129 = 129 ,          /*PVR Playback Service Ok*/                                                           
    CA_BAKMSG_TEXT130 = 130 ,          /* Ok*/                                                                               
    CA_BAKMSG_TEXT131 = 131 ,          /* Ok*/                                                                               
    CA_BAKMSG_TEXT132 = 132 ,          /* Ok*/                                                                               
    CA_BAKMSG_TEXT133 = 133 ,          /*No Match*/                                                                      
    CA_BAKMSG_TEXT134 = 134 ,          /*No PMT*/                                                                        
    CA_BAKMSG_TEXT135 = 135 ,           /* Key not loaded*/                                                                   
    CA_BAKMSG_TEXT136 = 136 ,          /*NoVendor ID*/                                                                       
    CA_BAKMSG_TEXT137 = 137 ,          /*No CAT*/                                                                            
    CA_BAKMSG_TEXT138 = 138 ,          /*No Vendor ID*/                                                                      
    CA_BAKMSG_TEXT139 = 139 ,          /*Service is currently scrambled*/                                                    
    CA_BAKMSG_TEXT140 = 140 ,          /* FTA Service*/                                                                      
    CA_BAKMSG_TEXT141 = 141 ,          /* Peeking content rights*/  

    /*-----------------------iiiiiiiiiiiiiiiiiiiiiiiiiiii------------------*/                                                         
    CA_BAKMSG_TEXT142 = 142 ,          /*Checking Smartcard*/                           
    CA_BAKMSG_TEXT143 = 143 ,          /*Service is currently scrambled*/ 
    CA_BAKMSG_TEXT144 = 144 ,          /*Subscriptions are expiring very soon*/   
    CA_BAKMSG_TEXT145 = 145 ,          /*Unspecified Status*/  
    
 /*---------------------------ADD-ADD-----------------------------*/   
    CA_BAKMSG_TEXT146 = 146 ,          /*Service is currently scrambled*/   
    CA_BAKMSG_TEXT147 = 147 ,          /*Service is currently scrambled*/ 
    CA_BAKMSG_TEXT148 = 148 ,          /*Service is currently scrambled*/  

/*----------------------------CA ADD-----------------------------*/   
    CA_BAKMSG_TEXT149 = 149 ,          /*country block clear:'E43-32'*/ 
    CA_BAKMSG_TEXT150 = 150 ,          /*SOL clear:'E45-32'*/ 
    CA_BAKMSG_TEXT151 = 151 ,          /*Runningstatus clear:'E38-32'*/ 
    CA_BAKMSG_TEXT152 = 152 ,          /*Maturity Rating blocked*/ 
    CA_BAKMSG_TEXT153 = 153 ,          /*Maturity Rating PIN Error*/ 
    CA_BAKMSG_TEXT154 = 154 ,          /*Maturity Rating PIN blocked*/ 

    CA_BAKMSG_TEXTMAX
}IRDETO_BAKMSG_TEXT_EN;

/*Error信息结构体,专用于描述菜单中出现的各种资源的状态.
由四个部分组成,m_bSeverity和m_wTextnum两个属性要根据IRDETO_STATUS_SEVERITY_EN
和IRDETO_BAKMSG_TEXT_EN找到相应的文本显示.m_wStatus和m_bSource可直接显示收到的值.
如收到结构体值为{2,16,4,8}则显示文本应为:E16-4 Severice is not available.
*/
typedef struct _SRVERRINFO_ST
{
	BYTE    m_bSeverity;
	WORD    m_wStatus;
	BYTE    m_bSource;
	WORD    m_wTextnum;
} IRDETO_SRVERRINFO_ST;

/*Error信息结构体,专用于描述Alarm消息.
m_bSeverity和m_wTextnum两个属性要根据IRDETO_STATUS_SEVERITY_EN
和IRDETO_BAKMSG_TEXT_EN找到相应的文本显示.m_wStatus和m_bSource可直接显示收到的值.
如收到结构体值为{2,16,4,8}则显示文本应为:E16-4 Severice is not available.
*/
typedef struct _CA_ERROR_INFO_ST
{
    DWORD   m_wServiceHandle;//需要显示Alarm消息节目的ServiceHandle
	BYTE    m_bSeverity;
	WORD    m_wStatus;
	BYTE    m_bSource;
    WORD    m_wTextnum;	
}IRDETO_CA_ERROR_INFO_ST;

/*智能卡菜单中各项智能卡的属性*/
typedef struct _SMARTCARDSTATUS_ST
{
    IRDETO_SRVERRINFO_ST m_stScStatus;//智能卡当前状态
    BYTE    m_szScNumber[SC_NUMBER_LEN];//智能卡卡号,字符串数组 
    BYTE    m_szScType[SC_TYPE_LEN];//智能卡类型,字符串数组  
    BYTE    m_szScVer[SC_VER_LEN];//智能卡版本,字符串数组 
    BYTE    m_szNationality[SC_NATION_LEN];//智能卡国家码,字符串数组 
    WORD    m_wPatchLevel;//智能卡补丁级别
    WORD    m_wOwnerId;//智能卡OwnerID
    BYTE    m_bVariant;//智能卡市场号
    BYTE    m_bBuild;//智能卡Build
}IRDETO_SMARTCARDSTATUS_ST;


/*Irdeto CA 所有的菜单类型*/
typedef enum _MENU_STATUS_EN
{
	CA_MENU_SOFTCELL_STATUS,//  CA主菜单         
	CA_MENU_SMARTCARD_STATUS,//智能卡菜单 
	CA_MENU_DVBSERVICE_STATUS,//DVB节目汇总菜单
	CA_MENU_SERVICEINDEX_STATUS,//	DVB单个节目信息菜单
	CA_MENU_EMM_SERVICE_STATUS, //EMM 信息菜单,暂时不需要实现
	CA_MENU_DES_SERVICE_STATUS,//ECM信息菜单 ,暂时不需要实现
	CA_MENU_LOADER_STATUS,//Loader 菜单
	CA_MENU_COMPONENTS_STATUS,//组件菜单,暂时不需要实现
	CA_MENU_MR_CARD_STATUS,//进入MR智能卡信息菜单
	CA_MENU_MAX_DATA
}IRDETO_MENU_STATUS_EN;

/*Irdeto CA 上传应用的消息类别*/
typedef enum _CAEEVENTGROUP_EN
{
	CA_ALARM,
	CA_INFO,
	CA_COMMAND,
	CA_STATUS
}IRDETO_CAEEVENTGROUP_EN;

/*CA_STATUS 消息的所有类型*/
typedef enum _CASTATUS_EN
{
	CA_STATUS_OTA_OK,        //表示当前有OTA升级码流存在,所带结构体dwParm2 = (DWORD)&T_OtaCfg
	CA_STATUS_OTA_STATUS,    //表示当前所处阶段,所带结构体dwParm2 = (DWORD)&IRDETO_OTASTATUS_ST
	CA_STATUS_OTA_FAILURE,   //表示OTA升级失败,所带结构体dwParm2 = (DWORD)&IRDETO_OTAERROR_EN
	CA_STATUS_SMC_IN,		// 插入智能卡, 而且该卡为正确的Irdeto卡
	CA_STATUS_SMC_OUT,		// 拔出智能卡
	CA_STATUS_SMC_ERROR		//非法智能卡
	
}IRDETO_CASTATUS_EN;

/*CA_INFO 消息的所有类型*/
typedef enum _CAINFO_EN
{
	CA_INFO_SOFTCELL_STATUS = 0, //CA 主菜单信息,所带结构体ddwParm2 = (DWORD)&IRDETO_CASTATUS_ST 
	CA_INFO_SC_STATUS,           //智能卡菜单信息,所带结构体ddwParm2 = (DWORD)&IRDETO_SMARTCARDSTATUS_ST 
	CA_INFO_MONI_EMM_STATUS = 2, //单个节目菜单信息中的EMM监听信息,所带结构体ddwParm2 = (DWORD)&IRDETO_MONIEMMSTATUS_ST 
	CA_INFO_MONI_DES_STATUS ,    //单个节目菜单信息中的ECM监听信息,所带结构体ddwParm2 = (DWORD)&IRDETO_MONIDESSTATUS_ST 
	CA_INFO_SRC_EMM_STATUS = 4,  //单个节目菜单信息中的EMM节目信息,所带结构体ddwParm2 = (DWORD)&IRDETO_EMMSRVSTATUS_ST 
	CA_INFO_SRC_ECM_STATUS ,     //单个节目菜单信息中的ECM节目信息,所带结构体ddwParm2 = (DWORD)&IRDETO_DESSRVSTATUS_ST 
    CA_INFO_COMPONENTS_STATUS = 6,// 已不使用
    CA_INFO_SC_PINCODECHANGE_STATUS, //修改PIN码,所带结构体ddwParm2 = (DWORD)&IRDETO_SCPINCODECHANG_ST 
    CA_INFO_SC_PINCODCHECK_STATUS = 8, //修改PIN码后的状态,所带结构体ddwParm2 = (DWORD)&IRDETO_SCPINCODECHANG_ST 
    CA_INFO_REMOTECHANNEL_STATUS,   //REMOTECHANNEL 功能,所带结构体ddwParm2 = (DWORD)&IRDETO_REMOTECHANNEL_ST 
	CA_INFO_IRD_STATUS = 10,     //暂不使用 
	CA_INFO_LOADER_STATUS,       //Loader 菜单信息,所带结构体ddwParm2 = (DWORD)&IRDETO_LOADERSTATUS_ST 
	CA_INFO_MEMORY_ERROR = 12,   //内存错误消息
	CA_INFO_SC_NUMSECTOR_STATUS, //智能卡Sector信息,所带结构体dwParm2 = (DWORD)&IRDETO_SC_NUMBEROFSECTOR_ST
	CA_INFO_CAREGION_STATUS = 14,//智能卡区域码信息,所带结构体dwParm2 = (DWORD)&IRDETO_CAREGION_ST
	CA_INFO_SCUSERDATA_STATUS ,  //智能卡Sector信息,所带结构体dwParm2 = (DWORD)&IRDETO_SC_USERDATA_ST
	CA_INFO_EMM_USERLOAD_STATUS = 16, //emm user payload data,所带结构体dwParm2 = (DWORD)&IRDETO_EMM_USERPAYLOAD_ST
	CA_INFO_DVBSERVICE_STATUS , //CA节目信息汇总菜单,所带结构体dwParm2 = (DWORD)&IRDETO_DVBDESCRABLE_ST
	CA_INFO_MAIL = 18,          //邮件信息,所带结构体dwParm2 = (DWORD)&IRDETO_CAMSG_ST
	CA_INFO_IPPVPIN_STATUS,     //IPPV购买信息,所带结构体dwParm2 = (DWORD)&IppvPurchaseInfo_S
	CA_INFO_IPPV_SECTOR_INFO = 20, //IPPV 某Sector 信息,所带结构体dwParm2 = (DWORD)&IppvSectorInfo_S
    CA_INFO_IPPV_EVENT_LIST,       //IPPV 某Sector Event信息列表,所带结构体dwParm2 = (DWORD)&IppvEventList_S
	
	CA_INFO_MR_CARD_STATUS,    //用户进入MR智能卡信息菜单时,需要显示的信息，所带结构体dwParm2 = (DWORD)&MrCardInfo_S
	CA_INFO_MR_USERPROFILE_DATA,//MR设置成功后传递用户的密码等信息，所带结构体dwParm2 = (DWORD)&MrUserProfileData_S

	CA_INFO_MAX
}IRDETO_CAINFO_EN;

typedef enum _CACOMMAND_EN
{
	CA_COMMAND_FINGER_PRINT = 0,         //ECM指纹命令,所带结构体dwParm2 = (DWORD)&IRDETO_CACOVERTFINGERPRINT_ST
	CA_COMMAND_HOMECHANNEL_SET,          //设置HomingChannel频点,所带结构体dwParm2 = (DWORD)&T_DeliverySystem
	CA_COMMAND_HOMECHANNEL_SCAN = 2,     //按下发频点重搜频点,所带结构体dwParm2 = (DWORD)&T_DeliverySystem
	CA_COMMAND_APP_HOMECHANNEL_SCAN,     //重搜APP主频点命令 
	CA_COMMAND_RESET_CHANNEL = 4,        //重新排序节目命令,所带结构体dwParm2 = (DWORD)BouquetID,消息所带参数为BouquetID值，APP需要根据BouquetID值重新排序节目，不属于该BouquetID值的节目应该屏蔽。
	CA_COMMAND_RECONNECT_CURRENT_SERVICE,//强制更新节目命令
	CA_COMMAND_REBOOT = 6,               //重启机顶盒命令
	CA_COMMAND_TUNE_SERVICE,             //强制换台命令,所带结构体dwParm2 = (DWORD)&IRDETO_TUNESVC_ST
	CA_COMMAND_CHANGE_PGPIN = 8,         //改变PIN 码命令,dwParm2 = (DWORD)&IRDETO_CAPIN_ST
	CA_COMMAND_CHANGE_INSTALLPIN,        //强制恢复PIN 码命令,dwParm2 = (DWORD)&IRDETO_CAPIN_ST
	CA_COMMAND_EMM_OTA = 10,             //强制OTA升级命令,dwParm2 = (DWORD)&IRDETO_CAEMMOTA_ST
	CA_COMMAND_OTA_NEXT,                 //OTA中NIT表分析完毕,将频点信息上传应用,所带结构体dwParm2 = (DWORD)&T_OtaCfg
	CA_COMMAND_OTA_DETECT = 12,          //OTA中BAT表分析完毕,将是否有升级信息的状态上传应用,所带结构体dwParm2 = (DWORD)TRUE:有OTA流存在, False:没有OTA流
	CA_COMMAND_ATTRIB,                   //TMS 消息,所带结构体dwParm2 = (DWORD)&IRDETO_ATTRIBCOVERT_ST
	CA_COMMAND_SURFLOCK = 14,            //SurfLock命令,所带结构体dwParm2 = (DWORD)IRDETO_SURFLOCK_ST
    CA_COMMAND_IPPVPREVIEW,              //IPPV预览命令,所带结构体dwParm2 = ((DWORD)&IppvPreviewInfo_S//amended by Stone 20090112
    CA_COMMAND_MRPIN,                    //应用在播放节目弹出密码况,要求用户输入MR 密码，所带结构体dwParm2 = ((DWORD)&MrPinInfo_S
    CA_COMMAND_SET_BISSKEY,              //应用收到此命令后，须调用CSCASetBISSSessionKey函数
	
	CA_COMMAND_MAX	
}IRDETO_CACOMMAND_EN;

/*Error信息结构中不合法的TextNum属性*/
#define INVALID_BAKMSG_TEXT         ( 0xFFFF )

/*已不使用*/
#define CA_MAX_SIZE_FINGERPRINT      (15)
/*已不使用*/

typedef struct _CAFINGERPRINT_ST  //FINGER PRINT
{
	BYTE    	m_cInfoNo;     	//特殊信息号
	INT     	m_nOvertime;  	 //超时限制
	BYTE    	m_acMsg[CA_MAX_SIZE_FINGERPRINT]; //指纹内容
}IRDETO_CAFINGERPRINT_ST;

/*ECM指纹信息*/
typedef struct _CACOVERTFINGERPRINT_ST 
{
	BYTE    	m_cInfoNo;     	//特殊信息号,目前为0
	WORD     	m_wDuration;  	 //超时限制,表示指纹显示的持续时间,单位秒.为0 表示不确定,一直等到下一条指纹替代该条指纹.
	BOOL        m_bFlash;        //是否闪烁,当闪烁为TRUE时,对于Overted 指纹则是一般的闪烁即可.对于Covert指纹,则要显示只需要显示部分帧画面.
    BOOL        m_bBanner;       //为TRUE时,需要显示消息 不带标题, 否则需要带标题
    BYTE        m_bCoverPer;     //覆盖率,消息框占整个显示屏幕的面积百分比.如果,要求消息框太小以至显示不下所有消息,则出框的消息要被截断.当指纹类型为Covert指纹时,指纹消息的背景要求透明.
    BOOL        m_bCovert;       //指纹类型,为TRUE时表示该指纹为Covert 指纹. 为FALSE时表示该指纹为Overt指纹.
	BYTE    	m_acMsg[CA_MAX_SIZE_FINGERPRINT]; //指纹内容,字符串数组
	/*---------------------------------------
	以下属性只有当有EOF功能是,并且 m_bCovert==FALSE 时才有意义，否则忽略  
	-----------------------------------------*/
	BYTE		m_ucLocationX;	//横坐标位置
    BYTE		m_ucLocationY;   //纵坐标位置
    BYTE        m_ucBgTransAlpha; // 背景透明度
    DWORD	    m_dwBgColour;		//背景颜色
    BYTE        m_ucFontTransAlpha; // 字体透明度
    DWORD	    m_dwFontColour;	  //字体颜色
    BYTE		m_ucFontType;	  //字体类型

}IRDETO_CACOVERTFINGERPRINT_ST;


/*Irdeto CA PIN 码*/
typedef struct  _CAPIN_ST
{
	BYTE*   m_pcPin; //PIN 码
	BYTE    m_cPinLen;//PIN 码长度
}IRDETO_CAPIN_ST;

/*EMM 强制升级命令*/
typedef struct _CAEMMOTA_ST
{
	BOOL    m_bDownLoadAllowed; //是否允许下载
	BOOL    m_bForced;  //是否为强制
}IRDETO_CAEMMOTA_ST;

/*邮箱中的邮件类型*/
typedef enum _TXTTYPE_ST
{
	CA_TXT_MAIL         	= 0,//Text -- Mailbox,	邮件消息
	CA_TXT_ANNOUNCE     	= 1,//Text -- Announcement,广播通知
	CA_TXT_CLUB         	= 2 //Club number,会员号
}IRDETO_TXTTYPE_ST;

/*暂未使用*/
typedef enum _TXTSHOWMODEL_EN
{
	CA_TXT_SHOW_NORMAL		= 0x00,		//Normal
	CA_TXT_SHOW_TIMED		= 0x01,		//Timed(automatic erase after date/time)
	CA_TXT_SHOW_FORCED		= 0x02		//Forced Display
}IRDETO_TXTSHOWMODEL_EN;

/*邮件内容最大长度*/
#define CA_MAX_SIZE_TXT_MAIL       (512)

/*邮件接收时间信息*/
typedef struct _CADATETIME_ST
{
	DWORD   m_dwYear;//年
	BYTE    m_cMonty;//月
	BYTE    m_cDay; //日
	BYTE    m_cHour; //时
	BYTE    m_cMinute; //分
}IRDETO_CADATETIME_ST;

/*邮件信息属性*/
typedef struct _CAMSG_ST
{
	BYTE    	m_cType; //T_TxtType// 0: Text_Mail  1: Text_Announcement 
	BYTE    	m_cClass;//T_TxtShowModel// 0: Normal     1: Timeed       2: ForcedDisplay
	BYTE    	m_cPriority; //邮件优先级
	DWORD   	m_dwLen; // 邮件长度
	IRDETO_CADATETIME_ST  m_stDeleteTime; 	//在TIMED邮件中存在
	BYTE    m_szContent[CA_MAX_SIZE_TXT_MAIL];//邮件内容,字符串数组
    WORD    m_wDuration;//for TMS mail  显示时间
    BYTE    m_bCoverPer; //for TMS mail 覆盖率
}IRDETO_CAMSG_ST;

/* 邮箱中的邮件信息*/
typedef struct _CAMAIL_ST		
{	
	BOOL    m_bReadFlag;          	//已读标志
	BOOL    m_bLockFlag;	      	 //邮件锁定标志
	IRDETO_CADATETIME_ST  m_stArriveTime; //邮件到达的时间
	IRDETO_CAMSG_ST m_stMsg; //邮件属性
}IRDETO_CAMAIL_ST;

 /*OTA升级所处阶段,及该阶段还有多少秒剩余时间就会超时.
 Step值:
 0  正在搜索NIT表
 1  正在搜索PAT表
 2  正在搜索PMT表
 3  正在搜索PID
 */
typedef struct _OTASTATUS_ST  
{
	BYTE    m_cStep;	//OTA阶段
	BYTE    m_cTimeLeft;//剩余时间
}IRDETO_OTASTATUS_ST;

/* OTA 失败类型*/
typedef enum _OTAERROR_EN     
{
	CA_OTA_ERROR_1,//NIT表没有搜索到
	CA_OTA_ERROR_2// PAT,PMT 或者是PID没有搜索到
}IRDETO_OTAERROR_EN;

/*节目信息结构体，供StartCA,StopCa使用*/
typedef struct _CAFTASERVICE_ST	
{
	WORD    m_wDemuxID;        //DMX ID号
	WORD    m_wServiceIndex;   //index
	WORD    m_wOrginalNetworkID;//原始网络ID号
	WORD    m_wTsID;           //流ID号
	WORD    m_wServiceID;      //节目ID号
	BYTE	m_bESStreamCount;  //ES 个数
	WORD   m_wESStreamPid[32]; //ES pid
	WORD    m_wServiceHandle; //Service Handle
	DWORD   m_dwUserData;        	//预留
}IRDETO_CAFTASERVICE_ST;

/*以下关于组件菜单的宏定义及结构体暂不使用start*/

#define		MAX_CPNT					    20
#define		CPNT_STATUS_NAME_LEN		    32
#define		CPNT_STATUS_VERSION_LEN	        21
#define		CPNT_STATUS_DATE_LEN		    128

typedef struct _COMPONSTAUT_INFO_ST
{
    BYTE    m_bMajor;
    BYTE    m_bMinor;
    DWORD   m_uSubVersion;
	BYTE	m_szComponName[CPNT_STATUS_NAME_LEN];	
	BYTE	m_szComponDate[CPNT_STATUS_DATE_LEN];	
    BYTE	m_szComponVersion[CPNT_STATUS_VERSION_LEN];	
}IRDETO_COMPONSTAUT_INFO_ST;

typedef struct _COMPONSTAUTS_ST
{
	BYTE	m_bTotalComponItem;
	IRDETO_COMPONSTAUT_INFO_ST m_szComponts[MAX_CPNT];	
}IRDETO_COMPONSTAUTS_ST;

/*以上关于组件菜单的宏定义及结构体暂不使用end*/

/* Irdeto 主菜单信息*/
typedef struct _CASTATUS_ST
{
	BYTE	m_bMajor;// Softcell Major version
	BYTE	m_bMinor;// Softcell Mini Version
	DWORD   m_uSubVersion; //Softcell SubVersion
	CHAR	m_acNameString[CPNT_STATUS_NAME_LEN];// Softcell  name,字符串数组
	CHAR	m_acBuildInfoString[CPNT_STATUS_DATE_LEN]; //Softcell  Build Info,字符串数组
	CHAR	m_acCaTaskVerString[CPNT_STATUS_VERSION_LEN];//CA Task Version,字符串数组
}IRDETO_CASTATUS_ST;

/*Emm菜单中最多可以显示的资源数*/
#define     MAX_SERVICE_RESOURCES           32

/*Emm监听数据最长字节*/
#define     MAX_SRC_EMM_MONITOR_LEN         64//changed by zhentiejun 081110

/*单个节目菜单中EMM节目信息*/
typedef struct _EMMSRVSTATUS_ST
{
	DWORD           m_wEmmHandle; //Emm handle
	WORD            m_wServiceHandle; //Service Handle
	IRDETO_SRVERRINFO_ST    m_stGlobalStatus; //Global Status
	IRDETO_SRVERRINFO_ST    m_stServiceStatus; // Service Status
	WORD            m_wEmmPid; //EMM pid
	BOOL			m_fEmmServiceDefine;//Emm severvice 是否被定义
	BYTE			m_bRscCount;//资源数
	DWORD           m_awRscType[MAX_SERVICE_RESOURCES];//资源类型列表
	IRDETO_SRVERRINFO_ST    m_stRscStatus[MAX_SERVICE_RESOURCES];//资源状态
    BYTE            m_acEmmString[MAX_SRC_EMM_MONITOR_LEN];//EMM 监听字节,16进制数
    DWORD           m_dwEmmResourceID; // added by zhentiejun 081127 EMM 资源ID
}IRDETO_EMMSRVSTATUS_ST;

/*Ecm监听数据最长字节*/
#define     MAX_SRC_ECM_MONITOR_LEN         64//changed by zhentiejun 081110

/*Ecm菜单中最多可以显示的ES数*/
#define     MAX_ECMSERVICE_STREAMS          32

/*单个节目菜单中ECM节目信息*/
typedef struct _DESSRVSTATUS_ST
{
	DWORD		m_wGroupHandle;// Group Handle
	DWORD		m_wServiceHandle;//Service Handle
	DWORD		m_wServiceType;// service  类别
	IRDETO_SRVERRINFO_ST    m_stGlobalStatus;//Global Status
	IRDETO_SRVERRINFO_ST    m_stServiceStatus;//Service Status
	BYTE		m_bStreamCount;// ES 个数
    WORD        m_wStrInfoStreamPid[MAX_ECMSERVICE_STREAMS];//ES pid
    WORD    	m_wStrInfoEcmPid[MAX_ECMSERVICE_STREAMS];//ECM pid
	IRDETO_SRVERRINFO_ST    m_stStrInfoStatus[MAX_ECMSERVICE_STREAMS];//ES 状态	
	BYTE		m_bRscCount;//资源数
	WORD        m_awRscType[MAX_SERVICE_RESOURCES];//资源类型
	IRDETO_SRVERRINFO_ST    m_stRscStatus[MAX_SERVICE_RESOURCES];//资源状态
    BYTE		m_abEcmData[MAX_SRC_ECM_MONITOR_LEN];//ECM 监听数据,16进制数
    WORD        m_abEcmMonitorCount;// added by zhentiejun 081126  //ECM 监听个数
    DWORD       m_dwEcmResourceID; // added by zhentiejun 081127 // ECM 资源ID
}IRDETO_DESSRVSTATUS_ST;

/* 单个节目信息中EMM 的监听信息 */
typedef struct _MONIEMMSTATUS_ST
{
	DWORD       m_wEmmHandle;//EMM handle
	WORD        m_wServiceHandle;//EMM 对应的Service Handle	
	BYTE        m_acEmmString[MAX_SRC_EMM_MONITOR_LEN];//EMM 监听字节,16进制数
}IRDETO_MONIEMMSTATUS_ST;

/* 单个节目信息中ECM 的监听信息 */
typedef struct _MONIDESSTATUS_ST
{
	DWORD       m_wGroupHandle;// Group Handle
	WORD        m_wServiceHandle;// Service Handle
	WORD        m_wEcmPid;// added by zhentiejun 081126 //ECM pid
	BYTE        m_abEcmData[MAX_SRC_ECM_MONITOR_LEN];// ECM 监听数据,16进制数
}IRDETO_MONIDESSTATUS_ST;

/*Irdeto CA 中出现的日期时间信息*/
typedef struct _DATETIME_ST
{
    BYTE    m_bYear;//年
    BYTE    m_bMonth;//月
    BYTE    m_bDate;// 日
    BYTE    m_bHour;//时
    BYTE    m_bMintue;//分
    BYTE    m_bSecond;//秒
}IRDETO_DATETIME_ST;

/* CA Loader菜单信息*/
typedef struct _LOADERSTATUS_ST
 {
    BYTE        m_bManufacturerId;//厂商号
    BYTE        m_bHardwareVersion;//硬件版本号
    WORD        m_bVariant;//市场号

    WORD       m_wSystemId;//System ID
    WORD        m_wKeyVersion;//key 版本
    WORD        m_wSignatureVersion;//签字版本

    DWORD       m_dLdSECSSN;// chip ID

    BYTE        m_bDownLoadID;// 下载号
    //--------------------------------------------------
    BYTE        m_bFirmVersion[CA_MAX_SIZE_FIRMWARE];//Firm Version,字符串数组

    BYTE        m_bLoadVersion[CA_MAX_SIZE_LOASER];//Load Version,字符串数组

    BYTE        m_bSerialNO[CA_MAX_SIZE_IRD_SERIALNUMBER];// 序列号,字符串数组

    WORD        m_wSpecInfo;//Spec 信息
    IRDETO_DATETIME_ST m_stSignDateTime;//签字日期
    IRDETO_DATETIME_ST m_stDownloadDateTime;//下载日期
    
}IRDETO_LOADERSTATUS_ST;

/*for Irdetoca smartcard pin code change/check, only supportd ippv */
#define MAX_SC_PINCODE_LEN 2

/*修改PIN码*/
typedef struct _SCPINCODECHANG_ST
{
    DWORD       m_wSCResourceId;//资源号
	BYTE        m_bCodeIndex;//PIN 码索引
    BYTE        m_OldPinCode[MAX_SC_PINCODE_LEN];//旧PIN 码,16进制数
    BYTE        m_NewPinCode[MAX_SC_PINCODE_LEN];//新PIN 码,16进制数
    BYTE        m_bCodeStatus;//PIN 状态
}IRDETO_SCPINCODECHANG_ST;

/*for Irdetoca remote channel */
#define MAX_PAYLOAD_LEN 256

/*实现 REMOTECHANNEL 功能*/
typedef struct _REMOTECHANNEL_ST
{
	DWORD       m_wResourceId;//资源号   
	BYTE        m_bPayloadLengthMax;//净荷最大长度
	BYTE        *pbPayload; // 净荷
}IRDETO_REMOTECHANNEL_ST;

#define MAX_SC_NUMSECTOR_SIZE 8 //智能卡Sector分区最大个数

/* 智能卡sector分区信息*/
typedef struct _SC_NUMBEROFSECTOR_ST
{
    DWORD       m_wResourceId;//资源号
    BYTE        m_bsectorCount;//sector 个数
    BYTE        m_bSector[MAX_SC_NUMSECTOR_SIZE];//Sector 号码,16进制数
    BYTE        m_bActive[MAX_SC_NUMSECTOR_SIZE];//sector是否激活,16进制数
    BYTE        m_bReserverd[MAX_SC_NUMSECTOR_SIZE];//是否为预留,16进制数
}IRDETO_SC_NUMBEROFSECTOR_ST;

/*智能卡区域码信息*/
typedef struct _CAREGION_ST
{
	DWORD       m_wResourceId;//资源号
	BYTE        m_bRequestType;//请求类型 
	BYTE		m_bSector;//Sector 号码    
	BYTE		m_bSectorStatus;//sector 状态
	BYTE		m_bRegion;//区域码        
	BYTE		m_bSubRegion;//子区域码  
}IRDETO_CAREGION_ST;

#define MAX_SC_USERINFO_LEN 28//智能卡描述用户信息的最长字节数

typedef struct _SC_USERDATA_ST
{
	DWORD     	m_wResourceId;//资源号
	BYTE        m_abUserData[MAX_SC_USERINFO_LEN]; //用户信息,16进制数
}IRDETO_SC_USERDATA_ST;

/*for Irdetoca surflock status*/
typedef struct _SURFLOCK_ST
{
	BYTE        m_bRequestType;//请求类型
	DWORD       m_wResourceId;//资源号
	BOOL        m_fSurfLocked;//是否已经SurfLock
	DWORD       m_wGroupHandle;//GroupHandle
} IRDETO_SURFLOCK_ST;

#define MAX_EMM_USERLOAD_LEN 256//emm user payload 最长字节数

/*for Irdetoca emm user payload data */
typedef struct _EMM_USERPAYLOAD_ST
{
	DWORD           m_wEmmHandle;//EMM handle			
	WORD            m_wMessageLength;//消息长度
	BYTE            m_abMsgData[MAX_EMM_USERLOAD_LEN];//所带消息,16进制数
} IRDETO_EMM_USERPAYLOAD_ST;

/*TMS消息内容最长字节数*/
#define MAX_ATTRIBDATA_LEN 511

/*TMS消息类型*/
typedef enum _TMSTYPE_EN
{
	CA_TMS_NORMALMAIL ,  //普通邮件         
	CA_TMS_FORCEDMAIL ,  //紧急邮件,也叫强制显示邮件         
	CA_TMS_FINGERPRT ,   //指纹        
	CA_TMS_MAXTYPE  
}IRDETO_TMSTYPE_EN;

/*已不使用*/
typedef struct _ATTRIB_ST
{
	BYTE        m_bIndex;				
	DWORD       m_wEmmHandle;     
	WORD        m_wServiceHandle;            
	WORD        m_wMsgLength;
	BYTE        m_bMessageData[MAX_ATTRIBDATA_LEN];
	IRDETO_TMSTYPE_EN        m_bType;
	BOOL        m_bFlash;
    BOOL        m_bBanner;
    BYTE        m_bCoverPer;
	WORD        m_wDuration;
} IRDETO_ATTRIB_ST;

/*TMS 消息*/
typedef struct _ATTRIBCOVERT_ST
{
	BYTE        m_bIndex;// TMS 消息索引号				
	DWORD       m_wEmmHandle; //EMM handle    
	WORD        m_wServiceHandle;//Service Handle            
	WORD        m_wMsgLength; //TMS 消息长度
	BYTE        m_bMessageData[MAX_ATTRIBDATA_LEN];//TMS 消息内容,字符串数组
	IRDETO_TMSTYPE_EN        m_bType;//TMS 消息类型.0:普通邮件 1:强制邮件 2:指纹
	BOOL        m_bFlash;	 //是否闪烁,当闪烁为TRUE时,对于Overted 指纹则是一般的闪烁即可.对于Covert指纹,则要显示只需要显示部分帧画面.
    BOOL        m_bBanner;   //为TRUE时,需要显示消息 不带标题, 否则需要带标题
    BYTE        m_bCoverPer;//覆盖率,消息框占整个显示屏幕的面积百分比.如果,要求消息框太小以至显示不下所有消息,则出框的消息要被截断.当指纹类型为Covert指纹时,指纹消息的背景要求透明.
	WORD        m_wDuration;//超时限制,表示指纹显示的持续时间,单位秒.为0 表示不确定,一直等到下一条指纹替代该条指纹
	BOOL        m_bCovert;  //当属性m_bType为指纹时,m_bCovert表示指纹类型,为TRUE时表示该指纹为Covert 指纹. 为FALSE时表示该指纹为Overt指纹.
	/*---------------------------------------
	以下属性只有当m_bType ＝ 2并且 m_bCovert == FALSE&&g_bIsEof== TRUE 时才有意义，否则忽略(EOF需求)   
	-----------------------------------------*/
	BYTE		m_ucLocationX;	//横坐标位置
    BYTE		m_ucLocationY;   //纵坐标位置
    BYTE        m_ucBgTransAlpha; // 背景透明度
    DWORD	    m_dwBgColour;		//背景颜色
    BYTE        m_ucFontTransAlpha; // 字体透明度
    DWORD	    m_dwFontColour;	  //字体颜色
    BYTE		m_ucFontType;	  //字体类型

} IRDETO_ATTRIBCOVERT_ST;

/*执行强制换台命令时,强制换台的参数信息*/
typedef struct _TUNESVC_ST
{
	DWORD       m_wGroupHandle;//强制换台的Service 的GroupHandle
	WORD        m_wNetworkID;//强制换台的Service 的 Network ID
	WORD        m_wTransportStreamID;//强制换台的Service 的 TS ID
	WORD        m_wServiceID;//强制换台的Service 的 ServiceID
}IRDETO_TUNESVC_ST;

/*Severice类型*/
typedef enum _SERVICETYPE_EN
{
    SERVICE_EMM ,//EMM 服务
    SERVICE_DES ,//ECM 服务
    SERVICE_MAX
}IRDETO_SERVICETYPE_EN;

/*CA 节目汇总菜单中每个节目的信息*/

typedef struct _DVBDESCRABLE_INFO_ST
{
    DWORD   m_dwClientHandle;//节目对应的Handle
    WORD    m_wIndex;//节目在菜单中的索引
    IRDETO_SERVICETYPE_EN  m_eServiceType;//Service类型
    BYTE    m_bInstance; //Instance 值  
}IRDETO_DVBDESCRABLE_INFO_ST;

/*CA 节目汇总菜单信息*/
typedef struct
{
    BYTE    m_bServiceCount;//节目总数
    IRDETO_DVBDESCRABLE_INFO_ST m_stDVBServiceInfo[CA_DEMUX_MAX+CA_SRV_MAX];
}IRDETO_DVBDESCRABLE_ST;

/*用于判断当前节目类型*/
typedef enum _ESCURRENTTYPE_EN
{
	CA_ES_IRDETO ,//Irdeto加扰节目
	CA_ES_OTHER , //非Irdeto加扰节目
	CA_ES_FREE,   //清流
	CA_ES_MAX
}IRDETO_ESCURRENTTYPE_EN;

/*可支持的国家码个数*/
#define CA_MAX_COUNTRY_NUM         (20)

/*单个国家码信息*/
typedef struct _COUNTRYBL0CK_INFO_ST
{
	BYTE    m_cFlag;        //可用标志：1：可用；0：不可用
	BYTE  	m_acCountry[3]; //国家代码,3个字母的ASCII 码
}IRDETO_COUNTRY_INFO_ST;

/*国家码列表*/
typedef struct _COUNTRYBL0CK_ST
{
    WORD    m_wCountryNum;    //结构中国家代码个数
	IRDETO_COUNTRY_INFO_ST  	m_acCountryList[CA_MAX_COUNTRY_NUM]; //存放的国家代码
}IRDETO_COUNTRYBL0CK_ST;

/*IPPV 预览信息*/
typedef struct _IppvPreviewInfo
{
	DWORD    m_dwResourceId;//资源号
	BYTE     m_ucSector;//IPPV Event 所在的Sector分区
	WORD     m_wEventId;//EventId
	WORD     m_wEventCost;//Event费用
	BYTE     m_ucClockValue;//当ClockValue < PreviewValue时,IPPV处于
	BYTE     m_ucPreviewValue;//节目预览阶段,CW可以正常设置,否则不允许观看
	WORD     m_wServiceCount;//正在观看该Event的Service数
}IppvPreviewInfo_S;

/*确认购买IPPV节目信息*/
typedef struct _IppvPurchaseInfo
{
	DWORD    m_dwResourceId;//资源号
	DWORD    m_dwSerHandle;//确认购买的ServiceHandle
	BYTE     m_ucSector;//IPPV Event 所在的Sector分区
	WORD     m_wEventId;//EventId
	BYTE     m_aucIppvPin[2];//用户输入的密码,16进制数
	BYTE     m_ucScPinStatus;//PIN码校验状态
}IppvPurchaseInfo_S;

/*IPPV PIN 码校验状态*/
typedef enum
{
	CA_IPPV_BUY_SUCCESS = 1,//PIN码校验成功
	CA_IPPV_BUY_EVENT_NOT_AVAILABLE,//Event 不存在
	CA_IPPV_BUY_PIN_FAIL = 0x51,//PIN码校验失败
	CA_IPPV_BUY_PIN_BLOCK = 0x54,//PIN码被阻塞
	CA_IPPV_BUY_STORE_FULL = 0x9b,//Event存储已满
	CA_IPPV_BUY_NO_CREDIT = 0x9c// 余额不足
	
}IppvPinStatus_e;

/*IPPV Sector 信息查询*/
typedef struct _IppvSectorInfo
{
	DWORD    m_dwResourceId;//资源号
	BYTE     m_ucSector;//Sector 号
	WORD     m_wDebitLimit;//该Sector的借记上线
	WORD     m_wDebitThreshold;//该Sector的借记警戒值
	WORD     m_wOperatorPurse;//Operator钱包
	WORD     m_wCustomerPurse;//Customer钱包
	BYTE     m_ucMaxIppvEvents;//该Sector可存储的Event数
}IppvSectorInfo_S;

/*IPPV 某Sector的借记临界信息*/
typedef struct _IppvThresholdInfo
{
	DWORD    m_dwResourceId;//资源号
	BYTE     m_ucSector;//Sector 号
	WORD     m_wDebitThreshold;//该Sector的借记警戒值
}IppvThresholdInfo_S;


typedef struct
{
	WORD     	m_wIPPVEventID;//EventID
	BOOL		m_bEventReported;//是否已经上报前端.1:已经上报前端.2:尚未上报
	WORD     	m_wDateCode;//Event购买日期
	WORD     	m_wEventCost;//Event购买费用
}IppvEventRecord_S;

/*IPPV 某Sector中存储的Event列表信息*/
typedef struct
{
	DWORD	      	    m_dwResourceId;//资源号
	BYTE                m_bSector;//Sector 号
	BYTE     	        m_bDaysGrace;//表示可以查询的时间段.该值为距离查询当天可以跨越的最多天数
	BYTE 		        m_bStatus;//列表状态,0:m_pstEventRecords属性中有数据 1:m_pstEventRecords属性中没有数据
	BYTE     		    m_bIPPVEventCount;//该Sector存储的Event数
	IppvEventRecord_S   m_pstEventRecords[CA_IPPV_EVENT_MAX];//Event信息纪录
}IppvEventList_S;

/*当需要实现组件级加扰功能时,CA返回给应用的当前加扰状态*/
typedef enum 
{
    EM_CA_SHUT_BOTH_AV,//音视频全关闭
    EM_CA_SHUT_AUDIO, //只关闭音频
    EM_CA_SHUT_VIDEO  //只关闭视频
}E_CaCompLevelType;

/*查询、开启、关闭MR功能等操作的返回结果信息*/
typedef enum
{
	EM_CA_MR_CONFIG_OK			= 0,//操作成功
	EM_CA_MR_CONFIG_CARD_ERROR,     //智能卡错误
	EM_CA_MR_CONFIG_CARD_MR_INCAPABLE,//智能卡不支持MR功能
	EM_CA_MR_CONFIG_PIN_WRONG,//Subscriber PIN错误
	EM_CA_MR_CONFIG_PIN_BLOCK,//Subscriber PIN被锁定
	EM_CA_MR_CONFIG_DISABLE_MR_NOT_ALLOWED//不允许关闭MR功能
} E_MrConfigResult;

/*当用户进入MR智能卡信息菜单时,CA返回给应用显示的信息*/
typedef struct 
{ 
	IRDETO_SRVERRINFO_ST    m_stCardStatus;//智能卡状态
	BOOL  m_bCapablity; //智能卡是否支持MR功能
	BOOL  m_bEnable; //智能卡MR功能是否开启
	E_MrConfigResult  m_eResult;/*当用户查询、开启、关闭MR功能时,CA返回给应用的作用结果*/
}MrCardInfo_S;


/*APP传递的Subscriber密码以及想改变的卡状态*/
typedef struct
{ 
	BYTE m_aucSubscriberPin[ 2 ]; //Subscriber密码
	BOOL m_bEnable; //开启或关闭MR功能,1:开启，2:关闭
}MrConfiguration_S;

/*profile操作*/
typedef enum
{
	EM_CA_MR_USER_PROFILE_READ = 0,//profile查看
	EM_CA_MR_USER_PROFILE_SET,        //profile设置
	EM_CA_MR_USER_PROFILE_DELETE   //profile删除
	
}E_MrProfileAction;

/*Profile操作返回结果信息*/
typedef enum
{
	EM_MR_USER_PROFILE_OK			= 0,//成功
	EM_MR_USER_PROFILE_CARD_ERROR,//智能卡错误
	EM_MR_USER_PROFILE_CARD_MR_INCAPABLE,//智能卡不支持MR功能
	EM_MR_USER_PROFILE_SUBSCRIBER_PIN_WRONG,//Subscriber PIN错误
	EM_MR_USER_PROFILE_SUBSCRIBER_PIN_BLOCK,//Subscriber PIN被锁定
	EM_MR_USER_PROFILE_UPDATE_NOT_ALLOWED,//不允许设置操作
       EM_MR_USER_PROFILE_USER_ID_OUT_OF_RANGE, //Profile ID超出范围
       EM_MR_USER_PROFILE_USER_NOT_EXIST, //Profile不存在
       EM_MR_USER_PROFILE_USER_AGE_OUT_OF_RANGE//年龄设置超出范围
} E_MrProfileResult;

/*Profile操作返回给用户的年龄级别密码、结果等数据*/
typedef struct
{
	BYTE	m_ucProfileID;
	BYTE	m_ucUserAge;
	BYTE	m_aucUserPin[ 2 ];//返回给用户的UserPin
	E_MrProfileResult  m_eResult;   /*MR Profile操作的返回结果*/
	
}MrUserProfileData_S;

/*传递Subscriber的密码,操作以及用户设置的年龄等数据*/
typedef struct 
{ 
	BYTE  m_aucSubscriberPin[ 2 ]; //Subscriber密码
	E_MrProfileAction  m_eAction;    //profile操作
	MrUserProfileData_S  m_stUserProfile;//profile数据
 
}MrProfile_S;

/*传递用户的年龄级别密码等数据,用于验证是否有权限观看*/
typedef struct 
{ 
  DWORD m_dwGroupHandle; //
  DWORD m_dwServiceType; //
  BYTE  m_aucMRPin[2];//输入的MR密码
  DWORD m_dwResourceId;// 
}MrPinInfo_S;

//REBOOT命令来源
typedef enum 
{
	EM_IRDETO_REBOOTOTA = 0 ,  //OTA
	EM_IRDETO_REBOOTEMM ,  //EMM 
	EM_IRDETO_REBOOTTYPEMAX  
}RebootType_E;


typedef struct _CaFlashAddress
{
    BOOL  m_bCaFlashAddressFlag;  //TRUE: 正式app;FALSE: TestApp
    DWORD m_dwMainFlashAddr;      //主Flash 地址
    DWORD m_dwMainFlashLength;    //主Flash 长度 
    DWORD m_dwBackupFlashAddr;    //备份Flash  地址若不使用直接传递0
    DWORD m_dwBackupFlashLength;  //备份Flash  长度若不使用直接传递0
    
    DWORD m_dwReserve1;            //保留，针对某些CA 特殊需求的地址
    DWORD m_dwReserve2;            //保留，针对某些CA 特殊需求的地址
    DWORD m_dwReserve3;            //保留，针对某些CA 特殊需求的地址
    DWORD m_dwReserve4;            //保留，针对某些CA 特殊需求的地址
}CaFlashAddress_S;

typedef enum _CaFlashAddrErr
{
	EM_FLASH_FATAL_ERROR = -4,        
    EM_FLASH_PARAM_ERROR = -3,
	EM_FLASH_LENGTH_ERROR = -2,
	EM_OTHER_FAILURE = -1,
	EM_FLASH_SET_SUCCESS = 0
}CaFlashAddrErr_E;

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
*Function Name: CS_CA_GetCACompileTime
*   获取CA版本信息
*
*Parameters
*   pacVerBuf[out]:指向存储CA编译时间的空间
*   dwBufSize[in]:给定一个最大长度值.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetCACompileTime(char* pComTimeBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CA_Init
*   初始化CA模块
*
*Parameters	
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_Init(void);

/**************************************************************************
*Function Name: CS_CA_PVR_StartSrvRecord
*  启动一个节目录制通道
*
*Parameters 
*	nServiceID [in]:
*		节目号
*	nVideoPID[in]:
*		视频PID
*	nAudioPID[in]:
*		音频PID
*	cDemux[in]:
*		通道号（目前之支持二路通道）
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_NewPmtSectionNotify (BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewCATSectionNotify
*  通知CA新CAT数据
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
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewCatSectionNotify (BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name:CS_CA_NewBATSectionNotify
* 通知CA新BAT数据
*
*Parameters 
*	pcSectionData [in]:
*		BAT 数据缓存区地址 
*	nLen [in]:
*		BAT数据长度
*	cDemux[in]:
*		通道号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewBatSectionNotify (BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewNitSectionNotify
*  通知CA新CAT数据
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
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewNitSectionNotify (BYTE* pcSectionData, INT nLen, BYTE cDemux);

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
INT CS_CA_TXT_Add(IRDETO_CAMAIL_ST* pstTxt);

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
INT CS_CA_TXT_GetInfo(DWORD dwIndex, IRDETO_CAMAIL_ST** ppstTxt);

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
BOOL CS_CA_TXT_HaveNew(IRDETO_TXTTYPE_ST eType);

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
*	bLockFlag[in]:已读(TRUE),未读(FALSE)
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
*	pbLockFlag[out]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_GetLockFalg(DWORD dwIndex, BOOL* pbLockFlag);

/**************************************************************************
*Function Name:CS_CA_OTA_GetTunerParm
* 获取升级时所需的参数
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
void CS_CA_OTA_GetTunerParm(void);

/**************************************************************************
*Function Name:CS_CA_OTA_GetUpdateInfo
* 获取升级过程中界面需提示的信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
void CS_CA_OTA_GetUpdateInfo(void);

/**************************************************************************
*Function Name:CS_CA_OTA_SaveCfg
* 保存升级后的信息
*
*Parameters 
*	pstOtaCfg[in]
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_OTA_SaveCfg(T_OtaCfg* pstOtaCfg);

/**************************************************************************
*Function Name: CS_CA_EnterIrdetoMenu
*  APP请求CA界面数据
*
*Parameters 
*	IRDETO_MENU_STATUS_EN:  请求类型
*   uParam:  请求类型所带参数
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_EnterIrdetoMenu( IRDETO_MENU_STATUS_EN enumQueryDataType, DWORD dwParam );

/**************************************************************************
*Function Name: CS_CA_ExitIrdetoMenu
*  APP推出请求CA界面数据
*
*Parameters 
*	IRDETO_MENU_STATUS_EN:  推出类型
*   uParam:  请求类型所带参数
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_ExitIrdetoMenu( IRDETO_MENU_STATUS_EN enumQueryDataType );

/**************************************************************************
*Function Name: CS_CA_GetFlashMailAddress
*  获取CA存放邮件flash首地址和大小
*
*Parameters 
*	pFlashMailAddr:     邮件首地址
*   pFlashMailSize:     邮件大小
*
*Return Values: 
*		无
**************************************************************************/
void CS_CA_GetFlashMailAddress(DWORD* pFlashMailAddr, DWORD* pFlashMailSize);

/**************************************************************************
*Function Name: CS_CA_SetCountryBlock
*  获取当前节目的国家码列表信息
*
*Parameters 
*	pstCountypList:  国家码列表信息
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_SetCountryBlock (IRDETO_COUNTRYBL0CK_ST* pstCountypList);

/**************************************************************************
*Function Name: CS_CA_SetSecureCWFlag
*  是否开启高级安全功能
*
*Parameters 
*	bFlag:  TREU则开启高级安全;FALSE不开启.
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_SetSecureCWFlag(BOOL bFlag);

/**************************************************************************
*Function Name: CS_CA_FAKE_NVM_EraseAllFlash
* 擦除CA对应的FLASH
*
*Parameters 
*	无
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
DWORD CS_CA_FAKE_NVM_EraseAllFlash(void);

/**************************************************************************
*Function Name: CSCAPurchaseIppv
* 确认购买IPPV节目
*
*Parameters 
*	pstIppvInfo : 确定购买的IPPV相关信息
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CSCAPurchaseIppv(IppvPurchaseInfo_S* pstIppvInfo);

/**************************************************************************
*Function Name: CSCAGetIppvSectorInfo
* 获取IPPV节目对应的卡中sector信息
*
*Parameters 
*	dwResourceId : 卡的resource ID
*     bSector:    要请求的卡的sector号  
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CSCAGetIppvSectorInfo(DWORD dwResourceId, BYTE bSector);

/**************************************************************************
*Function Name: CSCASetIppvThreshold
* 修改透支上限
*
*Parameters 
*	pstIppvThrInfo: 对应卡的sector号,resourceID,以及上限值
*      
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CSCASetIppvThreshold(IppvThresholdInfo_S* pstIppvThrInfo);

/**************************************************************************
*Function Name: CSCAGetIppvEventList
* 获取卡中Event列表信息
*
*Parameters 
*	dwResourceId : 卡的resource ID
*     bSector:    要请求的卡的sector号  
*     bDaysGrace:  要请求的天数    
*      
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CSCAGetIppvEventList (DWORD dwResourceId, BYTE bSector, BYTE bDaysGrace);

/**************************************************************************
*Function Name: CS_CA_GetCompLevelShutType
*  当应用收到E-16时,调用此函数以获得加扰方式为Complevel(组件级加扰)时,关闭的是音频,视频,还是音视频都关闭.
*  老接口，兼容老项目，新项目不建议使用.
*Parameters 
*	无
*
*Return Values: 
        -1 : FAILURE
*		0 : 音视频全部关闭 
*		1 : 关闭音频
        2 : 关闭视频
**************************************************************************/
INT CS_CA_GetCompLevelShutType(void);

/**************************************************************************
*Function Name: CSCAGetCompLevelShutType
*  当应用收到E-16时,调用此函数以获得加扰方式为Complevel(组件级加扰)时,关闭的是音频,视频,还是音视频都关闭.
*  用于多路解扰. 
*Parameters 
*	dwGroupHandle: 当收到E16消息时,回调函数中的第三个参数就是dwGroupHandle,应用在调用该函数时将param2中的值原值返回.
*
*Return Values: 
        -1 : FAILURE
*		0 : 音视频全部关闭 
*		1 : 关闭音频
        2 : 关闭视频
**************************************************************************/
 INT CSCAGetCompLevelShutType(DWORD dwGroupHandle);

/**************************************************************************
*Function Name: CS_CA_SOLParseBATData
*  搜索Bouquet_id为0x3623的BAT表后,调用此接口让CA分析BAT
* 
*Parameters 
*	pcData  BAT数据
*   nDataLen BAT长度
*Return Values: 
*         无
*		
*		
**************************************************************************/
void CS_CA_SOLParseBATData(BYTE* pcData, int nDataLen );

/**************************************************************************
*Function Name: CS_CA_SOLParseBATData
*  搜索到NIT表后,调用此接口让CA分析NIT
* 
*Parameters 
*	pcData  BAT数据
*     nDataLen BAT 长度
      bCompleteFlg NIT表是否完整

*Return Values: 
          无
*		
*		
**************************************************************************/
void CS_CA_SOLParseNitData(BYTE* pcData, int nDataLen, BOOL bCompleteFlg);

/**************************************************************************
*Function Name: CSCAGetOtaSerIdOfBat
*  获取OTA过程中BAT表中的serviceID
*
*Parameters 
*	无
*
*Return Values: 
*		OTA过程中BAT表中的serviceID
*		
**************************************************************************/
WORD CSCAGetOtaSerIdOfBat(void);

/**************************************************************************
*Function Name: CSCASetCardMrStatus
*  
*
*Parameters 
*	pstMrConfiguration:    应用传递用户的密码以及想改变的卡状态;
*Return Values: 
*	 成功: SUCCESS
        失败: FAILURE 
*		
**************************************************************************/
INT  CSCASetCardMrStatus (MrConfiguration_S  *pstMrConfiguration);

/**************************************************************************
*Function Name: CSCAMrProfileAction
*  
*
*Parameters 
*	pstMrProfile:    传递用户的密码以及Profile操作;
*Return Values: 
*	 成功: SUCCESS
        失败: FAILURE 
*		
**************************************************************************/
INT  CSCAMrProfileAction (MrProfile_S  *pstMrProfile);

/**************************************************************************
*Function Name: CSCAValidateMrPIN
*  
*
*Parameters 
*	pstPinInfo:    传递用户的密码等信息;
*Return Values: 
*	 成功: SUCCESS
        失败: FAILURE 
*		
**************************************************************************/
INT  CSCAValidateMrPIN (MrPinInfo_S  *pstPinInfo);

/**************************************************************************
*Function Name:CSCASetFlashAddress
* 设置CA 内核存储数据所需的FLASH 地址
*
*Parameters
*  pstFlashAddress: CA Flash 相关信息
                                
pstFlashAddress->m_bCaFlashAddressFlag:1 正常模式,正常读写FLash
                                        0 TESTAPP 模式,数据不写入flash

pstFlashAddress->m_dwMainFlashAddr: 分配给CA内核主数据区地址,必须为一个非零值
pstFlashAddress->m_dwMainFlashLength: CA内核主数据区大小,不小于64K
pstFlashAddress->m_dwBackupFlashAddr:分配给CA内核备份数据区地址,无备份区,设为0
pstFlashAddress->m_dwBackupFlashLength:CA内核备份数据区大小,无备份区,设为0;
                                       有备份区时,不小于64K                                    
*                     
*Return Values: 
*	见CaFlashAddrErr_E定义

Note:
    若该函数返回错误，将导致CA初始化失败!!
**************************************************************************/
CaFlashAddrErr_E CSCASetFlashAddress(CaFlashAddress_S *pstFlashAddress);

/**************************************************************************
*Function Name:CSCASetBISSSessionKey
* 节目使用BISS方式加扰时，应用调用此接口设置session key进行解扰
*
*Parameters
*  aucKey[]: session key,包含ODD KEY和EVEN KEY 
*  nKeyLen: session key的长度,包含ODD KEY和EVEN KEY                                                   
*                     
*Return Values: 
*	成功: SUCCESS
    失败: FAILURE 

Note:
    该函数可重复调用。
	示例：BISS key为[0x11, 0x22, 0x33, 0x66, 0x33, 0x22, 0x11, 0x66],则
	aucKey[] = {0x11, 0x22, 0x33, 0x66, 0x33, 0x22, 0x11, 0x66,
	0x11, 0x22, 0x33, 0x66, 0x33, 0x22, 0x11, 0x66};
	nKeyLen = 16;
**************************************************************************/
INT CSCASetBISSSessionKey(BYTE aucKey[], INT nKeyLen);

/*以下接口将废弃，暂时保留以兼容老项目*/
/**************************************************************************
*Function Name: CS_CA_GetFlashNVROMAddress
*  获取CA存放私有数据的flash首地址和大小
*
*Parameters 
*	pFlashNVMAddr:      NVM首地址
*   pFlashNVMSize:      NVM大小
*
*Return Values: 
*		无
**************************************************************************/
void CS_CA_GetFlashNVROMAddress(DWORD* pFlashNVMAddr, DWORD* pFlashNVMSize);

/**************************************************************************
*Function Name: CSCASetBackupFlashAddr
*  
*
*Parameters 
*	dwBackFlashAddr:    应用为CA分配的备份用的Flash的起始地址;
*	dwBackFlashNVMSize: 该块Flash的大小,单位是Byte
*Return Values: 
*	 成功: SUCCESS
     失败: FAILURE 
*		
**************************************************************************/
INT CSCASetBackupFlashAddr(DWORD dwBackFlashAddr, DWORD dwBackFlashNVMSize);

#ifdef  __cplusplus
}
#endif

#endif //_CS_IRDETO_INTERFACE_H_

