/******************************************************************************
FileName:  cspm_filemgr.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:
定义了PVR存储设别管理和PVR文件管理的相关接口
模块简称PVRFILE

修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create

*******************************************************************************/
#ifndef CSPM_FILEMGR_H
#define CSPM_FILEMGR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cspm_record.h"
#include "cspm_notifymgr.h"

/*! Pvr文件默认分组索引*/
#define CSPVRFILE_GROUP_ALL_IDX  0

/*!Pvr文件路径名长度*/
#define CSPVRFILE_PATH_NAME_LEN 256

/*!
========================================= 存储设备相关的接口 =========================================
*/
/*!
	获取系统所有存储设备的空闲块的数目
\return 系统所有存储设备的空闲块(块大小位512B)的数目
\note 返回的空闲块数目是硬盘中实际空闲块数目减去为TShift的预留块数目，
\note 例如硬盘中实际共有空闲块10M，TShift预留块数目为6M，则本函数返回的空闲块数目为4M
*/
DWORD CSPVRFILEGetAllStorageFreeBlocks(void);

/*!
	获取系统所有存储设备的块总数目
\return 系统所有存储设备的块(块大小位512B)总数目
*/
DWORD CSPVRFILEGetAllStorageBlocks(void);
/*!
	格式化系统的所有存储设备
\return 成功返回TRUE; 失败返回FALSE
\note 请停止所有Pvr文件回放、录制后再格式化硬盘
\note 如果系统正在使用存储设备，则格式化失败
*/
BOOL CSPVRFILEFormatAllStorage(void);
/*!
	 获取系统已识别的存储设备个数
\return 系统已识别的存储设备个数
*/
DWORD CSPVRFILEGetStorageCount(void);
/*!
	获取存储设备句柄
@paran[in] nIdx 存储设备索引
\return 存储设备句柄
*/
HCSHANDLE CSPVRFILEGetStorageHandle(int nIdx);
/*!
	获取存储设备空闲块数目
@param[in] hStorage 存储设备句柄
\return 系统存储设备的实际空闲块(块大小位512B)的数目
*/
DWORD CSPVRFILEGetStorageFreeBlocks(HCSHANDLE hStorage);
/*!
	获取存储设备总空间块数目
@param[in] hStorage 存储设备句柄
\return 系统存储设备的总块数目
*/
DWORD CSPVRFILEGetStorageBlocks(HCSHANDLE hStorage);
/*!
	格式化存储设备
@param[in] hStorage 存储设备句柄
\return 成功返回TRUE, 失败返回FALSE
\note 请停止所有Pvr文件回放、录制后再格式化硬盘
\note 如果系统正在使用存储设备，则格式化失败
*/
BOOL CSPVRFILEFormatStorage(HCSHANDLE hStorage);

/*!
	获取存储设备名称
@param[in] hStorage 存储设备句柄
\return 返回存储设备名称，ASII码字符串
*/
const char* CSPVRFILEGetStorageName(HCSHANDLE hStorage);

/*!
	设置录制目标存储设备
@param[in] hStorage 存储设备句柄
\return 成功返回TRUE，失败返回FALSE
\note 目标存储设备实际空闲空间必须大于TShift预留空间
*/
BOOL CSPVRFILESetRecordTarget(HCSHANDLE hStorage);

/*!
	是否存在已置删除标记位的文件
\return成功返回TRUE; 失败返回FALSE
\note 在Filelist对话框退出时，需判断是否存在已置删除标记文件，如果是，则需提示用户删除文件
*/
BOOL CSPVRFILEIsExistDelFlag(void);

/*!
	删除所有带删除标记的文件(从存储设备中删除，不可恢复)
\return 成功返回TRUE; 失败返回FALSE
\note 应用举例：删除文件是异步执行的,在删除文件过程中,会有删除文件进度通知用户
*/
BOOL CSPVRFILEConfirmDelFile(void);

/*!
	取消所有文件的删除标记
*/
BOOL CSPVRFILECancelAllDelFlag(void);

/*!
========================================= 文件分组相关接口 =========================================
\note 系统有一个默认分组 CSPVRFILE_GROUP_ALL_IDX, 包含了所有Pvr文件文件,该分组不可编辑
*/

/*!
	新建一个分组
@param[in] wsGroupName 分组名称WideWord的字符串
\return 新建分组的句柄
\note 所有分组信息会掉电保存,创建的分组,在机器关开机后,系统会自动创建该分组的对象
*/
HCSHANDLE CSPVRFILENewGroup(const char *wsGroupName);

/*!
	文件添加到分组
@param[in] hFile Pvr文件句柄
@param[in] hGroup 分组句柄
\return 成功返回TRUE; 失败返回FALSE
\note 如果文件已属于某分组，再调用本函数添加到该分组时返回TRUE
\note 不能将文件添加到默认分组CSPVRFILE_GROUP_ALL_IDX
*/
BOOL CSPVRFILEAddFileToGroup(HCSHANDLE hFile, HCSHANDLE hGroup);

/*！
	将文件从分组中移出
@param[in] hFile Pvr文件
@param[in] hGroup 分组句柄
\return 成功返回TRUE; 失败返回FALSE
\note 如果文件不属于某分组，再调用本函数从该分组移出文件时返回TRUE
\note 不能将文件从默认分组CSPVRFILE_GROUP_ALL_IDX中移出
\note 本函数只是将文件从分组中移出，并未删除该文件
*/
BOOL CSPVRFILERemoveFileFromGroup(HCSHANDLE hFile, HCSHANDLE hGroup);

/*!
	获取系统所有分组的数目
\return 返回系统所有分组的数目，系统出错时返回0
\note 正常情况下至少有一个默认ALL分组
*/
int CSPVRFILEGetGroupCount(void);

/*!
	获取分组
@param[in] nIdx 分组索引(大于等于ALL 且小于系统所有分组数目)
\return 分组句柄
*/
HCSHANDLE CSPVRFILEGetGroup(int nIdx);
/*!
	判断文件是否属于分组
@param[in] hFile Pvr文件
@param[in] hGroup 分组句柄
\return 文件属于分组返回TRUE; 否则返回FALSE
\note
*/
BOOL CSPVRFILEIsFileInGroup(HCSHANDLE hFile, HCSHANDLE hGroup);

/*!
	获取分组的用户名
@param[in] hGroup 分组句柄
@param[out] wsOutName 输出分组名（wideword类型）
@param[in] nLen 输出分组名字符串的大小
\return 成功返回TRUE; 失败返回FALSE
\note 默认分组CSPVRFILE_GROUP_ALL_IDX的分组名为 "ALL"
*/
BOOL CSPVRFILEGetGroupName(HCSHANDLE hGroup,char *wsName, int nLen);

/*!
	重命名分组名
@param[in] szGroupName 新分组名(wideword 字符串)
@param[in] nNameLen 新分组名字符串长度
@param[in] hGroup 分组句柄
\return 成功返回TRUE; 失败返回FALSE
\note 默认分组CSPVRFILE_GROUP_ALL_IDX的分组名不可修改
*/
BOOL CSPVRFILERenameGroup(const char* wsGroupName, int nNameLen, HCSHANDLE hGroup);

/*!
	删除节目分组
@param[in] hGroup 分组句柄
\return 成功返回TRUE; 失败返回FALSE
\note 不能删除默认分组CSPVRFILE_GROUP_ALL_IDX
\note 删除分组后，会对引起其他的分组索引的改变
*/
BOOL CSPVRFILEDelGroup(HCSHANDLE hGroup);

/*!
========================================= 文件列表相关接口 =========================================
\note 文件列表的所有遍历操作均通过Iterator完成，
*/

/*！
	创建迭代器
@param[in] hGroup 分组句柄
\retuen 迭代器句柄，用与迭代操作
\note 
*/
HCSHANDLE CSPVRFILECreateIterator(HCSHANDLE hGroup);

/*！
	定位至迭代器的第一个元素
@param[in] hIterator 迭代器句柄
\note 
*/
void CSPVRFILEIteratorGoFirst(HCSHANDLE hIterator);

/*!
	定位迭代器至下一个元素
@param[in] hIterator 迭代器句柄
\return 成功返回TRUE; 失败返回FALSE
\note 当迭代器的当前元素是最末尾元素时，定位下一个元素失败
*/
BOOL CSPVRFILEIteratorGoNext(HCSHANDLE hIterator);

/*!
	获取迭代器当前元素的Pvr文件
@param[in] hIterator 迭代器句柄
\return 迭代器当前元素的Pvr文件
*/
HCSHANDLE CSPVRFILEIteratorGetCurrentFile(HCSHANDLE hIterator);

/*！
	迭代器当前是否是最后一个元素
@param[in] hIterator 迭代器句柄
\return 迭代器当前是最后一个元素返回TRUE,否则返回FALSE
*/
BOOL CSPVRFILEIteratorIsTail(HCSHANDLE hIterator);

/*!
	销毁迭代器
@param[in] hIterator 迭代器句柄
*/
void CSPVRFILEDestroyIterator(HCSHANDLE hIterator);

/*！
	文件列表排序类型的枚举定义
*/
typedef enum _CSPVRFILESortType_E
{
	/*!按照录制时间排序*/
	EM_PVRFILE_SORT_BY_RECTIME,
	/*!按照录制文件的可播放时间排序*/
	EM_PVRFILE_SORT_BY_DUARATION,
	/*!按照录制的用户名进行排序*/
	EM_PVRFILE_SORT_BY_USERNAME,
	/*!按照录制的频道名进行排序*/
	EM_PVRFILE_SORT_BY_CHNLNAME,
	/*!按照回放次数进行排序*/
	EM_PVRFILE_SORT_BY_PLAYCOUNT,
	/*!系统支持的排序种类个数*/
	EM_PVRFILE_TYPE_COUNT
}CSPVRFILESortType_E;

/*!
	迭代器排序（暂未实现）
@param[in] hIterator 排序的目标迭代器句柄
@param[in] eSortType 排序的方式
@param[in] bIncrease 是否按升序排序
\return 成功返回TRUE; 失败返回FALSE
\note 排序后，迭代器的当前元素指向的Pvr文件不变
\note 支持多次排序
*/
BOOL CSPVRFILEIteratorSort(HCSHANDLE hIterator, CSPVRFILESortType_E eSortType, BOOL bIncrease);

/*!
========================================= 文件相关接口 =========================================
\note 对应每个Pvr文件，PVR内部创建一个Pvr文件对象与之对应，所有Pvr文件对象的创建由PVR内部执行，
\note 用户只能通过文件列表迭代器从PVR获取文件进行编辑操作，用户无法创建PVR文件
*/

/*!
	文件操作的返回值枚举类型定义
*/
typedef enum _CSPVRFILERet_E
{
	/*!操作成功*/
	EM_PVRFILE_RET_SUCCESS = 0,
	/*!操作失败*/
	EM_PVRFILE_RET_FAILED,
	/*!对应的文件已不存在*/
	EM_PVRFILE_RET_NOT_EXIST
}CSPVRFILERet_E;

/*!
	PVR文件信息
*/
typedef struct _CSPVRFILEFileInfo_S
{
	/*!录制文件的频道名（WideWord字符串）*/
	char		wsChnlName[CSPVR_NAME_SIZE];	
	/*!录制文件的用户友好名称，默认为[ChnlName+EventName]（WideWord字符串)*/
	char		wsUserName[CSPVR_NAME_SIZE];	
	/*!录制文件的可不放长度*/
	DWORD		dwDurationSec;	
	/*!录制文件的录制时间*/
	CSUDIPLUSOSSysTime_S sStartTime;		
}CSPVRFILEFileInfo_S;

/*!
	获取文件信息
@param [in] hFile PVR文件句柄
@param [out] psInfo 输出PVR文件信息
\return 操作结果
\note
*/
CSPVRFILERet_E CSPVRFILEGetFileInfo(HCSHANDLE hFile, CSPVRFILEFileInfo_S *psInfo);

/*!
	重命名pvr文件
\param[in] hFile PVR文件句柄
\param[out] wszUserName 新用户自定义名称（WideWord字符串）
\return 操作结果
*/
CSPVRFILERet_E CSPVRFILERename(HCSHANDLE hFile, const char *wsUserName);

/*!
	设置删除标记
@param[in] hFile PVR文件句柄
@param[out] bDelFlag 新的删除标记
\return 操作结果
\note 删除标记不会掉电保存
*/
CSPVRFILERet_E CSPVRFILESetDelFlag(HCSHANDLE hFile, BOOL bDelFlag);

/*!
	获取删除标记
@param[in] hFile PVR文件句柄
@param[out] pbOutDelFlag 输出文件的删除标记
\return 操作结果
\note
*/
CSPVRFILERet_E CSPVRFILEGetDelFlag(HCSHANDLE hFile, BOOL *pbDelFlag);

/*!
	设置上锁标记
@param[in] hFile PVR文件句柄
@param[in] bLockFlag 新上锁标记
\return 操作结果
\note 上锁标记会掉电保存
*/
CSPVRFILERet_E CSPVRFILESetLockFlag(HCSHANDLE hFile, BOOL bLockFlag);

/*!
	获取上锁标记
@param[in] hFile PVR文件句柄
@param[out] pbOutLockFlag 输出是否上锁标记
\return 操作结果
\note
*/
CSPVRFILERet_E CSPVRFILEGetLockFlag(HCSHANDLE hFile, BOOL *pbLockFlag);

/*!
	获取PVR文件路径
@param[in] hFile PVR文件句柄
@param[out] szFilePath 输出PVR文件的路径名,数组长度建议设置为 CSPVRFILE_PATH_NAME_LEN
@param[in] nPathLen 传入字符串数组的长度,建议设置为 CSPVRFILE_PATH_NAME_LEN
\return 操作结果
\note 输出的PVR文件路径名为ASII码字符串
\note PVR路径名用于PVR文件回放
*/
CSPVRFILERet_E CSPVRFILEGetFilePath(HCSHANDLE hFile, char *szFilePath, int nPathLen);

/*!
	判断文件是否正在录制中
@param[in] hFile PVR文件句柄
@param[out] pbOutIsRecording 输出该PVR文件是否正在录制
\return 
*/
CSPVRFILERet_E CSPVRFILEIsRecording(HCSHANDLE hFile, BOOL *pbIsRecording);

/*!
	判断文件是否正在回放中
@param[in] hFile PVR文件句柄
@param[out] pbOutIsPlaying 输出该PVR文件是否正在回放
*/
CSPVRFILERet_E CSPVRFILEIsPlaying(HCSHANDLE hFile, BOOL *pbIsPlaying);

/*文件管理模块支持的消息通知类型*/
typedef enum _CSPVRFILENotifyType_E
{
	/*!
		文件进度通知删除
		\note	dwNotifyType:	EM_PVRFILE_DEL_FILE
		\note	pNotifyData:	删除进度百分比(0--100)
		\note	hSender:		未定义
	*/
	EM_PVRFILE_DEL_PROGRESS = 0,
	/*!
		文件列表更新通知
		\note	dwNotifyType:	EM_PVRFILE_UPDATE_ITR
		\note	pNotifyData:	未定义  
		\note	hSender:		未定义
	*/
	EM_PVRFILE_UPDATE_FILELIST,
	/*!
		存储设备已满通知
		\note	dwNotifyType:	EM_PVRFILE_HDD_FULL
		\note	pNotifyData:	未定义  
		\note	hSender:		未定义
	*/
	EM_PVRFILE_HDD_FULL,
	/*!
		识别新存储设备通知
		\note	dwNotifyType:	EM_PVRFILE_INPLUG_STORAGE
		\note	pNotifyData:	未定义  
		\note	hSender:		未定义
	*/
	EM_PVRFILE_PLUGIN_STORAGE,
	/*!
		存储设备被拔除通知
		\note	dwNotifyType:	EM_PVRFILE_INPLUG_STORAGE
		\note	pNotifyData:	未定义  
		\note	hSender:		未定义
	*/
	EM_PVRFILE_PLUGOUT_STORAGE,
	/*!
		文件管理模块通知类型个数
	*/
	EM_PVRFILE_NOTIFY_TYPE_COUNT
}CSPVRFILENotifyType_E;

/*!
	获取消息通知管理器的句柄
\return 消息通知管理器的句柄
*/
HCSHANDLE CSPVRFILEGetNotifyMgr(void);

/*!
	获取系统的默认根目录
\return 系统默认的根目录*/
const char* CSPVRFILEGetDefaultDir(void);

/*!
	系统存储设备状态枚举类型定义
*/
typedef enum _CSPVRFILEStorageStatus_E
{
	/*!存储设备可正常使用*/
	EM_PVRFILE_STORAGE_USABLE =0,
	/*!存储设备不可用(不可启动TShift录制)*/
	EM_PVRFILE_STORAGE_UNUSABLE,
	/*!无可用的存储设备*/
	EM_PVRFILE_NO_STORAGE,
	/*!存储设备已满(可启动TShift录制)*/
	EM_PVRFILE_STORAGE_NOT_ENOUGH,
	/*!存储设备的文件系统不支持PVR*/
	EM_PVRFILE_STORAGE_FSTYPE_NOT_SUPPORT
}CSPVRFILEStorageStatus_E;

/*!
	获取系统存储设备状态
\return 系统存储设备状态
*/
CSPVRFILEStorageStatus_E CSPVRFILEGetStorageStatus(void);


/*!
	获取文件的加密key
@param[in] hFile PVR文件句柄
@param[out] pucCryptKey 存放加密key的buffer
@param[in] nKeySize 存放加密key的buffer大小
\return 返回加密key的实际大小
*/
int CSPVRFILEGetEncryptKey(HCSHANDLE hFile, BYTE * pucEncryptKey, int nKeyBufferLen);

/*!
	获取文件的父母锁
@param[in] hFile PVR文件句柄
\return 文件的父母锁
\note 如无父母锁，则为0
*/
int CSPVRFILEGetPgLevel(HCSHANDLE hFile);

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

