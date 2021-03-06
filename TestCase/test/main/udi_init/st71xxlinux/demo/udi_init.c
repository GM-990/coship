#include "setup.h"
#include "irdecode.h"
#include "cs_testkit.h"
#include "cs_toolset_plus.h"

#include "udi2_tuner.h"
#include "udi2_demux.h"

#include "cs_i2c_plus.h"
#include "cs_gpio_plus.h"
#include "cs_tuner_plus.h"
#include "cs_panel.h"
#include "cs_panel_plus.h"

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

void TimePrint(void) ; //打印初始化时间函数

/*panel keycallback function*/
static void UI_PanelControlUserProc( CSHDICallbackType fnCallBackType , void *pKey )
{
	CSPanelKeyParam_t *pKeyParam;
	pKeyParam = (CSPanelKeyParam_t *)pKey;
	printf("\nkeyvalue:0x%x,keystatus:0x%x\n",pKeyParam->m_ucPanelKeyCode,pKeyParam->m_enmPanelKeyStatus);
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
	
	ErrorCode = CSUDISYSInit();
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	ErrorCode = CSUDIPlatformInit();
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	/* connect demux 0 to tuner 0 */
	ErrorCode = CSUDIDEMUXConnectTuner(0, 0);
	CSASSERT(ErrorCode == CSUDI_SUCCESS);

	CSGPIOInitialize();

	ErrorCode = CSI2CInitialize(I2C_1, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);
	ErrorCode = CSI2CInitialize(I2C_2, CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSI2C_SUCCESS);

	/*init panel */
	memset(&sParams, 0, sizeof(CSUDIPanelParams_S));
	sParams.m_nMySize = sizeof(CSUDIPanelParams_S);
	sParams.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[0] = 92; //stb
	sParams.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[1] = 17; //interrupt
	sParams.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioData = 90;  //data
	sParams.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioClk= 91;    //clk
	sParams.m_ePanelType = EM_PANEL_TYPE_SPI_LED;
	
	ErrorCode = CSUDIPanelInit(0, &sParams);
	CSASSERT(ErrorCode == CSUDI_SUCCESS);
	
	ErrorCode = CSPanelDisplayText("good",4);
	CSASSERT(ErrorCode == CSUDI_SUCCESS);
	
	CSPanelRegisterKeySendCallback(UI_PanelControlUserProc);	   
	CSPanelSetSingleKeyRepeat(0x01, TRUE);//PANEL_UP
	CSPanelSetSingleKeyRepeat(0x04, TRUE);//PANEL_DOWN
	CSPanelSetSingleKeyRepeat(0x10, TRUE);//PANEL_LEFT
	CSPanelSetSingleKeyRepeat(0x20, TRUE);//PANEL_RIGHT

	/* init tuner */
	CSUDITunerInitParam_S sTunerInitParam;
	memset(&sTunerInitParam, 0, sizeof(sTunerInitParam));
	sTunerInitParam.m_eTSOutMode = TS_SERIAL;
	sTunerInitParam.m_eTunerName = EM_UDITUNER_NAME_NONE;
	sTunerInitParam.m_nPriority = 9*17;
	sTunerInitParam.m_uI2CIndex = I2C_2;
	sTunerInitParam.m_uResetGPIONum = 0xff;
	CSUDITunerRegisterQam();
	ErrorCode = CSUDITunerInit(0, &sTunerInitParam);
	CSASSERT(ErrorCode == CSUDI_SUCCESS);


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


