/******************************************************************************
文件名称：cs_sdp.h
版权所有(c) 2006， 深圳市同洲电子股份有限公司。

当前版本：Ver 1.0    
作    者：

内容摘要：
	sdp模块提供的外部接口文件。

修改记录：
版本号		日期		作者			修改内容
--------	----------	----------		------------
1.0			2006-2-27	刘正华	     重新修改接口重新排版
1.1         2006-6-22   刘正华       移植到PORTING层
*******************************************************************************/

#ifndef __CS_SDP_H__
#define __CS_SDP_H__
/******************************************************************************
*                                 头文件引用                                 *
******************************************************************************/
#include <stdio.h>		/* C标准库头文件*/

#ifdef PLATFORM_ST40
#include <time.h>
#else
#ifdef PLATFORM_ST20
#include "cs_clocktimer.h"
#else
#ifndef WIN32
#include <sys/time.h>
#endif
#endif
#endif

#include "mmcp_typedef.h"
#include "cs_clocktimer.h"
/******************************************************************************
*                                 宏、定义、结构体                           *
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#if defined(PLATFORM_ST40) || defined(PLATFORM_ST20)
typedef unsigned long  SDPUINT64;
typedef long  SDPINT64;
#else
typedef unsigned long long  SDPUINT64;
typedef long long  SDPINT64;
#endif


#define SDP_MAX_REPEAT_OFFSETS 16

#ifndef SDP_NTP_TO_UNIX_TIME
#define SDP_NTP_TO_UNIX_TIME 2208988800UL
#endif

typedef enum CSSDPLogLevel_E
{
	EM_SDP_LEVEL_INFO = 1,   /*一般*/
    EM_SDP_LEVEL_WARN,       /*警告*/
    EM_SDP_LEVEL_ERROR      /*错误*/
} CSSDPLogLevel_E; 


typedef enum CSSDPRet_E
{
	EM_SDP_RET_SUCCESS = 0,     	/*成功*/
    EM_SDP_RET_FAILURE,         	/*未知原因失败*/
    EM_SDP_RET_INVALPARA,       	/*参数无效*/
    EM_SDP_RET_NOMEM,           	/*超出内存使用限制或系统内存分配错误*/
    EM_SDP_RET_INVALVER,        	/*协议版本无效*/
    EM_SDP_RET_UNKNOWN_LINE,    	/*SDP数据中有不可识别行*/
    EM_SDP_RET_INVALBANDWIDTH,  	/*无效带宽*/
	EM_SDP_RET_TIME,            	/* t= 描述字解析错误*/
	EM_SDP_RET_REPEAT,          	/* r= 描述字解析错误*/
	EM_SDP_RET_REPEAT_NOTIME,   	/*当前时间未空 */
	EM_SDP_RET_TIME_ADJ,  	 		/* z= 描述字解析错误*/
	EM_SDP_RET_CONNECT,  			/* c= 描述字解析错误*/
	EM_SDP_RET_ORIGIN,  			/*o= 描述字解析错误 */
	EM_SDP_RET_MEDIA,  				/*m= 描述字解析错误 */
	EM_SDP_RET_KEY,  				/* k=  描述字解析错误*/
	EM_SDP_RET_ATTRIBUTES_NO_COLON  /*无法找到:分割符 */
} CSSDPRet_E;

/*数据链表结构*/
typedef struct CSSDPStringList_S
{
	struct CSSDPStringList_S *psNext;
	char   *pszStringVal;
} CSSDPStringList_S;

/*parses b=<modifier>:<value>*/
typedef enum CSSDPBandwidthModifier_E
{
	EM_SDP_BANDWIDTH_MODIFIER_NONE = 0,
	EM_SDP_BANDWIDTH_MODIFIER_CT,     
	EM_SDP_BANDWIDTH_MODIFIER_AS,     
	EM_SDP_BANDWIDTH_MODIFIER_USER   
} CSSDPBandwidthModifier_E;

/*
* These next 2 set of defines are used in CSSDPSessionDesc_t field conf_type
* and CSSDPMediaDesc_t field orient_type. They need to be in this order
* (0 for no value,user value at end),so the processing routine in
* sdp_decode.c can use an array lookup. See type_values,orient_values
* static variables and the check_value_list_or_user() routine.
* If you want to add a value,add before the user value,in the #define
* and in the static variable.
*/

#define CONFERENCE_TYPE_NONE 0
#define CONFERENCE_TYPE_BROADCAST 1
#define CONFERENCE_TYPE_MEETING 2
#define CONFERENCE_TYPE_MODERATED 3
#define CONFERENCE_TYPE_TEST 4
#define CONFERENCE_TYPE_H332 5
#define CONFERENCE_TYPE_OTHER 6

#define ORIENT_TYPE_NONE 0
#define ORIENT_TYPE_PORTRAIT 1
#define ORIENT_TYPE_LANDSCAPE 2
#define ORIENT_TYPE_SEASCAPE 3
#define ORIENT_TYPE_USER 4

typedef struct CSSDPBandwidth_S
{
	  struct CSSDPBandwidth_S *psNext;    /*带宽信息的链表指针*/
	  CSSDPBandwidthModifier_E eModifier; /*带宽信息的种类，eg.CT、AS*/
	  double  Bandwidth;                  /*带宽信息（通常是最大带宽）*/
	  char    *pszUserBand;
} CSSDPBandwidth_S; 

typedef struct CSSDPCategoryList_S
{
	  struct CSSDPCategoryList_S *psNext;  /*会话目录信息链表指针*/
	  SDPUINT64 u64Category;                  /*会话目录信息*/
} CSSDPCategoryList_S;


/*c=IN IP4 0.0.0.0 (连接的信息)*/
typedef struct CSSDPConnectDesc_S
{
	  char  *pszConnType;   /*"ip4"*/
	  char  *pszConnAddr;   /*conn_addr="0.0.0.0"*/
	  DWORD dwTTL;          /*使用组播地址也应该指定TTL，它们一起定义了组播数据报能够发送的范围，TTL值在0-255之间。TTL在斜线后给出*/
	  DWORD dwNumAddr;      /*标识组播的组数*/
	  int   nIsUsed;        /*是否使用标识*/
} CSSDPConnectDesc_S;

/*标识密钥由下列类型给出或者标识获取密钥的外部方法*/
typedef enum CSSDPKeyTypes_E
{
	EM_SDP_KEYTYPE_NONE = 0,  /*SDP中不包含Key字段*/
	EM_SDP_KEYTYPE_PROMPT,    /*Key字段不包含密钥*/
	EM_SDP_KEYTYPE_CLEAR,     /*Key字段给出密钥，且没做任何变换*/
	EM_SDP_KEYTYPE_BASE64,    /*Key字段给出密钥，但被Base64编码*/
	EM_SDP_KEYTYPE_URI        /*Key给出的密钥包含URI*/
} CSSDPKeyTypes_E;

typedef struct CSSDPKeyDesc_S
{
	CSSDPKeyTypes_E eKeyType; /*Key字段类型*/
	char *pszKey;             /*Key字段值*/
} CSSDPKeyDesc_S;

typedef struct CSSDPRtpmapDesc_S
{
	char  *pszEncodeName;     /*编码方式*/
	DWORD dwClockRate;        /*时钟采样率*/
	DWORD dwEncodeParam;      /*解码参数，视频没有解码参数，音频指定音轨数量*/
} CSSDPRtpmapDesc_S ;


typedef struct CSSDPFormatList_S
{
	struct CSSDPFormatList_S *psNext;  /*存放媒体格式的链表*/
	struct CSSDPMediaDesc_S  *psMedia; /*指向结构CSSDPMediaDesc_S的指针*/
	char                     *pszFmt;  /*格式类型*/
	CSSDPRtpmapDesc_S        *psRtpmap;/*执行结构CSSDPRtpmapDesc_S的指针*/
	char                     *pszFmtParam;/*eg:fmtp:96 profile-level-id=1; config=000001b003000001b50b…*/
                                          /*(负载类型是96，profile级别是1，视频的config信息)*/
}CSSDPFormatList_S;

/*eg:a=range:npt=0-4926.21294 (会话从0秒到4926.21294秒)*/
typedef struct CSSDPRangeDesc_S
{
	int     nHaveRange;       /*当设置好后把本字段置为true,同时本字段标识本结*/
							  /*构只设置一次，如果设为true了，如果再来设置则返回*/
	int     nRangeIsNpt;      /*是否使用NPT，如果使用设为ture*/
	int     nRangeIsClock;	/*是否使用clock，如果使用设为ture*/					  
	double  RangeStart;       /*开始时间*/
	double  RangeEnd;         /*播放完整个节目的结束时间*/
	WORD    wRangeSmpteFps;   /*使用SMPTE格式*/
	int     nRangeEndInfinite;/*如果没有结束时间，则本字段赋值为TRUE，标示无穷进播放*/
} CSSDPRangeDesc_S;

/* basic structure for definition of a media.*/
typedef struct  CSSDPMediaDesc_S
{
	struct CSSDPMediaDesc_S   *psNext;    /*有多种媒体时建立链表，例如音频、视频*/
	struct CSSDPSessionDesc_S *psParent;  /*指向结构CSSDPSessionDesc_S的指针*/
	char *pszMediaName;                   /*media name*/
	char *pszProto;                       /*protocol used*/
	char *pszSdpLang;                      
	char *pszLang;
	char *pszMediaDesc;                   /*description string*/
	char *pszControlString;               /*rtsp control string*/
	CSSDPFormatList_S *psMediaFmt;        /*媒体格式，如果有多种，以链表标识*/
	CSSDPStringList_S *psUnparsedLines;   /*没有解析出来的协议，存放在链表中*/
	int    nRecvonly;
	int    nSendrecv;
	int    nSendonly;
	WORD   wIpPort;                       /*ip port*/
	WORD   wNumOfPorts;                   /*number of ports*/
	DWORD  dwPtime;
	int    nPtimePresent;
	DWORD  dwQuality;
	int    nQualityPresent;
	double Framerate;
	int    nFrameratePresent;
	CSSDPConnectDesc_S sMediaConnect;
	CSSDPRangeDesc_S   sMediaRange;
	CSSDPBandwidth_S   *psMediaBandwidth;
	int                nOrientType;
	char               *pszOrientUserType;
	CSSDPKeyDesc_S     sKey;
	SDPUINT64 u64Xcreatedate;/*a=X-createdate:1123736310*/
	char   *pszXConttype; /*a=X-conttype:m2t*/
	char   *pszXClip;     /*a=:X-clipa1*/
	char   *pszXName;     /*a=X-name:/0811test2*/	
	double XDuration;     /*a=X-duration:2666.440*/	
	double XBitrate;      /*a=X-bitrate:3100000.000*/ 
} CSSDPMediaDesc_S;

typedef struct CSSDPTimeAdjDesc_S
{
	  struct CSSDPTimeAdjDesc_S *psNext;  
	  cstime_t adj_time;       				 /*开始转换的时间点*/
	  int nOffset;           				 /*时间偏移量*/
} CSSDPTimeAdjDesc_S;

typedef struct CSSDPTimeRepeatDesc_S
{
	  struct CSSDPTimeRepeatDesc_S *psNext;  /*重复时间描述信息链表指针*/
	  DWORD dwRepeatInterval;                /*重复时间间隔*/
	  DWORD dwActiveDuration;                /*会话持续时间*/
	  DWORD dwOffsetCnt;                     /*时间偏移值的个数*/
	  DWORD dwOffsets[SDP_MAX_REPEAT_OFFSETS];   /*时间偏移值数组*/
} CSSDPTimeRepeatDesc_S;

/*t=0 0 (会议会话开始的时间是0，结束的时间也是0，代表一个永久连接)*/
typedef struct CSSDPSessionTimeDesc_S
{
	struct CSSDPSessionTimeDesc_S *psNext; /*时间信息链表指针*/
	cstime_t start_time;                     /*开始时间*/
	cstime_t end_time;                       /*结束时间*/
	CSSDPTimeRepeatDesc_S *psRepeat;       /*会话重复时间描述结构指针*/
} CSSDPSessionTimeDesc_S;


/*会话描述结构*/
typedef struct CSSDPSessionDesc_S
{
	struct  CSSDPSessionDesc_S *psNext;      /*session链表指针*/
	/* o= fields */
	char   *pszOrigUsername;                 /*用户登录名*/
	SDPUINT64 u64SessionId;                     /*会话id*/
	SDPUINT64 u64SessionVersion;                /*会话版本号*/
	char   *pszCreateAddrType;               /*IP类型，IP4/IP6*/
	char   *pszCreateAddr;                   /*创建会话的主机IP地址 */
	CSSDPCategoryList_S *psCategoryList;     /*种类列表，没有被填充*/
	/* s= field */
	char   *pszSessionName;                  /*会话名字*/
	/* i= field */
	char   *pszSessionDesc;                  /*会话信息*/
	/* u = field */
	char   *pszURI;                          /*描述的URI*/
	/* Administrator Information */
	CSSDPStringList_S *psAdminPhone;         /*电话号码, 可以有多个*/
	CSSDPStringList_S *psAdminEmail;         /*email地址, 可以有多个*/
	/* connect info */
	CSSDPConnectDesc_S sSessionConnect;      /*会话链接信息*/
	CSSDPRangeDesc_S sSessionRange;         /*影片播放范围*/
	CSSDPBandwidth_S *psSessionBandwidth;    /*会话带宽信息*/
	/* key type */
	CSSDPKeyDesc_S sKey;                     /*Key字段信息*/
	char *pszKeywds;                       	 /*会话的关键字*/
	char *pszTool;                         	 /*创建会话描述的工具的名称和版本*/
	char *pszCharset;                        /*显示会话名称、信息的字符集*/
	char *pszSdplang;                        /*会话描述的语言*/
	char *pszLang;                         	 /*会话的默认语言*/
	char *pszControlString;                  /*绝对的URL*/ 
	char *pszEtag;                         
	/* start/end times,in an array form */ 
	CSSDPSessionTimeDesc_S *psTimeDesc;      /*存放播放的开始与结束时间*/
	CSSDPTimeAdjDesc_S *psTimeAdjDesc;  
	/* media descriptions */
	CSSDPMediaDesc_S *psMedia;               /*存放媒体描述信息*/
	/* unparsed lines */
	CSSDPStringList_S *psUnparsedLines;      /*存放未被解析的行*/
	int  nConfType;                          /*会话控制信息类型*/
	char *pszConfTypeUser;                   /**/
	int  nRecvonly;                          /*只接收*/
	int  nSendrecv; 						 /*接收和发送*/
	int  nSendonly;                          /*只发送*/
} CSSDPSessionDesc_S;

//typedef struct sdp_decode_info_ CSSDPDecodeInfo_S;
typedef struct CSSDPDecodeInfo_S
{
	int   nMode;              /*文件方式为0， 内存方式为1，其他不与处理*/
	const char * pszMemptr;   /*指向SDP数据内存*/
	FILE  *pFile;             /*文件指针*/
} CSSDPDecodeInfo_S;

/******************************************************************************
函数名称:	CSSDPDecode
功能描述:
    从输入的结构CSSDPDecodeInfo_S中取得SDP数据，并进行解析。
    解析中包含了RFC2326中要求的所有描述字。
    
输入参数:
    psDecodeInfo：用于SDP解码的数据结构体
    
输出参数:
    ppsSessionList：输出的会话列表
	pnSessionInfoNum：会话列表中会话信息个数

返 回 值:
    函数成功，返回EM_SDP_RET_SUCCESS。
	函数失败，返回其他错误码。
	
其他说明:

******************************************************************************/
int CSSDPDecode ( CSSDPDecodeInfo_S  *psDecodeInfo,
				  CSSDPSessionDesc_S **ppsSessionList,
							    int  *pnSessionInfoNum);

/******************************************************************************
函数名称:	CSSDPFreeSessionDesc
功能描述:
    释放SDP会话结构CSSDPSessionDesc中各字段申请的内存。
    
输入参数:
    psSessionDesc：SDP解码后数据结构体CSSDPSessionDesc的指针。
    
输出参数:
    无
    
返 回 值:
    无
    
其他说明:
******************************************************************************/
void CSSDPFreeSessionDesc (CSSDPSessionDesc_S *psSessionDesc);

/******************************************************************************
函数名称:	CSSDPSetLogLevel
功能描述:
    设置SDP模块打印等级,只有大于等于nLogLevel的日志才能被打印
    
输入参数:
    nLogLevel:打印等级,具体见CSSDPLogLevel_E

输出参数:
	无
	
返 回 值:
	无
	
其他说明:
******************************************************************************/
void CSSDPSetLogLevel(int nLogLevel);


#ifdef __cplusplus
}
#endif

#endif 


