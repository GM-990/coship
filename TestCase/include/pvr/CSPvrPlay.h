#ifndef _CS_PVRPLAY_H_
#define _CS_PVRPLAY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "CSPvrStream.h"

#define INVALID_SPEED 10000000
#define INVALID_DEVICE_INDEX (-1)
#define INVALID_SECOND_LEN  (-1)

#define PVR_PAUSE_SPEED 0
#define PVR_NORMAL_SPEED 1000

typedef enum
{
	EM_PLAY_SUCCESS = 0,
	EM_PLAY_FAILED,
	EM_PLAY_NOT_SUPPORT
}EPlayResult;

typedef enum
{
	EM_PLAY_SEEK_FROM_HEAD = 0,
	EM_PLAY_SEEK_FROM_CURRENT,
	EM_PLAY_SEEK_FROM_END
}EPlayPosFlag;

typedef enum
{
	EM_PLAY_FREE = 0,
	EM_PLAY_WORKING,
	EM_PLAY_NOT_SUPPORT_STATUS
}EPlayStatus;

typedef enum
{
	EM_PLAY_FILE = 0,
	EM_PLAY_TSHIFT,
	EM_PLAY_LIVE,
	EM_PLAY_NOT_SUPPORT_TYPE
}EPlayType;

typedef struct
{
	int nVideoIdx;
	int nAudioIdx;	
	int nTtxIdx;
}SPlayInit;

typedef enum
{
	EM_PLAY_START = 0,
	EM_PLAY_STOP,
	EM_PLAY_END_OF_STREAM,/*pEventData, nEventData均无用*/
	EM_PLAY_BEGIN_OF_STREAM,/*pEventData, nEventData均无用*/
/*[[[ add by luo jinhua 2007-07-06*/
	EM_PLAY_POS/*nEventData为当前播放位置,pEventData无效*/
/*]]]*/
}EPlayEvent;

typedef int(*FPlayNotify)(const int nPlayId, const EPlayEvent eEvent, const void *pEventData, const int nEventData, void *pUserData);
typedef void(*CSOnPlayVideoEnable_F)(int nVideoIndex, BOOL bEnable, void* pUserData);
typedef void(*CSOnPlaySoundEnable_F)(int nSoundIndex, BOOL bEnable, void* pUserData);


/*!
功能描述：
	初始化播放设备。

参数：
	SPlayInit
		[in]	可用播放设备信息列表

返回值
	成功初始化返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	pInitList的最后一项的VideoID和AudioID都为INVALID_DEVICE_INDEX,表示列表结束.
*/
EPlayResult	CSPlay_Init(const SPlayInit *pInitList);

/*!
功能描述：
	设置播放设备的解密状态

参数：
	nPlayId
		[in]	播放设备号
	bEncrypt
		[in]  播放是否需要解密

返回值
	成功初始化返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
*/
EPlayResult	CSPlay_SetEncrypt(const int nPlayId, BOOL bEncrypt);

/*!
功能描述：
	启动回放文件。

参数：
	nPlayId
		[in]	播放设备ID
	szFilename
		[in]	播放流文件夹路径
	nSpeed
		[in]	播放的速度
	nSecondPos
		[in]	播放的起始位置
	pPids
		[in]	要播放的PID

返回值
	成功启动播放返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	nSpeed=1000表示正常直播速度。nSecondPos为相对于起始位置的时长，单位s
*/
EPlayResult	CSPlay_StartFile(const int nPlayId, const char *szFilename,  const int nSpeed, const int nSecondPos, const SPvrBasePidData *pPids);
/*!
功能描述：
	启动Tshift播放。

参数：
	nPlayId
		[in]	播放设备ID
	hTShiftFile
		[in]	启动Tshift录制时返回的句柄
	nSpeed
		[in]	播放的速度
	nSecondPos
		[in]	播放的起始位置
	pPids
		[in]	要播放的PID
返回值
	成功启动播放返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	定义有效可播放的起始位置总为0，参数nSecondPos为相对于0的位置。（具体解释见CSPlay_GetCurSecondPos的注释）
*/
EPlayResult	CSPlay_StartTShift(const int nPlayId, const HCSHANDLE hTShiftFile, const int nSpeed, const int nSecondPos, const SPvrBasePidData *pPids);
/*!
功能描述：
	启动实时节目直播。

参数：
	nPlayId
		[in]	播放设备ID
	nDemuxIdx
		[in]	DemuxID
	pPids
		[in]	播放的各个PID

返回值
	成功启动播放返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	无
*/
EPlayResult	CSPlay_StartLive(const int nPlayId, const int nDemuxIdx, const SPvrBasePidData *pPids);
/*!
功能描述：
	停止实时节目直播/回放文件/Tshift播放。

参数：
	nPlayId
		[in]	播放设备ID

返回值
	成功停止播放返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	无
*/
EPlayResult	CSPlay_Stop(const int nPlayId);
/*!
功能描述：
	恢复已暂停的回放文件/Tshift播放。

参数：
	nPlayId
		[in]	播放设备ID

返回值
	成功恢复返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	无
*/
EPlayResult	CSPlay_Resume(const int nPlayId);
/*!
功能描述：
	暂停回放文件/Tshift播放。

参数：
	nPlayId
		[in]	播放设备ID

返回值
	成功暂停播放返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	无
*/
EPlayResult	CSPlay_Pause(const int nPlayId);
/*!
功能描述：
	设置回放文件/Tshift播放的速度。

参数：
	nPlayId
		[in]	播放设备ID
	nSpeed
		[in]	播放速度

返回值
	成功设置播放速度返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	1000表示正常播放速度
*/
EPlayResult	CSPlay_SetSpeed(const int nPlayId, const int nSpeed);
/*!
功能描述：
	获取播放设备的状态。

参数：
	nPlayId
		[in]	播放设备ID

返回值
	返回目前播放设备的状态。

备注：
	无
*/
EPlayStatus	CSPlay_GetStatus(const int nPlayId);
/*!
功能描述：
	修改播放的PID。

参数：
	nPlayId
		[in]	播放设备ID
	pPids
		[in]	PIDs

返回值
	操作成功返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	无
*/
EPlayResult	CSPlay_ModifyPids(const int nPlayId, const SPvrBasePidData *pPids);
/*!
功能描述：
	获取播放个数。

参数：
	nPlayId
		[in]	播放设备ID

返回值
	播放类型

备注：
	无
*/
int CSPlay_GetCount(void);
/*!
功能描述：
	获取播放类型。

参数：
	nPlayId
		[in]	播放设备ID

返回值
	播放类型

备注：
	无
*/
EPlayType	CSPlay_GetType(const int nPlayId);
/*!
功能描述：
	获取播放速度。

参数：
	nPlayId
		[in]	播放设备ID

返回值
	指定播放设备的当前播放速度

备注：
	无
*/
int			CSPlay_GetSpeed(const int nPlayId);
/*!
功能描述：
	设置播放位置。

参数：
	nPlayId
		[in]	播放设备ID
	nSecondPos
		[in]	偏移量
	ePlayPosFlag
		[in]	偏移的相对位置

返回值
	成功设置播放位置返回EM_PLAY_SUCCESS
	否则返回EM_PLAY_FAILED

备注：
	只有回放文件和Tshift播放可以设置播放位置
	Tshift播放的起始位置为有效可播放的起始位置（具体解释见CSPlay_GetCurSecondPos的注释）
*/
EPlayResult	CSPlay_Seek(const int nPlayId, const int nSecondPos, const EPlayPosFlag ePlayPosFlag);
/*!
功能描述：
	获取回放文件/Tshift播放的绝对时长。

参数：
	nPlayId
		[in]	播放设备ID

返回值
	回放文件/Tshift播放的绝对时长。
	失败返回INVALID_SECOND_LEN

备注：
*/
int	CSPlay_GetCurSecondPos(int nPlayId);

/*!*
获取当回放点抵达到文件头和尾之后，与头尾相差多少又可以开始回放。如：
TShift快进播放追上了录制的内容，这时候需要暂停，录制继续进行，当录制的长度
超过当前回放位置CSPlay_GetPlayValve()秒后，可以继续回放
@return 播放的阀值
*/
int CSPlay_GetPlayValve(void);
/*!
功能描述：
	注册播放通知回调。

参数：
	nPlayId
		[in]	播放设备ID
	fnNotify
		[in]	回调函数
	pUserData
		[in]	用户数据

返回值
	无

备注：
	无
*/
void			CSPlay_RegNotify(const int nPlayId, const FPlayNotify fnNotify, void *pUserData);


/*
功能描述:
	设置当前的音频输出类型
参数:
	bAc3Output
		[in] TRUE 表示当前输出类型为AC3输出
		      FALSE 表示当前输出类型为非AC3输出
返回值
	无
备注 :
	无
*/
void CSPlay_SetCurrentAudOutputType(BOOL bAc3Output);

EPlayResult CSPlay_GetCaDevice(const int nPlayId, HCSHANDLE *phOutCaDevice);

/*Flush 播放的音视频解码器以及缓冲区*/
EPlayResult	CSPlay_Flush(const int nPlayId);

void CSPlay_RegVideoEnableCallback(CSOnPlayVideoEnable_F fnOnPlayVideoEnable, void* pOnPlayVideoEnable);


void CSPlay_RegSoundEnableCallback(CSOnPlaySoundEnable_F fnOnPlaySoundEnable, void* pOnPlaySoundEnable);

#ifdef __cplusplus
}
#endif

#endif

