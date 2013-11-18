/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_invokenativetestcase.h"
#include "cs_invokenativetest.h"
#include "udi2_invokeNative.h"

#if 1
#define STACK_SIZE   "128"

CSUDI_Error_Code CSUDIInvokeNative(void * pfFunction, CSUDIInvokeArg_S * psArgs, int nArgsSize, 
	CSUDIInvokeValueType_E eReturnType, CSUDIInvokeValue_S * psReturnValue)
{
	unsigned int  * target=NULL;
	unsigned int  returnValue_temp = 0xFFFFFFFF;
	unsigned int ret_temp0 = 0xFFFFFFFF,ret_temp1 = 0xFFFFFFFF;
	int i = 0;

	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	 
	if (psReturnValue == NULL)
	{
		printf("returnValue = NULL in CSUDIInvokeNative");	
	}

	__asm__ ( "subu $sp, $sp,"STACK_SIZE"" );/*Create stackframe.*/
        __asm__ ( "move %0,$sp":"=r"(target): );
        __asm__ ( "addu $sp, "STACK_SIZE"" );

	/*���в���ѹջ*/
	for (i= 0; i<nArgsSize; i++)
	{	
		switch (psArgs[i].m_eType)
		{
		case EM_UDIINVOKE_VALUE_TYPEID_INT:
		case EM_UDIINVOKE_VALUE_TYPEID_CHAR:
		case EM_UDIINVOKE_VALUE_TYPEID_BYTE:
		case EM_UDIINVOKE_VALUE_TYPEID_SHORT:
		case EM_UDIINVOKE_VALUE_TYPEID_BOOLEAN:
			*target++ =(unsigned int)(psArgs[i].m_sArg.m_nLow);
			break;
			
		case EM_UDIINVOKE_VALUE_TYPEID_FLOAT:
			*target++ =*(unsigned int *)&(psArgs[i].m_sArg.m_nLow);
			break;
			
		case EM_UDIINVOKE_VALUE_TYPEID_LONG:
		case EM_UDIINVOKE_VALUE_TYPEID_DOUBLE:
			{              
                            unsigned int temp = (unsigned int)target;

                            if(temp % 8 != 0) target++;

                            *target++=psArgs[i].m_sArg.m_nLow;
                            *target++=psArgs[i].m_sArg.m_nHigh;	
			}
			break;
			
		case EM_UDIINVOKE_VALUE_TYPEID_OBJ:
			*target++ =(unsigned int)(psArgs[i].m_sArg.m_nLow);
			break;
			
		default:
			return CSUDIINVOKE_ERROR_BAD_PARAMETER;
		}
	
	}

	__asm__ ( "subu $sp, $sp,"STACK_SIZE"" );
	__asm__ ( "lw $4, 0($sp)" );/**a0-a3: ���������Ӻ�������ʱǰ4���Ǹ��������*/
	__asm__ ( "lw $5, 4($sp)" );/*a1*/
	__asm__ ( "lw $6, 8($sp)" );/*a2*/
	__asm__ ( "lw $7, 12($sp)" );/*a3*/
	__asm__ ( "move $25, %0" :: "r" (pfFunction) );
	
	__asm__ ( "jalr $25" );
	__asm__ ( "nop" );

	/* free stack */
	__asm__ ( "addu $sp, "STACK_SIZE"" );
	__asm__ ( "move %0 , $2": "=r"(ret_temp0): );
 	__asm__ ( "move %0, $3": "=r"(ret_temp1):  );
	
	returnValue_temp = ret_temp0;

	nErrCode = CSUDI_SUCCESS;
	
	if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_NONE || eReturnType == EM_UDIINVOKE_VALUE_TYPEID_ENDFUNC || eReturnType == EM_UDIINVOKE_VALUE_TYPEID_VOID) 
	{
		/*nothing to do*/
		nErrCode = CSUDIINVOKE_ERROR_BAD_PARAMETER;
	}
	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_INT)
	{
		psReturnValue->m_nLow = returnValue_temp;
	}

	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_SHORT)  
	{

		if (returnValue_temp&0x00008000) 
		{
			returnValue_temp=(returnValue_temp)|0xffff0000;
		}
		else
		{
			returnValue_temp=returnValue_temp&0x0000ffff;
		}

		psReturnValue->m_nLow =  returnValue_temp;
	}
	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_CHAR)  
	{
		returnValue_temp=returnValue_temp&0x0000ffff;
		psReturnValue->m_nLow =  returnValue_temp;
	}
	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_LONG)  
	{
		psReturnValue->m_nLow = ret_temp0;
		psReturnValue->m_nHigh = ret_temp1;
	}
	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_BYTE)  
	{
		if (returnValue_temp&0x00000080)
		{
			returnValue_temp=returnValue_temp |0xffffff00;
		}
		else
		{
			returnValue_temp=returnValue_temp&0x000000ff;
		}
		psReturnValue->m_nLow =  returnValue_temp;
	}
	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_FLOAT)  
	{
		psReturnValue->m_nLow = returnValue_temp;
	}
	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_DOUBLE)
	{
		psReturnValue->m_nLow = ret_temp0;
		psReturnValue->m_nHigh = ret_temp1;
	}
	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_BOOLEAN)
	{
		returnValue_temp = returnValue_temp&0x000000ff;
		psReturnValue->m_nLow =  returnValue_temp;
	}
	else if(eReturnType == EM_UDIINVOKE_VALUE_TYPEID_OBJ)
	{
		psReturnValue->m_nLow =  returnValue_temp;
	}
	else
	{
		/*bad param nothing to do*/
		nErrCode = CSUDIINVOKE_ERROR_BAD_PARAMETER;
	}
	
	return nErrCode;
}
#endif

//@TESTCASEID: CSTC_INVOKENATIVE_Init
//@CASEGROUP: 
//@DESCRIPTION:��ʼ�����Ի���,��ȡ�����ļ���Ϣ��
//@REMARK:�ýӿڱ�����ϵͳ��ʼ��ʱ���á�
BOOL CSTC_INVOKENATIVE_Init(void)
{
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_UnInit
//@CASEGROUP:
//@DESCRIPTION:ȥ��ʼ�����Ի���
BOOL CSTC_INVOKENATIVE_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0001
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_Testint��������ֻ��һ��int�Ͳ���ʱ�����������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0001(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[3];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 3;
	
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[2].m_sArg.m_nLow = TEST_INTVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestInt, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_INTVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0002
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestLong��������ֻ��һ��64λlong�Ͳ���ʱ�����������
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0002(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[3];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 3;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLong, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_LONG, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_LONGVALUE_LOW && sReturnValue.m_nHigh == TEST_LONGVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0003
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestFloat��������ֻ��һ��float�Ͳ���ʱ�����������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0003(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[3];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 3;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFloat, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_FLOAT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");

	printf("return value = %f\n", (float)*((float *)(&sReturnValue)));
	printf("return value = 0x%x\n", sReturnValue.m_nLow);
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_FLOATVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0004
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestDouble��������ֻ��һ��64λdouble�Ͳ���ʱ�����������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0004(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[3];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 3;
	
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[2].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestDouble, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_DOUBLE, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_DOUBLEVALUE_LOW && sReturnValue.m_nHigh == TEST_DOUBLEVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0005
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestIFL����������������������int,float,long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0005(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[2].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[4].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestIFL, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_INTVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0006
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestIFD����������������������int,float,double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0006(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[2].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestIFD, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_INTVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0007
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestIDL����������������������int,double,long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0007(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[2].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[3].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[4].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestIDL, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_INTVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0008
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestIDF����������������������int,double,float����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0008(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[2].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[3].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[4].m_sArg.m_nLow = TEST_FLOATVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestIDF, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_INTVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0009
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestILF����������������������int,long,float����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0009(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[2].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[3].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[4].m_sArg.m_nLow = TEST_FLOATVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestILF, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_INTVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0010
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestILD����������������������int,long,double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0010(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[2].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[3].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestILD, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_INTVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0011
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:�����ú���Invokenative_TestLDF����������������������long,double, float����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0011(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[3].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[4].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLDF, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_LONG, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_LONGVALUE_LOW && sReturnValue.m_nHigh == TEST_LONGVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0012
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestLDI����������������������long,double, int����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0012(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[3].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[4].m_sArg.m_nLow = TEST_INTVALUE;
	
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLDI, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_LONG, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_LONGVALUE_LOW && sReturnValue.m_nHigh == TEST_LONGVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0013
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestLFD����������������������long , float ,double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0013(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLFD, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_LONG, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_LONGVALUE_LOW && sReturnValue.m_nHigh == TEST_LONGVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0014
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestLFI����������������������long,float, int����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0014(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[4].m_sArg.m_nLow = TEST_INTVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLFI, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_LONG, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_LONGVALUE_LOW && sReturnValue.m_nHigh == TEST_LONGVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0015
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestLIF����������������������long, int, float ����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0015(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[3].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[4].m_sArg.m_nLow = TEST_FLOATVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLIF, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_LONG, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_LONGVALUE_LOW && sReturnValue.m_nHigh == TEST_LONGVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0016
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestLID����������������������long, int, double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0016(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[3].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLID, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_LONG, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_LONGVALUE_LOW && sReturnValue.m_nHigh == TEST_LONGVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0017
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFID����������������������float, int, double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0017(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[3].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFID, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_FLOAT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_FLOATVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0018
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFIL����������������������float, int, long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0018(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[3].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[4].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFIL, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_FLOAT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_FLOATVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}
//@TESTCASEID:CSTC_INVOKENATIVE_MT_0019
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFDL����������������������float, double, long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0019(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[3].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[4].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFDL, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_FLOAT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_FLOATVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0020
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFDI����������������������float, double, int����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0020(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[3].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[4].m_sArg.m_nLow = TEST_INTVALUE;
	
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFDI, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_FLOAT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_FLOATVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0021
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFLI����������������������float, long, int����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0021(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[3].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[4].m_sArg.m_nLow = TEST_INTVALUE;
	
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFLI, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_FLOAT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_FLOATVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0022
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFLD����������������������float, long, double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0022(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[3].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFLD, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_FLOAT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_FLOATVALUE),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0023
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestDIF����������������������double, int, float����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0023(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[2].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[3].m_sArg.m_nLow = TEST_INTVALUE;


	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[4].m_sArg.m_nLow = TEST_FLOATVALUE;

		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestDIF, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_DOUBLE, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_DOUBLEVALUE_LOW && sReturnValue.m_nHigh == TEST_DOUBLEVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0024
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestDIF����������������������double, int, long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0024(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[2].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[3].m_sArg.m_nLow = TEST_INTVALUE;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[4].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestDIL, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_DOUBLE, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_DOUBLEVALUE_LOW && sReturnValue.m_nHigh == TEST_DOUBLEVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0025
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestDFL����������������������double, float, long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0025(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[2].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[4].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestDFL, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_DOUBLE, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_DOUBLEVALUE_LOW && sReturnValue.m_nHigh == TEST_DOUBLEVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0026
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestDFI����������������������double, float, long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0026(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[2].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[4].m_sArg.m_nLow = TEST_INTVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestDFI, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_DOUBLE, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_DOUBLEVALUE_LOW && sReturnValue.m_nHigh == TEST_DOUBLEVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0027
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestDLI����������������������double, long, int����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0027(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[2].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[3].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[4].m_sArg.m_nLow = TEST_INTVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestDLI, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_DOUBLE, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_DOUBLEVALUE_LOW && sReturnValue.m_nHigh == TEST_DOUBLEVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0028
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestDLF����������������������double, long, float����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0028(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[5];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 5;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[2].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[3].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[4].m_sArg.m_nLow = TEST_FLOATVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestDLF, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_DOUBLE, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == TEST_DOUBLEVALUE_LOW && sReturnValue.m_nHigh == TEST_DOUBLEVALUE_HIGH),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0029
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestIIIII��������5������������int, int,int,int, int����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0029(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[7];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 7;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[2].m_sArg.m_nLow = TEST_INTVALUE;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[3].m_sArg.m_nLow = TEST_INTVALUE;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[4].m_sArg.m_nLow = TEST_INTVALUE;

	asArgs[5].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[5].m_sArg.m_nLow = TEST_INTVALUE;

	asArgs[6].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[6].m_sArg.m_nLow = TEST_INTVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestIIIII, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}


//@TESTCASEID:CSTC_INVOKENATIVE_MT_0030
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestLLLLL��������5������������long, long,long,long, long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0030(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[7];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 7;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[3].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[4].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[5].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[5].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[5].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[6].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[6].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[6].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLLLLL, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0031
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFFFFF��������5������������float, float,float,float, float����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0031(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[7];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 7;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[4].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[5].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[5].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[6].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[6].m_sArg.m_nLow = TEST_FLOATVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFFFFF, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0032
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestDDDDD��������5������������double, double,double,double, double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0032(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[7];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 7;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[2].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[3].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[3].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[5].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[5].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[5].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[6].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[6].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[6].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestDDDDD, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0033
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFFLLD��������5������������float, float,long,long, double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0033(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[7];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 7;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[4].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[5].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[5].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[5].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[6].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[6].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[6].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFFLLD, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}


//@TESTCASEID:CSTC_INVOKENATIVE_MT_0034
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFIFLFD��������6������������float, int ,float,long,float, double����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0034(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[8];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 8;
			
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[3].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[4].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[5].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[5].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[5].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[6].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[6].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[7].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[7].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[7].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFIFLFD, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0035
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestLFDFIFF��������7������������long,float,double, float,int, float, float����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0035(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[9];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 9;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[2].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[2].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[5].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[5].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[6].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[6].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[7].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[7].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[8].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[8].m_sArg.m_nLow = TEST_FLOATVALUE;
		
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestLFDFIFF, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0036
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestFFDDIILL��������8������������ float, float, double,double ,int,int, long, long����ʱ��������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0036(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[10];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 10;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[2].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[2].m_sArg.m_nLow = TEST_FLOATVALUE;
	
	asArgs[3].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
	asArgs[3].m_sArg.m_nLow = TEST_FLOATVALUE;

	asArgs[4].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[4].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[4].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[5].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
	asArgs[5].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
	asArgs[5].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;

	asArgs[6].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[6].m_sArg.m_nLow = TEST_INTVALUE;

	asArgs[7].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[7].m_sArg.m_nLow = TEST_INTVALUE;

	asArgs[8].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[8].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[8].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;

	asArgs[9].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
	asArgs[9].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
	asArgs[9].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestFFDDIILL, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0037
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestL16L�����������16����������long����ʱ�����������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0037(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[18];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 18;
	int i = 0;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	for(i=2; i<nArgsSize; i++)
	{
		asArgs[i].m_eType = EM_UDIINVOKE_VALUE_TYPEID_LONG;
		asArgs[i].m_sArg.m_nLow = TEST_LONGVALUE_LOW;
		asArgs[i].m_sArg.m_nHigh= TEST_LONGVALUE_HIGH;
	}
	
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestL16L, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}


//@TESTCASEID:CSTC_INVOKENATIVE_MT_0038
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestF16F�����������16����������float����ʱ�����������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0038(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[18];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 18;
	int i = 0;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	for(i=2; i<nArgsSize; i++)
	{
		asArgs[i].m_eType = EM_UDIINVOKE_VALUE_TYPEID_FLOAT;
		asArgs[i].m_sArg.m_nLow = TEST_FLOATVALUE;
	}
	
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestF16F, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0039
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestD16D�����������16����������double����ʱ�����������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0039(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[18];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 18;
	int i = 0;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	for(i=2; i<nArgsSize; i++)
	{
		asArgs[i].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
		asArgs[i].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
		asArgs[i].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
	}
	
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestD16D, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0040
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestD20D�����������20����������double����ʱ�����������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0040(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[22];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 22;
	int i = 0;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	for(i=2; i<nArgsSize; i++)
	{
		asArgs[i].m_eType = EM_UDIINVOKE_VALUE_TYPEID_DOUBLE;
		asArgs[i].m_sArg.m_nLow = TEST_DOUBLEVALUE_LOW;
		asArgs[i].m_sArg.m_nHigh= TEST_DOUBLEVALUE_HIGH;
	}
	
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestD20D, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

//@TESTCASEID:CSTC_INVOKENATIVE_MT_0041
//@CASEGROUP:CSUDIInvokeNative
//@DESCRIPTION:ǰ����Int���Ͳ����������������ͳ�ƣ��������������Ϣ�Ǵӵ�3��������ʼ�ģ������ú���Invokenative_TestI30I�����������30����������Int����ʱ�����������
//@PRECONDITION:��ʼ�����
//@EXPECTATION:  ִ����ȷ������CSUDI_SUCCESS�������ú�������ֵ��ȷ
//@EXECUTIONFLOW:1������CSUDIInvokeNativeִ�к�������
BOOL CSTC_INVOKENATIVE_MT_0041(void)
{
	CSUDI_Error_Code nErrCode = CSUDI_FAILURE;
	CSUDIInvokeArg_S asArgs[32];
	CSUDIInvokeValue_S sReturnValue;
	int nArgsSize = 32;
	int i = 0;
		
	asArgs[0].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[0].m_sArg.m_nLow = TEST_INTVALUE;
	
	asArgs[1].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
	asArgs[1].m_sArg.m_nLow = TEST_INTVALUE;
	
	for(i=2; i<nArgsSize; i++)
	{
		asArgs[i].m_eType = EM_UDIINVOKE_VALUE_TYPEID_INT;
		asArgs[i].m_sArg.m_nLow = TEST_INTVALUE;
	}
	
	nErrCode = CSUDIInvokeNative((void *)&Invokenative_TestI30I, asArgs,nArgsSize, EM_UDIINVOKE_VALUE_TYPEID_INT, &sReturnValue);

	CSTK_ASSERT_TRUE_FATAL((nErrCode == CSUDI_SUCCESS), "0.CSUDIInvokeNative ����ֵ����");
	
	CSTK_ASSERT_TRUE_FATAL((sReturnValue.m_nLow == 0),"1.У�����ݴ���");

	CSTK_FATAL_POINT;
	
	return TRUE;
}

