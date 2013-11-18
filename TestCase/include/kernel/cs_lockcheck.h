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
*��ӡ��ǰ��������ʹ��״̬��
*��ӡ��������ʾ��Ϣ��Ӧ��ϵ����:
*(1)	ERROR_LEVEL	
*			ֻ��ʾ�Ƿ�������������������������ƴ�ӡ��
*			<lockcheck> --------Mutex deadlock 1----
*			�������޴�ӡ��
*(2)	INFO_LEVEL(�Ƽ�ֵ)	
*			��ʾ�������Ĵ���λ�ã���ǰ����λ�ã��磺
*			<lockcheck> (1) Create  : ./main.c 118
*			<lockcheck>     LastLock: ./irCodeInit.c 955
*			<lockcheck>     NowLock: ./irCodeInit.c 557
*(3) DEBUG_LEVEL	
*			��ʾ�������Ĵ�����������������������Ϣ��������򿪡�
*/
void CSLockCheckPrint(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif   

