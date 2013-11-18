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


#define MAX_MODULE_TO_TEST	12
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

typedef  unsigned long int U32;
typedef U32 ST_ErrorCode_t;
extern ST_ErrorCode_t PIO_FE_Reset(BOOL OnOff);
extern U32 ST_GetClocksPerSecond(void);
#define Task_Delay(x)   task_delay(x)

void TimePrint(void) ; //打印初始化时间函数


void CSQPSK_UT_CallBack(CSHDICallbackType type , void * pt )
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
	Tuner预初始化，在平台初始化之前调用
*/
static void PerInitTuner()
{
	/* set HW config for N8000 ST7162 board (YMB.A083.B) */
	CSUDISYSSetCfg(CSUDICFG_TUNER_MODE, 0/*tuner 0*/, TS_SERIAL, 0);
	CSUDISYSSetCfg(CSUDICFG_TUNER_TSIN, 0/*tuner 0*/, 0 /* TSIN 0 */, 0);
	CSUDISYSSetCfg(CSUDICFG_TSIN_DMX, 0/*TSIN 0*/, 0 /* Demux 0 */, 0);
}

/*
	Tuner初始化
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
	Task_Delay(ST_GetClocksPerSecond()/1000);
	PIO_FE_Reset(FALSE);
	Task_Delay(ST_GetClocksPerSecond()/1000);
	PIO_FE_Reset(TRUE);
	Task_Delay(ST_GetClocksPerSecond()/100);

	CSQPSKInitializeEx(Tuner_0, I2C_0, 9*17, TS_SERIAL, 51);
	CSQPSKRegisterCallback(Tuner_0, CSQPSK_UT_CallBack);
}

/*锁频*/
static void LockTuner()
{
	CSQPSKSignalParameterS2 pCSQpskParam;
	memset(&pCSQpskParam, 0, sizeof(CSQPSKSignalParameterS2));
	
	/*指定锁频参数*/
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
	pCSQpskParam.dwFreqency= 3750000;		//频点
	pCSQpskParam.dwLNBLowFreq = 5150000;		//本振
	pCSQpskParam.dwSymbolRate = 10490;		//符号率

	CSQPSKConnectSignalS2(Tuner_0, &pCSQpskParam);
	CSSleep(3000);
}

/*
	播放
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

	while(1)
	{
		ErrorCode = CSUDIPLAYEROpen(&psPlayerChnl, ePlayerType, &hPlayer);
		if (CSUDI_SUCCESS != ErrorCode || hPlayer == NULL)
		{
			printf("________CSUDIPLAYEROpen FAIL_________\r\n");
		}

		if ((i%2) == 0)
		{
			aStreamInfo[0].m_nPid = 257;
			aStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			aStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

			aStreamInfo[1].m_nPid = 258;
			aStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			aStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;

			i = 1;
		}
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

		CSUDIOSThreadSleep(1000*3);

		if (CSUDI_SUCCESS  != CSUDIPLAYERClose (hPlayer))
		{
			CSTCPrint("Failed to close a player.\n");
			return CSUDI_FAILURE;
		}

	}

}

void udi_init(void)
{
	int ii=0;
	CSUDI_Error_Code ErrorCode = CSUDI_SUCCESS;

	CSDebugSetDefaultLevel(INFO_LEVEL);

	for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
	{
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
	}

	PerInitTuner();		//预初始化Tuner，必须在平台初始化之前

	ErrorCode = CSUDISYSInit();
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	ErrorCode = CSUDIPlatformInit();
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	CSTCInitTuner();		//初始化Tuner

	CSGPIOInitialize();

	/*此处OK*/

	/*I2C_0已经在平台里面初始化了*/	
	//ErrorCode = CSI2CInitialize(I2C_0, CSI2C_SPEED_NORMAL);
	ErrorCode = CSI2CInitialize(I2C_1, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);
	ErrorCode = CSI2CInitialize(I2C_2, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);
	ErrorCode = CSI2CInitialize(I2C_3, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);

//	LockTuner();			//锁频
//	PlayProgram();		//播放
}


void TimePrint(void)  //打印初始化时间函数
{
	int ii =0;

	CSTTPrint("\n\n\n");
	CSTTPrint("*********开始打印各个模块初始化所需要的时间*************\n\n");

	for(ii = 0;g_asTimeCost[ii].pModuleName != NULL; ii ++)
	{
		CSTTPrint("*********%s 模块初始化所需时间: %d ms\r\n", g_asTimeCost[ii].pModuleName,g_asTimeCost[ii].dwTimeCost);
	}
	CSTTPrint("\n\n");

	CSTTPrint("**********************打印结束**************************\n\n\n");
}


