/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_audiotestcase.h"
#include "udi2_audio.h"
#include "udi2_aout.h"
#include "cs_testkit.h"
#include "udi2_player.h"
#include "udi2_os.h"
#include "../cs_udi2testcase.h"
#include "udiplus_debug.h"

#define AUDIO_INDEX_0  (0)
#define AUDIO_TEST_TIMES  (5)
#define AUDIO_DEFAULT_VOLUME (16)

//ϵͳ����Ƶ����������
static int s_nAudioCount = 0;

////��Ƶ������֧�ֵ���Ƶ��ʽ��������
static char * g_pcAudioStreamType[EM_UDI_AUD_STREAMTYPE_NUM] ={
	"STREAM_INVALID",
	"STREAM_AC3",
	"STREAM_DTS",
	"STREAM_MPEG1",
	"STREAM_MPEG2",
	"STREAM_CDDA",
	"STREAM_PCM",
	"STREAM_LPCM",
	"STREAM_MP3",
	"STREAM_MPEG_AAC",
	"STREAM_HE_AAC",
	"STREAM_RAW_AAC",
	"STREAM_AAC_PLUS_ADTS",
	"STREAM_AAC_PLUS_LOAS",
	"STREAM_AC3_PLUS",
	"STREAM_DTS_HD",
	"STREAM_WMA_STD",
	"STREAM_WMA_PRO",
};

//��Ƶͨ�������ģʽ
static char * g_pcAudioMode[2] ={
	"DECODE",
	"BYPASS"
};


typedef struct 
{
	CSUDIAUDIOChannelType_E m_eChannel;
	char  			m_cChannelInfo[64];
}AudioTestChannel_S;

typedef struct {
	unsigned int  	uAudOutput;
	char 		    cAudOutputName[128];
}AudioTestOutputCfg_S;

typedef enum 
{
    EM_AUDIO_MULTILANGUAGE1 = 0, /*������*/
    EM_AUDIO_MULTILANGUAGE2, /*�����Բ��Լ������������ԣ�����������Ƶ��ͬ*/
    EM_AUDIO_DOLBYAC3, /*DolbyAC3��׼����*/
    EM_AUDIO_MPEG1 /*MPEG1��Ƶ���Խ�Ŀ*/
}AudioServiceIndex_E;

typedef struct 
{	
	const char*          m_pcTsFilename;  //�����ļ�����
	const char* 		 m_pcServiceDescript;  //��������
	int                  m_nPid;              ///< ��������PID 
	CSUDIAUDStreamType_E m_eStreamType;       ///<��Ƶ����
	CSUDI_BOOL			 m_bIsDiffChannel; /*�ý�Ŀ�Ƿ�֧��������������*/
	int					 m_nSampleRate;  ///<  ��Ƶ������ 
	int					 m_nBitWidth;	///<  ��Ƶ����λ�� 
	int					 m_nChannels;  	///<  ͨ����:1-mono,2-stereo 
}AudioTestServiceInfo_S;

static AudioTestServiceInfo_S s_sAudioServices[] = 
{
    {
        "Audio&Video Test_27Mbps_20070524.ts",
        "������",
        0X0475,
        EM_UDI_AUD_STREAM_MPEG2,
        CSUDI_TRUE,
		48000,
		16,
		2
    },
    {
    	"Audio&Video Test_27Mbps_20070524.ts",
        "��������������",
        0X0476,
        EM_UDI_AUD_STREAM_MPEG2,
        CSUDI_FALSE,
		48000,
		16,
		2
    },
    {
        "Audio&Video Test_27Mbps_20070524.ts",
        "DolbyAC3��׼����",
        0x28A,
        EM_UDI_AUD_STREAM_AC3,
        CSUDI_FALSE,
		48000,
		16,
		2
    },
    {
    	"H.264.ts",
    	"MPEG1��Ƶ���Խ�Ŀ",  
        0X65,
        EM_UDI_AUD_STREAM_MPEG1,
        CSUDI_FALSE,
		44100,
		16,
		2
    }    
};
static AudioTestChannel_S s_sAudioChannels[] = 
{
  	{EM_UDIAUDIO_CHANNEL_STEREO,		"STEREO"},
  	{EM_UDIAUDIO_CHANNEL_LEFT,			"LEFT"},
  	{EM_UDIAUDIO_CHANNEL_RIGHT,		"RIGHT"},
  	{EM_UDIAUDIO_CHANNEL_MIXED_MONO,	"MONO"}
};

static char s_aValue[64];
#define AUDIOCFG2VALUE_EUM(section,key)  \
	(memset(s_aValue, '\0', sizeof(s_aValue)) != NULL)? \
	((CSTKGetConfigInfo(section,#key, s_aValue, sizeof(s_aValue)) == CS_TK_CONFIG_SUCCESS) ? \
        CSTKGetIntFromStr(s_aValue,10) > 0 ? key : 0    \
        :0) \
        :0 
        
static char *cButTmp;
#define AUDIOCFG2STRING(key)\
	if(psOutput->uAudOutput & (EM_UDIAOUT_##key)) \
	{ \
	memcpy(cButTmp, #key, strlen(#key)); \
	strcat(cButTmp, ","); \
	cButTmp += (strlen(#key) + strlen(",")); \
	}

static CSUDI_BOOL IsAOUTSupport(CSUDIAOUTDevices_E enType);

//�����Ӧ�����Ƿ���������ļ���
//nSupportTypeIndexΪCSUDIAUDStreamType_Eö�ٶ�������Ӧ��ֵ
CSUDI_BOOL AUDIO_iCheckCapability(int nAudioIndex,int nType/*decode or bypass*/,int nSupportTypeIndex)
{
	char cBuf[32];
	char cKey[64];
	
	if(nSupportTypeIndex >= EM_UDI_AUD_STREAMTYPE_NUM || nType >=2) return CSUDI_FALSE;
	
	memset(cKey, 0, sizeof(cKey));

	//���������
	sprintf(cKey,"AUDIO_%d_%s_%s",nAudioIndex ,g_pcAudioMode[nType],g_pcAudioStreamType[nSupportTypeIndex]);
	
	memset(cBuf, '\0', sizeof(cBuf));

	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("AUDIO", cKey, cBuf, sizeof(cBuf))) 
	{
		CSTCPrint("Can't find cfg %s\n",cKey);
		return CSUDI_FALSE;
	}

	if(cBuf[0] == '1')
	{
		return CSUDI_TRUE;
	}
	return CSUDI_FALSE;
}

//�Ƿ�֧����Ӧ�����͵�bypass���
//nType ΪCSUDIAUDStreamType_Eö�ٶ���֧�ֵ�����
CSUDI_BOOL AUDIO_iIsSupportBypassType(int nAudioIndex,int nType)
{
	int nSupportIndex;
	CSUDIAUDIOCapability_S sCapablity;
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	if(CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity) != CSUDI_SUCCESS)
	{
		CSTCPrint("Get Audio (%d) capability failure !!\n",nAudioIndex);
		return CSUDI_FALSE;
	}
	
	for(nSupportIndex = 0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportByPass[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
	{
		if(sCapablity.m_sSupportByPass[nSupportIndex] == nType)
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}
//�Ƿ�֧����Ӧ�����͵Ľ������
//nType ΪCSUDIAUDStreamType_Eö�ٶ���֧�ֵ�����
CSUDI_BOOL AUDIO_iIsSupportDecodeType(int nAudioIndex,int nType)
{
	int nSupportIndex;
	CSUDIAUDIOCapability_S sCapablity;
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	if(CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity) != CSUDI_SUCCESS)
	{
		CSTCPrint("Get Audio (%d) capability failure !!\n",nAudioIndex);
		return CSUDI_FALSE;
	}
	
	for(nSupportIndex = 0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportDecode[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
	{
		if(sCapablity.m_sSupportDecode[nSupportIndex] == nType)
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}

//��ȡ֧��decode��Ӧ�������͵Ľ���������
//nType ΪCSUDIAUDStreamType_Eö�ٶ���֧�ֵ�����
CSUDI_BOOL AUDIO_iGetAudioIndexWithDecodeByType(int nType,int * nAudioIndex)
{
	int nAudioDecCount = 0;
	int ii;
	if(CSUDI_SUCCESS != CSUDIAUDIOGetCount(&nAudioDecCount) && nAudioDecCount != 0)
	{
		CSTCPrint(" Audio Count Failure !!!");
		return CSUDI_FALSE;
	}

	for(ii = 0; ii < nAudioDecCount; ii++)
	{
		if(AUDIO_iIsSupportDecodeType(ii,nType))	
			break;
	}

	if(ii != nAudioDecCount)
	{
		*nAudioIndex = ii;
		return CSUDI_TRUE;
	}
	
	CSTCPrint("There's no audio support type %d !!!!\n",nType);
	return CSUDI_FALSE;
}

//��ȡ֧��bypass��Ӧ�������͵Ľ���������
//nType ΪCSUDIAUDStreamType_Eö�ٶ���֧�ֵ�����
CSUDI_BOOL AUDIO_iGetAudioIndexWithBypassByType(int nType,int * nAudioIndex)
{
	int nAudioDecCount = 0;
	int ii;
	if(CSUDI_SUCCESS != CSUDIAUDIOGetCount(&nAudioDecCount) && nAudioDecCount != 0)
	{
		CSTCPrint(" Audio Count Failure !!!");
		return CSUDI_FALSE;
	}

	for(ii = 0; ii < nAudioDecCount; ii++)
	{
		if(AUDIO_iIsSupportBypassType(ii,nType))	
			break;
	}

	if(ii != nAudioDecCount)
	{
		*nAudioIndex = ii;
		return CSUDI_TRUE;
	}
	
	CSTCPrint("There's no audio support type %d !!!!\n",nType);
	return CSUDI_FALSE;
}
static CSUDI_BOOL AUDIO_iGetAudioOutputNameFromCfg(AudioTestOutputCfg_S * psOutput)
{
	if(psOutput == CSUDI_NULL) return CSUDI_FALSE;
	
	psOutput->uAudOutput = 0;
	memset(psOutput->cAudOutputName, '\0', sizeof(psOutput->cAudOutputName));

	cButTmp = psOutput->cAudOutputName;

	psOutput->uAudOutput |= AUDIOCFG2VALUE_EUM("AUDIO",EM_UDIAOUT_RCA);
	psOutput->uAudOutput |= AUDIOCFG2VALUE_EUM("AUDIO",EM_UDIAOUT_SPDIF);
	psOutput->uAudOutput |= AUDIOCFG2VALUE_EUM("AUDIO",EM_UDIAOUT_HDMI);

	AUDIOCFG2STRING(RCA);
	AUDIOCFG2STRING(SPDIF);
	AUDIOCFG2STRING(HDMI);
	
	CSTCPrint("uAudOutput Type = %x , cAudOutputName = %s\r\n",  \
		psOutput->uAudOutput, psOutput->cAudOutputName);
	return CSUDI_TRUE;
}

static CSUDI_BOOL AUDIO_iCheckPlay(int nAudioIndex,AudioTestServiceInfo_S * pService,AudioTestOutputCfg_S * psOutput)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	if(pService == CSUDI_NULL) return CSUDI_FALSE;
	
	CSTCPrint("��ʼ��������[%s]�ϵĽ�Ŀ[%s]...\r\n",pService->m_pcTsFilename,pService->m_pcServiceDescript);
	CSTCPrint("��ǰ������ƵΪ:%s,���ݸ�ʽΪ:%s.....\r\n", \
		pService->m_pcServiceDescript, (pService->m_eStreamType == EM_UDI_AUD_STREAM_AC3)?"AC3":"��AC3");

	if((pService->m_nPid == 0) || (pService->m_nPid == 0x1fff))
	{
		CSTCPrint("����:����Ƶ���......\r\n");
	}
	else
	{
		switch(pService->m_eStreamType)
		{
			case EM_UDI_AUD_STREAM_AC3:
				if ((psOutput->uAudOutput) & EM_UDIAOUT_SPDIF)
				{
					CSTCPrint("����:SPDIF�������������........\r\n");							
				}
				
				if(AUDIO_iIsSupportDecodeType(nAudioIndex,EM_UDI_AUD_STREAM_AC3))
				{
					if((psOutput->uAudOutput) & EM_UDIAOUT_SPDIF)
					{
						CSTCPrint("����:����ƽ̨�ܽ�AC3���ݣ�Ҫ�����кϷ��豸�����������.....\r\n");
					}				
				}
				else
				{
					if((psOutput->uAudOutput) & EM_UDIAOUT_SPDIF)
					{
						CSTCPrint("����:����ƽ̨���ܽ�AC3���ݣ�Ҫ������֮��ĺϷ��豸�����������.....\r\n");
					}				
				}
				break;

			default:
				CSTCPrint("����:�����豸(%s)������Ƶ���.\r\n", psOutput->cAudOutputName);
				break;
		}
	
	}

	CSTCPrint("��ȷ��������Ƶ������֧�ֵ��豸�ϲ�������,û��ͣ�١��������쳣����\r\n");
	CSTCPrint("0:Yes,������������ͬ .\r\n");
	CSTCPrint("1:No,����������ͬ.\r\n");
	bRet = CSTKWaitYes(); 		
	return bRet;
}

static void AUDIO_iSetDefault(int nAudioIndex)
{
	if(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(nAudioIndex,AUDIO_DEFAULT_VOLUME))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAUDIOSetVolume failed!\r\n");
	}
		
	if(CSUDI_SUCCESS != CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAUDIOSetChannel failed!\r\n");
	}
	
	if(CSUDI_SUCCESS != CSUDIAUDIOUnmute(nAudioIndex))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAUDIOUnmute failed!\r\n");
	}
	
	if(CSUDI_SUCCESS != CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_DECODE))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAOUTSetOPMode EM_UDIAOUT_SPDIF failed!\r\n");
	}
	
	if(CSUDI_SUCCESS != CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAOUTSetOPMode EM_UDIAOUT_HDMI failed!\r\n");
	}
}

static CSUDI_BOOL AUDIO_iStartPlay(int nAudioIndex, AudioTestServiceInfo_S * pService,AudioTestOutputCfg_S * psOutput,CSUDI_HANDLE * phPlayer,CSUDI_BOOL bSetDefault)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	if(pService == CSUDI_NULL || phPlayer == CSUDI_NULL || psOutput == CSUDI_NULL)
	{
		CSTCPrint("pService == CSUDI_NULL || phPlayer == CSUDI_NULL|| psOutput == CSUDI_NULL!!!\r\n");
		return CSUDI_FALSE;
	}

	if(CSUDI_TRUE != CSTC_UDI2PortingLock(0, pService->m_pcTsFilename))
	{
		CSTCPrint("Lock failure !!!\r\n");
		return CSUDI_FALSE;
	}
	
	if(CSUDI_TRUE != AUDIO_iGetAudioOutputNameFromCfg(psOutput))
	{
		CSTCPrint("Lock failure !!!\r\n");
		return CSUDI_FALSE;
	}
	
	sPlayerChnl.m_nAudioDecoder = 0;
	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nVideoDecoder = 0;



	if(CSUDI_SUCCESS != CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) || hPlayer == CSUDI_NULL)
	{
		CSTCPrint("[%s:%d]Open Player failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	
	*phPlayer = hPlayer;//һ��open�ɹ���Ӧ�ü�¼hPlayer,���򲥷�ʧ�ܺ��޷��رա�

	sStreamInfo.m_nPid = pService->m_nPid;
	sStreamInfo.m_uStreamType.m_eAudioType  = pService->m_eStreamType;
	sStreamInfo.m_eContentType = EM_UDI_CONTENT_AUDIO;

	
	if(CSUDI_SUCCESS != CSUDIPLAYERSetStream(hPlayer, &sStreamInfo, 1, CSUDI_NULL))
	{
		CSTCPrint("[%s:%d]Set Stream failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	if(bSetDefault)
	{
		AUDIO_iSetDefault(nAudioIndex);  ///set default
	}
	
	if(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer))
	{
		CSTCPrint("[%s:%d]Start Player failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}	
	
	CSUDIOSThreadSleep(1000);

	return CSUDI_TRUE;
	
}

static CSUDI_BOOL AUDIO_iStopPlay(int nAudioIndex,CSUDI_HANDLE hPlayer,CSUDI_BOOL bSetDefault)
{
	if(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer))
	{
		CSTCPrint("[%s:%d]Stop Player failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	
	if(bSetDefault)
	{
		AUDIO_iSetDefault(nAudioIndex);  ///set default
	}
	

	if(CSUDI_SUCCESS != CSUDIPLAYERClose(hPlayer))
	{
		CSTCPrint("[%s:%d]Close Player failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	
	
	CSTCPrint("ֹͣ������Ƶ..............!!\r\n");
	
	return CSUDI_TRUE;
}

static CSUDI_BOOL  AUDIO_iGetChannelInfo(const AudioTestChannel_S * psAudioChannel,  char **pcAudioChannelInfo)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	if((NULL != psAudioChannel)&&(NULL != pcAudioChannelInfo))
	{
		*pcAudioChannelInfo = (char *)(psAudioChannel->m_cChannelInfo);
		bRet = CSUDI_TRUE;
	}
	return bRet;
}

static CSUDI_BOOL  AUDIO_iCheckChannel(AudioTestChannel_S * psAudioChannel, unsigned int uAudOutputType)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;

	CSTCPrint("[AUDIO]*******************************************************\r\n");
	CSTCPrint("[AUDIO]����:��������: \r\n");
	
	CSTCPrint("[AUDIO]����:��ǰ����Ϊ%s  .\r\n", psAudioChannel->m_cChannelInfo);
	{
		if(uAudOutputType & EM_UDIAOUT_RCA)
		{
			switch(psAudioChannel->m_eChannel)
			{
			case EM_UDIAUDIO_CHANNEL_STEREO:
				CSTCPrint("[AUDIO]1.ͬʱ�����㲥�͵��ӵ�����.\r\n");
				CSTCPrint("[AUDIO]2.�ε���ɫ����ߣ���ɫ�����Ч�����������ӽ�Ŀ���������ε���ɫ����ߣ���ɫ�����Ч���������㲥��Ŀ������.\r\n");
				CSTCPrint("[AUDIO]3.�л�˲��������.\r\n");\
				CSTCPrint("[AUDIO]4.�л�����ʱ��Ƶ����˸.\r\n");
				break;
				
			case EM_UDIAUDIO_CHANNEL_LEFT:
				CSTCPrint("[AUDIO]1.�����ǰΰ�ɫ���Ǻ�ɫ����ߣ���Ӧ�����������ӽ�Ŀ������.\r\n");
				CSTCPrint("[AUDIO]2.�л�˲��������.\r\n");
				CSTCPrint("[AUDIO]3.�л�����ʱ��Ƶ����˸.\r\n");				
				break;
				
			case EM_UDIAUDIO_CHANNEL_RIGHT:
				CSTCPrint("[AUDIO]1.�����ǰΰ�ɫ���Ǻ�ɫ����ߣ���Ӧ���������㲥��Ŀ������.\r\n");
				CSTCPrint("[AUDIO]2.�л�˲��������.\r\n");
				CSTCPrint("[AUDIO]3.�л�����ʱ��Ƶ����˸.\r\n");				
				break;
				
			case EM_UDIAUDIO_CHANNEL_MIXED_MONO:
				CSTCPrint("[AUDIO]1.ͬʱ�����㲥�͵��ӵ�����.\r\n");
				CSTCPrint("[AUDIO]2.���ܰε���ɫ���ǰ�ɫ����ߣ���֤һ���������Ч���������������ӽ�Ŀ�͹㲥��Ŀ�Ļ����.\r\n");
				CSTCPrint("[AUDIO]3.�л�˲��������.\r\n");
				CSTCPrint("[AUDIO]4.�л�����ʱ��Ƶ����˸.\r\n");				
				break;
				
			default:
				return CSUDI_FALSE;
				break;
			}
		}
	}
	
	CSTCPrint("0:Yes,������������ͬ .\r\n");
	CSTCPrint("1:No,����������ͬ.\r\n");
	CSTCPrint("[AUDIO]*******************************************************\r\n");

	bRet = CSTKWaitYes();
	
	return bRet;

}
static CSUDI_BOOL AUDIO_iCheckVolume(CSUDI_BOOL bHasVolume)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	if(bHasVolume)
	{
		CSTCPrint("�������кϷ��豸�����������,����˲��������...........!!\r\n");
		CSTCPrint("0:Yes,������������ͬ .\r\n");
		CSTCPrint("1:No,����������ͬ.\r\n");
		bRet = CSTKWaitYes();
	}
	else
	{
		CSTCPrint("�������кϷ��豸�����������,����˲��������...........!!\r\n");
		CSTCPrint("0:Yes,������������ͬ .\r\n");
		CSTCPrint("1:No,����������ͬ.\r\n");
		bRet = CSTKWaitYes();
	}
	
	return bRet;
}


static int AUDIO_iGetNextServiceInx(int nServiceInx)
{
	int nIndex ;
	
	if(EM_AUDIO_MULTILANGUAGE2 <= nServiceInx)
	{
		nIndex = EM_AUDIO_MULTILANGUAGE1;
	}
	else
	{
		nIndex = EM_AUDIO_MULTILANGUAGE2;
	}
	
	return nIndex;
}

CSUDI_BOOL CSTC_AUDIO_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	char cBuf[32];
	memset(cBuf, 0, sizeof(cBuf));
	if(CSTKGetConfigInfo("AUDIO","AUDIO_NUMBER",cBuf,sizeof(cBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_nAudioCount = CSTKGetIntFromStr(cBuf,10);    
	}
	else
	{
		CSTCPrint("[AUDIO]�ֶ����Ҳ���AUDIO_NUMBER\r\n");
	}
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_AUDIO_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//ͨ����ȡ�����ļ���ѯƽ̨�Ƿ�֧��ĳ����Ƶͨ������
static CSUDI_BOOL IsAOUTSupport(CSUDIAOUTDevices_E enType)
{
	const char acSection[] = "AUDIO";
    	char acKey[25] = {0};
	char acResult[255] = {0};
	int nResultSize = (int)sizeof(acResult);

	switch(enType)
	{
		case EM_UDIAOUT_RCA:
			strcpy(acKey, "EM_UDIAOUT_RCA"); break;
		case EM_UDIAOUT_SPDIF: 	
			strcpy(acKey, "EM_UDIAOUT_SPDIF"); break;
		case EM_UDIAOUT_HDMI: 	
			strcpy(acKey, "EM_UDIAOUT_HDMI"); break;
		default: 
			break;
	}

	if (CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo(acSection, acKey, acResult, nResultSize))
	{
		 CSTCPrint("[UDI2AOUTEST]:��ȡConfig�ļ���Ϣʧ��! acSection: %s, acKey: %s\n", acSection, acKey);
	}

	if (1 == CSTKGetIntFromStr(acResult, 10))
	{
		return CSUDI_TRUE;	
	}
	return CSUDI_FALSE;
}

//@CASEGROUP:CSUDIAUDIOGetCount(int * pnAudioDecCount)
//@DESCRIPTION: ���Բ���Ϊ�Ϸ�ֵ�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�������ļ��ṩϵͳ֧�ֵ���Ƶ����ĸ���
//@INPUT:  pnAudioDecCount�Ϸ�ָ��
//@EXPECTATION: �ɹ�����CSUDI_SUCCESS�����һ�ȡ����Ƶ�����������������ļ��еĸ���һ��
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2����֤��ȡ����Ƶ�����������������������ļ��������
CSUDI_BOOL CSTC_AUDIO_GetCount_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTCPrint("Audio Count %d! Expected count is %d\n", nAudioDecCount,s_nAudioCount);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount == s_nAudioCount),
			"Step 2 Check Audio Count Failure !!!"
		);
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetCount(int * pnAudioDecCount)
//@DESCRIPTION: ���Բ���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  pnAudioDecCount=CSUDI_NULL
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: ����CSUDIAUDIOGetCount���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_GetCount_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetCount(CSUDI_NULL)),
			"Get Audio Count should be Failure !!!"
		);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}
//@CASEGROUP:CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
//@DESCRIPTION: �������в���Ϊ�Ϸ�ֵ�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�������ļ��ṩϵͳ֧�ֵ���Ƶ����ĸ���
//@PRECONDITION: 3�������ļ��ṩÿ����Ƶ��������������Ϣ
//@INPUT: 1��nAudioIndex = ϵͳ���ڵ�����Audio����ֵ
//@INPUT: 2��psCapabilityInfo = CSUDIAUDIOCapability_S�ṹָ��
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount�õ�Audio����ĿnAudio�������ɹ���ȡϵͳ֧�ֵĸ���
//@EXECUTIONFLOW: 2������CSUDIAUDIOGetCapability��ȡ����nAudio��������Ϣ�������ܳɹ���ȡ
//@EXECUTIONFLOW: 3��У��ÿ��Audio��������Ϣ�������������ļ��е���Ϣһ��
CSUDI_BOOL CSTC_AUDIO_GetCapability_0001(void)
{
	CSUDIAUDIOCapability_S sCapablity;
	int nAudioDecCount = 0;
	int nAudioIndex;
	int nSupportIndex;
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);
	for(nAudioIndex = 0; nAudioIndex < nAudioDecCount; nAudioIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity)),
				"Step 2 Get  Audio Capability Failure !!!"
			);
		//����decodeģʽ֧�ֵ���������
		for(nSupportIndex=0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportDecode[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iCheckCapability(nAudioIndex,EM_UDIAOUT_DECODE,sCapablity.m_sSupportDecode[nSupportIndex])),
					"Step 3 Check Decode mode Audio Capability Failure !!!"
				);
		}
		//����bypassģʽ֧�ֵ���������
		for(nSupportIndex=0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportByPass[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iCheckCapability(nAudioIndex,EM_UDIAOUT_BYPASS,sCapablity.m_sSupportByPass[nSupportIndex])),
					"Step 3 Check Bypass mode Audio Capability Failure !!!"
				);
		}
	}
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
//@DESCRIPTION: ������Ƶ����ֵΪ�Ƿ�ֵ�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ������Ƶ������
//@INPUT: 1��nAudioIndex = -1��nAudio��nAudio+1(nAudioΪϵͳ֧�ֵĸ���)
//@INPUT: 2��psCapabilityInfo = CSUDIAUDIOCapability_S�ṹָ��
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount�õ�Audio����ĿnAudio�������ɹ���ȡϵͳ֧�ֵĸ���
//@EXECUTIONFLOW: 2������CSUDIAUDIOGetCapability(-1,psCapabilityInfo)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAUDIOGetCapability(nAudio,psCapabilityInfo)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAUDIOGetCapability(nAudio+1,psCapabilityInfo)���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_GetCapability_0002(void)
{	
	int nAudioDecCount = 0;
	CSUDIAUDIOCapability_S sCapablity;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetCapability(-1,&sCapablity)),
			"Step 2 Get  Audio Capability with -1 index should be Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetCapability(nAudioDecCount,&sCapablity)),
			"Step 2 Get  Audio Capability with nAudioDecCount index should be Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetCapability(nAudioDecCount+1,&sCapablity)),
			"Step 2 Get  Audio Capability with nAudioDecCount+1 index should be Failure !!!"
		);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
//@DESCRIPTION: ����������Ϣ�ṹ��ָ��Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ������Ƶ������
//@INPUT: 1��nAudioIndex = ϵͳ���ڵ�����Audio����ֵ
//@INPUT: 2��psCapabilityInfo = CSUDI_NULL
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount�õ�Audio����ĿnAudio�������ɹ���ȡϵͳ֧�ֵĸ���
//@EXECUTIONFLOW: 2������ϵͳ��������Ƶ����������CSUDIAUDIOGetCapability(nAudioIndex,CSUDI_NULL)������ÿһ�����ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_GetCapability_0003(void)
{
	int nAudioDecCount = 0;
	int ii;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);
	for(ii = 0; ii < nAudioDecCount; ii++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS != CSUDIAUDIOGetCapability(ii,CSUDI_NULL)),
				"Get Audio Count should be Failure !!!"
			);
	}
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
//@DESCRIPTION: ���Բ���Ϊ�Ƿ�ֵ�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ������Ƶ������
//@INPUT: 1��nAudioIndex = -1��nAudio��nAudio+1(nAudioΪϵͳ֧�ֵĸ���)
//@INPUT: 2��psStatus = CSUDIAUDIOStatus_S�ṹָ�룬CSUDI_NULL
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount�õ�Audio����ĿnAudio�������ɹ���ȡϵͳ֧�ֵĸ���
//@EXECUTIONFLOW: 2������CSUDIAUDIOGetStatus(-1,psStatus)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAUDIOGetStatus(nAudio,psStatus)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAUDIOGetStatus(nAudio+1,psStatus)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������ÿһ��audio���ε���CSUDIAUDIOGetStatus(nAudioindex,CSUDI_NULL)���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_GetStatus_0001(void)
{
	int nAudioDecCount = 0;
	int nAudioIndex = 0;
	CSUDIAUDIOStatus_S sStatus;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetStatus(-1,&sStatus)),
			"Step 2 Get  Audio Status with -1 index should be Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetStatus(nAudioDecCount,&sStatus)),
			"Step 3 Get  Audio Status with nAudioDecCount index should be Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetStatus(nAudioDecCount+1,&sStatus)),
			"Step 4 Get  Audio Status with nAudioDecCount+1 index should be Failure !!!"
		);
	
	for(nAudioIndex=0; nAudioIndex<nAudioDecCount; ++nAudioIndex)
	{
		CSTCPrint("Audio Index %d \r\n",nAudioIndex);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS != CSUDIAUDIOGetStatus(nAudioIndex,CSUDI_NULL)),
				"Step 5 Get  Audio Status with  psStatus=CSUDI_NULL should be Failure !!!"
			);
	}
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
//@DESCRIPTION: �����ܹ���ȷ��ȡAC3��Ƶ���͵���Ƶ״̬��Ϣ
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ����֧��AC3���͵���Ƶ������
//@PRECONDITION: 3���ܹ�����������Ƶ��
//@INPUT: 1��nAudioIndex = ֧��AC3���͵���Ƶ������
//@INPUT: 2��psStatus = CSUDIAUDIOStatus_S�ṹָ��
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount��CSUDIAUDIOGetCapability��ȡ֧��AC3���͵���Ƶ����������ֵnAudio
//@EXECUTIONFLOW: 2������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 3����nAudioIndex�ϲ���"DolbyAC3��׼���Խ�Ŀ"���������ųɹ�
//@EXECUTIONFLOW: 4��ȷ�ϲ��ųɹ��󣬵���CSUDIAUDIOGetStatus����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5��У���ȡ����Ƶ״̬��Ϣ�������������е���Ϣһ��
//@EXECUTIONFLOW: 6��ֹͣnAudioIndex����
CSUDI_BOOL CSTC_AUDIO_GetStatus_0002(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIAUDIOStatus_S sStatus;	
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	memset(&sStatus,0,sizeof(CSUDIAUDIOStatus_S));
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));

	bRet = AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex);
	if(bRet == CSUDI_FALSE)
	{
		CSTCPrint("\rThere's no audio support EM_UDI_AUD_STREAM_AC3 !!!!\r\n");
		return CSUDI_TRUE;
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 3 start play DolbyAC3��׼���Խ�Ŀ failure!!!!"
		);

	CSTCPrint("Step 3 start play DolbyAC3��׼���Խ�Ŀ\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
			"step 3 Check play DolbyAC3��׼���Խ�Ŀ failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetStatus(nAudioIndex,&sStatus)),
			"Step 4 Get audio status failure !!!!"
		);
	
	CSTCPrint("Audio Status: sStatus.m_nSampleRate == %d \t sStatus.m_nBitWidth == %d \t sStatus.m_nChannels==%d \n ",
						sStatus.m_nSampleRate,sStatus.m_nBitWidth,sStatus.m_nChannels);
		
#if 0 /*<!-- yangpengchao 2010/3/12 17:32:26 */
	CSTK_ASSERT_TRUE(
			(CSUDI_TRUE == AUDIO_iCheckStatus(&sStatus,&s_sAudioServices[EM_AUDIO_DOLBYAC3])),
			"Step 5 check audio status failure !!!!"
		);
#endif /*0*//* yangpengchao 2010/3/12 17:32:26 --!>*/
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_DOLBYAC3].m_nSampleRate == sStatus.m_nSampleRate),
			"Step 5 check audio status failure !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_DOLBYAC3].m_nBitWidth == sStatus.m_nBitWidth),
			"Step 5 check audio status failure !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_DOLBYAC3].m_nChannels == sStatus.m_nChannels),
			"Step 5 check audio status failure !!!!"
		);
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 6 stop play failure !!!!"
			);
	}
	hPlayer = CSUDI_NULL;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
//@DESCRIPTION: �����ܹ���ȷ��ȡMPEG2��Ƶ���͵���Ƶ״̬��Ϣ
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ����֧��MPEG2���͵���Ƶ������
//@PRECONDITION: 3���ܹ�����������Ƶ��
//@INPUT: 1��nAudioIndex = ֧��MPEG1���͵���Ƶ������
//@INPUT: 2��psStatus = CSUDIAUDIOStatus_S�ṹָ��
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1��ͨ��CSUDIAUDIOGetCount��CSUDIAUDIOGetCapability��ȡ֧��MPEG2���͵���Ƶ����������ֵnAudio
//@EXECUTIONFLOW: 2������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 3����nAudioIndex�ϲ���"������"���������ųɹ�
//@EXECUTIONFLOW: 4��ȷ�ϲ��ųɹ��󣬵���CSUDIAUDIOGetStatus����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5��У���ȡ����Ƶ״̬��Ϣ�������������е���Ϣһ��
//@EXECUTIONFLOW: 6��ֹͣnAudio����
CSUDI_BOOL CSTC_AUDIO_GetStatus_0003(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIAUDIOStatus_S sStatus;	
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sStatus,0,sizeof(CSUDIAUDIOStatus_S));
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 3 start play MPEG2��Ƶ���Խ�Ŀ failure!!!!"
		);

	CSTCPrint("Step 3 start play MPEG2��Ƶ���Խ�Ŀ\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 3 Check play MPEG2��Ƶ���Խ�Ŀ failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetStatus(nAudioIndex,&sStatus)),
			"Step 4 Get audio status failure !!!!"
		);
	
	CSTCPrint("Audio Status: sStatus.m_nSampleRate == %d \t sStatus.m_nBitWidth == %d \t sStatus.m_nChannels==%d \n ",
						sStatus.m_nSampleRate,sStatus.m_nBitWidth,sStatus.m_nChannels);

	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MULTILANGUAGE1].m_nSampleRate == sStatus.m_nSampleRate),
			"Step 5 check audio status failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MULTILANGUAGE1].m_nBitWidth == sStatus.m_nBitWidth),
			"Step 5 check audio status failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MULTILANGUAGE1].m_nChannels == sStatus.m_nChannels),
			"Step 5 check audio status failure !!!!"
		);

	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 6 stop play failure !!!!"
			);
	}
	hPlayer = CSUDI_NULL;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
//@DESCRIPTION: �����ܹ���ȷ��ȡMPEG1��Ƶ���͵���Ƶ״̬��Ϣ
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ����֧��MPEG1���͵���Ƶ������
//@PRECONDITION: 3���ܹ�����������Ƶ��
//@INPUT: 1��nAudioIndex = ֧��MPEG1���͵���Ƶ������
//@INPUT: 2��psStatus = CSUDIAUDIOStatus_S�ṹָ��
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1��ͨ��CSUDIAUDIOGetCount��CSUDIAUDIOGetCapability��ȡ֧��MPEG1���͵���Ƶ����������ֵnAudio
//@EXECUTIONFLOW: 2������H.264.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 3����nAudio�ϲ���"MPEG1��Ƶ���Խ�Ŀ"���������ųɹ�
//@EXECUTIONFLOW: 4��ȷ�ϲ��ųɹ��󣬵���CSUDIAUDIOGetStatus����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5��У���ȡ����Ƶ״̬��Ϣ�������������е���Ϣһ��
//@EXECUTIONFLOW: 6��ֹͣnAudio����
CSUDI_BOOL CSTC_AUDIO_GetStatus_0004(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIAUDIOStatus_S sStatus;	
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sStatus,0,sizeof(CSUDIAUDIOStatus_S));
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG1,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG1 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MPEG1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 3 start play MPEG1��Ƶ���Խ�Ŀ failure!!!!"
		);

	CSTCPrint("Step 3 start play MPEG1��Ƶ���Խ�Ŀ\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MPEG1],&sOutputCfg)),
			"step 3 Check play MPEG1��Ƶ���Խ�Ŀ failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetStatus(nAudioIndex,&sStatus)),
			"Step 4 Get audio status failure !!!!"
		);
	
	CSTCPrint("Audio Status: sStatus.m_nSampleRate == %d \t sStatus.m_nBitWidth == %d \t sStatus.m_nChannels==%d \n ",
						sStatus.m_nSampleRate,sStatus.m_nBitWidth,sStatus.m_nChannels);

	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MPEG1].m_nSampleRate == sStatus.m_nSampleRate),
			"Step 5 check audio status failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MPEG1].m_nBitWidth == sStatus.m_nBitWidth),
			"Step 5 check audio status failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MPEG1].m_nChannels == sStatus.m_nChannels),
			"Step 5 check audio status failure !!!!"
		);

	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 6 stop play failure !!!!"
			);
	}
	hPlayer = CSUDI_NULL;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: ����nAudioIndexΪ�Ƿ�ֵ�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT: 1��nAudioIndex = -1��nAudio��nAudio+1(nAudioΪϵͳ֧�ֵĸ���)
//@INPUT: 2��eChannelType = ϵͳ֧�ֵ�����(EM_UDIAUDIO_CHANNEL_STEREO)
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount�õ�Audio����ĿnAudio�������ɹ���ȡϵͳ֧�ֵĸ���
//@EXECUTIONFLOW: 2������CSUDIAUDIOSetChannel(-1,eChannelType)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAUDIOSetChannel(nAudio,eChannelType)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAUDIOSetChannel(nAudio+1,eChannelType)���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_SetChannel_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetChannel(-1,EM_UDIAUDIO_CHANNEL_STEREO)),
			"Step 2 set audio channel with -1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetChannel(nAudioDecCount,EM_UDIAUDIO_CHANNEL_STEREO)),
			"Step 3 set audio channel with nAudioDecCount index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetChannel(nAudioDecCount+1,EM_UDIAUDIO_CHANNEL_STEREO)),
			"Step 4 set audio channel with nAudioDecCount+1 index should be Failure !!!"
		);
	
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: �����ܹ������л�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ������Ƶ���ͨ��ΪEM_UDIAOUT_RCA(������Ƶ�����������)
//@INPUT: 1��nAudioIndex = 0
//@INPUT: 2��eChannelType = ϵͳ֧�ֵ�����CSUDIAUDIOChannelType_E
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������"������",�������ųɹ�
//@EXECUTIONFLOW: 3��ȷ�ϲ��ųɹ��󣬱�������֧�ֵ���������CSUDIAUDIOSetChannel(nAudioIndex��eChannelType)
//@EXECUTIONFLOW: ����ΪEM_UDIAUDIO_CHANNEL_STEREO�Ĳ��Բ���
//@EXECUTIONFLOW: 	1��ͬʱ�����㲥�͵��ӵ�����
//@EXECUTIONFLOW: 	2���ε���ɫ����ߣ���ɫ�����Ч�����������ӽ�Ŀ���������ε���ɫ����ߣ���ɫ�����Ч���������㲥��Ŀ������
//@EXECUTIONFLOW: 	3���л�˲��������
//@EXECUTIONFLOW: ����ΪEM_UDIAUDIO_CHANNEL_LEFT�Ĳ��Բ���
//@EXECUTIONFLOW: 	1�������ǰΰ�ɫ���Ǻ�ɫ����ߣ���Ӧ�����������ӽ�Ŀ������
//@EXECUTIONFLOW: 	2���л�˲��������
//@EXECUTIONFLOW: ����ΪEM_UDIAUDIO_CHANNEL_RIGHT�Ĳ��Բ���
//@EXECUTIONFLOW: 	1�������ǰΰ�ɫ���Ǻ�ɫ����ߣ���Ӧ���������㲥��Ŀ������
//@EXECUTIONFLOW: 	2���л�˲��������
//@EXECUTIONFLOW: ����ΪEM_UDIAUDIO_CHANNEL_MIXED_MONO�Ĳ��Բ���
//@EXECUTIONFLOW: 	1�����ܰε���ɫ���ǰ�ɫ����ߣ���֤һ���������Ч���������������ӽ�Ŀ�͹㲥��Ŀ�Ļ����
//@EXECUTIONFLOW: 	2���л�˲��������
//@EXECUTIONFLOW: 4��ֹͣnAudioIndex����
CSUDI_BOOL CSTC_AUDIO_SetChannel_0002(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int jj;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play ������ failure!!!!"
		);

	CSTCPrint("Step 2 start play ������\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play ������ failure!!!!"
		);
	
	for(jj=0;jj < sizeof(s_sAudioChannels)/sizeof(AudioTestChannel_S); jj++)
	{
		CSTCPrint("����ΪEM_UDIAUDIO_CHANNEL_%s�Ĳ��Բ���\n",s_sAudioChannels[jj].m_cChannelInfo);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(AUDIO_INDEX_0,s_sAudioChannels[jj].m_eChannel)),
				"step 2 set Channel failure!!!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[jj],sOutputCfg.uAudOutput)),
				"step 3 check Channel failure!!!!"
			);
	}
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
				"Step 6 stop play failure !!!!"
			);
	}
	hPlayer = CSUDI_NULL;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: ����һ��������MPEG���������ó����������ڵ�����ʱ����Ϊ������������֮һ�ļ�����Ϊ������
//@DESCRIPTION: 1.��Ӧ����������������ҵ��ӻ��������쳣����
//@DESCRIPTION: 2.�ô��ڵ����������滻�����ڵ�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ������Ƶ���ͨ��ΪEM_UDIAOUT_RCA(������Ƶ�����������)
//@INPUT: 1��nAudioIndex = 0
//@INPUT: 2��eChannelType = ϵͳ֧�ֵ�����CSUDIAUDIOChannelType_E
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������������TBD �������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������"TBD",�������ųɹ�
//@EXECUTIONFLOW: 3��ȷ�ϲ��ųɹ��󣬱�������֧�ֵ���������CSUDIAUDIOSetChannel(nAudioIndex��eChannelType)
//@EXECUTIONFLOW: ����ΪEM_UDIAUDIO_CHANNEL_STEREO�Ĳ��Բ���
//@EXECUTIONFLOW: 4��ֹͣnAudioIndex����
CSUDI_BOOL CSTC_AUDIO_SetChannel_0003(void)
{
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIAUDIOMute(int nAudioIndex)
//@DESCRIPTION: ����nAudioIndexΪ�Ƿ�ֵ�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT: nAudioIndex = -1��nAudio��nAudio+1(nAudioΪϵͳ֧�ֵĸ���)
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount�õ�Audio����ĿnAudio�������ɹ���ȡϵͳ֧�ֵĸ���
//@EXECUTIONFLOW: 2������CSUDIAUDIOMute(-1)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAUDIOMute(nAudio)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAUDIOMute(nAudio+1)���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_Mute_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOMute(-1)),
			"Step 2 set mute with -1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOMute(nAudioDecCount)),
			"Step 3 set mute with nAudioDecCount index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOMute(nAudioDecCount+1)),
			"Step 4 set mute with nAudioDecCount+1 index should be Failure !!!"
		);
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}
//@CASEGROUP:CSUDIAUDIOUnmute(int nAudioIndex)
//@DESCRIPTION: ����nAudioIndexΪ�Ƿ�ֵ�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT: nAudioIndex =  -1��nAudio��nAudio+1(nAudioΪϵͳ֧�ֵĸ���)
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount�õ�Audio����ĿnAudio�������ɹ���ȡϵͳ֧�ֵĸ���
//@EXECUTIONFLOW: 2������CSUDIAUDIOUnmute(-1)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAUDIOUnmute(nAudio)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAUDIOUnmute(nAudio+1)���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_Unmute_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOUnmute(-1)),
			"Step 2 set unmute with -1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOUnmute(nAudioDecCount)),
			"Step 3 set unmute with nAudioDecCount index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOUnmute(nAudioDecCount+1)),
			"Step 4 set unmute with nAudioDecCount+1 index should be Failure !!!"
		);
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: ����nAudioIndex��nVolumeΪ�Ƿ�ֵ�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT: 1��nAudioIndex = -1��nAudio��nAudio+1(nAudioΪϵͳ֧�ֵĸ���)
//@INPUT: 2��nVolume = -1��32 (����[0,31]Ϊ�Ϸ�ֵ)
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������CSUDIAUDIOGetCount�õ�Audio����ĿnAudio�������ɹ���ȡϵͳ֧�ֵĸ���
//@EXECUTIONFLOW: 2������CSUDIAUDIOSetVolume(-1,0)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAUDIOSetVolume(nAudio,0)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAUDIOSetVolume(nAudio+1,0)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIAUDIOSetVolume(0��-1)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIAUDIOSetVolume(0��32)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIAUDIOSetVolume(-1��-1)���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIAUDIOSetVolume(-1��32)���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_SetVolume_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(-1,0)),
			"Step 2 set Volume with -1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(nAudioDecCount,0)),
			"Step 3 set Volume with nAudioDecCount index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(nAudioDecCount+1,0)),
			"Step 4 set Volume with nAudioDecCount+1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(0,-1)),
			"Step 5 set Volume with Volume -1 should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(0,32)),
			"Step 4 set Volume with audio_index 0 and volume 32 should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(-1,-1)),
			"Step 4 set Volume with audio_index -1 and volume -1 should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(-1,32)),
			"Step 4 set Volume with audio_index -1 and volume 32 should be Failure !!!"
		);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: �����������������Ĺ���
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: 1��nAudioIndex = 0
//@INPUT: 2��nVolume = ��0��ʼ������Ч������Χ��ÿ�ε���5����λ
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������"������",�������ųɹ�
//@EXECUTIONFLOW: 3����nVolume=0��ʼ,����Ч������Χ��,nVolumeÿ200ms����1,����CSUDIAUDIOSetVolume(nAudioIndex,nVolume)������ÿ�ε��óɹ�
//@EXECUTIONFLOW: 4��У��ÿ�ε�������ֵ�������������𽥱��(0Ϊ��С����,31Ϊ�������)
//@EXECUTIONFLOW: 5��ֹͣ������Ƶ���������кϷ������豸�����������������������
//@EXECUTIONFLOW: 6���ظ�����2��5 N(N=5)�Σ�����ÿһ���趼�õ�����ֵ
//@REMARK:�Ƿ���Ҫ������������ֵ?
CSUDI_BOOL CSTC_AUDIO_SetVolume_0002(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int jj;
	int nLoop;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play ������ failure!!!!"
		);

	CSTCPrint("Step 2 start play ������\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play ������ failure!!!!"
		);

	CSTCPrint("ע��۲����¹����������豸����������!!\r\n");
	CSTKWaitAnyKey();	
	
	for(nLoop = 0; nLoop < AUDIO_TEST_TIMES; nLoop++)
	{
		for(jj = 0;jj<32;jj += 1)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,jj)),
					"Step 4 set Volume Failure !!!"
				);
			
			CSTCPrint("The current volume is : %d \r\n",jj);
			CSUDIOSThreadSleep(200);
		}
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,0)),
				"Step 4 set Volume Failure !!!"
			);
		CSTCPrint("The current volume is : 0 \r\n");
		CSUDIOSThreadSleep(1000);
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
			"Step 5 stop play failure !!!!"
		);
	hPlayer = CSUDI_NULL;
	
	CSTCPrint("�������������������豸����������ֹͣ���ż�û������!!\r\n");
	CSTCPrint("0:Yes,������������ͬ .\r\n");
	CSTCPrint("1:No,����������ͬ.\r\n");
	bRet = CSTKWaitYes();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"Step 4 check volume failure !!!!"
		);
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
				"Step 5 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: ����������С�����Ĺ���
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: 1��nAudioIndex = 0
//@INPUT: 2��nVolume = ��31��ʼ������Ч������Χ��ÿ�εݼ�5����λ
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������"������",�������ųɹ�
//@EXECUTIONFLOW: 3����nVolume=31��ʼ,����Ч������Χ��,nVolumeÿ200ms�ݼ�1,����CSUDIAUDIOSetVolume(nAudioIndex,nVolume)������ÿ�ε��óɹ�
//@EXECUTIONFLOW: 4��У��ÿ�εݼ�����ֵ�������������𽥱�С(0Ϊ��С����,31Ϊ�������)
//@EXECUTIONFLOW: 5��ֹͣ������Ƶ���������кϷ������豸�����������������������
//@EXECUTIONFLOW: 6���ظ�����2��5 N(N=5)�Σ�����ÿһ���趼�õ�����ֵ
//@REMARK:�Ƿ���Ҫ������������ֵ?
CSUDI_BOOL CSTC_AUDIO_SetVolume_0003(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int jj;
	int nLoop;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play ������ failure!!!!"
		);

	CSTCPrint("Step 2 start play ������\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"check 2 check play ������ failure!!!!"
		);

	CSTCPrint("ע��۲����¹����������豸�����𽥼�С!!\r\n");
	CSTKWaitAnyKey();	
	
	for(nLoop = 0; nLoop < AUDIO_TEST_TIMES; nLoop++)
	{

		for(jj = 31;jj>=0;jj -= 1)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,jj)),
					"Step 4 set Volume Failure !!!"
				);
			CSTCPrint("The current volume is : %d \r\n",jj);
			CSUDIOSThreadSleep(200);
		}	
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,31)),
				"Step 4 set Volume Failure !!!"
			);
		CSTCPrint("The current volume is : 31 \r\n");
		CSUDIOSThreadSleep(1000);
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
			"Step 5 stop play failure !!!!"
		);
	hPlayer = CSUDI_NULL;
	CSTCPrint("�������������������豸�����𽥼�С��ֹͣ���ż�û������!!\r\n");
	CSTCPrint("0:Yes,������������ͬ .\r\n");
	CSTCPrint("1:No,����������ͬ.\r\n");
	bRet = CSTKWaitYes();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"Step 4 check volume failure !!!!"
		);
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
				"Step 5 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: ����������0���������������������ȷ��������
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: 1��nAudioIndex = 0
//@INPUT: 2��nVolume = 31,0
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������"������",�������ųɹ�
//@EXECUTIONFLOW: 3��nVolume=31����CSUDIAUDIOSetVolume���������óɹ�,������������
//@EXECUTIONFLOW: 4��nVolume=0����CSUDIAUDIOSetVolume���������豸����������С���������óɹ��������������������������������ͱ���
//@EXECUTIONFLOW: 5���ָ��ֳ�	
//@REMARK:
CSUDI_BOOL CSTC_AUDIO_SetVolume_0004(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play ������ failure!!!!"
		);

	CSTCPrint("Step 2 start play ������\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play ������ failure!!!!"
		);
	
	//EXECUTIONFLOW: 3��nVolume=31����CSUDIAUDIOSetVolume���������óɹ�,������������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,CSUDI_MAX_VOLUME)),
			"Step 3 set Volume Failure !!!"
		);
	
	CSTCPrint("Step 3 �뽫�����豸����������󣬲�ע�������豸���\r\n");
	CSTKWaitAnyKey();	

	//EXECUTIONFLOW: 4��nVolume=0����CSUDIAUDIOSetVolume���������豸����������С���������óɹ��������������������������������ͱ���
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,0)),
			"Step 4 set Volume Failure !!!"
		);	

	CSTCPrint("Step 4 �������������������豸�ޱ����������쳣�����������!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 4 check result failure!");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
			"Step 5 stop play failure !!!!"
		);
	hPlayer = CSUDI_NULL;	

	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
				"Step 5 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	
	CSUDIAUDIOSetVolume(AUDIO_INDEX_0,AUDIO_DEFAULT_VOLUME);
	return CSUDI_TRUE;
}


//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@DESCRIPTION: ����������������ȡ����������
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT: nAudioIndex = 0
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2����nAudio�ϲ���"������"���������ųɹ�
//@EXECUTIONFLOW: 3������CSUDIAUDIOMute(nAudio)���þ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ȴ�ȷ��:�������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 5������CSUDIAUDIOUnmute(nAudio)ȡ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6���ȴ�ȷ��:�������кϷ��豸���ָ��������,����˲��������
//@EXECUTIONFLOW: 7���ظ�����3��6 N(N=5)�Σ�����ÿһ���趼�õ�����ֵ
//@EXECUTIONFLOW: 8��ֹͣnAudioIndex����
CSUDI_BOOL CSTC_AUDIO_MuteAndUnmute_0001(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;	
	AudioTestOutputCfg_S sOutputCfg;
	int nLoop = 0;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_AC3 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play ������ failure!!!!"
		);

	CSTCPrint("Step 2 start play ������\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play ������ failure!!!!"
		);
	
	for(nLoop = 0; nLoop < AUDIO_TEST_TIMES; nLoop++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
				"step 3 set mute failure !!!!"
			);
		CSTCPrint("step 3 The current status: [mute] [stereo] [31]\r\n");
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
				"Step 4 check result failure !!!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
				"step 5 set mute failure !!!!"
			);
		CSTCPrint("step 5 The current status: [unmute] [stereo] [31]\r\n");
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
				"Step 6 check result failure !!!!"
			);
	}
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 8 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}



//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3��CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: �������Ǿ������������������л�ʱ����ϲ���
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: nAudioIndex = 0
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: ��ʼֵΪ:(����31+�Ǿ���) ����+����31 -> �Ǿ���+����0 -> ����31+�Ǿ��� -> ����+����31-> ֹͣ����
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2����nAudioIndex�ϲ���"������"���������ųɹ�
//@EXECUTIONFLOW: ����+����31
//@EXECUTIONFLOW: 3������CSUDIAUDIOMute(nAudioIndex)���þ�������������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 4������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: �Ǿ���+����0
//@EXECUTIONFLOW: 5������CSUDIAUDIOUnmute(nAudioIndex)���÷Ǿ�������������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 6������CSUDIAUDIOSetVolume(nAudioIndex,0)����������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: ����31+����
//@EXECUTIONFLOW: 7������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 8������CSUDIAUDIOMute(nAudioIndex)����������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: �Ǿ���+����31
//@EXECUTIONFLOW: 9������CSUDIAUDIOUnmute(nAudio)����������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 10������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 11��ֹͣnAudioIndex����
CSUDI_BOOL CSTC_AUDIO_IT_0001(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;	
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play ������ failure!!!!"
		);

	CSTCPrint("step 2 start play ������\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play ������ failure!!!!"
		);
	
	//����+����31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 3 set mute failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 4 set volume 31 failure !!!!"
		);
	CSTCPrint("step 3��4 The current status: [%s] volume=%d \n","mute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 4 check result failure !!!!"
		);
	
	//�Ǿ���+����0
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 5 set unmute failure !!!!"
		);
	
	CSTCPrint("step 5 The current status: [%s] volume=%d \n","unmute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 5 check result failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 6 set volume  0 failure !!!!"
		);
	
	CSTCPrint("step 6 The current status: [%s] volume=%d \n","unmute",0);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 6 check result failure !!!!"
		);
	
	//����31+����
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 7 set volume  31 failure !!!!"
		);
	CSTCPrint("step 7 The current status: [%s] volume=%d \n","unmute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 7 check result failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 8 set mute failure !!!!"
		);
	CSTCPrint("step 8 The current status: [%s] volume=%d \n","mute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 8 check result failure !!!!"
		);
	
	//�Ǿ���+����31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 9 set unmute failure !!!!"
		);
	CSTCPrint("step 9 The current status: [%s] volume=%d \n","unmute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 9 check result failure !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 10 set volume  31 failure !!!!"
		);
	CSTCPrint("step 10 The current status: [%s] volume=%d \n","unmute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 10 check result failure !!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 11 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}
//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3��CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: �������������������л�ʱ����ϲ���
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: nAudioIndex = ֧��MPG2���͵���Ƶ������
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: ��ʼֵΪ(��������+����31):����+��+31 ->��ͣ���� -> ��+31+�ָ����� -> ������+31 -> ���+31 -> ֹͣ����
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������"������",�������ųɹ������кϷ��豸��ͬʱ�����㲥�͵��ӵ�����
//@EXECUTIONFLOW: ����+��+31
//@EXECUTIONFLOW: 3������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)��������������������CSUDI_SUCCESS�����������кϷ��豸Ӧ�����������ӽ�Ŀ������
//@EXECUTIONFLOW: 4������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS�����������кϷ��豸Ӧ�����������ӽ�Ŀ������
//@EXECUTIONFLOW: ��ͣ���� 
//@EXECUTIONFLOW: 5����ͣ����,���������豸�����������
//@EXECUTIONFLOW: ��+31+�ָ�����  
//@EXECUTIONFLOW: 6������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)��������������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 8���ָ����ţ����������кϷ��豸���������㲥��Ŀ������
//@EXECUTIONFLOW: ������+31
//@EXECUTIONFLOW: 9������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS�����кϷ��豸��ͬʱ�����㲥�͵��ӵ�����
//@EXECUTIONFLOW: ���+31
//@EXECUTIONFLOW: 11������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 12������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS�����кϷ��豸���������ӽ�Ŀ�͹㲥��Ŀ�Ļ����
//@EXECUTIONFLOW: 13��ֹͣnAudioIndex����
CSUDI_BOOL CSTC_AUDIO_IT_0002(void)
{
	int nAudioIndex = -1;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	
	//����+��+31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play ������ failure!!!!"
		);
	
	CSTCPrint("step 2 start play ������\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play ������ failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)),
			"step 3 set Channel LEFT failure!!!!"
		);
	CSTCPrint("step 3 The current status: [left] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_LEFT],sOutputCfg.uAudOutput)),
			"step 3 check Channel failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 4 set volume 31 failure!!!!"
		);
	CSTCPrint("step 3��4 The current status: [left] [31] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 3��4 check result failure !!!!"
		);

	nRet = CSUDIPLAYERPause(hPlayer);
	
	//��ͣ���� 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == nRet),
			"step 5 pause play failure!!!!"
		);

	CSTCPrint("step 5 The current status: [pause] [left] [31] \n");

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
		"Step 5 check result failure !!!!"
	);
	
	//��+31+�ָ����� 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 6 set Channel RIGHT failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 7 set volume 31 failure!!!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)),
		"step 8 resume play failure!!!!"
	);

	CSTCPrint("step 6��7��8 The current status: [resume] [right] [31] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_RIGHT],sOutputCfg.uAudOutput)),
			"step 6��7��8 check Channel failure!!!!"
		);
	
	//������+31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)),
			"step 9 set Channel STEREO failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 10 set volume 31 failure!!!!"
		);
	CSTCPrint("step 9��10 The current status: [stereo] [31] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_STEREO],sOutputCfg.uAudOutput)),
			"step 9��10 check Channel failure!!!!"
		);

	//���+31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)),
			"step 11 set Channel STEREO failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 12 set volume 31 failure!!!!"
		);
	CSTCPrint("step 11��12 The current status: [mono] [31] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_MIXED_MONO],sOutputCfg.uAudOutput)),
			"step 11��12 check Channel failure!!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 13 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}
//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3��CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: �������������������л�ʱ����ϲ���(��)
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: nAudioIndex = ֧��MPG2���͵���Ƶ������
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: ��ʼֵΪ(��������+����31):��ͣ����+��+0 -> �ָ����� -> ��+0 -> ��+31 -> ��ͣ����->����+31+�ָ�����
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������"��������������",�������ųɹ������кϷ��豸��ͬʱ�����㲥�͵��ӵ�����
//@EXECUTIONFLOW: ��ͣ����+��+0
//@EXECUTIONFLOW: 3����ͣ����,���������豸�����������
//@EXECUTIONFLOW: 4������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)��������������������CSUDI_SUCCESS���������������
//@EXECUTIONFLOW: 5������CSUDIAUDIOSetVolume(nAudioIndex,0)����������CSUDI_SUCCESS�����������кϷ��豸�����������
//@EXECUTIONFLOW: �ָ����� 
//@EXECUTIONFLOW: 6���ָ�����,�������кϷ��豸���������
//@EXECUTIONFLOW: ��+0 
//@EXECUTIONFLOW: 7������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)��������������������CSUDI_SUCCESS,���������кϷ��豸�����������
//@EXECUTIONFLOW: 8������CSUDIAUDIOSetVolume(nAudioIndex,0)����������CSUDI_SUCCESS,���������кϷ��豸�����������
//@EXECUTIONFLOW: ��+31 
//@EXECUTIONFLOW: 9������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)��������������������CSUDI_SUCCESS,���������кϷ��豸�����������
//@EXECUTIONFLOW: 10������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS�����������кϷ��豸Ӧ�����������ӽ�Ŀ������
//@EXECUTIONFLOW: ��ͣ����
//@EXECUTIONFLOW: 11����ͣ����,���ƽ̨֧��ֱ���������ͣ,�����������豸�����������
//@EXECUTIONFLOW: ����+31+�ָ�����
//@EXECUTIONFLOW: 12������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)����������������������CSUDI_SUCCESS����������CSUDI_SUCCESS,���������кϷ��豸�����������
//@EXECUTIONFLOW: 13������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS����������CSUDI_SUCCESS,���������кϷ��豸�����������
//@EXECUTIONFLOW: 14���ָ����ţ����кϷ��豸���������ӽ�Ŀ�͹㲥��Ŀ�Ļ����
//@EXECUTIONFLOW: 15��ֹͣnAudioIndex����
CSUDI_BOOL CSTC_AUDIO_IT_0003(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_BOOL bRet;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 2 start play �������������� failure!!!!"
		);

	CSTCPrint("step 2 start play ��������������\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play �������������� failure!!!!"
		);
	
	//��ͣ����+��+0
	nRet = CSUDIPLAYERPause(hPlayer);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == nRet),
			"step 3 pause play failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)),
			"step 4 set Channel RIGHT failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 5 set volume 0 failure!!!!"
		);
	
	CSTCPrint("Step 3��4��5 The current status: [pause] [left] [volume=%d] \n",0);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 3��4��5 check result failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)),
		"step 6 resume play failure!!!!"
	);
	
	CSTCPrint("step 6 The current status: [resume] [left] [volume=%d] \n",0);
	CSTCPrint("�������кϷ��豸�����������,����˲��������...........!!\r\n");
	CSTCPrint("0:Yes,������������ͬ .\r\n");
	CSTCPrint("1:No,����������ͬ.\r\n");
	bRet = CSTKWaitYes();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"Step 6 check result failure !!!!"
		);
	
	//��+0 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 7 set Channel RIGHT failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 8 set volume 31 failure!!!!"
		);
	CSTCPrint("step 7��8 The current status: [play] [right] [volume=%d] \n",0);
	CSTCPrint("�������кϷ��豸�����������,����˲��������...........!!\r\n");
	CSTCPrint("0:Yes,������������ͬ .\r\n");
	CSTCPrint("1:No,����������ͬ.\r\n");
	bRet = CSTKWaitYes();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"Step 7��8 check result failure !!!!"
		);
	
	//��+31 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 9 set Channel RIGHT failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 10 set volume 31 failure!!!!"
		);	
	
	CSTCPrint("step 9��10 The current status: [play] [right] [volume=%d] \n",31);
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_RIGHT],sOutputCfg.uAudOutput)),
		"step 9��10 check Channel failure!!!!"
	);
	
	//��ͣ����
	nRet = CSUDIPLAYERPause(hPlayer);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == nRet),
			"step 11 pause play failure!!!!"
		);
	
	CSTCPrint("step 11 The current status: [pause] [right] [volume=%d] \n",31);

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
		"Step 11 check result failure !!!!"
	);
	
	//����+31+�ָ����� 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)),
			"step 12 set Channel STEREO failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 13 set volume 31 failure!!!!"
		);	

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)),
		"step 14 resume play failure!!!!"
	);
	
	CSTCPrint("step 12��13��14 The current status: [resume] [stereo] [volume=%d] \n",31);	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_STEREO],sOutputCfg.uAudOutput)),
		"step 12��13��14 check Channel failure!!!!"
	);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 15 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3��CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: �����;������������л�ʱ����ϲ���
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: nAudioIndex = ֧��MPG2���͵���Ƶ������
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: ��ʼֵΪ(��������+�Ǿ���+31):����+�� -> �Ǿ���+�� -> �Ǿ���+�� ->����+��->����+����->�Ǿ���+����->�Ǿ���+���->����+���
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������"��������������",�������ųɹ������кϷ��豸��ͬʱ�����㲥�͵��ӵ�����
//@EXECUTIONFLOW: ����+�� 
//@EXECUTIONFLOW: 3������CSUDIAUDIOMute(nAudioIndex)���þ�������������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 4������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)��������������������CSUDI_SUCCESS�����������кϷ��豸�����������
//@EXECUTIONFLOW: �Ǿ���+��
//@EXECUTIONFLOW: 5������CSUDIAUDIOUnmute(nAudioIndex)����������CSUDI_SUCCESS�����������кϷ��豸���������ӽ�Ŀ������
//@EXECUTIONFLOW: 6������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)��������������������CSUDI_SUCCESS�����������кϷ��豸Ӧ�����������ӽ�Ŀ������
//@EXECUTIONFLOW: �Ǿ���+��
//@EXECUTIONFLOW: 7������CSUDIAUDIOUnmute(nAudioIndex)����������CSUDI_SUCCESS�����������кϷ��豸���������ӽ�Ŀ������
//@EXECUTIONFLOW: 8������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)��������������������CSUDI_SUCCESS,���������кϷ��豸���������㲥��Ŀ������
//@EXECUTIONFLOW: ����+��
//@EXECUTIONFLOW: 9������CSUDIAUDIOMute(nAudioIndex)���þ�������������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 10������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)��������������������CSUDI_SUCCESS�����������豸�����������
//@EXECUTIONFLOW: ����+����
//@EXECUTIONFLOW: 11������CSUDIAUDIOMute(nAudioIndex)���þ�������������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 12������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)����������������������CSUDI_SUCCESS�����������豸�����������
//@EXECUTIONFLOW: �Ǿ���+����
//@EXECUTIONFLOW: 13������CSUDIAUDIOUnmute(nAudioIndex)����������CSUDI_SUCCESS�����������кϷ��豸��ͬʱ�����㲥�͵��ӵ�����
//@EXECUTIONFLOW: 14������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)����������������������CSUDI_SUCCESS�����кϷ��豸��ͬʱ�����㲥�͵��ӵ�����
//@EXECUTIONFLOW: �Ǿ���+���
//@EXECUTIONFLOW: 15������CSUDIAUDIOUnmute(nAudioIndex)����������CSUDI_SUCCESS�����������кϷ��豸���������ӽ�Ŀ�͹㲥��Ŀ�Ļ����
//@EXECUTIONFLOW: 16������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)���û����������������CSUDI_SUCCESS�����кϷ��豸���������ӽ�Ŀ�͹㲥��Ŀ�Ļ����
//@EXECUTIONFLOW: ����+���
//@EXECUTIONFLOW: 17������CSUDIAUDIOUnmute(nAudioIndex)����������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 18������CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)���û����������������CSUDI_SUCCESS�����������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 19��ֹͣnAudioIndex����
CSUDI_BOOL CSTC_AUDIO_IT_0004(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 2 start play �������������� failure!!!!"
		);

	CSTCPrint("step 2 start play ��������������\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play �������������� failure!!!!"
		);
	
	//����+�� 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 3 set mute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)),
			"step 4 set Channel LEFT failure!!!!"
		);
	CSTCPrint("step 3��4 The current status: [mute] [left] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 3��4 check result failure !!!!"
		);
	
	//�Ǿ���+��
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 5 set unmute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)),
			"step 6 set Channel LEFT failure!!!!"
		);
	CSTCPrint("step 5��6 The current status: [unmute] [left] \n");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_LEFT],sOutputCfg.uAudOutput)),
		"step 5��6 check Channel failure!!!!"
	);
	
	//�Ǿ���+��
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 7 set unmute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 8 set Channel RIGHT failure!!!!"
		);
	CSTCPrint("step 7��8 The current status: [unmute] [right] \n");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_RIGHT],sOutputCfg.uAudOutput)),
		"step 7��8 check Channel failure!!!!"
	);
	
	//����+��	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 9 set mute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 10 set Channel RIGHT failure!!!!"
		);
	CSTCPrint("step 9��10 The current status: [mute] [right] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 9��10 check result failure !!!!"
		);
	
	//����+����
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 11 set mute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)),
			"step 12 set Channel STEREO failure!!!!"
		);
	CSTCPrint("step 11��12 The current status: [mute] [stereo] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 11��12 check result failure !!!!"
		);
	
	//�Ǿ���+����
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 13 set Unmute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)),
			"step 14 set Channel STEREO failure!!!!"
		);
	CSTCPrint("step 13��14 The current status: [unmute] [stereo] \n");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_STEREO],sOutputCfg.uAudOutput)),
		"step 13��14  check Channel failure!!!!"
	);

	//�Ǿ���+���
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 15 set Unmute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)),
			"step 16 set Channel MIXED_MONO failure!!!!"
		);
	CSTCPrint("step 15��16 The current status: [unmute] [MONO] \n");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_MIXED_MONO],sOutputCfg.uAudOutput)),
		"step 15��16  check Channel failure!!!!"
	);

	//����+���
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 17 set mute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)),
			"step 18 set Channel STEREO failure!!!!"
		);
	CSTCPrint("step 17��18 The current status: [mute] [MONO] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 17��18 check result failure !!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 11 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3��CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@CASEGROUP:4��CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice, CSUDIAUDIOMode_E eMode)
//@DESCRIPTION: ��ϲ��Բ���DD��DD+��Ŀʱ����Ƶ�Ľ���ģʽͬ�������õĶ�Ӧ��ϵ
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@PRECONDITION: 2��ϵͳ������ƵSPDIF���
//@INPUT: nAudioIndex = 0
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: ��ʼֵΪ(EM_UDIAUDIO_DECODE+31):decode+AC3+����31 -> bypass+AC3+����31 -> bypass+AC3+����0 ->decode+MEPG2+����31
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIAUDIOGetCount��CSUDIAUDIOGetCapability��ȡ֧��AC3����decodeģʽ����Ƶ����������ֵnAudioIndex,�����֧��AC3���룬��ִ�в���6
//@EXECUTIONFLOW: 3������CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAUDIO_DECODE)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5����nAudioIndex�ϲ���"DolbyAC3��׼���Խ�Ŀ"���������ųɹ�,����������豸�����������
//@EXECUTIONFLOW: 6������CSUDIAUDIOSetVolume ����nAudioIndex������Ϊ0����������CSUDI_SUCCESS�������豸���������
//@EXECUTIONFLOW: 7������CSUDIAUDIOSetVolume ����nAudioIndex������Ϊ31����������CSUDI_SUCCESS����������豸�����������
//@EXECUTIONFLOW: 8������CSUDIAOUTSetOPMode����EM_UDIAOUT_SPDIFΪBYPASS����������CSUDI_SUCCESS����������豸������������������豸���dolby5.1������˲��������
//@EXECUTIONFLOW: 9������CSUDIAUDIOSetVolume����nAudioIndex������Ϊ0����������CSUDI_SUCCESS�����������豸���������,�����豸���������
//@EXECUTIONFLOW: 10������CSUDIAUDIOMute����nAudioIndex��������������CSUDI_SUCCESS�����������豸���������
//@EXECUTIONFLOW: 11��ֹͣ���ţ����������豸�����������
//@EXECUTIONFLOW: 12������CSUDIAOUTSetOPMode����SPDIF��HDMI ΪDECODE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 13������CSUDIAUDIOSetVolume ����nAudioIndex������Ϊ31������CSUDIAUDIOUnmute����nAudioIndex�˳������� ��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 14������"������",�������ųɹ����������кϷ��豸���������
//@EXECUTIONFLOW: 15��ֹͣ���ţ����������豸�����������
//@EXECUTIONFLOW: 16���ָ��ֳ�
CSUDI_BOOL CSTC_AUDIO_IT_0005(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_BOOL bRet = CSUDI_FALSE;

	if(IsAOUTSupport(EM_UDIAOUT_SPDIF))
	{
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	//EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
	//EXECUTIONFLOW: 2������CSUDIAUDIOGetCount��CSUDIAUDIOGetCapability��ȡ֧��AC3����decodeģʽ����Ƶ����������ֵnAudioIndex,�����֧��AC3���룬��ִ�в���6
	//EXECUTIONFLOW: 3������CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAUDIO_DECODE)����������CSUDI_SUCCESS
	//EXECUTIONFLOW: 4������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS
	//EXECUTIONFLOW: 5����nAudioIndex�ϲ���"DolbyAC3��׼���Խ�Ŀ"���������ųɹ�,����������豸�����������
	bRet = AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex);
	
	if(bRet == CSUDI_FALSE) //�����֧��AC3���룬��ִ�в���6
	{
		CSTCPrint("There's no audio support EM_UDI_AUD_STREAM_AC3 !!!!\r\n");
		bRet = AUDIO_iGetAudioIndexWithBypassByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex);
		if(bRet == CSUDI_FALSE) ///�����֧��AC3bypass����ִ�в���12
			goto LABEL;
	}
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
		"step 5 start play DolbyAC3��׼���Խ�Ŀ failure!!!!"
	);

	CSTCPrint("step 5 start play DolbyAC3��׼���Խ�Ŀ\n");

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
		"step 5 check play DolbyAC3��׼���Խ�Ŀ failure!!!!"
	);

	//EXECUTIONFLOW: 6������CSUDIAUDIOSetVolume ����nAudioIndex������Ϊ0����������CSUDI_SUCCESS�������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 6 set volume 0 failure!!!!"
		);

	CSTCPrint("step 6 set volume 0 \n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 6 check result failure !!!!"
		);

	//EXECUTIONFLOW: 7������CSUDIAUDIOSetVolume ����nAudioIndex������Ϊ31����������CSUDI_SUCCESS����������豸�����������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 7 set volume 31 failure!!!!"
		);

	CSTCPrint("step 7 set volume 31 \n");
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
		"Step 7 check result failure !!!!"
	);
	
	//EXECUTIONFLOW: 8������CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAUDIO_BYPASS)��
	//��������CSUDI_SUCCESS����������豸������������������豸���dolby5.1������˲��������
	//bypass+AC3+����31
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_BYPASS)),
		"step 8 set audio decode bypass mode failure!!!!"
	);
	
	CSTCPrint("step 8 set spdif audio decode bypass mode \n");
	CSTCPrint("���������豸�����ʽΪdolby��ʽ!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
		"Step 8 check result failure !!!!"
	);

	//EXECUTIONFLOW: 9������CSUDIAUDIOSetVolume����nAudioIndex������Ϊ0����������CSUDI_SUCCESS�����������豸���������,�����豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 9 set volume 0 failure!!!!"
		);

	CSTCPrint("step 9 set volume 0\n");
	CSTCPrint("����SPDIF�����豸������������ʽΪdolby 5.1��ʽ�����豸������!!\r\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 9 check result failure !!!!"
		);
		
	//EXECUTIONFLOW: 10������CSUDIAUDIOMute����������CSUDI_SUCCESS�����������豸���������
	//bypass+AC3+����
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 10 set mute failure!!!!"
		);

	CSTCPrint("step 10 set mute \n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 10 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 11��ֹͣ���ţ����������豸�����������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
			"Step 11 stop play failure !!!!"
		);
	hPlayer = CSUDI_NULL;
	
	CSTCPrint("step 11 stop play \n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 11 check result failure !!!!"
		);
	
	//decode+MEPG1+����31	
LABEL:	
	//EXECUTIONFLOW: 12������CSUDIAOUTSetOPMode����SPDIF��HDMI ΪDECODE����������CSUDI_SUCCESS
	CSTK_ASSERT_TRUE(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_DECODE)),
			"step 12 set SPDIF output decode mode failure!!!!"
		);	
	if( IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
		CSTK_ASSERT_TRUE(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
			"step 12 set HDMI output decode mode failure!!!!");	
		CSTCPrint("step 12 set SPDIF&HDMI output decode mode \n");
	}
	
	//EXECUTIONFLOW: 13������CSUDIAUDIOSetVolume(nAudioIndex,31)����������CSUDI_SUCCESS
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 13 set volume 31 failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 13 set unmute failure!!!!"
		);
	
	CSTCPrint("step 13 set volume 31 & set unmute \n");

	//EXECUTIONFLOW: 14������"������",�������ųɹ����������кϷ��豸���������
	bRet = AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"There's no audio decode support decode mpeg  !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 14 start play ������failure!!!!"
		);

	CSTCPrint("step 14 start play ������ \n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 14 check play ������failure!!!!"
		);
	
	//EXECUTIONFLOW: 15��ֹͣ���ţ����������豸�����������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
			"Step 15 stop play failure !!!!"
		);
	
	hPlayer = CSUDI_NULL;

	CSTCPrint("step 15 stop play \n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 15 check result failure !!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 16 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("Not support SPDIF!\n");
		return CSUDI_FALSE;
	}
}



//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3��CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@CASEGROUP:4��CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice)
//@CASEGROUP:5��CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice)
//@DESCRIPTION: �л�̨����Ӱ����Ƶ״̬������������С�;�������������
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: nAudioIndex = 0
//@INPUT:��Ŀ1--"��������������"
//@INPUT:��Ŀ2-- "������" 
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIAUDIOSetChannel����nAudioIndexΪ��������������󣬲�������
//@EXECUTIONFLOW: 3������"��������������",�������ųɹ������кϷ��豸��ͬʱ����"��������������"�㲥�͵��ӵ�����
//@EXECUTIONFLOW: 4��ֹͣ����"��������������",����"������",�������ųɹ������кϷ��豸��ͬʱ����"������"�㲥�͵��ӵ�����
//@EXECUTIONFLOW: 5������CSUDIAUDIOSetChannel����nAudioIndex����������������CSUDI_SUCCESS,
//@EXECUTIONFLOW: 6��ֹͣ����"������",����"��������������",�������ųɹ������кϷ��豸������"��������������"���ӽ�Ŀ������
//@EXECUTIONFLOW: 7��ֹͣ����"��������������",����"������",�������ųɹ������кϷ��豸������"������"���ӽ�Ŀ������
//@EXECUTIONFLOW: 8������CSUDIAUDIOSetChannel����nAudioIndex����������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9��ֹͣ����"������",����"��������������",�������ųɹ������кϷ��豸������"��������������"�㲥��Ŀ������
//@EXECUTIONFLOW: 10��ֹͣ����"��������������",����"������",�������ųɹ������кϷ��豸��ͬʱ����"������"�㲥��Ŀ������
//@EXECUTIONFLOW: 11������CSUDIAUDIOSetChannel����nAudioIndex���������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 12��ֹͣ����"������",����"��������������",�������ųɹ������кϷ��豸��ͬʱ����"��������������"�㲥�͵��ӵ�����
//@EXECUTIONFLOW: 13��ֹͣ����"��������������",����"������",�������ųɹ������кϷ��豸��ͬʱ����"������"�㲥�͵��ӵ�����
//@EXECUTIONFLOW: 14������CSUDIAUDIOMute ����nAudioIndex��������������CSUDI_SUCCESS�����кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 15��ֹͣ����"��������������",����"������",�������ųɹ������кϷ��豸�����������,����˲��������
//@EXECUTIONFLOW: 16������CSUDIAUDIOUnmute����nAudioIndex�Ǿ�������������CSUDI_SUCCESS�����кϷ��豸��ͬʱ����"��������������"�㲥�͵��ӵ�����
//@EXECUTIONFLOW: 17��ֹͣnAudioIndex����
//@EXECUTIONFLOW: 18���ָ��ֳ�
CSUDI_BOOL CSTC_AUDIO_IT_0006(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_UINT32 uAudioChannelTyp = EM_UDIAUDIO_CHANNEL_STEREO;
	int nLoop;
	int nAudioIndex = -1;
	int nCurServiceInx = -1;
	int nlastServiceInx = -1;
	int nStep = 0;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	nAudioIndex = AUDIO_INDEX_0;
	
	//EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��	
	//EXECUTIONFLOW: 2���������Ϊ��������������󣬲�������
	//"������"
	nCurServiceInx = EM_AUDIO_MULTILANGUAGE1;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play failure!!!!"
		);
	
	CSTCPrint("Step 1 start play %s\n", s_sAudioServices[nCurServiceInx].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg)),
			"check 1 start play failure!!!!"
		);
	
	++nStep ;
	
	//nCurServiceInx = EM_AUDIO_MULTILANGUAGE1;
	for(uAudioChannelTyp=0;uAudioChannelTyp < sizeof(s_sAudioChannels)/sizeof(AudioTestChannel_S); uAudioChannelTyp++)
	{
		++nStep;	
		CSTCPrint("Step %d set %s channel\n",nStep,s_sAudioChannels[uAudioChannelTyp].m_cChannelInfo);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,s_sAudioChannels[uAudioChannelTyp].m_eChannel)),
				"set Channel failure!!!!"
			);
		
		for(nLoop=0; nLoop<2; nLoop++)
		{
			if(hPlayer != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_FALSE)),
						"stop play failure !!!!"
					);
				hPlayer = CSUDI_NULL;
			}		
			
			nlastServiceInx = nCurServiceInx;		
			nCurServiceInx = AUDIO_iGetNextServiceInx(nlastServiceInx);
			++nStep;				
			CSTCPrint("Step %d play %s \n",nStep,s_sAudioServices[nCurServiceInx].m_pcServiceDescript);

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg,&hPlayer,CSUDI_FALSE)),
					"start play failure!!!!"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg)),
					"check play failure!!!!"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[uAudioChannelTyp],sOutputCfg.uAudOutput)),
					"check Channel failure!!!!"
				);	
		}		
	}

	uAudioChannelTyp = (sizeof(s_sAudioChannels)/sizeof(AudioTestChannel_S)-1);

	CSTCPrint("Step 14 mute audio decode\n");	

	//nStep = 13	
	//EXECUTIONFLOW: 14������CSUDIAUDIOMute ����nAudioIndex��������������CSUDI_SUCCESS�����кϷ��豸�����������,����˲��������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"Step 14 mute audio decode failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
		"Step 14 check result failure !!!!"
	);
	
	//EXECUTIONFLOW: 15��ֹͣ����"��������������",����"������",�������ųɹ������кϷ��豸�����������,����˲��������
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_FALSE)),
				"Step 15 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}		

	nCurServiceInx = EM_AUDIO_MULTILANGUAGE1;
	CSTCPrint("Step 15 play %s \n",s_sAudioServices[nCurServiceInx].m_pcServiceDescript);	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg,&hPlayer,CSUDI_FALSE)),
			"Step 15 start play failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
		"Step 15 check result failure !!!!"
	);

	//EXECUTIONFLOW: 16������CSUDIAUDIOUnmute����nAudioIndex�Ǿ�������������CSUDI_SUCCESS�����кϷ��豸��ͬʱ����"��������������"�㲥�͵��ӵ�����
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 16 unmoute audio decode  failure!!!!"
		);

	//�ָ��������鵱ǰ�����Ƿ��������������
	CSTCPrint("Step 16 play unmute %s \n",s_sAudioServices[nCurServiceInx].m_pcServiceDescript);

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
		"Step 16 check result failure !!!!"
	);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[uAudioChannelTyp],sOutputCfg.uAudOutput)),
			"Step 16 check Channel failure!!!!"
		);	

	//EXECUTIONFLOW: 17��ֹͣnAudioIndex����	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 17 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	
	//EXECUTIONFLOW: 18���ָ��ֳ�
	AUDIO_iSetDefault(nAudioIndex);
	
	return CSUDI_TRUE;
}


//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3��CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@CASEGROUP:4��CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@CASEGROUP:5��CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice, CSUDIAUDIOMode_E eMode)
//@DESCRIPTION:DD��DD+����bypass���ʱ��HDMI�ӿڣ�HDMI�豸֧��DD��DD+���룩��SPDIF�ӿ�
//@DESCRIPTION:������������Ҳû��Ч��������������Ч����
//@DESCRIPTION:���þ����ͽ��������Ч��CSUDIAUDIOMute��CSUDIAUDIOUnmute��CSUDIAOUTMute��CSUDIAOUTUnmute��
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�����������Ƶ��
//@INPUT: nAudioIndex = 0
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2����nAudioIndex=0�ϲ���"DolbyAC3��׼����"������EM_UDIAOUT_SPDIF��HDMI���ģʽΪBYPASS,�������ųɹ�
//@EXECUTIONFLOW: 3����nVolume=0��ʼ,����Ч������Χ��,nVolumeÿ200ms������1,����CSUDIAUDIOSetVolume(nAudioIndex,nVolume)������CSUDIAUDIOSetChannel���������������ͣ�����ÿ�ε��óɹ�
//@EXECUTIONFLOW: 4��У��ÿ�ε�������ֵ������SPDIF��HDMI�������û���κα仯��RCA����������𽥱��(0Ϊ��С����,31Ϊ�������)
//@EXECUTIONFLOW: 5������CSUDIAUDIOMute(nAudioIndex),�������кϷ������豸���������
//@EXECUTIONFLOW: 6������CSUDIAUDIOUnmute(nAudioIndex),�������кϷ������豸���������
//@EXECUTIONFLOW: 7��ֹͣ���β��ţ��ͷ�player
//@EXECUTIONFLOW: 8����ȡ֧��DolbyAC3�Ľ�����nAudioIndex����nAudioIndex�ϲ���"DolbyAC3��׼����"���������ųɹ�
//@EXECUTIONFLOW: 9������SPDIF��HDMI���ģʽΪDECODE,�������кϷ������豸���������
//@EXECUTIONFLOW: 10������CSUDIAUDIOSetVolume����������Ϊ0,�������кϷ������豸���������
//@EXECUTIONFLOW: 11������CSUDIAUDIOSetVolume����������Ϊ31(���),�������кϷ������豸���������
//@EXECUTIONFLOW: 12������CSUDIAUDIOMute(nAudioIndex),�������кϷ������豸���������
//@EXECUTIONFLOW: 13������CSUDIAUDIOUnmute(nAudioIndex),�������кϷ������豸���������
//@EXECUTIONFLOW: 14������CSUDIAUDIOMute(nAudioIndex),����CSUDIAUDIOSetVolume����������Ϊ0,�������кϷ������豸���������
//@EXECUTIONFLOW: 15������CSUDIAUDIOSetVolume����������Ϊ31(���),�������кϷ������豸���������
//@EXECUTIONFLOW: 16������CSUDIAUDIOUnmute(nAudioIndex),�������кϷ������豸���������
//@EXECUTIONFLOW: 17���ָ��ֳ�	
CSUDI_BOOL CSTC_AUDIO_AOUT_IT_0001(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_UINT32 uAudioChannelTyp = EM_UDIAUDIO_CHANNEL_STEREO;
	int jj;
	int nLoop;
	int nAudioIndex = -1;
	
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	nAudioIndex = AUDIO_INDEX_0;
	//EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
	//EXECUTIONFLOW: 2����nAudioIndex=0�ϲ���"DolbyAC3��׼����"������EM_UDIAOUT_SPDIF���ģʽΪBYPASS,�������ųɹ�
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play DolbyAC3��׼���� failure!!!!"
		);

	CSTCPrint("step 1 start play DolbyAC3��׼����\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex ,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
			"step 1 check play DolbyAC3��׼���� failure!!!!"
		);

	CSTCPrint("step 2 set SPDIF&HDMI output bypass mode \n");

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_BYPASS)),
			"step 2.1 set SPDIF bypass mode failure!!!!"
		);
    }
	if( IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
	
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_BYPASS)),
			"step 2.2 set HDMI bypass mode failure!!!!");
	}
	
	//EXECUTIONFLOW: 3����nVolume=0��ʼ,����Ч������Χ��,nVolumeÿ200ms������1,����CSUDIAUDIOSetVolume(nAudioIndex,nVolume)������CSUDIAUDIOSetChannel���������������ͣ�����ÿ�ε��óɹ�
	CSTCPrint("step 3��4 ��ע���עSPDIF��HDMI���������RCA�豸�������!!\r\n");
	//EXECUTIONFLOW: 4��У��ÿ�ε�������ֵ������SPDIF��HDMI�������û���κα仯��RCA����������𽥱��(0Ϊ��С����,31Ϊ�������)
	CSTCPrint("��������������SPDIF��HDMI�����������û���κα仯�����������ʽһֱΪDolby��ʽ!!\r\n");
	CSTCPrint("��������������RCA�豸����������!!\r\n");

	CSTKWaitAnyKey();	
	
	for(nLoop = 0; nLoop < 2; nLoop++)
	{

		for(jj = 0;jj<CSUDI_MAX_VOLUME;jj += 1)
		{	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,jj)),
					"Step 3 set Volume Failure !!!"
				);
			CSTCPrint("The current volume is : %d \r\n",jj);
			CSUDIOSThreadSleep(200);
		}	
		
		/*��������ΪCSUDI_MAX_VOLUMEʱ�����*/
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,CSUDI_MAX_VOLUME)),
				"Step 3 set Volume Failure !!!"
			);
		CSTCPrint("The current volume is : %d \r\n",CSUDI_MAX_VOLUME);
		CSUDIOSThreadSleep(1000);

		/*����CSUDIAUDIOSetChannel��SPDIF�����Ӱ��*/
		for(uAudioChannelTyp = EM_UDIAUDIO_CHANNEL_STEREO; uAudioChannelTyp <= EM_UDIAUDIO_CHANNEL_MIXED_MONO; uAudioChannelTyp++)
		{
			CSTCPrint("The current channle type is : %d \r\n",uAudioChannelTyp);
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex ,uAudioChannelTyp)),
				"Step 3 set channel Failure !!!"
			);
			{
				char *pcAudioChannelInfo = NULL;
				AUDIO_iGetChannelInfo(&s_sAudioChannels[uAudioChannelTyp],&pcAudioChannelInfo);
				if(NULL != pcAudioChannelInfo)
				{
					CSTCPrint("The current channle type is : %s \r\n",pcAudioChannelInfo);
					CSUDIOSThreadSleep(1000);
				}
			}
		}
	}	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 4 check volume failure !!!!"
		);	
	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
		CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_DECODE);
    }
	
	CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE);
	
	CSTCPrint("step 5 mute audio decode \n");
		
	//EXECUTIONFLOW: 5������CSUDIAUDIOMute(nAudioIndex),�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 5 mute audio decode failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 5 check result failure !!!!"
		);

	CSTCPrint("step 6 unmoute audio decode \n");
	
	//EXECUTIONFLOW: 6������CSUDIAUDIOUnmute(nAudioIndex),�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 6 unmoute audio decode  failure!!!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 6 check result failure !!!!"
		);

	CSTCPrint("step 7 stop play \n");
	
	//EXECUTIONFLOW: 7��ֹͣ���β��ţ��ͷ�player
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 7 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}

	nAudioIndex = -1;
	//EXECUTIONFLOW: 8����ȡ֧��DolbyAC3�Ľ�����nAudioIndex����nAudioIndex�ϲ���"DolbyAC3��׼����"���������ųɹ�
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_AC3 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 8 start play DolbyAC3��׼���� failure!!!!"
		);

	CSTCPrint("step 8 start play DolbyAC3��׼����\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
			"step 8 check play DolbyAC3��׼���� failure!!!!"
		);

	CSTCPrint("step 9 set SPDIF&HDMI output decode mode \n");

	//EXECUTIONFLOW: 9������SPDIF��HDMI���ģʽΪDECODE,�������кϷ������豸���������
	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_DECODE)),
				"step 9 set audio decode decode mode failure!!!!");
	}
	if( IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
				"step 9 set audio decode decode mode failure!!!!");
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 9 check result failure !!!!"
		);

	CSTCPrint("step 10 The current volume is : %d \r\n",0);
	
	//EXECUTIONFLOW: 10������CSUDIAUDIOSetVolume����������Ϊ0,�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,0)),
			"Step 10 set Volume Failure !!!"
		);
	CSTCPrint("The current volume is : %d \r\n",0);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 10 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 11������CSUDIAUDIOSetVolume����������Ϊ31(���),�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,CSUDI_MAX_VOLUME)),
			"Step 11 set Volume Failure !!!"
		);
	CSTCPrint("step 11 The current volume is : %d \r\n",CSUDI_MAX_VOLUME);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 11 check result failure !!!!"
		);

	CSTCPrint("step 12 mute audio decode \n");
	//EXECUTIONFLOW: 12������CSUDIAUDIOMute(nAudioIndex),�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 12 mute audio decode  failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 12 check result failure !!!!"
		);

	CSTCPrint("step 13 unmute audio decode \n");
	//EXECUTIONFLOW: 13������CSUDIAUDIOUnmute(nAudioIndex),�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 13 unmute audio decode  failure!!!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 13 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 14������CSUDIAUDIOMute(nAudioIndex),����CSUDIAUDIOSetVolume����������Ϊ0,�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,0)),
			"Step 14.1 set Volume Failure !!!"
		);
	
	CSTCPrint("step 14 mute audio decode \r\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 14.2 mute audio decode  failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 14 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 15������CSUDIAUDIOSetVolume����������Ϊ31(���),�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,CSUDI_MAX_VOLUME)),
			"Step 15 set Volume Failure !!!"
		);
	CSTCPrint("step 15 The current volume is : %d \r\n",CSUDI_MAX_VOLUME);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 15 check result failure !!!!"
		);

	//EXECUTIONFLOW: 16������CSUDIAUDIOUnmute(nAudioIndex),�������кϷ������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 16 unmute audio decode  failure!!!!"
		);

	CSTCPrint("step 16 unmute audio decode \r\n");
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 16 check result failure !!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 17 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2��CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3��CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@CASEGROUP:4��CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@CASEGROUP:5��CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice, CSUDIAUDIOMode_E eMode)
//@CASEGROUP:6��CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice)
//@CASEGROUP:7��CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice)
//@DESCRIPTION:��ϲ���DD��DD+��Ŀ��HDMI����豸�����Ч����CSUDIAUDIOMute\CSUDIAUDIOUnmute\CSUDIAOUTMute\CSUDIAOUTUnmuteͬCSUDIAOUTSetOPMode��ϲ���
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��HDMI�豸��֧��DD��DD+����
//@INPUT: 
//@EXPECTATION: ÿһ�����������ֵ
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIAUDIOGetCount��CSUDIAUDIOGetCapability��ȡ֧��AC3����decodeģʽ����Ƶ����������ֵnAudioIndex,��nAudioIndex�ϲ���"DolbyAC3��׼����"�������֧��AC3���룬ֱ���˳�����������
//@EXECUTIONFLOW: 3������CSUDIAOUTSetOPMode����HDMI���Ϊdecodeģʽ����nAudioIndex�ϲ���"DolbyAC3��׼����"���������ųɹ��������豸���������
//@EXECUTIONFLOW: 4������CSUDIAOUTSetOPMode����HDMI���Ϊbypassģʽ,����HDMI�豸�����������RCA���������
//@EXECUTIONFLOW: 5������CSUDIAOUTSetOPMode����HDMI���Ϊdecodeģʽ,����HDMI�豸���������
//@EXECUTIONFLOW: 6������CSUDIAUDIOMute ��nAudioIndex��Ƶ������������������豸���������
//@EXECUTIONFLOW: 7������CSUDIAOUTSetOPMode����HDMI���Ϊbypassģʽ�����������豸���������
//@EXECUTIONFLOW: 8������CSUDIAUDIOUnmute��nAudioIndex��Ƶ�����,����HDMI�豸�����������RCA���������
//@EXECUTIONFLOW: 9������CSUDIAOUTSetOPMode����HDMI���Ϊdecodeģʽ,���������豸���������
//@EXECUTIONFLOW: 10������CSUDIAOUTMute��HDMI�豸����ر�,����HDMI�豸�����������RCA���������
//@EXECUTIONFLOW: 11�� ����CSUDIAOUTSetOPMode����HDMI���Ϊbypassģʽ,����HDMI�豸�����������RCA���������
//@EXECUTIONFLOW: 12������CSUDIAOUTUnmute��HDMI�豸����򿪣�����HDMI�豸�����������RCA���������
//@EXECUTIONFLOW: 13������CSUDIAOUTSetOPMode����HDMI���Ϊdecodeģʽ,���������豸���������
//@EXECUTIONFLOW: 14���ָ��ֳ�
CSUDI_BOOL CSTC_AUDIO_AOUT_IT_0002(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nAudioIndex = -1;

	if (!IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
		CSTCPrint("Not support HDMI!\n");
		return CSUDI_FALSE;
	}
	
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	//EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��
	//EXECUTIONFLOW: 2������CSUDIAUDIOGetCount��CSUDIAUDIOGetCapability��ȡ֧��AC3����decodeģʽ����Ƶ����������ֵnAudioIndex,��nAudioIndex�ϲ���"DolbyAC3��׼����"�������֧��AC3���룬ֱ���˳�����������
	//EXECUTIONFLOW: 3������CSUDIAOUTSetOPMode����HDMI���Ϊdecodeģʽ����nAudioIndex�ϲ���"DolbyAC3��׼����"���������ųɹ��������豸���������
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex)),
		"There's no audio support EM_UDI_AUD_STREAM_AC3 !!!!"
	);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 3 start play DolbyAC3��׼���� failure!!!!"
		);	

	CSTCPrint("step 3 start play DolbyAC3��׼����!!\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex ,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
			"step 3 check play DolbyAC3��׼���� failure!!!!"
		);	

	CSTCPrint("���²������ò�֧��DD�����HDMI�豸����!! \r\n");	
	CSTKWaitYes();
	
	//EXECUTIONFLOW: 4������CSUDIAOUTSetOPMode����HDMI���Ϊbypassģʽ,����HDMI�豸�����������RCA���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_BYPASS)),
			"step 4 set HDMI output bypass mode failure!!!!"
		);

	CSTCPrint("step 4 set HDMI output bypass mode!!\r\n");
	
	CSTCPrint("����HDMI�豸�����������RCA���������!!\r\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 4 check result failure !!!!"
		);

	//EXECUTIONFLOW: 5������CSUDIAOUTSetOPMode����HDMI���Ϊdecodeģʽ,����HDMI�豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
			"step 5 set HDMI output decode mode failure!!!!"
		);
	
	CSTCPrint("step 5 set HDMI output decode mode!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 5 check result failure !!!!"
		);

	//EXECUTIONFLOW: 6������CSUDIAUDIOMute ��nAudioIndex��Ƶ������������������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 6 mute audio decode failure!!!!"
		);

	CSTCPrint("step 6 mute audio decode!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 6 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 7������CSUDIAOUTSetOPMode����HDMI���Ϊbypassģʽ�����������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_BYPASS)),
			"step 7 set HDMI output bypass mode failure!!!!"
		);
	
	CSTCPrint("step 7 set HDMI output bypass mode!! \r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 7 check result failure !!!!"
		);

	//EXECUTIONFLOW: 8������CSUDIAUDIOUnmute��nAudioIndex��Ƶ�����,����HDMI�豸�����������RCA���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 8 unmoute audio decode  failure!!!!"
		);
	
	CSTCPrint("step 8 unmoute audio decode!! \r\n");
	CSTCPrint("����HDMI�豸�����������RCA���������!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 8 check result failure !!!!"
		);

	//EXECUTIONFLOW: 9������CSUDIAOUTSetOPMode����HDMI���Ϊdecodeģʽ,���������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
			"step 9 set HDMI output decode mode failure!!!!"
		);
	
	CSTCPrint("step 9 set HDMI output decode mode!! \r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 9 check result failure !!!!"
		);

	//EXECUTIONFLOW: 10������CSUDIAOUTMute��HDMI�豸����ر�,����HDMI�豸�����������RCA���������
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI)),
				"step 10 mute HDMI output  failure!!!!"
			);
	
	CSTCPrint("step 10 mute HDMI output!! \r\n");
	CSTCPrint("����HDMI�豸�����������RCA���������!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 10 check result failure !!!!"
		);

	//EXECUTIONFLOW: 11�� ����CSUDIAOUTSetOPMode����HDMI���Ϊbypassģʽ,����HDMI�豸�����������RCA���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_BYPASS)),
			"step 11 set HDMI output bypass mode failure!!!!"
		);

	CSTCPrint("step 11 set HDMI output bypass mode!! \r\n");
	CSTCPrint("����HDMI�豸�����������RCA���������!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 11 check result failure !!!!"
		);

	//EXECUTIONFLOW: 12������CSUDIAOUTUnmute��HDMI�豸����򿪣�����HDMI�豸�����������RCA���������
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI)),
				"step 12 unmute HDMI output  failure!!!!"
			);

	CSTCPrint("step 12 unmute HDMI output!! \r\n");
	CSTCPrint("����HDMI�豸�����������RCA���������!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 12 check result failure !!!!"
		);

	//EXECUTIONFLOW: 13������CSUDIAOUTSetOPMode����HDMI���Ϊdecodeģʽ,���������豸���������
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
			"step 13 set HDMI output decode mode failure!!!!"
		);

	CSTCPrint("step 13 set HDMI output decode mode!! \r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 13 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 14���ָ��ֳ�
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 14 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}


