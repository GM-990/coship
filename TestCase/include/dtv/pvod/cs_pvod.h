/*************************************************************************
* Copyright (c) 2009, ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
*
* File: cs_pvod.h
* File Identify:
* Summary: PVOD接口定义
*
* Current Version: 1.0
* Author:
* date: 2009-06-05
*
* history:
/-------------------------------------------------------------------------/
revision             author                 reason            date

/-------------------------------------------------------------------------/
**************************************************************************/

/**
@defgroup cs_pvod PVOD终端处理接口API
@{
*/

/**
@file cs_pvod.h
*/

#ifndef _CS_PVOD_H_
#define _CS_PVOD_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"{
#endif

/** @brief PVOD文件路径最大长度 */
#define CSPVOD_PATH_MAX (255)

/** @brief PVOD在PMT中使用的stream_type定义 */
typedef enum _CSPVOD_StreamType_E
{
    EM_CSPVOD_StreamType = 0x06    ///< private_stream_2: ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data
}CSPvodStreamType_E;

/** @brief PVOD在SDT中使用的service_type定义 */
typedef enum _CSPVOD_ServiceType_E
{
    EM_CSPVOD_ServiceType = 0xF0   ///< 自定义的PVOD的业务类型
}CSPvodServiceType_E;

/** @brief PVOD工作状态定义 */
typedef enum _CSPvodState_E
{
    EM_CSPVOD_STATE_UNUSED = 0,     ///< 未使用
    EM_CSPVOD_STATE_CREATED,        ///< 创建
    EM_CSPVOD_STATE_INITED,         ///< 已初始化
    EM_CSPVOD_STATE_LISTENING,      ///< 监听中
    EM_CSPVOD_STATE_UPDATING,       ///< 更新中
    EM_CSPVOD_STATE_STOPPED        ///< 已停止
}CSPvodState_E;

typedef enum _CSPvodUpdateProgressMsg_E
{
	EM_CSPVOD_MSG_UpdateProgress = 0x9898	///< 更新进度消息
}CSPvodMsg_E;
/** @brief PVOD错误类型定义定义 */
typedef enum _CSPvodError_E
{
    EM_CSPVOD_ERR_NONE = 0,         ///< 无错误
    EM_CSPVOD_ERR_DEMUX,            ///< Demux错误, 无法操作Demux(操作包括请求与释放)
    EM_CSPVOD_ERR_DEMUX_TIMEOUT,    ///< Demux错误, 请求Demux超时
    EM_CSPVOD_ERR_FORMAT_PES,       ///< PES格式错误
    EM_CSPVOD_ERR_FORMAT_PVOD,      ///< PVOD格式错误
    EM_CSPVOD_ERR_DSC,              ///< 解析描述文件出错

    EM_CSPVOD_ERR_DISK_NOSPACE,     ///< 磁盘空间不够

    EM_CSPVOD_ERR_FS_ACCESS,        ///< 文件或目录访问失败
    EM_CSPVOD_ERR_FS_CREATE,        ///< 创建文件或目录失败
    EM_CSPVOD_ERR_FS_DELETE,        ///< 删除文件或目录失败
    EM_CSPVOD_ERR_FS_READ,          ///< 文件或目录读取失败
    EM_CSPVOD_ERR_FS_WRITE         ///< 文件或目录写入失败
}CSPvodError_E;

/** @brief PVOD终端模块的回调事件类型, 请转发事件到客户端应用自己的线程做后续处理@see CSPvod_RegisterCallback() */
typedef enum _CSPvodEventType
{
    EM_CSPVOD_EVENT_PROC_START = 0, ///< 成功请求Demux, 不成功时则返回EM_CSPVOD_EVENT_ERROR事件, 错误为EM_CSPVOD_ERR_DEMUX
    EM_CSPVOD_EVENT_VERSION_CHANGE, ///< 检测到版本更新
    EM_CSPVOD_EVENT_UPDATE_SUCCESS, ///< 版本更新完成
    EM_CSPVOD_EVENT_ERROR,          ///< 错误事件
    EM_CSPVOD_EVENT_TIMEOUT         ///< 超时
}CSPvodEventType_E;

/** @brief PVOD终端模块的事件数据,  请转发事件到客户端应用自己的线程做后续处理@see CSPvod_RegisterCallback()*/
typedef struct
{
    CSPvodEventType_E eEventType;   ///< 事件类型
    CSPvodError_E eErrType;         ///< 事件为EM_PVOD_UPDATE_ERROR时指示具体错误
    void* pvData;                   ///< 产生错误的数据指针, 一般只输出demux的返回的数据
    int nDatalen;                   ///< 产生错误的数据的长度
}CSPvodEvent_S;

/** @brief CSPvod_Create的参数@see CSPvod_Create() */
typedef struct
{
    char* pcRootPath;   ///< PVOD数据存储的目录
}CSPvodInstanParam_S;

/** @brief CSPvod_Start的参数@see CSPvod_Start() */
typedef struct
{
    DWORD dwDemuxId; ///< demux编号, 单demux/单tuner的机顶盒通常为0
    WORD wPesPid;    ///< 承载PVOD数据的PES流的PID
}CSPvodStartParam_S;

/**
@brief PVOD模块事件回调函数
@param[in] psEvent 输入参数, 事件数据
@param[in] pUserData 输入参数, 用户注册回调时的自定义参数
@param[in] hPvodHandle 输入参数, 事件的发出者句柄(CSPvod_Create的返回值)
@note 请勿在本回调中执行过多代码, 建议抛出事件到其它线程处理
@see CSPvod_RegisterCallback
*/
typedef void (*CSOnPvodCallback_F)(CSPvodEvent_S* psEvent, void* pvUserData, HCSHANDLE hPvodHandle);
typedef void (*CSOnPvodUpdateCallBack_F)(void* pvUserData);

/**
@brief PVOD模块的初始化
@return 返回模块初始化的结果
@retval TRUE 成功
@retval FALSE 失败
@note 必须在CSPvod_Create之前调用, 且只能被调用一次, 调用第二次时不处理
@post PVOD模块结束使用后可调用 ::CSPvod_ModuleTerminate() 释放资源
@see CSPvod_ModuleTerminate()
*/
BOOL CSPvod_ModuleInit(void);

/**
@brief PVOD模块的终止, 释放由 ::CSPvod_ModuleInit()申请的资源
@return 返回模块结束的结果
@retval TRUE 成功
@retval FALSE 失败
@note PVOD模块结束, 销毁资源, 在未调用::CSPvod_ModuleInit()之前也可使用
@see CSPvod_ModuleInit()
*/
BOOL CSPvod_ModuleTerminate(void);

/**
@brief 创建一个PVOD任务实例, 可创建多个PVOD实例

创建PVOD实例后, 使用 ::CSPvod_Destroy()销毁实例
@param[in] psInstanParam 输入参数, 创建时所需参数
@return 返回创建PVOD实例的结果
@retval 一个PVOD实例的句柄 成功
@retval NULL 失败
@note PVOD模块支持多实例
@code

HCSHANDLE hPvodHandle = NULL;
CSPvodInstanParam_S sInstanParam;
sInstanParam.pcRootPath = "/pvod_root";//linux形式
hPvodHandle = CSPvod_Create(&sInstanParam);

@endcode
@pre @e psInstanParam 不为NULL
@see CSPvod_Destroy(), CSPvod_Start(), CSPvod_Stop()
*/
HCSHANDLE CSPvod_Create(CSPvodInstanParam_S* psInstanParam);

/**
@brief 销毁一个PVOD实例
@param[in] hPvodHandle 输入参数, ::CSPvod_Create()所创建的PVOD实例句柄
@return 返回销毁PVOD实例的结果
@retval TRUE 成功
@retval FALSE 失败
@pre @e hPvodHandle 不为NULL, 为::CSPvod_Create() 返回值
@see CSPvod_Create(), CSPvod_Start(), CSPvod_Stop()
*/
BOOL CSPvod_Destroy(HCSHANDLE hPvodHandle);

/**
@brief 设置PVOD回调事件处理函数
@param[in] hPvodHandle 输入参数,  ::CSPvod_Create() 所创建的PVOD实例的句柄
@param[in] fnCallback 输入参数, 客户端的回调函数
@param[in] pvUserData 输入参数, 自定义数据, 在回调函数中使用
@return 返回注册回调处理函数的结果
@note 在::CSPvod_Create()调用之后, ::CSPvod_Start()调用之前调用, 回调函数fnCallback不可为空, 自定义数据pvUserData可为空
@code

static void PvodCallback(CSPvodEvent_S* psEvent, void* pvUserData, HCSHANDLE hPvodHandle);//函数声明
BOOL bRet = FALSE;
bRet = CSPvod_RegisterCallback(hPvodHandle, PvodCallback, pvUserData);

@endcode
@retval TRUE 成功
@retval FALSE 失败
@pre @e hPvodHandle 不为NULL, 为::CSPvod_Create() 返回值
@pre @e fnCallback 不为NULL
*/
BOOL CSPvod_RegisterCallback(HCSHANDLE hPvodHandle, CSOnPvodCallback_F fnCallback, PVOID pvUserData);

/**
@brief 开始PVOD处理：使用Demux请求数据, 开始PVOD数据的接收或更新

当需要停止处理时, 使用 ::CSPvod_Stop() 接口
@param[in] hPvodHandle 输入参数, ::CSPvod_Create() 创建的PVOD实例的句柄
@param[in] psStartParam 输入参数, 开始PVOD处理时的参数
@return 返回开始PVOD实例运行的结果
@retval TRUE 成功
@retval FALSE 失败
@note
- 每次调用本API, 会回   ::EM_CSPVOD_EVENT_PROC_START
- 调用中出错发回        ::EM_CSPVOD_EVENT_ERROR
- 检测到版本更新发回    ::EM_CSPVOD_EVENT_VERSION_CHANGE
- 某个版本更新成功发回  ::EM_CSPVOD_EVENT_UPDATE_SUCCESS
@code

BOOL bRet = FALSE;
CSPvodStartParam_S sStartParam;
sStartParam.dwDemuxId = 0;
sStartParam.wPesPid = 0x1DEF;
bRet = CSPvod_Start(hPvodHandle, &sStart);

@endcode
@pre @e hPvodHandle 不为NULL, 为::CSPvod_Create() 返回值
@pre @e psStartParam 不为NULL
@see CSPvod_Create(), CSPvod_Destroy(), CSPvod_Stop()
*/
BOOL CSPvod_Start(HCSHANDLE hPvodHandle, CSPvodStartParam_S* psStartParam);

/**
@brief 停止PVOD处理：取消Demux的数据请求, 停止 ::CSPvod_Start()接口开始的PVOD任务
@param[in] hPvodHandle 输入参数, ::CSPvod_Create() 创建的PVOD实例句柄
@return 返回停止PVOD实例运行的结果
@retval TRUE 成功
@retval FALSE 失败
@note 强制停止PVOD处理, 可在发生错误消息、切换频点、数据更新完成、超出设定的工作时段时调用
@pre @e hPvodHandle 不为NULL, 为::CSPvod_Create() 返回值
@see CSPvod_Create(), CSPvod_Destroy(), CSPvod_Start(),
*/
BOOL CSPvod_Stop(HCSHANDLE hPvodHandle);

/**
@brief 查询PVOD的状态和错误
@param[in] hPvodHandle 输入参数, ::CSPvod_Create() 所创建的PVOD实例
@param[out] peState 输出参数, PVOD的处理状态
@param[out] peError 输出参数, PVOD的错误
@return 返回获取PVOD实例状态和错误的结果
@retval TRUE 成功获取
@retval FALSE 失败
@pre @e hPvodHandle 不为NULL
@pre @e peState 不为NULL
@pre @e peError 不为NULL
*/
BOOL CSPvod_GetStatus(HCSHANDLE hPvodHandle, CSPvodState_E* peState, CSPvodError_E* peError);

/**
@brief 获取PVOD的版本号
@param[out] pcBuf 字符串形式版本号存放地址的指针
@param[in] nSize 字符存放缓冲区大小
@return 当缓冲区长度不够时, 返回-1
@retval >0 版本号的字符串长度
@retval -1 失败
@pre @e pcBuf 不为NULL
@pre @e nSize 大于0
@note 缓冲区大小一般取32
*/
int CSPvod_GetVersion(char *pcBuf, int nSize);

/**
@brief 获取PVOD是否有更新
@param[in] hPvodHandle 输入参数, ::CSPvod_Create() 所创建的PVOD实例
@return 获取失败时返回-1，PVOD处于更新数据时返回1，PVOD不处于更新数据时返回0
@retval -1 失败
@retval 0 非更新中
@retval 1 更新中
*/
int CSPvod_IsUpdated(HCSHANDLE hPvodHandle);

/**
@brief 获取PVOD更新进度
@param[in] hPvodHandle 输入参数, ::CSPvod_Create() 所创建的PVOD实例
@return 当前更新进度
@retval -1 失败
@retval 0 准备更新
@retval 大于0并且小于100 当前更新进度
@retval 100 更新完成
*/
int CSPvod_GetUpdateProgress(HCSHANDLE hPvodHandle);

/**
@brief 获取PVOD更新进度包总数
@param[in] hPvodHandle 输入参数, ::CSPvod_Create() 所创建的PVOD实例
@return 更新包总数
@retval -1 失败
@retval 0 不需要更新
@retval 更新包总数
*/
int CSPvod_GetUpdatePacketNumber(HCSHANDLE hPvodHandle);

#ifdef __cplusplus
}
#endif

#endif /* _CS_PVOD_H_ */

/**
@}
*/

/* End of File */
