
/*Include head files*/
#include <string.h>

#include "cs_tunertest.h"
#include "cs_testkit.h"

//系统上总的Tuner数
static int s_nActTunerCount = 0;

//系统是否支持可移动Tuner
static CSUDI_BOOL s_bIsSupportRDICfgTuner = CSUDI_FALSE;

//配置文件中总的频点数
static int s_nAllDeliverCount = 0;

//QPSK测试的频点个数
//static int s_nQpskDeliverCount = 5;

//Tuner 的能力信息
static CSUDITunerInfo_S s_asTunerCfgInfo[10];

//锁频频点信息
static CSUDITunerCableDeliver_S s_asCableDeliver[100];

//QPSK锁频频点信息
//static CSUDITunerSatelliteDeliver_S s_asQpskDeliver[100];

//任务正常运行结束标志,True:正常结束，False:非正常结束
CSUDI_BOOL g_bIsThreadNaturalEndFlag1 = CSUDI_FALSE;

CSUDI_BOOL g_bIsThreadNaturalEndFlag2 = CSUDI_FALSE;

//是否锁频成功的标志，用于主子任务通信
CSUDI_BOOL g_bIsTunerConnected = CSUDI_FALSE;



void CSTC_TUNER_InitCfg(void)
{
	char cTunerCfgBuf[32];
	char cTestParam[64];
	int nTunerType;
	int nTunerDataTransferType; 
	int i;
	
	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_DEVICE_COUNT",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_nActTunerCount = CSTKGetIntFromStr(cTunerCfgBuf, 10);
	}
	
	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_ALL_DELIVER_COUNT",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_nAllDeliverCount = CSTKGetIntFromStr(cTunerCfgBuf, 10);
		
		if(s_nAllDeliverCount < 4)
		{
			CSTCPrint("\n频点信息提供不足，至少需要4个才能进行测试，请检查配置文件\n");
		}
	}
	
	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_RDI_TUNER_SUPPORT",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		if(CSTKGetIntFromStr(cTunerCfgBuf, 10) == 1)
		{
			s_bIsSupportRDICfgTuner = CSUDI_TRUE;
		}
		else
		{
			s_bIsSupportRDICfgTuner = CSUDI_FALSE;
		}
	}

	memset(s_asTunerCfgInfo,0,sizeof(s_asTunerCfgInfo)/sizeof(s_asTunerCfgInfo[0]));
	
	for(i = 0 ; i < s_nActTunerCount ; i++)
	{
		memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
		memset(cTestParam,'\0',sizeof(cTestParam));
		sprintf(cTestParam,"%s%d%s","CS_TUNER",i,"_TYPE");
		
		if(CSTKGetConfigInfo("TUNER",cTestParam,cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
		{
			nTunerType = CSTKGetIntFromStr(cTunerCfgBuf,10);
			switch(nTunerType)
			{
				case EM_UDITUNER_TYPE_QAM:
					s_asTunerCfgInfo[i].m_eType = EM_UDITUNER_TYPE_QAM;
					break;
				case EM_UDITUNER_TYPE_COFDM:
					s_asTunerCfgInfo[i].m_eType = EM_UDITUNER_TYPE_COFDM;
					break;
				case EM_UDITUNER_TYPE_QPSK:
					s_asTunerCfgInfo[i].m_eType = EM_UDITUNER_TYPE_QPSK;
					break;
				default:
					s_asTunerCfgInfo[i].m_eType = EM_UDITUNER_TYPE_QAM;
					break;
			}
		}

		memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
		memset(cTestParam,'\0',sizeof(cTestParam));
		sprintf(cTestParam,"%s%d%s","CS_TUNER",i,"_DATA_TRANSFER_TYPE");

		if(CSTKGetConfigInfo("TUNER",cTestParam,cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
		{
			nTunerDataTransferType = CSTKGetIntFromStr(cTunerCfgBuf,10);
			switch(nTunerDataTransferType)
			{
				case EM_UDITUNER_DATATRANSFER_DMA:
					s_asTunerCfgInfo[i].m_eTransferType = EM_UDITUNER_DATATRANSFER_DMA;
					break;
				case EM_UDITUNER_DATATRANSFER_SOFT:
					s_asTunerCfgInfo[i].m_eTransferType = EM_UDITUNER_DATATRANSFER_SOFT;
					break;
				default:
					s_asTunerCfgInfo[i].m_eTransferType = EM_UDITUNER_DATATRANSFER_DMA;
					break;
			}
		}

		memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
		memset(cTestParam,'\0',sizeof(cTestParam));
		sprintf(cTestParam,"%s%d%s","CS_TUNER",i,"_PID_FILTER_SUPPORT");
		
		if(CSTKGetConfigInfo("TUNER",cTestParam,cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
		{
			if(CSTKGetIntFromStr(cTunerCfgBuf,10) == 1)
			{
				s_asTunerCfgInfo[i].m_bPIDFilterSupport = CSUDI_TRUE;
			}
			else
			{
				s_asTunerCfgInfo[i].m_bPIDFilterSupport = CSUDI_FALSE;
			}
		}
	}
			
	memset(s_asCableDeliver,0,sizeof(s_asCableDeliver)/sizeof(s_asCableDeliver[0]));
	for(i = 0 ; i< s_nAllDeliverCount - 2 ; i++)
	{
		memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
		memset(cTestParam,'\0',sizeof(cTestParam));
		sprintf(cTestParam,"%s%d","CS_TUNER_CABLE_DELIVER_FREQUENCY_",i+1);
		
		if(CSTKGetConfigInfo("TUNER",cTestParam,cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
		{
			s_asCableDeliver[i].m_uFrequency = CSTKGetIntFromStr(cTunerCfgBuf,10);
		}

		memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
		memset(cTestParam,'\0',sizeof(cTestParam));
		sprintf(cTestParam,"%s%d","CS_TUNER_CABLE_DELIVER_SYMBOL_RATE_",i+1);
		
		if(CSTKGetConfigInfo("TUNER",cTestParam,cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
		{
			s_asCableDeliver[i].m_uSymbolRate_24 = CSTKGetIntFromStr(cTunerCfgBuf,10);
		}

		memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
		memset(cTestParam,'\0',sizeof(cTestParam));
		sprintf(cTestParam,"%s%d","CS_TUNER_CABLE_DELIVER_MODULATION_",i+1);
		
		if(CSTKGetConfigInfo("TUNER",cTestParam,cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
		{
			s_asCableDeliver[i].m_uModulation_8 = CSTKGetIntFromStr(cTunerCfgBuf,10);
		}
		else
		{
			break;
		}
	}
	
	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_CABLE_DELIVER_FREQUENCY_SPECIAL",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_asCableDeliver[s_nAllDeliverCount-2].m_uFrequency = CSTKGetIntFromStr(cTunerCfgBuf,10);
	}

	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_CABLE_DELIVER_SYMBOL_RATE_SPECIAL",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_asCableDeliver[s_nAllDeliverCount-2].m_uSymbolRate_24 = CSTKGetIntFromStr(cTunerCfgBuf,10);
	}

	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_CABLE_DELIVER_MODULATION_SPECIAL",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_asCableDeliver[s_nAllDeliverCount-2].m_uModulation_8 = CSTKGetIntFromStr(cTunerCfgBuf,10);
	}

	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_CABLE_DELIVER_FREQUENCY_INVAIL",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_asCableDeliver[s_nAllDeliverCount-1].m_uFrequency = CSTKGetIntFromStr(cTunerCfgBuf,10);
	}

	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_CABLE_DELIVER_SYMBOL_RATE_INVAIL",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_asCableDeliver[s_nAllDeliverCount-1].m_uSymbolRate_24 = CSTKGetIntFromStr(cTunerCfgBuf,10);
	}

	memset(cTunerCfgBuf,'\0',sizeof(cTunerCfgBuf));
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_CABLE_DELIVER_MODULATION_INVAIL",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_asCableDeliver[s_nAllDeliverCount-1].m_uModulation_8 = CSTKGetIntFromStr(cTunerCfgBuf,10);
	}
}

CSUDI_BOOL CSTC_TUNER_InfoCmp(CSUDITunerInfo_S sTunerInfo1,CSUDITunerInfo_S sTunerInfo2)
{
	if((sTunerInfo1.m_eType == sTunerInfo2.m_eType) &&
	    	(sTunerInfo1.m_eTransferType == sTunerInfo2.m_eTransferType) &&
	    	(sTunerInfo1.m_bPIDFilterSupport == sTunerInfo2.m_bPIDFilterSupport))
	{
		return CSUDI_TRUE;
	}
	else
	{
		return CSUDI_FALSE;
	}
}

void CSTC_TUNER_Deliver_Init(CSUDITunerSRCDeliver_U unDeliver[], int nCount)
{
	int i;

       memset(unDeliver,0,sizeof(CSUDITunerSRCDeliver_U)*nCount);
	   
	for(i=0 ; i<nCount ; i++)
	{
		unDeliver[i].m_sCableDeliver.m_uFrequency = s_asCableDeliver[i].m_uFrequency;
		unDeliver[i].m_sCableDeliver.m_uModulation_8 = s_asCableDeliver[i].m_uModulation_8;
		unDeliver[i].m_sCableDeliver.m_uSymbolRate_24 = s_asCableDeliver[i].m_uSymbolRate_24;
	}
}

void CSTC_TUNER_TunerInfo_Init(CSUDITunerInfo_S asTunerInfo[], int nCount)
{
	int i;
	
	memset(asTunerInfo,0,sizeof(asTunerInfo)/sizeof(asTunerInfo[0]));

	for(i=0 ; i<nCount ; i++)
	{
		asTunerInfo[i].m_eType = s_asTunerCfgInfo[i].m_eType;
		asTunerInfo[i].m_eTransferType = s_asTunerCfgInfo[i].m_eTransferType;
		asTunerInfo[i].m_bPIDFilterSupport = s_asTunerCfgInfo[i].m_bPIDFilterSupport;
	}
}

void CSTC_TUNER_TunerParam_Init(int *pnTunerCount, int *pnDeliverCount, CSUDI_BOOL *pbSupportRDITuner)
{
	*pnTunerCount = s_nActTunerCount;
	*pnDeliverCount = s_nAllDeliverCount;
	*pbSupportRDITuner = s_bIsSupportRDICfgTuner;
}

int CSTC_TUNER_GetDeliverCount(void)
{
	return s_nAllDeliverCount;
}

int CSTC_TUNER_GetRandom(int radix , int min)
{
	int nMiliSecond;
	int nRandom;
	CSUDIOSTimeVal_S s_Time;

	CSUDIOSGetTime(&s_Time);
	
	nMiliSecond = s_Time.m_nSecond * 1000 + s_Time.m_nMiliSecond;

	nRandom = nMiliSecond % (radix - min);

	nRandom += min;

	return nRandom;
}

CSUDI_BOOL CSTC_TUNER_ThreadConnect(CSUDI_UINT32 dwTunerId, unsigned int DeliverIndex, CSUDI_BOOL bThreadEndFlag)
{
	CSUDITunerSRCDeliver_U unDeliver;
	int nSleepTime;
	int nConnectCount = 0;
	int nSucceedCount = 0;
	int i;
	
	memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));
	   
	unDeliver.m_sCableDeliver.m_uFrequency = s_asCableDeliver[DeliverIndex].m_uFrequency;
	unDeliver.m_sCableDeliver.m_uModulation_8 = s_asCableDeliver[DeliverIndex].m_uModulation_8;
	unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_asCableDeliver[DeliverIndex].m_uSymbolRate_24;

	for(i=0 ; i<10 ; i++)
	{
		if(CSUDI_SUCCESS == CSUDITunerConnect(dwTunerId, &unDeliver))
		{
			if(bThreadEndFlag)
			{
				CSUDIOSThreadSleep(800);
				if(g_bIsTunerConnected == CSUDI_TRUE)
				{
					g_bIsTunerConnected = CSUDI_FALSE;
					nSucceedCount++;
				}
				nSleepTime = CSTC_TUNER_GetRandom(100,10);

				CSUDIOSThreadSleep(nSleepTime);
			}
			else
			{
				CSUDIOSThreadSleep(800);
				
				nSleepTime = CSTC_TUNER_GetRandom(500,10);

				CSUDIOSThreadSleep(nSleepTime);
			}
			
			nConnectCount++;
		}
		
	}

	if(bThreadEndFlag)
	{
		if((nConnectCount == 10) && (nSucceedCount == 10))
		{
			return CSUDI_TRUE;
		}
	}
	else
	{
		if(nConnectCount== 10)
		{
			return CSUDI_TRUE;
		}
	}
	
	return CSUDI_FALSE;
}

void Tuner_TestThread1_1Entry(void *pvParam)
{
	int nCount = 0;
	
	TunerCallbackParam_S *psTunerCallbackParam;
	
	psTunerCallbackParam = CSUDIOSCalloc(2, sizeof(TunerCallbackParam_S));

	psTunerCallbackParam = (TunerCallbackParam_S *)pvParam;

	if(CSUDI_SUCCESS == CSUDITunerAddCallback(psTunerCallbackParam[0].dwTunerId, 
		psTunerCallbackParam[0].fnTunerCallback, psTunerCallbackParam[0].pvUserData))
	{
		nCount++;
	}

	CSUDIOSThreadSleep(2000);

	if(CSUDI_SUCCESS == CSUDITunerDelCallback(psTunerCallbackParam[1].dwTunerId, 
		psTunerCallbackParam[1].fnTunerCallback, psTunerCallbackParam[1].pvUserData))
	{
		nCount++;
	}

	if(nCount == 2)
	{
		psTunerCallbackParam[0].bThreadFlag = CSUDI_TRUE;
	}
	
}

void Tuner_TestThread1_2Entry(void *pvParam)
{
	int nCount = 0;

	TunerCallbackParam_S *psTunerCallbackParam;
	
	psTunerCallbackParam = CSUDIOSCalloc(2, sizeof(TunerCallbackParam_S));

	psTunerCallbackParam = (TunerCallbackParam_S *)pvParam;

	if(CSUDI_SUCCESS == CSUDITunerAddCallback(psTunerCallbackParam[1].dwTunerId, 
		psTunerCallbackParam[1].fnTunerCallback, psTunerCallbackParam[1].pvUserData))
	{
		nCount++;
	}

	CSUDIOSThreadSleep(2000);

	if(CSUDI_SUCCESS == CSUDITunerDelCallback(psTunerCallbackParam[0].dwTunerId, 
		psTunerCallbackParam[0].fnTunerCallback, psTunerCallbackParam[0].pvUserData))
	{
		nCount++;
	}

	if(nCount == 2)
	{
		psTunerCallbackParam[1].bThreadFlag = CSUDI_TRUE;
	}
	
}

void Tuner_TestThread2_1Entry(void *pvParam)
{
	CSUDI_UINT32 *pdwTunerId;
	
	pdwTunerId = (CSUDI_UINT32 *)pvParam;
	
	g_bIsThreadNaturalEndFlag1 = CSTC_TUNER_ThreadConnect(*pdwTunerId, 0, CSUDI_FALSE);

}

void Tuner_TestThread2_2Entry(void *pvParam)
{
	CSUDI_UINT32 *pdwTunerId;
	
	pdwTunerId = (CSUDI_UINT32 *)pvParam;
	
	g_bIsThreadNaturalEndFlag2 = CSTC_TUNER_ThreadConnect(*pdwTunerId, 0, CSUDI_FALSE);
}

void Tuner_TestThread3Entry(void *pvParam)
{
	CSUDI_UINT32 *pdwTunerId;
	
	pdwTunerId = (CSUDI_UINT32 *)pvParam;
	
	g_bIsThreadNaturalEndFlag2 = CSTC_TUNER_ThreadConnect(*pdwTunerId, 1, CSUDI_FALSE);
}

void Tuner_TestThread4_1Entry(void *pvParam)
{
	CSUDI_UINT32 *pdwTunerId;
	
	pdwTunerId = (CSUDI_UINT32 *)pvParam;
	
	g_bIsThreadNaturalEndFlag1 = CSTC_TUNER_ThreadConnect(*pdwTunerId, 0, CSUDI_TRUE);
}

void Tuner_TestThread4_2Entry(void *pvParam)
{
	CSUDI_UINT32 *pdwTunerId;
	
	pdwTunerId = (CSUDI_UINT32 *)pvParam;
	
	g_bIsThreadNaturalEndFlag2 = CSTC_TUNER_ThreadConnect(*pdwTunerId, 0, CSUDI_TRUE);
}




