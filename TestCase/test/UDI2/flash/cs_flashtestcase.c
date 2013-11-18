/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_flashtestcase.h"
#include "cs_flashudi2_test.h"
#include "cs_flash.h"
#include "udi2_flash.h"
#include "udi2_typedef.h"
#include "udi2_os.h"
#include "udi2_public.h"
#include "udi2_fs.h"

#define REPERT_COUNT 1000

#define FLASH_TEST_STACK_SIZE  64*1024

//#define BOOTER_TEST

#define MAX_FLASH_MALLOC_SIZE (16*1024*1024)

//flash空间的起始地址
extern CSUDI_UINT32 g_uStartAddress;

//用户可用空间的起始地址
extern CSUDI_UINT32 g_uTestStartAddress;

//用户可用空间的总大小
extern unsigned int g_uTestFlashSize;

//用户可用的最大的Block大小
extern unsigned int g_uTestBlockSize;

//flash分区集合信息
extern unsigned int  g_uALLRegionCount;
extern unsigned int  g_uALLRegionSize;
extern unsigned int  g_uALLBlockCount;
extern unsigned int  g_uALLMaxBlockSize;


//多任务测试时用到的全局变量
 static CSUDI_BOOL g_bFlashWrite1 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlashWrite2 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlashRead1 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlashRead2 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlashWrite3 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlashWrite4 = CSUDI_FALSE;

 static CSUDI_BOOL g_bFlagWrite1 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlagWrite2 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlagRead1 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlagRead2 = CSUDI_FALSE;
 static CSUDI_BOOL g_bFlagWrite3 = CSUDI_FALSE; 
 static CSUDI_BOOL g_bFlagWrite4 = CSUDI_FALSE;

//随机读写flash测试
#define DATA_BUF_SIZE (1024*128)
static unsigned int s_dwTestAreaStart = 0;
static unsigned int s_dwTestTestAreaSize = 0;
static BOOL s_bFlashTaskRun = FALSE;
static BOOL s_bFlashTaskFlag = TRUE;
static BOOL s_bLocalFsWriteReadTaskRun = TRUE;
static BOOL s_bLocalFsTaskFlag = TRUE;
static BOOL RandThread1Done = FALSE;
static BOOL RandThread2Done = FALSE;
static BOOL bRandRunFlag1 = TRUE;
static BOOL bRandRunFlag2 = TRUE;
 

CSUDI_BOOL CSTC_FLASH_Init(void)
{
	//在本测试用例集执行前调用
	 CSTC_FLASH_InitCfg();

	//如果flash系统把整个flash作为一个block，则设置测试用例的block大小为128K
	//否则设置测试用例的block大小为flash最大的block大小
	if(g_uALLBlockCount > 1)
	{
		g_uTestBlockSize = g_uALLMaxBlockSize;
	}
	
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_FLASH_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

void TestFlashWrite1(void * pvParam)
{
	int i;
	CSUDI_UINT8 uWriteData[1024];
	CSUDI_INT32 baseadd = g_uTestStartAddress + 1*g_uTestBlockSize;

	for(i=0;i < sizeof(uWriteData);i++)
	{
		uWriteData[i] = 0xaa;
	}

	if(CSUDIFlashWrite( baseadd ,  uWriteData, sizeof(uWriteData)) != CSUDI_SUCCESS)
	{
		CSTCPrint("任务1写flash失败");
		g_bFlagWrite1 = CSUDI_TRUE;
	}
	
	g_bFlashWrite1 = CSUDI_TRUE;	
}

void TestFlashRead1(void * pvParam)
{
	int i;
	CSUDI_UINT8 uReadData[1024];
	CSUDI_INT32 baseadd = g_uTestStartAddress + 1*g_uTestBlockSize;
	
	if(CSUDIFlashRead(baseadd , uReadData, sizeof(uReadData)) != CSUDI_SUCCESS)
	{
		CSTCPrint("任务1读flash失败");
		g_bFlagRead1 = CSUDI_TRUE;
	}

	for(i = 0;i < sizeof(uReadData); ++i )
	{
		if(uReadData[i] != 0xaa)
		{
			g_bFlagRead1 = CSUDI_TRUE;
			break;
		}
	}
	g_bFlashRead1 = CSUDI_TRUE;	
}

void TestFlashWrite2(void * pvParam)
{
	int i;
	CSUDI_UINT8 uWriteData[1024];
	CSUDI_INT32 baseadd = g_uTestStartAddress + 2*g_uTestBlockSize;

	for(i=0;i < sizeof(uWriteData);i++)
	{
		uWriteData[i] = 0x88;
	}

	if(CSUDIFlashWrite(baseadd, uWriteData, sizeof(uWriteData)) != CSUDI_SUCCESS)
	{
		CSTCPrint("任务1写flash失败");
		g_bFlagWrite2 = CSUDI_TRUE;
	}
	
	g_bFlashWrite2 = CSUDI_TRUE;	
}

void TestFlashRead2(void * pvParam)
{
	int i;
	CSUDI_UINT8 uReadData[1024];
	CSUDI_INT32 baseadd = g_uTestStartAddress + 2*g_uTestBlockSize;
	
	if(CSUDIFlashRead(baseadd, uReadData, sizeof(uReadData)) != CSUDI_SUCCESS)
	{
		CSTCPrint("任务1读flash失败");
		g_bFlagRead1 = CSUDI_TRUE;
	}

	for(i = 0;i < sizeof(uReadData); ++i )
	{
		if(uReadData[i] != 0x88)
		{
			g_bFlagRead2 = CSUDI_TRUE;
			break;
		}
	}
	g_bFlashRead2 = CSUDI_TRUE;	
}

void TestFlashWrite3(void * pvParam)
{
	int i;
	CSUDI_UINT8 uWriteData[1024];
	CSUDI_INT32 baseadd = g_uTestStartAddress + g_uTestFlashSize - sizeof(uWriteData);

	for(i=0;i < sizeof(uWriteData);i++)
	{
		uWriteData[i] = 0x88;
	}

	if(CSUDIFlashWrite(baseadd, uWriteData, sizeof(uWriteData)) != CSUDI_SUCCESS)
	{
		CSTCPrint("任务3写flash失败");
		g_bFlagWrite3 = CSUDI_TRUE;
	}
		
	g_bFlashWrite3 = CSUDI_TRUE;
}

void TestFlashWrite4(void * pvParam)
{
	int i;
	CSUDI_UINT8 uWriteData[1024];
	CSUDI_INT32 baseadd = g_uTestStartAddress + g_uTestFlashSize - sizeof(uWriteData);

	for(i=0;i < sizeof(uWriteData);i++)
	{
		uWriteData[i] = 0xaa;
	}

	if(CSUDIFlashWrite(baseadd,  uWriteData, sizeof(uWriteData)) != CSUDI_SUCCESS)
	{
		CSTCPrint("任务4写flash失败");
		g_bFlagWrite4 = CSUDI_TRUE;
	}
		
	g_bFlashWrite4 = CSUDI_TRUE;
}

//	CSUDIFlashEraseSpace(0x6800000,0x7760000);
//@CASEGROUP:CSUDIFlashGetInfo 
//@DESCRIPTION:测试参数eSuitType不合法时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:eSuitType = EM_UDI_FLASH_MAX
//@INPUT:psFlashSuit非空
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIFlashGetInfo(EM_UDI_FLASH_MAX, psFlashSuit)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0001(void)
{
	CSUDIFLASHRegionSuit_S sFlashRegionSuit;
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashGetInfo(EM_UDI_FLASH_MAX,&sFlashRegionSuit),"eSuitType = EM_UDI_FLASH_MAX时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashGetInfo 
//@DESCRIPTION:测试参数psFlashSuit = CSUDI_NULL时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:eSuitType值合法
//@INPUT:psFlashSuit = CSUDI_NULL
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIFlashGetInfo(EM_UDI_FLASH_ALL,CSUDI_NULL)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0002(void)
{
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashGetInfo(EM_UDI_FLASH_ALL,CSUDI_NULL),"psFlashSuit = CSUDI_NULL时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashGetInfo 
//@DESCRIPTION:测试能否正确得到分区集合信息时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:eSuitType值合法
//@INPUT:psFlashSuit值合法
//@EXPECTATION:返回CSUDI_SUCCESS，得到的分区集合信息与配置文件中的相符
//@REMARK:
//@EXECUTIONFLOW:1、eSuitType=EM_UDI_FLASH_ALL时调用CSUDIFlashGetInfo期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、期望得到的分区集合信息与配置文件中的相同
CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0003(void)
{
	CSUDIFLASHRegionSuit_S sFlashRegionSuit;

	int i;

	unsigned int uSize = 0;

	unsigned int uBlockCount = 0;
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashGetInfo(EM_UDI_FLASH_ALL,&sFlashRegionSuit),"第一步测试失败");

	CSTCPrint("\n********m_uRegintCount:%u,\n\tm_uSize:%0x,\n\tm_uMaxBlockSize:%0x,\n\tm_uBlockCount:%u\n",
	sFlashRegionSuit.m_uRegionCount,sFlashRegionSuit.m_uSize,sFlashRegionSuit.m_uMaxBlockSize,sFlashRegionSuit.m_uBlockCount);
	CSTCPrint("g_uALLRegionSize=0x%x,sFlashRegionSuit.m_uSize=0x%x, g_uALLBlockCount=%d\r\n ", g_uALLRegionSize, sFlashRegionSuit.m_uSize, g_uALLBlockCount);
	CSTCPrint("g_uStartAddress=0x%x,  sFlashRegionSuit.m_asBlockRegion[0].m_dwStartAddress=0x%x\r\n", g_uStartAddress, sFlashRegionSuit.m_asBlockRegion[0].m_dwStartAddress);
	
	CSTK_ASSERT_TRUE_FATAL(g_uALLRegionCount == sFlashRegionSuit.m_uRegionCount, "第二步得到flash count与配置文件不相同");

	CSTK_ASSERT_TRUE_FATAL(g_uALLRegionSize == sFlashRegionSuit.m_uSize, "第二步得到flash大小与配置文件不相同");

	CSTK_ASSERT_TRUE_FATAL(g_uALLBlockCount== sFlashRegionSuit.m_uBlockCount, "第二步得到flash block count与配置文件不相同");
	
	CSTK_ASSERT_TRUE_FATAL(g_uALLMaxBlockSize == sFlashRegionSuit.m_uMaxBlockSize, "第二步得到flash max block size与配置文件不相同");
	
	CSTK_ASSERT_TRUE_FATAL(g_uStartAddress == sFlashRegionSuit.m_asBlockRegion[0].m_dwStartAddress,"第二步得到flash的起始地址与配置文件不相同");
	CSTK_ASSERT_TRUE_FATAL(0 == (g_uTestBlockSize%sFlashRegionSuit.m_uMaxBlockSize),"第二步TestBlockSize 不是BlockSize的整数倍");

	for(i = 0; i < g_uALLRegionCount; ++i)
	{
		uBlockCount += sFlashRegionSuit.m_asBlockRegion[i].m_uBlockCount;
	
		uSize += (sFlashRegionSuit.m_asBlockRegion[i].m_uBlockCount)*(sFlashRegionSuit.m_asBlockRegion[i].m_uBlockSize); 
	}

	CSTK_ASSERT_TRUE_FATAL(sFlashRegionSuit.m_uSize == uSize ,"第二步得到分区集合总大小与所有分区Block总大小不相同");
	
	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:测试参数dwStartAddress不合法情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uStartAddress - 0x1000
//@INPUT:pucBuf值合法
//@INPUT:uDataLength值合法
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uStartAddress为flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashRead(g_uStartAddress - 0x1000,pucBuf,16)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0001(void)
{
	CSUDI_UINT8 pucBuf[16];
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashRead(g_uStartAddress - 0x1000, pucBuf, 16),"dwStartAddress不合法时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:测试参数pucBuf = CSUDI_NULL的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf = CSUDI_NULL
//@INPUT:uDataLength = 1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashRead(g_uTestStartAddress, CSUDI_NULL, 1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashRead(g_uTestStartAddress, CSUDI_NULL, 1024),"pucBuf = CSUDI_NULL时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:测试参数uDataLength = 0时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf值合法
//@INPUT:uDataLength = 0
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashRead(g_uTestStartAddress, pucBuf, 0)期望返回CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0003(void)
{
	CSUDI_UINT8 pucBuf[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 0),"uDataLength = 0时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:测试参数uDataLength不合法的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf值合法
//@INPUT:uDataLength = 1024*1024*1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashRead(g_uTestStartAddress, pucBuf, 1024*1024*1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0004(void)
{
	CSUDI_UINT8 pucBuf[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashRead(g_uTestStartAddress, pucBuf, 1024*1024*1024),"uDataLength不合法时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:测试dwStartAddress + uDataLength大于可读写的flash空间时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uStartAddress+1024
//@INPUT:pucBuf值合法
//@INPUT:uDataLength = g_uALLRegionSize
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uStartAddress为flash空间起始地址，g_uALLRegionSize为flash分区集合的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashRead(g_uStartAddress+1024, pucBuf, g_uALLRegionSize)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0005(void)
{
	CSUDI_UINT8 *pucBuf;
	unsigned int unMallocSzie;
	
	if (g_uALLRegionSize > MAX_FLASH_MALLOC_SIZE)
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);
		unMallocSzie = g_uALLMaxBlockSize;
		
	}
	else
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLRegionSize);
		unMallocSzie = g_uALLRegionSize;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashRead(g_uStartAddress+1024, pucBuf, g_uALLRegionSize),"测试失败");

	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");	
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite 
//@DESCRIPTION:测试参数dwStartAddress不合法情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uStartAddress - 0x1000
//@INPUT:pucBuf值非空
//@INPUT:uDataLength = 1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uStartAddress为flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite(g_uStartAddress - 0x1000, pucBuf, 1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0001(void)
{
	CSUDI_UINT8 pucBuf[1024];
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWrite(g_uStartAddress - 0x1000, pucBuf, 1024),"dwStartAddress不合法时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite 
//@DESCRIPTION:测试参数pucBuf = CSUDI_NULL的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf = CSUDI_NULL
//@INPUT:uDataLength = 1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite(g_uTestStartAddress, CSUDI_NULL, 16)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWrite(g_uTestStartAddress, CSUDI_NULL, 16),"pucBuf = CSUDI_NULL时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite 
//@DESCRIPTION:测试参数uDataLength = 0时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值非空
//@INPUT:uDataLength = 0
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite(g_uTestStartAddress, pucBuf, 0)期望返回CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0003(void)
{
	CSUDI_UINT8 pucBuf[1024];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, 0),"uDataLength = 0时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite 
//@DESCRIPTION:测试参数uDataLength不合法的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值非空
//@INPUT:uDataLength = 1024*1024*1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite(g_uTestStartAddress, pucBuf, 1024*1024*1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0004(void)
{
	CSUDI_UINT8 pucBuf[1024];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWrite(g_uTestStartAddress, pucBuf, 1024*1024*1024),"uDataLength不合法时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:测试dwStartAddress + uDataLength大于可读写的flash空间时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uStartAddress + 1024
//@INPUT:pucBuf值合法
//@INPUT:uDataLength = g_uALLRegionSize
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uStartAddress为flash空间起始地址，g_uALLRegionSize为flash分区集合的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite(g_uStartAddress+1024, pucBuf, g_uALLRegionSize)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0005(void)
{
	CSUDI_UINT8 *pucBuf;
	unsigned int unMallocSzie;
	
	if (g_uALLRegionSize > MAX_FLASH_MALLOC_SIZE)
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);
		unMallocSzie = g_uALLMaxBlockSize;
		
	}
	else
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLRegionSize);
		unMallocSzie = g_uALLRegionSize;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWrite(g_uStartAddress+1024, pucBuf, g_uALLRegionSize),"测试失败");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试参数dwStartAddress不合法情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uStartAddress-0x1000
//@INPUT:uSpaceSize = 1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uStartAddress为flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace(g_uStartAddress - 0x1000, 1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0001(void)
{
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashEraseSpace(g_uStartAddress - 0x1000, 1024),"dwStartAddress不合法时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试参数uSpaceSize = 0时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = 0 
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace(g_uTestStartAddress, 0)期望返回CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 0),"uSpaceSize = 0时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试参数uSpaceSize不合法的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = 1024*1024*1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace(g_uTestStartAddress, 1024*1024*1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashEraseSpace(g_uTestStartAddress, 1024*1024*1024),"uSpaceSize不合法时测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试dwStartAddress + uSpaceSize大于可读写的flash空间时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uStartAddress + 1024
//@INPUT:uSpaceSize = g_uALLRegionSize
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uStartAddress为flash空间起始地址，g_uALLRegionSize为flash分区集合的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace(g_uStartAddress + 1024, g_uALLRegionSize)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0004(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashEraseSpace(g_uStartAddress + 1024, g_uALLRegionSize),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试擦除整个用于测试的flash空间的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = g_uTestFlashSize
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestFlashSize为可用于测试的flash空间大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashRead读取flash中的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、期望读到的数据全为0xff
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0005(void)
{
	int i;
	unsigned int unLeftSzie = g_uTestFlashSize;
	unsigned int unReadSize = 0;
	unsigned int unMallocSzie = 0;
	unsigned int unTestStartAddr = g_uTestStartAddress;
	CSUDI_UINT8 *pucBuf = NULL;
	

	if (g_uTestFlashSize > MAX_FLASH_MALLOC_SIZE)
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);
		unMallocSzie = g_uALLMaxBlockSize;
	}
	else
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestFlashSize);
		unMallocSzie = g_uTestFlashSize;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTCPrint("测试运行中，请等待!!\n");

	CSTCPrint("正在擦除Flash，请等待!!\n");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize),"第一步擦除整个测试的FLASH空间失败");

	CSTCPrint("正在读取数据，请等待!!\n");
	while (unLeftSzie > 0)
	{
		unReadSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr, pucBuf, unReadSize),"第二步读取数据失败");

		for(i=0; i < unReadSize; ++i)
		{
			if (pucBuf[i] != 0xff)
			{
				CSTCPrint("pucBuf[%d]=0x%x, unReadSize=0x%x\r\n", i, pucBuf[i], unReadSize);
			}
			CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第三步数据校验失败");
		}

		unTestStartAddr += unReadSize;
		unLeftSzie -= unReadSize;
	}

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试擦除一个Block时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = g_uTestBlockSize
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashRead读取擦除后flash中该block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、期望读到的数据全为0xff
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0006(void)
{
	int i;

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步擦除一个Block时失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "第二步读取数据失败");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第三步数据校验失败");
	}
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试擦除多个Block时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = 3*g_uTestBlockSize
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace(g_uTestStartAddress, 3*g_uTestBlockSize)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashRead读取擦除后flash中的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、期望读到的数据全为0xff
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0007(void)
{
	int i;

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(3*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 3*g_uTestBlockSize),"第一步擦除多个Block时失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 3*g_uTestBlockSize), "第二步读取数据失败");

	for(i=0; i < 3*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第三步数据校验失败");
	}
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试要擦除的起始地址不规则时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress + 0x0003
//@INPUT:uSpaceSize = g_uTestBlockSize -1024
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block大小
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite把第一个block内容全部写为0x55期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、ErrCode = CSUDIFlashEraseSpace(g_uTestStartAddress + 0x0003, g_uTestBlockSize -1024)
//@EXECUTIONFLOW:3、如果ErrCode == CSUDI_SUCCESS则执行4步到5步
//@EXECUTIONFLOW:4、调用CSUDIFlashRead读取第一个block的内容，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、期望读到的block的数据从第三个起的g_uTestBlockSize -1024个数据为0xff，其余的数据为0x55
//@EXECUTIONFLOW:6、如果ErrCode == CSUDIFLASH_ERROR_BAD_PARAMETER则执行步7到第8步
//@EXECUTIONFLOW:7、调用CSUDIFlashRead读取第一个block的内容，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读到的数据全为0x55
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0008(void)
{
	int i;

	CSUDI_BOOL ErrCode;

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	memset(pucBuf, 0x55, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "第一步擦除测试失败");

	ErrCode = CSUDIFlashEraseSpace(g_uTestStartAddress + 0x0003, g_uTestBlockSize -1024);

	if(ErrCode == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "第四步读取测试失败");

		for(i=0; i < 0x0003; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第五步数据校验失败");
		}
		for( ; i < g_uTestBlockSize -1024 + 3; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第五步数据校验失败");
		}
		for( ; i < g_uTestBlockSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第五步数据校验失败");
		}
	}
	else
		if(ErrCode == CSUDIFLASH_ERROR_BAD_PARAMETER)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "第七步读取测试失败");
	
			for(i = 0 ; i < g_uTestBlockSize; ++i)
			{
				CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第八步数据校验失败");
			}
		}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:测试要擦除的大小不规则时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = 1023
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block大小
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite把第一个block内容全部写为0x55期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、ErrCode = CSUDIFlashEraseSpace(g_uTestStartAddress, 1023)
//@EXECUTIONFLOW:3、如果ErrCode == CSUDI_SUCCESS则执行4步到5步
//@EXECUTIONFLOW:4、调用CSUDIFlashRead读取第一个block的内容，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、期望读到的block的数据前1023个数据为0xff，其余的数据为0x55
//@EXECUTIONFLOW:6、如果ErrCode == CSUDIFLASH_ERROR_BAD_PARAMETER则执行步7到第8步
//@EXECUTIONFLOW:7、调用CSUDIFlashRead读取第一个block的内容，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读到的数据全为0x55
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0009(void)
{
	int i;

	CSUDI_BOOL ErrCode;

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	memset(pucBuf, 0x55, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "第一步测试失败");

	ErrCode = CSUDIFlashEraseSpace(g_uTestStartAddress, 1023);

	if(ErrCode == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "第四步测试失败");

		for(i=0; i < 1023 ; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第五步测试失败");
		}
		
		for( ; i < g_uTestBlockSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第五步测试失败");
		}
	}
	else
		if(ErrCode == CSUDIFLASH_ERROR_BAD_PARAMETER)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "第七步测试失败");
	
			for(i = 0 ; i < g_uTestBlockSize; ++i)
			{
				CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第八步测试失败");
			}
		}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:测试dwStartAddress不合法情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uStartAddress-0x1000
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uStartAddress为flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashWriteWithoutErase(g_uStartAddress - 0x1000, pucBuf, 1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0001(void)
{
	CSUDI_UINT8 pucBuf[1024];
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWriteWithoutErase(g_uStartAddress - 0x1000, pucBuf, 1024),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:测试pucBuf = CSUDI_NULL的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf = CSUDI_NULL
//@INPUT:uDataLength = 1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashWriteWithoutErase(g_uTestStartAddress, CSUDI_NULL, 1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWriteWithoutErase(g_uTestStartAddress, CSUDI_NULL, 1024),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:测试uDataLength = 0时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 0
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 0)期望返回CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0003(void)
{
	CSUDI_UINT8 pucBuf[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 0),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:测试uDataLength不合法的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1024*1024*1024
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 1024*1024*1024)期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0004(void)
{
	CSUDI_UINT8 pucBuf[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 1024*1024*1024),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:测试dwStartAddress + uDataLength大于可读写的flash空间时的情况
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uStartAddress + 1024
//@INPUT:pucBuf值合法
//@INPUT:uDataLength = g_uALLRegionSize
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:g_uStartAddress为flash空间起始地址，g_uALLRegionSize为flash分区集合的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashWriteWithoutErase期望返回非CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0005(void)
{
	CSUDI_UINT8 *pucBuf;
	unsigned int unMallocSzie;
	
	if (g_uALLRegionSize > MAX_FLASH_MALLOC_SIZE)
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);
		unMallocSzie = g_uALLMaxBlockSize;
		
	}
	else
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLRegionSize);
		unMallocSzie = g_uALLRegionSize;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWriteWithoutErase(g_uStartAddress + 1024, pucBuf, g_uALLRegionSize),"测试失败");

	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");	
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:对整个测试的FLASH空间进行读写时的读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestFlashSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestFlashSize为可用于测试的flash空间大小
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite函数向g_uTestStartAddress地址写入g_uTestFlashSize个字节0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestFlashSize个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、期望读出的数据都是0x55
//@EXECUTIONFLOW:4、调用CSUDIFlashWrite函数向g_uTestStartAddress地址写入g_uTestFlashSize个字节0xbb，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestFlashSize个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望读出的数据都是0xbb
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0001(void)
{
	int i;
	unsigned int unLeftSzie = g_uTestFlashSize;
	unsigned int unOperSize = 0;
	unsigned int unMallocSzie = 0;
	unsigned int unTestStartAddr = g_uTestStartAddress;
	CSUDI_UINT8 *pucBuf = NULL;
	
	if (g_uTestFlashSize > MAX_FLASH_MALLOC_SIZE)
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);
		unMallocSzie = g_uALLMaxBlockSize;
		
	}
	else
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestFlashSize);
	
		unMallocSzie = g_uTestFlashSize;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");

	

	CSTCPrint("测试运行中，请等待!!\n");

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	while (unLeftSzie > 0)
	{
		memset(pucBuf, 0x55, unMallocSzie);
	 	unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
	 	//CSTCPrint("CSUDIFlashWrite unTestStartAddr=0x%x,pucBuf=0x%x,unOperSize=0x%x, g_uALLMaxBlockSize=0x%xr\n", unTestStartAddr,pucBuf,unOperSize, g_uALLMaxBlockSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(unTestStartAddr,pucBuf,unOperSize),"第一步测试失败");
		
		memset(pucBuf, 0x00, unMallocSzie);
		//CSTCPrint("CSUDIFlashRead unTestStartAddr=0x%x,pucBuf=0x%x,unOperSize=0x%xr\n", unTestStartAddr,pucBuf,unOperSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"第二步测试失败");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第三步数据校验测试失败");
		}
		unTestStartAddr += unOperSize;
		unLeftSzie -= unOperSize;
	}

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	while (unLeftSzie > 0)
	{
		memset(pucBuf, 0xbb, unMallocSzie);
		 unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(unTestStartAddr,pucBuf,unOperSize),"第四步测试失败");
		memset(pucBuf, 0x00, unMallocSzie);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"第五步测试失败");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "第六步数据校验测试失败");
		}

		unTestStartAddr += unOperSize;
		unLeftSzie -= unOperSize;
	}

 	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:块内读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 16
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址,g_uTestBlockSize为用于测试的block大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向g_uTestStartAddress地址写入16个字节pubBuf指向的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出16个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据和写入的数据相同
//@EXECUTIONFLOW:5、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出第一个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望读出的数据前16个字节数据与pubBuf指向的数据相同，其余的全为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0002(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[16];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");

	for(i=0; i < 16; ++i)
	{
		pucBuf[i] = i;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf,16),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,16),"第三步测试失败");

	for(i=0; i < 16; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucData, '\0', g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucData,g_uTestBlockSize),"第五步测试失败");

	for(i=0; i < 16; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i == pucData[i], "第六步数据校验测试失败");
	}
	
	for(; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第六步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:整块读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除前两个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向第一个block内写入g_uTestBlockSize个字节的数据0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从第一个block内读出g_uTestBlockSize个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashWrite函数向第一个block内写入g_uTestBlockSize个字节的0x00，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashRead函数从第一个block内读出g_uTestBlockSize个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读出的数据都是0x00
//@EXECUTIONFLOW:8、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出前两个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读出的数据第一个block内的数据为0x00，第二个block内的数据为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0003(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第一步测试失败");
	 
	memset(pucBuf, 0x55, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第三步测试失败");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0x00, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第六步测试失败");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x00 == pucBuf[i], "第七步数据校验测试失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, 2*g_uTestBlockSize),"第八步测试失败");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x00== pucData[i], "第九步数据校验测试失败");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"销毁资源失败");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:跨块读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress值合法
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize + 512
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除前两个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向g_uTestStartAddress地址写入g_uTestBlockSize + 512个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize + 512个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashWrite函数向g_uTestStartAddress地址写入g_uTestBlockSize + 512个0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize + 512个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读出的数据都是0xaa
//@EXECUTIONFLOW:8、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出前两个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读出的数据前g_uTestBlockSize + 512是0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0004(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize + 512);

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, g_uTestBlockSize + 512);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize + 512),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize + 512),"第三步测试失败");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, g_uTestBlockSize + 512);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf,g_uTestBlockSize + 512),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize + 512),"第六步测试失败");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第七步数据校验测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, 2*g_uTestBlockSize),"第八步测试失败");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "第九步数据校验测试失败");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:右侧block对齐，左侧block不对齐时的读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress + 512
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize - 512
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向g_uTestStartAddress + 512地址写入g_uTestBlockSize - 512个字节数据0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 512地址读出g_uTestBlockSize - 512个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashWrite函数向g_uTestStartAddress + 512地址写入g_uTestBlockSize - 512个字节数据0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashRead函数从g_uTestStartAddress + 512地址读出g_uTestBlockSize - 512个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读出的数据都是0xaa
//@EXECUTIONFLOW:8、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出第一个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读出的数据前512个字节是0xff，其余的全部为0xaa
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0005(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 512, pucBuf, g_uTestBlockSize -512),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 512, pucBuf, g_uTestBlockSize -512),"第三步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 512,pucBuf,g_uTestBlockSize - 512),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 512,pucBuf,g_uTestBlockSize - 512),"第六步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第七步数据校验测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第八步测试失败");

	for(i=0; i < 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第九步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第九步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:左侧block对齐，右侧block不对齐时的读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize - 512
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向g_uTestStartAddress地址写入g_uTestBlockSize - 512个字节的0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize - 512个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashWrite函数向g_uTestStartAddress地址写入g_uTestBlockSize - 512个字节的0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize - 512个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读出的数据都是0xaa
//@EXECUTIONFLOW:8、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出第一个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读出的数据前g_uTestBlockSize - 512个字节是0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0006(void)
{
	int i;
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucData, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucData, g_uTestBlockSize - 512),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize -512),"第三步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucData[i], "第四步数据校验测试失败");
	}

	memset(pucData, 0xaa, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucData,g_uTestBlockSize - 512),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucData,g_uTestBlockSize - 512),"第六步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "第七步数据校验测试失败");
	}
	
	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"第八步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "第九步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:起始地址为奇地址，读写长度为奇数时的读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress分别取g_uTestStartAddress + 0x0001
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1023
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向g_uTestStartAddress + 0x0001地址写入1023个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1023个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashWrite函数向g_uTestStartAddress + 0x0001地址写入1023个0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1023个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读出的数据都是0xaa
//@EXECUTIONFLOW:8、调用CSUDIFlashRead函数读取第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读出的数据从第二个开始的1023个数据为0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0007(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1023];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, 1023);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0001, pucBuf, 1023),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001, pucBuf, 1023),"第三步测试失败");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, 1023);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0001,pucBuf,1023),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001,pucBuf,1023),"第六步测试失败");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第七步数据校验测试失败");
	}

	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"第八步测试失败");

	for(i=1; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "第九步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:起始地址为奇地址，读写长度为偶数时的读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress分别取g_uTestStartAddress + 0x0001
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1024
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向g_uTestStartAddress + 0x0001地址写入1024个字节数据0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1024个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashWrite函数向g_uTestStartAddress + 0x0001地址写入1024个字节数据0x88，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1024个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读出的数据都是0x88
//@EXECUTIONFLOW:8、调用CSUDIFlashRead函数读取第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读出的数据从第二个开始的1024个数据为0x88，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0008(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1024];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, 1024);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0001, pucBuf, 1024),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001, pucBuf, 1024),"第三步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0x88, 1024);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0001,pucBuf,1024),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001,pucBuf,1024),"第六步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x88 == pucBuf[i], "第七步数据校验测试失败");
	}
	
	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"第八步测试失败");

	for(i=1; i < 1025; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x88 == pucData[i], "第九步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:起始地址为偶地址，读写长度为奇数时的读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress取g_uTestStartAddress + 0x0006
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1023
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向g_uTestStartAddress + 0x0006地址写入1023个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0006地址读出1023个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashWrite函数向g_uTestStartAddress + 0x0006地址写入1023个0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0006地址读出1023个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读出的数据都是0xaa
//@EXECUTIONFLOW:8、调用CSUDIFlashRead函数读取第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读出的数据从第七个开始的1023个数据为0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0009(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1023];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, 1023);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0006, pucBuf, 1023),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006, pucBuf, 1023),"第三步测试失败");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, 1023);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0006,pucBuf,1023),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006,pucBuf,1023),"第六步测试失败");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第七步数据校验测试失败");
	}

	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"第八步测试失败");

	for(i=6; i < 1029; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "第九步数据校验测试失败");
	}

	for( ;  i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:起始地址为偶地址，读写长度为偶数时的读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress取g_uTestStartAddress + 0x0006
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1024
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite函数向g_uTestStartAddress + 0x0006地址写入1024个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0006地址读出1024个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashWrite函数向g_uTestStartAddress + 0x0006地址写入1024个0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0006地址读出1024个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读出的数据都是0xaa
//@EXECUTIONFLOW:8、调用CSUDIFlashRead函数读取第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读出的数据从第七个开始的1024个数据为0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0010(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1024];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, 1024);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0006, pucBuf, 1024),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006, pucBuf, 1024),"第三步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, 1024);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0006,pucBuf,1024),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006,pucBuf,1024),"第六步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第七步数据校验测试失败");
	}
	
	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"第八步测试失败");

	for(i=6; i < 1030; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "第九步数据校验测试失败");
	}

	for( ;  i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:测试多任务同时读写flash的不同block时的情况
//@PRECONDITION:Flash已成功初始化
//@INPUT:输入参数合法  
//@EXPECTATION:每一步都达到预期结果
//@EXECUTIONFLOW:1、创建写任务1，该任务将Flash第1个Block写入1024字节的数据0xaa
//@EXECUTIONFLOW:2、创建写任务2，该任务将Flash第2个Block写入1024字节的数据0x88
//@EXECUTIONFLOW:3、创建读任务1，该任务从Flash的第1个Block读出1024字节的数据，期望得到的数据都为0xaa
//@EXECUTIONFLOW:4、创建读任务2，该任务从Flash的第2个Block读出1024字节的数据，期望得到的数据都为0x88
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0011(void)
{
        int nPriority = 128;
	int nParam = 0;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle4 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread1", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashWrite1, &nParam, &hThreadhandle1),"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread2", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashWrite2, &nParam, &hThreadhandle2),"创建任务失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空");
		
	CSUDIOSThreadSleep(5000);

	while(!g_bFlashWrite1)
	{
		;
	}
	while(!g_bFlashWrite2)
	{
		;
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE != g_bFlagWrite1)&&(CSUDI_TRUE != g_bFlagWrite2),"测试失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread3", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashRead1, &nParam, &hThreadhandle3),"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread4", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashRead2, &nParam, &hThreadhandle4),"创建任务失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle4,"创建成功的任务输出的任务句柄不能为空");
	
	CSUDIOSThreadSleep(5000);

	while(!g_bFlashRead1)
	{
		CSUDIOSThreadSleep(2000);
	}
	while(!g_bFlashRead2)
	{
		CSUDIOSThreadSleep(2000);
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE != g_bFlagRead1)&&(CSUDI_TRUE != g_bFlagRead2),"测试失败");
			
	CSTK_FATAL_POINT
	{
		if (hThreadhandle1 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务1失败\n");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务2失败\n");
			   hThreadhandle2 = CSUDI_NULL;
		}
		if (hThreadhandle3 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "删除任务3失败\n");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle4 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle4), "删除任务4失败\n");
			   hThreadhandle2 = CSUDI_NULL;
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:测试多任务同时对同一block进行读写时的功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:输入参数合法
//@EXPECTATION:每一步都返回预期结果
//@REMARK:
//@EXECUTIONFLOW:1、CSUDIFlashWrite Flash 某个Block，数据为0x55，希望成功
//@EXECUTIONFLOW:2、创建任务1将Flash 某个Block的后1024个字节数据写为0x88
//@EXECUTIONFLOW:3、创建任务2将Flash 某个Block的后1024个字节数据写为0xaa
//@EXECUTIONFLOW:4、调用CSUDIFlashRead读取此前写的Block 的后1024个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、期望读到的数据为0x88或者为0xaa
//@EXECUTIONFLOW:6、调用CSUDIFlashRead读取此前写的Block 的0---(BolckSize-1024) 个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读到的数据为0x55
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0012(void)
{
	int i;
	CSUDI_UINT8 *pucBuf = CSUDI_NULL;
	
      	int nPriority = 128;
		
	int nParam = 0;
	
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;

	pucBuf = CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL((pucBuf != CSUDI_NULL),"分配空间失败\n");
	
	memset(pucBuf, 0x55, g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress+g_uTestFlashSize-g_uTestBlockSize, pucBuf, g_uTestBlockSize)), "Step fail");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread1", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashWrite3, &nParam, &hThreadhandle1),"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread2", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashWrite4, &nParam, &hThreadhandle2),"创建任务失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空");

	CSUDIOSThreadSleep(5000);
	
	while(!g_bFlashWrite3)
	{
		CSUDIOSThreadSleep(10);
	}
	while(!g_bFlashWrite4)
	{
		CSUDIOSThreadSleep(10);
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE != g_bFlagWrite3)&&(CSUDI_TRUE != g_bFlagWrite4),"任务执行内部失败");

	//为确保比较数据为内存中读出
	memset(pucBuf, 0, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + g_uTestFlashSize - g_uTestBlockSize, pucBuf, g_uTestBlockSize), "第三步测试失败");

	for(i=(g_uTestBlockSize-1024); i < 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((pucBuf[i] == 0x88)||(pucBuf[i] == 0xaa),"第五步数据校验测试失败");
	}

	for(i=0; i < (g_uTestBlockSize-1024); i++)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[i] == 0x55,"第七步数据校验测试失败");
	}
	
	CSTK_FATAL_POINT;
		
	if (pucBuf != CSUDI_NULL)
	{
		CSUDIOSFree(pucBuf);
		pucBuf = CSUDI_NULL;
	}
	
	if (hThreadhandle1 != CSUDI_NULL)
	{
		  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务失败");
		   hThreadhandle1 = CSUDI_NULL;
	}
	
	if (hThreadhandle2 != CSUDI_NULL)
	{
		  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务失败");
		   hThreadhandle2 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION: 对同一地址反复进行读写时的强度测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1024
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite向FLASH中写入1024个字节的数据,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead从FLASH中读取1024个字节的数据,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望得到的数据与第二步写入的数据相同
//@EXECUTIONFLOW:5、重复2--4步REPERT_COUNT次，期望每次都达到预期结果
//@EXECUTIONFLOW:6、调用CSUDIFlashRead从FLASH中读取第一个block的数据,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望得到的数据前1024个与写入的数据相同，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0013(void)
{
	int i;
	CSUDI_UINT8 pucBuf[1024];
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	for(i=0; i < 1024; ++i)
	{
		pucBuf[i] = i%256;
	}
	
	for(i=0; i < REPERT_COUNT; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf, 1024),"第二步测试失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf, 1024),"第三步测试失败");

		for(i=0; i < 1024; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(i%256 == pucBuf[i], "第四步数据校验测试失败");
		}
	}
	memset(pucData, '\0', g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"第八步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i%256 == pucData[i], "第九步数据校验测试失败");
	}

	for( ;  i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION: 写全为0xff数据测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWrite向FLASH中一个扇区全为0xff的数据,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead从FLASH中读取一个扇区的数据,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望得到的数据与第二步写入的数据相同
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0014(void)
{
	int i;
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");
	
	memset(pucData, 0xff, g_uTestBlockSize);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucData, g_uTestBlockSize),"第二步测试失败");
	
	memset(pucData, 0x00, g_uTestBlockSize);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucData, g_uTestBlockSize),"第三步测试失败");

	for( i=0;  i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第九步数据校验测试失败");
	}
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:对整个测试的FLASH空间进行不带擦除写及读时的读写功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestFlashSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestFlashSize为可用于测试的flash空间大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除整个测试的FLASH空间期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入g_uTestFlashSize个字节0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestFlashSize个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除整个测试的FLASH空间期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入g_uTestFlashSize个字节0xbb，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestFlashSize个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xbb
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0001(void)
{
	int i;
	unsigned int unLeftSzie = g_uTestFlashSize;
	unsigned int unOperSize = 0;
	unsigned int unMallocSzie = 0;
	unsigned int unTestStartAddr = g_uTestStartAddress;
	CSUDI_UINT8 *pucBuf = NULL;
	

	if (g_uTestFlashSize > MAX_FLASH_MALLOC_SIZE)
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);
		unMallocSzie = g_uALLMaxBlockSize;
	}
	else
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestFlashSize);
		unMallocSzie = g_uTestFlashSize;
	}
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTCPrint("测试运行中，请等待!!\n");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize),"第一步测试失败");

	CSTCPrint("第一步测试成功!!\n");	
	
	unLeftSzie = g_uTestFlashSize;
	unTestStartAddr = g_uTestStartAddress;
	while (unLeftSzie > 0)
	{
		memset(pucBuf, 0x55, unMallocSzie);
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(unTestStartAddr,pucBuf,unOperSize),"第二步测试失败");

		memset(pucBuf, '\0', unMallocSzie);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"第三步测试失败");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
		}

		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize),"第五步测试失败");
	unLeftSzie = g_uTestFlashSize;
	unTestStartAddr = g_uTestStartAddress;
	
	while (unLeftSzie > 0)
	{
		memset(pucBuf, 0xbb, unMallocSzie);

		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(unTestStartAddr,pucBuf, unOperSize),"第六步测试失败");

		memset(pucBuf, '\0', unOperSize);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"第七步测试失败");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "第八步数据校验测试失败");
		}

		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}

 	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:块内不带擦除写及读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 16
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入16个字节pubBuf指向的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出16个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据和写入的数据相同
//@EXECUTIONFLOW:5、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出第一个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望读出的数据前16个字节数据与pubBuf指向的数据相同，其余的全为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0002(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[16];
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");
	
	for(i = 0; i < 16; ++i)
	{
		pucBuf[i] = i;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 16),"第二步测试失败");

	memset(pucBuf, '\0', 16);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 16),"第三步测试失败");

	for(i=0; i < 16; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i == pucBuf[i], "第四步数据校验测试失败");
	}
	
	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"第五步测试失败");

	for(i=0; i < 16; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i == pucData[i], "第六步数据校验测试失败");
	}
	
	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第六步数据校验测试失败");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:整块不带擦出写读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除前两个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向第一个block内写入g_uTestBlockSize个字节的数据0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入g_uTestBlockSize个字节的0xee，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xee
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出前两个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读出的数据第一个block的数据为0xee，第二个block的数据为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0003(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, 2*g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第二步测试失败");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第三步测试失败");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xee, 2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第五步测试失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"第六步测试失败");

	memset(pucBuf, '\0',2* g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"第七步测试失败");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xee == pucBuf[i], "第八步数据校验测试失败");
	}
	
	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 2*g_uTestBlockSize),"第九步测试失败");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xee== pucBuf[i], "第十步数据校验测试失败");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第十步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:跨块不带擦除写及读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress值合法
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize + 512
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除前两个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入g_uTestBlockSize + 512个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize + 512个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除前两个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入g_uTestBlockSize + 512个0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize + 512个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xaa
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出前两个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读出的数据前g_uTestBlockSize + 512是0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0004(void)
{
	
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, 2*g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, g_uTestBlockSize + 512),"第二步测试失败");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize + 512),"第三步测试失败");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, 2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第五步测试失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,g_uTestBlockSize + 512),"第六步测试失败");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize + 512),"第七步测试失败");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第八步数据校验测试失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 2*g_uTestBlockSize),"第九步测试失败");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucBuf[i], "第十步数据校验测试失败");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第十步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:右侧block对齐，左侧block不对齐时不带擦除写及读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress + 512
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize - 512
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 512地址写入g_uTestBlockSize - 512个字节数据0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 512地址读出g_uTestBlockSize - 512个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 512地址写入g_uTestBlockSize - 512个字节数据0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress + 512地址读出g_uTestBlockSize - 512个字节数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xaa
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出第一个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读出的数据前512个字节是0xff，其余的全部为0xaa
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0005(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第一步测试失败");


	memset(pucBuf, 0x55, 2*g_uTestBlockSize);

	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 512, pucBuf, g_uTestBlockSize - 512),"第二步测试失败");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 512, pucBuf, g_uTestBlockSize - 512),"第三步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, 2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第五步测试失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 512,pucBuf,g_uTestBlockSize - 512),"第六步测试失败");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 512,pucBuf,g_uTestBlockSize - 512),"第七步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第八步数据校验测试失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 2*g_uTestBlockSize),"第九步测试失败");

	for(i=0; i < 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff== pucBuf[i], "第十步数据校验测试失败");
	}

	for(i=512; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第十步数据校验测试失败");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第十步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:左侧block对齐，右侧block不对齐时不带擦除写及读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize - 512
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入g_uTestBlockSize - 512个字节的0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize - 512个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入g_uTestBlockSize - 512个字节的0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize - 512个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xaa
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出第一个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读出的数据前g_uTestBlockSize - 512个字节是0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0006(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, 2*g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, g_uTestBlockSize - 512),"第二步测试失败");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize - 512),"第三步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, 2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"第五步测试失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,g_uTestBlockSize - 512),"第六步测试失败");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize - 512),"第七步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第八步数据校验测试失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 2*g_uTestBlockSize),"第九步测试失败");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第十步数据校验测试失败");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第十步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");

	return CSUDI_TRUE;	
}


//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:起始地址为奇地址，读写长度为奇数时不带擦除写及读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress分别取g_uTestStartAddress + 0x0001
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1023
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 0x0001地址写入1023个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1023个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 0x0001地址写入1023个0xbb，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1023个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xbb
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数读取第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读出的数据从第二个开始的1023个数据为0x88，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0007(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0001, pucBuf, 1023),"第二步测试失败");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001, pucBuf, 1023),"第三步测试失败");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xbb, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第五步测试失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0001,pucBuf,1023),"第六步测试失败");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001,pucBuf,1023),"第七步测试失败");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "第八步数据校验测试失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第九步测试失败");

	for(i=1; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xbb== pucBuf[i], "第十步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第十步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:起始地址为奇地址，读写长度为偶数时不带擦除写及读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress分别取g_uTestStartAddress + 0x0001
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1024
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 0x0001地址写入1024个字节数据0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1024个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 0x0001地址写入1024个字节数据0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1024个字节的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xaa
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数读取第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读出的数据从第二个开始的1024个数据为0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0008(void)
{	
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0001, pucBuf, 1024),"第二步测试失败");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001, pucBuf, 1024),"第三步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第五步测试失败");

	memset(pucBuf, 0xaa, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0001,pucBuf,1024),"第六步测试失败");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001,pucBuf,1024),"第七步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第八步数据校验测试失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第九步测试失败");

	for(i=1; i < 1025; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucBuf[i], "第十步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第十步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:起始地址为偶地址，读写长度为奇数时不带擦除写及读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress取g_uTestStartAddress + 0x0006
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1023
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 0x0006地址写入1023个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0001地址读出1023个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 0x0006地址写入1023个0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0006地址读出1023个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xaa
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数读取第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读出的数据从第七个开始的1023个数据为0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0009(void)
{	
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0006, pucBuf, 1023),"第二步测试失败");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006, pucBuf, 1023),"第三步测试失败");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(pucBuf, 0xaa, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第五步测试失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0006,pucBuf,1023),"第六步测试失败");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006,pucBuf,1023),"第七步测试失败");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第八步数据校验测试失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第九步测试失败");

	for(i=6; i < 1029; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucBuf[i], "第十步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第十步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:起始地址为偶地址，读写长度为偶数时不带擦除写及读功能测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress取g_uTestStartAddress + 0x0006
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1024
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 0x0006地址写入1024个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0006地址读出1024个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读出的数据都是0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress + 0x0006地址写入1024个0xaa，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIFlashRead函数从g_uTestStartAddress + 0x0006地址读出1024个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读出的数据都是0xaa
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数读取第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读出的数据从第七个开始的1024个数据为0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0010(void)
{	
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0006, pucBuf, 1024),"第二步测试失败");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006, pucBuf, 1024),"第三步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第五步测试失败");

	memset(pucBuf, 0xaa, g_uTestBlockSize);
 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0006,pucBuf,1024),"第六步测试失败");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006,pucBuf,1024),"第七步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第八步数据校验测试失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第九步测试失败");

	for(i=6; i < 1030; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucBuf[i], "第十步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "第十步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION: 对同一地址反复进行不带擦除写及读时的强度测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 1024
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace函数擦除第一个block内的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase向FLASH中写入1024个字节的数据0xaa,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIFlashRead从FLASH中读取1024个字节的数据,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望得到的数据全部为0xaa
//@EXECUTIONFLOW:5、重复1--4步REPERT_COUNT次，期望每次都达到预期结果
//@EXECUTIONFLOW:6、调用CSUDIFlashRead从FLASH中读取第一个block的数据,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望得到的数据前1024个为0xaa，其余的全部为0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0011(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1024];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"分配内存失败");

	memset(pucBuf, 0xaa, sizeof(pucBuf));
	
	for(i=0; i < REPERT_COUNT; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第一步测试失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf, 1024),"第二步测试失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf, 1024),"第三步测试失败");

		for(i=0; i < 1024; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "第四步数据校验测试失败");
		}
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"第五步测试失败");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucData[i], "第六步数据校验测试失败");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "第六步数据校验测试失败");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"销毁资源失败");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase 
//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:擦除、不带擦除写、擦除写及读整个测试的FLASH空间时的时间效率测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestFlashSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestFlashSize为可用于测试的flash大小
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite函数向g_uTestStartAddress地址写入g_uTestFlashSize个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、期望得到完成擦除写操作所用的时间
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestFlashSize个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、得到完成读操作所用的时间，且期望得到的数据全部为0x55
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除整个flash内容，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望得到完成擦除操作所用的时间
//@EXECUTIONFLOW:7、调用CSUDIFlashWriteWithoutErase函数向g_uTestStartAddress地址写入g_uTestFlashSize个0x55，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望得到完成不带擦除写操作所用的时间
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestFlashSize个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望得到的数据全部为0x55
//@EXECUTIONFLOW:11、打印以上各种读写操作耗费的时间
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0001(void)
{
	int i = 0;
	
	//记录擦除写所用的时间(单位毫秒)
	int nTemp1 = 0;
	//记录读所用的时间(单位毫秒)
	int nTemp2 = 0;
	//记录擦除所用的时间(单位毫秒)
	int  nTemp3 = 0;
	//记录不带擦除写所用的时间	(单位毫秒)
	int nTemp4 = 0;
	
	CSUDIOSTimeVal_S sSysTime1;
	CSUDIOSTimeVal_S sSysTime2;

	unsigned int unLeftSzie = g_uTestFlashSize;
	unsigned int unOperSize = 0;
	unsigned int unMallocSzie = 0;
	unsigned int unTestStartAddr = g_uTestStartAddress;
	CSUDI_UINT8 *pucBuf = NULL;
	
	if (g_uTestFlashSize > MAX_FLASH_MALLOC_SIZE)
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);
		unMallocSzie = g_uALLMaxBlockSize;
	}
	else
	{
		pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestFlashSize);
		unMallocSzie = g_uTestFlashSize;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	CSTCPrint("测试运行中，请等待!!\n");

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	memset(pucBuf, 0x55, unMallocSzie);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");

	while (unLeftSzie)
	{
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(unTestStartAddr,pucBuf,unOperSize),"第一步测试失败");
		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");

	nTemp1 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");

	while (unLeftSzie)
	{
		memset(pucBuf, '\0', unMallocSzie);
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr, pucBuf, unOperSize),"第三步测试失败");
		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第四步数据校验测试失败");
		}
	}

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp2 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize),"第五步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp3 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	memset(pucBuf, 0x55, unMallocSzie);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");

	while (unLeftSzie)
	{
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(unTestStartAddr,pucBuf,unOperSize),"第七步测试失败");
		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp4 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	while (unLeftSzie)
	{
		memset(pucBuf, 0x00, unMallocSzie);
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"第九步测试失败");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "第十步数据校验测试失败");
		}

		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}

	CSTCPrint("\n********整个测试的FLASH空间大小为0x%x(字节)\n", g_uTestFlashSize);
	CSTCPrint("\n********擦除写整个测试的FLASH空间所用的时间:%d(毫秒)\n********不带擦除写整个测试的FLASH空间所用的时间:%d(毫秒)", nTemp1, nTemp4);
	CSTCPrint("\n********读整个测试的FLASH空间所用的时间:%d(毫秒)\n********擦除整个测试的FLASH空间所用的时间:%d(毫秒)\n", nTemp2, nTemp3);

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWriteWithoutErase 
//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:擦除、不带擦除写、擦除写及读一个Block时的时间效率测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestBlockSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite函数把第一个block的数据全部写为0xbb，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、期望得到完成擦除写一个block所用的时间
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出g_uTestBlockSize个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、得到完成读一个block操作所用的时间，且期望得到的数据全部为0xbb
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除第一个block的内容，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望得到完成擦除一个block所用的时间
//@EXECUTIONFLOW:7、调用CSUDIFlashWriteWithoutErase函数把第一个block的数据全部写为0xbb，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望得到完成不带擦除写一个block所用的时间
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出第一个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望得到的数据全部为0xbb
//@EXECUTIONFLOW:11、打印以上各种读写操作耗费的时间
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0002(void)
{
	int i = 0;
	
	//记录擦除写所用的时间(单位毫秒)
	int nTemp1 = 0;
	//记录读所用的时间(单位毫秒)
	int nTemp2 = 0;
	//记录擦除所用的时间(单位毫秒)
	int  nTemp3 = 0;
	//记录不带擦除写所用的时间	(单位毫秒)
	int nTemp4 = 0;
	
	CSUDIOSTimeVal_S sSysTime1;
	CSUDIOSTimeVal_S sSysTime2;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
		
	memset(pucBuf, 0xbb, g_uTestBlockSize);

	CSTCPrint("测试运行中，请等待!!\n");

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"第一步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp1 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	memset(pucBuf, '\0', g_uTestBlockSize);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"第三步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp2 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);
	
	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"第五步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp3 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	memset(pucBuf, 0xbb, g_uTestBlockSize);	

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"第七步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp4 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"第九步测试失败");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "第十步数据校验测试失败");
	}

	CSTCPrint("\n********一个Block大小为0x%x(字节)\n", g_uTestBlockSize);
	CSTCPrint("\n********擦除写一个Block所用的时间:%d(毫秒)\n********不带擦除写一个Block所用的时间:%d(毫秒)", nTemp1, nTemp4);
	CSTCPrint("\n********读一个Block所用的时间:%d(毫秒)\n********擦除一个Block所用的时间:%d(毫秒)\n", nTemp2, nTemp3);

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWriteWithoutErase 
//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:擦除、不带擦除写、擦除写及读多个Block时的时间效率测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 3*g_uTestBlockSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@EXECUTIONFLOW:1、调用CSUDIFlashWrite函数把前三个block的数据全部写为0x66，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、期望得到完成擦除写三个block所用的时间
//@EXECUTIONFLOW:3、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出3*g_uTestBlockSize个数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、得到完成读三个block操作所用的时间，且期望得到的数据全部为0x66
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace函数擦除前三个block的内容，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望得到完成擦除三个block所用的时间
//@EXECUTIONFLOW:7、调用CSUDIFlashWriteWithoutErase函数把前三个block的数据全部写为0x66，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望得到完成不带擦除写三个block所用的时间
//@EXECUTIONFLOW:9、调用CSUDIFlashRead函数从g_uTestStartAddress地址读出前三个block的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望得到的数据全部为0x66
//@EXECUTIONFLOW:11、打印以上各种读写操作耗费的时间
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0003(void)
{
	int i = 0;
	
	//记录擦除写所用的时间(单位毫秒)
	int nTemp1 = 0;
	//记录读所用的时间(单位毫秒)
	int nTemp2 = 0;
	//记录擦除所用的时间(单位毫秒)
	int  nTemp3 = 0;
	//记录不带擦除写所用的时间	(单位毫秒)
	int nTemp4 = 0;
	
	CSUDIOSTimeVal_S sSysTime1;
	CSUDIOSTimeVal_S sSysTime2;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(3*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
		
	memset(pucBuf, 0x66, 3*g_uTestBlockSize);

	CSTCPrint("测试运行中，请等待!!\n");

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf,3*g_uTestBlockSize),"第一步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp1 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);
	
	memset(pucBuf, '\0', 3*g_uTestBlockSize);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 3*g_uTestBlockSize),"第三步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp2 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);
	
	for(i=0; i < 3*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x66 == pucBuf[i], "第四步数据校验测试失败");
	}

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 3*g_uTestBlockSize),"第五步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp3 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	memset(pucBuf, 0x66, 3*g_uTestBlockSize);	

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"获取系统时间失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,3*g_uTestBlockSize),"第七步测试失败");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"获取系统时间失败");
	
	nTemp4 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,3*g_uTestBlockSize),"第九步测试失败");

	for(i=0; i < 3*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x66 == pucBuf[i], "第十步数据校验测试失败");
	}

	CSTCPrint("\n********一个Block大小为0x%x(字节)\n", g_uTestBlockSize);
	CSTCPrint("\n********擦除写三个Block所用的时间:%d(毫秒)\n********不带擦除写三个Block所用的时间:%d(毫秒)", nTemp1, nTemp4);
	CSTCPrint("\n********读三个Block所用的时间:%d(毫秒)\n********擦除三个Block所用的时间:%d(毫秒)\n", nTemp2, nTemp3);

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"销毁资源失败");

	return CSUDI_TRUE;
}

/*lichanghua modify:最初的设置时64和1000,造成测试时间长达几个小时,没有必要*/
#define BLOCK_DIVIDE_FACTOR  (8)

#define LOOP_COUNT             (2)

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWriteWithoutErase 
//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:flash读写测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = 3*g_uTestBlockSize
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、依次向flash的每个块写入数据，再读取校验
//@EXECUTIONFLOW:2、步骤1循环执行100遍，每遍验证数据是否一致
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestBlockSize为可用于测试的block的大小
//@REMARK: 任何操作(创建、删除、读写)失败将返回CSUDI_FAILURE,并打印错误信息: "error occur at i = ? while call [XXX] func!"\r\n 
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0004(void)
{
	unsigned char *pBuffer1 = CSUDI_NULL;
	
	unsigned char *pBuffer2 = CSUDI_NULL;
	
	CSUDI_UINT32  addr = 0;
	
	CSUDI_UINT32  addr1 = 0;
	
	CSUDI_BOOL ret = CSUDI_TRUE;
	
	int i,j,k;
	
	int testcount = 0;
	
	int  eErrorCode = CSUDI_FAILURE;

	pBuffer1 = CSUDIOSMalloc(g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
	if(pBuffer1 == NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0,"读缓冲区分配内存失败\n");
	}
	
	pBuffer2 = CSUDIOSMalloc(g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);	
	if(pBuffer2 == NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0,"校验缓冲区分配内存失败\n");
	}

	for(i = 0; i < LOOP_COUNT; i++)
	{
		for(j = 0; j < g_uTestFlashSize/g_uTestBlockSize;  j++)
		{
			addr = g_uTestStartAddress + j*g_uTestBlockSize;


			//step1: erase a 64*1024 block.
			eErrorCode = CSUDIFlashEraseSpace(addr, g_uTestBlockSize);
			CSTK_ASSERT_TRUE_FATAL(eErrorCode == 0,"CSUDIFlashEraseSpace 失败\n");

			//step2: write data to 8*(8*1024) bytes region in every block.
			for(k = 0; k < BLOCK_DIVIDE_FACTOR; k++)
			{
				addr1 = addr + k*(g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				memset(pBuffer1, i + j + k, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				eErrorCode = CSUDIFlashWriteWithoutErase(addr1,pBuffer1, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				CSTK_ASSERT_TRUE_FATAL(eErrorCode == 0,"CSUDIFlashWriteWithoutErase 失败\n");
			}

			//step3: check data of  8*(8*1024) bytes region in every block.
			for(k = 0; k < BLOCK_DIVIDE_FACTOR; k++)
			{
				addr1 = addr + k*(g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				memset(pBuffer1, i + j + k, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				memset(pBuffer2, 0, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				eErrorCode = CSUDIFlashRead(addr1, pBuffer2, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				CSTK_ASSERT_TRUE_FATAL(eErrorCode == 0,"CSUDIFlashRead 失败\n");
				
				for(testcount = 0; testcount < g_uTestBlockSize/BLOCK_DIVIDE_FACTOR; testcount++)
				{
					if(pBuffer1[testcount] != pBuffer2[testcount])
					{
						CSTCPrint("\n...........check write data  FAILURE!!!!!!!!!!!!!!!!!!\r\n");
						
						CSTCPrint("[check data error!!] ucReadBuffer[%d] == 0x%x...., addr1=0x%x\r\n", testcount, pBuffer2[testcount], addr1);
						
						CSTCPrint("[check data error!!] ucWriteData[%d]  == 0x%x ...\r\n", testcount, pBuffer1[testcount]);
						
						CSTCPrint("########## strncmp error i = %d  j = %d k = %d ##########\r\n\n", i, j, k);
						
						ret = CSUDI_FALSE;
					}
				}
			}

		}

		CSTCPrint("# rate: %d%%\n", (i+1)*100/LOOP_COUNT);
	}
	
	CSTK_FATAL_POINT;
	
	if (pBuffer1 != CSUDI_NULL)
	{
		CSUDIOSFree(pBuffer1);
	}
	if (pBuffer2 != CSUDI_NULL)
	{
		CSUDIOSFree(pBuffer2);
	}

	return ret;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:测试系统对FLASH空间进行读写时的时间统计
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf 值合法
//@INPUT:uDataLength = g_uTestFlashSize
//@EXPECTATION:每一步都返回预期结果
//@REMARK:g_uTestStartAddress为可用于测试的flash空间起始地址，g_uTestFlashSize为可用于测试的flash空间大小
//@EXECUTIONFLOW:1、比较用于测试的Flash大小是否小于可供用户测试的Flash大小，期望(可供用户测试使用的Flash>1M)
//@EXECUTIONFLOW:2、调用CSUDIOSMalloc()申请1M内存大小用于测试，并赋初值0x55
//@EXECUTIONFLOW:3、获取当前时间，调用CSUDIFlashWrite()写入1M的内容，循环10次，并计算相应的写入速度
//@EXECUTIONFLOW:4、获取当前时间，调用CSUDIFlashRead()读入1M的内容，循环10次，并计算相应的读入速度，期望值为0x55
//@EXECUTIONFLOW:5、将内存内容赋值为0xbb
//@EXECUTIONFLOW:6、获取当前时间，调用CSUDIFlashWrite()写入10K的内容，循环100次，并计算相应的写入速度
//@EXECUTIONFLOW:7、获取当前时间，调用CSUDIFlashRead()读入10K的内容，循环100次，并计算相应的读入速度，期望值为0xbb
//@EXECUTIONFLOW:7、打印结果，并销毁所使用的内存资源
CSUDI_BOOL  CSTC_FLASH_IT_Performance_0001(void)
{
	CSUDI_INT32 i , j;
	
	CSUDI_INT32 nMemorySize10K = 10 * 1024 ;
	CSUDI_INT32 nMemorySize1M  = 1024 * 1024 ;
	
	CSUDI_UINT8 *pucBuf = CSUDI_NULL;
	
	CSUDIOSTimeVal_S sSysTime;
	
	CSUDIOSTimeVal_S  temp1;
	
	CSUDIOSTimeVal_S  temp2;
	
	CSUDIOSTimeVal_S  temp3;
	
	CSUDIOSTimeVal_S  temp4;
	
	CSTK_ASSERT_TRUE_FATAL( g_uTestFlashSize >= nMemorySize1M ,"第一步测试失败");
	
	pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc( nMemorySize1M );
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");
	
	memset(pucBuf, 0x55, nMemorySize1M );

	CSTCPrint("测试运行中，请等待!!\n");
	
	//写入1M的内容，循环10次，并计算相应的写入速度

	CSTCPrint("正在进行写入1M,循环10次的flash速度测试... \n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp1),"第三步获取系统时间失败");
	
	for( i=0; i < 10 ; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, nMemorySize1M),"第三步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"第三步获取系统时间失败");
		
	temp1.m_nSecond = sSysTime.m_nSecond - temp1.m_nSecond;
	temp1.m_nMiliSecond = sSysTime.m_nMiliSecond - temp1.m_nMiliSecond;
	
	//读入1M的内容，循环10次，并计算相应的读入速度

	CSTCPrint("正在进行读入1M,循环10次的flash速度测试... \n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp2),"第四步获取系统时间失败");
	
	for( i=0; i < 10 ; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, nMemorySize1M ),"第四步测试失败");

		for( j=0; j< nMemorySize1M ; ++j )
		{
		    CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[j], "第四步数据校验测试失败");
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"第四步获取系统时间失败");
	
	temp2.m_nSecond = sSysTime.m_nSecond - temp2.m_nSecond;
	temp2.m_nMiliSecond = sSysTime.m_nMiliSecond - temp2.m_nMiliSecond;
	
	memset(pucBuf, 0xbb, nMemorySize1M );
	
	//写入10K的内容，循环100次，并计算相应的读入速度

	CSTCPrint("正在进行写入10K,循环100次的flash速度测试... \n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp3),"第六步获取系统时间失败");
	
	for( i=0; i < 100 ; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf, nMemorySize10K ),"第六步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"第六步获取系统时间失败");
		
	temp3.m_nSecond = sSysTime.m_nSecond - temp3.m_nSecond;
	temp3.m_nMiliSecond = sSysTime.m_nMiliSecond - temp3.m_nMiliSecond;
	
	//读入10K的内容，循环100次，并计算相应的读入速度，

	CSTCPrint("正在进行读入10K,循环100次的flash速度测试... \n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp4),"第七步获取系统时间失败");
	
	for( i=0; i < 100 ; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, nMemorySize10K ),"第七步测试失败");
		
		for( j=0; j< nMemorySize10K ; ++j )
		{
		    CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[j], "第四步数据校验测试失败");
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"第七步获取系统时间失败");
	
	temp4.m_nSecond = sSysTime.m_nSecond - temp4.m_nSecond;
	temp4.m_nMiliSecond = sSysTime.m_nMiliSecond - temp4.m_nMiliSecond;
	
	CSTCPrint("每次写入1MB数据，循环10次的速率为：%0.2f M/s \n" , (1.0 * 10) / ((temp1.m_nSecond*1000) + temp1.m_nMiliSecond ) * 1000 );
	CSTCPrint("每次读入1MB数据，循环10次的速率为：%0.2f M/s \n" , (1.0 * 10) / ((temp2.m_nSecond*1000) + temp2.m_nMiliSecond ) * 1000 );
	CSTCPrint("每次写入10K数据，循环100次的速率为：%0.2f M/s \n" , (10.0 * 100) / ((temp3.m_nSecond*1000) + temp3.m_nMiliSecond ) * 1000 / 1024);
	CSTCPrint("每次读入10K数据，循环100次的速率为：%0.2f M/s \n" , (10.0 * 100) / ((temp4.m_nSecond*1000) + temp4.m_nMiliSecond ) * 1000 / 1024);
	
 	CSTK_FATAL_POINT
 	{

	   CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"销毁资源失败");
    }
	
	return CSUDI_TRUE;
}


//返回(min)到(max-1)之间的随机数
static int GetRandom(int min, int max)
{ 
	if(min > max)
	{
		CSTCPrint("传入的最大最小值有误,min=%d,max=%d\n", min, max);
		return CSUDI_FALSE;
	}	
	int nRet = ((rand()%((max) - (min))) + (min)); 
	return ( nRet == max ? (nRet-1) : nRet); 
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:随机地址随机大小读写测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:flash测试的起始地址
//@INPUT:flash测试的大小
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、获取flash基本信息，如扇区大小
//@EXECUTIONFLOW:2、随机生成flash的起始地址，写大小，写入数据并调用CSUDIFlashWrite写入数据
//@EXECUTIONFLOW:3、调用CSUDIFlashRead读取数据
//@EXECUTIONFLOW:4、校验数据
//@EXECUTIONFLOW:5、调用CSUDIFlashEraseSpace擦除随机大小
//@EXECUTIONFLOW:6、校验数据
//@EXECUTIONFLOW:6、重复步骤2, 3, 4, 5, 6    100次
//@EXECUTIONFLOW:7、销毁资源
CSUDI_BOOL  CSTC_FLASH_IT_Performance_0005(void)
{
	CSUDIFLASHRegionSuit_S sFlashSuit;
	DWORD unBlkSize, dwTestAddr, dwTestArea, dwTestSize, dwWriteSize;
	BYTE ucWriteValue = 0;
	PBYTE pWriteBuf = NULL;
	int ii, nTestTime;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashGetInfo(EM_UDI_FLASH_ALL, &sFlashSuit), "第一步获取flash信息失败");
	unBlkSize = sFlashSuit.m_uMaxBlockSize;
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (2 *unBlkSize), "用户可测试空间太小");
	
	pWriteBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*unBlkSize);
	CSTK_ASSERT_TRUE_FATAL(pWriteBuf != NULL, "分配空间失败");

	nTestTime = 100;
	
	while (nTestTime--)
	{
		dwTestAddr = GetRandom(g_uTestStartAddress, g_uTestStartAddress + g_uTestFlashSize-1);
		dwTestArea = g_uTestStartAddress + g_uTestFlashSize - dwTestAddr;
		dwTestSize = dwTestArea > (2*unBlkSize) ? (2*unBlkSize) : dwTestArea;
		dwWriteSize = GetRandom(1, dwTestSize);

		ucWriteValue = GetRandom(1, 255);
		memset(pWriteBuf, ucWriteValue, 2 * unBlkSize);
		CSTCPrint("flash 写开始dwTestAddr=0x%x, dwWriteSize=0x%x, unBlkSize=0x%x\n", dwTestAddr, dwWriteSize, unBlkSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(dwTestAddr, pWriteBuf, dwWriteSize), "第二步写flash失败");	
		CSTCPrint("flash 写完毕\n");

		CSTCPrint("flash 读开始dwTestAddr=0x%x, dwWriteSize=0x%x\n", dwTestAddr, dwWriteSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(dwTestAddr, pWriteBuf, dwWriteSize), "第三步读flash失败");
		CSTCPrint("flash 读完毕\n");
		for (ii=0; ii<dwWriteSize; ii++)
		{
			CSTK_ASSERT_TRUE_FATAL(pWriteBuf[ii] == ucWriteValue, "第四步校验flash失败");
		}

		CSTCPrint("flash 擦开始\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize), "第五步擦flash失败");	

		CSTCPrint("flash 读开始dwTestAddr=0x%x, dwWriteSize=0x%x\n", dwTestAddr, dwWriteSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(dwTestAddr, pWriteBuf, dwWriteSize), "第六步读flash失败");

		for (ii=0; ii<dwWriteSize; ii++)
		{
			CSTK_ASSERT_TRUE_FATAL(pWriteBuf[ii] == 0xff, "第七步擦除后校验失败");
		}

		CSTCPrint("校验数据完毕\n");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pWriteBuf), "销毁资源失败");

	return CSUDI_TRUE;
}

static void RandWriteFlashThread1(void)
{
	CSUDIFLASHRegionSuit_S sFlashSuit;
	DWORD unBlkSize = 0;
	PBYTE pWriteBuf = NULL;
	int nTestTime = 100;
	DWORD dwTestAddr, dwTestArea, dwTestSize, dwWriteSize, dwTwoBlockSize;
	BYTE ucWriteValue;
	int ii;
	
	if (CSUDI_SUCCESS != CSUDIFlashGetInfo(EM_UDI_FLASH_ALL, &sFlashSuit))
	{
		CSTCPrint("第一步获取flash信息失败\n");
		bRandRunFlag1 = FALSE;
		goto done;
	}
	
	unBlkSize = sFlashSuit.m_uMaxBlockSize;
	if (g_uTestFlashSize < (4 *unBlkSize))
	{
		CSTCPrint("用户可测试空间太小\n");
		bRandRunFlag1 = FALSE;
		goto done;
	}
	
	pWriteBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*unBlkSize);
	if (pWriteBuf == NULL)
	{
		CSTCPrint("分配空间失败\n");
		bRandRunFlag1 = FALSE;
		goto done;
	}
	
	while (nTestTime--)
	{
		dwTwoBlockSize = 2 * unBlkSize;
		dwTestAddr = GetRandom(g_uTestStartAddress, g_uTestStartAddress + dwTwoBlockSize-1);
		dwTestArea = g_uTestStartAddress + dwTwoBlockSize - dwTestAddr;
		dwTestSize = dwTestArea > (2*unBlkSize) ? (2*unBlkSize) : dwTestArea;
		dwWriteSize = GetRandom(1, dwTestSize);

		ucWriteValue = GetRandom(1, 255);
		memset(pWriteBuf, ucWriteValue, 2 * unBlkSize);
		CSTCPrint("flash 写开始dwTestAddr=0x%x, dwWriteSize=0x%x, unBlkSize=0x%x\n", dwTestAddr, dwWriteSize, unBlkSize);
		if (CSUDI_SUCCESS != CSUDIFlashWrite(dwTestAddr, pWriteBuf, dwWriteSize))
		{
			CSTCPrint("第二步写flash失败\n");
			bRandRunFlag1 = FALSE;
			break;
		}
		
		CSTCPrint("flash 写完毕\n");

		CSTCPrint("flash 读开始dwTestAddr=0x%x, dwWriteSize=0x%x\n", dwTestAddr, dwWriteSize);
		if (CSUDI_SUCCESS != CSUDIFlashRead(dwTestAddr, pWriteBuf, dwWriteSize))
		{
			CSTCPrint("第三步写flash失败\n");
			bRandRunFlag1 = FALSE;
			break;
		}
		
		CSTCPrint("flash 读完毕\n");
		for (ii=0; ii<dwWriteSize; ii++)
		{
			if (pWriteBuf[ii] != ucWriteValue)
			{
				CSTCPrint("第四步校验flash失败");
				bRandRunFlag1 = FALSE;
				break;
			}
		}

		if (!bRandRunFlag1)
		{
			break;
		}
		CSTCPrint("校验数据完毕\n");
	}

	if (CSUDI_SUCCESS != CSUDIOSFree(pWriteBuf))
	{
		CSTCPrint("销毁资源失败\n");
	}

done:
	RandThread1Done = TRUE;
}

static void RandWriteFlashThread2(void)
{
	CSUDIFLASHRegionSuit_S sFlashSuit;
	DWORD unBlkSize = 0;
	PBYTE pWriteBuf = NULL;
	int nTestTime = 100;
	DWORD dwTestAddr, dwTestArea, dwTestSize, dwWriteSize, dwTwoBlockSize;
	BYTE ucWriteValue;
	int ii;
	
	if (CSUDI_SUCCESS != CSUDIFlashGetInfo(EM_UDI_FLASH_ALL, &sFlashSuit))
	{
		CSTCPrint("第一步获取flash信息失败\n");
		bRandRunFlag2 = FALSE;
		goto done;
	}
	
	unBlkSize = sFlashSuit.m_uMaxBlockSize;
	if (g_uTestFlashSize < (4 *unBlkSize))
	{
		CSTCPrint("用户可测试空间太小\n");
		bRandRunFlag2 = FALSE;
		goto done;
	}
	
	pWriteBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*unBlkSize);
	if (pWriteBuf == NULL)
	{
		CSTCPrint("分配空间失败\n");
		bRandRunFlag2 = FALSE;
		goto done;
	}
	
	while (nTestTime--)
	{
		dwTwoBlockSize = 2 * unBlkSize;
		dwTestAddr = GetRandom(g_uTestStartAddress+dwTwoBlockSize, g_uTestStartAddress + g_uTestFlashSize-1);
		dwTestArea = g_uTestStartAddress + g_uTestFlashSize - dwTestAddr;
		dwTestSize = dwTestArea > (2*unBlkSize) ? (2*unBlkSize) : dwTestArea;
		dwWriteSize = GetRandom(1, dwTestSize);

		ucWriteValue = GetRandom(1, 255);
		memset(pWriteBuf, ucWriteValue, 2 * unBlkSize);
		CSTCPrint("flash 写开始dwTestAddr=0x%x, dwWriteSize=0x%x, unBlkSize=0x%x\n", dwTestAddr, dwWriteSize, unBlkSize);
		if (CSUDI_SUCCESS != CSUDIFlashWrite(dwTestAddr, pWriteBuf, dwWriteSize))
		{
			CSTCPrint("第二步写flash失败\n");
			bRandRunFlag1 = FALSE;
			break;
		}
		
		CSTCPrint("flash 写完毕\n");

		CSTCPrint("flash 读开始dwTestAddr=0x%x, dwWriteSize=0x%x\n", dwTestAddr, dwWriteSize);
		if (CSUDI_SUCCESS != CSUDIFlashRead(dwTestAddr, pWriteBuf, dwWriteSize))
		{
			CSTCPrint("第三步写flash失败\n");
			bRandRunFlag1 = FALSE;
			break;
		}
		
		CSTCPrint("flash 读完毕\n");
		for (ii=0; ii<dwWriteSize; ii++)
		{
			if (pWriteBuf[ii] != ucWriteValue)
			{
				CSTCPrint("第四步校验flash失败");
				bRandRunFlag2 = FALSE;
				break;
			}
		}

		if (!bRandRunFlag2)
		{
			break;
		}
		CSTCPrint("校验数据完毕\n");
	}

	if (CSUDI_SUCCESS != CSUDIOSFree(pWriteBuf))
	{
		CSTCPrint("销毁资源失败\n");
	}

done:
	RandThread2Done = TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:多线程随机地址随机大小读写测试
//@PRECONDITION:FLASH模块之前已成功初始化
//@INPUT:flash测试的起始地址
//@INPUT:flash测试的大小
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、获取用户设置的分区信息
//@EXECUTIONFLOW:2、获取flash基本信息，如扇区大小
//@EXECUTIONFLOW:3、创建两个线程随机读写flash
//@EXECUTIONFLOW:3、随机生成flash的起始地址，写大小，写入数据
//@EXECUTIONFLOW:4、调用CSUDIFlashWrite写入数据
//@EXECUTIONFLOW:5、调用CSUDIFlashRead读取数据
//@EXECUTIONFLOW:6、校验数据
//@EXECUTIONFLOW:7、重复步骤3, 4, 5, 6    10次
//@EXECUTIONFLOW:8、销毁资源
CSUDI_BOOL  CSTC_FLASH_IT_Performance_0006(void)
{
	CSUDI_HANDLE hTestFlash1, hTestFlash2;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSThreadCreate("WriteFlash1", 17*10, 4096, (CSUDIOSThreadEntry_F)RandWriteFlashThread1, NULL, &hTestFlash1), 
	"创建线程1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("WriteFlash2", 17*10, 4096, (CSUDIOSThreadEntry_F)RandWriteFlashThread2, NULL, &hTestFlash2),
	"创建线程2失败");

	while (!RandThread1Done || !RandThread2Done)
	{
		CSUDIOSThreadSleep(1000);
	}

	CSTK_FATAL_POINT;

	if (hTestFlash1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hTestFlash1), "删除任务1失败\n");
		hTestFlash1 = CSUDI_NULL;
	}

	if (hTestFlash2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hTestFlash2), "删除任务1失败\n");
		hTestFlash2 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//通过读取配置文件查询可供测试的Flash区域
static int GetFlashTestArea(unsigned int *puTestAreaStart, unsigned int *puTestAreaSize )
{
	*puTestAreaStart = g_uTestStartAddress;
	*puTestAreaSize = g_uTestFlashSize;

	return 0;
}


void FSFlashTestTask(void * pvParam)
{
	int  eErrCode = 0;
	unsigned char *pDatabuf = (unsigned char*)pvParam;
	unsigned int dwTestAddr = 0;
	unsigned int dwTestSize = 0;
	unsigned int dwTestAreaSize = 0;
	unsigned char testcode = 0;
	int i = 0;
	
	while(s_bFlashTaskRun)
	{
		dwTestAddr = GetRandom(s_dwTestAreaStart, s_dwTestAreaStart + s_dwTestTestAreaSize-1);

		dwTestAreaSize = s_dwTestAreaStart + s_dwTestTestAreaSize - dwTestAddr;
		if(dwTestAreaSize > DATA_BUF_SIZE)
		{
			dwTestAreaSize = DATA_BUF_SIZE;
		}
		
		dwTestSize = GetRandom(1, dwTestAreaSize);
		testcode = (unsigned char )GetRandom(1, 255);

		if ((dwTestAddr + dwTestSize) > (g_uTestStartAddress + g_uTestFlashSize))
		{
			CSTCPrint("[%s] 随机生成的地址出错dwTestAddr=0x%x, dwTestSize=0x%x\n", __FUNCTION__,dwTestAddr, dwTestSize);
			s_bFlashTaskFlag = FALSE;
			goto exit;
		}

		memset(pDatabuf, testcode, dwTestSize);

		eErrCode = CSUDIFlashWrite(dwTestAddr, pDatabuf, dwTestSize);
		if(eErrCode != CSUDI_SUCCESS)
		{
			CSTCPrint("[%s] 写flash失败dwTestAddr = 0x%x, dwTestSize = 0x%x\n", __FUNCTION__,dwTestAddr, dwTestSize);
			s_bFlashTaskFlag = FALSE;
			goto exit;
		}

		memset(pDatabuf, 0, dwTestSize);
		eErrCode = CSUDIFlashRead(dwTestAddr, pDatabuf, dwTestSize);
		if(eErrCode != CSUDI_SUCCESS)
		{
			CSTCPrint("[%s] 读flash 失败!dwTestAddr = 0x%x, dwTestSize = 0x%x\n", __FUNCTION__, dwTestAddr, dwTestSize);
			s_bFlashTaskFlag = FALSE;
			goto exit;
		}

		for(i=0; i<dwTestSize; i++)
		{
			if(pDatabuf[i] != testcode)
			{
				CSTCPrint("[%s] 校验数据失败!testcode = 0x%x, readdata = 0x%x\n", __FUNCTION__, testcode, pDatabuf[i]);
				s_bFlashTaskFlag = FALSE;
				goto exit;
			}
		}

		CSTCPrint("#(0x%x)(0x%x)(0x%x)\n", dwTestAddr, testcode, dwTestSize);

		CSUDIOSThreadSleep(testcode);
	}

exit:

	return;
}

void FSLocalFsWriteReadTask(void * pvParam)
{
	CSUDI_UINT32 dwIds[32];
	int  nActIdCnt = 0; 
	int i = 0;
	unsigned long  dwVolumeId = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	int nLen = 0;
	char   szTest[] = "FSLocalFsWriteReadTask";	 
	char   szFilePath[256];
	void *hFile = NULL;
	CSUDIFSStatFs_S  sFsStat;
	BOOL bReaded ;
	BOOL bWrited ;
	unsigned char *pDataBuf = (unsigned char*)pvParam;
	int nWriteSize = 0;
	unsigned char testcode = 100;
	CSUDI_BOOL bRDI = CSUDI_FALSE;
	
	nLen = sizeof(szTest);
	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	if(CSUDI_SUCCESS != CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt))
	{
		CSTCPrint("获取设备id失败\n");
		s_bLocalFsTaskFlag = FALSE;
		goto exit;
	}

	//避免因为没有可测试设备，反而测试通过
	if(nActIdCnt < 1)
	{
		CSTCPrint("获取设备数量失败\n");
		s_bLocalFsTaskFlag = FALSE;
		goto exit;
	}
	else
	{
		for(i = 0; i < nActIdCnt; i++)
		{
			if(!CSUDI_ISREMOVEABLE(dwIds[i] ))
			{
				bRDI = CSUDI_TRUE;
			}
		}

		if(!bRDI)
		{
			CSTCPrint("没有固定设备可测试\n");
			s_bLocalFsTaskFlag = FALSE;
			goto exit;
		}
	}

	while(s_bLocalFsWriteReadTaskRun)
	{
		for(i = 0; i < nActIdCnt; i++)
		{	
			if(!CSUDI_ISREMOVEABLE(dwIds[i] ))
			{
				dwVolumeId = dwIds[i];
				bReaded = FALSE;
				bWrited = FALSE;

				memset(&sDeviceInfo, 0, sizeof(sDeviceInfo));
				memset(&szFilePath, 0, sizeof(szFilePath));
				memset(&sFsStat, 0, sizeof(sFsStat));

				if(CSUDI_SUCCESS != CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo))
				{
				 	CSTCPrint("获取分卷信息失败\n");
				 	s_bLocalFsTaskFlag = FALSE;
				 	goto exit;
				}

				if(sDeviceInfo.m_eDeviceType == EM_UDIFS_DEVTYPE_VOLUME)
				{
					strcpy(szFilePath, sDeviceInfo.m_szMountPoint);

					CSUDIFSStatfs(szFilePath, &sFsStat);
				}

				if(sFsStat.m_eFsType >= EM_UDIFS_PARTITION_FAT32 && sFsStat.m_eFsType < EM_UDIFS_PARTITION_RW_END)
				{
					strcpy(szFilePath, sDeviceInfo.m_szMountPoint);
					nLen = strlen(szFilePath);
					if(szFilePath[nLen -1] != '/')
					{
						szFilePath[nLen] = '/';
						szFilePath[nLen + 1] = '\0';
					}
					
					strcat(szFilePath, szTest); 

					nWriteSize = GetRandom(1, DATA_BUF_SIZE);
					testcode = (unsigned char)GetRandom(1, 255);
					
					memset(pDataBuf,testcode,nWriteSize);

					hFile = CSUDIFSOpen(szFilePath, "w+");
					if(NULL == hFile)
					{
						CSTCPrint("创建并打开文件失败\n");
						s_bLocalFsTaskFlag = FALSE;
				 		goto exit;
					}
					
					if (nWriteSize !=  CSUDIFSWrite(hFile, (char*)pDataBuf, nWriteSize))
					{
						CSTCPrint("写入文件失败\n");
						s_bLocalFsTaskFlag = FALSE;
						CSUDIFSClose(hFile); 
				 		goto exit;
					}
					
					CSUDIFSClose(hFile);

					memset(pDataBuf,0,nWriteSize);
					hFile = CSUDIFSOpen(szFilePath, "r");
					if (NULL == hFile)
					{
						CSTCPrint("创建并打开读文件失败\n");
						s_bLocalFsTaskFlag = FALSE;
				 		goto exit;
					}
					if (nWriteSize != CSUDIFSRead(hFile, (char*)pDataBuf, nWriteSize))
					{
						CSTCPrint("读取数据失败\n");
						s_bLocalFsTaskFlag = FALSE;
						CSUDIFSClose(hFile); 
				 		goto exit;
				 	}

					for(i=0; i<nWriteSize; i++)
					{
						if (pDataBuf[i] != testcode)
						{
							CSTCPrint("读取的数据与写入的不一致\n");
							s_bLocalFsTaskFlag = FALSE;
							CSUDIFSClose(hFile); 
							goto exit;
						}
					}
					
					CSUDIFSClose(hFile); 
					CSUDIFSRemove(szFilePath);

					CSTCPrint("@ (0x%x)(0x%x)\n", testcode, nWriteSize); 
					
					bWrited = TRUE;
				}
		
			}
		
		}

		CSUDIOSThreadSleep(testcode);
	}
	
exit:
	return ;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION: 测试对本地文件系统进行读写操作的同时调用Flash驱动同时对Flash进行读写操作
//@PRECONDITION:FS和FLASH模块之前已成功初始化
//@PRECONDITION: 系统中存在分卷设备
//@INPUT: pcPathName = 分区路径 + "LocalFsWriteRead_0001"
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、调用CSUDIFSGetAllDeviceId()得到所有已挂载的分卷设备id	
//@EXECUTIONFLOW:2、调用宏判断设备是否为可移动设备
//@EXECUTIONFLOW:3、如果不是可移动设备调用CSUDIFSGetDeviceInfo获得分卷的挂载路径
//@EXECUTIONFLOW:4、调用CSUDIFSStatfs获取分卷信息并判断文件系统类型
//@EXECUTIONFLOW:5、如果文件系统是只读类型则调用CSUDIFSOpen(szFilePath, "r"))打开当前路径文件,要求成功
//@EXECUTIONFLOW:6、调用CSUDIFSRead读取文件，要求返回成功
//@EXECUTIONFLOW:7、调用CSUDIFSClose关闭文件，要求返回成功
//@EXECUTIONFLOW:8、如果文件系统是可读写类型则调用CSUDIFSOpen(szFilePath, "w+"\r\n ))创建并打开当前路径文件,要求成功
//@EXECUTIONFLOW:9、调用CSUDIFSWrite(hFile, szTest, sizeof(szTest))向文件中写入字符串，要求返回成功
//@EXECUTIONFLOW:10、调用CSUDIFSClose(hFile)关闭文件要求返回成功
//@EXECUTIONFLOW:11、调用CSUDIFSOpen(szFilePath, "r"))打开文件要求返回成功
//@EXECUTIONFLOW:12、调用CSUDIFSRead(hFile, szBuf, sizeof(szTest))读出字符串，与写入的比较要求一致
//@EXECUTIONFLOW:13、调用CSUDIFSClose(hFile)关闭文件要求返回成功
//@EXECUTIONFLOW:14、对所有分卷设备重复步骤2~13
CSUDI_BOOL CSTC_FLASH_IT_Performance_0007(void)
{

	void *hFlashTestTask = NULL;
	void * hLocalFsWriteReadTask = NULL;
	char *pFlashDatabuf = NULL;
	char *pLocalFsDatabuf = NULL;
	unsigned int testcnt = 36000; /* 10 hours*/
	unsigned int testsleeptime = 1000; /* 1000ms*/
	unsigned int cnt = testcnt; /* 10 hours*/

	CSTCPrint("[UDI2FSTEST]:此用例需要10 小时r\n");

	GetFlashTestArea(&s_dwTestAreaStart, &s_dwTestTestAreaSize);

	CSTCPrint("Flash 读写测试起始地址:0x%x\n", s_dwTestAreaStart);
	CSTCPrint("Flash 读写测试区域大小:0x%x\n", s_dwTestTestAreaSize);
	
	pFlashDatabuf = CSUDIOSMalloc(DATA_BUF_SIZE);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pFlashDatabuf,"分配内存失败");

	pLocalFsDatabuf = CSUDIOSMalloc(DATA_BUF_SIZE);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pLocalFsDatabuf,"分配内存失败");

	s_bFlashTaskRun = TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FSFlashTestTask", 4*17, 1024*64, FSFlashTestTask, (void*)pFlashDatabuf, &hFlashTestTask),
	"创建FSFlashTestTask失败");

	s_bLocalFsWriteReadTaskRun = TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FSLocalFsWriteReadTask", 4*17, 1024*64, FSLocalFsWriteReadTask, (void*)pLocalFsDatabuf, &hFlashTestTask),
	"创建FSLocalFsWriteReadTask失败");

	while((cnt > 0) && s_bFlashTaskFlag && s_bLocalFsTaskFlag)
	{
		CSUDIOSThreadSleep(testsleeptime);
		cnt--;
	}

	if(cnt <= 0)
	{
		CSTCPrint("测试完成!!!!\n");
		CSTCPrint("测试时间为%dS,未出现错误!!!!\n", testsleeptime*testcnt/1000);
	}

	CSTK_ASSERT_TRUE_FATAL(s_bFlashTaskFlag, "Flash读写线程测试失败");
	CSTK_ASSERT_TRUE_FATAL(s_bLocalFsTaskFlag, "文件系统读写线程测试失败");

	CSTK_FATAL_POINT;

	/* 设置标记并等待线程退出 */
	s_bFlashTaskRun = FALSE;
	s_bLocalFsWriteReadTaskRun = FALSE;
	
	CSUDIOSThreadSleep(1000);
	
	if(pFlashDatabuf != NULL)
	{
		CSUDIOSFree(pFlashDatabuf);
	}

	if(pLocalFsDatabuf != NULL)
	{
		CSUDIOSFree(pLocalFsDatabuf);
	}

	if(hFlashTestTask != NULL)
	{
		s_bFlashTaskRun = FALSE;

		CSUDIOSThreadJoin(hFlashTestTask);
		CSUDIOSThreadDestroy(hFlashTestTask);
	}

	if(hLocalFsWriteReadTask != NULL)
	{
		s_bLocalFsWriteReadTaskRun = FALSE;

		CSUDIOSThreadJoin(hLocalFsWriteReadTask);
		CSUDIOSThreadDestroy(hLocalFsWriteReadTask);
	}
	
	return CSUDI_TRUE;
}

#define FLASH_FEW_BYTES_TEST_CNT (2)
//备注 用例CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0001
//         ~CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0004
//很费时,如果不带擦除写一个byte的时间为20ms(这个是cfi flash的)，那么对于
//一个4MBytes大小的flash,需要23小时多,故将测试长度改为1MBytes

//@CASEGROUP:针对一个block的单字节写强度测试
//@DESCRIPTION:向flash逐字节写入0xaa,每写一次都读出来进行校验
//@PRECONDITION:Flash初始化成功，在写入前进行整片flash擦除操作
//@EXPECTATION:读、写、擦函数返回CSHDI_SUCCESS；读出数据与写入的数据一样
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0001(void)
{
	BYTE ucWriteData = 0xaa,ucReadData = 0;
	unsigned int ii,kk=0;
	DWORD dwCurAddr = g_uTestStartAddress;
	DWORD dwLength = 1;
	DWORD dwTestSize = g_uTestBlockSize;

	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= g_uTestBlockSize, "用户可测试空间太小");
	CSTCPrint("The next part will run %d time(s).\n", FLASH_FEW_BYTES_TEST_CNT);

	do{
		kk++;
		CSTCPrint("This is the %d time(s):\n", kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwCurAddr,dwTestSize),"Erase the total flash error\n");
		
		for(ii=0;ii<dwTestSize;ii++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwCurAddr+ii, &ucWriteData, dwLength),"Flash write error\n");
			
			ucReadData = 0;
			CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwCurAddr+ii, &ucReadData, dwLength),"Flash read error\n");
			
			CSTK_ASSERT_TRUE_FATAL(ucReadData==ucWriteData,"Flash check data error\n");

			// 每隔1KB就打印一次
			if(((ii << 22) == 0))
			{
				CSTCPrint("%3d(KB)/%d(KB) test OK!\n", (ii>>10)+1, g_uTestBlockSize>>10);
			}
		}
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);

	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:针对一个block的二字节写强度测试
//@DESCRIPTION:向flash逐次写入两字节0xaa,每写一次都读出来进行校验
//@PRECONDITION:Flash初始化成功，在写入前进行整片flash擦除操作
//@EXPECTATION:读、写、擦函数返回CSHDI_SUCCESS；读出数据与写入的数据一样
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0002(void)
{
	BYTE ucWriteData[10] = {0},ucReadData[10] = {0};
	int ii,kk=0;
	DWORD dwCurAddr = 0;
	DWORD dwLength = 2;
	DWORD dwTestSize = g_uTestBlockSize;
	
	dwCurAddr = g_uTestStartAddress+g_uTestBlockSize;
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (2*g_uTestBlockSize), "用户可测试空间太小");
	CSTCPrint("The next part will run %d time(s).\n", FLASH_FEW_BYTES_TEST_CNT);

	memset(ucWriteData,0xaa,dwLength);
	do{
		kk++;
		CSTCPrint("This is the %d time(s):\n", kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwCurAddr,dwTestSize),"Erase the total flash error\n");
		
		for(ii=0;ii<dwTestSize;ii+=2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwCurAddr+ii, ucWriteData, dwLength),"Flash write error\n");
			
			memset(ucReadData,0,dwLength);
			CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwCurAddr+ii, ucReadData, dwLength),"Flash read error\n");
			
			CSTK_ASSERT_TRUE_FATAL(0 == memcmp(ucWriteData,ucReadData,dwLength),"Flash check data error\n");

			// 每隔1KB就打印一次
			if(((ii << 22) == 0))
			{
				CSTCPrint("%3d(KB)/%d(KB) test OK!\n", (ii>>10)+1, g_uTestBlockSize>>10);
			}
		}
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);

	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:针对一个block的三字节写强度测试
//@DESCRIPTION:向flash逐次写入三字节0xaa,每写一次都读出来进行校验
//@PRECONDITION:Flash初始化成功，在写入前进行整片flash擦除操作
//@EXPECTATION:读、写、擦函数返回CSHDI_SUCCESS；读出数据与写入的数据一样
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0003(void)
{
	BYTE ucWriteData[10] = {0},ucReadData[10] = {0};
	int ii,kk=0;
	DWORD dwCurAddr = 0;
	DWORD dwLength = 3;
	DWORD dwTestSize = g_uTestBlockSize;
	
	dwCurAddr = g_uTestStartAddress+2*g_uTestBlockSize;
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (3*g_uTestBlockSize), "用户可测试空间太小");
	CSTCPrint("The next part will run %d time(s).\n", FLASH_FEW_BYTES_TEST_CNT);

	memset(ucWriteData,0xaa,dwLength);
	do{
		kk++;
		CSTCPrint("This is the %d time(s):\n", kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwCurAddr,dwTestSize),"Erase the total flash error\n");
		
		for(ii=0;(ii<dwTestSize)&&((ii+3)<dwTestSize);ii+=3)
		{
			CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwCurAddr+ii, ucWriteData, dwLength),"Flash write error\n");
			
			memset(ucReadData,0,dwLength);
			CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwCurAddr+ii, ucReadData, dwLength),"Flash read error\n");

			CSTK_ASSERT_TRUE_FATAL(0 == memcmp(ucWriteData,ucReadData,dwLength),"Flash check data error\n");		

			// 每隔1KB就打印一次，主要考虑被3整除的问题
			if(((ii << 22) == 0) || ((((ii-1) << 22) == 0)) || ((((ii-2) << 22) == 0)))
			{
				CSTCPrint("%3d(KB)/%d(KB) test OK!\n", (ii>>10)+1, g_uTestBlockSize>>10);
			}
		}
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);

	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:针对一个block的四字节写强度测试
//@DESCRIPTION:向flash逐次写入四字节0xaa,每写一次都读出来进行校验
//@PRECONDITION:Flash初始化成功，在写入前进行整片flash擦除操作
//@EXPECTATION:读、写、擦函数返回CSHDI_SUCCESS；读出数据与写入的数据一样
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0004(void)
{
	BYTE ucWriteData[10] = {0},ucReadData[10] = {0};
	int ii,kk=0;
	DWORD dwCurAddr = 0;
	DWORD dwLength = 4;
	DWORD dwTestSize = g_uTestBlockSize;
	
	dwCurAddr = g_uTestStartAddress+3*g_uTestBlockSize;

	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (4*g_uTestBlockSize), "用户可测试空间太小");
	CSTCPrint("The next part will run %d time(s).\n", FLASH_FEW_BYTES_TEST_CNT);

	memset(ucWriteData,0xaa,dwLength);
	do{
		kk++;
		CSTCPrint("This is the %d time(s):\n", kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwCurAddr,dwTestSize),"Erase the total flash error\n");
		
		for(ii=0;ii<dwTestSize;ii+=4)
		{
			CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwCurAddr+ii,ucWriteData, dwLength),"Flash write error\n");
			
			memset(ucReadData,0xaa,dwLength);
			CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwCurAddr+ii, ucReadData, dwLength),"Flash read error\n");
			
			CSTK_ASSERT_TRUE_FATAL(0 == memcmp(ucWriteData,ucReadData,dwLength),"Flash check data error\n");

			// 每隔1KB就打印一次
			if(((ii << 22) == 0))
			{
				CSTCPrint("%3d(KB)/%d(KB) test OK!\n", (ii>>10)+1, g_uTestBlockSize>>10);
			}
		}
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);
	
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}
//@CASEGROUP:针对一个block的单字节读强度测试
//@DESCRIPTION:向flash写入0-255的数据,每次读一个字节进行校验操作重复多次
//@PRECONDITION:Flash初始化成功，在写入前进行整个block 的擦除操作
//@EXPECTATION:读、写、擦函数返回CSHDI_SUCCESS；读出数据与写入的数据一样
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0005(void)
{
	BYTE ucReadData= 0;
	int ii,jj,kk=0;
	DWORD dwOffAddr = 4*g_uTestBlockSize;
	DWORD dwStartAddr = 0;
	DWORD dwLength = 1;
	CSUDI_UINT8 *pucWriteData = CSUDI_NULL;
	
	dwStartAddr = g_uTestStartAddress+dwOffAddr;
	pucWriteData	= (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucWriteData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (5*g_uTestBlockSize), "用户可测试空间太小");

	for(ii=0;ii<(g_uALLMaxBlockSize/0xff);ii++)
	{
		for(jj=0;jj<0xff;jj++)
		{
			*(pucWriteData+ii*0xff+jj)=jj;
		}
	}
	
	do
	{
		kk++;
		CSTCPrint("\n Erase flash a block %d times\n",kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwStartAddr,g_uALLMaxBlockSize),"Erase the sector error\n");
		
		CSTCPrint("\n CSUDIFlashWriteWithoutErase %d times\n",kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwStartAddr,pucWriteData, g_uALLMaxBlockSize),"Flash write error\n");
		
		for(ii=0;ii<(g_uALLMaxBlockSize/0xff);ii++)
		{
			for(jj=0;jj<0xff;jj++)
			{
				CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwStartAddr+ii*0xff+jj, &ucReadData, dwLength),"Flash read error\n");
				
				CSTK_ASSERT_TRUE_FATAL(ucReadData==jj,"Flash checkdata error\n");
				
			}
		}		
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);
	
	CSTK_FATAL_POINT;
	
	if (pucWriteData != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucWriteData),"销毁资源失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:针对一个block的二字节读强度测试
//@DESCRIPTION:向flash写入0-255的数据,每次读二个字节进行校验操作重复多次
//@PRECONDITION:Flash初始化成功，在写入前进行整个block 的擦除操作
//@EXPECTATION:读、写、擦函数返回CSHDI_SUCCESS；读出数据与写入的数据一样
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0006(void)
{
	BYTE ucReadData[2] = {0};
	int ii,jj,kk=0;
	DWORD dwOffAddr = 5*g_uTestBlockSize;
	DWORD dwStartAddr = 0;
	DWORD dwLength = 2;
	CSUDI_UINT8 *pucWriteData = CSUDI_NULL;

	dwStartAddr = g_uTestStartAddress+dwOffAddr;
	pucWriteData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucWriteData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (6*g_uTestBlockSize), "用户可测试空间太小");

	for(ii=0;ii<(g_uALLMaxBlockSize/0xff);ii++)
	{
		for(jj=0;jj<0xff;jj++)
		{
			*(pucWriteData+ii*0xff+jj)=jj;
		}
	}
	
	do
	{
		kk++;
		CSTCPrint("\n Erase flash a block %d times\n",kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwStartAddr,g_uALLMaxBlockSize),"Erase the sector error\n");
		
		CSTCPrint("\n CSUDIFlashWriteWithoutErase %d times\n",kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwStartAddr,pucWriteData, g_uALLMaxBlockSize),"Flash write error\n");
		
		for(ii=0;ii<(g_uALLMaxBlockSize/0xff);ii++)
		{
			for(jj=0;jj<0xff;jj+=2)
			{
				memset(ucReadData,0,dwLength);
				CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwStartAddr+ii*0xff+jj, ucReadData, dwLength),"Flash read error\n");
				
				CSTK_ASSERT_TRUE_FATAL(0 == memcmp(ucReadData,pucWriteData+(ii*0xff+jj),dwLength),"Flash checkdata error\n");
				
			}
		}		
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);
	
	CSTK_FATAL_POINT;
	
	if (pucWriteData != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucWriteData),"销毁资源失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:针对一个block的三字节读强度测试
//@DESCRIPTION:向flash写入0-255的数据,每次读三个字节进行校验操作重复多次
//@PRECONDITION:Flash初始化成功，在写入前进行整个block 的擦除操作
//@EXPECTATION:读、写、擦函数返回CSHDI_SUCCESS；读出数据与写入的数据一样
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0007(void)
{
	BYTE ucReadData[3] = {0};
	int ii,jj,kk=0;
	DWORD dwOffAddr = 6*g_uTestBlockSize;
	DWORD dwStartAddr = 0;
	DWORD dwLength = 3;
	CSUDI_UINT8 *pucWriteData = CSUDI_NULL;
	
	dwStartAddr = g_uTestStartAddress+dwOffAddr;
	pucWriteData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucWriteData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (7*g_uTestBlockSize), "用户可测试空间太小");

	for(ii=0;ii<(g_uALLMaxBlockSize/0xff);ii++)
	{
		for(jj=0;jj<0xff;jj++)
		{
			*(pucWriteData+ii*0xff+jj)=jj;
		}
	}
	
	do
	{
		kk++;
		CSTCPrint("\n Erase flash a block %d times\n",kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwStartAddr,g_uALLMaxBlockSize),"Erase the sector error\n");
		
		CSTCPrint("\n CSUDIFlashWriteWithoutErase %d times\n",kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwStartAddr,pucWriteData, g_uALLMaxBlockSize),"Flash write error\n");
		
		for(ii=0;ii<(g_uALLMaxBlockSize/0xff);ii++)
		{
			for(jj=0;jj<0xff;jj+=3)
			{
				memset(ucReadData,0,dwLength);
				CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwStartAddr+ii*0xff+jj, ucReadData, dwLength),"Flash read error\n");
				
				CSTK_ASSERT_TRUE_FATAL(0 == memcmp(ucReadData,pucWriteData+(ii*0xff+jj),dwLength),"Flash checkdata error\n");
				
			}
		}		
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);
	
	CSTK_FATAL_POINT;
	
	if (pucWriteData != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucWriteData),"销毁资源失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:针对一个block的四字节读强度测试
//@DESCRIPTION:向flash写入0-255的数据,每次读四个字节进行校验操作重复多次
//@PRECONDITION:Flash初始化成功，在写入前进行整个block 的擦除操作
//@EXPECTATION:读、写、擦函数返回CSHDI_SUCCESS；读出数据与写入的数据一样
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0008(void)
{
	BYTE ucReadData[4] = {0};
	int ii,jj,kk=0;
	DWORD dwOffAddr = 7*g_uTestBlockSize;
	DWORD dwStartAddr = 0;
	DWORD dwLength = 4;
	CSUDI_UINT8 *pucWriteData = CSUDI_NULL;
	
	dwStartAddr = g_uTestStartAddress+dwOffAddr;
	pucWriteData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uALLMaxBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucWriteData,"分配内存失败");
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (8*g_uTestBlockSize), "用户可测试空间太小");

	for(ii=0;ii<(g_uALLMaxBlockSize/0xff);ii++)
	{
		for(jj=0;jj<0xff;jj++)
		{
			*(pucWriteData+ii*0xff+jj)=jj;
		}
	}
	
	do
	{
		kk++;
		CSTCPrint("\n Erase flash a block %d times\n",kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwStartAddr,g_uALLMaxBlockSize),"Erase the sector error\n");
		
		CSTCPrint("\n CSUDIFlashWriteWithoutErase %d times\n",kk);
		CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwStartAddr, pucWriteData, g_uALLMaxBlockSize),"Flash write error\n");
		
		for(ii=0;ii<(g_uALLMaxBlockSize/0xff);ii++)
		{
			for(jj=0;jj<0xff;jj+=4)
			{
				memset(ucReadData,0,dwLength);
				CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwStartAddr+ii*0xff+jj, ucReadData, dwLength),"Flash read error\n");
				CSTK_ASSERT_TRUE_FATAL(0 == memcmp(ucReadData,pucWriteData+(ii*0xff+jj),dwLength),"Flash checkdata error\n");
			}
		}		
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);
	
	CSTK_FATAL_POINT;
	
	if (pucWriteData != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucWriteData),"销毁资源失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:针对两个block进行跨block读写测试
//@DESCRIPTION:向第一个block的最后两个字节和相邻第二个block的前两个字节写入数据，判断读写数据是否一致
//@PRECONDITION:Flash初始化成功，在写入前进行整个block 的擦除操作，测试空间至少为两个block
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、调用CSUDIFlashEraseSpace()擦除两个block空间数据，期望擦除返回成功
//@EXECUTIONFLOW:2、调用CSUDIFlashWriteWithoutErase函数向相邻两个block中第一个block最后两个字节和第二个block的前两个字节写入数据
//@EXECUTIONFLOW:3、调用CSUDIFlashRead读出步骤2中写入的四个字节的数据，要求读取数据成功
//@EXECUTIONFLOW:4、将步骤3读出的四个字节数据与步骤2中写入的四个字节数据值进行对比，期望读出的值和写入的一致
CSUDI_BOOL CSTC_FLASH_IT_FlashNeighborBlockWriteNoErase(void)
{
	BYTE ucWriteData[4],ucReadData[4];
	int ii=0;
	DWORD dwStartAddr = g_uTestStartAddress;
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (2*g_uTestBlockSize), "用户可测试空间太小");

	for(ii=0;ii<4;ii++)
	{
		ucWriteData[ii]=ii;
	}
		
	CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashEraseSpace(dwStartAddr, 2*g_uALLMaxBlockSize),"Erase the sector error\n");
	CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashWriteWithoutErase(dwStartAddr+g_uALLMaxBlockSize-2, ucWriteData, 4), "Flash write error\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSHDI_SUCCESS == CSUDIFlashRead(dwStartAddr+g_uALLMaxBlockSize-2, ucReadData, 4), "Flash read error\n");;	
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(ucReadData, ucWriteData, 4), "Flash checkdata error\n");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

