/* --------------------------------------------------------------------
×¢Òâ£º
1.ÔÚÐèÒªÓëÓÃ»§½»»¥µÄ²âÊÔÓÃÀýÖÐ£¬¿ÉÒÔ£º
	a. Ê¹ÓÃCSTKWaitAnyKeyµÈ´ýÓÃ»§ÊäÈëÈÎÒâ°´¼ü
	b. Ê¹ÓÃCSTKWaitYesµÈ´ýÓÃ»§ÊäÈëYES
2.²âÊÔÓÃÀýº¯ÊýÃüÃû£º²âÊÔÓÃÀýID£¬"²âÊÔÓÃÀýID"¶¨ÒåÔÚ²âÊÔÓÃÀýÎÄµµÖÐ
-----------------------------------------------------------------------*/
#include "cs_filetestcase.h"
#include "udi2_player.h"
#include "udi2_demux.h"
#include "udi2_audio.h"
#include "udi2_video.h"
#include "udi2_tuner.h"
#include "udi2_os.h"
#include "udi2_public.h"
#include "udi2_inject.h"
#include "udi2_tuner.h"
#include "udi2_fs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../cs_udi2testcase.h"
#include "cs_testkit_porting.h"
#include "udiplus_os.h"

#define REPEAT_TIMES (2)
#define MAX_SLEEP_NUM 32
#define NUSERDATA (1234)
#define INJECT_SEEK_SET (0)
#define INJECT_SEEK_CUR (1)
#define INJECT_SEEK_END (2)
#define INJECTTHREADPRO (100)
#define INJECTTHREADBUF (1024 * 148)
#define INJECT_INVALID_PID (0x1fff)
#define MAX_PESES_COUNT_IN_TS (EM_UDI_CONTENT_PCR+1)
#define LivePlayer (0)
#define SLEEP_TIME (2000)

#define REWIND_TO_BEGIN (1)
#define FORWARD_TO_END   (2)
#define PLAY_TO_END FORWARD_TO_END

typedef enum
{
	EM_UDI_FILEFORMAT_TS,
	EM_UDI_FILEFORMAT_WMV,
	EM_UDI_FILEFORMAT_AVI,
	EM_UDI_FILEFORMAT_RMVB,
	EM_UDI_FILEFORMAT_FLV,
	EM_UDI_FILEFORMAT_MP3,
	EM_UDI_FILEFORMAT_MP4,
	EM_UDI_FILEFORMAT_MOV,
	EM_UDI_FILEFORMAT_MKV,
	EM_UDI_FILEFORMAT_MAX
}CSUDIFileFormat_E;

typedef enum
{
	EM_UDI_FILENAME_TS_SD,
	EM_UDI_FILENAME_TS_HD,
	EM_UDI_FILENAME_WMV,
	EM_UDI_FILENAME_AVI_SD,
	EM_UDI_FILENAME_AVI_HD,
	EM_UDI_FILENAME_RMVB_SD,
	EM_UDI_FILENAME_RMVB_HD,
	EM_UDI_FILENAME_FLV,
	EM_UDI_FILENAME_MP3,
	EM_UDI_FILENAME_M4A,
	EM_UDI_FILENAME_AC3,
	EM_UDI_FILENAME_AAC,
	EM_UDI_FILENAME_MP4_SD,
	EM_UDI_FILENAME_MP4_HD,
	EM_UDI_FILENAME_3GP,
	EM_UDI_FILENAME_MOV,
	EM_UDI_FILENAME_MKV_SD,
	EM_UDI_FILENAME_MKV_HD,
	EM_UDI_FILENAME_MKV_HD_BSIZE,
	EM_UDI_FILENAME_MA,
	EM_UDI_FILENAME_MV,
	EM_UDI_FILENAME_MS,
	EM_UDI_FILENAME_MAX
}CSUDIFileNAME_E;

typedef struct _FilePath_S
{
	char *filename;

}FilePath_S;

typedef struct _FileTestStreamInfo_S
{
	CSUDIPlayerStreamType_E m_eStreamType; ///< ÎÄ¼þÁ÷ÀàÐÍ
	CSUDI_INT64 m_n64FileSize; ///< ÎÄ¼þ´óÐ¡£¬µ¥Î»×Ö½Ú
	CSUDI_INT64 m_n64StartTime; ///< ÎÄ¼þ²¥·ÅÆðÊ¼Ê±¼ä£¬µ¥Î»ms
	CSUDI_INT64 m_n64Duration; ///< ÎÄ¼þ×ÜÊ±³¤£¬µ¥Î»ms
	CSUDI_UINT32 m_u32Bps; ///< ÎÄ¼þÂëÂÊ£¬bits/s
	char m_acFileName[CSUDI_PLAYER_MAX_FILE_NAME_LEN]; ///< ÎÄ¼þÃû³Æ
	CSUDI_UINT32 m_u32ProgramNum; ///< Êµ¼Ê½ÚÄ¿¸öÊý
	CSUDI_UINT32 m_u32AudStreamNum; ///< ÒôÆµÁ÷¸öÊý
	CSUDI_UINT32 m_u32SubTitleNum; ///< ×ÖÄ»¸öÊý
	CSUDI_UINT32 m_u32VideoFormat; ///< ÊÓÆµ±àÂë¸ñÊ½£¬Öµ¶¨Òå²Î¿¼::HI_UNF_VCODEC_TYPE_E jia
	CSUDI_UINT16 m_u16Width; ///< ¿í¶È£¬µ¥Î»ÏñËØ jia
	CSUDI_UINT16 m_u16Height; ///< ¸ß¶È£¬µ¥Î»ÏñËØ jia
	CSUDI_UINT16 m_u16Channels; ///< ÉùµÀÊý, 1 or 2 jia
	CSUDI_UINT32 m_u32SampleRate; ///< 8000,11025,441000,... jia
	CSUDI_UINT32 m_u32AudioFormat; ///< ÒôÆµ±àÂë¸ñÊ½£¬Öµ¶¨Òå²Î¿¼::HA_FORMAT_E jia
}FileTestStreamInfo_S;

static FilePath_S stFilePath[] =
{
	{"./testdata/fileplay/test_sd.ts"},
	{"./testdata/fileplay/test_hd.ts"},
	{"./testdata/fileplay/test.wmv"},
	{"./testdata/fileplay/test_sd.avi"},
	{"./testdata/fileplay/test_hd.avi"},
	{"./testdata/fileplay/test_sd.rmvb"},
	{"./testdata/fileplay/test_hd.rmvb"},
	{"./testdata/fileplay/test.flv"},
	{"./testdata/fileplay/test.mp3"},
	{"./testdata/fileplay/test.m4a"},
	{"./testdata/fileplay/test.ac3"},
	{"./testdata/fileplay/test.aac"},
	{"./testdata/fileplay/test_sd.mp4"},
	{"./testdata/fileplay/test_hd.mp4"},
	{"./testdata/fileplay/test.3gp"},
	{"./testdata/fileplay/test.mov"},
	{"./testdata/fileplay/test_sd.mkv"},
	{"./testdata/fileplay/test_hd.mkv"},
	{"./testdata/fileplay/test_hd_bs.mkv"},
	{"./testdata/fileplay/test_ma.trp"},
	{"./testdata/fileplay/test_mv.ts"},
	{"./testdata/fileplay/test_ms.ts"},
};

static FileTestStreamInfo_S s_sStreamInfo[] = {
	{
		EM_UDIFILEPLAYER_STREAM_ES,
		238699652,
		37,
		76354,
		25009505,
		"test_ma.trp",
		1,
		5,
		0,
		0,
	 	720,
		576,
		6,
	 	44100,
		3
	},

	{
	},

	{

	},

	{
	}
};

static int s_svrfiletime[5] = {145, 67, 459, 312, 259};

static CSUDIFilePlayerEventType_E s_ePlayerEvent = EM_UDIFILEPLAYER_MAXEVENTTYPE;

CSUDI_BOOL CSTC_FILE_Init(void)
{
	//ÔÚ±¾²âÊÔÓÃÀý¼¯Ö´ÐÐÇ°µ÷ÓÃ
	CSUDIAUDIOSetVolume(0, 30);

	CSUDIVIDEOShow(0, CSUDI_TRUE);
	CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO);
	CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_MATCH_METHOD_IGNORE);

	return CSUDI_TRUE;
}

BOOL CSTC_FILE_UnInit(void)
{
	//ÔÚ±¾²âÊÔÓÃÀý¼¯Ö´ÐÐºóµ÷ÓÃ
	return TRUE;
}

static void PlayCallback (CSUDI_HANDLE hPlayer,CSUDIPlayerEventType_E eEvent,void * pvUserData)
{
       if(EM_UDIPLAYER_VIDEO_FRAME_COMING == eEvent)
       {
           if (pvUserData != NULL)
    		{
    			CSTCPrint("[%s, %d] eEvent:%d, eventData:%s\r\n", __FUNCTION__, __LINE__, eEvent, (char *)pvUserData);
    		}
       }
}

static void FilePlayCallback(CSUDI_HANDLE hPlayer, CSUDIFilePlayerEventType_E ePlayerEvent, void *pvEventData, void * pvUserData)
{
	if (EM_UDIFILEPLAYER_VIDEO_UNDERFLOW == ePlayerEvent)
	{
		if (pvEventData != NULL)
		{
			CSTCPrint("[%s, %d] eventType:%d, eventData:%s\r\n", __FUNCTION__, __LINE__, ePlayerEvent, (char *)pvEventData);
		}
	}
	else if (EM_UDIFILEPLAYER_BEGIN_OF_STREAM == ePlayerEvent)
	{
		unsigned char *pucMoveToend = pvUserData;

		CSTCPrint("ÒÑµ½×îÇ°£¡\n");
		if (pvUserData != NULL)
		{
			*pucMoveToend = REWIND_TO_BEGIN;
		}
	}
	else if (EM_UDIFILEPLAYER_END_OF_STREAM == ePlayerEvent)
	{
		unsigned char *pucMoveToend = pvUserData;

		CSTCPrint("ÒÑµ½×îºó£¡\n");
		if (pvUserData != NULL)
		{
			*pucMoveToend = FORWARD_TO_END;
		}
	}
}

static void FilePlayEventCallback(CSUDI_HANDLE hPlayer, CSUDIFilePlayerEventType_E ePlayerEvent, void *pvEventData, void * pvUserData)
{
	switch(ePlayerEvent)
	{
		case EM_UDIFILEPLAYER_ERR_ABORTED: ///< ÓÃ»§ÖÐÖ¹²Ù×÷£¬ÖÐÖ¹³É¹¦ÏûÏ¢
			CSTCPrint("[FilePlayEventCallback]UDI File Play Aborted error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_DECODE: ///< ½âÂëÆ÷³ö´í
			CSTCPrint("[FilePlayEventCallback]UDI File Play Decoder error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_FORMAT: ///< Ã½ÌåÎÄ¼þ²»´æÔÚ»òÎÄ¼þ¸ñÊ½³ö´í
			s_ePlayerEvent = EM_UDIFILEPLAYER_ERR_FORMAT;
			CSTCPrint("[FilePlayEventCallback]UDI File Play Format error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_SEEK: ///< ²¥·Å¶¨Î»³ö´íÊ±£¬Å×Ò»´ÎÏûÏ¢
			s_ePlayerEvent = EM_UDIFILEPLAYER_ERR_SEEK;
			CSTCPrint("[FilePlayEventCallback]UDI File Play Seek error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_PAUSE: ///< ÔÝÍ£²Ù×÷Ê§°ÜÊ±£¬Å×Ò»´ÎÏûÏ¢
			CSTCPrint("[FilePlayEventCallback]UDI File Play Pause error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_RESUME:  ///< ×´Ì¬»Ö¸´Ê§°ÜÊ±£¬Å×Ò»´ÎÏûÏ¢
			CSTCPrint("[FilePlayEventCallback]UDI File Play Resume error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_SETSPEED: ///< ÉèÖÃËÙÂÊÊ§°ÜÊ±£¬Å×Ò»´ÎÏûÏ¢
			s_ePlayerEvent = EM_UDIFILEPLAYER_ERR_SETSPEED;
			CSTCPrint("[FilePlayEventCallback]UDI File Play Set Speed error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_NETWORK:  ///< ÍøÂçÒì³££¬m_pvDataÎªint£¬ÖµÎªHTTP ±ê×¼error code, ±ÈÈçhttp´íÎóÎª 200, pvEventDataÖµÎª200£»
			CSTCPrint("[FilePlayEventCallback]UDI File Play Network error !\n\r");
			break;

		case EM_UDIPFILELAYER_STATE_LOADING: ///< »º´æ¿ªÊ¼,Ã¿´Î½øÈë»º´æ×´Ì¬¶¼ÒªÅ×Ò»´Î
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Loading !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_LOADED: ///< »º´æ½áÊø,ÓÐ×ã¹»Êý¾Ý¿ªÊ¼²¥·Å
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Loaded !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_HAVEMETADATA:  ///< ÄÜ¹»»ñÈ¡Ò»Ð©»ù±¾ÐÅÏ¢£¬ÈçÆ¬³¤
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Havemetadata !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_ENOUGHDATA_FORPLAY:  ///< ÓÐ×ã¹»µÄÊý¾Ý½øÐÐ²¥·Å£¬Èç½øÐÐ¿ì½ø¿ìÍË¶¼²Ù×÷
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Enoughdata For Play !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_DURATIONCHANGE:   ///< ½ÚÄ¿×ÜÊ±³¤·¢Éú±ä»¯£¬ÐèÒª²¥·ÅÆ÷¸üÐÂÊ±³¤£¬¿ÉÒÔÍ¨¹ýCSUDIPLAYERGetDuration »ñÈ¡ÐÂµÄ×ÜÊ±³¤
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Duration Change !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_RATECHANGE: ///< ½ÚÄ¿×Ü²¥·Å±¶Êý·¢Éú±ä»¯£¬¿ÉÒÔÍ¨¹ýCSUDIPLAYERGetSpeed »ñÈ¡ÐÂµÄ²¥·ÅËÙÂÊ
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Rate Change !\n\r");
			break;
		case EM_UDIFILEPLAYER_STATE_LOADING_PROGRESS: ///< µÈ´ý²¥·Å£¬»º´æÊý¾ÝµÄ½ø¶ÈÖµ£»½ø¶ÈÖµºÍ»ùÖµ»òÆðÀ´£¬m_pvDataÎªint, °Ù·ÖÖÆ£¬±ÈÈç»º³åµ½25%,  pvEventDataÖµÎª25£»
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Loading Progress !\n\r");
			break;
		case EM_UDIFILEPLAYER_STATE_LOADING_DOWNLOADRATE: ///< ÏÂÔØËÙÂÊ, pvEventDataÎªint, µ¥Î»:kbytes/s, ±ÈÈçÍøÂçÏÂÔØËÙ¶È25kbits/s, pvDataÖµÎª25£»
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Loading Download Rate !\n\r");
			break;

		case EM_UDIFILEPLAYER_STATE_STOP:                 ///< Í£Ö¹²¥·ÅÆ÷²Ù×÷³É¹¦
			s_ePlayerEvent = EM_UDIFILEPLAYER_STATE_STOP;
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Stop !\n\r");
			break;
		case EM_UDIFILEPLAYER_STATE_START:                ///< Æô¶¯²¥·ÅÆ÷²Ù×÷³É¹¦
			s_ePlayerEvent = EM_UDIFILEPLAYER_STATE_START;
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Start !\n\r");
			break;
		case EM_UDIFILEPLAYER_STATE_PAUSE:              ///< ÔÝÍ£²¥·ÅÆ÷²Ù×÷³É¹¦
			s_ePlayerEvent = EM_UDIFILEPLAYER_STATE_PAUSE;
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Pause !\n\r");
			break;
		case EM_UDIFILEPLAYER_SEEK_FINISH:           ///< Ñ¡Ê±²¥·Å¶¨Î»³É¹¦
			s_ePlayerEvent = EM_UDIFILEPLAYER_SEEK_FINISH;
			CSTCPrint("[FilePlayEventCallback]UDI File Play Seek Finish !\n\r");
			break;
		default:
			break;
	}
}

typedef struct
{
	const char*          m_pcTsFilename;  	  //ÂëÁ÷ÎÄ¼þÃû³Æ
	const char* 		 m_pcServiceDescript; 	//ÂëÁ÷ÃèÊö
	int                  m_nVideoPid;           ///< Êý¾ÝËùÔÚPID£¬-1±íÊ¾²»´æÔÚ
	CSUDIVIDStreamType_E m_eVidStreamType;      ///ÊÓÆµÀàÐÍ
	int                  m_nAudioPid;           ///< Êý¾ÝËùÔÚPID £¬-1±íÊ¾²»´æÔÚ
	CSUDIAUDStreamType_E m_eAudStreamType;      ///<ÒôÆµÀàÐÍ
	int 				 m_nPcrPid;				///< PCRÀàÐÍPID£¬-1±íÊ¾²»´æÔÚ
	int 				 m_nSubPid;				///<SUBTITLEÀàÐÍµÄPID£¬-1±íÊ¾²»´æÔÚ
	int 				 m_nTelPid;				///<TELETEXTÀàÐÍµÄPID£¬-1±íÊ¾²»´æÔÚ
}Player_TestServiceInfo_S;

typedef struct _TSStreamInfo_S
{
    char * m_pcName; //×¢ÈëÎÄ¼þµÄÃû
    CSUDIStreamInfo_S    m_TSContentInfo[MAX_PESES_COUNT_IN_TS];
    unsigned int m_uBufferLen;
    CSUDI_BOOL m_bRun;
    CSUDI_HANDLE m_hInjecter;
    CSUDI_BOOL m_bSeek;
    CSUDI_HANDLE m_hSeekEvent;
    CSUDI_BOOL m_bOpenFileSuccess;
}TS_StreamInfo_S;

static Player_TestServiceInfo_S g_sPlayer_SeviceInfo[] =
{
	{
		"Audio&Video Test_27Mbps_20070524.ts",
		"¶àÓïÑÔAC3MP2",
		60,
		EM_UDI_VID_STREAM_MPEG2,
		62,
		EM_UDI_AUD_STREAM_MPEG2,
		60,
		-1,
		-1
	}
};

static CSUDI_BOOL PLAYER_iLockTuner(void)
{
	return CSTC_UDI2PortingLock(0, g_sPlayer_SeviceInfo[LivePlayer].m_pcTsFilename);
}

static TS_StreamInfo_S g_TS_StreamInfo[] =
{
	{
        	"EPG_0606_121458.ts",/*EM_INJECT_TS_MPEG2_MPEG2*/
		{
			{0x15AE, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},
			{0x15AF, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}},
			{INJECT_INVALID_PID, 0, {0}},
			{INJECT_INVALID_PID, 0, {0}},
		 	{INJECT_INVALID_PID, 0, {0}},
	   	},
		10*1024,
		CSUDI_TRUE,
		CSUDI_NULL,
		CSUDI_FALSE,
		CSUDI_NULL,
		CSUDI_FALSE,
	}
};

static void * InjectFSOpen(const char * pcFileName, const char * pcMode)
{
	return (void *)CSTKP_FOpen(pcFileName, pcMode);
}

static int  InjectFSSeek(void *pFile, long lOffset, unsigned int uOrigin)
{
	return CSTKP_FSeek((CSUDI_HANDLE)pFile, lOffset,uOrigin);
}

static long  InjectFSTell(void *pFile)
{
	return CSTKP_FTell((CSUDI_HANDLE)pFile);
}

static int InjectFSRead(void* pFile, char * pcBuf, unsigned int uCount)
{
	return CSTKP_FRead(pcBuf, 1, uCount, (CSUDI_HANDLE)pFile);
}

static int  InjectFSClose(void *pFile)
{
	return CSTKP_FClose((CSUDI_HANDLE)pFile);
}

static CSUDI_BOOL getInjecterFile(const char *pFileName, void **file, long *pLength)
{
	void *pFileTemp;
	int nReturn = -1;

	pFileTemp = InjectFSOpen(pFileName, "r");

	if (!pFileTemp)
	{
		CSTCPrint("²âÊÔËùÐèÎÄ¼þ %s ´ò¿ªÊ§°Ü£¬ÇëÈ·±£testdataÄ¿Â¼ÏÂ´æÔÚ¸ÃÎÄ¼þ!!!\n", pFileName);
		CSTCPrint("[in getInjecterFile]InjectFSOpen is failed!file name is %s\n", pFileName);
		return CSUDI_FALSE;
	}

	nReturn = InjectFSSeek(pFileTemp,0,INJECT_SEEK_END);
	if(nReturn != 0)
	{
		CSTCPrint("[in getInjecterFile]InjectFSSeek end is failed!\n\r");
		return CSUDI_FALSE;
	}

	*pLength = InjectFSTell(pFileTemp);
	if(*pLength < 0)
	{
		CSTCPrint("[in getInjecterFile]InjectFSTell is failed!\n\r");
		return CSUDI_FALSE;
	}

	nReturn = InjectFSSeek(pFileTemp,0,INJECT_SEEK_SET);
	if(nReturn != 0)
	{
		CSTCPrint("[in getInjecterFile]InjectFSSeek begin is failed!\n\r");
		return CSUDI_FALSE;
	}

	*file = pFileTemp;
	return CSUDI_TRUE;
}

static void TS_injectTask(void * TSStreamInfo)
{
	void * ppvBuffer = NULL;

	unsigned  int  uLength = 0;

	TS_StreamInfo_S *pTSStreamInfo = (TS_StreamInfo_S*)TSStreamInfo;

	CSUDI_HANDLE hInjecter = pTSStreamInfo->m_hInjecter;

	int nBlockNum = 0;
	long nFileAllLength = 0;  //ÎÄ¼þ×Ü³¤¶È
	int nFileCurrentPos = 0; //ÎÄ¼þµ±Ç°Î»ÖÃ
	int nFileLeftLength = 0; //Ê£ÓàÎ´¶ÁÎÄ¼þ³¤¶È
	int nInjectedLength = 0; //¿É×¢ÈëµÄ´óÐ¡
	int nReadLength = 0;

	void* file = CSUDI_NULL;

	if(!getInjecterFile((const char*)pTSStreamInfo->m_pcName, &file, &nFileAllLength))
	{
       		CSTCPrint("getInjecterFile failed!\n\r");
       		pTSStreamInfo->m_bRun = CSUDI_TRUE;
     		pTSStreamInfo->m_bOpenFileSuccess = CSUDI_FALSE;
       		return;
	}

	if (pTSStreamInfo->m_uBufferLen<10240||pTSStreamInfo->m_uBufferLen>1024*1024)
	{
		pTSStreamInfo->m_uBufferLen = 10*1024;
	}
	pTSStreamInfo->m_bOpenFileSuccess = CSUDI_TRUE;
	pTSStreamInfo->m_bRun = CSUDI_TRUE;

	while(pTSStreamInfo->m_bRun)
	{
		//Çó³öµ±Ç°Ê£ÓàÎ´¶ÁÎÄ¼þµÄ³¤¶È
		nFileCurrentPos=InjectFSTell(file);	//µ±Ç°ÒÑ¶ÁÎÄ¼þµÄ³¤¶È

		nFileLeftLength=nFileAllLength-nFileCurrentPos; //Ê£ÓàÎ´¶ÁÎÄ¼þµÄ³¤¶È

                uLength = 0;

		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);

		if(uLength != 0)
		{
			nReadLength = pTSStreamInfo->m_uBufferLen < nFileLeftLength ? pTSStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //Ê£ÓàÎÄ¼þ³¤¶ÈÐ¡ÓÚ¿É×¢Èë¿Õ¼äµÄ³¤¶È
			{
				nInjectedLength = nReadLength;
			}
			else  //Ê£ÓàÎÄ¼þ³¤¶È´óÓÚµÈÓÚ¿É×¢Èë¿Õ¼äµÄ³¤¶È
			{
				nInjectedLength = uLength;
			}

			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //¶ÁÈ¡ÏàÓ¦³¤¶ÈµÄÎÄ¼þ

			CSUDIINJECTERWriteComplete(hInjecter,nInjectedLength);

			if(nFileLeftLength == 0)
			{
				InjectFSSeek(file,0,INJECT_SEEK_SET);
			}
		}
		else
	        {
			CSUDIOSThreadSleep(10);
	        }
	}

	if(!pTSStreamInfo->m_bRun )
	{
		InjectFSClose(file);
	}

}

//²éÕÒÖ§³ÖnDemxTypeÀàÐÍµÄdemx
//nDemxType²Î¼ûCSUDIDEMUXWorkType_E
static int searchDemuxID_Y_InInject(int nDemxType)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;

	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //ÏÈÈ¡µÃdemuxµÄÊýÁ¿
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //È¡Ïà¶ÔÓ¦ID=i µÄdemuxµÄÄÜÁ¦
			{
				if((sDemuxCapabilityInfo.m_dwWorkTypeMask & nDemxType ) != 0)
				{
					nDemuxID=i;
					break;
				}
			}
		}
	}
	CSTCPrint("nDemuxID nDemuxID=%d\n\r",nDemuxID);
	return  nDemuxID;
}

//²éÕÒÖ§³ÖnAudioTypeÀàÐÍµÄAudio Decoder
//nAudioType²Î¼ûCSUDIAUDStreamType_E, ÀàÐÍÎªEM_UDI_AUD_STREAM_AC3ÌØÊâ´¦Àí
static int searchAudioID_Y_InInject(int nAudioType)
{
	CSUDIAUDStreamType_E  eAudStreamTypeNum=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIAUDIOCapability_S   sAudioCapabilityInfo;
	int nAudioCount = 0;
	int nAudioID = -1;
	int i = 0;
	int j = 0;

	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //ÏÈÈ¡µÃÒôÆµ½âÂëÆ÷µÄÊýÁ¿
	{
		for (i=0; i<nAudioCount; i++)
		{

			if(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo))           //µ½Ïà¹ØIDµÄÒôÆµ½âÂëÆ÷µÄÄÜÁ¦
			{
				if(nAudioType == EM_UDI_AUD_STREAM_AC3)
				{
					for (j=0; j<eAudStreamTypeNum; j++)                                //ÆäÖÐÄÜÁ¦°üº¬Á½·½Ãæ£ºÖ§³Ö½âÂëµÄÄÜÁ¦ÓëÖ§³ÖÖ±½ÓÊä³öµÄÄÜÁ¦
					{
						if (sAudioCapabilityInfo.m_sSupportByPass[j] == EM_UDI_AUD_STREAM_UNKNOWN )
						{
							break;
						}
						if (sAudioCapabilityInfo.m_sSupportByPass[j] == nAudioType)
						{
							nAudioID=i;
							break;
						}
					}
				}
				else
				{
					for (j=0; j<eAudStreamTypeNum; j++)                                //ÆäÖÐÄÜÁ¦°üº¬Á½·½Ãæ£ºÖ§³Ö½âÂëµÄÄÜÁ¦ÓëÖ§³ÖÖ±½ÓÊä³öµÄÄÜÁ¦
					{
						if (sAudioCapabilityInfo.m_sSupportDecode[j] == EM_UDI_AUD_STREAM_UNKNOWN )
						{
                                                 //CSTKPrint("eAudStreamTypeNum=%d\n",sAudioCapabilityInfo.m_sSupportDecode[j]);
							break;
						}
						if (sAudioCapabilityInfo.m_sSupportDecode[j] == nAudioType)
						{
							nAudioID=i;
							break;
						}
					}
				}

				if (nAudioID != -1)
					break;
			}
		}
	}

	return nAudioID;
}

static int searchVideoID_Y_InInject(int type_index)
{
	int i;
	int j;
	CSUDIVIDStreamType_E  eVidStreamTypeNum=EM_UDI_VID_STREAMTYPE_NUM ;
	int nVideoCount;
	int nVidioID=-1;
	CSUDIVIDEOCapability_S   sVideoCapabilityInfo;

	CSUDIVIDEOGetCount(&nVideoCount);                                      //ÏÈÈ¡ÊýÁ¿

	for ( i=0;i < nVideoCount; i ++)
	{
		CSUDIVIDEOGetCapability( i , &sVideoCapabilityInfo);	//ÔÙÈ¡Ïà¹ØÄÜÁ¦£¬ÓëÒôÆµ²»Í¬µÄÊÇÊÓÆµÖ»ÓÐÖ§³ÖµÄÊÓÆµÀàÐÍÒ»Ïî

		for (j=0; j < eVidStreamTypeNum; j++)
		{
			if (sVideoCapabilityInfo.m_eStreamType[j] == EM_UDI_VID_STREAM_UNKNOWN )
			{
				break;
			}
			if (sVideoCapabilityInfo.m_eStreamType[j]==type_index)
			{
				nVidioID=i;
				break;
			}
		}

		if (nVidioID!=-1)
		{
			break;
		}
	}

	return nVidioID;

}

static int  getTSInjectInfo(CSUDIINJECTERChnl_S  *psInjecterChnl,CSUDIPlayerChnl_S  *psPlayerChnl,CSUDIINJECTEROpenParam_S *pOpenParam ,CSUDIStreamInfo_S  sStreamInfo[])
{
	int nAudioId = -1;
	int nVideoId = -1;
	int nDemuxID = -1;
	int i = 0;
	int nStreamCnt = 0;
	TS_StreamInfo_S *pTSStreamInfo = &g_TS_StreamInfo[LivePlayer];

//×¢ÈëµÄÒôÊÓÆµ½âÂëÆ÷
	psInjecterChnl->m_nAudioDecoder=-1;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;
	psInjecterChnl->m_nDemux=-1;

	nDemuxID=searchDemuxID_Y_InInject(EM_UDI_DEMUX_INJECT);
	if(nDemuxID == -1)
	{
		CSTCPrint( "Ã»ÓÐÖ§³Ö×¢ÈëµÄdemux! \n\r");
        	CSTCPrint("There's no demux device supporting inject \n");
       	        return -1;
	}

	psPlayerChnl->m_nDemux = nDemuxID;

	for (i=0;i<MAX_PESES_COUNT_IN_TS;i++)
	{
	    if (pTSStreamInfo->m_TSContentInfo[i].m_nPid == INJECT_INVALID_PID)
		{
			continue;
		}

		if (pTSStreamInfo->m_TSContentInfo[i].m_eContentType == EM_UDI_CONTENT_VIDEO)
		{
			nVideoId = searchVideoID_Y_InInject(pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eVideoType);
			if(nVideoId ==-1)
			{
				CSTCPrint( "Ã»ÓÐÖ§³ÖÖ¸¶¨ÀàÐÍÊÓÆµ½âÂëÆ÷!\n\r");
                                CSTCPrint("There's no video decode supporting stream type %d \n",\
                                pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eVideoType);
                                return -1;
			}
			psPlayerChnl->m_nVideoDecoder = nVideoId;

		}
		else if (pTSStreamInfo->m_TSContentInfo[i].m_eContentType == EM_UDI_CONTENT_AUDIO)
		{
			nAudioId = searchAudioID_Y_InInject(pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eAudioType);
			if(nAudioId ==-1)
			{
				CSTCPrint( "Ã»ÓÐÖ§³ÖÖ¸¶¨ÀàÐÍÒôÆµ½âÂëÆ÷!\n\r");
                		CSTCPrint("There's no audio decode supporting stream type %d \n",\
                    		pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eAudioType);
                		return -1;
			}
			psPlayerChnl->m_nAudioDecoder = nAudioId;
		}

		memcpy(&sStreamInfo[i],&(pTSStreamInfo->m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S));
        	nStreamCnt++;

	}

	if(nStreamCnt<1)//ÖÁÉÙ×¢ÈëÒ»Â·ÀàÐÍ(video,audio,pcr...)
	{
        	CSTCPrint("Ã»ÓÐÅäÖÃ×¢ÈëÀàÐÍ!!\n");
        	CSTCPrint("There's no stream type configed !!\n");
        	return -1;
	}

//²¥·ÅÍ¨µÀ
	psInjecterChnl->m_nDemux=psPlayerChnl->m_nDemux;

	pOpenParam->m_eInjecterType = EM_INJECTER_TS;
	pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_DEFAULT;

	return nStreamCnt;
}

//@CASEGROUP:CSUDIPLAYERProbe
//@DESCRIPTION:²âÊÔ²ÎÊýµÄÓÐÐ§ÐÔ£¬´«Èë²ÎÊý·Ç·¨£¬²é¿´ÊÇ·ñ·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@PRECONDITION:
//@INPUT:1¡¢²ÎÊýÎªCSUDI_NULL
//@EXPECTATION:·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@REMARK:
//@EXECUTIONFLOW:1¡¢´«Èë·Ç·¨²ÎÊýCSUDI_NULL£¬µ÷ÓÃCSUDIPLAYERProbe·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
BOOL CSTC_FILE_TEST_IT_CSUDIPLAYERProbe_0001(void)
{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERProbe(CSUDI_NULL), "´«Èë·Ç·¨²ÎÊý£¬Ó¦¸Ã·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_FATAL_POINT;

		return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback
//@DESCRIPTION:²âÊÔ²ÎÊý»òÓ¦ÓÃ³¡¾°ÔÚ²»·ûºÏ½Ó¿ÚÉè¼ÆÒªÇó£¬CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallbackº¯ÊýµÄÖ´ÐÐÐ§¹û
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢²ÎÊý²»·ûºÏÉè¼ÆÒªÇó
//@EXPECTATION:·ÇCSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback²ÎÊýÒªÇóÇë²Î¿¼UDIÒÆÖ²Ö¸ÄÏ
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢hPlayerÎªCSUDI_NULLÊ±£¬µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:3¡¢fnPlayerCallbackÎªCSUDI_NULL Ê±£¬µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢hPlayerÎªCSUDI_NULLÊ±£¬µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:5¡¢fnPlayerCallbackÎªCSUDI_NULLÊ±£¬µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢ÖØ¸´²½Öè2-²½Öè5Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_Add_DelPlayerCallback_0001(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int  nUserData=NUSERDATA;
	int i=0;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_FILE;

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1Ê§°Ü");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//testing CSUDIPLAYERAddFilePlayerCallback
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERAddFilePlayerCallback(CSUDI_NULL,FilePlayEventCallback,&nUserData), "²½Öè2 Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERAddFilePlayerCallback(hPlayer,CSUDI_NULL,&nUserData), "²½Öè3 Ê§°Ü");

		//testing CSUDIPLAYERDelFilePlayerCallback
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERDelFilePlayerCallback (CSUDI_NULL,FilePlayEventCallback,&nUserData), "²½Öè4 Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERDelFilePlayerCallback(hPlayer,CSUDI_NULL,&nUserData), "²½Öè5 Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "²½Öè14 Ê§°Ü");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback
//@DESCRIPTION:²âÊÔËùÓÐ²ÎÊý²¢ÇÒÓ¦ÓÃ³¡¾°¾ù·ûºÏ½Ó¿ÚÉè¼ÆÒªÇó£¬CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallbackº¯ÊýµÄÖ´ÐÐÐ§¹û
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢ËùÓÐ²ÎÊý¾ù·ûºÏ½Ó¿ÚÉè¼ÆÒªÇó
//@EXPECTATION:·µ»ØCSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback²ÎÊýÒªÇóÇë²Î¿¼UDIÒÆÖ²Ö¸ÄÏ
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallbackÌí¼Ó»Øµ÷º¯ÊýµÄÈý¸ö²»Í¬ÊÂÀý(pnUserDataÎªCSUDI_NULL,0x1234ÒÔ¼°ºÏÈÎÒâºÏ·¨µØÖ·Öµ)£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallbackÉ¾³ý²½Öè2ÖÐÌí¼ÓµÄÈý¸ö»Øµ÷º¯ÊýÊÂÀý£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢ÖØ¸´²½Öè2-²½Öè3Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_Add_DelPlayerCallback_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int  nUserData=NUSERDATA;
	int i=0;
	//´´½¨²¥·ÅÆ÷²ÎÊý
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_FILE;

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1 Ê§°Ü");

	for (i=0;i < REPEAT_TIMES; i++)
	{
		//testing CSUDIPLAYERAddPlayerCallback
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayEventCallback,&nUserData), "²½Öè2-1 Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayEventCallback,CSUDI_NULL), "²½Öè2-2 Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayEventCallback,(void *)0x1234), "²½Öè2-3 Ê§°Ü");

		//testing CSUDIPLAYERDelPlayerCallback
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayEventCallback, &nUserData), "²½Öè3-1 Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayEventCallback, CSUDI_NULL), "²½Öè3-2 Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayEventCallback, (void *)0x1234), "²½Öè3-3 Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "²½Öè5 Ê§°Ü");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback
//@DESCRIPTION:²âÊÔÀûÓÃCSUDIPLAYERAddFilePlayerCallback ×îÉÙÄÜ³É¹¦Ìí¼Ó32¸ö»Øµ÷º¯Êý
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢ËùÓÐ²ÎÊý¾ù·ûºÏ½Ó¿ÚÉè¼ÆÒªÇó
//@EXPECTATION:·µ»ØCSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback²ÎÊýÒªÇóÇë²Î¿¼UDIÒÆÖ²Ö¸ÄÏ
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallbackÌí¼Ó32¸ö»Øµ÷º¯ÊýÊÂÀý£¬ÆÚÍû¾ù·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallbackÉ¾³ý²½Öè2ÖÐÌí¼ÓµÄ32¸ö»Øµ÷º¯ÊýÊÂÀý£¬ÆÚÍû¾ù·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢ÖØ¸´²½Öè2-²½Öè3Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_Add_DelPlayerCallback_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int i=0, j=0;
	int  anUserData[33] = {0};
	//´´½¨²¥·ÅÆ÷²ÎÊý
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_FILE;

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1 Ê§°Ü");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//Ìí¼Ó32¸ö»Øµ÷
		for (j=0; j<32; j++)
		{
			anUserData[j] = j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayEventCallback,&anUserData[j]), "²½Öè2Ê§°Ü");
		}

		//É¾³ý32¸ö»Øµ÷
		for (j=0; j<32; j++)
		{
			anUserData[j] = j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback (hPlayer,FilePlayEventCallback,&anUserData[j]), "²½Öè3Ê§°Ü");
		}
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "²½Öè4 Ê§°Ü");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:²âÊÔ²ÎÊý»òÓ¦ÓÃ³¡¾°ÔÚ²»·ûºÏ½Ó¿ÚÉè¼ÆÒªÇó£¬CSUDIPLAYEROpen&CSUDIPLAYERCloseº¯ÊýµÄÖ´ÐÐÐ§¹û
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢²ÎÊý×éºÏ²»·ûºÏÉè¼ÆÒªÇó
//@EXPECTATION:·µ»Ø·ÇCSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢sPlayerChnl²»ÎªNULL£¬µ÷ÓÃCSUDIPLAYEROpen£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:2¡¢ePlayerType ÎªÎÞÐ§Öµ(Èç:EM_UDIPLAYER_MAXPLAYERTYPE)£¬µ÷ÓÃCSUDIPLAYEROpen£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢phPlayer ÎªCSUDI_NULL£¬µ÷ÓÃCSUDIPLAYEROpen£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢hPlayer ÎªCSUDI_NULL£¬µ÷ÓÃCSUDIPLAYERClose£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢ÖØ¸´²½Öè1-²½Öè4Ö¸¶¨´ÎÊý
CSUDI_BOOL CSTC_FILE_IT_CSUDIPLAYEROpenClose_0001(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_FILE;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S  sPlayerChnl;
	int i = 0;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = 0;
		sPlayerChnl.m_nVideoDecoder = 0;
		sPlayerChnl.m_nDemux = 0;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (NULL,EM_UDIPLAYER_MAXPLAYERTYPE,&hPlayer), "²½Öè2Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (NULL,ePlayerType,CSUDI_NULL), "²½Öè3Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClose (CSUDI_NULL), "²½Öè4Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:²âÊÔÕý³£Çé¿öÏÂ£¬CSUDIPLAYEROpen&CSUDIPLAYERCloseµÄÖ´ÐÐÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢ÄÜ°´ÎÄ¼þ²¥·Å·½Ê½Õý³£´ò¿ª²¥·ÅÆ÷
//@EXPECTATION:·µ»ØCSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬ÒÔÎÄ¼þ²¥·Å·½Ê½´ò¿ª²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERClose£¬¹Ø±Õ²½Öè2´ò¿ªµÄ²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢ÖØ¸´²½Öè1-²½Öè2Ö¸¶¨´ÎÊý
CSUDI_BOOL CSTC_FILE_IT_CSUDIPLAYEROpenClose_0002(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_FILE;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int i = 0;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "²½Öè2Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:²âÊÔÕý³£²ÎÊýÇé¿öÏÂ£¬Á¬Ðø¶à´Îµ÷ÓÃCSUDIPLAYEROpen&CSUDIPLAYERCloseµÄÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢ÄÜ°´ÎÄ¼þ²¥·Å·½Ê½Õý³£´ò¿ª²¥·ÅÆ÷
//@EXPECTATION:·µ»ØCSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬ÒÔÎÄ¼þ²¥·Å·½Ê½´ò¿ª²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢²ÎÊý²»±ä£¬ÔÙ´Îµ÷ÓÃCSUDIPLAYEROpen£¬ÒÔÎÄ¼þ²¥·Å·½Ê½´ò¿ª²¥·ÅÆ÷£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERClose£¬¹Ø±Õ²½Öè1´ò¿ªµÄ²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢²ÎÊý²»±ä£¬ÔÙ´Îµ÷ÓÃCSUDIPLAYERClose£¬¹Ø±Õ²½Öè1´ò¿ªµÄ²¥·ÅÆ÷£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢ÖØ¸´²½Öè1-²½Öè4Ö¸¶¨´ÎÊý
CSUDI_BOOL CSTC_FILE_IT_CSUDIPLAYEROpenClose_0003(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_FILE;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hHiPlayer = CSUDI_NULL;
	int i = 0;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (NULL,ePlayerType,&hHiPlayer), "²½Öè2Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "²½Öè3Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClose (hPlayer), "²½Öè4Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetPlaybackParam
//@DESCRIPTION:²âÊÔ²ÎÊý»òÓ¦ÓÃ³¡¾°ÔÚ²»·ûºÏ½Ó¿ÚÉè¼ÆÒªÇóµÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERSetStream&CSUDIPLAYERSetStream&CSUDIPLAYERGetPlaybackParamº¯ÊýµÄÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢²ÎÊý×éºÏ²»·ûºÏÉè¼ÆÒªÇó
//@EXPECTATION:·µ»Ø·ÇCSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢²âÊÔhPlayerÎªCSUDI_NULLµÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERSetStreamµÄÇé¿ö£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢²âÊÔpsStreamInfo²»ÎªCSUDI_NULLµÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERSetStreamµÄÇé¿ö£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢²âÊÔpsStreamInfo²»ÎªCSUDI_NULL²¢ÇÒnStreamCnt²»Îª0µÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERSetStreamµÄÇé¿ö£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢²âÊÔnStreamCnt²»Îª0µÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERSetStreamµÄÇé¿ö£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢²âÊÔstPlaybackParamÎªNULLµÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERSetStreamµÄÇé¿ö£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:7¡¢²âÊÔhPlayerÎªCSUDI_NULLµÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERGetPlaybackParamµÄÇé¿ö£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:8¡¢²âÊÔstPlaybackParamÎªNULLµÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERGetPlaybackParamµÄÇé¿ö£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:9¡¢ÖØ¸´²½Öè2-²½Öè8 Ö¸¶¨´ÎÊý
CSUDI_BOOL CSTC_FILE_TEST_IT_SetStream_GetPlaybackParam_0001(void)
{
	int nStreamCnt = 1;
	CSUDIStreamInfo_S asStreamInfo[10];
	CSUDIPlaybackParam_S stPlaybackParam;
	int  i= 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERSetStream (CSUDI_NULL,(void *)0, 0,&stPlaybackParam), "²½Öè2Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer, asStreamInfo,0,&stPlaybackParam), "²½Öè3Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,&stPlaybackParam), "²½Öè4Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,(void *)0,nStreamCnt,&stPlaybackParam), "²½Öè5Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,(void *)0,nStreamCnt,CSUDI_NULL), "²½Öè6Ê§°Ü");

		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetPlaybackParam (CSUDI_NULL,&stPlaybackParam), "²½Öè7Ê§°Ü");

		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetPlaybackParam (hPlayer,(void *)0), "²½Öè8Ê§°Ü");

	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡Ê§°Ü");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetPlaybackParam
//@DESCRIPTION:²âÊÔ²ÎÊý»òÓ¦ÓÃ³¡¾°ÔÚ·ûºÏ½Ó¿ÚÉè¼ÆÒªÇó£¬CSUDIPLAYERSetStream&CSUDIPLAYERGetPlaybackParamº¯ÊýµÄÖ´ÐÐÐ§¹û
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢²ÎÊý×éºÏ·ûºÏÉè¼ÆÒªÇó
//@EXPECTATION:·µ»ØCSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢²âÊÔÔÚ±¾µØÎÄ¼þ²¥·Å×´Ì¬ÏÂ£¬CSUDIPLAYERSetStreamº¯ÊýµÄÖ´ÐÐÇé¿ö
//@EXECUTIONFLOW:3¡¢²âÊÔÔÚ±¾µØÎÄ¼þ²¥·Å×´Ì¬ÏÂ£¬CSUDIPLAYERGetPlaybackParamº¯ÊýµÄÖ´ÐÐÇé¿ö
//@EXECUTIONFLOW:4¡¢ÖØ¸´²½Öè2-²½Öè3  Ö¸¶¨´ÎÊý
CSUDI_BOOL CSTC_FILE_TEST_IT_SetStream_GetPlaybackParam_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int  i= 0;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,(void *)0, 0,&stPlaybackParam), "²½Öè2Ê§°Ü");

		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPlaybackParam (hPlayer,&stPlaybackParam), "²½Öè3Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡Ê§°Ü");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   & CSUDIPLAYERStop
//@DESCRIPTION:²âÊÔ²¥·ÅÆ÷Ö¸Õë·Ç·¨µÄÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢hPlayer=CSUDI_NULL
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERStart£¬Ê¹hPlayer=CSUDI_NULL£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERStop£¬Ê¹hPlayer=CSUDI_NULL£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_FILE_TEST_IT_StartStop_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERStart(CSUDI_NULL), "²½Öè1Ê§°Ü");
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERStop(CSUDI_NULL), "²½Öè2Ê§°Ü");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   & CSUDIPLAYERStop
//@DESCRIPTION:²âÊÔÕý³£Æô¶¯/Í£Ö¹Ò»¸ö²¥·ÅÆ÷µÄÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢hPlayerÓÐÐ§
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream,ÉèÖÃÎÄ¼þÊôÐÔÐÅÏ¢£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERStart£¬Æô¶¯²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESSÇÒÕý³£²¥·Å
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESSÇÒÕý³£Í£Ö¹
//@EXECUTIONFLOW:5¡¢ÖØ¸´²½Öè3-²½Öè4Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:6¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_StartStop_0002(void)
{
	int i = 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	CSTCPrint("±¾ÓÃÀý²âÊÔÎÄ¼þ²¥·ÅµÄÕý³£Æô¶¯ºÍÍ£Ö¹£¬²âÊÔÎÄ¼þÎª: \"%s\"\n", stPlaybackParam.m_szFileName);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "²½Öè2Ê§°Ü");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "×¢²á»Øµ÷Ê§°Ü");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè3Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»´ÓÍ·¿ªÊ¼Õý³£²¥·Å£¿\n");
    	CSTCPrint("Is A/V output normal ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè3Ê§°Ü£º²¥·Å²»Õý³£");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè4Ê§°Ü");

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË£¿\n");
  		CSTCPrint("Does the player stop ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè4Ê§°Ü£ºÍ£Ö¹²¥·Å²»Õý³£");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "É¾³ý»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart
//@DESCRIPTION:ÔÚÓ¦ÓÃ³¡¾°²»·ûºÏÉè¼ÆÒªÇóµÄÇé¿öÏÂ£¬µ÷ÓÃCSUDIPLAYERStart/ CSUDIPLAYERStop,ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢hPlayerÓÐÐ§
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_STATUS
//@REMARK:CSUDIPLAYERStartÖ»ÄÜÔÚÉèÖÃÎÄ¼þÐÅÏ¢Ö®ºóÓëÍ£Ö¹²¥·ÅÆ÷Ö®ºóµ÷ÓÃ£¬CSUDIPLAYERStopÖ»ÄÜÔÚÆô¶¯²¥·ÅÆ÷Ö®ºó£¬Í£Ö¹²¥·ÅÆ÷Ö®Ç°²ÅÄÜµ÷ÓÃ
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢CSUDIPLAYEROpenÖ®ºóÁ¢¼´µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢CSUDIPLAYEROpenÖ®ºóÁ¢¼´µ÷ÓÃCSUDIPLAYERStart£¬Æô¶¯²¥·ÅÆ÷£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢CSUDIPLAYEROpenÖ®ºóµ÷ÓÃCSUDIPLAYERSetStream,ÉèÖÃÁ÷ÊôÐÔÐÅÏ¢£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢CSUDIPLAYERSetStreamÖ®ºó¡¢CSUDIPLAYERStartÖ®Ç°£¬µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERStart£¬Æô¶¯²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:7¡¢ÔÙ´Îµ÷ÓÃCSUDIPLAYERStart£¬Æô¶¯²¥·ÅÆ÷£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:9¡¢ÔÙ´Îµ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:10¡¢ÖØ¸´²½Öè5-²½Öè9Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:11¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_StartStop_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int i=0;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer), "²½Öè3Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "²½Öè4Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "×¢²á»Øµ÷Ê§°Ü");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "²½Öè5Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè6Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer), "²½Öè7Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "²½Öè8Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "²½Öè9Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "É¾³ý»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:²âÊÔ²¥·ÅÆ÷¾ä±úÎªCSUDI_NULLÊ±£¬CSUDIPLAYERPause & CSUDIPLAYERResumeµÄµ÷ÓÃÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢hPlayer=CSUDI_NULL
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERPause£¬Ê¹hPlayer=CSUDI_NULL£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERResume£¬Ê¹hPlayer=CSUDI_NULL£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_FILE_TEST_IT_PauseResume_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERPause(CSUDI_NULL), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERResume(CSUDI_NULL), "²½Öè2Ê§°Ü");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:²âÊÔÓ¦ÓÃ³¡¾°²»·ûºÏÉè¼ÆÒªÇóÊ±£¬CSUDIPLAYERPause & CSUDIPLAYERResumeº¯ÊýµÄÖ´ÐÐÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXPECTATION:Ö±²¥²¥·ÅÆ÷¿ÉÒÔ²»Ö§³ÖCSUDIPLAYERPause & CSUDIPLAYERResume
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢Open²¥·ÅÆ÷ºóÁ¢¼´µ÷ÓÃCSUDIPLAYERPause£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢Open²¥·ÅÆ÷ºóÁ¢¼´µ÷ÓÃCSUDIPLAYERResume£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢Open²¥·ÅÆ÷ºóµ÷ÓÃCSUDIPLAYERSetStreamÉèÖÃÁ÷ÊôÐÔÐÅÏ¢£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢Start²¥·ÅÖ®Ç°£¬µ÷ÓÃCSUDIPLAYERPause£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢Start²¥·ÅÖ®Ç°£¬µ÷ÓÃCSUDIPLAYERResume£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStart£¬Start²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:8¡¢StartÖ®ºó¡¢Pause²¥·ÅÖ®Ç°£¬µ÷ÓÃCSUDIPLAYERResume£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERPauseÔÝÍ£²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS »òCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:10¡¢Èç¹ûCSUDIPLAYERPause³É¹¦£¬Ôòµ÷ÓÃCSUDIPLAYERStart£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:11¡¢Èç¹ûCSUDIPLAYERPause³É¹¦£¬Ôòµ÷ÓÃCSUDIPLAYERSetStream£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:12¡¢Èç¹ûCSUDIPLAYERPause³É¹¦£¬ÔòÔÙ´Îµ÷ÓÃCSUDIPLAYERPause£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:13¡¢Èç¹ûCSUDIPLAYERPause³É¹¦£¬Ôòµ÷ÓÃCSUDIPLAYERResume£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:14¡¢Èç¹ûCSUDIPLAYERPause³É¹¦£¬ÔòÔÙ´Îµ÷ÓÃCSUDIPLAYERResume£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:15¡¢Èç¹ûCSUDIPLAYERPause³É¹¦£¬Ôòµ÷ÓÃCSUDIPLAYERPause£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:16¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:17¡¢ÖØ¸´²½Öè5-²½Öè16Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:18¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_PauseResume_0002(void)
{
	int i = 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause (hPlayer), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "²½Öè3Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "²½Öè4Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "×¢²á»Øµ÷Ê§°Ü");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause (hPlayer), "²½Öè5Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "²½Öè6Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè7Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "²½Öè8Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		udiRe = CSUDIPLAYERPause(hPlayer);
		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED==udiRe)),"²½Öè9Ê§°Ü");

		if (CSUDI_SUCCESS == udiRe)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERStart(hPlayer), "²½Öè10Ê§°Ü");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "²½Öè11Ê§°Ü");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause(hPlayer), "²½Öè12Ê§°Ü");

			CSUDIOSThreadSleep(SLEEP_TIME);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "²½Öè13Ê§°Ü");

			CSUDIOSThreadSleep(SLEEP_TIME);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "²½Öè14Ê§°Ü");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "²½Öè15Ê§°Ü");

			CSUDIOSThreadSleep(SLEEP_TIME);
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè16Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "É¾³ý»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡Ê§°Ü");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:²âÊÔÕý³£ÔÝÍ£/»Ö¸´Ò»¸ö²¥·ÅÆ÷µÄÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢hPlayerÓÐÐ§
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream,ÉèÖÃÎÄ¼þÊôÐÔÐÅÏ¢£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERStart£¬Æô¶¯²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESSÇÒÕý³£²¥·Å
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERPause£¬ÔÝÍ£²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESSÇÒÕý³£ÔÝÍ£
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERResume£¬»Ö¸´²¥·Å£¬ÆÚÍû·µ»ØCSUDI_SUCCESSÇÒÕý³£»Ö¸´²¥·Å
//@EXECUTIONFLOW:6¡¢ÖØ¸´²½Öè4-²½Öè5Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESSÇÒÕý³£Í£Ö¹
//@EXECUTIONFLOW:8¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_PauseResume_0003(void)
{
	int i = 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	CSTCPrint("±¾ÓÃÀý²âÊÔÎÄ¼þ²¥·ÅµÄÕý³£ÔÝÍ£ºÍ»Ö¸´£¬²âÊÔÎÄ¼þÎª: \"%s\"\n", stPlaybackParam.m_szFileName);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè3Ê§°Ü");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»Õý³£²¥·Å£¿\n");
	CSTCPrint("Is A/V output normal ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè3Ê§°Ü£º²¥·Å²»Õý³£");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer), "²½Öè4Ê§°Ü");

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÔÝÍ£²¥·ÅÁË£¿\n");
  		CSTCPrint("Does the player pause ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè4Ê§°Ü£ºÔÝÍ£²¥·Å²»Õý³£");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer), "²½Öè5Ê§°Ü");

		CSTCPrint("ÒôÊÓÆµÊÇ·ñ´ÓÔÝÍ£µã»Ö¸´²¥·ÅÁË£¿\n");
  		CSTCPrint("Does the player resume ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè5Ê§°Ü£º»Ö¸´²¥·Å²»Õý³£");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "²½Öè7Ê§°Ü");

	CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË£¿\n");
	CSTCPrint("Does the player stop ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè7Ê§°Ü£ºÍ£Ö¹²¥·Å²»Õý³£");

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetSpeed&CSUDIPLAYERGetSpeed
//@DESCRIPTION:1¡¢²âÊÔ±¾µØÎÄ¼þ²¥·ÅÇé¿öÏÂ£¬CSUDIPLAYERSetSpeedº¯ÊýµÄÖ´ÐÐÇé¿ö
//@DESCRIPTION:2¡¢²âÊÔ±¾µØÎÄ¼þ²¥·ÅÇé¿öÏÂ£¬CSUDIPLAYERGetSpeedº¯ÊýµÄÖ´ÐÐÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢hPlayerÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERSetSpeed£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢eSpeedÎªÎÞÐ§Öµ£¬µ÷ÓÃ CSUDIPLAYERSetSpeed£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢hPlayerÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERGetSpeed£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢eSpeedÎªÎÞÐ§Öµ£¬µ÷ÓÃ CSUDIPLAYERGetSpeed£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:7¡¢²ÎÊýÕýÈ·µÄÇé¿öÏÂ£¬Start²¥·ÅÆ÷Ö®Ç°µ÷ÓÃCSUDIPLAYERSetSpeed£¬ÆÚÍû·µ»ØµÄ²»ÎªCSUDI_SUCCESS»òÎªCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:8¡¢²ÎÊýÕýÈ·µÄÇé¿öÏÂ£¬Start²¥·ÅÆ÷Ö®Ç°µ÷ÓÃCSUDIPLAYERGetSpeed£¬ÆÚÍû·µ»ØµÄ²»ÎªCSUDI_SUCCESS»òÎªCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERStart£¬Start²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:10¡¢²ÎÊýÕýÈ·µÄÇé¿öÏÂ£¬StartÖ®ºóµ÷ÓÃCSUDIPLAYERSetSpeed£¬ÆÚÍû·µ»ØCSUDI_SUCCESS»òCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:11¡¢²ÎÊýÕýÈ·µÄÇé¿öÏÂ£¬StartÖ®ºóµ÷ÓÃCSUDIPLAYERGetSpeed£¬ÆÚÍû·µ»ØCSUDI_SUCCESS»òCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:12¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:13¡¢ÖØ¸´²½Öè3-²½Öè11Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:14¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_SetGetSpeed_0001(void)
{
	int i = 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code errorCode = CSUDI_SUCCESS;
	CSUDIPlayerSpeed_E eSpeed;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer,NULL,0,&stPlaybackParam), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "×¢²á»Øµ÷Ê§°Ü");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERSetSpeed(CSUDI_NULL,eSpeed), "²½Öè3Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSetSpeed(CSUDI_NULL,EM_UDIPLAYER_SPEED_FASTFORWARD_32+10), "²½Öè4Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERGetSpeed(CSUDI_NULL,&eSpeed), "²½Öè5Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetSpeed(hPlayer,CSUDI_NULL), "²½Öè6Ê§°Ü");

		errorCode = CSUDIPLAYERSetSpeed(hPlayer,eSpeed);

		//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS != errorCode), "²½Öè7Ê§°Ü");

		errorCode = CSUDIPLAYERGetSpeed(hPlayer,&eSpeed);

		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "²½Öè8Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè9Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		errorCode = CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_32);

		//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "²½Öè10Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		errorCode = CSUDIPLAYERGetSpeed(hPlayer,&eSpeed);

        CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "²½Öè11Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè12Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, NULL), "É¾³ý»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetSpeed&CSUDIPLAYERGetSpeed
//@DESCRIPTION:1¡¢²âÊÔ±¾µØ²¥·ÅÇé¿öÏÂ£¬CSUDIPLAYERSetSpeedº¯ÊýµÄÖ´ÐÐÇé¿ö
//@DESCRIPTION:2¡¢²âÊÔ±¾µØ²¥·ÅÇé¿öÏÂ£¬CSUDIPLAYERGetSpeedº¯ÊýµÄÖ´ÐÐÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢hPlayerÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERSetSpeed£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢eSpeedÎªÎÞÐ§Öµ£¬µ÷ÓÃ CSUDIPLAYERSetSpeed£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢hPlayerÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERGetSpeed£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢eSpeedÎªÎÞÐ§Öµ£¬µ÷ÓÃ CSUDIPLAYERGetSpeed£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:7¡¢²ÎÊýÕýÈ·µÄÇé¿öÏÂ£¬Start²¥·ÅÆ÷Ö®Ç°µ÷ÓÃCSUDIPLAYERSetSpeed£¬ÆÚÍû·µ»ØµÄ²»ÎªCSUDI_SUCCESS»òÎªCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:8¡¢²ÎÊýÕýÈ·µÄÇé¿öÏÂ£¬Start²¥·ÅÆ÷Ö®Ç°µ÷ÓÃCSUDIPLAYERGetSpeed£¬ÆÚÍû·µ»ØµÄ²»ÎªCSUDI_SUCCESS»òÎªCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERStart£¬Start²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:10¡¢²ÎÊýÕýÈ·µÄÇé¿öÏÂ£¬StartÖ®ºóµ÷ÓÃCSUDIPLAYERSetSpeed£¬ÆÚÍû·µ»ØCSUDI_SUCCESS»òCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:11¡¢²ÎÊýÕýÈ·µÄÇé¿öÏÂ£¬StartÖ®ºóµ÷ÓÃCSUDIPLAYERGetSpeed£¬ÆÚÍû·µ»ØCSUDI_SUCCESS»òCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:12¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:13¡¢ÖØ¸´²½Öè3-²½Öè11Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:14¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_SetGetSpeed_0002(void)
{
	int i = 0;
	unsigned char moveToend = 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code errorCode = CSUDI_SUCCESS;
	CSUDIPlayerSpeed_E eSpeed;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_FILE;
	CSUDI_BOOL bPlaying = CSUDI_FALSE;
	CSUDI_BOOL bAddCallback = CSUDI_FALSE;

	memset(&stPlaybackParam, 0, sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL   != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback, &moveToend), "×¢²á»Øµ÷Ê§°Ü");

	bAddCallback = CSUDI_TRUE;

	for (i = 0;i < REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE   == CSUDIPLAYERSetSpeed(CSUDI_NULL, eSpeed), "²½Öè3Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   != CSUDIPLAYERSetSpeed(CSUDI_NULL, EM_UDIPLAYER_SPEED_FASTFORWARD_32+10), "²½Öè4Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE   == CSUDIPLAYERGetSpeed(CSUDI_NULL, &eSpeed), "²½Öè5Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   != CSUDIPLAYERGetSpeed(hPlayer,CSUDI_NULL), "²½Öè6Ê§°Ü");

		errorCode = CSUDIPLAYERSetSpeed(hPlayer, eSpeed);

		//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode) || (CSUDI_SUCCESS != errorCode), "²½Öè7Ê§°Ü");

		errorCode = CSUDIPLAYERGetSpeed(hPlayer, &eSpeed);

		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode) || (CSUDI_SUCCESS == errorCode), "²½Öè8Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè9Ê§°Ü");

		bPlaying = CSUDI_TRUE;

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜÕý³£²¥·Å£¿\n");
		CSTCPrint("Is A/V output normal ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè9Ê§°Ü£º²¥·Å²»Õý³£");

		moveToend = 0;
		errorCode = CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_32);

		//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode) || (CSUDI_SUCCESS == errorCode), "²½Öè10Ê§°Ü");

		CSTCPrint("ÊÓÆµÊÇ·ñ¿ì½ø²¥·Å£¿ \n");
		CSTCPrint("Is A/V output fast forward?");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè10Ê§°Ü£º¿ì½ø²»Õý³£");

		if (FORWARD_TO_END == moveToend)
		{
			CSTCPrint("ÊÓÆµÒÑ¾­¿ì½øµ½Ä©Î²£¡\n");
			break;
		}

		errorCode = CSUDIPLAYERGetSpeed(hPlayer, &eSpeed);

        CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode) || (CSUDI_SUCCESS == errorCode), "²½Öè11Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   == CSUDIPLAYERStop(hPlayer), "²½Öè12Ê§°Ü");

		bPlaying = CSUDI_FALSE;

		CSTCPrint("ÊÓÆµÊÇ·ñÍ£Ö¹²¥·Å£¿ \n");
		CSTCPrint("Is A/V output stop?");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè10Ê§°Ü£ºÍ£Ö¹²»Õý³£");
	}

	CSTK_FATAL_POINT;

	if (bPlaying)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   == CSUDIPLAYERStop(hPlayer), "Í£Ö¹²¥·ÅÊ§°Ü");
	}

	if (bAddCallback)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, &moveToend), "É¾³ý»Øµ÷Ê§°Ü");
	}

	if (CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   == CSUDIPLAYERClose(hPlayer), "»Ö¸´ÏÖ³¡");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSeek
//@DESCRIPTION:1¡¢²âÊÔ±¾µØÎÄ¼þ²¥·ÅÇé¿öÏÂ£¬½Ó¿Úº¯ÊýCSUDIPLAYERSeekµÄÖ´ÐÐÇé¿ö
//@DESCRIPTION:2¡¢CSUDIPLAYERSeek·ÖÈýÖÖÇé¿ö½øÐÐÑéÖ¤
//@DESCRIPTION:3¡¢CSUDIPLAYERSeek CSUDIPLAYERGetCurPosInSec×éºÏÊ¹ÓÃÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:1¡¢ÓÐÐ§µÄhPlayer
//@INPUT:2¡¢ÓÐÐ§µÄnPosInSec
//@INPUT:3¡¢ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_CURRENT
//@EXPECTATION:Ö±²¥Ä£Ê½ÏÂ²»Ö§³Ö  CSUDIPLAYERSeek
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERStart£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢hPlayerÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERSeek£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢ÒÔÕý³£ºÏ·¨²ÎÊýµ÷ÓÃ CSUDIPLAYERSeek£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢ÖØ¸´²½Öè3-²½Öè4Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:8¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_Seek_0001(void)
{
	CSUDIPlayPosition_E   ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_CURRENT;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback, NULL), "×¢²á»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè3Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSeek(CSUDI_NULL,100,ePlayPosFlag), "²½Öè4Ê§°Ü");

	CSUDIOSThreadSleep(SLEEP_TIME);

	for (ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_HEAD;ePlayPosFlag<=EM_UDIPLAYER_POSITION_FROM_END;ePlayPosFlag++)
	{
		if (ePlayPosFlag == EM_UDIPLAYER_POSITION_FROM_END)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSeek(hPlayer,-100 ,ePlayPosFlag), "²½Öè5Ê§°Ü");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSeek(hPlayer, 50, ePlayPosFlag), "²½Öè5Ê§°Ü");
		}

		CSUDIOSThreadSleep(SLEEP_TIME);
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè7Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback, NULL), "É¾³ý»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPosInSec
//@DESCRIPTION:1¡¢²âÊÔ±¾µØÎÄ¼þ²¥·ÅÇé¿öÏÂ£¬½Ó¿Úº¯ÊýCSUDIPLAYERGetCurPosInSecµÄÖ´ÐÐÇé¿ö
//@DESCRIPTION:2¡¢CSUDIPLAYERGetCurPosInSec  CSUDIPLAYERSetSpeed  CSUDIPLAYERSeek×éºÏÊ¹ÓÃÇé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢hPlayerÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERGetCurPosInSec£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢pnPosInSecÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERGetCurPosInSec£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢ÒÔÕý³£ºÏ·¨²ÎÊýµ÷ÓÃ CSUDIPLAYERStart£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢ÒÔÕý³£ºÏ·¨²ÎÊýµ÷ÓÃ CSUDIPLAYERSeek£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:7¡¢ÒÔÕý³£ºÏ·¨²ÎÊýµ÷ÓÃ CSUDIPLAYERResume£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:7¡¢ÒÔÕý³£ºÏ·¨²ÎÊýµ÷ÓÃ CSUDIPLAYERGetCurPosInSec£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:8¡¢ÒÔÕý³£ºÏ·¨²ÎÊýµ÷ÓÃ CSUDIPLAYERStop£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:9¡¢ÖØ¸´²½Öè3-²½Öè5Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:10¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_GetCurPosInSec_0001(void)
{
	int  nPosInSec = 0;
	CSUDIPlaybackParam_S stPlaybackParam;
	int i= 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
    CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "×¢²á»Øµ÷Ê§°Ü");

    for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(CSUDI_NULL,&nPosInSec), "²½Öè3Ê§°Ü");

		//check porting code pls,it have not check pnPosInSec
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(hPlayer,CSUDI_NULL), "²½Öè4Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè5Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 100, EM_UDIPLAYER_POSITION_FROM_CURRENT), "²½Öè6Ê§°Ü");

		//CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer), "»Ö¸´²¥·ÅÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetCurPosInSec(hPlayer,&nPosInSec), "²½Öè7Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè8Ê§°Ü");

		CSUDIOSThreadSleep(300);//sleep 300ms ÊÇÎªÁËstopÖ®ºóÄÜÍêÈ«Çå¿ÕÊÓÆµ»º´æ
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "É¾³ý»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡Ê§°Ü");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetDuration
//@DESCRIPTION:1¡¢²âÊÔ±¾µØÎÄ¼þ²¥·ÅÇé¿öÏÂ£¬½Ó¿Úº¯ÊýCSUDIPLAYERGetDurationµÄÖ´ÐÐÇé¿ö
//@DESCRIPTION:2¡¢ÕâÀïÔÊÐí»ñµÃµÄ×ÜÊ±¼äºÍÊµ¼ÊÇé¿öÎó²î1Ãë
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1¡¢´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERStart£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:4¡¢hPlayerÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERGetDuration£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:5¡¢pnPosInSecÎªCSUDI_NULL£¬µ÷ÓÃ CSUDIPLAYERGetDuration£¬ÆÚÍû·µ»Ø·ÇCSUDI_SUCCESS
//@EXECUTIONFLOW:6¡¢ÒÔÕý³£ºÏ·¨²ÎÊýµ÷ÓÃ CSUDIPLAYERGetDuration£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:8¡¢ÖØ¸´²½Öè3-²½Öè5Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:9¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_GetDuration_0001(void)
{
	int  nPosInSec = 0;
	CSUDIPlaybackParam_S stPlaybackParam;
	int i = 0;
	int j = EM_UDI_FILENAME_WMV;
	CSUDI_Error_Code eCode = CSUDI_SUCCESS;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	//´´½¨²¥·ÅÆ÷
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "×¢²á»Øµ÷Ê§°Ü");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart (hPlayer), "²½Öè3Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetDuration(CSUDI_NULL,&nPosInSec), "²½Öè4Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetDuration(hPlayer,NULL), "²½Öè5Ê§°Ü");

		eCode = CSUDIPLAYERGetDuration(hPlayer,&nPosInSec);

		int value = 0;
		value = nPosInSec - s_svrfiletime[i];

		if (nPosInSec >= s_svrfiletime[i])
		{
			CSTK_ASSERT_TRUE_FATAL( value >= 0 && value <= 1, "»ñÈ¡ÎÄ¼þ×ÜÊ±¼äÊ§°Ü");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(value <= -1, "»ñÈ¡ÎÄ¼þ×ÜÊ±¼äÊ§°Ü");
		}

		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == eCode)||(CSUDI_SUCCESS == eCode), "²½Öè6Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè7Ê§°Ü");

		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;

		j++;

		strncpy(stPlaybackParam.m_szFileName, stFilePath[j].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "ÖØÐÂÉèÖÃ²¥·ÅÊôÐÔÊ§°Ü");

	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "É¾³ý»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "»Ö¸´ÏÖ³¡Ê§°Ü");

	return CSUDI_TRUE;
}


static CSUDI_BOOL FILE_TEST_IT_Play_Base(int formatType)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDI_BOOL bPlaying = CSUDI_FALSE;
	CSUDI_BOOL bAddCallback = CSUDI_FALSE;

	memset(&stPlaybackParam, 0, sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[formatType].filename, CSUDI_MAX_FILE_NAME_LEN);

	CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName)), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "²½Öè2Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè2Ê§°Ü£º´ò¿ªÎÄ¼þÒì³£");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "²½Öè3ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback,  CSUDI_NULL), "²½Öè4Ìí¼Ó»Øµ÷Ê§°Ü");

	bAddCallback = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè5Ê§°Ü");

	bPlaying = CSUDI_TRUE;

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»Õý³£²¥·Å£¿\n");
	CSTCPrint("Is A/V output normal ?\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè6Ê§°Ü£º²¥·Å²»Õý³£");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "²½Öè7Ê§°Ü");

	bPlaying = CSUDI_FALSE;

	CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË\n");
	CSTCPrint("Does the player stop ?\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè8Ê§°Ü£ºÍ£Ö¹²¥·Å²»Õý³£");
	
	CSTK_FATAL_POINT;

	if (bPlaying)
	{
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Í£Ö¹²¥·ÅÊ§°Ü");
	}

	if (bAddCallback)
	{
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, CSUDI_NULL), "É¾³ý»Øµ÷Ê§°Ü");
	}
			
	if (CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "¹Ø±ÕÎÄ¼þÊ§°Ü");
		
		hPlayer = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·Å±êÇåTS Á÷
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ýÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0001(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_TS_SD);

	return CSUDI_TRUE;
}


//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·Å¸ßÇåTSÁ÷
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼Ó²¥·Åµ½ÎÄ¼þÎ²µÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0002(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_TS_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅWMV¸ñÊ½µÄÊý¾ÝÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼Ó²¥·Åµ½ÎÄ¼þÎ²µÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0003(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_WMV);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅAVI¸ñÊ½±êÇåÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0004(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_AVI_SD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅAVI¸ñÊ½¸ßÇåÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0005(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_AVI_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅRMVB¸ñÊ½±êÇåÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0006(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_RMVB_SD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅRMVB¸ñÊ½¸ßÇåÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0007(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_RMVB_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅFLV¸ñÊ½ÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0008(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_FLV);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅMP3¸ñÊ½ÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÒôÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0009(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MP3);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅM4A¸ñÊ½ÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÒôÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0010(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_M4A);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅAC3¸ñÊ½ÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÒôÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0011(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_AC3);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅMP3¸ñÊ½ÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÒôÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0012(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_AAC);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅMP4¸ñÊ½±êÇåÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0013(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MP4_SD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅMP4¸ñÊ½¸ßÇåÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0014(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MP4_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·Å3GP¸ñÊ½ÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0015(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_3GP);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅMOV¸ñÊ½ÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0016(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MOV);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅMKV¸ñÊ½±êÇåÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0017(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MKV_SD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅMKV¸ñÊ½¸ßÇåÎÄ¼þ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0018(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MKV_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÖ§³Ö²¥·ÅMKV¸ñÊ½¸ßÇå´óÎÄ¼þ£¬´óÐ¡ÔÚ10GBÒÔÉÏ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:10    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0019(void)
{
	CSTCPrint("±¾ÓÃÀýÓÃÓÚ²âÊÔ²¥·Å´óÎÄ¼þ£¬ÇëÈ·±£²âÊÔÊý¾Ý´óÓÚ10GB!\n");
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MKV_HD_BSIZE);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þ²¥·ÅÊÇ·ñÖ§³Ö²»Í¬ÎÄ¼þÖ®¼äÇÐ»»
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÒôÆµ×ÊÔ´¸Ä±äµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:5¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:7¡¢ÔÙ´Îµ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃÐÂÎÄ¼þµÄ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:9¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÒôÊÓÆµÊÇ·ñ¿ªÊ¼²¥·Å
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:11¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:12¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:13¡¢ÖØ¸´²½Öè2-²½Öè10
//@EXECUTIONFLOW:14    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_0020(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDIPlaybackParam_S stPlaybackParam;
	int i = 0;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

	CSTCPrint("±¾ÓÃÀý²âÊÔ±¾µØÎÄ¼þ²¥·Å²»Í¬ÎÄ¼þÖ®¼äÇÐ»»\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	for(i = 0; i < REPEAT_TIMES; i++)
	{
		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;
		strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MKV_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTCPrint("µÚ %d ´Î²âÊÔ \"%s\" µÄ²¥·Å\n", i+1, stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback,  CSUDI_NULL), "²½Öè3Ìí¼Ó»Øµ÷Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè4Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»Õý³£²¥·Å£¿\n");
		CSTCPrint("Is A/V output normal ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè5Ê§°Ü£º²¥·Å²»Õý³£");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè6Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË\n");
  		CSTCPrint("Does the player stop ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè7Ê§°Ü£ºÍ£Ö¹²¥·ÅÊ§°Ü");

		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;
		strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTCPrint("µÚ %d ´Î²âÊÔ \"%s\" µÄ²¥·Å\n", i+1, stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè7ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè8Ê§°Ü");

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»Õý³£²¥·Å£¿\n");
		CSTCPrint("Is A/V output normal ?\n");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè9Ê§°Ü£º²¥·Å²»Õý³£");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè10Ê§°Ü");

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË\n");
  		CSTCPrint("Does the player stop ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè11Ê§°Ü£ºÍ£Ö¹²¥·Å²»Õý³£");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback,  CSUDI_NULL), "²½Öè12É¾³ý»Øµ÷Ê§°Ü");
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "²½Öè14Ê§°Ü");
		hPlayer=CSUDI_NULL;
	}

	return CSUDI_TRUE;
}


//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þ²¥·ÅÓëÖ±²¥ÄÜ¹»Õý³£ÇÐ»»
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÊÓÆµ×ÊÔ´¸Ä±äµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:5¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:8¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:9    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸öÖ±²¥²¥·ÅÆ÷
//@EXECUTIONFLOW:11¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:12¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÊÓÆµ×ÊÔ´¸Ä±äµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:13¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:14¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:15¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:16¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:17¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:18    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
//@EXECUTIONFLOW:19¡¢ÖØ¸´²½Öè1-²½Öè18Ö¸¶¨´ÎÊý
//@EXECUTIONFLOW:20¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_0021(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hLivePlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S stPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[3];  //²»»á³¬¹ý10¸ö
	int nStreamCnt = 0;
	CSUDIPlaybackParam_S stPlaybackParam;
	int i = 0;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;

	CSTCPrint("±¾ÓÃÀý²âÊÔ±¾µØÎÄ¼þ²¥·ÅÓëÖ±²¥µÄÇÐ»»\n");

	for(i = 0; i < REPEAT_TIMES; i++)
	{
		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		ePlayerType = EM_UDIPLAYER_FILE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;

		strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MKV_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTCPrint("µÚ %d ´Î²âÊÔ \"%s\" µÄÎÄ¼þ²¥·Å\n", i+1, stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback, CSUDI_NULL), "²½Öè3Ìí¼Ó»Øµ÷Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè4Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»Õý³£²¥·Å£¿\n");
		CSTCPrint("Is A/V output normal ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè5Ê§°Ü£º²¥·Å²»Õý³£");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè6Ê§°Ü");

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË\n");
  		CSTCPrint("Does the player stop ?\n");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback,  CSUDI_NULL), "²½Öè7É¾³ý»Øµ÷Ê§°Ü");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè8Ê§°Ü£ºÍ£Ö¹²¥·Å²»Õý³£");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "²½Öè9Ê§°Ü");

  		hPlayer=CSUDI_NULL;

		CSUDIOSThreadSleep(SLEEP_TIME);

		ePlayerType = EM_UDIPLAYER_LIVE;

		stPlayerChnl.m_nDemux = 0;
		stPlayerChnl.m_nAudioDecoder = 0;
		stPlayerChnl.m_nVideoDecoder = 0;

		CSTCPrint("µÚ %d ´Î²âÊÔ \"%s\" µÄÖ±²¥²¥·Å\n", i+1, g_sPlayer_SeviceInfo[LivePlayer].m_pcTsFilename);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== PLAYER_iLockTuner (), "ËøÆµÊ§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,ePlayerType,&hLivePlayer), "²½Öè10Ê§°Ü");

		sStreamInfo[0].m_nPid = g_sPlayer_SeviceInfo[LivePlayer].m_nVideoPid;
		sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
		sStreamInfo[0].m_uStreamType.m_eVideoType = g_sPlayer_SeviceInfo[LivePlayer].m_eVidStreamType;

		sStreamInfo[1].m_nPid = g_sPlayer_SeviceInfo[LivePlayer].m_nAudioPid;
		sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
		sStreamInfo[1].m_uStreamType.m_eAudioType = g_sPlayer_SeviceInfo[LivePlayer].m_eAudStreamType;
		if (g_sPlayer_SeviceInfo[LivePlayer].m_nPcrPid <= 0)
		{
			nStreamCnt = 2;
		}
		else
		{
			sStreamInfo[2].m_nPid = g_sPlayer_SeviceInfo[LivePlayer].m_nPcrPid;
			sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_PCR;
			nStreamCnt = 3;
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hLivePlayer, sStreamInfo, nStreamCnt, NULL), "²½Öè11ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERAddPlayerCallback(hLivePlayer, PlayCallback , EM_UDIPLAYER_VIDEO_FRAME_COMING, CSUDI_NULL), "²½Öè12Ìí¼Ó»Øµ÷Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hLivePlayer), "²½Öè13Ê§°Ü");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»Õý³£²¥·Å£¿\n");
		CSTCPrint("Is A/V output normal ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè14Ê§°Ü£º²¥·Å²»Õý³£");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hLivePlayer), "²½Öè15Ê§°Ü");
		CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË\n");
  		CSTCPrint("Does the player stop ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè16Ê§°Ü£ºÍ£Ö¹²¥·Å²»Õý³£");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hLivePlayer, PlayCallback , EM_UDIPLAYER_VIDEO_FRAME_COMING, CSUDI_NULL), "²½Öè17É¾³ý»Øµ÷Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "²½Öè18Ê§°Ü");

		hLivePlayer=CSUDI_NULL;
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "²½Öè20Ê§°Ü");
		hPlayer=CSUDI_NULL;
	}

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "²½Öè21Ê§°Ü");
		hLivePlayer=CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þ²¥·ÅÓë×¢Èë²¥·ÅÄÜ¹»Õý³£ÇÐ»»
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÊÓÆµ×ÊÔ´¸Ä±äµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:5¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ý»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:8¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÒôÊÓÆµÍ£Ö¹²¥·Å
//@EXECUTIONFLOW:9    µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSUDIINJECTEROpen£¬´´½¨Ò»¸ö×¢ÈëTS  ÂëÁ÷µÄ×¢ÈëÊµÀý£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:11¡¢µ÷ÓÃCSUDIPLAYEROpen £¬´´½¨Ò»¸ö×¢ÈëµÄ²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:12¡¢µ÷ÓÃCSUDIINJECTERSetAttribute £¬ÉèÖÃ"Í¬²½"ÊôÐÔ
//@EXECUTIONFLOW:13¡¢µ÷ÓÃCSUDIPLAYERSetStream £¬ÉèÖÃºÃTSÂëÁ÷ÐÅÏ¢£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:14¡¢µ÷ÓÃCSUDIINJECTERStart£¬¿ªÊ¼Êý¾Ý×¢Èë£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:15¡¢´´½¨Ò»¸öÏß³Ì,Ñ­»·¶ÁÈ¡TSÂëÁ÷ÎÄ¼þ£¬²¢½øÐÐTS×¢Èë²Ù×÷
//@EXECUTIONFLOW:16¡¢µ÷ÓÃCSUDIPLAYERStart £¬¿ªÊ¼²¥·ÅÊý¾Ý£¬ÆÚÍûÄÜ¹»Õý³£²¥·ÅÒôÊÓÆµ
//@EXECUTIONFLOW:17¡¢ÌáÊ¾ÓÃ»§È·ÈÏ£¬ÒôÊÓÆµ½ÚÄ¿ÄÚÈÝÄÜ²»ÄÜ²¥·Å³öÀ´
//@EXECUTIONFLOW:18¡¢Í¨Öª×¢ÈëÈÎÎñÍ£Ö¹×¢ÈëÊý¾Ý£¬µÈ´ý×¢ÈëÈÎÎñÕý³£½áÊø·µ»Ø
//@EXECUTIONFLOW:19      µ÷ÓÃCSUDIINJECTERClearÇå³ýInecterÒÑ¾­×¢Èëµ½Ä¿±ê»º´æÇøÖÐµÄÉÐÃ»ÓÐ²¥·ÅÍêµÄÈ«²¿Êý¾Ý
//@EXECUTIONFLOW:20¡¢µ÷ÓÃCSUDIPLAYERStop £¬Í£Ö¹²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:21¡¢µ÷ÓÃCSUDIINJECTERStop £¬Í£Ö¹×¢ÈëÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:22¡¢µ÷ÓÃCSUDIPLAYERClose £¬¹Ø±Õ²¥·ÅÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:23¡¢µ÷ÓÃCSUDIINJECTERClose £¬¹Ø±Õ×¢ÈëÆ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:24¡¢µ÷ÓÃCSUDIOSThreadDestroy £¬É¾³ý×¢ÈëÈÎÎñ£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:25¡¢ÖØ¸´²½Öè1-²½Öè24
//@EXECUTIONFLOW:26¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_0022(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hInjectPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hInject= CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	CSUDIPESSYNCMode_S 	stSyncMode;
	CSUDIINJECTEROpenParam_S  stOpenParams;
	CSUDIINJECTERChnl_S stInjecterChnl;
	CSUDIPlayerChnl_S stPlayerChnl;
	CSUDIStreamInfo_S  stStreamInfo[3];  //²»»á³¬¹ý10¸ö
	int nStreamCnt = 0;
	CSUDIPlaybackParam_S stPlaybackParam;
	int i = 0;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;

	memset(&stInjecterChnl,-1,sizeof(stInjecterChnl));
	memset(&stPlayerChnl,-1,sizeof(stPlayerChnl));
	memset(stStreamInfo,0,sizeof(stStreamInfo));
	memset(&stOpenParams,0,sizeof(stOpenParams));

	stSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	stSyncMode.m_hSyncHandle = CSUDI_NULL;

	CSTCPrint("±¾ÓÃÀý²âÊÔ±¾µØÎÄ¼þ²¥·ÅÓëÖ±²¥µÄÇÐ»»\n");

	nStreamCnt = getTSInjectInfo(&stInjecterChnl,&stPlayerChnl,&stOpenParams,stStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(-1 != nStreamCnt, "»ñÈ¡×¢ÈëTS Á÷ÐÅÏ¢Ê§°Ü")

	for(i = 0; i < REPEAT_TIMES; i++)
	{
		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		ePlayerType = EM_UDIPLAYER_FILE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;

		strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MKV_HD].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTCPrint("µÚ %d ´Î²âÊÔ \"%s\" µÄÎÄ¼þ²¥·Å\n", i+1, stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback, CSUDI_NULL), "²½Öè3Ìí¼Ó»Øµ÷Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "²½Öè4Ê§°Ü");

	    	CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»Õý³£²¥·Å£¿\n");
	    	CSTCPrint("Is A/V output normal ?\n");

	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè5Ê§°Ü£º²¥·Å²»Õý³£");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "²½Öè6Ê§°Ü");
    		CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË\n");
  		CSTCPrint("Does the player stop ?\n");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, CSUDI_NULL), "²½Öè7É¾³ý»Øµ÷Ê§°Ü");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè8Ê§°Ü£º²¥·Å²»Õý³£");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "²½Öè9Ê§°Ü");

  		hPlayer=CSUDI_NULL;

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("µÚ %d ´Î²âÊÔ \"%s\" µÄ×¢Èë²¥·Å\n", i+1, g_TS_StreamInfo[LivePlayer].m_pcName);

		ePlayerType = EM_UDIPLAYER_INJECT;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&stInjecterChnl,&stOpenParams,&hInject), "²½Öè10Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,ePlayerType,&hInjectPlayer), "²½Öè11Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hInject,EM_UDIINJECTER_PES_SYNC,&stSyncMode), "²½Öè12Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hInjectPlayer, stStreamInfo, nStreamCnt, NULL), "²½Öè13ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hInject), "²½Öè14Ê§°Ü");

		g_TS_StreamInfo[LivePlayer].m_hInjecter = hInject;
		g_TS_StreamInfo[LivePlayer].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TS_StreamInfo[LivePlayer], &hInjectThread), "²½Öè15Ê§°Ü");

		while(g_TS_StreamInfo[LivePlayer].m_bRun == CSUDI_FALSE)
		{
			CSUDIOSThreadSleep(500);
		}

		 CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TS_StreamInfo[LivePlayer].m_bOpenFileSuccess, "ÎÄ¼þ´ò¿ªÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hInjectPlayer), "²½Öè16Ê§°Ü");

	    	CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜ¹»Õý³£²¥·Å£¿\n");
	    	CSTCPrint("Is A/V output normal ?\n");

	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè17Ê§°Ü£º²¥·Å²»Õý³£");

	    	g_TS_StreamInfo[LivePlayer].m_bRun = CSUDI_FALSE;

	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"²½Öè18Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hInject),"²½Öè19Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hInjectPlayer), "²½Öè20Ê§°Ü");
    		CSTCPrint("ÒôÊÓÆµÊÇ·ñÍ£Ö¹²¥·ÅÁË\n");
  		CSTCPrint("Does the player stop ?\n");


		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInject), "²½Öè21Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hInjectPlayer), "²½Öè22Ê§°Ü");

		hInjectPlayer = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hInject), "²½Öè23Ê§°Ü");

		hInject = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "²½Öè24Ê§°Ü");

		hInjectThread = CSUDI_NULL;
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "²½Öè26Ê§°Ü");
		hPlayer=CSUDI_NULL;
	}

	if (hInjectPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hInjectPlayer), "²½Öè26Ê§°Ü");
		hInjectPlayer=CSUDI_NULL;
	}

	if (hInject != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hInject), "²½Öè26Ê§°Ü");
		hInject = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "²½Öè26Ê§°Ü");
		hInjectThread = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}


static CSUDI_BOOL FILE_TEST_IT_PlayToEnd_Base(int formatType)
{
	CSUDIPlayerFileInfo_S stFileInfo;
	unsigned char ucPlayToend = 0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIOSTimeVal_S sStartTime;
	CSUDIOSTimeVal_S sEndTime;
	int nSpanTime = 0;
	int nPosInSec = 0;
	CSUDI_BOOL bPlaying = CSUDI_FALSE;
	CSUDI_BOOL bAddCallback = CSUDI_FALSE;

	memset(&stPlaybackParam, 0, sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	strncpy(stPlaybackParam.m_szFileName, stFilePath[formatType].filename, CSUDI_MAX_FILE_NAME_LEN);

	CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å(²¥·Åµ½Ä©Î²)\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName)), "²»Ö§³ÖµÄ¸ñÊ½");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "´´½¨²¥·Å¾ä±úÊ§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback, (void *)&ucPlayToend), "×¢²á»Øµ÷Ê§°Ü");

	bAddCallback = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²¥·ÅÎÄ¼þÊ§°Ü");
	
	bPlaying = CSUDI_TRUE;

	// »ñÈ¡¿ªÊ¼²¥·ÅµÄÊ±¼ä
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "»ñÈ¡¿ªÊ¼Ê±¼äÊ§°Ü");
	
	CSUDIOSThreadSleep(SLEEP_TIME);

	// Çå³ý²¥·Å×´Ì¬±ê¼Ç
	ucPlayToend = 0;

	CSTCPrint("ÒôÊÓÆµÊÇ·ñÄÜÕý³£²¥·Å£¿\n");
	CSTCPrint("Is A/V output normal? \n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "ÒôÊÓÆµ²¥·ÅÊ§°Ü");

	CSTCPrint("ÕýÔÚ²¥·Å...\n");
	
	// µÈ´ý²¥·ÅÍê³É
	while (PLAY_TO_END != ucPlayToend)
	{
		CSUDIOSThreadSleep(50);
	}

	bPlaying = CSUDI_FALSE;
	
	CSTCPrint("²¥·ÅÍê±Ï\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sEndTime), "»ñÈ¡½áÊøÊ±¼äÊ§°Ü");

	nSpanTime = sEndTime.m_nSecond - sStartTime.m_nSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetDuration(hPlayer, &nPosInSec), "»ñÈ¡½ÚÄ¿×Ü³¤¶ÈÊ§°Ü");

	// Îó²î2sÒÔÄÚ
	CSTK_ASSERT_TRUE_FATAL(((nPosInSec>nSpanTime) ? (2>=(nPosInSec-nSpanTime)) : (2>=(nSpanTime-nPosInSec))), "²¥·ÅÊ±³¤Òì³£");

	CSTK_FATAL_POINT;

	if (bPlaying)
	{
		bPlaying = CSUDI_FALSE;
		
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Í£Ö¹²¥·ÅÊ§°Ü");
	}

	if (bAddCallback)
	{
		bAddCallback = CSUDI_FALSE;
		
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, (void *)&ucPlayToend), "É¾³ý»Øµ÷Ê§°Ü");	
	}

	if (CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "»Ö¸´ÏÖ³¡");

		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÄÜÍêÕûÕý³£²¥·ÅMP3ÒôÆµ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT: void
//@EXPECTATION: ÆÚÍû²¥·ÅÕý³£
//@REMARK:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µÈ´ý²¥·ÅÍê±Ï
//@EXECUTIONFLOW:8¡¢ÅÐ¶Ï²¥·Å×ÜÊ±³¤ÓëÊµ¼Ê²¥·ÅÊ±¼äÊÇ·ñÏàµÈ
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ýÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_PlayToEnd_0001(void)
{
	FILE_TEST_IT_PlayToEnd_Base(EM_UDI_FILENAME_MP3);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÊÇ·ñÄÜÍêÕûÕý³£²¥·Å´óÈÝÁ¿MKVÊÓÆµ
//@PRECONDITION:PLAYERÄ£¿é£¬ÒôÊÓÆµ½âÂëÆ÷±»Õý³£³õÊ¼»¯
//@INPUT: void
//@EXPECTATION: ÆÚÍû²¥·ÅÕý³£
//@REMARK:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYERProbe£¬ÆÚ´ý·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERAddFilePlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:6¡¢ÌáÊ¾²âÊÔÈËÔ±¹Û²ì£¬ÊÇ·ñÓÐÒôÊÓÆµ²¥·Å³öÀ´
//@EXECUTIONFLOW:7¡¢µÈ´ý²¥·ÅÍê±Ï
//@EXECUTIONFLOW:8¡¢ÅÐ¶Ï²¥·Å×ÜÊ±³¤ÓëÊµ¼Ê²¥·ÅÊ±¼äÊÇ·ñÏàµÈ
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERDelFilePlayerCallback£¬É¾³ýÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_IT_PlayToEnd_0002(void)
{
	FILE_TEST_IT_PlayToEnd_Base(EM_UDI_FILENAME_MKV_HD_BSIZE);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER CALLBACK
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þ·Ç·¨ÎÄ¼þÂ·¾¶»Øµ÷µÄ·µ»ØÖµ
//@PRECONDITION:PLAYERÄ£¿éÕý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERAddPlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ£¬FileNameÎª·Ç·¨Â·¾¶"./testdata/error.ts"
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:5¡¢µÈ´ý3Ãë£¬ÆÚÍûÓÐEM_UDIFILEPLAYER_ERR_FORMATµÄ»Øµ÷ÊÂ¼þµ½À´
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERDelPlayerCallback£¬É¾³ýÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_CALLBACK_IT_0001(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;
	strncpy(stPlaybackParam.m_szFileName, "./testdata/error.ts", CSUDI_MAX_FILE_NAME_LEN);

	s_ePlayerEvent = EM_UDIFILEPLAYER_MAXEVENTTYPE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "²½Öè2Ìí¼Ó»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè3ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè4Ê§°Ü");

	CSTCPrint("ÇëµÈ´ý2Ãë\n");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(EM_UDIFILEPLAYER_ERR_FORMAT == s_ePlayerEvent,"²½Öè5Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "²½Öè6Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "²½Öè7É¾³ý»Øµ÷Ê§°Ü");

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "²½Öè8Ê§°Ü");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER CALLBACK
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þ²¥·Å¶¨Î»´íÎó»Øµ÷µÄ·µ»ØÖµ
//@PRECONDITION:PLAYERÄ£¿éÕý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERAddPlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ£¬FileNameÎª·Ç·¨Â·¾¶"./testdata/error.ts"
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERSeek  ÉèÖÃ´íÎóÎ»ÖÃ£¬ÎªÎÄ¼þ¿ªÍ·µÄ-1
//@EXECUTIONFLOW:6¡¢µÈ´ý3Ãë£¬ÆÚÍûÓÐEM_UDIFILEPLAYER_ERR_SEEKµÄ»Øµ÷ÊÂ¼þµ½À´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelPlayerCallback£¬É¾³ýÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢ µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_CALLBACK_IT_0002(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;
	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_TS_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	s_ePlayerEvent = EM_UDIFILEPLAYER_MAXEVENTTYPE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "²½Öè2Ìí¼Ó»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè3ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè4Ê§°Ü");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERSeek(hPlayer, -1, EM_UDIPLAYER_POSITION_FROM_HEAD), "²½Öè5Ê§°Ü");

	CSTCPrint("ÇëµÈ´ý2Ãë\n");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(EM_UDIFILEPLAYER_ERR_SEEK == s_ePlayerEvent,"²½Öè6Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "²½Öè7Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "²½Öè8É¾³ý»Øµ÷Ê§°Ü");

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "²½Öè9Ê§°Ü");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER CALLBACK
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÉèÖÃ´íÎóËÙÂÊ»Øµ÷µÄ·µ»ØÖµ
//@PRECONDITION:PLAYERÄ£¿éÕý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERAddPlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERSetSpeed  ÉèÖÃ´íÎóËÙÂÊ£¬·Ö±ðÎª(EM_UDIPLAYER_SPEED_NORMAL-1)ºÍ(EM_UDIPLAYER_SPEED_MAX)
//@EXECUTIONFLOW:6¡¢µÈ´ý3Ãë£¬ÆÚÍûÓÐEM_UDIFILEPLAYER_ERR_SETSPEEDµÄ»Øµ÷ÊÂ¼þµ½À´
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERDelPlayerCallback£¬É¾³ýÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_CALLBACK_IT_0003(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;
	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_TS_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	s_ePlayerEvent = EM_UDIFILEPLAYER_MAXEVENTTYPE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "²½Öè2Ìí¼Ó»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè3ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè4Ê§°Ü");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_NORMAL-1), "²½Öè5Ê§°Ü");

	CSTCPrint("ÇëµÈ´ý2Ãë\n");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(EM_UDIFILEPLAYER_ERR_SETSPEED == s_ePlayerEvent,"²½Öè6Ê§°Ü");

	s_ePlayerEvent = EM_UDIFILEPLAYER_MAXEVENTTYPE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_MAX), "²½Öè5Ê§°Ü");

	CSTCPrint("ÇëµÈ´ý2Ãë\n");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(EM_UDIFILEPLAYER_ERR_SETSPEED == s_ePlayerEvent,"²½Öè6Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "²½Öè7Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "²½Öè8É¾³ý»Øµ÷Ê§°Ü");

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "²½Öè9Ê§°Ü");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER CALLBACK
//@DESCRIPTION:²âÊÔ±¾µØÎÄ¼þÕý³£²¥·Å\Ñ¡Ê±\ÔÝÍ£\»Ö¸´\Í£Ö¹Á÷³ÌµÄ»Øµ÷ÊÂ¼þ
//@PRECONDITION:PLAYERÄ£¿éÕý³£³õÊ¼»¯
//@INPUT:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERAddPlayerCallback£¬Ìí¼ÓÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStart  ´ò¿ª²¥·ÅÆ÷
//@EXECUTIONFLOW:5¡¢ÆÚÍû3ÃëÄÚÓÐEM_UDIFILEPLAYER_STATE_STARTµÄ»Øµ÷ÊÂ¼þµ½À´
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERSeek ÉèÖÃ²¥·ÅÆ÷Î»ÖÃ
//@EXECUTIONFLOW:7¡¢ÆÚÍû3ÃëÄÚÓÐEM_UDIFILEPLAYER_SEEK_FINISHµÄ»Øµ÷ÊÂ¼þµ½À´
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERPause ÔÝÍ£²¥·ÅÆ÷
//@EXECUTIONFLOW:9¡¢ÆÚÍû3ÃëÄÚÓÐEM_UDIFILEPLAYER_STATE_PAUSEµÄ»Øµ÷ÊÂ¼þµ½À´
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSUDIPLAYERResume »Ö¸´²¥·ÅÆ÷
//@EXECUTIONFLOW:11¡¢µ÷ÓÃCSUDIPLAYERStop  Í£Ö¹²¥·ÅÆ÷
//@EXECUTIONFLOW:12¡¢ÆÚÍû3ÃëÄÚÓÐEM_UDIFILEPLAYER_STATE_STOPµÄ»Øµ÷ÊÂ¼þµ½À´
//@EXECUTIONFLOW:13¡¢µ÷ÓÃCSUDIPLAYERDelPlayerCallback£¬É¾³ýÎÄ¼þ¿ªÊ¼²¥·ÅµÄ»Øµ÷ÊÂ¼þ
//@EXECUTIONFLOW:14¡¢µ÷ÓÃCSUDIPLAYERCloseÉ¾³ý²¥·ÅÆ÷
CSUDI_BOOL CSTC_FILE_TEST_CALLBACK_IT_0004(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_FILE;

	int i;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;
	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_TS_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

	s_ePlayerEvent = EM_UDIFILEPLAYER_MAXEVENTTYPE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "²½Öè2Ìí¼Ó»Øµ÷Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè3ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè4Ê§°Ü");

	for(i=0;i<MAX_SLEEP_NUM;i++)
	{
		CSUDIOSThreadSleep(100);
		if(EM_UDIFILEPLAYER_STATE_START == s_ePlayerEvent)
			break;
	}

	if(i==MAX_SLEEP_NUM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²½Öè5Ê§°Ü");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD), "²½Öè6Ê§°Ü");

	for(i=0;i<MAX_SLEEP_NUM;i++)
	{
		CSUDIOSThreadSleep(100);
		if(EM_UDIFILEPLAYER_SEEK_FINISH == s_ePlayerEvent)
			break;
	}

	if(i==MAX_SLEEP_NUM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²½Öè7Ê§°Ü");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer), "²½Öè8Ê§°Ü");

	for(i=0;i<MAX_SLEEP_NUM;i++)
	{
		CSUDIOSThreadSleep(100);
		if(EM_UDIFILEPLAYER_STATE_PAUSE == s_ePlayerEvent)
			break;
	}

	if(i==MAX_SLEEP_NUM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²½Öè9Ê§°Ü");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer), "²½Öè10Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "²½Öè11Ê§°Ü");

	for(i=0;i<MAX_SLEEP_NUM;i++)
	{
		CSUDIOSThreadSleep(100);
		if(EM_UDIFILEPLAYER_STATE_STOP == s_ePlayerEvent)
			break;
	}

	if(i==MAX_SLEEP_NUM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²½Öè12Ê§°Ü");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "²½Öè13É¾³ý»Øµ÷Ê§°Ü");

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "²½Öè14Ê§°Ü");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:²âÊÔÊäÈë²ÎÊý·Ç·¨µÄÇé¿öÏÂ£¬ÊÇ·ñ·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
//@INPUT:1¡¢²ÎÊý×éºÏ²»·ûºÏÉè¼ÆÒªÇó
//@EXPECTATION:·µ»Ø·ÇCSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬µÚÒ»¸ö²ÎÊýÉèÖÃÎª¿Õ£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬µÚ¶þ¸ö²ÎÊýÉèÖÃÎª¿Õ£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬Á½¸ö²ÎÊý¶¼ÉèÖÃÎª¿Õ£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERClose¹Ø±Õ²¥·Å¾ä±ú
//@EXECUTIONFLOW:5¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_GetFileInfo_0001(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlayerFileInfo_S  stFileInfo;
	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	CSUDIPlaybackParam_S stPlaybackParam;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MA].filename, CSUDI_MAX_FILE_NAME_LEN);

	udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName);

	if(udiRe == CSUDI_SUCCESS)
	{
		CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1:´´½¨²¥·Å¾ä±úÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1:´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2:ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetFileInfo(CSUDI_NULL, &stFileInfo), "²½Öè3:´«Èë·Ç·¨¾ä±ú£¬Ó¦¸Ã·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetFileInfo(hPlayer, CSUDI_NULL), "²½Öè4:´«Èë·Ç·¨²ÎÊý£¬Ó¦¸Ã·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetFileInfo(CSUDI_NULL, CSUDI_NULL), "²½Öè5:´«Èë·Ç·¨²ÎÊý£¬Ó¦¸Ã·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²»ÄÜ¹»Ö±½ÓÖ§³Ö±¾µØÃ½Ìå²¥·Å");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "²½Öè6:¹Ø±Õ²¥·Å¾ä±úÊ§°Ü");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:²âÊÔ»ñÈ¡ tsÁ÷ÎÄ¼þµÄÐÅÏ¢ÊÇ·ñÕýÈ·Çé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
///@INPUT:1¡¢ÓÐÐ§µÄhPlayer
//@INPUT:2¡¢ÓÐÐ§µÄstFileStreamId
//@EXPECTATION:²»Ö§³Ö  CSUDIPLAYERGetFileInfo»ñÈ¡ÆÕÍ¨ÎÄ¼þÐÅÏ¢
//@REMARK:ÂëÁ÷±ØÐëÎªallts.ts
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬»ñÈ¡ÎÄ¼þµÄÐÅÏ¢
//@EXECUTIONFLOW:4¡¢±È½ÏÎÄ¼þÐÅÏ¢ºÍ»ñÈ¡µ½µÄÎÄ¼þÐÅÏ¢ÊÇ·ñÏàµÈ£¬Èç¹ûÈ«²¿ÏàµÈÔòÖ¤Ã÷ÊÇÕýÈ·µÄ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERClose¹Ø±Õ²¥·Å¾ä±ú
//@EXECUTIONFLOW:6¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_GetFileInfo_0002(void)
{
	CSUDIPlayerFileInfo_S  stFileInfo;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_Error_Code   udiRe = CSUDI_SUCCESS;
  	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlaybackParam_S stPlaybackParam;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;


	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MA].filename, CSUDI_MAX_FILE_NAME_LEN);

	CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

	udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName);

	if(udiRe == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "´´½¨²¥·Å¾ä±úÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "»ñÈ¡ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_eStreamType == EM_UDIFILEPLAYER_STREAM_ES, "ÎÄ¼þÀàÐÍ´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_n64FileSize== stFileInfo.m_n64FileSize, "ÎÄ¼þ´óÐ¡²»Ò»ÖÂ");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_n64StartTime== stFileInfo.m_n64StartTime, "ÎÄ¼þ²¥·Å×ÜÊ±³¤´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_n64Duration== stFileInfo.m_n64Duration, "ÎÄ¼þ×ÜÊ±³¤´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32Bps == stFileInfo.m_u32Bps, "ÎÄ¼þÂëÂÊ´íÎó");
		CSTK_ASSERT_TRUE_FATAL(!strcmp(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_acFileName ,stFileInfo.m_acFileName) , "ÎÄ¼þÃû´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32ProgramNum == stFileInfo.m_u32ProgramNum, "½ÚÄ¿¸öÊý´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32AudStreamNum == stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum, "ÒôÆµ¸öÊý´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32SubTitleNum == stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum, "×ÖÄ»¸öÊý´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32VideoFormat == stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u32Format, "ÊÓÆµ¸ñÊ½´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u16Width == stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Width, "ÊÓÆµ¿í¶È´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u16Height == stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Height, "ÊÓÆµ¸ß¶È´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u16Channels == stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u16Channels, "ÒôÆµÉùµÀ´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32SampleRate == stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32SampleRate, "ÒôÆµÂëÂÊ´íÎó");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32AudioFormat == stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32Format, "ÒôÆµ¸ñÊ½´íÎó");

	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²»ÄÜ¹»Ö±½ÓÖ§³Ö±¾µØÃ½Ìå²¥·Å");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "¹Ø±Õ²¥·Å¾ä±úÊ§°Ü");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:²âÊÔ»ñÈ¡esÁ÷ÎÄ¼þµÄÐÅÏ¢ÊÇ·ñÕýÈ·Çé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
///@INPUT:1¡¢ÓÐÐ§µÄhPlayer
//@INPUT:2¡¢ÓÐÐ§µÄstFileStreamId
//@EXPECTATION:²»Ö§³Ö  CSUDIPLAYERGetFileInfo»ñÈ¡ÆÕÍ¨ÎÄ¼þÐÅÏ¢
//@REMARK:Ã»ÓÐÕÒµ½Ïà¹ØµÄÂëÁ÷£¬
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬»ñÈ¡ÎÄ¼þµÄÐÅÏ¢
//@EXECUTIONFLOW:4¡¢±È½ÏÎÄ¼þÐÅÏ¢ºÍ»ñÈ¡µ½µÄÎÄ¼þÐÅÏ¢ÊÇ·ñÏàµÈ£¬Èç¹ûÈ«²¿ÏàµÈÔòÖ¤Ã÷ÊÇÕýÈ·µÄ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERClose¹Ø±Õ²¥·Å¾ä±ú
//@EXECUTIONFLOW:6¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_GetFileInfo_0003(void)
{
	CSUDIPlayerFileInfo_S  stFileInfo;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlaybackParam_S stPlaybackParam;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;


	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MV].filename, CSUDI_MAX_FILE_NAME_LEN);

	CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "´´½¨²¥·Å¾ä±úÊ§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "»ñÈ¡ÐÅÏ¢Ê§°Ü");

	CSTCPrint("---------------m_eStreamType == %d\r\n", stFileInfo.m_eStreamType);
	CSTCPrint("---------------m_acFileName == %s\r\n", stFileInfo.m_acFileName);
	CSTCPrint("---------------m_n64FileSize == %lld\r\n", stFileInfo.m_n64FileSize);
	CSTCPrint("---------------m_n64StartTime == %lld\r\n", stFileInfo.m_n64StartTime);
	CSTCPrint("---------------m_n64Duration == %lld\r\n", stFileInfo.m_n64Duration);
	CSTCPrint("---------------m_u32Bps == %d\r\n", (int)stFileInfo.m_u32Bps);
	CSTCPrint("---------------m_u32ProgramNum == %d\r\n", (int)stFileInfo.m_u32ProgramNum);

	CSTCPrint("---------------m_u32AudStreamNum == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum);
	CSTCPrint("---------------m_u32SubTitleNum == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum);
	CSTCPrint("---------------m_u16Height == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Height);
	CSTCPrint("---------------m_u16Width == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Width);
	CSTCPrint("---------------m_u32Format == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u32Format);

	CSTCPrint("---------------m_acAudLang == %s\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_acAudLang);
	CSTCPrint("---------------m_u16Channels == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u16Channels);
	CSTCPrint("---------------m_u32Format == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32Format);
	CSTCPrint("---------------m_u32SampleRate == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32SampleRate);

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "¹Ø±Õ²¥·Å¾ä±úÊ§°Ü");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:²âÊÔ»ñÈ¡ÆÕÍ¨ÎÄ¼þµÄÐÅÏ¢ÊÇ·ñÕýÈ·Çé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
///@INPUT:1¡¢ÓÐÐ§µÄhPlayer
//@INPUT:2¡¢ÓÐÐ§µÄstFileStreamId
//@EXPECTATION:²»Ö§³Ö  CSUDIPLAYERGetFileInfo»ñÈ¡ÆÕÍ¨ÎÄ¼þÐÅÏ¢
//@REMARK:Ã»ÓÐÕÒµ½Ïà¹ØµÄÂëÁ÷£¬
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬»ñÈ¡ÎÄ¼þµÄÐÅÏ¢
//@EXECUTIONFLOW:4¡¢±È½ÏÎÄ¼þÐÅÏ¢ºÍ»ñÈ¡µ½µÄÎÄ¼þÐÅÏ¢ÊÇ·ñÏàµÈ£¬Èç¹ûÈ«²¿ÏàµÈÔòÖ¤Ã÷ÊÇÕýÈ·µÄ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERClose¹Ø±Õ²¥·Å¾ä±ú
//@EXECUTIONFLOW:6¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_GetFileInfo_0004(void)
{
	CSUDIPlayerFileInfo_S  stFileInfo;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlaybackParam_S stPlaybackParam;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP3].filename, CSUDI_MAX_FILE_NAME_LEN);

	CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "´´½¨²¥·Å¾ä±úÊ§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "»ñÈ¡ÐÅÏ¢Ê§°Ü");

	CSTCPrint("---------------m_eStreamType == %d\r\n", stFileInfo.m_eStreamType);
	CSTCPrint("---------------m_acFileName == %s\r\n", stFileInfo.m_acFileName);
	CSTCPrint("---------------m_n64FileSize == %lld\r\n", stFileInfo.m_n64FileSize);
	CSTCPrint("---------------m_n64StartTime == %lld\r\n", stFileInfo.m_n64StartTime);
	CSTCPrint("---------------m_n64Duration == %lld\r\n", stFileInfo.m_n64Duration);
	CSTCPrint("---------------m_u32Bps == %d\r\n", (int)stFileInfo.m_u32Bps);
	CSTCPrint("---------------m_u32ProgramNum == %d\r\n", (int)stFileInfo.m_u32ProgramNum);

	CSTCPrint("---------------m_u32AudStreamNum == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum);
	CSTCPrint("---------------m_u32SubTitleNum == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum);
	CSTCPrint("---------------m_u16Height == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Height);
	CSTCPrint("---------------m_u16Width == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Width);
	CSTCPrint("---------------m_u32Format == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u32Format);

	CSTCPrint("---------------m_acAudLang == %s\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_acAudLang);
	CSTCPrint("---------------m_u16Channels == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u16Channels);
	CSTCPrint("---------------m_u32Format == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32Format);
	CSTCPrint("---------------m_u32SampleRate == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32SampleRate);

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "¹Ø±Õ²¥·Å¾ä±úÊ§°Ü");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:²âÊÔ»ñÈ¡ÍøÂçÎÄ¼þµÄÐÅÏ¢ÊÇ·ñÕýÈ·Çé¿ö
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
///@INPUT:1¡¢ÓÐÐ§µÄhPlayer
//@INPUT:2¡¢ÓÐÐ§µÄstFileStreamId
//@EXPECTATION:²»Ö§³Ö  CSUDIPLAYERGetFileInfo»ñÈ¡ÍøÂçÎÄ¼þÐÅÏ¢
//@REMARK:Ã»ÓÐÕÒµ½Ïà¹ØµÄÂëÁ÷£¬
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬»ñÈ¡ÎÄ¼þµÄÐÅÏ¢
//@EXECUTIONFLOW:4¡¢±È½ÏÎÄ¼þÐÅÏ¢ºÍ»ñÈ¡µ½µÄÎÄ¼þÐÅÏ¢ÊÇ·ñÏàµÈ£¬Èç¹ûÈ«²¿ÏàµÈÔòÖ¤Ã÷ÊÇÕýÈ·µÄ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERClose¹Ø±Õ²¥·Å¾ä±ú
//@EXECUTIONFLOW:6¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_GetFileInfo_0005(void)
{
	CSUDIPlayerFileInfo_S  stFileInfo;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlaybackParam_S stPlaybackParam;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MA].filename, CSUDI_MAX_FILE_NAME_LEN);

	CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "´´½¨²¥·Å¾ä±úÊ§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "ÉèÖÃÁ÷ÐÅÏ¢Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "»ñÈ¡ÐÅÏ¢Ê§°Ü");

	CSTCPrint("---------------m_eStreamType == %d\r\n", stFileInfo.m_eStreamType);
	CSTCPrint("---------------m_acFileName == %s\r\n", stFileInfo.m_acFileName);
	CSTCPrint("---------------m_n64FileSize == %lld\r\n", stFileInfo.m_n64FileSize);
	CSTCPrint("---------------m_n64StartTime == %lld\r\n", stFileInfo.m_n64StartTime);
	CSTCPrint("---------------m_n64Duration == %lld\r\n", stFileInfo.m_n64Duration);
	CSTCPrint("---------------m_u32Bps == %d\r\n", (int)stFileInfo.m_u32Bps);
	CSTCPrint("---------------m_u32ProgramNum == %d\r\n", (int)stFileInfo.m_u32ProgramNum);

	CSTCPrint("---------------m_u32AudStreamNum == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum);
	CSTCPrint("---------------m_u32SubTitleNum == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum);
	CSTCPrint("---------------m_u16Height == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Height);
	CSTCPrint("---------------m_u16Width == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Width);
	CSTCPrint("---------------m_u32Format == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u32Format);

	CSTCPrint("---------------m_acAudLang == %s\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_acAudLang);
	CSTCPrint("---------------m_u16Channels == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u16Channels);
	CSTCPrint("---------------m_u32Format == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32Format);
	CSTCPrint("---------------m_u32SampleRate == %d\r\n", (int)stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32SampleRate);

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "¹Ø±Õ²¥·Å¾ä±úÊ§°Ü");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetFilePlayStream
//@DESCRIPTION:²âÊÔ²ÎÊý·Ç·¨µÄÇé¿öÏÂ£¬ÊÇ·ñ·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
//@INPUT:1¡¢²ÎÊý×éºÏ²»·ûºÏÉè¼ÆÒªÇó
//@EXPECTATION:·µ»Ø·ÇCSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬»ñÈ¡²¥ÎÄ¼þµÄÐÅÏ¢
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSUDIPLAYERStart£¬¿ªÊ¼²¥·ÅÎÄ¼þ
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERSetFilePlayStream£¬µÚÒ»¸ö²ÎÊýÉèÖÃÎª¿Õ£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERSetFilePlayStream£¬µÚ¶þ¸ö²ÎÊýÉèÖÃÎª¿Õ£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIPLAYERSetFilePlayStream£¬Á½¸ö²ÎÊý¶¼ÉèÖÃÎª¿Õ£¬ÆÚÍû·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·Å
//@EXECUTIONFLOW:9¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_SetFilePlayStream_0001(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlayerFileStreamId_S  stFileStreamId;
	CSUDI_Error_Code  udiRe = CSUDI_SUCCESS;
	CSUDIPlaybackParam_S stPlaybackParam;
	CSUDIPlayerFileInfo_S  stFileInfo;

	memset(&stFileStreamId, 0, sizeof(CSUDIPlayerFileStreamId_S));
	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MA].filename, CSUDI_MAX_FILE_NAME_LEN);

	udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName);

	if(udiRe == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1¡¢´´½¨²¥·Å¾ä±úÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1¡¢´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2¡¢ÉèÖÃÓÃ»§½ÚÄ¿ÐÅÏ¢Ê§°Ü")

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "²½Öè3¡¢»ñÈ¡ÎÄ¼þÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè4¡¢²¥·ÅÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetFilePlayStream(CSUDI_NULL, &stFileStreamId), "²½Öè5¡¢´«Èë·Ç·¨²ÎÊý£¬Ó¦¸Ã·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetFilePlayStream(hPlayer, CSUDI_NULL), "²½Öè6¡¢´«Èë·Ç·¨²ÎÊý£¬Ó¦¸Ã·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetFilePlayStream(CSUDI_NULL, CSUDI_NULL), "²½Öè7¡¢´«Èë·Ç·¨²ÎÊý£¬Ó¦¸Ã·µ»ØCSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSUDIOSThreadSleep(2*1000);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"²½Öè8¡¢Í£Ö¹²¥·ÅÊ§°Ü");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²»ÄÜ¹»Ö±½ÓÖ§³Ö±¾µØÃ½Ìå²¥·Å");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "²½Öè9¡¢¹Ø±Õ²¥·Å¾ä±úÊ§°Ü");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetFilePlayStream
//@DESCRIPTION:²âÊÔ¶àÒô¹ìÇé¿öÏÂ£¬¸Ä±äÒôÆµµÄpidÖµÒôÆµÊÇ·ñ¸Ä±ä
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
//@INPUT:1¡¢ÓÐÐ§µÄhPlayer
//@INPUT:2¡¢ÓÐÐ§µÄstFileStreamId
//@EXPECTATION:²»Ö§³Ö  CSUDIPLAYERSetFilePlayStreamÉèÖÃ¶àÒôÆµ
//@REMARK:±ØÐëÓÐ¶àÒôÆµµÄÂëÁ÷
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬»ñÈ¡²¥ÎÄ¼þµÄÐÅÏ¢
//@EXECUTIONFLOW:4¡¢±È½ÏÒôÆµµÄ¸öÊýÊÇ·ñ³¬¹ý×î´ó¸öÊý
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERSetFilePlayStream£¬ÉèÖÃÎÄ¼þµÄ²¥·ÅÊôÐÔ
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERStart£¬¿ªÊ¼²¥·ÅÎÄ¼þ
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSTKWaitYes£¬µÈ´ý²âÊÔÊÇ·ñÓÐÒôÆµÇÐ»»
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·Å
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERClose£¬¹Ø±Õ²¥·Å¾ä±ú
//@EXECUTIONFLOW:10¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_SetFilePlayStream_0002(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlayerFileStreamId_S  stFileStreamId;
	CSUDIPlaybackParam_S   stPlaybackParam;
	CSUDIPlayerFileInfo_S  stFileInfo;
	CSUDI_Error_Code   udiRe = CSUDI_SUCCESS;

	memset(&stFileStreamId, 0, sizeof(CSUDIPlayerFileStreamId_S));

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MA].filename, CSUDI_MAX_FILE_NAME_LEN);

	udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName);

	if(udiRe == CSUDI_SUCCESS)
	{
		CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1Ê§°Ü¡¢´´½¨²¥·Å¾ä±úÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1Ê§°Ü¡¢´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2Ê§°Ü¡¢ÉèÖÃÓÃ»§½ÚÄ¿ÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "²½Öè3Ê§°Ü¡¢»ñÈ¡ÎÄ¼þÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum <= CSUDI_PLAYER_MAX_STREAM_NUM, "²½Öè4Ê§°Ü¡¢ÒôÆµµÄ¸öÊý³¬¹ýÁË×î´óÊýÎå¸ö");

		int i = 0;
		for(i=0; i<stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum; i++)
		{
			stFileStreamId.m_u32AStreamId = i;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetFilePlayStream(hPlayer, &stFileStreamId), "²½Öè5Ê§°Ü¡¢ÉèÖÃÎÄ¼þµÄ²¥·ÅÊôÐÔ");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè6Ê§°Ü¡¢¿ªÊ¼²¥·Å²»Õý³£");

			if(i != 0)
			{
				CSTCPrint("ÊÇ·ñºÍÉÏÒ»¸öÒôÆµ²»Í¬\n");
	        	CSTCPrint("Does the player AUDIO are different to the former ?\n");
			}
			else
			{
				CSTCPrint("ÒôÆµÊÇ·ñ²¥·ÅÕý³£?\n");
	        	CSTCPrint("Does the player AUDIO has play OK ?\n");
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè7Ê§°Ü£ºÊÇ·ñÓëÉÏÒ»¸öÒôÆµ²»Í¬");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"²½Öè8Ê§°Ü¡¢Í£Ö¹²¥·ÅÊ§°Ü");

			CSUDIOSThreadSleep(1000);

		}


	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²»ÄÜ¹»Ö±½ÓÖ§³Ö±¾µØÃ½Ìå²¥·Å");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "²½Öè9Ê§°Ü£º¹Ø±Õ²¥·ÅÊ§°Ü");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetFilePlayStream
//@DESCRIPTION:²âÊÔ¶àÊÓÆµÇé¿öÏÂ£¬¸Ä±äÊÓÆµµÄpidÖµÊÓÆµÊÇ·ñ¸Ä±ä
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
//@INPUT:1¡¢ÓÐÐ§µÄhPlayer
//@INPUT:2¡¢ÓÐÐ§µÄstFileStreamId
//@EXPECTATION:²»Ö§³Ö  CSUDIPLAYERSetFilePlayStreamÉèÖÃ¶àÊÓÆµ
//@REMARK:±ØÐëÓÐ¶àÊÓÆµµÄÂëÁ÷
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬»ñÈ¡²¥ÎÄ¼þµÄÐÅÏ¢
//@EXECUTIONFLOW:4¡¢±È½ÏÊÓÆµµÄ¸öÊýÊÇ·ñ³¬¹ý×î´ó¸öÊý
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERSetFilePlayStream£¬ÉèÖÃÎÄ¼þµÄ²¥·ÅÊôÐÔ
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERStart£¬¿ªÊ¼²¥·ÅÎÄ¼þ
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSTKWaitYes£¬µÈ´ý²âÊÔÊÇ·ñÓÐÊÓÆµÇÐ»»
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·Å
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERClose£¬¹Ø±Õ²¥·Å¾ä±ú
//@EXECUTIONFLOW:10¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_SetFilePlayStream_0003(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlayerFileStreamId_S  stFileStreamId;
	CSUDIPlaybackParam_S   stPlaybackParam;
	CSUDIPlayerFileInfo_S  stFileInfo;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	memset(&stFileStreamId, 0, sizeof(CSUDIPlayerFileStreamId_S));

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;

	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));

	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MV].filename, CSUDI_MAX_FILE_NAME_LEN);

	udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName);

	if(udiRe == CSUDI_SUCCESS)
	{
		CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1¡¢´´½¨²¥·Å¾ä±úÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1¡¢´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2¡¢ÉèÖÃÓÃ»§½ÚÄ¿ÐÅÏ¢Ê§°Ü")

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "²½Öè3¡¢»ñÈ¡ÎÄ¼þÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(stFileInfo.m_u32ProgramNum <= CSUDI_PLAYER_MAX_PROGRAM_NUM, "²½Öè4¡¢ÊÓÆµµÄ¸öÊý³¬¹ýÁË×î´óÊýÎå¸ö");

		int i = 0;
		for(i=0; i<stFileInfo.m_u32ProgramNum; i++)
		{
			stFileStreamId.m_u32ProgramId = i;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetFilePlayStream(hPlayer, &stFileStreamId), "²½Öè5¡¢ÉèÖÃÓ¦¸Ã·µ»Ø");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè6¡¢¿ªÊ¼²¥·Å²»Õý³£");

			if(i != 0)
			{
				CSTCPrint("ÊÓÆµÊÇ·ñÇÐ»» ¿ \n");
	        	CSTCPrint("Does the player VUDIO are different to the former ?\n");
			}
			else
			{
				CSTCPrint("ÊÓÆµÊÇ²¥·Å³É¹¦ ¿ \n");
	        	CSTCPrint("Does the player VUDIO has play OK ?\n");
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè7¡¢¿ªÊ¼²¥·Å²»Õý³£");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"²½Öè8¡¢Í£Ö¹²¥·ÅÊ§°Ü");

			CSUDIOSThreadSleep(2*1000);
		}
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²»ÄÜ¹»Ö±½ÓÖ§³Ö±¾µØÃ½Ìå²¥·Å");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "²½Öè9¡¢¹Ø±Õ²¥·ÅÊ§°Ü");
	}

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIPLAYERSetFilePlayStream
//@DESCRIPTION:²âÊÔ¶à×ÖÄ»Çé¿öÏÂ£¬¸Ä±ä×ÖÄ»µÄpidÖµÒôÆµÊÇ·ñ¸Ä±ä
//@PRECONDITION:PLAYERÄ£¿é±»Õý³£³õÊ¼»¯£¬file¸ñÊ½µÄÎÄ¼þ´æÔÚ
//@INPUT:1¡¢ÓÐÐ§µÄhPlayer
//@INPUT:2¡¢ÓÐÐ§µÄstFileStreamId
//@EXPECTATION:²»Ö§³Ö  CSUDIPLAYERSetFilePlayStreamÉèÖÃ×ÖÄ»
//@REMARK:±ØÐëÓÐ¶à×ÖÄ»µÄÂëÁ÷
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIPLAYEROpen£¬´ò¿ªÒ»¸ö±¾µØÎÄ¼þ²¥·ÅÆ÷£¬µÃµ½²¥·ÅÆ÷¾ä±ú
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIPLAYERSetStream£¬ÉèÖÃ²¥·ÅÆ÷ÏàÓ¦ÊôÐÔ
//@EXECUTIONFLOW:3¡¢µ÷ÓÃCSUDIPLAYERGetFileInfo£¬»ñÈ¡²¥ÎÄ¼þµÄÐÅÏ¢
//@EXECUTIONFLOW:4¡¢±È½Ï×ÖÄ»µÄ¸öÊýÊÇ·ñ³¬¹ý×î´ó¸öÊý£¬ÊÇ·ñÓÐ×ÖÄ»
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIPLAYERSetFilePlayStream£¬ÉèÖÃÎÄ¼þµÄ²¥·ÅÊôÐÔ
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSUDIPLAYERStart£¬¿ªÊ¼²¥·ÅÎÄ¼þ
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSTKWaitYes£¬µÈ´ý²âÊÔÊÇ·ñÓÐ×ÖÄ»ÇÐ»»
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSUDIPLAYERStop£¬Í£Ö¹²¥·Å
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIPLAYERClose£¬¹Ø±Õ²¥·Å¾ä±ú
//@EXECUTIONFLOW:10¡¢»Ö¸´ÏÖ³¡
CSUDI_BOOL CSTC_FILE_TEST_IT_SetFilePlayStream_0004(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;
	CSUDIPlayerFileStreamId_S  stFileStreamId;
	CSUDIPlaybackParam_S   stPlaybackParam;
	CSUDIPlayerFileInfo_S  stFileInfo;
	CSUDI_Error_Code  udiRe = CSUDI_SUCCESS;

	memset(&stFileStreamId, 0, sizeof(CSUDIPlayerFileStreamId_S));
	memset(&stFileInfo, 0, sizeof(CSUDIPlayerFileInfo_S));
	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
	stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	stPlaybackParam.m_nSecondPos = 0;
	strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MS].filename, CSUDI_MAX_FILE_NAME_LEN);

	udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName);

	if(udiRe == CSUDI_SUCCESS)
	{
		CSTCPrint("±¾ÓÃÀý²âÊÔ \"%s\" µÄ²¥·Å\n", stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "²½Öè1¡¢´´½¨²¥·Å¾ä±úÊ§°Ü");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "²½Öè1¡¢´´½¨µÄ²¥·Å¾ä±úÎª¿Õ");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "²½Öè2¡¢ÉèÖÃÓÃ»§½ÚÄ¿ÐÅÏ¢Ê§°Ü")

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "²½Öè3¡¢»ñÈ¡ÎÄ¼þÐÅÏ¢Ê§°Ü");

		CSTK_ASSERT_TRUE_FATAL(stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum <= CSUDI_PLAYER_MAX_LANG_NUM, "²½Öè4¡¢×ÖÄ»µÄ¸öÊý³¬¹ýÁË×î´óÊýÎå¸ö");

		CSTK_ASSERT_TRUE_FATAL(stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum > 0, "²½Öè4¡¢×ÖÄ»µÄ¸öÊýÎªÁã£¬Çë¼Ó×ÖÄ»ÎÄ¼þ");

		int i = 0;
		for(i=0; i<stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum; i++)
		{
			stFileStreamId.m_u32SubTitleId = i;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetFilePlayStream(hPlayer, &stFileStreamId), "²½Öè5¡¢ÉèÖÃÓ¦¸Ã·µ»Ø");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "²½Öè6¡¢¿ªÊ¼²¥·Å²»Õý³£");

			if(i != 0)
			{
				CSTCPrint("the language is %s\n", stFileInfo.m_astProgramInfo[0].m_astSubTitle[i].m_acSubTitleName);
				CSTCPrint(" ×ÖÄ»µÄ±êÌâÊÇ·ñºÍÇ°Ò»¸ö²»Í¬\n");
	        	CSTCPrint("Does the player subtitle has differnt to the former ?\n");
			}
			else
			{
				CSTCPrint(" ÊÇ·ñÏÔÊ¾×ÖÄ»±êÌâ\n");
				CSTCPrint("the language is %s\n", stFileInfo.m_astProgramInfo[0].m_astSubTitle[i].m_acSubTitleName);
	        	CSTCPrint("Does the player subtitle has normal output    ?\n");
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"²½Öè7¡¢×ÖÄ»²¥·ÅÊ§°Ü!");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"²½Öè8¡¢Í£Ö¹²¥·ÅÊ§°Ü");

			CSUDIOSThreadSleep(1000);
		}
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "²»ÄÜ¹»Ö±½ÓÖ§³Ö±¾µØÃ½Ìå²¥·Å");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "²½Öè9¡¢¹Ø±Õ²¥·ÅÊ§°Ü");
	}

	return CSUDI_TRUE;
}


