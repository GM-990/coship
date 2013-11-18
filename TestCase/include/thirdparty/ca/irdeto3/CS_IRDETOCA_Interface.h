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
#define     MAX_SC_NUMBER       2 //�˵�����������ʾ�����ܿ���
#define     SC_NUMBER_LEN       16 //���ܿ�����󳤶�
#define     SC_TYPE_LEN         8 //���ܿ����͵������ֽڵ���󳤶�
#define     SC_VER_LEN          8 //���ܿ��汾�����ֽڵ���󳤶�
#define     SC_NATION_LEN       8 //���ܿ������������ֽڵ���󳤶�
#define     SOURCE_SMARTCARD    4 // Error ��Ϣ�е�Source ���Ե�һ��ȡֵ,��ʾ�ô�����ϢԴ�����ܿ�
#define     SOURCE_APPLICATION  32 // Error ��Ϣ�е�Source ���Ե�һ��ȡֵ,��ʾ�ô�����ϢԴ��Ӧ��

#define     SOURCE_MOD_EMM_SOURCE 6
#define     SOURCE_MOD_ECM_SOURCE 9
								   
#define     SOURCE_EMM_SOURCE 2// Error ��Ϣ�е�Source ���Ե�һ��ȡֵ,��ʾ�ô�����ϢԴ��EMM
#define     SOURCE_ECM_SOURCE 1// Error ��Ϣ�е�Source ���Ե�һ��ȡֵ,��ʾ�ô�����ϢԴ��ECM
								   // ��:E16-4 Severice is not available. "4"��ʾ�ô���Դ�����ܿ�							

#define 	CA_DEMUX_MAX  2 // ��֧�ֵ�Demux�����
#define 	CA_SRV_MAX 		3//��·���ŵĿ�֧�ֵĽ�Ŀ�����
#define     CA_IPPV_EVENT_MAX    32//IPPV �п���֧�ֵ�EVENT������

/* Error ��Ϣ�е�Severity ���Ե�ȡֵ,��ʾ�ô�������
��Ӧ���յ�Error��Ϣʱ,Ҫ���յ���Severityֵת��Ϊ��Ӧ����ĸ��ʾ.
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

/* Error ��Ϣ�е�Textnum ���Ե�ȡֵ,��ʾ�ô�����Ҫ��ʾ���ı���Ϣ
��Ӧ���յ�Error��Ϣʱ,Ҫ���յ���m_wTextnumֵת��Ϊ��Ӧע�͵��ı���Ϣ.
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

/*Error��Ϣ�ṹ��,ר���������˵��г��ֵĸ�����Դ��״̬.
���ĸ��������,m_bSeverity��m_wTextnum��������Ҫ����IRDETO_STATUS_SEVERITY_EN
��IRDETO_BAKMSG_TEXT_EN�ҵ���Ӧ���ı���ʾ.m_wStatus��m_bSource��ֱ����ʾ�յ���ֵ.
���յ��ṹ��ֵΪ{2,16,4,8}����ʾ�ı�ӦΪ:E16-4 Severice is not available.
*/
typedef struct _SRVERRINFO_ST
{
	BYTE    m_bSeverity;
	WORD    m_wStatus;
	BYTE    m_bSource;
	WORD    m_wTextnum;
} IRDETO_SRVERRINFO_ST;

/*Error��Ϣ�ṹ��,ר��������Alarm��Ϣ.
m_bSeverity��m_wTextnum��������Ҫ����IRDETO_STATUS_SEVERITY_EN
��IRDETO_BAKMSG_TEXT_EN�ҵ���Ӧ���ı���ʾ.m_wStatus��m_bSource��ֱ����ʾ�յ���ֵ.
���յ��ṹ��ֵΪ{2,16,4,8}����ʾ�ı�ӦΪ:E16-4 Severice is not available.
*/
typedef struct _CA_ERROR_INFO_ST
{
    DWORD   m_wServiceHandle;//��Ҫ��ʾAlarm��Ϣ��Ŀ��ServiceHandle
	BYTE    m_bSeverity;
	WORD    m_wStatus;
	BYTE    m_bSource;
    WORD    m_wTextnum;	
}IRDETO_CA_ERROR_INFO_ST;

/*���ܿ��˵��и������ܿ�������*/
typedef struct _SMARTCARDSTATUS_ST
{
    IRDETO_SRVERRINFO_ST m_stScStatus;//���ܿ���ǰ״̬
    BYTE    m_szScNumber[SC_NUMBER_LEN];//���ܿ�����,�ַ������� 
    BYTE    m_szScType[SC_TYPE_LEN];//���ܿ�����,�ַ�������  
    BYTE    m_szScVer[SC_VER_LEN];//���ܿ��汾,�ַ������� 
    BYTE    m_szNationality[SC_NATION_LEN];//���ܿ�������,�ַ������� 
    WORD    m_wPatchLevel;//���ܿ���������
    WORD    m_wOwnerId;//���ܿ�OwnerID
    BYTE    m_bVariant;//���ܿ��г���
    BYTE    m_bBuild;//���ܿ�Build
}IRDETO_SMARTCARDSTATUS_ST;


/*Irdeto CA ���еĲ˵�����*/
typedef enum _MENU_STATUS_EN
{
	CA_MENU_SOFTCELL_STATUS,//  CA���˵�         
	CA_MENU_SMARTCARD_STATUS,//���ܿ��˵� 
	CA_MENU_DVBSERVICE_STATUS,//DVB��Ŀ���ܲ˵�
	CA_MENU_SERVICEINDEX_STATUS,//	DVB������Ŀ��Ϣ�˵�
	CA_MENU_EMM_SERVICE_STATUS, //EMM ��Ϣ�˵�,��ʱ����Ҫʵ��
	CA_MENU_DES_SERVICE_STATUS,//ECM��Ϣ�˵� ,��ʱ����Ҫʵ��
	CA_MENU_LOADER_STATUS,//Loader �˵�
	CA_MENU_COMPONENTS_STATUS,//����˵�,��ʱ����Ҫʵ��
	CA_MENU_MR_CARD_STATUS,//����MR���ܿ���Ϣ�˵�
	CA_MENU_MAX_DATA
}IRDETO_MENU_STATUS_EN;

/*Irdeto CA �ϴ�Ӧ�õ���Ϣ���*/
typedef enum _CAEEVENTGROUP_EN
{
	CA_ALARM,
	CA_INFO,
	CA_COMMAND,
	CA_STATUS
}IRDETO_CAEEVENTGROUP_EN;

/*CA_STATUS ��Ϣ����������*/
typedef enum _CASTATUS_EN
{
	CA_STATUS_OTA_OK,        //��ʾ��ǰ��OTA������������,�����ṹ��dwParm2 = (DWORD)&T_OtaCfg
	CA_STATUS_OTA_STATUS,    //��ʾ��ǰ�����׶�,�����ṹ��dwParm2 = (DWORD)&IRDETO_OTASTATUS_ST
	CA_STATUS_OTA_FAILURE,   //��ʾOTA����ʧ��,�����ṹ��dwParm2 = (DWORD)&IRDETO_OTAERROR_EN
	CA_STATUS_SMC_IN,		// �������ܿ�, ���Ҹÿ�Ϊ��ȷ��Irdeto��
	CA_STATUS_SMC_OUT,		// �γ����ܿ�
	CA_STATUS_SMC_ERROR		//�Ƿ����ܿ�
	
}IRDETO_CASTATUS_EN;

/*CA_INFO ��Ϣ����������*/
typedef enum _CAINFO_EN
{
	CA_INFO_SOFTCELL_STATUS = 0, //CA ���˵���Ϣ,�����ṹ��ddwParm2 = (DWORD)&IRDETO_CASTATUS_ST 
	CA_INFO_SC_STATUS,           //���ܿ��˵���Ϣ,�����ṹ��ddwParm2 = (DWORD)&IRDETO_SMARTCARDSTATUS_ST 
	CA_INFO_MONI_EMM_STATUS = 2, //������Ŀ�˵���Ϣ�е�EMM������Ϣ,�����ṹ��ddwParm2 = (DWORD)&IRDETO_MONIEMMSTATUS_ST 
	CA_INFO_MONI_DES_STATUS ,    //������Ŀ�˵���Ϣ�е�ECM������Ϣ,�����ṹ��ddwParm2 = (DWORD)&IRDETO_MONIDESSTATUS_ST 
	CA_INFO_SRC_EMM_STATUS = 4,  //������Ŀ�˵���Ϣ�е�EMM��Ŀ��Ϣ,�����ṹ��ddwParm2 = (DWORD)&IRDETO_EMMSRVSTATUS_ST 
	CA_INFO_SRC_ECM_STATUS ,     //������Ŀ�˵���Ϣ�е�ECM��Ŀ��Ϣ,�����ṹ��ddwParm2 = (DWORD)&IRDETO_DESSRVSTATUS_ST 
    CA_INFO_COMPONENTS_STATUS = 6,// �Ѳ�ʹ��
    CA_INFO_SC_PINCODECHANGE_STATUS, //�޸�PIN��,�����ṹ��ddwParm2 = (DWORD)&IRDETO_SCPINCODECHANG_ST 
    CA_INFO_SC_PINCODCHECK_STATUS = 8, //�޸�PIN����״̬,�����ṹ��ddwParm2 = (DWORD)&IRDETO_SCPINCODECHANG_ST 
    CA_INFO_REMOTECHANNEL_STATUS,   //REMOTECHANNEL ����,�����ṹ��ddwParm2 = (DWORD)&IRDETO_REMOTECHANNEL_ST 
	CA_INFO_IRD_STATUS = 10,     //�ݲ�ʹ�� 
	CA_INFO_LOADER_STATUS,       //Loader �˵���Ϣ,�����ṹ��ddwParm2 = (DWORD)&IRDETO_LOADERSTATUS_ST 
	CA_INFO_MEMORY_ERROR = 12,   //�ڴ������Ϣ
	CA_INFO_SC_NUMSECTOR_STATUS, //���ܿ�Sector��Ϣ,�����ṹ��dwParm2 = (DWORD)&IRDETO_SC_NUMBEROFSECTOR_ST
	CA_INFO_CAREGION_STATUS = 14,//���ܿ���������Ϣ,�����ṹ��dwParm2 = (DWORD)&IRDETO_CAREGION_ST
	CA_INFO_SCUSERDATA_STATUS ,  //���ܿ�Sector��Ϣ,�����ṹ��dwParm2 = (DWORD)&IRDETO_SC_USERDATA_ST
	CA_INFO_EMM_USERLOAD_STATUS = 16, //emm user payload data,�����ṹ��dwParm2 = (DWORD)&IRDETO_EMM_USERPAYLOAD_ST
	CA_INFO_DVBSERVICE_STATUS , //CA��Ŀ��Ϣ���ܲ˵�,�����ṹ��dwParm2 = (DWORD)&IRDETO_DVBDESCRABLE_ST
	CA_INFO_MAIL = 18,          //�ʼ���Ϣ,�����ṹ��dwParm2 = (DWORD)&IRDETO_CAMSG_ST
	CA_INFO_IPPVPIN_STATUS,     //IPPV������Ϣ,�����ṹ��dwParm2 = (DWORD)&IppvPurchaseInfo_S
	CA_INFO_IPPV_SECTOR_INFO = 20, //IPPV ĳSector ��Ϣ,�����ṹ��dwParm2 = (DWORD)&IppvSectorInfo_S
    CA_INFO_IPPV_EVENT_LIST,       //IPPV ĳSector Event��Ϣ�б�,�����ṹ��dwParm2 = (DWORD)&IppvEventList_S
	
	CA_INFO_MR_CARD_STATUS,    //�û�����MR���ܿ���Ϣ�˵�ʱ,��Ҫ��ʾ����Ϣ�������ṹ��dwParm2 = (DWORD)&MrCardInfo_S
	CA_INFO_MR_USERPROFILE_DATA,//MR���óɹ��󴫵��û����������Ϣ�������ṹ��dwParm2 = (DWORD)&MrUserProfileData_S

	CA_INFO_MAX
}IRDETO_CAINFO_EN;

typedef enum _CACOMMAND_EN
{
	CA_COMMAND_FINGER_PRINT = 0,         //ECMָ������,�����ṹ��dwParm2 = (DWORD)&IRDETO_CACOVERTFINGERPRINT_ST
	CA_COMMAND_HOMECHANNEL_SET,          //����HomingChannelƵ��,�����ṹ��dwParm2 = (DWORD)&T_DeliverySystem
	CA_COMMAND_HOMECHANNEL_SCAN = 2,     //���·�Ƶ������Ƶ��,�����ṹ��dwParm2 = (DWORD)&T_DeliverySystem
	CA_COMMAND_APP_HOMECHANNEL_SCAN,     //����APP��Ƶ������ 
	CA_COMMAND_RESET_CHANNEL = 4,        //���������Ŀ����,�����ṹ��dwParm2 = (DWORD)BouquetID,��Ϣ��������ΪBouquetIDֵ��APP��Ҫ����BouquetIDֵ���������Ŀ�������ڸ�BouquetIDֵ�Ľ�ĿӦ�����Ρ�
	CA_COMMAND_RECONNECT_CURRENT_SERVICE,//ǿ�Ƹ��½�Ŀ����
	CA_COMMAND_REBOOT = 6,               //��������������
	CA_COMMAND_TUNE_SERVICE,             //ǿ�ƻ�̨����,�����ṹ��dwParm2 = (DWORD)&IRDETO_TUNESVC_ST
	CA_COMMAND_CHANGE_PGPIN = 8,         //�ı�PIN ������,dwParm2 = (DWORD)&IRDETO_CAPIN_ST
	CA_COMMAND_CHANGE_INSTALLPIN,        //ǿ�ƻָ�PIN ������,dwParm2 = (DWORD)&IRDETO_CAPIN_ST
	CA_COMMAND_EMM_OTA = 10,             //ǿ��OTA��������,dwParm2 = (DWORD)&IRDETO_CAEMMOTA_ST
	CA_COMMAND_OTA_NEXT,                 //OTA��NIT��������,��Ƶ����Ϣ�ϴ�Ӧ��,�����ṹ��dwParm2 = (DWORD)&T_OtaCfg
	CA_COMMAND_OTA_DETECT = 12,          //OTA��BAT��������,���Ƿ���������Ϣ��״̬�ϴ�Ӧ��,�����ṹ��dwParm2 = (DWORD)TRUE:��OTA������, False:û��OTA��
	CA_COMMAND_ATTRIB,                   //TMS ��Ϣ,�����ṹ��dwParm2 = (DWORD)&IRDETO_ATTRIBCOVERT_ST
	CA_COMMAND_SURFLOCK = 14,            //SurfLock����,�����ṹ��dwParm2 = (DWORD)IRDETO_SURFLOCK_ST
    CA_COMMAND_IPPVPREVIEW,              //IPPVԤ������,�����ṹ��dwParm2 = ((DWORD)&IppvPreviewInfo_S//amended by Stone 20090112
    CA_COMMAND_MRPIN,                    //Ӧ���ڲ��Ž�Ŀ���������,Ҫ���û�����MR ���룬�����ṹ��dwParm2 = ((DWORD)&MrPinInfo_S
    CA_COMMAND_SET_BISSKEY,              //Ӧ���յ�������������CSCASetBISSSessionKey����
	
	CA_COMMAND_MAX	
}IRDETO_CACOMMAND_EN;

/*Error��Ϣ�ṹ�в��Ϸ���TextNum����*/
#define INVALID_BAKMSG_TEXT         ( 0xFFFF )

/*�Ѳ�ʹ��*/
#define CA_MAX_SIZE_FINGERPRINT      (15)
/*�Ѳ�ʹ��*/

typedef struct _CAFINGERPRINT_ST  //FINGER PRINT
{
	BYTE    	m_cInfoNo;     	//������Ϣ��
	INT     	m_nOvertime;  	 //��ʱ����
	BYTE    	m_acMsg[CA_MAX_SIZE_FINGERPRINT]; //ָ������
}IRDETO_CAFINGERPRINT_ST;

/*ECMָ����Ϣ*/
typedef struct _CACOVERTFINGERPRINT_ST 
{
	BYTE    	m_cInfoNo;     	//������Ϣ��,ĿǰΪ0
	WORD     	m_wDuration;  	 //��ʱ����,��ʾָ����ʾ�ĳ���ʱ��,��λ��.Ϊ0 ��ʾ��ȷ��,һֱ�ȵ���һ��ָ���������ָ��.
	BOOL        m_bFlash;        //�Ƿ���˸,����˸ΪTRUEʱ,����Overted ָ������һ�����˸����.����Covertָ��,��Ҫ��ʾֻ��Ҫ��ʾ����֡����.
    BOOL        m_bBanner;       //ΪTRUEʱ,��Ҫ��ʾ��Ϣ ��������, ������Ҫ������
    BYTE        m_bCoverPer;     //������,��Ϣ��ռ������ʾ��Ļ������ٷֱ�.���,Ҫ����Ϣ��̫С������ʾ����������Ϣ,��������ϢҪ���ض�.��ָ������ΪCovertָ��ʱ,ָ����Ϣ�ı���Ҫ��͸��.
    BOOL        m_bCovert;       //ָ������,ΪTRUEʱ��ʾ��ָ��ΪCovert ָ��. ΪFALSEʱ��ʾ��ָ��ΪOvertָ��.
	BYTE    	m_acMsg[CA_MAX_SIZE_FINGERPRINT]; //ָ������,�ַ�������
	/*---------------------------------------
	��������ֻ�е���EOF������,���� m_bCovert==FALSE ʱ�������壬�������  
	-----------------------------------------*/
	BYTE		m_ucLocationX;	//������λ��
    BYTE		m_ucLocationY;   //������λ��
    BYTE        m_ucBgTransAlpha; // ����͸����
    DWORD	    m_dwBgColour;		//������ɫ
    BYTE        m_ucFontTransAlpha; // ����͸����
    DWORD	    m_dwFontColour;	  //������ɫ
    BYTE		m_ucFontType;	  //��������

}IRDETO_CACOVERTFINGERPRINT_ST;


/*Irdeto CA PIN ��*/
typedef struct  _CAPIN_ST
{
	BYTE*   m_pcPin; //PIN ��
	BYTE    m_cPinLen;//PIN �볤��
}IRDETO_CAPIN_ST;

/*EMM ǿ����������*/
typedef struct _CAEMMOTA_ST
{
	BOOL    m_bDownLoadAllowed; //�Ƿ���������
	BOOL    m_bForced;  //�Ƿ�Ϊǿ��
}IRDETO_CAEMMOTA_ST;

/*�����е��ʼ�����*/
typedef enum _TXTTYPE_ST
{
	CA_TXT_MAIL         	= 0,//Text -- Mailbox,	�ʼ���Ϣ
	CA_TXT_ANNOUNCE     	= 1,//Text -- Announcement,�㲥֪ͨ
	CA_TXT_CLUB         	= 2 //Club number,��Ա��
}IRDETO_TXTTYPE_ST;

/*��δʹ��*/
typedef enum _TXTSHOWMODEL_EN
{
	CA_TXT_SHOW_NORMAL		= 0x00,		//Normal
	CA_TXT_SHOW_TIMED		= 0x01,		//Timed(automatic erase after date/time)
	CA_TXT_SHOW_FORCED		= 0x02		//Forced Display
}IRDETO_TXTSHOWMODEL_EN;

/*�ʼ�������󳤶�*/
#define CA_MAX_SIZE_TXT_MAIL       (512)

/*�ʼ�����ʱ����Ϣ*/
typedef struct _CADATETIME_ST
{
	DWORD   m_dwYear;//��
	BYTE    m_cMonty;//��
	BYTE    m_cDay; //��
	BYTE    m_cHour; //ʱ
	BYTE    m_cMinute; //��
}IRDETO_CADATETIME_ST;

/*�ʼ���Ϣ����*/
typedef struct _CAMSG_ST
{
	BYTE    	m_cType; //T_TxtType// 0: Text_Mail  1: Text_Announcement 
	BYTE    	m_cClass;//T_TxtShowModel// 0: Normal     1: Timeed       2: ForcedDisplay
	BYTE    	m_cPriority; //�ʼ����ȼ�
	DWORD   	m_dwLen; // �ʼ�����
	IRDETO_CADATETIME_ST  m_stDeleteTime; 	//��TIMED�ʼ��д���
	BYTE    m_szContent[CA_MAX_SIZE_TXT_MAIL];//�ʼ�����,�ַ�������
    WORD    m_wDuration;//for TMS mail  ��ʾʱ��
    BYTE    m_bCoverPer; //for TMS mail ������
}IRDETO_CAMSG_ST;

/* �����е��ʼ���Ϣ*/
typedef struct _CAMAIL_ST		
{	
	BOOL    m_bReadFlag;          	//�Ѷ���־
	BOOL    m_bLockFlag;	      	 //�ʼ�������־
	IRDETO_CADATETIME_ST  m_stArriveTime; //�ʼ������ʱ��
	IRDETO_CAMSG_ST m_stMsg; //�ʼ�����
}IRDETO_CAMAIL_ST;

 /*OTA���������׶�,���ý׶λ��ж�����ʣ��ʱ��ͻᳬʱ.
 Stepֵ:
 0  ��������NIT��
 1  ��������PAT��
 2  ��������PMT��
 3  ��������PID
 */
typedef struct _OTASTATUS_ST  
{
	BYTE    m_cStep;	//OTA�׶�
	BYTE    m_cTimeLeft;//ʣ��ʱ��
}IRDETO_OTASTATUS_ST;

/* OTA ʧ������*/
typedef enum _OTAERROR_EN     
{
	CA_OTA_ERROR_1,//NIT��û��������
	CA_OTA_ERROR_2// PAT,PMT ������PIDû��������
}IRDETO_OTAERROR_EN;

/*��Ŀ��Ϣ�ṹ�壬��StartCA,StopCaʹ��*/
typedef struct _CAFTASERVICE_ST	
{
	WORD    m_wDemuxID;        //DMX ID��
	WORD    m_wServiceIndex;   //index
	WORD    m_wOrginalNetworkID;//ԭʼ����ID��
	WORD    m_wTsID;           //��ID��
	WORD    m_wServiceID;      //��ĿID��
	BYTE	m_bESStreamCount;  //ES ����
	WORD   m_wESStreamPid[32]; //ES pid
	WORD    m_wServiceHandle; //Service Handle
	DWORD   m_dwUserData;        	//Ԥ��
}IRDETO_CAFTASERVICE_ST;

/*���¹�������˵��ĺ궨�弰�ṹ���ݲ�ʹ��start*/

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

/*���Ϲ�������˵��ĺ궨�弰�ṹ���ݲ�ʹ��end*/

/* Irdeto ���˵���Ϣ*/
typedef struct _CASTATUS_ST
{
	BYTE	m_bMajor;// Softcell Major version
	BYTE	m_bMinor;// Softcell Mini Version
	DWORD   m_uSubVersion; //Softcell SubVersion
	CHAR	m_acNameString[CPNT_STATUS_NAME_LEN];// Softcell  name,�ַ�������
	CHAR	m_acBuildInfoString[CPNT_STATUS_DATE_LEN]; //Softcell  Build Info,�ַ�������
	CHAR	m_acCaTaskVerString[CPNT_STATUS_VERSION_LEN];//CA Task Version,�ַ�������
}IRDETO_CASTATUS_ST;

/*Emm�˵�����������ʾ����Դ��*/
#define     MAX_SERVICE_RESOURCES           32

/*Emm����������ֽ�*/
#define     MAX_SRC_EMM_MONITOR_LEN         64//changed by zhentiejun 081110

/*������Ŀ�˵���EMM��Ŀ��Ϣ*/
typedef struct _EMMSRVSTATUS_ST
{
	DWORD           m_wEmmHandle; //Emm handle
	WORD            m_wServiceHandle; //Service Handle
	IRDETO_SRVERRINFO_ST    m_stGlobalStatus; //Global Status
	IRDETO_SRVERRINFO_ST    m_stServiceStatus; // Service Status
	WORD            m_wEmmPid; //EMM pid
	BOOL			m_fEmmServiceDefine;//Emm severvice �Ƿ񱻶���
	BYTE			m_bRscCount;//��Դ��
	DWORD           m_awRscType[MAX_SERVICE_RESOURCES];//��Դ�����б�
	IRDETO_SRVERRINFO_ST    m_stRscStatus[MAX_SERVICE_RESOURCES];//��Դ״̬
    BYTE            m_acEmmString[MAX_SRC_EMM_MONITOR_LEN];//EMM �����ֽ�,16������
    DWORD           m_dwEmmResourceID; // added by zhentiejun 081127 EMM ��ԴID
}IRDETO_EMMSRVSTATUS_ST;

/*Ecm����������ֽ�*/
#define     MAX_SRC_ECM_MONITOR_LEN         64//changed by zhentiejun 081110

/*Ecm�˵�����������ʾ��ES��*/
#define     MAX_ECMSERVICE_STREAMS          32

/*������Ŀ�˵���ECM��Ŀ��Ϣ*/
typedef struct _DESSRVSTATUS_ST
{
	DWORD		m_wGroupHandle;// Group Handle
	DWORD		m_wServiceHandle;//Service Handle
	DWORD		m_wServiceType;// service  ���
	IRDETO_SRVERRINFO_ST    m_stGlobalStatus;//Global Status
	IRDETO_SRVERRINFO_ST    m_stServiceStatus;//Service Status
	BYTE		m_bStreamCount;// ES ����
    WORD        m_wStrInfoStreamPid[MAX_ECMSERVICE_STREAMS];//ES pid
    WORD    	m_wStrInfoEcmPid[MAX_ECMSERVICE_STREAMS];//ECM pid
	IRDETO_SRVERRINFO_ST    m_stStrInfoStatus[MAX_ECMSERVICE_STREAMS];//ES ״̬	
	BYTE		m_bRscCount;//��Դ��
	WORD        m_awRscType[MAX_SERVICE_RESOURCES];//��Դ����
	IRDETO_SRVERRINFO_ST    m_stRscStatus[MAX_SERVICE_RESOURCES];//��Դ״̬
    BYTE		m_abEcmData[MAX_SRC_ECM_MONITOR_LEN];//ECM ��������,16������
    WORD        m_abEcmMonitorCount;// added by zhentiejun 081126  //ECM ��������
    DWORD       m_dwEcmResourceID; // added by zhentiejun 081127 // ECM ��ԴID
}IRDETO_DESSRVSTATUS_ST;

/* ������Ŀ��Ϣ��EMM �ļ�����Ϣ */
typedef struct _MONIEMMSTATUS_ST
{
	DWORD       m_wEmmHandle;//EMM handle
	WORD        m_wServiceHandle;//EMM ��Ӧ��Service Handle	
	BYTE        m_acEmmString[MAX_SRC_EMM_MONITOR_LEN];//EMM �����ֽ�,16������
}IRDETO_MONIEMMSTATUS_ST;

/* ������Ŀ��Ϣ��ECM �ļ�����Ϣ */
typedef struct _MONIDESSTATUS_ST
{
	DWORD       m_wGroupHandle;// Group Handle
	WORD        m_wServiceHandle;// Service Handle
	WORD        m_wEcmPid;// added by zhentiejun 081126 //ECM pid
	BYTE        m_abEcmData[MAX_SRC_ECM_MONITOR_LEN];// ECM ��������,16������
}IRDETO_MONIDESSTATUS_ST;

/*Irdeto CA �г��ֵ�����ʱ����Ϣ*/
typedef struct _DATETIME_ST
{
    BYTE    m_bYear;//��
    BYTE    m_bMonth;//��
    BYTE    m_bDate;// ��
    BYTE    m_bHour;//ʱ
    BYTE    m_bMintue;//��
    BYTE    m_bSecond;//��
}IRDETO_DATETIME_ST;

/* CA Loader�˵���Ϣ*/
typedef struct _LOADERSTATUS_ST
 {
    BYTE        m_bManufacturerId;//���̺�
    BYTE        m_bHardwareVersion;//Ӳ���汾��
    WORD        m_bVariant;//�г���

    WORD       m_wSystemId;//System ID
    WORD        m_wKeyVersion;//key �汾
    WORD        m_wSignatureVersion;//ǩ�ְ汾

    DWORD       m_dLdSECSSN;// chip ID

    BYTE        m_bDownLoadID;// ���غ�
    //--------------------------------------------------
    BYTE        m_bFirmVersion[CA_MAX_SIZE_FIRMWARE];//Firm Version,�ַ�������

    BYTE        m_bLoadVersion[CA_MAX_SIZE_LOASER];//Load Version,�ַ�������

    BYTE        m_bSerialNO[CA_MAX_SIZE_IRD_SERIALNUMBER];// ���к�,�ַ�������

    WORD        m_wSpecInfo;//Spec ��Ϣ
    IRDETO_DATETIME_ST m_stSignDateTime;//ǩ������
    IRDETO_DATETIME_ST m_stDownloadDateTime;//��������
    
}IRDETO_LOADERSTATUS_ST;

/*for Irdetoca smartcard pin code change/check, only supportd ippv */
#define MAX_SC_PINCODE_LEN 2

/*�޸�PIN��*/
typedef struct _SCPINCODECHANG_ST
{
    DWORD       m_wSCResourceId;//��Դ��
	BYTE        m_bCodeIndex;//PIN ������
    BYTE        m_OldPinCode[MAX_SC_PINCODE_LEN];//��PIN ��,16������
    BYTE        m_NewPinCode[MAX_SC_PINCODE_LEN];//��PIN ��,16������
    BYTE        m_bCodeStatus;//PIN ״̬
}IRDETO_SCPINCODECHANG_ST;

/*for Irdetoca remote channel */
#define MAX_PAYLOAD_LEN 256

/*ʵ�� REMOTECHANNEL ����*/
typedef struct _REMOTECHANNEL_ST
{
	DWORD       m_wResourceId;//��Դ��   
	BYTE        m_bPayloadLengthMax;//������󳤶�
	BYTE        *pbPayload; // ����
}IRDETO_REMOTECHANNEL_ST;

#define MAX_SC_NUMSECTOR_SIZE 8 //���ܿ�Sector����������

/* ���ܿ�sector������Ϣ*/
typedef struct _SC_NUMBEROFSECTOR_ST
{
    DWORD       m_wResourceId;//��Դ��
    BYTE        m_bsectorCount;//sector ����
    BYTE        m_bSector[MAX_SC_NUMSECTOR_SIZE];//Sector ����,16������
    BYTE        m_bActive[MAX_SC_NUMSECTOR_SIZE];//sector�Ƿ񼤻�,16������
    BYTE        m_bReserverd[MAX_SC_NUMSECTOR_SIZE];//�Ƿ�ΪԤ��,16������
}IRDETO_SC_NUMBEROFSECTOR_ST;

/*���ܿ���������Ϣ*/
typedef struct _CAREGION_ST
{
	DWORD       m_wResourceId;//��Դ��
	BYTE        m_bRequestType;//�������� 
	BYTE		m_bSector;//Sector ����    
	BYTE		m_bSectorStatus;//sector ״̬
	BYTE		m_bRegion;//������        
	BYTE		m_bSubRegion;//��������  
}IRDETO_CAREGION_ST;

#define MAX_SC_USERINFO_LEN 28//���ܿ������û���Ϣ����ֽ���

typedef struct _SC_USERDATA_ST
{
	DWORD     	m_wResourceId;//��Դ��
	BYTE        m_abUserData[MAX_SC_USERINFO_LEN]; //�û���Ϣ,16������
}IRDETO_SC_USERDATA_ST;

/*for Irdetoca surflock status*/
typedef struct _SURFLOCK_ST
{
	BYTE        m_bRequestType;//��������
	DWORD       m_wResourceId;//��Դ��
	BOOL        m_fSurfLocked;//�Ƿ��Ѿ�SurfLock
	DWORD       m_wGroupHandle;//GroupHandle
} IRDETO_SURFLOCK_ST;

#define MAX_EMM_USERLOAD_LEN 256//emm user payload ��ֽ���

/*for Irdetoca emm user payload data */
typedef struct _EMM_USERPAYLOAD_ST
{
	DWORD           m_wEmmHandle;//EMM handle			
	WORD            m_wMessageLength;//��Ϣ����
	BYTE            m_abMsgData[MAX_EMM_USERLOAD_LEN];//������Ϣ,16������
} IRDETO_EMM_USERPAYLOAD_ST;

/*TMS��Ϣ������ֽ���*/
#define MAX_ATTRIBDATA_LEN 511

/*TMS��Ϣ����*/
typedef enum _TMSTYPE_EN
{
	CA_TMS_NORMALMAIL ,  //��ͨ�ʼ�         
	CA_TMS_FORCEDMAIL ,  //�����ʼ�,Ҳ��ǿ����ʾ�ʼ�         
	CA_TMS_FINGERPRT ,   //ָ��        
	CA_TMS_MAXTYPE  
}IRDETO_TMSTYPE_EN;

/*�Ѳ�ʹ��*/
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

/*TMS ��Ϣ*/
typedef struct _ATTRIBCOVERT_ST
{
	BYTE        m_bIndex;// TMS ��Ϣ������				
	DWORD       m_wEmmHandle; //EMM handle    
	WORD        m_wServiceHandle;//Service Handle            
	WORD        m_wMsgLength; //TMS ��Ϣ����
	BYTE        m_bMessageData[MAX_ATTRIBDATA_LEN];//TMS ��Ϣ����,�ַ�������
	IRDETO_TMSTYPE_EN        m_bType;//TMS ��Ϣ����.0:��ͨ�ʼ� 1:ǿ���ʼ� 2:ָ��
	BOOL        m_bFlash;	 //�Ƿ���˸,����˸ΪTRUEʱ,����Overted ָ������һ�����˸����.����Covertָ��,��Ҫ��ʾֻ��Ҫ��ʾ����֡����.
    BOOL        m_bBanner;   //ΪTRUEʱ,��Ҫ��ʾ��Ϣ ��������, ������Ҫ������
    BYTE        m_bCoverPer;//������,��Ϣ��ռ������ʾ��Ļ������ٷֱ�.���,Ҫ����Ϣ��̫С������ʾ����������Ϣ,��������ϢҪ���ض�.��ָ������ΪCovertָ��ʱ,ָ����Ϣ�ı���Ҫ��͸��.
	WORD        m_wDuration;//��ʱ����,��ʾָ����ʾ�ĳ���ʱ��,��λ��.Ϊ0 ��ʾ��ȷ��,һֱ�ȵ���һ��ָ���������ָ��
	BOOL        m_bCovert;  //������m_bTypeΪָ��ʱ,m_bCovert��ʾָ������,ΪTRUEʱ��ʾ��ָ��ΪCovert ָ��. ΪFALSEʱ��ʾ��ָ��ΪOvertָ��.
	/*---------------------------------------
	��������ֻ�е�m_bType �� 2���� m_bCovert == FALSE&&g_bIsEof== TRUE ʱ�������壬�������(EOF����)   
	-----------------------------------------*/
	BYTE		m_ucLocationX;	//������λ��
    BYTE		m_ucLocationY;   //������λ��
    BYTE        m_ucBgTransAlpha; // ����͸����
    DWORD	    m_dwBgColour;		//������ɫ
    BYTE        m_ucFontTransAlpha; // ����͸����
    DWORD	    m_dwFontColour;	  //������ɫ
    BYTE		m_ucFontType;	  //��������

} IRDETO_ATTRIBCOVERT_ST;

/*ִ��ǿ�ƻ�̨����ʱ,ǿ�ƻ�̨�Ĳ�����Ϣ*/
typedef struct _TUNESVC_ST
{
	DWORD       m_wGroupHandle;//ǿ�ƻ�̨��Service ��GroupHandle
	WORD        m_wNetworkID;//ǿ�ƻ�̨��Service �� Network ID
	WORD        m_wTransportStreamID;//ǿ�ƻ�̨��Service �� TS ID
	WORD        m_wServiceID;//ǿ�ƻ�̨��Service �� ServiceID
}IRDETO_TUNESVC_ST;

/*Severice����*/
typedef enum _SERVICETYPE_EN
{
    SERVICE_EMM ,//EMM ����
    SERVICE_DES ,//ECM ����
    SERVICE_MAX
}IRDETO_SERVICETYPE_EN;

/*CA ��Ŀ���ܲ˵���ÿ����Ŀ����Ϣ*/

typedef struct _DVBDESCRABLE_INFO_ST
{
    DWORD   m_dwClientHandle;//��Ŀ��Ӧ��Handle
    WORD    m_wIndex;//��Ŀ�ڲ˵��е�����
    IRDETO_SERVICETYPE_EN  m_eServiceType;//Service����
    BYTE    m_bInstance; //Instance ֵ  
}IRDETO_DVBDESCRABLE_INFO_ST;

/*CA ��Ŀ���ܲ˵���Ϣ*/
typedef struct
{
    BYTE    m_bServiceCount;//��Ŀ����
    IRDETO_DVBDESCRABLE_INFO_ST m_stDVBServiceInfo[CA_DEMUX_MAX+CA_SRV_MAX];
}IRDETO_DVBDESCRABLE_ST;

/*�����жϵ�ǰ��Ŀ����*/
typedef enum _ESCURRENTTYPE_EN
{
	CA_ES_IRDETO ,//Irdeto���Ž�Ŀ
	CA_ES_OTHER , //��Irdeto���Ž�Ŀ
	CA_ES_FREE,   //����
	CA_ES_MAX
}IRDETO_ESCURRENTTYPE_EN;

/*��֧�ֵĹ��������*/
#define CA_MAX_COUNTRY_NUM         (20)

/*������������Ϣ*/
typedef struct _COUNTRYBL0CK_INFO_ST
{
	BYTE    m_cFlag;        //���ñ�־��1�����ã�0��������
	BYTE  	m_acCountry[3]; //���Ҵ���,3����ĸ��ASCII ��
}IRDETO_COUNTRY_INFO_ST;

/*�������б�*/
typedef struct _COUNTRYBL0CK_ST
{
    WORD    m_wCountryNum;    //�ṹ�й��Ҵ������
	IRDETO_COUNTRY_INFO_ST  	m_acCountryList[CA_MAX_COUNTRY_NUM]; //��ŵĹ��Ҵ���
}IRDETO_COUNTRYBL0CK_ST;

/*IPPV Ԥ����Ϣ*/
typedef struct _IppvPreviewInfo
{
	DWORD    m_dwResourceId;//��Դ��
	BYTE     m_ucSector;//IPPV Event ���ڵ�Sector����
	WORD     m_wEventId;//EventId
	WORD     m_wEventCost;//Event����
	BYTE     m_ucClockValue;//��ClockValue < PreviewValueʱ,IPPV����
	BYTE     m_ucPreviewValue;//��ĿԤ���׶�,CW������������,��������ۿ�
	WORD     m_wServiceCount;//���ڹۿ���Event��Service��
}IppvPreviewInfo_S;

/*ȷ�Ϲ���IPPV��Ŀ��Ϣ*/
typedef struct _IppvPurchaseInfo
{
	DWORD    m_dwResourceId;//��Դ��
	DWORD    m_dwSerHandle;//ȷ�Ϲ����ServiceHandle
	BYTE     m_ucSector;//IPPV Event ���ڵ�Sector����
	WORD     m_wEventId;//EventId
	BYTE     m_aucIppvPin[2];//�û����������,16������
	BYTE     m_ucScPinStatus;//PIN��У��״̬
}IppvPurchaseInfo_S;

/*IPPV PIN ��У��״̬*/
typedef enum
{
	CA_IPPV_BUY_SUCCESS = 1,//PIN��У��ɹ�
	CA_IPPV_BUY_EVENT_NOT_AVAILABLE,//Event ������
	CA_IPPV_BUY_PIN_FAIL = 0x51,//PIN��У��ʧ��
	CA_IPPV_BUY_PIN_BLOCK = 0x54,//PIN�뱻����
	CA_IPPV_BUY_STORE_FULL = 0x9b,//Event�洢����
	CA_IPPV_BUY_NO_CREDIT = 0x9c// ����
	
}IppvPinStatus_e;

/*IPPV Sector ��Ϣ��ѯ*/
typedef struct _IppvSectorInfo
{
	DWORD    m_dwResourceId;//��Դ��
	BYTE     m_ucSector;//Sector ��
	WORD     m_wDebitLimit;//��Sector�Ľ������
	WORD     m_wDebitThreshold;//��Sector�Ľ�Ǿ���ֵ
	WORD     m_wOperatorPurse;//OperatorǮ��
	WORD     m_wCustomerPurse;//CustomerǮ��
	BYTE     m_ucMaxIppvEvents;//��Sector�ɴ洢��Event��
}IppvSectorInfo_S;

/*IPPV ĳSector�Ľ���ٽ���Ϣ*/
typedef struct _IppvThresholdInfo
{
	DWORD    m_dwResourceId;//��Դ��
	BYTE     m_ucSector;//Sector ��
	WORD     m_wDebitThreshold;//��Sector�Ľ�Ǿ���ֵ
}IppvThresholdInfo_S;


typedef struct
{
	WORD     	m_wIPPVEventID;//EventID
	BOOL		m_bEventReported;//�Ƿ��Ѿ��ϱ�ǰ��.1:�Ѿ��ϱ�ǰ��.2:��δ�ϱ�
	WORD     	m_wDateCode;//Event��������
	WORD     	m_wEventCost;//Event�������
}IppvEventRecord_S;

/*IPPV ĳSector�д洢��Event�б���Ϣ*/
typedef struct
{
	DWORD	      	    m_dwResourceId;//��Դ��
	BYTE                m_bSector;//Sector ��
	BYTE     	        m_bDaysGrace;//��ʾ���Բ�ѯ��ʱ���.��ֵΪ�����ѯ������Կ�Խ���������
	BYTE 		        m_bStatus;//�б�״̬,0:m_pstEventRecords������������ 1:m_pstEventRecords������û������
	BYTE     		    m_bIPPVEventCount;//��Sector�洢��Event��
	IppvEventRecord_S   m_pstEventRecords[CA_IPPV_EVENT_MAX];//Event��Ϣ��¼
}IppvEventList_S;

/*����Ҫʵ����������Ź���ʱ,CA���ظ�Ӧ�õĵ�ǰ����״̬*/
typedef enum 
{
    EM_CA_SHUT_BOTH_AV,//����Ƶȫ�ر�
    EM_CA_SHUT_AUDIO, //ֻ�ر���Ƶ
    EM_CA_SHUT_VIDEO  //ֻ�ر���Ƶ
}E_CaCompLevelType;

/*��ѯ���������ر�MR���ܵȲ����ķ��ؽ����Ϣ*/
typedef enum
{
	EM_CA_MR_CONFIG_OK			= 0,//�����ɹ�
	EM_CA_MR_CONFIG_CARD_ERROR,     //���ܿ�����
	EM_CA_MR_CONFIG_CARD_MR_INCAPABLE,//���ܿ���֧��MR����
	EM_CA_MR_CONFIG_PIN_WRONG,//Subscriber PIN����
	EM_CA_MR_CONFIG_PIN_BLOCK,//Subscriber PIN������
	EM_CA_MR_CONFIG_DISABLE_MR_NOT_ALLOWED//������ر�MR����
} E_MrConfigResult;

/*���û�����MR���ܿ���Ϣ�˵�ʱ,CA���ظ�Ӧ����ʾ����Ϣ*/
typedef struct 
{ 
	IRDETO_SRVERRINFO_ST    m_stCardStatus;//���ܿ�״̬
	BOOL  m_bCapablity; //���ܿ��Ƿ�֧��MR����
	BOOL  m_bEnable; //���ܿ�MR�����Ƿ���
	E_MrConfigResult  m_eResult;/*���û���ѯ���������ر�MR����ʱ,CA���ظ�Ӧ�õ����ý��*/
}MrCardInfo_S;


/*APP���ݵ�Subscriber�����Լ���ı�Ŀ�״̬*/
typedef struct
{ 
	BYTE m_aucSubscriberPin[ 2 ]; //Subscriber����
	BOOL m_bEnable; //������ر�MR����,1:������2:�ر�
}MrConfiguration_S;

/*profile����*/
typedef enum
{
	EM_CA_MR_USER_PROFILE_READ = 0,//profile�鿴
	EM_CA_MR_USER_PROFILE_SET,        //profile����
	EM_CA_MR_USER_PROFILE_DELETE   //profileɾ��
	
}E_MrProfileAction;

/*Profile�������ؽ����Ϣ*/
typedef enum
{
	EM_MR_USER_PROFILE_OK			= 0,//�ɹ�
	EM_MR_USER_PROFILE_CARD_ERROR,//���ܿ�����
	EM_MR_USER_PROFILE_CARD_MR_INCAPABLE,//���ܿ���֧��MR����
	EM_MR_USER_PROFILE_SUBSCRIBER_PIN_WRONG,//Subscriber PIN����
	EM_MR_USER_PROFILE_SUBSCRIBER_PIN_BLOCK,//Subscriber PIN������
	EM_MR_USER_PROFILE_UPDATE_NOT_ALLOWED,//���������ò���
       EM_MR_USER_PROFILE_USER_ID_OUT_OF_RANGE, //Profile ID������Χ
       EM_MR_USER_PROFILE_USER_NOT_EXIST, //Profile������
       EM_MR_USER_PROFILE_USER_AGE_OUT_OF_RANGE//�������ó�����Χ
} E_MrProfileResult;

/*Profile�������ظ��û������伶�����롢���������*/
typedef struct
{
	BYTE	m_ucProfileID;
	BYTE	m_ucUserAge;
	BYTE	m_aucUserPin[ 2 ];//���ظ��û���UserPin
	E_MrProfileResult  m_eResult;   /*MR Profile�����ķ��ؽ��*/
	
}MrUserProfileData_S;

/*����Subscriber������,�����Լ��û����õ����������*/
typedef struct 
{ 
	BYTE  m_aucSubscriberPin[ 2 ]; //Subscriber����
	E_MrProfileAction  m_eAction;    //profile����
	MrUserProfileData_S  m_stUserProfile;//profile����
 
}MrProfile_S;

/*�����û������伶�����������,������֤�Ƿ���Ȩ�޹ۿ�*/
typedef struct 
{ 
  DWORD m_dwGroupHandle; //
  DWORD m_dwServiceType; //
  BYTE  m_aucMRPin[2];//�����MR����
  DWORD m_dwResourceId;// 
}MrPinInfo_S;

//REBOOT������Դ
typedef enum 
{
	EM_IRDETO_REBOOTOTA = 0 ,  //OTA
	EM_IRDETO_REBOOTEMM ,  //EMM 
	EM_IRDETO_REBOOTTYPEMAX  
}RebootType_E;


typedef struct _CaFlashAddress
{
    BOOL  m_bCaFlashAddressFlag;  //TRUE: ��ʽapp;FALSE: TestApp
    DWORD m_dwMainFlashAddr;      //��Flash ��ַ
    DWORD m_dwMainFlashLength;    //��Flash ���� 
    DWORD m_dwBackupFlashAddr;    //����Flash  ��ַ����ʹ��ֱ�Ӵ���0
    DWORD m_dwBackupFlashLength;  //����Flash  ��������ʹ��ֱ�Ӵ���0
    
    DWORD m_dwReserve1;            //���������ĳЩCA ��������ĵ�ַ
    DWORD m_dwReserve2;            //���������ĳЩCA ��������ĵ�ַ
    DWORD m_dwReserve3;            //���������ĳЩCA ��������ĵ�ַ
    DWORD m_dwReserve4;            //���������ĳЩCA ��������ĵ�ַ
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
*   ��ȡCA�汾��Ϣ
*
*Parameters
*   pacVerBuf[out]:ָ��洢CA�汾��Ϣ�Ŀռ�
*   dwBufSize[in]:����һ����󳤶�ֵ.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CA_GetCACompileTime
*   ��ȡCA�汾��Ϣ
*
*Parameters
*   pacVerBuf[out]:ָ��洢CA����ʱ��Ŀռ�
*   dwBufSize[in]:����һ����󳤶�ֵ.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetCACompileTime(char* pComTimeBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CA_Init
*   ��ʼ��CAģ��
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
*  ����һ����Ŀ¼��ͨ��
*
*Parameters 
*	nServiceID [in]:
*		��Ŀ��
*	nVideoPID[in]:
*		��ƵPID
*	nAudioPID[in]:
*		��ƵPID
*	cDemux[in]:
*		ͨ���ţ�Ŀǰ֧֮�ֶ�·ͨ����
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_NewPmtSectionNotify (BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewCATSectionNotify
*  ֪ͨCA��CAT����
*
*Parameters 
*	pcSectionData[in]:
*		CAT ���ݻ�������ַ 
*     nLen [in]:
*		CAT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewCatSectionNotify (BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name:CS_CA_NewBATSectionNotify
* ֪ͨCA��BAT����
*
*Parameters 
*	pcSectionData [in]:
*		BAT ���ݻ�������ַ 
*	nLen [in]:
*		BAT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewBatSectionNotify (BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewNitSectionNotify
*  ֪ͨCA��CAT����
*
*Parameters 
*	pcSectionData[in]:
*		CAT ���ݻ�������ַ 
*     nLen [in]:
*		CAT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewNitSectionNotify (BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* ����һ���ʼ�
*
*Parameters 
*	pstTxt[in]:�ʼ�����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Add(IRDETO_CAMAIL_ST* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Del
* ɾ��ĳһ���ʼ�
*
*Parameters 
*	nIndex[in]:�ʼ��±꣬��0��ʼ
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Del(DWORD dwIndex);

/**************************************************************************
*Function Name:CS_CA_TXT_DelAll
* ɾ�����б�������ʼ�
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_DelAll(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetNum
* ��ȡ���ʼ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* ��ȡ����ĳһ���ʼ�������
*
*Parameters 
*	nIndex[in]:�ʼ��±꣬��0��ʼ
*	ppstTxt[out]:�����ʼ��Ĵ洢��ַ
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, IRDETO_CAMAIL_ST** ppstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNew
* ��ѯĳһ���ʼ��Ƿ���δ���ʼ�
*
*Parameters 
*	eType[in]:�ʼ�����
*
*Return Values: 
*	1: TRUE 
*	0: FALSE
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(IRDETO_TXTTYPE_ST eType);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* ��ȡδ���ʼ�����
*
*Parameters 
*	��
*
*Return Values: 
*	����δ���ʼ�����
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* �������ʼ����浽flash�У�һ���ǹػ���ϵ�ǰ���еĲ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_Txt_SetReadFlag
* �����ʼ��Ƿ��Ѷ���־
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	bRead[in]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_SetLockFalg
* �����ʼ��Ƿ�����
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	bLockFlag[in]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_SetLockFalg(DWORD dwIndex, BOOL bLockFlag);

/**************************************************************************
*Function Name:CS_CA_TXT_GetLockFalg
* �õ�ָ���ʼ�������״̬
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	pbLockFlag[out]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_GetLockFalg(DWORD dwIndex, BOOL* pbLockFlag);

/**************************************************************************
*Function Name:CS_CA_OTA_GetTunerParm
* ��ȡ����ʱ����Ĳ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
void CS_CA_OTA_GetTunerParm(void);

/**************************************************************************
*Function Name:CS_CA_OTA_GetUpdateInfo
* ��ȡ���������н�������ʾ����Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
void CS_CA_OTA_GetUpdateInfo(void);

/**************************************************************************
*Function Name:CS_CA_OTA_SaveCfg
* �������������Ϣ
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
*  APP����CA��������
*
*Parameters 
*	IRDETO_MENU_STATUS_EN:  ��������
*   uParam:  ����������������
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_EnterIrdetoMenu( IRDETO_MENU_STATUS_EN enumQueryDataType, DWORD dwParam );

/**************************************************************************
*Function Name: CS_CA_ExitIrdetoMenu
*  APP�Ƴ�����CA��������
*
*Parameters 
*	IRDETO_MENU_STATUS_EN:  �Ƴ�����
*   uParam:  ����������������
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_ExitIrdetoMenu( IRDETO_MENU_STATUS_EN enumQueryDataType );

/**************************************************************************
*Function Name: CS_CA_GetFlashMailAddress
*  ��ȡCA����ʼ�flash�׵�ַ�ʹ�С
*
*Parameters 
*	pFlashMailAddr:     �ʼ��׵�ַ
*   pFlashMailSize:     �ʼ���С
*
*Return Values: 
*		��
**************************************************************************/
void CS_CA_GetFlashMailAddress(DWORD* pFlashMailAddr, DWORD* pFlashMailSize);

/**************************************************************************
*Function Name: CS_CA_SetCountryBlock
*  ��ȡ��ǰ��Ŀ�Ĺ������б���Ϣ
*
*Parameters 
*	pstCountypList:  �������б���Ϣ
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_SetCountryBlock (IRDETO_COUNTRYBL0CK_ST* pstCountypList);

/**************************************************************************
*Function Name: CS_CA_SetSecureCWFlag
*  �Ƿ����߼���ȫ����
*
*Parameters 
*	bFlag:  TREU�����߼���ȫ;FALSE������.
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_SetSecureCWFlag(BOOL bFlag);

/**************************************************************************
*Function Name: CS_CA_FAKE_NVM_EraseAllFlash
* ����CA��Ӧ��FLASH
*
*Parameters 
*	��
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
DWORD CS_CA_FAKE_NVM_EraseAllFlash(void);

/**************************************************************************
*Function Name: CSCAPurchaseIppv
* ȷ�Ϲ���IPPV��Ŀ
*
*Parameters 
*	pstIppvInfo : ȷ�������IPPV�����Ϣ
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CSCAPurchaseIppv(IppvPurchaseInfo_S* pstIppvInfo);

/**************************************************************************
*Function Name: CSCAGetIppvSectorInfo
* ��ȡIPPV��Ŀ��Ӧ�Ŀ���sector��Ϣ
*
*Parameters 
*	dwResourceId : ����resource ID
*     bSector:    Ҫ����Ŀ���sector��  
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CSCAGetIppvSectorInfo(DWORD dwResourceId, BYTE bSector);

/**************************************************************************
*Function Name: CSCASetIppvThreshold
* �޸�͸֧����
*
*Parameters 
*	pstIppvThrInfo: ��Ӧ����sector��,resourceID,�Լ�����ֵ
*      
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CSCASetIppvThreshold(IppvThresholdInfo_S* pstIppvThrInfo);

/**************************************************************************
*Function Name: CSCAGetIppvEventList
* ��ȡ����Event�б���Ϣ
*
*Parameters 
*	dwResourceId : ����resource ID
*     bSector:    Ҫ����Ŀ���sector��  
*     bDaysGrace:  Ҫ���������    
*      
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CSCAGetIppvEventList (DWORD dwResourceId, BYTE bSector, BYTE bDaysGrace);

/**************************************************************************
*Function Name: CS_CA_GetCompLevelShutType
*  ��Ӧ���յ�E-16ʱ,���ô˺����Ի�ü��ŷ�ʽΪComplevel(���������)ʱ,�رյ�����Ƶ,��Ƶ,��������Ƶ���ر�.
*  �Ͻӿڣ���������Ŀ������Ŀ������ʹ��.
*Parameters 
*	��
*
*Return Values: 
        -1 : FAILURE
*		0 : ����Ƶȫ���ر� 
*		1 : �ر���Ƶ
        2 : �ر���Ƶ
**************************************************************************/
INT CS_CA_GetCompLevelShutType(void);

/**************************************************************************
*Function Name: CSCAGetCompLevelShutType
*  ��Ӧ���յ�E-16ʱ,���ô˺����Ի�ü��ŷ�ʽΪComplevel(���������)ʱ,�رյ�����Ƶ,��Ƶ,��������Ƶ���ر�.
*  ���ڶ�·����. 
*Parameters 
*	dwGroupHandle: ���յ�E16��Ϣʱ,�ص������еĵ�������������dwGroupHandle,Ӧ���ڵ��øú���ʱ��param2�е�ֵԭֵ����.
*
*Return Values: 
        -1 : FAILURE
*		0 : ����Ƶȫ���ر� 
*		1 : �ر���Ƶ
        2 : �ر���Ƶ
**************************************************************************/
 INT CSCAGetCompLevelShutType(DWORD dwGroupHandle);

/**************************************************************************
*Function Name: CS_CA_SOLParseBATData
*  ����Bouquet_idΪ0x3623��BAT���,���ô˽ӿ���CA����BAT
* 
*Parameters 
*	pcData  BAT����
*   nDataLen BAT����
*Return Values: 
*         ��
*		
*		
**************************************************************************/
void CS_CA_SOLParseBATData(BYTE* pcData, int nDataLen );

/**************************************************************************
*Function Name: CS_CA_SOLParseBATData
*  ������NIT���,���ô˽ӿ���CA����NIT
* 
*Parameters 
*	pcData  BAT����
*     nDataLen BAT ����
      bCompleteFlg NIT���Ƿ�����

*Return Values: 
          ��
*		
*		
**************************************************************************/
void CS_CA_SOLParseNitData(BYTE* pcData, int nDataLen, BOOL bCompleteFlg);

/**************************************************************************
*Function Name: CSCAGetOtaSerIdOfBat
*  ��ȡOTA������BAT���е�serviceID
*
*Parameters 
*	��
*
*Return Values: 
*		OTA������BAT���е�serviceID
*		
**************************************************************************/
WORD CSCAGetOtaSerIdOfBat(void);

/**************************************************************************
*Function Name: CSCASetCardMrStatus
*  
*
*Parameters 
*	pstMrConfiguration:    Ӧ�ô����û��������Լ���ı�Ŀ�״̬;
*Return Values: 
*	 �ɹ�: SUCCESS
        ʧ��: FAILURE 
*		
**************************************************************************/
INT  CSCASetCardMrStatus (MrConfiguration_S  *pstMrConfiguration);

/**************************************************************************
*Function Name: CSCAMrProfileAction
*  
*
*Parameters 
*	pstMrProfile:    �����û��������Լ�Profile����;
*Return Values: 
*	 �ɹ�: SUCCESS
        ʧ��: FAILURE 
*		
**************************************************************************/
INT  CSCAMrProfileAction (MrProfile_S  *pstMrProfile);

/**************************************************************************
*Function Name: CSCAValidateMrPIN
*  
*
*Parameters 
*	pstPinInfo:    �����û����������Ϣ;
*Return Values: 
*	 �ɹ�: SUCCESS
        ʧ��: FAILURE 
*		
**************************************************************************/
INT  CSCAValidateMrPIN (MrPinInfo_S  *pstPinInfo);

/**************************************************************************
*Function Name:CSCASetFlashAddress
* ����CA �ں˴洢���������FLASH ��ַ
*
*Parameters
*  pstFlashAddress: CA Flash �����Ϣ
                                
pstFlashAddress->m_bCaFlashAddressFlag:1 ����ģʽ,������дFLash
                                        0 TESTAPP ģʽ,���ݲ�д��flash

pstFlashAddress->m_dwMainFlashAddr: �����CA�ں�����������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwMainFlashLength: CA�ں�����������С,��С��64K
pstFlashAddress->m_dwBackupFlashAddr:�����CA�ں˱�����������ַ,�ޱ�����,��Ϊ0
pstFlashAddress->m_dwBackupFlashLength:CA�ں˱�����������С,�ޱ�����,��Ϊ0;
                                       �б�����ʱ,��С��64K                                    
*                     
*Return Values: 
*	��CaFlashAddrErr_E����

Note:
    ���ú������ش��󣬽�����CA��ʼ��ʧ��!!
**************************************************************************/
CaFlashAddrErr_E CSCASetFlashAddress(CaFlashAddress_S *pstFlashAddress);

/**************************************************************************
*Function Name:CSCASetBISSSessionKey
* ��Ŀʹ��BISS��ʽ����ʱ��Ӧ�õ��ô˽ӿ�����session key���н���
*
*Parameters
*  aucKey[]: session key,����ODD KEY��EVEN KEY 
*  nKeyLen: session key�ĳ���,����ODD KEY��EVEN KEY                                                   
*                     
*Return Values: 
*	�ɹ�: SUCCESS
    ʧ��: FAILURE 

Note:
    �ú������ظ����á�
	ʾ����BISS keyΪ[0x11, 0x22, 0x33, 0x66, 0x33, 0x22, 0x11, 0x66],��
	aucKey[] = {0x11, 0x22, 0x33, 0x66, 0x33, 0x22, 0x11, 0x66,
	0x11, 0x22, 0x33, 0x66, 0x33, 0x22, 0x11, 0x66};
	nKeyLen = 16;
**************************************************************************/
INT CSCASetBISSSessionKey(BYTE aucKey[], INT nKeyLen);

/*���½ӿڽ���������ʱ�����Լ�������Ŀ*/
/**************************************************************************
*Function Name: CS_CA_GetFlashNVROMAddress
*  ��ȡCA���˽�����ݵ�flash�׵�ַ�ʹ�С
*
*Parameters 
*	pFlashNVMAddr:      NVM�׵�ַ
*   pFlashNVMSize:      NVM��С
*
*Return Values: 
*		��
**************************************************************************/
void CS_CA_GetFlashNVROMAddress(DWORD* pFlashNVMAddr, DWORD* pFlashNVMSize);

/**************************************************************************
*Function Name: CSCASetBackupFlashAddr
*  
*
*Parameters 
*	dwBackFlashAddr:    Ӧ��ΪCA����ı����õ�Flash����ʼ��ַ;
*	dwBackFlashNVMSize: �ÿ�Flash�Ĵ�С,��λ��Byte
*Return Values: 
*	 �ɹ�: SUCCESS
     ʧ��: FAILURE 
*		
**************************************************************************/
INT CSCASetBackupFlashAddr(DWORD dwBackFlashAddr, DWORD dwBackFlashNVMSize);

#ifdef  __cplusplus
}
#endif

#endif //_CS_IRDETO_INTERFACE_H_

