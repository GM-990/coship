/****************************************************************************/
/*                   CONEXANT PROPRIETARY AND CONFIDENTIAL                  */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Conexant Systems India Pvt Ltd. 2004-2008                */
/*                                Hyderabad, India                                */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        NXP_cs_clocktimer.c
 *
 *
 * Description:     API implementation for COSHIP interface layer,PCM inject .
 *
 *
 * Author:          Santhosh Reddy K
 *
 ****************************************************************************/
/* $Id: demux.c 95092 2009-08-14 21:16:57Z whiteth $
 ****************************************************************************/

/*****************/
/* Include Files */
/*****************/
#include <stdio.h>

#include "stbcfg.h"
#include "kal.h"

#include "generic_include.h"
#include "udi2_audio.h"
#include "udi2_aout.h"
#include "udi2_inject.h"

#define IS_VALID_PCM_INDEX(nAudIndx)  \
	if(((nAudIndx)<= 0)||((nAudIndx)> PCM_INJECT_INDEX)) \
	{  \
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"input index:%d PCM must use index %d\n", \
			nAudIndx,PCM_INJECT_INDEX); \
			return FALSE; \
	}

#define  MODULE_NAME   "CS_AUDIO"
#define AV_LEVEL  ERROR_LEVEL

#define TMP_BUFF_FIX_SIZE   (64*1024)//PCM_BUF_SIZE//(94*1024) // ((PCM_BUF_SIZE))//

#define INVALID_PCM_STREAM_HANDLE   0
#define ENCODED_PCM_BUFFER_SIZE 96
#define PCM_BUF_SIZE    (2 * ENCODED_PCM_BUFFER_SIZE * 1024) /*2*96*1024 Bye*/
#define ONE_TIME_SIZE   (64*1024)
typedef enum _CS_CNXT_PCMPLAYState
{
  	CSPCM_PLAY_STATE_UNKNOWN=0,
    CSPCM_PLAY_STATE_INIT,
  	CSPCM_PLAY_STATE_IDLE, /*never play or stopped*/
    CSPCM_PLAY_STATE_PLAYING,
    CSPCM_PLAY_STATE_STOP,
    CSPCM_PLAY_STATE_PAUSE
}CS_CNXT_PCMPLAYState;
typedef enum _CS_CNXT_PCMINJECTState
{
  	CSPCM_INJECT_STATE_UNKNOWN=0,
    CSPCM_INJECT_STATE_INIT,
  	CSPCM_INJECT_STATE_IDLE, /*never play or stopped*/
    CSPCM_INJECT_STATE_START
}CS_CNXT_PCMINJECTState;


/*global definitions*/
static volatile bool bFlushWaiting = FALSE;
static CNXT_SEM_ID gPCMWriteCompleteSemaphore=0; /*sync usage, to notify client PCM write complete*/
static CNXT_SEM_ID gPCMInjectSem=0;
static int pcmClipPlayState[4]={1,1,1,1};/*PCM clip play state*/
static int pcmClipInjectState[4]={0,0,0,0};/*PCM clip play state*/
static void *pTmpPcmClipBuffer=NULL; /*temp PCM clip buffer, to remember the buffer passed to application by calling GetFreeBuffer*/
static int16 *pTmp16bitPcmClipBuffer = NULL;
static UINT uSizeofTmpPcmClipBuffer=0;
static BOOL bTmpPcmClipBuffInUse=FALSE;
extern CS_CNXT_Inject_Config g_sCSInjectConfig;    
/*extern global definitions*/


/*get everything ready for PCM inject, due to Coship test case could call open/start Inject/Player in random sequence, damn it!!*/
bool CSPcmClip_Init(int nAudIndx)
{
    CNXT_STATUS Retcode = CNXT_STATUS_OK;
    BOOL bRetCode=FALSE;

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    if(nAudIndx<0||nAudIndx>=MAX_AUDIO_OBJECTS)
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] Invalid nAudIndx.\n");
        return bRetCode;
    }
    if(!gPCMWriteCompleteSemaphore)
    {
        Retcode = cnxt_kal_sem_create(0, "PCMWCompSem", &(gPCMWriteCompleteSemaphore));
		
	    if(CNXT_STATUS_OK!=Retcode)
        {
            return bRetCode;
        }
    }
    else
    {   /* get wrong, it should be opened before*/
        return bRetCode;
    }
    if(!gPCMInjectSem)
    {
        Retcode = cnxt_kal_sem_create(1, "PCMInjSem", &(gPCMInjectSem));
		
	    if(CNXT_STATUS_OK!=Retcode)
        {
            return bRetCode;
        }
    }
    else
    {   /* get wrong, it should be opened before*/
        return bRetCode;
    }
    bRetCode = TRUE;

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return bRetCode;
}
bool CSPcmInject_Open(CS_CNXT_Inject_SubSystem *pInjectHandle, int nAudIndx)
{

    CNXT_CBUF_CAPS    cbufCaps;
    CNXT_CBUF_HANDLE  cbufHandle;
    CNXT_STATUS Retcode = CNXT_STATUS_OK;
    bool eErrcode=TRUE;

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);
    IS_VALID_PCM_INDEX(nAudIndx);
   	
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        return FALSE;
    }    

    do{

        /* Open a PCM CBUF */
        cbufCaps.Type                   = CNXT_CBUF_TYPE_PCM_STREAM;
        cbufCaps.uSize                  =  PCM_BUF_SIZE/*+4*/;
        cbufCaps.pUserAllocatedMemory   = 0;
        cbufCaps.uHighWaterMark         = 0;
        cbufCaps.uLowWaterMark          = PCM_BUF_SIZE/4;
        cbufCaps.Behaviour              = CNXT_CBUF_NORMAL_BEHAVIOUR;
        
        {
          cbufCaps.uUserAlignment         = 32;
        }

        Retcode = cnxt_cbuf_open ( &cbufHandle, 
                                &cbufCaps, 
                                NULL, 
                                NULL );
        if(Retcode==CNXT_STATUS_OK)
        {
            pInjectHandle->hPCMClipcbuf=cbufHandle;
        }
        else
        {
            pInjectHandle->hPCMClipcbuf=NULL;
            eErrcode = FALSE;
            break;
        }
        pInjectHandle->pEnmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

       }while(0);
    /*it's ready to play PCM clip*/
    pcmClipInjectState[nAudIndx]  = CSPCM_INJECT_STATE_IDLE;

    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return eErrcode;
}
bool CSPcmInject_Close(CS_CNXT_Inject_SubSystem *pInjectHandle)
{
    BOOL bRetCode=TRUE;
    CNXT_STATUS retCode;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    PIPE_STATUS PmStatus = PIPE_STATUS_OK;
    int nAudIndx = pInjectHandle->nAudIndx;

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);

    if(pcmClipInjectState[nAudIndx]  != CSPCM_INJECT_STATE_IDLE)
    {
        /*only allow to close when idle*/
        return FALSE;
    }
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
    do{
        if((pcmClipPlayState[nAudIndx] == CSPCM_PLAY_STATE_PLAYING)||
            (pcmClipPlayState[nAudIndx] == CSPCM_PLAY_STATE_PAUSE))
        {            
            pAPMObject = gTmApDeviceObj.pAudioPresentation[0];/*We have only one presentation device :)*/
                    
            PmStatus = pAPMObject->mixer_remove(pAPMObject,pInjectHandle->uMixerId);
            if (PmStatus != PIPE_STATUS_OK)
            {
              CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
              retCode = FALSE;
              break;
            }
            pcmClipPlayState[nAudIndx] = CSPCM_PLAY_STATE_STOP;
        }
        retCode = cnxt_cbuf_close(pInjectHandle->hPCMClipcbuf);
        if(retCode != CNXT_STATUS_OK)
        {
           CSDEBUG(MODULE_NAME,ERROR_LEVEL,
                          "Error %d - cnxt_cbuf_close\n", retCode);
           bRetCode = FALSE;
           break;
        }

        if(pTmpPcmClipBuffer)
        {
            cnxt_kal_mem_free(pTmpPcmClipBuffer);
            pTmpPcmClipBuffer = NULL;
            uSizeofTmpPcmClipBuffer=0;
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,
                           "pTmpPcmClipBuffer free\n");
        }

        pcmClipInjectState[nAudIndx]  = CSPCM_INJECT_STATE_INIT;
    }while(0);
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        bRetCode= FALSE;
    }
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return bRetCode;
}

bool CSPcmPlaySetPCMParams(CS_CNXT_Inject_SubSystem *pInjectHandle, CSUDIINJECTERPcmStartParam_S* psPcmStart)
{
    BOOL retCode=TRUE;
    //PIPE_STATUS PmStatus = PIPE_STATUS_OK;
    //PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    //PIPE_AUDIO_PRESENTATION_MIXER_ATTRIB eMixerAttrb;
    int nAudIndx = pInjectHandle->nAudIndx;
    
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);
    
    IS_VALID_PCM_INDEX(nAudIndx);
   
    if(pcmClipPlayState[nAudIndx]  <= CSPCM_PLAY_STATE_UNKNOWN /*|| pcmClipPlayState  > CSPCM_PLAY_STATE_IDLE*/)
    {
        return FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] m_uBitsSample = %d, m_uChannels = %d, m_uSampleRate = %d, .\n",
        psPcmStart->m_uBitsSample,psPcmStart->m_uChannels,psPcmStart->m_uSampleRate);
    if((psPcmStart->m_uBitsSample!=8 && psPcmStart->m_uBitsSample!=16 && psPcmStart->m_uBitsSample!=32)
        ||(psPcmStart->m_uChannels!=1&&psPcmStart->m_uChannels!=2)
        /*lichanghua cancel:驱动中不止支持这些类型*/
        /*
        ||(psPcmStart->m_uSampleRate!=32000
            &&psPcmStart->m_uSampleRate!=44100
            &&psPcmStart->m_uSampleRate!=48000
            )*/
        )
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] Bad parameters.\n");
        return FALSE;
    }

    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    do{
        
       //pAPMObject = gTmApDeviceObj.pAudioPresentation[nAudIndx];
       pInjectHandle->psPcmStartParams.m_uBitsSample = psPcmStart->m_uBitsSample;
       pInjectHandle->psPcmStartParams.m_uChannels = psPcmStart->m_uChannels;
       pInjectHandle->psPcmStartParams.m_uSampleRate = psPcmStart->m_uSampleRate;
       if(psPcmStart->m_uSampleRate == 11000)
       {        
        pInjectHandle->psPcmStartParams.m_uSampleRate = 11025;
       }
       
       break;
#if 0       
       PmStatus = pAPMObject->get_mixer_attrib(pAPMObject, pInjectHandle->uMixerId, &eMixerAttrb);
       if(PmStatus != PIPE_STATUS_OK)
       {
         CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
         retCode = FALSE;
         break;
       }
       /* ...then stop the pcm clip stream */            
       PmStatus = pAPMObject->mixer_remove(pAPMObject, pInjectHandle->uMixerId);
       if (PmStatus != PIPE_STATUS_OK)
       {
          CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
          retCode = FALSE;
          break;
       }

       CSDEBUG(MODULE_NAME, ERROR_LEVEL,"%s %d Endian %d \n", __FUNCTION__, __LINE__, eMixerAttrb.MixerCfg.Endian);
       
       eMixerAttrb.MixerCfg.uSampleRate = psPcmStart->m_uSampleRate;
       eMixerAttrb.MixerCfg.uBitsPerSample = psPcmStart->m_uBitsSample;
       eMixerAttrb.MixerCfg.uNumChannels = psPcmStart->m_uChannels;
             
       /* ...then start the pcm clip stream */       
       PmStatus = pAPMObject->mixer_add(pAPMObject,&eMixerAttrb.MixerCfg,&pInjectHandle->uMixerId);
       if (PmStatus != PIPE_STATUS_OK)
       {
         CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
         retCode = FALSE;
         break;
       }
#endif       
    }while(0);
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        retCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}


bool CSPcmInject_Start(CS_CNXT_Inject_SubSystem *pInjectHandle)
{
        BOOL retCode = TRUE;
        CNXT_STATUS status = CNXT_STATUS_OK; 
        
        int nAudIndx = pInjectHandle->nAudIndx;
        
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

        IS_VALID_PCM_INDEX(nAudIndx);
      
        if(pcmClipInjectState[nAudIndx]  != CSPCM_INJECT_STATE_IDLE )
        {
            return FALSE;
        }
        if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
        {
            CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
            return FALSE;
        }
        do{
           status = cnxt_cbuf_flush(pInjectHandle->hPCMClipcbuf, 
                                        CNXT_CBUF_RESET_FLUSH);
           if(status!=CNXT_STATUS_OK)
           {
                retCode= FALSE;
                break;
           }
           pcmClipInjectState[nAudIndx]  = CSPCM_INJECT_STATE_START;
       }while(0);
       /*ready to inject data*/
       if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
       {
            CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
            retCode= FALSE;
       }

       CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

       return retCode;
}

bool CSPcmInject_Stop(CS_CNXT_Inject_SubSystem *pInjectHandle)
{
    BOOL retCode = TRUE;    
    int nAudIndx = pInjectHandle->nAudIndx;
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);
   
    //cnxt_kal_thread_time_sleep(100);
    if(pcmClipInjectState[nAudIndx] != CSPCM_INJECT_STATE_START)
    {
        return FALSE;
    }
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
    do{
        /*ready to inject data*/
        pcmClipInjectState[nAudIndx] = CSPCM_INJECT_STATE_IDLE;
    }while(0);
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        retCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}

bool CSPcmInject_GetFreeBuffer(CS_CNXT_Inject_SubSystem *pInjectHandle, void ** ppvBuffer, unsigned  * puLength )
{
    CNXT_STATUS retCode;
    UINT32  uFreeBytes;
    BOOL bStatus=TRUE;
    int nAudIndx = pInjectHandle->nAudIndx;
        
    //CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    //CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

	IS_VALID_PCM_INDEX(nAudIndx);

    if(!ppvBuffer || !puLength)
    {
        return FALSE;
    }
    
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    do
    {
	    if(pcmClipInjectState[nAudIndx]  == CSPCM_INJECT_STATE_UNKNOWN)
	    {
	        bStatus = FALSE;
		    break;
	    }
		
        /* check how much space is available in the cbuf */
        retCode = cnxt_cbuf_get_free(pInjectHandle->hPCMClipcbuf, &uFreeBytes);
        if(retCode != CNXT_STATUS_OK)
        {
           CSDEBUG(MODULE_NAME,ERROR_LEVEL, 
                 "Error %d - cnxt_cbuf_get_free\n", retCode);
           break;
        }
        uFreeBytes++;/*plus 1 because in cbuff drv,cnxt_cbuf_get_free will minus 1 byte for unknown reason*/
        uFreeBytes-=32;
        uFreeBytes -= uFreeBytes%32;/*make 32bytes align*/
        if(uFreeBytes < (PCM_BUF_SIZE/4)/*TMP_BUFF_FIX_SIZE*/)
        {
            /*return nothing to let APP ask free buff again*/
            *puLength=0;
            *ppvBuffer=NULL;        
            //CSDEBUG(MODULE_NAME,ERROR_LEVEL,"free cbuff less than %d\n",TMP_BUFF_FIX_SIZE);
            bStatus = TRUE;
            break;
        }

        if(pTmpPcmClipBuffer&&bTmpPcmClipBuffInUse)
        {
            /*previous buffer is not free yet.*/
            //CSDEBUG(MODULE_NAME,ERROR_LEVEL,"previous pTmpPcmClipBuffer is not free, need writeComplete\n");
            uFreeBytes = 0; //uSizeofTmpPcmClipBuffer;
            //*ppvBuffer= pTmpPcmClipBuffer;
            *puLength=uFreeBytes;
        }
        else
        {
            //uFreeBytes = TMP_BUFF_FIX_SIZE;
            /*allocate temp buffer to get data from app*/
            retCode = cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_CACHED,
                            uFreeBytes,
                            CNXT_MEM_DEFAULT,
                            &pTmpPcmClipBuffer);
            if(retCode != CNXT_STATUS_OK || !pTmpPcmClipBuffer)
            {   
                pTmpPcmClipBuffer=NULL;
                uFreeBytes = 0;
                CSDEBUG(MODULE_NAME,ERROR_LEVEL,
                "Error (%d) cnxt_kal_mem_malloc Failed Line %d \n",
                retCode,
                __LINE__);
                bStatus = FALSE;   
            }
            bTmpPcmClipBuffInUse = TRUE;
            uSizeofTmpPcmClipBuffer = uFreeBytes;
            *ppvBuffer=pTmpPcmClipBuffer;
            *puLength=uFreeBytes;
        }
        //CSDEBUG(MODULE_NAME,ERROR_LEVEL,"success uFreeBytes = %d pTmpPcmClipBuffer=0x%x\n",uFreeBytes,pTmpPcmClipBuffer);
        pInjectHandle->uTsBufLen = uFreeBytes;
        bStatus = TRUE;
    }while(0);
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        bStatus= FALSE;
    }

    //CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.uFreeBytes %d\n", __FUNCTION__, uFreeBytes);

    return bStatus;
}


bool CSPcmInject_WriteComplete(CS_CNXT_Inject_SubSystem *pInjectHandle, unsigned int uWrittenSize)
{
    CNXT_STATUS retCode;
    UINT32     uNumWritten, uByteCount=0;
    u_int8 * uTmp8bitPCMBuffer;
    UINT32     uTotalWritten,uSize,uBytesFreeInCbuf;
    BOOL bStatus=TRUE;
    int nAudIndx = pInjectHandle->nAudIndx;

	IS_VALID_PCM_INDEX(nAudIndx);

    if( uWrittenSize==0)
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "Err: %s %d.\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if(pcmClipInjectState[nAudIndx]  == CSPCM_INJECT_STATE_UNKNOWN)
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "Err: %s %d.\n", __FUNCTION__, __LINE__);
        /*do nothing if not playing*/
        return FALSE;
    }
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    
    do{
        
        if(uWrittenSize>uSizeofTmpPcmClipBuffer)
        {
            CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] uWrittenSize>uSizeofTmpPcmClipBuffer, line %d.%d\n",
                            __LINE__, uSizeofTmpPcmClipBuffer);
            uWrittenSize = uSizeofTmpPcmClipBuffer;/*override this to make it PCM clip going on*/
            break;/*break here if don't wanna process overlong data*/
        }

        if(pTmpPcmClipBuffer)
        {
        
            uTotalWritten = 0;
            uSize = uWrittenSize;
#if 0
            if((eMixerAttrb.MixerCfg.uNumChannels +1)==1)
            {
                uSize -= uSize%16;/*16byte width align*/
            }
            else if((eMixerAttrb.MixerCfg.uNumChannels+1)==2)
            {
                uSize -= uSize%32;/*32bytet width align*/
            }
#endif
            if(pInjectHandle->psPcmStartParams.m_uBitsSample == 8)
            {                
                int Endian = pInjectHandle->pEnmEndian;
                retCode = cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_CACHED,
                                uSize*2,
                                CNXT_MEM_DEFAULT,
                                (void**)&pTmp16bitPcmClipBuffer);
                retCode =  cnxt_kal_memset((void *)pTmp16bitPcmClipBuffer,  0, uSize*2);
                uTmp8bitPCMBuffer = (u_int8 *)pTmpPcmClipBuffer;                
                if(Endian == EM_UDIINJECTER_LITTLE_ENDIAN)
                {
                    while(uByteCount != uSize)
                    {
                        pTmp16bitPcmClipBuffer[uByteCount] = (int16)(uTmp8bitPCMBuffer[uByteCount]-128);
                        pTmp16bitPcmClipBuffer[uByteCount]<<=8;
                        uByteCount++;
                    }
                }
                else
                {
                    while(uByteCount != uSize)
                    {
                        pTmp16bitPcmClipBuffer[uByteCount] = (int16)(uTmp8bitPCMBuffer[uByteCount]-128);
                        uByteCount++;
                    }
                }
                
                uSize<<=1;
                uTmp8bitPCMBuffer = (u_int8 *)pTmp16bitPcmClipBuffer;
            }
            else
            {
                uTmp8bitPCMBuffer = (u_int8 *)pTmpPcmClipBuffer;
            }
            while  (uSize > 0) 
            {
                if (uSize > ONE_TIME_SIZE)
                {
                    retCode = cnxt_cbuf_write_data(pInjectHandle->hPCMClipcbuf, 
                                                   (void *)(uTmp8bitPCMBuffer+uTotalWritten), 
                                                   ONE_TIME_SIZE, 
                                                   &uNumWritten);
                    /* Wait until this write has got processed. */
                    cnxt_kal_thread_time_sleep(1);
                }
                else
                {
                    retCode = cnxt_cbuf_write_data(pInjectHandle->hPCMClipcbuf, 
                                                   (void *)(uTmp8bitPCMBuffer+uTotalWritten), 
                                                   uSize, 
                                                   &uNumWritten );
                }
            
                if ((retCode == CNXT_STATUS_OK) || (retCode == CNXT_STATUS_TOO_MUCH_DATA))
                {
                    //CSDEBUG(MODULE_NAME,AV_LEVEL,"PCM Inject: BytesWritten: %d uSize %d\n",uNumWritten, uSize);
                    uTotalWritten  += uNumWritten;
                    uSize -= uNumWritten;
                
                    if (retCode == CNXT_STATUS_TOO_MUCH_DATA)
                    {
                        //CNXT_CBUF_POINTERS BufferPointers;
                        /* We expect an internal flush here, Wait for room in the cbuf */
                        do
                        {   cnxt_kal_thread_time_sleep(20);
                            retCode = cnxt_cbuf_get_free(pInjectHandle->hPCMClipcbuf,&uBytesFreeInCbuf);
                        /*We need free space equal to that we write. */        
                        }while(uBytesFreeInCbuf < (ONE_TIME_SIZE));
                    }
                }
                else
                {
                    CSDEBUG(MODULE_NAME,AV_LEVEL,
                                "PCM Inject: cnxt_cbuf_write_data Failed : %d\n",
                                retCode);
                    break;
                }
            
            }

            cnxt_kal_mem_free(pTmpPcmClipBuffer);
            pTmpPcmClipBuffer = NULL;
            if(pInjectHandle->psPcmStartParams.m_uBitsSample == 8)
            {
                cnxt_kal_mem_free(pTmp16bitPcmClipBuffer);
                pTmp16bitPcmClipBuffer = NULL;
            }
            bTmpPcmClipBuffInUse=FALSE;
            uSizeofTmpPcmClipBuffer = 0;
            //CSDEBUG(MODULE_NAME,AV_LEVEL,"CSPCM: Remaining Bytes:%d Bytes written: %d  \n",uWrittenSize,uTotalWritten);
            if(uTotalWritten==uWrittenSize)
            {
                //CSDEBUG(MODULE_NAME,AV_LEVEL,"PCM Inject: GOOD, Write completed.\n");         
                bStatus = TRUE;
                break;
            }
        }
        else
        {        
            CSDEBUG(MODULE_NAME,AV_LEVEL,"ERROR @line #%d pTmpPcmClipBuffer undeclared \n",__LINE__);        
            bStatus= FALSE;
            break;
        }
    }while(0);

    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        bStatus= FALSE;
    }

    //CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return bStatus;
}
bool CSPcmInject_Flush(CS_CNXT_Inject_SubSystem *pInjectHandle)
{
    CNXT_STATUS retCode;
    BOOL bRet;
    CNXT_CBUF_ATTRIBUTES attributes;
    UINT32    freesize;
    int nAudIndx = pInjectHandle->nAudIndx;


    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);
    
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
    
    do
    {
	    if(pcmClipPlayState[nAudIndx] != CSPCM_PLAY_STATE_PLAYING)
    	{
        	bRet = FALSE;
		    break;
    	}
	
        attributes.uLength = sizeof( CNXT_CBUF_ATTRIBUTES );
        retCode = cnxt_cbuf_get_unit_attributes(pInjectHandle->hPCMClipcbuf,&attributes);
        if(retCode != CNXT_STATUS_OK)
        {
               CSDEBUG(MODULE_NAME,ERROR_LEVEL, 
                     "Error %d - cnxt_cbuf_get_unit_attributes \n", retCode);  
                bRet = FALSE;
    		break;
        }
        do
        {
            retCode = cnxt_cbuf_get_free(pInjectHandle->hPCMClipcbuf,&freesize);
            if(retCode != CNXT_STATUS_OK)
            {
                   CSDEBUG(MODULE_NAME,ERROR_LEVEL,
                         "Error %d - cnxt_cbuf_get_free \n", retCode);  
                   bRet = FALSE;
        		break;
            }

        	/*here we release the mutex,because it is possiable that we will wait a long time*/
            
            freesize++;/*plus 1 because in cbuff drv,cnxt_cbuf_get_free will minus 1 byte for unknown reason*/
            if(attributes.Capabilities.uSize==freesize)
            {
                bRet = TRUE;
                break;
            }
            else
            {
                /*go waiting for buffer empty*/
                CSDEBUG(MODULE_NAME, AV_LEVEL, "waiting..attributes..uSize=%d, freesize=%d\n",attributes.Capabilities.uSize,freesize);

                /*go waiting for buffer empty*/
                bFlushWaiting=TRUE;
                cnxt_kal_thread_time_sleep(10);
                bRet = TRUE;
            }
        }while(0); /*TBD: Need to wait for semaphore*/
        retCode = cnxt_cbuf_flush(pInjectHandle->hPCMClipcbuf, 
                              CNXT_CBUF_RESET_FLUSH);
        if(retCode != CNXT_STATUS_OK)
        {
             CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Error %d - cnxt_cbuf_flush \n", retCode);  
             bRet=FALSE;
        }
    }while(0);
    
    retCode =cnxt_kal_sem_put(gPCMInjectSem);
    if(retCode != CNXT_STATUS_OK)
    {
           CSDEBUG(MODULE_NAME,ERROR_LEVEL,
                 "Error %d - cnxt_kal_sem_put \n", retCode);
    }

   
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return bRet;
}

bool CSPcmInjectClearBuffer(CS_CNXT_Inject_SubSystem *pInjectHandle)
{
    CNXT_STATUS Status;
    BOOL retCode=TRUE;
    int nAudIndx = pInjectHandle->nAudIndx;

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);
	
    if(pcmClipInjectState[nAudIndx]  == CSPCM_INJECT_STATE_UNKNOWN)
    {
        return FALSE;
    }

    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    do{
        Status = cnxt_cbuf_flush(pInjectHandle->hPCMClipcbuf, 
                              CNXT_CBUF_RESET_FLUSH);
        if(Status != CNXT_STATUS_OK)
        {
               CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Error %d - cnxt_cbuf_flush \n", retCode);  
               retCode = FALSE;
               break;
        }
        
    }while(0);

    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}


bool CSPcmPlaySetEndian(CS_CNXT_Inject_SubSystem *pInjectHandle, CSUDIINJECTERPcmEndian_E *pEnmEndian)
{
    //PIPE_STATUS PmStatus = PIPE_STATUS_OK;
    //PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    //PIPE_AUDIO_PRESENTATION_MIXER_ATTRIB eMixerAttrb;
    BOOL retCode=TRUE;
    int nAudIndx = pInjectHandle->nAudIndx;

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.Endian %d \n",nAudIndx, *pEnmEndian);

	IS_VALID_PCM_INDEX(nAudIndx);

    if(!pEnmEndian)
    {
        return FALSE;
    }

    if(pcmClipPlayState[nAudIndx]  <= CSPCM_PLAY_STATE_UNKNOWN /*|| pcmClipPlayState  > CSPCM_PLAY_STATE_IDLE*/)
    {
        /*only allow to config before play*/
        return FALSE;
    }

    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
       
    do
    {
   
       //pAPMObject = gTmApDeviceObj.pAudioPresentation[nAudIndx];

       pInjectHandle->pEnmEndian = *pEnmEndian;
       break;
    }while(0);

    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}
bool CSPcmPlayGetPCMParams(CS_CNXT_Inject_SubSystem *pInjectHandle , CSUDIINJECTERPcmStartParam_S* pvData)
{
    BOOL bStatus=FALSE;
    int nAudIndx = pInjectHandle->nAudIndx;
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

	IS_VALID_PCM_INDEX(nAudIndx);

    if( !pvData)
    {
        return FALSE;
    }
    if(pcmClipPlayState[nAudIndx]  <= CSPCM_PLAY_STATE_UNKNOWN)
    {
        return FALSE;
    }

    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    pvData->m_uBitsSample = pInjectHandle->psPcmStartParams.m_uBitsSample;
    pvData->m_uSampleRate = pInjectHandle->psPcmStartParams.m_uSampleRate;
    pvData->m_uChannels = pInjectHandle->psPcmStartParams.m_uChannels;
    
    bStatus =TRUE;

    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        bStatus= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return bStatus;

}

bool CSPcmPlayGetEndian(CS_CNXT_Inject_SubSystem *pInjectHandle , CSUDIINJECTERPcmEndian_E* pvData)
{
    BOOL retCode=TRUE;
    int nAudIndx = pInjectHandle->nAudIndx;
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

	IS_VALID_PCM_INDEX(nAudIndx);

    if(!pvData)
    {
        return FALSE;
    }
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
    *pvData = pInjectHandle->pEnmEndian;
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        retCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s endian %d.\n", __FUNCTION__, pInjectHandle->pEnmEndian);

    return retCode;

}


bool CSPcmPlay_Pause(int nAudIndx)
{
    BOOL bRetCode=TRUE;
    CNXT_STATUS retCode;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    PIPE_STATUS PmStatus = PIPE_STATUS_OK;
    CS_CNXT_Inject_SubSystem *pInjectHandle;
    int nInjIndx = 0;
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);
    nInjIndx = get_inject_handle_indx(nAudIndx);
    pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjIndx];
	
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    do{
        if(pcmClipPlayState[nAudIndx]  != CSPCM_PLAY_STATE_PLAYING)
        {
            break;
        }
        pAPMObject = gTmApDeviceObj.pAudioPresentation[0];/*We have only one presentation device :)*/
                    
        PmStatus = pAPMObject->mixer_remove(pAPMObject, pInjectHandle->uMixerId);
        if (PmStatus != PIPE_STATUS_OK)
        {
          CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
          retCode = FALSE;
          break;
        }
        pcmClipPlayState[nAudIndx] = CSPCM_PLAY_STATE_PAUSE;
    }while(0);
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        bRetCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return bRetCode;
}


bool CSPcmPlay_Resume(int nAudIndx)
{
    CNXT_STATUS status = CNXT_STATUS_OK;  
    PIPE_AUDIO_PRESENTATION_MIXER_CFG eMixerCfg;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    PIPE_STATUS PmStatus = PIPE_STATUS_OK;
    CS_CNXT_Inject_SubSystem *pInjectHandle;
    BOOL   retCode=FALSE;
    int nInjIndx = 0;
    int nPcmIndex = 0;
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);
    
    nInjIndx = get_inject_handle_indx(nAudIndx);
    pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjIndx];

    //nAudIndx = 0;/*TBD: Need to fix*/

    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }


    do{
        /*TBD: This needs to be fixed...we shouldn't do this :(... but without this, no audio*/
        status = cnxt_cbuf_flush(pInjectHandle->hPCMClipcbuf, 
                                    CNXT_CBUF_RESET_FLUSH);
        if(status!=CNXT_STATUS_OK)
        {
             retCode= FALSE;
             break;
        }
        pAPMObject = gTmApDeviceObj.pAudioPresentation[0];/*We have only one presentation device :)*/
        PmStatus = cs_pipe_apm_attach(NULL, PIPE_AUDIO_PRESENTATION_STREAM_INPUT, 0, FALSE);
        if (PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
            retCode = FALSE;
            break;
        }

	    nPcmIndex = nAudIndx-(CS_TRID_MAX_AUDIO-MAX_PCM_INSTANCES);
		
        /*Open a mixer with default configuration....later stage we set this as per application request*/
        eMixerCfg.MixerInput.uStreamNum = (PIPE_AUDIO_PLAY_STREAM_0);
        eMixerCfg.MixerInput.DataType   = PIPE_AUDIO_STEREO_PCM;
        eMixerCfg.MixerInput.InputSrc.hInputBuf = pInjectHandle->hPCMClipcbuf;
        eMixerCfg.MixerInput.InputType  = PIPE_AUDIO_PRESENTATION_MEM_INPUT;
        eMixerCfg.bLinearPCM = TRUE;
        eMixerCfg.bRepeatInLoop = FALSE;
        eMixerCfg.uSampleRate = pInjectHandle->psPcmStartParams.m_uSampleRate;
        eMixerCfg.uBitsPerSample = (pInjectHandle->psPcmStartParams.m_uBitsSample==8)?16:pInjectHandle->psPcmStartParams.m_uBitsSample;
        eMixerCfg.uNumChannels = pInjectHandle->psPcmStartParams.m_uChannels;
        eMixerCfg.Endian    = pInjectHandle->pEnmEndian;
        eMixerCfg.MixerInput.bMute = gTmPipeObject.TridCoshipSetup[0].bPcmMute[nPcmIndex] ;
	    eMixerCfg.MixerInput.nVolumeLeft = gTmPipeObject.TridCoshipSetup[0].unPcmVolume[nPcmIndex] ;
	    eMixerCfg.MixerInput.nVolumeRight = gTmPipeObject.TridCoshipSetup[0].unPcmVolume[nPcmIndex];
        eMixerCfg.MixerInput.LRMap = gTmPipeObject.TridCoshipSetup[0].unLRMap[nPcmIndex];
        eMixerCfg.bUnsignedData = FALSE;
        switch(pInjectHandle->pEnmEndian)
        {
             case EM_UDIINJECTER_BIG_ENDIAN:
                 eMixerCfg.Endian = PIPE_AUDIO_PRESENTATION_PCM_ENDIAN_BIG;
                 break;
             case EM_UDIINJECTER_LITTLE_ENDIAN:
             default:
                 eMixerCfg.Endian = PIPE_AUDIO_PRESENTATION_PCM_ENDIAN_LITTLE;
                 break;
        }
        
        PmStatus = pAPMObject->mixer_add(pAPMObject, &eMixerCfg, &pInjectHandle->uMixerId);
        if (PmStatus != PIPE_STATUS_OK)
        {
          CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d PmStatus %d\n", __FUNCTION__, __LINE__,PmStatus);
          retCode = FALSE;
          break;
        }

        /*it's ready to play PCM clip*/
        pcmClipPlayState[nAudIndx]  = CSPCM_PLAY_STATE_PLAYING;
        retCode = TRUE;
    }while(0);
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        retCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}


bool CSPcmPlay_Start(int nAudIndx, u_int8 *nMixerId)
{
    CNXT_STATUS status = CNXT_STATUS_OK;
    PIPE_AUDIO_PRESENTATION_MIXER_CFG eMixerCfg;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    PIPE_STATUS PmStatus = PIPE_STATUS_OK; 
    CS_CNXT_Inject_SubSystem *pInjectHandle;
    PIPE_PIPELINE_OBJECT *hPipeObj=NULL;
    BOOL retCode=TRUE;
    int nInjIndx = 0;
    int nPcmIndex = 0;
  
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);

    nInjIndx = get_inject_handle_indx(nAudIndx);
    pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjIndx];
    

    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
    
    do
    {
        /*
             if((pcmClipPlayState[nMixerId] != CSPCM_PLAY_STATE_IDLE) && (pcmClipPlayState != CSPCM_PLAY_STATE_INIT))
             {
                 retCode =  FALSE;
                 CSDEBUG(MODULE_NAME, ERROR_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
         		break;
             }*/
        /*TBD: This needs to be fixed...we shouldn't do this :(... but without this, no audio*/
        status = cnxt_cbuf_flush(pInjectHandle->hPCMClipcbuf, 
                                    CNXT_CBUF_RESET_FLUSH);
        if(status!=CNXT_STATUS_OK)
        {
            retCode= FALSE;
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
            break;
        }
        /*We have only one presentation device :)...hence we can safely always refer to index 0 for presentation*/
        pAPMObject = gTmApDeviceObj.pAudioPresentation[0];
        hPipeObj = cs_tm_get_live_pipe(0); /*This gives the pipeobj to which mixer needs to be attached*/
        CSDEBUG(MODULE_NAME, ERROR_LEVEL,"At: %s %d hPipeObj 0x%x \n", __FUNCTION__, __LINE__, hPipeObj);
        if(hPipeObj == NULL)
        {
        /*If live/ts playback is not playing configure it for default...otherwise use the same config as live*/
            PmStatus = cs_pipe_apm_attach(hPipeObj, PIPE_AUDIO_PRESENTATION_STREAM_INPUT, 0, FALSE);
            if (PmStatus != PIPE_STATUS_OK)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
                retCode = FALSE;
                break;
            }
        }

	    nPcmIndex = nAudIndx-(CS_TRID_MAX_AUDIO-MAX_PCM_INSTANCES);
		
        /*Open a mixer with default configuration....later stage we set this as per application request*/
        eMixerCfg.MixerInput.uStreamNum = (PIPE_AUDIO_PLAY_STREAM_0);
        eMixerCfg.MixerInput.DataType   = PIPE_AUDIO_STEREO_PCM;
        eMixerCfg.MixerInput.InputSrc.hInputBuf = pInjectHandle->hPCMClipcbuf;
        eMixerCfg.MixerInput.InputType  = PIPE_AUDIO_PRESENTATION_MEM_INPUT;
        eMixerCfg.bLinearPCM = TRUE;
        eMixerCfg.bRepeatInLoop = FALSE;
        eMixerCfg.uSampleRate = pInjectHandle->psPcmStartParams.m_uSampleRate;
        eMixerCfg.uBitsPerSample = (pInjectHandle->psPcmStartParams.m_uBitsSample==8)?16:pInjectHandle->psPcmStartParams.m_uBitsSample;
        eMixerCfg.uNumChannels = pInjectHandle->psPcmStartParams.m_uChannels;
        eMixerCfg.MixerInput.bMute = gTmPipeObject.TridCoshipSetup[0].bPcmMute[nPcmIndex] ;
	    eMixerCfg.MixerInput.nVolumeLeft = gTmPipeObject.TridCoshipSetup[0].unPcmVolume[nPcmIndex] ;
	    eMixerCfg.MixerInput.nVolumeRight = gTmPipeObject.TridCoshipSetup[0].unPcmVolume[nPcmIndex];
        eMixerCfg.MixerInput.LRMap = gTmPipeObject.TridCoshipSetup[0].unLRMap[nPcmIndex];
        eMixerCfg.bUnsignedData = FALSE;
        switch(pInjectHandle->pEnmEndian)
        {
             case EM_UDIINJECTER_BIG_ENDIAN:
                 eMixerCfg.Endian = PIPE_AUDIO_PRESENTATION_PCM_ENDIAN_BIG;
                 break;
             case EM_UDIINJECTER_LITTLE_ENDIAN:
             default:
                 eMixerCfg.Endian = PIPE_AUDIO_PRESENTATION_PCM_ENDIAN_LITTLE;
                 break;
        }
        //eMixerCfg.MixerInput.bMute = FALSE;
       
        PmStatus = pAPMObject->mixer_add(pAPMObject, &eMixerCfg, &pInjectHandle->uMixerId);
        if (PmStatus != PIPE_STATUS_OK)
        {
          CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d PmStatus %d\n", __FUNCTION__, __LINE__,PmStatus);
          retCode = FALSE;
          break;
        }
        /*We need to store mixerId to player as well...reason being pcmplay_stop may be called after injecter close*/
        *nMixerId = pInjectHandle->uMixerId;
        /*it's ready to play PCM clip*/
        pcmClipPlayState[nAudIndx]  = CSPCM_PLAY_STATE_PLAYING;
        retCode = TRUE;
    }while(0);
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        retCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}

bool CSPcmPlay_Stop(int nAudIndx, int uMixerId)
{
    BOOL bRetCode=TRUE;
    CNXT_STATUS retCode;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    //PIPE_AUDIO_PRESENTATION_MIXER_ATTRIB eMixerAttrb;
    PIPE_STATUS PmStatus = PIPE_STATUS_OK;
        
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.uMixerId %d \n",nAudIndx, uMixerId);

    IS_VALID_PCM_INDEX(nAudIndx);
	
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }

    do
    {
        if(pcmClipPlayState[nAudIndx] <= CSPCM_PLAY_STATE_IDLE)
        {
            bRetCode = FALSE;
    	    break;
        }
        else if(pcmClipPlayState[nAudIndx] != CSPCM_PLAY_STATE_STOP)
        {
            pAPMObject = gTmApDeviceObj.pAudioPresentation[0]; /*We have only one presentation device :)*/
                    
            PmStatus = pAPMObject->mixer_remove(pAPMObject, uMixerId);
            if (PmStatus != PIPE_STATUS_OK)
            {
              CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
              retCode = FALSE;
              break;
            }
        }
        pcmClipPlayState[nAudIndx]  = CSPCM_PLAY_STATE_STOP;
    }while(0);
    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        bRetCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return bRetCode;
}
bool CSPCMPlay_SetVolume(int nAudIndx, int volume)
{
    PIPE_STATUS PmStatus = PIPE_STATUS_OK; 
    BOOL retCode=TRUE;
    int nInjIndx = 0;
    PIPE_AUDIO_PRESENTATION_MIXER_ATTRIB eMixerAttrb; 
    CS_CNXT_Inject_SubSystem *pInjectHandle;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    int nPcmIndex = 0;
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);

    nInjIndx = get_inject_handle_indx(nAudIndx);
    pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjIndx];
    
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
    
    do
    {
        if(pcmClipPlayState[nAudIndx]  != CSPCM_PLAY_STATE_PLAYING)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Waring!! Not playing yet \n");
            break;
        }

        pAPMObject = gTmApDeviceObj.pAudioPresentation[0]; /*We have only one presentation device :)*/
        
        PmStatus = pAPMObject->get_mixer_attrib(pAPMObject, pInjectHandle->uMixerId, &eMixerAttrb);
        if(PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d PmStatus=%d\n", __FUNCTION__, __LINE__,PmStatus);
            retCode = FALSE;
            break;
        }

        eMixerAttrb.MixerCfg.MixerInput.nVolumeLeft = volume;
        eMixerAttrb.MixerCfg.MixerInput.nVolumeRight = volume;
        
        PmStatus = pAPMObject->set_mixer_attrib(pAPMObject, pInjectHandle->uMixerId, &eMixerAttrb);
        if(PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d PmStatus=%d\n", __FUNCTION__, __LINE__,PmStatus);
            retCode = FALSE;
            break;
        }
    }while(0);
    
    nPcmIndex = nAudIndx-(CS_TRID_MAX_AUDIO-MAX_PCM_INSTANCES);
    gTmPipeObject.TridCoshipSetup[0].unPcmVolume[nPcmIndex] = volume;

    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        retCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}

bool CSPCMPlay_Mute(int nAudIndx ,bool bMute)
{
    PIPE_STATUS PmStatus = PIPE_STATUS_OK; 
    BOOL retCode=TRUE;
    int nInjIndx = 0;
    PIPE_AUDIO_PRESENTATION_MIXER_ATTRIB eMixerAttrb;
    CS_CNXT_Inject_SubSystem *pInjectHandle;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    int nPcmIndex = 0;
    
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);

    nInjIndx = get_inject_handle_indx(nAudIndx);
    pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjIndx];
    
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
    
    do
    {
        if(pcmClipPlayState[nAudIndx]  != CSPCM_PLAY_STATE_PLAYING)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Waring!! Not playing yet \n");
            break;
        }

        pAPMObject = gTmApDeviceObj.pAudioPresentation[0]; /*We have only one presentation device :)*/
        
        PmStatus = pAPMObject->get_mixer_attrib(pAPMObject, pInjectHandle->uMixerId, &eMixerAttrb);
        if(PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d PmStatus=%d\n", __FUNCTION__, __LINE__,PmStatus);
            retCode = FALSE;
            break;
        }
        eMixerAttrb.MixerCfg.MixerInput.bMute = bMute;
        
        PmStatus = pAPMObject->set_mixer_attrib(pAPMObject, pInjectHandle->uMixerId, &eMixerAttrb);
        if(PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d PmStatus=%d\n", __FUNCTION__, __LINE__,PmStatus);
            retCode = FALSE;
            break;
        }
		
    }while(0);

    nPcmIndex = nAudIndx-(CS_TRID_MAX_AUDIO-MAX_PCM_INSTANCES);
	gTmPipeObject.TridCoshipSetup[0].bPcmMute[nPcmIndex] = bMute;

    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        retCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}
bool CSPCMPlay_SetChannel(int nAudIndx ,CSUDIAUDIOChannelType_E eChannelType)
{
    PIPE_STATUS PmStatus = PIPE_STATUS_OK; 
    BOOL retCode=TRUE;
    int nInjIndx = 0;
    PIPE_AUDIO_PRESENTATION_MIXER_ATTRIB eMixerAttrb;
    CS_CNXT_Inject_SubSystem *pInjectHandle;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObject;
    PIPE_AUDIO_PRESENTATION_INPUT *pMixerInput;
    int nPcmIndex = 0;
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.\n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] nAudioIndex=%d.\n",nAudIndx);

    IS_VALID_PCM_INDEX(nAudIndx);

    nInjIndx = get_inject_handle_indx(nAudIndx);
    pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjIndx];
    
    if(CNXT_STATUS_OK!=cnxt_kal_sem_get(gPCMInjectSem,CNXT_KAL_WAIT_FOREVER))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] get sem failed line %d.\n",__LINE__);
        return FALSE;
    }
    
    do
    {
        if(pcmClipPlayState[nAudIndx]  != CSPCM_PLAY_STATE_PLAYING)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Waring!! Not playing yet,but i think this situation shouldn't be happend!!\n");
            break;
        }

        pAPMObject = gTmApDeviceObj.pAudioPresentation[0]; /*We have only one presentation device :)*/
        
        PmStatus = pAPMObject->get_mixer_attrib(pAPMObject, pInjectHandle->uMixerId, &eMixerAttrb);
        if(PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
            retCode = FALSE;
            break;
        }
        pMixerInput = &eMixerAttrb.MixerCfg.MixerInput;
        switch(eChannelType)
		{
    		case EM_UDIAUDIO_CHANNEL_STEREO:
    			pMixerInput->LRMap = PIPE_AUDIO_PRESENTATION_LL_RR;
    			break;
    		case EM_UDIAUDIO_CHANNEL_LEFT:
    			pMixerInput->LRMap = PIPE_AUDIO_PRESENTATION_LL_LR;
    			break;
    		case EM_UDIAUDIO_CHANNEL_RIGHT:
    			pMixerInput->LRMap = PIPE_AUDIO_PRESENTATION_RL_RR;
    			break;
    		case EM_UDIAUDIO_CHANNEL_MIXED_MONO:
    			break;
    		default:
                retCode = FALSE;
    			break;
		}
        CSDEBUG(MODULE_NAME, ERROR_LEVEL,"LRMap %d \n",eMixerAttrb.MixerCfg.MixerInput.LRMap);
        PmStatus = pAPMObject->set_mixer_attrib(pAPMObject, pInjectHandle->uMixerId, &eMixerAttrb);
        if(PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
            retCode = FALSE;
            break;
        }
    }while(0);
    
    nPcmIndex = nAudIndx-(CS_TRID_MAX_AUDIO-MAX_PCM_INSTANCES);
    switch(eChannelType)
    {
        case EM_UDIAUDIO_CHANNEL_STEREO:
            gTmPipeObject.TridCoshipSetup[0].unLRMap[nPcmIndex] = PIPE_AUDIO_PRESENTATION_LL_RR;
            break;
        case EM_UDIAUDIO_CHANNEL_LEFT:
            gTmPipeObject.TridCoshipSetup[0].unLRMap[nPcmIndex] = PIPE_AUDIO_PRESENTATION_LL_LR;
            break;
        case EM_UDIAUDIO_CHANNEL_RIGHT:
            gTmPipeObject.TridCoshipSetup[0].unLRMap[nPcmIndex] = PIPE_AUDIO_PRESENTATION_RL_RR;
            break;
        case EM_UDIAUDIO_CHANNEL_MIXED_MONO:
            break;
        default:
            retCode = FALSE;
            break;
    }

    if(CNXT_STATUS_OK!=cnxt_kal_sem_put(gPCMInjectSem))
    {
        CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] put sem failed line %d.\n",__LINE__);
        retCode= FALSE;
    }

    CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s end.\n", __FUNCTION__);

    return retCode;
}

