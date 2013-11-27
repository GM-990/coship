#include "udiplus_os.h"
#include "udiplus_debug.h"
#include <string.h>


/*******************************************************************
 **                     Timer definitions                         **
 *******************************************************************/
#define CS_TIMER_LIMIT			128   
#define CS_TIMER_MAGIC			0xB6A8CAB1
#define TASK_PRIOR_STEP			(4*17)
#define OS_TRYLOCK_MINITICK 	40
#define MODULE_NAME   			"UDIPLusTimer"

#define TIMER_LOCK(handle)		{ CSUDIOSMutexWait(handle, 2000);}
#define TIMER_UNLOCK(handle)	{ CSUDIOSMutexRelease(handle);}

typedef struct _CSUDIPLUSOSTIMERNode_S
{
	CSUDI_UINT32						dwMagicNumber;
	unsigned int						nStartTime;
	int									nTimerOut;
	CSUDIPLUSOSTimerMode_E				eMode;
	CSUDI_BOOL							bEnabled;
	CSUDIOSTimerCallback_F				pEntry;
	void*								pvArg;
	struct _CSUDIPLUSOSTIMERNode_S*		prev;
	struct _CSUDIPLUSOSTIMERNode_S*		next;  
} CSUDIPLUSOSTIMERNode_S;

static int	g_nTimerCounts = 0;
static CSUDIPLUSOSTIMERNode_S*	g_pTimerListStart = CSUDI_NULL;
static CSUDIPLUSOSTIMERNode_S*	g_pTimerListEnd = CSUDI_NULL;
static CSUDI_HANDLE	g_hTimerMutex = CSUDI_NULL;
static CSUDI_HANDLE g_astTimerHandle[CS_TIMER_LIMIT] = {0};

static CSUDI_BOOL CSCheckTimerHandle(CSUDI_HANDLE hTimer);
static CSUDI_BOOL CSInitTimer(void);
static void CSTIMTaskEntry(void*);
static void CSTIMPeriodHandler(void);

static CSUDI_BOOL CSCheckTimerHandle(CSUDI_HANDLE hTimer)
{
	int ii;

	for(ii = 0; ii < CS_TIMER_LIMIT; ii++)
	{
		if(g_astTimerHandle[ii] == hTimer)
			return CSUDI_TRUE;
	}

	return CSUDI_FALSE;
}

static CSUDI_BOOL CSInitTimer(void)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	if (g_hTimerMutex != CSUDI_NULL)
	{
		//CSDEBUG(MODULE_NAME,INFO_LEVEL,"Timer Module Already Installed!\n");
		bRet = CSUDI_TRUE;
	}
	else
	{
		CSUDI_Error_Code tError;
		CSUDI_HANDLE	hThread;
		

		tError = CSUDIOSMutexCreate("UdiPlusTimer",CSUDIOS_MUTEX_OBTAIN,&g_hTimerMutex);
		if(CSASSERT_FAILED(tError == CSUDI_SUCCESS))
		{
			bRet = CSUDI_FALSE;
		}
		else
		{
			g_nTimerCounts		= 0;			/* Number of Timers Created */
			g_pTimerListStart	= CSUDI_NULL;			/* Head-of-the-List Timer */
			g_pTimerListEnd		= CSUDI_NULL;			/* End-of-the-List Timer */
			
			tError = CSUDIOSThreadCreate("UdiPlusTimer",TASK_PRIOR_STEP,1024*128,CSTIMTaskEntry,CSUDI_NULL,&hThread);
			if(CSASSERT_FAILED(tError == CSUDI_SUCCESS))
			{
				bRet = CSUDI_FALSE;
			}
			
			TIMER_UNLOCK(g_hTimerMutex);
			bRet = CSUDI_TRUE;
		}
	}
	
	return bRet;
}

CSUDI_Error_Code CSUDIPlusOSTimerCreate(CSUDIPLUSOSTimerMode_E eMode, CSUDIOSTimerCallback_F pCallback, void* pvArg, CSUDI_HANDLE *phTimer)
{
	CSUDI_Error_Code tError = CSUDI_FAILURE;

	if(!CSInitTimer())
	{
		return CSUDI_FAILURE;
	}

    if(pCallback == CSUDI_NULL 
		|| eMode >= EM_UDIPLUS_OS_TIMERMODE_MAX
		|| phTimer == CSUDI_NULL)
    {
		CSDEBUG(MODULE_NAME,NOTICE_LEVEL,"Try to create too many timers.\n");

		tError = CSUDIOSPLUS_ERROR_BAD_PARAMETER;
    }
    else if(g_nTimerCounts >= CS_TIMER_LIMIT)
	{
		CSDEBUG(MODULE_NAME,NOTICE_LEVEL,"Try to create too many timers.\n");
		
		tError = CSUDIOSPLUS_ERROR_NO_MEMORY;
	}
	else
	{
		CSUDI_BOOL bQueryTimer = CSUDI_FALSE;
		CSUDIPLUSOSTIMERNode_S* pTimer = CSUDI_NULL;
		int ii;

		TIMER_LOCK(g_hTimerMutex);

		pTimer = g_pTimerListStart;

		while(pTimer != CSUDI_NULL)
		{
			if (pTimer->dwMagicNumber == 0)
			{
				bQueryTimer = CSUDI_TRUE;

				break;
			}

			pTimer = pTimer->next;
		}

		if (!bQueryTimer)
		{		
			pTimer = (CSUDIPLUSOSTIMERNode_S*)CSUDIOSMalloc(sizeof(CSUDIPLUSOSTIMERNode_S));
		}

		CSASSERT(pTimer != CSUDI_NULL);

		pTimer->pEntry		= pCallback;
		pTimer->pvArg		= pvArg;
		pTimer->nTimerOut	= 0;
		pTimer->nStartTime	= 0;
		pTimer->bEnabled	= CSUDI_FALSE;
		pTimer->eMode		= eMode;
		pTimer->dwMagicNumber = CS_TIMER_MAGIC;
		
		if (!bQueryTimer)
		{			
			/* This is the first timer in the list */
			if(g_pTimerListStart == CSUDI_NULL)
			{
				g_pTimerListStart = pTimer;
				pTimer->prev = CSUDI_NULL;
			}
			else /* Add the timer to the end of the list */
			{
				g_pTimerListEnd->next = pTimer;
				pTimer->prev = g_pTimerListEnd;
			}
			pTimer->next = CSUDI_NULL;
			
			g_pTimerListEnd = pTimer;			
		}

		g_nTimerCounts++;
		
		for(ii = 0; ii < CS_TIMER_LIMIT; ii++)
		{
			if(g_astTimerHandle[ii]  == 0)
			{
				g_astTimerHandle[ii] = (CSUDI_HANDLE)pTimer;
				break;
			}
		}

		CSASSERT(ii < CS_TIMER_LIMIT);
		if(ii < CS_TIMER_LIMIT)
		{
			*phTimer = g_astTimerHandle[ii];
		}

		TIMER_UNLOCK(g_hTimerMutex);

		tError = CSUDI_SUCCESS;
	}

	return tError;
}

CSUDI_Error_Code CSUDIPlusOSTimerStart(CSUDI_HANDLE hTimer, unsigned int unMilliSeconds)
{
	CSUDI_Error_Code tError = CSUDI_FAILURE;
	
	TIMER_LOCK(g_hTimerMutex);
	
	if(CSUDI_FALSE == CSCheckTimerHandle(hTimer))
	{
		tError = CSUDIOSPLUS_ERROR_BAD_PARAMETER;
	}
	
	CSASSERT(hTimer != CSUDI_NULL && unMilliSeconds != 0);

	if (hTimer != CSUDI_NULL && unMilliSeconds != 0)
	{	
		CSUDIPLUSOSTIMERNode_S* pTimer = (CSUDIPLUSOSTIMERNode_S*)hTimer;

		if (pTimer->dwMagicNumber!= CS_TIMER_MAGIC)
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Try to start a not-created timer.\n");		
			
			tError = CSUDIOSPLUS_ERROR_BAD_PARAMETER;
		}
		else
		{
			unsigned int uNow = 0;
			uNow = CSUDIPlusOSGetTickCount();
			if(uNow > 0)
			{				
				pTimer->nTimerOut	= unMilliSeconds;
				pTimer->nStartTime	= uNow;
				pTimer->bEnabled	= CSUDI_TRUE;

				tError = CSUDI_SUCCESS;
			}
			else
			{
				CSASSERT(0);
			}
		}		
	}

	TIMER_UNLOCK(g_hTimerMutex);

	return tError;
}

CSUDI_Error_Code CSUDIPlusOSTimerStop(CSUDI_HANDLE hTimer)
{
	CSUDI_Error_Code tError = CSUDI_FAILURE;
		
	TIMER_LOCK(g_hTimerMutex);
	
	if(CSUDI_FALSE == CSCheckTimerHandle(hTimer))
	{
		tError = CSUDIOSPLUS_ERROR_BAD_PARAMETER;
	}
	
	CSASSERT(hTimer != CSUDI_NULL);

	if (hTimer != CSUDI_NULL)
	{	
		CSUDIPLUSOSTIMERNode_S* pTimer = (CSUDIPLUSOSTIMERNode_S*)hTimer;

		if (pTimer->dwMagicNumber != CS_TIMER_MAGIC)
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Try to stop a not-created timer.\n");		
			
			tError = CSUDIOSPLUS_ERROR_BAD_PARAMETER;
		}
		else
		{
			unsigned int uNow = 0;
			uNow = CSUDIPlusOSGetTickCount();
			pTimer->bEnabled = CSUDI_FALSE;
			pTimer->nStartTime = uNow;

			tError = CSUDI_SUCCESS;
		}
	}
	
	TIMER_UNLOCK(g_hTimerMutex);
	return tError;
}

CSUDI_Error_Code CSUDIPlusOSTimerReset(CSUDI_HANDLE hTimer)
{
	CSUDI_Error_Code tError = CSUDI_FAILURE;
	
	TIMER_LOCK(g_hTimerMutex);
	
	if(CSUDI_FALSE == CSCheckTimerHandle(hTimer))
	{
		tError = CSUDIOSPLUS_ERROR_BAD_PARAMETER;
	}
	
	CSASSERT(hTimer != CSUDI_NULL);

	if (hTimer != CSUDI_NULL)
	{	
		CSUDIPLUSOSTIMERNode_S* pTimer = (CSUDIPLUSOSTIMERNode_S*)hTimer;
		
		if (pTimer->dwMagicNumber != CS_TIMER_MAGIC)
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Try to stop a not-created timer.\n");		
			
			tError = CSUDIOSPLUS_ERROR_BAD_PARAMETER;
		}
		else
		{
			unsigned int uNow = 0;
			uNow = CSUDIPlusOSGetTickCount();
			if(uNow > 0)
			{
				pTimer->nStartTime	= uNow;
				tError = CSUDI_SUCCESS;
			}
			else
			{
				CSASSERT(0);
			}
		}
	}

	TIMER_UNLOCK(g_hTimerMutex);
	return tError;
}

CSUDI_Error_Code CSUDIPlusOSTimerDestroy(CSUDI_HANDLE hTimer)
{
	CSUDI_Error_Code tError = CSUDI_FAILURE;
	
	TIMER_LOCK(g_hTimerMutex);
	
	if(CSUDI_FALSE == CSCheckTimerHandle(hTimer))
	{
		tError = CSUDIOSPLUS_ERROR_BAD_PARAMETER;
	}
	
	CSASSERT(hTimer != CSUDI_NULL);

	if (hTimer != CSUDI_NULL)
	{	
		CSUDIPLUSOSTIMERNode_S* pTimer = (CSUDIPLUSOSTIMERNode_S*)hTimer;
		
		if (pTimer->dwMagicNumber== CS_TIMER_MAGIC)
		{
			pTimer->dwMagicNumber = 0;
			pTimer->bEnabled	= CSUDI_FALSE;			
			pTimer->pEntry		= CSUDI_NULL;
			pTimer->nStartTime	= 0;
			pTimer->nTimerOut	= 0;
			g_nTimerCounts --; /* Update total timer count */
			{
				int ii;

				for(ii = 0; ii < CS_TIMER_LIMIT; ii++)
				{
					if(g_astTimerHandle[ii] == hTimer)
					{
						g_astTimerHandle[ii] = 0;
						break;
					}
				}
			}

			tError = CSUDI_SUCCESS;
		}
		else
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Try to delete a not-created timer.\n");	
		}
	}

	TIMER_UNLOCK(g_hTimerMutex);
	return tError;
}
////////////////////////////////////////////////////////

static void CSTIMTaskEntry(void* pvParam)
{
    for(;;)
    {
		CSUDIOSThreadSleep(OS_TRYLOCK_MINITICK);
		CSTIMPeriodHandler();
    }
}

static void CSTIMPeriodHandler(void)
{   
	CSUDIPLUSOSTIMERNode_S	*pRunningTimer;
	unsigned int	uNow = 0;
	CSUDIOSTimerCallback_F	pCallback = CSUDI_NULL;
	void*			pUserData = CSUDI_NULL;

	if((pRunningTimer = g_pTimerListStart) == CSUDI_NULL)  /* Timer List is Empty */
		return;

	while(pRunningTimer != CSUDI_NULL && g_nTimerCounts != 0)
    {
		TIMER_LOCK(g_hTimerMutex);

		if (!pRunningTimer->bEnabled || (pRunningTimer->dwMagicNumber != CS_TIMER_MAGIC))
		{			
			pRunningTimer = pRunningTimer->next;
			
			TIMER_UNLOCK(g_hTimerMutex);

			continue;
		}

		pCallback = CSUDI_NULL;
	
		uNow = CSUDIPlusOSGetTickCount();

		if(((uNow > pRunningTimer->nStartTime) && (uNow >= (pRunningTimer->nStartTime + pRunningTimer->nTimerOut))) //timer 到时间了
			||(((uNow < pRunningTimer->nStartTime) && (0xffffffff - pRunningTimer->nStartTime + uNow) >= pRunningTimer->nTimerOut)))	//时间溢出
		{					
			if(pRunningTimer->eMode == EM_UDIPLUS_OS_TIMERMODE_REPEAT)
			{
				pRunningTimer->nStartTime = uNow;
			}
			else
			{
				pRunningTimer->bEnabled = CSUDI_FALSE; 
			}
			
			pCallback = pRunningTimer->pEntry ;
			pUserData = pRunningTimer->pvArg;		
		}

		pRunningTimer = pRunningTimer->next;

		TIMER_UNLOCK(g_hTimerMutex);

		if (pCallback != CSUDI_NULL)
		{
			pCallback(pUserData);
		}       
	}
}

/////////////////////////////////////////////////////////////
//Set system date, zzp.2006.10.19


