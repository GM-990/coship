/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_SUANTONGCTICA_Interface.h  
* Author:      
* Date:        June 2007
* Description: SuanTongCti CA interface
*            
*****************************************************************************************************************************/

/****************************************************************************************************************************
* 			                                     Revision History                                                           *
*****************************************************************************************************************************
*   Revision  : V1.0
*   Programmer: 
*   Create    : Harry Peng
*   Revision  : V1.1
*   Programmer: Yangwande
*   Modify    : 2011-09-01
*****************************************************************************************************************************/
#ifndef _CS_SUANTONGCTICA_INTERFACE_H_
#define _CS_SUANTONGCTICA_INTERFACE_H_


#include "udiplus_typedef.h"
#include "cs_cainterface_pvrext.h"

#ifdef  __cplusplus
extern "C" {
#endif


#define CS_SUANTONGCA_MESSAGE_BASE    (0)

/*
  ö������:    E_CaEventGroup
  
  �ṹ������:  ��������ͨCA�׸�Ӧ�õ����е���Ϣ����
  
  ����Լ��:    CS_SUANTONGCA_MESSAGE_BASE��ֵΪ0
*/
typedef enum _CaEventGroup
{
	CA_ALARM = CS_SUANTONGCA_MESSAGE_BASE,    
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	CA_MAX_STATUS
}E_CaEventGroup;


/*
  ö������:    E_CaInfo
  
  �ṹ������:  ��������ͨCA�׸�Ӧ�õ�����Info��Ϣ
  
  ����Լ��:    ��
*/
typedef enum _CaInfo
{
	CA_INFO_SMC_STATUS = (CS_SUANTONGCA_MESSAGE_BASE + 10),               /*10 dwParm2 = (DWORD)&T_SmcInfo*/
	CA_INFO_SYSTEMID,        							                  /*  dwParm2 = (DWORD)&T_CaSystemIdInfo, compliant with old version, not used now */
	CA_INFO_PPID,            							                  /*  dwParm2 = (DWORD)&T_PPIDList*/
	CA_INFO_PPID_DETAIL,     							                  /*  dwParm2 = (DWORD)&T_DetailPPIDInfo*/
	CA_INFO_TXT,             							                  /*  dwParm2 = (DWORD)&T_Msg*/
	CA_INFO_PARENTLEVEL,								                  /*  dwParm2 = (DWORD)dwParentLevel*/
	CA_INFO_USER_VIEW,          						                  /*  dwParm2 = (DWORD)&CaUserView_S*///hyb@090511
	CA_INFO_PPV_DETAIL,								                      /*  dwParm2 = (DWORD)&DetailPPVInfo_S*///cxj100519
	CA_INFO_IPPV_DETAIL,								                  /*  dwParm2 = (DWORD)&DetailIPPVInfo_S*///cxj100519
	CA_INFO_DISPLAY_STATUS,  							                  /*  dwParm2 = (DWORD)&T_CTI_DisplayStatus*///add by libin
	CA_INFO_MAX
}E_CaInfo;


/*
  ö������:    E_CaCommand
  
  �ṹ������:  ��������ͨCA�׸�Ӧ�õ�����Command��Ϣ
  
  ����Լ��:    ��
*/
typedef enum _CaCommand
{
	CA_COMMAND_FINGER_PRINT = (CS_SUANTONGCA_MESSAGE_BASE + 100),            /*100 dwParm2 = (DWORD)&T_CaSpecInfo*/
	CA_COMMAND_CHANGE_PGPIN,   							                     /*    dwParm2 = (DWORD)&T_CaPin, compliant with old version, not used now*/
	CA_COMMAND_IPPV_REQUEST,   							                     /*    dwParm2 = (DWORD)&T_PpvInfo*/
	CA_COMMAND_PAIRING,        							                     /*    dwParm2 = (DWORD)&T_SercialInfo, compliant with old version, not used now*/
	CA_COMMAND_OTA,			  					                             /*    dwParm2 = (DWORD)&T_OtaInfo, compliant with old version,not used now*/
   	CA_COMMAND_CHNL_SWITCH,							                         /*    dwParm2 = (DWORD)&CaChnlSwitch_S*///hyb@090511
	CA_COMMAND_CLEAN_SWITCH_MESSAGE,			                             /*�����Ļ����ת��ʾ��Ϣ, dwParm2 invalid*/
	CA_COMMAND_CHANNEL_SCAN,                                                 /*BAT����,������̨, dwParm2 invalid*/
	CA_COMMAND_HIDE_FINGERPRINT,											 /*����ָ��, dwParm2 invalid*/
	CA_COMMAND_MAX								                             
}E_CaCommand;


/*
  ö������:    E_CaStatus
  
  �ṹ������:  ��������ͨCA�׸�Ӧ�õ�����Status��Ϣ
  
  ����Լ��:    ��
*/
typedef enum _CaStatus
{
	CA_STATUS_PPID_ERROR = (CS_SUANTONGCA_MESSAGE_BASE + 200),               /*200 PPID Id Error*/
	CA_STATUS_READ_INFO_ERROR,         					                     /*    Read Information Fail*/
	CA_STATUS_PIN_ERROR,               					                     /*    Pin Code Error*/
	CA_STATUS_PIN_OK,                  					                     /*    Pin Code Ok*/
	CA_STATUS_MODIFY_PIN_SUCCESS,      					                     /*    Modify Pin Code Success*/
	CA_STATUS_MODIFY_PIN_FAIL,         					                     /*    Modify Pin Code Fail*/
	CA_STATUS_SERIAL_ERROR,            					                     /*    Serial Number Error, compliant with old version, not used now*/
	CA_STATUS_SET_SERIAL_SUCCESS,      					                     /*    Set Serial Number Success, compliant with old version, not used now*/
	CA_STATUS_SET_SERIAL_FAIL,         					                     /*    Set Serial Number Fail, compliant with old version, not used now*/
	CA_STATUS_RECORD_FAIL,             					                     /*    Request Record Flag Fail*/
	CA_STATUS_ALLOW_RECORD,            					                     /*    Allow Record*/
	CA_STATUS_REJECT_RECORD,           					                     /*    Reject Record*/
	CA_STATUS_MODIFY_PR_LEVEL_SUCCESS, 					                     /*    Modify Parent Rating Success*/
	CA_STATUS_MODIFY_PR_LEVEL_FAIL,    					                     /*    Modify Parent Rating Fail*/
	CA_STATUS_PR_LEVEL_ERROR,                   				             /*    Parent level error*//*add by libin*/
	CA_STATUS_GET_PPID_INFO_FAIL,      					                     /*    Get PPID Information Fail*/
	CA_STATUS_GET_DETAIL_PPID_INFO_FAIL, 					                 /*    Get Detail PPID Information Fail*/
	CA_STATUS_REQUEST_TYPE_ERROR,						                     /*    Request PPV/IPPV Program List Error*/
	CA_STATUS_REQUEST_DETAIL_PPV_FAIL,	
	CA_STATUS_MAX	
}E_CaStatus;


/*
  ö������:    E_CaAlarm
  
  �ṹ������:  ��������ͨCA�׸�Ӧ�õ�����Alarm��Ϣ
  
  ����Լ��:    E240��E241��E242��E254��ֻ�и߼���ȫ��Ŀ�Ż�ʹ�õ�
*/
typedef enum _CaAlarm
{
	CA_ALARM_FREE_PROGRAME = (CS_SUANTONGCA_MESSAGE_BASE + 300),              /*300 E00 ��ѽ�Ŀ*/
	CA_ALARM_SMC_OUT,             						                      /*    E01 ��������ܿ�*/
	CA_ALARM_SMC_UNKNOW,          						                      /*    E02 �޷�ʶ�����ܿ�*/
	CA_ALARM_SMC_CHECKING,        						                      /*    E03 ����У�����ܿ�*/	
	CA_ALARM_SMC_OK,              						                      /*    ������ʾ*/
	
	CA_ALARM_SMC_ERROR,          						                      /*305 E58 ���ܿ�����*/
	CA_ALARM_SMC_INFO_ERROR,     						                      /*    E63 ���ܿ�Ӳ������*/
	CA_ALARM_SMC_STATUS_ERROR,   						                      /*    E62 ���ܿ��������*/
	CA_ALARM_STB_SMC_NOT_MATCH, 						                      /*    E12 ������ƥ��*/
	CA_ALARM_PR_LEVEL_TOO_LOW,      						                  /*    E13 ��ĸ������̫��*/
	
	CA_ALARM_OTHER_CA_ENCRYPT,      						                  /*310 E10 �޷�ʶ��ļ��ܽ�Ŀ*/	
	CA_ALARM_ALLOCATE_ECM_CHANNEL_FAIL, 					                  /*    E25 ����ECM ͨ��ʧ��*/
	CA_ALARM_OPEN_DECRABLER_FAIL,           								  /*    E26 �������ͨ��ʧ��*/
	CA_ALARM_SMC_ERROR_DESCRABLING,         					              /*    E27 ����ʱ���ܿ�����*/
	CA_ALARM_CW_ERROR,                      					              /*    E14 �����ִ���*/
	
	CA_ALARM_NO_RIGHT,                      					              /*315 E15 û�н�����Ȩ*/
	CA_ALARM_DESCRAMBLE_SUCCESS,            					              /*    ������ʾ*/
	CA_ALARM_ALLOCATE_EMM_CHANNEL_FAIL,     					              /*    E59 ����EMM Channel/Filterʧ��*/
	CA_ALARM_IPPV_PROGRAME ,  							                      /*    E20 IPPV��Ŀ���û�ȡ���������ʾ��*/
    CA_ALARM_ORDER_IPPV_SUCCESS, 						                      /*    ������ʾ*/
    
	CA_ALARM_IPPV_PIN_ERROR,      						                      /*320 E51 IPPV�������*/
	CA_ALARM_IPPV_EXPIRED,        						                      /*    E52 IPPV��Ŀ����*/
	CA_ALARM_IPPV_LACKMONEY,							                      /*    E53 IPPV�㲥����*/
	CA_ALARM_IPPVQUERYMODE_UNSUPPORT,       					              /*    E54 IPPV �㲥��ʽ��֧��*/
	CA_ALARM_ORDER_IPPV_FAIL,    						                      /*    E55 IPPV�㲥ʧ��*/
	
	CA_ALARM_STOP_IPPV_SUCCESS,  						                      /*325 ������ʾ*/
	CA_ALARM_IPPVSTOP_FAIL,                           			              /*    E57 IPPV����ʧ��*/	
	CA_ALARM_NO_SIGNAL,      							                      /*    E60 ���ź�*/
	CA_ALARM_UNKNOW_SERVICE,      						                      /*    E61 δ֪��Ŀ����*/
	CA_ALARM_TABLE_STATUS_OK ,   						                      /*    δʹ��*/
	
	CA_ALARM_CAT_STATUS_ERROR,       						                  /*330 δʹ��*/
	CA_ALARM_STOP_PROGRAME_SUCCESS,  						                  /*    δʹ��*/
	CA_ALARM_STB_SMC_MATCH,          						                  /*    δʹ��*/
	CA_ALARM_SMC_IN,          							                      /*    δʹ��*/
	CA_ALARM_OTA_AVAILABLE,							                          /*    E65 ��⵽������Ϣ*/
	
	CA_ALARM_OTA_FAIL,								                          /*335 E64 OTA����ʧ��*/
	CA_ALARM_ST_PROGRAM, 							                          /*    ������ʾ*/
	CA_ALARM_ANTI_MOVE_CTRL,							                      /*    E11 �����벻ƥ��*/
	CA_ALARM_SON_CARD_OVERDUE,							                      /*    E17 �ӿ���֤����*/
	CA_ALARM_USERVIEW_CONTROL_ERROR,						                  /*    E18 �û�����/׼������*/
	
	CA_ALARM_REGIONVIEW_CONTROL_ERROR,						                  /*340 E19�������/׼������*/
	CA_ALARM_CARD_PAUSE,								                      /*    E24���ܿ���Ȩ����ͣ*/
	CA_ALARM_USERVIEW_SUCCESS,							                      /*    δʹ��*/
	CA_ALARM_USERVIEW_FAIL,							                          /*    δʹ��*/
	CA_ALARM_SONCARD_CERTIFY_SUCCESS,						                  /*    E05 �ӿ���֤�ɹ�*/
	
	CA_ALARM_SONCARD_CERTIFY_FAIL,           					              /*345 E07 �ӿ���֤ʧ��*/
	CA_ALARM_MOMCARD_CERTIFY_FAIL,           					              /*    E08 ĸ����֤ʧ��*/
	CA_ALARM_OP_RIGHT_PAUSE,							                      /*    E21 ���ܿ���Ŀ����Ȩ����ͣ*/
	CA_ALARM_CARD_UNACTIVE,							                          /*    E22 ���ܿ�����δ����״̬*/
	CA_ALARM_CARD_OVERDUE,							                          /*    E23 ���ܿ�������Ч��*/
	
	CA_ALARM_ALLOCATE_ECM_FILTER_FAIL, 	  				                      /*350 E29 ����ECM filter����*/
	CA_ALARM_PID_SWITCH_FAIL,			  				                      /*    E30 PID�л�����*/
	CA_ALARM_USERVIEW_SET_SUCCESS,						                      /*    E40 �û�׼�������óɹ�*/
	CA_ALARM_USERVIEW_DATAGET_SUCCESS,						                  /*    E41 �û�׼�������ݶ�ȡ�ɹ�*/
	CA_ALARM_USERVIEW_SETTIME_ERROR,						                  /*    E42 �û�׼��������ʱ�����*/
	
	CA_ALARM_USERVIEW_CTLTIME_TOOLONG,						                  /*355 E43 �û�׼��������ʱ�䳬��*/
	CA_ALARM_USERVIEW_SETCHNL_ERROR,						                  /*    E44 �û�׼��������ͨ������*/
	CA_ALARM_USERVIEW_SETMOD_ERROR,		    				                  /*    E45 �û�׼��������ģʽ����*/
	CA_ALARM_USERVIEW_PIN_ERROR,		    				                  /*    E46 �û�׼��������PIN�����*/
	CA_ALARM_USERVIEW_SET_FAIL,		    					                  /*    E47 �û�׼��������ʧ��*/
	
	CA_ALARM_PRGSTATUS_ERROR,							                      /*360 E28 ��Ŀ״̬����*/
	CA_ALARM_SMCID_NOTMATCH,							                      /*    E240 ���ܿ�ID��ƥ�䣨��Ը߼���ȫ����*/
	CA_ALARM_SMC_NOID,								                          /*    E241 ���ܿ���ID����Ը߼���ȫ����*/
	CA_ALARM_SMCID_MATCH,							                          /*    E242 ���ܿ�IDƥ�䣨��Ը߼���ȫ����*/
	CA_ALARM_SMCSECURITF_CHECKFAIL,						                      /*    E254 ���ܿ���ȫ������֤ʧ�� ����Ը߼���ȫ����*/
	CA_ALARM_ERROR_MAX
}E_CaAlarm;


/*
  ö������:    E_CaChnlSwitchType
  
  �ṹ������:  ������Ƶ����ת�ķ�ʽ
  
  ����Լ��:    ��
*/
typedef enum _CaChnlSwitchType
{
    EM_CA_CHNL_FORCE_SWITCH_IMMEDIATELY = 0,      	                         /*ǿ��������ת*/
    EM_CA_CHNL_FORCE_SWITCH_SPECIFY_TIME,        				             /*ǿ�ư�ʱ����ת(δʹ��)*/
    EM_CA_CHNL_USER_SWITCH_IMMEDIATELY,          				             /*�û�ѡ��������ת*/
    EM_CA_CHNL_USER_SWITCH_SPECIFY_TIME          				             /*�û�ѡ��ʱ��������ת(δʹ��)*/
}E_CaChnlSwitchType;


/*
  ö������:    E_CaModulation
  
  �ṹ������:  ��������Ŀ�����õĵ��Ʒ�ʽ
  
  ����Լ��:    ��
*/
typedef enum _CaModulation
{
    EM_CA_MODULATION_QAM = 1,      					                        /*QAM*/
    EM_CA_MODULATION_QPSK,        						                    /*QPSK*/
    EM_CA_MODULATION_BROADCAST,							                    /*����㲥*/
    EM_CA_MODULATION_MOBILE							                        /*�ƶ�����*/
}E_CaModulation;


/*
  ö������:    E_CaUserViewOp
  
  �ṹ������:  �����˶�׼�����Ĳ�����ʽ����ѯ׼����������׼��������ȡ������
  
  ����Լ��:    ��
*/
typedef enum _CaUserViewOp
{
    EM_CA_USER_VIEW_GET_INFO = 0,      						               /*��ѯ׼����*/
    EM_CA_USER_VIEW_SET_PARA,        						               /*����׼����*/
    EM_CA_USER_VIEW_CANCEL_SETTING						                   /*ȡ��׼����*/
}E_CaUserViewOp;


/*
  ö������:    E_CaUserViewStatus
  
  �ṹ������:  ����������׼����ģʽ
  
  ����Լ��:    ��
*/
typedef enum _CaUserViewStatus
{
    EM_CA_ALLOW_VIEW_EVERYDAY = 0,						                  /*�̶�ʱ���׼��*/
    EM_CA_ALLOW_VIEW_INTRADAY,       			 			              /*ָ��ʱ���׼��*/
   	EM_CA_FORBIDDEN_VIEW_EVERYDAY,						                  /*�̶�ʱ��ν���*/
    EM_CA_FORBIDDEN_VIEW_INTRADAY 						                  /*ָ��ʱ��ν���*/
}E_CaUserViewStatus;


#define CA_MAX_SYSTEMID_COUNT_LEN  (16)						              /*������ͨϵͳID��������*/

/*
  �ṹ������:  T_CurrentCaSystemInfo
  
  �ṹ������:  ������CAϵͳ����ϸ����
  
  ����Լ��:    �ýṹ��CAS Version 3�汾֮��δʹ��
*/
typedef struct _CurrentCaSystemInfo
{
	WORD     	m_wSystemId;							                  /*��ͨCAϵͳID*/
	WORD     	m_wEcmSubsystemId;						                  /*��ͨCA��ϵͳID(for ecm)*/
	WORD     	m_wEmmSubsystemId;						                  /*��ͨCA��ϵͳID(for emm)*/
}T_CurrentCaSystemInfo;


/*
  �ṹ������:  T_CaSystemIdInfo
  
  �ṹ������:  ������CAϵͳ�Ļ�������
  
  ����Լ��:    �ýṹ��CAS Version 3�汾֮��δʹ��
*/
typedef struct _CaSystemIdInfo
{
	BYTE     	m_cCount;							                     /*��ϵͳ�ĸ���*/
	T_CurrentCaSystemInfo  m_stCaSystemInfo[CA_MAX_SYSTEMID_COUNT_LEN];	 /*��ϵͳ��ϸ��Ϣ�б�*/
}T_CaSystemIdInfo;


#define CA_MAX_PPID_NAME_LEN       (17)						             /*������Ӫ�����Ƶ���󳤶�*/

/*
  �ṹ������:  T_PpvInfo
  
  �ṹ������:  ������IPPV��Ŀ�Ļ������Լ����б�
  
  ����Լ��:    ��
*/
typedef struct _PpvInfo
{
	INT     	m_nProgramNo;							                /*��ǰIPPV��Ŀ�����Ľ�Ŀ��ţ���PMT�����е�ProgramNo*/
	BYTE     	m_cStatus;							                    /*0:�������; 1:���������������ݶ��á�1��; 2:�������㲻�ܹ���*/
	DWORD     	m_dwPPID_Id;							                /*��ǰIPPV��Ŀ�ṩ�̵�ID*/
	BYTE     	m_acPPID_Label[CA_MAX_PPID_NAME_LEN];			        /*��Ŀ�ṩ�̵�����,�ַ�������*/
	BYTE     	m_fp_type_support;     					                /*�Ƿ�֧�ְ��ι��� 0:��ʾ��֧��; 1:��ʾ֧��*/
	BYTE    	m_ft_type_support; 						                /*�Ƿ�֧�ְ�ʱ�乺�� 0:��ʾ��֧��; 1:��ʾ֧��*/
	DWORD     	m_dwIppvNo;							                    /*IPPV��Ŀ�ı�ʶID��������Ӧ����ת����10�������ݽ�����ʾ*/
	FLOAT    	m_fIppv_P_Price;						                /*����Ŀ�㲥IPPV�ļ۸�, ���Ϊ��0��,��ʾ��֧�ְ�ʱ��㲥��ʽ.����ֵΪ�з�����,��λΪ���֡�,��λ��ǰ*/
	FLOAT    	m_fIppv_T_Price;						                /*��ʱ��㲥��IPPV�۸�, �۸�Ϊÿ������������Ϊ��0������ʾ��֧�ְ�ʱ��㲥�ķ�ʽ������ֵΪ�з���������λΪ���֡�����λ��ǰ*/
}T_PpvInfo;


#define CA_MAX_PINCODE_LEN         (4)							        /*����PIN�����󳤶�*/

/*
  �ṹ������:  T_PinCodeInfo
  
  �ṹ������:  ������PIN��Ļ�������
  
  ����Լ��:    ��
*/
typedef struct _PinCodeInfo
{	
	BYTE     	m_cPinLen;							                    /*PIN�볤��*/
	BYTE     	m_acPinCode[CA_MAX_PINCODE_LEN];				        /*PIN�룬16��������*/
}T_PinCodeInfo;


#define	CA_MAX_FINGERPRINT_LEN	   (9)				                    /*����Finger���ݵĳ���*/

/*
  �ṹ������:  T_CaSpecInfo
  
  �ṹ������:  ������ָ�Ƶ���ϸ����
  
  ����Լ��:    ����:m_bPriporityHighδʹ�ã�����Ŀ���ɵ�ʱ�����
*/
typedef struct _CaSpecInfo
{
	BOOL	  	m_bPriporityHigh;						               /*δʹ��*/
	WORD     	m_wDuration; 							               /*ָ�Ƶ���ʾʱ�䣬��λ:second��Ϊ0ʱ��ʾ������ʾ*/
	BYTE        m_acMsg[CA_MAX_FINGERPRINT_LEN];				       /*ָ�Ƶ�����,16��������,Ӧ��ֱ����ʾ16�������ݼ���*/
	BYTE      	m_ucPosition;							               /*��ʾλ�ã����û������������ݲ�ͬʱλ�ò�ͬ*/
}T_CaSpecInfo;


#define CA_MAIL_FROM_LEN          (256)							       /*�����ʼ������ߵĳ���,֮ǰ�İ汾Ϊ224*/
#define CA_MAIL_TITLE_LEN         (256)							       /*�����ʼ�����ĳ���*/
#define CA_MAIL_TXT_LEN           (2048)						       /*�����ʼ����ݵĳ���*/

/*
  �ṹ������:  T_CaDateTime
  
  �ṹ������:  ����ʱ��Ļ�����ʽ
  
  ����Լ��:    ��
*/
typedef struct _CaDateTime
{
	DWORD    	m_dwYear;
	BYTE     	m_cMonty;
	BYTE    	m_cDay;
	BYTE     	m_cHour;
	BYTE     	m_cMinute;
    BYTE     	m_cSecond;
    BYTE     	m_cFiveMillisecs;
}T_CaDateTime;


/*
  �ṹ������:  CaOSDShowMode_S
  
  �ṹ������:  ����OSD����ʾ��ʽ
  
  ����Լ��:    �����ǰԪ�ص�ֵΪ0����ʾ��ǰֵ��Ч�������Զ��巽ʽ
  		 ��ʾ�������ǰֵ��Ϊ0������Ҫȡ��ǰ��ֵ�����յ�ǰ��
  		 ֵȥ��ʾ�� ����m_ucLoopnum�������ֵΪ0������ʾ����
  		 ��Ӧ�þ��������ֵΪ5����Ӧ�ñ���ѭ����ʾ5��
*/
typedef struct _CaOSDShowMode_S
{
	WORD 		m_wCoordinateXlt;						             /*��ʾ��������Ͻ������Xֵ�����ֵ1920,�����ȡ�õĵ�ǰֵΪ0�������Լ��ķ�ʽ��ʾ*/
	WORD 		m_wCoordinateYlt;						             /*��ʾ��������Ͻ������Yֵ�����ֵ1280*/
	WORD 		m_wCoordinateXrt;						             /*��ʾ��������½������Xֵ�����ֵ1920*/
	WORD 		m_wCoordinateYrt;						             /*��ʾ��������½������Yֵ�����ֵ1280*/
	BYTE 		m_ucLoopnum;							             /*��ʾ����*/
	BYTE 		m_ucLoopinterval;						             /*ÿ����ʾ�ļ������λ��*/
	BYTE 		m_ucLoopspeed;						                 /*�����ٶȣ�ȡֵ1��2��3��Ӧ���١����١�����*/
	BYTE 		m_ucFontsize;							             /*������ֺţ�ȡֵ��Χ18_48*/
	DWORD 		m_dwColorfont;						                 /*������ɫ��32λ��ʾ��RGBֵ*/
	DWORD 		m_dwColorback;						                 /*������ɫ��32λ��ʾ��RGBֵ*/
}CaOSDShowMode_S;


/*
  �ṹ������:  T_Msg
  
  �ṹ������:  �����ʼ�����ϸ��Ϣ
  
  ����Լ��:    ֻ�е��ṹ�����������:bModeDefineΪTRUEʱ����Ҫ��������:sShowMode������
  		 ����ʾ��ʽȥ��ʾ���������ʾ��ʽ�涨���ṹ��:CaOSDShowMode_S����ϸ����
*/
typedef struct _Msg
{
	BOOL		m_bPriporityHigh;  						            /*FALSE: Mail  TRUE: Subtitle*/
	DWORD		m_dwFromLen;							            /*�ʼ������߳���*/
	BYTE    	m_szFrom[CA_MAIL_FROM_LEN];					        /*�ʼ������ߣ��ַ�������*/
	DWORD    	m_dwTitleLen;							            /*�ʼ����ⳤ��*/
	BYTE		m_szTitle[CA_MAIL_TITLE_LEN];				        /*�ʼ����⣬�ַ�������*/
	DWORD		m_dwContentLen;						                /*�ʼ����ݳ���*/
	BYTE		m_szContent[CA_MAIL_TXT_LEN];				        /*�ʼ����ݣ��ַ�������*/
	T_CaDateTime	m_stCreatTime;  						        /*�ʼ�����ʱ��*/
	T_CaDateTime	m_stSendTime;   						        /*�ʼ�����ʱ��*/
	BOOL 		bModeDefine; 							            /*ΪTRUEʱ���ṹ��sShowMode�й涨����ʾ��ʽ��ʾ�������Զ��巽ʽ��ʾ*/
	CaOSDShowMode_S sShowMode;							            /*OSD����ʾ��ʽ*/
}T_Msg;


/*
  �ṹ������:  T_CaMail
  
  �ṹ������:  �����ʼ��Ķ�������Լ����庬��
  
  ����Լ��:    ��ϸ���ʼ������������:m_stMsg�������ϸ����
*/
typedef struct _CaMail
{	
	BOOL            m_bReadFlag;    					           /*�Ѷ���־,FALSE:δ��; TRUE:�Ѷ�*/
	T_CaDateTime    m_stArriveTime;						           /*�ʼ������ʱ��*/
	T_Msg           m_stMsg;							           /*�ʼ���ϸ��Ϣ*/
}T_CaMail;


#define CA_MAX_ECM_LEN             (64)
#define CA_MAX_EMM_LEN             (64)
#define CA_MAX_SMC_VER_LEN         (9)
#define CA_MAX_SMC_NUMBER_LEN      (22)
#define CA_MAX_SMC_NAME_LEN        (17)
#define CA_MAX_SMC_ISSURE_TIME     (17)
#define CA_MAX_SMC_EXPIRE_TIME     (17)
#define CA_MAX_SMC_ANTIMOVE_LEN    (3)
#define CA_MAX_SMC_VRFSTARTTIME    (8)
#define CA_MAX_SMC_MTHUA           (5)
#define CA_MAX_ChipID_LENGTH       (16)


/*
  �ṹ������:  T_SmcInfo
  
  �ṹ������:  �������ܿ���ϸ��Ϣ�����Լ����庬��
  
  ����Լ��:    ���ܿ���״̬:m_cSmcStatus���ڼ��ɵ�ʱ�����ֻ��ʾ"����"����"�γ�"����״̬��
*/
typedef struct _SmcInfo
{
	BYTE      	m_cSmcStatus;							           /*���ܿ�״̬:0 SC_OK,1 SC_OUT,2 SC_HARD_ERR,4 SC_SOFT_ERR,6�ӿ���֤ʧ�ܣ�7�ӿ���֤�ɹ���8ĸ����֤ʧ��*/
	BYTE      	m_ucUsedStatus;						               /*0,���ܿ���Ч��1�����ܿ���ͣ��3�����ܿ���ȡ��*/
	WORD      	m_wSystemId;							           /*��ͨCAϵͳID*/
	WORD      	m_wEcmSubSystemId;						           /*��ͨCA��ϵͳID(for ecm)*/
	WORD      	m_wEmmSubSystemId;						           /*��ͨCA��ϵͳID(for emm)*/
	BYTE      	m_acSmcVer[CA_MAX_SMC_VER_LEN];				       /*���ܿ��汾�ţ��ַ�������*/
	BYTE      	m_acSmcNumber[CA_MAX_SMC_NUMBER_LEN];			   /*���ܿ��ţ�ʮ�����ַ���*/
	BYTE      	m_acSmcName[CA_MAX_SMC_NAME_LEN];				   /*���ܿ������������ַ�������*/
	BYTE      	m_acSmcIssureTime[CA_MAX_SMC_ISSURE_TIME];		   /*���ܿ�����ʱ�䣬�ַ�������*/
	BYTE      	m_acSmcExpireTime[CA_MAX_SMC_EXPIRE_TIME];		   /*���ܿ���Ч�ڽ�ֹʱ�䣬�ַ�������*/
	BYTE      	m_cParentalRating;						           /*���ܿ��ڸ�ĸ������,0:������;4~18Ϊ����*/
	BYTE      	m_cGroupControl;						           /*��Ŀ��������������*/
	BYTE      	m_acAntiMove[CA_MAX_SMC_ANTIMOVE_LEN];			   /*������ƴ��룬16��������*/
	BYTE          m_ucCardType;							           /*0,ĸ����1���ӿ���2�����ʹ���*/
	T_CaDateTime  sSoncardvfytime;						           /*�ӿ���֤ʱ�䣬�ַ������飬��Ϊ�ӿ�ʱ��ʾ*/
	WORD    	m_wsoncardwkperiod;						           /*�ӿ��������ڣ���Ϊ�ӿ�ʱ��ʾ,��λΪ����*/
	BYTE      	m_acmthercardua[CA_MAX_SMC_MTHUA];				   /*�ӿ�����ĸ��UA��16������ʾ����Ϊ�ӿ�ʱ��ʾ*/
	BYTE      	m_uccardmatching;						           /*���ܿ����״̬,0:δ���;1:�����;5:δע��STB Number;7:��STB��ƥ��*/
	BYTE      	m_ucstbirdmatching;						           /*���������״̬,2:δ���;3:�����;6:δע��STB Number*/
	BYTE      	m_ucChipIDlen;						               /*�����н�������ʶ��Chip ID������*/
	BYTE    	m_wStbChipID[CA_MAX_ChipID_LENGTH];			       /*��ǰ�����н�������ʶ��Chip ID��*/
	BYTE    	m_wScChipID[CA_MAX_ChipID_LENGTH ];			       /*���ܿ�ƥ���������ʶ��Chip ID��*/
	
}T_SmcInfo;


#define CA_MAX_PPID_NUMBER_LEN     (10)						       /*������Ӫ�̻�����Ϣ��������*/

/*
  �ṹ������:  T_PPIDInfo
  
  �ṹ������:  ������Ӫ�̻�����Ϣ�ĸ�������ֵ
  
  ����Լ��:    ���ܿ���״̬:m_cSmcStatus���ڼ��ɵ�ʱ�����ֻ��ʾ"����"����"�γ�"����״̬��
*/
typedef struct _PPIDInfo
{
	DWORD    	m_dwPPIDId;							               /*��Ŀ�ṩ��ID*/
	BYTE     	m_acPPIDName[CA_MAX_PPID_NAME_LEN];			       /*��Ŀ�ṩ�����ƣ��ַ�������*/
	BYTE        m_ucUsedStatus;						               /*0,PPID��Ч��1��PPID��ͣ*/
}T_PPIDInfo;


#define CS_CTICA_MAX_DISPLAY_NUM  (400)		                       /*�ɽ���ҵ����˵Ľ�Ŀ������*/

/*
  �ṹ������:  T_CTICA_FILTER_SRV_INFO
  
  �ṹ������:  ����ÿ��ҵ�����ϸ��Ϣ
  
  ����Լ��:    ��ʾ״̬�����ֿ��ܣ���������ϸ������
*/
//add by libin
typedef struct _CTICA_FILTER_SRV_INFO
{
    WORD 	m_wSrv_id;  								          /*servive_id*/
    WORD 	m_wTs_id;                                             /*transport_stream_id*/
    WORD 	m_wOrinet_id;								          /*original_network_id*/
    BYTE 	m_ucDisplayStatus;							          /*��ʾ״̬,  0,��Ŀ���Σ�1,��Ŀ��ʾ��0xFE,��Ŀ��Ϣ������*/
}T_CTICA_FILTER_SRV_INFO;							

/*
  �ṹ������:  T_CTI_DisplayStatus
  
  �ṹ������:  �������н���ҵ����˵Ľ�Ŀ��Ϣ
  
  ����Լ��:    ����Ϣ����"CA_INFO_DISPLAY_STATUS"���Ӧ����"CA_INFO_DISPLAY_STATUS"��Я���Ľṹ��
*/
//add by libin
typedef struct _CTI_DisplayStatus
{
	WORD m_wService_num;								                    /*����ҵ����˵�ʵ�ʽ�Ŀ����*/ 
	T_CTICA_FILTER_SRV_INFO m_stServiceDisplay[CS_CTICA_MAX_DISPLAY_NUM];	/*���н�Ŀ��״̬��Ϣ*/
}T_CTI_DisplayStatus;


/*
  �ṹ������:  T_PPIDList
  
  �ṹ������:  ������Ӫ�̻�����Ϣ���б���������ʾ��Ӫ�̻�����Ϣ��ʱ��͵��ø����ݽṹ
  
  ����Լ��:    ��
*/
typedef struct _PPIDList
{
	BYTE         m_cCount;							             /*��Ŀ�ṩ�̸���*/
	T_PPIDInfo   m_stPPIDInfo[CA_MAX_PPID_NUMBER_LEN];			 /*��Ŀ�ṩ����Ϣ*/
	BYTE         m_ucUsedStatus;						         /*0,PPID��Ч��1��PPID��ͣ*/
}T_PPIDList;

#define CA_PPID_LAST_CHANGE_CREDIT_DATE_LEN  (11)
#define CA_PPID_START_AUTHOR_DATE_LEN        (11)
#define CA_PPID_END_AUTHOR_DATE_LEN          (11)
#define CA_PPID_PROGRAME_NUMBER_LEN          (8)
#define CA_PPID_IPPV_NUMBER_LEN              (32)

/*
  �ṹ������:  T_PpvProgram
  
  �ṹ������:  ����PPV��Ŀ�Ŀ�ʼ�ͽ�����Ŀ��
  
  ����Լ��:    ��
*/
typedef struct _PpvProgram
{
	DWORD        m_dwStartPpvNo;						        /*PPV��ʼ��Ŀ�ţ����Ϊ0xffffffff����Ϊ20x00000000ʱ��Ч*/
	DWORD        m_dwEndPpvNo;							        /*PPV��ֹ��Ŀ�ţ����Ϊ0xffffffff����Ϊ20x00000000ʱ��Ч*/
}T_PpvProgram;


/*
  �ṹ������:  T_DetailPPIDInfo
  
  �ṹ������:  ������Ӫ����ϸ��Ϣ
  
  ����Լ��:    ��
*/
typedef struct _DetailPPIDInfo
{
	DWORD    	m_dwPPIDId;							                    /*��Ŀ�ṩ��ID*/
	FLOAT    	m_fCredit;			                                    /*ĳһ��Ŀ�ṩ���ڿ��ڵĽ��, ��λΪ��*/
	BYTE     	m_acCreditDate[CA_PPID_LAST_CHANGE_CREDIT_DATE_LEN];	/*ĳһ��Ŀ�ṩ�����һ�α䶯���ڽ������ڣ��ַ�������*/
	BYTE     	m_cClassNumber;						                    /*�Ѿ���Ȩ�Ľ�Ŀ������*/
	DWORD 	  	m_adwRightInfo[CA_PPID_PROGRAME_NUMBER_LEN];		    /*��Ȩ��Ϣ*/
	BYTE     	m_acProgStartDate[CA_PPID_START_AUTHOR_DATE_LEN];	    /*��Ȩ��ʼ���ڣ��ַ�������*/
	BYTE     	m_acProgEndDate[CA_PPID_END_AUTHOR_DATE_LEN];		    /*��Ȩ��ֹ���ڣ��ַ�������*/	
}T_DetailPPIDInfo;


/*
  �ṹ������:  DetailPPVInfo_S
  
  �ṹ������:  ��ѯPPV��Ŀʱ����ϸ��Ϣ
  
  ����Լ��:    һ�����ڲ鿴��Ӫ����ϸ��Ϣ��ʱ�������ʾ
*/
typedef struct _DetailPPVInfo
{
	DWORD    	m_dwPPIDId;							           /*��Ŀ�ṩ��ID*/
	BYTE     	m_ucPage;							           /*ҳ�ţ���0��ʼ*/
	BYTE     	m_ucProgPage;							       /*��ǰҳ�Ľ�Ŀ����*/
	T_PpvProgram  m_acPPVPrognum[CA_PPID_IPPV_NUMBER_LEN];	   /*ÿ���Ӧ��PPV��Ŀ��Ϣ*/
}DetailPPVInfo_S;


/*
  �ṹ������:  DetailIPPVInfo_S
  
  �ṹ������:  ��ѯIPPV��Ŀʱ����ϸ��Ϣ
  
  ����Լ��:    һ�����ڲ鿴��Ӫ����ϸ��Ϣ��ʱ�������ʾ
*/
typedef struct _DetailIPPVInfo//IPPV��ϸ��Ϣ
{
	DWORD    	m_dwPPIDId;							          /*��Ŀ�ṩ��ID*/
	BYTE     	m_ucPage;							          /*ҳ�ţ���0��ʼ*/
	BYTE     	m_ucProgPage;							      /*��ǰҳ�Ľ�Ŀ����*/
	DWORD    	m_dwPPVPrognum[CA_PPID_IPPV_NUMBER_LEN];	  /*ÿ����Ŀ�Ľ�Ŀ��*/
}DetailIPPVInfo_S;

#define CA_PAIRING_SERCIAL_NUMBER_LEN    (8)				  /*������������кŵ���󳤶�*/


/*
  �ṹ������:  T_SercialInfo
  
  �ṹ������:  ��ʶ���������к�
  
  ����Լ��:    ��
*/
typedef struct _SercialInfo
{
	INT      	m_nLen;							                       /*���������кų���*/
	BYTE     	m_acSercialNum[CA_PAIRING_SERCIAL_NUMBER_LEN];		   /*���������кţ�16��������*/
}T_SercialInfo;


/*
  �ṹ������:  T_OtaInfo
  
  �ṹ������:  OTA��ʹ�õľ�����������
  
  ����Լ��:    �ýṹ��CAS Version 3.2�汾֮��δʹ��
*/
typedef struct _OtaInfo
{
	BYTE    	m_cSoftVer;							         /*����汾*/
	DWORD   	m_dwDownLoadFrequency;					     /*�����������ݵ�Ƶ��*/
	WORD    	m_wDownLoadSymbol;						     /*����������������Ƶ��ķ�����*/
	BYTE    	m_cDownLoadModulation;					     /*����������������Ƶ��ĵ��Ʒ�ʽ*/
	WORD    	m_wDownLoadPid;						         /*�����������ݵ�PID*/
	BYTE    	m_cDownLoadTableid;						     /*�����������ݵ�TableID*/
	BYTE    	m_cDownLoadDuration;						 /*���������ʱ������*/
	T_CaDateTime  m_stIssueDate;						     /*���������ɹ���ʱ��*/
}T_OtaInfo;


/*
  �ṹ������:  CaSwitchByChnlPara_S
  
  �ṹ������:  ���尴Ƶ����ת�ĸ�������ֵ
  
  ����Լ��:    ��
*/
typedef struct
{    
    WORD    	m_wNetworkID; 			
   	WORD    	m_wTsID;           	
    WORD    	m_wServiceID;        
    WORD    	m_wEsPid; 		
}CaSwitchByChnlPara_S;


/*
  �ṹ������:  CaSwitchByTunerPara_S
  
  �ṹ������:  ���尴Ƶ����ת�ĸ�������ֵ
  
  ����Լ��:    ��
*/
typedef struct
{    
    WORD    		m_wFreq; 							   /*Ƶ�� :��λ:MHZ*/
    E_CaModulation   m_emModulation;         			   /*���Ʒ�ʽ*/
    WORD    		m_wSymRate;           				   /*symbol rate :Ksps*/
    WORD    		m_wServiceID; 						   /*service id*/
}CaSwitchByTunerPara_S;


/*
  ����������:  CaChnlSwitchPara_U
  
  �ṹ������:  ��Ƶ����ת�Ͱ�Ƶ����ת�ĸ������ֵ
  
  ����Լ��:    ��
*/
typedef union 
{
    CaSwitchByChnlPara_S   m_stSwitchByChnlPara;		   /*����Ŀ��ת�Ĳ���*/
    CaSwitchByTunerPara_S  m_stSwitchByTunerPara;		   /*��Ƶ����ת�Ĳ���*/
}CaChnlSwitchPara_U;


#define CA_MAX_TEXT_LEN               (256)

/*
  �ṹ������:  CaChnlSwitch_S
  
  �ṹ������:  ǿ����ת����Ҫ�ĸ�������ֵ
  
  ����Լ��:    ��
*/
typedef struct
{
    BOOL                   m_bIsCmdByTuner;				  /*FALSE: switch the channel by CaSwitchByChnlPara_S; TRUE:CaSwitchByTunerPara_S*/
    E_CaChnlSwitchType     m_emSwitchType;				  /*��ת����*/
    T_CaDateTime           m_stStartDate;				  /*Ƶ����ת��ʼʱ��,δʹ��*/
    T_CaDateTime           m_stExpireDate; 				  /*Ƶ����ת��ֹʱ��,δʹ��*/
    BYTE                   m_acText[CA_MAX_TEXT_LEN]; 	  /*��ת��ʾ��Ϣ���ݣ��ַ�������*/
    CaChnlSwitchPara_U     m_uPara;						  /*Ƶ����ת����*/
}CaChnlSwitch_S;


/*
  �ṹ������:  CaUserView_S
  
  �ṹ������:  ׼�������ù���������Ҫ�ĸ�������ֵ
  
  ����Լ��:    ��
*/
typedef struct
{
    E_CaUserViewOp         m_emOperation;				  /*�û�����ģʽ*/
    E_CaUserViewStatus     m_emStatus;					  /*׼����״̬*/
    WORD                   m_wStartChnl;    			  /*��ʼ����ͨ����*/
    WORD                   m_wEndChnl;					  /*��������ͨ����*/
    T_CaDateTime           m_stStartDate;				  /*׼������ʼʱ��*/
    T_CaDateTime           m_stExpireDate; 				  /*׼������ֹʱ��*/
}CaUserView_S;


/**************************************************************************
*Function Name: CS_CA_GetCoreVer
*  ��ȡ��ͨCA�ں˰汾��Ϣ
*
*Parameters 
*	pcVer[out]:
*		���ڴ洢CA�ں˰汾��Ϣ���� 
*   pnLen[in|out]:
*       ����CA�ں˰汾��Ϣ�ַ�������
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_GetCoreVer(char* pcVer, INT* pnLen);

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
* ��ע:��������Ŀ������Ŀ����Ҫ���ô˽ӿ�
**************************************************************************/
INT CS_CA_TXT_Add(T_CaMail* pstTxt);

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
*	�ʼ�����
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* ��ȡ����ĳһ���ʼ�������
*
*Parameters 
*	nIndex[in]:�ʼ��±꣬��0��ʼ
*	pstTxt[out]:CA���������ʼ����ݴ洢���ϲ����õĿռ�
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(INT nIndex, T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* �������ʼ��첽���浽flash�У���CS_CA_TXT_Add�����
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:��������Ŀ������Ŀ����Ҫ���ô˽ӿ�
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Save_Asyn
* �������ʼ��첽���浽flash�У���CS_CA_TXT_Add�����
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:��������Ŀ������Ŀ����Ҫ���ô˽ӿ�
**************************************************************************/
int CS_CA_TXT_Save_Asyn(void);

/**************************************************************************
*Function Name:CS_CA_TXT_SetReadFlag
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
INT CS_CA_TXT_SetReadFlag(INT nIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* ���ص�ǰ���ж��ٷ�δ���ʼ�
*
*Parameters 
*	��
*
*Return Values: 
*   δ���ʼ�����
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_MMI_CheckPinCode
* ����У��PIN��
*
*Parameters 
*	dwPin[in]:����PIN��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		CA_STATUS_PIN_OK    PIN����ȷ
*		CA_STATUS_PIN_ERROR PIN�����
**************************************************************************/
INT CS_CA_MMI_CheckPinCode(DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyPinCode
* �����޸�PIN��
*
*Parameters 
*	dwOldPin[in]:��PIN��
*   dwNewPin[in]:��PIN��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		CA_STATUS_MODIFY_PIN_SUCCESS   �޸�pin��ɹ�
*		CA_STATUS_MODIFY_PIN_FAIL      �޸�pin��ʧ��
**************************************************************************/
INT CS_CA_MMI_ModifyPinCode(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvConfirm
* �㲥PPV��Ŀ
*
*Parameters 
*	dwIppvNo[in]:��Ŀ��
*   dwPin[in]:����PinCode
*   bIsPpvp[in]:TRUE:����IPPV-P��FALSE����IPPV-T
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		CA_ALARM_ORDER_IPPV_SUCCESS       IPPVP�㲥�ɹ�
*		CA_ALARM_IPPV_PIN_ERROR           IPPV�������
*		CA_ALARM_IPPV_EXPIRED             IPPV��Ŀ����
*		CA_ALARM_IPPV_LACKMONEY           IPPV�㲥����
*		CA_ALARM_IPPVQUERYMODE_UNSUPPORT  IPPV�㲥��ʽ��֧��
*		CA_ALARM_ORDER_IPPV_FAIL          IPPV�㲥ʧ��
**************************************************************************/
INT CS_CA_MMI_PpvConfirm(DWORD dwIppvNo, DWORD dwPin, BOOL bIsPpvp);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvCancel
* ����IPPV-T��Ŀ�㲥
*
*Parameters 
*	dwIppvNo[in]:��Ŀ��
*   dwPin[in]:����PinCode
*  
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:���ӿ�ֻ�ǽ���IPPV-T��Ŀ�ĵ㲥��IPPV-P�㲥���ǲ���ȡ���ġ�
* �첽��ʽʵ��:
*		CA_ALARM_IPPV_PIN_ERROR           IPPV�������
*		CA_ALARM_STOP_IPPV_SUCCESS        IPPV�����ɹ�
*		CA_ALARM_IPPVSTOP_FAIL            IPPV����ʧ��
**************************************************************************/
INT CS_CA_MMI_PpvCancel(DWORD dwIppvNo, DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestSmcInfo
* �������ܿ���Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		CA_INFO_SMC_STATUS   dwParm2 = (DWORD)&T_SmcInfo
*       T_SmcInfo.m_cSmcStatusΪ0��ʱ��ɻ�ȡ���ܿ���Ϣ��Ϊ����ֵ��ʱ�������ܿ���Ϣ��ȡ����
**************************************************************************/
INT CS_CA_MMI_RequestSmcInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPPIDInfo
* ����PPID��Ϣ(��Ӫ����Ϣ)
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�CA_INFO_PPID   dwParm2 = (DWORD)&T_PPIDList
*       �����ʱ�򷵻�CA_STATUS_GET_PPID_INFO_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestPPIDInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestDetailPPIDInfo
* ����PPID��ϸ��Ϣ(��Ӫ����ϸ��Ϣ)
*
*Parameters 
*	dwPin[in]:����
*   dwPpId:��Ӫ��Id��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�CA_INFO_PPID_DETAIL   dwParm2 = (DWORD)&T_DetailPPIDInfo
*       �����ʱ�򷵻�
* 					CA_STATUS_READ_INFO_ERROR
*                   CA_STATUS_PIN_ERROR
*                   CA_STATUS_PPID_ERROR
*                   CA_STATUS_GET_DETAIL_PPID_INFO_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestDetailPPIDInfo(DWORD dwPin, DWORD dwPpId);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestDetailPPVInfo
* ����PPV��ϸ��Ϣ(��Ӫ��PPV��Ŀ����ϸ��Ϣ)
*
*Parameters 
*   dwPpId:��Ӫ��Id��
*	bType[in]:����Ľ�Ŀ���ͣ�TRUEΪPPV��Ŀ��FALSEΪIPPV��Ŀ
*	ucListPage[in]:����Ľ�Ŀ�б�ҳ������0��ʼ,PPV��8ҳ��IPPVֻ��1ҳ
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ����������PPV��Ŀ����CA_INFO_PPV_DETAIL,dwParm2 = (DWORD)&DetailPPVInfo_S
*                 ��������IPPV��Ŀ����CA_INFO_IPPV_DETAIL,dwParm2 = (DWORD)&DetailIPPVInfo_S
*       �����ʱ�򷵻�
* 					CA_STATUS_READ_INFO_ERROR                  
*                   CA_STATUS_PPID_ERROR
*                   CA_STATUS_REQUEST_TYPE_ERROR
*                   CA_STATUS_REQUEST_DETAIL_PPV_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestDetailPPVInfo(DWORD dwPpId, BOOL bType, BYTE ucListPage);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestRecordSrvAllowedFlag
* �����¼�Ʊ�־
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�
*					CA_STATUS_ALLOW_RECORD   
*					CA_STATUS_REJECT_RECORD   
*       �����ʱ�򷵻�
* 					CA_STATUS_RECORD_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestRecordSrvAllowedFlag();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyParentRating
* �����޸ĸ�ĸ��
*
*Parameters 
*	dwPin[in]:��������
*   cParentRatingValue[in]:Ҫ�޸ĵļ���,��Чֵ4~18:����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�
*					CA_STATUS_MODIFY_PR_LEVEL_SUCCESS   
*       �����ʱ�򷵻�
*					CA_STATUS_MODIFY_PR_LEVEL_FAIL   
* 					CA_STATUS_PIN_ERROR
**************************************************************************/
INT CS_CA_MMI_ModifyParentRating(DWORD dwPin, BYTE cParentRatingValue);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCurrentParentLevel
* ����ǰ��ĸ������
*
*Parameters 
	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		CA_INFO_PARENTLEVEL   dwParm2 = (DWORD)dwParentLevel
**************************************************************************/
INT CS_CA_MMI_RequestCurrentParentLevel();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPlayLowLevelProg
* ���󲥷Ÿ�ĸ�����ƵĽ�Ŀ������Ŀ�ĸ�ĸ��������Ͳ��ܹۿ�ʱ�����øýӿ����󲥷Ÿý�Ŀ
* �ýӿڻ�ͬʱ�޸ĸ�ĸ������
*Parameters 
*	dwPin:��������
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* ��ע:�첽��ʽʵ��:
*		��ȷ��ʱ�򷵻�
*					CA_STATUS_MODIFY_PR_LEVEL_SUCCESS   
*       �����ʱ�򷵻�
*					CA_STATUS_MODIFY_PR_LEVEL_FAIL   
* 					CA_STATUS_PIN_ERROR	
**************************************************************************/
INT CS_CA_MMI_RequestPlayLowLevelProg(DWORD dwPin);


/**************************************************************************
*Function Name:CS_CA_GetCurrentProgramNo
* ��ȡ��ǰ��Ŀ�ţ����ڱȽ��Ƿ��ѻ�̨
*
*Parameters 
*	��
*
*Return Values: 
*	��Ŀ�� 
**************************************************************************/
INT CS_CA_GetCurrentProgramNo();

/**************************************************************************
*Function Name:CS_CA_MMI_CheckIsNewPPV
*�жϵ�ǰ��PPV �Ƿ�Ϊ��
*Parameters 
*	pstOldPPV  ���Ƚϵ���PPV ��Ϣ
*	pstNewPPV ���Ƚϵ���PPV ��Ϣ

*Return Values:
	TRUE : PPV ��Ŀ�и���
	FALSE :PPV ��Ŀδ����
*
**************************************************************************/
BOOL CS_CA_MMI_CheckIsNewPPV(T_PpvInfo * pstOldPPV, T_PpvInfo * pstNewPPV);

/**************************************************************************
*Function Name:CS_CA_TXT_SetRepeatCheck
*�Ƿ�����ظ��ʼ��ж�
*Parameters 
*	bIsCheck 
*	TRUE ,�����ظ��ʼ��ж�.
*	FALSE ,�����ظ��ʼ��ж�.

*Return Values:
	SUCCESS :�ɹ�
	FAILURE :ʧ��
*ע��CA ��Ĭ�ϲ����ǽ����ظ��жϣ��������ظ������ʼ�
*����Ҫ�����ظ��ʼ�������Ե��ô˺��������޸ġ�
**************************************************************************/
INT CS_CA_TXT_SetRepeatCheck(BOOL bIsCheck);

/**************************************************************************
*Function Name:CS_CA_GetFlashAddress
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ�����
*Parameters 
*	dwFlashSize: ָ��Ҫ����FLASH��Ĵ�С
*Return Values:
*	���ط������ʼ��ַ������Ƿ���һ��Block��Flash�����Բ���dwFlashSize
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(DWORD dwFlashSize);

/**************************************************************************
*Function Name:get_software_version
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ����ã���ȡ��ǰ����汾
*Parameters 
*	chSWVision: ���ڴ洢��ǰ����汾��
*   nsizeSW   : ����汾����
*Return Values:
*	SUCCESS: �ɹ�
*   ����   : ʧ��
*��ע:CAS Version 3.2�汾֮��δʹ��,����ʵ�ֿ��ÿ�
**************************************************************************/
DWORD get_software_version (char* chSWVision, int nsizeSW);

/**************************************************************************
*Function Name:get_hardware_version
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ����ã���ȡ��ǰӲ���汾
*Parameters 
*	chHWVision: ���ڴ洢��ǰӲ���汾��
*   nsizeHW   : Ӳ���汾����
*Return Values:
*	SUCCESS: �ɹ�
*   ����   : ʧ��
*��ע:CAS Version 3.2�汾֮��δʹ��,����ʵ�ֿ��ÿ�
**************************************************************************/
DWORD get_hardware_version (char* chHWVision, int nsizeHW);

/**************************************************************************
*Function Name:CS_CA_MMI_UserViewOp
*�����û�׼����������Ŀ
*Parameters 
*   stUserView: CaUserView_S�ṹ��
*   dwPin     : PIN��
*Return Values:
*   SUCCESS: �ɹ�
*   FAILURE: ��������
* ��ע:�첽��ʽʵ��:
*	CA_INFO_USER_VIEW   dwParm2 = (DWORD)&CaUserView_S
*   stUserView.m_stStartDate.m_dwYear��С��2000,
*   Ҫ��ʼʱ��С�ڽ���ʱ�䣬��ʼͨ����С�ڽ���ͨ���ţ�����Ӧ��ʾ�û��������
**************************************************************************/
INT CS_CA_MMI_UserViewOp(CaUserView_S stUserView, DWORD dwPin);

/**************************************************************************
*Function Name:CSCAFactoryDefault
*�ָ���������ʱ��CA���������
*Parameters 
*  ��
*Return Values:
*  ��
**************************************************************************/
void CSCAFactoryDefault(void);

/**************************************************************************
*Function Name:CS_CA_GetMailFlashAddress
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ�����
*Parameters 
*   ��
*Return Values:
*   ���ط������ʼ��ַ
**************************************************************************/
extern DWORD CS_CA_GetMailFlashAddress(void);

/**************************************************************************
*Function Name:CSCAGetTSAntiMoveCtrlCode
*��ȡ�������������
*Parameters 
*   pcAntiMoveCtrl: �������������
*Return Values:
*   SUCCESS: �ɹ�
*   ����   : ʧ��
**************************************************************************/
INT CSCAGetTSAntiMoveCtrlCode(BYTE* pcAntiMoveCtrl);

/**************************************************************************
*Function Name:CSCaGetCurrentTime
*CAģ�������ӿڣ���Ŀ����ʱʵ�ִ˺�����CAģ�����
*Parameters 
*   stDateTime :��ǰEPGʱ��
*Return Values:
*   SUCCESS: �ɹ�
*   ����   : ʧ��
**************************************************************************/
extern INT CSCaGetCurrentTime(T_CaDateTime *stDateTime);

#ifdef  __cplusplus
}
#endif

#endif //_CS_SUANTONGCTICA_INTERFACE_H_

