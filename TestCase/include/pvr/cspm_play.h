/******************************************************************************
FileName:  cspm_play.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:
定义了PVR播放(包括PVR直播,TShift播放,PVR回放)的相关接口
模块简称PVRPLAY

修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create

*******************************************************************************/
#ifndef CSPM_PLAY_H
#define CSPM_PLAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "cspm_notifymgr.h"

/*!无效播放位置*/
#define  CSPVRPLAY_INVALID_TIME 0xffffffff
/*!无效的播放速度*/
#define CSPVRPLAY_INVALID_SPEED 0xffffffff
/*!正常播放速度*/
#define CSPVRPLAY_NORMAL_SPEED 1000

/*!节目Pid参数*/
typedef struct _CSPVRPLAYChnlParam_S
{
	/*!视频pid*/
	WORD 	wVideoPid;
	/*!视频类型，使用porting的cs_sysvideo.h中的CSVIDStreamType定义*/
	BYTE   	ucVideoType;
	/*!音频pid*/
	WORD	wAudioPid;
	/*!音频类型，使用porting的cs_sysaudio.h中的CSAUDStreamType定义*/
	BYTE   	ucAudioType;
	/*!pcr pid*/
	WORD	wPcrPid;	
	/*!teletext pid*/
	WORD	wTeletextPid;	
	/*!closed caption pid*/
	WORD	wClosedCaptionPid;
	/*!subtitile pid*/
	WORD 	wSubtitlePid;
}CSPVRPLAYChnlParam_S;

/*!播放类型*/
typedef enum _CSPVRPLAYType_E
{
	/*!实时流直播*/
	EM_PVRPLAY_PLAYTYPE_LIVE=0,
	/*!PVR文件回放*/
	EM_PVRPLAY_PLAYTYPE_FILE,	
	/*!TShift播放*/
	EM_PVRPLAY_PLAYTYPE_TSHIFT,
	/*!未知类型*/
	EM_PVRPLAY_PLAYTYPE_UNKOWN
}CSPVRPLAYType_E;

/*!
	获取系统支持的播放句柄个数
\return 系统支持的播放句柄个数(pvr系统初始化时用户设置的个数)
*/
DWORD CSPVRPLAYGetSupportPlayCount(void);
/*!
	获取播放句柄
@param[in] nIdx 播放句柄的索引
\return 成功获取时返回播放句柄,否则返回NULL
*/
HCSHANDLE CSPVRPLAYGetPlayHandle(int nIdx);

/*!
	初始化节目pid参数
@param[out] psParam 输出初始化的录制启动参数
\note 请先调用本函数初始化CSPVRPLAYChnlParam_S结构体,
\note 再对感兴趣的域进行赋值,再调用CSPVRPLAYStartLive启动直播
*/
void CSPVRPLAYGetDefaultChnlParam(CSPVRPLAYChnlParam_S *psParam);

/*!
	启动实时流直播
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[in]  hPath 播放使用设备的路径
@param[in]  szDvbLocator 播放dvb 节目的locator
\return 成功返回TRUE,否则返回FALSE
\note 如果系统配置TShift启动模式是EM_PVRPLAY_AWAYS, 则启动直播时,会自动启动TShift录制
\note 该函数内启动播放时异步启动的
\note 在播放过程中,请勿删除hPath
*/
BOOL CSPVRPLAYStartLive(HCSHANDLE hPVRPlayHandle, HCSHANDLE hPath, const char* szDvbLocator);

/*!
	启动PVR文件回放
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[in]  hPath 播放使用设备的路径
@param[in]  hPvrFile 调用CSPVRFOpen得到的Pvr文件句柄
\return 成功返回TRUE,否则返回FALSE
\note 函数内启动播放是异步的
\note 在播放过程中,请勿删除hPath或者hPvrStream
*/
BOOL CSPVRPLAYStartFile(HCSHANDLE hPVRPlayHandle,HCSHANDLE hPath,HCSHANDLE hPvrStream);

/*!
	获取正在播放节目的pid
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[out] psChnlPid  频道pid信息
\return 成功返回TRUE,否则返回FALSE
\note 
*/
BOOL CSPVRPLAYGetChnlPid(HCSHANDLE hPVRPlayHandle, CSPVRPLAYChnlParam_S *psChnlPid);

/*!
	修改正在播放节目的pid
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[out] psChnlPid  频道新pid信息
\return 成功返回TRUE,否则返回FALSE
\note 同步调用,立即生效
*/
BOOL CSPVRPLAYModifyPid(HCSHANDLE hPVRPlayHandle,const CSPVRPLAYChnlParam_S *psChnlPid);

/*!
	暂停播放(对TShift播放,直播,文件回放均适用)
@param[in]  hPVRPlayHandle Pvr播放句柄
\return 成功返回TRUE,否则返回FALSE
\note 如果系统配置TShift启动模式是EM_PVRPLAY_START_WHEN_PAUSE，则直播暂停时会自动启动TShift录制
\note 如果系统配置TShift启动模式不是EM_PVRPLAY_NEVER_START，则直播被暂停时，会自动切换为TShift播放
\note 同步调用,立即生效
*/
BOOL CSPVRPLAYPause(HCSHANDLE hPVRPlayHandle);

/*!
	恢复播放(对TShift播放,直播,文件回放适用)
@param[in]  hPVRPlayHandle Pvr播放句柄
\return 成功返回TRUE,否则返回FALSE
\note 同步调用,立即生效
\note 当前不管以什么速度播放，Resume成功后均变为常速播放
*/
BOOL CSPVRPLAYResume(HCSHANDLE hPVRPlayHandle);
/*!
	停止播放(对TShift播放,直播,文件回放均适用)
@param[in]  hPVRPlayHandle Pvr播放句柄
\return 成功返回TRUE,否则返回FALSE
\note 同步调用,立即生效
\note 停止播放时,请勿忘删除hPath(Pvrmgr模块内不负责删除hPath)
\note 如果是停止文件回放,请勿忘删除hPvrStream(PvrMgr模块内不负责删除PvrStream)
*/
BOOL CSPVRPLAYStop(HCSHANDLE hPVRPlayHandle);

/*!
	重新播放(对TShift播放,直播,文件回放均适用)
@param[in]  hPVRPlayHandle Pvr播放句柄
\return 成功返回TRUE,否则返回FALSE
\note 同步调用,立即生效
\note 对于TShift播放，Restart后将自动切换为直播									
*/
BOOL CSPVRPLAYRestart(HCSHANDLE hPVRPlayHandle);
/*!
	设置播放位置(对TShift播放,文件回放适用)
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[in] dwTimeSec 需设置的播放位置(基准位置为开始位置,单位为秒)
\return 成功返回TRUE,否则返回FALSE
\note 直播时无法设置播放位置
\note 同步调用,立即生效
\note 设置播放位置成功后，播放速度被置为常速
*/
BOOL CSPVRPLAYSeek(HCSHANDLE hPVRPlayHandle,DWORD dwTimeSec);

/*!
	获取当前播放位置(对TShift播放,文件回放适用)
@param[in]  hPVRPlayHandle Pvr播放句柄
\return  获取成功返回当前播放位置(单位为秒), 失败时返回CSPVRPLAY_INVALID_TIME
*/
DWORD CSPVRPLAYGetCurTimeSec(HCSHANDLE hPVRPlayHandle);

/*!
	设置播放速度(对TShift播放,直播,文件回放均适用)
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[in]  nSpeed 需设置的播放速度
return 是否成功设置播放速度
\note CSPVRPLAY_NORMAL_SPEED为常速播放,
\note 支持的快退播放速度有: (-16, -8,-6,-4,-2,-1)*CSPVRPLAY_NORMAL_SPEED
\note 支持的慢退播放速度有: (-1/8, -1/4,-1/2)*CSPVRPLAY_NORMAL_SPEED
\note 支持的慢进播放速度有: (1/8,  1/4, 1/2)*CSPVRPLAY_NORMAL_SPEED
\note 支持的快进播放速度有: (2,4,6,8,16,32)*CSPVRPLAY_NORMAL_SPEED
\note 直播时,如果系统配置TShift启动模式是EM_PVRPLAY_AWAYS,可设置小于CSPVRPLAY_NORMAL_SPEED的速度.
\npte 直播时设置速度成功后,会自动切换为TShift播放
*/
BOOL CSPVRPLAYSetSpeed(HCSHANDLE hPVRPlayHandle, int nSpeed);

/*!
	获取当前播放速度
@param[in]  hPVRPlayHandle Pvr播放句柄
\return 获取成功返回当前播放速度，失败时返回CSPVRPLAY_INVALID_SPEED
\note 返回的速度是以CSPVRPLAY_NORMAL_SPEED为基准
\note 返回负数表示正在回退
\note 对TShift播放,直播,文件回放均适用
*/
int CSPVRPLAYGetSpeed(HCSHANDLE hPVRPlayHandle);

/*!
	获取当前播放类型
@param[in]  hPVRPlayHandle Pvr播放句柄
\return 当前播放类型
\note 对TShift播放,直播,文件回放均适用
*/
CSPVRPLAYType_E CSPVRPLAYGetPlayType(HCSHANDLE hPVRPlayHandle);

/*!
	获取正在回放的PVR文件流
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[out]  phOutPvrStream 正在回放的PVR文件流句柄
\return 成功返回TRUE
\note 仅当hPVRPlayHandle正在播放类型为PVR文件回放时，本接口适用。
*/
BOOL  CSPVRPLAYGetPlayingStream(HCSHANDLE hPVRPlayHandle,HCSHANDLE* phOutPvrStream);

/*!
	获取当前可播放长度
@param[in]  hPVRPlayHandle Pvr播放句柄
\return 可播放长度,单位为秒
\note 如果当前播放是直播或者未启动播放，则返回CSPVRPLAY_INVALID_TIME
\note 对TShift播放,文件回放适用
*/
DWORD CSPVRPLAYGetDurationSec(HCSHANDLE hPVRPlayHandle);

/*! 播放的通知类型定义*/
typedef enum _CSPVRPLAYNotifyType_E
{
	/*!
		直播与TShift播放切换通知
		\note	dwNotifyType:	EM_PVRPLAY_TSHIFT_SWITCH 
		\note	pNotifyData:	切换后的播放类型，见CSPVRPLAYType_E的定义
		\note	hSender: 		hPvrPlayHandle播放句柄
	*/
	EM_PVRPLAY_TSHIFT_SWITCH = 0,
	/*!
		播放速度变化通知
		\note	dwNotifyType:	EM_PVRPLAY_SPEED_CHANGE 
		\note	pNotifyData:	变化后的播放速度
		\note	hSender: 		hPvrPlayHandle播放句柄
	*/
	EM_PVRPLAY_SPEED_CHANGE,
	/*!
		播放到文件末尾通知
		\note	dwNotifyType:	EM_PVRPLAY_END_OF_FILE 
		\note	pNotifyData:	未定义
		\note	hSender: 		hPvrPlayHandle播放句柄
	*/
	EM_PVRPLAY_END_OF_FILE,	
	/*!
		播放到文件开头通知
		\note	dwNotifyType:	EM_PVRPLAY_BEGIN_OF_FILE 
		\note	pNotifyData:	未定义
		\note	hSender: 		hPvrPlayHandle播放句柄
	*/
	EM_PVRPLAY_BEGIN_OF_FILE,	
	/*!
		播放模块通知种类的个数
	*/
	EM_PVRPLAY_NOTIFY_TYPE_COUNT
}CSPVRPLAYNotifyType_E;

/*!
	获取消息通知管理器的句柄
\return 消息通知管理器的句柄
*/
HCSHANDLE CSPVRPLAYGetNotifyMgr(void);

/*!
	设置PVR PLAY是否支持Teletext 
@param[in]  bSupport: PVR PLAY是否支持Teletext
\note 若支持，播放时需要把Teletext的PID 通过CSUDIPLAYERSetStream设置给驱动
\note VBI Teletext 需要通过CSUDIPLAYERSetStream把PID设置给驱动，OSD Teletext不需要
*/
void CSPVRPLAYSetSupportTtxCc(BOOL bSupport);

/*!
	获取PVR PLAY是否支持Teletext 
\return 支持返回TRUE, 否则返回FALSE
*/
BOOL CSPVRPLAYGetSupportTtxCc();

/*!
	设置PVR PLAY是否支持Subtitle
@param[in]  bSupport: PVR PLAY是否支持Subtitle
\note 若支持，播放时需要把Subtitle的PID 通过CSUDIPLAYERSetStream设置给驱动
*/
void CSPVRPLAYSetSupportSubtitle(BOOL bSupport);

/*!
	设置PVR PLAY是否支持Subtitle 
\return 支持返回TRUE, 否则返回FALSE
*/
BOOL CSPVRPLAYGetSupportSubtitle();



#ifdef MMCP_UDI2
/*!
	将MP通过CSUDIPLAYEROpen的播放handle传递给pvr的udi2toudi1的adapter层，用于启动各种播放
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[out] hPlayer  MP的Open的播放handle
\return 
\note 同步调用,立即生效
*/
void CSPVRPLAYSetPlayerHandle(int hPVRPlayHandle, CSUDI_HANDLE hPlayer);

/*!
	将Pvr播放句柄对应的时移播放句柄返回给MP
@param[in]  hPVRPlayHandle Pvr播放句柄
@param[out] MP的播放handle
\return 成功返回合法的时移播放句柄，失败返回CSUDI_NULL
\note 同步调用,立即生效
*/
CSUDI_HANDLE CSPVRPLAYGetTShiftPlayerHandle(int hPVRPlayHandle);
#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

