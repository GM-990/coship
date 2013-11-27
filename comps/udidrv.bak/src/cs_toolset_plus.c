/****************************************************************************/
/*
 * Filename:        cs_toolset_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------

 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_toolset_plus.h"


#include "generic_include.h"
#include "tm_inject.h"
#include "tm_player.h"
#include "tm_screen.h"
#include "tm_video.h"
#include "record.h"

#define CSSYS_PLATFORM_VERSION   ("Trident.1.0")
#define PortingLibraryVersion    ("PORTING-Trident-Shiner")
#define MODULE_NAME              ("CS_TOOLSET")

extern u_int8 *pRecordData;
CNXT_CBUF_HANDLE gCbuf;

CS_TM_PIPE_OBJECTS gTmPipeObject;
CS_TM_VP_DEVICE_OBJECTS gTmVpDeviceObj;
CNXT_GRAPHICS_HANDLE   gTridGraphicsHandle;
CS_TM_AP_DEVICE_OBJECTS gTmApDeviceObj;

extern void cs_tm_playback_notify( PIPE_PIPELINE_OBJECT *pPipeline,
                                    void                     *pUserData,
                                    PIPE_NOTIFY_EVENT         Event,
                                    void                     *pData,
                                    void                     *pTag );

void Coship_pipe_event_notifier( struct pipe_pipeline_obj *pPipe,
                                    void *pUserData,
                                    PIPE_NOTIFY_EVENT Event,
                                    void *pData,
                                    void *pTag );

static void signal_handler(unsigned int theSignal)
{
	CSUDIPlatformTerm();
	
	printf("end CSUDIPlatformTerm\n");
	
	exit(0);
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
         TunerCfg.Type  =  PIPE_CABLE_TUNER_TYPE;
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

//frank.zhou---------------------------------------------------------------------------------------------------
/**
@brief 移植层配置接口，调用时机由各eCfgType决定,具体见CSSYSCfg_E定义

该接口可被调用多次，同一类型的配置以最后一次的调用为准
@param[in] eCfgType 配置类型,具体见CSSYSCfg_E定义;
@param[in] nDeviceIndex 所要配置的设备index,如果该eCfgType无对应的设备时,传入-1;
@param[in] unValue1	配置参数1,具体含义由eCfgType决定;
@param[in] unValue2 配置参数2,具体含义由eCfgType决定;
@return 配置成功返回CSUDI_SUCCESS ，配置失败返回错误值。
*/
CSUDI_Error_Code CSUDISYSSetCfg(CSUDISYSCfg_E eCfgType, int nDeviceIndex, CSUDI_UINT32 unValue1, CSUDI_UINT32 unValue2)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 移植层系统初始化接口，实现系统的分区划分，并启动操作系统，系统启动时第一个被调用

@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误值。
*/
CSUDI_Error_Code CSUDISYSInit(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode=CSUDI_FAILURE;
	CNXT_STATUS Result;
	CSUDI_HANDLE hThread;

    CSDEBUG(MODULE_NAME, 10, "module version:%s,%s,%s\n",CSSYS_PLATFORM_VERSION,__DATE__,__TIME__);

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

		Retcode=CSUDI_SUCCESS;
	}while(0);
	
	signal(SIGINT,  signal_handler);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 移植层系统初始化接口

该接口只可被调用一次，必须在CSUDISYSInit后调用
@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误值。
@note 
- 初始化机顶盒主芯片相关设备驱动，如AV decoder，Demux等等，并初始化相应的移植层模块，
	如Player,Inject,Audio,Video,Screen,Demux,Section等，初始化之后调用.用户不需要关心各设备的初始化顺序。
	初始化的配置参数使用CSUDISYSSetCfg设置，如果没有设置将按默认值初始化。
*/
CSUDI_Error_Code CSUDIPlatformInit(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	
	CSUDI_Error_Code retCode=CSUDI_SUCCESS;
		CNXT_STATUS Result;
	
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"!!! CSUDIPlatformInit Begin\n");
		
		Result = hardware_init();
	
#ifdef DEBUG
		
		CNXT_TRACE_FLAGS		TraceFlags;
		u_int8					uLoop;
		/* Set the inital trace flags. These are reset with values from EEPROM when
		the main instance starts up. */
		for(uLoop = 0; uLoop < CNXT_TRACE_FLAGS_WORDS; uLoop++)
		{
			TraceFlags.Flags[uLoop] = 0;
		}
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_APPLICATION,	FALSE,	&TraceFlags);
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_SVCLIST_DVB,	FALSE, &TraceFlags);
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_DVBLIB,		FALSE, &TraceFlags);
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_DEMUX,		FALSE, &TraceFlags);
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_CI,			FALSE, &TraceFlags);
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_DEMOD,		FALSE,	&TraceFlags);
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_TVENC,		FALSE, &TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_VBI  ,		FALSE, &TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_RF_MOD,		FALSE, &TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_AUDIO,		FALSE, &TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_VIDEO,		FALSE,	&TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_DEMUX_CLKREC, FALSE, &TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_PCM,			FALSE, &TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_MIPIDLE,		FALSE, &TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_VPP,	FALSE,	&TraceFlags );
		cnxt_kal_trace_set_module_flag(CNXT_TRACE_ISA,			TRUE,  &TraceFlags); 
		cnxt_kal_trace_set_level(CNXT_TRACE_LEVEL_1, TRUE, &TraceFlags);
#endif /* DEBUG */
		if( Result == CNXT_STATUS_OK )
		{
			/*destroy frame buffer */
			CSOSGTermFB_P(); //commented by frank.zhou
			
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
			
			if(CNXT_STATUS_OK == CS_TM_Screen_Init())
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success to CS_TM_Screen_Init().....\n");
				retCode = CSUDI_SUCCESS;
			}
			/*
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
			*/
			//commented by frank.zhou
		}
		else
		{
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Failed to Init Hardware.....\n");
			retCode = CSUDI_FAILURE;
		}
		CSDEBUG(MODULE_NAME,ERROR_LEVEL,"!!! CSUDIPlatformInit End\n");
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 设置真待机唤醒参数

@param[in] pstStandbyParams 见CSUDISYSStandbyParams_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDISYSSetWakeupParams(const CSUDISYSWakeupParams_S* pstWakeupParams)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 设置芯片待机的钩子函数

@param[in] fnHook 钩子函数，见CSUDIChipStandbyHookFunc_F定义
@return 如果调用成功，返回调用CSUDI_SUCCESS；如果调用失败则返回错误代码值。
@note fnHook可以为NULL，表示取消钩子函数
*/
CSUDI_Error_Code CSUDISYSSetChipStandbyHook(CSUDIChipStandbyHookFunc_F fnHook)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
} 

/**
@brief 设置引导信息

根据引导类型设置额外的引导信息，用于在CSUDIToolsetExec时使用相同的目标代码完成不同的工作。
如loader和app可以使用相同Linux内核的两个拷贝，但根据本接口设置的引导参数加载不同的rootfs。
@param[in] eType 引导信息
@param[in] pExecParam 引导参数，具体意义由CSUDIToolsetExec自行决定，并告知初始化模块设置，调用者必须保证所指向内存一直有效。
@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误代码
@see CSUDIToolsetExec
*/
CSUDI_Error_Code CSUDIToolsetSetExecInfo(CSUDIToolsetExecType_E eType,void* pExecParam)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 设置目标机器的序列号和Key，与UDI接口CSUDIGetSerialKey对应

@param[in] psSerialKey 指向存放获取到的序列号和Key的结构体指针，序列号和Key均是以0结尾的字符串，请参见CSUDISerialKey_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 由于在coship内部，CSUDIGetSerialKey接口驱动部门实现，但是驱动并不知道如何获取序列号和Key，所以需要有一个set接口，供应用在初始化时调用
*/
//CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey);

