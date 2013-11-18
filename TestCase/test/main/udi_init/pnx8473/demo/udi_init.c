#include "cs_flash_plus.h"
#include "cs_panel_plus.h"
#include "cs_tuner_plus.h"
#include "cs_gpio_plus.h"
#include "cs_i2c_plus.h"
#include "cs_eeprom_plus.h"
#include "udi2_panel.h"
#include "cs_toolset_plus.h"
#include "udi2_fs.h"
#include "udiplus_debug.h"
#define MODULE_NAME  "COSHI_INIT"
#define CNXT24501_TUNER_I2C_INDEX 	2
#define CNXT24501_TUNER_TSMODE 	TS_SERIAL
#define CNXT24501_EEPROM_I2C_INDEX	1
#define CNXT24501_EEPROM_I2C_ADDR	0xA0
#define CNXT24501_EEPROM_TYPE	AT24C64

#define MAX_MODULE_TO_TEST	12
/*For test initialize time */
extern void UI_IRInitialize( int nPriority );
static void Panel_init(void);

static void Equipment_Init()
{

	CSHDI_Error_t   ErrorCode = CSHDI_SUCCESS;
	CSUDI_Error_Code nUDIRet;
	CSUDITunerInitParam_S sTunerInitParam;
	CSUDIPanelParams_S sPanelCfgUdi2;
	CSUDIPanelLightValule_S sValue;
/*
	g_dwTimeStart = CSGetTickCount();
	ErrorCode = CSGPIOInitialize();
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSGPIOInitialize result = %d.\n", __FUNCTION__, ErrorCode);
	g_dwTimeStop = CSGetTickCount();
  
  	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	if(ModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
  	g_asTimeCost[ModuleIndex].pModuleName = "GPIOInitialize";
  	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
*/
	ErrorCode = CSI2CInitialize(CNXT24501_TUNER_I2C_INDEX, CSI2C_SPEED_NORMAL);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSI2CInitialize Index (%d) result = %d.\n", __FUNCTION__, CNXT24501_TUNER_I2C_INDEX, ErrorCode);
/*

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
 */
	memset(&sTunerInitParam, 0, sizeof(sTunerInitParam));

	sTunerInitParam.m_eTSOutMode = CNXT24501_TUNER_TSMODE;
	sTunerInitParam.m_nPriority = 156;
	sTunerInitParam.m_uI2CIndex = CNXT24501_TUNER_I2C_INDEX;
	sTunerInitParam.m_uResetGPIONum = 0xff;

	nUDIRet = CSUDITunerRegisterQam();
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSUDITunerRegisterQam result = 0x%08x.\n", __FUNCTION__, nUDIRet);

	nUDIRet = CSUDITunerInit(0, &sTunerInitParam);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s] CSUDITunerInit result = 0x%08x.\n", __FUNCTION__, nUDIRet);

#if 0
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
#elif 0
	CSFlashPartition_S stPartion[]=
	{
		{"un-test",0x0,0x6c00000,128*1024*20},
		{"test1",0x6c00000,0xa00000,128*1024*20},	
		{"test2",0x7600000,0xa00000,128*1024*20}
	};
	
	CSFLSSetFLSCount(1);
	ErrorCode = CSFLSSetSpace(0,0x0,0x8000000);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSFLSSetSpace result = %d.\n", __FUNCTION__, ErrorCode);
	ErrorCode = CSFLSInitialize(0xff);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSFLSInitialize result = %d.\n", __FUNCTION__, ErrorCode);
	ErrorCode = CSFLSSetPartition(stPartion, sizeof(stPartion)/sizeof(CSFlashPartition_S));
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSFLSSetPartition result = %d.\n", __FUNCTION__, ErrorCode);
#endif
	printf("Panel & IR Init\n");
	Panel_init();
	UI_IRInitialize(1*17);
	return;
}
static void Panel_init(void)
{
	CSUDIPanelParams_S sPinCfg;
	CSUDIPanelLightValule_S sShowInform;
	CSUDIPanelTextValue_S sTextValue;
	CSUDIPanelLightValule_S sValue;
	CSUDI_Error_Code nUDIRet;
	
	memset(&sPinCfg, 0, sizeof(CSUDIPanelParams_S));
	//这里初始化GPIO口，成功初始化以后就可以正常显示了。
	sPinCfg.m_nMySize = sizeof(CSUDIPanelParams_S);
	sPinCfg.m_ePanelType = EM_PANEL_TYPE_SPI_LED;
	sPinCfg.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[0] = 7;
	sPinCfg.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[1] = 27;
	sPinCfg.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioData = 63;
	sPinCfg.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioClk = 64;

	CSUDIPanelInit(0, &sPinCfg );
	
	
	sValue.m_eLightName = EM_UDIPANEL_TEXT0;
	sValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
	memcpy(sValue.m_uValue.m_sTextValue.m_szStr, "good", sizeof("good"));
	sValue.m_uValue.m_sTextValue.m_nStrCount = sizeof("good");

	nUDIRet = CSUDIPanelDisplay(0, &sValue);
	CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s] CSUDIPanelDisplay result = 0x%08x.\n", __FUNCTION__, nUDIRet);
}
static void iSetuppnx8473Porting()
{

	CSUDI_BOOL bRet;
	CSUDI_Error_Code nStatus;
		
	bRet = CSUDISYSInit();
	CSASSERT(bRet == CSUDI_SUCCESS);
		
	bRet = CSUDIPlatformInit();
	CSASSERT(bRet == CSUDI_SUCCESS);
	Equipment_Init();

    bRet = CSUDIFSInit();
    CSASSERT(bRet == CSUDI_SUCCESS);
	
	//CSUDIFSAddPartition("/dev/mtdblock4",EM_UDIFS_PARTITION_JFFS2);
	
	nStatus = CSUDIDEMUXConnectTuner(0,0);
	CSASSERT(nStatus == CSUDI_SUCCESS);
	
	nStatus = CSUDIDEMUXConnectTuner(0,1);
	CSASSERT(nStatus == CSUDI_SUCCESS);	
}

void udi_init(void)
{
	iSetuppnx8473Porting();	
}



