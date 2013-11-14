/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_player.c
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
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_player.h"

#include "generic_include.h"
#include "tm_player.h"

#include "data_mngr.h"
#include "pvr_fs.h"


u_int8 *pRecordData;


#define  MODULE_NAME   "CS_PLAYER"
#define CS_INVALID_PID  (8192)
#define MAX_PID (0x1fff)
#define MIN_PID  0
#define AMM_THRESHOLD_VALUE 3584
#define CSUDI_INVALID_INDEX (-1)
#define IS_VALID_PID(pid)  ( (((pid) < MIN_PID) || ((pid) >= MAX_PID))?FALSE:TRUE)
	
	
#define IS_VALID_PLAYER_INDX(uPlayerIndx)  \
	do                                                   \
	{                                                      \
		if((uPlayerIndx>MAX_PLAYER_HANDLES)||(uPlayerIndx<0))                     \
		{                                                \
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "%s INVALID INDX \n", __FUNCTION__); \
			return CSUDIPLAYER_ERROR_INVALID_HANDLE; \
		}                                                       \
	} while( 0 )
#define IS_VALID_PLAYER_HNDL(phndl)  \
	do                                                   \
	{                                                      \
		if( phndl == NULL )                     \
		{                                                \
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "%s NULL HNDL\n", __FUNCTION__); \
			return CSUDIPLAYER_ERROR_INVALID_HANDLE; \
		}                                                       \
	} while( 0 )
	
	
CS_CNXT_Player_Config gPlayerConfig;
static UDI_PLAYER_HNDL gUDIPlayerHndl[MAX_PLAYER_HANDLES];/*For time being we are restricting it to 32*/
CNXT_SEM_ID gPlayerSem;
CNXT_SEM_ID gPlayerCallbackSem;

extern bool bPlayerStarted;
extern bool bInjectIframe;
extern bool bPlayerStatus ;
extern CNXT_QUEUE_ID injectDataQueue;
extern CSUDI_Error_Code TM_AOUTSetMute(CSUDIAOUTDevices_E eAudioDevice,CSUDI_BOOL bMute);


CNXT_STATUS CS_TM_Player_Init(void)
{	
	PIPE_DEMUX_OBJECT		 *hDemuxObj=NULL;
	CNXT_STATUS Retcode = CNXT_STATUS_OK ;
	char sem_name[20];
	int i;
	sprintf(sem_name,"PlayerSem0");
	Retcode=cnxt_kal_sem_create(1, sem_name, &(gPlayerSem));
	if(CNXT_STATUS_OK!=Retcode)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "sem_create  failed\n");
		return Retcode;
	}
    cnxt_kal_memset(sem_name,0,20);
    sprintf(sem_name,"Callback0");
	Retcode=cnxt_kal_sem_create(1, sem_name, &(gPlayerCallbackSem));
	if(CNXT_STATUS_OK!=Retcode)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Callback sem_create  failed\n");
		return Retcode;
	}
    
    // add these code for resolved live trickmode issue.

	if (cnxt_pvr_dm_init() != CNXT_STATUS_OK)
	{
		 CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "cnxt_data_mngr_init failed\n");
	}

	hDemuxObj = gTmPipeObject.hDemuxObj[1];

	PIPE_RECORD_CFG  RecCfg;
	cnxt_kal_memset ( RecCfg.uRecCfg, 0, sizeof(RecCfg.uRecCfg));
	RecCfg.uRecordBufSize = 30 * 512 * 188;
	
	if ( hDemuxObj->record_config ( hDemuxObj, &RecCfg ) != PIPE_STATUS_OK )
	{
	  printf("Config demux for recorder\n" );
	}
	
	for(i=0;i<MAX_PLAYER_HANDLES;i++)
	{
	  gUDIPlayerHndl[i].bPlayerOpen = FALSE;
	  gUDIPlayerHndl[i].uPlayerindx = 0;
	  gUDIPlayerHndl[i].uStreamCount = 0;
	}
		
	return Retcode;
}



BOOL CSUDIPLAYERTerm()
{
	int  i = 0;

	printf("start %s\n",__FUNCTION__);
	
	for(i=0;i<MAX_PLAYER_HANDLES;i++)
	{
	  	if (gUDIPlayerHndl[i].bPlayerOpen)
		{
			if (CSUDI_SUCCESS != CSUDIPLAYERClose(&gUDIPlayerHndl[i]))
			{
				CSDEBUG(MODULE_NAME,ERROR_LEVEL,"fail to cose player\n");
				break;
			}
		}
	}

	printf("exit %s\n",__FUNCTION__);

	return(i==MAX_PLAYER_HANDLES);
}



void cs_tm_notify_player(CSUDIPlayerEventType_E UDIEvent)

{
    int uPlayerIndx, callbkIndx;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
    for(uPlayerIndx=0; uPlayerIndx<MAX_PLAYER_HANDLES; uPlayerIndx++)
    {
        pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];

        for(callbkIndx=0; callbkIndx<MAX_CALLBK_FUNCS; callbkIndx++)
        {
            if(pPlayerSubsystem->PlayCallbackData[callbkIndx].Func==NULL)
                continue;
            if((pPlayerSubsystem->PlayCallbackData[callbkIndx].eEvent & UDIEvent)==UDIEvent)
            {
                pPlayerSubsystem->PlayCallbackData[callbkIndx].Func(&gUDIPlayerHndl[uPlayerIndx], UDIEvent, 
                                                    pPlayerSubsystem->PlayCallbackData[callbkIndx].pvUserData);
            }
        }
    }
    return;
}



static BOOL CS_CheckStreamParam(int nStreamCnt,CSUDIStreamInfo_S * psStreamInfo)
{
	int count,temp_count;

	if (nStreamCnt == 1)
	{
		return TRUE;
	}
	
	for(count =0; count < nStreamCnt; count++)
	{
		for(temp_count =count+1; temp_count < nStreamCnt; temp_count++)
		{
			if(psStreamInfo[count].m_nPid == psStreamInfo[temp_count].m_nPid)
			{
				if (psStreamInfo[temp_count].m_eContentType != EM_UDI_CONTENT_PCR
						&&psStreamInfo[count].m_eContentType != EM_UDI_CONTENT_PCR)
				{
					return FALSE;
				}
			}
			if (psStreamInfo[count].m_eContentType == psStreamInfo[temp_count].m_eContentType)	
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}



int cs_cnxt_get_free_callbackindx(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
	int i=0;
	for(i=0;i<MAX_CALLBK_FUNCS;i++)
	{
		if(pPlayerSubsystem->PlayCallbackData[i].bInuse == FALSE)
		break;
	}
	return i;
}
int cs_cnxt_get_free_player(void)
{
	int i=0;
	for(i=0;i<MAX_PLAYER_HANDLES;i++)
	{
		if(gPlayerConfig.hPlayerHandle[i].bPlayerInuse== FALSE)
		break;
	}
	return i;

}



bool bIsPlayeropen(u_int32 hPlayer)
{
	int i=0;
	for(i=0;i<MAX_PLAYER_HANDLES;i++)
	{
		
		if(hPlayer == (u_int32)&gUDIPlayerHndl[i])
		{
			if(gUDIPlayerHndl[i].bPlayerOpen == TRUE)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Playeropen hndl%d 0x%x 0x%x\n",i, hPlayer, &gUDIPlayerHndl[i]);
				return TRUE;
			}
		}
	}

	return FALSE;
}
bool Is_audio_fmt_supported(CSUDIAUDStreamType_E eStreamType)
{
	switch(eStreamType)
	{
	case EM_UDI_AUD_STREAM_AC3:
	case EM_UDI_AUD_STREAM_DTS:
	case EM_UDI_AUD_STREAM_MPEG1:
	case EM_UDI_AUD_STREAM_MPEG2:
	case EM_UDI_AUD_STREAM_PCM:
	case EM_UDI_AUD_STREAM_LPCM:
	case EM_UDI_AUD_STREAM_MP3:
	/*case EM_UDI_AUD_STREAM_MPEG_AAC:
	case EM_UDI_AUD_STREAM_HE_AAC:
	case EM_UDI_AUD_STREAM_RAW_AAC:
	case EM_UDI_AUD_STREAM_AAC_PLUS_ADTS:
	case EM_UDI_AUD_STREAM_AAC_PLUS_LOAS:*/
	case EM_UDI_AUD_STREAM_AC3_PLUS:
	case EM_UDI_AUD_STREAM_WMA_STD:
	case EM_UDI_AUD_STREAM_DTS_HD:
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
bool Is_video_fmt_supported(CSUDIAUDStreamType_E eStreamType)
{
	switch(eStreamType)
	{
	case EM_UDI_VID_STREAM_MPEG1:
	case EM_UDI_VID_STREAM_MPEG2:
	case EM_UDI_VID_STREAM_H264:
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


bool bAudioDecoderInUse(int AudIndx)
{
	int uPlayerIndx;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	for(uPlayerIndx =0;uPlayerIndx<MAX_PLAYER_HANDLES; uPlayerIndx++)
	{
		pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
		if(pPlayerSubsystem==NULL)
		continue;
		if((pPlayerSubsystem->AudioIndx == AudIndx)&&
				(pPlayerSubsystem->bPlayerInuse == TRUE)/*&&
				(pPlayerSubsystem->bVideoOnlyStream == FALSE)*/) /*This is required as no player module is using audio*/
		{
			return TRUE;
		}
	}
	return FALSE;
}


bool bVideoDecoderInUse(int VidIndx)
{
	int uPlayerIndx;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	for(uPlayerIndx =0;uPlayerIndx<MAX_PLAYER_HANDLES; uPlayerIndx++)
	{
		pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
		if(pPlayerSubsystem==NULL)
		continue;
		if((pPlayerSubsystem->VideoIndx== VidIndx)&&
				(pPlayerSubsystem->bPlayerInuse == TRUE)/*&&
				(pPlayerSubsystem->bRadioStream == FALSE)*/) /*This is required as no player module is using video*/
		{
			return TRUE;
		}
	}
	return FALSE;
}


int cs_tm_get_surface_indx(int VidIndx)
{ 
    /*int uPlayerIndx;
    CS_TM_Player_SubSystem *pPlayerSubsystem;

    for(uPlayerIndx=0; uPlayerIndx<MAX_PLAYER_HANDLES; uPlayerIndx++)
    {
        pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
        if((pPlayerSubsystem->VideoIndx == VidIndx)&&
            (pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
            (pPlayerSubsystem->bIframeInject == TRUE))
        {
            printf("I-frame PIPE_VP_VIDEO_SECONDARY_SURFACE \n ");
            return PIPE_VP_VIDEO_SECONDARY_SURFACE;
        }
    }
    */
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"I-frame PIPE_VP_VIDEO_PRIMARY_SURFACE \n ");

    return PIPE_VP_VIDEO_PRIMARY_SURFACE;
}



PIPE_PIPELINE_OBJECT* cs_tm_get_live_pipe(int AudIndx)
{
    PIPE_PIPELINE_OBJECT *hPipeObject;    
    int uPlayerIndx;
    CS_TM_Player_SubSystem *pPlayerSubsystem;

    hPipeObject = NULL;
    for(uPlayerIndx=0; uPlayerIndx<MAX_PLAYER_HANDLES; uPlayerIndx++)
    {
        pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
        if((pPlayerSubsystem->pAudioObj != NULL)&&
            (pPlayerSubsystem->AudioIndx == AudIndx))
        {
            hPipeObject = pPlayerSubsystem->pPipeObj;
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s: PipeObj 0x%x \n",__FUNCTION__, hPipeObject);
            break;
        }
    }

    return hPipeObject;
}



PIPE_STATUS cs_pipe_vpm_attach(PIPE_VP_SURFACE_INPUT Input, 
                                     PIPE_VP_SURFACE_TYPE uSurfaceNum, 
                                     int PipeIndx)
{
    PIPE_STATUS PmStatus = PIPE_STATUS_OK;

    PIPE_VP_SURFACE_ATTRIB sAttrib;

    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s Start  \n", __FUNCTION__);
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Attaching %d video[%d]<->surface[%d] PipeIndx %d\n",
                                       Input.Type,uSurfaceNum, uSurfaceNum, PipeIndx);

    // 2011-01-14 TerenceZhang Masked begin:

    cnxt_kal_memset(&sAttrib,0,sizeof(sAttrib));	   	
    PmStatus = gTmVpDeviceObj.pHDVideoSurface[uSurfaceNum]->get_attrib(gTmVpDeviceObj.pHDVideoSurface[uSurfaceNum], &sAttrib);                                                                      
    if (PmStatus != PIPE_STATUS_OK)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
        return PIPE_STATUS_UNKNOWN;
    }

#ifdef DUAL_VIDEO_SURFACE
    if((sAttrib.Input.pPipe != Input.pPipe)||(sAttrib.Input.Type!=Input.Type))
#else
    if((sAttrib.Input.pPipe != Input.pPipe)||(sAttrib.Input.Type!=Input.Type)||(sAttrib.Input.hImageHandle!=Input.hImageHandle))
#endif
    {

        if((sAttrib.bSurfaceEnabled==TRUE)&&(sAttrib.Input.Type!=PIPE_VP_SURFACE_INPUT_TYPE_NONE))
        {
            PmStatus = gTmVpDeviceObj.pHDVideoSurface[uSurfaceNum]->surface_disable(gTmVpDeviceObj.pHDVideoSurface[uSurfaceNum]);
            if(PmStatus != PIPE_STATUS_OK)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
                return PIPE_STATUS_UNKNOWN;
            }
        }    

        PmStatus = gTmVpDeviceObj.pHDVideoSurface[uSurfaceNum]->set_input(gTmVpDeviceObj.pHDVideoSurface[uSurfaceNum],&Input);
        if (PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
        }

#ifndef DUAL_VIDEO_SURFACE
        if((gTmVpDeviceObj.bShowVideo[uSurfaceNum]==TRUE)&&(Input.Type!=PIPE_VP_SURFACE_INPUT_TYPE_NONE))
        {
            PmStatus = gTmVpDeviceObj.pHDVideoSurface[uSurfaceNum]->surface_enable(gTmVpDeviceObj.pHDVideoSurface[uSurfaceNum]);
            if(PmStatus != PIPE_STATUS_OK)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
                return PIPE_STATUS_UNKNOWN;
            }
        }            
#endif		
    }



    cnxt_kal_memset(&sAttrib,0,sizeof(sAttrib));	   	
    PmStatus = gTmVpDeviceObj.pSDVideoSurface[uSurfaceNum]->get_attrib(gTmVpDeviceObj.pSDVideoSurface[uSurfaceNum],&sAttrib);
    if (PmStatus != PIPE_STATUS_OK)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
        return PIPE_STATUS_UNKNOWN;
    }

#ifdef DUAL_VIDEO_SURFACE
    if((sAttrib.Input.pPipe != Input.pPipe)||(sAttrib.Input.Type!=Input.Type))
#else
    if((sAttrib.Input.pPipe != Input.pPipe)||(sAttrib.Input.Type!=Input.Type)||(sAttrib.Input.hImageHandle!=Input.hImageHandle))
#endif
    {

        if((sAttrib.bSurfaceEnabled==TRUE)&&(sAttrib.Input.Type!=PIPE_VP_SURFACE_INPUT_TYPE_NONE))
        {
            PmStatus = gTmVpDeviceObj.pSDVideoSurface[uSurfaceNum]->surface_disable(gTmVpDeviceObj.pSDVideoSurface[uSurfaceNum]);
            if(PmStatus != PIPE_STATUS_OK)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
                return PIPE_STATUS_UNKNOWN;
            }
        }    

        PmStatus = gTmVpDeviceObj.pSDVideoSurface[uSurfaceNum]->set_input(gTmVpDeviceObj.pSDVideoSurface[uSurfaceNum],&Input);
        if (PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
        }

#ifndef DUAL_VIDEO_SURFACE
        if((gTmVpDeviceObj.bShowVideo[uSurfaceNum]==TRUE)&&(Input.Type!=PIPE_VP_SURFACE_INPUT_TYPE_NONE))
        {
            PmStatus = gTmVpDeviceObj.pSDVideoSurface[uSurfaceNum]->surface_enable(gTmVpDeviceObj.pSDVideoSurface[uSurfaceNum]);
            if(PmStatus != PIPE_STATUS_OK)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
                return PIPE_STATUS_UNKNOWN;
            }
        }            
#endif
    }

#ifdef DUAL_VIDEO_SURFACE
    {
        extern void cs_tm_display_update();
        cs_tm_display_update();
    }
#endif
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s end  \n", __FUNCTION__);
// 2011-01-14 TerenceZhang Masked end
    return PmStatus;
}
PIPE_STATUS cs_pipe_apm_attach(PIPE_PIPELINE_OBJECT *hPipeObj, 
                                     PIPE_AUDIO_PRESENTATION_INPUT_TYPE InputType,
                                     int nAudIndx,
                                     bool bDecPassthru)
{
    PIPE_STATUS PmStatus = PIPE_STATUS_OK;   
    int chain=0;
    PIPE_AUDIO_PRESENTATION_ATTRIB audio_pre_attr;
    PIPE_AUDIO_PRESENTATION_OUTPUT_ATTRIB AudioPresentationOutputAttr;
    extern CSUDI_Volume_Cfg_S g_sVlmCfg;
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObj = gTmApDeviceObj.pAudioPresentation[chain];
    extern CSUDI_BOOL g_bHdmiMuteInOpMode;
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Enter Function :%s \n",__FUNCTION__);
    
    do
    {        
        cnxt_kal_memset(&audio_pre_attr, 0, sizeof(audio_pre_attr));    
        PmStatus = pAPMObj->get_attrib(pAPMObj,&audio_pre_attr);        
        if(PmStatus != PIPE_STATUS_OK)                
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
            break;            
        }
        {//basic config
            audio_pre_attr.Cfg.pVPDevice = gTmVpDeviceObj.pHDDevice;
            audio_pre_attr.Cfg.uNumOfInput = 1;
            
            audio_pre_attr.Cfg.Input[0].bIsMaster = TRUE;
            audio_pre_attr.Cfg.Input[0].uStreamNum = PIPE_AUDIO_PLAY_STREAM_0;
            audio_pre_attr.Cfg.Input[0].DataType = PIPE_AUDIO_STEREO_PCM;
            audio_pre_attr.Cfg.Input[0].InputSrc.pInputPipe = hPipeObj;
            audio_pre_attr.Cfg.Input[0].InputType = PIPE_AUDIO_PRESENTATION_STREAM_INPUT;
            audio_pre_attr.Cfg.Input[0].nVolumeLeft   = g_sVlmCfg.nVolumeLeft;
            audio_pre_attr.Cfg.Input[0].nVolumeRight = g_sVlmCfg.nVolumeRight;
            //audio_pre_attr.Cfg.Input[0].bPcmMode = TRUE;
            
            audio_pre_attr.Cfg.Input[1].bIsMaster = TRUE;
            audio_pre_attr.Cfg.Input[1].uStreamNum = PIPE_AUDIO_ENC_STREAM_0;
            audio_pre_attr.Cfg.Input[1].DataType = PIPE_AUDIO_PASSTHROUGH_DOLBY;
            audio_pre_attr.Cfg.Input[1].InputSrc.pInputPipe = hPipeObj;
            audio_pre_attr.Cfg.Input[1].InputType = PIPE_AUDIO_PRESENTATION_STREAM_INPUT;
            audio_pre_attr.Cfg.Input[1].nVolumeLeft   = g_sVlmCfg.nVolumeLeft;
            audio_pre_attr.Cfg.Input[1].nVolumeRight = g_sVlmCfg.nVolumeRight;
            //audio_pre_attr.Cfg.Input[1].bPcmMode = TRUE;
            
            audio_pre_attr.Cfg.OutputMap[0] = 0;
            audio_pre_attr.Cfg.OutputMap[1] = 0;
            
            audio_pre_attr.Cfg.OutputMap[0] |=( 1 << PIPE_AUDIO_RCA_OUTPUT_0);
            
            if(gTmPipeObject.TridCoshipSetup[chain].eChannelType == EM_UDIAUDIO_CHANNEL_STEREO)
            {
                audio_pre_attr.Cfg.Input[0].LRMap = PIPE_AUDIO_PRESENTATION_LL_RR;
            }
            else if(gTmPipeObject.TridCoshipSetup[chain].eChannelType == EM_UDIAUDIO_CHANNEL_LEFT)
            {
                audio_pre_attr.Cfg.Input[0].LRMap = PIPE_AUDIO_PRESENTATION_LL_LR;
            }
            else if(gTmPipeObject.TridCoshipSetup[chain].eChannelType == EM_UDIAUDIO_CHANNEL_RIGHT)
            {
                audio_pre_attr.Cfg.Input[0].LRMap = PIPE_AUDIO_PRESENTATION_RL_RR;
            }
            else if(gTmPipeObject.TridCoshipSetup[chain].eChannelType == EM_UDIAUDIO_CHANNEL_MIXED_MONO)
            {
                //how to handle this case???
                audio_pre_attr.Cfg.Input[0].LRMap = PIPE_AUDIO_PRESENTATION_LL_RR;
            }

        }
        if(gTmPipeObject.TridCoshipSetup[chain].eAoutMode == EM_UDIAOUT_BYPASS)
        {

            if((gTmPipeObject.TridCoshipSetup[chain].eBypassAoutDevice&EM_UDIAOUT_SPDIF)==EM_UDIAOUT_SPDIF)
            {
                audio_pre_attr.Cfg.OutputMap[1] |= (1<<PIPE_AUDIO_SPDIF_OUTPUT_0);
            }
            else
            {
                audio_pre_attr.Cfg.OutputMap[0] |= (1<<PIPE_AUDIO_SPDIF_OUTPUT_0);
            }
            
            if((gTmPipeObject.TridCoshipSetup[chain].eBypassAoutDevice&EM_UDIAOUT_HDMI)==EM_UDIAOUT_HDMI)
            {
                audio_pre_attr.Cfg.OutputMap[1] |= (1<<PIPE_AUDIO_HDMI_OUTPUT_0);
            }
            else
            {
                 audio_pre_attr.Cfg.OutputMap[0] |= (1<<PIPE_AUDIO_HDMI_OUTPUT_0);
            }

            if((audio_pre_attr.Cfg.OutputMap[1] == 0)&& (bDecPassthru == TRUE))
            {/*configured for decode and pass through, but output map not set... Is it correct? */
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d %d \n", __FUNCTION__, __LINE__, PmStatus);
                break;
            }
            else if(bDecPassthru == TRUE)
            {                
                audio_pre_attr.Cfg.uNumOfInput = 2;
            }
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"bDecPassThru %d uNumInput %d @ %d\n", bDecPassthru,
                                            audio_pre_attr.Cfg.uNumOfInput, __LINE__);
            if((bDecPassthru==FALSE)&&(audio_pre_attr.Cfg.uNumOfInput==1))
            {
                audio_pre_attr.Cfg.OutputMap[0] = 1<< PIPE_AUDIO_RCA_OUTPUT_0 |
                                                         1 << PIPE_AUDIO_HDMI_OUTPUT_0 |
                                                        1 << PIPE_AUDIO_SPDIF_OUTPUT_0;
                audio_pre_attr.Cfg.OutputMap[1] = 0;
            }
        }
        else if(gTmPipeObject.TridCoshipSetup[chain].eAoutMode == EM_UDIAOUT_DECODE)
        {
            
            if((gTmPipeObject.TridCoshipSetup[chain].eDecAoutDevice&EM_UDIAOUT_SPDIF)==EM_UDIAOUT_SPDIF)
            {
                audio_pre_attr.Cfg.OutputMap[0] |=( 1 << PIPE_AUDIO_SPDIF_OUTPUT_0 );
            }
            else
            {
               if(bDecPassthru == TRUE) 
               {
                   audio_pre_attr.Cfg.OutputMap[1] |=( 1 << PIPE_AUDIO_SPDIF_OUTPUT_0 );
               }
               else
               {
                   audio_pre_attr.Cfg.OutputMap[0] |=( 1 << PIPE_AUDIO_SPDIF_OUTPUT_0 );
               }
            }
            if((gTmPipeObject.TridCoshipSetup[chain].eDecAoutDevice&EM_UDIAOUT_HDMI)==EM_UDIAOUT_HDMI)
            {
                audio_pre_attr.Cfg.OutputMap[0] |=( 1 << PIPE_AUDIO_HDMI_OUTPUT_0 );
            }           
            else
            {
                if(bDecPassthru == TRUE) 
                {
                    audio_pre_attr.Cfg.OutputMap[1] |=( 1 << PIPE_AUDIO_HDMI_OUTPUT_0 );
                }
                else
                {
                  audio_pre_attr.Cfg.OutputMap[0] |=( 1 << PIPE_AUDIO_HDMI_OUTPUT_0 );  
                }
                
            }
            
            if((audio_pre_attr.Cfg.OutputMap[1] == 0)&&(bDecPassthru == TRUE))
            {/*configured for decode and pass through, but output map not set... Is it correct? */
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERROR: %s %d %d \n", __FUNCTION__, __LINE__, PmStatus);
                break;
            }
            else if(bDecPassthru == TRUE)
            {                
                audio_pre_attr.Cfg.uNumOfInput = 2;
            }
            
            if((bDecPassthru==FALSE)&&(audio_pre_attr.Cfg.uNumOfInput==1))
            {
                audio_pre_attr.Cfg.OutputMap[0] =  1<< PIPE_AUDIO_RCA_OUTPUT_0 |
                                                         1 << PIPE_AUDIO_HDMI_OUTPUT_0 |
                                                        1 << PIPE_AUDIO_SPDIF_OUTPUT_0;
                audio_pre_attr.Cfg.OutputMap[1] = 0;
            }
        }
        
        {
			audio_pre_attr.Cfg.Output[PIPE_AUDIO_RCA_OUTPUT_0].bMute = gTmPipeObject.TridCoshipSetup[0].AoutRCAmute;
            audio_pre_attr.Cfg.Output[PIPE_AUDIO_SPDIF_OUTPUT_0].bMute = gTmPipeObject.TridCoshipSetup[0].Aoutspdifmute;
            audio_pre_attr.Cfg.Output[PIPE_AUDIO_HDMI_OUTPUT_0].bMute = gTmPipeObject.TridCoshipSetup[0].AoutHDMImute;
            audio_pre_attr.Cfg.uPortVolMap = audio_pre_attr.Cfg.OutputMap[0]|audio_pre_attr.Cfg.OutputMap[1];
            PmStatus = pAPMObj->config(pAPMObj,&audio_pre_attr.Cfg);
            if (PmStatus != PIPE_STATUS_OK)
            {
                
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR: %s %d %d \n", __FUNCTION__, __LINE__, PmStatus);
                break;
            }

        }

        if((audio_pre_attr.Cfg.OutputMap[1]&( 1 << PIPE_AUDIO_HDMI_OUTPUT_0 )) == ( 1 << PIPE_AUDIO_HDMI_OUTPUT_0 ))
        {
            bool bHDMIAC3support = FALSE;

            extern bool IsHdmiSupportEncData(void);
            //check HDMI port  bypass cap
            bHDMIAC3support = IsHdmiSupportEncData();
            if(bHDMIAC3support == FALSE && !gTmPipeObject.TridCoshipSetup[chain].AoutHDMImute)
            {     
                if(TM_AOUTSetMute(EM_UDIAOUT_HDMI,TRUE) != CSUDI_SUCCESS)                
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
                    break;            
                }
                
                g_bHdmiMuteInOpMode = TRUE;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Function :%s mute hdmi while not supported encode in bypass \n",__FUNCTION__);
            }
        }else if((audio_pre_attr.Cfg.OutputMap[0]&( 1 << PIPE_AUDIO_HDMI_OUTPUT_0 )) == ( 1 << PIPE_AUDIO_HDMI_OUTPUT_0 ))
        {
            if(g_bHdmiMuteInOpMode && !gTmPipeObject.TridCoshipSetup[chain].AoutHDMImute)
            {
                if(TM_AOUTSetMute(EM_UDIAOUT_HDMI,FALSE) != CSUDI_SUCCESS)                
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
                    break;            
                } 
                g_bHdmiMuteInOpMode = FALSE;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Function :%s unmute hdmi \n",__FUNCTION__);
            }
        } 
    }while(0);
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Exit Function :%s \n",__FUNCTION__);
	return PmStatus;
}


PIPE_STATUS cs_pipe_apm_detach(PIPE_PIPELINE_OBJECT *hPipeObj)
{
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;
    PIPE_AUDIO_PRESENTATION_CFG AudioPresentationCfg;
    
	cnxt_kal_memset(&AudioPresentationCfg, 0, sizeof(AudioPresentationCfg));

    PmStatus = gTmApDeviceObj.pAudioPresentation[0]->config(gTmApDeviceObj.pAudioPresentation[0], 
                                                            &AudioPresentationCfg);
    if(PmStatus != PIPE_STATUS_OK)
    {
  	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERROR: %s %d \n", __FUNCTION__, __LINE__);
    }
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Exit Function :%s \n",__FUNCTION__);
	return PmStatus;
}


PIPE_AUDIO_FORMAT get_tm_audio_format(CSUDIAUDStreamType_E StreamType)
{
	/* start audio decoder according to input info*/
	CNXT_AUDIO_FORMAT AudioFormat;
	switch(StreamType)
	{
	case EM_UDI_AUD_STREAM_AC3:          ///<   Dolby Digital AC3 audio  
		AudioFormat = PIPE_AUDIO_FMT_DOLBY_DIGITAL;
		break;
	case EM_UDI_AUD_STREAM_MPEG1:        ///<   MPEG1/2, layer 1/2. This does not support layer 3 (mp3) 
	case EM_UDI_AUD_STREAM_MPEG2:        ///<   MPEG1/2, layer 1/2. This does not support layer 3 (mp3) 
		AudioFormat = PIPE_AUDIO_FMT_MPEG;
		break;
	case EM_UDI_AUD_STREAM_MP3:          ///<  MPEG1/2, layer 3 
		AudioFormat = PIPE_AUDIO_FMT_MP3;
		break;
	case EM_UDI_AUD_STREAM_MPEG_AAC:     ///<  Advanced audio coding. Part of MPEG-4  
	case EM_UDI_AUD_STREAM_HE_AAC:       ///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE)  
	case EM_UDI_AUD_STREAM_RAW_AAC: 
		AudioFormat = PIPE_AUDIO_FMT_AAC;
		break;     
	case EM_UDI_AUD_STREAM_AAC_PLUS_ADTS: ///<   AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format)  
	case EM_UDI_AUD_STREAM_AAC_PLUS_LOAS:///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream)  
		AudioFormat = PIPE_AUDIO_FMT_AAC_PLUS;
		break;
	case EM_UDI_AUD_STREAM_AC3_PLUS:     ///<   Dolby Digital Plus (AC3+ or DDP) audio 
		AudioFormat = PIPE_AUDIO_FMT_DD_PLUS;
		break;
	case EM_UDI_AUD_STREAM_DTS:          ///<   Digital Surround sound 
	case EM_UDI_AUD_STREAM_DTS_HD:       ///<   Digital Surround sound, HD 
		AudioFormat = PIPE_AUDIO_FMT_DTS;
		break;
	case EM_UDI_AUD_STREAM_WMA_STD:      ///<   WMA Standard  
	case EM_UDI_AUD_STREAM_WMA_PRO:      ///<   WMA Professional 
		AudioFormat = PIPE_AUDIO_FMT_WMA;
		break;
	case EM_UDI_AUD_STREAM_CDDA:         ///<  1979年，Philips和Sony公司结盟联合开发CD-DA Compact Disc-Digital Audio,精密光盘数字音频）标准 
	case EM_UDI_AUD_STREAM_PCM:          ///<  PCM data
	case EM_UDI_AUD_STREAM_LPCM:     
		/*AudioFormat = CNXT_AUDIO_FMT_AUTO;
					break;*/
	default:
		/*unknown format*/
		AudioFormat=-1;
		break;
		/*break;*/
	}
	return AudioFormat;
}



bool cs_tm_restart_video(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S psStreamInfo)
{
	bool retcode = TRUE;
	int uPlayerIndx;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_DEMUX_OBJECT        *hDemuxObj=NULL;
	PIPE_VIDEO_OBJECT        *hVideoObj=NULL;
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;

	IS_VALID_PLAYER_HNDL(hPlayer);
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);

	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
	
	hPipeObj = pPlayerSubsystem->pPipeObj;
	hDemuxObj = pPlayerSubsystem->pDemuxObj;
	hVideoObj = pPlayerSubsystem->pVideoObj;
	do
	{
	    PmStatus = hDemuxObj->demux_play_ctrl(hDemuxObj, FALSE);
        if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
			eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
			break;
		}
		if((hVideoObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
		{
			/* stop video */
			PmStatus = hVideoObj->decoder_ctrl(hVideoObj, FALSE);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			PmStatus = hPipeObj->delete_device(hPipeObj, (PIPE_OBJ_HANDLE)hVideoObj);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
		}
		if((hVideoObj != NULL)&& IS_VALID_PID(psStreamInfo.m_nPid))
		{
			PIPE_VIDEO_CFG VideoConfig;
			PIPE_VP_SURFACE_INPUT Input;
			
			PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)hVideoObj);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			pPlayerSubsystem->bRadioStream = FALSE;

			PmStatus = hDemuxObj->set_video_pid(hDemuxObj, psStreamInfo.m_nPid);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			cnxt_kal_memset(&VideoConfig, 0, sizeof(VideoConfig));
			VideoConfig.Format               = pPlayerSubsystem->VideoFormat;
			VideoConfig.Definition           = PIPE_VIDEO_HD;
			VideoConfig.DecodingType         = PIPE_VIDEO_MOTION_VIDEO;
			VideoConfig.bHwAccelerate        = TRUE;
			VideoConfig.bPrimaryDecoder     = TRUE;
			VideoConfig.uUserDataEnableMap = 0;
			/* config video */
			PmStatus = hVideoObj->config(hVideoObj, &VideoConfig);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_PIPELINE;
			Input.pPipe = hPipeObj;
#ifdef DUAL_VIDEO_SURFACE
			PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_PRIMARY_SURFACE, //pPlayerSubsystem->VideoIndx,
                                                    pPlayerSubsystem->DemuxIndx);
#else
			PmStatus = cs_pipe_vpm_attach(Input, cs_tm_get_surface_indx(pPlayerSubsystem->VideoIndx), //pPlayerSubsystem->VideoIndx,
                                                    pPlayerSubsystem->DemuxIndx);
#endif
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			/* start video */
			PmStatus = hVideoObj->decoder_ctrl(hVideoObj, TRUE);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
		}
	    PmStatus = hDemuxObj->demux_play_ctrl(hDemuxObj, TRUE);
        if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
			eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
			break;
		}
	}while(0);
	return retcode;
}
extern bool bIsEncDataRequired(PIPE_AUDIO_FORMAT AudioFormat);


bool cs_tm_restart_audio(CS_TM_Player_SubSystem *pPlayerSubsystem, CSUDIStreamInfo_S psStreamInfo)
{
	bool retcode = TRUE;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_DEMUX_OBJECT        *hDemuxObj=NULL;
	PIPE_AUDIO_OBJECT        *hAudioObj=NULL;
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;

	hPipeObj = pPlayerSubsystem->pPipeObj;
	hDemuxObj = pPlayerSubsystem->pDemuxObj;
	hAudioObj = pPlayerSubsystem->pAudioObj;
	do
	{
		if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
		{	          		      
			/* stop audio */
			PmStatus = hAudioObj->decoder_ctrl(hAudioObj, FALSE);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			PmStatus = cs_pipe_apm_detach(hPipeObj);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
			}
           
		}
		if((hAudioObj != NULL)&&(IS_VALID_PID(psStreamInfo.m_nPid)))
		{
			PIPE_AUDIO_CFG AudioConfig;
            bool bDecPassthrough = FALSE;
            PIPE_AUDIO_FORMAT AudFormat = get_tm_audio_format(pPlayerSubsystem->StreamType);
            
            bDecPassthrough = bIsEncDataRequired(AudFormat);
           
			pPlayerSubsystem->bVideoOnlyStream = FALSE;
			
			PmStatus = hDemuxObj->set_audio_pid(hDemuxObj, psStreamInfo.m_nPid);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			
			cnxt_kal_memset(&AudioConfig, 0, sizeof(AudioConfig));
			AudioConfig.Format = AudFormat;
			AudioConfig.Mode = bDecPassthrough ? PIPE_AUDIO_MODE_DECODE_AND_PASSTHROUGH:PIPE_AUDIO_MODE_DECODE;
			AudioConfig.DualMono = PIPE_AUDIO_DUAL_MONO_NORMAL;

			if (AudioConfig.Format == PIPE_AUDIO_FMT_AAC)
			{
				AudioConfig.FormatCfg.AacCfg.bEnableDownMix = TRUE;
				AudioConfig.FormatCfg.AacCfg.DownmixType = PIPE_AUDIO_DOWNMIX_LORO;
				AudioConfig.FormatCfg.AacCfg.uScaleFactor = 1;
			}
			/* config audio */
			PmStatus = hAudioObj->config(hAudioObj, &AudioConfig);
			if(PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
					
		    PmStatus = cs_pipe_apm_attach(hPipeObj, PIPE_AUDIO_PRESENTATION_STREAM_INPUT, 
                                        pPlayerSubsystem->AudioIndx, bDecPassthrough);
			if(PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			
			/* start audio */
			PmStatus = hAudioObj->decoder_ctrl(hAudioObj, TRUE);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}

			               
			/*<!-- 2011-1-30 15:49:58 Note: begin, add by guoyuqi,set mute or unmute --!>*/
            if(gTmPipeObject.TridCoshipSetup[0].AudioMute == 1)
            {
                PmStatus = hAudioObj->mute(hAudioObj, TRUE);
			    if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}			
            }
            else
            {
                
                 PmStatus = hAudioObj->mute(hAudioObj, FALSE);
			     if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}						
            }
			/*<!-- 2011-1-30 15:49:58 Note: end --!>*/		
		}
	}while(0);
	return retcode;
}


/**
@brief UDI Player是否支持播放传入的URL指定的媒体文件.
 
@param[in] pcURL 需要探测的媒体文件URL.
@return 支持返回CSUDI_SUCCESS, 不支持返回CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED.
@note 参考udi2_player.h文件中对第三方媒体播放器流程相关的描述.
@note 该接口主要针对芯片能够直接支持本地媒体播放，如不支持请直接返回CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/	
CSUDI_Error_Code CSUDIPLAYERProbe(const char * pcURL)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief  注册指定事件类型的回调函数

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] fnPlayerCallback 回调函数指针，详见CSUDIPLAYERCallback_F定义
@param[in] eEvent 回调函数类型，详见CSUDIPlayerEventType_E
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 至少支持注册的回调函数个数为32个，最大不限
@note 不允许注册EM_UDIPLAYER_MAXEVENTTYPE类型的回调
@note 回调函数指针+回调函数类型+用户数据唯一标识一个回调,当两次注册它们完全一样时，第二次将返回CSUDIPLAYER_ERROR_CALLBACK_EXIST

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERAddPlayerCallback(CSUDI_HANDLE hPlayer,CSUDIPLAYERCallback_F fnPlayerCallback,CSUDIPlayerEventType_E eEvent,void * pvUserData )
{
	int uPlayerIndx, FreeIndx=0, indx;
	CNXT_STATUS             Retcode;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERAddPlayerCallback 0x%x eEvent %d\n", hPlayer, eEvent);
	IS_VALID_PLAYER_HNDL(hPlayer);
	if((eEvent >= EM_UDIPLAYER_MAXEVENTTYPE)||(fnPlayerCallback == NULL))
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"pvUserData 0x%x fnCallbk 0x%x\n", pvUserData, fnPlayerCallback);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	Retcode = cnxt_kal_sem_get(gPlayerCallbackSem, CNXT_KAL_WAIT_FOREVER);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	do
	{
		uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
		pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];

		for(indx=0;indx<MAX_CALLBK_FUNCS;indx++)
		{
			if((pPlayerSubsystem->PlayCallbackData[indx].bInuse == TRUE)&&
					(pPlayerSubsystem->PlayCallbackData[indx].eEvent == eEvent)&&
					(pPlayerSubsystem->PlayCallbackData[indx].Func == fnPlayerCallback)&&
					(pPlayerSubsystem->PlayCallbackData[indx].pvUserData == pvUserData))
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ALREADY ADDED event %d fnCallbk 0x%x\n", eEvent, fnPlayerCallback);
				eErrCode = CSUDIPLAYER_ERROR_CALLBACK_EXIST;
				break;
			}
		}
		if(eErrCode == CSUDI_SUCCESS)
		{
			/*App can add upto a max of 32 call back functions to a Player*/
			FreeIndx = cs_cnxt_get_free_callbackindx(pPlayerSubsystem);
			if(FreeIndx == MAX_CALLBK_FUNCS)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"FreeIndx %d\n", FreeIndx);
				eErrCode = CSUDIPLAYER_ERROR_NO_MEMORY;
				break;
			}
			pPlayerSubsystem->PlayCallbackData[FreeIndx].eEvent = eEvent;
			pPlayerSubsystem->PlayCallbackData[FreeIndx].Func = fnPlayerCallback;
			pPlayerSubsystem->PlayCallbackData[FreeIndx].pvUserData = pvUserData;
			pPlayerSubsystem->PlayCallbackData[FreeIndx].bInuse = TRUE;
		}
	}while(0);
	Retcode = cnxt_kal_sem_put(gPlayerCallbackSem);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s(%d) 0x%x Func 0x%x event %d Data 0x%x\n", __FUNCTION__, FreeIndx, hPlayer,fnPlayerCallback, eEvent,pvUserData);
	return eErrCode;
}


 /**
@brief 删除指定事件类型的回调函数
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] fnPlayerCallback 回调函数指针，详见CSUDIPLAYERCallback_F定义
@param[in] eEvent 回调函数类型，详见CSUDIPlayerEventType_E
@param[in] pvUserData 同CSUDIPLAYERAddPLAYERCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
@note 允许在回调函数中删除回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERDelPlayerCallback(CSUDI_HANDLE hPlayer,CSUDIPLAYERCallback_F fnPlayerCallback,CSUDIPlayerEventType_E eEvent, void * pvUserData )
{
	int uPlayerIndx, FreeIndx=0, i;
	CNXT_STATUS             Retcode;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERDelPlayerCallback 0x%x event %d \n", hPlayer, eEvent);
	IS_VALID_PLAYER_HNDL(hPlayer);
	if((eEvent >= EM_UDIPLAYER_MAXEVENTTYPE)||(fnPlayerCallback == NULL))
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"pvUserData 0x%x fnCallbk 0x%x\n", pvUserData, fnPlayerCallback);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	Retcode = cnxt_kal_sem_get(gPlayerCallbackSem, CNXT_KAL_WAIT_FOREVER);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	do
	{
		uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
		pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
		/*App can del upto a max of 32 call back functions to a Player*/
		for(i=0;i<MAX_CALLBK_FUNCS;i++)
		{
			if((pPlayerSubsystem->PlayCallbackData[i].Func == fnPlayerCallback)&&
					(pPlayerSubsystem->PlayCallbackData[i].eEvent == eEvent)&&
					(pPlayerSubsystem->PlayCallbackData[i].pvUserData == pvUserData))
			{
				break;
			}
		}
		if(i == MAX_CALLBK_FUNCS)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"NO Function found \n");
			eErrCode = CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST;
			break;
		}
		pPlayerSubsystem->PlayCallbackData[i].eEvent = EM_UDIPLAYER_MAXEVENTTYPE; /*Invalid event*/
		pPlayerSubsystem->PlayCallbackData[i].Func = NULL;
		pPlayerSubsystem->PlayCallbackData[i].pvUserData = NULL;
		pPlayerSubsystem->PlayCallbackData[i].bInuse = FALSE;
	}while(0);
	Retcode = cnxt_kal_sem_put(gPlayerCallbackSem);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s(%d) 0x%x Func 0x%x event %d Data 0x%x\n",__FUNCTION__, FreeIndx, hPlayer,fnPlayerCallback, eEvent,pvUserData);
	return eErrCode;
}

 /**
@brief  注册文件播放事件类型的回调函数

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] fnPlayerCallback 回调函数指针，详见CSUDIFilePLAYERCallback_F定义
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 至少支持注册的回调函数个数为32个，最大不限
@note 回调函数指针+用户数据唯一标识一个回调,当两次注册它们完全一样时，第二次将返回CSUDIPLAYER_ERROR_CALLBACK_EXIST
@note 只允许ePlayerType为EM_UDIPLAYER_FILE的hPlayer时，才能注册此回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERAddFilePlayerCallback(CSUDI_HANDLE hPlayer, CSUDIFilePLAYERCallback_F fnPlayerCallback, void * pvUserData )
{
	 UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
 
	 CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	 UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	  
	 return Retcode;
}

 /**
@brief 删除文件播放事件类型的回调函数

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] fnPlayerCallback 回调函数指针，详见CSUDIFilePLAYERCallback_F定义
@param[in] pvUserData 同CSUDIPLAYERAddFilePlayerCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
@note 允许在回调函数中删除回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERDelFilePlayerCallback(CSUDI_HANDLE hPlayer, CSUDIFilePLAYERCallback_F fnPlayerCallback, void * pvUserData )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	  
	return Retcode;
}


/**
@brief  获取播放的文件信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[out] pstFileInfo 获取的文件信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetFileInfo(CSUDI_HANDLE hPlayer, CSUDIPlayerFileInfo_S * pstFileInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief  设置播放的文件信息，被设置的文件按此属性播放

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] pstFileStreamId 需要设置的文件播放信息
@note 通过CSUDIPLAYERGetFilePlayInfo 获取需要设置的信息的索引
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetFilePlayStream(CSUDI_HANDLE hPlayer, CSUDIPlayerFileStreamId_S * pstFileStreamId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 创建一个播放器

@param[in] psPlayerChnl   播放器通道的数据结构指针，请参见CSUDIPlayerChnl_S。
@param[in] ePlayerType   播放器播放类型，请参见CSUDIPLAYERType_E。
@param[out] phPlayer  创建成功后生成的播放器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note psPlayerChnl 中的成员设备资源索引可以为CSUDI_INVALID_INDEX，CSUDI_INVALID_INDEX代表该设备资源不存在或者播放器不需要此设备实现播放操作。
@note 创建Player时应杜绝传入冗余设备，如播放I帧时不得传入Audio
@note 如果播放器的类型是直播，那一定要指定demux ID,且音视频不能同时为-1，否则返回CSUDIPLAYER_ERROR_DEVICE_DISORDER
@note 如果此时该视频解码器正在进行I帧解码(由于CSUDIOSGDecodeImage接口的使用)，并且本函数需要马上用到该解码器，则该接口应进行等待直到I帧解码完成为止
@note 如果以直播形式打开Player且所用Demux未与任何Tuner连接,仍能创建成功，但将无法播放出任何数据
@note 如果以Inject等不需要Tuner的形式打开Player且所用Demux已经连接了Tuner，则返回CSUDIPLAYER_ERROR_DEVICE_BUSY
@see CSUDIOSGDecodeImage

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer)
{
	int  FreeIndx;
	bool     Retcode;
	CNXT_STATUS RetStatus;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_DEMUX_OBJECT        *pDmxObject=NULL;
	PIPE_AUDIO_OBJECT        *pAudObject=NULL;
	PIPE_VIDEO_OBJECT        *pVidObject=NULL;
	PIPE_PIPELINE_CFG PipeCfg;
	PIPE_DEMUX_CFG demuxConfig;
	int uDmxIndx = 0,uAudIndx = 0,uVidIndx = 0;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYEROpen 0x%x Type %d\n", phPlayer, ePlayerType);
	IS_VALID_PLAYER_HNDL(phPlayer);
	if((psPlayerChnl == NULL)||(ePlayerType>=EM_UDIPLAYER_MAXPLAYERTYPE))
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Error2 CSUDIPLAYER_ERROR_BAD_PARAMETER\n");
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	RetStatus = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(RetStatus != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	do
	{
		Retcode = bIsPlayeropen((u_int32)*phPlayer);
		if(Retcode == TRUE)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Error3 CSUDIPLAYER_ERROR_DEVICE_BUSY\n");
			eErrCode=CSUDIPLAYER_ERROR_DEVICE_BUSY;
			break;
		}
		FreeIndx = cs_cnxt_get_free_player();
		if(FreeIndx == MAX_PLAYER_HANDLES)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Error4 CSUDIPLAYER_ERROR_NO_MEMORY\n");
			eErrCode=CSUDIPLAYER_ERROR_NO_MEMORY;
			break;
		}
		if(ePlayerType==EM_UDIPLAYER_LIVE)
		{
			if(((psPlayerChnl->m_nDemux < 0)||(psPlayerChnl->m_nDemux>=MAX_NUM_TSI_DEMUX))||
					(psPlayerChnl->m_nAudioDecoder < CSUDI_INVALID_INDEX)||(psPlayerChnl->m_nAudioDecoder >=MAX_AUDIO_OBJECTS)||
					(psPlayerChnl->m_nVideoDecoder < CSUDI_INVALID_INDEX)||(psPlayerChnl->m_nVideoDecoder >=MAX_VIDEO_OBJECTS))
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Error5 CSUDIPLAYER_ERROR_INVALID_DEVICE_ID\n");
				/*If videdecoder is -1, it means audio only stream, if audio decoder is -1, it means video only stream*/
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"DMX %d Audio %d Video %d\n", psPlayerChnl->m_nDemux, psPlayerChnl->m_nAudioDecoder, psPlayerChnl->m_nVideoDecoder);
				eErrCode=CSUDIPLAYER_ERROR_INVALID_DEVICE_ID;
				break;
			}
			if((psPlayerChnl->m_nVideoDecoder == CSUDI_INVALID_INDEX) &&
					(psPlayerChnl->m_nAudioDecoder == CSUDI_INVALID_INDEX))
			{
				/*For live play both audio and video cannot be -1*/
				eErrCode= CSUDIPLAYER_ERROR_INVALID_DEVICE_ID;
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Error5 CSUDIPLAYER_ERROR_INVALID_DEVICE_ID\n");
				break;
			}
		}
		
		/*Check Audio Decoder device is in USE*/
		if((psPlayerChnl->m_nAudioDecoder > CSUDI_INVALID_INDEX)&&
				(bAudioDecoderInUse(psPlayerChnl->m_nAudioDecoder)))
		{
			eErrCode=CSUDIPLAYER_ERROR_DEVICE_BUSY;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Err AUDIO ERROR_DEVICE_BUSY\n");
			break;
		}
		/*Check Video Decoder device is in USE*/
		if((psPlayerChnl->m_nVideoDecoder > CSUDI_INVALID_INDEX)&&
				(bVideoDecoderInUse(psPlayerChnl->m_nVideoDecoder)))
		{
			eErrCode=CSUDIPLAYER_ERROR_DEVICE_BUSY;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Err VIDEO ERROR_DEVICE_BUSY\n");
			break;
		}
		/*From Inject document it is not necessary that all the three should exist? VERIIFY ONCE*/
		pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[FreeIndx];
		
		uAudIndx = pPlayerSubsystem->AudioIndx = psPlayerChnl->m_nAudioDecoder;
		uVidIndx = pPlayerSubsystem->VideoIndx = psPlayerChnl->m_nVideoDecoder;
		uDmxIndx = pPlayerSubsystem->DemuxIndx = psPlayerChnl->m_nDemux;
		pPlayerSubsystem->PlayerType = ePlayerType;
		pPlayerSubsystem->bPlayerInuse = TRUE;
		pPlayerSubsystem->ePlayerState = E_PLAYER_OPEN;
		pPlayerSubsystem->bStreamModified = FALSE;		
        pPlayerSubsystem->CurVideoFormat = 0x0ff;
        pPlayerSubsystem->CurStreamType = 0x0ffff;
		pPlayerSubsystem->bTrickMode = FALSE;
		
        if(uDmxIndx>=0)
        {
         	pDmxObject = gTmPipeObject.hDemuxObj[uDmxIndx];
        }
        if(uAudIndx==0) /*uAudIndx=1 is reserved for PCM play*/
        {
              pAudObject = gTmPipeObject.hAudioObj[uAudIndx];
        }
        if(uVidIndx>=0)
        {
              pVidObject = gTmPipeObject.hVideoObj[uVidIndx];
        }
        if(ePlayerType==EM_UDIPLAYER_LIVE)  /*This is live*/		   
        {   
       		/*Here we need to add devices*/
 		    /*Based on demux index, use the respective pipe*/	
 
             if(uVidIndx > CSUDI_INVALID_INDEX)
             {
 			    pPlayerSubsystem->bVideoOnlyStream = TRUE;
             }
 			pPlayerSubsystem->bRadioStream = TRUE;
 			
 			hPipeObj = gTmPipeObject.hPipeObj[uDmxIndx];
            
 			/* config demux */
 			cnxt_kal_memset(&demuxConfig, 0, sizeof(demuxConfig));
 			demuxConfig.Type = PIPE_DEMUX_TS;
 			demuxConfig.DescramblerType   = PIPE_DEMUX_DESCRAMBLE_DVB;
 
 			PmStatus = pDmxObject->config(pDmxObject, &demuxConfig);
 			if (PmStatus != PIPE_STATUS_OK)
 			{
    				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
 				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
 				break;
 			}
 			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"this is for TS \n");
 	    }
        if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
	     /*(pPlayerSubsystem->AudioIndx >= 0)&&
	                  (pPlayerSubsystem->VideoIndx >= 0)&&*/  /*Inject can happen for audio only files*/
	     (pPlayerSubsystem->DemuxIndx >= 0))
        {
			pPlayerSubsystem->bVideoOnlyStream = TRUE;
			pPlayerSubsystem->bRadioStream = TRUE;    			
			hPipeObj = gTmPipeObject.hPipeObj[uDmxIndx];
        }

	    if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT) &&
            ((pPlayerSubsystem->AudioIndx == 0)||(pPlayerSubsystem->VideoIndx >= 0)) && /*This is not PCM inject*/
	        (pPlayerSubsystem->DemuxIndx== -1)&&
	        (bInjectIframe!=TRUE)) /*This is not I-frame inject... This is required to distinguish b/n PES video and I-frame*/
	    {
			pPlayerSubsystem->bVideoOnlyStream = TRUE;
			pPlayerSubsystem->bRadioStream = TRUE;
		
		    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"this is for PES/ES decoder. set playState to E_PLAYER_READY\n");
            
			/*Create ES inject PIPE*/
			if(uVidIndx >= 0)
			{
				/*For multiple ES case. i.e. 1 PES video and 1 still video...DOES THIS HAPPEN????*/
				hPipeObj = gTmPipeObject.hPipeObj[ES_VIDEO_OBJECT+uVidIndx];
				pDmxObject = gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+uVidIndx]; 
				pVidObject = gTmPipeObject.hVideoObj[uVidIndx];
			}
			if(uAudIndx >= 0)
			{
				hPipeObj = gTmPipeObject.hPipeObj[ES_AUDIO_OBJECT];
				pDmxObject = gTmPipeObject.hDemuxObj[ES_AUDIO_OBJECT];
				pAudObject= gTmPipeObject.hAudioObj[uAudIndx];
			}
			/* config pipe */
			PipeCfg.Type = PIPE_PIPELINE_NONE_TS_TYPE;
			PipeCfg.uStcIndex = 0;
			PipeCfg.bPipPipe = FALSE;
			if(pVidObject!= NULL)
			{
				PipeCfg.SyncMaster = PIPE_SYNC_VIDEO_MASTER;
			}
			else if(pAudObject!= NULL)
			{
				PipeCfg.SyncMaster = PIPE_SYNC_AUDIO_MASTER;
			}
			PmStatus = hPipeObj->config(hPipeObj, &PipeCfg);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d %d \n", __FUNCTION__, __LINE__, PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
		
			pPlayerSubsystem->uSubTitlePID = -1;
			pPlayerSubsystem->uTeletextPID = -1;
    	}
        else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
                (pPlayerSubsystem->VideoIndx >= 0)&& /*This is not PCM inject*/
		        (pPlayerSubsystem->DemuxIndx== -1)&&
		        (bInjectIframe==TRUE))
        {
            pPlayerSubsystem->bIframeInject = TRUE;/*Store this player is for I-frame*/
            bInjectIframe = FALSE; /*Reset I-frame flag*/
            CS_Open_Iframe_decode(pPlayerSubsystem, pPlayerSubsystem->VideoIndx);
    		pVidObject = pPlayerSubsystem->pVideoObj;
    		pDmxObject = pPlayerSubsystem->pDemuxObj;
    		hPipeObj = pPlayerSubsystem->pPipeObj;
        }
        else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
                (pPlayerSubsystem->AudioIndx > 0)&& /*This is FOR PCM inject*/
		        (pPlayerSubsystem->DemuxIndx== -1))
        {
		    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Nothing to do for PCM player. set playState to E_PLAYER_READY\n");
        }
		
		pPlayerSubsystem->ePlayerState = E_PLAYER_OPEN;
		pPlayerSubsystem->pAudioObj = pAudObject;
		pPlayerSubsystem->pVideoObj = pVidObject;
		pPlayerSubsystem->pDemuxObj = pDmxObject;
		pPlayerSubsystem->pPipeObj = hPipeObj;
		gUDIPlayerHndl[FreeIndx].uPlayerindx = FreeIndx;
		gUDIPlayerHndl[FreeIndx].bPlayerOpen = TRUE;
		gUDIPlayerHndl[FreeIndx].uStreamCount = 0;
		
		*phPlayer = &gUDIPlayerHndl[FreeIndx];
	}while(0);
	RetStatus = cnxt_kal_sem_put(gPlayerSem);
	if(RetStatus != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: CSUDIPLAYEROpen 0x%x Audio %d Video %d Demux %d PlayerType %d Errcode 0x%x \n", *phPlayer, psPlayerChnl->m_nAudioDecoder,
	psPlayerChnl->m_nVideoDecoder,psPlayerChnl->m_nDemux,ePlayerType, eErrCode);
	return eErrCode;
}


/**
@brief 销毁一个播放器

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 销毁一个播放器，与CSUDIPLAYEROpen成对使用。
@note 一个播放器被销毁后，其相关资源需一并自行销毁(如该播放器关联的回调函数)。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERClose(CSUDI_HANDLE hPlayer)
{
	int uPlayerIndx, nDmxIndex;
	CNXT_STATUS             RetStatus;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_DEMUX_OBJECT        *pDmxObject=NULL;
	PIPE_AUDIO_OBJECT        *pAudObject=NULL;
	PIPE_VIDEO_OBJECT        *pVidObject=NULL;
    PIPE_STATUS PmStatus = PIPE_STATUS_OK;
    CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;
	PIPE_VP_SURFACE_INPUT Input;

	
	IS_VALID_PLAYER_HNDL(hPlayer);
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	CSUDIPLAYERType_E ePlayerType;
	//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERClose 0x%x indx %d\n", hPlayer, uPlayerIndx);
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
	if(pPlayerSubsystem->bPlayerInuse == FALSE)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERClose...PlayerOpen Not called\n");
		return CSUDIPLAYER_ERROR_INVALID_HANDLE;
	}
	
	RetStatus = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(RetStatus != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	do
	{
		nDmxIndex = pPlayerSubsystem->DemuxIndx; //gPlayerConfig.Demux;
		ePlayerType = pPlayerSubsystem->PlayerType;
		pDmxObject = pPlayerSubsystem->pDemuxObj;
		pAudObject = pPlayerSubsystem->pAudioObj;
		pVidObject = pPlayerSubsystem->pVideoObj;
		hPipeObj = pPlayerSubsystem->pPipeObj;
		/*We need to first check demux and sort out whether it is a radiostream or videoonly stream	
			if(nDmxIndex != -1)
			pDMXHandle = &gDemuxConfig.hDemuxHandle[nDmxIndex];*/
		
		if((ePlayerType == EM_UDIPLAYER_INJECT) && 
				(pPlayerSubsystem->StreamType==EM_UDI_AUD_STREAM_LPCM
					||pPlayerSubsystem->StreamType==EM_UDI_AUD_STREAM_PCM))
		{
			;/*do nothing for PCM inject on player close*/
		}
		else if((ePlayerType==EM_UDIPLAYER_LIVE)||
				((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
					(pPlayerSubsystem->bIframeInject == FALSE)))
		{
			/*Here we need to delete devices*/
			/*Based on demux index, use the respective pipe*/	
			
			if((pPlayerSubsystem->ePlayerState == E_PLAYER_START)||
					(pPlayerSubsystem->ePlayerState == E_PLAYER_PAUSE))
			{
				RetStatus = cnxt_kal_sem_put(gPlayerSem);
				if(RetStatus != CNXT_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
					return CSUDIPLAYER_ERROR_BAD_PARAMETER;
				}
				CSUDIPLAYERStop(hPlayer);
				RetStatus = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
				if(RetStatus != CNXT_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
					return CSUDIPLAYER_ERROR_BAD_PARAMETER;
				}
			}
#ifndef DUAL_VIDEO_SURFACE            
    	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"@ %s %d \n", __FUNCTION__, __LINE__);
            
            if(pVidObject != NULL)
            {
                Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_NONE;
    			Input.pPipe = hPipeObj;
                Input.hImageHandle = NULL; //2011-01-18 TerenceZhang Added
    		    PmStatus = cs_pipe_vpm_attach(Input, pPlayerSubsystem->VideoIndx, 0);
    	        if (PmStatus != PIPE_STATUS_OK)
    	        {
    	            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Err: %s %d \n", __FUNCTION__, __LINE__);
    			    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
    	       	    break;
    	        }
            }
#endif
		//2011-01-19 BOB begin:
			if((pVidObject != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
			{

				
                if( hPipeObj->pVideoDecoder!= NULL)
                {
                    PmStatus = hPipeObj->delete_device(hPipeObj, (PIPE_OBJ_HANDLE)pVidObject);
				    if (PmStatus != PIPE_STATUS_OK)
				    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR; 				
					    break;
				    }
                    else
                    {
                        printf("------delete video device success---\n");
                    }
                }
			}
			if((pAudObject != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
			{
                if( hPipeObj->pAudioDecoder != NULL)
                {
                    PmStatus = hPipeObj->delete_device(hPipeObj,(PIPE_OBJ_HANDLE)pAudObject);
				    if (PmStatus != PIPE_STATUS_OK)
				    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__,PmStatus);
					    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR; 				
					    break;
				    }
                    else
                    {
                        printf("------delete audio success----\n");
                    }
                }
			}
		//2011-01-19 BOB end
		}
        else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
					(pPlayerSubsystem->bIframeInject == TRUE))
        {
	        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"@ %s %d \n", __FUNCTION__, __LINE__);
            if((pPlayerSubsystem->ePlayerState == E_PLAYER_START)||
					(pPlayerSubsystem->ePlayerState == E_PLAYER_PAUSE))
			{
				RetStatus = cnxt_kal_sem_put(gPlayerSem);
				if(RetStatus != CNXT_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
					return CSUDIPLAYER_ERROR_BAD_PARAMETER;
				}
				CSUDIPLAYERStop(hPlayer);
				RetStatus = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
				if(RetStatus != CNXT_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
					return CSUDIPLAYER_ERROR_BAD_PARAMETER;
				}
			}            
            CS_Close_Iframe_decode(pPlayerSubsystem);
        }
		
		
	}while(0);
    pPlayerSubsystem->bPlayerInuse = FALSE;
	cnxt_kal_memset(pPlayerSubsystem, 0, sizeof(CS_TM_Player_SubSystem));
	gUDIPlayerHndl[uPlayerIndx].bPlayerOpen = FALSE;
	pPlayerSubsystem->ePlayerState = E_PLAYER_CLOSE;
	RetStatus = cnxt_kal_sem_put(gPlayerSem);
	if(RetStatus != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_PUT FAILED %d \n", RetStatus);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit: CSUDIPLAYERClose...\n");
	return CSUDI_SUCCESS;
}

/**
@brief 用户设置Audio/video/pcr/sub/tt节目信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] psStreamInfo 输入节目信息，具体结构参看CSUDIStreamInfo_S 数据结构。
@param[in] nStreamCnt 要设置的节目信息个数
@param[in] psPlaybackParam 文件回放/时移播放参数信息；该参数可以为CSUDI_NULL,表示进行直播
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 试图设置两个音视频信息时返回CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER,即不能同时设置两个音视频PID 
@note 供用户设置Audio/video/pcr/sub/tt节目信息.具体请参见CSUDIStreamInfo_S的定义 
@note 试图设置解码器不支持的音视频类型时返回CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER
@note 该接口设置的音频格式与输出模式无关，如当前所有Aout设备均为DECODER模式，但当前Player中的Audio设备
仅支持AC3音频的BYPASS模式，使用本接口设置一个AC3音频时，仍返回成功，但实际输出会没有声音。
@note 播放参数中不设置不需要的属性PID，如一个有AV的Player来播放广播时不传入视频PID
@note 该接口及相关接口无需进行PID标准级的判断，即0,0x10等被其他PSI占用的PID仍为合法的PID，实际若上述PID上无对应音视频数据时，无法播放成功即可。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam)
{
	int32       nVidIndex=0,nDmxIndex=0, nAudIndex=0;
	CNXT_STATUS Retcode;
	int uPlayerIndx, count;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: %s 0x%x Streams %d\n", __FUNCTION__, hPlayer, nStreamCnt);
	//cnxt_kal_thread_time_sleep(100);
	
	IS_VALID_PLAYER_HNDL(hPlayer);
	
	if((psStreamInfo==NULL)||(nStreamCnt<=0)) //(psPlaybackParam==NULL)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL," At line :%d  \n",__LINE__);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
	if((pPlayerSubsystem->ePlayerState == E_PLAYER_START)||
			(pPlayerSubsystem->ePlayerState == E_PLAYER_PAUSE))
	{
		return CSUDIPLAYER_ERROR_DEVICE_BUSY;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL," In Funtion :%s  \n",__FUNCTION__);

	nDmxIndex = pPlayerSubsystem->DemuxIndx ;
	nVidIndex = pPlayerSubsystem->VideoIndx;
	nAudIndex = pPlayerSubsystem->AudioIndx;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"CSUDIPLAYERSetStream streamcnt %d nDmxIndex %d nVidIndx %d nAudIndx %d \n", 
	nStreamCnt, nDmxIndex, nVidIndex, nAudIndex);
	
	/*This is to check that any two pids in  streaminfo are same*/    
	if (!CS_CheckStreamParam(nStreamCnt,(CSUDIStreamInfo_S *)psStreamInfo))
	{
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	
	Retcode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	do
	{
		pPlayerSubsystem->uVidPID = 0x1FFF;
		pPlayerSubsystem->uAudPID = 0x1FFF;
		pPlayerSubsystem->uPcrPID = 0x1FFF;
		pPlayerSubsystem->uSubTitlePID = 0x1FFF;
		pPlayerSubsystem->uTeletextPID = 0x1FFF;
		for(count =0; count < nStreamCnt; count++)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"content type %d PID 0x%x\n", psStreamInfo[count].m_eContentType, psStreamInfo[count].m_nPid);
			switch(psStreamInfo[count].m_eContentType)
			{
			case EM_UDI_CONTENT_VIDEO:
				if((nVidIndex < 0)||(nVidIndex > MAX_VIDEO_OBJECTS) ||
						(pPlayerSubsystem->uVidPID == psStreamInfo[count].m_nPid) || /*PID repeated in same stream*/
						(pPlayerSubsystem->uVidPID != 0x1FFF)) /*previous content is also Video*/
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"streamtype error1\n");
						eErrCode = CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER;
						break;
					}
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"video type %d \n",
                                    psStreamInfo[count].m_uStreamType.m_eVideoType);
					if(((IS_VALID_PID(psStreamInfo[count].m_nPid) == FALSE)||
						((psStreamInfo[count].m_nPid==MIN_PID)))&&(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE))
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Invalid PID 0x%x %d \n", psStreamInfo->m_nPid, __LINE__);
						eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
						break;
					}
					pPlayerSubsystem->uVidPID = psStreamInfo[count].m_nPid;
					if((psStreamInfo[count].m_uStreamType.m_eVideoType == EM_UDI_VID_STREAM_MPEG2)||
						(psStreamInfo[count].m_uStreamType.m_eVideoType == EM_UDI_VID_STREAM_MPEG1))
				{
					pPlayerSubsystem->VideoFormat = CNXT_VIDEO_FORMAT_MPEG2;
				}
				else if(psStreamInfo[count].m_uStreamType.m_eVideoType == EM_UDI_VID_STREAM_H264)
				{
					pPlayerSubsystem->VideoFormat = CNXT_VIDEO_FORMAT_MPEG4;
				}
				else
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Vid feature not supported\n");
					eErrCode = CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
					break;
				}
				break;
			case EM_UDI_CONTENT_AUDIO:
				if((nAudIndex < 0)||(nAudIndex >= MAX_AUDIO_OBJECTS) ||
						(pPlayerSubsystem->uAudPID == psStreamInfo[count].m_nPid) ||/*PID repeated in same stream*/
						(pPlayerSubsystem->uAudPID != 0x1FFF)) 			      /*previous content is also Video*/
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"streamtype error2\n");
					eErrCode = CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER;
					break;
				}
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"audio type %d \n",
                                    psStreamInfo[count].m_uStreamType.m_eAudioType);
				if(Is_audio_fmt_supported(psStreamInfo[count].m_uStreamType.m_eAudioType)==FALSE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"feature not supported1\n");
					eErrCode = CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
					break;
				}
				if((IS_VALID_PID(psStreamInfo[count].m_nPid) == FALSE)
						||((psStreamInfo[count].m_nPid==MIN_PID)&&(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)))
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Invalid PID 0x%x\n", psStreamInfo->m_nPid);
					eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
					break;
				}
				pPlayerSubsystem->uAudPID = psStreamInfo[count].m_nPid;
				pPlayerSubsystem->StreamType = psStreamInfo[count].m_uStreamType.m_eAudioType;
				break;
			case EM_UDI_CONTENT_SUBTITLE:
				if(IS_VALID_PID(psStreamInfo[count].m_nPid) == FALSE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Invalid PID 0x%x\n", psStreamInfo[count].m_nPid);
					eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
					break;
				}
				pPlayerSubsystem->uSubTitlePID = psStreamInfo[count].m_nPid;
				break;
			case EM_UDI_CONTENT_TELTEXT:
				if(IS_VALID_PID(psStreamInfo[count].m_nPid) == FALSE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Invalid PID 0x%x\n", psStreamInfo[count].m_nPid);
					eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
					break;
				}
				pPlayerSubsystem->uTeletextPID = psStreamInfo[count].m_nPid;
				break;
			case EM_UDI_CONTENT_PCR :
				pPlayerSubsystem->uPcrPID = psStreamInfo[count].m_nPid;
				break;
			default:
				eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
				break;
			}
		}
		if(eErrCode != CSUDI_SUCCESS)
		{
			break;
		}
		/*if(gVideoConfig.hVideoHandle[0].VideoState != VidPlaying)
		{
		gVideoConfig.hVideoHandle[0].VideoState = VidReady;
		}*/
		pPlayerSubsystem->nStreamCnt = nStreamCnt;
		for(count =0; count < nStreamCnt; count++)
		{
			cnxt_kal_memcpy(&pPlayerSubsystem->sStreamInfo[count], &psStreamInfo[count], sizeof(CSUDIStreamInfo_S));
		}
		cnxt_kal_memset(&pPlayerSubsystem->sPlaybackParams, 0, sizeof(CSUDIPlaybackParam_S));
		if(psPlaybackParam != NULL)
		{
			cnxt_kal_memcpy(&pPlayerSubsystem->sPlaybackParams, psPlaybackParam, sizeof(CSUDIPlaybackParam_S));
		}
		if(pPlayerSubsystem->ePlayerState!=E_PLAYER_START)
		{
			/*If player is already in playing then don't set this state, as we check for START state in PLAYERStart*/
			pPlayerSubsystem->ePlayerState = E_PLAYER_READY;
		}
	}while(0);
#if 0
	if(1)//pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)
	{
		int vidpid,audpid,pcrpid;
		printf("\n Vid pid:");
		scanf("%d", &vidpid);
		pPlayerSubsystem->uVidPID = vidpid;
		printf("\n aud pid:");
		scanf("%d", &audpid);
		pPlayerSubsystem->uAudPID =audpid;
		printf("\n stream type:");
		scanf("%d", &pcrpid);
		pPlayerSubsystem->uPcrPID = pcrpid;
		printf("\n audio stream type:");
		scanf("%d", &pcrpid);
		pPlayerSubsystem->StreamType = pcrpid;//EM_UDI_AUD_STREAM_AC3; //_PLUS;
		printf("\n video stream type:");
		scanf("%d", &pcrpid);
		pPlayerSubsystem->VideoFormat = pcrpid;
	}
#endif	
	Retcode = cnxt_kal_sem_put(gPlayerSem);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s eErrCode %d \n",__FUNCTION__, eErrCode);
	return eErrCode;
}

/**
@brief 用户得到Audio/video/pcr/sub/tt节目信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[out] psStreamInfo 保存节目信息的地址，具体结构参看CSUDIStreamInfo_S数据结构。
@param[in,out] pnStreamCnt 获取的节目信息个数.输入参数时用作psStreamInfo中所有元素个数，输出参数为psStreamInfo有效节目元素个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 
- 此接口在创建播放器后就可以使用，如果还没有设置节目，则返回nStreamCnt =0，且psStreamInfo的内容不能够被改变
- 使用此接口获取到的流信息与CSUDIPLAYERSetStream设置的信息相对应，传入的StreamInfo数组必须传足够大(建议>=5)，否则可能无法取到需要的信息

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetStream(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S * psStreamInfo,int * pnStreamCnt)
{
	int count=0, uPlayerIndx;
	CNXT_STATUS RetStatus;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	
	IS_VALID_PLAYER_HNDL(hPlayer);
	if((psStreamInfo==NULL)||(pnStreamCnt==NULL))
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"CSUDIPLAYERGetStream  0x%x\n", psStreamInfo);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	RetStatus = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(RetStatus != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL," In Funtion :%s  \n",__FUNCTION__);
	
	for(count =0; count <pPlayerSubsystem->nStreamCnt; count++)
	{
		cnxt_kal_memcpy(&psStreamInfo[count], &pPlayerSubsystem->sStreamInfo[count], sizeof(CSUDIStreamInfo_S));
	}
	*pnStreamCnt = pPlayerSubsystem->nStreamCnt;
	
	RetStatus = cnxt_kal_sem_put(gPlayerSem);
	if(RetStatus != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", RetStatus);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERGetStream 0x%x\n", hPlayer);
	return CSUDI_SUCCESS;
}


/**
@brief 获取回放参数信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[out] psPlaybackParam 保存回放信息，具体结构参看CSUDIPlaybackParam_S数据结构。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetPlaybackParam(CSUDI_HANDLE hPlayer, CSUDIPlaybackParam_S * psPlaybackParam)
{	
	int uPlayerIndx;
	CNXT_STATUS Retcode;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	IS_VALID_PLAYER_HNDL(hPlayer);
	if(psPlaybackParam==NULL)
	{
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	
	Retcode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
	}
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
	if(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)
	{
		Retcode = cnxt_kal_sem_put(gPlayerSem);
		return CSUDIPLAYER_ERROR_INVALID_HANDLE;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL," In Funtion :%s  \n",__FUNCTION__);
	cnxt_kal_memcpy(psPlaybackParam, &pPlayerSubsystem->sPlaybackParams, sizeof(CSUDIPlaybackParam_S));
	
	Retcode = cnxt_kal_sem_put(gPlayerSem);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: CSUDIPLAYERGetPlaybackParam 0x%x\n", hPlayer);
	return CSUDI_SUCCESS;
}

/**
@brief 用户修改Audio/video/pcr/sub/tt节目信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] psStreamInfo 待修改的节目信息，具体情况请参见CSUDIStreamInfo_S数据结构。
@param[in] nStreamCnt  修改的节目信息个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 试图设置两个音视频信息时返回CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER  ,即不能同时设置两个音视频PID  
@note 试图设置解码器不支持的音视频类型时返回CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER    
@note 修改节目信息会将前一次的设置全部覆盖，建议调用本接口前先通过CSUDIPLAYERGetPlaybackParam获取当前状态，再进行修改。
@note 修改后的动作于修改前的动作保持一致。如果修改前处于播放状态，则修改后也处于播放状态。
@note 该接口设置的音频格式与输出模式无关，如当前所有Aout设备均为DECODER模式，但当前Player中的Audio设备
仅支持AC3音频的BYPASS模式，使用本接口设置一个AC3音频时，仍返回成功，但实际输出会没有声音。
@todo 状态机

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERModifyStream(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S * psStreamInfo,int nStreamCnt)
{
	int uPlayerIndx, count=0;
	CNXT_STATUS Retcode;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	int32       nVidIndex=0, nAudIndex=0;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
    int32 uTempPcrPid=0x1FFF;

	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: %s 0x%x Streams %d\n", __FUNCTION__, hPlayer, nStreamCnt);
	//cnxt_kal_thread_time_sleep(100);
	IS_VALID_PLAYER_HNDL(hPlayer);
	if((psStreamInfo==NULL)||(nStreamCnt<=0)) //(psPlaybackParam==NULL)
	{
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	
	Retcode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	do
	{
		pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
		
		if(pPlayerSubsystem->ePlayerState!= E_PLAYER_START)
		{
			eErrCode = CSUDIPLAYER_ERROR_INVALID_STATUS;
			break;
		}

		if (!CS_CheckStreamParam(nStreamCnt,psStreamInfo))
		{
			eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
			break;
		}
		
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"AudPID 0x%x VidPid 0x%x \n", pPlayerSubsystem->uAudPID, pPlayerSubsystem->uVidPID);
		for(count =0; count <nStreamCnt;count++)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"content type %d PID 0x%x\n", psStreamInfo[count].m_eContentType,psStreamInfo[count].m_nPid );
			switch(psStreamInfo[count].m_eContentType)
			{
			case EM_UDI_CONTENT_VIDEO:
				
				if(Is_video_fmt_supported(psStreamInfo[count].m_uStreamType.m_eAudioType)==FALSE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"feature not supported1\n");
					eErrCode = CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
					break;
				}
				if((nVidIndex < 0)||(nVidIndex > MAX_VIDEO_OBJECTS) ||
						(pPlayerSubsystem->uVidPID == psStreamInfo[count].m_nPid)) /*PID repeated in same stream*/
				
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PID didn't changed \n");
					break;
				}				
				if((IS_VALID_PID(psStreamInfo[count].m_nPid) == FALSE)||(psStreamInfo[count].m_nPid<1)) 
				{/*This requirement is just to ensure that ES pid is >0 */
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Invalid PID 0x%x\n", psStreamInfo->m_nPid);
					eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
					break;
				}
				if((psStreamInfo[count].m_uStreamType.m_eVideoType == EM_UDI_VID_STREAM_MPEG2)||
						(psStreamInfo[count].m_uStreamType.m_eVideoType == EM_UDI_VID_STREAM_MPEG1))
				{
					pPlayerSubsystem->VideoFormat = CNXT_VIDEO_FORMAT_MPEG2;
				}
				else if(psStreamInfo[count].m_uStreamType.m_eVideoType == EM_UDI_VID_STREAM_H264)
				{
					pPlayerSubsystem->VideoFormat = CNXT_VIDEO_FORMAT_MPEG4;
				}
				else
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Vid feature not supported\n");
					eErrCode = CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
					break;
				}
				if(cs_tm_restart_video(hPlayer, psStreamInfo[count])!=TRUE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Err cs_cnxt_restart_video\n");
					Retcode = cnxt_kal_sem_put(gPlayerSem);
					return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
				}
				/*We do add/del device based on valid PID...Hence modify the pid in playersubsystem after restart*/
				pPlayerSubsystem->uVidPID = psStreamInfo[count].m_nPid;
				break;
			case EM_UDI_CONTENT_AUDIO:
				
				if(Is_audio_fmt_supported(psStreamInfo[count].m_uStreamType.m_eAudioType)==FALSE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"feature not supported1\n");
					eErrCode = CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
					break;
				}
				if((nAudIndex < 0)||(nAudIndex > MAX_VIDEO_OBJECTS) ||
						(pPlayerSubsystem->uAudPID == psStreamInfo[count].m_nPid)) /*PID repeated in same stream*/
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PID didn't changed \n");
					break;
				}
				if((IS_VALID_PID(psStreamInfo[count].m_nPid) == FALSE)||(psStreamInfo[count].m_nPid<1))
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Invalid PID 0x%x\n", psStreamInfo->m_nPid);
					eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
					break;
				}
				pPlayerSubsystem->StreamType = psStreamInfo[count].m_uStreamType.m_eAudioType;
				if(cs_tm_restart_audio(pPlayerSubsystem, psStreamInfo[count])!=TRUE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Err cs_cnxt_restart_audio\n");
					Retcode = cnxt_kal_sem_put(gPlayerSem);
					return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
				}
				/*We do add/del device based on valid PID...Hence modify the pid in playersubsystem after restart*/
				pPlayerSubsystem->uAudPID = psStreamInfo[count].m_nPid;
				break;
			case EM_UDI_CONTENT_SUBTITLE:
				if(IS_VALID_PID(psStreamInfo[count].m_nPid) == FALSE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Invalid PID 0x%x\n", psStreamInfo[count].m_nPid);
					eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
					break;
				}
				pPlayerSubsystem->uSubTitlePID = psStreamInfo[count].m_nPid;
				break;
			case EM_UDI_CONTENT_TELTEXT:
				if(IS_VALID_PID(psStreamInfo[count].m_nPid) == FALSE)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Invalid PID 0x%x\n", psStreamInfo[count].m_nPid);
					eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
					break;
				}
				pPlayerSubsystem->uTeletextPID = psStreamInfo[count].m_nPid;
				break;
			case EM_UDI_CONTENT_PCR :
				uTempPcrPid = psStreamInfo[count].m_nPid;                
				break;
			default:
				eErrCode = CSUDIPLAYER_ERROR_BAD_PARAMETER;
				break;
			}
		}
	}while(0);
	if(eErrCode == CSUDI_SUCCESS)
	{
		pPlayerSubsystem->bStreamModified = TRUE;
	}
	Retcode = cnxt_kal_sem_put(gPlayerSem);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
    /*There is a change in pcr pid. So, we need to reconfigure the pipe for sync/no_sync*/
	if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)&&
        (eErrCode == CSUDI_SUCCESS)&&
        (pPlayerSubsystem->uPcrPID != uTempPcrPid))
    {   
        CSUDIPLAYERStop( hPlayer);
        /*This is required, as sometimes, application is just audio & video but not pcr, 
                 in such case, we need start decoder with no sync*/
        pPlayerSubsystem->uPcrPID = uTempPcrPid;
        CSUDIPLAYERStart( hPlayer);
    }
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s eErrCode: 0x%x\n",__FUNCTION__, eErrCode);
	return eErrCode;
}

/**
@brief 启动一个播放器的工作 

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 按照CSUDIPLAYERSetInputStream 等函数设置的流信息以及其他相关参数播放一个媒体流。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERStart(CSUDI_HANDLE hPlayer)
{
	CNXT_STATUS               Status;
	//u_int32       nVidIndex = 0,nDmxIndex =0, nAudIndex=0;
	int uPlayerIndx;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_DEMUX_OBJECT        *hDemuxObj=NULL;
	PIPE_AUDIO_OBJECT        *hAudioObj=NULL;
	PIPE_VIDEO_OBJECT        *hVideoObj=NULL;
	PIPE_PIPELINE_CFG PipeCfg;
	PIPE_DEMUX_CFG demuxConfig;
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;
	
	PIPE_FLUSH_MODE 		 FlushMode; 
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERStart 0x%x\n", hPlayer);

	IS_VALID_PLAYER_HNDL(hPlayer);
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	Status = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(Status != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Status);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	do
	{
		pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
		if(pPlayerSubsystem->ePlayerState == E_PLAYER_START)
		{
			eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
			break;
		}
		if((pPlayerSubsystem->ePlayerState != E_PLAYER_READY)&&
				(pPlayerSubsystem->ePlayerState != E_PLAYER_STOP))
		{
			eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
			break;
		}
		if( (pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)  &&  (injectDataQueue == 0x0ffff) )
		{
			Status = cnxt_kal_qu_create(100, NULL, &injectDataQueue);
			if(Status != CNXT_STATUS_OK)
			{
			   printf(" Queue creation failed...\n\r" );
			}
		}
		
		hPipeObj = pPlayerSubsystem->pPipeObj;
		hDemuxObj = pPlayerSubsystem->pDemuxObj;
		hVideoObj = pPlayerSubsystem->pVideoObj;
		hAudioObj = pPlayerSubsystem->pAudioObj;
		if(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)
		{
#ifdef DUAL_VIDEO_SURFACE
			if(hVideoObj != NULL)
			{
				
				if(gTmVpDeviceObj.eImgattachpoint != CNXT_VIDEO_IMG_ATTACH )
				{
					gTmVpDeviceObj.eImgattachpoint = CNXT_VIDEO_IMG_ATTACH;
				}
			}
#endif
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"TS LIVE START \n");
			if((pPlayerSubsystem->bStreamModified == TRUE)||
					(pPlayerSubsystem->ePlayerState == E_PLAYER_READY)||
					(pPlayerSubsystem->ePlayerState == E_PLAYER_STOP))
			{
				pPlayerSubsystem->bStreamModified = FALSE;	
				
				if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
				{
					PIPE_AUDIO_CFG AudioConfig;
                    bool bDecPassthrough = FALSE;
                    PIPE_AUDIO_FORMAT AudFormat = get_tm_audio_format(pPlayerSubsystem->StreamType);
                    
                    bDecPassthrough = bIsEncDataRequired(AudFormat);
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s--------bDecPassthrough: %d\n",__FUNCTION__,bDecPassthrough);
					if( hPipeObj->pAudioDecoder == NULL)
					{
						PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)hAudioObj);
						if (PmStatus != PIPE_STATUS_OK)
						{
							CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
							eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
							break;
						}
					}
					pPlayerSubsystem->bVideoOnlyStream = FALSE;
					
					PmStatus = hDemuxObj->set_audio_pid(hDemuxObj, pPlayerSubsystem->uAudPID);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
					
					cnxt_kal_memset(&AudioConfig, 0, sizeof(AudioConfig));
					AudioConfig.Format = AudFormat;
					AudioConfig.Mode = bDecPassthrough?PIPE_AUDIO_MODE_DECODE_AND_PASSTHROUGH:PIPE_AUDIO_MODE_DECODE;
					AudioConfig.DualMono = PIPE_AUDIO_DUAL_MONO_NORMAL;

					if (AudioConfig.Format == PIPE_AUDIO_FMT_AAC)
					{
						AudioConfig.FormatCfg.AacCfg.bEnableDownMix = TRUE;
						AudioConfig.FormatCfg.AacCfg.DownmixType = PIPE_AUDIO_DOWNMIX_LORO;
						AudioConfig.FormatCfg.AacCfg.uScaleFactor = 1;
					}
					/* config audio */
                    if(gTmPipeObject.TridCoshipSetup[0].eChannelType == EM_UDIAUDIO_CHANNEL_MIXED_MONO)
                    {
                        AudioConfig.DualMono = PIPE_AUDIO_DUAL_MONO_MIXEDMONO;
                    }
                    
					PmStatus = hAudioObj->config(hAudioObj, &AudioConfig);
                    
					if(PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
                    
					PmStatus = cs_pipe_apm_attach(hPipeObj, PIPE_AUDIO_PRESENTATION_STREAM_INPUT, 
                                                  pPlayerSubsystem->AudioIndx, bDecPassthrough);
			        if(PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
				if((hVideoObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
				{
					PIPE_VIDEO_CFG VideoConfig;
					PIPE_VP_SURFACE_INPUT Input;
					
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"At %s %d \n", __FUNCTION__, __LINE__);
					
					if( hPipeObj->pVideoDecoder== NULL)
					{
						PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)hVideoObj);
						if (PmStatus != PIPE_STATUS_OK)
						{
							CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
							eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
							break;
						}
					}
					pPlayerSubsystem->bRadioStream = FALSE;

					PmStatus = hDemuxObj->set_video_pid(hDemuxObj, pPlayerSubsystem->uVidPID);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
					cnxt_kal_memset(&VideoConfig, 0, sizeof(VideoConfig));
					VideoConfig.Format               = pPlayerSubsystem->VideoFormat;
					VideoConfig.Definition           = PIPE_VIDEO_HD;
					VideoConfig.DecodingType         = PIPE_VIDEO_MOTION_VIDEO;
					VideoConfig.bHwAccelerate        = TRUE;
					VideoConfig.bPrimaryDecoder     = TRUE;
					VideoConfig.uUserDataEnableMap = 0;
					/* config video */
					PmStatus = hVideoObj->config(hVideoObj, &VideoConfig);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
					if( pPlayerSubsystem->bTrickMode == FALSE )
					{
						Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_PIPELINE;
						Input.pPipe = hPipeObj;
						Input.hImageHandle = NULL; //2011-01-18 TerenceZhang Added          
						
						PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_PRIMARY_SURFACE, //pPlayerSubsystem->VideoIndx, 
	                                                    pPlayerSubsystem->DemuxIndx);
						if (PmStatus != PIPE_STATUS_OK)
						{
							CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
							eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
							break;
						}
					}
				}	
				/* config pipe */
				PipeCfg.Type = PIPE_PIPELINE_DVB_TS_TYPE;
				PipeCfg.uStcIndex = 0;
				PipeCfg.bPipPipe = FALSE;				
				
				if(IS_VALID_PID(pPlayerSubsystem->uPcrPID))
				{
					PipeCfg.SyncMaster = PIPE_SYNC_DEMUX_MASTER;
				}
				else
				{
					PipeCfg.SyncMaster = PIPE_SYNC_NO_SYNC;
				}
				
				PmStatus = hPipeObj->config(hPipeObj, &PipeCfg);
				if (PmStatus != PIPE_STATUS_OK)
				{
				    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
                if(hPipeObj != NULL)
                {
                    FlushMode = PIPE_FULL_FLUSH;
                    PmStatus = hPipeObj->flush(hPipeObj,FlushMode);
    				if(PmStatus != PIPE_STATUS_OK )
    				{
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
    					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
    					break;
    				}
                }
                    
				if(IS_VALID_PID(pPlayerSubsystem->uPcrPID))
				{
					PmStatus = hDemuxObj->set_pcr_pid(hDemuxObj, pPlayerSubsystem->uPcrPID);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
					PmStatus = hDemuxObj->clock_recovery_ctrl(hDemuxObj, TRUE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
				if((hVideoObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
				{
					/* start video */
					PmStatus = hVideoObj->decoder_ctrl(hVideoObj, TRUE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
				if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
				{
					/* start audio */
					PmStatus = hAudioObj->decoder_ctrl(hAudioObj, TRUE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
                    //set mute or mute here                    
                    if(gTmPipeObject.TridCoshipSetup[0].AudioMute == 1)
                    {
                         CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d MUTE......\n",__FUNCTION__,__LINE__);
                        hAudioObj->mute(hAudioObj, TRUE);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d UNMUTE......\n",__FUNCTION__,__LINE__);
                        hAudioObj->mute(hAudioObj, FALSE);
                    }
				}
				if(hDemuxObj != NULL)
				{
					PmStatus = hDemuxObj->demux_play_ctrl(hDemuxObj, TRUE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
			}
		}
        else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT) &&/*
					(pPlayerSubsystem->AudioIndx >= 0) &&
					(pPlayerSubsystem->VideoIndx >= 0) &&*/  /*TS inject... TBD: MAY FAIL audio only...*/
					(pPlayerSubsystem->DemuxIndx >= 0))
        {
				PIPE_VP_SURFACE_INPUT Input;	
				pPlayerSubsystem->bStreamModified = FALSE;
                
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"TS INJECT START \n");
                
#ifdef DUAL_VIDEO_SURFACE
			if(hVideoObj != NULL)
			{
				
				if(gTmVpDeviceObj.eImgattachpoint != CNXT_VIDEO_IMG_ATTACH )
				{
					gTmVpDeviceObj.eImgattachpoint = CNXT_VIDEO_IMG_ATTACH;
				}
			}
#endif	
                /* config demux */
                cnxt_kal_memset(&demuxConfig, 0, sizeof(demuxConfig));
                demuxConfig.Type = PIPE_DEMUX_TS;
                
                PmStatus = hDemuxObj->config(hDemuxObj, &demuxConfig);
                if (PmStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
                    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                    break;
                }
				
				if( hPipeObj->pDemux== NULL)
				{
					hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)hDemuxObj);
				}
				
				if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
				{
					PIPE_AUDIO_CFG AudioConfig;
                    bool bDecPassthrough = FALSE;
                    PIPE_AUDIO_FORMAT AudFormat = get_tm_audio_format(pPlayerSubsystem->StreamType);
                    
                    bDecPassthrough = bIsEncDataRequired(AudFormat);
					
					pPlayerSubsystem->bVideoOnlyStream = FALSE;
					cnxt_kal_memset(&AudioConfig, 0, sizeof(AudioConfig));
					AudioConfig.Format = AudFormat;
					AudioConfig.Mode = bDecPassthrough?PIPE_AUDIO_MODE_DECODE_AND_PASSTHROUGH:PIPE_AUDIO_MODE_DECODE;
					AudioConfig.DualMono = PIPE_AUDIO_DUAL_MONO_NORMAL;

					if (AudioConfig.Format == PIPE_AUDIO_FMT_AAC)
					{
						AudioConfig.FormatCfg.AacCfg.bEnableDownMix = TRUE;
						AudioConfig.FormatCfg.AacCfg.DownmixType = PIPE_AUDIO_DOWNMIX_LORO;
						AudioConfig.FormatCfg.AacCfg.uScaleFactor = 1;
					}
                    if(gTmPipeObject.TridCoshipSetup[0].eChannelType == EM_UDIAUDIO_CHANNEL_MIXED_MONO)
                    {
                        AudioConfig.DualMono = PIPE_AUDIO_DUAL_MONO_MIXEDMONO;
                    }
                    
                    if(pPlayerSubsystem->CurStreamType != pPlayerSubsystem->StreamType)
                    {
                        /* config audio */
					    PmStatus = hAudioObj->config(hAudioObj, &AudioConfig);
					    if(PmStatus != PIPE_STATUS_OK)
					    {
                            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						    break;
					    }
                        pPlayerSubsystem->CurStreamType = pPlayerSubsystem->StreamType;                            
                    }
					
					if( hPipeObj->pAudioDecoder== NULL)
					{
						PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)hAudioObj);
						if (PmStatus != PIPE_STATUS_OK)
						{
							CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
							eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
							break;
						}
					}

					    PmStatus = cs_pipe_apm_attach(hPipeObj, PIPE_AUDIO_PRESENTATION_STREAM_INPUT, 
                                                      pPlayerSubsystem->AudioIndx, bDecPassthrough);
			        if(PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
				if((hVideoObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
				{
					PIPE_VIDEO_CFG VideoConfig;
					
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"At %s %d \n", __FUNCTION__, __LINE__);
					pPlayerSubsystem->bRadioStream = FALSE;
					
					cnxt_kal_memset(&VideoConfig, 0, sizeof(VideoConfig));
					VideoConfig.Format               = pPlayerSubsystem->VideoFormat;
					VideoConfig.Definition           = PIPE_VIDEO_HD;
					VideoConfig.DecodingType         = PIPE_VIDEO_MOTION_VIDEO;
					VideoConfig.bHwAccelerate        = TRUE;
					VideoConfig.bPrimaryDecoder     = TRUE;
					VideoConfig.uUserDataEnableMap = 0;
					/* config video */
                    if(pPlayerSubsystem->CurVideoFormat != pPlayerSubsystem->VideoFormat)
                    {
                        PmStatus = hVideoObj->config(hVideoObj, &VideoConfig);
					    if (PmStatus != PIPE_STATUS_OK)
					    {
                            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						    break;
					    }
                        pPlayerSubsystem->CurVideoFormat = VideoConfig.Format;                                                                                     
                        printf(" ########### video config success ############\n\r ");
                    }
					
					if( hPipeObj->pVideoDecoder== NULL)
					{
						PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)hVideoObj);
						if (PmStatus != PIPE_STATUS_OK)
						{
							CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
							eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
							break;
						}
					}	
				}	
				{

					Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_PIPELINE;
					Input.pPipe = hPipeObj;
 					Input.hImageHandle = NULL; //2011-01-18 TerenceZhang Added          
					PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_PRIMARY_SURFACE, //pPlayerSubsystem->VideoIndx, 
                                                         pPlayerSubsystem->DemuxIndx);
					if(PmStatus != PIPE_STATUS_OK )
					{
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						break;
					}

					FlushMode = PIPE_FULL_FLUSH;
                    PmStatus = hPipeObj->flush(hPipeObj,FlushMode);
					if(PmStatus != PIPE_STATUS_OK )
					{
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						break;
					}
				}	
				/* config pipe */
				PipeCfg.Type = PIPE_PIPELINE_DVB_TS_TYPE;
				PipeCfg.uStcIndex = 0;
				PipeCfg.bPipPipe = FALSE;

				if(hAudioObj!= NULL)
				{
					PipeCfg.SyncMaster = PIPE_SYNC_AUDIO_MASTER;
				}
				else if(hVideoObj!= NULL)
				{
					PipeCfg.SyncMaster = PIPE_SYNC_VIDEO_MASTER;
				}

				PmStatus = hPipeObj->config(hPipeObj, &PipeCfg);
				if (PmStatus != PIPE_STATUS_OK)
				{
				    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
				
				if(hDemuxObj != NULL)
				{
					PmStatus = hDemuxObj->demux_play_ctrl(hDemuxObj, TRUE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}

				if((hVideoObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
				{
					/* start video */
					PmStatus = hVideoObj->decoder_ctrl(hVideoObj, TRUE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
				if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
				{
					/* start audio */
					PmStatus = hAudioObj->decoder_ctrl(hAudioObj, TRUE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
                    
                    //set mute or mute here                    
                    if(gTmPipeObject.TridCoshipSetup[0].AudioMute == 1)
                    {
                         CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d MUTE......\n",__FUNCTION__,__LINE__);
                        hAudioObj->mute(hAudioObj, TRUE);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d UNMUTE......\n",__FUNCTION__,__LINE__);
                        hAudioObj->mute(hAudioObj, FALSE);
                    }

				}

				
				{
					PmStatus = hDemuxObj->set_audio_pid(hDemuxObj, pPlayerSubsystem->uAudPID);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
					
					PmStatus = hDemuxObj->set_video_pid(hDemuxObj, pPlayerSubsystem->uVidPID);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
				
				{
					PmStatus = hPipeObj->set_decoding_speed(hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 1024);
                    if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						//break;
					}
					bPlayerStarted = TRUE;
				}
		 }
         else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
                 ((pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_PCM)||
                  (pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_LPCM)))
         {
             /*Code For Play PCM from memory.....No dmx, No video, no Audio decoder*/
              if( TRUE!=CSPcmPlay_Start(pPlayerSubsystem->AudioIndx, &pPlayerSubsystem->nMixerId))
              {
				  CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"ERR: %s %d PmStatus %d\n",__FUNCTION__,__LINE__,PmStatus);
                  eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                  break;
              }
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"starting PCM player\n", __FUNCTION__, __LINE__);
         }
	     else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&                  
                  (pPlayerSubsystem->bIframeInject == FALSE)) /*For PES/ES play*/
	     {
	#ifdef DUAL_VIDEO_SURFACE
			if(hVideoObj != NULL)
			{
				
				
				if(gTmVpDeviceObj.eImgattachpoint != CNXT_VIDEO_IMG_ATTACH )
				{
					gTmVpDeviceObj.eImgattachpoint = CNXT_VIDEO_IMG_ATTACH;
				}
			}
	#endif
			if((pPlayerSubsystem->bStreamModified == TRUE)||
					(pPlayerSubsystem->ePlayerState == E_PLAYER_READY))
			{
				if(hVideoObj != NULL)
				{
					PIPE_VIDEO_CFG VideoConfig;
					PIPE_VP_SURFACE_INPUT Input;
                    int Indx=0;

                    if(hVideoObj!=NULL)
                    {
                        Indx = pPlayerSubsystem->VideoIndx+ES_VIDEO_OBJECT;
                    }
                    else if(hAudioObj!=NULL)
                    {
                        Indx = ES_AUDIO_OBJECT;
                    }

                    pPlayerSubsystem->pDemuxObj = hDemuxObj = gTmPipeObject.hDemuxObj[Indx] = PIPE_OBJ_CREATE(demux);
                    /* config demux */
                    cnxt_kal_memset(&demuxConfig, 0, sizeof(demuxConfig));
					//2011-01-19 BOB begin:
					if((hVideoObj!=NULL) && (hAudioObj==NULL))
					{
                    	demuxConfig.Type = PIPE_DEMUX_VIDEO_ES;
                    }
					else if((hVideoObj==NULL) && (hAudioObj!=NULL))
					{
						demuxConfig.Type = PIPE_DEMUX_AUDIO_ES;
					}
					else
					{
						demuxConfig.Type = PIPE_DEMUX_MULTI_ES;
					}
					demuxConfig.DescramblerType   = PIPE_DEMUX_DESCRAMBLE_NONE;
					//2011-01-19 BOB end
                    
                    PmStatus = hDemuxObj->config(hDemuxObj, &demuxConfig);
                    if (PmStatus != PIPE_STATUS_OK)
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
                        eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                        break;
                    }
    				/*Add video object to Inject pipeline*/
					if( hPipeObj->pVideoDecoder == NULL )
					{
    					hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)hVideoObj);
					}
					pPlayerSubsystem->bRadioStream = FALSE;
					if( hPipeObj->pDemux == NULL )
					{
	                    PmStatus = hPipeObj->add_device(hPipeObj,(PIPE_OBJ_HANDLE)hDemuxObj);
	                    if (PmStatus != PIPE_STATUS_OK )
	                    {
	                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
	                        eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
	                        break;
	                    } 
					}
					cnxt_kal_memset(&VideoConfig, 0, sizeof(VideoConfig));
					VideoConfig.Format                   = pPlayerSubsystem->VideoFormat;
					VideoConfig.Definition               = PIPE_VIDEO_HD;
					VideoConfig.DecodingType         = PIPE_VIDEO_MOTION_VIDEO;/*TBD: How to decide whether it is for i-frame or PES video?*/
					VideoConfig.bHwAccelerate        = TRUE;
					VideoConfig.bPrimaryDecoder     = TRUE;
					VideoConfig.uUserDataEnableMap = 0;

					/* config video */
					PmStatus = hVideoObj->config(hVideoObj, &VideoConfig);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
					Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_PIPELINE;
					Input.pPipe = hPipeObj;
					Input.hImageHandle = NULL; //2011-01-18 TerenceZhang Added          

					PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_PRIMARY_SURFACE, //pPlayerSubsystem->VideoIndx, 
                                                         Indx);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}	
				if(hAudioObj != NULL)
				{
					PIPE_AUDIO_CFG AudioConfig;
                    bool bDecPassthrough = FALSE;
                    PIPE_AUDIO_FORMAT AudFormat = get_tm_audio_format(pPlayerSubsystem->StreamType);
                    bDecPassthrough = bIsEncDataRequired(AudFormat);
    				/*Add Audio object to Inject pipeline*/
					
					if( hPipeObj->pAudioDecoder == NULL )
					{
    					hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)hAudioObj);
					}
					pPlayerSubsystem->bVideoOnlyStream = FALSE;
                    
					cnxt_kal_memset(&AudioConfig, 0, sizeof(AudioConfig));
					AudioConfig.Format = AudFormat;
					AudioConfig.Mode = bDecPassthrough?PIPE_AUDIO_MODE_DECODE_AND_PASSTHROUGH:PIPE_AUDIO_MODE_DECODE;
					AudioConfig.DualMono = PIPE_AUDIO_DUAL_MONO_NORMAL;

					if (AudioConfig.Format == PIPE_AUDIO_FMT_AAC)
					{
						AudioConfig.FormatCfg.AacCfg.bEnableDownMix = TRUE;
						AudioConfig.FormatCfg.AacCfg.DownmixType = PIPE_AUDIO_DOWNMIX_LORO;
						AudioConfig.FormatCfg.AacCfg.uScaleFactor = 1;
					}
					/* config audio */
					PmStatus = hAudioObj->config(hAudioObj, &AudioConfig);
					if(PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
                    PmStatus = cs_pipe_apm_attach(hPipeObj, PIPE_AUDIO_PRESENTATION_STREAM_INPUT,
                                                        pPlayerSubsystem->AudioIndx, bDecPassthrough);
					if(PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
			}
			if(hVideoObj != NULL)
			{
				/* start video */
				PmStatus = hVideoObj->decoder_ctrl(hVideoObj, TRUE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
			}
			if(hAudioObj != NULL)
			{
				/* start audio */
				PmStatus = hAudioObj->decoder_ctrl(hAudioObj, TRUE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
                
                //set mute or mute here                    
                if(gTmPipeObject.TridCoshipSetup[0].AudioMute == 1)
                {
                     CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d MUTE......\n",__FUNCTION__,__LINE__);
                    hAudioObj->mute(hAudioObj, TRUE);
                }
                else
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d UNMUTE......\n",__FUNCTION__,__LINE__);
                    hAudioObj->mute(hAudioObj, FALSE);
                }
			}
            if((hAudioObj != NULL)||(hVideoObj != NULL))
            {
                PmStatus = hDemuxObj->demux_play_ctrl(hDemuxObj, TRUE);
                if (PmStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
                    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                    break;
                }
                hPipeObj->set_decoding_speed(hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 1024);
            }
			bPlayerStarted = TRUE;
		}
		else if(pPlayerSubsystem->bIframeInject == TRUE)
		{
#ifdef DUAL_VIDEO_SURFACE

			if(gTmVpDeviceObj.eImgattachpoint != CNXT_STILL_IMG_ATTACH)
			{
				gTmVpDeviceObj.eImgattachpoint = CNXT_STILL_IMG_ATTACH;
			}
#endif
			CS_start_Iframe_decode(pPlayerSubsystem);
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Nothing to do for I-frame inject %s %d \n", __FUNCTION__, __LINE__);
		}
		pPlayerSubsystem->ePlayerState = E_PLAYER_START;
	}while(0);
	Status = cnxt_kal_sem_put(gPlayerSem);
	if(Status != CNXT_STATUS_OK)
	{
		CSDebug(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Status);
		return CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
	}
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s eErrCode %d\n",__FUNCTION__, eErrCode);
	return eErrCode;
}



/**
@brief 停止一个播放器的工作

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 停止一个播放器的工作，与CSUDIPLAYERStart成对使用。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERStop(CSUDI_HANDLE hPlayer)
{
	CNXT_STATUS               Status;
	//u_int32       nVidIndex,nDmxIndex;
	int uPlayerIndx;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	
	IS_VALID_PLAYER_HNDL(hPlayer);
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	Status = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(Status != CNXT_STATUS_OK)
	{
		CSDebug(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Status);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s  playerstate:%d\n",__FUNCTION__,pPlayerSubsystem->ePlayerState);
	
	do
	{
		if((pPlayerSubsystem->ePlayerState != E_PLAYER_START)&&
				(pPlayerSubsystem->ePlayerState != E_PLAYER_PAUSE))
		{
			eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
			break;
		}
        if( (pPlayerSubsystem->VideoIndx >=0)&&(pPlayerSubsystem->bRadioStream == FALSE)&&(pPlayerSubsystem->bTrickMode == FALSE))
        {
            if(gTmPipeObject.TridCoshipSetup[pPlayerSubsystem->VideoIndx].eStopMode == EM_UDIVIDEO_STOPMODE_BLACK)/*BLANK*/
            {
                
#ifdef DUAL_VIDEO_SURFACE
				
#else             
				PIPE_STATUS Status;
                
				Status = gTmVpDeviceObj.pHDVideoSurface[pPlayerSubsystem->VideoIndx]->surface_disable(gTmVpDeviceObj.pHDVideoSurface[pPlayerSubsystem->VideoIndx]);
				if(Status != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}
				Status = gTmVpDeviceObj.pSDVideoSurface[pPlayerSubsystem->VideoIndx]->surface_disable(gTmVpDeviceObj.pSDVideoSurface[pPlayerSubsystem->VideoIndx]);
				if(Status != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}           
#endif
            }
        }
		if(pPlayerSubsystem->bIframeInject == TRUE)
		{
			/*Do nothing for I-frame inject as data is fed in inject start itself*/
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL," I-frame inject %s %d \n", __FUNCTION__, __LINE__);
            CS_stop_Iframe_decode(pPlayerSubsystem);
		}
        else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
             ((pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_PCM)||
              (pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_LPCM)))
        {
            /*Code For Play PCM from memory.....No dmx, No video, no Audio decoder*/
            if(TRUE!=CSPcmPlay_Stop(pPlayerSubsystem->AudioIndx, pPlayerSubsystem->nMixerId))
            {
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL," error :%s  %d \n",__FUNCTION__, __LINE__);                    
                eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                break;
            }
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Stopping PCM player\n", __FUNCTION__, __LINE__);
        }
		else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)||
				((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)/*&&
				(pPlayerSubsystem->AudioIndx >= 0) &&
					(pPlayerSubsystem->VideoIndx >= 0) &&
					(pPlayerSubsystem->DemuxIndx >= 0)*/))
		{
			
			PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
			PIPE_DEMUX_OBJECT        *hDemuxObj;
			PIPE_AUDIO_OBJECT        *hAudioObj;
			PIPE_VIDEO_OBJECT        *hVideoObj;
			PIPE_STATUS PmStatus = PIPE_STATUS_OK;
			
			bPlayerStarted = FALSE;
			hPipeObj = pPlayerSubsystem->pPipeObj;
			hDemuxObj = pPlayerSubsystem->pDemuxObj;
			hVideoObj = pPlayerSubsystem->pVideoObj;
			hAudioObj = pPlayerSubsystem->pAudioObj;
			
			pPlayerSubsystem->bStreamModified = FALSE;
			if((pPlayerSubsystem->bRadioStream == FALSE))
			{
				int     chain=0;    
				  
				if((gTmPipeObject.TridCoshipSetup[chain].eStopMode == EM_UDIVIDEO_STOPMODE_BLACK)&&(pPlayerSubsystem->bTrickMode == FALSE))
				{
#ifdef DUAL_VIDEO_SURFACE
				
#else             
                
					PmStatus = gTmVpDeviceObj.pHDVideoSurface[chain]->surface_disable(gTmVpDeviceObj.pHDVideoSurface[chain]);
					if(PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
						eErrCode = CSUDI_FAILURE;
						break;
					}
					PmStatus = gTmVpDeviceObj.pSDVideoSurface[chain]->surface_disable(gTmVpDeviceObj.pSDVideoSurface[chain]);
					if(PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
						eErrCode = CSUDI_FAILURE;
						break;
					}
#endif
				}
			}
		
			if(hDemuxObj != NULL)
			{
				PmStatus = hDemuxObj->demux_play_ctrl(hDemuxObj, FALSE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
				if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)&&
						IS_VALID_PID(pPlayerSubsystem->uPcrPID))
				{
					PmStatus = hDemuxObj->clock_recovery_ctrl(hDemuxObj, FALSE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
			}			
			
			if((hVideoObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
			{	          			
				/* stop video */
				PmStatus = hVideoObj->decoder_ctrl(hVideoObj, FALSE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
			}
			if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
			{	          		      
				/* stop audio */
				PmStatus = hAudioObj->decoder_ctrl(hAudioObj, FALSE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
				
			}
			if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
			{
				PmStatus = cs_pipe_apm_detach(hPipeObj);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
				}
			}
            if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
                (pPlayerSubsystem->DemuxIndx < 0)) /*This is for PES/ES stream*/
            {
				PmStatus = hPipeObj->delete_device(hPipeObj,(PIPE_OBJ_HANDLE)hDemuxObj);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
                PmStatus = PIPE_OBJ_DESTROY(demux,hDemuxObj);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
            }
			
			if(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)
			{				
				cnxt_kal_qu_destroy(injectDataQueue);
				injectDataQueue = 0x0ffff;
			} 
		}
		if((pPlayerSubsystem->ePlayerState == E_PLAYER_START)||
			(pPlayerSubsystem->ePlayerState == E_PLAYER_PAUSE))
		{
			pPlayerSubsystem->ePlayerState = E_PLAYER_STOP;
		}
	}while(0);
	Status = cnxt_kal_sem_put(gPlayerSem);
	if(Status != CNXT_STATUS_OK)
	{
		CSDebug(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Status);
		return CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s eErrCode %d \n",__FUNCTION__, eErrCode);
	
	return eErrCode;
}

/**
@brief 暂停与hPlayer相关的一个播放器的工作

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 
      - 成功返回CSUDI_SUCCESS；
      - 失败则返回错误代码值；
      - 不支持此操作返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 暂停与hPlayer相关的一个播放器的工作；只有正在播放某一节目（不论是只有音频还是视频）时调用才有效。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERPause(CSUDI_HANDLE hPlayer)
{
	int32       nVidIndex=0,nDmxIndex=0, nAudIndex=0;
	int uPlayerIndx;
	CNXT_STATUS Retcode;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	PIPE_AUDIO_OBJECT        *hAudioObj;
	PIPE_VIDEO_OBJECT        *hVideoObj;
	PIPE_DEMUX_OBJECT        *hDemuxObj;
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;
	
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Enter Funtion :%s  \n",__FUNCTION__);
	IS_VALID_PLAYER_HNDL(hPlayer);
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);

	Retcode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
	nDmxIndex = pPlayerSubsystem->DemuxIndx ;
	nVidIndex = (pPlayerSubsystem->bRadioStream)?0:pPlayerSubsystem->VideoIndx;
	nAudIndex = (pPlayerSubsystem->bVideoOnlyStream)?0:pPlayerSubsystem->AudioIndx;
	/*
	pVIDHandle = &gVideoConfig.hVideoHandle[nVidIndex];
	pAUDHandle = &gAudioConfig.hAudioHandle[nAudIndex];*/

	if(pPlayerSubsystem->ePlayerState!= E_PLAYER_START)
	{	
		Retcode = cnxt_kal_sem_put(gPlayerSem);
		return CSUDIPLAYER_ERROR_INVALID_STATUS;
	}
	do
	{
	       if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
	   	    ((pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_PCM)||
	   	     (pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_LPCM)))
	       {
                /*Code For Play PCM from memory.....No dmx, No video, no Audio decoder*/
                if( TRUE!=CSPcmPlay_Pause(pPlayerSubsystem->AudioIndx) )
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL," error :%s  %d \n",__FUNCTION__, __LINE__);
                    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                    break;
                }
	       }
		   else if(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)
		   {
#if 1
			hVideoObj = pPlayerSubsystem->pVideoObj;
			hAudioObj = pPlayerSubsystem->pAudioObj;
			hDemuxObj = pPlayerSubsystem->pDemuxObj;
			if(hDemuxObj != NULL)
			{
				PmStatus = hDemuxObj->demux_play_ctrl(hDemuxObj, FALSE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
				if(IS_VALID_PID(pPlayerSubsystem->uPcrPID))
				{
					PmStatus = hDemuxObj->clock_recovery_ctrl(hDemuxObj, FALSE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
			}
			if(hAudioObj != NULL)
			{                        
				/* stop audio */
				PmStatus = hAudioObj->decoder_ctrl(hAudioObj, FALSE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
			}
			if(hVideoObj != NULL)
			{                      
				/* stop video */
				PmStatus = hVideoObj->decoder_ctrl(hVideoObj, FALSE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
			}
#endif			 
		}
		else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)/*&&
					(pPlayerSubsystem->AudioIndx >= 0) &&
						(pPlayerSubsystem->VideoIndx >= 0) &&
						(pPlayerSubsystem->DemuxIndx >= 0)*/)
		{
			PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
			
    			hPipeObj = pPlayerSubsystem->pPipeObj;
				bPlayerStatus = 1;
    			PmStatus = hPipeObj->set_decoding_speed(hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 0);
    			if(PmStatus != PIPE_STATUS_OK)
    			{
    				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                    break;
    			}
	       }
		   pPlayerSubsystem->ePlayerState = E_PLAYER_PAUSE;
	}while(0);
	Retcode = cnxt_kal_sem_put(gPlayerSem);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Exit Funtion :%s  0x%x \n",__FUNCTION__, hPlayer);
	return eErrCode;
}

/**
@brief 恢复暂停模式 

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 不支持此操作返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 恢复暂停模式;与CSUDIPLAYERPause成对使用。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERResume(CSUDI_HANDLE hPlayer)
{
	int32       nVidIndex=0,nDmxIndex=0, nAudIndex=0;
	int uPlayerIndx;
	CNXT_STATUS Retcode;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	PIPE_AUDIO_OBJECT        *hAudioObj;
	PIPE_VIDEO_OBJECT        *hVideoObj;
	PIPE_DEMUX_OBJECT        *hDemuxObj;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_PIPELINE_CFG PipeCfg;
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERResume 0x%x Streams %d\n", hPlayer);
	IS_VALID_PLAYER_HNDL(hPlayer);
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	
	Retcode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
	
	if(pPlayerSubsystem->ePlayerState!= E_PLAYER_PAUSE)
	{
		Retcode = cnxt_kal_sem_put(gPlayerSem);
		return CSUDIPLAYER_ERROR_INVALID_STATUS;
	}
	
	nDmxIndex = pPlayerSubsystem->DemuxIndx ;
	nVidIndex = (pPlayerSubsystem->bRadioStream)?0:pPlayerSubsystem->VideoIndx;
	nAudIndex = (pPlayerSubsystem->bVideoOnlyStream)?0:pPlayerSubsystem->AudioIndx;
	/*
	pVIDHandle = &gVideoConfig.hVideoHandle[nVidIndex];
	pAUDHandle = &gAudioConfig.hAudioHandle[nAudIndex];*/
	do
	{
	       if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
	   	       ((pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_PCM)||
	   	       (pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_LPCM)))
	       {
    	       	/*Code For Play PCM from memory.....No dmx, No video, no Audio decoder*/
                if( TRUE!=CSPcmPlay_Resume(pPlayerSubsystem->AudioIndx) )
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL," error :%s  %d \n",__FUNCTION__, __LINE__);
                    Retcode = cnxt_kal_sem_put(gPlayerSem);
                    return CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                }
	       }
		   else if(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)
		   {
              hVideoObj = pPlayerSubsystem->pVideoObj;
              hAudioObj = pPlayerSubsystem->pAudioObj;
              hDemuxObj = pPlayerSubsystem->pDemuxObj;
              hPipeObj  = pPlayerSubsystem->pPipeObj;
#if 1
			/* config pipe */
			PipeCfg.Type = PIPE_PIPELINE_DVB_TS_TYPE;
			PipeCfg.uStcIndex = 0;
			PipeCfg.bPipPipe = FALSE;

			if(IS_VALID_PID(pPlayerSubsystem->uPcrPID))
			{
				PipeCfg.SyncMaster = PIPE_SYNC_DEMUX_MASTER;
			}
			else
			{
				PipeCfg.SyncMaster = PIPE_SYNC_NO_SYNC;
			}
			PmStatus = hPipeObj->config(hPipeObj, &PipeCfg);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d %d \n", __FUNCTION__, __LINE__, PmStatus);
				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
				break;
			}
			if(hDemuxObj != NULL)
			{
				PmStatus = hDemuxObj->demux_play_ctrl(hDemuxObj, TRUE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
				
				if(IS_VALID_PID(pPlayerSubsystem->uPcrPID))
				{
					PmStatus = hDemuxObj->clock_recovery_ctrl(hDemuxObj, TRUE);
					if (PmStatus != PIPE_STATUS_OK)
					{
						CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
						eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
						break;
					}
				}
			}
			if(hAudioObj != NULL)
			{                        
				/* start audio */
				PmStatus = hAudioObj->decoder_ctrl(hAudioObj, TRUE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
                
                //set mute or mute here                    
                if(gTmPipeObject.TridCoshipSetup[0].AudioMute == 1)
                {
                     CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d MUTE......\n",__FUNCTION__,__LINE__);
                    hAudioObj->mute(hAudioObj, TRUE);
                }
                else
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d UNMUTE......\n",__FUNCTION__,__LINE__);
                    hAudioObj->mute(hAudioObj, FALSE);
                }
			}
			if(hVideoObj != NULL)
			{                      
				/* stop video */
				PmStatus = hVideoObj->decoder_ctrl(hVideoObj, TRUE);
				if (PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus[%d]\n", __FUNCTION__, __LINE__,PmStatus);
					eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
					break;
				}
			}
#endif		 
		   }
	       else if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)/*&&
				 	  (pPlayerSubsystem->AudioIndx >= 0) &&
			   		  (pPlayerSubsystem->VideoIndx >= 0) &&
			   		  (pPlayerSubsystem->DemuxIndx >= 0)*/)
	       {
    			hPipeObj = pPlayerSubsystem->pPipeObj;
				bPlayerStatus = 0;
    			PmStatus = hPipeObj->set_decoding_speed(hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 1024);
    			if(PmStatus != PIPE_STATUS_OK)
    			{
    				eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
    			}
                hAudioObj = pPlayerSubsystem->pAudioObj;
                
                if(hAudioObj != NULL)
                {                                            
                    //set mute or mute here                    
                    if(gTmPipeObject.TridCoshipSetup[0].AudioMute == 1)
                    {
                         CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d MUTE......\n",__FUNCTION__,__LINE__);
                        hAudioObj->mute(hAudioObj, TRUE);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d UNMUTE......\n",__FUNCTION__,__LINE__);
                        hAudioObj->mute(hAudioObj, FALSE);
                    }
                }
    	   }
	       else
	       {
	       }
		pPlayerSubsystem->ePlayerState = E_PLAYER_START;
	}while(0);
	
	Retcode = cnxt_kal_sem_put(gPlayerSem);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: CSUDIPLAYERResume 0x%x\n", hPlayer);
	return eErrCode;
}


/**
@brief 清除播放器中缓存的数据

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 不支持此操作返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 本接口不改变Player状态，仅清除缓冲数据，若仍在播放状态，则新数据会重新开始播放

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERClear(CSUDI_HANDLE hPlayer)
{
    CNXT_STATUS Retcode;
    int uPlayerIndx;
    UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
    CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_DEMUX_OBJECT        *hDemuxObj=NULL;
	PIPE_AUDIO_OBJECT        *hAudioObj=NULL;
	PIPE_VIDEO_OBJECT        *hVideoObj=NULL;
    
    IS_VALID_PLAYER_HNDL(hPlayer);
    
    uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
    IS_VALID_PLAYER_INDX(uPlayerIndx);
    
    Retcode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
    if(Retcode != CNXT_STATUS_OK)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
        return CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
    }
    
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERClear \n");
    
    do
    {
        
		pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
		if(pPlayerSubsystem->ePlayerState != E_PLAYER_START)
		{
			eErrCode = CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
			CSDEBUG(MODULE_NAME, ERROR_LEVEL," CSUDIPePlayerState not start or pause,do not need to clear\n");
			break;
		}

		hPipeObj = pPlayerSubsystem->pPipeObj;
		hDemuxObj = pPlayerSubsystem->pDemuxObj;
		hVideoObj = pPlayerSubsystem->pVideoObj;
		hAudioObj = pPlayerSubsystem->pAudioObj;
        if(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)
        {
            TM_AOUTSetMute((EM_UDIAOUT_RCA|EM_UDIAOUT_SPDIF|EM_UDIAOUT_HDMI),CSUDI_TRUE);
            if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
            {
                hAudioObj->mute(hAudioObj, TRUE);
            }
            if (hPipeObj->set_decoding_speed( hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 0 ) 
                                      != PIPE_STATUS_OK )
            {
               CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Failed to set decoding speed.line[%d]\n",__LINE__);
               eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
               break;
            }    
            
            cnxt_kal_thread_time_sleep(50);
            if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))         
            {
               if(hAudioObj->decoder_ctrl ( hAudioObj, FALSE ) != PIPE_STATUS_OK )
               {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL," Audio decoder stop fail\n");
                    eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
                    break;
               }     
            }
            
            if((hVideoObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
            {
               if(hVideoObj->decoder_ctrl ( hVideoObj, FALSE )!= PIPE_STATUS_OK )
               {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL," Video decoder stop fail\n");
                    eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
                    break;
               }    
            }
            
            if(hDemuxObj != NULL)
            {           
               if(hDemuxObj->demux_play_ctrl ( hDemuxObj, FALSE )!= PIPE_STATUS_OK )
               {
					 CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR line=%d %d \n", __LINE__);
                     eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
                     break;
               }     
            }
            
            /* flush the pipeline */
            if(hPipeObj != NULL)
            {
                if(hPipeObj->flush(hPipeObj,PIPE_FULL_FLUSH)!= PIPE_STATUS_OK )
               {
                   CSDEBUG(MODULE_NAME, ERROR_LEVEL,"can't flush the pipe line\n" );
                   eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
                   break;
               }
            }
            cnxt_kal_thread_time_sleep(50);
            
            if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
            {
               if(hAudioObj->decoder_ctrl ( hAudioObj, TRUE )!= PIPE_STATUS_OK )
               {
                   CSDEBUG(MODULE_NAME, ERROR_LEVEL," Audio decoder start fail\n");
                   eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
                   break;
               }     
               hAudioObj->mute(hAudioObj, TRUE);
            }
            
             if((hVideoObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uVidPID)))
            {
               if(hVideoObj->decoder_ctrl ( hVideoObj, TRUE )!= PIPE_STATUS_OK )
               {
                   CSDEBUG(MODULE_NAME, ERROR_LEVEL," video decoder start fail\n");
                   eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
                   break;
               }    
            }
            
            if(hDemuxObj != NULL)
            {           
               if(hDemuxObj->demux_play_ctrl (hDemuxObj, TRUE )!= PIPE_STATUS_OK )
               {
                     CSDEBUG(MODULE_NAME, ERROR_LEVEL," demux ctrl fail\n");
                     eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
                     break;
               }     
            }
            if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
            {
                hAudioObj->mute(hAudioObj, TRUE);
            }
            if(hPipeObj != NULL)
            {
                if (hPipeObj->set_decoding_speed ( hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 1024 ) != PIPE_STATUS_OK )
                {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Failed to set decoding speed.line[%d]\n",__LINE__);
                    eErrCode=CSUDIPLAYER_ERROR_INVALID_STATUS;
                    break;
                }       
            }
            
            if((hAudioObj != NULL)&&(IS_VALID_PID(pPlayerSubsystem->uAudPID)))
            {
               
               //set mute or mute here                    
               if(gTmPipeObject.TridCoshipSetup[0].AudioMute == 1)
               {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line: %d MUTE......\n",__FUNCTION__,__LINE__);
                   hAudioObj->mute(hAudioObj, TRUE);
               }
               else
               {
                   CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line: %d UNMUTE......\n",__FUNCTION__,__LINE__);
                   hAudioObj->mute(hAudioObj, FALSE);
               }
               
            }
            if(gTmPipeObject.TridCoshipSetup[0].AoutRCAmute == 0)
            {
                TM_AOUTSetMute(EM_UDIAOUT_RCA,CSUDI_FALSE);
            }
            if(gTmPipeObject.TridCoshipSetup[0].Aoutspdifmute == 0)
            {
                TM_AOUTSetMute(EM_UDIAOUT_SPDIF,CSUDI_FALSE);
            }
            if(gTmPipeObject.TridCoshipSetup[0].AoutHDMImute == 0)
            {
                TM_AOUTSetMute(EM_UDIAOUT_HDMI,CSUDI_FALSE);
            }
            
        }
        
    }while(0);
    Retcode = cnxt_kal_sem_put(gPlayerSem);
    if(Retcode != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
        return CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
    }

    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: CSUDIPLAYERClear %d\n", eErrCode);
    return eErrCode;
}


/**
@brief 设置播放速率

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] eSpeed 要设置的速率模式,详见CSUDIPlayerSpeed_E定义
@return 
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 该接口对直播节目无效，返回CSUDI_ERROR_PLAYER_NOTSUPPORT（不支持此操作）
@note 该参数修改后立即生效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetSpeed(CSUDI_HANDLE  hPlayer, CSUDIPlayerSpeed_E eSpeed)
{
	//int32       nVidIndex=0,nDmxIndex=0, nAudIndex=0;
	int uPlayerIndx;
	CNXT_STATUS Retcode;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERSetSpeed 0x%x speed %d\n", hPlayer,eSpeed);
	IS_VALID_PLAYER_HNDL(hPlayer);
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);

	Retcode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
	
	if(pPlayerSubsystem->ePlayerState < E_PLAYER_READY)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"FAIL: CSUDIPLAYERSetSpeed state %d\n", pPlayerSubsystem->ePlayerState);
		Retcode = cnxt_kal_sem_put(gPlayerSem);
		return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
	}
	if((pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)&&
			((pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_PCM)||
				(pPlayerSubsystem->StreamType == EM_UDI_AUD_STREAM_LPCM)))
	{
	}
	else if(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_INJECT)
	{
	}
	
	Retcode = cnxt_kal_sem_put(gPlayerSem);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: CSUDIPLAYERSetSpeed 0x%x eSpeed %d\n", hPlayer, eSpeed);
	return CSUDI_SUCCESS;
}

/**
@brief 得到播放速率

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[out] peSpeed 要得到的速率模式,详见CSUDIPlayerSpeed_E定义
@return 
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 该接口对直播节目无效，返回CSUDI_ERROR_PLAYER_NOTSUPPORT（不支持此操作）

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetSpeed(CSUDI_HANDLE  hPlayer, CSUDIPlayerSpeed_E *peSpeed)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 设置播放位置

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] nPosInSec 偏移量
@param[in] ePlayPosFlag 偏移的相对位置
@return 	
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 只有回放文件和Tshift播放可以设置播放位置，Tshift播放的起始位置为有效可播放的起始位置（具体解释见CSUDIPLAYERGetCurPosInSec的注释）

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSeek(CSUDI_HANDLE  hPlayer,const int nPosInSec, const CSUDIPlayPosition_E ePlayPosFlag)
{     //int32       nVidIndex=0,nDmxIndex=0, nAudIndex=0;
	int uPlayerIndx;
	CS_TM_Player_SubSystem *pPlayerSubsystem;
	UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERSeek 0x%x nPos %d\n", hPlayer,nPosInSec);
	IS_VALID_PLAYER_HNDL(hPlayer);
	uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
	IS_VALID_PLAYER_INDX(uPlayerIndx);
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
	
	if(pPlayerSubsystem->ePlayerState!= E_PLAYER_START)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"FAIL: CSUDIPLAYERSeek state %d\n", pPlayerSubsystem->ePlayerState);
		return CSUDIPLAYER_ERROR_INVALID_STATUS;
	}
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERSeek 0x%x\n", hPlayer);
	return CSUDI_SUCCESS;
}


/**
@brief 获取回放文件/Tshift播放的当前播放位置的绝对时长。

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[out] pnPosInSec  存放当前播放位置(单位:秒)
@return  
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 在非"回放文件/Tshift播放"的情况下,该接口的行为未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetCurPosInSec(CSUDI_HANDLE  hPlayer, int * pnPosInSec)
{
    CNXT_STATUS Retcode;
    CS_TM_Player_SubSystem *pPlayerSubsystem;
    int uPlayerIndx;
    UDI_PLAYER_HNDL *pUDIPlayerHndl = (UDI_PLAYER_HNDL *)hPlayer;
    CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;

    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"START: CSUDIPLAYERGetCurPosInSec 0x%x\n", hPlayer);
    
    IS_VALID_PLAYER_HNDL(hPlayer);
    
    uPlayerIndx = pUDIPlayerHndl->uPlayerindx;
    IS_VALID_PLAYER_INDX(uPlayerIndx);
    

	if(pnPosInSec == NULL)
	{
		return CSUDIPLAYER_ERROR_BAD_PARAMETER;
	}
    
	pPlayerSubsystem = &gPlayerConfig.hPlayerHandle[uPlayerIndx];
    
    if(pPlayerSubsystem->PlayerType == EM_UDIPLAYER_LIVE)
    {
        return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED;
    }
    Retcode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
    if(Retcode != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
        return CSUDIPLAYER_ERROR_BAD_PARAMETER;
    }
    do
    {        
    }while(0);
    Retcode = cnxt_kal_sem_put(gPlayerSem);
    if(Retcode != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, DEBUG_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
        return CSUDIPLAYER_ERROR_BAD_PARAMETER;
    }
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: CSUDIPLAYERGetCurPosInSecs 0x%x\n", hPlayer);
    return eErrCode;
}

/**
@brief 获取当前正在播放本地文件的节目总时长

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[out] pnSeconds 节目总时长,单位秒
@return  
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
         
@note 本接口只针对UDI播放视频文件有效,其它实时流或驱动不支持的视频文件pdwSeconds为0，返回值为CSUDI_ERROR_PLAYER_NOTSUPPORT
      对于普通的TS文件，如果是走Inject注入流程，也可以不用返回节目时长

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetDuration(CSUDI_HANDLE  hPlayer, int *pnSeconds)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 获取当前正在播放(显示)的码流PTS信息。

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] eContentType 流类型 详见CSUDIContentType_E相关定义
@param[out] psPTS 用于存放当前正在播放(显示)的码流PTS值,该值33bit。
@return  
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 这里的PCR作为STC

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetCurPTS(CSUDI_HANDLE  hPlayer, CSUDIContentType_E eContentType, CSUDI_UINT64 *psPTS)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 获取当前正在播放节目指定流类型的packet个数。

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] eContentType 当前正在播放的流类型详见CSUDIContentType_E
@param[out] puPacketCnt 存放packet包个数,指单个PID类型的包个数
@return  
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
         
@note 本接口主要用于判断前端码流是否停播

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetPacketCount(CSUDI_HANDLE  hPlayer, CSUDIContentType_E eContentType, unsigned int * puPacketCnt)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 进入退出TrickMode

通知驱动层后续音视频可能会按非正常速度进入解码器。
常用于VOD播放时的快退快进等，这种情况下Tuner过来的数据为非正常速度，
平台需做好准备以防止过度阶段及播放阶段的马赛克。
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] bTrickMode 为CSUDI_TRUE表示进入TrickMode,为CSUDI_FALSE表示退出TrickMode
@return
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 已经在TrickMode时再次进入TrickMode或者已经在非TrickMode时再次退出TrickMode返回CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
@note 回放文件和Tshift播放由于控制流程在底层处理，因此不需要调用本接口。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERTrickMode(CSUDI_HANDLE hPlayer,CSUDI_BOOL bTrickMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 进入退出快速输出模式
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] bQuickOutputMode 为CSUDI_TRUE表示进入QuickOutputMode,为CSUDI_FALSE表示退出QuickOutputMode
@return
	- 成功返回CSUDI_SUCCESS；
	- 失败则返回CSUDI_FAILURE；
@note 进入快速输出模式，解码器需要做特殊处理
          例如: 1、关闭隔行算法
                2、包溢出时进行丢弃
                3、按解码顺序输出
@note 该接口主要用于对低延时需求特别高的应用场景
          例如: 1、永新云游戏，云视频业务
@note 该接口需要在播放器非运行状态调用
          例如: 1、open之后start之前
                2、stop之后close之前

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERQuickOutputMode(CSUDI_HANDLE hPlayer,CSUDI_BOOL bQuickOutputMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 设置player的音视频同步模式
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] eAVSyncMode 为需要设置的同步模式，见CSUDIPLAYERAVSyncMode_E
@return
	- 成功返回CSUDI_SUCCESS；
	- 失败则返回CSUDI_FAILURE；
@note 如果不使用该接口，那么默认是采用PCR同步模式

@note 该接口需要在播放器非运行状态调用
          例如: 1、open之后start之前
                2、stop之后close之前

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIPLAYERSetAVSyncMode(CSUDI_HANDLE hPlayer,CSUDIPLAYERAVSyncMode_E eAVSyncMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 获取player的音视频同步模式
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[out] peAVSyncMode 为保存同步模式的指针，见CSUDIPLAYERAVSyncMode_E
@return
	- 成功返回CSUDI_SUCCESS；
	- 失败则返回CSUDI_FAILURE；

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIPLAYERGetAVSyncMode(CSUDI_HANDLE hPlayer,CSUDIPLAYERAVSyncMode_E *peAVSyncMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

