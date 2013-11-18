#include "setup.h"
#include "irdecode.h"
#ifndef MMCP_UDI2
#include "cs_sysdemux.h"
#endif
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

void TimePrint(void) ; //打印初始化时间函数

//extern void UI_PanelControlUserProc(CSHDICallbackType tCallbackType, void *param);

#include "memory_cfg.h"
#ifndef MMCP_UDI2
static void iSetupSt5197Porting()
{

	CSHDI_Error_t ErrorCode = CSHDI_SUCCESS;
	CSSYSInfo stSysInfo;
	CSSYSAdvancedParamEx stAdvaceParamEX;
	CSOSGRect RegionSD;
	CSQAM_Error_t QamInitErrorCode;
	CSSPIPinIndex stSpiPinIndex= {0};

	CSSYSMemoryRange memProfile[] = 
	{
		{CSSYS_MEM_TYPE_SYSPART, 		(void*)SYSTEM_PARTITIOAN_START_ADDR, 	SYSTEM_PARTITIOAN_SIZE, 	},/* user heap */
		{CSSYS_MEM_TYPE_NONCACHE, 		(void*)NCACHE_PARTITIOAN_START_ADDR,	NCACHE_PARTITIOAN_SIZE,		},/* non-cached */
		{CSSYS_MEM_TYPE_OS, 			(void*)OS_PARTITIOAN_START_ADDR,		OS_PARTITIOAN_SIZE,			},/* os */
		{CSSYS_MEM_TYPE_DRIVER, 		(void*)DRIVER_PARTITIOAN_START_ADDR,	DRIVER_PARTITIOAN_SIZE,		},/* driver */
		{CSSYS_MEM_TYPE_AVMEM_USER, 	(void*)AVMEM_USER_START_ADDR,			AVMEM_USER_SIZE,			},	/* user osd avmem */
		{CSSYS_MEM_TYPE_AVMEM, 			(void*)AVMEM_SYS_START_ADDR,			AVMEM_SYS_SIZE,				},	/* avmem sys */
	} ;

	int nMemRangeCount = sizeof(memProfile)/sizeof(CSSYSMemoryRange);

	CSDebugSetDefaultLevel(DEBUG_LEVEL);//CSSetDebugLevel(0);

	/* 配内存 */
	CSSYSSetCfg(CSCFG_MEM_PROFILE,0,(DWORD)&memProfile[0], nMemRangeCount);

	//chip boot
	if (!CSSYSChipBoot(0) )
	{
		CSASSERT(0);
		//return;
	}

	/* 配置初始化时的制式 */
	CSSYSSetCfg(CSCFG_SD_TIMING_MODE, 0, CSVID_TIMING_MODE_PAL, 0);

	memset(&stSysInfo, 0, sizeof(CSSYSInfo));
	stSysInfo.OutputAUDInfo.OutputDevice=CSAUD_OUTPUT_RCA | CSAUD_OUTPUT_SPDIF ;
	stSysInfo.OuputTVInfo.OutputDevice =	CSVID_OUTPUT_CVBS | CSVID_OUTPUT_YUV;

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
	if (0 == CSSYSChipInitEx(&stSysInfo, (HCSHANDLE)(&stAdvaceParamEX)))
	{
		CSASSERT(0);
		//return;
	}

	/*init osd*/
	RegionSD.left = 0;
	RegionSD.top = 0;
	RegionSD.Width = 720;
	RegionSD.Height = 576;

	ErrorCode = CSOSGOpen(CSLAYER_GFX1, 0, &RegionSD,  TRUE);
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	//GPIO init ,actually init in Plat,here for UT;Gary
	ErrorCode =  CSGPIOInitialize();
	CSASSERT(ErrorCode == CS_GPIO_SUCCESS);
    
    	//SPIinit 
	ErrorCode =  CSSPIInitialize(SPI_0, stSpiPinIndex, CSSPI_SPEED_HIGHT);
	CSASSERT(ErrorCode == 0);

	//Initalize flash
	ErrorCode = CSFLSSetFLSCount(1);
	ErrorCode = CSFLSSetSpace( 0, 0, 0x400000);
	ErrorCode = CSFLSInitialize(0xff);
	CSASSERT(ErrorCode == CSHDI_SUCCESS);

	//I2C_0
	ErrorCode = CSI2CInitialize(I2C_0, CSI2C_SPEED_NORMAL);//used for tuner
	CSASSERT(ErrorCode == CSI2C_SUCCESS);

	// tuner init
	//QamInitErrorCode = CSQAMSetDVBMCNSMode(0, DVBMCNS_DVB);
	//CSASSERT(QamInitErrorCode == CSQAM_SUCCESS);

	QamInitErrorCode =CSQAMInitializeEx(0, 0, 9*17, TS_PARALLEL,0xff);
	CSASSERT(QamInitErrorCode == CSQAM_SUCCESS);

	
	// IR init
	UI_IRInitialize(5*17);

	//USB Power Enable
	ErrorCode = CSGPIOOpen(33, CS_GPIO_OUPUT);
	CSASSERT(ErrorCode == CS_GPIO_SUCCESS);

	ErrorCode = CSGPIOWrite(33, 1);
	CSASSERT(ErrorCode == CS_GPIO_SUCCESS);

	ErrorCode = CSGPIOOpen(34, CS_GPIO_OUPUT);
	CSASSERT(ErrorCode == CS_GPIO_SUCCESS);

	ErrorCode = CSGPIOWrite(34, 1);
	CSASSERT(ErrorCode == CS_GPIO_SUCCESS);

	//USB PHY Reset
	ErrorCode = CSGPIOOpen(20, CS_GPIO_OUPUT);
	CSASSERT(ErrorCode == CS_GPIO_SUCCESS);

	ErrorCode = CSGPIOWrite(20, 0);
	CSASSERT(ErrorCode == CS_GPIO_SUCCESS);

	CSSleep(1000);

	ErrorCode = CSGPIOWrite(20, 1);
	CSASSERT(ErrorCode == CS_GPIO_SUCCESS);

	g_dwTimeStop = CSGetTickCount();
	g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
 

    TimePrint();  //打印初始化时间
	
}
#endif

void udi_init(void)
{
#ifndef MMCP_UDI2
	int ii=0;

	for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
	{	
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
	}

	CSRMSetRMSupport(TRUE);

	iSetupSt5197Porting();

	InitUSPSectionDemux(0);
#endif
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


