/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.������������������CSTC_��������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_toolsettestcase.h"
#include "cs_toolset_test.h"
#include <string.h>

//оƬ֧�ֻ�ȡChipID��
extern CSUDI_BOOL g_bSupportGetChipID;

//��оƬ��ID��Ϣ
extern CSUDICHIPID_S g_sCHIPID;

//���������кź�Keyֵ
extern CSUDISerialKey_S g_sSerialKey;

//ϵͳ֧�ֲ�֧��halt����
extern CSUDI_BOOL g_bSupportSysHalt;

CSUDI_BOOL CSTC_TOOLSET_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	CSTC_TOOLSET_InitCfg();
// 	printf("#####\n");
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_TOOLSET_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIGetChipID
//@DESCRIPTION:���Բ���psChipID = CSUDI_NULLʱ�����
//@PRECONDITION:��оƬ�ѳɹ���ʼ��
//@INPUT: psChipID = CSUDI_NULL 
//@EXPECTATION:���ط�CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIGetChipID�������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_TOOLSET_IT_GetChipID_0001(void)
{

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIGetChipID(CSUDI_NULL), "��������ʧ��");

    	CSTK_FATAL_POINT

	return CSUDI_TRUE;
} 

//@CASEGROUP: CSUDIGetChipID
//@DESCRIPTION:�����ܷ���ȷ�õ���оƬID��Ϣ
//@PRECONDITION:��оƬ�ѳɹ���ʼ��
//@INPUT:psChipIDֵ�Ϸ�
//@EXPECTATION:����CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1�������оƬ֧�ֻ�ȡChipID����CSUDIGetChipID��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�������õ�����оƬID��Ϣ�������ļ������оƬID��Ϣ��ͬ
//@EXECUTIONFLOW:3���������CSUDIGetChipID�������� CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_TOOLSET_IT_GetChipID_0002(void)
{
	CSUDICHIPID_S sChipID;
	
	if(CSUDI_TRUE == g_bSupportGetChipID)
	{
	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIGetChipID(&sChipID), "��һ����������ʧ��");
			
		CSTK_ASSERT_TRUE_FATAL(g_sCHIPID.m_nLength == sChipID.m_nLength, "�ڶ�����ȡChipID��Ϣʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(0 == (strcmp(g_sCHIPID.m_szChipID ,  sChipID.m_szChipID)), "�ڶ�����ȡChipID��Ϣʧ��");

	    	CSTK_FATAL_POINT
	}
	else
	{
	    	CSTK_ASSERT_TRUE_FATAL(CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED == CSUDIGetChipID(&sChipID), "����������ʧ��");
	}
	return CSUDI_TRUE;
    
} 

//@CASEGROUP: CSUDIGetSerialKey 
//@DESCRIPTION:���Բ���psSerialKey = CSUDI_NULLʱ�����
//@PRECONDITION:Ŀ������ѳɹ���ʼ��
//@INPUT:psSerialKey = CSUDI_NULL
//@EXPECTATION:���ط�CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIGetSerialKey�������ط�CSUDI_SUCCESS 
CSUDI_BOOL CSTC_TOOLSET_IT_GetSerialKey_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIGetSerialKey(CSUDI_NULL),"��������ʧ��");
	
    	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
    
} 

//@CASEGROUP: CSUDIGetSerialKey 
//@DESCRIPTION:������ȷ�õ�Ŀ��������кź�KEYֵʱ�����
//@PRECONDITION:Ŀ������ѳɹ���ʼ��
//@INPUT: psSerialKeyֵ�Ϸ�
//@EXPECTATION: ����CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIGetSerialKey��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�������õ���Ŀ��������к��������ļ������Ϣ��ͬ
//@EXECUTIONFLOW:3�������õ���KEYֵ�������ļ����KEYֵ��ͬ
CSUDI_BOOL CSTC_TOOLSET_IT_GetSerialKey_0002(void)
{
	CSUDISerialKey_S  sSerialKey;


   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIGetSerialKey(&sSerialKey),"��һ������ֵ��ΪCSUDI_SUCCESS ");

	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(sSerialKey.m_szSerial,g_sSerialKey.m_szSerial),"�ڶ�����ȡ���к�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(sSerialKey.m_szPairsKey,g_sSerialKey.m_szPairsKey),"��������ȡKEYֵʧ��");
	
	CSTK_FATAL_POINT
	 	
	return CSUDI_TRUE;
} 

//@CASEGROUP: CSUDISYSReboot
//@DESCRIPTION:�����ܷ�ɹ�ʵ��������Ŀ�����
//@PRECONDITION:Ŀ������ɹ���ʼ��
//@INPUT: 
//@EXPECTATION: ʵ�ֳɹ�����CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1������CSUDISYSReboot��������CSUDI_SUCCESS��Ŀ������������ɹ�
CSUDI_BOOL CSTC_TOOLSET_IT_Reboot_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSReboot(),"��������ʧ��"); 
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "û�гɹ�ʵ��������"); 
	
    CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSHalt
//@DESCRIPTION:�����ܷ�ɹ�ʵ��ϵͳ�ػ�
//@PRECONDITION:Ŀ������ɹ���ʼ��
//@INPUT: 
//@EXPECTATION: ʵ�ֳɹ�����CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1�����ƽ̨֧��ϵͳ�ػ�����CSUDISYSHalt��������CSUDI_SUCCESS���޷���ֵ��Ŀ������ػ�
//@EXECUTIONFLOW:1�����ƽ̨��֧��ϵͳ�ػ�����CSUDISYSHalt��������CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_TOOLSET_IT_Halt_0001(void)
{
	if(CSUDI_TRUE == g_bSupportSysHalt)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSHalt(),"��������ʧ��"); 
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "û�гɹ�ʵ��ϵͳ�ػ�"); 	
	}
	else
	    	CSTK_ASSERT_TRUE_FATAL(CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED == CSUDISYSHalt(),"��������ʧ��"); 

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:���Բ���pbReal = CSUDI_NULLʱ�����
//@PRECONDITION:ϵͳ�ɹ���ʼ��
//@INPUT: ����bStandbyֵ�Ϸ�
//@INPUT: ����nStandbyTimeֵ�Ϸ�
//@INPUT: ����pbReal = CSUDI_NULL
//@EXPECTATION: ���ط�CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1������CSUDISYSStandby������������ֵ��CSUDI_SUCCESS  
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0001(void)
{
   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDISYSStandby(CSUDI_TRUE,50,CSUDI_NULL),"��������ʧ��"); 

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:���Բ���nStandbyTime�Ƿ������
//@PRECONDITION:ϵͳ�ɹ���ʼ��
//@INPUT: ����bStandby = CSUDI_TRUE
//@INPUT: ����nStandbyTime = -1
//@INPUT: ����pbReal�ǿ�
//@EXPECTATION: ���ط�CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1������CSUDISYSStandby������������ֵ��CSUDI_SUCCESS  
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0002(void)
{
	CSUDI_BOOL bReal;
	
   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDISYSStandby(CSUDI_TRUE,-1,&bReal),"��������ʧ��"); 
		
    CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:�����ܷ�ɹ�ʵ�ִ������������Զ�����
//@PRECONDITION:ϵͳ�ɹ���ʼ��
//@INPUT: ����bStandby = CSUDI_TRUE  
//@INPUT: ����nStandbyTime = 20
//@INPUT: ����pbReal�ǿ�
//@EXPECTATION: ÿһ�����ﵽԤ�ڽ��
//@REMARK:�ֶ�����
//@EXECUTIONFLOW:1������CSUDISYSStandby��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����*pbReal����ΪCSUDI_FALSE��ִ��3��5��
//@EXECUTIONFLOW:3�������ʾ����ʾ�Ƿ�رռ�ǰ�������ʾ�Ƿ�Ϊ�ٴ�����ʾ״̬
//@EXECUTIONFLOW:4���������bStandbyΪ CSUDI_FALSEʱ����CSUDISYSStandby�������Ӽٴ�������
//@EXECUTIONFLOW:5�����ǰ�����ʾ����ʾ������ʾ�Ƿ�ȫ����ʾ����
//@EXECUTIONFLOW:6�����*pbReal����ΪCSUDI_TRUE��ִ�����²���
//@EXECUTIONFLOW:7�����ǰ�����ʾ����ʾ������ʾ�Ƿ��Ƿ�ȫ���ر�
//@EXECUTIONFLOW:8��������20���ϵͳ�˳�����״̬
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0003(void)
{
	 CSUDI_BOOL bReal;

	CSTCPrint("�����������״̬���ɹ�������20����Զ����ѣ���ע���ܷ����������ͻ���\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_TRUE,20,&bReal),"��һ���������״̬ʧ��");		

	if(bReal == CSUDI_FALSE)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_FALSE,20,&bReal),"���Ĳ��˳��ٴ�����������ʧ��");		
	}	
	else
	{
		printf("\n********************���Գɹ�******************************\n");	
	}
    	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:�����ܷ�ɹ�ʵ�ִ�����������ͨ��ң������������
//@PRECONDITION:ϵͳ�ɹ���ʼ��
//@INPUT: ����bStandby = CSUDI_TRUE  
//@INPUT: ����nStandbyTime = 0
//@INPUT: ����pbReal�ǿ�
//@EXPECTATION: ÿһ�����ﵽԤ�ڽ��
//@REMARK:�ֶ�����
//@EXECUTIONFLOW:1������CSUDISYSStandby��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����*pbReal����ΪCSUDI_FALSE��ִ��3��5��
//@EXECUTIONFLOW:3�������ʾ����ʾ�Ƿ�رռ�ǰ�������ʾ�Ƿ�Ϊ�ٴ�����ʾ״̬
//@EXECUTIONFLOW:4���������bStandbyΪ CSUDI_FALSEʱ����CSUDISYSStandby�������Ӽٴ�������
//@EXECUTIONFLOW:5�����ǰ�����ʾ����ʾ������ʾ�Ƿ�ȫ����ʾ����
//@EXECUTIONFLOW:6�����*pbReal����ΪCSUDI_TRUE��ִ�����²���
//@EXECUTIONFLOW:7�����ǰ�����ʾ����ʾ������ʾ�Ƿ��Ƿ�ȫ���ر�
//@EXECUTIONFLOW:8����ң�����������������ܹ���������
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0004(void)
{
	 CSUDI_BOOL bReal;

	CSTCPrint("�����������״̬���ɹ��������밴ң�������������ѣ���ע���ܷ����������ͻ���\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_TRUE,0,&bReal),"��һ���������״̬ʧ��");		

	if(bReal == CSUDI_FALSE)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_FALSE,60,&bReal),"���Ĳ��˳��ٴ�����������ʧ��");		
	}	
	else
	{
			
	}
    	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:�����ܷ�ɹ�ʵ�ִ�����������ͨ��ǰ��尴������
//@PRECONDITION:ϵͳ�ɹ���ʼ��
//@INPUT: ����bStandby = CSUDI_TRUE  
//@INPUT: ����nStandbyTime = 0
//@INPUT: ����pbReal�ǿ�
//@EXPECTATION: ÿһ�����ﵽԤ�ڽ��
//@REMARK:�ֶ�����
//@EXECUTIONFLOW:1�����ǰ����д�������ִ�����²��裬��������������
//@EXECUTIONFLOW:2������CSUDISYSStandby��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����*pbReal����ΪCSUDI_FALSE��ִ��3��5��
//@EXECUTIONFLOW:4�������ʾ����ʾ�Ƿ�رռ�ǰ�������ʾ�Ƿ�Ϊ�ٴ�����ʾ״̬
//@EXECUTIONFLOW:5���������bStandbyΪ CSUDI_FALSEʱ����CSUDISYSStandby�������Ӽٴ�������
//@EXECUTIONFLOW:6�����ǰ�����ʾ����ʾ������ʾ�Ƿ�ȫ����ʾ����
//@EXECUTIONFLOW:7�����*pbReal����ΪCSUDI_TRUE��ִ�����²���
//@EXECUTIONFLOW:8�����ǰ�����ʾ����ʾ������ʾ�Ƿ��Ƿ�ȫ���ر�
//@EXECUTIONFLOW:9����ǰ���������������ܹ���������
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0005(void)
{
	 CSUDI_BOOL bReal;

	CSTCPrint("���Դ������Ƿ���ͨ��ǰ��尴�����ѣ���ȷ��ǰ����Ƿ��д�����\n");
	CSTCPrint("y: �д���������������Ĳ��ԣ�n: �޴���������������\n");
	 CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "ǰ����޴���������������");

	CSTCPrint("�����������״̬���ɹ��������밴ǰ�����������ѣ���ע���ܷ����������ͻ���\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_TRUE,0,&bReal),"��һ���������״̬ʧ��");		

	if(bReal == CSUDI_FALSE)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_FALSE,60,&bReal),"���Ĳ��˳��ٴ�����������ʧ��");		
	}	
	else
	{
			
	}
    	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

