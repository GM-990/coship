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
#if 0
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
	
        nRetVal = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
        CSASSERT(nRetVal == 0);

        nRetVal = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        CSASSERT(nRetVal == 0);
        
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return;

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
CSUDI_Error_Code CSUDIOSMsgQueueCreate(const char * pcName,int nMaxMsgs,int nSizePerMsg,CSUDI_HANDLE * phMsgQueue)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;

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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
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
CSUDI_Error_Code CSUDIOSEventCreate(const char * pcName,unsigned int  uFlags,CSUDI_HANDLE * phEvent)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
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
	
    CSASSERT(nMemorySize > 0);

    if(nMemorySize > 0)
    {
        pvMem =  malloc(nMemorySize);
    }

    CSASSERT(pvMem != NULL);
    if(NULL == pvMem)
    {

        CSDEBUG(MODULE_NAME,ERROR_LEVEL, "[CSMalloc]ERROR[errno=%d]: malloc %x fail\r\n", errno,nMemorySize);
    }
    
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);  
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
    
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);
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

    if((nElements>0) && (nElementSize>0))
    {
        pvMem =  calloc(nElements,nElementSize);
    }

    CSASSERT(pvMem != NULL);
    if(NULL == pvMem)
    {
        CSDEBUG(MODULE_NAME,ERROR_LEVEL, "[CSCalloc]ERROR[errno=%d]: calloc %x %x fail\r\n", errno,nElements,nElementSize);
    }

		UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);  
		
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
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
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
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
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

#endif