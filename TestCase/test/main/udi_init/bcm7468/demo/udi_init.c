#include "setup.h"
#include "cs_testkit.h"
#include "udi_init.h"
#include "cs_flash_plus.h"
#include "cs_panel_plus.h"
#include "cs_tuner_plus.h"
#include "cs_gpio_plus.h"
#include "cs_i2c_plus.h"
#include "cs_eeprom_plus.h"
#include "udi2_panel.h"
#include "udi_os.h"
#include "irdecode.h"
#include "cs_toolset_plus.h"
/*For test initialize time */

#define MAX_MODULE_TO_TEST	12
#define FLASH_COUNT		(1)
#define FLASH_BASE_ADDR	(0x11000000)
#define FLASH_BASE_SIZE	(0x07000000)
#define FLASH_BLOCK_SIZE	(0x20000)


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

static void Equipment_Init()
{
    CSHDI_Error_t   ErrorCode;
#if 0

    CSUDI_Error_Code errCode = CSUDI_FAILURE;

    CSPanelPinCfg_t sPanelPinCfg;
    HCSHANDLE hChipHandle = 0;

    // IR init

    g_dwTimeStart = CSGetTickCount();

    UI_IRInitialize(5*17);

    g_dwTimeStop = CSGetTickCount();
    g_dwTimeCost =  g_dwTimeStop - g_dwTimeStart;
    g_asTimeCost[ModuleIndex].pModuleName = "IR";
    g_asTimeCost[ModuleIndex++].dwTimeCost = g_dwTimeCost;

#endif
    /* init EEPROM & FLASH */
	
    ErrorCode = CSFLSSetFLSCount(FLASH_COUNT);
    ErrorCode |= CSFLSSetSpace( 0, FLASH_BASE_ADDR, FLASH_BASE_SIZE);
    ErrorCode |= CSFLSInitialize(0);
    CSASSERT(ErrorCode == CSHDI_SUCCESS);
	
}

static void iSetupBcm7468Porting()
{
	CSUDI_BOOL bRet;

	bRet = CSUDISYSInit();
	CSASSERT(bRet == CSUDI_SUCCESS);
		
	bRet = CSUDIPlatformInit();
	CSASSERT(bRet == CSUDI_SUCCESS);
		
	CSDebugSetDefaultLevel(0);
		
	Equipment_Init();
#if 0

	if(ModuleIndex >= MAX_MODULE_TO_TEST)
	{
		CSTTPrint("*********没有完成所有模块的初始化!!*************\n");
		return;
	}
#endif 
		
  TimePrint();  //打印初始化时间		
}
void udi_init()
{
	int ii=0;

	for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
	{	
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
	}
	
	iSetupBcm7468Porting();
	
	Equipment_Init(); 

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



