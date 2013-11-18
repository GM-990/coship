/******************************************************************************
文件名称：cs_rtsp_client.h
版权所有(c) 2006， 深圳市同洲电子股份有限公司。

当前版本：Ver 1.0    
作    者：

内容摘要：
	rtsp模块提供的外部接口文件。

修改记录：
版本号		日期		作者			修改内容
--------	----------	----------		------------
1.0			2006-2-11	刘正华	重新修改接口重新排版
1.1         2006-6-22   刘正华  移植到PORTING层
2.0			2007-6-11	佟福磊	根据研发中心编码规范重新定义接口，整理代码
*******************************************************************************/

#ifndef __RTSP_CLIENT_H__
#define __RTSP_CLIENT_H__ 

/******************************************************************************
 *                                 头文件引用                                 *
 ******************************************************************************/
#include "mmcp_typedef.h"
/******************************************************************************
 *                                 宏、定义、结构体                           *
 ******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif


#define RTSP_RECV_BUF_LEN 		(5120)	/*接收服务器消息的缓冲区*/
#define RTSP_SEND_BUF_LEN 		(2048)     /*组装RTSP命令下发缓冲区*/
#define RTSP_URL_MAX_LEN		(1024)		/*url的最大长度*/
/*服务器发送过来消息的类型*/
typedef enum  
{
	EM_RTSP_SENDORDER=0,		/*服务器返回消息成功*/
	EM_RTSP_ANNOUNCE,			/*服务器主动下发ANNOUNCE*/
	EM_RTSP_OPTIONS,			/*服务器主动下发OPTIONS*/
	EM_RTSP_GETPARAMETER,		/*服务器主动下发GETPARAMETER */
	EM_RTSP_SETPARAMETER,		/*服务器主动下发SETPARAMETER */
	EM_RTSP_REDIRECT,			/*服务器主动下发REDIRECT */
	EM_RTSP_RTPDATA,			/*服务器下发rtp 数据 */
    EM_RTSP_SEND_ERROR_ORDER	/*服务器返回消息错误*/
} CSRTSPServerResponse_E;

typedef enum 
{
	EM_RTSP_TIME_NOFORMAT= 0,  /*没有标示时间范围的字段*/
	EM_RTSP_TIME_NPT,			 /*采用npt时间格式*/
	EM_RTSP_TIME_SMPTE			/*采用smpte时间格式*/
} CSRTSPTimeFormat_E;

/*日志级别描述，可显示小于指定级别的所有日志。*/
typedef enum CSRTSPLogLevel_E
{
	EM_RTSP_LOG_DEBUG = 0,
	EM_RTSP_LOG_INFO,
	EM_RTSP_LOG_NOTICE,
	EM_RTSP_LOG_WARNING,
	EM_RTSP_LOG_ERR,
	EM_RTSP_LOG_CRIT,
	EM_RTSP_LOG_ALERT,
	EM_RTSP_LOG_EMERG
} CSRTSPLogLevel_E;

/*用于组装发给服务器的RTSP消息的结构*/
typedef struct 
{
	char *pszAccept;           /*指示客户端所能识别的表示描述内容的类型*/
	char *pszAcceptEncoding;  /*示客户端所能识别的内容编码的格式，如compress。如果请求中没有Accept-Encoding消息头，服务器就认为客户端可以接受任何内容编码的格式*/
	char *pszAcceptLanguage;  /*指示所支持的表现描述和原因短语的语言类型*/
	char *pszAcceptCharset;/* Accept-Charset: UTF-8*/
	char *pszAcceptAuthentication;/* X-Accept-Authentication: Basic, NTLM, Digest*/
	char *pszAuthorization;	/*授权，认可*/
	DWORD uBandwidth;       /*描述了客户端估计能得到的现有的带宽*/
	DWORD uBlockSize;       /*是从客户端发送到媒体服务器的，询问服务器媒体包的大小，必须是八字节的正数，这个包的大小还不包括底层协议(IP,UDP,RTP)协议的包头大小*/
	char *pszCacheControl;/*Cache-Control: must-revalidate*/
	char *pszConference;
	char *pszConnection;
	char *pszFrom;
	char *pszProxyAuth;
	char *pszProxyRequire;
	char *pszRange;
	char *pszReferer;
	double Scale;/*是用来控制播放/记录的速率的。速率为1时，表示正常播放/记录速率。速率为2时，表示两倍快进速率。速率为0.5时，表示0.5速率的慢镜头。速率为-2时，表示两倍的快退速率。*/
	char *pszSession;/*随机选取的变长的至少8个字节,它由服务器分配*/
	double Speed;/*描述请求服务器按照指定的传输速率将数据传送到客户端,视服务器的能力和媒体流的需要而定*/
	char *pszTransport;/*用来配置流媒体的传输协议和传输参数(如目的地址,目的端口,多播TTL)*/
	char * pszDate;  /*消息发送时的时间，格式为yyyy-mm-dd hh:mm:ss */ 
	char *pszUseragent;
	char *pszUser;
	char *pszRequire;
	char *pszOVSExtensions;		/*ovs服务器扩展*/
	char *pszOVSMayForward;
	int  bHasMayForward;
	char *pszOVSPlayNow;
	int  bHasPlayNow;
	char *pszOVSPrepareAssets;
	int  bHasPrepareAssets;
	char *pszOVSMayNotify;
	int  bHasMayNotify;
	char *pszOVSNoFlush;
	int  bHasNoFlush;	
	char *pszClientId;       /*客户端ID号,机顶盒分配号*/
	char *pszTransactionId;   /*事务号*/
	char *pszContentLength;      /*消息体长度*/
	char *pszContentType;        /*消息体类型*/  
	char *pszBody;              /*消息体内容*/
	char *pszCommandExpand;	/*扩展接口*/
	char *pszCSeq;  /*序列号*/
} CSRTSPCommandInfo_S;

	
/*解析NPT消息结构*/
typedef struct  
{
	CSRTSPTimeFormat_E eTimeFormat;	/*0：没有时间范围，1：采用NPT时间格式，2：采用smpte时间格式*/
	double RangeStart;
	double RangeEnd;
	WORD wSmpteFps;  				/*SMPTE格式时描述帧速率*/

} CSRTSPRangeDesc_S;

/*服务器上报的所有RTSP消息都解析在结构 CSRTSPDecode_S中*/
typedef struct 
{
	DWORD uCseq;
	int bHaveCseq;
	int bCloseConnection;
	char acRetCode[4];         /* 3 byte return code - \0 delimited */
	char *pszRetResp;
	char *pszBody;              /* Contains body returned */
	char *pszAccept;
	char *pszAcceptEncoding;
	char *pszAcceptLanguage;
	char *pszAllowPublic;          /*options 返回的PUBLIC命令*/
	char *pszAuthorization;
	char *pszBandwidth;
	char *pszBlocksize;
	char *pszCacheControl;
	char *pszContentBase;	/*服务器返回的URL路径*/
	char *pszContentEncoding;/*Content-Encoding描述Entity-Body的编码格式，例如：Content-Encoding：gzip*/
	DWORD uContentLength; /*描述了内容(message body)的长度,单位是octet*/
	char *pszContentLanguage;
	char *pszContentLocation;/*描述了实体内容的位置.如果一个资源包含了多个实体,每个实体可以在不同的路径上,这时Conntent-Location指明了单个实体的路径*/
	char *pszContentType;/*描述了Entity-Body的媒体类型，例如：Content-Type：text/parameters*/
	char *pszCookie;
	char *pszDate;
	char *pszExpires;/*一个媒体描述或者媒体流的过期时间*/
	char *pszFrom;
	char *pszIfModifiedSince;
	char *pszLastModified;/*让服务器返回媒体描述或者媒体流最近一次的修改时间*/
	char *pszLocation;           /*重定位信息*/
	char *pszProxyAuthenticate;
	char *pszProxyRequire;
	char *pszRange;
	CSRTSPRangeDesc_S *psRangeDesc;/*解析range中的数据*/
	char *pszReferer;
	char *pszRequire;
	char *pszRetryAfter;
	char *pszRtpInfo;
	char *pszScale;
	char *pszServer;
	char *pszSession;
    char *pszSpeed;
    char *pszTransport;
	char *pszUnsupported;
	char *pszUserAgent;
	char *pszVia;
	char *pszWWWAuthenticate;
	char *pszAnnounceUrl;	
	char *pszServerDate;
	char *pszOVSExtensions;
	char *pszNotice;	/*<USE>x-notice: 2101 "End-of-Stream Reached" event-date=19990706T215715.777Z*/
	char *pszControlAddress;
	char *pszAuthenticationInfo;
	char *pszOVSPlayNow;
	char *pszOVSNoFlush;
	char *pszServerTimeout;
	char *pszExtensionsInfo; /*x-Info:"EOS"*/
	double  Position;   //x-position服务器应答中返回时间  单位是秒
	int  nTimeShiftTV;/*其值为1时表示切换到直播信道，x为0时表示切换到录播信道*/
	double VCRRange;/*且xxx.xxx不为0，则表示这是一个时移频道，其中xxx.xxx表示当前可以时移的最大时间，单位S*/
	char *pszExpandCommand;/*扩展解析字段*/	
} CSRTSPDecode_S;

/*函数接口返回值描述。*/
typedef enum 
{
	EM_RTSP_RESPONSE_SUCCESS = 0,				/*返回正确*/
	EM_RTSP_RESPONSE_REDIRECT, 	/*重定向处理，告诉上层已经重定向，使用新的链接*/
	EM_RTSP_RESPONSE_RECV_ERROR,  	/*解析返回数据接收错误，读数据超时，或SOCKET错误,关闭SOCKET*/
	EM_RTSP_RESPONSE_BAD,   	/*服务器返回错误码，不关闭SOCKET，许上层解析错误码，作后面处理*/
	EM_RTSP_RESPONSE_MISSING_OR_BAD_PARAM,   /*参数组织错误，许重新组装参数下发命令*/
	EM_RTSP_RESPONSE_BAD_URL,         		/*URL错误，组装新的URL*/
	EM_RTSP_RESPONSE_CLOSED_SOCKET, 		 /*SOCKET号错误*/
	EM_RTSP_RESPONSE_MALFORM_HEADER,  	/*服务器返回数据解析时出错*/
	EM_RTSP_RESPONSE_RTP_MESSAGE,     /*服务器返回数据为RTP数据*/
	EM_RTSP_SELECT_RECV_ERROR,         /*SELECT 出错返回*/
	EM_RTSP_SELECT_TIMEOUT,            	/*SOCKET 超时处理*/
	EM_RTSP_REDIRECT_FAILED,           	/*重定向，重新建立SOCEKET时失败*/
	EM_RTSP_SEND_FAILED,                	/*发送数据失败*/
	EM_RTSP_RECV_NOT_COMPLETE,        /*本次接收数据不够解析*/
	EM_RTSP_PARAMETER_ERR,
	EM_RTSP_FAILURE,
	EM_RTSP_REBUILD			/*tcp重连*/
} CSRTSPRet_E;

typedef enum 
{
	EM_RTSP_CALLBACK_RECVRTSP = 0,		/*向上层报告服务器发送的RTSP消息*/
	EM_RTSP_CALLBACK_RECVRTP			/*向上层发送RTP数据*/
} CSRTSPCallback_E;

typedef void (*FonRtspRecvMsg)(
			CSRTSPCallback_E 			eCallbackType,/*回调函数类型*/
			CSRTSPServerResponse_E 		eResponseType, /*服务器响应类型*/
			CSRTSPDecode_S				*psResp,/* eCallbackType */
			char						*pcRtpBuf ,
			BYTE						ucRtpChannel,
			WORD 						wBufLen,
			char 						*pcUserData);

/******************************************************************************
*                                 函数声明                                   *
******************************************************************************/

/******************************************************************************
函数名称:	CSRTSPCreateClient
功能描述:
    建立客户端与服务器的连接
输入参数:
pszUrl		输入节目的URI,pszUrl是以'\0'结束的字符串，其总长度不超过1024个字节,
			存储空间由调用者申请；
dwTimeout 	建立链接超时，单位毫秒，dwTimeout必须大于0。

输出参数:
phClientHandle 客户端的句柄。

返 回 值:
    如果成功，返回0；否则返回错误号。
其他说明:
******************************************************************************/	
int CSRTSPCreateClient(char *pszUrl, DWORD dwTimeout,HCSHANDLE *phClientHandle);

/******************************************************************************
函数名称:	CSRTSPDestroyClient
功能描述:
    销毁客户端句柄。
输入参数:
hClientHandle  客户端句柄。

输出参数:

返 回 值:
无
其他说明:
******************************************************************************/	
int CSRTSPDestroyClient(HCSHANDLE hClientHandle);

/******************************************************************************
函数名称:	CSRTSPSendDescribe
功能描述:
    如果nSendOnly为1，组装DESCRIBE命令结构,并发送到服务器，发送完毕后立即返回。
	如果nSendOnly为0，组装DESCRIBE命令结构发送到服务器，并解析服务器返回的消息，
解析结果由ppsDecodeResult带出。通过此接口可获得节目相关的描述信息。

输入参数:
hClientHandle		接口3.1创建的客户端句柄；
sCmd	 			组装RTSP命令的消息结构，由调用者分配内存；
dwRedirectUrlLen  	pszRedirectUrl的长度；
nSendOnly			1：发送后立即返回，ppsDecodeResult和pszRedirectUrl可为NULL;
					0：发送并接受消息，ppsDecodeResult和pszRedirectUrl均不能为NULL。


输出参数:
ppsDecodeResult  	外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向的内存区进行更改操作，也不能销毁；
pszRedirectUrl  	服务器返回的重定向url，由调用者分配内存，返回以'\0'结束的字符串。

返 回 值:
如果成功，返回0，否则返回错误号。
其他说明:
******************************************************************************/	
int CSRTSPSendDescribe(HCSHANDLE			hClientHandle,
						CSRTSPCommandInfo_S	 	sCmd, 
									DWORD		dwRedirectUrlLen,
									int		nSendOnly,
									char 		*pszRedirectUrl, 
							CSRTSPDecode_S 	    **ppsDecodeResult);

/******************************************************************************
函数名称:	CSRTSPSendSetup
功能描述:
	如果nSendOnly为1，组装SETUP命令结构,并发送到服务器，发送完毕后立即返回。
	如果nSendOnly为0，组装SETUP命令结构发送到服务器，并解析服务器返回的消息，
解析结果由ppsDecodeResult带出。通过次接口可与服务器协商传输相关的参数。
输入参数:
hClientHandle	接口3.1创建的客户端句柄；
pszUrl			需建立连接的流对应的"子"url, pszUrl是以'\0'结束的字符串，其总长度不超过1024个字节，存储空间由调用者申请；
sCmd	 		组装RTSP命令的消息结构，由调用者分配内存；
nSendOnly		1：发送后立即返回，ppsDecodeResult和phSessionHandle可为NULL;
				0：发送并接受消息，ppsDecodeResult和phSessionHandle均不能为NULL。

输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向的内存区进行更改操作，也不能销毁；
phSessionHandle  获取会话信息存储的句柄。


返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
******************************************************************************/	
int CSRTSPSendSetup(HCSHANDLE				hClientHandle, 
		     					 char			*pszUrl, 
		     	CSRTSPCommandInfo_S 			sCmd, 
		 						int			nSendOnly,
		     			 HCSHANDLE			*phSessionHandle, 
					CSRTSPDecode_S 	  		**ppsDecodeResult);

/******************************************************************************
函数名称:	CSRTSPSendPlay
功能描述:
	如果nSendOnly为1，组装PLAY命令结构,并发送到服务器，发送完毕后立即返回。
	如果nSendOnly为0，组装PLAY命令结构发送到服务器，并解析服务器返回的消息，
解析结果由ppsDecodeResult带出。通过本函数可以将节目播放起来。
输入参数:
hSessionHandle	会话的句柄，下发CSRTSPSendSetup时创建；
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。
输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对
					其指向的内存区进行更改操作，也不能销毁。
如果成功，返回0，否则返回错误号
其他说明:
	如果本消息第一次下发，建议nSendOnly置0，因为考虑到如果本命令不能正确收到就
	不能进行下面的操作。如果在暂停后再下发播放，建议nSendOnly置1，因为阻塞式的
	接收命令，可能会影响的上层处理的效率。
******************************************************************************/
int CSRTSPSendPlay(HCSHANDLE	   		hSessionHandle,
				CSRTSPCommandInfo_S 	   	sCmd,
							int			nSendOnly,
			            CSRTSPDecode_S     	**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendPlayByClientHandle
功能描述:
	如果nSendOnly为1，组装PLAY命令结构,并发送到服务器，发送完毕后立即返回。
	如果nSendOnly为0，组装PLAY命令结构发送到服务器，并解析服务器返回的消息，
解析结果由ppsDecodeResult带出。通过本函数可以将节目播放起来。
输入参数:
hClientHandle	创建的RTSP客户端句柄；
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。
输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对
					其指向的内存区进行更改操作，也不能销毁。
如果成功，返回0，否则返回错误号
其他说明:
	如果本消息第一次下发，建议nSendOnly置0，因为考虑到如果本命令不能正确收到就
	不能进行下面的操作。如果在暂停后再下发播放，建议nSendOnly置1，因为阻塞式的
	接收命令，可能会影响的上层处理的效率。
与CSRTSPSendPlay 不同之处:
	CSRTSPSendPlay 第一个参数:是由 CSRTSPSendSetup创建所得的会话句柄
	CSRTSPSendPlayByClientHandle 第一个参数句柄 :是创建的RTSP客户端的句柄

******************************************************************************/

int CSRTSPSendPlayByClientHandle(HCSHANDLE	hClientHandle,
                   CSRTSPCommandInfo_S sCmd,
                   int	nSendOnly,
                   CSRTSPDecode_S **ppsDecodeResult);

/******************************************************************************
函数名称:	CSRTSPSendPause
功能描述:
	如果nSendOnly为1，组装PAUSE命令结构,并发送到服务器，发送完毕后立即返回。
	如果nSendOnly为0，组装PAUSE命令结构发送到服务器，并解析服务器返回的消息，
解析结果由ppsDecodeResult带出。通过本函数可以将播放的节目暂停播放。
输入参数:
hSessionHandle	会话的句柄，下发CSRTSPSendSetup时创建；
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。
输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对
				其指向的内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。

******************************************************************************/
int CSRTSPSendPause(HCSHANDLE	   		hSessionHandle,
					CSRTSPCommandInfo_S 	   	sCmd,
								int			nSendOnly,
			            CSRTSPDecode_S     	**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendPauseByClientHandle
功能描述:
	如果nSendOnly为1，组装PAUSE命令结构,并发送到服务器，发送完毕后立即返回。
	如果nSendOnly为0，组装PAUSE命令结构发送到服务器，并解析服务器返回的消息，
解析结果由ppsDecodeResult带出。通过本函数可以将播放的节目暂停播放。
输入参数:
hSessionHandle	创建的RTSP客户端句柄；
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。
输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对
				其指向的内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。
与CSRTSPSendPause 不同之处:
	CSRTSPSendPause 第一个参数:是由 CSRTSPSendSetup创建所得的会话句柄
	CSRTSPSendPauseByClientHandle 第一个参数句柄 :是创建的RTSP客户端的句柄
	
*/
int CSRTSPSendPauseByClientHandle(HCSHANDLE	hClientHandle,
                    CSRTSPCommandInfo_S sCmd,
                    int	nSendOnly,
                    CSRTSPDecode_S **ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendTeardown
功能描述:
	如果nSendOnly为1，组装TEARDOWN命令结构,并发送到服务器，发送完毕后立即返回。
	如果nSendOnly为0，组装TEARDOWN命令结构发送到服务器，并解析服务器返回的消息，
解析结果由ppsDecodeResult带出。通过本函数可以将已播放的节目停止。
输入参数:
hSessionHandle	会话的句柄，下发CSRTSPSendSetup时创建；
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。
输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向
					的内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。
******************************************************************************/
int CSRTSPSendTeardown(HCSHANDLE	   	hSessionHandle,
				CSRTSPCommandInfo_S 	   		sCmd,
								int			nSendOnly,
			            CSRTSPDecode_S     		**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendGetParameter
功能描述:
	如果nSendOnly为1，组装GETPARAMETER命令结构,并发送到服务器，发送完毕后立即返回。
	如果nSendOnly为0，组装GETPARAMETER命令结构发送到服务器，并解析服务器返回的消息，
解析结果由ppsDecodeResult带出。
    
输入参数:
hClientHandle  客户端句柄。
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。


输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向的
				内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。
	本命令根据服务器支持的不同，有不同的命令组装，例如：kasenna服务器可以支持在消息
体中组装"position"得到当前节目播放的时间，组装"scale"可以得到当前服务器支持的快进快
退的速率。但大多数服务器用他来进行与服务器进行握手。
******************************************************************************/
int CSRTSPSendGetParameter(HCSHANDLE			hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendSetParameter
功能描述:
    向服务器发送setparameter命令，并返回解析结果。
    有的服务器不能支持本命令，在组装下发时，需要先了解服务器是否支持
    
输入参数:
hClientHandle  客户端句柄。
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。

输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向
				的内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。
	根据服务器支持的不同，它也用不同的用途，比如微软服务器通过次命令告诉客户流播放
完毕，或者流快退的开始处。
******************************************************************************/
int CSRTSPSendSetParameter(HCSHANDLE			hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendOptions
功能描述:
    向服务器发送options命令，并返回解析结果。通过下发本命令可以得到服务器支持的rtsp命令。
    有的服务器不能支持本命令，在组装下发时，需要先了解服务器是否支持
    
输入参数:
hClientHandle  客户端句柄。
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。

输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向
				的内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。

******************************************************************************/
int CSRTSPSendOptions(HCSHANDLE					hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendAnnounce
功能描述:
    向服务器发送announce命令，并返回解析结果。在实际应用中本命令由服务器主动发送的比较多，
    可以在节目播放完毕后或者快退到开头时通过本命令通知上层
    
输入参数:
hClientHandle  客户端句柄。
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。

输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向
				的内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。
	ANNOUNCE方法用于两个用途：当从客户端发送到服务器时，ANNOUNCE提交由请求URI确定
的媒体对象的描述给服务器。当从服务器发送到客户端时，ANNOUNCE实时（real-time）地
更新会话描述。在实际应用中本命令由服务器主动发送的比较多，可以在节目播放完毕后或
者快退到开头时通过本命令通知上层（如kasenna服务器）。
******************************************************************************/
int CSRTSPSendAnnounce(HCSHANDLE				hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendRecord
功能描述:
    向服务器发送record命令，并返回解析结果。在实际应用中本命令由服务器主动发送的比较多，
    可以在节目播放完毕后或者快退到开头时通过本命令通知上层
    
输入参数:
hClientHandle  客户端句柄。
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。

输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向
				的内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。

******************************************************************************/
int CSRTSPSendRecord(HCSHANDLE					hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPRecvRtspMessage
功能描述:
	本接口实现的功能可以接收服务器发送的任何消息，包括，rtsp应答消息，主动下发rtsp消息，
rtp over rtsp消息。
	本函数的处理原则是：调用模块监听到有数据后，调用本函数接完数据，然后对数据进行解析，
解析的原则是，如果不是一条完整的数据，就不作解析直接返回，保留原有的数据接收缓冲去不便，
如果是一条完整的数据，解析完后，移动数据缓冲区的偏移指针，将解析完的数据覆盖掉。

  
输入参数:
hClientHandle      	客户端句柄;
dwTimeout   		超时时间，单位毫秒。
输出参数:
无
返 回 值:
 如果成功，返回0，否则返回错误号
其他说明:
******************************************************************************/
int CSRTSPRecvRtspMessage(HCSHANDLE	hClientHandle,DWORD dwTimeout);
/******************************************************************************
函数名称:	CSRTSPGetRtspMessage
功能描述:
	本接口实现的功能可以主动获取服务器发送的数据。
  
输入参数:
hClientHandle      	客户端句柄; 
dwTimeout   		超时时间，单位毫秒。

输出参数:
pcBuf				存放rtp数据的缓冲区,由用户申请;
pdwBufLen			实际存放到缓冲区的rtp数据长度;
pChannelId			rtp数据的;
pnMsgType			服务器返回消息的类型；
ppsDecodeResult		外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其
					指向的内存区进行更改操作，也不能销毁。

返 回 值:
 如果成功，返回0，否则返回错误号
其他说明:
******************************************************************************/
int CSRTSPGetRtspMessage(HCSHANDLE 			hClientHandle,
									DWORD 			uTimeout,
									char 			*pcBuf,
									DWORD 			*puBufLen,
									BYTE			*pChannelId,
									int				*pnMsgType,
									CSRTSPDecode_S	**ppsDecodeResult);
/******************************************************************************
函数名称:	CSRTSPSendResponse
功能描述:
用来应答服务器主动上报的消息。如果服务器主动上报的消息需要应答时，
可以调用本接口，组装相应的消息应答服务器

  
输入参数:
hClientHandle   接口3.1创建的客户端句柄;
pszStatus	 	响应号，必须是以'\0'结束的字符串，例如"200"；
pszStatusDesc	响应描述，必须是以'\0'结束的字符串，例如"ok"；
pszSession		会话号，必须是以'\0'结束的字符串，例如"22456712566"；
sCmd    		RTSP命令组装的消息结构。

输出参数:
无
返 回 值:
 如果成功，返回0，否则返回错误号
其他说明:
******************************************************************************/
int CSRTSPSendResponse (HCSHANDLE	hClientHandle,
							char *pszStatus,
                            char *pszStatusDesc,
                            char *pszSession,
                            CSRTSPCommandInfo_S sCmd);
/******************************************************************************
函数名称:	CSRTSPSetCallbackFunc
功能描述:
设置回调函数。
  
输入参数:
hClientHandle			客户端句柄;
fnCallback				要注册的回调函数指针;
pcUserdata				回调函数的用户参数。


输出参数:
无
返 回 值:
 如果成功，返回0，否则返回错误号
其他说明:
******************************************************************************/
int CSRTSPSetCallbackFunc (HCSHANDLE			hClientHandle,
							FonRtspRecvMsg			fnCallback,
									char			*pcUserdata); 

/******************************************************************************
函数名称:	CSRTSPSetLogLevel
功能描述:
    设置日志打印级别
    
输入参数:
int nLogLevel  日志打印级别

输出参数:
无
返 回 值:
无
其他说明:
******************************************************************************/
void CSRTSPSetLogLevel(int nLogLevel);

/******************************************************************************
函数名称:	CSRTSPSetRecvTimeout
功能描述:
   设置接收数据超时
输入参数:
hClientHandle		客户端句柄;
dwTimeout        	超时时间，单位毫秒.

输出参数:
	无
返 回 值:
    成功0，错误其他号

其他说明:
******************************************************************************/
int CSRTSPSetRecvTimeout(HCSHANDLE	hClientHandle,DWORD dwTimeOut);
/******************************************************************************
函数名称:	CSRTSPSetSendTimeout
功能描述:
   设置发送数据超时
输入参数:
hClientHandle		客户端句柄;
dwTimeout        	超时时间，单位毫秒.

输出参数:
	无
返 回 值:
    成功0，错误其他号

其他说明:
******************************************************************************/
int CSRTSPSetSendTimeout(HCSHANDLE	hClientHandle,DWORD dwTimeOut);

/******************************************************************************
函数名称:	CSRTSPCloseServerSocket
功能描述:
    关闭控制连接socket
输入参数:
hClientHandle		客户端句柄;

输出参数:
	无
返 回 值:
    成功0，错误其他号

其他说明:
******************************************************************************/

int CSRTSPCloseServerSocket(HCSHANDLE hClientHandle);

/* add by zch 
创建控制连接*/
/******************************************************************************
函数名称:	CSRTSPCloseServerSocket
功能描述:
	创建控制连接socket
输入参数:
hClientHandle		客户端句柄;
dwTimeout           超时时间，单位毫秒.

输出参数:
	无
返 回 值:
	成功0，错误其他号

其他说明:
******************************************************************************/
int CSRTSPCreateSocket(HCSHANDLE hClientHandle,DWORD dwTimeout);


/******************************************************************************
函数名称:	CSRTSPSetCseqStrategy
功能描述:
	设置RTSP发送时包序号增加策略
输入参数:
hClientHandle		客户端句柄;
nStrategy          0:收到服务器响应后递增
			   1:发送时递增

输出参数:
	无
返 回 值:
	成功0，错误其他号

其他说明:
******************************************************************************/

// 默认收到服务器响应后递增，
// 0:收到服务器响应后递增
// 1:发送时递增
int CSRTSPSetCseqStrategy(HCSHANDLE hClientHandle,int nStrategy);

/*
函数名称:	CSRTSPSendPing
功能描述:
    向服务器发送PINH命令，并返回解析结果。
    有的服务器不能支持本命令，在组装下发时，需要先了解服务器是否支持
    
输入参数:
hClientHandle  客户端句柄。
sCmd   			组装RTSP命令的消息结构；
nSendOnly		1：发送后立即返回，ppsDecodeResult可为NULL；
				0：发送并接受消息，ppsDecodeResult不能为NULL。

输出参数:
ppsDecodeResult  外部模块传入的指针，他指向数据解析的内存，但外部模块不能对其指向
				的内存区进行更改操作，也不能销毁。
返 回 值:
如果成功，返回0，否则返回错误号
其他说明:
	下发此命令时建议nSendOnly置1，因为阻塞式的接收命令，可能会影响的上层处理的效率。
	根据服务器支持的不同，它也用不同的用途，比如微软服务器通过次命令告诉客户流播放
完毕，或者流快退的开始处。
    用于向服务器发送心跳。
*/
int CSRTSPSendPing(HCSHANDLE    hSessionHandle,
                    CSRTSPCommandInfo_S sCmd,
                    int nSendOnly,
                    CSRTSPDecode_S **ppsDecodeResult);
                    
                    
/******************************************************************************
函数名称:	CSRTSPGetCurrentSeq
功能描述:
    获取当前RTSP 解析的序列号
输入参数:
hClientHandle		客户端句柄;

输出参数:
	无
返 回 值:
    成功0，错误其他号

其他说明:
******************************************************************************/
int CSRTSPGetCurrentSeq(HCSHANDLE hClientHandle);
/******************************************************************************
函数名称:	CSRTSPSetCurrentUrl
功能描述:
    设置当前发送的RTSP 命令头带的URL
输入参数:
hClientHandle		客户端句柄;
pcUrl  要设置的url串
输出参数:
	无
返 回 值:
    成功0，错误其他号

其他说明:
******************************************************************************/
int CSRTSPSetCurrentUrl(HCSHANDLE hClientHandle, char *pcUrl);

int CSRTSPModifySeesionUrl(HCSHANDLE	 hSessionHandle, char *pcUrl);

int CSRTSPModifyCurrentSeq(HCSHANDLE hClientHandle, int nOffSet);

#ifdef __cplusplus
}
#endif

#endif
