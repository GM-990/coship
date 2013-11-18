#ifndef _GRASYS_H_
#define _GRASYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SUPPORT_MUTIL_THREAD 1

#if  SUPPORT_MUTIL_THREAD
#define USE_PTHREADS  /*default is use pthread */
#ifdef USE_PTHREADS
#include <pthread.h>


//此宏控制Microwin中的锁的调试模式
//打开此宏后可以使用 dbg_GRAMutex_dump 把Microwin中的各个锁
//的使用情况打印出出来
//#define MUTEXT_DEBUG

#ifdef MUTEXT_DEBUG /*<!-- yinbenqing 2010/1/2 11:58:38 */
struct _GRA_Mutex {
	pthread_mutex_t id;
	int recursive;
	pthread_t owner;
    char callfunction[128];
    int nLineNum;
    };

typedef struct _GRA_Mutex GRA_Mutex;

extern void dbg_GRAMutex_dump(void );

extern GRA_Mutex *dbg_GRA_CreateMutex(void );
extern int dbg_GRA_MutexP(char *function, int lineNume, GRA_Mutex * mutex);
extern int dbg_GRA_MutexV(char *function, int lineNume, GRA_Mutex * mutex);
extern void dbg_GRA_DestroyMutex(char *function, int lineNume, GRA_Mutex * mutex);

#define GRACREATEMUTEX() dbg_GRA_CreateMutex()
#define GRALOCK(m) dbg_GRA_MutexP((char *)__FUNCTION__, __LINE__, (m))
#define GRAUNLOCK(m)  dbg_GRA_MutexV((char *)__FUNCTION__, __LINE__, (m))
#define GRADESTORYMUTEX(m) dbg_GRA_DestroyMutex((char *)__FUNCTION__, __LINE__, (m))
#else /* MUTEXT_DEBUG *//* yinbenqing 2010/1/2 11:58:38 */
struct _GRA_Mutex {
	pthread_mutex_t id;
	int recursive;
	pthread_t owner;
    };

typedef struct _GRA_Mutex GRA_Mutex;

extern GRA_Mutex *GRA_CreateMutex(void);
extern int GRA_MutexP(GRA_Mutex * mutex);
extern int GRA_MutexV(GRA_Mutex * mutex);
extern void GRA_DestroyMutex(GRA_Mutex * mutex);

#define GRACREATEMUTEX() GRA_CreateMutex()
#define GRALOCK(m) GRA_MutexP(m)
#define GRAUNLOCK(m)  GRA_MutexV(m)
#define GRADESTORYMUTEX(m) GRA_DestroyMutex(m)    

#endif /* MUTEXT_DEBUG *//* yinbenqing 2010/1/2 11:58:38 --!>*/
    
#else
    struct _GRA_Mutex {
	Uint8 nothing;
    }GRA_Mutex;
#define GRACREATEMUTEX() 
#define GRALOCK(m) 
#define GRAUNLOCK(m)  
#define GRADESTORYMUTEX(mutex) 
#endif				/*USE_PTHREAD */

#else
   struct _GRA_Mutex {
	Uint8 nothing;
    }GRA_Mutex;

#define GRACREATEMUTEX() 
#define GRALOCK(m) 
#define GRAUNLOCK(m)  
#define GRADESTORYMUTEX(m) 
#error "please define SUPPORT_MUTIL_THREAD"
#endif				/*SUPPORT_MUTIL_THREAD */

#ifdef __cplusplus
}
#endif

#endif /*_GRASYS_H_*/
