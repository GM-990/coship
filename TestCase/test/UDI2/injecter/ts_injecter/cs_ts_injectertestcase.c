/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "udi2_demux.h"
#include "udi2_audio.h"
#include "udi2_video.h"
#include "udi2_fs.h"
#include "udi2_os.h"
#include "udi2_player.h"
#include "udi2_inject.h"
#include "udi2_section.h"
#include "udiplus_os.h"
#include "cs_ts_injectertestcase.h"
#include "cs_testkit.h"
#include "cs_testkit_porting.h"
#define REPEAT_TIMES 1
#define ERRORDATA (2009)
#define WAITINJECTTIME (1000)   
#define INJECTTHREADPRO (100)
#define INJECTTHREADBUF (1024 * 148)
#define INJECT_SEEK_SET (0)
#define INJECT_SEEK_CUR (1)
#define INJECT_SEEK_END (2)
#define INJECT_INVALID_PID (0x1fff)
#define TS_LOOP_TIMES  (5)

typedef enum 
{
    EM_INJECT_TS_MPEG2_MPEG2 = 0, 	/*��������*/
    EM_INJECT_TS_H264_AAC, 		/**/
    EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_1,/*iptv*/
    EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_2,/*iptv*/
    EM_INJECT_TS_CCTV11,    /*EPG_0606_121458.ts 3NVOD-23.556*/
    EM_INJECT_TS_CCTV1,     /*EPG_0606_121458.ts 3ָ��Ƶ����0*/
    EM_INJECT_TS_MPEG2_MPEG2_AUDIO_1,/*EPG_0606_121458.ts 3NVOD-23.555��Ƶpid=0x15b1*/
    EM_INJECT_TS_MPEG2_MPEG2_AUDIO_2,/*EPG_0606_121458.ts 3NVOD-23.555��Ƶpid=0x15b0*/
    EM_INJECT_PCM_001,/*better man.wav*/
    EM_INJECT_TS_MAX_COUNT
}ES_ServiceIndex_E;

#define MAX_PESES_COUNT_IN_TS (EM_UDI_CONTENT_PCR+1) /*��ο�CSUDIContentType_Eö�ٵ����͸���*/

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
}TSStreamInfo_S;

static TSStreamInfo_S g_TSStreamInfo[EM_INJECT_TS_MAX_COUNT] = 
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
    },
    {
        "H.264.ts",/*EM_INJECT_TS_H264_AAC*/
       	{
			{0x1401, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_H264}},   
			{0x1402, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG_AAC}}, 
			{0x1401, EM_UDI_CONTENT_PCR, {0}}, 
			{INJECT_INVALID_PID, 0, {0}},   
			{INJECT_INVALID_PID, 0, {0}},   
       	},
		10*1024,	
		CSUDI_TRUE,
		CSUDI_NULL,
		CSUDI_FALSE,
		CSUDI_NULL,
		CSUDI_FALSE,
    },
    {
        "ASB.ts",/*EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_1*/
      	{
	        {0x31, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},
	        {0x34, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}},  
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
	},  
    {
        "audionotsync.ts",/*EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_2*/
       	{
	        {0x1022, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_H264}},
	        {0x1023, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}},  
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
    },  
    {
        "EPG_0606_121458.ts",/*EM_INJECT_TS_CCTV11*/
        {
            {0x15b8, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},
            {0x15b9, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}}, 
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
    },
    {
        "EPG_0606_121458.ts",/*EM_INJECT_TS_CCTV1*/
        {
            {0x460, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},
            {0x461, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}}, 
            {0x460, EM_UDI_CONTENT_PCR, {0}}, 
            {INJECT_INVALID_PID, 0, {0}},   
            {INJECT_INVALID_PID, 0, {0}}, 
        },
        10*1024,    
        CSUDI_TRUE,
        CSUDI_NULL, 
        CSUDI_FALSE,
        CSUDI_NULL,
        CSUDI_FALSE,
    },
    {
        "EPG_0606_121458.ts",/*EM_INJECT_TS_MPEG2_MPEG2_AUDIO_1*/
        {
            {0x15AE, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},
            {0x15b1, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}}, 
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
    },
    {
        "EPG_0606_121458.ts",/*EM_INJECT_TS_MPEG2_MPEG2_AUDIO_2*/
        {
            {0x15AE, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},
            {0x15b0, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}}, 
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
    },
    {
        "better man.wav",/*EM_INJECT_PCM_001*/
        {
            {INJECT_INVALID_PID, 0, {0}},
            {0, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_PCM}}, 
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

//������Ƶ����Ƶ��������ʼ����Ӧ�Ĳ�����Ϣ
static void initAVInfo(int nAudioId, int nVideoId)
{
    if(nAudioId != -1)
    {
        //CSUDIAUDIOUnmute(nAudioId);
        CSUDIAUDIOSetVolume(nAudioId, 31);
    }
    if(nVideoId != -1)
    {
        CSUDIVIDEOShow(nVideoId, CSUDI_TRUE);
        CSUDIVIDEOSetAspectRatio(nVideoId,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO);
        CSUDIVIDEOSetMatchMethod(nVideoId,EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_MATCH_METHOD_IGNORE);
    }
    return ;
}

//����֧��EM_UDI_DEMUX_INJECT | EM_UDI_DEMUX_PLAY���͵�demx
static int searchDemuxID_Y_InInject(void)
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
				if((sDemuxCapabilityInfo.m_dwWorkTypeMask & EM_UDI_DEMUX_PLAY) != 0 
					&& (sDemuxCapabilityInfo.m_dwWorkTypeMask & EM_UDI_DEMUX_INJECT) != 0)
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

static CSUDI_BOOL TS_iSetStream(CSUDI_BOOL bIsSet,CSUDI_HANDLE hPlayer,ES_ServiceIndex_E eSerIndex)
{
	CSUDIStreamInfo_S  sStreamInfo[MAX_PESES_COUNT_IN_TS];  //���ᳬ��10��
	int nStreamCnt = 0;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	int i = 0;
    TSStreamInfo_S *pTSStreamInfo;
	
	if(eSerIndex >= sizeof(g_TSStreamInfo)/sizeof(TSStreamInfo_S))  return CSUDI_FALSE;
    
	pTSStreamInfo = &g_TSStreamInfo[eSerIndex];
    
	for (i=0;i<MAX_PESES_COUNT_IN_TS;i++)
	{
	    if (pTSStreamInfo->m_TSContentInfo[i].m_nPid == INJECT_INVALID_PID)
		{
			break;
		}
        
		memcpy(&sStreamInfo[i],&(pTSStreamInfo->m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S));
        nStreamCnt++;
	}

	CSTCPrint("���ڲ��� %s...... ��Ŀ\r\n",pTSStreamInfo->m_pcName);
	
	if(bIsSet)
	{
		nRet = CSUDIPLAYERSetStream(hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL);
	}
	else
	{
		nRet = CSUDIPLAYERModifyStream(hPlayer,sStreamInfo,nStreamCnt);
	}

	return (nRet == CSUDI_SUCCESS);
}

///���ݴ����index������Ҫ�����������͵���Ƶ������
//�������������ʾ���Ӧ����Ƶ����Ƶ����ö�����ȡֵ

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
	
	TSStreamInfo_S *pTSStreamInfo = (TSStreamInfo_S*)TSStreamInfo;

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

static void TS_injectSeekTask(void * TSStreamInfo)
{
	void * ppvBuffer = NULL;

	unsigned  int  uLength = 0;
	
	TSStreamInfo_S *pTSStreamInfo = (TSStreamInfo_S*)TSStreamInfo;

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
		pTSStreamInfo->m_bOpenFileSuccess= CSUDI_FALSE; 
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
		if (pTSStreamInfo->m_bSeek)
		{
			CSUDIOSEventSet(pTSStreamInfo->m_hSeekEvent);

			//��ת���ļ���ʼ
			InjectFSSeek(file,0,INJECT_SEEK_SET);

			//�ȵ�SEEK��־ΪFALSE��ʼ������
			while(pTSStreamInfo->m_bSeek)
			{
				CSUDIOSThreadSleep(100);
			}
		}

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
		CSUDIOSThreadSleep(10);
	}

	if(!pTSStreamInfo->m_bRun )
	{
		InjectFSClose(file);
	}
}
static void TS_injectTask_OverLengh(void * ESStreamInfo)
{
	void * ppvBuffer = NULL;

	unsigned  int  uLength = 0;
	
	TSStreamInfo_S *pESStreamInfo = (TSStreamInfo_S*)ESStreamInfo;

	CSUDI_HANDLE hInjecter = pESStreamInfo->m_hInjecter;
	
	int nBlockNum = 0;
	long nFileAllLength = 0;  //�ļ��ܳ���
	int nFileCurrentPos = 0; //�ļ���ǰλ��
	int nFileLeftLength = 0; //ʣ��δ���ļ�����
	int nInjectedLength = 0; //��ע��Ĵ�С
	int nReadLength = 0;
	
	void* file = CSUDI_NULL;
	
	if(!getInjecterFile((const char*)pESStreamInfo->m_pcName, &file, &nFileAllLength))
	{
       CSTCPrint("getInjecterFile failed!\n\r");
       pESStreamInfo->m_bRun = CSUDI_TRUE;
       pESStreamInfo->m_bOpenFileSuccess= CSUDI_FALSE; 
       return;
	}
	
	if (pESStreamInfo->m_uBufferLen<10240||pESStreamInfo->m_uBufferLen>1024*1024)
	{
		pESStreamInfo->m_uBufferLen = 10*1024;
	}
	pESStreamInfo->m_bOpenFileSuccess = CSUDI_TRUE; 
	pESStreamInfo->m_bRun = CSUDI_TRUE;
	
	//pBuffer = CSUDIOSMalloc(pESStreamInfo->m_uBufferLen);
	
	while(pESStreamInfo->m_bRun)
	{
		//�����ǰʣ��δ���ļ��ĳ���
		nFileCurrentPos=InjectFSTell(file);	//��ǰ�Ѷ��ļ��ĳ���
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
				
		if(uLength != 0)
		{
			nReadLength = pESStreamInfo->m_uBufferLen < nFileLeftLength ? pESStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //ʣ���ļ�����С�ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = nReadLength;
			}
			else  //ʣ���ļ����ȴ��ڵ��ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //��ȡ��Ӧ���ȵ��ļ�

			CSUDIINJECTERWriteComplete(hInjecter,uLength+10);
			
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

	if(!pESStreamInfo->m_bRun )
	{
		InjectFSClose(file);
	}
	
}

static int  getTSInjectInfo(ES_ServiceIndex_E  eFileType,CSUDIINJECTERChnl_S  *psInjecterChnl,CSUDIPlayerChnl_S  *psPlayerChnl,CSUDIINJECTEROpenParam_S *pOpenParam ,CSUDIStreamInfo_S  sStreamInfo[])
{
	int nAudioId = -1;
	int nVideoId = -1;
	int nDemuxID = -1;
	int i = 0;
	int nStreamCnt = 0;
	TSStreamInfo_S *pTSStreamInfo = &g_TSStreamInfo[eFileType];

//ע�������Ƶ������
	psInjecterChnl->m_nAudioDecoder=-1;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;	
	psInjecterChnl->m_nDemux=-1;
		
	nDemuxID=searchDemuxID_Y_InInject();
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

    initAVInfo(psPlayerChnl->m_nAudioDecoder,psPlayerChnl->m_nVideoDecoder);
	return nStreamCnt;
}

static CSUDI_BOOL closeInjectPlayer(CSUDI_HANDLE hPlayer)
{
	CSUDI_BOOL bRe = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL !=hPlayer,"��������");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��");
	
	bRe = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bRe;
	
}

static CSUDI_HANDLE OpenTSInectPlayer(CSUDIPlayerChnl_S *pChnl, CSUDIStreamInfo_S *pStreamInfo, int nCount)
{
	CSUDI_HANDLE hPlayer= CSUDI_NULL;
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pChnl, "����1����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pStreamInfo, "����2����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (pChnl, EM_UDIPLAYER_INJECT,&hPlayer), "�򿪲�����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,pStreamInfo,nCount,CSUDI_NULL), "������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "��������ʧ��");

	CSTK_FATAL_POINT;
	
	return hPlayer;
}

static void task_For_TSOpenColse(void *pvData)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	int i= 0;
	ES_ServiceIndex_E eSeviceID = EM_INJECT_TS_MPEG2_MPEG2;
	int nStreamCount= 0;

	eSeviceID = (ES_ServiceIndex_E)(int)pvData;

	if (eSeviceID > EM_INJECT_TS_MAX_COUNT)
	{
		eSeviceID = EM_INJECT_TS_MPEG2_MPEG2;
	}
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));
	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	
	nStreamCount = getTSInjectInfo(eSeviceID,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSUDIOSThreadSleep(10); 
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		hINJECTER = CSUDI_NULL;
		
		CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER);
		
		CSUDIOSThreadSleep(1); 
		
		if (hINJECTER != CSUDI_NULL)
		{
			CSUDIINJECTERClose(hINJECTER);
		}
	}
	
	CSTK_FATAL_POINT;
	
	return ;
}

static void task_For_ESStartStop(void * hInjecterHander )
{
	int i =0;
		
	CSUDI_HANDLE hHandler = CSUDI_NULL;
	
	CSUDIOSThreadSleep(10);
	
	if (hInjecterHander != CSUDI_NULL)
	{
	 	hHandler =hInjecterHander;
		
		for (i=0;i<REPEAT_TIMES;i++)
		{
			CSUDIINJECTERStart (hHandler);
			
			CSUDIOSThreadSleep(1); 
			
			CSUDIINJECTERStop(hHandler);
		}
	}
	
	return ;
}

CSUDI_BOOL CSTC_TS_INJECTER_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_TS_INJECTER_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:����Open/Close TSע����ʱ���ڲ������������Ҫ������µ�ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:����������TSע�����ӿ����Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2��psOpenParams->m_eContentType=EM_UDIINJECTER_CONTENT_DEFAULT,����CSUDIINJECTEROpen��ͼ��һ��TSע����,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��psInjecterChnl->m_nDemux ������-1ʱ������CSUDIINJECTEROpen��ͼ��һ��TSע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��psInjecterChnl->m_nVideoDecoder ����ʱʱ������CSUDIINJECTEROpen��ͼ��һ��TSע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��psInjecterChnl->m_nAudioDecoder ����ʱʱ������CSUDIINJECTEROpen��ͼ��һ��TSע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��phINJECTER = CSUDI_NULL ʱ������CSUDIINJECTEROpen��ͼ��һ��TSע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��psInjecterChnl = CSUDI_NULLʱ������CSUDIINJECTEROpen��ͼ��һ��TSע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8��psOpenParams = CSUDI_NULLʱ������CSUDIINJECTEROpen��ͼ��һ��TSע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERClose��ͼ�ر�һ����Ч��TSע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����2 ������7����1000��
CSUDI_BOOL CSTC_TS_INJECTER_IT_OpenClose_0001(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	CSUDIINJECTERContentType_E e_ContentType = EM_UDIINJECTER_CONTENT_DEFAULT;
	int nDeviceID = -1;
	
	int i=0;
	
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	for (i=0;i<REPEAT_TIMES; i++)
	{
		e_ContentType = sOpenParams.m_eContentType;
		
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_VIDEO;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

		sOpenParams.m_eContentType = e_ContentType;
		
		nDeviceID = sInjecterChnl.m_nDemux;
		sInjecterChnl.m_nDemux=ERRORDATA;		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_DEVICE_ID == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����3ʧ��");
		sInjecterChnl.m_nDemux=nDeviceID;
		
		nDeviceID = sInjecterChnl.m_nVideoDecoder;
		sInjecterChnl.m_nVideoDecoder = ERRORDATA;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����4ʧ��");
		sInjecterChnl.m_nVideoDecoder = nDeviceID;

		nDeviceID = sInjecterChnl.m_nAudioDecoder;
		sInjecterChnl.m_nAudioDecoder = ERRORDATA;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����5ʧ��");
		sInjecterChnl.m_nAudioDecoder = nDeviceID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,CSUDI_NULL), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (CSUDI_NULL,&sOpenParams,&hINJECTER), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,CSUDI_NULL,&hINJECTER), "����8ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(CSUDI_NULL), "����9ʧ��");

	}

	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:�������в��������Ͻӿ����Ҫ�������£�CSUDIINJECTEROpen & CSUDIINJECTERClose��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:���в�����Ӧ�ó��������Ͻӿ����Ҫ��
//@EXPECTATION:����CSUDI_SUCCESS  
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2�Ͳ���3��1000�Ρ�
CSUDI_BOOL CSTC_TS_INJECTER_IT_OpenClose_0002(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
		
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����3ʧ��");
		 hINJECTER = CSUDI_NULL;
	}
	CSTK_FATAL_POINT;

	if ( hINJECTER!= CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:����ES ע����Open��Close ���ɶԵ��ã����Close��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:���������Ͻӿ����Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS  
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen��������һ��TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTEROpen��������һ��TSע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERClose���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����2������5����1000��
CSUDI_BOOL CSTC_TS_INJECTER_IT_OpenClose_0003(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL,  hINJECTER1 = CSUDI_NULL;
		
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "����2ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS !=  CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER1)), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(hINJECTER), "����5ʧ��");
	}
	hINJECTER = CSUDI_NULL;
	
	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:���߳̽������CSUDIINJECTEROpen & CSUDIINJECTERClose�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �Ϸ���
//@EXPECTATION:�����������������߼���ȷ
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2����������һ�����񣬸�����ͬʱ���첽ִ������Ĳ���3��4
//@EXECUTIONFLOW:3������CSUDIINJECTEROpen��������һ��TSע��������������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:4���������3�ɹ��������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����3������4����1000��
//@EXECUTIONFLOW:6���ȴ�����2������������������˳�
//@EXECUTIONFLOW:7�����ٲ���2����������
CSUDI_BOOL CSTC_TS_INJECTER_IT_OpenClose_0004(void)
{
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OpenClose", INJECTTHREADPRO, INJECTTHREADBUF, task_For_TSOpenColse, (void *)EM_INJECT_TS_MPEG2_MPEG2, &hInjectThread), "������ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		udiRe = CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_DEVICE_BUSY == udiRe )),"����3ʧ��");//how is not-support

		CSUDIOSThreadSleep(2);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����4ʧ��");
			hINJECTER = CSUDI_NULL;
		}
	}
		
	CSTK_FATAL_POINT;
	
	if (CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����6ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����7ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:����TSע���������ΪCSUDI_NULLʱ��Start/Stop������ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER = CSUDI_NULL
//@EXPECTATION:Start/Stop����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2��������TSע��ע�������õ�TSע�������
//@EXECUTIONFLOW:3����hINJECTER = CSUDI_NULL,����CSUDIINJECTERStart,��������CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:4����hINJECTER = CSUDI_NULL,����CSUDIINJECTERStop,��������CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:5���ظ�����3������4����1000��
//@EXECUTIONFLOW:6������CSUDIINJECTERClose���رմ�TSע����
CSUDI_BOOL CSTC_TS_INJECTER_IT_StartStop_0001(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
		
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStart(CSUDI_NULL), "����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStop(CSUDI_NULL), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;

	if ( hINJECTER!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����6ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:������������£�CSUDIINJECTERStart��CSUDIINJECTERStop�ɶԵ��õ����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ�����򿪵�TSע���������
//@EXPECTATION:CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2��������TSע��ע�������õ�TSע�������
//@EXECUTIONFLOW:3������CSUDIINJECTERStart,������TSע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:4������CSUDIINJECTERStop,ֹͣ��TSע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:5���ظ�����3������4����1000��
//@EXECUTIONFLOW:6������CSUDIINJECTERClose���رմ�TSע����
CSUDI_BOOL CSTC_TS_INJECTER_IT_StartStop_0002(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
		
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����6ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:������������£�CSUDIINJECTERStart��CSUDIINJECTERStop���ɶԻ�������õ����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ�����򿪵�TSע���������
//@EXPECTATION:���ط�CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2��������TSע��ע�������õ�TSע�������
//@EXECUTIONFLOW:3������CSUDIINJECTERStop,ֹͣ��TSע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:4������CSUDIINJECTERStart,������TSע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:5������CSUDIINJECTERStart,������TSע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:6������CSUDIINJECTERStop,ֹͣ��TSע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:7������CSUDIINJECTERStop,ֹͣ��TSע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:8���ظ�����3������7����1000��
//@EXECUTIONFLOW:9������CSUDIINJECTERClose���رմ�TSע����
CSUDI_BOOL CSTC_TS_INJECTER_IT_StartStop_0003(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
		
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStart(hINJECTER), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "����7ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����9ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:���߳̽������CSUDIINJECTERStart & CSUDIINJECTERStop�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �Ϸ���TSע����
//@EXPECTATION:�����������������߼���ȷ
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen��������һ��TSע�������õ�ע�������
//@EXECUTIONFLOW:3����������һ�����񣬸�����ͬʱ���첽ִ������Ĳ���4��5
//@EXECUTIONFLOW:4������CSUDIINJECTERStart��������TSע��������������CSUDI_SUCCESS��CSUDIREC_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:5������CSUDIINJECTERStop��ֹͣ��TSע��������������CSUDI_SUCCESS��CSUDIREC_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:6���ظ�����6������7����10��
//@EXECUTIONFLOW:7������CSUDIOSThreadJoin���ȴ�����5����������ִ����ɣ��ر��˳�
//@EXECUTIONFLOW:8������CSUDIINJECTERClose���رմ�TSע����
CSUDI_BOOL CSTC_TS_INJECTER_IT_StartStop_0004(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ESStartStop", INJECTTHREADPRO, INJECTTHREADBUF, task_For_ESStartStop, (void *)hINJECTER, &hInjectThread), "����3ʧ��");
	
	for (i=0;i<REPEAT_TIMES; i++)
	{	
		udiRe = CSUDIINJECTERStart(hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STARTED == udiRe )),"����4ʧ��");

		CSUDIOSThreadSleep(2);


		udiRe =  CSUDIINJECTERStop(hINJECTER);
		
		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STOPPED == udiRe )),"����5ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if (CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����7ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����8ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����8ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:������TSע��ģʽ���ӿڲ������������Ҫ�������£�CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute��ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1�������Ͻӿ����Ҫ�� 
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERSetAttribute�������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERSetAttribute����ͬ�����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERSetAttribute ���ûص��������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERSetAttribute ����ֻ��״̬���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERSetAttribute ����ֻ�����������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERSetAttribute ���÷Ƿ����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERGetAttribute��ȡ���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:11��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute ��ȡͬ�����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute��ȡ�ص����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute ��ȡ״̬���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:13��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute ��ȡ���������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERSetAttribute ��ȡ�Ƿ����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:15���ظ�����3~����14��1000��
//@EXECUTIONFLOW:16������CSUDIINJECTERClose���رմ�TSע����
CSUDI_BOOL CSTC_TS_INJECTER_IT_SetGetAttribute_0001(void)
{
	CSUDIINJECTERStatus_S  sStatus;
	CSUDIINJECTERBuffer_S  sBuffer;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
//����				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,CSUDI_NULL),  "����4ʧ��");
				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_STATUS, &sStatus),  "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sBuffer),  "����7ʧ��");

//��ȡ	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,CSUDI_NULL),  "����10ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,CSUDI_NULL),  "����12ʧ��");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,CSUDI_NULL),  "����13ʧ��");		
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:������TSע��ģʽ���ӿڲ������������Ҫ�������£�CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute��ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1�������Ͻӿ����Ҫ�� 
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����ͬ����ʽ���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ���ûص��������ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5������CSUDIINJECTERGetAttribute��ȡͬ����ʽ���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���жϻ�ȡ�õ���ͬ����ʽ����Ϊ����3�����õ�����һ��
//@EXECUTIONFLOW:7������CSUDIINJECTERGetAttribute��ȡ���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERGetAttribute��ȡ״̬���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIINJECTERGetAttribute��ȡ�ص��������ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:10�������һ�����سɹ������жϻ�ȡ�õ��Ļص���������Ϊ����4�����õ�����һ��
//@EXECUTIONFLOW:11���ظ�����3~����10��10��
//@EXECUTIONFLOW:12������CSUDIINJECTERClose���رմ�TSע����
CSUDI_BOOL CSTC_TS_INJECTER_IT_SetGetAttribute_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTERStatus_S  sStatus;
	CSUDIINJECTERBuffer_S  sBuffer;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode =  EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = hINJECTER;//����udi ͷ�ļ���������Ҫ����injecteropen �򿪵ľ��
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
//����			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode),  "����3ʧ��");					

//��ȡ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode),  "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(((sSyncMode.m_eSyncMode == EM_UDIINJECTER_SYNC_AUDIO_FIRST)
			||(sSyncMode.m_hSyncHandle == CSUDI_NULL)),"����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sBuffer),  "����7ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,&sStatus),  "����8ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����12ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:����TSע������start֮���ٵ���CSUDIINJECTERSetAttribute/CSUDIINJECTERGetAttribute�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �ǿ�
//@INPUT:2��eAttrType=EM_UDIINJECTER_TS_ENDIAN  
//@INPUT:3��pvData�ǿ�
//@EXPECTATION:����CSUDIINJECTERSetAttribute    ����CSUDIINJECTER_ERROR_INVALID_STATUS������CSUDIINJECTERGetAttribute����CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK:����ע����ܵ���CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����ͬ�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERStart������TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:6������CSUDIINJECTERSetAttribute����"�ص�����"���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERGetAttribute��ȡ"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���жϻ�ȡ�õ���ͬ������Ϊ����3�����õ�����һ��
//@EXECUTIONFLOW:9������CSUDIINJECTERGetAttribute��ȡ"״̬"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERGetAttribute��ȡ"������"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������CSUDIINJECTERGetAttribute��ȡ"�ص�����"���ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:12���ظ�����6~����11��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERStop��ֹͣ��TSע����
//@EXECUTIONFLOW:14������CSUDIINJECTERClose���رմ�TSע����
CSUDI_BOOL CSTC_TS_INJECTER_IT_SetGetAttribute_0003(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTERStatus_S  sStatus;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	int i= 0;
	
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");

	sSyncMode.m_eSyncMode =  EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
//����
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����5ʧ��");

//��ȡ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����7ʧ��");

		CSTK_ASSERT_TRUE_FATAL(((sSyncMode.m_eSyncMode == EM_UDIINJECTER_SYNC_AUDIO_FIRST)
			||(sSyncMode.m_hSyncHandle == CSUDI_NULL)),"����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,&sStatus), "����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sStatus), "����10ʧ��");
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����13ʧ��");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush 
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERFlush��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��TSע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL��2) Startע����֮ǰ,3) Stopע����֮�����������µ��ô˺��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��������ע��֮ǰ����CSUDIINJECTERFlush,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����hINJECTER == CSUDI_NULL������£�����CSUDIINJECTERFlush���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERStopֹͣ��TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����hINJECTER Ϊ����2�õ���TSע�������ʱ������CSUDIINJECTERFlush���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����6~����9��1000��
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���ر�ע����
//@EXECUTIONFLOW:12���رղ���2�򿪵Ĳ�����
CSUDI_BOOL CSTC_TS_INJECTER_IT_Flush_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
		
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "����4ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����5ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(CSUDI_NULL), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "����9ʧ��");		
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
		hPlayer = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush                
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERFlush��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��TSע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERStart������TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERGetFreeBuffer�������õ�ע�������л������ĵ�ַ�ʹ�С��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERWriteComplete��֪ͨ�ײ�ע��������׼���ã�������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERFlush���ȴ�����ע������ݲ����꣬������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����6~����8��5��
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��TSע����
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���رմ�TSע����
//@EXECUTIONFLOW:12���رղ���2�򿪵Ĳ�����
CSUDI_BOOL CSTC_TS_INJECTER_IT_Flush_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	
	int i= 0;
	int nStreamCount = 0;
	
	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	for (i=0;i<5; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer(hINJECTER, &pvBuffer, &uLength), "����6ʧ��");	

		if (uLength > 0)
		{
			memset(pvBuffer,0x0,uLength);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERWriteComplete (hINJECTER,uLength), "����7ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER), "����8ʧ��");
		}
		else
		{
			CSUDIOSThreadSleep(10);
		}
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����10ʧ��");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
		hPlayer = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERClear 
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERClear��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��TSע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL��2) Startע����֮ǰ,3) Stopע����֮�����������µ��ô˺��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��������ע��֮ǰ����CSUDIINJECTERFlush,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����hINJECTER == CSUDI_NULL������£�����CSUDIINJECTERClear���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERStopֹͣ��TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����hINJECTER Ϊ����2�õ���TSע�������ʱ������CSUDIINJECTERFlush���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����6~����9��1000��
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���ر�ע����
//@EXECUTIONFLOW:12���رղ���2�򿪵Ĳ�����
CSUDI_BOOL CSTC_TS_INJECTER_IT_Clear_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "����4ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����5ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(CSUDI_NULL), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "����9ʧ��");		
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
		hPlayer = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush                
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERFlush��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��TSע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERStart������TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERGetFreeBuffer�������õ�ע�������л������ĵ�ַ�ʹ�С��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERWriteComplete��֪ͨ�ײ�ע��������׼���ã�������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERFlush���ȴ�����ע������ݲ����꣬������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����6~����8��1000��
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��TSע����
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���رմ�TSע����
//@EXECUTIONFLOW:12���رղ���2�򿪵Ĳ�����
CSUDI_BOOL CSTC_TS_INJECTER_IT_Clear_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
		
	int i= 0;
	int nStreamCount = 0;
	
	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	for (i=0;i<REPEAT_TIMES; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer(hINJECTER, &pvBuffer, &uLength), "����6ʧ��");	

		if (uLength > 0)
		{
			memset(pvBuffer,0x0,uLength);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERWriteComplete (hINJECTER,uLength), "����7ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER), "����8ʧ��");
		}
		else
		{
			CSUDIOSThreadSleep(10);
		}
	}
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����10ʧ��");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
		hPlayer = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERGetFreeBuffer
//@DESCRIPTION:���Խӿڲ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERGetFreeBuffer��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��TSע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:1) Start֮ǰ��2)Stop֮��3)ע����ΪCSUDI_NULL��4) ����������ָ��ΪCSUDI_NULL��5)���������Ȳ���ΪCSUDI_NULL������5������£�������Ӧ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5�����в���������������£�����CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7��hINJECTERΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8��ppvBufferΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��puLengthΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��TSע����
//@EXECUTIONFLOW:11�����в���������������£�����CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ظ�����6~����10��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose���رմ�TSע����
//@EXECUTIONFLOW:14���رղ���2�򿪵Ĳ�����
CSUDI_BOOL CSTC_TS_INJECTER_IT_GetFreeBuffer_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
		
	int i= 0;
	int nStreamCount = 0;
	
	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "����5ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERGetFreeBuffer (CSUDI_NULL,&pvBuffer,&uLength), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,CSUDI_NULL), "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer(hINJECTER, CSUDI_NULL, &uLength), "����9ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "����11ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����13ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERWriteComplete
//@DESCRIPTION:���Խӿڲ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERWriteComplete��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��TSע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) Start֮ǰ��2)Stop֮��3)ע����ΪCSUDI_NULL��4) uWrittenSize��ǰ�ڵõ��Ŀ��г��ȴ�����4������£�������Ӧ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERWriteComplete��ͼд��1�ֽ����ݣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������TSע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERGetFreeBuffer�õ����л�������ַ�ͳ���
//@EXECUTIONFLOW:8��hINJECTERΪCSUDI_NULL��uWrittenSizeΪ����7�õ��Ŀ��л��������ȣ�����CSUDIINJECTERWriteComplete����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:9��hINJECTERΪ����2�õ��ľ����uWrittenSizeΪ����7�õ��Ŀ��л��������ȼ�1������CSUDIINJECTERWriteComplete���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��TSע����
//@EXECUTIONFLOW:11��hINJECTERΪ����2�õ��ľ����uWrittenSizeΪ����7�õ��Ŀ��л��������ȣ�����CSUDIINJECTERWriteComplete���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ظ�����6~����11��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose���رմ�TSע����
//@EXECUTIONFLOW:14���رղ���2�򿪵Ĳ�����
CSUDI_BOOL CSTC_TS_INJECTER_IT_WriteComplete_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	int i= 0;
	int nStreamCount = 0;
	
	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");	

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER,1), "����5ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERWriteComplete (CSUDI_NULL,uLength), "����8ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER, uLength+1), "����9ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER, uLength), "����11ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����13ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIINJECTERWriteComplete     
//@CASEGROUP:2��CSUDIINJECTERGetFreeBuffer   
//@DESCRIPTION:����·��CSUDIINJECTERGetFreeBuffer-->ֹͣPlayer-->���»���������-->CSUDIINJECTERWriteComplete���������������״̬
//@PRECONDITION:INJECTERģ�鱻������ʼ����ES Player����������
//@INPUT:1�����в���������
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��TSע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart��������TSע����
//@EXECUTIONFLOW:6������CSUDIINJECTERGetFreeBuffer ��ʹppvBuffer  �ǿգ�puLength �ǿ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStop��ֹͣ������
//@EXECUTIONFLOW:8���޸�ppvBuffer�е�����
//@EXECUTIONFLOW:9������CSUDIINJECTERWriteComplete ,д�����ݣ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERClose���رղ�����
//@EXECUTIONFLOW:11������CSUDIINJECTERStop��ֹͣ��ע����
//@EXECUTIONFLOW:12������CSUDIINJECTERClose���رմ�ע����
CSUDI_BOOL CSTC_TS_INJECTER_IT_WriteComplete_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;	
	
	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;

	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(EM_INJECT_TS_MPEG2_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength),"����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(uLength > 0,"����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");			
	
	memset(pvBuffer,0xFE,uLength);

	CSUDIINJECTERWriteComplete(hINJECTER, uLength);
	 
	CSTK_FATAL_POINT;

	if(CSUDI_NULL !=hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����10ʧ��");
		hPlayer = CSUDI_NULL;
	}
	if(CSUDI_NULL !=hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����11ʧ��");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����12ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_MPEG2_MPEG2 TS�������������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;
	int nStreamCount = 0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"ע���豸ָ�����Ϸ�");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

/*<!-- Modify by 903126,2010-12-24 15:51:35: */
/* Note: �ȴ�ts��ʼע�����ڴ򿪲�����,iptv��Ŀ��Ĳ������̾������*/
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);
 /* Modify end 2010-12-24 15:51:35 --!>*/

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
	CSTCPrint("Test ts inject of MPEG2\n");
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����11-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_MPEG2_MPEG2 TS���������в��š���ͣ�ͻָ���ͣ
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS�����ܿ���TS���ŵ��������š���ͣ��ָ�����
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERPause ����ͣTS����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:12������CSUDIPLAYERResume ���ָ�TS����
//@EXECUTIONFLOW:13���ظ�����9-����12��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;
			
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2  ���������в��š���ͣ�ͻָ���ͣ\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("����Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����10ʧ��");
		
		CSTCPrint("����Ƶ��ͣ��������\n");
		CSTCPrint("Do it pause playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����12ʧ��");
		
	}
	
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����15-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����16ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_MPEG2_MPEG2 TS���������в��š�ֹͣ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS�����ܿ��Ʋ����е�ֹͣ��ָ�
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERStop ��ֹͣTS����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:12������CSUDIPLAYERStart ���ָ�TS����
//@EXECUTIONFLOW:13���ظ�����9-����12��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0003(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;
			
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2   ���������в��š�ֹͣ����\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("����Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");
		
		CSTCPrint("����Ƶֹͣ��������\n");
		CSTCPrint("Do it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����12ʧ��");
		
	}
	
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����15-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����16ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_MPEG2_MPEG2 TS����������ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����ָ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  CSUDIINJECTERFlush�ȴ���ע������ݲ�����ȫ���ܷ���
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIINJECTERFlush ���ȴ���ע�������ȫ�������꣬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0004(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;
			
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 ����������ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"����11ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����12-1ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����13ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:"����ע��EM_INJECT_TS_MPEG2_MPEG2 ����������ע�����������CSUDIINJECTERClear�����ע�뵫δ�����������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  CSUDIINJECTERClear����������
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIINJECTERClear �������ע������ݣ���������Ƶ����ֹͣ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0005(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 ����������ע�����������CSUDIINJECTERClear�����ע�뵫δ�����������\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"����11ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����12-1ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����13ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_H264_AAC TS�������������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��H264+AAC����Ƶ
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0006(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_H264_AAC;

	if (searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG_AAC) == -1 || searchVideoID_Y_InInject(EM_UDI_VID_STREAM_H264) == -1 )
	{
		CSTCPrint("����ע���EM_INJECT_TS_H264_AAC ������ʽ����֧��\n");
		return CSUDI_TRUE;
	}

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_H264_AAC �������������ŵ����\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����11-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_H264_AAC TS���������в��š���ͣ�ͻָ���ͣ
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��EM_INJECT_TS_H264_AAC�����ܿ����������š���ͣ��ָ�����
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����ƵƵ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERPause ����ͣTS����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:12������CSUDIPLAYERResume ���ָ�TS����
//@EXECUTIONFLOW:13���ظ�����9-����12��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0007(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_H264_AAC;

	if (searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG_AAC) == -1 || searchVideoID_Y_InInject(EM_UDI_VID_STREAM_H264) == -1 )
	{
		CSTCPrint("����ע���EM_INJECT_TS_H264_AAC ������ʽ����֧��\n");
		return CSUDI_TRUE;
	}
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_H264_AAC ���������в��š���ͣ�ͻָ���ͣ\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream ,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("����Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����10ʧ��");
		
		CSTCPrint("����Ƶ��ͣ��������\n");
		CSTCPrint("Does it pause play ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����12ʧ��");
		
	}
	
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����15-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����16ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_H264_AAC TS���������в��š�ֹͣ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ������Ƶ�����ܿ��Ʋ����е�ֹͣ��ָ�
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERStop ��ֹͣTS����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:12������CSUDIPLAYERStart ���ָ�TS����
//@EXECUTIONFLOW:13���ظ�����9-����12��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0008(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_H264_AAC;

	if (searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG_AAC) == -1 || searchVideoID_Y_InInject(EM_UDI_VID_STREAM_H264) == -1 )
	{
		CSTCPrint("����ע���EM_INJECT_TS_H264_AAC ������ʽ����֧��\n");
		return CSUDI_TRUE;
	}
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	CSTCPrint("����ע��EM_INJECT_TS_H264_AAC ���������в��š�ֹͣ����\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("����Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");
		
		CSTCPrint("����Ƶֹͣ��������\n");
		CSTCPrint("Does it stop play ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����12ʧ��");
		
	}
	
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����15-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����16ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_H264_AAC TS����������ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����ָ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  CSUDIINJECTERFlush�ȴ���ע������ݲ�����ȫ���ܷ���
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIINJECTERFlush ���ȴ���ע�������ȫ�������꣬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0009(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_H264_AAC;

	if (searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG_AAC) == -1 || searchVideoID_Y_InInject(EM_UDI_VID_STREAM_H264) == -1 )
	{
		CSTCPrint("����ע���EM_INJECT_TS_H264_AAC ������ʽ����֧��\n");
		return CSUDI_TRUE;
	}
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_H264_AAC ����������ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"����11ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����12-1ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����13ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_H264_AAC ����������ע�����������CSUDIINJECTERClear�����ע������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  CSUDIINJECTERClear����������
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIINJECTERClear �������ע������ݣ���������Ƶ����ֹͣ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0010(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_H264_AAC;

	if (searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG_AAC) == -1 || searchVideoID_Y_InInject(EM_UDI_VID_STREAM_H264) == -1 )
	{
		CSTCPrint("����ע���EM_INJECT_TS_H264_AAC ������ʽ����֧��\n");
		return CSUDI_TRUE;
	}
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTCPrint("����ע��EM_INJECT_TS_H264_AAC ����������ע�����������CSUDIINJECTERClear�����ע������\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"����11ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����12-1ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����13ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_AUDIO_MPEG2 TS���������������򿪵�������һ���������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ���ܲ��ų�ָ������Ƶ
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ��Ŀ���ݲ��ܲ��ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0011(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;

	if (searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG_AAC) == -1 || searchVideoID_Y_InInject(EM_UDI_VID_STREAM_H264) == -1 )
	{
		CSTCPrint("����ע���EM_INJECT_TS_H264_AAC ������ʽ����֧��\n");
		return CSUDI_TRUE;
	}
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	CSTCPrint("����ע��EM_INJECT_TS_AUDIO_MPEG2 TS���������������򿪵�������һ���������\n");

	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");

	eServiceStream = EM_INJECT_TS_H264_AAC;
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��ȷ������Ƶ��Ŀ���ݲ��ܲ��ų���\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

    #if 0 /*<!-- lili 2010-3-23 12:11:03 */
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��
    #else
	g_TSStreamInfo[EM_INJECT_TS_MPEG2_MPEG2].m_bRun = CSUDI_FALSE; //ֹͣע��
    #endif /*0*//* lili 2010-3-23 12:11:03 --!>*/

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����11-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע������ݴ���ͨ��CSUDIINJECTERGetFreeBuffer�õ��Ļ�������С
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ���ܲ��ų�ָ��TS��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ�����Ƶ��Ŀ���ݲ����ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0012(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask_OverLengh, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 ������ע������ݴ���ͨ��CSUDIINJECTERGetFreeBuffer�õ��Ļ�������С\n");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��Ŀ���ݲ��ܲ��ų�����\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����11-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_MPEG2_MPEG2 TS����(������Ϊiptv�鲥��ʱ��������������)���������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:   1������ע����
//@INPUT:   2������������
//@INPUT:   3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0013(void)
{
    CSUDIPESSYNCMode_S  sSyncMode;
    
    CSUDIINJECTEROpenParam_S  sOpenParams;
    CSUDIINJECTERChnl_S sInjecterChnl;
    CSUDIPlayerChnl_S sPlayerChnl;
    CSUDIStreamInfo_S sStreamInfo[10];
    
    CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

    CSUDI_HANDLE hPlayer = CSUDI_NULL;
    
    CSUDI_HANDLE hInjectThread = CSUDI_NULL;
    
    ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_1;
    int nStreamCount = 0;
    
    memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
    memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));    
    memset(sStreamInfo,0,sizeof(sStreamInfo));
    memset(&sOpenParams,0,sizeof(sOpenParams));
    
    CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
    
    nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"ע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

    sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
    sSyncMode.m_hSyncHandle = CSUDI_NULL;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

    g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

/*<!-- Modify by 903126,2010-12-24 15:51:35: */
/* Note: �ȴ�ts��ʼע�����ڴ򿪲�����,iptv��Ŀ��Ĳ������̾������*/
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);
 /* Modify end 2010-12-24 15:51:35 --!>*/

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");
 
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

    CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
    CSTCPrint("Test ts inject of MPEG2\n");
    CSUDIOSThreadSleep(2000);
    
    CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��"); 

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����11-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");
		hINJECTER = CSUDI_NULL;
	}

    return CSUDI_TRUE;
}
//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_MPEG2_MPEG2 TS����(������Ϊiptv�鲥��ʱ��������������)���������ŵ����
//@DESCRIPTION:ͬʱ���Կ�����˲����;�������֮�����ϲ���
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�������ģ��seek����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���?
//@EXECUTIONFLOW:10������CSUDIAUDIOMute���о����������������������
//@EXECUTIONFLOW:11��֪ͨע������ʼ����ģ��seek����(�����ļ�ͷ��ʼע��)
//@EXECUTIONFLOW:12����ʾ�û�ȷ����Ƶ�Ƿ��ͷ��ʼ����
//@EXECUTIONFLOW:13������CSUDIAUDIOUnmute����ȡ�������������������������
//@EXECUTIONFLOW:14������CSUDIOSThreadJoin ���ȴ�ע���߳̽�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0014(void)
{
    CSUDIPESSYNCMode_S  sSyncMode;
    
    CSUDIINJECTEROpenParam_S  sOpenParams;
    CSUDIINJECTERChnl_S sInjecterChnl;
    CSUDIPlayerChnl_S sPlayerChnl;
    CSUDIStreamInfo_S sStreamInfo[10];
    
    CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

    CSUDI_HANDLE hPlayer = CSUDI_NULL;
    
    CSUDI_HANDLE hInjectThread = CSUDI_NULL;
    
    ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_1;
    int nStreamCount = 0;
    
    memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
    memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));    
    memset(sStreamInfo,0,sizeof(sStreamInfo));
    memset(&sOpenParams,0,sizeof(sOpenParams));
    
    CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
    
    nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"ע���豸ָ�����Ϸ�");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

    sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
    sSyncMode.m_hSyncHandle = CSUDI_NULL;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

    g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("Seek Event", CSUDIOS_EVENT_AUTO_RESET, &(g_TSStreamInfo[eServiceStream].m_hSeekEvent)),  "����5ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectSeekTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

/*<!-- Modify by 903126,2010-12-24 15:51:35: */
/* Note: �ȴ�ts��ʼע�����ڴ򿪲�����,iptv��Ŀ��Ĳ������̾������*/
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);
 /* Modify end 2010-12-24 15:51:35 --!>*/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

    CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
    CSTCPrint("Test ts inject of MPEG2\n");
    CSUDIOSThreadSleep(2000);
    
    CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	CSUDIAUDIOMute(sPlayerChnl.m_nAudioDecoder);
	CSTCPrint("����,������������� \n");
    CSTCPrint("Do it mute playing ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");
    
    g_TSStreamInfo[eServiceStream].m_bSeek = CSUDI_TRUE; //׼��SEEK
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSEventWait(g_TSStreamInfo[eServiceStream].m_hSeekEvent, CSUDIOS_TIMEOUT_INFINITY),"����11ʧ��");
    CSUDIINJECTERClear(hINJECTER);
    CSUDIPLAYERClear(hPlayer);     
    g_TSStreamInfo[eServiceStream].m_bSeek = CSUDI_FALSE;

    CSTCPrint("����ע��SEEK �������������ŵ����\n");
    CSTCPrint("Test ts inject of MPEG2\n");
    CSUDIOSThreadSleep(2000);
    
    CSTCPrint("��Ƶ�Ƿ��ͷ��ʼ�������ţ�\n");
    CSTCPrint("Is it replaying normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");

	CSUDIAUDIOUnmute(sPlayerChnl.m_nAudioDecoder);
	CSTCPrint("unmute audio \n");
    CSTCPrint("����Ƶ���������������Ѿ�����unmute�������������������\n");
    CSTCPrint("Is it replaying normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ��");

    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��"); 

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����15-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����16ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
		hINJECTER = CSUDI_NULL;
	}

    return CSUDI_TRUE;
}
//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_MPEG2_MPEG2 TS����(������Ϊiptv�鲥��ʱ��������Ƶ��ͬ��)���������ŵ����
//@DESCRIPTION:ͬʱ������ͣ���źͻָ������;�������֮�����ϲ���
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0015(void)
{
	CSUDIPESSYNCMode_S	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	int nStreamCount = 0;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_2;//VID:h264
	
	if (searchVideoID_Y_InInject(EM_UDI_VID_STREAM_H264) == -1 )
	{
		CSTCPrint("����ע���EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_2 ������ʽ����֧��\n");
		return CSUDI_TRUE;
	}
			
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2  ���������в��š���ͣ�ͻָ���ͣ\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");
	
	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");
	
	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("����Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����10ʧ��");
		
		CSTCPrint("����Ƶ��ͣ��������\n");
		CSTCPrint("Do it pause playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����12ʧ��");
		CSTCPrint("%d times to test \n",(5-i-1));
	}
	//add for testing mute&unmute interacts with pause&resume
	CSUDIAUDIOMute(sPlayerChnl.m_nAudioDecoder);
	CSTCPrint("mute audio \n");
	for (i=0;i<5;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����10ʧ��");
		
		CSTCPrint("����Ƶ��ͣ��������\n");
		CSTCPrint("Do it pause playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����12ʧ��");
		CSTCPrint("����Ƶ�ָ����ţ�������û����������?\n");
		CSTCPrint("resume playing but expect no audio output \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		CSTCPrint("%d times to test \n",(5-i-1));
	}
	CSUDIAUDIOUnmute(sPlayerChnl.m_nAudioDecoder);
	CSTCPrint("unmute audio \n");
	
	CSTCPrint("����Ƶ���ų�������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��"); 
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����15-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����16ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;

}
//���·�װֱ�Ӵ�sectionģ����ֲ������Ŀ����Ϊ�� ������������ģ��
typedef enum {
    EM_TS_SECTION_FILTER_PAT_001,
    EM_TS_SECTION_FILTER_MAX
}TS_SECTION_Filter_E;

#define TS_PAT_PID (0)
static CSUDIFILTERMatchMask_S g_sMMFilter[EM_TS_SECTION_FILTER_MAX] = { \
	{
		{0x00},
		{0xff},
		{0x00}
	}  /*PAT_FILTER001*/	
};
static int	s_nDMXFilterIndex[EM_TS_SECTION_FILTER_MAX];
static CSUDI_BOOL	s_bSectionCallbackSuc[EM_TS_SECTION_FILTER_MAX];//���뵽���ݲ���ͨ����֤
static CSUDI_BOOL	s_bSectionCallbackRuq[EM_TS_SECTION_FILTER_MAX];//ֻ�����뵽����
/*
*���ûص�����״ֵ̬����Ҫ������filter֮ǰ����
*/
static void TS_iSetCallbackStatus( int nFilterIndex, CSUDI_BOOL bSuc ,CSUDI_BOOL bReq )
{

	if(nFilterIndex >= EM_TS_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserData < EM_SECTION_FILTER_MAX\n");
		 return ;
	}

	s_bSectionCallbackSuc[nFilterIndex] = bSuc;
	s_bSectionCallbackRuq[nFilterIndex] = bReq;
	
	return;
}

static CSUDI_BOOL TS_iSetSectionFilter(CSUDISECTIONRequestInfo_S * psReqInfo,int nFilterIndex ,int nDemuxIndex,int nPid)
{
	if(nFilterIndex >= EM_TS_SECTION_FILTER_MAX) 
	{
		return CSUDI_FALSE;
	}
	
	if(psReqInfo != CSUDI_NULL)
	{
		memset(psReqInfo,0,sizeof(CSUDISECTIONRequestInfo_S));
		
		psReqInfo->m_eReqType = EM_UDI_REQ_SECTION_DATA;
		psReqInfo->m_nDemuxID = nDemuxIndex;
		psReqInfo->m_nPID = nPid;
		
		memcpy(&psReqInfo->m_sMMFilter,&g_sMMFilter[nFilterIndex],sizeof(psReqInfo->m_sMMFilter));
		
		psReqInfo->m_bCRCCheck = CSUDI_TRUE;
	}
	
	s_nDMXFilterIndex[nFilterIndex] = nFilterIndex;
	
	return CSUDI_TRUE; 
}
static void TS_iCallback_1( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	int nFilterIndex = 0; //Ĭ����PAT_FILTER_001

	if(s_bSectionCallbackRuq[nFilterIndex]) //����Ѿ����������򷵻أ�����ȥ�ظ�У��
	{
		return;
	}
	
   	if(pSection == CSUDI_NULL || (int)pvUserData >= EM_TS_SECTION_FILTER_MAX)
	{
		return ;
	}

	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	CSTCPrint("pvUserData========%d\n",(int)pvUserData);

	nLength  = pSection->m_nDataLen;
	
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			
			CSTCPrint( " FILTER%03d  data callback length = %d*****\n", nFilterIndex+1,nLength);
			
			TS_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_TRUE);  //���ûص��������ݵ���ʱû��У��
			break;
	case EM_UDI_REQ_PES_DATA:
		break;
		
	default:
		break;

	}
	return;
}
static CSUDI_BOOL TS_iGetCallbackStatus( CSUDI_BOOL bCheck, const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData, CSUDI_BOOL bDebug)
{
	int i = 0;
	int nWaitTime = 2; // 2s
	CSUDI_BOOL bRet = CSUDI_FALSE;
   	 int nFilterIndex;

   	if(psRequestInfo == CSUDI_NULL || (int)pvUserData >= EM_TS_SECTION_FILTER_MAX)
	{
		return CSUDI_FALSE;
	}
	
    nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
    
	if(nFilterIndex >= EM_TS_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserDataӦ�ò�����EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE ;
	}

	for(i=0;i < nWaitTime;i++) //�ȴ�2s��ȡPAT��ȡ
	{
	    CSUDIOSThreadSleep(1000); 
        
        if( bCheck ) //��ҪУ������
		{	  
			if( s_bSectionCallbackSuc[nFilterIndex])
			{
				bRet = CSUDI_TRUE;
			}
		}
		else   //����ҪУ������
		{
			if( s_bSectionCallbackRuq[nFilterIndex])
			{
				bRet = CSUDI_TRUE;
			}
		}

		if(bRet)
		{
			return bRet;
		}
	}
	
	if(!bDebug) 
	{
		return CSUDI_FALSE;
	}
	if(bCheck == CSUDI_TRUE && s_bSectionCallbackRuq[nFilterIndex] == CSUDI_TRUE)
	{
		CSTCPrint("[SECTION]FILTER%03d���뵽����,��δͨ��������֤������������ļ��Ƿ���ȷ\r\n", nFilterIndex+1);
		CSTCPrint("[SECTION] ��ǰFILTER����:\r\n");
		CSTCPrint("[SECTION]FILTER%03d Receives data,but checking failure!\r\n", nFilterIndex+1);
		CSTCPrint("[SECTION] Current FILTER context:\r\n");
		CSTCPrint("[SECTION] bCRCCheck %d\r\n",psRequestInfo->m_bCRCCheck);
		CSTCPrint("[SECTION] dwDemuxID %d\r\n",psRequestInfo->m_nDemuxID);
		CSTCPrint("[SECTION] dwPID 0x%x\r\n",psRequestInfo->m_nPID);
		
		CSTCPrint("[SECTION] match: ");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.match[i]);
		}
		
		CSTCPrint("\r\n[SECTION]mask:  ");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.mask[i]);
		}
		
		CSTCPrint("\r\n[SECTION]negate:");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.negate[i]);
		}
	}

	if( s_bSectionCallbackRuq[nFilterIndex] == CSUDI_FALSE && bCheck == CSUDI_TRUE)
	{
		CSTCPrint("\r\n[SECTION]Filter %03d δ���뵽����\r\n",nFilterIndex+1);
		CSTCPrint("\r\n[SECTION]Filter %03d not Receive data\r\n",nFilterIndex+1);
	}
	return CSUDI_FALSE;	
}
/*
* ����һ��filter�������ûص�����״ֵ̬
* ��Ҫ���ڲ��Թ�עfilter��ȡ������ֵ���������������CSTC_SECTION_IT_Allocate_0003
*/
static CSUDI_BOOL TS_iAllocFilter(const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData,CSUDI_HANDLE *phFilter)
{
	int nFilterIndex = 0xff;
	
	if((int)pvUserData >= EM_TS_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserDataӦ�ò�����EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE;
	}
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	
	TS_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);
	
	return (CSUDI_SUCCESS == CSUDIFILTERAllocate(psRequestInfo,TS_iCallback_1,pvUserData,phFilter));

}

static CSUDI_BOOL TS_iStartFilter(CSUDI_HANDLE hFilter,const void * pvUserData)
{
	int nFilterIndex = 0xff;
	CSUDI_Error_Code nRet = CSUDI_FALSE;
	if((int)pvUserData >= EM_TS_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserData < EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE;
	}
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	
	TS_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);	
	
	nRet = CSUDIFILTERStart(hFilter);
	CSTCPrint("SECTION_iStartFilter %03d.....\n",nFilterIndex+1);
	
	if(CSUDI_SUCCESS != nRet)
	{
		CSTCPrint("SECTION_iStartFilter Failure !!\n");
	}
	return (CSUDI_SUCCESS == nRet);
}

static CSUDI_BOOL TS_iFreeFilter(CSUDI_HANDLE hFilter,const void* pvUserData)
{
	int nFilterIndex = 0xff;
	CSUDI_Error_Code nRet = CSUDI_FALSE;
	
	if((int)pvUserData >= EM_TS_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserDataӦ�ò�����EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE;
	}
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	CSTCPrint("SECTION_iFreeFilter %03d.....\n",nFilterIndex+1);
	nRet = CSUDIFILTERFree(hFilter);
	
	CSUDIOSThreadSleep(500); 
	
	TS_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);
	
	return (CSUDI_SUCCESS == nRet);
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��demux����PSI֮�����ע�벥�ŵĹ���
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@INPUT:PAT_FILTER001: �ɻ�ȡ���ݣ����Ϊ1
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00
//@INPUT: MASK	:0xff
//@INPUT: NEGATE:0x00
//@INPUT:------------------------------
//@EXPECTATION:  �����������ݲ����ų�ָ��TS��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIDEMUXGetCount��ȡdemux����,������Ϊ0
//@EXECUTIONFLOW:3������CSUDIDEMUXGetCapability��ȡ֧������ע���demuxID����������
//@EXECUTIONFLOW:4��ʹ��demuxID����CSUDIINJECTEROpen������һ��ע��TS������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:6������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFILTERAllocate��������PAT���filter1����������ɹ�
//@EXECUTIONFLOW:8������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ��������
//@EXECUTIONFLOW:9��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW:10��ֹͣfilter1�����󣬲��ͷ�filter
//@EXECUTIONFLOW:11������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:14����ʾ�û�ȷ�ϣ��Ƿ�����Ƶ��Ŀ�ܲ��ų���?
//@EXECUTIONFLOW:15��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:20������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0016(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;

	CSUDIPESSYNCMode_S	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
    
	int nStreamCount = 0;

	int nDemuxCount = 0;
	int nDemuxIndex = 0;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount),
			"Step 1 get demux count failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(0 != nDemuxCount),
			"Step 2 demux count should not be 0 Failure !!!"
		);
	
	for(nDemuxIndex = 0; nDemuxIndex < nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sTmpCap),
				"Step 2 Get demux capability failure !!!"
			);
		
		if((sTmpCap.m_dwWorkTypeMask & EM_UDI_DEMUX_INJECT) == EM_UDI_DEMUX_INJECT && sTmpCap.m_nFilterNum > 0) //֧������ע��
		{
			break;
		}
	}
	
	if(nDemuxIndex == nDemuxCount)
	{
			CSTCPrint("�Ҳ�����inject������demux�豸����֧��inject���ܵ�demux�豸�ϲ�����filter�豸����ƽ̨��֧�ָò�������!!!");
			return CSUDI_TRUE;
	}
	
	CSTCPrint("Demux Id = %d support Ts inject\n",nDemuxIndex);

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&& \
							(nDemuxIndex==sInjecterChnl.m_nDemux)&& \
							(sInjecterChnl.m_nAudioDecoder==-1)&& \
							(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1)),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");
	
	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

	do{		
    		TS_iSetSectionFilter(&sTmpReqInfo,EM_TS_SECTION_FILTER_PAT_001,nDemuxIndex,TS_PAT_PID);	
    		CSTK_ASSERT_TRUE_FATAL(
    				(CSUDI_TRUE == TS_iAllocFilter(&sTmpReqInfo,(void*)EM_TS_SECTION_FILTER_PAT_001,&hFilter)),
    				"Step 6 Allocate filter1 Failure !!!"
    			);
    		CSTK_ASSERT_TRUE_FATAL(
    				(CSUDI_NULL != hFilter),
    				"Step 6 Allocate filter1 failure !!!"
    			);
    		CSTK_ASSERT_TRUE_FATAL(
    				(CSUDI_TRUE == TS_iStartFilter(hFilter,(void*)EM_TS_SECTION_FILTER_PAT_001)),
    				"Step 8 Start Filter1 Failure !!!"
    			);
    		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
    				(CSUDI_TRUE == TS_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_TS_SECTION_FILTER_PAT_001,CSUDI_TRUE)),
    				"Step 9 Check Expected Data Failure !!!"
    			);
    		//free filter1
    		{
    			CSTK_ASSERT_TRUE_FATAL(
    					(CSUDI_TRUE == TS_iFreeFilter(hFilter,(void*)EM_TS_SECTION_FILTER_PAT_001)),
    					"Step 10  Free Filter1 Failure !!!"
    				);
    			hFilter = CSUDI_NULL;
    		}
	}while(0);    

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����11ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����12ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����13ʧ��");

    CSUDIOSThreadSleep(2000);

    CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ��");

    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����15ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����16-1ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����17ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����18ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����19ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����20ʧ��");
		hINJECTER = CSUDI_NULL;
	}

    return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_CCTV1 TS�����������л�Ƶ�����ŵ����(����CSUDIPLAYERModifyStream�ӿ�)
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�����Ƶ��Ŀ�ܲ��ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERModifyStream�����ý�Ŀ��ϢΪEM_INJECT_TS_MPEG2_MPEG2
//@EXECUTIONFLOW:11���ȴ�3s�У�Ȼ�������ý�Ŀ��ϢΪEM_INJECT_TS_CCTV1
//@EXECUTIONFLOW:12��������Ŀ����ȷ����
//@EXECUTIONFLOW:13���ظ�10��12���費С��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0017(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_CCTV11;
	int nStreamCount = 0;
	int i = 0;
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"ע���豸ָ�����Ϸ�");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

/*<!-- Modify by 903126,2010-12-24 15:51:35: */
/* Note: �ȴ�ts��ʼע�����ڴ򿪲�����,iptv��Ŀ��Ĳ������̾������*/
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);
 /* Modify end 2010-12-24 15:51:35 --!>*/

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
	CSTCPrint("Test ts inject of MPEG2\n");
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");	
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==TS_iSetStream(CSUDI_FALSE,hPlayer,EM_INJECT_TS_MPEG2_MPEG2),"����10ʧ��");
    
    CSTCPrint("�л���Ŀ������Ƶ��������������\n");
	CSTCPrint("After switching program,Is it still playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

    for(i=0;i<TS_LOOP_TIMES;i++)
    {
        if((i%2) == 0)
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==TS_iSetStream(CSUDI_FALSE,hPlayer,EM_INJECT_TS_CCTV11),"����10ʧ��");
        }
        else
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==TS_iSetStream(CSUDI_FALSE,hPlayer,EM_INJECT_TS_MPEG2_MPEG2),"����11ʧ��");
        }
        
        CSUDIOSThreadSleep(3000);
    }
    CSTCPrint("�л���Ŀ������Ƶ��������������\n");
	CSTCPrint("After switching program,Is it still playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");


	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��


	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����15-1ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����16ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}
static unsigned int s_nStartPlayTime=0;
static CSUDI_BOOL s_bShow = CSUDI_FALSE;
static void TS_PLAYERCallback(CSUDI_HANDLE hPlayer,CSUDIPlayerEventType_E eEvent,void * pvUserData)
{
    if(eEvent == EM_UDIPLAYER_VIDEO_FRAME_COMING)
    {
        if(!s_bShow)
        {
            CSTCPrint("Ts playing switch program time:%d(ms)\n",(CSUDIPlusOSGetTickCount()-s_nStartPlayTime)); 
            CSUDIVIDEOShow(0,CSUDI_TRUE);
        }
        s_bShow = CSUDI_TRUE;
    }
}
//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_TS_CCTV1 TS�����������л�Ƶ�����ŵ����(ֹͣ�������ŵķ�ʽ)
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9��������5000ms�����յ���֡�����¼�
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16���ظ�����1��15������5��,����ÿһ�����ɹ�
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0018(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int nStream = 0;
	int nStreamCount = 0;
    
	int i = 0;
	int nWaiteTimes = 0;

	if (searchVideoID_Y_InInject(EM_UDI_VID_STREAM_H264) == -1 )
	{
		CSTCPrint("����ע���EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_2 ������ʽ����֧��\n");
		return CSUDI_TRUE;
	}

    for(i= 0;i< TS_LOOP_TIMES;i++)
    {
        for(nStream = EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_1;nStream<=EM_INJECT_TS_MPEG2_MPEG2_AUDIO_2;nStream++)
        {
        	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
        	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
        	memset(sStreamInfo,0,sizeof(sStreamInfo));
        	memset(&sOpenParams,0,sizeof(sOpenParams));

        	nStreamCount = getTSInjectInfo(nStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_FALSE),"CSUDIVIDEOShow ʧ��");
            
            CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
            CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"ע���豸ָ�����Ϸ�");
        	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

        	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
        	sSyncMode.m_hSyncHandle = CSUDI_NULL;
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

        	g_TSStreamInfo[nStream].m_hInjecter = hINJECTER;
        	g_TSStreamInfo[nStream].m_bRun = CSUDI_FALSE;
        	
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[nStream], &hInjectThread), "����5ʧ��");

            while(!g_TSStreamInfo[nStream].m_bRun) CSUDIOSThreadSleep(500);

            CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[nStream].m_bOpenFileSuccess, "�ļ���ʧ��");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
            s_bShow = CSUDI_FALSE;
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback (hPlayer,TS_PLAYERCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING,NULL), "����6ʧ��");
        	
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");

            s_nStartPlayTime = CSUDIPlusOSGetTickCount();
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

			nWaiteTimes = 0;
            while(!s_bShow && nWaiteTimes<10)
            {
                CSUDIOSThreadSleep(500);
                nWaiteTimes ++;
            }

			CSTK_ASSERT_TRUE_FATAL(nWaiteTimes<10, "5000ms��δ�յ���֡�����¼�");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hPlayer,TS_PLAYERCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING,NULL), "����8ʧ��");
        	CSTCPrint("����ע��%s �������������ŵ����\n",g_TSStreamInfo[nStream].m_pcName);
        	CSTCPrint("Test ts inject of %s\n",g_TSStreamInfo[nStream].m_pcName);
        	CSUDIOSThreadSleep(3000);
        	
        	g_TSStreamInfo[nStream].m_bRun = CSUDI_FALSE; //ֹͣע��

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11ʧ��");
            hInjectThread = CSUDI_NULL;

            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����12ʧ��");    
            
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����13ʧ��");
            hINJECTER = CSUDI_NULL;
            
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����14ʧ��");   
            
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����15ʧ��");
            hPlayer = CSUDI_NULL;
        }
    }

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[nStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����11-1ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����11-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}
//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����tsע����PCMע���л����ŵ��龰
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��TS����pcm��
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTEROpen������һ��ע��PCM��ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERSetAttribute ������pcm����
//@EXECUTIONFLOW:18������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������һ���߳�,ѭ����ȡpcm�ļ���������PCMע�����
//@EXECUTIONFLOW:20������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:21������CSUDIPLAYERSetStream �����ú�PCM��Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:22������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ���������PCM
//@EXECUTIONFLOW:23����ʾ�û�ȷ�ϣ��Ƿ�����Ƶ���ų���
//@EXECUTIONFLOW:24��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:25������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:26������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:27������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:28������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:29������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TS_INJECTER_IT_INJECTER_0019(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;
	int nStreamCount = 0;

	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;
    int i = 0;
    for(i = 0;i<TS_LOOP_TIMES;i++)
    {
        eServiceStream = EM_INJECT_TS_MPEG2_MPEG2;
    	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
    	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
    	memset(sStreamInfo,0,sizeof(sStreamInfo));
    	memset(&sOpenParams,0,sizeof(sOpenParams));
    	
    	CSTCPrint("����ע��EM_INJECT_TS_MPEG2_MPEG2 �������������ŵ����\n");
    	
    	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
        CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"����1 ע����Դ��ʼ��ʧ��");
        CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"ע���豸ָ�����Ϸ�");
    	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

    	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
    	sSyncMode.m_hSyncHandle = CSUDI_NULL;
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

    	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
    	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");

        while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����7ʧ��");
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");
        
    	CSTCPrint("����ע��%s �������������ŵ����\n",g_TSStreamInfo[eServiceStream].m_pcName);
    	CSTCPrint("Test ts inject of %s\n",g_TSStreamInfo[eServiceStream].m_pcName);
    	
    	CSTCPrint("����Ƶ��������������\n");
    	CSTCPrint("Is it playing normally ?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

    	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����11ʧ��");    
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����12ʧ��");
        hINJECTER = CSUDI_NULL;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����13ʧ��");   
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
        hPlayer = CSUDI_NULL;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
        hInjectThread = CSUDI_NULL;

        //pcm play
        eServiceStream = EM_INJECT_PCM_001;
        sInjecterChnl.m_nAudioDecoder = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
        sInjecterChnl.m_nDemux = CSUDI_INVALID_INDEX;
        sInjecterChnl.m_nVideoDecoder = CSUDI_INVALID_INDEX;
        CSTCPrint("m_nAudioDecoder = %d\n",sInjecterChnl.m_nAudioDecoder);
        CSTK_ASSERT_TRUE_FATAL((sInjecterChnl.m_nAudioDecoder != -1),"ע����Դ��ʼ��ʧ��");

    	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
    	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����16ʧ��");

        sPcmStartParam.m_uBitsSample = 16;
        sPcmStartParam.m_uChannels = 2;
        sPcmStartParam.m_uSampleRate = 44100;
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����17-1ʧ��");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����17-2ʧ��");
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����18ʧ��");

        g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
    	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("pcm Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "����19ʧ��");

        while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "�ļ���ʧ��");

        sPlayerChnl.m_nAudioDecoder = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
        sPlayerChnl.m_nDemux = CSUDI_INVALID_INDEX;
        sPlayerChnl.m_nVideoDecoder = CSUDI_INVALID_INDEX;
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����20ʧ��");

        sStreamInfo[0].m_nPid = 0;
    	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
    	sStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_PCM  ;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,1,CSUDI_NULL), "����21ʧ��");
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����22ʧ��");

		CSTCPrint("����ע��%s ���������ŵ����\n", g_TSStreamInfo[eServiceStream].m_pcName);
    	CSTCPrint("Test pcm inject of %s\n", g_TSStreamInfo[eServiceStream].m_pcName);
       
    	CSTCPrint("��Ƶ��������������\n");
    	CSTCPrint("Is it playing normally ?\n");
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����23ʧ��");

        g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����24ʧ��");	
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����25ʧ��");    
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����26ʧ��");
        hINJECTER = CSUDI_NULL;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����27ʧ��");   
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����28ʧ��");
        hPlayer = CSUDI_NULL;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����29ʧ��");
        hInjectThread = CSUDI_NULL;
    }
   
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����25ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����26ʧ��");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����27ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����28ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����29-1ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����29-2ʧ��");
		hInjectThread = CSUDI_NULL;
	}

	return CSUDI_TRUE;   
}


