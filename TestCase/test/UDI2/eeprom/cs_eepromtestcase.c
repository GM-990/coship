/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_eepromtestcase.h"

#include "udi2_os.h"

//强度测试时重复执行的次数
#define g_nCount 1000

//eeprom的起始地址 
extern CSUDI_UINT32 g_dwStartAddress;

//eeprom空间大小 
extern int g_nEepromSize;
//多线程用到的全局变量
static CSUDI_BOOL g_bThreadEnd1;

static CSUDI_BOOL g_bThreadEnd2;

static CSUDI_BOOL g_bThreadEnd3;

static CSUDI_BOOL g_bThreadEnd4;

static CSUDI_BOOL g_bThreadEnd5;

static CSUDI_BOOL g_bThreadEnd6;

static CSUDI_BOOL g_bThreadEnd7;

static CSUDI_BOOL g_bFlag1;

static CSUDI_BOOL g_bFlag2;

static CSUDI_BOOL g_bFlag3;

#define PANEL_TEST_STACK_SIZE 8*1024

CSUDI_BOOL CSTC_EEPROM_Init(void)
{
	//在本测试用例集执行前调用
	CSTC_EEPROM_InitCfg();
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_EEPROM_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//任务1向EEPROM中写入1024个数据0x55
void EEPROM_TestThread1Entry(void * pvParam)
{
	CSUDI_UINT8 pucData[1024];

	memset(pucData, 0x55, 1024);

	CSUDIEPRWrite(g_dwStartAddress, pucData, 1024);
	
	g_bThreadEnd1 = CSUDI_TRUE;
	
}
//任务2向EEPROM中写入1024个数据0x88
void EEPROM_TestThread2Entry(void *pvParam)
{
	CSUDI_UINT8 pucData[1024];

	memset(pucData, 0x88, 1024);

	CSUDIEPRWrite(g_dwStartAddress, pucData, 1024);
	
	g_bThreadEnd2 = CSUDI_TRUE;

}

//任务3向EEPROM中写入1024个数据0xee
void EEPROM_TestThread3Entry(void *pvParam)
{
	
	CSUDI_UINT8 pucData[1024];

	memset(pucData, 0xee, 1024);

	CSUDIEPRWrite(g_dwStartAddress, pucData, 1024);
	
	g_bThreadEnd3 = CSUDI_TRUE;

}

//任务4从EEPROM的起始地址起读取1024个数据且期望全为0x55
void EEPROM_TestThread4Entry(void * pvParam)
{
	int i;
	
	CSUDI_UINT8 pucData[1024];

	memset(pucData, '\0', 1024);

	CSUDIEPRRead(g_dwStartAddress, pucData, 1024);
	for(i = 0; i < 1024; ++i)
	{
		if(pucData[i] != 0x55)
		{
			g_bFlag1 = CSUDI_FALSE;
			break;
		}	
	}
	
	g_bThreadEnd4 = CSUDI_TRUE;
	
}
//任务5从EEPROM的起始地址+512起读取1024个数据且期望前512个为0x55，后512个为0xee
void EEPROM_TestThread5Entry(void *pvParam)
{
	int i;
	CSUDI_UINT8 pucData[1024];

	memset(pucData, '\0', 1024);

	CSUDIEPRRead(g_dwStartAddress + 512, pucData, 1024);

	for(i = 0; i < 512; ++i)
	{
		if(pucData[i] != 0x55)
		{
			g_bFlag2 = CSUDI_FALSE;
			break;
		}	
	}
	
	for(; i < 1024; ++i)
	{
		if(pucData[i] != 0xee)
		{
			g_bFlag2 = CSUDI_FALSE;
			break;
		}	
	}
	
	g_bThreadEnd5 = CSUDI_TRUE;

}

//任务6从EEPROM的起始地址+1024起读取1024个数据且期望全为0xee
void EEPROM_TestThread6Entry(void *pvParam)
{
	int i;
	
	CSUDI_UINT8 pucData[1024];

	memset(pucData, '\0', 1024);

	CSUDIEPRRead(g_dwStartAddress + 1024, pucData, 1024);
	
	for(i = 0; i < 1024; ++i)
	{
		if(pucData[i] != 0xee)
		{
			g_bFlag3 = CSUDI_FALSE;
			break;
		}	
	}
	
	g_bThreadEnd6 = CSUDI_TRUE;

}

//任务7向EEPROM中写入1024个数据0xee
void EEPROM_TestThread7Entry(void *pvParam)
{
	
	CSUDI_UINT8 pucData[1024];

	memset(pucData, 0xee, 1024);

	CSUDIEPRWrite(g_dwStartAddress+1024, pucData, 1024);
	
	g_bThreadEnd7 = CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRGetInfo  
//@DESCRIPTION:测试参数psEPRInfo = CSUDI_NULL时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:psEPRInfo = CSUDI_NULL
//@EXPECTATION:CSUDIEEPROM_ERROR_BAD_PARAMETER
//@REMARK:
//@EXECUTIONFLOW:1、调用函数CSUDIEPRGetInfo期望返回CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRGetInfo_0001(void)
{
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIEEPROM_ERROR_BAD_PARAMETER == CSUDIEPRGetInfo(CSUDI_NULL),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRGetInfo  
//@DESCRIPTION:测试能否正确得到芯片信息时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:psEPRInfo值合法
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、调用函数CSUDIEPRGetInfo期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、期望得到的EEPROM芯片信息与配置文件里的相同
CSUDI_BOOL  CSTC_EEPROM_IT_EPRGetInfo_0002(void)
{
	CSUDIEPRInfo_S s_EprInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRGetInfo(&s_EprInfo),"第一步测试失败");
	
	CSTK_ASSERT_TRUE_FATAL(g_dwStartAddress == s_EprInfo.m_dwStartAddress,"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(g_nEepromSize == s_EprInfo.m_nSize,"第二步测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:测试参数dwStartAddress非法的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress 值不合法
//@INPUT:pucData 值合法
//@INPUT:uDataLength = 16
//@EXPECTATION:返回CSUDI_FAILURE
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite(g_dwStartAddress + g_nEepromSize, pucData, 16)期望返回CSUDI_FAILURE
//@EXECUTIONFLOW:2、调用CSUDIEPRWrite(g_dwStartAddress - 0x0001, pucData, 16)期望返回CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0001(void)
{

	CSUDI_UINT8 pucData[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress + g_nEepromSize, pucData, 16),"测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress - 0x0001, pucData, 16),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:测试参数pucData = CSUDI_NULL时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData = CSUDI_NULL 
//@INPUT:uDataLength = 1024
//@EXPECTATION:返回CSUDI_FAILURE
//@REMARK:g_dwStartAddress为EEPROM的起始地址
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite(g_dwStartAddress, CSUDI_NULL, 1024)期望返回CSUDIEEPROM_ERROR_BAD_PARAMETER
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0002(void)
{
		
	CSTK_ASSERT_TRUE_FATAL(CSUDIEEPROM_ERROR_BAD_PARAMETER == CSUDIEPRWrite(g_dwStartAddress, CSUDI_NULL, 1024),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:测试参数uDataLength值不合法的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength值不合法
//@EXPECTATION:返回CSUDI_FAILURE
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize + 1)期望返回CSUDI_FAILURE
//@EXECUTIONFLOW:2、调用CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize + 1024)期望返回CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0003(void)
{


	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize + 1);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "分配资源失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize + 1),"第一步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize + 1024),"第二步测试失败");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"销毁资源失败"); 
	
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:测试要写入的数据dwStartAddress +  uDataLength大于EEPROM空间时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress + 1024
//@INPUT:pucData 非空 
//@INPUT:uDataLength = g_nEepromSize - 1023
//@EXPECTATION:返回CSUDI_FAILURE
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite(g_dwStartAddress + 1024, pucData, g_nEepromSize - 1023)期望返回CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0004(void)
{
	CSUDI_UINT8 *pucData;
	
	printf("\nliubing test g_nEepromSize:%d+++++++++++++\n",g_nEepromSize);

	pucData= (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize - 1023);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "分配资源失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress + 1024, pucData, g_nEepromSize - 1023),"测试失败");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"销毁资源失败");
	
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:测试uDataLength = 0时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength = 0
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_dwStartAddress为EEPROM的起始地址
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite(g_dwStartAddress, pucData, 0)期望返回CSUDI_SUCCESS
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0005(void)
{

	CSUDI_UINT8 pucData[1024];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, 0),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:测试参数dwStartAddress非法的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress 值不合法
//@INPUT:pucData 非空 
//@INPUT:uDataLength = 16
//@EXPECTATION:返回CSUDI_FAILURE
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRRead(g_dwStartAddress + g_nEepromSize,pucData, 16)期望返回CSUDI_FAILURE
//@EXECUTIONFLOW:2、调用CSUDIEPRRead(g_dwStartAddress - 0x0001, pucData, 16)期望返回CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0001(void)
{

	CSUDI_UINT8 pucData[16];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRRead(g_dwStartAddress + g_nEepromSize,pucData, 16),"测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRRead(g_dwStartAddress - 0x0001,pucData, 16),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:测试参数pucData = CSUDI_NULL 时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData = CSUDI_NULL 
//@INPUT:uDataLength = 1024
//@EXPECTATION:返回CSUDIEEPROM_ERROR_BAD_PARAMETER
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRRead(g_dwStartAddress, CSUDI_NULL, 1024)期望返回CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0002(void)
{
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIEEPROM_ERROR_BAD_PARAMETER == CSUDIEPRRead(g_dwStartAddress, CSUDI_NULL, 1024),"测试失败");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:测试参数uDataLength值不合法的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength值不合法
//@EXPECTATION:返回CSUDI_FAILURE
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRRead(g_dwStartAddress, pucData,  g_nEepromSize + 1)期望返回CSUDI_FAILURE
//@EXECUTIONFLOW:2、调用CSUDIEPRRead(g_dwStartAddress, pucData,  g_nEepromSize + 1024)期望返回CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0003(void)
{

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize + 1);		
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "分配资源失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRRead(g_dwStartAddress, pucData,  g_nEepromSize + 1),"测试失败");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:测试要读取的数据dwStartAddress +  uDataLength大于EEPROM空间时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress + 1024
//@INPUT:pucData 非空 
//@INPUT:uDataLength = g_nEepromSize - 1023
//@EXPECTATION:返回CSUDI_FAILURE
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRRead(g_dwStartAddress + 1024, pucData, g_nEepromSize - 1023)期望返回CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0004(void)
{
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize - 1023);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "分配资源失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRRead(g_dwStartAddress + 1024, pucData, g_nEepromSize - 1023),"测试失败");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"销毁资源失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:测试uDataLength = 0时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength = 0
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:g_dwStartAddress为EEPROM的起始地址
//@EXECUTIONFLOW:1、调用CSUDIEPRRead(g_dwStartAddress, pucData, 0)期望返回CSUDI_SUCCESS
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0005(void)
{
	CSUDI_UINT8 pucData[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, 0),"测试失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试读写整个EEPROM空间时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength = g_nEepromSize
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用函数CSUDIEPRWrite向EEPROM中写入pucData指向的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用函数CSUDIEPRRead读取EEPROM中的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、期望读取的数据与写入的数据相同
//@EXECUTIONFLOW:4、调用函数CSUDIEPRWrite向EEPROM中写入与上组不同的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用函数CSUDIEPRRead读取EEPROM中的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望读取的数据与写入的数据相同
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0001(void)
{
	int i;
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "分配资源失败");
	
	memset(pucData,'a',g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize),"第一步测试失败");

	memset(pucData,'\0',g_nEepromSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, g_nEepromSize),"第二步测试失败");

	for(i=0 ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"第三步测试失败");
	}
	
	memset(pucData,0x55,g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize),"第四步测试失败");

	memset(pucData,'\0',g_nEepromSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, g_nEepromSize),"第五步测试失败");

	for(i=0 ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucData[i],"第六步测试失败");
	}
	
	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"释放内存失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试奇地址读写奇数个字符时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress分别取g_dwStartAddress + 0x1001、g_dwStartAddress + 0x0003
//@INPUT:pucData 非空 
//@INPUT:uDataLength =  1023
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、dwStartAddress = g_dwStartAddress + 0x1001时调用函数CSUDIEPRWrite向EEPROM中写入1023个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用函数CSUDIEPRRead从g_dwStartAddress + 0x1001地址起读取1023个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读取的数据与第二步写入的数据相同
//@EXECUTIONFLOW:5、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff
//@EXECUTIONFLOW:6、dwStartAddress = g_dwStartAddress + 0x0003时调用函数CSUDIEPRWrite向EEPROM中写入1023个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用函数CSUDIEPRRead 从g_dwStartAddress + 0x0003地址起读取1023个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读取的数据与第六步写入的数据相同
//@EXECUTIONFLOW:9、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff
//@EXECUTIONFLOW:10、dwStartAddress = g_dwStartAddress + 0x0003时调用函数CSUDIEPRWrite向EEPROM中写入1023个数据0x55期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用函数CSUDIEPRRead读取EEPROM中的所有数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、期望读取的数据从0x0003开始有1023个字节为0x55，其余的数据全为0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0002(void)
{
	int i;

	CSUDI_UINT8 pucData[1023];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");

	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");

	memset(pucData, 'a', sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x1001, pucData, 1023),"第二步测试失败");

	memset(pucData, '\0', sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x1001, pucData, 1023),"第三步测试失败");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"第四步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第五步测试失败");
	
	memset(pucData,'8',sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0003, pucData, 1023),"第六步测试失败");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x0003, pucData, 1023),"第七步测试失败");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('8' == pucData[i],"第八步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第九步测试失败");
	
	memset(pucData, 0x55, sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0003, pucData, 1023),"第十步测试失败");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第十一步测试失败");

	for(i=0 ;i < 0x0003; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}
	for(  ;i < 0x0003 + 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第十二步测试失败");
	}	
	for(  ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}
		
	CSTK_FATAL_POINT
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试奇地址读写偶数个字符时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress分别取g_dwStartAddress + 0x1001、g_dwStartAddress + 0x0003
//@INPUT:pucData 非空 
//@INPUT:uDataLength =  1024
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、dwStartAddress = g_dwStartAddress + 0x1001时调用函数CSUDIEPRWrite向EEPROM中写入1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用函数CSUDIEPRRead从g_dwStartAddress + 0x1001地址起读取1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读取的数据与第二步写入的数据相同
//@EXECUTIONFLOW:5、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff
//@EXECUTIONFLOW:6、dwStartAddress = g_dwStartAddress + 0x0003时调用函数CSUDIEPRWrite向EEPROM中写入1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用函数CSUDIEPRRead 从g_dwStartAddress + 0x0003地址起读取1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读取的数据与第六步写入的数据相同
//@EXECUTIONFLOW:9、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff
//@EXECUTIONFLOW:10、dwStartAddress = g_dwStartAddress + 0x0003时调用函数CSUDIEPRWrite向EEPROM中写入1024个数据0x55期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用函数CSUDIEPRRead读取EEPROM中的所有数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、期望读取的数据从0x0003开始有1024个字节为0x55，其余的数据全为0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0003(void)
{
	int i;

	CSUDI_UINT8 pucData[1024];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");

	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");

	memset(pucData, 'a', sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x1001, pucData, 1024),"第二步测试失败");

	memset(pucData, '\0', sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x1001, pucData, 1024),"第三步测试失败");

	for(i=0 ;i < 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"第四步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第五步测试失败");
	
	memset(pucData,'8',sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0003, pucData, 1024),"第六步测试失败");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x0003, pucData, 1024),"第七步测试失败");

	for(i=0 ;i < 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('8' == pucData[i],"第八步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第九步测试失败");
	
	memset(pucData, 0x55, sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0003, pucData, 1024),"第十步测试失败");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第十一步测试失败");

	for(i=0 ;i < 0x0003; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}
	for(  ;i < 0x0003 + 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第十二步测试失败");
	}	
	for(  ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
		
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试偶地址读写奇数个字符时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress分别取g_dwStartAddress、g_dwStartAddress + 0x0026
//@INPUT:pucData 非空 
//@INPUT:uDataLength =  1023
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、dwStartAddress = g_dwStartAddress时调用函数CSUDIEPRWrite向EEPROM中写入1023个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用函数CSUDIEPRRead从g_dwStartAddress地址起读取1023个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读取的数据与第二步写入的数据相同
//@EXECUTIONFLOW:5、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff
//@EXECUTIONFLOW:6、dwStartAddress = g_dwStartAddress + 0x0026时调用函数CSUDIEPRWrite向EEPROM中写入1023个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用函数CSUDIEPRRead 从g_dwStartAddress + 0x0026地址起读取1023个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读取的数据与第六步写入的数据相同
//@EXECUTIONFLOW:9、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff
//@EXECUTIONFLOW:10、dwStartAddress = g_dwStartAddress + 0x0026时调用函数CSUDIEPRWrite向EEPROM中写入1023个数据0x55期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用函数CSUDIEPRRead读取EEPROM中的所有数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、期望读取的数据从0x0003开始有1023个字节为0x55，其余的数据全为0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0004(void)
{
	int i;

	CSUDI_UINT8 pucData[1023];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");

	memset(pucData, 'a', sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, 1023),"第二步测试失败");

	memset(pucData, '\0', sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, 1023),"第三步测试失败");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"第四步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第五步测试失败");
	
	memset(pucData,'8',sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0026, pucData, 1023),"第六步测试失败");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x0026, pucData, 1023),"第七步测试失败");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('8' == pucData[i],"第八步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第九步测试失败");
	
	memset(pucData, 0x55, sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0026, pucData, 1023),"第十步测试失败");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第十一步测试失败");

	for(i=0 ;i < 0x0026; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}
	for(  ;i < 0x0026 + 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第十二步测试失败");
	}	
	for(  ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
		
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试偶地址读写偶数个字符时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress分别取g_dwStartAddress、g_dwStartAddress + 0x0026
//@INPUT:pucData 非空 
//@INPUT:uDataLength =  1024
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、dwStartAddress = g_dwStartAddress 时调用函数CSUDIEPRWrite向EEPROM中写入1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用函数CSUDIEPRRead从g_dwStartAddress地址起读取1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读取的数据与第二步写入的数据相同
//@EXECUTIONFLOW:5、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff
//@EXECUTIONFLOW:6、dwStartAddress = g_dwStartAddress + 0x0026时调用函数CSUDIEPRWrite向EEPROM中写入1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用函数CSUDIEPRRead 从g_dwStartAddress + 0x0026地址起读取1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读取的数据与第六步写入的数据相同
//@EXECUTIONFLOW:9、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff
//@EXECUTIONFLOW:10、dwStartAddress = g_dwStartAddress + 0x0026时调用函数CSUDIEPRWrite向EEPROM中写入1024个数据0x55期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用函数CSUDIEPRRead读取EEPROM中的所有数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、期望读取的数据从0x0026开始有1024个字节为0x55，其余的数据全为0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0005(void)
{
	int i;

	CSUDI_UINT8 pucData[1024];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");

	memset(pucData, 'a', sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, 1024),"第二步测试失败");

	memset(pucData, '\0', sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, 1024),"第三步测试失败");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"第四步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第五步测试失败");
	
	memset(pucData,'8',sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0026, pucData, 1024),"第六步测试失败");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x0026, pucData, 1024),"第七步测试失败");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('8' == pucData[i],"第八步测试失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第九步测试失败");
	
	memset(pucData, 0x55, sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0026, pucData, 1024),"第十步测试失败");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第十一步测试失败");

	for(i=0 ;i < 0x0026; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}
	for(  ;i < 0x0026 + 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"第十二步测试失败");
	}	
	for(  ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
		
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试连续多次读写EEPROM时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength = 1024
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_RepeatTime为重复执行的次数
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用函数CSUDIEPRWrite向EEPROM中写入1024个pucData指向的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用函数CSUDIEPRRead读取1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读取的数据与第二步写入的内容相同
//@EXECUTIONFLOW:5、重复2至4步g_RepeatTime次
//@EXECUTIONFLOW:6、调用函数CSUDIEPRRead读取整个EEPROM中的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读取的数据前1024个与pucData所指的内容相同，其余的全部为0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0006(void)
{
	int i;
	int j;
	int nTestStartVal = 0x11;
	int nTestCount = 256 - 0x11;//CSUDI_UINT8最大存255

	CSUDI_UINT8 pucData[1024];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");

	for(i=0; i < nTestCount; ++i)
	{
		memset(pucData, nTestStartVal+i, sizeof(pucData));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, 1024),"第二步测试失败");

		memset(pucData, '\0', sizeof(pucData));
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, 1024),"第三步测试失败");

		for(j=0 ;j < 1024;  ++j)
		{
			CSTK_ASSERT_TRUE_FATAL(nTestStartVal+i == pucData[j],"第四步测试失败");
		}
	}
	
	memset(pucBuf, '\0', g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第六步测试失败");

	for(j=0; j < 1024;  ++j)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[j] == pucData[j],"第七步测试失败");
	}

	for(j=1024; j < g_nEepromSize;  ++j)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[j],"第七步测试失败");
	}

 	CSTK_FATAL_POINT
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
	
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试连续多次写一次性读EEPROM时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength 值合法
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用函数CSUDIEPRWrite向EEPROM中dwStartAddress地址起写入256个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用函数CSUDIEPRWrite向EEPROM中dwStartAddress + 256地址起写入256个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用函数CSUDIEPRWrite向EEPROM中dwStartAddress + 512地址起写入256个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用函数CSUDIEPRWrite向EEPROM中dwStartAddress + 768地址起写入256个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用函数CSUDIEPRRead从EEPROM中dwStartAddress地址起读出1024个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读取的数据与写入的内容相同
//@EXECUTIONFLOW:8、调用函数CSUDIEPRRead从EEPROM中dwStartAddress地址起读出EEPROM的全部数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、期望读取的数据开始的1024个与写入的内容相同，其余的数据全为0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0007(void)
{
	int i;

	CSUDI_UINT32  dwWriteAddress;

	CSUDI_UINT8 pucDataW[256];

	CSUDI_UINT8 pucDataR[1024];

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");

	for(i = 0; i < 256; ++i)
	{
		pucDataW[i] = i ;
	}
	
	dwWriteAddress = g_dwStartAddress;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwWriteAddress, pucDataW, 256),"第二步测试失败");

	dwWriteAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwWriteAddress, pucDataW, 256),"第三步测试失败");

	dwWriteAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwWriteAddress, pucDataW, 256),"第四步测试失败");

	dwWriteAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwWriteAddress, pucDataW, 256),"第五步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 1024),"第六步测试失败");

	for(i = 0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(( i%256)  == pucDataR[i],"第七步测试失败");
	}

	memset(pucBuf, '\0', g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第八步测试失败");

	for(i = 0; i< 1024;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[i] == pucDataR[i],"第九步测试失败");
	}

	for(i = 1024; i < g_nEepromSize;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第九步测试失败");
	}

 	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
		
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试一次写多次读EEPROM时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength 值合法
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用函数CSUDIEPRWrite向EEPROM中dwStartAddress地址起写入1024数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用函数CSUDIEPRRead每次从EEPROM中dwStartAddress地址起读取256个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读到的数据与写入的数据是相同的
//@EXECUTIONFLOW:5、调用函数CSUDIEPRRead从EEPROM中dwStartAddress + 256地址起读取256个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望读到的数据与写入的数据是相同的
//@EXECUTIONFLOW:7、调用函数CSUDIEPRRead从EEPROM中dwStartAddress + 512地址起读取256个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望读到的数据与写入的数据是相同的
//@EXECUTIONFLOW:9、调用函数CSUDIEPRRead从EEPROM中dwStartAddress + 768地址起读取256个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、期望读到的数据与写入的数据是相同的
//@EXECUTIONFLOW:11、调用函数CSUDIEPRRead从EEPROM中读取全部数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、期望读到的数据前1024个与写入的内容相同，其余的全部为0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0008(void)
{
	int i;
	
	CSUDI_UINT32 dwReadAddress;

	CSUDI_UINT8 pucDataW[1024];

	CSUDI_UINT8 pucDataR[256];

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");

	for(i = 0; i < 1024; ++i)
	{
		pucDataW[i] = i%256;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 1024),"第二步测试失败");

	dwReadAddress = g_dwStartAddress;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwReadAddress, pucDataR, 256),"第三步测试失败");

	for(i = 0; i < 256; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL( i == pucDataR[i],"第四步测试失败");
	}
	
	dwReadAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwReadAddress, pucDataR, 256),"第五步测试失败");

	for(i = 0; i < 256; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL( i == pucDataR[i],"第六步测试失败");
	}
	
	dwReadAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwReadAddress, pucDataR, 256),"第七步测试失败");
	
	for(i = 0; i < 256; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL( i == pucDataR[i],"第八步测试失败");
	}
	
	dwReadAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwReadAddress, pucDataR, 256),"第九步测试失败");

	for(i = 0; i < 256; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL( i == pucDataR[i],"第十步测试失败");
	}

	memset(pucBuf, '\0', g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第十一步测试失败");

	for(i = 0; i< 1024;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[i] == pucDataW[i],"第十二步测试失败");
	}

	for(i = 1024; i < g_nEepromSize;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第十二步测试失败");
	}

 	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
		
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试交叉读写EEPROM时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength 值合法
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@EXECUTIONFLOW:1、调用CSUDIEPRWrite把整个EEPROM空间全部写为0xff，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用函数CSUDIEPRWrite向EEPROM中写入32个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用函数CSUDIEPRRead从EEPROM中读取32个数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、期望读到的数据与写入的相同
//@EXECUTIONFLOW:5、读写的地址加32后重复2至4步，重复4次，期望每次都达到预期结果
//@EXECUTIONFLOW:6、调用函数CSUDIEPRRead从EEPROM中读取整个EEPROM的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、期望读到的数据前128个字节为以上写入的数据，其余的全部为0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0009(void)
{
	int i;

	int j = 0;
	
	CSUDI_UINT32 dwAddress;

	CSUDI_UINT8 pucData[32];

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");

	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");

	dwAddress = g_dwStartAddress;

	for(i = 0; i < 32; ++i)
	{
		pucData[i] = i ;
	}

	while(j < 4)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwAddress, pucData, 32),"第二步测试失败");

		memset(pucData, '\0', sizeof(pucData));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwAddress, pucData, 32),"第三步测试失败");

		for(i = 0; i < 32; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL( i == pucData[i],"第四步测试失败");
		}

		dwAddress += 32;
		
		++j;
	}
	
	memset(pucBuf, '\0', g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第六步测试失败");

	for(i = 0; i< 128;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i%32 == pucBuf[i],"第七步测试失败");
	}

	for(i = 128; i < g_nEepromSize;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第七步测试失败");
	}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
		
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:读写g_nEepromSize、8k、4k、2k、32byte时的速度测试
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData 非空 
//@INPUT:uDataLength 值合法
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:g_dwStartAddress为EEPROM的起始地址，g_nEepromSize为EEPROM的大小
//@REMARK:进行这些操作的时间标准还需要进一步确定
//@EXECUTIONFLOW:1、调用函数CSUDIEPRWrite向EEPROM中写入g_nEepromSize个字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、得到完成写入8k数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:3、调用函数CSUDIEPRRead从EEPROM中读取g_nEepromSize个字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、得到完成读取8k数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:5、期望读到的数据与写入的数据相同
//@EXECUTIONFLOW:6、调用函数CSUDIEPRWrite向EEPROM中写入8k字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、得到完成写入8k数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:8、调用函数CSUDIEPRRead从EEPROM中读取8k字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、得到完成读取8k数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:10、期望读到的数据与写入的数据相同
//@EXECUTIONFLOW:11、调用函数CSUDIEPRWrite向EEPROM中写入4k字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、得到完成写入4k数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:13、调用函数CSUDIEPRRead从EEPROM中读取4k字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、得到完成读取4k数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:15、期望读到的数据与写入的数据相同
//@EXECUTIONFLOW:16、调用函数CSUDIEPRWrite向EEPROM中写入2k字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、得到完成写入2k数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:18、调用函数CSUDIEPRRead从EEPROM中读取2k字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、得到完成读取2k数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:20、期望读到的数据与写入的数据相同
//@EXECUTIONFLOW:21、调用函数CSUDIEPRWrite向EEPROM中写入32byte字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:22、得到完成写入32byte数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:23、调用函数CSUDIEPRRead从EEPROM中读取32byte字节的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:24、得到完成读取32byte数据所用的时间，期望所用的时间满足要求
//@EXECUTIONFLOW:25、期望读到的数据与写入的数据相同
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0010(void)
{
	int i;
	
	int nWriteTime;
	
	int nReadTime;
	
	CSUDIOSTimeVal_S sTime;

	CSUDI_UINT8 pucDataW[8*1024];

	CSUDI_UINT8 pucDataR[8*1024];

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "分配资源失败");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"第一步测试失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 16000,"第二步测试失败");

	memset(pucBuf, 0x00, g_nEepromSize);	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"第三步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 16000,"第四步测试失败");

	for(i=0; i < g_nEepromSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"第五步测试失败");
	}
		
	memset(pucDataW, 'X' ,sizeof(pucDataW));
	
	memset(pucDataR, '\0' ,sizeof(pucDataR));
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 8*1024),"第六步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 6000,"第七步测试失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 8*1024),"第八步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 6000,"第九步测试失败");

	for(i=0; i < 8*1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucDataR[i] == pucDataW[i],"第十步测试失败");
	}

	memset(pucDataW, '8' ,sizeof(pucDataW));
	
	memset(pucDataR, '\0' ,sizeof(pucDataR));
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 4*1024),"第十一步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 3000,"第十二步测试失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 4*1024),"第十三步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 3000,"第十四步测试失败");

	for(i=0; i < 4*1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucDataR[i] == pucDataW[i],"第十五步测试失败");
	}
	
	memset(pucDataW, 'M' ,sizeof(pucDataW));
	
	memset(pucDataR, '\0' ,sizeof(pucDataR));
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 2*1024),"第十六步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 1500,"第十七步测试失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 2*1024),"第十八步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 1500,"第十九步测试失败");

	for(i=0; i < 2*1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucDataR[i] == pucDataW[i],"第二十步测试失败");
	}
	
	memset(pucDataW, 'q' ,sizeof(pucDataW));
	
	memset(pucDataR, '\0' ,sizeof(pucDataR));
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 32),"第二十一步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 30,"第二十二步测试失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 32),"第二十三步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"得到系统时间失败");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 30,"第二十四步测试失败");

	for(i=0; i < 32; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucDataR[i] == pucDataW[i],"第二十五步测试失败");
	}
	
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"释放内存失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试多个任务同时写EEPROM时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:合法参数
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:
//@EXECUTIONFLOW:1、创建任务1向EEPROM中写入数据0x55
//@EXECUTIONFLOW:2、创建任务2向EEPROM中写入数据0x88
//@EXECUTIONFLOW:3、创建任务3向EEPROM中写入数据0xee
//@EXECUTIONFLOW:4、调用CSUDIEPRRead读取EEPROM中的数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、期望得到的数据是全0x55或0x88或0xee
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0011(void)
{
	int i;
	CSUDI_UINT8 pucBuf[1024];
	int nPriority = 128;
	int nParam = 0;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;

	g_bThreadEnd1 = CSUDI_FALSE;
	g_bThreadEnd2 = CSUDI_FALSE;
	g_bThreadEnd3 = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread1", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread1Entry, &nParam, &hThreadhandle1),
			"创建任务1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread2", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread2Entry, &nParam, &hThreadhandle2),
			"创建任务2失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread3", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread3Entry, &nParam, &hThreadhandle3),
			"创建任务3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"创建成功的任务输出的任务句柄不能为空");

	while((!g_bThreadEnd1)  || (!g_bThreadEnd2) ||(!g_bThreadEnd3))
	{
		CSUDIOSThreadSleep(10);
	}
	
	memset(pucBuf, '\0', 1024);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, 1024),"测试失败");

	CSTK_ASSERT_TRUE_FATAL(((pucBuf[0] == 0x55) ||(pucBuf[0] == 0x88) ||(pucBuf[0] == 0xee )),"测试失败");

	for(i=1; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[i-1] == pucBuf[0],"测试失败");
	}

	CSTK_FATAL_POINT
	{
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
		if (hThreadhandle3 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "删除任务失败");
			   hThreadhandle3 = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试多个任务同时读EEPROM时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:合法参数
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:
//@EXECUTIONFLOW:1、调用函数CSUDIEPRWrite向EEPROM的起始地址起写入2k的数据期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、创建任务1从EEPROM的起始地址开始读取1k的数据，期望读到的数据与写入的相同
//@EXECUTIONFLOW:3、创建任务2从EEPROM的起始地址+512起读取1k的数据，期望读到的数据与写入的相同
//@EXECUTIONFLOW:4、创建任务3从EEPROM的起始地址+1024起读取1k的数据，期望读到的数据与写入的相同
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0012(void)
{
	int i;
	CSUDI_UINT8 pucBuf[2048];
	int nPriority = 128;
	int nParam = 0;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;

	g_bThreadEnd4 = CSUDI_FALSE;
	g_bThreadEnd5 = CSUDI_FALSE;
	g_bThreadEnd6 = CSUDI_FALSE;
	g_bFlag1 = CSUDI_TRUE;
	g_bFlag2 = CSUDI_TRUE;
	g_bFlag3 = CSUDI_TRUE;
	
	for(i=0; i < 1024; ++i)
	{
		pucBuf[i] = 0x55;
	}
	
	for(; i < 2048; ++i)
	{
		pucBuf[i] = 0xee;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, 2048),"测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread1", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread4Entry, &nParam, &hThreadhandle1),
			"创建任务失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread2", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread5Entry, &nParam, &hThreadhandle2),
			"创建任务失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread3", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread6Entry, &nParam, &hThreadhandle3),
			"创建任务失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"创建成功的任务输出的任务句柄不能为空");
	
	while((!g_bThreadEnd4) || (!g_bThreadEnd5) || (!g_bThreadEnd6))
	{
		CSUDIOSThreadSleep(10);
	}
		
	CSTK_ASSERT_TRUE_FATAL(((g_bFlag1 == CSUDI_TRUE)&&(g_bFlag2 == CSUDI_TRUE)&&(g_bFlag3 == CSUDI_TRUE)),"读取数据与写入数据不一致");
		
	CSTK_FATAL_POINT
	{
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
		if (hThreadhandle3 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "删除任务失败");
			   hThreadhandle3 = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:测试多个任务同时读写EEPROM时的情况
//@PRECONDITION:EEPROM模块已成功初始化
//@INPUT:合法参数
//@EXPECTATION:每一步都能输出预期结果
//@REMARK:
//@EXECUTIONFLOW:1、创建任务1向EEPROM中从起始地址起写入1024个字节的数据0x55
//@EXECUTIONFLOW:2、创建任务2向EEPROM中从起始地址+ 1024起写入1024个字节的数据0xee
//@EXECUTIONFLOW:3、创建任务3从EEPROM的起始地址起读取1024个字节的数据，期望读到的数据全部为0x55
//@EXECUTIONFLOW:4、创建任务4从EEPROM的起始地址+1024起读取1024个字节的数据，期望读到的数据全部为0xee
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0013(void)
{
	int nPriority = 128;
	int nParam = 0;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle4 = CSUDI_NULL;

	g_bThreadEnd1 = CSUDI_FALSE;
	g_bThreadEnd4 = CSUDI_FALSE;
	g_bThreadEnd6 = CSUDI_FALSE;
	g_bThreadEnd7 = CSUDI_FALSE;
	g_bFlag1 = CSUDI_TRUE;
	g_bFlag3 = CSUDI_TRUE;

	//多线程写数据
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread1", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread1Entry, &nParam, &hThreadhandle1),
			"创建任务失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread7", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread7Entry, &nParam, &hThreadhandle2),
			"创建任务失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空");
	
	while((!g_bThreadEnd1) ||(!g_bThreadEnd7))
	{
		CSUDIOSThreadSleep(10);
	}

	//多线程读数据
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread4", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread4Entry, &nParam, &hThreadhandle3),
			"创建任务失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread6", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread6Entry, &nParam, &hThreadhandle4),
			"创建任务失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle4,"创建成功的任务输出的任务句柄不能为空");
	
	while((!g_bThreadEnd4) ||(!g_bThreadEnd6))
	{
		CSUDIOSThreadSleep(10);
	}
		
	CSTK_ASSERT_TRUE_FATAL(((g_bFlag1 == CSUDI_TRUE)&&(g_bFlag3 == CSUDI_TRUE)),"读取数据与写入数据不一致");
		
	CSTK_FATAL_POINT
	{
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
		if (hThreadhandle3 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "删除任务失败");
			   hThreadhandle3 = CSUDI_NULL;
		}
		if (hThreadhandle4 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle4), "删除任务失败");
			   hThreadhandle4 = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

