/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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
	CSUDIPlayerStreamType_E m_eStreamType; ///< �ļ�������
	CSUDI_INT64 m_n64FileSize; ///< �ļ���С����λ�ֽ�
	CSUDI_INT64 m_n64StartTime; ///< �ļ�������ʼʱ�䣬��λms
	CSUDI_INT64 m_n64Duration; ///< �ļ���ʱ������λms
	CSUDI_UINT32 m_u32Bps; ///< �ļ����ʣ�bits/s
	char m_acFileName[CSUDI_PLAYER_MAX_FILE_NAME_LEN]; ///< �ļ�����
	CSUDI_UINT32 m_u32ProgramNum; ///< ʵ�ʽ�Ŀ����
	CSUDI_UINT32 m_u32AudStreamNum; ///< ��Ƶ������
	CSUDI_UINT32 m_u32SubTitleNum; ///< ��Ļ����
	CSUDI_UINT32 m_u32VideoFormat; ///< ��Ƶ�����ʽ��ֵ����ο�::HI_UNF_VCODEC_TYPE_E jia
	CSUDI_UINT16 m_u16Width; ///< ��ȣ���λ���� jia
	CSUDI_UINT16 m_u16Height; ///< �߶ȣ���λ���� jia
	CSUDI_UINT16 m_u16Channels; ///< ������, 1 or 2 jia
	CSUDI_UINT32 m_u32SampleRate; ///< 8000,11025,441000,... jia
	CSUDI_UINT32 m_u32AudioFormat; ///< ��Ƶ�����ʽ��ֵ����ο�::HA_FORMAT_E jia
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
	//�ڱ�����������ִ��ǰ����
	CSUDIAUDIOSetVolume(0, 30);

	CSUDIVIDEOShow(0, CSUDI_TRUE);
	CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO);
	CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_MATCH_METHOD_IGNORE);

	return CSUDI_TRUE;
}

BOOL CSTC_FILE_UnInit(void)
{
	//�ڱ�����������ִ�к����
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

		CSTCPrint("�ѵ���ǰ��\n");
		if (pvUserData != NULL)
		{
			*pucMoveToend = REWIND_TO_BEGIN;
		}
	}
	else if (EM_UDIFILEPLAYER_END_OF_STREAM == ePlayerEvent)
	{
		unsigned char *pucMoveToend = pvUserData;

		CSTCPrint("�ѵ����\n");
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
		case EM_UDIFILEPLAYER_ERR_ABORTED: ///< �û���ֹ��������ֹ�ɹ���Ϣ
			CSTCPrint("[FilePlayEventCallback]UDI File Play Aborted error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_DECODE: ///< ����������
			CSTCPrint("[FilePlayEventCallback]UDI File Play Decoder error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_FORMAT: ///< ý���ļ������ڻ��ļ���ʽ����
			s_ePlayerEvent = EM_UDIFILEPLAYER_ERR_FORMAT;
			CSTCPrint("[FilePlayEventCallback]UDI File Play Format error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_SEEK: ///< ���Ŷ�λ����ʱ����һ����Ϣ
			s_ePlayerEvent = EM_UDIFILEPLAYER_ERR_SEEK;
			CSTCPrint("[FilePlayEventCallback]UDI File Play Seek error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_PAUSE: ///< ��ͣ����ʧ��ʱ����һ����Ϣ
			CSTCPrint("[FilePlayEventCallback]UDI File Play Pause error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_RESUME:  ///< ״̬�ָ�ʧ��ʱ����һ����Ϣ
			CSTCPrint("[FilePlayEventCallback]UDI File Play Resume error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_SETSPEED: ///< ��������ʧ��ʱ����һ����Ϣ
			s_ePlayerEvent = EM_UDIFILEPLAYER_ERR_SETSPEED;
			CSTCPrint("[FilePlayEventCallback]UDI File Play Set Speed error !\n\r");
			break;
		case EM_UDIFILEPLAYER_ERR_NETWORK:  ///< �����쳣��m_pvDataΪint��ֵΪHTTP ��׼error code, ����http����Ϊ 200, pvEventDataֵΪ200��
			CSTCPrint("[FilePlayEventCallback]UDI File Play Network error !\n\r");
			break;

		case EM_UDIPFILELAYER_STATE_LOADING: ///< ���濪ʼ,ÿ�ν��뻺��״̬��Ҫ��һ��
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Loading !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_LOADED: ///< �������,���㹻���ݿ�ʼ����
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Loaded !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_HAVEMETADATA:  ///< �ܹ���ȡһЩ������Ϣ����Ƭ��
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Havemetadata !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_ENOUGHDATA_FORPLAY:  ///< ���㹻�����ݽ��в��ţ�����п�����˶�����
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Enoughdata For Play !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_DURATIONCHANGE:   ///< ��Ŀ��ʱ�������仯����Ҫ����������ʱ��������ͨ��CSUDIPLAYERGetDuration ��ȡ�µ���ʱ��
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Duration Change !\n\r");
			break;
		case EM_UDIPFILELAYER_STATE_RATECHANGE: ///< ��Ŀ�ܲ��ű��������仯������ͨ��CSUDIPLAYERGetSpeed ��ȡ�µĲ�������
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Rate Change !\n\r");
			break;
		case EM_UDIFILEPLAYER_STATE_LOADING_PROGRESS: ///< �ȴ����ţ��������ݵĽ���ֵ������ֵ�ͻ�ֵ��������m_pvDataΪint, �ٷ��ƣ����绺�嵽25%,  pvEventDataֵΪ25��
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Loading Progress !\n\r");
			break;
		case EM_UDIFILEPLAYER_STATE_LOADING_DOWNLOADRATE: ///< ��������, pvEventDataΪint, ��λ:kbytes/s, �������������ٶ�25kbits/s, pvDataֵΪ25��
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Loading Download Rate !\n\r");
			break;

		case EM_UDIFILEPLAYER_STATE_STOP:                 ///< ֹͣ�����������ɹ�
			s_ePlayerEvent = EM_UDIFILEPLAYER_STATE_STOP;
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Stop !\n\r");
			break;
		case EM_UDIFILEPLAYER_STATE_START:                ///< ���������������ɹ�
			s_ePlayerEvent = EM_UDIFILEPLAYER_STATE_START;
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Start !\n\r");
			break;
		case EM_UDIFILEPLAYER_STATE_PAUSE:              ///< ��ͣ�����������ɹ�
			s_ePlayerEvent = EM_UDIFILEPLAYER_STATE_PAUSE;
			CSTCPrint("[FilePlayEventCallback]UDI File Play State Pause !\n\r");
			break;
		case EM_UDIFILEPLAYER_SEEK_FINISH:           ///< ѡʱ���Ŷ�λ�ɹ�
			s_ePlayerEvent = EM_UDIFILEPLAYER_SEEK_FINISH;
			CSTCPrint("[FilePlayEventCallback]UDI File Play Seek Finish !\n\r");
			break;
		default:
			break;
	}
}

typedef struct
{
	const char*          m_pcTsFilename;  	  //�����ļ�����
	const char* 		 m_pcServiceDescript; 	//��������
	int                  m_nVideoPid;           ///< ��������PID��-1��ʾ������
	CSUDIVIDStreamType_E m_eVidStreamType;      ///��Ƶ����
	int                  m_nAudioPid;           ///< ��������PID ��-1��ʾ������
	CSUDIAUDStreamType_E m_eAudStreamType;      ///<��Ƶ����
	int 				 m_nPcrPid;				///< PCR����PID��-1��ʾ������
	int 				 m_nSubPid;				///<SUBTITLE���͵�PID��-1��ʾ������
	int 				 m_nTelPid;				///<TELETEXT���͵�PID��-1��ʾ������
}Player_TestServiceInfo_S;

typedef struct _TSStreamInfo_S
{
    char * m_pcName; //ע���ļ�����
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
		"������AC3MP2",
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
		CSTCPrint("���������ļ� %s ��ʧ�ܣ���ȷ��testdataĿ¼�´��ڸ��ļ�!!!\n", pFileName);
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
	long nFileAllLength = 0;  //�ļ��ܳ���
	int nFileCurrentPos = 0; //�ļ���ǰλ��
	int nFileLeftLength = 0; //ʣ��δ���ļ�����
	int nInjectedLength = 0; //��ע��Ĵ�С
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
		//�����ǰʣ��δ���ļ��ĳ���
		nFileCurrentPos=InjectFSTell(file);	//��ǰ�Ѷ��ļ��ĳ���

		nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���

                uLength = 0;

		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);

		if(uLength != 0)
		{
			nReadLength = pTSStreamInfo->m_uBufferLen < nFileLeftLength ? pTSStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //ʣ���ļ�����С�ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = nReadLength;
			}
			else  //ʣ���ļ����ȴ��ڵ��ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = uLength;
			}

			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //��ȡ��Ӧ���ȵ��ļ�

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

//����֧��nDemxType���͵�demx
//nDemxType�μ�CSUDIDEMUXWorkType_E
static int searchDemuxID_Y_InInject(int nDemxType)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;

	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //��ȡ��demux������
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //ȡ���ӦID=i ��demux������
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

//����֧��nAudioType���͵�Audio Decoder
//nAudioType�μ�CSUDIAUDStreamType_E, ����ΪEM_UDI_AUD_STREAM_AC3���⴦��
static int searchAudioID_Y_InInject(int nAudioType)
{
	CSUDIAUDStreamType_E  eAudStreamTypeNum=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIAUDIOCapability_S   sAudioCapabilityInfo;
	int nAudioCount = 0;
	int nAudioID = -1;
	int i = 0;
	int j = 0;

	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //��ȡ����Ƶ������������
	{
		for (i=0; i<nAudioCount; i++)
		{

			if(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo))           //�����ID����Ƶ������������
			{
				if(nAudioType == EM_UDI_AUD_STREAM_AC3)
				{
					for (j=0; j<eAudStreamTypeNum; j++)                                //�����������������棺֧�ֽ����������֧��ֱ�����������
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
					for (j=0; j<eAudStreamTypeNum; j++)                                //�����������������棺֧�ֽ����������֧��ֱ�����������
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

	CSUDIVIDEOGetCount(&nVideoCount);                                      //��ȡ����

	for ( i=0;i < nVideoCount; i ++)
	{
		CSUDIVIDEOGetCapability( i , &sVideoCapabilityInfo);	//��ȡ�������������Ƶ��ͬ������Ƶֻ��֧�ֵ���Ƶ����һ��

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

//ע�������Ƶ������
	psInjecterChnl->m_nAudioDecoder=-1;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;
	psInjecterChnl->m_nDemux=-1;

	nDemuxID=searchDemuxID_Y_InInject(EM_UDI_DEMUX_INJECT);
	if(nDemuxID == -1)
	{
		CSTCPrint( "û��֧��ע���demux! \n\r");
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
				CSTCPrint( "û��֧��ָ��������Ƶ������!\n\r");
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
				CSTCPrint( "û��֧��ָ��������Ƶ������!\n\r");
                		CSTCPrint("There's no audio decode supporting stream type %d \n",\
                    		pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eAudioType);
                		return -1;
			}
			psPlayerChnl->m_nAudioDecoder = nAudioId;
		}

		memcpy(&sStreamInfo[i],&(pTSStreamInfo->m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S));
        	nStreamCnt++;

	}

	if(nStreamCnt<1)//����ע��һ·����(video,audio,pcr...)
	{
        	CSTCPrint("û������ע������!!\n");
        	CSTCPrint("There's no stream type configed !!\n");
        	return -1;
	}

//����ͨ��
	psInjecterChnl->m_nDemux=psPlayerChnl->m_nDemux;

	pOpenParam->m_eInjecterType = EM_INJECTER_TS;
	pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_DEFAULT;

	return nStreamCnt;
}

//@CASEGROUP:CSUDIPLAYERProbe
//@DESCRIPTION:���Բ�������Ч�ԣ���������Ƿ����鿴�Ƿ񷵻�CSUDIPLAYER_ERROR_BAD_PARAMETER
//@PRECONDITION:
//@INPUT:1������ΪCSUDI_NULL
//@EXPECTATION:����CSUDIPLAYER_ERROR_BAD_PARAMETER
//@REMARK:
//@EXECUTIONFLOW:1������Ƿ�����CSUDI_NULL������CSUDIPLAYERProbe����CSUDIPLAYER_ERROR_BAD_PARAMETER
BOOL CSTC_FILE_TEST_IT_CSUDIPLAYERProbe_0001(void)
{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERProbe(CSUDI_NULL), "����Ƿ�������Ӧ�÷���CSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_FATAL_POINT;

		return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback������ִ��Ч��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���������������Ҫ��
//@EXPECTATION:��CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback����Ҫ����ο�UDI��ֲָ��
//@EXECUTIONFLOW:1����һ�������ļ�������
//@EXECUTIONFLOW:2��hPlayerΪCSUDI_NULLʱ������CSUDIPLAYERAddFilePlayerCallback����������CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:3��fnPlayerCallbackΪCSUDI_NULL ʱ������CSUDIPLAYERAddFilePlayerCallback���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayerΪCSUDI_NULLʱ������CSUDIPLAYERDelFilePlayerCallback����������CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:5��fnPlayerCallbackΪCSUDI_NULLʱ������CSUDIPLAYERDelFilePlayerCallback���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����2-����5ָ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_Add_DelPlayerCallback_0001(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int  nUserData=NUSERDATA;
	int i=0;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_FILE;

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//testing CSUDIPLAYERAddFilePlayerCallback
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERAddFilePlayerCallback(CSUDI_NULL,FilePlayEventCallback,&nUserData), "����2 ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERAddFilePlayerCallback(hPlayer,CSUDI_NULL,&nUserData), "����3 ʧ��");

		//testing CSUDIPLAYERDelFilePlayerCallback
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERDelFilePlayerCallback (CSUDI_NULL,FilePlayEventCallback,&nUserData), "����4 ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERDelFilePlayerCallback(hPlayer,CSUDI_NULL,&nUserData), "����5 ʧ��");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����14 ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback
//@DESCRIPTION:�������в�������Ӧ�ó��������Ͻӿ����Ҫ��CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback������ִ��Ч��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1�����в��������Ͻӿ����Ҫ��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback����Ҫ����ο�UDI��ֲָ��
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERAddFilePlayerCallback��ӻص�������������ͬ����(pnUserDataΪCSUDI_NULL,0x1234�Լ�������Ϸ���ֵַ)����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERDelFilePlayerCallbackɾ������2����ӵ������ص�������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2-����3ָ������
//@EXECUTIONFLOW:5������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_Add_DelPlayerCallback_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int  nUserData=NUSERDATA;
	int i=0;
	//��������������
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_FILE;

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1 ʧ��");

	for (i=0;i < REPEAT_TIMES; i++)
	{
		//testing CSUDIPLAYERAddPlayerCallback
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayEventCallback,&nUserData), "����2-1 ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayEventCallback,CSUDI_NULL), "����2-2 ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayEventCallback,(void *)0x1234), "����2-3 ʧ��");

		//testing CSUDIPLAYERDelPlayerCallback
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayEventCallback, &nUserData), "����3-1 ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayEventCallback, CSUDI_NULL), "����3-2 ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayEventCallback, (void *)0x1234), "����3-3 ʧ��");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����5 ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback
//@DESCRIPTION:��������CSUDIPLAYERAddFilePlayerCallback �����ܳɹ����32���ص�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1�����в��������Ͻӿ����Ҫ��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddFilePlayerCallback & CSUDIPLAYERDelFilePlayerCallback����Ҫ����ο�UDI��ֲָ��
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERAddFilePlayerCallback���32���ص���������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERDelFilePlayerCallbackɾ������2����ӵ�32���ص���������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2-����3ָ������
//@EXECUTIONFLOW:5������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_Add_DelPlayerCallback_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int i=0, j=0;
	int  anUserData[33] = {0};
	//��������������
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_FILE;

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1 ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//���32���ص�
		for (j=0; j<32; j++)
		{
			anUserData[j] = j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayEventCallback,&anUserData[j]), "����2ʧ��");
		}

		//ɾ��32���ص�
		for (j=0; j<32; j++)
		{
			anUserData[j] = j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback (hPlayer,FilePlayEventCallback,&anUserData[j]), "����3ʧ��");
		}
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����4 ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIPLAYEROpen&CSUDIPLAYERClose������ִ��Ч��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��sPlayerChnl��ΪNULL������CSUDIPLAYEROpen����������CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:2��ePlayerType Ϊ��Чֵ(��:EM_UDIPLAYER_MAXPLAYERTYPE)������CSUDIPLAYEROpen���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��phPlayer ΪCSUDI_NULL������CSUDIPLAYEROpen���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayer ΪCSUDI_NULL������CSUDIPLAYERClose���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����1-����4ָ������
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (NULL,EM_UDIPLAYER_MAXPLAYERTYPE,&hPlayer), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (NULL,ePlayerType,CSUDI_NULL), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClose (CSUDI_NULL), "����4ʧ��");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:������������£�CSUDIPLAYEROpen&CSUDIPLAYERClose��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���ܰ��ļ����ŷ�ʽ�����򿪲�����
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen�����ļ����ŷ�ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYERClose���رղ���2�򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3���ظ�����1-����2ָ������
CSUDI_BOOL CSTC_FILE_IT_CSUDIPLAYEROpenClose_0002(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_FILE;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int i = 0;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����2ʧ��");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:����������������£�������ε���CSUDIPLAYEROpen&CSUDIPLAYERClose�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���ܰ��ļ����ŷ�ʽ�����򿪲�����
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen�����ļ����ŷ�ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:2���������䣬�ٴε���CSUDIPLAYEROpen�����ļ����ŷ�ʽ�򿪲��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERClose���رղ���1�򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������䣬�ٴε���CSUDIPLAYERClose���رղ���1�򿪵Ĳ��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����1-����4ָ������
CSUDI_BOOL CSTC_FILE_IT_CSUDIPLAYEROpenClose_0003(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_FILE;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hHiPlayer = CSUDI_NULL;
	int i = 0;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (NULL,ePlayerType,&hHiPlayer), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClose (hPlayer), "����4ʧ��");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetPlaybackParam
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ�������£�����CSUDIPLAYERSetStream&CSUDIPLAYERSetStream&CSUDIPLAYERGetPlaybackParam���������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ�������
//@EXECUTIONFLOW:2������hPlayerΪCSUDI_NULL������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������psStreamInfo��ΪCSUDI_NULL������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������psStreamInfo��ΪCSUDI_NULL����nStreamCnt��Ϊ0������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5������nStreamCnt��Ϊ0������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������stPlaybackParamΪNULL������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������hPlayerΪCSUDI_NULL������£�����CSUDIPLAYERGetPlaybackParam��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������stPlaybackParamΪNULL������£�����CSUDIPLAYERGetPlaybackParam��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����2-����8 ָ������
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERSetStream (CSUDI_NULL,(void *)0, 0,&stPlaybackParam), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer, asStreamInfo,0,&stPlaybackParam), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,&stPlaybackParam), "����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,(void *)0,nStreamCnt,&stPlaybackParam), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,(void *)0,nStreamCnt,CSUDI_NULL), "����6ʧ��");

		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetPlaybackParam (CSUDI_NULL,&stPlaybackParam), "����7ʧ��");

		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetPlaybackParam (hPlayer,(void *)0), "����8ʧ��");

	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetPlaybackParam
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIPLAYERSetStream&CSUDIPLAYERGetPlaybackParam������ִ��Ч��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������Ϸ������Ҫ��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ�������
//@EXECUTIONFLOW:2�������ڱ����ļ�����״̬�£�CSUDIPLAYERSetStream������ִ�����
//@EXECUTIONFLOW:3�������ڱ����ļ�����״̬�£�CSUDIPLAYERGetPlaybackParam������ִ�����
//@EXECUTIONFLOW:4���ظ�����2-����3  ָ������
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,(void *)0, 0,&stPlaybackParam), "����2ʧ��");

		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPlaybackParam (hPlayer,&stPlaybackParam), "����3ʧ��");
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   & CSUDIPLAYERStop
//@DESCRIPTION:���Բ�����ָ��Ƿ������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer=CSUDI_NULL
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYERStart��ʹhPlayer=CSUDI_NULL����������CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:2������CSUDIPLAYERStop��ʹhPlayer=CSUDI_NULL����������CSUDIPLAYER_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_FILE_TEST_IT_StartStop_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERStart(CSUDI_NULL), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERStop(CSUDI_NULL), "����2ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   & CSUDIPLAYERStop
//@DESCRIPTION:������������/ֹͣһ�������������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream,�����ļ�������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������������������CSUDI_SUCCESS����������
//@EXECUTIONFLOW:4������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS������ֹͣ
//@EXECUTIONFLOW:5���ظ�����3-����4ָ������
//@EXECUTIONFLOW:6���ָ��ֳ�
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

	CSTCPrint("�����������ļ����ŵ�����������ֹͣ�������ļ�Ϊ: \"%s\"\n", stPlaybackParam.m_szFileName);

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ע��ص�ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("����Ƶ�Ƿ��ܹ���ͷ��ʼ�������ţ�\n");
    	CSTCPrint("Is A/V output normal ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����3ʧ�ܣ����Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����4ʧ��");

		CSTCPrint("����Ƶ�Ƿ�ֹͣ�����ˣ�\n");
  		CSTCPrint("Does the player stop ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ�ܣ�ֹͣ���Ų�����");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart
//@DESCRIPTION:��Ӧ�ó������������Ҫ�������£�����CSUDIPLAYERStart/ CSUDIPLAYERStop,�������ط�CSUDI_SUCCESS
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_STATUS
//@REMARK:CSUDIPLAYERStartֻ���������ļ���Ϣ֮����ֹͣ������֮����ã�CSUDIPLAYERStopֻ��������������֮��ֹͣ������֮ǰ���ܵ���
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2��CSUDIPLAYEROpen֮����������CSUDIPLAYERStop��ֹͣ���������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��CSUDIPLAYEROpen֮����������CSUDIPLAYERStart���������������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��CSUDIPLAYEROpen֮�����CSUDIPLAYERSetStream,������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��CSUDIPLAYERSetStream֮��CSUDIPLAYERStart֮ǰ������CSUDIPLAYERStop��ֹͣ���������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���ٴε���CSUDIPLAYERStart���������������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ٴε���CSUDIPLAYERStop��ֹͣ���������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����5-����9ָ������
//@EXECUTIONFLOW:11���ָ��ֳ�
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ע��ص�ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer), "����7ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����8ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "����9ʧ��");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:���Բ��������ΪCSUDI_NULLʱ��CSUDIPLAYERPause & CSUDIPLAYERResume�ĵ������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer=CSUDI_NULL
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYERPause��ʹhPlayer=CSUDI_NULL����������CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:2������CSUDIPLAYERResume��ʹhPlayer=CSUDI_NULL����������CSUDIPLAYER_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_FILE_TEST_IT_PauseResume_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERPause(CSUDI_NULL), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERResume(CSUDI_NULL), "����2ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:����Ӧ�ó������������Ҫ��ʱ��CSUDIPLAYERPause & CSUDIPLAYERResume������ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:ֱ�����������Բ�֧��CSUDIPLAYERPause & CSUDIPLAYERResume
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2��Open����������������CSUDIPLAYERPause���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��Open����������������CSUDIPLAYERResume���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��Open�����������CSUDIPLAYERSetStream������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��Start����֮ǰ������CSUDIPLAYERPause���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��Start����֮ǰ������CSUDIPLAYERResume���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERStart��Start����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8��Start֮��Pause����֮ǰ������CSUDIPLAYERResume���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERPause��ͣ����������������CSUDI_SUCCESS ��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:10�����CSUDIPLAYERPause�ɹ��������CSUDIPLAYERStart���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:11�����CSUDIPLAYERPause�ɹ��������CSUDIPLAYERSetStream���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12�����CSUDIPLAYERPause�ɹ������ٴε���CSUDIPLAYERPause���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:13�����CSUDIPLAYERPause�ɹ��������CSUDIPLAYERResume����������CSUDI_SUCCESS
//@EXECUTIONFLOW:14�����CSUDIPLAYERPause�ɹ������ٴε���CSUDIPLAYERResume���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:15�����CSUDIPLAYERPause�ɹ��������CSUDIPLAYERPause����������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17���ظ�����5-����16ָ������
//@EXECUTIONFLOW:18���ָ��ֳ�
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause (hPlayer), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ע��ص�ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause (hPlayer), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "����8ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		udiRe = CSUDIPLAYERPause(hPlayer);
		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED==udiRe)),"����9ʧ��");

		if (CSUDI_SUCCESS == udiRe)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERStart(hPlayer), "����10ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "����11ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause(hPlayer), "����12ʧ��");

			CSUDIOSThreadSleep(SLEEP_TIME);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����13ʧ��");

			CSUDIOSThreadSleep(SLEEP_TIME);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "����14ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����15ʧ��");

			CSUDIOSThreadSleep(SLEEP_TIME);
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����16ʧ��");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:����������ͣ/�ָ�һ�������������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream,�����ļ�������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������������������CSUDI_SUCCESS����������
//@EXECUTIONFLOW:4������CSUDIPLAYERPause����ͣ����������������CSUDI_SUCCESS��������ͣ
//@EXECUTIONFLOW:5������CSUDIPLAYERResume���ָ����ţ���������CSUDI_SUCCESS�������ָ�����
//@EXECUTIONFLOW:6���ظ�����4-����5ָ������
//@EXECUTIONFLOW:7������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS������ֹͣ
//@EXECUTIONFLOW:8���ָ��ֳ�
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

	CSTCPrint("�����������ļ����ŵ�������ͣ�ͻָ��������ļ�Ϊ: \"%s\"\n", stPlaybackParam.m_szFileName);

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����3ʧ��");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTCPrint("Is A/V output normal ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����3ʧ�ܣ����Ų�����");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer), "����4ʧ��");

		CSTCPrint("����Ƶ�Ƿ���ͣ�����ˣ�\n");
  		CSTCPrint("Does the player pause ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ�ܣ���ͣ���Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer), "����5ʧ��");

		CSTCPrint("����Ƶ�Ƿ����ͣ��ָ������ˣ�\n");
  		CSTCPrint("Does the player resume ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ��ָ����Ų�����");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����7ʧ��");

	CSTCPrint("����Ƶ�Ƿ�ֹͣ�����ˣ�\n");
	CSTCPrint("Does the player stop ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ�ֹͣ���Ų�����");

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetSpeed&CSUDIPLAYERGetSpeed
//@DESCRIPTION:1�����Ա����ļ���������£�CSUDIPLAYERSetSpeed������ִ�����
//@DESCRIPTION:2�����Ա����ļ���������£�CSUDIPLAYERGetSpeed������ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��hPlayerΪCSUDI_NULL������ CSUDIPLAYERSetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��eSpeedΪ��Чֵ������ CSUDIPLAYERSetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��hPlayerΪCSUDI_NULL������ CSUDIPLAYERGetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��eSpeedΪ��Чֵ������ CSUDIPLAYERGetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��������ȷ������£�Start������֮ǰ����CSUDIPLAYERSetSpeed���������صĲ�ΪCSUDI_SUCCESS��ΪCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:8��������ȷ������£�Start������֮ǰ����CSUDIPLAYERGetSpeed���������صĲ�ΪCSUDI_SUCCESS��ΪCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9������CSUDIPLAYERStart��Start����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:10��������ȷ������£�Start֮�����CSUDIPLAYERSetSpeed����������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:11��������ȷ������£�Start֮�����CSUDIPLAYERGetSpeed����������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:12������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13���ظ�����3-����11ָ������
//@EXECUTIONFLOW:14���ָ��ֳ�
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer,NULL,0,&stPlaybackParam), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ע��ص�ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERSetSpeed(CSUDI_NULL,eSpeed), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSetSpeed(CSUDI_NULL,EM_UDIPLAYER_SPEED_FASTFORWARD_32+10), "����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERGetSpeed(CSUDI_NULL,&eSpeed), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetSpeed(hPlayer,CSUDI_NULL), "����6ʧ��");

		errorCode = CSUDIPLAYERSetSpeed(hPlayer,eSpeed);

		//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS != errorCode), "����7ʧ��");

		errorCode = CSUDIPLAYERGetSpeed(hPlayer,&eSpeed);

		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "����8ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		errorCode = CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_32);

		//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "����10ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		errorCode = CSUDIPLAYERGetSpeed(hPlayer,&eSpeed);

        CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "����11ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, NULL), "ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetSpeed&CSUDIPLAYERGetSpeed
//@DESCRIPTION:1�����Ա��ز�������£�CSUDIPLAYERSetSpeed������ִ�����
//@DESCRIPTION:2�����Ա��ز�������£�CSUDIPLAYERGetSpeed������ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��hPlayerΪCSUDI_NULL������ CSUDIPLAYERSetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��eSpeedΪ��Чֵ������ CSUDIPLAYERSetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��hPlayerΪCSUDI_NULL������ CSUDIPLAYERGetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��eSpeedΪ��Чֵ������ CSUDIPLAYERGetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��������ȷ������£�Start������֮ǰ����CSUDIPLAYERSetSpeed���������صĲ�ΪCSUDI_SUCCESS��ΪCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:8��������ȷ������£�Start������֮ǰ����CSUDIPLAYERGetSpeed���������صĲ�ΪCSUDI_SUCCESS��ΪCSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9������CSUDIPLAYERStart��Start����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:10��������ȷ������£�Start֮�����CSUDIPLAYERSetSpeed����������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:11��������ȷ������£�Start֮�����CSUDIPLAYERGetSpeed����������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:12������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13���ظ�����3-����11ָ������
//@EXECUTIONFLOW:14���ָ��ֳ�
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL   != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback, &moveToend), "ע��ص�ʧ��");

	bAddCallback = CSUDI_TRUE;

	for (i = 0;i < REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE   == CSUDIPLAYERSetSpeed(CSUDI_NULL, eSpeed), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   != CSUDIPLAYERSetSpeed(CSUDI_NULL, EM_UDIPLAYER_SPEED_FASTFORWARD_32+10), "����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE   == CSUDIPLAYERGetSpeed(CSUDI_NULL, &eSpeed), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   != CSUDIPLAYERGetSpeed(hPlayer,CSUDI_NULL), "����6ʧ��");

		errorCode = CSUDIPLAYERSetSpeed(hPlayer, eSpeed);

		//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode) || (CSUDI_SUCCESS != errorCode), "����7ʧ��");

		errorCode = CSUDIPLAYERGetSpeed(hPlayer, &eSpeed);

		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode) || (CSUDI_SUCCESS == errorCode), "����8ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

		bPlaying = CSUDI_TRUE;

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("����Ƶ�Ƿ����������ţ�\n");
		CSTCPrint("Is A/V output normal ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����9ʧ�ܣ����Ų�����");

		moveToend = 0;
		errorCode = CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_32);

		//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode) || (CSUDI_SUCCESS == errorCode), "����10ʧ��");

		CSTCPrint("��Ƶ�Ƿ������ţ� \n");
		CSTCPrint("Is A/V output fast forward?");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����10ʧ�ܣ����������");

		if (FORWARD_TO_END == moveToend)
		{
			CSTCPrint("��Ƶ�Ѿ������ĩβ��\n");
			break;
		}

		errorCode = CSUDIPLAYERGetSpeed(hPlayer, &eSpeed);

        CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode) || (CSUDI_SUCCESS == errorCode), "����11ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   == CSUDIPLAYERStop(hPlayer), "����12ʧ��");

		bPlaying = CSUDI_FALSE;

		CSTCPrint("��Ƶ�Ƿ�ֹͣ���ţ� \n");
		CSTCPrint("Is A/V output stop?");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����10ʧ�ܣ�ֹͣ������");
	}

	CSTK_FATAL_POINT;

	if (bPlaying)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��");
	}

	if (bAddCallback)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, &moveToend), "ɾ���ص�ʧ��");
	}

	if (CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS   == CSUDIPLAYERClose(hPlayer), "�ָ��ֳ�");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSeek
//@DESCRIPTION:1�����Ա����ļ���������£��ӿں���CSUDIPLAYERSeek��ִ�����
//@DESCRIPTION:2��CSUDIPLAYERSeek���������������֤
//@DESCRIPTION:3��CSUDIPLAYERSeek CSUDIPLAYERGetCurPosInSec���ʹ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1����Ч��hPlayer
//@INPUT:2����Ч��nPosInSec
//@INPUT:3��ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_CURRENT
//@EXPECTATION:ֱ��ģʽ�²�֧��  CSUDIPLAYERSeek
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayerΪCSUDI_NULL������ CSUDIPLAYERSeek���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���������Ϸ��������� CSUDIPLAYERSeek����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����3-����4ָ������
//@EXECUTIONFLOW:7������CSUDIPLAYERStop����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���ָ��ֳ�
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback, NULL), "ע��ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSeek(CSUDI_NULL,100,ePlayPosFlag), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEP_TIME);

	for (ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_HEAD;ePlayPosFlag<=EM_UDIPLAYER_POSITION_FROM_END;ePlayPosFlag++)
	{
		if (ePlayPosFlag == EM_UDIPLAYER_POSITION_FROM_END)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSeek(hPlayer,-100 ,ePlayPosFlag), "����5ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSeek(hPlayer, 50, ePlayPosFlag), "����5ʧ��");
		}

		CSUDIOSThreadSleep(SLEEP_TIME);
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback, NULL), "ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPosInSec
//@DESCRIPTION:1�����Ա����ļ���������£��ӿں���CSUDIPLAYERGetCurPosInSec��ִ�����
//@DESCRIPTION:2��CSUDIPLAYERGetCurPosInSec  CSUDIPLAYERSetSpeed  CSUDIPLAYERSeek���ʹ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��hPlayerΪCSUDI_NULL������ CSUDIPLAYERGetCurPosInSec���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��pnPosInSecΪCSUDI_NULL������ CSUDIPLAYERGetCurPosInSec���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���������Ϸ��������� CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���������Ϸ��������� CSUDIPLAYERSeek����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������Ϸ��������� CSUDIPLAYERResume����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������Ϸ��������� CSUDIPLAYERGetCurPosInSec����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������Ϸ��������� CSUDIPLAYERStop����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����3-����5ָ������
//@EXECUTIONFLOW:10���ָ��ֳ�
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ע��ص�ʧ��");

    for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(CSUDI_NULL,&nPosInSec), "����3ʧ��");

		//check porting code pls,it have not check pnPosInSec
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(hPlayer,CSUDI_NULL), "����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����5ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 100, EM_UDIPLAYER_POSITION_FROM_CURRENT), "����6ʧ��");

		//CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer), "�ָ�����ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetCurPosInSec(hPlayer,&nPosInSec), "����7ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");

		CSUDIOSThreadSleep(300);//sleep 300ms ��Ϊ��stop֮������ȫ�����Ƶ����
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetDuration
//@DESCRIPTION:1�����Ա����ļ���������£��ӿں���CSUDIPLAYERGetDuration��ִ�����
//@DESCRIPTION:2�����������õ���ʱ���ʵ��������1��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayerΪCSUDI_NULL������ CSUDIPLAYERGetDuration���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��pnPosInSecΪCSUDI_NULL������ CSUDIPLAYERGetDuration���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6���������Ϸ��������� CSUDIPLAYERGetDuration����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERStop����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���ظ�����3-����5ָ������
//@EXECUTIONFLOW:9���ָ��ֳ�
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

	//����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ע��ص�ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart (hPlayer), "����3ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetDuration(CSUDI_NULL,&nPosInSec), "����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetDuration(hPlayer,NULL), "����5ʧ��");

		eCode = CSUDIPLAYERGetDuration(hPlayer,&nPosInSec);

		int value = 0;
		value = nPosInSec - s_svrfiletime[i];

		if (nPosInSec >= s_svrfiletime[i])
		{
			CSTK_ASSERT_TRUE_FATAL( value >= 0 && value <= 1, "��ȡ�ļ���ʱ��ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(value <= -1, "��ȡ�ļ���ʱ��ʧ��");
		}

		CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == eCode)||(CSUDI_SUCCESS == eCode), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����7ʧ��");

		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));
		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;

		j++;

		strncpy(stPlaybackParam.m_szFileName, stFilePath[j].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,NULL,0,&stPlaybackParam), "�������ò�������ʧ��");

	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer,FilePlayCallback,NULL), "ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");

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

	CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName)), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����2ʧ�ܣ����ļ��쳣");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "����3��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback,  CSUDI_NULL), "����4��ӻص�ʧ��");

	bAddCallback = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����5ʧ��");

	bPlaying = CSUDI_TRUE;

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTCPrint("Is A/V output normal ?\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����6ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����7ʧ��");

	bPlaying = CSUDI_FALSE;

	CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
	CSTCPrint("Does the player stop ?\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����8ʧ�ܣ�ֹͣ���Ų�����");
	
	CSTK_FATAL_POINT;

	if (bPlaying)
	{
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��");
	}

	if (bAddCallback)
	{
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, CSUDI_NULL), "ɾ���ص�ʧ��");
	}
			
	if (CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�ر��ļ�ʧ��");
		
		hPlayer = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ��ű���TS ��
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0001(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_TS_SD);

	return CSUDI_TRUE;
}


//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ��Ÿ���TS��
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback����Ӳ��ŵ��ļ�β�Ļص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0002(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_TS_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���WMV��ʽ�������ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback����Ӳ��ŵ��ļ�β�Ļص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0003(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_WMV);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���AVI��ʽ�����ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0004(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_AVI_SD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���AVI��ʽ�����ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0005(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_AVI_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���RMVB��ʽ�����ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0006(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_RMVB_SD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���RMVB��ʽ�����ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0007(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_RMVB_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���FLV��ʽ�ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0008(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_FLV);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���MP3��ʽ�ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0009(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MP3);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���M4A��ʽ�ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0010(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_M4A);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���AC3��ʽ�ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0011(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_AC3);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���MP3��ʽ�ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0012(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_AAC);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���MP4��ʽ�����ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0013(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MP4_SD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���MP4��ʽ�����ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0014(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MP4_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���3GP��ʽ�ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵ��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0015(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_3GP);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���MOV��ʽ�ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵ��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0016(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MOV);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���MKV��ʽ�����ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵ��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0017(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MKV_SD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���MKV��ʽ�����ļ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵ��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0018(void)
{
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MKV_HD);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ�֧�ֲ���MKV��ʽ������ļ�����С��10GB����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵ��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:10    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0019(void)
{
	CSTCPrint("���������ڲ��Բ��Ŵ��ļ�����ȷ���������ݴ���10GB!\n");
	FILE_TEST_IT_Play_Base(EM_UDI_FILENAME_MKV_HD_BSIZE);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ������Ƿ�֧�ֲ�ͬ�ļ�֮���л�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERAddFilePlayerCallback�������Ƶ��Դ�ı�Ļص��¼�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7���ٴε���CSUDIPLAYERSetStream���������ļ��Ĳ�������Ӧ����
//@EXECUTIONFLOW:8������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬����Ƶ�Ƿ�ʼ����
//@EXECUTIONFLOW:10������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:11����ʾ������Ա�۲죬����Ƶ�Ƿ�ֹͣ����
//@EXECUTIONFLOW:12������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:13���ظ�����2-����10
//@EXECUTIONFLOW:14    ����CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_0020(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDIPlaybackParam_S stPlaybackParam;
	int i = 0;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;

	memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

	CSTCPrint("���������Ա����ļ����Ų�ͬ�ļ�֮���л�\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	for(i = 0; i < REPEAT_TIMES; i++)
	{
		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;
		strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MKV_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTCPrint("�� %d �β��� \"%s\" �Ĳ���\n", i+1, stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����2��������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback,  CSUDI_NULL), "����3��ӻص�ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
		CSTCPrint("Is A/V output normal ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
  		CSTCPrint("Does the player stop ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ�ֹͣ����ʧ��");

		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;
		strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MP4_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTCPrint("�� %d �β��� \"%s\" �Ĳ���\n", i+1, stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����7��������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

		CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
		CSTCPrint("Is A/V output normal ?\n");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ�ܣ����Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");

		CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
  		CSTCPrint("Does the player stop ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ�ܣ�ֹͣ���Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback,  CSUDI_NULL), "����12ɾ���ص�ʧ��");
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����14ʧ��");
		hPlayer=CSUDI_NULL;
	}

	return CSUDI_TRUE;
}


//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ�������ֱ���ܹ������л�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERAddFilePlayerCallback�������Ƶ��Դ�ı�Ļص��¼�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:8����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:9    ����CSUDIPLAYERCloseɾ��������
//@EXECUTIONFLOW:10������CSUDIPLAYEROpen����һ��ֱ��������
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:12������CSUDIPLAYERAddFilePlayerCallback�������Ƶ��Դ�ı�Ļص��¼�
//@EXECUTIONFLOW:13������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:14����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:15������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:16������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:17����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:18    ����CSUDIPLAYERCloseɾ��������
//@EXECUTIONFLOW:19���ظ�����1-����18ָ������
//@EXECUTIONFLOW:20���ָ��ֳ�
CSUDI_BOOL CSTC_FILE_TEST_IT_0021(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hLivePlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S stPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[3];  //���ᳬ��10��
	int nStreamCnt = 0;
	CSUDIPlaybackParam_S stPlaybackParam;
	int i = 0;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_FILE;

	CSTCPrint("���������Ա����ļ�������ֱ�����л�\n");

	for(i = 0; i < REPEAT_TIMES; i++)
	{
		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		ePlayerType = EM_UDIPLAYER_FILE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;

		strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MKV_SD].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTCPrint("�� %d �β��� \"%s\" ���ļ�����\n", i+1, stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����2��������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback, CSUDI_NULL), "����3��ӻص�ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
		CSTCPrint("Is A/V output normal ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

		CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
  		CSTCPrint("Does the player stop ?\n");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback,  CSUDI_NULL), "����7ɾ���ص�ʧ��");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����8ʧ�ܣ�ֹͣ���Ų�����");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����9ʧ��");

  		hPlayer=CSUDI_NULL;

		CSUDIOSThreadSleep(SLEEP_TIME);

		ePlayerType = EM_UDIPLAYER_LIVE;

		stPlayerChnl.m_nDemux = 0;
		stPlayerChnl.m_nAudioDecoder = 0;
		stPlayerChnl.m_nVideoDecoder = 0;

		CSTCPrint("�� %d �β��� \"%s\" ��ֱ������\n", i+1, g_sPlayer_SeviceInfo[LivePlayer].m_pcTsFilename);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== PLAYER_iLockTuner (), "��Ƶʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,ePlayerType,&hLivePlayer), "����10ʧ��");

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

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hLivePlayer, sStreamInfo, nStreamCnt, NULL), "����11��������Ϣʧ��");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERAddPlayerCallback(hLivePlayer, PlayCallback , EM_UDIPLAYER_VIDEO_FRAME_COMING, CSUDI_NULL), "����12��ӻص�ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hLivePlayer), "����13ʧ��");

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
		CSTCPrint("Is A/V output normal ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ�ܣ����Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hLivePlayer), "����15ʧ��");
		CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
  		CSTCPrint("Does the player stop ?\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����16ʧ�ܣ�ֹͣ���Ų�����");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hLivePlayer, PlayCallback , EM_UDIPLAYER_VIDEO_FRAME_COMING, CSUDI_NULL), "����17ɾ���ص�ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "����18ʧ��");

		hLivePlayer=CSUDI_NULL;
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����20ʧ��");
		hPlayer=CSUDI_NULL;
	}

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "����21ʧ��");
		hLivePlayer=CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ�������ע�벥���ܹ������л�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERAddFilePlayerCallback�������Ƶ��Դ�ı�Ļص��¼�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERDelFilePlayerCallback��ɾ���ص��¼�
//@EXECUTIONFLOW:8����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:9    ����CSUDIPLAYERCloseɾ��������
//@EXECUTIONFLOW:10������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:13������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:16������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:17����ʾ�û�ȷ�ϣ�����Ƶ��Ŀ�����ܲ��ܲ��ų���
//@EXECUTIONFLOW:18��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:19      ����CSUDIINJECTERClear���Inecter�Ѿ�ע�뵽Ŀ�껺�����е���û�в������ȫ������
//@EXECUTIONFLOW:20������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:21������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:22������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:23������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:24������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:25���ظ�����1-����24
//@EXECUTIONFLOW:26���ָ��ֳ�
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
	CSUDIStreamInfo_S  stStreamInfo[3];  //���ᳬ��10��
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

	CSTCPrint("���������Ա����ļ�������ֱ�����л�\n");

	nStreamCnt = getTSInjectInfo(&stInjecterChnl,&stPlayerChnl,&stOpenParams,stStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(-1 != nStreamCnt, "��ȡע��TS ����Ϣʧ��")

	for(i = 0; i < REPEAT_TIMES; i++)
	{
		memset(&stPlaybackParam,0,sizeof(stPlaybackParam));

		ePlayerType = EM_UDIPLAYER_FILE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

		stPlaybackParam.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
		stPlaybackParam.m_nSecondPos = 0;

		strncpy(stPlaybackParam.m_szFileName, stFilePath[EM_UDI_FILENAME_MKV_HD].filename, CSUDI_MAX_FILE_NAME_LEN);

		CSTCPrint("�� %d �β��� \"%s\" ���ļ�����\n", i+1, stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����2��������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback, CSUDI_NULL), "����3��ӻص�ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	    	CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	    	CSTCPrint("Is A/V output normal ?\n");

	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
    		CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
  		CSTCPrint("Does the player stop ?\n");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, CSUDI_NULL), "����7ɾ���ص�ʧ��");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����8ʧ�ܣ����Ų�����");

  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����9ʧ��");

  		hPlayer=CSUDI_NULL;

		CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("�� %d �β��� \"%s\" ��ע�벥��\n", i+1, g_TS_StreamInfo[LivePlayer].m_pcName);

		ePlayerType = EM_UDIPLAYER_INJECT;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&stInjecterChnl,&stOpenParams,&hInject), "����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,ePlayerType,&hInjectPlayer), "����11ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hInject,EM_UDIINJECTER_PES_SYNC,&stSyncMode), "����12ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hInjectPlayer, stStreamInfo, nStreamCnt, NULL), "����13��������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hInject), "����14ʧ��");

		g_TS_StreamInfo[LivePlayer].m_hInjecter = hInject;
		g_TS_StreamInfo[LivePlayer].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TS_StreamInfo[LivePlayer], &hInjectThread), "����15ʧ��");

		while(g_TS_StreamInfo[LivePlayer].m_bRun == CSUDI_FALSE)
		{
			CSUDIOSThreadSleep(500);
		}

		 CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TS_StreamInfo[LivePlayer].m_bOpenFileSuccess, "�ļ���ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hInjectPlayer), "����16ʧ��");

	    	CSUDIOSThreadSleep(SLEEP_TIME);

		CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	    	CSTCPrint("Is A/V output normal ?\n");

	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����17ʧ�ܣ����Ų�����");

	    	g_TS_StreamInfo[LivePlayer].m_bRun = CSUDI_FALSE;

	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����18ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hInject),"����19ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hInjectPlayer), "����20ʧ��");
    		CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
  		CSTCPrint("Does the player stop ?\n");


		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInject), "����21ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hInjectPlayer), "����22ʧ��");

		hInjectPlayer = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hInject), "����23ʧ��");

		hInject = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����24ʧ��");

		hInjectThread = CSUDI_NULL;
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����26ʧ��");
		hPlayer=CSUDI_NULL;
	}

	if (hInjectPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hInjectPlayer), "����26ʧ��");
		hInjectPlayer=CSUDI_NULL;
	}

	if (hInject != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hInject), "����26ʧ��");
		hInject = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����26ʧ��");
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

	CSTCPrint("���������� \"%s\" �Ĳ���(���ŵ�ĩβ)\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == (udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName)), "��֧�ֵĸ�ʽ");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "�������ž��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "�����Ĳ��ž��Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayCallback, (void *)&ucPlayToend), "ע��ص�ʧ��");

	bAddCallback = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "�����ļ�ʧ��");
	
	bPlaying = CSUDI_TRUE;

	// ��ȡ��ʼ���ŵ�ʱ��
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sStartTime), "��ȡ��ʼʱ��ʧ��");
	
	CSUDIOSThreadSleep(SLEEP_TIME);

	// �������״̬���
	ucPlayToend = 0;

	CSTCPrint("����Ƶ�Ƿ����������ţ�\n");
	CSTCPrint("Is A/V output normal? \n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����Ƶ����ʧ��");

	CSTCPrint("���ڲ���...\n");
	
	// �ȴ��������
	while (PLAY_TO_END != ucPlayToend)
	{
		CSUDIOSThreadSleep(50);
	}

	bPlaying = CSUDI_FALSE;
	
	CSTCPrint("�������\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sEndTime), "��ȡ����ʱ��ʧ��");

	nSpanTime = sEndTime.m_nSecond - sStartTime.m_nSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetDuration(hPlayer, &nPosInSec), "��ȡ��Ŀ�ܳ���ʧ��");

	// ���2s����
	CSTK_ASSERT_TRUE_FATAL(((nPosInSec>nSpanTime) ? (2>=(nPosInSec-nSpanTime)) : (2>=(nSpanTime-nPosInSec))), "����ʱ���쳣");

	CSTK_FATAL_POINT;

	if (bPlaying)
	{
		bPlaying = CSUDI_FALSE;
		
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��");
	}

	if (bAddCallback)
	{
		bAddCallback = CSUDI_FALSE;
		
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayCallback, (void *)&ucPlayToend), "ɾ���ص�ʧ��");	
	}

	if (CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�ָ��ֳ�");

		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ���������������MP3��Ƶ
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: void
//@EXPECTATION: ������������
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7���ȴ��������
//@EXECUTIONFLOW:8���жϲ�����ʱ����ʵ�ʲ���ʱ���Ƿ����
//@EXECUTIONFLOW:9������CSUDIPLAYERDelFilePlayerCallback��ɾ���ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:10������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_PlayToEnd_0001(void)
{
	FILE_TEST_IT_PlayToEnd_Base(EM_UDI_FILENAME_MP3);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER
//@DESCRIPTION:���Ա����ļ��Ƿ��������������Ŵ�����MKV��Ƶ
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: void
//@EXPECTATION: ������������
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYERProbe���ڴ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERAddFilePlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:7���ȴ��������
//@EXECUTIONFLOW:8���жϲ�����ʱ����ʵ�ʲ���ʱ���Ƿ����
//@EXECUTIONFLOW:9������CSUDIPLAYERDelFilePlayerCallback��ɾ���ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:10������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_FILE_TEST_IT_PlayToEnd_0002(void)
{
	FILE_TEST_IT_PlayToEnd_Base(EM_UDI_FILENAME_MKV_HD_BSIZE);

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER CALLBACK
//@DESCRIPTION:���Ա����ļ��Ƿ��ļ�·���ص��ķ���ֵ
//@PRECONDITION:PLAYERģ��������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:2������CSUDIPLAYERAddPlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ���ԣ�FileNameΪ�Ƿ�·��"./testdata/error.ts"
//@EXECUTIONFLOW:4������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:5���ȴ�3�룬������EM_UDIFILEPLAYER_ERR_FORMAT�Ļص��¼�����
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERDelPlayerCallback��ɾ���ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:8������CSUDIPLAYERCloseɾ��������
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "����2��ӻص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����3��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSTCPrint("��ȴ�2��\n");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(EM_UDIFILEPLAYER_ERR_FORMAT == s_ePlayerEvent,"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "����7ɾ���ص�ʧ��");

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����8ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER CALLBACK
//@DESCRIPTION:���Ա����ļ����Ŷ�λ����ص��ķ���ֵ
//@PRECONDITION:PLAYERģ��������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:2������CSUDIPLAYERAddPlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ���ԣ�FileNameΪ�Ƿ�·��"./testdata/error.ts"
//@EXECUTIONFLOW:4������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:5������CSUDIPLAYERSeek  ���ô���λ�ã�Ϊ�ļ���ͷ��-1
//@EXECUTIONFLOW:6���ȴ�3�룬������EM_UDIFILEPLAYER_ERR_SEEK�Ļص��¼�����
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelPlayerCallback��ɾ���ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:9�� ����CSUDIPLAYERCloseɾ��������
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "����2��ӻص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����3��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERSeek(hPlayer, -1, EM_UDIPLAYER_POSITION_FROM_HEAD), "����5ʧ��");

	CSTCPrint("��ȴ�2��\n");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(EM_UDIFILEPLAYER_ERR_SEEK == s_ePlayerEvent,"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "����8ɾ���ص�ʧ��");

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����9ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER CALLBACK
//@DESCRIPTION:���Ա����ļ����ô������ʻص��ķ���ֵ
//@PRECONDITION:PLAYERģ��������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:2������CSUDIPLAYERAddPlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:5������CSUDIPLAYERSetSpeed  ���ô������ʣ��ֱ�Ϊ(EM_UDIPLAYER_SPEED_NORMAL-1)��(EM_UDIPLAYER_SPEED_MAX)
//@EXECUTIONFLOW:6���ȴ�3�룬������EM_UDIFILEPLAYER_ERR_SETSPEED�Ļص��¼�����
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERDelPlayerCallback��ɾ���ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:9������CSUDIPLAYERCloseɾ��������
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "����2��ӻص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����3��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_NORMAL-1), "����5ʧ��");

	CSTCPrint("��ȴ�2��\n");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(EM_UDIFILEPLAYER_ERR_SETSPEED == s_ePlayerEvent,"����6ʧ��");

	s_ePlayerEvent = EM_UDIFILEPLAYER_MAXEVENTTYPE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_MAX), "����5ʧ��");

	CSTCPrint("��ȴ�2��\n");

	CSUDIOSThreadSleep(SLEEP_TIME);

	CSTK_ASSERT_TRUE_FATAL(EM_UDIFILEPLAYER_ERR_SETSPEED == s_ePlayerEvent,"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "����8ɾ���ص�ʧ��");

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����9ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:FILEPLAYER CALLBACK
//@DESCRIPTION:���Ա����ļ���������\ѡʱ\��ͣ\�ָ�\ֹͣ���̵Ļص��¼�
//@PRECONDITION:PLAYERģ��������ʼ��
//@INPUT:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ�������
//@EXECUTIONFLOW:2������CSUDIPLAYERAddPlayerCallback������ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:4������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:5������3������EM_UDIFILEPLAYER_STATE_START�Ļص��¼�����
//@EXECUTIONFLOW:6������CSUDIPLAYERSeek ���ò�����λ��
//@EXECUTIONFLOW:7������3������EM_UDIFILEPLAYER_SEEK_FINISH�Ļص��¼�����
//@EXECUTIONFLOW:8������CSUDIPLAYERPause ��ͣ������
//@EXECUTIONFLOW:9������3������EM_UDIFILEPLAYER_STATE_PAUSE�Ļص��¼�����
//@EXECUTIONFLOW:10������CSUDIPLAYERResume �ָ�������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:12������3������EM_UDIFILEPLAYER_STATE_STOP�Ļص��¼�����
//@EXECUTIONFLOW:13������CSUDIPLAYERDelPlayerCallback��ɾ���ļ���ʼ���ŵĻص��¼�
//@EXECUTIONFLOW:14������CSUDIPLAYERCloseɾ��������
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "����2��ӻص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����3��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	for(i=0;i<MAX_SLEEP_NUM;i++)
	{
		CSUDIOSThreadSleep(100);
		if(EM_UDIFILEPLAYER_STATE_START == s_ePlayerEvent)
			break;
	}

	if(i==MAX_SLEEP_NUM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "����5ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD), "����6ʧ��");

	for(i=0;i<MAX_SLEEP_NUM;i++)
	{
		CSUDIOSThreadSleep(100);
		if(EM_UDIFILEPLAYER_SEEK_FINISH == s_ePlayerEvent)
			break;
	}

	if(i==MAX_SLEEP_NUM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "����7ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer), "����8ʧ��");

	for(i=0;i<MAX_SLEEP_NUM;i++)
	{
		CSUDIOSThreadSleep(100);
		if(EM_UDIFILEPLAYER_STATE_PAUSE == s_ePlayerEvent)
			break;
	}

	if(i==MAX_SLEEP_NUM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "����9ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer), "����10ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����11ʧ��");

	for(i=0;i<MAX_SLEEP_NUM;i++)
	{
		CSUDIOSThreadSleep(100);
		if(EM_UDIFILEPLAYER_STATE_STOP == s_ePlayerEvent)
			break;
	}

	if(i==MAX_SLEEP_NUM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "����12ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelFilePlayerCallback(hPlayer, FilePlayEventCallback, CSUDI_NULL), "����13ɾ���ص�ʧ��");

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����14ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:������������Ƿ�������£��Ƿ񷵻�CSUDIPLAYER_ERROR_BAD_PARAMETER
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����һ����������Ϊ�գ���������CSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo���ڶ�����������Ϊ�գ���������CSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����������������Ϊ�գ���������CSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4������CSUDIPLAYERClose�رղ��ž��
//@EXECUTIONFLOW:5���ָ��ֳ�
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
		CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1:�������ž��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1:�����Ĳ��ž��Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "����2:��������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetFileInfo(CSUDI_NULL, &stFileInfo), "����3:����Ƿ������Ӧ�÷���CSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetFileInfo(hPlayer, CSUDI_NULL), "����4:����Ƿ�������Ӧ�÷���CSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERGetFileInfo(CSUDI_NULL, CSUDI_NULL), "����5:����Ƿ�������Ӧ�÷���CSUDIPLAYER_ERROR_BAD_PARAMETER");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���ܹ�ֱ��֧�ֱ���ý�岥��");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����6:�رղ��ž��ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:���Ի�ȡ ts���ļ�����Ϣ�Ƿ���ȷ���
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
///@INPUT:1����Ч��hPlayer
//@INPUT:2����Ч��stFileStreamId
//@EXPECTATION:��֧��  CSUDIPLAYERGetFileInfo��ȡ��ͨ�ļ���Ϣ
//@REMARK:��������Ϊallts.ts
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����ȡ�ļ�����Ϣ
//@EXECUTIONFLOW:4���Ƚ��ļ���Ϣ�ͻ�ȡ�����ļ���Ϣ�Ƿ���ȣ����ȫ�������֤������ȷ��
//@EXECUTIONFLOW:5������CSUDIPLAYERClose�رղ��ž��
//@EXECUTIONFLOW:6���ָ��ֳ�
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

	CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

	udiRe = CSUDIPLAYERProbe(stPlaybackParam.m_szFileName);

	if(udiRe == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "�������ž��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "�����Ĳ��ž��Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "��������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "��ȡ��Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_eStreamType == EM_UDIFILEPLAYER_STREAM_ES, "�ļ����ʹ���");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_n64FileSize== stFileInfo.m_n64FileSize, "�ļ���С��һ��");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_n64StartTime== stFileInfo.m_n64StartTime, "�ļ�������ʱ������");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_n64Duration== stFileInfo.m_n64Duration, "�ļ���ʱ������");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32Bps == stFileInfo.m_u32Bps, "�ļ����ʴ���");
		CSTK_ASSERT_TRUE_FATAL(!strcmp(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_acFileName ,stFileInfo.m_acFileName) , "�ļ�������");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32ProgramNum == stFileInfo.m_u32ProgramNum, "��Ŀ��������");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32AudStreamNum == stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum, "��Ƶ��������");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32SubTitleNum == stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum, "��Ļ��������");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32VideoFormat == stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u32Format, "��Ƶ��ʽ����");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u16Width == stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Width, "��Ƶ��ȴ���");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u16Height == stFileInfo.m_astProgramInfo[0].m_stVidStream.m_u16Height, "��Ƶ�߶ȴ���");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u16Channels == stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u16Channels, "��Ƶ��������");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32SampleRate == stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32SampleRate, "��Ƶ���ʴ���");
		CSTK_ASSERT_TRUE_FATAL(s_sStreamInfo[EM_UDIFILEPLAYER_STREAM_ES].m_u32AudioFormat == stFileInfo.m_astProgramInfo[0].m_astAudStream[0].m_u32Format, "��Ƶ��ʽ����");

	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���ܹ�ֱ��֧�ֱ���ý�岥��");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "�رղ��ž��ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:���Ի�ȡes���ļ�����Ϣ�Ƿ���ȷ���
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
///@INPUT:1����Ч��hPlayer
//@INPUT:2����Ч��stFileStreamId
//@EXPECTATION:��֧��  CSUDIPLAYERGetFileInfo��ȡ��ͨ�ļ���Ϣ
//@REMARK:û���ҵ���ص�������
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����ȡ�ļ�����Ϣ
//@EXECUTIONFLOW:4���Ƚ��ļ���Ϣ�ͻ�ȡ�����ļ���Ϣ�Ƿ���ȣ����ȫ�������֤������ȷ��
//@EXECUTIONFLOW:5������CSUDIPLAYERClose�رղ��ž��
//@EXECUTIONFLOW:6���ָ��ֳ�
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

	CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "�������ž��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "�����Ĳ��ž��Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "��ȡ��Ϣʧ��");

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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "�رղ��ž��ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:���Ի�ȡ��ͨ�ļ�����Ϣ�Ƿ���ȷ���
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
///@INPUT:1����Ч��hPlayer
//@INPUT:2����Ч��stFileStreamId
//@EXPECTATION:��֧��  CSUDIPLAYERGetFileInfo��ȡ��ͨ�ļ���Ϣ
//@REMARK:û���ҵ���ص�������
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����ȡ�ļ�����Ϣ
//@EXECUTIONFLOW:4���Ƚ��ļ���Ϣ�ͻ�ȡ�����ļ���Ϣ�Ƿ���ȣ����ȫ�������֤������ȷ��
//@EXECUTIONFLOW:5������CSUDIPLAYERClose�رղ��ž��
//@EXECUTIONFLOW:6���ָ��ֳ�
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

	CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "�������ž��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "�����Ĳ��ž��Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "��ȡ��Ϣʧ��");

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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "�رղ��ž��ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetFileInfo
//@DESCRIPTION:���Ի�ȡ�����ļ�����Ϣ�Ƿ���ȷ���
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
///@INPUT:1����Ч��hPlayer
//@INPUT:2����Ч��stFileStreamId
//@EXPECTATION:��֧��  CSUDIPLAYERGetFileInfo��ȡ�����ļ���Ϣ
//@REMARK:û���ҵ���ص�������
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����ȡ�ļ�����Ϣ
//@EXECUTIONFLOW:4���Ƚ��ļ���Ϣ�ͻ�ȡ�����ļ���Ϣ�Ƿ���ȣ����ȫ�������֤������ȷ��
//@EXECUTIONFLOW:5������CSUDIPLAYERClose�رղ��ž��
//@EXECUTIONFLOW:6���ָ��ֳ�
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

	CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(NULL, ePlayerType, &hPlayer), "�������ž��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "�����Ĳ��ž��Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, NULL, 0, &stPlaybackParam), "��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "��ȡ��Ϣʧ��");

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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "�رղ��ž��ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetFilePlayStream
//@DESCRIPTION:���Բ����Ƿ�������£��Ƿ񷵻�CSUDIPLAYER_ERROR_BAD_PARAMETER
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����ȡ���ļ�����Ϣ
//@EXECUTIONFLOW:4������CSUDIPLAYERStart����ʼ�����ļ�
//@EXECUTIONFLOW:5������CSUDIPLAYERSetFilePlayStream����һ����������Ϊ�գ���������CSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIPLAYERSetFilePlayStream���ڶ�����������Ϊ�գ���������CSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:7������CSUDIPLAYERSetFilePlayStream����������������Ϊ�գ���������CSUDIPLAYER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����
//@EXECUTIONFLOW:9���ָ��ֳ�
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1���������ž��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1�������Ĳ��ž��Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "����2�������û���Ŀ��Ϣʧ��")

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "����3����ȡ�ļ���Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����4������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetFilePlayStream(CSUDI_NULL, &stFileStreamId), "����5������Ƿ�������Ӧ�÷���CSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetFilePlayStream(hPlayer, CSUDI_NULL), "����6������Ƿ�������Ӧ�÷���CSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetFilePlayStream(CSUDI_NULL, CSUDI_NULL), "����7������Ƿ�������Ӧ�÷���CSUDIPLAYER_ERROR_BAD_PARAMETER");

		CSUDIOSThreadSleep(2*1000);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"����8��ֹͣ����ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���ܹ�ֱ��֧�ֱ���ý�岥��");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����9���رղ��ž��ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetFilePlayStream
//@DESCRIPTION:���Զ���������£��ı���Ƶ��pidֵ��Ƶ�Ƿ�ı�
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
//@INPUT:1����Ч��hPlayer
//@INPUT:2����Ч��stFileStreamId
//@EXPECTATION:��֧��  CSUDIPLAYERSetFilePlayStream���ö���Ƶ
//@REMARK:�����ж���Ƶ������
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����ȡ���ļ�����Ϣ
//@EXECUTIONFLOW:4���Ƚ���Ƶ�ĸ����Ƿ񳬹�������
//@EXECUTIONFLOW:5������CSUDIPLAYERSetFilePlayStream�������ļ��Ĳ�������
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����ʼ�����ļ�
//@EXECUTIONFLOW:7������CSTKWaitYes���ȴ������Ƿ�����Ƶ�л�
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����
//@EXECUTIONFLOW:9������CSUDIPLAYERClose���رղ��ž��
//@EXECUTIONFLOW:10���ָ��ֳ�
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
		CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1ʧ�ܡ��������ž��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1ʧ�ܡ������Ĳ��ž��Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "����2ʧ�ܡ������û���Ŀ��Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "����3ʧ�ܡ���ȡ�ļ���Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum <= CSUDI_PLAYER_MAX_STREAM_NUM, "����4ʧ�ܡ���Ƶ�ĸ�����������������");

		int i = 0;
		for(i=0; i<stFileInfo.m_astProgramInfo[0].m_u32AudStreamNum; i++)
		{
			stFileStreamId.m_u32AStreamId = i;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetFilePlayStream(hPlayer, &stFileStreamId), "����5ʧ�ܡ������ļ��Ĳ�������");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����6ʧ�ܡ���ʼ���Ų�����");

			if(i != 0)
			{
				CSTCPrint("�Ƿ����һ����Ƶ��ͬ\n");
	        	CSTCPrint("Does the player AUDIO are different to the former ?\n");
			}
			else
			{
				CSTCPrint("��Ƶ�Ƿ񲥷�����?\n");
	        	CSTCPrint("Does the player AUDIO has play OK ?\n");
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ��Ƿ�����һ����Ƶ��ͬ");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"����8ʧ�ܡ�ֹͣ����ʧ��");

			CSUDIOSThreadSleep(1000);

		}


	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���ܹ�ֱ��֧�ֱ���ý�岥��");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����9ʧ�ܣ��رղ���ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetFilePlayStream
//@DESCRIPTION:���Զ���Ƶ����£��ı���Ƶ��pidֵ��Ƶ�Ƿ�ı�
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
//@INPUT:1����Ч��hPlayer
//@INPUT:2����Ч��stFileStreamId
//@EXPECTATION:��֧��  CSUDIPLAYERSetFilePlayStream���ö���Ƶ
//@REMARK:�����ж���Ƶ������
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����ȡ���ļ�����Ϣ
//@EXECUTIONFLOW:4���Ƚ���Ƶ�ĸ����Ƿ񳬹�������
//@EXECUTIONFLOW:5������CSUDIPLAYERSetFilePlayStream�������ļ��Ĳ�������
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����ʼ�����ļ�
//@EXECUTIONFLOW:7������CSTKWaitYes���ȴ������Ƿ�����Ƶ�л�
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����
//@EXECUTIONFLOW:9������CSUDIPLAYERClose���رղ��ž��
//@EXECUTIONFLOW:10���ָ��ֳ�
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
		CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1���������ž��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1�������Ĳ��ž��Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "����2�������û���Ŀ��Ϣʧ��")

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "����3����ȡ�ļ���Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(stFileInfo.m_u32ProgramNum <= CSUDI_PLAYER_MAX_PROGRAM_NUM, "����4����Ƶ�ĸ�����������������");

		int i = 0;
		for(i=0; i<stFileInfo.m_u32ProgramNum; i++)
		{
			stFileStreamId.m_u32ProgramId = i;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetFilePlayStream(hPlayer, &stFileStreamId), "����5������Ӧ�÷���");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����6����ʼ���Ų�����");

			if(i != 0)
			{
				CSTCPrint("��Ƶ�Ƿ��л� � \n");
	        	CSTCPrint("Does the player VUDIO are different to the former ?\n");
			}
			else
			{
				CSTCPrint("��Ƶ�ǲ��ųɹ� � \n");
	        	CSTCPrint("Does the player VUDIO has play OK ?\n");
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7����ʼ���Ų�����");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"����8��ֹͣ����ʧ��");

			CSUDIOSThreadSleep(2*1000);
		}
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���ܹ�ֱ��֧�ֱ���ý�岥��");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����9���رղ���ʧ��");
	}

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIPLAYERSetFilePlayStream
//@DESCRIPTION:���Զ���Ļ����£��ı���Ļ��pidֵ��Ƶ�Ƿ�ı�
//@PRECONDITION:PLAYERģ�鱻������ʼ����file��ʽ���ļ�����
//@INPUT:1����Ч��hPlayer
//@INPUT:2����Ч��stFileStreamId
//@EXPECTATION:��֧��  CSUDIPLAYERSetFilePlayStream������Ļ
//@REMARK:�����ж���Ļ������
//@EXECUTIONFLOW:1������CSUDIPLAYEROpen����һ�������ļ����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:3������CSUDIPLAYERGetFileInfo����ȡ���ļ�����Ϣ
//@EXECUTIONFLOW:4���Ƚ���Ļ�ĸ����Ƿ񳬹����������Ƿ�����Ļ
//@EXECUTIONFLOW:5������CSUDIPLAYERSetFilePlayStream�������ļ��Ĳ�������
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����ʼ�����ļ�
//@EXECUTIONFLOW:7������CSTKWaitYes���ȴ������Ƿ�����Ļ�л�
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����
//@EXECUTIONFLOW:9������CSUDIPLAYERClose���رղ��ž��
//@EXECUTIONFLOW:10���ָ��ֳ�
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
		CSTCPrint("���������� \"%s\" �Ĳ���\n", stPlaybackParam.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (NULL,ePlayerType,&hPlayer), "����1���������ž��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1�������Ĳ��ž��Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, NULL, 0, &stPlaybackParam), "����2�������û���Ŀ��Ϣʧ��")

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetFileInfo(hPlayer, &stFileInfo), "����3����ȡ�ļ���Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum <= CSUDI_PLAYER_MAX_LANG_NUM, "����4����Ļ�ĸ�����������������");

		CSTK_ASSERT_TRUE_FATAL(stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum > 0, "����4����Ļ�ĸ���Ϊ�㣬�����Ļ�ļ�");

		int i = 0;
		for(i=0; i<stFileInfo.m_astProgramInfo[0].m_u32SubTitleNum; i++)
		{
			stFileStreamId.m_u32SubTitleId = i;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetFilePlayStream(hPlayer, &stFileStreamId), "����5������Ӧ�÷���");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����6����ʼ���Ų�����");

			if(i != 0)
			{
				CSTCPrint("the language is %s\n", stFileInfo.m_astProgramInfo[0].m_astSubTitle[i].m_acSubTitleName);
				CSTCPrint(" ��Ļ�ı����Ƿ��ǰһ����ͬ\n");
	        	CSTCPrint("Does the player subtitle has differnt to the former ?\n");
			}
			else
			{
				CSTCPrint(" �Ƿ���ʾ��Ļ����\n");
				CSTCPrint("the language is %s\n", stFileInfo.m_astProgramInfo[0].m_astSubTitle[i].m_acSubTitleName);
	        	CSTCPrint("Does the player subtitle has normal output    ?\n");
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7����Ļ����ʧ��!");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"����8��ֹͣ����ʧ��");

			CSUDIOSThreadSleep(1000);
		}
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���ܹ�ֱ��֧�ֱ���ý�岥��");
	}

	CSTK_FATAL_POINT;

	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����9���رղ���ʧ��");
	}

	return CSUDI_TRUE;
}


