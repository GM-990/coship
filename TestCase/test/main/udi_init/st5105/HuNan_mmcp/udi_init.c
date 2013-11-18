#include "setup.h"
#if 0 /*<!-- yinbenqing 2011/7/6 17:48:33 */

#include "irdecode.h"
#include "cs_sysdemux.h"
#include "cs_testkit.h"

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


#define SYSTEM_MEMORY_SIZE_5105   (0x1300000 + 0xc00000) 		/* 28 Mb */
#define NCACHE_MEMORY_SIZE_5105   (0x0080000) 		/* 0.5 Mb */

static unsigned char   SystemMemory[SYSTEM_MEMORY_SIZE_5105];
static unsigned char   NcacheMemory[NCACHE_MEMORY_SIZE_5105];

#pragma ST_section (SystemMemory,   "system_section")
#pragma ST_section (NcacheMemory,  "ncache_section")

extern void UI_IRInitialize( int nPriority );
int TcpIpInit(void);

void CSPortingInit(void)
{
	CSSYSInfo stSysInfo;	
	CSSYSMemoryRange memProfile[4];
	CSOSGPixelFormat_S stPixelFormat = {0,0,0,0,16,8,0,24,0x00ff0000,0x0000ff00,0x000000ff,0xff000000,0,32,4};
	CSSYSAdvancedParam tmp;	

	memProfile[0].m_pStartAddr = (void *)SystemMemory;
	memProfile[0].m_dwSize = SYSTEM_MEMORY_SIZE_5105;
	memProfile[0].m_eMemoryType = CSSYS_MEM_TYPE_SYSPART;
	
	memProfile[1].m_pStartAddr = (void *)NcacheMemory;
	memProfile[1].m_dwSize = NCACHE_MEMORY_SIZE_5105;
	memProfile[1].m_eMemoryType = CSSYS_MEM_TYPE_NONCACHE;

	CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);

	CSRMSetRMSupport(FALSE);
	
	CSSYSSetCfg(CSCFG_SD_TIMING_MODE,0,CSVID_TIMING_MODE_PAL,0);
	
	CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0],2);
	
	//boot  system
	if (!CSSYSChipBoot(0))
	{
		CSASSERT(0);
	}
	 
	memset(&stSysInfo, 0, sizeof(CSSYSInfo));
	
	stSysInfo.OuputTVInfo.OutputDevice	= CSVID_OUTPUT_CVBS | CSVID_OUTPUT_YUV;	
	stSysInfo.OutputAUDInfo.OutputDevice=CSAUD_OUTPUT_RCA | CSAUD_OUTPUT_SPDIF ;

	memset(&tmp,0,sizeof(tmp));
	
	tmp.m_bDualOSDBuffer = TRUE;
	
	tmp.m_bSupportHDDecoder = TRUE; 
	
	tmp.m_bSupportHDDecoder_H264 = FALSE;
	
	CSSYSChipInit(&stSysInfo, (HCSHANDLE)&tmp);	
}

BOOL EquipmentInit(void)
{
    CSHDI_Error_t ErrorCode = CSHDI_SUCCESS;

    CSPanelPinCfg_t sPanelPinCfg;
    int gpioIndex = 14;

    /* init gpio & i2c */
    ErrorCode = CSGPIOInitialize();
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
    CSGPIOOpen(9, 2);
    CSGPIOWrite(9, 0);
    ErrorCode |= CSI2CInitialize(I2C_0, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    /* init uart */
    ErrorCode = CSHDIUartInitial( 1, 115200);
    ErrorCode |= CSHDIUartSetDefault(1);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    /* init tuner */
    ErrorCode = CSQAMInitializeEx( 0, I2C_0, 6 * 17, TS_SERIAL, 10 );
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    /* init panel */
    sPanelPinCfg.m_ucKeyScanPin = 28;   //K11
    sPanelPinCfg.m_pucComPinGroup[0] = 26; //Com0
    sPanelPinCfg.m_pucComPinGroup[1] = 18; //Com1
    sPanelPinCfg.m_pucComPinGroup[2] = 19; //Com2
    sPanelPinCfg.m_pucComPinGroup[3] = 21; //Com3
    sPanelPinCfg.m_pucComPinGroup[4] = 22; //Com4
    sPanelPinCfg.m_pucComPinGroup[5] = 0xff; //Com4
    sPanelPinCfg.m_pucSegPinGroup[0] = 29; //PData
    sPanelPinCfg.m_pucSegPinGroup[1] = 27; //PClk
    ErrorCode = CSPanelInitialize(2 * 17, &sPanelPinCfg);


    // IR init
    UI_IRInitialize(5*17);

    //CSPanelRegisterKeySendCallback((CSHDICallback)UI_PanelControlUserProc);

    //display "good"
    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    /*
    	CSBL_SetFlashEntireSize(FLASH_BASE_ADDRESS, FLASH_BUFFER_SIZE);
    	CSBL_SetSysinfoAddr(FLSADDR_SYSINFO);
    	CSBL_BlsysappInit();
    */
    TimePrint(); //打印初始化时间
    
    return TRUE;
}


BOOL ChipInit_st5105(void)
{
	CSPortingInit();

    EquipmentInit();

	//TcpIpInit();
	
    return TRUE;
}



void udi_init(void)
{
    CSHDI_Error_t   ErrorCode;
    CSPanelPinCfg_t sPanelPinCfg;
    int ii=0;

    for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
    {	
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
   }

    CSRMSetRMSupport(TRUE);

#if defined(PLATFORM_ST40)
#if defined(st7100)||defined(st7109)
	iSetupSt710xPorting();
	
#elif defined(st7111)
	iSetupSt71xxPorting();
	
#else /* defined(st7100)||defined(st7109) */
#error unknown PLATFORM_ST40!!!
#endif /*defined(st7100)||defined(st7109)*/
    return;

#elif defined(PLATFORM_ST20)

    ChipInit_st5105();
	
#elif defined(PLATFORM_BCM740X)

    iSetupBcm740xPorting();
    return;
#elif defined(PLATFORM_BCM7309)

    iSetupBcm7309Porting();

    return;
#elif defined(PLATFORM_X86)

    ChipInit_linux();
#endif

    /* init gpio & i2c */
    ErrorCode = CSGPIOInitialize();
    CSASSERT(ErrorCode == CSHDI_SUCCESS);


#if defined(PLATFORM_ST20)

    CSGPIOOpen(9, 2);
    CSGPIOWrite(9, 0);
#endif

#if defined(PLATFORM_ST20)

    ErrorCode |= CSI2CInitialize(I2C_0,CSI2C_SPEED_NORMAL);
#else

    ErrorCode |= CSI2CInitialize(I2C_0,CSI2C_SPEED_NORMAL);
    ErrorCode |= CSI2CInitialize(I2C_1,CSI2C_SPEED_NORMAL);
    ErrorCode |= CSI2CInitialize(I2C_2,CSI2C_SPEED_NORMAL);
    ErrorCode |= CSI2CInitialize(I2C_3,CSI2C_SPEED_NORMAL);

#endif

    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    /* init uart */
#if defined(PLATFORM_ST20)

    ErrorCode = CSHDIUartSetDefault(1);
#else

    ErrorCode = CSHDIUartSetDefault(0);
#endif

    ErrorCode |= CSHDIUartInitial( 1, 115200);

    CSASSERT(ErrorCode == CSHDI_SUCCESS);


    /* init tuner */
#if defined(PLATFORM_ST20)

    ErrorCode = CSQAMInitializeEx( 0, I2C_0, 6*17, TS_SERIAL, 10 );
#elif defined(PLATFORM_ST40)
#elif defined(PLATFORM_BCM740X)

    ErrorCode = CSQAMInitializeEx(CSTUNER_0, I2C_2, 159, TS_SERIAL, 0xff);
#else

    ErrorCode = CSQAMInitialize(Tuner_0, 9*17, TS_PARALLEL, 38);
#endif

    CSASSERT(ErrorCode == CSHDI_SUCCESS);

#if 0

    TestTuner();

    while(1)
    {
        CSSleep(100);
    }
#endif

    CSASSERT(ErrorCode == CSHDI_SUCCESS);
#if defined(PLATFORM_ST40)
    //Initialize panel
    sPanelPinCfg.m_ucKeyScanPin = 19;   //K11
    sPanelPinCfg.m_pucComPinGroup[0] = 20; //Com0
    sPanelPinCfg.m_pucComPinGroup[1] = 21; //Com1
    sPanelPinCfg.m_pucComPinGroup[2] = 40; //Com2
    sPanelPinCfg.m_pucComPinGroup[3] = 22; //Com3
    sPanelPinCfg.m_pucComPinGroup[4] = 41; //Com4
    sPanelPinCfg.m_pucComPinGroup[5] = 0xff; //Com5
    sPanelPinCfg.m_pucSegPinGroup[0] = 23; //PData
    sPanelPinCfg.m_pucSegPinGroup[1] = 42; //PClk
    ErrorCode = CSPanelInitialize(159, &sPanelPinCfg);
    CSPanelConfigDisplayTimeDiv(520);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
#elif defined(PLATFORM_ST20)

    sPanelPinCfg.m_ucKeyScanPin = 28;   //K11
    sPanelPinCfg.m_pucComPinGroup[0] = 26; //Com0
    sPanelPinCfg.m_pucComPinGroup[1] = 18; //Com1
    sPanelPinCfg.m_pucComPinGroup[2] = 19; //Com2
    sPanelPinCfg.m_pucComPinGroup[3] = 21; //Com3
    sPanelPinCfg.m_pucComPinGroup[4] = 22; //Com4
    sPanelPinCfg.m_pucComPinGroup[5] = 0xff; //Com4
    sPanelPinCfg.m_pucSegPinGroup[0] = 29; //PData
    sPanelPinCfg.m_pucSegPinGroup[1] = 27; //PClk

    CSPanelInitialize(5*17, &sPanelPinCfg) ;
#else
    /* init panel */
    sPanelPinCfg.m_ucKeyScanPin = 18;   //K11
    sPanelPinCfg.m_pucComPinGroup[0] = 21; //Com0
    sPanelPinCfg.m_pucComPinGroup[1] = 24; //Com1
    sPanelPinCfg.m_pucComPinGroup[2] = 20; //Com2
    sPanelPinCfg.m_pucComPinGroup[3] = 23; //Com3
    sPanelPinCfg.m_pucComPinGroup[4] = 19; //Com4
    sPanelPinCfg.m_pucComPinGroup[5] = 0xff; //Com5
    sPanelPinCfg.m_pucSegPinGroup[0] = 37; //PData
    sPanelPinCfg.m_pucSegPinGroup[1] = 25; //PClk
    ErrorCode = CSPanelInitialize(220, &sPanelPinCfg);
#endif

    //CSPanelRegisterKeySendCallback((CSHDICallback)UI_PanelControlUserProc);

    ErrorCode = CSPanelDisplayText("good", 4);

    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    // IR init
    UI_IRInitialize(5*17);

    /* init EEPROM & FLASH */
    //ErrorCode = CSEPRInitialize(I2C_3, 0xa0, AT24C64);

    CSASSERT(ErrorCode == CSHDI_SUCCESS);

#if defined(PLATFORM_ST20)

    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode |= CSFLSSetSpace( 0, 0x40000000, 0x800000);
    ErrorCode |= CSFLSInitialize(14);
#elif defined(PLATFORM_ST40)

    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode = CSFLSSetSpace( 0, 0xA0000000, 0x800000);
    ErrorCode = CSFLSInitialize(43);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
#elif  defined(PLATFORM_X86)

    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode |= CSFLSSetSpace( 0, 0xC0000000, 0x800000);
    ErrorCode |= CSFLSInitialize(14);
#else

    ErrorCode = CSFLSSetFLSCount(1);
    //ErrorCode |= CSFLSSetSpace( 0, FLASH_BASE_ADDRESS, FLASH_BUFFER_SIZE);
    ErrorCode |= CSFLSInitialize(13);
#endif

    CSASSERT(ErrorCode == CSHDI_SUCCESS);

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
#else
void udi_init(void)
{

}

#endif /*0*//* yinbenqing 2011/7/6 17:48:33 --!>*/
