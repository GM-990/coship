/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include <stdio.h>
#include<string.h>
#include "cs_ostestcase.h"
#include "cs_ostest.h"

#define OS_TEST_COUNT              1000
#define OS_MEMORY_TEST_COUNT       10
#define TEST_THREAD_STACK_SIZE     128*1024
#define TEST_MEMORY_MALLOC_SIZE    1024*1024
#define OS_MAX_THREAD_COUNT        256

extern CSUDI_BOOL g_bThreadEnd;
extern int g_nTestData;
extern int g_nCPUCount;
extern int g_nRAMSize;
extern CSUDI_BOOL s_bSupportSuspend;

CSUDI_BOOL CSTC_CS_OSUDI2_Init(void)
{
	//在本测试用例集执行前调用
	CSTC_OS_InitCfg();
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_CS_OSUDI2_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试参数全部合法时能否成功创建任务
//@PRECONDITION:传入参数全部合法
//@INPUT:1、任务名="OS_TestThread1"
//@INPUT:2、任务优先级=7
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= OS_TestThread1Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务OS_TestThread1并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:2、任务OS_TestThread1调用CSUDIOSThreadSleep 等待10ms，然后将g_bThreadEnd改为true并返回
//@EXECUTIONFLOW:3、在主任务中调用CSUDIOSThreadSleep 等待100ms，然后判断g_bThreadEnd的值，如果不为true则返回失败
//@EXECUTIONFLOW:4、在主任务中调用CSUDIOSThreadDestroy删除创建的任务并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"创建的新任务执行失败");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			  hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试任务名超过32字节时能否成功创建任务
//@PRECONDITION:
//@INPUT:1、任务名="OS_TestThread01234567890123456789"
//@INPUT:2、任务优先级=64
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= OS_TestThread1Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:任务创建失败
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务失败返回CSUDIOS_ERROR_BAD_PARAMETER，否则测试用例失败
//@EXECUTIONFLOW:2、判断任务句柄值，如果!=CSUDI_NULL调用CSUDIOSThreadDestroy删除创建的任务并返回测试用例失败，否则测试用例成功
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0002( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSThreadCreate("OS_TestThread01234567890123456789", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"创建任务名超过32字节的任务应该返回参数错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hThreadhandle,"创建失败的任务输出的任务句柄必须为空");
	
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试优先级为256时能否成功创建任务
//@PRECONDITION:
//@INPUT:1、任务名="OS_TestThread1"
//@INPUT:2、任务优先级=256
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= OS_TestThread1Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:任务创建失败
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务OS_TestThread1失败返回错误代码值，否则测试用例失败
//@EXECUTIONFLOW:2、判断任务句柄值，如果!=CSUDI_NULL调用CSUDIOSThreadDestroy删除创建的任务并返回测试用例失败，否则测试用例成功
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0003( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 256;
	int nParam = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"创建任务优先级为256的任务应该返回失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hThreadhandle,"创建失败的任务输出的任务句柄必须为空");
	
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试优先级为-1时能否成功创建任务
//@PRECONDITION:
//@INPUT:1、任务名="OS_TestThread1"
//@INPUT:2、任务优先级=-1
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= OS_TestThread1Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:任务创建失败
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务OS_TestThread1失败返回错误代码值，否则测试用例失败
//@EXECUTIONFLOW:2、判断任务句柄值，如果!=CSUDI_NULL调用CSUDIOSThreadDestroy删除创建的任务并返回测试用例失败，否则测试用例成功
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0004( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = -1;
	int nParam = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"创建任务优先级为-1的任务应该返回失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hThreadhandle,"创建失败的任务输出的任务句柄必须为空");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate
//@DESCRIPTION:测试任务能否分配到申请的栈
//@PRECONDITION:
//@INPUT:1、任务名="OS_TestThread2"
//@INPUT:2、任务优先级=128
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= OS_TestThread2Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务OS_TestThread2并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:2、创建任务之后使用while(!g_bThreadEnd)等待新任务执行完毕
//@EXECUTIONFLOW:3、在任务OS_TestThread2中定义一个255KB的int型数组并初始化，要求成功
//@EXECUTIONFLOW:4、在任务OS_TestThread2中对数组最后一个元素赋值成0x88888888，并通过nPrama将最后一个元素的值传回主任务，然后将g_bThreadEnd赋为true
//@EXECUTIONFLOW:5、主任务判断数组最后一个元素的值是否等于0x88888888，不等于则测试失败
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy删除OS_TestThread2，应该删除成功，如果删除失败则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0005( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 128;
	char cParam = 'D';
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread2", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread2Entry, &cParam, &hThreadhandle),
			"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

	while(!g_bThreadEnd)
	{
		CSUDIOSThreadSleep(10);
	}
	
	CSTK_ASSERT_TRUE_FATAL(cParam == 'A',"子任务中无法使用创建时分配的栈");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试入口函数为空时能否成功创建任务
//@PRECONDITION:
//@INPUT:1、任务名="OS_TestThread1"
//@INPUT:2、任务优先级=64
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= CSUDI_NULL
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:创建失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务OS_TestThread1失败返回错误代码值，否则测试用例失败
//@EXECUTIONFLOW:2、判断任务句柄值，如果!=CSUDI_NULL调用CSUDIOSThreadDestroy删除创建的任务并返回测试用例失败，否则测试用例成功
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0006( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 CSUDI_NULL, &nParam, &hThreadhandle),
			"创建任务入口函数为空的任务应该返回失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hThreadhandle,"创建失败的任务输出的任务句柄必须为空");
	
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试参数能否正常传递
//@PRECONDITION:可以成功创建任务
//@INPUT:合法参数
//@EXPECTATION:参数传递正确
//@EXECUTIONFLOW:1、主任务调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread3_1,优先级64,堆栈大小为128kb,入口函数OS_TestThread3_1Entry,函数入口参数为0x00000000，要求创建成功
//@EXECUTIONFLOW:2、在任务OS_TestThread3_1中获取传递的参数判断是否等于0x00000000，如果相等则将本函数的入口参数值改为1
//@EXECUTIONFLOW:3、主任务调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread3_2,优先级64,堆栈大小为128kb,入口函数OS_TestThread3_2Entry,函数入口参数为0xffffffff，要求创建成功
//@EXECUTIONFLOW:4、在任务OS_TestThread3_2中获取传递的参数判断是否等于0xffffffff，如果相等则将本函数的入口参数值改为2
//@EXECUTIONFLOW:5、主任务调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread3_3,优先级64,堆栈大小为128kb,入口函数OS_TestThread3_3Entry,函数入口参数为0x8acdefc2，要求创建成功
//@EXECUTIONFLOW:6、在任务OS_TestThread3_3中获取传递的参数判断是否等于0x8acdefc2，如果相等则将本函数的入口参数值改为3
//@EXECUTIONFLOW:7、主任务调用CSUDIOSThreadSleep等待1秒后判断以上各函数的入口参数是否分别为1、2、3，如果全部正确则测试成功，否则失败
//@EXECUTIONFLOW:8、删除所有创建的任务，若删除失败则测试失败
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0007( void )
{
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;
	int nPriority = 64;
	int nParam1 = 0x00000000;
	int nParam2 = 0xffffffff;
	int nParam3 = 0x8acdefc2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread3_1", nPriority , TEST_THREAD_STACK_SIZE, 
							 OS_TestThread3_1Entry, &nParam1, &hThreadhandle1),
			"创建任务1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread3_2", nPriority , TEST_THREAD_STACK_SIZE, 
							 OS_TestThread3_2Entry, &nParam2, &hThreadhandle2),
			"创建任务2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread3_3", nPriority , TEST_THREAD_STACK_SIZE, 
							 OS_TestThread3_3Entry, &nParam3, &hThreadhandle3),
			"创建任务3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"创建成功的任务输出的任务句柄不能为空");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(((nParam1 == 1) && (nParam2 == 2) && (nParam3 == 3)), "参数传递错误");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle1 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务1失败");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务2失败");
			   hThreadhandle2 = CSUDI_NULL;
		}
		if (hThreadhandle3 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "删除任务3失败");
			   hThreadhandle3 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试任务名为CSUDI_NULL时能否成功创建任务
//@PRECONDITION:
//@INPUT:1、任务名=CSUDI_NULL
//@INPUT:2、任务优先级=7
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= OS_TestThread1Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:2、在新创建的任务中调用CSUDIOSThreadSleep 等待10ms，然后将g_bThreadEnd改为true并返回
//@EXECUTIONFLOW:3、在主任务中调用CSUDIOSThreadSleep 等待100ms，然后判断g_bTestPassed的值，若为true则返回成功，否则返回失败
//@EXECUTIONFLOW:4、在主任务中调用CSUDIOSThreadDestroy删除创建的任务并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0008( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(CSUDI_NULL, nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"创建的新任务执行失败");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试任务名为空时能否成功创建任务
//@PRECONDITION:
//@INPUT:1、任务名=""
//@INPUT:2、任务优先级=7
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= OS_TestThread1Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:2、在新创建的任务中调用CSUDIOSThreadSleep 等待10ms，然后将g_bThreadEnd改为true并返回
//@EXECUTIONFLOW:3、在主任务中调用CSUDIOSThreadSleep 等待100ms，然后判断g_bTestPassed的值，若为true则返回成功，否则返回失败
//@EXECUTIONFLOW:4、在主任务中调用CSUDIOSThreadDestroy删除创建的任务并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0009( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"创建的新任务执行失败");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试能否成功创建128个栈大小为8K的任务
//@PRECONDITION:传入参数全部合法
//@INPUT:1、任务名="OS_TestThread4_1 ~ OS_TestThread4_128"
//@INPUT:2、任务优先级=64
//@INPUT:3、申请栈大小=8KB
//@INPUT:4、任务入口函数= OS_TestThread4_1Entry ~ OS_TestThread4_128Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、定义数组int aParam[128]，初始化成0，将g_bThreadEnd赋值为false
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate创建128个任务并全部返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:3、在每个任务中对相应的数组元素赋值，如在OS_TestThread4_64中aParam[63] = 1，然后通过while(!g_bThreadEnd)保证创建的任务全部在运行
//@EXECUTIONFLOW:4、在主任务中调用CSUDIOSThreadSleep 等待1000ms，然后将g_bThreadEnd赋值为true
//@EXECUTIONFLOW:5、主任务读取数组值进行校验，要求全部校验成功，否则测试用例失败
//@EXECUTIONFLOW:6、主任务调用CSUDIOSThreadDestroy删除创建的任务并全部返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0010( void )
{
	CSUDI_HANDLE hThreadhandle[128] = {CSUDI_NULL};
	int nPriority = 64;
	int aParam[128] = {0};
	int i;

	char aacThreadName[128][32] = {{0}};
	char acNameNumber[4];

	g_bThreadEnd = CSUDI_FALSE;

	for (i = 1 ; i<=128;i++)
	{
		strcat(aacThreadName[i-1],"OS_TestThread4_");
		sprintf(acNameNumber, "%d", i); 
		strcat(aacThreadName[i-1],acNameNumber);
	}
	
	for(i=0 ; i<128 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(aacThreadName[i], nPriority , 8*1024, 
								 OS_TestThread4Entry, &aParam[i], &hThreadhandle[i]),
				 "创建任务失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle[i],"创建成功的任务输出的任务句柄不能为空");
	}

	CSUDIOSThreadSleep(1000);

	g_bThreadEnd = CSUDI_TRUE;
	
	CSUDIOSThreadSleep(2000);
	
	for (i=0 ; i<128 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(aParam[i] == 1, "校验失败");
	}

	CSTK_FATAL_POINT
	{
		if (g_bThreadEnd == CSUDI_FALSE)
		{
			g_bThreadEnd = CSUDI_TRUE;
			CSUDIOSThreadSleep(2000);
		}

		for (i=0 ; i<128 ; i++)
		{
			if (hThreadhandle[i] != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle[i]), "删除任务失败");
				hThreadhandle[i] = CSUDI_NULL;
			}
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试申请栈大小为2K时能否成功创建任务
//@PRECONDITION:
//@INPUT:1、任务名="OS_TestThread1"
//@INPUT:2、任务优先级=7
//@INPUT:3、申请栈大小=2KB
//@INPUT:4、任务入口函数= OS_TestThread1Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:2、在新创建的任务中调用CSUDIOSThreadSleep 等待10ms，然后将g_bThreadEnd改为true并返回
//@EXECUTIONFLOW:3、在主任务中调用CSUDIOSThreadSleep 等待100ms，然后判断g_bTestPassed的值，若为true则返回成功，否则返回失败
//@EXECUTIONFLOW:4、在主任务中调用CSUDIOSThreadDestroy删除创建的任务并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0011( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2048, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"创建的新任务执行失败");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:测试任务名相同时能否成功创建任务
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、主任务调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread5，优先级为64，栈大小为256KB，任务入口函数为OS_TestThread5_1Entry，入口函数参数为0，要求返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:2、函数OS_TestThread5_1Entry调用CSUDIOSThreadSleep 等待10ms后通过while(!g_bThreadEnd)阻塞，如果跳出while循环则将传入的参数改为1
//@EXECUTIONFLOW:3、主任务调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread5，优先级为64，栈大小为256KB，任务入口函数为OS_TestThread5_2Entry，入口函数参数为0，要求返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:4、函数OS_TestThread5_2Entry调用CSUDIOSThreadSleep 等待10ms后通过while(!g_bThreadEnd)阻塞，如果跳出while循环则将传入的参数改为1
//@EXECUTIONFLOW:5、主任务调用CSUDIOSThreadSleep 等待50ms，然后将g_bThreadEnd的值从false改为true
//@EXECUTIONFLOW:6、主任务调用CSUDIOSThreadSleep 等待50ms后判断两个入口函数参数的值，若都为1则返回成功，否则返回失败
//@EXECUTIONFLOW:7、然后调用CSUDIOSThreadDestroy删除创建的任务并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0012 (void )
{
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	int nPriority = 64;
	int nParam1 = 0;
	int nParam2 = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread5", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread5_1Entry, &nParam1, &hThreadhandle1),
			"创建任务1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread5", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread5_2Entry, &nParam2, &hThreadhandle2),
			"创建任务2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空");

	CSUDIOSThreadSleep(50);

	g_bThreadEnd = CSUDI_TRUE;

	CSUDIOSThreadSleep(50);

	CSTK_ASSERT_TRUE_FATAL((nParam1 == 1) ,"任务1执行出错");
	CSTK_ASSERT_TRUE_FATAL((nParam2 == 1) ,"任务2执行出错");

	CSTK_FATAL_POINT
	{
		if (g_bThreadEnd == CSUDI_FALSE)
		{
			g_bThreadEnd = CSUDI_TRUE;
			CSUDIOSThreadSleep(50);
		}
		
		if (hThreadhandle1 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务1失败");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务2失败");
			   hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadDestroy  
//@DESCRIPTION:测试能否成功删除正在运行的任务
//@PRECONDITION: 可以成功创建任务OS_TestThread6
//@INPUT: 合法的任务句柄
//@EXPECTATION:删除失败返回CSUDIOS_ERROR_THREAD_BUSY
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread6,优先级64,堆栈大小为256kb,入口函数OS_TestThread6Entry,参数为0，要求创建成功
//@EXECUTIONFLOW:2、任务OS_TestThread6中循环执行sleep(10)十次 ，然后将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、主任务调用CSUDIOSThreadSleep 等待10ms，然后调用CSUDIOSThreadDestroy删除OS_TestThread6任务并返回
//@EXECUTIONFLOW:4、判断删除时的返回值，如果是CSUDIOS_ERROR_THREAD_BUSY则成功，否则失败
//@EXECUTIONFLOW:5、调用CSUDIOSThreadSleep等待300ms，然后判断g_bThreadEnd的值是否为true，是则成功，否则失败
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy删除OS_TestThread6任务并要求返回成功
CSUDI_BOOL CSTC_OS_IT_ThreadDestroy_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

	CSUDIOSThreadSleep(10);

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_THREAD_BUSY == CSUDIOSThreadDestroy(hThreadhandle), "删除正在运行的任务应该返回CSUDIOS_ERROR_THREAD_BUSY");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"新任务应该还在执行");
	
	CSUDIOSThreadSleep(300);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"新任务执行出错");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadDestroy  
//@DESCRIPTION:测试能否成功删除无效任务
//@PRECONDITION:
//@INPUT:任务句柄=CSUDI_NULL
//@EXPECTATION:删除失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW: 1、调用CSUDIOSThreadDestroy 删除无效任务，期望返回失败
CSUDI_BOOL CSTC_OS_IT_ThreadDestroy_0002( void )
{
	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadDestroy(CSUDI_NULL), "任务句柄为空时，删除任务不能返回成功");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@CASEGROUP:CSUDIOSThreadDestroy
//@DESCRIPTION:参数全部合法时任务创建和销毁的强度测试
//@PRECONDITION:传入参数全部合法
//@INPUT:1、任务名="OS_TestThread1"
//@INPUT:2、任务优先级=88
//@INPUT:3、申请栈大小=256KB
//@INPUT:4、任务入口函数= OS_TestThread1Entry
//@INPUT:5、任务入口函数参数= 0
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务OS_TestThread1并返回CSUDI_SUCCESS,否则测试用例失败
//@EXECUTIONFLOW:2、任务OS_TestThread1调用CSUDIOSThreadSleep 等待10ms，然后将g_bThreadEnd改为true并返回
//@EXECUTIONFLOW:3、在主任务中调用CSUDIOSThreadJoin 等待OS_TestThread1结束，然后判断g_bThreadEnd的值，如果不为true则返回失败
//@EXECUTIONFLOW:4、在主任务中调用CSUDIOSThreadDestroy删除创建的任务并返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:5、重复执行1 ~ 4步骤OS_TEST_COUNT次，OS_TEST_COUNT通过配置文件传入
CSUDI_BOOL CSTC_OS_IT_ThreadCreateDestroy_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 88;
	int nParam = 0;	
	int i;
	
	CSTCPrint("任务创建和销毁强度测试开始，请等待。。。\n");
	
	for(i=0 ; i<OS_TEST_COUNT; i++)
	{
		g_bThreadEnd = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
								 OS_TestThread1Entry, &nParam, &hThreadhandle),
				 "创建任务失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle, "创建成功的任务输出的任务句柄不能为空");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hThreadhandle), "等待子任务时出错");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "创建的新任务执行失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");

		hThreadhandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");

			  hThreadhandle = CSUDI_NULL;
		}
	}
	CSTCPrint("任务创建和销毁强度测试结束！！！\n");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSuspend  
//@DESCRIPTION:测试能否成功挂起一个正在运行的任务
//@PRECONDITION: 可以成功创建任务OS_TestThread6
//@INPUT: 合法的任务句柄
//@EXPECTATION:平台支持则成功挂起任务，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread6,优先级64,堆栈大小为256kb,入口函数OS_TestThread6Entry,参数为0，要求创建成功
//@EXECUTIONFLOW:2、任务OS_TestThread6中循环执行sleep(10)十次 ，然后将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、主任务调用CSUDIOSThreadSuspend挂起OS_TestThread6任务并返回
//@EXECUTIONFLOW:4、判断挂起时的返回值，如果平台支持任务挂起则返回成功并执行步骤5~7，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED并直接跳至步骤7
//@EXECUTIONFLOW:5、等待500ms后判断g_bThreadEnd的值，要求为CSUDI_FALSE，否则说明任务并没有真的挂起，测试失败
//@EXECUTIONFLOW:6、调用CSUDIOSThreadResume唤醒任务，然后等待150ms，再判断g_bThreadEnd的值，要求为CSUDI_TRUE
//@EXECUTIONFLOW:7、调用CSUDIOSThreadDestroy删除OS_TestThread6任务并要求返回成功
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

	if(s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadSuspend(hThreadhandle), "步骤3挂起任务失败");

		CSUDIOSThreadSleep(500);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadResume(hThreadhandle), "唤醒任务失败");

		CSUDIOSThreadSleep(300);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤6失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadSuspend(hThreadhandle), "挂起任务应该返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
	}
	
	while(g_bThreadEnd == CSUDI_FALSE)
	{
		CSUDIOSThreadSleep(10);
	}
	
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			if(g_bThreadEnd != CSUDI_TRUE)
			{
				CSUDIOSThreadSleep(300);
			}
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤7删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSuspend  
//@DESCRIPTION:参数为CSUDI_NULL的测试
//@PRECONDITION: 
//@INPUT: CSUDI_NULL
//@EXPECTATION:平台支持则返回失败，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1、调用CSUDIOSThreadSuspend挂起任务句柄为空的任务，平台支持要求返回失败，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0002( void )
{
	if(s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadSuspend(CSUDI_NULL), "步骤1失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadSuspend(CSUDI_NULL), "步骤1失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSuspend  
//@DESCRIPTION:测试能否成功挂起一个已经挂起的任务
//@PRECONDITION: 可以成功创建任务OS_TestThread6
//@INPUT: 合法的任务句柄
//@EXPECTATION:平台支持则成功挂起任务，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread6,优先级64,堆栈大小为256kb,入口函数OS_TestThread6Entry,参数为0，要求创建成功
//@EXECUTIONFLOW:2、任务OS_TestThread6中循环执行sleep(10)十次 ，然后将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、主任务调用CSUDIOSThreadSuspend挂起OS_TestThread6任务并返回
//@EXECUTIONFLOW:4、判断挂起时的返回值，如果平台支持任务挂起则返回成功并执行步骤5~8，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED并直接跳至步骤7
//@EXECUTIONFLOW:5、调用CSUDIOSThreadSuspend再次挂起OS_TestThread6任务并返回成功
//@EXECUTIONFLOW:6、等待300ms后判断g_bThreadEnd的值，要求为CSUDI_FALSE，否则说明任务并没有真的挂起，测试失败
//@EXECUTIONFLOW:7、调用CSUDIOSThreadResume唤醒任务，然后等待150ms，再判断g_bThreadEnd的值，要求为CSUDI_TRUE
//@EXECUTIONFLOW:8、调用CSUDIOSThreadSuspend挂起一个已经自行返回的任务，要求返回失败
//@EXECUTIONFLOW:9、调用CSUDIOSThreadDestroy删除OS_TestThread6任务并要求返回成功
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0003( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

	if(s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadSuspend(hThreadhandle), "步骤3挂起任务失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadSuspend(hThreadhandle), "步骤5失败");

		CSUDIOSThreadSleep(300);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadResume(hThreadhandle), "唤醒任务失败");
		
		CSUDIOSThreadSleep(300);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadSuspend(hThreadhandle), "步骤8失败");		
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadSuspend(hThreadhandle), "挂起任务应该返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
	}
	
	while(g_bThreadEnd == CSUDI_FALSE)
	{
		CSUDIOSThreadSleep(10);
	}
	
	CSTK_FATAL_POINT;
	
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			if(g_bThreadEnd != CSUDI_TRUE)
			{
				CSUDIOSThreadSleep(300);
			}
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤9删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSuspend  
//@DESCRIPTION:测试能否成功挂起一个已经返回的任务
//@PRECONDITION: 可以成功创建任务OS_TestThread1
//@INPUT: 合法的任务句柄
//@EXPECTATION:平台支持则返回失败，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread1,优先级64,堆栈大小为256kb,入口函数OS_TestThread1Entry,参数为0，要求创建成功
//@EXECUTIONFLOW:2、任务OS_TestThread1中等待10ms，然后将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、主任务调用CSUDIOSThreadSleep 等待50ms，然后调用CSUDIOSThreadSuspend挂起OS_TestThread1任务并返回
//@EXECUTIONFLOW:4、判断挂起时的返回值，如果平台支持任务挂起则返回失败，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5、判断g_bThreadEnd的值要求为CSUDI_TRUE
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy删除OS_TestThread6任务并要求返回成功
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0004( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

	if(s_bSupportSuspend)
	{
		CSUDIOSThreadSleep(50);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadSuspend(hThreadhandle), "步骤3挂起任务应该返回失败");
	}
	else
	{
		CSUDIOSThreadSleep(50);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadSuspend(hThreadhandle), "挂起任务应该返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤5失败");
		
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			if(g_bThreadEnd != CSUDI_TRUE)
			{
				CSUDIOSThreadSleep(50);
			}
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadResume 
//@DESCRIPTION:测试能否成功唤醒一个正在运行的任务
//@PRECONDITION: 可以成功创建任务OS_TestThread6
//@INPUT: 合法的任务句柄
//@EXPECTATION:平台支持则返回成功，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread6,优先级64,堆栈大小为256kb,入口函数OS_TestThread6Entry,参数为0，要求创建成功
//@EXECUTIONFLOW:2、任务OS_TestThread6中循环执行sleep(10)十次 ，然后将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、主任务调用CSUDIOSThreadResume唤醒OS_TestThread6任务并返回
//@EXECUTIONFLOW:4、判断唤醒时的返回值，如果平台支持则返回成功，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5、调用CSUDIOSThreadSleep等待300ms，然后判断g_bThreadEnd的值是否为true，是则成功，否则失败
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy删除OS_TestThread6任务并要求返回成功
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

	if(s_bSupportSuspend)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadResume(hThreadhandle), "唤醒任务失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadResume(hThreadhandle), "唤醒任务应该返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
	}

	CSUDIOSThreadSleep(300);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"新任务执行出错");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			if(g_bThreadEnd != CSUDI_TRUE)
			{
				CSUDIOSThreadSleep(300);
			}
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadResume 
//@DESCRIPTION:测试能否成功唤醒一个已经返回的任务
//@PRECONDITION: 可以成功创建任务OS_TestThread1
//@INPUT: 合法的任务句柄
//@EXPECTATION:平台支持则返回失败，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread1,优先级64,堆栈大小为256kb,入口函数OS_TestThread1Entry,参数为0，要求创建成功
//@EXECUTIONFLOW:2、任务OS_TestThread1等待10ms ，然后将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、主任务等待50ns后调用CSUDIOSThreadResume唤醒OS_TestThread1任务并返回
//@EXECUTIONFLOW:4、判断唤醒时的返回值，如果平台支持则返回失败，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5、判断g_bThreadEnd的值要求为CSUDI_TRUE
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy删除OS_TestThread6任务并要求返回成功
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0002( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

	if(s_bSupportSuspend)
	{		
		CSUDIOSThreadSleep(50);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadResume(hThreadhandle), "唤醒任务失败");
	}
	else
	{
		CSUDIOSThreadSleep(50);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadResume(hThreadhandle), "唤醒任务应该返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤5失败");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			if(g_bThreadEnd != CSUDI_TRUE)
			{
				CSUDIOSThreadSleep(50);
			}
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadResume  
//@DESCRIPTION:参数为CSUDI_NULL的测试
//@PRECONDITION: 
//@INPUT: CSUDI_NULL
//@EXPECTATION:平台支持则返回失败，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1、调用CSUDIOSThreadResume唤醒任务句柄为空的任务，平台支持要求返回失败，否则返回CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0003( void )
{
	if(s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadResume(CSUDI_NULL), "步骤1失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadResume(CSUDI_NULL), "步骤1失败");
	}
	
	CSTK_FATAL_POINT

	return CSUDI_TRUE; 
}

//@CASEGROUP:CSUDIOSThreadJoin 
//@DESCRIPTION:测试一个任务等待另一个正在运行的任务是否成功
//@PRECONDITION:可以成功创建任务OS_TestThread6
//@INPUT:OS_TestThread6的任务句柄
//@EXPECTATION:可以等待成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、主任务调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread6,优先级64,堆栈大小为256kb,入口函数OS_TestThread6Entry,参数为0，要求创建成功
//@EXECUTIONFLOW:2、任务OS_TestThread6中循环执行sleep(10)十次 ，然后将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、主任务调用CSUDIOSThreadJoin等待OS_TestThread5，如果等待成功判断g_bThreadEnd是否等于true，等于则返回成功，否则失败
//@EXECUTIONFLOW:4、调用CSUDIOSThreadDestroy删除刚创建的任务并返回CSUDI_SUCCESS，否则失败
CSUDI_BOOL CSTC_OS_IT_ThreadJoin_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hThreadhandle), "当任务句柄有效时应该等待成功");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"新任务执行出错");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadJoin 
//@DESCRIPTION:测试一个任务等待另一个已经返回的任务是否成功
//@PRECONDITION:1、可以成功创建任务OS_TestThread1
//@INPUT:OS_TestThread1的任务句柄
//@EXPECTATION:可以成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread1,优先级7,堆栈大小为256kb,入口函数OS_TestThread1Entry,参数为0，要求创建成功
//@EXECUTIONFLOW:2、任务OS_TestThread1调用CSUDIOSThreadSleep 等待10ms，然后将g_bThreadEnd改为true并返回
//@EXECUTIONFLOW:3、在主任务中调用CSUDIOSThreadSleep等待100ms，然后调用CSUDIOSThreadJoin等待OS_TestThread1
//@EXECUTIONFLOW:4、判断步骤3的返回值，如果是CSUDI_SUCCESS，则删除任务OS_TestThread1，返回成功，否则删除任务测试用例返回失败
CSUDI_BOOL CSTC_OS_IT_ThreadJoin_0002( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hThreadhandle), "当任务句柄有效时应该等待成功");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSelf  
//@DESCRIPTION:测试能否正确获取一个任务的句柄
//@PRECONDITION:可以成功创建任务OS_TestThread7
//@INPUT:
//@EXPECTATION:可以成功获取任务句柄并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务，任务名为OS_TestThread7,优先级7,堆栈大小为256kb,入口函数OS_TestThread7Entry,参数为CSUDI_NULL，要求创建成功
//@EXECUTIONFLOW:2、在OS_TestThread7调用CSUDIOSThreadSelf获取任务自身的句柄并自行返回
//@EXECUTIONFLOW:3、判断获取的任务句柄和创建时的任务句柄是否相等，不相等则测试用例失败
//@EXECUTIONFLOW:4、删除创建的任务，要求删除成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadSelf_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	CSUDI_HANDLE hParamhandle = CSUDI_NULL;	
	int nPriority = 7;

	g_bThreadEnd = CSUDI_FALSE;

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread7", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread7Entry, &hParamhandle, &hThreadhandle),
			 "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(hParamhandle == hThreadhandle,"获取的任务句柄和生成的不一致");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSelf
//@CASEGROUP:CSUDIOSThreadJoin 
//@DESCRIPTION:测试参数为CSUDI_NULL的时候，函数的执行结果
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSThreadSelf，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSThreadJoin，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_ThreadSelfJoin_0001( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadSelf(CSUDI_NULL), "步骤1失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadJoin(CSUDI_NULL), "步骤2失败");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSleep  
//@DESCRIPTION:测试将一个任务挂起一段时间的准确性
//@PRECONDITION:
//@INPUT:1000
//@EXPECTATION:容错在正负10ms
//@EXECUTIONFLOW:1、调用CSUDIOSGetTime 获取当前系统时间并返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSThreadSleep等待1000ms
//@EXECUTIONFLOW:3、调用CSUDIOSGetTime 再次获取当前系统时间并返回成功
//@EXECUTIONFLOW:4、判断两次获取系统时间的差值和等待时间的差，如果容错在正负10ms则返回成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadSleep_0001( void )
{
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "第一次获取系统时间失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "第二次获取系统时间失败");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 1000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-10) && (nMiliSecond <=10), "等待的时间不在系统容错值内");
	
	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadYield  
//@DESCRIPTION:测试调用CSUDIOSThreadYield接口能否放弃本次执行权
//@PRECONDITION:可以成功创建任务
//@INPUT:
//@EXPECTATION:可以放弃本次执行权
//@EXECUTIONFLOW:1、调用CSUDIThreadCreate 创建两个子任务要求返回成功
//@EXECUTIONFLOW:2、子任务1是优先级为255的高优先级任务，在此任务中一直调用CSUDIOSThreadYield接口放弃执行权
//@EXECUTIONFLOW:3、子任务2是优先级为0的低优先级任务，该任务为一个10个元素的数组赋值，如果该任务执行完则将g_bThreadEnd赋值为true
//@EXECUTIONFLOW:4、主任务判断g_bThreadEnd的值为true后，再判断任务2执行所用的时间，要求为1秒内，否则测试失败
//@EXECUTIONFLOW:5、删除创建的任务，要求删除成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_ThreadYield_0001( void )
{
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread8_1",255,8*1024,OS_TestThread8_1Entry,CSUDI_NULL,&hThreadhandle1),"创建子任务1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"子任务1句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread8_2",0,8*1024,OS_TestThread8_2Entry,CSUDI_NULL,&hThreadhandle2),"创建子任务2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"子任务2句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "第一次获取系统时间失败");

	while(g_bThreadEnd == CSUDI_FALSE)
	{
		CSUDIOSThreadSleep(10);
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "第二次获取系统时间失败");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond); 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond <=1000), "任务2没在规定时间内执行完");

	CSUDIOSThreadSleep(20);
	
	CSTK_FATAL_POINT
	{
		if(CSUDI_NULL != hThreadhandle1)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1),"删除任务1失败");
			hThreadhandle1 = CSUDI_NULL;
		}
		
		if(CSUDI_NULL != hThreadhandle2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2),"删除任务2失败");
			hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试能否成功创建消息队列
//@PRECONDITION:传入参数全部合法
//@INPUT:1、消息队列名称="OS_TestMsgQueue1"
//@INPUT:2、最大消息个数=32
//@INPUT:3、消息最大长度=64
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列OS_TestMsgQueue1要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，应该销毁成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0001( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试队列名超过32字节时能否成功创建消息队列
//@PRECONDITION:
//@INPUT:1、消息队列名称="OS_TestMsgQueue01234567890123456789"
//@INPUT:2、最大消息个数=32
//@INPUT:3、消息最大长度=64
//@EXPECTATION:创建失败并返回CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列OS_TestMsgQueue01234567890123456789并返回
//@EXECUTIONFLOW:2、判断步骤1的返回值，应该等于CSUDIOS_ERROR_BAD_PARAMETER，否则失败
//@EXECUTIONFLOW:3、如果队列句柄非空则调用CSUDIOSMsgQueueDestroy销毁创建的消息队列且返回失败，否则返回成功
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0002( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSMsgQueueCreate("OS_TestMsgQueue01234567890123456789", nMaxMsgs, nSizePerMsg, &hMessageQHandle),
		"创建任务名超过32字节的消息队列应该返回参数错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hMessageQHandle, "创建失败的消息队列输出的句柄必须为空");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试队列名CSUDI_NULL时能否成功创建消息队列
//@PRECONDITION:
//@INPUT:1、消息队列名称=CSUDI_NULL
//@INPUT:2、最大消息个数=32
//@INPUT:3、消息最大长度=64
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建名称为空的消息队列要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，应该销毁成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0003( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate(CSUDI_NULL,nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试最大消息个数为0时能否成功创建消息队列
//@PRECONDITION:
//@INPUT:1、消息队列名称="OS_TestMsgQueue1"
//@INPUT:2、最大消息个数=0
//@INPUT:3、消息最大长度=64
//@EXPECTATION:创建失败并返回错误代码(非CSUDI_SUCCESS)且消息队列句柄为CSUDI_NULL
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列OS_TestMsgQueue1并返回错误代码值
//@EXECUTIONFLOW:2、判断步骤1的返回值，应该不等于CSUDI_SUCCESS，否则失败
//@EXECUTIONFLOW:3、如果队列句柄非空则调用CSUDIOSMsgQueueDestroy销毁创建的消息队列且返回失败，否则返回成功
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0004( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 0;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueCreate("OS_TestMsgQueue1", nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建最大消息个数为0的消息队列应该返回失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hMessageQHandle, "创建失败的消息队列输出的句柄必须为空");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试最大消息个数为128时能否成功创建消息队列
//@PRECONDITION:
//@INPUT:1、消息队列名称="OS_TestMsgQueue1"
//@INPUT:2、最大消息个数=128
//@INPUT:3、消息最大长度=64
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列OS_TestMsgQueue1要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，应该销毁成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0005( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 128;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试消息最大长度为0时能否成功创建消息队列
//@PRECONDITION:
//@INPUT:1、消息队列名称="OS_TestMsgQueue1"
//@INPUT:2、最大消息个数=32
//@INPUT:3、消息最大长度=0
//@EXPECTATION:创建失败并返回错误代码(非CSUDI_SUCCESS)且消息队列句柄为CSUDI_NULL
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列OS_TestMsgQueue1要求返回非CSUDI_SUCCESS的错误代码值
//@EXECUTIONFLOW:2、如果队列句柄非空则调用CSUDIOSMsgQueueDestroy销毁创建的消息队列且返回失败，否则返回成功
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0006( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息最大长度为0的消息队列应该返回失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hMessageQHandle,"创建失败的消息队列输出的句柄必须为空");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试消息最大长度为128时能否成功创建消息队列
//@PRECONDITION:
//@INPUT:1、消息队列名称="OS_TestMsgQueue1"
//@INPUT:2、最大消息个数=64
//@INPUT:3、消息最大长度=128
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列OS_TestMsgQueue1要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，应该销毁成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0007( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 64;
	int nSizePerMsg = 128;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试能否成功创建64个消息队列
//@PRECONDITION:传入参数全部合法
//@INPUT:1、消息队列名称="OS_TestMsgQueue1_1 ~ OS_TestMsgQueue1_64"
//@INPUT:2、最大消息个数=32
//@INPUT:3、消息最大长度=64
//@EXPECTATION:全部创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列OS_TestMsgQueue1_1 ~ OS_TestMsgQueue1_64要求全部返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueDestroy销毁所有创建的消息队列，应该全部销毁成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0008( void )
{
	CSUDI_HANDLE hMessageQHandle[64] = {CSUDI_NULL};
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int i;

	char aacMsgQName[64][32] = {{0}};
	char acNameNumber[4];

	for (i = 1 ; i<=64;i++)
	{
		strcat(aacMsgQName[i-1],"OS_TestMsgQueue1_");
		sprintf(acNameNumber, "%d", i); 
		strcat(aacMsgQName[i-1],acNameNumber);
	}

	for (i=0 ; i<64 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate(aacMsgQName[i],nMaxMsgs,nSizePerMsg,&hMessageQHandle[i]), "创建消息队列失败");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<64;i++)
		{
			if(hMessageQHandle[i] != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle[i]), "销毁消息队列失败");
				hMessageQHandle[i] = CSUDI_NULL;
			}
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:测试消息队列名称相同时能否成功创建消息队列
//@PRECONDITION:传入参数全部合法
//@INPUT:1、消息队列名称="OS_TestMsgQueue1"
//@INPUT:2、最大消息个数=32
//@INPUT:3、消息最大长度=64
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列OS_TestMsgQueue1要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、再次调用CSUDIOSMsgQueueCreate创建消息队列名称为OS_TestMsgQueue1的消息队列，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，应该销毁成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0009( void )
{
	CSUDI_HANDLE hMessageQHandle1 = CSUDI_NULL;
	CSUDI_HANDLE hMessageQHandle2 = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle1),
		"创建消息队列失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle1,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle2),
		"创建消息队列失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle2,"创建成功的消息队列任务句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle1 != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle1), "销毁消息队列失败");
			 hMessageQHandle1 = CSUDI_NULL;
		}

		if(hMessageQHandle2 != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle2), "销毁消息队列失败");
			 hMessageQHandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueDestroy   
//@DESCRIPTION:测试能否成功删除无效的消息队列
//@PRECONDITION:
//@INPUT: CSUDI_NULL
//@EXPECTATION:删除失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueDestroy,传入参数为CSUDI_NULL,期望返回非CSUDI_SUCCESS。
CSUDI_BOOL CSTC_OS_IT_MsgQueueDestroy_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueDestroy(CSUDI_NULL), "消息队列为空时，删除不能返回成功");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:测试能否成功向消息队列里发送消息
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、需要发送的消息数据0x88888888
//@INPUT:3、地址分配的内存长度=4
//@INPUT:4、等待超时时间=500ms 
//@EXPECTATION:发送成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为32，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列，超时时间为500ms ，要求发送成功
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive从消息队列中接收一条数据，超时时间为500ms ，要求接收成功
//@EXECUTIONFLOW:4、判断接收到的数据和发送的是否相等，不等则测试用例失败
//@EXECUTIONFLOW:5、调用CSUDIOSMsgQueueDestroy销毁刚创建的消息队列并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0001( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nSendMsg = 0x88888888;
	int nMsgBytes = 4;
	int anRecvMsg[1];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nSendMsg,nMsgBytes,500), "发送消息失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,nMsgBytes,500), "接收消息失败");

	CSTK_ASSERT_TRUE_FATAL((anRecvMsg[0] == 0x88888888), "接收到得消息和发送的不相等");  

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:测试能否成功向无效消息队列里发送消息
//@PRECONDITION:
//@INPUT:1、无效的消息队列句柄(CSUDI_NULL)
//@INPUT:2、需要发送的消息数据0x88888888
//@INPUT:3、地址分配的内存长度=4
//@INPUT:4、等待超时时间=500ms 
//@EXPECTATION:发送失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueSend发送一条消息到无效消息队列
//@EXECUTIONFLOW:2、判断CSUDIOSMsgQueueSend的返回值，不等于CSUDI_SUCCESS则返回成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0002( void )
{
	int nMsg = 0x88888888;
	int nMsgBytes = 4;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueSend(CSUDI_NULL,&nMsg,nMsgBytes,500), "往无效的消息队列中发送消息不能成功");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:测试数据地址为空时能否成功向消息队列里发送消息
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、发送消息数据的内存地址=CSUDI_NULL
//@INPUT:3、地址分配的内存长度=4
//@INPUT:4、等待超时时间=500ms 
//@EXPECTATION:发送失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为32，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条数据地址为空的消息到消息队列OS_TestMsgQueue1
//@EXECUTIONFLOW:3、判断CSUDIOSMsgQueueSend的返回值，不等于CSUDI_SUCCESS则返回成功，否则测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，要求返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0003( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nMsgBytes = 4;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueSend(hMessageQHandle,CSUDI_NULL,nMsgBytes,500), "数据地址为空时发送消息不能成功");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:测试内存长度为0时能否成功向消息队列里发送消息
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、需要发送的消息数据0x88888888
//@INPUT:3、地址分配的内存长度=0
//@INPUT:4、等待超时时间=500ms 
//@EXPECTATION:发送失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为32，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列OS_TestMsgQueue1
//@EXECUTIONFLOW:3、判断CSUDIOSMsgQueueSend的返回值，不等于CSUDI_SUCCESS则返回成功，否则测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，要求返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0004( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,0,500), "内存长度为0时发送消息不能成功");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:测试各种超时设置情况能否成功向消息队列里发送消息
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、需要发送的消息数据0x88888888
//@INPUT:3、地址分配的内存长度=4
//@INPUT:4、等待超时时间:执行步骤中描述
//@EXPECTATION:每一步都达到期望值
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为1，消息最大长度为8，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列OS_TestMsgQueue1，超时时间CSUDIOS_TIMEOUT_IMMEDIATE
//@EXECUTIONFLOW:3、判断CSUDIOSMsgQueueSend的返回值，等于CSUDI_SUCCESS则返回成功，否则测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueSend再发送一条消息到消息队列OS_TestMsgQueue1超时时间CSUDIOS_TIMEOUT_IMMEDIATE
//@EXECUTIONFLOW:5、判断CSUDIOSMsgQueueSend的返回值，等于CSUDIOS_ERROR_TIMEOUT则返回成功，否则测试用例失败
//@EXECUTIONFLOW:6、调用CSUDIOSMsgQueueSend再发送一条消息到消息队列OS_TestMsgQueue1超时时间100ms
//@EXECUTIONFLOW:7、判断CSUDIOSMsgQueueSend的返回值，等于CSUDIOS_ERROR_TIMEOUT则返回成功，否则测试用例失败
//@EXECUTIONFLOW:8、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，要求返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0005( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 8;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "发送消息失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "往已满的消息队列中发送消息应该超时");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,100), "往已满的消息队列中发送消息应该超时");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:测试当发送的消息长度超过消息队列每个消息最大长度时能否发送成功
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、需要发送的消息数据char acSendMsg[5] = "abcd"
//@INPUT:3、地址分配的内存长度=4
//@INPUT:4、等待超时时间=CSUDIOS_TIMEOUT_IMMEDIATE 
//@EXPECTATION:每一步都达到期望值
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为1，消息最大长度为2，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列OS_TestMsgQueue1，超时时间CSUDIOS_TIMEOUT_IMMEDIATE，发送消息长度为4,应该成功
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive从消息队列中接收一条消息，接收长度为4，要求成功但实际接收到的数据为2字节即"ab"
//@EXECUTIONFLOW:4、调用strlen判断接收到的消息长度，应该为2，否则失败
//@EXECUTIONFLOW:5、调用strncmp将发送和接收的消息前2个字节进行比较应该相等，否则失败
//@EXECUTIONFLOW:6、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，要求返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0006( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 2;
	int nMsgBytes = 4;

	char acSendMsg[5] = "abcd";
	char acRecvMsg[5] = {0};

	memset(acRecvMsg,'F',sizeof(acRecvMsg) - 1);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,acSendMsg,nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "发送消息失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,acRecvMsg,nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL((acRecvMsg[0] == 'a') && (acRecvMsg[1] == 'b') && (acRecvMsg[2] == 'F'), "接收到的消息与预期不同");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:测试当超时参数为CSUDIOS_TIMEOUT_INFINITY时，CSUDIOSMsgQueueSend的执行结果
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、需要发送的消息数据char acSendMsg[5] = "abcd"
//@INPUT:3、地址分配的内存长度=4
//@INPUT:4、等待超时时间=CSUDIOS_TIMEOUT_INFINITY 
//@EXPECTATION:每一步都达到期望值
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为1，消息最大长度为4，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列OS_TestMsgQueue1，超时时间CSUDIOS_TIMEOUT_INFINITY，发送消息长度为4，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate，函数入口OS_TestMsgQThread3Entry中等待10s后调用CSUDIOSMsgQueueReceive，并将g_bThreadEnd赋值为CSUDI_TRUE
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueSend再发送一条消息到消息队列OS_TestMsgQueue1，超时时间CSUDIOS_TIMEOUT_INFINITY，发送消息长度为4，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、判断g_bThreadEnd，期望为CSUDI_TRUE
//@EXECUTIONFLOW:6、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIOSThreadDestroy销毁步骤3创建的线程，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0007( void )
{
    CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    int nPriority = 7;
    int nMaxMsgs = 1;
    int nSizePerMsg = 4;
    int nMsgBytes = 5;

    char acSendMsg[5] = "abcd";
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1", nMaxMsgs, nSizePerMsg, &hMessageQHandle), "步骤1，创建消息队列失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle, "创建成功的消息队列任务句柄不能为空");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle, acSendMsg, nMsgBytes, CSUDIOS_TIMEOUT_INFINITY), "步骤2，发送消息失败");

    g_bThreadEnd = CSUDI_FALSE;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread", nPriority, TEST_THREAD_STACK_SIZE, 
                            OS_TestMsgQThread3Entry, hMessageQHandle, &hThreadhandle), "步骤3，创建任务失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle, "创建成功的任务输出的任务句柄不能为空");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle, acSendMsg, nMsgBytes, CSUDIOS_TIMEOUT_INFINITY), "步骤4，发送消息失败");

    CSUDIOSThreadSleep(100);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "步骤4失败，没有等待10s就返回");

    CSTK_FATAL_POINT
   
    if (hMessageQHandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "步骤6，销毁消息队列失败");
        hMessageQHandle = CSUDI_NULL;
    }

	if(hThreadhandle != CSUDI_NULL)
    {           
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤7，删除任务失败");
        hThreadhandle = CSUDI_NULL;
    }
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:测试能否成功从无效的消息队列里接收消息
//@PRECONDITION:
//@INPUT:1、无效的消息队列句柄(CSUDI_NULL)
//@INPUT:2、地址分配的内存长度=4
//@INPUT:3、等待超时时间=500ms 
//@EXPECTATION:接收失败并返回错误代码(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueReceive向无效的消息队列中接收消息，要求返回错误代码
//@EXECUTIONFLOW:2、判断CSUDIOSMsgQueueReceive的返回值，不等于CSUDI_SUCCESS则返回成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0001( void )
{
	int anRecvMsg[1];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueReceive(CSUDI_NULL,anRecvMsg,4,500), "从无效的消息队列中接收消息应该返回失败");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:测试当接收消息数据的内存地址为空时能否成功从有消息的消息队列里接收消息
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、地址分配的内存长度=4
//@INPUT:3、等待超时时间=500ms 
//@EXPECTATION:接收失败并返回错误代码(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为32，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列，超时时间为500ms，要求发送成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive从消息队列中接收一条消息，超时时间为500ms，要求接收接收失败并返回错误代码值
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueDestroy销毁刚创建的消息队列并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0002( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,500), "发送消息失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueReceive(hMessageQHandle,CSUDI_NULL,nMsgBytes,500), "接收消息数据的内存地址为空时应该接收失败");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:测试当地址分配的内存长度为0时能否成功从有消息的消息队列里接收消息
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、地址分配的内存长度=0
//@INPUT:3、等待超时时间=500ms 
//@EXPECTATION:接收失败并返回错误代码(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为32，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列，要求发送成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive从消息队列中接收一条消息，要求接收接收失败并返回错误代码值
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueDestroy销毁刚创建的消息队列并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0003( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	int anRecvMsg[1];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,500), "发送消息失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,0,500), "接收消息数据的内存地址长度为0时应该接收失败");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:测试消息个数为1第两次发送消息是否能返回CSUDIOS_ERROR_TIMEOUT
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、消息个数=1
//@INPUT:3、等待超时时间=0ms 
//@EXPECTATION:接收失败并返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为1，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列，要求发送成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueSend发送一条消息到消息队列，要求发送失败并返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueDestroy销毁刚创建的消息队列并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0004( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	//int anRecvMsg[1];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,0), "第一次发送消息失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,0), "第二次发送消息应该返回CSUDIOS_ERROR_TIMEOUT");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:测试消息个数为第两次接收消息是否能返回CSUDIOS_ERROR_TIMEOUT
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、消息个数=1
//@INPUT:3、等待超时时间=0ms 
//@EXPECTATION:接收失败并返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为1，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列，要求发送成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive从消息队列中接收一条消息，要求接收成功
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueReceive从消息队列中接收一条消息，要求接收失败并返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:5、调用CSUDIOSMsgQueueDestroy销毁刚创建的消息队列并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0005( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	int anRecvMsg[1];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,500), "第一次发送消息失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,nMsgBytes,0), "第一次接收消息数据失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT== CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,nMsgBytes,0), "第二次接收消息数据应该返回CSUDIOS_ERROR_TIMEOUT");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive   
//@DESCRIPTION:测试当超时参数为CSUDIOS_TIMEOUT_INFINITY时，CSUDIOSMsgQueueReceive的执行结果
//@PRECONDITION:可以成功创建消息队列OS_TestMsgQueue1
//@INPUT:1、成功创建的消息队列OS_TestMsgQueue1的句柄
//@INPUT:2、需要发送的消息数据char acSendMsg[5] = "abcd"
//@INPUT:3、地址分配的内存长度=4
//@INPUT:4、等待超时时间=CSUDIOS_TIMEOUT_INFINITY 
//@EXPECTATION:每一步都达到期望值
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为1，消息最大长度为4，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate，函数入口OS_TestMsgQThread4Entry中等待10s后调用CSUDIOSMsgQueueSend，并将g_bThreadEnd赋值为CSUDI_TRUE
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive接收消息，超时时间CSUDIOS_TIMEOUT_INFINITY，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、判断g_bThreadEnd，期望为CSUDI_TRUE
//@EXECUTIONFLOW:5、调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy销毁步骤3创建的线程，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0006( void )
{
    CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    int nPriority = 7;
    int nMaxMsgs = 1;
    int nSizePerMsg = 4;
    int nMsgBytes = 5;
    char acRecvMsg[5] = {0};

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1", nMaxMsgs, nSizePerMsg, &hMessageQHandle), "步骤1，创建消息队列失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle, "创建成功的消息队列任务句柄不能为空");

    g_bThreadEnd = CSUDI_FALSE;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread", nPriority, TEST_THREAD_STACK_SIZE, 
                            OS_TestMsgQThread4Entry, hMessageQHandle, &hThreadhandle), "步骤2，创建任务失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle, "创建成功的任务输出的任务句柄不能为空");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle, acRecvMsg, nMsgBytes, CSUDIOS_TIMEOUT_INFINITY), "步骤3，接收消息失败");

    CSUDIOSThreadSleep(100);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "步骤4失败，没有等待10s就返回");

    CSTK_FATAL_POINT
   
    if (hMessageQHandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "步骤5，销毁消息队列失败");
        hMessageQHandle = CSUDI_NULL;
    }

    if(hThreadhandle != CSUDI_NULL)
    {           
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤6，删除任务失败");
        hThreadhandle = CSUDI_NULL;
    }
    return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMsgQueueCreate
//@CASEGROUP:2、CSUDIOSMsgQueueSend
//@CASEGROUP:3、CSUDIOSMsgQueueReceive
//@CASEGROUP:4、CSUDIOSMsgQueueDestroy
//@DESCRIPTION:测试当发送的消息数量为最大消息数量时能否发送成功
//@DESCRIPTION:测试发送和接受的消息次序是否一致
//@PRECONDITION:
//@INPUT:正常参数测试
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列,名称为OS_TestMsgQueue1,最大消息数量为20,每个消息的最大长度为4字节，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送消息，超时时间为CSUDIOS_TIMEOUT_IMMEDIATE，连续发送20个消息
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive 接收消息,超时时间为CSUDIOS_TIMEOUT_IMMEDIATE，比较接收到的消息值与发送的值以及次序是否相等,不相等则测试失败
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueDestroy 销毁创建的消息队列，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0001( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 20;
	int nSizePerMsg = 4;
	int nMsgBytes = 4;
	int aanSendMsg[4][5];
	int aanRecvMsg[4][5];
	int i,j;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	for (i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<5 ; j++ )
		{
			aanSendMsg[i][j] = (i * 10000000) + j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&aanSendMsg[i][j],nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "发送消息失败");
		}
	}
	
	for (i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<5 ; j++ )
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,&aanRecvMsg[i][j],nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "接收消息失败");
						
			CSTK_ASSERT_TRUE_FATAL(aanRecvMsg[i][j] == aanSendMsg[i][j],"接收到的消息和发送的不一致");
		}
	}

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMsgQueueCreate
//@CASEGROUP:2、CSUDIOSMsgQueueSend
//@CASEGROUP:3、CSUDIOSMsgQueueReceive
//@CASEGROUP:4、CSUDIOSMsgQueueDestroy
//@DESCRIPTION:1、测试从空消息队列中接收消息能否成功
//@DESCRIPTION:2、测试往满消息队列中发送消息能否成功
//@DESCRIPTION:3、测试当从满消息队列中接收一条消息后，先前发送阻塞的消息能否成功发送到消息队列
//@PRECONDITION:
//@INPUT:正常参数测试
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列,名称为OS_TestMsgQueue1,最大消息数量为1,每个消息的最大长度为4字节，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueReceive接收消息,超时时间为100毫秒,应该超时
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate 创建一个生产者任务
//@EXECUTIONFLOW:4、调用CSUDIOSThreadCreate 创建一个消费者任务
//@EXECUTIONFLOW:5、在生产者任务中调用CSUDIOSMsgQueueSend发送一条消息,应该成功
//@EXECUTIONFLOW:6、在生产者任务中调用CSUDIOSThreadSleep 等待10ms后，调用CSUDIOSMsgQueueSend再次发送一条消息,超时设置为3秒，此时该任务应该被阻塞
//@EXECUTIONFLOW:7、主任务调用CSUDIOSThreadSleep等待1秒后判断g_bThreadEnd的值，应该为fasle，否则失败
//@EXECUTIONFLOW:8、在消费者任务中调用CSUDIOSThreadSleep 等待2秒钟后，调用CSUDIOSMsgQueueReceive接收消息,超时时间为100ms，应该成功接收到消息
//@EXECUTIONFLOW:9、当步骤8接收消息成功后，在生产者任务中第二次发送被阻塞的消息应该可以发送成功，然后将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:10、主任务调用CSUDIOSThreadSleep等待2秒后判断g_bThreadEnd的值，应该为true，否则失败
//@EXECUTIONFLOW:11、主任务调用CSUDIOSMsgQueueDestroy销毁创建的消息队列，要求返回成功
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy  销毁创建的任务，要求返回成功
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0002( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	CSUDI_HANDLE hProducerThread = CSUDI_NULL;
	CSUDI_HANDLE hCustomerThread = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 4;
	int nMsgBytes = 4;
	int anRecvMsg[1];

	g_bThreadEnd = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMsgQueueReceive(hMessageQHandle, anRecvMsg, nMsgBytes, 100), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMsgQThread1", 64, 8*1024, 
									OS_TestMsgQThread1Entry, hMessageQHandle, &hProducerThread),
			"创建生产者任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hProducerThread,"成功创建的任务输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMsgQThread2", 64, 8*1024, 
									OS_TestMsgQThread2Entry, hMessageQHandle, &hCustomerThread),
			"创建消费者任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hCustomerThread,"成功创建的任务输出句柄不能为空");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"步骤7失败");

	CSUDIOSThreadSleep(2000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤10失败");
	
	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}

		if(hProducerThread != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hProducerThread), "删除任务失败");
			hProducerThread = CSUDI_NULL;
		}

		if(hCustomerThread != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hCustomerThread), "删除任务失败");
			hCustomerThread = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMsgQueueCreate
//@CASEGROUP:2、CSUDIOSMsgQueueSend
//@CASEGROUP:3、CSUDIOSMsgQueueReceive
//@CASEGROUP:4、CSUDIOSMsgQueueDestroy
//@DESCRIPTION:测试消息队列发送消息的拷贝模式
//@PRECONDITION:
//@INPUT:正常参数测试
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate创建消息队列,名称为OS_TestMsgQueue1,最大消息数量为20,每个消息的最大长度为4字节，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送消息，超时时间为CSUDIOS_TIMEOUT_IMMEDIATE，连续发送20个消息，每次发送消息完成后立即将消息地址中的数据清零
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive 接收消息,超时时间为CSUDIOS_TIMEOUT_IMMEDIATE，比较接收到的消息值与发送的值以及次序是否相等,不相等则测试失败
//@EXECUTIONFLOW:4、调用CSUDIOSMsgQueueDestroy 销毁创建的消息队列，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0003( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 20;
	int nSizePerMsg = 4;
	int nMsgBytes = 4;
	int aanSendMsg[4][5] = {{0}};
	int aanRecvMsg[4][5] = {{0}};
	int i,j;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"创建消息队列失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");


	for (i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<5 ; j++ )
		{
			aanSendMsg[i][j] = (i * 10000000) + j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&aanSendMsg[i][j],nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "发送消息失败");
			aanSendMsg[i][j] = 0;
		}
	}
	
	for (i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<5 ; j++ )
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,&aanRecvMsg[i][j],nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "接收消息失败");

			CSTK_ASSERT_TRUE_FATAL(aanRecvMsg[i][j] == ((i * 10000000) + j), "接收到的消息和发送的不一致");
		}
	}

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMsgQueueCreate
//@CASEGROUP:2、CSUDIOSMsgQueueSend
//@CASEGROUP:3、CSUDIOSMsgQueueReceive
//@CASEGROUP:4、CSUDIOSMsgQueueDestroy
//@DESCRIPTION:测试消息队列中内存泄露情况
//@PRECONDITION:
//@INPUT:正常参数测试
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为32，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列，超时时间为500ms ，要求发送成功
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive从消息队列中接收一条数据，超时时间为500ms ，要求接收成功
//@EXECUTIONFLOW:4、判断接收到的数据和发送的是否相等，不等则测试用例失败
//@EXECUTIONFLOW:5、调用CSUDIOSMsgQueueDestroy销毁刚创建的消息队列并返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:6、重复以上1 ~ 5步骤OS_TEST_COUNT次，OS_TEST_COUNT通过配置文件传入
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0004( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 4;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	int anRecvMsg[1];
	int i;

	CSTCPrint("消息队列强度测试开始，请等待。。。\n");
	
	for (i=0 ; i<OS_TEST_COUNT ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
			"创建消息队列失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,500), "发送消息失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,nMsgBytes,500), "接收消息失败");

		CSTK_ASSERT_TRUE_FATAL((anRecvMsg[0] == 0x88888888), "接收到得消息和发送的不相等");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");

		hMessageQHandle = CSUDI_NULL;
	}
	
	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "销毁消息队列失败");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	CSTCPrint("消息队列强度测试结束！！！\n");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMsgQueueCreate
//@CASEGROUP:2、CSUDIOSMsgQueueSend
//@CASEGROUP:3、CSUDIOSMsgQueueReceive
//@CASEGROUP:4、CSUDIOSMsgQueueDestroy
//@DESCRIPTION:测试CSUDIOSMsgQueueReceive接收消息越界的情况
//@PRECONDITION:
//@INPUT:正常参数测试
//@EXECUTIONFLOW:1、调用CSUDIOSMsgQueueCreate 创建消息队列，名称为OS_TestMsgQueue1，最大消息个数为10，消息最大长度为64，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMsgQueueSend发送一条消息到消息队列，消息内容为"aaaa"，消息长度为4，超时时间为500ms ，要求发送成功
//@EXECUTIONFLOW:3、调用CSUDIOSMsgQueueReceive从消息队列中接收一条数据，接收消息的数组初始化为"bbbb",接收消息的内存长度为2，超时时间为500ms ，要求接收成功
//@EXECUTIONFLOW:4、判断接收到的数据是否是"aabb"，不是则测试用例失败
//@EXECUTIONFLOW:5、重复以上2 ~ 4步骤10次
//@EXECUTIONFLOW:6、调用CSUDIOSMsgQueueDestroy，销毁刚创建的消息队列，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0005( void )
{
    CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
    int nMaxMsgs = 10;
    int nSizePerMsg = 8;
    char acSenMsg[5] = "aaaa";
    char acRecvMsg[5] = "bbbb";
    int i;
    int nTestCount = 10;
    int nRet = 0;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1", nMaxMsgs, nSizePerMsg, &hMessageQHandle),
        "步骤1，创建消息队列失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"创建成功的消息队列任务句柄不能为空");
    
    for (i=0; i<nTestCount; i++)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle, acSenMsg, 4, 500), "步骤2，发送消息失败");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle, acRecvMsg, 2, 500), "步骤3，接收消息失败");

        nRet = strncmp(acRecvMsg, "aabb", 4);
        
        CSTK_ASSERT_TRUE_FATAL(nRet == 0, "步骤4，接收消息越界");
    }
    
    CSTK_FATAL_POINT
    {
        if(hMessageQHandle != CSUDI_NULL)
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "步骤6，销毁消息队列失败");
            hMessageQHandle = CSUDI_NULL;
        }
    }

    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试正常参数能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1"
//@INPUT:2、有效信号数量=0
//@INPUT:3、最大信号数量=1
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1，要求返回CSUDI_SUCCESS并输出有效的信号量句柄
//@EXECUTIONFLOW:2、调用CSUDIOSSemDestroy 销毁创建的信号量，要求销毁成功并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_SemCreate_0001( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出的句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试有效信号数量和最大信号数量相等且不为0时能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1"
//@INPUT:2、有效信号数量=1
//@INPUT:3、最大信号数量=1
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1，要求返回CSUDI_SUCCESS并输出有效的信号量句柄
//@EXECUTIONFLOW:2、调用CSUDIOSSemDestroy 销毁创建的信号量，要求销毁成功并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_SemCreate_0002( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 1;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出的句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试正常参数能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1"
//@INPUT:2、有效信号数量=2
//@INPUT:3、最大信号数量=10
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1，要求返回CSUDI_SUCCESS并输出有效的信号量句柄
//@EXECUTIONFLOW:2、调用CSUDIOSSemDestroy 销毁创建的信号量，要求销毁成功并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_SemCreate_0003( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 2;
	int  nMaxCount = 10;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出的句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试有效信号数量和最大信号数量都为0时能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1"
//@INPUT:2、有效信号数量=0
//@INPUT:3、最大信号数量=0
//@EXPECTATION:创建失败并返回错误代码(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:2、判断输出的信号量句柄是否为CSUDI_NULL，如果不为空则调用CSUDIOSSemDestroy 销毁创建的信号量并返回失败，否则成功
CSUDI_BOOL CSTC_OS_IT_SemCreate_0004( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "当有效信号数量和最大信号数量都为0时创建应该失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"创建失败的信号量输出句柄应该为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试有效信号数量大于最大信号数量时能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1"
//@INPUT:2、有效信号数量=1
//@INPUT:3、最大信号数量=0
//@EXPECTATION:创建失败并返回错误代码(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:2、判断输出的信号量句柄是否为CSUDI_NULL，如果不为空则调用CSUDIOSSemDestroy 销毁创建的信号量并返回失败，否则成功
CSUDI_BOOL CSTC_OS_IT_SemCreate_0005( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 1;
	int  nMaxCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "当有效信号数量大于最大信号数量时创建应该失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"创建失败的信号量输出句柄应该为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试有效信号数量大于最大信号数量时能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1"
//@INPUT:2、有效信号数量=2
//@INPUT:3、最大信号数量=1
//@EXPECTATION:创建失败并返回错误代码(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:2、判断输出的信号量句柄是否为CSUDI_NULL，如果不为空则调用CSUDIOSSemDestroy 销毁创建的信号量并返回失败，否则成功
CSUDI_BOOL CSTC_OS_IT_SemCreate_0006( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 2;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "当有效信号数量大于最大信号数量时创建应该失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"创建失败的信号量输出句柄应该为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试有效信号数量大于最大信号数量时能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1"
//@INPUT:2、有效信号数量=12
//@INPUT:3、最大信号数量=10
//@EXPECTATION:创建失败并返回错误代码(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:2、判断输出的信号量句柄是否为CSUDI_NULL，如果不为空则调用CSUDIOSSemDestroy 销毁创建的信号量并返回失败，否则成功
CSUDI_BOOL CSTC_OS_IT_SemCreate_0007( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 12;
	int  nMaxCount = 10;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "当有效信号数量大于最大信号数量时创建应该失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"创建失败的信号量输出句柄应该为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试信号量名称CSUDI_NULL时能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称=CSUDI_NULL
//@INPUT:2、有效信号数量=0
//@INPUT:3、最大信号数量=1
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建名称为空的信号量，要求返回CSUDI_SUCCESS并输出有效的信号量句柄
//@EXECUTIONFLOW:2、调用CSUDIOSSemDestroy 销毁创建的信号量，要求销毁成功并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_SemCreate_0008( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate(CSUDI_NULL, nInitialCount, nMaxCount, &hSemhandle), "创建名称为空的信号量应该返回成功");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试信号量名称超过32字节时能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSemaphore01234567890123456789"
//@INPUT:2、有效信号数量=0
//@INPUT:3、最大信号数量=1
//@EXPECTATION:创建失败并返回CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1失败并返回CSUDIOS_ERROR_BAD_PARAMETER，否则测试用例失败
//@EXECUTIONFLOW:2、判断输出的信号量句柄是否为CSUDI_NULL，如果不为空则调用CSUDIOSSemDestroy 销毁创建的信号量并返回失败，否则成功
CSUDI_BOOL CSTC_OS_IT_SemCreate_0009( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSSemCreate("OS_TestSemaphore01234567890123456789", nInitialCount, nMaxCount, &hSemhandle), "创建名称超过32字节的信号量应该返回参数错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"创建失败的信号量输出句柄应该为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试最大信号数量为100时创建100个信号量能否创建成功
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1_1 ~ OS_TestSem1_100"
//@INPUT:2、有效信号数量=0
//@INPUT:3、最大信号数量=100
//@EXPECTATION:全部创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestSem1_1 ~ OS_TestSem1_100，要求全部返回CSUDI_SUCCESS并输出有效的信号量句柄
//@EXECUTIONFLOW:2、调用CSUDIOSSemDestroy 销毁创建的所有信号量，要求全部销毁成功并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_SemCreate_0010( void )
{
	CSUDI_HANDLE hSemhandle[100] = {CSUDI_NULL};
	int  nInitialCount = 0;
	int  nMaxCount = 100;
	int i;

	char aacSemName[100][32] = {{0}};
	char acNameNumber[4];

	for (i = 1 ; i<=100;i++)
	{
		strcat(aacSemName[i-1],"OS_TestSem1_");
		sprintf(acNameNumber, "%d", i); 
		strcat(aacSemName[i-1],acNameNumber);
	}

	for(i=0 ; i<100 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate(aacSemName[i], nInitialCount, nMaxCount, &hSemhandle[i]), "创建信号量失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle[i],"创建成功的信号量输出句柄不能为空");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<100 ; i++)
		{
			if (hSemhandle[i] != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle[i]), "销毁信号量失败");
				hSemhandle[i] = CSUDI_NULL;
			}
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:测试信号量名称相同时能否成功创建信号量
//@PRECONDITION:
//@INPUT:1、信号量名称="OS_TestSem1"
//@INPUT:2、有效信号数量=0
//@INPUT:3、最大信号数量=1
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量OS_TestOSSem1，要求返回CSUDI_SUCCESS并输出有效的信号量句柄
//@EXECUTIONFLOW:2、再次调用CSUDIOSSemCreate创建信号量名为OS_TestOSSem1的信号量，要求返回CSUDI_SUCCESS并输出有效的信号量句柄
//@EXECUTIONFLOW:3、调用CSUDIOSSemDestroy 销毁创建的信号量，要求销毁成功并返回CSUDI_SUCCESS，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_SemCreate_0011( void )
{
	CSUDI_HANDLE hSemhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hSemhandle2 = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestOSSem1", nInitialCount, nMaxCount, &hSemhandle1), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle1,"成功创建的信号量输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestOSSem1", nInitialCount, nMaxCount, &hSemhandle2), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle2,"成功创建的信号量输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hSemhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle1), "销毁信号量失败");
			hSemhandle1 = CSUDI_NULL;
		}

		if (hSemhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle2), "销毁信号量失败");
			hSemhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSSemWait 
//@CASEGROUP:2、CSUDIOSSemRelease 
//@DESCRIPTION:测试先等待后释放信号量
//@PRECONDITION:1、可以成功创建信号量
//@PRECONDITION:2、可以成功销毁信号量
//@INPUT:正常参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSSemCreate创建信号量OS_TestOSSem1，有效数量为0，最大数量为1并成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSSemWait 等待信号量，超时时间设为100ms，应该返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:3、调用CSUDIOSThreadSleep 等待100ms，调用CSUDIOSSemRelease 释放一个信号量，要求返回成功
//@EXECUTIONFLOW:4、调用CSUDIOSSemWait 等待信号量，超时时间设为500ms ，应该等待成功
//@EXECUTIONFLOW:5、调用CSDestroySemaphore销毁创建的信号量，应该销毁成功
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0001( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出的句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSSemWait(hSemhandle,100), "步骤2失败");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,500), "步骤4失败");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSSemWait 
//@CASEGROUP:2、CSUDIOSSemRelease 
//@DESCRIPTION:测试先释放后等待信号量
//@PRECONDITION:1、可以成功创建信号量
//@PRECONDITION:2、可以成功销毁信号量
//@INPUT:正常参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSSemCreate创建信号量OS_TestOSSem1，有效数量为0，最大数量为1并成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSThreadSleep 等待100ms，调用CSUDIOSSemRelease 释放一个信号量，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSSemWait 等待信号量，超时时间设为CSUDIOS_TIMEOUT_IMMEDIATE，应该等待成功
//@EXECUTIONFLOW:4、调用CSDestroySemaphore销毁创建的信号量，应该销毁成功
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0002( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出的句柄不能为空");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "步骤3失败");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSSemWait 
//@CASEGROUP:2、CSUDIOSSemRelease 
//@DESCRIPTION:测试在等待的过程中有信号量释放，是否可以等待成功
//@PRECONDITION:1、可以成功创建任务和信号量
//@PRECONDITION:2、可以成功销毁任务和信号量
//@INPUT:正常参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSSemCreate创建信号量，有效数量为0，最大数量为1并成功返回
//@EXECUTIONFLOW:2、在主任务中调用CSUDIOSGetTime  获取当前系统时间
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate 创建新任务,在新任务中调用CSUDIOSThreadSleep 等待2000ms，调用CSUDIOSSemRelease 释放一个信号量
//@EXECUTIONFLOW:4、在主任务中调用CSUDIOSSemWait 等待信号量，超时时间设为5秒 ，一段时间后应该等待成功
//@EXECUTIONFLOW:5、在主任务中调用CSUDIOSGetTime 再次 获取当前系统时间
//@EXECUTIONFLOW:6、判断两次系统时间的差值和2000的差是否在正负20ms之间，如果是则成功，否则失败
//@EXECUTIONFLOW:7、调用CSUDIOSThreadJoin等待步骤3创建的任务退出，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSDestroySemaphore销毁创建的信号量，应该销毁成功
//@EXECUTIONFLOW:9、调用CSUDIOSThreadDestroy  销毁创建的任务，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0003( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int  nInitialCount = 0;
	int  nMaxCount = 1;
	int  nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出的句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestSemThread", 64, 8*1024, OS_TestSemThreadEntry, hSemhandle,  &hThreadhandle), "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,5000), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "步骤5失败");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 2000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-20) && (nMiliSecond <=20), "主任务没有在规定时间内等待到信号量");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hThreadhandle), "等待任务退出失败");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSSemWait 
//@CASEGROUP:2、CSUDIOSSemRelease 
//@DESCRIPTION:测试多次等待和释放的是否成功
//@PRECONDITION:1、可以成功创建信号量
//@PRECONDITION:2、可以成功销毁信号量
//@INPUT:正常参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSSemCreate创建信号量，有效数量为0，最大数量为100并成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSSemRelease连续释放信号量101次，要求前100次成功，最后一次失败
//@EXECUTIONFLOW:3、调用CSUDIOSSemWait连续等待信号量101次，超时时间都设置成10ms，要求前100次成功，最后一次超时
//@EXECUTIONFLOW:4、调用CSDestroySemaphore销毁创建的信号量，应该销毁成功
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0004( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 100;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "创建信号量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出的句柄不能为空");
	
	for(i=0 ; i<101 ; i++)
	{
		if (i != 100)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "步骤2前100次应该成功");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemRelease(hSemhandle), "步骤2最后一次应该失败");
		}
	}

	for(i=0 ; i<101 ; i++)
	{
		if (i != 100)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,10), "步骤3前100次应该成功");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemWait(hSemhandle,10), "步骤3失败最后一次应该失败");
		}
	}

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemWait
//@CASEGROUP:CSUDIOSSemRelease 
//@DESCRIPTION:测试参数为CSUDI_NULL的时候，函数的执行结果
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemWait，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSSemRelease，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0005( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemWait(CSUDI_NULL, CSUDIOS_TIMEOUT_IMMEDIATE), "步骤1失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemRelease(CSUDI_NULL), "步骤2失败");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemWait
//@DESCRIPTION:测试参数为CSUDIOS_TIMEOUT_INFINITY的时候，CSUDIOSSemWait的执行结果
//@PRECONDITION:
//@INPUT:CSUDIOS_TIMEOUT_INFINITY
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate，函数入口OS_TestSemThread1Entry中将g_bThreadEnd赋值为CSUDI_TRUE，等待10s后调用CSUDIOSSemRelease，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSSemWait，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、判断g_bThreadEnd，期望为CSUDI_TRUE
//@EXECUTIONFLOW:5、调用CSUDIOSSemDestroy，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0006( void )
{
    CSUDI_HANDLE hSemhandle = CSUDI_NULL;
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    int nInitialCount = 0;
    int nMaxCount = 100;
    int nPriority = 7;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "步骤1，创建信号量失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle, "成功创建的信号量输出的句柄不能为空");

    g_bThreadEnd = CSUDI_FALSE;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread", nPriority, TEST_THREAD_STACK_SIZE, 
                            OS_TestSemThread1Entry, hSemhandle, &hThreadhandle), "步骤2，创建任务失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"创建成功的任务输出的任务句柄不能为空");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle, CSUDIOS_TIMEOUT_INFINITY), "步骤3，请求资源失败");

    CSUDIOSThreadSleep(100);
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "步骤4失败，没有等待10s就返回");
    
    CSTK_FATAL_POINT

    if (hSemhandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "步骤5，销毁信号量失败");
        hSemhandle = CSUDI_NULL;
    }
    
    if(hThreadhandle != CSUDI_NULL)
    {           
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤6，删除任务失败");
        hThreadhandle = CSUDI_NULL;
    }
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemRelease 
//@DESCRIPTION:测试CSUDIOSSemRelease接口的执行时间
//@PRECONDITION:CSUDIOSGetTime接口没有问题
//@INPUT:
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建信号量，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSGetTime获取系统时间sTime1，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSSemRelease释放信号量，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIOSGetTime获取系统时间sTime2，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、计算执行CSUDIOSSemRelease的时间并打印值，期望用时小于5ms
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0007( void )
{
    CSUDIOSTimeVal_S sTime1;
    CSUDIOSTimeVal_S sTime2;
    int nMiliSecond = 0;
    CSUDI_HANDLE hSemhandle = CSUDI_NULL;
    int nInitialCount = 1;
    int nMaxCount = 10;
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "步骤1，创建信号量失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle, "成功创建的信号量输出的句柄不能为空");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "步骤2，第一次获取系统时间失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "步骤3失败，释放信号量失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "步骤4，第二次获取系统时间失败");

    nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) ; 

    CSTCPrint("执行CSUDIOSSemRelease消耗的时间为:%d(ms)\n", nMiliSecond);
	
    CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 5, "步骤5，等待的时间不在系统容错值内");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}


//@CASEGROUP:1、CSUDIOSSemCreate 
//@CASEGROUP:2、CSUDIOSSemWait 
//@CASEGROUP:1、CSUDIOSSemRelease 
//@CASEGROUP:2、CSDestroySemaphore 
//@DESCRIPTION:信号量强度测试
//@PRECONDITION:
//@INPUT:正常参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSSemCreate创建信号量，有效数量为0，最大数量为1并成功返回
//@EXECUTIONFLOW:2、对创建的信号量执行一次释放，一次等待操作，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSSemDestroy 销毁创建的信号量，要求销毁成功并返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:4、重复1~ 3步骤OS_TEST_COUNT次，OS_TEST_COUNT通过配置文件传入
CSUDI_BOOL CSTC_OS_IT_Sem_0001( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;
	int i;

	CSTCPrint("信号量强度测试开始，请等待。。。\n");

	for ( i=0 ; i<OS_TEST_COUNT ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "创建信号量失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"成功创建的信号量输出的句柄不能为空");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");

		hSemhandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "销毁信号量失败");
			hSemhandle = CSUDI_NULL;
		}
	}

	CSTCPrint("信号量强度测试结束！！！\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:测试合法参数输入时能否成功创建和销毁互斥量
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSMutex1"
//@INPUT:2、互斥量选项=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量OS_TestOSMutex1并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用接口CSUDIOSMutexDestroy销毁刚创建的互斥量返回成功
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0001( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:测试名称CSUDI_NULL时能否成功创建和销毁互斥量
//@PRECONDITION:
//@INPUT:1、名称=CSUDI_NULL
//@INPUT:2、互斥量选项=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建名称为空的互斥量要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用接口CSUDIOSMutexDestroy销毁刚创建的互斥量要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0002( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate(CSUDI_NULL,uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:测试互斥量选项为0时能否成功创建和销毁互斥量
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSMutex1"
//@INPUT:2、互斥量选项=0
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量OS_TestOSMutex1并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用接口CSUDIOSMutexDestroy销毁刚创建的互斥量要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0003( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:测试名称超过32字节时能否成功创建和销毁互斥量
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSMutex01234567890123456789"
//@INPUT:2、互斥量选项=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量失败并返回错误代码CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2、判断步骤1输出的互斥量句柄，如果非空则调用接口CSUDIOSMutexDestroy销毁刚创建的互斥量并返回失败，否则成功
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0004( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSMutexCreate("OS_TestOSMutex01234567890123456789",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hMutexhandle,"创建失败的互斥量输出句柄必须为空");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:测试互斥量名称相同时能否成功创建和销毁互斥量
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSMutex1"
//@INPUT:2、互斥量选项=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量OS_TestOSMutex1并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、再次调用接口CSUDIOSMutexCreate创建互斥量名为OS_TestOSMutex1的互斥量并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用接口CSUDIOSMutexDestroy销毁刚创建的互斥量返回成功
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0005( void )
{
	CSUDI_HANDLE hMutexhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hMutexhandle2 = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle1), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle1,"成功创建的互斥量输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle2), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle2,"成功创建的互斥量输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle1), "销毁互斥量失败");
			hMutexhandle1 = CSUDI_NULL;
		}

		if (hMutexhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle2), "销毁互斥量失败");
			hMutexhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate
//@CASEGROUP:2.CSUDIOSMutexDestroy
//@DESCRIPTION:测试互斥量的内存泄露情况
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSMutex1"
//@INPUT:2、互斥量选项=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMutexCreate创建互斥量OS_TestOSMutex1并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMutexDestroy销毁信号量,预期返回成功
//@EXECUTIONFLOW:3、重复以上1~2步骤OS_TEST_COUNT次，OS_TEST_COUNT通过配置文件传入
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0006( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;
	int i;

	CSTCPrint("互斥量强度测试开始，请等待。。。\n");

	for (i=0 ; i<OS_TEST_COUNT ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");

		hMutexhandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
	}

	CSTCPrint("互斥量强度测试结束！！！\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMutexWait  
//@CASEGROUP:2、CSUDIOSMutexRelease  
//@DESCRIPTION:测试互斥量选项=CSUDIOS_MUTEX_OBTAIN时先等待后释放互斥量的情况
//@PRECONDITION:1、可以成功创建互斥量
//@PRECONDITION:2、可以成功销毁互斥量
//@INPUT:合法参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量，名称为OS_TestOSMutex1，互斥量选项=CSUDIOS_MUTEX_OBTAIN，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate创建新任务，在新任务中调用CSUDIOSMutexWait 等待互斥量，超时时间为2秒，  如果等到则将g_bThreadEnd由false改为true
//@EXECUTIONFLOW:3、主任务等待100ms后判断g_bThreadEnd的值，要求为false，否则测试失败
//@EXECUTIONFLOW:4、主任务调用CSUDIOSMutexRelease 释放一个互斥量
//@EXECUTIONFLOW:5、主任务等待100ms后再次判断g_bThreadEnd的值，要求为true，否则测试失败
//@EXECUTIONFLOW:6、调用CSUDIOSMutexDestroy销毁创建的互斥量，要求销毁成功
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0001( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread1", 64, 8*1024, OS_TestMutexThread1Entry, hMutexhandle, &hThreadhandle), "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"成功创建的任务输出句柄不能为空");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(g_bThreadEnd == CSUDI_FALSE,"第三步失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "释放互斥量失败");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(g_bThreadEnd == CSUDI_TRUE,"第五步失败");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMutexWait  
//@CASEGROUP:2、CSUDIOSMutexRelease  
//@DESCRIPTION:测试互斥量选项=0时先等待后释放互斥量的情况
//@PRECONDITION:1、可以成功创建互斥量
//@PRECONDITION:2、可以成功销毁互斥量
//@INPUT:合法参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量，名称为OS_TestOSMutex1，互斥量选项=0，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMutexWait 等待互斥量，超时时间设为100 ，应该返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSMutexRelease 释放一个互斥量
//@EXECUTIONFLOW:4、调用CSUDIOSMutexWait 等待互斥量，超时时间设为100 ，应该返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIOSMutexDestroy销毁创建的互斥量，要求销毁成功
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0002( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "释放互斥量失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "步骤4失败");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMutexWait  
//@CASEGROUP:2、CSUDIOSMutexRelease  
//@DESCRIPTION:测试互斥量能否正常工作
//@PRECONDITION:1、可以成功创建任务和互斥量
//@PRECONDITION:2、可以成功销毁任务和互斥量
//@INPUT:合法参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量，名称为OS_TestOSMutex1，互斥量选项=0，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate 创建三个新任务，要求全部创建成功
//@EXECUTIONFLOW:3、分别在三个新任务中调用CSUDIOSThreadSleep等待1、2、3秒，随后调用CSUDIOSMutexWait等待互斥量OS_TestOSMutex1
//@EXECUTIONFLOW:4、锁定成功后，分别在三个新任务中调用CSUDIOSThreadSleep等待6、4、2秒，并改变g_nTestData的值，然后释放互斥量并自行返回
//@EXECUTIONFLOW:5、在主任务中通过判断g_nTestData的值是否跟预期相等来判断三个新任务执行完毕的先后顺序
//@EXECUTIONFLOW:6、第一次等待1秒的任务应该最先执行完，等待3秒的最后，如果执行顺序与此不付，则测试用例失败
//@EXECUTIONFLOW:7、调用CSUDIOSMutexDestroy销毁创建的互斥量，要求销毁成功
//@EXECUTIONFLOW:8、调用CSUDIOSThreadDestroy  销毁创建的任务，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0003( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;
	unsigned int uOptions = 0;

	g_nTestData = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread2_1", 64, 8*1024, OS_TestMutexThread2_1Entry, hMutexhandle, &hThreadhandle1), "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"成功创建的任务输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread2_2", 64, 8*1024, OS_TestMutexThread2_2Entry, hMutexhandle, &hThreadhandle2), "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"成功创建的任务输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread2_3", 64, 8*1024, OS_TestMutexThread2_3Entry, hMutexhandle, &hThreadhandle3), "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"成功创建的任务输出句柄不能为空");

	CSTCPrint("\n请等待15秒!!!!\n");

	while(g_nTestData == 0)
	{
		CSUDIOSThreadSleep(10);
	}

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"执行顺序与预期不符");

	while(g_nTestData == 1)
	{
		CSUDIOSThreadSleep(10);
	}

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 2,"执行顺序与预期不符");

	while(g_nTestData == 2)
	{
		CSUDIOSThreadSleep(10);
	}

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 3,"执行顺序与预期不符");
	
	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
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

		if (hThreadhandle3 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "删除任务失败");
			hThreadhandle3 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMutexWait  
//@CASEGROUP:2、CSUDIOSMutexRelease  
//@DESCRIPTION:测试对已经解锁的互斥量进行解锁操作以及两个任务之间操作互斥量
//@PRECONDITION:1、可以成功创建任务和互斥量
//@PRECONDITION:2、可以成功销毁任务和互斥量
//@INPUT:合法参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量，名称为OS_TestOSMutex1，互斥量选项=0，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMutexWait 等待互斥量，超时时间设为100 ，应该返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSMutexRelease 释放互斥量，应该返回成功
//@EXECUTIONFLOW:4、再次调用CSUDIOSMutexRelease 释放互斥量，应该返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:5、调用CSUDIOSMutexWait 等待互斥量，超时时间设为100 ，应该返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIOSThreadCreate 创建新任务并在新任务中调用CSUDIOSMutexWait等待互斥量，超时时间为1500ms，若等到则将g_nTestData从0改为1，否则将g_nTestData从0改为2
//@EXECUTIONFLOW:7、主任务调用CSUDIOSThreadSleep等待500ms后判断g_nTestData的值，如果不为0，则说明新任务在主任务已经加锁的情况下获得了互斥量，测试用例失败
//@EXECUTIONFLOW:8、主任务调用CSUDIOSThreadSleep等待2秒后判断g_nTestData的值，如果是2则说明新任务等待互斥量失败后返回，并运行完毕，否则测试用例失败
//@EXECUTIONFLOW:9、主任务调用CSUDIOSMutexRelease 释放互斥量，应该返回成功
//@EXECUTIONFLOW:10、调用CSUDIOSMutexDestroy销毁创建的互斥量，要求销毁成功
//@EXECUTIONFLOW:11、调用CSUDIOSThreadDestroy  销毁创建的任务，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0004( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uOptions = 0;

	g_nTestData = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMutexRelease(hMutexhandle), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread3", 64, 8*1024, OS_TestMutexThread3Entry, hMutexhandle, &hThreadhandle), "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"成功创建的任务输出句柄不能为空");

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 0,"步骤7失败");

	CSUDIOSThreadSleep(2000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 2,"步骤8失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "释放互斥量失败");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMutexWait  
//@CASEGROUP:2、CSUDIOSMutexRelease  
//@DESCRIPTION:测试互斥量的嵌套调用
//@PRECONDITION:1、可以成功创建任务和互斥量
//@PRECONDITION:2、可以成功销毁任务和互斥量
//@INPUT:合法参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量，名称为OS_TestOSMutex1，互斥量选项=0，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、重复调用CSUDIOSMutexWait 等待互斥量10次，超时时间设为CSUDIOS_TIMEOUT_IMMEDIATE  ，应该全部返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate 创建新任务并在新任务中调用CSUDIOSMutexWait等待互斥量，超时时间为2秒 ，若等到则将g_bThreadEnd从false改为true，否则不操作
//@EXECUTIONFLOW:4、主任务调用CSUDIOSMutexRelease 释放互斥量9次，应该返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、主任务调用CSUDIOSThreadSleep等待500ms后判断g_bThreadEnd的值，如果为false，则说明新任务依然没有等到互斥量，否则测试用例失败
//@EXECUTIONFLOW:6、主任务调用CSUDIOSMutexRelease 释放互斥量，应该返回成功
//@EXECUTIONFLOW:7、主任务调用CSUDIOSThreadSleep等待500ms后判断g_bThreadEnd的值，如果为true，则说明主任务在进行10次加锁后调用10次解锁成功，新任务可以等到互斥量并运行结束，否则测试用例失败
//@EXECUTIONFLOW:8、调用CSUDIOSMutexDestroy销毁创建的互斥量，要求销毁成功
//@EXECUTIONFLOW:9、调用CSUDIOSThreadDestroy  销毁创建的任务，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0005( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uOptions = 0;
	int i;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");

	for (i=0 ; i<10 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "步骤2失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread1", 64, 8*1024, OS_TestMutexThread1Entry, hMutexhandle, &hThreadhandle), "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"成功创建的任务输出句柄不能为空");

	for (i=0 ; i<9 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "步骤4失败");
	}

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "步骤6失败");

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤7失败");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMutexWait
//@CASEGROUP:2、CSUDIOSMutexRelease  
//@DESCRIPTION:测试一个任务对互斥量进行加锁后，另一个任务对其进行加锁和解锁的情况
//@PRECONDITION:1、可以成功创建任务和互斥量
//@PRECONDITION:2、可以成功销毁任务和互斥量
//@INPUT:合法参数输入
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSMutexCreate创建互斥量，名称为OS_TestOSMutex1，互斥量选项=0，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate 创建新任务并在新任务中调用CSUDIOSMutexWait等待互斥量，超时时间为20ms ，若等到则将g_bThreadEnd从false改为true，否则不操作
//@EXECUTIONFLOW:3、新任务改变g_bThreadEnd的值后调用CSUDIOSThreadSleep等待1秒，再调用CSUDIOSMutexRelease解锁
//@EXECUTIONFLOW:4、主任务调用CSUDIOSThreadSleep等待100ms后判断g_bThreadEnd的值，应该为true，否则失败
//@EXECUTIONFLOW:5、主任务调用CSUDIOSMutexWait加锁，超时时间为100ms，应该返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:6、主任务调用CSUDIOSMutexRelease解锁，应该返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、主任务调用CSUDIOSThreadSleep等待1秒后再调用CSUDIOSMutexWait加锁，应该返回成功
//@EXECUTIONFLOW:8、主任务调用CSUDIOSMutexRelease解锁，应该返回成功
//@EXECUTIONFLOW:9、调用CSUDIOSMutexDestroy销毁创建的互斥量，要求销毁成功
//@EXECUTIONFLOW:10、调用CSUDIOSThreadDestroy  销毁创建的任务，要求返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0006( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uOptions = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread4", 64, 8*1024, OS_TestMutexThread4Entry, hMutexhandle, &hThreadhandle), "创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"成功创建的任务输出句柄不能为空");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMutexWait(hMutexhandle,100), "步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMutexRelease(hMutexhandle), "步骤6失败");

	CSUDIOSThreadSleep(1500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "步骤7失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "步骤8失败");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMutexWait
//@CASEGROUP:CSUDIOSMutexRelease 
//@DESCRIPTION:测试参数为CSUDI_NULL的时候，函数的执行结果
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMutexWait，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSMutexRelease，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0007( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMutexWait(CSUDI_NULL, CSUDIOS_TIMEOUT_IMMEDIATE), "步骤1失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMutexRelease(CSUDI_NULL), "步骤2失败");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMutexWait
//@DESCRIPTION:测试超时时间参数为CSUDIOS_TIMEOUT_INFINITY时，CSUDIOSMutexWait的执行结果
//@PRECONDITION:
//@INPUT:CSUDIOS_TIMEOUT_INFINITY
//@EXPECTATION:等待10秒后返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSMutexCreate，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate 创建新任务并在新任务中调用CSUDIOSMutexWait后再等待10s，接着调用CSUDIOSMutexRelease，并将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、调用CSUDIOSMutexWait，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、判断g_bThreadEnd的值，期望为CSUDI_TRUE
//@EXECUTIONFLOW:5、调用CSUDIOSMutexDestroy，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0008( void )
{
    CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    unsigned int uOptions = 0;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1", uOptions, &hMutexhandle), "步骤1失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");
    
    g_bThreadEnd = CSUDI_FALSE;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread5", 64, 8*1024, OS_TestMutexThread5Entry, hMutexhandle, &hThreadhandle), "步骤2，创建任务失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"成功创建的任务输出句柄不能为空");
    
    CSUDIOSThreadSleep(100);//让线程中的CSUDIOSMutexWait先调用
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle, CSUDIOS_TIMEOUT_INFINITY), "步骤3，加锁操作失败");

    CSUDIOSThreadSleep(100);//等待线程先结束

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "步骤4失败，没有等待10s就返回");
    
    CSTK_FATAL_POINT
    {
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "销毁互斥量失败");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "删除任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMutexRelease 
//@DESCRIPTION:测试CSUDIOSMutexRelease接口的执行时间
//@PRECONDITION:CSUDIOSGetTime接口没有问题
//@INPUT:
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSSemCreate创建互斥量，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSGetTime获取系统时间sTime1，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSMutexRelease释放互斥量，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIOSGetTime获取系统时间sTime2，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、计算执行CSUDIOSMutexRelease的时间并打印值，期望用时小于5ms
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0009( void )
{
    CSUDIOSTimeVal_S sTime1;
    CSUDIOSTimeVal_S sTime2;
    CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
    unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;
    int nMiliSecond = 0;
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1", uOptions, &hMutexhandle), "步骤1失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"成功创建的互斥量输出句柄不能为空");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "步骤2，第一次获取系统时间失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "步骤3失败，释放互斥量失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "步骤4，第二次获取系统时间失败");

    nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) ; 
	
    CSTCPrint("执行CSUDIOSSemRelease消耗的时间为:%d(ms)\n", nMiliSecond);

    CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 5, "步骤5，等待的时间不在系统容错值内");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetTime    
//@DESCRIPTION:测试获取自系统启动后经过的时间的准确性
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:两次获取时间的差值和Sleep的时间相减后容错值在每秒正负10ms
//@EXECUTIONFLOW:1、调用CSUDIOSGetTime获取自系统启动后经过的时间，应该返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSThreadSleep 等待1秒，应该返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSGetTime再次获取自系统启动后经过的时间，应该返回成功
//@EXECUTIONFLOW:4、判断两次获取系统时间的差值和等待时间的差，如果容错在正负10ms则返回成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_GetTime_0001( void )
{
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "第一次获取系统时间失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "第二次获取系统时间失败");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 1000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-10) && (nMiliSecond <=10), "等待的时间不在系统容错值内");
	
	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetTime    
//@DESCRIPTION:测试获取自系统启动后经过的时间的准确性
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:两次获取时间的差值和Sleep的时间相减后容错值在每秒正负10ms
//@EXECUTIONFLOW:1、调用CSUDIOSGetTime获取自系统启动后经过的时间，应该返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSThreadSleep 等待5秒，应该返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSGetTime再次获取自系统启动后经过的时间，应该返回成功
//@EXECUTIONFLOW:4、判断两次获取系统时间的差值和等待时间的差，如果容错在正负50ms则返回成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_GetTime_0002( void )
{
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;

	CSTCPrint("\n请等待5秒。。。\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "第一次获取系统时间失败");

	CSUDIOSThreadSleep(5000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "第二次获取系统时间失败");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 5000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-50) && (nMiliSecond <=50), "等待的时间不在系统容错值内");
	
	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetTime    
//@DESCRIPTION:测试获取自系统启动后经过的时间的准确性
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:两次获取时间的差值和Sleep的时间相减后容错值在每秒正负10ms
//@EXECUTIONFLOW:1、调用CSUDIOSGetTime获取自系统启动后经过的时间，应该返回成功
//@EXECUTIONFLOW:2、调用CSUDIOSThreadSleep 等待10秒，应该返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSGetTime再次获取自系统启动后经过的时间，应该返回成功
//@EXECUTIONFLOW:4、判断两次获取系统时间的差值和等待时间的差，如果容错在正负100ms则返回成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_GetTime_0003( void )
{
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;

	CSTCPrint("\n请等待10秒。。。\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "第一次获取系统时间失败");

	CSUDIOSThreadSleep(10000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "第二次获取系统时间失败");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 10000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-100) && (nMiliSecond <=100), "等待的时间不在系统容错值内");
	
	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetTime    
//@DESCRIPTION:测试该接口参数为CSUDI_NULL时能否获取系统时间
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:获取时间失败返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSGetTime，参数使用CSUDI_NULL，获取自系统启动后经过的时间，应该返回失败
CSUDI_BOOL CSTC_OS_IT_GetTime_0004( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetTime(CSUDI_NULL), "第一步失败");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventCreate   
//@CASEGROUP:2、CSUDIOSEventDestroy   
//@DESCRIPTION:测试正常参数输入时能否创建和销毁事件
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSEvent1"
//@INPUT:2、事件选项=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件OS_TestOSEvent1，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSEventDestroy 销毁创建的事件返回成功
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0001( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventCreate   
//@CASEGROUP:2、CSUDIOSEventDestroy   
//@DESCRIPTION:测试事件名CSUDI_NULL时能否成功创建和销毁事件
//@PRECONDITION:
//@INPUT:1、名称=CSUDI_NULL
//@INPUT:2、事件选项=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件名为空的时间，应该返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSEventDestroy 销毁创建的事件返回成功
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0002( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate(CSUDI_NULL,uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventCreate   
//@CASEGROUP:2、CSUDIOSEventDestroy   
//@DESCRIPTION:测试事件选项为CSUDIOS_EVENT_AUTO_RESET时能否成功创建和销毁事件
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSEvent1"
//@INPUT:2、事件选项=CSUDIOS_EVENT_AUTO_RESET
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件OS_TestOSEvent1，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSEventDestroy 销毁创建的事件返回成功
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0003( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_AUTO_RESET;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventCreate   
//@CASEGROUP:2、CSUDIOSEventDestroy   
//@DESCRIPTION:测试两个事件选项标志同时使用时能否成功创建和销毁事件
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSEvent1"
//@INPUT:2、事件选项=CSUDIOS_EVENT_INITIAL_STATUS|CSUDIOS_EVENT_AUTO_RESET
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件OS_TestOSEvent1，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSEventDestroy 销毁创建的事件返回成功
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0004( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS|CSUDIOS_EVENT_AUTO_RESET;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventCreate   
//@CASEGROUP:2、CSUDIOSEventDestroy   
//@DESCRIPTION:测试事件选项为0时能否成功创建和销毁事件
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSEvent1"
//@INPUT:2、事件选项=0
//@EXPECTATION:创建成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件OS_TestOSEvent1，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSEventDestroy 销毁创建的事件并返回成功，否则测试用例失败
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0005( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"创建成功的事件输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventCreate   
//@CASEGROUP:2、CSUDIOSEventDestroy   
//@DESCRIPTION:测试事件名称长度超过32字节时能否成功创建和销毁事件
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSEvent01234567890123456789"
//@INPUT:2、事件选项=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:创建失败并返回CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件OS_TestOSEvent01234567890123456789，要求返回CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2、判断输出的事件句柄，如果非空则调用CSUDIOSEventDestroy 销毁创建的事件并返回测试用例失败，否则返回成功
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0006( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSEventCreate("OS_TestOSEvent01234567890123456789",uFlags,&hEventhandle), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hEventhandle,"创建失败的事件输出句柄必须为空");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventCreate   
//@CASEGROUP:2、CSUDIOSEventDestroy   
//@DESCRIPTION:测试事件名称相同时能否创建和销毁事件
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSEvent1"
//@INPUT:2、事件选项=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:创建成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件OS_TestOSEvent1，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、再次调用CSUDIOSEventCreate创建事件名为OS_TestOSEvent1的事件，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSEventDestroy 销毁创建的事件返回成功
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0007( void )
{
	CSUDI_HANDLE hEventhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hEventhandle2 = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle1), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle1,"成功创建的事件输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle2), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle2,"成功创建的事件输出句柄不能为空");

	CSTK_FATAL_POINT
	{
		if(hEventhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle1), "销毁事件失败");
			hEventhandle1 = CSUDI_NULL;
		}

		if(hEventhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle2), "销毁事件失败");
			hEventhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSEventCreate
//@CASEGROUP:2.CSUDIOSEventDestroy
//@DESCRIPTION:事件创建和销毁的强度测试
//@PRECONDITION:
//@INPUT:1、名称="OS_TestOSEvent1"
//@INPUT:2、事件选项=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件OS_TestOSEvent1，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSEventDestroy 销毁创建的事件返回成功
//@EXECUTIONFLOW:3、重复以上1~2步骤OS_TEST_COUNT次，OS_TEST_COUNT通过配置文件传入
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0008( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;
	int i;

	CSTCPrint("事件强度测试开始，请等待。。。\n");

	for(i=0 ; i<OS_TEST_COUNT ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");

		hEventhandle = CSUDI_NULL;

	}
	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}
	}

	CSTCPrint("事件强度测试结束！！！\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventWait 
//@CASEGROUP:2、CSUDIOSEventReset 
//@CASEGROUP:3、CSUDIOSEventSet 
//@DESCRIPTION:1、测试当事件标志参数为CSUDIOS_EVENT_INITIAL_STATUS时先将事件设置成无效再等待的情况
//@DESCRIPTION:2、测试等待过程中将事件设置成有效后能否等到事件
//@PRECONDITION:1、可以成功创建事件和任务
//@PRECONDITION:2、可以成功销毁事件和任务
//@INPUT:正常参数
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSEventCreate创建事件，名称为"OS_TestOSEvent1"，标志参数值为CSUDIOS_EVENT_INITIAL_STATUS，可以成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSEventReset将事件置为无效
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate 创建一个新任务
//@EXECUTIONFLOW:4、新任务中调用CSUDIOSEventWait等待事件,超时时间为1500ms ，应该等不到事件而阻塞，如等到则将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:5、主任务调用CSUDIOSThreadSleep 等待1秒钟后,判断g_bThreadEnd是否为false,若不等于false则测试失败
//@EXECUTIONFLOW:6、主任务调用CSUDIOSEventSet将事件置为有效
//@EXECUTIONFLOW:7、此时新任务应该等到事件,将g_bThreadEnd置为true
//@EXECUTIONFLOW:8、主任务判断g_bThreadEnd是否为true,不等于true则测试失败
//@EXECUTIONFLOW:9、主任务调用CSUDIOSEventDestroy销毁创建的事件返回成功
//@EXECUTIONFLOW:10、主任务调用CSUDIOSThreadDestroy 销毁创建的任务返回成功
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0001( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventReset(hEventhandle), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread1", 64, 8*1024, OS_TestEventThread1Entry, hEventhandle, &hThreadhandle), "任务创建失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "步骤6失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤8失败");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "销毁任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventWait 
//@CASEGROUP:2、CSUDIOSEventReset 
//@CASEGROUP:3、CSUDIOSEventSet 
//@DESCRIPTION:测试当事件标志参数为CSUDIOS_EVENT_INITIAL_STATUS时等待事件的超时和错误返回情况
//@PRECONDITION:1、可以成功创建事件
//@PRECONDITION:2、可以成功销毁事件
//@INPUT:正常参数
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSEventCreate创建事件，名称为"OS_TestOSEvent1"，标志参数值为CSUDIOS_EVENT_INITIAL_STATUS，可以成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSEventReset将事件置为无效
//@EXECUTIONFLOW:3、调用CSUDIOSEventWait等待事件,超时时间为100毫秒,应该超时返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:4、调用CSUDIOSEventWait等待事件,超时时间为500毫秒,应该超时返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:5、调用CSUDIOSEventWait等待事件,超时时间为CSUDIOS_TIMEOUT_IMMEDIATE ,应该超时返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:6、调用CSUDIOSEventSet将事件置为有效
//@EXECUTIONFLOW:7、调用CSUDIOSEventWait等待事件,超时时间为100毫秒,应该成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIOSEventDestroy销毁创建的事件，要求返回成功
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0002( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventReset(hEventhandle), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,100), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,500), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle,100), "步骤7失败");
	
	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventWait 
//@CASEGROUP:2、CSUDIOSEventReset 
//@CASEGROUP:3、CSUDIOSEventSet 
//@DESCRIPTION:1、测试事件以标志参数CSUDIOS_EVENT_INITIAL_STATUS创建后立即等待是否成功
//@DESCRIPTION:2、测试多个任务等待同一事件是否成功
//@PRECONDITION:1、可以成功创建事件和任务
//@PRECONDITION:2、可以成功销毁事件和任务
//@INPUT:正常参数
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSEventCreate创建事件，名称为"OS_TestOSEvent1"，标志参数值为CSUDIOS_EVENT_INITIAL_STATUS，可以成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSEventWait等待事件，超时时间为CSUDIOS_TIMEOUT_IMMEDIATE，应该等待成功
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate 创建一个新任务并在新任务中调用CSUDIOSEventWait等待事件，超时时间为1秒，应该返回成功
//@EXECUTIONFLOW:4、在新任务中将g_nTestData从0改成1，之后调用CSUDIOSThreadSleep等待2秒
//@EXECUTIONFLOW:5、调用CSUDIOSThreadSleep 等待1秒钟
//@EXECUTIONFLOW:6、主任务判断g_nTestData是否为1,若不等于1则测试失败
//@EXECUTIONFLOW:7、主任务调用CSUDIOSEventReset将事件置为无效
//@EXECUTIONFLOW:8、在新任务中调用CSUDIOSEventWait等待事件，超时时间为3秒，应该等不到事件
//@EXECUTIONFLOW:9、主任务调用CSUDIOSThreadSleep等待2秒后判断g_nTestData是否等于1，如果不为1则测试用例失败
//@EXECUTIONFLOW:10、主任务调用CSUDIOSEventSet将事件置为有效
//@EXECUTIONFLOW:11、新任务此时应该可以等到事件，并将g_nTestData从1改成2
//@EXECUTIONFLOW:12、主任务判断g_nTestData是否为2,若不等于2则测试失败
//@EXECUTIONFLOW:13、主任务调用CSUDIOSEventDestroy销毁创建的事件返回成功
//@EXECUTIONFLOW:14、主任务调用CSUDIOSThreadDestroy 销毁创建的任务返回成功
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0003( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	g_nTestData = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread2",64,8*1024,OS_TestEventThread2Entry,hEventhandle,&hThreadhandle), "任务创建失败");

	CSTCPrint("\n请等待5秒。。。\n");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventReset(hEventhandle), "步骤7失败");

	CSUDIOSThreadSleep(2000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"步骤9失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "步骤10失败");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 2,"步骤12失败");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "销毁任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventWait 
//@CASEGROUP:2、CSUDIOSEventReset 
//@CASEGROUP:3、CSUDIOSEventSet 
//@DESCRIPTION:1、测试事件以标志参数CSUDIOS_EVENT_AUTO_RESET创建后立即等待是否成功
//@DESCRIPTION:2、事件等待，设置有效和无效的综合测试
//@PRECONDITION:1、可以成功创建事件和任务
//@PRECONDITION:2、可以成功销毁事件和任务
//@INPUT:正常参数
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSEventCreate创建事件，名称为"OS_TestOSEvent1"，标志参数值为CSUDIOS_EVENT_AUTO_RESET ，可以成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSEventWait等待事件，超时时间为100ms，应该返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate 创建一个新任务并在新任务中调用CSUDIOSEventWait等待事件，超时时间为1秒 ，应该等不到事件
//@EXECUTIONFLOW:4、新任务如果等到事件成功则将g_nTestData从0改成1，之后调用CSUDIOSThreadSleep等待100ms
//@EXECUTIONFLOW:5、主任务等待500ms后判断g_nTestData是否为0,若不等于0则测试失败
//@EXECUTIONFLOW:6、主任务调用CSUDIOSEventSet将事件置为有效之后调用CSUDIOSThreadSleep等待1秒
//@EXECUTIONFLOW:7、主任务判断g_nTestData是否为1,若不等于1则测试失败
//@EXECUTIONFLOW:8、在新任务中调用CSUDIOSEventWait等待事件，超时时间为2秒，应该等不到事件
//@EXECUTIONFLOW:9、新任务如果等待事件成功则将g_nTestData从1改成2，然后自行返回
//@EXECUTIONFLOW:10、在主任务中调用CSUDIOSEventWait等待事件，超时时间为100ms，应该返回CSUDIOS_ERROR_TIMEOUT，否则测试用例失败
//@EXECUTIONFLOW:11、主任务判断g_nTestData是否为1,若不等于1则测试失败
//@EXECUTIONFLOW:12、主任务调用CSUDIOSEventSet将事件置为有效之后调用CSUDIOSThreadSleep等待2秒
//@EXECUTIONFLOW:13、新任务此时应该可以等到事件，并将g_dwTestEventData从1改成2
//@EXECUTIONFLOW:14、主任务判断g_nTestData是否为2,若不等于2则测试失败
//@EXECUTIONFLOW:15、主任务调用CSUDIOSEventDestroy销毁创建的事件返回成功
//@EXECUTIONFLOW:16、主任务调用CSUDIOSThreadDestroy 销毁创建的任务返回成功
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0004( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_AUTO_RESET;

	g_nTestData = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,100), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread3",64,8*1024,OS_TestEventThread3Entry,hEventhandle,&hThreadhandle), "任务创建失败");

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 0,"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "步骤6失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"步骤7失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,100), "步骤10失败");

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"步骤11失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "步骤12失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 2,"步骤14失败");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "销毁任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventWait 
//@CASEGROUP:2、CSUDIOSEventReset 
//@CASEGROUP:3、CSUDIOSEventSet 
//@DESCRIPTION:测试创建的事件标志参数为CSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUS时等待、设置有效和无效的情况	
//@PRECONDITION:1、可以成功创建事件和任务
//@PRECONDITION:2、可以成功销毁事件和任务
//@INPUT:正常参数
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSEventCreate创建事件，名称为"OS_TestOSEvent1"，标志参数值为CSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUS ，可以成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSEventWait等待事件，超时时间为CSUDIOS_TIMEOUT_IMMEDIATE，应该等待成功
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate 创建一个新任务并在新任务中调用CSUDIOSEventWait等待事件，超时时间为1500ms ，应该等不到事件
//@EXECUTIONFLOW:4、新任务如果等到事件成功则将g_bThreadEnd从false改成true
//@EXECUTIONFLOW:5、主任务判断g_bThreadEnd是否为false,若不等于false则测试失败
//@EXECUTIONFLOW:6、主任务调用CSUDIOSEventSet将事件置为有效之后调用CSUDIOSThreadSleep等待1秒
//@EXECUTIONFLOW:7、主任务判断g_bThreadEnd是否为true,若不等于true则测试失败
//@EXECUTIONFLOW:8、在主任务中调用CSUDIOSEventWait等待事件，超时时间为100ms，应该返回CSUDIOS_ERROR_TIMEOUT，否则测试用例失败
//@EXECUTIONFLOW:9、主任务调用CSUDIOSEventDestroy销毁创建的事件返回成功
//@EXECUTIONFLOW:10、主任务调用CSUDIOSThreadDestroy 销毁创建的任务返回成功
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0005( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_AUTO_RESET|CSUDIOS_EVENT_INITIAL_STATUS;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread1",64,8*1024,OS_TestEventThread1Entry,hEventhandle,&hThreadhandle), "任务创建失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "步骤6失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤7失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,100), "步骤8失败");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "销毁任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSEventWait 
//@CASEGROUP:2、CSUDIOSEventReset 
//@CASEGROUP:3、CSUDIOSEventSet 
//@DESCRIPTION:测试创建的事件标志参数为 0 时等待、设置有效和无效的情况	
//@PRECONDITION:1、可以成功创建事件和任务
//@PRECONDITION:2、可以成功销毁事件和任务
//@INPUT:正常参数
//@EXPECTATION:每一步骤都输出预期结果
//@EXECUTIONFLOW:1、调用接口CSUDIOSEventCreate创建事件，名称为"OS_TestOSEvent1"，标志参数值为CSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUS ，可以成功返回
//@EXECUTIONFLOW:2、调用CSUDIOSEventWait等待事件，超时时间为CSUDIOS_TIMEOUT_IMMEDIATE，应该返回CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate 创建一个新任务并在新任务中调用CSUDIOSEventWait等待事件，超时时间为1500ms ，应该等不到事件
//@EXECUTIONFLOW:4、新任务如果等到事件成功则将g_bThreadEnd从false改成true
//@EXECUTIONFLOW:5、主任务判断g_bThreadEnd是否为false,若不等于false则测试失败
//@EXECUTIONFLOW:6、主任务调用CSUDIOSEventSet将事件置为有效之后调用CSUDIOSThreadSleep等待1秒
//@EXECUTIONFLOW:7、主任务判断g_bThreadEnd是否为true,若不等于true则测试失败
//@EXECUTIONFLOW:8、在主任务中调用CSUDIOSEventWait等待事件，超时时间为100ms，应该返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:9、主任务调用CSUDIOSEventDestroy销毁创建的事件返回成功
//@EXECUTIONFLOW:10、主任务调用CSUDIOSThreadDestroy 销毁创建的任务返回成功
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0006( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "创建事件失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread1",64,8*1024,OS_TestEventThread1Entry,hEventhandle,&hThreadhandle), "任务创建失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "步骤6失败");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"步骤7失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle,100), "步骤8失败");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "销毁事件失败");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "销毁任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSEventWait
//@CASEGROUP:CSUDIOSEventReset 
//@CASEGROUP:CSUDIOSEventSet 
//@DESCRIPTION:测试参数为CSUDI_NULL的时候，函数的执行结果
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventWait，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSEventReset，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSEventSet，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0007( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventWait(CSUDI_NULL, CSUDIOS_TIMEOUT_IMMEDIATE), "步骤1失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventReset(CSUDI_NULL), "步骤2失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventSet(CSUDI_NULL), "步骤3失败");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSEventWait
//@CASEGROUP:CSUDIOSEventReset 
//@CASEGROUP:CSUDIOSEventSet 
//@DESCRIPTION:测试参数为CSUDI_NULL的时候，函数的执行结果
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventWait，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSEventReset，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSEventSet，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0008( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventWait(CSUDI_NULL, CSUDIOS_TIMEOUT_IMMEDIATE), "步骤1失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventReset(CSUDI_NULL), "步骤2失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventSet(CSUDI_NULL), "步骤3失败");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSEventWait
//@DESCRIPTION:测试参数为CSUDIOS_TIMEOUT_INFINITY的时候，CSUDIOSEventWait的执行结果
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:返回每步的期望结果
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate创建新任务并在新任务中等待10s后再调用CSUDIOSEventSet，并将g_bThreadEnd从false改为true
//@EXECUTIONFLOW:3、调用CSUDIOSEventWait，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、判断g_bThreadEnd的值，期望为CSUDI_TRUE
//@EXECUTIONFLOW:5、调用CSUDIOSEventDestroy，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0009( void )
{
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    CSUDI_HANDLE hEventhandle = CSUDI_NULL;
    unsigned int uFlags = CSUDIOS_EVENT_AUTO_RESET;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1", uFlags, &hEventhandle), "步骤1，创建事件失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

    g_bThreadEnd = CSUDI_FALSE;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread4", 64, 8*1024, OS_TestEventThread4Entry, hEventhandle, &hThreadhandle), "步骤2，创建任务失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"成功创建的任务输出句柄不能为空");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle, CSUDIOS_TIMEOUT_INFINITY), "步骤3，等待事件失败");

    CSUDIOSThreadSleep(100);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "步骤4失败，没有等待10s就返回");

    CSTK_FATAL_POINT
    
    if(hEventhandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "步骤5，销毁事件失败");
        hEventhandle = CSUDI_NULL;
    }
    
    if(hThreadhandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤6，删除任务失败");
        hThreadhandle = CSUDI_NULL;
    }
    
    return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIOSEventSet 
//@DESCRIPTION:测试CSUDIOSEventSet接口的执行时间
//@PRECONDITION:CSUDIOSGetTime接口没有问题
//@INPUT:
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDIOSEventCreate创建事件，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、调用CSUDIOSGetTime获取系统时间sTime1，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIOSEventSet设置事件有效，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIOSGetTime获取系统时间sTime2，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、计算执行CSUDIOSEventSet的时间并打印值，期望用时小于5ms
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0010( void )
{
    CSUDI_HANDLE hEventhandle = CSUDI_NULL;
    unsigned int uFlags = 0;
    CSUDIOSTimeVal_S sTime1;
    CSUDIOSTimeVal_S sTime2;
    int nMiliSecond = 0;
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1", uFlags, &hEventhandle), "步骤1，创建事件失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"成功创建的事件输出句柄不能为空");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "步骤2，第一次获取系统时间失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "步骤3，设置事件有效失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "步骤4，第二次获取系统时间失败");

    nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) ; 

    CSTCPrint("执行CSUDIOSSemRelease消耗的时间为:%d(ms)\n", nMiliSecond);

    CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 5, "步骤5，等待的时间不在系统容错值内");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMalloc 
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:测试1000个字节内存的分配和释放是否成功
//@PRECONDITION:
//@INPUT:分配的内存块大小=1000
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMalloc申请1000字节的内存成功并返回内存首地址
//@EXECUTIONFLOW:2、往内存中存入250个4字节的数，数字分别为0,1…99，循环往复可以存入成功
//@EXECUTIONFLOW:3、从该内存中读出相应的数据并进行校验，应该全部校验成功，如有不同则本测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSFree释放申请的内存并成功返回
CSUDI_BOOL CSTC_OS_IT_MallocFree_0001( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uSize = 1000;
	int i;

	puChunk = (unsigned int*)CSUDIOSMalloc(uSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"申请内存失败");

	for(i=0 ; i<250 ; i++)
	{
		puChunk[i] = i%100;
	}

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk[i] == (i%100), "校验不成功");
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMalloc 
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:测试8M内存的分配和释放是否成功
//@PRECONDITION:
//@INPUT:分配的内存块大小=8*1024*1024
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMalloc申请8*1024*1024字节的内存成功并返回内存首地址
//@EXECUTIONFLOW:2、往内存中存入2*1024*1024个4字节的数，数字分别为0,1…99，循环往复可以存入成功
//@EXECUTIONFLOW:3、从该内存中读出相应的数据并进行校验，应该全部校验成功，如有不同则本测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSFree释放申请的内存并成功返回
CSUDI_BOOL CSTC_OS_IT_MallocFree_0002( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uSize = 8*TEST_MEMORY_MALLOC_SIZE;
	int i;
 
	puChunk = (unsigned int*)CSUDIOSMalloc(uSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"申请内存失败");

	for(i=0 ; i<2*TEST_MEMORY_MALLOC_SIZE ; i++)  
	{
		puChunk[i] = i%100;  
	}
	
	for(i=0 ; i<2*TEST_MEMORY_MALLOC_SIZE ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk[i] == (i%100), "校验不成功");
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL) 
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMalloc 
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:8M内存的分配和释放是否成功的强度测试
//@PRECONDITION:
//@INPUT:分配的内存块大小=8*1024*1024
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMalloc申请8*1024*1024字节的内存成功并返回内存首地址
//@EXECUTIONFLOW:2、往内存中存入2*1024*1024个4字节的数，数字分别为0,1…99，循环往复可以存入成功
//@EXECUTIONFLOW:3、从该内存中读出相应的数据并进行校验，应该全部校验成功，如有不同则本测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSFree释放申请的内存并成功返回
//@EXECUTIONFLOW:5、重复1~4步OS_MEMORY_TEST_COUNT次，OS_MEMORY_TEST_COUNT通过配置文件传入
CSUDI_BOOL CSTC_OS_IT_MallocFree_0003( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uSize = 8*TEST_MEMORY_MALLOC_SIZE;
	int i,j;

	CSTCPrint("内存强度测试开始，请等待。。。\n");

	for(j=0 ; j<OS_MEMORY_TEST_COUNT ; j++)
	{
		CSTCPrint("\n第%d次申请内存测试开始!!!!!\n",j);
		
		puChunk = (unsigned int*)CSUDIOSMalloc(uSize);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"申请内存失败");

		for(i=0 ; i<2*TEST_MEMORY_MALLOC_SIZE ; i++)
		{
			puChunk[i] = i%100;
		}

		for(i=0 ; i<2*TEST_MEMORY_MALLOC_SIZE ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk[i] == (i%100), "校验不成功");
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
		puChunk = CSUDI_NULL;

		CSTCPrint("\n第%d次申请内存测试结束!!!!!\n",j);
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
			puChunk = CSUDI_NULL;
		}
	}

	CSTCPrint("内存强度测试结束！！！\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMalloc 
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:测试内存大小不是4的整数倍的分配和释放是否成功
//@PRECONDITION:
//@INPUT:分配的内存块大小=1023
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMalloc申请1023字节的内存成功并返回内存首地址
//@EXECUTIONFLOW:2、调用memset将每个字节都设成字符'C'
//@EXECUTIONFLOW:3、从该内存中读出最后一个字节的数据看是否等于'C'，如不等则本测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSFree释放申请的内存并成功返回
CSUDI_BOOL CSTC_OS_IT_MallocFree_0004( void )
{
	char *puChunk = CSUDI_NULL;
	unsigned int uSize = 1023;

	puChunk = (char*)CSUDIOSMalloc(uSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"申请内存失败");

	memset(puChunk,'C',1023);

	CSTK_ASSERT_TRUE_FATAL(puChunk[1022] == 'C',"读出的字符和存入的不一致");
	
	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSMalloc 
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:测试分配大小为0字节的内存是否成功
//@PRECONDITION:
//@INPUT:分配的内存块大小=0
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMalloc申请0字节的内存失败并返回CSUDI_NULL
//@EXECUTIONFLOW:2、判断步骤1的返回值如果不等于CSUDI_NULL则调用CSUDIOSFree释放分配的内存并返回测试用例失败，否则返回成功
CSUDI_BOOL CSTC_OS_IT_MallocFree_0005( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uSize = 0;

	puChunk = (unsigned int*)CSUDIOSMalloc(uSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == puChunk,"步骤1失败");

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSCalloc  
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:测试小块内存的分配和释放是否成功
//@PRECONDITION:
//@INPUT:1、分配元素个数=4
//@INPUT:2、每个元素的大小=1000
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSCalloc申请4块1000字节的内存块成功并返回内存首地址
//@EXECUTIONFLOW:2、从该内存中读出相应的数据并校验看是否都被成功初始化为0，如存在非0数据则本测试用例失败
//@EXECUTIONFLOW:3、往每块内存中存入250个4字节的数，数字分别为0,1…99，循环往复可以存入成功
//@EXECUTIONFLOW:4、从该内存中读出相应的数据并进行校验，应该全部校验成功，如有不同则本测试用例失败
//@EXECUTIONFLOW:5、调用CSUDIOSFree释放申请的内存并成功返回
CSUDI_BOOL CSTC_OS_IT_CallocFree_0001( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uElements = 4;
	unsigned int uElementSize = 1000;
	int i,j;

	puChunk = (unsigned int*)CSUDIOSCalloc(uElements,uElementSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"分配内存失败");

	for(i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<250 ; j++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk[i*250+j] == 0,"分配的内存自动初始化失败");
		}
	}

	for(i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<250 ; j++)
		{
			puChunk[i*250+j] = j%100;
		}
	}

	for(i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<250 ; j++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk[i*250+j] == j%100,"存入的数据和读出的不一致");
		}
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSCalloc  
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:大块内存的分配和释放是否成功的强度测试
//@PRECONDITION:
//@INPUT:1、分配元素个数=4
//@INPUT:2、每个元素的大小=1000
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSCalloc申请4块1000字节的内存块成功并返回内存首地址
//@EXECUTIONFLOW:2、从该内存中读出相应的数据并校验看是否都被成功初始化为0，如存在非0数据则本测试用例失败
//@EXECUTIONFLOW:3、往每块内存中存入250个4字节的数，数字分别为0,1…99，循环往复可以存入成功
//@EXECUTIONFLOW:4、从该内存中读出相应的数据并进行校验，应该全部校验成功，如有不同则本测试用例失败
//@EXECUTIONFLOW:5、调用CSUDIOSFree释放申请的内存并成功返回
//@EXECUTIONFLOW:6、重复1~5步OS_MEMORY_TEST_COUNT次，OS_MEMORY_TEST_COUNT通过配置文件传入
CSUDI_BOOL CSTC_OS_IT_CallocFree_0002( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uElements = 4;
	unsigned int uElementSize = 1000;
	int i,j,m;


	CSTCPrint("Calloc强度测试开始，请等待。。。\n");

	for(m=0 ; m<OS_MEMORY_TEST_COUNT ; m++)
	{
		puChunk = (unsigned int*)CSUDIOSCalloc(uElements,uElementSize);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"分配内存失败");

		for(i=0 ; i<4 ; i++)
		{
			for(j=0 ; j<250 ; j++)
			{
				CSTK_ASSERT_TRUE_FATAL(puChunk[i*250+j] == 0,"分配的内存自动初始化失败");
			}
		}

		for(i=0 ; i<4 ; i++)
		{
			for(j=0 ; j<250 ; j++)
			{
				puChunk[i*250+j] = j%100;
			}
		}

		for(i=0 ; i<4 ; i++)
		{
			for(j=0 ; j<250 ; j++)
			{
				CSTK_ASSERT_TRUE_FATAL(puChunk[i*250+j] == j%100,"存入的数据和读出的不一致");
			}
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");

		puChunk = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
			puChunk = CSUDI_NULL;
		}
	}

	CSTCPrint("Calloc强度测试结束！！！\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSRealloc   
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:测试重分配的内存大小小于申请的内存大小的情况能否重分配成功
//@PRECONDITION:CSUDIOSMalloc可以成功执行
//@INPUT:1、待重分配的内存地址=CSUDIOSMalloc返回的内存指针
//@INPUT:2、重分配的内存大小=500
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMalloc申请1000字节的内存成功并返回内存首地址
//@EXECUTIONFLOW:2、往内存中存入250个4字节的数，数字分别为0,1…99，循环往复可以存入成功
//@EXECUTIONFLOW:3、从该内存中读出相应的数据并进行校验，应该全部校验成功，如有不同则本测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSRealloc重新分配500字节的内存
//@EXECUTIONFLOW:5、从重新分配的内存中读出相应的数据并校验，应该可以全部校验成功
//@EXECUTIONFLOW:6、往重新分配的内存中存入125个4字节的数，数字分别为0,1,2...  49，循环往复可以存入成功
//@EXECUTIONFLOW:7、从内存中读出相应的数据并校验，应该可以全部校验成功
//@EXECUTIONFLOW:8、调用CSUDIOSFree释放申请的内存并成功返回
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0001( void )
{
	unsigned int *puChunk1 = CSUDI_NULL;
	unsigned int *puChunk2 = CSUDI_NULL;
	unsigned int uSize = 1000;
	int i;

	puChunk1 = (unsigned int*)CSUDIOSMalloc(uSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk1,"步骤1失败");

	for(i=0 ; i<250 ; i++)
	{
		puChunk1[i] = i%100;
	}

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk1[i] == (i%100), "校验不成功");
	}

	puChunk2= (unsigned int*)CSUDIOSRealloc(puChunk1,500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk2,"步骤4失败");
	
	puChunk1 = CSUDI_NULL;
	
	for(i=0 ; i<125 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "步骤5失败");
	}

	for(i=0 ; i<125 ; i++)
	{
		puChunk2[i] = i%50;
	}

	for(i=0 ; i<125 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%50), "步骤7失败");
	}

	CSTK_FATAL_POINT;
	
	{
		if(puChunk1 != puChunk2)
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "释放内存出错");
				puChunk1 = CSUDI_NULL;
			}
			
			if (puChunk2 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk2), "释放内存出错");
				puChunk2 = CSUDI_NULL;
			}
		}
		else
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "释放内存出错");
				puChunk1 = CSUDI_NULL;
				puChunk2 = CSUDI_NULL;
			}
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSRealloc   
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:测试重分配的内存大小大于申请的内存大小的情况能否重分配成功
//@PRECONDITION:CSUDIOSMalloc可以成功执行
//@INPUT:1、待重分配的内存地址=CSUDIOSMalloc返回的内存指针
//@INPUT:2、重分配的内存大小=2000
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMalloc申请1000字节的内存成功并返回内存首地址
//@EXECUTIONFLOW:2、往内存中存入250个4字节的数，数字分别为0,1…99，循环往复可以存入成功
//@EXECUTIONFLOW:3、从该内存中读出相应的数据并进行校验，应该全部校验成功，如有不同则本测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSRealloc重新分配2000字节的内存
//@EXECUTIONFLOW:5、从重新分配的内存中读出前1000字节的数据并校验，应该可以全部校验成功
//@EXECUTIONFLOW:6、往重新分配的内存中存入500个4字节的数，数字分别为0,1,2...  99，循环往复可以存入成功
//@EXECUTIONFLOW:7、从内存中读出相应的数据并校验，应该可以全部校验成功
//@EXECUTIONFLOW:8、调用CSUDIOSFree释放申请的内存并成功返回
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0002( void )
{
	unsigned int *puChunk1 = CSUDI_NULL;
	unsigned int *puChunk2 = CSUDI_NULL;
	unsigned int uSize = 1000;
	int i;

	puChunk1 = (unsigned int*)CSUDIOSMalloc(uSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk1,"步骤1失败");

	for(i=0 ; i<250 ; i++)
	{
		puChunk1[i] = i%100;
	}

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk1[i] == (i%100), "校验不成功");
	}

	puChunk2= (unsigned int*)CSUDIOSRealloc(puChunk1,2000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk2,"步骤4失败");

	puChunk1 = CSUDI_NULL;

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "步骤5失败");
	}

	for(i=0 ; i<500 ; i++)
	{
		puChunk2[i] = i%100;
	}

	for(i=0 ; i<500 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "步骤7失败");
	}

	CSTK_FATAL_POINT
	{
		if(puChunk1 != puChunk2)
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "释放内存出错");
				puChunk1 = CSUDI_NULL;
			}
			
			if (puChunk2 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk2), "释放内存出错");
				puChunk2 = CSUDI_NULL;
			}
		}
		else
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "释放内存出错");
				puChunk1 = CSUDI_NULL;
				puChunk2 = CSUDI_NULL;
			}
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSRealloc   
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:测试重分配的内存地址等于CSUDI_NULL的情况能否重分配成功
//@PRECONDITION:
//@INPUT:1、待重分配的内存地址=CSUDI_NULL
//@INPUT:2、重分配的内存大小=1000
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSRealloc重新分配1000字节的内存
//@EXECUTIONFLOW:2、往重新分配的内存中存入250个4字节的数，数字分别为0,1,2...  99，循环往复可以存入成功
//@EXECUTIONFLOW:3、从内存中读出相应的数据并校验，应该可以全部校验成功
//@EXECUTIONFLOW:4、调用CSUDIOSFree释放申请的内存并成功返回
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0003( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	int i;
	
	puChunk= (unsigned int*)CSUDIOSRealloc((unsigned int*)CSUDI_NULL,1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"步骤1失败");

	for(i=0 ; i<250 ; i++)
	{
		puChunk[i] = i%100;
	}

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk[i] == (i%100), "校验不成功");
	}

	CSTK_FATAL_POINT;
	
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "释放内存出错");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIOSRealloc   
//@CASEGROUP:2、CSUDIOSFree 
//@DESCRIPTION:内存的重分配和释放的强度测试
//@PRECONDITION:CSUDIOSMalloc可以成功执行
//@INPUT:1、待重分配的内存地址=CSUDIOSMalloc返回的内存指针
//@INPUT:2、重分配的内存大小=2000
//@EXPECTATION:每一步骤都达到预期结果
//@EXECUTIONFLOW:1、调用CSUDIOSMalloc申请1000字节的内存成功并返回内存首地址
//@EXECUTIONFLOW:2、往内存中存入250个4字节的数，数字分别为0,1…99，循环往复可以存入成功
//@EXECUTIONFLOW:3、从该内存中读出相应的数据并进行校验，应该全部校验成功，如有不同则本测试用例失败
//@EXECUTIONFLOW:4、调用CSUDIOSRealloc重新分配2000字节的内存
//@EXECUTIONFLOW:5、从重新分配的内存中读出前1000字节的数据并校验，应该可以全部校验成功
//@EXECUTIONFLOW:6、往重新分配的内存中存入500个4字节的数，数字分别为0,1,2...  99，循环往复可以存入成功
//@EXECUTIONFLOW:7、从内存中读出相应的数据并校验，应该可以全部校验成功
//@EXECUTIONFLOW:8、调用CSUDIOSFree释放申请的内存并成功返回
//@EXECUTIONFLOW:9、重复执行1~8步OS_MEMORY_TEST_COUNT次，OS_MEMORY_TEST_COUNT通过配置文件传入
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0004( void )
{
	unsigned int *puChunk1 = CSUDI_NULL;
	unsigned int *puChunk2 = CSUDI_NULL;
	unsigned int uSize = 1000;
	int i,j;

	CSTCPrint("Realloc强度测试开始，请等待。。。\n");

	for(j=0 ; j<OS_MEMORY_TEST_COUNT ; j++)
	{
		puChunk1 = (unsigned int*)CSUDIOSMalloc(uSize);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk1,"步骤1失败");

		for(i=0 ; i<250 ; i++)
		{
			puChunk1[i] = i%100;
		}

		for(i=0 ; i<250 ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk1[i] == (i%100), "校验不成功");
		}

		puChunk2= (unsigned int*)CSUDIOSRealloc(puChunk1,2000);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk2,"步骤4失败");

		puChunk1 = CSUDI_NULL;
		
		for(i=0 ; i<250 ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "步骤5失败");
		}

		for(i=0 ; i<500 ; i++)
		{
			puChunk2[i] = i%100;
		}

		for(i=0 ; i<500 ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "步骤7失败");
		}
		
		if(puChunk1 != puChunk2)
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "释放内存出错");
				puChunk1 = CSUDI_NULL;
			}	
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk2), "释放内存出错");
			puChunk2 = CSUDI_NULL;
		}
		else
		{
			if (puChunk1 != CSUDI_NULL)
			{			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "释放内存出错");
			}
			puChunk1 = CSUDI_NULL;
			puChunk2 = CSUDI_NULL;
		}
	}

	CSTK_FATAL_POINT;
	
	{
		if(puChunk1 != puChunk2)
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "释放内存出错");
				puChunk1 = CSUDI_NULL;
			}
			
			if (puChunk2 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk2), "释放内存出错");
				puChunk2 = CSUDI_NULL;
			}
		}
		else
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "释放内存出错");
				puChunk1 = CSUDI_NULL;
				puChunk2 = CSUDI_NULL;
			}
		}
	}

	CSTCPrint("Realloc强度测试结束！！！\n");

	return CSUDI_TRUE;
}

#if 1
static int g_aSize[100] = {1, 100, 256, 144, 1024, 154863, 980, 456, 74777, 520, 
						10240, 50247, 698, 756, 46756, 620, 788, 634, 461024, 141024,
						48, 92, 871, 358, 145457, 364, 999, 1069, 454541, 412421, 
						8694, 597, 1541544, 648, 4774212, 3948, 7455, 12, 96, 853,
						50*1024, 6812, 89954, 4122, 12, 75515, 55, 455, 52525, 15,
						2232, 546, 12, 454, 226, 45435, 4887, 44212, 78899, 8551, 
						212, 15, 445, 4585, 75, 255, 369, 784589, 111, 457,
						45565, 411, 78, 59, 25565, 8423, 254, 3694498,225, 11,
						1212, 44545, 36, 588, 4444, 3368, 7787, 1556, 3, 454,
						787574, 325, 4578, 7899, 15, 789, 236, 2265, 575, 1121555};

static void * g_pAddr[100] = {0};


#define 	K 	(1024)
#define 	M 	(1024*1024)

//测试的粒度是1K
static unsigned int GetMaxBlockSize(unsigned uBaseMax, unsigned int uBaseMin)
{
	unsigned int maxsize= 0;

	CSUDI_BOOL bFound= CSUDI_FALSE;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	void * pMem = CSUDI_NULL;
	
	if ((uBaseMin >= (2*K)) && ((uBaseMax > uBaseMin+(2*K))))
	{
		for (maxsize = uBaseMax; maxsize >=uBaseMin;)
		{
			 pMem = CSUDIOSMalloc(maxsize);
			 
			 if (pMem != CSUDI_NULL)
			 {
			 	memset(pMem, 0xa5, maxsize);
			 	
			 	udiRe =  CSUDIOSFree(pMem);
			 	if (udiRe != CSUDI_SUCCESS)
				{
					CSTCPrint("内存释放失败!!!");
				}
				
				bFound = CSUDI_TRUE;
				
			 	break;
			 }
			 else
			 {
			 	maxsize  -= K;
			 }
		}
	}
	
	if (bFound  == CSUDI_FALSE)
	{
		maxsize = 0;
		
		CSTCPrint("要求探测的区间错误!!!");
	}
	
	return maxsize;
	
}

typedef struct _testMemStruct
{
	void * m_pvAdd;	
	struct _testMemStruct  *m_pNext;

}TestMemStruct;

static void PrintMemorySize(void)
{
#define MAX_BASE_ARRAY_LEN 20

	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	unsigned int uTotalSize = 0;
	
	unsigned int uMaxBlock =0;

	int i= 0;

	void *pMem= CSUDI_NULL;
	
	TestMemStruct *pMemStruct= CSUDI_NULL;
	TestMemStruct *pMemStructIterator= CSUDI_NULL;
	
	unsigned int aBasicSize[MAX_BASE_ARRAY_LEN] = {512*M,256*M,128*M,64*M, 32*M,16*M,8*M, 4*M, 2*M, 1*M, 512*K, 256*K, 128*K,64*K,32*K,16*K,8*K,4*K,2*K,K};
	
	TestMemStruct *astMemStruct[MAX_BASE_ARRAY_LEN];
	int anMemCount[MAX_BASE_ARRAY_LEN];
	
	memset(astMemStruct, 0, sizeof(astMemStruct));
	memset(anMemCount,0,sizeof(anMemCount));

	//试分配
	for (i = 0; i <MAX_BASE_ARRAY_LEN;  i++ )
	{
		while (1)
		{
			pMem = CSUDIOSMalloc(aBasicSize[i]);
			
			if (pMem != CSUDI_NULL)
			{
				CSTCPrint("Try malloc memory size 0x%x success\n", aBasicSize[i]);
				
				memset(pMem, i, aBasicSize[i]);
				
				pMemStruct = (TestMemStruct *)pMem;
				pMemStruct->m_pvAdd = pMem;
				pMemStruct->m_pNext = CSUDI_NULL;

				if(astMemStruct[i] == CSUDI_NULL)
				{
					astMemStruct[i] = pMemStruct;
				}
				else
				{
					/*查找最后一个节点*/
					pMemStructIterator  = astMemStruct[i];
					
					while (pMemStructIterator->m_pNext != CSUDI_NULL)
					{
						pMemStructIterator  =  pMemStructIterator->m_pNext;
					}

					pMemStructIterator->m_pNext = pMemStruct;
				}

				anMemCount[i] += 1;
				
			}
			else
			{
				break;
			}
		}
	}

	//释放
	for (i = MAX_BASE_ARRAY_LEN-1;  i >=0 ;  i-- ) 
	{		
		pMemStruct  = astMemStruct[i];
		
		pMem = (void *)pMemStruct;
		
		while (pMem != CSUDI_NULL)
		{
			pMemStruct  =  pMemStruct->m_pNext;
			
			udiRe = CSUDIOSFree(pMem);

			if (udiRe != CSUDI_SUCCESS)
			{
				CSTCPrint("内存释放失败!!!");
			}
			
			pMem = (void *)pMemStruct;
		}
	}

	//探测最大的内存块
	for (i=0; i < MAX_BASE_ARRAY_LEN; i++)
	{
		if (anMemCount[i] != 0)
		{
			if (i >0)
			{
				uMaxBlock = GetMaxBlockSize(aBasicSize[i-1], aBasicSize[i]);
				
				break;
			}
			else
			{
				CSTCPrint("最大内存块大小不够，请更新程序!!!");
			}
		}
	}

	//统计全部内存	
	uTotalSize = 0;	
	for (i=0; i < MAX_BASE_ARRAY_LEN; i++)
	{
		uTotalSize += anMemCount[i]*aBasicSize[i];
	}
	
	CSTCPrint("##################################\n");
	CSTCPrint("#可用内存的总大小:0x%x字节\n",uTotalSize);
	CSTCPrint("#可用内存的最大块:0x%x字节\n",uMaxBlock);
	CSTCPrint("##################################\n");
}



static void TestMemFragment(void)
{
	int nCount = 0;
	int nRand = 0;
	int nRand2 = 0;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	CSTCPrint("碎片测试之前的内存情况\n");
	PrintMemorySize();
	
	while(nCount < 0x1000)
	{
		nCount++;
		nRand = rand()%100;
		
		if(g_pAddr[nRand] != CSUDI_NULL)
		{
			udiRe = CSUDIOSFree(g_pAddr[nRand]);
			
			if(udiRe == CSUDI_SUCCESS)
			{
				g_pAddr[nRand] = NULL;
			}
			else
			{
				CSTCPrint("释放内存失败\n");
			}
		}
		nRand2 = rand()%100;
		
		g_pAddr[nRand] = CSUDIOSMalloc( g_aSize[nRand2]);
		
		if(g_pAddr[nRand] == CSUDI_NULL)
		{
			CSTCPrint("\nmalloc failed. malloc size:%d, nCount:%d.", g_aSize[nRand2], nCount);					
		}
	}
	
	nRand2 = sizeof(g_aSize)/sizeof(int);
	
	for (nRand=0;nRand < nRand2;nRand++)
	{
		if(g_pAddr[nRand] != CSUDI_NULL)
		{
			udiRe = CSUDIOSFree(g_pAddr[nRand]);
			
			if(udiRe == CSUDI_SUCCESS)
			{
				g_pAddr[nRand] = NULL;
			}
			else
			{
				CSTCPrint("释放内存失败\n");
			}
		}
	}	
	
	CSTCPrint("碎片测试之后的内存情况\n");
	
	PrintMemorySize();
}

//@CASEGROUP:测试可用内存大小   ，包括最大块和总内存
//@DESCRIPTION:可用内存大小测试
//@PRECONDITION:CSUDIOSMalloc/CSUDIOSFree可以成功执行
//@INPUT:
//@EXPECTATION:打印输出系统可用内存大小和最大内存块
//@EXECUTIONFLOW:
CSUDI_BOOL CSTC_OS_IT_Available_Memory( void )
{
	PrintMemorySize();
	
	return CSUDI_TRUE;
}

//@CASEGROUP:测试内存碎片情况
//@DESCRIPTION: 随机分配不固定内存大小并随机释放已分配内存的情况下，内存碎片情况
//@PRECONDITION:CSUDIOSMalloc/CSUDIOSFree可以成功执行
//@INPUT:
//@EXPECTATION:对比打印输出系统经过内存碎片测试前后可用内存大小和最大内存块
//@EXECUTIONFLOW:
CSUDI_BOOL CSTC_OS_IT_Memory_Fragments( void )
{
	TestMemFragment();
	
	return CSUDI_TRUE;
}
#endif

//@CASEGROUP:测试系统对内存的读写速率  
//@DESCRIPTION:对内存进行初始化，赋值以及拷贝等操作，获取读写速率
//@PRECONDITION:CSUDIOSMalloc/CSUDIOSFree可以成功执行
//@INPUT:pucBuf 值合法
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、使用memset函数清0，每次操作1MB数据量，重复200次,并获取时间
//@EXECUTIONFLOW:2、使用memcpy函数，每次操作1MB数据量，重复200次，并获取时间
//@EXECUTIONFLOW:3、使用单字节直接赋值，每个循环操作16次，循环200×1024×1024/16次，并获取时间
//@EXECUTIONFLOW:4、使用4字节直接赋值，每个循环操作16次，循环200×1024×1024/64次，并获取时间
CSUDI_BOOL  CSTC_OS_IT_MemoryPerformance_0001(void)
{
    CSUDI_INT32 i , j;

    CSUDI_INT32 nMemory1M =  1024 * 1024 ;
    CSUDI_INT32 nMemoryTestCount = 200 * 1024 * 1024 ;
     
    CSUDI_UINT8 *pucBuf = CSUDI_NULL ;
    CSUDI_UINT8 *pucBuf2 = CSUDI_NULL ;

    CSUDIOSTimeVal_S  sSysTime;
	CSUDI_UINT8 cBufferByte;
	CSUDI_UINT32 uBufferDWord;

    CSUDIOSTimeVal_S  temp1;
    CSUDIOSTimeVal_S  temp2;
    CSUDIOSTimeVal_S  temp3;
    CSUDIOSTimeVal_S  temp4;

    CSTCPrint("测试开始,此测试大概需要20s,请耐心等待\n");

    pucBuf =  (CSUDI_UINT8 *)CSUDIOSMalloc( nMemory1M );

    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"分配内存失败");

    //用于测试使用memset函数清0，每次操作1MB数据量，重复200次

    CSTCPrint("正在进行内存的memset函数清0测试...\n");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp1),"第一步获取系统时间失败");

    for ( i =0 ; i <  200 ; ++i)
    {
        memset(pucBuf, 0x0, nMemory1M);
    }

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"第一步获取系统时间失败");

    temp1.m_nSecond = sSysTime.m_nSecond - temp1.m_nSecond;
    temp1.m_nMiliSecond = sSysTime.m_nMiliSecond - temp1.m_nMiliSecond;
    
    //使用memcpy函数，每次操作1MB数据量，重复重复200次
    CSTCPrint("正在进行内存的memcpy拷贝测试...\n");
    
    memset(pucBuf, 0x55, sizeof(pucBuf));

    pucBuf2 =  (CSUDI_UINT8 *)CSUDIOSMalloc( nMemory1M );

    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf2,"分配内存失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp2),"第二步获取系统时间失败");

    for (i =0; i < 200 ; ++i)
    {
        memcpy(pucBuf2 , pucBuf , nMemory1M);
    }

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"第二步获取系统时间失败");

    temp2.m_nSecond = sSysTime.m_nSecond - temp2.m_nSecond;
    temp2.m_nMiliSecond = sSysTime.m_nMiliSecond - temp2.m_nMiliSecond;
     
    //使用单字节直接赋值，每个循环操作16次，循环200×1024×1024/16次

    CSTCPrint("正在进行内存的单字节赋值测试...\n");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp3),"第三步获取系统时间失败");

    for ( i =0 ; i < (nMemoryTestCount / 16) ; ++i )
    {
        for( j =0 ; j < 16 ; ++j )
        {
            cBufferByte = 0x55;
        }     
    }

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"第三步获取系统时间失败");

    temp3.m_nSecond = sSysTime.m_nSecond - temp3.m_nSecond;
    temp3.m_nMiliSecond = sSysTime.m_nMiliSecond - temp3.m_nMiliSecond;
   
    //使用4字节直接赋值，每个循环操作16次，循环200×1024×1024/64次

    CSTCPrint("正在进行内存的四字节赋值测试...\n");    

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp4),"第四步获取系统时间失败");

    for ( i =0 ; i < ( nMemoryTestCount / 64) ; ++i)
    {
        for( j =0 ; j < 16 ; ++j )
        {
            uBufferDWord = 0x55bb55bb;
        }  
    }

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"第四步获取系统时间失败");

    temp4.m_nSecond = sSysTime.m_nSecond - temp4.m_nSecond;
    temp4.m_nMiliSecond = sSysTime.m_nMiliSecond - temp4.m_nMiliSecond;
   
    CSTCPrint("Memset 清零1MB的数据200次的速率为: %0.2f M/s \n " , (1.0 * 200 ) / ((temp1.m_nSecond*1000) + temp1.m_nMiliSecond ) * 1000);
    CSTCPrint("Memcpy 拷贝1MB的数据200次的速率为: %0.2f M/s\n " , (1.0 * 200 ) / ((temp2.m_nSecond*1000) + temp2.m_nMiliSecond ) * 1000);
    CSTCPrint("单字节直接赋值的速率为: %0.2f M/s\n " , 200.0 / ((temp3.m_nSecond*1000) + temp3.m_nMiliSecond ) * 1000 );
    CSTCPrint("四字节直接赋值的速率为: %0.2f M/s\n " , 200.0 / ((temp4.m_nSecond*1000) + temp4.m_nMiliSecond ) * 1000 );

    CSTK_FATAL_POINT
    {

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"销毁资源失败");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf2),"销毁资源失败");
    }    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetFreeRAM   
//@DESCRIPTION:测试获取可用的RAM空间的准确性
//@PRECONDITION:
//@INPUT:合法参数
//@EXPECTATION:获取到的可用RAM空间值在0到配置文件中描述的总空间值之间
//@EXECUTIONFLOW:1、调用CSUDIOSGetFreeRAM(合法参数)获取可用RAM空间，应该返回成功
//@EXECUTIONFLOW:2、判断获取到的可用的RAM空间在0到总空间值之间
CSUDI_BOOL CSTC_OS_IT_GetFreeRAM_0001( void )
{
	unsigned int nFreeCount=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetFreeRAM(&nFreeCount),"步骤一:获取可用的RAM空间失败");

	CSTK_ASSERT_TRUE_FATAL((nFreeCount >= 0 && nFreeCount <= g_nRAMSize),"获取到的可用RAM空间不在0到总空间范围内");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetFreeRAM   
//@DESCRIPTION:测试参数为非法值情况
//@PRECONDITION:
//@INPUT:非法参数
//@EXPECTATION:获取可用RAM空间时返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSGetFreeRAM(CSUDI_NULL)，应该返回失败(非CSUDI_SUCCESS)
CSUDI_BOOL CSTC_OS_IT_GetFreeRAM_0002( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetFreeRAM(CSUDI_NULL),"步骤一:传入非法参数应该返回失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetCPUCount   
//@DESCRIPTION:测试获取CPU个数的准确性
//@PRECONDITION:
//@INPUT:合法参数
//@EXPECTATION:获取到的CPU个数与配置文件中描述相同
//@EXECUTIONFLOW:1、调用CSUDIOSGetCPUCount(合法参数)获取CPU个数，应该返回成功
//@EXECUTIONFLOW:2、判断获取到的CPU个数与配置文件中描述的一致
CSUDI_BOOL CSTC_OS_IT_GetCPUCount_0001( void )
{
	unsigned int nCpuCount=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetCPUCount(&nCpuCount),"步骤一:获取CPU个数失败");

	CSTK_ASSERT_TRUE_FATAL(nCpuCount == g_nCPUCount,"步骤二:获取到得CPU个数与配置文件不一致");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetCPUCount   
//@DESCRIPTION:测试参数为非法值情况
//@PRECONDITION:
//@INPUT:非法参数
//@EXPECTATION:获取CPU个数时返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSGetCPUCount(CSUDI_NULL)，应该返回失败(非CSUDI_SUCCESS)
CSUDI_BOOL CSTC_OS_IT_GetCPUCount_0002( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetCPUCount(CSUDI_NULL),"获取CPU个数返回失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetCPUUsage   
//@DESCRIPTION:测试CPU利用率的正确性
//@INPUT:1、index从0开始，小于配置文件中描述的CPU个数
//@PRECONDITION:
//@INPUT:2、pnUsage合法
//@EXPECTATION:获取到正确的CPU利用率
//@EXECUTIONFLOW:1、调用CSUDIOSGetCPUUsage(存在的索引,合法参数)，应该返回成功
//@EXECUTIONFLOW:2、判断获取到的CPU利用率在0~100之间
//@EXECUTIONFLOW:3、传入所有可用索引，重复步骤1,2
CSUDI_BOOL CSTC_OS_IT_GetCPUUsage_0001( void )
{
	unsigned int nIndex=0;
	unsigned int nUsage=0;
	int i = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetCPUUsage(nIndex, &nUsage),"步骤一:获取index=0的CPU利用率失败");

	CSTK_ASSERT_TRUE_FATAL((nUsage >= 0 && nUsage <= 100) ,"步骤二:获取的CUP利用率不在0~100之间");

	for ( i =0 ; i < g_nCPUCount ; ++i)
	{
		CSTCPrint("获取index=%d的CPU的利用率\n", i);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetCPUUsage(i, &nUsage),"获取存在的索引的CPU利用率，应该返回成功");

		CSTK_ASSERT_TRUE_FATAL((nUsage >= 0 && nUsage <= 100),"获取的CUP利用率不在0~100之间");
	}

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetCPUUsage   
//@DESCRIPTION:测试参数为非法值的情况
//@PRECONDITION:
//@INPUT:1、index取不存在的索引值
//@INPUT:2、pnUsage为非法值
//@EXPECTATION:获取CPU利用率时返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSGetCPUUsage(100,合法参数)，应该返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:2、调用CSUDIOSGetCPUUsage(0,非法参数)，应该返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:3、调用CSUDIOSGetCPUUsage(100,非法参数)，应该返回失败(非CSUDI_SUCCESS)
CSUDI_BOOL CSTC_OS_IT_GetCPUUsage_0002( void )
{
	unsigned int nIndex=100;
	unsigned int pnUsage=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetCPUUsage(nIndex, &pnUsage),"步骤一:传入不存在的索引，应该返回失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetCPUUsage(0, CSUDI_NULL),"步骤二:传入存在的索引但参数2非法，应该返回失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetCPUUsage(nIndex, CSUDI_NULL),"步骤三:传入不存在的索引且参数2非法，应该返回失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetAllThreadHandle   
//@DESCRIPTION:测试获取系统中所有任务的句柄的正确性
//@PRECONDITION:存在一个已知句柄的任务
//@INPUT:合法参数，数组足够大
//@EXPECTATION:获取正确的任务句柄数组
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建一个任务成功，记录任务句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGetAllThreadHandle(合法参数,合法参数)，应该返回成功
//@EXECUTIONFLOW:3、判断获取到的任务句柄数组大小不小于1
//@EXECUTIONFLOW:4、判断获取到的任务句柄数组中有一个句柄与步骤1创建的任务句柄相等
//@EXECUTIONFLOW:5、调用CSUDIOSThreadDestroy删除步骤1创建的任务，应该返回成功
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	CSUDI_HANDLE ahThread[OS_MAX_THREAD_COUNT] = {0};
	unsigned int nSize = OS_MAX_THREAD_COUNT;
	int i = 0;
	CSUDI_BOOL bHave = CSUDI_FALSE;	

	g_bThreadEnd = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),"步骤一:创建一个任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"步骤一:创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetAllThreadHandle(ahThread, &nSize),"步骤二:返回失败");

	CSTK_ASSERT_TRUE_FATAL(nSize >= 1,"步骤三:获取到的务句柄数组大小小于1");
	
	for ( i = 0 ; i < nSize; i++)
	{
		if (ahThread[i] == hThreadhandle)
		{
			bHave = CSUDI_TRUE;
			break;
		}
	}
	CSTK_ASSERT_TRUE_FATAL(bHave == CSUDI_TRUE,"步骤四:获取到的任务句柄数组中不包含步骤1创建的任务");

	CSTK_FATAL_POINT
	{
		while(!g_bThreadEnd)
		{
			CSUDIOSThreadSleep(10);
		}
		
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤五:删除步骤1创建的任务失败");
			  hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetAllThreadHandle   
//@DESCRIPTION:测试获取系统中所有任务的句柄的正确性
//@PRECONDITION:存在两个已知句柄的任务
//@INPUT:合法参数，但数组不足够大
//@EXPECTATION:获取到正确的任务句柄数组
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务1成功
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate创建任务2成功
//@EXECUTIONFLOW:3、调用CSUDIOSGetAllThreadHandle(合法参数,1)，应该返回成功
//@EXECUTIONFLOW:4、判断获取到的任务句柄数组大小为1
//@EXECUTIONFLOW:5、调用CSUDIOSThreadDestroy删除步骤1创建的任务，应该返回成功
//@EXECUTIONFLOW:6、调用CSUDIOSThreadDestroy删除步骤2创建的任务，应该返回成功
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0002( void )
{
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	CSUDI_HANDLE ahThread[OS_MAX_THREAD_COUNT] = {0};
	unsigned int uSize = 1;
	unsigned int uCount = 0;
	
	memset(ahThread, 0, sizeof(ahThread));

	g_bThreadEnd = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle1),"步骤一:创建任务1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"步骤一:创建成功的任务1输出的任务句柄不能为空");

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread2", nPriority , 2*TEST_THREAD_STACK_SIZE, 
								OS_TestThread1Entry, &nParam, &hThreadhandle2),"步骤二:创建任务2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"步骤二:创建成功的任务2输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetAllThreadHandle(ahThread, &uSize),"步骤三:返回失败");

	while ((uCount<OS_MAX_THREAD_COUNT) && (ahThread[uCount]!=CSUDI_NULL))
	{
		uCount++;
	}

	CSTK_ASSERT_TRUE_FATAL(uCount == 1,"步骤四失败");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle1 != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "步骤五:删除步骤1创建的任务失败");
			hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "步骤六:删除步骤2创建的任务失败");
			hThreadhandle2 = CSUDI_NULL;
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetAllThreadHandle   
//@DESCRIPTION:测试获取系统中所有任务的句柄的正确性
//@PRECONDITION:
//@INPUT:合法参数，数组足够大
//@EXPECTATION:1、任务存在时获取到的句柄数组中存在该任务句柄
//@EXPECTATION:2、任务删除后获取到的句柄数组中不存在改任务句柄
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务成功，记录任务句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGetAllThreadHandle(合法参数,合法参数)，应该返回成功
//@EXECUTIONFLOW:3、判断获取到的任务句柄数组中存在步骤1创建的任务的句柄
//@EXECUTIONFLOW:4、调用CSUDIOSThreadDestroy删除步骤1创建的任务，应该返回成功
//@EXECUTIONFLOW:5、判断获取到的任务句柄数组中不存在步骤1创建的任务的句柄
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0003( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	CSUDI_HANDLE ahThread[OS_MAX_THREAD_COUNT] = {0};
	unsigned int nSize = OS_MAX_THREAD_COUNT;
	int i = 0;
	CSUDI_BOOL bHave = CSUDI_FALSE;

	g_bThreadEnd = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),"步骤一:创建一个任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"步骤一:创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetAllThreadHandle(ahThread, &nSize),"步骤二获取任务的句柄失败");

	for ( i = 0 ; i < nSize; i++)
	{
		if (ahThread[i] == hThreadhandle)
		{
			bHave = CSUDI_TRUE;
			break;
		}
	}
	CSTK_ASSERT_TRUE_FATAL(bHave == CSUDI_TRUE,"步骤三:获取到的任务句柄数组中不包含步骤1创建的任务");

	CSTK_FATAL_POINT

	if (hThreadhandle != CSUDI_NULL)
	{
		while(!g_bThreadEnd)
		{
			CSUDIOSThreadSleep(10);
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤四:删除步骤1创建的任务失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetAllThreadHandle(ahThread, &nSize),"步骤五:获取任务句柄数组失败");

	bHave = CSUDI_FALSE;
	for ( i = 0 ; i < nSize; i++)
	{
		if (ahThread[i] == hThreadhandle)
		{
			bHave = CSUDI_TRUE;
			break;
		}
	}
	CSTK_ASSERT_TRUE_FATAL(bHave == CSUDI_FALSE,"步骤五:获取到的任务句柄数组中存在了已经删除的任务句柄");
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetAllThreadHandle   
//@DESCRIPTION:测试参数为非法值的情况
//@PRECONDITION:
//@INPUT:非法参数
//@EXPECTATION:获取任务句柄数组时返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSGetAllThreadHandle(CSUDI_NULL,合法参数)，应该返回失败
//@EXECUTIONFLOW:2、调用CSUDIOSGetAllThreadHandle(合法参数,取0)，应该返回失败
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0004( void )
{
	CSUDI_HANDLE ahThread[OS_MAX_THREAD_COUNT] = {0};
	unsigned int nSize1 = 10;
	unsigned int nSize2 = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetAllThreadHandle(CSUDI_NULL, &nSize1),"步骤一:传入非法参数应该返回失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetAllThreadHandle(ahThread, &nSize2),"步骤二:传入非法参数应该返回失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadInfo   
//@DESCRIPTION:测试获取任务信息的正确性
//@PRECONDITION:存在一个已知任务句柄及其他任务信息的任务
//@INPUT:合法参数
//@EXPECTATION:获取到正确的任务信息
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务成功，记录任务句柄及其他任务信息
//@EXECUTIONFLOW:2、调用CSUDIOSGetThreadInfo(传入步骤1记录的任务句柄)获取任务信息，应该返回成功
//@EXECUTIONFLOW:3、判断获取到的任务信息与创建任务时的信息一致
//@EXECUTIONFLOW:4、调用CSUDIOSThreadDestroy删除步骤1创建的任务，应该返回成功
CSUDI_BOOL CSTC_OS_IT_GetThreadInfo_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	CSUDIOSThreadInfo_S sThreadInfo;

	memset(&sThreadInfo, 0, sizeof(sThreadInfo));

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),"步骤一:创建一个任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"步骤一:创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetThreadInfo(hThreadhandle, &sThreadInfo),"步骤二:获取任务信息失败");

	CSTK_ASSERT_TRUE_FATAL(((sThreadInfo.uPriority == nPriority) && (sThreadInfo.uThreadId == (unsigned int)hThreadhandle) && (strcmp(sThreadInfo.szThreadName,"OS_TestThread1")==0)),"步骤三:获取的任务信息跟创建的任务信息不一致");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤四:删除步骤1创建的任务失败");
			hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadInfo   
//@DESCRIPTION:测试参数为非法值的情况
//@PRECONDITION:
//@INPUT:非法参数
//@EXPECTATION:获取任务信息时返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSGetThreadInfo(CSUDI_NULL, 合法参数)，应该返回失败
//@EXECUTIONFLOW:2、调用CSUDIOSGetThreadInfo(CSUDI_NULL, CSUDI_NULL)，应该返回失败
CSUDI_BOOL CSTC_OS_IT_GetThreadInfo_0002( void )
{
	CSUDIOSThreadInfo_S sThreadInfo;
	
	memset(&sThreadInfo, 0, sizeof(sThreadInfo));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadInfo(CSUDI_NULL, &sThreadInfo),"步骤一:传入非法参数应该返回失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadInfo(CSUDI_NULL, CSUDI_NULL),"步骤二:传入非法参数应该返回失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadUsedRAM   
//@DESCRIPTION:测试获取指定任务的RAM消耗的正确性
//@PRECONDITION:存在一个已知任务句柄的任务
//@INPUT:合法参数
//@EXPECTATION:获取到正确的指定任务的RAM消耗
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务成功，记录任务句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGetThreadUsedRAM(步骤1的任务句柄, 合法参数)，应该返回成功
//@EXECUTIONFLOW:3、判断步骤2获取到的RAM消耗在0到配置文件中描述的总RAM值之间
//@EXECUTIONFLOW:4、调用CSUDIOSThreadDestroy删除步骤1创建的任务，应该返回成功
//@EXECUTIONFLOW:5、调用CSUDIOSGetThreadUsedRAM(步骤1的任务句柄, 合法参数)，应该返回失败
CSUDI_BOOL CSTC_OS_IT_GetThreadUsedRAM_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	unsigned int nUsedRam = 0;

	g_bThreadEnd = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),"步骤一:创建一个任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"步骤一:创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetThreadUsedRAM(hThreadhandle, &nUsedRam),"步骤二:返回失败");
	
	CSTK_ASSERT_TRUE_FATAL((nUsedRam >= 0 && nUsedRam <= g_nRAMSize),"步骤三:获取步骤1创建的任务所占RAM空间不在有效范围");

	CSTK_FATAL_POINT

	{
		if (hThreadhandle != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤七:删除步骤1创建的任务失败");
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadUsedRAM(hThreadhandle, &nUsedRam),"步骤五:获取已删除的任务所占RAM空间应该返回失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadUsedRAM   
//@DESCRIPTION:测试参数为非法值的情况
//@PRECONDITION:
//@INPUT:非法参数
//@EXPECTATION:获取任务的RAM消耗时返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSGetThreadUsedRAM(CSUDI_NULL, 合法参数)，应该返回失败
//@EXECUTIONFLOW:2、调用CSUDIOSGetThreadUsedRAM(CSUDI_NULL, CSUDI_NULL)，应该返回失败
CSUDI_BOOL CSTC_OS_IT_GetThreadUsedRAM_0002( void )
{
	unsigned int nUsedRam = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIOSGetThreadUsedRAM(CSUDI_NULL, &nUsedRam),"步骤一:传入非法参数应该返回失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIOSGetThreadUsedRAM(CSUDI_NULL, CSUDI_NULL),"步骤二:传入非法参数应该返回失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadCPUUsage   
//@DESCRIPTION:测试获取指定任务的CPU负载的正确性
//@PRECONDITION:存在一个已知任务句柄的任务
//@INPUT:合法参数
//@EXPECTATION:获取到正确的指定任务的CPU负载
//@EXECUTIONFLOW:1、调用CSUDIOSThreadCreate创建任务成功，该任务循环做一定的数学运算，记录任务句柄
//@EXECUTIONFLOW:2、调用CSUDIOSGetThreadCPUUsage(步骤1的任务句柄, 合法参数)，应该返回成功
//@EXECUTIONFLOW:3、判断步骤2获取到的CPU负载值在0~100之间
//@EXECUTIONFLOW:4、通过配置文件描述判断平台是否支持任务的Suspend，如果支持则调用CSUDIOSThreadSuspend并判断是否返回成功，否则步骤4,5,6跳过
//@EXECUTIONFLOW:5、调用CSUDIOSGetThreadCPULoad(步骤1的任务句柄, 合法参数)，应该返回成功
//@EXECUTIONFLOW:6、判断步骤5获取到的CPU负载值小于步骤2获取到的值
//@EXECUTIONFLOW:7、调用CSUDIOSThreadDestroy删除步骤1创建的任务，应该返回成功
//@EXECUTIONFLOW:8、调用CSUDIOSGetThreadCPULoad(步骤1的任务句柄, 合法参数)，应该返回失败
CSUDI_BOOL CSTC_OS_IT_GetThreadCPULoad_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	unsigned int nUsage = 0;
	unsigned int nUsage2 = 0;

	g_bThreadEnd = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1_1Entry, &nParam, &hThreadhandle),"步骤一:创建一个任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"步骤一:创建成功的任务输出的任务句柄不能为空");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetThreadCPUUsage(hThreadhandle, &nUsage),"步骤二:返回失败");

	CSTK_ASSERT_TRUE_FATAL((nUsage >= 0 && nUsage <= 100),"步骤三获取CPU负载值不在0~100之间");

	if (s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadSuspend(hThreadhandle),"步骤四:挂起任务操作返回失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetThreadCPUUsage(hThreadhandle, &nUsage2),"步骤五:返回失败");

		CSTK_ASSERT_TRUE_FATAL(nUsage > nUsage2,"步骤六:得到的CPU负载大于上次故失败");
	
	}

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "步骤七:删除步骤1创建的任务失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadCPUUsage(hThreadhandle, &nUsage),"步骤八:应该返回失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadCPUUsage   
//@DESCRIPTION:测试参数为非法值的情况
//@PRECONDITION:
//@INPUT:非法参数
//@EXPECTATION:获取指定任务的CPU负载时返回失败(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDIOSGetThreadCPUUsage(CSUDI_NULL, 合法参数)，应该返回失败
//@EXECUTIONFLOW:2、调用CSUDIOSGetThreadCPUUsage(CSUDI_NULL, CSUDI_NULL)，应该返回失败
CSUDI_BOOL CSTC_OS_IT_GetThreadCPULoad_0002( void )
{
	unsigned int nUsage = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadCPUUsage(CSUDI_NULL, &nUsage),"步骤一:应该返回失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadCPUUsage(CSUDI_NULL, CSUDI_NULL),"步骤二:应该返回失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

