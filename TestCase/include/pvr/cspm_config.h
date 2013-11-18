/******************************************************************************
FileName:  cspm_config.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:
定义了PVR初始化接口,PVR系统配置信息的获取和设置接口
模块简称PVR

修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create

*******************************************************************************/
#ifndef _CSPM_CONFIG_H
#define _CSPM_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_os.h"
#include "CSUSPCommonType.h"

/*!Pvr系统支持的最多播放个数*/
#define CSPVR_MAX_PLAY_COUNT  8

/*!无效的设备索引*/
#define CSPVR_INVALID_DEVICE_IDX -1

/*!Pvr数目默认最大长度*/
#define CSPVR_DEFAULT_DATA_MAX_SIZE 0x800

/*!默认TShift预留空间大小*/
#define  CSPVR_DEFAULT_RESERVE_BLOCKS_FOR_TSHIFT (10*1024*1024)

/*!TShift启动模式*/
typedef enum _CSPVRTShiftMode_E
{
	/*!每次启动直播时,自动启动TShift录制*/
	EM_PVR_TSHIFTMODE_AWAYS=0,
	/*!直播被暂停时启动TShift录制*/
	EM_PVR_TSHIFTMODE_START_WHEN_PAUSE,	
	/*!永远不要启动TShift*/
	EM_PVR_TSHIFTMODE_NEVER_START	
}CSPVRTShiftMode_E;

/*!PVR模块配置参数*/
typedef struct _CSPVRConfig_S
{
	/*! 默认录制时长*/
	DWORD				dwDefaultRecDurationInS;
	/*! 录制时是否进行EPG切割*/
	BOOL				bEnableEpgCutRec; 
	/*! TShift启动模式*/
	CSPVRTShiftMode_E		eTShiftMode; 
}CSPVRConfig_S;

/*播放设备*/
typedef struct _CSPVRPlayDevice_S
{
	/*!视频解码器索引*/
	int nVideoDeviceIdx;
	/*!音频解码器索引*/
	int nAudioDeviceIdx;
	/*!TTX解码器索引*/
	int nTtxDeviceIdx;
	/*!Tshift录制索引,如果不需要该路播放TShift，则置为CSPVR_INVALID_DEVICE_IDX*/
	int nTShiftRecIdx;
	/*!输出窗口索引*/
	int nTvWinIdx;
}CSPVRPlayDevice_S;

/*!获取当前码流时间的回调类型定义*/
typedef void (*CSPVRGetSysTime_F)(CSUDIPLUSOSSysTime_S *pOutSysTime);

/*!
	获取时区的回调类型定义
@param[in] 无
\return 时区，如+8, -5等
*/
typedef int  (*CSPVRTimeZone_F)(void);


/*!
	保存Pvr数据的的回调类型定义
@param[in] pData 需保存数据在内存中的起始地址
@param[in] nDataLen 需保存数据的长度
\return 成功返回TRUE,失败返回FALSE
\note 请将Pvr数据保存到支持掉电保存的存储设备中,建议保存到Flash
*/
typedef BOOL (*CSPVRSaveData_F)(PVOID pData,int nDataLen);

/*!
	导入Pvr数据的回调类型定义
@param[out] pData 导入数据的内存目标起始地址
@param[in] nDataLen 需要导入的数据大小
\return 成功返回TRUE,失败返回FALSE
\note 本函数实现请与CSPVRSaveData_F函数实现配对,Pvr数据保存到哪里,就从哪里导入数据
*/
typedef BOOL (*CSPVRLoadData_F)(PVOID pData, int nDataLen);

/*!PVR模块初始化参数*/
typedef struct _CSPVRInitParam_S
{
	/*!Pvr应用需要支持的Pvr录制个数（必须小于等于平台支持的个数）,*/
	DWORD					dwFileRecCount;
	/*!Pvr应用需要支持的TShift录制个数（必须小于等于平台支持的个数）*/
	DWORD					dwTShiftRecCount;
	/*!系统支持的播放个数，（必须小于等于平台支持的个数）*/
	DWORD					dwPlayCount;
	/*!各个播放对应的设备*/
	CSPVRPlayDevice_S		asPlayDevice[CSPVR_MAX_PLAY_COUNT];	
	/*!获取当前码流时间的方法*/
	CSPVRGetSysTime_F		fnGetSysTime;
	/*!获取时区的方法*/
    CSPVRTimeZone_F         fnGetTimeZone;
	/*!保存Pvr数据的方法*/
	CSPVRSaveData_F			fnSaveData;
	/*!导入Pvr数据的回调方法*/
	CSPVRLoadData_F			fnLoadData;	
	/*!Pvr存储数据的最大值(主要用于PVR参数保存和预订信息存储)*/
	DWORD					dwMaxDataSize;
	/*!是否支持内置硬盘*/
	BOOL					bSupportEmbeddedHdd;
	/*!为TShift录制预留空间块(块大小为512)个数*/
	DWORD					dwReserveBlocksForTShift;
}CSPVRInitParam_S;
/*!
	获取默认参数
@param[out] psParam 输出默认参数
\note 请先调用本函数初始化CSPVRPFChnlParam_S结构体,
\note 再对感兴趣的域进行赋值
\note 再调用CSPVRInit初始化Pvr模块
*/
void CSPVRGetDefaultParam(CSPVRInitParam_S *psParam);

/*!
	初始化PVR 模块
@param[in] psParam 初始化参数
\return 是否成功初始化
\note 不允许多次初始化
\note 请在系统初始化时初始化本模块,禁止在运行过程中初始化本模块
*/
BOOL CSPVRInit(const CSPVRInitParam_S *pParam);

/*!
	获取当前系统配置信息
@param[in] psConfig 输出当前系统配置信息
\return 是否成功获取当前系统配置信息
\note
*/
BOOL CSPVRGetConfig(CSPVRConfig_S *psConfig);

/*!
	设置系统配置信息
@param[in] psConfig 新系统配置信息
\return 是否成功设置系统配置信息
\note 所有系统配置信息在以后（包括关开机重启后）的PVR运行过程中均有效
*/
BOOL CSPVRSetConfig(const CSPVRConfig_S *psConfig);

/*!
	获取PvrMgr 模块版本号
@param[out] pcVersion 输出版本信息字符串格式为 "xx.xx.xx"
@param[in] nSize 缓冲区pcVersion的长度
\return 返回PVR模块的版本号
\note 第一种返回值的情况，用户可测试该函数，并为第二个参数nSize提供参考。
如果用户输入的第二个参数小于版本信息的实际长度，尾部多余的字节被截掉
*/
int CSPVRGetVersion( char * pcVersion, int nSize );


#ifdef __cplusplus
}
#endif

#endif

/* End of File */

