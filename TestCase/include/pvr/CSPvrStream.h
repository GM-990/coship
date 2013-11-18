#ifndef _CS_PVRSTREAM_H_
#define _CS_PVRSTREAM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "CSPvrBase.h"

#define MAX_FILE_NAME_LEN (256)

typedef enum
{
	EM_PVRSTREAM_SUCCESS = 0,
	EM_PVRSTREAM_FAILED,
	EM_PVRSTREM_NOT_SUPPORTED
}EPvrStreamResult;

typedef struct 
{
	char szStreamName[MAX_FILE_NAME_LEN];
	SPvrBasePidData sPids;	
	int nSecondLength;
}SPvrStreamInfo;

typedef struct
{
	UINT uStartTimeSecond;
	UINT uEndTimeSecond;
}SPvrStreamCutMark;


/*!
功能描述：
	获取普通录制的流文件信息。

参数：
	pOrgStreamName
		[in]	流文件夹路径
	pInfo
		[out]	流文件信息

返回值
	成功获取返回EM_PVRSTREAM_SUCCESS
	否则返回EM_PVRSTREAM_FAILED

备注：
	无
*/
EPvrStreamResult	PvrStream_GetNormalInfo(const char *pPvrStreamName, SPvrStreamInfo *pInfo);
/*!
功能描述：
	获取普通录制的流文件信息。

参数：
	pOrgStreamName
		[in]	流文件夹路径
	pInfo
		[out]	流文件信息

返回值
	成功获取返回EM_PVRSTREAM_SUCCESS
	否则返回EM_PVRSTREAM_FAILED

备注：
	无
*/
EPvrStreamResult	PvrStream_GetTShiftInfo( HCSHANDLE hTShiftFile, SPvrStreamInfo *pInfo);

/**
[[[ add by luo jinhua 2007-07-25
功能描述：
	剪辑文件，将文件的开头部分剪辑除掉（删除0 -- nCropSecondPos的内容）
参数：
	szPvrStreamName
		[in]	流文件夹路径
	nCropSecondPos
		[in]	需要剪切掉的头部长度

返回值
	成功返回EM_PVRSTREAM_SUCCESS
	否则返回EM_PVRSTREAM_FAILED

备注：
	无
*/
EPvrStreamResult PvrStream_CropStreamHead(const char *szPvrStreamName, int nCropSecondPos);
/*]]]*/

/**
[[[ add by luo jinhua 2007-07-25
功能描述：
	PVR录制流的剪切功能
参数：
	szStreamName
		[in]	需要被剪切的录制流
	pMarks
		[in]	剪切的起、止时间段数组
	nMarksCount
		[in] 剪切的时间段数量
	pOutCutLen
		[out] 录制流剪切掉的总长度
	pPlayPosInSec
		[in/out] 输入为当前播放的位置；
				输出为剪切后当前的播放位置				

返回值
	成功返回EM_PVRSTREAM_SUCCESS
	否则返回EM_PVRSTREAM_FAILED

备注：
	例如，一段1000S的录制，当前播放位置为800，
	剪切200～400段的数据，
	那么当前的播放位置就为800 -（400 - 200）= 600S  
*/
EPvrStreamResult	PvrStream_Cut(const char *szStreamName,  SPvrStreamCutMark *pMarks, int nMarksCount, UINT *pOutCutLen,UINT *pPlayPosInSec);

/*!
功能描述：
	将时移录制拷贝为普通录制

参数：
	hTShiftFile 
		[in]	时移录制的句柄
	uStartSecond 
		[in]	拷贝起始点在时移录制中的位置 
	uEndSecond  
		[in]	拷贝结束点在移录制中的位置
	szNewStreamName  
		[in]	拷贝的新的文件名


返回值
	成功返回EM_PVRSTREAM_SUCCESS
	否则返回EM_PVRSTREAM_FAILED

备注：
	无
*/
EPvrStreamResult	PvrStream_CopyTshiftToFile( HCSHANDLE hTShiftFile, UINT uStartSecond, UINT uEndSecond, const char *szNewStreamName);

#ifdef __cplusplus
}
#endif

#endif

