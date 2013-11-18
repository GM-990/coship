/******************************************************************************
FileName:  csmediaplayer.h
Copyright (c) 2007�� Shen Zhen Coship Electronics CO.,LTD.��

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
1.2         2008-06-10 liuzhenghua       hServiceHnadle:����DM�е�Service���
                                         ��DVB�������ȴ�DM�л�ȡ�洢������ƵPID,
                                         ȥ���ţ��Ѽӿ컻̨�ٶȡ�
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
#define CSMP_THREAD_PRIORITY		(100)/*�������ȼ�*/
#define CSMP_THREAD_STACKSIZE		(128*1024)/*����ջ��С*/
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
	EM_PLAYER_IGMP,     /*IP �鲥*/
	EM_PLAYER_RTSP,      /*IP �㲥*/
	EM_PLAYER_PVRFILE,      /*PVR�ļ�����*/   
	EM_PLAYER_TSHIFT,	/*PVR ʱ�Ʋ���*/
	EM_PLAYER_CMMB,		/*cmmb*/
	EM_PLAYER_RTSPC,	/*IPQAM �㲥*/	
	EM_PLAYER_HTTP,
	EM_PLAYER_IFRAME //����udi2��Ч
 }CSMPDSType_E;

typedef enum CSMPSwitchType_E
{
	 EM_MP_SWITCH_TYPE_UNKNOWN =0,				/*��̨ʱ����Ƶ�Ĵ������ͣ�δ����*/
	 EM_MP_SWITCH_TYPE_SCREEN_BLACK	,			/*��̨ʱ����Ƶ�Ĵ������ͣ�����*/
	 EM_MP_SWITCH_TYPE_SCREEN_IFRAME	,		/*��̨ʱ����Ƶ�Ĵ������ͣ�����*/
	 EM_MP_SWITCH_TYPE_SCREEN_FADE_IN_OUT		/*��̨ʱ����Ƶ�Ĵ������ͣ����뵭��*/
}CSMPSwitchType_E;

/*MediaPlayer module return value*/
typedef enum CSMPRet_E
{
	EM_MP_SUCCESS = 0,       /*�ɹ�*/
	EM_MP_FAILURE,           /*δ֪ԭ��ʧ��*/
	EM_MP_INVALPARA,         /*������Ч*/
	EM_MP_EXIST,             /*��Դ�Ѵ���*/
	EM_MP_INEXISTENCE,       /*��Դ�Ѳ�����*/
	EM_MP_NONSUPPORT         /*ϵͳ��֧��*/
} CSMPRet_E;

/*MediaPlayer State*/
typedef enum CSMPState_E
{
    EM_MP_STATE_INIT,    /*MP��ʼ״̬*/
  	EM_MP_STATE_READY,  /*MP����״̬����Ŀֹͣ��Ҳ�Ǿ���״̬*/
    EM_MP_STATE_PLAYING, /*MP����״̬*/
    EM_MP_STATE_PAUSE,   /*MP��ͣ״̬*/
    EM_MP_STATE_TRICKMODE,  /*MP Trick mode״̬*/
    EM_MP_STATE_CONTROL_SENDING, /*���������·�״̬���ص����ز�ת����ʽ״̬*/
    EM_MP_STATE_END      /*MP����״̬*/
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
	EM_CSMPAUDIO_STEREO_CHAN,	  /*����������*/
	EM_CSMPAUDIO_LEFT_CHAN = 0, 	/*������*/
	EM_CSMPAUDIO_RIGHT_CHAN,    	/*������*/
	EM_CSMPAUDIO_MIX_CHAN      	/*�������*/
} CSMPAudioChan_E;

/*
EM_CSMPVIDEO_FMT_NTSC��ͷ�Ķ���30Hz
EM_CSMPVIDEO_FMT_PAL ��ͷ�Ķ���25Hz
EM_CSMPVIDEO_FMT_SECAM ��
    SECAM��ʽ��֡Ƶÿ��25֡��ɨ����625�У�����ɨ�裬�������4:3���ֱ���Ϊ720��576��Լ40�����أ����ȴ���6.0MHz����ɫ���ز� 4.25MHz��ɫ��        ����1.0MHz(U)��1.0MHz(V)�������ز�6.5MHz��SECAM��ʽ��֡Ƶÿ��25֡��ɨ����625�У�����ɨ�裬�������4:3���ֱ���Ϊ720��576��Լ40            �����أ����ȴ���6.0MHz����ɫ���ز� 4.25MHz��ɫ�ȴ���1.0MHz(U)��1.0MHz(V)�������ز�6.5MHz��
EM_CSMPVIDEO_FMT_480P   60Hz
EM_CSMPVIDEO_FMT_576P   ͨ����50 Hz
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
	EM_CSMPWIN_ASPECT_RATIO_AUTO   /*  �Զ�ѡ�� */
}CSMPAspectRatio_E;

typedef enum CSMPOutput_E
{
	EM_CSMPOUTPUT_HD = 0,	/* HD�����壩,HDMI, YPbPr */
	EM_CSMPOUTPUT_SD      	/* SD�����壩,CVBS,S-VIDEO */
}CSMPOutput_E;

typedef enum CSMPStopMode_E	
{	
	EM_CSMP_STOPMODE_BLACK,			/*black screen*/
	EM_CSMP_STOPMODE_FREEZE,			/*freeze last frame*/	
	EM_CSMP_STOPMODE_FIFO				/*slow black*/
}CSMPStopMode_E;

typedef struct CSMPRect_S
{
	DWORD dwX;            	/*X����ֵ*/
	DWORD dwY;           	/*Y����ֵ*/
	DWORD dwWidth;        	/*��*/
	DWORD dwHeight; 		/*��*/
}CSMPRect_S;

typedef enum CSMPVdisWinParam_E  
{
		EM_CSMP_VDIS_WIN_BRIGHTNESS = 0,     /*����*/
		EM_CSMP_VDIS_WIN_CONTRAST,           /*�Աȶ�*/
		EM_CSMP_VDIS_WIN_SATURATION,        /*���Ͷ�*/
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
	WORD  wVideoPID;              /*��ƵPID*/
	BYTE   ucVideoFormat;          	/*��Ƶ��ʽ*/
}CSMPVideoParams_S;


typedef struct CSMPAudioParams_S
{
	WORD  wAudioPID;                /*��ƵPID*/
	BYTE  ucAudioFormat;           /*��Ƶ��ʽ*/
	BYTE  ucLanguageCode[4];        /*����������*/   
	BYTE  ucRawFormat;             /*ԭʼ��Ƶ���ͣ�����������в�ͬ��������AC3����*/
}CSMPAudioParams_S; 
/*subtitle,teletext�ṹ����*/
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
	BOOL 			bWindowEnable;	    /**< ��Ƶ��ʾ�����Ƿ��*/	
	int 			nSourceWidth;		    /**< Դ��Ƶ�����*/
	int 			nSourceHight;		    /**< Դ��Ƶ���߶�*/
	int 			nFrameRate;		    /**< Դ��Ƶ������*/
	BOOL			bInterlaced;		    /**< TRUE��ʾ����;FALSE��ʾ����*/
	CSMPVdisVideoFormat_E		eTimingModeSD;	    /**< �����豸��Ƶ����ֱ���*/
	CSMPVdisVideoFormat_E		eTimingModeHD;	    /**< �����豸��Ƶ����ֱ���*/
	DWORD			dwFrameCount;		        /**< ��Ƶ�Ѳ��ŵ���֡��*/
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
	EM_CSMP_START_PLAY, 			/*����*/
	EM_CSMP_STOP_PLAY,
	EM_CSMP_VIDEO_IFRAME_SHOW,	/*video����ʾIframe*/
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
	CS_VID_NORMAL = 0,	//���ڲ��ŵ�ǰ��Ŀ
	CS_VID_BLACK,  		//����
	CS_VID_FREEZE	//��֡
}CSMPVideoSetType;

typedef enum _CSMPAUDOutputDevice_e
{
	EM_CSMP_AUD_OUTPUT_RCA	=0x00000001,	/**< ������Ƶ������������ӿ�*/
	EM_CSMP_AUD_OUTPUT_SPDIF	=0x00000002,	/**< ������Ƶ�ӿ�*/
	EM_CSMP_AUD_OUTPUT_RF		=0x00000004,	/**< RF�������*/
	EM_CSMP_AUD_OUTPUT_SCART	=0x00000008,	/**< scart�ӿ�*/
	EM_CSMP_AUD_OUTPUT_HDMI  	=0x00000010	    /**< hdmi �ӿ�*/
}CSMPAUDOutputDevice_E;	


typedef enum _CSMP_AUDIO_OUTPUT_DATA_FORMAT_e
{
    EM_CSMP_AUDIO_OUTPUT_DECODE = 0,
    EM_CSMP_AUDIO_OUTPUT_BYPASS,
    EM_CSMP_AUDIO_OUTPUT_AUTO
}CSMP_AUDIO_OUTPUT_DATA_FORMAT_E;

typedef enum CSMediaPlayerCaStatus_E
{
	EM_CSMP_CA_AV_OK,//��ѽ�Ŀ����ܽ�Ŀ���ųɹ�
	EM_CSMP_CA_SMARTCARD_PLUGOUT,
	EM_CSMP_CA_DESCAMBLED,
	EM_CSMP_CA_OTHER_ENCRYPT,
	EM_CSMP_CA_NOT_AUTHORIZED
}CSMediaPlayerCaStatus_E;

/**@brief ��Ƶ����豸����*/
typedef enum _CSMPVIDOUTDevice_e
{
	EM_CSMP_VID_OUTPUT_CVBS,	/**< CSBVS��Ƶ����ӿ� */
	EM_CSMP_VID_OUTPUT_YC,	/**< S-Video ��Ƶ����ӿ� */
	EM_CSMP_VID_OUTPUT_YUV, /**< ������Ƶ����ӿ� */
	EM_CSMP_VID_OUTPUT_RF, /**< ��Ƶ�ź���Ƶ����ӿ� */
	EM_CSMP_VID_OUTPUT_SCART, /**< SCART��Ƶ����ӿ� */
	EM_CSMP_VID_OUTPUT_HDMI, /**< HDMI��Ƶ����ӿ� */
	EM_CSMP_VID_OUTPUT_DVO,  /**< ������Ƶ���*/
	EM_CSMP_VID_OUTPUT_NUM /**< ��Ƶ�ӿڸ��� */
}CSMPVIDOUTDevice_E;

/**@brief ��Ƶ����˿ڵ���ɫ���� */
typedef enum _CSMPVIDOutputType_e
{
	EM_CSMP_VID_OUTPUT_TYPE_YUV, /**< YUVģʽ */
	EM_CSMP_VID_OUTPUT_TYPE_RGB, 	/**< RGBģʽ */
	EM_CSMP_VID_OUTPUT_TYPE_NUM	 /**< ģʽ���� */
}CSMPVIDOutputType_E;


typedef enum _CSMPOptType_E
{               
    EM_CSMP_OPTTYPE_STREAMTYPE = 0,  ///< ����PMT����StreamType ����ʾ���ͣ�pValueֵΪ int��pValue ֵΪ0ʱ��PMT����StreamType=0x82 ��ʾ������ΪCSMP_AUDIO_CODEC_DTS��
                                    ///< pValue ֵΪ1ʱ��PMT����StreamType=0x82 ��ʾ������ΪΪDCII subtitle    

	EM_CSMP_OPTTYPE_VALIDVIDEOFRAME ,     ///< ������Ƶ���Ŵӵڼ�֡��ʼ��pValueֵΪ int��һ������������ã�
									///< ���Ҫ��ÿ�ζ��ӽ���ĵڼ�֡��ʼ����ʱ������ʹ�ô�ѡ��
	EM_CSMP_OPTTYPE_VALIDAUDIOFRAME      ///< ������Ƶ���Ŵӵڼ�֡��ʼ��pValueֵΪ int��һ������������ã�
									///< ���Ҫ��ÿ�ζ��ӽ���ĵڼ�֡��ʼ����ʱ������ʹ�ô�ѡ��

}CSMPOptType_E;     ///< Mediaplayer ������ö������

/**@brief ������*/
typedef enum 
{
	EM_CSMP_CONTENT_VIDEO = 0,		///< ��Ƶ����	 						
	EM_CSMP_CONTENT_AUDIO, 			///< ��Ƶ����						
	EM_CSMP_CONTENT_SUBTITLE,    	///< ��Ļ����
	EM_CSMP_CONTENT_TELTEX, 		///< ͼ�ĵ�������
	EM_CSMP_CONTENT_PCR 			///< PCR����	
}CSMPContentType_E;

/******************************************************************************
 *                                 Global Function Declare                     *
 ******************************************************************************/
 /******************************************************************************
Function Name:    FOnCSMPHaveMsgToUser

Description:
	�û��ص�,mediaplayer ����Ϣ���ظ��û�

Input:
    nUserData : �û�����
    nMsg : ��Ϣ����
    dwPara1 : 	����1 
    dwPara2 :	����2 
    
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
        
       ����mediaplayer ,�ͷ������Դ 
Input:
    hPlayHandle  mediaplayer ���

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE
    

Other:
*******************************************************************************/
int  CSMPDestroy(HCSHANDLE hPlayHandle );


/******************************************************************************
Function Name:    CSMPSetDataSource

Description:
        ��������Դ ����������Դ�󣬰�һ�ֲ��ŷ�ʽ:dtv ip  file

Input:
    hPlayHandle : ���������
    pszMRL :   ý����Դ��λ��
            "dtv://orinetworkid.tsid.serviceId"
		    "rtsp://192.168.1.10/boa.ts"
		    "igmp://224.0.0.20:2345"
		    "file://f:/video/h264/boa.ts"
		    ����������Ϊ�գ���pszMRLΪ��ʱ������洢RM handle������Ϊ����ѡ������Դʹ��
	dwDeviceId : �豸���
	hServiceHnadle:����DM�е�Service���

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPSetDataSource(HCSHANDLE        hPlayHandle,
							  const char      *pszMRL,
							  DWORD     dwDeviceId ,
							  HCSHANDLE hServiceHnadle);



/******************************************************************************
Function Name:    CSMPPlay

Description:
    ����ý�岥��

Input:
    hPlayHandle : ���������
	hPumpHandle : pumpģ��������VOD����ʹ��
Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPPlay(HCSHANDLE hPlayHandle,HCSHANDLE hPumpHandle);


/******************************************************************************
Function Name:    CSMPStop

Description:
        ֹͣý�岥��
Input:
    hPlayHandle : ���������
	nStopFlag :��ʶ�Ƿ�ͬ��ֹͣ��0Ϊͬ��ֹͣ����0�첽ֹͣ
Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPStop(HCSHANDLE hPlayHandle,WORD nStopFlag);
/******************************************************************************
Function Name:    CSMPChangePlay

Description:
        ���л���Ŀʱ������CSMPChangePlay�ӿڣ����ӿڴ���URL��
        �·������������ڲ��ţ�����ҪֹͣDECODE��
        ����ͨ����������������Ƶ��PID����ʵ�֡�
        ������Ǵ�IP��DVB֮����л�������Ҫ����Play��stop���� ��
        ��PSI����ͻ��ʱҲ���ñ��ӿڡ����ǿ��Բ�����pszMRL��
        ��������ΪNULL��
Input:
    hPlayHandle : ��������� 
    pszMRL :   ý����Դ��λ��
            "dtv://orinetworkid.tsid.serviceId"
		    "rtsp://192.168.1.10/boa.ts"
		    "igmp://224.0.0.20:2345"
		    "file://f:/video/h264/boa.ts"
			dwDeviceId : �豸���
	hPumpHandle : pumpģ��������VOD����ʹ��
	bPMTChange :������ʾPMTͻ����TRUEΪͻ�䣬
				FALSE,����PMT callback�����
Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPChangePlay(HCSHANDLE hPlayHandle,const char *pszMRL,HCSHANDLE hPumpHandle,BOOL bPMTChange);

/******************************************************************************
Function Name:    CSMPSetRate

Description:
        ���ò����ٶ�
Input:
    hPlayHandle : ���������
    nRate : �ٶ�ֵ,���õ�����Ϊʵ�ʲ������ʵ�1000��

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/

int  CSMPSetRate( HCSHANDLE  hPlayHandle , int  nRate );

/******************************************************************************
Function Name:    CSMPGetRate

Description:
        ��ȡ�����ٶ�
Input:
    hPlayHandle : ���������
    nRate : �ٶ�ֵ

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetRate( HCSHANDLE  hPlayHandle , int  *pnRate );

/******************************************************************************
Function Name:    CSMPPause

Description:
        ��ͣý�岥��
Input:
    hPlayHandle : ���������

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPPause( HCSHANDLE  hPlayHandle );
/******************************************************************************
��������:    CSMPResume

����:
    �ָ����ŵ�ǰ��Ŀ.
    ��ֻ�����ڱ����ļ���IP�㲥��
    �첽���ã����������CSMPCreateע��Ļص��з��ء�

�������:
	hMpHandle : Mediaplay���������.
    
�������:
	�ޡ�
    
����ֵ:	
	EM_MP_SUCCESS : �����ɹ�
	EM_MP_FAILURE : ����ʧ��
	EM_MP_INVALPARA : ������Ч
	
����:
*******************************************************************************/
int CSMPResume(HCSHANDLE hPlayHandle);

/******************************************************************************
Function Name:    CSMPSetCurrentTime

Description:
        ���ò�����ʼʱ�� ����SEEK����
Input:
    hPlayHandle : ���������
    nSeconds : ���Ž���,��λ����

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPSetCurrentTime( HCSHANDLE hPlayHandle , DWORD nSeconds);

/******************************************************************************
Function Name:    CSMPSetCurrentTimeEx
 
Description:
        ���ò�����ʼʱ�� ����SEEK����
Input:
    hPlayHandle : ���������
    nSeconds : ʱ����
    nType: 1��ʾNPT��2��ʾUTC��Ϊʱ������
 
Output:
    None
 
Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE
 
Other:
*******************************************************************************/
int  CSMPSetCurrentTimeEx( HCSHANDLE  hPlayHandle ,DWORD nSeconds, int nType);
/******************************************************************************
Function Name:    CSMPGetCurrentTime

Description:
        �õ���ǰʱ�� 
Input:
    hPlayHandle : ���������
    nSeconds : ʱ��ֵ ��λ�� 

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPGetCurrentTime( HCSHANDLE  hPlayHandle,int *pnSeconds);
/******************************************************************************
Function Name:    CSMPSetDebugLevel

Description:
        ���õ��Լ���
Input:
    hPlayHandle : ���������
    nLevel : ����  ���:CSMP_DBG_Level_e

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPSetDebugLevel(HCSHANDLE  hPlayHandle,int nLevel);
/******************************************************************************
Function Name:    CSMPGetProgramInfo

Description:
       �õ���ǰ����������Ϣ����������Ƶ ����Ϣ�� 

Input:
    hPlayHandle : ���������
	psMediaInfo ��ǰ����������Ϣ
Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPGetProgramInfo( HCSHANDLE     hPlayHandle ,CSMPProgrameInfo_S  *psMediaInfo);
/******************************************************************************
Function Name:    CSMPSetAudioChannel

Description:
        ���õ�ǰ����Ƶͨ�����������ҡ��������ȡ�

Input:
    hPlayHandle��MediaPlay ���
	nChannel����Ƶ�����������CSMPAudioChan_Eö�١�
	
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
    �ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE


Other:
*******************************************************************************/
int  CSMPSetAudioChannel ( HCSHANDLE  hPlayHandle ,
        					int   nChannel );

/******************************************************************************
Function Name:    CSMPGetAudioChannel

Description:
        ��ȡ��ǰ����Ƶͨ�����������ҡ��������ȡ�

Input:
    hPlayHandle��MediaPlay ���
	
Output:
    pnChannel�������ȡ������Ƶ�����������CSMPAudioChan_Eö�١�
	
		typedef enum CSMPAUDChannelType_E
		{
		  	EM_CSMPAUD_CHANNEL_STEREO,
		  	EM_CSMPAUD_CHANNEL_LEFT,
		  	EM_CSMPAUD_CHANNEL_RIGHT,
		  	EM_CSMPAUD_CHANNEL_MIXED_MONO
		}CSMPAUDChannelType_E;	

Return Value:
    �ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE


Other:
*******************************************************************************/
int  CSMPGetAudioChannel ( HCSHANDLE hPlayHandle ,CSMPAUDChannelType_E* pnChannel );

/******************************************************************************
Function Name:    CSMPSetStopMode

Description:
	 ��̨ʱ��Ƶ�Ĵ�������:���� ���� ���뵭��


Input:
    hPlayHandle��MediaPlay ���
    nStopMode  ��̨ʱ����Ƶ�Ĵ������� CSMPStopMode

Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetStopMode( HCSHANDLE  hPlayHandle,  int nStopMode );

/******************************************************************************
Function Name:    CSMPGetStopMode

Description:
        ��ȡ��̨ʱ�Ĵ�������

Input:
    hPlayHandle��MediaPlay ���

Output:
    pnStopMode  ��̨ʱ����Ƶ�Ĵ������� MPSwitchType_E

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetStopMode(HCSHANDLE	hPlayHandle ,int  *pnStopMode );

/******************************************************************************
Fuzhnction Name:    CSMPSetAudioSwitch

Description:
        ���ò��ŵ���Ƶ������Ϊһ·��Ŀ���ܰ��������Ƶ����
        ���磺����������������Ӣ�����ȣ����Ը����û�������ѡ����Ҫ����
        ����Ƶ����

Input:
    	hPlayHandle��MediaPlay ���
    	wPID����ƵPID������
		nType :��Ƶ���ͣ�Ϊproting��cs_sysaudio.h���ڵ�����
Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE
Other:
*******************************************************************************/
int  CSMPSwitchAudioPID ( HCSHANDLE  hPlayHandle,  WORD  wPID, WORD wType);

/******************************************************************************
Function Name:    CSMPSwitchVideoPID

Description:
       �л���ƵPID
       

Input:
    	hPlayHandle��MediaPlay ���
    	wPID����ƵPID������
		nType :��Ƶ����
Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
#include "cs_sysvideo.h"  
#include "cs_sysaudio.h"  
*******************************************************************************/
int  CSMPSwitchVideoPID ( HCSHANDLE  hPlayHandle,  WORD  wPID, WORD nType);


/******************************************************************************
Function Name:    CSMPSetVBITeletextEnable

Description:
       �򿪻�ر�VBI Teletext��ʾ
       

Input:
    	hPlayHandle��MediaPlay ���
    	bEnable: TRUE,��VBI Teletext��ʾ;FALSE,�ر�VBI Teletext��ʾ;
Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetVBITeletextEnable ( HCSHANDLE  hPlayHandle,  BOOL bEnable);


/******************************************************************************
Function Name:    CSMPGetDuration

Description:
         �õ�VOD ��Ŀ��Ŀ�ĳ��� 

Input:
    	hPlayHandle��MediaPlay ���

Output:
	pnSeconds :  ʱ�䳤��ֵ  ��λ��

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetDuration( HCSHANDLE  hPlayHandle ,DWORD    *pdwSeconds);


/******************************************************************************
Function Name:    CSMPGetVolume

Description:
       �õ�����ֵ	 ��ʵ��

Input:
    	hPlayHandle��MediaPlay ���None
		pnVolume : ����ֵ
Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetVolume( HCSHANDLE  hPlayHandle , int *pnVolume );

/******************************************************************************
Function Name:    CSMPGetVolume

Description:
       ��������ֵ

Input:
    	hPlayHandle��MediaPlay ���
		nVolume : ����ֵ �û���Ҫ���������ֵ����[0,31]���������㣻
Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetVolume( HCSHANDLE  hPlayHandle , int  nVolume );

/******************************************************************************
Function Name:    CSMPSetVolumeMute

Description:
       ���þ���
Input:
    	hPlayHandle��MediaPlay ���
    	ntype : ������־��CSMPMute_E

Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetVolumeMute( HCSHANDLE  hPlayHandle ,int nType) ;//menu 


/******************************************************************************
Function Name:    CSMPSetAOutMute

Description:
       ����ȫ�־�����ָ����Ƶ���ͨ��Ϊ����
Input:
    	hPlayHandle��MediaPlay ���
    	eAudioDevice :��Ƶ����豸����CSMPAUDOutputDevice_E

Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE
	��֧�ַ���EM_MP_NONSUPPORT
	
Other: UDI1.0ƽ̨�·���EM_MP_NONSUPPORT
*******************************************************************************/
int  CSMPSetAOutMute( HCSHANDLE  hPlayHandle ,CSMPAUDOutputDevice_E eAudioDevice) ;


/******************************************************************************
Function Name:    CSMPSetAOutUnMute

Description:
       ����ȫ�־�����ȡ��ָ����Ƶ���ͨ���ľ���״̬
Input:
    	hPlayHandle��MediaPlay ���
    	eAudioDevice :��Ƶ����豸����CSMPAUDOutputDevice_E

Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE
	��֧�ַ���EM_MP_NONSUPPORT
	
Other:UDI1.0ƽ̨�·���EM_MP_NONSUPPORT
*******************************************************************************/
int  CSMPSetAOutUnMute( HCSHANDLE  hPlayHandle ,CSMPAUDOutputDevice_E eAudioDevice) ;


/******************************************************************************
Function Name:    CSMPGetVolumeMute

Description:
       �õ�������־

Input:
    	hPlayHandle��MediaPlay ���
    	ntype : ������־��CSMPMute_E

Output:
      	ntype : ������־��CSMPMute_E
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPGetVolumeMute( HCSHANDLE  hPlayHandle ,int *pnType);

/******************************************************************************
Function Name:    CSMPSetVideoFormat

Description:
       ������Ƶ�����ʽ��

Input:
		hPlayHandle��MediaPlay ���
		nVideoFormat	����Ƶ��ʽ���μ������CSMPVdisVideoFormat_E
Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

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
       ���ô��ڼ���λ�úͳߴ硣
Input:
    	hPlayHandle��MediaPlay ���
    	psRect :��Ļ��ʾλ�ü��ߴ�CSMPRect_S
Output:
������None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPSetWinClip( int hPlayHandle,const CSMPRect_S *psRect );


/******************************************************************************
Function Name:    CSMPSetWinRect

Description:
        ������Ļ��ʾλ�ü��ߴ硣

Input:
    	hPlayHandle��MediaPlay ���
    	psRect :��Ļ��ʾλ�ü��ߴ�CSMPRect_S
Output:
������None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

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
       ���ô��ڵ�����ֵ�������ȡ��Աȶȡ�ɫ�ȡ���ȵ�

Input:
    hPlayHandle��MediaPlay ���
    nWinParam   ������
    dwValue��������ֵ

Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
		typedef enum _CSMPVdisWinParam_E  
		{
				EM_CSMP_VDIS_WIN_BRIGHTNESS = 0,     ����
				EM_CSMP_VDIS_WIN_CONTRAST,           �Աȶ�
				EM_CSMP_VDIS_WIN_SATURATION,         ���Ͷ�
		}CSMPVdisWinParam_E;
*******************************************************************************/
int  CSMPSetWinParams( HCSHANDLE 	hPlayHandle , 
						int            nWinParam ,
						DWORD      dwValue);


/******************************************************************************
Function Name:    CSMPSetAspectRatio

Description:
       ������Ƶ���������ʾ���ʡ�

Input:
    nRatio :��Ƶ���������ʾ���ʡ�

Output:
    None

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

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
        �õ��������͡�����CSMPDSType_E

Input:
        hPlayHandle��MediaPlay ���

Output:
    pnType :�õ���������  

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

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
       ��ò�������ǰ��״̬��Ϣ

Input:
    hPlayHandle��MediaPlay ���

Output:
    pnStatus :�����ɹ��󷵻ز�����״̬���μ�����CSMPState_E

Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:

*******************************************************************************/
int  CSMPGetStatus( HCSHANDLE  hPlayHandle , int   *pnStatus );
/******************************************************************************
Function Name:    CSMPFreeze

Description:
      ��ʹ���澲ֹ�ڵ�ǰ���ŵ�֡������ʹ������ͣ��

Input:
    hPlayHandle��MediaPlay ���
Output:
    None
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPFreeze(HCSHANDLE  hPlayHandle);
/******************************************************************************
Function Name:    CSMPUnFreeze

Description:
     ȡ�����澲ֹ���������š�

Input:
    hPlayHandle��MediaPlay ���
Output:
    None
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPUnFreeze(HCSHANDLE  hPlayHandle);

/******************************************************************************
Function Name:    CSMPPVMStreamHandle

Description:
     ��ȡPVRģ��stream�ľ��
		ͬ������
Input:
    hPlayHandle��MediaPlay ���
Output:
    None
Return Value:
	�ɹ����ؾ����
	ʧ�ܷ��� 0��
Other:
*******************************************************************************/
HCSHANDLE  CSMPGetPVMStreamHandle (HCSHANDLE  hPlayHandle);

/******************************************************************************
Function Name:    CSMPCallbackToLive

Description:
         ���ӿڵ���ippump��disconnect�ӿڣ��·�teardown���
         ͣ����������Ľ�����ͬʱ�·�ֹͣdecode���Ȼ��֪ͨselection
         �ط��Ѿ�ͣ������selectionȥ����ֱ����
Input:
    hPlayHandle : ���������

Output:
    None

Return Value:
    �ɹ� EM_MP_SUCCESS
    ʧ�� EM_MP_FAILURE

Other:
*******************************************************************************/
int  CSMPCallbackToLive( HCSHANDLE  hPlayHandle );

/******************************************************************************
Function Name:    CSMPSetMatchMethod
Description:
       ���ô��ڵ�pan scan�ȸ�ʽ
Input:
    nMatchMethod���μ�ö��CSMPMatchMethod  
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
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE
Other:

*******************************************************************************/
int  CSMPSetMatchMethod( HCSHANDLE  hPlayHandle,int nMatchMethod);

/******************************************************************************
Function Name:    CSMPSetVideo
Description:
       ���ô��ڵ�pan scan�ȸ�ʽ
Input:
    nMsg���μ�ö��CSMPEventMsg  
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
		EM_CSMP_START_PLAY, //����
		EM_CSMP_STOP_PLAY,
		EM_CSMP_VIDEO_IFRAME_SHOW,//video����ʾIframe
		EM_CSMP_VIDEO_IFRAME_HIDE,
		EM_CSMP_MAX
	}CSMPEventMsg;
	
    nType: �μ�ö��CSMPVideoSetType
    	typedef enum _CSMPVideoSetType
	{
		CS_MODE_NORMAL = 0,	//���ڲ��ŵ�ǰ��Ŀ
		CS_MODE_BLACK,  		//����
		CS_MODE_FREEZE	//��֡
	}CSMPVideoSetType;
Output:
    None
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE
Other:

*******************************************************************************/
int CSMPSetVideo(HCSHANDLE hPlayHandle, int nMsg, int nType);

/******************************************************************************
Function Name:    CSMPSetVideo
Description:
       ���ô��ڵ�pan scan�ȸ�ʽ
Input:
    nMsg���μ�ö��CSMPEventMsg  
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
		EM_CSMP_START_PLAY, //����
		EM_CSMP_STOP_PLAY,
		EM_CSMP_VIDEO_IFRAME_SHOW,//video����ʾIframe
		EM_CSMP_VIDEO_IFRAME_HIDE,
		CSSYSEVENT_USER_OPEN,		
		CSSYSEVENT_USER_CLOSE,
		EM_CSMP_MAX
	}CSMPEventMsg;
	
    nType: �μ�ö��CSMPMute_E
    	typedef enum CSMPMute_E
	{	
		EM_MPUnMute = 0,	    
		EM_MPMute 
	}CSMPMute_E;
Output:
    None
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE
Other:

*******************************************************************************/
int CSMPSetAudio(HCSHANDLE hPlayHandle, int nMsg, int nType);


/******************************************************************************
Function Name:    CSMediaplyerGetVersion

Description:
        ��ȡmediaplay�汾��

Input:
    	��

Output:
	��

Return Value:
	����mediaplayer�汾�ţ�Ϊ�ַ���

Other:
*******************************************************************************/
char* CSMediaplyerGetVersion(void);
/******************************************************************************
Function Name:    CSIPPUMPIsSupportTrickMode

Description:
        ��ȡmediaplay�汾��

Input:
    	��

Output:
	��

Return Value:
	֧�ַ���EM_MP_SUCCESS��
	���򷵻�EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPIsSupportTrickMode(HCSHANDLE  hPlayHandle);
/******************************************************************************
Function Name:    CSMPGetStreamStatus

Description:
        ��ȡ������Ϣ

Input:
    hPlayHandle:player���

Output:
	psStreamStatus:������Ϣ�洢�ռ�

Return Value:
	֧�ַ���EM_MP_SUCCESS��
	���򷵻�EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPGetStreamStatus(HCSHANDLE  hPlayHandle,CSMPStreamStatus_S *psStreamStatus);

/******************************************************************************
Function Name:    CSMPGetMatchMethod

Description:
        ��ȡ��Ļ���䷽ʽ

Input:
    hPlayHandle:player���

Output:
	pnMatchMethod:��Ļ���䷽ʽ�Ĵ洢�ռ�

Return Value:
	֧�ַ���EM_MP_SUCCESS��
	���򷵻�EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPGetMatchMethod(HCSHANDLE  hPlayHandle,int *pnMatchMethod );

int  CSMPSaveBookMark( HCSHANDLE hPlayHandle ,DWORD pdwSeconds);
int  CSMPGetStartTime( HCSHANDLE hPlayHandle ,DWORD *pdwSeconds);
/******************************************************************************
Function Name:    CSMPSetIppumpCallback

Description:
        ֪ͨMEDIAPLAYER��IPPUMPע��ص�����

Input:
    	hPlayHandle:MediaPlay ���
	hPumpHandle:ippump���
Output:
	��

Return Value:
	֧�ַ���EM_MP_SUCCESS��
	���򷵻�EM_MP_FAILURE
Other:
*******************************************************************************/
int CSMPSetIppumpCallback(HCSHANDLE hPlayHandle,HCSHANDLE hPumpHandle);

/******************************************************************************
Function Name:    CSMPSetOutputDataFormat

Description:
        ������Ƶ����豸��������ݸ�ʽ��ѹ�����ѹ����ʽ��
��Ҫ��������AC3����Ƶ��ʽ�Ƿ�Ҫ�ڻ���������н��룬����ֱ�������������豸�Ի�ø��õ�Ч��
Input:
    	mpHandle�ײ��豸�������
	nDevice��Ƶ����˿�
	nFormat����ȡCSMP_AUDIO_OUTPUT_DATA_FORMAT_E������ֵ

	typedef enum _CSMP_AUDIO_OUTPUT_DATA_FORMAT_e
   {
    EM_CSMP_AUDIO_OUTPUT_DECODE = 0,
    EM_CSMP_AUDIO_OUTPUT_BYPASS,
    EM_CSMP_AUDIO_OUTPUT_AUTO
   }CSMP_AUDIO_OUTPUT_DATA_FORMAT_E;

	nFormat = EM_CSMP_AUDIO_OUTPUT_AUTOʱ�����������Ƶ��ʽ��AC3��AC3_PLUS,���������豸�����룬�����PCM���ɻ����н���
	nFormat = EM_CSMP_AUDIO_OUTPUT_DECODEʱ��������Ƶ��ǿ���ɻ����н���
	nFormat = EM_CSMP_AUDIO_OUTPUT_BYPASSʱ����������Ƶ��ǿ���������豸������
Output:
	��

Return Value:
     ʧ��:EM_RET_FAILURE���ɹ�:EM_RET_SUCCESS
Other:
*******************************************************************************/
int  CSMPSetOutputDataFormat(HCSHANDLE mpHandle, int nDevice, int nFormat);

/******************************************************************************
Function Name:    CSMPGetOutputDataFormat

Description:
        ��ȡ��Ƶ����豸��������ݸ�ʽ��ѹ�����ѹ����ʽ��

Input:
    	mpHandle�ײ��豸�������
	nDevice��Ƶ����˿�
Output:
	pFormat:��Ƶ�����ʽ

Return Value:
      ʧ��:EM_RET_FAILURE���ɹ�:EM_RET_SUCCESS
Other:
*******************************************************************************/
int  CSMPGetOutputDataFormat(HCSHANDLE mpHandle, int nDevice, int *pFormat);

/******************************************************************************
Function Name:    CSMPGetEncryptProgramSwitch

Description:
        ��ȡ���ܽ�Ŀ����ǰ�Ƿ���Ҫֹͣ����Ƶ��־

Input:
    	mpHandle�ײ��豸�������
Output:
	pbSwitch:����Ƶ���ر�־

Return Value:
      ʧ��:EM_RET_FAILURE���ɹ�:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPGetEncryptProgramSwitch(HCSHANDLE mpHandle, BOOL *pbSwitch);

/******************************************************************************
Function Name:    CSMPGetEncryptProgramSwitch

Description:
        ���ü��ܽ�Ŀ����ǰ�Ƿ���Ҫֹͣ����Ƶ��־

Input:
    	mpHandle�ײ��豸�������
    	bSwitch:����Ƶ���ر�־
Output:
	

Return Value:
      ʧ��:EM_RET_FAILURE���ɹ�:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPSetEncryptProgramSwitch(HCSHANDLE mpHandle, BOOL bSwitch);

/******************************************************************************
Function Name:    CSMPGetEncryptProgramSwitch

Description:
       ��ȡ��Ŀֹͣʱ��Ƶ����ʾ����
Input:
    	mpHandle�ײ��豸�������
Output:
    	peVideoType:��Ƶ��ʾ����

Return Value:
      ʧ��:EM_RET_FAILURE���ɹ�:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPGetVideoShowType(HCSHANDLE mpHandle, CSMPVideoSetType *peVideoType);

/******************************************************************************
Function Name:    CSMPSetReferenceWinBounds

Description:
        ������Ƶ���ڵĴ�С

Input:
    	hPlayHandle�ײ��豸�������
    	nWidth:��Ƶ���ڵĿ�
    	nHeight:��Ƶ���ڵĸ�
Output:
	
Return Value:
      ʧ��:EM_RET_FAILURE���ɹ�:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPSetReferenceWinBounds(HCSHANDLE hPlayHandle, int nWidth, int nHeight);

/******************************************************************************
Function Name:    CSMPSetSmartCardState

Description:
        �������ܿ�״̬���˽ӿ�Ŀǰֻ��ǰ��ͣ���ļ����Ӱ��

Input:
    	hPlayHandle		��������� 
    	eState			���ܿ�״̬,EM_CSMP_CA_AV_OK��������Ƶ�ܲ��ţ����������ܲ���ԭ��
Output:
	
Return Value:
      	ʧ��:EM_RET_FAILURE���ɹ�:EM_RET_SUCCESS
Other:
*******************************************************************************/
int CSMPSetSmartCardState(HCSHANDLE hPlayHandle,CSMediaPlayerCaStatus_E eState);

/******************************************************************************
Function Name:    CSMPClearDecoder

Description:
       ����������л��������

Input:
    	hPlayHandle��MediaPlay ���

Output:
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
���ӿڲ��ı�Player״̬��������������ݣ������ڲ���״̬���������ݻ����¿�ʼ����
*******************************************************************************/
int CSMPClearDecoder(HCSHANDLE hPlayHandle);


/******************************************************************************
Function Name:    CSMPSetVOutType

Description:
       ������Ƶ����˿ڵ���ɫ����

Input:
    	hPlayHandle��MediaPlay ���
        eVoutDevice:    ��Ƶ����豸,ֻ֧�ַ���(EM_CSMP_VID_OUTPUT_YUV)/(EM_CSMP_VID_OUTPUT_HDMI)
        eType:              �������ɫ���ͣ�RGB����YUV
Output:
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/
int CSMPSetVOutType(HCSHANDLE hPlayHandle,CSMPVIDOUTDevice_E eVoutDevice, CSMPVIDOutputType_E eType);




/******************************************************************************
Function Name:	  CSMPGetIppluginMessage

Description:
	  ����MASKID ��VOD ����л�ȡ��Ϣ����
Input:
		hPlayHandle��MediaPlay ���
		nMaskId: 
		pcOutMessage: �����Ϣ���ݵĻ�����
		nMessageBufLen: �����Ϣ���ݵĻ����� ��С
Output:
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/

int CSMPGetIppluginMessage(HCSHANDLE hPlayHandle,int nMaskId, char *pcOutMessage,int nMessageBufLen);


/******************************************************************************
Function Name:	  CSMPSetDefaultAVPid

Description:
	  ����Ĭ�ϵ�����ƵPID
Input:
		hPlayHandle��MediaPlay ���
		nAudioPid: ��ƵPID
	       nVideoPid:��ƵPID

Output:
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/

int CSMPSetDefaultAVPid(HCSHANDLE hPlayHandle,int nAudioPid,int nVideoPid);

/******************************************************************************
Function Name:	  CSMPSetAudioControlMode

Description:
	  ���������Ŀ���ģʽ
Input:
		
      nMode :0 ȫ�ֿ��ƣ�Ĭ��
      nMode :1 ��������
Output:
Return Value:
	�ɹ�����EM_MP_SUCCESS��
	ʧ�ܷ���EM_MP_FAILURE

Other:
*******************************************************************************/

int CSMPSetAudioControlMode(int nMode);

/******************************************************************************
Function Name:	  CSMPSetSelection
 
Description:
		
	   ���� MP ��Selection���
Input:
	HCSHANDLE hPlayHandle
	HCSHANDLE hSelection
 
Output:
	None
 
Return Value:
	�ɹ� EM_MP_SUCCESS
	ʧ�� EM_MP_FAILURE
	
 
Other:
*******************************************************************************/
int CSMPSetSelection(HCSHANDLE hPlayHandle, HCSHANDLE hSelection);

/******************************************************************************
Function Name:	  CSMPGetCurrentPTS
 
Description:		
	   ���ݵ�ǰmediaplayer��ʵ��,��ȡ��ǰ��Ŀ��PTS
Input:
	HCSHANDLE hPlayHandle:MediaPlay ���
 
Output:
	CSUDI_UINT64 *psPTS:�����ǰ��Ŀ��pts
 
Return Value:
	�ɹ� EM_MP_SUCCESS
	ʧ�� EM_MP_FAILURE
	����Ϊ�� EM_MP_INVALPARA
 
Other:ע��ͨ���˽ӿ�ֻ�ܻ�ȡPCR��PTS
*******************************************************************************/
int CSMPGetCurrentPTS(HCSHANDLE hPlayHandle, CSUDI_UINT64 *psPTS);

/******************************************************************************
Function Name:	  CSMPGetCurrentPTSByType
 
Description:		
	   ���ݵ�ǰmediaplayer��ʵ��,��ȡ��ǰ��Ŀָ�������͵�PTS
Input:
	hPlayHandle:MediaPlay ���
	eContentType: ������
 
Output:
	pulPTS:�����ǰ��Ŀָ�������͵�pts
 
Return Value:
	�ɹ� EM_MP_SUCCESS
	ʧ�� EM_MP_FAILURE
	�������� EM_MP_INVALPARA
 
Other:�˽ӿڿ����ڻ�ȡָ����������PTS
*******************************************************************************/
int CSMPGetCurrentPTSByType(HCSHANDLE hPlayHandle, CSMPContentType_E eContentType, CSUDI_UINT64 *pulPTS);

/*******************************************************************************
Function Name:    CSMPSetOption
 
Description:
    ����Mediaplayer ������
Input:
    hPlayHandle�� ���������
    eOption: ���õ����������
           �� eOption = EM_MP_OPTTYPE_STREAMTYPEʱ��pValue��ָ���ֵΪ int������ȡ0��1.  
           ȡ0ʱ��PMT����StreamType=0x82��ʾ������ΪCSMP_AUDIO_CODEC_DTS��
           ȡ1ʱ��PMT����StreamType=0x82��ʾ������ΪDCII subtitle
                
    pValue��������ֵָ��
          
Output:
  ��

Return Value:
    �ɹ���TRUE   (1)
    ʧ�ܣ�FALSE  (0) 
 
Other:
Ŀ��:    ���� Mediaplayer ͨ������������ӿ�
********************************************************************************/
BOOL CSMPSetOption(HCSHANDLE hPlayHandle, CSMPOptType_E eOption, void *pValue);


/*******************************************************************************
Function Name:    CSMPClearOption
 
Description:
    ���Mediaplayer ��Ӧ���������������,���CSMPSetOption�����õ�ѡ��
Input:
    hPlayHandle�� ���������
 
          
Output:
  ��

Return Value:
    �ɹ�: EM_MP_SUCCESS
    ʧ��: EM_MP_FAILURE
 
Other:

********************************************************************************/
int CSMPClearOption(HCSHANDLE hPlayHandle);

/******************************************************************************
Fuzhnction Name:    CSMPSwitchPIDToPlayFile
 
Description:
        ���ò��ŵ���Ƶ������Ϊһ·��Ŀ���ܰ��������Ƶ����
        ���磺����������������Ӣ�����ȣ����Ը����û�������ѡ����Ҫ����
        ����Ƶ����
 
Input:
    	hPlayHandle��MediaPlay ���
    	@param[in] hPlayer ���������
		@param[in] nProgramId ��ĿID
		@param[in] nVideoPid ��ƵID
		@param[in] nAudioPid ��ƵID
		@param[in] nSubTitleId ��ĻID
Output:
    None
 
Return Value:
	�ɹ�����TRUE��
	ʧ�ܷ���FALSE
Other:
*******************************************************************************/
BOOL CSMPSwitchPIDToPlayFile ( HCSHANDLE hPlayHandle, int nProgramId, int nVideoPid, int nAudioPid, int nSubTitleId);

int CSMPGetAspectRatio(HCSHANDLE hPlayHandle);
#ifdef __cplusplus
}
#endif

#endif  /* CSMP_H */

/* End of File */

