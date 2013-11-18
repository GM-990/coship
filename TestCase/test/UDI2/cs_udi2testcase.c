/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_udi2testcase.h"
#include "udi2_error.h"
#include "udi2_os.h"
#include "udiplus_debug.h"
#include "udi2_tuner.h"
#include "udi2_demux.h"
#include "udi2_section.h"


static CSUDI_BOOL s_bIsSignalConnected = CSUDI_FALSE;

static CSUDI_BOOL s_bIsSignalLost = CSUDI_FALSE;

static CSUDI_BOOL s_bIsSignalSearching = CSUDI_FALSE;

static CSUDI_BOOL s_bPrepareFrontEndInfo = CSUDI_FALSE;

//前端信息描述
#define MAX_TS_FILENAME_LENGTH  128
typedef struct
{
    CSUDITunerSRCDeliver_U uFreq;
    char    cTsFileName[MAX_TS_FILENAME_LENGTH];
}CSUDIFrontEndInfo_S;

static int s_nFreqCount = 0;
static CSUDIFrontEndInfo_S* s_pFrontEndInfo = CSUDI_NULL;
static CSUDI_BOOL s_bFrontEndShared = CSUDI_FALSE;   //前端设备是否有共享，如有共享每次锁频前都需要提示用户设置设备
static int s_nActiveFreqIndex = -1;
static void iUDI2PrepareFrontEndInfo(void);

//#define TC_PORTING_LOCK()   {CSASSERT(hPortingMutex != CSUDI_NULL);if(hPortingMutex != CSUDI_NULL) {int nRet;nRet = CSUDIOSMutexWait(hPortingMutex, 5000);CSASSERT(nRet == CSUDI_SUCCESS);}}
//#define TC_PORTING_UNLOCK()	{CSASSERT(hPortingMutex != CSUDI_NULL);if(hPortingMutex != CSUDI_NULL) {CSUDIOSMutexRelease(hPortingMutex);}}

CSUDI_BOOL CSTC_UDI2_Init(void)
{
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_UDI2_UnInit(void)
{
	return CSUDI_TRUE;
}

/*
*filter的回调函数
*/
static CSUDI_BOOL s_bRequestdata = CSUDI_FALSE;
static void iUDI2Porting_iCallback_1( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	int i;
   	if(pSection == CSUDI_NULL)
	{
		return ;
	}
 	nLength = pSection->m_nDataLen;
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			{
				CSTCPrint( "[UDI2_INIT]PAT data callback length = %d*****\n", nLength);
				s_bRequestdata = CSUDI_TRUE	;

				for( i = 0; i < nLength; ++ i)
				{

					CSTCPrint( "\n*************** 申请到的数据为%02x\n",pSection->m_pucData[i]);

				}
			}
			break;
	case EM_UDI_REQ_PES_DATA:
		s_bRequestdata = CSUDI_TRUE	;
		CSTCPrint( "[UDI2_INIT]PES data callback length = %d*****\n", nLength);
		for( i = 0; i < nLength; ++ i)
		{

			CSTCPrint( "\n*************** 申请到的数据为%02x\n",pSection->m_pucData[i]);

		}
		break;
		
	default:
		break;
	}
	return;
}
#define UDI2_TUNER0_ID 0
CSUDI_BOOL CSTC_UDI2_TestTunerDemux_001()
{

	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_BOOL bRequested = CSUDI_FALSE;
	int nWaitLoop = 5;
		
	CSTCPrint("[UDI2_INIT]begin lock ..........\n");
	
	//nRet = CSUDIDEMUXConnectTuner(UDI2_TUNER0_ID, 0);
	//CSASSERT(nRet==CSUDI_SUCCESS);
	//CSTC_UDI2PortingLock(UDI2_TUNER0_ID,"Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts");
	
	CSTC_UDI2PortingLock(UDI2_TUNER0_ID,"EPG_0606_121458.ts");
	
	CSTCPrint("[UDI2_INIT]end lock ...........\n");

	while(!s_bIsSignalConnected) //wait for singnal connect
	{
		CSUDIOSThreadSleep(1000);
		CSTCPrint("[UDI2_INIT] locking .........\r\n");
	}
	
	s_bRequestdata = CSUDI_FALSE;
	
	memset(&sTmpReqInfo,0,sizeof(sTmpReqInfo));
	sTmpReqInfo.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	//sTmpReqInfo.m_eReqType = EM_UDI_REQ_PES_DATA;
	sTmpReqInfo.m_nDemuxID = 0;
	sTmpReqInfo.m_nPID = 0x0;          //vedio
	sTmpReqInfo.m_bCRCCheck = CSUDI_TRUE;
#if 1
	sTmpReqInfo.m_sMMFilter.match[0] = 0x0;  //pat
	sTmpReqInfo.m_sMMFilter.mask[0] = 0xff;
	sTmpReqInfo.m_sMMFilter.negate[0] = 0x0;
#endif	
    nRet = CSUDIFILTERAllocate(&sTmpReqInfo,iUDI2Porting_iCallback_1,(void*)UDI2_TUNER0_ID,&hFilter);
	CSASSERT(nRet==CSUDI_SUCCESS);
	
	if(hFilter != CSUDI_NULL && nRet == CSUDI_SUCCESS )
	{
		CSTCPrint("[UDI2_INIT]Start filter.....\n");
		nRet = CSUDIFILTERStart(hFilter);
		CSASSERT(nRet == CSUDI_SUCCESS);
	}
	else
	{
		if(hFilter == CSUDI_NULL) 
		{
			CSTCPrint("[UDI2_INIT]allocate filter hFilter == CSUDI_NULL.....\n");
		}
		else
		{
		   CSTCPrint("[UDI2_INIT]allocate filter failure.....\n");
		}
		return CSUDI_FALSE;
	}
	CSTCPrint("[UDI2_INIT] wait 2s .........\r\n");
	//wait a while
	while((nWaitLoop--)>0)
	{
		CSUDIOSThreadSleep(1000);
		if(s_bRequestdata)
			break;
	}
	
	CSUDIFILTERFree(hFilter);
	hFilter = CSUDI_NULL;
	
	bRequested = s_bRequestdata;
	s_bRequestdata = CSUDI_FALSE;
	
	//nRet = CSUDIDEMUXDisconnectTuner(UDI2_TUNER0_ID, 0);
	//CSASSERT(nRet==CSUDI_SUCCESS);
	return bRequested;
}



static int s_abTunerLockIndex[] = {-1,-1,-1,-1};
static void iTunerCallback(CSUDITunerType_E eType, CSUDITunerCallbackMessage_S *psMessage)
{
	if(psMessage != CSUDI_NULL)
	{
		switch(psMessage->m_eEvent)
		{
			case EM_UDITUNER_SIGNAL_CONNECTED:
				s_bIsSignalConnected = CSUDI_TRUE;
				s_abTunerLockIndex[psMessage->m_dwTunerId] = (int)(psMessage->m_pvUserData);
				CSTCPrint("[UDI2_INIT] Tuner %d Freq %d  connected!!!!\n",psMessage->m_dwTunerId,psMessage->m_uFrequency);
				break;
			case EM_UDITUNER_SIGNAL_LOST:
				s_bIsSignalLost = CSUDI_TRUE;
				CSTCPrint("[UDI2_INIT]Signal lost!!!!\n");
				break;
			case EM_UDITUNER_SIGNAL_SEARCHING:
				s_bIsSignalSearching = CSUDI_TRUE;
				CSTCPrint("[UDI2_INIT]tuner searching ! Maybe disconnect cable!\n");
				break;
			default:
				CSTCPrint("[UDI2_INIT]No Signal!!!!\n");
				break;
		}
	}
}
/*
CSUDI_BOOL CSTC_UDI2PortingLock(void)
{
	int nUserData = 0x88888888;
	CSUDITunerSRCDeliver_U unDeliver;
	
	s_bIsSignalConnected = CSUDI_FALSE;

	s_bIsSignalLost = CSUDI_FALSE;

	s_bIsSignalSearching = CSUDI_FALSE;

	if(CSUDI_SUCCESS != CSUDITunerAddCallback(UDI2_TUNER0_ID, iTunerCallback, &nUserData))
	{
		CSTCPrint("[UDI2_INIT]Add Callback Failed!!!!\n");
		return CSUDI_FALSE;
	}
	
	memset(&unDeliver,0,sizeof(CSUDITunerCableDeliver_S));

	unDeliver.m_sCableDeliver.m_uFrequency = 411000;
	unDeliver.m_sCableDeliver.m_uSymbolRate_24 = 6875;
	unDeliver.m_sCableDeliver.m_uModulation_8= EM_UDITUNER_QAM64;

	if(CSUDI_SUCCESS != CSUDITunerConnect(UDI2_TUNER0_ID, &unDeliver))
	{
		CSTCPrint("[UDI2_INIT]Signal Connect Failed!!!!\n");
		return CSUDI_FALSE;
	}

	CSUDIOSThreadSleep(100);  //wait 2s

	if(s_bIsSignalConnected != CSUDI_TRUE)
	{
		CSTCPrint("[UDI2_INIT]Can't Receive the Connected Message!!!!\n");
		return CSUDI_FALSE;
	}	
	
	if(CSUDI_SUCCESS != CSUDITunerDelCallback(UDI2_TUNER0_ID,iTunerCallback,&nUserData))
	{
		CSTCPrint("[UDI2_INIT]Delete Callback Failed!!!!\n");
	}
	return CSUDI_TRUE;
}
*/
void iUDI2PrepareFrontEndInfo(void)
{
    int i;
    char key[32];
    char value[MAX_TS_FILENAME_LENGTH];
    int shared = 0;
	CSTKCfgError_E cfgRe;
    if(s_pFrontEndInfo == NULL)
    {
        if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", value, sizeof(value)))
        {
            shared = CSTKGetIntFromStr(value,10);
            if(shared)
            {
                s_bFrontEndShared = CSUDI_TRUE;
            }
            else
            {
                s_bFrontEndShared = CSUDI_FALSE;
            }
            CSTCPrint("[Porting]FrontEnd Dev share = %d\r\n",s_bFrontEndShared);
        }
        
        if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_CNT", value, sizeof(value)))
        {
            s_nFreqCount = CSTKGetIntFromStr(value,10);
            CSTCPrint("[Porting]Total Freq count = %d\r\n",s_nFreqCount);

            if(s_nFreqCount > 0)
            {
                s_pFrontEndInfo = (CSUDIFrontEndInfo_S*)CSUDIOSMalloc(sizeof(CSUDIFrontEndInfo_S)*s_nFreqCount);
                CSASSERT(s_pFrontEndInfo != NULL);

                if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","TUNER_TYPE", value, sizeof(value)))
                {
                    int tunerType = CSTKGetIntFromStr(value,10);
                    CSTCPrint("[Porting]Tuner type = %d(1-Cable 2-Satellite 3-Terrestrial)\r\n",tunerType);

                    CSASSERT(CS_TK_CONFIG_SUCCESS == 0);
					
                    for(i=0;i<s_nFreqCount;i++)
                    {
                        cfgRe = CS_TK_CONFIG_SUCCESS;

                        sprintf(key,"TS%d_TSFILE",i);
                        cfgRe |= CSTKGetConfigInfo("UDI2",key, value, sizeof(value));
                        strcpy(s_pFrontEndInfo[i].cTsFileName,value);

                        if(tunerType == 1)
                        {
                            sprintf(key,"CAB_TS%d_FREQ",i);
                            cfgRe |= CSTKGetConfigInfo("UDI2",key, value, sizeof(value));
                             s_pFrontEndInfo[i].uFreq.m_sCableDeliver.m_uFrequency = CSTKGetIntFromStr(value,10);
                            
                            sprintf(key,"CAB_TS%d_SR",i);
                            cfgRe |= CSTKGetConfigInfo("UDI2",key, value, sizeof(value));
                            s_pFrontEndInfo[i].uFreq.m_sCableDeliver.m_uSymbolRate_24 = CSTKGetIntFromStr(value,10);
                            
                            sprintf(key,"CAB_TS%d_MODE",i);
                            cfgRe |= CSTKGetConfigInfo("UDI2",key, value, sizeof(value));
                            s_pFrontEndInfo[i].uFreq.m_sCableDeliver.m_uModulation_8 = CSTKGetIntFromStr(value,10);
                        }
                        else if(tunerType == 2)
                        {
                            //todo add qpsk param get
                        }
                        else if(tunerType == 3)
                        {
                            //todo add cofom param get
                        }
                        else
                        {
                            cfgRe = CS_TK_CONFIG_FAILURE;
                            CSTCPrint("'TUNER_TYPE'错误，请检查配置文件\r\n");
                        }

                        if(cfgRe != CS_TK_CONFIG_SUCCESS)//假设所有调制方式的频点一样多
                        {
                            break;
                        }
                    }

                    s_nFreqCount = (i > 0) ? i:0;//reset s_nFreqCount
                }
            }            
        }
    }    
}
CSUDI_BOOL CSTC_UDI2PortingLock(int nTunerIndex,const char* pTsFilename)
{
	int i;
	int cnt = 20;
	int sleeptime = 100;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	if(!s_bPrepareFrontEndInfo)
	{
		iUDI2PrepareFrontEndInfo();
		s_bPrepareFrontEndInfo = CSUDI_TRUE;
		CSTCPrint("iUDI2PrepareFrontEndInfo initial ....\r\n");
	}

	if(pTsFilename != CSUDI_NULL && nTunerIndex != -1)
	{
		CSTCPrint("CSTC_UDI2PortingLock s_nFreqCount:%d\r\n", s_nFreqCount);

		for(i=0;i<s_nFreqCount;i++)
		{
			if(strcmp(pTsFilename,s_pFrontEndInfo[i].cTsFileName) == 0)
			{      
				CSTCPrint("Lock tsfile %s Found @ freq %d\r\n",pTsFilename,i);

				if(s_bFrontEndShared && s_nActiveFreqIndex != i)
				{
					CSTCPrint("请在 C:%d/T:%d/S:%d 频点准备好[%s]码流,并按y确认。\r\n",
					s_pFrontEndInfo[i].uFreq.m_sCableDeliver.m_uFrequency,
					s_pFrontEndInfo[i].uFreq.m_sTerrestrialDeliver.m_uFrequency,
					s_pFrontEndInfo[i].uFreq.m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency,
					s_pFrontEndInfo[i].cTsFileName);

					if(CSTKWaitYes())
					{
						s_nActiveFreqIndex = i;
					}
					else
					{
						CSTCPrint("前端未准备好，锁频失败\r\n");
						i = s_nFreqCount;                        
					}
				}

				break;
			}
		}

		if(i < s_nFreqCount)//found ts file
		{
			if(CSUDI_SUCCESS != CSUDITunerAddCallback(nTunerIndex, iTunerCallback, (void*)i))
			{
			CSTCPrint("[UDI2_INIT]Add Callback Failed!!!!\n");
			return CSUDI_FALSE;
			}

			s_abTunerLockIndex[nTunerIndex] = -1;
			if(CSUDI_SUCCESS == CSUDITunerConnect(nTunerIndex, &s_pFrontEndInfo[i].uFreq))
			{ 
				while(cnt--)
				{
					CSUDIOSThreadSleep(sleeptime);

					if(s_abTunerLockIndex[nTunerIndex] == i)
					{
						break;
					}
				}

				if(s_abTunerLockIndex[nTunerIndex] == i)
				{
					CSTCPrint("Lock Tuner(%d) SUCCESS!\r\n",nTunerIndex);
					bRet = CSUDI_TRUE;
				}
				else
				{
					CSTCPrint("Lock Tuner(%d) FAILED!\r\n",nTunerIndex);
				}
			}
			else
			{
				CSTCPrint("Lock tsfile %s Failed\r\n",pTsFilename);
			}

			if(CSUDI_SUCCESS != CSUDITunerDelCallback(UDI2_TUNER0_ID,iTunerCallback,(void*)i))
			{
				CSTCPrint("[UDI2_INIT]Delete Callback Failed!!!!\n");
			}
		}
		else
		{
			CSTCPrint("配置文件中未找到 %s 的描述，或前端未准备好\r\n", pTsFilename);
			CSTCPrint("tsfile %s not found in config file or not prepared\r\n", pTsFilename);
		}
	}

	return bRet;
}

#if 0
static CSUDI_UINT32 g_anTunerID[20];

void UDI_Tuner_test(void)
{
    	CSUDITunerInitParam_S sTunerInitParam;
    	CSUDI_Error_Code eRet;
    	int nUserData = 8;
    	CSUDITunerSRCDeliver_U sTunerParam;
    	CSUDITunerInfo_S sTunerInfo;
    	int nActTunerCount;
    	int ii;

        memset(&sTunerInitParam, 0, sizeof(sTunerInitParam));

        sTunerInitParam.m_eTSOutMode = TS_PARALLEL;
        sTunerInitParam.m_nPriority = 159;
        sTunerInitParam.m_uI2CIndex = 0;
        sTunerInitParam.m_uResetGPIONum = 0xff;

        CSUDITunerRegisterQam();

       if ( CSUDITunerInit(0, &sTunerInitParam) != CSUDI_SUCCESS)
       {
            CSTCPrint("\n[Equipment_Init] CSUDITunerInit failed\n");
       }
       else
       {
             CSTCPrint("\n[Equipment_Init] CSUDITunerInit success\n");
       }

       eRet = CSUDITunerAddCallback(0, portingTunerCallback, &nUserData);
       CSTCPrint("CSUDITunerAddCallback Ret = %d\r\n", eRet);

       sTunerParam.m_sCableDeliver.m_uFrequency = 411000;
       sTunerParam.m_sCableDeliver.m_uSymbolRate_24 = 6875;
       sTunerParam.m_sCableDeliver.m_uModulation_8 = EM_UDITUNER_QAM64;

       eRet =  CSUDITunerConnect(0, &sTunerParam);
       CSTCPrint("CSUDITunerConnect Ret = %d\r\n", eRet);

       eRet = CSUDITunerGetAllDeviceId(g_anTunerID, 20, &nActTunerCount);
       CSTCPrint("CSUDITunerGetAllDeviceId Ret = %d, nActTunerCount = %d\r\n", eRet, nActTunerCount);

       for (ii=0; ii<nActTunerCount; ii++)
       {
            CSTCPrint("CSUDITunerGetAllDeviceId g_anTunerID[%d] = %d\r\n", ii, g_anTunerID[ii]);
            eRet = CSUDITunerGetDeviceInfo(g_anTunerID[ii], &sTunerInfo);
           CSTCPrint("CSUDITunerGetDeviceInfo Ret = %d, sTunerInfo.m_eType = %d, sTunerInfo.m_eTransferType=%d, sTunerInfo.m_bPIDFilterSupport = %d\r\n", 
           eRet, sTunerInfo.m_eType, sTunerInfo.m_eTransferType, sTunerInfo.m_bPIDFilterSupport);
       }

       CSUDIOSThreadSleep(2000);

      // eRet =  CSUDITunerDelCallback(0, portingTunerCallback, &nUserData);
      // CSTCPrint("CSUDITunerDelCallback Ret = %d\r\n", eRet);

	CSTCPrint("[UDI2_INIT]TestTunerDemux\n");
	{
		CSUDI_HANDLE hSuspend = CSUDI_NULL;
		CSUDIOSEventCreate("Suspend Root",0,&hSuspend);
   		CSUDIOSEventWait(hSuspend, CSUDIOS_TIMEOUT_INFINITY);
	}
}
#endif
