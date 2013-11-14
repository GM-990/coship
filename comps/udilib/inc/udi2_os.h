/**@defgroup OS OS 定义了操作系统相关的一系列操作，如任务、信号量等。

@brief OS模块对应头文件<udi2_os.h>，定义了操作系统相关的一系列操作，如任务、信号量
等。其内容主要包括任务部分、消息队列部分、信号量部分、互斥量部分、时间部分、事件
部分和内存部分。

@section  udi2 os与pthread的关系:
udi2 os实现上可依赖于pthread,也可不依赖于pthread，不做强制要求。建议在底层有pthread支持的，
os实现内部调用pthread。

但是，mmcp是依赖于pthread的。平台必须提供pthread的实现。如果平台没有pthread，则mmcp
依然无法正常运行。在没有pthread的平台上，coship可以提供pthread的库，也可采用第三方
自己实现的pthread。但是，无论谁实现pthread， 必须保证，在调用其他udi接口之前，pthread
已经可用，这就要求在udi_init()中初始化pthread. 在第三方集成中，由于udi接口是合作公司
实现的，因此，应该向他们提供pthread初始化接口以便在udi_init中调用.

@par 任务部分
任务部分包括创建、销毁、挂起、等待任务等接口。其意义基本相当于OS中的线程概念，即
所有任务共享地址空间。需要特别注意的是随意销毁一个任务是非常危险的，正确的做法是：\n
1、先发消息通知被销毁任务。\n
2、被销毁任务接到该消息后释放所有资源（如信号量、消息队列等），并将任务入口函数返回。\n
3、等待Join函数返回成功后，再调用Destroy函数将其销毁。

@note
- 试图去销毁一个正在运行的任务（即任务入口函数还没有返回的任务）时，会返回CSUDIOS_ERROR_THREAD_BUSY。
- 在本任务中销毁本任务是不被允许的。

@note 另外，对本部分的Suspend、Resume接口以及Yield接口做如下说明：
- Suspend和Resume接口仅用于调试，目前我们发现在部分操作系统上难以实现，如基于NPTL线
程库Linux系统，这种情况下这两个接口可不实现，返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
即可。但同时我们仍推荐尽量实现这两个接口，以方便问题的调试。
- Yield接口用于让任务放弃本次调度的时间片，以防止单一高优先级任务长时间抢占CPU资源而
造成其它任务无法执行。虽然大多数情况下，Sleep一小会儿时间也能达到同样的效果，但不同的
平台这个Sleep时间的极限值是不同的，例如：有些平台这个时间设为0即可启动Sleep操作，
而有些平台却需要设置这个时间超过一个最小值，（如BCM7403的最小值约为3ms）才可启动
Sleep操作，否则就会忽略。为了屏蔽这些平台的差异，所以定义了此接口。

@par 消息队列部分
消息队列部分包括创建、销毁消息队列及发送、接收消息等接口。消息队列是任务间传送消息
及少量数据的最好选择，下图演示了消息队列的基本功能。消息在消息队列是以浅拷贝形式存在
的，不建议通过消息队列传送大量数据，如Section等。虽然单个消息队列的最大消息数量及消
息长度不会太大，但仍建议采用限制所有消息队列总内存或者直接使用系统堆内存的方式限制消
息队列的数量。
@image HTML udi_os_msgq.png

@par 信号量部分
信号量部分包括创建、销毁、等待、释放信号量等接口。用于在多任务间协调各个线程, 以保证
它们能够正确、合理的使用公共资源。\n
单值信号量与互斥量之间是有语义上的区别的。与互斥量相比，单值信号量表示一种资源，如果
资源只有一个且已被这一个任务获取，除非该任务释放该资源，否则任何任务（包括已经获取到
这个资源的任务）均无法获取该资源。
@note 有多个任务等待同一个信号量时，一旦该信号量被释放，不确定哪个任务可以成功获得该
信号量。

@par 互斥量部分
互斥量部分包括创建、销毁、获取、释放互斥量等接口。这里我们将互斥量定义为嵌套锁的概念，
其本质相当于一把锁，提供对共享资源的访问保护。\n
与单值信号量相比，互斥量表示一种资源的所有权状态，当一个任务获取了某个资源的使用权限
之后，除非该任务释放该权限，否则其它任何任务均无法获取该权限；但对于这个任务本身，如
果想重申自己拥有该权限是没有问题的。即同一任务在已经对一个互斥量加锁后，可再次对其加
锁，但多次加锁后需要多次释放才能释放该锁。\n
@note 有多个任务等待同一个互斥量时，一旦该互斥量被释放，不确定哪个任务可以成功加锁。

@par 时间部分
时间部分仅包括一个获取系统启动后经过的时间接口。实际上Eastwin仅关心两次使用该接口获得
时间的差值，用于了解两次调用之间过去了多长时间，而这个时间的0时刻是否是从开机的一瞬间
并不重要。请在不太影响系统效率的前提下，尽量提供更高精度的时间信息，推荐精确到CPU Tick
级别，最少精确到毫秒级别，否则可能会对系统产生不可预料的影响。\n\n

@par 事件部分
事件部分包括创建、销毁、设置、重置、等待事件等接口。顾名思义，事件用于表示一些事情的
发生或未发生。比如对于异步的锁频操作，我们可以先创建一个Event，然后在调用异步锁频接口
后去等待这个Event，而在锁频成功的回调中Set这个Event。
一般来说这种方式是同步异步事件的最佳选择。虽然使用信号量、消息队列等也可也达到类似的
作用，甚至在部分平台Event就是通过信号量或者消息队列来实现的，但它们之间仍存在语义上的
差异，分别适用于不同的情况。\n
事件工作模式分如下两种情况：
- 如果创建时CSUDIOS_EVENT_AUTO_RESET选项被设置，则事件在发生并在任何位置被捕获后，立
刻自动无效，也就是说如果一个Event有两个位置在等待，但只被Set过一次，这种情况下只有一个
位置可以等到该事件发生。
- 如果创建时CSUDIOS_EVENT_AUTO_RESET选项没有被设置，则事件发生后除非被主动Reset，否则
永远有效。也就是说如果一个Event有多个位置在等待，且被Set过一次后，这种情况下，所有位置
都可以等到该事件发生。

@par 内存部分
内存部分包括Malloc、Realloc、Calloc、Free等标准C函数接口。Calloc理论上效率会比
Malloc+memset高一点，虽然必要性不是特别大，但鉴于大多数平台都有这个接口，所以封装一下也
非常容易。Realloc在平台无法提供的情况下，可以使用Malloc+memcpy+Free来实现，但推荐尽量使
用平台原生接口实现，因为在很多情况下，Realloc可以在原分配内存的基础上增加一段而非完全重
新分配，效率会高的多，也不容易产生内存碎片。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.3 2009/09/15 修正一些语言描述错误和拼写错误
@version 2.0.2 2009/09/07 增加yield接口
@version 2.0.2 2009/08/24 第一次综合评审后修改的版本
@version 2.0.1 2009/08/16 演示版本
@{
*/

#ifndef _UDI2_OS_H_
#define _UDI2_OS_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDIOS_TIMEOUT_IMMEDIATE  		(0)			///< 超时参数之立即超时选项

#define CSUDIOS_TIMEOUT_INFINITY   		(~0)		///< 超时参数之永不超时选项

#define CSUDIOS_EVENT_AUTO_RESET		(1 << 0)	///< CSUDIOSEventWait之后事件自动无效选项

#define CSUDIOS_EVENT_INITIAL_STATUS	(1 << 1)	///< 初始化事件后立即有效选项

#define CSUDIOS_MUTEX_DEFAULT			(0)			///< 创建互斥量时，使用默认选项

#define	CSUDIOS_MUTEX_OBTAIN			(1 << 0)	///< 创建互斥量后立即获取到锁选项
#define CSUDIOS_THREAD_NAME_MAX_LEN    32

/**@brief OS 相关错误代码值*/
enum
{
	CSUDIOS_ERROR_BAD_PARAMETER = CSUDI_OS_ERROR_BASE,	///< 参数错误
	CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED,				///< 不支持此操作
	CSUDIOS_ERROR_UNKNOWN_ERROR,						///< 一般错误
	CSUDIOS_ERROR_NO_MEMORY,							///<	 无内存可用错误
	CSUDIOS_ERROR_TIMEOUT,								///< 超时错误
	CSUDIOS_ERROR_THREAD_BUSY									///< 任务在运行中

};

/**@brief 时间相关结构体*/
typedef struct 
{
	int  m_nSecond;				///< 秒
	int  m_nMiliSecond;			///< 毫秒
	long m_lUSecond;            ///< 微秒，即一百万分之一秒
}CSUDIOSTimeVal_S;

/**@brief 任务的基本信息结构体*/
typedef struct
{
	unsigned int uThreadId;	///<任务ID
	unsigned int uPriority;		///<任务优先级
	char szThreadName[CSUDIOS_THREAD_NAME_MAX_LEN];	///<任务名称
}CSUDIOSThreadInfo_S;

/*******************************************************************
**                         Task definitions                       **
**    考虑到删除,挂起,设置优先级等操作的不安全性,建议删除这些接口
 *******************************************************************/

/**
@brief 任务入口函数

新建一个任务时，定义一个本类型的函数作为入口函数,任务启动后，该函数会在新任务中被调用.
@param[in] pvParam 入口参数，其值为调用CSUDIOSThreadCreate函数时的pvArg参数.
@see CSUDIOSThreadCreate
*/
typedef void (*CSUDIOSThreadEntry_F)(void * pvParam);

/**
@brief 创建任务

*创建一个任务,任务间共享逻辑地址,相当于线程的概念。
@param[in] pcName 任务名称,长度不能超过32字节,可以为CSUDI_NULL，如果长度超过32个字节则返回CSUDIOS_ERROR_BAD_PARAMETER，并且任务创建失败
@param[in] nPriority 任务的优先级,范围为0-255;0优先级最低,255优先级最高。如果超过最大优先级则返回CSUDIOS_ERROR_BAD_PARAMETER,并且任务创建失败
@param[in] nStackSize 任务堆栈大小,至少支持512KB的大小。如果操作系统支持自动大小任务堆栈管理,建议实现忽略该参数,但使用者必须给出该参数以适应不支持自动栈大小的系统
@param[in] fnThreadEntry 任务的入口函数
@param[in] pvArg 任务的入口参数
@param[out] phThread 生成的任务句柄,后续操作使用该句柄
@return 成功返回CSUDI_SUCCESS;如果失败返回错误代码值，且phThread为CSUDI_NULL
@note 
- pcName并不影响创建任务成功与否,即可以为CSUDI_NULL,可以重复,但不推荐这样用
- 后续对该任务的所有操作均使用phThread
- 线程名称应允许空格

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadCreate(const char * pcName,int nPriority,int nStackSize, CSUDIOSThreadEntry_F fnThreadEntry,void * pvArg,CSUDI_HANDLE * phThread);

/**
@brief 删除任务

通过句柄删除一个已经创建的任务
@param[in] hThread 已创建的任务句柄，即CSUDIOSThreadCreate的输出参数phThread
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值;当hThread无效时,删除失败
@note 
- 要求任务入口函数在完成任务后自行返回，然后再调用本接口销毁以释放资源
- 不允许在本任务中销毁本任务
- 试图去销毁一个正在运行的任务（即任务入口函数还没有返回的任务）时，会返回CSUDIOS_ERROR_TASK_BUSY。
@see  CSUDIOSThreadJoin

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadDestroy (CSUDI_HANDLE hThread);

/**
@brief 挂起任务

通过句柄挂起一个正在运行的任务，使其停止运行
@param[in] hThread 已创建的任务句柄，即CSUDIOSThreadCreate的输出参数phThread
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值;当hThread无效时,任务挂起失败
@note 
- 挂起一个已经挂起的任务仍返回成功
- 挂起一个已经自行返回的任务返回失败
@deprecated 此接口仅用于调试，如果平台不支持，可不实现，此时此接口返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadSuspend(CSUDI_HANDLE hThread);

/**
@brief 唤醒已挂起任务

通过句柄唤醒一个已挂起的任务，使其恢复运行
@param[in] hThread 已创建的任务句柄，即CSUDIOSThreadCreate的输出参数phThread
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值;当hThread无效时，唤醒任务失败
@note 
- 唤醒一个正在运行的任务仍返回成功
- 唤醒一个已经自行返回的任务返回失败
@deprecated 此接口仅用于调试，如果平台不支持，可不实现，此时此接口返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadResume(CSUDI_HANDLE hThread);

/**
@brief 使一个任务等待另一个任务结束

@param[in] hThread 已创建的任务句柄，即CSUDIOSThreadCreate的输出参数phThread
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 
- 当被等待任务返回时，该接口返回CSUDI_SUCCESS。
- 等待一个已经返回但还没有被销毁的任务时，该接口返回CSUDI_SUCCESS。
- 当多个任务同时调用此接口去等待一个任务时，其结果是未定义的。
- 不允许等待一个已经被销毁的任务。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadJoin (CSUDI_HANDLE hThread); 

/**
@brief 获取当前任务的句柄
@param[out] phThread 当前的任务的句柄，即CSUDIOSThreadCreate的输出参数phThread
@return  如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值;

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIOSThreadSelf(CSUDI_HANDLE * phThread);

/**
@brief 挂起当前任务一段时间

@param[in] uMilliSeconds 挂起的时间，单位是毫秒
@note 
该接口强制当前任务挂起至少uMilliSeconds毫秒，如果指定挂起的时间uMilliSeconds为0毫秒时，则由平台决定是以下哪种行为：
- 放弃本次执行权。
- 不关注该值。 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void CSUDIOSThreadSleep(unsigned int uMilliSeconds);

/**
@brief 使任务放弃本次执行的机会

即放弃本次执行权。
@note 
- 该接口调用后立即生效并立即返回。
- 在多任务环境(程序)中，为了防止某任务独占CPU资源(此时其它任务无法得到响应)，可以让当前执行的任务放弃本次执行的机会。
- 调用该接口并不能保证下一个运行的任务不是该任务。即如果该接口返回时，该任务再次获得执行权，那么被该接口暂停的任务会马上开始运行。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void CSUDIOSThreadYield (void); 

/*******************************************************************
 **                    Message Queue definitions                  **
 *******************************************************************/
/**
@brief 创建消息队列

@param[in] pcName 消息队列名称,长度不能超过32个字节，可以为CSUDI_NULL；如果长度超过32个字节则返回CSUDIOS_ERROR_BAD_PARAMETER，并且消息队列创建失败
@param[in] nMaxMsgs 消息队列最大容许的消息个数，一般来说不会超过128个。至少容许一个消息，否则消息队列创建失败
@param[in] nSizePerMsg 每一个消息的最大长度,单位字节，一般来说不会超过128字节。至少支持一个字节，否则消息队列创建失败
@param[out] phMsgQueue 生成的消息队列句柄
@return 成功返回CSUDI_SUCCESS;失败返回错误代码值，且phMsgQue为CSUDI_NULL；
@note 
- pcName并不影响创建消息队列成功与否,即可以为CSUDI_NULL,可以重复,但不推荐这样用
- 虽然单个消息队列的最大消息数量及消息长度不会太大，但仍建议采用限制所有消息队列总内存或者直接使用系统堆内存的方式限制消息队列的数量
@note 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueCreate(const char * pcName,int nMaxMsgs,int nSizePerMsg,CSUDI_HANDLE * phMsgQueue);

/**
@brief 销毁消息队列

通过句柄销毁一个消息队列
@param[in] hMsgQueue 消息队列句柄，即CSUDIOSMsgQueueCreate的输出参数phMsgQueue。
@return 成功返回CSUDI_SUCCESS;失败返回错误代码值
@warning 删除一个正在被等待或者发送消息的消息队列,结果是不确定的,请慎用该接口

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueDestroy(CSUDI_HANDLE hMsgQueue);

/**
@brief 从消息队列接收一条消息

从消息队列里接收一条消息，如果消息队列为空则等待直到消息队列中有消息或者超时为止
@param[in] hMsgQueue 消息队列句柄，即CSUDIMsgQueueCreate的输出参数phMsgQueue
@param[out] pvMsg 接收消息数据的内存地址
@param[in] nMaxMsgBytes 地址分配的内存长度
@param[in] uTimeout 等待超时时间，毫秒单位，也可选用:
- CSUDIOS_TIMEOUT_IMMEDIATE	不等待，马上超时
- CSUDIOS_TIMEOUT_INFINITY	永远不超时
@return 成功返回CSUDI_SUCCESS;超时返回CSUDIOS_ERROR_TIMEOUT;失败返回错误代码
@note 
- 当hMsgQueue无效时,或pvMsg无效,或nMaxMsgBytes==0时,返回失败,同时不从消息队列中读取消息
- 当nMaxMsgBytes小于消息长度时,取有效部分消息数据，同时返回成功，消息从队列中取走
- 当nMaxMsgBytes大于消息长度时，消息可完全放入pvMsg,但超出消息长度后部分内容不确定

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueReceive(CSUDI_HANDLE hMsgQueue,void * pvMsg,int nMaxMsgBytes,unsigned int uTimeout);

/**
@brief 往消息队列里发送一条消息

如果消息队列已满则等待直到消息队列中有空余位置或者超时为止
@param[in] hMsgQueue 消息队列句柄，即CSUDIOSMsgQueueCreate的输出参数phMsgQueue
@param[in] pvMsg 发送消息数据的内存地址
@param[in] nMsgBytes 地址分配的内存长度
@param[in] uTimeout 等待超时时间，毫秒单位，也可选用:
- CSUDIOS_TIMEOUT_IMMEDIATE	不等待，马上超时
- CSUDIOS_TIMEOUT_INFINITY	永远不超时
@return 成功返回CSUDI_SUCCESS;超时返回CSUDIOS_ERROR_TIMEOUT;失败返回错误代码
@note 
- 当hMsgQueue无效时,或pvMsg无效,或nMsgBytes==0时,返回失败。
- 当nMsgBytes大于最大长度时,取有效部分消息数据，同时返回成功。
- 消息队列以拷贝模式工作，pvMsg在本函数返回后即可释放。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueSend(CSUDI_HANDLE hMsgQueue, const void * pvMsg, int nMsgBytes, unsigned int uTimeout);




/*******************************************************************
 **                     Semaphore definitions                     **
 *******************************************************************/
/**
@brief 创建信号量

@param[in] pcName 信号量名称,长度不能超过32个字节，如果长度超过32个字节则返回CSUDIOS_ERROR_BAD_PARAMETER，并且信号量创建失败
@param[in] nInitialCount 有效信号数量，相当于PV原语中的初始值
@param[in] nMaxCount 最大信号数量
@param[out] phSemaphore 生成的信号量句柄
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值，且phSem为CSUDI_NULL
@note 
- pcName并不影响创建信号量成功与否，即可以为CSUDI_NULL，可以重复,但不推荐这样用
- 有多个任务等待同一个信号量时,一旦该信号量被释放,不确定哪个任务可以成功获得信号量
- 信号量名称应允许空格
@todo 目前大多数平台上nMaxCount无效，请不要使用，nMaxCount为不限大小

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemCreate(const char * pcName,int nInitialCount,int nMaxCount,CSUDI_HANDLE * phSemaphore);

/**
@brief 销毁信号量

@param[in] hSemaphore 信号量句柄，即CSUDIOSMsgQueueCreate的输出参数phMsgQueue
@return 成功返回CSUDI_SUCCESS;失败返回出错代码
@warning 销毁一个正在被操作（如等待）的信号量是一个不确定的行为

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemDestroy(CSUDI_HANDLE hSemaphore);

/**
@brief 等待信号量，相当于P操作(P是请求资源)

@param[in] hSemaphore 信号量句柄，即CSUDIOSMsgQueueCreate的输出参数phMsgQueue
@param[in] uTimeout 等待超时时间，毫秒单位，也可选用:
- CSUDIOS_TIMEOUT_IMMEDIATE	不等待，马上超时
- CSUDIOS_TIMEOUT_INFINITY	永远不超时
@return 成功返回CSUDI_SUCCESS;超时返回CSUDIOS_ERROR_TIMEOUT;失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemWait(CSUDI_HANDLE hSemaphore,unsigned int uTimeout);

/**
@brief 释放信号量，相当于V操作(V是释放资源）
@param[in] hSemaphore 信号量句柄，即CSUDIOSMsgQueueCreate的输出参数phMsgQueue
@return 成功返回CSUDI_SUCCESS;失败返回出错代码
@note 当信号量被释放次数达到最大个数时，返回CSUDIOS_ERROR_UNKNOWN_ERROR

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemRelease(CSUDI_HANDLE hSemaphore);




/*******************************************************************
 **                     Mutex definitions                         **
 *******************************************************************/
/**
@brief 创建可嵌套的互斥量

@param[in] pcName 互斥量名称,长度不能超过32个字节，如果长度超过32个字节则返回CSUDIOS_ERROR_BAD_PARAMETER，并且信号量创建失败
@param[in] uOptions 互斥量选项,目前支持:(各选项可相或)
- CSUDIOS_MUTEX_OBTAIN 创建互斥量后立即获得该锁(该位为0或CSUDIOS_MUTEX_DEFAULT表示不获得该锁)
@param[out] phMutex 互斥量句柄
@return 成功返回CSUDI_SUCCESS;失败返回出错代码,且phMutex为CSUDI_NULL
@note 
- pcName并不影响创建互斥量成功与否,即可以为CSUDI_NULL,可以重复,但不推荐这样用
- 同一任务在已经对一个互斥量加锁后,可再次对其加锁,即使用嵌套锁,但多次加锁后需要多次释放才能释放该锁。
- 一个任务对一个互斥量加锁后,其它任务对其进行开锁将返回失败。
- 以上两点属于高级特性,不建议程序逻辑依赖之
- 互斥量名称应允许空格
- 有多个任务等待同一个互斥量时,一旦该互斥量被释放,不确定哪个任务可以成功加锁

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexCreate(const char * pcName, unsigned int uOptions,CSUDI_HANDLE * phMutex);

/**
@brief 销毁互斥量

@param[in] hMutex 互斥量句柄
@return 成功返回CSUDI_SUCCESS;失败返回出错代码
@warning 销毁一个正在被操作（如等待）的互斥量是一个不确定的行为

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexDestroy(CSUDI_HANDLE hMutex);

/**
@brief 等待互斥量,即加锁操作

@param[in] hMutex 互斥量句柄
@param[in] uTimeout 等待超时时间,毫秒单位,也可选用:
- CSUDIOS_TIMEOUT_IMMEDIATE	不等待,马上超时
- CSUDIOS_TIMEOUT_INFINITY	永远不超时
@return 成功返回CSUDI_SUCCESS;超时返回CSUDIOS_ERROR_TIMEOUT;失败返回错误代码。 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexWait(CSUDI_HANDLE hMutex,unsigned int uTimeout);

/**
@brief 释放互斥量,即开锁操作

@param[in] hMutex 互斥量句柄
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 试图去释放没有加锁或者已经解锁的互斥量将返回CSUDIOS_ERROR_UNKNOWN_ERROR错误。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexRelease(CSUDI_HANDLE hMutex);

/**
@brief 获取自系统启动后经过的时间

@param[out] psTime 系统启动后经过的时间,详见CSUDIOSTimeVal_S定义。为CSUDI_NULL则返回CSUDIOS_ERROR_BAD_PARAMETER
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 
- 要求精确到操作系统所能提供的最大精度，最少到毫秒,否则可能会对系统产生不可预料的影响!
- 实际上Eastwin仅关心两次使用该接口获得时间的差值，用于获取两次调用的时间间隔，而这个时间的0时刻是否是从开机的一瞬间算起并不重要。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetTime(CSUDIOSTimeVal_S * psTime);




/*******************************************************************
 **                     Event definitions                         **
 *******************************************************************/
/**
@brief 创建事件

@param[in] pcName 事件名称,长度不能超过32个字节，如果长度超过32个字节则返回CSUDIOS_ERROR_BAD_PARAMETER，并且创建事件失败
@param[in] uFlags 事件选项,标志可同时使用,也可分别使用。如果此参数为其它值则返回CSUDIOS_ERROR_BAD_PARAMETER。
@note uFlags 的取值包括(各项可相或):
- CSUDIOS_EVENT_INITIAL_STATUS 初始化后事件立即有效，即马上可以Wait到。(若该位为0则表示事件需要Set后方生效)
- CSUDIOS_EVENT_AUTO_RESET CSUDIOSEventWait之后事件自动变为无效。(若该位为0则表示Wait事件后事件仍然有效，除非主动Reset)

@note 这意味着如果uFlags的取值为:
- CSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUS 初始化后事件立即有效，即马上可以Wait到，Wait之后事件自动变为无效。
- 0 初始化事件之后，需要Set使事件生效；该事件在reset之前永远有效。
@see CSUDIOSEventSet CSUDIOSEventReset
@param[out] phEvent 事件句柄
@return 成功返回CSUDI_SUCCESS;失败返回错误代码,且phEvent为CSUDI_NULL
@note 
- pstrName并不影响创建事件成功与否，即可以为CSUDI_NULL，可以重复,但不推荐这样用
- 有多个任务等待同一个事件时，一旦该事件有效，不确定哪个任务(先)等到事件
- 事件名称应允许空格

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventCreate(const char * pcName,unsigned int  uFlags,CSUDI_HANDLE * phEvent);

/**
@brief 销毁事件

@param[in] hEvent 事件句柄
@return 如果hEvent有效返回CSUDI_SUCCESS;如果hEvent无效则返回CSUDIOS_ERROR_BAD_PARAMETER
@warning 销毁一个正在被操作（如等待）的事件是一个不确定的行为

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventDestroy(CSUDI_HANDLE hEvent);

/**
@brief 设置事件无效

事件无效后，CSUDIOSEventWait将挂起直到事件有效或者超时
@param[in] hEvent 事件句柄
@return 如果成功返回CSUDI_SUCCESS;如果失败返回CSUDI_FAILURE

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventReset(CSUDI_HANDLE hEvent);

/**
@brief 设置事件有效

事件有效后，CSWaitForSingleEvent将立即返回
@param[in] hEvent 事件句柄
@return 如果成功返回CSUDI_SUCCESS;如果失败返回CSUDI_FAILURE

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventSet(CSUDI_HANDLE hEvent);

/**
@brief 等待事件

事件有效时立即返回;无效时挂起任务直到事件有效或者超时
@param[in] hEvent 事件句柄
@param[in] uTimeout 等待超时时间，单位为毫秒，也可选用:
- CSUDIOS_TIMEOUT_IMMEDIATE	不等待，马上超时
- CSUDIOS_TIMEOUT_INFINITY	永远不超时
@return 
	   - 如果成功返回CSUDI_SUCCESS
	   - 如果hEvent错误返回CSUDIOS_ERROR_BAD_PARAMETER;
	   - 在hEvent正确的情况下，如果超时则返回CSUDIOS_ERROR_TIMEOUT。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventWait(CSUDI_HANDLE hEvent,unsigned int uTimeout);


/*******************************************************************
 **                     Memory definitions                        **
 *******************************************************************/
/**
@brief 分配内存,但不初始化

相当于malloc,新分配的内存其内容是不确定的
@param[in] uSize 待分配的内存大小
@return 成功返回内存指针;失败返回CSUDI_NULL
@note uSize等于0时，函数分配失败，且返回值为CSUDI_NULL。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void * CSUDIOSMalloc( unsigned int uSize );

/**
@brief 重新分配内存

相当于realloc,重新分配内存为新的大小,并拷贝尽量多的内容,如果新内存空间比旧的大,则新增部分的内容是不确定的
@param[in] pvAddr 待重新分配的内存地址
@param[in] uSize 待分配的内存大小
@return 成功返回内存指针;失败返回CSUDI_NULL
@note 
- uSize等于0时该函数执行失败,可以是CSUDI_NULL也可以是一个地址。
- 如果pvAddr不是一个之前CSUDIOSMalloc,CSUDIOSRealloc返回的地址,结果是不确定的
- 如果pvAddr==CSUDI_NULL,功能与CSUDIOSMalloc一样
- 如果分配失败,旧的内存不会自动释放,其内容也不发生变化，返回值可能与pvAddr不一致,也可能一致

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void* CSUDIOSRealloc( void * pvAddr,unsigned int uSize );

/**
@brief 分配内存，并初始化

相当于calloc，分配nElements个nElementSize大小的objects,并将内存全部设为0
@param[in] uElements 待分配的元素个数
@param[in] uElementSize 待分配的每个元素的大小
@return 如果成功返回内存指针;如果失败返回为NULL
@note nElements或nElementSize等于0时的返回值是不确定的，可以是CSUDI_NULL也可以是一个地址。 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void* CSUDIOSCalloc(unsigned int uElements, unsigned int uElementSize);

/**
@brief 释放分配的内存

相当于free,释放CSUDIOSMalloc,CSUDIOSRealloc,CSUDIOSCalloc分配的内存
@param[in] pvAddr 待释放的内存地址
@return 如果成功返回CSUDI_SUCCESS;如果失败返回CSUDI_FAILURE
@note 
- 如果pvAddr不是一个之前CSUDIOSMalloc,CSUDIOSRealloc,CSUDIOSCalloc返回的地址,结果是不确定的
- 多次释放同一段内存将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSFree( void * pvAddr );


/**
@brief 获取可用的RAM空间
@param[out] pnFreeCount 系统可用的RAM空间
@return 获取成功则返回CSUDI_SUCCESS，失败则返回CSUDI_FAILURE。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetFreeRAM(unsigned int * puFreeCount);

/**
@brief 获取CPU个数
@param[out] pnCpuCount CPU的个数
@return 获取成功则返回CSUDI_SUCCESS，失败则返回CSUDI_FAILURE。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetCPUCount(unsigned int * puCpuCount);


/**
@brief 获取指定索引CPU的处理器利用率(%)
@param[out] pnUsage CPU处理器利用率，取值范围为0--1000;
@return 获取成功则返回CSUDI_SUCCESS，失败则返回CSUDI_FAILURE。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetCPUUsage(unsigned int index, unsigned int * puUsage);

/**
@brief 获取系统中所有任务的句柄
@param[out]  hThread 系统中所有任务的句柄数组
@param[in/out]   nSize hThread数组的大小，返回实际Thread的个数
@return 获取成功则返回CSUDI_SUCCESS，失败则返回CSUDI_FAILURE。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetAllThreadHandle(CSUDI_HANDLE* phThread, unsigned int*  puSize);

/**
@brief 获取指定任务的基本信息，包括任务名称、任务ID、任务优先级
@param[in]   hThread 指定任务的句柄
@param[out]  psThreadInfo 任务的基本信息
@return 获取成功则返回CSUDI_SUCCESS，psThreadInfo不为NULL，失败则返回CSUDI_FAILURE。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadInfo(CSUDI_HANDLE hThread, CSUDIOSThreadInfo_S* psThreadInfo);


/**
@brief 获取指定任务的RAM消耗
@param[in]   hThread 指定任务的句柄
@param[out]  pnUsedRam 指定任务的RAM消耗
@return 获取成功则返回CSUDI_SUCCESS，失败则返回CSUDI_FAILURE。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadUsedRAM(CSUDI_HANDLE hThread, unsigned int * puUsedRam);


/**
@brief 获取指定任务的CPU利用率(%)
@param[in]   hThread 指定任务的句柄
@param[out] pnUsage 指定任务的CPU负载，取值范围为:0--1000
@return 获取成功则返回CSUDI_SUCCESS，失败则返回CSUDI_FAILURE。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadCPUUsage(CSUDI_HANDLE hThread, unsigned int * puUsage);


#ifdef	__cplusplus
}
#endif
/** @} */

#endif
