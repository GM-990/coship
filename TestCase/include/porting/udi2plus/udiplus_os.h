/**@defgroup OS_PLUS OS Plus UDI OS 模块的补充模块，提供定时器、时间等接口
@brief 使用UDI OS模块,对一些常用,但不需要平台相关的OS接口,进行软件封装

@note 本模块的定义的部分接口可能存在效率或者精度问题,请不要过于依赖其严格精度,如定时器等
@warning  本模块仅供MMCP内部使用，不允许将本模块的任何定义、接口提供给MMCP以外的模块实用，包括但不限于UDI、集成接口中
@warning  不允许在需要对MMCP外部发布的头文件中包含本头文件

@version 2.0.2 2009/09/16 根据UDI2.1版本的定义，重新调整本模块
@version 2.0.1 2009/04/02 UDI2初稿
- 将原UDI OS中的定时器,时钟功能移到此处
@{
*/

#include "udi2_typedef.h"
#include "udi2_os.h"
#include "udi2_error.h"
#include "udiplus_error.h"

#ifndef _UDIPLUS_OS_H_
#define _UDIPLUS_OS_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief OS Plus 模块相关错误代码值*/
enum
{
	CSUDIOSPLUS_ERROR_BAD_PARAMETER = CSUDIPLUS_OS_ERROR_BASE,	///< 参数错误
	CSUDIOSPLUS_ERROR_FEATURE_NOT_SUPPORTED,					///< 不支持此操作
	CSUDIOSPLUS_ERROR_UNKNOWN_ERROR,							///< 一般错误
	CSUDIOSPLUS_ERROR_NO_MEMORY,								///< 无内存可用错误
	CSUDIOSPLUS_ERROR_TIMEOUT,									///< 超时错误
	
	CSUDIOSPLUS_ERROR_MAX										///< 边界值
};

/**@brief 定时器选项*/
typedef enum
{
    EM_UDIPLUS_OS_TIMERMODE_REPEAT,			///< 重复模式选项，即每指定时间触发一次
    EM_UDIPLUS_OS_TIMERMODE_ONESHOT,		///< 单次模式选项，即仅在指定时间后触发一次
    EM_UDIPLUS_OS_TIMERMODE_MAX			    ///< 模式选项结束标记，无效参数
}CSUDIPLUSOSTimerMode_E;

typedef struct
{
	unsigned int m_nYear_13         :13;	///< 年,[1970,...]
	unsigned int m_nMonth_8         :8;		///< 月,[1, 12]，注意这里的定义与C99标准不同
	unsigned int m_nDay_8           :8;		///< 日,[1, 31]
	unsigned int m_nWeekDay_3       :3;		///< days since Sunday,[0, 6]
}CSUDIPLUSOSDATE_S;

typedef struct
{
	unsigned int m_nHour_5         :5;		///< 时,[0, 23]
	unsigned int m_nMinute_6       :6;		///< 分,[0, 59]
	unsigned int m_nSecond_6       :6;		///< 秒,[0, 59]
	unsigned int m_nMilliSeconds_15 :15;
}CSUDIPLUSOSTIME_S;

typedef struct
{
	CSUDIPLUSOSDATE_S m_sDate;
	CSUDIPLUSOSTIME_S m_sTime;
}CSUDIPLUSOSSysTime_S;

/**
@brief 定时器回调函数

新建一个定时器时,定义一个本类型的函数作为回调函数,定时器每次触发,该函数会被调用。
@param[in] pParam 入口参数，其值为调用CSUDI_Timer_Create函数时的pvArg参数。
@see CSUDI_Timer_Create
*/
typedef void (*CSUDIOSTimerCallback_F)(void* pParam);

typedef CSUDI_INT32	CSUDIPLUS_TIME_T;		///< 表示时间秒数的专用定义,可以直接计算差值或者打印，但不要假设其位数(目前是32位，但今后可能变长)

/*******************************************************************
 **                     Clock definitions                         **
 *******************************************************************/
/**
@brief 设置系统时间

设置系统时间,之后会自动计时以保证时间是准确的
@param[in] nSeconds 自1970年1月1日凌晨起至现在的总秒数
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
*/
CSUDI_Error_Code CSUDIPlusOSClockSet(CSUDIPLUS_TIME_T nSeconds);

/**
@brief 获取当前系统时间

一旦设置之后,会自动开始计时,即使的精度依赖于CSUDIOSGetTime的精度
@param[out] pnSeconds 自1970年1月1日凌晨起至现在的总秒数
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 在未设置时间之前,认为开机时间为1970年1月1日凌晨
*/
CSUDI_Error_Code CSUDIPlusOSClockGet(CSUDIPLUS_TIME_T* pnSeconds);

/**
@brief 获取自系统启动后经过的时间毫秒数

@return 成功返回自系统启动后经过的时间毫秒数，失败返回0
*/
unsigned int CSUDIPlusOSGetTickCount(void);

/**
@brief 将系统时间转换为秒数

将年月日时分秒形式的时间转换成自1970年1月1日起至现在的总秒数
@param[in] psDateTime 输入时间,以年月日时分秒表示，结构体中Week,m_MilliSeconds_8字段可不填写
@param[out] pnSeconds 输出时间，以自1970年1月1日凌晨起至现在的总秒数表示
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
*/  	
CSUDI_Error_Code CSUDIPlusOSSystime2Second(CSUDIPLUSOSSysTime_S* psDateTime,CSUDIPLUS_TIME_T* pnSeconds);

/**
@brief 将秒数转换为系统时间

将自1970年1月1日起至现在的总秒数转换成年月日时分秒形式
@param[in] nSeconds 输入时间，以自1970年1月1日起至现在的总秒数表示
@param[out] psDateTime 输出时间,以年月日时分秒表示，输出结构体中m_MilliSeconds_8总是0
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
*/ 
CSUDI_Error_Code CSUDIPlusOSSecond2Systime(CSUDIPLUS_TIME_T nSeconds,CSUDIPLUSOSSysTime_S* psDateTime);

/*******************************************************************
 **                     Timer definitions                         **
 *******************************************************************/
 /**
@brief 创建定时器

@param[in] eMode 定时器工作模式,详将CSUDIPLUSOSTimerMode_E定义
@param[in] pCallback 定时器触发的回调函数，定时器触发时会在同一个任务中调用此函数
@param[in] pvArg 定时器回调函数的参数，即调用pCallback时的参数
@param[out] phTimer 定时器句柄
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 定时器的精度约为100ms，超过这个精度的定时器将是非常不准确的
@note 所以定时器回调在同一个任务中发生，因此请尽快使定时器回调函数返回，否则将对其它定时器产生影响
@note 定时器回调函数中，仍可以调用定时器函数进行操作
*/
CSUDI_Error_Code CSUDIPlusOSTimerCreate(CSUDIPLUSOSTimerMode_E eMode, CSUDIOSTimerCallback_F pCallback, void* pvArg, CSUDI_HANDLE *phTimer);

/**
@brief 启动一个定时器

启动定时器,约nMilliSeconds毫秒后,将触发对应的回调函数
@param[in] hTimer 定时器句柄，CSUDIPlusOSTimerCreate接口的输出参数
@param[in] nMilliSeconds 定时时间,单位毫秒
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 启动一个已经启动的定时器将重置计时时间,并返回成功
*/
CSUDI_Error_Code CSUDIPlusOSTimerStart(CSUDI_HANDLE hTimer, unsigned int unMilliSeconds);

/**
@brief 停止一个定时器

停止一个定时器,回调函数将不再被触发
@param[in] hTimer 定时器句柄
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 停止一个已经停止的定时器仍可返回成功
*/
CSUDI_Error_Code CSUDIPlusOSTimerStop(CSUDI_HANDLE hTimer);

/**
@brief 重置一个定时器

重置一个定时器,定时时间在调用后重新开始记录
@param[in] hTimer 定时器句柄
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 重置一个已经停止的定时器仍返回成功,但这个操作没有任何意义
*/
CSUDI_Error_Code CSUDIPlusOSTimerReset(CSUDI_HANDLE hTimer);

/**
@brief 销毁一个定时器

销毁一个定时器,销毁后该定时器句柄不再有效,也无法再次启动
@param[in] hTimer 定时器句柄
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 无论是停止还是激活状态，均可以销毁一个定时器
*/
CSUDI_Error_Code CSUDIPlusOSTimerDestroy(CSUDI_HANDLE hTimer);


#ifdef	__cplusplus
}
#endif
/** @} */

#endif

