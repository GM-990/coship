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
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include <unistd.h> 
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>


#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_osg.h"

#include "generic_include.h"
#include "cnxt_types.h"
#include "image.h"
#include "image_types.h"
#include "presentation.h"
#include "graphics_gen.h"

#include "graphics_helper.h"
#include "drm.h"
#include "string.h"
#include "cbuf.h"
#include "cnxtfb.h"

#include "drm.h"
#include "string.h"
#include "cbuf.h"
#include "cnxtfb.h"
#include "tm_osg.h"


#define SURFACE_USE_UNCACHE

#define MODULE_NAME              ("CS_OSG")
#define SURFACE_USE_UNCACHE

#define PIPE_JPEG_CHUNK_SIZE     (64*1024)

static CNXT_IMAGE_HANDLE			g_hDecoderImage;

static StillPicInfo stJPGInfo;

static bool				 bOsdInitialised = 0;


extern CNXT_GRAPHICS_HANDLE   gTridGraphicsHandle;

PIPE_VP_SURFACE_OBJECT        *pgHDOSDSurface = NULL;
PIPE_VP_SURFACE_OBJECT        *pgSDOSDSurface = NULL;
u_int16 SD_width = 720;
u_int16 SD_height = 576;
u_int16 HD_width = RGN1_WIDTH;
u_int16 HD_height = RGN1_HEIGHT;
static u_int8 uDisplayPage[2] ={0,0};

TM_Image_Surface             gUdiImageArray[MAX_SURFACE_NUM];

CS_CNXT_OsgIframe_SubSystem gOsgIframeSubsys[MAX_OSG_IFRAME];

static CNXT_IMAGE_HANDLE          gImageHandle_hd;
static CNXT_IMAGE_HANDLE          gImageHandle_sd;
static bool   blit_done[2] = {FALSE,FALSE};
CNXT_SEM_ID gOsg_sem[2];
static unsigned int uStartTimeMs_start,uStartTimeMs_end;
static unsigned int s_cnt = 0;
static unsigned int s_TotalTime_full = 0;
static unsigned int s_TotalTime_small = 0;
static int osg_fd=-1;


void CS_CNXT_SD_DRM_Notify(void)
{

  
	bool 					   bSendEvent = FALSE;
	CNXT_STATUS		   Retcode;
	TM_Image_Surface   *pDstImageSurface, *pSrcImageSurface;

	CNXT_RECT		   srcImageRect;
	CNXT_GRAPHICS_OP    GraphicsOp={0};
	u_int8           iDisplayPage;

	if (!bOsdInitialised)
	{

		return ;
	}

//	printf("[SD]CS_CNXT_SD_DRM_Notify.[%d]\n", CSUDIPlusOSGetTickCount());

	if(blit_done[0] == TRUE)
	{

		Retcode = cnxt_kal_sem_get(gOsg_sem[0], CNXT_KAL_WAIT_FOREVER);
		if(Retcode != CNXT_STATUS_OK)
		{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
				return ;
		}
		//printf("VSYNC_SD \n");
		blit_done[0] = FALSE;

		/* Enable hardware connection to the DRM for the still video region */

		iDisplayPage = (uDisplayPage[0])%2;
		Retcode = pgSDOSDSurface->set_graphics_page(pgSDOSDSurface,TRUE, iDisplayPage); //modify by frank
		if(Retcode != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"cnxt_drm_rgn_set_options :%d at line :%d\n",Retcode,__LINE__);
			Retcode = cnxt_kal_sem_put(gOsg_sem[0]);
			return ;
		}
#if 0				 
		{
			CNXT_XY 			 DstImageXY;

			/* get destination X and Y */
			srcImageRect.nLeft	   = 0;
			srcImageRect.nTop	   = 0;
			srcImageRect.nRight    = RGN0_WIDTH;
			srcImageRect.nBottom   = RGN0_HEIGHT;
			DstImageXY.nX = 0;
			DstImageXY.nY = 0;
			pSrcImageSurface = &gUdiImageArray[0];
			pDstImageSurface = &gUdiImageArray[0];
			/* call copy blit */
			GraphicsOp.uBltControl = CNXT_GRAPHICS_OP_ROP;
			GraphicsOp.uAlphaUse = 0;
			GraphicsOp.uAlpha = 0;
			GraphicsOp.bAsyncSig = TRUE; //FALSE;
			GraphicsOp.bAsync = FALSE;
			
			Retcode = cnxt_graphics_copy_blit(gTridGraphicsHandle, 
												pSrcImageSurface->pImageHandle, 
												((uDisplayPage[0]+1)%2), 
												&srcImageRect, 
												NULL, 
												pDstImageSurface->pImageHandle, 
												(uDisplayPage[0]), 
												&DstImageXY, 
												&GraphicsOp);
			if(Retcode != CNXT_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error: performing cnxt_graphics_copy_blit Retcode=0x%x..\n", Retcode);
				//return;
			}
		}
		uDisplayPage[0] = (uDisplayPage[0]+1)%2;
#endif				
		Retcode = cnxt_kal_sem_put(gOsg_sem[0]);
		if(Retcode != CNXT_STATUS_OK)
		{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
				return ;
		}
	}


	if(blit_done[1] == TRUE)
	{
		Retcode = cnxt_kal_sem_get(gOsg_sem[1], CNXT_KAL_WAIT_FOREVER);
		if(Retcode != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
			return ;
		}
		//printf("VSYNC_HD \n");
		blit_done[1] = FALSE;
		
		/* Enable hardware connection to the DRM for the still video region */
			
		iDisplayPage = uDisplayPage[1]%2;

		Retcode = pgHDOSDSurface->set_graphics_page(pgHDOSDSurface, TRUE, iDisplayPage);

		if(Retcode != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"cnxt_drm_rgn_set_options :%d at line :%d\n",Retcode,__LINE__);
			Retcode = cnxt_kal_sem_put(gOsg_sem[1]);
			return ;
		}


		Retcode = cnxt_kal_sem_put(gOsg_sem[1]);
		if(Retcode != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
			return ;
		}
   }

	return;
}


CNXT_STATUS CS_TRID_OSG_init(CSUDIOSGPixelFormat_E ePixelFormat)
{
	PIPE_VP_SURFACE_CFG     SurfaceCfg;
	PIPE_VP_DEVICE_ATTRIB   hdAttr;
	PIPE_VP_DEVICE_ATTRIB   sdAttr;
	PIPE_VP_SURFACE_INPUT   Input;
	PIPE_VP_SURFACE_POSITION postion; 
	int i;
	CNXT_PIXEL_MODE         pixel_mode_tmp;
	CNXT_STATUS        Retcode = CSUDI_SUCCESS;
	char sem_name[20]; 
	u_int32 EventMask;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "function %s in\n",__FUNCTION__);
	int chain = 0;

	do
	{
#if 0
		osg_fd = open("/dev/fb0", O_RDWR);
		if(osg_fd < 0)
		{
		    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "function %s fail to open fb0 \n\r",__FUNCTION__);
		    Retcode = CNXT_STATUS_FAILURE;
		}
		else
		{
			if (ioctl(osg_fd, FBIO_STOPDISPLAY, NULL) < 0) 
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL, " %s: fail to stop display for fb\n", __FUNCTION__);
		        	Retcode = CNXT_STATUS_FAILURE;
			}		
			if (ioctl(osg_fd, FBIO_CLOSESURFACE, NULL) < 0) 
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL, " %s: fail to close surface for fb\n", __FUNCTION__);
		       	Retcode = CNXT_STATUS_FAILURE;
			}
		}
#endif
		if(gTmVpDeviceObj.pHDDevice == NULL ||(gTmVpDeviceObj.pSDDevice == NULL))
		{
			
			CSDEBUG(MODULE_NAME,ERROR_LEVEL, "VP HD Device or VP SD Device is NOT init\n" );
			Retcode = CNXT_STATUS_FAILURE;	        
			break;			
		}

		sprintf(sem_name,"OSGSem0");
		Retcode=cnxt_kal_sem_create(1, sem_name, &(gOsg_sem[0]));
		if(CNXT_STATUS_OK!=Retcode)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "sem_create  failed\n");
			Retcode = CNXT_STATUS_FAILURE; 
			break;
		}
		sprintf(sem_name,"OSGSem1");
		Retcode=cnxt_kal_sem_create(1, sem_name, &(gOsg_sem[1]));
		if(CNXT_STATUS_OK!=Retcode)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "sem_create  failed\n");
			Retcode = CNXT_STATUS_FAILURE; 
			break;
		}
		/* get HD size */
		if(gTmVpDeviceObj.pHDDevice->get_attrib(gTmVpDeviceObj.pHDDevice, &hdAttr)!= PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error\n",__FUNCTION__);
			Retcode = CNXT_STATUS_FAILURE; 
			break;
		}
		internal_display_mode(&hdAttr);
		/* get SD size */
		if(gTmVpDeviceObj.pSDDevice->get_attrib(gTmVpDeviceObj.pSDDevice, &sdAttr)!= PIPE_STATUS_OK)
		
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error\n",__FUNCTION__);
			Retcode = CNXT_STATUS_FAILURE; 
			break;
		}
		
		internal_display_mode(&sdAttr);
		/* Create a presentation surface on the HD&SD device. */
		if(gTmVpDeviceObj.pHDDevice->surface_open( gTmVpDeviceObj.pHDDevice, PIPE_VP_OSD_SURFACE ) != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE; 
			break;
		}
		if(gTmVpDeviceObj.pHDDevice != NULL)
		{
			pgHDOSDSurface  = gTmVpDeviceObj.pHDDevice->Surface[PIPE_VP_OSD_SURFACE];
		}
		if(pgHDOSDSurface->config(pgHDOSDSurface, &SurfaceCfg) != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break;
		}
		
		/* SD */
		
		if ( gTmVpDeviceObj.pSDDevice->surface_open( gTmVpDeviceObj.pSDDevice, PIPE_VP_OSD_SURFACE ) != PIPE_STATUS_OK )
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break; 
		}
		
		if(gTmVpDeviceObj.pSDDevice != NULL)
		{
			pgSDOSDSurface = gTmVpDeviceObj.pSDDevice->Surface[PIPE_VP_OSD_SURFACE];
		}
		
		if(pgSDOSDSurface->config(pgSDOSDSurface, &SurfaceCfg) != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break; 
		}
		
		pixel_mode_tmp = change_pixel_modes_udi2nxp(ePixelFormat);
		if( pixel_mode_tmp == PIXEL_NOT_SUPPORT )
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,	"PIXEL_NOT_SUPPORT.\n");
			Retcode = CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED;
			break;
		}	 

		internal_graphics_fill_image_caps_osg(pixel_mode_tmp, HD_width,HD_height);
		/* set input */
		Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_IMAGE;
		Input.hImageHandle = gImageHandle_hd;
		if( pgHDOSDSurface->set_input ( pgHDOSDSurface, &Input ) != PIPE_STATUS_OK )
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break; 
		}
		/* set position */
		postion.uFlags = PIPE_VP_SURFACE_SRC_FULL_IMAGE|PIPE_VP_SURFACE_DEST_FULL_SCREEN;
		if( pgHDOSDSurface->set_position(pgHDOSDSurface, &postion)!= PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break; 
		}
		/* Enable the HD Surfaces */
		if( pgHDOSDSurface->surface_enable(pgHDOSDSurface) != PIPE_STATUS_OK )
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break; 
		}
		
		/*Enable SD display buffer*/
	
		Input.hImageHandle = gImageHandle_sd;
		if( pgSDOSDSurface->set_input ( pgSDOSDSurface, &Input ) != PIPE_STATUS_OK )
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break; 
		}    
		/*set position*/
		postion.uFlags = PIPE_VP_SURFACE_SRC_FULL_IMAGE|PIPE_VP_SURFACE_DEST_FULL_SCREEN;			
		if( pgSDOSDSurface->set_position(pgSDOSDSurface, &postion)!= PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break; 
		}
		
		/* Enable the SD Surface */
		if( pgSDOSDSurface->surface_enable(pgSDOSDSurface) != PIPE_STATUS_OK )
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s error LINE=%d\n",__FUNCTION__,__LINE__);
			Retcode = CNXT_STATUS_FAILURE;
			break; 
		}

		EventMask = (1 << (PIPE_VP_EVENT_VSYNC & 0xFFFFFF));
	
		pgSDOSDSurface->subscribe_event(pgSDOSDSurface, EventMask);			
//		pgHDOSDSurface->subscribe_event(pgHDOSDSurface, EventMask);

		pgSDOSDSurface->set_notifier(pgSDOSDSurface,CS_CNXT_SD_DRM_Notify,NULL);
//		pgHDOSDSurface->set_notifier(pgSDOSDSurface,CS_CNXT_HD_DRM_Notify,NULL);
		
		pgSDOSDSurface->set_graphics_page(pgSDOSDSurface, TRUE, 0);
		pgHDOSDSurface->set_graphics_page(pgHDOSDSurface, TRUE, 0);

		for(i = 0; i< MAX_SURFACE_NUM;i++)
		{
			gUdiImageArray[i].used = FALSE;
			gUdiImageArray[i].pixelFormat = 0xff;
			gUdiImageArray[i].height = 0;
			gUdiImageArray[i].width = 0;
			// gUdiImageArray[i].pSurfaceHandle=NULL;
			gUdiImageArray[i].index = i;
			gUdiImageArray[i].alphaValueFlag = 0;
			gUdiImageArray[i].alphaValue = 0xff;
			gUdiImageArray[i].colorKey = 0;
			gUdiImageArray[i].colorKeyFlag = 0;
			
		}

		gUdiImageArray[1].used = TRUE;
		gUdiImageArray[1].pixelFormat = (u_int8)ePixelFormat;
		gUdiImageArray[1].height = HD_height;
		gUdiImageArray[1].width = HD_width;
		gUdiImageArray[1].pImageHandle=gImageHandle_hd;

		gUdiImageArray[0].used = TRUE;
		gUdiImageArray[0].pixelFormat = (u_int8)ePixelFormat;
		gUdiImageArray[0].height = SD_height;
		gUdiImageArray[0].width = SD_width;
		gUdiImageArray[0].pImageHandle=gImageHandle_sd;
		cnxt_kal_memset(&gOsgIframeSubsys[0],0,sizeof(CS_CNXT_OsgIframe_SubSystem));	
		gOsgIframeSubsys[chain].bIframeEnable = FALSE;
		gOsgIframeSubsys[chain].bJpegEnable = FALSE;
		
	}while(0);	

	osd_sd_output_flick_patch();

	
	bOsdInitialised = 1;
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "function %s initial ok \n\r",__FUNCTION__);
	return Retcode;
}

void CSOSGTermFB_P()
{
	osg_fd = open("/dev/fb0", O_RDWR);
	
	if(osg_fd < 0)
	{
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "function %s fail to open fb0 \n\r",__FUNCTION__);
	    return ;
	}
	else
	{
		if (ioctl(osg_fd, FBIO_STOPDISPLAY, NULL) < 0) 
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, " %s: fail to stop display for fb\n", __FUNCTION__);
	        	return ;
		}		
		#if 0	 //commented by frank.zhou
		if (ioctl(osg_fd, FBIO_CLOSESURFACE, NULL) < 0) 
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, " %s: fail to close surface for fb\n", __FUNCTION__);
	       	return ;
		}
		#endif
	}

	return ;
}

//frank.zhou---------------------------------------------------------------------------------------------------
/**
@brief 创建一个绘图平面

@param[in] ePixelFormat 绘图平面的颜色模式。
@param[in] nWidth  绘图平面的宽度，必须大于 0。
@param[in] nHeight 绘图平面的高度，必须大于 0。
@param[out] phSurface 成功返回绘图平面的句柄，失败为空，详细错误参见错误代码值。
@return 成功返回 CSUDI_SUCCESS,否则返回错误代码值。 
@note 

- 该接口创建显示缓存空间，理论上可以支持无限大，支持创建无限多个直到内存不足，(建议提供16M的内存用于创建Surface)，具体需要几个Surface根据项目需求的不同而不同，目前应用用到比较多的是:\n
     1、游戏应用中的可变图片会用到此Surface。 \n
     2、Java应用中用到BufferedImage用到此Surface。\n 
     3、2D特效的实现会用到此Surface)。 \n
- 此处颜色模式可以与最终显示的颜色模式不同，在Blit时做特殊处理即可，如果平台不支持这样的处理也可在应用选择颜色模式时加以避免。\n
- 如果平台资源实在有限，也可允许不能创建任何Surface，这种情况下会直接操作显存，但必须要求界面与显存的分辨率，颜色模式等保持一致。
- 如果硬件不支持再创建Surface时，建议返回失败。这时中间件会模拟创建surface并进行操作。
*/
CSUDI_Error_Code CSUDIOSGCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight, CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	CNXT_IMAGE_CAPS     ImageCaps;
	CNXT_IMAGE_HANDLE	ImageHandle = 0;
	CNXT_PIXEL_MODE       pixel_mode;
	int i;
	CNXT_STATUS     Status = CNXT_STATUS_OK;
	//CSUDI_Error_Code status = CSUDI_SUCCESS;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "Enter function %s in nWidth=%d nHeight=%d\n",__FUNCTION__,nWidth,nHeight);
	if (!bOsdInitialised)
	{

		return CSUDIOSG_ERROR_UNKNOWN_ERROR;
	}
	
	if (phSurface == CSUDI_NULL)
	{
		CSDEBUG(MODULE_NAME,ERROR_LEVEL,  "phSurface == CSUDI_NULL\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER;
	}

	*phSurface = CSUDI_NULL;
	
	if (ePixelFormat >= EM_UDIOSG_PIXEL_FORMAT_NUM)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "EM_UDIOSG_PIXEL_FORMAT_NUM\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER;
		
	}


	 if(/*(nWidth > RGN1_WIDTH)||(nHeight > RGN1_HEIGHT)||*/(nHeight <= 0)||(nWidth <= 0))
	 {
	    CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error width:%d weight:%d.\n",nWidth,nHeight);
	     return CSUDIOSG_ERROR_BAD_PARAMETER;
	 } 
	
	for(i=0; i<MAX_SURFACE_NUM; i++)
	{
		if(gUdiImageArray[i].used == FALSE)
		{
			break;
		}
	}
	
	if(i == MAX_SURFACE_NUM)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error there is no enough image to used?.\n");
		return CSUDIOSG_ERROR_UNKNOWN_ERROR;
	}
	
	do {
		pixel_mode = change_pixel_modes_udi2nxp(ePixelFormat);
		if( pixel_mode == PIXEL_NOT_SUPPORT )
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "PIXEL_NOT_SUPPORT.\n");
			Retcode= CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED;
			break;
		}               

		
		Status = CS_OSG_FillImageCaps(pixel_mode, (u_int16)nWidth, (u_int16)nHeight, &ImageCaps);
		if(Status != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Unable to fill in Destination image caps. Retcode = %d.\n", Retcode);
			Retcode =   CSUDIOSG_ERROR_UNKNOWN_ERROR;//????
			break;
		}
		Status = cnxt_image_open(&ImageHandle, &ImageCaps, NULL, NULL);
		
		if(Status != CNXT_STATUS_OK)
		{
			
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error open a destination image. IMAGEStatus = %d.\n", Retcode);
			Retcode =   CSUDIOSG_ERROR_UNKNOWN_ERROR;//????
			break;
		}
		
		gUdiImageArray[i].used = TRUE;
		gUdiImageArray[i].pixelFormat = (u_int8)ePixelFormat;
		gUdiImageArray[i].width = (u_int16)nWidth;
		gUdiImageArray[i].height = (u_int16)nHeight;
		gUdiImageArray[i].pImageHandle=ImageHandle;
		*phSurface = (CSUDI_HANDLE)&gUdiImageArray[i];

	}while(0);	
	return Retcode;
}


/**
@brief 销毁一个绘图平面，释放该绘图平面的内存。

@param[in] hSurface CSUDIOSGCreateSurface 成功返回的句柄。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 
@note 
- 由CSUDIOSGCreateSurface创建的缓存区域不使用后，一定要调用此接口销毁释放。\n
- 尝试销毁一个显存空间，将返回CSUDIOSG_ERROR_BAD_PARAMETER。
*/
CSUDI_Error_Code CSUDIOSGDestroySurface(CSUDI_HANDLE hSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	 
	CNXT_STATUS 		Status = CNXT_STATUS_OK;
	//CSUDI_Error_Code    status = CSUDI_SUCCESS;
	PIPE_VP_SURFACE_INPUT Input;
	TM_Image_Surface *SurfaceTmp;
	
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Enter Function :%s \n",__FUNCTION__); 
	
	do
	{

		if(!bOsdInitialised)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s osd hasn't been initialised error\n",__FUNCTION__);
			Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
			break;
		}
		
		SurfaceTmp = (TM_Image_Surface*)hSurface;
		if( SurfaceTmp == NULL)

		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "In Function :%s Error check image handle. \n",__FUNCTION__);

			Retcode = CSUDIOSG_ERROR_BAD_PARAMETER;
			break;
		}

		if((SurfaceTmp->used == FALSE)
				||(SurfaceTmp->pImageHandle == 0)
				||(SurfaceTmp->index == 0)
				||(SurfaceTmp->index == 1))

		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "In Function :%s Error check image handle. line=%d \n",__FUNCTION__,__LINE__ );
			
			Retcode = CSUDIOSG_ERROR_BAD_PARAMETER;
			break;
		}

		if((SurfaceTmp==&gUdiImageArray[0] )||(& gUdiImageArray[1]==SurfaceTmp))
		{
			if(&gUdiImageArray[0]==SurfaceTmp)
			{
				/* detatch image from presentation manager */
				Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_NONE;
				Input.hImageHandle = SurfaceTmp->pImageHandle;
				
				if( pgSDOSDSurface->set_input (pgSDOSDSurface, &Input ) != PIPE_STATUS_OK )
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s pgSDOSDSurface->set_input error \n",__FUNCTION__);
					Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
					break;
				} 
				Status = cnxt_image_close(SurfaceTmp->pImageHandle);
				if(Status != CNXT_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s cnxt_image_close error \n",__FUNCTION__);
					Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
					break; 
				}
				
				/* Enable the SD Surfaces */
				if( pgSDOSDSurface->surface_disable(pgSDOSDSurface) != PIPE_STATUS_OK )
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s surface_disable \n",__FUNCTION__);
					Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
					break; 
				}

			}
			else
			{
				/* detatch image from presentation manager */
				Input.Type = PIPE_VP_SURFACE_INPUT_TYPE_NONE;
				Input.hImageHandle = SurfaceTmp->pImageHandle;
				
				if( pgHDOSDSurface->set_input ( pgHDOSDSurface, &Input ) != PIPE_STATUS_OK )
				{
					
					CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s pgHDOSDSurface->set_input error \n",__FUNCTION__);
					Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
					break; 
				}
				
				Status = cnxt_image_close(SurfaceTmp->pImageHandle);	
				if(Status != CNXT_STATUS_OK)
				{
					
					CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s cnxt_image_close error \n",__FUNCTION__);
					Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
					break;
					
				}
				
				if( pgHDOSDSurface->surface_disable(pgHDOSDSurface) != PIPE_STATUS_OK )
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL, "In Function :%s cnxt_image_close error \n",__FUNCTION__);
					Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
					break;
				}
			}
			

			SurfaceTmp->used = FALSE;
			
			SurfaceTmp->pixelFormat = 0xff;
			SurfaceTmp->height = 0;
			SurfaceTmp->width = 0;
			SurfaceTmp->alphaValueFlag = 0;
			SurfaceTmp->alphaValue = 0xff;
			SurfaceTmp->colorKey = 0;
			SurfaceTmp->colorKeyFlag = 0;

		}
		else
		{
			
			Status = cnxt_image_close(SurfaceTmp->pImageHandle);
			if(Status != CNXT_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error closing image handle. IMAGEStatus = %d.\n", Retcode);
				Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;//????
				break;
			}
		}
		
		SurfaceTmp->used = FALSE;

		
		SurfaceTmp->pixelFormat = 0xff;
		SurfaceTmp->height = 0;
		SurfaceTmp->width = 0;
		SurfaceTmp->alphaValueFlag = 0;
		SurfaceTmp->alphaValue = 0xff;
		SurfaceTmp->colorKey = 0;
		SurfaceTmp->colorKeyFlag = 0;


	}while(0);
	return Retcode;
}


/**
@brief 得到显存surface句柄。

@param[in] nIndex 显存索引，从0开始直到没有更多的显存，返回失败为止。
@param[out] phSurface 成功返回显存Surface句柄。

@return 成功返回 CSUDI_SUCCESS,否则返回错误代码。

@note 
- 这种方式得到的Surface不能使用CSUDIOSGDestroySurface销毁。\n
- 如果高标清同时支持，nIndex 为 0 表示标清显存；nIndex 为 1 表示高清显存。\n
- 只需要返回所有用于完整OSD显示的显存,如标清显存，高清显存，不要返回特殊层显存如光标层，背景层等。\n
- 除非不能创建任何其他Surface，否则中间件一般不会直接操作显存地址进行绘制,该句柄多数情况下只会用来往上进行Blit。
*/
CSUDI_Error_Code CSUDIOSGGetDisplaySurface(int nIndex, CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	if (!bOsdInitialised)
	{

		return CSUDIOSG_ERROR_UNKNOWN_ERROR;
	}

	if (phSurface == CSUDI_NULL)
	{
		return CSUDIOSG_ERROR_BAD_PARAMETER;
	}

	*phSurface = CSUDI_NULL;
	
	if((nIndex != 0)&&(nIndex != 1))
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Error this is not display buffer.\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER;
	}
	

	if(nIndex == 0)
	{
		*phSurface = ((CSUDI_HANDLE)&gUdiImageArray[0]); 

	}
	if (nIndex == 1)
	{
		*phSurface = ((CSUDI_HANDLE)&gUdiImageArray[1]);
	}
	return Retcode;
}

/**
@brief 获取绘图平面或者显存空间的信息。

中间件会通过该接口获取的信息进行绘图操作。
@param[in] hSurface 绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值。
@param[out] psSurfaceInfo 保存得到的绘图平面或者显存空间信息。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 。
@note
- 此接口能够得到显存或者缓存的空间地址
- Alpha超过0xFF返回CSUDIOSG_ERROR_BAD_PARAMETER
- 若不支持0xFF级透明度，需驱动层自行将0-255映射到所支持的范围内，不得仅处理支持部分。
如某平台仅支持0-127的透明度，则该值传入255应相当于127；传入127应相当于63
*/
CSUDI_Error_Code CSUDIOSGGetSurfaceInfo(CSUDI_HANDLE hSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	CNXT_STATUS 				  Status;
	//CSUDI_Error_Code			  status = CSUDI_FAILURE;
	TM_Image_Surface			  *pImageSurface;
	CNXT_IMAGE_PAGE_PROPERTIES	  ImagePageProp;

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "ENTER Function :%s in. line=%d\n",__FUNCTION__,__LINE__);

	if (!bOsdInitialised)
	{

		return CSUDIOSG_ERROR_UNKNOWN_ERROR;
	}

	pImageSurface = (TM_Image_Surface*)hSurface;
	if( pImageSurface == NULL)
	{

		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "In Function :%s Error check image handle. line=%d\n",__FUNCTION__,__LINE__);
		return CSUDIOSG_ERROR_BAD_PARAMETER; 
	}
	
	if( (pImageSurface->used != TRUE)
			||(pImageSurface->pImageHandle == 0) )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "In Function :%s Error check image handle.\n",__FUNCTION__);
		return CSUDIOSG_ERROR_BAD_PARAMETER; 
	}

	if(psSurfaceInfo == NULL)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error check psSurfaceInfo.\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER; 
	}

	do
	{	
		/*<!-- Modify by lengwenhua,2010/09/10 09:40:20: */
		if (hSurface ==  &gUdiImageArray[0] || hSurface ==	&gUdiImageArray[1])
		{
			int nhSurfaceType = 0;
			
			if( hSurface == &gUdiImageArray[1] )
			{
				nhSurfaceType = 1;
			}
			
			Status = cnxt_kal_sem_get(gOsg_sem[ nhSurfaceType ], CNXT_KAL_WAIT_FOREVER);
			if (Status != CNXT_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
				Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
				break ;
			}
			
			Status = cnxt_image_get_page_properties(pImageSurface->pImageHandle,uDisplayPage[nhSurfaceType]%2, &ImagePageProp);
			if (Status != CNXT_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Error get page properties of a source image. IMAGEStatus = %d.\n",Retcode);
				Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
				Retcode = cnxt_kal_sem_put(gOsg_sem[ nhSurfaceType]);
				break;
			}
			
			
			Status = cnxt_kal_sem_put(gOsg_sem[ nhSurfaceType]);
			if ( Status != CNXT_STATUS_OK )
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_PUT FAILED %d \n", Retcode);
                Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
				break ;
			}
		}
		/*<!-- Modify by lengwenhua,2010/09/10 09:40:20: */
		else
		{
			Status = cnxt_image_get_page_properties(pImageSurface->pImageHandle,0, &ImagePageProp);
			if (Status != CNXT_STATUS_OK)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Error get page properties of a source image. IMAGEStatus = %d.\n",Retcode);
				Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;				
				break;
			}
		}
		psSurfaceInfo->m_ePixelFormat	= (CSUDIOSGPixelFormat_E)pImageSurface->pixelFormat;
		psSurfaceInfo->m_pvSurfaceMem	= (void *)ImagePageProp.PageData.puPlanes[0];
		psSurfaceInfo->m_nMemSize		= ImagePageProp.uPageSize;
		psSurfaceInfo->m_nAlphaFlag 	= pImageSurface->alphaValueFlag; //?? vijay The flag of 1 indicates that the plane's global m_uAlpha work, is initialized to 0 
		psSurfaceInfo->m_uAlpha 		= pImageSurface->alphaValue;
		psSurfaceInfo->m_nColorKeyFlag	= pImageSurface->colorKeyFlag;//??vijay The flag is 1, said m_uColorKey work, is initialized to 0
		psSurfaceInfo->m_uColorkey		= pImageSurface->colorKey;
		psSurfaceInfo->m_nWidth 		= ImagePageProp.PageSetProp.uBitmapW;
		psSurfaceInfo->m_nHeight		= ImagePageProp.PageSetProp.uBitmapH;
		psSurfaceInfo->m_nPitch 		= ImagePageProp.uStride;
		//psSurfaceInfo->m_pvPalette	  = (void *)ImagePalette.pPalEntry;//??vijay m_ePixelFormat specified when the color palette for the EM_UDIOSG_PIXEL_FORMAT_PALETTE8 data area, there is no palette Surface, structure, reserved for the palette, so to use void */
		psSurfaceInfo->m_pvPalette		= NULL;

		Retcode = CSUDI_SUCCESS;
	}while(0);
	return Retcode;
}

/**
@brief 设置绘图平面的信息，只能设置绘图平面的m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey四个成员，对其他成员操作无效。

@param[in] hSurface 绘图平面句柄.CSUDIOSGCreateSurface返回值。
@param[in] psSurfaceInfo 保存得到的绘图平面或者显存空间信息。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 。
@note 
- 此接口在CSUDIOSGGetSurfaceInfo后使用，只能设置绘图平面的信息，不能设置显存空间的信息。\n
- 此接口在仅仅用于改变Surface的m_uAlpha和m_uColorkey属性，并不会对显示产生变化。\n
- 当psSurfaceInfo中同时有除m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey以外的成员改变时，其他成员修改操作无效，仅仅对m_nAlphaFlag、m_uAlpha和m_nColorKeyFlag、m_uColorkey生效，返回CSUDI_SUCCESS。
- m_uAlpha超过0xFF返回CSUDIOSG_ERROR_BAD_PARAMETER
- 若不支持0xFF级透明度，需驱动层自行将0-255映射到所支持的范围内，不得仅处理支持部分。
如某平台仅支持0-127的透明度，则该值传入255应相当于127；传入127应相当于63
*/
CSUDI_Error_Code CSUDIOSGSetSurfaceInfo(CSUDI_HANDLE hSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	CNXT_STATUS    retcode;
	CSUDI_Error_Code status = CSUDI_SUCCESS;
	TM_Image_Surface		*pImageSurface;
	CNXT_IMAGE_PAGE_SET_PROPERTIES	pProperties;
	int pixel_index;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "function %s in \n",__FUNCTION__);

	if (!bOsdInitialised)
	{

		return CSUDIOSG_ERROR_UNKNOWN_ERROR;
	}

	pImageSurface = (TM_Image_Surface*)hSurface;
	if( pImageSurface == NULL)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "In Function :%s Error check image handle. line=%d\n",__FUNCTION__,__LINE__);
		return CSUDIOSG_ERROR_BAD_PARAMETER; 
	}

	if((pImageSurface->used == FALSE)
			||(pImageSurface->pImageHandle == 0))
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "In Function :%s Error check image handle. line=%d\n",__FUNCTION__,__LINE__);
		return CSUDIOSG_ERROR_BAD_PARAMETER; 
	}

	if(psSurfaceInfo == NULL)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error check psSurfaceInfo.\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER; 
	}

	if((psSurfaceInfo->m_uAlpha > 0xFF))
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error Alpha value beyond 0xFF.\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER;
	}

	if ((psSurfaceInfo->m_nAlphaFlag!=0&&psSurfaceInfo->m_nAlphaFlag!=1)
			||(psSurfaceInfo->m_nColorKeyFlag!=0&&psSurfaceInfo->m_nColorKeyFlag!=1))
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error Alpha or colorkey flag.\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER;
	}

	if (hSurface == &gUdiImageArray[0] ||hSurface == &gUdiImageArray[1])
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "can't set display surface\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER;
	}
	
	switch(pImageSurface->pixelFormat)
	{
	case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		pixel_index = 0;
		break;
		

		
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_420:						
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_444:			
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422: 
		pixel_index = 1;
		break;
	default:
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Not support.\n");
			return CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED; 
		}
	}

	if(psSurfaceInfo->m_nColorKeyFlag == 1)
	{
		if(pixel_index == 0)
		{
			pProperties.TransLower.ColorType = RGB;
			pProperties.TransUpper.ColorType = RGB;
			pProperties.TransLower.Color.RGB.uAlpha = 0x00 ;
			pProperties.TransUpper.Color.RGB.uAlpha = 0xff ;

			pProperties.TransLower.Color.RGB.uRed	= (u_int8)((psSurfaceInfo->m_uColorkey)>>16);
			pProperties.TransLower.Color.RGB.uGreen = (u_int8)((psSurfaceInfo->m_uColorkey)>>8);
			pProperties.TransLower.Color.RGB.uBlue	= (u_int8)(psSurfaceInfo->m_uColorkey);

			pProperties.TransUpper.Color.RGB.uRed	= (u_int8)((psSurfaceInfo->m_uColorkey)>>16);
			pProperties.TransUpper.Color.RGB.uGreen = (u_int8)((psSurfaceInfo->m_uColorkey)>>8);
			pProperties.TransUpper.Color.RGB.uBlue	= (u_int8)(psSurfaceInfo->m_uColorkey);
		}
		else
		{
			pProperties.TransLower.ColorType = YCC;
			pProperties.TransUpper.ColorType = YCC;

			pProperties.TransLower.Color.YCC.uY  = (u_int8)((psSurfaceInfo->m_uColorkey)>>16);
			pProperties.TransLower.Color.YCC.uCb = (u_int8)((psSurfaceInfo->m_uColorkey)>>8);
			pProperties.TransLower.Color.YCC.uCr = (u_int8)(psSurfaceInfo->m_uColorkey);
			pProperties.TransLower.Color.YCC.uAlpha = 0x00;

			pProperties.TransUpper.Color.YCC.uY  = (u_int8)((psSurfaceInfo->m_uColorkey)>>16);
			pProperties.TransUpper.Color.YCC.uCb = (u_int8)((psSurfaceInfo->m_uColorkey)>>8);
			pProperties.TransUpper.Color.YCC.uCr = (u_int8)(psSurfaceInfo->m_uColorkey);
			pProperties.TransUpper.Color.YCC.uAlpha = 0xff;
		}
		
		retcode = cnxt_image_set_page_properties( pImageSurface->pImageHandle,0,IMAGE_PAGE_CHROMAKEY, &pProperties); 
		if(retcode != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Error get page properties of a source image. IMAGEStatus = %d.\n",retcode);
			status = CSUDIOSG_ERROR_UNKNOWN_ERROR;
		}
	}


	
	pImageSurface->alphaValueFlag = psSurfaceInfo->m_nAlphaFlag;
	pImageSurface->alphaValue = psSurfaceInfo->m_uAlpha;
	pImageSurface->colorKeyFlag = psSurfaceInfo->m_nColorKeyFlag;
	pImageSurface->colorKey = psSurfaceInfo->m_uColorkey;
	//	pImageSurface->pixelFormat = pImageSurface->pixelFormat;
	return Retcode;
}


/**
@brief Blit操作接口，以利用硬件加速性能，异步操作。

@param[in] hDstSurface 目标绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值。
@param[in] psDstRect 目标surface或显示空间hDstSurface上矩形区域,如果为空，则取hDstSurface的全部区域。
@param[in] hSrcSurface 源绘图平面或者显存空间句柄。
@param[in] psSrcRect 源surface或显示空间hSrcSurface上矩形区域,如果为空，则取hSrcSurface的全部区域。
@param[in] eOpMode 混合模式，见CSUDIOSGOpMode_E定义。
@return 成功返回 CSUDI_SUCCESS，其它返回错误代码。
@note 
- 如果是alpha混合模式，全局alpha值根据源Surface和目的Surface的alpha flag 确定，如果源和目的Surface的alpha flag全为1，则都使用.混合时的alpha值根据
      三者比例得出，例如:如果只有源的alpha flag 为1， 混合的alpha为 Sa * Pa / 255 （Sa为源surface的全局alpha，Pa为源surface的一个像素的alpha）
- 如果硬件不支持，不需要软件模拟实现，直接返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
- 如果不支持硬件加速请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，即除非有更高效的实现方式或者其它充分的理由,否则不要对该操作进行软件模拟。\n
- 源和目标的区域大小psSrcRect、psDstRect可以为空，但是非空时，必须是对应surface上大小的范围内，不支持裁剪则返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标均可能为显存Surface或者普通Surface，若不支持请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标可能为同一个Surface，若不支持请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标可能为同一个Surface时，psDstRect和psSrcRect不允许重叠，否则返回CSUDIOSG_ERROR_BAD_PARAMETER。\n
- 源和目标的颜色模式可能不一致，一般不推荐这种使用，若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标可能分别是YUV和RGB的颜色模式，若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。\n
- 源和目标矩形的大小可能不一致,若不支持可返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,但一般来说高清平台必须支持，软件的缩放效率极低，中间件不会提供。\n
- 源和目标的矩形可能有缩放、拉伸等处理,若不支持请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED，需支持2D特效的平台必须实现该功能。\n
- 为充分提高绘图性能，该接口可实现成异步模式，通过CSUDIOSGSync确保该操作执行完毕。\n
- 在CSUDIOSGSync返回前改变相关Surface，绘图效果是不确定的，但不能死机或产生其他不可恢复的异常。\n
- 如果平台同时支持高标清显示，要求在向标清显存blit时，一定要设置防闪(或者防锯齿)操作。\n
- 平台支持的拉伸和缩放比例至少为: 1:15。\n
- 进行blit操作时，只有源的colorkey有效，即当源绘图平面的colorkey有效时，源平面所有颜色为colorkey的像素均不会被blit到目标平面。
*/
CSUDI_Error_Code CSUDIOSGBlit(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect,
                        CSUDI_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 填充目标绘图平面或者显存空间上一块矩形。

@param[in] hDstSurface 绘图平面或者显存空间句柄.CSUDIOSGCreateSurface或者CSUDIOSGGetDisplaySurface的返回值。
@param[in] psDstRect 要填充在目标绘图平面上的位置和大小，为空时，表示全屏填充。
@param[in] uPixel 要填充的颜色值，ARGB8888格式的像素值。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值。
@note 
- 为充分提高绘图性能，该接口需要实现成异步模式，通过CSUDIOSGSync确保该操作执行完毕，在CSUDIOSGSync返回前改变相关Surface，绘图效果是不确定的，但不能死机或产生其他不可恢复的异常。\n
- 必须使用硬件加速Fill,如果硬件不支持，请返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,即除非有更高效的实现方式或者其它充分的理由,否则不要对该操作进行软件模拟。\n
- psDstRect 必须在hDstSurface的大小范围内，如果超过hDstSurface，则返回CSUDIOSG_ERROR_BAD_PARAMETER。
- 若hDstSurface不是ARGB8888格式的则需在移植层完成转换工作。
*/
CSUDI_Error_Code CSUDIOSGFill(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 同步CSUDIOSGBlit，CSUDIOSGFill等OSG操作。

@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值。
@note 
- 在CSUDIOSGBlit，CSUDIOSGFill等之后调用,函数返回时，前述操作全部完成。\n
- 该接口并不是显示Surface上的内容。 \n
- 如果CSUDIOSGBlit和CSUDIOSGFill是同步的操作，建议该接口放空，直接返回成功。 \n
- 中间件利用软件操作Surface的内存时，也会用此接口同步Surface的内存，为了保证操作成功，防止数据处于cache状态，如果该芯片
- 无此cache问题，则建议放空，直接返回成功。
- 中间件在画图过程中会频繁使用此接口。
- 该接口与Blit的关系可参考C99标准fflush。
*/
CSUDI_Error_Code CSUDIOSGSync(void)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 硬件解码图片,包括I帧格式(MPEG,BMP)/JPEG格式/PNG格式等。

@param[in] pszImageType 图片的类型,使用字符串避免了定义多的枚举类型。格式与类型的定义: IFRAME - I帧格式，BMP - bmp格式, JPEG - jpeg格式, PNG - png格式
@param[in] pvImageMem  图片数据地址。
@param[in] nMemSize   图片数据大小。
@param[in] eScalingMode 解码输出模式，控制解码的缩放
@param[in] psDstRect 解码输出Surface的大小，仅宽高数据有效，根据eScalingMode的不同该值的效果有所不同
@param[out] phSurface 解码后的数据存放的surface。
@return 成功返回 CSUDI_SUCCESS,否则返回错误代码。
@note 
- 必须实现对MPEG格式的硬件解码，其他格式允许不支持，此时返回CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED。
- 本接口将创建一个绘图平面，通过phSurface返回给上层使用，在使用完之后，必须调用CSUDIOSGDestroySurface释放。
- 如果本接口解码图片(例如I帧)用到的设备处于忙状态，请返回CSUDIOSG_ERROR_DEVICE_BUSY。
- 解码I帧一般需要用到Video解码器，会与视频播放冲突，本接口应尽量使用闲置Video解码器进行解码，若实在找不到闲置解码器则返回CSUDIOSG_ERROR_DEVICE_BUSY，
此情况下EastWin如必须要进行解码，则应通知上层暂停视频播放后再次调用该接口。
- 本接口仅适用于可以通过硬件方式加速解码的情况，除非有非常充分的理由，否则请不要进行任何软件模拟实现。
@note eScalingMode与psDstRect的关系
- EM_UDIOSG_SCALE_FULLSIZE：psDstRect不生效，可以为CSUDI_NULL，输出的Surface宽高与原始图片保持一致
- EM_UDIOSG_SCALE_OPTIMUM：最适合缩放，图片在保持完整和比例的前提下，尽量大的放置在psDstRect区域中，但不留边。\n
即当比例不一致时输出Surface的大小会在某一个方向（宽或高）比psDstRect小。\n
如：图片原始大小为1000*500，给定的区域为100*100，则输出的Surface大小为100*50，图片按比例占满整个Surface。
- EM_UDIOSG_SCALE_FIXSIZE：拉升，psDstRect.m_nWidth，psDstRect.m_nHeight均有效，输出的Surface高度为psDstRect.m_nHeight，宽度为psDstRect.m_nWidth
*/
CSUDI_Error_Code CSUDIOSGDecodeImage(const char * pszImageType, const void * pvImageMem, int nMemSize,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect,CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);
	CSUDI_Error_Code status = CSUDI_SUCCESS;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "function %s in \n",__FUNCTION__);

	if (!bOsdInitialised)
	{

		return CSUDIOSG_ERROR_UNKNOWN_ERROR;
	}
	
//	uStartTimeMs_start=CSUDIPlusOSGetTickCount();
	if((pszImageType == NULL)||(pvImageMem == NULL)||(nMemSize <= 0))
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "CSUDIOSG_ERROR_BAD_PARAMETER.\n");
		return CSUDIOSG_ERROR_BAD_PARAMETER;
	}
	if((eScalingMode == EM_UDIOSG_SCALE_OPTIMUM)||(eScalingMode == EM_UDIOSG_SCALE_FIXSIZE))
	{
		if(psDstRect == NULL)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "CSUDIOSG_ERROR_BAD_PARAMETER.\n");
			return CSUDIOSG_ERROR_BAD_PARAMETER;
		}
	}
	do
	{
		switch(pszImageType[0])
		{
		case 'J':
			{
			BOOL bRe = FALSE;
			bRe = bIsHWDecodeSupported((u_int8*)pvImageMem,nMemSize);
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  " image pixel  %s support.\n",bRe?"is":"not");
			if(!bRe)
			{
				status = CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED;
			}
			else
			{
				*phSurface = Decode_stillimage(CS_TM_JPEG,pvImageMem,nMemSize,eScalingMode,psDstRect);
				 if(*phSurface == NULL)
				{
					 CSDEBUG(MODULE_NAME, ERROR_LEVEL,	" StillDecode_Jpeg(pvImageMem,nMemSize,eScalingMode,psDstRect); .phSurface==NULL\n");
						 status = CSUDIOSG_ERROR_UNKNOWN_ERROR;
					 
				 }
			}
		}		  
		#if 0
			{
				*phSurface = Decode_stillimage(CS_TM_JPEG,pvImageMem,nMemSize,eScalingMode,psDstRect);
				if(*phSurface == NULL)
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,  " StillDecode_Jpeg(pvImageMem,nMemSize,eScalingMode,psDstRect); .phSurface==NULL\n");
					status = CSUDIOSG_ERROR_UNKNOWN_ERROR;
				}
			}
			#endif
			break;
		case 'I':
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"CSUDIOSGDecodeImage: decoding iframe......\n\r");
			*phSurface = Decode_stillimage(CS_TM_IFRAME,pvImageMem,nMemSize,eScalingMode,psDstRect);
			if(*phSurface == NULL)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Decode_stillimage(CS_TM_IFRAME,pvImageMem,nMemSize,eScalingMode,psDstRect) .CSUDIOSG_ERROR_UNKNOWN_ERROR\n");
				status = CSUDIOSG_ERROR_UNKNOWN_ERROR;
			}
			break;
		case 'P': 		
		case 'B': 		
		default:
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,	"Error check image pixel format.line=%d\n",__LINE__);
			status = CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED;
		}
	}while(0);
#if 0	
	uStartTimeMs_end=CSUDIPlusOSGetTickCount();
		
	s_TotalTime_full  += uStartTimeMs_end - uStartTimeMs_start;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"EXIT Function :%s %d\n",__FUNCTION__,__LINE__);
	printf("[%03d] decode jpg once %d ms += Total time %d ms.\n", 
		s_cnt++, uStartTimeMs_end-uStartTimeMs_start, s_TotalTime_full);
#endif
	return Retcode;
}

/**
@brief 获取3D显示必须的NativeDisplay句柄
@param[out] phNativeDisplay NativeDisplay句柄。
@return 返回错误类型。
@note 
-NativeDisplay句柄与具体平台中opengl es的实现相关。
-类似于win32的hdc
*/
CSUDI_Error_Code  CSUDIOSGGetNativeDisplay(CSUDI_HANDLE * phNativeDisplay)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取hSurface对应的NativeWindow句柄
@param[in]  hSurface Surface句柄
@param[out] phNativeWindow NativeWindow句柄
@return 返回错误类型。
@note 
-NativeWindow句柄与具体平台中opengl es的实现相关。
-类似于win32的window
*/
CSUDI_Error_Code CSUDIOSGGetNativeWindow (CSUDI_HANDLE hSurface, CSUDI_HANDLE *phNativeWindow)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

