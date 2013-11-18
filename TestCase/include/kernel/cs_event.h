#ifndef _CS_EVENT_H__    
#define _CS_EVENT_H__    

#include "cs_pthread.h"
#include <cs_memory.h>
#include "mmcp_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------+
 |------------------------    Structure Definition    -----------------------|
 +-------------------------------------------- ------------------------------*/


/* Event type */
/*! Any event type */
#define EVT_ANY_TYPE    0xffffffff
/*! Any event code */
#define EVT_ANY_CODE    0xffffffff

/*  中间件保留区间  0x1  <=x  <= 0xffff  */
#define EVT_ALARM_EV_TYPE    			(0x1)
#define EVT_CAM_SHOW_INFO    			(0x10)
#define	EVT_CAM_CARD_INFO    			(0x11)

#define EVT_PI_EV_DLOAD_SECTION_LOADED		(0x0021) 
#define EVT_PI_EV_DLOAD_TABLE_LOADED		(0x0022)
#define EVT_PI_EV_DLOAD_LOAD_FAILED			(0x0023)
#define EVT_PI_EV_DLOAD_EIT_TABLE_LOADED	(0x0024)
#define EVT_PI_EV_DLOAD_VERSION_CHANGED		(0x0025)


#define	EVT_TYPE_PVR_NOTIFY    			(0x1001)
#define	EVT_TYPE_NVOD         			(0x1002)
#define	EVT_TYPE_NETWORK       			(0x1003)
#define	EVT_TYPE_USB_EVENT     			(0x1234)
#define	EVT_TYPE_SUBTITLE_EVENT     	(0x1235)

/* key  0x3001 => 0x300f */
#define	EVT_INPUT_REMOTECTRL   			(0x3001)
#define	EVT_INPUT_FRONTBOARD    		(0x3002)
#define	EVT_INPUT_KEYBOARD    			(0x3003)
#define	EVT_INPUT_MOUSE    				(0x3004)

/*   DSM   0X3370  - 0X338F*/
#define	EVT_DSMOC_EVT_LOAD_DSI_START    	(0x3370)

#define	EVT_SILOADER_EVENT_SILOADER    (0x33a0)
#define	EVT_SILOADER_EVENT_MONITOR     (0x33a1)

#define EVT_TIMER       			(0x33E0)
#define EVT_TYPE_TIME_LOAD   			(0x33d1)

#define EVT_SIMPLECONTROLLER_USER_EVENT_TYPE  	(0x33f1)
#define EVT_PROGRAMEVENT_GUIDE_MONITOR_TYPE  	(0x3400)

#define EVT_NETWORK_INTERFACE_MANAGER_TYPE  (0x3410)
#define	EVT_KEEP_ALIVE_TYPE    				(0x35ff)
#define	EVT_SELECTOR_EVENT_TYPE    			(0x3600)

#define	EVT_NAVIGATOR_EVENT_TYPE     		(0x53e1)

#define EVT_EVENT_RC_CONNECTION  			(0xa601)


#define	EVT_EVENT_DATABASE_MONITOR    		(0xf100)

#define	EVT_EVENT_BROWSER_TYPE    			(0xf527)

#define	 EVT_SEARCH_EVENT_TYPE  			(0xffee)

#define EVT_ADS_EVENT_TYPE 			(0xffef)

#define EVT_EVENT_EIT_LOADER                (0xFD00)
#define EVT_EVENT_PF_TIMEOUT                (0xFD01)
#define	EVT_EVENT_PGRATE_CHANGE     		(0xFE00)

#define EVT_CA_ALARM_EVENT          		(0xfe01)
#define	EVT_CA_COMMAND_EVENT    			(0xfe02)
#define	EVT_CA_STATUS_EVENT    				(0xfe03)
#define	EVT_CA_INFO_EVENT    				(0xfe04)
#define	EVT_CA_MMI_EVENT     				(0xfe05)
#define EVT_OTA_EVENT						(0xfe06)
#define EVT_CA_SMC_EVENT					(0xfe07)
#define EVT_EVENT_SA_TYPE                   (0xFF01)
/*! User event  用户区间  0x10000 <= x <= 0x5fffffff  */
#define EVT_USER       					(0x10000)

/* reserve  预留区间   0x60000000 <= x < 0xffffffff */
#define EVT_RESER                        (0x60000000)


/*!Timer event */

/* Error codes */
enum _MMCP_EvtErrorCode
{
  /*!success */
  EVT_SUCCESS = 0,
  /*!insufficient memory could be used */
  ERR_EVT_OUT_OF_MEMORY = -2,
  /*!invalid parameter(s) passed */
  ERR_EVT_INVALID_PARAM = -3,
  /*!illegal access to specified target */
  ERR_EVT_ILLEGAL_ACCESS = -4,
  /*!no specified target found */
  ERR_EVT_NOT_FOUND = -5,
  /*!specified event has grabbed */
  ERR_EVT_HAS_GRABBED = -6,
  /*! no available event */
  ERR_EVT_NO_EVENT = -7,
  /*! timer operation fail */
  ERR_EVT_TIMER_FAILED = -8
};

/*---------------------------------------------------------------------------+
 |------------------------    Structure Definition    -----------------------|
 +-------------------------------------------- ------------------------------*/

/* can used by the user */
typedef struct _MMCP_EvtEventEntry MMCP_EvtEventEntry;
struct _MMCP_EvtEventEntry {
  int type; /**< event type */
  int code; /**< event code */
  void  *data; /**<additional data of a Event */
  DWORD callback_count; /**<Callback number that treat this event*/
};

/**
 * \typedef MMCP_EvtCallbackFunc
 * \brief A type definition for Event processing callback function.
 */
typedef int (*MMCP_EvtCallbackFunc)(
  const MMCP_EvtEventEntry *event,
  void  *client_data,
  void  *object,  /* reserved for Java. */
  void  *method   /* reserved for Java. */
);

/**
 * 回调函数的入口信息。
 */
 struct _MMCP_EvtCallbackList;
 typedef struct _MMCP_EvtCallbackList MMCP_EvtCallbackList;

/**
 * \typedef MMCP_EvtFreeFunc
 * \brief A type definition for freeing Event data function.
 */
typedef void (*MMCP_EvtFreeFunc)(
  MMCP_EvtEventEntry *event
);

/*---------------------------------------------------------------------------+
 |----------------------    Event Manager Interface    ----------------------|
 +-------------------------------------------- ------------------------------*/

/**
 * Initializes Event Manager.
 */
int
MMCP_EvtInit (BOOL debug);

/**
 * Registers a callback for one type and a range of codes for the specific 
 * thread. This function allows a thread to register a callback function 
 * for other thread.
 */
int MMCP_EvtRegisterCallback(
  pthread_t thread,
  int type,
  int code_min,
  int code_max,
  MMCP_EvtCallbackFunc func,
  void  *client_data,
  void  *object,
  void  *method,
  int *callback_id
);

/**
 * Unregisters a callback.
 * The MMCP_EvtUnregisterCallback function removes a callback entry
 * registered by a thread, when thread_id = 0, means for the current thread..
 */
int MMCP_EvtUnregisterCallback(pthread_t thread_id, int callback_id);

/**
 * Unregisters all callbacks for the thread thread_id.
 * The MMCP_EvtUnregisterAllCallbacks function removes all callback entries
 * registered by the thread thread_id, when thread_id = 0, means for the
 * current thread.
 */
void MMCP_EvtUnregisterAllCallbacks(pthread_t thread_id);

/**
 * Modifies a callback entry.
 * The MMCP_EvtChangCallback function allows a thread to change its callback
 * entry previously registered.
 *
 */
void MMCP_EvtChangeCallback (int callback_id, MMCP_EvtCallbackFunc func,
                            void  *client_data);

/**
 * Changes a client data attached to callback entry.
 * The MMCP_EvtSetCallbackData function is used to change the client data attached
 * to callback entry. 
 * This callback entry must be registered by the current thread.
 */
int MMCP_EvtSetCallbackData (int callback_id, void  *client_data);

/**
 * Registers a grab entry.
 * The MMCP_EvtGrabEvent function registers a grab entry with the specified type 
 * and code values. Only the thread which registered the grab entry can receive
 * the event matching the type and code values.
 *
 */
int MMCP_EvtGrabEvent(pthread_t thread_id, int type, int code);

/**
 * Determines whether an event has been grabbed or not.
 * if thread_id == 0, means for the current thread.
 */
int
MMCP_EvtIsGrabbed(pthread_t thread_id, int type, int code);
/**
 * Removes a grab entry.
 * The MMCP_EvtUngrabEvent function removes a grab entry matching the type and 
 * code values. This grab entry must be registered by the current thread.
 */
int MMCP_EvtUngrabEvent(pthread_t thread_id, int type, int code);

/**
 * Get the next available event associated with the current thread. If 
 * no event is available, the function return immediately.
 */
int MMCP_EvtGetEvent (MMCP_EvtEventEntry *event);

/**
 * Get the next available event associated with the current thread. If 
 * no event is available, the function wait until it comes.
 */
int MMCP_EvtGetEventBlocking(MMCP_EvtEventEntry *event);

/**
 * Get the next available event associated with the given thread id. the 
 * function returns if blocking = false, and
 * wait until event comes if blocking = true. When thread_id = 0, means it 
 * is for the current thread.
 */
int MMCP_EvtGetTypedEvent(int type, int code, 
			    MMCP_EvtEventEntry *event,
			    BOOL blocking, pthread_t thread_id);
/**
 * Resume a blocked thread which is waiting for the event coming.
 */
void MMCP_EvtSignal (pthread_t thread_id);

/**
 * Process the specific event.
 */
int MMCP_EvtProcessEvent (MMCP_EvtEventEntry *event);

/**
 * Sends directly a Event to a thread.
 * The MMCP_EvtSendEvent function sends a Event to a specific thread.
 * It put the Event to the Event list and return immediately.
 */
int MMCP_EvtSendEvent (pthread_t thread_id, MMCP_EvtEventEntry *event,
			MMCP_EvtFreeFunc func);

/**
 * Broadcasts a Event in the system.
 * The MMCP_EvtBroadcastEvent function broadcasts a Event in the system,
 * all threads which are interested in this Event will receive it.
 */
int MMCP_EvtBroadcastEvent (MMCP_EvtEventEntry *event,
			     MMCP_EvtFreeFunc func);

/**
 * Peeks a Event from the Event list.
 * The MMCP_EvtPeekEvent function searches the Event corresponding to
 * (type, code) in the Event list, and returns the address of the
 * Event if found. NOTE: The thread cannot peek a Event belonged to
 * another thread, and should not modify the contents of the Event.
 */
MMCP_EvtEventEntry *MMCP_EvtPeekEvent (int type, int code);

/**
 * Remove a Event.
 * The MMCP_EvtRemoveEvent function removes a Event belongs to the
 * current thread from the Event list,
 *if tid is -1,then remove this  event in all threads.
 *if tid is 0, then remove this  event in current threads.
 */
void MMCP_EvtRemoveEvent (pthread_t tid, MMCP_EvtEventEntry *event);

/**
 * Removes all the Events associated with the specified thread.
 * The MMCP_EvtRemoveAllEvents function removes all the Events belonged to
 * the specified thread from the Event list.
 */
void MMCP_EvtRemoveAllEvents (pthread_t tid);

int
MMCP_EvtCreateTimer(pthread_t tid, 
		    MMCP_EvtCallbackFunc func, 
		    void *object, 
		    void *method, 
			 BOOL repeats,
		    HCSHANDLE *timer_id);

/**
 * Creates a timer with a specified interval value.
 * The MMCP_EvtSetTimer function creates a timer with the specified interval
 * value. If the function succeeds, the timer is started immediately. Each
 * time the specified interval passes, the timer sends an event to specified
 * thread. To stop a timer,call MMCP_EvtStopTimer; and call MMCP_EvtResumeTimer
 * to restart it.
 *
 */
int MMCP_EvtSetTimer(pthread_t tid, 
		    int delay, 
		    int interval, 
		    int count,
		    HCSHANDLE timer_id);

/**
 * Stops a timer.
 * The MMCP_EvtStopTimer stops the timer specified by the timerid.The timer can
 * be restarted by call the MMCP_EvtResume function.
 */
int MMCP_EvtStopTimer(pthread_t tid, HCSHANDLE timer_id);

/**
 * Restarts a timer.
 * The MMCP_EvtResumeTimer function restarts a timer specified by the timer id.
 */
int MMCP_EvtResumeTimer (HCSHANDLE timer_id,int delay, int interval);

/**
 * Destroys a timer.
 * The MMCP_EvtRemoveTimer destroys a timer specified by the timer id.
 */
int MMCP_EvtRemoveTimer(pthread_t tid, HCSHANDLE timer_id);

 /**
  * Locate a Callback: this function is used for Timer.c to get a Timer
  * callback struct so that it can destroy created Java Global Ref
  */
MMCP_EvtCallbackList *
MMCP_EvtLocateCallback (pthread_t tid, int type, int code);

void * MMCP_EvtGetCallbakObject(MMCP_EvtCallbackList * psEvtCallback);

#ifdef __cplusplus
}
#endif

#endif /* __MMCP_EVENT_H__ */
