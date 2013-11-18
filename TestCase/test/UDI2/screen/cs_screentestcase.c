/* --------------------------------------------------------------------
◊¢“‚£∫
1.‘⁄–Ë“™”Î”√ªßΩªª•µƒ≤‚ ‘”√¿˝÷–£¨ø…“‘£∫
	a.  π”√CSTKWaitAnyKeyµ»¥˝”√ªß ‰»Î»Œ“‚∞¥º¸
	b.  π”√CSTKWaitYesµ»¥˝”√ªß ‰»ÎYES
2.≤‚ ‘”√¿˝∫Ø ˝√¸√˚£∫≤‚ ‘”√¿˝ID£¨"≤‚ ‘”√¿˝ID"∂®“Â‘⁄≤‚ ‘”√¿˝Œƒµµ÷–
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
	//‘⁄±æ≤‚ ‘”√¿˝ºØ÷¥––«∞µ˜”√
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_SCREEN_UnInit(void)
{
	//‘⁄±æ≤‚ ‘”√¿˝ºØ÷¥––∫Ûµ˜”√
	return CSUDI_TRUE;
}

//Õ®π˝∂¡»°≈‰÷√Œƒº˛≤È—Ø∆ΩÃ® «∑Ò÷ß≥÷∏ﬂ«Â ”∆µ ‰≥ˆÕ®µ¿¿‡–Õ
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
			bRet = CSUDI_TRUE;	// ±Ì æ≤ª÷ß≥÷∏ﬂ«Â∆ΩÃ®
		}
	}
	else
	{
		CSTCPrint("ªÒ»°EM_UDI_VOUT_DEVICE_HD≈‰÷√œÓ–≈œ¢ ß∞‹!\n");
	}
	
	return bRet;
}

//Õ®π˝≈‰÷√Œƒº˛≤È—Ø∆ΩÃ® «∑Ò∏ﬂ±Í«ÂÕ¨‘¥
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
		CSTCPrint("ªÒ»°CS_OSGUDI2_SHARE_HD_DISPLAY_SERFACE≈‰÷√œÓ–≈œ¢ ß∞‹!\n");
	}

	return bRet;
}

//—∞’“ ”∆µΩ‚¬Î∆˜
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


//—∞’“∆ΩÃ®÷ß≥÷µƒ∑÷±Ê¬ 
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


// ≤•∑≈±Í«Â ”∆µΩ⁄ƒø‘¥
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
		CSTCPrint("ªÒ»°Video , demux¥ÌŒÛ\n");
		
		return CSUDI_NULL;
	}

	sPlayerChnl.m_nDemux = nDemuxIndex;
	sPlayerChnl.m_nVideoDecoder = nVideoIndex;
	sPlayerChnl.m_nAudioDecoder = 0;

	//À¯≤‚ ‘¬Î¡˜∆µµ„
	if (CSTC_UDI2PortingLock(0, "Audio&Video Test_27Mbps_20070524.ts"))
	{
		CSUDIVIDEOSetStopMode(nVideoIndex, EM_UDIVIDEO_STOPMODE_BLACK);

		if (CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer))
		{
			if (hPlayer != CSUDI_NULL)
			{
				if (EM_UDISCREEN_RESOLUTION_PAL == eResolution)
				{
					sStreamInfo.m_nPid = 1140;  //PALΩ⁄ƒø‘¥
					sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
					sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
				}

				if (EM_UDISCREEN_RESOLUTION_NTSC == eResolution)
				{
					sStreamInfo.m_nPid = 5137;  //NTSCΩ⁄ƒø‘¥
					sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
					sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
				}

				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, &sStreamInfo, 1, CSUDI_NULL)))
				{
					CSTCPrint("Player set stream  ß∞‹\n");
					
					if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
					{
						CSTCPrint("πÿ±’≤•∑≈∆˜ ß∞‹\n");
					}
					
					return CSUDI_NULL;
				}

				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)))
				{
					CSTCPrint("Player start  ß∞‹\n");
					
					if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
					{
						CSTCPrint("πÿ±’≤•∑≈∆˜ ß∞‹\n");
					}
					
					return CSUDI_NULL;
				}

				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOShow(0,TRUE)))
				{
					CSTCPrint("Video show  ß∞‹\n");
					
					if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
					{
						CSTCPrint("πÿ±’≤•∑≈∆˜ ß∞‹\n");
					}
					
					return CSUDI_NULL;
				}
			}
		}
		else
		{
			CSTCPrint("Player open  ß∞‹\n");
		}
	}
	else
	{
		CSTCPrint("À¯∂®Audio&Video Test_27Mbps_20070524.ts  ß∞‹\n");
	}

	return hPlayer;	
}

// ≤•∑≈∏ﬂ«Â ”∆µΩ⁄ƒø‘¥
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
		CSTCPrint("ªÒ»°Video , demux¥ÌŒÛ\n");
		
		return CSUDI_NULL;
	}

	sPlayerChnl.m_nDemux = nDemuxIndex;
	sPlayerChnl.m_nVideoDecoder = nVideoIndex;
	sPlayerChnl.m_nAudioDecoder = 0;

	//À¯≤‚ ‘¬Î¡˜∆µµ„
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
				CSTCPrint("À¯∂®SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts ß∞‹\n");

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
				CSTCPrint("À¯∂®mpeg2hd+h264sd.ts  ß∞‹\n");
				
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
				CSTCPrint("À¯∂®SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts  ß∞‹\n");
				
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
				CSTCPrint("À¯∂®mpeg2hd+h264sd.ts  ß∞‹\n");
				
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
				sStreamInfo.m_nPid = 1140;  //PALΩ⁄ƒø‘¥
				sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
				sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
			}

			if (EM_UDISCREEN_RESOLUTION_NTSC == eResolution)
			{
				sStreamInfo.m_nPid = 5137;  //NTSCΩ⁄ƒø‘¥
				sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
				sStreamInfo.m_uStreamType.m_eVideoType = eVideoType;
			}

			if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, &sStreamInfo, 1, CSUDI_NULL)))
			{
				CSTCPrint("Player set stream  ß∞‹\n");
				
				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
				{
					CSTCPrint("πÿ±’≤•∑≈∆˜ ß∞‹\n");
				}
				
				return CSUDI_NULL;
			}

			if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)))
			{
				CSTCPrint("Player start  ß∞‹\n");
				
				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
				{
					CSTCPrint("πÿ±’≤•∑≈∆˜ ß∞‹\n");
				}
				
				return CSUDI_NULL;
			}

			if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOShow(0,TRUE)))
			{
				CSTCPrint("Video show  ß∞‹\n");
				
				if (CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)))
				{
					CSTCPrint("πÿ±’≤•∑≈∆˜ ß∞‹\n");
				}
				
				return CSUDI_NULL;
			}
		}
	}
	else
	{
		CSTCPrint("Player open  ß∞‹\n");
	}

	return hPlayer;	
}

//∆¡ƒª∑÷±Ê¬ ∏ƒ±‰ªÿµ˜∫Ø ˝
static void CSUDISCREENCallback (CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice,void * pvUserData)
{
	(void)(eEvt);
	(void)(eScreenDevice);
	(void)(pvUserData);
}

//∑µªÿ”√ªß ˝æ›
static int g_nUserData;

static CSUDISCREENEvent_E g_eEvt;
static CSUDISCREENType_E g_eScreenDevice;

//∆¡ƒª∑÷±Ê¬ ∏ƒ±‰ªÿµ˜∫Ø ˝
static void CSUDISCREENChangeCallback (CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice,void * pvUserData)
{
	g_nUserData = *(int*)pvUserData;
	g_eEvt = eEvt;
	g_eScreenDevice = eScreenDevice;
}

//@CASEGROUP:CSUDISCREENAddCallback 
//@DESCRIPTION:≤‚ ‘ªÿµ˜∫Ø ˝Œ™NULLµƒ“Ï≥£«Èøˆ
//@PRECONDITION:
//@INPUT:1. fnScreenCallback = NULL
//@INPUT:2. pvUserData = NULL
//@EXPECTATION: ∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:µ˜”√CSUDISCREENAddCallback,ªÿµ˜∫Ø ˝¥´»ÎNULL,∆⁄Õ˚∑µªÿ÷µŒ™CSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_AddCallback_0001( void )
{
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;
	int nUserData = 0x1234;

	bResult = CSUDISCREENAddCallback(CSUDI_NULL, &nUserData);

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == bResult), "≤Œ ˝ºÏ≤‚ ß∞‹");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENAddCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:≤‚ ‘À˘”–≤Œ ˝æ˘Œ™’˝≥£,◊¢≤·≥…π¶≤¢«“…æ≥˝≥…π¶µƒ«Èøˆ
//@PRECONDITION:
//@INPUT:1. fnScreenCallback = ∫œ∑®∫Ø ˝µÿ÷∑
//@INPUT:2. pvUserData = NULL
//@EXPECTATION: ◊¢≤·≥…π¶≤¢«“…æ≥˝≥…π¶
//@EXECUTIONFLOW:1. µ˜”√CSUDISCREENAddCallback◊¢≤·¥´»Î∫œ∑®≤Œ ˝,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:2. µ˜”√CSUDISCREENDelCallback…æ≥˝◊¢≤·µƒ∫Ø ˝,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
CSUDI_BOOL CSTC_SCREEN_TEST_IT_AddCallback_0002( void )
{
	int nUserData = 0x1234;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENCallback, CSUDI_NULL)), "◊¢≤·ªÿµ˜∫Ø ˝ ß∞‹1");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENCallback, &nUserData)), "◊¢≤·ªÿµ˜∫Ø ˝ ß∞‹2");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENChangeCallback, CSUDI_NULL)), "◊¢≤·ªÿµ˜∫Ø ˝ ß∞‹3");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENChangeCallback, &nUserData)), "◊¢≤·ªÿµ˜∫Ø ˝ ß∞‹4");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENCallback, CSUDI_NULL)), "…æ≥˝ªÿµ˜∫Ø ˝ ß∞‹1");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENCallback, &nUserData)), "…æ≥˝ªÿµ˜∫Ø ˝ ß∞‹2");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENChangeCallback, CSUDI_NULL)), "…æ≥˝ªÿµ˜∫Ø ˝ ß∞‹3");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENChangeCallback, &nUserData)), "…æ≥˝ªÿµ˜∫Ø ˝ ß∞‹4");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENAddCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:≤‚ ‘◊¢≤· ˝¡ø≥¨π˝◊Ó¥Û◊¢≤· ˝32µƒ«Èøˆ
//@PRECONDITION:
//@INPUT:◊¢≤· ±¥´»Îœ‡Õ¨µƒªÿµ˜∫Ø ˝,≤ªÕ¨µƒpvUserDataµÿ÷∑
//@EXPECTATION: «∞32∏ˆªÿµ˜◊¢≤·≥…π¶,32“‘∫Û◊¢≤· ß∞‹
//@EXECUTIONFLOW:1. ∑¥∏¥µ˜”√CSUDISCREENAddCallback◊¢≤·ªÿµ˜∫Ø ˝33¥Œ,√ø¥Œ◊¢≤·¥´»Îµƒªÿµ˜∫Ø ˝“ª—˘,µ´ «ªÿµ˜∫Ø ˝≤Œ ˝µÿ÷∑≤ª“ª÷¬
//@EXECUTIONFLOW: ∆⁄Õ˚[0~31]”¶∏√◊¢≤·≥…π¶,µ⁄32¥Œ”¶∏√◊¢≤· ß∞‹
//@EXECUTIONFLOW:2. µ˜”√CSUDISCREENDelCallback…æ≥˝◊¢≤·≥…π¶µƒªÿµ˜∫Ø ˝,∆⁄Õ˚…æ≥˝≥…π¶
CSUDI_BOOL CSTC_SCREEN_TEST_IT_AddCallback_0003( void )
{
	int i = 0;
	int j = 0;

	for (i=0; i<33; i++)
	{
		if (32 == i)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDISCREENAddCallback(CSUDISCREENCallback, (int *)i), "≤Ω÷Ë1 ß∞‹\n");
			 break;
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENCallback, (int *)i), "≤Ω÷Ë1 ß∞‹£¨◊¢≤·≤ª≥…π¶\n");
		j ++;
	}

	CSTK_FATAL_POINT
	{
		for (i=0; i<j; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENCallback, (int *)i), "≤Ω÷Ë2 ß∞‹£¨…æ≥˝◊¢≤·≤ª≥…π¶\n");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENDelCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:≤‚ ‘CSUDISCREENDelCallback∂‘¥ÌŒÛ≤Œ ˝µƒºÏ≤‚ƒ‹¡¶
//@INPUT:…æ≥˝ ±¥´»Îµƒ≤Œ ˝¥ÌŒÛ
//@EXECUTIONFLOW:1. ¥´»Î¥ÌŒÛµƒ≤Œ ˝÷¡CSUDISCREENDelCallback
//@EXECUTIONFLOW:2. ∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_DelCallback_0001( void )
{
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	
	nResult = CSUDISCREENDelCallback(CSUDI_NULL, CSUDI_NULL);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "≤Œ ˝ºÏ≤‚ ß∞‹");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENDelCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:≤‚ ‘CSUDISCREENDelCallback…æ≥˝Œ¥∂®“Âµƒªÿµ˜∫Ø ˝
//@PRECONDITION:
//@INPUT:¥´»Î∫œ∑®≤Œ ˝
//@EXECUTIONFLOW:1. ◊¢≤·ªÿµ˜∫Ø ˝£¨≤Œ ˝Œ™CSUDISCREENCallback∫Õ&nUserData1
//@EXECUTIONFLOW:2. ≥¢ ‘…æ≥˝ªÿµ˜∫Ø ˝£¨≤Œ ˝Œ™CSUDISCREENCallback∫Õ&nUserData2£¨∆⁄Õ˚ ß∞‹
//@EXECUTIONFLOW:3. ≥¢ ‘…æ≥˝ªÿµ˜∫Ø ˝£¨≤Œ ˝Œ™CSUDISCREENChangeCallback∫Õ&nUserData1£¨∆⁄Õ˚ ß∞‹
//@EXECUTIONFLOW:4. ≥¢ ‘…æ≥˝ªÿµ˜∫Ø ˝£¨≤Œ ˝Œ™CSUDISCREENCallback∫Õ&nUserData1£¨∆⁄Õ˚≥…π¶
CSUDI_BOOL CSTC_SCREEN_TEST_IT_DelCallback_0002( void )
{
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	int nUserData1 = 0x1234;
	int nUserData2 = 0x4321;
	
	nResult = CSUDISCREENAddCallback(CSUDISCREENCallback, &nUserData1);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "◊¢≤·ªÿµ˜∫Ø ˝ ß∞‹");

	nResult = CSUDISCREENDelCallback(CSUDISCREENCallback, &nUserData2);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "…æ≥˝Œ¥◊¢≤·µƒªÿµ˜∫Ø ˝”¶∏√ ß∞‹1");
	
	nResult = CSUDISCREENDelCallback(CSUDISCREENChangeCallback, &nUserData1);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "…æ≥˝Œ¥◊¢≤·µƒªÿµ˜∫Ø ˝”¶∏√ ß∞‹2");

	nResult = CSUDISCREENDelCallback(CSUDISCREENCallback, &nUserData1);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "…æ≥˝ªÿµ˜∫Ø ˝ ß∞‹");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDISCREENDelCallback 
//@CASEGROUP:CSUDISCREENDelCallback 
//@DESCRIPTION:≤‚ ‘1000¥Œ…æ≥˝◊¢≤·ªÿµ˜∫Ø ˝
//@EXPECTATION:√ø¥Œ∂º◊¢≤·≥…π¶≤¢«“…æ≥˝≥…π¶
//@INPUT:¥´»Î∫œ∑®≤Œ ˝
//@EXECUTIONFLOW:1. µ˜”√CSUDISCREENAddCallback¥´»Î∫œ∑®µƒªÿµ˜∫Ø ˝“‘º∞userdata,∆⁄Õ˚◊¢≤·≥…π¶
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENDelCallback…æ≥˝◊¢≤·µƒªÿµ˜∫Ø ˝,∆⁄Õ˚∑µªÿ≥…π¶
//@EXECUTIONFLOW:3.÷ÿ∏¥≤Ω÷Ë1∫Õ2π≤1000¥Œ,∆⁄Õ˚√ø¥Œ∂º∑µªÿ≥…π¶
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
//@DESCRIPTION:‘⁄√ª”–µ˜”√CSUDISCREENSetResolution∫Ø ˝«∞,µ˜”√CSUDISCREENGetResolution”¶ƒ‹ªÒ»°ƒ¨»œ÷µ
//@PRECONDITION: œµÕ≥√ª”–µ˜”√CSUDISCREENSetResolution
//@EXPECTATION: µ˜”√CSUDISCREENGetResolution”¶∑µªÿ≥…π¶,≤¢ƒ‹ªÒ»°ƒ¨»œ÷µ
//@EXECUTIONFLOW:1. µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â ‰≥ˆÕ®µ¿µƒƒ¨»œ∑÷±Ê¬ ”¶∑µªÿ≥…π¶,À˘ªÒ»°µƒ÷µ”¶‘⁄EM_UDISCREEN_RESOLUTION_INVALID∫ÕEM_UDISCREEN_RESOLUTION_576P÷Æº‰
//@EXECUTIONFLOW:2. »Áπ˚Œ™∏ﬂ«Â∆ΩÃ®,µ˜”√CSUDISCREENGetResolutionªÒ»°∏ﬂ«Â ‰≥ˆÕ®µ¿µƒƒ¨»œ∑÷±Ê¬ ”¶∑µªÿ≥…π¶,À˘ªÒ»°µƒ÷µ”¶‘⁄EM_UDISCREEN_RESOLUTION_576P∫ÕEM_UDISCREEN_RESOLUTION_NUM÷Æº‰
CSUDI_BOOL CSTC_SCREEN_TEST_IT_BeforeSetResolution_0001( void )
{
	CSUDI_Error_Code nResult = 0;
	CSUDISCREENResolution_E sResolution = EM_UDISCREEN_RESOLUTION_INVALID;
	
	nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD, &sResolution);
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == nResult) && (sResolution > EM_UDISCREEN_RESOLUTION_INVALID && sResolution <= EM_UDISCREEN_RESOLUTION_576P), 
		"ªÒ»°±Í«Â ‰≥ˆÕ®µ¿Resolution»± °÷µ ß∞‹");

	if (!NotSupportHD())
	{
		sResolution = EM_UDISCREEN_RESOLUTION_INVALID;
		nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &sResolution);
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == nResult) && (sResolution >= EM_UDISCREEN_RESOLUTION_720P && sResolution < EM_UDISCREEN_RESOLUTION_NUM), 
			"ªÒ»°∏ﬂ«Â ‰≥ˆÕ®µ¿Resolution»± °÷µ ß∞‹");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘…Ë÷√…Ë±∏Œ™NULLµƒ«Èøˆ
//@PRECONDITION:
//@INPUT:1.peScreenDevice = NULL
//@INPUT:2.peResolution = CSUDISCREENResolution_E¿‡–Õ ˝◊È
//@INPUT:3.nCount=2
//@EXPECTATION: ∑µªÿ¥ÌŒÛ÷µCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:µ˜”√CSUDISCREENSetResolution peScreenDevice¥´»ÎNULL,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0001( void )
{
	CSUDISCREENResolution_E scResolution[2];
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;

	bResult = CSUDISCREENSetResolution(CSUDI_NULL, scResolution, 2);

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == bResult), "≤Œ ˝ºÏ≤‚ ß∞‹");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘∑÷±Ê¬ Œ™NULLµƒ«Èøˆ
//@PRECONDITION:
//@INPUT:1.peScreenDevice = …Ë±∏ ˝◊È
//@INPUT:2.peResolution = NULL
//@INPUT:3.nCount=2
//@EXPECTATION: ∑µªÿ¥ÌŒÛ÷µCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:µ˜”√CSUDISCREENSetResolution peResolution¥´»ÎNULL,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0002( void )
{
	CSUDISCREENType_E scScreenDevice[2];
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;

	bResult = CSUDISCREENSetResolution(scScreenDevice, CSUDI_NULL, 2);

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == bResult), "≤Œ ˝ºÏ≤‚ ß∞‹");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘nCount ¥ÌŒÛ∫ÕCSUDISCREENType_E∫ÕCSUDISCREENResolution_E“‘º∞nCount≤ª∆•≈‰µƒ«Èøˆ
//@PRECONDITION:
//@INPUT:1. peScreenDevice = …Ë±∏ ˝◊È,CSUDISCREENResolution_E¿‡–Õ ˝◊È,nCount = 0
//@INPUT:2. peScreenDevice = …Ë±∏ ˝◊È,CSUDISCREENResolution_E¿‡–Õ ˝◊È,nCount = -1
//@INPUT:2. peScreenDevice = …Ë±∏ ˝◊È,CSUDISCREENResolution_E¿‡–Õ ˝◊È,nCount = 3
//@EXPECTATION: ∑µªÿ¥ÌŒÛ÷µCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1. µ˜”√CSUDISCREENSetResolution nCount¥´»Î0,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2. µ˜”√CSUDISCREENSetResolution nCount¥´»Î-1,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3. µ˜”√CSUDISCREENSetResolution nCount¥´»Î3,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4. µ˜”√CSUDISCREENSetResolution£¨ CSUDISCREENType_E∫ÕCSUDISCREENResolution_E÷–µƒ≥…‘±¥ÌŒÛ,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0003( void )
{
	CSUDISCREENType_E scScreenDevice[2];
	CSUDISCREENType_E scScreenDevice_4[1];
	CSUDISCREENResolution_E scResolution[2];
	CSUDISCREENResolution_E scResolution_4[1];
	 		 
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice, scResolution, 0)),
		"≤Œ ˝ºÏ≤‚ ß∞‹1");

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice, scResolution, -1)),
		"≤Œ ˝ºÏ≤‚ ß∞‹2");

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice, scResolution, 3)),
		"≤Œ ˝ºÏ≤‚ ß∞‹3");

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice_4, scResolution, 2)),
		"≤Œ ˝ºÏ≤‚ ß∞‹4-1");

	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetResolution(scScreenDevice, scResolution_4, 2)),
		"≤Œ ˝ºÏ≤‚ ß∞‹4-2");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘Ω⁄ƒø‘¥ «±Í«ÂPALµƒ«Èøˆœ¬,∂‘…Ë±∏Ω¯––∏˜÷÷÷∆ Ω◊™ªª
//@PRECONDITION:
//@INPUT:1. ≤•∑≈±Í«ÂPALΩ⁄ƒø‘¥
//@EXPECTATION:1. µ±∂‘…Ë±∏…Ë÷√∑÷±Ê¬  ±∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª
//@EXPECTATION:2. µ±±Í«Â∫Õ∏ﬂ«Â ‰≥ˆµƒ≥°∆µ∫ÕΩ⁄ƒø‘¥µƒ≥°∆µ“ª÷¬,º¥æ˘Œ™50HZ ±“™«Ûª≠√ÊŒﬁ»Œ∫Œ“Ï≥£
//@EXPECTATION:3. µ± ‰≥ˆ±Í«Â∫Õ∏ﬂ«Â≥°∆µ≤ª“ª÷¬ ±ª≠√Êø…ƒ‹”–“Ï≥£,µ´ «“™«Û≤ªÀ¿ª˙
//@REMARK:1.…Ë±∏ «∑Ò÷ß≥÷ƒ≥÷÷∑÷±Ê¬  «Õ®π˝CSUDISCREENGetCapabilityªÒµ√
//@REMARK:2.»Áπ˚∆ΩÃ®÷ß≥÷1080P,µ±…Ë÷√ ‰≥ˆŒ™1080P ±,»∑»œª≠√Ê «∑Ò’˝≥£–Ë“™µÁ ”“≤÷ß≥÷1080P
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë»Áœ¬
//@EXECUTIONFLOW:1.1 À¯≤‚ ‘¬Î¡˜∆µµ„
//@EXECUTIONFLOW:1.2 µ˜”√CSUDIVIDEOGetCountªÒ»°videoΩ‚¬Î∆˜ ˝¡ø
//@EXECUTIONFLOW:1.3 µ˜”√CSUDIVIDEOGetCapability“¿¥Œ—∞’“µ⁄“ª∏ˆ÷ß≥÷±Í«ÂΩ‚¬Îµƒdecoder,≤¢Ω´∆‰◊˜Œ™Ω⁄ƒø≤•∑≈µƒ ”∆µΩ‚¬Î∆˜
//@EXECUTIONFLOW:1.4 µ˜”√CSUDIDEMUXGetCountªÒ»°demuxµƒ ˝¡ø
//@EXECUTIONFLOW:1.5 µ˜”√CSUDIDEMUXGetCapability“¿¥Œ—∞’“µ⁄“ª∏ˆ÷ß≥÷EM_UDI_DEMUX_PLAYµƒdemux”√”⁄Ω⁄ƒø≤•∑≈µƒdemux
//@EXECUTIONFLOW:1.6 µ˜”√CSUDIPLAYEROpen¥Úø™“ª∏ˆ≤•∑≈∆˜,∆⁄Õ˚¥Úø™≥…π¶
//@EXECUTIONFLOW:1.7 µ˜”√CSUDIPLAYERSetStream…Ë÷√≤‚ ‘¬Î¡˜ Ù–‘
//@EXECUTIONFLOW:1.8 µ˜”√CSUDIPLAYERStart≤•∑≈≤‚ ‘¬Î¡˜,∆⁄Õ˚”– ”∆µ ‰≥ˆ
//@EXECUTIONFLOW:2. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:3. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:4. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:5. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:6. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:7. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:8. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:9. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:10. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:11. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:12. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:13. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:14. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:15. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:16. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0004( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("[UDI2SCREENTEST]: ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	////∂‘”⁄±Í«Â∆ΩÃ®≤ª÷ß≥÷±Í«Â∏ﬂ«ÂÕ®µ¿“ª∆…Ë÷√£¨œ÷µ•∂¿∂‘±Í«ÂÕ®µ¿Ω¯––PALº∞NTSC÷∆ Ω◊™ªª£¨ƒ‹…Ë÷√≥…π¶«“ª≠√Ê’˝≥£‘Ú≤‚ ‘Õ®π˝°£
	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë1 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]:Yes,…Ë÷√∑÷±Ê¬ Œ™PAL∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]:No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë2 ß∞‹\n");
			
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë3 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]:Yes,…Ë÷√∑÷±Ê¬ Œ™NTSC∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]:No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë3 ß∞‹\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë1 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,…Ë÷√∑÷±Ê¬ Œ™PAL∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë4 ß∞‹\n");
	}
	else
	{
		CSTCPrint("********«Î»∑»œ“—¡¨Ω”SD/HD ¡Ω÷÷ ‰≥ˆ∂À***********\n");
	   	CSTKWaitAnyKey();
	   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë2 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë2 ß∞‹\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë3 ß∞‹\n"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë3 ß∞‹\n");

		 //»Áπ˚∆ΩÃ®÷ß≥÷1080P 50HZ
		 if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
		 {
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë4 ß∞‹\n"); 
			CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
			CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë4 ß∞‹\n");
		 }

		 CSTCPrint("[UDI2SCREENTEST] œ¬√ÊΩ´∂‘…Ë±∏Ω¯––÷∆ Ω◊™ªª◊‘∂ØªØ≤‚ ‘£¨∞¥»Œ“‚º¸ø™ º...\n");
		 CSTKWaitAnyKey();

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë5 ß∞‹\n");
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë6 ß∞‹\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë7 ß∞‹\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë8 ß∞‹\n"); 
		CSUDIOSThreadSleep(1000);

		//»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ  
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë9 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);

			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë10 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);
		}

		//»Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë11 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);
		}

		 //»Áπ˚∆ΩÃ®÷ß≥÷1080P 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë12 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);

			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë13 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);
		}
	}

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ 
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "≤Ω÷Ë14 ß∞‹\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
			}
		}

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘Ω⁄ƒø‘¥ «±Í«ÂNTSCµƒ«Èøˆœ¬,∂‘…Ë±∏Ω¯––∏˜÷÷÷∆ Ω◊™ªª
//@PRECONDITION:
//@INPUT:1. ≤•∑≈±Í«ÂNTSCΩ⁄ƒø‘¥
//@EXPECTATION:1. µ±∂‘…Ë±∏…Ë÷√∑÷±Ê¬  ±∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª
//@EXPECTATION:2. µ±±Í«Â∫Õ∏ﬂ«Â ‰≥ˆµƒ≥°∆µ∫ÕΩ⁄ƒø‘¥µƒ≥°∆µ“ª÷¬,º¥æ˘Œ™60HZ ±“™«Ûª≠√ÊŒﬁ»Œ∫Œ“Ï≥£
//@EXPECTATION:3. µ± ‰≥ˆ±Í«Â∫Õ∏ﬂ«Â≥°∆µ≤ª“ª÷¬ ±ª≠√Êø…ƒ‹”–“Ï≥£,µ´ «“™«Û≤ªÀ¿ª˙
//@REMARK:1.…Ë±∏ «∑Ò÷ß≥÷ƒ≥÷÷∑÷±Ê¬  «Õ®π˝CSUDISCREENGetCapabilityªÒµ√
//@REMARK:2.»Áπ˚∆ΩÃ®÷ß≥÷1080P,µ±…Ë÷√ ‰≥ˆŒ™1080P ±,»∑»œª≠√Ê «∑Ò’˝≥£–Ë“™µÁ ”“≤÷ß≥÷1080P
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂNTSC¬Î¡˜,œÍœ∏≤Ω÷Ë«Â≤Œº˚CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:3. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:4. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:5. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:6. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:7. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:8. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:9. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:10. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:11. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:12. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:13. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:14. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:15. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:16. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0005( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_NTSC);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("[UDI2SCREENTEST]:: ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	////∂‘”⁄±Í«Â∆ΩÃ®≤ª÷ß≥÷±Í«Â°¢∏ﬂ«ÂÕ®µ¿“ª∆…Ë÷√£¨œ÷µ•∂¿∂‘±Í«ÂÕ®µ¿Ω¯––PALº∞NTSC÷∆ Ω◊™ªª£¨ƒ‹…Ë÷√≥…π¶«“ª≠√Ê’˝≥£‘Ú≤‚ ‘Õ®π˝°£
	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë3 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,…Ë÷√∑÷±Ê¬ Œ™PAL∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë2 ß∞‹\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë1 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,…Ë÷√∑÷±Ê¬ Œ™NTSC∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë3 ß∞‹\n");

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë3 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,…Ë÷√∑÷±Ê¬ Œ™PAL∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë4 ß∞‹\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

		CSTCPrint("********«Î»∑»œ“—¡¨Ω”SD/HD ¡Ω÷÷ ‰≥ˆ∂À***********\n");
	  	CSTKWaitAnyKey();
		
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë2 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë2 ß∞‹\n");

		 //»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë3 ß∞‹\n");
			CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
			CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë3 ß∞‹\n");
		}
		 
		//»Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë4 ß∞‹\n"); 
			CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
			CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë4 ß∞‹\n");
		}

		CSTCPrint("[UDI2SCREENTEST] œ¬√ÊΩ´∂‘…Ë±∏Ω¯––÷∆ Ω◊™ªª◊‘∂ØªØ≤‚ ‘£¨∞¥»Œ“‚º¸ø™ º..\n");
		CSTKWaitAnyKey();

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë5 ß∞‹\n");
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë6 ß∞‹\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë7 ß∞‹\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë8 ß∞‹\n"); 
		CSUDIOSThreadSleep(1000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë9 ß∞‹\n"); 
		CSUDIOSThreadSleep(1000);

		//»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë10 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);
		}

		//»Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë11 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);

			scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë12 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);
		}

		//»Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ
		if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
		{
			scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
			scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë13 ß∞‹\n"); 
			CSUDIOSThreadSleep(1000);
		}
	}

	 CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ 
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "≤Ω÷Ë14 ß∞‹\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
			}
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘Ω⁄ƒø‘¥ «∏ﬂ«ÂEM_UDISCREEN_RESOLUTION_720P_50HZµƒ«Èøˆœ¬,∂‘±Í«Â∫Õ∏ﬂ«Â…Ë±∏Ω¯––±Í«Â÷∆ Ω◊™ªª,∏√≤‚ ‘”√¿˝÷˜“™”√”⁄∏ﬂ«Â∆ΩÃ®
//@PRECONDITION:∆ΩÃ®÷ß≥÷∏ﬂ«ÂΩ‚¬Î
//@INPUT:1. ≤•∑≈∏ﬂ«Â720P_50HZΩ⁄ƒø‘¥
//@EXPECTATION:1. µ±∂‘…Ë±∏…Ë÷√∑÷±Ê¬  ±∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª
//@EXPECTATION:2. µ±±Í«Â∫Õ∏ﬂ«Â ‰≥ˆµƒ≥°∆µ∫ÕΩ⁄ƒø‘¥µƒ≥°∆µ“ª÷¬,º¥æ˘Œ™60HZ ±“™«Ûª≠√ÊŒﬁ»Œ∫Œ“Ï≥£
//@EXPECTATION:3. µ± ‰≥ˆ±Í«Â∫Õ∏ﬂ«Â≥°∆µ≤ª“ª÷¬ ±ª≠√Êø…ƒ‹”–“Ï≥£,µ´ «“™«Û≤ªÀ¿ª˙
//@REMARK:1.…Ë±∏ «∑Ò÷ß≥÷ƒ≥÷÷∑÷±Ê¬  «Õ®π˝CSUDISCREENGetCapabilityªÒµ√
//@REMARK:2.»Áπ˚∆ΩÃ®÷ß≥÷1080P,µ±…Ë÷√ ‰≥ˆŒ™1080P ±,»∑»œª≠√Ê «∑Ò’˝≥£–Ë“™µÁ ”“≤÷ß≥÷1080P
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈∏ﬂ«Â720P_50HZ¬Î¡˜,œÍœ∏≤Ω÷Ë»Áœ¬:
//@EXECUTIONFLOW:1.1 À¯≤‚ ‘¬Î¡˜∆µµ„
//@EXECUTIONFLOW:1.2 µ˜”√CSUDIVIDEOGetCountªÒ»°videoΩ‚¬Î∆˜ ˝¡ø
//@EXECUTIONFLOW:1.3 µ˜”√CSUDIVIDEOGetCapability“¿¥Œ—∞’“µ⁄“ª∏ˆ÷ß≥÷∏ﬂ«ÂΩ‚¬Îµƒdecoder,≤¢Ω´∆‰◊˜Œ™Ω⁄ƒø≤•∑≈µƒ ”∆µΩ‚¬Î∆˜
//@EXECUTIONFLOW:1.4 µ˜”√CSUDIDEMUXGetCountªÒ»°demuxµƒ ˝¡ø
//@EXECUTIONFLOW:1.5 µ˜”√CSUDIDEMUXGetCapability“¿¥Œ—∞’“µ⁄“ª∏ˆ÷ß≥÷EM_UDI_DEMUX_PLAYµƒdemux”√”⁄Ω⁄ƒø≤•∑≈µƒdemux
//@EXECUTIONFLOW:1.6 µ˜”√CSUDIPLAYEROpen¥Úø™“ª∏ˆ≤•∑≈∆˜,∆⁄Õ˚¥Úø™≥…π¶
//@EXECUTIONFLOW:1.7 µ˜”√CSUDIPLAYERSetStream…Ë÷√≤‚ ‘¬Î¡˜ Ù–‘
//@EXECUTIONFLOW:1.8 µ˜”√CSUDIPLAYERStart≤•∑≈≤‚ ‘¬Î¡˜,∆⁄Õ˚”– ”∆µ ‰≥ˆ
//@EXECUTIONFLOW:2. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:3. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:4. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:5. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:6. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:7. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:8. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:9. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:10. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:11. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:12. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:13. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:14. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:15. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:16. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0006( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	if (NotSupportHD())
	{
		CSTCPrint("[UDI2SCREENTEST]  ∏√”√¿˝”√”⁄≤‚ ‘∏ﬂ«Â∆ΩÃ®\r\n");
		return CSUDI_TRUE;
	}

	hPlayer = PlayHDProgram(EM_UDI_VID_STREAM_H264 , EM_UDISCREEN_RESOLUTION_720P_50HZ);  //≤‚ ‘Ω⁄ƒø‘¥ «∏ﬂ«ÂEM_UDISCREEN_RESOLUTION_720P_50HZ
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹");

	CSTCPrint("[UDI2SCREENTEST]:: ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

	CSTCPrint("********«Î»∑»œ“—¡¨Ω”SD/HD ¡Ω÷÷ ‰≥ˆ∂À***********\n");
	CSTKWaitAnyKey();
	
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë2 ß∞‹");
	CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë2 ß∞‹");

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë3 ß∞‹"); 
	CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë3 ß∞‹");

	 //»Áπ˚∆ΩÃ®÷ß≥÷1080P 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë4 ß∞‹"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë4 ß∞‹");
	}

	CSTCPrint("[UDI2SCREENTEST] œ¬√ÊΩ´∂‘…Ë±∏Ω¯––÷∆ Ω◊™ªª◊‘∂ØªØ≤‚ ‘...\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë5 ß∞‹");
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë6 ß∞‹"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë7 ß∞‹"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë8 ß∞‹"); 
	CSUDIOSThreadSleep(3000);

	//»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë9 ß∞‹"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë10 ß∞‹"); 
		CSUDIOSThreadSleep(3000);
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë11 ß∞‹"); 
		CSUDIOSThreadSleep(3000);
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080P 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë12 ß∞‹"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë13 ß∞‹"); 
		CSUDIOSThreadSleep(3000);
	}

	CSTK_FATAL_POINT
	{
		 //ª÷∏¥≤‚ ‘«∞∑÷±Ê
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
		}

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘Ω⁄ƒø‘¥ «∏ﬂ«ÂEM_UDISCREEN_RESOLUTION_720Pµƒ«Èøˆœ¬,∂‘±Í«Â∫Õ∏ﬂ«Â…Ë±∏Ω¯––±Í«Â÷∆ Ω◊™ªª,∏√≤‚ ‘”√¿˝÷˜“™”√”⁄∏ﬂ«Â∆ΩÃ®
//@PRECONDITION:∆ΩÃ®÷ß≥÷∏ﬂ«ÂΩ‚¬Î
//@INPUT:1. ≤•∑≈∏ﬂ«Â720PΩ⁄ƒø‘¥
//@EXPECTATION:1. µ±∂‘…Ë±∏…Ë÷√∑÷±Ê¬  ±∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª
//@EXPECTATION:2. µ±±Í«Â∫Õ∏ﬂ«Â ‰≥ˆµƒ≥°∆µ∫ÕΩ⁄ƒø‘¥µƒ≥°∆µ“ª÷¬,º¥æ˘Œ™60HZ ±“™«Ûª≠√ÊŒﬁ»Œ∫Œ“Ï≥£
//@EXPECTATION:3. µ± ‰≥ˆ±Í«Â∫Õ∏ﬂ«Â≥°∆µ≤ª“ª÷¬ ±ª≠√Êø…ƒ‹”–“Ï≥£,µ´ «“™«Û≤ªÀ¿ª˙
//@REMARK:1.…Ë±∏ «∑Ò÷ß≥÷ƒ≥÷÷∑÷±Ê¬  «Õ®π˝CSUDISCREENGetCapabilityªÒµ√
//@REMARK:2.»Áπ˚∆ΩÃ®÷ß≥÷1080P,µ±…Ë÷√ ‰≥ˆŒ™1080P ±,»∑»œª≠√Ê «∑Ò’˝≥£–Ë“™µÁ ”“≤÷ß≥÷1080P
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂNTSC¬Î¡˜,œÍœ∏≤Ω÷Ë«Â≤Œº˚CSTC_SCREEN_TEST_IT_SetResolution_0006≤Ω÷Ë1
//@EXECUTIONFLOW:2. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:3. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:4. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:5. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:6. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:7. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:8. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:9. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:10. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:11. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:12. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:13. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:14. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:15. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:16. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0007( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	if (NotSupportHD())
	{
		CSTCPrint("[UDI2SCREENTEST]  ∏√”√¿˝”√”⁄≤‚ ‘∏ﬂ«Â∆ΩÃ®\r\n");
		return CSUDI_TRUE;
	}

	hPlayer = PlayHDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_720P);  //≤‚ ‘Ω⁄ƒø‘¥ «∏ﬂ«ÂEM_UDISCREEN_RESOLUTION_720P

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("[UDI2SCREENTEST]:: ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

	CSTCPrint("********«Î»∑»œ“—¡¨Ω”SD/HD ¡Ω÷÷ ‰≥ˆ∂À***********\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë2 ß∞‹\n");
	CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë2 ß∞‹\n");

	//»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë3 ß∞‹\n"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë3 ß∞‹\n");
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë4 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë4 ß∞‹\n");
	}

	CSTCPrint("[UDI2SCREENTEST] œ¬√ÊΩ´∂‘…Ë±∏Ω¯––÷∆ Ω◊™ªª◊‘∂ØªØ≤‚ ‘...\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë5 ß∞‹\n");
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë6 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë7 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë8 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë9 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	//»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë10 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë11 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë12 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080P 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë13 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ 
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘Ω⁄ƒø‘¥ «∏ﬂ«ÂEM_UDISCREEN_RESOLUTION_1080I_50HZµƒ«Èøˆœ¬,∂‘±Í«Â∫Õ∏ﬂ«Â…Ë±∏Ω¯––±Í«Â÷∆ Ω◊™ªª,∏√≤‚ ‘”√¿˝÷˜“™”√”⁄∏ﬂ«Â∆ΩÃ®
//@PRECONDITION:∆ΩÃ®÷ß≥÷∏ﬂ«ÂΩ‚¬Î
//@INPUT:1. ≤•∑≈∏ﬂ«Â1080I_50HZΩ⁄ƒø‘¥
//@EXPECTATION:1. µ±∂‘…Ë±∏…Ë÷√∑÷±Ê¬  ±∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª
//@EXPECTATION:2. µ±±Í«Â∫Õ∏ﬂ«Â ‰≥ˆµƒ≥°∆µ∫ÕΩ⁄ƒø‘¥µƒ≥°∆µ“ª÷¬,º¥æ˘Œ™60HZ ±“™«Ûª≠√ÊŒﬁ»Œ∫Œ“Ï≥£
//@EXPECTATION:3. µ± ‰≥ˆ±Í«Â∫Õ∏ﬂ«Â≥°∆µ≤ª“ª÷¬ ±ª≠√Êø…ƒ‹”–“Ï≥£,µ´ «“™«Û≤ªÀ¿ª˙
//@REMARK:1.…Ë±∏ «∑Ò÷ß≥÷ƒ≥÷÷∑÷±Ê¬  «Õ®π˝CSUDISCREENGetCapabilityªÒµ√
//@REMARK:2.»Áπ˚∆ΩÃ®÷ß≥÷1080P,µ±…Ë÷√ ‰≥ˆŒ™1080P ±,»∑»œª≠√Ê «∑Ò’˝≥£–Ë“™µÁ ”“≤÷ß≥÷1080P
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈∏ﬂ«Â720P_50HZ¬Î¡˜,œÍœ∏≤Ω÷Ë«Â≤Œº˚CSTC_SCREEN_TEST_IT_SetResolution_0006≤Ω÷Ë1
//@EXECUTIONFLOW:2. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:3. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:4. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:5. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:6. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:7. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:8. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:9. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:10. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:11. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:12. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:13. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:14. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:15. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:16. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0008( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	if (NotSupportHD())
	{
		CSTCPrint("[UDI2SCREENTEST]  ∏√”√¿˝”√”⁄≤‚ ‘∏ﬂ«Â∆ΩÃ®\r\n");
		return CSUDI_TRUE;
	}

	hPlayer = PlayHDProgram( EM_UDI_VID_STREAM_H264, EM_UDISCREEN_RESOLUTION_1080I_50HZ);  //≤‚ ‘Ω⁄ƒø‘¥ «∏ﬂ«ÂEM_UDISCREEN_RESOLUTION_1080I_50HZ

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("[UDI2SCREENTEST]:: ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

	CSTCPrint("********«Î»∑»œ“—¡¨Ω”SD/HD ¡Ω÷÷ ‰≥ˆ∂À***********\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë2 ß∞‹\n");
	CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë2 ß∞‹\n");

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë3 ß∞‹\n"); 
	CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë3 ß∞‹\n");

	//»Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë4 ß∞‹\n"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë4 ß∞‹\n");
	}

	CSTCPrint("[UDI2SCREENTEST] œ¬√ÊΩ´∂‘…Ë±∏Ω¯––÷∆ Ω◊™ªª◊‘∂ØªØ≤‚ ‘...\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë5 ß∞‹\n");
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë6 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë7 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë8 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	//»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë9 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë10 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë11 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080P 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë12 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë13 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ 
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘Ω⁄ƒø‘¥ «∏ﬂ«ÂEM_UDISCREEN_RESOLUTION_1080Iµƒ«Èøˆœ¬,∂‘±Í«Â∫Õ∏ﬂ«Â…Ë±∏Ω¯––±Í«Â÷∆ Ω◊™ªª,∏√≤‚ ‘”√¿˝÷˜“™”√”⁄∏ﬂ«Â∆ΩÃ®
//@PRECONDITION:∆ΩÃ®÷ß≥÷∏ﬂ«ÂΩ‚¬Î
//@INPUT:1. ≤•∑≈∏ﬂ«Â1080I 60HZΩ⁄ƒø‘¥
//@EXPECTATION:1. µ±∂‘…Ë±∏…Ë÷√∑÷±Ê¬  ±∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª
//@EXPECTATION:2. µ±±Í«Â∫Õ∏ﬂ«Â ‰≥ˆµƒ≥°∆µ∫ÕΩ⁄ƒø‘¥µƒ≥°∆µ“ª÷¬,º¥æ˘Œ™60HZ ±“™«Ûª≠√ÊŒﬁ»Œ∫Œ“Ï≥£
//@EXPECTATION:3. µ± ‰≥ˆ±Í«Â∫Õ∏ﬂ«Â≥°∆µ≤ª“ª÷¬ ±ª≠√Êø…ƒ‹”–“Ï≥£,µ´ «“™«Û≤ªÀ¿ª˙
//@REMARK:1.…Ë±∏ «∑Ò÷ß≥÷ƒ≥÷÷∑÷±Ê¬  «Õ®π˝CSUDISCREENGetCapabilityªÒµ√
//@REMARK:2.»Áπ˚∆ΩÃ®÷ß≥÷1080P,µ±…Ë÷√ ‰≥ˆŒ™1080P ±,»∑»œª≠√Ê «∑Ò’˝≥£–Ë“™µÁ ”“≤÷ß≥÷1080P
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂNTSC¬Î¡˜,œÍœ∏≤Ω÷Ë«Â≤Œº˚CSTC_SCREEN_TEST_IT_SetResolution_0006≤Ω÷Ë1
//@EXECUTIONFLOW:2. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:3. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:4. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£
//@EXECUTIONFLOW:5. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:6. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:7. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:8. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:9. …Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:10. »Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:11. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:12. »Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_NTSC,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:13. »Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ,…Ë÷√±Í«Â ”∆µ ‰≥ˆ∏Ò ΩŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚≤ªÀ¿ª˙,sleep 1S
//@EXECUTIONFLOW:14. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:15. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:16. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0009( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];

	if (NotSupportHD())
	{
		CSTCPrint("[UDI2SCREENTEST]  ∏√”√¿˝”√”⁄≤‚ ‘∏ﬂ«Â∆ΩÃ®\r\n");
		return CSUDI_TRUE;
	}

	hPlayer = PlayHDProgram( EM_UDI_VID_STREAM_H264, EM_UDISCREEN_RESOLUTION_1080I);  //≤‚ ‘Ω⁄ƒø‘¥ «∏ﬂ«ÂEM_UDISCREEN_RESOLUTION_1080I

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("[UDI2SCREENTEST]:: ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

	CSTCPrint("********«Î»∑»œ“—¡¨Ω”SD/HD ¡Ω÷÷ ‰≥ˆ∂À***********\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë2 ß∞‹\n");
	CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
	CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë2 ß∞‹\n");

	//»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë3 ß∞‹\n");
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë3 ß∞‹\n");
	} 

	//»Áπ˚∆ΩÃ®÷ß≥÷1080p 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë4 ß∞‹\n"); 
		CSTCPrint("[UDI2SCREENTEST]::Yes,∑÷±Ê¬ ’˝≥£«–ªª,ª≠√Ê’˝≥£,Œﬁ∂∂∂Ø°¢Œﬁ…¡À∏\r\n");
		CSTCPrint("[UDI2SCREENTEST]::No,∑÷±Ê¬ ≤ªƒ‹’˝≥£«–ªªªÚª≠√Ê≤ª’˝≥£ \r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë4 ß∞‹\n");
	}

	CSTCPrint("[UDI2SCREENTEST] œ¬√ÊΩ´∂‘…Ë±∏Ω¯––÷∆ Ω◊™ªª◊‘∂ØªØ≤‚ ‘...\n");
	CSTKWaitAnyKey();

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë5 ß∞‹\n");
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë6 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë7 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë8 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	scResolution[1] = EM_UDISCREEN_RESOLUTION_720P;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë9 ß∞‹\n"); 
	CSUDIOSThreadSleep(3000);

	//»Áπ˚∆ΩÃ®÷ß≥÷1080i 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë10 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080p 50HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë11 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);

		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë12 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	//»Áπ˚∆ΩÃ®÷ß≥÷1080P 60HZ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080P ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë13 ß∞‹\n"); 
		CSUDIOSThreadSleep(3000);
	}

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ 
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘Ω⁄ƒø‘¥ «±Í«ÂPALµƒ«Èøˆœ¬,∂‘±Í«Â…Ë±∏Ω¯––±Í«Â÷∆ Ω◊™ªª,∏√≤‚ ‘”√¿˝÷˜“™”√”⁄÷ß≥÷π˙º  –≥°µƒ±Í«Â∆ΩÃ®
//@PRECONDITION: ∆ΩÃ®÷ß≥÷π˙Õ‚≤ø∑÷ªÚ’ﬂ»´≤ø ”∆µ∑÷±Ê¬  ‰≥ˆ
//@INPUT:1. ≤•∑≈±Í«ÂPALΩ⁄ƒø‘¥
//@EXPECTATION: µ±∂‘…Ë±∏…Ë÷√∑÷±Ê¬  ±∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª
//@REMARK:1.…Ë±∏ «∑Ò÷ß≥÷ƒ≥÷÷∑÷±Ê¬  «Õ®π˝CSUDISCREENGetCapabilityªÒµ√
//@REMARK:2.”––©∑÷±Ê¬  ‰≥ˆ,‘⁄»∑»œª≠√Ê «∑Ò’˝≥£ ±,–Ë“™÷ß≥÷∏√∑÷±Ê¬ µƒµÁ ”,µ‰–Õµƒ»ÁEM_UDISCREEN_RESOLUTION_SECAM
//@REMARK:3.√ª”–∏√œÓ≤‚ ‘–Ë«Û«Î÷±Ω”Ã¯π˝
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂNTSC¬Î¡˜,œÍœ∏≤Ω÷Ë«Â≤Œº˚CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2. ¬ﬁ¡–À˘”–∆ΩÃ®÷ß≥÷µƒ∑÷±Ê¬ ,≤¢µ»¥˝”√ªß—°‘Ò
//@EXECUTIONFLOW:3. …Ë÷√”√ªß—°‘Òµƒ÷∆ Ω
//@EXECUTIONFLOW:4. ÷ÿ∏¥3µΩ4≤Ω÷±µΩ”√ªß—°‘ÒÕÀ≥ˆŒ™÷π
//@EXECUTIONFLOW:5. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:6. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:7. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0010( void )
{
	CSUDISCREENCapability_S sCapabilityInfo;
	CSUDISCREENType_E eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDISCREENResolution_E scResolution[2];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	char arr[10];
	int j = 0;
	int k= 0;

	CSTCPrint("[UDI2SCREENTEST]∏√≤‚ ‘”√¿˝÷˜“™”√”⁄÷ß≥÷π˙º  –≥°µƒ±Í«Â∆ΩÃ®\n");

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //≤‚ ‘Ω⁄ƒø
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("[UDI2SCREENTEST]:: ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£? \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice, &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetCapability( eScreenDevice, &sCapabilityInfo), "ªÒ»°…Ë±∏∑÷±Ê¬ …Ë÷√ƒ‹¡¶ ß∞‹\n");

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

		CSTCPrint("∏˘æ›∆¡ƒªœ‘ æ—°‘Òœ‡”¶µƒ∑÷±Ê¬ £¨∞¥q º¸ ÕÀ≥ˆ \n");  

		if(CSTKGetString(arr, &j))
		{
			if((0<=j) && (j<k))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &sCapabilityInfo.m_eResolution[j], 1), "…Ë÷√∑÷±Ê¬  ß∞‹\n");
				CSTCPrint("∑÷±Ê¬ …Ë÷√ «∑Ò”Î‘§∆⁄“ª÷¬???\n");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "…Ë÷√∑÷±Ê¬ ”Î‘§∆⁄≤ª∑˚");
			}
			else
			{
				CSTCPrint("√ª”–∂‘”¶µƒ∑÷±Ê¬ ø…“‘…Ë÷√\r\n\n");  
			}
		}
		else
		{
			if('q' == arr[0])
			{
				CSTCPrint("ÕÀ≥ˆ\r\n");  
				break;
			}
			else
			{
				CSTCPrint(" ‰»Î¥ÌŒÛ£¨«Î÷ÿ–¬ ‰»Î\r\n\n");  
			}
		}
	}while(1);      	

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ 
		if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
		{
			scResolution[0] = g_scResolution[0];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &g_scResolution[0], 1), "≤Ω÷Ë5 ß∞‹\n");
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘Ω⁄ƒø‘¥ «±Í«ÂNTSCµƒ«Èøˆœ¬,∂‘±Í«Â…Ë±∏Ω¯––±Í«Â÷∆ Ω◊™ªª,∏√≤‚ ‘”√¿˝÷˜“™”√”⁄÷ß≥÷π˙º  –≥°µƒ±Í«Â∆ΩÃ®
//@PRECONDITION: ∆ΩÃ®÷ß≥÷π˙Õ‚≤ø∑÷ªÚ’ﬂ»´≤ø ”∆µ∑÷±Ê¬  ‰≥ˆ
//@INPUT:1. ≤•∑≈±Í«ÂNTSCΩ⁄ƒø‘¥
//@EXPECTATION: µ±∂‘…Ë±∏…Ë÷√∑÷±Ê¬  ±∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª
//@REMARK:1.…Ë±∏ «∑Ò÷ß≥÷ƒ≥÷÷∑÷±Ê¬  «Õ®π˝CSUDISCREENGetCapabilityªÒµ√
//@REMARK:2.”––©∑÷±Ê¬  ‰≥ˆ,‘⁄»∑»œª≠√Ê «∑Ò’˝≥£ ±,–Ë“™÷ß≥÷∏√∑÷±Ê¬ µƒµÁ ”,µ‰–Õµƒ»ÁEM_UDISCREEN_RESOLUTION_SECAM
//@REMARK:3.√ª”–∏√œÓ≤‚ ‘–Ë«Û«Î÷±Ω”Ã¯π˝
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂNTSC¬Î¡˜,œÍœ∏≤Ω÷Ë«Â≤Œº˚CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2. ¬ﬁ¡–À˘”–∆ΩÃ®÷ß≥÷µƒ∑÷±Ê¬ ,≤¢µ»¥˝”√ªß—°‘Ò
//@EXECUTIONFLOW:3. …Ë÷√”√ªß—°‘Òµƒ÷∆ Ω
//@EXECUTIONFLOW:4. ÷ÿ∏¥3µΩ4≤Ω÷±µΩ”√ªß—°‘ÒÕÀ≥ˆŒ™÷π
//@EXECUTIONFLOW:5. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:6. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:7. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0011( void )
{
	CSUDISCREENCapability_S sCapabilityInfo;
	CSUDISCREENType_E eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDISCREENResolution_E scResolution[2];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	char arr[10];
	int j = 0;
	int k = 0;

	CSTCPrint("[UDI2SCREENTEST]∏√≤‚ ‘”√¿˝÷˜“™”√”⁄÷ß≥÷π˙º  –≥°µƒ±Í«Â∆ΩÃ®\n");

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_NTSC);  //≤‚ ‘Ω⁄ƒø
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("[UDI2SCREENTEST]:: ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetCapability( eScreenDevice, &sCapabilityInfo), "ªÒ»°…Ë±∏∑÷±Ê¬ …Ë÷√ƒ‹¡¶ ß∞‹\n");

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

		CSTCPrint("∏˘æ›∆¡ƒªœ‘ æ—°‘Òœ‡”¶µƒ∑÷±Ê¬ £¨∞¥q º¸ ÕÀ≥ˆ \n");  

		if(CSTKGetString(arr, &j))
		{
			if((0<=j) && (j<k))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &sCapabilityInfo.m_eResolution[j], 1), "…Ë÷√∑÷±Ê¬  ß∞‹\n");
				CSTCPrint("∑÷±Ê¬ …Ë÷√ «∑Ò”Î‘§∆⁄“ª÷¬???\n");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "…Ë÷√∑÷±Ê¬ ”Î‘§∆⁄≤ª∑˚");
			}
			else
			{
				CSTCPrint("√ª”–∂‘”¶µƒ∑÷±Ê¬ ø…“‘…Ë÷√\r\n\n");  
			}
		}
		else
		{
			if('q' == arr[0])
			{
				CSTCPrint("ÕÀ≥ˆ\r\n");  
				break;
			}
			else
			{
				CSTCPrint(" ‰»Î¥ÌŒÛ£¨«Î÷ÿ–¬ ‰»Î\r\n\n");  
			}
		}
	}while(1);      	

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ 
		if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
		{
			scResolution[0] = g_scResolution[0];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &g_scResolution[0], 1), "≤Ω÷Ë5 ß∞‹\n");
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
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
//@DESCRIPTION:≤‚ ‘ ”∆µ∑÷±Ê¬ «–ªª ±ªÿµ˜∫Ø ˝µƒ”––ß–‘“‘º∞’˝»∑–‘
//@PRECONDITION: 
//@INPUT: ’˝≥£≤•∑≈“ª∏ˆ±Í«ÂPALΩ⁄ƒø‘¥,◊¢≤· ”∆µ∑÷±Ê¬ «–ªªªÿµ˜∫Ø ˝
//@EXPECTATION:  ”∆µ∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª, ‰≥ˆŒﬁ“Ï≥£,≤¢«“ªÿµ˜∫Ø ˝π¶ƒ‹’˝≥£
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2. µ˜”√CSUDISCREENAddCallback◊¢≤· ”∆µ∑÷±Ê¬ «–ªªªÿµ˜∫Ø ˝
//@EXECUTIONFLOW:3. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_PAL,»Áπ˚≥ı ºªØ ±µƒ∑÷±Ê¬ ≤ªŒ™EM_UDISCREEN_RESOLUTION_PAL ∆⁄Õ˚(1)ªÿµ˜∫Ø±ªµ˜”√(2)ªÿµ˜∫Ø ˝≤Œ ˝’˝»∑
//@EXECUTIONFLOW:(3) ¬º˛¿‡–ÕŒ™EM_UDISCREEN_RESOLUTION_CHANGED (4)…Ë±∏Œ™EM_UDI_VOUT_DEVICE_SD
//@EXECUTIONFLOW:4. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_NTSC,∆⁄Õ˚(1)ªÿµ˜∫Ø±ªµ˜”√(2)ªÿµ˜∫Ø ˝≤Œ ˝’˝»∑
//@EXECUTIONFLOW:(3) ¬º˛¿‡–ÕŒ™EM_UDISCREEN_RESOLUTION_CHANGED (4)…Ë±∏Œ™EM_UDI_VOUT_DEVICE_SD
//@EXECUTIONFLOW:5. »Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â ‰≥ˆ…Ë÷√∏ﬂ«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,»Áπ˚≥ı ºªØ ±µƒ∑÷±Ê¬ ≤ªŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ ∆⁄Õ˚(1)ªÿµ˜∫Ø±ªµ˜”√(2)ªÿµ˜∫Ø ˝≤Œ ˝’˝»∑
//@EXECUTIONFLOW:(3) ¬º˛¿‡–ÕŒ™EM_UDISCREEN_RESOLUTION_CHANGED (4)…Ë±∏Œ™EM_UDI_VOUT_DEVICE_HD
//@EXECUTIONFLOW:6. »Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â ‰≥ˆ…Ë÷√∏ﬂ«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,±Í«ÂŒ™EM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:∆⁄Õ˚(1)ªÿµ˜∫Ø±ªµ˜”√(2)ªÿµ˜∫Ø ˝≤Œ ˝’˝»∑(3) ¬º˛¿‡–ÕŒ™EM_UDISCREEN_RESOLUTION_CHANGED (4)…Ë±∏º¥”–∏ﬂ«Â”÷”–±Í«Â
//@EXECUTIONFLOW:7. »Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â ‰≥ˆ…Ë÷√∏ﬂ«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,±Í«ÂŒ™EM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:∆⁄Õ˚(1)ªÿµ˜∫Ø±ªµ˜”√(2)ªÿµ˜∫Ø ˝≤Œ ˝’˝»∑(3) ¬º˛¿‡–ÕŒ™EM_UDISCREEN_RESOLUTION_CHANGED (4)…Ë±∏÷ª”–∏ﬂ«Â√ª”–±Í«Â
//@EXECUTIONFLOW:8. µ˜”√CSUDISCREENDelCallback»•≥˝ªÿµ˜∫Ø ˝
//@EXECUTIONFLOW:9. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_NTSC∆⁄Õ˚ªÿµ˜∫Ø ˝≤ª±ªµ˜”√
//@EXECUTIONFLOW:10. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_PAL,∆⁄Õ˚ªÿµ˜∫Ø ˝≤ª±ªµ˜”√
//@EXECUTIONFLOW:11. »Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â ‰≥ˆ…Ë÷√∏ﬂ«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚ªÿµ˜∫Ø ˝≤ª±ªµ˜”√
//@EXECUTIONFLOW:12. »Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â ‰≥ˆ…Ë÷√∏ﬂ«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,
//@EXECUTIONFLOW:±Í«Â ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_NTSC,∆⁄Õ˚ªÿµ˜∫Ø ˝≤ª±ªµ˜”√
//@EXECUTIONFLOW:13. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_PAL,∆⁄Õ˚ªÿµ˜∫Ø ˝≤ª±ªµ˜”√
//@EXECUTIONFLOW:14. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:15. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:16. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0012( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];
	int nUserData = 0x1234;

	InitParam();

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL); //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "≤Ω÷Ë1£¨∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");
	CSTCPrint(" ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°±Í«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°±Í«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°∏ﬂ«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CSUDISCREENChangeCallback, (int*)&nUserData), "≤Ω÷Ë2£¨◊¢≤·ªÿµ˜∫Ø ˝ ß∞‹\n");

	InitParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë3 ß∞‹\n");

	if (scResolution[0] != g_scResolution[0])
	{
		CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "≤Ω÷Ë3£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "≤Ω÷Ë3£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_SD == g_eScreenDevice, "≤Ω÷Ë3£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");
	}

	InitParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë4 ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "≤Ω÷Ë4£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
	CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "≤Ω÷Ë4£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
	CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_SD == g_eScreenDevice, "≤Ω÷Ë4£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");

	//»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â ‰≥ˆ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I_50HZ ))
	{
		InitParam();
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1], 1), "≤Ω÷Ë5 ß∞‹\n");
		if (EM_UDISCREEN_RESOLUTION_1080I_50HZ != g_scResolution[1])
		{
			CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "≤Ω÷Ë5£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
			CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "≤Ω÷Ë5£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
			CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_HD == g_eScreenDevice, "≤Ω÷Ë5£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");
		}

		InitParam();
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë6 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "≤Ω÷Ë6£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "≤Ω÷Ë6£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_SD == g_eScreenDevice, "≤Ω÷Ë6£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");
	}

	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_720P_50HZ ))
	{
		InitParam();

		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë7 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "≤Ω÷Ë7.1£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "≤Ω÷Ë7.1£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "≤Ω÷Ë7.1£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");

		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°∏ﬂ«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1], 1), "≤Ω÷Ë7 ß∞‹\n");

        if (EM_UDISCREEN_RESOLUTION_720P_50HZ != g_scResolution[1])
		{
			CSTK_ASSERT_TRUE_FATAL(0x1234 == g_nUserData, "≤Ω÷Ë7.2£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
			CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_CHANGED == g_eEvt, "≤Ω÷Ë7.2£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
			CSTK_ASSERT_TRUE_FATAL(EM_UDI_VOUT_DEVICE_HD == g_eScreenDevice, "≤Ω÷Ë7.2£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");
        }
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CSUDISCREENChangeCallback, (int*)&nUserData), "≤Ω÷Ë8 ß∞‹\n");

	InitParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë9 ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "≤Ω÷Ë9£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "≤Ω÷Ë9£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "≤Ω÷Ë9£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë10 ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "≤Ω÷Ë10£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "≤Ω÷Ë10£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "≤Ω÷Ë10£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");

	//»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â ‰≥ˆ
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I_50HZ ))
	{
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1], 1), "≤Ω÷Ë11 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "≤Ω÷Ë11£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "≤Ω÷Ë11£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "≤Ω÷Ë11£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");
	}

	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_720P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë12 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "≤Ω÷Ë12£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "≤Ω÷Ë12£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
		CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "≤Ω÷Ë12£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");
	}

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë13 ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(0 == g_nUserData, "≤Ω÷Ë13£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eEvt, "≤Ω÷Ë13£¨ªÿµ˜ ¬º˛¿‡–Õ≤ª’˝»∑\n");
	CSTK_ASSERT_TRUE_FATAL(3 == g_eScreenDevice, "≤Ω÷Ë13£¨ªÿµ˜…Ë±∏¿‡–Õ≤ª’˝»∑\n");

	CSTK_FATAL_POINT
		
	//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√
	if (NotSupportHD())
	{
		if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "≤Ω÷Ë14 ß∞‹\n");
		}
	}
	else
	{
		if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
		{
			scResolution[0] = g_scResolution[0];
			scResolution[1] = g_scResolution[1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
		}
	} 

	if (CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
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
		CSTCPrint("[UDI2SCREENTEST]g_cCB_AUserDataªÿµ˜≤Œ ˝Œ™%s \r\n", g_cCB_AUserData);
	}

	if (0 == strcmp(pvUserData, cTemp_B))
	{
		strcpy(g_cCB_CUserData, pvUserData);
		CSTCPrint("[UDI2SCREENTEST]g_cCB_CUserDataªÿµ˜≤Œ ˝Œ™%s \r\n", g_cCB_CUserData);
	}
}

static void CB_B(CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice, void * pvUserData)
{
	strcpy(g_cCB_BUserData, pvUserData);
	CSTCPrint("[UDI2SCREENTEST]g_cCB_BUserDataªÿµ˜≤Œ ˝Œ™%s \r\n", g_cCB_BUserData);
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@CASEGROUP:CSUDISCREENAddCallback 
//@CASEGROUP:CSUDISCREENDelCallback
//@DESCRIPTION:≤‚ ‘◊¢≤·∂‡∏ˆªÿµ˜∫Ø ˝µƒ’˝»∑–‘
//@PRECONDITION: 
//@INPUT:1. ’˝≥£≤•∑≈“ª∏ˆ±Í«ÂPALΩ⁄ƒø‘¥,◊¢≤·∂‡∏ˆ ”∆µ∑÷±Ê¬ «–ªªªÿµ˜∫Ø ˝
//@EXPECTATION:  ”∆µ∑÷±Ê¬ ∑¢…˙’˝≥£«–ªª, ‰≥ˆŒﬁ“Ï≥£, ‰≥ˆŒﬁ“Ï≥£,≤¢«“ªÿµ˜∫Ø ˝π¶ƒ‹’˝≥£
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2. µ˜”√CSUDISCREENAddCallback◊¢≤·ªÿµ˜∫Ø ˝CB_A,ªÿµ˜≤Œ ˝CBP_A,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:3. µ˜”√CSUDISCREENAddCallback◊¢≤·ªÿµ˜∫Ø ˝CB_B,ªÿµ˜≤Œ ˝CBP_A,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:4. µ˜”√CSUDISCREENAddCallback◊¢≤·ªÿµ˜∫Ø ˝CB_A,ªÿµ˜≤Œ ˝CBP_B,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:5. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_NTSC,∆⁄Õ˚»˝¥Œ◊¢≤·µƒªÿµ˜∫Ø ˝æ˘ƒ‹±ªªÿµ˜,«“∑µªÿ≤Œ ˝∑÷±Œ™CBP_A,CBP_A,CBP_B
//@EXECUTIONFLOW:6. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_PAL,∆⁄Õ˚(CB_A,CBP_A) (CB_B,CBP_A)∫ÕCB_A,CBP_B)æ˘±ªªÿµ˜
//@EXECUTIONFLOW:7. µ˜”√CSUDISCREENDelCallback…æ≥˝(CB_A,CBP_A)µƒªÿµ˜∫Ø ˝
//@EXECUTIONFLOW:8. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_NTSC,∆⁄Õ˚(CB_B,CBP_A)∫ÕCB_A,CBP_B)±ªªÿµ˜(CB_A,CBP_A)≤ªƒ‹±ªªÿµ˜
//@EXECUTIONFLOW:9. µ˜”√CSUDISCREENDelCallback…æ≥˝(CB_B,CBP_A)µƒªÿµ˜∫Ø ˝
//@EXECUTIONFLOW:10. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_PAL,∆⁄Õ˚(CB_A,CBP_B)±ªªÿµ˜,(CB_B,CBP_A) (CB_A,CBP_A)≤ªƒ‹±ªªÿµ˜
//@EXECUTIONFLOW:11. µ˜”√CSUDISCREENDelCallback…æ≥˝(CB_A,CBP_B)µƒªÿµ˜∫Ø ˝
//@EXECUTIONFLOW:12. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_NTSC,∆⁄Õ˚(CB_A,CBP_B)(CB_B,CBP_A) (CB_A,CBP_A)æ˘≤ªƒ‹±ªªÿµ˜
//@EXECUTIONFLOW:13. …Ë÷√±Í«Â ”∆µ ‰≥ˆŒ™EM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:14. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
//@EXECUTIONFLOW:15. µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:16. µ˜”√CSUDIPLAYERCloseπÿ±’≤‚ ‘≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0013( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];
	char scCB_AUserData[] = "CBP_A";
	char scCB_BUserData[] = "CBP_B";
	
	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");
	CSUDIOSThreadSleep(500);
	
	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°±Í«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°±Í«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°∏ﬂ«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	}
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CB_A, (char *) scCB_AUserData), "≤Ω÷Ë2 ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CB_B, (char *) scCB_AUserData), "≤Ω÷Ë3 ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(CB_A, (char *) scCB_BUserData), "≤Ω÷Ë4 ß∞‹\n");

	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë5 ß∞‹\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 == strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 == strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 == strcmp(scCB_BUserData, g_cCB_CUserData)), "≤Ω÷Ë5 ß∞‹£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
	   
	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë6 ß∞‹\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 == strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 == strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 == strcmp(scCB_BUserData, g_cCB_CUserData)), "≤Ω÷Ë6 ß∞‹£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CB_A, scCB_AUserData), "≤Ω÷Ë7 ß∞‹\n");

	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë8 ß∞‹\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 != strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 == strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 == strcmp(scCB_BUserData, g_cCB_CUserData)), "≤Ω÷Ë8£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CB_B, scCB_AUserData), "≤Ω÷Ë9 ß∞‹\n");

	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë10 ß∞‹\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 != strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 != strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 == strcmp(scCB_BUserData, g_cCB_CUserData)), "≤Ω÷Ë10£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENDelCallback(CB_A, scCB_BUserData), "≤Ω÷Ë11 ß∞‹\n");

	InitGlobalParam();
	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë12 ß∞‹\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL((0 != strcmp(scCB_AUserData, g_cCB_AUserData)) && (0 != strcmp(scCB_AUserData, g_cCB_BUserData)) && (0 != strcmp(scCB_BUserData, g_cCB_CUserData)), "≤Ω÷Ë12£¨ªÿµ˜”√ªß ˝æ›≤ª’˝»∑\n");
   
	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0], 1), "≤Ω÷Ë13 ß∞‹\n");

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "≤Ω÷Ë14 ß∞‹\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
			}
		}

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetResolution 
//@DESCRIPTION:≤‚ ‘‘⁄±Í«Â…Ë±∏…œ…Ë÷√∏ﬂ«Â∑÷±Ê¬ “‘º∞‘⁄∏ﬂ«Â…Ë±∏…œ…Ë÷√±Í«Â∑÷±Ê¬ 
//@PRECONDITION: 
//@INPUT:1. ∂‘SD…Ë±∏…Ë÷√∏ﬂ«Â∑÷±Ê¬ 
//@INPUT:2. ∂‘HD…Ë±∏…Ë÷√±Í«Â∑÷±Ê¬ 
//@EXPECTATION: ‘⁄±Í«Â…Ë±∏…œ…Ë÷√∏ﬂ«Â∑÷±Ê¬ “‘º∞‘⁄∏ﬂ«Â…Ë±∏…œ…Ë÷√±Í«Â∑÷±Ê¬  ±∑µªÿ≤ª÷ß≥÷
//@EXECUTIONFLOW:1.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∫Õ∏ﬂ«Âµƒ∑÷±Ê¬ 
//@EXECUTIONFLOW:2.‘⁄±Í«Â…Ë±∏…œ…Ë÷√∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_720P,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:3.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ¥±‰ªØ
//@EXECUTIONFLOW:4.‘⁄±Í«Â…Ë±∏…œ…Ë÷√∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ¥±‰ªØ
//@EXECUTIONFLOW:6.‘⁄±Í«Â…Ë±∏…œ…Ë÷√∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_1080I,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:7.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ¥±‰ªØ
//@EXECUTIONFLOW:8.‘⁄±Í«Â…Ë±∏…œ…Ë÷√∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ¥±‰ªØ
//@EXECUTIONFLOW:10.‘⁄±Í«Â…Ë±∏…œ…Ë÷√∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_1080P,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:11.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ¥±‰ªØ
//@EXECUTIONFLOW:12.‘⁄±Í«Â…Ë±∏…œ…Ë÷√∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_1080P_50HZ,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:13.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ¥±‰ªØ
//@EXECUTIONFLOW:14.‘⁄∏ﬂ«Â…Ë±∏…œ…Ë÷√∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_NTSC,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:15.µ˜”√CSUDISCREENGetResolutionªÒ»°∏ﬂ«Â∑÷±Ê¬ ,∆⁄Õ˚Œ¥±‰ªØ
//@EXECUTIONFLOW:16.‘⁄∏ﬂ«Â…Ë±∏…œ…Ë÷√∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_PAL,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:17.µ˜”√CSUDISCREENGetResolutionªÒ»°∏ﬂ«Â∑÷±Ê¬ ,∆⁄Õ˚Œ¥±‰ªØ
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetResolution_0014( void )
{
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E  scResolution[2];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 ,EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	do
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");

		if (!NotSupportHD())
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
		}

		scResolution[0] = EM_UDISCREEN_RESOLUTION_720P;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë2 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "≤Ω÷Ë3 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "≤Ω÷Ë3 ß∞‹\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë4 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "≤Ω÷Ë5 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "≤Ω÷Ë5 ß∞‹\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_1080I;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë6 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "≤Ω÷Ë7 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "≤Ω÷Ë7 ß∞‹\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë8 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "≤Ω÷Ë9 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "≤Ω÷Ë9 ß∞‹\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_1080P;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë10 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "≤Ω÷Ë11 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "≤Ω÷Ë11 ß∞‹\n");

		scResolution[0]  = EM_UDISCREEN_RESOLUTION_1080P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë12 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0] ), "≤Ω÷Ë13 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[0] == scResolution[0], "≤Ω÷Ë13 ß∞‹\n");
	} while(0); 

	if (!NotSupportHD())
	{
		scResolution[1]  = EM_UDISCREEN_RESOLUTION_NTSC;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1] , 1), "≤Ω÷Ë14 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &scResolution[1] ), "≤Ω÷Ë15 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[1] == scResolution[1], "≤Ω÷Ë15 ß∞‹\n");

		scResolution[1]  = EM_UDISCREEN_RESOLUTION_PAL;
		CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1] , 1), "≤Ω÷Ë16 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &scResolution[1] ), "≤Ω÷Ë17 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(g_scResolution[1] == scResolution[1], "≤Ω÷Ë17 ß∞‹\n");
	}

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ 
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "≤Ω÷Ë14 ß∞‹\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
			}
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENGetResolution 
//@DESCRIPTION:≤‚ ‘ªÒ»°÷∏’ÎŒ™NULLµƒ«Èøˆ
//@PRECONDITION:
//@INPUT:1. eScreenDevice = EM_UDI_VOUT_DEVICE_SD,peResolution = NULL
//@INPUT:2. eScreenDevice = EM_UDI_VOUT_DEVICE_HD+!,peResolution = &sResolution
//@INPUT:2. eScreenDevice = EM_UDI_VOUT_DEVICE_SD-1,peResolution = &sResolution
//@EXPECTATION: ∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.µ˜”√CSUDISCREENGetResolution,¥´»Î¥ÌŒÛ≤Œ ˝,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_GetResolution_0001( void )
{
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	CSUDISCREENResolution_E sResolution;

	nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "≤Œ ˝¥ÌŒÛºÏ≤‚ ß∞‹1");

	nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD -1, &sResolution);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "≤Œ ˝¥ÌŒÛºÏ≤‚ ß∞‹2");

	nResult = CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD +1, &sResolution);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "≤Œ ˝¥ÌŒÛºÏ≤‚ ß∞‹3");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENGetResolution 
//@DESCRIPTION:≤‚ ‘∑÷±Ê¬ ◊™ªª∫Û «∑Òƒ‹πªªÒ»°µΩ’˝»∑µƒµ±«∞∑÷±Ê¬ 
//@PRECONDITION:
//@INPUT:1. ∑÷±∂‘∑÷±Ê¬ Ω¯––∏˜÷÷«–ªª
//@INPUT:2. CSUDISCREENGetResolutionµ˜”√æ˘¥´»Î∫œ∑®≤Œ ˝
//@EXPECTATION: ∑÷±Ê¬ ≤˙…˙±‰ªØ∫ÛÕ®π˝CSUDISCREENGetResolutionƒ‹πªªÒ»°µΩµ±«∞’˝»∑µƒ∑÷±Ê¬ 
//@EXECUTIONFLOW:1.…Ë÷√±Í«Â∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_PAL,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ™EM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:3.…Ë÷√±Í«Â∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_NTSC,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:4.µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ™EM_UDISCREEN_RESOLUTION_NTSC
//@EXECUTIONFLOW:5.»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â∑÷±Ê¬ ,‘Ú…Ë÷√∏ﬂ«Â…Ë±∏∑÷±Ê¬ Œ™EM_UDISCREEN_RESOLUTION_1080I_50HZ,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:6.»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â∑÷±Ê¬ ,µ˜”√CSUDISCREENGetResolutionªÒ»°∏ﬂ«Â∑÷±Ê¬ ,∆⁄Õ˚Œ™EM_UDISCREEN_RESOLUTION_1080I_50HZ
//@EXECUTIONFLOW:7.»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â∑÷±Ê¬ ,‘ÚÕ¨ ±…Ë÷√±Í«ÂŒ™EM_UDISCREEN_RESOLUTION_PAL,∏ﬂ«ÂŒ™EM_UDISCREEN_RESOLUTION_720P_50HZ,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:8.»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â∑÷±Ê¬ ,µ˜”√CSUDISCREENGetResolutionªÒ»°±Í«Â∑÷±Ê¬ ,∆⁄Õ˚Œ™EM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:9.»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â∑÷±Ê¬ ,µ˜”√CSUDISCREENGetResolutionªÒ»°∏ﬂ«Â∑÷±Ê¬ ,∆⁄Õ˚Œ™EM_UDISCREEN_RESOLUTION_720P_50HZ
//@EXECUTIONFLOW:10. ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√,∏√∑÷±ÊŒ™∏√ƒ£øÈ≤‚ ‘≥ı ºªØ ±ªÒµ√µƒ∏ﬂ±Í«Â∑÷±Ê¬ 
CSUDI_BOOL CSTC_SCREEN_TEST_IT_GetResolution_0002( void )
{
	CSUDISCREENType_E scScreenDevice[2] = {EM_UDI_VOUT_DEVICE_SD, EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E scResolution[2];
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL );  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");
	CSTCPrint(" ”∆µΩ⁄ƒø≤•∑≈ª≠√Ê «∑Ò’˝≥£?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "≤Ω÷Ë1 ß∞‹\n");

	if (NotSupportHD())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°±Í«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &g_scResolution[0]), "ªÒ»°±Í«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[1], &g_scResolution[1]), "ªÒ»°∏ﬂ«Â≤‚ ‘«∞∑÷±Ê¬  ß∞‹\n");
	}

	scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë1 ß∞‹\n"); 
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0]), "≤Ω÷Ë2,ªÒ»°∑÷±Ê¬ \n");  
	CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_PAL == scResolution[0], "≤Ω÷Ë2 ß∞‹,ªÒ»°µƒ∑÷±Ê¬ ”Î…Ë÷√µƒ≤ª“ª—˘\n");

	scResolution[0] = EM_UDISCREEN_RESOLUTION_NTSC;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &scResolution[0] , 1), "≤Ω÷Ë3 ß∞‹\n");
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(scScreenDevice[0], &scResolution[0]), "≤Ω÷Ë4,ªÒ»°∑÷±Ê¬ \n"); 
	CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_NTSC == scResolution[0], "≤Ω÷Ë4 ß∞‹,ªÒ»°µƒ∑÷±Ê¬ ”Î…Ë÷√µƒ≤ª“ª—˘\n");

	//»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â∑÷±Ê¬ 
	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_1080I_50HZ ))
	{    
		scResolution[1] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[1], &scResolution[1], 2), "≤Ω÷Ë5 ß∞‹\n");
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &scResolution[1] ), "≤Ω÷Ë6 ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_1080I_50HZ == scResolution[1], "≤Ω÷Ë6 ß∞‹,ªÒ»°µƒ∑÷±Ê¬ ”Î…Ë÷√µƒ≤ª“ª—˘\n")
	} 

	if (IsScreenSupport(scScreenDevice, EM_UDISCREEN_RESOLUTION_720P_50HZ ))
	{
		scResolution[0] = EM_UDISCREEN_RESOLUTION_PAL;
		scResolution[1] = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë7 ß∞‹\n"); 
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD, &scResolution[0] ), "≤Ω÷Ë8£¨ªÒ»°∑÷±Ê¬  ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_PAL == scResolution[0], "≤Ω÷Ë8 ß∞‹,ªÒ»°µƒ∑÷±Ê¬ ”Î…Ë÷√µƒ≤ª“ª—˘\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &scResolution[1] ), "≤Ω÷Ë9£¨ªÒ»°∑÷±Ê¬  ß∞‹\n");
		CSTK_ASSERT_TRUE_FATAL(EM_UDISCREEN_RESOLUTION_720P_50HZ == scResolution[1], "≤Ω÷Ë9 ß∞‹,ªÒ»°µƒ∑÷±Ê¬ ”Î…Ë÷√µƒ≤ª“ª—˘\n");
	}

	CSTK_FATAL_POINT
	{
		//ª÷∏¥≤‚ ‘«∞∑÷±Ê¬ …Ë÷√
		if (NotSupportHD())
		{
			if (g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&scScreenDevice[0], &g_scResolution[0], 1), "≤Ω÷Ë14 ß∞‹\n");
			}
		}
		else
		{
			if ((g_scResolution[0]>EM_UDISCREEN_RESOLUTION_INVALID && g_scResolution[0]<=EM_UDISCREEN_RESOLUTION_576P)
				&& (g_scResolution[1]>=EM_UDISCREEN_RESOLUTION_720P && g_scResolution[0]<EM_UDISCREEN_RESOLUTION_NUM))
			{
				scResolution[0] = g_scResolution[0];
				scResolution[1] = g_scResolution[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(scScreenDevice, scResolution, 2), "≤Ω÷Ë14 ß∞‹\n");
			}
		}
	
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENGetCapability 
//@DESCRIPTION:≤‚ ‘ªÒ»°÷∏’ÎŒ™NULLµƒ«Èøˆ
//@PRECONDITION:
//@INPUT:1. eScreenDevice = EM_UDI_VOUT_DEVICE_HD,psCapabilityInfo = NULL
//@INPUT:2. eScreenDevice = EM_UDI_VOUT_DEVICE_HD+1,psCapabilityInfo = &sCapability
//@INPUT:3. eScreenDevice = EM_UDI_VOUT_DEVICE_SD-1,psCapabilityInfo = &sCapability
//@EXPECTATION: ∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.µ˜”√CSUDISCREENGetCapability,¥´»Î¥ÌŒÛ≤Œ ˝£¨∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_SCREEN_TEST_IT_GetCapability_0001( void )
{
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	CSUDISCREENCapability_S sCapability;

	nResult = CSUDISCREENGetCapability(EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "≤Œ ˝¥ÌŒÛºÏ≤‚ ß∞‹1");

	nResult = CSUDISCREENGetCapability(EM_UDI_VOUT_DEVICE_HD+1, &sCapability);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "≤Œ ˝¥ÌŒÛºÏ≤‚ ß∞‹2");

	nResult = CSUDISCREENGetCapability(EM_UDI_VOUT_DEVICE_SD-1, &sCapability);
	CSTK_ASSERT_TRUE_FATAL((CSUDISCREEN_ERROR_BAD_PARAMETER == nResult), "≤Œ ˝¥ÌŒÛºÏ≤‚ ß∞‹3");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDISCREENGetCapability 
//@DESCRIPTION:≤‚ ‘œ‡”¶µƒÕ®µ¿”¶∏√ªÒ»°œ‡”¶µƒ∑÷±Ê¬ 
//@INPUT:1. eScreenDevice = EM_UDI_VOUT_DEVICE_SD,psCapabilityInfo = ∫œ∑®÷∏’Î
//@INPUT:2. eScreenDevice = EM_UDI_VOUT_DEVICE_HD,psCapabilityInfo = ∫œ∑®÷∏’Î(‘⁄÷ß≥÷HDµƒ«Èøˆœ¬)
//@EXPECTATION: 1.∑µªÿCSUDI_SUCCESS
//@EXPECTATION: 2.±Í«Â…Ë±∏≤ª”¶∏√”–∏ﬂ«Â∑÷±Ê¬ ÷ß≥÷£¨∏ﬂ«Â…Ë±∏≤ª”¶∏√∑µªÿ±Í«Â∑÷±Ê¬ 
//@EXPECTATION: 3.±Í«Â÷¡…Ÿ÷ß≥÷EM_UDISCREEN_RESOLUTION_NTSC∫ÕEM_UDISCREEN_RESOLUTION_PAL
//@EXPECTATION: 4.∏ﬂ«Â÷¡…Ÿ÷ß≥÷EM_UDISCREEN_RESOLUTION_720P_50HZ,EM_UDISCREEN_RESOLUTION_720P,EM_UDISCREEN_RESOLUTION_1080I_50HZ,EM_UDISCREEN_RESOLUTION_1080I
//@EXECUTIONFLOW:1.µ˜”√CSUDISCREENGetCapabilityªÒ»°±Í«Â…Ë±∏÷ß≥÷µƒ∑÷±Ê¬ 
//@EXECUTIONFLOW:2.±È¿˙±Í«Â…Ë±∏÷ß≥÷µƒ∑÷±Ê¬ ,∆‰À˘÷ß≥÷µƒ∑÷±Ê¬ ÷–≤ª”¶∏√”–∏ﬂ«Âµƒ∑÷±Ê¬ 
//@EXECUTIONFLOW:3.ºÏ≤È±Í«Â÷¡…Ÿ÷ß≥÷EM_UDISCREEN_RESOLUTION_NTSC∫ÕEM_UDISCREEN_RESOLUTION_PAL
//@EXECUTIONFLOW:4.±È¿˙∏ﬂ«Â…Ë±∏÷ß≥÷µƒ∑÷±Ê¬ ,∆‰À˘÷ß≥÷µƒ∑÷±Ê¬ ÷–≤ª”¶∏√”–±Í«Âµƒ∑÷±Ê¬ (‘⁄∆ΩÃ®÷ß≥÷HDµƒÃıº˛œ¬)
//@EXECUTIONFLOW:5.ºÏ≤È∏ﬂ«Â÷¡…Ÿ÷ß≥÷EM_UDISCREEN_RESOLUTION_720P_50HZ,EM_UDISCREEN_RESOLUTION_720P,EM_UDISCREEN_RESOLUTION_1080I_50HZ,EM_UDISCREEN_RESOLUTION_1080I
CSUDI_BOOL CSTC_SCREEN_TEST_IT_GetCapability_0002( void )
{
	CSUDISCREENResolution_E  eResolution = EM_UDISCREEN_RESOLUTION_720P;
	CSUDISCREENType_E eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	for (eResolution = EM_UDISCREEN_RESOLUTION_720P; eResolution<EM_UDISCREEN_RESOLUTION_NUM; eResolution++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == IsScreenSupport(&eScreenDevice, eResolution ), "Step2 ß∞‹");
	} 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IsScreenSupport(&eScreenDevice, EM_UDISCREEN_RESOLUTION_PAL), "Step3 failure:SD must support PAL\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IsScreenSupport(&eScreenDevice, EM_UDISCREEN_RESOLUTION_NTSC), "Step3failure:SD must support NTSC\n");

	if(!NotSupportHD())
	{
		eScreenDevice = EM_UDI_VOUT_DEVICE_HD;

		for (eResolution = EM_UDISCREEN_RESOLUTION_NTSC; eResolution<EM_UDISCREEN_RESOLUTION_480P; eResolution++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == IsScreenSupport(&eScreenDevice, eResolution ), "Step3 ß∞‹");
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
//@DESCRIPTION:≤‚ ‘¡¡∂»≤Œ ˝∑÷±Œ™-1∫Õ101∑«∑®÷µµƒ«Èøˆ
//@PRECONDITION:Ω⁄ƒø≤•∑≈’˝≥£
//@INPUT:1. nBrightness = -1
//@INPUT:2. nBrightness = 101
//@EXPECTATION:1.∑µªÿ¥ÌŒÛ¬ÎCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENSetBrightness…Ë÷√¡¡∂»Œ™-1,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3.µ˜”√CSUDISCREENSetBrightness…Ë÷√¡¡∂»Œ™101,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4.µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:5.µ˜”√CSUDIPLAYERCloseπÿ±’≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetBrightness_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetBrightness(-1), "≤Œ ˝ºÏ≤‚ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetBrightness(101), "≤Œ ˝ºÏ≤‚ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDISCREENSetContrast 
//@DESCRIPTION:≤‚ ‘¡¡∂»¥”0~100∑∂Œß√øº∂20 µ›‘ˆ±‰ªØµƒ«Èøˆ
//@PRECONDITION:Ω⁄ƒø≤•∑≈’˝≥£
//@INPUT:1. nBrightness = [0-100]
//@EXPECTATION:1.√ø¥Œ…Ë÷√∑µªÿCSUDI_SUCCESS,«“∆¡ƒª±‰ªØ∫Õ…Ë÷√÷µœ‡∑˚
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENSetContrast…Ë÷√¡¡∂»Œ™0
//@EXECUTIONFLOW:3.¡¡∂»÷µµ›‘ˆ20≤¢µ˜”√CSUDISCREENSetBrightness…Ë÷√÷±µΩ¥ÔµΩ◊Ó¥Û÷µ100
//@EXECUTIONFLOW:4.≤‚ ‘ÕÍ≥…ª÷∏¥≤‚ ‘œ÷≥°,Ω´¡¡∂»…Ë÷√Œ™50
//@EXECUTIONFLOW:5.µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:6.µ˜”√CSUDIPLAYERCloseπÿ±’≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetBrightness_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int i = 0;
	
	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("“‘œ¬√ø2√Î…Ë÷√“ª¥Œ∆¡ƒª¡¡∂»£¨ π∑¢…˙µ›‘ˆ±‰ªØ£¨«Î◊¢“‚π€≤Ï∆¡ƒª¡¡∂»£¨∞¥»Œ“‚º¸ø™ º≤‚ ‘\r\n");
	CSTKWaitAnyKey();

	for(i=0; i<=100; i+=20)
	{
		CSTCPrint("…Ë÷√∆¡ƒª¡¡∂»Œ™%d\n", i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetBrightness(i), "…Ë÷√¡¡∂» ß∞‹\n");
		CSUDIOSThreadSleep(2*1000);
	}

	CSTCPrint("∆¡ƒª¡¡∂»∑¢…˙µ›‘ˆ±‰ªØ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "…Ë÷√∆¡ƒª¡¡∂»∫Û–ßπ˚≤ª’˝»∑");

	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetBrightness(50), "…Ë÷√¡¡∂» ß∞‹\n");

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetContrast 
//@DESCRIPTION:≤‚ ‘∂‘±»∂»≤Œ ˝∑÷±Œ™-1∫Õ101∑«∑®÷µµƒ«Èøˆ
//@PRECONDITION:Ω⁄ƒø≤•∑≈’˝≥£
//@INPUT:1. Contrast = -1
//@INPUT:2. Contrast = 101
//@EXPECTATION:1.∑µªÿ¥ÌŒÛ¬ÎCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENSetContrast…Ë÷√∂‘±»∂»Œ™-1,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3.µ˜”√CSUDISCREENSetContrast…Ë÷√∂‘±»∂»Œ™101,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4.µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:5.µ˜”√CSUDIPLAYERCloseπÿ±’≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetContrast_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetContrast(-1), "≤Œ ˝ºÏ≤‚ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetContrast(101), "≤Œ ˝ºÏ≤‚ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetBrightness 
//@DESCRIPTION:≤‚ ‘∂‘±»∂»¥”0~100∑∂Œß√øº∂20 µ›‘ˆ±‰ªØµƒ«Èøˆ
//@PRECONDITION:Ω⁄ƒø≤•∑≈’˝≥£
//@INPUT:1. Contrast = [0-100]
//@EXPECTATION:1.√ø¥Œ…Ë÷√∑µªÿCSUDI_SUCCESS,«“∆¡ƒª±‰ªØ∫Õ…Ë÷√÷µœ‡∑˚
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENSetBrightness…Ë÷√∂‘±»∂»Œ™0
//@EXECUTIONFLOW:3.∂‘±»∂»÷µµ›‘ˆ20≤¢µ˜”√CSUDISCREENSetContrast…Ë÷√÷±µΩ¥ÔµΩ◊Ó¥Û÷µ100
//@EXECUTIONFLOW:4.≤‚ ‘ÕÍ≥…ª÷∏¥≤‚ ‘œ÷≥°,Ω´∂‘±»∂»…Ë÷√Œ™50
//@EXECUTIONFLOW:5.µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:6. µ˜”√CSUDIPLAYERCloseπÿ±’≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetContrast_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int i = 0;
	
	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("“‘œ¬√ø2√Î…Ë÷√“ª¥Œ∆¡ƒª∂‘±»∂»£¨ π∑¢…˙µ›‘ˆ±‰ªØ£¨«Î◊¢“‚π€≤Ï∆¡ƒª∂‘±»∂»£¨∞¥»Œ“‚º¸ø™ º≤‚ ‘\r\n");
	CSTKWaitAnyKey();
		
	for(i=0; i<=100; i+=20)
	{
		CSTCPrint("…Ë÷√∆¡ƒª∂‘±»∂»Œ™%d\n", i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetContrast(i), "…Ë÷√∂‘±»∂» ß∞‹\n");
		CSUDIOSThreadSleep(2*1000);
	}

	CSTCPrint("∆¡ƒª∂‘±»∂»∑¢…˙µ›‘ˆ±‰ªØ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "…Ë÷√∆¡ƒª∂‘±»∂»∫Û–ßπ˚≤ª’˝»∑");

	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetContrast(50), "…Ë÷√∂‘±»∂» ß∞‹\n");
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetSaturation 
//@DESCRIPTION:≤‚ ‘±•∫Õ∂»≤Œ ˝∑÷±Œ™-1∫Õ101∑«∑®÷µµƒ«Èøˆ
//@PRECONDITION:Ω⁄ƒø≤•∑≈’˝≥£
//@INPUT:1. Saturation = -1
//@INPUT:2. Saturation = 101
//@EXPECTATION:1.∑µªÿ¥ÌŒÛ¬ÎCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENSetSaturation…Ë÷√±•∫Õ∂»Œ™-1,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3.µ˜”√CSUDISCREENSetSaturation…Ë÷√±•∫Õ∂»Œ™101,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4.µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:5. µ˜”√CSUDIPLAYERCloseπÿ±’≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetSaturation_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2 , EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetSaturation(-1), "≤Œ ˝ºÏ≤‚ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetSaturation(101), "≤Œ ˝ºÏ≤‚ ß∞‹\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISCREENSetSaturation 
//@DESCRIPTION:≤‚ ‘±•∫Õ∂»¥”0~100∑∂Œß√øº∂20 µ›‘ˆ±‰ªØµƒ«Èøˆ
//@PRECONDITION:Ω⁄ƒø≤•∑≈’˝≥£
//@INPUT:1. Saturation = [0-100]
//@EXPECTATION:1.√ø¥Œ…Ë÷√∑µªÿCSUDI_SUCCESS,«“∆¡ƒª±‰ªØ∫Õ…Ë÷√÷µœ‡∑˚
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENSetSaturation…Ë÷√±•∫Õ∂»Œ™0
//@EXECUTIONFLOW:3.±•∫Õ∂»÷µµ›‘ˆ20≤¢µ˜”√CSUDISCREENSetSaturation…Ë÷√÷±µΩ¥ÔµΩ◊Ó¥Û÷µ100
//@EXECUTIONFLOW:4.≤‚ ‘ÕÍ≥…ª÷∏¥≤‚ ‘œ÷≥°,Ω´±•∫Õ∂»…Ë÷√Œ™50
//@EXECUTIONFLOW:5.µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:6. µ˜”√CSUDIPLAYERCloseπÿ±’≤•∑≈∆˜
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetSaturation_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int i = 0;
	
	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹\n");

	CSTCPrint("“‘œ¬√ø2√Î…Ë÷√“ª¥Œ∆¡ƒª±•∫Õ∂»£¨ π∑¢…˙µ›‘ˆ±‰ªØ£¨«Î◊¢“‚π€≤Ï∆¡ƒª±•∫Õ∂»£¨∞¥»Œ“‚º¸ø™ º≤‚ ‘\r\n");
	CSTKWaitAnyKey();

	for(i=0; i<=100; i+=20)
	{
		CSTCPrint("…Ë÷√∆¡ƒª±•∫Õ∂»Œ™%d\n", i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetSaturation(i), "…Ë÷√±•∫Õ∂» ß∞‹\n");
		CSUDIOSThreadSleep(2*1000);
	}

	CSTCPrint("∆¡ƒª±•∫Õ∂»∑¢…˙µ›‘ˆ±‰ªØ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "…Ë÷√∆¡ƒª±•∫Õ∂»∫Û–ßπ˚≤ª’˝»∑");

	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetSaturation(50), "…Ë÷√±•∫Õ∂» ß∞‹\n");
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}


//@CASEGROUP:SetOSDTransparency 
//@DESCRIPTION:≤‚ ‘Õ∏√˜∂»≤Œ ˝∑÷±Œ™-1∫Õ101∑«∑®÷µµƒ«Èøˆ“‘º∞0∫Õ100µƒ±ﬂΩÁ«Èøˆ
//@INPUT:1. Saturation = -1,Saturation = 101
//@INPUT:2. Saturation = 0,Saturation = 100
//@EXPECTATION:1.Saturation = -1,Saturation = 101,∑µªÿ¥ÌŒÛ¬ÎCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXPECTATION:1.Saturation = 0,Saturation = 100,∑µªÿ¥ÌŒÛ¬ÎCSUDI_SUCCESS
//@EXECUTIONFLOW:1.µ˜”√CSUDISCREENSetSaturation…Ë÷√Õ∏√˜∂»Œ™-1,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2.µ˜”√CSUDISCREENSetSaturation…Ë÷√Õ∏√˜∂»Œ™101,∆⁄Õ˚∑µªÿCSUDISCREEN_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3.µ˜”√CSUDISCREENSetSaturation…Ë÷√Õ∏√˜∂»Œ™0,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:4.µ˜”√CSUDISCREENSetSaturation…Ë÷√Õ∏√˜∂»Œ™100,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
CSUDI_BOOL CSTC_SCREEN_TEST_IT_SetOSDTransparency_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER == CSUDISCREENSetOSDTransparency(-1), "step 1 ¥ÌŒÛ≤Œ ˝ºÏ≤È ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDISCREEN_ERROR_BAD_PARAMETER ==CSUDISCREENSetOSDTransparency(101), "step 2 ¥ÌŒÛ≤Œ ˝ºÏ≤È ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(0), "step 3 ±ﬂΩÁ≤Œ ˝≤‚ ‘ ß∞‹\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDISCREENSetOSDTransparency(100), "step 4 ±ﬂΩÁ≤Œ ˝≤‚ ‘ ß∞‹\n");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:SetOSDTransparency 
//@DESCRIPTION:≤‚ ‘‘⁄√ª”–video±≥æ∞µƒ«Èøˆ…Ë÷√OSGÕ∏√˜∂»“≤ƒ‹∆–ßπ˚
//@PRECONDITION:∆ΩÃ®OSGƒ£øÈπ¶ƒ‹’˝≥£
//@INPUT:1. Saturation = [0-100]
//@EXPECTATION:1.√ø¥Œ…Ë÷√∑µªÿCSUDI_SUCCESS
//@EXPECTATION:2. ∆¡ƒªOSG±‰ªØ”ÎÕ∏√˜∂»…Ë÷√œ‡∑˚
//@EXECUTIONFLOW:1.∞¥»Áœ¬≤Ω÷Ë‘⁄∏ﬂ±Í«Âœ‘¥Ê…œª≠≤‚ ‘ÕºœÒ(»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â≤≈∞¸∫¨∏ﬂ«Â):
//@EXECUTIONFLOW:1.1 µ˜”√CSUDIOSGGetDisplaySurfaceªÒ»°œ‘¥Êæ‰±˙,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:1.2 µ˜”√CSUDIOSGFill‘⁄{260,188}Œª÷√ÃÓ≥‰{200,200}µƒ∫Ï…´OSG,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:2.µ˜”√CSUDIOSGSyncÕ¨≤Ω ˝æ›,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:3.…Ë÷√OSGÕ∏√˜∂»,¥”[0,100],√øº∂µ›‘ˆ20£¨√ø¥Œ…Ë÷√∫Ûsleep 1.5s
//@EXECUTIONFLOW:4.µ»¥˝”√ªß≈–∂®≤‚ ‘Ω·π˚
//@EXECUTIONFLOW:5.µ˜”√CSUDISCREENSetOSDTransparencyª÷∏¥OSGµƒÕ∏√˜∂»Œ™≤ªÕ∏√˜,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:6.µ˜”√CSUDIOSGFillª÷∏¥∏ﬂ±Í«ÂµƒOSG œ‘¥ÊŒ™÷µ0x0,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplaySurfaceSD, &sSurfaceInfoSD)), "ªÒ»°¥Ê–≈œ¢ ß∞‹");
	}
	
	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hDisplaySurfaceHD)),
			"get display surface failure");

		memset(&sSurfaceInfoHD, 0, sizeof(sSurfaceInfoHD));
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplaySurfaceHD, &sSurfaceInfoHD)), "ªÒ»°¥Ê–≈œ¢ ß∞‹");
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
	CSTCPrint("∆¡ƒª…œ(%d,%d)Œª÷√ø…“‘ø¥º˚∫Ï…´øÌ∏ﬂŒ™(%d,%d)µƒæÿ–ŒøÚ,∞¸¿®∏ﬂ«Â(»Áπ˚÷ß≥÷∏ﬂ«Âµƒª∞)\n",
	stRect.m_nX,stRect.m_nY,stRect.m_nWidth,stRect.m_nHeight);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTCPrint("«Î◊¢“‚OSGµƒ±‰ªØ,OSGµƒÕ∏√˜∂»Ω´”…0±‰ªØµΩ100£¨√øº∂20,»Œ“‚º¸ºÃ–¯\n");
	CSTKWaitAnyKey();

	for(i=0;i<=100;i+=20)
	{
		CSTCPrint("…Ë÷√OSGÕ∏√˜∂»µΩ:%d\n",i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(i), "¥ÌŒÛ≤Œ ˝ºÏ≤È ß∞‹\n");
		CSUDIOSThreadSleep(1500);
	}

	CSTCPrint("∆¡ƒªOSG¥”ÕÍ»´Õ∏√˜÷Ω•±‰≥…ÕÍ»´«ÂŒ˙?? (Y/N)\n");
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
//@DESCRIPTION:≤‚ ‘‘⁄≤•∑≈videoµƒ«Èøˆœ¬,OSGÕ∏√˜∂»¥”0~100∑∂Œß√øº∂20 µ›‘ˆ±‰ªØµƒ«Èøˆ
//@PRECONDITION:Ω⁄ƒø≤•∑≈’˝≥£
//@INPUT:1. Saturation = [0-100]
//@EXPECTATION:1.√ø¥Œ…Ë÷√∑µªÿCSUDI_SUCCESS
//@EXPECTATION:2. ∆¡ƒª±‰ªØ∫Õ…Ë÷√÷µœ‡∑˚,∞¸¿®video≤ª ‹OSGÕ∏√˜∂»…Ë÷√±‰ªØ;OSGÕ∏√˜∂»±‰ªØ’˝≥£
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2.µ˜”√CSUDIOSGGetDisplaySurfaceªÒ»°±Í«Âœ‘¥Êæ‰±˙
//@EXECUTIONFLOW:3.µ˜”√CSUDIOSGFillΩ´±Í«Âœ‘¥ÊÃÓ≥‰≥…¿∂…´0xFF0000FF,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:4.µ˜”√CSUDIOSGFillΩ´±Í«Âœ‘¥Ê◊¯±Í{200,200}¥¶ÃÓ≥‰{250,250}≥§µƒÕ∏√˜…´0x0,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:5.µ˜”√CSUDIVIDEOSetWindowSize…Ë÷√±Í«Âvideo¥∞ø⁄¥Û–°Œ™OSGÕ∏√˜«¯”Ú¥Û–°,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:6.»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â‘Ú÷ÿ∏¥≤Ω÷Ë2~5,‘⁄∏ﬂ«Â…œ◊ˆœ‡Õ¨µƒª≠Õº∫Õ…Ë÷√£¨√ø∏ˆ≤Ω÷Ë∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:7.…Ë÷√OSGÕ∏√˜∂»,¥”[0,100],√øº∂µ›‘ˆ20£¨√ø¥Œ…Ë÷√∫Ûsleep 1.5s
//@EXECUTIONFLOW:8.µ»¥˝”√ªß≈–∂®≤‚ ‘Ω·π˚
//@EXECUTIONFLOW:9.µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:10. µ˜”√CSUDIPLAYERCloseπÿ±’≤•∑≈∆˜,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:11.µ˜”√CSUDIVIDEOSetWindowSizeª÷∏¥∏ﬂ±Í«Âµƒ ”∆µ¥∞ø⁄Œ™»´∆¡,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:12.µ˜”√CSUDISCREENSetOSDTransparencyª÷∏¥OSGµƒÕ∏√˜∂»Œ™≤ªÕ∏√˜,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:12.µ˜”√CSUDIOSGFillª÷∏¥∏ﬂ±Í«ÂµƒOSG œ‘¥ÊŒ™÷µ0x0,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
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

	hPlayer = PlaySDProgram( EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹");
	
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
	
	CSTCPrint("∆¡ƒª{200,200,250,250}Œª÷√Œ™–°¥∞ø⁄ ”∆µ,∆‰”‡Œ™¿∂…´OSG(◊¢“‚:  ”∆µ‘¥±æ…Ì◊Û”“…‘”–∫⁄±ﬂ)£?(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");
	
	CSTCPrint("«Î◊¢“‚π€≤Ï∆¡ƒªOSGÕ∏√˜∂»±‰ªØ\n");
	CSTKWaitAnyKey();

	for(i = 0; i <= 100; i += 20)
	{
		CSTCPrint("…Ë÷√OSGÕ∏√˜∂»µΩ:%d\n",i);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(i), "…Ë÷√Õ∏√˜∂» ß∞‹");
		CSUDIOSThreadSleep(1500);
	}

	CSTCPrint("∆¡ƒªOSG¥”ÕÍ»´Õ∏√˜÷Ω•±‰≥…ÕÍ»´«ÂŒ˙,«“video≤ª ‹”∞œÏ?? (Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "Õ£÷π≤•∑≈Ω⁄ƒø ß∞‹");
	
	CSTK_FATAL_POINT
	 
	if (CSUDI_NULL != hPlayer)
	{
		 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹");
		 hPlayer = CSUDI_NULL;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,NULL),
		"fail to set SD video to full screen");

	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,NULL),
			"fail to set HD video to full screen");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(100), "…Ë÷√Õ∏√˜∂» ß∞‹\n");

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
//@DESCRIPTION:≤‚ ‘∑÷±Ê¬ µƒ«–ªª≤ªª·‘Ï≥…OSGÕ∏√˜∂»µƒ∂™ ß
//@PRECONDITION:Ω⁄ƒø≤•∑≈’˝≥£
//@INPUT:1. Saturation = [0-100]
//@EXPECTATION:1.√ø¥Œ…Ë÷√∑µªÿCSUDI_SUCCESS,«“∆¡ƒª±‰ªØ∫Õ…Ë÷√÷µœ‡∑˚
//@EXECUTIONFLOW:1.µ˜”√playerƒ£øÈΩ”ø⁄≤•∑≈±Í«ÂPAL¬Î¡˜,œÍœ∏≤Ω÷Ë«Î≤Œøº≤‚ ‘”√¿˝CSTC_SCREEN_TEST_IT_SetResolution_0003≤Ω÷Ë1
//@EXECUTIONFLOW:2.∞¥»Áœ¬≤Ω÷Ë‘⁄∏ﬂ±Í«Âœ‘¥Ê…œª≠≤‚ ‘ÕºœÒ(»Áπ˚∆ΩÃ®÷ß≥÷∏ﬂ«Â≤≈∞¸∫¨∏ﬂ«Â):
//@EXECUTIONFLOW:2.1 µ˜”√CSUDIOSGGetDisplaySurfaceªÒ»°œ‘¥Êæ‰±˙,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:2.2 µ˜”√CSUDIOSGFill‘⁄{50,50}Œª÷√ÃÓ≥‰{350,350}µƒ∫Ï…´OSG,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:2.3 µ˜”√CSUDIVIDEOSetWindowSize…Ë÷√ ”∆µ ‰≥ˆ–°¥∞ø⁄ƒ£ Ω,∆⁄Õ˚∑µªÿCSUDI_SUCCESS
//@EXECUTIONFLOW:3.µ˜”√CSUDISCREENSetOSDTransparency…Ë÷√OSGµƒÕ∏√˜∂»Œ™40,∆⁄Õ˚OSGŒ™∞ÎÕ∏√˜–ßπ˚.
//@EXECUTIONFLOW:4.µ˜”√CSUDISCREENSetResolution…Ë÷√±Í«Â∑÷±Ê¬ Œ™NTSC,∏ﬂ«ÂŒ™720P_50HZ(»Áπ˚÷ß≥÷∏ﬂ«Âµƒª∞)
//@EXECUTIONFLOW:5.µ˜”√CSUDISCREENSetResolution…Ë÷√±Í«Â∑÷±Ê¬ Œ™PAL,∏ﬂ«ÂŒ™1080I_50HZ(»Áπ˚÷ß≥÷∏ﬂ«Âµƒª∞)
//@EXECUTIONFLOW:6.µ»¥˝»∑»œ∑÷±Ê¬ µƒ…Ë÷√ «∑Òª·‘Ï≥…OSGÕ∏√˜∂»…Ë÷√µƒ∂™ ß
//@EXECUTIONFLOW:7.µ˜”√CSUDIPLAYERStopÕ£÷π≤‚ ‘Ω⁄ƒø≤•∑≈
//@EXECUTIONFLOW:8. µ˜”√CSUDIPLAYERCloseπÿ±’≤•∑≈∆˜
//@EXECUTIONFLOW:9. µ˜”√CSUDIVIDEOSetWindowSize…Ë÷√∏ﬂ±Í«Â ”∆µ¥∞ø⁄Œ™»´∆¡
//@EXECUTIONFLOW:10.µ˜”√CSUDISCREENSetOSDTransparency…Ë÷√OSGÕ∏√˜∂»Œ™100,ª÷∏¥≤‚ ‘œ÷≥°
//@EXECUTIONFLOW:11.µ˜”√CSUDIOSGFillΩ´∏ﬂ±Í«Âœ‘¥Ê«ÂŒ™0x0
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
	
	hPlayer = PlaySDProgram(EM_UDI_VID_STREAM_MPEG2, EM_UDISCREEN_RESOLUTION_PAL);  //µ˜”√PLAYER≤•∑≈≤‚ ‘¬Î¡˜

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "∆Ù∂ØΩ⁄ƒø≤•∑≈ ß∞‹");

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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(40), "…Ë÷√Õ∏√˜∂» ß∞‹");
	
	CSTCPrint("1. ∆¡ƒªœ‘ æŒ™∫Ï…´OSGøÚ“‘º∞ ”∆µ–°¥∞ø⁄,«“¡Ω’ﬂ”–≤ø∑÷÷ÿµ˛(Y/N)\n");
	CSTCPrint("2. OSGŒ™∞ÎÕ∏√˜–ßπ˚(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTCPrint("∞¥»Œ“‚º¸Ω¯––∑÷±Ê¬ µƒ…Ë÷√,«Î¡Ù“‚∑÷±Ê¬ µƒ«–ªª «∑Òª·‘Ï≥…OSGÕ∏√˜∂»µƒ∂™ ß\n");
	CSTKWaitAnyKey();

	g_scResolution[0] = EM_UDISCREEN_RESOLUTION_INVALID;
	g_scResolution[1] = EM_UDISCREEN_RESOLUTION_INVALID;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice[0], &g_scResolution[0]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹");

	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice[1], &g_scResolution[1]), "ªÒ»°≤‚ ‘«∞∑÷±Ê¬  ß∞‹");
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

	CSTCPrint("∑÷±Ê¬ «–ªª≤ªª·‘Ï≥…OSGÕ∏√˜∂»∂™ ß?(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTK_FATAL_POINT
	 
	if (CSUDI_NULL != hPlayer)
	{
		 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "πÿ±’≤•∑≈∆˜ ß∞‹");
		 hPlayer = CSUDI_NULL;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,NULL),
		"fail to set SD video to full screen");

	if (bSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,NULL),
		"fail to set HD video to full screen");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetOSDTransparency(100), "…Ë÷√Õ∏√˜∂» ß∞‹");

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
				"ªπ‘≠≤‚ ‘«∞∑÷±Ê¬  ß∞‹");
	}

	if (bSupportHD
		&& (g_scResolution[1] >= EM_UDISCREEN_RESOLUTION_720P && g_scResolution[1] < EM_UDISCREEN_RESOLUTION_NUM))
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice[1], &g_scResolution[1], 1),
				"ªπ‘≠≤‚ ‘«∞∑÷±Ê¬  ß∞‹");
	}

	return CSUDI_TRUE;
}

