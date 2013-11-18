/*
	关于这个文件的详细说明，请参见函数接口文档
*/

#ifndef ServiceManagerInterface_H
#define ServiceManagerInterface_H

#include "mmcp_typedef.h"
#include "SICommonType.h"

#ifdef __cplusplus
extern "C" 
{
#endif


/*
 *	启动选项定义，用于外部接口
 */
#define SM_EPG						0x01		//启动EPG
#define SM_UPDATE					0x02		//启动自动更新
#define SM_TELETEXT					0x04		//启动Teletext
#define SM_SUBTITLING				0x08		//启动subtitling
#define SM_CA						0x10		//启动CA
#define SM_PG_ANY					0x20		//启动PG侦听(响应所有的PG信息)
#define SM_PG_ONCE					0x40		//启动PG侦听(响应第一次PG信息)
#define SM_SERVICE_LOCK				0x80		//启动节目加锁侦听

#define SM_START_PAT_FOR_PMT        0x80000     //换台的时候先搜索PAT，解析PMT的PID

#define SM_ONLY_MONITOR_TABLE       0x40000     //播放时只启动PAT、BAT、SDT、NIT、CAT
												//表侦听，但不启动解码器，表侦听策略保
												//持不变，如下所列

#define SM_BAT_ONLY_MONITOR			0x1000		//频点切换仅启动BAT侦听 
#define SM_NIT_ONLY_MONITOR			0x400		//频点切换仅启动NIT侦听
#define SM_SDT_ONLY_MONITOR			0x800		//频点切换仅启动SDT侦听

#define SM_CAT_SEARCH_AND_MONITOR	0x200		//频点切换启动CAT搜索然后启动侦听
#define SM_BAT_SEARCH_AND_MONITOR	0x100		//频点切换启动BAT搜索然后启动侦听
#define SM_NIT_SEARCH_AND_MONITOR	0x2000		//频点切换启动NIT搜索然后启动侦听
#define SM_SDT_SEARCH_AND_MONITOR	0x4000		//频点切换启动SDT搜索然后启动侦听

#define SM_SDT_SEARCH_EVERY_SERVICE 0x8000		//每次换台都启动一次ActSDT搜索然后侦听
#define SM_PAT_ONLY_MONITOR			0x10000


//以下宏定义为了与以前的接口兼容而做出保留，
//所有下述的选项均有新的定义形式
#define SM_BAT_SENSITIVE			0x100		//频点切换启动BAT搜索然后启动侦听
#define SM_CAT						0x200		//频点切换启动CAT搜索然后启动侦听
#define SM_CAT_SENSITIVE			0x200		//频点切换启动CAT搜索然后启动侦听
#define SM_NIT_SENSITIVE			0x400		//频点切换仅启动NIT侦听
#define SM_SDT_SENSITIVE			0x800		//频点切换仅启动SDT侦听

/*
 *	表搜索完成策略宏定义，用于外部接口
 */
#define USPSM_COMPLETE_SECTION_NUM 1			  //根据SectionNum判断Section是否搜全
#define USPSM_COMPLETE_RECYCLE_RET 2			  //根据包重复判断Section是否搜全

typedef enum _ServiceCallbackMessageType
{
  USP_SMR_SERVICE_LOCK
  , USP_SMR_SERVICE_NOLOCK
  , USP_SMR_NO_SIGNAL
  , USP_SMR_SIGNAL_CONNECTED
  , USP_SMR_SIGNAL_LOST
  , USP_SMR_PG_MESSAGE
  , USP_SMR_NO_PG_MESSAGE
  , USP_SMR_PMT_VER_CHANGED
  , USP_SMR_SDT_VER_CHANGED
  , USP_SMR_NIT_VER_CHANGED
  , USP_SMR_PMT_COMPLETE
  , USP_SMR_CAT_COMPLETE
  , USP_SMR_DM_CHANGE
  , USP_SMR_SIGNAL_SEARCHING
  , USP_SMR_NIT_COMPLETE
  , USP_SMR_SDT_COMPLETE
  , USP_SMR_BAT_COMPLETE
  , USP_SMR_RUNNING_STATUS
  , USP_SMR_STARTPLAY_DONE
  , USP_SMR_STOPPLAY_DONE
  , USP_SMR_SOURCE_CHANGE
  , USP_SMR_START_LOCK_SOURCE
  , USP_SMR_PAT_DATA_AVAIL
  , USP_SMR_PAT_DATA_COMPLETE
  , USP_SMR_CANNOTGET_PMT//当解析PAT表后找到PMT相应的pid，请求pmt还是超时
  , USP_SMR_NO_CUR_SERVICEID//PMT表更新后删除了当前节目的service_id
  , USP_SMR_GET_AVAIL_APID//PMT表中有合法的音频pid
  , USP_SMR_GET_INVALI_APID//PMT表中没有合法的音频pid
  , USP_SMR_GET_AVAIL_VPID//PMT表中有合法的视频pid
  , USP_SMR_GET_INVALI_VPID//PMT表中没有合法的视频pid
  , USP_SMR_PMT_READY
  , USP_SMR_DISABLEAV_FOR_PG
  , USP_SMR_PMT_PID_CHANGE
}ServiceCallbackMessageType;

typedef struct 
{
	unsigned int unAudioPID			:13;	//audio pid
	unsigned int unAudioSMType		:3;		//1: 13818-3audio or 11172audio; 2:AC3Audio
	unsigned int unAudioType		:8;		//audio type: elementary stream type: see 13818-1 Table 2-36
	unsigned int unCanUsedFlag		:1;		//1: audio pid can be used; 0: audio pid can't be used
	unsigned int unLanguageIndex	:7;		//请参见上述SM支持的语言定义
}CSUSPSMAudioInfo;		//用于多语言支持

typedef struct _SMData		//SM返还给用户的一段缓冲区
{
	int m_nType;			//缓冲区类型

	BYTE* m_pucBuf;			//缓冲区首地址
	int m_nBufLen;			//缓冲区长度
}SMData;

typedef enum _CSUSPSMServicePlayType
{
	SM_UNKNOW,      //未定义类型
	SM_DVB,		    //普通DVB业务
	SM_RADIO_PAGE	//广播页播放，只改变音频，不改变视频
}CSUSPSMServicePlayType;

typedef struct _PlayDevice
{
	HCSHANDLE hSourceHandle;//播放过程中用于锁频的句柄
	DWORD     dwDemuxID;//播放过程中各种表侦听用的demuxid
}CSUSPSMPlayDevice;

#define  USPSM_MAX_AUDIO_NUM 64		//支持的最大语言数

#define AUDIO_LANG_UNKNOW			0		//unknown language 
#define AUDIO_LANG_ENG              1       //"English"   英语
#define AUDIO_LANG_FIN              2       //"Finnish"   芬兰语  
#define AUDIO_LANG_FRAFRE           3       //"French"    法语 
#define AUDIO_LANG_DEUGER			4       //"German"    德语 
#define AUDIO_LANG_ELLGRE           5		//"Greek"     希腊语  
#define AUDIO_LANG_HUN              6		//"Hungarian" 匈牙利语
#define AUDIO_LANG_ITA              7       //"Italian"	  意大利语
#define AUDIO_LANG_NOR              8		//"Norwegian" 挪威语
#define AUDIO_LANG_POL              9		//"Polish"    波兰语      
#define AUDIO_LANG_POR              10		//"Portuguese" 葡萄牙语
#define AUDIO_LANG_RONRUM           11      //"Romanian"   罗马尼亚语
#define AUDIO_LANG_RUS              12		//"Russian"    俄语
#define AUDIO_LANG_SLV              13		//"Slovenian"  斯洛文尼亚语
#define AUDIO_LANG_ESLSPA           14		//"Spanish"    西班牙语   
#define AUDIO_LANG_SVESWE           15      //"Swedish"    瑞典语
#define AUDIO_LANG_TUR              16		//"Turkish"    土耳其语 
#define AUDIO_LANG_ARA              17		//"Arabic"     阿拉伯语
#define AUDIO_LANG_CHIZHO           18      //"Chinese"    中国语 
#define AUDIO_LANG_CESCZE           19      //"Czech"      捷克语  
#define AUDIO_LANG_DAN              20		//"Danish"     丹麦语 
#define AUDIO_LANG_DUTNLA           21      //"Dutch"      荷兰语

typedef void (*ServiceManagerCallback)(ServiceCallbackMessageType type,void* pParam);

/************************************************************************
                    以下函数用于创建ServiceManager                  
************************************************************************/

//此两个函数用于创建ServiceManager句柄, 启动ServiceManager的主任务;
//对于这个句柄，在应用程序的生命期应该保持存在.
HCSHANDLE CSUSPSMCreateServiceManager( HCSHANDLE hDemux, HCSHANDLE hSource, HCSHANDLE hEpg, HCSHANDLE hDataContainer, ServiceManagerCallback fnCallback, HCSHANDLE hUserData );
HCSHANDLE CSUSPSMCreateServiceManagerEx( HCSHANDLE hDemux, HCSHANDLE hMemoryPool, HCSHANDLE hSource, HCSHANDLE hEpg, HCSHANDLE hDataContainer, ServiceManagerCallback fnCallback, int nStackSize, int nPriority, HCSHANDLE hUserData );

//启动侦听任务，包括CAT，BAT，SDT，NIT
BOOL CSUSPSMStartMonitorThread( HCSHANDLE hServiceManager, DWORD dwOption );
BOOL CSUSPSMStartMonitorThreadEx( HCSHANDLE hServiceManager, int nStackSize, int nPriority, DWORD dwOption );

//设定NIT，BAT侦听时的NetworkID，BouquetID.这两个寒暑应该在启动侦听任务的函数调用之前调用
BOOL CSUSPSMSetNetworkID( HCSHANDLE hServiceManager, WORD wNetworkID, BOOL bNetworkIDMask );
BOOL CSUSPSMSetBouquetID( HCSHANDLE hServiceManager, WORD wBouquetID, BOOL bBouquetIDMask );

//设定是否由SM内部在NIT/PAT/SDT/BAT更新时自动把最新的版本号设定到数据管理模块
//		默认：自动设定，dwStorageType = 0; 当dwStorageType = 1,那么内部不自动设定;其它值无定义
BOOL CSUSPSMSetTableVersionStorageType( HCSHANDLE hServiceManager, DWORD dwStorageType );

// 设定音频输出模式：此参数会被传送到SM调用CSUSPSMSPlayStream中的结构体中
//
//	dwAudioOutputMode各个值得定义请参见csuspsmsupportinterface.h中关于audiooutputmode的定义：
//		#define SMS_AUIDO_OUTPUTMODE_UNKNOWN		0
//		#define SMS_AUIDO_OUTPUTMODE_PCM			1
//		#define SMS_AUIDO_OUTPUTMODE_DIGITAL		2
BOOL CSUSPSMSetAudioOutputMode( HCSHANDLE hServiceManager, DWORD dwAudioOutputMode );

//	设定换台的音视频切换类型：传统，黑屏，静桢，淡入淡出；
//		如果用户不调用这个函数或者输入参数为0,则为传统模式;否则为指定模式,需要平台的相应版本支持
//
//	dwServiceSwitchType的值请参见csuspsmsupportinterface.h中关于换台时音视频的处理类型的定义：
//	#define SMS_SER_SWITCH_TYPE_UNKNOWN				0	//换台时音视频的处理类型：未定义
//	#define SMS_SER_SWITCH_TYPE_SCREEN_BLACK		1	//换台时音视频的处理类型：黑屏
//	#define SMS_SER_SWITCH_TYPE_SCREEN_IFRAME		2	//换台时音视频的处理类型：静桢
//	#define SMS_SER_SWITCH_TYPE_SCREEN_FADE_IN_OUT	3	//换台时音视频的处理类型：淡入淡出
BOOL CSUSPSMSetServiceSwitchType( HCSHANDLE hServiceManager, DWORD dwServiceSwitchType );

//	设定是否在PMT搜索完成后自动更新音视频的解码器。在默认状态下（也就是这个接口不被调用，或者策略被设定为TRUE），
//	SM内部会自动更新解码器（如果经分析PMT需要更新的话）。如果用户把参数设定为FALSE，那么SM在PMT搜索成功后不再进
//	行解码器的自动更新动作，但是依旧会启动CA，抛出PMT原始数据，启动侦听等动作。
BOOL CSUSPSMSetAutoUpdatePolicyForPMT( HCSHANDLE hServiceManager, BOOL bAuotUpdatePolicyForPMT );

//设定BAT搜索完成判断策略
BOOL CSUSPSMSetBATCompletePolicy( HCSHANDLE hServiceManager, int nBATCompletePolicy );
//设定NIT搜索完成判断策略
BOOL CSUSPSMSetNITCompletePolicy( HCSHANDLE hServiceManager, int nNITCompletePolicy );
//设定Other SDT搜索完成判断策略
BOOL CSUSPSMSetOtherSDTCompletePolicy( HCSHANDLE hServiceManager, int nOtherSDTCompletePolicy );
//设定锁频成功之后，解码器启动之前的SM主任务挂起时间。默认为0，不挂起。ms为单位。
BOOL CSUSPSMSetSleepTimeAtPointA( HCSHANDLE hServiceManager, DWORD dwSleepTime );
//设定SM解码器启动，主要的表启动之后SM主任务的挂起时间。默认为0，不挂起。ms为单位。
BOOL CSUSPSMSetSleepTimeAtPointB( HCSHANDLE hServiceManager, DWORD dwSleepTime );
//这个函数销毁ServiceManager实体，在程序的生命期不能被调用
BOOL CSUSPSMDeleteServiceManager( HCSHANDLE hServiceManager );
//在创建SM后调用，支持多实例，传入SM句柄和Tuner类型
BOOL CSUSPSMSetSourceHandle( HCSHANDLE hServiceManager, HCSHANDLE hSource );
//设置BAT搜全次数
BOOL CSUSPSMSetBATSectionCompleteRepeatTimes( HCSHANDLE hServiceManager, int nRepeatTimes );

/************************************************************************
	 以下函数用于播放，停止播放一个节目；打开与关闭音视频输出等 
************************************************************************/

//播放一个节目
BOOL CSUSPSMPlayService( HCSHANDLE hServiceManager, HCSHANDLE hService, DWORD dwOption );

//停止播放正在播放的节目
BOOL CSUSPSMStopPlay( HCSHANDLE hServiceManager );
//停止播放正在播放的节目-同步形式
BOOL CSUSPSMStopPlaySync( HCSHANDLE hServiceManager );

//启动与停止音视频输出
BOOL CSUSPSMEnableAV( HCSHANDLE hServiceManager );
BOOL CSUSPSMDisableAV( HCSHANDLE hServiceManager );
BOOL CSUSPSMDisableAudio( HCSHANDLE hServiceManager );
BOOL CSUSPSMDisableVideo( HCSHANDLE hServiceManager );
BOOL CSUSPSMEnableAudio( HCSHANDLE hServiceManager );
BOOL CSUSPSMEnableVideo( HCSHANDLE hServiceManager );

//停止所有音频输出，打开所有音频输出（详细见接口文档）
BOOL CSUSPSMMute( HCSHANDLE hServiceManager );
BOOL CSUSPSMUnMute( HCSHANDLE hServiceManager );

//解除节目锁，PG锁
BOOL CSUSPSMUnLockService( HCSHANDLE hServiceManager );
BOOL CSUSPSMUnPGService( HCSHANDLE hServiceManager );

//获取正在播放节目的句柄
HCSHANDLE CSUSPSMGetCurPlayingServiceHandle( HCSHANDLE hServiceManager );

//将PMT的请求放在解码器之前
BOOL CSUSPSMEarlyRequestPMT( HCSHANDLE hServiceManager, BOOL bEarlyRequest );

//根据节目的类型播放节目
BOOL CSUSPSMPlayServiceByType( HCSHANDLE hServiceManager, HCSHANDLE hService, CSUSPSMServicePlayType enmServiceType, DWORD dwOption );

/************************************************************************
		以下函数用于对正在播放的节目进行处理，						
		如改变音频，获取Teletext等                               
************************************************************************/

// 以下函数只能在PMT搜索完成后调用

// 得到当前节目的PMT原始数据（这两个函数获取存储在SM中的PMT原始数据的首地址和长度，请千万不要更改地址中的内容）
const BYTE * CSUSPSMGetPMTDataBuf( HCSHANDLE hServiceManager );
int CSUSPSMGetPMTDataBufLen( HCSHANDLE hServiceManager );

//  改变节目的音频PID（第三个参数目前保留，第四个参数为音频语言，如果用户设定为NULL，则SM不更新音频语言类型）
BOOL CSUSPSMChangeAudio( HCSHANDLE hServiceManager, DWORD dwAudioPID, int nType, const char * pcLanguageCode );
//  功能同上，但是最后一个参数是音频语言对应的Index，具体见本文件上面的宏定义
BOOL CSUSPSMChangeAudioEx( HCSHANDLE hServiceManager, DWORD dwAudioPID, int nType, int nLanguageIndex );

//	获取节目的不同国家语言属性。
//	nIndex: 输入参数。输入不同的nIndex获取到不同国家语言的音频属性。nIndex的值由 AUDIO_LANG_...宏确定。//			
//	pAudioInfo: 输出参数。存放对应的PID值
BOOL CSUSPSMGetAudioInfo( HCSHANDLE hServiceManager, int nIndex, CSUSPSMAudioInfo * pAudioInfo );

//获取正在播放节目的第一个AudioPID（不包含AC3）
HCSHANDLE CSUSPSMGetFirstAudioPID( HCSHANDLE hServiceManager, WORD * pwAudioPID, BYTE * pucStreamType, int * pnLanguageIndex );
//获取正在播放节目的下一个AudioPID（不包含AC3）
HCSHANDLE CSUSPSMGetNextAudioPID( HCSHANDLE hServiceManager, HCSHANDLE hPre, WORD * pwAudioPID, BYTE * pucStreamType, int * pnLanguageIndex );

//获取正在播放节目的Audio总个数（不包含AC3）
int CSUSPSMGetAllAudioCount( HCSHANDLE hServiceManager );
//获取正在播放节目的指定第nIndex个Audio（不包含AC3）
BOOL CSUSPSMFindIndexAudioPID( HCSHANDLE hServiceManager, int nIndex, WORD * pwAudioPID, BYTE * pucStreamType, int * pnLanguageIndex );

//包含所有已知的Audio ElemType，例如，03 ，04 的Audio Stream，AC3 Audio
int CSUSPSMGetAllTypeAudioCount( HCSHANDLE hServiceManager );
//包含所有已知的Audio ElemType，例如，03 ，04 的Audio Stream，AC3 Audio
BOOL CSUSPSMFindIndexTypeAudioPID( HCSHANDLE hServiceManager, int nIndex, CSUSPSMAudioInfo * pstAudioInfo );

//设置是否限定CAT、PMT表的抛出次序，bCATPMTSeqSensitive设为TRUE先抛出CAT再抛出PMT，设为FALSE则没有次序限定
BOOL CSUSPSMSetCATPMTSeqSensitive( HCSHANDLE hServiceManager, BOOL bCATPMTSeqSensitive );
//返回CAT、PMT表的抛出次序设定
BOOL CSUSPSMGetCATPMTSeqSensitive( HCSHANDLE hServiceManager );

//获取正在播放节目的AC3 PID。当没有AC3 PID时，返回 0。
WORD CSUSPSMGetAC3PID( HCSHANDLE hServiceManager );
//设定正在播放节目的AC3 PID。这个函数启动AC3解码器
BOOL CSUSPSMSetAC3Decoder( HCSHANDLE hServiceManager, WORD wAC3PID );

//获取正在播放节目的第一个Teletest
HCSHANDLE CSUSPSMGetFirstTeletext( HCSHANDLE hServiceManager, SITeletext * pSITeletext );
//获取正在播放节目的下一个Teletest
HCSHANDLE CSUSPSMGetNextTeletext( HCSHANDLE hServiceManager, HCSHANDLE hPre, SITeletext * pSITeletext );
//获取正在播放节目的第一个Subtitle
HCSHANDLE CSUSPSMGetFirstSubtitle( HCSHANDLE hServiceManager, SISubtitle * pSISubtile );
//获取正在播放节目的下一个Subtitle
HCSHANDLE CSUSPSMGetNextSubtitle( HCSHANDLE hServiceManager, HCSHANDLE hPre, SISubtitle * pSISubtile );

//注册EPG句柄。用于在SM运行的中途删除EPG后重新注册EPG句柄
BOOL CSUSPSMRegesterEPG( HCSHANDLE hServiceManager, HCSHANDLE hEPG );
//注销EPG句柄。用于在用户删除EPG句柄之前，把SM中保存的EPG句柄注销
BOOL CSUSPSMUnRegesterEPG( HCSHANDLE hServiceManager );

// 获取ServiceManager的版本号
int CSUSPSMGetVersionInfo( char * pcVersionInfo, int nSize );

BOOL CSUSPSMSetBATUserBouquetIDFilter( HCSHANDLE hServiceManager, BOOL bUserSetBouquetIDSwitch, WORD ucBouquetArray[], int nArrayLen );
//定时请求PMT表，设置定时时间，dwInterval的单位为ms
BOOL CSUSPSMSetRequestPMTInterval( HCSHANDLE hServiceManager, DWORD dwInterval, BOOL bTimeRequestPMT );
//设置CA和PG全锁状态的优先级，如果bCAPriority设为True，则在PG全锁状态启动CA，有CA消息抛出
//默认为FALSE
BOOL CSUSPSMSetCAPriorToPG( HCSHANDLE hServiceManager, BOOL bCAPriority );
/******************************************************************************
Function Name:    CSUSPSMRegesterAudioType

Description:向SM注册音频类型

Input:
hServiceManager: SM句柄
pucAudioStreamTypes : 注册的音频StreamType（如0x03），为长度不超过16个的数组首地址
pucAudioSMSTypes : 	音频StreamType所对应的平台驱动定义（在文件CSUSPSMSupportedInterface.h中）的音频类型（如SMS_AUIDO_TYPE_NORMAL或SMS_AUIDO_TYPE_MPEG1等等），
为长度不超过16个的数组首地址，其中的元素与pucAudioStreamTypes的元素要一一对应（与pucAudioStreamTypes的长度须相等）
wMapCount :	参数2 

Output:

Return Value: 注册成功TRUE;
注册失败FALSE;

Remark:如果pucAudioStreamTypes中的元素的值有重复的，返回FALSE
注册进来的streamtype不能为0，否则返回FALSE，应包含SM默认的类型如下所示；
如果返回FALSE，则使用SM内部默认的streamtype，它们是：
0x03（SMS_AUIDO_TYPE_NORMAL或SMS_AUIDO_TYPE_MPEG1）
0x04（SMS_AUIDO_TYPE_NORMAL或SMS_AUIDO_TYPE_MPEG2）
0x81（SMS_AUIDO_TYPE_AC3）
*******************************************************************************/
BOOL CSUSPSMRegesterAudioType( HCSHANDLE hServiceManager, BYTE * pucAudioStreamType, BYTE * pucAudioSMSType, WORD wMapCount );
/******************************************************************************
Function Name:    CSUSPSMRegesterVideoType

Description:向SM注册视频类型

Input:
hServiceManager: SM句柄
pucAudioStreamTypes : 注册的视频StreamType（如0x01），为长度不超过16个的数组首地址
pucAudioSMSTypes : 	视频StreamType所对应的平台驱动定义（在文件CSUSPSMSupportedInterface.h中）的音频类型（如SMS_VIDEO_TYPE_MPEG2等等），
为长度不超过16个的数组首地址，其中的元素与pucVideoStreamTypes的元素要一一对应（与pucVideoStreamTypes的长度也须相等）
wMapCount : 需要注册进SM实际的视频类型个数 

Output:

Return Value: 注册成功TRUE;
注册失败FALSE;

Remark:	1、如果pucVideoStreamTypes中的元素的值有重复的，返回FALSE
2、注册进来的streamtype不能为0，否则返回FALSE，应包含SM默认的类型如下所示；
3、如果返回FALSE，则使用SM内部默认的streamtype，它们是：
0x01（SMS_AUIDO_TYPE_MPEG2）
0x02（SMS_AUIDO_TYPE_MPEG2）
*******************************************************************************/
BOOL CSUSPSMRegesterVideoType( HCSHANDLE hServiceManager, BYTE * pucVideoStreamType, BYTE * pucVideoSMSType, WORD wMapCount );

BOOL CSUSPSMPlayByDevice( HCSHANDLE hServiceManager, HCSHANDLE hService, CSUSPSMServicePlayType enmServiceType, CSUSPSMPlayDevice sPlayDevice, DWORD dwOption );
#ifdef __cplusplus
}
#endif


#endif /* ServiceManagerInterface_H */
