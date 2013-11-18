#include "setup.h"
#include "irdecode.h"
#include "cs_hdicommon_plus.h"
#include "cs_panel.h"
#include "cs_testkit.h"
#include "udi_init.h"
#include "cs_tuner_plus.h"

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

    BrcmPlat_init();
    CSUDIOSGInit();
    CSUDIDEMUXInit();
    CSUDIFILTERInit();
    CSUDIAUDIOInit();
    CSUDIVIDEOInit();
    CSUDIAOUTInit();
    CSUDIDSMInit();
    CSUDIINJECTERInit();
    CSUDIPLAYERInit();
    CSUDISCREENInit();
    CSUDIFSInit();
       
    {
        CSUDI_Error_Code errCode = CSUDI_FAILURE;
        CSUDITunerInitParam_S sTunerInitParam;
        
        errCode = CSUDITunerRegisterQam();
        memset(&sTunerInitParam, 0, sizeof(sTunerInitParam));
           
        sTunerInitParam.m_eTSOutMode = TS_PARALLEL;
        sTunerInitParam.m_nPriority = 159;
        sTunerInitParam.m_uI2CIndex = I2C_2;//no use of this value, just for complie,xiezhenlei add.
        sTunerInitParam.m_uResetGPIONum = 0xff;//0xff means there is no hardware reset for demodulator bcm3109. xiezhenlei add.
           
        if ( CSUDITunerInit(0, &sTunerInitParam) != CSUDI_SUCCESS)
        {
            printf("%s CSUDITunerInit failed. \n",__FUNCTION__);
        }	
        else	
        {
            printf("%s CSUDITunerInit successed. \n",__FUNCTION__);
        }
    }

#if 0
    CSHDI_Error_t   ErrorCode;
    CSPanelPinCfg_t sPanelPinCfg;
    HCSHANDLE hChipHandle = 0;
	
    //CSOSGPixelFormat_S stPixelFormat = {3,3,3,7,10,5,0,15,0x00007C00,0x000003E0,0x0000001F,0x0008000,0,16,2};
    //CSOSGRect Region = {0,0,720,576};

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

    ErrorCode = CSI2CInitialize( 0, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    ErrorCode = CSQPSKInitialize(Tuner_0, 9*17, TS_PARALLEL, 15);
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
	    g_asTimeCost[ModuleIndex].pModuleName = "PANEL";
	    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
    }

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
    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode |= CSFLSSetSpace( 0, 0xbf000000, 0x1000000);

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
    //CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);
    //CSOSGOpen(0,0,&Region,TRUE);
#endif
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



