/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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
	//�ڱ�����������ִ��ǰ����
	CSTC_OS_InitCfg();
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_CS_OSUDI2_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:���Բ���ȫ���Ϸ�ʱ�ܷ�ɹ���������
//@PRECONDITION:�������ȫ���Ϸ�
//@INPUT:1��������="OS_TestThread1"
//@INPUT:2���������ȼ�=7
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= OS_TestThread1Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������OS_TestThread1������CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:2������OS_TestThread1����CSUDIOSThreadSleep �ȴ�10ms��Ȼ��g_bThreadEnd��Ϊtrue������
//@EXECUTIONFLOW:3�����������е���CSUDIOSThreadSleep �ȴ�100ms��Ȼ���ж�g_bThreadEnd��ֵ�������Ϊtrue�򷵻�ʧ��
//@EXECUTIONFLOW:4�����������е���CSUDIOSThreadDestroyɾ�����������񲢷���CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"������������ִ��ʧ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			  hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:��������������32�ֽ�ʱ�ܷ�ɹ���������
//@PRECONDITION:
//@INPUT:1��������="OS_TestThread01234567890123456789"
//@INPUT:2���������ȼ�=64
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= OS_TestThread1Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:���񴴽�ʧ��
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������ʧ�ܷ���CSUDIOS_ERROR_BAD_PARAMETER�������������ʧ��
//@EXECUTIONFLOW:2���ж�������ֵ�����!=CSUDI_NULL����CSUDIOSThreadDestroyɾ�����������񲢷��ز�������ʧ�ܣ�������������ɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0002( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSThreadCreate("OS_TestThread01234567890123456789", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"��������������32�ֽڵ�����Ӧ�÷��ز�������");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hThreadhandle,"����ʧ�ܵ��������������������Ϊ��");
	
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:�������ȼ�Ϊ256ʱ�ܷ�ɹ���������
//@PRECONDITION:
//@INPUT:1��������="OS_TestThread1"
//@INPUT:2���������ȼ�=256
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= OS_TestThread1Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:���񴴽�ʧ��
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������OS_TestThread1ʧ�ܷ��ش������ֵ�������������ʧ��
//@EXECUTIONFLOW:2���ж�������ֵ�����!=CSUDI_NULL����CSUDIOSThreadDestroyɾ�����������񲢷��ز�������ʧ�ܣ�������������ɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0003( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 256;
	int nParam = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"�����������ȼ�Ϊ256������Ӧ�÷���ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hThreadhandle,"����ʧ�ܵ��������������������Ϊ��");
	
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:�������ȼ�Ϊ-1ʱ�ܷ�ɹ���������
//@PRECONDITION:
//@INPUT:1��������="OS_TestThread1"
//@INPUT:2���������ȼ�=-1
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= OS_TestThread1Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:���񴴽�ʧ��
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������OS_TestThread1ʧ�ܷ��ش������ֵ�������������ʧ��
//@EXECUTIONFLOW:2���ж�������ֵ�����!=CSUDI_NULL����CSUDIOSThreadDestroyɾ�����������񲢷��ز�������ʧ�ܣ�������������ɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0004( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = -1;
	int nParam = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"�����������ȼ�Ϊ-1������Ӧ�÷���ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hThreadhandle,"����ʧ�ܵ��������������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate
//@DESCRIPTION:���������ܷ���䵽�����ջ
//@PRECONDITION:
//@INPUT:1��������="OS_TestThread2"
//@INPUT:2���������ȼ�=128
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= OS_TestThread2Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������OS_TestThread2������CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:2����������֮��ʹ��while(!g_bThreadEnd)�ȴ�������ִ�����
//@EXECUTIONFLOW:3��������OS_TestThread2�ж���һ��255KB��int�����鲢��ʼ����Ҫ��ɹ�
//@EXECUTIONFLOW:4��������OS_TestThread2�ж��������һ��Ԫ�ظ�ֵ��0x88888888����ͨ��nPrama�����һ��Ԫ�ص�ֵ����������Ȼ��g_bThreadEnd��Ϊtrue
//@EXECUTIONFLOW:5���������ж��������һ��Ԫ�ص�ֵ�Ƿ����0x88888888�������������ʧ��
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroyɾ��OS_TestThread2��Ӧ��ɾ���ɹ������ɾ��ʧ�����������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0005( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 128;
	char cParam = 'D';
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread2", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread2Entry, &cParam, &hThreadhandle),
			"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

	while(!g_bThreadEnd)
	{
		CSUDIOSThreadSleep(10);
	}
	
	CSTK_ASSERT_TRUE_FATAL(cParam == 'A',"���������޷�ʹ�ô���ʱ�����ջ");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:������ں���Ϊ��ʱ�ܷ�ɹ���������
//@PRECONDITION:
//@INPUT:1��������="OS_TestThread1"
//@INPUT:2���������ȼ�=64
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= CSUDI_NULL
//@INPUT:5��������ں�������= 0
//@EXPECTATION:����ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������OS_TestThread1ʧ�ܷ��ش������ֵ�������������ʧ��
//@EXECUTIONFLOW:2���ж�������ֵ�����!=CSUDI_NULL����CSUDIOSThreadDestroyɾ�����������񲢷��ز�������ʧ�ܣ�������������ɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0006( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 CSUDI_NULL, &nParam, &hThreadhandle),
			"����������ں���Ϊ�յ�����Ӧ�÷���ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hThreadhandle,"����ʧ�ܵ��������������������Ϊ��");
	
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:���Բ����ܷ���������
//@PRECONDITION:���Գɹ���������
//@INPUT:�Ϸ�����
//@EXPECTATION:����������ȷ
//@EXECUTIONFLOW:1�����������CSUDIOSThreadCreate��������������ΪOS_TestThread3_1,���ȼ�64,��ջ��СΪ128kb,��ں���OS_TestThread3_1Entry,������ڲ���Ϊ0x00000000��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2��������OS_TestThread3_1�л�ȡ���ݵĲ����ж��Ƿ����0x00000000���������򽫱���������ڲ���ֵ��Ϊ1
//@EXECUTIONFLOW:3�����������CSUDIOSThreadCreate��������������ΪOS_TestThread3_2,���ȼ�64,��ջ��СΪ128kb,��ں���OS_TestThread3_2Entry,������ڲ���Ϊ0xffffffff��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:4��������OS_TestThread3_2�л�ȡ���ݵĲ����ж��Ƿ����0xffffffff���������򽫱���������ڲ���ֵ��Ϊ2
//@EXECUTIONFLOW:5�����������CSUDIOSThreadCreate��������������ΪOS_TestThread3_3,���ȼ�64,��ջ��СΪ128kb,��ں���OS_TestThread3_3Entry,������ڲ���Ϊ0x8acdefc2��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:6��������OS_TestThread3_3�л�ȡ���ݵĲ����ж��Ƿ����0x8acdefc2���������򽫱���������ڲ���ֵ��Ϊ3
//@EXECUTIONFLOW:7�����������CSUDIOSThreadSleep�ȴ�1����ж����ϸ���������ڲ����Ƿ�ֱ�Ϊ1��2��3�����ȫ����ȷ����Գɹ�������ʧ��
//@EXECUTIONFLOW:8��ɾ�����д�����������ɾ��ʧ�������ʧ��
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
			"��������1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread3_2", nPriority , TEST_THREAD_STACK_SIZE, 
							 OS_TestThread3_2Entry, &nParam2, &hThreadhandle2),
			"��������2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread3_3", nPriority , TEST_THREAD_STACK_SIZE, 
							 OS_TestThread3_3Entry, &nParam3, &hThreadhandle3),
			"��������3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"�����ɹ����������������������Ϊ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(((nParam1 == 1) && (nParam2 == 2) && (nParam3 == 3)), "�������ݴ���");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle1 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������1ʧ��");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������2ʧ��");
			   hThreadhandle2 = CSUDI_NULL;
		}
		if (hThreadhandle3 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "ɾ������3ʧ��");
			   hThreadhandle3 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:����������ΪCSUDI_NULLʱ�ܷ�ɹ���������
//@PRECONDITION:
//@INPUT:1��������=CSUDI_NULL
//@INPUT:2���������ȼ�=7
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= OS_TestThread1Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate�������񲢷���CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:2�����´����������е���CSUDIOSThreadSleep �ȴ�10ms��Ȼ��g_bThreadEnd��Ϊtrue������
//@EXECUTIONFLOW:3�����������е���CSUDIOSThreadSleep �ȴ�100ms��Ȼ���ж�g_bTestPassed��ֵ����Ϊtrue�򷵻سɹ������򷵻�ʧ��
//@EXECUTIONFLOW:4�����������е���CSUDIOSThreadDestroyɾ�����������񲢷���CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0008( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(CSUDI_NULL, nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"������������ִ��ʧ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:����������Ϊ��ʱ�ܷ�ɹ���������
//@PRECONDITION:
//@INPUT:1��������=""
//@INPUT:2���������ȼ�=7
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= OS_TestThread1Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate�������񲢷���CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:2�����´����������е���CSUDIOSThreadSleep �ȴ�10ms��Ȼ��g_bThreadEnd��Ϊtrue������
//@EXECUTIONFLOW:3�����������е���CSUDIOSThreadSleep �ȴ�100ms��Ȼ���ж�g_bTestPassed��ֵ����Ϊtrue�򷵻سɹ������򷵻�ʧ��
//@EXECUTIONFLOW:4�����������е���CSUDIOSThreadDestroyɾ�����������񲢷���CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0009( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"������������ִ��ʧ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:�����ܷ�ɹ�����128��ջ��СΪ8K������
//@PRECONDITION:�������ȫ���Ϸ�
//@INPUT:1��������="OS_TestThread4_1 ~ OS_TestThread4_128"
//@INPUT:2���������ȼ�=64
//@INPUT:3������ջ��С=8KB
//@INPUT:4��������ں���= OS_TestThread4_1Entry ~ OS_TestThread4_128Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1����������int aParam[128]����ʼ����0����g_bThreadEnd��ֵΪfalse
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate����128������ȫ������CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:3����ÿ�������ж���Ӧ������Ԫ�ظ�ֵ������OS_TestThread4_64��aParam[63] = 1��Ȼ��ͨ��while(!g_bThreadEnd)��֤����������ȫ��������
//@EXECUTIONFLOW:4�����������е���CSUDIOSThreadSleep �ȴ�1000ms��Ȼ��g_bThreadEnd��ֵΪtrue
//@EXECUTIONFLOW:5���������ȡ����ֵ����У�飬Ҫ��ȫ��У��ɹ��������������ʧ��
//@EXECUTIONFLOW:6�����������CSUDIOSThreadDestroyɾ������������ȫ������CSUDI_SUCCESS�������������ʧ��
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
				 "��������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle[i],"�����ɹ����������������������Ϊ��");
	}

	CSUDIOSThreadSleep(1000);

	g_bThreadEnd = CSUDI_TRUE;
	
	CSUDIOSThreadSleep(2000);
	
	for (i=0 ; i<128 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(aParam[i] == 1, "У��ʧ��");
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
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle[i]), "ɾ������ʧ��");
				hThreadhandle[i] = CSUDI_NULL;
			}
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:��������ջ��СΪ2Kʱ�ܷ�ɹ���������
//@PRECONDITION:
//@INPUT:1��������="OS_TestThread1"
//@INPUT:2���������ȼ�=7
//@INPUT:3������ջ��С=2KB
//@INPUT:4��������ں���= OS_TestThread1Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate�������񲢷���CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:2�����´����������е���CSUDIOSThreadSleep �ȴ�10ms��Ȼ��g_bThreadEnd��Ϊtrue������
//@EXECUTIONFLOW:3�����������е���CSUDIOSThreadSleep �ȴ�100ms��Ȼ���ж�g_bTestPassed��ֵ����Ϊtrue�򷵻سɹ������򷵻�ʧ��
//@EXECUTIONFLOW:4�����������е���CSUDIOSThreadDestroyɾ�����������񲢷���CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0011( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2048, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"������������ִ��ʧ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@DESCRIPTION:������������ͬʱ�ܷ�ɹ���������
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1�����������CSUDIOSThreadCreate��������������ΪOS_TestThread5�����ȼ�Ϊ64��ջ��СΪ256KB��������ں���ΪOS_TestThread5_1Entry����ں�������Ϊ0��Ҫ�󷵻�CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:2������OS_TestThread5_1Entry����CSUDIOSThreadSleep �ȴ�10ms��ͨ��while(!g_bThreadEnd)�������������whileѭ���򽫴���Ĳ�����Ϊ1
//@EXECUTIONFLOW:3�����������CSUDIOSThreadCreate��������������ΪOS_TestThread5�����ȼ�Ϊ64��ջ��СΪ256KB��������ں���ΪOS_TestThread5_2Entry����ں�������Ϊ0��Ҫ�󷵻�CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:4������OS_TestThread5_2Entry����CSUDIOSThreadSleep �ȴ�10ms��ͨ��while(!g_bThreadEnd)�������������whileѭ���򽫴���Ĳ�����Ϊ1
//@EXECUTIONFLOW:5�����������CSUDIOSThreadSleep �ȴ�50ms��Ȼ��g_bThreadEnd��ֵ��false��Ϊtrue
//@EXECUTIONFLOW:6�����������CSUDIOSThreadSleep �ȴ�50ms���ж�������ں���������ֵ������Ϊ1�򷵻سɹ������򷵻�ʧ��
//@EXECUTIONFLOW:7��Ȼ�����CSUDIOSThreadDestroyɾ�����������񲢷���CSUDI_SUCCESS�������������ʧ��
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
			"��������1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread5", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread5_2Entry, &nParam2, &hThreadhandle2),
			"��������2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��");

	CSUDIOSThreadSleep(50);

	g_bThreadEnd = CSUDI_TRUE;

	CSUDIOSThreadSleep(50);

	CSTK_ASSERT_TRUE_FATAL((nParam1 == 1) ,"����1ִ�г���");
	CSTK_ASSERT_TRUE_FATAL((nParam2 == 1) ,"����2ִ�г���");

	CSTK_FATAL_POINT
	{
		if (g_bThreadEnd == CSUDI_FALSE)
		{
			g_bThreadEnd = CSUDI_TRUE;
			CSUDIOSThreadSleep(50);
		}
		
		if (hThreadhandle1 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������1ʧ��");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������2ʧ��");
			   hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadDestroy  
//@DESCRIPTION:�����ܷ�ɹ�ɾ���������е�����
//@PRECONDITION: ���Գɹ���������OS_TestThread6
//@INPUT: �Ϸ���������
//@EXPECTATION:ɾ��ʧ�ܷ���CSUDIOS_ERROR_THREAD_BUSY
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������������ΪOS_TestThread6,���ȼ�64,��ջ��СΪ256kb,��ں���OS_TestThread6Entry,����Ϊ0��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2������OS_TestThread6��ѭ��ִ��sleep(10)ʮ�� ��Ȼ��g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3�����������CSUDIOSThreadSleep �ȴ�10ms��Ȼ�����CSUDIOSThreadDestroyɾ��OS_TestThread6���񲢷���
//@EXECUTIONFLOW:4���ж�ɾ��ʱ�ķ���ֵ�������CSUDIOS_ERROR_THREAD_BUSY��ɹ�������ʧ��
//@EXECUTIONFLOW:5������CSUDIOSThreadSleep�ȴ�300ms��Ȼ���ж�g_bThreadEnd��ֵ�Ƿ�Ϊtrue������ɹ�������ʧ��
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroyɾ��OS_TestThread6����Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadDestroy_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

	CSUDIOSThreadSleep(10);

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_THREAD_BUSY == CSUDIOSThreadDestroy(hThreadhandle), "ɾ���������е�����Ӧ�÷���CSUDIOS_ERROR_THREAD_BUSY");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"������Ӧ�û���ִ��");
	
	CSUDIOSThreadSleep(300);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"������ִ�г���");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadDestroy  
//@DESCRIPTION:�����ܷ�ɹ�ɾ����Ч����
//@PRECONDITION:
//@INPUT:������=CSUDI_NULL
//@EXPECTATION:ɾ��ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW: 1������CSUDIOSThreadDestroy ɾ����Ч������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadDestroy_0002( void )
{
	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadDestroy(CSUDI_NULL), "������Ϊ��ʱ��ɾ�������ܷ��سɹ�");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadCreate 
//@CASEGROUP:CSUDIOSThreadDestroy
//@DESCRIPTION:����ȫ���Ϸ�ʱ���񴴽������ٵ�ǿ�Ȳ���
//@PRECONDITION:�������ȫ���Ϸ�
//@INPUT:1��������="OS_TestThread1"
//@INPUT:2���������ȼ�=88
//@INPUT:3������ջ��С=256KB
//@INPUT:4��������ں���= OS_TestThread1Entry
//@INPUT:5��������ں�������= 0
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������OS_TestThread1������CSUDI_SUCCESS,�����������ʧ��
//@EXECUTIONFLOW:2������OS_TestThread1����CSUDIOSThreadSleep �ȴ�10ms��Ȼ��g_bThreadEnd��Ϊtrue������
//@EXECUTIONFLOW:3�����������е���CSUDIOSThreadJoin �ȴ�OS_TestThread1������Ȼ���ж�g_bThreadEnd��ֵ�������Ϊtrue�򷵻�ʧ��
//@EXECUTIONFLOW:4�����������е���CSUDIOSThreadDestroyɾ�����������񲢷���CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:5���ظ�ִ��1 ~ 4����OS_TEST_COUNT�Σ�OS_TEST_COUNTͨ�������ļ�����
CSUDI_BOOL CSTC_OS_IT_ThreadCreateDestroy_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 88;
	int nParam = 0;	
	int i;
	
	CSTCPrint("���񴴽�������ǿ�Ȳ��Կ�ʼ����ȴ�������\n");
	
	for(i=0 ; i<OS_TEST_COUNT; i++)
	{
		g_bThreadEnd = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
								 OS_TestThread1Entry, &nParam, &hThreadhandle),
				 "��������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle, "�����ɹ����������������������Ϊ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hThreadhandle), "�ȴ�������ʱ����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "������������ִ��ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");

		hThreadhandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");

			  hThreadhandle = CSUDI_NULL;
		}
	}
	CSTCPrint("���񴴽�������ǿ�Ȳ��Խ���������\n");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSuspend  
//@DESCRIPTION:�����ܷ�ɹ�����һ���������е�����
//@PRECONDITION: ���Գɹ���������OS_TestThread6
//@INPUT: �Ϸ���������
//@EXPECTATION:ƽ̨֧����ɹ��������񣬷��򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������������ΪOS_TestThread6,���ȼ�64,��ջ��СΪ256kb,��ں���OS_TestThread6Entry,����Ϊ0��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2������OS_TestThread6��ѭ��ִ��sleep(10)ʮ�� ��Ȼ��g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3�����������CSUDIOSThreadSuspend����OS_TestThread6���񲢷���
//@EXECUTIONFLOW:4���жϹ���ʱ�ķ���ֵ�����ƽ̨֧����������򷵻سɹ���ִ�в���5~7�����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED��ֱ����������7
//@EXECUTIONFLOW:5���ȴ�500ms���ж�g_bThreadEnd��ֵ��Ҫ��ΪCSUDI_FALSE������˵������û����Ĺ��𣬲���ʧ��
//@EXECUTIONFLOW:6������CSUDIOSThreadResume��������Ȼ��ȴ�150ms�����ж�g_bThreadEnd��ֵ��Ҫ��ΪCSUDI_TRUE
//@EXECUTIONFLOW:7������CSUDIOSThreadDestroyɾ��OS_TestThread6����Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

	if(s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadSuspend(hThreadhandle), "����3��������ʧ��");

		CSUDIOSThreadSleep(500);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadResume(hThreadhandle), "��������ʧ��");

		CSUDIOSThreadSleep(300);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����6ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadSuspend(hThreadhandle), "��������Ӧ�÷���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "����7ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSuspend  
//@DESCRIPTION:����ΪCSUDI_NULL�Ĳ���
//@PRECONDITION: 
//@INPUT: CSUDI_NULL
//@EXPECTATION:ƽ̨֧���򷵻�ʧ�ܣ����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1������CSUDIOSThreadSuspend����������Ϊ�յ�����ƽ̨֧��Ҫ�󷵻�ʧ�ܣ����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0002( void )
{
	if(s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadSuspend(CSUDI_NULL), "����1ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadSuspend(CSUDI_NULL), "����1ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSuspend  
//@DESCRIPTION:�����ܷ�ɹ�����һ���Ѿ����������
//@PRECONDITION: ���Գɹ���������OS_TestThread6
//@INPUT: �Ϸ���������
//@EXPECTATION:ƽ̨֧����ɹ��������񣬷��򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������������ΪOS_TestThread6,���ȼ�64,��ջ��СΪ256kb,��ں���OS_TestThread6Entry,����Ϊ0��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2������OS_TestThread6��ѭ��ִ��sleep(10)ʮ�� ��Ȼ��g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3�����������CSUDIOSThreadSuspend����OS_TestThread6���񲢷���
//@EXECUTIONFLOW:4���жϹ���ʱ�ķ���ֵ�����ƽ̨֧����������򷵻سɹ���ִ�в���5~8�����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED��ֱ����������7
//@EXECUTIONFLOW:5������CSUDIOSThreadSuspend�ٴι���OS_TestThread6���񲢷��سɹ�
//@EXECUTIONFLOW:6���ȴ�300ms���ж�g_bThreadEnd��ֵ��Ҫ��ΪCSUDI_FALSE������˵������û����Ĺ��𣬲���ʧ��
//@EXECUTIONFLOW:7������CSUDIOSThreadResume��������Ȼ��ȴ�150ms�����ж�g_bThreadEnd��ֵ��Ҫ��ΪCSUDI_TRUE
//@EXECUTIONFLOW:8������CSUDIOSThreadSuspend����һ���Ѿ����з��ص�����Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:9������CSUDIOSThreadDestroyɾ��OS_TestThread6����Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0003( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

	if(s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadSuspend(hThreadhandle), "����3��������ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadSuspend(hThreadhandle), "����5ʧ��");

		CSUDIOSThreadSleep(300);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadResume(hThreadhandle), "��������ʧ��");
		
		CSUDIOSThreadSleep(300);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadSuspend(hThreadhandle), "����8ʧ��");		
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadSuspend(hThreadhandle), "��������Ӧ�÷���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "����9ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSuspend  
//@DESCRIPTION:�����ܷ�ɹ�����һ���Ѿ����ص�����
//@PRECONDITION: ���Գɹ���������OS_TestThread1
//@INPUT: �Ϸ���������
//@EXPECTATION:ƽ̨֧���򷵻�ʧ�ܣ����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������������ΪOS_TestThread1,���ȼ�64,��ջ��СΪ256kb,��ں���OS_TestThread1Entry,����Ϊ0��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2������OS_TestThread1�еȴ�10ms��Ȼ��g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3�����������CSUDIOSThreadSleep �ȴ�50ms��Ȼ�����CSUDIOSThreadSuspend����OS_TestThread1���񲢷���
//@EXECUTIONFLOW:4���жϹ���ʱ�ķ���ֵ�����ƽ̨֧����������򷵻�ʧ�ܣ����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5���ж�g_bThreadEnd��ֵҪ��ΪCSUDI_TRUE
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroyɾ��OS_TestThread6����Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0004( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

	if(s_bSupportSuspend)
	{
		CSUDIOSThreadSleep(50);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadSuspend(hThreadhandle), "����3��������Ӧ�÷���ʧ��");
	}
	else
	{
		CSUDIOSThreadSleep(50);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadSuspend(hThreadhandle), "��������Ӧ�÷���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����5ʧ��");
		
	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			if(g_bThreadEnd != CSUDI_TRUE)
			{
				CSUDIOSThreadSleep(50);
			}
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadResume 
//@DESCRIPTION:�����ܷ�ɹ�����һ���������е�����
//@PRECONDITION: ���Գɹ���������OS_TestThread6
//@INPUT: �Ϸ���������
//@EXPECTATION:ƽ̨֧���򷵻سɹ������򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������������ΪOS_TestThread6,���ȼ�64,��ջ��СΪ256kb,��ں���OS_TestThread6Entry,����Ϊ0��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2������OS_TestThread6��ѭ��ִ��sleep(10)ʮ�� ��Ȼ��g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3�����������CSUDIOSThreadResume����OS_TestThread6���񲢷���
//@EXECUTIONFLOW:4���жϻ���ʱ�ķ���ֵ�����ƽ̨֧���򷵻سɹ������򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5������CSUDIOSThreadSleep�ȴ�300ms��Ȼ���ж�g_bThreadEnd��ֵ�Ƿ�Ϊtrue������ɹ�������ʧ��
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroyɾ��OS_TestThread6����Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

	if(s_bSupportSuspend)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadResume(hThreadhandle), "��������ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadResume(hThreadhandle), "��������Ӧ�÷���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
	}

	CSUDIOSThreadSleep(300);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"������ִ�г���");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			if(g_bThreadEnd != CSUDI_TRUE)
			{
				CSUDIOSThreadSleep(300);
			}
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadResume 
//@DESCRIPTION:�����ܷ�ɹ�����һ���Ѿ����ص�����
//@PRECONDITION: ���Գɹ���������OS_TestThread1
//@INPUT: �Ϸ���������
//@EXPECTATION:ƽ̨֧���򷵻�ʧ�ܣ����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������������ΪOS_TestThread1,���ȼ�64,��ջ��СΪ256kb,��ں���OS_TestThread1Entry,����Ϊ0��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2������OS_TestThread1�ȴ�10ms ��Ȼ��g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3��������ȴ�50ns�����CSUDIOSThreadResume����OS_TestThread1���񲢷���
//@EXECUTIONFLOW:4���жϻ���ʱ�ķ���ֵ�����ƽ̨֧���򷵻�ʧ�ܣ����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5���ж�g_bThreadEnd��ֵҪ��ΪCSUDI_TRUE
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroyɾ��OS_TestThread6����Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0002( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread1Entry, &nParam, &hThreadhandle),
			"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

	if(s_bSupportSuspend)
	{		
		CSUDIOSThreadSleep(50);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadResume(hThreadhandle), "��������ʧ��");
	}
	else
	{
		CSUDIOSThreadSleep(50);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadResume(hThreadhandle), "��������Ӧ�÷���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����5ʧ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			if(g_bThreadEnd != CSUDI_TRUE)
			{
				CSUDIOSThreadSleep(50);
			}
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadResume  
//@DESCRIPTION:����ΪCSUDI_NULL�Ĳ���
//@PRECONDITION: 
//@INPUT: CSUDI_NULL
//@EXPECTATION:ƽ̨֧���򷵻�ʧ�ܣ����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:1������CSUDIOSThreadResume����������Ϊ�յ�����ƽ̨֧��Ҫ�󷵻�ʧ�ܣ����򷵻�CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0003( void )
{
	if(s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadResume(CSUDI_NULL), "����1ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED == CSUDIOSThreadResume(CSUDI_NULL), "����1ʧ��");
	}
	
	CSTK_FATAL_POINT

	return CSUDI_TRUE; 
}

//@CASEGROUP:CSUDIOSThreadJoin 
//@DESCRIPTION:����һ������ȴ���һ���������е������Ƿ�ɹ�
//@PRECONDITION:���Գɹ���������OS_TestThread6
//@INPUT:OS_TestThread6��������
//@EXPECTATION:���Եȴ��ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1�����������CSUDIOSThreadCreate��������������ΪOS_TestThread6,���ȼ�64,��ջ��СΪ256kb,��ں���OS_TestThread6Entry,����Ϊ0��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2������OS_TestThread6��ѭ��ִ��sleep(10)ʮ�� ��Ȼ��g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3�����������CSUDIOSThreadJoin�ȴ�OS_TestThread5������ȴ��ɹ��ж�g_bThreadEnd�Ƿ����true�������򷵻سɹ�������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSThreadDestroyɾ���մ��������񲢷���CSUDI_SUCCESS������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadJoin_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 64;
	int nParam = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread6", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							 OS_TestThread6Entry, &nParam, &hThreadhandle),
			"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hThreadhandle), "����������ЧʱӦ�õȴ��ɹ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"������ִ�г���");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			  hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadJoin 
//@DESCRIPTION:����һ������ȴ���һ���Ѿ����ص������Ƿ�ɹ�
//@PRECONDITION:1�����Գɹ���������OS_TestThread1
//@INPUT:OS_TestThread1��������
//@EXPECTATION:���Գɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������������ΪOS_TestThread1,���ȼ�7,��ջ��СΪ256kb,��ں���OS_TestThread1Entry,����Ϊ0��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2������OS_TestThread1����CSUDIOSThreadSleep �ȴ�10ms��Ȼ��g_bThreadEnd��Ϊtrue������
//@EXECUTIONFLOW:3�����������е���CSUDIOSThreadSleep�ȴ�100ms��Ȼ�����CSUDIOSThreadJoin�ȴ�OS_TestThread1
//@EXECUTIONFLOW:4���жϲ���3�ķ���ֵ�������CSUDI_SUCCESS����ɾ������OS_TestThread1�����سɹ�������ɾ�����������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadJoin_0002( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	
	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),
			 "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hThreadhandle), "����������ЧʱӦ�õȴ��ɹ�");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSelf  
//@DESCRIPTION:�����ܷ���ȷ��ȡһ������ľ��
//@PRECONDITION:���Գɹ���������OS_TestThread7
//@INPUT:
//@EXPECTATION:���Գɹ���ȡ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������������ΪOS_TestThread7,���ȼ�7,��ջ��СΪ256kb,��ں���OS_TestThread7Entry,����ΪCSUDI_NULL��Ҫ�󴴽��ɹ�
//@EXECUTIONFLOW:2����OS_TestThread7����CSUDIOSThreadSelf��ȡ��������ľ�������з���
//@EXECUTIONFLOW:3���жϻ�ȡ���������ʹ���ʱ���������Ƿ���ȣ���������������ʧ��
//@EXECUTIONFLOW:4��ɾ������������Ҫ��ɾ���ɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadSelf_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	CSUDI_HANDLE hParamhandle = CSUDI_NULL;	
	int nPriority = 7;

	g_bThreadEnd = CSUDI_FALSE;

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread7", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread7Entry, &hParamhandle, &hThreadhandle),
			 "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");
	
	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(hParamhandle == hThreadhandle,"��ȡ�������������ɵĲ�һ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			   hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSelf
//@CASEGROUP:CSUDIOSThreadJoin 
//@DESCRIPTION:���Բ���ΪCSUDI_NULL��ʱ�򣬺�����ִ�н��
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSThreadSelf���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSThreadJoin���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_ThreadSelfJoin_0001( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadSelf(CSUDI_NULL), "����1ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSThreadJoin(CSUDI_NULL), "����2ʧ��");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadSleep  
//@DESCRIPTION:���Խ�һ���������һ��ʱ���׼ȷ��
//@PRECONDITION:
//@INPUT:1000
//@EXPECTATION:�ݴ�������10ms
//@EXECUTIONFLOW:1������CSUDIOSGetTime ��ȡ��ǰϵͳʱ�䲢���سɹ�
//@EXECUTIONFLOW:2������CSUDIOSThreadSleep�ȴ�1000ms
//@EXECUTIONFLOW:3������CSUDIOSGetTime �ٴλ�ȡ��ǰϵͳʱ�䲢���سɹ�
//@EXECUTIONFLOW:4���ж����λ�ȡϵͳʱ��Ĳ�ֵ�͵ȴ�ʱ��Ĳ����ݴ�������10ms�򷵻سɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadSleep_0001( void )
{
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "��һ�λ�ȡϵͳʱ��ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "�ڶ��λ�ȡϵͳʱ��ʧ��");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 1000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-10) && (nMiliSecond <=10), "�ȴ���ʱ�䲻��ϵͳ�ݴ�ֵ��");
	
	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSThreadYield  
//@DESCRIPTION:���Ե���CSUDIOSThreadYield�ӿ��ܷ��������ִ��Ȩ
//@PRECONDITION:���Գɹ���������
//@INPUT:
//@EXPECTATION:���Է�������ִ��Ȩ
//@EXECUTIONFLOW:1������CSUDIThreadCreate ��������������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:2��������1�����ȼ�Ϊ255�ĸ����ȼ������ڴ�������һֱ����CSUDIOSThreadYield�ӿڷ���ִ��Ȩ
//@EXECUTIONFLOW:3��������2�����ȼ�Ϊ0�ĵ����ȼ����񣬸�����Ϊһ��10��Ԫ�ص����鸳ֵ�����������ִ������g_bThreadEnd��ֵΪtrue
//@EXECUTIONFLOW:4���������ж�g_bThreadEnd��ֵΪtrue�����ж�����2ִ�����õ�ʱ�䣬Ҫ��Ϊ1���ڣ��������ʧ��
//@EXECUTIONFLOW:5��ɾ������������Ҫ��ɾ���ɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_ThreadYield_0001( void )
{
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread8_1",255,8*1024,OS_TestThread8_1Entry,CSUDI_NULL,&hThreadhandle1),"����������1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"������1�������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread8_2",0,8*1024,OS_TestThread8_2Entry,CSUDI_NULL,&hThreadhandle2),"����������2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"������2�������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "��һ�λ�ȡϵͳʱ��ʧ��");

	while(g_bThreadEnd == CSUDI_FALSE)
	{
		CSUDIOSThreadSleep(10);
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "�ڶ��λ�ȡϵͳʱ��ʧ��");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond); 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond <=1000), "����2û�ڹ涨ʱ����ִ����");

	CSUDIOSThreadSleep(20);
	
	CSTK_FATAL_POINT
	{
		if(CSUDI_NULL != hThreadhandle1)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1),"ɾ������1ʧ��");
			hThreadhandle1 = CSUDI_NULL;
		}
		
		if(CSUDI_NULL != hThreadhandle2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2),"ɾ������2ʧ��");
			hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:�����ܷ�ɹ�������Ϣ����
//@PRECONDITION:�������ȫ���Ϸ�
//@INPUT:1����Ϣ��������="OS_TestMsgQueue1"
//@INPUT:2�������Ϣ����=32
//@INPUT:3����Ϣ��󳤶�=64
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����OS_TestMsgQueue1Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ӧ�����ٳɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0001( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:���Զ���������32�ֽ�ʱ�ܷ�ɹ�������Ϣ����
//@PRECONDITION:
//@INPUT:1����Ϣ��������="OS_TestMsgQueue01234567890123456789"
//@INPUT:2�������Ϣ����=32
//@INPUT:3����Ϣ��󳤶�=64
//@EXPECTATION:����ʧ�ܲ�����CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����OS_TestMsgQueue01234567890123456789������
//@EXECUTIONFLOW:2���жϲ���1�ķ���ֵ��Ӧ�õ���CSUDIOS_ERROR_BAD_PARAMETER������ʧ��
//@EXECUTIONFLOW:3��������о���ǿ������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ�����ҷ���ʧ�ܣ����򷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0002( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSMsgQueueCreate("OS_TestMsgQueue01234567890123456789", nMaxMsgs, nSizePerMsg, &hMessageQHandle),
		"��������������32�ֽڵ���Ϣ����Ӧ�÷��ز�������");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hMessageQHandle, "����ʧ�ܵ���Ϣ��������ľ������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:���Զ�����CSUDI_NULLʱ�ܷ�ɹ�������Ϣ����
//@PRECONDITION:
//@INPUT:1����Ϣ��������=CSUDI_NULL
//@INPUT:2�������Ϣ����=32
//@INPUT:3����Ϣ��󳤶�=64
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate��������Ϊ�յ���Ϣ����Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ӧ�����ٳɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0003( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate(CSUDI_NULL,nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:���������Ϣ����Ϊ0ʱ�ܷ�ɹ�������Ϣ����
//@PRECONDITION:
//@INPUT:1����Ϣ��������="OS_TestMsgQueue1"
//@INPUT:2�������Ϣ����=0
//@INPUT:3����Ϣ��󳤶�=64
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)����Ϣ���о��ΪCSUDI_NULL
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����OS_TestMsgQueue1�����ش������ֵ
//@EXECUTIONFLOW:2���жϲ���1�ķ���ֵ��Ӧ�ò�����CSUDI_SUCCESS������ʧ��
//@EXECUTIONFLOW:3��������о���ǿ������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ�����ҷ���ʧ�ܣ����򷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0004( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 0;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueCreate("OS_TestMsgQueue1", nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"���������Ϣ����Ϊ0����Ϣ����Ӧ�÷���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hMessageQHandle, "����ʧ�ܵ���Ϣ��������ľ������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:���������Ϣ����Ϊ128ʱ�ܷ�ɹ�������Ϣ����
//@PRECONDITION:
//@INPUT:1����Ϣ��������="OS_TestMsgQueue1"
//@INPUT:2�������Ϣ����=128
//@INPUT:3����Ϣ��󳤶�=64
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����OS_TestMsgQueue1Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ӧ�����ٳɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0005( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 128;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:������Ϣ��󳤶�Ϊ0ʱ�ܷ�ɹ�������Ϣ����
//@PRECONDITION:
//@INPUT:1����Ϣ��������="OS_TestMsgQueue1"
//@INPUT:2�������Ϣ����=32
//@INPUT:3����Ϣ��󳤶�=0
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)����Ϣ���о��ΪCSUDI_NULL
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����OS_TestMsgQueue1Ҫ�󷵻ط�CSUDI_SUCCESS�Ĵ������ֵ
//@EXECUTIONFLOW:2��������о���ǿ������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ�����ҷ���ʧ�ܣ����򷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0006( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ��󳤶�Ϊ0����Ϣ����Ӧ�÷���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hMessageQHandle,"����ʧ�ܵ���Ϣ��������ľ������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:������Ϣ��󳤶�Ϊ128ʱ�ܷ�ɹ�������Ϣ����
//@PRECONDITION:
//@INPUT:1����Ϣ��������="OS_TestMsgQueue1"
//@INPUT:2�������Ϣ����=64
//@INPUT:3����Ϣ��󳤶�=128
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����OS_TestMsgQueue1Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ӧ�����ٳɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0007( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 64;
	int nSizePerMsg = 128;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			 hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:�����ܷ�ɹ�����64����Ϣ����
//@PRECONDITION:�������ȫ���Ϸ�
//@INPUT:1����Ϣ��������="OS_TestMsgQueue1_1 ~ OS_TestMsgQueue1_64"
//@INPUT:2�������Ϣ����=32
//@INPUT:3����Ϣ��󳤶�=64
//@EXPECTATION:ȫ�������ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����OS_TestMsgQueue1_1 ~ OS_TestMsgQueue1_64Ҫ��ȫ������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueDestroy�������д�������Ϣ���У�Ӧ��ȫ�����ٳɹ��������������ʧ��
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate(aacMsgQName[i],nMaxMsgs,nSizePerMsg,&hMessageQHandle[i]), "������Ϣ����ʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<64;i++)
		{
			if(hMessageQHandle[i] != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle[i]), "������Ϣ����ʧ��");
				hMessageQHandle[i] = CSUDI_NULL;
			}
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueCreate  
//@DESCRIPTION:������Ϣ����������ͬʱ�ܷ�ɹ�������Ϣ����
//@PRECONDITION:�������ȫ���Ϸ�
//@INPUT:1����Ϣ��������="OS_TestMsgQueue1"
//@INPUT:2�������Ϣ����=32
//@INPUT:3����Ϣ��󳤶�=64
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����OS_TestMsgQueue1Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2���ٴε���CSUDIOSMsgQueueCreate������Ϣ��������ΪOS_TestMsgQueue1����Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ӧ�����ٳɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0009( void )
{
	CSUDI_HANDLE hMessageQHandle1 = CSUDI_NULL;
	CSUDI_HANDLE hMessageQHandle2 = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle1),
		"������Ϣ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle1,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle2),
		"������Ϣ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle2,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hMessageQHandle1 != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle1), "������Ϣ����ʧ��");
			 hMessageQHandle1 = CSUDI_NULL;
		}

		if(hMessageQHandle2 != CSUDI_NULL)
		{
			 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle2), "������Ϣ����ʧ��");
			 hMessageQHandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueDestroy   
//@DESCRIPTION:�����ܷ�ɹ�ɾ����Ч����Ϣ����
//@PRECONDITION:
//@INPUT: CSUDI_NULL
//@EXPECTATION:ɾ��ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueDestroy,�������ΪCSUDI_NULL,�������ط�CSUDI_SUCCESS��
CSUDI_BOOL CSTC_OS_IT_MsgQueueDestroy_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueDestroy(CSUDI_NULL), "��Ϣ����Ϊ��ʱ��ɾ�����ܷ��سɹ�");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:�����ܷ�ɹ�����Ϣ�����﷢����Ϣ
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����Ҫ���͵���Ϣ����0x88888888
//@INPUT:3����ַ������ڴ泤��=4
//@INPUT:4���ȴ���ʱʱ��=500ms 
//@EXPECTATION:���ͳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ32����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ���У���ʱʱ��Ϊ500ms ��Ҫ���ͳɹ�
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive����Ϣ�����н���һ�����ݣ���ʱʱ��Ϊ500ms ��Ҫ����ճɹ�
//@EXECUTIONFLOW:4���жϽ��յ������ݺͷ��͵��Ƿ���ȣ��������������ʧ��
//@EXECUTIONFLOW:5������CSUDIOSMsgQueueDestroy���ٸմ�������Ϣ���в�����CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0001( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nSendMsg = 0x88888888;
	int nMsgBytes = 4;
	int anRecvMsg[1];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nSendMsg,nMsgBytes,500), "������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,nMsgBytes,500), "������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL((anRecvMsg[0] == 0x88888888), "���յ�����Ϣ�ͷ��͵Ĳ����");  

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:�����ܷ�ɹ�����Ч��Ϣ�����﷢����Ϣ
//@PRECONDITION:
//@INPUT:1����Ч����Ϣ���о��(CSUDI_NULL)
//@INPUT:2����Ҫ���͵���Ϣ����0x88888888
//@INPUT:3����ַ������ڴ泤��=4
//@INPUT:4���ȴ���ʱʱ��=500ms 
//@EXPECTATION:����ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueSend����һ����Ϣ����Ч��Ϣ����
//@EXECUTIONFLOW:2���ж�CSUDIOSMsgQueueSend�ķ���ֵ��������CSUDI_SUCCESS�򷵻سɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0002( void )
{
	int nMsg = 0x88888888;
	int nMsgBytes = 4;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueSend(CSUDI_NULL,&nMsg,nMsgBytes,500), "����Ч����Ϣ�����з�����Ϣ���ܳɹ�");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:�������ݵ�ַΪ��ʱ�ܷ�ɹ�����Ϣ�����﷢����Ϣ
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2��������Ϣ���ݵ��ڴ��ַ=CSUDI_NULL
//@INPUT:3����ַ������ڴ泤��=4
//@INPUT:4���ȴ���ʱʱ��=500ms 
//@EXPECTATION:����ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ32����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ�����ݵ�ַΪ�յ���Ϣ����Ϣ����OS_TestMsgQueue1
//@EXECUTIONFLOW:3���ж�CSUDIOSMsgQueueSend�ķ���ֵ��������CSUDI_SUCCESS�򷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0003( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nMsgBytes = 4;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueSend(hMessageQHandle,CSUDI_NULL,nMsgBytes,500), "���ݵ�ַΪ��ʱ������Ϣ���ܳɹ�");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:�����ڴ泤��Ϊ0ʱ�ܷ�ɹ�����Ϣ�����﷢����Ϣ
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����Ҫ���͵���Ϣ����0x88888888
//@INPUT:3����ַ������ڴ泤��=0
//@INPUT:4���ȴ���ʱʱ��=500ms 
//@EXPECTATION:����ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ32����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ����OS_TestMsgQueue1
//@EXECUTIONFLOW:3���ж�CSUDIOSMsgQueueSend�ķ���ֵ��������CSUDI_SUCCESS�򷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0004( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,0,500), "�ڴ泤��Ϊ0ʱ������Ϣ���ܳɹ�");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:���Ը��ֳ�ʱ��������ܷ�ɹ�����Ϣ�����﷢����Ϣ
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����Ҫ���͵���Ϣ����0x88888888
//@INPUT:3����ַ������ڴ泤��=4
//@INPUT:4���ȴ���ʱʱ��:ִ�в���������
//@EXPECTATION:ÿһ�����ﵽ����ֵ
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ1����Ϣ��󳤶�Ϊ8��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ����OS_TestMsgQueue1����ʱʱ��CSUDIOS_TIMEOUT_IMMEDIATE
//@EXECUTIONFLOW:3���ж�CSUDIOSMsgQueueSend�ķ���ֵ������CSUDI_SUCCESS�򷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueSend�ٷ���һ����Ϣ����Ϣ����OS_TestMsgQueue1��ʱʱ��CSUDIOS_TIMEOUT_IMMEDIATE
//@EXECUTIONFLOW:5���ж�CSUDIOSMsgQueueSend�ķ���ֵ������CSUDIOS_ERROR_TIMEOUT�򷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:6������CSUDIOSMsgQueueSend�ٷ���һ����Ϣ����Ϣ����OS_TestMsgQueue1��ʱʱ��100ms
//@EXECUTIONFLOW:7���ж�CSUDIOSMsgQueueSend�ķ���ֵ������CSUDIOS_ERROR_TIMEOUT�򷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:8������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0005( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 8;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "����������Ϣ�����з�����ϢӦ�ó�ʱ");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,100), "����������Ϣ�����з�����ϢӦ�ó�ʱ");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:���Ե����͵���Ϣ���ȳ�����Ϣ����ÿ����Ϣ��󳤶�ʱ�ܷ��ͳɹ�
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����Ҫ���͵���Ϣ����char acSendMsg[5] = "abcd"
//@INPUT:3����ַ������ڴ泤��=4
//@INPUT:4���ȴ���ʱʱ��=CSUDIOS_TIMEOUT_IMMEDIATE 
//@EXPECTATION:ÿһ�����ﵽ����ֵ
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ1����Ϣ��󳤶�Ϊ2��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ����OS_TestMsgQueue1����ʱʱ��CSUDIOS_TIMEOUT_IMMEDIATE��������Ϣ����Ϊ4,Ӧ�óɹ�
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive����Ϣ�����н���һ����Ϣ�����ճ���Ϊ4��Ҫ��ɹ���ʵ�ʽ��յ�������Ϊ2�ֽڼ�"ab"
//@EXECUTIONFLOW:4������strlen�жϽ��յ�����Ϣ���ȣ�Ӧ��Ϊ2������ʧ��
//@EXECUTIONFLOW:5������strncmp�����ͺͽ��յ���Ϣǰ2���ֽڽ��бȽ�Ӧ����ȣ�����ʧ��
//@EXECUTIONFLOW:6������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
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
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,acSendMsg,nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,acRecvMsg,nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL((acRecvMsg[0] == 'a') && (acRecvMsg[1] == 'b') && (acRecvMsg[2] == 'F'), "���յ�����Ϣ��Ԥ�ڲ�ͬ");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueSend   
//@DESCRIPTION:���Ե���ʱ����ΪCSUDIOS_TIMEOUT_INFINITYʱ��CSUDIOSMsgQueueSend��ִ�н��
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����Ҫ���͵���Ϣ����char acSendMsg[5] = "abcd"
//@INPUT:3����ַ������ڴ泤��=4
//@INPUT:4���ȴ���ʱʱ��=CSUDIOS_TIMEOUT_INFINITY 
//@EXPECTATION:ÿһ�����ﵽ����ֵ
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ1����Ϣ��󳤶�Ϊ4��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ����OS_TestMsgQueue1����ʱʱ��CSUDIOS_TIMEOUT_INFINITY��������Ϣ����Ϊ4��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate���������OS_TestMsgQThread3Entry�еȴ�10s�����CSUDIOSMsgQueueReceive������g_bThreadEnd��ֵΪCSUDI_TRUE
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueSend�ٷ���һ����Ϣ����Ϣ����OS_TestMsgQueue1����ʱʱ��CSUDIOS_TIMEOUT_INFINITY��������Ϣ����Ϊ4��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ж�g_bThreadEnd������ΪCSUDI_TRUE
//@EXECUTIONFLOW:6������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIOSThreadDestroy���ٲ���3�������̣߳�Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0007( void )
{
    CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    int nPriority = 7;
    int nMaxMsgs = 1;
    int nSizePerMsg = 4;
    int nMsgBytes = 5;

    char acSendMsg[5] = "abcd";
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1", nMaxMsgs, nSizePerMsg, &hMessageQHandle), "����1��������Ϣ����ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle, "�����ɹ�����Ϣ��������������Ϊ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle, acSendMsg, nMsgBytes, CSUDIOS_TIMEOUT_INFINITY), "����2��������Ϣʧ��");

    g_bThreadEnd = CSUDI_FALSE;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread", nPriority, TEST_THREAD_STACK_SIZE, 
                            OS_TestMsgQThread3Entry, hMessageQHandle, &hThreadhandle), "����3����������ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle, "�����ɹ����������������������Ϊ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle, acSendMsg, nMsgBytes, CSUDIOS_TIMEOUT_INFINITY), "����4��������Ϣʧ��");

    CSUDIOSThreadSleep(100);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "����4ʧ�ܣ�û�еȴ�10s�ͷ���");

    CSTK_FATAL_POINT
   
    if (hMessageQHandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "����6��������Ϣ����ʧ��");
        hMessageQHandle = CSUDI_NULL;
    }

	if(hThreadhandle != CSUDI_NULL)
    {           
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "����7��ɾ������ʧ��");
        hThreadhandle = CSUDI_NULL;
    }
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:�����ܷ�ɹ�����Ч����Ϣ�����������Ϣ
//@PRECONDITION:
//@INPUT:1����Ч����Ϣ���о��(CSUDI_NULL)
//@INPUT:2����ַ������ڴ泤��=4
//@INPUT:3���ȴ���ʱʱ��=500ms 
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueReceive����Ч����Ϣ�����н�����Ϣ��Ҫ�󷵻ش������
//@EXECUTIONFLOW:2���ж�CSUDIOSMsgQueueReceive�ķ���ֵ��������CSUDI_SUCCESS�򷵻سɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0001( void )
{
	int anRecvMsg[1];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueReceive(CSUDI_NULL,anRecvMsg,4,500), "����Ч����Ϣ�����н�����ϢӦ�÷���ʧ��");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:���Ե�������Ϣ���ݵ��ڴ��ַΪ��ʱ�ܷ�ɹ�������Ϣ����Ϣ�����������Ϣ
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����ַ������ڴ泤��=4
//@INPUT:3���ȴ���ʱʱ��=500ms 
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ32����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ���У���ʱʱ��Ϊ500ms��Ҫ���ͳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive����Ϣ�����н���һ����Ϣ����ʱʱ��Ϊ500ms��Ҫ����ս���ʧ�ܲ����ش������ֵ
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueDestroy���ٸմ�������Ϣ���в�����CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0002( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,500), "������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueReceive(hMessageQHandle,CSUDI_NULL,nMsgBytes,500), "������Ϣ���ݵ��ڴ��ַΪ��ʱӦ�ý���ʧ��");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:���Ե���ַ������ڴ泤��Ϊ0ʱ�ܷ�ɹ�������Ϣ����Ϣ�����������Ϣ
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����ַ������ڴ泤��=0
//@INPUT:3���ȴ���ʱʱ��=500ms 
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ32����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ���У�Ҫ���ͳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive����Ϣ�����н���һ����Ϣ��Ҫ����ս���ʧ�ܲ����ش������ֵ
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueDestroy���ٸմ�������Ϣ���в�����CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0003( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 32;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	int anRecvMsg[1];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,500), "������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,0,500), "������Ϣ���ݵ��ڴ��ַ����Ϊ0ʱӦ�ý���ʧ��");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:������Ϣ����Ϊ1�����η�����Ϣ�Ƿ��ܷ���CSUDIOS_ERROR_TIMEOUT
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����Ϣ����=1
//@INPUT:3���ȴ���ʱʱ��=0ms 
//@EXPECTATION:����ʧ�ܲ�����CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ1����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ���У�Ҫ���ͳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ���У�Ҫ����ʧ�ܲ�����CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueDestroy���ٸմ�������Ϣ���в�����CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0004( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	//int anRecvMsg[1];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,0), "��һ�η�����Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,0), "�ڶ��η�����ϢӦ�÷���CSUDIOS_ERROR_TIMEOUT");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive    
//@DESCRIPTION:������Ϣ����Ϊ�����ν�����Ϣ�Ƿ��ܷ���CSUDIOS_ERROR_TIMEOUT
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����Ϣ����=1
//@INPUT:3���ȴ���ʱʱ��=0ms 
//@EXPECTATION:����ʧ�ܲ�����CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ1����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ���У�Ҫ���ͳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive����Ϣ�����н���һ����Ϣ��Ҫ����ճɹ�
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueReceive����Ϣ�����н���һ����Ϣ��Ҫ�����ʧ�ܲ�����CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:5������CSUDIOSMsgQueueDestroy���ٸմ�������Ϣ���в�����CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0005( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 64;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	int anRecvMsg[1];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,500), "��һ�η�����Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,nMsgBytes,0), "��һ�ν�����Ϣ����ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT== CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,nMsgBytes,0), "�ڶ��ν�����Ϣ����Ӧ�÷���CSUDIOS_ERROR_TIMEOUT");

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMsgQueueReceive   
//@DESCRIPTION:���Ե���ʱ����ΪCSUDIOS_TIMEOUT_INFINITYʱ��CSUDIOSMsgQueueReceive��ִ�н��
//@PRECONDITION:���Գɹ�������Ϣ����OS_TestMsgQueue1
//@INPUT:1���ɹ���������Ϣ����OS_TestMsgQueue1�ľ��
//@INPUT:2����Ҫ���͵���Ϣ����char acSendMsg[5] = "abcd"
//@INPUT:3����ַ������ڴ泤��=4
//@INPUT:4���ȴ���ʱʱ��=CSUDIOS_TIMEOUT_INFINITY 
//@EXPECTATION:ÿһ�����ﵽ����ֵ
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ1����Ϣ��󳤶�Ϊ4��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate���������OS_TestMsgQThread4Entry�еȴ�10s�����CSUDIOSMsgQueueSend������g_bThreadEnd��ֵΪCSUDI_TRUE
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive������Ϣ����ʱʱ��CSUDIOS_TIMEOUT_INFINITY��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ж�g_bThreadEnd������ΪCSUDI_TRUE
//@EXECUTIONFLOW:5������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroy���ٲ���3�������̣߳�Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0006( void )
{
    CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    int nPriority = 7;
    int nMaxMsgs = 1;
    int nSizePerMsg = 4;
    int nMsgBytes = 5;
    char acRecvMsg[5] = {0};

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1", nMaxMsgs, nSizePerMsg, &hMessageQHandle), "����1��������Ϣ����ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle, "�����ɹ�����Ϣ��������������Ϊ��");

    g_bThreadEnd = CSUDI_FALSE;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread", nPriority, TEST_THREAD_STACK_SIZE, 
                            OS_TestMsgQThread4Entry, hMessageQHandle, &hThreadhandle), "����2����������ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle, "�����ɹ����������������������Ϊ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle, acRecvMsg, nMsgBytes, CSUDIOS_TIMEOUT_INFINITY), "����3��������Ϣʧ��");

    CSUDIOSThreadSleep(100);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "����4ʧ�ܣ�û�еȴ�10s�ͷ���");

    CSTK_FATAL_POINT
   
    if (hMessageQHandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "����5��������Ϣ����ʧ��");
        hMessageQHandle = CSUDI_NULL;
    }

    if(hThreadhandle != CSUDI_NULL)
    {           
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "����6��ɾ������ʧ��");
        hThreadhandle = CSUDI_NULL;
    }
    return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMsgQueueCreate
//@CASEGROUP:2��CSUDIOSMsgQueueSend
//@CASEGROUP:3��CSUDIOSMsgQueueReceive
//@CASEGROUP:4��CSUDIOSMsgQueueDestroy
//@DESCRIPTION:���Ե����͵���Ϣ����Ϊ�����Ϣ����ʱ�ܷ��ͳɹ�
//@DESCRIPTION:���Է��ͺͽ��ܵ���Ϣ�����Ƿ�һ��
//@PRECONDITION:
//@INPUT:������������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����,����ΪOS_TestMsgQueue1,�����Ϣ����Ϊ20,ÿ����Ϣ����󳤶�Ϊ4�ֽڣ�Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend������Ϣ����ʱʱ��ΪCSUDIOS_TIMEOUT_IMMEDIATE����������20����Ϣ
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive ������Ϣ,��ʱʱ��ΪCSUDIOS_TIMEOUT_IMMEDIATE���ȽϽ��յ�����Ϣֵ�뷢�͵�ֵ�Լ������Ƿ����,����������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueDestroy ���ٴ�������Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS
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
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	for (i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<5 ; j++ )
		{
			aanSendMsg[i][j] = (i * 10000000) + j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&aanSendMsg[i][j],nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "������Ϣʧ��");
		}
	}
	
	for (i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<5 ; j++ )
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,&aanRecvMsg[i][j],nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "������Ϣʧ��");
						
			CSTK_ASSERT_TRUE_FATAL(aanRecvMsg[i][j] == aanSendMsg[i][j],"���յ�����Ϣ�ͷ��͵Ĳ�һ��");
		}
	}

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMsgQueueCreate
//@CASEGROUP:2��CSUDIOSMsgQueueSend
//@CASEGROUP:3��CSUDIOSMsgQueueReceive
//@CASEGROUP:4��CSUDIOSMsgQueueDestroy
//@DESCRIPTION:1�����Դӿ���Ϣ�����н�����Ϣ�ܷ�ɹ�
//@DESCRIPTION:2������������Ϣ�����з�����Ϣ�ܷ�ɹ�
//@DESCRIPTION:3�����Ե�������Ϣ�����н���һ����Ϣ����ǰ������������Ϣ�ܷ�ɹ����͵���Ϣ����
//@PRECONDITION:
//@INPUT:������������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����,����ΪOS_TestMsgQueue1,�����Ϣ����Ϊ1,ÿ����Ϣ����󳤶�Ϊ4�ֽڣ�Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueReceive������Ϣ,��ʱʱ��Ϊ100����,Ӧ�ó�ʱ
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate ����һ������������
//@EXECUTIONFLOW:4������CSUDIOSThreadCreate ����һ������������
//@EXECUTIONFLOW:5���������������е���CSUDIOSMsgQueueSend����һ����Ϣ,Ӧ�óɹ�
//@EXECUTIONFLOW:6���������������е���CSUDIOSThreadSleep �ȴ�10ms�󣬵���CSUDIOSMsgQueueSend�ٴη���һ����Ϣ,��ʱ����Ϊ3�룬��ʱ������Ӧ�ñ�����
//@EXECUTIONFLOW:7�����������CSUDIOSThreadSleep�ȴ�1����ж�g_bThreadEnd��ֵ��Ӧ��Ϊfasle������ʧ��
//@EXECUTIONFLOW:8���������������е���CSUDIOSThreadSleep �ȴ�2���Ӻ󣬵���CSUDIOSMsgQueueReceive������Ϣ,��ʱʱ��Ϊ100ms��Ӧ�óɹ����յ���Ϣ
//@EXECUTIONFLOW:9��������8������Ϣ�ɹ����������������еڶ��η��ͱ���������ϢӦ�ÿ��Է��ͳɹ���Ȼ��g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:10�����������CSUDIOSThreadSleep�ȴ�2����ж�g_bThreadEnd��ֵ��Ӧ��Ϊtrue������ʧ��
//@EXECUTIONFLOW:11�����������CSUDIOSMsgQueueDestroy���ٴ�������Ϣ���У�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy  ���ٴ���������Ҫ�󷵻سɹ�
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
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMsgQueueReceive(hMessageQHandle, anRecvMsg, nMsgBytes, 100), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMsgQThread1", 64, 8*1024, 
									OS_TestMsgQThread1Entry, hMessageQHandle, &hProducerThread),
			"��������������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hProducerThread,"�ɹ���������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMsgQThread2", 64, 8*1024, 
									OS_TestMsgQThread2Entry, hMessageQHandle, &hCustomerThread),
			"��������������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hCustomerThread,"�ɹ���������������������Ϊ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"����7ʧ��");

	CSUDIOSThreadSleep(2000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����10ʧ��");
	
	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}

		if(hProducerThread != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hProducerThread), "ɾ������ʧ��");
			hProducerThread = CSUDI_NULL;
		}

		if(hCustomerThread != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hCustomerThread), "ɾ������ʧ��");
			hCustomerThread = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMsgQueueCreate
//@CASEGROUP:2��CSUDIOSMsgQueueSend
//@CASEGROUP:3��CSUDIOSMsgQueueReceive
//@CASEGROUP:4��CSUDIOSMsgQueueDestroy
//@DESCRIPTION:������Ϣ���з�����Ϣ�Ŀ���ģʽ
//@PRECONDITION:
//@INPUT:������������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate������Ϣ����,����ΪOS_TestMsgQueue1,�����Ϣ����Ϊ20,ÿ����Ϣ����󳤶�Ϊ4�ֽڣ�Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend������Ϣ����ʱʱ��ΪCSUDIOS_TIMEOUT_IMMEDIATE����������20����Ϣ��ÿ�η�����Ϣ��ɺ���������Ϣ��ַ�е���������
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive ������Ϣ,��ʱʱ��ΪCSUDIOS_TIMEOUT_IMMEDIATE���ȽϽ��յ�����Ϣֵ�뷢�͵�ֵ�Լ������Ƿ����,����������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSMsgQueueDestroy ���ٴ�������Ϣ���У�Ҫ�󷵻�CSUDI_SUCCESS
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
		"������Ϣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");


	for (i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<5 ; j++ )
		{
			aanSendMsg[i][j] = (i * 10000000) + j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&aanSendMsg[i][j],nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "������Ϣʧ��");
			aanSendMsg[i][j] = 0;
		}
	}
	
	for (i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<5 ; j++ )
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,&aanRecvMsg[i][j],nMsgBytes,CSUDIOS_TIMEOUT_IMMEDIATE), "������Ϣʧ��");

			CSTK_ASSERT_TRUE_FATAL(aanRecvMsg[i][j] == ((i * 10000000) + j), "���յ�����Ϣ�ͷ��͵Ĳ�һ��");
		}
	}

	CSTK_FATAL_POINT
	{
		if (hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMsgQueueCreate
//@CASEGROUP:2��CSUDIOSMsgQueueSend
//@CASEGROUP:3��CSUDIOSMsgQueueReceive
//@CASEGROUP:4��CSUDIOSMsgQueueDestroy
//@DESCRIPTION:������Ϣ�������ڴ�й¶���
//@PRECONDITION:
//@INPUT:������������
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ32����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ���У���ʱʱ��Ϊ500ms ��Ҫ���ͳɹ�
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive����Ϣ�����н���һ�����ݣ���ʱʱ��Ϊ500ms ��Ҫ����ճɹ�
//@EXECUTIONFLOW:4���жϽ��յ������ݺͷ��͵��Ƿ���ȣ��������������ʧ��
//@EXECUTIONFLOW:5������CSUDIOSMsgQueueDestroy���ٸմ�������Ϣ���в�����CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:6���ظ�����1 ~ 5����OS_TEST_COUNT�Σ�OS_TEST_COUNTͨ�������ļ�����
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0004( void )
{
	CSUDI_HANDLE hMessageQHandle = CSUDI_NULL;
	int nMaxMsgs = 1;
	int nSizePerMsg = 4;
	int nMsg = 0x88888888;
	int nMsgBytes = 4;
	int anRecvMsg[1];
	int i;

	CSTCPrint("��Ϣ����ǿ�Ȳ��Կ�ʼ����ȴ�������\n");
	
	for (i=0 ; i<OS_TEST_COUNT ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueCreate("OS_TestMsgQueue1",nMaxMsgs,nSizePerMsg,&hMessageQHandle),
			"������Ϣ����ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle,&nMsg,nMsgBytes,500), "������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle,anRecvMsg,nMsgBytes,500), "������Ϣʧ��");

		CSTK_ASSERT_TRUE_FATAL((anRecvMsg[0] == 0x88888888), "���յ�����Ϣ�ͷ��͵Ĳ����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");

		hMessageQHandle = CSUDI_NULL;
	}
	
	CSTK_FATAL_POINT
	{
		if(hMessageQHandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "������Ϣ����ʧ��");
			hMessageQHandle = CSUDI_NULL;
		}
	}

	CSTCPrint("��Ϣ����ǿ�Ȳ��Խ���������\n");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMsgQueueCreate
//@CASEGROUP:2��CSUDIOSMsgQueueSend
//@CASEGROUP:3��CSUDIOSMsgQueueReceive
//@CASEGROUP:4��CSUDIOSMsgQueueDestroy
//@DESCRIPTION:����CSUDIOSMsgQueueReceive������ϢԽ������
//@PRECONDITION:
//@INPUT:������������
//@EXECUTIONFLOW:1������CSUDIOSMsgQueueCreate ������Ϣ���У�����ΪOS_TestMsgQueue1�������Ϣ����Ϊ10����Ϣ��󳤶�Ϊ64��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMsgQueueSend����һ����Ϣ����Ϣ���У���Ϣ����Ϊ"aaaa"����Ϣ����Ϊ4����ʱʱ��Ϊ500ms ��Ҫ���ͳɹ�
//@EXECUTIONFLOW:3������CSUDIOSMsgQueueReceive����Ϣ�����н���һ�����ݣ�������Ϣ�������ʼ��Ϊ"bbbb",������Ϣ���ڴ泤��Ϊ2����ʱʱ��Ϊ500ms ��Ҫ����ճɹ�
//@EXECUTIONFLOW:4���жϽ��յ��������Ƿ���"aabb"���������������ʧ��
//@EXECUTIONFLOW:5���ظ�����2 ~ 4����10��
//@EXECUTIONFLOW:6������CSUDIOSMsgQueueDestroy�����ٸմ�������Ϣ���У���������CSUDI_SUCCESS
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
        "����1��������Ϣ����ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMessageQHandle,"�����ɹ�����Ϣ��������������Ϊ��");
    
    for (i=0; i<nTestCount; i++)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueSend(hMessageQHandle, acSenMsg, 4, 500), "����2��������Ϣʧ��");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(hMessageQHandle, acRecvMsg, 2, 500), "����3��������Ϣʧ��");

        nRet = strncmp(acRecvMsg, "aabb", 4);
        
        CSTK_ASSERT_TRUE_FATAL(nRet == 0, "����4��������ϢԽ��");
    }
    
    CSTK_FATAL_POINT
    {
        if(hMessageQHandle != CSUDI_NULL)
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMsgQueueDestroy(hMessageQHandle), "����6��������Ϣ����ʧ��");
            hMessageQHandle = CSUDI_NULL;
        }
    }

    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:�������������ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1"
//@INPUT:2����Ч�ź�����=0
//@INPUT:3������ź�����=1
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1��Ҫ�󷵻�CSUDI_SUCCESS�������Ч���ź������
//@EXECUTIONFLOW:2������CSUDIOSSemDestroy ���ٴ������ź�����Ҫ�����ٳɹ�������CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_SemCreate_0001( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������ľ������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:������Ч�ź�����������ź���������Ҳ�Ϊ0ʱ�ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1"
//@INPUT:2����Ч�ź�����=1
//@INPUT:3������ź�����=1
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1��Ҫ�󷵻�CSUDI_SUCCESS�������Ч���ź������
//@EXECUTIONFLOW:2������CSUDIOSSemDestroy ���ٴ������ź�����Ҫ�����ٳɹ�������CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_SemCreate_0002( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 1;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������ľ������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:�������������ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1"
//@INPUT:2����Ч�ź�����=2
//@INPUT:3������ź�����=10
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1��Ҫ�󷵻�CSUDI_SUCCESS�������Ч���ź������
//@EXECUTIONFLOW:2������CSUDIOSSemDestroy ���ٴ������ź�����Ҫ�����ٳɹ�������CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_SemCreate_0003( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 2;
	int  nMaxCount = 10;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������ľ������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:������Ч�ź�����������ź�������Ϊ0ʱ�ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1"
//@INPUT:2����Ч�ź�����=0
//@INPUT:3������ź�����=0
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:2���ж�������ź�������Ƿ�ΪCSUDI_NULL�������Ϊ�������CSUDIOSSemDestroy ���ٴ������ź���������ʧ�ܣ�����ɹ�
CSUDI_BOOL CSTC_OS_IT_SemCreate_0004( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "����Ч�ź�����������ź�������Ϊ0ʱ����Ӧ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"����ʧ�ܵ��ź���������Ӧ��Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:������Ч�ź�������������ź�����ʱ�ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1"
//@INPUT:2����Ч�ź�����=1
//@INPUT:3������ź�����=0
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:2���ж�������ź�������Ƿ�ΪCSUDI_NULL�������Ϊ�������CSUDIOSSemDestroy ���ٴ������ź���������ʧ�ܣ�����ɹ�
CSUDI_BOOL CSTC_OS_IT_SemCreate_0005( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 1;
	int  nMaxCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "����Ч�ź�������������ź�����ʱ����Ӧ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"����ʧ�ܵ��ź���������Ӧ��Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:������Ч�ź�������������ź�����ʱ�ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1"
//@INPUT:2����Ч�ź�����=2
//@INPUT:3������ź�����=1
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:2���ж�������ź�������Ƿ�ΪCSUDI_NULL�������Ϊ�������CSUDIOSSemDestroy ���ٴ������ź���������ʧ�ܣ�����ɹ�
CSUDI_BOOL CSTC_OS_IT_SemCreate_0006( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 2;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "����Ч�ź�������������ź�����ʱ����Ӧ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"����ʧ�ܵ��ź���������Ӧ��Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:������Ч�ź�������������ź�����ʱ�ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1"
//@INPUT:2����Ч�ź�����=12
//@INPUT:3������ź�����=10
//@EXPECTATION:����ʧ�ܲ����ش������(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:2���ж�������ź�������Ƿ�ΪCSUDI_NULL�������Ϊ�������CSUDIOSSemDestroy ���ٴ������ź���������ʧ�ܣ�����ɹ�
CSUDI_BOOL CSTC_OS_IT_SemCreate_0007( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 12;
	int  nMaxCount = 10;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "����Ч�ź�������������ź�����ʱ����Ӧ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"����ʧ�ܵ��ź���������Ӧ��Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:�����ź�������CSUDI_NULLʱ�ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������=CSUDI_NULL
//@INPUT:2����Ч�ź�����=0
//@INPUT:3������ź�����=1
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate��������Ϊ�յ��ź�����Ҫ�󷵻�CSUDI_SUCCESS�������Ч���ź������
//@EXECUTIONFLOW:2������CSUDIOSSemDestroy ���ٴ������ź�����Ҫ�����ٳɹ�������CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_SemCreate_0008( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate(CSUDI_NULL, nInitialCount, nMaxCount, &hSemhandle), "��������Ϊ�յ��ź���Ӧ�÷��سɹ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:�����ź������Ƴ���32�ֽ�ʱ�ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSemaphore01234567890123456789"
//@INPUT:2����Ч�ź�����=0
//@INPUT:3������ź�����=1
//@EXPECTATION:����ʧ�ܲ�����CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1ʧ�ܲ�����CSUDIOS_ERROR_BAD_PARAMETER�������������ʧ��
//@EXECUTIONFLOW:2���ж�������ź�������Ƿ�ΪCSUDI_NULL�������Ϊ�������CSUDIOSSemDestroy ���ٴ������ź���������ʧ�ܣ�����ɹ�
CSUDI_BOOL CSTC_OS_IT_SemCreate_0009( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSSemCreate("OS_TestSemaphore01234567890123456789", nInitialCount, nMaxCount, &hSemhandle), "�������Ƴ���32�ֽڵ��ź���Ӧ�÷��ز�������");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hSemhandle,"����ʧ�ܵ��ź���������Ӧ��Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:��������ź�����Ϊ100ʱ����100���ź����ܷ񴴽��ɹ�
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1_1 ~ OS_TestSem1_100"
//@INPUT:2����Ч�ź�����=0
//@INPUT:3������ź�����=100
//@EXPECTATION:ȫ�������ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestSem1_1 ~ OS_TestSem1_100��Ҫ��ȫ������CSUDI_SUCCESS�������Ч���ź������
//@EXECUTIONFLOW:2������CSUDIOSSemDestroy ���ٴ����������ź�����Ҫ��ȫ�����ٳɹ�������CSUDI_SUCCESS�������������ʧ��
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate(aacSemName[i], nInitialCount, nMaxCount, &hSemhandle[i]), "�����ź���ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle[i],"�����ɹ����ź�������������Ϊ��");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<100 ; i++)
		{
			if (hSemhandle[i] != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle[i]), "�����ź���ʧ��");
				hSemhandle[i] = CSUDI_NULL;
			}
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemCreate
//@DESCRIPTION:�����ź���������ͬʱ�ܷ�ɹ������ź���
//@PRECONDITION:
//@INPUT:1���ź�������="OS_TestSem1"
//@INPUT:2����Ч�ź�����=0
//@INPUT:3������ź�����=1
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź���OS_TestOSSem1��Ҫ�󷵻�CSUDI_SUCCESS�������Ч���ź������
//@EXECUTIONFLOW:2���ٴε���CSUDIOSSemCreate�����ź�����ΪOS_TestOSSem1���ź�����Ҫ�󷵻�CSUDI_SUCCESS�������Ч���ź������
//@EXECUTIONFLOW:3������CSUDIOSSemDestroy ���ٴ������ź�����Ҫ�����ٳɹ�������CSUDI_SUCCESS�������������ʧ��
CSUDI_BOOL CSTC_OS_IT_SemCreate_0011( void )
{
	CSUDI_HANDLE hSemhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hSemhandle2 = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestOSSem1", nInitialCount, nMaxCount, &hSemhandle1), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle1,"�ɹ��������ź�������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestOSSem1", nInitialCount, nMaxCount, &hSemhandle2), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle2,"�ɹ��������ź�������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle1), "�����ź���ʧ��");
			hSemhandle1 = CSUDI_NULL;
		}

		if (hSemhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle2), "�����ź���ʧ��");
			hSemhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSSemWait 
//@CASEGROUP:2��CSUDIOSSemRelease 
//@DESCRIPTION:�����ȵȴ����ͷ��ź���
//@PRECONDITION:1�����Գɹ������ź���
//@PRECONDITION:2�����Գɹ������ź���
//@INPUT:������������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSSemCreate�����ź���OS_TestOSSem1����Ч����Ϊ0���������Ϊ1���ɹ�����
//@EXECUTIONFLOW:2������CSUDIOSSemWait �ȴ��ź�������ʱʱ����Ϊ100ms��Ӧ�÷���CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:3������CSUDIOSThreadSleep �ȴ�100ms������CSUDIOSSemRelease �ͷ�һ���ź�����Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4������CSUDIOSSemWait �ȴ��ź�������ʱʱ����Ϊ500ms ��Ӧ�õȴ��ɹ�
//@EXECUTIONFLOW:5������CSDestroySemaphore���ٴ������ź�����Ӧ�����ٳɹ�
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0001( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������ľ������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSSemWait(hSemhandle,100), "����2ʧ��");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,500), "����4ʧ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSSemWait 
//@CASEGROUP:2��CSUDIOSSemRelease 
//@DESCRIPTION:�������ͷź�ȴ��ź���
//@PRECONDITION:1�����Գɹ������ź���
//@PRECONDITION:2�����Գɹ������ź���
//@INPUT:������������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSSemCreate�����ź���OS_TestOSSem1����Ч����Ϊ0���������Ϊ1���ɹ�����
//@EXECUTIONFLOW:2������CSUDIOSThreadSleep �ȴ�100ms������CSUDIOSSemRelease �ͷ�һ���ź�����Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDIOSSemWait �ȴ��ź�������ʱʱ����ΪCSUDIOS_TIMEOUT_IMMEDIATE��Ӧ�õȴ��ɹ�
//@EXECUTIONFLOW:4������CSDestroySemaphore���ٴ������ź�����Ӧ�����ٳɹ�
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0002( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������ľ������Ϊ��");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "����3ʧ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSSemWait 
//@CASEGROUP:2��CSUDIOSSemRelease 
//@DESCRIPTION:�����ڵȴ��Ĺ��������ź����ͷţ��Ƿ���Եȴ��ɹ�
//@PRECONDITION:1�����Գɹ�����������ź���
//@PRECONDITION:2�����Գɹ�����������ź���
//@INPUT:������������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSSemCreate�����ź�������Ч����Ϊ0���������Ϊ1���ɹ�����
//@EXECUTIONFLOW:2�����������е���CSUDIOSGetTime  ��ȡ��ǰϵͳʱ��
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate ����������,���������е���CSUDIOSThreadSleep �ȴ�2000ms������CSUDIOSSemRelease �ͷ�һ���ź���
//@EXECUTIONFLOW:4�����������е���CSUDIOSSemWait �ȴ��ź�������ʱʱ����Ϊ5�� ��һ��ʱ���Ӧ�õȴ��ɹ�
//@EXECUTIONFLOW:5�����������е���CSUDIOSGetTime �ٴ� ��ȡ��ǰϵͳʱ��
//@EXECUTIONFLOW:6���ж�����ϵͳʱ��Ĳ�ֵ��2000�Ĳ��Ƿ�������20ms֮�䣬�������ɹ�������ʧ��
//@EXECUTIONFLOW:7������CSUDIOSThreadJoin�ȴ�����3�����������˳���Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSDestroySemaphore���ٴ������ź�����Ӧ�����ٳɹ�
//@EXECUTIONFLOW:9������CSUDIOSThreadDestroy  ���ٴ���������Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0003( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int  nInitialCount = 0;
	int  nMaxCount = 1;
	int  nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������ľ������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestSemThread", 64, 8*1024, OS_TestSemThreadEntry, hSemhandle,  &hThreadhandle), "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ�����������ľ������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,5000), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "����5ʧ��");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 2000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-20) && (nMiliSecond <=20), "������û���ڹ涨ʱ���ڵȴ����ź���");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hThreadhandle), "�ȴ������˳�ʧ��");

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSSemWait 
//@CASEGROUP:2��CSUDIOSSemRelease 
//@DESCRIPTION:���Զ�εȴ����ͷŵ��Ƿ�ɹ�
//@PRECONDITION:1�����Գɹ������ź���
//@PRECONDITION:2�����Գɹ������ź���
//@INPUT:������������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSSemCreate�����ź�������Ч����Ϊ0���������Ϊ100���ɹ�����
//@EXECUTIONFLOW:2������CSUDIOSSemRelease�����ͷ��ź���101�Σ�Ҫ��ǰ100�γɹ������һ��ʧ��
//@EXECUTIONFLOW:3������CSUDIOSSemWait�����ȴ��ź���101�Σ���ʱʱ�䶼���ó�10ms��Ҫ��ǰ100�γɹ������һ�γ�ʱ
//@EXECUTIONFLOW:4������CSDestroySemaphore���ٴ������ź�����Ӧ�����ٳɹ�
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0004( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 100;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "�����ź���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������ľ������Ϊ��");
	
	for(i=0 ; i<101 ; i++)
	{
		if (i != 100)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "����2ǰ100��Ӧ�óɹ�");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemRelease(hSemhandle), "����2���һ��Ӧ��ʧ��");
		}
	}

	for(i=0 ; i<101 ; i++)
	{
		if (i != 100)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,10), "����3ǰ100��Ӧ�óɹ�");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemWait(hSemhandle,10), "����3ʧ�����һ��Ӧ��ʧ��");
		}
	}

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemWait
//@CASEGROUP:CSUDIOSSemRelease 
//@DESCRIPTION:���Բ���ΪCSUDI_NULL��ʱ�򣬺�����ִ�н��
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemWait���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSSemRelease���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0005( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemWait(CSUDI_NULL, CSUDIOS_TIMEOUT_IMMEDIATE), "����1ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSSemRelease(CSUDI_NULL), "����2ʧ��");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemWait
//@DESCRIPTION:���Բ���ΪCSUDIOS_TIMEOUT_INFINITY��ʱ��CSUDIOSSemWait��ִ�н��
//@PRECONDITION:
//@INPUT:CSUDIOS_TIMEOUT_INFINITY
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate���������OS_TestSemThread1Entry�н�g_bThreadEnd��ֵΪCSUDI_TRUE���ȴ�10s�����CSUDIOSSemRelease����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSSemWait����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ж�g_bThreadEnd������ΪCSUDI_TRUE
//@EXECUTIONFLOW:5������CSUDIOSSemDestroy����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroy����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0006( void )
{
    CSUDI_HANDLE hSemhandle = CSUDI_NULL;
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    int nInitialCount = 0;
    int nMaxCount = 100;
    int nPriority = 7;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "����1�������ź���ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle, "�ɹ��������ź�������ľ������Ϊ��");

    g_bThreadEnd = CSUDI_FALSE;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread", nPriority, TEST_THREAD_STACK_SIZE, 
                            OS_TestSemThread1Entry, hSemhandle, &hThreadhandle), "����2����������ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�����ɹ����������������������Ϊ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle, CSUDIOS_TIMEOUT_INFINITY), "����3��������Դʧ��");

    CSUDIOSThreadSleep(100);
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "����4ʧ�ܣ�û�еȴ�10s�ͷ���");
    
    CSTK_FATAL_POINT

    if (hSemhandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "����5�������ź���ʧ��");
        hSemhandle = CSUDI_NULL;
    }
    
    if(hThreadhandle != CSUDI_NULL)
    {           
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "����6��ɾ������ʧ��");
        hThreadhandle = CSUDI_NULL;
    }
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSSemRelease 
//@DESCRIPTION:����CSUDIOSSemRelease�ӿڵ�ִ��ʱ��
//@PRECONDITION:CSUDIOSGetTime�ӿ�û������
//@INPUT:
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate�����ź�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSGetTime��ȡϵͳʱ��sTime1����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSSemRelease�ͷ��ź�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIOSGetTime��ȡϵͳʱ��sTime2����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������ִ��CSUDIOSSemRelease��ʱ�䲢��ӡֵ��������ʱС��5ms
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0007( void )
{
    CSUDIOSTimeVal_S sTime1;
    CSUDIOSTimeVal_S sTime2;
    int nMiliSecond = 0;
    CSUDI_HANDLE hSemhandle = CSUDI_NULL;
    int nInitialCount = 1;
    int nMaxCount = 10;
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "����1�������ź���ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle, "�ɹ��������ź�������ľ������Ϊ��");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "����2����һ�λ�ȡϵͳʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "����3ʧ�ܣ��ͷ��ź���ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "����4���ڶ��λ�ȡϵͳʱ��ʧ��");

    nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) ; 

    CSTCPrint("ִ��CSUDIOSSemRelease���ĵ�ʱ��Ϊ:%d(ms)\n", nMiliSecond);
	
    CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 5, "����5���ȴ���ʱ�䲻��ϵͳ�ݴ�ֵ��");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}


//@CASEGROUP:1��CSUDIOSSemCreate 
//@CASEGROUP:2��CSUDIOSSemWait 
//@CASEGROUP:1��CSUDIOSSemRelease 
//@CASEGROUP:2��CSDestroySemaphore 
//@DESCRIPTION:�ź���ǿ�Ȳ���
//@PRECONDITION:
//@INPUT:������������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSSemCreate�����ź�������Ч����Ϊ0���������Ϊ1���ɹ�����
//@EXECUTIONFLOW:2���Դ������ź���ִ��һ���ͷţ�һ�εȴ�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDIOSSemDestroy ���ٴ������ź�����Ҫ�����ٳɹ�������CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:4���ظ�1~ 3����OS_TEST_COUNT�Σ�OS_TEST_COUNTͨ�������ļ�����
CSUDI_BOOL CSTC_OS_IT_Sem_0001( void )
{
	CSUDI_HANDLE hSemhandle = CSUDI_NULL;
	int  nInitialCount = 0;
	int  nMaxCount = 1;
	int i;

	CSTCPrint("�ź���ǿ�Ȳ��Կ�ʼ����ȴ�������\n");

	for ( i=0 ; i<OS_TEST_COUNT ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("OS_TestSem1", nInitialCount, nMaxCount, &hSemhandle), "�����ź���ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hSemhandle,"�ɹ��������ź�������ľ������Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemRelease(hSemhandle), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(hSemhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");

		hSemhandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (hSemhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(hSemhandle), "�����ź���ʧ��");
			hSemhandle = CSUDI_NULL;
		}
	}

	CSTCPrint("�ź���ǿ�Ȳ��Խ���������\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:���ԺϷ���������ʱ�ܷ�ɹ����������ٻ�����
//@PRECONDITION:
//@INPUT:1������="OS_TestOSMutex1"
//@INPUT:2��������ѡ��=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������OS_TestOSMutex1������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����ýӿ�CSUDIOSMutexDestroy���ٸմ����Ļ��������سɹ�
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0001( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:��������CSUDI_NULLʱ�ܷ�ɹ����������ٻ�����
//@PRECONDITION:
//@INPUT:1������=CSUDI_NULL
//@INPUT:2��������ѡ��=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate��������Ϊ�յĻ�����Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����ýӿ�CSUDIOSMutexDestroy���ٸմ����Ļ�����Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0002( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate(CSUDI_NULL,uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:���Ի�����ѡ��Ϊ0ʱ�ܷ�ɹ����������ٻ�����
//@PRECONDITION:
//@INPUT:1������="OS_TestOSMutex1"
//@INPUT:2��������ѡ��=0
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������OS_TestOSMutex1������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����ýӿ�CSUDIOSMutexDestroy���ٸմ����Ļ�����Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0003( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:�������Ƴ���32�ֽ�ʱ�ܷ�ɹ����������ٻ�����
//@PRECONDITION:
//@INPUT:1������="OS_TestOSMutex01234567890123456789"
//@INPUT:2��������ѡ��=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������ʧ�ܲ����ش������CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2���жϲ���1����Ļ��������������ǿ�����ýӿ�CSUDIOSMutexDestroy���ٸմ����Ļ�����������ʧ�ܣ�����ɹ�
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0004( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSMutexCreate("OS_TestOSMutex01234567890123456789",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hMutexhandle,"����ʧ�ܵĻ���������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate 
//@CASEGROUP:2.CSUDIOSMutexDestroy 
//@DESCRIPTION:���Ի�����������ͬʱ�ܷ�ɹ����������ٻ�����
//@PRECONDITION:
//@INPUT:1������="OS_TestOSMutex1"
//@INPUT:2��������ѡ��=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������OS_TestOSMutex1������CSUDI_SUCCESS
//@EXECUTIONFLOW:2���ٴε��ýӿ�CSUDIOSMutexCreate������������ΪOS_TestOSMutex1�Ļ�����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ýӿ�CSUDIOSMutexDestroy���ٸմ����Ļ��������سɹ�
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0005( void )
{
	CSUDI_HANDLE hMutexhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hMutexhandle2 = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle1), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle1,"�ɹ������Ļ���������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle2), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle2,"�ɹ������Ļ���������������Ϊ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle1), "���ٻ�����ʧ��");
			hMutexhandle1 = CSUDI_NULL;
		}

		if (hMutexhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle2), "���ٻ�����ʧ��");
			hMutexhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSMutexCreate
//@CASEGROUP:2.CSUDIOSMutexDestroy
//@DESCRIPTION:���Ի��������ڴ�й¶���
//@PRECONDITION:
//@INPUT:1������="OS_TestOSMutex1"
//@INPUT:2��������ѡ��=CSUDIOS_MUTEX_OBTAIN
//@EXPECTATION:�ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMutexCreate����������OS_TestOSMutex1������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMutexDestroy�����ź���,Ԥ�ڷ��سɹ�
//@EXECUTIONFLOW:3���ظ�����1~2����OS_TEST_COUNT�Σ�OS_TEST_COUNTͨ�������ļ�����
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0006( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;
	int i;

	CSTCPrint("������ǿ�Ȳ��Կ�ʼ����ȴ�������\n");

	for (i=0 ; i<OS_TEST_COUNT ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");

		hMutexhandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
	}

	CSTCPrint("������ǿ�Ȳ��Խ���������\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMutexWait  
//@CASEGROUP:2��CSUDIOSMutexRelease  
//@DESCRIPTION:���Ի�����ѡ��=CSUDIOS_MUTEX_OBTAINʱ�ȵȴ����ͷŻ����������
//@PRECONDITION:1�����Գɹ�����������
//@PRECONDITION:2�����Գɹ����ٻ�����
//@INPUT:�Ϸ���������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������������ΪOS_TestOSMutex1��������ѡ��=CSUDIOS_MUTEX_OBTAIN��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate�������������������е���CSUDIOSMutexWait �ȴ�����������ʱʱ��Ϊ2�룬  ����ȵ���g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3��������ȴ�100ms���ж�g_bThreadEnd��ֵ��Ҫ��Ϊfalse���������ʧ��
//@EXECUTIONFLOW:4�����������CSUDIOSMutexRelease �ͷ�һ��������
//@EXECUTIONFLOW:5��������ȴ�100ms���ٴ��ж�g_bThreadEnd��ֵ��Ҫ��Ϊtrue���������ʧ��
//@EXECUTIONFLOW:6������CSUDIOSMutexDestroy���ٴ����Ļ�������Ҫ�����ٳɹ�
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0001( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread1", 64, 8*1024, OS_TestMutexThread1Entry, hMutexhandle, &hThreadhandle), "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�ɹ���������������������Ϊ��");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(g_bThreadEnd == CSUDI_FALSE,"������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "�ͷŻ�����ʧ��");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(g_bThreadEnd == CSUDI_TRUE,"���岽ʧ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMutexWait  
//@CASEGROUP:2��CSUDIOSMutexRelease  
//@DESCRIPTION:���Ի�����ѡ��=0ʱ�ȵȴ����ͷŻ����������
//@PRECONDITION:1�����Գɹ�����������
//@PRECONDITION:2�����Գɹ����ٻ�����
//@INPUT:�Ϸ���������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������������ΪOS_TestOSMutex1��������ѡ��=0��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMutexWait �ȴ�����������ʱʱ����Ϊ100 ��Ӧ�÷���CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSMutexRelease �ͷ�һ��������
//@EXECUTIONFLOW:4������CSUDIOSMutexWait �ȴ�����������ʱʱ����Ϊ100 ��Ӧ�÷���CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIOSMutexDestroy���ٴ����Ļ�������Ҫ�����ٳɹ�
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0002( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	unsigned int uOptions = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "�ͷŻ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "����4ʧ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMutexWait  
//@CASEGROUP:2��CSUDIOSMutexRelease  
//@DESCRIPTION:���Ի������ܷ���������
//@PRECONDITION:1�����Գɹ���������ͻ�����
//@PRECONDITION:2�����Գɹ���������ͻ�����
//@INPUT:�Ϸ���������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������������ΪOS_TestOSMutex1��������ѡ��=0��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate ��������������Ҫ��ȫ�������ɹ�
//@EXECUTIONFLOW:3���ֱ��������������е���CSUDIOSThreadSleep�ȴ�1��2��3�룬������CSUDIOSMutexWait�ȴ�������OS_TestOSMutex1
//@EXECUTIONFLOW:4�������ɹ��󣬷ֱ��������������е���CSUDIOSThreadSleep�ȴ�6��4��2�룬���ı�g_nTestData��ֵ��Ȼ���ͷŻ����������з���
//@EXECUTIONFLOW:5������������ͨ���ж�g_nTestData��ֵ�Ƿ��Ԥ��������ж�����������ִ����ϵ��Ⱥ�˳��
//@EXECUTIONFLOW:6����һ�εȴ�1�������Ӧ������ִ���꣬�ȴ�3���������ִ��˳����˲��������������ʧ��
//@EXECUTIONFLOW:7������CSUDIOSMutexDestroy���ٴ����Ļ�������Ҫ�����ٳɹ�
//@EXECUTIONFLOW:8������CSUDIOSThreadDestroy  ���ٴ���������Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0003( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;
	unsigned int uOptions = 0;

	g_nTestData = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread2_1", 64, 8*1024, OS_TestMutexThread2_1Entry, hMutexhandle, &hThreadhandle1), "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�ɹ���������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread2_2", 64, 8*1024, OS_TestMutexThread2_2Entry, hMutexhandle, &hThreadhandle2), "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�ɹ���������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread2_3", 64, 8*1024, OS_TestMutexThread2_3Entry, hMutexhandle, &hThreadhandle3), "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"�ɹ���������������������Ϊ��");

	CSTCPrint("\n��ȴ�15��!!!!\n");

	while(g_nTestData == 0)
	{
		CSUDIOSThreadSleep(10);
	}

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"ִ��˳����Ԥ�ڲ���");

	while(g_nTestData == 1)
	{
		CSUDIOSThreadSleep(10);
	}

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 2,"ִ��˳����Ԥ�ڲ���");

	while(g_nTestData == 2)
	{
		CSUDIOSThreadSleep(10);
	}

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 3,"ִ��˳����Ԥ�ڲ���");
	
	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}

		if (hThreadhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������ʧ��");
			hThreadhandle1 = CSUDI_NULL;
		}

		if (hThreadhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������ʧ��");
			hThreadhandle2 = CSUDI_NULL;
		}

		if (hThreadhandle3 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "ɾ������ʧ��");
			hThreadhandle3 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMutexWait  
//@CASEGROUP:2��CSUDIOSMutexRelease  
//@DESCRIPTION:���Զ��Ѿ������Ļ��������н��������Լ���������֮�����������
//@PRECONDITION:1�����Գɹ���������ͻ�����
//@PRECONDITION:2�����Գɹ���������ͻ�����
//@INPUT:�Ϸ���������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������������ΪOS_TestOSMutex1��������ѡ��=0��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMutexWait �ȴ�����������ʱʱ����Ϊ100 ��Ӧ�÷���CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSMutexRelease �ͷŻ�������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:4���ٴε���CSUDIOSMutexRelease �ͷŻ�������Ӧ�÷��ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:5������CSUDIOSMutexWait �ȴ�����������ʱʱ����Ϊ100 ��Ӧ�÷���CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIOSThreadCreate �������������������е���CSUDIOSMutexWait�ȴ�����������ʱʱ��Ϊ1500ms�����ȵ���g_nTestData��0��Ϊ1������g_nTestData��0��Ϊ2
//@EXECUTIONFLOW:7�����������CSUDIOSThreadSleep�ȴ�500ms���ж�g_nTestData��ֵ�������Ϊ0����˵�����������������Ѿ�����������»���˻���������������ʧ��
//@EXECUTIONFLOW:8�����������CSUDIOSThreadSleep�ȴ�2����ж�g_nTestData��ֵ�������2��˵��������ȴ�������ʧ�ܺ󷵻أ���������ϣ������������ʧ��
//@EXECUTIONFLOW:9�����������CSUDIOSMutexRelease �ͷŻ�������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:10������CSUDIOSMutexDestroy���ٴ����Ļ�������Ҫ�����ٳɹ�
//@EXECUTIONFLOW:11������CSUDIOSThreadDestroy  ���ٴ���������Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0004( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uOptions = 0;

	g_nTestData = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMutexRelease(hMutexhandle), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread3", 64, 8*1024, OS_TestMutexThread3Entry, hMutexhandle, &hThreadhandle), "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�ɹ���������������������Ϊ��");

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 0,"����7ʧ��");

	CSUDIOSThreadSleep(2000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 2,"����8ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "�ͷŻ�����ʧ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMutexWait  
//@CASEGROUP:2��CSUDIOSMutexRelease  
//@DESCRIPTION:���Ի�������Ƕ�׵���
//@PRECONDITION:1�����Գɹ���������ͻ�����
//@PRECONDITION:2�����Գɹ���������ͻ�����
//@INPUT:�Ϸ���������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������������ΪOS_TestOSMutex1��������ѡ��=0��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2���ظ�����CSUDIOSMutexWait �ȴ�������10�Σ���ʱʱ����ΪCSUDIOS_TIMEOUT_IMMEDIATE  ��Ӧ��ȫ������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate �������������������е���CSUDIOSMutexWait�ȴ�����������ʱʱ��Ϊ2�� �����ȵ���g_bThreadEnd��false��Ϊtrue�����򲻲���
//@EXECUTIONFLOW:4�����������CSUDIOSMutexRelease �ͷŻ�����9�Σ�Ӧ�÷���CSUDI_SUCCESS
//@EXECUTIONFLOW:5�����������CSUDIOSThreadSleep�ȴ�500ms���ж�g_bThreadEnd��ֵ�����Ϊfalse����˵����������Ȼû�еȵ��������������������ʧ��
//@EXECUTIONFLOW:6�����������CSUDIOSMutexRelease �ͷŻ�������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:7�����������CSUDIOSThreadSleep�ȴ�500ms���ж�g_bThreadEnd��ֵ�����Ϊtrue����˵���������ڽ���10�μ��������10�ν����ɹ�����������Եȵ������������н����������������ʧ��
//@EXECUTIONFLOW:8������CSUDIOSMutexDestroy���ٴ����Ļ�������Ҫ�����ٳɹ�
//@EXECUTIONFLOW:9������CSUDIOSThreadDestroy  ���ٴ���������Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0005( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uOptions = 0;
	int i;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");

	for (i=0 ; i<10 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "����2ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread1", 64, 8*1024, OS_TestMutexThread1Entry, hMutexhandle, &hThreadhandle), "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�ɹ���������������������Ϊ��");

	for (i=0 ; i<9 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "����4ʧ��");
	}

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "����6ʧ��");

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����7ʧ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMutexWait
//@CASEGROUP:2��CSUDIOSMutexRelease  
//@DESCRIPTION:����һ������Ի��������м�������һ�����������м����ͽ��������
//@PRECONDITION:1�����Գɹ���������ͻ�����
//@PRECONDITION:2�����Գɹ���������ͻ�����
//@INPUT:�Ϸ���������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSMutexCreate����������������ΪOS_TestOSMutex1��������ѡ��=0��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate �������������������е���CSUDIOSMutexWait�ȴ�����������ʱʱ��Ϊ20ms �����ȵ���g_bThreadEnd��false��Ϊtrue�����򲻲���
//@EXECUTIONFLOW:3��������ı�g_bThreadEnd��ֵ�����CSUDIOSThreadSleep�ȴ�1�룬�ٵ���CSUDIOSMutexRelease����
//@EXECUTIONFLOW:4�����������CSUDIOSThreadSleep�ȴ�100ms���ж�g_bThreadEnd��ֵ��Ӧ��Ϊtrue������ʧ��
//@EXECUTIONFLOW:5�����������CSUDIOSMutexWait��������ʱʱ��Ϊ100ms��Ӧ�÷���CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:6�����������CSUDIOSMutexRelease������Ӧ�÷��ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7�����������CSUDIOSThreadSleep�ȴ�1����ٵ���CSUDIOSMutexWait������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:8�����������CSUDIOSMutexRelease������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:9������CSUDIOSMutexDestroy���ٴ����Ļ�������Ҫ�����ٳɹ�
//@EXECUTIONFLOW:10������CSUDIOSThreadDestroy  ���ٴ���������Ҫ�󷵻�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0006( void )
{
	CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uOptions = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1",uOptions,&hMutexhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread4", 64, 8*1024, OS_TestMutexThread4Entry, hMutexhandle, &hThreadhandle), "��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�ɹ���������������������Ϊ��");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSMutexWait(hMutexhandle,100), "����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMutexRelease(hMutexhandle), "����6ʧ��");

	CSUDIOSThreadSleep(1500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle,100), "����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "����8ʧ��");

	CSTK_FATAL_POINT
	{
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMutexWait
//@CASEGROUP:CSUDIOSMutexRelease 
//@DESCRIPTION:���Բ���ΪCSUDI_NULL��ʱ�򣬺�����ִ�н��
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMutexWait���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSMutexRelease���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0007( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMutexWait(CSUDI_NULL, CSUDIOS_TIMEOUT_IMMEDIATE), "����1ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSMutexRelease(CSUDI_NULL), "����2ʧ��");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMutexWait
//@DESCRIPTION:���Գ�ʱʱ�����ΪCSUDIOS_TIMEOUT_INFINITYʱ��CSUDIOSMutexWait��ִ�н��
//@PRECONDITION:
//@INPUT:CSUDIOS_TIMEOUT_INFINITY
//@EXPECTATION:�ȴ�10��󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSMutexCreate����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate �������������������е���CSUDIOSMutexWait���ٵȴ�10s�����ŵ���CSUDIOSMutexRelease������g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3������CSUDIOSMutexWait����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ж�g_bThreadEnd��ֵ������ΪCSUDI_TRUE
//@EXECUTIONFLOW:5������CSUDIOSMutexDestroy����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroy����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0008( void )
{
    CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    unsigned int uOptions = 0;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1", uOptions, &hMutexhandle), "����1ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");
    
    g_bThreadEnd = CSUDI_FALSE;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread5", 64, 8*1024, OS_TestMutexThread5Entry, hMutexhandle, &hThreadhandle), "����2����������ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�ɹ���������������������Ϊ��");
    
    CSUDIOSThreadSleep(100);//���߳��е�CSUDIOSMutexWait�ȵ���
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexWait(hMutexhandle, CSUDIOS_TIMEOUT_INFINITY), "����3����������ʧ��");

    CSUDIOSThreadSleep(100);//�ȴ��߳��Ƚ���

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "����4ʧ�ܣ�û�еȴ�10s�ͷ���");
    
    CSTK_FATAL_POINT
    {
		if (hMutexhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexDestroy(hMutexhandle), "���ٻ�����ʧ��");
			hMutexhandle = CSUDI_NULL;
		}
		if (hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "ɾ������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSMutexRelease 
//@DESCRIPTION:����CSUDIOSMutexRelease�ӿڵ�ִ��ʱ��
//@PRECONDITION:CSUDIOSGetTime�ӿ�û������
//@INPUT:
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSSemCreate��������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSGetTime��ȡϵͳʱ��sTime1����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSMutexRelease�ͷŻ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIOSGetTime��ȡϵͳʱ��sTime2����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������ִ��CSUDIOSMutexRelease��ʱ�䲢��ӡֵ��������ʱС��5ms
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0009( void )
{
    CSUDIOSTimeVal_S sTime1;
    CSUDIOSTimeVal_S sTime2;
    CSUDI_HANDLE hMutexhandle = CSUDI_NULL;
    unsigned int uOptions = CSUDIOS_MUTEX_OBTAIN;
    int nMiliSecond = 0;
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexCreate("OS_TestOSMutex1", uOptions, &hMutexhandle), "����1ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hMutexhandle,"�ɹ������Ļ���������������Ϊ��");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "����2����һ�λ�ȡϵͳʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSMutexRelease(hMutexhandle), "����3ʧ�ܣ��ͷŻ�����ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "����4���ڶ��λ�ȡϵͳʱ��ʧ��");

    nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) ; 
	
    CSTCPrint("ִ��CSUDIOSSemRelease���ĵ�ʱ��Ϊ:%d(ms)\n", nMiliSecond);

    CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 5, "����5���ȴ���ʱ�䲻��ϵͳ�ݴ�ֵ��");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetTime    
//@DESCRIPTION:���Ի�ȡ��ϵͳ�����󾭹���ʱ���׼ȷ��
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:���λ�ȡʱ��Ĳ�ֵ��Sleep��ʱ��������ݴ�ֵ��ÿ������10ms
//@EXECUTIONFLOW:1������CSUDIOSGetTime��ȡ��ϵͳ�����󾭹���ʱ�䣬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:2������CSUDIOSThreadSleep �ȴ�1�룬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:3������CSUDIOSGetTime�ٴλ�ȡ��ϵͳ�����󾭹���ʱ�䣬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:4���ж����λ�ȡϵͳʱ��Ĳ�ֵ�͵ȴ�ʱ��Ĳ����ݴ�������10ms�򷵻سɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_GetTime_0001( void )
{
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "��һ�λ�ȡϵͳʱ��ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "�ڶ��λ�ȡϵͳʱ��ʧ��");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 1000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-10) && (nMiliSecond <=10), "�ȴ���ʱ�䲻��ϵͳ�ݴ�ֵ��");
	
	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetTime    
//@DESCRIPTION:���Ի�ȡ��ϵͳ�����󾭹���ʱ���׼ȷ��
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:���λ�ȡʱ��Ĳ�ֵ��Sleep��ʱ��������ݴ�ֵ��ÿ������10ms
//@EXECUTIONFLOW:1������CSUDIOSGetTime��ȡ��ϵͳ�����󾭹���ʱ�䣬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:2������CSUDIOSThreadSleep �ȴ�5�룬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:3������CSUDIOSGetTime�ٴλ�ȡ��ϵͳ�����󾭹���ʱ�䣬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:4���ж����λ�ȡϵͳʱ��Ĳ�ֵ�͵ȴ�ʱ��Ĳ����ݴ�������50ms�򷵻سɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_GetTime_0002( void )
{
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;

	CSTCPrint("\n��ȴ�5�롣����\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "��һ�λ�ȡϵͳʱ��ʧ��");

	CSUDIOSThreadSleep(5000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "�ڶ��λ�ȡϵͳʱ��ʧ��");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 5000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-50) && (nMiliSecond <=50), "�ȴ���ʱ�䲻��ϵͳ�ݴ�ֵ��");
	
	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetTime    
//@DESCRIPTION:���Ի�ȡ��ϵͳ�����󾭹���ʱ���׼ȷ��
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:���λ�ȡʱ��Ĳ�ֵ��Sleep��ʱ��������ݴ�ֵ��ÿ������10ms
//@EXECUTIONFLOW:1������CSUDIOSGetTime��ȡ��ϵͳ�����󾭹���ʱ�䣬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:2������CSUDIOSThreadSleep �ȴ�10�룬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:3������CSUDIOSGetTime�ٴλ�ȡ��ϵͳ�����󾭹���ʱ�䣬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:4���ж����λ�ȡϵͳʱ��Ĳ�ֵ�͵ȴ�ʱ��Ĳ����ݴ�������100ms�򷵻سɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_GetTime_0003( void )
{
	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int nMiliSecond;

	CSTCPrint("\n��ȴ�10�롣����\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "��һ�λ�ȡϵͳʱ��ʧ��");

	CSUDIOSThreadSleep(10000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "�ڶ��λ�ȡϵͳʱ��ʧ��");

	nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) - 10000; 

	CSTK_ASSERT_TRUE_FATAL((nMiliSecond >=-100) && (nMiliSecond <=100), "�ȴ���ʱ�䲻��ϵͳ�ݴ�ֵ��");
	
	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetTime    
//@DESCRIPTION:���Ըýӿڲ���ΪCSUDI_NULLʱ�ܷ��ȡϵͳʱ��
//@PRECONDITION:
//@INPUT:
//@EXPECTATION:��ȡʱ��ʧ�ܷ��ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSGetTime������ʹ��CSUDI_NULL����ȡ��ϵͳ�����󾭹���ʱ�䣬Ӧ�÷���ʧ��
CSUDI_BOOL CSTC_OS_IT_GetTime_0004( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetTime(CSUDI_NULL), "��һ��ʧ��");

	CSTK_FATAL_POINT
	{
		;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventCreate   
//@CASEGROUP:2��CSUDIOSEventDestroy   
//@DESCRIPTION:����������������ʱ�ܷ񴴽��������¼�
//@PRECONDITION:
//@INPUT:1������="OS_TestOSEvent1"
//@INPUT:2���¼�ѡ��=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼�OS_TestOSEvent1��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSEventDestroy ���ٴ������¼����سɹ�
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0001( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventCreate   
//@CASEGROUP:2��CSUDIOSEventDestroy   
//@DESCRIPTION:�����¼���CSUDI_NULLʱ�ܷ�ɹ������������¼�
//@PRECONDITION:
//@INPUT:1������=CSUDI_NULL
//@INPUT:2���¼�ѡ��=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼���Ϊ�յ�ʱ�䣬Ӧ�÷���CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSEventDestroy ���ٴ������¼����سɹ�
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0002( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate(CSUDI_NULL,uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventCreate   
//@CASEGROUP:2��CSUDIOSEventDestroy   
//@DESCRIPTION:�����¼�ѡ��ΪCSUDIOS_EVENT_AUTO_RESETʱ�ܷ�ɹ������������¼�
//@PRECONDITION:
//@INPUT:1������="OS_TestOSEvent1"
//@INPUT:2���¼�ѡ��=CSUDIOS_EVENT_AUTO_RESET
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼�OS_TestOSEvent1��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSEventDestroy ���ٴ������¼����سɹ�
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0003( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_AUTO_RESET;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventCreate   
//@CASEGROUP:2��CSUDIOSEventDestroy   
//@DESCRIPTION:���������¼�ѡ���־ͬʱʹ��ʱ�ܷ�ɹ������������¼�
//@PRECONDITION:
//@INPUT:1������="OS_TestOSEvent1"
//@INPUT:2���¼�ѡ��=CSUDIOS_EVENT_INITIAL_STATUS|CSUDIOS_EVENT_AUTO_RESET
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼�OS_TestOSEvent1��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSEventDestroy ���ٴ������¼����سɹ�
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0004( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS|CSUDIOS_EVENT_AUTO_RESET;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventCreate   
//@CASEGROUP:2��CSUDIOSEventDestroy   
//@DESCRIPTION:�����¼�ѡ��Ϊ0ʱ�ܷ�ɹ������������¼�
//@PRECONDITION:
//@INPUT:1������="OS_TestOSEvent1"
//@INPUT:2���¼�ѡ��=0
//@EXPECTATION:�����ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼�OS_TestOSEvent1��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSEventDestroy ���ٴ������¼������سɹ��������������ʧ��
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0005( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�����ɹ����¼�����������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventCreate   
//@CASEGROUP:2��CSUDIOSEventDestroy   
//@DESCRIPTION:�����¼����Ƴ��ȳ���32�ֽ�ʱ�ܷ�ɹ������������¼�
//@PRECONDITION:
//@INPUT:1������="OS_TestOSEvent01234567890123456789"
//@INPUT:2���¼�ѡ��=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:����ʧ�ܲ�����CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼�OS_TestOSEvent01234567890123456789��Ҫ�󷵻�CSUDIOS_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2���ж�������¼����������ǿ������CSUDIOSEventDestroy ���ٴ������¼������ز�������ʧ�ܣ����򷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0006( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_BAD_PARAMETER == CSUDIOSEventCreate("OS_TestOSEvent01234567890123456789",uFlags,&hEventhandle), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == hEventhandle,"����ʧ�ܵ��¼�����������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventCreate   
//@CASEGROUP:2��CSUDIOSEventDestroy   
//@DESCRIPTION:�����¼�������ͬʱ�ܷ񴴽��������¼�
//@PRECONDITION:
//@INPUT:1������="OS_TestOSEvent1"
//@INPUT:2���¼�ѡ��=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:�����ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼�OS_TestOSEvent1��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2���ٴε���CSUDIOSEventCreate�����¼���ΪOS_TestOSEvent1���¼���Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSEventDestroy ���ٴ������¼����سɹ�
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0007( void )
{
	CSUDI_HANDLE hEventhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hEventhandle2 = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle1), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle1,"�ɹ��������¼�����������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle2), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle2,"�ɹ��������¼�����������Ϊ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle1), "�����¼�ʧ��");
			hEventhandle1 = CSUDI_NULL;
		}

		if(hEventhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle2), "�����¼�ʧ��");
			hEventhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1.CSUDIOSEventCreate
//@CASEGROUP:2.CSUDIOSEventDestroy
//@DESCRIPTION:�¼����������ٵ�ǿ�Ȳ���
//@PRECONDITION:
//@INPUT:1������="OS_TestOSEvent1"
//@INPUT:2���¼�ѡ��=CSUDIOS_EVENT_INITIAL_STATUS
//@EXPECTATION:�ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼�OS_TestOSEvent1��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSEventDestroy ���ٴ������¼����سɹ�
//@EXECUTIONFLOW:3���ظ�����1~2����OS_TEST_COUNT�Σ�OS_TEST_COUNTͨ�������ļ�����
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0008( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;
	int i;

	CSTCPrint("�¼�ǿ�Ȳ��Կ�ʼ����ȴ�������\n");

	for(i=0 ; i<OS_TEST_COUNT ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");

		hEventhandle = CSUDI_NULL;

	}
	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}
	}

	CSTCPrint("�¼�ǿ�Ȳ��Խ���������\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventWait 
//@CASEGROUP:2��CSUDIOSEventReset 
//@CASEGROUP:3��CSUDIOSEventSet 
//@DESCRIPTION:1�����Ե��¼���־����ΪCSUDIOS_EVENT_INITIAL_STATUSʱ�Ƚ��¼����ó���Ч�ٵȴ������
//@DESCRIPTION:2�����Եȴ������н��¼����ó���Ч���ܷ�ȵ��¼�
//@PRECONDITION:1�����Գɹ������¼�������
//@PRECONDITION:2�����Գɹ������¼�������
//@INPUT:��������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSEventCreate�����¼�������Ϊ"OS_TestOSEvent1"����־����ֵΪCSUDIOS_EVENT_INITIAL_STATUS�����Գɹ�����
//@EXECUTIONFLOW:2������CSUDIOSEventReset���¼���Ϊ��Ч
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate ����һ��������
//@EXECUTIONFLOW:4���������е���CSUDIOSEventWait�ȴ��¼�,��ʱʱ��Ϊ1500ms ��Ӧ�õȲ����¼�����������ȵ���g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:5�����������CSUDIOSThreadSleep �ȴ�1���Ӻ�,�ж�g_bThreadEnd�Ƿ�Ϊfalse,��������false�����ʧ��
//@EXECUTIONFLOW:6�����������CSUDIOSEventSet���¼���Ϊ��Ч
//@EXECUTIONFLOW:7����ʱ������Ӧ�õȵ��¼�,��g_bThreadEnd��Ϊtrue
//@EXECUTIONFLOW:8���������ж�g_bThreadEnd�Ƿ�Ϊtrue,������true�����ʧ��
//@EXECUTIONFLOW:9�����������CSUDIOSEventDestroy���ٴ������¼����سɹ�
//@EXECUTIONFLOW:10�����������CSUDIOSThreadDestroy ���ٴ��������񷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0001( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventReset(hEventhandle), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread1", 64, 8*1024, OS_TestEventThread1Entry, hEventhandle, &hThreadhandle), "���񴴽�ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "����6ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����8ʧ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "��������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventWait 
//@CASEGROUP:2��CSUDIOSEventReset 
//@CASEGROUP:3��CSUDIOSEventSet 
//@DESCRIPTION:���Ե��¼���־����ΪCSUDIOS_EVENT_INITIAL_STATUSʱ�ȴ��¼��ĳ�ʱ�ʹ��󷵻����
//@PRECONDITION:1�����Գɹ������¼�
//@PRECONDITION:2�����Գɹ������¼�
//@INPUT:��������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSEventCreate�����¼�������Ϊ"OS_TestOSEvent1"����־����ֵΪCSUDIOS_EVENT_INITIAL_STATUS�����Գɹ�����
//@EXECUTIONFLOW:2������CSUDIOSEventReset���¼���Ϊ��Ч
//@EXECUTIONFLOW:3������CSUDIOSEventWait�ȴ��¼�,��ʱʱ��Ϊ100����,Ӧ�ó�ʱ����CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:4������CSUDIOSEventWait�ȴ��¼�,��ʱʱ��Ϊ500����,Ӧ�ó�ʱ����CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:5������CSUDIOSEventWait�ȴ��¼�,��ʱʱ��ΪCSUDIOS_TIMEOUT_IMMEDIATE ,Ӧ�ó�ʱ����CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:6������CSUDIOSEventSet���¼���Ϊ��Ч
//@EXECUTIONFLOW:7������CSUDIOSEventWait�ȴ��¼�,��ʱʱ��Ϊ100����,Ӧ�óɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIOSEventDestroy���ٴ������¼���Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0002( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventReset(hEventhandle), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,100), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,500), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle,100), "����7ʧ��");
	
	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventWait 
//@CASEGROUP:2��CSUDIOSEventReset 
//@CASEGROUP:3��CSUDIOSEventSet 
//@DESCRIPTION:1�������¼��Ա�־����CSUDIOS_EVENT_INITIAL_STATUS�����������ȴ��Ƿ�ɹ�
//@DESCRIPTION:2�����Զ������ȴ�ͬһ�¼��Ƿ�ɹ�
//@PRECONDITION:1�����Գɹ������¼�������
//@PRECONDITION:2�����Գɹ������¼�������
//@INPUT:��������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSEventCreate�����¼�������Ϊ"OS_TestOSEvent1"����־����ֵΪCSUDIOS_EVENT_INITIAL_STATUS�����Գɹ�����
//@EXECUTIONFLOW:2������CSUDIOSEventWait�ȴ��¼�����ʱʱ��ΪCSUDIOS_TIMEOUT_IMMEDIATE��Ӧ�õȴ��ɹ�
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate ����һ�����������������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ1�룬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:4�����������н�g_nTestData��0�ĳ�1��֮�����CSUDIOSThreadSleep�ȴ�2��
//@EXECUTIONFLOW:5������CSUDIOSThreadSleep �ȴ�1����
//@EXECUTIONFLOW:6���������ж�g_nTestData�Ƿ�Ϊ1,��������1�����ʧ��
//@EXECUTIONFLOW:7�����������CSUDIOSEventReset���¼���Ϊ��Ч
//@EXECUTIONFLOW:8�����������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ3�룬Ӧ�õȲ����¼�
//@EXECUTIONFLOW:9�����������CSUDIOSThreadSleep�ȴ�2����ж�g_nTestData�Ƿ����1�������Ϊ1���������ʧ��
//@EXECUTIONFLOW:10�����������CSUDIOSEventSet���¼���Ϊ��Ч
//@EXECUTIONFLOW:11���������ʱӦ�ÿ��Եȵ��¼�������g_nTestData��1�ĳ�2
//@EXECUTIONFLOW:12���������ж�g_nTestData�Ƿ�Ϊ2,��������2�����ʧ��
//@EXECUTIONFLOW:13�����������CSUDIOSEventDestroy���ٴ������¼����سɹ�
//@EXECUTIONFLOW:14�����������CSUDIOSThreadDestroy ���ٴ��������񷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0003( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_INITIAL_STATUS;

	g_nTestData = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread2",64,8*1024,OS_TestEventThread2Entry,hEventhandle,&hThreadhandle), "���񴴽�ʧ��");

	CSTCPrint("\n��ȴ�5�롣����\n");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventReset(hEventhandle), "����7ʧ��");

	CSUDIOSThreadSleep(2000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"����9ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "����10ʧ��");

	CSUDIOSThreadSleep(100);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 2,"����12ʧ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "��������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventWait 
//@CASEGROUP:2��CSUDIOSEventReset 
//@CASEGROUP:3��CSUDIOSEventSet 
//@DESCRIPTION:1�������¼��Ա�־����CSUDIOS_EVENT_AUTO_RESET�����������ȴ��Ƿ�ɹ�
//@DESCRIPTION:2���¼��ȴ���������Ч����Ч���ۺϲ���
//@PRECONDITION:1�����Գɹ������¼�������
//@PRECONDITION:2�����Գɹ������¼�������
//@INPUT:��������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSEventCreate�����¼�������Ϊ"OS_TestOSEvent1"����־����ֵΪCSUDIOS_EVENT_AUTO_RESET �����Գɹ�����
//@EXECUTIONFLOW:2������CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ100ms��Ӧ�÷���CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate ����һ�����������������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ1�� ��Ӧ�õȲ����¼�
//@EXECUTIONFLOW:4������������ȵ��¼��ɹ���g_nTestData��0�ĳ�1��֮�����CSUDIOSThreadSleep�ȴ�100ms
//@EXECUTIONFLOW:5��������ȴ�500ms���ж�g_nTestData�Ƿ�Ϊ0,��������0�����ʧ��
//@EXECUTIONFLOW:6�����������CSUDIOSEventSet���¼���Ϊ��Ч֮�����CSUDIOSThreadSleep�ȴ�1��
//@EXECUTIONFLOW:7���������ж�g_nTestData�Ƿ�Ϊ1,��������1�����ʧ��
//@EXECUTIONFLOW:8�����������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ2�룬Ӧ�õȲ����¼�
//@EXECUTIONFLOW:9������������ȴ��¼��ɹ���g_nTestData��1�ĳ�2��Ȼ�����з���
//@EXECUTIONFLOW:10�����������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ100ms��Ӧ�÷���CSUDIOS_ERROR_TIMEOUT�������������ʧ��
//@EXECUTIONFLOW:11���������ж�g_nTestData�Ƿ�Ϊ1,��������1�����ʧ��
//@EXECUTIONFLOW:12�����������CSUDIOSEventSet���¼���Ϊ��Ч֮�����CSUDIOSThreadSleep�ȴ�2��
//@EXECUTIONFLOW:13���������ʱӦ�ÿ��Եȵ��¼�������g_dwTestEventData��1�ĳ�2
//@EXECUTIONFLOW:14���������ж�g_nTestData�Ƿ�Ϊ2,��������2�����ʧ��
//@EXECUTIONFLOW:15�����������CSUDIOSEventDestroy���ٴ������¼����سɹ�
//@EXECUTIONFLOW:16�����������CSUDIOSThreadDestroy ���ٴ��������񷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0004( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_AUTO_RESET;

	g_nTestData = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,100), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread3",64,8*1024,OS_TestEventThread3Entry,hEventhandle,&hThreadhandle), "���񴴽�ʧ��");

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 0,"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "����6ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,100), "����10ʧ��");

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 1,"����11ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "����12ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(g_nTestData == 2,"����14ʧ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "��������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventWait 
//@CASEGROUP:2��CSUDIOSEventReset 
//@CASEGROUP:3��CSUDIOSEventSet 
//@DESCRIPTION:���Դ������¼���־����ΪCSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUSʱ�ȴ���������Ч����Ч�����	
//@PRECONDITION:1�����Գɹ������¼�������
//@PRECONDITION:2�����Գɹ������¼�������
//@INPUT:��������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSEventCreate�����¼�������Ϊ"OS_TestOSEvent1"����־����ֵΪCSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUS �����Գɹ�����
//@EXECUTIONFLOW:2������CSUDIOSEventWait�ȴ��¼�����ʱʱ��ΪCSUDIOS_TIMEOUT_IMMEDIATE��Ӧ�õȴ��ɹ�
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate ����һ�����������������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ1500ms ��Ӧ�õȲ����¼�
//@EXECUTIONFLOW:4������������ȵ��¼��ɹ���g_bThreadEnd��false�ĳ�true
//@EXECUTIONFLOW:5���������ж�g_bThreadEnd�Ƿ�Ϊfalse,��������false�����ʧ��
//@EXECUTIONFLOW:6�����������CSUDIOSEventSet���¼���Ϊ��Ч֮�����CSUDIOSThreadSleep�ȴ�1��
//@EXECUTIONFLOW:7���������ж�g_bThreadEnd�Ƿ�Ϊtrue,��������true�����ʧ��
//@EXECUTIONFLOW:8�����������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ100ms��Ӧ�÷���CSUDIOS_ERROR_TIMEOUT�������������ʧ��
//@EXECUTIONFLOW:9�����������CSUDIOSEventDestroy���ٴ������¼����سɹ�
//@EXECUTIONFLOW:10�����������CSUDIOSThreadDestroy ���ٴ��������񷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0005( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = CSUDIOS_EVENT_AUTO_RESET|CSUDIOS_EVENT_INITIAL_STATUS;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread1",64,8*1024,OS_TestEventThread1Entry,hEventhandle,&hThreadhandle), "���񴴽�ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "����6ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,100), "����8ʧ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "��������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSEventWait 
//@CASEGROUP:2��CSUDIOSEventReset 
//@CASEGROUP:3��CSUDIOSEventSet 
//@DESCRIPTION:���Դ������¼���־����Ϊ 0 ʱ�ȴ���������Ч����Ч�����	
//@PRECONDITION:1�����Գɹ������¼�������
//@PRECONDITION:2�����Գɹ������¼�������
//@INPUT:��������
//@EXPECTATION:ÿһ���趼���Ԥ�ڽ��
//@EXECUTIONFLOW:1�����ýӿ�CSUDIOSEventCreate�����¼�������Ϊ"OS_TestOSEvent1"����־����ֵΪCSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUS �����Գɹ�����
//@EXECUTIONFLOW:2������CSUDIOSEventWait�ȴ��¼�����ʱʱ��ΪCSUDIOS_TIMEOUT_IMMEDIATE��Ӧ�÷���CSUDIOS_ERROR_TIMEOUT
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate ����һ�����������������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ1500ms ��Ӧ�õȲ����¼�
//@EXECUTIONFLOW:4������������ȵ��¼��ɹ���g_bThreadEnd��false�ĳ�true
//@EXECUTIONFLOW:5���������ж�g_bThreadEnd�Ƿ�Ϊfalse,��������false�����ʧ��
//@EXECUTIONFLOW:6�����������CSUDIOSEventSet���¼���Ϊ��Ч֮�����CSUDIOSThreadSleep�ȴ�1��
//@EXECUTIONFLOW:7���������ж�g_bThreadEnd�Ƿ�Ϊtrue,��������true�����ʧ��
//@EXECUTIONFLOW:8�����������е���CSUDIOSEventWait�ȴ��¼�����ʱʱ��Ϊ100ms��Ӧ�÷���CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:9�����������CSUDIOSEventDestroy���ٴ������¼����سɹ�
//@EXECUTIONFLOW:10�����������CSUDIOSThreadDestroy ���ٴ��������񷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0006( void )
{
	CSUDI_HANDLE hEventhandle = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	unsigned int uFlags = 0;

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1",uFlags,&hEventhandle), "�����¼�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT == CSUDIOSEventWait(hEventhandle,CSUDIOS_TIMEOUT_IMMEDIATE), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestEventThread1",64,8*1024,OS_TestEventThread1Entry,hEventhandle,&hThreadhandle), "���񴴽�ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bThreadEnd,"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "����6ʧ��");

	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd,"����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle,100), "����8ʧ��");

	CSTK_FATAL_POINT
	{
		if(hEventhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "�����¼�ʧ��");
			hEventhandle = CSUDI_NULL;
		}

		if(hThreadhandle != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "��������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSEventWait
//@CASEGROUP:CSUDIOSEventReset 
//@CASEGROUP:CSUDIOSEventSet 
//@DESCRIPTION:���Բ���ΪCSUDI_NULL��ʱ�򣬺�����ִ�н��
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventWait���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSEventReset���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSEventSet���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0007( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventWait(CSUDI_NULL, CSUDIOS_TIMEOUT_IMMEDIATE), "����1ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventReset(CSUDI_NULL), "����2ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventSet(CSUDI_NULL), "����3ʧ��");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSEventWait
//@CASEGROUP:CSUDIOSEventReset 
//@CASEGROUP:CSUDIOSEventSet 
//@DESCRIPTION:���Բ���ΪCSUDI_NULL��ʱ�򣬺�����ִ�н��
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventWait���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSEventReset���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSEventSet���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0008( void )
{
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventWait(CSUDI_NULL, CSUDIOS_TIMEOUT_IMMEDIATE), "����1ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventReset(CSUDI_NULL), "����2ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSEventSet(CSUDI_NULL), "����3ʧ��");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSEventWait
//@DESCRIPTION:���Բ���ΪCSUDIOS_TIMEOUT_INFINITY��ʱ��CSUDIOSEventWait��ִ�н��
//@PRECONDITION:
//@INPUT:CSUDI_NULL
//@EXPECTATION:����ÿ�����������
//@EXECUTIONFLOW:1������CSUDIOSEventCreate����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate�������������������еȴ�10s���ٵ���CSUDIOSEventSet������g_bThreadEnd��false��Ϊtrue
//@EXECUTIONFLOW:3������CSUDIOSEventWait���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ж�g_bThreadEnd��ֵ������ΪCSUDI_TRUE
//@EXECUTIONFLOW:5������CSUDIOSEventDestroy����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroy����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0009( void )
{
    CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
    CSUDI_HANDLE hEventhandle = CSUDI_NULL;
    unsigned int uFlags = CSUDIOS_EVENT_AUTO_RESET;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1", uFlags, &hEventhandle), "����1�������¼�ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

    g_bThreadEnd = CSUDI_FALSE;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestMutexThread4", 64, 8*1024, OS_TestEventThread4Entry, hEventhandle, &hThreadhandle), "����2����������ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"�ɹ���������������������Ϊ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventWait(hEventhandle, CSUDIOS_TIMEOUT_INFINITY), "����3���ȴ��¼�ʧ��");

    CSUDIOSThreadSleep(100);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bThreadEnd, "����4ʧ�ܣ�û�еȴ�10s�ͷ���");

    CSTK_FATAL_POINT
    
    if(hEventhandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventDestroy(hEventhandle), "����5�������¼�ʧ��");
        hEventhandle = CSUDI_NULL;
    }
    
    if(hThreadhandle != CSUDI_NULL)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "����6��ɾ������ʧ��");
        hThreadhandle = CSUDI_NULL;
    }
    
    return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIOSEventSet 
//@DESCRIPTION:����CSUDIOSEventSet�ӿڵ�ִ��ʱ��
//@PRECONDITION:CSUDIOSGetTime�ӿ�û������
//@INPUT:
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDIOSEventCreate�����¼�����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIOSGetTime��ȡϵͳʱ��sTime1����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIOSEventSet�����¼���Ч����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIOSGetTime��ȡϵͳʱ��sTime2����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������ִ��CSUDIOSEventSet��ʱ�䲢��ӡֵ��������ʱС��5ms
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0010( void )
{
    CSUDI_HANDLE hEventhandle = CSUDI_NULL;
    unsigned int uFlags = 0;
    CSUDIOSTimeVal_S sTime1;
    CSUDIOSTimeVal_S sTime2;
    int nMiliSecond = 0;
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventCreate("OS_TestOSEvent1", uFlags, &hEventhandle), "����1�������¼�ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hEventhandle,"�ɹ��������¼�����������Ϊ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime1), "����2����һ�λ�ȡϵͳʱ��ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSEventSet(hEventhandle), "����3�������¼���Чʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime2), "����4���ڶ��λ�ȡϵͳʱ��ʧ��");

    nMiliSecond = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond) ; 

    CSTCPrint("ִ��CSUDIOSSemRelease���ĵ�ʱ��Ϊ:%d(ms)\n", nMiliSecond);

    CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 5, "����5���ȴ���ʱ�䲻��ϵͳ�ݴ�ֵ��");

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMalloc 
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:����1000���ֽ��ڴ�ķ�����ͷ��Ƿ�ɹ�
//@PRECONDITION:
//@INPUT:������ڴ���С=1000
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMalloc����1000�ֽڵ��ڴ�ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2�����ڴ��д���250��4�ֽڵ��������ֱַ�Ϊ0,1��99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:3���Ӹ��ڴ��ж�����Ӧ�����ݲ�����У�飬Ӧ��ȫ��У��ɹ������в�ͬ�򱾲�������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
CSUDI_BOOL CSTC_OS_IT_MallocFree_0001( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uSize = 1000;
	int i;

	puChunk = (unsigned int*)CSUDIOSMalloc(uSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"�����ڴ�ʧ��");

	for(i=0 ; i<250 ; i++)
	{
		puChunk[i] = i%100;
	}

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk[i] == (i%100), "У�鲻�ɹ�");
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMalloc 
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:����8M�ڴ�ķ�����ͷ��Ƿ�ɹ�
//@PRECONDITION:
//@INPUT:������ڴ���С=8*1024*1024
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMalloc����8*1024*1024�ֽڵ��ڴ�ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2�����ڴ��д���2*1024*1024��4�ֽڵ��������ֱַ�Ϊ0,1��99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:3���Ӹ��ڴ��ж�����Ӧ�����ݲ�����У�飬Ӧ��ȫ��У��ɹ������в�ͬ�򱾲�������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
CSUDI_BOOL CSTC_OS_IT_MallocFree_0002( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uSize = 8*TEST_MEMORY_MALLOC_SIZE;
	int i;
 
	puChunk = (unsigned int*)CSUDIOSMalloc(uSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"�����ڴ�ʧ��");

	for(i=0 ; i<2*TEST_MEMORY_MALLOC_SIZE ; i++)  
	{
		puChunk[i] = i%100;  
	}
	
	for(i=0 ; i<2*TEST_MEMORY_MALLOC_SIZE ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk[i] == (i%100), "У�鲻�ɹ�");
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL) 
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMalloc 
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:8M�ڴ�ķ�����ͷ��Ƿ�ɹ���ǿ�Ȳ���
//@PRECONDITION:
//@INPUT:������ڴ���С=8*1024*1024
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMalloc����8*1024*1024�ֽڵ��ڴ�ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2�����ڴ��д���2*1024*1024��4�ֽڵ��������ֱַ�Ϊ0,1��99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:3���Ӹ��ڴ��ж�����Ӧ�����ݲ�����У�飬Ӧ��ȫ��У��ɹ������в�ͬ�򱾲�������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
//@EXECUTIONFLOW:5���ظ�1~4��OS_MEMORY_TEST_COUNT�Σ�OS_MEMORY_TEST_COUNTͨ�������ļ�����
CSUDI_BOOL CSTC_OS_IT_MallocFree_0003( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uSize = 8*TEST_MEMORY_MALLOC_SIZE;
	int i,j;

	CSTCPrint("�ڴ�ǿ�Ȳ��Կ�ʼ����ȴ�������\n");

	for(j=0 ; j<OS_MEMORY_TEST_COUNT ; j++)
	{
		CSTCPrint("\n��%d�������ڴ���Կ�ʼ!!!!!\n",j);
		
		puChunk = (unsigned int*)CSUDIOSMalloc(uSize);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"�����ڴ�ʧ��");

		for(i=0 ; i<2*TEST_MEMORY_MALLOC_SIZE ; i++)
		{
			puChunk[i] = i%100;
		}

		for(i=0 ; i<2*TEST_MEMORY_MALLOC_SIZE ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk[i] == (i%100), "У�鲻�ɹ�");
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
		puChunk = CSUDI_NULL;

		CSTCPrint("\n��%d�������ڴ���Խ���!!!!!\n",j);
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
			puChunk = CSUDI_NULL;
		}
	}

	CSTCPrint("�ڴ�ǿ�Ȳ��Խ���������\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMalloc 
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:�����ڴ��С����4���������ķ�����ͷ��Ƿ�ɹ�
//@PRECONDITION:
//@INPUT:������ڴ���С=1023
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMalloc����1023�ֽڵ��ڴ�ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2������memset��ÿ���ֽڶ�����ַ�'C'
//@EXECUTIONFLOW:3���Ӹ��ڴ��ж������һ���ֽڵ����ݿ��Ƿ����'C'���粻���򱾲�������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
CSUDI_BOOL CSTC_OS_IT_MallocFree_0004( void )
{
	char *puChunk = CSUDI_NULL;
	unsigned int uSize = 1023;

	puChunk = (char*)CSUDIOSMalloc(uSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"�����ڴ�ʧ��");

	memset(puChunk,'C',1023);

	CSTK_ASSERT_TRUE_FATAL(puChunk[1022] == 'C',"�������ַ��ʹ���Ĳ�һ��");
	
	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSMalloc 
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:���Է����СΪ0�ֽڵ��ڴ��Ƿ�ɹ�
//@PRECONDITION:
//@INPUT:������ڴ���С=0
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMalloc����0�ֽڵ��ڴ�ʧ�ܲ�����CSUDI_NULL
//@EXECUTIONFLOW:2���жϲ���1�ķ���ֵ���������CSUDI_NULL�����CSUDIOSFree�ͷŷ�����ڴ沢���ز�������ʧ�ܣ����򷵻سɹ�
CSUDI_BOOL CSTC_OS_IT_MallocFree_0005( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uSize = 0;

	puChunk = (unsigned int*)CSUDIOSMalloc(uSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL == puChunk,"����1ʧ��");

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSCalloc  
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:����С���ڴ�ķ�����ͷ��Ƿ�ɹ�
//@PRECONDITION:
//@INPUT:1������Ԫ�ظ���=4
//@INPUT:2��ÿ��Ԫ�صĴ�С=1000
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSCalloc����4��1000�ֽڵ��ڴ��ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2���Ӹ��ڴ��ж�����Ӧ�����ݲ�У�鿴�Ƿ񶼱��ɹ���ʼ��Ϊ0������ڷ�0�����򱾲�������ʧ��
//@EXECUTIONFLOW:3����ÿ���ڴ��д���250��4�ֽڵ��������ֱַ�Ϊ0,1��99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:4���Ӹ��ڴ��ж�����Ӧ�����ݲ�����У�飬Ӧ��ȫ��У��ɹ������в�ͬ�򱾲�������ʧ��
//@EXECUTIONFLOW:5������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
CSUDI_BOOL CSTC_OS_IT_CallocFree_0001( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uElements = 4;
	unsigned int uElementSize = 1000;
	int i,j;

	puChunk = (unsigned int*)CSUDIOSCalloc(uElements,uElementSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"�����ڴ�ʧ��");

	for(i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<250 ; j++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk[i*250+j] == 0,"������ڴ��Զ���ʼ��ʧ��");
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
			CSTK_ASSERT_TRUE_FATAL(puChunk[i*250+j] == j%100,"��������ݺͶ����Ĳ�һ��");
		}
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSCalloc  
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:����ڴ�ķ�����ͷ��Ƿ�ɹ���ǿ�Ȳ���
//@PRECONDITION:
//@INPUT:1������Ԫ�ظ���=4
//@INPUT:2��ÿ��Ԫ�صĴ�С=1000
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSCalloc����4��1000�ֽڵ��ڴ��ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2���Ӹ��ڴ��ж�����Ӧ�����ݲ�У�鿴�Ƿ񶼱��ɹ���ʼ��Ϊ0������ڷ�0�����򱾲�������ʧ��
//@EXECUTIONFLOW:3����ÿ���ڴ��д���250��4�ֽڵ��������ֱַ�Ϊ0,1��99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:4���Ӹ��ڴ��ж�����Ӧ�����ݲ�����У�飬Ӧ��ȫ��У��ɹ������в�ͬ�򱾲�������ʧ��
//@EXECUTIONFLOW:5������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
//@EXECUTIONFLOW:6���ظ�1~5��OS_MEMORY_TEST_COUNT�Σ�OS_MEMORY_TEST_COUNTͨ�������ļ�����
CSUDI_BOOL CSTC_OS_IT_CallocFree_0002( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	unsigned int uElements = 4;
	unsigned int uElementSize = 1000;
	int i,j,m;


	CSTCPrint("Callocǿ�Ȳ��Կ�ʼ����ȴ�������\n");

	for(m=0 ; m<OS_MEMORY_TEST_COUNT ; m++)
	{
		puChunk = (unsigned int*)CSUDIOSCalloc(uElements,uElementSize);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"�����ڴ�ʧ��");

		for(i=0 ; i<4 ; i++)
		{
			for(j=0 ; j<250 ; j++)
			{
				CSTK_ASSERT_TRUE_FATAL(puChunk[i*250+j] == 0,"������ڴ��Զ���ʼ��ʧ��");
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
				CSTK_ASSERT_TRUE_FATAL(puChunk[i*250+j] == j%100,"��������ݺͶ����Ĳ�һ��");
			}
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");

		puChunk = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
			puChunk = CSUDI_NULL;
		}
	}

	CSTCPrint("Callocǿ�Ȳ��Խ���������\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSRealloc   
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:�����ط�����ڴ��СС��������ڴ��С������ܷ��ط���ɹ�
//@PRECONDITION:CSUDIOSMalloc���Գɹ�ִ��
//@INPUT:1�����ط�����ڴ��ַ=CSUDIOSMalloc���ص��ڴ�ָ��
//@INPUT:2���ط�����ڴ��С=500
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMalloc����1000�ֽڵ��ڴ�ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2�����ڴ��д���250��4�ֽڵ��������ֱַ�Ϊ0,1��99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:3���Ӹ��ڴ��ж�����Ӧ�����ݲ�����У�飬Ӧ��ȫ��У��ɹ������в�ͬ�򱾲�������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSRealloc���·���500�ֽڵ��ڴ�
//@EXECUTIONFLOW:5�������·�����ڴ��ж�����Ӧ�����ݲ�У�飬Ӧ�ÿ���ȫ��У��ɹ�
//@EXECUTIONFLOW:6�������·�����ڴ��д���125��4�ֽڵ��������ֱַ�Ϊ0,1,2...  49��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:7�����ڴ��ж�����Ӧ�����ݲ�У�飬Ӧ�ÿ���ȫ��У��ɹ�
//@EXECUTIONFLOW:8������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0001( void )
{
	unsigned int *puChunk1 = CSUDI_NULL;
	unsigned int *puChunk2 = CSUDI_NULL;
	unsigned int uSize = 1000;
	int i;

	puChunk1 = (unsigned int*)CSUDIOSMalloc(uSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk1,"����1ʧ��");

	for(i=0 ; i<250 ; i++)
	{
		puChunk1[i] = i%100;
	}

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk1[i] == (i%100), "У�鲻�ɹ�");
	}

	puChunk2= (unsigned int*)CSUDIOSRealloc(puChunk1,500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk2,"����4ʧ��");
	
	puChunk1 = CSUDI_NULL;
	
	for(i=0 ; i<125 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "����5ʧ��");
	}

	for(i=0 ; i<125 ; i++)
	{
		puChunk2[i] = i%50;
	}

	for(i=0 ; i<125 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%50), "����7ʧ��");
	}

	CSTK_FATAL_POINT;
	
	{
		if(puChunk1 != puChunk2)
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "�ͷ��ڴ����");
				puChunk1 = CSUDI_NULL;
			}
			
			if (puChunk2 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk2), "�ͷ��ڴ����");
				puChunk2 = CSUDI_NULL;
			}
		}
		else
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "�ͷ��ڴ����");
				puChunk1 = CSUDI_NULL;
				puChunk2 = CSUDI_NULL;
			}
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSRealloc   
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:�����ط�����ڴ��С����������ڴ��С������ܷ��ط���ɹ�
//@PRECONDITION:CSUDIOSMalloc���Գɹ�ִ��
//@INPUT:1�����ط�����ڴ��ַ=CSUDIOSMalloc���ص��ڴ�ָ��
//@INPUT:2���ط�����ڴ��С=2000
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMalloc����1000�ֽڵ��ڴ�ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2�����ڴ��д���250��4�ֽڵ��������ֱַ�Ϊ0,1��99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:3���Ӹ��ڴ��ж�����Ӧ�����ݲ�����У�飬Ӧ��ȫ��У��ɹ������в�ͬ�򱾲�������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSRealloc���·���2000�ֽڵ��ڴ�
//@EXECUTIONFLOW:5�������·�����ڴ��ж���ǰ1000�ֽڵ����ݲ�У�飬Ӧ�ÿ���ȫ��У��ɹ�
//@EXECUTIONFLOW:6�������·�����ڴ��д���500��4�ֽڵ��������ֱַ�Ϊ0,1,2...  99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:7�����ڴ��ж�����Ӧ�����ݲ�У�飬Ӧ�ÿ���ȫ��У��ɹ�
//@EXECUTIONFLOW:8������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0002( void )
{
	unsigned int *puChunk1 = CSUDI_NULL;
	unsigned int *puChunk2 = CSUDI_NULL;
	unsigned int uSize = 1000;
	int i;

	puChunk1 = (unsigned int*)CSUDIOSMalloc(uSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk1,"����1ʧ��");

	for(i=0 ; i<250 ; i++)
	{
		puChunk1[i] = i%100;
	}

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk1[i] == (i%100), "У�鲻�ɹ�");
	}

	puChunk2= (unsigned int*)CSUDIOSRealloc(puChunk1,2000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk2,"����4ʧ��");

	puChunk1 = CSUDI_NULL;

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "����5ʧ��");
	}

	for(i=0 ; i<500 ; i++)
	{
		puChunk2[i] = i%100;
	}

	for(i=0 ; i<500 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "����7ʧ��");
	}

	CSTK_FATAL_POINT
	{
		if(puChunk1 != puChunk2)
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "�ͷ��ڴ����");
				puChunk1 = CSUDI_NULL;
			}
			
			if (puChunk2 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk2), "�ͷ��ڴ����");
				puChunk2 = CSUDI_NULL;
			}
		}
		else
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "�ͷ��ڴ����");
				puChunk1 = CSUDI_NULL;
				puChunk2 = CSUDI_NULL;
			}
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSRealloc   
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:�����ط�����ڴ��ַ����CSUDI_NULL������ܷ��ط���ɹ�
//@PRECONDITION:
//@INPUT:1�����ط�����ڴ��ַ=CSUDI_NULL
//@INPUT:2���ط�����ڴ��С=1000
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSRealloc���·���1000�ֽڵ��ڴ�
//@EXECUTIONFLOW:2�������·�����ڴ��д���250��4�ֽڵ��������ֱַ�Ϊ0,1,2...  99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:3�����ڴ��ж�����Ӧ�����ݲ�У�飬Ӧ�ÿ���ȫ��У��ɹ�
//@EXECUTIONFLOW:4������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0003( void )
{
	unsigned int *puChunk = CSUDI_NULL;
	int i;
	
	puChunk= (unsigned int*)CSUDIOSRealloc((unsigned int*)CSUDI_NULL,1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk,"����1ʧ��");

	for(i=0 ; i<250 ; i++)
	{
		puChunk[i] = i%100;
	}

	for(i=0 ; i<250 ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(puChunk[i] == (i%100), "У�鲻�ɹ�");
	}

	CSTK_FATAL_POINT;
	
	{
		if (puChunk != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk), "�ͷ��ڴ����");
			puChunk = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIOSRealloc   
//@CASEGROUP:2��CSUDIOSFree 
//@DESCRIPTION:�ڴ���ط�����ͷŵ�ǿ�Ȳ���
//@PRECONDITION:CSUDIOSMalloc���Գɹ�ִ��
//@INPUT:1�����ط�����ڴ��ַ=CSUDIOSMalloc���ص��ڴ�ָ��
//@INPUT:2���ط�����ڴ��С=2000
//@EXPECTATION:ÿһ���趼�ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������CSUDIOSMalloc����1000�ֽڵ��ڴ�ɹ��������ڴ��׵�ַ
//@EXECUTIONFLOW:2�����ڴ��д���250��4�ֽڵ��������ֱַ�Ϊ0,1��99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:3���Ӹ��ڴ��ж�����Ӧ�����ݲ�����У�飬Ӧ��ȫ��У��ɹ������в�ͬ�򱾲�������ʧ��
//@EXECUTIONFLOW:4������CSUDIOSRealloc���·���2000�ֽڵ��ڴ�
//@EXECUTIONFLOW:5�������·�����ڴ��ж���ǰ1000�ֽڵ����ݲ�У�飬Ӧ�ÿ���ȫ��У��ɹ�
//@EXECUTIONFLOW:6�������·�����ڴ��д���500��4�ֽڵ��������ֱַ�Ϊ0,1,2...  99��ѭ���������Դ���ɹ�
//@EXECUTIONFLOW:7�����ڴ��ж�����Ӧ�����ݲ�У�飬Ӧ�ÿ���ȫ��У��ɹ�
//@EXECUTIONFLOW:8������CSUDIOSFree�ͷ�������ڴ沢�ɹ�����
//@EXECUTIONFLOW:9���ظ�ִ��1~8��OS_MEMORY_TEST_COUNT�Σ�OS_MEMORY_TEST_COUNTͨ�������ļ�����
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0004( void )
{
	unsigned int *puChunk1 = CSUDI_NULL;
	unsigned int *puChunk2 = CSUDI_NULL;
	unsigned int uSize = 1000;
	int i,j;

	CSTCPrint("Reallocǿ�Ȳ��Կ�ʼ����ȴ�������\n");

	for(j=0 ; j<OS_MEMORY_TEST_COUNT ; j++)
	{
		puChunk1 = (unsigned int*)CSUDIOSMalloc(uSize);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk1,"����1ʧ��");

		for(i=0 ; i<250 ; i++)
		{
			puChunk1[i] = i%100;
		}

		for(i=0 ; i<250 ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk1[i] == (i%100), "У�鲻�ɹ�");
		}

		puChunk2= (unsigned int*)CSUDIOSRealloc(puChunk1,2000);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != puChunk2,"����4ʧ��");

		puChunk1 = CSUDI_NULL;
		
		for(i=0 ; i<250 ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "����5ʧ��");
		}

		for(i=0 ; i<500 ; i++)
		{
			puChunk2[i] = i%100;
		}

		for(i=0 ; i<500 ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(puChunk2[i] == (i%100), "����7ʧ��");
		}
		
		if(puChunk1 != puChunk2)
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "�ͷ��ڴ����");
				puChunk1 = CSUDI_NULL;
			}	
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk2), "�ͷ��ڴ����");
			puChunk2 = CSUDI_NULL;
		}
		else
		{
			if (puChunk1 != CSUDI_NULL)
			{			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "�ͷ��ڴ����");
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
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "�ͷ��ڴ����");
				puChunk1 = CSUDI_NULL;
			}
			
			if (puChunk2 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk2), "�ͷ��ڴ����");
				puChunk2 = CSUDI_NULL;
			}
		}
		else
		{
			if (puChunk1 != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(puChunk1), "�ͷ��ڴ����");
				puChunk1 = CSUDI_NULL;
				puChunk2 = CSUDI_NULL;
			}
		}
	}

	CSTCPrint("Reallocǿ�Ȳ��Խ���������\n");

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

//���Ե�������1K
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
					CSTCPrint("�ڴ��ͷ�ʧ��!!!");
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
		
		CSTCPrint("Ҫ��̽����������!!!");
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

	//�Է���
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
					/*�������һ���ڵ�*/
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

	//�ͷ�
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
				CSTCPrint("�ڴ��ͷ�ʧ��!!!");
			}
			
			pMem = (void *)pMemStruct;
		}
	}

	//̽�������ڴ��
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
				CSTCPrint("����ڴ���С����������³���!!!");
			}
		}
	}

	//ͳ��ȫ���ڴ�	
	uTotalSize = 0;	
	for (i=0; i < MAX_BASE_ARRAY_LEN; i++)
	{
		uTotalSize += anMemCount[i]*aBasicSize[i];
	}
	
	CSTCPrint("##################################\n");
	CSTCPrint("#�����ڴ���ܴ�С:0x%x�ֽ�\n",uTotalSize);
	CSTCPrint("#�����ڴ������:0x%x�ֽ�\n",uMaxBlock);
	CSTCPrint("##################################\n");
}



static void TestMemFragment(void)
{
	int nCount = 0;
	int nRand = 0;
	int nRand2 = 0;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	CSTCPrint("��Ƭ����֮ǰ���ڴ����\n");
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
				CSTCPrint("�ͷ��ڴ�ʧ��\n");
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
				CSTCPrint("�ͷ��ڴ�ʧ��\n");
			}
		}
	}	
	
	CSTCPrint("��Ƭ����֮����ڴ����\n");
	
	PrintMemorySize();
}

//@CASEGROUP:���Կ����ڴ��С   ��������������ڴ�
//@DESCRIPTION:�����ڴ��С����
//@PRECONDITION:CSUDIOSMalloc/CSUDIOSFree���Գɹ�ִ��
//@INPUT:
//@EXPECTATION:��ӡ���ϵͳ�����ڴ��С������ڴ��
//@EXECUTIONFLOW:
CSUDI_BOOL CSTC_OS_IT_Available_Memory( void )
{
	PrintMemorySize();
	
	return CSUDI_TRUE;
}

//@CASEGROUP:�����ڴ���Ƭ���
//@DESCRIPTION: ������䲻�̶��ڴ��С������ͷ��ѷ����ڴ������£��ڴ���Ƭ���
//@PRECONDITION:CSUDIOSMalloc/CSUDIOSFree���Գɹ�ִ��
//@INPUT:
//@EXPECTATION:�Աȴ�ӡ���ϵͳ�����ڴ���Ƭ����ǰ������ڴ��С������ڴ��
//@EXECUTIONFLOW:
CSUDI_BOOL CSTC_OS_IT_Memory_Fragments( void )
{
	TestMemFragment();
	
	return CSUDI_TRUE;
}
#endif

//@CASEGROUP:����ϵͳ���ڴ�Ķ�д����  
//@DESCRIPTION:���ڴ���г�ʼ������ֵ�Լ������Ȳ�������ȡ��д����
//@PRECONDITION:CSUDIOSMalloc/CSUDIOSFree���Գɹ�ִ��
//@INPUT:pucBuf ֵ�Ϸ�
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1��ʹ��memset������0��ÿ�β���1MB���������ظ�200��,����ȡʱ��
//@EXECUTIONFLOW:2��ʹ��memcpy������ÿ�β���1MB���������ظ�200�Σ�����ȡʱ��
//@EXECUTIONFLOW:3��ʹ�õ��ֽ�ֱ�Ӹ�ֵ��ÿ��ѭ������16�Σ�ѭ��200��1024��1024/16�Σ�����ȡʱ��
//@EXECUTIONFLOW:4��ʹ��4�ֽ�ֱ�Ӹ�ֵ��ÿ��ѭ������16�Σ�ѭ��200��1024��1024/64�Σ�����ȡʱ��
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

    CSTCPrint("���Կ�ʼ,�˲��Դ����Ҫ20s,�����ĵȴ�\n");

    pucBuf =  (CSUDI_UINT8 *)CSUDIOSMalloc( nMemory1M );

    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");

    //���ڲ���ʹ��memset������0��ÿ�β���1MB���������ظ�200��

    CSTCPrint("���ڽ����ڴ��memset������0����...\n");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp1),"��һ����ȡϵͳʱ��ʧ��");

    for ( i =0 ; i <  200 ; ++i)
    {
        memset(pucBuf, 0x0, nMemory1M);
    }

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"��һ����ȡϵͳʱ��ʧ��");

    temp1.m_nSecond = sSysTime.m_nSecond - temp1.m_nSecond;
    temp1.m_nMiliSecond = sSysTime.m_nMiliSecond - temp1.m_nMiliSecond;
    
    //ʹ��memcpy������ÿ�β���1MB���������ظ��ظ�200��
    CSTCPrint("���ڽ����ڴ��memcpy��������...\n");
    
    memset(pucBuf, 0x55, sizeof(pucBuf));

    pucBuf2 =  (CSUDI_UINT8 *)CSUDIOSMalloc( nMemory1M );

    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf2,"�����ڴ�ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp2),"�ڶ�����ȡϵͳʱ��ʧ��");

    for (i =0; i < 200 ; ++i)
    {
        memcpy(pucBuf2 , pucBuf , nMemory1M);
    }

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"�ڶ�����ȡϵͳʱ��ʧ��");

    temp2.m_nSecond = sSysTime.m_nSecond - temp2.m_nSecond;
    temp2.m_nMiliSecond = sSysTime.m_nMiliSecond - temp2.m_nMiliSecond;
     
    //ʹ�õ��ֽ�ֱ�Ӹ�ֵ��ÿ��ѭ������16�Σ�ѭ��200��1024��1024/16��

    CSTCPrint("���ڽ����ڴ�ĵ��ֽڸ�ֵ����...\n");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp3),"��������ȡϵͳʱ��ʧ��");

    for ( i =0 ; i < (nMemoryTestCount / 16) ; ++i )
    {
        for( j =0 ; j < 16 ; ++j )
        {
            cBufferByte = 0x55;
        }     
    }

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"��������ȡϵͳʱ��ʧ��");

    temp3.m_nSecond = sSysTime.m_nSecond - temp3.m_nSecond;
    temp3.m_nMiliSecond = sSysTime.m_nMiliSecond - temp3.m_nMiliSecond;
   
    //ʹ��4�ֽ�ֱ�Ӹ�ֵ��ÿ��ѭ������16�Σ�ѭ��200��1024��1024/64��

    CSTCPrint("���ڽ����ڴ�����ֽڸ�ֵ����...\n");    

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp4),"���Ĳ���ȡϵͳʱ��ʧ��");

    for ( i =0 ; i < ( nMemoryTestCount / 64) ; ++i)
    {
        for( j =0 ; j < 16 ; ++j )
        {
            uBufferDWord = 0x55bb55bb;
        }  
    }

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"���Ĳ���ȡϵͳʱ��ʧ��");

    temp4.m_nSecond = sSysTime.m_nSecond - temp4.m_nSecond;
    temp4.m_nMiliSecond = sSysTime.m_nMiliSecond - temp4.m_nMiliSecond;
   
    CSTCPrint("Memset ����1MB������200�ε�����Ϊ: %0.2f M/s \n " , (1.0 * 200 ) / ((temp1.m_nSecond*1000) + temp1.m_nMiliSecond ) * 1000);
    CSTCPrint("Memcpy ����1MB������200�ε�����Ϊ: %0.2f M/s\n " , (1.0 * 200 ) / ((temp2.m_nSecond*1000) + temp2.m_nMiliSecond ) * 1000);
    CSTCPrint("���ֽ�ֱ�Ӹ�ֵ������Ϊ: %0.2f M/s\n " , 200.0 / ((temp3.m_nSecond*1000) + temp3.m_nMiliSecond ) * 1000 );
    CSTCPrint("���ֽ�ֱ�Ӹ�ֵ������Ϊ: %0.2f M/s\n " , 200.0 / ((temp4.m_nSecond*1000) + temp4.m_nMiliSecond ) * 1000 );

    CSTK_FATAL_POINT
    {

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"������Դʧ��");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf2),"������Դʧ��");
    }    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetFreeRAM   
//@DESCRIPTION:���Ի�ȡ���õ�RAM�ռ��׼ȷ��
//@PRECONDITION:
//@INPUT:�Ϸ�����
//@EXPECTATION:��ȡ���Ŀ���RAM�ռ�ֵ��0�������ļ����������ܿռ�ֵ֮��
//@EXECUTIONFLOW:1������CSUDIOSGetFreeRAM(�Ϸ�����)��ȡ����RAM�ռ䣬Ӧ�÷��سɹ�
//@EXECUTIONFLOW:2���жϻ�ȡ���Ŀ��õ�RAM�ռ���0���ܿռ�ֵ֮��
CSUDI_BOOL CSTC_OS_IT_GetFreeRAM_0001( void )
{
	unsigned int nFreeCount=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetFreeRAM(&nFreeCount),"����һ:��ȡ���õ�RAM�ռ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL((nFreeCount >= 0 && nFreeCount <= g_nRAMSize),"��ȡ���Ŀ���RAM�ռ䲻��0���ܿռ䷶Χ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetFreeRAM   
//@DESCRIPTION:���Բ���Ϊ�Ƿ�ֵ���
//@PRECONDITION:
//@INPUT:�Ƿ�����
//@EXPECTATION:��ȡ����RAM�ռ�ʱ����ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSGetFreeRAM(CSUDI_NULL)��Ӧ�÷���ʧ��(��CSUDI_SUCCESS)
CSUDI_BOOL CSTC_OS_IT_GetFreeRAM_0002( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetFreeRAM(CSUDI_NULL),"����һ:����Ƿ�����Ӧ�÷���ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetCPUCount   
//@DESCRIPTION:���Ի�ȡCPU������׼ȷ��
//@PRECONDITION:
//@INPUT:�Ϸ�����
//@EXPECTATION:��ȡ����CPU�����������ļ���������ͬ
//@EXECUTIONFLOW:1������CSUDIOSGetCPUCount(�Ϸ�����)��ȡCPU������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:2���жϻ�ȡ����CPU�����������ļ���������һ��
CSUDI_BOOL CSTC_OS_IT_GetCPUCount_0001( void )
{
	unsigned int nCpuCount=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetCPUCount(&nCpuCount),"����һ:��ȡCPU����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nCpuCount == g_nCPUCount,"�����:��ȡ����CPU�����������ļ���һ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetCPUCount   
//@DESCRIPTION:���Բ���Ϊ�Ƿ�ֵ���
//@PRECONDITION:
//@INPUT:�Ƿ�����
//@EXPECTATION:��ȡCPU����ʱ����ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSGetCPUCount(CSUDI_NULL)��Ӧ�÷���ʧ��(��CSUDI_SUCCESS)
CSUDI_BOOL CSTC_OS_IT_GetCPUCount_0002( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetCPUCount(CSUDI_NULL),"��ȡCPU��������ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetCPUUsage   
//@DESCRIPTION:����CPU�����ʵ���ȷ��
//@INPUT:1��index��0��ʼ��С�������ļ���������CPU����
//@PRECONDITION:
//@INPUT:2��pnUsage�Ϸ�
//@EXPECTATION:��ȡ����ȷ��CPU������
//@EXECUTIONFLOW:1������CSUDIOSGetCPUUsage(���ڵ�����,�Ϸ�����)��Ӧ�÷��سɹ�
//@EXECUTIONFLOW:2���жϻ�ȡ����CPU��������0~100֮��
//@EXECUTIONFLOW:3���������п����������ظ�����1,2
CSUDI_BOOL CSTC_OS_IT_GetCPUUsage_0001( void )
{
	unsigned int nIndex=0;
	unsigned int nUsage=0;
	int i = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetCPUUsage(nIndex, &nUsage),"����һ:��ȡindex=0��CPU������ʧ��");

	CSTK_ASSERT_TRUE_FATAL((nUsage >= 0 && nUsage <= 100) ,"�����:��ȡ��CUP�����ʲ���0~100֮��");

	for ( i =0 ; i < g_nCPUCount ; ++i)
	{
		CSTCPrint("��ȡindex=%d��CPU��������\n", i);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetCPUUsage(i, &nUsage),"��ȡ���ڵ�������CPU�����ʣ�Ӧ�÷��سɹ�");

		CSTK_ASSERT_TRUE_FATAL((nUsage >= 0 && nUsage <= 100),"��ȡ��CUP�����ʲ���0~100֮��");
	}

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetCPUUsage   
//@DESCRIPTION:���Բ���Ϊ�Ƿ�ֵ�����
//@PRECONDITION:
//@INPUT:1��indexȡ�����ڵ�����ֵ
//@INPUT:2��pnUsageΪ�Ƿ�ֵ
//@EXPECTATION:��ȡCPU������ʱ����ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSGetCPUUsage(100,�Ϸ�����)��Ӧ�÷���ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:2������CSUDIOSGetCPUUsage(0,�Ƿ�����)��Ӧ�÷���ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:3������CSUDIOSGetCPUUsage(100,�Ƿ�����)��Ӧ�÷���ʧ��(��CSUDI_SUCCESS)
CSUDI_BOOL CSTC_OS_IT_GetCPUUsage_0002( void )
{
	unsigned int nIndex=100;
	unsigned int pnUsage=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetCPUUsage(nIndex, &pnUsage),"����һ:���벻���ڵ�������Ӧ�÷���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetCPUUsage(0, CSUDI_NULL),"�����:������ڵ�����������2�Ƿ���Ӧ�÷���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetCPUUsage(nIndex, CSUDI_NULL),"������:���벻���ڵ������Ҳ���2�Ƿ���Ӧ�÷���ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetAllThreadHandle   
//@DESCRIPTION:���Ի�ȡϵͳ����������ľ������ȷ��
//@PRECONDITION:����һ����֪���������
//@INPUT:�Ϸ������������㹻��
//@EXPECTATION:��ȡ��ȷ������������
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate����һ������ɹ�����¼������
//@EXECUTIONFLOW:2������CSUDIOSGetAllThreadHandle(�Ϸ�����,�Ϸ�����)��Ӧ�÷��سɹ�
//@EXECUTIONFLOW:3���жϻ�ȡ���������������С��С��1
//@EXECUTIONFLOW:4���жϻ�ȡ������������������һ������벽��1���������������
//@EXECUTIONFLOW:5������CSUDIOSThreadDestroyɾ������1����������Ӧ�÷��سɹ�
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
							OS_TestThread1Entry, &nParam, &hThreadhandle),"����һ:����һ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"����һ:�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetAllThreadHandle(ahThread, &nSize),"�����:����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nSize >= 1,"������:��ȡ�������������СС��1");
	
	for ( i = 0 ; i < nSize; i++)
	{
		if (ahThread[i] == hThreadhandle)
		{
			bHave = CSUDI_TRUE;
			break;
		}
	}
	CSTK_ASSERT_TRUE_FATAL(bHave == CSUDI_TRUE,"������:��ȡ���������������в���������1����������");

	CSTK_FATAL_POINT
	{
		while(!g_bThreadEnd)
		{
			CSUDIOSThreadSleep(10);
		}
		
		if (hThreadhandle != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "������:ɾ������1����������ʧ��");
			  hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetAllThreadHandle   
//@DESCRIPTION:���Ի�ȡϵͳ����������ľ������ȷ��
//@PRECONDITION:����������֪���������
//@INPUT:�Ϸ������������鲻�㹻��
//@EXPECTATION:��ȡ����ȷ������������
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������1�ɹ�
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate��������2�ɹ�
//@EXECUTIONFLOW:3������CSUDIOSGetAllThreadHandle(�Ϸ�����,1)��Ӧ�÷��سɹ�
//@EXECUTIONFLOW:4���жϻ�ȡ���������������СΪ1
//@EXECUTIONFLOW:5������CSUDIOSThreadDestroyɾ������1����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:6������CSUDIOSThreadDestroyɾ������2����������Ӧ�÷��سɹ�
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
							OS_TestThread1Entry, &nParam, &hThreadhandle1),"����һ:��������1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"����һ:�����ɹ�������1���������������Ϊ��");

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread2", nPriority , 2*TEST_THREAD_STACK_SIZE, 
								OS_TestThread1Entry, &nParam, &hThreadhandle2),"�����:��������2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����:�����ɹ�������2���������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetAllThreadHandle(ahThread, &uSize),"������:����ʧ��");

	while ((uCount<OS_MAX_THREAD_COUNT) && (ahThread[uCount]!=CSUDI_NULL))
	{
		uCount++;
	}

	CSTK_ASSERT_TRUE_FATAL(uCount == 1,"������ʧ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle1 != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "������:ɾ������1����������ʧ��");
			hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "������:ɾ������2����������ʧ��");
			hThreadhandle2 = CSUDI_NULL;
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetAllThreadHandle   
//@DESCRIPTION:���Ի�ȡϵͳ����������ľ������ȷ��
//@PRECONDITION:
//@INPUT:�Ϸ������������㹻��
//@EXPECTATION:1���������ʱ��ȡ���ľ�������д��ڸ�������
//@EXPECTATION:2������ɾ�����ȡ���ľ�������в����ڸ�������
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������ɹ�����¼������
//@EXECUTIONFLOW:2������CSUDIOSGetAllThreadHandle(�Ϸ�����,�Ϸ�����)��Ӧ�÷��سɹ�
//@EXECUTIONFLOW:3���жϻ�ȡ���������������д��ڲ���1����������ľ��
//@EXECUTIONFLOW:4������CSUDIOSThreadDestroyɾ������1����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:5���жϻ�ȡ���������������в����ڲ���1����������ľ��
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
							OS_TestThread1Entry, &nParam, &hThreadhandle),"����һ:����һ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"����һ:�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetAllThreadHandle(ahThread, &nSize),"�������ȡ����ľ��ʧ��");

	for ( i = 0 ; i < nSize; i++)
	{
		if (ahThread[i] == hThreadhandle)
		{
			bHave = CSUDI_TRUE;
			break;
		}
	}
	CSTK_ASSERT_TRUE_FATAL(bHave == CSUDI_TRUE,"������:��ȡ���������������в���������1����������");

	CSTK_FATAL_POINT

	if (hThreadhandle != CSUDI_NULL)
	{
		while(!g_bThreadEnd)
		{
			CSUDIOSThreadSleep(10);
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "������:ɾ������1����������ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetAllThreadHandle(ahThread, &nSize),"������:��ȡ����������ʧ��");

	bHave = CSUDI_FALSE;
	for ( i = 0 ; i < nSize; i++)
	{
		if (ahThread[i] == hThreadhandle)
		{
			bHave = CSUDI_TRUE;
			break;
		}
	}
	CSTK_ASSERT_TRUE_FATAL(bHave == CSUDI_FALSE,"������:��ȡ���������������д������Ѿ�ɾ����������");
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetAllThreadHandle   
//@DESCRIPTION:���Բ���Ϊ�Ƿ�ֵ�����
//@PRECONDITION:
//@INPUT:�Ƿ�����
//@EXPECTATION:��ȡ����������ʱ����ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSGetAllThreadHandle(CSUDI_NULL,�Ϸ�����)��Ӧ�÷���ʧ��
//@EXECUTIONFLOW:2������CSUDIOSGetAllThreadHandle(�Ϸ�����,ȡ0)��Ӧ�÷���ʧ��
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0004( void )
{
	CSUDI_HANDLE ahThread[OS_MAX_THREAD_COUNT] = {0};
	unsigned int nSize1 = 10;
	unsigned int nSize2 = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetAllThreadHandle(CSUDI_NULL, &nSize1),"����һ:����Ƿ�����Ӧ�÷���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetAllThreadHandle(ahThread, &nSize2),"�����:����Ƿ�����Ӧ�÷���ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadInfo   
//@DESCRIPTION:���Ի�ȡ������Ϣ����ȷ��
//@PRECONDITION:����һ����֪������������������Ϣ������
//@INPUT:�Ϸ�����
//@EXPECTATION:��ȡ����ȷ��������Ϣ
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������ɹ�����¼������������������Ϣ
//@EXECUTIONFLOW:2������CSUDIOSGetThreadInfo(���벽��1��¼��������)��ȡ������Ϣ��Ӧ�÷��سɹ�
//@EXECUTIONFLOW:3���жϻ�ȡ����������Ϣ�봴������ʱ����Ϣһ��
//@EXECUTIONFLOW:4������CSUDIOSThreadDestroyɾ������1����������Ӧ�÷��سɹ�
CSUDI_BOOL CSTC_OS_IT_GetThreadInfo_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	CSUDIOSThreadInfo_S sThreadInfo;

	memset(&sThreadInfo, 0, sizeof(sThreadInfo));

	g_bThreadEnd = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),"����һ:����һ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"����һ:�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetThreadInfo(hThreadhandle, &sThreadInfo),"�����:��ȡ������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(((sThreadInfo.uPriority == nPriority) && (sThreadInfo.uThreadId == (unsigned int)hThreadhandle) && (strcmp(sThreadInfo.szThreadName,"OS_TestThread1")==0)),"������:��ȡ��������Ϣ��������������Ϣ��һ��");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "������:ɾ������1����������ʧ��");
			hThreadhandle = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadInfo   
//@DESCRIPTION:���Բ���Ϊ�Ƿ�ֵ�����
//@PRECONDITION:
//@INPUT:�Ƿ�����
//@EXPECTATION:��ȡ������Ϣʱ����ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSGetThreadInfo(CSUDI_NULL, �Ϸ�����)��Ӧ�÷���ʧ��
//@EXECUTIONFLOW:2������CSUDIOSGetThreadInfo(CSUDI_NULL, CSUDI_NULL)��Ӧ�÷���ʧ��
CSUDI_BOOL CSTC_OS_IT_GetThreadInfo_0002( void )
{
	CSUDIOSThreadInfo_S sThreadInfo;
	
	memset(&sThreadInfo, 0, sizeof(sThreadInfo));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadInfo(CSUDI_NULL, &sThreadInfo),"����һ:����Ƿ�����Ӧ�÷���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadInfo(CSUDI_NULL, CSUDI_NULL),"�����:����Ƿ�����Ӧ�÷���ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadUsedRAM   
//@DESCRIPTION:���Ի�ȡָ�������RAM���ĵ���ȷ��
//@PRECONDITION:����һ����֪������������
//@INPUT:�Ϸ�����
//@EXPECTATION:��ȡ����ȷ��ָ�������RAM����
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������ɹ�����¼������
//@EXECUTIONFLOW:2������CSUDIOSGetThreadUsedRAM(����1��������, �Ϸ�����)��Ӧ�÷��سɹ�
//@EXECUTIONFLOW:3���жϲ���2��ȡ����RAM������0�������ļ�����������RAMֵ֮��
//@EXECUTIONFLOW:4������CSUDIOSThreadDestroyɾ������1����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:5������CSUDIOSGetThreadUsedRAM(����1��������, �Ϸ�����)��Ӧ�÷���ʧ��
CSUDI_BOOL CSTC_OS_IT_GetThreadUsedRAM_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	unsigned int nUsedRam = 0;

	g_bThreadEnd = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1Entry, &nParam, &hThreadhandle),"����һ:����һ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"����һ:�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetThreadUsedRAM(hThreadhandle, &nUsedRam),"�����:����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((nUsedRam >= 0 && nUsedRam <= g_nRAMSize),"������:��ȡ����1������������ռRAM�ռ䲻����Ч��Χ");

	CSTK_FATAL_POINT

	{
		if (hThreadhandle != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "������:ɾ������1����������ʧ��");
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadUsedRAM(hThreadhandle, &nUsedRam),"������:��ȡ��ɾ����������ռRAM�ռ�Ӧ�÷���ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadUsedRAM   
//@DESCRIPTION:���Բ���Ϊ�Ƿ�ֵ�����
//@PRECONDITION:
//@INPUT:�Ƿ�����
//@EXPECTATION:��ȡ�����RAM����ʱ����ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSGetThreadUsedRAM(CSUDI_NULL, �Ϸ�����)��Ӧ�÷���ʧ��
//@EXECUTIONFLOW:2������CSUDIOSGetThreadUsedRAM(CSUDI_NULL, CSUDI_NULL)��Ӧ�÷���ʧ��
CSUDI_BOOL CSTC_OS_IT_GetThreadUsedRAM_0002( void )
{
	unsigned int nUsedRam = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIOSGetThreadUsedRAM(CSUDI_NULL, &nUsedRam),"����һ:����Ƿ�����Ӧ�÷���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIOSGetThreadUsedRAM(CSUDI_NULL, CSUDI_NULL),"�����:����Ƿ�����Ӧ�÷���ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadCPUUsage   
//@DESCRIPTION:���Ի�ȡָ�������CPU���ص���ȷ��
//@PRECONDITION:����һ����֪������������
//@INPUT:�Ϸ�����
//@EXPECTATION:��ȡ����ȷ��ָ�������CPU����
//@EXECUTIONFLOW:1������CSUDIOSThreadCreate��������ɹ���������ѭ����һ������ѧ���㣬��¼������
//@EXECUTIONFLOW:2������CSUDIOSGetThreadCPUUsage(����1��������, �Ϸ�����)��Ӧ�÷��سɹ�
//@EXECUTIONFLOW:3���жϲ���2��ȡ����CPU����ֵ��0~100֮��
//@EXECUTIONFLOW:4��ͨ�������ļ������ж�ƽ̨�Ƿ�֧�������Suspend�����֧�������CSUDIOSThreadSuspend���ж��Ƿ񷵻سɹ���������4,5,6����
//@EXECUTIONFLOW:5������CSUDIOSGetThreadCPULoad(����1��������, �Ϸ�����)��Ӧ�÷��سɹ�
//@EXECUTIONFLOW:6���жϲ���5��ȡ����CPU����ֵС�ڲ���2��ȡ����ֵ
//@EXECUTIONFLOW:7������CSUDIOSThreadDestroyɾ������1����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW:8������CSUDIOSGetThreadCPULoad(����1��������, �Ϸ�����)��Ӧ�÷���ʧ��
CSUDI_BOOL CSTC_OS_IT_GetThreadCPULoad_0001( void )
{
	CSUDI_HANDLE hThreadhandle = CSUDI_NULL;
	int nPriority = 7;
	int nParam = 0;	
	unsigned int nUsage = 0;
	unsigned int nUsage2 = 0;

	g_bThreadEnd = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OS_TestThread1", nPriority , 2*TEST_THREAD_STACK_SIZE, 
							OS_TestThread1_1Entry, &nParam, &hThreadhandle),"����һ:����һ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle,"����һ:�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetThreadCPUUsage(hThreadhandle, &nUsage),"�����:����ʧ��");

	CSTK_ASSERT_TRUE_FATAL((nUsage >= 0 && nUsage <= 100),"��������ȡCPU����ֵ����0~100֮��");

	if (s_bSupportSuspend)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadSuspend(hThreadhandle),"������:���������������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetThreadCPUUsage(hThreadhandle, &nUsage2),"������:����ʧ��");

		CSTK_ASSERT_TRUE_FATAL(nUsage > nUsage2,"������:�õ���CPU���ش����ϴι�ʧ��");
	
	}

	CSTK_FATAL_POINT
	{
		if (hThreadhandle != CSUDI_NULL)
		{
			while(!g_bThreadEnd)
			{
				CSUDIOSThreadSleep(10);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle), "������:ɾ������1����������ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadCPUUsage(hThreadhandle, &nUsage),"�����:Ӧ�÷���ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOSGetThreadCPUUsage   
//@DESCRIPTION:���Բ���Ϊ�Ƿ�ֵ�����
//@PRECONDITION:
//@INPUT:�Ƿ�����
//@EXPECTATION:��ȡָ�������CPU����ʱ����ʧ��(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDIOSGetThreadCPUUsage(CSUDI_NULL, �Ϸ�����)��Ӧ�÷���ʧ��
//@EXECUTIONFLOW:2������CSUDIOSGetThreadCPUUsage(CSUDI_NULL, CSUDI_NULL)��Ӧ�÷���ʧ��
CSUDI_BOOL CSTC_OS_IT_GetThreadCPULoad_0002( void )
{
	unsigned int nUsage = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadCPUUsage(CSUDI_NULL, &nUsage),"����һ:Ӧ�÷���ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIOSGetThreadCPUUsage(CSUDI_NULL, CSUDI_NULL),"�����:Ӧ�÷���ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

