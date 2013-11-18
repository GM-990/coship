/*Include head files*/
#include <stdlib.h>
#include <string.h>
#include"cs_ostest.h"

CSUDI_BOOL g_bThreadEnd = CSUDI_FALSE;
CSUDI_BOOL s_bSupportSuspend = CSUDI_FALSE;
int g_nTestData = 0;
int g_nCPUCount = 0;
int g_nRAMSize = 0;

CSUDI_BOOL CSTC_OS_InitCfg(void)
{
	char cOSCfgBuf[32];

	memset(cOSCfgBuf,'\0',sizeof(cOSCfgBuf));
	if(CSTKGetConfigInfo("OS", "CS_OSUDI2_SUPPORT_SUSPEND", cOSCfgBuf, sizeof(cOSCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		if(1 == CSTKGetIntFromStr(cOSCfgBuf,10))
		{
			s_bSupportSuspend = CSUDI_TRUE;
		}
		else
		{
			s_bSupportSuspend = CSUDI_FALSE;
		}
	}
	
	memset(cOSCfgBuf,'\0',sizeof(cOSCfgBuf));
	if(CSTKGetConfigInfo("OS", "CS_OSUDI2_CPU_COUNT", cOSCfgBuf, sizeof(cOSCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		g_nCPUCount = CSTKGetIntFromStr(cOSCfgBuf, 10);
	}

	memset(cOSCfgBuf,'\0',sizeof(cOSCfgBuf));
	if(CSTKGetConfigInfo("OS", "CS_OSUID2_RAM_SIZE", cOSCfgBuf, sizeof(cOSCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		g_nRAMSize = CSTKGetIntFromStr(cOSCfgBuf, 16);
	}
	return CSUDI_TRUE;
}

/**************************************************************
The function for Thread module test
**************************************************************/
void OS_TestThread1Entry(void * pvParam)
{
	CSUDIOSThreadSleep(10);
	
	g_bThreadEnd = CSUDI_TRUE;
}

void OS_TestThread1_1Entry(void *pvParam)
{
	int i;
	long sum = 0;
	for ( i = 0; i < 10000; ++i)
	{
		sum += i;
	}

	g_bThreadEnd = CSUDI_TRUE;
}

void OS_TestThread2Entry(void *pvParam)
{
	char *cParam;
	
	char acArray[255*1024];

	cParam = (char *)pvParam;
	
	memset(acArray,'F',sizeof(acArray));
	
	CSUDIOSThreadSleep(20);
	
	acArray[255*1024 -1] = 'A';
	
	*cParam = acArray[255*1024 -1];

	g_bThreadEnd = CSUDI_TRUE;
}

void OS_TestThread3_1Entry(void *pvParam)
{
	int * pnArg;
	pnArg = (int*)pvParam;
	
	if (*pnArg == 0x00000000)
	{
		*pnArg = 1;
	}
	pnArg = CSUDI_NULL;
}

void OS_TestThread3_2Entry(void *pvParam)
{	
	int * pnArg;
	pnArg = (int*)pvParam;
	
	if (*pnArg == 0xffffffff)
	{
		*pnArg = 2;
	}
	pnArg = CSUDI_NULL;
}

void OS_TestThread3_3Entry(void *pvParam)
{
	int * pnArg;
	pnArg = (int*)pvParam;
	
	if (*pnArg == 0x8acdefc2)
	{
		*pnArg = 3;
	}
	pnArg = CSUDI_NULL;
}

void OS_TestThread4Entry(void *pvParam)
{
	int *pnArg;
	pnArg = (int*)pvParam;

	while(!g_bThreadEnd)
	{
		CSUDIOSThreadSleep(10);
	}
	*pnArg = 1;
	pnArg = CSUDI_NULL;
}

void OS_TestThread5_1Entry(void *pvParam)
{
	int * pnArg;
	pnArg = (int*)pvParam;

	while(!g_bThreadEnd)
	{
		CSUDIOSThreadSleep(10);
	}

	*pnArg = 1;
	pnArg = CSUDI_NULL;
}

void OS_TestThread5_2Entry(void *pvParam)
{
	int * pnArg;
	pnArg = (int*)pvParam;
	
	while(!g_bThreadEnd)
	{
		CSUDIOSThreadSleep(10);
	}

	*pnArg = 1;
	pnArg = CSUDI_NULL;
}

void OS_TestThread6Entry(void *pvParam)
{
	int i;
	for (i=0 ; i<10 ; i++)
	{
		CSUDIOSThreadSleep(10);
	}
	g_bThreadEnd = CSUDI_TRUE;
}

void OS_TestThread7Entry(void *pvParam)
{
	CSUDIOSThreadSelf(pvParam);
}

void OS_TestThread8_1Entry(void *pvParam)
{
	while(g_bThreadEnd == CSUDI_FALSE)
	{
		CSUDIOSThreadYield();
	}
}

void OS_TestThread8_2Entry(void *pvParam)
{
	int a_nArray[10];
	int i;
	
	for(i=0 ; i<10 ; i++)
	{
		a_nArray[i] = i;
	}

	g_bThreadEnd = CSUDI_TRUE;
}

/**************************************************************
The function for MsgQ module test
**************************************************************/
void OS_TestMsgQThread1Entry(void *pvParam)
{
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	
	if(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(pvParam, &nMsg, nMsgBytes, CSUDIOS_TIMEOUT_IMMEDIATE))
	{
		CSUDIOSThreadSleep(10);

		if(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(pvParam, &nMsg, nMsgBytes, 3000))
		{
			g_bThreadEnd = CSUDI_TRUE;
		}

	}
}

void OS_TestMsgQThread2Entry(void *pvParam)
{
	int anMsg[1];
	int nMsgBytes = 4;

	CSUDIOSThreadSleep(2000);

	CSUDIOSMsgQueueReceive(pvParam,anMsg,nMsgBytes,100);
}

void OS_TestMsgQThread3Entry(void *pvParam)
{
    char acRecvMsg[5] = {0};
    
    if( CSUDI_NULL == pvParam)
    {
        CSTCPrint("[%s,%d] pvParam is¡NNULL\n", __FILE__, __LINE__);
    
        return;
    }
    
    CSTCPrint("ÇëµÈ´ý10s......\n");
    
    CSUDIOSThreadSleep(10000);
    
    g_bThreadEnd = CSUDI_TRUE;
    
    if(CSUDI_SUCCESS != CSUDIOSMsgQueueReceive(pvParam, acRecvMsg, 4, CSUDIOS_TIMEOUT_IMMEDIATE))
    {
		CSTCPrint("[%s,%d]CSUDIOSMsgQueueReceive failed \n", __FILE__, __LINE__);

        return;
    }
}

void OS_TestMsgQThread4Entry(void *pvParam)
{
    char acSendMsg[5] = "abcd";
    
    if( CSUDI_NULL == pvParam)
    {
        CSTCPrint("[%s,%d] pvParam is¡NNULL\n", __FILE__, __LINE__);

        return;
    }
    CSTCPrint("ÇëµÈ´ý10s......\n");
    
    CSUDIOSThreadSleep(10000);
    
    g_bThreadEnd = CSUDI_TRUE;
    
    if(CSUDI_SUCCESS != CSUDIOSMsgQueueSend(pvParam, acSendMsg, 4, CSUDIOS_TIMEOUT_IMMEDIATE))
    {
        CSTCPrint("[%s,%d]CSUDIOSMsgQueueSend failed \n", __FILE__, __LINE__);

        return;
    }
}

/**************************************************************
The function for Semaphore module test
**************************************************************/

void OS_TestSemThreadEntry(void *pvParam)
{
	CSUDIOSThreadSleep(2000);

	CSUDIOSSemRelease(pvParam);
}

void OS_TestSemThread1Entry(void *pvParam)
{
    if( CSUDI_NULL == pvParam)
    {
        CSTCPrint("[%s,%d] pvParam is¡NNULL\n", __FILE__, __LINE__);

        return;
    }
    CSTCPrint("ÇëµÈ´ý10s......\n");
    
    CSUDIOSThreadSleep(10000);

    g_bThreadEnd = CSUDI_TRUE;

    if(CSUDI_SUCCESS != CSUDIOSSemRelease(pvParam))
    {
        CSTCPrint("[%s,%d]CSUDIOSSemRelease failed \n", __FILE__, __LINE__);

        return;
    }
}

/**************************************************************
The function for Mutex module test
**************************************************************/

void OS_TestMutexThread1Entry(void *pvParam)
{
	CSUDIOSMutexWait(pvParam, 2000);

	g_bThreadEnd = CSUDI_TRUE;

	CSUDIOSMutexRelease(pvParam);
}

void OS_TestMutexThread2_1Entry(void *pvParam)
{
	CSUDIOSThreadSleep(1000);

	CSUDIOSMutexWait(pvParam,100);

	CSUDIOSThreadSleep(6000);

	g_nTestData = 1;

	CSUDIOSMutexRelease(pvParam);
}

void OS_TestMutexThread2_2Entry(void *pvParam)
{
	CSUDIOSThreadSleep(2000);

	CSUDIOSMutexWait(pvParam,9000);

	CSUDIOSThreadSleep(4000);

	g_nTestData = 2;

	CSUDIOSMutexRelease(pvParam);
}

void OS_TestMutexThread2_3Entry(void *pvParam)
{
	CSUDIOSThreadSleep(8000);

	CSUDIOSMutexWait(pvParam,15000);

	CSUDIOSThreadSleep(2000);

	g_nTestData = 3;

	CSUDIOSMutexRelease(pvParam);
}

void OS_TestMutexThread3Entry(void *pvParam)
{
	if(CSUDI_SUCCESS == CSUDIOSMutexWait(pvParam,1500))
	{
		g_nTestData = 1;
	}
	else
	{
		g_nTestData = 2;
	}
	
}

void OS_TestMutexThread4Entry(void *pvParam)
{
	CSUDIOSMutexWait(pvParam, 20);

	g_bThreadEnd = CSUDI_TRUE;

	CSUDIOSThreadSleep(1000);

	CSUDIOSMutexRelease(pvParam);
}

void OS_TestMutexThread5Entry(void *pvParam)
{
	if(CSUDI_NULL == pvParam)
    {
		CSTCPrint("[%s,%d]pvParam is NULL.\n", __FILE__, __LINE__);

        return;
    }
	
    if(CSUDI_SUCCESS != CSUDIOSMutexWait(pvParam, CSUDIOS_TIMEOUT_IMMEDIATE))
    {
        CSTCPrint("[%s,%d]CSUDIOSMutexWait failed \n", __FILE__, __LINE__);
        
        return;
    }

    CSTCPrint("ÇëµÈ´ý10s......\n");
    
    CSUDIOSThreadSleep(10000);
    
    g_bThreadEnd = CSUDI_TRUE;

    if(CSUDI_SUCCESS != CSUDIOSMutexRelease(pvParam))
    {
		CSTCPrint("[%s,%d]CSUDIOSMutexRelease failed \n", __FILE__, __LINE__);
    
		return;
    }
}

/**************************************************************
The function for Event module test
**************************************************************/
void OS_TestEventThread1Entry(void *pvParam)
{
	CSUDIOSEventWait(pvParam,1500);
	
	g_bThreadEnd = CSUDI_TRUE;
}

void OS_TestEventThread2Entry(void *pvParam)
{
	if(CSUDI_SUCCESS == CSUDIOSEventWait(pvParam,1000))
	{
		g_nTestData = 1;

		CSUDIOSThreadSleep(2000);
	}

	CSUDIOSEventWait(pvParam,3000);

	g_nTestData = 2;
}

void OS_TestEventThread3Entry(void *pvParam)
{
	CSUDIOSEventWait(pvParam,1000);

	g_nTestData = 1;

	CSUDIOSThreadSleep(100);

	CSUDIOSEventWait(pvParam,2000);

	g_nTestData = 2;
}

void OS_TestEventThread4Entry(void *pvParam)
{
    if(CSUDI_NULL == pvParam)
    {
        CSTCPrint("[%s,%d]pvParam is NULL.\n", __FILE__, __LINE__);

        return;
    }
    
    CSTCPrint("ÇëµÈ´ý10s......\n");
    
    CSUDIOSThreadSleep(10000);
	
    g_bThreadEnd = CSUDI_TRUE;

	if(CSUDI_SUCCESS != CSUDIOSEventSet(pvParam))
    {
		CSTCPrint("[%s,%d]CSUDIOSEventSet failed \n", __FILE__, __LINE__);
    
		return;
    }
}

