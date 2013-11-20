/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_inject.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_inject.h"
#include "generic_include.h"

#define MODULE_NAME              ("CS_INJECT")

#define IS_VALID_DEMUX(dmx) (((dmx < -1)||(dmx >= MAX_DEMUX_OBJECTS))?FALSE:TRUE)
#define IS_VALID_AUDIO(aud) (((aud < -1)||(aud >= MAX_AUDIO_OBJECTS))?FALSE:TRUE)
#define IS_VALID_VIDEO(vid) (((vid < -1)||(vid >= MAX_VIDEO_OBJECTS))?FALSE:TRUE)

#define MAX_TS_BUF_LEN 512*188
static CNXT_SEM_ID gInjecterSem;

#ifdef DUAL_VIDEO_SURFACE
CNXT_IMAGE_HANDLE gStillImageHandle=NULL;
bool bInjectPlayIframe = FALSE;
bool bFinishedIframeDec = FALSE;
#else
CNXT_IMAGE_HANDLE gStillImageHandle;
#endif

#ifdef DUAL_VIDEO_SURFACE
void cs_tm_display_update(void);
#ifdef STATIC_IFRAME_DECODE
bool cs_tm_iframe_release(void);
bool cs_tm_iframe_start(void);
bool cs_tm_iframe_stop(void);
#else
static bool cs_tm_iframe_start(CS_TM_Player_SubSystem *pPlayerSubsystem);
static bool cs_tm_iframe_stop(CS_TM_Player_SubSystem *pPlayerSubsystem);
#endif
#endif

#define IS_VALID_INJECTER(phndl)  \
	do                                                   \
	{                                                      \
		if( phndl == NULL )                     \
		{                                                \
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "%s 0x%x NULL HNDL\n", __FUNCTION__, phndl); \
			return CSUDIINJECTER_ERROR_INVALID_HANDLE; \
		}                                                       \
	} while( 0 )


bool bPlayerStarted = FALSE;
bool bPlayerStatus = FALSE;
bool bInjectIframe = FALSE;
u_int8 *pIframeBuff = NULL;
u_int32 uIframeLen=0;

CNXT_QUEUE_ID injectDataQueue=0x0ffff;
u_int8 *pInternalBuffer;


CS_CNXT_Inject_Config g_sCSInjectConfig;
static UDI_INJECT_HNDL s_aInjectHndl[MAX_INJECT_HANDLES];
static u_int32 tempsuccesscounter = 10;
static int gMinSendSize;

extern u_int32 cntDmxFilterInInjectMode;
extern PIPE_STATUS cs_pipe_vpm_attach(PIPE_VP_SURFACE_INPUT Input,PIPE_VP_SURFACE_TYPE uSurfaceNum, int PipeIndx);
extern CNXT_SEM_ID gPlayerSem;

#if 1
#define IPVOD_FZ_SIZE (1316*100)
#define IPVOD_BUF_SIZE (IPVOD_FZ_SIZE*50)
#define IPVOD_DECBUF_SIZE_FOR_PLAY (IPVOD_FZ_SIZE*30)

#define MAX_SEND_SIZE (IPVOD_FZ_SIZE)
#define MIN_SEND_SIZE (IPVOD_FZ_SIZE/10)
static int gMinSendSize;
u_int32 total_size;
bool gFristFlag = 1;

static char IPVOD_buf[IPVOD_BUF_SIZE];
static int IPVOD_buf_rpos=0,IPVOD_buf_wpos=0,IPVOD_buf_used=0,IPVOD_buf_count=0;





int cs_tm_get_free_inject_handle(void)
{
	int i=0;
	for(i=0; i<MAX_INJECT_HANDLES;i++)
	{
		if(g_sCSInjectConfig.hInjectHandle[i].bInUse == FALSE)
		{
			break;
		}
	}
	return i;
}

void IPVOD_buf_reset(void)
{
	printf("IPVOD_buf_reset used=%d\n",IPVOD_buf_used);
	IPVOD_buf_used = 0;	
	IPVOD_buf_rpos = 0;
	IPVOD_buf_wpos = 0;
	IPVOD_buf_count = 0;
}

void IPVOD_buf_append(u_int8 *p,unsigned int len)
{
	if(len + IPVOD_buf_used >= IPVOD_BUF_SIZE){
		return;
	}
	
	if(IPVOD_BUF_SIZE - IPVOD_buf_wpos < len){
		memcpy(IPVOD_buf,IPVOD_buf+IPVOD_buf_rpos,IPVOD_buf_used);
		IPVOD_buf_rpos = 0;
		IPVOD_buf_wpos = IPVOD_buf_used;
	}

	memcpy(IPVOD_buf + IPVOD_buf_wpos,p,len);
	
	IPVOD_buf_used += len;
	IPVOD_buf_wpos += len;
	IPVOD_buf_count += len;
}

static char* IPVOD_buf_read(int len)
{
	char *ret = IPVOD_buf+IPVOD_buf_rpos;

	IPVOD_buf_used -= len;
	if(IPVOD_buf_used == 0){
		IPVOD_buf_rpos = 0;
		IPVOD_buf_wpos = 0;
	}else{
		IPVOD_buf_rpos += len;
	}	
	return ret;
}

static void IPVOD_adapter_reset()
{
	IPVOD_buf_reset();
}

static int check_es_buf( CSUDI_HANDLE hINJECTER )
{
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
	int nInjectIndx;
	CS_CNXT_Inject_SubSystem *pInjectHandle;
    
	IS_VALID_INJECTER(hINJECTER);
	
	nInjectIndx = phInjectHndl->uIndx;
	pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];

	PIPE_AUDIO_OBJECT	*pAudioObj = NULL;
	PIPE_AUDIO_ES_BUFFER_ATTRIB AudioEsBuff;

	PIPE_STATUS PmStatus;
	pAudioObj = pInjectHandle->pAudObject;
				
	if(pAudioObj != NULL)
	{
		PmStatus = pAudioObj->get_es_buffer_attrib(pAudioObj, &AudioEsBuff);
		if(PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL," something serious \n");
		}
	}

	if((AudioEsBuff.uUsedSpace >= (AudioEsBuff.uSize * 4)/5))
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL," AudioEsBuff.uSize = %d\n\r", AudioEsBuff.uSize);
		return 0;
	}
	else
	{
		return 1;
	}
	
}
#endif

#ifdef DUAL_VIDEO_SURFACE
extern void cs_tm_notify_player(CSUDIPlayerEventType_E UDIEvent);
void cs_tm_playback_notify( PIPE_PIPELINE_OBJECT *pPipeline,
							void                     *pUserData,
							PIPE_NOTIFY_EVENT         Event,
							void                     *pData,
							void                     *pTag )

{
	u_int32 uMsg[4];

	printf("\n-----DUAL_VIDEO_SURFACE----[%lx]----------\n\n",Event);

	switch(Event)
	{		
		case PIPE_EVENT_PIPELINE_REQ_CONT_BLK_FROM_T:
			cnxt_kal_qu_send(injectDataQueue, uMsg);
                    break;
		case PIPE_EVENT_PIPELINE_REQ_NEXT_CONT_BLK:							
			cnxt_kal_qu_send(injectDataQueue, uMsg);
			break;
		case PIPE_EVENT_PIPELINE_MEDIA_DATA_CONSUMED:
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_PIPELINE_MEDIA_DATA_CONSUMED \n");	         
			break;
		case PIPE_EVENT_VIDEO_DECODE_COMPLETE:       
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"[%s]PIPE_EVENT_VIDEO_DECODE_COMPLETE \n",__FUNCTION__);			
			bFinishedIframeDec = TRUE;
			cs_tm_display_update();		
            cs_tm_notify_player(EM_UDIPLAYER_VIDEO_FRAME_COMING);	
			break;
		case PIPE_EVENT_PIPELINE_STATE_CHANGED:
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_PIPELINE_STATE_CHANGED \n");
			break;
		case PIPE_EVENT_VIDEO_STARTED :          
			break;
		case PIPE_EVENT_VIDEO_DISPLAY_UPDATE :		
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"[%s]PIPE_EVENT_VIDEO_DISPLAY_UPDATE \n",__FUNCTION__);	
			cs_tm_display_update();		
            cs_tm_notify_player(EM_UDIPLAYER_VIDEO_FRAME_COMING);	
			break;
		case PIPE_EVENT_VIDEO_PLAYING :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_PLAYING \n");
			break;
		case PIPE_EVENT_VIDEO_BUFFER_EMPTY:
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_BUFFER_EMPTY \n");
			break;
		case PIPE_EVENT_AUDIO_STOPPED :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STOPPED \n");
			break;
		case PIPE_EVENT_AUDIO_STARTED :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STARTED \n");
			break;
		case PIPE_EVENT_AUDIO_STREAM_LOST :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STREAM_LOST \n");
			break;
		case PIPE_EVENT_AUDIO_SYNC_ACQ :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_ACQ \n");
			break;
		case PIPE_EVENT_AUDIO_SYNC_LOST :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_LOST \n");
			break;
		case PIPE_EVENT_AUDIO_BUFFER_EMPTY :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_BUFFER_EMPTY \n");
			break;
		case PIPE_EVENT_VIDEO_STREAM_CHANGE:
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_STREAM_CHANGE \n");
			break;
		case PIPE_EVENT_DEMUX_FILTER_DATA:
			Section_filter_event_notifier(pPipeline,pUserData,Event,pData,pTag);
			break;
		default:
			break;
	}
}


void cs_tm_display_update()
{
	PIPE_VP_SURFACE_ATTRIB sPriSDAttrib;
	PIPE_VP_SURFACE_ATTRIB sSecSDAttrib;

	PIPE_VP_SURFACE_ATTRIB sPriHDAttrib;
	PIPE_VP_SURFACE_ATTRIB sSecHDAttrib;

	PIPE_VP_SURFACE_OBJECT *spPriSDSurface;
	PIPE_VP_SURFACE_OBJECT *spSecSDSurface;
	PIPE_VP_SURFACE_OBJECT *spPriHDSurface;
	PIPE_VP_SURFACE_OBJECT *spSecHDSurface;

	do
	{
		if((gTmVpDeviceObj.bShowVideo[0])==FALSE)
		{
			printf("---------DISPLAY UPDATE----------DO NOTHING------\n");
			
			break;
		}
		
		////////////////////////////////////////////////////////////////////////////
		/// Get surface attrib
        
		cnxt_kal_memset(&sPriSDAttrib,0,sizeof(sPriSDAttrib));	 
		cnxt_kal_memset(&sSecSDAttrib,0,sizeof(sSecSDAttrib));	   	
		cnxt_kal_memset(&sPriHDAttrib,0,sizeof(sPriHDAttrib));	   	
		cnxt_kal_memset(&sSecHDAttrib,0,sizeof(sSecHDAttrib));	   	

		spPriSDSurface = gTmVpDeviceObj.pSDVideoSurface[PIPE_VP_VIDEO_PRIMARY_SURFACE];
		spSecSDSurface = gTmVpDeviceObj.pSDVideoSurface[PIPE_VP_VIDEO_SECONDARY_SURFACE];
		spPriHDSurface = gTmVpDeviceObj.pHDVideoSurface[PIPE_VP_VIDEO_PRIMARY_SURFACE];
		spSecHDSurface = gTmVpDeviceObj.pHDVideoSurface[PIPE_VP_VIDEO_SECONDARY_SURFACE];

		if((spPriSDSurface== NULL)||(spSecSDSurface== NULL)||(spPriHDSurface== NULL)||(spSecHDSurface== NULL))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
			break;
		}

		if (PIPE_STATUS_OK!=(spPriSDSurface->get_attrib(spPriSDSurface,&sPriSDAttrib)))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
			break;
		}
		
		if (PIPE_STATUS_OK!=(spSecSDSurface->get_attrib(spSecSDSurface,&sSecSDAttrib)))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
			break;
		}
		
		if (PIPE_STATUS_OK!=(spPriHDSurface->get_attrib(spPriHDSurface,&sPriHDAttrib)))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
			break;
		}
		
		if (PIPE_STATUS_OK!=(spSecHDSurface->get_attrib(spSecHDSurface,&sSecHDAttrib)))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
			break;
		}




		////////////////////////////////////////////////////////////////////////////
		/// Control video surface according to the attach point. 

		if(gTmVpDeviceObj.eImgattachpoint ==  CNXT_VIDEO_IMG_ATTACH )
		{

            if((sPriSDAttrib.bSurfaceEnabled)&&
                (sPriHDAttrib.bSurfaceEnabled)&&
                !(sSecSDAttrib.bSurfaceEnabled)&&
                !(sSecHDAttrib.bSurfaceEnabled))
            {
                break;
            }
		    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n-####--DISPLAY UPDATE----ATTACH:%d == SHOW: %lx-::--LIVE VIDEO MODE---\n",
		                    gTmVpDeviceObj.eImgattachpoint,
		                    (gTmVpDeviceObj.bShowVideo[0]));     


			if((sPriSDAttrib.Input.Type != PIPE_VP_SURFACE_INPUT_TYPE_NONE)&&
				(sPriHDAttrib.Input.Type != PIPE_VP_SURFACE_INPUT_TYPE_NONE))
			{

				if (PIPE_STATUS_OK!=(spPriSDSurface->surface_enable(spPriSDSurface)))
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}
				
				if (PIPE_STATUS_OK!=(spPriHDSurface->surface_enable(spPriHDSurface)))
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}
			}

			if((sSecHDAttrib.Input.Type != PIPE_VP_SURFACE_INPUT_TYPE_NONE)&&
				(sSecSDAttrib.Input.Type != PIPE_VP_SURFACE_INPUT_TYPE_NONE))
			{
				if (PIPE_STATUS_OK!=(spSecSDSurface->surface_disable(spSecSDSurface)))
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}
				
				if (PIPE_STATUS_OK!=(spSecHDSurface->surface_disable(spSecHDSurface)))
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}
			}		
			
		}
		else if(gTmVpDeviceObj.eImgattachpoint ==  CNXT_STILL_IMG_ATTACH)
		{
            if(!(sPriSDAttrib.bSurfaceEnabled)&&
                !(sPriHDAttrib.bSurfaceEnabled)&&
                (sSecSDAttrib.bSurfaceEnabled)&&
                (sSecHDAttrib.bSurfaceEnabled))
            {
                break;
            }
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n---DISPLAY UPDATE----ATTACH:%d == SHOW: %lx-::--I FRAME STILL MODE---\n",
		                    gTmVpDeviceObj.eImgattachpoint,
		                    (gTmVpDeviceObj.bShowVideo[0]));     

			if((sSecHDAttrib.Input.Type != PIPE_VP_SURFACE_INPUT_TYPE_NONE)&&
				(sSecSDAttrib.Input.Type != PIPE_VP_SURFACE_INPUT_TYPE_NONE))
			{
			
				if (PIPE_STATUS_OK!=(spSecSDSurface->surface_enable(spSecSDSurface)))
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}
				
				if (PIPE_STATUS_OK!=(spSecHDSurface->surface_enable(spSecHDSurface)))
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}
			}
			
			if((sPriSDAttrib.Input.Type != PIPE_VP_SURFACE_INPUT_TYPE_NONE)&&
				(sPriHDAttrib.Input.Type != PIPE_VP_SURFACE_INPUT_TYPE_NONE))
			{
				if (PIPE_STATUS_OK!=(spPriSDSurface->surface_disable(spPriSDSurface)))
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}
				
				if (PIPE_STATUS_OK!=(spPriHDSurface->surface_disable(spPriHDSurface)))
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					break;
				}	
			}
		}
		else
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n---------DISPLAY UPDATE----------SURFACE IS IN IDLE MODE!------\n");
		}         
	}while(0);
	
	//cs_tm_notify_player(EM_UDIPLAYER_VIDEO_FRAME_COMING);	
	
}

#else
void cs_tm_playback_notify( PIPE_PIPELINE_OBJECT *pPipeline,
							void                     *pUserData,
							PIPE_NOTIFY_EVENT         Event,
							void                     *pData,
							void                     *pTag )

{
    CSUDIPlayerEventType_E UDIEvent;
	u_int32 uMsg[4];
	//CS_CNXT_Inject_SubSystem *pInjectHandle = (CS_CNXT_Inject_SubSystem *)pUserData;

	printf("\n---NO --DUAL_VIDEO_SURFACE----[%lx]----------\n\n",Event);

	switch(Event)
	{
	
	case PIPE_EVENT_PIPELINE_REQ_CONT_BLK_FROM_T:
	case PIPE_EVENT_PIPELINE_REQ_NEXT_CONT_BLK:
		cnxt_kal_qu_send(injectDataQueue, uMsg);
		break;
	case PIPE_EVENT_PIPELINE_MEDIA_DATA_CONSUMED:
		break;
	case PIPE_EVENT_VIDEO_DECODE_COMPLETE:
		// 2011-01-13 TerenceZhang begin:removed here to let the application know that the decoding complete.
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_VIDEO_DECODE_COMPLETE \n");	   
		UDIEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING;
		cs_tm_notify_player(UDIEvent);	
		// 2011-01-13 TerenceZhang end		
		break;
	case PIPE_EVENT_PIPELINE_STATE_CHANGED:
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_PIPELINE_STATE_CHANGED \n");
		break;

	case PIPE_EVENT_VIDEO_STARTED :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_STARTED \n");       
        UDIEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING;
		//printf("notify user msg:%d from playback\n",UDIEvent);
		/*lichanghua cancel :去掉解决IPTV TS注入闪上一帧问题*/
		//cs_tm_notify_player(UDIEvent);
		break;
	case PIPE_EVENT_VIDEO_DISPLAY_UPDATE :
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_DISPLAY_UPDATE \n"); 
		cs_tm_notify_player(UDIEvent);	
		break;
	case PIPE_EVENT_VIDEO_PLAYING :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_PLAYING \n");
		break;
	case PIPE_EVENT_VIDEO_BUFFER_EMPTY:
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_BUFFER_EMPTY \n");
		break;
	case PIPE_EVENT_AUDIO_STOPPED :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STOPPED \n");
		break;
	case PIPE_EVENT_AUDIO_STARTED :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STARTED \n");
		break;
	case PIPE_EVENT_AUDIO_STREAM_LOST :
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STREAM_LOST \n");
		break;
	case PIPE_EVENT_AUDIO_SYNC_ACQ :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_ACQ \n");
		break;
	case PIPE_EVENT_AUDIO_SYNC_LOST :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_LOST \n");
		break;
	case PIPE_EVENT_AUDIO_BUFFER_EMPTY :
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_BUFFER_EMPTY \n");
		break;
	case PIPE_EVENT_VIDEO_STREAM_CHANGE:
		{
#if 0            
			PIPE_VIDEO_ATTRIB   Attrib;
			printf("PIPE_EVENT_VIDEO_STREAM_CHANGE");
			gTridPipeObject.hVideoObj[i]->get_attrib(gTridPipeObject.hVideoObj[i],&Attrib);               
			printf("Attrib[0].PicInfo.AspectRatio=%d\r\n", Attrib.PicInfo.AspectRatio );
			gTridPipeObject.TridVideoSubSystem[i].PictureInfo.AspectRatio = Attrib.PicInfo.AspectRatio;
#endif
		}
		break;
#if 0        
	case PIPE_EVENT_DEMUX_SYNC_ACQUIRED:
	case PIPE_EVENT_DEMUX_SYNC_LOST:
	case PIPE_EVENT_DEMUX_HDR_ERROR:
	case PIPE_EVENT_DEMUX_FILTER_DATA:
	case PIPE_EVENT_DEMUX_FILTER_BUF_OVERFLOW:
	case PIPE_EVENT_DEMUX_FILTER_ERROR:
		
            //printf("PIPE_EVENT_DEMUX_FILTER \n");
		Section_filter_event_notifier(pPipe,pUserData,Event,pData,pTag);
#endif            
	case PIPE_EVENT_DEMUX_FILTER_DATA:
		Section_filter_event_notifier(pPipeline,pUserData,Event,pData,pTag);
		break;


	default:
		break;
	}
        //cs_tm_notify_player(UDIEvent);
}
#endif

CNXT_STATUS cs_tm_inject_init(void)
{
	CNXT_STATUS Retcode = CNXT_STATUS_OK;
	char sem_name[20];
	int count;
	CNXT_IMAGE_CAPS          imageCaps;
#ifdef DUAL_VIDEO_SURFACE
	PIPE_VP_SURFACE_INPUT Input;
    
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;	
#endif

	sprintf(sem_name,"UDIInjectSem");
	Retcode=cnxt_kal_sem_create(1, sem_name, &gInjecterSem);
	if(CNXT_STATUS_OK!=Retcode)
	{
		return Retcode;
	}
#if 0	
	Retcode = cnxt_kal_qu_create(100, NULL, &injectDataQueue);
	if(Retcode != CNXT_STATUS_OK)
	{
	   printf(" Queue creation failed...\n\r" );
	}
#endif
	for(count =0;count<MAX_INJECT_HANDLES;count++)
	{
		s_aInjectHndl[count].uIndx = 0xFF;
		s_aInjectHndl[count].bInjecterOpen = FALSE;
	}

    if(FALSE== CSPcmClip_Init(0))
    {
        return CNXT_STATUS_INTERNAL_ERROR;
    }
#ifdef EXTERNAL_IMAGE_ATTACH    
    {
        imageCaps.PalTable.ColorType      = YCC;
        imageCaps.PalTable.uNumPalEntries = 0;
        imageCaps.PalTable.pPalEntry      = NULL;
        imageCaps.PoolId[0]     = CNXT_POOL_ID_DEFAULT_UNCACHED; //;
        imageCaps.PoolId[1]     = CNXT_POOL_ID_DEFAULT_UNCACHED; //;
        imageCaps.pPage         = NULL;
        imageCaps.uOrgW         = 2048;
        imageCaps.uOrgH         = 1088;
        imageCaps.PixelMode     = PIXEL_FORMAT_NV12;
        imageCaps.uNumPages     = 1;
        imageCaps.uMaxNumPages  = imageCaps.uNumPages;
        imageCaps.ColorSpace    = YCC_HD_BT709;
        imageCaps.bCached       = FALSE;
        imageCaps.bIsContiguous = TRUE;
        imageCaps.ImageType     = CNXT_IMAGE_STILL_GRAPHICS;
        Retcode = cnxt_image_open(&gStillImageHandle, &imageCaps, NULL, NULL);
        if ( Retcode != CNXT_STATUS_OK )
        {
           CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to open image, err = %d\n", Retcode);
           return PIPE_STATUS_DRIVER_ERROR;
        }
     }
#endif        
#ifdef DUAL_VIDEO_SURFACE
	if(gStillImageHandle==NULL)
	{
		cnxt_kal_memset(&imageCaps,0,sizeof(imageCaps));

		imageCaps.PalTable.ColorType      = YCC;
		imageCaps.PalTable.uNumPalEntries = 0;
		imageCaps.PalTable.pPalEntry      = NULL;
		imageCaps.PoolId[0]     = CNXT_POOL_ID_DEFAULT_UNCACHED; //;
		imageCaps.PoolId[1]     = CNXT_POOL_ID_DEFAULT_UNCACHED; //;
		imageCaps.pPage         = NULL;
		imageCaps.uOrgW         = 2048;
		imageCaps.uOrgH         = 1088;
		imageCaps.PixelMode     = PIXEL_FORMAT_NV12;
		imageCaps.uNumPages     = 1;
		imageCaps.uMaxNumPages  = imageCaps.uNumPages;
		imageCaps.ColorSpace    = YCC_HD_BT709;
		imageCaps.bCached       = FALSE;
		imageCaps.bIsContiguous = TRUE;
		imageCaps.ImageType     = CNXT_IMAGE_STILL_GRAPHICS;
		Retcode = cnxt_image_open(&gStillImageHandle, &imageCaps, NULL, NULL);
		if ( Retcode != CNXT_STATUS_OK )
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to open image, err = %d\n", Retcode);
			return CNXT_STATUS_INTERNAL_ERROR;
		}
	}

	cnxt_kal_memset(&Input,0,sizeof(Input));

	Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_IMAGE;
	Input.pPipe = NULL;
	Input.hImageHandle = gStillImageHandle;

	PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_SECONDARY_SURFACE, NULL);
	if (PmStatus != PIPE_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to ATTACH image,err=%d\n", PmStatus);
		return CNXT_STATUS_INTERNAL_ERROR;
	}
#endif

    
    return Retcode;
}

int get_inject_handle_indx(int nAudIndx)
{
    int InjectIndx=0;
	CS_CNXT_Inject_SubSystem *pInjectHandle = NULL;
	for(InjectIndx=0;InjectIndx<MAX_INJECT_HANDLES;InjectIndx++)
    {
		pInjectHandle = &g_sCSInjectConfig.hInjectHandle[InjectIndx];
        if(pInjectHandle->nAudIndx == nAudIndx)
            break;
    }   
    return InjectIndx;
}

bool bIsInjecteropen(u_int32 hInjecter, const CSUDIINJECTERChnl_S * psInjecterChnl, 
                            const CSUDIINJECTEROpenParam_S * psOpenParams)
{
	int i=0;
	for(i=0;i<MAX_INJECT_HANDLES;i++)
	{        
		if(hInjecter == (u_int32)&s_aInjectHndl[i])
		{
			if(s_aInjectHndl[i].bInjecterOpen == TRUE)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Injecteropen hndl 0x%x 0x%x\n", hInjecter, &s_aInjectHndl[i]);
				return TRUE;
			}
		}	  
		if(s_aInjectHndl[i].bInjecterOpen == TRUE)
		{
			int InjectIndx=0;		
			CS_CNXT_Inject_SubSystem *pInjectHandle = NULL;
			InjectIndx = s_aInjectHndl[i].uIndx;
			pInjectHandle = &g_sCSInjectConfig.hInjectHandle[InjectIndx];
			if((pInjectHandle->nAudIndx == psInjecterChnl->m_nAudioDecoder )&&
					(pInjectHandle->nDmxIndx== psInjecterChnl->m_nDemux )&&
					(pInjectHandle->nVidIndx== psInjecterChnl->m_nVideoDecoder)&&
					(pInjectHandle->eContentType == psOpenParams->m_eContentType)&&
					(pInjectHandle->eInjecterType == psOpenParams->m_eInjecterType))
			{	
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Injecter Exists with same config \n");
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CSUDIINJECTERTerm()
{
	int count = 0;

	//printf("start %s\n",__FUNCTION__);
	
	for(count =0;count<MAX_INJECT_HANDLES;count++)
	{
		if (s_aInjectHndl[count].bInjecterOpen)
		{
			if (CSUDI_SUCCESS != CSUDIINJECTERClose(&s_aInjectHndl[count]))
			{
				CSDEBUG(MODULE_NAME,ERROR_LEVEL,"fail to close injecter\n");
				break;
			}
		}
	}

	//printf("exit %s\n",__FUNCTION__);

	return (count == MAX_INJECT_HANDLES);
}

#ifdef DUAL_VIDEO_SURFACE
#ifdef STATIC_IFRAME_DECODE
bool cs_tm_iframe_release(void)
{	
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;	
	
	PIPE_VP_SURFACE_INPUT Input;
	CNXT_IMAGE_CAPS          imageCaps;

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);

	if(gTmPipeObject.hVideoObj[1] == NULL)
	{
		gTmPipeObject.hVideoObj[1] = PIPE_OBJ_CREATE ( video ); 
		if(gTmPipeObject.hVideoObj[1] == NULL)
		{
			cnxt_kal_trace ( TL_ERROR, "Still decode: PIPE_OBJ_CREATE ( video ) FAIL\n");
			return FALSE;
		}

		if(gStillImageHandle!=NULL)
		{

			cnxt_kal_memset(&Input,0,sizeof(Input));
			Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_NONE;
			PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_SECONDARY_SURFACE, NULL);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to ATTACH image,err=%d\n", PmStatus);
				return FALSE;
			}
		
		
			if (CNXT_STATUS_OK != cnxt_image_close(gStillImageHandle))
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to open image\n");
				return FALSE;
			}
			
			gStillImageHandle = NULL;
		}


		cnxt_kal_memset(&imageCaps,0,sizeof(imageCaps));

		imageCaps.PalTable.ColorType      = YCC;
		imageCaps.PalTable.uNumPalEntries = 0;
		imageCaps.PalTable.pPalEntry      = NULL;
		imageCaps.PoolId[0]     = CNXT_POOL_ID_DEFAULT_UNCACHED;
		imageCaps.PoolId[1]     = CNXT_POOL_ID_DEFAULT_UNCACHED;
		imageCaps.pPage         = NULL;
		imageCaps.uOrgW         = 2048;
		imageCaps.uOrgH         = 1088;
		imageCaps.PixelMode     = PIXEL_FORMAT_NV12;
		imageCaps.uNumPages     = 1;
		imageCaps.uMaxNumPages  = imageCaps.uNumPages;
		imageCaps.ColorSpace    = YCC_HD_BT709;
		imageCaps.bCached       = FALSE;
		imageCaps.bIsContiguous = TRUE;
		imageCaps.ImageType     = CNXT_IMAGE_STILL_GRAPHICS;
		
		if (CNXT_STATUS_OK != cnxt_image_open(&gStillImageHandle, &imageCaps, NULL, NULL))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to open image\n");
			return FALSE;
		}

		//printf("###############gStillImageHandle =%lx###############\n",gStillImageHandle);
		cnxt_kal_memset(&Input,0,sizeof(Input));
		Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_IMAGE;
		Input.pPipe = NULL;
		Input.hImageHandle = gStillImageHandle;
		PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_SECONDARY_SURFACE, NULL);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to ATTACH image,err=%d\n", PmStatus);
			return FALSE;
		}
		
	}
	else
	{
		cs_tm_iframe_stop();
	}


	////////////////////////////////////////////////////////////////////////////
	/// Create pipeline releated objects.

	gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT] = PIPE_OBJ_CREATE(demux);
	if(gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT]== NULL )
	{
	      CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot create demux objects\n" );
	      return FALSE;
	}


	if(gTmPipeObject.hPipeObj[ES_VIDEO_OBJECT]== NULL )
	{
	      CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot create pipeline objects\n" );
	      return FALSE;
	}
		
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out: %s %d \n", __FUNCTION__, __LINE__);
	return TRUE;	
}


bool cs_tm_iframe_stop(void)
{
	PIPE_STATUS PmStatus = PIPE_STATUS_OK; 
	PIPE_PIPELINE_OBJECT *hPipeObj=NULL;
	PIPE_DEMUX_OBJECT    *pDmxObject=NULL;
	PIPE_VIDEO_OBJECT    *pVidObject=NULL;

	BOOL eErrCode=TRUE;


	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
	
	hPipeObj = gTmPipeObject.hPipeObj[ES_VIDEO_OBJECT];
	pDmxObject =  gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT];
	pVidObject =  gTmPipeObject.hVideoObj[1];

	do
	{

		if(gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT]==NULL)break;


		PmStatus = hPipeObj->delete_device(hPipeObj, (PIPE_OBJ_HANDLE)pVidObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			eErrCode = FALSE;
			break;
		}
	
		PmStatus = hPipeObj->delete_device(hPipeObj, (PIPE_OBJ_HANDLE)pDmxObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			eErrCode = FALSE;
			break;
		}

		
		PmStatus = PIPE_OBJ_DESTROY ( demux, pDmxObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			eErrCode = FALSE;
			break;
		}
	}while(0);
	
	gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT] = NULL;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s %d eErrCode %d \n", __FUNCTION__, __LINE__, eErrCode);   
	return eErrCode;
}

bool cs_tm_iframe_start(void)
{
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;

	PIPE_DEMUX_OBJECT        *pDmxObject=NULL;
	PIPE_VIDEO_OBJECT        *pVidObject=NULL;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_MEDIA_DATA_INFO MediaDataInfo;
	PIPE_PIPELINE_CFG PipeCfg;
	PIPE_DEMUX_CFG demuxConfig;
	PIPE_VIDEO_CFG VideoConfig;
	
	BOOL eErrCode=TRUE;

	u_int32   uEventMask        = 0;
    
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
    
	bFinishedIframeDec = FALSE;
	do
	{  	    
		hPipeObj = gTmPipeObject.hPipeObj[ES_VIDEO_OBJECT];
		pDmxObject = gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT];
		pVidObject = gTmPipeObject.hVideoObj[1];

		////////////////////////////////////////////////////////////////////////
		/// configure the pipeline

		/* config pipe */
		cnxt_kal_memset(&PipeCfg, 0, sizeof(PipeCfg));		
		PipeCfg.Type = PIPE_PIPELINE_NONE_TS_TYPE;
		PipeCfg.uStcIndex = 1;
		PipeCfg.bPipPipe = FALSE;
		PipeCfg.SyncMaster = PIPE_SYNC_NO_SYNC;

		PmStatus = hPipeObj->config(hPipeObj, &PipeCfg);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d %d \n", __FUNCTION__, __LINE__, PmStatus);
			eErrCode = FALSE;
			break;
		}

		/* config demux */
		cnxt_kal_memset(&demuxConfig, 0, sizeof(demuxConfig));
		demuxConfig.Type = PIPE_DEMUX_VIDEO_ES;
		demuxConfig.DescramblerType   = PIPE_DEMUX_DESCRAMBLE_NONE;

		PmStatus = pDmxObject->config(pDmxObject, &demuxConfig);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			eErrCode = FALSE;
			break;
		}

		/* config video */

		uEventMask = ((1 << (PIPE_EVENT_VIDEO_STARTED & 0xFFFFFF)) |
		          (1 << (PIPE_EVENT_VIDEO_PLAYING & 0xFFFFFF))|
		          (1 << (PIPE_EVENT_VIDEO_BUFFER_EMPTY & 0xFFFFFF))|                      
		          (1 << (PIPE_EVENT_VIDEO_DECODE_COMPLETE & 0xFFFFFF))|
		          (1<<(PIPE_EVENT_VIDEO_STREAM_CHANGE & 0xFFFFFF)));
		
		pVidObject->subscribe_event(pVidObject, uEventMask);                 

	 
		cnxt_kal_memset(&VideoConfig, 0, sizeof(VideoConfig));
		VideoConfig.DecodingType  =    PIPE_VIDEO_MOTION_STILL;
		VideoConfig.uCBuffSize    =    1024*1024 + 1; /*pInjectHandle->iFrameParams.m_nDataLen*/
		VideoConfig.uImageWidth   =    2048;
		VideoConfig.uImageHeight  =    1088;
		VideoConfig.Format        =    PIPE_VIDEO_FORMAT_MPEG2;
		VideoConfig.Definition    =    PIPE_VIDEO_HD;
		VideoConfig.bHwAccelerate   = TRUE;
		VideoConfig.bPrimaryDecoder = TRUE;
		VideoConfig.uUserDataEnableMap = 0;
		//VideoConfig.StillImageHandle = gStillImageHandle; //commented by frank.zhou
		PmStatus = pVidObject->config(pVidObject, &VideoConfig);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			eErrCode = FALSE;
			break;
		}

		cnxt_kal_thread_time_sleep(10);

		////////////////////////////////////////////////////////////////////////
		/// add device
		  
		PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)pDmxObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			eErrCode = FALSE;
			break;
		}
		PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)pVidObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			eErrCode = FALSE;
			break;
		}

		////////////////////////////////////////////////////////////////////////
		/// flush the resources and attach video decoding to his surface
		
		PmStatus = hPipeObj->flush(hPipeObj, PIPE_FULL_FLUSH);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			eErrCode = FALSE;
			break;
		}
        
		
		////////////////////////////////////////////////////////////////////////
		/// start decoder	  
		PmStatus = pDmxObject->demux_play_ctrl(pDmxObject, TRUE);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			eErrCode = FALSE;
			break;
		}
		PmStatus = pVidObject->decoder_ctrl(pVidObject, TRUE);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			eErrCode = FALSE;
			break;
		}

		PmStatus = hPipeObj->set_decoding_speed(hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 1024);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			eErrCode = FALSE;
			break;
		}

		////////////////////////////////////////////////////////////////////////
		/// inject media data  
		if(pIframeBuff == NULL)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			eErrCode = FALSE;
			break;
		}	 


#ifdef LOOPINJECT
        {
            int uNumWritten,uTotalWritten, uNumW,uNumLeft;
            uNumWritten = uTotalWritten = uNumW= uNumLeft =0;

            cnxt_kal_memset(&MediaDataInfo,0,sizeof(MediaDataInfo));
            MediaDataInfo.Type = PIPE_MEDIA_VIDEO_ES_DATA;
            MediaDataInfo.uOverlap = 0;
            MediaDataInfo.uPts	  = 0;

            uNumLeft =   uIframeLen;

            MediaDataInfo.pData  = pIframeBuff;

            do
            {

                    if(uNumLeft>=64*1024)
                    {
                        MediaDataInfo.uLength = 64*1024;
                    }
                    else
                    {
                        MediaDataInfo.uLength = uNumLeft+1;
                    }
                
                    PmStatus = hPipeObj->inject_media_data(hPipeObj, &MediaDataInfo);
                    if (PmStatus != PIPE_STATUS_OK)
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
                        eErrCode = FALSE;
                        break;
                    }
        

                    uNumLeft -=MediaDataInfo.uLength;

                    if(uNumLeft<=0)
                        break;

                    MediaDataInfo.pData +=MediaDataInfo.uLength;
                    cnxt_kal_thread_time_sleep(5);

            }while(uNumLeft>0);
        }

#else
		cnxt_kal_memset(&MediaDataInfo,0,sizeof(MediaDataInfo));

		MediaDataInfo.Type = PIPE_MEDIA_VIDEO_ES_DATA;
		MediaDataInfo.pData   = pIframeBuff;
		MediaDataInfo.uLength = uIframeLen+1;

        MediaDataInfo.uOverlap = 0;
        MediaDataInfo.uPts	  = 0;

		PmStatus = hPipeObj->inject_media_data(hPipeObj, &MediaDataInfo);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			eErrCode = FALSE;
			break;
		}
#endif
	  
	}while(0);
      
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s %d eErrCode %d \n", __FUNCTION__, __LINE__, eErrCode);
	return eErrCode;
}



bool CS_Open_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem, int nVidIndex)
{
	bool bRet =0;

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);  

	if(nVidIndex!=0)
	{
		CSDEBUG(MODULE_NAME,ERROR_LEVEL, "do not support video index malloc.\n" );
		return FALSE;
	}	

	bRet = cs_tm_iframe_release();
	if(bRet==FALSE)
	{
		CSDEBUG(MODULE_NAME,ERROR_LEVEL, "do not support video index malloc.\n" );
		return FALSE;
	}	

	pPlayerSubsystem->pPipeObj = gTmPipeObject.hPipeObj[ES_VIDEO_OBJECT];
	pPlayerSubsystem->pDemuxObj = gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT];
	pPlayerSubsystem->pVideoObj = gTmPipeObject.hVideoObj[1];

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out: %s %d \n", __FUNCTION__, __LINE__);  
	return bRet;
}

bool CS_Close_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{	
	//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);  
	//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out: %s %d \n", __FUNCTION__, __LINE__);  
	return TRUE;
}

bool CS_stop_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
	bool bRet =TRUE;
	PIPE_STATUS PmStatus = PIPE_STATUS_OK; 
	PIPE_DEMUX_OBJECT    *pDmxObject=NULL;
	PIPE_VIDEO_OBJECT    *pVidObject=NULL;

	PIPE_VIDEO_ATTRIB sAttrib;

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
	pDmxObject =  gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT];
	pVidObject =  gTmPipeObject.hVideoObj[1];

	do{
		
		PmStatus = pVidObject->get_attrib(pVidObject, &sAttrib);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			bRet = FALSE;
			break;
		}

		if(sAttrib.State!=PIPE_VIDEO_RUNNING)break;

		PmStatus = pVidObject->decoder_ctrl(pVidObject, FALSE);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			bRet = FALSE;
			break;
		}

		PmStatus = pDmxObject->demux_play_ctrl(pDmxObject, FALSE);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			bRet = FALSE;
			break;
		}

	}while(0);

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out: %s %d \n", __FUNCTION__, __LINE__);  
	return bRet;
}

bool CS_start_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
	bool bRet =0;
	int iTimeOutCnt =0;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);  
	
	bRet = cs_tm_iframe_start();
	
	while((bFinishedIframeDec==FALSE)&&((iTimeOutCnt++)<50))
	{
		cnxt_kal_thread_time_sleep(5);
	}
	if(iTimeOutCnt>=50)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"In: %s %d: I FRAME DECODE TIME OUT! \n", __FUNCTION__, __LINE__);  
		bRet = FALSE;		              
	}

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out: %s %d \n", __FUNCTION__, __LINE__);  
	
	return bRet;
}

#else  //#ifdef STATIC_IFRAME_DECODE
static bool cs_tm_iframe_stop(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
	PIPE_STATUS PmStatus = PIPE_STATUS_OK; 
	PIPE_PIPELINE_OBJECT *hPipeObj=NULL;
	PIPE_DEMUX_OBJECT    *pDmxObject=NULL;
	PIPE_VIDEO_OBJECT    *pVidObject=NULL;

	BOOL retCode=TRUE;
	int nVidIndx =0;

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
	
	pDmxObject = pPlayerSubsystem->pDemuxObj;	       
	pVidObject = pPlayerSubsystem->pVideoObj;
	hPipeObj = pPlayerSubsystem->pPipeObj;
	nVidIndx = pPlayerSubsystem->VideoIndx;

	do
	{
		PmStatus = pVidObject->decoder_ctrl(pVidObject, FALSE);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
            retCode = FALSE;
			break;
		}
		PmStatus = pDmxObject->demux_play_ctrl(pDmxObject, FALSE);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
            retCode = FALSE;
            break;
		} 

		PmStatus = hPipeObj->delete_device(hPipeObj, (PIPE_OBJ_HANDLE)pVidObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
            retCode = FALSE;
			break;
		}
		PmStatus = hPipeObj->delete_device(hPipeObj, (PIPE_OBJ_HANDLE)pDmxObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
            retCode = FALSE;
			break;
		}

	#ifdef VDOBJ_MALLOC

        printf("\n########DESTROY video obj###%lx##########\n",pVidObject);

        
		PmStatus = PIPE_OBJ_DESTROY(video,pVidObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
            retCode = FALSE;
			break;
		}

	#endif
		
		PmStatus = PIPE_OBJ_DESTROY ( demux, pDmxObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
            retCode = FALSE;
			break;
		}
	}while(0);

    #ifdef VDOBJ_MALLOC
	gTmPipeObject.hVideoObj[1] = NULL;
	#endif
	
	gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+nVidIndx] = NULL;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s %d \n", __FUNCTION__, __LINE__);   
	return retCode;
}

static bool cs_tm_iframe_start(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;
	PIPE_DEMUX_OBJECT        *pDmxObject=NULL;
	PIPE_VIDEO_OBJECT        *pVidObject=NULL;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_MEDIA_DATA_INFO MediaDataInfo;
	PIPE_PIPELINE_CFG PipeCfg;
	PIPE_DEMUX_CFG demuxConfig;
	PIPE_VIDEO_CFG VideoConfig;
	
	BOOL retCode=TRUE;
	int nVidIndx=0;
	u_int32  uEventMask = 0;
    
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
    
	nVidIndx = pPlayerSubsystem->VideoIndx;
	bFinishedIframeDec = FALSE;
	do
	{  	    
		////////////////////////////////////////////////////////////////////////////
		/// Create pipeline releated objects.

		gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+ nVidIndx] = PIPE_OBJ_CREATE(demux);
		if(gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+ nVidIndx]== NULL )
		{
		      CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot create demux objects\n" );
		      return FALSE;
		}

		#ifdef VDOBJ_MALLOC
		gTmPipeObject.hVideoObj[1] = PIPE_OBJ_CREATE ( video ); 
		if( gTmPipeObject.hVideoObj[1] == NULL )
		{
		      CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot create video objects\n" );
		      return FALSE;
		}

        printf("\n########create video obj###%lx##########\n",gTmPipeObject.hVideoObj[1]);
		#endif
		
		hPipeObj = pPlayerSubsystem->pPipeObj = gTmPipeObject.hPipeObj[ES_VIDEO_OBJECT+nVidIndx];
		pDmxObject = pPlayerSubsystem->pDemuxObj = gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+ nVidIndx];
		pVidObject = pPlayerSubsystem->pVideoObj = gTmPipeObject.hVideoObj[1];

		////////////////////////////////////////////////////////////////////////
		/// configure the pipeline

		/* config pipe */
		cnxt_kal_memset(&PipeCfg, 0, sizeof(PipeCfg));		
		PipeCfg.Type = PIPE_PIPELINE_NONE_TS_TYPE;
		PipeCfg.uStcIndex = 1;
		PipeCfg.bPipPipe = FALSE;
		PipeCfg.SyncMaster = PIPE_SYNC_NO_SYNC;

		PmStatus = hPipeObj->config(hPipeObj, &PipeCfg);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d %d \n", __FUNCTION__, __LINE__, PmStatus);
			retCode = FALSE;
			break;
		}

		/* config demux */
		cnxt_kal_memset(&demuxConfig, 0, sizeof(demuxConfig));
		demuxConfig.Type = PIPE_DEMUX_VIDEO_ES;
		demuxConfig.DescramblerType   = PIPE_DEMUX_DESCRAMBLE_NONE;

		PmStatus = pDmxObject->config(pDmxObject, &demuxConfig);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			retCode = FALSE;
			break;
		}

		/* config video */

		uEventMask = ((1 << (PIPE_EVENT_VIDEO_STARTED & 0xFFFFFF)) |
		          (1 << (PIPE_EVENT_VIDEO_PLAYING & 0xFFFFFF))|
		          (1 << (PIPE_EVENT_VIDEO_BUFFER_EMPTY & 0xFFFFFF))|                      
		          (1 << (PIPE_EVENT_VIDEO_DECODE_COMPLETE & 0xFFFFFF))|
		          (1<<PIPE_EVENT_VIDEO_STREAM_CHANGE & 0xFFFFFF));
		
		pVidObject->subscribe_event(pVidObject, uEventMask);                 

	 
		cnxt_kal_memset(&VideoConfig, 0, sizeof(VideoConfig));
		VideoConfig.DecodingType  =    PIPE_VIDEO_MOTION_STILL;
		VideoConfig.uCBuffSize    =    1024*1024 + 1; /*pInjectHandle->iFrameParams.m_nDataLen*/
		VideoConfig.uImageWidth   =    2048;
		VideoConfig.uImageHeight  =    1088;
		VideoConfig.Format        =    PIPE_VIDEO_FORMAT_MPEG2;
		VideoConfig.Definition    =    PIPE_VIDEO_HD;
		VideoConfig.bHwAccelerate   = TRUE;
		VideoConfig.bPrimaryDecoder = TRUE;
		VideoConfig.uUserDataEnableMap = 0;
		VideoConfig.StillImageHandle = gStillImageHandle;
		PmStatus = pVidObject->config(pVidObject, &VideoConfig);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			retCode = FALSE;
			break;
		}

		cnxt_kal_thread_time_sleep(10);

		////////////////////////////////////////////////////////////////////////
		/// add device
		  
		PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)pDmxObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			retCode = FALSE;
			break;
		}
		PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)pVidObject);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			retCode = FALSE;
			break;
		}

		////////////////////////////////////////////////////////////////////////
		/// flush the resources and attach video decoding to his surface
		
		PmStatus = hPipeObj->flush(hPipeObj, PIPE_FULL_FLUSH);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			retCode = FALSE;
			break;
		}
        
	  	#if 0

		//Retcode = cnxt_cbuf_flush(hCbuf, CNXT_CBUF_RESET_FLUSH);


		cnxt_kal_memset(&Input,0,sizeof(Input));
		Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_IMAGE;
        
		Input.hImageHandle = gStillImageHandle;
	
		uSurfaceIdx=  cs_tm_get_surface_indx(nVidIndx);
		
		PmStatus = cs_pipe_vpm_attach(Input, uSurfaceIdx, NULL);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
			break;
		}
		#endif
		
		////////////////////////////////////////////////////////////////////////
		/// start decoder	  
		PmStatus = pDmxObject->demux_play_ctrl(pDmxObject, TRUE);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			retCode = FALSE;
			break;
		}
		PmStatus = pVidObject->decoder_ctrl(pVidObject, TRUE);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			retCode = FALSE;
			break;
		}

		PmStatus = hPipeObj->set_decoding_speed(hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 1024);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			retCode = FALSE;
			break;
		}

		////////////////////////////////////////////////////////////////////////
		/// inject media data  
		if(pIframeBuff == NULL)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			retCode = FALSE;
			break;
		}	 


#ifdef LOOPINJECT
        {

            int uNumWritten,uTotalWritten, uNumW,uNumLeft;
            uNumWritten = uTotalWritten = uNumW= uNumLeft =0;

            cnxt_kal_memset(&MediaDataInfo,0,sizeof(MediaDataInfo));
            MediaDataInfo.Type = PIPE_MEDIA_VIDEO_ES_DATA;
            MediaDataInfo.uOverlap = 0;
            MediaDataInfo.uPts	  = 0;

            uNumLeft =   uIframeLen;

            MediaDataInfo.pData  = pIframeBuff;

            do
            {

                if(uNumLeft>=64*1024)
                {
                    MediaDataInfo.uLength = 64*1024;
                }
                else
                {
                    MediaDataInfo.uLength = uNumLeft+1;
                }

                PmStatus = hPipeObj->inject_media_data(hPipeObj, &MediaDataInfo);
                if (PmStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
                    break;
                }


                uNumLeft -= MediaDataInfo.uLength;
                
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"=================DATA INJ = %lx uNumLeft = %lx====\n",MediaDataInfo.uLength,uNumLeft);

                if(uNumLeft<=0)
                {
                    break;
                }
                MediaDataInfo.pData +=MediaDataInfo.uLength;
                cnxt_kal_thread_time_sleep(5);

            }while(uNumLeft>0);
        }

#else
		cnxt_kal_memset(&MediaDataInfo,0,sizeof(MediaDataInfo));

		MediaDataInfo.Type = PIPE_MEDIA_VIDEO_ES_DATA;
		MediaDataInfo.pData   = pIframeBuff;
		MediaDataInfo.uLength = uIframeLen+1;

        MediaDataInfo.uOverlap = 0;
        MediaDataInfo.uPts	  = 0;

		PmStatus = hPipeObj->inject_media_data(hPipeObj, &MediaDataInfo);
		if (PmStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
			break;
		}
#endif
	  
	}while(0);
      
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s %d  \n", __FUNCTION__, __LINE__);
	return retCode;
}



bool CS_Open_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem, int nVidIndex)
{	
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;		
	PIPE_VP_SURFACE_INPUT Input;
	CNXT_IMAGE_CAPS          imageCaps;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);

	if(gTmPipeObject.hVideoObj[1] == NULL)
	{
		gTmPipeObject.hVideoObj[1] = PIPE_OBJ_CREATE ( video ); 
		if(gTmPipeObject.hVideoObj[1] == NULL)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Still decode: PIPE_OBJ_CREATE ( video ) FAIL\n");
			return FALSE;
		}

		if(gStillImageHandle!=NULL)
		{

			cnxt_kal_memset(&Input,0,sizeof(Input));
			Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_NONE;
			PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_SECONDARY_SURFACE, NULL);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to ATTACH image,err=%d\n", PmStatus);
				return FALSE;
			}
		
		
			if (CNXT_STATUS_OK != cnxt_image_close(gStillImageHandle))
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to open image\n");
				return FALSE;
			}
			
			gStillImageHandle = NULL;
		}

		if(gStillImageHandle==NULL)
		{
			cnxt_kal_memset(&imageCaps,0,sizeof(imageCaps));

			imageCaps.PalTable.ColorType      = YCC;
			imageCaps.PalTable.uNumPalEntries = 0;
			imageCaps.PalTable.pPalEntry      = NULL;
			imageCaps.PoolId[0]     = CNXT_POOL_ID_DEFAULT_UNCACHED;
			imageCaps.PoolId[1]     = CNXT_POOL_ID_DEFAULT_UNCACHED;
			imageCaps.pPage         = NULL;
			imageCaps.uOrgW         = 2048;
			imageCaps.uOrgH         = 1088;
			imageCaps.PixelMode     = PIXEL_FORMAT_NV12;
			imageCaps.uNumPages     = 1;
			imageCaps.uMaxNumPages  = imageCaps.uNumPages;
			imageCaps.ColorSpace    = YCC_HD_BT709;
			imageCaps.bCached       = FALSE;
			imageCaps.bIsContiguous = TRUE;
			imageCaps.ImageType     = CNXT_IMAGE_STILL_GRAPHICS;
			
			if (CNXT_STATUS_OK != cnxt_image_open(&gStillImageHandle, &imageCaps, NULL, NULL))
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to open image\n");
				return FALSE;
			}

			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"###############gStillImageHandle =%lx###############\n",gStillImageHandle);


			cnxt_kal_memset(&Input,0,sizeof(Input));
			Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_IMAGE;
			Input.pPipe = NULL;
			Input.hImageHandle = gStillImageHandle;
			PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_SECONDARY_SURFACE, NULL);
			if (PmStatus != PIPE_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Still decode: Failed to ATTACH image,err=%d\n", PmStatus);
				return FALSE;
			}
		
		}
	}

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out: %s %d \n", __FUNCTION__, __LINE__);
	return TRUE;	
}

bool CS_Close_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);  
	return TRUE;
}

bool CS_stop_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
	bool bRet =0;

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);  

	bRet = cs_tm_iframe_stop(pPlayerSubsystem);	

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out: %s %d \n", __FUNCTION__, __LINE__);  
	
	return bRet;
}

bool CS_start_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
	bool bRet =0;
	int iTimeOutCnt =0;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);  
	
	bRet = cs_tm_iframe_start(pPlayerSubsystem);
	
	while((bFinishedIframeDec==FALSE)&&((iTimeOutCnt++)<50))
	{
		cnxt_kal_thread_time_sleep(5);
	}
	if(iTimeOutCnt>=50)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"In: %s %d: I FRAME DECODE TIME OUT! \n", __FUNCTION__, __LINE__);  
		bRet = FALSE;		              
	}
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out: %s %d \n", __FUNCTION__, __LINE__);  
	
	return bRet;
}
#endif  //#ifdef STATIC_IFRAME_DECODE
#else
bool CS_Open_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem, int nVidIndex)
{	
	u_int32   uEventMask        = 0;

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
	if(nVidIndex >= 0)
	{
		/*For multiple ES case. i.e. 1 PES video and 1 still video...DOES THIS HAPPEN????*/
		pPlayerSubsystem->pPipeObj = gTmPipeObject.hPipeObj[ES_VIDEO_OBJECT+nVidIndex];
		//pInjectHandle->pDmxObject = pDmxObject = gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+nVidIndex] = PIPE_OBJ_CREATE(demux);

		pPlayerSubsystem->pVideoObj = gTmPipeObject.hVideoObj[1] = PIPE_OBJ_CREATE ( video ); 
		 /*We are restricting videoobj 1 for I-frame*/
		if( gTmPipeObject.hVideoObj[1] == NULL )
		{
		      CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot create video objects\n" );
		      return FALSE;
		}

		// 2011-01-13 TerenceZhang begin:add event PIPE_EVENT_VIDEO_DECODE_COMPLETE.
		uEventMask = (1 << (PIPE_EVENT_VIDEO_STARTED & 0xFFFFFF)) |
					(1 << (PIPE_EVENT_VIDEO_DECODE_COMPLETE & 0xFFFFFF)) |
					(1 << (PIPE_EVENT_VIDEO_DISPLAY_UPDATE & 0xFFFFFF)) |
					(1 << (PIPE_EVENT_VIDEO_PLAYING & 0xFFFFFF))|
					(1 << (PIPE_EVENT_VIDEO_BUFFER_EMPTY & 0xFFFFFF));
		// 2011-01-13 TerenceZhang end	

		gTmPipeObject.hVideoObj[1]->subscribe_event(gTmPipeObject.hVideoObj[1], uEventMask);                 
	}   
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s %d \n", __FUNCTION__, __LINE__);
	return TRUE;
}

bool CS_Close_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
	PIPE_DEMUX_OBJECT        *pDmxObject=NULL;
	PIPE_VIDEO_OBJECT        *pVidObject=NULL;
	PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
	PIPE_VP_SURFACE_INPUT Input;
    PIPE_STATUS PmStatus = PIPE_STATUS_OK;
    
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
    
    pVidObject = pPlayerSubsystem->pVideoObj;
    pDmxObject = pPlayerSubsystem->pDemuxObj;
    hPipeObj = pPlayerSubsystem->pPipeObj;

#ifdef EXTERNAL_IMAGE_ATTACH          
    Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_NONE;
    Input.hImageHandle = gStillImageHandle;
#else
    Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_NONE;
    Input.pPipe = hPipeObj;
#endif
    PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_PRIMARY_SURFACE,(ES_VIDEO_OBJECT+pPlayerSubsystem->VideoIndx));
    if (PmStatus != PIPE_STATUS_OK)
    {
        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Err: %s %d \n", __FUNCTION__, __LINE__);
    }
    PIPE_OBJ_DESTROY(video,pVidObject);

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s %d \n", __FUNCTION__, __LINE__);

    return TRUE;
}
bool CS_start_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
      CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;
      PIPE_STATUS PmStatus = PIPE_STATUS_OK;
  	  PIPE_VP_SURFACE_INPUT Input;
      PIPE_DEMUX_OBJECT        *pDmxObject=NULL;
      PIPE_VIDEO_OBJECT        *pVidObject=NULL;
  	  PIPE_PIPELINE_OBJECT 	*hPipeObj = NULL;
      PIPE_MEDIA_DATA_INFO MediaDataInfo;
      PIPE_PIPELINE_CFG PipeCfg;
      PIPE_DEMUX_CFG demuxConfig;
  	  PIPE_VIDEO_CFG VideoConfig;
      BOOL retCode=TRUE;
      int nInjIndx = 0,nVidIndx=0;
      u_int32   uEventMask        = 0;
    
      CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
    
      nVidIndx = pPlayerSubsystem->VideoIndx;
      pPlayerSubsystem->pDemuxObj = gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+ nVidIndx] = PIPE_OBJ_CREATE(demux);
      do
      {  	    
          /*configure the pipeline*/
          pDmxObject = pPlayerSubsystem->pDemuxObj;          
          pVidObject = pPlayerSubsystem->pVideoObj;
          hPipeObj = pPlayerSubsystem->pPipeObj;
          /* config pipe */
          PipeCfg.Type = PIPE_PIPELINE_NONE_TS_TYPE;
          PipeCfg.uStcIndex = 1;
          PipeCfg.bPipPipe = FALSE;
          PipeCfg.SyncMaster = PIPE_SYNC_NO_SYNC;

          PmStatus = hPipeObj->config(hPipeObj, &PipeCfg);
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d %d \n", __FUNCTION__, __LINE__, PmStatus);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
              break;
          }
          /* config demux */
          cnxt_kal_memset(&demuxConfig, 0, sizeof(demuxConfig));
          demuxConfig.Type = PIPE_DEMUX_VIDEO_ES;
          demuxConfig.DescramblerType   = PIPE_DEMUX_DESCRAMBLE_NONE;

          PmStatus = pDmxObject->config(pDmxObject, &demuxConfig);
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
              break;
          }
          cnxt_kal_memset(&VideoConfig, 0, sizeof(VideoConfig));
          VideoConfig.DecodingType  =    PIPE_VIDEO_MOTION_STILL;
          VideoConfig.uCBuffSize    =    1024*1024 + 1; /*pInjectHandle->iFrameParams.m_nDataLen*/
          VideoConfig.uImageWidth   =    2048;
          VideoConfig.uImageHeight  =    1088;
          VideoConfig.Format        =    PIPE_VIDEO_FORMAT_MPEG2;
          VideoConfig.Definition    =    PIPE_VIDEO_HD;
          VideoConfig.bHwAccelerate   = TRUE;
          VideoConfig.bPrimaryDecoder = TRUE;
          VideoConfig.uUserDataEnableMap = 0;
#ifdef EXTERNAL_IMAGE_ATTACH          
          VideoConfig.StillImageHandle = gStillImageHandle;
#else
          VideoConfig.StillImageHandle =NULL;
#endif
          /* config video */
          PmStatus = pVidObject->config(pVidObject, &VideoConfig);
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                break;
          }
          PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)pDmxObject);
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                break;
          }
          PmStatus = hPipeObj->add_device(hPipeObj, (PIPE_OBJ_HANDLE)pVidObject);
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                break;
          }
          //2011-01-18 TerenceZhang begin:moved here from the end of this function.
          PmStatus = hPipeObj->flush(hPipeObj, PIPE_FULL_FLUSH);
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
              break;
          }
          //2011-01-18 TerenceZhang end
          
#ifdef EXTERNAL_IMAGE_ATTACH          
          Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_IMAGE;
          Input.hImageHandle = gStillImageHandle;
#else          
          Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_PIPELINE;
          Input.pPipe = hPipeObj;
          Input.hImageHandle = NULL; //2011-01-18 TerenceZhang Added
#endif          
          PmStatus = cs_pipe_vpm_attach(Input, PIPE_VP_VIDEO_PRIMARY_SURFACE, (ES_VIDEO_OBJECT+nVidIndx));
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
              break;
          }
          
          PmStatus = pDmxObject->demux_play_ctrl(pDmxObject, TRUE);
          if (PmStatus != PIPE_STATUS_OK)
          {
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
                eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                break;
          }
          /*start decoder*/
          PmStatus = pVidObject->decoder_ctrl(pVidObject, TRUE);
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
              break;
          }
          PmStatus = hPipeObj->set_decoding_speed(hPipeObj, PIPE_PLAY_CONTINUOUS_MODE, 1024);
          if (PmStatus != PIPE_STATUS_OK)
          {
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
                eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                break;
          }

          if(pIframeBuff == NULL)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
              eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
              break;
          }	 				
		   
          MediaDataInfo.Type = PIPE_MEDIA_VIDEO_ES_DATA;
          MediaDataInfo.pData   = pIframeBuff;
          MediaDataInfo.uLength = uIframeLen;
        
          PmStatus = hPipeObj->inject_media_data(hPipeObj, &MediaDataInfo);
          if (PmStatus != PIPE_STATUS_OK)
          {
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
                eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
                break;
          }
          //2011-01-18 TerenceZhang begin:moved up
          #if 0
          cnxt_kal_thread_time_sleep(100);
          /*stop decoder*/
          PmStatus = hPipeObj->flush(hPipeObj, PIPE_FULL_FLUSH);
          if (PmStatus != PIPE_STATUS_OK)
          {
              CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
              //eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
            //break;
          }
          #endif
          //2011-01-18 TerenceZhang end
    }while(0);
      
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s %d eErrCode %d \n", __FUNCTION__, __LINE__, eErrCode);
    return retCode;
}
bool CS_stop_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem)
{
    PIPE_STATUS PmStatus = PIPE_STATUS_OK; 
    PIPE_PIPELINE_OBJECT *hPipeObj=NULL;
    PIPE_DEMUX_OBJECT    *pDmxObject=NULL;
    PIPE_VIDEO_OBJECT    *pVidObject=NULL;
    CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;
	PIPE_VP_SURFACE_INPUT Input;
    BOOL retCode=TRUE;
    int nInjIndx = 0;

    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);
    
	pDmxObject = pPlayerSubsystem->pDemuxObj;	       
    pVidObject = pPlayerSubsystem->pVideoObj;
    hPipeObj = pPlayerSubsystem->pPipeObj;
    do
    {
        PmStatus = pVidObject->decoder_ctrl(pVidObject, FALSE);
        if (PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
    	    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
       	    break;
        }
        PmStatus = pDmxObject->demux_play_ctrl(pDmxObject, FALSE);
        if (PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
    	    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
        	break;
        } 

    	PmStatus = hPipeObj->delete_device(hPipeObj, (PIPE_OBJ_HANDLE)pVidObject);
        if (PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d PmStatus %d \n", __FUNCTION__, __LINE__, PmStatus);
    	    eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
       	    break;
        }
        PmStatus = hPipeObj->delete_device(hPipeObj, (PIPE_OBJ_HANDLE)pDmxObject);
        if (PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
            eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
        	break;
        }
        PmStatus = PIPE_OBJ_DESTROY ( demux, pDmxObject);
        if (PmStatus != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
            eErrCode = CSUDIPLAYER_ERROR_UNKNOWN_ERROR;
        	break;
        }
    }while(0);
    gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+pPlayerSubsystem->VideoIndx] = NULL;
    
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: %s %d eErrCode %d \n", __FUNCTION__, __LINE__, eErrCode);
    
    return retCode;

}
#endif

//frank.zhou---------------------------------------------------------------------------------------------------
/**
@brief 创建一个INJECTER注入实例,为INJECTER注入做好准备
@param[in] psInjecterChnl 数据注入通道数据结构指针。
@param[in] psOpenParams 将要创建的INJECTER实例的类型，主要包括PCM,TS,PES,ES等，详情请参见CSUDIINJECTERType_E
@param[out] phINJECTER 本次打开的INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note psInjecterChnl结构体中的设备索引可以为无效值(-1),代表该设备不需要或不存在
@note Demux必须先于Tuner断开连接，才能用于创建Inject，即一个Demux不能同时拥有两个源设备
@note 创建Inject须尽量使用最少的设备，如：
- 注入TS流的时候，注入内容只能是EM_UDIINJECTER_CONTENT_DEFAULT，否则返回CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- 注入PCM的时候，注入内容只能是EM_UDIINJECTER_CONTENT_AUDIO，否则返回CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- 注入TS流的时候，注入通道只能选DemuxID，音视频ID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- 注入PCM的时候，注入通道只能选AudioID，视频ID与DemuxID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- 注入I帧的时候，注入通道只能选VideoID,音频ID与DemuxID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- AVI,RM,RMVB, 等涉及播放多媒体文件播放的注入可不支持。如果芯片驱动对多媒体文件播放有更好的API，请告知同洲，
	同洲可对此API进行评估，如果合适，则封装上层接口直接调用其API.就不用再实现底层的注入支持。
*/
CSUDI_Error_Code  CSUDIINJECTEROpen(const CSUDIINJECTERChnl_S * psInjecterChnl, const CSUDIINJECTEROpenParam_S * psOpenParams, CSUDI_HANDLE * phINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;	
	int FreeInjectIndx;
	CSUDIINJECTERType_E  InputDataType; 
	CSUDIINJECTERContentType_E  InputContentType;   	
	int nDmxIndex=-1, nVidIndex=-1, nAudIndex=-1;	
	CS_CNXT_Inject_SubSystem *pInjectHandle = NULL;
	CNXT_STATUS Status = CNXT_STATUS_OK; 
	PIPE_DEMUX_OBJECT        *pDmxObject=NULL;
	PIPE_AUDIO_OBJECT        *pAudObject=NULL;
	PIPE_VIDEO_OBJECT        *pVidObject=NULL;
    
	IS_VALID_INJECTER(phINJECTER);
	if((psInjecterChnl == NULL)||(psOpenParams == NULL))
	{
		return CSUDIINJECTER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In: %s %d \n", __FUNCTION__, __LINE__);

	Status =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
	if(Status != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	do
	{
		if(bIsInjecteropen((u_int32)*phINJECTER,  psInjecterChnl, psOpenParams))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Injecter 0x%x DEVICE BUSY \n", phINJECTER);
			eErrCode = CSUDIINJECTER_ERROR_DEVICE_BUSY;
			break;
		}
		FreeInjectIndx = cs_tm_get_free_inject_handle();
		if(FreeInjectIndx == MAX_INJECT_HANDLES)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Injecter 0x%x CSUDIINJECTER_ERROR_NO_MEMORY \n", phINJECTER);
			eErrCode = CSUDIINJECTER_ERROR_NO_MEMORY;
			break;
		}
		s_aInjectHndl[FreeInjectIndx].uIndx = FreeInjectIndx;
		pInjectHandle = &g_sCSInjectConfig.hInjectHandle[FreeInjectIndx];
		if(psInjecterChnl->m_nDemux >= 0)
		{
			nDmxIndex = psInjecterChnl->m_nDemux;
			/*Open Demux*/
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "dmx %d \n",nDmxIndex);
		}
		if(psInjecterChnl->m_nVideoDecoder>= 0)
		{
			nVidIndex = psInjecterChnl->m_nVideoDecoder;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "vid %d \n",nVidIndex);
			/*Open video decoder*/
		}
		if(psInjecterChnl->m_nAudioDecoder>= 0)
		{
			nAudIndex = psInjecterChnl->m_nAudioDecoder;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "aud %d \n",nAudIndex);
			/*Open audio decoder*/
		}
		
		InputDataType = psOpenParams->m_eInjecterType;
		InputContentType = psOpenParams->m_eContentType;
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "%s %d dmx %d Vid %d Aud %d InputDataType %d ContentType %d\n", __FUNCTION__, __LINE__,
		nDmxIndex, nVidIndex, nAudIndex, InputDataType, InputContentType);
		if(!IS_VALID_DEMUX(nDmxIndex) &&
				!IS_VALID_AUDIO(nAudIndex) &&  /*All three indexes are wrong*/
				!IS_VALID_VIDEO(nVidIndex))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: %s %d dmx %d Vid %d Aud %d\n",__FUNCTION__, __LINE__,
			nDmxIndex, nVidIndex, nAudIndex);
			eErrCode = CSUDIINJECTER_ERROR_INVALID_DEVICE_ID;
			break;
		}
		*phINJECTER = &s_aInjectHndl[FreeInjectIndx];
		
		if((InputContentType == EM_UDIINJECTER_CONTENT_DEFAULT)&&
				((nDmxIndex<MAX_NUM_TSI_DEMUX)||(nDmxIndex>MAX_DEMUX_OBJECTS))) /*DmxIndex should be equal with inject indx*/
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE1 \n");
			if(IS_VALID_DEMUX(nDmxIndex))
			{
				eErrCode = CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE;
			}
			else
			{
				eErrCode = CSUDIINJECTER_ERROR_INVALID_DEVICE_ID;
			}
			break;
		}
		if((InputDataType == EM_INJECTER_TS) &&
				((nDmxIndex<MAX_NUM_TSI_DEMUX)||(nDmxIndex>MAX_DEMUX_OBJECTS)))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: %s %d dmx %d Vid %d Aud %d\n",__FUNCTION__, __LINE__,nDmxIndex, nVidIndex, nAudIndex);
			eErrCode = CSUDIINJECTER_ERROR_INVALID_DEVICE_ID;
			break;
		}
		
		if((((nAudIndex< -1)||(nAudIndex>MAX_AUDIO_OBJECTS))&&(InputContentType == EM_UDIINJECTER_CONTENT_AUDIO))||
				(((nVidIndex< -1)||(nVidIndex>MAX_VIDEO_OBJECTS))&&(InputContentType == EM_UDIINJECTER_CONTENT_VIDEO)))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: %s %d dmx %d Vid %d Aud %d\n",__FUNCTION__, __LINE__,nDmxIndex, nVidIndex, nAudIndex);
			eErrCode = CSUDIINJECTER_ERROR_INVALID_DEVICE_ID;
			break;	  	
		}
		if((InputDataType < EM_INJECTER_ES) &&(InputContentType == EM_UDIINJECTER_CONTENT_DEFAULT))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: %s %d dmx %d Vid %d Aud %d\n",__FUNCTION__, __LINE__,nDmxIndex, nVidIndex, nAudIndex);
			eErrCode = CSUDIINJECTER_ERROR_DEVICE_DISORDER;
			break;
		}	  
		if((InputDataType == EM_INJECTER_ES) &&(InputContentType != EM_UDIINJECTER_CONTENT_IFRAME)&&
				(nDmxIndex!=-1))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: %s %d dmx %d Vid %d Aud %d\n",__FUNCTION__, __LINE__,nDmxIndex, nVidIndex, nAudIndex);
			eErrCode = CSUDIINJECTER_ERROR_DEVICE_DISORDER;
			break;
		}
		if((nAudIndex< -1)||(nAudIndex>MAX_AUDIO_OBJECTS)||
				(nVidIndex< -1) ||(nVidIndex>MAX_VIDEO_OBJECTS))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: %s %d dmx %d Vid %d Aud %d\n",__FUNCTION__, __LINE__,nDmxIndex, nVidIndex, nAudIndex);
			eErrCode = CSUDIINJECTER_ERROR_DEVICE_DISORDER;
			break;
		}
		if(nDmxIndex>=0)
		{
			pDmxObject = gTmPipeObject.hDemuxObj[nDmxIndex];
		}
		if(nAudIndex>=0)
		{
			pAudObject = gTmPipeObject.hAudioObj[nAudIndex];
		}
		if(nVidIndex>=0)
		{
			pVidObject = gTmPipeObject.hVideoObj[nVidIndex];
		}
		if(((nAudIndex>=0)&&(pAudObject==NULL)) ||
				((nVidIndex>=0)&&(pVidObject==NULL)) ||
				((nDmxIndex>=0)&&(nDmxIndex< MAX_DEMUX_OBJECTS)&&(pDmxObject==NULL)))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE2 \n");
			eErrCode = CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE;
			break;
		}
		if(((pAudObject!=NULL)&&(InputContentType==EM_UDIINJECTER_CONTENT_VIDEO))||
				((pVidObject!=NULL)&&(InputContentType==EM_UDIINJECTER_CONTENT_AUDIO)))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE3 \n");
			eErrCode = CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE;
			break;
		}
		if(((InputContentType == EM_UDIINJECTER_CONTENT_DEFAULT)&&(InputDataType!=EM_INJECTER_TS))||
				((InputContentType != EM_UDIINJECTER_CONTENT_DEFAULT)&&(InputDataType==EM_INJECTER_TS)))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE4 \n");
			eErrCode = CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE;
			break;
		}        
		if(((InputContentType == EM_UDIINJECTER_CONTENT_DEFAULT)&&(InputDataType==EM_INJECTER_TS))&&
				((nAudIndex< -1)||(nAudIndex>= MAX_AUDIO_OBJECTS) ||
					(nVidIndex< -1)||(nVidIndex>= MAX_VIDEO_OBJECTS)))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIINJECTER_ERROR_DEVICE_DISORDER0 000\n");
			eErrCode = CSUDIINJECTER_ERROR_DEVICE_DISORDER;
			break;
		}
		if((nDmxIndex>=MAX_NUM_TSI_DEMUX)&&(nDmxIndex<MAX_DEMUX_OBJECTS)&&
				(InputDataType==EM_INJECTER_PCM))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIINJECTER_ERROR_DEVICE_DISORDER1 \n");
			eErrCode = CSUDIINJECTER_ERROR_DEVICE_DISORDER;
			break;
		}
		
		if((InputContentType>=EM_UDIINJECTER_CONTENT_SUBTITLE)&&(InputContentType != EM_UDIINJECTER_CONTENT_IFRAME)&&
				((nAudIndex>=0)||/*(nVidIndex>=0)||*/(nDmxIndex>=0)))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIINJECTER_ERROR_DEVICE_DISORDER2 \n");
			eErrCode = CSUDIINJECTER_ERROR_DEVICE_DISORDER;
			break;
		}
		
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "%s %d Type %d\n",__FUNCTION__, __LINE__, psOpenParams->m_eInjecterType);
		if(psOpenParams->m_eInjecterType == EM_INJECTER_TS)
		{
			/*Create TS inject PIPE*/
			pInjectHandle->pInjectPipeObj = gTmPipeObject.hPipeObj[nDmxIndex];

			/*pInjectHandle->pInjectPipeObj->set_notifier(pInjectHandle->pInjectPipeObj,
											(PIPE_PIPELINE_PFNNOTIFY)cs_tm_playback_notify,
											(void *)pInjectHandle);*/
			pInjectHandle->pDmxObject = pDmxObject;
			/*Add video object to Inject pipeline
		pInjectHandle->pInjectPipeObj->add_device(pInjectHandle->pInjectPipeObj, (PIPE_OBJ_HANDLE)pDmxObject);*/
			
			if(pAudObject == NULL)
			{
				/*Add Audio object to Inject pipeline
			pInjectHandle->pInjectPipeObj->add_device(pInjectHandle->pInjectPipeObj, (PIPE_OBJ_HANDLE)pAudObject);*/
				pInjectHandle->pAudObject = pAudObject = gTmPipeObject.hAudioObj[0];
			}
			if(pVidObject == NULL)
			{
				/*Add video object to Inject pipeline
			pInjectHandle->pInjectPipeObj->add_device(pInjectHandle->pInjectPipeObj, (PIPE_OBJ_HANDLE)pVidObject);*/
				pInjectHandle->pVidObject = pVidObject = gTmPipeObject.hVideoObj[0];
			}
			bPlayerStarted = FALSE;
			/*In Open we will just open and add the devices to pipe. In player set stream we configure them,
		and during start we enable them */
		}
	  else if(((psOpenParams->m_eInjecterType == EM_INJECTER_ES)||
	  		(psOpenParams->m_eInjecterType == EM_INJECTER_PES))&&
	  		(InputContentType != EM_UDIINJECTER_CONTENT_IFRAME))
		{
			/*Create ES inject PIPE*/
			if((nVidIndex >= 0)&&(nAudIndex < 0))
			{
				/*For multiple ES case. i.e. 1 PES video and 1 still video...DOES THIS HAPPEN????*/
				pInjectHandle->pInjectPipeObj = gTmPipeObject.hPipeObj[ES_VIDEO_OBJECT+nVidIndex];
				pInjectHandle->pDmxObject = pDmxObject = gTmPipeObject.hDemuxObj[ES_VIDEO_OBJECT+nVidIndex]; 
			}
			if((nVidIndex < 0)&&(nAudIndex >= 0))
			{
				pInjectHandle->pInjectPipeObj = gTmPipeObject.hPipeObj[ES_AUDIO_OBJECT];
				pInjectHandle->pDmxObject = pDmxObject = gTmPipeObject.hDemuxObj[ES_AUDIO_OBJECT];
			}
			/*pInjectHandle->pInjectPipeObj->set_notifier(pInjectHandle->pInjectPipeObj,
												(PIPE_PIPELINE_PFNNOTIFY)cs_tm_es_playback_notify,
												(void *)pInjectHandle);
			Add video object to Inject pipeline
		pInjectHandle->pInjectPipeObj->add_device(pInjectHandle->pInjectPipeObj, (PIPE_OBJ_HANDLE)pDmxObject);*/
			
			if(pAudObject != NULL)
			{
				/*Add Audio object to Inject pipeline
			pInjectHandle->pInjectPipeObj->add_device(pInjectHandle->pInjectPipeObj, (PIPE_OBJ_HANDLE)pAudObject);*/
				pInjectHandle->pAudObject = pAudObject;
			}
			if(pVidObject != NULL)
			{
				/*Add video object to Inject pipeline
			pInjectHandle->pInjectPipeObj->add_device(pInjectHandle->pInjectPipeObj, (PIPE_OBJ_HANDLE)pVidObject);*/
				pInjectHandle->pVidObject = pVidObject;
			}
			bPlayerStarted = FALSE;
		}
      else if(InputContentType == EM_UDIINJECTER_CONTENT_IFRAME)
      {
           bInjectIframe = TRUE;/*Create ES inject PIPE*/
  		   /*Create ES inject PIPE...we are sure for I-frame only video is needed :) */		  
       }
       else if(psOpenParams->m_eInjecterType == EM_INJECTER_PCM)
       {
            if(nDmxIndex > 0)
            {
                 CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIINJECTER_ERROR_DEVICE_DISORDER4 FAILED \n");
                 eErrCode = CSUDIINJECTER_ERROR_DEVICE_DISORDER;
                 break;
            }
            if(FALSE == CSPcmInject_Open(pInjectHandle,nAudIndex))
            {
                 CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSPcmPlay_Open FAILED \n");
                 eErrCode = CSUDIINJECTER_ERROR_DEVICE_BUSY;
                 break;
            }
        }
		pInjectHandle->nAudIndx = nAudIndex;
		pInjectHandle->nVidIndx = nVidIndex;
		pInjectHandle->nDmxIndx = nDmxIndex;
		pInjectHandle->eInjecterType = InputDataType;
		pInjectHandle->eContentType = InputContentType;
		pInjectHandle->eInjecterState = E_INJECT_OPEN;
		s_aInjectHndl[FreeInjectIndx].uIndx = FreeInjectIndx;
		s_aInjectHndl[FreeInjectIndx].bInjecterOpen = TRUE;
		pInjectHandle->bInUse = TRUE;
		
		*phINJECTER = &s_aInjectHndl[FreeInjectIndx];
	}while(0);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit:%s %d eErrCode 0x%x \n ",__FUNCTION__, __LINE__, eErrCode);
	Status =  cnxt_kal_sem_put(gInjecterSem);
	if(Status != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	return (CSUDI_Error_Code )(eErrCode);
	return Retcode;
}

/**
@brief 关闭一个INJECTER注入实例,释放相关资源
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code  CSUDIINJECTERClose(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
	CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;     
	int nInjectIndx, nAudIndx, nVidIndx=0,nDmxIndex;
	CS_CNXT_Inject_SubSystem *pInjectHandle;
	CNXT_STATUS Status = CNXT_STATUS_OK;
	PIPE_DEMUX_OBJECT        *pDmxObject=NULL;
	PIPE_AUDIO_OBJECT        *pAudObject=NULL;
	PIPE_VIDEO_OBJECT        *pVidObject=NULL;

	IS_VALID_INJECTER(hINJECTER);
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d \n", __FUNCTION__, __LINE__);
	nInjectIndx = phInjectHndl->uIndx;
	pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];
	if(pInjectHandle->eInjecterState <= E_INJECT_CLOSE)
	{
		return CSUDIINJECTER_ERROR_INVALID_STATUS;
	}
	
	if(pInjectHandle->eInjecterState == E_INJECT_START)
	{
		Retcode = CSUDIINJECTERStop(hINJECTER);
		if(Retcode != CSUDI_SUCCESS)
		{
            CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIINJECTERStop at %d\n",__LINE__);
			return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
		}
	}   
	Status =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
	if(Status != CNXT_STATUS_OK)
	{
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: cnxt_kal_sem_get at %d\n",__LINE__);
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	do
	{
		if(pInjectHandle->eInjecterType == EM_INJECTER_TS)
		{
			nAudIndx = pInjectHandle->nAudIndx;
			nVidIndx = pInjectHandle->nVidIndx;
			nDmxIndex = pInjectHandle->nDmxIndx;
			if(nDmxIndex>=0)
			{
				pDmxObject = gTmPipeObject.hDemuxObj[nDmxIndex];
			}
			if(nAudIndx>=0)
			{
				pAudObject = gTmPipeObject.hAudioObj[nAudIndx];
			}
			if(nVidIndx>=0)
			{
				pVidObject = gTmPipeObject.hVideoObj[nVidIndx];
			}		

		}
		else if(pInjectHandle->eContentType == EM_UDIINJECTER_CONTENT_IFRAME )
		{            
            
		}
        else if(pInjectHandle->eInjecterType == EM_INJECTER_ES )
        {
        }
		else if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
		{
           if(FALSE ==  CSPcmInject_Close (pInjectHandle))
           {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSPcmPlay_Close \n");
                eErrCode = CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
                break;
            }
        }
        cnxt_kal_memset(pInjectHandle,0,sizeof(CS_CNXT_Inject_SubSystem));
        pInjectHandle->bInUse = FALSE;
        s_aInjectHndl[nInjectIndx].bInjecterOpen = FALSE;
        pInjectHandle->eInjecterState = E_INJECT_CLOSE;
    }while(0);
    Status =  cnxt_kal_sem_put(gInjecterSem);
    if(Status != CNXT_STATUS_OK)
    {
        return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
    }
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "End: %s eErrCode %d \n", __FUNCTION__, eErrCode);

	return (CSUDI_Error_Code )(eErrCode);
	return Retcode;
}

/**
@brief 设置INJECTER注入实例的属性信息，本函数调整INJECTER实例的属性、相关参数、数据源等信息。
@param[in] hINJECTER 注入实例句柄，由CSUDIINJECTEROpen返回
@param[in] eAttrType ，将要调整的属性类型，请参见CSUDIINJECTERAttributeType_E的详细说明
@param[in] pvData 具体的属性数据结构指针，内容请参见CSUDIINJECTERAttributeType_E的详细说明。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 如果已经调用CSUDIINJECTERStart启动数据注入，不允许再调用此接口对回调函数进行设置。
@note 如果不支持设置回调函数，调用此接口将返回不支持操作
*/
CSUDI_Error_Code CSUDIINJECTERSetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, const void * pvData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
	int nInjectIndx;
	CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;
	CS_CNXT_Inject_SubSystem *pInjectHandle;
	CNXT_STATUS Status = CNXT_STATUS_OK;
	CSUDIINJECTERPcmStartParam_S *psPcmStartParams;
	CSUDIINJECTERIFrameParam_S* piFrameParams;
	CSUDIPESSYNCMode_S *psSyncmode;
	CSUDIINJECTERBuffer_S *psBuffer;
	CSUDIINJECTERStatus_S *psStatus;
	
	IS_VALID_INJECTER(hINJECTER);

	if(pvData == NULL)
	{
		return CSUDIINJECTER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d \n", __FUNCTION__, __LINE__);
	do
	{
		Status =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
		if(Status != CNXT_STATUS_OK)
		{
			return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
		}
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d \n", __FUNCTION__, __LINE__);
		nInjectIndx = phInjectHndl->uIndx;
		pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];

		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "In: %s Attr %d pvData 0x%x >eInjecterType %d \n", __FUNCTION__, eAttrType, pvData, pInjectHandle->eInjecterType);
		switch(eAttrType)
		{
		case EM_UDIINJECTER_STATUS:
			if(pInjectHandle->eInjecterState != E_INJECT_START)
			{
				Status =  cnxt_kal_sem_put(gInjecterSem);
				return CSUDIINJECTER_ERROR_INVALID_STATUS;
			}
			psStatus = (CSUDIINJECTERStatus_S *)pvData;
			pInjectHandle->psStatus.m_nAvailableDataSize = psStatus->m_nAvailableDataSize;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Available Data %d \n", psStatus->m_nAvailableDataSize);
			break;
		case EM_UDIINJECTER_BUFFER:
			if(pInjectHandle->eInjecterState != E_INJECT_START)
			{
				Status =  cnxt_kal_sem_put(gInjecterSem);
				return CSUDIINJECTER_ERROR_INVALID_STATUS;
			}
			psBuffer = (CSUDIINJECTERBuffer_S*)pvData;
			pInjectHandle->psBuffer.m_nFIFOBufferSize = psBuffer->m_nFIFOBufferSize;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "buffersize %d \n", psBuffer->m_nFIFOBufferSize);         
			break;
		case EM_UDIINJECTER_PES_SYNC:
			psSyncmode = (CSUDIPESSYNCMode_S *)pvData;
			pInjectHandle->psSyncmode.m_eSyncMode = psSyncmode->m_eSyncMode;
			pInjectHandle->psSyncmode.m_hSyncHandle = psSyncmode->m_hSyncHandle;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "syncmode %d synchndl 0x%x \n", psSyncmode->m_eSyncMode, psSyncmode->m_hSyncHandle);
			break;
		case EM_UDIINJECTER_PCM_PARAMS:
			psPcmStartParams = (CSUDIINJECTERPcmStartParam_S *)pvData;
			pInjectHandle->psPcmStartParams.m_uBitsSample = psPcmStartParams->m_uBitsSample;
			pInjectHandle->psPcmStartParams.m_uSampleRate= psPcmStartParams->m_uSampleRate;
			pInjectHandle->psPcmStartParams.m_uChannels = psPcmStartParams->m_uChannels;
			
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "bps %d channels %d samplerate %d \n", 
			psPcmStartParams->m_uBitsSample, psPcmStartParams->m_uChannels, psPcmStartParams->m_uSampleRate);
			break;
		case EM_UDIINJECTER_PCM_ENDIAN:
			//psStatus = pvData;
			break;
		case EM_UDIINJECTER_CALLBACK:
			//pInjectHandle->InjectCallbkFunc = (CSUDIINJECTERReqDataCallback_F)pvData;
			break;
		case EM_UDIINJECTER_IFRAME_PARAMS:
			piFrameParams = (CSUDIINJECTERIFrameParam_S* )pvData;
			pInjectHandle->iFrameParams.m_pcIFrameData = piFrameParams->m_pcIFrameData;
			pInjectHandle->iFrameParams.m_nDataLen = piFrameParams->m_nDataLen;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "i-frame params m_nDataLen %d \n", piFrameParams->m_nDataLen);
			break;
        default:
                    eErrCode = CSUDIINJECTER_ERROR_BAD_PARAMETER;
                    break;
		}   
		if(pInjectHandle->eInjecterState == E_INJECT_START)
		{
			eErrCode = CSUDIINJECTER_ERROR_INVALID_STATUS;
			break;
		}
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "ARG CHECK PASSED \n");
		if(pInjectHandle->eInjecterType == EM_INJECTER_TS)
		{
			if(eAttrType == EM_UDIINJECTER_PCM_ENDIAN)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: BAD PARAM Attr %d \n", eAttrType);
				eErrCode = CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED;
				break;
			}       
			if(eAttrType >= EM_UDIINJECTER_PCM_PARAMS)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: BAD PARAM Attr %d \n", eAttrType);
				eErrCode = CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED;
				break;
			}
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
		{
			if(eAttrType == EM_UDIINJECTER_PCM_PARAMS)
			{
	              if( TRUE!= CSPcmPlaySetPCMParams(pInjectHandle, (CSUDIINJECTERPcmStartParam_S*)pvData))
	              {
	          		   CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: CSPcmPlaySetPCMParams \n");
	                      eErrCode = CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	                      break;
	              }
			}
			else if(eAttrType == EM_UDIINJECTER_PCM_ENDIAN)
			{
	              if( TRUE!=CSPcmPlaySetEndian(pInjectHandle, (CSUDIINJECTERPcmEndian_E*)pvData))
	              {
	          		    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSPcmPlaySetEndian \n");
	                    eErrCode = CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	                    break;
	              }
			}
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_ES)
		{
			if(eAttrType == EM_UDIINJECTER_IFRAME_PARAMS)
			{
				break;
			}       
			else if(eAttrType >= EM_UDIINJECTER_PCM_PARAMS)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: BAD PARAM Attr %d \n", eAttrType);
				eErrCode = CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED;
				break;
			}
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_PES)
		{        
			if(eAttrType >= EM_UDIINJECTER_PCM_PARAMS)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: BAD PARAM Attr %d \n", eAttrType);
				eErrCode = CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED;
				break;
			}
		}
	}while(0);
	Status =  cnxt_kal_sem_put(gInjecterSem);
	if(Status != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "End: %s \n", __FUNCTION__);
	
	return (CSUDI_Error_Code)(eErrCode);
	return Retcode;
}


/**
@brief 获取INJECTER注入实例的各属性信息。
@param[in] hINJECTER 注入实例句柄，由CSUDIINJECTEROpen返回
@param[in] eAttrType ，将要调整的属性类型，请参见CSUDIINJECTERAttributeType_E的详细说明
@param[out] pvData 具体的属性数据结构指针，内容请参见CSUDIINJECTERAttributeType_E的详细说明。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIINJECTERGetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, void * pvData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
	int nInjectIndx;
	CSUDI_Error_Code eErrCode=CSUDI_SUCCESS;
	CS_CNXT_Inject_SubSystem *pInjectHandle;
	CSUDIPESSYNCMode_S *psSyncmode;
	CSUDIINJECTERBuffer_S *psBuffer;
	CSUDIINJECTERStatus_S *psStatus;
	CSUDIINJECTERIFrameParam_S* piFrameParams;
	
	CNXT_STATUS Status = CNXT_STATUS_OK;

	IS_VALID_INJECTER(hINJECTER);
	
	nInjectIndx = phInjectHndl->uIndx;
	pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];
	
	if(pvData == NULL)
	{
		return CSUDIINJECTER_ERROR_BAD_PARAMETER;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d \n", __FUNCTION__, __LINE__);
	Status =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
	if(Status != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	do
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "In: %s Attr %d pvData 0x%x eInjecterType %d \n", 
		__FUNCTION__, eAttrType, pvData, pInjectHandle->eInjecterType);
		if((pInjectHandle->eInjecterType == EM_INJECTER_TS)||
				(pInjectHandle->eInjecterType == EM_INJECTER_PES))
		{
			switch(eAttrType)
			{
			case EM_UDIINJECTER_STATUS: /*Get available data*/
				psStatus = (CSUDIINJECTERStatus_S *)pvData;
				psStatus->m_nAvailableDataSize= pInjectHandle->psStatus.m_nAvailableDataSize;
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Available Data \n");
				break;
			case EM_UDIINJECTER_BUFFER: /*Get free buffer*/
				psBuffer = (CSUDIINJECTERBuffer_S*)pvData;
				psBuffer->m_nFIFOBufferSize = pInjectHandle->psBuffer.m_nFIFOBufferSize;
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "buffersize %d \n");
				break;
			case EM_UDIINJECTER_PES_SYNC:
				psSyncmode = (CSUDIPESSYNCMode_S *)pvData;
				psSyncmode->m_eSyncMode = pInjectHandle->psSyncmode.m_eSyncMode;
				psSyncmode->m_hSyncHandle = pInjectHandle->psSyncmode.m_hSyncHandle;
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "syncmode  \n");
				break;
			case EM_UDIINJECTER_PCM_PARAMS:
			case EM_UDIINJECTER_PCM_ENDIAN:
				eErrCode = CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED;					 
				break;
			case EM_UDIINJECTER_CALLBACK:
			case EM_UDIINJECTER_IFRAME_PARAMS:
				break;
			}   
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
		{
			if(eAttrType == EM_UDIINJECTER_PCM_PARAMS)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "EM_UDIINJECTER_PCM_PARAMS  \n");
                    if( TRUE!= CSPcmPlayGetPCMParams(pInjectHandle, (CSUDIINJECTERPcmStartParam_S*)pvData))
                    {
                      Status =  cnxt_kal_sem_put(gInjecterSem);
                      return CSUDI_FAILURE;
                    }
			}
			else if(eAttrType == EM_UDIINJECTER_PCM_ENDIAN)
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "EM_UDIINJECTER_PCM_ENDIAN  \n");
                    if( TRUE!=CSPcmPlayGetEndian(pInjectHandle, (CSUDIINJECTERPcmEndian_E*)pvData))
                    {
                      Status =  cnxt_kal_sem_put(gInjecterSem);
                      return CSUDI_FAILURE;
                    }
			}
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_ES)
		{
			switch(eAttrType)
			{
			case EM_UDIINJECTER_STATUS: /*Get available data*/
				psStatus = (CSUDIINJECTERStatus_S *)pvData;
				psStatus->m_nAvailableDataSize= pInjectHandle->psStatus.m_nAvailableDataSize;
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Available Data \n");
				break;
			case EM_UDIINJECTER_BUFFER: /*Get free buffer*/
				psBuffer = (CSUDIINJECTERBuffer_S*)pvData;
				psBuffer->m_nFIFOBufferSize = pInjectHandle->psBuffer.m_nFIFOBufferSize;
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "buffersize %d \n");
				break;
			case EM_UDIINJECTER_PES_SYNC:
				psSyncmode = (CSUDIPESSYNCMode_S *)pvData;
				psSyncmode->m_eSyncMode = pInjectHandle->psSyncmode.m_eSyncMode;
				psSyncmode->m_hSyncHandle = pInjectHandle->psSyncmode.m_hSyncHandle;
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "syncmode  \n");
				break;
			case EM_UDIINJECTER_PCM_PARAMS:
			case EM_UDIINJECTER_PCM_ENDIAN:
				eErrCode = CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED;
				break;
			case EM_UDIINJECTER_CALLBACK:
				break;
			case EM_UDIINJECTER_IFRAME_PARAMS:				
				piFrameParams = (CSUDIINJECTERIFrameParam_S*)pvData;
				piFrameParams->m_pcIFrameData = pInjectHandle->iFrameParams.m_pcIFrameData;
				piFrameParams->m_nDataLen = pInjectHandle->iFrameParams.m_nDataLen;
				break;
			}   
		}
	}while(0);
	Status =  cnxt_kal_sem_put(gInjecterSem);
	if(Status != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "End: %s \n", __FUNCTION__);

	return (eErrCode);
	return Retcode;
}

/**
@brief  开始一个INJECTER 数据 注入
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果注入实例已经开始，则返回CSUDIREC_ERROR_ALREADY_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStart(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	int nInjectIndx;
    UDI_INJECT_HNDL *phInjHndl = (UDI_INJECT_HNDL *)hINJECTER;
    CS_CNXT_Inject_SubSystem *pInjectHandle;
    CNXT_STATUS Status = CNXT_STATUS_OK;
    CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;
	
    IS_VALID_INJECTER(hINJECTER);

    Status =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
    if(Status != CNXT_STATUS_OK)
    {
    	return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
    }
    
    do
    {
        nInjectIndx = phInjHndl->uIndx;
        pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];

        CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "In: %s %d \n", __FUNCTION__, __LINE__);

        if(pInjectHandle->eInjecterState == E_INJECT_START)
        {
            eErrCode = CSUDIINJECTER_ERROR_ALREADY_STARTED;
            break;
        }

        if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
        {
            if( TRUE!=CSPcmInject_Start(pInjectHandle) )
            {
                Status =  cnxt_kal_sem_put(gInjecterSem);
                return CSUDI_FAILURE;
            }
        }
        else if(pInjectHandle->eContentType != EM_UDIINJECTER_CONTENT_IFRAME)
        {		
            /*Nothing to do in Inject start */
            Status =  cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_BANK1_UNCACHED,
                                			512*188,
                                			CNXT_MEM_ALIGN_DEFAULT,
                                			(void **)&pInjectHandle->pTsBuff); 

#if 1			
			Status =  cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_BANK1_UNCACHED,
                    			188*1024,
                    			CNXT_MEM_ALIGN_DEFAULT,
                    			(void **)&pInternalBuffer); 
            tempsuccesscounter = 30;
            gMinSendSize = MIN_SEND_SIZE;
#endif
            pInjectHandle->bTsBuffInUse = FALSE;
        }
        else if(pInjectHandle->eContentType == EM_UDIINJECTER_CONTENT_IFRAME)
        {	     
#ifdef DUAL_VIDEO_SURFACE

            #if 1
            // 2011-01-17 TerenceZhang begin
		uIframeLen = pInjectHandle->iFrameParams.m_nDataLen;

		if(uIframeLen %(64*1024))
			uIframeLen = (uIframeLen/(64*1024)+1)*(64*1024);

		//uIframeLen = 1024*1024;
            #else
                 uIframeLen = pInjectHandle->iFrameParams.m_nDataLen;
            
            printf("\n=================DATA ORG LEN: 0x%lx====\n",pInjectHandle->iFrameParams.m_nDataLen);
            #endif
            /*Inject data*/
            /* receiving a data buffer */
            Status =  cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_BANK1_UNCACHED,
                                             uIframeLen+1/*pInjectHandle->iFrameParams.m_nDataLen*/,
                                             CNXT_MEM_ALIGN_DEFAULT,
                                             (void **)&pIframeBuff);   
#else
            // 2011-01-17 TerenceZhang begin
            if(pInjectHandle->iFrameParams.m_nDataLen >1024*1024)
                uIframeLen = pInjectHandle->iFrameParams.m_nDataLen;
            else
                uIframeLen = 1024*1024;
			
            /*Inject data*/
            /* receiving a data buffer */
            Status =  cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_BANK1_UNCACHED,
                                             uIframeLen/*pInjectHandle->iFrameParams.m_nDataLen*/,
                                             0,
                                             (void **)&pIframeBuff);
#endif
            if(Status!= CNXT_STATUS_OK)
            {
                Retcode =  CSUDIINJECTER_ERROR_NO_MEMORY;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                break;
            }	 				
            else
            {
                cnxt_kal_memset(pIframeBuff,0,uIframeLen);
                cnxt_kal_memcpy(pIframeBuff,pInjectHandle->iFrameParams.m_pcIFrameData,
                                    pInjectHandle->iFrameParams.m_nDataLen );
				
                //uIframeLen = pInjectHandle->iFrameParams.m_nDataLen;	
            }	      
            //2011-01-17 TerenceZhang end
        }
        pInjectHandle->eInjecterState = E_INJECT_START;
    }while(0);
    
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "End: %s eErrCode 0x%x \n", __FUNCTION__, eErrCode);
    Status =  cnxt_kal_sem_put(gInjecterSem);
    if(Status != CNXT_STATUS_OK)
    {
        return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
    }

    return (CSUDI_Error_Code)(eErrCode);
	return Retcode;
}


/**
@brief 停止一个INJECTER 数据注入
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果注入实例已经被停止，则返回CSUDIREC_ERROR_ALREADY_STOPPED
@note  如果注入实例没有启动，则返回CSUDIREC_ERROR_NOT_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStop(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
    int nInjectIndx;
    CS_CNXT_Inject_SubSystem *pInjectHandle;
    CNXT_STATUS RetCode = CNXT_STATUS_OK;
    CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;
	//PIPE_VP_SURFACE_INPUT Input;

	IS_VALID_INJECTER(hINJECTER);
	
	nInjectIndx = phInjectHndl->uIndx;
	pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "In: %s \n", __FUNCTION__);
	IPVOD_buf_reset();

	RetCode =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
	if(RetCode != CNXT_STATUS_OK)
	{
	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	do
	{

    	if(pInjectHandle->eInjecterState == E_INJECT_STOP)
    	{
    	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
		    eErrCode = CSUDIINJECTER_ERROR_ALREADY_STOPPED;
       	    break;
    	}
    	if(pInjectHandle->eInjecterState != E_INJECT_START)
    	{
    	    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
		    eErrCode = CSUDIINJECTER_ERROR_NOT_STARTED ;
       	    break;
    	}
		if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
		{
	        if( TRUE!=CSPcmInject_Stop(pInjectHandle) )
	        {
	            RetCode =  cnxt_kal_sem_put(gInjecterSem);
	            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
	            return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	        }
		}
	    else if(pInjectHandle->eContentType != EM_UDIINJECTER_CONTENT_IFRAME)
	    {        
		  	 RetCode =  cnxt_kal_mem_free(pInjectHandle->pTsBuff);
             RetCode =  cnxt_kal_mem_free(pInternalBuffer);
	    }
        else if(pInjectHandle->eContentType == EM_UDIINJECTER_CONTENT_IFRAME)
        {
            if(pIframeBuff==NULL)
            {
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                eErrCode = CSUDIINJECTER_ERROR_UNKNOWN_ERROR ;
                break;
            }	 				
            RetCode =  cnxt_kal_mem_free(pIframeBuff);
#ifdef DUAL_VIDEO_SURFACE
            if(RetCode !=CNXT_STATUS_OK)
            {
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                eErrCode = CSUDIINJECTER_ERROR_UNKNOWN_ERROR ;
                break;
            }
#endif            
            pIframeBuff = NULL;
            uIframeLen = 0;
        }
	    pInjectHandle->eInjecterState = E_INJECT_STOP;
	}while(0);
	
	RetCode =  cnxt_kal_sem_put(gInjecterSem);
	if(RetCode != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "End: %s eErrCode 0x%x \n", __FUNCTION__, eErrCode);

	return (CSUDI_Error_Code)(eErrCode);
	return Retcode;
}

/**
@brief等待Inecter已经注入到目标缓存区中的数据播放完成

本函数会堵塞，一直等到缓存中的数据播放完成了才返回CSUDI_SUCCESS，或下层驱动超时返回CSUDI_FAILURE
一般用于待播放数据全部注入完毕后，等待播放结束
@param[in] hINJECTER INJECTER数据注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  
*/
CSUDI_Error_Code CSUDIINJECTERFlush(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
	int nInjectIndx, nAudIndx=0, nVidIndx=0;
	CS_CNXT_Inject_SubSystem *pInjectHandle;
	CNXT_STATUS RetCode = CNXT_STATUS_OK;
	CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;

	IS_VALID_INJECTER(hINJECTER);
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d \n", __FUNCTION__, __LINE__);

	RetCode =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
	if(RetCode != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	do
	{
		nInjectIndx = phInjectHndl->uIndx;
		pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];
		
		if(pInjectHandle->eInjecterState != E_INJECT_START)
		{
			eErrCode = CSUDIINJECTER_ERROR_ALREADY_STARTED;
			break;
		}
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "In: %s \n", __FUNCTION__);
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d \n", __FUNCTION__, __LINE__);
		if(pInjectHandle->eInjecterType == EM_INJECTER_TS)
		{   	 	
			PIPE_AUDIO_OBJECT	*pAudioObj = NULL;
			PIPE_VIDEO_OBJECT   *pVideoObj = NULL;
			PIPE_AUDIO_ES_BUFFER_ATTRIB AudioEsBuff;
			PIPE_VIDEO_ES_BUFFER_ATTRIB VideoEsBuff;
			//nAudIndx = pInjectHandle->nAudIndx;
			//nVidIndx = pInjectHandle->nVidIndx;
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d \n", __FUNCTION__, __LINE__);
			pAudioObj = gTmPipeObject.hAudioObj[nAudIndx];
			pVideoObj = gTmPipeObject.hVideoObj[nVidIndx];
			do
			{
				pAudioObj->get_es_buffer_attrib(pAudioObj, &AudioEsBuff);
				pVideoObj->get_es_buffer_attrib(pVideoObj, &VideoEsBuff);
				
				if((AudioEsBuff.uUsedSpace==0 )&&
						(VideoEsBuff.uUsedSpace==0 ))
				{
					break;
				}
			}while(0);
			/*No functionality available to flush ES buffers...Need to check with Pipeline*/
		}
        else if((pInjectHandle->eInjecterType == EM_INJECTER_ES)||
                (pInjectHandle->eInjecterType == EM_INJECTER_PES))
		{
			//[alfredchen] here we could handle the ES Iframe case for display on video layer.
			if(pInjectHandle->eContentType == EM_UDIINJECTER_CONTENT_IFRAME)
			{
				
			}
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
		{
             if( TRUE!=CSPcmInject_Flush(pInjectHandle) )
             {
                 eErrCode = CSUDI_FAILURE;
                 break;
             }
		}
        else
        {
        }
	}while(0);
	RetCode =  cnxt_kal_sem_put(gInjecterSem);
	if(RetCode != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "End: %s  \n", __FUNCTION__);
	
	return (CSUDI_Error_Code)(eErrCode);
	return Retcode;
}

/**
@brief 清除Inecter已经注入到目标缓存区中的，尚没有播放完的全部数据

主要用于快进、快退、定位等操作时清空已注入的数据，已防止这些操作的延迟
@param[in] hINJECTER INJECTER数据注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  
*/
CSUDI_Error_Code CSUDIINJECTERClear(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
	int nInjectIndx;
	CS_CNXT_Inject_SubSystem *pInjectHandle;
	CNXT_STATUS RetCode = CNXT_STATUS_OK;
	CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;

	IS_VALID_INJECTER(hINJECTER);
	
	nInjectIndx = phInjectHndl->uIndx;
	pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "In: %s \n", __FUNCTION__);
	if(pInjectHandle->eInjecterState != E_INJECT_START)
	{
		return CSUDIINJECTER_ERROR_ALREADY_STARTED;
	}

	RetCode =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
	if(RetCode != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	do
	{
		if(pInjectHandle->eInjecterType == EM_INJECTER_TS)
		{        
			break;
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_ES)
		{
			//[alfredchen] here we could handle the ES Iframe case for display on video layer.
			break;
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
		{
            if( TRUE!=CSPcmInjectClearBuffer(pInjectHandle) )
            {
                RetCode =  cnxt_kal_sem_put(gInjecterSem);
                return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
            }
			break;
		}
	}while(0);
	RetCode =  cnxt_kal_sem_put(gInjecterSem);
	if(RetCode != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "End: %s  \n", __FUNCTION__);

	return (CSUDI_Error_Code)(eErrCode);
	return Retcode;
}

/**
@brief 获取下次注入到Injecter目标缓冲区中的空闲Buffer地址指针和连续空闲Buffer的大小。
@param[in] hINJECTER INJECTER数据注入实例句柄
@param[out] ppvBuffer 返回获取到的内存地址指针
@param[out] puLength 返回获取到的内存长度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果buffer已经满，再去调用此接口应该返回失败，且puLength长度为0
@note  该函数必须与CSUDIINJECTERWriteComplete配对使用，对该函数的调用要考虑线程安全
@code 
	char  	*pcBuf = NULL; 
	unsigned int unBufSize = 0;
	...
	if( (CSUDIINJECTERGetFreeBuffer(hInject, (void *)&pcBuf, &unBufSize) == CSUDI_SUCCESS) && (unBufSize > 0))
	{
		//往buffer中写入内容,  此处假设unWriteLen <= unBufSize
		memcpy(pcBuf, pcDataTobeWrite, unWriteLen );		
		CSUDIINJECTERWriteComplete(hInject, unWriteLen);
	}
	...
@endcode
*/
CSUDI_Error_Code CSUDIINJECTERGetFreeBuffer(CSUDI_HANDLE hINJECTER, void ** ppvBuffer, unsigned  int * puLength )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
	int nInjectIndx;
	CS_CNXT_Inject_SubSystem *pInjectHandle;
	CNXT_STATUS RetCode = CNXT_STATUS_OK;
	CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;

	IS_VALID_INJECTER(hINJECTER);
	if((ppvBuffer == NULL)||(puLength == NULL))
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FAIL: CSDMXGetFreeBuffer ppBuffer 0x%x pSize 0x%x\n", ppvBuffer, puLength);
		return CSUDIINJECTER_ERROR_BAD_PARAMETER;
	}
	
	nInjectIndx = phInjectHndl->uIndx;
	pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];

	if(pInjectHandle->eInjecterState != E_INJECT_START)
	{
		return CSUDIINJECTER_ERROR_ALREADY_STARTED;
	}

	RetCode =  cnxt_kal_sem_get(gInjecterSem, CNXT_KAL_WAIT_FOREVER);
	if(RetCode != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	do
	{
		if((pInjectHandle->eInjecterType == EM_INJECTER_TS)||
				(pInjectHandle->eInjecterType == EM_INJECTER_ES)||
				(pInjectHandle->eInjecterType == EM_INJECTER_PES))
		{           
			PIPE_AUDIO_OBJECT	*pAudioObj = NULL;
			PIPE_VIDEO_OBJECT   *pVideoObj = NULL;
			PIPE_AUDIO_ES_BUFFER_ATTRIB AudioEsBuff;
			PIPE_VIDEO_ES_BUFFER_ATTRIB VideoEsBuff;
            PIPE_STATUS PmStatus;
			pAudioObj = pInjectHandle->pAudObject;
			pVideoObj = pInjectHandle->pVidObject;
			AudioEsBuff.uUsedSpace = AudioEsBuff.uHwm = 0;
			VideoEsBuff.uUsedSpace = VideoEsBuff.uHwm = 0;
			if((pInjectHandle->nDmxIndx == 2)
			   &&(pInjectHandle->pInjectPipeObj->pAudioDecoder== NULL)
			   &&(pInjectHandle->pInjectPipeObj->pVideoDecoder== NULL) )
			{
				//for demux filter in ts inject mode, no audio or video obj
				*puLength = 0;				
				if((pInjectHandle->bTsBuffInUse == FALSE))
				{
					pInjectHandle->bTsBuffInUse = TRUE;
					*ppvBuffer = pInjectHandle->pTsBuff;
					*puLength = MAX_TS_BUF_LEN;
					pInjectHandle->uTsBufLen = *puLength;
				}
				else if(cntDmxFilterInInjectMode == 0)
				{
					pInjectHandle->bTsBuffInUse = FALSE;
				}
		}
		else
		{
			if(pAudioObj != NULL)
			{
				PmStatus = pAudioObj->get_es_buffer_attrib(pAudioObj, &AudioEsBuff);
                if(PmStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL," something serious \n");
                }
                //AudioEsBuff.uHwm = (AudioEsBuff.uHwm *3)/4;
			}
			if(pVideoObj != NULL)
			{
				PmStatus = pVideoObj->get_es_buffer_attrib(pVideoObj, &VideoEsBuff);
                if(PmStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL," something serious \n");
                }
                //VideoEsBuff.uHwm = (VideoEsBuff.uHwm *1)/2;
			}
            *puLength = 0;
			
			if((AudioEsBuff.uUsedSpace <= (AudioEsBuff.uHwm/2))&&
					(VideoEsBuff.uUsedSpace <= VideoEsBuff.uHwm) &&
					(bPlayerStarted == TRUE)&&
					(pInjectHandle->bTsBuffInUse == FALSE)&&(bPlayerStatus == 0))
			{
				pInjectHandle->bTsBuffInUse = TRUE;
				*ppvBuffer = pInjectHandle->pTsBuff;
				*puLength = MAX_TS_BUF_LEN;
				pInjectHandle->uTsBufLen = *puLength;
			}
            else if(bPlayerStarted == FALSE)
            {
				pInjectHandle->bTsBuffInUse = FALSE;                
            }
            //CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"started %d Inuse %d \n", bPlayerStarted, pInjectHandle->bTsBuffInUse);
            }
			break;
		}	    
		else if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
		{
	          if( TRUE!=CSPcmInject_GetFreeBuffer(pInjectHandle, ppvBuffer, puLength) )
	          {
	              eErrCode=CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
                  CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Fail: %s %d\n", __FUNCTION__, __LINE__);
	          }
		}
	}while(0);
	RetCode = cnxt_kal_sem_put(gInjecterSem);
	if(RetCode != CNXT_STATUS_OK)
	{
		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	}
	//CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "End: %s  eErrCode 0x%x len %d \n", __FUNCTION__, eErrCode, *puLength);
	return (CSUDI_Error_Code)(eErrCode);
	return Retcode;
}

/**
@brief 通知本次注入的数据
@param[in] hINJECTER INJECTER数据注入实例句柄
@param[in] uWrittenSize 已拷贝数据的大小
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 如果buffer长度为nLength, 注入长度大于nLength,则该接口返回CSUDIINJECTER_ERROR_NO_MEMORY
@note  该函数必须与CSUDIINJECTERGetFreeBuffer配对使用，对该函数的调用要考虑线程安全
*/
CSUDI_Error_Code CSUDIINJECTERWriteComplete(CSUDI_HANDLE hINJECTER, unsigned int uWrittenSize)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	UDI_INJECT_HNDL *phInjectHndl = (UDI_INJECT_HNDL *)hINJECTER;
	int nInjectIndx;
	CS_CNXT_Inject_SubSystem *pInjectHandle;
	CNXT_STATUS RetCode = CNXT_STATUS_OK;
	CSUDI_Error_Code eErrCode = CSUDI_SUCCESS;
	PIPE_STATUS PmStatus = PIPE_STATUS_OK;
	u_int32 uMsg[4];

	//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d Enter Size %d \n", __FUNCTION__, __LINE__, uWrittenSize);
	IS_VALID_INJECTER(phInjectHndl);
	if(uWrittenSize<0)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Fail: %s writtensize 0x%x\n", __FUNCTION__, uWrittenSize);
		return CSUDIINJECTER_ERROR_BAD_PARAMETER;
	}
	
	nInjectIndx = phInjectHndl->uIndx;
	pInjectHandle = &g_sCSInjectConfig.hInjectHandle[nInjectIndx];

	if(pInjectHandle->eInjecterState != E_INJECT_START)
	{
		return CSUDIINJECTER_ERROR_ALREADY_STARTED;
	}
	if(uWrittenSize > pInjectHandle->uTsBufLen)
	{
		return CSUDIINJECTER_ERROR_BAD_PARAMETER;
	}

    if((s_aInjectHndl[nInjectIndx].bInjecterOpen ==FALSE)||(pInjectHandle->eInjecterState == E_INJECT_STOP))
    {
        return CSUDI_SUCCESS;
    }

	if( pInjectHandle->eInjecterType == EM_INJECTER_TS )
	{	
		if( uWrittenSize == 0 )
		{
			pInjectHandle->bTsBuffInUse = FALSE;
			return eErrCode; 
		}
		if(uWrittenSize>0){
			IPVOD_buf_append(pInjectHandle->pTsBuff,uWrittenSize);
			total_size += uWrittenSize;
			if( (total_size < (IPVOD_BUF_SIZE / 2))&&(gFristFlag==1))
			{
				pInjectHandle->bTsBuffInUse = FALSE;
				gFristFlag = 0;
				total_size = 0;
				return CSUDI_SUCCESS;
			}
		}
		if (  (pInjectHandle->nDmxIndx == 2)
			&&(pInjectHandle->pInjectPipeObj->pAudioDecoder==NULL)
			&&(pInjectHandle->pInjectPipeObj->pVideoDecoder== NULL))
		{
			if(cntDmxFilterInInjectMode == 0)
			{
				pInjectHandle->bTsBuffInUse = FALSE;
				return CSUDI_SUCCESS;
			}
		}
		else
		{
			if(bPlayerStarted != TRUE){
				pInjectHandle->bTsBuffInUse = FALSE;				
				return CSUDI_SUCCESS;
			}
		}
	
		if ((IPVOD_buf_used < gMinSendSize))//&&(tempflag == 1)) 
        {
			pInjectHandle->bTsBuffInUse = FALSE;
            //printf("IPVOD_buf_used[%d * 1316] NOT enough\n",IPVOD_buf_used/1316);
			return CSUDI_SUCCESS;
		}		

	}
    
	if(pInjectHandle->eInjecterType == EM_INJECTER_PCM )
	{
			//pcm inject,do nothing
	}
	else
	{
		RetCode = cnxt_kal_sem_get(gPlayerSem, CNXT_KAL_WAIT_FOREVER);
        if(RetCode != CNXT_STATUS_OK)
	    {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"%s %d cnxt_kal_sem_put fail  \n", __FUNCTION__, __LINE__);
    		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
	    }
		if(cnxt_kal_qu_receive(injectDataQueue, CNXT_KAL_WAIT_FOREVER, (void*)uMsg) != CNXT_STATUS_OK)
		{
			printf( " receive queue ...\n\r " );
		}
        
		//2011-03-19 TerenceZhang begin
		if((s_aInjectHndl[nInjectIndx].bInjecterOpen ==FALSE)||(pInjectHandle->eInjecterState == E_INJECT_STOP))
		{
			RetCode = cnxt_kal_sem_put(gPlayerSem);
			if(RetCode != CNXT_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"%s %d cnxt_kal_sem_put fail  \n", __FUNCTION__, __LINE__);
				return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
			}
			return CSUDI_SUCCESS;
		}
		//2011-03-19 TerenceZhang end		      
		
	}
    
	do
	{
		if(pInjectHandle->eInjecterType == EM_INJECTER_TS)
		{
			PIPE_PIPELINE_OBJECT     *pInjectPipeObj = pInjectHandle->pInjectPipeObj;
			PIPE_MEDIA_DATA_INFO MediaDataInfo;
			
			MediaDataInfo.Type = PIPE_MEDIA_TS_DATA;
#if 1
			if(IPVOD_buf_used > MAX_SEND_SIZE)
			{
				MediaDataInfo.uLength = MAX_SEND_SIZE;
			}
			else
			{
				MediaDataInfo.uLength = IPVOD_buf_used;
			}
			memcpy( pInternalBuffer, IPVOD_buf_read(MediaDataInfo.uLength),MediaDataInfo.uLength);
#endif			
			MediaDataInfo.pData   = pInternalBuffer;//pInjectHandle->pTsBuff;
			MediaDataInfo.uOverlap = 0;
			MediaDataInfo.uPts= 0;
			
			if (  (pInjectHandle->nDmxIndx == 2)
				&&(pInjectHandle->pInjectPipeObj->pAudioDecoder==NULL)
				&&(pInjectHandle->pInjectPipeObj->pVideoDecoder==NULL))
			{
				if (cntDmxFilterInInjectMode != 0)
				{
					//hw demux filter mode, without bPlayerStarted == TRUE
					if(uWrittenSize>0)
					{
						do
						{
							PmStatus = pInjectPipeObj->inject_media_data(pInjectPipeObj, &MediaDataInfo);
							 /*This means that */
							if(PmStatus == PIPE_STATUS_DRIVER_ERROR)
							{
								printf(" inject media fail....\n\r ");
								cnxt_kal_thread_time_sleep(10);
							}
						}while(0);					  
					}
					pInjectHandle->bTsBuffInUse = FALSE;
				}
			}
			else
			{
				//playback mode
				if(bPlayerStarted == TRUE)
				{
	                if(uWrittenSize>0)
	                {
	    			    do
	                    {
	        				PmStatus = pInjectPipeObj->inject_media_data(pInjectPipeObj, &MediaDataInfo);
	        				 /*This means that */
	                        if(PmStatus == PIPE_STATUS_DRIVER_ERROR)
	                        {
	                            //printf("inject media failed....[%d]MediaDataInfo.uLength[%d * 1316]\n",PmStatus,MediaDataInfo.uLength/1316);
								cnxt_kal_thread_time_sleep(10);                                
                                if((s_aInjectHndl[nInjectIndx].bInjecterOpen ==FALSE)||(pInjectHandle->eInjecterState == E_INJECT_STOP))
                                {
                                    RetCode = cnxt_kal_sem_put(gPlayerSem);
                                    if(RetCode != CNXT_STATUS_OK)
                                    {
                                        CSDEBUG(MODULE_NAME, ERROR_LEVEL,"%s %d cnxt_kal_sem_put fail  \n", __FUNCTION__, __LINE__);
                                        return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
                                    }
                                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR:::%s %d   \n", __FUNCTION__, __LINE__);
                                    return CSUDI_SUCCESS;
                                }
	                        }
                            else
                            {
                                if(tempsuccesscounter > 1)
                                {
                                    //printf("inject media success....[%d]MediaDataInfo.uLength[%d * 1316]\n",PmStatus,MediaDataInfo.uLength/1316);
                                    cnxt_kal_thread_time_sleep(10);
                                    tempsuccesscounter--;
                                }
                                //cnxt_kal_thread_time_sleep(10);
                            }
#if 1                            
                            if(gMinSendSize > 1316)
                            {
                                gMinSendSize = gMinSendSize - 1316*5;
                            }
                            else
                            {
                                gMinSendSize = 1000;
                            }
                            if((s_aInjectHndl[nInjectIndx].bInjecterOpen ==FALSE)||(pInjectHandle->eInjecterState == E_INJECT_STOP))
		                    {
                                RetCode = cnxt_kal_sem_put(gPlayerSem);
			                    if(RetCode != CNXT_STATUS_OK)
			                    {
                                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"%s %d cnxt_kal_sem_put fail  \n", __FUNCTION__, __LINE__);
				                    return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
			                    }
                                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"ERROR:::%s %d   \n", __FUNCTION__, __LINE__);
			                    return CSUDI_SUCCESS;
		                    }
#endif                            
                             
	                    }while(0);                    
	                }
					pInjectHandle->bTsBuffInUse = FALSE;
				}
			}
			break;
		}
		else if((pInjectHandle->eInjecterType == EM_INJECTER_ES)||
				(pInjectHandle->eInjecterType == EM_INJECTER_PES))
		{
			PIPE_PIPELINE_OBJECT     *pInjectPipeObj = pInjectHandle->pInjectPipeObj;
			PIPE_MEDIA_DATA_INFO MediaDataInfo;
			/* receiving a data buffer */
			if(pInjectHandle->nAudIndx >= 0)
			{
				MediaDataInfo.Type = PIPE_MEDIA_AUDIO_ES_DATA;			
			}
			else if(pInjectHandle->nVidIndx >= 0)
			{
				MediaDataInfo.Type = PIPE_MEDIA_VIDEO_ES_DATA;			
			}
			MediaDataInfo.pData   = pInjectHandle->pTsBuff;
			MediaDataInfo.uLength = uWrittenSize;
			if(bPlayerStarted == TRUE)
			{
				PmStatus = pInjectPipeObj->inject_media_data(pInjectPipeObj, &MediaDataInfo);
				//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"###Pmstatus %d \n", PmStatus);
				pInjectHandle->bTsBuffInUse = FALSE;
			}
			break;
		}
		else if(pInjectHandle->eInjecterType == EM_INJECTER_PCM)
		{
	        if( TRUE!=CSPcmInject_WriteComplete(pInjectHandle, uWrittenSize) )
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Fail: %s writtensize %d \n", __FUNCTION__, uWrittenSize);
                eErrCode = CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
            }
			break;
		}
	}while(0);
    if(pInjectHandle->eInjecterType != EM_INJECTER_PCM)
    {
    	RetCode = cnxt_kal_sem_put(gPlayerSem);
    	if(RetCode != CNXT_STATUS_OK)
    	{
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"%s %d cnxt_kal_sem_put fail  \n", __FUNCTION__, __LINE__);
    		return CSUDIINJECTER_ERROR_UNKNOWN_ERROR;
    	}
    }
    //CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"%s %d end  \n", __FUNCTION__, __LINE__);
	return (CSUDI_Error_Code)(eErrCode);
	return Retcode;
}

