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

void udi_init()
{
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
    g_asTimeCost[ModuleIndex].pModuleName = "ChipBoot + ChipInit";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
    
    //lili modified 
    //CSDebugSet must be called after CSSYSChipInit.
    CSDebugSet("CSGraphics",NOTICE_LEVEL);
    CSDebugSet("EventManager",CLI_LEVEL);
    CSDebugSet("CoshipFS",WARN_LEVEL);
    CSDebugSet("SiCache", WARN_LEVEL);
    CSDebugSet("CoshipFS-DSM", WARN_LEVEL);
    CSDebugSet("TestMain", DEBUG_LEVEL);
    CSDebugSet("TestKitCase", CLI_LEVEL);
    CSDebugSet("TSMONITOR",INFO_LEVEL);
    CSDebugSet("CSGraphics",ERROR_LEVEL);

    CSDebugSet("dhcp",0);
    CSDebugSet("memleak",INFO_LEVEL);
    CSDebugSet("mediaplayer",INFO_LEVEL);

    CSDebugSet("cs_qam",INFO_LEVEL);
    CSDebug("TestMain",DEBUG_LEVEL, "[main]: TestKit Starting.............\n");
    //end

    /*************************************************************
    RM的配置，逻辑上的配置，直接copy就可以了，避免程序运行失败
    *************************************************************/
    CSRMTunerConfig_S sTunerInfo;
    CSRMDemuxConfig_S sDemuxInfo;
    CSRMSetRMSupport(TRUE);
    sTunerInfo.m_emTunerType = EM_TUNER_CAB;
    CSRMSetTunerConfig(0,&sTunerInfo);
	 
    sDemuxInfo.m_nSupportType = DEMUX_SUPPORT_LIVE|DEMUX_SUPPORT_REC| DEMUX_SUPPORT_PLAYBACK;
    sDemuxInfo.m_emLiveChannel = EM_DEMUX_CHANNELCOUNT_1;
    sDemuxInfo.m_emRecordChannel = EM_DEMUX_CHANNELCOUNT_1;
    sDemuxInfo.m_emPlaybackChannel = EM_DEMUX_CHANNELCOUNT_1;
    sDemuxInfo.m_nTunerLinkMark = 3;
    CSRMSetDemuxConfig(0,&sDemuxInfo);
    InitUSPSectionDemux(0);

    
#if 0
    {
        int i;
        int cnt = CSRMGetDeviceCounts(EM_RESOURCE_DEMUX);
        CSDebug("TestMain",INFO_LEVEL,"Init Demux Cnt = %d\r\n",cnt);
        for(i=0;i<cnt;i++)
        {
            InitUSPSectionDemux(i);
        }
    }
#endif

    CSDebugSetDefaultLevel(DEBUG_LEVEL);


    /* init gpio & i2c */
    ErrorCode = CSGPIOInitialize();
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //I2C init
    {
        CSHDI_Error_t eErrCode = CSHDI_SUCCESS;
        CSI2C_SPEED_t emI2CSpeed;
        emI2CSpeed = CSI2C_SPEED_NORMAL;

        //just initialize I2C_0 
        CSI2CInitialize(I2C_0, emI2CSpeed);
        CSASSERT(eErrCode == CSI2C_SUCCESS);
        USPPrint(INFO_LEVEL, "\n[Equipment_Init] CSI2CInitialize ret = %d.\n", ErrorCode);
    }

    //QAM initialize
    ErrorCode = CSQAMInitializeEx(0, I2C_0, 159, TS_PARALLEL, 12);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
    USPPrint(INFO_LEVEL, "\n[Equipment_Init] CSQAMInitializeEx ret = %d.\n", ErrorCode);

    /*************************************************************************
    Linux cs_os中的timer初始化
    **************************************************************************/
    CSInitTimer();

#if 0 //lili modified. on zoran supraxd180,we did not need initialize panel diver
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
#endif
    g_dwTimeStart = CSGetTickCount();
	
    ErrorCode = CSPanelInitialize(159, &sPanelPinCfg);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    if (ErrorCode == CSHDI_SUCCESS)
    {
	    g_dwTimeStop = CSGetTickCount();
	    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	    g_asTimeCost[ModuleIndex].pModuleName = "PANEL";
	    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
    }

    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    // IR init

    g_dwTimeStart = CSGetTickCount();

    //lili removed. we do not need do IR initialize
    //UI_IRInitialize(5*17);

    g_dwTimeStop = CSGetTickCount();
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[ModuleIndex].pModuleName = "IR";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

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
	    g_asTimeCost[ModuleIndex].pModuleName = "FLASH";
	    g_asTimeCost[ModuleIndex].dwTimeCost = g_dwTimeCost;
    }

    CSOSGInit(0);
    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);
    CSOSGOpen(0,0,&Region,TRUE);

    TimePrint();//打印初始化时间
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



