/******************************************************************************
文件名称：CSFINESUMCAINTERFACE.H
版权所有(c) 2010， 深圳市同洲电子股份有限公司。

当前版本：Ver 1.0    
作    者：胡远峰 (huyuanfeng@coship.com)

内容摘要：
		文件功能说明

修改记录：
版本号		日期		作者			修改内容
--------	----------	----------		------------
1.0			2010-5-11	胡远峰	创建
*******************************************************************************/
#include "cs_cainterface.h"
#include "cs_cainterface_pvrext.h"

#ifndef CSFINESUMCAINTERFACE_H
#define CSFINESUMCAINTERFACE_H
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 头文件引用                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 宏、定义、结构体                           *
 ******************************************************************************/
#define	CS_FSCA_MSG_BASE    			(0)
#define	CS_FSCA_ALARM_MSG_BASE    		(100)
#define	CS_FSCA_INFO_MSG_BASE    		(200)
#define	CS_FSCA_COMMAND_MSG_BASE    	(300)
#define	CS_FSCA_STATUS_MSG_BASE    		(400)
#define	CS_FSCA_MMI_MSG_BASE    		(500)

#define MAX_CAINFO_LEN   	   			(10)		/*CA厂家信息最大长度*/
#define MAX_CAVERSION_LEN	   			(10)	 	/*CA模块版本号长度*/
#define	MAX_OSD_LEN						(184)		/*OSD内容的最大长度*/
#define CARDID_LEN						(8)			/*卡号长度*/
#define ZONEID_LEN         				(4)			/*卡区域ID*/

#define MAX_EMAIL_TITLE_LEN	     		(20)		/*邮件标题的长度*/
#define MAX_EMAIL_NUM            		(100)		/*机顶盒保存的最大邮件个数*/
#define MAX_EMAIL_CONTENT_LEN    		(180)		/*邮件内容的长度*/

typedef enum _CaEventGroup
{
	CA_ALARM = CS_FSCA_MSG_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	
	CA_EVENTGROUP_MAX
}CaEventGroup_E;

typedef enum _CaAlarm
{
	CA_ALARM_CANCEL = CS_FSCA_ALARM_MSG_BASE,	/*取消当前消息显示*/
	CA_ALARM_FAIL, 								/*操作失败*/
	CA_ALARM_BADCARD, 							/*无法识别的卡，不能使用*/
	CA_ALARM_NOCARD, 							/*卡片没有插入*/
	CA_ALARM_BADVERSION, 						/*数据格式版本不支持*/
	CA_ALARM_BADCAS, 							/*不支持的CA系统*/
	
	CA_ALARM_BADSP, 							/* 6 不支持的网络运营商*/
	CA_ALARM_BADFILTER, 						/*错误的过滤规则*/
	CA_ALARM_SERVSTART, 						/*服务起用操作成功*/
	CA_ALARM_SERVPAUSE, 						/*服务冻结操作成功*/
	CA_ALARM_PAIRING, 							/*节目需要机卡对应*/
	
	CA_ALARM_PAUSED, 							/* 11 服务已经被冻结*/
	CA_ALARM_CARDDELETE, 						/*卡删除操作成功*/
	CA_ALARM_FILESYSBAD, 						/*文件系统出错*/
	CA_ALARM_LEVELERROR, 						/*不能收看的节目级别*/
	CA_ALARM_DECRYPTFAIL, 						/*解密出错*/
	
	CA_ALARM_DATAERROR, 						/* 16 错误的数据格式*/
	CA_ALARM_NEAREND, 							/*授权即将到期，请及时充值*/
	CA_ALARM_PAIRINGFAIL,						/*机卡配对失败*/
	CA_ALARM_NOENTITLE,							/*没有授权*/
	CA_ALARM_NEWEMAIL,							/*新邮件通知*/
	
	CA_ALARM_EMAILFULL,							/* 21 邮件已满*/
	CA_ALARM_NOMONEY,							/*电子钱包金额不足*/
	CA_ALARM_ISSONCARD,							/*当前卡片为子卡，无法获取喂卡数据*/
	CA_ALARM_NEEDECM,							/*未知的当前时间，卡片需要先获取ECM数据*/
	CA_ALARM_ISMOTHERCARD,						/*当前卡片为母卡，不需要获取喂卡数据*/
	
	CA_ALARM_BADDATA,							/* 26 错误的喂卡数据*/
	CA_ALARM_NOTINIT,							/*卡片未完成初始化*/
	CA_ALARM_NOOVERTIME,						/*当前卡片是母卡，无子卡到期时间*/
	CA_ALARM_SONCARDEXRIE,						/*子卡到期，需要插入母卡*/
	CA_ALARM_NITCHECKERROR,						/*NIT表区域验证失败*/
	
	CA_ALARM_NITZONEERROR,						/* 31 智能卡未通过区域验证，需要接收正确的NIT表*/
	CA_ALARM_HIDEINFOERROR,						/*获取节目隐藏信息错误*/
	CA_ALARM_UPDATEFILTER,						/*更新过滤规则*/
	CA_ALARM_GETMOTHERCARDDATA,					/*获取母卡数据成功*/
	CA_ALARM_SETSONCARDDATA,					/*子卡设置数据成功*/

	CA_ALARM_SERVFREE,							/* 36 免费节目*/
	CA_ALARM_SERVOTHER,							/*其他CA加扰节目*/
	
    CA_ALARM_MAX
}CaAlarm_E;

typedef enum _CaCommand
{
	//邮件状态更新
	CA_COMMAND_EMAIL = CS_FSCA_COMMAND_MSG_BASE,/*dwParm2 = (DWORD)CaEmailIconType_E*/
	//指纹
	CA_COMMAND_FINGERPRINT,						/*dwParm2 = (DWORD)CardId, if equal to zero,means cancel fingerprint */
	//显示OSD信息
	CA_COMMAND_SHOW_OSD,						/*dwParm2 = (DWORD)&CaOsdInfo_S*/
	//隐藏OSD信息
	CA_COMMAND_HIDE_OSD,						/*dwParm2 = (WORD)CaOsdStyle_E*/

	CA_COMMAND_MAX
}CaCommand_E;

typedef enum _CaStatus
{
	CA_STATUS_SMC_IN = CS_FSCA_STATUS_MSG_BASE,
	CA_STATUS_SMC_OUT,
	CA_STATUS_SMC_OK,
	CA_STATUS_SMC_ERROR,
	
	CA_STATUS_MAX
}CaStatus_E;

typedef enum _CaEmailIconType
{
	CA_EMAIL_HIDE_ICON,				/*隐藏邮件通知图标*/
	CA_EMAIL_NEW_ICON,				/*新邮件通知，显示新邮件图标*/
	CA_EMAIL_FULL_ICON				/*磁盘空间以满，图标闪烁。*/	
}CaEmailIconType_E;

typedef enum _CaOsdStyle
{
	CA_OSD_TOP,						/*在屏幕上方显示*/
	CA_OSD_BOTTOM					/*在屏幕下方显示*/
}CaOsdStyle_E;

typedef enum _CA_ReturnValue
{
	CA_RETURN_OK, 					/*操作成功*/
	CA_RETURN_FAIL, 				/*操作失败*/
	CA_RETURN_INVALIDCARD, 			/*IC卡无效或无IC卡插入*/
	CA_RETURN_CARDNOTPAIRING,		/* IC卡没有对应任何机顶盒*/
	CA_RETURN_CARDPAIRED,			/* IC卡已对应其他机顶盒*/
	CA_RETURN_VERSIONERROR,			/*不支持的内核版本*/
	CA_RETURN_POINTINVALID,			/*指针为空*/
	CA_RETURN_DATANOTFOUND,			/*数据没有找到*/
	CA_RETURN_UNKOWNCAID,			/*未知CAID*/
	CA_RETURN_ISSONCARD,			/*当前卡片为子卡，无法获取喂卡数据*/	
	CA_RETURN_NEEDECM,				/*未知的当前时间，卡片需要先获取ECM数据*/
	CA_RETURN_ISMOTHERCARD,			/*当前卡片为母卡，不需要获取喂卡数据*/
	CA_RETURN_BADDATA,				/*错误的喂卡数据*/
	CA_RETURN_INSERT_SONCARD,		/*请插入子卡*/
	CA_RETURN_NO_ZONE_CONTROL,		/*该卡片无区域控制*/
	CA_RETURN_HIDE_INFO_ERROR		/*获取节目隐藏信息错误*/
}CA_ReturnValue_E;

typedef struct _CaOsdInfo
{
	CaOsdStyle_E 		m_eStyle;
	CHAR				m_acMessage[MAX_OSD_LEN];
}CaOsdInfo_S;

typedef struct _CaInfo
{
	CHAR	m_acCAProvider[MAX_CAINFO_LEN +1];				/*CA提供商名称*/
	CHAR	m_acCAVersion[MAX_CAVERSION_LEN +1];			/*CA模块版本号*/
	BYTE	m_ucServiceProviderID;							/*运营商ID*/
	BYTE    m_ucCardControlLevel;                       	/*智能卡节目等级*/
	BYTE    m_aucCardId[CARDID_LEN];						/*智能卡卡号*/
	BYTE    m_aucZoneId[ZONEID_LEN];						/*智能卡区域ID*/
}CaInfo_S;

typedef struct  _CaDate
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
}CaDate_S;

typedef struct _CaEmailContent
{
	CHAR		m_acEmailContent[MAX_EMAIL_CONTENT_LEN+1];	/*Email的正文*/
}CaEmailContent_S;

typedef struct _CaEmailTitle                       
{
	DWORD		m_dwEmailID;                                /*Email ID  */
	BYTE		m_ucNewEmail;                               /*0 不是新邮件 1是新邮件 */
	CHAR   		m_acEmailHead[MAX_EMAIL_TITLE_LEN+1];  		/*邮件标题，最长为30 */
	WORD 		m_wImportance;								/*重要性, 0：普通，1：重要 */
	CaDate_S	m_stCreateTime;                             /*EMAIL创建的时间*/
}CaEmailHead_S;

typedef enum _CaQAMType
{
	CA_QAMTYPE_QPSK,
	CA_QAMTYPE_8QAM,
	CA_QAMTYPE_16QAM,
	CA_QAMTYPE_32QAM,
	CA_QAMTYPE_64QAM,
	CA_QAMTYPE_128QAM
}CaQAMType_E;

typedef struct _CaNITFrequenceInfo
{
	DWORD		m_dwFrequenceKHz;		//频点KHz
	DWORD		m_dwSymbolKBps;			//符号率KBps
	CaQAMType_E	m_dwQamType;			//调试方式
}CaNITFrequenceInfo_S;

/******************************************************************************
 *                                 函数声明                                   *
 ******************************************************************************/

/******************************************************************************
函数名称:	CSCAGetMailNum

功能描述:
		获取邮件总数和剩余邮件空间

输入参数:
		无

输出参数:
		pnEmailNum:邮件总数
		pnEmptyNum:剩余邮件空间

返 回 值:
		0 : SUCCESS
		-1: FAILURE
		
其他说明:
*******************************************************************************/
INT CSCAGetMailNum(INT* pnEmailNum, INT* pnEmptyNum);

/******************************************************************************
函数名称:	CSCAGetMailHead

功能描述:
		获取邮件标题信息

输入参数:
		nIndex:邮件索引号(从1开始)

输出参数:
		pstMailHead:邮件标题内容，见CaEmailHead_S

返 回 值:
		0 : SUCCESS
		-1: FAILURE
		
其他说明:
*******************************************************************************/
INT CSCAGetMailHead(INT nIndex, CaEmailHead_S *pstMailHead);

/******************************************************************************
函数名称:	CSCAGetMailContentByIndex

功能描述:
		根据索引号获取邮件内容

输入参数:
		nIndex:邮件索引号(从1开始)

输出参数:
		pstMailContent:邮件内容，见CaEmailContent_S

返 回 值:
		0 : SUCCESS
		-1: FAILURE

其他说明:
*******************************************************************************/
INT CSCAGetMailContentByIndex(INT nIndex, CaEmailContent_S *pstMailContent);

/******************************************************************************
函数名称:	CSCAGetMailContentByEmailId

功能描述:
		根据EmailId获取邮件内容

输入参数:
		wEmailId:邮件ID号

输出参数:
		pstMailContent:邮件内容，见CaEmailContent_S

返 回 值:
		0 : SUCCESS
		-1: FAILURE

其他说明:
*******************************************************************************/
INT CSCAGetMailContentByEmailId(WORD wEmailId, CaEmailContent_S *pstMailContent);

/******************************************************************************
函数名称:	CSCADelMail

功能描述:
		根据索引号删除一封邮件

输入参数:
		nIndex:邮件索引号(从1开始)

输出参数:
		无

返 回 值:
		0 : SUCCESS
		-1: FAILURE

其他说明:
*******************************************************************************/
INT CSCADelMail(INT nIndex);

/******************************************************************************
函数名称:	CSCADelMailAll

功能描述:
		删除所有邮件

输入参数:
		无

输出参数:
		无

返 回 值:
		0 : SUCCESS
		-1: FAILURE

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
		0 : SUCCESS
		-1: FAILURE

其他说明:
*******************************************************************************/
INT CSCAGetCaInfo(CaInfo_S *pstCaInfo);

/******************************************************************************
函数名称:	CSCAGetPairstatus

功能描述:
		获取机卡配对情况

输入参数:
		无

输出参数:
		无

返 回 值:
		CA_RETURN_OK			--机卡配对成功						
		CA_RETURN_CARDNOTPAIRING--IC卡没有对应任何机顶盒	
		CA_RETURN_CARDPAIRED	--IC卡已对应其他机顶盒		
		CA_RETURN_INVALIDCARD	--卡无效或卡未插入	
		
其他说明:
*******************************************************************************/
CA_ReturnValue_E CSCAGetPairstatus(void);

/******************************************************************************
函数名称:	CSCANotifyShowOSDOver

功能描述:
		通知CA停止显示OSD信息

输入参数:
		无

输出参数:
		无

返 回 值:
		无

其他说明:
		在接收到CA_COMMAND_SHOW_OSD消息，OSD完整滚动一周后
		调用这个接口来通知CA停止OSD显示
*******************************************************************************/
void CSCANotifyShowOSDOver(void);

/******************************************************************************
函数名称:	CSCAControlZone

功能描述:
		获取卡片区域限制情况

输入参数:
		当返回值为CA_RETURN_NO_ZONE_CONTROL时，参数无意义
		stNitFreInfo:返回需要接收的NIT表的频点信息	
输出参数:
		无

返 回 值:
		CA_RETURN_FAIL				--操作失败
		CA_RETURN_OK				--操作成功						
		CA_RETURN_INVALIDCARD		--IC无效卡片	
		CA_RETURN_NO_ZONE_CONTROL	--该卡片无区域控制	
		
其他说明:
*******************************************************************************/
CA_ReturnValue_E CSCAControlZone(CaNITFrequenceInfo_S* pstNitFreInfo);

/******************************************************************************
函数名称:	CSCAGetControlLevel

功能描述:
		获取卡片等级	

输入参数:
		ucLevel:返回卡片的节目隐藏等级		
输出参数:
		无

返 回 值:
		CA_RETURN_UNKOWNCAID		--未知CAID
		CA_RETURN_FAIL				--操作失败
		CA_RETURN_OK				--操作成功						
		CA_RETURN_INVALIDCARD		--IC无效卡片	
		FSCA_RETURN_HIDE_INFO_ERROR	--获取节目隐藏信息错误
		
其他说明:
*******************************************************************************/
CA_ReturnValue_E CSCAGetControlLevel(BYTE* ucLevel);

/******************************************************************************
函数名称:	CSCAGetCardID

功能描述:
		获取卡片ID	

输入参数:
		paucCardID:返回卡片的ID	(8 字节)		
输出参数:
		无

返 回 值:
		CA_RETURN_FAIL				--操作失败
		CA_RETURN_OK				--操作成功						
		CA_RETURN_INVALIDCARD		--IC无效卡片	
		
其他说明:
*******************************************************************************/
CA_ReturnValue_E CSCAGetCardID(BYTE* paucCardID);

/******************************************************************************
函数名称:	CSCAGetZoneID

功能描述:
		获取卡片区域ID	
		
输入参数:
		paucZoneID:返回卡片区域ID	(4字节)	
输出参数:
		无

返 回 值:
		CA_RETURN_FAIL				--操作失败
		CA_RETURN_OK				--操作成功						
		CA_RETURN_INVALIDCARD		--IC无效卡片	
		
其他说明:
*******************************************************************************/
CA_ReturnValue_E CSCAGetZoneID(BYTE* paucZoneID);

#ifdef __cplusplus
}
#endif
#endif  /* CSFINESUMCAINTERFACE_H */

/* 文件结束 */

