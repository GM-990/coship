#include "setup.h"
#include "irdecode.h"
#include "cs_sysdemux.h"
#include "cs_testkit.h"

//#define MCU_PANEL
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


#define 	LMI_SYS_BASE 	0xA4000000
#define 	LMI_SYS_SIZE 	0x08000000
#define 	LMI_VID_BASE 	0xB0000000
#define 	LMI_VID_SIZE 	0x04000000
#define 	MEM_OFFSET_FOR_AV_FW 0x400000
/* CODE_MEMORY_SIZE应与连接时所用的board.mem中所定义的___ramsize大小相同，
其包含了以下几部分的内容:
#-----------------------------------------------------------------------------
# 程序空间的大小,
# 包括:
#      	1 代码段,
#      	2 数据段,
#	3 平台管理的堆和栈空间(system patition, nocache patition),
# 	4 系统管理的堆和栈空间.
#-----------------------------------------------------------------------------
st7100的内存空间定义如下
######################################################################################
    ##
    ## ---------------------------------------
    ##         System Memory Map Definition
    ## --------------------------------------- 
    ##
    ##             SYSTEM MEM DDR (MAX 256M)
    ## 		  +--------------------------+  <--- _RAM_BASE_ADDR_ = 0x84000000
    ##            | 	VIDEO FIRMWARE	     |
    ## 		  +--------------------------+   
    ##            | 	AUDIO FIRMWARE	     |    
    ##            +--------------------------+           
    ##            | 		0x1000	     |
    ##            +--------------------------+ <--- PROGRAM_RAM_START = 0x84401000
    ##  	  |                          |  
    ##            | 	    CODE SPACE	     |
    ##  	  |                          |      
    ##            +--------------------------+ 	
    ##  	  |                          |      
    ##            |         DATA SPACE	     |
    ##  	  |                          |
    ##            +--------------------------+
    ##  	  |                          |        
    ##            | 	    SYSTEM_PATITION  |
    ##  	  |                          |        
    ##            +--------------------------+
    ##            | 	   NoCache Patition  |
    ##            +--------------------------+   
    ##		  | 		System Heap  |
    ##            +--------------------------+   
    ##		  |            System Stack  |
    ##	 -->	  +--------------------------+ <--- SYSTEM_STACK_TOP_OFFSET
    ##  	  |                          |    
    ##            |                          |
    ##            | 		AVMEM0       |
    ##  	  |                          |    
    ##            |                          |
    ## 		  +--------------------------+ <--- _RAM_BASE_ADDR_+DDR0_TOTAL_SIZE
######################################################################################
######################################################################################
    ## ---------------------------------------
    ##         Video Memory Map Definition
    ## --------------------------------------- 
    ##             VID MEM DDR(MAX 256M)
    ## 		  +--------------------------+  <--- DDR1_BASE_ADDR
    ##  	  |                          |    
    ##            |                          |
    ##  	  |                          |    
    ##            |                          |  
    ##  	  |                          |    
    ##            |                          |
    ##            | 		AVMEM1       |
    ##  	  |                          |    
    ##            |                          |
    ##  	  |                          |    
    ##            |                          |    
    ## 		  +--------------------------+ <--- DDR1_BASE_ADDR+DDR1_TOTAL_SIZE
########################################################################################
########################################################################################

*/   
#define 	CODE_MEMORY_SIZE  0x2B00000 /*0x2000000*/
#define 	ROM_BASE 	0xA0000000

//set here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define STB_TYPE_IS_CDVBC9750     /* DDR=2: 128M + 64M,FLASH=8M, TUNER=CABLE   */
#define STB_TYPE_IS_CDVBC8750      /* DDR=1:128M ,FLASH=8M, TUNER=CABLE   */

#define	SYSTEM_SIZE	(0x1A00000)	//(SYSTEM_PATITION_SIZE)
#define	NCACHE_SIZE	(0x80000) 	//(NCACHE_PATITION_SIZE)

static unsigned char s_ucAPPlicationHeap[SYSTEM_SIZE];
static unsigned char s_ucNCacheHeap[NCACHE_SIZE];

//#define TEST_PVR
#ifdef TEST_PVR
#define MEM_DDR_NUMBER 2
#define MEM_CODE_DATA_RAMSIZE 0x3c00000
#undef MEM_OFFSET_FOR_AV_FW
#define MEM_OFFSET_FOR_AV_FW 0x00400000

 #undef LMI_VID_SIZE
 #define LMI_VID_SIZE 0x04000000

#undef CODE_MEMORY_SIZE
#define 	CODE_MEMORY_SIZE 0x2000000

static void iSetupSt710xPorting() 
{
	int ErrorCode = 0;
	CSPanelPinCfg_t sPanelPinCfg;
	BOOL bRe = FALSE;
	CSSYSInfo stSysInfo;
	CSSYSAdvancedParam tmp;	
	CSSYSAdvancedParamEx sSysEx;
	CSSYSMemoryRange memProfile[4];
	int nDDR = 1;
	DWORD dwSH4_CODE_MEMORY_SIZE=0;
	DWORD dwPARTITION0_START=0;
	DWORD dwPARTITION0_STOP	=0;
	DWORD dwPARTITION1_START=0;
	DWORD dwPARTITION1_STOP	=0;

	#ifdef MEM_DDR_NUMBER
		nDDR = MEM_DDR_NUMBER;
	#else
		nDDR = 1;
	#endif

	#ifdef MEM_CODE_DATA_RAMSIZE
	   	dwSH4_CODE_MEMORY_SIZE = MEM_CODE_DATA_RAMSIZE;
	#else
		dwSH4_CODE_MEMORY_SIZE = 0xC00000;
	#endif

	#ifdef MEM_OFFSET_FOR_AV_FW
		dwPARTITION0_START = (0xA4000000 + MEM_OFFSET_FOR_AV_FW + dwSH4_CODE_MEMORY_SIZE);
	#else
		dwPARTITION0_START = (0xA4000000 + 0x00400000 + dwSH4_CODE_MEMORY_SIZE);
	#endif

	//这里配置为2*64M的情况，无VID Memory
		////这里配置为2*64M的情况，无VID Memory
	if  ((LMI_SYS_SIZE == 0x8000000) && (nDDR == 1))
	{
		dwPARTITION0_STOP = (0xA4000000 + 0x04000000 - 4);
		
	    	dwPARTITION1_START= dwPARTITION0_STOP + 4;

		dwPARTITION1_STOP =    ((0xA4000000 + 0x04000000));
	}
	else
	{
		dwPARTITION0_STOP	= (0xA4000000 + 0x08000000);
		if ( nDDR == 2)
		{
			dwPARTITION1_START= 0xB0000000;
			dwPARTITION1_STOP =  0xB0000000+LMI_VID_SIZE;
		}
	}
	
	memProfile[0].m_pStartAddr = (void *)s_ucAPPlicationHeap;
	memProfile[0].m_dwSize = SYSTEM_SIZE;
	memProfile[0].m_eMemoryType = CSSYS_MEM_TYPE_SYSPART;
	
	memProfile[1].m_pStartAddr = (void *)s_ucNCacheHeap;
	memProfile[1].m_dwSize = NCACHE_SIZE;
	memProfile[1].m_eMemoryType = CSSYS_MEM_TYPE_NONCACHE;

	memProfile[2].m_pStartAddr = (void *)dwPARTITION0_START;
	memProfile[2].m_dwSize = dwPARTITION0_STOP - dwPARTITION0_START;
	memProfile[2].m_eMemoryType = CSSYS_MEM_TYPE_AVMEM;

	memProfile[3].m_pStartAddr = (void *)dwPARTITION1_START;
	memProfile[3].m_dwSize = dwPARTITION1_STOP - dwPARTITION1_START;
	memProfile[3].m_eMemoryType = CSSYS_MEM_TYPE_AVMEM;

	CSSYSSetCfg(CSCFG_CSOS_USEPOSIX, CSDEVICE_IGNORE, 0, 0);	
	CSSYSSetCfg(CSCFG_HD_TIMING_MODE,0,CSVID_TIMING_MODE_1080I_50HZ,0);	
	CSSYSSetCfg(CSCFG_SD_TIMING_MODE,0,CSVID_TIMING_MODE_PAL,0);
	
	if (dwPARTITION1_START == dwPARTITION1_STOP)
	{
		CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0],3);
	}
	else
	{
		CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0],4);		
	}

	//boot  system
	 g_dwTimeStart = CSGetTickCount();
	
	bRe = CSSYSChipBoot(0);
    
        g_dwTimeStop = CSGetTickCount();
        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;

	memset(&stSysInfo, 0, sizeof(CSSYSInfo));
	stSysInfo.OuputTVInfo.OutputDevice = CSVID_OUTPUT_CVBS | CSVID_OUTPUT_YUV;		
	stSysInfo.OutputAUDInfo.OutputDevice=CSAUD_OUTPUT_RCA | CSAUD_OUTPUT_HDMI;

	memset(&tmp,0,sizeof(tmp));	
	tmp.m_bDualOSDBuffer 		= TRUE;
	tmp.m_bSupportHDDecoder 		= FALSE;				/*MPEG-2 HD;*/
	tmp.m_bSupportHDDecoder_H264 = FALSE;			/*H.264 HD;*/
	tmp.m_dwMemoryStartAddr = LMI_SYS_BASE;	
	tmp.m_dwMemoryTotalSize =  LMI_SYS_SIZE;/*64M bytes*/

	//CSSYSChipInit(&stSysInfo, (HCSHANDLE)&tmp);	
	memset(&sSysEx,0,sizeof(sSysEx));
	sSysEx.m_stSysAdvancedParam = tmp;
	sSysEx.m_ucPtiNumber = 3;
	sSysEx.m_bPvrSupport = TRUE;

	 g_dwTimeStart = CSGetTickCount();
	 
	CSSYSChipInitEx(&stSysInfo, (HCSHANDLE)&sSysEx);

        g_dwTimeStop = CSGetTickCount();
        g_dwTimeCost +=  (g_dwTimeStop - g_dwTimeStart);
        g_asTimeCost[ModuleIndex].pModuleName = "ChipBoot + ChipInit";
        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

	/* init gpio & i2c */
	ErrorCode = CSGPIOInitialize();
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	ErrorCode |= CSI2CInitialize(I2C_0,CSI2C_SPEED_NORMAL);
	ErrorCode |= CSI2CInitialize(I2C_1,CSI2C_SPEED_NORMAL);
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	/*init tuner*/

	 g_dwTimeStart = CSGetTickCount();
	
	ErrorCode = CSCOFDMInitialize(Tuner_0, 9*17, TS_PARALLEL, 38);
	CSASSERT(ErrorCode == CSHDI_SUCCESS);
	
	if (ErrorCode == CSHDI_SUCCESS)
	{
		g_dwTimeStop = CSGetTickCount();
	        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	        g_asTimeCost[ModuleIndex].pModuleName = "COFDM";
	        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
	}
        
	/* init uart */

	 g_dwTimeStart = CSGetTickCount();
	
	ErrorCode = CSHDIUartSetDefault(0);
	ErrorCode |= CSHDIUartInitial( 1, 115200);
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	if (ErrorCode == CSHDI_SUCCESS)
	{
		g_dwTimeStop = CSGetTickCount();
	        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	        g_asTimeCost[ModuleIndex].pModuleName = "UART";
	        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
	}


	//Initialize panel

	 g_dwTimeStart = CSGetTickCount();

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

	if (ErrorCode == CSHDI_SUCCESS)
	{
		g_dwTimeStop = CSGetTickCount();
	        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	        g_asTimeCost[ModuleIndex].pModuleName = "PANEL";
	        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
	}
	
	CSPanelConfigDisplayTimeDiv(520);
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	{
		CSRMTunerConfig_S sTunerInfo;
		CSRMDemuxConfig_S sDemuxInfo;
		sTunerInfo.m_emTunerType = EM_TUNER_TER;
		CSRMSetTunerConfig(0,&sTunerInfo);
		
		sDemuxInfo.m_nSupportType = DEMUX_SUPPORT_LIVE|DEMUX_SUPPORT_REC| DEMUX_SUPPORT_PLAYBACK;
		sDemuxInfo.m_emLiveChannel = EM_DEMUX_CHANNELCOUNT_1;
		sDemuxInfo.m_emRecordChannel = EM_DEMUX_CHANNELCOUNT_1;
		sDemuxInfo.m_emPlaybackChannel = EM_DEMUX_CHANNELCOUNT_1;
		sDemuxInfo.m_nTunerLinkMark = 3;
		CSRMSetDemuxConfig(0,&sDemuxInfo);
		CSSYSSetPvrSupport(TRUE);

		InitUSPSectionDemux(0);
		InitUSPSectionDemux(1);
		InitUSPSectionDemux(3);
		
		CSTRACE(0xff,"[%s:%d]\n",__FUNCTION__,__LINE__);
		CSSTRGInit();
		CSTRACE(0xff,"[%s:%d]\n",__FUNCTION__,__LINE__);
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
	//ErrorCode = CSEPRInitialize(I2C_3, 0xa0, AT24C64);	
	//CSASSERT(ErrorCode == CSHDI_SUCCESS);

	ErrorCode = CSFLSSetFLSCount(1);
	ErrorCode = CSFLSSetSpace( 0, ROM_BASE, 0x800000);
	
	 g_dwTimeStart = CSGetTickCount();
	
	ErrorCode = CSFLSInitialize(43);//43??
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	if (ErrorCode == CSHDI_SUCCESS)
	{
		g_dwTimeStop = CSGetTickCount();
	        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	        g_asTimeCost[ModuleIndex].pModuleName = "FLASH";
	        g_asTimeCost[ModuleIndex].dwTimeCost = g_dwTimeCost;
	}

	TimePrint(); //打印初始化时间
	
	return;
}
#else
int GetMaxUartNum(void);
static void iSetupSt710xPorting()
{
    BOOL bRe = FALSE;
    CSHDI_Error_t ErrorCode = CSHDI_SUCCESS;
    CSSYSInfo stSysInfo;
    CSSYSAdvancedParam tmp;
    CSPanelPinCfg_t panelCfg;
    CSQAM_Error_t QamInitErrorCode = CSQAM_FAILURE;
    CSSYSMemoryRange memProfile[4];
    int nDDR = 0;
    DWORD dwSH4_CODE_MEMORY_SIZE=0;
    DWORD dwPARTITION0_START=0;
    DWORD dwPARTITION0_STOP	=0;
    DWORD dwPARTITION1_START=0;
    DWORD dwPARTITION1_STOP	=0;

    //**********start: BasicInfraStructure_Init****************************
#ifdef STB_TYPE_IS_CDVBC9750
	nDDR = 2；
#else
#ifdef STB_TYPE_IS_CDVBC8750
    nDDR = 1;/*DDR=128M ,FLASH=8M */
#else	
    nDDR = 2;/*1;成都兴网的硬件是128M+64M字节的内存*/
#endif
#endif
 
    dwSH4_CODE_MEMORY_SIZE = CODE_MEMORY_SIZE;
    dwPARTITION0_START = LMI_SYS_BASE + MEM_OFFSET_FOR_AV_FW + 0x10000 + dwSH4_CODE_MEMORY_SIZE;
    if  ((LMI_SYS_SIZE == 0x8000000) && (nDDR == 1))
    {
		if(dwSH4_CODE_MEMORY_SIZE < 0x04000000)
		{
			/*av memory partition的分配最好不要跨两个64M字节的边界，否则视频会花屏*/
	        	dwPARTITION0_STOP = LMI_SYS_BASE + 0x04000000 - 4;

			dwPARTITION1_START= LMI_SYS_BASE + 0x04000000;

	        	dwPARTITION1_STOP = LMI_SYS_BASE + 0x08000000;
		}
		else
		{
			/*CODE_MEMORY_SIZE如果大于64M,则av memory只需要分配一个partition就ok可*/
	        dwPARTITION0_STOP = LMI_SYS_BASE + LMI_SYS_SIZE;
		}

    }
    else
    {
        dwPARTITION0_STOP	= LMI_SYS_BASE + LMI_SYS_SIZE;
        if ( nDDR == 2)
        {
            dwPARTITION1_START= LMI_VID_BASE;
            dwPARTITION1_STOP =  LMI_VID_BASE+LMI_VID_SIZE;
        }
    }

    memProfile[0].m_pStartAddr = (void *)s_ucAPPlicationHeap;
    memProfile[0].m_dwSize = SYSTEM_SIZE;
    memProfile[0].m_eMemoryType = CSSYS_MEM_TYPE_SYSPART;

    memProfile[1].m_pStartAddr = (void *)s_ucNCacheHeap;
    memProfile[1].m_dwSize = NCACHE_SIZE;
    memProfile[1].m_eMemoryType = CSSYS_MEM_TYPE_NONCACHE;

    memProfile[2].m_pStartAddr = (void *)dwPARTITION0_START;
    memProfile[2].m_dwSize = dwPARTITION0_STOP - dwPARTITION0_START;
    memProfile[2].m_eMemoryType = CSSYS_MEM_TYPE_AVMEM;

    memProfile[3].m_pStartAddr = (void *)dwPARTITION1_START;
    memProfile[3].m_dwSize = dwPARTITION1_STOP - dwPARTITION1_START;
    memProfile[3].m_eMemoryType = CSSYS_MEM_TYPE_AVMEM;

    CSSYSSetCfg(CSCFG_CSOS_USEPOSIX, CSDEVICE_IGNORE, 0, 0);
    CSSYSSetCfg(CSCFG_HD_TIMING_MODE,0,CSVID_TIMING_MODE_1080I_50HZ,0);
    CSSYSSetCfg(CSCFG_SD_TIMING_MODE,0,CSVID_TIMING_MODE_PAL,0);

    if (dwPARTITION1_START == dwPARTITION1_STOP)
    {
        CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0],3);
    }
    else
    {
        CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0],4);
    }
    //boot  system
    g_dwTimeStart = CSGetTickCount();

    bRe = CSSYSChipBoot(0);
    
    g_dwTimeStop = CSGetTickCount();
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
  
    //**********start: application init**********************************
    CSOSGRect Region;
    CSOSGPixelFormat_S g_stGlobalPixelFormat =
        {
            3,3,3,7,10,5,0,15,0x00007C00,0x000003E0,0x0000001F,0x0008000,0,32,2
        };
    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1,g_stGlobalPixelFormat);

    stSysInfo.OutputAUDInfo.OutputDevice=CSAUD_OUTPUT_RCA | CSAUD_OUTPUT_SPDIF | CSAUD_OUTPUT_HDMI;
    stSysInfo.OuputTVInfo.OutputDevice =  CSVID_OUTPUT_CVBS | CSVID_OUTPUT_HDMI | CSVID_OUTPUT_YUV;

    memset(&tmp,0,sizeof(CSSYSAdvancedParam));
    tmp.m_bDualOSDBuffer = TRUE;
    tmp.m_bSupportHDDecoder = TRUE;	//MPEG 2 高清
    tmp.m_bSupportHDDecoder_H264 = TRUE; // .264 高清
    tmp.m_dwMemoryStartAddr = LMI_SYS_BASE;
    tmp.m_dwMemoryTotalSize = LMI_SYS_SIZE;

    /*init chip*/
	
    g_dwTimeStart = CSGetTickCount();
	
    CSSYSChipInit(&stSysInfo, (HCSHANDLE)&tmp);

    g_dwTimeStop = CSGetTickCount();

    g_dwTimeCost +=  (g_dwTimeStop - g_dwTimeStart);
    g_asTimeCost[ModuleIndex].pModuleName = "ChipBoot + ChipInit";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

    //I2C_0 and I2C_1 init
    ErrorCode = CSI2CInitialize(I2C_0, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);

    ErrorCode = CSI2CInitialize(I2C_1, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);

    /* init uart */
    {	
		#ifndef MCU_PANEL 		
        T_ST_Custom_Param param;
        param.dwCommClock = 96000000;
        CSHDIUartSetCustomParam(&param);
		#endif
    	g_dwTimeStart = CSGetTickCount();

        ErrorCode = CSHDIUartInitial(2, 115200);
        CSASSERT(ErrorCode == CSHDI_SUCCESS);
		
	if (ErrorCode == CSHDI_SUCCESS)
	{
		g_dwTimeStop = CSGetTickCount();
	        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	        g_asTimeCost[ModuleIndex].pModuleName = "UART";
	        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
	}

        ErrorCode = CSHDIUartSetDefault(2);
        CSASSERT(ErrorCode == 2);
    }

    Region.left = 0;
    Region.top = 0;
    Region.Width = 1280;
    Region.Height = 720;

    /*init osd*/
    ErrorCode = CSOSGOpen(CSLAYER_GFX1, 0, &Region,  TRUE);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //open GPIO
    CSGPIOOpen(18,CS_GPIO_OUPUT);
    //**********end:application init*************************************

    //*****start: Equipment_Init*****************************************
    //QAM init
    QamInitErrorCode = CSQAMSetDVBMCNSMode(0, DVBMCNS_DVB);

    g_dwTimeStart = CSGetTickCount();

    QamInitErrorCode =CSQAMInitialize(0, 9*17, TS_PARALLEL,0xff);

    CSASSERT(QamInitErrorCode == CSQAM_SUCCESS);

    if(QamInitErrorCode == CSQAM_SUCCESS)
    {
        USPPrint(0xff,"%s %d[ Equipment_Init ]  QamInitErrorCode == CSQAM_SUCCESS\n",__FILE__,__LINE__);

	g_dwTimeStop = CSGetTickCount();
        g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
        g_asTimeCost[ModuleIndex].pModuleName = "QAM";
        g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

    }

    // IR init
    g_dwTimeStart = CSGetTickCount();
	
    UI_IRInitialize(5*17);
	
    g_dwTimeStop = CSGetTickCount();
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[ModuleIndex].pModuleName = "IR";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

    //Initialize panel
    
    g_dwTimeStart = CSGetTickCount();

#ifdef MCU_PANEL
	CSPanelSetType(CSPANEL_CNT_MCU);
	panelCfg.m_ucKeyScanPin = 1;	/*在真待机时，将panelCfg.m_ucKeyScanPin 作为主芯片与单片机通信用的串口索引*/
	//panelCfg.m_pucComPinGroup[0] = 'N';
#else
	CSPanelSetType(CSPANEL_CNT_164_LED);
	panelCfg.m_ucKeyScanPin = 19;   //K11
	panelCfg.m_pucComPinGroup[0] = 20; //Com0
	panelCfg.m_pucComPinGroup[1] = 21; //Com1
	panelCfg.m_pucComPinGroup[2] = 40; //Com2
	panelCfg.m_pucComPinGroup[3] = 22; //Com3
	panelCfg.m_pucComPinGroup[4] = 41; //Com4
	panelCfg.m_pucComPinGroup[5] = 0xff; //Com5
	panelCfg.m_pucSegPinGroup[0] = 23; //PData
	panelCfg.m_pucSegPinGroup[1] = 42; //PClk
#endif

    ErrorCode = CSPanelInitialize(5*17-1, &panelCfg);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    if (ErrorCode == CSHDI_SUCCESS)
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
    
    g_dwTimeStart = CSGetTickCount();
	
    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode = CSFLSSetSpace( 0, ROM_BASE, 0x800000);
    ErrorCode = CSFLSInitialize(43);//43??
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    if (ErrorCode == CSHDI_SUCCESS)
    {
	g_dwTimeStop = CSGetTickCount();
	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
	g_asTimeCost[ModuleIndex].pModuleName = "FLASH";
	g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;
    }

    //**** end: Equipment_Init *************************************

	
     TimePrint(); //打印初始化时间

    return;
}
#endif


void udi_init(void)
{
    
    int ii=0;

    for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
    {	
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
   }

    CSRMSetRMSupport(TRUE);

	iSetupSt710xPorting();

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


