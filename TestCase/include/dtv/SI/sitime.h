#ifndef sitime_H
#define sitime_H

#ifdef PRAGMA
#pragma interface "sitime.h"
#endif

#include "mmcp_typedef.h"
#include "mmcp_os.h"
#include "CSUSPCommonType.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum _ENFLAG		//时区偏移标志
{
	ENFLAG_POSITIVE,		//正，表示加上时间偏移
	ENFLAG_NEGATIVE			//负，表示减上时间偏移
}enTimeOffsetFlag;

#define TDT 0				//搜索TDT表
#define TOT 1				//搜索TOT表

typedef struct
{
	long lTimeout;			//超时时间，-1表示不超时
	DWORD dwDemuxID;		//demux设备ID，
	float fTimeOffset;		//时间偏移量
	DWORD dwTable;			//指定请求那个SI table，0 表示 TDT ，1表示 TOT，当请求TOT是fTimeOffset会被忽略.
}SITM_InitParams;

/*
函数名 TimeNotifyCallback
	时间通知回调函数，当从si码流中得到新的时间，调用该函数通知用户

	void(*TimeNotifyCallback)(
		CSUDIPLUSOSTIME_S* pTime,
		CSUDIPLUSOSDATE_S* pDate
	);	

Parameters
	
	pTime
		[out]时间结构体
	pDate
		[out]日期结构体

Return Values
		无
*/
typedef void (*TimeNotifyCallback)(CSUDIPLUSOSTIME_S* pTime, CSUDIPLUSOSDATE_S* pDate);
/*
函数名 TimeNotifyCallbackEx
	时间通知回调函数，当从si码流中得到新的时间，调用该函数通知用户

	void(*TimeNotifyCallback)(
		CSUDIPLUSOSTIME_S* pTime,
		CSUDIPLUSOSDATE_S* pDate,
		float* pfTimeOffset,
		void* pUserData
	);	

Parameters
	
	pTime
		[out]时间结构体
	pDate
		[out]日期结构体
	pfTimeOffset
		[out]时间偏移值
	pUserData
		[out]用户数据

Return Values
		无
*/

typedef void (*TimeNotifyCallbackEx)(CSUDIPLUSOSTIME_S* pTime, CSUDIPLUSOSDATE_S* pDate, float* pfTimeOffset,void* pUserData);

/*
函数名 SITM_Init
	初始化SITIME，在其他函数调用前该函数必须先调用该函数,在调用该函数之前一定要正确初始化SITM_InitParams结构体。

	void SITM_Init(
		SITM_InitParams* pInitParams
	);	

Parameters
	
	pInitParams
		[in] 初始化结构体指针

Return Values

BOOL: 返回TRUE，表示初始化成功
返回FALSE，表示初始化失败
*/
BOOL SITM_Init(SITM_InitParams* pInitParams);


/*
函数名 SITM_Final
	SI时间模块销毁函数,销毁所有模块注册的回调（请慎用）

	void SITM_Final();	

Parameters
	
	无

Return Values

BOOL： TRUE表示注销成功，
FALSE表示注销失败。
*/
BOOL SITM_Final();

/*
函数名 SITM_RequestCurrentTime
	请求当前时间，异步过程，当时间更新时，callback参数所指定的函数会被调用，该函数应紧跟在函数SITM_Init之后。

	void SITM_RequestCurrentTime(
		TimeNotifyCallback callback
	);	

Parameters
	
	callback
		[in] TimeNotifyCallback回调

Return Values

int： -1，表示注册失败，
返回值大于等于0，表示注册的回调的序号
*/
int SITM_RequestCurrentTime(TimeNotifyCallback callback);
/*
函数名 SITM_RequestCurrentTimeEx
	请求当前时间，异步过程，当时间更新时，callback参数所指定的函数会被调用，该函数应紧跟在函数SITM_Init之后。

	void SITM_RequestCurrentTimeEx(
		TimeNotifyCallback callback,
		void * pvUserData
	);	

Parameters
	
	callback
		用户获取时间的回调。
	pvUserData
		用户数据。实际内容是一个BYTE字符串，其中，字节0，1，2是countryCode，如'C','H','I',字节3是regionId,如2.。长度不限，但后面的字节被忽略。	

Return Values

int： -1，表示注册失败，
返回值大于等于0，表示注册的回调的序号
*/

int SITM_RequestCurrentTimeEx(TimeNotifyCallbackEx callback,void* pvUserData);
//该函数指明SITIME计算时间时不处理时区偏移信息，即时区偏移设为0
int SITM_RequestCurrentTimeWithoutTimeoffset(TimeNotifyCallbackEx callback,void* pUserData);

/*
函数名 SITM_Cancel
	注销回调

	void SITM_Cancel(
		int handle
	);	

Parameters
	
	handle
		[in]注册的回调的序号

Return Values

int： -1，表示注册失败，
返回值大于等于0，表示注册的回调的序号
*/
void SITM_Cancel(int handle);


/*
函数名 SITM_GetNearTime
	获取近似的当前时间，同步函数，调用后马上返回，返回值为最近更新的当前时间

	void SITM_GetNearTime(
		CSUDIPLUSOSTIME_S* pTime,
		CSUDIPLUSOSDATE_S* pDate
	);	

Parameters
	
	pTime
		[out]时间结构体指针
	pDate
		[out]日期结构体指针

Return Values

BOOL： TRUE表示设置策略成功，
FALSE表示设置策略失败。

*/
BOOL SITM_GetNearTime(CSUDIPLUSOSTIME_S* pTime, CSUDIPLUSOSDATE_S* pDate);

/*
函数名 SITM_GetNearTimeEx
	获取近似的当前时间，同步函数，调用后马上返回，返回值为最近更新的当前时间

	void SITM_GetNearTime(
		CSUDIPLUSOSTIME_S* pTime,
		CSUDIPLUSOSDATE_S* pDate
	);	
	与SITM_GetNearTime的不同之处在于在里面根据系统时间做了跳动，比SITM_GetNearTime更精确些

Parameters
	
	pTime
		[out]时间结构体指针
	pDate
		[out]日期结构体指针

Return Values

BOOL： TRUE表示设置策略成功，
FALSE表示设置策略失败。

*/
BOOL SITM_GetNearTimeEx(CSUDIPLUSOSTIME_S* pTime, CSUDIPLUSOSDATE_S* pDate);

/*
函数名 SITM_GetTimeOffset
	得到当前时间的偏移量，若请求的是TOT表，则偏移量为TOT中规定的偏移量，否则为用户初始化传递的偏移量

	void SITM_GetTimeOffset(
		float* pfTimeOffset
	);	

Parameters
	
	pfTimeOffset
		[out]时间偏移值

Return Values

  BOOL： TRUE表示成功，
FALSE表示失败

*/
BOOL SITM_GetTimeOffset(float* pfTimeOffset);


/*
函数名 SITM_SetTimeOffset
	设置时间偏移

	void SITM_SetTimeOffset(
		enTimeOffsetFlag enFlag,
		CSUDIPLUSOSTIME_S* pTimeOffset
	);	

Parameters
	
	enFlag
		[in]时间偏移标志：正表示加时间偏移量，负表示减时间偏移量。
	pTimeOffset
		[in]时间偏移量。

Return Values

BOOL： TRUE表示设置成功，
FALSE表示设置失败

*/
BOOL SITM_SetTimeOffset(enTimeOffsetFlag enFlag, CSUDIPLUSOSTIME_S* pTimeOffset);


/*
函数名 SITM_TimeConvert
	时间转换，将当前时间累加时间偏移值

	void SITM_TimeConvert(
		CSUDIPLUSOSDATE_S* pDate,
		TTIME* pTime,
		float fOffsetHour
	);	

Parameters
	
	pDate
		[in/out]日期结构体。
	pTime
		[in/out]时间结构体。
	fOffsetHour
		[in]时间偏移量。

Return Values

无

*/
void SITM_TimeConvert(CSUDIPLUSOSDATE_S* pDate,CSUDIPLUSOSTIME_S* pTime,float fOffsetHour);


/*
函数名 SITM_MJDConvert
	时间转换，将当前时间加上时间偏移量

	void SITM_MJDConvert(
		int nMJD,
		int nUTC,
		float fOffsetHour
		CSUDIPLUSOSDATE_S* pDate,
		TTIME* pTime,	
	);	

Parameters
	
	nMJD
		[in]当前日期
	nUTC
		[in]当前时间
	fOffsetHour
		[in]时间偏移量。
	pDate
		[out]转换后的加上时间偏移量的日期
	pTime
		[out]转换后的加上时间偏移量的时间

Return Values

无

*/
void SITM_MJDConvert(int nMJD,int nUTC,float fOffsetHour,CSUDIPLUSOSDATE_S* pDate,CSUDIPLUSOSTIME_S* pTime );

/*
函数名 SITM_Start
启动时间搜索，重新开始请求TDT或者TOT表
SITM_Init之后时间搜索是自动启动的，无需调用本接口
本接口仅在SITM_Stop之后用于恢复时间搜索
本接口的调用不会影响回调函数的注册，但SITM_Stop后，回调不会再上来直到再次调用SITM_Start为止
本接口仅为TDT表或TOT表与音视频PID冲突时，暂停TDT或TOT表请求以释放资源而设置，一般情况下无需使用
*/
BOOL SITM_Start(void);

/*
函数名 SITM_Stop
停止时间搜索，即停止请求TDT或者TOT表
本接口的调用不会影响回调函数的注册，但SITM_Stop后，回调不会再上来直到再次调用SITM_Start为止
本接口仅为TDT表或TOT表与音视频PID冲突时，暂停TDT或TOT表请求以释放资源而设置，一般情况下无需使用
*/
BOOL SITM_Stop(void);

/**
@brief 根据输入参数先停止接收TDT，然后重新接收 TDT 

@param[in] pInitParams:输入参数结构体
@return TRUE :重新启动接收TDT成功；FALSE:重新启动接收TDT失败
@note 
*/

BOOL SITM_Restart(SITM_InitParams* pInitParams);
#ifdef __cplusplus
}
#endif


#endif /* sitime_H */
