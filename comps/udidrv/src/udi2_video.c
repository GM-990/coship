/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_video.c
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

#include "udi2_video.h"
#include "generic_include.h"

#include "udi2_os.h"
#include "tm_osg.h"
#include "tm_video.h"

#define MODULE_NAME              ("CS_VIDEO")

extern CS_TM_VP_DEVICE_OBJECTS gTmVpDeviceObj;
extern CS_TM_PIPE_OBJECTS gTmPipeObject;
extern CNXT_GRAPHICS_HANDLE   gTridGraphicsHandle;
static CNXT_SEM_ID gUdi2VideoSem;
extern void CS_CNXT_SD_DRM_Notify(void);
extern void CS_CNXT_HD_DRM_Notify(void);
CSUDI_Error_Code TM_internal_SetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect);
CSUDI_Error_Code TM_internal_SetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect );

extern int cs_tm_get_surface_indx(int VidIndx);

CS_CNXT_WINDOWSIZE  CSCNXTWINDOWSIZES[] =
{
	{PIPE_VP_DISPLAY_MODE_NTSC,720,480},
	{PIPE_VP_DISPLAY_MODE_PAL,720,576},
	{PIPE_VP_DISPLAY_MODE_480P,720,480},
	{PIPE_VP_DISPLAY_MODE_576p,720,576},
	{PIPE_VP_DISPLAY_MODE_720P,1280,720},
	{PIPE_VP_DISPLAY_MODE_720P_50Hz,1280,720},
	{PIPE_VP_DISPLAY_MODE_1080I,1920,1080},
	{PIPE_VP_DISPLAY_MODE_1080I_50Hz,1920,1080}
};

#define NUM_RESOLUTIONS (sizeof(CSCNXTWINDOWSIZES)/sizeof(CS_CNXT_WINDOWSIZE))
u_int32 guNumResolutions = NUM_RESOLUTIONS;


static CSUDI_Error_Code CS_TRID_GetFullScreenWidowSize(CSUDISCREENType_E eScreenDevice,
															 CSUDISCREENResolution_E eResolution,
															 CSUDIWinRect_S * pstRect)
{
 CSUDI_Error_Code enRet = CSUDI_SUCCESS;
 
 pstRect->m_nX = 0;
 pstRect->m_nY = 0;

 if (eScreenDevice == EM_UDI_VOUT_DEVICE_SD)
 {
	 pstRect->m_nWidth = 720;
	 switch(eResolution)
	 {
	 case EM_UDISCREEN_RESOLUTION_NTSC: 		 ///<  SD: EM_UDICTL_MIX_ENCODING_MODE_NTSCM 
	 case EM_UDISCREEN_RESOLUTION_NTSC_443:  ///<  NTSC M mode but with 4.43361875MHz color subcarrier frequency. 
	 case EM_UDISCREEN_RESOLUTION_NTSC_JAPAN:	 ///<  Japan NTSC, no pedestal level 
	 case EM_UDISCREEN_RESOLUTION_480P: 	  ///< NTSC Progressive (27Mhz)  
		 pstRect->m_nHeight = 480;
		 break;
	 case EM_UDISCREEN_RESOLUTION_PAL:				 ///<  SD: EM_UDICTL_MIX_ENCODING_MODE_PALBDGHI 
	 case EM_UDISCREEN_RESOLUTION_PAL_M:		 ///<  PAL Brazil 
	 case EM_UDISCREEN_RESOLUTION_PAL_N:		 ///< PAL_N 
	 case EM_UDISCREEN_RESOLUTION_PAL_NC:		 ///<  PAL_N, Argentina (PAL N mode but with 3.58205625 MHz color subcarrier frequency and no pedestal (= 0 IRE blanking setup)) 
	 case EM_UDISCREEN_RESOLUTION_PAL_B:		 ///< Australia 
	 case EM_UDISCREEN_RESOLUTION_PAL_B1:	 ///< Hungary 
	 case EM_UDISCREEN_RESOLUTION_PAL_D:		 ///<  China ==EM_UDISCREEN_RESOLUTION_PAL 
	 case EM_UDISCREEN_RESOLUTION_PAL_D1:	 ///<  Poland 
	 case EM_UDISCREEN_RESOLUTION_PAL_G:		 ///< Europe. Same as EM_UDISCREEN_RESOLUTION_PAL. 
	 case EM_UDISCREEN_RESOLUTION_PAL_H:		 ///<  Europe 
	 case EM_UDISCREEN_RESOLUTION_PAL_K:			 ///< Europe 
	 case EM_UDISCREEN_RESOLUTION_PAL_I:		 ///<  U.K. 
	 case EM_UDISCREEN_RESOLUTION_576P: 	 ///<  Australian mode	 
		 pstRect->m_nHeight = 576;
		 break;
	 default:
		 enRet = CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED;
		 break;
	 }
 }
 else
 {
	 switch(eResolution)
	 {
	 case  EM_UDISCREEN_RESOLUTION_720P:	 ///< HD: EIA770.3 #1 P60 = SMPTE 296M #1 P60 
	 case EM_UDISCREEN_RESOLUTION_720P_24HZ:	///<  HD 720p 24Hz	 
	 case EM_UDISCREEN_RESOLUTION_720P_50HZ: ///< 7HD 20p 50Hz
		 pstRect->m_nWidth = 1280;
		 pstRect->m_nHeight = 720;
		 break;
	 case EM_UDISCREEN_RESOLUTION_1080I:		///< HD: EIA770.3 #3 I60 = SMPTE274M #4 I60 	 
	 case EM_UDISCREEN_RESOLUTION_1080I_50HZ: ///<	HD:  SMPTE 274M   I50 
	 case EM_UDISCREEN_RESOLUTION_1080P:	 ///<  HD 1080 Progressive, 60Hz 
	 case EM_UDISCREEN_RESOLUTION_1080P_24HZ:///<  HD 1080 Progressive, 24Hz 
	 case EM_UDISCREEN_RESOLUTION_1080P_25HZ:///<  HD 1080 Progressive, 25Hz 
	 case EM_UDISCREEN_RESOLUTION_1080P_30HZ:	 ///<  HD 1080 Progressive, 30Hz 
	 case EM_UDISCREEN_RESOLUTION_1080P_50HZ:///<  HD 1080 Progressive, 50Hz	 
		 pstRect->m_nWidth = 1920;
		 pstRect->m_nHeight = 1080;
		 break;
	 default:
		 enRet = CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED;
		 break;
	 }
 }
 CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"*eScreenDevice[%d]*eResolution[%d]***pstRect->m_nWidth[%d]**pstRect->m_nHeight[%d]\n",eScreenDevice,eResolution,pstRect->m_nWidth,pstRect->m_nHeight);
 return enRet;
}

/**
@brief  注册指定事件类型的回调函数

@param[in] nVideoIndex 视频解码器的索引
@param[in] fnVideoCallback 回调函数指针，详见CSUDIVIDEOCallback_F定义
@param[in] eEvent 回调函数类型，详见CSUDIVIDEOEventType_E
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 至少支持注册的回调函数个数为32个，最大不限
@note 不允许注册EM_UDIVIDEO_MAXEVENTTYPE类型的回调
@note 回调函数指针+回调函数类型+用户数据唯一标识一个回调,当两次注册它们完全一样时，第二次将返回CSUDIVIDEO_ERROR_CALLBACK_EXIST

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOAddCallback(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent,void * pvUserData )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief 删除指定事件类型的回调函数

@param[in] nVideoIndex 视频解码器的索引
@param[in] fnVideoCallback 回调函数指针，详见CSUDIVIDEOCallback_F定义
@param[in] eEvent 回调函数类型，详见CSUDIVIDEOEventType_E
@param[in] pvUserData 同CSUDIVIDEOAddCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIVIDEO_ERROR_CALLBACK_NOT_EXIST
@note 允许在回调函数中删除回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEODelCallback(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent, void * pvUserData )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief 控制VBI方式的teletext\close caption\Macrovision 功能开关

@param[in] nVideoIndex 视频解码器的索引
@param[in] eVideoVBIType VBI功能类型，可以组合控制
@param[in] bEnable 打开或关闭eVideoVBIType对应的功能
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 系统启动时所有功能默认关闭，只有调用本接口才能打开或关闭对应功能，设置一次永久生效
@note 当功能为EM_UDIVIDEO_VBI_TELTEXT时，需要同player接口CSUDIPLAYERSetStream配合使用,eg:   
@note 启动VBI teltext 功能
@note  sStreamInfo.m_eContentType=EM_UDI_CONTENT_TELTEXT;
@note  sStreamInfo.m_nPid=teltext_pid;
@note  CSUDIPLAYERSetStream(hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL);
@note  CSUDIVIDEOSetVBIEnable(nVideoIndex,EM_UDIVIDEO_VBI_TELTEXT, CSUDI_TRUE);
@note  其它功能因不需要设置PID,故只需调用CSUDIVIDEOSetVBIEnable接口

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetVBIEnable(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL bEnable)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取VBI各个功能teletext\close caption\Macrovision 开关状态

@param[in] nVideoIndex 视频解码器的索引
@param[in] eVideoVBIType VBI功能类型，每次只能查询一个功能
@param[out] pbEnable ,eVideoVBIType对应的功能开关状态
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetVBIEnable(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL* pbEnable)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取Video Decoder设备的总数

@param[out] pnVideoDecCount 系统中 存在Video Decoder设备的个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，这种情况下pnVideoDecCount的内容不确定

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetCount(int * pnVideoDecCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CNXT_STATUS Status = CNXT_STATUS_OK;	    
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s\n",__FUNCTION__);
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if(pnVideoDecCount == NULL)
		{
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		*pnVideoDecCount = CS_TRID_MAX_DEC;        
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s VideoDecCount[%d]\n",__FUNCTION__,*pnVideoDecCount);
	}while(0);        
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s\n",__FUNCTION__);
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Status);	
	return Retcode;
}

/**
@brief 获取指定Video Decoder设备的能力信息

无论空闲或者被占用
@param[in] nVideoIndex Video Decoder设备的索引
@param[out] psCapabilityInfo Video Decoder设备能力信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，这种情况下psCapabilityInfo结构的内容不确定

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CNXT_STATUS Status = CNXT_STATUS_OK;	    
	u_int32 indextemp = 0;    
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDIVIDEO_ERROR_INVALID_DEVICE_ID;
			break;
		}
		if(psCapabilityInfo == NULL)
		{
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		psCapabilityInfo->m_eStreamType[indextemp++] = EM_UDI_VID_STREAM_MPEG1;
		psCapabilityInfo->m_eStreamType[indextemp++] = EM_UDI_VID_STREAM_MPEG2;
		psCapabilityInfo->m_eStreamType[indextemp++] = EM_UDI_VID_STREAM_H264;	
		psCapabilityInfo->m_eStreamType[indextemp] = 0;	
		
	}while(0);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out of Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);    
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**@brief 视频截图(快照)接口，将当前正在显示的视频图像的部分(或全部)截取并输出到目标图像的指定区域。

@param[in] nVideoIndex  Video Decoder设备的索引
@param[in] psSrcRect 输入窗口大小，表示要截取当前视频图像的哪一部分，为CSUDI_NULL表示整个图像，注意不能超出当前视频图像大小。
@param[in] psDstRect 输出窗口大小，表示输出到hSurface的哪一部分，为CSUDI_NULL表示整个hSurface，注意不能超出hSurface大小。
@param[out] phSurface 视频截图将输出到hSurface的指定区域，该Surface是调用OSG接口创建的缓存或是直接获取显存；
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且hSurface结构的内容未定义。
@note 在截取I帧图片时，通过调用CSUDIVIDEOGetStreamStatus获取当前I帧信息，例如图像宽高信息。
@note 如果产品不需要视频快照功能或者硬件不支持，该接口可以不实现。此时需返回CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect, CSUDI_HANDLE* phSurface)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CNXT_GRAPHICS_OP             GraphicsOp;
	CNXT_RECT             ImageRect;
	CNXT_RECT_SCALE       DstImageRectScale;
	CNXT_IMAGE_PAGE_PROPERTIES    ImagePageProp;
	TM_Image_Surface             *pImageSurface;

	CNXT_XY xy;
	CNXT_IMAGE_CAPS         ImageCaps;
	CNXT_IMAGE_PAGE_PLANE   PagePlane;
	CNXT_IMAGE_HANDLE hOSDImage;
   
	  
	PIPE_VIDEO_OBJECT        *pVidObject=NULL;
	PIPE_VIDEO_ATTRIB         VideoAttrib;

	u_int8 uNumPlanesReturned;
	u_int32               uPixelAddress[IMAGE_MAX_PLANES]={0};
	u_int32  uByteWidth,uStride;
	CNXT_STATUS Status = CNXT_STATUS_OK;
         
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s  surface :%x src :%x dst :%x\n",__FUNCTION__,phSurface,psSrcRect,psDstRect);
    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDIVIDEO_ERROR_INVALID_DEVICE_ID;
			break;
		}
		if(phSurface == NULL)
		{
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		pVidObject = gTmPipeObject.hVideoObj[nVideoIndex];
		
		pImageSurface = (TM_Image_Surface*)(*phSurface);
		GraphicsOp.uColorUse = CNXT_GRAPHICS_COLOR_STORE_RESULT;
		GraphicsOp.ROP = (CNXT_ROP)CNXT_GRAPHICS_ROP_COPY;
		GraphicsOp.uBltControl = CNXT_GRAPHICS_OP_ROP | CNXT_GRAPHICS_OP_PRESERVE_ALPHA;
		GraphicsOp.uAlphaUse = CNXT_GRAPHICS_ALPHA_STORE_FIXED;
		GraphicsOp.uAlpha = 0xFF;
		Status = cnxt_image_get_page_properties(pImageSurface->pImageHandle, 0, &ImagePageProp);
		if(Status != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "Error get page properties of a source image. IMAGEStatus = %d.\n",Status);
			Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
			break;
		}

//		gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uWidth = 720;
//		gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uHeight = 576;
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"wisht :%d height :%d  widht :%d height %d\n",gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uWidth,gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uHeight,
		ImagePageProp.PageSetProp.uBitmapW,ImagePageProp.PageSetProp.uBitmapH);
		xy.nX = 0;
		xy.nY = 0;
		
		uByteWidth = ( gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uWidth  *  0x1000)  / 0x1000;
		if(((gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uWidth *0x1000) % 0x1000) > 0)
		{
			uByteWidth ++;
		}
		/* calculate stride */
		
		uStride = (uByteWidth + (128 - 1)) & (~(128 - 1));
		//        pPrvData = (PIPE_VP_SURFACE_PRV_DATA *)gTmVpDeviceObj.pSDVideoSurface[nVideoIndex]->hPrivHandle;
		pVidObject->get_attrib(pVidObject,&VideoAttrib);
		CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"pVidObject->get_attrib>>>hImageHandle[%d]uImageWidth[%d]uImageHeight[%d]\n",VideoAttrib.hImage,VideoAttrib.VideoCfg.uImageWidth,VideoAttrib.VideoCfg.uImageHeight);
		//[alfred chen]: todo here I need one video image handler    
		cnxt_image_get_pixel_address(VideoAttrib.hImage,&xy,0,2,&uNumPlanesReturned,(void *)&uPixelAddress);
		ImageCaps.ColorSpace = YCC_SD_BT470;
		ImageCaps.PixelMode = PIXEL_FORMAT_NV12; 
		ImageCaps.PalTable.ColorType      = YCC;
		ImageCaps.PalTable.uNumPalEntries = 0;
		ImageCaps.PalTable.pPalEntry      = NULL;
		ImageCaps.bIsContiguous           = TRUE;            
		ImageCaps.uOrgW       =  gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uWidth;
		ImageCaps.uOrgH       =  gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uHeight;
		ImageCaps.PoolId[0]               = CNXT_POOL_ID_DEFAULT_UNCACHED;
		ImageCaps.PoolId[1]               = CNXT_POOL_ID_DEFAULT_UNCACHED;
		ImageCaps.uNumPages = 1;
		ImageCaps.uMaxNumPages = 1;
		ImageCaps.bCached = FALSE;
		ImageCaps.pPage = NULL;    
		ImageCaps.uStride                 = uStride;    
		PagePlane.puPlanes[0]             =(void *) uPixelAddress[0];
		PagePlane.puPlanes[1] =(void *) (uPixelAddress[0] + ( ImageCaps.uStride *gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uHeight));
		PagePlane.puPlanes[2] = NULL;
		PagePlane.puPlanes[3] = NULL;
		ImageCaps.pPage                   = &PagePlane;
		
		Status = cnxt_image_open(&hOSDImage, &ImageCaps, NULL, NULL);
		if(Status != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"iamge open:%d at line :%d\n",Status,__LINE__);
			Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
			break;
		}
		if(psSrcRect == NULL)
		{
			ImageRect.nLeft   = 0;
			ImageRect.nRight  = gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uWidth;
			ImageRect.nTop    = 0;
			ImageRect.nBottom = (gTmPipeObject.TridVideoSubSystem[nVideoIndex].PictureInfo.Size.uHeight);
		}
		else
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"src not NULL x  :%d  width :%d  y :%xheight %d\n", psSrcRect->m_nX, psSrcRect->m_nWidth,
			psSrcRect->m_nY,psSrcRect->m_nHeight);
			ImageRect.nLeft   = psSrcRect->m_nX;
			ImageRect.nRight  = psSrcRect->m_nX + psSrcRect->m_nWidth;
			ImageRect.nTop    = psSrcRect->m_nY;
			ImageRect.nBottom = psSrcRect->m_nY + psSrcRect->m_nHeight; 
		}
		if(psDstRect == NULL)
		{
			DstImageRectScale.bRect = TRUE;
			DstImageRectScale.RectOrScale.Rect.nLeft   = 0;
			DstImageRectScale.RectOrScale.Rect.nRight  = ImagePageProp.PageSetProp.uBitmapW;
			DstImageRectScale.RectOrScale.Rect.nTop    = 0;
			DstImageRectScale.RectOrScale.Rect.nBottom = ImagePageProp.PageSetProp.uBitmapH;
		}
		else
		{
			DstImageRectScale.bRect = TRUE;
			DstImageRectScale.RectOrScale.Rect.nLeft   =  psDstRect->m_nX;
			DstImageRectScale.RectOrScale.Rect.nRight  =  psDstRect->m_nX + psDstRect->m_nWidth;
			DstImageRectScale.RectOrScale.Rect.nTop    =   psDstRect->m_nY;
			DstImageRectScale.RectOrScale.Rect.nBottom =  psDstRect->m_nY + psDstRect->m_nHeight; 
		}
		Status = cnxt_graphics_stretch_copy( gTridGraphicsHandle,hOSDImage, 0,
		&ImageRect,NULL,pImageSurface->pImageHandle, 0,  &DstImageRectScale, &GraphicsOp);
		
		if(Status != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d status :%x\n",__FUNCTION__,__LINE__,Status);
			Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
			break;
		}
		Status =  cnxt_graphics_cmd_done( gTridGraphicsHandle, FALSE, NULL );
		if( Status != CNXT_STATUS_OK )
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d status :%x\n",__FUNCTION__,__LINE__,Status);
			Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
			break;
		}
		Status = cnxt_image_close(hOSDImage);
		if(Status != CNXT_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"in function  :%s status :%d at line :%d\n",__FUNCTION__,Status,__LINE__);
			Retcode = CSUDIOSG_ERROR_UNKNOWN_ERROR;
			break;
		}
	}while(0);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"EXIT Function :%s  Retcode[%d]\n",__FUNCTION__,Retcode);     
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取解码器播放状态信息

如当前播放的视频大小等
@param[in]  nVideoIndex Video Decoder设备资源的索引
@param[out] psStreamStatus 捕获IFRAME信息 见CSUDIVIDStreamStatus_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psStreamStatus结构的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CNXT_STATUS Status = CNXT_STATUS_OK;	        
    PIPE_STATUS PStatus;
    static int32 tempcounter=0;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
    PIPE_VIDEO_ATTRIB videoAttr;
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		if(psStreamStatus == NULL)
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
        PStatus = gTmPipeObject.hVideoObj[nVideoIndex]->get_attrib(gTmPipeObject.hVideoObj[nVideoIndex],&videoAttr);
        if(PStatus != PIPE_STATUS_OK)                
        {
            Retcode = CSUDI_FAILURE;
            CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"Error :%s.%d Status[%d]\n",__FUNCTION__,__LINE__,PStatus);
            break;            
        }
        
		psStreamStatus->m_nSourceHight = videoAttr.PictureInfo.PictureSize.uHeight;
		psStreamStatus->m_nSourceWidth  =  videoAttr.PictureInfo.PictureSize.uWidth;
		psStreamStatus->m_nFrameRate = videoAttr.FrameRate;
		psStreamStatus->m_bInterlaced =  videoAttr.PictureInfo.bInterlaced;
        tempcounter++;
		psStreamStatus->m_dwFrameCount = 0xff+tempcounter;

		switch(videoAttr.VideoCfg.Format)
		{
            case PIPE_VIDEO_FORMAT_MPEG2:
                psStreamStatus->m_eVidStreamType = EM_UDI_VID_STREAM_MPEG2;
                break;
            case PIPE_VIDEO_FORMAT_H264:
                psStreamStatus->m_eVidStreamType = EM_UDI_VID_STREAM_H264;
                break;
            case PIPE_VIDEO_FORMAT_MPEG4PART2:
                psStreamStatus->m_eVidStreamType = EM_UDI_VID_STREAM_MPEG4;
                break;
            default:
                psStreamStatus->m_eVidStreamType = EM_UDI_VID_STREAM_UNKNOWN;
                break;
		}
		
	}while(0);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out of Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 显示/隐藏视频解码器的输出

@param[in] nVideoIndex  视频解码器的索引
@param[in] bIsShow 为CSUDI_TRUE则表示显示该窗口;为CSUDI_FALSE则表示隐藏该窗口
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值 
@note 系统初始化时应为显示状态

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOShow(int nVideoIndex, CSUDI_BOOL bIsShow)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int 	chain=0,surface_indx=0;    
	PIPE_STATUS PStatus;
	CNXT_STATUS Status = CNXT_STATUS_OK;
	PIPE_VP_SURFACE_ATTRIB sSDAttrib,sHDAttrib;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s bIsShow[%d]\n",__FUNCTION__,bIsShow);
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);  
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		chain = nVideoIndex; 
        /*Get surface index from player*/
        surface_indx = cs_tm_get_surface_indx(nVideoIndex);
		if((gTmVpDeviceObj.pHDVideoSurface[surface_indx] == NULL) ||
            (gTmVpDeviceObj.pSDVideoSurface[surface_indx] == NULL))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
		
		// 2011-01-14 TerenceZhang Masked begin
		cnxt_kal_memset(&sHDAttrib,0,sizeof(sHDAttrib));	   	
		PStatus = gTmVpDeviceObj.pHDVideoSurface[surface_indx]->get_attrib(gTmVpDeviceObj.pHDVideoSurface[surface_indx], 
		                                                                  &sHDAttrib);
		if (PStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}

		cnxt_kal_memset(&sSDAttrib,0,sizeof(sSDAttrib));	   	
		PStatus = gTmVpDeviceObj.pSDVideoSurface[surface_indx]->get_attrib(gTmVpDeviceObj.pSDVideoSurface[surface_indx], 
		                                                                  &sSDAttrib);
		if (PStatus != PIPE_STATUS_OK)
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
        
		if(bIsShow == 1)
		{
			if((sHDAttrib.bSurfaceEnabled == FALSE)&&(sHDAttrib.Input.Type!=PIPE_VP_SURFACE_INPUT_TYPE_NONE))
			{
				PStatus = gTmVpDeviceObj.pHDVideoSurface[surface_indx]->surface_enable(gTmVpDeviceObj.pHDVideoSurface[surface_indx]);
				if(PStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					Retcode = CSUDI_FAILURE;
					break;
				}
			}

			if((sSDAttrib.bSurfaceEnabled == FALSE)&&(sSDAttrib.Input.Type!=PIPE_VP_SURFACE_INPUT_TYPE_NONE))
			{
				PStatus = gTmVpDeviceObj.pSDVideoSurface[surface_indx]->surface_enable(gTmVpDeviceObj.pSDVideoSurface[surface_indx]);
				if(PStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					Retcode = CSUDI_FAILURE;
					break;
				}
			}
		}
		else
		{
			if((sHDAttrib.bSurfaceEnabled == TRUE)&&(sHDAttrib.Input.Type!=PIPE_VP_SURFACE_INPUT_TYPE_NONE))
			{
				PStatus = gTmVpDeviceObj.pHDVideoSurface[surface_indx]->surface_disable(gTmVpDeviceObj.pHDVideoSurface[surface_indx]);
				if(PStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					Retcode = CSUDI_FAILURE;
					break;
				}
			}

			if((sSDAttrib.bSurfaceEnabled == TRUE)&&(sSDAttrib.Input.Type!=PIPE_VP_SURFACE_INPUT_TYPE_NONE))
			{
				PStatus = gTmVpDeviceObj.pSDVideoSurface[surface_indx]->surface_disable(gTmVpDeviceObj.pSDVideoSurface[surface_indx]);
				if(PStatus != PIPE_STATUS_OK)
				{
					CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Into Function :%s line[%d] error\n",__FUNCTION__,__LINE__);
					Retcode = CSUDI_FAILURE;
					break;
				}
			}   	
		}
	}while(0);
	// 2011-01-14 TerenceZhang Masked end

    gTmVpDeviceObj.bShowVideo[surface_indx] = bIsShow;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out of Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 设置指定视频输出的屏幕宽高比

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 指定视频输出屏幕
@param[in] eAspectRatio 视频输出接口的宽高比，如4:3 或 16:9
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note auto表示根据节目的宽高比自动设置屏幕宽高比，也即将宽高比效果交给底层驱动处理
@note 设置指定视频输出接口输出画面的宽高比，目前只支持16:9/4:3/auto等模式。
具体效果依赖于所播放的节目的宽高比以及指定视频输出接口的适应模式(MatchMethod)等因素
@note 此接口设置的显示效果只在视频全屏显示时有效，在视频小窗口显示时不起作用，即无论此接口如何设置，视频小窗口显示时始终是填满小窗口的

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int 	chain=0;    
    int surface_indx;
	PIPE_STATUS PStatus;    
	PIPE_VP_DEVICE_AR_MODE DeviceAspectRatio=0;    
	CNXT_STATUS Status = CNXT_STATUS_OK;
	//PIPE_VP_SURFACE_AR_MODE AspectRatio;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		chain = nVideoIndex; 
        
		surface_indx = cs_tm_get_surface_indx(nVideoIndex);


		if((gTmVpDeviceObj.pHDDevice == NULL) ||(gTmVpDeviceObj.pSDDevice == NULL))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
		
		if(!((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD || (eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD ))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"eScreenDevice[%d]Aspect ratio[%d]index:[%d]\n",eScreenDevice,eAspectRatio,nVideoIndex);  
		if((eAspectRatio <= 0) || (eAspectRatio > EM_UDIVIDEO_ASPECT_RATIO_AUTO ))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}      
        
		switch(eAspectRatio)
		{
		case EM_UDIVIDEO_ASPECT_RATIO_4_3:
			DeviceAspectRatio = PIPE_VP_DEVICE_AR_4_3;
			break;
		case EM_UDIVIDEO_ASPECT_RATIO_16_9:
			DeviceAspectRatio = PIPE_VP_DEVICE_AR_16_9;
			break;
		case EM_UDIVIDEO_ASPECT_RATIO_AUTO:
			DeviceAspectRatio = PIPE_VP_DEVICE_AR_1_1;
            break;
		case EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN:
		default:
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
			
		}
		
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD)
		{
			gTmPipeObject.TridCoshipSetup[chain].ARHD = eAspectRatio;
            if((eAspectRatio != EM_UDIVIDEO_ASPECT_RATIO_AUTO)&&(eAspectRatio != EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN))
            {
                
                PStatus = gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pHDDevice,DeviceAspectRatio);
			    if(PStatus != PIPE_STATUS_OK)
			    {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
				    Retcode = CSUDI_FAILURE;
			    }
                if(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_4_3)
                {
                    if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {

                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        gTmVpDeviceObj.pHDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_NONE);
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 100;
                        TempRect.m_nY = 0;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 200;
                        TempRect.m_nHeight = TempRect.m_nHeight;
                        TM_internal_SetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success, do nothing line[%d]\n",__LINE__);
                    }
                }
                else if((gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_RAW)||(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_16_9))
                {
                    if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success, do nothing line[%d]\n",__LINE__);
                    }
                }
                
            }
            else
            {
                //auto mode
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"CNXT_AR_1_1[%d]CNXT_AR_20_9[%d]pic ar[%d]\n",CNXT_AR_1_1,CNXT_AR_20_9,gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio);
                if(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_RAW)
                {
                    PStatus = gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pHDDevice,CNXT_AR_16_9);
                }
                else
                {
                    PStatus = gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pHDDevice,gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio);
                }
			    if(PStatus != PIPE_STATUS_OK)
			    {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
				    Retcode = CSUDI_FAILURE;
			    }
                TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,NULL);
                TM_internal_SetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,NULL);

                
            }
		}
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD)
		{            
			gTmPipeObject.TridCoshipSetup[chain].ARSD = eAspectRatio;
            if((eAspectRatio != EM_UDIVIDEO_ASPECT_RATIO_AUTO)&&(eAspectRatio != EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN))
            {
                PStatus = gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,DeviceAspectRatio);
                if(PStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
                    Retcode = CSUDI_FAILURE;
                }
                if(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_4_3)
                {
                    if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_LETTERBOX);
                        gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_16_9);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_PANSCAN);
                        gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_16_9);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                        
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_PANSCAN);
                        gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_16_9);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                        
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success, do nothing[%d]\n",__LINE__);
                    }
                }
                else if((gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_RAW)||(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_16_9))
                {
                    if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_PANSCAN);
                        gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d][%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {

                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success, do nothing[%d]\n",__LINE__);
                    }
                }
                
            }
            else
            {
                //auto mode                
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"CNXT_AR_1_1[%d]CNXT_AR_20_9[%d]system AspectRatio[%d]\n",CNXT_AR_1_1,CNXT_AR_20_9,gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio);
                if(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_RAW)
                {
                    PStatus = gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,CNXT_AR_16_9);
                }
                else
                {
                    PStatus = gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio);                    
                }
                if(PStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
                    Retcode = CSUDI_FAILURE;
                }
                TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,NULL);
                TM_internal_SetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,NULL);
            }
		}
		else
		{
			
		}
	}while(0);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out of Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief获取指定视频输出的屏幕宽高比

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 指定视频输出屏幕
@param[out] peAspectRatio 视频输出接口的宽高比，如4:3 或 16:9
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 本接口用于获取当前设置的屏幕宽高比状态，如果设置的是AUTO，则返回AUTO，
流的情况由CSUDIVIDEOGetStreamStatus接口获得

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetAspectRatio(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E *peAspectRatio)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CNXT_STATUS Status = CNXT_STATUS_OK;       
	//PIPE_STATUS Status;            
	int     chain=0;    
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		chain = nVideoIndex; 
		if((gTmVpDeviceObj.pHDDevice == NULL) ||(gTmVpDeviceObj.pSDDevice == NULL))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
		
		if(!((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD || (eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD ))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}                          
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD)
		{
			*peAspectRatio = gTmPipeObject.TridCoshipSetup[chain].ARHD;
		}
		else if((eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD)
		{
			*peAspectRatio = gTmPipeObject.TridCoshipSetup[chain].ARSD;
		}
		else
		{
			
		}
	}while(0);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out of Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief设置指定视频输出接口的适应模式

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 指定视频输出屏幕
@param[in] eMatchMethod 指定视频输出端口的适应模式，如PAN_SCAN,LETTER_BOX,等
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置指定视频输出端口的屏幕适应模式，
具体效果依赖于所播放的节目的宽高比以及指定视频输出接口的画面比(AspectRatio)等因素，请参考下图：
@image HTML VIDAspectRatioType.png
@note 此接口设置的显示效果只在视频全屏显示时有效，在视频小窗口显示时不起作用，即无论此接口如何设置，视频小窗口显示时始终是填满小窗口的

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CNXT_STATUS Status = CNXT_STATUS_OK;       
	//    PIPE_STATUS Status;            
	int     chain=0, surface_indx;    
	PIPE_VP_SURFACE_AR_MODE SurfaceAspectRatio=0;
	PIPE_STATUS PStatus;    
    
	PIPE_VP_DEVICE_AR_MODE DeviceAspectRatio=0;    
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		chain = nVideoIndex; 
        /*Get surface index from player*/
        surface_indx = cs_tm_get_surface_indx(nVideoIndex);
		if((gTmVpDeviceObj.pHDVideoSurface[surface_indx] == NULL) ||(gTmVpDeviceObj.pSDVideoSurface[surface_indx] == NULL))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
		
		if(!((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD || (eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD ))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}      

		if((eMatchMethod < 0) || (eMatchMethod >= EM_UDIVIDEO_MATCH_METHOD_NUM ))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}      
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"eScreenDevice[%d]eMatchMethod[%d]index:[%d]\n",eScreenDevice,eMatchMethod,nVideoIndex);  
        
		switch(eMatchMethod)
		{
            case EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX:
			    SurfaceAspectRatio = PIPE_VP_SURFACE_AR_LETTERBOX;
			    break;
		    case EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN:
			    SurfaceAspectRatio = PIPE_VP_SURFACE_AR_PANSCAN;
			    break;
		    case EM_UDIVIDEO_MATCH_METHOD_COMBINED:
		    case EM_UDIVIDEO_MATCH_METHOD_IGNORE:
			SurfaceAspectRatio = PIPE_VP_SURFACE_AR_NONE;
			break;
		    default:
			    break;
			
		}
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD)
		{
			gTmPipeObject.TridCoshipSetup[chain].ARTypeHD = eMatchMethod;
			gTmVpDeviceObj.pHDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pHDVideoSurface[surface_indx],SurfaceAspectRatio);
            if((gTmPipeObject.TridCoshipSetup[chain].ARHD != EM_UDIVIDEO_ASPECT_RATIO_AUTO)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD != EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN))
            {
                
                switch(gTmPipeObject.TridCoshipSetup[chain].ARHD)
                {
                case EM_UDIVIDEO_ASPECT_RATIO_4_3:
                    DeviceAspectRatio = PIPE_VP_DEVICE_AR_4_3;
                    break;
                case EM_UDIVIDEO_ASPECT_RATIO_16_9:
                    DeviceAspectRatio = PIPE_VP_DEVICE_AR_16_9;
                    break;
                case EM_UDIVIDEO_ASPECT_RATIO_AUTO:
                    DeviceAspectRatio = PIPE_VP_DEVICE_AR_1_1;
                    break;
                case EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN:
                default:
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
                    Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
                    break;
                    
                }
                PStatus = gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pHDDevice,DeviceAspectRatio);
			    if(PStatus != PIPE_STATUS_OK)
			    {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
				    Retcode = CSUDI_FAILURE;
			    }
                if(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_4_3)
                {
                    if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {

                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        gTmVpDeviceObj.pHDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_NONE);
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 100;
                        TempRect.m_nY = 0;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 200;
                        TempRect.m_nHeight = TempRect.m_nHeight;
                        TM_internal_SetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success, do nothing line[%d]\n",__LINE__);
                    }
                }
                else if((gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_RAW)||(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_16_9))
                {
                    if((gTmPipeObject.TridCoshipSetup[chain].ARTypeHD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARHD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[0].uHDResolution,&TempRect);
                        TempRect.m_nX = 50;
                        TempRect.m_nWidth  = TempRect.m_nWidth- 100;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed , need to adjust line[%d]\n",__LINE__);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success, do nothing line[%d]\n",__LINE__);
                    }
                }
                
            }
            else
            {
                //auto mode
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"CNXT_AR_1_1[%d]CNXT_AR_20_9[%d]pic ar[%d]\n",CNXT_AR_1_1,CNXT_AR_20_9,gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio);
                if(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_RAW)
                {
                    PStatus = gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pHDDevice,CNXT_AR_16_9);
                }
                else
                {
                    PStatus = gTmVpDeviceObj.pHDDevice->set_aspect_ratio(gTmVpDeviceObj.pHDDevice,gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio);
                }
			    if(PStatus != PIPE_STATUS_OK)
			    {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
				    Retcode = CSUDI_FAILURE;
			    }
                TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_HD,NULL);
                TM_internal_SetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,NULL);

                
            }
            
           
			
		}
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD)
		{
			gTmPipeObject.TridCoshipSetup[chain].ARTypeSD = eMatchMethod;
			gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],SurfaceAspectRatio);
            if((gTmPipeObject.TridCoshipSetup[chain].ARSD != EM_UDIVIDEO_ASPECT_RATIO_AUTO)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD != EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN))
            {
                
                switch(gTmPipeObject.TridCoshipSetup[chain].ARSD)
                {
                case EM_UDIVIDEO_ASPECT_RATIO_4_3:
                    DeviceAspectRatio = PIPE_VP_DEVICE_AR_4_3;
                    break;
                case EM_UDIVIDEO_ASPECT_RATIO_16_9:
                    DeviceAspectRatio = PIPE_VP_DEVICE_AR_16_9;
                    break;
                case EM_UDIVIDEO_ASPECT_RATIO_AUTO:
                    DeviceAspectRatio = PIPE_VP_DEVICE_AR_1_1;
                    break;
                case EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN:
                default:
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
                    Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
                    break;
                    
                }
                PStatus = gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,DeviceAspectRatio);
                if(PStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
                    Retcode = CSUDI_FAILURE;
                }
                if(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_4_3)
                {
                    if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_LETTERBOX);
                        gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_16_9);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_PANSCAN);
                        gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_16_9);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                        
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_PANSCAN);
                        gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_16_9);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                        
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success, do nothing[%d]\n",__LINE__);
                    }
                }
                else if((gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_RAW)||(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_16_9))
                {
                    if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_aspect_ratio_mode(gTmVpDeviceObj.pSDVideoSurface[surface_indx],PIPE_VP_SURFACE_AR_PANSCAN);
                        gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d][%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_COMBINED)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {

                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_4_3))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)&&(gTmPipeObject.TridCoshipSetup[chain].ARSD == EM_UDIVIDEO_ASPECT_RATIO_16_9))
                    {
                        CSUDIWinRect_S TempRect;
                        CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                        TempRect.m_nX = 0;
                        TempRect.m_nY = 50;
                        //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                        //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                        TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    }
                    else
                    {
                        CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"success, do nothing[%d]\n",__LINE__);
                    }
                }
                
            }
            else
            {
                //auto mode                
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"CNXT_AR_1_1[%d]CNXT_AR_20_9[%d]system AspectRatio[%d]\n",CNXT_AR_1_1,CNXT_AR_20_9,gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio);
                if(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio == CNXT_AR_RAW)
                {
                    PStatus = gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,CNXT_AR_16_9);
                }
                else
                {
                    PStatus = gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,gTmPipeObject.TridVideoSubSystem[0].PictureInfo.AspectRatio);                    
                }
                if(PStatus != PIPE_STATUS_OK)
                {
                    CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
                    Retcode = CSUDI_FAILURE;
                }
                if(gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_IGNORE)
                {
                    TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,NULL);
                    TM_internal_SetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,NULL);
                }
                if((gTmPipeObject.TridCoshipSetup[chain].ARTypeSD == EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)&&(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth > 720)
                    &&(gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight > 576))
                {
                    CSUDIWinRect_S TempRect;
                    PStatus = gTmVpDeviceObj.pSDDevice->set_aspect_ratio(gTmVpDeviceObj.pSDDevice,PIPE_VP_DEVICE_AR_4_3);
                    if(PStatus != PIPE_STATUS_OK)
                    {
                        CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d \n",__FUNCTION__,__LINE__);
                        Retcode = CSUDI_FAILURE;
                    }
                    CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&TempRect);
                    TempRect.m_nX = 0;
                    TempRect.m_nY = 0;
                    //TempRect.m_nWidth = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uWidth;
                    //TempRect.m_nHeight = gTmPipeObject.TridVideoSubSystem[0].PictureInfo.Size.uHeight;
                    TM_internal_SetWinClip(0,EM_UDI_VOUT_DEVICE_SD,&TempRect);                        
                    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"fixed...line[%d]\n",__LINE__);
                    
                }
            }

		}
		else
		{
			
		}
        
	}while(0);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Out of Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief获取指定视频输出接口的适应模式

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 指定视频输出屏幕
@param[out] peMatchMethod 指定视频输出端口的适应模式，如PAN_SCAN,LETTER_BOX,等
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetMatchMethod(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E *peMatchMethod)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	//    PIPE_STATUS Status;            
	int     chain=0,surface_indx;    
	CNXT_STATUS Status = CNXT_STATUS_OK;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
	Status = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		chain = nVideoIndex; 
        /*Get surface index from player*/
        surface_indx = cs_tm_get_surface_indx(nVideoIndex);
		if((gTmVpDeviceObj.pHDVideoSurface[surface_indx] == NULL) ||(gTmVpDeviceObj.pSDVideoSurface[surface_indx] == NULL))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
		
		if(!((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD || (eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD ))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}                          
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD)
		{
			*peMatchMethod = gTmPipeObject.TridCoshipSetup[chain].ARTypeHD;
		}
		else if((eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD)
		{
			*peMatchMethod = gTmPipeObject.TridCoshipSetup[chain].ARTypeSD;
		}
		else
		{
			
		}
	}while(0);
	Status = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 设置停止视频的模式

即节目切换时，视频切换效果，如静帧、黑屏
@param[in] nVideoIndex  Video Decoder设备资源的索引
@param[in] eStopMode 停止模式，详情请参考CSUDIVIDEOStopMode_E。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int     chain=0;    
	CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
	Cnxt_Retcode = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
        
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
        if((eStopMode>EM_UDIVIDEO_STOPMODE_FREEZE)||(eStopMode<EM_UDIVIDEO_STOPMODE_BLACK))
        {
			Retcode = CSUDI_FAILURE;
			break;            
        }
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"nVideoIndex[%d] eStopMode[%d]\n",nVideoIndex,eStopMode);
		chain = nVideoIndex; 
		gTmPipeObject.TridCoshipSetup[chain].eStopMode = eStopMode;
	}while(0);
	
	Cnxt_Retcode = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief获取停止视频的模式

即节目切换时，视频切换效果，如静帧、黑屏
@param[in] nVideoIndex  Video Decoder设备资源的索引
@param[out] peStopMode 停止模式，详情请参考CSUDIVIDEOStopMode_E。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E *peStopMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int     chain=0;    
	CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
	Cnxt_Retcode = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		chain = nVideoIndex; 
		*peStopMode = gTmPipeObject.TridCoshipSetup[chain].eStopMode;

	}while(0);
	
	Cnxt_Retcode = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 设置指定视频解码输出的窗口大小，实现缩放功能

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 设置在哪个屏幕上的视频窗口大小
@param[in] pstRect 窗口的大小以及窗口位置，参看CSUDIWinRect_S结构；如果pstRect为NULL，则显示为全屏。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note pstRect为相对与当前视频分辨率的矩形,该信息可以从CSUDISCREENGetResolution接口获取
@note 不考虑裁剪矩形为负数的情况

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int     chain=0, surface_indx=0;
	CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s eScreenDevice[%d]\n",__FUNCTION__,eScreenDevice);
	Cnxt_Retcode = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		chain = nVideoIndex; 
        /*Get surface index from player*/
        surface_indx = cs_tm_get_surface_indx(nVideoIndex);
		if((gTmVpDeviceObj.pHDVideoSurface[surface_indx]== NULL) ||
            (gTmVpDeviceObj.pHDVideoSurface[surface_indx]== NULL))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
		if(pstRect != NULL)
		{
			if((pstRect->m_nWidth <= 0 ) || (pstRect->m_nHeight <= 0))
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error in %s at line :%d\n",__FUNCTION__,__LINE__);
				Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
				break;
			}
			if (eScreenDevice & EM_UDI_VOUT_DEVICE_HD)
			{
				gTmPipeObject.TridVideoSubSystem[nVideoIndex].bVidFullScreenHD = FALSE;
			}
			if (eScreenDevice & EM_UDI_VOUT_DEVICE_SD)
			{
				gTmPipeObject.TridVideoSubSystem[nVideoIndex].bVidFullScreenSD = FALSE;
			}
			//printf("pstRect>>>>m_nX[%d]m_nY[%d]m_nWidth[%d]m_nHeight[%d]\n",pstRect->m_nX,pstRect->m_nY,pstRect->m_nWidth,pstRect->m_nHeight);
		}
		else
		{
			if (eScreenDevice & EM_UDI_VOUT_DEVICE_HD)
			{
				gTmPipeObject.TridVideoSubSystem[nVideoIndex].bVidFullScreenHD = TRUE;
			}
			if (eScreenDevice & EM_UDI_VOUT_DEVICE_SD)
			{
				gTmPipeObject.TridVideoSubSystem[nVideoIndex].bVidFullScreenSD = TRUE;
			}
			//printf("pstRect is NULL\n");
		}
		
		
		if(!((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD || (eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD ))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD)
		{
			PIPE_VP_SURFACE_POSITION hdPosition;
			CSUDIWinRect_S hdSrcRect;
			memset(&hdPosition, 0,sizeof(PIPE_VP_SURFACE_POSITION));
			memset(&hdSrcRect,0,sizeof(CSUDIWinRect_S));            
			
			if(pstRect != NULL)
			{
				gTmPipeObject.TridVideoSubSystem[chain].bVidFullScreenHD = FALSE;
				gTmPipeObject.TridVideoSubSystem[chain].RectHD = *pstRect;
				CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[chain].uHDResolution,&hdSrcRect);
				if(gTmPipeObject.TridVideoSubSystem[chain].bVidFullImageHD == TRUE)
				{
					hdPosition.uFlags |= PIPE_VP_SURFACE_SRC_FULL_IMAGE;
				}
				else
				{                    
					hdPosition.SrcRectOrScale.SrcRect.nX = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD.m_nX*0x4000)/hdSrcRect.m_nWidth;
					hdPosition.SrcRectOrScale.SrcRect.nY= (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD.m_nY*0x4000)/hdSrcRect.m_nHeight;
					hdPosition.SrcRectOrScale.SrcRect.nSizeX = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD.m_nWidth*0x4000)/hdSrcRect.m_nWidth;
					hdPosition.SrcRectOrScale.SrcRect.nSizeY = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD.m_nHeight*0x4000)/hdSrcRect.m_nHeight;
				}
				hdPosition.DstRect.nX = pstRect->m_nX;
				hdPosition.DstRect.nY= pstRect->m_nY;
				hdPosition.DstRect.nSizeX= pstRect->m_nWidth;
				hdPosition.DstRect.nSizeY = pstRect->m_nHeight;
			}
			else
			{
				CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[chain].uHDResolution,&hdSrcRect);
				if(gTmPipeObject.TridVideoSubSystem[chain].bVidFullImageHD == TRUE)
				{
					hdPosition.uFlags |= PIPE_VP_SURFACE_SRC_FULL_IMAGE;
				}
				else
				{                    
					hdPosition.SrcRectOrScale.SrcRect.nX = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD.m_nX*0x4000)/hdSrcRect.m_nWidth;
					hdPosition.SrcRectOrScale.SrcRect.nY= (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD.m_nY*0x4000)/hdSrcRect.m_nHeight;
					hdPosition.SrcRectOrScale.SrcRect.nSizeX = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD.m_nWidth*0x4000)/hdSrcRect.m_nWidth;
					hdPosition.SrcRectOrScale.SrcRect.nSizeY = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD.m_nHeight*0x4000)/hdSrcRect.m_nHeight;
				}
				gTmPipeObject.TridVideoSubSystem[chain].bVidFullScreenHD = TRUE;
				hdPosition.uFlags |= PIPE_VP_SURFACE_DEST_FULL_SCREEN;
			}
			
			gTmVpDeviceObj.pHDVideoSurface[surface_indx]->set_position(gTmVpDeviceObj.pHDVideoSurface[surface_indx],&hdPosition);
		}
		else if((eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD)
		{
			PIPE_VP_SURFACE_POSITION sdPosition;            
			CSUDIWinRect_S sdSrcRect;
			memset(&sdPosition, 0,sizeof(PIPE_VP_SURFACE_POSITION));            
			memset(&sdSrcRect,0,sizeof(CSUDIWinRect_S));            
			
			if(pstRect != NULL)
			{
				
				gTmPipeObject.TridVideoSubSystem[chain].bVidFullScreenSD = FALSE;
				gTmPipeObject.TridVideoSubSystem[chain].RectSD = *pstRect;
				CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&sdSrcRect);                
				if(gTmPipeObject.TridVideoSubSystem[chain].bVidFullImageSD == TRUE)
				{
					sdPosition.uFlags |= PIPE_VP_SURFACE_SRC_FULL_IMAGE;
				}
				else
				{
					sdPosition.SrcRectOrScale.SrcRect.nX = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD.m_nX*0x4000)/sdSrcRect.m_nWidth;
					sdPosition.SrcRectOrScale.SrcRect.nY= (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD.m_nY*0x4000)/sdSrcRect.m_nHeight;
					sdPosition.SrcRectOrScale.SrcRect.nSizeX = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD.m_nWidth*0x4000)/sdSrcRect.m_nWidth;
					sdPosition.SrcRectOrScale.SrcRect.nSizeY = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD.m_nHeight*0x4000)/sdSrcRect.m_nHeight;
				}
				sdPosition.DstRect.nX = pstRect->m_nX;
				sdPosition.DstRect.nY = pstRect->m_nY;
				sdPosition.DstRect.nSizeX = pstRect->m_nWidth;
				sdPosition.DstRect.nSizeY = pstRect->m_nHeight;

			}
			else
			{
				CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[chain].uSDResolution,&sdSrcRect);                
				if(gTmPipeObject.TridVideoSubSystem[chain].bVidFullImageSD == TRUE)
				{
					sdPosition.uFlags |= PIPE_VP_SURFACE_SRC_FULL_IMAGE;
				}
				else
				{
					sdPosition.SrcRectOrScale.SrcRect.nX = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD.m_nX*0x4000)/sdSrcRect.m_nWidth;
					sdPosition.SrcRectOrScale.SrcRect.nY= (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD.m_nY*0x4000)/sdSrcRect.m_nHeight;
					sdPosition.SrcRectOrScale.SrcRect.nSizeX = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD.m_nWidth*0x4000)/sdSrcRect.m_nWidth;
					sdPosition.SrcRectOrScale.SrcRect.nSizeY = (gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD.m_nHeight*0x4000)/sdSrcRect.m_nHeight;
				}
				gTmPipeObject.TridVideoSubSystem[chain].bVidFullScreenSD = TRUE;
				sdPosition.uFlags |= PIPE_VP_SURFACE_DEST_FULL_SCREEN;
				
			}
			CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"sdPosition.DstRect>>>>nX[%d]nY[%d]nSizeX[%d]nSizeY[%d]\n",sdPosition.DstRect.nX,\
																				   sdPosition.DstRect.nY,\ 
																				   sdPosition.DstRect.nSizeX,\
																				   sdPosition.DstRect.nSizeY); 
			
			CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"sdPosition.SrcRectOrScale.SrcRec>>>>nX[%d]nY[%d]nSizeX[%d]nSizeY[%d]\n",sdPosition.SrcRectOrScale.SrcRect.nX, \
																									sdPosition.SrcRectOrScale.SrcRect.nY, \
																									sdPosition.SrcRectOrScale.SrcRect.nSizeX,\
																									sdPosition.SrcRectOrScale.SrcRect.nSizeY);            
			gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_position(gTmVpDeviceObj.pSDVideoSurface[surface_indx],&sdPosition);            
		}
		else
		{
			
		}
	}while(0);
	
	Cnxt_Retcode = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取指定视频解码输出的窗口大小

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 设置在哪个屏幕上的视频窗口大小
@param[out] pstRect 窗口的大小以及窗口位置，参看CSUDIWinRect_S结构；
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * pstRect )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;       
	int     chain=0,surface_indx=0;
	CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s\n",__FUNCTION__);
	Cnxt_Retcode = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
		chain = nVideoIndex; 
        /*Get surface index from player*/
        surface_indx = cs_tm_get_surface_indx(nVideoIndex);
		if((gTmVpDeviceObj.pHDVideoSurface[surface_indx]== NULL) ||
            (gTmVpDeviceObj.pHDVideoSurface[surface_indx]== NULL))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
		if (pstRect == CSUDI_NULL)
		{
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		if(!((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD || (eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD ))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Error :%s line: %d error\n",__FUNCTION__,__LINE__);
			Retcode = CSUDI_FAILURE;
			break;
		}
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD)
		{
			if(gTmPipeObject.TridVideoSubSystem[nVideoIndex].bVidFullScreenHD == TRUE)
			{
				CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,gTmPipeObject.TmDisplaySubSys[nVideoIndex].uHDResolution,pstRect);                
			}
			else
			{
				*pstRect = gTmPipeObject.TridVideoSubSystem[chain].RectHD;
			}
			CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"HD->pstRect>>>>m_nX[%d]m_nY[%d]m_nWidth[%d]m_nHeight[%d]\n",pstRect->m_nX,pstRect->m_nY,pstRect->m_nWidth,pstRect->m_nHeight);
		}
		else if((eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD)
		{
			if(gTmPipeObject.TridVideoSubSystem[nVideoIndex].bVidFullScreenSD == TRUE)
			{
				CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,gTmPipeObject.TmDisplaySubSys[nVideoIndex].uSDResolution,pstRect);                            
			}
			else
			{
				*pstRect = gTmPipeObject.TridVideoSubSystem[chain].RectSD;
			}
			CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"SD->pstRect>>>>m_nX[%d]m_nY[%d]m_nWidth[%d]m_nHeight[%d]\n",pstRect->m_nX,pstRect->m_nY,pstRect->m_nWidth,pstRect->m_nHeight);
		}
		else
		{
			
		}
		
		
	}while(0);
	
	Cnxt_Retcode = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 设置窗口剪切位置和尺寸

设置后整个视频窗口仅显示设置区域的视频，用于实现局部放大缩小等操作，
即设置后将制定区域在整个Screen上显示(如果没有设置过CSUDIVIDEOSetWindowSize则为全屏显示)
@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 设置在哪个屏幕上的视频剪切区域
@param[in] psRect 指定剪切的区域，详见CSUDIWinRect_S定义。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 如果psRect为CSUDI_NULL，则表示全屏显示
@note pstRect为相对与当前视频分辨率的矩形,该信息可以从CSUDISCREENGetResolution接口获取

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int     chain=0,surface_indx=0;
//	CNXT_STATUS               Status;
//	CNXT_DRM_RGN_HANDLE hRgnHandle[2];
//	u_int32 uLoop,
    u_int32 uStandard=0;
//	uint16 setCounter=0;
//	uint16 i;
//	CNXT_RECT_SCALE		stSrcScale;
	CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
	PIPE_VP_SURFACE_POSITION hdPosition;
	PIPE_VP_SURFACE_POSITION sdPosition;
	Cnxt_Retcode = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);   
	
	memset(&sdPosition, 0,sizeof(PIPE_VP_SURFACE_POSITION));
	memset(&hdPosition, 0,sizeof(PIPE_VP_SURFACE_POSITION));
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s eScreenDevice[%d]\n",__FUNCTION__,eScreenDevice);
	do
	{
        
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDI_FAILURE;
			break;
		}
        /*Get surface index from player*/
        surface_indx = cs_tm_get_surface_indx(nVideoIndex);
		if(!((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD || (eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD ))
		{
			Retcode =  CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		if((psRect != NULL)&&((psRect->m_nX<0)||(psRect->m_nY<0)||(psRect->m_nWidth<0)||(psRect->m_nHeight<0)))
		{
			Retcode =  CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;            
		}            
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_SD) == EM_UDI_VOUT_DEVICE_SD)
		{
			uStandard =   gTmPipeObject.TmDisplaySubSys[chain].uSDVidStandard; 
			if(psRect != NULL)
			{
				gTmPipeObject.TridVideoSubSystem[chain].bVidFullImageSD = FALSE;
				sdPosition.SrcRectOrScale.SrcRect.nX = (psRect->m_nX*0x4000)/720;
				sdPosition.SrcRectOrScale.SrcRect.nY= (psRect->m_nY*0x4000)/576;
				sdPosition.SrcRectOrScale.SrcRect.nSizeX = (psRect->m_nWidth*0x4000)/720;
				sdPosition.SrcRectOrScale.SrcRect.nSizeY = (psRect->m_nHeight*0x4000)/576;
				CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"SD->psRect->m_nX[%d]m_nY[%d]m_nWidth[%d]m_nHeight[%d]\n",psRect->m_nX,psRect->m_nY,psRect->m_nWidth,psRect->m_nHeight);
				if(gTmPipeObject.TridVideoSubSystem[chain].bVidFullScreenSD == TRUE)
				{
					sdPosition.uFlags |= PIPE_VP_SURFACE_DEST_FULL_SCREEN;
				}
				else
				{
					sdPosition.DstRect.nX = gTmPipeObject.TridVideoSubSystem[chain].RectSD.m_nX;
					sdPosition.DstRect.nY = gTmPipeObject.TridVideoSubSystem[chain].RectSD.m_nY;
					sdPosition.DstRect.nSizeX = gTmPipeObject.TridVideoSubSystem[chain].RectSD.m_nWidth;
					sdPosition.DstRect.nSizeY = gTmPipeObject.TridVideoSubSystem[chain].RectSD.m_nHeight;                    
				}
			}
			else
			{
				gTmPipeObject.TridVideoSubSystem[chain].bVidFullImageSD = TRUE;
				sdPosition.uFlags |= PIPE_VP_SURFACE_SRC_FULL_IMAGE;
				if(gTmPipeObject.TridVideoSubSystem[chain].bVidFullScreenSD == TRUE)
				{
					sdPosition.uFlags |= PIPE_VP_SURFACE_DEST_FULL_SCREEN;
				}
				else
				{
					sdPosition.DstRect.nX = gTmPipeObject.TridVideoSubSystem[chain].RectSD.m_nX;
					sdPosition.DstRect.nY = gTmPipeObject.TridVideoSubSystem[chain].RectSD.m_nY;
					sdPosition.DstRect.nSizeX = gTmPipeObject.TridVideoSubSystem[chain].RectSD.m_nWidth;
					sdPosition.DstRect.nSizeY = gTmPipeObject.TridVideoSubSystem[chain].RectSD.m_nHeight;                    
				}
				//printf("SD->psRect is NULL\n");
			}
		}
		if((eScreenDevice&EM_UDI_VOUT_DEVICE_HD) == EM_UDI_VOUT_DEVICE_HD)
		{
			uStandard = gTmPipeObject.TmDisplaySubSys[chain].uHDVidStandard; 
			gTmPipeObject.TridVideoSubSystem[chain].bVidFullImageHD = FALSE;
			if(psRect != NULL)
			{
				hdPosition.SrcRectOrScale.SrcRect.nX = (psRect->m_nX*0x4000)/1920;
				hdPosition.SrcRectOrScale.SrcRect.nY= (psRect->m_nY*0x4000)/1080;
				hdPosition.SrcRectOrScale.SrcRect.nSizeX = (psRect->m_nWidth*0x4000)/1920;
				hdPosition.SrcRectOrScale.SrcRect.nSizeY = (psRect->m_nHeight*0x4000)/1080;
				if(gTmPipeObject.TridVideoSubSystem[chain].bVidFullScreenHD == TRUE)
				{
					hdPosition.uFlags |= PIPE_VP_SURFACE_DEST_FULL_SCREEN;
				}
				else
				{                    
					hdPosition.DstRect.nX = gTmPipeObject.TridVideoSubSystem[chain].RectHD.m_nX;
					hdPosition.DstRect.nY = gTmPipeObject.TridVideoSubSystem[chain].RectHD.m_nY;
					hdPosition.DstRect.nSizeX = gTmPipeObject.TridVideoSubSystem[chain].RectHD.m_nWidth;
					hdPosition.DstRect.nSizeY = gTmPipeObject.TridVideoSubSystem[chain].RectHD.m_nHeight;
				}
				
				
				CSDEBUG(MODULE_NAME,DEBUG_LEVEL,"HD->psRect->m_nX[%d]m_nY[%d]m_nWidth[%d]m_nHeight[%d]\n",psRect->m_nX,psRect->m_nY,psRect->m_nWidth,psRect->m_nHeight);
			}
			else
			{
				gTmPipeObject.TridVideoSubSystem[chain].bVidFullImageHD = TRUE;
				hdPosition.uFlags |= PIPE_VP_SURFACE_SRC_FULL_IMAGE;
				if(gTmPipeObject.TridVideoSubSystem[chain].bVidFullScreenHD == TRUE)
				{
					hdPosition.uFlags |= PIPE_VP_SURFACE_DEST_FULL_SCREEN;
				}
				else
				{                    
					hdPosition.DstRect.nX = gTmPipeObject.TridVideoSubSystem[chain].RectHD.m_nX;
					hdPosition.DstRect.nY = gTmPipeObject.TridVideoSubSystem[chain].RectHD.m_nY;
					hdPosition.DstRect.nSizeX = gTmPipeObject.TridVideoSubSystem[chain].RectHD.m_nWidth;
					hdPosition.DstRect.nSizeY = gTmPipeObject.TridVideoSubSystem[chain].RectHD.m_nHeight;
				}
				//printf("HD->psRect is NULL\n");
			}
		}

		/*save it */
		if (eScreenDevice&EM_UDI_VOUT_DEVICE_SD)
		{
			if (psRect != NULL)
			{
				gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD = *psRect;
				gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_position(gTmVpDeviceObj.pSDVideoSurface[surface_indx],&sdPosition);            
			}
			else
			{
                gTmVpDeviceObj.pSDVideoSurface[surface_indx]->set_position(gTmVpDeviceObj.pSDVideoSurface[surface_indx],&sdPosition);            
				memset(&gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD,0,sizeof(gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleSD));
			}
		}
		if (eScreenDevice&EM_UDI_VOUT_DEVICE_HD)
		{
			if (psRect != NULL)
			{
				gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD = *psRect;
				gTmVpDeviceObj.pHDVideoSurface[surface_indx]->set_position(gTmVpDeviceObj.pHDVideoSurface[surface_indx],&hdPosition);
			}
			else
			{
				/*we will check if width and height are 0 in CSUDIVIDEOGetWinClip*/
                gTmVpDeviceObj.pHDVideoSurface[surface_indx]->set_position(gTmVpDeviceObj.pHDVideoSurface[surface_indx],&hdPosition);            
				memset(&gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD,0,sizeof(gTmPipeObject.TridVideoSubSystem[chain].SrcRectScaleHD));
			}
		}
	}while(0);
	
	Cnxt_Retcode = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取窗口剪切位置和尺寸

@param[in] nVideoIndex 视频解码器的索引
@param[in] eScreenDevice 设置在哪个屏幕上的视频剪切区域
@param[out] psRect 剪切的区域，详见CSUDIWinRect_S定义。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * psRect)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CSUDIWinRect_S *pstSrcRect=NULL;
	CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
	Cnxt_Retcode = cnxt_kal_sem_get(gUdi2VideoSem, CNXT_KAL_WAIT_FOREVER);    
	do
	{
		if((nVideoIndex < 0)||(nVideoIndex>=CS_TRID_MAX_DEC))
		{
			Retcode = CSUDIVIDEO_ERROR_INVALID_DEVICE_ID;
			break;
		}
		
		if (eScreenDevice != EM_UDI_VOUT_DEVICE_HD && eScreenDevice != EM_UDI_VOUT_DEVICE_SD)
		{
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		
		if (psRect == CSUDI_NULL)
		{
			Retcode = CSUDIVIDEO_ERROR_BAD_PARAMETER;
			break;
		}
		if (eScreenDevice == EM_UDI_VOUT_DEVICE_HD)
		{
			pstSrcRect = &gTmPipeObject.TridVideoSubSystem[nVideoIndex].SrcRectScaleHD;
			/*window clip:full src screen*/
			if (pstSrcRect->m_nWidth == 0 && pstSrcRect->m_nHeight == 0)
			{
				CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_HD,
				gTmPipeObject.TmDisplaySubSys[nVideoIndex].uHDResolution,
				psRect);
			}
			/*window clip:little box src screen*/
			else
			{
				*psRect = *pstSrcRect;
			}
		}        
		if (eScreenDevice == EM_UDI_VOUT_DEVICE_SD)
		{
			pstSrcRect = &gTmPipeObject.TridVideoSubSystem[nVideoIndex].SrcRectScaleSD;
			/*window clip:full src screen*/
			if (pstSrcRect->m_nWidth == 0 && pstSrcRect->m_nHeight == 0)
			{
				CS_TRID_GetFullScreenWidowSize(EM_UDI_VOUT_DEVICE_SD,
				gTmPipeObject.TmDisplaySubSys[nVideoIndex].uSDResolution,
				psRect);
			}
			/*window clip:little box src screen*/
			else
			{
				*psRect = *pstSrcRect;
			}
		}
	}while(0);

	Cnxt_Retcode = cnxt_kal_sem_put(gUdi2VideoSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

