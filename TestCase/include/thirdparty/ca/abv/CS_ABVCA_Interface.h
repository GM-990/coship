/**@defgroup ABV CA interface
@brief ABVCA对应用提供头文件
@note  Copyright Shenzhen Coship Electronics Co., Ltd., 

@version 1.0  2011/12/31  Yuhongdan/Yuanqinlong
@{
*/

#ifndef _CS_ABVCA_INTERFACE_H_
#define _CS_ABVCA_INTERFACE_H_

#include "udiplus_typedef.h"
#include "cs_cainterface.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief CA Message主类型*/
typedef enum _CaMessage
{
	CA_COMMAND,
	CA_ALARM,
	CA_STATUS
}CaMessage_E;

/**@brief ALARM消息
@note 当收到节目不能观看的错误消息时，由消息回调的第3个参数给出
@停止音频、视频还是音视频以区分音视频单独加扰的情况*/
typedef enum _CaAlarm
{
	CA_ALARM_CA_NO_ERR,                ///< 0清除提示
	CA_ALARM_STB_SC_NO_PAIRED, 	       ///< 1机卡不匹配(节目不相关)
	CA_ALARM_SC_ERROR, 			       ///< 2错误的智能卡(节目不相关)
	CA_ALARM_NO_SC, 			       ///< 3请插入智能卡(节目不相关)
	CA_ALARM_READ_WRITER_ERR, 	       ///< 4卡交互失败(节目不相关)
	CA_ALARM_NOT_IN_WATCH_TIME,        ///< 5不在设置的节目观看时间之内(节目不相关)
	CA_ALARM_CARD_OUT_OF_DATE, 	       ///< 6智能卡过期(节目相关)
	CA_ALARM_NO_ENTITLE, 		       ///< 7没有授权(节目相关)
	CA_ALARM_STB_AREA_RESTRECT,        ///< 8区域阻塞(节目相关)
	CA_ALARM_CA_NO_PAIRED, 		       ///< 9需要机卡配对(节目相关)
	CA_ALARM_ECM_UNKNOWEN_ERROR,       ///< 10错误的节目信息(节目相关)
	CA_ALARM_NOT_CONTACT_MASTER,       ///< 11没有母卡(节目相关) 
	CA_ALARM_IPPV_PRO_NO_ENOUGH_TOKEN, ///< 12IPPV 节目,余额不足(节目相关)
	CA_ALARM_WAIT_ENTITLE,	           ///< 13等待接收授权(节目相关)
	CA_ALARM_SRV_FREE,                 ///< 14清流节目
	CA_ALARM_SRV_OTHER,		           ///< 15其它CA加扰节目
	CA_ALARM_SRV_ABV,                  ///< 16本CA加扰节目
	CA_ALARM_MAX
	
}CaAlarm_E;

/**@brief COMMAND消息*/
typedef enum  _CaCommand
{
    CA_COMMAND_NEW_EMAIL,		  ///< 0(节目不相关)显示或隐藏新邮件图标dwParm2=1显示，dwParm2=2隐藏
 	CA_COMMAND_FULL_EMAIL,		  ///< 1(节目不相关)显示或隐藏满邮件图标dwParm2=1显示，dwParm2=2隐藏
	CA_COMMAND_ECM_FINGERPRINT,	  ///< 2(节目相关)ECM指纹 dwParm2 = (DWORD)&ECM_FINGERPRINT_S
	CA_COMMAND_ECM_FP_CANCEL,	  ///< 3(节目相关)擦除ECM指纹显示
	CA_COMMAND_EMM_FINGERPRINT,	  ///< 4(节目不相关)EMM指纹 dwParm2 = (DWORD)&EMM_FINGERPRINT_S
	CA_COMMAND_EMM_FP_CANCEL,	  ///< 5(节目不相关)擦除EMM指纹显示，dwParm2 =1取消所有emmpf, dwParm2=2取消channel的emmpf
	CA_COMMAND_SHOW_SUBTITLE,	  ///< 6(节目不相关)显示Subtitle信息, dwParm2 = (DWORD)&CaSubtitleInfo_S
	CA_COMMAND_OTA,				  ///< 7(节目不相关)OTA信息 dwParm2 = (DWORD)&CaOtaInfo_S
	CA_COMMAND_IPPV_NOTIFY,		  ///< 8(节目相关)IPPV购买信息 ,需要定义CAIPPVProInfo_S[2]来获取信息，即2个CAIPPVProInfo_S大小数据结构
	                              ///< 应用需要弹出1个或者2个ppv的购买框，具体说明如下
	            				  /**@note 例子如下
	            				     @CAIPPVProInfo_S sCAIPPVProInfo[2];
				                     @memset(sCAIPPVProInfo, 0, sizeof(CAIPPVProInfo_S)*2);
				                     @sCAIPPVProInfo[0] = *(CAIPPVProInfo_S*)dwParm2;                          //当前的ppv
				                     @sCAIPPVProInfo[1] = *(CAIPPVProInfo_S*)(dwParm2+sizeof(CAIPPVProInfo_S));//下一个ppv
				                     @如果获取到的sCAIPPVProInfo[i].m_wIPPVID == 0xffff 表明这个ppv是不存在的，应用不需要显示处理
				                     @弹出框后，可以购买(调用CSCAConfirmBuyIPPV)或取消(调用CSCACancelWatchProgram)
				                     @具体调用说明方式参见接口说明 
				                     */
	CA_COMMAND_CLEAR_IPPV_SHOW,   ///< 9(节目相关)取消ippv对话框
	CA_COMMAND_MATURE_NOTIFY,     ///< 10(节目相关)成人级节目，
	                             /**@note 应用需要有提示框让用户输入pin码
	                                @确认pin码并请求继续观看调用CSCAConfirmWatchMrProgram
	                                @取消观看调用CSCACancelWatchProgram
	                              	*/
	CA_COMMAND_CHANNEL_EMM_FP,	 ///< 11(节目相关)节目用户指纹 dwParm2 = (DWORD)&ChannelEMM_FINGERPRINT_S
	CA_COMMAND_MAX
}CaCommand_E;

/**@brief STATUS消息*/
typedef enum _CaStatus
{
	CA_STATUS_CARD_IN_BEGIN, 			///< 0(节目不相关)卡开始初始化(这个消息可以不用显示)
	CA_STATUS_CARD_IN_END_SUCCESS,      ///< 1(节目不相关)卡初始化成功(这个消息3s后必须消失)
	CA_STATUS_CARD_IN_END_FAIL, 		///< 2(节目不相关)卡初始化失败
	CA_STATUS_SC_PIN_RESET, 			///< 3(节目不相关)密码重置成功(这个消息3s后必须消失)
	CA_STATUS_STB_SC_PAIRED,			///< 4(节目不相关)机卡配对成功(这个消息3s后必须消失)
	CA_STATUS_STB_SC_FREE_PAIRED, 	    ///< 5(节目不相关)解除机卡配对成功(这个消息3s后必须消失)
	CA_STATUS_STB_SC_PAIRED_FAILED, 	///< 6(节目不相关)机卡配对失败
	CA_STATUS_MAX
}CaStatus_E;

/**@note 消息的显示说明
@AbvCa的消息的提示是要和serviceid联系起来的
@应用需要调用接口INT CSCARegisterMsgCallback(DWORD dwUserData, F_CBMsg fcbMsg)注册回调
@应用可以通过回调得到数据的最后一个参数dwparm4得到节目id:	
@DWORD serviceId;
@serviceId =  (*(FuncbParam_S*)dwparm4).m_hService; 
@当serviceId = 0时表示和节目无关，所有节目需要提示;
@当serviceId != 0时要和当前的节目id比较，与当前节目ID相同才提示
*/

typedef enum
{
	EM_ABVCA_STOPAV,      ///< 0.不能观看节目时CA_ALARM消息需要停音视频，由消息回调的第3个参数给出
	EM_ABVCA_STOPAUDIO,   ///< 1.不能观看节目时CA_ALARM消息需要停音频，由消息回调的第3个参数给出
	EM_ABVCA_STOPVIDEO    ///< 2.不能观看节目时CA_ALARM消息需要停视频，由消息回调的第3个参数给出
}ABVCAAVSTOP_E;

/**@brief 时间结构体*/
typedef struct  _CaTime
{
	WORD		m_wYear;
	BYTE		m_ucMonth;
	BYTE		m_ucDay;
	BYTE		m_ucHour;
	BYTE		m_ucMinute;
}CATime_S;

#define CA_MAX_ECM_SIZE_FINGERPRINT      (17)
#define CA_MAX_EMM_SIZE_FINGERPRINT      (1024)

/**@brief ECM(节目)指纹信息*/
typedef struct _CaEcmFingerprint 
{
	BYTE    	m_acMsg[CA_MAX_ECM_SIZE_FINGERPRINT]; ///< 指纹内容,字符串数组
	WORD		m_wLocationX;	                      ///< 横坐标位置
	WORD		m_wLocationY;                        ///< 纵坐标位置
	BOOL        m_ucType;                             ///< 类型:TRUE表明需要按获取到的坐标显示;FALSE自己规定显示位置
}ECM_FINGERPRINT_S;

/**@brief EMM(用户)指纹信息
@note Emm字体和背景颜色说明范围(0x0~0xf)
@0 {0xff, 0xff, 0xff}, WHITE 白
@1 {0xc0, 0xc0, 0xc0}, LTGRAY 灰白
@2 {0x00, 0xff, 0xff}, LTCYAN 青
@3 {0x00, 0x80, 0x80}, CYAN 藏青
@4 {0xff, 0x00, 0xff}, LTMAGENTA 品红
@5 {0x80, 0x00, 0x80}, MAGENTA 紫
@6 {0x00, 0x00, 0xff}, LTBLUE 蓝
@7 {0x00, 0x00, 0x80}, BLUE 深蓝
@8 {0xff, 0xff, 0x00}, YELLOW 黄
@9 {0x80, 0x80, 0x00}, BROWN 橄榄绿
@a {0x00, 0xff, 0x00}, LTGREEN 绿
@b {0x00, 0x80, 0x00}, GREEN 深绿
@c {0xff, 0x00, 0x00}, LTRED 红
@d {0x80, 0x00, 0x00}, RED 深红
@e {0x80, 0x80, 0x80}, GRAY 深灰
@f {0x00, 0x00, 0x00}, BLACK 黑--透明色
@如果获取不到内容显示卡号
*/
typedef struct _CaEmmFingerprint 
{
	BYTE    	m_acMsg[CA_MAX_EMM_SIZE_FINGERPRINT]; ///< 指纹内容,字符串数组
	WORD		m_wcLocationX;	                      ///< 横坐标位置
    WORD		m_wcLocationY;                        ///< 纵坐标位置
	WORD     	m_wDuration;  	                      ///< 指纹显示的持续时间,单位秒.
	WORD     	m_wRepetition; 	                      ///< 指纹显示的重复次数
	WORD     	m_wInterval; 	                      ///< 两次指纹显示间的时间间隔
	BYTE		m_ucFontColour;                       ///< 字体颜色
	BYTE		m_ucBgColour;	                      ///< 背景颜色
	BOOL        m_bfontFlash;                         ///< 字体是否闪烁,当闪烁为TRUE时
	BYTE        m_ucStyle;                            ///< 编码方式-----具体看邮件信息的说明
}EMM_FINGERPRINT_S;

typedef struct _CaChannelEmmFingerprint 
{
	BYTE		m_acMsg[CA_MAX_EMM_SIZE_FINGERPRINT+17]; ///< 指纹内容,字符串数组
	WORD		m_wcLocationX;						  ///< 横坐标位置
	WORD		m_wcLocationY;						  ///< 纵坐标位置
	WORD		m_wDuration;						  ///< 指纹显示的持续时间,单位秒.
	WORD		m_wRepetition;						  ///< 指纹显示的重复次数
	WORD		m_wInterval;						  ///< 两次指纹显示间的时间间隔
	BYTE		m_ucFontColour; 					  ///< 字体颜色
	BYTE		m_ucBgColour;						  ///< 背景颜色
	BOOL		m_bfontFlash;						  ///< 字体是否闪烁,当闪烁为TRUE时
	BYTE		m_ucStyle;							  ///< 编码方式-----具体看邮件信息的说明
}ChannelEMM_FINGERPRINT_S;


#define MAX_EMAIL_LEN        (1024)
#define MAX_SENDERNAME_LEN   (20)  
#define MAX_MAILTITLE_LEN    (50)
#define MAX_EMAIL_NUM        (100)

/**@brief 邮件正文信息*/
typedef struct _CaMailContentInfo_S
{
	BYTE        m_byStyle;///< 编码类型
	                      ///< 0x00(ASCII)        支持文字:English,Indonesian
	                      ///< 0x05(ISO_8859_9)   支持文字:French,German,Turkish,Spanish,Dutch,Italian,Portuguese
	                      ///< 0x03(ISO_8859_7)   支持文字:Greek
	                      ///< 0x01(ISO_8859_5)   支持文字:Russian
	                      ///< 0x02(ISO_8859_6)   支持文字:Arabic,persian
	                      ///< 0x06(ISO_8859_2)   支持文字:polish,Czech
	                      ///< 0x07(ISO_8859_11)  支持文字:Thai
	                      ///< 0x12(GB2312)       支持文字:Chinese
	WORD        m_acEmailLen;
	BYTE		m_acEmailContent[MAX_EMAIL_LEN+1];	///< Email的正文 
}CaMailContentInfo_S;

/**@brief 邮件头信息*/
typedef struct _CaMailHeadInfo_S                       
{
	BOOL			m_bReadFlag;                        ///< 邮件状态标志0:未读，1:已读
	WORD			m_wEmailID;                         ///< 邮件 ID 
	CATime_S		m_sMailTime;                        ///< 邮件发送时间
	BYTE     		m_ucSenderLen;                      ///< 发送者姓名长度
	BYTE     		m_acSenderName[MAX_SENDERNAME_LEN+1]; ///< 发送者姓名
	BYTE    		m_ucTitleLen;                 	    ///< 标题长度
	BYTE     		m_acTitle[MAX_MAILTITLE_LEN+1];       ///< 标题
}CaMailHeadInfo_S;

#define MAX_SUBDATA_LEN (1024)

/**@brief Subtile--屏显信息*/
typedef struct _CaSubtitleInfo 
{
	WORD	m_wSubLen;                    ///< 总长度，应用可以忽略
	WORD	m_wSubID;                     ///< Subtitle ID  
	WORD	m_wDuration;  	              ///< Subtitle显示的持续时间,单位秒.
	WORD	m_wRepetition; 	              ///< Subtitle显示的重复次数
	WORD	m_wInterval; 		          ///< Subtitle指纹显示间的时间间隔
	WORD	m_wFlashLen;                  ///< 长度
	BYTE 	m_acSubdata[MAX_SUBDATA_LEN]; ///< Subtitle数据
	BYTE    m_ucStyle;                    ///< 编码方式
}CaSubtitleInfo_S;

#define CA_MAX_LABLE_LEN  (8)
#define CA_MAX_SMC_NUMBER_LEN  (16)
#define CA_MAX_SMC_NAME_LEN  (5)


/**@brief 智能卡基本信息*/
typedef struct _SmcInfo
{
	BYTE      	m_acLabel[CA_MAX_LABLE_LEN+1];           ///< 供应商名称
	BYTE      	m_acSmcNumber[CA_MAX_SMC_NUMBER_LEN+1];  ///< 智能卡号
	BYTE      	m_acSmclName[CA_MAX_SMC_NAME_LEN+1];     ///< 智能卡名称
	BYTE	    m_ucVersion;		                     ///< 智能卡版本号
	BYTE		m_ucAdultlevel;	                         ///< 智能卡成人级别
	BYTE		m_ucAreal;		                         ///< 智能卡区域码
	BOOL		m_bPaired;		                         ///< 智能卡是否配对 TRUE:配对了  FALSE:未配对
	BYTE		m_acStartHour;                           ///< 观看时间开始时间的小时位,
	BYTE		m_acStartMinute;                         ///< 观看时间开始时间的分钟位,
	BYTE		m_acEndHour;                             ///< 观看时间结束时间的小时位,
	BYTE		m_acEndMinute;                           ///< 观看时间结束时间的分钟位,
	BOOL		m_bIsChildSC;	                         ///< 智能卡是否是子卡 TRUE:子卡  FALSE:母卡
	BYTE      	m_acMasterNum[CA_MAX_SMC_NUMBER_LEN+1];  ///< 母卡卡号
}CaSmcInfo_S;

#define MAX_OPERATOR_NUM         (4)       ///< 最多的运营商个数
#define MAX_OPERATORPPID_LEN     (6)       ///< 最长的运营商PPID
#define MAX_OPERATORLABLE_LEN    (8)       ///< 最长的运营商LABLE

/**@brief 运营商的基本信息结构体*/
typedef struct _CaOperatorInfo
{
	BYTE		m_acOperatorPPID[MAX_OPERATORPPID_LEN+1];     ///< 运营商PPID,显示方式%s*/
	BYTE		m_acOperatorLable[MAX_OPERATORLABLE_LEN+1];   ///< 运营商Lable,显示方式%s*/
	DWORD		m_dwBalance;                                  ///< 运营商余额
}CaOperatorInfo_S;

/**@brief 授权类型枚举*/
typedef enum
{
	E_PPID_FREE_ENTITLE, ///< FREE 授权,显示时只显示lable 和start/end时间即可，并且时间不需要显示小时和分钟
	E_PPID_PPC_ENTITLE,	 ///< PPC  授权,结构中的信息均需要显示*/
	E_PPID_PPV_ENTITLE	 ///< PPV  授权,结构中的信息均需要显示*/
} CaPPIDEntitleType_E;

#define  MAX_LABEL_LEN   (8)

/**@brief 授权详细信息*/
typedef struct 
{
	WORD 		m_wLinkID;	                ///< Packet ID
	BYTE 		m_acLabel[MAX_LABEL_LEN+1]; ///< Packet name
	BYTE 		m_ucPayState;               ///< 0:Already paid	 1,2:Paid by time  3:Paid by prog  4:Have not paid
	CATime_S  	m_sNDate;					///< Send time
	CATime_S 	m_sSDate;					///< Start time
	CATime_S  	m_sEDate;					///< End time
}CaEntitlementInfo_S;

#define  MAX_DATA_LEN    (5)

/**@brief OTA信息结构体*/
typedef struct  _CaOtaInfo
{
	WORD 		m_wOtalen;			        ///< OTA数据的总长度
	DWORD 		m_dwOtaMesgid;	            ///< OTA消息的MessageID
	BYTE		m_acOtadate[MAX_DATA_LEN];	///< OTA数据的发送时间
	WORD		m_wOtapid;			        ///< OTA数据的PID
	BYTE		m_ucDvbrecievetype;         ///< 接收机的类型IRD Type :DVB-C   DVB-S DVB-T
	BYTE		m_ucDvbmanufactor;	        ///< 厂商代码*/
	DWORD		m_dwDvbhardvesion;	        ///< 硬件版本号
	DWORD		m_dwDvbsoftversion;	        ///< 软件版本号
	BYTE		m_ucDvbModulation;	        ///< 调制方式
	DWORD		m_dwDvbfreq;		        ///< frequencyOTA数据所在的发送频点的频率
	DWORD		m_dwDvbsymb;		        ///< symbolrateOTA数据所在的发送频点的符号率
	BYTE		m_ucDvbpolarization;	    ///< 极化方式
	BYTE		m_ucDvbFEC;		            ///< DVB-C和DVB-S中使用的前向纠错编码
	BYTE		m_ucDvbGuardinterval;       ///< 保护间隔
	BYTE		m_ucDvbTransmission_mode;   ///< 传输模式
	BYTE		m_ucDvbcoderate;	        ///< 优先级码率
	BYTE		m_ucDvbBandWidth;	        ///< 带宽
	BYTE		m_ucDvbConstellation;       ///< 卫星所在的经度
}CaOtaInfo_S;

#define MAX_IPPV_LEN   (24)

/**@brief PPV信息结构体
@note  
- 当m_ucIPPVType = 1时，说明ppv是按时计费的，购买方式为 "按时间计费"m_dwIPPVTimePrice有效，m_dwIPPVViewPrice无效
- 当m_ucIPPVType = 2时，说明ppv是按次数付费，购买方式为 "按次数付费"m_dwIPPVTimePrice无效，m_dwIPPVViewPrice有效
- 当m_ucIPPVType = 0时，要提供2种方式共用户选择，用户选择那种就按那种方式确认m_dwIPPVTimePrice，m_dwIPPVViewPrice都有效
*/
typedef struct  _CAIPPVProInfo_S
{
	BYTE	  m_ucIPPVType;                ///< 1:Pay per minute  2:Pay per View  0:Both types can be chosen by users
	DWORD	  m_dwIPPVTimePrice;           ///< price of pay per minute --long int---
	DWORD	  m_dwIPPVViewPrice;           ///< price of pay per view ---long int---
	BYTE	  m_ucIPPVName[MAX_IPPV_LEN+1];///< Program instrction
	CATime_S  m_sIPPVStartTime;            ///< Start time
	CATime_S  m_sIPPVEndTime;              ///< Finish time
	WORD      m_wIPPVID;                   ///< ppvID  当 m_wIPPVID=0xffff  时表明当前的ppv不存在
}CAIPPVProInfo_S;

/**@brief 消费历史*/
typedef struct _IPPVHistoryInfo_S
{
	CATime_S  m_sIPPVEntitletime;    ///< 消费或充值时间
	LONG	  m_dwIPPVEntitletokens; ///< 消费或充值数量
}IPPVHistoryInfo_S;

#define MAX_UPDATEDATA_LEN  (5)

/**@brief CA的返回值*/
#define CA_RTN_FAIL                 		(0xff)  ///< 操作失败
#define	CA_RTN_OK                   		(0x00)  ///< 操作成功
#define	CA_RTN_PIN_ERROR           			(0x01)  ///< PIN码错误
#define	CA_RTN_PIN_LOCK						(0x02)	///< PIN码锁定
#define	CA_RTN_PIN_NO_MATCH					(0x03)  ///< 新PIN码和确认码不一致
#define	CA_RTN_MR_ERROR						(0x04)  ///< 成人级别节目错误
#define	CA_RTN_WT_ERROR						(0x05)  ///< 卡读写错误
#define	CA_RTN_IPPV_ERROR					(0x06)  ///< IPPV购买出错
#define	CA_RTN_NO_ENOUGH_TOKENS				(0x07)  ///< 金额不足
#define	CA_RTN_CARDOUT_ERROR				(0x08)  ///< 无卡或错误的智能卡
#define	CA_RTN_TYPE_ERROR					(0x09)  ///< 智能卡类型错误
#define	CA_RTN_OTHER_ERROR					(0x0A)  ///< 其它错误

/**
@brief 获取智能卡信息

@param[out] *pstSmcInfo获取的卡信息
@return  CA_RTN_OK    成功 
@		 CA_RTN_FAIL  失败
*/
INT CSCAGetSmcInfo(CaSmcInfo_S * pstSmcInfo);

/**
@brief 获取卡更新信息

@param[in] ucBuffLen aucInfo的长度(大于等于MAX_UPDATEDATA_LEN+1)
@param[out]aucInfo MAX_UPDATEDATA_LEN个字节的长度信息
@return 成功:CA_RTN_OK  失败:CA_RTN_FAIL
*/
INT CSCAGetSmcUpdataInfo(BYTE* aucInfo, BYTE ucBuffLen);

/**
@brief 修改智能卡密码

@param[in] pucOldPin      旧密码
@param[in] ucOldPinLen    旧密码长度
@param[in] pucNewPin      新密码
@param[in] ucNewPinLen    新密码长度
@param[in] pucMatchPin    确认密码
@param[in] ucMatchPinLen  确认密码长度
@param[out]pucRetryTimes  还可以重试的次数
@return  CA_RTN_OK   	      修改密码成功
@	     CA_RTN_PIN_ERROR     密码错误
@	     CA_RTN_PIN_LOCK	  PIN码已被锁定
@	     CA_RTN_PIN_NO_MATCH  新密码不匹配
@	     CA_RTN_OUT_ERROR	  无卡或错误智能卡	
@	     CA_RTN_OTHER_ERROR   其它错误
@note 用户输入1234，那么pucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4,  ucPinLen=4
*/
INT CSCAChangePin(BYTE *pucOldPin, 
                  BYTE ucOldPinLen,
                  BYTE *pucNewPin, 
                  BYTE ucNewPinLen,
				  BYTE *pucMatchPin,
				  BYTE ucMatchPinLen, 
				  BYTE *pucRetryTimes);

/**
@brief 设置成人级别(6--21有效)

@param[in] pucPinCode      密码
@param[in] ucPinLen        密码长度
@param[in] ucRating        成人级别
@param[out]pucRetryTimes   还可以重试的次数
@return  CA_RTN_OK   	    设置成功
@	     CA_RTN_ERROR       密码错误
@	     CA_RTN_PIN_LOCK    PIN码已被锁定
@	     CA_RTN_OUT_ERROR   无卡或错误智能卡	
@	     CA_RTN_OTHER_ERROR 其它错误
@note 用户输入1234，那么pucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4, ucPinLen=4
*/
INT CSCASetRating(BYTE *pucPinCode, 
                  BYTE ucPinLen,
                  BYTE ucRating, 
                  BYTE *pucRetryTimes);

/**
@brief 设置工作时段(00:00-23:59)

@param[in] pucPinCode      密码
@param[in] ucPinLen        密码长度
@param[in] ucStartTime_h   开始时间小时
@param[in] ucStartTime_m   开始时间分钟
@param[in] ucEndTime_h     结束时间小时
@param[in] ucEndTime_m     结束时间分钟
@param[out]pucRetryTimes   还可以重试的次数
@return  CA_RTN_OK   	    设置成功
@	     CA_RTN_ERROR       密码错误
@	     CA_RTN_PIN_LOCK    PIN码已被锁定
@	     CA_RTN_OUT_ERROR   无卡或错误智能卡	
@	     CA_RTN_OTHER_ERROR 其它错误
@note 用户输入1234，那么pucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4, ucPinLen=4
*/
INT CSCASetWorkTime(BYTE *pucPinCode, 
                    BYTE ucPinLen,
                    BYTE ucStartTime_h, 
                    BYTE ucStartTime_m, 
                    BYTE ucEndTime_h, 
                    BYTE ucEndTime_m, 
                    BYTE *pucRetryTimes);

/**
@brief 获取运营商总数

@param[out] *ucNumofOperator  总数
@return  CA_RTN_OK   	      成功
@	     CA_RTN_FAIL          失败
*/
INT CSCAGetOperatorTotal(WORD * ucNumofOperator);

/**
@brief  获取某个运营商信息

@param[in]  ucPPIDNO          运营商索引,从0开始
@param[out] *pstOperatorInfo  运营商信息
@return  CA_RTN_OK   	      成功
@	     CA_RTN_FAIL          失败
*/
INT CSCAGetOperatorInfo(BYTE ucPPIDNO, CaOperatorInfo_S* pstOperatorInfo);

/**
@brief  获取运营商某类授权信息的总数

@param[in]  ucPPIDNO          运营商索引,从0开始
@param[in]  ePPIDEntitleType  授权信息类型:free or ppc or ppv
@param[out] *wNum             获取到的授权信息总数      
@return  CA_RTN_OK   	      成功
@	     CA_RTN_FAIL          失败
*/
INT CSCAGetServiceEntitlesTotal(BYTE ucPPIDNO, 
                                CaPPIDEntitleType_E ePPIDEntitleType, 
                                WORD* wNum);

/**
@brief  获取某个运营商某个授权的详细信息

@param[in]  ucPPIDNO             运营商索引,从0开始
@param[in]  wEntitleINO          需要获取第几个授权信息,从0开始
@param[in]  ePPIDEntitleType     授权信息类型:free or ppc or ppv
@param[out] *pstServiceEntitles  授权详细信息    
@return  CA_RTN_OK   	      成功
@	     CA_RTN_FAIL          失败
*/
INT CSCAGetServiceEntitles(BYTE ucPPIDNO, 
                           WORD wEntitleINO, 
                           CaPPIDEntitleType_E ePPIDEntitleType, 
                           CaEntitlementInfo_S* pstServiceEntitles);

/**
@brief  获取某个运营商消费或充值历史总数

@param[in]  ucPPIDNO       运营商索引,从0开始
@param[out] *wNumofRecord  或充值历史总数 
@return  CA_RTN_OK   	   成功
@	     CA_RTN_FAIL       失败
*/
INT CSCAGetHistRecordTotal(BYTE ucPPIDNO, WORD *wNumofRecord);

/**
@brief  获取某个运营商某个消费或充值历史的详细信息
     
@param[in]  ucPPIDNO       运营商索引,从0开始
@param[in]  wRecordNO      历史记录编号(0开始)
@param[out] *psIPPVHistoryInfo  消费历史或充值详细信息
@return  CA_RTN_OK   	      成功
@	     CA_RTN_FAIL          失败
*/
INT CSCAGetHistRecordInfo(BYTE ucPPIDNO, 
                          WORD wRecordNO, 
                          IPPVHistoryInfo_S *psIPPVHistoryInfo);

/**
@brief  当界面弹出购买框，用户确认要购买IPPV时调用此函数
     
@param[in]  pucPin       密码
@param[in]  ucPinLen     密码长度
@param[in]  ucPayType    1:按时间付费，2:按次数付费
@param[in]  ucIndex      0:当前节目，1:下一个节目
@param[out] *ucRetryNum  密码还可以重试几次，最大5次
@return  CA_RTN_OK   	      成功
@	     CA_RTN_FAIL          失败
@note 用户输入1234，那么pucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4, ucPinLen=4
*/
INT CSCAConfirmBuyIPPV(BYTE* pucPin, 
                       BYTE ucPinLen,
                       BYTE ucPayType, 
                       BYTE ucIndex, 
                       BYTE* ucRetryNum);

/**
@brief  当界面弹出成人级别节目时，用户确认要观看此节目时调用此函数
     
@param[in]  pucPin       密码
@param[in]  ucPinLen     密码长度
@param[out] *ucRetryNum  密码还可以重试几次，最大5次
@return  CA_RTN_OK   	       成功
@        CA_RTN_CARDOUT_ERROR  无卡或智能卡错误
@        CA_RTN_PIN_ERROR      密码错误
@        CA_RTN_OTHER_ERROR    购买ippv出错
@        CA_RTN_PIN_LOCK       智能卡锁定
@note 用户输入1234，那么pucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4, ucPinLen=4
*/
INT CSCAConfirmWatchMrProgram(BYTE* ucPin, BYTE ucPinLen, BYTE* ucRetryNum);

/**
@brief  当界面弹出IPPV/成人级别节目提示时，用户购买失败或取消购买时调用
     
@param[in]  ucType      TRUE 取消购买或不输入密码，以后不再弹出IPPV、成人级别提示   
@						FALSE需要购买，但是购买失败，以后还会有IPPV、成人级别提示
@return  CA_RTN_OK   	成功
@	     CA_RTN_FAIL    失败
*/
INT CSCACancelWatchProgram(BOOL ucType);

/**
@brief  获取邮件的总数
     
@param[out]  *ucNum   邮件总数
@return  CA_RTN_OK    成功
@	     CA_RTN_FAIL  失败
*/
INT CSCAGetMailTotalNum(BYTE *ucNum);

/**
@brief  获取未读邮件的总数
     
@param[out]  *ucNum   未读邮件总数
@return  CA_RTN_OK    成功
@	     CA_RTN_FAIL  失败
*/
INT CSCAGetNewMailTotalNum(BYTE *ucNum);

/**
@brief  获取一封邮件的头信息
     
@param[in]  ucIndex      邮件索引;0开始
@param[out]  *pMailHead   对应索引下的邮件头信息
@return  CA_RTN_OK    成功
@	     CA_RTN_FAIL  失败
*/
INT CSCAGetMailHead(BYTE ucIndex, CaMailHeadInfo_S *pMailHead);

/**
@brief  获取一封邮件的内容
     
@param[in]  ucIndex           邮件索引;0开始
@param[out]  *psMailContent   对应索引下的邮件内容
@return  CA_RTN_OK    成功
@	     CA_RTN_FAIL  失败
*/
INT CSCAGetMailContent(BYTE ucIndex, CaMailContentInfo_S *psMailContent);

/**
@brief  删除一封邮件
     
@param[in]  ucIndex   邮件索引;0开始
@return  CA_RTN_OK    成功
@	     CA_RTN_FAIL  失败
*/
INT CSCADelOneMail(BYTE ucIndex);

/**
@brief  删除所有邮件
     
@return  CA_RTN_OK    成功
@	     CA_RTN_FAIL  失败
*/
INT CSCADelAllMail(void);

/**
@brief  设置pmtpid

@param[in]  wDemuxId    demuxid
@param[in]  wOriNetId   原始网络id
@param[in]  wTsID       ts流id
@param[in]  wServiceID  节目id
@param[in]  wPmtPid     pmtpid 
@return  CA_RTN_OK    成功
@	     CA_RTN_FAIL  失败
*/
void CSCASetSerivcePmtPid(WORD wDemuxId, 
                          WORD wOriNetId, 
                          WORD wTsID, 
                          WORD wServiceID, 
                          WORD wPmtPid);

/**
@brief  获取ABV CA  数据区的Flash 地址(备份区与主写区一定不能在同一个Block)

@param[in]  dwSize   分配flash的大小
@return     一个连续Block (64K) 的起始地址
*/
extern DWORD CSCAGetFlashAddress(DWORD dwSize);

/**
@brief  获取ABV CA  备份数据区的Flash 地址(备份区与主写区一定不能在同一个Block)

@param[in]  dwSize  分配flash的大小
@return     一个连续Block (64K) 的起始地址
*/
extern DWORD CSCAGetBackupFlashAddress(DWORD dwSize);

/**
@brief 从应用获得CA库需要的信息

@param[out]  *ucManucode    STB厂商代码(abv会给我们分配这个代码)
@param[out]  *pdwHardVer    硬件版本
@param[out]  *pdwSoftVer    软件版本
*/
extern void CSCAGetSTBInfoFromApp(BYTE *ucManucode, DWORD *pdwHardVer, DWORD *pdwSoftVer);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif //_CS_ABVCA_INTERFACE_H_

