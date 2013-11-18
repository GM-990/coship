/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_SUANTONGCA_Interface.h  
* Author:      
* Date:        June 2007
* Description: SuanTong CA interface
*            
*****************************************************************************************************************************/

/****************************************************************************************************************************
* 			                                     Revision History                                                           *
*****************************************************************************************************************************
*   Revision  : V1.0
*   Programmer: 
*   Create    : Harry Peng
*****************************************************************************************************************************/
#ifndef _CS_SUANTONGCA_INTERFACE_H_
#define _CS_SUANTONGCA_INTERFACE_H_


#include "udiplus_typedef.h"
#include "cs_cainterface_pvrext.h"

#ifdef  __cplusplus
extern "C" {
#endif


#define CS_SUANTONGCA_MESSAGE_BASE    (0)
//hyb@090511
#define CA_MAX_TEXT_LEN                 (100)

typedef enum _CaEventGroup
{
	CA_ALARM = CS_SUANTONGCA_MESSAGE_BASE,    
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	CA_MAX_STATUS
}E_CaEventGroup;

typedef enum _CaInfo
{
	CA_INFO_SMC_STATUS = (CS_SUANTONGCA_MESSAGE_BASE + CA_MAX_STATUS + 1),     /*6 dwParm2 = (DWORD)&T_SmcInfo*/
	CA_INFO_SYSTEMID,        	/*dwParm2 = (DWORD)&T_CaSystemIdInfo,compliant with old version,not used now */
	CA_INFO_PPID,            	/*dwParm2 = (DWORD)&T_PPIDList*/
	CA_INFO_PPID_DETAIL,     	/*dwParm2 = (DWORD)&T_DetailPPIDInfo*/
	CA_INFO_TXT,             	/*10dwParm2 = (DWORD)&T_Msg*/
	CA_INFO_PARENTLEVEL,		/*dwParm2 = (DWORD)dwParentLevel*/
	CA_INFO_USER_VIEW,          /*dwParm2 = (DWORD)&CaUserView_S*///hyb@090511
	CA_INFO_MAX
}E_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_FINGER_PRINT  = (CS_SUANTONGCA_MESSAGE_BASE + CA_INFO_MAX + 1),   /*14 dwParm2 = (DWORD)&T_CaSpecInfo*/
	CA_COMMAND_CHANGE_PGPIN,   /*15dwParm2 = (DWORD)&T_CaPin , compliant with old version,not used now*/
	CA_COMMAND_IPPV_REQUEST,   /*16dwParm2 = (DWORD)&T_PpvInfo*/
	CA_COMMAND_PAIRING,        /*dwParm2 = (DWORD)&T_SercialInfo , compliant with old version,not used now*/
	CA_COMMAND_OTA,			   /*dwParm2 = (DWORD)&T_OtaInfo*/
    CA_COMMAND_CHNL_SWITCH,/*dwParm2 = (DWORD)&CaChnlSwitch_S*///hyb@090511

	CA_COMMAND_MAX	
}E_CaCommand;

typedef enum _CaStatus
{
	CA_STATUS_PPID_ERROR  = (CS_SUANTONGCA_MESSAGE_BASE + CA_COMMAND_MAX + 1),      //21        /*20 PPID Id Error*/
	CA_STATUS_READ_INFO_ERROR,         		/*Read Information Error*/
	CA_STATUS_PIN_ERROR,               		/*Pin Code Error*/
	CA_STATUS_PIN_OK,                  		/*Pin Code Ok*/
	CA_STATUS_MODIFY_PIN_SUCCESS,      		/*25Modify Pin Code Success*/
	CA_STATUS_MODIFY_PIN_FAIL,         		/*Modify Pin Code Fail*/
	CA_STATUS_SERIAL_ERROR,            		/*Serial Number Error*/
	CA_STATUS_SET_SERIAL_SUCCESS,      		/*28Set Serial Number Success*/
	CA_STATUS_SET_SERIAL_FAIL,         		/*29Set Serial Number Fail*/
	CA_STATUS_RECORD_FAIL,             		/*30Request Record Flag Fail*/
	CA_STATUS_ALLOW_RECORD,            		/*Allow Record*/
	CA_STATUS_REJECT_RECORD,           		/*Reject Record*/
	CA_STATUS_MODIFY_PR_LEVEL_SUCCESS, 		/*Modify Parent Rating Success*/
	CA_STATUS_MODIFY_PR_LEVEL_FAIL,    		/*Modify Parent Rating Fail*/
	CA_STATUS_GET_PPID_INFO_FAIL,      		/*35Get PPID Information Fail*/
	CA_STATUS_GET_DETAIL_PPID_INFO_FAIL, 	/*Get Detail PPID Information Fail*/
	
	CA_STATUS_MAX
	
}E_CaStatus;

typedef enum _CaAlarm
{
	CA_ALARM_FREE_PROGRAME  = (CS_SUANTONGCA_MESSAGE_BASE + CA_STATUS_MAX + 1),   /*38 E00 Free Programe*/
	CA_ALARM_SMC_OUT,             			/*E01 Please Insert Smart Card*/
	CA_ALARM_SMC_UNKNOW,          			/*40E02 Unknow Smart Card*/
	CA_ALARM_SMC_CHECKING,        			/*E03 Checking Smart Card*/
	
	CA_ALARM_SMC_OK,              			/*Smart Card Ok 04*/
	CA_ALARM_SMC_ERROR,          			/*Smart Card Error*/
	CA_ALARM_SMC_INFO_ERROR,     			/*44 Smart Card Information Error:SC_HARD_ERR*/
	CA_ALARM_SMC_STATUS_ERROR,   			/*45Smart Card Statuwords Error:SC_SOFT_ERR*/
	
	CA_ALARM_STB_SMC_NOT_MATCH  , 			/*E08 STB and SC not Match*/
	CA_ALARM_PR_LEVEL_TOO_LOW,      		/*E09 Parental Rating too lowly*/
	CA_ALARM_OTHER_CA_ENCRYPT,      		/*E10 Unknow encrypt Program*/
	
	CA_ALARM_ALLOCATE_ECM_CHANNEL_FAIL  , 	/*E11 Allocate ECM Channel/Filter Fail*/
	CA_ALARM_OPEN_DECRABLER_FAIL,           /*50E12 Open Descrambler Fail*/
	CA_ALARM_SMC_ERROR_DESCRABLING,         /*E13 SC Error when Descraming*/
	CA_ALARM_CW_ERROR,                      /*E14 CW Error*/
	CA_ALARM_NO_RIGHT,                      /*E15 Have No Decrypt Right*/
	CA_ALARM_DESCRAMBLE_SUCCESS,            /*E16 Descramble Success*/
	CA_ALARM_ALLOCATE_EMM_CHANNEL_FAIL,     /*55Alloc Emm Channel or Filter fail*/
	
	CA_ALARM_IPPV_PROGRAME ,  				/*E20 IPPV Program*/
	CA_ALARM_IPPV_PIN_ERROR,      			/*E21 IPPV PIN Error*/
	CA_ALARM_IPPV_EXPIRED,        			/*E22 IPPV Expired*/
	CA_ALARM_ORDER_IPPVP_FAIL,    			/*E23 Order IPPVP Fail*/
	CA_ALARM_ORDER_IPPVP_SUCCESS, 			/*60E24 Order IPPVP Success*/
	CA_ALARM_START_IPPVT_FAIL,    			/*E27 Order IPPV-T Fail*/
	CA_ALARM_START_IPPVT_SUCCESS, 			/*E26 Order IPPV-T Success*/
	CA_ALARM_PAY_IPPVT_FAIL,      			/*IPPVT Pay Fail*/
	CA_ALARM_STOP_IPPVT_FAIL,     			/*E29 End IPPV_T Fail*/
	CA_ALARM_STOP_IPPVT_SUCCESS,  			/*65E28 End IPPV-T Success*/

	CA_ALARM_NO_SIGNAL ,      				/*No Signal*/
	CA_ALARM_UNKNOW_SERVICE,      			/*Service Unknow*/

	CA_ALARM_TABLE_STATUS_OK ,   			/*Cat Or Pmt Status Ok*/
	CA_ALARM_CAT_STATUS_ERROR,       		/*Cat Status error*/
	CA_ALARM_STOP_PROGRAME_SUCCESS,  		/*70Stop program success*/

	CA_ALARM_STB_SMC_MATCH,          		/*STB and SC Match*/
	CA_ALARM_SMC_IN,          				/*Smart Card Insert*/

	CA_ALARM_OTA_AVAILABLE,					/*OTA Infomation Detect*/
	CA_ALARM_OTA_FAIL,						/*OTA Upgrate Failed */

	CA_ALARM_ST_PROGRAM, 					/*75If SUANTONG CA Encrypt*/

	CA_ALARM_ANTI_MOVE_CTRL,				/*E07 Anti move ctrl*/
	CA_ALARM_SON_CARD_OVERDUE,				/*E17 Son Card Authentication Overdue*/	
	CA_ALARM_USERVIEW_CONTROL_ERROR,		/*78E18 User View Control Error*/
	CA_ALARM_REGIONVIEW_CONTROL_ERROR,		/*E19 Region View Control Error*/
	CA_ALARM_CARD_PAUSE,					/*80E25 Smart Card Pause*/
	CA_ALARM_USERVIEW_SUCCESS,				/*E30 User View Success*/
	CA_ALARM_USERVIEW_FAIL,					/*E31 User View Fail*/
	CA_ALARM_SYSTEMCHECK,                   /*System checking, keep card in and power on*/
	CA_ALARM_SYSTEMCHECK_OVER,              /*System checking finished*/

	CA_ALARM_ERROR_MAX
}E_CaAlarm;
//hyb@090511
typedef enum _CaChnlSwitchType
{
    EM_CA_CHNL_FORCE_SWITCH_IMMEDIATELY= 0,      /*force switch the specify channel immediately */
    EM_CA_CHNL_FORCE_SWITCH_SPECIFY_TIME,        /*force switch the specify channel in the specify time*/
    EM_CA_CHNL_USER_SWITCH_IMMEDIATELY,          /*switch the specify channel immediately according to  user's choice*/
    EM_CA_CHNL_USER_SWITCH_SPECIFY_TIME          /*switch the specify channel in the specify time according to user's choice*/
}E_CaChnlSwitchType;
typedef enum _CaModulation
{
    EM_CA_MODULATION_QAM= 1,      /*QAM */
    EM_CA_MODULATION_QPSK,        /*QPSK*/
    EM_CA_MODULATION_BROADCAST,/*broadcast*/
    EM_CA_MODULATION_MOBILE/*mobile net*/
}E_CaModulation;
typedef enum _CaUserViewOp
{
    EM_CA_USER_VIEW_GET_INFO= 0,      /*get the infomation of user view*/
    EM_CA_USER_VIEW_SET_PARA,        /*set the parameters of user view*/
    EM_CA_USER_VIEW_CANCEL_SETTING/*cancel the settings*/
}E_CaUserViewOp;

typedef enum _CaUserViewStatus
{
    EM_CA_ALLOW_VIEW_EVERYDAY = 0,/*allow user view in a specifiedl time everyday*/
    EM_CA_ALLOW_VIEW_INTRADAY,        /*allow user view in a specifiedl time only today*/
    EM_CA_FORBIDDEN_VIEW_EVERYDAY,/*forbidden user view in a specifiedl time everyday*/
    EM_CA_FORBIDDEN_VIEW_INTRADAY       /*allow user view in a specifiedl time only today*/
}E_CaUserViewStatus;

/*not used in CAS Version 3 or above,start */
#define CA_MAX_SYSTEMID_COUNT_LEN  (16)

typedef struct _CurrentCaSystemInfo
{
	WORD     m_wSystemId;//算通CA系统ID
	WORD     m_wEcmSubsystemId;//算通CA子系统ID(for ecm)
	WORD     m_wEmmSubsystemId;//算通CA献拥统ID(for emm)
}T_CurrentCaSystemInfo;

typedef struct _CaSystemIdInfo
{
	BYTE     m_cCount;
	T_CurrentCaSystemInfo  m_stCaSystemInfo[CA_MAX_SYSTEMID_COUNT_LEN];
}T_CaSystemIdInfo;
/*not used in CAS Version 3 or above,end */

#define CA_MAX_PPID_NAME_LEN       (17)
typedef struct _PpvInfo
{
	INT      m_nProgramNo;//当前节目号
	BYTE     m_cStatus;//0:Credit and OverCredit are Valid; 1: Credit and OverCredit are Invalid
	DWORD    m_dwPPID_Id;//当前IPPV节目提供商的ID
	BYTE     m_acPPID_Label[CA_MAX_PPID_NAME_LEN];//节目提供商的名字,字符串数组
	FLOAT    m_fCredit;     /*余额*/
	FLOAT    m_fOverCredit; /*可透支额:这两个字段都不能显示给用户看，主要用于判断*/
	DWORD    m_dwIppvNo;//IPPV节目号
	FLOAT    m_fIppv_P_Price;//按节目点播IPPV的价格
	FLOAT    m_fIppv_T_Price;//按时间点播的IPPV价格
}T_PpvInfo;

#define CA_MAX_PINCODE_LEN     (4)
typedef struct _PinCodeInfo
{
	BYTE     m_cPinLen;//pin码长度
	BYTE     m_acPinCode[CA_MAX_PINCODE_LEN];//pin码，16进制数据
}T_PinCodeInfo;

#define	 CA_MAX_FINGERPRINT_LEN		  (10)
typedef struct _CaSpecInfo
{
	BOOL     m_bPriporityHigh;//not used 
	WORD     m_wDuration; //显示时间 单位:second
	BYTE     m_acMsg[CA_MAX_FINGERPRINT_LEN];//内容,16进制数据，应用直接显示16进制数据即可
}T_CaSpecInfo;

/*Mail*/
#define CA_MAIL_FROM_LEN     (200)
#define CA_MAIL_TITLE_LEN    (200)
#define CA_MAIL_TXT_LEN      (1024)

typedef struct _CaDateTime
{
	DWORD    m_dwYear;
	BYTE     m_cMonty;
	BYTE     m_cDay;
	BYTE     m_cHour;
	BYTE     m_cMinute;
    //hyb@090511, add two parameters for force switch channel    
    BYTE     m_cSecond;
    BYTE     m_cFiveMillisecs;//five milliseconds
}T_CaDateTime;

typedef struct _Msg
{
	BOOL     m_bPriporityHigh;  //FALSE: Mail  TRUE: Subtitle
	DWORD    m_dwFromLen;//邮件发送者长度
	BYTE     m_szFrom[CA_MAIL_FROM_LEN];//邮件发送者，字符串数组
	DWORD    m_dwTitleLen;//邮件标题长度
	BYTE     m_szTitle[CA_MAIL_TITLE_LEN];//邮件标题，字符串数组
	DWORD    m_dwContentLen;//邮件内容长度
	BYTE     m_szContent[CA_MAIL_TXT_LEN];//邮件内容，字符串数组
	T_CaDateTime    m_stCreatTime;  //邮件生成时间
	T_CaDateTime    m_stSendTime;   //邮件发送时间
}T_Msg;

typedef struct _CaMail
{	
	BOOL            m_bReadFlag;    //已读标志
	T_CaDateTime    m_stArriveTime; //邮件到达的时间
	T_Msg           m_stMsg;//邮件
}T_CaMail;

#define CA_MAX_ECM_LEN         (64)
#define CA_MAX_EMM_LEN         (64)
#define CA_MAX_SMC_VER_LEN     (9)
#define CA_MAX_SMC_NUMBER_LEN  (22)
#define CA_MAX_SMC_NAME_LEN    (17)
#define CA_MAX_SMC_ISSURE_TIME (17)
#define CA_MAX_SMC_EXPIRE_TIME (17)
#define CA_MAX_SMC_ANTIMOVE_LEN (3)

typedef struct _SmcInfo
{
	BYTE      m_cSmcStatus;//智能卡状态:0 SC_OK,1 SC_OUT,2 SC_HARD_ERR,3 SC_SOFT_ERR
	WORD      m_wSystemId;//算通CA系统ID
	WORD      m_wEcmSubSystemId;//算通CA子系统ID(for ecm)
	WORD      m_wEmmSubSystemId;//算通CA子系统ID(for emm)
	BYTE      m_acSmcVer[CA_MAX_SMC_VER_LEN];//智能卡版本号，字符串数组
	BYTE      m_acSmcNumber[CA_MAX_SMC_NUMBER_LEN];//智能卡号，十进制字符串
	BYTE      m_acSmcName[CA_MAX_SMC_NAME_LEN];//智能卡类型描述，字符串数组
	BYTE      m_acSmcIssureTime[CA_MAX_SMC_ISSURE_TIME];//智能卡发卡时间，字符串数组
	BYTE      m_acSmcExpireTime[CA_MAX_SMC_EXPIRE_TIME];//智能卡有效期截止时间，字符串数组
	BYTE      m_cParentalRating;//智能卡内父母锁级别
	BYTE      m_cPairStatus;//0:未匹配,1:不匹配,2:匹配
	BYTE      m_cGroupControl;//节目分组搜索控制码
	BYTE      m_acAntiMove[CA_MAX_SMC_ANTIMOVE_LEN];//禁止智能卡异地漫游控制代码，16进制数据
}T_SmcInfo;

#define CA_MAX_PPID_NUMBER_LEN       (10)
typedef struct _PPIDInfo
{
	DWORD    m_dwPPIDId;//节目提供商ID
	BYTE     m_acPPIDName[CA_MAX_PPID_NAME_LEN];//节目提供商名称，字符串数组
}T_PPIDInfo;

typedef struct _PPIDList
{
	BYTE         m_cCount;//节目提供商个数
	T_PPIDInfo   m_stPPIDInfo[CA_MAX_PPID_NUMBER_LEN];//节目提供商信息
}T_PPIDList;

#define CA_PPID_LAST_CHANGE_CREDIT_DATE_LEN  (11)
#define CA_PPID_START_AUTHOR_DATE_LEN        (11)
#define CA_PPID_END_AUTHOR_DATE_LEN          (11)
#define CA_PPID_PROGRAME_NUMBER_LEN          (8)
#define CA_PPID_IPPV_NUMBER_LEN              (32)

typedef struct _PpvProgram
{
	DWORD    m_dwStartPpvNo;//PPV起始节目号，如果为0xffffffff或者为20x00000000时无效
	DWORD    m_dwEndPpvNo;//PPV终止节目号，如果为0xffffffff或者为20x00000000时无效
}T_PpvProgram;

typedef struct _DetailPPIDInfo
{
	DWORD    m_dwPPIDId;//节目提供商ID
	FLOAT    m_fCredit;//某一节目提供商在卡内的金额
	BYTE     m_acCreditDate[CA_PPID_LAST_CHANGE_CREDIT_DATE_LEN];//某一节目提供商最近一次变动卡内金额的日期，字符串数组
	BYTE     m_cClassNumber;//已经授权的节目的数量
	DWORD 	 m_adwRightInfo[CA_PPID_PROGRAME_NUMBER_LEN];//授权信息
	BYTE     m_acProgStartDate[CA_PPID_START_AUTHOR_DATE_LEN];//授权起始日期，字符串数组
	BYTE     m_acProgEndDate[CA_PPID_END_AUTHOR_DATE_LEN];//授权截止日期，字符串数组
	BYTE     m_cPpvCount;//预定的ppv节目的节目个数，最大为256(由智能卡决定)
	T_PpvProgram m_stPpvProgram[CA_PPID_IPPV_NUMBER_LEN];//ppv节目信息
	BYTE     m_cPpvtCount;//按节目点播的ippv节目的个数
	DWORD    m_adwPpvtNo[CA_PPID_IPPV_NUMBER_LEN];//按节目点播的ippv节目的节目号，如果为0xffffffff或者为20x00000000时无效
}T_DetailPPIDInfo;

#define CA_PAIRING_SERCIAL_NUMBER_LEN     (8)
typedef struct _SercialInfo
{
	INT      m_nLen;//机顶盒序列号长度
	BYTE     m_acSercialNum[CA_PAIRING_SERCIAL_NUMBER_LEN];//机顶盒序列号，16进制数据
}T_SercialInfo;

typedef struct _OtaInfo
{
	BYTE   m_cSoftVer;//软件版本
	DWORD   m_dwDownLoadFrequency;//下载升级数据的频点
	WORD   m_wDownLoadSymbol;//下载升级数据所在频点的符号率
	BYTE   m_cDownLoadModulation;//下载升级数据所在频点的调制方式
	WORD   m_wDownLoadPid;//下载升级数据的PID
	BYTE   m_cDownLoadTableid;//下载升级数据的TableID
	BYTE   m_cDownLoadDuration;//
	T_CaDateTime m_stIssueDate;//
}T_OtaInfo;
//hyb@090511
typedef struct
{    
    WORD    m_wNetworkID; 
    WORD    m_wTsID;           
    WORD    m_wServiceID;           
    WORD    m_wEsPid; 
}CaSwitchByChnlPara_S;
typedef struct
{    
    WORD    m_wFreq; //frequency :MHZ
    E_CaModulation    m_emModulation;           
    WORD    m_wSymRate;           //symbol rate :Ksps
    WORD    m_wServiceID; 
}CaSwitchByTunerPara_S;

typedef union 
{
    CaSwitchByChnlPara_S              m_stSwitchByChnlPara;//按节目跳转的参数
    CaSwitchByTunerPara_S           m_stSwitchByTunerPara;//按频点跳转的参数
}CaChnlSwitchPara_U;

typedef struct
{
    BOOL                   m_bIsCmdByTuner;/*false: switch the channel by CaSwitchByChnlPara_S; true:CaSwitchByTunerPara_S*/
    E_CaChnlSwitchType     m_emSwitchType;//跳转类型
    T_CaDateTime           m_stStartDate;//频道跳转开始时间
    T_CaDateTime           m_stExpireDate; // 频道跳转截止时间  
    BYTE                   m_acText[CA_MAX_TEXT_LEN]; //  文本字符串，，字符串数组
    CaChnlSwitchPara_U     m_uPara;//频道跳转参数
}CaChnlSwitch_S;

typedef struct
{
    E_CaUserViewOp        m_emOperation  ;//用户操作模式
    E_CaUserViewStatus    m_emStatus;//准禁播状态
    WORD                   m_wStartChnl;    //开始加扰通道号
    WORD                   m_wEndChnl;//结束加扰通道号
    T_CaDateTime           m_stStartDate;//准禁播开始时间
    T_CaDateTime           m_stExpireDate; // 准禁播截止时间

}CaUserView_S;


/**************************************************************************
*Function Name: CS_CA_GetCoreVer
*  获取算通CA内核版本信息
*
*Parameters 
*	pcVer[out]:
*		用于存储CA内核版本信息缓存 
*   pnLen[in|out]:
*       输入初始值为30，返回真实值
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
*   nCount:未读邮件个数
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum( );

/**************************************************************************
*Function Name:CS_CA_MMI_CheckPinCode
* 请求校验密码
*
*Parameters 
*	pstPinCode[in]:输入密码
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE，备用返回值，目前没有用到
* 备注:异步方式实现:
*		CA_STATUS_PIN_OK   校验pin码成功
*		CA_STATUS_PIN_ERROR pin码错误
**************************************************************************/
INT CS_CA_MMI_CheckPinCode(DWORD dwPin);

/**************************************************************************
*Function Name:CS_CA_MMI_ModifyPinCode
* 请求修改密码
*
*Parameters 
*	pstOldPin[in]:旧密码
*   pstNewPin[in]:新密码
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE，备用返回值，目前没有用到
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
*   pstPinCode[in]:输入PinCode
*   bIsPpvp[in]:TRUE:请求PPVP，FALSE请求PPVT
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE，备用返回值，目前没有用到
* 备注:异步方式实现:
* 如果是PPVP
*		CA_ALARM_ORDER_IPPVP_SUCCESS   IPPVP点播成功
*		CA_ALARM_ORDER_IPPVP_FAIL      IPPVP点播失败
*		CA_ALARM_IPPV_PIN_ERROR        pin码错误
*		CA_ALARM_IPPV_EXPIRED          IPPVP节目过期
*如果是PPVT
*		CA_ALARM_START_IPPVT_SUCCESS   IPPVT点播成功
*		CA_ALARM_ORDER_IPPVP_FAIL      IPPVT点播失败
*		CA_ALARM_IPPV_PIN_ERROR        pin码错误
*		CA_ALARM_IPPV_EXPIRED          IPPVP节目过期
**************************************************************************/
INT CS_CA_MMI_PpvConfirm(DWORD dwIppvNo, DWORD dwPin, BOOL bIsPpvp);

/**************************************************************************
*Function Name:CS_CA_MMI_PpvCancel
* 结束PPVT节目点播
*
*Parameters 
*	dwIppvNo[in]:节目号
*   pstPinCode[in]:输入PinCode
*  
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE，备用返回值，目前没有用到
* 备注:本接口只是结束IPPVT节目的点播，IPPVP点播后是不能取消的。
* 异步方式实现:
*		CA_ALARM_IPPV_PIN_ERROR        pin码错误
*		CA_ALARM_STOP_IPPVT_SUCCESS    结束IPPVT成功
*		CA_ALARM_STOP_IPPVT_FAIL       结束IPPVT失败
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
*	-1: FAILURE，备用返回值，目前没有用到
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
*	-1: FAILURE，备用返回值，目前没有用到
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
*	pstPin[in]:密码
*   dwPpId:运营商Id号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE，备用返回值，目前没有用到
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
*Function Name:CA_CA_MMI_RequestRecordSrvAllowedFlag
* 请求防录制标志
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE，备用返回值，目前没有用到
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
*	pstPin[in]:输入密码
*   cParentRatingValue[in]:要修改的级别
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE，备用返回值，目前没有用到
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
* 请求修改父母锁级别
*
*Parameters 
	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE，备用返回值，目前没有用到
* 备注:异步方式实现:
*		CA_INFO_PARENTLEVEL   dwParm2 = (DWORD)dwParentLevel
**************************************************************************/

INT CS_CA_MMI_RequestCurrentParentLevel();

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
*	返回分配的起始地址
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
**************************************************************************/
DWORD get_hardware_version (char* chHWVision, int nsizeHW);
/**************************************************************************
*Function Name:CS_CA_MMI_UserViewOp
*设置用户准播、禁播节目
*Parameters 
*   stUserView: CaUserView_S结构体
*   dwPin   : 密码
*Return Values:
*   SUCCESS: 成功
*   FAILURE: 参数错误
* 备注:异步方式实现:
*		CA_INFO_USER_VIEW   dwParm2 = (DWORD)&CaUserView_S
**************************************************************************/
INT CS_CA_MMI_UserViewOp(CaUserView_S stUserView,DWORD dwPin);
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
*   [out]pstDateTime:保存当前的EPG时间
*Return Values:
*   SUCCESS:0，获得EPG时间成功
*   FAILURE:-1，获得EPG时间失败
**************************************************************************/
extern INT CSCaGetCurrentTime(T_CaDateTime *pstDateTime);


#ifdef  __cplusplus
}
#endif

#endif //_CS_SUANTONG_INTERFACE_H_

