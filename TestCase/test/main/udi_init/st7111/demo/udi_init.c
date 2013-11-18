#include <os21.h>
#include "setup.h"
#include "irdecode.h"
#include "cs_sysdemux.h"
#include "cs_testkit.h"
#include "cs_cofdm.h"
#include "cs_sysav.h"


#include "memory_cfg.h"
#include "cs_qpsk.h"
#include "cs_panel_plus.h"

#define ST7197_N8760C

#define GPIO_FP_CLK         (91)
#define GPIO_FP_DATA        (90)
#define GPIO_FP_CS          (92)
#define GPIO_KEY_INT        (17)

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

static void iSetupSt71xxPorting()
{

    CSHDI_Error_t ErrorCode = CSHDI_SUCCESS;
    CSSYSInfo stSysInfo;
    CSSYSAdvancedParamEx stAdvaceParamEX;
    CSOSGRect RegionSDOSD,RegionHDOSD;
    

    CSSYSMemoryRange memProfile[] = {
        {CSSYS_MEM_TYPE_SYSPART,			(void*)SYSTEM_PARTITIOAN_START_ADDR,	SYSTEM_PARTITIOAN_SIZE, 	},/* user heap */
        {CSSYS_MEM_TYPE_NONCACHE, 		(void*)NCACHE_PARTITIOAN_START_ADDR,	NCACHE_PARTITIOAN_SIZE, },/* non-cached */
        {CSSYS_MEM_TYPE_OS, 						(void*)OS_PARTITIOAN_START_ADDR,				OS_PARTITIOAN_SIZE, 		},/* os */
        {CSSYS_MEM_TYPE_DRIVER, 				(void*)DRIVER_PARTITIOAN_START_ADDR,		DRIVER_PARTITIOAN_SIZE, },/* driver */
        {CSSYS_MEM_TYPE_AVMEM_USER, 	(void*)AVMEM_USER_START_ADDR, 				AVMEM_USER_SIZE,				},	/* user osd avmem */
        {CSSYS_MEM_TYPE_AVMEM,				(void*)AVMEM_SYS_START_ADDR,					AVMEM_SYS_SIZE, 				},	/* avmem sys */
        {CSSYS_MEM_TYPE_AVMEM,				(void*)AVMEM_VID_START_ADDR,					AVMEM_VID_SIZE, 				},	/* avmem vid */
    } ;

    int memRangeCount = sizeof(memProfile)/sizeof(CSSYSMemoryRange);

    /* 如果无 video memory， memRangeCount减一 */
    if (AVMEM_VID_SIZE == 0)
    {
        memRangeCount--;
    }
     
    CSDebugSetDefaultLevel(ERROR_LEVEL);//CSSetDebugLevel(0);

    /* 配内存 */
    CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0], memRangeCount);

    /* 配置初始化时的制式 */
    CSSYSSetCfg(CSCFG_HD_TIMING_MODE, 0, CSVID_TIMING_MODE_1080I_50HZ, 0);
    CSSYSSetCfg(CSCFG_SD_TIMING_MODE, 0, CSVID_TIMING_MODE_PAL, 0);

    //boot	system
    g_dwTimeStart = CSGetTickCount();

    if (!CSSYSChipBoot(0))
    {
        CSASSERT(0);
    }
   
    g_dwTimeStop = CSGetTickCount();
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
 
    memset(&stSysInfo, 0, sizeof(CSSYSInfo));

#if (HD_DISPLAY_SUPPORT == 1 )

    stSysInfo.OutputAUDInfo.OutputDevice=CSAUD_OUTPUT_RCA | CSAUD_OUTPUT_SPDIF | CSAUD_OUTPUT_HDMI;
    stSysInfo.OuputTVInfo.OutputDevice =	CSVID_OUTPUT_CVBS | CSVID_OUTPUT_HDMI|CSVID_OUTPUT_YUV;
#else

	stSysInfo.OutputAUDInfo.OutputDevice=CSAUD_OUTPUT_RCA | CSAUD_OUTPUT_SPDIF ;
	stSysInfo.OuputTVInfo.OutputDevice =	CSVID_OUTPUT_CVBS | CSVID_OUTPUT_YUV;
#endif

    memset(&stAdvaceParamEX, 0, sizeof(CSSYSAdvancedParamEx));
    stAdvaceParamEX.m_stSysAdvancedParam.m_bDualOSDBuffer = HD_DISPLAY_SUPPORT;
    stAdvaceParamEX.m_stSysAdvancedParam.m_bSupportHDDecoder = HD_DECODER_SUPPORT;
    stAdvaceParamEX.m_stSysAdvancedParam.m_bSupportHDDecoder_H264 = HD_H264_SUPPORT;
    stAdvaceParamEX.m_stSysAdvancedParam.m_dwMemoryStartAddr = LMI_SYS_BASE;
    stAdvaceParamEX.m_stSysAdvancedParam.m_dwMemoryTotalSize = LMI_SYS_SIZE;
    stAdvaceParamEX.m_bPvrSupport = FALSE;
    stAdvaceParamEX.m_bPipSupport = FALSE;
    stAdvaceParamEX.m_ucPtiNumber = 1;

    /*init chip*/

    g_dwTimeStart = CSGetTickCount();

    if ((HCSHANDLE)NULL == CSSYSChipInitEx(&stSysInfo, (HCSHANDLE)&stAdvaceParamEX))
    {
        CSASSERT(0);
    }

    g_dwTimeStop = CSGetTickCount();
    g_dwTimeCost += ( g_dwTimeStop - g_dwTimeStart);
    g_asTimeCost[ModuleIndex].pModuleName = "ChipBoot + ChipInit";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

    RegionSDOSD.left = 0;
    RegionSDOSD.top = 0;
    RegionSDOSD.Width = 720;
    RegionSDOSD.Height = 576;

    RegionHDOSD.left = 0;
    RegionHDOSD.top = 0;
    RegionHDOSD.Width = 1920;
    RegionHDOSD.Height = 1080;

    /*init osd*/
    ErrorCode = CSOSGOpen(CSLAYER_GFX2, 0, &RegionSDOSD,  TRUE);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    ErrorCode = CSOSGOpen(CSLAYER_GFX1, 0, &RegionHDOSD,  TRUE);

    //GPIO init ,actually init in Plat,here for UT;Gary
    ErrorCode =  CSGPIOInitialize();
    CSASSERT(ErrorCode == CSI2C_SUCCESS);

   
#if defined(ST7197_N8760C)
	//I2C_0 and I2C_1  init	for st7197 N8760C
    ErrorCode = CSI2CInitialize(I2C_0, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);
    ErrorCode = CSI2CInitialize(I2C_1, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);
#endif
	 //I2C_2 and I2C_3 init	st7111
    ErrorCode = CSI2CInitialize(I2C_2, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);
    ErrorCode = CSI2CInitialize(I2C_3, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);

    g_dwTimeStart = CSGetTickCount();
#if defined(ST_7111)
    ErrorCode = CSQPSKInitialize( 0, 6*17, TS_PARALLEL, 0xff);
    CSASSERT(ErrorCode == CSQPSK_SUCCESS);
#endif
#if defined(ST_7105)
    ErrorCode =  CSCOFDMInitializeEx(Tuner_0, I2C_2, 9*17,  TS_PARALLEL, 50);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
#endif
#if defined(ST7197_N8760C)
	ErrorCode = CSQAMInitializeEx(Tuner_0, I2C_2, 9*17, TS_PARALLEL, 27);
#endif

    if (ErrorCode == CSQPSK_SUCCESS)
    {
	    g_dwTimeStop = CSGetTickCount();
	    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	    g_asTimeCost[ModuleIndex].pModuleName = "QPSK";
	    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
	    	
    }
    g_dwTimeStart = CSGetTickCount();

    UI_IRInitialize(5*17);

    g_dwTimeStop = CSGetTickCount();
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[ModuleIndex].pModuleName = "IR";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

#if defined(ST7197_N8760C)
    CSUDIPanelParams_S sParams; 	
    sParams.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[0] = GPIO_FP_CS; //stb
    sParams.m_ePanelHwCfg.m_sLEDCfg.m_aucGpioComPin[1] = GPIO_KEY_INT; //interrupt
    sParams.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioData = GPIO_FP_DATA;  //data
    sParams.m_ePanelHwCfg.m_sLEDCfg.m_ucGpioClk= GPIO_FP_CLK;    //clk
    sParams.m_ePanelType = EM_PANEL_TYPE_SPI_LED;
    sParams.m_nMySize = sizeof(CSUDIPanelParams_S);
    ErrorCode = CSUDIPanelInit(0, &sParams);
    CSASSERT(ErrorCode == CSUDI_SUCCESS);
#elif defined (ST_7111)||defined(ST_7105)
    CSPanelPinCfg_t panelCfg ;
    panelCfg.m_ucKeyScanPin = 26 ;	//K11
    panelCfg.m_pucComPinGroup[0] = 14 ;//Com0
    panelCfg.m_pucComPinGroup[1] = 13 ;//Com1
    panelCfg.m_pucComPinGroup[2] = 12 ;//Com2
    panelCfg.m_pucComPinGroup[3] = 11;//Com3
    panelCfg.m_pucComPinGroup[4] = 30 ;//Com4
    panelCfg.m_pucComPinGroup[5] = 0xff; //Com5
    panelCfg.m_pucSegPinGroup[0] = 28; //PData
    panelCfg.m_pucSegPinGroup[1] = 29; //PClk
    ErrorCode = CSPanelInitialize(159, &panelCfg);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
#endif

    
    if (ErrorCode == CSQPSK_SUCCESS)
    {
	    g_dwTimeStop = CSGetTickCount();
	    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	    g_asTimeCost[ModuleIndex].pModuleName = "PANEL";
	    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
	    	
    }

    //display "good"
    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //Initalize flash
    ErrorCode = CSFLSSetFLSCount(1);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

#if defined(ST_7111)
    ErrorCode = CSFLSSetSpace( 0, 0xA0000000, 0x800000);
    ErrorCode = CSFLSInitialize(46);
#elif defined(ST7197_N8760C)
    ErrorCode = CSFLSSetSpace( 0, 0xA0000000, 0x2000000);
    ErrorCode = CSFLSInitialize(0xff);
#elif
    ErrorCode = CSFLSSetSpace( 0, 0xA0000000, 0x800000);
    ErrorCode = CSFLSInitialize(57);
#endif
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    if (ErrorCode == CSQPSK_SUCCESS)
    {
	    g_dwTimeStop = CSGetTickCount();
	    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	    g_asTimeCost[ModuleIndex].pModuleName = "FLASH";
	    g_asTimeCost[ModuleIndex].dwTimeCost = g_dwTimeCost;
	    	
    }

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

	CSRMSetRMSupport(TRUE);

	iSetupSt71xxPorting();
	
    return;


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


