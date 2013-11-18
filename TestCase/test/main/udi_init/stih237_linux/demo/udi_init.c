//#include "setup.h"
//#include "irdecode.h"
#include "cs_testkit.h"
#include "cs_toolset_plus.h"

#include "udi2_tuner.h"
#include "udi2_demux.h"

#include "cs_i2c_plus.h"
#include "cs_gpio_plus.h"
#include "cs_tuner_plus.h"

#include "cs_panel_plus.h"
#include "cs_fs_plus.h"
#include "udiplus_debug.h"
#include "cs_qpsk.h"
#include "udi2_player.h"
#include "cs_flash2_plus.h"
#include "udi_assert.h"
#include "udi2_video.h"
#include "udi2_os.h"

#include "cs_panel_plus.h"
#include "cs_panel.h"

#define MAX_MODULE_TO_TEST	12
#define PIO_PIN(x,y)    (8*x+y)
/*For test initialize time */

typedef	struct  _InitialTimeCost_t
{
    char   *pModuleName;
    DWORD dwTimeCost;
}
InitialTimeCost_t;

DWORD g_dwTimeStart;
DWORD g_dwTimeStop;
DWORD g_dwTimeCost;

int ModuleIndex = 0;

InitialTimeCost_t g_asTimeCost[MAX_MODULE_TO_TEST];

void TimePrint(void) ; //��ӡ��ʼ��ʱ�亯��

static void Tuner_CallbackTest5(CSUDITunerType_E eType, CSUDITunerCallbackMessage_S *psMessage)
{	

	if(psMessage != CSUDI_NULL)
	{
		switch(psMessage->m_eEvent)
		{
			case EM_UDITUNER_SIGNAL_CONNECTED:
				CSTCPrint("\nSignal has been connected\n");
				break;
			case EM_UDITUNER_SIGNAL_LOST:
				CSTCPrint("\nSignal has been lost\n");
				break;
			case EM_UDITUNER_SIGNAL_SEARCHING:
				CSTCPrint("\nSignal has been searching\n");
				break;
			default:
				break;
		}
	}
	else
	{
		CSTCPrint("\n�ص���ϢpsMessageΪ��!!!!\n");
	}

//	g_bIsTunerConnected = CSUDI_TRUE;//������������Ǻ������أ�����ע��Ƶ���

    return;
}

static void CSQPSK_UT_CallBack(CSHDICallbackType type , void * pt )
{
	QPSKCallbackMessage *msg = (QPSKCallbackMessage *)pt;

	switch (msg->dwNIMEvent)
	{
    		case QPSK_CALLBACK_SIGNAL_DROPPED:
    			printf( "[CSQPSK_UT_CallBack] SIGNAL_DROPPED\r\n");
    			break;
				
    		case QPSK_CALLBACK_SIGNAL_LOST:
    			printf( "[CSQPSK_UT_CallBack] SIGNAL_LOST\r\n");
    			break;
    			
    		case QPSK_CALLBACK_SIGNAL_SEARCHING:
    			printf( "[CSQPSK_UT_CallBack] SIGNAL_SEARCHING\r\n");
    			break;

    		case QPSK_CALLBACK_SIGNAL_CONNECTED:
    			printf( "[CSQPSK_UT_CallBack] SIGNAL_CONNECTED\r\n");
    			break;

    		default:
    			break;
	}	
}

/*
	TunerԤ��ʼ������ƽ̨��ʼ��֮ǰ����
*/
static void PerInitTuner()
{
	/* set HW config for N8000 ST7162 board (YMB.A083.B) */
	CSUDISYSSetCfg(CSUDICFG_TUNER_MODE, 0/*tuner 0*/, TS_SERIAL, 0);
	CSUDISYSSetCfg(CSUDICFG_TUNER_TSIN, 0/*tuner 0*/, 0 /* TSIN 0 */, 0);
	CSUDISYSSetCfg(CSUDICFG_TSIN_DMX, 0/*TSIN 0*/, 0 /* Demux 0 */, 0);
}

/*
	Tuner��ʼ��
*/
static void CSTCInitTuner()
{
	CSUDI_Error_Code ErrorCode;

	/* connect demux 0 to tuner 0 */
	ErrorCode = CSUDIDEMUXDisconnectTuner(0, 0);
	if (ErrorCode != CSUDI_SUCCESS) CSHDIUartPrint("CSUDIDEMUXDisconnectTuner Failed.\n");

	ErrorCode = CSUDIDEMUXConnectTuner(0, 0);
	if (ErrorCode != CSUDI_SUCCESS) CSHDIUartPrint("CSUDIDEMUXConnectTuner Failed.\n");

	PIO_FE_Reset(TRUE);
	CSUDIOSThreadSleep(1000);
	PIO_FE_Reset(FALSE);
	CSUDIOSThreadSleep(1000);
	PIO_FE_Reset(TRUE);
	CSUDIOSThreadSleep(1000);
}

/*��Ƶ*/
static void LockTuner()
{
	CSQPSKSignalParameterS2 pCSQpskParam;
	memset(&pCSQpskParam, 0, sizeof(CSQPSKSignalParameterS2));
	
	/*ָ����Ƶ����*/
	pCSQpskParam.b22K = CSHDI_OFF;
	pCSQpskParam.bDiSEqC10 = DISEQC10_OFF;//PORT_4_A;//DISEQC10_OFF;
	pCSQpskParam.bDiSEqCRepeat = TWICEREPEAT;
	pCSQpskParam.bIQ = 0;
	pCSQpskParam.bLNBPower = LNB_OFF;
	pCSQpskParam.bLNBType = SINGLE;
	pCSQpskParam.bSwitch0_12V = SWITCH_0V;
	pCSQpskParam.bToneBurst = TONEBURST_NONE;
	pCSQpskParam.m_dwstandard  = DVBS_STANDARD;
	pCSQpskParam.bFECRates = FEC_AUTO;
	pCSQpskParam.bPolar = POLARISATION_H;
	pCSQpskParam.m_dwModulation = CS_QPSK;
	pCSQpskParam.dwFreqency= 3880000;		//Ƶ��
	pCSQpskParam.dwLNBLowFreq = 5150000;		//����
	pCSQpskParam.dwSymbolRate = 27500;		//������

	CSQPSKConnectSignalS2(Tuner_0, &pCSQpskParam);
	CSUDIOSThreadSleep(3000);
}

/*
	����
*/
static void PlayProgram()
{
	CSUDI_Error_Code ErrorCode;
	CSUDIPlayerChnl_S psPlayerChnl;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_LIVE;
	CSUDI_HANDLE hPlayer = NULL;
	CSUDIStreamInfo_S aStreamInfo[2];
	

	psPlayerChnl.m_nAudioDecoder = 0;
	psPlayerChnl.m_nDemux = 0;
	psPlayerChnl.m_nVideoDecoder = 0;

	int i=0;

//	while(1)
	{
		ErrorCode = CSUDIPLAYEROpen(&psPlayerChnl, ePlayerType, &hPlayer);
		if (CSUDI_SUCCESS != ErrorCode || hPlayer == NULL)
		{
			printf("________CSUDIPLAYEROpen FAIL_________\r\n");
		}

//		if ((i%2) == 0)
		{
			aStreamInfo[0].m_nPid = 100;
			aStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			aStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG1;

			aStreamInfo[1].m_nPid = 101;
			aStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			aStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG1;

			i = 1;
		}
#if 0
		else
		{
			aStreamInfo[0].m_nPid = 513;
			aStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			aStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

			aStreamInfo[1].m_nPid = 514;
			aStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			aStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;

			i = 0;
		}
#endif
		ErrorCode = CSUDIPLAYERSetStream(hPlayer, aStreamInfo, 2, NULL);
		if (CSUDI_SUCCESS != ErrorCode)
		{
			printf("________CSUDIPLAYERSetStream FAIL_________\r\n");
		}
		
		ErrorCode = CSUDIPLAYERStart(hPlayer);
		if (CSUDI_SUCCESS != ErrorCode)
		{
			printf("________CSUDIPLAYERStart FAIL_________\r\n");
		}

		CSUDIVIDEOShow(0,1);

	//	CSUDIOSThreadSleep(1000*3);

	//	if (CSUDI_SUCCESS  != CSUDIPLAYERClose (hPlayer))
		{
		//	CSTCPrint("Failed to close a player.\n");
		//	return CSUDI_FAILURE;
		}

	}

}
#if 0
static void CSUDIInitPanel(void)
{
	CSUDIPanelGpioCfg_S sParams;
	CSUDI_Error_Code ErrorCode;
	CSUDIPanelType_E ePanelType = EM_PANEL_TYPE_GPIO_NO_LED;
	
	memset(&sParams, 0xff, sizeof(CSUDIPanelGpioCfg_S));
#if 0 /*niuchunguo test,2013/5/14  9:20:47 */
	sParams.m_ulKeyNum = 8;
	sParams.m_ucKeyPinGroup[0] =121;//CH-DOWN
	sParams.m_ucKeyPinGroup[1] =25;//POWER
	sParams.m_ucKeyPinGroup[2] =0xff;//ENTER
	sParams.m_ucKeyPinGroup[3] = 122;//CH-LEFT
	sParams.m_ucKeyPinGroup[4] =123;//CH-RIGHT
	sParams.m_ucKeyPinGroup[5] =0xff;
	sParams.m_ucKeyPinGroup[6] = 0xff;
	sParams.m_ucKeyPinGroup[7] = 0xff;
#else
	sParams.m_ulKeyNum = 8;
	sParams.m_ucKeyPinGroup[0] =PIO_PIN(15,3);        //CH+
	sParams.m_ucKeyPinGroup[1] =PIO_PIN(14,7);	     //CH-
	sParams.m_ucKeyPinGroup[2] =PIO_PIN(3,1);          //power
	sParams.m_ucKeyPinGroup[3] =PIO_PIN(15,1);       //ok
	sParams.m_ucKeyPinGroup[4] =PIO_PIN(15,4);       // vol -
	sParams.m_ucKeyPinGroup[5] =PIO_PIN(14,4);       //vol +
	sParams.m_ucKeyPinGroup[6] = 0xff;
	sParams.m_ucKeyPinGroup[7] =0xff;
#endif /* End test */

	
	
	ErrorCode = CSUDIPanelInitExt(0,ePanelType,(void *) &sParams);
	CSTRACE(ERROR_LEVEL, "[panel_init]:CSUDIPanelInitExt Return ErrorCode =%d ! \r\n", ErrorCode);
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	ErrorCode = CSPanelSetSingleKeyRepeat(CSUDI_VK_OK,TRUE);
	ErrorCode = CSPanelSetSingleKeyRepeat( CSUDI_VK_LEFT, TRUE);
	ErrorCode = CSPanelSetSingleKeyRepeat(CSUDI_VK_RIGHT,TRUE);
	ErrorCode = CSPanelSetSingleKeyRepeat( CSUDI_VK_UP, TRUE);
}
#endif
static void  CSUDIInitFlash(void)
{
	CSHDI_Error_t eRet = CSHDI_FAILURE;
	CSFLS2InitParams_S NandParams;
	CSFLS2Partition_S FlsPartition;
       char ParName[32] = "f_partition0";

	strncpy((char *)FlsPartition.m_szPTName, ParName, sizeof(FlsPartition.m_szPTName));
	FlsPartition.m_uOffset = 0;
	FlsPartition.m_uPTSize = 0x8000000;
	FlsPartition.m_uSpareSize = 0x1000000;

	memset(&NandParams, 0, sizeof(NandParams));
	NandParams.m_dwBaseAddress = 0xA0000000;// + NorParams.m_dwChipSize;//0xA0000000 + NorParams.m_dwChipSize;
	NandParams.m_nVPPGPIOIndex = 0xff;
	NandParams.m_dwChipSize = 0x8000000;
	eRet = CSFLS2InitNand(0, &NandParams);
	CSASSERT(eRet == CSHDI_SUCCESS);
       CSFLS2SetNandPartition(0, &FlsPartition, 1);
}


void udi_init(void)
{
	int ii=0;
	CSUDI_Error_Code ErrorCode = CSUDI_SUCCESS;
	CSUDIPanelParams_S sParams; 

	CSDebugSetDefaultLevel(INFO_LEVEL);

	for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
	{
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
	}

	/* set HW config for N8000 ST7162 board (YMB.A083.B) */
	CSUDISYSSetCfg(CSUDICFG_TUNER_MODE, 0/*tuner 0*/, TS_SERIAL, 0);
	CSUDISYSSetCfg(CSUDICFG_TUNER_TSIN, 0/*tuner 0*/, 2 /* TSIN 2 */, 0);

	PerInitTuner();		//Ԥ��ʼ��Tuner��������ƽ̨��ʼ��֮ǰ
	
	ErrorCode = CSUDISYSInit();
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	ErrorCode = CSUDIPlatformInit();
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	/* connect demux 0 to tuner 0 */
	ErrorCode = CSUDIDEMUXConnectTuner(0, 0);
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	CSGPIOInitialize();

	ErrorCode = CSI2CInitialize(I2C_0, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);
	ErrorCode = CSI2CInitialize(I2C_1, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);
	ErrorCode = CSI2CInitialize(I2C_2, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);
	ErrorCode = CSI2CInitialize(I2C_3, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);

	CSTCInitTuner();		//��ʼ��Tuner

#if 1


	/* init tuner */
	CSUDITunerInitParam_S sTunerInitParam;
	memset(&sTunerInitParam, 0, sizeof(sTunerInitParam));
	sTunerInitParam.m_eTSOutMode = TS_SERIAL;
	sTunerInitParam.m_eTunerName = EM_UDITUNER_NAME_NONE;
	sTunerInitParam.m_nPriority = 9*17;
	sTunerInitParam.m_uI2CIndex = I2C_0;
	sTunerInitParam.m_uResetGPIONum = 51;
	CSUDITunerRegisterQpsk();
	ErrorCode = CSUDITunerInit(0, &sTunerInitParam);
	CSASSERT(ErrorCode == CSUDI_SUCCESS);
	int nUserData =0x1234;
	CSUDITunerAddCallback(0,Tuner_CallbackTest5,&nUserData);

	CSUDITunerSRCDeliver_U s_unDeliver;
	CSUDITunerSRCDeliver_U * punDeliver = &s_unDeliver;
        punDeliver->m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency = 3880000;
        punDeliver->m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 = 27500;
        punDeliver->m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uModulations_2 = EM_UDITUNER_QPSK;
        punDeliver->m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFECInner_4 = EM_UDITUNER_FEC_AUTO;
        punDeliver->m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uStandards_2 = 0;
        punDeliver->m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uPilot = EM_UDITUNER_PILOT_OFF;
        punDeliver->m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uPolar_4 = EM_UDITUNER_POLARISATION_V;
        punDeliver->m_sSourceSatelliteDeliver.m_sAntennaPar.m_uSwitch22k = EM_UDITUNER_SWITCH_OFF;
        punDeliver->m_sSourceSatelliteDeliver.m_sAntennaPar.m_uSwith12v = EM_UDITUNER_SWITCH_0V;
        punDeliver->m_sSourceSatelliteDeliver.m_sDiSEqC1020.m_uCommitted = EM_UDITUNER_DISEQC10_OFF;   //?
        punDeliver->m_sSourceSatelliteDeliver.m_sDiSEqC1020.m_uRepeatedTimes = 2;   //?
        punDeliver->m_sSourceSatelliteDeliver.m_sDiSEqC1121.m_uUncommitted = 0;    //?
        punDeliver->m_sSourceSatelliteDeliver.m_sTransponderOffset.m_uIQ_1 = 0;
        punDeliver->m_sSourceSatelliteDeliver.m_sLNB.m_uLnbPower = EM_UDITUNER_SWITCH_OFF;
        punDeliver->m_sSourceSatelliteDeliver.m_sLNB.m_uLnbType = EM_UDITUNER_SINGLE;
        punDeliver->m_sSourceSatelliteDeliver.m_sDiSEqC1020.m_uToneburst = EM_UDITUNER_TONEBURST_NONE;        //?
        punDeliver->m_sSourceSatelliteDeliver.m_sLNB.m_uHighFrequency = 0;
        punDeliver->m_sSourceSatelliteDeliver.m_sLNB.m_uLowFrequency= 5150000;
        punDeliver->m_sSourceSatelliteDeliver.m_sLNB.m_uCenterFrequency=0;
        punDeliver->m_sSourceSatelliteDeliver.m_sTransponderOffset.m_uOffsetFrequency_23 = 0;
        punDeliver->m_sSourceSatelliteDeliver.m_sLNB.m_uOptions = 0;
        punDeliver->m_sSourceSatelliteDeliver.m_sLNB.m_uSatCRLNBNo = EM_UDITUNER_UNICABLE_NONE;/*m_uSatCRLNBNo֮ǰû���á�Ϊ��ά��udi�нṹ�岻�䣬���ڰ�����������unicable�豸�����͸�qpsk����xiezhenlei modify 2010.04.07*/
        punDeliver->m_sSourceSatelliteDeliver.m_sLNB.m_uSatCRNo = 0;
	
	CSUDITunerConnect(0, punDeliver),
	CSUDIOSThreadSleep(1000);
#else


	CSQPSKInitializeEx(Tuner_0, I2C_0, 9*17, TS_SERIAL, 51);
	CSQPSKRegisterCallback(Tuner_0, CSQPSK_UT_CallBack);
	LockTuner();			//��Ƶ
#endif
	//CSUDIInitPanel();
//	PlayProgram();		//����
//	while (1);
	CSUDIInitFlash();

}

void TimePrint(void)  //��ӡ��ʼ��ʱ�亯��
{
	int ii =0;

	CSTTPrint("\n\n\n");
	CSTTPrint("*********��ʼ��ӡ����ģ���ʼ������Ҫ��ʱ��*************\n\n");

	for(ii = 0;g_asTimeCost[ii].pModuleName != NULL; ii ++)
	{
		CSTTPrint("*********%s ģ���ʼ������ʱ��: %d ms\r\n", g_asTimeCost[ii].pModuleName,g_asTimeCost[ii].dwTimeCost);
	}
	CSTTPrint("\n\n");

	CSTTPrint("**********************��ӡ����**************************\n\n\n");
}


