/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_os.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <sched.h> 
#include <pthread.h>
#include <semaphore.h>

#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <linux/msg.h>
#include <linux/sem.h>
#include <sys/timeb.h>
#include <sys/time.h>


#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udiplus_debug.h"
#include "udiplus_typedef.h"
#include "udiplus_error.h"

#include "udi2_os.h"


#define CSSTD_UNUSED(x) ((void)x)

typedef struct timeval cs_time_t;

#define TASK_PRIOR_STEP		17

/*The task priority are mapped to POSIX*/

#define POSIX_TASK_LOWEST            1
#define POSIX_TASK_LOW                  20
#define POSIX_TASK_NORMAL            40
#define POSIX_TASK_HIGH                 70
#define POSIX_TASK_HIGHEST           90


/*NPTL��Ҫ�����8K��С��Ϊ��ջ����������������ڴ汣����*/
#define NPTL_ADDED_STACK_SIZE	(8*1024)


/*for print module name*/
#define  MODULE_NAME   "CS_OS"


//vivian, can not find these definition in android toolchain
/* Cancellation */
enum
{
  PTHREAD_CANCEL_ENABLE,
#define PTHREAD_CANCEL_ENABLE   PTHREAD_CANCEL_ENABLE
  PTHREAD_CANCEL_DISABLE
#define PTHREAD_CANCEL_DISABLE  PTHREAD_CANCEL_DISABLE
};
enum
{
  PTHREAD_CANCEL_DEFERRED,
#define PTHREAD_CANCEL_DEFERRED	PTHREAD_CANCEL_DEFERRED
  PTHREAD_CANCEL_ASYNCHRONOUS
#define PTHREAD_CANCEL_ASYNCHRONOUS	PTHREAD_CANCEL_ASYNCHRONOUS
};

int sem_timedwait(sem_t *men, const struct timespec * abs_timeout);


typedef struct
{
    CSUDI_HANDLE semaphore;
    DWORD owner;
    int count;
}cs_mutex_t;



static void adjusttimespec(struct timespec* ts)
{
    CSASSERT(NULL != ts);

    if(NULL != ts)
    {
        /* 1000000000*ns = 1s */
        if(ts->tv_nsec > 1000000000)
        {
            ts->tv_nsec -= 1000000000;
            ++ts->tv_sec;
        }
    }

    return;
}


typedef struct CSThreadInfo_{
	pthread_t 	stThreadHndl;
	int			nPThreadPrio;
	CSUDIOSThreadEntry_F fnThreadEntry;
	void * pvArg;
	struct CSThreadInfo_ *pstNext;
}CSThreadInfo;



static CSThreadInfo *g_pstThreadListHead = NULL;
static CSUDI_HANDLE g_hThreadLock = NULL;

static BOOL ThreadInfoLock(void)
{
	if (g_hThreadLock == NULL)
	{
		if (CSUDI_SUCCESS != CSUDIOSMutexCreate("ThreadInfo", 0, &g_hThreadLock))
		{
			return FALSE;
		}
	}

	if (CSUDI_SUCCESS != CSUDIOSMutexWait(g_hThreadLock,CSUDIOS_TIMEOUT_INFINITY))
	{
		return FALSE;
	}

	return TRUE;
}


static BOOL ThreadInfoUnLock(void)
{
	if (g_hThreadLock == NULL)
	{
		CSDEBUG(MODULE_NAME,ERROR_LEVEL,"error:should lock fist before release lock\n");
		return FALSE;
	}

	if (CSUDI_SUCCESS != CSUDIOSMutexRelease(g_hThreadLock))
	{
		return FALSE;
	}

	return TRUE;
}



static void RemoveThreadInfo(CSThreadInfo *pstThreadInfo)
{
	CSThreadInfo *pstTmp;
		
	/*here thead life is over ,clean resource*/
	pstTmp = g_pstThreadListHead;

	/*the first thread node is the current thread*/
	if (pstTmp == pstThreadInfo)
	{
		g_pstThreadListHead = pstThreadInfo->pstNext;
	}
	else
	{
		while(pstTmp)
		{
			if (pstTmp->pstNext == pstThreadInfo)
			{
				break;
			}

			pstTmp = pstTmp->pstNext;
		}

		if (pstTmp)
		{
			pstTmp->pstNext = pstThreadInfo->pstNext;
		}
		else
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Error: can't find the thead info in list !!!\n");
		}
	}
	//CSDEBUG(MODULE_NAME,ERROR_LEVEL,"free thread info\n");
	CSUDIOSFree(pstThreadInfo);
}


static void PThreadEntryProxy(void * pvParam)
{
	CSThreadInfo *pstThreadInfo = (CSThreadInfo*)pvParam;

	if (pvParam == NULL)
	{
		CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Error thread entry!!!\n");
		return ;
	}

	if (pstThreadInfo->fnThreadEntry == NULL)
	{
		CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Error thread info!!!\n");
		return ;
	}
	
	pstThreadInfo->fnThreadEntry(pstThreadInfo->pvArg);
	
	ThreadInfoLock();
	RemoveThreadInfo(pstThreadInfo);
	ThreadInfoUnLock();
}



/**
@brief ��������

*����һ������,����乲���߼���ַ,�൱���̵߳ĸ��
@param[in] pcName ��������,���Ȳ��ܳ���32�ֽ�,����ΪCSUDI_NULL��������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER���������񴴽�ʧ��
@param[in] nPriority ��������ȼ�,��ΧΪ0-255;0���ȼ����,255���ȼ���ߡ��������������ȼ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER,�������񴴽�ʧ��
@param[in] nStackSize �����ջ��С,����֧��512KB�Ĵ�С���������ϵͳ֧���Զ���С�����ջ����,����ʵ�ֺ��Ըò���,��ʹ���߱�������ò�������Ӧ��֧���Զ�ջ��С��ϵͳ
@param[in] fnThreadEntry �������ں���
@param[in] pvArg �������ڲ���
@param[out] phThread ���ɵ�������,��������ʹ�øþ��
@return �ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ����phThreadΪCSUDI_NULL
@note 
- pcName����Ӱ�촴������ɹ����,������ΪCSUDI_NULL,�����ظ�,�����Ƽ�������
- �����Ը���������в�����ʹ��phThread
- �߳�����Ӧ����ո�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadCreate(const char * pcName,int nPriority,int nStackSize, CSUDIOSThreadEntry_F fnThreadEntry,void * pvArg,CSUDI_HANDLE * phThread)
{
	
	CSUDI_Error_Code 			enRet = CSUDI_FAILURE;
	unsigned int 				dwNameLen = 0;
	unsigned int 				stacksize = 0;
	int 						nRetVal;
	int						nNewPrio;
	int						nTempPrio = nPriority/TASK_PRIOR_STEP;
	struct sched_param 		param;
	pthread_t 				thread;
	pthread_attr_t  			attr;
	CSThreadInfo 				*pstThreadInfo;
	
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
	
	if (phThread == CSUDI_NULL || fnThreadEntry == CSUDI_NULL)
	{
		CSASSERT(phThread != CSUDI_NULL);
		CSASSERT(fnThreadEntry != CSUDI_NULL);
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	if (nStackSize < 0 || nPriority < 0 || nPriority > 255)
	{
		CSASSERT((nStackSize > 0) && (nPriority >= 0 && nPriority <= 255));
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}
	
	CSASSERT(pcName != NULL);

	if(pcName != NULL)
	{
	    dwNameLen = strlen(pcName);
	}

	if (dwNameLen > 32)
	{
		CSASSERT(dwNameLen < 32);
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	pstThreadInfo = CSUDIOSMalloc(sizeof(CSThreadInfo));
	if (pstThreadInfo == NULL)
	{
		return CSUDIOS_ERROR_NO_MEMORY;
	}
	
	*phThread = CSUDI_NULL;
        if(nTempPrio < 4)
        {
            nNewPrio = POSIX_TASK_LOWEST;
        }
        else if(nTempPrio  < 8)
        {
            nNewPrio = POSIX_TASK_LOW;
        }
        else if(nTempPrio  < 12)
        {
            nNewPrio = POSIX_TASK_NORMAL;
        }
        else if(nTempPrio  < 14)
        {
            nNewPrio = POSIX_TASK_HIGH;
        }
        else
        {
            nNewPrio = POSIX_TASK_HIGHEST;
        }
        
        /*
        	typedef struct __pthread_attr_s
        	{
        		  int __detachstate;
        		  int __schedpolicy;
        		  struct __sched_param __schedparam;
        		  int __inheritsched;
        		  int __scope;
        		  size_t __guardsize;
        		  int __stackaddr_set;
        		  void *__stackaddr;
        		  size_t __stacksize;
        	} pthread_attr_t;
        */
        nRetVal = pthread_attr_init(&attr);
        
#if 1
        CSASSERT(nRetVal == 0);
        if(nStackSize<0x4000)
        {
            nStackSize = 0x4000;
        }
        stacksize = (DWORD)nStackSize+NPTL_ADDED_STACK_SIZE;
        nRetVal = pthread_attr_setstacksize(&attr,stacksize);
        CSASSERT(nRetVal == 0);

				stacksize = 0;
        nRetVal = pthread_attr_getstacksize(&attr,(size_t *)&stacksize);
        CSASSERT(nRetVal == 0);

				pstThreadInfo->fnThreadEntry = fnThreadEntry;
				pstThreadInfo->pvArg = pvArg;
				pstThreadInfo->nPThreadPrio = nNewPrio;

				ThreadInfoLock();
				if (g_pstThreadListHead == NULL)
				{
					g_pstThreadListHead = pstThreadInfo;
					pstThreadInfo->pstNext = NULL;
				}
				else
				{
					pstThreadInfo->pstNext = g_pstThreadListHead;
					g_pstThreadListHead = pstThreadInfo;
				}
				ThreadInfoUnLock();
#endif

		 		nRetVal = pthread_create (&thread, &attr, (void *(*)(void *))PThreadEntryProxy, pstThreadInfo);
        CSASSERT(nRetVal == 0);

	 			pstThreadInfo->stThreadHndl = thread;
	//vivian
        //nRetVal = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
        //CSASSERT(nRetVal == 0);

        //nRetVal = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        //CSASSERT(nRetVal == 0);
        
        if (nRetVal == 0 && thread != 0)
        {
            /*use default shecdule no task priority*/
#if 1
#ifdef USE_OTHER_SCHED
						int nPolicy = SCHED_OTHER;
#else
						int nPolicy = SCHED_RR;
#endif     
						nPolicy = SCHED_RR;
            param.sched_priority = nNewPrio;
            nRetVal = pthread_setschedparam (thread, nPolicy, &param);
            CSASSERT(nRetVal == 0);
						//printf("create %x thread ret:%d===\n",thread,nRetVal);
#endif

            *phThread = (CSUDI_HANDLE)thread;

						enRet = CSUDI_SUCCESS ;
				}

				nRetVal = pthread_attr_destroy(&attr);
				CSASSERT(nRetVal >= 0);

				//CSDEBUG(MODULE_NAME,INFO_LEVEL, "[CS_OS] Warnning: nStackSize can not be set!!.\n",nStackSize);
				//CSDEBUG(MODULE_NAME,INFO_LEVEL, "[CS_OS] CSCreateThread(%s,%d,0x%08x)\n",pstrName, nPriority,nStackSize);
	
				UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, enRet);    
				return enRet;
				
}

/**
@brief ɾ������

ͨ�����ɾ��һ���Ѿ�����������
@param[in] hThread �Ѵ���������������CSUDIOSThreadCreate���������phThread
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ;��hThread��Чʱ,ɾ��ʧ��
@note 
- Ҫ��������ں����������������з��أ�Ȼ���ٵ��ñ��ӿ��������ͷ���Դ
- �������ڱ����������ٱ�����
- ��ͼȥ����һ���������е����񣨼�������ں�����û�з��ص�����ʱ���᷵��CSUDIOS_ERROR_TASK_BUSY��
@see  CSUDIOSThreadJoin

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadDestroy (CSUDI_HANDLE hThread)
{
    	CSUDI_Error_Code enRet = CSUDI_FAILURE;
	int nRetVal;
	CSThreadInfo *pstTmp;

    	CSASSERT (hThread != CSUDI_NULL);

	if (hThread == CSUDI_NULL)
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	nRetVal = pthread_cancel ((pthread_t)hThread);         /* Send cancel request   */
	CSASSERT(nRetVal == 0);

	nRetVal = pthread_join ((pthread_t) hThread, CSUDI_NULL);    /* Wait for task to exit */
	CSASSERT(nRetVal == 0);

	ThreadInfoLock();
	pstTmp = g_pstThreadListHead;
	while(pstTmp)
	{
		if (pstTmp->stThreadHndl == hThread)
		{
			break;
		}
		pstTmp=pstTmp->pstNext;
	}
	if (pstTmp)
	{
		RemoveThreadInfo(pstTmp);
	}
	else
	{
		CSDEBUG(MODULE_NAME,ERROR_LEVEL,"WARNING:can't find thread info,maybe has been free when thread exit\n");
	}
	
	ThreadInfoUnLock();
	
	enRet = CSUDI_SUCCESS;

    return enRet;
}

/**
@brief ��������

ͨ���������һ���������е�����ʹ��ֹͣ����
@param[in] hThread �Ѵ���������������CSUDIOSThreadCreate���������phThread
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ;��hThread��Чʱ,�������ʧ��
@note 
- ����һ���Ѿ�����������Է��سɹ�
- ����һ���Ѿ����з��ص����񷵻�ʧ��
@deprecated �˽ӿڽ����ڵ��ԣ����ƽ̨��֧�֣��ɲ�ʵ�֣���ʱ�˽ӿڷ���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadSuspend(CSUDI_HANDLE hThread)
{
#if 0
	CSUDI_Error_Code enRet = CSUDI_FAILURE;

	CSASSERT (hThread != NULL);

  	if (hThread == CSUDI_NULL)
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}
	if (pthread_kill((pthread_t)hThread, SIGSTOP) == 0)
	{
	    enRet = CSUDI_SUCCESS;
	}

	CSASSERT(enRet == CSUDI_SUCCESS);

	return enRet;
#else
	return CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED;
#endif
}

/**
@brief �����ѹ�������

ͨ���������һ���ѹ��������ʹ��ָ�����
@param[in] hThread �Ѵ���������������CSUDIOSThreadCreate���������phThread
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ;��hThread��Чʱ����������ʧ��
@note 
- ����һ���������е������Է��سɹ�
- ����һ���Ѿ����з��ص����񷵻�ʧ��
@deprecated �˽ӿڽ����ڵ��ԣ����ƽ̨��֧�֣��ɲ�ʵ�֣���ʱ�˽ӿڷ���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadResume(CSUDI_HANDLE hThread)
{
#if 0
	CSUDI_Error_Code enRet = CSUDI_FAILURE;

	CSASSERT (hThread != NULL);

  	if (hThread == CSUDI_NULL)
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	if (pthread_kill((pthread_t)hThread,SIGCONT) == 0)
	{
	    enRet = CSUDI_SUCCESS;
	}

	CSASSERT(enRet == CSUDI_SUCCESS);

	return enRet;
#else
	return CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED;
#endif
}
/**
@brief ʹһ������ȴ���һ���������

@param[in] hThread �Ѵ���������������CSUDIOSThreadCreate���������phThread
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note 
- �����ȴ����񷵻�ʱ���ýӿڷ���CSUDI_SUCCESS��
- �ȴ�һ���Ѿ����ص���û�б����ٵ�����ʱ���ýӿڷ���CSUDI_SUCCESS��
- ���������ͬʱ���ô˽ӿ�ȥ�ȴ�һ������ʱ��������δ����ġ�
- ������ȴ�һ���Ѿ������ٵ�����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadJoin (CSUDI_HANDLE hThread)
{
	CSUDI_Error_Code enRet = CSUDI_FAILURE;

	CSASSERT (hThread != NULL);

  	if (hThread == CSUDI_NULL)
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	if (pthread_join((pthread_t)hThread,(void**)CSUDI_NULL) == 0)
	{
		enRet = CSUDI_SUCCESS;
	}

	return enRet;
}

/**
@brief ��ȡ��ǰ����ľ��
@param[out] phThread ��ǰ������ľ������CSUDIOSThreadCreate���������phThread
@return  ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ;

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIOSThreadSelf(CSUDI_HANDLE * phThread)
{
	CSUDI_Error_Code enRet = CSUDI_FAILURE;

	CSASSERT (phThread != NULL);

  	if (phThread == CSUDI_NULL)
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	if (CSUDI_NULL != (*phThread = (CSUDI_HANDLE)pthread_self()))
	{
		enRet = CSUDI_SUCCESS;
	}

	return enRet;
}


static DWORD CSGetThreadId(VOID)
{
    return (DWORD)pthread_self();
}


/**
@brief ����ǰ����һ��ʱ��

@param[in] uMilliSeconds �����ʱ�䣬��λ�Ǻ���
@note 
�ýӿ�ǿ�Ƶ�ǰ�����������uMilliSeconds���룬���ָ�������ʱ��uMilliSecondsΪ0����ʱ������ƽ̨����������������Ϊ��
- ��������ִ��Ȩ��
- ����ע��ֵ�� 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void CSUDIOSThreadSleep(unsigned int uMilliSeconds)
{
    struct timespec delay;
    struct timespec rem;
    int rc;

    if(uMilliSeconds==0)
    {
        return;
    }

    delay.tv_sec = (int)uMilliSeconds/1000;
    delay.tv_nsec = 1000 * 1000 * (uMilliSeconds%1000);

    for(;;) {
        rc = nanosleep(&delay, &rem); /* [u]sleep can't be used because it uses SIGALRM */
        if (rc!=0) {
            if (errno==EINTR) {
                delay = rem; /* sleep again */
                continue;
            }

            CSASSERT(0);

            return ;
        }
        break; /* done */
    }

    return;
}

/**
@brief ʹ�����������ִ�еĻ���

����������ִ��Ȩ��
@note 
- �ýӿڵ��ú�������Ч���������ء�
- �ڶ����񻷾�(����)�У�Ϊ�˷�ֹĳ�����ռCPU��Դ(��ʱ���������޷��õ���Ӧ)�������õ�ǰִ�е������������ִ�еĻ��ᡣ
- ���øýӿڲ����ܱ�֤��һ�����е������Ǹ����񡣼�����ýӿڷ���ʱ���������ٴλ��ִ��Ȩ����ô���ýӿ���ͣ����������Ͽ�ʼ���С�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void CSUDIOSThreadYield (void)
{
	#if 0
	pthread_yield();
	#else
	CSUDIOSThreadSleep(3);
	#endif
}

/*******************************************************************
 **                    Message Queue definitions                  **
 *******************************************************************/
/**
@brief ������Ϣ����

@param[in] pcName ��Ϣ��������,���Ȳ��ܳ���32���ֽڣ�����ΪCSUDI_NULL��������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER��������Ϣ���д���ʧ��
@param[in] nMaxMsgs ��Ϣ��������������Ϣ������һ����˵���ᳬ��128������������һ����Ϣ��������Ϣ���д���ʧ��
@param[in] nSizePerMsg ÿһ����Ϣ����󳤶�,��λ�ֽڣ�һ����˵���ᳬ��128�ֽڡ�����֧��һ���ֽڣ�������Ϣ���д���ʧ��
@param[out] phMsgQueue ���ɵ���Ϣ���о��
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������ֵ����phMsgQueΪCSUDI_NULL��
@note 
- pcName����Ӱ�촴����Ϣ���гɹ����,������ΪCSUDI_NULL,�����ظ�,�����Ƽ�������
- ��Ȼ������Ϣ���е������Ϣ��������Ϣ���Ȳ���̫�󣬵��Խ����������������Ϣ�������ڴ����ֱ��ʹ��ϵͳ���ڴ�ķ�ʽ������Ϣ���е�����
@note 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/



typedef struct
{
    DWORD  Name;
    DWORD  ByteQueueSize;                   /* Size of the queue in bytes   */
    DWORD  ByteNodeSize;                   	/* Message size in bytes        */
    DWORD  MsgCount;
    DWORD  Head;                      		 /* Head offset from queue start */
    DWORD  Tail;                       		/* Tail offset from queue start */
    CSUDI_HANDLE QMutex;              	 /* Queue mutex                  */
    CSUDI_HANDLE QEvent;               	/* Queue event                  */
    CSUDI_HANDLE QEventAvailSpace;/* Queue event                  */
    BYTE  *StartPtr;                 		 /* Pointer to queue start       */
    BOOL	m_bIsRun;
}MSG_QUEUE;



CSUDI_Error_Code CSUDIOSMsgQueueCreate(const char * pcName,int nMaxMsgs,int nSizePerMsg,CSUDI_HANDLE * phMsgQueue)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	CSUDI_HANDLE 		Qmutex = (CSUDI_HANDLE)CSUDI_NULL;
	CSUDI_HANDLE 		Qevent = (CSUDI_HANDLE)CSUDI_NULL;
	CSUDI_HANDLE 		QEventAvailSpace = (CSUDI_HANDLE)CSUDI_NULL;
	MSG_QUEUE*			Qptr = CSUDI_NULL;
	DWORD 				nNameLen = 0;

	CSASSERT(nSizePerMsg > 0);

	if(pcName != CSUDI_NULL)
	{
	    nNameLen = strlen(pcName);
	}

	CSASSERT(nNameLen < 32);

	if((nSizePerMsg <= 0) || (nNameLen >= 32) || (nMaxMsgs <= 0) || (phMsgQueue == CSUDI_NULL))
	{
	    return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	*phMsgQueue = CSUDI_NULL;
    /*================================================
     * Create memory, mutex and event for the queue
     *===============================================*/
    Qptr = (MSG_QUEUE *) malloc ( ( sizeof ( MSG_QUEUE ) + (DWORD)( nMaxMsgs * ( (( nSizePerMsg + 3 ) / 4) * 4 ) ) ) * 2 ) ;

    if ( Qptr != CSUDI_NULL)
    {
        enRet = CSUDIOSMutexCreate("MsgQueueMutex", 0,&Qmutex);

        if ( enRet == CSUDI_SUCCESS)
        {
            enRet = CSUDIOSEventCreate(0,0 ,&Qevent);

            if ( enRet == CSUDI_SUCCESS)
            {
                enRet = CSUDIOSEventCreate( 0, CSUDIOS_EVENT_INITIAL_STATUS,&QEventAvailSpace);

                if ( enRet == CSUDI_SUCCESS)
                {
                    /*==========================================
                    * Initialize the queue
                    *=========================================*/
                    enRet = CSUDIOSMutexWait(Qmutex, CSUDIOS_TIMEOUT_INFINITY);
					
                    if(enRet == CSUDI_SUCCESS)
                    {
                        Qptr->Name				= 0;//pstrName;
                        Qptr->ByteQueueSize		= (DWORD)(nMaxMsgs * nSizePerMsg * 2);
                        Qptr->ByteNodeSize		= (DWORD)(nSizePerMsg * 2);
                        Qptr->MsgCount 			= 0;
                        Qptr->Head      		= 0;
                        Qptr->Tail      		= 0;
                        Qptr->QMutex    		= Qmutex;
                        Qptr->QEvent    		= Qevent;
                        Qptr->QEventAvailSpace  = QEventAvailSpace;
                        Qptr->StartPtr  		= (BYTE *) ( Qptr) + sizeof ( MSG_QUEUE ) * 2;
                        Qptr->m_bIsRun			= TRUE;

                        enRet = CSUDIOSMutexRelease(Qmutex);
                        if(enRet == CSUDI_SUCCESS)
                        {
                            *phMsgQueue = (CSUDI_HANDLE)Qptr;
				enRet = CSUDI_SUCCESS;
                        }
                    }
                }
            }
        }

        if ( *phMsgQueue == CSUDI_NULL )
        {
            CSASSERT(*phMsgQueue != CSUDI_NULL);
            if ( Qevent != CSUDI_NULL )
            {
                enRet = CSUDIOSEventDestroy( Qevent );
                if(enRet != CSUDI_SUCCESS)
                {
                    CSDEBUG(MODULE_NAME,INFO_LEVEL,"CSDestroyEventdwRet=%d\r\n",enRet);
                }
            }

            if ( Qmutex != (CSUDI_HANDLE)CSUDI_NULL )
            {
                enRet = CSUDIOSMutexDestroy(Qmutex);
                if(enRet != CSUDI_SUCCESS)
                {
                    CSDEBUG(MODULE_NAME,INFO_LEVEL,"CSDestroyMutex=%d\r\n",enRet);
                }
            }

            if(CSUDI_NULL != Qptr)
            {
                free (Qptr);
            }
        }
    }

    return enRet ;
}

/**
@brief ������Ϣ����

ͨ���������һ����Ϣ����
@param[in] hMsgQueue ��Ϣ���о������CSUDIOSMsgQueueCreate���������phMsgQueue��
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������ֵ
@warning ɾ��һ�����ڱ��ȴ����߷�����Ϣ����Ϣ����,����ǲ�ȷ����,�����øýӿ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueDestroy(CSUDI_HANDLE hMsgQueue)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	CSUDI_HANDLE 			Qmutex;
	MSG_QUEUE   			*Qptr;

	if (hMsgQueue == CSUDI_NULL)
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}        

	Qptr = (MSG_QUEUE *) hMsgQueue;
	Qmutex = Qptr->QMutex;

	if ( Qmutex != 0 )
	{
	    /*=============================================
	    * Delete queue event, mutex and memory
	    *============================================*/
	    enRet = CSUDIOSMutexWait( Qmutex, CSUDIOS_TIMEOUT_INFINITY );
	    if(enRet == CSUDI_SUCCESS)
	    {
	        if ( Qptr->m_bIsRun == TRUE )
	        {
	            Qptr->m_bIsRun = FALSE;
				
	            enRet = CSUDIOSEventDestroy(Qptr->QEvent);
	            enRet = CSUDIOSEventDestroy(Qptr->QEventAvailSpace);
	            enRet = CSUDIOSMutexRelease (Qmutex);				
	            enRet = CSUDIOSMutexDestroy (Qmutex);

	            Qptr->QMutex = 0;
	            Qptr->QEvent = 0;
	            Qptr->QEventAvailSpace = 0;

	            free (Qptr);

	            enRet = CSUDI_SUCCESS;
	        }
	    }
	}

    CSASSERT(enRet == CSUDI_SUCCESS);

    return enRet;
}

/**
@brief ����Ϣ���н���һ����Ϣ

����Ϣ���������һ����Ϣ�������Ϣ����Ϊ����ȴ�ֱ����Ϣ����������Ϣ���߳�ʱΪֹ
@param[in] hMsgQueue ��Ϣ���о������CSUDIMsgQueueCreate���������phMsgQueue
@param[out] pvMsg ������Ϣ���ݵ��ڴ��ַ
@param[in] nMaxMsgBytes ��ַ������ڴ泤��
@param[in] uTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return �ɹ�����CSUDI_SUCCESS;��ʱ����CSUDIOS_ERROR_TIMEOUT;ʧ�ܷ��ش������
@note 
- ��hMsgQueue��Чʱ,��pvMsg��Ч,��nMaxMsgBytes==0ʱ,����ʧ��,ͬʱ������Ϣ�����ж�ȡ��Ϣ
- ��nMaxMsgBytesС����Ϣ����ʱ,ȡ��Ч������Ϣ���ݣ�ͬʱ���سɹ�����Ϣ�Ӷ�����ȡ��
- ��nMaxMsgBytes������Ϣ����ʱ����Ϣ����ȫ����pvMsg,��������Ϣ���Ⱥ󲿷����ݲ�ȷ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueReceive(CSUDI_HANDLE hMsgQueue,void * pvMsg,int nMaxMsgBytes,unsigned int uTimeout)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	CSUDI_HANDLE Qmutex;
	MSG_QUEUE   *Qptr;
	DWORD      Head = 0;//, Tail;

	CSASSERT(hMsgQueue != NULL && pvMsg != NULL && nMaxMsgBytes > 0);

	if (hMsgQueue == NULL || pvMsg == NULL || nMaxMsgBytes <= 0)
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

        Qptr = (MSG_QUEUE *) hMsgQueue;
        Qmutex = Qptr->QMutex;

        enRet = CSUDIOSEventWait(Qptr->QEvent, uTimeout);

        if ( enRet == CSUDI_SUCCESS)
        {
            enRet = CSUDIOSMutexWait( Qmutex, CSUDIOS_TIMEOUT_INFINITY );

            if ( enRet == CSUDI_SUCCESS )
            {
                Head = Qptr->Head;
                //Tail = Qptr->Tail;

                if ( Qptr->MsgCount > 0 )
                {
                    DWORD dwCopySize = (DWORD)nMaxMsgBytes > ( Qptr->ByteNodeSize / 2 ) ? ( Qptr->ByteNodeSize / 2 ) : (DWORD)nMaxMsgBytes;

                    memcpy ( pvMsg, Qptr->StartPtr + Head, dwCopySize );
                    memset( Qptr->StartPtr + Head, 0, dwCopySize );

                    Head += Qptr->ByteNodeSize;

                    if ( Head >= Qptr->ByteQueueSize )
                    {
                        Head = 0;
                    }

                    Qptr->Head = Head;
                    Qptr->MsgCount--;

                    if ( Qptr->MsgCount == 0 )
                    {
                        CSUDIOSEventReset(Qptr->QEvent);
                    }

                    CSUDIOSEventSet( Qptr->QEventAvailSpace );

                    enRet = CSUDI_SUCCESS;
                }

                CSUDIOSMutexRelease( Qmutex );
            }
            else
            {
                CSASSERT( enRet == CSUDI_SUCCESS );
            }
        }
        else if (enRet == CSUDIOS_ERROR_TIMEOUT)
        {
            enRet = CSUDIOS_ERROR_TIMEOUT;
        }

    CSASSERT( enRet == CSUDI_SUCCESS || enRet == CSUDIOS_ERROR_TIMEOUT);

    return enRet;
}

/**
@brief ����Ϣ�����﷢��һ����Ϣ

�����Ϣ����������ȴ�ֱ����Ϣ�������п���λ�û��߳�ʱΪֹ
@param[in] hMsgQueue ��Ϣ���о������CSUDIOSMsgQueueCreate���������phMsgQueue
@param[in] pvMsg ������Ϣ���ݵ��ڴ��ַ
@param[in] nMsgBytes ��ַ������ڴ泤��
@param[in] uTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return �ɹ�����CSUDI_SUCCESS;��ʱ����CSUDIOS_ERROR_TIMEOUT;ʧ�ܷ��ش������
@note 
- ��hMsgQueue��Чʱ,��pvMsg��Ч,��nMsgBytes==0ʱ,����ʧ�ܡ�
- ��nMsgBytes������󳤶�ʱ,ȡ��Ч������Ϣ���ݣ�ͬʱ���سɹ���
- ��Ϣ�����Կ���ģʽ������pvMsg�ڱ��������غ󼴿��ͷš�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueSend(CSUDI_HANDLE hMsgQueue, const void * pvMsg, int nMsgBytes, unsigned int uTimeout)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	CSUDI_HANDLE 			Qmutex;
	MSG_QUEUE   			*Qptr;
	DWORD 				Tail;//Head

	CSASSERT(hMsgQueue != NULL && pvMsg != NULL && nMsgBytes > 0);
	if (hMsgQueue == CSUDI_NULL || pvMsg == CSUDI_NULL || nMsgBytes <= 0 )
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	Qptr = (MSG_QUEUE *) hMsgQueue;
	Qmutex = Qptr->QMutex;

	enRet = CSUDIOSEventWait( Qptr->QEventAvailSpace, uTimeout);

	if ( enRet == CSUDI_SUCCESS)
	{
	    enRet  = CSUDIOSMutexWait( Qmutex, CSUDIOS_TIMEOUT_INFINITY );

	    if ( enRet == CSUDI_SUCCESS)
	    {
	        Tail = Qptr->Tail;

	        if ( Qptr->ByteQueueSize >= ( ( Qptr->MsgCount + 1 ) * Qptr->ByteNodeSize ) )
	        {
	            DWORD dwCopySize = ( (DWORD)nMsgBytes > ( Qptr->ByteNodeSize / 2 ) ? ( Qptr->ByteNodeSize / 2 ) : (DWORD)nMsgBytes );

	            memcpy( Qptr->StartPtr + Tail, pvMsg, dwCopySize );

	            Tail += Qptr->ByteNodeSize;

	            if ( Tail >= Qptr->ByteQueueSize )
	            {
	                Tail = 0;
	            }

	            Qptr->Tail = Tail;
	            Qptr->MsgCount++;

	            if ( Qptr->MsgCount >= ( Qptr->ByteQueueSize/Qptr->ByteNodeSize ) )
	            {
	                CSUDIOSEventReset(Qptr->QEventAvailSpace);
	            }

	            CSUDIOSEventSet( Qptr->QEvent );            /* Set the queue event  */

	            enRet = CSUDI_SUCCESS;
	        }

	        CSUDIOSMutexRelease(Qmutex);
	    }
	    else
	    {
	        CSASSERT( enRet == CSUDI_SUCCESS);
	    }
	}
	else if ( enRet == CSUDIOS_ERROR_TIMEOUT )
	{
	    enRet = CSUDIOS_ERROR_TIMEOUT;
	}

	CSASSERT(enRet == CSUDI_SUCCESS);

	return enRet;
}


/*******************************************************************
 **                     Semaphore definitions                     **
 *******************************************************************/
/**
@brief �����ź���

@param[in] pcName �ź�������,���Ȳ��ܳ���32���ֽڣ�������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER�������ź�������ʧ��
@param[in] nInitialCount ��Ч�ź��������൱��PVԭ���еĳ�ʼֵ
@param[in] nMaxCount ����ź�����
@param[out] phSemaphore ���ɵ��ź������
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ����phSemΪCSUDI_NULL
@note 
- pcName����Ӱ�촴���ź����ɹ���񣬼�����ΪCSUDI_NULL�������ظ�,�����Ƽ�������
- �ж������ȴ�ͬһ���ź���ʱ,һ�����ź������ͷ�,��ȷ���ĸ�������Գɹ�����ź���
- �ź�������Ӧ����ո�
@todo Ŀǰ�����ƽ̨��nMaxCount��Ч���벻Ҫʹ�ã�nMaxCountΪ���޴�С

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemCreate(const char * pcName,int nInitialCount,int nMaxCount,CSUDI_HANDLE * phSemaphore)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	DWORD dwNameLen = 0;

	CSASSERT(pstrName != NULL);

	if(pcName != NULL)
	{
	    dwNameLen = strlen(pcName);
	}

	CSASSERT((dwNameLen <= 32) && (nMaxCount >= nInitialCount) && (nInitialCount >= 0) && (nMaxCount > 0));
	if ((dwNameLen <= 32) && (nMaxCount >= nInitialCount) && (nInitialCount >= 0) && (nMaxCount > 0))
	{
	    int nRet = 1;

	    sem_t *sem = NULL;

		*phSemaphore = NULL;

	    sem = ( sem_t *)malloc( sizeof( sem_t ) ); /* allocate Memory  */

	    CSASSERT(sem != NULL);

	    if ( sem != NULL )
	    {
	        /*the semaphore is local to the current process( pshared is zero ) */
	        nRet = sem_init( sem, 0,  (DWORD)nInitialCount );
	        /*return 0 on success and -1 on unknown error */
	        if ( nRet == 0 )
	        {
	            *phSemaphore = (CSUDI_HANDLE)sem;
			enRet = CSUDI_SUCCESS;
	        }
	        else
	        {
	            CSASSERT(0);
	            free( sem ); /* free memory.  */
	        }
	    }
	}
	else
	{
		enRet = CSUDIOS_ERROR_BAD_PARAMETER;
	}

    return enRet;
}

/**
@brief �����ź���

@param[in] hSemaphore �ź����������CSUDIOSMsgQueueCreate���������phMsgQueue
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��س������
@warning ����һ�����ڱ���������ȴ������ź�����һ����ȷ������Ϊ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemDestroy(CSUDI_HANDLE hSemaphore)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;

	CSASSERT(hSemaphore != NULL);

	if (hSemaphore != NULL)
	{
	    if (sem_destroy((sem_t *)hSemaphore) == 0)
	    {
	        free((sem_t *)hSemaphore); /* free memory.  */
	        enRet = CSUDI_SUCCESS;
	    }
	}
	else
	{
		enRet = CSUDIOS_ERROR_BAD_PARAMETER;
	}

	CSASSERT(enRet == CSUDI_SUCCESS);

	return enRet;
}


/**
@brief �ȴ��ź������൱��P����(P��������Դ)

@param[in] hSemaphore �ź����������CSUDIOSMsgQueueCreate���������phMsgQueue
@param[in] uTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return �ɹ�����CSUDI_SUCCESS;��ʱ����CSUDIOS_ERROR_TIMEOUT;ʧ�ܷ��ش������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemWait(CSUDI_HANDLE hSemaphore,unsigned int uTimeout)
{
   CSUDI_Error_Code 		enRet = CSUDI_FAILURE;

    CSASSERT(hSemaphore != NULL);

    if (hSemaphore != NULL)
    {
        int nRet;

        if(uTimeout == 0xFFFFFFFF)
        {
            nRet = sem_wait((sem_t *)hSemaphore);
            if (nRet == 0)
            {
                enRet = CSUDI_SUCCESS;
            }
            else
            {
                //printf("Wait(%d)nRet is %d\n", uTimeout, nRet);
                enRet = CSUDI_FAILURE;
            }
        }
        else if (uTimeout == 0)
        {
            nRet = sem_trywait((sem_t *)hSemaphore);
            if (nRet == 0)
            {
                enRet = CSUDI_SUCCESS;
            }
            else if(errno == EAGAIN)
            {
                enRet = CSUDIOS_ERROR_TIMEOUT;
            }
            else
            {
                /*printf("Wait(%d)nRet is %d\n", uTimeout, nRet);*/
                enRet = CSUDI_FAILURE;
            }
        }
        else
        {
            struct timespec ts;
            struct timeval tv;

            nRet = gettimeofday (&tv, NULL);
            CSASSERT(nRet == 0);

            ts.tv_sec = tv.tv_sec + (int)(uTimeout/1000);
            ts.tv_nsec = tv.tv_usec*1000 + (int)(uTimeout%1000)*1000000;

            adjusttimespec(&ts);

            nRet = sem_timedwait((sem_t *)hSemaphore, &ts);

            if (nRet == 0)
            {
                enRet = CSUDI_SUCCESS;
            }
            else if(errno == ETIMEDOUT)
            {
                enRet = CSUDIOS_ERROR_TIMEOUT;
            }
            else
            {
                enRet = CSUDI_FAILURE;
                CSDEBUG("CS_OS",ERROR_LEVEL,"CSWaitForSemaphore.CS_OSP_FAILURE.errno=%d",errno);
            }
        }
    }
	else
	{
		enRet = CSUDIOS_ERROR_BAD_PARAMETER;
	}

    return enRet;
}

/**
@brief �ͷ��ź������൱��V����(V���ͷ���Դ��
@param[in] hSemaphore �ź����������CSUDIOSMsgQueueCreate���������phMsgQueue
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��س������
@note ���ź������ͷŴ����ﵽ������ʱ������CSUDIOS_ERROR_UNKNOWN_ERROR

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/

CSUDI_Error_Code CSUDIOSSemRelease(CSUDI_HANDLE hSemaphore)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;

	CSASSERT(hSemaphore != CSUDI_NULL);

	if (hSemaphore != CSUDI_NULL)
	{
	        if (0 == sem_post((sem_t *)hSemaphore))
	        {
			enRet = CSUDI_SUCCESS;
	        }
	}
	else
	{
		enRet = CSUDIOS_ERROR_BAD_PARAMETER;
	}

	CSASSERT(enRet == CSUDI_SUCCESS);
	return enRet;
}


/*******************************************************************
 **                     Mutex definitions                         **
 *******************************************************************/
/**
@brief ������Ƕ�׵Ļ�����

@param[in] pcName ����������,���Ȳ��ܳ���32���ֽڣ�������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER�������ź�������ʧ��
@param[in] uOptions ������ѡ��,Ŀǰ֧��:(��ѡ������)
- CSUDIOS_MUTEX_OBTAIN ������������������ø���(��λΪ0��CSUDIOS_MUTEX_DEFAULT��ʾ����ø���)
@param[out] phMutex ���������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��س������,��phMutexΪCSUDI_NULL
@note 
- pcName����Ӱ�촴���������ɹ����,������ΪCSUDI_NULL,�����ظ�,�����Ƽ�������
- ͬһ�������Ѿ���һ��������������,���ٴζ������,��ʹ��Ƕ����,����μ�������Ҫ����ͷŲ����ͷŸ�����
- һ�������һ��������������,�������������п���������ʧ�ܡ�
- �����������ڸ߼�����,����������߼�����֮
- ����������Ӧ����ո�
- �ж������ȴ�ͬһ��������ʱ,һ���û��������ͷ�,��ȷ���ĸ�������Գɹ�����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexCreate(const char * pcName, unsigned int uOptions,CSUDI_HANDLE * phMutex)
{
	cs_mutex_t* 			mutex = NULL;
	DWORD 				dwNameLen = 0;
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;

	CSSTD_UNUSED(uOptions);

	if(pcName != CSUDI_NULL)
	{
	    dwNameLen = strlen(pcName);
	}

	if(phMutex == CSUDI_NULL)
	{
		CSASSERT(phMutex != CSUDI_NULL);
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	if(dwNameLen >= 31)
	{
	    return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	*phMutex = CSUDI_NULL;
	mutex =(cs_mutex_t*) CSUDIOSMalloc(sizeof(cs_mutex_t));

	CSASSERT(mutex);

	if( CSUDI_NULL != mutex )
	{
		CSUDI_HANDLE semaphore;
		enRet = CSUDIOSSemCreate (pcName,1,1,&semaphore) ;
		CSASSERT(semaphore);

		if( enRet == CSUDI_SUCCESS)
		{
			if (uOptions == CSUDIOS_MUTEX_OBTAIN)
			{
				enRet = CSUDIOSSemWait(semaphore,CSUDIOS_TIMEOUT_IMMEDIATE);
				CSASSERT(enRet == CSUDI_SUCCESS);
				if (enRet == CSUDI_SUCCESS)
				{
					mutex->count = 1;
					mutex->owner = CSGetThreadId();
				}
			}
			else
			{
				mutex->count = 0;
				mutex->owner = (DWORD)CSUDI_NULL;
			}
			mutex->semaphore = semaphore;
			*phMutex = mutex;
		}

		if (enRet != CSUDI_SUCCESS)
		{
		        if(mutex)
		        {
		            CSUDIOSFree(mutex);
		            mutex = CSUDI_NULL;
		        }
		        CSDEBUG(MODULE_NAME,ERROR_LEVEL,"[USP_OSP] can't create semaphore for a mutex  !\n");
		}
	}
	else
	{
	    CSDEBUG(MODULE_NAME,ERROR_LEVEL,"[USP_OSP] can't malloc memory for  a mutex  !\n");
	}

    return enRet;
}

/**
@brief ���ٻ�����

@param[in] hMutex ���������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��س������
@warning ����һ�����ڱ���������ȴ����Ļ�������һ����ȷ������Ϊ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexDestroy(CSUDI_HANDLE hMutex)
{
    CSUDI_Error_Code 		enRet = CSUDI_FAILURE;

    if(hMutex)
    {
        cs_mutex_t* mutex = (cs_mutex_t*) hMutex;

        if(CSUDIOSSemDestroy( mutex->semaphore) == CSUDI_SUCCESS)
        {
        	CSUDIOSFree(mutex);
              enRet = CSUDI_SUCCESS;
        }
        //CSDEBUG(MODULE_NAME,INFO_LEVEL,"[CS_OS]: DestroyMutex %xh\r\n", hMutex);
    }
	else
	{
		enRet = CSUDIOS_ERROR_BAD_PARAMETER;
	}

    return enRet;
}

/**
@brief �ȴ�������,����������

@param[in] hMutex ���������
@param[in] uTimeout �ȴ���ʱʱ��,���뵥λ,Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ�,���ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return �ɹ�����CSUDI_SUCCESS;��ʱ����CSUDIOS_ERROR_TIMEOUT;ʧ�ܷ��ش�����롣 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexWait(CSUDI_HANDLE hMutex,unsigned int uTimeout)
{
    CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
    CSASSERT(hMutex);
    if(hMutex)
    {
        cs_mutex_t* mutex = (cs_mutex_t*) hMutex;

        DWORD  tid;

        tid = CSGetThreadId();

        if(mutex->owner == tid)
        {
            ++mutex->count;
            enRet = CSUDI_SUCCESS;
        }
        else
        {
            enRet = CSUDIOSSemWait(mutex->semaphore,uTimeout);
            if( enRet == CSUDI_SUCCESS)
            {
                mutex->owner = tid;
                mutex->count = 1;
            }
            else
            {
                CSDEBUG(MODULE_NAME,ERROR_LEVEL,"[CS_OS] can't  acquire semaphore for a mutex %xh, uTimeout=%d !\r\n", mutex, uTimeout);
            }
        }
    }
	else
	{
		enRet = CSUDIOS_ERROR_BAD_PARAMETER;
	}

    return enRet;
}


/**
@brief �ͷŻ�����,����������

@param[in] hMutex ���������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ��ͼȥ�ͷ�û�м��������Ѿ������Ļ�����������CSUDIOS_ERROR_UNKNOWN_ERROR����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexRelease(CSUDI_HANDLE hMutex)
{
     CSUDI_Error_Code 		enRet = CSUDI_FAILURE;

    if(hMutex)
    {
        cs_mutex_t* mutex = (cs_mutex_t*) hMutex;

        DWORD  tid;
        tid = CSGetThreadId();

        if (mutex->owner != tid)
        {
            CSDEBUG(MODULE_NAME,WARN_LEVEL,"[USP_OSP] can't release mutex %08x, count=%d, owner=%x, curtask=%x\n", hMutex, mutex->count, mutex->owner, tid);
            return CSUDI_FAILURE;
        }

        if(mutex->count > 1 )
        {
            enRet = CSUDI_SUCCESS;
            --mutex->count;
            //CSDEBUG(MODULE_NAME,INFO_LEVEL,"[USP_OSP]: Release Mutex %xh  but  Count > 0\r\n", hMutex);
        }
        else if (mutex->count == 1)
        {
            //save
            DWORD pOwner = mutex->owner ;
            int nCount = mutex->count;

            //clear
            mutex->owner = (DWORD)CSUDI_NULL;
            mutex->count = 0;

            //release
            if((enRet=CSUDIOSSemRelease(mutex->semaphore)) != CSUDI_SUCCESS)
            {
                //failed then restore
                mutex->owner = pOwner;
                mutex->count = nCount;
                CSDEBUG(MODULE_NAME,ERROR_LEVEL,"[USP_OSP] can't relase semaphore for a mutex %08X !\n", hMutex);
            }
        }
        else
        {
            CSDEBUG(MODULE_NAME,WARN_LEVEL,"[USP_OSP] can't release mutex %08XH, count=%d!\n", hMutex, mutex->count);
        }
    }
	else
	{
		enRet = CSUDIOS_ERROR_BAD_PARAMETER;
	}

    return enRet;

}

/**
@brief ��ȡ��ϵͳ�����󾭹���ʱ��

@param[out] psTime ϵͳ�����󾭹���ʱ��,���CSUDIOSTimeVal_S���塣ΪCSUDI_NULL�򷵻�CSUDIOS_ERROR_BAD_PARAMETER
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note 
- Ҫ��ȷ������ϵͳ�����ṩ����󾫶ȣ����ٵ�����,������ܻ��ϵͳ��������Ԥ�ϵ�Ӱ��!
- ʵ����Eastwin����������ʹ�øýӿڻ��ʱ��Ĳ�ֵ�����ڻ�ȡ���ε��õ�ʱ�����������ʱ���0ʱ���Ƿ��Ǵӿ�����һ˲�����𲢲���Ҫ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetTime(CSUDIOSTimeVal_S * psTime)
{
	struct timeval stTime;

	if (psTime == CSUDI_NULL)
	{
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}
	
	gettimeofday(&stTime, NULL);

	psTime->m_nSecond = stTime.tv_sec;
	psTime->m_nMiliSecond = stTime.tv_usec/1000;
	psTime->m_lUSecond = stTime.tv_usec%1000;

	return CSUDI_SUCCESS;
}





/*******************************************************************
 **                     Event definitions                         **
 *******************************************************************/
/**
@brief �����¼�

@param[in] pcName �¼�����,���Ȳ��ܳ���32���ֽڣ�������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER�����Ҵ����¼�ʧ��
@param[in] uFlags �¼�ѡ��,��־��ͬʱʹ��,Ҳ�ɷֱ�ʹ�á�����˲���Ϊ����ֵ�򷵻�CSUDIOS_ERROR_BAD_PARAMETER��
@note uFlags ��ȡֵ����(��������):
- CSUDIOS_EVENT_INITIAL_STATUS ��ʼ�����¼�������Ч�������Ͽ���Wait����(����λΪ0���ʾ�¼���ҪSet����Ч)
- CSUDIOS_EVENT_AUTO_RESET CSUDIOSEventWait֮���¼��Զ���Ϊ��Ч��(����λΪ0���ʾWait�¼����¼���Ȼ��Ч����������Reset)

@note ����ζ�����uFlags��ȡֵΪ:
- CSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUS ��ʼ�����¼�������Ч�������Ͽ���Wait����Wait֮���¼��Զ���Ϊ��Ч��
- 0 ��ʼ���¼�֮����ҪSetʹ�¼���Ч�����¼���reset֮ǰ��Զ��Ч��
@see CSUDIOSEventSet CSUDIOSEventReset
@param[out] phEvent �¼����
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������,��phEventΪCSUDI_NULL
@note 
- pstrName����Ӱ�촴���¼��ɹ���񣬼�����ΪCSUDI_NULL�������ظ�,�����Ƽ�������
- �ж������ȴ�ͬһ���¼�ʱ��һ�����¼���Ч����ȷ���ĸ�����(��)�ȵ��¼�
- �¼�����Ӧ����ո�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/

typedef struct
{
    pthread_cond_t	condvar;
    pthread_mutex_t	mutex;
    int				signaled;
    int				manual_reset;
}T_CSOSPEvnt;


CSUDI_Error_Code CSUDIOSEventCreate(const char * pcName,unsigned int  uFlags,CSUDI_HANDLE * phEvent)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	T_CSOSPEvnt			*pEvent;
	DWORD 				dwNameLen = 0;
	int 					nRet;

	if(pcName != CSUDI_NULL)
	{
	    dwNameLen = strlen(pcName);
	}

	if (dwNameLen > 32 || phEvent == CSUDI_NULL)
	{
		CSASSERT(dwNameLen <=32);
		CSASSERT(phEvent != CSUDI_NULL);
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	*phEvent = CSUDI_NULL;
	pEvent = (T_CSOSPEvnt *)malloc(sizeof(T_CSOSPEvnt));
	CSASSERT(pEvent != NULL);
	if (pEvent != NULL)
	{
		if((uFlags&CSUDIOS_EVENT_AUTO_RESET) ==  CSUDIOS_EVENT_AUTO_RESET)
		{
		    pEvent->manual_reset = FALSE;
		}
		else
		{
		    //????????????????????????????????????????????????
		    // always manual reset now, need to be confirmed.
		    //????????????????????????????????????????????????
		    pEvent->manual_reset = TRUE;
		}

		if((uFlags&CSUDIOS_EVENT_INITIAL_STATUS) == CSUDIOS_EVENT_INITIAL_STATUS)
		{
		    pEvent->signaled = TRUE;
		}
		else
		{
		    pEvent->signaled = FALSE;
		}

		nRet = pthread_mutex_init( &pEvent->mutex, NULL );
		CSASSERT(nRet == 0);
		if(nRet == 0) 
		{
	    		nRet = pthread_cond_init( &pEvent->condvar, NULL );
			CSASSERT(nRet == 0);
			if (nRet == 0)
			{
				enRet = CSUDI_SUCCESS;
				*phEvent = (CSUDI_HANDLE)pEvent;
			}
		}
		
		if (enRet != CSUDI_SUCCESS)
		{
			free(pEvent);
		}
	}
	
	CSASSERT(enRet == CSUDI_SUCCESS);

	return enRet;
}


/**
@brief �����¼�

@param[in] hEvent �¼����
@return ���hEvent��Ч����CSUDI_SUCCESS;���hEvent��Ч�򷵻�CSUDIOS_ERROR_BAD_PARAMETER
@warning ����һ�����ڱ���������ȴ������¼���һ����ȷ������Ϊ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventDestroy(CSUDI_HANDLE hEvent)
{
   	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	T_CSOSPEvnt 			*event;
	int 					nRet;

	if (hEvent == CSUDI_NULL)
	{
		CSASSERT(hEvent != CSUDI_NULL);
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}
	
        event = (T_CSOSPEvnt *)hEvent;

        nRet = pthread_cond_broadcast( &event->condvar );
        CSASSERT(nRet == 0);
		
        nRet = pthread_cond_destroy( &event->condvar );
        CSASSERT(nRet == 0);

        nRet = pthread_mutex_destroy(&event->mutex);
        CSASSERT(nRet == 0);

   	free(event);
	enRet = CSUDI_SUCCESS;

	return enRet;
}



/**
@brief �����¼���Ч

�¼���Ч��CSUDIOSEventWait������ֱ���¼���Ч���߳�ʱ
@param[in] hEvent �¼����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ���CSUDI_FAILURE

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventReset(CSUDI_HANDLE hEvent)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	T_CSOSPEvnt 			*event;
	int 					nRet;

	if (hEvent == CSUDI_NULL)
	{
		CSASSERT(hEvent != CSUDI_NULL);
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}

	event = (T_CSOSPEvnt *)hEvent;

	nRet = pthread_mutex_lock(&event->mutex);

	event->signaled = FALSE;

	nRet += pthread_mutex_unlock(&event->mutex);

	CSASSERT(nRet == 0);

	if (nRet == 0)
	{
	    enRet = CSUDI_SUCCESS;
	}

	CSASSERT(enRet == CSUDI_SUCCESS);

	return enRet;
}

/**
@brief �����¼���Ч

�¼���Ч��CSWaitForSingleEvent����������
@param[in] hEvent �¼����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ���CSUDI_FAILURE

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventSet(CSUDI_HANDLE hEvent)
{
	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	T_CSOSPEvnt 			*event;
	int 					nRet;

	if (hEvent == CSUDI_NULL)
	{
		CSASSERT(hEvent != CSUDI_NULL);
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}
	
	event = (T_CSOSPEvnt *)hEvent;

	nRet = pthread_mutex_lock(&event->mutex);

	event->signaled = TRUE;
	/* Wake up one or all depending on whether the event is auto-reseting. */
	if( event->manual_reset )
	    nRet += pthread_cond_broadcast(&event->condvar);
	else
	    nRet += pthread_cond_signal(&event->condvar);

	nRet += pthread_mutex_unlock(&event->mutex);

	CSASSERT(nRet == 0);

	if (nRet == 0)
	{
	    enRet = CSUDI_SUCCESS;
	}


	CSASSERT(enRet == CSUDI_SUCCESS);

	return enRet;
}

/**
@brief �ȴ��¼�

�¼���Чʱ��������;��Чʱ��������ֱ���¼���Ч���߳�ʱ
@param[in] hEvent �¼����
@param[in] uTimeout �ȴ���ʱʱ�䣬��λΪ���룬Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return 
	   - ����ɹ�����CSUDI_SUCCESS
	   - ���hEvent���󷵻�CSUDIOS_ERROR_BAD_PARAMETER;
	   - ��hEvent��ȷ������£������ʱ�򷵻�CSUDIOS_ERROR_TIMEOUT��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventWait(CSUDI_HANDLE hEvent,unsigned int uTimeout)
{
    	CSUDI_Error_Code 		enRet = CSUDI_FAILURE;
	T_CSOSPEvnt 			*event;
	int 					nRet;

	if (hEvent == CSUDI_NULL)
	{
		CSASSERT(hEvent != CSUDI_NULL);
		return CSUDIOS_ERROR_BAD_PARAMETER;
	}
	
        event = (T_CSOSPEvnt *)hEvent;

        nRet = pthread_mutex_lock(&event->mutex);
        CSASSERT(nRet == 0);

        /* Return immediately if the event is signalled. */
        if(event->signaled)
        {
            if(!event->manual_reset)
            {
                event->signaled = FALSE;
            }

            nRet += pthread_mutex_unlock(&event->mutex);
            CSASSERT(nRet == 0);
            return( CSUDI_SUCCESS);
        }

        /* If just testing the state, return OSAL_TIMEOUT. */
        if( uTimeout == 0 )
        {
            nRet += pthread_mutex_unlock(&event->mutex);
            CSASSERT(nRet == 0);
            enRet = CSUDIOS_ERROR_TIMEOUT;
        }
        else if (uTimeout == 0xFFFFFFFF)
        {
            /* Wait for condition variable to be signaled or broadcast. */
            nRet = pthread_cond_wait(&event->condvar, &event->mutex);
            if (nRet == 0)
            {
                enRet = CSUDI_SUCCESS;
            }
            else
            {
                CSDEBUG(MODULE_NAME,ERROR_LEVEL, "[CSWaitForSingleEvent]ERROR: pthread_cond_wait(%d) return %d\n", uTimeout, nRet);
                enRet = CSUDI_FAILURE;
            }
        }
        else
        {
            struct timespec ts;
            struct timeval tv;

            nRet = gettimeofday (&tv, NULL);
            CSASSERT(nRet == 0);

            ts.tv_sec = tv.tv_sec + (int)(uTimeout/1000);
            ts.tv_nsec = tv.tv_usec*1000 + (int)((uTimeout%1000)*1000000);

            adjusttimespec(&ts);

            nRet = pthread_cond_timedwait(&event->condvar, &event->mutex, &ts);
            //CSDEBUG(MODULE_NAME,3, "nRet=%d, EINVAL:%d, ETIMEDOUT:%d\n", nRet, EINVAL, ETIMEDOUT);
            if (nRet == 0)
            {
                enRet = CSUDI_SUCCESS;
            }
            else if (nRet == ETIMEDOUT)
            {
                enRet = CSUDIOS_ERROR_TIMEOUT;
            }
            else
            {
                CSDEBUG(MODULE_NAME,ERROR_LEVEL, "[CSWaitForSingleEvent]ERROR: pthread_cond_timedwait(%d) return %d\n", uTimeout, nRet);
                enRet = CSUDI_FAILURE;
            }

        }

        if(CSUDI_SUCCESS == enRet)
        {
            if(!event->manual_reset)
            {
                event->signaled = FALSE;
            }
        }
        nRet = pthread_mutex_unlock(&event->mutex);
        CSASSERT(nRet == 0);

    CSASSERT(enRet == CSUDI_SUCCESS || enRet == CSUDIOS_ERROR_TIMEOUT);
    return enRet;
}


/*******************************************************************
 **                     Memory definitions                        **
 *******************************************************************/
/**
@brief �����ڴ�,������ʼ��

�൱��malloc,�·�����ڴ��������ǲ�ȷ����
@param[in] uSize ��������ڴ��С
@return �ɹ������ڴ�ָ��;ʧ�ܷ���CSUDI_NULL
@note uSize����0ʱ����������ʧ�ܣ��ҷ���ֵΪCSUDI_NULL��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void * CSUDIOSMalloc( unsigned int uSize )
{
		PVOID pvMem =  NULL;
	
		UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
	
    CSASSERT(uSize > 0);

    if(uSize > 0)
    {
        pvMem =  malloc(uSize);
    }

    CSASSERT(pvMem != NULL);
    if(NULL == pvMem)
    {

        CSDEBUG(MODULE_NAME,ERROR_LEVEL, "[CSMalloc]ERROR[errno=%d]: malloc %x fail\r\n", errno,uSize);
    }

    return pvMem;

}

/**
@brief ���·����ڴ�

�൱��realloc,���·����ڴ�Ϊ�µĴ�С,�����������������,������ڴ�ռ�ȾɵĴ�,���������ֵ������ǲ�ȷ����
@param[in] pvAddr �����·�����ڴ��ַ
@param[in] uSize ��������ڴ��С
@return �ɹ������ڴ�ָ��;ʧ�ܷ���CSUDI_NULL
@note 
- uSize����0ʱ�ú���ִ��ʧ��,������CSUDI_NULLҲ������һ����ַ��
- ���pvAddr����һ��֮ǰCSUDIOSMalloc,CSUDIOSRealloc���صĵ�ַ,����ǲ�ȷ����
- ���pvAddr==CSUDI_NULL,������CSUDIOSMallocһ��
- �������ʧ��,�ɵ��ڴ治���Զ��ͷ�,������Ҳ�������仯������ֵ������pvAddr��һ��,Ҳ����һ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void* CSUDIOSRealloc( void * pvAddr,unsigned int uSize )
{
		PVOID pvMem =  NULL;
		
		UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    if(uSize > 0)
    {
        pvMem =  realloc(pvAddr,uSize);
    }

    CSASSERT(pvMem != NULL);
    
    return pvMem;
    
}

/**
@brief �����ڴ棬����ʼ��

�൱��calloc������nElements��nElementSize��С��objects,�����ڴ�ȫ����Ϊ0
@param[in] uElements �������Ԫ�ظ���
@param[in] uElementSize �������ÿ��Ԫ�صĴ�С
@return ����ɹ������ڴ�ָ��;���ʧ�ܷ���ΪNULL
@note nElements��nElementSize����0ʱ�ķ���ֵ�ǲ�ȷ���ģ�������CSUDI_NULLҲ������һ����ַ�� 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void* CSUDIOSCalloc(unsigned int uElements, unsigned int uElementSize)
{
	PVOID pvMem =  NULL;
	
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    CSASSERT(nElements > 0);
    CSASSERT(nElementSize > 0);

    if((uElements>0) && (uElementSize>0))
    {
        pvMem =  calloc(uElements,uElementSize);
    }

    CSASSERT(pvMem != NULL);
    if(NULL == pvMem)
    {
        CSDEBUG(MODULE_NAME,ERROR_LEVEL, "[CSCalloc]ERROR[errno=%d]: calloc %x %x fail\r\n", errno,uElements,uElementSize);
    }

    return pvMem;

}

/**
@brief �ͷŷ�����ڴ�

�൱��free,�ͷ�CSUDIOSMalloc,CSUDIOSRealloc,CSUDIOSCalloc������ڴ�
@param[in] pvAddr ���ͷŵ��ڴ��ַ
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ���CSUDI_FAILURE
@note 
- ���pvAddr����һ��֮ǰCSUDIOSMalloc,CSUDIOSRealloc,CSUDIOSCalloc���صĵ�ַ,����ǲ�ȷ����
- ����ͷ�ͬһ���ڴ潫����ʧ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSFree( void * pvAddr )
{
    if (pvAddr != NULL)
    {
        free(pvAddr);
    }

    return CSUDI_SUCCESS;
}


/**
@brief ��ȡ���õ�RAM�ռ�
@param[out] pnFreeCount ϵͳ���õ�RAM�ռ�
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetFreeRAM(unsigned int * puFreeCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	
	if(puFreeCount == CSUDI_NULL)
		Retcode = CSUDIOS_ERROR_BAD_PARAMETER;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡCPU����
@param[out] pnCpuCount CPU�ĸ���
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetCPUCount(unsigned int * puCpuCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	
	if(puCpuCount == CSUDI_NULL)
		Retcode = CSUDIOS_ERROR_BAD_PARAMETER;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡָ������CPU�Ĵ�����������(%)
@param[out] pnUsage CPU�����������ʣ�ȡֵ��ΧΪ0--1000;
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetCPUUsage(unsigned int index, unsigned int * puUsage)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	if(puUsage == CSUDI_NULL)
		Retcode = CSUDIOS_ERROR_BAD_PARAMETER;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡϵͳ����������ľ��
@param[out]  hThread ϵͳ����������ľ������
@param[in/out]   nSize hThread����Ĵ�С������ʵ��Thread�ĸ���
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetAllThreadHandle(CSUDI_HANDLE* phThread, unsigned int*  puSize)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	if((phThread == CSUDI_NULL) || (puSize == CSUDI_NULL))
		Retcode = CSUDIOS_ERROR_BAD_PARAMETER;
	
	if(*puSize == 0)
		Retcode = CSUDIOS_ERROR_BAD_PARAMETER;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡָ������Ļ�����Ϣ�������������ơ�����ID���������ȼ�
@param[in]   hThread ָ������ľ��
@param[out]  psThreadInfo ����Ļ�����Ϣ
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��psThreadInfo��ΪNULL��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadInfo(CSUDI_HANDLE hThread, CSUDIOSThreadInfo_S* psThreadInfo)
{
		//CSUDI_FAILURE
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	
	if((psThreadInfo == CSUDI_NULL) || (hThread == CSUDI_NULL))
		Retcode = CSUDIOS_ERROR_BAD_PARAMETER;
	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡָ�������RAM����
@param[in]   hThread ָ������ľ��
@param[out]  pnUsedRam ָ�������RAM����
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadUsedRAM(CSUDI_HANDLE hThread, unsigned int * puUsedRam)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	if((puUsedRam == CSUDI_NULL) || (hThread == CSUDI_NULL))
		Retcode = CSUDIOS_ERROR_BAD_PARAMETER;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡָ�������CPU������(%)
@param[in]   hThread ָ������ľ��
@param[out] pnUsage ָ�������CPU���أ�ȡֵ��ΧΪ:0--1000
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadCPUUsage(CSUDI_HANDLE hThread, unsigned int * puUsage)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	if((puUsage == CSUDI_NULL) || ((hThread == CSUDI_NULL)))
		Retcode = CSUDIOS_ERROR_BAD_PARAMETER;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}
