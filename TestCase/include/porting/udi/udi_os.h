/**@defgroup CSOS os  对于常用操作系统相关的操作，如创建任务等提供统一接口及定义
@brief 对于常用操作系统相关的操作，如创建任务等提供统一接口及定义。
@brief 除了本文档定义的接口，还需要实现pthread
 
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _UDI_OS_H_
#define _UDI_OS_H_

#include "udi_typedef.h"
#include "udi_assert.h"

#ifdef  __cplusplus
extern "C" {
#endif

//当需要调试某个模块的内存泄露时,打开该宏,clean并完全重新编译该模块即可
//#define USE_DBG_MALLOC

/**@brief 返回该值表示成功*/
#define CS_OSP_SUCCESS					(0)
/**@brief 返回该值表示失败*/
#define CS_OSP_FAILURE					(1)
/**@brief 返回该值表示超时*/
#define CS_OSP_TIMEOUT					(2)

/**@brief 创建互斥量后不立即加锁选项*/
#define	CS_OSP_MUTEX_NOTOBTAIN				(0)
/**@brief 创建互斥量后立即加锁选项*/
#define	CS_OSP_MUTEX_OBTAIN				(1)

/**@brief CSWaitForSingleEvent之后事件自动无效选项*/
#define CS_OSP_EVENT_MANUAL_RESET			(0x01)
/**@brief 初始化事件后立即有效选项*/
#define CS_OSP_EVENT_INITIAL_STATUS			(0x10)

/**@brief 超时参数之立即超时选项*/
#define CS_OSP_TIMEOUT_IMMEDIATE  			(0)
/**@brief 超时参数之永不超时选项*/
#define CS_OSP_TIMEOUT_INFINITY   			(0xFFFFFFFF)

/**@brief 任务启动后立即运行选项*/
#define CS_OSP_TASK_FLAGS_RUN_IMMEDIATELY  		(0)
/**@brief 任务启动后自动挂起选项*/
#define CS_OSP_TASK_FLAGS_SUSPENDED 			(1)
/**@brief 任务名称最大长度*/
#define OS_OSP_THREAD_NAME_MAX_LEN		(32)

/**@brief 定时器选项*/
typedef enum
{
    TIM_MODE_REPEAT = 0,		/**< 重复模式选项，即每指定时间触发一次*/
    TIM_MODE_ONE_SHOT,			/**< 单次模式选项，即仅在指定时间后触发一次*/
    TIM_MODE_END				/**< 模式选项结束标记，无效参数*/
}TIMER_MODE;

/**@brief 任务的基本信息结构体*/
typedef struct
{
	unsigned int uThreadId;	///<任务ID
	unsigned int uPriority;		///<任务优先级
	char szThreadName[OS_OSP_THREAD_NAME_MAX_LEN];	///<任务名称
}CSThreadInfo_S;

/**
@brief 任务入口函数

新建一个任务时，定义一个本类型的函数作为入口函数，任务启动后，该函数会在新任务中被调用。
@param[in] pParam 入口参数，其值为调用CSCreateThread函数时的pvArg参数。
@see CSCreateThread CSCreateThreadEx
*/
typedef void (ThreadEntry)(void *pParam);

/**
@brief 定时器回调函数

新建一个定时器时，定义一个本类型的函数作为回调函数，定时器每次触发，该函数会被调用。
@param[in] pParam 入口参数，其值为调用CSCreateTimer函数时的pvArg参数。
@see CSCreateTimer
*/
typedef void (*TimerCallback)(void* pParam);

/*******************************************************************
 **                         Task definitions                      **
 *******************************************************************/
/**
@brief 创建任务

*创建一个任务，任务间共享逻辑地址，相当于线程的概念。
@param[in] pstrName 任务名称,长度不能超过32个字符，可以为NULL
@param[in] nPriority 任务的优先级,范围为0-255;0优先级最低,255优先级最高
@param[in] nStackSize 任务堆栈大小,如果操作系统支持自动大小任务堆栈管理,建议实现忽略该参数
@param[in] pThreadEntry 任务的入口函数
@param[in] pvArg 任务的入口参数
@param[out] pdwThreadID 输出创建任务的ID，可以为NULL，仅用于调试时唯一确认一个任务，不可用于任何任务操作
@return 如果成功返回任务句柄;如果失败返回为NULL;当没有系统内存,或pThreadEntry = NULL时,创建失败
@note pstrName并不影响创建任务成功与否，即可以为NULL，可以重复,但不推荐这样用
@note 后续所有操作均使用返回的句柄，pdwThreadID仅作唯一标识一个任务之用
*/
HCSHANDLE CSCreateThread (const char* pstrName,int nPriority,int nStackSize, ThreadEntry * pThreadEntry,PVOID pvArg, PDWORD pdwThreadID);

/**
@brief 创建任务扩展接口

CSCreateThread的扩展版本，说明参见CSCreateThread
@param[in] pstrName 任务名称,长度不能超过32个字符，可以为NULL
@param[in] nPriority 任务的优先级,范围为0-255;0优先级最低,255优先级最高
@param[in] nStackSize 任务堆栈大小,如果操作系统支持自动大小任务堆栈管理,建议实现忽略该参数
@param[in] pThreadEntry 任务的入口函数
@param[in] pvArg 任务的入口参数
@param[in] dwInitStatus 任务选项，可选值：
- CS_OSP_TASK_FLAGS_RUN_IMMEDIATELY	任务创建后立即启动
- CS_OSP_TASK_FLAGS_SUSPENDED		任务创建后自动挂起
@param[out] pdwThreadID 输出创建任务的ID，可以为NULL，仅用于调试时唯一确认一个任务，不可用于任何任务操作
@return 如果成功返回任务句柄;如果失败返回为NULL;当没有系统内存,或pThreadEntry = NULL时,创建失败
@note pstrName并不影响创建任务成功与否，即可以为NULL，可以重复,但不推荐这样用
@note 后续所有操作均使用返回的句柄，pdwThreadID仅作唯一标识一个任务之用
@see CSCreateThread
*/
HCSHANDLE CSCreateThreadEx (const char* pstrName,int nPriority, int nStackSize, ThreadEntry * pThreadEntry,PVOID pvArg, DWORD dwInitStatus, PDWORD pdwThreadID);

/**
@brief 删除任务

通过句柄删除一个已经创建的任务
@param[in] hThread 已创建的任务句柄，即CSCreateThread的返回值
@return 如果成功返回CS_OSP_SUCCESS;如果失败返回为CS_OSP_FAILURE;当hThread无效时,删除失败
@note 已自行返回的任务对其首次调用该接口不得返回失败
@note 建议任务入口函数在完成任务后自行返回，然后再调用本接口销毁以释放资源
@note 在任务运行中调用本接口将任务销毁，这个行为是未定义的
*/
DWORD CSDestroyThread (HCSHANDLE hThread);

/**
@brief 挂起任务

通过句柄挂起一个正在运行的任务，使其停止运行
@param[in] hThread 已创建的任务句柄，即CSCreateThread的返回值
@return 如果成功返回CS_OSP_SUCCESS;如果失败返回为CS_OSP_FAILURE;当hThread无效时,删除失败
@note 挂起一个已经挂起的任务仍返回成功
@note 挂起一个已经自行返回的任务返回失败
@deprecated 挂起另外的线程不仅危险，也难以利用posix实现，虽然早期的linuxThread可以做到，但NPTL已经不行了
*/
DWORD CSSuspendThread(HCSHANDLE hThread);

/**
@brief 唤醒已挂起任务

通过句柄唤醒一个已挂起的任务，使其恢复运行
@param[in] hThread 已创建的任务句柄，即CSCreateThread的返回值
@return 如果成功返回CS_OSP_SUCCESS;如果失败返回为CS_OSP_FAILURE;当hThread无效时,删除失败
@note 唤醒一个正在运行的任务仍返回成功
@note 唤醒一个已经自行返回的任务返回失败
@deprecated 同CSSuspendThread
*/
DWORD CSResumeThread(HCSHANDLE hThread);

/**
@brief 使一个任务等待另一个任务结束

@param[in] hThread 已创建的任务句柄，即CSCreateThread的返回值
@return 如果成功返回CS_OSP_SUCCESS;如果失败返回为CS_OSP_FAILURE;
@note 
- 当hThread无效时,立即返回CS_OSP_FAILURE；
- 当被等待任务返回时，该接口返回CS_OSP_SUCCESS；
- 等待一个已经返回但还没有被销毁的任务时，该接口返回CS_OSP_SUCCESS；
- 当多个任务同时调用此接口去等待一个任务时，其结果是未定义的；
- 不允许等待一个已经被销毁的任务。
*/
DWORD CSJoinThread(HCSHANDLE hThread); 

/**
@brief 获取当前任务的任务ID

@return 当前的任务的任务ID
@note 返回值等于CSCreateThread时的pdwThreadID参数，仅用于调试时唯一确认一个任务，不可用于任何任务操作
*/
DWORD CSGetThreadId(void);

/**
@brief 设置任务优先级

通过句柄设置一个任务的优先级
@param[in] hThread 已创建的任务句柄，即CSCreateThread的返回值
@param[in] nPriority 需要设定的任务优先级,范围为0-255;0优先级最低,255优先级最高
@return 如果成功返回CS_OSP_SUCCESS;如果失败返回为CS_OSP_FAILURE;当hThread无效时,删除失败
@note 设置一个已经自行返回的任务的优先级返回失败
*/
DWORD CSSetThreadPriority(HCSHANDLE hThread, int  nPriority);

/**
@brief 获取任务优先级

通过句柄获取一个任务的优先级
@param[in] hThread 已创建的任务句柄，即CSCreateThread的返回值
@param[out] pnPriority 返回任务的优先级,范围为0-255;0优先级最低,255优先级最高
@return 如果成功返回CS_OSP_SUCCESS;如果失败返回为CS_OSP_FAILURE;当hThread无效时,删除失败
@note 一个已经自行返回的任务仍可以获取其优先级
@note 实现可能自行调整任务的优先级，所以得到的并不一定就是设置的值
*/
DWORD CSGetThreadPriority(HCSHANDLE hThread, int* pnPriority);

/**
@brief 挂起当前任务一段时间

@param[in] dwMilliSeconds 挂起的时间，单位是毫秒
*/
void CSSleep(DWORD dwMilliSeconds);


/*******************************************************************
 **                    Message Queue definitions                  **
 *******************************************************************/
/**
@brief 创建消息队列

@param[in] pstrName 消息队列名称,长度不能超过32个字节，可以为NULL
@param[in] nMaxMsgs 消息队列最大容许的消息个数
@param[in] nMaxMsgLen 每一个消息的最大长度,Byte单位
@param[in] dwOptions 保留参数，目前无效，必须传NULL，否则给出断言并返回失败
@return 如果成功返回消息队列句柄;如果失败返回为NULL;当没有系统内存时,或nMaxMsgs=0,或nMaxMsgLen=0 时,创建失败
@note pstrName并不影响创建消息队列成功与否，即可以为NULL，可以重复,但不推荐这样用
*/
HCSHANDLE CSCreateMsgQueue(const char* pstrName,int nMaxMsgs,int nMaxMsgLen, DWORD dwOptions);

/**
@brief 销毁消息队列

通过句柄销毁一个消息队列
@param[in] hMsgQueue 消息队列句柄，即CSCreateMsgQueue的返回值
@return 如果成功返回:CS_OSP_SUCCESS;如果失败返回:CS_OSP_FAILURE;当hMsgQueue无效时,删除失败
@note 删除一个正在被等待或者发送消息的消息队列，结果是不确定的
*/
DWORD CSDestroyMsgQueue(HCSHANDLE hMsgQueue);

/**
@brief 从消息队列接收一条消息

从消息队列接收一条消息，如果消息队列为空则等待直到消息队列中有消息或者超时为止
@param[in] hMsgQueue 消息队列句柄，即CSCreateMsgQueue的返回值
@param[out] pvMsg 接收消息数据的内存地址
@param[in] nMaxMsgBytes 地址分配的内存长度
@param[in] dwTimeout 超时时间,毫秒单位，单位为毫秒;为CS_OSP_TIMEOUT_INFINITY时永久不超时
@return 如果成功返回:CS_OSP_SUCCESS;如果超时返回:CS_OSP_TIMEOUT;如果失败返回:CS_OSP_FAILURE
@note 当hMsgQueue无效时,或pvMsg无效,或nMaxMsgBytes==0时,返回失败,同时不从消息对列中读取消息
@note 当nMaxMsgBytes小于消息长度时,取有效部分消息数据，同时返回成功，消息从队列中取走
*/
DWORD CSReceiveMsg(HCSHANDLE hMsgQueue,void* pvMsg,int nMaxMsgBytes,DWORD dwTimeout);

/**
@brief 往消息队列里发送一条消息

往消息队列里发送一条消息，如果消息队列已满则等待直到消息队列中有空余位置或者超时为止
@param[in] hMsgQueue 消息队列句柄，即CSCreateMsgQueue的返回值
@param[in] pvMsg 发送消息数据的内存地址
@param[in] nMsgBytes 地址分配的内存长度
@param[in] dwTimeout 超时时间,毫秒单位，单位为毫秒;为CS_OSP_TIMEOUT_INFINITY时永久不超时
@return 如果成功返回:CS_OSP_SUCCESS;如果超时返回:CS_OSP_TIMEOUT;如果失败返回:CS_OSP_FAILURE
@note 当hMsgQueue无效时,或pvMsg无效,或nMsgBytes==0时,返回失败
@note 当nMsgBytes大于最大长度时,取有效部分消息数据，同时返回成功
@note 消息队列已拷贝模式工作，pvMsg在本函数返回后即可释放
*/
DWORD CSSendMsg(HCSHANDLE hMsgQueue, const PVOID pvMsg, int nMsgBytes, DWORD dwTimeout);

/**
@brief 获取消息队列中消息的总数量

通过消息队列句柄获取消息队列中消息的总数量
@param[in] hMsgQueue 消息队列句柄，即CSCreateMsgQueue的返回值
@return 如果成功返回>=0;如果失败返回-1;当hMsgQueue无效时返回-1;
*/
int CSGetMsgNum(HCSHANDLE hMsgQueue);


/*******************************************************************
 **                     Semaphore definitions                     **
 *******************************************************************/
/**
@brief 创建信号量

@param[in] pstrName 信号量名称,长度不能超过32个字符
@param[in] nInitialCount 有效信号数量，相当于PV原语中的初始值
@param[in] nMaxCount 最大信号数量
@return 如果成功返回信号量句柄;如果失败返回为NULL
@note pstrName并不影响创建信号量成功与否，即可以为NULL，可以重复,但不推荐这样用
@note 有多个任务等待同一个信号量时，一旦该信号量被释放，不确定哪个任务可以成功获得信号量
*/
HCSHANDLE CSCreateSemaphore(const char* pstrName,int nInitialCount,int nMaxCount);

/**
@brief 销毁信号量

@param[in] hSemaphore 信号量句柄
@return 如果hSemaphore有效返回CS_OSP_SUCCESS;如果hSemaphore为无效句柄返回CS_OSP_FAILURE
@warning 销毁一个正在被操作（如等待）的信号量是一个不确定的行为
*/
DWORD CSDestorySemaphore(HCSHANDLE hSemaphore);

/**
@brief 等待信号量，相当于P操作

@param[in] hSemaphore 信号量句柄
@param[in] dwTimeout 等待超时时间，毫秒单位，也可选用:
- CS_OSP_TIMEOUT_IMMEDIATE	不等待，马上超时
- CS_OSP_TIMEOUT_INFINITY	永远不超时
@return 如果hSemaphore无效返回CS_OSP_FAILURE;如果hSemaphore有效，如果超时返回CS_OSP_TIMEOUT;如果成功返回CS_OSP_SUCCESS
*/
DWORD CSWaitForSemaphore(HCSHANDLE hSemaphore,DWORD dwTimeout);

/**
@brief 释放信号量，相当于V操作

@param[in] hSemaphore 信号量句柄
@param[in] nReleaseCount 释放信号量的次数
@return 成功返回CS_OSP_SUCCESS;失败返回CS_OSP_FAILURE
- 如果hSemaphore无效或者lReleaseCount为0返回CS_OSP_FAILURE
- 如果hSemaphore有效,lReleaseCount>=1且每次释放成功返回CS_OSP_SUCCESS
- 如果lReleaseCount>1但有多于1次没有释放成功返回CS_OSP_FAILURE
*/
DWORD CSReleaseSemaphore(HCSHANDLE hSemaphore, int nReleaseCount);


/*******************************************************************
 **                     Mutex definitions                         **
 *******************************************************************/
/**
@brief 创建互斥量

@param[in] pstrName 互斥量名称,长度不能超过32个字符
@param[in] dwFlags 最大信号数量
- CS_OSP_MUTEX_OBTAIN 创建互斥量后立即加锁
- CS_OSP_MUTEX_NOTOBTAIN 创建互斥量后不立即加锁
@return 如果成功返回信号量句柄;如果失败返回为NULL
@note pstrName并不影响创建互斥两成功与否，即可以为NULL，可以重复,但不推荐这样用
@note 同一任务在已经对一个互斥量加锁后，可再次对其加锁
@note 一个任务对一个互斥量加锁后，其它任务对其进行开锁将返回失败
@note 以上两点属于高级特性，不建议程序逻辑依赖之
@note 有多个任务等待同一个互斥量时，一旦该互斥量被释放，不确定哪个任务可以成功加锁
*/
HCSHANDLE CSCreateMutex(const char* pstrName, DWORD  dwFlags);

/**
@brief 销毁互斥量

@param[in] hMutex 互斥量句柄
@return 如果hMutex有效返回CS_OSP_SUCCESS;如果hMutex无效返回CS_OSP_FAILURE
@warning 销毁一个正在被操作（如等待）的互斥量是一个不确定的行为
*/
DWORD CSDestroyMutex(HCSHANDLE hMutex);

/**
@brief 等待互斥量，即加锁操着

@param[in] hMutex 互斥量句柄
@param[in] dwTimeout 等待超时时间，毫秒单位，也可选用:
- CS_OSP_TIMEOUT_IMMEDIATE	不等待，马上超时
- CS_OSP_TIMEOUT_INFINITY	永远不超时
@return 如果hMutex无效返回CS_OSP_FAILURE;如果hMutex有效，如果超时返回CS_OSP_TIMEOUT;如果成功返回CS_OSP_SUCCESS
*/
DWORD CSWaitForMutex(HCSHANDLE hMutex,DWORD dwTimeout);

/**
@brief 释放互斥量，即开锁操作

@param[in] hMutex 互斥量句柄
@return 如果hMutex无效返回CS_OSP_FAILURE;如果hMutex有效返回CS_OSP_SUCCESS
*/
DWORD CSReleaseMutex(HCSHANDLE hMutex);


/*******************************************************************
 **                     Event definitions                         **
 *******************************************************************/
/**
@brief 创建事件

@param[in] pstrName 事件名称,长度不能超过32个字符
@param[in] dwFlags 事件选项,标志可同时使用,也可分别使用
- CS_OSP_EVENT_MANUAL_RESET CSWaitForSingleEvent之后事件自动无效
- CS_OSP_EVENT_INITIAL_STATUS 初始化后事件立即有效，即马上可以Wait到
@return 如果成功返回事件句柄;如果失败返回CS_OSP_FAILURE
@note pstrName并不影响创建事件成功与否，即可以为NULL，可以重复,但不推荐这样用
@note 有多个任务等待同一个事件时，一旦该事件有效，不确定哪个任务(先)等到事件
*/
HCSHANDLE CSCreateEvent(const char* pstrName,DWORD  dwFlags);

/**
@brief 销毁事件

@param[in] hEvent 事件句柄
@return 如果hEvent有效返回CS_OSP_SUCCESS;如果hMutex无效返回CS_OSP_FAILURE
@warning 销毁一个正在被操作（如等待）的事件是一个不确定的行为
*/
DWORD CSDestroyEvent(HCSHANDLE hEvent);

/**
@brief 设置事件无效

事件无效后，CSWaitForSingleEvent将挂起直到事件有效或者超时
@param[in] hEvent 事件句柄
@return 如果成功返回CS_OSP_SUCCESS;如果失败返回CS_OSP_FAILURE
*/
DWORD CSResetEvent(HCSHANDLE hEvent);

/**
@brief 设置事件有效

事件有效后，CSWaitForSingleEvent将立即返回
@param[in] hEvent 事件句柄
@return 如果成功返回CS_OSP_SUCCESS;如果失败返回CS_OSP_FAILURE
*/
DWORD CSSetEvent(HCSHANDLE hEvent);

/**
@brief 等待事件

事件有效时立即返回;无效时挂起任务直到事件有效或者超时
@param[in] hEvent 事件句柄
@param[in] dwTimeout 等待超时时间，毫秒单位，也可选用:
- CS_OSP_TIMEOUT_IMMEDIATE	不等待，马上超时
- CS_OSP_TIMEOUT_INFINITY	永远不超时
@return 如果hEvent无效返回CS_OSP_FAILURE;如果hEvent有效，如果超时返回CS_OSP_TIMEOUT;如果成功返回CS_OSP_SUCCESS
*/
DWORD CSWaitForSingleEvent(HCSHANDLE hEvent,DWORD dwTimeout);

/**
@brief 初始化定时器

@return 如果初始化成功返回CS_OSP_SUCCESS;如果初始化失败返回CS_OSP_FAILURE
@note 只允许初始化一次
@note 这是一个Coship专用接口，只可在应用初始化时使用，不允许在中间件调用
*/
DWORD  CSInitTimer(void);

/**
@brief 创建定时器

@param[in] enmTimerMode 定时器工作模式
- TIM_MODE_REPEAT 重复模式，即每指定时间触发一次
- TIM_MODE_ONE_SHOT 单次模式选项，即仅在指定时间后触发一次
@param[in] pCallback 定时器触发的回调函数
@param[in] pvArg 定时器回调函数的参数
@param[out] phTimer 定时器句柄
@return 如果初始化成功返回CS_OSP_SUCCESS;如果初始化失败返回CS_OSP_FAILURE
@note 定时器的精度必须在100ms以上
@note 请尽快使定时器回调函数返回，否则对其它定时器产生影响
@note 定时器回调函数中，仍可以调用定时器函数进行操着
*/
DWORD CSCreateTimer(TIMER_MODE enmTimerMode, TimerCallback pCallback, PVOID pvArg, HCSHANDLE *phTimer);

/**
@brief 启动一个定时器

启动定时器，约dwMilliSeconds毫秒后，将触发对应的回调函数
@param[in] hTimer 定时器句柄
@param[in] dwMilliSeconds 定时时间，单位毫秒
@return 如果初始化成功返回CS_OSP_SUCCESS;如果初始化失败返回CS_OSP_FAILURE
@note 启动一个已经启动的定时器将重置计时时间，并返回成功
*/
DWORD CSStartTimer(HCSHANDLE hTimer, DWORD dwMilliSeconds);

/**
@brief 停止一个定时器

停止一个定时器，回调函数将不再被触发
@param[in] hTimer 定时器句柄
@return 如果初始化成功返回CS_OSP_SUCCESS;如果初始化失败返回CS_OSP_FAILURE
@note 停止一个已经停止的定时器仍可返回成功
*/
DWORD CSStopTimer(HCSHANDLE hTimer);

/**
@brief 重置一个定时器

重置一个定时器，定时时间在调用后重新开始记录
@param[in] hTimer 定时器句柄
@return 如果初始化成功返回CS_OSP_SUCCESS;如果初始化失败返回CS_OSP_FAILURE
@note 重置一个已经停止的定时器仍返回成功，但这个操着并无意义
*/
DWORD CSResetTimer(HCSHANDLE hTimer);

/**
@brief 销毁一个定时器

销毁一个定时器，销毁后该定时器不再有效，也无法再次启动
@param[in] hTimer 定时器句柄
@return 如果初始化成功返回CS_OSP_SUCCESS;如果初始化失败返回CS_OSP_FAILURE
*/
DWORD CSDestroyTimer(HCSHANDLE hTimer);

/**
@brief 获取自系统启动后经过的时间毫秒数

@return 返回自系统启动后经过的时间毫秒数
*/
DWORD CSGetTickCount(void);

/**
@brief 设置系统时间

设置系统时间，之后会自动计时以保证时间是准确的
@param[in] dwSeconds 单位是秒，自1970年1月1日起至现在的总秒数。
@return 如果成功返回TRUE;如果失败返回FALSE
*/
BOOL CSSetSoftClock(DWORD dwSeconds);

/**
@brief 获取当前系统时间

@param[out] pdwSeconds 单位是秒，自1970年1月1日起至现在的总秒数。
@return 如果成功返回TRUE;如果失败返回FALSE
@note 在未设置时间之前，返回的时间是不确定的
*/
BOOL CSGetSoftClock(DWORD* pdwSeconds);

/**
@brief 设置系统时间

以另一种单位来设置系统时间. 与CSSetSoftClock效果相同
@param[in] pDateTime 时间数据结构，以年月日时分秒表示
@return 如果成功返回TRUE;如果失败返回FALSE
*/  	
BOOL CSSetSoftTime(TSYSTEMTIME* pDateTime);

/**
@brief 获取系统时间

以另一种单位来获取系统时间. 与CSGetSoftClock效果相同
@param[in] pDateTime 时间数据结构，以年月日时分秒表示
@return 如果成功返回TRUE;如果失败返回FALSE
*/ 
BOOL CSGetSoftTime(TSYSTEMTIME* pDateTime);


#ifndef USE_DBG_MALLOC /*此定义需根据需要在此文件中手动添加*/
/*******************************************************************
 **                     Memory definitions                        **
 *******************************************************************/
/**
@brief 分配内存，但不初始化

相当于malloc，新分配的内存其内容是不确定的
@param[in] nMemorySize 待分配的内存大小
@return 如果成功返回内存指针;如果失败返回为NULL
@note nMemorySize等于0时的返回值是不确定的，可以是0也可以是是一个地址
*/
PVOID CSMalloc( unsigned int nMemorySize );

/**
@brief 重分配内存

相当于realloc，重新分配内存为新的大小，并拷贝尽量多的内容，如果新内存空间比旧的大，则新增部分的内容是不确定的
@param[in] pvOldMemoryAddr 待重新分配的内存地址
@param[in] nMemorySize 待分配的内存大小
@return 如果成功返回内存指针;如果失败返回为NULL
@note nMemorySize等于0时的返回值是不确定的，可以是0也可以是是一个地址
如果pvOldMemoryAddr不是一个之前CSMalloc,CSRealloc,CSCalloc返回的地址，结果是不确定的
如果pvOldMemoryAddr==NULL，功能与CSMalloc一样
如果分配失败，旧的内存不会自动释放，其内容也不发生变化
返回值可能与pvOldMemoryAddr不一致，也可能一致
*/
PVOID CSRealloc( PVOID pvOldMemoryAddr,unsigned int nMemorySize );

/**
@brief 分配内存，并初始化

相当于calloc，分配nElements个nElementSize大小的objects,并将内存全部设为0
@param[in] nElements 待分配的元素个数
@param[in] nElementSize 待分配的每个元素的大小
@return 如果成功返回内存指针;如果失败返回为NULL
@note nElements或nElementSize等于0时的返回值是不确定的，可以是0也可以是是一个地址
*/
PVOID CSCalloc(unsigned int nElements, unsigned int nElementSize);

/**
@brief 释放分配的内存

相当于free，释放CSMalloc,CSRealloc,CSCalloc分配的内存
@param[in] pvMemoryAddr 待释放的内存地址
@return 如果成功返回TRUE;如果失败返回FALSE
@note 如果pvMemoryAddr不是一个之前CSMalloc,CSRealloc,CSCalloc返回的地址，结果是不确定的
@note 多次释放同一段内存将返回失败
*/
BOOL CSFree( PVOID pvMemoryAddr );

/**
@brief Dump 内存使用情况

@return 无
@note 
@note 
*/
#define CSMemDump() {}
#else
/*此文件专门用于调试*/
#include "cs_memleak.h"
#endif

/**
@brief 获取可用的RAM空间
@param[out] pnFreeCount 系统可用的RAM空间
@return 如果初始化成功返回CS_OSP_SUCCESS，失败则返回CS_OSP_FAILURE。
*/
DWORD CSGetFreeRAM(unsigned int * puFreeCount);

/**
@brief 获取CPU个数
@param[out] pnCpuCount CPU的个数
@return 获取成功则返回CS_OSP_SUCCESS，失败则返回CS_OSP_FAILURE。
*/
DWORD CSGetCPUCount(unsigned int * puCpuCount);


/**
@brief 获取指定索引CPU的处理器利用率(%)
@param[out] pnUsage CPU处理器利用率，取值范围为0--1000;
@return 获取成功则返回CS_OSP_SUCCESS，失败则返回CS_OSP_FAILURE。
*/
DWORD CSGetCPUUsage(unsigned int index, unsigned int * puUsage);

/**
@brief 获取系统中所有任务的句柄
@param[out]  hThread 系统中所有任务的句柄数组
@param[in/out]   nSize hThread数组的大小，返回实际Thread的个数
@return 获取成功则返回CS_OSP_SUCCESS，失败则返回CS_OSP_FAILURE。
*/
DWORD CSGetAllThreadHandle(HCSHANDLE* phThread, unsigned int*  puSize);

/**
@brief 获取指定任务的基本信息，包括任务名称、任务ID、任务优先级
@param[in]   hThread 指定任务的句柄
@param[out]  psThreadInfo 任务的基本信息
@return 获取成功则返回CS_OSP_SUCCESS，psThreadInfo不为NULL，失败则返回CS_OSP_FAILURE。
*/
DWORD CSGetThreadInfo(HCSHANDLE hThread, CSThreadInfo_S* psThreadInfo);


/**
@brief 获取指定任务的RAM消耗
@param[in]   hThread 指定任务的句柄
@param[out]  pnUsedRam 指定任务的RAM消耗
@return 获取成功则返回CS_OSP_SUCCESS，失败则返回CS_OSP_FAILURE。
*/
DWORD CSGetThreadUsedRAM(HCSHANDLE hThread, unsigned int * puUsedRam);


/**
@brief 获取指定任务的CPU利用率(%)
@param[in]   hThread 指定任务的句柄
@param[out] pnUsage 指定任务的CPU负载，取值范围为:0--1000
@return 获取成功则返回CS_OSP_SUCCESS，失败则返回CS_OSP_FAILURE。
*/
DWORD CSGetThreadCPUUsage(HCSHANDLE hThread, unsigned int * puUsage);

//add ended
#ifdef  __cplusplus
}
#endif

/** @} */

#endif
