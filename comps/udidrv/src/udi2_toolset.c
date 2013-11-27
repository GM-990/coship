/****************************************************************************/
/*
* Filename:        tm_cs_toolset.c
*
*
* Description:     API implementation for COSHIP interface layer .
*
*
* Author:          Trident Design-In Team
*
****************************************************************************/

/*****************/
/* Include Files */
/*****************/
#include "generic_include.h"
#include "tm_inject.h"
#include "tm_player.h"
#include "tm_screen.h"
#include "tm_video.h"
// 2011-01-17 TerenceZhang add for real standby feature.
#include "tmscsScd.h"
#include "cs_toolset_plus.h"
#include "panel_buttons.h"
#include "data_mngr.h"
#include "pvr_fs.h"
#include "cbuf.h"
#include "record.h"

#define CSSYS_PLATFORM_VERSION   ("Trident.1.0")
#define PortingLibraryVersion    ("PORTING-Trident-Shiner")
#define MODULE_NAME              ("CS_TOOLSET")

/*lichanghua :用于进程退出自动测试*/
//#define PROCESS_EXIT_TEST

/******************************************************************************
*                                global Variables                                                                                *
******************************************************************************/
extern CNXT_STATUS cnxt_plat_init_device(void);
extern CNXT_STATUS cnxt_plat_term_device(void);
extern CNXT_STATUS cnxt_pvr_drv_init(void);
extern CNXT_STATUS cnxt_css_drv_init(void);
extern CNXT_STATUS cnxt_css_drv_term(void);
extern CNXT_STATUS cnxt_scs_init_device(void);
extern int CSUDISMCInit (void);
extern int cnxt_vss_drv_init(void);
extern CNXT_STATUS CS_TRID_OSG_init(CSUDIOSGPixelFormat_E ePixelFormat);
CS_TM_PIPE_OBJECTS gTmPipeObject;
CS_TM_VP_DEVICE_OBJECTS gTmVpDeviceObj;
CNXT_GRAPHICS_HANDLE   gTridGraphicsHandle;
CS_TM_AP_DEVICE_OBJECTS gTmApDeviceObj;

// 2011-01-17 TerenceZhang begin
#define COSHIP_IRRX_INIT_MASK      (1UL<<0)
#define COSHIP_GPIOEXP_INIT_MASK   (1UL<<1)
#define COSHIP_IIC_INIT_MASK       (1UL<<2)
#define COSHIP_BUTTONS_INIT_MASK   (1UL<<3)
#define COSHIP_SCD_INIT_MASK       (1UL<<4)
#define COSHIP_SCD_OPEN_MASK       (1UL<<5)

//#define UDI2_WAKEUP_BY_FRONT_PANEL     //support FP WAKEUP maybe implement in firmware.
#define UDI2_WAKEUP_BY_IR
 
extern u_int8 *pRecordData;
CNXT_CBUF_HANDLE gCbuf;

static CNXT_SCD_HANDLE hgScd = NULL;
// 2011-01-17 TerenceZhang end

static CSUDIChipStandbyHookFunc_F s_fnChipStandbyHookFunc = NULL;
/*初始化默认支持nec 遥控器唤醒*/
static CSUDISYSWakeupParams_S s_sSYSWakeupParams = {
    .m_eWakeUpMode = CSUDISYS_WAKEUPE_MODE_IR,
    .m_bModeEnable = TRUE, 
    .m_uWakeupParams.m_stIRKeys={
        .m_astKeys ={
                        {
                            .m_enmIRProtocol = CSIR_PROTOCOL_NEC,
                            .m_nKeyCode = 0xba45,
                        },
                     },
        .m_nKeyNum = 1,
        },
    };

/******************************************************************************
*                                 Local Variables                                                                                *
******************************************************************************/
#ifdef DUAL_VIDEO_SURFACE
extern void cs_tm_display_update();
#endif
extern void cs_tm_playback_notify( PIPE_PIPELINE_OBJECT *pPipeline,
                                    void                     *pUserData,
                                    PIPE_NOTIFY_EVENT         Event,
                                    void                     *pData,
                                    void                     *pTag );
static CNXT_STATUS  hardware_init(void);
static CNXT_STATUS  create_objects(void);
extern void cs_tm_notify_player(CSUDIPlayerEventType_E UDIEvent);
extern void cs_frontpanel_power_notify (CNXT_BUTTONS_HANDLE hHandle, 
                                        void               *pUserData,
                                        CNXT_BUTTONS_EVENT  Event,
                                        void               *pData,
                                        void               *pTag );
void Coship_pipe_event_notifier( struct pipe_pipeline_obj *pPipe,
                                    void *pUserData,
                                    PIPE_NOTIFY_EVENT Event,
                                    void *pData,
                                    void *pTag )
{
    switch(Event)
    {
    case PIPE_EVENT_PIPELINE_STATE_CHANGED:
        //CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_PIPELINE_STATE_CHANGED \n");
        break;
    case PIPE_EVENT_TUNER_LOCKED:
        //CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_TUNER_LOCKED \n");
        break;
    case PIPE_EVENT_TUNER_SIGNAL_LOST :
        //CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_TUNER_SIGNAL_LOST \n");
        break;
    case PIPE_EVENT_VIDEO_DECODE_COMPLETE:
        //CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_DECODE_COMPLETE \n");
        break;
    case PIPE_EVENT_VIDEO_STARTED :
        //CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_STARTED \n");
        break;
    case PIPE_EVENT_VIDEO_DISPLAY_UPDATE :
        //printf("<CS_TOOLSET> [Coship_pipe_event_notifier] PIPE_EVENT_VIDEO_DISPLAY_UPDATE \n");
#ifdef DUAL_VIDEO_SURFACE
        cs_tm_display_update();
        cs_tm_notify_player(EM_UDIPLAYER_VIDEO_FRAME_COMING);	

#else
        cs_tm_notify_player(EM_UDIPLAYER_VIDEO_FRAME_COMING);
#endif
        break;
    case PIPE_EVENT_VIDEO_PLAYING :
        //CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_PLAYING \n");
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
        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STREAM_LOST \n");
        break;
    case PIPE_EVENT_AUDIO_SYNC_ACQ :
        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_ACQ \n");
        break;
    case PIPE_EVENT_AUDIO_SYNC_LOST :
        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_LOST \n");
        break;
    case PIPE_EVENT_AUDIO_STREAM_CHANGE :
        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STREAM_CHANGE \n");
        break;        
    case PIPE_EVENT_AUDIO_BUFFER_EMPTY :
        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_BUFFER_EMPTY \n");
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
    case PIPE_EVENT_DEMUX_SYNC_ACQUIRED:
    case PIPE_EVENT_DEMUX_SYNC_LOST:
    case PIPE_EVENT_DEMUX_HDR_ERROR:
    case PIPE_EVENT_DEMUX_FILTER_DATA:
    case PIPE_EVENT_DEMUX_FILTER_BUF_OVERFLOW:
    case PIPE_EVENT_DEMUX_FILTER_ERROR:
    case PIPE_EVENT_DEMUX_FILTER_DATA_PES:
            //printf("PIPE_EVENT_DEMUX_FILTER \n");
        Section_filter_event_notifier(pPipe,pUserData,Event,pData,pTag);
		break;	
	case PIPE_EVENT_PIPELINE_RECORD_DATA:
		{
			CNXT_STATUS Retcode;
            PIPE_STATUS PmStatus;
			PIPE_VIDEO_OBJECT	*pVideoObj = NULL;
			u_int32 uAvailableData=0,pNumBytesRead;
			PIPE_MEDIA_DATA_INFO MediaDataInfo;
			PIPE_VIDEO_ES_BUFFER_ATTRIB VideoEsBuff;
			CNXT_RECORD_DATA *pRecBuf = (CNXT_RECORD_DATA*)pData;			
			u_int32 uBytesRead = 0;			

			pVideoObj = gTmPipeObject.hPipeObj[0]->pVideoDecoder;

			if(pVideoObj != NULL)
			{
				PmStatus = pVideoObj->get_es_buffer_attrib(pVideoObj, &VideoEsBuff);
				if(PmStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL," something serious \n");
				}
			}
			if(VideoEsBuff.uUsedSpace <= VideoEsBuff.uHwm)
			{
				if( cnxt_cbuf_read_data( pRecBuf->hCbuf, pRecordData, pRecBuf->uLength, &uBytesRead ) != CNXT_STATUS_OK)
				{
					printf("READ CBUF FAIL\n");
				}				
				MediaDataInfo.Type = PIPE_MEDIA_TS_DATA;			
				MediaDataInfo.pData   = pRecordData;
				MediaDataInfo.uLength = pRecBuf->uLength;	
				if(gTmPipeObject.hPipeObj[0]->inject_media_data(gTmPipeObject.hPipeObj[0], &MediaDataInfo) != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, DEBUG_LEVEL," trick mode inject media data fail \n");
				}

			}
		}
		break;
    default:
        //CSDEBUG(MODULE_NAME, DEBUG_LEVEL," UnKown Event =%d \n",Event);
        break;
    }
}



static CNXT_STATUS  hardware_init(void)
{
    PIPE_PIPELINE_MNGR_CFG PipelineInitCfg;
    CNXT_GRAPHICS_CAPS       GraphicsCaps;
    CNXT_STATUS Result       = CNXT_STATUS_OK;
    CSUDI_Error_Code retCode =CSUDI_FAILURE;

    do{
        cnxt_kal_memset(&gTmPipeObject, 0, sizeof(CS_TM_PIPE_OBJECTS));
        cnxt_kal_memset(&gTmVpDeviceObj, 0, sizeof(CS_TM_VP_DEVICE_OBJECTS));
        cnxt_kal_memset(&gTmApDeviceObj, 0, sizeof(CS_TM_AP_DEVICE_OBJECTS));
        
        Result = cnxt_css_drv_init();
        if(CNXT_STATUS_OK != Result)
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"ERROR: Could not initialize css driver err = %d Exiting...\n", Result);
            cnxt_plat_term_device ();
            retCode=CSUDI_FAILURE;
            break;

        }
      
        Result = cnxt_pvr_drv_init();
        if(CNXT_STATUS_OK != Result)
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"ERROR: Could not initialize pvr driver err = %d Exiting...\n", Result);
            cnxt_plat_term_device ();
            retCode=CSUDI_FAILURE;
            break;

        }
		
       Result = cnxt_cbuf_init(NULL);
       if((CNXT_STATUS_OK != Result)&&(CNXT_STATUS_ALREADY_INIT != Result))
       {
            CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Init Cbuf\n");
            retCode = CSUDI_FAILURE;
            break;
       }
        
        Result =  pipe_vp_init_manager();
        if (Result!= PIPE_STATUS_OK && Result != PIPE_STATUS_ALREADY_INIT)
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"pm_init_manager failed with error: %x\n", Result);
            retCode=CSUDI_FAILURE;
            break;
        }

		/* init pipeline */
		PipelineInitCfg.uNumOfMotionDecoders = 1;//2-->dualhd;
		PipelineInitCfg.uVideoFrameRate = 25;

        if (pipe_pipeline_init(&PipelineInitCfg) != PIPE_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"pipe_pipeline_init failed\n");
            retCode=CSUDI_FAILURE;
            break;
        }

        if (tm_section_init() != CSUDI_SUCCESS)
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"trid_section_init failed\n");
            retCode=CSUDI_FAILURE;
            break;
        }

        if (tm_desc_init() != CSUDI_SUCCESS)
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"trid_section_init failed\n");
            retCode=CSUDI_FAILURE;
            break;
        }
        
        Result = cnxt_vss_drv_init ();
        if(CNXT_STATUS_OK != Result)
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"ERROR: Could not initialize scs driver err = %d Exiting...\n", Result);           
            retCode=CSUDI_FAILURE;
            break;
        }

		
        Result = cnxt_image_init(NULL);
        if ((Result != CNXT_STATUS_ALREADY_INIT) && (Result != CNXT_STATUS_OK))
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"cnxt_image_init failed %d\n", Result);
            retCode=CSUDI_FAILURE;
            break;
        }
		
        Result = cnxt_graphics_init(NULL);
        if((Result != CNXT_STATUS_ALREADY_INIT) && (Result != CNXT_STATUS_OK))
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"cnxt_graphics_init failed\n");
            retCode=CSUDI_FAILURE;
            break;
        }
        GraphicsCaps.uUnitNumber = 0;
        /* open a graphics handle */
        Result = cnxt_graphics_open(&gTridGraphicsHandle, &GraphicsCaps, NULL, NULL);
        if (Result != CNXT_STATUS_OK)
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"cnxt_graphics_open failed\n");
            retCode=CSUDI_FAILURE;
            break;
        }
#if 0        
        Result = cnxt_buttons_init(NULL);
        if(Result != CNXT_STATUS_OK && Result != CNXT_STATUS_ALREADY_INIT)
        {
            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
            retCode=CSUDI_FAILURE;
            break;           
        }
        
        {
            CNXT_BUTTONS_HANDLE    hHandle;
            CNXT_BUTTONS_CAPS      Caps;
            Caps.bEnableReset = 0;
            Caps.uTimeoutMs = 10;
            Caps.uUnitNum = 0;
            Result= cnxt_buttons_open(&hHandle,&Caps,cs_frontpanel_power_notify,NULL);
            if(Result != CNXT_STATUS_OK && Result != CNXT_STATUS_ALREADY_INIT)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                retCode=CSUDI_FAILURE;
                break;           
            }
        }
#endif
        retCode=CSUDI_SUCCESS;
        
    }while(0);

    return retCode;
}

BOOL PanelStandbyPlatInit()
{
	CNXT_STATUS Result       = CNXT_STATUS_OK;
	CNXT_BUTTONS_HANDLE    hHandle;
       CNXT_BUTTONS_CAPS      Caps;
	BOOL bRet = FALSE;
 
	do{
		Result = cnxt_buttons_init(NULL);
	        if(Result != CNXT_STATUS_OK && Result != CNXT_STATUS_ALREADY_INIT)
	        {
	            CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
	            break;           
	        }
        
            Caps.bEnableReset = 0;
            Caps.uTimeoutMs = 10;
            Caps.uUnitNum = 0;
            //Result= cnxt_buttons_open(&hHandle,&Caps,cs_frontpanel_power_notify,NULL);
            if(Result != CNXT_STATUS_OK && Result != CNXT_STATUS_ALREADY_INIT)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                break;           
            }

		bRet = TRUE;
        }while(0);

	return bRet;
}

static CNXT_STATUS create_objects(void)
{
    u_int32   uEventMask        = 0;
    u_int8    uIndex = 0;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    PIPE_DEMUX_CFG    DemuxCfg;

    do
    {
        for (uIndex = 0; uIndex < MAX_PIPE_OBJECTS ;uIndex++)
        {
            printf("Creating PIPE No(%d)\n",uIndex);
            DemuxCfg.Type              = PIPE_DEMUX_TS;
            DemuxCfg.DescramblerType   = PIPE_DEMUX_DESCRAMBLE_DVB;
            gTmPipeObject.hPipeObj[uIndex] = PIPE_OBJ_CREATE ( pipeline );
            if  ( gTmPipeObject.hPipeObj[uIndex] == NULL ) 
            {
                CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot open Pipe necessary objects\n" );
                retCode = CSUDI_FAILURE;
                break;
            }
            else if ( gTmPipeObject.hPipeObj[uIndex]->set_notifier( gTmPipeObject.hPipeObj[uIndex], Coship_pipe_event_notifier,NULL) != PIPE_STATUS_OK )
            {
                CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Cannot set the notifier objects\n" );
                retCode = CSUDI_FAILURE;
                break;
            }
            
            if(uIndex < 3)/*Add only for live & TS playback...for playback it is done in inject*/
            {
                gTmPipeObject.hDemuxObj[uIndex] = PIPE_OBJ_CREATE ( demux );
                if ( gTmPipeObject.hDemuxObj[uIndex] == NULL )
                {
                    CSDEBUG(MODULE_NAME,ERROR_LEVEL," demux object create failed \n" );
                    retCode = CSUDI_FAILURE;
                    break;
                }
                if ( gTmPipeObject.hPipeObj[uIndex]->add_device(gTmPipeObject.hPipeObj[uIndex], (PIPE_OBJ_HANDLE)gTmPipeObject.hDemuxObj[uIndex])!= PIPE_STATUS_OK )
                {
                    CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot add demux to pipeline\n" );
                    retCode = CSUDI_FAILURE;
                    break;
                } 
            }
            
            if(uIndex >= 2)
            {
                if ( gTmPipeObject.hPipeObj[uIndex]->set_notifier( gTmPipeObject.hPipeObj[uIndex], cs_tm_playback_notify,NULL) != PIPE_STATUS_OK )
                {
                    CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Cannot set the notifier objects\n" );
                    retCode = CSUDI_FAILURE;
                    break;
                }
                if (2 == uIndex)
                {
                       gTmPipeObject.hDemuxObj[2]->config(gTmPipeObject.hDemuxObj[2], &DemuxCfg);
                }                                   
            }
            else if ( gTmPipeObject.hDemuxObj[uIndex]->config(gTmPipeObject.hDemuxObj[uIndex], &DemuxCfg) != PIPE_STATUS_OK )
            {
                CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Cannot config demux\n" );
                retCode = CSUDI_FAILURE;
                break;
            }
        }
        uIndex = 0;
        printf("%s %d \n", __FUNCTION__, __LINE__);
        gTmPipeObject.hTunerObj[uIndex]= PIPE_OBJ_CREATE ( tuner );
        if ( gTmPipeObject.hTunerObj[uIndex] == NULL )
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Failed in openning tuner \n" );
            retCode = CSUDI_FAILURE;
            break;
        }

         printf("%s %d \n", __FUNCTION__, __LINE__);
         PIPE_TUNER_CFG    TunerCfg;
#if 1
         TunerCfg.Type  =  PIPE_CABLE_TUNER_TYPE;
#else
		 TunerCfg.Type  =  PIPE_SAT_TUNER_TYPE;
#endif
         if ( gTmPipeObject.hTunerObj[uIndex]->config ( gTmPipeObject.hTunerObj[uIndex], &TunerCfg ) != PIPE_STATUS_OK )
         {
/*<!-- Modify by 903126,2010-12-25 15:23:26: */
/* Note: we comment these becasuase on iptv product they don't use tuner and 
it becomes a disaster if here breaking out */
           //retCode = CSUDI_FAILURE;
           printf("Failed in config tuner !!!!!\n" );
           //break;
 /* Modify end 2010-12-25 15:23:26 --!>*/
         }

         printf("%s %d \n", __FUNCTION__, __LINE__);
         gTmPipeObject.hVideoObj[uIndex] = PIPE_OBJ_CREATE ( video );
         if  ( gTmPipeObject.hVideoObj[uIndex] == NULL ) 
         {
               CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot create video objects\n" );
               retCode = CSUDI_FAILURE;
               break;
         }
         /*We need to provide 2 video instances
         gTmPipeObject.hVideoObj[uIndex+1] = PIPE_OBJ_CREATE ( video ); 
         if  ( gTmPipeObject.hVideoObj[uIndex+1] == NULL ) 
         {
               CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot create video objects\n" );
               retCode = CSUDI_FAILURE;
               break;
         }*/
#ifdef DUAL_VIDEO_SURFACE

         printf("%s %d \n", __FUNCTION__, __LINE__);
         uEventMask = ((1 << (PIPE_EVENT_VIDEO_STARTED & 0xFFFFFF)) |
                      (1 << (PIPE_EVENT_VIDEO_DISPLAY_UPDATE & 0xFFFFFF)) |
                      (1 << (PIPE_EVENT_VIDEO_PLAYING & 0xFFFFFF))|
                      (1 << (PIPE_EVENT_VIDEO_BUFFER_EMPTY & 0xFFFFFF))|                      
                      (1<<(PIPE_EVENT_VIDEO_STREAM_CHANGE & 0xFFFFFF)));
         gTmPipeObject.hVideoObj[uIndex]->subscribe_event(gTmPipeObject.hVideoObj[uIndex], uEventMask); 
         //gTmPipeObject.hVideoObj[uIndex+1]->subscribe_event(gTmPipeObject.hVideoObj[uIndex+1], uEventMask); 

#ifndef VDOBJ_MALLOC
        uIndex = 1;
         gTmPipeObject.hVideoObj[uIndex] = PIPE_OBJ_CREATE ( video );
         if  ( gTmPipeObject.hVideoObj[uIndex] == NULL ) 
         {
               CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Cannot create video objects\n" );
               retCode = CSUDI_FAILURE;
               break;
         }

         uEventMask = ((1 << (PIPE_EVENT_VIDEO_STARTED & 0xFFFFFF)) |
                      (1 << (PIPE_EVENT_VIDEO_PLAYING & 0xFFFFFF))|
                      (1 << (PIPE_EVENT_VIDEO_BUFFER_EMPTY & 0xFFFFFF))|                      
                      (1 << (PIPE_EVENT_VIDEO_DECODE_COMPLETE & 0xFFFFFF))|
                      (1<<(PIPE_EVENT_VIDEO_STREAM_CHANGE & 0xFFFFFF)));
         gTmPipeObject.hVideoObj[uIndex]->subscribe_event(gTmPipeObject.hVideoObj[uIndex], uEventMask); 
#else
	gTmPipeObject.hVideoObj[1] = NULL;
#endif
#else
         printf("%s %d \n", __FUNCTION__, __LINE__);
         uEventMask = ((1 << (PIPE_EVENT_VIDEO_STARTED & 0xFFFFFF)) |
                      (1 << (PIPE_EVENT_VIDEO_DISPLAY_UPDATE & 0xFFFFFF)) |
                      (1 << (PIPE_EVENT_VIDEO_PLAYING & 0xFFFFFF))|
                      (1 << (PIPE_EVENT_VIDEO_BUFFER_EMPTY & 0xFFFFFF))|                      
                      (1 << (PIPE_EVENT_VIDEO_DECODE_COMPLETE & 0xFFFFFF))|
                      (1<<(PIPE_EVENT_VIDEO_STREAM_CHANGE & 0xFFFFFF)));
         gTmPipeObject.hVideoObj[uIndex]->subscribe_event(gTmPipeObject.hVideoObj[uIndex], uEventMask); 
         //gTmPipeObject.hVideoObj[uIndex+1]->subscribe_event(gTmPipeObject.hVideoObj[uIndex+1], uEventMask); 

#endif
         for(uIndex =0; uIndex<MAX_AUDIO_OBJECTS; uIndex++)
         {         
             printf("%s %d \n", __FUNCTION__, __LINE__);
             gTmPipeObject.hAudioObj[uIndex] = PIPE_OBJ_CREATE ( audio );
             if  ( gTmPipeObject.hAudioObj[uIndex] == NULL ) 
             {
                   CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Cannot create audio objects\n" );
                   retCode = CSUDI_FAILURE;
                   break;
             }
             uEventMask = (1 << (PIPE_EVENT_AUDIO_STOPPED & 0xFFFFFF)) |
                          (1 << (PIPE_EVENT_AUDIO_STREAM_CHANGE & 0xFFFFFF)) |
                          (1 << (PIPE_EVENT_AUDIO_STARTED & 0xFFFFFF)) |
                          (1 << (PIPE_EVENT_AUDIO_STREAM_LOST & 0xFFFFFF)) |
                          (1 << (PIPE_EVENT_AUDIO_SYNC_ACQ & 0xFFFFFF)) |
                          (1 << (PIPE_EVENT_AUDIO_SYNC_LOST & 0xFFFFFF)) |
                          (1 << (PIPE_EVENT_AUDIO_BUFFER_EMPTY & 0xFFFFFF));
             gTmPipeObject.hAudioObj[uIndex]->subscribe_event(gTmPipeObject.hAudioObj[uIndex], uEventMask);
             
         }
        retCode = CSUDI_SUCCESS;
    }while(0);

    
    return retCode;
}

#define DO(Fun) \
{	\
	unsigned int unRet; \
	if((unRet=Fun)) \
	{ \
		printf("fail to call %s ret:%x\n",#Fun,unRet); \
	} \
}

BOOL DestroyPipeObjects()
{
	int uIndex;
	BOOL bRet = FALSE;

	for (uIndex = 0; uIndex < MAX_PIPE_OBJECTS ;uIndex++)
	{
		printf("check %d pipeline\n",uIndex);
		if (gTmPipeObject.hPipeObj[uIndex]->pTuner)
		{	
			printf("del tuner:%x\n",gTmPipeObject.hPipeObj[uIndex]->pTuner);
			DO(gTmPipeObject.hPipeObj[uIndex]->delete_device(gTmPipeObject.hPipeObj[uIndex], gTmPipeObject.hPipeObj[uIndex]->pTuner));
		}
		if (gTmPipeObject.hPipeObj[uIndex]->pDemux)
		{
			printf("del pDemux:%x\n",gTmPipeObject.hPipeObj[uIndex]->pDemux);
			DO(gTmPipeObject.hPipeObj[uIndex]->delete_device(gTmPipeObject.hPipeObj[uIndex], gTmPipeObject.hPipeObj[uIndex]->pDemux));
		}
		if (gTmPipeObject.hPipeObj[uIndex]->pVideoDecoder)
		{
			printf("del pVideoDecoder:%x\n",gTmPipeObject.hPipeObj[uIndex]->pVideoDecoder);
			DO(gTmPipeObject.hPipeObj[uIndex]->delete_device(gTmPipeObject.hPipeObj[uIndex], gTmPipeObject.hPipeObj[uIndex]->pVideoDecoder));
		}
		if (gTmPipeObject.hPipeObj[uIndex]->pAudioDecoder)
		{
			printf("del pAudioDecoder:%x\n",gTmPipeObject.hPipeObj[uIndex]->pAudioDecoder);
			DO(gTmPipeObject.hPipeObj[uIndex]->delete_device(gTmPipeObject.hPipeObj[uIndex], gTmPipeObject.hPipeObj[uIndex]->pAudioDecoder));
		}
	}
	
	for(uIndex =0; uIndex<MAX_AUDIO_OBJECTS; uIndex++)
	{
		if (gTmPipeObject.hAudioObj[uIndex])
		{
			printf("start to close %d audio objec:%x\n",uIndex,gTmPipeObject.hAudioObj[uIndex]);
			DO(PIPE_OBJ_DESTROY(audio,gTmPipeObject.hAudioObj[uIndex]));
		}
	}

	for(uIndex =0; uIndex<MAX_VIDEO_OBJECTS; uIndex++)
	{
		if (gTmPipeObject.hVideoObj[uIndex])
		{
			printf("start to close %d video objec:%x\n",uIndex,gTmPipeObject.hVideoObj[uIndex]);
			DO(PIPE_OBJ_DESTROY(video,gTmPipeObject.hVideoObj[uIndex]));
		}
	}

#if 0
	for(uIndex =0; uIndex<MAX_NUM_TSI_DEMUX; uIndex++)
	{
		DO(gTmPipeObject.hPipeObj[uIndex]->delete_device(gTmPipeObject.hPipeObj[uIndex], (PIPE_OBJ_HANDLE)gTmPipeObject.hTunerObj[0]));
	}
#endif
	for(uIndex =0; uIndex<MAX_TUNER_OBJECTS; uIndex++)
	{
		if (gTmPipeObject.hTunerObj[uIndex])
		{
			DO(PIPE_OBJ_DESTROY(tuner,gTmPipeObject.hTunerObj[uIndex]));
		}
	}

	for(uIndex =0; uIndex<MAX_PIPE_OBJECTS; uIndex++)
	{
		//printf("start close %d demux\n",uIndex);
		//DO(gTmPipeObject.hPipeObj[uIndex]->delete_device(gTmPipeObject.hPipeObj[uIndex], (PIPE_OBJ_HANDLE)gTmPipeObject.hDemuxObj[uIndex]));
		if (gTmPipeObject.hDemuxObj[uIndex])
		{
			printf("start to close %d demux objec:%x\n",uIndex,gTmPipeObject.hDemuxObj[uIndex]);
			DO(PIPE_OBJ_DESTROY(demux,gTmPipeObject.hDemuxObj[uIndex]));
		}
	}

	 for (uIndex = 0; uIndex < MAX_PIPE_OBJECTS ;uIndex++)
 	{
 		printf("start to close %d pipeline objec:%x\n",uIndex,gTmPipeObject.hPipeObj[uIndex]);
 		DO(PIPE_OBJ_DESTROY(pipeline,gTmPipeObject.hPipeObj[uIndex]));
 	}

	 bRet = TRUE;

	 return bRet;
}

BOOL CSUDIDriverTerm()
{
	CNXT_STATUS Result;
	int i = 0;

	printf("start %s\n",__FUNCTION__);

	//Result = cnxt_buttons_term();

	DO(cnxt_iic_term());	

	/*如果遥控器放在单独进程不需要term*/
	//Result = cnxt_irrx_term();
	
	DO(cnxt_smc_term());

	DO(cnxt_pvr_dm_term());
	/*dm里面term是通过发信号实现所以sleep等待接收信号的线程完成
	term的操作*/
	CSUDIOSThreadSleep(100);

	DO(cnxt_graphics_close(gTridGraphicsHandle));
	
	DO(cnxt_graphics_term());
	
	DO(cnxt_image_term());

	DO(pipe_pipeline_term());

	DO(cnxt_cbuf_term());	

	DO(cnxt_pvr_drv_term());
	
	DO(cnxt_scs_term_device());

	DO(cnxt_css_drv_term());
	
	DO(cnxt_vss_drv_term());

	DO(cnxt_plat_term_device());

	DO(cnxt_kal_terminate());

	printf("exit %s ret:%x\n",__FUNCTION__,Result); 

	return TRUE;
}

/*lichanghua :用于进程退出自动测试*/
#ifdef PROCESS_EXIT_TEST
extern void GrAllowPaint(BOOL bAllowPaint);
#endif

CSUDI_Error_Code CSUDIPlatformTerm()
{
	CSUDI_Error_Code enRet = CSUDI_FAILURE;

	/*lichanghua :用于进程退出自动测试*/
	/*stop GUI */
#ifdef PROCESS_EXIT_TEST
	GrAllowPaint(FALSE);
#endif

	CSUDIINJECTERTerm();
	CSUDIPLAYERTerm();
	CSUDISECTIONTerm();
	//CSUDISMCTerm();
	//CSUDIOSGTermAll();

	CSUDIVIDEOTerm();
	CSUDIAUDIOTerm();

	//CSUDIIRTerm();
	
	DestroyPipeObjects();
	
	CSUDIDriverTerm();

	return enRet;
}

static void signal_handler(unsigned int theSignal)
{
	CSUDIPlatformTerm();
	
	printf("end CSUDIPlatformTerm\n");
	
	exit(0);
}

/*lichanghua :用于进程退出自动测试*/
#ifdef PROCESS_EXIT_TEST
#include "udi2_input.h"
extern void UDI_RemoteControlUserProc(CSUDIInputInfo_S *psInputInfo ,void * pvUserData);

static void CSUDIOSThreadEntry_Exit(void * pvParam)
{
	CSUDIInputInfo_S stInput;
	
	CSUDIOSThreadSleep(25*1000);

	memset(&stInput,0,sizeof(CSUDIInputInfo_S));
	stInput.m_eType = EM_UDIINPUT_REMOTECTRL;
	stInput.m_eStatus = EM_UDIINPUT_KEY_PRESS;
	stInput.m_nKeyCode = 0x1B;/*CSUDI_VK_EXIT*/
	UDI_RemoteControlUserProc(&stInput,NULL);
	CSUDIOSThreadSleep(3*1000);
	stInput.m_nKeyCode = 0X01D4;/*CSUDI_VK_MENU*/
	UDI_RemoteControlUserProc(&stInput,NULL);
	CSUDIOSThreadSleep(8*1000);
	CSUDIPlatformTerm();
	exit(0);
}
#endif
CSUDI_Error_Code CSUDISYSInit(void)
{
    CSUDI_Error_Code retCode=CSUDI_FAILURE;
    CNXT_STATUS Result;
	CSUDI_HANDLE hThread;

    CSDEBUG(MODULE_NAME, 10, "module version:%s,%s,%s\n",CSSYS_PLATFORM_VERSION,__DATE__,__TIME__);
    CSDEBUG(MODULE_NAME, 10, "porting version:%s\n",PortingLibraryVersion);

	/*lichanghua :用于进程退出自动测试*/
#ifdef PROCESS_EXIT_TEST
	CSUDIOSThreadCreate("demon", 17*12, 1024*16,CSUDIOSThreadEntry_Exit,NULL, &hThread);
#endif


    /*lichanghua:将下面几个初始化操作是IR独立到一个进程所需要的
    最小初始化集,所以将他们放这个函数里面,这样在IR的进程中
    只需要调用CSUDISYSInit就可以了,避免增加接口*/
	do{
		Result = cnxt_kal_initialize();
		if (CNXT_STATUS_OK != Result)
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"ERROR: Could not initialize cnxt_kal_initialize err = %d Exiting...\n", Result);
			break;
		}

		Result = cnxt_plat_init_device();
		if (CNXT_STATUS_OK != Result)
		{
		    CSDEBUG(MODULE_NAME,ERROR_LEVEL,"ERROR: Could not initialize platform err = %d Exiting...\n", Result);
		    break;
		}

		Result = cnxt_scs_init_device();
		if(CNXT_STATUS_OK != Result)
		{
		    CSDEBUG(MODULE_NAME,ERROR_LEVEL,"ERROR: Could not initialize scs driver err = %d Exiting...\n", Result);
		    cnxt_plat_term_device ();
		    break;
		}

		retCode=CSUDI_SUCCESS;
	}while(0);
	
	signal(SIGINT,  signal_handler);

    return retCode;
}


CSUDI_Error_Code CSUDIPlatformInit(void)
{
    CSUDI_Error_Code retCode=CSUDI_SUCCESS;
    CNXT_STATUS Result;

    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"!!! CSUDIPlatformInit Begin\n");
	
    Result = hardware_init();

#ifdef DEBUG
    
    CNXT_TRACE_FLAGS        TraceFlags;
    u_int8                  uLoop;
    /* Set the inital trace flags. These are reset with values from EEPROM when
    the main instance starts up. */
    for(uLoop = 0; uLoop < CNXT_TRACE_FLAGS_WORDS; uLoop++)
    {
        TraceFlags.Flags[uLoop] = 0;
    }
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_APPLICATION,  FALSE,  &TraceFlags);
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_SVCLIST_DVB,  FALSE, &TraceFlags);
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_DVBLIB,       FALSE, &TraceFlags);
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_DEMUX,        FALSE, &TraceFlags);
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_CI,           FALSE, &TraceFlags);
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_DEMOD,        FALSE,  &TraceFlags);
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_TVENC,        FALSE, &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_VBI  ,        FALSE, &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_RF_MOD,       FALSE, &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_AUDIO,        FALSE, &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_VIDEO,        FALSE,  &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_DEMUX_CLKREC, FALSE, &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_PCM,          FALSE, &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_MIPIDLE,      FALSE, &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_VPP,  FALSE,  &TraceFlags );
    cnxt_kal_trace_set_module_flag(CNXT_TRACE_ISA,          TRUE,  &TraceFlags); 
    cnxt_kal_trace_set_level(CNXT_TRACE_LEVEL_1, TRUE, &TraceFlags);
#endif /* DEBUG */
    if( Result == CNXT_STATUS_OK )
    {
    	/*destroy frame buffer */
    	//CSOSGTermFB_P();
		
        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"!!! hardware init done........ \n");
        if(CNXT_STATUS_OK == create_objects())
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Objects are created......\n");
            retCode = CSUDI_SUCCESS;
        }
        else
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Failed to Create objects.....\n");
            retCode = CSUDI_FAILURE;
        }

		
        if(CNXT_STATUS_OK == CS_TRID_Video_init())
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"success to CS_TRID_Video_init().....\n");
            retCode = CSUDI_SUCCESS;
        }
        else
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Failed to CS_TRID_Video_init().....\n");
            retCode = CSUDI_FAILURE;
        }
        if(CNXT_STATUS_OK == CS_TRID_Audio_init())
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"success to CS_TRID_Audio_init().....\n");
            retCode = CSUDI_SUCCESS;
        }
        else
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Failed to CS_TRID_Audio_init().....\n");
            retCode = CSUDI_FAILURE;
        }
	#if 0
        if(CNXT_STATUS_OK == CS_TRID_OSG_init(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888))
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"success to CS_TRID_OSG_init().....\n");
            retCode = CSUDI_SUCCESS;
        }
        else
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Failed to CS_TRID_OSG_init().....\n");
            retCode = CSUDI_FAILURE;
        }
	#endif		
        if(CNXT_STATUS_OK == CS_TM_Player_Init())
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL," SUCCESS CS_TM_Player_Init......\n");
            retCode = CSUDI_SUCCESS;
        }
        if(CNXT_STATUS_OK == cs_tm_inject_init())
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"SUCCESS cs_tm_inject_init......\n");
            retCode = CSUDI_SUCCESS;
        }
#if 0
        if(CNXT_STATUS_OK == CS_TM_Screen_Init())
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success to CS_TM_Screen_Init().....\n");
            retCode = CSUDI_SUCCESS;
        }
		if(CSUDI_SUCCESS == CSUDISMCInit())
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success to CS_TM_Screen_Init().....\n");
			retCode = CSUDI_SUCCESS;
		}
        if (CNXT_STATUS_OK == cs_cnxt_utidriver_init())
        {
               CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success to UTIDriverInit().....\n");
               retCode = CSUDI_SUCCESS;
        }
#endif		
    }
    else
    {
        CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Failed to Init Hardware.....\n");
        retCode = CSUDI_FAILURE;
    }
    CSDEBUG(MODULE_NAME,ERROR_LEVEL,"!!! CSUDIPlatformInit End\n");

    return retCode;
}

CSUDI_Error_Code CSUDIGetChipID(CSUDICHIPID_S * psChipID)
{
    CSUDI_Error_Code retCode=CSUDI_SUCCESS;

    return retCode;
}

CSUDI_Error_Code CSUDIGetSerialKey(CSUDISerialKey_S * psSerialKey)
{
    CSUDI_Error_Code retCode=CSUDI_SUCCESS;

    return retCode;
}

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
CSUDI_Error_Code CSUDISYSReboot(void)
{
    
#define RESET_RST_SOFTRESET_REG_REG1            (0xE06E6008)
#define RESET_RST_LOCKCMD_REG_REG1            (0xE06E6028)
#define RESET_RST_LOCKSTAT_REG_REG1             (0xE06E602c)

#define NXP_SOFTRESET_REG	 (RESET_RST_SOFTRESET_REG_REG1)

#define NXP_PAGE	(1024*4)

	CSUDI_Error_Code retCode=CSUDI_FAILURE;
	unsigned int unLogicalAddr = 0;
	unsigned int phystart = NXP_SOFTRESET_REG&(~(NXP_PAGE-1));
	unsigned int UnOff = 0;
	int nMapLen=NXP_PAGE;
	int nMapFd = -1;

	/*lichanghua:this function write one rigster to reboot , it is reference to hwlib_reboot_IRD*/

	CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"enter %s\n",__FUNCTION__);

	do{
		nMapFd = open("/dev/mem", O_RDWR|O_SYNC);
		if(-1 == nMapFd)
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"open /dev/mem fail\n");
			break;
		}
		//printf("CSUDISYSReboot 11\n");
		
		/****映射系统控制寄存器****/
		unLogicalAddr = (unsigned int)mmap((void *)0, nMapLen, PROT_READ|PROT_WRITE, MAP_SHARED, nMapFd, phystart);
		if ((unsigned int)MAP_FAILED == unLogicalAddr)
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"mmap fail\n");
			break;
		}
		//printf("CSUDISYSReboot 22\n");
		UnOff =RESET_RST_LOCKCMD_REG_REG1-phystart;
		*(unsigned int*)(unLogicalAddr+UnOff)=0xf8;
		/*note:here need delay sometime ,otherwise fail to reboot*/
		CSUDIOSThreadSleep(10);

		UnOff =RESET_RST_LOCKCMD_REG_REG1-phystart;
		*(unsigned int*)(unLogicalAddr+UnOff)=0x2b;

		UnOff =RESET_RST_LOCKSTAT_REG_REG1-phystart;
		*(unsigned int*)(unLogicalAddr+UnOff)=0x2;

		UnOff =RESET_RST_SOFTRESET_REG_REG1-phystart;
		*(unsigned int*)(unLogicalAddr+UnOff)=0x1;
		
		/*here , it shoud be reboot , so following code is un-used actually*/
		if(-1==munmap((void*)unLogicalAddr,nMapLen))
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"munmap fail\n");
			break;
		}

		//printf("CSUDISYSReboot 33\n");
	}while(0);

	if (nMapFd >= 0)
	{
		close(nMapFd);
	}

    return retCode;
}



CSUDI_Error_Code CSUDISYSHalt(void)
{
    CSUDI_Error_Code retCode=CSUDI_SUCCESS;

    return retCode;
}
// 2011-01-17 TerenceZhang begin: Add for real standby feature.

static void _csudisys_standby_cleanup(u_int32 uMask)
{
   if(uMask & COSHIP_SCD_OPEN_MASK)
   {
        cnxt_scd_close(hgScd);
        hgScd = NULL;
   }

   if(uMask & COSHIP_SCD_INIT_MASK)
   {
        cnxt_scd_term();
   }
   if(uMask & COSHIP_IRRX_INIT_MASK)
   {
        cnxt_irrx_term();
   }
   if(uMask & COSHIP_BUTTONS_INIT_MASK)
   {
        cnxt_buttons_term();
   }
   if(uMask & COSHIP_GPIOEXP_INIT_MASK)
   {
       cnxt_gpio_expander_term();
   }
   if(uMask & COSHIP_IIC_INIT_MASK)
   {
      cnxt_iic_term();
   }
}
CSUDI_Error_Code CSUDISYSStandby(CSUDI_BOOL bStandby,int nStandbyTime,CSUDI_BOOL *pbReal)
{
    int duration;
    int uInitMask = 0;
    CSUDI_Error_Code retCode=CSUDI_SUCCESS;   
    CNXT_STATUS RetCode = CNXT_STATUS_OK;
    
    CNXT_SCD_CAPS ScdCaps;
    CNXT_SC_WAKEUP_EVENT_IR_CONFIG    IrCfg;
    CNXT_SC_WAKEUP_EVENT_FPB_CONFIG   FpbCfg;  
    CNXT_SC_WAKEUP_EVENT_RTC_CONFIG   RtcCfg;
    CNXT_SC_STDBY_MODE_REQUEST_CONFIG ReqConfig;

    printf("\n###################bStandby = %d ,Standbytime =%d(s)#####################\n",bStandby,nStandbyTime);

/*<!-- Add by 903126,2011-2-14 12:08:23: 防止没有进入standby模式时，standby的ir就接收了唤醒按键，导致已进入待机立即重启*/
    CSUDIOSThreadSleep(1000);
 /*End by --!>*/
        
    if(!s_fnChipStandbyHookFunc)//app not register hook function
    {
        if(bStandby == TRUE)
        {
            RetCode = cnxt_scd_init();
            if( RetCode != CNXT_STATUS_OK && RetCode != CNXT_STATUS_ALREADY_INIT)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                return CSUDI_FAILURE;            
            }
            if(RetCode == CNXT_STATUS_OK)
            {
                uInitMask |= COSHIP_SCD_INIT_MASK;
            }
            
            ScdCaps.DriverId = CNXT_SCD_DRIVER_ID_TEST;
            ScdCaps.UnitType = CNXT_SCD_UNIT_TYPE_SYSTEM;
            RetCode = cnxt_scd_open(&hgScd, 
                                    &ScdCaps, 
                                    NULL, 
                                    (void *)0);
            if( RetCode != CNXT_STATUS_OK )
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d RetCode:%d\n", __FUNCTION__, __LINE__,RetCode);
                _csudisys_standby_cleanup(uInitMask);
                return CSUDI_FAILURE;            
            }
            else
            {
              uInitMask |= COSHIP_SCD_OPEN_MASK;
            }

            ReqConfig.uWakeupEnable = 0;


#ifdef UDI2_WAKEUP_BY_IR

            RetCode = cnxt_irrx_init(NULL);
            if(RetCode != CNXT_STATUS_OK && RetCode != CNXT_STATUS_ALREADY_INIT)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                _csudisys_standby_cleanup(uInitMask);
                return CSUDI_FAILURE;            
            }

            if(RetCode == CNXT_STATUS_OK)
            {
              uInitMask |= COSHIP_IRRX_INIT_MASK;
            }
            
            if(s_sSYSWakeupParams.m_bModeEnable &&
                (s_sSYSWakeupParams.m_uWakeupParams.m_stIRKeys.m_nKeyNum > 0))
            {
                int nKeyNum = s_sSYSWakeupParams.m_uWakeupParams.m_stIRKeys.m_nKeyNum;
                
                IrCfg.bAllIrCmds = FALSE;
                
                /*底层实现方式请参考tmscsScf_Prv.c文件中的internal_scf_irrx_key_check 
                               * 函数来实现多个按键的判断
                              */
                if(nKeyNum == 1)
                {
                          
                    IrCfg.uIrCmd0Base = s_sSYSWakeupParams.m_uWakeupParams.m_stIRKeys.m_astKeys[0].m_nKeyCode;       
                    IrCfg.uIrCmd0 = 0x1; 
                }
                else 
                {   
                    IrCfg.uIrCmd0Base = s_sSYSWakeupParams.m_uWakeupParams.m_stIRKeys.m_astKeys[0].m_nKeyCode;       
                    IrCfg.uIrCmd0 = 0x1; 
                        
                    IrCfg.uIrCmd1Base = s_sSYSWakeupParams.m_uWakeupParams.m_stIRKeys.m_astKeys[1].m_nKeyCode;       
                    IrCfg.uIrCmd1 = 0x1; 

                }
            }
            else 
            {
                IrCfg.bAllIrCmds = TRUE;//any ir keycode can wakeup standby
            }
            
            RetCode = cnxt_scd_configure_ir_wakeup_source(hgScd, &IrCfg);
            if( RetCode != CNXT_STATUS_OK )
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                _csudisys_standby_cleanup(uInitMask);
                return CSUDI_FAILURE;            
            }
            ReqConfig.uWakeupEnable |= CNXT_SC_WAKEUP_SOURCE_IR;       
#endif

#ifdef UDI2_WAKEUP_BY_FRONT_PANEL
            RetCode = cnxt_iic_init(NULL);
            if(RetCode != CNXT_STATUS_OK && RetCode != CNXT_STATUS_ALREADY_INIT)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                _csudisys_standby_cleanup(uInitMask);
                return CSUDI_FAILURE;            
            }

            if(RetCode == CNXT_STATUS_OK)
              uInitMask |= COSHIP_IIC_INIT_MASK;

            RetCode = cnxt_gpio_expander_init();
            if(RetCode != CNXT_STATUS_OK && RetCode != CNXT_STATUS_ALREADY_INIT)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                _csudisys_standby_cleanup(uInitMask);
                return CSUDI_FAILURE;            
            }

            if(RetCode == CNXT_STATUS_OK)
              uInitMask |= COSHIP_GPIOEXP_INIT_MASK;

            RetCode = cnxt_buttons_init(NULL);
            if(RetCode != CNXT_STATUS_OK && RetCode != CNXT_STATUS_ALREADY_INIT)
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                _csudisys_standby_cleanup(uInitMask);
                return CSUDI_FAILURE;            
            }

            if(RetCode == CNXT_STATUS_OK)
              uInitMask |= COSHIP_BUTTONS_INIT_MASK;

            FpbCfg.uFpbCmd = CNXT_SC_WAKEUP_BTN_MASK_POWER; 
            RetCode = cnxt_scd_configure_fpb_wakeup_source(hgScd, &FpbCfg);
            if( RetCode != CNXT_STATUS_OK )
            {
                CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                _csudisys_standby_cleanup(uInitMask);
                return CSUDI_FAILURE;            
            }

            ReqConfig.uWakeupEnable |= CNXT_SC_WAKEUP_SOURCE_FPB;   
#endif

            if(nStandbyTime != 0)
            {            
                duration = nStandbyTime;
                
                RtcCfg.uSecs = duration % 60;
                duration /= 60;
                RtcCfg.uMins = duration % 60;
                duration /= 60;
                RtcCfg.uHours = duration % 24;
                duration /= 24;
                RtcCfg.uDays = duration % 31;
                duration /= 31;
                RtcCfg.uMonths = duration % 25;

                RetCode = cnxt_scd_configure_rtc_wakeup_source(hgScd, &RtcCfg);
                if( RetCode != CNXT_STATUS_OK )
                {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d \n", __FUNCTION__, __LINE__);
                    _csudisys_standby_cleanup(uInitMask);
                    return CSUDI_FAILURE;            
                }
                ReqConfig.uWakeupEnable |= CNXT_SC_WAKEUP_SOURCE_RTC;
            }

            ReqConfig.State = CNXT_SC_STATE_PASSIVE_STDBY;

            RetCode = cnxt_scd_change_pm_state(hgScd, &ReqConfig);
            if( RetCode != CNXT_STATUS_OK )
            {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"\n ERR: %s %d RetCode:%d\n", __FUNCTION__, __LINE__,RetCode);
                    _csudisys_standby_cleanup(uInitMask);
                    return CSUDI_FAILURE;            
            }

            *pbReal = TRUE;

        }
        else
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"\nWe used real standby mode and do not support wakeup by cpu!\n");
            retCode = CSUDI_FAILURE;
        }
    }
    else
    {
        s_fnChipStandbyHookFunc(bStandby,nStandbyTime,pbReal);
    }
    return retCode;
}
// 2011-01-17 TerenceZhang end

CSUDI_Error_Code CSUDISYSSetWakeupParams(const CSUDISYSWakeupParams_S* pstWakeupParams)
{
    if(pstWakeupParams)
    {
        cnxt_kal_memset(&s_sSYSWakeupParams,0,sizeof(CSUDISYSWakeupParams_S));
        cnxt_kal_memcpy(&s_sSYSWakeupParams,pstWakeupParams,sizeof(CSUDISYSWakeupParams_S));
        return CSUDI_SUCCESS;
    }
    return CSUDI_FAILURE;
}

CSUDI_Error_Code CSUDISYSSetChipStandbyHook(CSUDIChipStandbyHookFunc_F fnHook)
{
    if(fnHook)
    {
        s_fnChipStandbyHookFunc = fnHook;
        return CSUDI_SUCCESS;    
    }
    return CSUDI_FAILURE;
}

