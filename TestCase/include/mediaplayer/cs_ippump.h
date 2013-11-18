/******************************************************************************
文件名:	CSIPPLAYPUMP.H
Copyright (c) 2007， Shen Zhen Coship Electronics CO.,LTD.。

版本:		Ver 1.0    
作者:		Nevill (luoyangzhi@coship.com)

文件描述:
    Playpump的接口文件，主要供Mediaplay使用。
	
    Playpump是对IP组播和点播的一种抽象，将其抽象成与tuner类似的功能，一旦调用
    connect函数对其进行"锁频"操作，流数据就会送入到硬件DEMUX中。然而这里的DEMUX
    是能够进行软注入的DEMUX,如ST平台的SWTS和BCM平台的Playback模式。
    对于DVB双向，其RTSP模块仅获取播放数据所需要的频道信息，由上层负责进行锁频操作
    当然，个人认为将锁频操作放在此中也未尝不可，而且更像普通的tuner。

修改记录:
版本   	日期		修改者		说明
--------	----------	--------	------------
1.0			2007-6-22	Nevill		创建
1.1			2007-7-26	Tongfulei	增加返回值类型:EM_PUMP_NOT_SUPPORT
1.2         2008-10-30  刘正华      添加RTSP交换的枚举类型
*******************************************************************************/

#ifndef CSIPPLAYPUMP_H
#define CSIPPLAYPUMP_H

/******************************************************************************
 *                                 Header File Include                        *
 ******************************************************************************/
#include "mmcp_typedef.h"
#include "cs_player_message.h"
#include "cs_ipplugin.h"

/******************************************************************************
 *                                 Macro/Define/Structus                      *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif



#define IPPUMP_DEFAULT_THREAD_PRI   (125)		/*建议优先级*/	
#define IPPUMP_DEFAULT_THREAD_STACK (0x20000) /* 128 * 1024，建议栈大小*/

#define IPUDP_ES_MODE		(0x44)
#define IPTV_MODE		(0x33)
#define IPQAM_VOD		(0x22)


typedef struct _CSIPPUMPDVBInfo_S
{
    int m_nServiceID;
    char *pcMineType;
    DWORD m_dwMaxBitRate;//KHZ
    int m_nHeight;
    int m_nWidth;
    int m_nFramePerSeconf;
    int m_nAspectRatio;
    int m_nStartTime; //ms
    DWORD m_dwDuration;
    int m_nPlayModes;
    int m_nNumTrickSpeed;
    int m_nTrickSpeed;
    int m_nTypeSpecificData;
    int m_nVideoPID;
    int m_nAudioPID;
    int m_nPCRPID;
    int m_nProgramNo;
    int m_nFrequency;
    int m_nModulation;//0 - Not Define,  1 - 16QAM,	2 - 32QAM,	3 - 64QAM, 4 - 128QAM, 5 - 256QAM
    int m_nSymbolRate;
    int m_nAnnex;
    int m_nChannelSpace;
}CSIPPUMPDVBInfo_S;

typedef  enum 
{
	EM_IPPumpStreamTrans_UNKNOWN,          /*未知类型*/
	EM_IPPumpStreamTrans_TCP,            /*IPTV MODE*/
	EM_IPPumpStreamTrans_UDP,	        /*IPQAM MODE*/
}IPPumpStremTransMode_E;

typedef int (*FOnCSIPPUMPHaveMsgToUser)(DWORD nUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);

/******************************************************************************
 *                                 Global Function Declare                    *
 ******************************************************************************/
/******************************************************************************
函数名:     CSIPPUMPCreate

函数描述:
    创建一个Playpump

输入:
    dwThreadPriority:收数据线程的优先级；
    dwThreadStackSize:收数据线程栈的大小。

输出:
    phPlaypump － 生成的Playpump句柄，供以后需要调用本模块其他函数时使用

返回值:
    EM_PUMP_SUCCESS － 成功

其他:线程优先级和栈的大小，建议采用IPPUMP_DEFAULT_THREAD_PRI和IPPUMP_DEFAULT_THREAD_STACK 
*******************************************************************************/

int CSIPPUMPCreate(DWORD			dwThreadPriority,
					 	DWORD			dwThreadStackSize,
					 	HCSHANDLE 		*phPlaypump );


/******************************************************************************
函数名: 	CSIPPUMPDestroy

函数描述:
		销毁一个Playpump

输入:
hPlaypump － 要销毁的playpump句柄

输出:
	无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:

*******************************************************************************/
int CSIPPUMPDestroy(HCSHANDLE hPlaypump);


/******************************************************************************
函数名: 	CSIPPUMPConnect

函数描述:
	连接一个模拟tuner设备(此处的模拟是说明其把网络数据模拟成tuner属性，而不是
	说tuner本身传送的数据是模拟电视信号)

输入:
	hPlaypump － playpump句柄
	hDevice － 与硬件设备有关的句柄，如DEMUX,DECODE等
	nIpMode － 用来指定IP模式
	pszURL － 数据来源URL
	    对于组播: igmp://224.0.0.1:5001
	    对于点播: rtsp://192.168.1.1:644/vod.ts
	    对于dvb双向: 
	nTimeout - 超时时间
	
输出:
    无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:

*******************************************************************************/
int CSIPPUMPConnect(HCSHANDLE hPlaypump, HCSHANDLE hDevice, int nIpMode, char *pszURL, DWORD dwTimeout);


/******************************************************************************
函数名: 	CSIPPUMPDisconnect

函数描述:
	断开一个模拟tuner设备

输入:
	hPlaypump － playpump句柄
	nNICID － 网卡ID
	
输出:
    无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:

*******************************************************************************/
int CSIPPUMPDisconnect(HCSHANDLE hPlaypump, int nNICID);


/******************************************************************************
函数名: 	CSIPPUMPSetCallback

函数描述:
	对当前Playpump设置回调函数

输入:
	hPlaypump － playpump句柄
	fnCallback － 要注册的回调函数
	pUser － 回调要用到的用户数据，调用回调是传回
	
输出:
	无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:

*******************************************************************************/
int CSIPPUMPSetCallback(HCSHANDLE hPlaypump, FOnCSIPPUMPHaveMsgToUser fnCallback, void *pUser);


/******************************************************************************
函数名: 	CSIPPUMPResetCallback

函数描述:
   对当前Playpump删除回调函数

输入:
	hPlaypump － playpump句柄
    fnCallback － 要删除的回调函数
	
输出:
    无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:

*******************************************************************************/
int CSIPPUMPResetCallback(HCSHANDLE hPlaypump);


/******************************************************************************
函数名: 	CSIPPUMPPause

函数描述:
	暂停一个Playpump

输入:
	hPlaypump － playpump句柄
	
输出:
    无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    暂停仅对点播有效
*******************************************************************************/
int CSIPPUMPPause(HCSHANDLE hPlaypump);


/******************************************************************************
函数名: 	CSIPPUMPResume

函数描述:
	恢复一个Playpump的播放

输入:
	hPlaypump － playpump句柄
	
输出:
	无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    恢复播放仅对点播有效

*******************************************************************************/
int CSIPPUMPResume(HCSHANDLE hPlaypump);


/******************************************************************************
函数名: 	CSIPPUMPSetRate

函数描述:
	设置一个Playpump的播放速率

输入:
	hPlaypump － playpump句柄
	nRate － 要设置的播放速度。为了支持慢进且不使用浮点数，输入速率与标准倍率
	符合以下的公式: nRate = RealRate * 1000
	
输出:
	无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    输入速率1000时为1倍速前进，2000为两倍速前进，4000为四倍速，为500表示慢进。
    如果小于0说明为后退。
    该函数为异步函数。
*******************************************************************************/
int CSIPPUMPSetRate(HCSHANDLE hPlaypump, int nRate);


/******************************************************************************
函数名: 	CSIPPUMPSetTime

函数描述:
	设置一个Playpump的播放时间点

输入:
	hPlaypump － playpump句柄
	nTime － 要设置的时间点，单位为毫秒
	
输出:
	无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    该函数为异步函数。
*******************************************************************************/
int CSIPPUMPSetTime(HCSHANDLE hPlaypump, DWORD dwTime);

/******************************************************************************
函数名: 	 CSIPPUMPSetTimeByType

函数描述:
	设置一个Playpump的播放时间点

输入:
	hPlaypump － playpump句柄
	nTime － 要设置的时间点，单位为毫秒
	nType -- 为1时使用NPT时间，其它的使用UTC 时间 
输出:
	无

返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    该函数为异步函数。
*******************************************************************************/
int  CSIPPUMPSetTimeByType(HCSHANDLE hPlaypump, DWORD dwTime,int nType);

/******************************************************************************
函数名: 	CSIPPUMPGetTime

函数描述:
	获取一个Playpump的当前播放时间点

输入:
	hPlaypump － playpump句柄
	
输出:
	pnTime － 当前播放时间，单位秒

返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    该函数为同步函数。
*******************************************************************************/
int CSIPPUMPGetTime(HCSHANDLE hPlaypump, int *pnTime);


/******************************************************************************
函数名: 	CSIPPUMPGetDuration

函数描述:
	获取一个节目的长度

输入:
	hPlaypump － playpump句柄
	
输出:
	pdwDuration － 节目长度，单位秒

返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    该函数为同步函数。
*******************************************************************************/
int CSIPPUMPGetDuration(HCSHANDLE hPlaypump, DWORD *pdwDuration);

/******************************************************************************
函数名: 	CSIPPUMPCreateSendSocket

函数描述:
	创建上传数据所需要的资源

输入:
	hPlaypump － playpump句柄
	pszIPAdress － 目的主机IP地址,如"192.168.95.65"
	wPort － 目的主机接收端口
输出:
	无
返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    该函数为同步函数。
*******************************************************************************/
int CSIPPUMPCreateSendSocket(HCSHANDLE hPlaypump, char *pszIPAdress,WORD wPort,DWORD dwTimeout,int nTcpFlag);
/******************************************************************************
函数名: 	CSIPPUMPDestroySendSocket

函数描述:
	释放上传数据所需要的资源

输入:
	hPlaypump － playpump句柄
输出:
	无
返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    该函数为同步函数。
*******************************************************************************/
int CSIPPUMPDestroySendSocket(HCSHANDLE hPlaypump);
/******************************************************************************
函数名: 	CSIPPUMPSendStream

函数描述:
	上传数据到指定位置

输入:
	hPlaypump － playpump句柄
	pucBuffer － 要上传的数据
	dwLen － 要上传数据的长度
	dwTimeout － 发送超时时间
输出:
	无
返回值:
	EM_PUMP_SUCCESS － 成功

其他:
    该函数为同步函数。
*******************************************************************************/
int CSIPPUMPSendStream(HCSHANDLE hPlaypump,BYTE *pucBuffer,DWORD dwLen ,DWORD dwTimeout);

/******************************************************************************
函数名: 	CSIPPUMPIsSupportTrickMode

函数描述:
	获取当前节目是够支持快进快退

输入:
		hPlaypump：IPPUMP句柄

输出:
	无
返回值:
	EM_PUMP_SUCCESS － 支持

其他:
    该函数为同步函数。
*******************************************************************************/
int CSIPPUMPIsSupportTrickMode(HCSHANDLE hPlaypump);

/******************************************************************************
函数名: 	CSIPPUMPDisableBreakPointToSee

函数描述:
	设置一个Playpump点播插件是否去除去断点续播的功能，默认为FALSE，主要用于台州
	项目中前端系统的容错，调用接口后保证第一次播放时，无论应用传下来的locator起始
	播放时间为多少，中间件向前端发送播放命令的时候都是从头开始播放，并且对该系统中
	的进度条错误问题做容错处理

输入:
	bDisable 默认为FALSE，只有输入为TRUE后表示是点播插件去除断点续播的功能
	
输出:
	无

返回值:
	无
*******************************************************************************/
void CSIPPUMPDisableBreakPointToSee(BOOL bDisable);

#ifdef __cplusplus
}
#endif

#endif  /* CSIPPLAYPUMP_H */

/* End of File */

