#ifndef _CS_RECORD_H_
#define _CS_RECORD_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "CSPvrBase.h"
#include "CSPvrStream.h"

typedef enum
{
	EM_RECORD_SUCCESS = 0,
	EM_RECORD_FAILED,
	EM_RECORD_NOTWORKING,
	EM_RECORD_SPEED_LIMIT
}ERecordResult;

typedef enum
{
	EM_RECORD_FREE = 0,
	EM_RECORD_WORKING,
	EM_RECORD_NOT_SUPPORT_STATUS
}ERecordStatus;

typedef enum
{
	EM_RECORD_NORMAL = 0,
	EM_RECORD_TSHIFT
}ERecordType;

//add by luojinhua 2006-12-18
#define INVALID_SECOND_LEN  (-1)

typedef enum
{
	EM_RECORD_START = 0,
	EM_RECORD_STOP,
	EM_RECORD_WRAP_AROUND,
	///add by luojinhua 2007-09-19
	EM_RECORD_TSFILE_END,
	EM_RECORD_VOLUME_FULL
}ERecordEvent;

///[[[ add by luo jinhua 2007-09-17
typedef enum
{
	EM_RECORD_PVR = 0,
	EM_RECORD_TS,
	EM_RECORD_SREAM_TYPE_COUNT
}ERecordStreamType;
///]]]

typedef int(*FRecordNotify)(const int nRecId, const ERecordEvent eEvent,const void *pEventData, const int nEventData, void *pUserData);
void CSRecord_RegNotify(const int nRecordId,FRecordNotify fnRecordCallback, void * pUserData);
//add end

/*!
功能描述：
	初始化录制设备。

参数：
	nNormalCount
		[in]	普通录制设备的数目
	nTShiftCount
		[in]	Tshift录制设备的数目

返回值
	成功初始化返回EM_FILE_SUCCESS
	否则返回EM_FILE_FAILED

备注：
	0 — nNormalCount-1 为普通录制的RecID
	nNormalCount — （nNormalCount + nTShiftCount - 1） 为Tshift录制的RecID
*/
ERecordResult	CSRecord_Init(int nNormalCount, int nTShiftCount);

/*!
获取TShift record起始序号，其值为nNormalCount
返回: TShift record起始序号
*/
int CSRecord_GetTShiftRecordFirstIndex(void);

/*!
获取最多可进行多少路TShift录制，其值为nTShiftCount
返回: 最多可进行多少路TShift录制
*/
int CSRecord_GetTShiftRecordCount(void);

/*!
功能描述：
	设置录制的加密状态

参数：
	nRecId
		[in]	录制设备ID
	bEncrypt
		[in]	设置是否进行加密录制
返回值
	成功启动录制返回EM_RECORD_SUCCESS
备注：
	无
*/
ERecordResult CSRecord_SetEncrypt(const int nRecId, BOOL bEncrypt);

/*!
功能描述：
	启动普通录制。

参数：
	nRecId
		[in]	录制设备ID
	nDemuxIdx
		[in]	DemuxID
	pStreamName
		[in]	目标流文件名
	SPvrBasePidData
		[in]	录制的PID

返回值
	成功启动录制返回EM_FILE_SUCCESS
	否则返回EM_FILE_FAILED

备注：
	无
*/
ERecordResult	CSRecord_StartNormal(const int nRecId, const int nDemuxIdx, const char* pStreamName, const SPvrBasePidData *pPids);
///[[[ add by luo jinhua 2007-09-17
ERecordResult CSRecord_StartTs(const int nRecId, const int nDemuxIdx,const char* szFilename, const SPvrBasePidData *psPids);
//ERecordResult CSRecord_StartNormalEx(const int nRecId, const int nDemuxIdx, const char* pStreamName, const SPvrBasePidData *pPids, ERecordStreamType eType); 

///[[[ add by luo jinhua 2007-07-15
ERecordResult	CSRecord_RecordAppendToExistFile(const int nRecId, const int nDemuxIdx, const char* pStreamName, const SPvrBasePidData *pPids);

ERecordResult CSRecord_SwitchTshiftToNomalFile(const int nRecId, const char *szNomalFileName);

ERecordResult CSRecord_CheckTShiftSpace(HCSHANDLE hTShift, BOOL *pbOutIsSpaceEnough);
///]]]

/*!
功能描述：
	启动Tshift录制。

参数：
	nRecId
		[in]	录制设备ID
	nDemuxIdx
		[in]	DemuxID
	pStreamName
		[in]	目标流文件名
	SPvrBasePidData
		[in]	录制的PID

返回值
	成功启动录制返回EM_FILE_SUCCESS
	否则返回EM_FILE_FAILED

备注：
	无
*/
ERecordResult	CSRecord_StartTShift(const int nRecId, const int nDemuxIdx,  const SPvrBasePidData *pPids, HCSHANDLE* phTShiftFile);
/*!
功能描述：
	停止录制。

参数：
	nRecId
		[in]	录制设备ID

返回值
	成功停止录制返回EM_FILE_SUCCESS
	否则返回EM_FILE_FAILED

备注：
	无
*/
ERecordResult	CSRecord_Stop(const int nRecId);
/*!
功能描述：
	暂停录制。

参数：
	nRecId
		[in]	录制设备ID

返回值
	成功暂停录制返回EM_FILE_SUCCESS
	否则返回EM_FILE_FAILED

备注：
	无
*/
ERecordResult	CSRecord_Pause(const int nRecId);
/*!
功能描述：
	恢复已暂停的录制。

参数：
	nRecId
		[in]	录制设备ID

返回值
	成功恢复录制返回EM_FILE_SUCCESS
	否则返回EM_FILE_FAILED

备注：
	无
*/
ERecordResult	CSRecord_Resume(const int nRecId);
/*!
功能描述：
	获取已录制时长。

参数：
	nRecId
		[in]	录制设备ID

返回值
	正确获取返回已录制时长
	否则返回INVALID_SECOND_LEN

备注：
	nRecId录制为非Working状态时,获取失败
	获取已录制时长为有效可播放时长，
	对于Tshift录制，当录制缓存满时，新录制数据会覆盖就录制数据，
	有效可播放时长不变，因而获取的录制长度保持不变。
*/
int CSRecord_GetSecondLength(const int nRecId);
/*!
功能描述：
	获取录制设备状态。

参数：
	nRecId
		[in]	录制设备ID

返回值
	返回录制设备状态

备注：
	无
*/
ERecordStatus	CSRecord_GetStatus(const int nRecId);
/*!
功能描述：
	获取录制类型。

参数：
	nRecId
		[in]	录制设备ID

返回值
	返回录制类型

备注：
	无
*/
ERecordType	CSRecord_GetType(const int nRecId);

/*!
功能描述：
	获取录制的绝对时间

参数：
	nRecId
		[in]	录制设备ID

返回值
	返回录制的绝对时间

备注：
	无
*/
int CSRecord_GetStartTimeInSecond(const int nRecId);


/*!
功能描述：
	设置TShift录制可用的空间大小
参数：
	uPacketSize
		[in]	TShift录制可用的空间大小,188字节为单位
备注:
	无
*/
void CSRecord_SetTShiftRecordPacketSpace(UINT uPacketSize);

/*获取录制的解扰设备*/
ERecordResult CSRecord_GetCaDevice(const int nRecId,HCSHANDLE *phCaDevice);

/*!
功能描述：
	设置TShift录制路径
参数：
	szTshiftPath
		[in]	TShift录制的路径
备注:
	无
*/
void CSRecord_SetTshiftRecordPath(char * szTshiftPath);


#ifdef __cplusplus
}
#endif

#endif
