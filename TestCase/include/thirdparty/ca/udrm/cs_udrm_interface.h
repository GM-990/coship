/******************************************************************************
 *文件名称：cs_udrm_interface.h
 *版权所有(c) 2010， 深圳市同洲电子股份有限公司。
 *
 *当前版本：Ver 1.0    
 *作    者：万算林 (wansuanlin@coship.com)
 *
 *内容摘要：
 *	文件功能说明
 *
 *	修改记录：
 *	版本号		日期		作者			修改内容
 *	--------	----------	----------		------------
 *	1.0			2010-06-24	万算林			创建
*******************************************************************************/

/******************************************************************************
 *                                 头文件引用                                 *
 ******************************************************************************/
#include "udiplus_typedef.h"
#include "cs_cainterface.h"

#ifndef _CS_UDRM_INTERFACE_H_
#define _CS_UDRM_INTERFACE_H_
#ifdef  __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 宏、定义、结构体                           *
 ******************************************************************************/

#define	CS_UDRM_MSG_BASE                (0)
#define	CS_UDRM_ALARM_MSG_BASE          (100)
#define CS_UDRM_INFO_MSG_BASE           (200)
#define	CS_UDRM_COMMAND_MSG_BASE        (300)

typedef enum _CaEventGroup
{
	CA_ALARM = CS_UDRM_MSG_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	
	CA_EVENTGROUP_MAX
}CaEventGroup_E;

typedef enum _CaAlarm
{
	CA_ALARM_SERVFREE = CS_UDRM_ALARM_MSG_BASE, /*清流节目*/
	CA_ALARM_SECURE_CHIPSET_ERROR,              /*UTI安全芯片错误*/
	CA_ALARM_SECURE_CHIPSET_INVALID,            /*UTI安全芯片无效*/
	CA_ALARM_NO_OPERTOR,                        /*终端没有此运营商*/
	CA_ALARM_NVRAM_ERROR,                       /*NVRAM错误*/
	CA_ALARM_NO_SIGNAL,                         /*没有信号*/
	
	CA_ALARM_PROGRAM_ABNORMITY,                 /*6 节目异常，请联系运营商*/
	CA_ALARM_NOENTITLE,                         /*节目未授权*/
	CA_ALARM_CANCEL,                            /*用于取消当前消息显示*/
	CA_ALARM_PROGRAM_AREA_LIMIT,                /*节目区域限制播出*/
	CA_ALARM_WATCHLEVEL,                        /*节目成人级别限制*/

	CA_ALARM_SHOW_FINGER,                       /*11 指纹显示*/
	CA_ALARM_ORDER_LIMIT,                       /*购买信用受限*/
	CA_ALARM_PROGRAM_PREVIEW,                   /*节目预览*/
	CA_ALARM_PRODUCT_ORDERED,                   /*产品已经购买*/
	CA_ALARM_AUTHOR_NUMBER_MAX,                 /*授权记录已达到最大数目*/

	CA_ALARM_NOT_ACCOUNT,                       /*16 未开户*/
	CA_ALARM_BLACKOUT,                          /*条件禁播*/
	CA_ALARM_PWD_RESET,                         /*密码已重置*/
	CA_ALARM_LINK_SERVER_FAIL,                  /*连接服务器失败*/
	CA_ALARM_PROGRAM_PROTECT_BYDRM,             /*本节目受DRM保护*/
	
	CA_ALARM_SECURE_CHIPSET_LOCK,               /*21 UTI安全芯片锁定*/
	CA_ALARM_SECURE_CHIPSET_UNLOCK,             /*UTI安全芯片解除锁定*/
	CA_ALARM_SERVOTHER,							/*其他CA加扰节目*/
	
    CA_ALARM_MAX
}CaAlarm_E;

typedef enum _CaCommand
{
	CA_COMMAND_EMAIL = CS_UDRM_COMMAND_MSG_BASE,/*邮件状态更新, dwParm2 = (DWORD)&CaMail_S*/
	CA_COMMAND_SHOW_FINGERPRINT,                /*显示指纹, dwParm2 = (DWORD)&CaFingerPrintInfo_S */
	CA_COMMAND_HIDE_FINGERPRINT,                /*隐藏指纹*/
	CA_COMMAND_SHOW_OSD,                        /*显示OSD信息, dwParm2 = (DWORD)&CaOsdInfo_S*/
	CA_COMMAND_HIDE_OSD,                        /*隐藏OSD, dwParm2 = (WORD)CaOsdStyle_E*/
	CA_COMMAND_IPPV_NOTIFY,                     /*IPPV通知, dwParam2 = (DWORD)&CaIPPVTInfo_S*/
	CA_COMMAND_PPT_NOTIFY,                      /*PPT通知, dwParam2 = (DWORD)&CaIPPVTInfo_S*/
	CA_COMMAND_IPPV_CANCEL,                     /*清除IPPV对话框*/
	CA_COMMAND_PPT_CANCEL,                      /*清除PPT对话框*/
	CA_COMMAND_OTA_INFO,                        /*OTA升级信息, dwParam2 = (DWORD)&CaOtaInfo_S*/

	CA_COMMAND_MAX
}CaCommand_E;

#define MAX_EMAIL_TITLE_LEN	     (20)   /*邮件标题的长度*/
#define MAX_EMAIL_NUM            (100)  /*机顶盒保存的最大邮件个数*/
#define MAX_EMAIL_CONTENT_LEN    (255)  /*邮件内容的长度*/
#define MAX_EVENT_INFO_LEN       (64)   /*Event最大信息长度*/
#define MAX_UTDRM_MAJORVER_LEN   (8)    /*UT主版本长度*/
#define MAX_UTDRM_SUBVER_LEN     (8)    /*UT子版本长度*/
#define MAX_CAPORTING_VER_LEN    (32)   /*CA Porting模块版本长度*/
#define MAX_UTUSER_SERIALNUM_LEN (32)   /*UT用户序列号长度*/
#define PIN_CODE_LEN             (6)    /*PIN码长度*/
#define MAX_FINGERPRINT_LEN      (20)   /*指纹信息*/

typedef struct _CaInfo
{
	CHAR	m_acUTMajorVer[MAX_UTDRM_MAJORVER_LEN];        /*UT主版本号*/
	CHAR    m_acUTSubVer[MAX_UTDRM_SUBVER_LEN];            /*UT子版本号*/
	CHAR	m_acCAVersion[MAX_CAPORTING_VER_LEN];          /*CA Porting模块版本号*/
	CHAR    m_acUTUserSerialNum[MAX_UTUSER_SERIALNUM_LEN]; /*UT用户序列号*/
}CaInfo_S;

typedef enum
{
	EM_EMAIL_HIDE_ICON,                 /*隐藏邮件通知图标*/
	EM_EMAIL_NEW_ICON,                  /*新邮件通知，显示新邮件图标*/
	EM_EMAIL_FULL_ICON                  /*磁盘空间以满，图标闪烁*/	
}CaEmailIconType_EM;

typedef struct  _CaDate
{
	DWORD   m_dwYear;
	DWORD   m_dwMonth;
	DWORD   m_dwDay;
	DWORD   m_dwHour;
	DWORD   m_dwMinute;
	DWORD   m_dwSecond;
}CaDate_S;

typedef struct _CaMail
{
	DWORD    m_dwEmailID;                              /*Email ID*/
	BOOL     m_bReaded;                                /*是否已读标志:FALSE->表示未读 TRUE->表示已读*/
	CHAR     m_acEmailHead[MAX_EMAIL_TITLE_LEN+1];     /*邮件标题，最长为20*/
	WORD     m_wImportance;                            /*0：强制显示，其它：普通*/
	CaDate_S m_stCreateTime;                           /*EMAIL创建的时间*/
	CHAR    m_acEmailContent[MAX_EMAIL_CONTENT_LEN+1];  /*Email的正文*/
}CaMail_S;/*共占312个字节*/

typedef enum _CaOsdStyle
{
	EM_CA_OSD_TOP,					/*在屏幕上方显示*/
	EM_CA_OSD_BOTTOM,               /*在屏幕下方显示*/
	EM_CA_OSD_TOPBOTTOM             /*在屏幕上、下方同时显示*/
}CaOsdStyle_EM;

typedef struct _CaOsdInfo
{
	CaOsdStyle_EM m_emOsdStyle;                        /*OSD显示方式*/
	WORD     m_wImportance;                            /*重要性, 0：普通，1：重要 */
	WORD     m_wDurtion;                               /*要求显示的时间长度，单位：秒*/
	CaDate_S m_stCreateTime;                           /*OSD创建的时间*/
	CHAR     m_acOsdContent[MAX_EMAIL_CONTENT_LEN+1];  /*OSD内容*/
}CaOsdInfo_S;

typedef enum _IPPVTType
{
	EM_PPVTTYPE_PPV,
	EM_PPVTTYPE_IPPV,
	EM_PPVTTYPE_PPT
}IPPVTType_E;

typedef enum _CaReturnCode
{
	EM_RETURNCODE_FAIL = -1,
	EM_RETURNCODE_SUCCESS,
	EM_RETURNCODE_INVALID_PARAM,
	EM_RETURNCODE_NVRAM_FAIL,
	EM_RETURNCODE_PIN_WRONG,
	EM_CAINIT_NOTCOMPLETE
}CaReturnCode_E;

/***************************************************************
 *关于CaIPPVTInfo_S结构体：
 *1.当应用主动调用CaPorting接口获取PPV/IPPV/PPT时，该结构体中的
 *  m_wDmxId和m_wServiceId字段无效;对于这种操作中, 
 *  1).如果是PPV类型,则m_aucEventInfo字段有效，其为Event name,其它两种类型时，此字段无效;
 *  2).如果是PPV类型,则m_dwPrice字段无效,其它两种类型有效;
 *  3).m_dwPayUnit字段只针对PPT有效;
 *2.当CaPorting主动上报CA_COMMAND_IPPV_NOTIFY和CA_COMMAND_PPT_NOTIFY
 *  时，m_wDmxId和m_wServiceId字段有效;
 */
typedef struct _IPPVTInfo
{
	BYTE     m_ucIppvType;  /*0:PPV, 1:IPPV, 2:PPT*/
	DWORD    m_dwOperatorIndex;
	DWORD    m_dwSectorIndex;
	WORD     m_wDmxId;
	WORD     m_wServiceId;
	DWORD    m_dwEventId;
	BYTE     m_aucEventInfo[MAX_EVENT_INFO_LEN];
	DWORD    m_dwPrice;
	DWORD    m_dwPayUnit;   /*该字段只有是PPT节目才有效*/
	CaDate_S m_stStartTime;
	CaDate_S m_stEndTime;
}CaIPPVTInfo_S;

typedef struct _ProductInfo
{
	DWORD    m_dwOperatorId;
	DWORD    m_dwSectorId;
	DWORD    m_dwProductId;
}CaProductInfo_S;

typedef struct _WalletInfo
{
	DWORD    m_dwOperatorId;
	DWORD    m_dwSectorId;
	DWORD    m_dwValue;
}CaWalletInfo_S;

typedef struct _FingerPrint
{
	CHAR     m_acFingerPrint[MAX_FINGERPRINT_LEN];
}CaFingerPrintInfo_S;

typedef struct _OtaInfo
{
	DWORD     m_dwManufactureId;              //厂商ID，厂商指定
	DWORD     m_dwModuleId;                   // 设备ID，厂商指定
	DWORD     m_dwLoaderVersion;              // loader版本，厂商指定
	DWORD     m_dwSoftwareVersion;            // 软件版本
	DWORD     m_dwServiceId;                  // 业务号
	DWORD     m_dwOperatorIdMatchValid;      // 是否匹配运营商的标志 0 - 不用匹配 1- 需要匹配
	DWORD     m_dwOperatorId;                 // 运营商ID
	DWORD     m_dwAreaIdMatchValid;          // 是否匹配区域号的标志 0 - 不用匹配 1 -需要匹配
	DWORD     m_dwAreaId;                    // 区域号
	DWORD     m_dwSoftwareVersionMatchValid; // 是否需要匹配特定软件版本号的标志 0 - 不用匹配 1 - 需要匹配
	DWORD     m_dwCertainSoftwareVersion;     // 特定软件版本号
	DWORD     m_dwGlobalTriggerValid;         //是否是全局触发
	DWORD     m_dwGlobalTriggerType;          //全局触发类型, 0：为强制升级，1：非强制升级
	DWORD     m_dwEMMTriggerType; 		      //私有触发类型，0：为强制升级，1：非强制升级
}CaOtaInfo_S;

/******************************************************************************
 *                                 函数声明                                   *
 *说明：
 *	所有应用上要获取信息所需的要空间都由应用分配，比如，获取电子钱包信息，
 *		CaWalletInfo_S这个结构空间由应用分配
 ******************************************************************************/

/******************************************************************************
函数名称:	CSCAGetSNFromTestMode

功能描述:
		测试模式下获取正式SN号，用于生产时生成报文文件

输入参数:
		pacSN : 应用分配一个不小于32个字节的Buffer，用于存储SN号
		pnLen : 应用告之分配的Buffer大小值

输出参数:
		pacSN : 返回SN号，字符串格式
		pnLen : 返回真实的字符串长度

返 回 值:
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误

其他说明:
*******************************************************************************/
INT CSCAGetSNFromTestMode(CHAR *pacSN, INT *pnLen);

/******************************************************************************
函数名称:	CSCAGetUDRMAreaValue

功能描述:
		获取区域值

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1
		pdwAreaValue    : 声明一个DWORD类型的Buffer，用于存储区域值

输出参数:
		pdwAreaValue : 返回区域值

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetUDRMAreaValue(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwAreaValue);

/******************************************************************************
函数名称:	CSCAAppInfoSet

功能描述:
		CA初始化完成之后，调用此函数将厂商代号，设置ID，Loader版本传给CA内核

输入参数:
		wManufactureId : 厂商代号
		wModuleId      : 设备ID
		wLoaderVersion : Loader版本

输出参数:
		无

返 回 值:
		EM_RETURNCODE_FAIL          : 设置失败
		EM_RETURNCODE_SUCCESS       : 设置成功
		
其他说明:
*******************************************************************************/
INT CSCAAppInfoSet(DWORD dwManufactureId, DWORD dwModuleId, DWORD dwLoaderVersion);

/******************************************************************************
函数名称:	CSCALoaderStatus

功能描述:
		此函数用于应用可以实时查看是否有升级信息(备注：开机到检测到用升级信息，
		CA只会上报一次CA_COMMAND_OTA_INFO消息)

输入参数:
		pstLoaderStatus : 应用分配空间，用于存储当前的升级信息

输出参数:
		pstLoaderStatus : 返回各值

返 回 值:
		EM_RETURNCODE_FAIL          : 当前没有升级信息
		EM_RETURNCODE_SUCCESS       : 当前有正确的升级信息
		EM_RETURNCODE_INVALID_PARAM : 当前码流中有升级信息，但CA内核没有过滤到升级信息
		
其他说明:
		查看此状态时，要按照上述返回值给出提示信息框
*******************************************************************************/
INT CSCALoaderStatus(CaOtaInfo_S *pstLoaderStatus);

/******************************************************************************
函数名称:	CSCAGetMailNum

功能描述:
		获取邮件总数和剩余邮件空间

输入参数:
		无

输出参数:
		pnTotalEmailNum  : 邮件总数
		pnNoReadEmailNum : 未读邮件个数

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败
		
其他说明:
*******************************************************************************/
INT CSCAGetMailNum(INT* pnTotalEmailNum, INT* pnNoReadEmailNum);

/******************************************************************************
函数名称:	CSCASetMailNum

功能描述:
		设置需要存储的邮件最大个数，最大的个数为209封

输入参数:
		pnTotalEmailNum	：需要存储的邮件个数

输出参数:
 		pnTotalEmailNum	：实际设到CA中的正确邮件个数


返 回 值:
		SUCCESS:           成   功
		FAILURE:           传入函数的是空指针
		
其他说明:
*******************************************************************************/
INT CSCASetMailNum(INT* pnTotalEmailNum);

/******************************************************************************
函数名称:	CSCAGetMailContentByIndex

功能描述:
		根据索引号获取邮件内容

输入参数:
		nIndex:邮件索引号(从0开始)

输出参数:
		pstMailContent:邮件内容，见CaEmailContent_S

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetMailByIndex(INT nIndex, CaMail_S *pstMailInfo);

/******************************************************************************
函数名称:	CSCAGetMailContentByEmailId

功能描述:
		根据EmailId获取邮件内容

输入参数:
		dwEmailId:邮件ID号

输出参数:
		pstMailInfo:邮件内容，见CaMail_S

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetMailByEmailId(DWORD dwEmailId, CaMail_S *pstMailInfo);

/******************************************************************************
函数名称:	CSCASetMailReadedFlag

功能描述:
		通过EmailId设置邮件已读标志

输入参数:
		dwEmailId : 邮件标识号
		bReadFlag : 已读标志

输出参数:
		无

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCASetMailReadedFlag(DWORD dwEmailId, BOOL bReadFlag);

/******************************************************************************
函数名称:	CSCADelMail

功能描述:
		根据索引号删除一封邮件

输入参数:
		dwEmailId:邮件标识号

输出参数:
		无

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCADelMail(DWORD dwEmailId);

/******************************************************************************
函数名称:	CSCADelMailAll

功能描述:
		删除所有邮件

输入参数:
		无

输出参数:
		无

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCADelMailAll(void);

/******************************************************************************
函数名称:	CSCAGetCaInfo

功能描述:
		获取CA基本信息

输入参数:
		无

输出参数:
		pstCaInfo:CA基本信息，见CaInfo_S

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetCaInfo(CaInfo_S *pstCaInfo);

/******************************************************************************
函数名称:	CSCAGetOperatorNumber

功能描述:
		获取运营商个数

输入参数:
		无

输出参数:
		pdwOperatorNum:用于输出运营商个数

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetOperatorNumber(DWORD *pdwOperatorNum);

/******************************************************************************
函数名称:	CSCAGetSectorNumber

功能描述:
		获取每个运营商扇区个数(扇区相当于节目提供商)

输入参数:
		dwOperatorIndex:运营商标识号，从0开始，最大值为运营商个数减1

输出参数:
		pdwSectorNum:输出每个运营商扇区个数

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetSectorNumber(DWORD dwOperatorIndex, DWORD *pdwSectorNum);

/******************************************************************************
函数名称:	CSCAGetProductNumber

功能描述:
		获取每个扇区的产品个数

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1

输出参数:
		pdwProductNum:产品个数

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetProductNumber(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwProductNum);

/******************************************************************************
函数名称:	CSCAGetProductInfo

功能描述:
		获取每个产品的信息

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1
		dwProductIndex  : 产品标识号，从0开始，最大值为产品个数减1

输出参数:
		pstProductInfo:产品信息

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetProductInfo(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD dwProductIndex, CaProductInfo_S* pstProductInfo);

/******************************************************************************
函数名称:	CSCAGetIPPVTNumber

功能描述:
		1.当传入的emIppvtType为：EM_PPVTTYPE_PPV--获取每个扇区的PPV(Pay Pre View按次付费)个数
		2.当传入的emIppvtType为：EM_PPVTTYPE_IPPV--获取每个扇区的IPPV(请注意：Impulse Pay Pre View即兴按次付费)个数
		3.当传入的emIppvtType为：EM_PPVTTYPE_PPT--获取每个扇区的PPT(Pay Pre Time按时付费)个数

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1
		emIppvtType     : PPV/IPPV/PPT其中之一

输出参数:
		pdwIPPVTNum:输出对应的PPV或IPPV或PPT个数

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetIPPVTNumber(DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD *pdwIPPVTNum);

/******************************************************************************
函数名称:	CSCAGetIPPVTInfo

功能描述:
		1.当传入的emIppvtType为：EM_PPVTTYPE_PPV--获取每个PPV事件信息
		2.当传入的emIppvtType为：EM_PPVTTYPE_IPPV--获取每个IPPV事件信息
		3.当传入的emIppvtType为：EM_PPVTTYPE_PPT--获取每个PPT事件信息

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1
		emIppvtType     : PPV/IPPV/PPT其中之一
		dwIPPVTIndex    : PPV/IPPV/PPT标识号，从0开始，最大值为个数减1

输出参数:
		pstIPPVTInfo:输出PPV或IPPV或PPT信息

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetIPPVTInfo(DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD dwIPPVTIndex, CaIPPVTInfo_S* pstIPPVTInfo);

/******************************************************************************
函数名称:	CSCAOrderIPPVT

功能描述:
		1.当传入的emIppvtType为：EM_PPVTTYPE_IPPV--订购IPPV
		2.当传入的emIppvtType为：EM_PPVTTYPE_PPT--订购PPT

输入参数:
		bIsOrder        : TRUE表示购买， FALSE表示取消购买对话框
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1
		emIppvtType     : PPV/IPPV/PPT其中之一
		dwEventId       : IPPV事件号

输出参数:
		无

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAOrderIPPVT(BOOL bIsOrder, DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD dwEventId);

/******************************************************************************
函数名称:	CSCAGetViewLevel

功能描述:
		获取观看级别

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1

输出参数:
		pdwLevel:输出观看级别

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败

其他说明:
*******************************************************************************/
INT CSCAGetViewLevel(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwLevel);

/******************************************************************************
函数名称:	CSCASetViewLevel

功能描述:
		设置观看级别

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1
		dwLevel         : 希望设置的观看级别
		paucPin         : PIN码，是unsigned char类型数组，长度为6

输出参数:
		无

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败
		EM_RETURNCODE_PIN_WRONG     : PIN码错误

其他说明:
*******************************************************************************/
INT CSCASetViewLevel(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD dwLevel, BYTE *paucPin);

/******************************************************************************
函数名称:	CSCACheckPin

功能描述:
		校验PIN码

输入参数:
		paucPin         : PIN码，是unsigned char类型数组，长度为6

输出参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败
		EM_RETURNCODE_PIN_WRONG     : PIN码错误

其他说明:
*******************************************************************************/
INT CSCACheckPin(DWORD dwOperatorIndex, DWORD dwSectorIndex, BYTE *paucPin);

/******************************************************************************
函数名称:	CSCASetPin

功能描述:
		修改PIN码

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1
		paucOldPin      : 旧PIN码，是unsigned char类型数组，长度为6
		paucNewPin      : 需要更新的PIN码，是unsigned char类型数组，长度为6

输出参数:
		无

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败
		EM_RETURNCODE_PIN_WRONG     : PIN码错误

其他说明:
*******************************************************************************/
INT CSCASetPin(DWORD dwOperatorIndex, DWORD dwSectorIndex, BYTE *paucOldPin, BYTE *paucNewPin);

/******************************************************************************
函数名称:	CSCAGetWalletInfo

功能描述:
		获取电子钱包信息

输入参数:
		dwOperatorIndex : 运营商标识号，从0开始，最大值为运营商个数减1
		dwSectorIndex   : 扇区标识号，从0开始，最大值为扇区个数减1

输出参数:
		pstWalletInfo : 输出电子钱包信息

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误
		EM_RETURNCODE_NVRAM_FAIL    : NVRAM失败
		EM_RETURNCODE_PIN_WRONG     : PIN码错误

其他说明:
*******************************************************************************/
INT CSCAGetWalletInfo(DWORD dwOperatorIndex, DWORD dwSectorIndex, CaWalletInfo_S *pstWalletInfo);

/******************************************************************************
函数名称:	CSCAGetUDRMBuildInformation

功能描述:
		查询UDRM版本编译信息

输入参数:
		paucBuff : 存储版本编译信息的buf
		nBuffSize   : 存储版本编译信息的buf的大小，至少为128字节，如果小于128字节，函数将返回EM_RETURNCODE_INVALID_PARAM

输出参数:
		paucBuff : 版本编译信息

返 回 值:
		EM_RETURNCODE_FAIL          : 失败
		EM_RETURNCODE_SUCCESS       : 成功
		EM_RETURNCODE_INVALID_PARAM : 参数错误	

其他说明:
*******************************************************************************/
INT CSCAGetUDRMBuildInformation(BYTE *paucBuff, int nBuffSize);

#ifdef  __cplusplus
}
#endif

#endif //_CS_UDRM_INTERFACE_H_
