/****************************************************************************/
/*                   CONEXANT PROPRIETARY AND CONFIDENTIAL                  */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Conexant Systems India Pvt Ltd. 2004-2008                */
/*                                Hyderabad, India                                */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        NXP_cs_smc.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 * Author:          Santhosh Reddy K
 *
 ****************************************************************************/
/* $Id:  NXP_cs_i2c.c 95092 2009-08-14 21:16:57Z whiteth $
 ****************************************************************************/

/*****************/
/* Include Files */
/*****************/
#include "cnxt_types.h"
#include "generic_include.h"

#include "udi2_smc.h"
#include "udiplus_debug.h"
#include "udiplus_os.h"
#include "smc.h"
#include "stdio.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~TODO: move to header file start~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define SCUDI_MAX_SMC_UNIT_NUMBER   1 /*max 2 slots*/
#define MAX_SMC_HANDLES SCUDI_MAX_SMC_UNIT_NUMBER /*????*/
#define SCUDI_INVALID_DEV_NUM       SCUDI_MAX_SMC_UNIT_NUMBER

/*TODO: move the type define to smc header file*/
typedef  struct __CS_CNXT_SMC_SubSystem
{
   CNXT_SMC_HANDLE          smcHandle;
    int                     state;/**/
    CSUDISMCCallback        fnCallBack;
    CSUDISMCParam_S         smcParam_s;
    /*All other parameters to be filled as required */
    u_int32 	            deviceno;

    CNXT_SEM_ID protect_sem; /*mutex usage, to protect global resource*/
 }CS_CNXT_SMC_SubSystem;

typedef struct __CS_CNXT_SMC_Config
{
    bool                    bInit; /*TRUE: inited; FALSE:unkown*/

	CS_CNXT_SMC_SubSystem   hSMCHandle[MAX_SMC_HANDLES];
	u_int32 count;
}CS_CNXT_SMC_Config;

#define TIMER_NOTCREAT (1)
#define TIMER_CREATED  (2)
#define TIMER_RUNING   (3)
#define TIMER_STOP     (4)


typedef struct 
{
	u_int32 m_dw1st;
	u_int32 m_dw2nd;
	u_int32 m_dw3rd;
	u_int32 m_dw4th;
}T_CSSmcMsg;

#define  MODULE_NAME   "CS_SYSSMC"
#define  SMC_LEVEL      DEBUG_LEVEL

#define SMCT_MAX_ATR_LENGTH       32 
#define SMC_DEFAULT_CLK_FREQ      1000000 // 3579000 default clock freq before reset
#define SMC_RESET_TIMEOUT         2000    //1200 /*in ms*/
#define SMC_POWERDOWN_TIMEOUT     20      /*in ms*/
#define SMC_RW_DEFAULT_TIMEOUT    4000    //2000//1200 /*in ms*/
#define SMC_POWERDOWN_SEM_TIMEOUT 100

/*to check whether card handle already been opened*/
#define SCUDI_SMC_CHECK_CARD_OPEN(smcHandle)            \
if(smcHandle==0)                                        \
{                                                       \
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: not open yet.\n");   \
    return CSUDISMC_ERROR_FAILURE;                            \
}

/*Global Varibles*/
CS_CNXT_SMC_Config gSMCConfig={FALSE,};

static CNXT_THREAD_ID gSMCThreadID=0;
static CNXT_QUEUE_ID  gSMCQueueID =0;
static unsigned int   g_uSmcCntTime = 0;
static DWORD          g_dwTimerStatus = TIMER_NOTCREAT;
static CNXT_TICK_ID   g_hTimer = 0;
static int   g_cnxt_catype = 0;
/* Main*/
void smc_main_thread(int argc, void **argv)
{
	T_CSSmcMsg    stMsg;
	int32         nCardIndex=0;
   
    //cnxt_kal_trace( TL_FUNC, "SMC: main thread\n" );
   
   /* Keep running until we get a message telling us to terminate */
   while(TRUE)
   {
   	  if(CNXT_STATUS_OK == cnxt_kal_qu_receive(gSMCQueueID,CNXT_KAL_WAIT_FOREVER,&stMsg))
   	  {
	  	nCardIndex = stMsg.m_dw2nd;
	  	switch(stMsg.m_dw1st)
	  	{
			  case CNXT_SMC_EVENT_CARD_INSERTED:
				gSMCConfig.hSMCHandle[nCardIndex].fnCallBack(nCardIndex,CSUDISMC_ERROR_IN);
				break;
			case CNXT_SMC_EVENT_CARD_REMOVED:
				gSMCConfig.hSMCHandle[nCardIndex].fnCallBack(nCardIndex,CSUDISMC_ERROR_OUT);
				break;
	  	    default:
				//printf("internal_smct_notify_func:this event is NOT handled!ret=%d\n",stMsg.m_dw1st);
				break;		
	  	}
   	  }
	  else
	  {
	  	//cnxt_kal_trace( TL_FUNC, "SMC: receive msg fail!\n" );
	  }
   }
   
   //cnxt_kal_trace( TL_FUNC, "SMC: end main thread\n" );   
   cnxt_kal_thread_terminate();
   
}  /* smartcard_main_thread() */

static CNXT_STATUS DoSmcNormalInsert(CNXT_QUEUE_ID queueID,CNXT_SMC_EVENT Event,u_int32 uCardIndex)
{
	if(queueID == 0)
		return CNXT_STATUS_BAD_PARAMETER;
	else
	{
		T_CSSmcMsg  stMsg;
		stMsg.m_dw1st = Event;
		stMsg.m_dw2nd = uCardIndex;
		return cnxt_kal_qu_send(queueID, &stMsg);
	}
}
static void SMCLTimerFun(CNXT_TICK_ID hTickID, void *p)
{
	T_CSSmcMsg  *pstMsg;

	if(!p)
	{
		printf("bad param!\n");
		return;
	}
	
	pstMsg = (T_CSSmcMsg *)p;
	
	if (g_hTimer && CNXT_STATUS_OK ==DoSmcNormalInsert(gSMCQueueID,pstMsg->m_dw1st,pstMsg->m_dw2nd))
	{	
		g_dwTimerStatus = TIMER_STOP;
	}
}

static CNXT_STATUS DoSmcQuickInsert(CNXT_QUEUE_ID queueID,CNXT_SMC_EVENT Event,u_int32 uCardIndex)
{
	CNXT_STATUS KALStatus = CNXT_STATUS_OK;
	static T_CSSmcMsg  stMsg;

	stMsg.m_dw1st = Event;
	stMsg.m_dw2nd = uCardIndex;
	switch(g_dwTimerStatus)
	{
		case TIMER_NOTCREAT:
		{	
			if(CNXT_STATUS_OK == cnxt_kal_tick_create(SMCLTimerFun, &stMsg,"SmcQuickIn",&g_hTimer))
			{
				g_dwTimerStatus =TIMER_CREATED ;

				KALStatus = cnxt_kal_tick_set(g_hTimer,1500,TRUE);
				KALStatus = cnxt_kal_tick_start(g_hTimer);
				if(CNXT_STATUS_OK == KALStatus)
				{
					g_dwTimerStatus =TIMER_RUNING;
				}
				else
				{
					CSASSERT(FALSE);
				}
			}
			else
			{
				CSASSERT(FALSE);
			}		
		}
		break;
	
		case TIMER_RUNING:
		{
			KALStatus = cnxt_kal_tick_set(g_hTimer,1500,TRUE);
		}
		break;

		case TIMER_CREATED:
		case TIMER_STOP:
		{
			KALStatus = cnxt_kal_tick_set(g_hTimer,1500,TRUE);
			KALStatus = cnxt_kal_tick_start(g_hTimer);
			g_dwTimerStatus = TIMER_RUNING;
		}		
		break;

		default:
		{
			KALStatus = cnxt_kal_tick_stop(g_hTimer);
			g_dwTimerStatus = TIMER_STOP;
		}break;
	}
	return KALStatus;	
}
/*
static CNXT_STATUS internal_smc_Insert(CNXT_QUEUE_ID queueID,CNXT_SMC_EVENT Event,u_int32 uCardIndex)
{	
	CNXT_STATUS KALStatus;
	unsigned int uCurTime = 0;
	
	uCurTime = CSUDIPlusOSGetTickCount();
	
	if(uCurTime > g_uSmcCntTime && uCurTime < g_uSmcCntTime+3000)
	{
		KALStatus = DoSmcQuickInsert(queueID, Event,uCardIndex);
	}
	else
	{
		KALStatus = DoSmcNormalInsert(queueID,Event,uCardIndex);
	}

	g_uSmcCntTime = uCurTime;	
	
	return KALStatus;
}
*/

/*Function declaration*/
static void internal_smct_notify_func(CNXT_SMC_HANDLE Handle,
                                void *pUserData,
                                CNXT_SMC_EVENT Event,
                                void *pData,
                                void *Tag)
{
    /* Save the event and post the appropriate notification semaphore */
	u_int32       nCardIndex=0;	
	T_CSSmcMsg  stMsg;
	CNXT_STATUS KALStatus;
	
	if(gSMCConfig.count!=0)
	{		
		stMsg.m_dw1st = Event;
		stMsg.m_dw2nd = nCardIndex;
		
		switch(Event)
		{
			case CNXT_SMC_EVENT_CARD_INSERTED:
				//KALStatus = internal_smc_Insert(gSMCQueueID,Event,nCardIndex);
				KALStatus = cnxt_kal_qu_send(gSMCQueueID, &stMsg);
				break;

			case CNXT_SMC_EVENT_CARD_REMOVED:
				KALStatus = cnxt_kal_qu_send(gSMCQueueID, &stMsg);
				break;
			default:
				KALStatus = cnxt_kal_qu_send(gSMCQueueID, &stMsg);
				break;			
		}
		
	
		if(CNXT_STATUS_OK != KALStatus)
			printf("card msg send fail! 0x%x\n",KALStatus);      
	}
	else
	{
	    printf("internal_smct_notify_func:there is no smc device opened!\n");
	}

    return;
}

/*
csudi_smc_atr_parse()
Description:
    To seperate ATR(including TS,T0,TA(i),TB(i),TC(i),TD(i) and TCK(if available)) 
    and Historical Byte (T1...TK) 
    from orignal ATR (TS, T0 TA(0)...TK,TCK)
Parameters:
Input:
    u_int8 *puOrgATR,       Original ATR pass from driver
    u_int32 uOrgATRLength,  Original ATR Length
Output:
    u_int8 *puATR,      ATR excluding Historical Bytes
    u_int32 *uATRLength,    output ATR Length
    u_int8 *puHistByte,         Historical Bytes
    u_int32 *uHisLength,        Historical Bytes Length

*/

int CSUDISMCInit (void)
{
    CNXT_STATUS         retCode;
    int i;
    
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==TRUE)
    {
        CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] initialized before.\n");
        return CNXT_STATUS_ALREADY_INIT;
    }
	retCode = cnxt_smc_term();
    retCode = cnxt_smc_init(NULL);
    if(retCode!=CNXT_STATUS_OK)
    {
        CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] error: cnxt_smc_init failed.\n");
        return CSUDI_FAILURE;
    }
    for(i=0;i<SCUDI_MAX_SMC_UNIT_NUMBER;i++)
    {
        gSMCConfig.hSMCHandle[i].smcHandle=0;
        /*to create lock semaphore now???*/
        gSMCConfig.hSMCHandle[i].protect_sem=0;
        gSMCConfig.hSMCHandle[i].deviceno=SCUDI_INVALID_DEV_NUM;
    }

    gSMCConfig.bInit=TRUE;
    gSMCConfig.count=0;

    retCode = cnxt_kal_qu_create(CNXT_SMC_MSG_QUEUE_SIZE,"smcqueue",&gSMCQueueID);
    if( retCode != CNXT_STATUS_OK )
    {
		CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] error: create queue failed.\n");
        return CSUDI_FAILURE;
    }
   
    retCode = cnxt_kal_thread_create((PFNTHREAD)smc_main_thread,
									   	0,
									   	NULL,
									   	CNXT_SMC_STACK_SIZE,
									   	CNXT_SMC_THREAD_PRIORITY,
									   	"smartcard",
									   	&gSMCThreadID);
    if( retCode != CNXT_STATUS_OK )
    {
		CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] error: create thread failed.\n");
        return CSUDI_FAILURE;
    }

	g_uSmcCntTime = CSUDIPlusOSGetTickCount();
   
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	return CSUDI_SUCCESS;
}

BOOL CSUDISMCTerm()
{
	int i = 0;
	int nTotal;
	CSUDI_Error_Code bRet = CSUDI_SUCCESS;
	
	if (CSUDI_SUCCESS != (bRet=CSUDISMCGetCount(&nTotal)))
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] %s fail in %d.\n", __FUNCTION__,__LINE__);
		return FALSE;
	}
	
	cnxt_kal_thread_cancel(gSMCThreadID);
	cnxt_kal_qu_destroy(gSMCQueueID);
	
	for(i=0;i<nTotal;i++)
	{
		bRet |= CSUDISMCClose(i);
	}

	return (bRet == CSUDI_SUCCESS);
}

CSUDI_Error_Code CSUDISMCGetCount(int * pnSMCCount)
{
	CSUDI_Error_Code retCode=CSUDI_SUCCESS;
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);	
	if(pnSMCCount == NULL)
	{
	   return CSUDISMC_ERROR_BAD_PARAMETER;
	}
    if(gSMCConfig.bInit==FALSE)
    {
        return CSUDISMC_ERROR_FAILURE;
    }
	*pnSMCCount = 1;
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);	
    return retCode;    
}


CSUDI_Error_Code CSUDISMCOpen(int nCardIndex, CSUDISMCCallback fnSMCCallback)
{
    CNXT_STATUS         retCode;
    CNXT_SMC_CAPS       smcCaps;
    CNXT_SMC_HANDLE     smcHandle;
	u_int8 vcclevel;
	CNXT_SMC_CONFIG_TIMEOUT smcconfigTimeout;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDISMC_ERROR_FAILURE;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0)||(fnSMCCallback == NULL))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid card index.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);

    /*TODO:: allow to open a slot mutiple times??*/
    if(gSMCConfig.hSMCHandle[nCardIndex].smcHandle != NULL)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] %s error: card alreay opened.\n",__FUNCTION__);
        return CSUDI_SUCCESS;
    }
	

    smcCaps.uUnit=nCardIndex;    
    retCode = cnxt_smc_open(&smcHandle, &smcCaps, 
                              (CNXT_SMC_PFNNOTIFY)&internal_smct_notify_func, (void *)nCardIndex);
    if(retCode!=CNXT_STATUS_OK)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: cnxt_smc_open failed.\n");
        return CSUDISMC_ERROR_FAILURE;
    }
    
    if(gSMCConfig.hSMCHandle[nCardIndex].protect_sem==0)
    {
        CHAR str_sem_name[20];
        
        sprintf(str_sem_name,"UDISMCSem%d",nCardIndex);
        retCode=cnxt_kal_sem_create(1, str_sem_name, &(gSMCConfig.hSMCHandle[nCardIndex].protect_sem));
        if(CNXT_STATUS_OK!=retCode)
        {
            return CSUDISMC_ERROR_FAILURE;
        }
    }
    else
    {
        /*something wrong???*/
    }
	
    //config this smart card port here.............[alfred]
	retCode = cnxt_smc_get_vcclevel(smcHandle,&vcclevel);
	 if(retCode!=CNXT_STATUS_OK)
	 {
		retCode = cnxt_smc_close(smcHandle);
		retCode = cnxt_smc_term();
		return CNXT_STATUS_FAILURE; 
	}

	vcclevel=CNXT_SMC_VCC_5V;
	retCode = cnxt_smc_set_vcclevel(smcHandle,vcclevel);
	if(retCode!=CNXT_STATUS_OK)
	{
		retCode = cnxt_smc_close(smcHandle);
		retCode = cnxt_smc_term();
		return CNXT_STATUS_FAILURE; 
	}
	   
	retCode = cnxt_smc_get_config_timeout(smcHandle,&smcconfigTimeout);	
	if(retCode != CNXT_STATUS_OK)
	{	
	    printf("cnxt_smc_get_config_timeout failed retCode[%d]\n",retCode);
		return CSUDISMC_ERROR_FAILURE;
	}

	if(retCode != CNXT_STATUS_OK)
	{	
	    printf("cnxt_smc_set_config_timeout failed retCode[%d]\n",retCode);
		return CSUDISMC_ERROR_FAILURE;
	}
	
    gSMCConfig.hSMCHandle[nCardIndex].fnCallBack=fnSMCCallback;
    gSMCConfig.hSMCHandle[nCardIndex].smcHandle=smcHandle;
    gSMCConfig.hSMCHandle[nCardIndex].deviceno=nCardIndex;

    gSMCConfig.count++;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
    return CSUDI_SUCCESS;	
}

CSUDI_Error_Code CSUDISMCClose(int nCardIndex)
{
    CNXT_STATUS         retCode;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDISMC_ERROR_FAILURE;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid card index.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER;
    }

    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);

    if(CNXT_STATUS_OK!=cnxt_kal_sem_delete(gSMCConfig.hSMCHandle[nCardIndex].protect_sem))
    {
        return CSUDISMC_ERROR_FAILURE;
    }
    gSMCConfig.hSMCHandle[nCardIndex].protect_sem=0;

    retCode = cnxt_smc_close(gSMCConfig.hSMCHandle[nCardIndex].smcHandle);
    if(retCode!=CNXT_STATUS_OK)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: cnxt_smc_close failed.\n");
        return CSUDISMC_ERROR_FAILURE;
    }
    gSMCConfig.hSMCHandle[nCardIndex].smcHandle=0;
    gSMCConfig.hSMCHandle[nCardIndex].deviceno=SCUDI_INVALID_DEV_NUM;

    gSMCConfig.count--;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);

    return CSUDI_SUCCESS;    
	
}

CSUDI_Error_Code CSUDISMCReset(int nCardIndex, unsigned char * pucAtr, int * pnAtrLength, CSUDI_BOOL bColdRst)
{
    CNXT_STATUS         retCode;
    UINT                uClkFreq;
    CS_CNXT_SMC_SubSystem   *pSMCHandle=NULL;
    CNXT_SMC_STATE    State;
	
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDISMC_ERROR_FAILURE;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0) || (pucAtr == NULL) || (pnAtrLength == NULL))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);

    pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];
    *pnAtrLength=0;
    
    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
	retCode = cnxt_kal_sem_get(gSMCConfig.hSMCHandle[nCardIndex].protect_sem, CNXT_KAL_WAIT_FOREVER);
	if(retCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", retCode);
		printf("failed @%s %d.ret=%d\n",__FILE__,__LINE__,retCode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
		   
    //set 1MHz for reset, follow iso7816, add by lihongqiang 2011-1-26
	uClkFreq = SMC_DEFAULT_CLK_FREQ;

	/* set clock frequency */
   retCode = cnxt_smc_set_clock_freq(pSMCHandle->smcHandle, uClkFreq);

    retCode = cnxt_smc_get_state(pSMCHandle->smcHandle, &State);
	if(retCode!=CNXT_STATUS_OK)
	{
		printf("failed @%s %d.ret=%d\n",__FILE__,__LINE__,retCode);
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		printf("failed @%s %d.ret=%d\n",__FILE__,__LINE__,retCode);
		return CSUDISMC_ERROR_FAILURE;
	}

    if(State==CNXT_SMC_EMPTY)
    {
        retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_FAILURE;
    }
	
		if(bColdRst)
		{
	         retCode = cnxt_smc_powerdone_card(pSMCHandle->smcHandle, SMC_POWERDOWN_SEM_TIMEOUT, FALSE, NULL);
			
		     cnxt_kal_thread_time_sleep(SMC_POWERDOWN_TIMEOUT);
             retCode = cnxt_smc_reset_card(pSMCHandle->smcHandle, SMC_RESET_TIMEOUT, FALSE, NULL);
		}
		else
		{
			retCode = cnxt_smc_warm_reset_card(pSMCHandle->smcHandle,SMC_RESET_TIMEOUT,FALSE,NULL);
		}

		if(retCode!=CNXT_STATUS_OK)
		{
			printf("failed @%s %d.ret=%d\n",__FILE__,__LINE__,retCode);
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "error:failed @%s %d retCode[%d]\n",__FILE__,__LINE__,retCode); 
			retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
			return CSUDISMC_ERROR_FAILURE;
		}

//modify by lihongqiang removed 2010.07.19{

    retCode = cnxt_smc_set_clock_freq(pSMCHandle->smcHandle,gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_nFrequency);	

	if(retCode != CNXT_STATUS_OK)
	{
		return CSUDISMC_ERROR_FAILURE;
	}

//add by lihongqiang removed 2010.07.19}

    *pnAtrLength = SMCT_MAX_ATR_LENGTH;
    /* read ATR */ 
    retCode = cnxt_smc_get_atr(pSMCHandle->smcHandle, (u_int8*)pucAtr, (u_int32*)pnAtrLength);
		
	if(retCode!=CNXT_STATUS_OK)
	{
		printf("failed @%s %d.ret=%d\n",__FILE__,__LINE__,retCode);
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
    return CSUDI_SUCCESS;
	
	
}


CSUDI_Error_Code CSUDISMCSetParams(int nCardIndex,const CSUDISMCParam_S * psSMCParams)
{
	
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);
	if(gSMCConfig.bInit==FALSE)
	 {
		 CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
		 return CSUDISMC_ERROR_FAILURE;
	 }
	 if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER) || (psSMCParams == NULL)||(nCardIndex<0) )
	 {
		 CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
		 return CSUDISMC_ERROR_BAD_PARAMETER;
	 }
	 if((psSMCParams->m_eCheckBit>= EM_UDISMC_PARITY_MAX)||(psSMCParams->m_eProtocol>=EM_UDISMC_PROTOCOL_MAX)
	 	||(psSMCParams->m_eStandard>= EM_UDISMC_STANDARD_MAX)||(psSMCParams->m_eStopBit >= EM_UDISMC_PARITY_MAX)
	 	||(psSMCParams->m_nBaudRate<0)||(psSMCParams->m_nETU<0)||(psSMCParams->m_nFrequency<0))
	 {
	     return CSUDISMC_ERROR_BAD_PARAMETER;
	 }
	 
	 //SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
     gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_eCheckBit = psSMCParams->m_eCheckBit;
     gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_eProtocol = psSMCParams->m_eProtocol;
     gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_eStandard = psSMCParams->m_eStandard;
     gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_eStopBit = psSMCParams->m_eStopBit;
     gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_nBaudRate = psSMCParams->m_nBaudRate;
     gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_nETU = psSMCParams->m_nETU;
     gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_nFrequency = (psSMCParams->m_nFrequency/10000)*10000;
	 
	 CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	 return CSUDI_SUCCESS;

}


CSUDI_Error_Code CSUDISMCGetParams(int nCardIndex,CSUDISMCParam_S * psSMCParams)
{
    CS_CNXT_SMC_SubSystem   *pSMCHandle=NULL;
	
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);
	if(gSMCConfig.bInit==FALSE)
	 {
		 CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
		 return CSUDISMC_ERROR_FAILURE;
	 }
	 if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER) || (psSMCParams == NULL)||(nCardIndex<0) )
	 {
		 CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
		 return CSUDISMC_ERROR_BAD_PARAMETER;
	 }
	 //SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
	 pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];
     psSMCParams->m_eCheckBit = pSMCHandle->smcParam_s.m_eCheckBit;
     psSMCParams->m_eProtocol = pSMCHandle->smcParam_s.m_eProtocol;
     psSMCParams->m_eStandard = pSMCHandle->smcParam_s.m_eStandard;
     psSMCParams->m_eStopBit = pSMCHandle->smcParam_s.m_eStopBit;
     psSMCParams->m_nBaudRate = pSMCHandle->smcParam_s.m_nBaudRate;
     psSMCParams->m_nETU = pSMCHandle->smcParam_s.m_nETU;
     psSMCParams->m_nFrequency = pSMCHandle->smcParam_s.m_nFrequency;
	 CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	 
	 return CSUDI_SUCCESS;
}

CSUDI_Error_Code cnxt_DataFixForSuantong(unsigned char * pucData, int nDataLen, int nMaxLen)
{
	unsigned char chLRC;
	int nTemp;
	if(!pucData || nDataLen == 0 || nMaxLen == 0)
	{
		return CSUDI_FAILURE;
	}

	if(nDataLen + 4 >= nMaxLen)
	{
		return CSUDI_FAILURE;
	}
	
	for(nTemp = nDataLen-1; nTemp >= 0; nTemp--)
	{
		pucData[nTemp+3]= pucData[nTemp];
	}	

	pucData[0]=0;
	pucData[1]=0;
	pucData[2]=nDataLen;
	chLRC=0;

	for (nTemp = 0; nTemp <  3+(INT)nDataLen; nTemp++)
	{
		chLRC^=pucData[nTemp];
	}

	pucData[nTemp]=chLRC;
	
	return CSUDI_SUCCESS;
}

CSUDI_Error_Code CSUDISMCDataExchange(
									int             nCardIndex, 
									unsigned char  *pucWriteData,
	                                int             nNumberToWrite,
	                                unsigned char  *pcResponseData,
	                                int            *pnNumberRead,
	                                unsigned char  *pucStatusWord)
{
    CNXT_STATUS           retCode;
    CS_CNXT_SMC_SubSystem *pSMCHandle=NULL;
    CNXT_SMC_STATE        State;
	int                   operationType = SMC_READ_OPERATION;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDI_SUCCESS;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0) )
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid index.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER ;
    }

    if((pucWriteData==NULL) || nNumberToWrite<=0 || (pucStatusWord==NULL)
		||(pcResponseData==NULL)||(pnNumberRead==NULL))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER ;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);

	
    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
	retCode = cnxt_kal_sem_get(gSMCConfig.hSMCHandle[nCardIndex].protect_sem, CNXT_KAL_WAIT_FOREVER);
	if(retCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", retCode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}

    pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];
    
    retCode = cnxt_smc_get_state(pSMCHandle->smcHandle, &State);

	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}

    if(State==CNXT_SMC_EMPTY)
    {    
    	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_OUT;
    }
    if(State==CNXT_SMC_NOT_READY)
    {
    
	    retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_NOT_RESET;
	}
	switch(gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_eProtocol)
	{
	   case EM_UDISMC_PROTOCOL_T0:	
		   retCode = cnxt_smc_set_protocol (pSMCHandle->smcHandle,
							  CNXT_SMC_PROTOCOL_T0);
		   break;
	   case EM_UDISMC_PROTOCOL_T1:
			  retCode = cnxt_smc_set_protocol (pSMCHandle->smcHandle,
								 CNXT_SMC_PROTOCOL_T1);
		   break;
	   case EM_UDISMC_PROTOCOL_T14:
	   	 retCode = cnxt_smc_set_protocol (pSMCHandle->smcHandle,
								 CNXT_SMC_PROTOCOL_T14);
		   break;
		   
	   case EM_UDISMC_PROTOCOL_MAX:
	   case EM_UDISMC_PROTOCOL_UNKNOWN:
	   default:
		
		   break;
	}
	if(retCode != CNXT_STATUS_OK)
	{
	    printf("set protocol is NOT success  retCode[%d]\n",retCode);
		return CSUDISMC_ERROR_FAILURE;
	}

	if (pSMCHandle->smcParam_s.m_eProtocol == EM_UDISMC_PROTOCOL_T1)
	{	
 	    *pnNumberRead = SMC_T1_MAX_READLEN;//ISO7816 T1
 	    
        retCode = cnxt_smc_read_write (pSMCHandle->smcHandle,
                                   FALSE,
                                   pucWriteData,
                                   nNumberToWrite,
                                   pcResponseData,
                                   (u_int32 *)pnNumberRead,
                                   SMC_RW_DEFAULT_TIMEOUT,
                                   NULL );
		
	}
	else if(pSMCHandle->smcParam_s.m_eProtocol == EM_UDISMC_PROTOCOL_T0)
	{
		unsigned char uCommand[5];
		
		if(*pnNumberRead>512)
			*pnNumberRead=512;

        if(nNumberToWrite >= SMC_CMD_HEADER_LEN)
        {
			if((*(pucWriteData+SMC_CMD_T0_READLEN)+SMC_CMD_HEADER_LEN)==nNumberToWrite)
			{
			    //it is write operation
			    *pnNumberRead = 2;
				operationType = SMC_WRITE_OPERATION;
			}
			else
			{
			    //it is read operatoin
			    *pnNumberRead = *(pucWriteData+SMC_CMD_T0_READLEN)+2;//modify by 904379lhq
				operationType = SMC_READ_OPERATION;
			}
			
			retCode = cnxt_smc_read_write (pSMCHandle->smcHandle,
                   FALSE,
                   pucWriteData,
                   nNumberToWrite,
                   pcResponseData,
                   (u_int32 *)pnNumberRead,
                   SMC_RW_DEFAULT_TIMEOUT,
                   NULL );

        }	
		else //if(nNumberToWrite < SMC_CMD_HEADER_LEN)
		{		
			int i;

			//memcpy(uCommand,pucWriteData,nNumberToWrite);
			for(i=0;i<SMC_CMD_HEADER_LEN; i++)
			{
                if(i<nNumberToWrite)
                {
                    uCommand[i]=pucWriteData[i];
                }
                else
                {
                    uCommand[i]=0x00;
                }
			}

			nNumberToWrite=SMC_CMD_HEADER_LEN;
	
		  
			retCode = cnxt_smc_read_write (pSMCHandle->smcHandle,
                               FALSE,
                               uCommand,
                               nNumberToWrite,
                               pcResponseData,
                               (u_int32 *)pnNumberRead,
                               SMC_RW_DEFAULT_TIMEOUT,
                               NULL );
        }                             
	}
	else if(pSMCHandle->smcParam_s.m_eProtocol == EM_UDISMC_PROTOCOL_T14)
	{
        if(*pnNumberRead < SMC_T14_LEAST_READLEN)
        {
            *pnNumberRead = SMC_T14_LEAST_READLEN;
        }
        else if( *pnNumberRead > SMC_T14_MAX_READLEN)
        {
            *pnNumberRead = SMC_T14_MAX_READLEN;
        }
        
		retCode = cnxt_smc_read_write (pSMCHandle->smcHandle,
                                   FALSE,
                                   pucWriteData,
                                   nNumberToWrite,
                                   pcResponseData,
                                   (u_int32 *)pnNumberRead,
                                   SMC_RW_DEFAULT_TIMEOUT,
                                   NULL );

	}
	else
	{
        retCode = cnxt_smc_read_write (pSMCHandle->smcHandle,
                                   FALSE,
                                   pucWriteData,
                                   nNumberToWrite,
                                   pcResponseData,
                                   (u_int32 *)pnNumberRead,
                                   SMC_RW_DEFAULT_TIMEOUT,
                                   NULL );
	}
	
    if(retCode != CNXT_STATUS_OK)
    {
       	printf("->udi smc read write failed,%d\n",retCode);
     	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_TIMEOUT;
    }
	
    if(pSMCHandle->smcParam_s.m_eProtocol == EM_UDISMC_PROTOCOL_T0)
    {
        if((*pnNumberRead)>=2)
        {
          *pucStatusWord = *(pcResponseData+(*pnNumberRead)-2);
    	  *(pucStatusWord+1) = *(pcResponseData+(*pnNumberRead)-1);
       	  *pnNumberRead -= 2;
    	}
    }
    else //if(pSMCHandle->smcParam_s.m_eProtocol == EM_UDISMC_PROTOCOL_T1)
    {
        //did NOT need to fill up the pucStatusWord
        *pucStatusWord = 0;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	
	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
    return CSUDI_SUCCESS;

}

CSUDI_Error_Code CSUDISMCSend(
					 int             nCardIndex,
				     unsigned char  *pucWriteBuf,
				     int             nNumberToWrite,
				     int            *pnNumberWrite,
				     int             nTimeout)
{
    CNXT_STATUS         retCode;
    CS_CNXT_SMC_SubSystem   *pSMCHandle=NULL;
    CNXT_SMC_STATE    State;
	unsigned char Tempdatabuffer[100];
    u_int32 templength;	
	u_int32 startuTime = 0;
	u_int32 enduTime = 0;
	templength = 0;
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDI_SUCCESS;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid index.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER ;
    }

    if((pucWriteBuf==NULL) || (pnNumberWrite==NULL)||(nNumberToWrite<=0)||(nTimeout<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER ;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.nTimeout[%dms]\n", nCardIndex,nTimeout);
    
    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
	retCode = cnxt_kal_sem_get(gSMCConfig.hSMCHandle[nCardIndex].protect_sem, CNXT_KAL_WAIT_FOREVER);
	if(retCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", retCode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}

    pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];

    retCode = cnxt_smc_get_state(pSMCHandle->smcHandle, &State);		
		
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}

    if(State==CNXT_SMC_EMPTY)
    {
    
	    retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_FAILURE;
    }
    if(State==CNXT_SMC_NOT_READY)
    {
        
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_FAILURE;
    }
	cnxt_kal_get_system_time(&startuTime);

    retCode = cnxt_smc_read_write (pSMCHandle->smcHandle,
                                   FALSE,
                                   pucWriteBuf,
                                   nNumberToWrite,
                                   Tempdatabuffer,
                                   &templength,
                                   nTimeout,
                                   NULL );
			
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}
	cnxt_kal_get_system_time(&enduTime);
	if((enduTime-startuTime)>nTimeout)
	{
	   
	   retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
	    return CSUDISMC_ERROR_FAILURE;
	}
    if(retCode==CNXT_STATUS_INTERNAL_ERROR)
    {
        *pnNumberWrite=0;		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_TIMEOUT;
    }
    
    *pnNumberWrite=nNumberToWrite;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	
	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
    return CSUDI_SUCCESS;

}

CSUDI_Error_Code CSUDISMCReceive(
						int             nCardIndex,
				        unsigned char  *pucReadBuf,
				        int             nNumberToRead,
				        int            *pnNumberRead,
				        int             nTimeout)
{
    CNXT_STATUS         retCode;
    CS_CNXT_SMC_SubSystem   *pSMCHandle=NULL;
    CNXT_SMC_STATE    State;
	unsigned char Tempdatabuffer[10];
    u_int32 templength;
	templength = 0;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDI_SUCCESS;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid index.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER ;
    }

    if((pucReadBuf==NULL) || (nNumberToRead<=0) || (pnNumberRead==NULL)||(nTimeout<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER ;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);
    
    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
	retCode = cnxt_kal_sem_get(gSMCConfig.hSMCHandle[nCardIndex].protect_sem, CNXT_KAL_WAIT_FOREVER);
	if(retCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", retCode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}

    pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];

    retCode = cnxt_smc_get_state(pSMCHandle->smcHandle, &State);
			
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}
    if(State==CNXT_SMC_EMPTY)
    {
    
	    retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_OUT;
    }
    if(State==CNXT_SMC_NOT_READY)
    {
        
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_CARD_BUSY;
    }

    cnxt_kal_memset(pucReadBuf, 0, nNumberToRead);
    
    retCode = cnxt_smc_read_write (pSMCHandle->smcHandle,
                                   FALSE,
                                   Tempdatabuffer,
                                   templength,
                                   pucReadBuf,
                                   (u_int32 *)&nNumberToRead,
                                   nTimeout,
                                   NULL );
    
    if(retCode==CNXT_STATUS_INTERNAL_ERROR)
    {
        *pnNumberRead = 0;
		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_TIMEOUT;
    }
        
    *pnNumberRead = nNumberToRead;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	
	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
    return CSUDI_SUCCESS;
	

}

CSUDI_Error_Code CSUDISMCSetPPS(
						 int             nCardIndex,
	                    unsigned char  *pucWriteData,
                           int             nNumberToWriteLen,
	                    unsigned char  *pucResponseData,
                           int            *pnResponseDataLen)
{
    CNXT_STATUS         retCode;
    CS_CNXT_SMC_SubSystem   *pSMCHandle=NULL;
    CNXT_SMC_STATE   State;

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);
    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDI_FAILURE;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER;
    }
	
    if((pucWriteData==NULL) || (nNumberToWriteLen<=0) 
		||(pucResponseData==NULL)||(pnResponseDataLen==NULL))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER ;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);
    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
		
	retCode = cnxt_kal_sem_get(gSMCConfig.hSMCHandle[nCardIndex].protect_sem, CNXT_KAL_WAIT_FOREVER);
	if(retCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", retCode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	
    pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];

	retCode = cnxt_smc_get_state(pSMCHandle->smcHandle, &State);		
		
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}
    if(State==CNXT_SMC_EMPTY)
    {
    
	    retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_FAILURE;
    }

	
#if (SMC_PPS_SUPPORTED == YES)  //must be yes,otherwise atr parse error. comment by lihongqiang. 2011-1-26
    retCode = cnxt_smc_start_pps(pSMCHandle->smcHandle, (u_int8*)pucWriteData,pucResponseData,(u_int8*)pnResponseDataLen,TRUE);
#endif

	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);			
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}
		
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
    return CSUDI_SUCCESS;


}

CSUDI_Error_Code CSUDISMCDetect(int nCardIndex)
{
	
    CNXT_STATUS         retCode;
    CS_CNXT_SMC_SubSystem   *pSMCHandle=NULL;
    CNXT_SMC_STATE   State;


    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDI_FAILURE;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);
    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
	retCode = cnxt_kal_sem_get(gSMCConfig.hSMCHandle[nCardIndex].protect_sem, CNXT_KAL_WAIT_FOREVER);
	if(retCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", retCode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}

    pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];

    retCode = cnxt_smc_get_state(pSMCHandle->smcHandle, &State);
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s cnxt_smc_get_state retCode[%d]State[%d]\n",__FUNCTION__,retCode,State);
		
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}
	
    if(State==CNXT_SMC_EMPTY)
    {
    
	    retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_OUT;
    }
	else if(State == CNXT_SMC_NOT_READY)
	{
	    
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
	    return CSUDISMC_ERROR_IN;
	}
	else if(State == CNXT_SMC_READY)
	{   
	    
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
	    return CSUDISMC_ERROR_READY;
	}

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	
	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
    return CSUDI_SUCCESS;

}
CSUDI_Error_Code CSUDISMCActive(int nCardIndex)
{
    CNXT_STATUS         retCode;
    CS_CNXT_SMC_SubSystem   *pSMCHandle=NULL;
    CNXT_SMC_STATE   State;


    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDI_FAILURE;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);
    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
	retCode = cnxt_kal_sem_get(gSMCConfig.hSMCHandle[nCardIndex].protect_sem, CNXT_KAL_WAIT_FOREVER);
	if(retCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", retCode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}

    pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];

    retCode = cnxt_smc_get_state(pSMCHandle->smcHandle, &State);
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);		
		return CSUDISMC_ERROR_FAILURE;
	}

    if(State==CNXT_SMC_EMPTY)
    {
    
	    retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
        return CSUDISMC_ERROR_OUT;
    }

	retCode = cnxt_smc_set_clock_freq(pSMCHandle->smcHandle, SMC_DEFAULT_CLK_FREQ);
	
    retCode = cnxt_smc_reset_card(pSMCHandle->smcHandle, SMC_RESET_TIMEOUT, FALSE, NULL);
		
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}
    retCode = cnxt_smc_set_clock_freq(pSMCHandle->smcHandle,
		gSMCConfig.hSMCHandle[nCardIndex].smcParam_s.m_nFrequency);
	if(retCode != CNXT_STATUS_OK)
	{
		return CSUDISMC_ERROR_FAILURE;
	}
	
//	cnxt_smc_set_vcclevel(pSMCHandle->smcHandle,CNXT_SMC_VCC_3V);
	
	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);

    return CSUDI_SUCCESS;    

}

CSUDI_Error_Code CSUDISMCDeactive(int nCardIndex)
{
    CNXT_STATUS         retCode;
    CS_CNXT_SMC_SubSystem   *pSMCHandle=NULL;
    CNXT_SMC_STATE   State;


    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s begin.\n", __FUNCTION__);

    if(gSMCConfig.bInit==FALSE)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: smc not initialized yet.\n");
        return CSUDI_FAILURE;
    }
    if((nCardIndex>=SCUDI_MAX_SMC_UNIT_NUMBER)||(nCardIndex<0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_SMC] error: invalid input.\n");
        return CSUDISMC_ERROR_BAD_PARAMETER;
    }

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] dCardIndex %d.\n", nCardIndex);
    SCUDI_SMC_CHECK_CARD_OPEN(gSMCConfig.hSMCHandle[nCardIndex].smcHandle)
	retCode = cnxt_kal_sem_get(gSMCConfig.hSMCHandle[nCardIndex].protect_sem, CNXT_KAL_WAIT_FOREVER);
	if(retCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", retCode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}

    pSMCHandle=&gSMCConfig.hSMCHandle[nCardIndex];

    retCode = cnxt_smc_get_state(pSMCHandle->smcHandle, &State);
		
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);		
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}
    if(State==CNXT_SMC_EMPTY)
    {
        return CSUDISMC_ERROR_OUT;
    }
	
    retCode = cnxt_smc_powerdone_card(pSMCHandle->smcHandle, SMC_POWERDOWN_TIMEOUT, FALSE, NULL);
		
	if(retCode!=CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "failed @%s %d.\n",__FILE__,__LINE__);
		retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
		return CSUDISMC_ERROR_FAILURE;
	}

    CSDEBUG(MODULE_NAME, SMC_LEVEL, "[CS_SMC] %s end.\n", __FUNCTION__);
	retCode = cnxt_kal_sem_put(gSMCConfig.hSMCHandle[nCardIndex].protect_sem);
    return CSUDI_SUCCESS;    
}

void CSUDISMCSetCardReaderResetDir(BOOL bIsReverseDir)
{
	return;
}

CSUDI_Error_Code CSUDISMCSetCaType(int nCardIndex, int eType)
{
	if(eType <= 0 && eType > 10)//SMC_IRDETO_CA
		return CSUDI_FAILURE;

	g_cnxt_catype = eType;
	return SUCCESS;
}
int CSUDISMCGetCaType(int nCardIndex)
{
	return g_cnxt_catype;
}



