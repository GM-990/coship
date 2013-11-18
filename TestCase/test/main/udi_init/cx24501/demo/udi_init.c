#include "setup.h"
#include "cs_testkit.h"
#include "udi_init.h"
#include "cs_flash_plus.h"
#include "cs_panel_plus.h"
#include "cs_tuner_plus.h"
#include "cs_gpio_plus.h"
#include "cs_i2c_plus.h"
#include "cs_eeprom_plus.h"
#include "udi2_panel.h"
#include "udi_os.h"
#include "irdecode.h"
#include "cs_toolset_plus.h"

#define MODULE_NAME  "COSHI_INIT"
#define CNXT24501_TUNER_I2C_INDEX 	2
#define CNXT24501_TUNER_TSMODE 	TS_PARALLEL
#define CNXT24501_EEPROM_I2C_INDEX	1
#define CNXT24501_EEPROM_I2C_ADDR	0xA0
#define CNXT24501_EEPROM_TYPE	AT24C64

#define MAX_MODULE_TO_TEST	12
/*For test initialize time */

typedef	struct  _InitialTimeCost_t
{
	char   *pModuleName;
	DWORD dwTimeCost;
}InitialTimeCost_t;

DWORD g_dwTimeStart;
DWORD g_dwTimeStop;
DWORD g_dwTimeCost;

int ModuleIndex = 0;

InitialTimeCost_t g_asTimeCost[MAX_MODULE_TO_TEST];	

void TimePrint(void) ; //打印初始化时间函数

//extern void UI_PanelControlUserProc(CSHDICallbackType tCallbackType, void *param);

static void Equipment_Init()
{
	CSHDI_Error_t   ErrorCode = CSHDI_SUCCESS;
	CSUDI_Error_Code nUDIRet;
	CSUDITunerInitParam_S sTunerInitParam;
	CSUDIPanelParams_S sPanelCfgUdi2;
	CSUDIPanelLightValule_S sValue;

	g_dwTimeStart = CSGetTickCount();
	ErrorCode = CSGPIOInitialize();
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSGPIOInitialize result = %d.\n", __FUNCTION__, ErrorCode);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "GPIOInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
  
	g_dwTimeStart = CSGetTickCount();
	ErrorCode = CSI2CInitialize(CNXT24501_TUNER_I2C_INDEX, CSI2C_SPEED_NORMAL);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSI2CInitialize Index (%d) result = %d.\n", __FUNCTION__, CNXT24501_TUNER_I2C_INDEX, ErrorCode);

	ErrorCode = CSI2CInitialize(CNXT24501_EEPROM_I2C_INDEX, CSI2C_SPEED_NORMAL);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSI2CInitialize Index (%d)  result = %d.\n", __FUNCTION__, CNXT24501_EEPROM_I2C_INDEX, ErrorCode);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "I2CInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

	g_dwTimeStart = CSGetTickCount();
	CSFLSSetFLSCount(1);
	ErrorCode = CSFLSInitialize(0xff);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSFLSInitialize result = %d.\n", __FUNCTION__, ErrorCode);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "FlashInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

	g_dwTimeStart = CSGetTickCount();	
	ErrorCode = CSEPRInitialize(CNXT24501_EEPROM_I2C_INDEX , CNXT24501_EEPROM_I2C_ADDR, CNXT24501_EEPROM_TYPE);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSEPRInitialize result = %d.\n", __FUNCTION__, ErrorCode);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "EEPROMInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;	
	memset(&sTunerInitParam, 0, sizeof(sTunerInitParam));

	sTunerInitParam.m_eTSOutMode = CNXT24501_TUNER_TSMODE;
	sTunerInitParam.m_nPriority = 159;
	sTunerInitParam.m_uI2CIndex = CNXT24501_TUNER_I2C_INDEX;
	sTunerInitParam.m_uResetGPIONum = 0xff;

	nUDIRet = CSUDITunerRegisterQam();
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSUDITunerRegisterQam result = 0x%08x.\n", __FUNCTION__, nUDIRet);

	nUDIRet = CSUDITunerInit(0, &sTunerInitParam);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s] CSUDITunerInit result = 0x%08x.\n", __FUNCTION__, nUDIRet);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "TunerInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

	g_dwTimeStart = CSGetTickCount();
	UI_IRInitialize(5*17);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "IRInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

	g_dwTimeStart = CSGetTickCount();
	 /*panel initialization and display "good".*/
	sPanelCfgUdi2.m_nMySize = sizeof(CSUDIPanelParams_S);
	sPanelCfgUdi2.m_ePanelType = EM_PANEL_TYPE_COMMON_LED;
	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[0] = 181;
	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[1] = 180;
	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[2] = 186;
	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[3] = 182;
	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[4] = 183;
	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[5] = 0xff;
	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioData = 9;
	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioClk = 8;

	sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioKey = 96;

	nUDIRet = CSUDIPanelInit(0, &sPanelCfgUdi2);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s] CSUDIPanelInit result = 0x%08x.\n", __FUNCTION__, nUDIRet);

	sValue.m_eLightName = EM_UDIPANEL_TEXT0;
	sValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
	memcpy(sValue.m_uValue.m_sTextValue.m_szStr, "good", sizeof("good"));
	sValue.m_uValue.m_sTextValue.m_nStrCount = sizeof("good");

	nUDIRet = CSUDIPanelDisplay(0, &sValue);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s] CSUDIPanelDisplay result = 0x%08x.\n", __FUNCTION__, nUDIRet);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "PanelInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

	g_dwTimeStart = CSGetTickCount();
	UI_IRInitialize(5*17);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "IRInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
	
	return;
}

static void iSetupCx24501Porting()
{
	CSUDI_BOOL bRet;
	CSUDI_Error_Code nStatus;
		
	bRet = CSUDISYSInit();
	CSASSERT(bRet == CSUDI_SUCCESS);
		
	bRet = CSUDIPlatformInit();
	CSASSERT(bRet == CSUDI_SUCCESS);
		
	CSDebugSetDefaultLevel(ERROR_LEVEL);
		
	Equipment_Init();

	if(ModuleIndex >= MAX_MODULE_TO_TEST)
	{
		CSTTPrint("*********没有完成所有模块的初始化!!*************\n");
		return;
	}
		
	nStatus = CSUDIDEMUXConnectTuner(0,0);
	CSASSERT(nStatus == CSUDI_SUCCESS);
	
	nStatus = CSUDIDEMUXConnectTuner(0,1);
	CSASSERT(nStatus == CSUDI_SUCCESS);	
    TimePrint();  //打印初始化时间	
}

void udi_init(void)
{
	int ii=0;

	for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
	{	
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
	}
	
	iSetupCx24501Porting();
	
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


