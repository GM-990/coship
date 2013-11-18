#include "setup.h"
#include "irdecode.h"
#include "cs_testkit.h"
#include "udi_init.h"

#ifdef MMCP_UDI2
#include "udi_os.h"
#include "cs_panel_plus.h"
#include "cs_tuner_plus.h"
#include "udi2_panel.h"
#include "cs_gpio_plus.h"
#include "cs_i2c_plus.h"
#include "cs_flash_plus.h"
#include "cs_toolset_plus.h"
#else
#include "cs_sysdemux.h"
#endif

/*For test initialize time */
#define MAX_MODULE_TO_TEST	12
#ifdef MMCP_UDI2
#define MODULE_NAME  "COSHI_INIT"
#define HI3110E_I2C_BUS_INDEX  0
#define HI3110E_IR_PRIORITY  (5*17)
#define HI3110E_FLASH_COUNT  1
#define HI3110E_FLASH_BASE_ADDR  0x1F000000
#define HI3110E_FLASH_SIZE  0x1000000
#define HI3110E_FLASH_VPP_GPIO_INDEX  0x30
#endif

typedef	struct  _InitialTimeCost_t
{
	char   *pModuleName;
	DWORD dwTimeCost;
}InitialTimeCost_t;

DWORD g_dwTimeStart;
DWORD g_dwTimeStop;
DWORD g_dwTimeCost;

int g_nModuleIndex = 0;

InitialTimeCost_t g_asTimeCost[MAX_MODULE_TO_TEST];	

void TimePrint(void) ; //打印初始化时间函数

#ifdef MMCP_UDI2
static void Equipment_Init()
{
    CSHDI_Error_t   ErrorCode = CSHDI_SUCCESS;
    CSUDI_Error_Code nUDIRet;
    CSUDITunerInitParam_S sTunerInitParam;
    CSUDIPanelParams_S sPanelCfgUdi2;
    CSUDIPanelLightValule_S sValue;

    /*GPIO initialization*/
    g_dwTimeStart = CSGetTickCount();
    ErrorCode = CSGPIOInitialize();
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSGPIOInitialize result = %d.\n", __FUNCTION__, ErrorCode);
    g_dwTimeStop = CSGetTickCount();
  
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    if(g_nModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
    g_asTimeCost[g_nModuleIndex].pModuleName = "GPIOInitialize";
    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;

    /*I2C initialization*/
    g_dwTimeStart = CSGetTickCount();
    ErrorCode = CSI2CInitialize(HI3110E_I2C_BUS_INDEX, CSI2C_SPEED_NORMAL);
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSI2CInitialize Index (%d)  result = %d.\n", __FUNCTION__, HI3110E_I2C_BUS_INDEX, ErrorCode);
    g_dwTimeStop = CSGetTickCount();
  
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    if(g_nModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
    g_asTimeCost[g_nModuleIndex].pModuleName = "I2CInitialize";
    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;

    /*Tuner initialization*/
    g_dwTimeStart = CSGetTickCount();	
    memset(&sTunerInitParam, 0, sizeof(sTunerInitParam));

    sTunerInitParam.m_eTSOutMode = TS_PARALLEL;
    sTunerInitParam.m_nPriority = 9*17;
    sTunerInitParam.m_uI2CIndex = Tuner_0;
    sTunerInitParam.m_uResetGPIONum = 0xff;

    nUDIRet = CSUDITunerRegisterQam();
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSUDITunerRegisterQam result = 0x%08x.\n", __FUNCTION__, nUDIRet);

    nUDIRet = CSUDITunerInit(0, &sTunerInitParam);
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s] CSUDITunerInit result = 0x%08x.\n", __FUNCTION__, nUDIRet);
    g_dwTimeStop = CSGetTickCount();
  
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    if(g_nModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
    g_asTimeCost[g_nModuleIndex].pModuleName = "TunerInitialize";
    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;

    /*panel initialization and display "good".*/
    g_dwTimeStart = CSGetTickCount();
    
    sPanelCfgUdi2.m_nMySize = sizeof(CSUDIPanelParams_S);
    sPanelCfgUdi2.m_ePanelType = EM_PANEL_TYPE_COMMON_LED;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[0] = 13;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[1] = 12;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[2] = 11;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[3] = 10;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[4] = 14;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[5] = 0xff;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioData = 8;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioClk = 9;
    sPanelCfgUdi2.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioKey = 15;	

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
    if(g_nModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
    g_asTimeCost[g_nModuleIndex].pModuleName = "PanelInitialize";
    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;

    /*IR initialization */	
    g_dwTimeStart = CSGetTickCount();
    UI_IRInitialize(HI3110E_IR_PRIORITY);
    g_dwTimeStop = CSGetTickCount();
  
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    if(g_nModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
    g_asTimeCost[g_nModuleIndex].pModuleName = "IRInitialize";
    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;
		
    /*Flash initialization */
    g_dwTimeStart = CSGetTickCount();
	
    CSFLSSetFLSCount(HI3110E_FLASH_COUNT);
    CSFLSSetSpace( 0, HI3110E_FLASH_BASE_ADDR, HI3110E_FLASH_SIZE);
    
    ErrorCode = CSFLSInitialize(HI3110E_FLASH_VPP_GPIO_INDEX);
    CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[%s]CSFLSInitialize result = %d.\n", __FUNCTION__, ErrorCode);
    g_dwTimeStop = CSGetTickCount();
  
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    if(g_nModuleIndex > (MAX_MODULE_TO_TEST-1)) return;
    g_asTimeCost[g_nModuleIndex].pModuleName = "FlashInitialize";
    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;

    return;

}

static void iSetupHi3110ePorting()
{
    CSUDI_BOOL bRet;
    CSUDI_Error_Code nStatus;
		
    bRet = CSUDISYSInit();
    CSASSERT(bRet == CSUDI_SUCCESS);

    bRet = CSUDIPlatformInit();
    CSASSERT(bRet == CSUDI_SUCCESS);
		
    CSDebugSetDefaultLevel(ERROR_LEVEL);
		
    Equipment_Init();

    if(g_nModuleIndex >= MAX_MODULE_TO_TEST)
    {
        CSTTPrint("*********没有完成所有模块的初始化!!*************\n");
        return;
    }
		
    nStatus = CSUDIDEMUXConnectTuner(0,0);
    CSASSERT(nStatus == CSUDI_SUCCESS);

    TimePrint();  //打印初始化时间	
}
#endif

void udi_init()
{
    int ii=0;

	for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
	{	
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
	}
	
#ifdef MMCP_UDI2	
	iSetupHi3110ePorting();
	
#else
    CSHDI_Error_t   ErrorCode;
    CSPanelPinCfg_t sPanelPinCfg;
    HCSHANDLE hChipHandle = 0;

    /*chenyingle edit from argb1555 to argb8888,hi3110e use argb8888.osg test pictures would init base on this.*/
    /*CSOSGPixelFormat_S stPixelFormat = {3,3,3,7,10,5,0,15,0x00007C00,0x000003E0,0x0000001F,0x0008000,0,16,2};*/
    CSOSGPixelFormat_S stPixelFormat = {0,0,0,0,16,8,0,24,0x00FF00,0x0000FF00,0x000000FF,0xFF00000,0,32,4};
    
    CSOSGRect Region = {0,0,720,576};
    CSRMSetRMSupport(TRUE);

    g_dwTimeStart = CSGetTickCount();

    if (!CSSYSChipBoot(0) )
    {
        CSDebug("TestMain",FATAL_LEVEL, "[%s.%d]: Fail at CSSYSChipBoot\n", __FILE__, __LINE__);
        return ;
    }
	
    hChipHandle = CSSYSChipInit(0, 0);
    if ( 0 == hChipHandle )
    {
        CSDebug("TestMain",FATAL_LEVEL, "[%s.%d]: Fail at CSSYSChipInit\n", __FILE__, __LINE__);
        CSSYSChipHalt(0);
        return ;
    }

    g_dwTimeStop = CSGetTickCount();

    CSTTPrint("\n^^^^^^^^^^^^^^^^start:%d\r\n", g_dwTimeStart);
    CSTTPrint("\n^^^^^^^^^^^^^^^^stop:%d\r\n", g_dwTimeStop);

	
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[g_nModuleIndex].pModuleName = "ChipBoot + ChipInit";
    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;

    {
        int i;
        int cnt = CSRMGetDeviceCounts(EM_RESOURCE_DEMUX);
        CSDebug("TestMain",INFO_LEVEL,"Init Demux Cnt = %d\r\n",cnt);
        for(i=0;i<cnt;i++)
        {
            InitUSPSectionDemux(i);
        }
    }

    /* init gpio & i2c */
    ErrorCode = CSGPIOInitialize();
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
	
    ErrorCode = CSI2CInitialize(0, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CS_GPIO_SUCCESS);
    USPPrint(INFO_LEVEL, "\n[Equipment_Init] CSI2CInitialize ret = %d.\n", ErrorCode);

    ErrorCode = CSQAMInitialize(Tuner_0, 9*17, TS_PARALLEL, 0xff);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //Initialize panel
    sPanelPinCfg.m_ucKeyScanPin = 3;   //K11
    sPanelPinCfg.m_pucComPinGroup[0] = 6; //Com0
    sPanelPinCfg.m_pucComPinGroup[1] = 10; //Com1
    sPanelPinCfg.m_pucComPinGroup[2] = 8; //Com2
    sPanelPinCfg.m_pucComPinGroup[3] = 9; //Com3
    sPanelPinCfg.m_pucComPinGroup[4] = 0; //Com4
    sPanelPinCfg.m_pucComPinGroup[5] = 0xff; //Com5
    sPanelPinCfg.m_pucSegPinGroup[0] = 2; //PData
    sPanelPinCfg.m_pucSegPinGroup[1] = 1; //PClk

    g_dwTimeStart = CSGetTickCount();
	
    ErrorCode = CSPanelInitialize(159, &sPanelPinCfg);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    if (ErrorCode == CSHDI_SUCCESS)
    {
	    g_dwTimeStop = CSGetTickCount();
	    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	    g_asTimeCost[g_nModuleIndex].pModuleName = "PANEL";
	    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;
    }

    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    // IR init

    g_dwTimeStart = CSGetTickCount();

    UI_IRInitialize(5*17);

    g_dwTimeStop = CSGetTickCount();
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[g_nModuleIndex].pModuleName = "IR";
    g_asTimeCost[g_nModuleIndex++].dwTimeCost = g_dwTimeCost;

    /* init EEPROM & FLASH */
    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode |= CSFLSSetSpace( 0, 0x1F00000, 0x1000000);

    g_dwTimeStart = CSGetTickCount();

    ErrorCode |= CSFLSInitialize(30);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    if (ErrorCode == CSHDI_SUCCESS)
    {
	    g_dwTimeStop = CSGetTickCount();
	    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	    g_asTimeCost[g_nModuleIndex].pModuleName = "FLASH";
	    g_asTimeCost[g_nModuleIndex].dwTimeCost = g_dwTimeCost;
    }

    CSOSGInit(0);
    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);
    CSOSGOpen(0,0,&Region,TRUE);

    TimePrint();//打印初始化时间
#endif 
}//end iSetupBcm7309Porting()

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



