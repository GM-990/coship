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
BOOL g_bJPGDecode = FALSE;
int32 g_pJPGdataOffset = 0;
int32 g_nMemSize = 0;

u_int8* gJPGBuffer  = NULL;
#define PIPE_IFRAME_BUFFER_SIZE   (1*1024*1024)
u_int8* gIframeBuffer  = NULL;
BOOL g_binject = FALSE;
#ifdef DUAL_VIDEO_SURFACE
CNXT_IMAGE_HANDLE gOsgImageHandle = NULL;
#ifdef STATIC_IFRAME_DECODE
extern bool cs_tm_iframe_stop(void);
#endif
#endif

u_int32 GfxSclFilterCoeff[256]=
{
	0x10000000, 0x10200800, 0x10401400, 0x10601C00, 	 /* hor LUT 0 entry: 00-03  */
	0x10802800, 0x109033FF, 0x10A03FFF, 0x10C04BFF, 	 /* hor LUT 0 entry: 04-07  */
	0x10D05BFF, 0x10E067FF, 0x10F073FE, 0x10F083FE, 	 /* hor LUT 0 entry: 08-11  */
	0x110093FE, 0x1110A3FE, 0x1110AFFD, 0x1120BFFC, 	 /* hor LUT 0 entry: 12-15  */
	0x1110CFFC, 0x1120E3FC, 0x1120F3FB, 0x112107FB, 	 /* hor LUT 0 entry: 16-19  */
	0x111117FA, 0x11112BFA, 0x11113BF9, 0x11114FF9, 	 /* hor LUT 0 entry: 20-23  */
	0x110163F8, 0x110177F8, 0x11018BF7, 0x10F19FF7, 	 /* hor LUT 0 entry: 24-27  */
	0x10F1B3F6, 0x10E1C7F5, 0x10E1DBF5, 0x10D1EFF4, 	 /* hor LUT 0 entry: 28-31  */
	0x10C203F4, 0x10C217F3, 0x10B22BF2, 0x10B23FF2, 	 /* hor LUT 0 entry: 32-35  */
	0x10A253F1, 0x109267F1, 0x10927BF0, 0x10828FF0, 	 /* hor LUT 0 entry: 36-39  */
	0x1082A3F0, 0x1072B7EF, 0x1072CBEF, 0x1062DBEF, 	 /* hor LUT 0 entry: 40-43  */
	0x1062EFEF, 0x1052FFEE, 0x105313EE, 0x104323EE, 	 /* hor LUT 0 entry: 44-47  */
	0x104337EF, 0x104343EE, 0x103357EF, 0x102363EF, 	 /* hor LUT 0 entry: 48-51  */
	0x102373F0, 0x102387F1, 0x102393F1, 0x10139FF2, 	 /* hor LUT 0 entry: 52-55  */
	0x1013ABF3, 0x1013B7F4, 0x1013C7F6, 0x1013D3F7, 	 /* hor LUT 0 entry: 56-59  */
	0x1003DBF8, 0x1003E7FA, 0x1003EFFC, 0x1003FBFE, 	 /* hor LUT 0 entry: 60-63  */
	0x10000000, 0x10200800, 0x10401400, 0x10601C00, 	 /* hor LUT 1 entry: 00-03  */
	0x10802800, 0x109033FF, 0x10A03FFF, 0x10C04BFF, 	 /* hor LUT 1 entry: 04-07  */
	0x10D05BFF, 0x10E067FF, 0x10F073FE, 0x10F083FE, 	 /* hor LUT 1 entry: 08-11  */
	0x110093FE, 0x1110A3FE, 0x1110AFFD, 0x1120BFFC, 	 /* hor LUT 1 entry: 12-15  */
	0x1110CFFC, 0x1120E3FC, 0x1120F3FB, 0x112107FB, 	 /* hor LUT 1 entry: 16-19  */
	0x111117FA, 0x11112BFA, 0x11113BF9, 0x11114FF9, 	 /* hor LUT 1 entry: 20-23  */
	0x110163F8, 0x110177F8, 0x11018BF7, 0x10F19FF7, 	 /* hor LUT 1 entry: 24-27  */
	0x10F1B3F6, 0x10E1C7F5, 0x10E1DBF5, 0x10D1EFF4, 	 /* hor LUT 1 entry: 28-31  */
	0x10C203F4, 0x10C217F3, 0x10B22BF2, 0x10B23FF2, 	 /* hor LUT 1 entry: 32-35  */
	0x10A253F1, 0x109267F1, 0x10927BF0, 0x10828FF0, 	 /* hor LUT 1 entry: 36-39  */
	0x1082A3F0, 0x1072B7EF, 0x1072CBEF, 0x1062DBEF, 	 /* hor LUT 1 entry: 40-43  */
	0x1062EFEF, 0x1052FFEE, 0x105313EE, 0x104323EE, 	 /* hor LUT 1 entry: 44-47  */
	0x104337EF, 0x104343EE, 0x103357EF, 0x102363EF, 	 /* hor LUT 1 entry: 48-51  */
	0x102373F0, 0x102387F1, 0x102393F1, 0x10139FF2, 	 /* hor LUT 1 entry: 52-55  */
	0x1013ABF3, 0x1013B7F4, 0x1013C7F6, 0x1013D3F7, 	 /* hor LUT 1 entry: 56-59  */
	0x1003DBF8, 0x1003E7FA, 0x1003EFFC, 0x1003FBFE, 	 /* hor LUT 1 entry: 60-63  */
	0x0CB0D400, 0x0CD0DC00, 0x0CF0E400, 0x0D10EC00, 	 /* vert LUT 0 entry: 00-03  */
	0x0D20F000, 0x0D40FC00, 0x0D610400, 0x0D811001, 	 /* vert LUT 0 entry: 04-07  */
	0x0D911401, 0x0DB12001, 0x0DC12401, 0x0DE13001, 	 /* vert LUT 0 entry: 08-11  */
	0x0DF13801, 0x0E114402, 0x0E214C02, 0x0E415402, 	 /* vert LUT 0 entry: 12-15  */
	0x0E515C02, 0x0E616803, 0x0E817403, 0x0E917C03, 	 /* vert LUT 0 entry: 16-19  */
	0x0EA18404, 0x0EB19004, 0x0EC19804, 0x0EE1A405, 	 /* vert LUT 0 entry: 20-23  */
	0x0EE1AC05, 0x0EF1B806, 0x0F01C407, 0x0F11CC07, 	 /* vert LUT 0 entry: 24-27  */
	0x0F31D407, 0x0F31E008, 0x0F41EC09, 0x0F51F80A, 	 /* vert LUT 0 entry: 28-31  */
	0x0F62000A, 0x0F62080B, 0x0F72140C, 0x0F82200D, 	 /* vert LUT 0 entry: 32-35  */
	0x0F92240D, 0x0F92340F, 0x0F924010, 0x0FA24C11, 	 /* vert LUT 0 entry: 36-39  */
	0x0FB25412, 0x0FB25812, 0x0FC26814, 0x0FC27015, 	 /* vert LUT 0 entry: 40-43  */
	0x0FC27C16, 0x0FD28417, 0x0FD28C18, 0x0FD2981A, 	 /* vert LUT 0 entry: 44-47  */
	0x0FE2A41B, 0x0FE2AC1C, 0x0FE2B41E, 0x0FE2BC1F, 	 /* vert LUT 0 entry: 48-51  */
	0x0FF2C821, 0x0FF2D022, 0x0FF2DC24, 0x0FF2E025, 	 /* vert LUT 0 entry: 52-55  */
	0x0FF2EC27, 0x0FF2F028, 0x1002FC2A, 0x1003042C, 	 /* vert LUT 0 entry: 56-59  */
	0x1003102E, 0x1003142F, 0x10031C31, 0x10032433, 	 /* vert LUT 0 entry: 60-63  */
	0x0CB0D400, 0x0CD0DC00, 0x0CF0E400, 0x0D10EC00, 	 /* vert LUT 1 entry: 00-03  */
	0x0D20F000, 0x0D40FC00, 0x0D610400, 0x0D811001, 	 /* vert LUT 1 entry: 04-07  */
	0x0D911401, 0x0DB12001, 0x0DC12401, 0x0DE13001, 	 /* vert LUT 1 entry: 08-11  */
	0x0DF13801, 0x0E114402, 0x0E214C02, 0x0E415402, 	 /* vert LUT 1 entry: 12-15  */
	0x0E515C02, 0x0E616803, 0x0E817403, 0x0E917C03, 	 /* vert LUT 1 entry: 16-19  */
	0x0EA18404, 0x0EB19004, 0x0EC19804, 0x0EE1A405, 	 /* vert LUT 1 entry: 20-23  */
	0x0EE1AC05, 0x0EF1B806, 0x0F01C407, 0x0F11CC07, 	 /* vert LUT 1 entry: 24-27  */
	0x0F31D407, 0x0F31E008, 0x0F41EC09, 0x0F51F80A, 	 /* vert LUT 1 entry: 28-31  */
	0x0F62000A, 0x0F62080B, 0x0F72140C, 0x0F82200D, 	 /* vert LUT 1 entry: 32-35  */
	0x0F92240D, 0x0F92340F, 0x0F924010, 0x0FA24C11, 	 /* vert LUT 1 entry: 36-39  */
	0x0FB25412, 0x0FB25812, 0x0FC26814, 0x0FC27015, 	 /* vert LUT 1 entry: 40-43  */
	0x0FC27C16, 0x0FD28417, 0x0FD28C18, 0x0FD2981A, 	 /* vert LUT 1 entry: 44-47  */
	0x0FE2A41B, 0x0FE2AC1C, 0x0FE2B41E, 0x0FE2BC1F, 	 /* vert LUT 1 entry: 48-51  */
	0x0FF2C821, 0x0FF2D022, 0x0FF2DC24, 0x0FF2E025, 	 /* vert LUT 1 entry: 52-55  */
	0x0FF2EC27, 0x0FF2F028, 0x1002FC2A, 0x1003042C, 	 /* vert LUT 1 entry: 56-59  */
	0x1003102E, 0x1003142F, 0x10031C31, 0x10032433, 	 /* vert LUT 1 entry: 60-63  */
};


static BOOL bIsHWDecodeSupported(u_int8 *ptempImage,u_int32 unImageLen)
{
        int sos = 0;
        u_int8 y_component_id,y_sampling_factor,y_quantaization;
        u_int8 cb_component_id,cb_sampling_factor,cb_quantaization;
        u_int8 cr_component_id,cr_sampling_factor,cr_quantaization;
        int length;
	    u_int8 *pEndImage = ptempImage+unImageLen;
        bool end_of_image = FALSE;        
        bool HARDDECODE = FALSE;
        bool SOFTDECODE = FALSE;
     
       
         /* Outer loop repeats once for each marker. */
         for (;ptempImage<pEndImage;) 
         {
             /* Collect the marker proper, unless we already did. */
             /* NB: first_marker() enforces the requirement that SOI appear first. */
             if(*ptempImage == 0xFF && (*(ptempImage+1) == M_SOI))
             {
                  //printf("\n SOI %x ", *ptempImage);
                  ++ptempImage;
                  ++ptempImage; 
                  //printf("\n SOI %x ", *ptempImage);
             }

            do
            {
               switch (*ptempImage) 
              {
                    case M_SOF0:
                            //printf("\n image is a baseline image...check for subsampling");      
                            ptempImage =    ptempImage + 8; // 8= lenght -2, precission - 1, height -2, width - 2, no.of components - 1
                            /*printf("\n pointer is at  *ptempImage = %x %x %x %x %x %x",*ptempImage,*(ptempImage + 1),*(ptempImage + 2),
                           *(ptempImage + 3),*(ptempImage + 4),*(ptempImage + 5));*/
                        
                            if(*ptempImage == 0x03) // if no. of components is 3
                            {
                               y_component_id = *(++ptempImage);				   			   
                               y_sampling_factor = *(++ptempImage);
                               y_quantaization = *(++ptempImage);    
                               cb_component_id = *(++ptempImage);
                               cb_sampling_factor = *(++ptempImage);
                               cb_quantaization = *(++ptempImage);    
                               cr_component_id = *(++ptempImage);
                               cr_sampling_factor = *(++ptempImage);
                               cr_quantaization = *(++ptempImage);                
                               if(y_sampling_factor == 0x12)
                               {
                                    SOFTDECODE = TRUE;
                                    //printf("\n Nevis only support the raster scan order...so bypass to softdecode"); 
                                    break;
                               }
                               else
                               {
                                    HARDDECODE = TRUE;  // no more parsing needed...send it to harddecode
                                    //printf("\n else...Nevis only support the raster scan order...leave it to hard decode");  
                                    break;
                               }
                            }
                            ptempImage++; 
                        break;   
                   case M_SOF1:
                   case M_SOF2:
                   case M_SOF3:
                   case M_SOF5:
                   case M_SOF6:
                   case M_SOF7:
                   case M_SOF9:
                   case M_SOF10:
                   case M_SOF11:
                   case M_SOF13:
                   case M_SOF14:
                   case M_SOF15:
                   case M_JPG:
                   case M_DHT:   
                   case M_DAC:
                   case M_RST0:
                   case M_RST1:
                   case M_RST2:
                   case M_RST3:
                   case M_RST4:
                   case M_RST5:
                   case M_RST6:
                   case M_RST7:
                        ptempImage++;
                        length = (u_int32)*(ptempImage);
                        length = length << 8;
                        length = length + (u_int32)*(ptempImage + 1);
                        //printf("\n SOF's... length = %d  *ptempImage = %x",length,*ptempImage); 
                        ptempImage = ptempImage + length;
                        //printf("\n SOF's... length = %d  *ptempImage = %x",length,*ptempImage);
                        //printf("\n app0... pointer is at *ptempImage = %x",*ptempImage);   
                        break;
                  
                case M_EOI:
                        end_of_image = TRUE;
                        //printf("\n End of image");
                        break;  
              
                 case M_SOS:   
                        //printf("\n SOS...*ptempImage = %x",*ptempImage);                     
                        ptempImage++; 
                        length = (u_int32)*(ptempImage);
                        length = length << 8;
                        length = length + (u_int32)*(ptempImage + 1);         
                        /*printf("\n SOS... length = %d  *ptempImage =  %x %x %x %x %x %x",length,
                           *ptempImage,*(ptempImage + 1),*(ptempImage + 2),
                           *(ptempImage + 3),*(ptempImage + 4),*(ptempImage + 5));*/
                        ptempImage = ptempImage + length;
                        /*printf("\n SOS... *ptempImage =  %x %x %x %x %x %x",*ptempImage,*(ptempImage + 1),*(ptempImage + 2),
                           *(ptempImage + 3),*(ptempImage + 4),*(ptempImage + 5));*/
                        //printf("\n look for next valid marker");
                        while(ptempImage<pEndImage)
                        {
                           if(*ptempImage == 0xFF && (*(ptempImage + 1) == M_SOS))
                           {
                               sos++;
                               if(sos > 1)
                               {
                                   SOFTDECODE = TRUE;     
                                   //printf("\n image is a progress image...send it to softdecode");       
                                   break;
                               }
                            }
                            else if(*ptempImage == 0xFF && (*(ptempImage + 1) == M_EOI))
                            {
                                end_of_image = TRUE;                        
                                //printf("\n SOS... end_of_image -- *ptempImage = %x",*ptempImage);    
                                break;
                            }
                           ptempImage++;  //look for next marker 
                        }
                        //printf("\n SOS... *ptempImage = %x",*ptempImage);                   
                  break;          
                  
                  case M_DQT:
                  case M_DNL:
                  case M_DRI:
                  case M_DHP:
                  case M_EXP:         
                  case M_APP0:
                  case M_APP1:
                  case M_APP2:
                  case M_APP3:
                  case M_APP4:
                  case M_APP5:
                  case M_APP6:
                  case M_APP7:
                  case M_APP8:
                  case M_APP9:
                  case M_APP10:
                  case M_APP11:
                  case M_APP12:
                  case M_APP13:
                  case M_APP14:
                  case M_APP15:     
                  case M_JPG0:
                  case M_JPG13:
                  case M_COM:
                  case M_TEM:  
                        //printf("\n app pointer... *ptempImage = %x",*ptempImage);  
                        ptempImage++;
                        length = (u_int32)*(ptempImage);
                        length = length << 8;
                        length = length + (u_int32)*(ptempImage + 1);
                        //printf("\n app0... length = %d  *ptempImage = %x %x",length,*ptempImage,*(ptempImage + 1));
                        ptempImage = ptempImage + length;
                        /*printf("\n pointer is at  *ptempImage = %x %x %x %x %x %x",*ptempImage,*(ptempImage + 1),*(ptempImage + 2),
                           *(ptempImage + 3),*(ptempImage + 4),*(ptempImage + 5));*/
                        //printf("\n app0... *ptempImage = %x",*ptempImage);              
                  break;
   
                  /*case M_ERROR:
                        //printf("\n error data");
                   break;
                   */
                   default:
                        //printf("\n marker or invalid data");
                        //printf("\n default... *ptempImage = %x",*ptempImage);                 
                        ++ptempImage;
                        //printf("\n default... *ptempImage = %x",*ptempImage);                       
                   break;          
                }
              }while((ptempImage<pEndImage)&&(*ptempImage !=0xFF) && (end_of_image != TRUE) && 
                                        (HARDDECODE == FALSE) && (SOFTDECODE == FALSE)) ;
   
              //printf("\n after while... *ptempImage = %x",*ptempImage);         
   		if (ptempImage>=pEndImage)
   		{
			SOFTDECODE = TRUE;
			break;
   		}
              if(HARDDECODE == TRUE)
              {
                  //printf("\n HARDDECODE is true  ptempImage = %x",*ptempImage);
                  break;
              }
              if((*ptempImage == 0xFF) && (end_of_image != TRUE))
              {
                   ++ptempImage; 
                   //printf("\n new marker  ptempImage = %x",*ptempImage);
              }
              else
              {
                   //printf("\n end of image ptempImage =  %x",*ptempImage);
                   break;
              }
           }
		
         return (!SOFTDECODE);
}

static void pipe_stillimage_notifier ( struct pipe_pipeline_obj *pPipe,
										void                     *pUserData,
										PIPE_NOTIFY_EVENT         Event,
										void                     *pData,
										void                     *pTag )
{
	int chain = 0;
	CNXT_VIDEO_PICTURE_INFO * pPictureInfo;
	if(gOsgIframeSubsys[chain].pPipeObj != pPipe)
	return;
	switch ( Event )
	{
	case PIPE_EVENT_DEMUX_SYNC_ACQUIRED:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_DEMUX_SYNC_ACQUIRED \n");
		break;
		
	case PIPE_EVENT_DEMUX_SYNC_LOST:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_DEMUX_SYNC_LOST \n");
		break;

	case PIPE_EVENT_AUDIO_STARTED:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_AUDIO_STARTED \n");
		break;

	case PIPE_EVENT_AUDIO_STOPPED:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_AUDIO_STOPPED \n");
		break;

	case PIPE_EVENT_AUDIO_SYNC_ACQ:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_AUDIO_SYNC_ACQ \n");
		break;
		
	case PIPE_EVENT_AUDIO_STREAM_LOST:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_AUDIO_STREAM_LOST \n");
		break;
	case PIPE_EVENT_VIDEO_STARTED:
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_VIDEO_STARTED \n");
		break;

	case PIPE_EVENT_VIDEO_STOPPED:
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_VIDEO_STOPPED \n");
		break;

	case PIPE_EVENT_VIDEO_SYNC_ACQ:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_VIDEO_SYNC_ACQ \n");
		break;
		
	case PIPE_EVENT_VIDEO_SYNC_LOST:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_VIDEO_SYNC_LOST \n");
		break;

	case PIPE_EVENT_PIPELINE_MEDIA_DATA_CONSUMED:
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_PIPELINE_MEDIA_DATA_CONSUMED \n");
		break;
	case PIPE_EVENT_VIDEO_STREAM_CHANGE:
		{
			
			pPictureInfo = (CNXT_VIDEO_PICTURE_INFO *)pData;
			
			
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"	Notify : PIPE_EVENT_VIDEO_STREAM_CHANGE pPictureInfo->Size.uWidth =%d pPictureInfo->Size.uHeight=%d\n",pPictureInfo->Size.uWidth,pPictureInfo->Size.uHeight);
			stJPGInfo.uImageWidth = pPictureInfo->Size.uWidth;
			stJPGInfo.uImageHeight = pPictureInfo->Size.uHeight;
			stJPGInfo.uOriWidth = pPictureInfo->Size.uWidth;
			stJPGInfo.uOriHeight = pPictureInfo->Size.uHeight;
		}
		break;


	case PIPE_EVENT_PIPELINE_REQ_CONT_BLK_FROM_T:
		{


			CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : PIPE_EVENT_PIPELINE_REQ_CONT_BLK_FROM_T \n");
					g_binject = TRUE;
	
		}
		break;

	case PIPE_EVENT_PIPELINE_REQ_NEXT_CONT_BLK:
		{

			CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Notify : ....PIPE_EVENT_PIPELINE_REQ_NEXT_CONT_BLK \n");
				g_binject = TRUE;
		}
		break;
	case PIPE_EVENT_PIPELINE_REQ_NEXT_SECTION_BLK:
		break;

	default:
		break;

	}

	return;
}

CNXT_STATUS create_stillimage_pipeline(CS_TM_STILLIMAGE_TYPE type)
{
	int chain = 0;    
	PIPE_VIDEO_CFG       VideoCfg;    
	PIPE_STATUS          pipeStatus;    
	PIPE_DEMUX_CFG       DemuxCfg;
	PIPE_PIPELINE_CFG    PipeCfg;
	PIPE_FLUSH_MODE  FlushMode; 
	PIPE_VIDEO_ATTRIB           VideoAttrib;
	
	u_int32   uEventMask		= 0;

#ifdef DUAL_VIDEO_SURFACE
	CNXT_IMAGE_CAPS      imageCaps;

#ifdef OSG_USEIMAGE
	if(gOsgImageHandle==NULL)
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

		if ( CNXT_STATUS_OK != cnxt_image_open(&gOsgImageHandle, &imageCaps, NULL, NULL))
		{
			cnxt_kal_trace ( TL_ERROR, "Still decode: Failed to open image, err ="
			               "\n");
			return CNXT_STATUS_FAILURE;
		}

		printf("###############gOsgImageHandle =%lx###############\n",(int) gOsgImageHandle);

	}
#endif
#endif
	gOsgIframeSubsys[chain].pPipeObj = PIPE_OBJ_CREATE(pipeline);
	if(gOsgIframeSubsys[chain].pPipeObj == NULL)
	{
		
		CSDEBUG(MODULE_NAME, ERROR_LEVEL," Cannot create pipeline exit function	%s line=%d \n",__FUNCTION__,__LINE__);
		
		return CNXT_STATUS_FAILURE;
	}
	
	if( TRUE == bVideoDecoderInUse(0))
	{
		
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, " create_stillimage_pipeline: video decoder 0 is be used function	%s line=%d \n",__FUNCTION__,__LINE__);
		return CNXT_STATUS_FAILURE;
	}

#ifdef DUAL_VIDEO_SURFACE

	if(gTmPipeObject.hVideoObj[1]!=NULL)
	{

		#ifdef STATIC_IFRAME_DECODE
		cs_tm_iframe_stop();
		#endif
		if (PIPE_STATUS_OK!=PIPE_OBJ_DESTROY(video,gTmPipeObject.hVideoObj[1]))
		{
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
			return CNXT_STATUS_FAILURE;
		}
			

	}
	
	gTmPipeObject.hVideoObj[1] = gOsgIframeSubsys[chain].pStillVidObject  = PIPE_OBJ_CREATE ( video ); 
	if(gOsgIframeSubsys[chain].pStillVidObject == NULL)
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"failed %s %d \n", __FUNCTION__, __LINE__);
		return CNXT_STATUS_FAILURE;
	}

#else
	gOsgIframeSubsys[chain].pStillVidObject   = PIPE_OBJ_CREATE ( video );
		if(gOsgIframeSubsys[chain].pStillVidObject  == NULL)
			{
				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"OSG ERROR Cannot create video\n" );
				return CNXT_STATUS_FAILURE;
			}
#endif

	//gOsgIframeSubsys[chain].pStillVidObject = gTmPipeObject.hVideoObj[0];
	
	gOsgIframeSubsys[chain].pDmxObject = PIPE_OBJ_CREATE(demux);
	
	if(gOsgIframeSubsys[chain].pDmxObject == NULL)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Cannot create demux\n" );
		return CNXT_STATUS_FAILURE;
	}
#ifdef DUAL_VIDEO_SURFACE
    	cnxt_kal_memset(&DemuxCfg,0,sizeof(DemuxCfg));
#endif
	if(type == CS_TM_JPEG)
	{
		DemuxCfg.Type = PIPE_DEMUX_VIDEO_ES;
	}
	else
	{
		DemuxCfg.Type = PIPE_DEMUX_VIDEO_ES;
        
		CSDEBUG(MODULE_NAME, ERROR_LEVEL," create_stillimage_pipeline: just iframe \n\r" );
	}
	
	pipeStatus = gOsgIframeSubsys[chain].pDmxObject->config(gOsgIframeSubsys[chain].pDmxObject, &DemuxCfg);
	if ( pipeStatus != PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Cannot config demux[%d]\n",pipeStatus );
		return CNXT_STATUS_FAILURE; 
	}
	//cnxt_kal_thread_time_sleep(10);
	cnxt_kal_memset(&VideoCfg, 0, sizeof(VideoCfg));
	
	if(type == CS_TM_IFRAME)
	{
#ifdef DUAL_VIDEO_SURFACE
		VideoCfg.DecodingType        = PIPE_VIDEO_MOTION_STILL;
		VideoCfg.Definition   = PIPE_VIDEO_HD;
		VideoCfg.bHwAccelerate = TRUE;
		VideoCfg.bPrimaryDecoder = TRUE;
		VideoCfg.uCBuffSize = 1024*1024*1+1;
		VideoCfg.uImageWidth = 2048;
		VideoCfg.uImageHeight = 1088;

		#ifdef OSG_USEIMAGE
              VideoCfg.StillImageHandle = gOsgImageHandle;
        	#endif
		
		VideoCfg.Format = PIPE_VIDEO_FORMAT_MPEG2;
#else
		VideoCfg.DecodingType        = PIPE_VIDEO_MOTION_STILL;
		VideoCfg.Definition   = PIPE_VIDEO_HD;
		VideoCfg.bHwAccelerate = TRUE;

#if 1
		VideoCfg.uCBuffSize = 1024*1024*2;
		VideoCfg.uImageWidth = 2048;
		VideoCfg.uImageHeight = 1088;
#endif
		
		VideoCfg.Format = PIPE_VIDEO_FORMAT_MPEG2;
#endif
	}
	else
	{

		/* config video */
		VideoCfg.DecodingType = PIPE_VIDEO_MOTION_STILL;
		VideoCfg.Definition   = PIPE_VIDEO_HD;
		VideoCfg.uCBuffSize = PIPE_JPEG_CHUNK_SIZE;
		VideoCfg.uImageWidth = DecodeJPGMaxWidth;
		VideoCfg.uImageHeight = DecodeJPGMaxHeight;
		VideoCfg.JpegRotMode = PIPE_VIDEO_JPEG_NO_ROTATION;
		VideoCfg.Format = PIPE_VIDEO_FORMAT_JPEG;
		VideoCfg.bPrimaryDecoder    = TRUE;
		VideoCfg.uUserDataEnableMap = 0;
	    VideoCfg.bHwAccelerate = TRUE;
#ifdef DUAL_VIDEO_SURFACE

		#ifdef OSG_USEIMAGE
              VideoCfg.StillImageHandle = gOsgImageHandle;
              #endif
#endif
	}
	
	pipeStatus = gOsgIframeSubsys[chain].pStillVidObject->config(gOsgIframeSubsys[chain].pStillVidObject, &VideoCfg);
	if ( pipeStatus != PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Cannot config video[%d]\n",pipeStatus );
		return CNXT_STATUS_FAILURE; 
	}
	
	//PIPE_VIDEO_OBJECT
	//cnxt_kal_thread_time_sleep(10);
	uEventMask = (1 << (PIPE_EVENT_VIDEO_STARTED & 0xFFFFFF)) |
	(1 << (PIPE_EVENT_VIDEO_STOPPED & 0xFFFFFF)) |
	(1 << (PIPE_EVENT_VIDEO_DISPLAY_UPDATE & 0xFFFFFF)) |
	(1 << (PIPE_EVENT_VIDEO_PLAYING & 0xFFFFFF))|
	(1 << (PIPE_EVENT_VIDEO_STREAM_CHANGE & 0xFFFFFF))|
	(1 << (PIPE_EVENT_VIDEO_BUFFER_EMPTY & 0xFFFFFF)); 
	
	gOsgIframeSubsys[chain].pStillVidObject->subscribe_event(gOsgIframeSubsys[chain].pStillVidObject, uEventMask); 
	//cnxt_kal_thread_time_sleep(10);
	cnxt_kal_memset(&PipeCfg, 0, sizeof(PIPE_PIPELINE_CFG));
	PipeCfg.uStcIndex = 0;
	PipeCfg.Type = PIPE_PIPELINE_NONE_TS_TYPE;
	PipeCfg.SyncMaster = PIPE_SYNC_NO_SYNC;//PIPE_SYNC_VIDEO_MASTER;//
	PipeCfg.bPipPipe = FALSE;
	
	if ( gOsgIframeSubsys[chain].pPipeObj->config ( gOsgIframeSubsys[chain].pPipeObj, &PipeCfg ) != PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Cannot config pipline\n" );
		
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  " exit function  %s line=%d \n",__FUNCTION__,__LINE__);
		return CNXT_STATUS_FAILURE;
	}   
	//cnxt_kal_thread_time_sleep(10);

	pipeStatus = gOsgIframeSubsys[chain].pPipeObj->add_device(gOsgIframeSubsys[chain].pPipeObj, (PIPE_OBJ_HANDLE)gOsgIframeSubsys[chain].pDmxObject);
	if ( pipeStatus != PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Cannot add demux to pipeline[%d]\n",pipeStatus );
		return CNXT_STATUS_FAILURE; 
	}

	pipeStatus = gOsgIframeSubsys[chain].pPipeObj->add_device(gOsgIframeSubsys[chain].pPipeObj, (PIPE_OBJ_HANDLE)gOsgIframeSubsys[chain].pStillVidObject);
	if ( pipeStatus != PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Cannot add video to pipeline[%d]\n",pipeStatus );
		return CNXT_STATUS_FAILURE; 
	}  
	pipeStatus = gOsgIframeSubsys[chain].pPipeObj->set_notifier ( gOsgIframeSubsys[chain].pPipeObj, pipe_stillimage_notifier,(void *)gOsgIframeSubsys[chain].pPipeObj );
	if ( pipeStatus != PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Failed to set notifier function[%d]\n",pipeStatus );
		return CNXT_STATUS_FAILURE;
	}
	
	FlushMode = PIPE_FULL_FLUSH;
	if(gOsgIframeSubsys[chain].pPipeObj->flush(gOsgIframeSubsys[chain].pPipeObj,FlushMode)!= PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"can't flush the pipe line\n" );
	}
	/* enable demux */
	if ( gOsgIframeSubsys[chain].pPipeObj->pDemux->demux_play_ctrl ( gOsgIframeSubsys[chain].pPipeObj->pDemux, TRUE ) != PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"failed to enable demux\n" );
		return CNXT_STATUS_FAILURE;
	}

	if(gOsgIframeSubsys[chain].pPipeObj->pVideoDecoder->decoder_ctrl(gOsgIframeSubsys[chain].pPipeObj->pVideoDecoder, TRUE)!=PIPE_STATUS_OK )
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"video decoder control fail\n" );
		return CNXT_STATUS_FAILURE;         
	}
	pipeStatus = gOsgIframeSubsys[chain].pPipeObj->set_decoding_speed(gOsgIframeSubsys[chain].pPipeObj,PIPE_PLAY_CONTINUOUS_MODE, 1024 );
	if(pipeStatus !=PIPE_STATUS_OK ) 
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, "set_decoding_speed fail %d\n",pipeStatus);
		return CNXT_STATUS_FAILURE;      
	}
	if( type == CS_TM_JPEG )
	{
		gOsgIframeSubsys[0].pStillVidObject->get_attrib( gOsgIframeSubsys[chain].pStillVidObject, &VideoAttrib );
		g_hDecoderImage = VideoAttrib.hImage;
	}
	else
	{
		gOsgIframeSubsys[0].pStillVidObject->get_attrib( gOsgIframeSubsys[0].pStillVidObject, &VideoAttrib );
		g_hDecoderImage = VideoAttrib.hImage;


		CSDEBUG(MODULE_NAME, DEBUG_LEVEL," Get right image handle\n\r ");
	}

	CSDEBUG(MODULE_NAME, DEBUG_LEVEL," create_stillimage_pipeline: create stillimage pipeline success \n\r");
	if(type == CS_TM_IFRAME)
	cnxt_kal_thread_time_sleep(50);

	return CNXT_STATUS_OK;
	
}
static CSUDI_HANDLE GetStillSurface(StillPicInfo *pstStillInfo,
                                        CSUDIOSGScalingMode_E  eScalingMode,const CSUDIOSGRect_S *  psDstRect,
                                        CNXT_IMAGE_HANDLE hImageHandle)
{
	int nDstWidth=0,nDstWeight=0;
	CSUDI_HANDLE hSurface=NULL;
	CNXT_RECT ImageRect;
	TM_Image_Surface	 *pImageSurface=NULL;
	CNXT_STATUS Retcode;
	CNXT_XY ImageXY;
	CNXT_RECT_SCALE DstRectScale;
	CNXT_GRAPHICS_OP    GraphicsOp;
	int nSrcWidth,nSrcHeight;
	CSUDIOSGRect_S DstRect_tmp;

	if(hImageHandle==NULL)
	{

		return NULL;
	}
	
	memset(&DstRect_tmp,0,sizeof(CSUDIOSGRect_S));
	if(psDstRect != NULL)
	{
	    cnxt_kal_memcpy(&DstRect_tmp,psDstRect,sizeof(CSUDIOSGRect_S));
	}
	else
	{
		if(pstStillInfo!=NULL)
		{
			DstRect_tmp.m_nHeight = pstStillInfo->uImageHeight;
			DstRect_tmp.m_nWidth = pstStillInfo->uImageWidth;
			
		}
		else
		{
			DstRect_tmp.m_nHeight = RGN0_HEIGHT;
			DstRect_tmp.m_nWidth = RGN0_WIDTH;
		}
		
	}
	switch(eScalingMode)
	{
	case EM_UDIOSG_SCALE_FIXSIZE:
		nDstWidth =DstRect_tmp.m_nWidth;
		nDstWeight = DstRect_tmp.m_nHeight;
		break;
	case EM_UDIOSG_SCALE_OPTIMUM:
		{
			int nSrcRate = 0;
			int nDestRate = 0;
			//HD_width

			nSrcWidth = pstStillInfo->uImageWidth;
			nSrcHeight = pstStillInfo->uImageHeight;
			if (pstStillInfo->uOriWidth>0&&pstStillInfo->uOriHeight>0)
			{
				//it should be jpg
				if (pstStillInfo->uOriWidth>DecodeJPGMaxWidth||
						pstStillInfo->uOriHeight>DecodeJPGMaxHeight)
				{
					nSrcRate = (1000*pstStillInfo->uOriWidth)/pstStillInfo->uOriHeight;
					nDestRate = (1000*DecodeJPGMaxWidth)/DecodeJPGMaxHeight;
					if (nSrcRate >= nDestRate)
					{
						nSrcHeight = (pstStillInfo->uOriHeight*DecodeJPGMaxWidth)/pstStillInfo->uOriWidth;
						nSrcWidth = DecodeJPGMaxWidth;
					}
					else if(nSrcRate < nDestRate)
					{
						nSrcWidth = (pstStillInfo->uOriWidth*DecodeJPGMaxHeight)/pstStillInfo->uOriHeight;
						nSrcHeight = DecodeJPGMaxHeight;
					}
				}
			}

			//printf("opt:first adjust(%d,%d)\n",nSrcWidth,nSrcHeight);
			
			nDstWidth = DstRect_tmp.m_nWidth;
			nDstWeight = DstRect_tmp.m_nHeight;

			//printf("OPTIMUM %d %d\n",nDstWidth,nDstWeight);
			
			nSrcRate = 1000*(nSrcWidth)/(nSrcHeight);
			nDestRate = 1000*(nDstWidth)/(nDstWeight);
			/*if optimum mode ,we need adjust blit width and weight according to picture
			width and weight rate*/
			if (nSrcRate>nDestRate)
			{
				nDstWeight = (nSrcHeight*nDstWidth)/nSrcWidth;
			}
			else if (nSrcRate < nDestRate)
			{
				nDstWidth = (nSrcWidth*nDstWeight)/nSrcHeight;
			}
		}
		break;
	case EM_UDIOSG_SCALE_FULLSIZE:
		{
			int nSrcRate = 0;
			int nDestRate = 0;

			nSrcWidth = pstStillInfo->uImageWidth;
			nSrcHeight = pstStillInfo->uImageHeight;
			if ((pstStillInfo->uOriWidth>DecodeJPGMaxWidth||pstStillInfo->uOriHeight>DecodeJPGMaxHeight)
					|| (nSrcWidth>RGN1_WIDTH||nSrcHeight>RGN1_HEIGHT))
			{
				nSrcRate = 1000*(nSrcWidth)/(nSrcHeight);
				nDestRate = 1000*(RGN1_WIDTH)/(RGN1_HEIGHT);
				if (nSrcRate>nDestRate)
				{
					nDstWeight = (nSrcHeight*RGN1_WIDTH)/nSrcWidth;
					nDstWidth = RGN1_WIDTH;
				}
				else if (nSrcRate < nDestRate)
				{
					nDstWidth = (nSrcWidth*RGN1_HEIGHT)/nSrcHeight;
					nDstWeight = RGN1_HEIGHT;
				}
				else
				{
					nDstWidth = RGN1_WIDTH;
					nDstWeight = RGN1_HEIGHT;
				}
			}
			else
			{
				nDstWidth = nSrcWidth;
				nDstWeight = nSrcHeight;
			}
			break;
		}
	}	

	if (CSUDI_SUCCESS != CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,
				nDstWidth,nDstWeight,&hSurface))
	{
		return NULL;
	}
	/*Copy blit the 420 image to the specified surface */
	GraphicsOp.uBltControl  = CNXT_GRAPHICS_OP_ROP |CNXT_GRAPHICS_OP_PRESERVE_ALPHA ;
	GraphicsOp.uAlphaUse    = CNXT_GRAPHICS_ALPHA_STORE_SRC;

	GraphicsOp.uAlpha = 0xFF;
	GraphicsOp.uColorUse    = CNXT_GRAPHICS_COLOR_STORE_RESULT;
	GraphicsOp.bAsyncSig    = TRUE;
	GraphicsOp.bAsync       = FALSE;
	GraphicsOp.ROP          = CNXT_GRAPHICS_ROP_COPY;


	ImageXY.nX      = ImageXY.nY      = 0;
	ImageRect.nLeft = ImageRect.nTop  = 0;

	pImageSurface = (TM_Image_Surface*)hSurface;

	ImageRect.nRight    = pstStillInfo->uImageWidth;
	ImageRect.nBottom   = pstStillInfo->uImageHeight;
	internal_fill_graphicsOp_by_default(&GraphicsOp);
#if 0
    {
    
        
        u_int8              *puFilterCoefsNew;
        u_int16              i,j;      
        u_int32              uValue;
        
           /* Allocate memory to store the Coefficients */
           Retcode = cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_CACHED, 
                                        sizeof(u_int8) * 256 * 4, 
                                        CNXT_MEM_DEFAULT, 
                                        (void **)&puFilterCoefsNew);
           if(Retcode != CNXT_STATUS_OK)
           {
              return;
           }
        
        /* Copy Filter Coefficients  */
        
            for(i=0,j=0;i<256;i++,j=j+4)
            {
                uValue  = GfxSclFilterCoeff[i];
                puFilterCoefsNew[j+3] = (uValue & 0xFF000000) >> 24;
                puFilterCoefsNew[j+2] = (uValue & 0x00FF0000) >> 16;
                puFilterCoefsNew[j+1] = (uValue & 0x0000FF00) >> 8;
                puFilterCoefsNew[j]   = (uValue & 0x000000FF);
            }
        
           Retcode =  cnxt_graphics_set_filter_coefficients(
                                   gTridGraphicsHandle,
                                   TRUE,
                                   1024,
                                   puFilterCoefsNew);    
        
           if(Retcode != CNXT_STATUS_OK)
           {
              CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Error Setting filter coefficients Retcode: %d \n ",Retcode);  
              hSurface = NULL;
              return hSurface;
           }
        
           Retcode =  cnxt_graphics_set_filter_coefficients(
                                   gTridGraphicsHandle,
                                   FALSE,
                                   1024,
                                   puFilterCoefsNew);    
        
           if(Retcode != CNXT_STATUS_OK)
           {
              CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Error Setting filter coefficients Retcode: %d \n ",Retcode);  
              hSurface = NULL;
              return hSurface;
           }
    
    }
#endif
	if((ImageRect.nRight == nDstWidth)&&(ImageRect.nBottom == nDstWeight))
	{
		Retcode = cnxt_graphics_copy_blit(
		gTridGraphicsHandle, hImageHandle, 0, &ImageRect, NULL, 
		pImageSurface->pImageHandle, 0, &ImageXY, &GraphicsOp);
	}
	else
	{
		
		DstRectScale.bRect = TRUE;
		DstRectScale.RectOrScale.Rect.nLeft   = DstRectScale.RectOrScale.Rect.nTop = 0;
		DstRectScale.RectOrScale.Rect.nRight  = nDstWidth;
		DstRectScale.RectOrScale.Rect.nBottom = nDstWeight;
		
		Retcode = cnxt_graphics_stretch_copy(
		gTridGraphicsHandle, hImageHandle, 0, &ImageRect, NULL, 
		pImageSurface->pImageHandle, 0, &DstRectScale, &GraphicsOp);
	}
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Error: performing cnxt_graphics_copy_blit Retcode=0x%x..\n", Retcode);           
		CSUDIOSGDestroySurface(hSurface);
		hSurface = NULL;
	}

	return hSurface;
}

CNXT_STATUS destroy_stillimage_pipeline(CS_TM_STILLIMAGE_TYPE type)
{
	int chain = 0;
	PIPE_VP_SURFACE_INPUT       Input;
	

	
	//cnxt_kal_thread_time_sleep(100);	  
	/* stop video decoder and demux */
	if(gOsgIframeSubsys[chain].pPipeObj->pVideoDecoder)
	{  
		if(gOsgIframeSubsys[chain].pPipeObj->pVideoDecoder->decoder_ctrl(gOsgIframeSubsys[chain].pPipeObj->pVideoDecoder,FALSE)!=PIPE_STATUS_OK )
		{
			
			
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,  " error exit function	%s line=%d \n",__FUNCTION__,__LINE__);
			return CNXT_STATUS_FAILURE;          
		}
	}
	//internal_pipe_process_plbk_state
	if(gOsgIframeSubsys[chain].pPipeObj->pDemux)
	{
		if(gOsgIframeSubsys[chain].pPipeObj->pDemux->demux_play_ctrl(gOsgIframeSubsys[chain].pPipeObj->pDemux, FALSE) != PIPE_STATUS_OK )
		{
			
			CSDEBUG(MODULE_NAME,ERROR_LEVEL,  " error exit function	%s line=%d \n",__FUNCTION__,__LINE__);
			return CNXT_STATUS_FAILURE;          
		}
	}
	/* Flush pipeline */
	/*delete video decoder from pipeline*/
	if( gOsgIframeSubsys[chain].pPipeObj->pVideoDecoder != NULL)
	{	 
		if ( gOsgIframeSubsys[chain].pPipeObj->delete_device(gOsgIframeSubsys[chain].pPipeObj, (PIPE_OBJ_HANDLE)gOsgIframeSubsys[chain].pStillVidObject)!= PIPE_STATUS_OK )
		{
			
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  " error exit function	%s line=%d \n",__FUNCTION__,__LINE__);
			return CNXT_STATUS_FAILURE; 
		}  
		gOsgIframeSubsys[chain].pPipeObj->pVideoDecoder = NULL;
	}
	
	if(gOsgIframeSubsys[chain].pPipeObj->pDemux != NULL)
	{
		if ( gOsgIframeSubsys[chain].pPipeObj->delete_device(gOsgIframeSubsys[chain].pPipeObj, (PIPE_OBJ_HANDLE)gOsgIframeSubsys[chain].pDmxObject)!= PIPE_STATUS_OK )
		{
			
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  " error  exit function	%s line=%d \n",__FUNCTION__,__LINE__);
			return CNXT_STATUS_FAILURE; 
		}
	}
	/* Destroy pipeline and demux */
	
	PIPE_OBJ_DESTROY ( video, gOsgIframeSubsys[chain].pStillVidObject);
#ifdef DUAL_VIDEO_SURFACE
    	gTmPipeObject.hVideoObj[1] = gOsgIframeSubsys[chain].pStillVidObject = NULL;


	#ifdef OSG_USEIMAGE        
	if(CNXT_STATUS_OK != cnxt_image_close(gOsgImageHandle))
	{
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Err: %s %d \n", __FUNCTION__, __LINE__);
		return CNXT_STATUS_FAILURE;
	}
	gOsgImageHandle = NULL;
	#endif
#endif
	
	PIPE_OBJ_DESTROY ( demux, gOsgIframeSubsys[chain].pDmxObject);
	PIPE_OBJ_DESTROY ( pipeline, gOsgIframeSubsys[chain].pPipeObj);
	gOsgIframeSubsys[chain].pDmxObject = NULL;
	gOsgIframeSubsys[chain].pStillVidObject = NULL;
	gOsgIframeSubsys[chain].pPipeObj = NULL;
	gOsgIframeSubsys[chain].bIframeEnable = FALSE;
	gOsgIframeSubsys[chain].bJpegEnable = FALSE;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  " exit function  %s line=%d \n",__FUNCTION__,__LINE__);
	return CNXT_STATUS_OK;
	
}

static CNXT_STATUS pipe_jpeg_inject(void)
{
	CNXT_STATUS Status = CNXT_STATUS_OK;
	PIPE_MEDIA_DATA_INFO MediaDataInfo;    
	PIPE_STATUS          pipeStatus = PIPE_STATUS_OK;    
	int chain = 0;
	PIPE_VIDEO_ATTRIB pAttrib;
	u_int32  nWriteCount = 0;
	u_int32  nread= 0;
	u_int32 count = 0;
	u_int32 i = 0;
	
	if(gJPGBuffer != NULL)
	{
		cnxt_kal_mem_free(gJPGBuffer);
		gJPGBuffer = NULL;
	}
	if(g_bJPGDecode == FALSE || gIframeBuffer == NULL || g_nMemSize == 0 || ( g_pJPGdataOffset >=g_nMemSize) )
	{

		if(g_pJPGdataOffset >= g_nMemSize )
		{

			if ( gOsgIframeSubsys[0].pPipeObj->flush ( gOsgIframeSubsys[0].pPipeObj, PIPE_FULL_FLUSH ) != PIPE_STATUS_OK )
			{
				CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "JPEG incremental: Cannot flush pipe\n" );
				// internal_system_test_cleanup_jpeg_pipe(i, bOsdDisp);
			  
			}
			CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "END OF filel\n");
			g_pJPGdataOffset = 0;
			g_bJPGDecode = FALSE;
			return CNXT_STATUS_UNKNOWN;
		}
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "NOT A JPG DECODE WORK \n");
		return CNXT_STATUS_UNKNOWN;
	}

	
	 do
   {
      /* Since there is no event from VSS to indicate when it has consumed the
       * JPEG data, we need to poll for cbuf free size to be non-zero, we are 
       * polling max for 1 sec 
       */
      
      if ( gOsgIframeSubsys[0].pStillVidObject->get_attrib( gOsgIframeSubsys[0].pStillVidObject, &pAttrib ) != PIPE_STATUS_OK )
      {
      
         CSDEBUG(MODULE_NAME, ERROR_LEVEL, "pipe_jpeg_input_incremental: Cannot get video attributes\n" );
         return PIPE_STATUS_INTERNAL_ERROR;
      }
     
      nWriteCount = pAttrib.cbufFreeSize;
      
      if(nWriteCount == 0)
      {
      
         count++;         
         cnxt_kal_thread_time_sleep(5);
		 
      }
   }while((nWriteCount == 0) && (count < 200));

	 
	if(nWriteCount > 0)
	{
	
		 if(nWriteCount > (PIPE_JPEG_CHUNK_SIZE)/2)
	 	{
	 	
		nWriteCount = (PIPE_JPEG_CHUNK_SIZE)/2;
		
	 	}


	    if(nWriteCount > (g_nMemSize - g_pJPGdataOffset))
	 	{
	 	
	 	nread = g_nMemSize - g_pJPGdataOffset;
		
	 	}
		else
		{
		  nread = nWriteCount ; 
		  
		}


		if(gJPGBuffer == NULL )
		{

			Status = cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_UNCACHED,
			nread,
			CNXT_MEM_ALIGN_DEFAULT,
			(void **) &gJPGBuffer);

			}

		if((Status != CNXT_STATUS_OK) || (gJPGBuffer ==  NULL))
		{
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "cnxt_kal_mem_malloc Failed cnxtStatus=%dl\n",Status);
			return CNXT_STATUS_UNKNOWN;
		}
	
	
		memcpy(gJPGBuffer,gIframeBuffer+g_pJPGdataOffset,nread);
		g_pJPGdataOffset += nread;

		//if(g_pJPGdataOffset >= g_nMemSize)
		//g_pJPGdataOffset = 0;

		 	}
	else
   {
      /* This could be the case where decoder has stopped consuming data maybe
       * because decoder is done the decoding and the file has some extra end
       * after the valid JPEG data. for best effort handling, we can flush our
       * CBUF.
       */
      cnxt_kal_trace (TL_ERROR, "Not able to get space in JPEG CBUF after "
                      "waiting for 1 second\n");

	  return CNXT_STATUS_UNKNOWN;
    
   }

	MediaDataInfo.Type    = PIPE_MEDIA_VIDEO_ES_DATA;
	MediaDataInfo.pData   = (u_int8*)gJPGBuffer;
	//MediaDataInfo.pData   = (u_int8*)pvImageMem;
	MediaDataInfo.uOverlap = 0;
	MediaDataInfo.uPts	  = 0;

	/*MediaDataInfo.uLength = nWriteCount;
	if( nread > 1024*1024 )
	MediaDataInfo.uLength = nread;
	else*/
	MediaDataInfo.uLength = nread;//(1024*1024+1);

	//if((1024*1024+1);)
	

	pipeStatus = gOsgIframeSubsys[chain].pPipeObj->inject_media_data(gOsgIframeSubsys[chain].pPipeObj,&MediaDataInfo);
	if(pipeStatus !=PIPE_STATUS_OK ) 
	{
		
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, " Decode_stillimage : set inject media data fail %d\n",pipeStatus);
	
		return pipeStatus;      
	}
	
	return pipeStatus; 
}

static CSUDI_HANDLE Decode_stillimage(CS_TM_STILLIMAGE_TYPE type,const void * pvImageMem, int nMemSize,	CSUDIOSGScalingMode_E  eScalingMode,const CSUDIOSGRect_S *  psDstRect)
{
	CNXT_STATUS Status;
	PIPE_MEDIA_DATA_INFO MediaDataInfo;    
	PIPE_STATUS          pipeStatus;    
	CSUDI_HANDLE  phSurface = NULL;    
	int chain = 0;
	int timeout = 200;
	CNXT_IMAGE_PAGE_PROPERTIES page_prop;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "ENTER function %s in eScalingMode = %d \n",__FUNCTION__,eScalingMode);
	g_nMemSize = nMemSize;

   if(type == CS_TM_JPEG)
   	 g_bJPGDecode =  TRUE;
	

	cnxt_kal_memset(&MediaDataInfo, 0, sizeof(PIPE_MEDIA_DATA_INFO));
	g_nMemSize = nMemSize;

#if 1
	if(gIframeBuffer == NULL )
	{
		if( nMemSize >= 1024*1024 )
		{
			Status = cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_UNCACHED,
			nMemSize+1,
                                			CNXT_MEM_ALIGN_DEFAULT,
                                			(void **) &gIframeBuffer);
		}
		else
		{
			Status = cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_UNCACHED,
                            			(1024*1024),
                            			CNXT_MEM_ALIGN_DEFAULT,
                            			(void **) &gIframeBuffer);
		}
		

		if((Status != CNXT_STATUS_OK) || (gIframeBuffer ==  NULL))
		{
			
			CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "cnxt_kal_mem_malloc Failed cnxtStatus=%dl\n",Status);
				g_bJPGDecode = FALSE;
		g_nMemSize = 0;
		g_pJPGdataOffset = 0;
			return NULL;
		}
	}

	memcpy(gIframeBuffer,pvImageMem,nMemSize);
#endif

	Status = create_stillimage_pipeline(type);
		if(Status != CNXT_STATUS_OK)
	{
		
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Decode_stillimage: creat image fail\n",__FUNCTION__);
		if(!gIframeBuffer)
		{
			cnxt_kal_mem_free(gIframeBuffer);
			gIframeBuffer = NULL;
		}
		if(!gJPGBuffer)
		{
			cnxt_kal_mem_free(gJPGBuffer);
			gJPGBuffer = NULL;
		}
		g_bJPGDecode = FALSE;
		g_nMemSize = 0;
		g_pJPGdataOffset = 0;
		return NULL;
	}

//	pipe_jpeg_inject();
	if(g_bJPGDecode)
	{
		do{
//			if(g_binject == TRUE)
			{
				g_binject = FALSE;
				pipe_jpeg_inject();
			}
			if(g_bJPGDecode==FALSE)
				{
			//	cnxt_kal_thread_time_sleep(150);
	
				 break;
				}
//			cnxt_kal_thread_time_sleep(10);
		}while(1);
		//cnxt_kal_thread_time_sleep(150);
	
	}
   else
   {
	MediaDataInfo.Type    = PIPE_MEDIA_VIDEO_ES_DATA;
	//MediaDataInfo.pData   = (u_int8*)gIframeBuffer;
	MediaDataInfo.pData   = (u_int8*)gIframeBuffer;
	//MediaDataInfo.pData   = (u_int8*)pvImageMem;
	if( nMemSize > 1024*1024 )
	    MediaDataInfo.uLength = nMemSize;
	else
	    MediaDataInfo.uLength = (1024*1024+1);

	

	pipeStatus = gOsgIframeSubsys[chain].pPipeObj->inject_media_data(gOsgIframeSubsys[chain].pPipeObj,&MediaDataInfo);
	if(pipeStatus !=PIPE_STATUS_OK ) 
	{
		
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, " Decode_stillimage : set inject media data fail %d\n",pipeStatus);

		 	
		destroy_stillimage_pipeline(type);
		if(!gIframeBuffer)
		{
		cnxt_kal_mem_free(gIframeBuffer);
		gIframeBuffer = NULL;
		}
		if(!gJPGBuffer)
		{
			cnxt_kal_mem_free(gJPGBuffer);
			gJPGBuffer = NULL;
		}
		g_bJPGDecode = FALSE;
		g_nMemSize = 0;
		g_pJPGdataOffset = 0;

		return NULL;      
	}

	do{
		cnxt_kal_thread_time_sleep(15);
	}while((--timeout>0)&&(!((stJPGInfo.uImageWidth>0)&&(stJPGInfo.uImageHeight>0))));
}

		
    if(g_bJPGDecode)
	{
		if(CNXT_STATUS_OK!=cnxt_image_get_page_properties(g_hDecoderImage, 0, &page_prop))
		{
		  CSDEBUG(MODULE_NAME, ERROR_LEVEL, " cnxt_image_get_page_properties Error in  %s at line :%d\n",__FUNCTION__,__LINE__);
		  destroy_stillimage_pipeline(type);
			  if(NULL!=gIframeBuffer)
		{
			cnxt_kal_mem_free(gIframeBuffer);
			gIframeBuffer = NULL;
		}
		if(NULL!=gJPGBuffer)
		{
			cnxt_kal_mem_free(gJPGBuffer);
			gJPGBuffer = NULL;
		}
		g_bJPGDecode = FALSE;
		g_nMemSize = 0;

		g_pJPGdataOffset = 0;
		  return  NULL;
	}

	stJPGInfo.uImageWidth = page_prop.PageSetProp.uBitmapW;
	stJPGInfo.uImageHeight = page_prop.PageSetProp.uBitmapH;
}
	
	if (!(stJPGInfo.uImageWidth>0&&stJPGInfo.uImageHeight>0))
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Error in %s at line :%d\n",__FUNCTION__,__LINE__);
		memset(&stJPGInfo,0,sizeof(StillPicInfo));

		destroy_stillimage_pipeline(type);
 	if(NULL!=gIframeBuffer)
	{
		cnxt_kal_mem_free(gIframeBuffer);
		gIframeBuffer = NULL;
	}
	if(NULL!=gJPGBuffer)
	{
		cnxt_kal_mem_free(gJPGBuffer);
		gJPGBuffer = NULL;
	}
	   g_bJPGDecode = FALSE;
	   g_nMemSize = 0;

	   g_pJPGdataOffset = 0;
		return NULL;
	}

    phSurface = GetStillSurface(&stJPGInfo,eScalingMode,psDstRect,g_hDecoderImage);
    memset(&stJPGInfo,0,sizeof(StillPicInfo));
    destroy_stillimage_pipeline(type);
	if(NULL!=gIframeBuffer)
	{
    cnxt_kal_mem_free(gIframeBuffer);
    gIframeBuffer = NULL;
	}
	if(NULL!=gJPGBuffer)
	{
		cnxt_kal_mem_free(gJPGBuffer);
		gJPGBuffer = NULL;
	}
	g_bJPGDecode = FALSE;
	g_nMemSize = 0;
	
	g_pJPGdataOffset = 0;

	return phSurface;
	

}

static CNXT_PIXEL_MODE change_pixel_modes_udi2nxp(CSUDIOSGPixelFormat_E ePixelFormat)
{
	CNXT_PIXEL_MODE nxp_pixel_mode;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,  "ENRER  fucntion:%s ePixelFormat =%d\n",__FUNCTION__,ePixelFormat);
	switch(ePixelFormat)
	{
	case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		nxp_pixel_mode = PIXEL_FORMAT_16RGB_565;
		break;
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		nxp_pixel_mode = PIXEL_FORMAT_16ARGB_1555;            
		break;
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		nxp_pixel_mode = PIXEL_FORMAT_16ARGB_4444; 
		break;
	case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		nxp_pixel_mode = PIXEL_FORMAT_32ARGB;
		break;               
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_420:
		nxp_pixel_mode = PIXEL_FORMAT_NV12;
		break;                        
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_444: 
		nxp_pixel_mode = PIXEL_FORMAT_CXY2;
		break;            
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422:
		/*because this board just support 422 planar's blit ,so cancel it for simple deal*/
#if 0
		nxp_pixel_mode = PIXEL_FORMAT_UYVY;
		break;
#endif
	case EM_UDIOSG_PIXEL_FORMAT_RGB_888:
	case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:    
	case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_411:
	case EM_UDIOSG_PIXEL_FORMAT_RGBA_4444:
	case EM_UDIOSG_PIXEL_FORMAT_PALETTE8:        
	default: 
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,  "Error check image pixel format.__line=%d\n",__LINE__);
		nxp_pixel_mode = PIXEL_NOT_SUPPORT;
	}
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"EXIT Function :%s %d\n",__FUNCTION__,__LINE__);
	return nxp_pixel_mode;
}

static CNXT_STATUS CS_OSG_FillImageCaps( CNXT_PIXEL_MODE   PixelMode, u_int16 uWidth, u_int16 uHeight,CNXT_IMAGE_CAPS   *pImageCaps )
{
	u_int32 uLoop;

	if(pImageCaps == NULL)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL, "Error: pImageCaps is null.\n");
		return CNXT_STATUS_BAD_PARAMETER;
	}

	cnxt_kal_memset(pImageCaps, 0, sizeof(CNXT_IMAGE_CAPS));
	pImageCaps->PixelMode = PixelMode;
	pImageCaps->uNumPages = 1;
	pImageCaps->uMaxNumPages = 2;
	pImageCaps->uOrgH = uHeight;
	pImageCaps->uOrgW = uWidth;
	pImageCaps->bIsContiguous = TRUE;
#ifdef SURFACE_USE_UNCACHE
	pImageCaps->bCached = FALSE;
#else
	pImageCaps->bCached = TRUE;
#endif
	pImageCaps->uStride = uWidth;
	pImageCaps->pPage = NULL;    
	// pImageCaps->CursorInvert.bEnabled = FALSE;  
	pImageCaps->ColorSpace = YCC_SD_BT470;  //??????????????????????????????
	pImageCaps->PalTable.uNumPalEntries = 0;
	pImageCaps->PalTable.pPalEntry = NULL;
	for(uLoop = 0; uLoop < IMAGE_MAX_PLANES; uLoop++)
	{
#ifdef SURFACE_USE_UNCACHE
		pImageCaps->PoolId[uLoop] = CNXT_POOL_ID_DEFAULT_UNCACHED;
#else
		pImageCaps->PoolId[uLoop] = CNXT_POOL_ID_DEFAULT_CACHED;
#endif
	}

	return CNXT_STATUS_OK;
}

void osd_sd_output_flick_patch()
{
	CNXT_STATUS      Retcode;  
	u_int8              *puFilterCoefsNew;
	u_int16              i,j;      
	u_int32              uValue;

   /* Allocate memory to store the Coefficients */
   Retcode = cnxt_kal_mem_malloc(CNXT_POOL_ID_DEFAULT_CACHED, 
                                sizeof(u_int8) * 256 * 4, 
                                CNXT_MEM_DEFAULT, 
                                (void **)&puFilterCoefsNew);
   if(Retcode != CNXT_STATUS_OK)
   {
      return;
   }

	/* Copy Filter Coefficients  */

    for(i=0,j=0;i<256;i++,j=j+4)
    {
        uValue  = GfxSclFilterCoeff[i];
        puFilterCoefsNew[j+3] = (uValue & 0xFF000000) >> 24;
        puFilterCoefsNew[j+2] = (uValue & 0x00FF0000) >> 16;
        puFilterCoefsNew[j+1] = (uValue & 0x0000FF00) >> 8;
        puFilterCoefsNew[j]   = (uValue & 0x000000FF);
    }

   Retcode =  cnxt_graphics_set_filter_coefficients(
                           gTridGraphicsHandle,
                           TRUE,
                           1024,
                           puFilterCoefsNew);    

   if(Retcode != CNXT_STATUS_OK)
   {
      CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Error Setting filter coefficients Retcode: %d \n ",Retcode); 
   }

   Retcode =  cnxt_graphics_set_filter_coefficients(
                           gTridGraphicsHandle,
                           FALSE,
                           1024,
                           puFilterCoefsNew);    

   if(Retcode != CNXT_STATUS_OK)
   {
      CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Error Setting filter coefficients Retcode: %d \n ",Retcode);  
   }

   Retcode = cnxt_kal_mem_free(puFilterCoefsNew);
   if(Retcode != CNXT_STATUS_OK)
   {
      return;
   }    
}

static void internal_display_mode(PIPE_VP_DEVICE_ATTRIB *ATTR)
{
	PIPE_VP_DISPLAY_MODE mode;

	mode = ATTR->DisplayMode;
	switch(mode)
	{
	case PIPE_VP_DISPLAY_MODE_NTSC:
		SD_width = 720;
		SD_height = 480;
		break;
	case PIPE_VP_DISPLAY_MODE_PAL:
		SD_width = 720;
		SD_height = 576;
		break;
	case PIPE_VP_DISPLAY_MODE_480I:
	case PIPE_VP_DISPLAY_MODE_480P:
		HD_width = 720;
		HD_height = 480;
		break;
	case PIPE_VP_DISPLAY_MODE_720P:
	case PIPE_VP_DISPLAY_MODE_720P_50Hz:
	case PIPE_VP_DISPLAY_MODE_720P_5994Hz:
		HD_width = 1280;
		HD_height = 720;
		break;
	case PIPE_VP_DISPLAY_MODE_576p:
		HD_width = 720;
		HD_height = 576;
		break;	    
	case PIPE_VP_DISPLAY_MODE_1080I_50Hz:
	case PIPE_VP_DISPLAY_MODE_1080I_5994Hz:
	case PIPE_VP_DISPLAY_MODE_1080I:
	case PIPE_VP_DISPLAY_MODE_1080P_25Hz:
	case PIPE_VP_DISPLAY_MODE_1080P_24Hz:
	case PIPE_VP_DISPLAY_MODE_1080P_50Hz:
	case PIPE_VP_DISPLAY_MODE_1080P_5994Hz:
	case PIPE_VP_DISPLAY_MODE_1080P_60Hz:
	case PIPE_VP_DISPLAY_MODE_1080P_30Hz:
	case PIPE_VP_DISPLAY_MODE_1080P_2997Hz:
	case PIPE_VP_DISPLAY_MODE_1080P_2397Hz:
		HD_width = 1280;//1920;
		HD_height = 720;//1080;
		break;
	case PIPE_VP_DISPLAY_MODE_PAL_B_WEUR:
	case PIPE_VP_DISPLAY_MODE_SECAM:
	default:
		SD_width = 720;
		SD_height = 576;
		break;
	}
}

static CNXT_STATUS internal_graphics_fill_image_caps_osg(CNXT_PIXEL_MODE pixel_mode,u_int16 width, u_int16 height)
{
	CNXT_IMAGE_CAPS ImageCaps,sdImgCaps;
	CNXT_STATUS retCode;
	
	ImageCaps.ColorSpace              = YCC_SD_BT470;
	ImageCaps.PixelMode               = pixel_mode; 
	ImageCaps.PalTable.ColorType      = RGB;
	ImageCaps.PalTable.uNumPalEntries = 0;
	ImageCaps.PalTable.pPalEntry      = NULL;
	ImageCaps.bIsContiguous           = TRUE;            
	ImageCaps.uOrgW                   = width;
	ImageCaps.uOrgH                   = height;
	ImageCaps.PoolId[0]               = CNXT_POOL_ID_DEFAULT_UNCACHED;
	ImageCaps.PoolId[1]               = CNXT_POOL_ID_DEFAULT_UNCACHED;
	ImageCaps.uNumPages               = 2;
	ImageCaps.uMaxNumPages            = 2;
	ImageCaps.bCached                 = FALSE;
	ImageCaps.pPage                   = NULL;   
	ImageCaps.ImageType               = CNXT_IMAGE_STILL_GRAPHICS;
	ImageCaps.uStride                 = width;
	
	retCode = cnxt_image_open(&gImageHandle_hd, &ImageCaps, NULL, NULL);
	if(retCode != CNXT_STATUS_OK)
	{
		cnxt_kal_trace ( TL_ERROR,"internal_graphics_fill_image_caps :%d at line :%d\n",retCode,__LINE__);
		return retCode;
	}
	sdImgCaps.ColorSpace              = YCC_SD_BT470;
	sdImgCaps.PixelMode               = pixel_mode; 
	sdImgCaps.PalTable.ColorType      = RGB;
	sdImgCaps.PalTable.uNumPalEntries = 0;
	sdImgCaps.PalTable.pPalEntry      = NULL;
	sdImgCaps.bIsContiguous           = TRUE;            
	sdImgCaps.uOrgW                   = 720;
	sdImgCaps.uOrgH                   = 576;
	sdImgCaps.PoolId[0]               = CNXT_POOL_ID_DEFAULT_UNCACHED;
	sdImgCaps.PoolId[1]               = CNXT_POOL_ID_DEFAULT_UNCACHED;
	sdImgCaps.uNumPages               = 2;
	sdImgCaps.uMaxNumPages            = 2;
	sdImgCaps.bCached                 = FALSE;
	sdImgCaps.pPage                   = NULL;   
	sdImgCaps.ImageType               = CNXT_IMAGE_STILL_GRAPHICS;
	sdImgCaps.uStride                 = 720;
	
	retCode = cnxt_image_open(&gImageHandle_sd, &sdImgCaps, NULL, NULL);
	if(retCode != CNXT_STATUS_OK)
	{
		cnxt_kal_trace ( TL_ERROR,"internal_graphics_fill_image_caps :%d at line :%d\n",retCode,__LINE__);
		return retCode;
	}
	
	return CNXT_STATUS_OK;
}

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

