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
@brief ����һ����ͼƽ��

@param[in] ePixelFormat ��ͼƽ�����ɫģʽ��
@param[in] nWidth  ��ͼƽ��Ŀ�ȣ�������� 0��
@param[in] nHeight ��ͼƽ��ĸ߶ȣ�������� 0��
@param[out] phSurface �ɹ����ػ�ͼƽ��ľ����ʧ��Ϊ�գ���ϸ����μ��������ֵ��
@return �ɹ����� CSUDI_SUCCESS,���򷵻ش������ֵ�� 
@note 

- �ýӿڴ�����ʾ����ռ䣬�����Ͽ���֧�����޴�֧�ִ������޶��ֱ���ڴ治�㣬(�����ṩ16M���ڴ����ڴ���Surface)��������Ҫ����Surface������Ŀ����Ĳ�ͬ����ͬ��ĿǰӦ���õ��Ƚ϶����:\n
     1����ϷӦ���еĿɱ�ͼƬ���õ���Surface�� \n
     2��JavaӦ�����õ�BufferedImage�õ���Surface��\n 
     3��2D��Ч��ʵ�ֻ��õ���Surface)�� \n
- �˴���ɫģʽ������������ʾ����ɫģʽ��ͬ����Blitʱ�����⴦���ɣ����ƽ̨��֧�������Ĵ���Ҳ����Ӧ��ѡ����ɫģʽʱ���Ա��⡣\n
- ���ƽ̨��Դʵ�����ޣ�Ҳ�������ܴ����κ�Surface����������»�ֱ�Ӳ����Դ棬������Ҫ��������Դ�ķֱ��ʣ���ɫģʽ�ȱ���һ�¡�
- ���Ӳ����֧���ٴ���Surfaceʱ�����鷵��ʧ�ܡ���ʱ�м����ģ�ⴴ��surface�����в�����
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
@brief ����һ����ͼƽ�棬�ͷŸû�ͼƽ����ڴ档

@param[in] hSurface CSUDIOSGCreateSurface �ɹ����صľ����
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ 
@note 
- ��CSUDIOSGCreateSurface�����Ļ�������ʹ�ú�һ��Ҫ���ô˽ӿ������ͷš�\n
- ��������һ���Դ�ռ䣬������CSUDIOSG_ERROR_BAD_PARAMETER��
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
@brief �õ��Դ�surface�����

@param[in] nIndex �Դ���������0��ʼֱ��û�и�����Դ棬����ʧ��Ϊֹ��
@param[out] phSurface �ɹ������Դ�Surface�����

@return �ɹ����� CSUDI_SUCCESS,���򷵻ش�����롣

@note 
- ���ַ�ʽ�õ���Surface����ʹ��CSUDIOSGDestroySurface���١�\n
- ����߱���ͬʱ֧�֣�nIndex Ϊ 0 ��ʾ�����Դ棻nIndex Ϊ 1 ��ʾ�����Դ档\n
- ֻ��Ҫ����������������OSD��ʾ���Դ�,������Դ棬�����Դ棬��Ҫ����������Դ�����㣬������ȡ�\n
- ���ǲ��ܴ����κ�����Surface�������м��һ�㲻��ֱ�Ӳ����Դ��ַ���л���,�þ�����������ֻ���������Ͻ���Blit��
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
@brief ��ȡ��ͼƽ������Դ�ռ����Ϣ��

�м����ͨ���ýӿڻ�ȡ����Ϣ���л�ͼ������
@param[in] hSurface ��ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ��
@param[out] psSurfaceInfo ����õ��Ļ�ͼƽ������Դ�ռ���Ϣ��
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ ��
@note
- �˽ӿ��ܹ��õ��Դ���߻���Ŀռ��ַ
- Alpha����0xFF����CSUDIOSG_ERROR_BAD_PARAMETER
- ����֧��0xFF��͸���ȣ������������н�0-255ӳ�䵽��֧�ֵķ�Χ�ڣ����ý�����֧�ֲ��֡�
��ĳƽ̨��֧��0-127��͸���ȣ����ֵ����255Ӧ�൱��127������127Ӧ�൱��63
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
@brief ���û�ͼƽ�����Ϣ��ֻ�����û�ͼƽ���m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey�ĸ���Ա����������Ա������Ч��

@param[in] hSurface ��ͼƽ����.CSUDIOSGCreateSurface����ֵ��
@param[in] psSurfaceInfo ����õ��Ļ�ͼƽ������Դ�ռ���Ϣ��
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ ��
@note 
- �˽ӿ���CSUDIOSGGetSurfaceInfo��ʹ�ã�ֻ�����û�ͼƽ�����Ϣ�����������Դ�ռ����Ϣ��\n
- �˽ӿ��ڽ������ڸı�Surface��m_uAlpha��m_uColorkey���ԣ����������ʾ�����仯��\n
- ��psSurfaceInfo��ͬʱ�г�m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey����ĳ�Ա�ı�ʱ��������Ա�޸Ĳ�����Ч��������m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey��Ч������CSUDI_SUCCESS��
- m_uAlpha����0xFF����CSUDIOSG_ERROR_BAD_PARAMETER
- ����֧��0xFF��͸���ȣ������������н�0-255ӳ�䵽��֧�ֵķ�Χ�ڣ����ý�����֧�ֲ��֡�
��ĳƽ̨��֧��0-127��͸���ȣ����ֵ����255Ӧ�൱��127������127Ӧ�൱��63
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
@brief Blit�����ӿڣ�������Ӳ���������ܣ��첽������

@param[in] hDstSurface Ŀ���ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ��
@param[in] psDstRect Ŀ��surface����ʾ�ռ�hDstSurface�Ͼ�������,���Ϊ�գ���ȡhDstSurface��ȫ������
@param[in] hSrcSurface Դ��ͼƽ������Դ�ռ�����
@param[in] psSrcRect Դsurface����ʾ�ռ�hSrcSurface�Ͼ�������,���Ϊ�գ���ȡhSrcSurface��ȫ������
@param[in] eOpMode ���ģʽ����CSUDIOSGOpMode_E���塣
@return �ɹ����� CSUDI_SUCCESS���������ش�����롣
@note 
- �����alpha���ģʽ��ȫ��alphaֵ����ԴSurface��Ŀ��Surface��alpha flag ȷ�������Դ��Ŀ��Surface��alpha flagȫΪ1����ʹ��.���ʱ��alphaֵ����
      ���߱����ó�������:���ֻ��Դ��alpha flag Ϊ1�� ��ϵ�alphaΪ Sa * Pa / 255 ��SaΪԴsurface��ȫ��alpha��PaΪԴsurface��һ�����ص�alpha��
- ���Ӳ����֧�֣�����Ҫ���ģ��ʵ�֣�ֱ�ӷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
- �����֧��Ӳ�������뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED���������и���Ч��ʵ�ַ�ʽ����������ֵ�����,����Ҫ�Ըò����������ģ�⡣\n
- Դ��Ŀ��������СpsSrcRect��psDstRect����Ϊ�գ����Ƿǿ�ʱ�������Ƕ�Ӧsurface�ϴ�С�ķ�Χ�ڣ���֧�ֲü��򷵻�CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ�������Ϊ�Դ�Surface������ͨSurface������֧���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ�����Ϊͬһ��Surface������֧���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ�����Ϊͬһ��Surfaceʱ��psDstRect��psSrcRect�������ص������򷵻�CSUDIOSG_ERROR_BAD_PARAMETER��\n
- Դ��Ŀ�����ɫģʽ���ܲ�һ�£�һ�㲻�Ƽ�����ʹ�ã�����֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ����ֱܷ���YUV��RGB����ɫģʽ������֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ����εĴ�С���ܲ�һ��,����֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,��һ����˵����ƽ̨����֧�֣����������Ч�ʼ��ͣ��м�������ṩ��\n
- Դ��Ŀ��ľ��ο��������š�����ȴ���,����֧���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED����֧��2D��Ч��ƽ̨����ʵ�ָù��ܡ�\n
- Ϊ�����߻�ͼ���ܣ��ýӿڿ�ʵ�ֳ��첽ģʽ��ͨ��CSUDIOSGSyncȷ���ò���ִ����ϡ�\n
- ��CSUDIOSGSync����ǰ�ı����Surface����ͼЧ���ǲ�ȷ���ģ�����������������������ɻָ����쳣��\n
- ���ƽ̨ͬʱ֧�ָ߱�����ʾ��Ҫ����������Դ�blitʱ��һ��Ҫ���÷���(���߷����)������\n
- ƽ̨֧�ֵ���������ű�������Ϊ: 1:15��\n
- ����blit����ʱ��ֻ��Դ��colorkey��Ч������Դ��ͼƽ���colorkey��Чʱ��Դƽ��������ɫΪcolorkey�����ؾ����ᱻblit��Ŀ��ƽ�档
*/
CSUDI_Error_Code CSUDIOSGBlit(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect,
                        CSUDI_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ���Ŀ���ͼƽ������Դ�ռ���һ����Ρ�

@param[in] hDstSurface ��ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ��
@param[in] psDstRect Ҫ�����Ŀ���ͼƽ���ϵ�λ�úʹ�С��Ϊ��ʱ����ʾȫ����䡣
@param[in] uPixel Ҫ������ɫֵ��ARGB8888��ʽ������ֵ��
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ��
@note 
- Ϊ�����߻�ͼ���ܣ��ýӿ���Ҫʵ�ֳ��첽ģʽ��ͨ��CSUDIOSGSyncȷ���ò���ִ����ϣ���CSUDIOSGSync����ǰ�ı����Surface����ͼЧ���ǲ�ȷ���ģ�����������������������ɻָ����쳣��\n
- ����ʹ��Ӳ������Fill,���Ӳ����֧�֣��뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,�������и���Ч��ʵ�ַ�ʽ����������ֵ�����,����Ҫ�Ըò����������ģ�⡣\n
- psDstRect ������hDstSurface�Ĵ�С��Χ�ڣ��������hDstSurface���򷵻�CSUDIOSG_ERROR_BAD_PARAMETER��
- ��hDstSurface����ARGB8888��ʽ����������ֲ�����ת��������
*/
CSUDI_Error_Code CSUDIOSGFill(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ͬ��CSUDIOSGBlit��CSUDIOSGFill��OSG������

@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ��
@note 
- ��CSUDIOSGBlit��CSUDIOSGFill��֮�����,��������ʱ��ǰ������ȫ����ɡ�\n
- �ýӿڲ�������ʾSurface�ϵ����ݡ� \n
- ���CSUDIOSGBlit��CSUDIOSGFill��ͬ���Ĳ���������ýӿڷſգ�ֱ�ӷ��سɹ��� \n
- �м�������������Surface���ڴ�ʱ��Ҳ���ô˽ӿ�ͬ��Surface���ڴ棬Ϊ�˱�֤�����ɹ�����ֹ���ݴ���cache״̬�������оƬ
- �޴�cache���⣬����ſգ�ֱ�ӷ��سɹ���
- �м���ڻ�ͼ�����л�Ƶ��ʹ�ô˽ӿڡ�
- �ýӿ���Blit�Ĺ�ϵ�ɲο�C99��׼fflush��
*/
CSUDI_Error_Code CSUDIOSGSync(void)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief Ӳ������ͼƬ,����I֡��ʽ(MPEG,BMP)/JPEG��ʽ/PNG��ʽ�ȡ�

@param[in] pszImageType ͼƬ������,ʹ���ַ��������˶�����ö�����͡���ʽ�����͵Ķ���: IFRAME - I֡��ʽ��BMP - bmp��ʽ, JPEG - jpeg��ʽ, PNG - png��ʽ
@param[in] pvImageMem  ͼƬ���ݵ�ַ��
@param[in] nMemSize   ͼƬ���ݴ�С��
@param[in] eScalingMode �������ģʽ�����ƽ��������
@param[in] psDstRect �������Surface�Ĵ�С�������������Ч������eScalingMode�Ĳ�ͬ��ֵ��Ч��������ͬ
@param[out] phSurface ���������ݴ�ŵ�surface��
@return �ɹ����� CSUDI_SUCCESS,���򷵻ش�����롣
@note 
- ����ʵ�ֶ�MPEG��ʽ��Ӳ�����룬������ʽ����֧�֣���ʱ����CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��
- ���ӿڽ�����һ����ͼƽ�棬ͨ��phSurface���ظ��ϲ�ʹ�ã���ʹ����֮�󣬱������CSUDIOSGDestroySurface�ͷš�
- ������ӿڽ���ͼƬ(����I֡)�õ����豸����æ״̬���뷵��CSUDIOSG_ERROR_DEVICE_BUSY��
- ����I֡һ����Ҫ�õ�Video��������������Ƶ���ų�ͻ�����ӿ�Ӧ����ʹ������Video���������н��룬��ʵ���Ҳ������ý������򷵻�CSUDIOSG_ERROR_DEVICE_BUSY��
�������EastWin�����Ҫ���н��룬��Ӧ֪ͨ�ϲ���ͣ��Ƶ���ź��ٴε��øýӿڡ�
- ���ӿڽ������ڿ���ͨ��Ӳ����ʽ���ٽ��������������зǳ���ֵ����ɣ������벻Ҫ�����κ����ģ��ʵ�֡�
@note eScalingMode��psDstRect�Ĺ�ϵ
- EM_UDIOSG_SCALE_FULLSIZE��psDstRect����Ч������ΪCSUDI_NULL�������Surface�����ԭʼͼƬ����һ��
- EM_UDIOSG_SCALE_OPTIMUM�����ʺ����ţ�ͼƬ�ڱ��������ͱ�����ǰ���£�������ķ�����psDstRect�����У��������ߡ�\n
����������һ��ʱ���Surface�Ĵ�С����ĳһ�����򣨿��ߣ���psDstRectС��\n
�磺ͼƬԭʼ��СΪ1000*500������������Ϊ100*100���������Surface��СΪ100*50��ͼƬ������ռ������Surface��
- EM_UDIOSG_SCALE_FIXSIZE��������psDstRect.m_nWidth��psDstRect.m_nHeight����Ч�������Surface�߶�ΪpsDstRect.m_nHeight�����ΪpsDstRect.m_nWidth
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
@brief ��ȡ3D��ʾ�����NativeDisplay���
@param[out] phNativeDisplay NativeDisplay�����
@return ���ش������͡�
@note 
-NativeDisplay��������ƽ̨��opengl es��ʵ����ء�
-������win32��hdc
*/
CSUDI_Error_Code  CSUDIOSGGetNativeDisplay(CSUDI_HANDLE * phNativeDisplay)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡhSurface��Ӧ��NativeWindow���
@param[in]  hSurface Surface���
@param[out] phNativeWindow NativeWindow���
@return ���ش������͡�
@note 
-NativeWindow��������ƽ̨��opengl es��ʵ����ء�
-������win32��window
*/
CSUDI_Error_Code CSUDIOSGGetNativeWindow (CSUDI_HANDLE hSurface, CSUDI_HANDLE *phNativeWindow)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

