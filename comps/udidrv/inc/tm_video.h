
/****************************************************************************/
/*                   Trident Multimedia Technologies (Shanghai) Co, LTD                                   */
/*                        SOFTWARE FILE/MODULE HEADER                                                        */
/*                 Copyright Trident Multimedia Technologies (Shanghai) Co, LTD 2004-200       */
/*                            All Rights Reserved                                                                         */
/****************************************************************************/
/*
 * Filename:        trid_video.h
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 * Author:          Alfred Chen 2010.11
 *
 ****************************************************************************/


#ifndef _TRID_VIDEO_INCLUDE_H_
#define _TRID_VIDEO_INCLUDE_H_

#if 0



#define STILL_VIDEO_PLANE  CNXT_DRM_PLANE_V1
#define MOTION_VIDEO_PLANE CNXT_DRM_PLANE_V0

/********************************/
/* Video image size definitions */
/********************************/

/*
 * Image widths need to be a multiple of the tile width which is 128
 * (e.g. use 768 instead of 720).
 */
#define MAX_HD_VIDEO_WIDTH        1920
#define MAX_HD_VIDEO_HEIGHT       1080

#define MAX_SD_VIDEO_WIDTH        768
#define MAX_SD_VIDEO_HEIGHT       576

#define MAX_SATURATION   100
#define MIN_SATURATION   0
#define MAX_BRIGHTNESS   100
#define MIN_BRIGHTNESS   0
#define MAX_CONTRAST     100
#define MIN_CONTRAST     0
#define MAX_ALPHA        100
#define MIN_ALPHA           0
#define DEFAULT_VIDEO_STANDARD_HD CNXT_VIDEO_STANDARD_ATSC_1080I_50HZ
#define DEFAULT_VIDEO_STANDARD_SD CNXT_VIDEO_STANDARD_PAL_B_WEUR

#define DEFAULT_RESOLUTION_WIDTH 1280
#define DEFAULT_RESOLUTION_HEIGHT 720


CNXT_STATUS CS_CNXT_Video_open();

#endif

typedef struct CS_CNXT_WINDOWSIZE
{
    u_int32 uVideoStandard;
    u_int32 width;
    u_int32 height;
}CS_CNXT_WINDOWSIZE;

typedef enum
{
    VidClosed,
    VidOpened,
    VidReady,
    VidStopped,
    VidStarted,
    VidPlaying,
    VidPause
}VidState;
CNXT_STATUS CS_TRID_Video_init(void);

#endif

