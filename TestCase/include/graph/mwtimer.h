#ifndef _GRATIMER_H_
#define _GRATIMER_H_


#include <mwtypes.h>
#include <mwcommon.h>

#ifdef __cplusplus
extern "C" {
#endif

/* This is the OS scheduler timeslice, in milliseconds */
#define GRA_TIMESLICE		10

#define TIMER_RESOLUTION	10
#define ROUND_RESOLUTION(X) (((X+TIMER_RESOLUTION-1)/TIMER_RESOLUTION)*TIMER_RESOLUTION)
#define GRA_TIMER_ONESHOT	0
#define GRA_TIMER_PERIODIC	1
#define GRALONGVAL(a,b) (long)(a-b)

typedef int (*GRA_TimerCallback) (void *param);

typedef struct _GRA_Timer {
	GRA_List list;
	int id;
	Uint8 type;		/* GRATIMER_ONESHOT or GRATIMER_PERIODIC */
	Uint32 interval;
	GRA_TimerCallback callback;
	void *arg;
	Uint32 begin;
	pthread_t owner;
}GRA_Timer;

extern int GRA_TimerInit(void);
extern Uint32 GRA_GetTicks(void);
extern void GRA_Delay(Uint32 ms);
extern GRA_Timer *GRA_AddTimer(Uint8 type,Uint32 interval, 
			       GRA_TimerCallback callback, 
			       void *param,pthread_t owner);

    extern GRA_bool GRA_RemoveTimer(GRA_Timer * t);
    extern void GRA_TimerCheck(void);
    extern double GRA_GetTime(void);

typedef void (*MWTimerCallback)(void* pParam);

/**@brief 定时器选项*/
typedef enum
{
    MWTIMER_MODE_REPEAT = 0,		/**< 重复模式选项，即每指定时间触发一次*/
    MWTIMER_MODE_ONE_SHOT,			/**< 单次模式选项，即仅在指定时间后触发一次*/
    MWTIMER_MODE_END				/**< 模式选项结束标记，无效参数*/
}MWTimerMode_E;

int MWTimerInit(void);
int MWTimerCreate(MWTimerMode_E enmTimerMode, MWTimerCallback pCallback, PVOID pUserData, HCSHANDLE *phTimer);
int MWTimerStart(HCSHANDLE hTimer, unsigned int dwMilliSeconds);
int MWTimerStop(HCSHANDLE hTimer);
int MWTimerReset(HCSHANDLE hTimer);
int MWTimerDestroy(HCSHANDLE hTimer);

    
#ifdef __cplusplus
}
#endif
#endif				/* _GRATIMER_H_ */
