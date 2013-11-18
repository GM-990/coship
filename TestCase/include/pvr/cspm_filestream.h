/******************************************************************************
FileName:  cspm_filestream.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:
定义了PVR回放文件流的相关接口
模块简称PVRSTREAM

修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create

*******************************************************************************/
#ifndef CSPM_FILESTREAM_H
#define CSPM_FILESTREAM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "CSUSPCommonType.h"
#include "cspm_record.h"

/*!多音频语言名称字符串长度长度*/
#define CSPVR_AUDIO_NAME_LEN 6
/*!支持的最多音频个数*/
#define CSPVR_AUDIO_COUNT_MAX 6
/*!非法的书签值*/
#define CSPVR_INVALID_BOOKMARK_VALUE 0xffffffff

    /*!支持的最多字幕个数*/
#define CSPVR_SUBTITLE_COUNT_MAX 8

#define CSPVR_SUBTITLE_MAXDESCNT_PERSTREAM (20)

    /*!音频信息*/
    typedef struct _CSPVRSTREAMAudInfo_S
    {
        /*!左声道输出语言(ASII码字符串)*/
        char szLeftName[CSPVR_AUDIO_NAME_LEN];
        /*!右声道输出语言(ASII码字符串)*/
        char szRightName[CSPVR_AUDIO_NAME_LEN];
        /*!音频pid*/
        WORD wPid;
    } CSPVRSTREAMAudInfo_S;


    /*!Subtitle 信息*/
    typedef struct _CSPVRSubtitleEntity_S
    {
        /*!Subtitle 语言信息*/
        BYTE   aucLanguage[3];
        BYTE   ucSubtitleType;
        WORD   wCompPageID;
        WORD   wAnciPageID;
    } CSPVRSubtitleEntity_S;

    typedef struct _CSPVRSubtitileParams_S
    {
        BYTE    ucStreamType;
        WORD    wSubtitlePID;
        BYTE    ucDescriptorCount;
        CSPVRSubtitleEntity_S sDescriptor[CSPVR_SUBTITLE_MAXDESCNT_PERSTREAM];
    } CSPVRSubtitileParams_S;

    /*!
    	PVR文件流信息
    */
    typedef struct _CSPVRSTREAMPvrInfo_S
    {
        /*!频道名(WideWord字符串)*/
        char				wsChnlName[CSPVR_NAME_SIZE];
        /*!用户可修改的用户友好名称,WideWord字符串*/
        char				wsUserName[CSPVR_NAME_SIZE];
        /*!多音频信息*/
        CSPVRSTREAMAudInfo_S	sMutiAud[CSPVR_AUDIO_COUNT_MAX];
        /*!多音频的个数*/
        int					nAudCount;
        /*!该文件的启动录制时间*/
        CSUDIPLUSOSSysTime_S			sStartRecordTime;
        /*!Subtitle 信息*/
        CSPVRSubtitileParams_S m_asSubtitle[CSPVR_SUBTITLE_COUNT_MAX];
        /*!Subtitle 的个数*/
        int m_nSubtitleCount;
    } CSPVRSTREAMPvrInfo_S;

/*!
	打开一个PVR文件流
@param[in] PVR文件的路径名，在PVR文件管理模块调用CSPVRFMFGetFilePath函数获取得到的
\return PVR文件流的句柄
\note 打开一个PVR流文件用于回放
\note请先调用CSPVRFMFGetFilePath获取文件路径后,再调用本函数Open该文件
*/
HCSHANDLE CSPVRSTREAMOpen(const char *szFilePath);

/*!
	获取PVR文件流信息
@param[in] hPvrStream PVR文件流
@param[out] psOutInfo 输出PVR文件流信息
\return 成功返回TRUE,否则返回FALSE
\note
*/
BOOL CSPVRSTREAMGetInfo(HCSHANDLE hPvrStream, CSPVRSTREAMPvrInfo_S *psOutInfo);

/*!
	关闭PVR文件流
@param[in] hPvrStream PVR文件流句柄
\return 成功返回TRUE,否则返回FALSE
\note 正在回放的PVR文件流不能被关闭
*/
BOOL CSPVRSTREAMClose(HCSHANDLE hPvrStream);

/*!
	获取PVR文件流的书签个数
@param[in] hPvrStream PVR文件流句柄
\return PVR文件流的书签个数
*/
DWORD CSPVRSTREAMGetBookMarkCount(HCSHANDLE hPvrStream);

/*!
	获取PVR文件流的书签的位置
@param[in] hPvrStream PVR文件流句柄
\return PVR文件流的书签的位置(为回放的时间位置,单位为秒), 出错时返回 PVRPF_INVALID_TIME
*/
DWORD CSPVRSTREAMGetBookMark(HCSHANDLE hPvrStream, int nIdx);

/*!打书签标记的结果*/
typedef enum _CSPVRSTREAMMarkRet_E
{
	/*!成功标记了当前位置*/
	EM_PVRSTREAM_MARK_SUCCESS=0,
	/*!移除了当前位置的标记*/
	EM_PVRSTREAM_MARK_REMOVE,
	/*!标记已满*/
	EM_PVRSTREAM_MARK_FULL,
	/*!标记出错*/
	EM_PVRSTREAM_MARK_FAILED
}CSPVRSTREAMMarkRet_E;

/*!
	书签标记PVR文件流的指定位置
@param[in] hPvrStream PVR文件流句柄
@param[in] dwMarkValueInS 需标记的PVR文件流的位置(回放的时间位置,单位为秒)
\return 标记的结果(见CSPVRSTREAMMarkRet_E的定义)
\note 
*/
CSPVRSTREAMMarkRet_E CSPVRSTREAMSetBookMark(HCSHANDLE hPvrStream, DWORD dwMarkValueSec);

/*!
	片断隐藏
@param[in]  hPvrStream PVR文件流句柄
@param[in] dwStartPosInS 需要隐藏片断的开始位置(回放的时间位置,单位为秒)
@param[in] dwEndPodInS 需要隐藏片断的结束位置(回放的时间位置,单位为秒)
\return 成功返回TRUE,否则返回FALSE
\note 片断隐藏对当前正在进行的回放以及以后(包括关开机后)的该PVR文件回放均有效
*/
BOOL CSPVRSTREAMHideSegment(HCSHANDLE hPvrStream, DWORD dwStartPosSec,DWORD dwEndPosSec);

/*!
	恢复所有已隐藏的片断
@param[in] hPvrStream PVR文件流句柄
\return 成功返回TRUE,否则返回FALSE
\note 
*/
BOOL CSPVRSTREAMUnHideAllSegment(HCSHANDLE hPvrStream);

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

