/******************************************************************************
FileName:  csmediaplayer.h
Copyright (c) 2007， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        liuzhenghua (liuzhenghua@coship.com)

File Description:
        This file is the manager module of mediaplayer, It implement the 
        interface of mediaplayer.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2007-07-2  liuzhenghua       Create
1.1         2008-02-21 liuzhenghua       add for pvr and citv
										 CSMPPVMStreamHandle
										 CSMPTearDown	
1.2         2008-06-10 liuzhenghua       hServiceHnadle:增加DM中的Service句柄
                                         在DVB播放中先从DM中获取存储的音视频PID,
                                         去播放，已加快换台速度。
*******************************************************************************/

#ifndef CSMEDIAPLAYER_H
#define CSMEDIAPLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include "mmcp_typedef.h"
#include "cs_player_message.h"

/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/

//#define ST_PLATFORM
#define CSMP_THREAD_PRIORITY		(100)/*建议优先级*/
#define CSMP_THREAD_STACKSIZE		(128*1024)/*建议栈大小*/
#define CS_VIDEOCOUNT_PER_PROG 		(30)
#define CS_AUDIOCOUNT_PER_PROG 		(30)
#define CS_SUBTITLECOUNT_PER_PROG 	(8)
#define CS_TELETEXTCOUNT_PER_PROG 	(1)
#define CS_SUBTITLE_PER_STREAM (20)
#define CS_TELETEXT_PER_STREAM (20)

typedef enum CSMPMute_E
{	
	EM_MPUnMute = 0,	    
	EM_MPMute 
}CSMPMute_E;

typedef enum CSMPDSType_E
{
	EM_PLAYER_UNSUPPORT = 0,	    /* UNSUPPORT */
	EM_PLAYER_DVB,	    /* DVB */
	EM_PLAYER_FILE,  	/* FILE */
	EM_PLAYER_IGMP,     /*IP 组播*/
	EM_PLAYER_RTSP,      /*IP 点播*/
	EM_PLAYER_PVRFILE,      /*PVR文件播放*/   
	EM_PLAYER_TSHIFT,	/*PVR 时移播放*/
	EM_PLAYER_CMMB,		/*cmmb*/
	EM_PLAYER_RTSPC,	/*IPQAM 点播*/	
	EM_PLAYER_HTTP,
	EM_PLAYER_IFRAME //仅对udi2有效
 }CSMPDSType_E;

typedef enum CSMPSwitchType_E
{
	 EM_MP_SWITCH_TYPE_UNKNOWN =0,				/*换台时音视频的处理类型：未定义*/
	 EM_MP_SWITCH_TYPE_SCREEN_BLACK	,			/*换台时音视频的处理类型：黑屏*/
	 EM_MP_SWITCH_TYPE_SCREEN_IFRAME	,		/*换台时音视频的处理类型：静桢*/
	 EM_MP_SWITCH_TYPE_SCREEN_FADE_IN_OUT		/*换台时音视频的处理类型：淡入淡出*/
}CSMPSwitchType_E;

/*MediaPlayer module return value*/
typedef enum CSMPRet_E
{
	EM_MP_SUCCESS = 0,       /*成功*/
	EM_MP_FAILURE,           /*未知原因失败*/
	EM_MP_INVALPARA,         /*参数无效*/
	EM_MP_EXIST,             /*资源已存在*/
	EM_MP_INEXISTENCE,       /*资源已不存在*/
	EM_MP_NONSUPPORT         /*系统不支持*/
} CSMPRet_E;

/*MediaPlayer State*/
typedef enum CSMPState_E
{
    EM_MP_STATE_INIT,    /*MP初始状态*/
  	EM_MP_STATE_READY,  /*MP就绪状态，节目停止后也是就绪状态*/
    EM_MP_STATE_PLAYING, /*MP播放状态*/
    EM_MP_STATE_PAUSE,   /*MP暂停状态*/
    EM_MP_STATE_TRICKMODE,  /*MP Trick mode状态*/
    EM_MP_STATE_CONTROL_SENDING, /*控制命令下发状态，回调返回才转入正式状态*/
    EM_MP_STATE_END      /*MP结束状态*/
} CSMPState_E;

typedef enum CSMP_DBG_Level_E
{
    EM_MP_DBG_FATAL = 0,   /* Highest debug level.*/
    EM_MP_DBG_ERROR = 1,
    EM_MP_DBG_WARN  = 2,
    EM_MP_DBG_INFO  = 3,
    EM_MP_DBG_DEBUG = 4    /* lowest debug level */
}CSMP_DBG_Level_E;

typedef enum CSMPAudioChan_E
{	
	EM_CSMPAUDIO_STEREO_CHAN,	  /*立体声声道*/
	EM_CSMPAUDIO_LEFT_CHAN = 0, 	/*左声道*/
	EM_CSMPAUDIO_RIGHT_CHAN,    	/*右声道*/
	EM_CSMPAUDIO_MIX_CHAN      	/*混合声道*/
} CSMPAudioChan_E;

/*
EM_CSMPVIDEO_FMT_NTSC开头的都是30Hz
EM_CSMPVIDEO_FMT_PAL 开头的都是25Hz
EM_CSMPVIDEO_FMT_SECAM ：
    SECAM制式的帧频每秒25帧，扫描线625行，隔行扫描，画面比例4:3，分辨率为720×576，约40万像素，亮度带宽6.0MHz；彩色幅载波 4.25MHz；色度        带宽1.0MHz(U)，1.0MHz(V)；声音载波6.5MHz。SECAM制式的帧频每秒25帧，扫描线625行，隔行扫描，画面比例4:3，分辨率为720×576，约40            万像素，亮度带宽6.0MHz；彩色幅载波 4.25MHz；色度带宽1.0MHz(U)，1.0MHz(V)；声音载波6.5MHz。
EM_CSMPVIDEO_FMT_480P   60Hz
EM_CSMPVIDEO_FMT_576P   通常是50 Hz
EM_CSMPVIDEO_FMT_720P  60Hz
EM_CSMPVIDEO_FMT_1080I 60Hz
EM_CSMPVIDEO_FMT_1080P 60Hz
*/
typedef enum CSMPVdisVideoFormat_E
{
	EM_CSMPVIDEO_FMT_NTSC = 0,          /* 480i, NSTC-M for North America  */ 
	EM_CSMPVIDEO_FMT_NTSC_J,            /* 480i (Japan) */
	EM_CSMPVIDEO_FMT_PAL_B,             /* Australia */
	EM_CSMPVIDEO_FMT_PAL_B1,            /* Hungary */
	EM_CSMPVIDEO_FMT_PAL_D,             /* China */
	EM_CSMPVIDEO_FMT_PAL_D1,            /* Poland */
	EM_CSMPVIDEO_FMT_PAL_G,             /* Europe */
	EM_CSMPVIDEO_FMT_PAL_H,             /* Europe */
	EM_CSMPVIDEO_FMT_PAL_K,             /* Europe */
	EM_CSMPVIDEO_FMT_PAL_I,             /* U.K. */
	EM_CSMPVIDEO_FMT_PAL_M,             /* 525-lines (Brazil) */
	EM_CSMPVIDEO_FMT_PAL_N,             /* Jamaica, Uruguay */
	EM_CSMPVIDEO_FMT_PAL_NC,            /* N combination (Argentina) */
	EM_CSMPVIDEO_FMT_SECAM,             /* LDK/SECAM (France,Russia) */
	EM_CSMPVIDEO_FMT_1080i,             /* HD 1080i */
	EM_CSMPVIDEO_FMT_1080p,             /* HD 1080p 60/59.94Hz, SMPTE 274M-1998 */
	EM_CSMPVIDEO_FMT_720p,              /* HD 720p */
	EM_CSMPVIDEO_FMT_480p,              /* HD 480p */
	EM_CSMPVIDEO_FMT_1080i_50Hz,        /* HD 1080i 50Hz, 1125 sample per line, SMPTE 274M */
	EM_CSMPVIDEO_FMT_1080p_24Hz,        /* HD 1080p 24Hz, 2750 sample per line, SMPTE 274M-1998 */
	EM_CSMPVIDEO_FMT_1080p_25Hz,        /* HD 1080p 25Hz, 2640 sample per line, SMPTE 274M-1998 */
	EM_CSMPVIDEO_FMT_1080p_30Hz,        /* HD 1080p 30Hz, 2200 sample per line, SMPTE 274M-1998 */
	EM_CSMPVIDEO_FMT_1250i_50Hz,        /* HD 1250i 50Hz, another 1080i_50hz standard SMPTE 295M */
	EM_CSMPVIDEO_FMT_720p_24Hz,         /* HD 720p 23.976/24Hz, 750 line, SMPTE 296M */
	EM_CSMPVIDEO_FMT_720p_50Hz,         /* HD 720p 50Hz (Australia) */
	EM_CSMPVIDEO_FMT_576p_50Hz,         /* HD 576p 50Hz (Australia) */
	EM_CSMPVIDEO_FMT_CUSTOM_1440x240p_60Hz,  /* 240p 60Hz 7411 custom format. */
	EM_CSMPVIDEO_FMT_CUSTOM_1440x288p_50Hz,  /* 288p 50Hz 7411 custom format. */
	EM_CSMPVIDEO_FMT_CUSTOM_1366x768p,       /* Custom 1366x768 mode */
	EM_CSMPVIDEO_FMT_CUSTOM_1366x768p_50Hz,  /* Custom 1366x768 50Hz mode */
	EM_CSMPVIDEO_FMT_DVI_640x480p,       /* DVI Safe mode for computer monitors */
	EM_CSMPVIDEO_FMT_DVI_800x600p,       /* DVI VESA mode for computer monitors */
	EM_CSMPVIDEO_FMT_DVI_1024x768p,      /* DVI VESA mode for computer monitors */
	EM_CSMPVIDEO_FMT_DVI_1280x768p,      /* DVI VESA mode for computer monitors */
	EM_CSMPVIDEO_FMT_DVI_1280x720p_50Hz, /* DVI VESA mode for computer monitors */
	EM_CSMPVIDEO_FMT_DVI_1280x720p,      /* DVI VESA mode for computer monitors */
	EM_CSMPVIDEO_FMT_DVI_1280x720p_ReducedBlank, /* DVI VESA mode for computer monitors */
	EM_CSMPVIDEO_FMT_NTSC_443,			/* NTSC M mode but with 4.43361875MHz co */
	EM_CSMPVIDEO_FMT_1080p_50Hz,        /* HD 1080 Progressive, 50Hz */
	EM_CSMPVIDEO_FMT_576p,              /* Australian mode 	 */
	EM_CSMPVIDEO_FMT_MaxCount            /* Counter. Do not use! */
}CSMPVdisVideoFormat_E;

typedef enum CSMPAspectRatio_E
{
	EM_CSMPWIN_ASPECT_RATIO_UNKNOWN = 0,  
	EM_CSMPWIN_ASPECT_RATIO_4_3,  
	EM_CSMPWIN_ASPECT_RATIO_16_9,
	EM_CSMPWIN_ASPECT_RATIO_AUTO   /*  自动选择 */
}CSMPAspectRatio_E;

typedef enum CSMPOutput_E
{
	EM_CSMPOUTPUT_HD = 0,	/* HD（高清）,HDMI, YPbPr */
	EM_CSMPOUTPUT_SD      	/* SD（标清）,CVBS,S-VIDEO */
}CSMPOutput_E;

typedef enum CSMPStopMode_E	
{	
	EM_CSMP_STOPMODE_BLACK,			/*black screen*/
	EM_CSMP_STOPMODE_FREEZE,			/*freeze last frame*/	
	EM_CSMP_STOPMODE_FIFO				/*slow black*/
}CSMPStopMode_E;

typedef struct CSMPRect_S
{
	DWORD dwX;            	/*X坐标值*/
	DWORD dwY;           	/*Y坐标值*/
	DWORD dwWidth;        	/*宽*/
	DWORD dwHeight; 		/*高*/
}CSMPRect_S;

typedef enum CSMPVdisWinParam_E  
{
		EM_CSMP_VDIS_WIN_BRIGHTNESS = 0,     /*亮度*/
		EM_CSMP_VDIS_WIN_CONTRAST,           /*对比度*/
		EM_CSMP_VDIS_WIN_SATURATION,        /*饱和度*/
		EM_CSMP_VDIS_WIN_ALPHA
}CSMPVdisWinParam_E;

typedef enum CSMPAUDChannelType_E
{
  	EM_CSMPAUD_CHANNEL_STEREO,
  	EM_CSMPAUD_CHANNEL_LEFT,
  	EM_CSMPAUD_CHANNEL_RIGHT,
  	EM_CSMPAUD_CHANNEL_MIXED_MONO
}CSMPAUDChannelType_E;	


typedef struct CSMPVideoParams_S
{
	WORD  wVideoPID;              /*视频PID*/
	BYTE   ucVideoFormat;          	/*视频格式*/
}CSMPVideoParams_S;


typedef struct CSMPAudioParams_S
{
	WORD  wAudioPID;                /*音频PID*/
	BYTE  ucAudioFormat;           /*音频格式*/
	BYTE  ucLanguageCode[4];        /*语言描述符*/   
	BYTE  ucRawFormat;             /*原始音频类型，用于区别带有不同描述符的AC3类型*/
}CSMPAudioParams_S; 
/*subtitle,teletext结构定义*/
typedef struct CSSubtitleEntity_S
{
    BYTE   aucLanguage[3];
    BYTE   ucSubtitleType;
    WORD   wCompPageID;
    WORD   wAnciPageID;
}CSSubtitleEntity_S;

typedef struct CSSubtitileParams_S
{
    BYTE    ucStreamType;
    WORD    wSubtitlePID;
    BYTE    ucDescriptorCount;
    CSSubtitleEntity_S sDescriptor[CS_SUBTITLE_PER_STREAM];
}CSSubtitileParams_S;

typedef struct CSTeletextEntity_S
{
    BYTE   aucLanguage[3];
    BYTE    ucTeletextType;
    BYTE    ucTeleMagNum;
    BYTE    ucTelePageNum;
}CSTeletextEntity_S;

typedef struct CSTeletextParams_S
{
    BYTE    ucStreamType;
    WORD    wTeletextPID;
    BYTE    ucDescriptorCount;
    CSTeletextEntity_S sDescriptor[CS_TELETEXT_PER_STREAM];
}CSTeletextParams_S;
typedef struct CSMPProgrameInfo_S
{
    WORD	wPCRPID;
    WORD	wVideoCount;
	WORD    wVideoIndex;
    CSMPVideoParams_S	asVideo[CS_VIDEOCOUNT_PER_PROG];
    WORD	wAudioCount;
	WORD    wAudioIndex;
    CSMPAudioParams_S	asAudio[CS_AUDIOCOUNT_PER_PROG];
    WORD	wSubtitleCount;
	WORD    wSubtitleIndex;	
    CSSubtitileParams_S	asSubtitle[CS_SUBTITLECOUNT_PER_PROG];
    WORD	wTeletextCount;
	WORD    wTeletextIndex;	
    CSTeletextParams_S	asTeletext[CS_TELETEXTCOUNT_PER_PROG];
}CSMPProgrameInfo_S;

typedef struct _CSMPStreamStatus_S
{
	BOOL 			bWindowEnable;	    /**< 视频显示窗口是否打开*/	
	int 			nSourceWidth;		    /**< 源视频流宽度*/
	int 			nSourceHight;		    /**< 源视频流高度*/
	int 			nFrameRate;		    /**< 源视频流码率*/
	BOOL			bInterlaced;		    /**< TRUE表示隔行;FALSE表示逐行*/
	CSMPVdisVideoFormat_E		eTimingModeSD;	    /**< 标清设备视频输出分辨率*/
	CSMPVdisVideoFormat_E		eTimingModeHD;	    /**< 高清设备视频输出分辨率*/
	DWORD			dwFrameCount;		        /**< 视频已播放的总帧数*/
}CSMPStreamStatus_S;
//lzh add 
/*Video Output Aspect Ratio ConversionType*/
typedef enum _CSMPMatchMethod
{
	EM_CSMP_LETTER_BOX,	/*Letter Box*/
	EM_CSMP_PAN_SCAN,		/*Pan Scan*/
	EM_CSMP_COMBINED,		/*ComBined*/
	EM_CSMP_IGNORE,		/*Ignore*/
	EM_CSMP_NUM
}CSMPMatchMethod;

typedef enum _CSMPEventMsg
{
	EM_CSMP_SYSTEM_RUNNING,		/*system run */
	EM_CSMP_SYSTEM_STANDBY,		/*system stanby */	
	EM_CSMP_SIGNAL_LOCK, 			/*tuner locked */
	EM_CSMP_SIGNAL_LOST,			/*tuner lost */	
	EM_CSMP_SMARTCARD_IN,  		/* insert smart card*/
	EM_CSMP_SMARTCARD_OUT,		/* remove smart card*/	
	EM_CSMP_PG_UNLOCK,			/*video decoder stop event*/
	EM_CSMP_PG_LOCK,			 	/*video decoder be running event*/
	EM_CSMP_SERVICE_NOLOCK,		/*8*/
	EM_CSMP_SERVICE_LOCK,			/*9*/
	EM_CSMP_START_PLAY, 			/*播放*/
	EM_CSMP_STOP_PLAY,
	EM_CSMP_VIDEO_IFRAME_SHOW,	/*video层显示Iframe*/
	EM_CSMP_VIDEO_IFRAME_HIDE,
	EM_CSMP_USER_OPEN,
	EM_CSMP_USER_CLOSE,
	EM_CSMP_FRONT_RUN,			        /**< front TS be running event*/
	EM_CSMP_FRONT_STOP,			        /**< front TS be stoped event*/
	EM_CSMP_PROGRAM_SWITCH_OPEN,
	EM_CSMP_PROGRAM_SWITCH_CLOSE,
	EM_CSMP_MAX
}CSMPEventMsg;

typedef enum _CSMPVideoSetType
{
	CS_VID_NORMAL = 0,	//正在播放当前节目
	CS_VID_BLACK,  		//黑屏
	CS_VID_FREEZE	//静帧
}CSMPVideoSetType;

typedef enum _CSMPAUDOutputDevice_e
{
	EM_CSMP_AUD_OUTPUT_RCA	=0x00000001,	/**< 常规音频左右声道输出接口*/
	EM_CSMP_AUD_OUTPUT_SPDIF	=0x00000002,	/**< 数字音频接口*/
	EM_CSMP_AUD_OUTPUT_RF		=0x00000004,	/**< RF调制输出*/
	EM_CSMP_AUD_OUTPUT_SCART	=0x00000008,	/**< scart接口*/
	EM_CSMP_AUD_OUTPUT_HDMI  	=0x00000010	    /**< hdmi 接口*/
}CSMPAUDOutputDevice_E;	


typedef enum _CSMP_AUDIO_OUTPUT_DATA_FORMAT_e
{
    EM_CSMP_AUDIO_OUTPUT_DECODE = 0,
    EM_CSMP_AUDIO_OUTPUT_BYPASS,
    EM_CSMP_AUDIO_OUTPUT_AUTO
}CSMP_AUDIO_OUTPUT_DATA_FORMAT_E;

typedef enum CSMediaPlayerCaStatus_E
{
	EM_CSMP_CA_AV_OK,//免费节目或加密节目解扰成功
	EM_CSMP_CA_SMARTCARD_PLUGOUT,
	EM_CSMP_CA_DESCAMBLED,
	EM_CSMP_CA_OTHER_ENCRYPT,
	EM_CSMP_CA_NOT_AUTHORIZED
}CSMediaPlayerCaStatus_E;

/**@brief 视频输出设备类型*/
typedef enum _CSMPVIDOUTDevice_e
{
	EM_CSMP_VID_OUTPUT_CVBS,	/**< CSBVS视频输出接口 */
	EM_CSMP_VID_OUTPUT_YC,	/**< S-Video 视频输出接口 */
	EM_CSMP_VID_OUTPUT_YUV, /**< 分量视频输出接口 */
	EM_CSMP_VID_OUTPUT_RF, /**< 射频信号视频输出接口 */
	EM_CSMP_VID_OUTPUT_SCART, /**< SCART视频输出接口 */
	EM_CSMP_VID_OUTPUT_HDMI, /**< HDMI视频输出接口 */
	EM_CSMP_VID_OUTPUT_DVO,  /**< 数字视频输出*/
	EM_CSMP_VID_OUTPUT_NUM /**< 视频接口个数 */
}CSMPVIDOUTDevice_E;

/**@brief 视频输出端口的颜色类型 */
typedef enum _CSMPVIDOutputType_e
{
	EM_CSMP_VID_OUTPUT_TYPE_YUV, /**< YUV模式 */
	EM_CSMP_VID_OUTPUT_TYPE_RGB, 	/**< RGB模式 */
	EM_CSMP_VID_OUTPUT_TYPE_NUM	 /**< 模式计数 */
}CSMPVIDOutputType_E;


typedef enum _CSMPOptType_E
{               
    EM_CSMP_OPTTYPE_STREAMTYPE = 0,  ///< 设置PMT表中StreamType 所表示类型，pValue值为 int，pValue 值为0时，PMT表中StreamType=0x82 表示流类型为CSMP_AUDIO_CODEC_DTS，
                                    ///< pValue 值为1时，PMT表中StreamType=0x82 表示流类型为为DCII subtitle    

	EM_CSMP_OPTTYPE_VALIDVIDEOFRAME ,     ///< 设置视频播放从第几帧开始，pValue值为 int，一般情况不需设置，
									///< 如果要求每次都从解出的第几帧开始播放时，可以使用此选项
	EM_CSMP_OPTTYPE_VALIDAUDIOFRAME      ///< 设置音频播放从第几帧开始，pValue值为 int，一般情况不需设置，
									///< 如果要求每次都从解出的第几帧开始播放时，可以使用此选项

}CSMPOptType_E;     ///< Mediaplayer 配置项枚举类型

/**@brief 流类型*/
typedef enum 
{
	EM_CSMP_CONTENT_VIDEO = 0,		///< 视频类型	 						
	EM_CSMP_CONTENT_AUDIO, 			///< 音频类型						
	EM_CSMP_CONTENT_SUBTITLE,    	///< 字幕类型
	EM_CSMP_CONTENT_TELTEX, 		///< 图文电视类型
	EM_CSMP_CONTENT_PCR 			///< PCR类型	
}CSMPContentType_E;

/******************************************************************************
 *                                 Global Function Declare                     *
 ******************************************************************************/
 /******************************************************************************
Function Name:    FOnCSMPHaveMsgToUser

Description:
	用户回调,mediaplayer 把消息返回给用户

Input:
    nUserData : 用户数据
    nMsg : 消息类型
    dwPara1 : 	参数1 
    dwPara2 :	参数2 
    
Output:

Return Value:
    success : 	EM_MP_SUCCESS
	fail :		EM_MP_FAILURE

Other:
*******************************************************************************/
typedef int (*FOnCSMPHaveMsgToUser)(DWORD   nUserData,
										 int   nMsg,
										 DWORD dwPara1,
										 DWORD dwPara2);

/******************************************************************************
Function Name:    CSMPCreate

Description:
        create the  mediaplayer 
        the function  CSPlayerCreate create the main thread and related 
        resource.

Input:
    fnCallback : the callback function to transfer message.
    pnUserData : the parameter of the callback.
    
Output:
    phMpHandle : the handle of mediaplay

Return Value:


Other:
*******************************************************************************/
int  CSMPCreate(FOnCSMPHaveMsgToUser 	fnCallback, 
					 DWORD                  dwUserData,
					 DWORD					dwThreadPriority,
					 DWORD					dwThreadStackSize,
					 HCSHANDLE             *phMpHandle);

/******************************************************************************
Function Name:    CSMPDestroy

Description:
        
       销毁mediaplayer ,释放相关资源 
Input:
    hPlayHandle  mediaplayer 句柄

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE
    

Other:
*******************************************************************************/
int  CSMPDestroy(HCSHANDLE hPlayHandle );


/******************************************************************************
Function Name:    CSMPSetDataSource

Description:
        设置数据源 ，设置数据源后，绑定一种播放方式:dtv ip  file

Input:
    hPlayHandle : 播放器句柄
    pszMRL :   媒体资源定位符
            "dtv://orinetworkid.tsid.serviceId"
		    "rtsp://192.168.1.10/boa.ts"
		    "igmp://224.0.0.20:2345"
		    "file://f:/video/h264/boa.ts"
		    本参数可以为空，当pszMRL为空时，则仅存储RM handle，不作为播放选择数据源使用
	dwDeviceId : 设备句柄
	hServiceHnadle:增加DM中的Service句柄

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPSetDataSource(HCSHANDLE        hPlayHandle,
							  const char      *pszMRL,
							  DWORD     dwDeviceId ,
							  HCSHANDLE hServiceHnadle);



/******************************************************************************
Function Name:    CSMPPlay

Description:
    启动媒体播放

Input:
    hPlayHandle : 播放器句柄
	hPumpHandle : pump模块句柄，供VOD控制使用
Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPPlay(HCSHANDLE hPlayHandle,HCSHANDLE hPumpHandle);


/******************************************************************************
Function Name:    CSMPStop

Description:
        停止媒体播放
Input:
    hPlayHandle : 播放器句柄
	nStopFlag :标识是否同步停止，0为同步停止，非0异步停止
Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPStop(HCSHANDLE hPlayHandle,WORD nStopFlag);
/******************************************************************************
Function Name:    CSMPChangePlay

Description:
        在切换节目时，调用CSMPChangePlay接口，本接口传入URL，
        下发命令后，如果正在播放，则不需要停止DECODE，
        而是通过重新设置音、视频的PID，来实现。
        但如果是从IP与DVB之间的切换，则还需要调用Play与stop操作 。
        当PSI发生突变时也调用本接口。但是可以不传入pszMRL，
        将其设置为NULL。
Input:
    hPlayHandle : 播放器句柄 
    pszMRL :   媒体资源定位符
            "dtv://orinetworkid.tsid.serviceId"
		    "rtsp://192.168.1.10/boa.ts"
		    "igmp://224.0.0.20:2345"
		    "file://f:/video/h264/boa.ts"
			dwDeviceId : 设备句柄
	hPumpHandle : pump模块句柄，供VOD控制使用
	bPMTChange :用来标示PMT突出，TRUE为突变，
				FALSE,正常PMT callback后调用
Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPChangePlay(HCSHANDLE hPlayHandle,const char *pszMRL,HCSHANDLE hPumpHandle,BOOL bPMTChange);

/******************************************************************************
Function Name:    CSMPSetRate

Description:
        设置播放速度
Input:
    hPlayHandle : 播放器句柄
    nRate : 速度值,设置的速率为实际播放速率的1000倍

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/

int  CSMPSetRate( HCSHANDLE  hPlayHandle , int  nRate );

/******************************************************************************
Function Name:    CSMPGetRate

Description:
        获取播放速度
Input:
    hPlayHandle : 播放器句柄
    nRate : 速度值

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetRate( HCSHANDLE  hPlayHandle , int  *pnRate );

/******************************************************************************
Function Name:    CSMPPause

Description:
        暂停媒体播放
Input:
    hPlayHandle : 播放器句柄

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPPause( HCSHANDLE  hPlayHandle );
/******************************************************************************
函数名称:    CSMPResume

描述:
    恢复播放当前节目.
    现只能用于本地文件和IP点播。
    异步调用，操作结果在CSMPCreate注册的回调中返回。

输入参数:
	hMpHandle : Mediaplay播放器句柄.
    
输出参数:
	无。
    
返回值:	
	EM_MP_SUCCESS : 函数成功
	EM_MP_FAILURE : 函数失败
	EM_MP_INVALPARA : 参数无效
	
其他:
*******************************************************************************/
int CSMPResume(HCSHANDLE hPlayHandle);

/******************************************************************************
Function Name:    CSMPSetCurrentTime

Description:
        设置播放启始时间 ，即SEEK操作
Input:
    hPlayHandle : 播放器句柄
    nSeconds : 播放进度,单位：秒

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPSetCurrentTime( HCSHANDLE hPlayHandle , DWORD nSeconds);

/******************************************************************************
Function Name:    CSMPSetCurrentTimeEx
 
Description:
        设置播放启始时间 ，即SEEK操作
Input:
    hPlayHandle : 播放器句柄
    nSeconds : 时间秒
    nType: 1表示NPT，2表示UTC，为时移所用
 
Output:
    None
 
Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE
 
Other:
*******************************************************************************/
int  CSMPSetCurrentTimeEx( HCSHANDLE  hPlayHandle ,DWORD nSeconds, int nType);
/******************************************************************************
Function Name:    CSMPGetCurrentTime

Description:
        得到当前时间 
Input:
    hPlayHandle : 播放器句柄
    nSeconds : 时间值 单位秒 

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPGetCurrentTime( HCSHANDLE  hPlayHandle,int *pnSeconds);
/******************************************************************************
Function Name:    CSMPSetDebugLevel

Description:
        设置调试级别
Input:
    hPlayHandle : 播放器句柄
    nLevel : 级别  详见:CSMP_DBG_Level_e

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPSetDebugLevel(HCSHANDLE  hPlayHandle,int nLevel);
/******************************************************************************
Function Name:    CSMPGetProgramInfo

Description:
       得到当前播放流的信息，包括音视频 等信息。 

Input:
    hPlayHandle : 播放器句柄
	psMediaInfo 当前播放流的信息
Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPGetProgramInfo( HCSHANDLE     hPlayHandle ,CSMPProgrameInfo_S  *psMediaInfo);
/******************************************************************************
Function Name:    CSMPSetAudioChannel

Description:
        设置当前的音频通道，包括左、右、立体声等。

Input:
    hPlayHandle：MediaPlay 句柄
	nChannel：音频声道，具体见CSMPAudioChan_E枚举。
	
		typedef enum CSMPAUDChannelType_E
		{
		  	EM_CSMPAUD_CHANNEL_STEREO,
		  	EM_CSMPAUD_CHANNEL_LEFT,
		  	EM_CSMPAUD_CHANNEL_RIGHT,
		  	EM_CSMPAUD_CHANNEL_MIXED_MONO
		}CSMPAUDChannelType_E;	
Output:
    None

Return Value:
    成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE


Other:
*******************************************************************************/
int  CSMPSetAudioChannel ( HCSHANDLE  hPlayHandle ,
        					int   nChannel );

/******************************************************************************
Function Name:    CSMPGetAudioChannel

Description:
        获取当前的音频通道，包括左、右、立体声等。

Input:
    hPlayHandle：MediaPlay 句柄
	
Output:
    pnChannel：保存获取到的音频声道，具体见CSMPAudioChan_E枚举。
	
		typedef enum CSMPAUDChannelType_E
		{
		  	EM_CSMPAUD_CHANNEL_STEREO,
		  	EM_CSMPAUD_CHANNEL_LEFT,
		  	EM_CSMPAUD_CHANNEL_RIGHT,
		  	EM_CSMPAUD_CHANNEL_MIXED_MONO
		}CSMPAUDChannelType_E;	

Return Value:
    成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE


Other:
*******************************************************************************/
int  CSMPGetAudioChannel ( HCSHANDLE hPlayHandle ,CSMPAUDChannelType_E* pnChannel );

/******************************************************************************
Function Name:    CSMPSetStopMode

Description:
	 换台时视频的处理类型:黑屏 静桢 淡入淡出


Input:
    hPlayHandle：MediaPlay 句柄
    nStopMode  换台时音视频的处理类型 CSMPStopMode

Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetStopMode( HCSHANDLE  hPlayHandle,  int nStopMode );

/******************************************************************************
Function Name:    CSMPGetStopMode

Description:
        获取换台时的处理类型

Input:
    hPlayHandle：MediaPlay 句柄

Output:
    pnStopMode  换台时音视频的处理类型 MPSwitchType_E

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetStopMode(HCSHANDLE	hPlayHandle ,int  *pnStopMode );

/******************************************************************************
Fuzhnction Name:    CSMPSetAudioSwitch

Description:
        设置播放的音频流。因为一路节目可能包含多个音频流，
        比如：国语流，粤语流，英语流等，可以根据用户的需求选择需要播出
        的音频流。

Input:
    	hPlayHandle：MediaPlay 句柄
    	wPID：音频PID索引。
		nType :音频类型，为proting层cs_sysaudio.h定于的类型
Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE
Other:
*******************************************************************************/
int  CSMPSwitchAudioPID ( HCSHANDLE  hPlayHandle,  WORD  wPID, WORD wType);

/******************************************************************************
Function Name:    CSMPSwitchVideoPID

Description:
       切换视频PID
       

Input:
    	hPlayHandle：MediaPlay 句柄
    	wPID：视频PID索引。
		nType :视频类型
Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
#include "cs_sysvideo.h"  
#include "cs_sysaudio.h"  
*******************************************************************************/
int  CSMPSwitchVideoPID ( HCSHANDLE  hPlayHandle,  WORD  wPID, WORD nType);


/******************************************************************************
Function Name:    CSMPSetVBITeletextEnable

Description:
       打开或关闭VBI Teletext显示
       

Input:
    	hPlayHandle：MediaPlay 句柄
    	bEnable: TRUE,打开VBI Teletext显示;FALSE,关闭VBI Teletext显示;
Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetVBITeletextEnable ( HCSHANDLE  hPlayHandle,  BOOL bEnable);


/******************************************************************************
Function Name:    CSMPGetDuration

Description:
         得到VOD 节目节目的长度 

Input:
    	hPlayHandle：MediaPlay 句柄

Output:
	pnSeconds :  时间长度值  单位秒

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetDuration( HCSHANDLE  hPlayHandle ,DWORD    *pdwSeconds);


/******************************************************************************
Function Name:    CSMPGetVolume

Description:
       得到音量值	 无实现

Input:
    	hPlayHandle：MediaPlay 句柄None
		pnVolume : 音量值
Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetVolume( HCSHANDLE  hPlayHandle , int *pnVolume );

/******************************************************************************
Function Name:    CSMPGetVolume

Description:
       设置音量值

Input:
    	hPlayHandle：MediaPlay 句柄
		nVolume : 音量值 用户需要输出的音量值，按[0,31]阶音量计算；
Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetVolume( HCSHANDLE  hPlayHandle , int  nVolume );

/******************************************************************************
Function Name:    CSMPSetVolumeMute

Description:
       设置静音
Input:
    	hPlayHandle：MediaPlay 句柄
    	ntype : 静音标志　CSMPMute_E

Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetVolumeMute( HCSHANDLE  hPlayHandle ,int nType) ;//menu 


/******************************************************************************
Function Name:    CSMPSetAOutMute

Description:
       设置全局静音，指定音频输出通道为静音
Input:
    	hPlayHandle：MediaPlay 句柄
    	eAudioDevice :音频输出设备，见CSMPAUDOutputDevice_E

Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE
	不支持返回EM_MP_NONSUPPORT
	
Other: UDI1.0平台下返回EM_MP_NONSUPPORT
*******************************************************************************/
int  CSMPSetAOutMute( HCSHANDLE  hPlayHandle ,CSMPAUDOutputDevice_E eAudioDevice) ;


/******************************************************************************
Function Name:    CSMPSetAOutUnMute

Description:
       设置全局静音，取消指定音频输出通道的静音状态
Input:
    	hPlayHandle：MediaPlay 句柄
    	eAudioDevice :音频输出设备，见CSMPAUDOutputDevice_E

Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE
	不支持返回EM_MP_NONSUPPORT
	
Other:UDI1.0平台下返回EM_MP_NONSUPPORT
*******************************************************************************/
int  CSMPSetAOutUnMute( HCSHANDLE  hPlayHandle ,CSMPAUDOutputDevice_E eAudioDevice) ;


/******************************************************************************
Function Name:    CSMPGetVolumeMute

Description:
       得到静音标志

Input:
    	hPlayHandle：MediaPlay 句柄
    	ntype : 静音标志　CSMPMute_E

Output:
      	ntype : 静音标志　CSMPMute_E
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetVolumeMute( HCSHANDLE  hPlayHandle ,int *pnType);

/******************************************************************************
Function Name:    CSMPSetVideoFormat

Description:
       设置视频输出制式。

Input:
		hPlayHandle：MediaPlay 句柄
		nVideoFormat	：视频制式，参见定义见CSMPVdisVideoFormat_E
Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
typedef enum _CSVIDOutputDevice
{
	CSVID_OUTPUT_CVBS		=0x00000001,		video cvbs BNC output interface 
	CSVID_OUTPUT_YC			=0x00000002,		    video S-VHS output interface
	CSVID_OUTPUT_RGB		=0x00000004,		video R,G, B, component output 														
	                                              interface
	CSVID_OUTPUT_YUV		=0x00000008,		video Y, Cb, Cr output interface
	CSVID_OUTPUT_RF			=0x00000010,		Reserve
	CSVID_OUTPUT_SCART		=0x00000020,		Reserve
	CSVID_OUTPUT_HDMI		=0x00000040		hdmi interface
}CSVIDOutputDevice;	

*******************************************************************************/
int  CSMPSetVideoFormat( HCSHANDLE  hPlayHandle,int nVideoFormat);

/******************************************************************************
Function Name:    CSMPSetWinClip

Description:
       设置窗口剪切位置和尺寸。
Input:
    	hPlayHandle：MediaPlay 句柄
    	psRect :屏幕显示位置及尺寸SMPRect_S
Output:
　　　None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetWinClip( int hPlayHandle,const CSMPRect_S *psRect );


/******************************************************************************
Function Name:    CSMPSetWinRect

Description:
        设置屏幕显示位置及尺寸。

Input:
    	hPlayHandle：MediaPlay 句柄
    	psRect :屏幕显示位置及尺寸SMPRect_S
Output:
　　　None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
	typedef enum _CSVIDWindow
	{
		CSVID_WINDOW_VIDEO,				video window
		CSVID_WINDOW_OSD,				osd window
		CSVID_WINDOW_BACKGROUND,		background window
		CSVID_WINDOW_ALLWINDOW			all window
	}CSVIDWindow;
	typedef struct _CSWINRECT
	{
		LONG    left;
		LONG    top;
		LONG    right;
		LONG    bottom;
	} CSWINRECT;


*******************************************************************************/
int  CSMPSetWinRect(HCSHANDLE  hPlayHandle ,const CSMPRect_S  *psRect ) ;

/******************************************************************************
Function Name:    CSMPSetWinParams

Description:
       设置窗口的属性值，如亮度、对比度、色度、锐度等

Input:
    hPlayHandle：MediaPlay 句柄
    nWinParam   属性名
    dwValue　　属性值

Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
		typedef enum _CSMPVdisWinParam_E  
		{
				EM_CSMP_VDIS_WIN_BRIGHTNESS = 0,     亮度
				EM_CSMP_VDIS_WIN_CONTRAST,           对比度
				EM_CSMP_VDIS_WIN_SATURATION,         饱和度
		}CSMPVdisWinParam_E;
*******************************************************************************/
int  CSMPSetWinParams( HCSHANDLE 	hPlayHandle , 
						int            nWinParam ,
						DWORD      dwValue);


/******************************************************************************
Function Name:    CSMPSetAspectRatio

Description:
       设置视频输出窗口显示比率。

Input:
    nRatio :视频输出窗口显示比率。

Output:
    None

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
typedef enum CSMPAspectRatio_E
{
	EM_CSMPWIN_ASPECT_RATIO_4_3 = 0,  
	EM_CSMPWIN_ASPECT_RATIO_16_9,
	EM_CSMPWIN_ASPECT_RATIO_AUTO   
}CSMPAspectRatio_E;

*******************************************************************************/
int  CSMPSetAspectRatio( HCSHANDLE  hPlayHandle ,
						int           nRatio);


/******************************************************************************
Function Name:    CSMPGetServiceType

Description:
        得到服务类型　见　CSMPDSType_E

Input:
        hPlayHandle：MediaPlay 句柄

Output:
    pnType :得到服务类型  

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
	typedef enum 
	{
		EM_PLAYER_UNSUPPORT = 0,	    UNSUPPORT 
		EM_PLAYER_IP ,	     IP 
	    EM_PLAYER_DVB,	     DVB 
	    EM_PLAYER_FILE,  	    FILE 
	    EM_PLAYER_IGMP,
	    EM_PLAYER_RTSP
	 }CSMPDSType_E;
*******************************************************************************/
int CSMPGetContentType(HCSHANDLE  hPlayHandle , int *pnType);
/******************************************************************************
Function Name:    CSMPGetStatus

Description:
       获得播放器当前的状态信息

Input:
    hPlayHandle：MediaPlay 句柄

Output:
    pnStatus :函数成功后返回播放器状态，参见定义CSMPState_E

Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:

*******************************************************************************/
int  CSMPGetStatus( HCSHANDLE  hPlayHandle , int   *pnStatus );
/******************************************************************************
Function Name:    CSMPFreeze

Description:
      仅使画面静止在当前播放的帧，不能使播放暂停。

Input:
    hPlayHandle：MediaPlay 句柄
Output:
    None
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPFreeze(HCSHANDLE  hPlayHandle);
/******************************************************************************
Function Name:    CSMPUnFreeze

Description:
     取消画面静止，继续播放。

Input:
    hPlayHandle：MediaPlay 句柄
Output:
    None
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPUnFreeze(HCSHANDLE  hPlayHandle);

/******************************************************************************
Function Name:    CSMPPVMStreamHandle

Description:
     获取PVR模块stream的句柄
		同步操作
Input:
    hPlayHandle：MediaPlay 句柄
Output:
    None
Return Value:
	成功返回句柄；
	失败返回 0；
Other:
*******************************************************************************/
HCSHANDLE  CSMPGetPVMStreamHandle (HCSHANDLE  hPlayHandle);

/******************************************************************************
Function Name:    CSMPCallbackToLive

Description:
         本接口调用ippump的disconnect接口，下发teardown命令，
         停掉与服务器的交互，同时下发停止decode命令，然后通知selection
         回放已经停掉，由selection去启动直播。
Input:
    hPlayHandle : 播放器句柄

Output:
    None

Return Value:
    成功 EM_MP_SUCCESS
    失败 EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPCallbackToLive( HCSHANDLE  hPlayHandle );

/******************************************************************************
Function Name:    CSMPSetMatchMethod
Description:
       设置窗口的pan scan等格式
Input:
    nMatchMethod，参见枚举CSMPMatchMethod  
	typedef enum _CSMPMatchMethod
{
	CSVID_MATCH_METHOD_LETTER_BOX,	//Letter Box
	CSVID_MATCH_METHOD_PAN_SCAN,	//Pan Scan
	CSVID_MATCH_METHOD_COMBINED,	//ComBined
	CSVID_MATCH_METHOD_IGNORE,		//Ignore
	CSVID_MATCH_METHOD_NUM
}CSMPMatchMethod;
Output:
    None
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE
Other:

*******************************************************************************/
int  CSMPSetMatchMethod( HCSHANDLE  hPlayHandle,int nMatchMethod);

/******************************************************************************
Function Name:    CSMPSetVideo
Description:
       设置窗口的pan scan等格式
Input:
    nMsg，参见枚举CSMPEventMsg  
	typedef enum _CSMPEventMsg
	{
		EM_CSMP_SYSTEM_RUNNING,		//system run 
		EM_CSMP_SYSTEM_STANDBY,		//system stanby 
		EM_CSMP_SIGNAL_LOCK, 		//tuner locked 
		EM_CSMP_SIGNAL_LOST,			//tuner lost 
		EM_CSMP_SMARTCARD_IN,  		// insert smart card
		EM_CSMP_SMARTCARD_OUT,		// remove smart card
		EM_CSMP_PG_UNLOCK,			//video decoder stop event
		EM_CSMP_PG_LOCK,			 	//video decoder be running event
		EM_CSMP_SERVICE_NOLOCK,					//12
		EM_CSMP_SERVICE_LOCK,						//13
		EM_CSMP_START_PLAY, //播放
		EM_CSMP_STOP_PLAY,
		EM_CSMP_VIDEO_IFRAME_SHOW,//video层显示Iframe
		EM_CSMP_VIDEO_IFRAME_HIDE,
		EM_CSMP_MAX
	}CSMPEventMsg;
	
    nType: 参见枚举CSMPVideoSetType
    	typedef enum _CSMPVideoSetType
	{
		CS_MODE_NORMAL = 0,	//正在播放当前节目
		CS_MODE_BLACK,  		//黑屏
		CS_MODE_FREEZE	//静帧
	}CSMPVideoSetType;
Output:
    None
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE
Other:

*******************************************************************************/
int CSMPSetVideo(HCSHANDLE hPlayHandle, int nMsg, int nType);

/******************************************************************************
Function Name:    CSMPSetVideo
Description:
       设置窗口的pan scan等格式
Input:
    nMsg，参见枚举CSMPEventMsg  
	typedef enum _CSMPEventMsg
	{
		EM_CSMP_SYSTEM_RUNNING,		//system run 
		EM_CSMP_SYSTEM_STANDBY,		//system stanby 
		EM_CSMP_SIGNAL_LOCK, 		//tuner locked 
		EM_CSMP_SIGNAL_LOST,			//tuner lost 
		EM_CSMP_SMARTCARD_IN,  		// insert smart card
		EM_CSMP_SMARTCARD_OUT,		// remove smart card
		EM_CSMP_PG_UNLOCK,			//video decoder stop event
		EM_CSMP_PG_LOCK,			 	//video decoder be running event
		EM_CSMP_SERVICE_NOLOCK,					//12
		EM_CSMP_SERVICE_LOCK,						//13
		EM_CSMP_START_PLAY, //播放
		EM_CSMP_STOP_PLAY,
		EM_CSMP_VIDEO_IFRAME_SHOW,//video层显示Iframe
		EM_CSMP_VIDEO_IFRAME_HIDE,
		CSSYSEVENT_USER_OPEN,		
		CSSYSEVENT_USER_CLOSE,
		EM_CSMP_MAX
	}CSMPEventMsg;
	
    nType: 参见枚举CSMPMute_E
    	typedef enum CSMPMute_E
	{	
		EM_MPUnMute = 0,	    
		EM_MPMute 
	}CSMPMute_E;
Output:
    None
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE
Other:

*******************************************************************************/
int CSMPSetAudio(HCSHANDLE hPlayHandle, int nMsg, int nType);


/******************************************************************************
Function Name:    CSMediaplyerGetVersion

Description:
        获取mediaplay版本号

Input:
    	无

Output:
	无

Return Value:
	返回mediaplayer版本号，为字符串

Other:
*******************************************************************************/
char* CSMediaplyerGetVersion(void);
/******************************************************************************
Function Name:    CSIPPUMPIsSupportTrickMode

Description:
        获取mediaplay版本号

Input:
    	无

Output:
	无

Return Value:
	支持返回EM_MP_SUCCESS；
	否则返回EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPIsSupportTrickMode(HCSHANDLE  hPlayHandle);
/******************************************************************************
Function Name:    CSMPGetStreamStatus

Description:
        获取码流信息

Input:
    hPlayHandle:player句柄

Output:
	psStreamStatus:码流信息存储空间

Return Value:
	支持返回EM_MP_SUCCESS；
	否则返回EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPGetStreamStatus(HCSHANDLE  hPlayHandle,CSMPStreamStatus_S *psStreamStatus);

/******************************************************************************
Function Name:    CSMPGetMatchMethod

Description:
        获取屏幕适配方式

Input:
    hPlayHandle:player句柄

Output:
	pnMatchMethod:屏幕适配方式的存储空间

Return Value:
	支持返回EM_MP_SUCCESS；
	否则返回EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPGetMatchMethod(HCSHANDLE  hPlayHandle,int *pnMatchMethod );

int  CSMPSaveBookMark( HCSHANDLE hPlayHandle ,DWORD pdwSeconds);
int  CSMPGetStartTime( HCSHANDLE hPlayHandle ,DWORD *pdwSeconds);
/******************************************************************************
Function Name:    CSMPSetIppumpCallback

Description:
        通知MEDIAPLAYER向IPPUMP注册回调函数

Input:
    	hPlayHandle:MediaPlay 句柄
	hPumpHandle:ippump句柄
Output:
	无

Return Value:
	支持返回EM_MP_SUCCESS；
	否则返回EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPSetIppumpCallback(HCSHANDLE hPlayHandle,HCSHANDLE hPumpHandle);

/******************************************************************************
Function Name:    CSMPSetOutputDataFormat

Description:
        设置音频输出设备的输出数据格式（压缩或非压缩格式）
主要用来设置AC3等音频格式是否要在机顶盒里进行解码，还是直接输出到音响等设备以获得更好的效果
Input:
    	mpHandle底层设备操作句柄
	nDevice音频输出端口
	nFormat可以取CSMP_AUDIO_OUTPUT_DATA_FORMAT_E的三个值

	typedef enum _CSMP_AUDIO_OUTPUT_DATA_FORMAT_e
   {
    EM_CSMP_AUDIO_OUTPUT_DECODE = 0,
    EM_CSMP_AUDIO_OUTPUT_BYPASS,
    EM_CSMP_AUDIO_OUTPUT_AUTO
   }CSMP_AUDIO_OUTPUT_DATA_FORMAT_E;

	nFormat = EM_CSMP_AUDIO_OUTPUT_AUTO时，如果码流音频格式是AC3或AC3_PLUS,则由音响设备来解码，如果是PCM则由机顶盒解码
	nFormat = EM_CSMP_AUDIO_OUTPUT_DECODE时，所有音频都强制由机顶盒解码
	nFormat = EM_CSMP_AUDIO_OUTPUT_BYPASS时，所有音视频都强制由音响设备来解码
Output:
	无

Return Value:
     失败:EM_RET_FAILURE，成功:EM_RET_SUCCESS
Other:
*******************************************************************************/
int  CSMPSetOutputDataFormat(HCSHANDLE mpHandle, int nDevice, int nFormat);

/******************************************************************************
Function Name:    CSMPGetOutputDataFormat

Description:
        获取音频输出设备的输出数据格式（压缩或非压缩格式）

Input:
    	mpHandle底层设备操作句柄
	nDevice音频输出端口
Output:
	pFormat:音频输出格式

Return Value:
      失败:EM_RET_FAILURE，成功:EM_RET_SUCCESS
Other:
*******************************************************************************/
int  CSMPGetOutputDataFormat(HCSHANDLE mpHandle, int nDevice, int *pFormat);

/******************************************************************************
Function Name:    CSMPGetEncryptProgramSwitch

Description:
        获取加密节目播放前是否需要停止视音频标志

Input:
    	mpHandle底层设备操作句柄
Output:
	pbSwitch:视音频开关标志

Return Value:
      失败:EM_RET_FAILURE，成功:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPGetEncryptProgramSwitch(HCSHANDLE mpHandle, BOOL *pbSwitch);

/******************************************************************************
Function Name:    CSMPGetEncryptProgramSwitch

Description:
        设置加密节目播放前是否需要停止视音频标志

Input:
    	mpHandle底层设备操作句柄
    	bSwitch:视音频开关标志
Output:
	

Return Value:
      失败:EM_RET_FAILURE，成功:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPSetEncryptProgramSwitch(HCSHANDLE mpHandle, BOOL bSwitch);

/******************************************************************************
Function Name:    CSMPGetEncryptProgramSwitch

Description:
       获取节目停止时视频的显示类型
Input:
    	mpHandle底层设备操作句柄
Output:
    	peVideoType:视频显示类型

Return Value:
      失败:EM_RET_FAILURE，成功:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPGetVideoShowType(HCSHANDLE mpHandle, CSMPVideoSetType *peVideoType);

/******************************************************************************
Function Name:    CSMPSetReferenceWinBounds

Description:
        设置视频窗口的大小

Input:
    	hPlayHandle底层设备操作句柄
    	nWidth:视频窗口的宽
    	nHeight:视频窗口的高
Output:
	
Return Value:
      失败:EM_RET_FAILURE，成功:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPSetReferenceWinBounds(HCSHANDLE hPlayHandle, int nWidth, int nHeight);

/******************************************************************************
Function Name:    CSMPSetSmartCardState

Description:
        设置智能卡状态，此接口目前只对前端停播的检测有影响

Input:
    	hPlayHandle		播放器句柄 
    	eState			智能卡状态,EM_CSMP_CA_AV_OK代表卡音视频能播放，其他代表不能播放原因
Output:
	
Return Value:
      	失败:EM_RET_FAILURE，成功:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPSetSmartCardState(HCSHANDLE hPlayHandle,CSMediaPlayerCaStatus_E eState);

/******************************************************************************
Function Name:    CSMPClearDecoder

Description:
       清除播放器中缓存的数据

Input:
    	hPlayHandle：MediaPlay 句柄

Output:
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
本接口不改变Player状态，仅清除缓冲数据，若仍在播放状态，则新数据会重新开始播放
*******************************************************************************/
int CSMPClearDecoder(HCSHANDLE hPlayHandle);


/******************************************************************************
Function Name:    CSMPSetVOutType

Description:
       设置视频输出端口的颜色类型

Input:
    	hPlayHandle：MediaPlay 句柄
        eVoutDevice:    视频输出设备,只支持分量(EM_CSMP_VID_OUTPUT_YUV)/(EM_CSMP_VID_OUTPUT_HDMI)
        eType:              输出的颜色类型，RGB或是YUV
Output:
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPSetVOutType(HCSHANDLE hPlayHandle,CSMPVIDOUTDevice_E eVoutDevice, CSMPVIDOutputType_E eType);




/******************************************************************************
Function Name:	  CSMPGetIppluginMessage

Description:
	  根据MASKID 从VOD 插件中获取消息内容
Input:
		hPlayHandle：MediaPlay 句柄
		nMaskId: 
		pcOutMessage: 存放消息内容的缓冲区
		nMessageBufLen: 存放消息内容的缓冲区 大小
Output:
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/

int CSMPGetIppluginMessage(HCSHANDLE hPlayHandle,int nMaskId, char *pcOutMessage,int nMessageBufLen);


/******************************************************************************
Function Name:	  CSMPSetDefaultAVPid

Description:
	  设置默认的音视频PID
Input:
		hPlayHandle：MediaPlay 句柄
		nAudioPid: 音频PID
	       nVideoPid:视频PID

Output:
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/

int CSMPSetDefaultAVPid(HCSHANDLE hPlayHandle,int nAudioPid,int nVideoPid);

/******************************************************************************
Function Name:	  CSMPSetAudioControlMode

Description:
	  设置音量的控制模式
Input:
		
      nMode :0 全局控制，默认
      nMode :1 单独控制
Output:
Return Value:
	成功返回EM_MP_SUCCESS；
	失败返回EM_MP_FAILURE

Other:
*******************************************************************************/

int CSMPSetAudioControlMode(int nMode);

/******************************************************************************
Function Name:	  CSMPSetSelection
 
Description:
		
	   设置 MP 的Selection句柄
Input:
	HCSHANDLE hPlayHandle
	HCSHANDLE hSelection
 
Output:
	None
 
Return Value:
	成功 EM_MP_SUCCESS
	失败 EM_MP_FAILURE
	
 
Other:
*******************************************************************************/
int CSMPSetSelection(HCSHANDLE hPlayHandle, HCSHANDLE hSelection);

/******************************************************************************
Function Name:	  CSMPGetCurrentPTS
 
Description:		
	   根据当前mediaplayer的实例,获取当前节目的PTS
Input:
	HCSHANDLE hPlayHandle:MediaPlay 句柄
 
Output:
	CSUDI_UINT64 *psPTS:输出当前节目的pts
 
Return Value:
	成功 EM_MP_SUCCESS
	失败 EM_MP_FAILURE
	参数为空 EM_MP_INVALPARA
 
Other:注意通过此接口只能获取PCR的PTS
*******************************************************************************/
int CSMPGetCurrentPTS(HCSHANDLE hPlayHandle, CSUDI_UINT64 *psPTS);

/******************************************************************************
Function Name:	  CSMPGetCurrentPTSByType
 
Description:		
	   根据当前mediaplayer的实例,获取当前节目指定流类型的PTS
Input:
	hPlayHandle:MediaPlay 句柄
	eContentType: 流类型
 
Output:
	pulPTS:输出当前节目指定流类型的pts
 
Return Value:
	成功 EM_MP_SUCCESS
	失败 EM_MP_FAILURE
	参数错误 EM_MP_INVALPARA
 
Other:此接口可用于获取指定类型流的PTS
*******************************************************************************/
int CSMPGetCurrentPTSByType(HCSHANDLE hPlayHandle, CSMPContentType_E eContentType, CSUDI_UINT64 *pulPTS);

/*******************************************************************************
Function Name:    CSMPSetOption
 
Description:
    设置Mediaplayer 配置项
Input:
    hPlayHandle： 播放器句柄
    eOption: 设置的配置项类别
           当 eOption = EM_MP_OPTTYPE_STREAMTYPE时，pValue所指向的值为 int，可以取0，1.  
           取0时，PMT表中StreamType=0x82表示流类型为CSMP_AUDIO_CODEC_DTS，
           取1时，PMT表中StreamType=0x82表示流类型为DCII subtitle
                
    pValue：配置项值指针
          
Output:
  无

Return Value:
    成功：TRUE   (1)
    失败：FALSE  (0) 
 
Other:
目的:    增加 Mediaplayer 通用设置配置项接口
********************************************************************************/
BOOL CSMPSetOption(HCSHANDLE hPlayHandle, CSMPOptType_E eOption, void *pValue);


/*******************************************************************************
Function Name:    CSMPClearOption
 
Description:
    清除Mediaplayer 对应句柄的所有配置项,针对CSMPSetOption所设置的选项
Input:
    hPlayHandle： 播放器句柄
 
          
Output:
  无

Return Value:
    成功: EM_MP_SUCCESS
    失败: EM_MP_FAILURE
 
Other:

********************************************************************************/
int CSMPClearOption(HCSHANDLE hPlayHandle);

/******************************************************************************
Fuzhnction Name:    CSMPSwitchPIDToPlayFile
 
Description:
        设置播放的音频流。因为一路节目可能包含多个音频流，
        比如：国语流，粤语流，英语流等，可以根据用户的需求选择需要播出
        的音频流。
 
Input:
    	hPlayHandle：MediaPlay 句柄
    	@param[in] hPlayer 播放器句柄
		@param[in] nProgramId 节目ID
		@param[in] nVideoPid 视频ID
		@param[in] nAudioPid 音频ID
		@param[in] nSubTitleId 字幕ID
Output:
    None
 
Return Value:
	成功返回TRUE；
	失败返回FALSE
Other:
*******************************************************************************/
BOOL CSMPSwitchPIDToPlayFile ( HCSHANDLE hPlayHandle, int nProgramId, int nVideoPid, int nAudioPid, int nSubTitleId);

int CSMPGetAspectRatio(HCSHANDLE hPlayHandle);
#ifdef __cplusplus
}
#endif

#endif  /* CSMP_H */

/* End of File */

