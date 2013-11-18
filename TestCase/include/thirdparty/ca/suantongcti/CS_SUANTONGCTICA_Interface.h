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
  枚举名称:    E_CaEventGroup
  
  结构体作用:  定义了算通CA抛给应用的所有的消息类型
  
  特殊约定:    CS_SUANTONGCA_MESSAGE_BASE的值为0
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
  枚举名称:    E_CaInfo
  
  结构体作用:  定义了算通CA抛给应用的所有Info消息
  
  特殊约定:    无
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
  枚举名称:    E_CaCommand
  
  结构体作用:  定义了算通CA抛给应用的所有Command消息
  
  特殊约定:    无
*/
typedef enum _CaCommand
{
	CA_COMMAND_FINGER_PRINT = (CS_SUANTONGCA_MESSAGE_BASE + 100),            /*100 dwParm2 = (DWORD)&T_CaSpecInfo*/
	CA_COMMAND_CHANGE_PGPIN,   							                     /*    dwParm2 = (DWORD)&T_CaPin, compliant with old version, not used now*/
	CA_COMMAND_IPPV_REQUEST,   							                     /*    dwParm2 = (DWORD)&T_PpvInfo*/
	CA_COMMAND_PAIRING,        							                     /*    dwParm2 = (DWORD)&T_SercialInfo, compliant with old version, not used now*/
	CA_COMMAND_OTA,			  					                             /*    dwParm2 = (DWORD)&T_OtaInfo, compliant with old version,not used now*/
   	CA_COMMAND_CHNL_SWITCH,							                         /*    dwParm2 = (DWORD)&CaChnlSwitch_S*///hyb@090511
	CA_COMMAND_CLEAN_SWITCH_MESSAGE,			                             /*清除屏幕的跳转提示信息, dwParm2 invalid*/
	CA_COMMAND_CHANNEL_SCAN,                                                 /*BAT更新,重新搜台, dwParm2 invalid*/
	CA_COMMAND_HIDE_FINGERPRINT,											 /*隐藏指纹, dwParm2 invalid*/
	CA_COMMAND_MAX								                             
}E_CaCommand;


/*
  枚举名称:    E_CaStatus
  
  结构体作用:  定义了算通CA抛给应用的所有Status消息
  
  特殊约定:    无
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
  枚举名称:    E_CaAlarm
  
  结构体作用:  定义了算通CA抛给应用的所有Alarm消息
  
  特殊约定:    E240、E241、E242、E254，只有高级安全项目才会使用到
*/
typedef enum _CaAlarm
{
	CA_ALARM_FREE_PROGRAME = (CS_SUANTONGCA_MESSAGE_BASE + 300),              /*300 E00 免费节目*/
	CA_ALARM_SMC_OUT,             						                      /*    E01 请插入智能卡*/
	CA_ALARM_SMC_UNKNOW,          						                      /*    E02 无法识别智能卡*/
	CA_ALARM_SMC_CHECKING,        						                      /*    E03 正在校验智能卡*/	
	CA_ALARM_SMC_OK,              						                      /*    不必显示*/
	
	CA_ALARM_SMC_ERROR,          						                      /*305 E58 智能卡错误*/
	CA_ALARM_SMC_INFO_ERROR,     						                      /*    E63 智能卡硬件错误*/
	CA_ALARM_SMC_STATUS_ERROR,   						                      /*    E62 智能卡软件错误*/
	CA_ALARM_STB_SMC_NOT_MATCH, 						                      /*    E12 机卡不匹配*/
	CA_ALARM_PR_LEVEL_TOO_LOW,      						                  /*    E13 父母锁级别太低*/
	
	CA_ALARM_OTHER_CA_ENCRYPT,      						                  /*310 E10 无法识别的加密节目*/	
	CA_ALARM_ALLOCATE_ECM_CHANNEL_FAIL, 					                  /*    E25 分配ECM 通道失败*/
	CA_ALARM_OPEN_DECRABLER_FAIL,           								  /*    E26 分配解扰通道失败*/
	CA_ALARM_SMC_ERROR_DESCRABLING,         					              /*    E27 解扰时智能卡错误*/
	CA_ALARM_CW_ERROR,                      					              /*    E14 控制字错误*/
	
	CA_ALARM_NO_RIGHT,                      					              /*315 E15 没有解密授权*/
	CA_ALARM_DESCRAMBLE_SUCCESS,            					              /*    不必显示*/
	CA_ALARM_ALLOCATE_EMM_CHANNEL_FAIL,     					              /*    E59 分配EMM Channel/Filter失败*/
	CA_ALARM_IPPV_PROGRAME ,  							                      /*    E20 IPPV节目（用户取消购买后显示）*/
    CA_ALARM_ORDER_IPPV_SUCCESS, 						                      /*    不必显示*/
    
	CA_ALARM_IPPV_PIN_ERROR,      						                      /*320 E51 IPPV密码错误*/
	CA_ALARM_IPPV_EXPIRED,        						                      /*    E52 IPPV节目过期*/
	CA_ALARM_IPPV_LACKMONEY,							                      /*    E53 IPPV点播金额不足*/
	CA_ALARM_IPPVQUERYMODE_UNSUPPORT,       					              /*    E54 IPPV 点播方式不支持*/
	CA_ALARM_ORDER_IPPV_FAIL,    						                      /*    E55 IPPV点播失败*/
	
	CA_ALARM_STOP_IPPV_SUCCESS,  						                      /*325 不必显示*/
	CA_ALARM_IPPVSTOP_FAIL,                           			              /*    E57 IPPV结束失败*/	
	CA_ALARM_NO_SIGNAL,      							                      /*    E60 无信号*/
	CA_ALARM_UNKNOW_SERVICE,      						                      /*    E61 未知节目类型*/
	CA_ALARM_TABLE_STATUS_OK ,   						                      /*    未使用*/
	
	CA_ALARM_CAT_STATUS_ERROR,       						                  /*330 未使用*/
	CA_ALARM_STOP_PROGRAME_SUCCESS,  						                  /*    未使用*/
	CA_ALARM_STB_SMC_MATCH,          						                  /*    未使用*/
	CA_ALARM_SMC_IN,          							                      /*    未使用*/
	CA_ALARM_OTA_AVAILABLE,							                          /*    E65 检测到升级信息*/
	
	CA_ALARM_OTA_FAIL,								                          /*335 E64 OTA升级失败*/
	CA_ALARM_ST_PROGRAM, 							                          /*    不必显示*/
	CA_ALARM_ANTI_MOVE_CTRL,							                      /*    E11 区域码不匹配*/
	CA_ALARM_SON_CARD_OVERDUE,							                      /*    E17 子卡认证过期*/
	CA_ALARM_USERVIEW_CONTROL_ERROR,						                  /*    E18 用户禁播/准播控制*/
	
	CA_ALARM_REGIONVIEW_CONTROL_ERROR,						                  /*340 E19区域禁播/准播控制*/
	CA_ALARM_CARD_PAUSE,								                      /*    E24智能卡授权被暂停*/
	CA_ALARM_USERVIEW_SUCCESS,							                      /*    未使用*/
	CA_ALARM_USERVIEW_FAIL,							                          /*    未使用*/
	CA_ALARM_SONCARD_CERTIFY_SUCCESS,						                  /*    E05 子卡认证成功*/
	
	CA_ALARM_SONCARD_CERTIFY_FAIL,           					              /*345 E07 子卡认证失败*/
	CA_ALARM_MOMCARD_CERTIFY_FAIL,           					              /*    E08 母卡验证失败*/
	CA_ALARM_OP_RIGHT_PAUSE,							                      /*    E21 智能卡节目商授权被暂停*/
	CA_ALARM_CARD_UNACTIVE,							                          /*    E22 智能卡处于未激活状态*/
	CA_ALARM_CARD_OVERDUE,							                          /*    E23 智能卡超出有效期*/
	
	CA_ALARM_ALLOCATE_ECM_FILTER_FAIL, 	  				                      /*350 E29 分配ECM filter出错*/
	CA_ALARM_PID_SWITCH_FAIL,			  				                      /*    E30 PID切换出错*/
	CA_ALARM_USERVIEW_SET_SUCCESS,						                      /*    E40 用户准禁播设置成功*/
	CA_ALARM_USERVIEW_DATAGET_SUCCESS,						                  /*    E41 用户准禁播数据读取成功*/
	CA_ALARM_USERVIEW_SETTIME_ERROR,						                  /*    E42 用户准禁播设置时间错误*/
	
	CA_ALARM_USERVIEW_CTLTIME_TOOLONG,						                  /*355 E43 用户准禁播控制时间超长*/
	CA_ALARM_USERVIEW_SETCHNL_ERROR,						                  /*    E44 用户准禁播设置通道错误*/
	CA_ALARM_USERVIEW_SETMOD_ERROR,		    				                  /*    E45 用户准禁播设置模式错误*/
	CA_ALARM_USERVIEW_PIN_ERROR,		    				                  /*    E46 用户准禁播设置PIN码错误*/
	CA_ALARM_USERVIEW_SET_FAIL,		    					                  /*    E47 用户准禁播设置失败*/
	
	CA_ALARM_PRGSTATUS_ERROR,							                      /*360 E28 节目状态错误*/
	CA_ALARM_SMCID_NOTMATCH,							                      /*    E240 智能卡ID不匹配（针对高级安全卡）*/
	CA_ALARM_SMC_NOID,								                          /*    E241 智能卡无ID（针对高级安全卡）*/
	CA_ALARM_SMCID_MATCH,							                          /*    E242 智能卡ID匹配（针对高级安全卡）*/
	CA_ALARM_SMCSECURITF_CHECKFAIL,						                      /*    E254 智能卡安全功能验证失败 （针对高级安全卡）*/
	CA_ALARM_ERROR_MAX
}E_CaAlarm;


/*
  枚举名称:    E_CaChnlSwitchType
  
  结构体作用:  定义了频道跳转的方式
  
  特殊约定:    无
*/
typedef enum _CaChnlSwitchType
{
    EM_CA_CHNL_FORCE_SWITCH_IMMEDIATELY = 0,      	                         /*强制立即跳转*/
    EM_CA_CHNL_FORCE_SWITCH_SPECIFY_TIME,        				             /*强制按时间跳转(未使用)*/
    EM_CA_CHNL_USER_SWITCH_IMMEDIATELY,          				             /*用户选择立即跳转*/
    EM_CA_CHNL_USER_SWITCH_SPECIFY_TIME          				             /*用户选择按时间立即跳转(未使用)*/
}E_CaChnlSwitchType;


/*
  枚举名称:    E_CaModulation
  
  结构体作用:  定义了项目所采用的调制方式
  
  特殊约定:    无
*/
typedef enum _CaModulation
{
    EM_CA_MODULATION_QAM = 1,      					                        /*QAM*/
    EM_CA_MODULATION_QPSK,        						                    /*QPSK*/
    EM_CA_MODULATION_BROADCAST,							                    /*地面广播*/
    EM_CA_MODULATION_MOBILE							                        /*移动网络*/
}E_CaModulation;


/*
  枚举名称:    E_CaUserViewOp
  
  结构体作用:  定义了对准禁播的操作方式，查询准禁播，设置准禁播还是取消设置
  
  特殊约定:    无
*/
typedef enum _CaUserViewOp
{
    EM_CA_USER_VIEW_GET_INFO = 0,      						               /*查询准禁播*/
    EM_CA_USER_VIEW_SET_PARA,        						               /*设置准禁播*/
    EM_CA_USER_VIEW_CANCEL_SETTING						                   /*取消准禁播*/
}E_CaUserViewOp;


/*
  枚举名称:    E_CaUserViewStatus
  
  结构体作用:  定义了设置准禁播模式
  
  特殊约定:    无
*/
typedef enum _CaUserViewStatus
{
    EM_CA_ALLOW_VIEW_EVERYDAY = 0,						                  /*固定时间段准播*/
    EM_CA_ALLOW_VIEW_INTRADAY,       			 			              /*指定时间段准播*/
   	EM_CA_FORBIDDEN_VIEW_EVERYDAY,						                  /*固定时间段禁播*/
    EM_CA_FORBIDDEN_VIEW_INTRADAY 						                  /*指定时间段禁播*/
}E_CaUserViewStatus;


#define CA_MAX_SYSTEMID_COUNT_LEN  (16)						              /*定义算通系统ID的最大个数*/

/*
  结构体名称:  T_CurrentCaSystemInfo
  
  结构体作用:  定义了CA系统的详细属性
  
  特殊约定:    该结构在CAS Version 3版本之后未使用
*/
typedef struct _CurrentCaSystemInfo
{
	WORD     	m_wSystemId;							                  /*算通CA系统ID*/
	WORD     	m_wEcmSubsystemId;						                  /*算通CA子系统ID(for ecm)*/
	WORD     	m_wEmmSubsystemId;						                  /*算通CA子系统ID(for emm)*/
}T_CurrentCaSystemInfo;


/*
  结构体名称:  T_CaSystemIdInfo
  
  结构体作用:  定义了CA系统的基本属性
  
  特殊约定:    该结构在CAS Version 3版本之后未使用
*/
typedef struct _CaSystemIdInfo
{
	BYTE     	m_cCount;							                     /*子系统的个数*/
	T_CurrentCaSystemInfo  m_stCaSystemInfo[CA_MAX_SYSTEMID_COUNT_LEN];	 /*子系统详细信息列表*/
}T_CaSystemIdInfo;


#define CA_MAX_PPID_NAME_LEN       (17)						             /*定义运营商名称的最大长度*/

/*
  结构体名称:  T_PpvInfo
  
  结构体作用:  定义了IPPV节目的基本属性及其列表
  
  特殊约定:    无
*/
typedef struct _PpvInfo
{
	INT     	m_nProgramNo;							                /*当前IPPV节目所属的节目编号，即PMT请求中的ProgramNo*/
	BYTE     	m_cStatus;							                    /*0:允许购买节; 1:读卡错误，所有数据都置’1’; 2:卡内余额不足不能购买*/
	DWORD     	m_dwPPID_Id;							                /*当前IPPV节目提供商的ID*/
	BYTE     	m_acPPID_Label[CA_MAX_PPID_NAME_LEN];			        /*节目提供商的名字,字符串数组*/
	BYTE     	m_fp_type_support;     					                /*是否支持按次购买 0:表示不支持; 1:表示支持*/
	BYTE    	m_ft_type_support; 						                /*是否支持按时间购买 0:表示不支持; 1:表示支持*/
	DWORD     	m_dwIppvNo;							                    /*IPPV节目的标识ID，机顶盒应将其转换成10进制数据进行显示*/
	FLOAT    	m_fIppv_P_Price;						                /*按节目点播IPPV的价格, 如果为’0’,表示不支持按时间点播方式.此数值为有符号数,单位为“分”,高位在前*/
	FLOAT    	m_fIppv_T_Price;						                /*按时间点播的IPPV价格, 价格为每分钟所需金额。如果为’0’，表示不支持按时间点播的方式。此数值为有符号数，单位为“分”，高位在前*/
}T_PpvInfo;


#define CA_MAX_PINCODE_LEN         (4)							        /*定义PIN码的最大长度*/

/*
  结构体名称:  T_PinCodeInfo
  
  结构体作用:  定义了PIN码的基本属性
  
  特殊约定:    无
*/
typedef struct _PinCodeInfo
{	
	BYTE     	m_cPinLen;							                    /*PIN码长度*/
	BYTE     	m_acPinCode[CA_MAX_PINCODE_LEN];				        /*PIN码，16进制数据*/
}T_PinCodeInfo;


#define	CA_MAX_FINGERPRINT_LEN	   (9)				                    /*定义Finger内容的长度*/

/*
  结构体名称:  T_CaSpecInfo
  
  结构体作用:  定义了指纹的详细属性
  
  特殊约定:    属性:m_bPriporityHigh未使用，在项目集成的时候忽略
*/
typedef struct _CaSpecInfo
{
	BOOL	  	m_bPriporityHigh;						               /*未使用*/
	WORD     	m_wDuration; 							               /*指纹的显示时间，单位:second，为0时表示永久显示*/
	BYTE        m_acMsg[CA_MAX_FINGERPRINT_LEN];				       /*指纹的内容,16进制数据,应用直接显示16进制数据即可*/
	BYTE      	m_ucPosition;							               /*显示位置，由用户决定，该数据不同时位置不同*/
}T_CaSpecInfo;


#define CA_MAIL_FROM_LEN          (256)							       /*定义邮件发送者的长度,之前的版本为224*/
#define CA_MAIL_TITLE_LEN         (256)							       /*定义邮件标题的长度*/
#define CA_MAIL_TXT_LEN           (2048)						       /*定义邮件内容的长度*/

/*
  结构体名称:  T_CaDateTime
  
  结构体作用:  定义时间的基本格式
  
  特殊约定:    无
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
  结构体名称:  CaOSDShowMode_S
  
  结构体作用:  定义OSD的显示方式
  
  特殊约定:    如果当前元素的值为0，表示当前值无效，按照自定义方式
  		 显示，如果当前值不为0，则需要取当前的值，按照当前的
  		 值去显示， 比如m_ucLoopnum，如果其值为0，则显示次数
  		 由应用决定，如果值为5，则应用必须循环显示5次
*/
typedef struct _CaOSDShowMode_S
{
	WORD 		m_wCoordinateXlt;						             /*显示区域的左上角坐标的X值，最大值1920,如果所取得的当前值为0，则按照自己的方式显示*/
	WORD 		m_wCoordinateYlt;						             /*显示区域的左上角坐标的Y值，最大值1280*/
	WORD 		m_wCoordinateXrt;						             /*显示区域的右下角坐标的X值，最大值1920*/
	WORD 		m_wCoordinateYrt;						             /*显示区域的右下角坐标的Y值，最大值1280*/
	BYTE 		m_ucLoopnum;							             /*显示次数*/
	BYTE 		m_ucLoopinterval;						             /*每次显示的间隔，单位秒*/
	BYTE 		m_ucLoopspeed;						                 /*滚动速度，取值1，2，3对应慢速、中速、快速*/
	BYTE 		m_ucFontsize;							             /*字体的字号，取值范围18_48*/
	DWORD 		m_dwColorfont;						                 /*文字颜色，32位表示的RGB值*/
	DWORD 		m_dwColorback;						                 /*背景颜色，32位表示的RGB值*/
}CaOSDShowMode_S;


/*
  结构体名称:  T_Msg
  
  结构体作用:  定义邮件的详细信息
  
  特殊约定:    只有当结构体里面的属性:bModeDefine为TRUE时才需要按照属性:sShowMode所定义
  		 的显示方式去显示，具体的显示方式规定见结构体:CaOSDShowMode_S的详细描述
*/
typedef struct _Msg
{
	BOOL		m_bPriporityHigh;  						            /*FALSE: Mail  TRUE: Subtitle*/
	DWORD		m_dwFromLen;							            /*邮件发送者长度*/
	BYTE    	m_szFrom[CA_MAIL_FROM_LEN];					        /*邮件发送者，字符串数组*/
	DWORD    	m_dwTitleLen;							            /*邮件标题长度*/
	BYTE		m_szTitle[CA_MAIL_TITLE_LEN];				        /*邮件标题，字符串数组*/
	DWORD		m_dwContentLen;						                /*邮件内容长度*/
	BYTE		m_szContent[CA_MAIL_TXT_LEN];				        /*邮件内容，字符串数组*/
	T_CaDateTime	m_stCreatTime;  						        /*邮件生成时间*/
	T_CaDateTime	m_stSendTime;   						        /*邮件发送时间*/
	BOOL 		bModeDefine; 							            /*为TRUE时按结构体sShowMode中规定的显示方式显示，否则按自定义方式显示*/
	CaOSDShowMode_S sShowMode;							            /*OSD的显示方式*/
}T_Msg;


/*
  结构体名称:  T_CaMail
  
  结构体作用:  定义邮件的对外的属性及具体含义
  
  特殊约定:    详细的邮件内容描述详见:m_stMsg里面的详细描述
*/
typedef struct _CaMail
{	
	BOOL            m_bReadFlag;    					           /*已读标志,FALSE:未读; TRUE:已读*/
	T_CaDateTime    m_stArriveTime;						           /*邮件到达的时间*/
	T_Msg           m_stMsg;							           /*邮件详细信息*/
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
  结构体名称:  T_SmcInfo
  
  结构体作用:  定义智能卡详细信息的属性及具体含义
  
  特殊约定:    智能卡的状态:m_cSmcStatus，在集成的时候可以只显示"插入"或者"拔出"两种状态。
*/
typedef struct _SmcInfo
{
	BYTE      	m_cSmcStatus;							           /*智能卡状态:0 SC_OK,1 SC_OUT,2 SC_HARD_ERR,4 SC_SOFT_ERR,6子卡认证失败，7子卡认证成功，8母卡验证失败*/
	BYTE      	m_ucUsedStatus;						               /*0,智能卡有效；1，智能卡暂停；3，智能卡被取消*/
	WORD      	m_wSystemId;							           /*算通CA系统ID*/
	WORD      	m_wEcmSubSystemId;						           /*算通CA子系统ID(for ecm)*/
	WORD      	m_wEmmSubSystemId;						           /*算通CA子系统ID(for emm)*/
	BYTE      	m_acSmcVer[CA_MAX_SMC_VER_LEN];				       /*智能卡版本号，字符串数组*/
	BYTE      	m_acSmcNumber[CA_MAX_SMC_NUMBER_LEN];			   /*智能卡号，十进制字符串*/
	BYTE      	m_acSmcName[CA_MAX_SMC_NAME_LEN];				   /*智能卡类型描述，字符串数组*/
	BYTE      	m_acSmcIssureTime[CA_MAX_SMC_ISSURE_TIME];		   /*智能卡发卡时间，字符串数组*/
	BYTE      	m_acSmcExpireTime[CA_MAX_SMC_EXPIRE_TIME];		   /*智能卡有效期截止时间，字符串数组*/
	BYTE      	m_cParentalRating;						           /*智能卡内父母锁级别,0:无限制;4~18为年龄*/
	BYTE      	m_cGroupControl;						           /*节目分组搜索控制码*/
	BYTE      	m_acAntiMove[CA_MAX_SMC_ANTIMOVE_LEN];			   /*区域控制代码，16进制数据*/
	BYTE          m_ucCardType;							           /*0,母卡；1，子卡；2，类型错误*/
	T_CaDateTime  sSoncardvfytime;						           /*子卡认证时间，字符串数组，卡为子卡时显示*/
	WORD    	m_wsoncardwkperiod;						           /*子卡工作周期，卡为子卡时显示,单位为分钟*/
	BYTE      	m_acmthercardua[CA_MAX_SMC_MTHUA];				   /*子卡所属母卡UA，16进制显示，卡为子卡时显示*/
	BYTE      	m_uccardmatching;						           /*智能卡配对状态,0:未配对;1:已配对;5:未注册STB Number;7:与STB不匹配*/
	BYTE      	m_ucstbirdmatching;						           /*机顶盒配对状态,2:未配对;3:已配对;6:未注册STB Number*/
	BYTE      	m_ucChipIDlen;						               /*机顶盒解码器标识（Chip ID）长度*/
	BYTE    	m_wStbChipID[CA_MAX_ChipID_LENGTH];			       /*当前机顶盒解码器标识（Chip ID）*/
	BYTE    	m_wScChipID[CA_MAX_ChipID_LENGTH ];			       /*智能卡匹配解码器标识（Chip ID）*/
	
}T_SmcInfo;


#define CA_MAX_PPID_NUMBER_LEN     (10)						       /*定义运营商基本信息的最大个数*/

/*
  结构体名称:  T_PPIDInfo
  
  结构体作用:  定义运营商基本信息的各个属性值
  
  特殊约定:    智能卡的状态:m_cSmcStatus，在集成的时候可以只显示"插入"或者"拔出"两种状态。
*/
typedef struct _PPIDInfo
{
	DWORD    	m_dwPPIDId;							               /*节目提供商ID*/
	BYTE     	m_acPPIDName[CA_MAX_PPID_NAME_LEN];			       /*节目提供商名称，字符串数组*/
	BYTE        m_ucUsedStatus;						               /*0,PPID有效；1，PPID暂停*/
}T_PPIDInfo;


#define CS_CTICA_MAX_DISPLAY_NUM  (400)		                       /*可进行业务过滤的节目最大个数*/

/*
  结构体名称:  T_CTICA_FILTER_SRV_INFO
  
  结构体作用:  定义每个业务的详细信息
  
  特殊约定:    显示状态有三种可能，如下有详细的叙述
*/
//add by libin
typedef struct _CTICA_FILTER_SRV_INFO
{
    WORD 	m_wSrv_id;  								          /*servive_id*/
    WORD 	m_wTs_id;                                             /*transport_stream_id*/
    WORD 	m_wOrinet_id;								          /*original_network_id*/
    BYTE 	m_ucDisplayStatus;							          /*显示状态,  0,节目屏蔽；1,节目显示；0xFE,节目信息不存在*/
}T_CTICA_FILTER_SRV_INFO;							

/*
  结构体名称:  T_CTI_DisplayStatus
  
  结构体作用:  定义所有进行业务过滤的节目信息
  
  特殊约定:    与消息类型"CA_INFO_DISPLAY_STATUS"相对应，是"CA_INFO_DISPLAY_STATUS"所携带的结构体
*/
//add by libin
typedef struct _CTI_DisplayStatus
{
	WORD m_wService_num;								                    /*进行业务过滤的实际节目个数*/ 
	T_CTICA_FILTER_SRV_INFO m_stServiceDisplay[CS_CTICA_MAX_DISPLAY_NUM];	/*所有节目的状态信息*/
}T_CTI_DisplayStatus;


/*
  结构体名称:  T_PPIDList
  
  结构体作用:  定义运营商基本信息的列表，在请求显示运营商基本信息的时候就调用该数据结构
  
  特殊约定:    无
*/
typedef struct _PPIDList
{
	BYTE         m_cCount;							             /*节目提供商个数*/
	T_PPIDInfo   m_stPPIDInfo[CA_MAX_PPID_NUMBER_LEN];			 /*节目提供商信息*/
	BYTE         m_ucUsedStatus;						         /*0,PPID有效；1，PPID暂停*/
}T_PPIDList;

#define CA_PPID_LAST_CHANGE_CREDIT_DATE_LEN  (11)
#define CA_PPID_START_AUTHOR_DATE_LEN        (11)
#define CA_PPID_END_AUTHOR_DATE_LEN          (11)
#define CA_PPID_PROGRAME_NUMBER_LEN          (8)
#define CA_PPID_IPPV_NUMBER_LEN              (32)

/*
  结构体名称:  T_PpvProgram
  
  结构体作用:  定义PPV节目的开始和结束节目号
  
  特殊约定:    无
*/
typedef struct _PpvProgram
{
	DWORD        m_dwStartPpvNo;						        /*PPV起始节目号，如果为0xffffffff或者为20x00000000时无效*/
	DWORD        m_dwEndPpvNo;							        /*PPV终止节目号，如果为0xffffffff或者为20x00000000时无效*/
}T_PpvProgram;


/*
  结构体名称:  T_DetailPPIDInfo
  
  结构体作用:  定义运营商详细信息
  
  特殊约定:    无
*/
typedef struct _DetailPPIDInfo
{
	DWORD    	m_dwPPIDId;							                    /*节目提供商ID*/
	FLOAT    	m_fCredit;			                                    /*某一节目提供商在卡内的金额, 单位为分*/
	BYTE     	m_acCreditDate[CA_PPID_LAST_CHANGE_CREDIT_DATE_LEN];	/*某一节目提供商最近一次变动卡内金额的日期，字符串数组*/
	BYTE     	m_cClassNumber;						                    /*已经授权的节目的数量*/
	DWORD 	  	m_adwRightInfo[CA_PPID_PROGRAME_NUMBER_LEN];		    /*授权信息*/
	BYTE     	m_acProgStartDate[CA_PPID_START_AUTHOR_DATE_LEN];	    /*授权起始日期，字符串数组*/
	BYTE     	m_acProgEndDate[CA_PPID_END_AUTHOR_DATE_LEN];		    /*授权截止日期，字符串数组*/	
}T_DetailPPIDInfo;


/*
  结构体名称:  DetailPPVInfo_S
  
  结构体作用:  查询PPV节目时的详细信息
  
  特殊约定:    一般是在查看运营商详细信息的时候进行显示
*/
typedef struct _DetailPPVInfo
{
	DWORD    	m_dwPPIDId;							           /*节目提供商ID*/
	BYTE     	m_ucPage;							           /*页号，从0开始*/
	BYTE     	m_ucProgPage;							       /*当前页的节目组数*/
	T_PpvProgram  m_acPPVPrognum[CA_PPID_IPPV_NUMBER_LEN];	   /*每组对应的PPV节目信息*/
}DetailPPVInfo_S;


/*
  结构体名称:  DetailIPPVInfo_S
  
  结构体作用:  查询IPPV节目时的详细信息
  
  特殊约定:    一般是在查看运营商详细信息的时候进行显示
*/
typedef struct _DetailIPPVInfo//IPPV详细信息
{
	DWORD    	m_dwPPIDId;							          /*节目提供商ID*/
	BYTE     	m_ucPage;							          /*页号，从0开始*/
	BYTE     	m_ucProgPage;							      /*当前页的节目个数*/
	DWORD    	m_dwPPVPrognum[CA_PPID_IPPV_NUMBER_LEN];	  /*每个节目的节目号*/
}DetailIPPVInfo_S;

#define CA_PAIRING_SERCIAL_NUMBER_LEN    (8)				  /*定义机顶盒序列号的最大长度*/


/*
  结构体名称:  T_SercialInfo
  
  结构体作用:  标识机顶盒序列号
  
  特殊约定:    无
*/
typedef struct _SercialInfo
{
	INT      	m_nLen;							                       /*机顶盒序列号长度*/
	BYTE     	m_acSercialNum[CA_PAIRING_SERCIAL_NUMBER_LEN];		   /*机顶盒序列号，16进制数据*/
}T_SercialInfo;


/*
  结构体名称:  T_OtaInfo
  
  结构体作用:  OTA所使用的具体数据类型
  
  特殊约定:    该结构在CAS Version 3.2版本之后未使用
*/
typedef struct _OtaInfo
{
	BYTE    	m_cSoftVer;							         /*软件版本*/
	DWORD   	m_dwDownLoadFrequency;					     /*下载升级数据的频点*/
	WORD    	m_wDownLoadSymbol;						     /*下载升级数据所在频点的符号率*/
	BYTE    	m_cDownLoadModulation;					     /*下载升级数据所在频点的调制方式*/
	WORD    	m_wDownLoadPid;						         /*下载升级数据的PID*/
	BYTE    	m_cDownLoadTableid;						     /*下载升级数据的TableID*/
	BYTE    	m_cDownLoadDuration;						 /*下载所需的时间周期*/
	T_CaDateTime  m_stIssueDate;						     /*下载升级成功的时间*/
}T_OtaInfo;


/*
  结构体名称:  CaSwitchByChnlPara_S
  
  结构体作用:  定义按频道跳转的各项属性值
  
  特殊约定:    无
*/
typedef struct
{    
    WORD    	m_wNetworkID; 			
   	WORD    	m_wTsID;           	
    WORD    	m_wServiceID;        
    WORD    	m_wEsPid; 		
}CaSwitchByChnlPara_S;


/*
  结构体名称:  CaSwitchByTunerPara_S
  
  结构体作用:  定义按频点跳转的各项属性值
  
  特殊约定:    无
*/
typedef struct
{    
    WORD    		m_wFreq; 							   /*频点 :单位:MHZ*/
    E_CaModulation   m_emModulation;         			   /*调制方式*/
    WORD    		m_wSymRate;           				   /*symbol rate :Ksps*/
    WORD    		m_wServiceID; 						   /*service id*/
}CaSwitchByTunerPara_S;


/*
  联合体名称:  CaChnlSwitchPara_U
  
  结构体作用:  按频点跳转和按频道跳转的各项参数值
  
  特殊约定:    无
*/
typedef union 
{
    CaSwitchByChnlPara_S   m_stSwitchByChnlPara;		   /*按节目跳转的参数*/
    CaSwitchByTunerPara_S  m_stSwitchByTunerPara;		   /*按频点跳转的参数*/
}CaChnlSwitchPara_U;


#define CA_MAX_TEXT_LEN               (256)

/*
  结构体名称:  CaChnlSwitch_S
  
  结构体作用:  强制跳转所需要的各项属性值
  
  特殊约定:    无
*/
typedef struct
{
    BOOL                   m_bIsCmdByTuner;				  /*FALSE: switch the channel by CaSwitchByChnlPara_S; TRUE:CaSwitchByTunerPara_S*/
    E_CaChnlSwitchType     m_emSwitchType;				  /*跳转类型*/
    T_CaDateTime           m_stStartDate;				  /*频道跳转开始时间,未使用*/
    T_CaDateTime           m_stExpireDate; 				  /*频道跳转截止时间,未使用*/
    BYTE                   m_acText[CA_MAX_TEXT_LEN]; 	  /*跳转提示信息内容，字符串数组*/
    CaChnlSwitchPara_U     m_uPara;						  /*频道跳转参数*/
}CaChnlSwitch_S;


/*
  结构体名称:  CaUserView_S
  
  结构体作用:  准禁播设置过程中所需要的各项属性值
  
  特殊约定:    无
*/
typedef struct
{
    E_CaUserViewOp         m_emOperation;				  /*用户操作模式*/
    E_CaUserViewStatus     m_emStatus;					  /*准禁播状态*/
    WORD                   m_wStartChnl;    			  /*开始加扰通道号*/
    WORD                   m_wEndChnl;					  /*结束加扰通道号*/
    T_CaDateTime           m_stStartDate;				  /*准禁播开始时间*/
    T_CaDateTime           m_stExpireDate; 				  /*准禁播截止时间*/
}CaUserView_S;


/**************************************************************************
*Function Name: CS_CA_GetCoreVer
*  获取算通CA内核版本信息
*
*Parameters 
*	pcVer[out]:
*		用于存储CA内核版本信息缓存 
*   pnLen[in|out]:
*       返回CA内核版本信息字符串长度
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
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
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:兼容老项目，新项目不需要调用此接口
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
*	邮件总数
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
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(INT nIndex, T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* 将所有邮件异步保存到flash中，在CS_CA_TXT_Add后调用
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:兼容老项目，新项目不需要调用此接口
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Save_Asyn
* 将所有邮件异步保存到flash中，在CS_CA_TXT_Add后调用
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:兼容老项目，新项目不需要调用此接口
**************************************************************************/
int CS_CA_TXT_Save_Asyn(void);

/**************************************************************************
*Function Name:CS_CA_TXT_SetReadFlag
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
INT CS_CA_TXT_SetReadFlag(INT nIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* 返回当前还有多少封未读邮件
*
*Parameters 
*	无
*
*Return Values: 
*   未读邮件个数
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum(void);

/**************************************************************************
*Function Name:CS_CA_MMI_CheckPinCode
* 请求校验PIN码
*
*Parameters 
*	dwPin[in]:输入PIN码
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		CA_STATUS_PIN_OK    PIN码正确
*		CA_STATUS_PIN_ERROR PIN码错误
**************************************************************************/
INT CS_CA_MMI_CheckPinCode(DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyPinCode
* 请求修改PIN码
*
*Parameters 
*	dwOldPin[in]:旧PIN码
*   dwNewPin[in]:新PIN码
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		CA_STATUS_MODIFY_PIN_SUCCESS   修改pin码成功
*		CA_STATUS_MODIFY_PIN_FAIL      修改pin码失败
**************************************************************************/
INT CS_CA_MMI_ModifyPinCode(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvConfirm
* 点播PPV节目
*
*Parameters 
*	dwIppvNo[in]:节目号
*   dwPin[in]:输入PinCode
*   bIsPpvp[in]:TRUE:请求IPPV-P，FALSE请求IPPV-T
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		CA_ALARM_ORDER_IPPV_SUCCESS       IPPVP点播成功
*		CA_ALARM_IPPV_PIN_ERROR           IPPV密码错误
*		CA_ALARM_IPPV_EXPIRED             IPPV节目过期
*		CA_ALARM_IPPV_LACKMONEY           IPPV点播金额不足
*		CA_ALARM_IPPVQUERYMODE_UNSUPPORT  IPPV点播方式不支持
*		CA_ALARM_ORDER_IPPV_FAIL          IPPV点播失败
**************************************************************************/
INT CS_CA_MMI_PpvConfirm(DWORD dwIppvNo, DWORD dwPin, BOOL bIsPpvp);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvCancel
* 结束IPPV-T节目点播
*
*Parameters 
*	dwIppvNo[in]:节目号
*   dwPin[in]:输入PinCode
*  
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:本接口只是结束IPPV-T节目的点播，IPPV-P点播后是不能取消的。
* 异步方式实现:
*		CA_ALARM_IPPV_PIN_ERROR           IPPV密码错误
*		CA_ALARM_STOP_IPPV_SUCCESS        IPPV结束成功
*		CA_ALARM_IPPVSTOP_FAIL            IPPV结束失败
**************************************************************************/
INT CS_CA_MMI_PpvCancel(DWORD dwIppvNo, DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestSmcInfo
* 请求智能卡信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		CA_INFO_SMC_STATUS   dwParm2 = (DWORD)&T_SmcInfo
*       T_SmcInfo.m_cSmcStatus为0的时候可获取智能卡信息，为其他值的时候是智能卡信息读取错误
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
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		正确的时候返回CA_INFO_PPID   dwParm2 = (DWORD)&T_PPIDList
*       错误的时候返回CA_STATUS_GET_PPID_INFO_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestPPIDInfo();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestDetailPPIDInfo
* 请求PPID详细信息(运营商详细信息)
*
*Parameters 
*	dwPin[in]:密码
*   dwPpId:运营商Id号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		正确的时候返回CA_INFO_PPID_DETAIL   dwParm2 = (DWORD)&T_DetailPPIDInfo
*       错误的时候返回
* 					CA_STATUS_READ_INFO_ERROR
*                   CA_STATUS_PIN_ERROR
*                   CA_STATUS_PPID_ERROR
*                   CA_STATUS_GET_DETAIL_PPID_INFO_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestDetailPPIDInfo(DWORD dwPin, DWORD dwPpId);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestDetailPPVInfo
* 请求PPV详细信息(运营商PPV节目的详细信息)
*
*Parameters 
*   dwPpId:运营商Id号
*	bType[in]:请求的节目类型，TRUE为PPV节目，FALSE为IPPV节目
*	ucListPage[in]:请求的节目列表页数，从0开始,PPV共8页，IPPV只有1页
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		正确的时候若是请求PPV节目返回CA_INFO_PPV_DETAIL,dwParm2 = (DWORD)&DetailPPVInfo_S
*                 若是请求IPPV节目返回CA_INFO_IPPV_DETAIL,dwParm2 = (DWORD)&DetailIPPVInfo_S
*       错误的时候返回
* 					CA_STATUS_READ_INFO_ERROR                  
*                   CA_STATUS_PPID_ERROR
*                   CA_STATUS_REQUEST_TYPE_ERROR
*                   CA_STATUS_REQUEST_DETAIL_PPV_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestDetailPPVInfo(DWORD dwPpId, BOOL bType, BYTE ucListPage);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestRecordSrvAllowedFlag
* 请求防录制标志
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		正确的时候返回
*					CA_STATUS_ALLOW_RECORD   
*					CA_STATUS_REJECT_RECORD   
*       错误的时候返回
* 					CA_STATUS_RECORD_FAIL
**************************************************************************/
INT CS_CA_MMI_RequestRecordSrvAllowedFlag();

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyParentRating
* 请求修改父母锁
*
*Parameters 
*	dwPin[in]:输入密码
*   cParentRatingValue[in]:要修改的级别,有效值4~18:年龄
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		正确的时候返回
*					CA_STATUS_MODIFY_PR_LEVEL_SUCCESS   
*       错误的时候返回
*					CA_STATUS_MODIFY_PR_LEVEL_FAIL   
* 					CA_STATUS_PIN_ERROR
**************************************************************************/
INT CS_CA_MMI_ModifyParentRating(DWORD dwPin, BYTE cParentRatingValue);

/**************************************************************************
*Function Name:CS_CA_MMI_RequestCurrentParentLevel
* 请求当前父母锁级别
*
*Parameters 
	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		CA_INFO_PARENTLEVEL   dwParm2 = (DWORD)dwParentLevel
**************************************************************************/
INT CS_CA_MMI_RequestCurrentParentLevel();

/**************************************************************************
*Function Name:CS_CA_MMI_RequestPlayLowLevelProg
* 请求播放父母锁控制的节目，当节目的父母锁级别过低不能观看时，调用该接口请求播放该节目
* 该接口会同时修改父母锁级别
*Parameters 
*	dwPin:输入密码
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
* 备注:异步方式实现:
*		正确的时候返回
*					CA_STATUS_MODIFY_PR_LEVEL_SUCCESS   
*       错误的时候返回
*					CA_STATUS_MODIFY_PR_LEVEL_FAIL   
* 					CA_STATUS_PIN_ERROR	
**************************************************************************/
INT CS_CA_MMI_RequestPlayLowLevelProg(DWORD dwPin);


/**************************************************************************
*Function Name:CS_CA_GetCurrentProgramNo
* 获取当前节目号，用于比较是否已换台
*
*Parameters 
*	无
*
*Return Values: 
*	节目号 
**************************************************************************/
INT CS_CA_GetCurrentProgramNo();

/**************************************************************************
*Function Name:CS_CA_MMI_CheckIsNewPPV
*判断当前的PPV 是否为新
*Parameters 
*	pstOldPPV  待比较的老PPV 信息
*	pstNewPPV 待比较的新PPV 信息

*Return Values:
	TRUE : PPV 节目有更新
	FALSE :PPV 节目未更新
*
**************************************************************************/
BOOL CS_CA_MMI_CheckIsNewPPV(T_PpvInfo * pstOldPPV, T_PpvInfo * pstNewPPV);

/**************************************************************************
*Function Name:CS_CA_TXT_SetRepeatCheck
*是否进行重复邮件判断
*Parameters 
*	bIsCheck 
*	TRUE ,进行重复邮件判断.
*	FALSE ,进行重复邮件判断.

*Return Values:
	SUCCESS :成功
	FAILURE :失败
*注意CA 的默认策略是进行重复判断，不允许重复接收邮件
*如需要接收重复邮件，则可以调用此函数进行修改。
**************************************************************************/
INT CS_CA_TXT_SetRepeatCheck(BOOL bIsCheck);

/**************************************************************************
*Function Name:CS_CA_GetFlashAddress
*CA模块依赖接口，项目集成时实现此函数供CA模块调用
*Parameters 
*	dwFlashSize: 指定要分配FLASH块的大小
*Return Values:
*	返回分配的起始地址，最好是分配一个Block的Flash，忽略参数dwFlashSize
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(DWORD dwFlashSize);

/**************************************************************************
*Function Name:get_software_version
*CA模块依赖接口，项目集成时实现此函数供CA模块调用，获取当前软件版本
*Parameters 
*	chSWVision: 用于存储当前软件版本号
*   nsizeSW   : 软件版本长度
*Return Values:
*	SUCCESS: 成功
*   其它   : 失败
*备注:CAS Version 3.2版本之后未使用,函数实现可置空
**************************************************************************/
DWORD get_software_version (char* chSWVision, int nsizeSW);

/**************************************************************************
*Function Name:get_hardware_version
*CA模块依赖接口，项目集成时实现此函数供CA模块调用，获取当前硬件版本
*Parameters 
*	chHWVision: 用于存储当前硬件版本号
*   nsizeHW   : 硬件版本长度
*Return Values:
*	SUCCESS: 成功
*   其它   : 失败
*备注:CAS Version 3.2版本之后未使用,函数实现可置空
**************************************************************************/
DWORD get_hardware_version (char* chHWVision, int nsizeHW);

/**************************************************************************
*Function Name:CS_CA_MMI_UserViewOp
*设置用户准播、禁播节目
*Parameters 
*   stUserView: CaUserView_S结构体
*   dwPin     : PIN码
*Return Values:
*   SUCCESS: 成功
*   FAILURE: 参数错误
* 备注:异步方式实现:
*	CA_INFO_USER_VIEW   dwParm2 = (DWORD)&CaUserView_S
*   stUserView.m_stStartDate.m_dwYear不小于2000,
*   要求开始时间小于结束时间，开始通道号小于结束通道号，否则应提示用户输入错误
**************************************************************************/
INT CS_CA_MMI_UserViewOp(CaUserView_S stUserView, DWORD dwPin);

/**************************************************************************
*Function Name:CSCAFactoryDefault
*恢复出厂设置时将CA数据区清空
*Parameters 
*  无
*Return Values:
*  无
**************************************************************************/
void CSCAFactoryDefault(void);

/**************************************************************************
*Function Name:CS_CA_GetMailFlashAddress
*CA模块依赖接口，项目集成时实现此函数供CA模块调用
*Parameters 
*   无
*Return Values:
*   返回分配的起始地址
**************************************************************************/
extern DWORD CS_CA_GetMailFlashAddress(void);

/**************************************************************************
*Function Name:CSCAGetTSAntiMoveCtrlCode
*获取码流里的区域码
*Parameters 
*   pcAntiMoveCtrl: 码流里的区域码
*Return Values:
*   SUCCESS: 成功
*   其它   : 失败
**************************************************************************/
INT CSCAGetTSAntiMoveCtrlCode(BYTE* pcAntiMoveCtrl);

/**************************************************************************
*Function Name:CSCaGetCurrentTime
*CA模块依赖接口，项目集成时实现此函数供CA模块调用
*Parameters 
*   stDateTime :当前EPG时间
*Return Values:
*   SUCCESS: 成功
*   其它   : 失败
**************************************************************************/
extern INT CSCaGetCurrentTime(T_CaDateTime *stDateTime);

#ifdef  __cplusplus
}
#endif

#endif //_CS_SUANTONGCTICA_INTERFACE_H_

