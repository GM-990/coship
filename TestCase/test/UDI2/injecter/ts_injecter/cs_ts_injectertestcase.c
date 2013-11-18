/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
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
    EM_INJECT_TS_MPEG2_MPEG2 = 0, 	/*湖南卫视*/
    EM_INJECT_TS_H264_AAC, 		/**/
    EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_1,/*iptv*/
    EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_2,/*iptv*/
    EM_INJECT_TS_CCTV11,    /*EPG_0606_121458.ts 3NVOD-23.556*/
    EM_INJECT_TS_CCTV1,     /*EPG_0606_121458.ts 3指定频道号0*/
    EM_INJECT_TS_MPEG2_MPEG2_AUDIO_1,/*EPG_0606_121458.ts 3NVOD-23.555音频pid=0x15b1*/
    EM_INJECT_TS_MPEG2_MPEG2_AUDIO_2,/*EPG_0606_121458.ts 3NVOD-23.555音频pid=0x15b0*/
    EM_INJECT_PCM_001,/*better man.wav*/
    EM_INJECT_TS_MAX_COUNT
}ES_ServiceIndex_E;

#define MAX_PESES_COUNT_IN_TS (EM_UDI_CONTENT_PCR+1) /*请参考CSUDIContentType_E枚举的类型个数*/

typedef struct _TSStreamInfo_S
{
    char * m_pcName; //注入文件的名
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

//根据音频和视频解码器初始化相应的播放信息
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

//查找支持EM_UDI_DEMUX_INJECT | EM_UDI_DEMUX_PLAY类型的demx
static int searchDemuxID_Y_InInject(void)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;

	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //先取得demux的数量
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //取相对应ID=i 的demux的能力
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

//查找支持nAudioType类型的Audio Decoder
//nAudioType参见CSUDIAUDStreamType_E, 类型为EM_UDI_AUD_STREAM_AC3特殊处理
static int searchAudioID_Y_InInject(int nAudioType)
{
	CSUDIAUDStreamType_E  eAudStreamTypeNum=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIAUDIOCapability_S   sAudioCapabilityInfo;
	int nAudioCount = 0;
	int nAudioID = -1;
	int i = 0;
	int j = 0;
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //先取得音频解码器的数量
	{
		for (i=0; i<nAudioCount; i++)
		{
			
			if(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo))           //到相关ID的音频解码器的能力
			{
				if(nAudioType == EM_UDI_AUD_STREAM_AC3)
				{
					for (j=0; j<eAudStreamTypeNum; j++)                                //其中能力包含两方面：支持解码的能力与支持直接输出的能力
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
					for (j=0; j<eAudStreamTypeNum; j++)                                //其中能力包含两方面：支持解码的能力与支持直接输出的能力
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
	CSUDIStreamInfo_S  sStreamInfo[MAX_PESES_COUNT_IN_TS];  //不会超过10个
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

	CSTCPrint("正在播放 %s...... 节目\r\n",pTSStreamInfo->m_pcName);
	
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

///根据传入的index来决定要搜索哪种类型的视频解码器
//其中输入参数表示相对应的视频在音频类型枚举里的取值

static int searchVideoID_Y_InInject(int type_index)
{
	int i;
	int j;
	CSUDIVIDStreamType_E  eVidStreamTypeNum=EM_UDI_VID_STREAMTYPE_NUM ;
	int nVideoCount;
	int nVidioID=-1;
	CSUDIVIDEOCapability_S   sVideoCapabilityInfo;	
	
	CSUDIVIDEOGetCount(&nVideoCount);                                      //先取数量

	for ( i=0;i < nVideoCount; i ++)
	{
		CSUDIVIDEOGetCapability( i , &sVideoCapabilityInfo);	//再取相关能力，与音频不同的是视频只有支持的视频类型一项

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
		CSTCPrint("测试所需文件 %s 打开失败，请确保testdata目录下存在该文件!!!\n", pFileName);
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
	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	int nInjectedLength = 0; //可注入的大小
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
		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
				
		if(uLength != 0)
		{
			nReadLength = pTSStreamInfo->m_uBufferLen < nFileLeftLength ? pTSStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //剩余文件长度小于可注入空间的长度
			{
				nInjectedLength = nReadLength;
			}
			else  //剩余文件长度大于等于可注入空间的长度
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //读取相应长度的文件
									
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
	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	int nInjectedLength = 0; //可注入的大小
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

			//跳转到文件开始
			InjectFSSeek(file,0,INJECT_SEEK_SET);

			//等到SEEK标志为FALSE后开始送数据
			while(pTSStreamInfo->m_bSeek)
			{
				CSUDIOSThreadSleep(100);
			}
		}

		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度

		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度

		uLength = 0;

		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);

		if(uLength != 0)
		{
			nReadLength = pTSStreamInfo->m_uBufferLen < nFileLeftLength ? pTSStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //剩余文件长度小于可注入空间的长度
			{
				nInjectedLength = nReadLength;
			}
			else  //剩余文件长度大于等于可注入空间的长度
			{
				nInjectedLength = uLength;
			}

			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //读取相应长度的文件
					

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
	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	int nInjectedLength = 0; //可注入的大小
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
		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
				
		if(uLength != 0)
		{
			nReadLength = pESStreamInfo->m_uBufferLen < nFileLeftLength ? pESStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //剩余文件长度小于可注入空间的长度
			{
				nInjectedLength = nReadLength;
			}
			else  //剩余文件长度大于等于可注入空间的长度
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //读取相应长度的文件

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

//注入的音视频解码器
	psInjecterChnl->m_nAudioDecoder=-1;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;	
	psInjecterChnl->m_nDemux=-1;
		
	nDemuxID=searchDemuxID_Y_InInject();
	if(nDemuxID == -1)
	{
		CSTCPrint( "没有支持注入的demux! \n\r");
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
				CSTCPrint( "没有支持指定类型视频解码器!\n\r");
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
				CSTCPrint( "没有支持指定类型音频解码器!\n\r");
                CSTCPrint("There's no audio decode supporting stream type %d \n",\
                    pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eAudioType);
                return -1;
			}
			psPlayerChnl->m_nAudioDecoder = nAudioId;
		}
        
		memcpy(&sStreamInfo[i],&(pTSStreamInfo->m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S));
        nStreamCnt++;
        
	}

	if(nStreamCnt<1)//至少注入一路类型(video,audio,pcr...)
	{
        CSTCPrint("没有配置注入类型!!\n");
        CSTCPrint("There's no stream type configed !!\n");
        return -1;
	}
    
//播放通道
	psInjecterChnl->m_nDemux=psPlayerChnl->m_nDemux;

	pOpenParam->m_eInjecterType = EM_INJECTER_TS;
	pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_DEFAULT;

    initAVInfo(psPlayerChnl->m_nAudioDecoder,psPlayerChnl->m_nVideoDecoder);
	return nStreamCnt;
}

static CSUDI_BOOL closeInjectPlayer(CSUDI_HANDLE hPlayer)
{
	CSUDI_BOOL bRe = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL !=hPlayer,"参数错误");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败");
	
	bRe = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bRe;
	
}

static CSUDI_HANDLE OpenTSInectPlayer(CSUDIPlayerChnl_S *pChnl, CSUDIStreamInfo_S *pStreamInfo, int nCount)
{
	CSUDI_HANDLE hPlayer= CSUDI_NULL;
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pChnl, "参数1错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pStreamInfo, "参数2错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (pChnl, EM_UDIPLAYER_INJECT,&hPlayer), "打开播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,pStreamInfo,nCount,CSUDI_NULL), "设置流失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "启动播放失败");

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
	
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
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
	//在本测试用例集执行前调用
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_TS_INJECTER_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试Open/Close TS注入器时，在参数不符合设计要求情况下的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:参数不符合TS注入器接口设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、psOpenParams->m_eContentType=EM_UDIINJECTER_CONTENT_DEFAULT,调用CSUDIINJECTEROpen试图打开一个TS注入器,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、psInjecterChnl->m_nDemux 不等于-1时，调用CSUDIINJECTEROpen试图打开一个TS注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、psInjecterChnl->m_nVideoDecoder 错误时时，调用CSUDIINJECTEROpen试图打开一个TS注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、psInjecterChnl->m_nAudioDecoder 错误时时，调用CSUDIINJECTEROpen试图打开一个TS注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、phINJECTER = CSUDI_NULL 时，调用CSUDIINJECTEROpen试图打开一个TS注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、psInjecterChnl = CSUDI_NULL时，调用CSUDIINJECTEROpen试图打开一个TS注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、psOpenParams = CSUDI_NULL时，调用CSUDIINJECTEROpen试图打开一个TS注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERClose试图关闭一个无效的TS注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤2 至步骤7，共1000次
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	for (i=0;i<REPEAT_TIMES; i++)
	{
		e_ContentType = sOpenParams.m_eContentType;
		
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_VIDEO;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

		sOpenParams.m_eContentType = e_ContentType;
		
		nDeviceID = sInjecterChnl.m_nDemux;
		sInjecterChnl.m_nDemux=ERRORDATA;		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_DEVICE_ID == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤3失败");
		sInjecterChnl.m_nDemux=nDeviceID;
		
		nDeviceID = sInjecterChnl.m_nVideoDecoder;
		sInjecterChnl.m_nVideoDecoder = ERRORDATA;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤4失败");
		sInjecterChnl.m_nVideoDecoder = nDeviceID;

		nDeviceID = sInjecterChnl.m_nAudioDecoder;
		sInjecterChnl.m_nAudioDecoder = ERRORDATA;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤5失败");
		sInjecterChnl.m_nAudioDecoder = nDeviceID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,CSUDI_NULL), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (CSUDI_NULL,&sOpenParams,&hINJECTER), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,CSUDI_NULL,&hINJECTER), "步骤8失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(CSUDI_NULL), "步骤9失败");

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
//@DESCRIPTION:测试所有参数均符合接口设计要求的情况下，CSUDIINJECTEROpen & CSUDIINJECTERClose的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:所有参数和应用场景均符合接口设计要求
//@EXPECTATION:返回CSUDI_SUCCESS  
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复步骤2和步骤3共1000次。
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤3失败");
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
//@DESCRIPTION:测试ES 注入器Open和Close 不成对调用，多次Close的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:参数均符合接口设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS  
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，正常打开一个TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTEROpen，正常打开一个TS注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERClose，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复步骤2至步骤5，共1000次
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "步骤2失败");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS !=  CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER1)), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(hINJECTER), "步骤5失败");
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
//@DESCRIPTION:多线程交叉调用CSUDIINJECTEROpen & CSUDIINJECTERClose的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 合法的
//@EXPECTATION:不死机，不死锁，逻辑正确
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、启动另外一个任务，该任务同时并异步执行下面的步骤3和4
//@EXECUTIONFLOW:3、调用CSUDIINJECTEROpen，正常打开一个TS注入器，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:4、如果步骤3成功，则调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤3至步骤4，共1000次
//@EXECUTIONFLOW:6、等待步骤2启动的任务正常完成退出
//@EXECUTIONFLOW:7、销毁步骤2启动的任务
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OpenClose", INJECTTHREADPRO, INJECTTHREADBUF, task_For_TSOpenColse, (void *)EM_INJECT_TS_MPEG2_MPEG2, &hInjectThread), "步骤六失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		udiRe = CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_DEVICE_BUSY == udiRe )),"步骤3失败");//how is not-support

		CSUDIOSThreadSleep(2);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤4失败");
			hINJECTER = CSUDI_NULL;
		}
	}
		
	CSTK_FATAL_POINT;
	
	if (CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤6失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤7失败");
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
//@DESCRIPTION:测试TS注入事例句柄为CSUDI_NULL时，Start/Stop函数的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER = CSUDI_NULL
//@EXPECTATION:Start/Stop函数均返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、正常打开TS注入注入器，得到TS注入器句柄
//@EXECUTIONFLOW:3、以hINJECTER = CSUDI_NULL,调用CSUDIINJECTERStart,期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:4、以hINJECTER = CSUDI_NULL,调用CSUDIINJECTERStop,期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:5、重复步骤3至步骤4，共1000次
//@EXECUTIONFLOW:6、调用CSUDIINJECTERClose，关闭此TS注入器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStart(CSUDI_NULL), "步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStop(CSUDI_NULL), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;

	if ( hINJECTER!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤6失败");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试正常情况下，CSUDIINJECTERStart和CSUDIINJECTERStop成对调用的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为正常打开的TS注入事例句柄
//@EXPECTATION:CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、正常打开TS注入注入器，得到TS注入器句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERStart,启动此TS注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStop,停止此TS注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:5、重复步骤3至步骤4，共1000次
//@EXECUTIONFLOW:6、调用CSUDIINJECTERClose，关闭此TS注入器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤6失败");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试正常情况下，CSUDIINJECTERStart和CSUDIINJECTERStop不成对或逆序调用的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为正常打开的TS注入事例句柄
//@EXPECTATION:返回非CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、正常打开TS注入注入器，得到TS注入器句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERStop,停止此TS注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart,启动此TS注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart,启动此TS注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStop,停止此TS注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:7、调用CSUDIINJECTERStop,停止此TS注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:8、重复步骤3至步骤7，共1000次
//@EXECUTIONFLOW:9、调用CSUDIINJECTERClose，关闭此TS注入器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStart(hINJECTER), "步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "步骤7失败");
	}
	
	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤9失败");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:多线程交叉调用CSUDIINJECTERStart & CSUDIINJECTERStop的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 合法的TS注入句柄
//@EXPECTATION:不死机，不死锁，逻辑正确
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，正常打开一个TS注入器，得到注入器句柄
//@EXECUTIONFLOW:3、启动另外一个任务，该任务同时并异步执行下面的步骤4和5
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，启动此TS注入器，期望返回CSUDI_SUCCESS或CSUDIREC_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStop，停止此TS注入器，期望返回CSUDI_SUCCESS或CSUDIREC_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:6、重复步骤6至步骤7，共10次
//@EXECUTIONFLOW:7、调用CSUDIOSThreadJoin，等待步骤5启动的任务执行完成，关闭退出
//@EXECUTIONFLOW:8、调用CSUDIINJECTERClose，关闭此TS注入器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ESStartStop", INJECTTHREADPRO, INJECTTHREADBUF, task_For_ESStartStop, (void *)hINJECTER, &hInjectThread), "步骤3失败");
	
	for (i=0;i<REPEAT_TIMES; i++)
	{	
		udiRe = CSUDIINJECTERStart(hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STARTED == udiRe )),"步骤4失败");

		CSUDIOSThreadSleep(2);


		udiRe =  CSUDIINJECTERStop(hINJECTER);
		
		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STOPPED == udiRe )),"步骤5失败");
	}
	
	CSTK_FATAL_POINT;
	
	if (CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤7失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤8失败");
		hInjectThread = CSUDI_NULL;
	}
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤8失败");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试在TS注入模式，接口参数不符合设计要求的情况下，CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、不符合接口设计要求 
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERSetAttribute设置属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、pvData = CSUDI_NULL,时，调用CSUDIINJECTERSetAttribute设置同步属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、pvData = CSUDI_NULL,时，调用CSUDIINJECTERSetAttribute 设置回调函数属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERSetAttribute 设置只读状态属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERSetAttribute 设置只读缓冲区属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERSetAttribute 设置非法属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERGetAttribute获取属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:11、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute 获取同步属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute获取回调属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute 获取状态属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:13、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute 获取缓冲区属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERSetAttribute 获取非法属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:15、重复步骤3~步骤14共1000次
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose，关闭此TS注入器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
//设置				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,CSUDI_NULL),  "步骤4失败");
				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_STATUS, &sStatus),  "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sBuffer),  "步骤7失败");

//获取	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,CSUDI_NULL),  "步骤10失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,CSUDI_NULL),  "步骤12失败");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,CSUDI_NULL),  "步骤13失败");		
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试在TS注入模式，接口参数均符合设计要求的情况下，CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、均符合接口设计要求 
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置同步方式属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute 设置回调函数属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5、调用CSUDIINJECTERGetAttribute获取同步方式属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、判断获取得到的同步方式属性为步骤3所设置的属性一致
//@EXECUTIONFLOW:7、调用CSUDIINJECTERGetAttribute获取缓冲区属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERGetAttribute获取状态属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIINJECTERGetAttribute获取回调函数属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:10、如果上一步返回成功，则判断获取得到的回调函数属性为步骤4所设置的属性一致
//@EXECUTIONFLOW:11、重复步骤3~步骤10共10次
//@EXECUTIONFLOW:12、调用CSUDIINJECTERClose，关闭此TS注入器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode =  EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = hINJECTER;//根据udi 头文件，这里需要关联injecteropen 打开的句柄
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
//设置			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode),  "步骤3失败");					

//获取
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode),  "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(((sSyncMode.m_eSyncMode == EM_UDIINJECTER_SYNC_AUDIO_FIRST)
			||(sSyncMode.m_hSyncHandle == CSUDI_NULL)),"步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sBuffer),  "步骤7失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,&sStatus),  "步骤8失败");
	}
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤12失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试TS注入器在start之后再调用CSUDIINJECTERSetAttribute/CSUDIINJECTERGetAttribute的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 非空
//@INPUT:2、eAttrType=EM_UDIINJECTER_TS_ENDIAN  
//@INPUT:3、pvData非空
//@EXPECTATION:调用CSUDIINJECTERSetAttribute    返回CSUDIINJECTER_ERROR_INVALID_STATUS，调用CSUDIINJECTERGetAttribute返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK:启动注入后不能调用CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置同步属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart启动此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERSetAttribute设置"回调函数"属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERGetAttribute获取"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、判断获取得到的同步属性为步骤3所设置的属性一致
//@EXECUTIONFLOW:9、调用CSUDIINJECTERGetAttribute获取"状态"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERGetAttribute获取"缓冲区"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用CSUDIINJECTERGetAttribute获取"回调函数"属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:12、重复步骤6~步骤11共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop，停止此TS注入器
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose，关闭此TS注入器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");

	sSyncMode.m_eSyncMode =  EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	for (i=0; i<REPEAT_TIMES; i++)
	{
//设置
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤5失败");

//获取
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤7失败");

		CSTK_ASSERT_TRUE_FATAL(((sSyncMode.m_eSyncMode == EM_UDIINJECTER_SYNC_AUDIO_FIRST)
			||(sSyncMode.m_hSyncHandle == CSUDI_NULL)),"步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,&sStatus), "步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sStatus), "步骤10失败");
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤13失败");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush 
//@DESCRIPTION:测试参数或应用场景不符合设计要求时,接口函数CSUDIINJECTERFlush的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的TS注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL，2) Start注入器之前,3) Stop注入器之后等三种情况下调用此函数均返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、在启动注入之前调用CSUDIINJECTERFlush,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、在hINJECTER == CSUDI_NULL的情况下，调用CSUDIINJECTERFlush，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERStop停止此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、在hINJECTER 为步骤2得到的TS注入器句柄时，调用CSUDIINJECTERFlush，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤6~步骤9共1000次
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭注入器
//@EXECUTIONFLOW:12、关闭步骤2打开的播放器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "步骤4失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤5失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(CSUDI_NULL), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "步骤9失败");		
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");
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
//@DESCRIPTION:测试参数和应用场景均符合设计要求时,接口函数CSUDIINJECTERFlush的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的TS注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart启动此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetFreeBuffer，期望得到注入器空闲缓冲区的地址和大小，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERWriteComplete，通知底层注入数据已准备好，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERFlush，等待本次注入的数据播放完，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复步骤6~步骤8共5次
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此TS注入器
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭此TS注入器
//@EXECUTIONFLOW:12、关闭步骤2打开的播放器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	for (i=0;i<5; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer(hINJECTER, &pvBuffer, &uLength), "步骤6失败");	

		if (uLength > 0)
		{
			memset(pvBuffer,0x0,uLength);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERWriteComplete (hINJECTER,uLength), "步骤7失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER), "步骤8失败");
		}
		else
		{
			CSUDIOSThreadSleep(10);
		}
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤10失败");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");
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
//@DESCRIPTION:测试参数或应用场景不符合设计要求时,接口函数CSUDIINJECTERClear的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的TS注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL，2) Start注入器之前,3) Stop注入器之后等三种情况下调用此函数均返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、在启动注入之前调用CSUDIINJECTERFlush,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、在hINJECTER == CSUDI_NULL的情况下，调用CSUDIINJECTERClear，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERStop停止此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、在hINJECTER 为步骤2得到的TS注入器句柄时，调用CSUDIINJECTERFlush，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤6~步骤9共1000次
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭注入器
//@EXECUTIONFLOW:12、关闭步骤2打开的播放器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "步骤4失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤5失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(CSUDI_NULL), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "步骤9失败");		
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");
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
//@DESCRIPTION:测试参数和应用场景均符合设计要求时,接口函数CSUDIINJECTERFlush的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的TS注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart启动此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetFreeBuffer，期望得到注入器空闲缓冲区的地址和大小，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERWriteComplete，通知底层注入数据已准备好，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERFlush，等待本次注入的数据播放完，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复步骤6~步骤8共1000次
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此TS注入器
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭此TS注入器
//@EXECUTIONFLOW:12、关闭步骤2打开的播放器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	for (i=0;i<REPEAT_TIMES; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer(hINJECTER, &pvBuffer, &uLength), "步骤6失败");	

		if (uLength > 0)
		{
			memset(pvBuffer,0x0,uLength);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERWriteComplete (hINJECTER,uLength), "步骤7失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER), "步骤8失败");
		}
		else
		{
			CSUDIOSThreadSleep(10);
		}
	}
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤10失败");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");
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
//@DESCRIPTION:测试接口参数和应用场景不符合设计要求时,接口函数CSUDIINJECTERGetFreeBuffer的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的TS注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:1) Start之前，2)Stop之后，3)注入句柄为CSUDI_NULL，4) 缓冲区参数指针为CSUDI_NULL，5)缓冲区长度参数为CSUDI_NULL，上列5种情况下，函数均应返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、所有参数均正常的情况下，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、hINJECTER为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8、ppvBuffer为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、puLength为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此TS注入器
//@EXECUTIONFLOW:11、所有参数均正常的情况下，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、重复步骤6~步骤10共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，关闭此TS注入器
//@EXECUTIONFLOW:14、关闭步骤2打开的播放器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "步骤5失败");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERGetFreeBuffer (CSUDI_NULL,&pvBuffer,&uLength), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,CSUDI_NULL), "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer(hINJECTER, CSUDI_NULL, &uLength), "步骤9失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "步骤11失败");
	}
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤13失败");
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
//@DESCRIPTION:测试接口参数或应用场景不符合设计要求时,接口函数CSUDIINJECTERWriteComplete的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的TS注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) Start之前，2)Stop之后，3)注入句柄为CSUDI_NULL，4) uWrittenSize比前期得到的空闲长度大，上列4种情况下，函数均应返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERWriteComplete试图写入1字节数据，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此TS注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERGetFreeBuffer得到空闲缓冲区地址和长度
//@EXECUTIONFLOW:8、hINJECTER为CSUDI_NULL，uWrittenSize为步骤7得到的空闲缓冲区长度，调用CSUDIINJECTERWriteComplete，期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:9、hINJECTER为步骤2得到的句柄，uWrittenSize为步骤7得到的空闲缓冲区长度加1，调用CSUDIINJECTERWriteComplete，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此TS注入器
//@EXECUTIONFLOW:11、hINJECTER为步骤2得到的句柄，uWrittenSize为步骤7得到的空闲缓冲区长度，调用CSUDIINJECTERWriteComplete，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、重复步骤6~步骤11共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，关闭此TS注入器
//@EXECUTIONFLOW:14、关闭步骤2打开的播放器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");	

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER,1), "步骤5失败");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERWriteComplete (CSUDI_NULL,uLength), "步骤8失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER, uLength+1), "步骤9失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER, uLength), "步骤11失败");
	}
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤13失败");
		hINJECTER = CSUDI_NULL;
	}
	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIINJECTERWriteComplete     
//@CASEGROUP:2、CSUDIINJECTERGetFreeBuffer   
//@DESCRIPTION:测试路径CSUDIINJECTERGetFreeBuffer-->停止Player-->更新缓存区数据-->CSUDIINJECTERWriteComplete，检查驱动的运行状态
//@PRECONDITION:INJECTER模块被正常初始化，ES Player能正常工作
//@INPUT:1、所有参数均正常
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个TS注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，启动此TS注入器
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetFreeBuffer ，使ppvBuffer  非空，puLength 非空
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStop，停止播放器
//@EXECUTIONFLOW:8、修改ppvBuffer中的内容
//@EXECUTIONFLOW:9、调用CSUDIINJECTERWriteComplete ,写入数据，期望非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERClose，关闭播放器
//@EXECUTIONFLOW:11、调用CSUDIINJECTERStop，停止此注入器
//@EXECUTIONFLOW:12、调用CSUDIINJECTERClose，关闭此注入器
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
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	hPlayer = OpenTSInectPlayer(&sPlayerChnl,sStreamInfo,nStreamCount);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength),"步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(uLength > 0,"步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤8失败");			
	
	memset(pvBuffer,0xFE,uLength);

	CSUDIINJECTERWriteComplete(hINJECTER, uLength);
	 
	CSTK_FATAL_POINT;

	if(CSUDI_NULL !=hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤10失败");
		hPlayer = CSUDI_NULL;
	}
	if(CSUDI_NULL !=hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤11失败");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤12失败");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_MPEG2_MPEG2 TS码流，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定TS流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，是否能音视频节目播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
	
	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"注入设备指定不合法");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

/*<!-- Modify by 903126,2010-12-24 15:51:35: */
/* Note: 等待ts开始注入了在打开播放器,iptv项目组的播放流程就是如此*/
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);
 /* Modify end 2010-12-24 15:51:35 --!>*/

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
	CSTCPrint("Test ts inject of MPEG2\n");
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤11-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_MPEG2_MPEG2 TS码流，进行播放、暂停和恢复暂停
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定TS流，能控制TS播放的正常播放、暂停与恢复播放
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERPause ，暂停TS播放
//@EXECUTIONFLOW:11、提示用户确认，音视频是否停止
//@EXECUTIONFLOW:12、调用CSUDIPLAYERResume ，恢复TS播放
//@EXECUTIONFLOW:13、重复步骤9-步骤12共5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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

	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2  码流，进行播放、暂停和恢复暂停\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("音视频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤10失败");
		
		CSTCPrint("音视频暂停播放了吗？\n");
		CSTCPrint("Do it pause playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤12失败");
		
	}
	
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤15-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤16失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_MPEG2_MPEG2 TS码流，进行播放、停止操作
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定TS流，能控制播放中的停止与恢复
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop ，停止TS播放
//@EXECUTIONFLOW:11、提示用户确认，音视频是否停止
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart ，恢复TS播放
//@EXECUTIONFLOW:13、重复步骤9-步骤12共5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
	
	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2   码流，进行播放、停止操作\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("音视频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");
		
		CSTCPrint("音视频停止播放了吗？\n");
		CSTCPrint("Do it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤12失败");
		
	}
	
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤15-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤16失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_MPEG2_MPEG2 TS码流，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在指定的TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  CSUDIINJECTERFlush等待已注入的内容播放完全才能返回
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIINJECTERFlush ，等待已注入的数据全部播放完，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
	
	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"步骤11失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤12-1失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12-2失败");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤13失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:"测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常注入结束，调用CSUDIINJECTERClear清除已注入但未播放完的数据
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  CSUDIINJECTERClear能立即返回
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClear ，清除已注入的数据，期望音视频立即停止，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
	
	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常注入结束，调用CSUDIINJECTERClear清除已注入但未播放完的数据\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"步骤11失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤12-1失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12-2失败");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤13失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_H264_AAC TS码流，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定H264+AAC音视频
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，是否能音视频节目播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
		CSTCPrint("测试注入的EM_INJECT_TS_H264_AAC 码流格式不能支持\n");
		return CSUDI_TRUE;
	}

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_TS_H264_AAC 码流，正常播放的情况\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤11-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_H264_AAC TS码流，进行播放、暂停和恢复暂停
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定EM_INJECT_TS_H264_AAC流，能控制正常播放、暂停与恢复播放
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频频是否播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERPause ，暂停TS播放
//@EXECUTIONFLOW:11、提示用户确认，音视频是否停止
//@EXECUTIONFLOW:12、调用CSUDIPLAYERResume ，恢复TS播放
//@EXECUTIONFLOW:13、重复步骤9-步骤12共5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
		CSTCPrint("测试注入的EM_INJECT_TS_H264_AAC 码流格式不能支持\n");
		return CSUDI_TRUE;
	}
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_TS_H264_AAC 码流，进行播放、暂停和恢复暂停\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream ,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("音视频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤10失败");
		
		CSTCPrint("音视频暂停播放了吗？\n");
		CSTCPrint("Does it pause play ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤12失败");
		
	}
	
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤15-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤16失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_H264_AAC TS码流，进行播放、停止操作
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定音视频流，能控制播放中的停止与恢复
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop ，停止TS播放
//@EXECUTIONFLOW:11、提示用户确认，音视频是否停止
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart ，恢复TS播放
//@EXECUTIONFLOW:13、重复步骤9-步骤12共5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
		CSTCPrint("测试注入的EM_INJECT_TS_H264_AAC 码流格式不能支持\n");
		return CSUDI_TRUE;
	}
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	CSTCPrint("测试注入EM_INJECT_TS_H264_AAC 码流，进行播放、停止操作\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("音视频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");
		
		CSTCPrint("音视频停止播放了吗？\n");
		CSTCPrint("Does it stop play ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤12失败");
		
	}
	
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤15-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤16失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_H264_AAC TS码流，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在指定的TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  CSUDIINJECTERFlush等待已注入的内容播放完全才能返回
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIINJECTERFlush ，等待已注入的数据全部播放完，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
		CSTCPrint("测试注入的EM_INJECT_TS_H264_AAC 码流格式不能支持\n");
		return CSUDI_TRUE;
	}
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_TS_H264_AAC 码流，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"步骤11失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤12-1失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12-2失败");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤13失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_H264_AAC 码流，正常注入结束，调用CSUDIINJECTERClear清除已注入数据
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  CSUDIINJECTERClear能立即返回
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClear ，清除已注入的数据，期望音视频立即停止，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
		CSTCPrint("测试注入的EM_INJECT_TS_H264_AAC 码流格式不能支持\n");
		return CSUDI_TRUE;
	}
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTCPrint("测试注入EM_INJECT_TS_H264_AAC 码流，正常注入结束，调用CSUDIINJECTERClear清除已注入数据\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"步骤11失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤12-1失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12-2失败");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤13失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_AUDIO_MPEG2 TS码流，但播放器打开的是另外一个流的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  不能播放出指定音视频
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频节目内容不能播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
		CSTCPrint("测试注入的EM_INJECT_TS_H264_AAC 码流格式不能支持\n");
		return CSUDI_TRUE;
	}
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	CSTCPrint("测试注入EM_INJECT_TS_AUDIO_MPEG2 TS码流，但播放器打开的是另外一个流的情况\n");

	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");

	eServiceStream = EM_INJECT_TS_H264_AAC;
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&&((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("请确认音视频节目内容不能播放出来\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

    #if 0 /*<!-- lili 2010-3-23 12:11:03 */
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入
    #else
	g_TSStreamInfo[EM_INJECT_TS_MPEG2_MPEG2].m_bRun = CSUDI_FALSE; //停止注入
    #endif /*0*//* lili 2010-3-23 12:11:03 --!>*/

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤11-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入的数据大于通过CSUDIINJECTERGetFreeBuffer得到的缓冲区大小
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  不能播放出指定TS流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，音视频节目内容不播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask_OverLengh, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，注入的数据大于通过CSUDIINJECTERGetFreeBuffer得到的缓冲区大小\n");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频节目内容不能播放出来吗？\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤11-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_MPEG2_MPEG2 TS码流(此码流为iptv组播放时存在严重马赛克)，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:   1、创建注入器
//@INPUT:   2、创建播放器
//@INPUT:   3、创建注入线程
//@EXPECTATION:  能播放出指定TS流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，是否能音视频节目播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
    
    CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
    
    nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

    sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
    sSyncMode.m_hSyncHandle = CSUDI_NULL;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

    g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

/*<!-- Modify by 903126,2010-12-24 15:51:35: */
/* Note: 等待ts开始注入了在打开播放器,iptv项目组的播放流程就是如此*/
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);
 /* Modify end 2010-12-24 15:51:35 --!>*/

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");
 
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

    CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
    CSTCPrint("Test ts inject of MPEG2\n");
    CSUDIOSThreadSleep(2000);
    
    CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败"); 

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤11-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");
		hINJECTER = CSUDI_NULL;
	}

    return CSUDI_TRUE;
}
//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_MPEG2_MPEG2 TS码流(此码流为iptv组播放时存在严重马赛克)，正常播放的情况
//@DESCRIPTION:同时测试快进快退播发和静音操作之间的组合测试
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定TS流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作和模拟seek操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，是否能音视频节目播放出来?
//@EXECUTIONFLOW:10、调用CSUDIAUDIOMute进行静音操作，期望无声音输出
//@EXECUTIONFLOW:11、通知注入任务开始进行模拟seek操作(即从文件头开始注入)
//@EXECUTIONFLOW:12、提示用户确认视频是否从头开始播放
//@EXECUTIONFLOW:13、调用CSUDIAUDIOUnmute进行取消静音操作，期望有声音输出
//@EXECUTIONFLOW:14、调用CSUDIOSThreadJoin ，等待注入线程结束，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
    
    CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
    
    nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"注入设备指定不合法");
    CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

    sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
    sSyncMode.m_hSyncHandle = CSUDI_NULL;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

    g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("Seek Event", CSUDIOS_EVENT_AUTO_RESET, &(g_TSStreamInfo[eServiceStream].m_hSeekEvent)),  "步骤5失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectSeekTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

/*<!-- Modify by 903126,2010-12-24 15:51:35: */
/* Note: 等待ts开始注入了在打开播放器,iptv项目组的播放流程就是如此*/
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);
 /* Modify end 2010-12-24 15:51:35 --!>*/
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

    CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
    CSTCPrint("Test ts inject of MPEG2\n");
    CSUDIOSThreadSleep(2000);
    
    CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	CSUDIAUDIOMute(sPlayerChnl.m_nAudioDecoder);
	CSTCPrint("静音,期望无声音输出 \n");
    CSTCPrint("Do it mute playing ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");
    
    g_TSStreamInfo[eServiceStream].m_bSeek = CSUDI_TRUE; //准备SEEK
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSEventWait(g_TSStreamInfo[eServiceStream].m_hSeekEvent, CSUDIOS_TIMEOUT_INFINITY),"步骤11失败");
    CSUDIINJECTERClear(hINJECTER);
    CSUDIPLAYERClear(hPlayer);     
    g_TSStreamInfo[eServiceStream].m_bSeek = CSUDI_FALSE;

    CSTCPrint("测试注入SEEK 操作，正常播放的情况\n");
    CSTCPrint("Test ts inject of MPEG2\n");
    CSUDIOSThreadSleep(2000);
    
    CSTCPrint("视频是否从头开始正常播放？\n");
    CSTCPrint("Is it replaying normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");

	CSUDIAUDIOUnmute(sPlayerChnl.m_nAudioDecoder);
	CSTCPrint("unmute audio \n");
    CSTCPrint("音视频可以正常播放吗？已经进行unmute操作，期望有声音输出\n");
    CSTCPrint("Is it replaying normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败");

    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败"); 

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤15-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤16失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
		hINJECTER = CSUDI_NULL;
	}

    return CSUDI_TRUE;
}
//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_MPEG2_MPEG2 TS码流(此码流为iptv组播放时存在音视频不同步)，正常播放的情况
//@DESCRIPTION:同时测试暂停播放和恢复播发和静音操作之间的组合测试
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定TS流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，是否能音视频节目播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
		CSTCPrint("测试注入的EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_2 码流格式不能支持\n");
		return CSUDI_TRUE;
	}
			
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2  码流，进行播放、暂停和恢复暂停\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");
	
	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");
	
	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("音视频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤10失败");
		
		CSTCPrint("音视频暂停播放了吗？\n");
		CSTCPrint("Do it pause playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤12失败");
		CSTCPrint("%d times to test \n",(5-i-1));
	}
	//add for testing mute&unmute interacts with pause&resume
	CSUDIAUDIOMute(sPlayerChnl.m_nAudioDecoder);
	CSTCPrint("mute audio \n");
	for (i=0;i<5;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤10失败");
		
		CSTCPrint("音视频暂停播放了吗？\n");
		CSTCPrint("Do it pause playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤12失败");
		CSTCPrint("音视频恢复播放，但期望没有声音出来?\n");
		CSTCPrint("resume playing but expect no audio output \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		CSTCPrint("%d times to test \n",(5-i-1));
	}
	CSUDIAUDIOUnmute(sPlayerChnl.m_nAudioDecoder);
	CSTCPrint("unmute audio \n");
	
	CSTCPrint("音视频播放出来了吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");

	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败"); 
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤15-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤16失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
		hINJECTER = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;

}
//以下封装直接从section模块移植过来，目的是为了 不依赖于其他模块
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
static CSUDI_BOOL	s_bSectionCallbackSuc[EM_TS_SECTION_FILTER_MAX];//申请到数据并且通过验证
static CSUDI_BOOL	s_bSectionCallbackRuq[EM_TS_SECTION_FILTER_MAX];//只是申请到数据
/*
*设置回调函数状态值，主要在启动filter之前设置
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
	int nFilterIndex = 0; //默认用PAT_FILTER_001

	if(s_bSectionCallbackRuq[nFilterIndex]) //如果已经请求到数据则返回，不会去重复校验
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
			
			TS_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_TRUE);  //设置回调请求到数据但暂时没有校验
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
		 CSTCPrint("(int)pvUserData应该不大于EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE ;
	}

	for(i=0;i < nWaitTime;i++) //等待2s获取PAT获取
	{
	    CSUDIOSThreadSleep(1000); 
        
        if( bCheck ) //需要校验数据
		{	  
			if( s_bSectionCallbackSuc[nFilterIndex])
			{
				bRet = CSUDI_TRUE;
			}
		}
		else   //不需要校验数据
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
		CSTCPrint("[SECTION]FILTER%03d申请到数据,但未通过数据验证，请检查测试流文件是否正确\r\n", nFilterIndex+1);
		CSTCPrint("[SECTION] 当前FILTER数据:\r\n");
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
		CSTCPrint("\r\n[SECTION]Filter %03d 未申请到数据\r\n",nFilterIndex+1);
		CSTCPrint("\r\n[SECTION]Filter %03d not Receive data\r\n",nFilterIndex+1);
	}
	return CSUDI_FALSE;	
}
/*
* 分配一个filter，并设置回调函数状态值
* 主要用在测试关注filter获取的数据值的情况，比如用例CSTC_SECTION_IT_Allocate_0003
*/
static CSUDI_BOOL TS_iAllocFilter(const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData,CSUDI_HANDLE *phFilter)
{
	int nFilterIndex = 0xff;
	
	if((int)pvUserData >= EM_TS_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserData应该不大于EM_SECTION_FILTER_MAX\n");
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
		 CSTCPrint("(int)pvUserData应该不大于EM_SECTION_FILTER_MAX\n");
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
//@DESCRIPTION:测试注入demux搜索PSI之后进行注入播放的功能
//@PRECONDITION:1、INJECTER模块被正常初始化
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@INPUT:PAT_FILTER001: 可获取数据，深度为1
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00
//@INPUT: MASK	:0xff
//@INPUT: NEGATE:0x00
//@INPUT:------------------------------
//@EXPECTATION:  能搜索到数据并播放出指定TS流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIDEMUXGetCount获取demux个数,期望不为0
//@EXECUTIONFLOW:3、调用CSUDIDEMUXGetCapability获取支持数据注入的demuxID，期望存在
//@EXECUTIONFLOW:4、使用demuxID调用CSUDIINJECTEROpen，创建一个注入TS码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFILTERAllocate分配搜索PAT表的filter1，期望分配成功
//@EXECUTIONFLOW:8、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据
//@EXECUTIONFLOW:9、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW:10、停止filter1的请求，并释放filter
//@EXECUTIONFLOW:11、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:14、提示用户确认，是否音视频节目能播放出来?
//@EXECUTIONFLOW:15、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:20、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
		
		if((sTmpCap.m_dwWorkTypeMask & EM_UDI_DEMUX_INJECT) == EM_UDI_DEMUX_INJECT && sTmpCap.m_nFilterNum > 0) //支持数据注入
		{
			break;
		}
	}
	
	if(nDemuxIndex == nDemuxCount)
	{
			CSTCPrint("找不到有inject能力的demux设备或者支持inject功能的demux设备上不存在filter设备，此平台不支持该测试用例!!!");
			return CSUDI_TRUE;
	}
	
	CSTCPrint("Demux Id = %d support Ts inject\n",nDemuxIndex);

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&& \
							(nDemuxIndex==sInjecterChnl.m_nDemux)&& \
							(sInjecterChnl.m_nAudioDecoder==-1)&& \
							(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1)),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");
	
	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
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
    		CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤11失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤12失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤13失败");

    CSUDIOSThreadSleep(2000);

    CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败");

    g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤15失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤16-1失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16-2失败");
		hInjectThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤17失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤18失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤19失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤20失败");
		hINJECTER = CSUDI_NULL;
	}

    return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_TS_CCTV1 TS码流，进行切换频道播放的情况(采用CSUDIPLAYERModifyStream接口)
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定TS流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，是否音视频节目能播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERModifyStream，设置节目信息为EM_INJECT_TS_MPEG2_MPEG2
//@EXECUTIONFLOW:11、等待3s中，然后再设置节目信息为EM_INJECT_TS_CCTV1
//@EXECUTIONFLOW:12、期望节目能正确播放
//@EXECUTIONFLOW:13、重复10到12步骤不小于5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
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
	
	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
	
	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
    CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
    CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"注入设备指定不合法");
	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

/*<!-- Modify by 903126,2010-12-24 15:51:35: */
/* Note: 等待ts开始注入了在打开播放器,iptv项目组的播放流程就是如此*/
    while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);
 /* Modify end 2010-12-24 15:51:35 --!>*/

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
	CSTCPrint("Test ts inject of MPEG2\n");
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");	
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==TS_iSetStream(CSUDI_FALSE,hPlayer,EM_INJECT_TS_MPEG2_MPEG2),"步骤10失败");
    
    CSTCPrint("切换节目，音视频可以正常播放吗？\n");
	CSTCPrint("After switching program,Is it still playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

    for(i=0;i<TS_LOOP_TIMES;i++)
    {
        if((i%2) == 0)
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==TS_iSetStream(CSUDI_FALSE,hPlayer,EM_INJECT_TS_CCTV11),"步骤10失败");
        }
        else
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==TS_iSetStream(CSUDI_FALSE,hPlayer,EM_INJECT_TS_MPEG2_MPEG2),"步骤11失败");
        }
        
        CSUDIOSThreadSleep(3000);
    }
    CSTCPrint("切换节目，音视频可以正常播放吗？\n");
	CSTCPrint("After switching program,Is it still playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");


	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入


	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤15-1失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15-2失败");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤16失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
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
//@DESCRIPTION:测试注入EM_INJECT_TS_CCTV1 TS码流，进行切换频道播放的情况(停止重启播放的方式)
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定TS流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、期望在5000ms内能收到新帧到来事件
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、重复步骤1到15不少于5次,期望每一步都成功
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
		CSTCPrint("测试注入的EM_INJECT_TS_MPEG2_MPEG2_FROM_IPTV_2 码流格式不能支持\n");
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
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_FALSE),"CSUDIVIDEOShow 失败");
            
            CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
            CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"注入设备指定不合法");
        	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

        	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
        	sSyncMode.m_hSyncHandle = CSUDI_NULL;
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

        	g_TSStreamInfo[nStream].m_hInjecter = hINJECTER;
        	g_TSStreamInfo[nStream].m_bRun = CSUDI_FALSE;
        	
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[nStream], &hInjectThread), "步骤5失败");

            while(!g_TSStreamInfo[nStream].m_bRun) CSUDIOSThreadSleep(500);

            CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[nStream].m_bOpenFileSuccess, "文件打开失败");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
            s_bShow = CSUDI_FALSE;
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback (hPlayer,TS_PLAYERCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING,NULL), "步骤6失败");
        	
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");

            s_nStartPlayTime = CSUDIPlusOSGetTickCount();
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

			nWaiteTimes = 0;
            while(!s_bShow && nWaiteTimes<10)
            {
                CSUDIOSThreadSleep(500);
                nWaiteTimes ++;
            }

			CSTK_ASSERT_TRUE_FATAL(nWaiteTimes<10, "5000ms内未收到新帧到来事件");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hPlayer,TS_PLAYERCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING,NULL), "步骤8失败");
        	CSTCPrint("测试注入%s 码流，正常播放的情况\n",g_TSStreamInfo[nStream].m_pcName);
        	CSTCPrint("Test ts inject of %s\n",g_TSStreamInfo[nStream].m_pcName);
        	CSUDIOSThreadSleep(3000);
        	
        	g_TSStreamInfo[nStream].m_bRun = CSUDI_FALSE; //停止注入

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11失败");
            hInjectThread = CSUDI_NULL;

            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤12失败");    
            
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤13失败");
            hINJECTER = CSUDI_NULL;
            
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤14失败");   
            
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤15失败");
            hPlayer = CSUDI_NULL;
        }
    }

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[nStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤11-1失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤11-2失败");
		hInjectThread = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");
		hINJECTER = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}
//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试ts注入与PCM注入切换播放的情景
//@PRECONDITION:1、INJECTER模块被正常初始化
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定TS流和pcm流
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，是否能音视频节目播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTEROpen，创建一个注入PCM的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERSetAttribute ，设置pcm属性
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、创建一个线程,循环读取pcm文件，并进行PCM注入操作
//@EXECUTIONFLOW:20、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:21、调用CSUDIPLAYERSetStream ，设置好PCM信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:22、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放PCM
//@EXECUTIONFLOW:23、提示用户确认，是否能音频播放出来
//@EXECUTIONFLOW:24、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:25、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:26、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:27、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:28、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:29、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
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
    	
    	CSTCPrint("测试注入EM_INJECT_TS_MPEG2_MPEG2 码流，正常播放的情况\n");
    	
    	nStreamCount = getTSInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,sStreamInfo);
        CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"步骤1 注入资源初始化失败");
        CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"注入设备指定不合法");
    	CSTK_ASSERT_TRUE_FATAL(((sPlayerChnl.m_nDemux!= -1)&& ((sPlayerChnl.m_nAudioDecoder != -1) || (sPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

    	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
    	sSyncMode.m_hSyncHandle = CSUDI_NULL;
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

    	g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
    	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");

        while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤7失败");
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");
        
    	CSTCPrint("测试注入%s 码流，正常播放的情况\n",g_TSStreamInfo[eServiceStream].m_pcName);
    	CSTCPrint("Test ts inject of %s\n",g_TSStreamInfo[eServiceStream].m_pcName);
    	
    	CSTCPrint("音视频可以正常播放吗？\n");
    	CSTCPrint("Is it playing normally ?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

    	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤11失败");    
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤12失败");
        hINJECTER = CSUDI_NULL;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤13失败");   
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
        hPlayer = CSUDI_NULL;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
        hInjectThread = CSUDI_NULL;

        //pcm play
        eServiceStream = EM_INJECT_PCM_001;
        sInjecterChnl.m_nAudioDecoder = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
        sInjecterChnl.m_nDemux = CSUDI_INVALID_INDEX;
        sInjecterChnl.m_nVideoDecoder = CSUDI_INVALID_INDEX;
        CSTCPrint("m_nAudioDecoder = %d\n",sInjecterChnl.m_nAudioDecoder);
        CSTK_ASSERT_TRUE_FATAL((sInjecterChnl.m_nAudioDecoder != -1),"注入资源初始化失败");

    	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
    	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤16失败");

        sPcmStartParam.m_uBitsSample = 16;
        sPcmStartParam.m_uChannels = 2;
        sPcmStartParam.m_uSampleRate = 44100;
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤17-1失败");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤17-2失败");
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤18失败");

        g_TSStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
    	g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("pcm Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &g_TSStreamInfo[eServiceStream], &hInjectThread), "步骤19失败");

        while(!g_TSStreamInfo[eServiceStream].m_bRun) CSUDIOSThreadSleep(500);

        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_TSStreamInfo[eServiceStream].m_bOpenFileSuccess, "文件打开失败");

        sPlayerChnl.m_nAudioDecoder = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
        sPlayerChnl.m_nDemux = CSUDI_INVALID_INDEX;
        sPlayerChnl.m_nVideoDecoder = CSUDI_INVALID_INDEX;
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤20失败");

        sStreamInfo[0].m_nPid = 0;
    	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
    	sStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_PCM  ;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,1,CSUDI_NULL), "步骤21失败");
    	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤22失败");

		CSTCPrint("测试注入%s ，正常播放的情况\n", g_TSStreamInfo[eServiceStream].m_pcName);
    	CSTCPrint("Test pcm inject of %s\n", g_TSStreamInfo[eServiceStream].m_pcName);
       
    	CSTCPrint("音频可以正常播放吗？\n");
    	CSTCPrint("Is it playing normally ?\n");
        CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤23失败");

        g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤24失败");	
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤25失败");    
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤26失败");
        hINJECTER = CSUDI_NULL;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤27失败");   
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤28失败");
        hPlayer = CSUDI_NULL;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤29失败");
        hInjectThread = CSUDI_NULL;
    }
   
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤25失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤26失败");
		hPlayer = CSUDI_NULL;
	}
	
	if(CSUDI_NULL != hINJECTER)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤27失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤28失败");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectThread)
	{
		g_TSStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤29-1失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤29-2失败");
		hInjectThread = CSUDI_NULL;
	}

	return CSUDI_TRUE;   
}


