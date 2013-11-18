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
@brief  ע��ָ���¼����͵Ļص�����

@param[in] nVideoIndex ��Ƶ������������
@param[in] fnVideoCallback �ص�����ָ�룬���CSUDIVIDEOCallback_F����
@param[in] eEvent �ص��������ͣ����CSUDIVIDEOEventType_E
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����֧��ע��Ļص���������Ϊ32���������
@note ������ע��EM_UDIVIDEO_MAXEVENTTYPE���͵Ļص�
@note �ص�����ָ��+�ص���������+�û�����Ψһ��ʶһ���ص�,������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIVIDEO_ERROR_CALLBACK_EXIST

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
@brief ɾ��ָ���¼����͵Ļص�����

@param[in] nVideoIndex ��Ƶ������������
@param[in] fnVideoCallback �ص�����ָ�룬���CSUDIVIDEOCallback_F����
@param[in] eEvent �ص��������ͣ����CSUDIVIDEOEventType_E
@param[in] pvUserData ͬCSUDIVIDEOAddCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIVIDEO_ERROR_CALLBACK_NOT_EXIST
@note �����ڻص�������ɾ���ص�

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
@brief ����VBI��ʽ��teletext\close caption\Macrovision ���ܿ���

@param[in] nVideoIndex ��Ƶ������������
@param[in] eVideoVBIType VBI�������ͣ�������Ͽ���
@param[in] bEnable �򿪻�ر�eVideoVBIType��Ӧ�Ĺ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ϵͳ����ʱ���й���Ĭ�Ϲرգ�ֻ�е��ñ��ӿڲ��ܴ򿪻�رն�Ӧ���ܣ�����һ��������Ч
@note ������ΪEM_UDIVIDEO_VBI_TELTEXTʱ����Ҫͬplayer�ӿ�CSUDIPLAYERSetStream���ʹ��,eg:   
@note ����VBI teltext ����
@note  sStreamInfo.m_eContentType=EM_UDI_CONTENT_TELTEXT;
@note  sStreamInfo.m_nPid=teltext_pid;
@note  CSUDIPLAYERSetStream(hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL);
@note  CSUDIVIDEOSetVBIEnable(nVideoIndex,EM_UDIVIDEO_VBI_TELTEXT, CSUDI_TRUE);
@note  ������������Ҫ����PID,��ֻ�����CSUDIVIDEOSetVBIEnable�ӿ�

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
@brief ��ȡVBI��������teletext\close caption\Macrovision ����״̬

@param[in] nVideoIndex ��Ƶ������������
@param[in] eVideoVBIType VBI�������ͣ�ÿ��ֻ�ܲ�ѯһ������
@param[out] pbEnable ,eVideoVBIType��Ӧ�Ĺ��ܿ���״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

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
@brief ��ȡVideo Decoder�豸������

@param[out] pnVideoDecCount ϵͳ�� ����Video Decoder�豸�ĸ���
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ�����������pnVideoDecCount�����ݲ�ȷ��

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
@brief ��ȡָ��Video Decoder�豸��������Ϣ

���ۿ��л��߱�ռ��
@param[in] nVideoIndex Video Decoder�豸������
@param[out] psCapabilityInfo Video Decoder�豸������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ�����������psCapabilityInfo�ṹ�����ݲ�ȷ��

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

/**@brief ��Ƶ��ͼ(����)�ӿڣ�����ǰ������ʾ����Ƶͼ��Ĳ���(��ȫ��)��ȡ�������Ŀ��ͼ���ָ������

@param[in] nVideoIndex  Video Decoder�豸������
@param[in] psSrcRect ���봰�ڴ�С����ʾҪ��ȡ��ǰ��Ƶͼ�����һ���֣�ΪCSUDI_NULL��ʾ����ͼ��ע�ⲻ�ܳ�����ǰ��Ƶͼ���С��
@param[in] psDstRect ������ڴ�С����ʾ�����hSurface����һ���֣�ΪCSUDI_NULL��ʾ����hSurface��ע�ⲻ�ܳ���hSurface��С��
@param[out] phSurface ��Ƶ��ͼ�������hSurface��ָ�����򣬸�Surface�ǵ���OSG�ӿڴ����Ļ������ֱ�ӻ�ȡ�Դ棻
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����hSurface�ṹ������δ���塣
@note �ڽ�ȡI֡ͼƬʱ��ͨ������CSUDIVIDEOGetStreamStatus��ȡ��ǰI֡��Ϣ������ͼ������Ϣ��
@note �����Ʒ����Ҫ��Ƶ���չ��ܻ���Ӳ����֧�֣��ýӿڿ��Բ�ʵ�֡���ʱ�践��CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED��

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
@brief ��ȡ����������״̬��Ϣ

�統ǰ���ŵ���Ƶ��С��
@param[in]  nVideoIndex Video Decoder�豸��Դ������
@param[out] psStreamStatus ����IFRAME��Ϣ ��CSUDIVIDStreamStatus_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psStreamStatus�ṹ������δ����

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
@brief ��ʾ/������Ƶ�����������

@param[in] nVideoIndex  ��Ƶ������������
@param[in] bIsShow ΪCSUDI_TRUE���ʾ��ʾ�ô���;ΪCSUDI_FALSE���ʾ���ظô���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ 
@note ϵͳ��ʼ��ʱӦΪ��ʾ״̬

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
@brief ����ָ����Ƶ�������Ļ��߱�

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[in] eAspectRatio ��Ƶ����ӿڵĿ�߱ȣ���4:3 �� 16:9
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note auto��ʾ���ݽ�Ŀ�Ŀ�߱��Զ�������Ļ��߱ȣ�Ҳ������߱�Ч�������ײ���������
@note ����ָ����Ƶ����ӿ��������Ŀ�߱ȣ�Ŀǰֻ֧��16:9/4:3/auto��ģʽ��
����Ч�������������ŵĽ�Ŀ�Ŀ�߱��Լ�ָ����Ƶ����ӿڵ���Ӧģʽ(MatchMethod)������
@note �˽ӿ����õ���ʾЧ��ֻ����Ƶȫ����ʾʱ��Ч������ƵС������ʾʱ�������ã������۴˽ӿ�������ã���ƵС������ʾʱʼ��������С���ڵ�

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
@brief��ȡָ����Ƶ�������Ļ��߱�

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[out] peAspectRatio ��Ƶ����ӿڵĿ�߱ȣ���4:3 �� 16:9
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���ӿ����ڻ�ȡ��ǰ���õ���Ļ��߱�״̬��������õ���AUTO���򷵻�AUTO��
���������CSUDIVIDEOGetStreamStatus�ӿڻ��

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
@brief����ָ����Ƶ����ӿڵ���Ӧģʽ

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[in] eMatchMethod ָ����Ƶ����˿ڵ���Ӧģʽ����PAN_SCAN,LETTER_BOX,��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����ָ����Ƶ����˿ڵ���Ļ��Ӧģʽ��
����Ч�������������ŵĽ�Ŀ�Ŀ�߱��Լ�ָ����Ƶ����ӿڵĻ����(AspectRatio)�����أ���ο���ͼ��
@image HTML VIDAspectRatioType.png
@note �˽ӿ����õ���ʾЧ��ֻ����Ƶȫ����ʾʱ��Ч������ƵС������ʾʱ�������ã������۴˽ӿ�������ã���ƵС������ʾʱʼ��������С���ڵ�

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
@brief��ȡָ����Ƶ����ӿڵ���Ӧģʽ

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[out] peMatchMethod ָ����Ƶ����˿ڵ���Ӧģʽ����PAN_SCAN,LETTER_BOX,��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

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
@brief ����ֹͣ��Ƶ��ģʽ

����Ŀ�л�ʱ����Ƶ�л�Ч�����羲֡������
@param[in] nVideoIndex  Video Decoder�豸��Դ������
@param[in] eStopMode ֹͣģʽ��������ο�CSUDIVIDEOStopMode_E��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

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
@brief��ȡֹͣ��Ƶ��ģʽ

����Ŀ�л�ʱ����Ƶ�л�Ч�����羲֡������
@param[in] nVideoIndex  Video Decoder�豸��Դ������
@param[out] peStopMode ֹͣģʽ��������ο�CSUDIVIDEOStopMode_E��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

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
@brief ����ָ����Ƶ��������Ĵ��ڴ�С��ʵ�����Ź���

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ���ڴ�С
@param[in] pstRect ���ڵĴ�С�Լ�����λ�ã��ο�CSUDIWinRect_S�ṹ�����pstRectΪNULL������ʾΪȫ����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note pstRectΪ����뵱ǰ��Ƶ�ֱ��ʵľ���,����Ϣ���Դ�CSUDISCREENGetResolution�ӿڻ�ȡ
@note �����ǲü�����Ϊ���������

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
@brief ��ȡָ����Ƶ��������Ĵ��ڴ�С

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ���ڴ�С
@param[out] pstRect ���ڵĴ�С�Լ�����λ�ã��ο�CSUDIWinRect_S�ṹ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

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
@brief ���ô��ڼ���λ�úͳߴ�

���ú�������Ƶ���ڽ���ʾ�����������Ƶ������ʵ�־ֲ��Ŵ���С�Ȳ�����
�����ú��ƶ�����������Screen����ʾ(���û�����ù�CSUDIVIDEOSetWindowSize��Ϊȫ����ʾ)
@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ��������
@param[in] psRect ָ�����е��������CSUDIWinRect_S���塣
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���psRectΪCSUDI_NULL�����ʾȫ����ʾ
@note pstRectΪ����뵱ǰ��Ƶ�ֱ��ʵľ���,����Ϣ���Դ�CSUDISCREENGetResolution�ӿڻ�ȡ

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
@brief ��ȡ���ڼ���λ�úͳߴ�

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ��������
@param[out] psRect ���е��������CSUDIWinRect_S���塣
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

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

