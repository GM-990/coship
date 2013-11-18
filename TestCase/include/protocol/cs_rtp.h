/******************************************************************************
文件名称：CS_RTP.H
版权所有(c) 2007， 深圳市同洲电子股份有限公司。

当前版本：Ver 1.0    
作    者：段续斌 (duanxubin@coship.com)

内容摘要：
		文件功能说明

修改记录：
版本号		日期		作者			修改内容
--------	----------	----------		------------
1.0			2007-5-28	段续斌	创建
1.1			2009-4-24	陈      琳   	修改
*******************************************************************************/

#ifndef CSRTP_H
#define CSRTP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "udi2_socket.h"
#include "mmcp_typedef.h"
/******************************************************************************
 *                                 头文件引用                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 宏、定义、结构体                           *
 ******************************************************************************/

/******************************************************************************
 *                                 函数声明                                   *
 ******************************************************************************/

/*RTP对象句柄,屏蔽RTP对象的实现细节*/
typedef	VOID*		HRTPHANDLE;

/*RTP模块返回错误码,0表示执行成功,其他表示失败*/
typedef enum
{
	EM_RTP_ERROR = -100, 	/*错误号开始*/
	EM_RTP_PARAERR,		/*输入参数错误*/
	EM_RTP_NOMEMORY, 		/*内存没有申请到*/
	EM_RTP_GOOD = 0		/*函数执行成功*/
}CSRTPErrorCode_E;


/*RTP回调类型, 使用方法参见CSRTPCallback的使用方法*/
typedef enum 
{
        EM_RX_RTP, 		/*RTP数据包*/
        EM_RX_SR, 		/*SR包*/
        EM_RX_RR, 		/*RR包*/
        EM_RX_SDES,		/*SDES 条目*/
        EM_RX_BYE,         	/* Source is leaving the session, database entry is still valid                           */
        EM_SOURCE_CREATED, /*一个源被创建*/
        EM_SOURCE_DELETED, /* Source has been removed from the database                                              */
        EM_RX_RR_EMPTY,    	/* We've received an empty reception report block */
        EM_RR_TIMEOUT, 	/*源超时*/
        EM_RX_APP 			/*APP包*/
} CSRTPEventType_E;

/*RTP回调事件结构, 使用方法参见CSRTPCallback的使用方法*/
typedef struct 
{
	DWORD		 	dwSSRC; /*同步源*/
	CSRTPEventType_E  	eType; 	/*包类型*/
	VOID			*pData; 	/*数据*/
	CSUDITimeval_S	 	*psTS; 	/*当前时间*/
} CSRTPEvent_S;


/*RTP模块返回错误码,0表示执行成功,其他表示失败*/
typedef enum
{
	EM_RTP_TYPE  = 0, 	/*RTP 数据*/
	EM_RTCP_TYPE 	 	/*RTCP数据*/
}CSRTPPacketType_E;

/*APP包解析后的回调结构, 使用方法参见CSRTPCallback的使用方法,.同时也是发送APP包的结构*/
typedef struct
{
	BYTE 	ucVersion; 		/*版本*/
	BYTE 	ucPadding;		/*填充位*/
	BYTE 	ucSubType;		/*应用使用*/
	BYTE 	ucPayloadType;	/*负载类型*/
	WORD  	wLength;			/* 数据长度  就是pcData 的长度       */
	DWORD   dwSSRC;
	CHAR    acName[4];       		/* four ASCII characters  */
	CHAR    *pcData;        		/* variable length field */
}CSRTPRtcpAPP_S;

/*RTP模块的事件回调,事件类型参见rtp_event_type*/
typedef void (*FOnRTPEvent)( HRTPHANDLE hRTPSession, 
								CSRTPEvent_S *psEvent,
								BYTE *pucUserdata);

/*发送RTCP数据包时,用户生产APP数据的回调,假如没有组册该函数,则RTCP包中将不包含APP数据.*/
typedef CSRTPRtcpAPP_S * (*FOnRTCPApp)(HRTPHANDLE hRTPSession, 
												DWORD dwSsrc, 
												INT nMaxSize);


HRTPHANDLE  CSRTPCreate(BYTE *pucUserdata, FOnRTPEvent fnCallback,DWORD dwMemoryLen);

INT CSRTPDestroy(HRTPHANDLE hRTPSession);

INT CSRTPParseData(HRTPHANDLE hRTPSession,
						CSRTPPacketType_E eType,
						INT nBuffeLen,
						BYTE * pucBuffer,DWORD dwTimeout);

INT CSRTPGetPayloadData(HRTPHANDLE hRTPSession,DWORD dwTimeout,
								BYTE *pcDemuxBuffer, 
								DWORD *nNeedDataLen);
INT CSRTPBuildRtcpPacket(HRTPHANDLE hRTPSession, 
								DWORD dwRtpTS,
								INT nBufLen,
								BYTE *pucBuffer,
								FOnRTCPApp fnAppCallback);
#ifdef __cplusplus
}
#endif

#endif  /* CSRTPINTERFACE_H */

/* 文件结束 */



