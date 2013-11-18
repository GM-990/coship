/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_screentestcase.h"
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_player.h"
#include "udi2_demux.h"
#include "udi2_descramble.h"
#include "udi2_video.h"
#include "udi2_os.h"
#include "udi2_screen.h"
#include "udi2_tuner.h"
#include "udiplus_debug.h"
#include "cs_testkit.h"
#include "../cs_udi2testcase.h"

static CSUDISCREENResolution_E g_scResolution[2];
	
CSUDI_BOOL CSTC_SCREEN_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_SCREEN_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//ͨ����ȡ�����ļ���ѯƽ̨�Ƿ�֧�ָ�����Ƶ���ͨ������
static CSUDI_BOOL NotSupportHD( )
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	const char acSection[] = "SCREEN";
	char acKey[] = "EM_UDI_VOUT_DEVICE_HD";
	char acResult[255] = {0};
	int nResultSize = (int)sizeof(acResult);

	if (CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(acSection, acKey, acResult, nResultSize))
	{
		if (0 == CSTKGetIntFromStr(acResult, 10))
		{
			bRet = CSUDI_TRUE;	// ��ʾ��֧�ָ���ƽ̨
		}
	}
	else
	{
		CSTCPrint("��ȡEM_UDI_VOUT_DEVICE_HD��������Ϣʧ��!\n");
	}
	
	return bRet;
}

//ͨ�������ļ���ѯƽ̨�Ƿ�߱���ͬԴ
static CSUDI_BOOL IsShareHDDisplay()
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char cBuf[32] = {0};
	
	memset(cBuf, '\0', sizeof(cBuf));
	if (CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("OSG", "CS_OSGUDI2_SHARE_HD_DISPLAY_SERFACE", cBuf, sizeof(cBuf)))
	{
		if (1 == CSTKGetIntFromStr(cBuf,10))
		{
			bRet = CSUDI_TRUE;
		}
	}
	else
	{
		CSTCPrint("��ȡCS_OSGUDI2_SHARE_HD_DISPLAY_SERFACE��������Ϣʧ��!\n");
	}

	return bRet;
}

//Ѱ����Ƶ������
static int FindVideoDecoder(int nVideoDecCount, CSUDIVIDStreamType_E eFindVideoType)
{
	CSUDIVIDEOCapability_S sVideoCapabilityInfo;
	int nVideoID = -1;
	int i = 0;
	int k = 0;
	
	for (i=0; i<nVideoDecCount; i++)
	{
		if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOGetCapability(i, &sVideoCapabilityInfo)))
		{
			break;
		}

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

		if (nVideoID != -1)
		{
			break;
		}
	}

	return nVideoID;
}


//Ѱ��ƽ̨֧�ֵķֱ���
static CSUDI_BOOL IsScreenSupport(CSUDISCREENType_E * peScreenDevice, CSUDISCREENResolution_E eResolution)
{
	CSUDISCREENCapability_S sCapabilityInfo;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int k = 0;

	if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDISCREENGetCapability( *peScreenDevice, &sCapabilityInfo)))
	{
		return CSUDI_FALSE;
	}

	for (k=0; k<EM_UDISCREEN_RESOLUTION_NUM; k++)
	{
		if (eResolution == sCapabilityInfo.m_eResolution[k])
		{
			bRet = CSUDI_TRUE;
			break;
		}
		if (EM_UDISCREEN_RESOLUTION_INVALID == sCapabilityInfo.m_eResolution[k])
		{
			break;
		}
	}

	return bRet;   
}


// ���ű�����Ƶ��ĿԴ
static CSUDI_HANDLE PlaySDProgram(CSUDIVIDStreamType_E eVideoType, CSUDISCREENResolution_E eResolution)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIDEMUXCapability_S  sDemuxCapabilityInfo;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	int nVideoDecCount = 0;
	int nVideoIndex = -1;
	int nDemuxCount = 0 ; 
	int nDemuxIndex = -1;
	int i = 0;

	if (CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoDecCount))
	{
		if (nVideoDecCount > 0)
		{
			nVideoIndex = FindVideoDecoder(nVideoDecCount, eVideoType);
		}
	}

	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))
	{
		if (nDemuxCount > 0)
		{
			for(i=0; i<nDemuxCount; i++)
			{
				if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))
				{
					if (EM_UDI_DEMUX_PLAY == (sDemuxCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_PLAY))  
					{
						nDemuxIndex = i;
						break;
					}
				}
			}
		}
	}

	if ((nVideoIndex==-1) || (nDemuxIndex==-1))
	{
		CSTCPrint("��ȡVideo , demux����\n");
		
		return CSUDI_NULL;
	}

	sPlayerChnl.m_nDemux = nDemuxIndex;
	sPlayerChnl.m_nVideoDecoder = nVideoIndex;
	sPlayerChnl.m_nAudioDecoder = 0;

	//����������Ƶ��
	if (CSTC_UDI2PortingLock(0, "Audio&Video Test_27Mbps_20070524.ts"))
	{
		CSUDIVIDEOSetStopMode(nVideoIndex, EM_UDIVIDEO_STOPMODE_BLACK);

		if (CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer))
		{
			if (hPlayer != CSUDI_NULL)
			{
				if (EM_UDISCREEN_RESOLUTION_PAL == eResolution)
				{
					sStreamInfo.m_nPid = 1140;  //PAL��ĿԴ
					sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
					sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
				}

				if (EM_UDISCREEN_RESOLUTION_NTSC == eResolution)
				{
					sStreamInfo.m_nPid = 5137;  //NTSC��ĿԴ
					sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
					sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
				}

				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, &sStreamInfo, 1, CSUDI_NULL)))
				{
					CSTCPrint("Player set stream ʧ��\n");
					
					if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
					{
						CSTCPrint("�رղ�����ʧ��\n");
					}
					
					return CSUDI_NULL;
				}

				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)))
				{
					CSTCPrint("Player start ʧ��\n");
					
					if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
					{
						CSTCPrint("�رղ�����ʧ��\n");
					}
					
					return CSUDI_NULL;
				}

				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOShow(0,TRUE)))
				{
					CSTCPrint("Video show ʧ��\n");
					
					if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
					{
						CSTCPrint("�رղ�����ʧ��\n");
					}
					
					return CSUDI_NULL;
				}
			}
		}
		else
		{
			CSTCPrint("Player open ʧ��\n");
		}
	}
	else
	{
		CSTCPrint("����Audio&Video Test_27Mbps_20070524.ts ʧ��\n");
	}

	return hPlayer;	
}

// ���Ÿ�����Ƶ��ĿԴ
static CSUDI_HANDLE PlayHDProgram(CSUDIVIDStreamType_E eVideoType, CSUDISCREENResolution_E eResolution)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIDEMUXCapability_S  sDemuxCapabilityInfo;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	int nVideoDecCount = 0;
	int nVideoIndex = -1;
	int nDemuxCount = 0 ; 
	int nDemuxIndex = -1;
	int i = 0;
	
	if (CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoDecCount))
	{
		if (nVideoDecCount > 0)
		{
			nVideoIndex = FindVideoDecoder(nVideoDecCount, eVideoType);
		}
	}

	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))
	{
		if (nDemuxCount > 0)
		{
			for(i=0; i<nDemuxCount; i++)
			{
				if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))
				{
					if (EM_UDI_DEMUX_PLAY == (sDemuxCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_PLAY))  
					{
						nDemuxIndex = i;
						break;
					}
				}
			}
		}
	}

	if ((nVideoIndex==-1) || (nDemuxIndex==-1))
	{
		CSTCPrint("��ȡVideo , demux����\n");
		
		return CSUDI_NULL;
	}

	sPlayerChnl.m_nDemux = nDemuxIndex;
	sPlayerChnl.m_nVideoDecoder = nVideoIndex;
	sPlayerChnl.m_nAudioDecoder = 0;

	//����������Ƶ��
	switch(eResolution)
	{
		case EM_UDISCREEN_RESOLUTION_720P_50HZ :
		{
			if (CSTC_UDI2PortingLock(0, "SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts"))
			{
				sStreamInfo.m_nPid = 0x328;  
				sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
				sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
			}
			else
			{
				CSTCPrint("����SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.tsʧ��\n");

				return CSUDI_NULL;
			}
		}
		break;
		case EM_UDISCREEN_RESOLUTION_720P :
		{
			if (CSTC_UDI2PortingLock(0, "mpeg2hd+h264sd.ts"))
			{
				sStreamInfo.m_nPid = 0x328;  
				sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
				sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
			}
			else
			{
				CSTCPrint("����mpeg2hd+h264sd.ts ʧ��\n");
				
				return CSUDI_NULL;
			}
		}
		break;
		case EM_UDISCREEN_RESOLUTION_1080I_50HZ :
		{
			if (CSTC_UDI2PortingLock(0, "SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts"))
			{
				sStreamInfo.m_nPid = 0x33c;  
				sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
				sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
			}
			else
			{
				CSTCPrint("����SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts ʧ��\n");
				
				return NULL;
			}
		}
		break;
		case EM_UDISCREEN_RESOLUTION_1080I :
		{
			if (CSTC_UDI2PortingLock(0, "mpeg2hd+h264sd.ts"))
			{
				sStreamInfo.m_nPid = 0x33c;  
				sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
				sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
			}
			else
			{
				CSTCPrint("����mpeg2hd+h264sd.ts ʧ��\n");
				
				return NULL;
			}
		}
		break;
		default : break;
	}

	if (CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer))
	{
		if (hPlayer != CSUDI_NULL)
		{
			if (EM_UDISCREEN_RESOLUTION_PAL == eResolution)
			{
				sStreamInfo.m_nPid = 1140;  //PAL��ĿԴ
				sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
				sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
			}

			if (EM_UDISCREEN_RESOLUTION_NTSC == eResolution)
			{
				sStreamInfo.m_nPid = 5137;  //NTSC��ĿԴ
				sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
				sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
			}

			if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, &sStreamInfo, 1, CSUDI_NULL)))
			{
				CSTCPrint("Player set stream ʧ��\n");
				
				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
				{
					CSTCPrint("�رղ�����ʧ��\n");
				}
				
				return CSUDI_NULL;
			}

			if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)))
			{
				CSTCPrint("Player start ʧ��\n");
				
				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
				{
					CSTCPrint("�رղ�����ʧ��\n");
				}
				
				return CSUDI_NULL;
			}

			if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOShow(0,TRUE)))
			{
				CSTCPrint("Video show ʧ��\n");
				
				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
				{
					CSTCPrint("�رղ�����ʧ��\n");
				}
				
				return CSUDI_NULL;
			}
		}
	}
	else
	{
		CSTCPrint("Player open ʧ��\n");
	}

	return hPlayer;	
}

//��Ļ�ֱ��ʸı�ص�����
static void CSUDISCREENCallback (CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice,void * pvUserData)
{
	(void)(eEvt);
	(void)(eScreenDevice);
	(void)(pvUserData);
}

//�����û�����
static int g_nUserData;

static CSUDISCREENEvent_E g_eEvt;
static CSUDISCREENType_E g_eScreenDevice;

//��Ļ�ֱ��ʸı�ص�����
static void CSUDISCREENChangeCallback (CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice,void * pvUserData)
{
	g_nUserData = *(int*)pvUserData;
	g_eEvt = eEvt;
	g_eScreenDevice = eScreenDevice;
}

//@CASEGROUP:CSUDISCREENAddCallback 
//@DESCRIPTION:���Իص�����ΪNULL���쳣���
//@PRECONDITION:
//@INPUT:1. fnScreenCallback = NULL
//@INPUT:2. pvUserData = NULL
//@EXPECTATION: ����CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:����CSUDISCREENAddCallback,�ص���������NULL,��������ֵΪCSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_AddCallback_0001( void )
{
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;
	int nUserData = 0x1234;

	bResult = CSUDISCREENAddCallback(CSUDI_NULL, &nUserData);

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == bResult), "�������ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENAddCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:�������в�����Ϊ����,ע��ɹ�����ɾ���ɹ������
//@PRECONDITION:
//@INPUT:1. fnScreenCallback = �Ϸ�������ַ
//@INPUT:2. pvUserData = NULL
//@EXPECTATION: ע��ɹ�����ɾ���ɹ�
//@EXECUTIONFLOW:1. ����CSUDISCREENAddCallbackע�ᴫ��Ϸ�����,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2. ����CSUDISCREENDelCallbackɾ��ע��ĺ���,��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_SCREEN_TEST_IT_AddCallback_0002( void )
{
	int nUserData = 0x1234;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENCallback, CSUDI_NULL)), "ע��ص�����ʧ��1");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENCallback, &nUserData)), "ע��ص�����ʧ��2");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENChangeCallback, CSUDI_NULL)), "ע��ص�����ʧ��3");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENChangeCallback, &nUserData)), "ע��ص�����ʧ��4");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENCallback, CSUDI_NULL)), "ɾ���ص�����ʧ��1");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENCallback, &nUserData)), "ɾ���ص�����ʧ��2");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENChangeCallback, CSUDI_NULL)), "ɾ���ص�����ʧ��3");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENChangeCallback, &nUserData)), "ɾ���ص�����ʧ��4");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENAddCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:����ע�������������ע����32�����
//@PRECONDITION:
//@INPUT:ע��ʱ������ͬ�Ļص�����,��ͬ��pvUserData��ַ
//@EXPECTATION: ǰ32���ص�ע��ɹ�,32�Ժ�ע��ʧ��
//@EXECUTIONFLOW:1. ��������CSUDISCREENAddCallbackע��ص�����33��,ÿ��ע�ᴫ��Ļص�����һ��,���ǻص�����������ַ��һ��
//@EXECUTIONFLOW: ����[0~31]Ӧ��ע��ɹ�,��32��Ӧ��ע��ʧ��
//@EXECUTIONFLOW:2. ����CSUDISCREENDelCallbackɾ��ע��ɹ��Ļص�����,����ɾ���ɹ�
CSUDI_BOOL CSTC_SCREEN_TEST_IT_AddCallback_0003( void )
{
	int i = 0;
	int j = 0;

	for (i=0; i<33; i++)
	{
		if (32 == i)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDISCREENAddCallback(CSUDISCREENCallback, (int *)i), "����1ʧ��\n");
			 break;
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENCallback, (int *)i), "����1ʧ�ܣ�ע�᲻�ɹ�\n");
		j ++;
	}

	CSTK_FATAL_POINT
	{
		for (i=0; i<j; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENCallback, (int *)i), "����2ʧ�ܣ�ɾ��ע�᲻�ɹ�\n");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENDelCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:����CSUDISCREENDelCallback�Դ�������ļ������
//@INPUT:ɾ��ʱ����Ĳ�������
//@EXECUTIONFLOW:1. �������Ĳ�����CSUDISCREENDelCallback
//@EXECUTIONFLOW:2. ��������CSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_DelCallback_0001( void )
{
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	
	nResult = CSUDISCREENDelCallback(CSUDI_NULL, CSUDI_NULL);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "�������ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENDelCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:����CSUDISCREENDelCallbackɾ��δ����Ļص�����
//@PRECONDITION:
//@INPUT:����Ϸ�����
//@EXECUTIONFLOW:1. ע��ص�����������ΪCSUDISCREENCallback��&nUserData1
//@EXECUTIONFLOW:2. ����ɾ���ص�����������ΪCSUDISCREENCallback��&nUserData2������ʧ��
//@EXECUTIONFLOW:3. ����ɾ���ص�����������ΪCSUDISCREENChangeCallback��&nUserData1������ʧ��
//@EXECUTIONFLOW:4. ����ɾ���ص�����������ΪCSUDISCREENCallback��&nUserData1�������ɹ�
CSUDI_BOOL CSTC_SCREEN_TEST_IT_DelCallback_0002( void )
{
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	int nUserData1 = 0x1234;
	int nUserData2 = 0x4321;
	
	nResult = CSUDISCREENAddCallback(CSUDISCREENCallback, &nUserData1);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "ע��ص�����ʧ��");

	nResult = CSUDISCREENDelCallback(CSUDISCREENCallback, &nUserData2);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "ɾ��δע��Ļص�����Ӧ��ʧ��1");
	
	nResult = CSUDISCREENDelCallback(CSUDISCREENChangeCallback, &nUserData1);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "ɾ��δע��Ļص�����Ӧ��ʧ��2");

	nResult = CSUDISCREENDelCallback(CSUDISCREENCallback, &nUserData1);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "ɾ���ص�����ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDISCREENDelCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:����1000��ɾ��ע��ص�����
//@EXPECTATION:ÿ�ζ�ע��ɹ�����ɾ���ɹ�
//@INPUT:����Ϸ�����
//@EXECUTIONFLOW:1. ����CSUDISCREENAddCallback����Ϸ��Ļص������Լ�userdata,����ע��ɹ�
//@EXECUTIONFLOW:2.����CSUDISCREENDelCallbackɾ��ע��Ļص�����,�������سɹ�
//@EXECUTIONFLOW:3.�ظ�����1��2��1000��,����ÿ�ζ����سɹ�
CSUDI_BOOL CSTC_SCREEN_TEST_IT_DelCallback_0003( void )
{
	int i=0;
	int nUserData1 = 0x1234;
	
	for(i=0;i<1000;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENCallback,(void*)nUserData1),
				"step 1 fail to add screen callback\n");
		CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENCallback, (void*)nUserData1),
				"step 2 fail to delete screen callback\n");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENGetResolution 
//@DESCRIPTION:��û�е���CSUDISCREENSetResolution����ǰ,����CSUDISCREENGetResolutionӦ�ܻ�ȡĬ��ֵ
//@PRECONDITION: ϵͳû�е���CSUDISCREENSetResolution
//@EXPECTATION: ����CSUDISCREENGetResolutionӦ���سɹ�,���ܻ�ȡĬ��ֵ
//@EXECUTIONFLOW:1. ����CSUDISCREENGetResolution��ȡ�������ͨ����Ĭ�Ϸֱ���Ӧ���سɹ�,����ȡ��ֵӦ��EM_UDISCREEN_RESOLUTION_INVALID��EM_UDISCREEN_RESOLUTION_576P֮��
//@EXECUTIONFLOW:2. ���Ϊ����ƽ̨,����CSUDISCREENGetResolution��ȡ�������ͨ����Ĭ�Ϸֱ���Ӧ���سɹ�,����ȡ��ֵӦ��EM_UDISCREEN_RESOLUTION_576P��EM_UDISCREEN_RESOLUTION_NUM֮��
CSUDI_BOOL CSTC_SCREEN_TEST_IT_BeforeSetResolution_0001( void )
{
	CSUDI_Error_Code nResult = 0;
	CSUDISCREENResolution_E sResolution = EM_UDISCREEN_RESOLUTION_INVALID;
	
	nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD, &sResolution);
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == nResult) && (sResolution > EM_UDISCREEN_RESOLUTION_INVALID && sResolution <= EM_UDISCREEN_RESOLUTION_576P), 
		"��ȡ�������ͨ��Resolutionȱʡֵʧ��");

	if (!NotSupportHD())
	{
		sResolution = EM_UDISCREEN_RESOLUTION_INVALID;
		nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &sResolution);
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == nResult) && (sResolution >= EM_UDISCREEN_RESOLUTION_720P && sResolution < EM_UDISCREEN_RESOLUTION_NUM), 
			"��ȡ�������ͨ��Resolutionȱʡֵʧ��");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���������豸ΪNULL�����
//@PRECONDITION:
//@INPUT:1.peScreenDevice = NULL
//@INPUT:2.peResolution = CSUDISCREENResolution_E��������
//@INPUT:3.nCount=2
//@EXPECTATION: ���ش���ֵCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:����CSUDISCREENSetResolution peScreenDevice����NULL,��������CSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0001( void )
{
	CSUDISCREENResolution_E scResolution[2];
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;

	bResult = CSUDISCREENSetResolution(CSUDI_NULL, scResolution, 2);

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == bResult), "�������ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Էֱ���ΪNULL�����
//@PRECONDITION:
//@INPUT:1.peScreenDevice = �豸����
//@INPUT:2.peResolution = NULL
//@INPUT:3.nCount=2
//@EXPECTATION: ���ش���ֵCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:����CSUDISCREENSetResolution peResolution����NULL,��������CSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0002( void )
{
	CSUDISCREENType_E scScreenDevice[2];
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;

	bResult = CSUDISCREENSetResolution(scScreenDevice, CSUDI_NULL, 2);

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == bResult), "�������ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:����nCount �����CSUDISCREENType_E��CSUDISCREENResolution_E�Լ�nCount��ƥ������
//@PRECONDITION:
//@INPUT:1. peScreenDevice = �豸����,CSUDISCREENResolution_E��������,nCount = 0
//@INPUT:2. peScreenDevice = �豸����,CSUDISCREENResolution_E��������,nCount = -1
//@INPUT:2. peScreenDevice = �豸����,CSUDISCREENResolution_E��������,nCount = 3
//@EXPECTATION: ���ش���ֵCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1. ����CSUDISCREENSetResolution nCount����0,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2. ����CSUDISCREENSetResolution nCount����-1,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3. ����CSUDISCREENSetResolution nCount����3,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4. ����CSUDISCREENSetResolution�� CSUDISCREENType_E��CSUDISCREENResolution_E�еĳ�Ա����,��������CSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0003( void )
{
	CSUDISCREENType_E scScreenDevice[2];
	CSUDISCREENType_E scScreenDevice_4[1];
	CSUDISCREENResolution_E scResolution[2];
	CSUDISCREENResolution_E scResolution_4[1];
	 		 
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice, scResolution, 0)),
		"�������ʧ��1");

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice, scResolution, -1)),
		"�������ʧ��2");

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice, scResolution, 3)),
		"�������ʧ��3");

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice_4, scResolution, 2)),
		"�������ʧ��4-1");

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice, scResolution_4, 2)),
		"�������ʧ��4-2");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Խ�ĿԴ�Ǳ���PAL�������,���豸���и�����ʽת��
//@PRECONDITION:
//@INPUT:1. ���ű���PAL��ĿԴ
//@EXPECTATION:1. �����豸���÷ֱ���ʱ�ֱ��ʷ��������л�
//@EXPECTATION:2. ������͸�������ĳ�Ƶ�ͽ�ĿԴ�ĳ�Ƶһ��,����Ϊ50HZʱҪ�������κ��쳣
//@EXPECTATION:3. ���������͸��峡Ƶ��һ��ʱ����������쳣,����Ҫ������
//@REMARK:1.�豸�Ƿ�֧��ĳ�ֱַ�����ͨ��CSUDISCREENGetCapability���
//@REMARK:2.���ƽ̨֧��1080P,���������Ϊ1080Pʱ,ȷ�ϻ����Ƿ�������Ҫ����Ҳ֧��1080P
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ��������
//@EXECUTIONFLOW:1.1 ����������Ƶ��
//@EXECUTIONFLOW:1.2 ����CSUDIVIDEOGetCount��ȡvideo����������
//@EXECUTIONFLOW:1.3 ����CSUDIVIDEOGetCapability����Ѱ�ҵ�һ��֧�ֱ�������decoder,��������Ϊ��Ŀ���ŵ���Ƶ������
//@EXECUTIONFLOW:1.4 ����CSUDIDEMUXGetCount��ȡdemux������
//@EXECUTIONFLOW:1.5 ����CSUDIDEMUXGetCapability����Ѱ�ҵ�һ��֧��EM_UDI_DEMUX_PLAY��demux���ڽ�Ŀ���ŵ�demux
//@EXECUTIONFLOW:1.6 ����CSUDIPLAYEROpen��һ��������,�����򿪳ɹ�
//@EXECUTIONFLOW:1.7 ����CSUDIPLAYERSetStream���ò�����������
//@EXECUTIONFLOW:1.8 ����CSUDIPLAYERStart���Ų�������,��������Ƶ���
//@EXECUTIONFLOW:2. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:3. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:4. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:5. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:6. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:7. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:8. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:9. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:10. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:11. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:12. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:13. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:14. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0004( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2SCREENTEST]:��Ƶ��Ŀ���Ż����Ƿ�����?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	////���ڱ���ƽ̨��֧�ֱ������ͨ��һ�����ã��ֵ����Ա���ͨ������PAL��NTSC��ʽת���������óɹ��һ������������ͨ����
	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����1ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]:Yes,���÷ֱ���ΪPAL�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]:No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��\n");
			
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����3ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]:Yes,���÷ֱ���ΪNTSC�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]:No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����3ʧ��\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����1ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,���÷ֱ���ΪPAL�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����4ʧ��\n");
	}
	else
	{
		CSTCPrint("********��ȷ��������SD/HD ���������***********\n");
	   	CSTKWaitAnyKey();
	   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ����ǰ�ֱ���ʧ��\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����2ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����3ʧ��\n"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����3ʧ��\n");

		 //���ƽ̨֧��1080P 50HZ
		 if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
		 {
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����4ʧ��\n"); 
			CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
			CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����4ʧ��\n");
		 }

		 CSTCPrint("[UDI2SCREENTEST] ���潫���豸������ʽת���Զ������ԣ����������ʼ...\n");
		 CSTKWaitAnyKey();

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����5ʧ��\n");
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����6ʧ��\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����7ʧ��\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����8ʧ��\n"); 
		CSUDIOSThreadSleep(1000);

		//���ƽ̨֧��1080i 60HZ  
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����9ʧ��\n"); 
			CSUDIOSThreadSleep(1000);

			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����10ʧ��\n"); 
			CSUDIOSThreadSleep(1000);
		}

		//���ƽ̨֧��1080p 50HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����11ʧ��\n"); 
			CSUDIOSThreadSleep(1000);
		}

		 //���ƽ̨֧��1080P 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����12ʧ��\n"); 
			CSUDIOSThreadSleep(1000);

			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����13ʧ��\n"); 
			CSUDIOSThreadSleep(1000);
		}
	}

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ���
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "����14ʧ��\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
			}
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

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Խ�ĿԴ�Ǳ���NTSC�������,���豸���и�����ʽת��
//@PRECONDITION:
//@INPUT:1. ���ű���NTSC��ĿԴ
//@EXPECTATION:1. �����豸���÷ֱ���ʱ�ֱ��ʷ��������л�
//@EXPECTATION:2. ������͸�������ĳ�Ƶ�ͽ�ĿԴ�ĳ�Ƶһ��,����Ϊ60HZʱҪ�������κ��쳣
//@EXPECTATION:3. ���������͸��峡Ƶ��һ��ʱ����������쳣,����Ҫ������
//@REMARK:1.�豸�Ƿ�֧��ĳ�ֱַ�����ͨ��CSUDISCREENGetCapability���
//@REMARK:2.���ƽ̨֧��1080P,���������Ϊ1080Pʱ,ȷ�ϻ����Ƿ�������Ҫ����Ҳ֧��1080P
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���NTSC����,��ϸ������μ�CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P,�����ֱ��������л�,��������
//@EXECUTIONFLOW:3. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I,�����ֱ��������л�,��������
//@EXECUTIONFLOW:4. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P,�����ֱ��������л�,��������
//@EXECUTIONFLOW:5. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:6. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:7. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:8. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:9. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:10. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:11. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:12. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:13. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:14. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0005( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_NTSC);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2SCREENTEST]::��Ƶ��Ŀ���Ż����Ƿ�����?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	////���ڱ���ƽ̨��֧�ֱ��塢����ͨ��һ�����ã��ֵ����Ա���ͨ������PAL��NTSC��ʽת���������óɹ��һ������������ͨ����
	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����3ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,���÷ֱ���ΪPAL�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����1ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,���÷ֱ���ΪNTSC�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����3ʧ��\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����3ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,���÷ֱ���ΪPAL�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����4ʧ��\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ����ǰ�ֱ���ʧ��\n");

		CSTCPrint("********��ȷ��������SD/HD ���������***********\n");
	  	CSTKWaitAnyKey();
		
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����2ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��\n");

		 //���ƽ̨֧��1080i 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����3ʧ��\n");
			CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
			CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����3ʧ��\n");
		}
		 
		//���ƽ̨֧��1080p 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����4ʧ��\n"); 
			CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
			CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����4ʧ��\n");
		}

		CSTCPrint("[UDI2SCREENTEST] ���潫���豸������ʽת���Զ������ԣ����������ʼ..\n");
		CSTKWaitAnyKey();

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����5ʧ��\n");
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����6ʧ��\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����7ʧ��\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����8ʧ��\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����9ʧ��\n"); 
		CSUDIOSThreadSleep(1000);

		//���ƽ̨֧��1080i 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����10ʧ��\n"); 
			CSUDIOSThreadSleep(1000);
		}

		//���ƽ̨֧��1080p 50HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����11ʧ��\n"); 
			CSUDIOSThreadSleep(1000);

			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����12ʧ��\n"); 
			CSUDIOSThreadSleep(1000);
		}

		//���ƽ̨֧��1080p 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����13ʧ��\n"); 
			CSUDIOSThreadSleep(1000);
		}
	}

	 CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ���
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "����14ʧ��\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
			}
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

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Խ�ĿԴ�Ǹ���EM_UDISCREEN_RESOLUTION_720P_50HZ�������,�Ա���͸����豸���б�����ʽת��,�ò���������Ҫ���ڸ���ƽ̨
//@PRECONDITION:ƽ̨֧�ָ������
//@INPUT:1. ���Ÿ���720P_50HZ��ĿԴ
//@EXPECTATION:1. �����豸���÷ֱ���ʱ�ֱ��ʷ��������л�
//@EXPECTATION:2. ������͸�������ĳ�Ƶ�ͽ�ĿԴ�ĳ�Ƶһ��,����Ϊ60HZʱҪ�������κ��쳣
//@EXPECTATION:3. ���������͸��峡Ƶ��һ��ʱ����������쳣,����Ҫ������
//@REMARK:1.�豸�Ƿ�֧��ĳ�ֱַ�����ͨ��CSUDISCREENGetCapability���
//@REMARK:2.���ƽ̨֧��1080P,���������Ϊ1080Pʱ,ȷ�ϻ����Ƿ�������Ҫ����Ҳ֧��1080P
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ÿ���720P_50HZ����,��ϸ��������:
//@EXECUTIONFLOW:1.1 ����������Ƶ��
//@EXECUTIONFLOW:1.2 ����CSUDIVIDEOGetCount��ȡvideo����������
//@EXECUTIONFLOW:1.3 ����CSUDIVIDEOGetCapability����Ѱ�ҵ�һ��֧�ָ�������decoder,��������Ϊ��Ŀ���ŵ���Ƶ������
//@EXECUTIONFLOW:1.4 ����CSUDIDEMUXGetCount��ȡdemux������
//@EXECUTIONFLOW:1.5 ����CSUDIDEMUXGetCapability����Ѱ�ҵ�һ��֧��EM_UDI_DEMUX_PLAY��demux���ڽ�Ŀ���ŵ�demux
//@EXECUTIONFLOW:1.6 ����CSUDIPLAYEROpen��һ��������,�����򿪳ɹ�
//@EXECUTIONFLOW:1.7 ����CSUDIPLAYERSetStream���ò�����������
//@EXECUTIONFLOW:1.8 ����CSUDIPLAYERStart���Ų�������,��������Ƶ���
//@EXECUTIONFLOW:2. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:3. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:4. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:5. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:6. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:7. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:8. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:9. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:10. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:11. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:12. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:13. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:14. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0006( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	if (NotSupportHD())
	{
		CSTCPrint("[UDI2SCREENTEST]  ���������ڲ��Ը���ƽ̨\r\n");
		return CSUDI_TRUE;
	}

	hPlayer = PlayHDProgram(EM_UDI_VID_STREAM_H264 , EM_UDISCREEN_RESOLUTION_720P_50HZ);  //���Խ�ĿԴ�Ǹ���EM_UDISCREEN_RESOLUTION_720P_50HZ
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��");

	CSTCPrint("[UDI2SCREENTEST]::��Ƶ��Ŀ���Ż����Ƿ�����?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ����ǰ�ֱ���ʧ��\n");

	CSTCPrint("********��ȷ��������SD/HD ���������***********\n");
	CSTKWaitAnyKey();
	
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����2ʧ��");
	CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��");

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����3ʧ��"); 
	CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����3ʧ��");

	 //���ƽ̨֧��1080P 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����4ʧ��"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����4ʧ��");
	}

	CSTCPrint("[UDI2SCREENTEST] ���潫���豸������ʽת���Զ�������...\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����5ʧ��");
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����6ʧ��"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����7ʧ��"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����8ʧ��"); 
	CSUDIOSThreadSleep(3000);

	//���ƽ̨֧��1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����9ʧ��"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����10ʧ��"); 
		CSUDIOSThreadSleep(3000);
	}

	//���ƽ̨֧��1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����11ʧ��"); 
		CSUDIOSThreadSleep(3000);
	}

	//���ƽ̨֧��1080P 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����12ʧ��"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����13ʧ��"); 
		CSUDIOSThreadSleep(3000);
	}

	CSTK_FATAL_POINT
	{
		 //�ָ�����ǰ�ֱ�
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
		}

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Խ�ĿԴ�Ǹ���EM_UDISCREEN_RESOLUTION_720P�������,�Ա���͸����豸���б�����ʽת��,�ò���������Ҫ���ڸ���ƽ̨
//@PRECONDITION:ƽ̨֧�ָ������
//@INPUT:1. ���Ÿ���720P��ĿԴ
//@EXPECTATION:1. �����豸���÷ֱ���ʱ�ֱ��ʷ��������л�
//@EXPECTATION:2. ������͸�������ĳ�Ƶ�ͽ�ĿԴ�ĳ�Ƶһ��,����Ϊ60HZʱҪ�������κ��쳣
//@EXPECTATION:3. ���������͸��峡Ƶ��һ��ʱ����������쳣,����Ҫ������
//@REMARK:1.�豸�Ƿ�֧��ĳ�ֱַ�����ͨ��CSUDISCREENGetCapability���
//@REMARK:2.���ƽ̨֧��1080P,���������Ϊ1080Pʱ,ȷ�ϻ����Ƿ�������Ҫ����Ҳ֧��1080P
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���NTSC����,��ϸ������μ�CSTC_SCREEN_TEST_IT_SetResolution_0006����1
//@EXECUTIONFLOW:2. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P,�����ֱ��������л�,��������
//@EXECUTIONFLOW:3. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I,�����ֱ��������л�,��������
//@EXECUTIONFLOW:4. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P,�����ֱ��������л�,��������
//@EXECUTIONFLOW:5. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:6. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:7. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:8. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:9. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:10. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:11. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:12. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:13. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:14. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0007( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	if (NotSupportHD())
	{
		CSTCPrint("[UDI2SCREENTEST]  ���������ڲ��Ը���ƽ̨\r\n");
		return CSUDI_TRUE;
	}

	hPlayer = PlayHDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_720P);  //���Խ�ĿԴ�Ǹ���EM_UDISCREEN_RESOLUTION_720P

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2SCREENTEST]::��Ƶ��Ŀ���Ż����Ƿ�����?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ����ǰ�ֱ���ʧ��\n");

	CSTCPrint("********��ȷ��������SD/HD ���������***********\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����2ʧ��\n");
	CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��\n");

	//���ƽ̨֧��1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����3ʧ��\n"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����3ʧ��\n");
	}

	//���ƽ̨֧��1080p 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����4ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����4ʧ��\n");
	}

	CSTCPrint("[UDI2SCREENTEST] ���潫���豸������ʽת���Զ�������...\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����5ʧ��\n");
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����6ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����7ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����8ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����9ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	//���ƽ̨֧��1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����10ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//���ƽ̨֧��1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����11ʧ��\n"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����12ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//���ƽ̨֧��1080P 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����13ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ���
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
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

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Խ�ĿԴ�Ǹ���EM_UDISCREEN_RESOLUTION_1080I_50HZ�������,�Ա���͸����豸���б�����ʽת��,�ò���������Ҫ���ڸ���ƽ̨
//@PRECONDITION:ƽ̨֧�ָ������
//@INPUT:1. ���Ÿ���1080I_50HZ��ĿԴ
//@EXPECTATION:1. �����豸���÷ֱ���ʱ�ֱ��ʷ��������л�
//@EXPECTATION:2. ������͸�������ĳ�Ƶ�ͽ�ĿԴ�ĳ�Ƶһ��,����Ϊ60HZʱҪ�������κ��쳣
//@EXPECTATION:3. ���������͸��峡Ƶ��һ��ʱ����������쳣,����Ҫ������
//@REMARK:1.�豸�Ƿ�֧��ĳ�ֱַ�����ͨ��CSUDISCREENGetCapability���
//@REMARK:2.���ƽ̨֧��1080P,���������Ϊ1080Pʱ,ȷ�ϻ����Ƿ�������Ҫ����Ҳ֧��1080P
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��Ÿ���720P_50HZ����,��ϸ������μ�CSTC_SCREEN_TEST_IT_SetResolution_0006����1
//@EXECUTIONFLOW:2. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:3. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:4. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,�����ֱ��������л�,��������
//@EXECUTIONFLOW:5. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:6. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:7. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:8. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:9. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:10. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:11. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:12. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:13. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:14. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0008( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	if (NotSupportHD())
	{
		CSTCPrint("[UDI2SCREENTEST]  ���������ڲ��Ը���ƽ̨\r\n");
		return CSUDI_TRUE;
	}

	hPlayer = PlayHDProgram( EM_UDI_VID_STREAM_H264, EM_UDISCREEN_RESOLUTION_1080I_50HZ);  //���Խ�ĿԴ�Ǹ���EM_UDISCREEN_RESOLUTION_1080I_50HZ

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2SCREENTEST]::��Ƶ��Ŀ���Ż����Ƿ�����?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ����ǰ�ֱ���ʧ��\n");

	CSTCPrint("********��ȷ��������SD/HD ���������***********\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����2ʧ��\n");
	CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��\n");

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����3ʧ��\n"); 
	CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����3ʧ��\n");

	//���ƽ̨֧��1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����4ʧ��\n"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����4ʧ��\n");
	}

	CSTCPrint("[UDI2SCREENTEST] ���潫���豸������ʽת���Զ�������...\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����5ʧ��\n");
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����6ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����7ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����8ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	//���ƽ̨֧��1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����9ʧ��\n"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����10ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//���ƽ̨֧��1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����11ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//���ƽ̨֧��1080P 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����12ʧ��\n"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����13ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ���
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
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

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Խ�ĿԴ�Ǹ���EM_UDISCREEN_RESOLUTION_1080I�������,�Ա���͸����豸���б�����ʽת��,�ò���������Ҫ���ڸ���ƽ̨
//@PRECONDITION:ƽ̨֧�ָ������
//@INPUT:1. ���Ÿ���1080I 60HZ��ĿԴ
//@EXPECTATION:1. �����豸���÷ֱ���ʱ�ֱ��ʷ��������л�
//@EXPECTATION:2. ������͸�������ĳ�Ƶ�ͽ�ĿԴ�ĳ�Ƶһ��,����Ϊ60HZʱҪ�������κ��쳣
//@EXPECTATION:3. ���������͸��峡Ƶ��һ��ʱ����������쳣,����Ҫ������
//@REMARK:1.�豸�Ƿ�֧��ĳ�ֱַ�����ͨ��CSUDISCREENGetCapability���
//@REMARK:2.���ƽ̨֧��1080P,���������Ϊ1080Pʱ,ȷ�ϻ����Ƿ�������Ҫ����Ҳ֧��1080P
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���NTSC����,��ϸ������μ�CSTC_SCREEN_TEST_IT_SetResolution_0006����1
//@EXECUTIONFLOW:2. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P,�����ֱ��������л�,��������
//@EXECUTIONFLOW:3. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I,�����ֱ��������л�,��������
//@EXECUTIONFLOW:4. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P,�����ֱ��������л�,��������
//@EXECUTIONFLOW:5. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:6. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:7. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:8. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:9. ���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P,����������,sleep 1S
//@EXECUTIONFLOW:10. ���ƽ̨֧��1080i 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080I,����������,sleep 1S
//@EXECUTIONFLOW:11. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:12. ���ƽ̨֧��1080p 50HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_NTSC,����ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,����������,sleep 1S
//@EXECUTIONFLOW:13. ���ƽ̨֧��1080p 60HZ,���ñ�����Ƶ�����ʽΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_1080P,����������,sleep 1S
//@EXECUTIONFLOW:14. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0009( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	if (NotSupportHD())
	{
		CSTCPrint("[UDI2SCREENTEST]  ���������ڲ��Ը���ƽ̨\r\n");
		return CSUDI_TRUE;
	}

	hPlayer = PlayHDProgram( EM_UDI_VID_STREAM_H264, EM_UDISCREEN_RESOLUTION_1080I);  //���Խ�ĿԴ�Ǹ���EM_UDISCREEN_RESOLUTION_1080I

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2SCREENTEST]::��Ƶ��Ŀ���Ż����Ƿ�����?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ����ǰ�ֱ���ʧ��\n");

	CSTCPrint("********��ȷ��������SD/HD ���������***********\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����2ʧ��\n");
	CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����2ʧ��\n");

	//���ƽ̨֧��1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����3ʧ��\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����3ʧ��\n");
	} 

	//���ƽ̨֧��1080p 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����4ʧ��\n"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,�ֱ��������л�,��������,�޶���������˸\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,�ֱ��ʲ��������л����治���� \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����4ʧ��\n");
	}

	CSTCPrint("[UDI2SCREENTEST] ���潫���豸������ʽת���Զ�������...\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����5ʧ��\n");
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����6ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����7ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����8ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����9ʧ��\n"); 
	CSUDIOSThreadSleep(3000);

	//���ƽ̨֧��1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����10ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//���ƽ̨֧��1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����11ʧ��\n"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����12ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//���ƽ̨֧��1080P 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����13ʧ��\n"); 
		CSUDIOSThreadSleep(3000);
	}

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ���
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
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

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Խ�ĿԴ�Ǳ���PAL�������,�Ա����豸���б�����ʽת��,�ò���������Ҫ����֧�ֹ����г��ı���ƽ̨
//@PRECONDITION: ƽ̨֧�ֹ��ⲿ�ֻ���ȫ����Ƶ�ֱ������
//@INPUT:1. ���ű���PAL��ĿԴ
//@EXPECTATION: �����豸���÷ֱ���ʱ�ֱ��ʷ��������л�
//@REMARK:1.�豸�Ƿ�֧��ĳ�ֱַ�����ͨ��CSUDISCREENGetCapability���
//@REMARK:2.��Щ�ֱ������,��ȷ�ϻ����Ƿ�����ʱ,��Ҫ֧�ָ÷ֱ��ʵĵ���,���͵���EM_UDISCREEN_RESOLUTION_SECAM
//@REMARK:3.û�и������������ֱ������
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���NTSC����,��ϸ������μ�CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2. ��������ƽ̨֧�ֵķֱ���,���ȴ��û�ѡ��
//@EXECUTIONFLOW:3. �����û�ѡ�����ʽ
//@EXECUTIONFLOW:4. �ظ�3��4��ֱ���û�ѡ���˳�Ϊֹ
//@EXECUTIONFLOW:5. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:6. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:7. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0010( void )
{
	CSUDISCREENCapability_S sCapabilityInfo;
	CSUDISCREENType_E eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDISCREENResolution_E scResolution[2];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	char arr[10];
	int j = 0;
	int k= 0;

	CSTCPrint("[UDI2SCREENTEST]�ò���������Ҫ����֧�ֹ����г��ı���ƽ̨\n");

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //���Խ�Ŀ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2SCREENTEST]::��Ƶ��Ŀ���Ż����Ƿ�����? \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice, &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetCapability( eScreenDevice, &sCapabilityInfo), "��ȡ�豸�ֱ�����������ʧ��\n");

	do
	{
		memset(arr,0,sizeof(arr));
		for (k=0; k<EM_UDISCREEN_RESOLUTION_NUM; k++)
		{
			switch (sCapabilityInfo.m_eResolution[k])
			{
				case EM_UDISCREEN_RESOLUTION_NTSC: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_NTSC \n", k); break;
				case EM_UDISCREEN_RESOLUTION_NTSC_443: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_NTSC_443 \n", k); break;
				case EM_UDISCREEN_RESOLUTION_NTSC_JAPAN: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_NTSC_JAPAN \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_M: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_M \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_N: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_N \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_NC: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_NC \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_B: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_B \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_B1: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_B1 \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_D: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_D \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_D1: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_D1 \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_G: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_G \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_H: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_H \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_K: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_K \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_I: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_I \n", k); break;
				case EM_UDISCREEN_RESOLUTION_SECAM: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_SECAM \n", k); break;

				case EM_UDISCREEN_RESOLUTION_480P: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_480P \n", k); break;
				case EM_UDISCREEN_RESOLUTION_576P: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_576P \n", k); break;
				case EM_UDISCREEN_RESOLUTION_720P: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_720P \n", k); break;
				case EM_UDISCREEN_RESOLUTION_720P_24HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_720P_24HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_720P_50HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_720P_50HZ \n", k); break;

				case EM_UDISCREEN_RESOLUTION_1080I: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080I \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1080I_50HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080I_50HZ \n", k); break;

				case EM_UDISCREEN_RESOLUTION_1080P: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1080P_24HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P_24HZ \n",k); break;
				case EM_UDISCREEN_RESOLUTION_1080P_25HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P_25HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1080P_30HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P_30HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1080P_50HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P_50HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1250I_50HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1250I_50HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_VESA: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_VESA \n", k); break;
				default: break;

			}

			if (EM_UDISCREEN_RESOLUTION_INVALID == sCapabilityInfo.m_eResolution[k])
			{
				break;
			}
		}

		CSTCPrint("������Ļ��ʾѡ����Ӧ�ķֱ��ʣ���q �� �˳� \n");  

		if(CSTKGetString(arr, &j))
		{
			if((0<=j) && (j<k))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &sCapabilityInfo.m_eResolution[j], 1), "���÷ֱ���ʧ��\n");
				CSTCPrint("�ֱ��������Ƿ���Ԥ��һ��???\n");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���÷ֱ�����Ԥ�ڲ���");
			}
			else
			{
				CSTCPrint("û�ж�Ӧ�ķֱ��ʿ�������\r\n\n");  
			}
		}
		else
		{
			if('q' == arr[0])
			{
				CSTCPrint("�˳�\r\n");  
				break;
			}
			else
			{
				CSTCPrint("�����������������\r\n\n");  
			}
		}
	}while(1);      	

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ���
		if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
		{
			scResolution[0] = g_scResolution[0];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &g_scResolution[0], 1), "����5ʧ��\n");
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

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:���Խ�ĿԴ�Ǳ���NTSC�������,�Ա����豸���б�����ʽת��,�ò���������Ҫ����֧�ֹ����г��ı���ƽ̨
//@PRECONDITION: ƽ̨֧�ֹ��ⲿ�ֻ���ȫ����Ƶ�ֱ������
//@INPUT:1. ���ű���NTSC��ĿԴ
//@EXPECTATION: �����豸���÷ֱ���ʱ�ֱ��ʷ��������л�
//@REMARK:1.�豸�Ƿ�֧��ĳ�ֱַ�����ͨ��CSUDISCREENGetCapability���
//@REMARK:2.��Щ�ֱ������,��ȷ�ϻ����Ƿ�����ʱ,��Ҫ֧�ָ÷ֱ��ʵĵ���,���͵���EM_UDISCREEN_RESOLUTION_SECAM
//@REMARK:3.û�и������������ֱ������
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���NTSC����,��ϸ������μ�CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2. ��������ƽ̨֧�ֵķֱ���,���ȴ��û�ѡ��
//@EXECUTIONFLOW:3. �����û�ѡ�����ʽ
//@EXECUTIONFLOW:4. �ظ�3��4��ֱ���û�ѡ���˳�Ϊֹ
//@EXECUTIONFLOW:5. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:6. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:7. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0011( void )
{
	CSUDISCREENCapability_S sCapabilityInfo;
	CSUDISCREENType_E eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDISCREENResolution_E scResolution[2];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	char arr[10];
	int j = 0;
	int k = 0;

	CSTCPrint("[UDI2SCREENTEST]�ò���������Ҫ����֧�ֹ����г��ı���ƽ̨\n");

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_NTSC);  //���Խ�Ŀ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("[UDI2SCREENTEST]::��Ƶ��Ŀ���Ż����Ƿ�����?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetCapability( eScreenDevice, &sCapabilityInfo), "��ȡ�豸�ֱ�����������ʧ��\n");

	do
	{
		for (k=0; k<EM_UDISCREEN_RESOLUTION_NUM; k++)
		{
			switch (sCapabilityInfo.m_eResolution[k])
			{
				case EM_UDISCREEN_RESOLUTION_NTSC: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_NTSC \n", k); break;
				case EM_UDISCREEN_RESOLUTION_NTSC_443: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_NTSC_443 \n", k); break;
				case EM_UDISCREEN_RESOLUTION_NTSC_JAPAN: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_NTSC_JAPAN \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_M: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_M \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_N: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_N \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_NC: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_NC \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_B: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_B \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_B1: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_B1 \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_D: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_D \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_D1: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_D1 \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_G: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_G \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_H: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_H \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_K: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_K \n", k); break;
				case EM_UDISCREEN_RESOLUTION_PAL_I: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_PAL_I \n", k); break;
				case EM_UDISCREEN_RESOLUTION_SECAM: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_SECAM \n", k); break;

				case EM_UDISCREEN_RESOLUTION_480P: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_480P \n", k); break;
				case EM_UDISCREEN_RESOLUTION_576P: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_576P \n", k); break;
				case EM_UDISCREEN_RESOLUTION_720P: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_720P \n", k); break;
				case EM_UDISCREEN_RESOLUTION_720P_24HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_720P_24HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_720P_50HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_720P_50HZ \n", k); break;

				case EM_UDISCREEN_RESOLUTION_1080I: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080I \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1080I_50HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080I_50HZ \n", k); break;

				case EM_UDISCREEN_RESOLUTION_1080P: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1080P_24HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P_24HZ \n",k); break;
				case EM_UDISCREEN_RESOLUTION_1080P_25HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P_25HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1080P_30HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P_30HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1080P_50HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1080P_50HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_1250I_50HZ: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_1250I_50HZ \n", k); break;
				case EM_UDISCREEN_RESOLUTION_VESA: CSTCPrint("%d: EM_UDISCREEN_RESOLUTION_VESA \n", k); break;
				default: break;

			}

			if (EM_UDISCREEN_RESOLUTION_INVALID == sCapabilityInfo.m_eResolution[k])
			{
				break;
			}
		}

		CSTCPrint("������Ļ��ʾѡ����Ӧ�ķֱ��ʣ���q �� �˳� \n");  

		if(CSTKGetString(arr, &j))
		{
			if((0<=j) && (j<k))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &sCapabilityInfo.m_eResolution[j], 1), "���÷ֱ���ʧ��\n");
				CSTCPrint("�ֱ��������Ƿ���Ԥ��һ��???\n");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���÷ֱ�����Ԥ�ڲ���");
			}
			else
			{
				CSTCPrint("û�ж�Ӧ�ķֱ��ʿ�������\r\n\n");  
			}
		}
		else
		{
			if('q' == arr[0])
			{
				CSTCPrint("�˳�\r\n");  
				break;
			}
			else
			{
				CSTCPrint("�����������������\r\n\n");  
			}
		}
	}while(1);      	

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ���
		if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
		{
			scResolution[0] = g_scResolution[0];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &g_scResolution[0], 1), "����5ʧ��\n");
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

static void InitParam()
{
	g_nUserData = 0;
	g_eEvt = 3;
	g_eScreenDevice = 3;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@CASEGROUP:CSUDISCREENAddCallback 
//@CASEGROUP:CSUDISCREENDelCallback
//@DESCRIPTION:������Ƶ�ֱ����л�ʱ�ص���������Ч���Լ���ȷ��
//@PRECONDITION: 
//@INPUT: ��������һ������PAL��ĿԴ,ע����Ƶ�ֱ����л��ص�����
//@EXPECTATION: ��Ƶ�ֱ��ʷ��������л�,������쳣,���һص�������������
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2. ����CSUDISCREENAddCallbackע����Ƶ�ֱ����л��ص�����
//@EXECUTIONFLOW:3. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_PAL,�����ʼ��ʱ�ķֱ��ʲ�ΪEM_UDISCREEN_RESOLUTION_PAL ����(1)�ص���������(2)�ص�����������ȷ
//@EXECUTIONFLOW:(3)�¼�����ΪEM_UDISCREEN_RESOLUTION_CHANGED (4)�豸ΪEM_UDI_VOUT_DEVICE_SD
//@EXECUTIONFLOW:4. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_NTSC,����(1)�ص���������(2)�ص�����������ȷ
//@EXECUTIONFLOW:(3)�¼�����ΪEM_UDISCREEN_RESOLUTION_CHANGED (4)�豸ΪEM_UDI_VOUT_DEVICE_SD
//@EXECUTIONFLOW:5. ���ƽ̨֧�ָ���������ø�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,�����ʼ��ʱ�ķֱ��ʲ�ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ ����(1)�ص���������(2)�ص�����������ȷ
//@EXECUTIONFLOW:(3)�¼�����ΪEM_UDISCREEN_RESOLUTION_CHANGED (4)�豸ΪEM_UDI_VOUT_DEVICE_HD
//@EXECUTIONFLOW:6. ���ƽ̨֧�ָ���������ø�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����ΪEM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:����(1)�ص���������(2)�ص�����������ȷ(3)�¼�����ΪEM_UDISCREEN_RESOLUTION_CHANGED (4)�豸���и������б���
//@EXECUTIONFLOW:7. ���ƽ̨֧�ָ���������ø�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,����ΪEM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:����(1)�ص���������(2)�ص�����������ȷ(3)�¼�����ΪEM_UDISCREEN_RESOLUTION_CHANGED (4)�豸ֻ�и���û�б���
//@EXECUTIONFLOW:8. ����CSUDISCREENDelCallbackȥ���ص�����
//@EXECUTIONFLOW:9. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_NTSC�����ص�������������
//@EXECUTIONFLOW:10. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_PAL,�����ص�������������
//@EXECUTIONFLOW:11. ���ƽ̨֧�ָ���������ø�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,�����ص�������������
//@EXECUTIONFLOW:12. ���ƽ̨֧�ָ���������ø�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,
//@EXECUTIONFLOW:�������ΪEM_UDISCREEN_RESOLUTION_NTSC,�����ص�������������
//@EXECUTIONFLOW:13. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_PAL,�����ص�������������
//@EXECUTIONFLOW:14. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0012( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];
	int nUserData = 0x1234;

	InitParam();

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL); //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����1��������Ŀ����ʧ��\n");
	CSTCPrint("��Ƶ��Ŀ���Ż����Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ�������ǰ�ֱ���ʧ��\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ�������ǰ�ֱ���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ�������ǰ�ֱ���ʧ��\n");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENChangeCallback, (int*)&nUserData), "����2��ע��ص�����ʧ��\n");

	InitParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����3ʧ��\n");

	if (scResolution[0] != g_scResolution[0])
	{
		CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "����3���ص��û����ݲ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "����3���ص��¼����Ͳ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_SD == g_eScreenDevice, "����3���ص��豸���Ͳ���ȷ\n");
	}

	InitParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����4ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "����4���ص��û����ݲ���ȷ\n");
	CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "����4���ص��¼����Ͳ���ȷ\n");
	CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_SD == g_eScreenDevice, "����4���ص��豸���Ͳ���ȷ\n");

	//���ƽ̨֧�ָ������
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I_50HZ ))
	{
		InitParam();
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1], 1), "����5ʧ��\n");
		if (EM_UDISCREEN_RESOLUTION_1080I_50HZ != g_scResolution[1])
		{
			CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "����5���ص��û����ݲ���ȷ\n");
			CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "����5���ص��¼����Ͳ���ȷ\n");
			CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_HD == g_eScreenDevice, "����5���ص��豸���Ͳ���ȷ\n");
		}

		InitParam();
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����6ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "����6���ص��û����ݲ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "����6���ص��¼����Ͳ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_SD == g_eScreenDevice, "����6���ص��豸���Ͳ���ȷ\n");
	}

	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_720P_50HZ ))
	{
		InitParam();

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����7ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "����7.1���ص��û����ݲ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "����7.1���ص��¼����Ͳ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "����7.1���ص��豸���Ͳ���ȷ\n");

		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ�������ǰ�ֱ���ʧ��\n");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1], 1), "����7ʧ��\n");

        if (EM_UDISCREEN_RESOLUTION_720P_50HZ != g_scResolution[1])
		{
			CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "����7.2���ص��û����ݲ���ȷ\n");
			CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "����7.2���ص��¼����Ͳ���ȷ\n");
			CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_HD == g_eScreenDevice, "����7.2���ص��豸���Ͳ���ȷ\n");
        }
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENChangeCallback, (int*)&nUserData), "����8ʧ��\n");

	InitParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����9ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "����9���ص��û����ݲ���ȷ\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "����9���ص��¼����Ͳ���ȷ\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "����9���ص��豸���Ͳ���ȷ\n");

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����10ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "����10���ص��û����ݲ���ȷ\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "����10���ص��¼����Ͳ���ȷ\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "����10���ص��豸���Ͳ���ȷ\n");

	//���ƽ̨֧�ָ������
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I_50HZ ))
	{
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1], 1), "����11ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "����11���ص��û����ݲ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "����11���ص��¼����Ͳ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "����11���ص��豸���Ͳ���ȷ\n");
	}

	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_720P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����12ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "����12���ص��û����ݲ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "����12���ص��¼����Ͳ���ȷ\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "����12���ص��豸���Ͳ���ȷ\n");
	}

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����13ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "����13���ص��û����ݲ���ȷ\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "����13���ص��¼����Ͳ���ȷ\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "����13���ص��豸���Ͳ���ȷ\n");

	CSTK_FATAL_POINT
		
	//�ָ�����ǰ�ֱ�������
	if (NotSupportHD())
	{
		if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "����14ʧ��\n");
		}
	}
	else
	{
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
		}
	} 

	if (CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

static char  g_cCB_AUserData[16] = {0};
static char  g_cCB_BUserData[16] = {0};
static char  g_cCB_CUserData[16] = {0};

static void InitGlobalParam()
{
	memset(g_cCB_AUserData, 0, sizeof(g_cCB_AUserData));
	memset(g_cCB_BUserData, 0, sizeof(g_cCB_BUserData));
	memset(g_cCB_CUserData, 0, sizeof(g_cCB_CUserData));
}

static void CB_A(CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice, void * pvUserData)
{
	char cTemp_A[] = "CBP_A";
	char cTemp_B[] = "CBP_B";

	if (0 == strcmp(pvUserData, cTemp_A))
	{
		strcpy(g_cCB_AUserData, pvUserData);
		CSTCPrint("[UDI2SCREENTEST]g_cCB_AUserData�ص�����Ϊ%s \r\n", g_cCB_AUserData);
	}

	if (0 == strcmp(pvUserData, cTemp_B))
	{
		strcpy(g_cCB_CUserData, pvUserData);
		CSTCPrint("[UDI2SCREENTEST]g_cCB_CUserData�ص�����Ϊ%s \r\n", g_cCB_CUserData);
	}
}

static void CB_B(CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice, void * pvUserData)
{
	strcpy(g_cCB_BUserData, pvUserData);
	CSTCPrint("[UDI2SCREENTEST]g_cCB_BUserData�ص�����Ϊ%s \r\n", g_cCB_BUserData);
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@CASEGROUP:CSUDISCREENAddCallback 
//@CASEGROUP:CSUDISCREENDelCallback
//@DESCRIPTION:����ע�����ص���������ȷ��
//@PRECONDITION: 
//@INPUT:1. ��������һ������PAL��ĿԴ,ע������Ƶ�ֱ����л��ص�����
//@EXPECTATION: ��Ƶ�ֱ��ʷ��������л�,������쳣,������쳣,���һص�������������
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2. ����CSUDISCREENAddCallbackע��ص�����CB_A,�ص�����CBP_A,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3. ����CSUDISCREENAddCallbackע��ص�����CB_B,�ص�����CBP_A,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4. ����CSUDISCREENAddCallbackע��ص�����CB_A,�ص�����CBP_B,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_NTSC,��������ע��Ļص��������ܱ��ص�,�ҷ��ز����ֱ�ΪCBP_A,CBP_A,CBP_B
//@EXECUTIONFLOW:6. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_PAL,����(CB_A,CBP_A) (CB_B,CBP_A)��CB_A,CBP_B)�����ص�
//@EXECUTIONFLOW:7. ����CSUDISCREENDelCallbackɾ��(CB_A,CBP_A)�Ļص�����
//@EXECUTIONFLOW:8. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_NTSC,����(CB_B,CBP_A)��CB_A,CBP_B)���ص�(CB_A,CBP_A)���ܱ��ص�
//@EXECUTIONFLOW:9. ����CSUDISCREENDelCallbackɾ��(CB_B,CBP_A)�Ļص�����
//@EXECUTIONFLOW:10. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_PAL,����(CB_A,CBP_B)���ص�,(CB_B,CBP_A) (CB_A,CBP_A)���ܱ��ص�
//@EXECUTIONFLOW:11. ����CSUDISCREENDelCallbackɾ��(CB_A,CBP_B)�Ļص�����
//@EXECUTIONFLOW:12. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_NTSC,����(CB_A,CBP_B)(CB_B,CBP_A) (CB_A,CBP_A)�����ܱ��ص�
//@EXECUTIONFLOW:13. ���ñ�����Ƶ���ΪEM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:14. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
//@EXECUTIONFLOW:15. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:16. ����CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0013( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];
	char scCB_AUserData[] = "CBP_A";
	char scCB_BUserData[] = "CBP_B";
	
	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");
	CSUDIOSThreadSleep(500);
	
	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ�������ǰ�ֱ���ʧ��\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ�������ǰ�ֱ���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ�������ǰ�ֱ���ʧ��\n");
	}
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CB_A, (char *) scCB_AUserData), "����2ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CB_B, (char *) scCB_AUserData), "����3ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CB_A, (char *) scCB_BUserData), "����4ʧ��\n");

	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����5ʧ��\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 == strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 == strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 == strcmp(scCB_BUserData, g_cCB_CUserData)), "����5ʧ�ܣ��ص��û����ݲ���ȷ\n");
	   
	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����6ʧ��\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 == strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 == strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 == strcmp(scCB_BUserData, g_cCB_CUserData)), "����6ʧ�ܣ��ص��û����ݲ���ȷ\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CB_A, scCB_AUserData), "����7ʧ��\n");

	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����8ʧ��\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 != strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 == strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 == strcmp(scCB_BUserData, g_cCB_CUserData)), "����8���ص��û����ݲ���ȷ\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CB_B, scCB_AUserData), "����9ʧ��\n");

	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����10ʧ��\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 != strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 != strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 == strcmp(scCB_BUserData, g_cCB_CUserData)), "����10���ص��û����ݲ���ȷ\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CB_A, scCB_BUserData), "����11ʧ��\n");

	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����12ʧ��\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 != strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 != strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 != strcmp(scCB_BUserData, g_cCB_CUserData)), "����12���ص��û����ݲ���ȷ\n");
   
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "����13ʧ��\n");

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ�������
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "����14ʧ��\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
			}
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

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:�����ڱ����豸�����ø���ֱ����Լ��ڸ����豸�����ñ���ֱ���
//@PRECONDITION: 
//@INPUT:1. ��SD�豸���ø���ֱ���
//@INPUT:2. ��HD�豸���ñ���ֱ���
//@EXPECTATION: �ڱ����豸�����ø���ֱ����Լ��ڸ����豸�����ñ���ֱ���ʱ���ز�֧��
//@EXECUTIONFLOW:1.����CSUDISCREENGetResolution��ȡ����͸���ķֱ���
//@EXECUTIONFLOW:2.�ڱ����豸�����÷ֱ���ΪEM_UDISCREEN_RESOLUTION_720P,��������CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:3.����CSUDISCREENGetResolution��ȡ����ֱ���,����δ�仯
//@EXECUTIONFLOW:4.�ڱ����豸�����÷ֱ���ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,��������CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5.����CSUDISCREENGetResolution��ȡ����ֱ���,����δ�仯
//@EXECUTIONFLOW:6.�ڱ����豸�����÷ֱ���ΪEM_UDISCREEN_RESOLUTION_1080I,��������CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:7.����CSUDISCREENGetResolution��ȡ����ֱ���,����δ�仯
//@EXECUTIONFLOW:8.�ڱ����豸�����÷ֱ���ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,��������CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9.����CSUDISCREENGetResolution��ȡ����ֱ���,����δ�仯
//@EXECUTIONFLOW:10.�ڱ����豸�����÷ֱ���ΪEM_UDISCREEN_RESOLUTION_1080P,��������CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:11.����CSUDISCREENGetResolution��ȡ����ֱ���,����δ�仯
//@EXECUTIONFLOW:12.�ڱ����豸�����÷ֱ���ΪEM_UDISCREEN_RESOLUTION_1080P_50HZ,��������CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:13.����CSUDISCREENGetResolution��ȡ����ֱ���,����δ�仯
//@EXECUTIONFLOW:14.�ڸ����豸�����÷ֱ���ΪEM_UDISCREEN_RESOLUTION_NTSC,��������CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:15.����CSUDISCREENGetResolution��ȡ����ֱ���,����δ�仯
//@EXECUTIONFLOW:16.�ڸ����豸�����÷ֱ���ΪEM_UDISCREEN_RESOLUTION_PAL,��������CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:17.����CSUDISCREENGetResolution��ȡ����ֱ���,����δ�仯
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0014( void )
{
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E  scResolution[2];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 ,EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	do
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��\n");

		if (!NotSupportHD())
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ����ǰ�ֱ���ʧ��\n");
		}

		scResolution[0] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����2ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "����3ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "����3ʧ��\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����4ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "����5ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "����5ʧ��\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����6ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "����7ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "����7ʧ��\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����8ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "����9ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "����9ʧ��\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����10ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "����11ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "����11ʧ��\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����12ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "����13ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "����13ʧ��\n");
	} while(0); 

	if (!NotSupportHD())
	{
		scResolution[1]  = EM_UDISCREEN_RESOLUTION_NTSC;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1] , 1), "����14ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &scResolution[1] ), "����15ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[1] == scResolution[1], "����15ʧ��\n");

		scResolution[1]  = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1] , 1), "����16ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &scResolution[1] ), "����17ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[1] == scResolution[1], "����17ʧ��\n");
	}

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ���
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "����14ʧ��\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
			}
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

//@CASEGROUP:CSUDISCREENGetResolution 
//@DESCRIPTION:���Ի�ȡָ��ΪNULL�����
//@PRECONDITION:
//@INPUT:1. eScreenDevice = EM_UDI_VOUT_DEVICE_SD,peResolution = NULL
//@INPUT:2. eScreenDevice = EM_UDI_VOUT_DEVICE_HD+!,peResolution = &sResolution
//@INPUT:2. eScreenDevice = EM_UDI_VOUT_DEVICE_SD-1,peResolution = &sResolution
//@EXPECTATION: ����CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����CSUDISCREENGetResolution,����������,��������CSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_GetResolution_0001( void )
{
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	CSUDISCREENResolution_E sResolution;

	nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "����������ʧ��1");

	nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD -1, &sResolution);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "����������ʧ��2");

	nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD +1, &sResolution);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "����������ʧ��3");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENGetResolution 
//@DESCRIPTION:���Էֱ���ת�����Ƿ��ܹ���ȡ����ȷ�ĵ�ǰ�ֱ���
//@PRECONDITION:
//@INPUT:1. �ֱ�Էֱ��ʽ��и����л�
//@INPUT:2. CSUDISCREENGetResolution���þ�����Ϸ�����
//@EXPECTATION: �ֱ��ʲ����仯��ͨ��CSUDISCREENGetResolution�ܹ���ȡ����ǰ��ȷ�ķֱ���
//@EXECUTIONFLOW:1.���ñ���ֱ���ΪEM_UDISCREEN_RESOLUTION_PAL,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2.����CSUDISCREENGetResolution��ȡ����ֱ���,����ΪEM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:3.���ñ���ֱ���ΪEM_UDISCREEN_RESOLUTION_NTSC,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDISCREENGetResolution��ȡ����ֱ���,����ΪEM_UDISCREEN_RESOLUTION_NTSC
//@EXECUTIONFLOW:5.���ƽ̨֧�ָ���ֱ���,�����ø����豸�ֱ���ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6.���ƽ̨֧�ָ���ֱ���,����CSUDISCREENGetResolution��ȡ����ֱ���,����ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ
//@EXECUTIONFLOW:7.���ƽ̨֧�ָ���ֱ���,��ͬʱ���ñ���ΪEM_UDISCREEN_RESOLUTION_PAL,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:8.���ƽ̨֧�ָ���ֱ���,����CSUDISCREENGetResolution��ȡ����ֱ���,����ΪEM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:9.���ƽ̨֧�ָ���ֱ���,����CSUDISCREENGetResolution��ȡ����ֱ���,����ΪEM_UDISCREEN_RESOLUTION_720P_50HZ
//@EXECUTIONFLOW:10. �ָ�����ǰ�ֱ�������,�÷ֱ�Ϊ��ģ����Գ�ʼ��ʱ��õĸ߱���ֱ���
CSUDI_BOOL CSTC_SCREEN_TEST_IT_GetResolution_0002( void )
{
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL );  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");
	CSTCPrint("��Ƶ��Ŀ���Ż����Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����1ʧ��\n");

	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ�������ǰ�ֱ���ʧ��\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "��ȡ�������ǰ�ֱ���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "��ȡ�������ǰ�ֱ���ʧ��\n");
	}

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����1ʧ��\n"); 
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0]), "����2,��ȡ�ֱ���\n");  
	CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_PAL == scResolution[0], "����2ʧ��,��ȡ�ķֱ��������õĲ�һ��\n");

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "����3ʧ��\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0]), "����4,��ȡ�ֱ���\n"); 
	CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_NTSC == scResolution[0], "����4ʧ��,��ȡ�ķֱ��������õĲ�һ��\n");

	//���ƽ̨֧�ָ���ֱ���
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I_50HZ ))
	{    
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1], 2), "����5ʧ��\n");
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &scResolution[1] ), "����6ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_1080I_50HZ == scResolution[1], "����6ʧ��,��ȡ�ķֱ��������õĲ�һ��\n")
	} 

	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_720P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����7ʧ��\n"); 
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD, &scResolution[0] ), "����8����ȡ�ֱ���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_PAL == scResolution[0], "����8ʧ��,��ȡ�ķֱ��������õĲ�һ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &scResolution[1] ), "����9����ȡ�ֱ���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_720P_50HZ == scResolution[1], "����9ʧ��,��ȡ�ķֱ��������õĲ�һ��\n");
	}

	CSTK_FATAL_POINT
	{
		//�ָ�����ǰ�ֱ�������
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "����14ʧ��\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "����14ʧ��\n");
			}
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

//@CASEGROUP:CSUDISCREENGetCapability 
//@DESCRIPTION:���Ի�ȡָ��ΪNULL�����
//@PRECONDITION:
//@INPUT:1. eScreenDevice = EM_UDI_VOUT_DEVICE_HD,psCapabilityInfo = NULL
//@INPUT:2. eScreenDevice = EM_UDI_VOUT_DEVICE_HD+1,psCapabilityInfo = &sCapability
//@INPUT:3. eScreenDevice = EM_UDI_VOUT_DEVICE_SD-1,psCapabilityInfo = &sCapability
//@EXPECTATION: ����CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����CSUDISCREENGetCapability,��������������������CSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_GetCapability_0001( void )
{
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	CSUDISCREENCapability_S sCapability;

	nResult = CSUDISCREENGetCapability(EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "����������ʧ��1");

	nResult = CSUDISCREENGetCapability(EM_UDI_VOUT_DEVICE_HD+1, &sCapability);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "����������ʧ��2");

	nResult = CSUDISCREENGetCapability(EM_UDI_VOUT_DEVICE_SD-1, &sCapability);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "����������ʧ��3");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENGetCapability 
//@DESCRIPTION:������Ӧ��ͨ��Ӧ�û�ȡ��Ӧ�ķֱ���
//@INPUT:1. eScreenDevice = EM_UDI_VOUT_DEVICE_SD,psCapabilityInfo = �Ϸ�ָ��
//@INPUT:2. eScreenDevice = EM_UDI_VOUT_DEVICE_HD,psCapabilityInfo = �Ϸ�ָ��(��֧��HD�������)
//@EXPECTATION: 1.����CSUDI_SUCCESS
//@EXPECTATION: 2.�����豸��Ӧ���и���ֱ���֧�֣������豸��Ӧ�÷��ر���ֱ���
//@EXPECTATION: 3.��������֧��EM_UDISCREEN_RESOLUTION_NTSC��EM_UDISCREEN_RESOLUTION_PAL
//@EXPECTATION: 4.��������֧��EM_UDISCREEN_RESOLUTION_720P_50HZ,EM_UDISCREEN_RESOLUTION_720P,EM_UDISCREEN_RESOLUTION_1080I_50HZ,EM_UDISCREEN_RESOLUTION_1080I
//@EXECUTIONFLOW:1.����CSUDISCREENGetCapability��ȡ�����豸֧�ֵķֱ���
//@EXECUTIONFLOW:2.���������豸֧�ֵķֱ���,����֧�ֵķֱ����в�Ӧ���и���ķֱ���
//@EXECUTIONFLOW:3.����������֧��EM_UDISCREEN_RESOLUTION_NTSC��EM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:4.���������豸֧�ֵķֱ���,����֧�ֵķֱ����в�Ӧ���б���ķֱ���(��ƽ̨֧��HD��������)
//@EXECUTIONFLOW:5.����������֧��EM_UDISCREEN_RESOLUTION_720P_50HZ,EM_UDISCREEN_RESOLUTION_720P,EM_UDISCREEN_RESOLUTION_1080I_50HZ,EM_UDISCREEN_RESOLUTION_1080I
CSUDI_BOOL CSTC_SCREEN_TEST_IT_GetCapability_0002( void )
{
	CSUDISCREENResolution_E  eResolution = EM_UDISCREEN_RESOLUTION_720P;
	CSUDISCREENType_E eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	for (eResolution = EM_UDISCREEN_RESOLUTION_720P; eResolution<EM_UDISCREEN_RESOLUTION_NUM; eResolution++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == IsScreenSupport(&eScreenDevice, eResolution ), "Step2ʧ��");
	} 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IsScreenSupport(&eScreenDevice, EM_UDISCREEN_RESOLUTION_PAL), "Step3 failure:SD must support PAL\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IsScreenSupport(&eScreenDevice, EM_UDISCREEN_RESOLUTION_NTSC), "Step3failure:SD must support NTSC\n");

	if(!NotSupportHD())
	{
		eScreenDevice = EM_UDI_VOUT_DEVICE_HD;

		for (eResolution = EM_UDISCREEN_RESOLUTION_NTSC; eResolution<EM_UDISCREEN_RESOLUTION_480P; eResolution++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == IsScreenSupport(&eScreenDevice, eResolution ), "Step3ʧ��");
		} 

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IsScreenSupport(&eScreenDevice, EM_UDISCREEN_RESOLUTION_720P), "Step3failure:HD must support 720P\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IsScreenSupport(&eScreenDevice, EM_UDISCREEN_RESOLUTION_720P_50HZ), "Step3failure:HD must support 720P_50HZ\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IsScreenSupport(&eScreenDevice, EM_UDISCREEN_RESOLUTION_1080I), "Step3failure:HD must support 1080I\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IsScreenSupport(&eScreenDevice, EM_UDISCREEN_RESOLUTION_1080I_50HZ), "Step3failure:HD must support 1080I_50HZ\n");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetBrightness 
//@DESCRIPTION:�������Ȳ����ֱ�Ϊ-1��101�Ƿ�ֵ�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. nBrightness = -1
//@INPUT:2. nBrightness = 101
//@EXPECTATION:1.���ش�����CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2.����CSUDISCREENSetBrightness��������Ϊ-1,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3.����CSUDISCREENSetBrightness��������Ϊ101,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4.����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:5.����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetBrightness_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetBrightness(-1), "�������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetBrightness(101), "�������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDISCREENSetContrast 
//@DESCRIPTION:�������ȴ�0~100��Χÿ��20 �����仯�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. nBrightness = [0-100]
//@EXPECTATION:1.ÿ�����÷���CSUDI_SUCCESS,����Ļ�仯������ֵ���
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2.����CSUDISCREENSetContrast��������Ϊ0
//@EXECUTIONFLOW:3.����ֵ����20������CSUDISCREENSetBrightness����ֱ���ﵽ���ֵ100
//@EXECUTIONFLOW:4.������ɻָ������ֳ�,����������Ϊ50
//@EXECUTIONFLOW:5.����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:6.����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetBrightness_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int i = 0;
	
	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("����ÿ2������һ����Ļ���ȣ�ʹ���������仯����ע��۲���Ļ���ȣ����������ʼ����\r\n");
	CSTKWaitAnyKey();

	for(i=0; i<=100; i+=20)
	{
		CSTCPrint("������Ļ����Ϊ%d\n", i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetBrightness(i), "��������ʧ��\n");
		CSUDIOSThreadSleep(2*1000);
	}

	CSTCPrint("��Ļ���ȷ��������仯?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "������Ļ���Ⱥ�Ч������ȷ");

	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetBrightness(50), "��������ʧ��\n");

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetContrast 
//@DESCRIPTION:���ԶԱȶȲ����ֱ�Ϊ-1��101�Ƿ�ֵ�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. Contrast = -1
//@INPUT:2. Contrast = 101
//@EXPECTATION:1.���ش�����CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2.����CSUDISCREENSetContrast���öԱȶ�Ϊ-1,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3.����CSUDISCREENSetContrast���öԱȶ�Ϊ101,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4.����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:5.����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetContrast_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetContrast(-1), "�������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetContrast(101), "�������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetBrightness 
//@DESCRIPTION:���ԶԱȶȴ�0~100��Χÿ��20 �����仯�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. Contrast = [0-100]
//@EXPECTATION:1.ÿ�����÷���CSUDI_SUCCESS,����Ļ�仯������ֵ���
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2.����CSUDISCREENSetBrightness���öԱȶ�Ϊ0
//@EXECUTIONFLOW:3.�Աȶ�ֵ����20������CSUDISCREENSetContrast����ֱ���ﵽ���ֵ100
//@EXECUTIONFLOW:4.������ɻָ������ֳ�,���Աȶ�����Ϊ50
//@EXECUTIONFLOW:5.����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:6. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetContrast_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int i = 0;
	
	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("����ÿ2������һ����Ļ�Աȶȣ�ʹ���������仯����ע��۲���Ļ�Աȶȣ����������ʼ����\r\n");
	CSTKWaitAnyKey();
		
	for(i=0; i<=100; i+=20)
	{
		CSTCPrint("������Ļ�Աȶ�Ϊ%d\n", i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetContrast(i), "���öԱȶ�ʧ��\n");
		CSUDIOSThreadSleep(2*1000);
	}

	CSTCPrint("��Ļ�Աȶȷ��������仯?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "������Ļ�ԱȶȺ�Ч������ȷ");

	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetContrast(50), "���öԱȶ�ʧ��\n");
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetSaturation 
//@DESCRIPTION:���Ա��ͶȲ����ֱ�Ϊ-1��101�Ƿ�ֵ�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. Saturation = -1
//@INPUT:2. Saturation = 101
//@EXPECTATION:1.���ش�����CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2.����CSUDISCREENSetSaturation���ñ��Ͷ�Ϊ-1,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3.����CSUDISCREENSetSaturation���ñ��Ͷ�Ϊ101,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4.����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:5. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetSaturation_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetSaturation(-1), "�������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetSaturation(101), "�������ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetSaturation 
//@DESCRIPTION:���Ա��Ͷȴ�0~100��Χÿ��20 �����仯�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. Saturation = [0-100]
//@EXPECTATION:1.ÿ�����÷���CSUDI_SUCCESS,����Ļ�仯������ֵ���
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2.����CSUDISCREENSetSaturation���ñ��Ͷ�Ϊ0
//@EXECUTIONFLOW:3.���Ͷ�ֵ����20������CSUDISCREENSetSaturation����ֱ���ﵽ���ֵ100
//@EXECUTIONFLOW:4.������ɻָ������ֳ�,�����Ͷ�����Ϊ50
//@EXECUTIONFLOW:5.����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:6. ����CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetSaturation_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int i = 0;
	
	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��\n");

	CSTCPrint("����ÿ2������һ����Ļ���Ͷȣ�ʹ���������仯����ע��۲���Ļ���Ͷȣ����������ʼ����\r\n");
	CSTKWaitAnyKey();

	for(i=0; i<=100; i+=20)
	{
		CSTCPrint("������Ļ���Ͷ�Ϊ%d\n", i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetSaturation(i), "���ñ��Ͷ�ʧ��\n");
		CSUDIOSThreadSleep(2*1000);
	}

	CSTCPrint("��Ļ���Ͷȷ��������仯?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "������Ļ���ͶȺ�Ч������ȷ");

	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetSaturation(50), "���ñ��Ͷ�ʧ��\n");
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}


//@CASEGROUP:SetOSDTransparency 
//@DESCRIPTION:����͸���Ȳ����ֱ�Ϊ-1��101�Ƿ�ֵ������Լ�0��100�ı߽����
//@INPUT:1. Saturation = -1,Saturation = 101
//@INPUT:2. Saturation = 0,Saturation = 100
//@EXPECTATION:1.Saturation = -1,Saturation = 101,���ش�����CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXPECTATION:1.Saturation = 0,Saturation = 100,���ش�����CSUDI_SUCCESS
//@EXECUTIONFLOW:1.����CSUDISCREENSetSaturation����͸����Ϊ-1,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2.����CSUDISCREENSetSaturation����͸����Ϊ101,��������CSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3.����CSUDISCREENSetSaturation����͸����Ϊ0,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDISCREENSetSaturation����͸����Ϊ100,��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetOSDTransparency_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetOSDTransparency(-1), "step 1 ����������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER ==CSUDISCREENSetOSDTransparency(101), "step 2 ����������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(0), "step 3 �߽��������ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDISCREENSetOSDTransparency(100), "step 4 �߽��������ʧ��\n");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:SetOSDTransparency 
//@DESCRIPTION:������û��video�������������OSG͸����Ҳ����Ч��
//@PRECONDITION:ƽ̨OSGģ�鹦������
//@INPUT:1. Saturation = [0-100]
//@EXPECTATION:1.ÿ�����÷���CSUDI_SUCCESS
//@EXPECTATION:2. ��ĻOSG�仯��͸�����������
//@EXECUTIONFLOW:1.�����²����ڸ߱����Դ��ϻ�����ͼ��(���ƽ̨֧�ָ���Ű�������):
//@EXECUTIONFLOW:1.1 ����CSUDIOSGGetDisplaySurface��ȡ�Դ���,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:1.2 ����CSUDIOSGFill��{260,188}λ�����{200,200}�ĺ�ɫOSG,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2.����CSUDIOSGSyncͬ������,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3.����OSG͸����,��[0,100],ÿ������20��ÿ�����ú�sleep 1.5s
//@EXECUTIONFLOW:4.�ȴ��û��ж����Խ��
//@EXECUTIONFLOW:5.����CSUDISCREENSetOSDTransparency�ָ�OSG��͸����Ϊ��͸��,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6.����CSUDIOSGFill�ָ��߱����OSG �Դ�Ϊֵ0x0,��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetOSDTransparency_0002( void )
{

	CSUDI_HANDLE hDisplaySurfaceSD = CSUDI_NULL,hDisplaySurfaceHD = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfoSD,sSurfaceInfoHD;
	CSUDIOSGRect_S stRect  = {
								260,
								188,
								200,
								200,
								};
	int i=0;
	BOOL bSupportHD=!NotSupportHD();

	if (!IsShareHDDisplay())
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0,&hDisplaySurfaceSD)),
			"get display surface failure");

		memset(&sSurfaceInfoSD, 0, sizeof(sSurfaceInfoSD));
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplaySurfaceSD, &sSurfaceInfoSD)), "��ȡ����Ϣʧ��");
	}
	
	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hDisplaySurfaceHD)),
			"get display surface failure");

		memset(&sSurfaceInfoHD, 0, sizeof(sSurfaceInfoHD));
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplaySurfaceHD, &sSurfaceInfoHD)), "��ȡ����Ϣʧ��");
	}

	

	if (!IsShareHDDisplay())
	{
		stRect.m_nX = (sSurfaceInfoSD.m_nWidth - stRect.m_nWidth)/2;
		stRect.m_nY = (sSurfaceInfoSD.m_nHeight - stRect.m_nHeight)/2;
	
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGFill(hDisplaySurfaceSD, CSUDI_NULL, 0)),
			"Fill Failure !");
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGFill(hDisplaySurfaceSD, &stRect, 0x80ff0000)),
			"Fill Failure !");
	}

	if (bSupportHD)
	{
		stRect.m_nX = (sSurfaceInfoHD.m_nWidth - stRect.m_nWidth)/2;
		stRect.m_nY = (sSurfaceInfoHD.m_nHeight - stRect.m_nHeight)/2;

		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGFill(hDisplaySurfaceHD, CSUDI_NULL, 0)),
			"Fill Failure !");
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGFill(hDisplaySurfaceHD, &stRect, 0x80ff0000)),
			"Fill Failure !");
	}
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	CSTCPrint("��Ļ��(%d,%d)λ�ÿ��Կ�����ɫ���Ϊ(%d,%d)�ľ��ο�,��������(���֧�ָ���Ļ�)\n",
	stRect.m_nX,stRect.m_nY,stRect.m_nWidth,stRect.m_nHeight);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTCPrint("��ע��OSG�ı仯,OSG��͸���Ƚ���0�仯��100��ÿ��20,���������\n");
	CSTKWaitAnyKey();

	for(i=0;i<=100;i+=20)
	{
		CSTCPrint("����OSG͸���ȵ�:%d\n",i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(i), "����������ʧ��\n");
		CSUDIOSThreadSleep(1500);
	}

	CSTCPrint("��ĻOSG����ȫ͸���𽥱����ȫ����?? (Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTK_FATAL_POINT;

	if (!IsShareHDDisplay())
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGFill(hDisplaySurfaceSD, CSUDI_NULL, 0)),
			"Fill Failure !");
	}
	
	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGFill(hDisplaySurfaceHD, CSUDI_NULL, 0)),
			"Fill Failure !");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:SetOSDTransparency 
//@DESCRIPTION:�����ڲ���video�������,OSG͸���ȴ�0~100��Χÿ��20 �����仯�����
//@PRECONDITION:��Ŀ��������
//@INPUT:1. Saturation = [0-100]
//@EXPECTATION:1.ÿ�����÷���CSUDI_SUCCESS
//@EXPECTATION:2. ��Ļ�仯������ֵ���,����video����OSG͸�������ñ仯;OSG͸���ȱ仯����
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2.����CSUDIOSGGetDisplaySurface��ȡ�����Դ���
//@EXECUTIONFLOW:3.����CSUDIOSGFill�������Դ�������ɫ0xFF0000FF,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDIOSGFill�������Դ�����{200,200}�����{250,250}����͸��ɫ0x0,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIVIDEOSetWindowSize���ñ���video���ڴ�СΪOSG͸�������С,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6.���ƽ̨֧�ָ������ظ�����2~5,�ڸ���������ͬ�Ļ�ͼ�����ã�ÿ��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7.����OSG͸����,��[0,100],ÿ������20��ÿ�����ú�sleep 1.5s
//@EXECUTIONFLOW:8.�ȴ��û��ж����Խ��
//@EXECUTIONFLOW:9.����CSUDIPLAYERStopֹͣ���Խ�Ŀ����,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:10. ����CSUDIPLAYERClose�رղ�����,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11.����CSUDIVIDEOSetWindowSize�ָ��߱������Ƶ����Ϊȫ��,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12.����CSUDISCREENSetOSDTransparency�ָ�OSG��͸����Ϊ��͸��,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12.����CSUDIOSGFill�ָ��߱����OSG �Դ�Ϊֵ0x0,��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetOSDTransparency_0003( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hSurfaceSD = CSUDI_NULL,hSurfaceHD = CSUDI_NULL;
	int i = 0;
	CSUDIOSGRect_S stRect={200,200,250,250};
	CSUDIWinRect_S  stVideoRect;
	CSUDIOSGSurfaceInfo_S stInfo;
	BOOL bSupportHD=!NotSupportHD();
	CSUDIWinRect_S sWinFullRect;

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��");
	
	memcpy(&stVideoRect,&stRect,sizeof(stVideoRect));

	if (!IsShareHDDisplay())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0,&hSurfaceSD),
			"fail to get SD display surface");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceSD,NULL,0xFF0000FF),
			"fail to fill whole SD surface to blue");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceSD,&stRect,0x0),
				"fail to fill SD region {200,200,250,250} to transparence");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,&stVideoRect),
			"fail to set SD video to little box");
	}

	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hSurfaceHD),
		"fail to get SD display surface");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceHD,NULL,0xFF0000FF),
			"fail to fill whole HD surface to blue");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurfaceHD, &stInfo),
			"fail to get hd surface by CSUDIOSGGetSurfaceInfo");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceHD,&stRect,0x0),
				"fail to fill HD region {200,200,250,250} to transparence");

		memset(&sWinFullRect, 0, sizeof(sWinFullRect));
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(0, EM_UDI_VOUT_DEVICE_HD, &sWinFullRect),
			"fail to get HD full windowrect");
	
		stVideoRect.m_nX = (stRect.m_nX*sWinFullRect.m_nWidth)/stInfo.m_nWidth;
		stVideoRect.m_nWidth = (stRect.m_nWidth*sWinFullRect.m_nWidth)/stInfo.m_nWidth;
		stVideoRect.m_nY = (stRect.m_nY*sWinFullRect.m_nHeight)/stInfo.m_nHeight;
		stVideoRect.m_nHeight = (stRect.m_nHeight*sWinFullRect.m_nHeight)/stInfo.m_nHeight;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,&stVideoRect),
			"fail to set HD video to little box");
	}

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint("��Ļ{200,200,250,250}λ��ΪС������Ƶ,����Ϊ��ɫOSG(ע��: ��ƵԴ�����������кڱ�)�?(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");
	
	CSTCPrint("��ע��۲���ĻOSG͸���ȱ仯\n");
	CSTKWaitAnyKey();

	for(i = 0; i <= 100; i += 20)
	{
		CSTCPrint("����OSG͸���ȵ�:%d\n",i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(i), "����͸����ʧ��");
		CSUDIOSThreadSleep(1500);
	}

	CSTCPrint("��ĻOSG����ȫ͸���𽥱����ȫ����,��video����Ӱ��?? (Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��");
	
	CSTK_FATAL_POINT
	 
	if (CSUDI_NULL != hPlayer)
	{
		 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��");
		 hPlayer = CSUDI_NULL;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,NULL),
		"fail to set SD video to full screen");

	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,NULL),
			"fail to set HD video to full screen");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(100), "����͸����ʧ��\n");

	if (hSurfaceSD)
	{
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceSD, NULL, 0x0)),
					"fill OSG to 0,recover test site failure"
				);
	}

	if (hSurfaceHD)
	{
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceHD, NULL, 0x0)),
					"fill OSG to 0,recover test site failure"
				);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:SetOSDTransparency 
//@DESCRIPTION:���Էֱ��ʵ��л��������OSG͸���ȵĶ�ʧ
//@PRECONDITION:��Ŀ��������
//@INPUT:1. Saturation = [0-100]
//@EXPECTATION:1.ÿ�����÷���CSUDI_SUCCESS,����Ļ�仯������ֵ���
//@EXECUTIONFLOW:1.����playerģ��ӿڲ��ű���PAL����,��ϸ������ο���������CSTC_SCREEN_TEST_IT_SetResolution_0003����1
//@EXECUTIONFLOW:2.�����²����ڸ߱����Դ��ϻ�����ͼ��(���ƽ̨֧�ָ���Ű�������):
//@EXECUTIONFLOW:2.1 ����CSUDIOSGGetDisplaySurface��ȡ�Դ���,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2.2 ����CSUDIOSGFill��{50,50}λ�����{350,350}�ĺ�ɫOSG,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2.3 ����CSUDIVIDEOSetWindowSize������Ƶ���С����ģʽ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3.����CSUDISCREENSetOSDTransparency����OSG��͸����Ϊ40,����OSGΪ��͸��Ч��.
//@EXECUTIONFLOW:4.����CSUDISCREENSetResolution���ñ���ֱ���ΪNTSC,����Ϊ720P_50HZ(���֧�ָ���Ļ�)
//@EXECUTIONFLOW:5.����CSUDISCREENSetResolution���ñ���ֱ���ΪPAL,����Ϊ1080I_50HZ(���֧�ָ���Ļ�)
//@EXECUTIONFLOW:6.�ȴ�ȷ�Ϸֱ��ʵ������Ƿ�����OSG͸�������õĶ�ʧ
//@EXECUTIONFLOW:7.����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:8. ����CSUDIPLAYERClose�رղ�����
//@EXECUTIONFLOW:9. ����CSUDIVIDEOSetWindowSize���ø߱�����Ƶ����Ϊȫ��
//@EXECUTIONFLOW:10.����CSUDISCREENSetOSDTransparency����OSG͸����Ϊ100,�ָ������ֳ�
//@EXECUTIONFLOW:11.����CSUDIOSGFill���߱����Դ���Ϊ0x0
CSUDI_BOOL CSTC_SCREEN_TEST_IT_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hSurfaceSD = CSUDI_NULL,hSurfaceHD = CSUDI_NULL;
	int i = 0;
	CSUDIOSGRect_S stRect={50,50,350,350};
	CSUDIWinRect_S stRectVideo;
	BOOL bSupportHD=!NotSupportHD();

#define SCREEN_COUNT	(2)
	
	CSUDISCREENType_E eScreenDevice[SCREEN_COUNT];
	CSUDISCREENResolution_E eResolutionSD[SCREEN_COUNT] = 
	{
		EM_UDISCREEN_RESOLUTION_NTSC,
		EM_UDISCREEN_RESOLUTION_PAL
	};
	CSUDISCREENResolution_E eResolutionHD[2]=
	{
		EM_UDISCREEN_RESOLUTION_720P_50HZ,
		EM_UDISCREEN_RESOLUTION_1080I_50HZ
	};

	eScreenDevice[0] = EM_UDI_VOUT_DEVICE_SD;
	eScreenDevice[1] = EM_UDI_VOUT_DEVICE_HD;

	memcpy(&stRectVideo,&stRect,sizeof(stRect));
	stRectVideo.m_nX += 50;
	stRectVideo.m_nY += 50;
	
	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //����PLAYER���Ų�������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "������Ŀ����ʧ��");

	if (!IsShareHDDisplay())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0,&hSurfaceSD),
			"fail to get SD display surface");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceSD,&stRect,0xFFFF0000),
				"fail to fill SD region {50,50,350,350} to red");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,&stRectVideo),
			"fail to set SD video to little box");
	}

	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hSurfaceHD),
		"fail to get SD display surface");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceHD,&stRect,0xFFFF0000),
				"fail to fill HD region {50,50,350,350} to red");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,&stRectVideo),
			"fail to set HD video to little box");
	}

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(40), "����͸����ʧ��");
	
	CSTCPrint("1. ��Ļ��ʾΪ��ɫOSG���Լ���ƵС����,�������в����ص�(Y/N)\n");
	CSTCPrint("2. OSGΪ��͸��Ч��(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTCPrint("����������зֱ��ʵ�����,������ֱ��ʵ��л��Ƿ�����OSG͸���ȵĶ�ʧ\n");
	CSTKWaitAnyKey();

	g_scResolution[0] = EM_UDISCREEN_RESOLUTION_INVALID;
	g_scResolution[1] = EM_UDISCREEN_RESOLUTION_INVALID;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice[0], &g_scResolution[0]), "��ȡ����ǰ�ֱ���ʧ��");

	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice[1], &g_scResolution[1]), "��ȡ����ǰ�ֱ���ʧ��");
	}

	for(i=0;i<SCREEN_COUNT;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[0],&eResolutionSD[i],1),
				"fail to set SD screen resoluction");
		if (bSupportHD)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[1],&eResolutionHD[i],1),
				"fail to set HD screen resoluction");
		}
		CSUDIOSThreadSleep(3000);
	}

	CSTCPrint("�ֱ����л��������OSG͸���ȶ�ʧ?(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTK_FATAL_POINT
	 
	if (CSUDI_NULL != hPlayer)
	{
		 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��");
		 hPlayer = CSUDI_NULL;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,NULL),
		"fail to set SD video to full screen");

	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,NULL),
		"fail to set HD video to full screen");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(100), "����͸����ʧ��");

	if (hSurfaceSD)
	{
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceSD, NULL, 0x0)),
					"fill OSG to 0,recover test site failure"
				);
	}

	if (hSurfaceHD)
	{
		CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceHD, NULL, 0x0)),
					"fill OSG to 0,recover test site failure"
				);
	}

	if (g_scResolution[0] > EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0] <= EM_UDISCREEN_RESOLUTION_576P)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[0], &g_scResolution[0], 1),
				"��ԭ����ǰ�ֱ���ʧ��");
	}

	if (bSupportHD
		&& (g_scResolution[1] >= EM_UDISCREEN_RESOLUTION_720P && g_scResolution[1] < EM_UDISCREEN_RESOLUTION_NUM))
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[1], &g_scResolution[1], 1),
				"��ԭ����ǰ�ֱ���ʧ��");
	}

	return CSUDI_TRUE;
}

