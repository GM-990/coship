#include "setup.h"
#include "irdecode.h"
#include "cs_sysdemux.h"
#include "cs_testkit.h"
//extern void UI_PanelControlUserProc(CSHDICallbackType tCallbackType, void *param);
#if defined(PLATFORM_ST20)

#define SYSTEM_MEMORY_SIZE_5105   (0x600000+0x800000)  		/* 14 Mb */
#define NCACHE_MEMORY_SIZE_5105   (0x0080000) 		/* 0.5 Mb */
/* Private Function prototypes ---------------------------------------- */

/* Global ----------------------------------------------------------------- */

/* Private Variables -------------------------------------------------- */

static unsigned char   SystemMemory[SYSTEM_MEMORY_SIZE_5105];
static unsigned char   NcacheMemory[NCACHE_MEMORY_SIZE_5105];

#pragma ST_section (SystemMemory,   "system_section")
#pragma ST_section (NcacheMemory ,  "ncache_section")
BOOL ChipInit_st5105(void)
{
    CSSYSInfo stSysInfo;
    CSSYSMemoryRange memProfile[4];
    int ErrorCode;

    CSOSGPixelFormat_S stPixelFormat = {0,0,0,0,16,8,0,24,0x00ff0000,0x0000ff00,0x000000ff,0xff000000,0,32,4};

    CSSYSAdvancedParam tmp;

    CSOSGRect stOSGRegion;

    memProfile[0].m_pStartAddr = (void *)SystemMemory;
    memProfile[0].m_dwSize = SYSTEM_MEMORY_SIZE_5105;
    memProfile[0].m_eMemoryType = CSSYS_MEM_TYPE_SYSPART;

    memProfile[1].m_pStartAddr = (void *)NcacheMemory;
    memProfile[1].m_dwSize = NCACHE_MEMORY_SIZE_5105;
    memProfile[1].m_eMemoryType = CSSYS_MEM_TYPE_NONCACHE;

    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);

    CSSYSSetCfg(CSCFG_CSOS_USEPOSIX, CSDEVICE_IGNORE, 1,0);

    CSSYSSetCfg(CSCFG_HD_TIMING_MODE,0,CSVID_TIMING_MODE_1080I_50HZ,0);

    CSSYSSetCfg(CSCFG_SD_TIMING_MODE,0,CSVID_TIMING_MODE_PAL,0);

    CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0],2);

    //boot  system
    if (!CSSYSChipBoot(0))
    {
        CSASSERT(0);
    }

    memset(&stSysInfo, 0, sizeof(CSSYSInfo));

    stSysInfo.OuputTVInfo.OutputDevice	= CSVID_OUTPUT_CVBS | CSVID_OUTPUT_HDMI | CSVID_OUTPUT_YUV;

    stSysInfo.OutputAUDInfo.OutputDevice=CSAUD_OUTPUT_RCA | CSAUD_OUTPUT_SPDIF | CSAUD_OUTPUT_HDMI;

    memset(&tmp,0,sizeof(tmp));

    tmp.m_bDualOSDBuffer = TRUE;

    tmp.m_bSupportHDDecoder = TRUE;

    tmp.m_bSupportHDDecoder_H264 = FALSE;


    CSSYSChipInit(&stSysInfo, (HCSHANDLE)&tmp);

    return TRUE;
}
#endif

#if defined(PLATFORM_ST40)

#if defined(st7100)||defined(st7109)

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
#define 	CODE_MEMORY_SIZE  0x5000000 /*0x2000000*/
#define 	ROM_BASE 	0xA0000000

//#define STB_TYPE_IS_CDVBC9750

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

		dwPARTITION1_STOP =    ((0xA4000000 + LMI_SYS_SIZE));
	}
	else
	{
		dwPARTITION0_STOP	= (0xA4000000 + LMI_SYS_SIZE);
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
	bRe = CSSYSChipBoot(0);

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
	CSSYSChipInitEx(&stSysInfo, (HCSHANDLE)&sSysEx);

	/* init gpio & i2c */
	ErrorCode = CSGPIOInitialize();
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	ErrorCode |= CSI2CInitialize(I2C_0,CSI2C_SPEED_NORMAL);
	ErrorCode |= CSI2CInitialize(I2C_1,CSI2C_SPEED_NORMAL);
	ErrorCode |= CSI2CInitialize(I2C_2,CSI2C_SPEED_NORMAL);
	ErrorCode |= CSI2CInitialize(I2C_3,CSI2C_SPEED_NORMAL);	
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	/*init tuner*/
	ErrorCode = CSCOFDMInitialize(Tuner_0, 9*17, TS_PARALLEL, 38);
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	/* init uart */
	ErrorCode = CSHDIUartSetDefault(0);
	ErrorCode |= CSHDIUartInitial( 1, 115200);
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

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
	UI_IRInitialize(5*17);	

	/* init EEPROM & FLASH */	
	//ErrorCode = CSEPRInitialize(I2C_3, 0xa0, AT24C64);	
	//CSASSERT(ErrorCode == CSHDI_SUCCESS);

	ErrorCode = CSFLSSetFLSCount(1);
	ErrorCode = CSFLSSetSpace( 0, ROM_BASE, 0x800000);
	ErrorCode = CSFLSInitialize(43);//43??
	CSASSERT(ErrorCode == CSHDI_SUCCESS);
			
	return;
}
#else
/************************************************
 *说明：测试的盒子是CDVBC970
 *这个设置是对应于这个盒子的设置
 * DDR=128M + 64M, FLASH=8M
*************************************************/
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
    nDDR = 2;/*1;成都兴网的硬件是128M+64M字节的内存*/
#endif

    dwSH4_CODE_MEMORY_SIZE = CODE_MEMORY_SIZE;
    dwPARTITION0_START = LMI_SYS_BASE + MEM_OFFSET_FOR_AV_FW + dwSH4_CODE_MEMORY_SIZE;
	if(LMI_SYS_SIZE <= dwSH4_CODE_MEMORY_SIZE)
	{
		/*
		说明此硬件配置根本无法满足需求，还是不要用它测试了吧
		CSTTPrint("Error in iSetupSt710xPorting():LMI_SYS_SIZE(0x%x)<=CODE_MEMORY_SIZE(0x%x)",LMI_SYS_SIZE,dwSH4_CODE_MEMORY_SIZE);
		*/
		return;
	}
    if  ((LMI_SYS_SIZE == 0x8000000) && (nDDR == 1))
    {
		if(dwSH4_CODE_MEMORY_SIZE < 0x04000000)
		{
			/*av memory partition的分配最好不要跨两个64M字节的边界，否则视频会花屏*/
	        dwPARTITION0_STOP = LMI_SYS_BASE + 0x04000000 - 4;

			dwPARTITION1_START= LMI_SYS_BASE + 0x04000000;

	        dwPARTITION1_STOP = LMI_SYS_BASE + LMI_SYS_SIZE;
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
    bRe = CSSYSChipBoot(0);
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
    CSSYSChipInit(&stSysInfo, (HCSHANDLE)&tmp);

    //I2C_0 and I2C_1 init
    ErrorCode = CSI2CInitialize(I2C_0, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);

    ErrorCode = CSI2CInitialize(I2C_1, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);

    /* init uart */
    {
        T_ST_Custom_Param param;
        param.dwCommClock = 96000000;
        CSHDIUartSetCustomParam(&param);
        ErrorCode = CSHDIUartInitial(2, 115200);
        CSASSERT(ErrorCode == CSHDI_SUCCESS);
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
    QamInitErrorCode = CSQAMSetDVBMCNSMode(1, DVBMCNS_DVB);

    QamInitErrorCode =CSQAMInitialize(0, 9*17, TS_PARALLEL,0xff);

    CSASSERT(QamInitErrorCode == CSQAM_SUCCESS);

    if(QamInitErrorCode == CSQAM_SUCCESS)
    {
        USPPrint(0xff,"%s %d[ Equipment_Init ]  QamInitErrorCode == CSQAM_SUCCESS\n",__FILE__,__LINE__);
    }

    QamInitErrorCode =CSQAMInitialize(1, 9*17, TS_PARALLEL,0xff);

    CSASSERT(QamInitErrorCode == CSQAM_SUCCESS);

    if(QamInitErrorCode == CSQAM_SUCCESS)
    {
        USPPrint(0xff,"%s %d[ Equipment_Init ]  QamInitErrorCode == CSQAM_SUCCESS\n",__FILE__,__LINE__);
    }

    // IR init
    UI_IRInitialize(5*17);

    //Initialize panel

    panelCfg.m_ucKeyScanPin = 19;   //K11
    panelCfg.m_pucComPinGroup[0] = 20; //Com0
    panelCfg.m_pucComPinGroup[1] = 21; //Com1
    panelCfg.m_pucComPinGroup[2] = 40; //Com2
    panelCfg.m_pucComPinGroup[3] = 22; //Com3
    panelCfg.m_pucComPinGroup[4] = 41; //Com4
    panelCfg.m_pucComPinGroup[5] = 0xff; //Com5
    panelCfg.m_pucSegPinGroup[0] = 23; //PData
    panelCfg.m_pucSegPinGroup[1] = 42; //PClk

    ErrorCode = CSPanelInitialize(5*17-1, &panelCfg);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //display "good"
    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //Initalize flash
    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode = CSFLSSetSpace( 0, ROM_BASE, 0x800000);
    ErrorCode = CSFLSInitialize(43);//43??
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //**** end: Equipment_Init *************************************
    return;
}
#endif
#elif defined(st7111)
#include "memory_cfg.h"
#include "cs_qpsk.h"

static void iSetupSt71xxPorting()
{

    CSHDI_Error_t ErrorCode = CSHDI_SUCCESS;
    CSSYSInfo stSysInfo;
    CSSYSAdvancedParamEx stAdvaceParamEX;
    CSOSGRect RegionOSD ;
    CSPanelPinCfg_t panelCfg ;

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

    /* 配内存 */
    CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0], memRangeCount);

    /* 配置初始化时的制式 */
    CSSYSSetCfg(CSCFG_HD_TIMING_MODE, 0, CSVID_TIMING_MODE_1080I_50HZ, 0);
    CSSYSSetCfg(CSCFG_SD_TIMING_MODE, 0, CSVID_TIMING_MODE_PAL, 0);

    //boot	system
    if (!CSSYSChipBoot(0))
    {
        CSASSERT(0);
    }

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
    stAdvaceParamEX.m_bPvrSupport = PVR_SUPPORT;
    stAdvaceParamEX.m_bPipSupport = PIP_SUPPORT;
    stAdvaceParamEX.m_ucPtiNumber = 3;

    /*init chip*/
    if ((HCSHANDLE)NULL == CSSYSChipInitEx(&stSysInfo, (HCSHANDLE)&stAdvaceParamEX))
    {
        CSASSERT(0);
    }

    RegionOSD.left = 0;
    RegionOSD.top = 0;
    RegionOSD.Width = 720;
    RegionOSD.Height = 576;

    /*init osd*/
    ErrorCode = CSOSGOpen(CSLAYER_GFX1, 0, &RegionOSD,  TRUE);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //GPIO init ,actually init in Plat,here for UT;Gary
    ErrorCode =  CSGPIOInitialize();
    CSASSERT(ErrorCode == CSI2C_SUCCESS);

    //I2C_2 and I2C_3 init	st7111
    ErrorCode = CSI2CInitialize(I2C_2, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);
    ErrorCode = CSI2CInitialize(I2C_3, CSI2C_SPEED_NORMAL);
    CSASSERT(ErrorCode == CSI2C_SUCCESS);

    ErrorCode = CSQPSKInitialize( 0, 6*17, TS_PARALLEL, 0xff);
    CSASSERT(ErrorCode == CSQPSK_SUCCESS);

    UI_IRInitialize(5*17);

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

    //display "good"
    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    //Initalize flash
    ErrorCode = CSFLSSetFLSCount(1);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    ErrorCode = CSFLSSetSpace( 0, 0xA0000000, 0x800000);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    ErrorCode = CSFLSInitialize(46);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
}

#else /* defined(st7100) */
#error		unknown ST40 platform!!!
#endif /* defined(st7100) */

#elif defined(PLATFORM_BCM7309)
static void iSetupBcm7309Porting()
{
    CSHDI_Error_t   ErrorCode;
    CSPanelPinCfg_t sPanelPinCfg;
    HCSHANDLE hChipHandle = 0;
    CSOSGPixelFormat_S stPixelFormat = {3,3,3,7,10,5,0,15,0x00007C00,0x000003E0,0x0000001F,0x0008000,0,16,2};
    CSOSGRect Region = {0,0,720,576};
    //CSRMSetRMSupport(TRUE);

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

    ErrorCode = CSQAMInitialize(Tuner_0, 9*17, TS_PARALLEL, 38);
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
    ErrorCode = CSPanelInitialize(159, &sPanelPinCfg);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    // IR init
    UI_IRInitialize(5*17);

    /* init EEPROM & FLASH */
    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode |= CSFLSSetSpace( 0, 0xbf000000, 0x1000000);
    ErrorCode |= CSFLSInitialize(30);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    CSOSGInit(0);
    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);
    CSOSGOpen(0,0,&Region,TRUE);

}//end iSetupBcm7309Porting()

#elif defined(PLATFORM_BCM740X)
static void iSetupBcm740xPorting()
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
    ErrorCode = CSHDIUartSetDefault(0);
    ErrorCode |= CSHDIUartInitial( 2, 9600);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    /* init tuner */
    ErrorCode = CSQAMInitializeEx(CSTUNER_0, I2C_2, 159, TS_SERIAL, 0xff);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

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

    //CSPanelRegisterKeySendCallback((CSHDICallback)UI_PanelControlUserProc);

    ErrorCode = CSPanelDisplayText("good", 4);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    // IR init
    UI_IRInitialize(5*17);

    /* init EEPROM & FLASH */
    ErrorCode = CSEPRInitialize(I2C_3, 0xa0, AT24C64);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    ErrorCode = CSFLSSetFLSCount(1);
    ErrorCode |= CSFLSSetSpace( 0, 0X1E000000, 0x2000000);
    ErrorCode |= CSFLSInitialize(13);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);

    CSOSGInit(0);
    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);
    CSOSGOpen(0,0,&Region,TRUE);

}//end iSetupBcm740xPorting()

#elif defined(PLATFORM_X86)
static void ChipInit_linux(void)
{
    CSSYSChipBoot(0);
    CSOSGInit(0);
}
#else
/*#error "undefined PLATFORM"*/
#endif

#if 0
void testQAMCallback(CSHDICallbackType enumTunerType, void* pParam)
{
    T_QAMCallbackMessage* pMsg = (T_QAMCallbackMessage*)pParam;

    switch(pMsg->dwNIMEvent)
    {
    case QAM_CALLBACK_SIGNAL_CONNECTED:
        {
            CSTRACE(INFO_LEVEL, "\n[testQAMCallback]Signal Connected.\n");
            CSTRACE(INFO_LEVEL, "start to play!\n");
            //			Decode_StartPlay(1921, 1922, 1921);
            if(TRUE == CSUSPSMSPlayStream( 0, 0x25a, 0x386, 0x25a, 0 ))
            {
                CSTRACE(INFO_LEVEL, "Play Stream Success!\n");
            }
            else
            {
                CSTRACE(INFO_LEVEL, "Play Stream Fail!\n");
            }
            CSVIDEnableOutputWindow(0,0);
            CSAUDSetVolume(0, 0, 250);

            //TestDemux();
            //DMXRequest01Proc();
        }
        break;

    case QAM_CALLBACK_SIGNAL_DROPPED:
        CSTRACE(INFO_LEVEL, "\n[testQAMCallback]Signal dropped.\n");
        break;

    case QAM_CALLBACK_SIGNAL_LOST:
        CSTRACE(INFO_LEVEL, "\n[testQAMCallback]Signal lost.\n");
        break;

    case QAM_CALLBACK_SIGNAL_SEARCHING:
        CSTRACE(INFO_LEVEL, "\n[testQAMCallback]Signal searching.\n");
        break;

    default:
        break;
    }
}

void TestTuner(void)
{
    T_QAMParameters stQamParam;

    CSQAMRegisterCallback(0, testQAMCallback);

    memset( &stQamParam, 0, sizeof(stQamParam));

    stQamParam.dwFrequencyKHz = 490000;
    stQamParam.dwSymbolRateKHz = 6875;
    stQamParam.bQAMMode = CS_QAM64;

    CSQAMConnectSignal(0, &stQamParam);
    CSSleep(3000);
}
#endif


void udi_init(void)
{
    CSHDI_Error_t   ErrorCode;
    CSPanelPinCfg_t sPanelPinCfg;

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



#if 0

#define	NCACHE_SIZE		0x100000
#define	SYSTEM_SIZE		0x1E80000
#define	ESVIDBUF_SIZE		(1024*1024*4)
#define	ESAUDBUF_SIZE		(1024*1024)

static unsigned char s_ucSystemPartition[SYSTEM_SIZE];
static unsigned char s_ucNCacheHeap[NCACHE_SIZE];
static DWORD dwSH4_CODE_MEMORY_SIZE	    = 0x04000000;
static DWORD dwSH4_TOTOL_MEMORY_SIZE	= 0x08000000;
static BOOL	g_bMWModule = TRUE;
HCSHANDLE hSysInfo;

void CSPortingDriverEntry_ST40(void)
{
    CSHDI_Error_t ErrCode = CSHDI_SUCCESS;
    CSSYSInfo stSysInfo;
    CSSYSMemoryRange memProfile[4];
    int ErrorCode;
    CSOSGPixelFormat_S stPixelFormat = {0,0,0,0,16,8,0,24,0x00ff0000,0x0000ff00,0x000000ff,0xff000000,0,32,4};
    CSSYSAdvancedParam tmp;
    CSOSGRect stOSGRegion;
    CSPanelPinCfg_t panelCfg;

    memProfile[0].m_pStartAddr = (void *)s_ucSystemPartition;
    memProfile[0].m_dwSize = SYSTEM_SIZE;
    memProfile[0].m_eMemoryType = CSSYS_MEM_TYPE_SYSPART;

    memProfile[1].m_pStartAddr = (void *)s_ucNCacheHeap;
    memProfile[1].m_dwSize = NCACHE_SIZE;
    memProfile[1].m_eMemoryType = CSSYS_MEM_TYPE_NONCACHE;

    memProfile[2].m_pStartAddr = (void *)0xA8400000;
    memProfile[2].m_dwSize = 0x3C00000;
    memProfile[2].m_eMemoryType = CSSYS_MEM_TYPE_AVMEM;

    //128M
    //memProfile[3].m_pStartAddr = (void *)0xB0000000;
    //memProfile[3].m_dwSize = 0x4000000;
    //memProfile[3].m_eMemoryType = CSSYS_MEM_TYPE_AVMEM;


    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);
    CSSYSSetCfg(CSCFG_CSOS_USEPOSIX, CSDEVICE_IGNORE, 1,0);
    CSSYSSetCfg(CSCFG_HD_TIMING_MODE,0,CSVID_TIMING_MODE_1080I_50HZ,0);
    CSSYSSetCfg(CSCFG_SD_TIMING_MODE,0,CSVID_TIMING_MODE_PAL,0);
    CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0],3);

    if (!CSSYSChipBoot(0))
    {
        CSASSERT(0);
    }

    memset(&stSysInfo, 0, sizeof(CSSYSInfo));

    stSysInfo.OuputTVInfo.OutputDevice	= CSVID_OUTPUT_CVBS | CSVID_OUTPUT_HDMI | CSVID_OUTPUT_YUV;

    stSysInfo.OutputAUDInfo.OutputDevice=CSAUD_OUTPUT_RCA | CSAUD_OUTPUT_SPDIF | CSAUD_OUTPUT_HDMI;

    memset(&tmp,0,sizeof(tmp));

    tmp.m_bDualOSDBuffer = TRUE;

    tmp.m_bSupportHDDecoder = TRUE;

    tmp.m_bSupportHDDecoder_H264 = FALSE;

    tmp.m_dwMemoryStartAddr = 0x84000000;

    tmp.m_dwMemoryTotalSize = dwSH4_TOTOL_MEMORY_SIZE;

    hSysInfo = CSSYSChipInit(&stSysInfo, (HCSHANDLE)&tmp);

    CSOSGSetGlobalPixelFormat(CSLAYER_GFX1, stPixelFormat);

    stOSGRegion.left = 0;
    stOSGRegion.top = 0;
    stOSGRegion.Width = 1920;
    stOSGRegion.Height = 1080;

    ErrorCode = CSOSGOpen(CSLAYER_GFX1, 0, &stOSGRegion,  TRUE);

    CSASSERT(ErrorCode == 0);

}
#endif


