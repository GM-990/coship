/******************************************************************************
FileName:  cspm_record.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:
定义了PVR录制的相关接口
模块简称PVRREC

修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create

*******************************************************************************/
#ifndef CSPM_RECORD_H
#define CSPM_RECORD_H

#ifdef __cplusplus 
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "cspm_notifymgr.h"
#include "CSUSPCommonType.h"
#include "mmcp_os.h"

/*pvr名称的长度*/
#define CSPVR_NAME_SIZE 60

/*!
	获取系统支持的录制句柄个数
\return 系统支持的录制句柄个数
*/
DWORD CSPVRRECGetSupportRecCount(void);

/*!启动录制结果，函数CSPVRRECStartRec的返回值*/
typedef enum _CSPVRRECRet_E
{
	/*!启动录制成功*/
	EM_PVRREC_RET_SUCCUSS = 0,
	/*!设备冲突*/
	EM_PVRREC_RET_NO_DEVICE,	
	/*!硬盘已满*/
	EM_PVRREC_RET_HDD_FULL,	
	/*!硬盘未准备好*/
	EM_PVRREC_RET_NO_HDD,		
	/*!正在录制中,无法启动*/
	EM_PVRREC_RET_IS_WORKING,		
	/*!未知错误启动失败*/
	EM_PVRREC_RET_UNKOWN_FAILED		
}CSPVRRECRet_E;

/*!
	启动录制
@param[in] pszLocator 标识头为dtv:的Locator格式为 "dtv:// OrigNetId.TsID.ServiceID"，例如:"dtv:45.36.119"
@param[out] phRecHandle 输出录制句柄(启动失败则输出NULL)
\return 启动录制的结果(见CSPVRRECRet_E定义)
\note 
*/
CSPVRRECRet_E CSPVRRECStartRec(const char *pszLocator, HCSHANDLE *phRecHandle);

/*!
	获取正在录制的句柄
@param[out] phOutRecHandleArray 输出正在录制的句柄数组
@param[in] nArraySize 数组大小
\return 返回的正在录制句柄个数
\note 
*/
int CSPVRRECGetRecording(HCSHANDLE* phOutRecHandleArray,int nArraySize);

/*!
	启动录制当前正在直播的频道
@param[out] phRecHandle 输出录制句柄(启动失败则输出NULL)
\return 启动录制的结果(见CSPVRRECRet_E定义)
*/
CSPVRRECRet_E CSPVRRECStartRecCurrentChnl(HCSHANDLE *phRecHandle);

/*!
	停止录制
@param[in] hPvrRecHandle 录制句柄
\return 是否成功停止录制
\note
*/
BOOL CSPVRRECStop(HCSHANDLE hPvrRecHandle);

/*!录制的启动模式*/
typedef enum _CSPVRRECRecStartMode_E
{
	/*!即时录制*/
	EM_PVRREC_STARTMODE_QUICK = 0,	
	/*!预订录制*/
	EM_PVRREC_STARTMODE_SCHEDULE,	
	/*!录制启动模式最大个数*/
	EM_PVRREC_STARTMODE_COUNT
}CSPVRRECRecStartMode_E;

/*!正在录制的信息*/
typedef struct _CSPVRRECRecInfo_S
{
	/*!频道名称WideWord 字符串*/
	char					wsChnlName[CSPVR_NAME_SIZE];	
	/*!用户可修改的用户友好名,WideWord字符串*/
	char					wsUserName[CSPVR_NAME_SIZE];
	/*!启动录制的时间*/
	CSUDIPLUSOSSysTime_S				sStartTime;	
	/*!录制的启动模式*/
	CSPVRRECRecStartMode_E	eStartMode;				
}CSPVRRECRecInfo_S;

/*!
	获取录制信息
@param[in] hPvrRecHandle 录制句柄
@param[out] psInfo 输出录制信息
\return 成功返回TRUE,失败返回FALSE
\note
*/
BOOL CSPVRRECGetRecInfo(HCSHANDLE  hPvrRecHandle, CSPVRRECRecInfo_S *psInfo);

/*!
	获取录制时间
@param[in] hPvrRecHandle 录制句柄
@param[out] pdwDurationMin 录制的总持续时间,单位为分钟
@param[out] pdwPastTimeSec 录制已过去的时间,单位为秒
@param[out] pdwRecLenthSec 录制的有效时长,单位为秒
\return 成功返回TRUE,失败返回FALSE
\note
*/
BOOL CSPVRRECGetRecTime(	HCSHANDLE  hPvrRecHandle, 
					   	DWORD *pdwDurationMin,
					   	DWORD *pdwPastTimeSec,
					   	DWORD *pdwRecLenthSec);
/*!
	重置录制持续时间
@param[in] hPvrRecHandle 录制句柄
@param[in] dwDurationMin 录制持续总时长,单位为分钟
\return 成功返回TRUE,失败返回FALSE
\note 设置的持续时间不得小于已录制时长
*/
BOOL CSPVRRECSetDuration(HCSHANDLE hPvrRecHandle, DWORD dwDurationMin);

/*!
	重置录制目标文件的用户友好名
@param[in] hPvrRecHandle 录制句柄
@param[in] wsUserName 用户友好名,WideWord字符串
@param[in] nNameLen 用户友好名的字符串长度，例如UserName为宽字符串"abc",则长度为3
\return 成功返回TRUE,失败返回FALSE
*/
BOOL CSPVRRECSetUserName(HCSHANDLE hPvrRecHandle, const char *wsUserName, int nNameLen);

/*!
	判断service 是否正在录制
@param[in] hService  Service句柄
\return 正在录制返回TRUE,未录制返回FALSE
*/
BOOL  CSPVRREC_IsRecording(HCSHANDLE hService);

/*!
	录制回调通知枚举类型定义
*/
typedef enum _CSPVRRECNotifyType_E
{
	/*!
		录制停止通知
		\note	dwNotifyType:	EM_PVRREC_NOTIFY_STOP 
		\note	pNotifyData:	未定义, 
		\note	hSender: 		hPvrRecHandle录制句柄
	*/
	EM_PVRREC_NOTIFY_STOP = 0,
	/*!
		录制模块通知种类的个数
	*/
	EM_PVRREC_NOTIFY_TYPE_COUNT
}CSPVRRECNotifyType_E;

typedef enum _CSPVRRECStopCase_E
{
	EM_PVRREC_STOPEDBY_UNKOWN = 0,
	EM_PVRREC_STOPEDBY_STORAGE_FULL,
	EM_PVRREC_STOPEDBY_STORAGE_REMOVED,
	EM_PVRREC_STOPEDBY_TIMEOUT,
	EM_PVRREC_STOPEDBY_IOERROR
}CSPVRRECStopCase_E;
/*!
	获取消息通知管理器的句柄
\return 消息通知管理器的句柄
*/
HCSHANDLE CSPVRRECGetNotifyMgr(void);

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

