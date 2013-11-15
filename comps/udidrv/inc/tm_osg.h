
/****************************************************************************/
/*                   Trident Multimedia Technologies (Shanghai) Co, LTD                                   */
/*                        SOFTWARE FILE/MODULE HEADER                                                        */
/*                 Copyright Trident Multimedia Technologies (Shanghai) Co, LTD 2004-200       */
/*                            All Rights Reserved                                                                         */
/****************************************************************************/
/*
 * Filename:        trid_osg.h
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 * Author:           2010.11
 *
 ****************************************************************************/


#ifndef _TRID_OSG_INCLUDE_H_
#define _TRID_OSG_INCLUDE_H_

#define SOI 0xFFD8
#define EOI 0xFFD9
#define SOS 0xFFDA
/* the height of the osd region0 image */
#define RGN1_HEIGHT 720
/* the width of the osd region0 image */
#define RGN1_WIDTH  1280

#define RGN0_HEIGHT 576
/* the width of the osd region1 image */
#define RGN0_WIDTH  720

#define PIXEL_NOT_SUPPORT   0xff

#define DecodeJPGMaxWidth (2048)
#define DecodeJPGMaxHeight (2048)

#define MAX_SURFACE_NUM       128
#define MIN_BLIT_RECT_WIDTH 16

#define MIN_BLIT_RECT_HEIGHT 16

typedef enum {       /* JPEG marker codes */
  M_SOF0  = 0xc0,
  M_SOF1  = 0xc1,
  M_SOF2  = 0xc2,
  M_SOF3  = 0xc3,
  
  M_SOF5  = 0xc5,
  M_SOF6  = 0xc6,
  M_SOF7  = 0xc7,
  
  M_JPG   = 0xc8,
  M_SOF9  = 0xc9,
  M_SOF10 = 0xca,
  M_SOF11 = 0xcb,
  
  M_SOF13 = 0xcd,
  M_SOF14 = 0xce,
  M_SOF15 = 0xcf,
  
  M_DHT   = 0xc4,
  
  M_DAC   = 0xcc,
  
  M_RST0  = 0xd0,
  M_RST1  = 0xd1,
  M_RST2  = 0xd2,
  M_RST3  = 0xd3,
  M_RST4  = 0xd4,
  M_RST5  = 0xd5,
  M_RST6  = 0xd6,
  M_RST7  = 0xd7,
  
  M_SOI   = 0xd8,
  M_EOI   = 0xd9,
  M_SOS   = 0xda,
  M_DQT   = 0xdb,
  M_DNL   = 0xdc,
  M_DRI   = 0xdd,
  M_DHP   = 0xde,
  M_EXP   = 0xdf,
  
  M_APP0  = 0xe0,
  M_APP1  = 0xe1,
  M_APP2  = 0xe2,
  M_APP3  = 0xe3,
  M_APP4  = 0xe4,
  M_APP5  = 0xe5,
  M_APP6  = 0xe6,
  M_APP7  = 0xe7,
  M_APP8  = 0xe8,
  M_APP9  = 0xe9,
  M_APP10 = 0xea,
  M_APP11 = 0xeb,
  M_APP12 = 0xec,
  M_APP13 = 0xed,
  M_APP14 = 0xee,
  M_APP15 = 0xef,
  
  M_JPG0  = 0xf0,
  M_JPG13 = 0xfd,
  M_COM   = 0xfe,
  
  M_TEM   = 0x01,
  
  M_ERROR = 0x100
} JPEG_MARKER;

typedef enum 
{
    CS_TM_JPEG,
    CS_TM_IFRAME,
    CS_TM_UNKNOWN
}CS_TM_STILLIMAGE_TYPE;

typedef struct {
   // u_int8              length;
    u_int8              index;
    bool                used;
    u_int8              pixelFormat;
    u_int16             width;
    u_int16             height;
    u_int32             colorKey;
    u_int32             alphaValue;
	int                 alphaValueFlag;
	int                 colorKeyFlag;
    void                 *pImageHandle;
//	PIPE_VP_SURFACE_INPUT inputType;

}TM_Image_Surface;

typedef struct STILLPICINFO{
	u_int16 uImageWidth;
	u_int16 uImageHeight;
	u_int16 uOriWidth;
	u_int16 uOriHeight;
}StillPicInfo;



typedef  struct CS_CNXT_OsgIframe_SubSystem
{
    PIPE_PIPELINE_OBJECT     *pPipeObj;
    PIPE_DEMUX_OBJECT        *pDmxObject;
	PIPE_VIDEO_OBJECT 	     *pStillVidObject;
    bool                     bIframeEnable;
    bool                     bJpegEnable;
 }CS_CNXT_OsgIframe_SubSystem;

#define MAX_OSG_IFRAME (1)


CSUDI_Error_Code ImageInfo(const void * pvImageMem, int nMemSize,int * ImgWidth,int * ImgHeight,const char * pszImageType);
CSUDI_HANDLE StillDecode_IFrame(const void * pvImageMem,int nMemSize, CSUDIOSGScalingMode_E  eScalingMode,const CSUDIOSGRect_S *psDstRect);
CNXT_STATUS destroy_IFrame_pipeline(void);
CNXT_STATUS create_IFrame_pipeline(void);
CSUDI_HANDLE StillDecode_Jpeg(const void * pvImageMem, int nMemSize,CSUDIOSGScalingMode_E  eScalingMode,const CSUDIOSGRect_S *	psDstRect);
CNXT_STATUS create_jpeg_pipeline(unsigned int ucJpgSize);

#endif

