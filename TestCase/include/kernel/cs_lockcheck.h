/**
*/

#ifndef _UDI_DEADLINE_H_
#define _UDI_DEADLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

int dl_pthread_mutex_init (char* pFile,int nLine,pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);

int dl_pthread_mutex_destroy (char* pFile,int nLine,pthread_mutex_t* mutex);

int dl_pthread_mutex_trylock(char* pFile,int nLine,pthread_mutex_t* mutex);

int dl_pthread_mutex_lock(char* pFile,int nLine,pthread_mutex_t* mutex);

int dl_pthread_mutex_unlock(char* pFile,int nLine,pthread_mutex_t* mutex);

int dl_pthread_cond_init(char* pFile,int nLine,pthread_cond_t* cond, const pthread_condattr_t* attr);

int dl_pthread_cond_destroy (char* pFile,int nLine,pthread_cond_t* cond);

int dl_pthread_cond_signal (char* pFile,int nLine,pthread_cond_t* cond);

int dl_pthread_cond_broadcast (char* pFile,int nLine,pthread_cond_t* cond);

int dl_pthread_cond_wait (char* pFile,int nLine,pthread_cond_t* cond, pthread_mutex_t* mutex);

int dl_pthread_cond_timedwait (char* pFile,int nLine, pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime);

/*
*打印当前所有锁的使用状态。
*打印级别与显示信息对应关系如下:
*(1)	ERROR_LEVEL	
*			只显示是否存在死锁，若有死锁则有类似打印：
*			<lockcheck> --------Mutex deadlock 1----
*			无死锁无打印。
*(2)	INFO_LEVEL(推荐值)	
*			显示所有锁的创建位置，当前加锁位置，如：
*			<lockcheck> (1) Create  : ./main.c 118
*			<lockcheck>     LastLock: ./irCodeInit.c 955
*			<lockcheck>     NowLock: ./irCodeInit.c 557
*(3) DEBUG_LEVEL	
*			显示所有锁的创建、加锁、解锁、销毁信息。不建议打开。
*/
void CSLockCheckPrint(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif   

