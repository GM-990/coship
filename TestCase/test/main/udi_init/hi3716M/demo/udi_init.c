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
#include "udi2_fs.h"
/*For test initialize time */

#define MAX_MODULE_TO_TEST	12

#define FLASH_PARTION "/dev/mtdblock9"

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

static void TunerCallback( CSUDITunerType_E eType, CSUDITunerCallbackMessage_S* psMessage)
{
	printf( "********************[%s]begin******************\n", __FUNCTION__ );
	printf( "tuner id:%d\n", psMessage->m_dwTunerId);
	printf( "lock event:%d.\n", psMessage->m_eEvent);
	printf( "m_uBandWidth:%d, m_uFrequency:%d,m_uQAMMode:%d,m_uSymbolRate:%d.\n", psMessage->m_uBandWidth, psMessage->m_uFrequency, psMessage->m_uQAMMode, psMessage->m_uSymbolRate);
	printf( "********************[%s]end******************\n", __FUNCTION__);
}


static void Equipment_Init()
{
	CSUDI_Error_Code errCode = CSUDI_FAILURE;
	CSUDITunerInitParam_S sTunerInitParam;
	
	//errCode = CSI2CInitialize(3, CSI2C_SPEED_NORMAL);
	//CSASSERT(errCode == CSHDI_SUCCESS);
	//printf("CSI2CInitialize :errCode = %d\n",errCode);

	errCode = CSI2CInitialize(4, CSI2C_SPEED_NORMAL);
	CSASSERT(errCode == CSHDI_SUCCESS);
	//printf("CSI2CInitialize :errCode = %d\n",errCode);

	errCode = CSUDITunerRegisterQam();
	memset(&sTunerInitParam, 0, sizeof(sTunerInitParam));

	sTunerInitParam.m_eTSOutMode = TS_PARALLEL;
	sTunerInitParam.m_nPriority = 159;
	sTunerInitParam.m_uI2CIndex = I2C_4;//no use of this value, just for complie,xiezhenlei add.
	sTunerInitParam.m_uResetGPIONum = 0xff;//0xff means there is no hardware reset for demodulator bcm3109. xiezhenlei add.
	   
	if ( CSUDITunerInit(0, &sTunerInitParam) != CSUDI_SUCCESS)
	{
		printf("%s CSUDITunerInit failed. \n",__FUNCTION__);
	}	
	else	
	{
		printf("%s CSUDITunerInit successed. \n",__FUNCTION__);
		CSUDITunerAddCallback(0, TunerCallback, NULL);
	}


	return;
}


static void iSetupHi3716HPorting()
{
	CSUDI_BOOL bRet;

	bRet = CSUDISYSInit();
	CSASSERT(bRet == CSUDI_SUCCESS);
		
	bRet = CSUDIPlatformInit();
	CSASSERT(bRet == CSUDI_SUCCESS);
		
	CSDebugSetDefaultLevel(CLI_LEVEL);
		
	Equipment_Init();

	if(ModuleIndex >= MAX_MODULE_TO_TEST)
	{
		CSTTPrint("*********没有完成所有模块的初始化!!*************\n");
		return;
	}
		
  TimePrint();  //打印初始化时间		
}


void udi_init()
{
	int ii=0;
	int nRet = 0;
	
	for(ii = 0;ii < MAX_MODULE_TO_TEST; ++ii )
	{	
		g_asTimeCost[ii].pModuleName= NULL;
		g_asTimeCost[ii].dwTimeCost = 0;
	}
	
	iSetupHi3716HPorting();

	system("umount /mnt/hd/HDD0/");
	CSUDIFSInit();
	nRet = CSUDIFSAddPartition(FLASH_PARTION,EM_UDIFS_PARTITION_UBIFS);
	if ( nRet != CSUDI_SUCCESS)
	{
		CSTTPrint("[%s] CSUDIFSAddPartition failed. ret = 0x%x.\n", __FUNCTION__, nRet);
	}
	else
	{
		CSTTPrint("[%s] CSUDIFSAddPartition success!\n", __FUNCTION__);
	}
	
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



