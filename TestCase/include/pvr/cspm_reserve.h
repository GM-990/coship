/******************************************************************************
FileName:  cs_pvrmgrreserve.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:
定义了PVR预订的相关接口,仅支持预订录制
模块简称PVRFRSV

修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create

*******************************************************************************/
#ifndef CSPM_RESERVE_H
#define CSPM_RESERVE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "cspm_notifymgr.h"
#include "CSUSPCommonType.h"
#include "cspm_record.h"

enum{
	/*!星期日*/
	CSPVRRSV_WEEKDAY_7 = 0x01,
	/*!星期一*/
	CSPVRRSV_WEEKDAY_1 = 0x02,
	/*!星期二*/
	CSPVRRSV_WEEKDAY_2 = 0x04,
	/*!星期三*/
	CSPVRRSV_WEEKDAY_3 = 0x08,
	/*!星期四*/
	CSPVRRSV_WEEKDAY_4 = 0x10,
	/*!星期五*/
	CSPVRRSV_WEEKDAY_5 = 0x20,
	/*!星期六*/
	CSPVRRSV_WEEKDAY_6 = 0x40
};
/*!
	预订回调通知枚举类型定义
*/
typedef enum _CSPVRFRSVNotifyType_E
{
	/*!
		预订录制即将启动通知
		\note	dwNotifyType:	EM_PVRRSV_BEFORE_START 
		\note	pNotifyData:	距离启动录制的时间(单位为秒)
		\note	hSender:预订录制句柄
	*/
	EM_PVRRSV_BEFORE_START,
	/*!
		预订录制已成功启动通知
		\note	dwNotifyType:EM_PVRRSV_START_SUCCESS
		\note	pNotifyData:启动的录制句柄,与调用CSPVRRFStartRec返回的句柄使用方法一致
		\note	hSender:预订录制句柄
	*/
	EM_PVRRSV_START_SUCCESS,
	/*!
		预订录制启动失败通知
		\note	dwNotifyType:EM_PVRRSV_START_FAILED
		\note	pNotifyData:启动失败的原因
		\note	hSender:预订录制句柄
	*/
	EM_PVRRSV_START_FAILED,
	/*!
		预订录制即将结束通知
		\note	dwNotifyType:EM_PVRRSV_BEFORE_STOP
		\note	pNotifyData:启动的录制句柄,与调用CSPVRRFStartRec返回的句柄使用方法一致
		\note	hSender:预订录制句柄
	*/
	EM_PVRRSV_BEFORE_STOP,
	/*!
		预订录制项过期被删除的通知
		\note	dwNotifyType:EM_PVRRSV_AUTO_DEL
		\note	pNotifyData:预订录制句柄
		\note	hSender:无
	*/
	EM_PVRRSV_OUTOF_DATE,
	/*!
		录制模块通知种类的个数
	*/
	EM_PVRRSV_NOTIFY_TYPE_COUNT
}CSPVRRSVNotifyType_E;

/*!
	获取消息通知管理器的句柄
\return 消息通知管理器的句柄
*/
HCSHANDLE CSPVRRSVGetNotifyMgr(void);

/*!
	预订录制结果返回类型枚举定义
*/
typedef enum _CSPVRRSVRet_E
{
	/*!预订项已满*/
	EM_PVRSV_RET_FULL = 0,
	/*!预订成功*/
	EM_PVRRSV_RET_SUCC,
	/*!预订冲突*/
	EM_PVRRSV_RET_CONFLICT,
	/*!该预订项已存在*/
	EM_PVRRSV_RET_HAS_EXIST,
	/*!预订失败*/
	EM_PVRRSV_RET_FAILED,
}CSPVRRSVRet_E;

/*!
	预订录制的重复模式
*/
typedef enum _CSPVRRSVMode_E
{
	/*单次
		dwModeExtData无效*/
	EM_PVRRSV_MODE_ONCE =0,
	/*每天
		dwModeExtData无效*/
	EM_PVRRSV_MODE_DAY,
	/*每周
		dwModeExtData标志每周的哪些天启动录制
		使用常量CSPVRRSV_WEEKDAY_1......7分别表示每周的哪些天启动录制*/
	EM_PVRRSV_MODE_WEEK, 
}CSPVRRSVMode_E;

/*!
	预订参数定义
*/
typedef struct _CSPVRRSVParam_S
{
	/*!预订开始时间*/
	CSUDIPLUSOSSysTime_S 		sStartTime;
	/*!预订结束时间*/
	CSUDIPLUSOSSysTime_S 		sEndTime;
	/*!预订的重复模式*/
	CSPVRRSVMode_E 		eMode;
	/*!预订重复模式的扩展数据,见预订重复模式枚举说明*/
	DWORD				dwModeExtData;
	/*!预订录制目标文件的用户友好名*/
	char					wsUserName[CSPVR_NAME_SIZE];
}CSPVRRSVParam_S;

/*!
	获取预订参数的默认值
@param[out] psParam 默认参数值
\note 请先调用本函数初始化PVRRFRsvParam_S结构体,
\note 再对感兴趣的域进行赋值,再调用CSPVRRRSVRecord进行预订录制
*/
void CSPVRRSVGetDefaultParam(CSPVRRSVParam_S *psParam);

/*!
	预订录制
@param[in] szLocator 标识头为dtv:的Locator格式为 "dtv:// OrigNetId.TsID.ServiceID"，例如:"dtv:45.36.119"
@param[in] psReserveParam 预订参数
@param[out] phRsvHandle 输出预订录制项的句柄，
\return 预订结果,(见CSPVRRSVRet_E的定义)
*/
CSPVRRSVRet_E CSPVRRSVRecord(const char *szLocator, const CSPVRRSVParam_S* psReserveParam, HCSHANDLE *phRsvHandle);

/*!
	预订冲突判断
@param[in] szLocator 标识头为dtv:的Locator格式为 "dtv:// OrigNetId.TsID.ServiceID"，例如:"dtv:45.36.119"
@param[in] psReserveParam 预订参数
@param[out] phConfilctItems 输出冲突的预订录制项句柄
@param[in] dwConfilctItemsArrayLen 输出预订录制项句柄数组大小
\return 返回的预订冲突个数
\note 如果冲突的预订项个数大于dwRsvRecArrayLen，则返回dwRsvRecArrayLen
*/
DWORD CSPVRRSVGetConflict(const char *szLocator, const CSPVRRSVParam_S* psReserveParam, HCSHANDLE *phConfilctItems, DWORD dwConfilctItemsArrayLen);

/*!
	根据频道和时间信息获取预订句柄
@param[in] szLocator 标识头为dtv:的Locator格式为 "dtv:// OrigNetId.TsID.ServiceID"
@param[in] psStartTime 预订的开始时间
@param[out] psEndTime 预订的结束时间
\return 预订句柄，如果不存在该时段的该频道预定，则返回NULL
\note 当且仅当预订项的开始时间和结束时间都与参数相等时，返回该预订项
*/
HCSHANDLE CSPVRRSVGetHandleByInfo(const char *szLocator, const CSUDIPLUSOSSysTime_S* psStartTime,const CSUDIPLUSOSSysTime_S* psEndTime);

/*!
	获取预订个数
\return 已有预订的个数
*/
int CSPVRRSVGetCount(void);

/*!
	获取预订对象句柄
@param[in] nIdx 预订录制的索引
\return 指定索引的预订项句柄
*/
HCSHANDLE CSPVRRSVGetHandle(int nIdx);

/*!
	修改预订信息
@param[in] hReserveHandle 预订句柄
@param[in] psReserveParam 新预订信息
\return 修改操作的结果
*/
CSPVRRSVRet_E CSPVRRSVModify(HCSHANDLE hReserveHandle, const CSPVRRSVParam_S* psReserveParam);

/*!
	获取预订录制信息
@param[in] hReserveHandle 预订句柄
@param[out] psRsvInfo 预订信息
\return 成功则返回TRUE,否则返回FALSE
*/
BOOL CSPVRRSVGetReserveInfo(HCSHANDLE hReserveHandle, CSPVRRSVParam_S *psReserveInfo);

/*!
	获取预订的频道
@param[in] hReserveHandle 预订句柄
@param[out] pdwTpId 输出频道的TPId
@param[out] pdwServiceId 输出频道的serviceId
\return 成功返回TRUE,失败返回FALSE
\note 
*/
BOOL CSPVRRSVGetReserveChnl(HCSHANDLE hReserveHandle, DWORD *pdwTpId, DWORD *pdwServiceId);

/*!
	判断预订是否已经启动
@param[in] hRsvHandle 预订句柄
\return 该预订录制是否正在进行录制
*/
BOOL CSPVRRSVIsWorking(HCSHANDLE hRsvHandle);

/*!
	删除预定
@param[in] hRsvHandle 预订录制句柄
\return 成功则返回TRUE,否则返回FALSE
\note 正在录制的预订项无法删除，用户必须先停止录制才能删除
*/
BOOL CSPVRRSVDel(HCSHANDLE hRsvHandle);

/*!
	取消预订
@param[in] hRsvHandle 预订句柄
\return 成功则返回TRUE,否则返回FALSE
\note 只能取消即将启动的预订录制项的本次预订
\note 正在进行录制的预订无法取消
\note 每周或每天预订，只能取消本次预订，第二天的预订照样会启动
*/
BOOL CSPVRRSVCancel(HCSHANDLE hRsvHandle);


/*!
	设置预订启动前通知偏移时间
@param[in] nSeconds 偏移时间，单位为秒
\return 成功则返回TRUE,否则返回FALSE
*/
BOOL CSPVRRSVSetPrevStartNotifyTime(int nSeconds);

/*!
	获取预订启动前通知偏移时间
\return 偏移时间，单位为秒
*/
int CSPVRRSVGetPrevStartNotifyTime();

/*!
	恢复默认时调用，将pvr模块配置参数和预定数据恢复到出厂设置
\return 成功则返回TRUE,否则返回FALSE
*/
BOOL CSPVRRSVConfigWithDefault(void);

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

