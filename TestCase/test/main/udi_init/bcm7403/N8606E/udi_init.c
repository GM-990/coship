#include "setup.h"
#include "irdecode.h"
#include "cs_sysdemux.h"
#include "cs_testkit.h"
#include "udi_init.h"


/*For test initialize time */

#define MAX_MODULE_TO_TEST	12

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

/*udi_init is used to init porting,and different hardware config has defferent version of udi_int*/
void udi_init()
{
    CSSYSAdvancedParamEx ParamEx;
    HCSHANDLE hChipHandle = 0;
    CSHDI_Error_t   ErrorCode;
    CSPanelPinCfg_t sPanelPinCfg;
    CSRMTunerConfig_S sTunerInfo;
    CSRMDemuxConfig_S sDemuxInfo;

    CSOSGPixelFormat_S stPixelFormat = {0,0,0,0,16,8,0,24,0x00ff0000,0x0000ff00,0x000000ff,0xff000000,0,32,4};
    CSOSGRect Region = {0,0,720,576};
    /* init tuner & demux*/
    sTunerInfo.m_emTunerType = EM_TUNER_CAB;
    CSRMSetTunerConfig(0,&sTunerInfo);

    sDemuxInfo.m_nSupportType = DEMUX_SUPPORT_LIVE|DEMUX_SUPPORT_REC| DEMUX_SUPPORT_PLAYBACK;
    sDemuxInfo.m_emLiveChannel = EM_DEMUX_CHANNELCOUNT_1;
    sDemuxInfo.m_emRecordChannel = EM_DEMUX_CHANNELCOUNT_1;
    sDemuxInfo.m_emPlaybackChannel = EM_DEMUX_CHANNELCOUNT_1;
    sDemuxInfo.m_nTunerLinkMark = 3;
    CSRMSetDemuxConfig(0,&sDemuxInfo);

    /* init chip driver */
#ifdef _COSHIP_PVR_

    CSSYSSetPvrSupport(TRUE);
#else

CSSYSSetPvrSupport(FALSE);
#endif

    CSSYSSetCfg(CSCFG_TUNER_TSIN, CSTUNER_0, CSTSIN_0, 0);
    CSSYSSetCfg(CSCFG_TUNER_TSIN, CSTUNER_1, CSTSIN_1, 0);
    CSSYSSetCfg(CSCFG_TSIN_DMX, CSTSIN_0, CSDMX_0, 0);
    CSSYSSetCfg(CSCFG_TSIN_DMX, CSTSIN_1, CSDMX_1, 0);
    CSSYSSetCfg(CSCFG_TUNER_MODE, CSTUNER_0, CSSYS_TS_SERIAL, 0);
    CSSYSSetCfg(CSCFG_TUNER_MODE, CSTUNER_1, CSSYS_TS_SERIAL, 0);

    CSSYSSetCfg(CSCFG_TSIN_DMX, CSTSIN_7, CSDMX_2, 0);

    g_dwTimeStart = CSGetTickCount();
    if (!CSSYSChipBoot(0) )
    {
        CSDebug("TestMain",FATAL_LEVEL, "[%s.%d]: Fail at CSSYSChipBoot\n", __FILE__, __LINE__);
        return ;
    }

    ParamEx.m_dwReserve = 0;//m_dwReserve=0表示不支持nds ca， 为1支持nds ca,仅适用于7403平台
    hChipHandle = CSSYSChipInitEx(NULL, (HCSHANDLE) &ParamEx);
    if ( 0 == hChipHandle )
    {
        CSDebug("TestMain",FATAL_LEVEL, "[%s.%d]: Fail at CSSYSChipInit\n", __FILE__, __LINE__);
        CSSYSChipHalt(0);
        return ;
    }

    g_dwTimeStop = CSGetTickCount();
//    CSTTPrint("\n^^^^^^^^^^^^^^^^start:%d\r\n", g_dwTimeStart);
//    CSTTPrint("\n^^^^^^^^^^^^^^^^stop:%d\r\n", g_dwTimeStop);
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[ModuleIndex].pModuleName = "ChipBoot + ChipInit";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

    {
        int i;
        int cnt = CSRMGetDeviceCounts(EM_RESOURCE_DEMUX);
        for(i=0;i<cnt;i++)
        {
            InitUSPSectionDemux(i);
        }
    }

    /* init gpio & i2c */
    ErrorCode = CSGPIOInitialize();
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    ErrorCode |= CSI2CInitialize(I2C_0,CSI2C_SPEED_NORMAL);
    ErrorCode |= CSI2CInitialize(I2C_1,CSI2C_SPEED_NORMAL);
    ErrorCode |= CSI2CInitialize(I2C_2,CSI2C_SPEED_NORMAL);
    ErrorCode |= CSI2CInitialize(I2C_3,CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    /* init uart */
    g_dwTimeStart = CSGetTickCount();
    
    ErrorCode = CSHDIUartSetDefault(0);
    ErrorCode |= CSHDIUartInitial( 2, 9600);
    
     g_dwTimeStop = CSGetTickCount();
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
    if(ErrorCode == CSHDI_SUCCESS)
    {
        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
        g_asTimeCost[ModuleIndex].pModuleName = "UART";
        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
    }
    
   /* init tuner */
     g_dwTimeStart = CSGetTickCount();
   
    ErrorCode = CSQAMInitializeEx(CSTUNER_0, I2C_2, 159, TS_SERIAL, 0xff);
    
    g_dwTimeStop = CSGetTickCount();
    
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
    if(ErrorCode == CSHDI_SUCCESS)
    {
        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
        g_asTimeCost[ModuleIndex].pModuleName = "QAM";
        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
    }
    
    /* init panel */
    g_dwTimeStart = CSGetTickCount();
    
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

     g_dwTimeStop = CSGetTickCount();

     CSASSERT(ErrorCode == CSHDI_SUCCESS);
    if(ErrorCode == CSHDI_SUCCESS)
    {
        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
        g_asTimeCost[ModuleIndex].pModuleName = "Panel";
        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
    }
    //CSPanelRegisterKeySendCallback((CSHDICallback)UI_PanelControlUserProc);

    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    // IR init

     g_dwTimeStart = CSGetTickCount();
    
    UI_IRInitialize(5*17);
    g_dwTimeStop = CSGetTickCount();

    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[ModuleIndex].pModuleName = "IR";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

    /* init EEPROM & FLASH */

    
    ErrorCode = CSEPRInitialize(I2C_3, 0xa0, AT24C64);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
    
    g_dwTimeStart = CSGetTickCount();
    
    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode |= CSFLSSetSpace( 0, 0X1E000000, 0x2000000);
    ErrorCode |= CSFLSInitialize(13);

     g_dwTimeStop = CSGetTickCount();
     
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
     if(ErrorCode == CSHDI_SUCCESS)
    {
        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
        g_asTimeCost[ModuleIndex].pModuleName = "FLASH";
        g_asTimeCost[ModuleIndex].dwTimeCost = g_dwTimeCost;
    }

    CSOSGInit(0);
    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);
    CSOSGOpen(0,0,&Region,TRUE);

    TimePrint();//打印初始化时间

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



