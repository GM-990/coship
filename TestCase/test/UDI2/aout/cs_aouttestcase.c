/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_aouttestcase.h"
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_section.h"
#include "udi2_player.h"
#include "udi2_video.h"
#include "udi2_audio.h"
#include "udi2_demux.h"
#include "udi2_os.h"
#include "udi2_tuner.h"
#include "udiplus_debug.h"
#include "cs_testkit.h"
#include "../cs_udi2testcase.h"

static CSUDI_BOOL IsAOUTSupport(CSUDIAOUTDevices_E enType);
CSUDIAOUTDevices_E eSupportAudioDevice = 0;

typedef enum 
{
    EM_UDI_AOUT_STREAM_MPEG2,          /*MPEG2��׼����*/
	EM_UDI_AOUT_STREAM_AC3,				/*DolbyAC3��׼����*/
	EM_AOUT_MAX
}AoutTestServiceIndex_E;

typedef struct 
{	
	const char*          m_pcTsFilename;  	  //�����ļ�����
	const char* 		 m_pcServiceDescript; 	//��������
	int                  m_nVideoPid;           ///< ��������PID��-1��ʾ������
	CSUDIVIDStreamType_E m_eVidStreamType;      ///��Ƶ����
	int                  m_nAudioPid;           ///< ��������PID ��-1��ʾ������
	CSUDIAUDStreamType_E m_eAudStreamType;      ///<��Ƶ����
}AoutTestServiceInfo_S;

static AoutTestServiceInfo_S g_sAoutSeviceInfo[] ={
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"������AC3MP2",
			60,
			EM_UDI_VID_STREAM_MPEG2,
			62,
			EM_UDI_AUD_STREAM_MPEG2,
		}, /*EM_LIVE_MPEG2_MULTI_LAN1*/
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"DolbyAC3��׼����",
			512,
			EM_UDI_VID_STREAM_MPEG2,
			650,
			EM_UDI_AUD_STREAM_AC3,
		}, /*EM_LIVE_MPEG2_AC3*/
};

CSUDI_BOOL CSTC_AOUT_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	if(IsAOUTSupport(EM_UDIAOUT_RCA))
	{
		CSTCPrint("��ƽ̨֧��EM_UDIAOUT_RCA�豸\n");
		eSupportAudioDevice |= EM_UDIAOUT_RCA;
	}
	if(IsAOUTSupport(EM_UDIAOUT_SPDIF))
	{
		CSTCPrint("��ƽ̨֧��SPDIF�豸\n");
		eSupportAudioDevice |= EM_UDIAOUT_SPDIF;
	}
	if(IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
		CSTCPrint("��ƽ̨֧��HDMI�豸\n");
		eSupportAudioDevice |= EM_UDIAOUT_HDMI;
	}
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_AOUT_UnInit(void)
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

//Ѱ��֧��ĳ����Ƶ���͵Ľ�����
static int FindVideoDecoder(int nVideoDecCount, CSUDIVIDStreamType_E eFindVideoType)
{
	CSUDIVIDEOCapability_S sVideoCapabilityInfo;
	int nVideoID = -1;
	int i = 0;
	int k = 0;

	for (i=0; i<nVideoDecCount; i++)
	{
		if(!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOGetCapability(i, &sVideoCapabilityInfo)))
		{
      			for (k=0; k<EM_UDI_VID_STREAMTYPE_NUM; k++)
      			{
				if (eFindVideoType == sVideoCapabilityInfo.m_eStreamType[k])
				{
					nVideoID = i;
					break;
				}
				if (EM_UDI_VID_STREAM_UNKNOWN == sVideoCapabilityInfo.m_eStreamType[k])
				{
					break;
				}
			}
		}
			  
		if (nVideoID != -1)
		{
			break;
		}
	}
	return nVideoID;
}

//Ѱ��֧��ĳ����Ƶ���͵Ľ�����
static int FindAudioDecoder(int nAudioDecCount, CSUDIAUDStreamType_E eFindAudioType)
{
	CSUDIAUDIOCapability_S sAudioCapabilityInfo;
	int nAudioID = -1;
	int i = 0;
   	int k = 0;

	for (i=0; i<nAudioDecCount; i++)
	{
		if(!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo)))
		{
			for (k=0; k< EM_UDI_AUD_STREAMTYPE_NUM; k++ )
			{
				if (eFindAudioType == sAudioCapabilityInfo.m_sSupportDecode[k]) 
				{
					nAudioID = i;
					break;
				}
				if (EM_UDI_AUD_STREAM_UNKNOWN == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					break;
				}
			}
		}
			  
		if (nAudioID != -1)
		{
			break;
		}

	}
   	return nAudioID;
}

//��ѯƽ̨�Ƿ�֧��ĳ�����͵���Ƶ�������ʽ
static CSUDI_BOOL IsSupportDecoder(CSUDIAUDStreamType_E eFindAudioType)
{
	CSUDIAUDIOCapability_S sAudioCapabilityInfo;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int nAudioDecCount = 0;
	int i = 0;
	int k = 0;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)))
		return CSUDI_FALSE;

	if(CSASSERT_FAILED(nAudioDecCount > 0))
		return CSUDI_FALSE;

	for (i=0; i<nAudioDecCount; i++)
	{
		if(!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo)))
		{
			for (k=0; k <= EM_UDI_AUD_STREAMTYPE_NUM; k++ )
			{
				if (EM_UDI_AUD_STREAM_UNKNOWN == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					break;
				}
				if (EM_UDI_AUD_STREAMTYPE_NUM == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					break;
				}
				if (eFindAudioType == sAudioCapabilityInfo.m_sSupportDecode[k]) 
				{
					bRet = CSUDI_TRUE;
					return bRet;
				}			
			}
		}
	}	
	return bRet;		
}

// ��������Ƶ���Խ�Ŀ
static CSUDI_HANDLE PlayProgram01(AoutTestServiceIndex_E AoutTestIndex,int* pnAudioIndex)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIDEMUXCapability_S sDemuxCapabilityInfo;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S scStreamInfo[2];
	int nVideoDecCount = 0;
	int nVideoIndex = -1;
	int nAudioDecCount = 0;
	int nAudioIndex = -1;
	int nDemuxCount = 0 ; 
	int i = 0;

	if(AoutTestIndex >= EM_AOUT_MAX)
	{
		CSTCPrint("����Ĳ�����Ƶ������Ч!!!!\n");
		return CSUDI_NULL;
	}
	
	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoDecCount)))
		return CSUDI_NULL;
	if(CSASSERT_FAILED(nVideoDecCount > 0))
		return CSUDI_NULL;
	nVideoIndex = FindVideoDecoder(nVideoDecCount, g_sAoutSeviceInfo[AoutTestIndex].m_eVidStreamType);

	if(CSASSERT_FAILED(nVideoIndex != -1))
		return CSUDI_NULL;
	
	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)))
		return CSUDI_NULL;
	if(CSASSERT_FAILED(nAudioDecCount > 0))
		return CSUDI_NULL;
	nAudioIndex = FindAudioDecoder(nAudioDecCount, g_sAoutSeviceInfo[AoutTestIndex].m_eAudStreamType);

	if(g_sAoutSeviceInfo[AoutTestIndex].m_eAudStreamType == EM_UDI_AUD_STREAM_AC3)
	{	
		if(!IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
		{
			CSTCPrint("�ý�������֧��AC3��Ƶ��ʽ!!!!\n");
			nAudioIndex = 0;
		}
	}

	if(CSASSERT_FAILED(nAudioIndex != -1))
		return CSUDI_NULL;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)))
		return CSUDI_NULL;
	if(CSASSERT_FAILED(nDemuxCount > 0))
		return CSUDI_NULL;
	for (i=0; i<nDemuxCount; i++)
	{
		if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo)))
			return CSUDI_NULL;
		if (EM_UDI_DEMUX_PLAY == (sDemuxCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_PLAY))  
		{
			break;
		}
	}

	if(CSASSERT_FAILED(i<nDemuxCount))
		return CSUDI_NULL;
	sPlayerChnl.m_nDemux = i;
	sPlayerChnl.m_nAudioDecoder = nAudioIndex;
	sPlayerChnl.m_nVideoDecoder = nVideoIndex;

	//����������Ƶ��
	if(CSASSERT_FAILED(CSUDI_TRUE == CSTC_UDI2PortingLock(0, g_sAoutSeviceInfo[AoutTestIndex].m_pcTsFilename)))
		return CSUDI_NULL;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer)))
		return CSUDI_NULL;

	if(CSASSERT_FAILED(hPlayer != NULL))
		return CSUDI_NULL;

  	//��Ŀ��������
	scStreamInfo[0].m_nPid = g_sAoutSeviceInfo[AoutTestIndex].m_nAudioPid;
	scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	scStreamInfo[0].m_uStreamType.m_eAudioType = g_sAoutSeviceInfo[AoutTestIndex].m_eAudStreamType;
	scStreamInfo[1].m_nPid = g_sAoutSeviceInfo[AoutTestIndex].m_nVideoPid;
	scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	scStreamInfo[1].m_uStreamType.m_eVideoType = g_sAoutSeviceInfo[AoutTestIndex].m_eVidStreamType;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL)))
		return hPlayer;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)))
		return hPlayer;
	
	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)))
		return hPlayer;

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA)))
		return hPlayer;
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF)))
		return hPlayer;
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI)))
		return hPlayer;
	}
	
	if(NULL != pnAudioIndex)
	{
		*pnAudioIndex = nAudioIndex;
	}
	
	return hPlayer;	
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:���Խ�ĿԴΪMPEG��Ƶ,���ν����豸mute�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. ��Ŀ��������,���д����豸�����������
//@EXPECTATION:mute��unmute������ȷ,�һ����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ���MPEG��Ƶ����,��ϸ��������
//@EXECUTIONFLOW:1.1 ����������Ƶ��
//@EXECUTIONFLOW:1.2 ����CSUDIVIDEOGetCount��ȡvideo����������
//@EXECUTIONFLOW:1.3 ����CSUDIVIDEOGetCapability����Ѱ�ҵ�һ��֧��MPEG2��decoder,��������Ϊ��Ŀ���ŵ���Ƶ������
//@EXECUTIONFLOW:1.4 ����CSUDIAUDIOGetCount��ȡaudio������������
//@EXECUTIONFLOW:1.5 ����CSUDIAUDIOGetCapability����Ѱ�ҵ�һ��֧��MPEG2��decoder,��������Ϊ��Ŀ���ŵ���Ƶ������
//@EXECUTIONFLOW:1.6 ����CSUDIDEMUXGetCount��ȡdemux������
//@EXECUTIONFLOW:1.7 ����CSUDIDEMUXGetCapability����Ѱ�ҵ�һ��֧��EM_UDI_DEMUX_PLAY��demux���ڽ�Ŀ���ŵ�demux
//@EXECUTIONFLOW:1.8 ����CSUDIPLAYEROpen��һ��������,�����򿪳ɹ�
//@EXECUTIONFLOW:1.9 ����CSUDIPLAYERSetStream���ò�����������
//@EXECUTIONFLOW:1.10 ����CSUDIPLAYERStart���Ų�������,���������豸�����������
//@EXECUTIONFLOW:2. ����CSUDIAOUTMute�ر�RCA������������ȴ�ȷ��
//@EXECUTIONFLOW:3. ���ƽ̨֧��SPDIF��������CSUDIAOUTMute�ر�SPDIF����������ȴ�ȷ��
//@EXECUTIONFLOW:4. ���ƽ̨֧��HDMI��������CSUDIAOUTMute�ر�HDMI����������ȴ�ȷ��
//@EXECUTIONFLOW:5. ����CSUDIAOUTUnmute�������豸���������,���ȴ�ȷ��
//@EXECUTIONFLOW:6. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:7. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;

	hPlayer = PlayProgram01(AoutTestIndex, NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����1ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTCPrint("����1:�ѳɹ��������ţ���ȷ��RCA��������� \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " ����2ʧ�ܣ��ر�RCA�������ʧ��\n");
       	CSTCPrint("����2:�ѹر�RCA�������������ȷ��RCA�Ƿ����������\r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");
	}

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)	
		{
			CSTCPrint("ǰ�沽��ľ�����Ӧ��Ӱ�쵽SPDIF����ȷ��SPDIF ���������\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "RCA������Ӧ��Ӱ�쵽SPDIF\n");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), "����3ʧ�ܣ��ر�SPDIF�������ʧ��\n");
	       	CSTCPrint("����3:�ѹر�SPDIF�����������SPDIF �Ƿ����������\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
         }
	}

	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTCPrint("ǰ�沽��ľ�����Ӧ��Ӱ�쵽HDMI����ȷ��HDMI ���������\r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "ǰ��ͨ���ľ�����Ӧ��Ӱ�쵽HDMI\n");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " ����4ʧ�ܣ��ر�HDMI�������ʧ��\n");
       	CSTCPrint("����4:�ѹر�HDMI�����������HDMI �Ƿ����������\r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");
	}

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����5-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����5-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����5-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
	}	
	CSTCPrint("����5:��ȡ������ͨ���ľ���״̬����ȷ������ͨ����������������������ͱ������쳣\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");

   	CSTK_FATAL_POINT
  	{
  		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
  			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"�����������RCA�豸������ʧ��\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
  			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"�����������SPDIF�豸������ʧ��\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
  			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"�����������HDMI�豸������ʧ��\n");
		}
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:���Խ�ĿԴΪAC3��Ƶ,���ν����豸mute�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. ��Ŀ��������,���д����豸�����������
//@EXPECTATION:mute��unmute������ȷ,�һ����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ���AC3��Ƶ����,��ϸ��������
//@EXECUTIONFLOW:1.1 ����������Ƶ��
//@EXECUTIONFLOW:1.2 ����CSUDIVIDEOGetCount��ȡvideo����������
//@EXECUTIONFLOW:1.3 ����CSUDIVIDEOGetCapability����Ѱ�ҵ�һ��֧��MPEG2��decoder,��������Ϊ��Ŀ���ŵ���Ƶ������
//@EXECUTIONFLOW:1.4 ����CSUDIAUDIOGetCapability����Ѱ�ҵ�һ��֧��AC3��decoder,��������Ϊ��Ŀ���ŵ���Ƶ�����������ƽ̨��֧��AC3���룬ֱ��ָ����Ƶ��������indexΪ0
//@EXECUTIONFLOW:1.5 ����CSUDIAOUTSetOPMode����������ƵΪBYPASSģʽ
//@EXECUTIONFLOW:1.6 ����CSUDIDEMUXGetCount��ȡdemux������
//@EXECUTIONFLOW:1.7 ����CSUDIDEMUXGetCapability����Ѱ�ҵ�һ��֧��EM_UDI_DEMUX_PLAY��demux���ڽ�Ŀ���ŵ�demux
//@EXECUTIONFLOW:1.8 ����CSUDIPLAYEROpen��һ��������,�����򿪳ɹ�
//@EXECUTIONFLOW:1.9 ����CSUDIPLAYERSetStream���ò�����������
//@EXECUTIONFLOW:1.10 ����CSUDIPLAYERStart���Ų�������,����:���ƽ̨֧��AC3�����������豸�����������,����ֻ��SPDIF���������
//@EXECUTIONFLOW:2. ���ƽ̨֧��AC3���룬������ͨ�������������
//@EXECUTIONFLOW:3. ���ιر�ͨ����������������Ӱ�졣
//@EXECUTIONFLOW:4. ���δ�����ͨ��������ϣ����֧�ֵ�ͨ��������������
//@EXECUTIONFLOW:5. ��SPDIFͨ�����ģʽ��ΪBYPASS��ϣ��SPDIFͨ���������������
//@EXECUTIONFLOW:6. ���ƽ̨��֧��AC3���룬�������SPDIFͨ�����������
//@EXECUTIONFLOW:7. �ر�SPDIF ͨ�����������ϣ���رճɹ�
//@EXECUTIONFLOW:9. ��SPDIF ͨ�����ģʽ��ΪDECODE��ϣ��û���������
//@EXECUTIONFLOW:10. �ظ�2-9��2 ��
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	int i = 0;
	int nRepeatTimes = 2;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "���Ž�Ŀʧ��,hPlayer Ϊ��!");

	for(i = 0; i < nRepeatTimes; i++) 
	{	
		if(IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
		{
			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
			}
			
			CSTCPrint("����2:������ƽ̨֧�ֵ�����ͨ�������ģʽΪDECODEģʽ����ȷ���Ƿ�����ͨ����������\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");
			if(eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " ����3ʧ�ܣ��ر�RCA�������ʧ��\n");
				CSTCPrint("����3:������RCA��������ȷ��RCA �Ƿ����������\r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
			}

			if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
				if(eSupportAudioDevice & EM_UDIAOUT_SPDIF)	
				{
					CSTCPrint("ǰ�沽��ľ�����Ӧ��Ӱ�쵽SPDIF����ȷ��SPDIF ���������\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "RCA������Ӧ��Ӱ�쵽SPDIF\n");
			
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " ����3ʧ�ܣ��ر�SPDIF�������ʧ��\n");
				    CSTCPrint("����3:������SPDIF������SPDIF �Ƿ����������\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");		
                }
             }

			if(eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTCPrint("ǰ�沽��ľ�����Ӧ��Ӱ�쵽HDMI����ȷ��HDMI ���������\r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "ǰ��ͨ���ľ�����Ӧ��Ӱ�쵽HDMI\n");
		
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " ����3ʧ�ܣ��ر�HDMI�������ʧ��\n");
			    CSTCPrint("����3:������HDMI������HDMI �Ƿ����������\r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
			}

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA), " �ָ�RCAͨ���������\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF), " �ָ�SPDIFͨ���������\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI), " �ָ�HDMIͨ���������\n");
			}
	       	CSTCPrint("����4:��ȡ��ƽ̨֧�ֵ������豸�ľ���״̬����ȷ�������豸��������������������ͱ������쳣\r\n");
	       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");

			if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
				if(eSupportAudioDevice & EM_UDIAOUT_SPDIF)	
                {
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS),"����AC3����ʧ��\n");
					CSTCPrint("����5:������SPDIF���ģʽΪBYPASS����ȷ��SPDIF��Ȼ�������������\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");
                }
			}
		}
		else
		{
 
        	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
				if(eSupportAudioDevice & EM_UDIAOUT_SPDIF)	
                {
					//CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"����DECODEģʽʧ��\n");
					CSTCPrint("����1.10:ƽ̨��֧��AC3���룬��ȷ�������豸��û���������\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1.10ʧ��\n");
					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS),"����BYPASSģʽʧ��\n");
				
					CSTCPrint("ֻ��SPDIF���������\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1.10ʧ��\n");
				 
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " ����3ʧ�ܣ��ر�SPDIF�������ʧ��\n");
				    CSTCPrint("SPDIF �Ƿ����������\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
				
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF), " �ָ�����ͨ���������\n");
			       	CSTCPrint("SPDIFͨ�������ָ������������ͱ������쳣\r\n");
			       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"����AC3����ʧ��\n");
					CSTCPrint("��ȷ������ͨ��û���������\r\n");
			       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");
                }
			}
		}
	}

   CSTK_FATAL_POINT
   {
   		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"�����������RCA�豸������ʧ��\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"�����������SPDIF�豸������ʧ��\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"�����������HDMI�豸������ʧ��\n");
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
			hPlayer = CSUDI_NULL;
		}
   }
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:���Զ��mute��,��unmute�Ƿ�������������
//@PRECONDITION:��Ŀ��������
//@INPUT:1. ��Ŀ��������,���д����豸�����������
//@EXPECTATION:1.���mute�����豸��ʱ�������豸û���������
//@EXPECTATION:2.unmute��ʱ�������ָ�����
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0001����1
//@EXECUTIONFLOW:2. ѭ������5��CSUDIAOUTMute�ر������豸���������,���ȴ�ȷ��
//@EXECUTIONFLOW:3. ����CSUDIAOUTUnmute�������豸������������ȴ�ȷ��
//@EXECUTIONFLOW:4. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:5. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0003( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	int i = 0;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex, NULL);  //����PLAYER���Ų�������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

    if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
    }    

	CSTCPrint("[UDI2AOUTEST]����1:���Խ�ĿԴΪMPEG��Ƶ,�Ƿ�ƽ̨֧�ֵ��豸����������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");

	for (i= 0; i<5; i++)
	{
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"����2-1ʧ�ܣ�δ�ܹر�RCA�豸����\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"����2-2ʧ�ܣ�δ�ܹر�SPDIF�豸����\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"����2-3ʧ�ܣ�δ�ܹر�HDMI�豸����\n");
		}

		CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�ƽ̨֧�ֵ������豸����\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
	    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");      
	}

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����3-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����3-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����3-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
	}

	CSTCPrint("[UDI2AOUTEST]����3:�Ѷ�ƽ̨֧�ֵ������豸ȡ������\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�����豸��������� \r\n");
   	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
		   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:���Ե�mute�豸��,��̨�����ƻ�mute״̬
//@PRECONDITION:��Ŀ��������
//@INPUT:1. ��Ŀ��������,���д����豸�����������
//@EXPECTATION:1.mute.unmute������ȷ
//@EXPECTATION:2.��̨�����ƻ�����״̬
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0001����1
//@EXECUTIONFLOW:2.����CSUDIAOUTMute�ر������豸���������,���ȴ�ȷ��
//@EXECUTIONFLOW:3.����CSUDIPLAYERStopֹͣ��ǰ��Ŀ
//@EXECUTIONFLOW:4.����CSUDIPLAYERSetStream�����½�Ŀ����
//@EXECUTIONFLOW:5.����CSUDIPLAYERStart�����½�Ŀ,������Ŀ��������,��ͼ��,���������豸�������־���״̬
//@EXECUTIONFLOW:6.����CSUDIAOUTUnmute�������豸�������,���������豸���������
//@EXECUTIONFLOW:7. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:8. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0004( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	CSUDIStreamInfo_S scStreamInfo[2]; 
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2AOUTEST]����1:���Խ�ĿԴΪMPEG��Ƶ,�Ƿ�ƽ̨֧�ֵ��豸����������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"����2-1ʧ�ܣ�δ�ܹر�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"����2-2ʧ�ܣ�δ�ܹر�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"����2-3ʧ�ܣ�δ�ܹر�HDMI�豸����\n");
	}

	CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�ƽ̨֧�ֵ������豸����\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����3ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n");

	//�����½�Ŀ����,���²��Ž�Ŀ
	scStreamInfo[0].m_nPid = 1141;
	scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;
	scStreamInfo[1].m_nPid = 1140;
	scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL), "���ý�Ŀ����ʧ��\n");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����Ƶ���󣬲��Ž�Ŀʧ��\n");

	   CSTCPrint("[UDI2AOUTEST]����5:�Ѹ���Ƶ��\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,����Ƶ���������豸��Ȼ���־���\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,����Ƶ�����豸��������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����6-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����6-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����6-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
	}

	 CSTCPrint("[UDI2AOUTEST]����6:�Ѷ�ƽ̨֧�ֵ������豸ȡ������\n");
    CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�����豸��������� \r\n");
    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����6ʧ��\n");
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
		}
  	}
	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:����CSUDIAOUTMute ������������
//@PRECONDITION:CSUDIAOUTMute ����CSUDIAOUT_ERROR_BAD_PARAMETER
//@INPUT:�߼�����Ĳ���
//@EXECUTIONFLOW:1.����CSUDIAOUTMute������������
//@EXECUTIONFLOW:2.ϣ������CSUDIAOUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0005( void )
{
	int i = 0;
	
	int nErrorParam[3] = {0};

	nErrorParam[0] = -1;
	nErrorParam[1] = 0;
	nErrorParam[2] = EM_UDIAOUT_HDMI+1;
	
	for(i=0; i<sizeof(nErrorParam)/sizeof(nErrorParam[0]); i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIAOUTMute(nErrorParam[i])), "�������ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:��ϲ���PCM����ʱ��������ӿڵĿ���
//@PRECONDITION:CSUDIAOUTMute ����CSUDIAOUT_ERROR_BAD_PARAMETER
//@INPUT:�߼�����Ĳ���
//@EXECUTIONFLOW:1.����CSUDIAOUTMute������������
//@EXECUTIONFLOW:2.ϣ������CSUDIAOUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0006( void )
{
	int i = 0;
	
	int nErrorParam[3] = {0};

	nErrorParam[0] = -1;
	nErrorParam[1] = 0;
	nErrorParam[2] = EM_UDIAOUT_HDMI+1;

	for(i=0; i<sizeof(nErrorParam)/sizeof(nErrorParam[0]); i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIAOUTMute(nErrorParam[i])), "�������ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:���Խ�ĿԴΪMPEG,���ν����豸unmute�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. ��Ŀ��������,���д����豸�����������
//@EXPECTATION:1.mute�����豸��ʱ�������豸û���������
//@EXPECTATION:2.����unmute�豸��ʱ����Ӧ�豸�ָ��������
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0001����1
//@EXECUTIONFLOW:2. ����CSUDIAOUTMute�ر������豸���������,���ȴ�ȷ��
//@EXECUTIONFLOW:3. ����CSUDIAOUTUnmute��RCA������������ȴ�ȷ��
//@EXECUTIONFLOW:4. ���ƽ̨֧��SPDIF��������CSUDIAOUTUnmute��SPDIF����������ȴ�ȷ��
//@EXECUTIONFLOW:5. ���ƽ̨֧��HDMI��������CSUDIAOUTUnmute��HDMI����������ȴ�ȷ��
//@EXECUTIONFLOW:6. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:7. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2AOUTEST]����1:���Խ�ĿԴΪMPEG��Ƶ,�Ƿ�ƽ̨֧�ֵ��豸����������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"����2-1ʧ�ܣ�δ�ܹر�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"����2-2ʧ�ܣ�δ�ܹر�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"����2-3ʧ�ܣ�δ�ܹر�HDMI�豸����\n");
	}

	CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�ƽ̨֧�ֵ������豸����\n");
    CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����3ʧ�ܣ�δ�ܴ�RCA�豸����\n");
		CSTCPrint("[UDI2AOUTEST]����3:�Ѷ�RCAȡ������\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,RCA ��������������������ͱ������쳣\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,RCA ��������� \r\n");
	    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
	}

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
        {
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF), " ����4ʧ�ܣ���SPDIF�������ʧ��\n");
			CSTCPrint("[UDI2AOUTEST]����4:�Ѷ�SPDIFȡ������\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF ��������������������ͱ������쳣\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,SPDIF ��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");
        }
	}

	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI), " ����5ʧ�ܣ���HDMI�������ʧ��\n");
		CSTCPrint("[UDI2AOUTEST]����5:�Ѷ�HDMIȡ������\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,HDMI ��������������������ͱ������쳣\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,HDMI ��������� \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");
	}
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:���Խ�ĿԴΪAC3,���ν����豸unmute�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. ��Ŀ��������,���д����豸�����������
//@EXPECTATION:1.mute�����豸��ʱ�������豸û���������
//@EXPECTATION:2.����unmute�豸��ʱ����Ӧ�豸�ָ��������
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.���ƽ̨��֧��AC3����ֱ�ӷ���TRUE,�������playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0002����1
//@EXECUTIONFLOW:2. ����CSUDIAOUTMute�ر������豸���������,���ȴ�ȷ��
//@EXECUTIONFLOW:3. ����CSUDIAOUTUnmute��RCA������������ȴ�ȷ��
//@EXECUTIONFLOW:4. ���ƽ̨֧��SPDIF��������CSUDIAOUTUnmute��SPDIF����������ȴ�ȷ��
//@EXECUTIONFLOW:5. ���ƽ̨֧��HDMI��������CSUDIAOUTUnmute��HDMI����������ȴ�ȷ��
//@EXECUTIONFLOW:6. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:7. ���ƽ̨֧��AC3������Ƶ����,����CSUDIAOUTSetOPMode����������ƵΪ����ģʽ
//@EXECUTIONFLOW:8. �ظ�����1.9������6,�ظ�һ��
//@EXECUTIONFLOW:9. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	CSUDIStreamInfo_S scStreamInfo[2]; 
	int i = 2;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;

	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");
	
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{

		if (IsAOUTSupport(EM_UDIAOUT_RCA))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
		}
		if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS) ʧ��!");
        }
		if (IsAOUTSupport( EM_UDIAOUT_HDMI))
		{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
		}
		CSTCPrint("[UDI2AOUTEST]����1:���Խ�ĿԴΪAC3��Ƶ�����ƽ̨֧��AC3���룬��ƽ̨֧�ֵ��豸�����������������ֻ��SPDIF�������� \r\n");
   		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");	
		do 
		{
			if (1 == i)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
				if (IsAOUTSupport(EM_UDIAOUT_RCA))
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
				}
		    	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		        {
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE) ʧ��!");
		        }  
				if (IsAOUTSupport( EM_UDIAOUT_HDMI))
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
				}
				//������������
				scStreamInfo[0].m_nPid = 650;
				scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
				scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_AC3;
				scStreamInfo[1].m_nPid = 512;
				scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
				scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL), "CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL) ʧ��!");
             			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "CSUDIPLAYERStart(hPlayer) ʧ��!");
			}
			
	       		i--;
			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"����2-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"����2-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"����2-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
			}

			CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�ƽ̨֧�ֵ������豸����\n");
		    	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
          		CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
	       		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����3ʧ�ܣ�δ�ܴ�RCA�豸����\n");
				CSTCPrint("[UDI2AOUTEST]����3:�Ѷ�RCAȡ������\n");
				CSTCPrint("[UDI2AOUTEST]:Yes,RCA ��������������������ͱ������쳣\r\n");
				CSTCPrint("[UDI2AOUTEST]:No,RCA ��������� \r\n");
		       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
			}


			if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
			   	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
				{
			    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF), " ����4ʧ�ܣ���SPDIF�������ʧ��\n");
					CSTCPrint("[UDI2AOUTEST]����4:�Ѷ�SPDIFȡ������\n");
					CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF ��������������������ͱ������쳣\r\n");
					CSTCPrint("[UDI2AOUTEST]:No,SPDIF ��������� \r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");		
	            }
			}

			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI), " ����5ʧ�ܣ���HDMI�������ʧ��\n");
				CSTCPrint("[UDI2AOUTEST]����5:�Ѷ�HDMIȡ������\n");
				CSTCPrint("[UDI2AOUTEST]:Yes,HDMI ��������������������ͱ������쳣\r\n");
				CSTCPrint("[UDI2AOUTEST]:No,HDMI ��������� \r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");
			}
		}while(i);
	}
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����6ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����9ʧ�ܣ��رղ�����ʧ��\n");
		}
   	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:���Խ�ĿԴΪMPEG,mute�����豸�ʹ������豸�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. ������Ŀ����,���д����豸�����������
//@EXPECTATION:1.mute�����豸��ʱ�������豸û���������
//@EXPECTATION:2.unmute�����豸��ʱ��,�����豸���ָ��������
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0001����1
//@EXECUTIONFLOW:2. ����CSUDIAOUTMute�ر������豸���������,���ȴ�ȷ��
//@EXECUTIONFLOW:3. ����CSUDIAOUTUnmute�������豸�������,���ȴ�ȷ��
//@EXECUTIONFLOW:4. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:5. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0003( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2AOUTEST]����1:���Խ�ĿԴΪMPEG��Ƶ,�Ƿ�ƽ̨֧�ֵ��豸����������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " ����2-1ʧ�ܣ��ر�RCA�豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " ����2-2ʧ�ܣ��ر�SPDIF�豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " ����2-3ʧ�ܣ��ر�HDMI�豸�������ʧ��\n");
	}

	CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�ƽ̨֧�ֵ������豸����\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
   	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����3-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����3-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����3-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
	}

	CSTCPrint("[UDI2AOUTEST]����3:�Ѷ�ƽ̨֧�ֵ������豸ȡ������\n");
 	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�����豸��������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
	
   	CSTK_FATAL_POINT
   	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:���Խ�ĿԴΪAC3,mute�����豸�ʹ������豸�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. ������Ŀ����,���д����豸�����������
//@EXPECTATION:1.mute�����豸��ʱ�������豸û���������
//@EXPECTATION:2.unmute�����豸��ʱ��,�����豸���ָ��������
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0002����1
//@EXECUTIONFLOW:2. ����CSUDIAOUTMute�ر������豸���������,���ȴ�ȷ��
//@EXECUTIONFLOW:3. ����CSUDIAOUTUnmute�������豸�������,���ȴ�ȷ��,���ƽ̨��֧��AC3������ֻ��SPDIF���������
//@EXECUTIONFLOW:4. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:5. ���ƽ̨֧��AC3������Ƶ����,����CSUDIAOUTSetOPMode����������ƵΪ����ģʽ
//@EXECUTIONFLOW:6. �ظ�����1.9������4,�ظ�һ��
//@EXECUTIONFLOW:7. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0004( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	CSUDIStreamInfo_S scStreamInfo[2]; 
	int i = 2;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	if (IsAOUTSupport(EM_UDIAOUT_RCA))
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
	}

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����BYPASSģʽʧ��");
    }   
	if (IsAOUTSupport( EM_UDIAOUT_HDMI))
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE),"����DECODERģʽʧ��\n");
	}
	
	CSTCPrint("[UDI2AOUTEST]����1:���Խ�ĿԴΪAC3��Ƶ,�Ƿ�ƽ̨֧�ֵ��豸���������������ƽ̨��֧��AC3������ֻ��SPDIF��������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");
	
	do 
	{
		if (1 == i)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "����EM_UDIAOUT_DECODEģʽʧ��");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "����EM_UDIAOUT_DECODEģʽʧ��");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "����EM_UDIAOUT_DECODEģʽʧ��");
			}
			
			//������������
			scStreamInfo[0].m_nPid = 650;
			scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
			scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_AC3;
			scStreamInfo[1].m_nPid = 512;
			scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
			scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL), "CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL)ʧ��!");
         		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "CSUDIPLAYERStart(hPlayer)ʧ��!");
		}
		
      		i--;
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"����2-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"����2-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"����2-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
		}
		CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�ƽ̨֧�ֵ������豸����\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
       		CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");

		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����3-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����3-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����3-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
		}
		CSTCPrint("[UDI2AOUTEST]����3:�Ѷ�ƽ̨֧�ֵ������豸ȡ������\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
      		CSTCPrint("[UDI2AOUTEST]:No,�����豸��������� \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");
		
	}while(IsSupportDecoder(EM_UDI_AUD_STREAM_AC3) && i);

   	CSTK_FATAL_POINT
  	{

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����6ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����7ʧ�ܣ��رղ�����ʧ��\n");
		}
   	}
	return CSUDI_TRUE;      
}

//@CASEGROUP:CSUDIAOUTMute 
//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:���Խ�ĿԴΪMPEG,CSUDIAUDIOMute\CSUDIAUDIOUnmute��CSUDIAOUTMute\CSUDIAOUTUnmute���߼��������
//@PRECONDITION:��Ŀ��������
//@INPUT:1.��Ŀ ��������,���д����豸�����������
//@EXPECTATION:1.CSUDIAUDIOMute��ʱ��ʹ�豸��unmute��Ҳ�������������
//@EXPECTATION:2.ֻ��CSUDIAUDIOMute������Ӧ�豸unmute��ʱ����Ӧ�豸�������������
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0001����1
//@EXECUTIONFLOW:2. ����CSUDIAUDIOUnmute��audio���������,����CSUDIAOUTUnmute�������豸���������,���������豸���������
//@EXECUTIONFLOW:3. ����CSUDIAUDIOMute�ر�audio������,���������豸��û���������
//@EXECUTIONFLOW:4. ����CSUDIAUDIOUnmute,���������豸�ָ��������
//@EXECUTIONFLOW:5. ����CSUDIAOUTMute�ر������豸���������,���������豸����
//@EXECUTIONFLOW:6. ����CSUDIAOUTUnmute�������豸���������,���������豸�����������
//@EXECUTIONFLOW:7. �ر�RCA���������,����RCA����
//@EXECUTIONFLOW:8. ���ƽ̨֧��HDMI,�ر�HDMI���������,����HDMI����
//@EXECUTIONFLOW:9. ���ƽ̨֧��SPDIF,�ر�SPDIF���������,����SPDIF����
//@EXECUTIONFLOW:10. ����CSUDIAOUTUnmute�������豸���������,���������豸�����������
//@EXECUTIONFLOW:11. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:12. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0005( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nAudioIndex = -1;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,&nAudioIndex);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL((nAudioIndex != -1),"������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2AOUTEST]����1:���Խ�ĿԴΪMPEG��Ƶ,�Ƿ�ƽ̨֧�ֵ��豸����������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "����2ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����2-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����2-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����2-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
	}

	CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�Audio��ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex), "����3ʧ��\n");
	CSTCPrint("[UDI2AOUTEST]����3:�Ѷ�Audio���þ���(CSUDIAUDIOMute&CSUDIAOUTUnmute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,Mute Audio �����豸���������\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "����4ʧ��\n");
	CSTCPrint("[UDI2AOUTEST]����4:�Ѷ�Audioȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,Unmute Audio �����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " ����5-1ʧ�ܣ��ر�RCA�豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " ����5-2ʧ�ܣ��ر�SPDIF�豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " ����5-3ʧ�ܣ��ر�HDMI�豸�������ʧ��\n");
	}

	CSTCPrint("[UDI2AOUTEST]����5:�Ѷ�ƽ̨֧�ֵ������豸����(CSUDIAUDIOUnmute&CSUDIAOUTMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
   	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����6-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����6-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����6-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
	}
	CSTCPrint("[UDI2AOUTEST]����6:�Ѷ�ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�����豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����6ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " ����7ʧ�ܣ��ر�RCA�������ʧ��\n");
		CSTCPrint("[UDI2AOUTEST]����7:�Ѷ�RCA����\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,RCA û���������\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,RCA ���������\r\n");
       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����7ʧ��\n");
	}


    if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
	    if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " ����8ʧ�ܣ��ر�SPDIF�������ʧ��\n");
			CSTCPrint("[UDI2AOUTEST]����8:�Ѷ�SPDIF����\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF ���������\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,SPDIF ��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����8ʧ��\n");
		}
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " ����9ʧ�ܣ��ر�HDMI�������ʧ��\n");
		CSTCPrint("[UDI2AOUTEST]����9:�Ѷ�HDMI����\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,HDMI ���������\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,HDMI ��������� \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����9ʧ��\n");
	}

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����10-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����10-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����10-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
	}

	CSTCPrint("[UDI2AOUTEST]����10:�Ѷ�ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
    CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����10ʧ��\n");
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:���Խ�ĿԴΪMPEG,CSUDIAUDIOMute\CSUDIAOUTMute��CSUDIAUDIOUnmute\CSUDIAOUTUnmute���߼��������
//@PRECONDITION:��Ŀ��������
//@INPUT:1.��Ŀ ��������,���д����豸�����������
//@EXPECTATION:1.CSUDIAUDIOMute��ʱ��ʹ�豸��unmute��Ҳ�������������
//@EXPECTATION:2.ֻ��CSUDIAUDIOUnmute������Ӧ�豸unmute��ʱ����Ӧ�豸�������������
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0001����1
//@EXECUTIONFLOW:2. ����CSUDIAUDIOUnmute��audio���������,����CSUDIAOUTUnmute�������豸���������,���������豸���������
//@EXECUTIONFLOW:3. ����CSUDIAUDIOMute�ر�audio������,���������豸��û���������
//@EXECUTIONFLOW:4. ����CSUDIAOUTMute�ر������豸���������,���������豸�Ա��־���
//@EXECUTIONFLOW:5. ����CSUDIAUDIOUnmute��audio���������,���������豸�Ա��־���
//@EXECUTIONFLOW:6.  ����CSUDIAOUTUnmute�������豸���������,���������豸�����������
//@EXECUTIONFLOW:7. ����CSUDIAOUTMute�ر������豸���������,���������豸��û���������
//@EXECUTIONFLOW:8. ����CSUDIAUDIOMute�ر�audio������,���������豸��û���������
//@EXECUTIONFLOW:9.  ����CSUDIAOUTUnmute�������豸��������������������豸��û���������
//@EXECUTIONFLOW:10.  ����CSUDIAUDIOUnmute��audio���������,���������豸�����������
//@EXECUTIONFLOW:11. �ر�RCA���������,����RCA����
//@EXECUTIONFLOW:12. ���ƽ̨֧��HDMI,�ر�HDMI���������,����HDMI����
//@EXECUTIONFLOW:13. ���ƽ̨֧��SPDIF,�ر�SPDIF���������,����SPDIF����
//@EXECUTIONFLOW:14. ����CSUDIAOUTUnmute�������豸���������,���������豸�����������
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0006( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nAudioIndex = -1;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,&nAudioIndex);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL((nAudioIndex != -1),"������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2AOUTEST]����1:���Խ�ĿԴΪMPEG��Ƶ,�Ƿ�ƽ̨֧�ֵ��豸����������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����1ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "����2��Audio���������ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����2-1��RCA����豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����2-2��SPDIF����豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����2-3��HDMI����豸�������ʧ��\n");
	}

	CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�Audio��ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
    	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex), "����3�ر�Audio���������ʧ��\n");
	CSTCPrint("[UDI2AOUTEST]����3:�Ѷ�Audio���þ���(CSUDIAUDIOUnmute&CSUDIAUDIOMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"����4-1�ر�RCA�豸���������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"����4-2�ر�SPDIF�豸���������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"����4-3�ر�HDMI�豸���������ʧ��\n");
	}
	CSTCPrint("[UDI2AOUTEST]����4:�Ѷ�ƽ̨֧�ֵ������豸����(CSUDIAOUTMute&CSUDIAUDIOMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸Ӧ���־���\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "����5��Audio���������ʧ��\n");
	CSTCPrint("[UDI2AOUTEST]����5:�Ѷ�Audioȡ������(CSUDIAUDIOUnmute&CSUDIAOUTMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸Ӧ���־���\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����6-1��RCA�豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����6-2��SPDIF�豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����6-3��HDMI�豸�������ʧ��\n");
	}

	CSTCPrint("[UDI2AOUTEST]����6:�Ѷ�ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����6ʧ��\n");
	
	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"����7-1�ر�RCA�豸���������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"����7-2�ر�SPDIF�豸���������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"����7-3�ر�HDMI�豸���������ʧ��\n");
	}

	CSTCPrint("[UDI2AOUTEST]����7:�Ѷ�ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸Ӧ���־���\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����7ʧ��\n");
		           
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex), "����8�ر�Audio�������ʧ��\n");
	CSTCPrint("[UDI2AOUTEST]����8:�Ѷ�Audio���þ���(CSUDIAUDIOMute&CSUDIAOUTMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸�Ա��־���\r\n");
   	CSTCPrint("[UDI2AOUTEST]:No,�豸���������\r\n");
   	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����8ʧ��\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����9-1��RCA�豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����9-2��SPDIF�豸�������ʧ��\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����9-3��HDMI�豸�������ʧ��\n");
	}

	CSTCPrint("[UDI2AOUTEST]����9:�Ѷ�ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOMute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��Ȼû���������\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����9ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "����10��Audio�������ʧ��\n");
	CSTCPrint("[UDI2AOUTEST]����10:�Ѷ�Audioȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����10ʧ��\n");
	
	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " ����11ʧ�ܣ��ر�RCA�������ʧ��\n");
		CSTCPrint("[UDI2AOUTEST]����11:�Ѷ�RCA����\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,RCA û���������\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,RCA ��������� \r\n");
       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����11ʧ��\n");
	}

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
	{

		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " ����12ʧ�ܣ��ر�SPDIF�������ʧ��\n");
			CSTCPrint("[UDI2AOUTEST]����12:�Ѷ�SPDIF����\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF ���������\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,SPDIF ��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����12ʧ��\n");	
		}
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " ����13ʧ�ܣ��ر�HDMI�������ʧ��\n");
		CSTCPrint("[UDI2AOUTEST]����13:�Ѷ�HDMI����\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,HDMI ���������\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,HDMI ��������� \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����13ʧ��\n");	
	}

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����14-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����14-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����14-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
	}

	CSTCPrint("[UDI2AOUTEST]����14:�Ѷ�ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����14ʧ��\n");
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����15ֹͣ���Ž�Ŀʧ��\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����16�رղ�����ʧ��\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:���Խ�ĿԴΪAC3,CSUDIAUDIOMute\CSUDIAUDIOUnmute��CSUDIAOUTMute\CSUDIAOUTUnmute���߼��������
//@PRECONDITION:��Ŀ��������
//@INPUT:1.��Ŀ ��������,���д����豸�����������
//@EXPECTATION:1.CSUDIAUDIOMute��ʱ��ʹ�豸��unmute��Ҳ�������������
//@EXPECTATION:2.ֻ��CSUDIAUDIOMute������Ӧ�豸unmute��ʱ����Ӧ�豸�������������
//@EXPECTATION:3.mute��unmute�����л�ʱû�������ͱ������쳣
//@EXECUTIONFLOW:1.���ƽ̨��֧��AC3����ֱ�ӷ���TRUE,�������playerģ��ӿڲ��Ų�������,��ϸ������ο���������CSTC_AOUT_TEST_IT_AOUTMute_0002����1
//@EXECUTIONFLOW:2. ����CSUDIAUDIOUnmute��audio���������,����CSUDIAOUTUnmute�������豸���������,���������豸���������
//@EXECUTIONFLOW:3. ����CSUDIAUDIOMute�ر�audio������,���������豸��û���������
//@EXECUTIONFLOW:4. ����CSUDIAUDIOUnmute,���������豸�ָ��������
//@EXECUTIONFLOW:5. ����CSUDIAOUTMute�ر������豸���������,���������豸����
//@EXECUTIONFLOW:6. ����CSUDIAOUTUnmute�������豸���������,���������豸�����������
//@EXECUTIONFLOW:7. �ر�RCA���������,����RCA����
//@EXECUTIONFLOW:8. ���ƽ̨֧��HDMI,�ر�HDMI���������,����HDMI����
//@EXECUTIONFLOW:9. ���ƽ̨֧��SPDIF,�ر�SPDIF���������,����SPDIF����
//@EXECUTIONFLOW:10. ����CSUDIAOUTUnmute�������豸���������,���������豸�����������
//@EXECUTIONFLOW:11. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:12. ���ƽ̨֧��AC3������Ƶ����,����CSUDIAOUTSetOPMode����������ƵΪ����ģʽ
//@EXECUTIONFLOW:13. �ظ�����1.9������11,�ظ�һ��
//@EXECUTIONFLOW:14. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0007( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S scStreamInfo[2];
	int i = 2;
	int nAudioIndex = -1;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;

	hPlayer = PlayProgram01(AoutTestIndex,&nAudioIndex);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL((nAudioIndex != -1),"������Ŀ����ʧ��\n");
	
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{	
		do 
		{
			if( IsAOUTSupport( EM_UDIAOUT_RCA))
			{
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "����DECODEģʽʧ��");
			}
			if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����BYPASSģʽʧ��");
        	} 

			if( IsAOUTSupport( EM_UDIAOUT_HDMI))
			{
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "����DECODEģʽʧ��");
			}
			if (1 == i)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
                
				if( IsAOUTSupport( EM_UDIAOUT_RCA))
				{
			
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "����DECODEģʽʧ��");
				}
				if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "����DECODEģʽʧ��");
                }
				if( IsAOUTSupport( EM_UDIAOUT_HDMI))
				{
			
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "����DECODEģʽʧ��");
				}
				//������������
				scStreamInfo[0].m_nPid = 650;
				scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
				scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_AC3;
				scStreamInfo[1].m_nPid = 512;
				scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
				scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL), "CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL)ʧ��!");
             			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "CSUDIPLAYERStart(hPlayer)ʧ��!");
			}
          		i--;
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "����2ʧ��\n");

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����2-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����2-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����2-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
			}

			CSTCPrint("[UDI2AOUTEST]����2:�Ѷ�Audio��ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex), "����3ʧ��\n");
			CSTCPrint("[UDI2AOUTEST]����3:�Ѷ�Audio���þ���(CSUDIAUDIOMute&CSUDIAOUTUnmute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
          		CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "����4ʧ��\n");
			CSTCPrint("[UDI2AOUTEST]����4:�Ѷ�Audioȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
          		CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA)," ����5-1ʧ�ܣ��ر�RCA�豸�������ʧ��\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF)," ����5-2ʧ�ܣ��ر�SPDIF�豸�������ʧ��\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI)," ����5-3ʧ�ܣ��ر�HDMI�豸�������ʧ��\n");
			}
			CSTCPrint("[UDI2AOUTEST]����5:�Ѷ�ƽ̨֧�ֵ������豸����(CSUDIAUDIOUnmute&CSUDIAOUTMute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,�����豸���������\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����6-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����6-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����6-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
			}
			CSTCPrint("[UDI2AOUTEST]����6:�Ѷ�ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����6ʧ��\n");


            if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
            {
          	  if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
				{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " ����7ʧ�ܣ��ر�RCA�������ʧ��\n");
				CSTCPrint("[UDI2AOUTEST]����7:�Ѷ�RCA����\n");
				CSTCPrint("[UDI2AOUTEST]:Yes,RCA ���������\r\n");
	          	CSTCPrint("[UDI2AOUTEST]:No,RCA ��������� \r\n");
		       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����7ʧ��\n");
				}
			
            
				if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " ����8ʧ�ܣ��ر�SPDIF�������ʧ��\n");
					CSTCPrint("[UDI2AOUTEST]����8:�Ѷ�SPDIF����\n");
					   CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF ���������\r\n");
			              CSTCPrint("[UDI2AOUTEST]:No,SPDIF ��������� \r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����8ʧ��\n");
					
				}
            }

			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " ����9ʧ�ܣ��ر�HDMI�������ʧ��\n");
				CSTCPrint("[UDI2AOUTEST]����9:�Ѷ�HDMI����\n");
				CSTCPrint("[UDI2AOUTEST]:Yes,HDMI ���������\r\n");
				CSTCPrint("[UDI2AOUTEST]:No,HDMI ��������� \r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����9ʧ��\n");
			}

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"����10-1ʧ�ܣ�δ�ܴ�RCA�豸����\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"����10-2ʧ�ܣ�δ�ܴ�SPDIF�豸����\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"����10-3ʧ�ܣ�δ�ܴ�HDMI�豸����\n");
			}
			CSTCPrint("[UDI2AOUTEST]����10:�Ѷ�ƽ̨֧�ֵ������豸ȡ������(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
	       	CSTCPrint("[UDI2AOUTEST]:Yes,�����豸��������������������ͱ������쳣\r\n");
          		CSTCPrint("[UDI2AOUTEST]:No,�豸��������� \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����10ʧ��\n");
		}while(i);

	}

	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����11ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ�ܣ��رղ�����ʧ��\n");
		}
   	}
	return CSUDI_TRUE;
} 

//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:����CSUDIAOUTMute ������������
//@PRECONDITION:CSUDIAOUTUnmute ����CSUDIAOUT_ERROR_BAD_PARAMETER
//@INPUT:�߼�����Ĳ���
//@EXECUTIONFLOW:1.����CSUDIAOUTUnmute������������
//@EXECUTIONFLOW:2.ϣ������CSUDIAOUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0008( void )
{
	int i = 0;
	
	int nErrorParam[3] = {0};

	nErrorParam[0] = -1;
	nErrorParam[1] = 0;
	nErrorParam[2] = EM_UDIAOUT_HDMI+1;

	
	for(i=0; i<sizeof(nErrorParam)/sizeof(nErrorParam[0]); i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIAOUT_ERROR_BAD_PARAMETER ==CSUDIAOUTUnmute(nErrorParam[i])), "�������ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: ������Ƶ�����������ģʽΪEM_UDIAOUT_BYPASS����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ��������������Ƶ
//@INPUT: 1��eAudioDevice = ֧��AC3���͵Ľ����豸
//@INPUT: 2��eMode = EM_UDIAOUT_BYPASS
//@EXPECTATION: ÿһ�����������ֵ���ڲ���״̬�����ó�EM_UDIAOUT_BYPASS֮��Ӧ��������Ч
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��,����AC3��Ƶ�������������ý����������������
//@EXECUTIONFLOW: 2�����ƽ̨��֧��AC3���룬����CSUDIAOUTSetOPMode(eAudioDevice, EM_UDIAOUT_BYPASS)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3�����������豸���������
//@EXECUTIONFLOW: 4�����ƽ̨֧��AC3���룬����CSUDIAOUTSetOPMode(eAudioDevice, EM_UDIAOUT_DECODE)����������CSUDI_SUCCESS������
//@EXECUTIONFLOW: 5�����ƽ̨֧��AC3���룬��eAudioDevice�ϲ���"DolbyAC3��׼���Խ�Ŀ"���������ųɹ�,����������֮��ĺϷ��豸�����������
//@EXECUTIONFLOW: 6������CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW: 7������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_SetOPMode_0001(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "����4-1ʧ��");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "����4-2ʧ��");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "����4-3ʧ��");
		}
		CSTCPrint("[UDI2AOUTEST]����5:������ƽ̨֧�ֵ���������豸�Ľ���ģʽΪDECODE���Ƿ����е�����ӿڶ�Ӧ���豸����������� \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");
	}
	else
	{

      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		{
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����4ʧ��");
				CSTCPrint("[UDI2AOUTEST]����5:���ò���ģʽΪBYPASS���Ƿ��������䶼��������� \r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");
			}
         }
	}
	
	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����6ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����7ʧ�ܣ��رղ�����ʧ��\n");
		}
   	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: ������Ƶ�����������ģʽ���߼���ȷ��
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ������ƵSPDIF���
//@INPUT: 1��eAudioDevice = EM_UDIAOUT_RCA
//@INPUT: 2��eMode = EM_UDIAOUT_BYPASS
//@EXPECTATION: ���������������½���ִ��Decoder ���������򷵻�CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW: 1������CSUDIAOUTSetOPMode,eMode = EM_UDIAOUT_BYPASS,ϣ��CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW: 2������CSUDIAOUTSetOPMode,eMode = EM_UDIAOUT_DECODE,ϣ��CSUDI_SUCCESS
CSUDI_BOOL CSTC_AOUT_SetOPMode_0002(void)
{	
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	
	nResult = CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_BYPASS);

	CSTK_ASSERT_TRUE_FATAL((CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED == nResult), "EM_UDIAOUT_RCA�½���ΪEM_UDIAOUT_DECODE");

	nResult = CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== nResult), "EM_UDIAOUT_RCA��EM_UDIAOUT_DECODE Ӧ�óɹ�");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: ������Ƶ�����������ģʽ���߼���ȷ��
//@PRECONDITION: CSUDIAOUTSetOPMode ����CSUDIAOUT_ERROR_BAD_PARAMETER
//@INPUT: 1��eAudioDevice = �߼�����
//@INPUT: 2��eMode = �߼�����
//@EXPECTATION: ����CSUDIAOUTSetOPMode ����CSUDIAOUT_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1������CSUDIAOUTSetOPMode
//@EXECUTIONFLOW: 2��ϣ��CSUDIAOUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_AOUT_SetOPMode_0003(void)
{	
	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(-1, EM_UDIAOUT_DECODE), "�������ʧ��1");

	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(0, EM_UDIAOUT_DECODE), "�������ʧ��2");

	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI+1, EM_UDIAOUT_DECODE), "�������ʧ��3");

	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE-1), "�������ʧ��4");

	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_BYPASS+1), "�������ʧ��5");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: ����DD��DD+��ĿCSUDIAOUTSetOPMode���������ģʽʱSPDIF��HDMI��RCA���ܻ���Ӱ��
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ��������������Ƶ
//@PRECONDITION: 3�����Ž�Ŀ���������ģʽ
//@INPUT: 1��eAudioDevice = ֧��AC3���͵Ľ����豸
//@EXPECTATION: ÿһ�����������ֵ���ڲ���״̬�����ó�EM_UDIAOUT_BYPASS֮��Ӧ��������Ч
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��,����AC3��Ƶ�������������ý����������������
//@EXECUTIONFLOW: 2��CSUDIAOUTSetOPMode����HDMI����Ľ���ģʽ��SPDIF���Ϊ����ģʽ�����оƬ֧��DD+��DD���룬������ӿ����������PCM������ֻ��֧��DD������豸��������
//@EXECUTIONFLOW: 3��CSUDIAOUTSetOPMode����HDMI����Ľ���ģʽ��SPDIF���Ϊbypassģʽ�����оƬ֧��DD+��DD���룬��HDMI��RCA���PCM��SPDIF bypass���������ֻ��֧��DD������豸��������
//@EXECUTIONFLOW: 4��CSUDIAOUTSetOPMode����HDMI�����bypassģʽ��SPDIF���Ϊ����ģʽ�����оƬ֧��DD+��DD���룬��SPDIF��RCA���P������PCM��HDMI���������������ֻ��֧��DD������豸��������
//@EXECUTIONFLOW: 5��CSUDIAOUTSetOPMode����HDMI�����bypassģʽ��SPDIF���Ϊbypassģʽ�����оƬ֧��DD+��DD���룬RCA���P������PCM��SPDIF bypass�����HDMI���������������ֻ��֧��DD������豸��������
//@EXECUTIONFLOW: 6������CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW: 7������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_SetOPMode_0004(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");
	
	//EXECUTIONFLOW: 2��CSUDIAOUTSetOPMode����HDMI����Ľ���ģʽ��SPDIF���Ϊ����ģʽ��
	//���оƬ֧��DD+��DD���룬������ӿ����������PCM��
	//����ֻ��֧��DD������豸��������
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "����4-1ʧ��");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "����4-2ʧ��");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{	
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "����4-3ʧ��");
		}
		CSTCPrint("[UDI2AOUTEST]����2:������ƽ̨֧�ֵ���������豸�Ľ���ģʽΪDECODE���Ƿ������豸����������� \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");
	}
	else
	{
	
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		{
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����2ʧ��");
				CSTCPrint("[UDI2AOUTEST]����2:����SPDIF���ģʽΪBYPASS���Ƿ��������䶼��������� \r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");
			}
		}
        
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "����2ʧ��");
			CSTCPrint("[UDI2AOUTEST]����2:����HDMI���ģʽΪBYPASS���������Ϊ�����е�һ�� \r\n");	
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸֧��DD����Ӧ�����ΪDolby��ʽ����Ƶ����\r\n");
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸��֧��DD����Ӧ�ò�����κ���Ƶ����\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��\n");
		}
	}

	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{	
		//EXECUTIONFLOW: 3��CSUDIAOUTSetOPMode����HDMI����Ľ���ģʽ��SPDIF���Ϊbypassģʽ��
		//���оƬ֧��DD+��DD���룬��HDMI��RCA���PCM��SPDIF bypass���
		
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		{
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����3ʧ��");
				CSTCPrint("[UDI2AOUTEST]����3:����SPDIF���ģʽΪBYPASS������SPDIF�豸���ΪDolby��ʽ \r\n");			
			}
		}
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "����3ʧ��");
			CSTCPrint("[UDI2AOUTEST]����3:����HDMI���ģʽΪ�������������HDMI�豸�������������ΪPCM��ʽ \r\n");			
		}
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����3ʧ��\n");



			//EXECUTIONFLOW: 4��CSUDIAOUTSetOPMode����HDMI�����bypassģʽ��SPDIF���Ϊ����ģʽ��
		//���оƬ֧��DD+��DD���룬��SPDIF��RCA���P������PCM��HDMI���������
		
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "����4ʧ��");
				CSTCPrint("[UDI2AOUTEST]����4:����SPDIF���ģʽΪ�������������SPDIF�豸���ΪPCM��ʽ \r\n");			
			}
        }
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "����4ʧ��");
			CSTCPrint("[UDI2AOUTEST]����4:����HDMI���ģʽΪBYPASS���������Ϊ�����е�һ�� \r\n");			
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸֧��DD����Ӧ�����ΪDolby��ʽ����Ƶ����\r\n");
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸��֧��DD����Ӧ�ò�����κ���Ƶ����\r\n");		
        }
	
		CSTCPrint("[UDI2AOUTEST]RCA����Ƶ�������\r\n");	
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");



		//EXECUTIONFLOW: 5��CSUDIAOUTSetOPMode����HDMI�����bypassģʽ��SPDIF���Ϊbypassģʽ��
		//���оƬ֧��DD+��DD���룬RCA���P������PCM��SPDIF bypass�����HDMI�����������
		

      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����4ʧ��");
				CSTCPrint("[UDI2AOUTEST]����5:����SPDIF���ģʽΪBYPASS������SPDIF�豸���ΪDolby��ʽ \r\n"); 		
			}
        }

		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "����4ʧ��");
			CSTCPrint("[UDI2AOUTEST]����5:����HDMI���ģʽΪBYPASS���������Ϊ�����е�һ�� \r\n");			
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸֧��DD����Ӧ�����ΪDolby��ʽ����Ƶ����\r\n");
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸��֧��DD����Ӧ�ò�����κ���Ƶ����\r\n");		
		}
		CSTCPrint("[UDI2AOUTEST]����5:RCA����Ƶ�������\r\n");	
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����5ʧ��\n");

	}

		
	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����6ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n"); 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����7ʧ�ܣ��رղ�����ʧ��\n");
		
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: ����DD��DD+��ĿCSUDIAOUTSetOPMode���������ģʽʱSPDIF��HDMI��RCA���ܻ���Ӱ��
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ��������������Ƶ
//@PRECONDITION: 3�����Ž�Ŀǰ�������ģʽ
//@INPUT: 1��eAudioDevice = ֧��AC3���͵Ľ����豸
//@EXPECTATION: ÿһ�����������ֵ���ڲ���״̬�����ó�EM_UDIAOUT_BYPASS֮��Ӧ��������Ч
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��,����AC3��Ƶ����
//@EXECUTIONFLOW: 2�����оƬ֧��DD+��DD���룬CSUDIAOUTSetOPMode���ø����ӿ����Ϊ����ģʽ�� ��������PCM�����������������SPDIF��HDMIΪbypassģʽ����������Dolby�������
//@EXECUTIONFLOW: 3������CSUDIPLAYERStopֹͣ���Խ�Ŀ���Ų�CSUDIAOUTSetOPMode����HDMI����Ľ���ģʽ��SPDIF���Ϊbypassģʽ
//@EXECUTIONFLOW: 4�����²���AC3��Ƶ����������:���оƬ֧��DD+��DD���룬��HDMI��RCA���PCM��SPDIF bypass���������ֻ��֧��DD������豸��������
//@EXECUTIONFLOW: 5������CSUDIPLAYERStopֹͣ���Խ�Ŀ���Ų�CSUDIAOUTSetOPMode����HDMI�����bypassģʽ��SPDIF���Ϊ����ģʽ
//@EXECUTIONFLOW: 6�����²���AC3��Ƶ����������:���оƬ֧��DD+��DD���룬��SPDIF��RCA���������PCM��HDMI���������������ֻ��֧��DD������豸��������
//@EXECUTIONFLOW: 7������CSUDIPLAYERStopֹͣ���Խ�Ŀ���Ų�CSUDIAOUTSetOPMode����HDMI�����bypassģʽ��SPDIF���Ϊbypassģʽ
//@EXECUTIONFLOW: 8�����²���AC3��Ƶ����������:���оƬ֧��DD+��DD���룬RCA���������PCM��SPDIF bypass�����HDMI���������������ֻ��֧��DD������豸��������
//@EXECUTIONFLOW: 9������CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW: 10������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_AOUT_SetOPMode_0005(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;

	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");
	
	//EXECUTIONFLOW: 1��CSUDIAOUTSetOPMode����HDMI����Ľ���ģʽ��SPDIF���Ϊ����ģʽ��
	//���оƬ֧��DD+��DD���룬������ӿ����������PCM��
	//����ֻ��֧��DD������豸��������
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "����1-1ʧ��");
			CSTCPrint("[UDI2AOUTEST]����2:����RCA���ģʽΪDECODER \r\n");	
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "����1-2ʧ��");
			CSTCPrint("[UDI2AOUTEST]����2:����SPDIF���ģʽΪDECODER \r\n");	
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "����1-3ʧ��");
			CSTCPrint("[UDI2AOUTEST]����2:����HDMI���ģʽΪDECODER \r\n");	
		}

		CSTCPrint("[UDI2AOUTEST]����2:�Ƿ�֧��������豸����PCM������� \r\n");	
			
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��:�������������\n");
	}
	else
	{
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����1ʧ��");
				CSTCPrint("[UDI2AOUTEST]����2:����SPDIF���ģʽΪBYPASS���Ƿ��������䶼��������� \r\n");
			}
		}
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "����1ʧ��");
			CSTCPrint("[UDI2AOUTEST]����2:����HDMI���ģʽΪBYPASS���������Ϊ�����е�һ�� \r\n");			
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸֧��DD����Ӧ�����ΪDolby��ʽ����Ƶ����\r\n");
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸��֧��DD����Ӧ�ò�����κ���Ƶ����\r\n");
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����2ʧ��:�������������\n");
	}

 	//�������Ѿ���оƬ��֧��DD�����������в�����
 	//����ֻ�����оƬ֧��DD����������²���
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		//EXECUTIONFLOW: 3������CSUDIPLAYERStopֹͣ���Խ�Ŀ���Ų�CSUDIAOUTSetOPMode����HDMI����Ľ���ģʽ��SPDIF���Ϊbypassģʽ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����3ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n"); 

		
	
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����3ʧ��");
				CSTCPrint("[UDI2AOUTEST]����3:����SPDIF���ģʽΪBYPASS������SPDIF�豸���ΪDolby��ʽ \r\n"); 		
			}
		}
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "����3ʧ��");
			CSTCPrint("[UDI2AOUTEST]����3:����HDMI���ģʽΪ�������������HDMI�豸�������������ΪPCM��ʽ \r\n");			
		}
		//EXECUTIONFLOW: 4�����²���AC3��Ƶ����������:���оƬ֧��DD+��DD���룬��HDMI��RCA���PCM��SPDIF bypass���������ֻ��֧��DD������豸��������
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),"����4������Ŀ����ʧ��\n");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����4ʧ��\n");
		
		//EXECUTIONFLOW: 5������CSUDIPLAYERStopֹͣ���Խ�Ŀ���Ų�CSUDIAOUTSetOPMode����HDMI�����bypassģʽ��SPDIF���Ϊ����ģʽ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����5ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n"); 

		
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "����5ʧ��");
				CSTCPrint("[UDI2AOUTEST]����5:����SPDIF���ģʽΪ�������������SPDIF�豸���ΪPCM��ʽ \r\n"); 		
			}
        }
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "����5ʧ��");
			CSTCPrint("[UDI2AOUTEST]����5:����HDMI���ģʽΪBYPASS���������Ϊ�����е�һ�� \r\n");			
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸֧��DD����Ӧ�����ΪDolby��ʽ����Ƶ����\r\n");
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸��֧��DD����Ӧ�ò�����κ���Ƶ����\r\n");		
        }
		
		//EXECUTIONFLOW: 6�����²���AC3��Ƶ����������:���оƬ֧��DD+��DD���룬��SPDIF��RCA���P������PCM��HDMI���������������ֻ��֧��DD������豸��������
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),"����6������Ŀ����ʧ��\n");

		CSTCPrint("[UDI2AOUTEST]����5:RCA����Ƶ�������\r\n");	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����6ʧ��\n");

		//EXECUTIONFLOW: 7������CSUDIPLAYERStopֹͣ���Խ�Ŀ���Ų�CSUDIAOUTSetOPMode����HDMI�����bypassģʽ��SPDIF���Ϊbypassģʽ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����7ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n"); 
		
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����7ʧ��");
				CSTCPrint("[UDI2AOUTEST]����7:����SPDIF���ģʽΪBYPASS������SPDIF�豸���ΪDolby��ʽ \r\n"); 		
			}
		}
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "����7ʧ��");
			CSTCPrint("[UDI2AOUTEST]����7:����HDMI���ģʽΪBYPASS���������Ϊ�����е�һ�� \r\n");			
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸֧��DD����Ӧ�����ΪDolby��ʽ����Ƶ����\r\n");
			CSTCPrint("[UDI2AOUTEST]���HDMI�豸��֧��DD����Ӧ�ò�����κ���Ƶ����\r\n");		
        }
		
		//EXECUTIONFLOW: 8�����²���AC3��Ƶ����������:���оƬ֧��DD+��DD���룬RCA���P������PCM��SPDIF bypass�����HDMI���������������ֻ��֧��DD������豸��������
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),"����8������Ŀ����ʧ��\n");

		CSTCPrint("[UDI2AOUTEST]����7:RCA����Ƶ�������\r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����8ʧ��\n");
	}
	
	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����9ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n"); 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����10ʧ�ܣ��رղ�����ʧ��\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: ��������ͣ״̬���Ƿ��ܳɹ�������Ƶͨ�������ģʽ
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��ϵͳ��������������Ƶ
//@INPUT: 1��eAudioDevice = ֧��AC3���͵Ľ����豸����������SPDIFΪ���ý����豸
//@INPUT: 2��eMode = EM_UDIAOUT_BYPASS
//@EXPECTATION: ÿһ�����������ֵ������ͣ״̬���ܳɹ�������Ƶͨ�������ģʽ���ָ�����������������Ƶ�����õ�ģʽ��Ч
//@EXECUTIONFLOW: 1������Audio&Video Test_27Mbps_20070524.ts�������ڵ�Ƶ��,����AC3��Ƶ�������������ý������������������ΪPCM��ʽ
//@EXECUTIONFLOW: 2������CSUDIPLAYERPause�ӿڣ�ʹplayer������ͣ״̬����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3��������Ա�۲죬�������ý����������������
//@EXECUTIONFLOW: 4������CSUDIAOUTSetOPMode�ӿڣ����ģʽ�����EM_UDIAOUT_BYPASS
//@EXECUTIONFLOW: 5������CSUDIPLAYERResume�ӿڣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6��������Ա�۲���Խ�����������ý�������ָ����������ΪDolby��ʽ
//@EXECUTIONFLOW: 7������CSUDIPLAYERStopֹͣ���Խ�Ŀ���ţ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIPLAYERClose�رղ���������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_AOUT_SetOPMode_0006(void)
{
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"����1ʧ��\n");
		
			CSTCPrint("����1:������SPDIFͨ�����ģʽΪDECODEģʽ����ȷ�����ý������������������ΪPCM��ʽ\r\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����1ʧ��\n");

			nRet = CSUDIPLAYERPause(hPlayer);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nRet), "����2ʧ��\n");

			CSTCPrint("����3:����ͣ���ţ����ý���������ͣ�������?\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����3ʧ��\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "����4ʧ��");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer), "����5ʧ��\n");

			CSTCPrint("����6:������SPDIFͨ�����ģʽΪBYPASSģʽ�һָ����ţ���ȷ�����ý�������ָ����������ΪDolby��ʽ\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "����6ʧ��\n");
		}   
	}
	
	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����7ʧ�ܣ�ֹͣ���Ž�Ŀʧ��\n");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����8ʧ�ܣ��رղ�����ʧ��\n");
		}
   	}
   	
	return CSUDI_TRUE;
}



