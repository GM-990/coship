/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_eepromtestcase.h"

#include "udi2_os.h"

//ǿ�Ȳ���ʱ�ظ�ִ�еĴ���
#define g_nCount 1000

//eeprom����ʼ��ַ 
extern CSUDI_UINT32 g_dwStartAddress;

//eeprom�ռ��С 
extern int g_nEepromSize;
//���߳��õ���ȫ�ֱ���
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
	//�ڱ�����������ִ��ǰ����
	CSTC_EEPROM_InitCfg();
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_EEPROM_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//����1��EEPROM��д��1024������0x55
void EEPROM_TestThread1Entry(void * pvParam)
{
	CSUDI_UINT8 pucData[1024];

	memset(pucData, 0x55, 1024);

	CSUDIEPRWrite(g_dwStartAddress, pucData, 1024);
	
	g_bThreadEnd1 = CSUDI_TRUE;
	
}
//����2��EEPROM��д��1024������0x88
void EEPROM_TestThread2Entry(void *pvParam)
{
	CSUDI_UINT8 pucData[1024];

	memset(pucData, 0x88, 1024);

	CSUDIEPRWrite(g_dwStartAddress, pucData, 1024);
	
	g_bThreadEnd2 = CSUDI_TRUE;

}

//����3��EEPROM��д��1024������0xee
void EEPROM_TestThread3Entry(void *pvParam)
{
	
	CSUDI_UINT8 pucData[1024];

	memset(pucData, 0xee, 1024);

	CSUDIEPRWrite(g_dwStartAddress, pucData, 1024);
	
	g_bThreadEnd3 = CSUDI_TRUE;

}

//����4��EEPROM����ʼ��ַ���ȡ1024������������ȫΪ0x55
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
//����5��EEPROM����ʼ��ַ+512���ȡ1024������������ǰ512��Ϊ0x55����512��Ϊ0xee
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

//����6��EEPROM����ʼ��ַ+1024���ȡ1024������������ȫΪ0xee
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

//����7��EEPROM��д��1024������0xee
void EEPROM_TestThread7Entry(void *pvParam)
{
	
	CSUDI_UINT8 pucData[1024];

	memset(pucData, 0xee, 1024);

	CSUDIEPRWrite(g_dwStartAddress+1024, pucData, 1024);
	
	g_bThreadEnd7 = CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRGetInfo  
//@DESCRIPTION:���Բ���psEPRInfo = CSUDI_NULLʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:psEPRInfo = CSUDI_NULL
//@EXPECTATION:CSUDIEEPROM_ERROR_BAD_PARAMETER
//@REMARK:
//@EXECUTIONFLOW:1�����ú���CSUDIEPRGetInfo��������CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRGetInfo_0001(void)
{
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIEEPROM_ERROR_BAD_PARAMETER == CSUDIEPRGetInfo(CSUDI_NULL),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRGetInfo  
//@DESCRIPTION:�����ܷ���ȷ�õ�оƬ��Ϣʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:psEPRInfoֵ�Ϸ�
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1�����ú���CSUDIEPRGetInfo��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�������õ���EEPROMоƬ��Ϣ�������ļ������ͬ
CSUDI_BOOL  CSTC_EEPROM_IT_EPRGetInfo_0002(void)
{
	CSUDIEPRInfo_S s_EprInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRGetInfo(&s_EprInfo),"��һ������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(g_dwStartAddress == s_EprInfo.m_dwStartAddress,"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(g_nEepromSize == s_EprInfo.m_nSize,"�ڶ�������ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:���Բ���dwStartAddress�Ƿ������
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress ֵ���Ϸ�
//@INPUT:pucData ֵ�Ϸ�
//@INPUT:uDataLength = 16
//@EXPECTATION:����CSUDI_FAILURE
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite(g_dwStartAddress + g_nEepromSize, pucData, 16)��������CSUDI_FAILURE
//@EXECUTIONFLOW:2������CSUDIEPRWrite(g_dwStartAddress - 0x0001, pucData, 16)��������CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0001(void)
{

	CSUDI_UINT8 pucData[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress + g_nEepromSize, pucData, 16),"����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress - 0x0001, pucData, 16),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:���Բ���pucData = CSUDI_NULLʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData = CSUDI_NULL 
//@INPUT:uDataLength = 1024
//@EXPECTATION:����CSUDI_FAILURE
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIEPRWrite(g_dwStartAddress, CSUDI_NULL, 1024)��������CSUDIEEPROM_ERROR_BAD_PARAMETER
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0002(void)
{
		
	CSTK_ASSERT_TRUE_FATAL(CSUDIEEPROM_ERROR_BAD_PARAMETER == CSUDIEPRWrite(g_dwStartAddress, CSUDI_NULL, 1024),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:���Բ���uDataLengthֵ���Ϸ������
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLengthֵ���Ϸ�
//@EXPECTATION:����CSUDI_FAILURE
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize + 1)��������CSUDI_FAILURE
//@EXECUTIONFLOW:2������CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize + 1024)��������CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0003(void)
{


	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize + 1);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "������Դʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize + 1),"��һ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize + 1024),"�ڶ�������ʧ��");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"������Դʧ��"); 
	
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:����Ҫд�������dwStartAddress +  uDataLength����EEPROM�ռ�ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress + 1024
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength = g_nEepromSize - 1023
//@EXPECTATION:����CSUDI_FAILURE
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite(g_dwStartAddress + 1024, pucData, g_nEepromSize - 1023)��������CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0004(void)
{
	CSUDI_UINT8 *pucData;
	
	printf("\nliubing test g_nEepromSize:%d+++++++++++++\n",g_nEepromSize);

	pucData= (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize - 1023);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "������Դʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRWrite(g_dwStartAddress + 1024, pucData, g_nEepromSize - 1023),"����ʧ��");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"������Դʧ��");
	
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@DESCRIPTION:����uDataLength = 0ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength = 0
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIEPRWrite(g_dwStartAddress, pucData, 0)��������CSUDI_SUCCESS
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWrite_0005(void)
{

	CSUDI_UINT8 pucData[1024];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, 0),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:���Բ���dwStartAddress�Ƿ������
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress ֵ���Ϸ�
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength = 16
//@EXPECTATION:����CSUDI_FAILURE
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRRead(g_dwStartAddress + g_nEepromSize,pucData, 16)��������CSUDI_FAILURE
//@EXECUTIONFLOW:2������CSUDIEPRRead(g_dwStartAddress - 0x0001, pucData, 16)��������CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0001(void)
{

	CSUDI_UINT8 pucData[16];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRRead(g_dwStartAddress + g_nEepromSize,pucData, 16),"����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRRead(g_dwStartAddress - 0x0001,pucData, 16),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:���Բ���pucData = CSUDI_NULL ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData = CSUDI_NULL 
//@INPUT:uDataLength = 1024
//@EXPECTATION:����CSUDIEEPROM_ERROR_BAD_PARAMETER
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRRead(g_dwStartAddress, CSUDI_NULL, 1024)��������CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0002(void)
{
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIEEPROM_ERROR_BAD_PARAMETER == CSUDIEPRRead(g_dwStartAddress, CSUDI_NULL, 1024),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:���Բ���uDataLengthֵ���Ϸ������
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLengthֵ���Ϸ�
//@EXPECTATION:����CSUDI_FAILURE
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRRead(g_dwStartAddress, pucData,  g_nEepromSize + 1)��������CSUDI_FAILURE
//@EXECUTIONFLOW:2������CSUDIEPRRead(g_dwStartAddress, pucData,  g_nEepromSize + 1024)��������CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0003(void)
{

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize + 1);		
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "������Դʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRRead(g_dwStartAddress, pucData,  g_nEepromSize + 1),"����ʧ��");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:����Ҫ��ȡ������dwStartAddress +  uDataLength����EEPROM�ռ�ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress + 1024
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength = g_nEepromSize - 1023
//@EXPECTATION:����CSUDI_FAILURE
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRRead(g_dwStartAddress + 1024, pucData, g_nEepromSize - 1023)��������CSUDI_FAILURE
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0004(void)
{
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize - 1023);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "������Դʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FAILURE == CSUDIEPRRead(g_dwStartAddress + 1024, pucData, g_nEepromSize - 1023),"����ʧ��");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRRead  
//@DESCRIPTION:����uDataLength = 0ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength = 0
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIEPRRead(g_dwStartAddress, pucData, 0)��������CSUDI_SUCCESS
CSUDI_BOOL  CSTC_EEPROM_IT_EPRRead_0005(void)
{
	CSUDI_UINT8 pucData[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, 0),"����ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:���Զ�д����EEPROM�ռ�ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength = g_nEepromSize
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1�����ú���CSUDIEPRWrite��EEPROM��д��pucDataָ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����ú���CSUDIEPRRead��ȡEEPROM�е�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��������ȡ��������д���������ͬ
//@EXECUTIONFLOW:4�����ú���CSUDIEPRWrite��EEPROM��д�������鲻ͬ��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5�����ú���CSUDIEPRRead��ȡEEPROM�е�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6��������ȡ��������д���������ͬ
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0001(void)
{
	int i;
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData, "������Դʧ��");
	
	memset(pucData,'a',g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize),"��һ������ʧ��");

	memset(pucData,'\0',g_nEepromSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, g_nEepromSize),"�ڶ�������ʧ��");

	for(i=0 ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"����������ʧ��");
	}
	
	memset(pucData,0x55,g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, g_nEepromSize),"���Ĳ�����ʧ��");

	memset(pucData,'\0',g_nEepromSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, g_nEepromSize),"���岽����ʧ��");

	for(i=0 ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucData[i],"����������ʧ��");
	}
	
	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"�ͷ��ڴ�ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:�������ַ��д�������ַ�ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress�ֱ�ȡg_dwStartAddress + 0x1001��g_dwStartAddress + 0x0003
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength =  1023
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��dwStartAddress = g_dwStartAddress + 0x1001ʱ���ú���CSUDIEPRWrite��EEPROM��д��1023��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ú���CSUDIEPRRead��g_dwStartAddress + 0x1001��ַ���ȡ1023��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��������ȡ��������ڶ���д���������ͬ
//@EXECUTIONFLOW:5������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff
//@EXECUTIONFLOW:6��dwStartAddress = g_dwStartAddress + 0x0003ʱ���ú���CSUDIEPRWrite��EEPROM��д��1023��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7�����ú���CSUDIEPRRead ��g_dwStartAddress + 0x0003��ַ���ȡ1023��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8��������ȡ�������������д���������ͬ
//@EXECUTIONFLOW:9������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff
//@EXECUTIONFLOW:10��dwStartAddress = g_dwStartAddress + 0x0003ʱ���ú���CSUDIEPRWrite��EEPROM��д��1023������0x55��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11�����ú���CSUDIEPRRead��ȡEEPROM�е�����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12��������ȡ�����ݴ�0x0003��ʼ��1023���ֽ�Ϊ0x55�����������ȫΪ0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0002(void)
{
	int i;

	CSUDI_UINT8 pucData[1023];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");

	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");

	memset(pucData, 'a', sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x1001, pucData, 1023),"�ڶ�������ʧ��");

	memset(pucData, '\0', sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x1001, pucData, 1023),"����������ʧ��");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"���Ĳ�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"���岽����ʧ��");
	
	memset(pucData,'8',sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0003, pucData, 1023),"����������ʧ��");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x0003, pucData, 1023),"���߲�����ʧ��");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('8' == pucData[i],"�ڰ˲�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"�ھŲ�����ʧ��");
	
	memset(pucData, 0x55, sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0003, pucData, 1023),"��ʮ������ʧ��");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"��ʮһ������ʧ��");

	for(i=0 ;i < 0x0003; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}
	for(  ;i < 0x0003 + 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"��ʮ��������ʧ��");
	}	
	for(  ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}
		
	CSTK_FATAL_POINT
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:�������ַ��дż�����ַ�ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress�ֱ�ȡg_dwStartAddress + 0x1001��g_dwStartAddress + 0x0003
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength =  1024
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��dwStartAddress = g_dwStartAddress + 0x1001ʱ���ú���CSUDIEPRWrite��EEPROM��д��1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ú���CSUDIEPRRead��g_dwStartAddress + 0x1001��ַ���ȡ1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��������ȡ��������ڶ���д���������ͬ
//@EXECUTIONFLOW:5������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff
//@EXECUTIONFLOW:6��dwStartAddress = g_dwStartAddress + 0x0003ʱ���ú���CSUDIEPRWrite��EEPROM��д��1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7�����ú���CSUDIEPRRead ��g_dwStartAddress + 0x0003��ַ���ȡ1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8��������ȡ�������������д���������ͬ
//@EXECUTIONFLOW:9������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff
//@EXECUTIONFLOW:10��dwStartAddress = g_dwStartAddress + 0x0003ʱ���ú���CSUDIEPRWrite��EEPROM��д��1024������0x55��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11�����ú���CSUDIEPRRead��ȡEEPROM�е�����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12��������ȡ�����ݴ�0x0003��ʼ��1024���ֽ�Ϊ0x55�����������ȫΪ0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0003(void)
{
	int i;

	CSUDI_UINT8 pucData[1024];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");

	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");

	memset(pucData, 'a', sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x1001, pucData, 1024),"�ڶ�������ʧ��");

	memset(pucData, '\0', sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x1001, pucData, 1024),"����������ʧ��");

	for(i=0 ;i < 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"���Ĳ�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"���岽����ʧ��");
	
	memset(pucData,'8',sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0003, pucData, 1024),"����������ʧ��");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x0003, pucData, 1024),"���߲�����ʧ��");

	for(i=0 ;i < 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('8' == pucData[i],"�ڰ˲�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"�ھŲ�����ʧ��");
	
	memset(pucData, 0x55, sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0003, pucData, 1024),"��ʮ������ʧ��");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"��ʮһ������ʧ��");

	for(i=0 ;i < 0x0003; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}
	for(  ;i < 0x0003 + 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"��ʮ��������ʧ��");
	}	
	for(  ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
		
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:����ż��ַ��д�������ַ�ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress�ֱ�ȡg_dwStartAddress��g_dwStartAddress + 0x0026
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength =  1023
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��dwStartAddress = g_dwStartAddressʱ���ú���CSUDIEPRWrite��EEPROM��д��1023��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ú���CSUDIEPRRead��g_dwStartAddress��ַ���ȡ1023��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��������ȡ��������ڶ���д���������ͬ
//@EXECUTIONFLOW:5������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff
//@EXECUTIONFLOW:6��dwStartAddress = g_dwStartAddress + 0x0026ʱ���ú���CSUDIEPRWrite��EEPROM��д��1023��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7�����ú���CSUDIEPRRead ��g_dwStartAddress + 0x0026��ַ���ȡ1023��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8��������ȡ�������������д���������ͬ
//@EXECUTIONFLOW:9������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff
//@EXECUTIONFLOW:10��dwStartAddress = g_dwStartAddress + 0x0026ʱ���ú���CSUDIEPRWrite��EEPROM��д��1023������0x55��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11�����ú���CSUDIEPRRead��ȡEEPROM�е�����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12��������ȡ�����ݴ�0x0003��ʼ��1023���ֽ�Ϊ0x55�����������ȫΪ0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0004(void)
{
	int i;

	CSUDI_UINT8 pucData[1023];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");

	memset(pucData, 'a', sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, 1023),"�ڶ�������ʧ��");

	memset(pucData, '\0', sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, 1023),"����������ʧ��");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"���Ĳ�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"���岽����ʧ��");
	
	memset(pucData,'8',sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0026, pucData, 1023),"����������ʧ��");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x0026, pucData, 1023),"���߲�����ʧ��");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('8' == pucData[i],"�ڰ˲�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"�ھŲ�����ʧ��");
	
	memset(pucData, 0x55, sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0026, pucData, 1023),"��ʮ������ʧ��");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"��ʮһ������ʧ��");

	for(i=0 ;i < 0x0026; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}
	for(  ;i < 0x0026 + 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"��ʮ��������ʧ��");
	}	
	for(  ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
		
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:����ż��ַ��дż�����ַ�ʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress�ֱ�ȡg_dwStartAddress��g_dwStartAddress + 0x0026
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength =  1024
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��dwStartAddress = g_dwStartAddress ʱ���ú���CSUDIEPRWrite��EEPROM��д��1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ú���CSUDIEPRRead��g_dwStartAddress��ַ���ȡ1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��������ȡ��������ڶ���д���������ͬ
//@EXECUTIONFLOW:5������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff
//@EXECUTIONFLOW:6��dwStartAddress = g_dwStartAddress + 0x0026ʱ���ú���CSUDIEPRWrite��EEPROM��д��1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7�����ú���CSUDIEPRRead ��g_dwStartAddress + 0x0026��ַ���ȡ1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8��������ȡ�������������д���������ͬ
//@EXECUTIONFLOW:9������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff
//@EXECUTIONFLOW:10��dwStartAddress = g_dwStartAddress + 0x0026ʱ���ú���CSUDIEPRWrite��EEPROM��д��1024������0x55��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11�����ú���CSUDIEPRRead��ȡEEPROM�е�����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12��������ȡ�����ݴ�0x0026��ʼ��1024���ֽ�Ϊ0x55�����������ȫΪ0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0005(void)
{
	int i;

	CSUDI_UINT8 pucData[1024];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");

	memset(pucData, 'a', sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, 1024),"�ڶ�������ʧ��");

	memset(pucData, '\0', sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, 1024),"����������ʧ��");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('a' == pucData[i],"���Ĳ�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"���岽����ʧ��");
	
	memset(pucData,'8',sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0026, pucData, 1024),"����������ʧ��");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress + 0x0026, pucData, 1024),"���߲�����ʧ��");

	for(i=0 ;i < 1023; i++)
	{
		CSTK_ASSERT_TRUE_FATAL('8' == pucData[i],"�ڰ˲�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"�ھŲ�����ʧ��");
	
	memset(pucData, 0x55, sizeof(pucData));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress + 0x0026, pucData, 1024),"��ʮ������ʧ��");

	memset(pucData,'\0',sizeof(pucData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"��ʮһ������ʧ��");

	for(i=0 ;i < 0x0026; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}
	for(  ;i < 0x0026 + 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"��ʮ��������ʧ��");
	}	
	for(  ;i < g_nEepromSize; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
		
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:����������ζ�дEEPROMʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength = 1024
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_RepeatTimeΪ�ظ�ִ�еĴ���
//@EXECUTIONFLOW:1������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����ú���CSUDIEPRWrite��EEPROM��д��1024��pucDataָ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ú���CSUDIEPRRead��ȡ1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��������ȡ��������ڶ���д���������ͬ
//@EXECUTIONFLOW:5���ظ�2��4��g_RepeatTime��
//@EXECUTIONFLOW:6�����ú���CSUDIEPRRead��ȡ����EEPROM�е�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7��������ȡ������ǰ1024����pucData��ָ��������ͬ�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0006(void)
{
	int i;
	int j;
	int nTestStartVal = 0x11;
	int nTestCount = 256 - 0x11;//CSUDI_UINT8����255

	CSUDI_UINT8 pucData[1024];
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");

	for(i=0; i < nTestCount; ++i)
	{
		memset(pucData, nTestStartVal+i, sizeof(pucData));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucData, 1024),"�ڶ�������ʧ��");

		memset(pucData, '\0', sizeof(pucData));
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucData, 1024),"����������ʧ��");

		for(j=0 ;j < 1024;  ++j)
		{
			CSTK_ASSERT_TRUE_FATAL(nTestStartVal+i == pucData[j],"���Ĳ�����ʧ��");
		}
	}
	
	memset(pucBuf, '\0', g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"����������ʧ��");

	for(j=0; j < 1024;  ++j)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[j] == pucData[j],"���߲�����ʧ��");
	}

	for(j=1024; j < g_nEepromSize;  ++j)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[j],"���߲�����ʧ��");
	}

 	CSTK_FATAL_POINT
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
	
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:�����������дһ���Զ�EEPROMʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength ֵ�Ϸ�
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����ú���CSUDIEPRWrite��EEPROM��dwStartAddress��ַ��д��256��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ú���CSUDIEPRWrite��EEPROM��dwStartAddress + 256��ַ��д��256��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4�����ú���CSUDIEPRWrite��EEPROM��dwStartAddress + 512��ַ��д��256��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5�����ú���CSUDIEPRWrite��EEPROM��dwStartAddress + 768��ַ��д��256��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6�����ú���CSUDIEPRRead��EEPROM��dwStartAddress��ַ�����1024��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7��������ȡ��������д���������ͬ
//@EXECUTIONFLOW:8�����ú���CSUDIEPRRead��EEPROM��dwStartAddress��ַ�����EEPROM��ȫ��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9��������ȡ�����ݿ�ʼ��1024����д���������ͬ�����������ȫΪ0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0007(void)
{
	int i;

	CSUDI_UINT32  dwWriteAddress;

	CSUDI_UINT8 pucDataW[256];

	CSUDI_UINT8 pucDataR[1024];

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");

	for(i = 0; i < 256; ++i)
	{
		pucDataW[i] = i ;
	}
	
	dwWriteAddress = g_dwStartAddress;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwWriteAddress, pucDataW, 256),"�ڶ�������ʧ��");

	dwWriteAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwWriteAddress, pucDataW, 256),"����������ʧ��");

	dwWriteAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwWriteAddress, pucDataW, 256),"���Ĳ�����ʧ��");

	dwWriteAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwWriteAddress, pucDataW, 256),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 1024),"����������ʧ��");

	for(i = 0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(( i%256)  == pucDataR[i],"���߲�����ʧ��");
	}

	memset(pucBuf, '\0', g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"�ڰ˲�����ʧ��");

	for(i = 0; i< 1024;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[i] == pucDataR[i],"�ھŲ�����ʧ��");
	}

	for(i = 1024; i < g_nEepromSize;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"�ھŲ�����ʧ��");
	}

 	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
		
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:����һ��д��ζ�EEPROMʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength ֵ�Ϸ�
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����ú���CSUDIEPRWrite��EEPROM��dwStartAddress��ַ��д��1024������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ú���CSUDIEPRReadÿ�δ�EEPROM��dwStartAddress��ַ���ȡ256��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������������������д�����������ͬ��
//@EXECUTIONFLOW:5�����ú���CSUDIEPRRead��EEPROM��dwStartAddress + 256��ַ���ȡ256��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������������������д�����������ͬ��
//@EXECUTIONFLOW:7�����ú���CSUDIEPRRead��EEPROM��dwStartAddress + 512��ַ���ȡ256��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������������������д�����������ͬ��
//@EXECUTIONFLOW:9�����ú���CSUDIEPRRead��EEPROM��dwStartAddress + 768��ַ���ȡ256��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������������������д�����������ͬ��
//@EXECUTIONFLOW:11�����ú���CSUDIEPRRead��EEPROM�ж�ȡȫ��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12����������������ǰ1024����д���������ͬ�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0008(void)
{
	int i;
	
	CSUDI_UINT32 dwReadAddress;

	CSUDI_UINT8 pucDataW[1024];

	CSUDI_UINT8 pucDataR[256];

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");

	for(i = 0; i < 1024; ++i)
	{
		pucDataW[i] = i%256;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 1024),"�ڶ�������ʧ��");

	dwReadAddress = g_dwStartAddress;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwReadAddress, pucDataR, 256),"����������ʧ��");

	for(i = 0; i < 256; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL( i == pucDataR[i],"���Ĳ�����ʧ��");
	}
	
	dwReadAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwReadAddress, pucDataR, 256),"���岽����ʧ��");

	for(i = 0; i < 256; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL( i == pucDataR[i],"����������ʧ��");
	}
	
	dwReadAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwReadAddress, pucDataR, 256),"���߲�����ʧ��");
	
	for(i = 0; i < 256; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL( i == pucDataR[i],"�ڰ˲�����ʧ��");
	}
	
	dwReadAddress +=256;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwReadAddress, pucDataR, 256),"�ھŲ�����ʧ��");

	for(i = 0; i < 256; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL( i == pucDataR[i],"��ʮ������ʧ��");
	}

	memset(pucBuf, '\0', g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"��ʮһ������ʧ��");

	for(i = 0; i< 1024;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[i] == pucDataW[i],"��ʮ��������ʧ��");
	}

	for(i = 1024; i < g_nEepromSize;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"��ʮ��������ʧ��");
	}

 	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
		
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:���Խ����дEEPROMʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength ֵ�Ϸ�
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIEPRWrite������EEPROM�ռ�ȫ��дΪ0xff����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�����ú���CSUDIEPRWrite��EEPROM��д��32��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3�����ú���CSUDIEPRRead��EEPROM�ж�ȡ32��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������������������д�����ͬ
//@EXECUTIONFLOW:5����д�ĵ�ַ��32���ظ�2��4�����ظ�4�Σ�����ÿ�ζ��ﵽԤ�ڽ��
//@EXECUTIONFLOW:6�����ú���CSUDIEPRRead��EEPROM�ж�ȡ����EEPROM��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����������������ǰ128���ֽ�Ϊ����д������ݣ������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0009(void)
{
	int i;

	int j = 0;
	
	CSUDI_UINT32 dwAddress;

	CSUDI_UINT8 pucData[32];

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");

	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");

	dwAddress = g_dwStartAddress;

	for(i = 0; i < 32; ++i)
	{
		pucData[i] = i ;
	}

	while(j < 4)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(dwAddress, pucData, 32),"�ڶ�������ʧ��");

		memset(pucData, '\0', sizeof(pucData));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(dwAddress, pucData, 32),"����������ʧ��");

		for(i = 0; i < 32; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL( i == pucData[i],"���Ĳ�����ʧ��");
		}

		dwAddress += 32;
		
		++j;
	}
	
	memset(pucBuf, '\0', g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"����������ʧ��");

	for(i = 0; i< 128;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i%32 == pucBuf[i],"���߲�����ʧ��");
	}

	for(i = 128; i < g_nEepromSize;  ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"���߲�����ʧ��");
	}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
		
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:��дg_nEepromSize��8k��4k��2k��32byteʱ���ٶȲ���
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_dwStartAddress
//@INPUT:pucData �ǿ� 
//@INPUT:uDataLength ֵ�Ϸ�
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:g_dwStartAddressΪEEPROM����ʼ��ַ��g_nEepromSizeΪEEPROM�Ĵ�С
//@REMARK:������Щ������ʱ���׼����Ҫ��һ��ȷ��
//@EXECUTIONFLOW:1�����ú���CSUDIEPRWrite��EEPROM��д��g_nEepromSize���ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:2���õ����д��8k�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:3�����ú���CSUDIEPRRead��EEPROM�ж�ȡg_nEepromSize���ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���õ���ɶ�ȡ8k�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:5������������������д���������ͬ
//@EXECUTIONFLOW:6�����ú���CSUDIEPRWrite��EEPROM��д��8k�ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���õ����д��8k�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:8�����ú���CSUDIEPRRead��EEPROM�ж�ȡ8k�ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���õ���ɶ�ȡ8k�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:10������������������д���������ͬ
//@EXECUTIONFLOW:11�����ú���CSUDIEPRWrite��EEPROM��д��4k�ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12���õ����д��4k�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:13�����ú���CSUDIEPRRead��EEPROM�ж�ȡ4k�ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14���õ���ɶ�ȡ4k�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:15������������������д���������ͬ
//@EXECUTIONFLOW:16�����ú���CSUDIEPRWrite��EEPROM��д��2k�ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17���õ����д��2k�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:18�����ú���CSUDIEPRRead��EEPROM�ж�ȡ2k�ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19���õ���ɶ�ȡ2k�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:20������������������д���������ͬ
//@EXECUTIONFLOW:21�����ú���CSUDIEPRWrite��EEPROM��д��32byte�ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:22���õ����д��32byte�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:23�����ú���CSUDIEPRRead��EEPROM�ж�ȡ32byte�ֽڵ�������������CSUDI_SUCCESS
//@EXECUTIONFLOW:24���õ���ɶ�ȡ32byte�������õ�ʱ�䣬�������õ�ʱ������Ҫ��
//@EXECUTIONFLOW:25������������������д���������ͬ
CSUDI_BOOL  CSTC_EEPROM_IT_EPRWriteRead_0010(void)
{
	int i;
	
	int nWriteTime;
	
	int nReadTime;
	
	CSUDIOSTimeVal_S sTime;

	CSUDI_UINT8 pucDataW[8*1024];

	CSUDI_UINT8 pucDataR[8*1024];

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf, "������Դʧ��");
	
	memset(pucBuf, 0xff, g_nEepromSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, g_nEepromSize),"��һ������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 16000,"�ڶ�������ʧ��");

	memset(pucBuf, 0x00, g_nEepromSize);	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, g_nEepromSize),"����������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 16000,"���Ĳ�����ʧ��");

	for(i=0; i < g_nEepromSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"���岽����ʧ��");
	}
		
	memset(pucDataW, 'X' ,sizeof(pucDataW));
	
	memset(pucDataR, '\0' ,sizeof(pucDataR));
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 8*1024),"����������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 6000,"���߲�����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 8*1024),"�ڰ˲�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 6000,"�ھŲ�����ʧ��");

	for(i=0; i < 8*1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucDataR[i] == pucDataW[i],"��ʮ������ʧ��");
	}

	memset(pucDataW, '8' ,sizeof(pucDataW));
	
	memset(pucDataR, '\0' ,sizeof(pucDataR));
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 4*1024),"��ʮһ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 3000,"��ʮ��������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 4*1024),"��ʮ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 3000,"��ʮ�Ĳ�����ʧ��");

	for(i=0; i < 4*1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucDataR[i] == pucDataW[i],"��ʮ�岽����ʧ��");
	}
	
	memset(pucDataW, 'M' ,sizeof(pucDataW));
	
	memset(pucDataR, '\0' ,sizeof(pucDataR));
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 2*1024),"��ʮ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 1500,"��ʮ�߲�����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 2*1024),"��ʮ�˲�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 1500,"��ʮ�Ų�����ʧ��");

	for(i=0; i < 2*1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucDataR[i] == pucDataW[i],"�ڶ�ʮ������ʧ��");
	}
	
	memset(pucDataW, 'q' ,sizeof(pucDataW));
	
	memset(pucDataR, '\0' ,sizeof(pucDataR));
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucDataW, 32),"�ڶ�ʮһ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nWriteTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nWriteTime < 30,"�ڶ�ʮ��������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime = sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucDataR, 32),"�ڶ�ʮ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sTime),"�õ�ϵͳʱ��ʧ��");

	nReadTime -=sTime.m_nMiliSecond;

	CSTK_ASSERT_TRUE_FATAL(nReadTime < 30,"�ڶ�ʮ�Ĳ�����ʧ��");

	for(i=0; i < 32; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucDataR[i] == pucDataW[i],"�ڶ�ʮ�岽����ʧ��");
	}
	
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"�ͷ��ڴ�ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:���Զ������ͬʱдEEPROMʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:
//@EXECUTIONFLOW:1����������1��EEPROM��д������0x55
//@EXECUTIONFLOW:2����������2��EEPROM��д������0x88
//@EXECUTIONFLOW:3����������3��EEPROM��д������0xee
//@EXECUTIONFLOW:4������CSUDIEPRRead��ȡEEPROM�е����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5�������õ���������ȫ0x55��0x88��0xee
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
			"��������1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread2", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread2Entry, &nParam, &hThreadhandle2),
			"��������2ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread3", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread3Entry, &nParam, &hThreadhandle3),
			"��������3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"�����ɹ����������������������Ϊ��");

	while((!g_bThreadEnd1)  || (!g_bThreadEnd2) ||(!g_bThreadEnd3))
	{
		CSUDIOSThreadSleep(10);
	}
	
	memset(pucBuf, '\0', 1024);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRRead(g_dwStartAddress, pucBuf, 1024),"����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(((pucBuf[0] == 0x55) ||(pucBuf[0] == 0x88) ||(pucBuf[0] == 0xee )),"����ʧ��");

	for(i=1; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[i-1] == pucBuf[0],"����ʧ��");
	}

	CSTK_FATAL_POINT
	{
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

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:���Զ������ͬʱ��EEPROMʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:
//@EXECUTIONFLOW:1�����ú���CSUDIEPRWrite��EEPROM����ʼ��ַ��д��2k��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:2����������1��EEPROM����ʼ��ַ��ʼ��ȡ1k�����ݣ�����������������д�����ͬ
//@EXECUTIONFLOW:3����������2��EEPROM����ʼ��ַ+512���ȡ1k�����ݣ�����������������д�����ͬ
//@EXECUTIONFLOW:4����������3��EEPROM����ʼ��ַ+1024���ȡ1k�����ݣ�����������������д�����ͬ
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEPRWrite(g_dwStartAddress, pucBuf, 2048),"����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread1", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread4Entry, &nParam, &hThreadhandle1),
			"��������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread2", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread5Entry, &nParam, &hThreadhandle2),
			"��������ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread3", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread6Entry, &nParam, &hThreadhandle3),
			"��������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"�����ɹ����������������������Ϊ��");
	
	while((!g_bThreadEnd4) || (!g_bThreadEnd5) || (!g_bThreadEnd6))
	{
		CSUDIOSThreadSleep(10);
	}
		
	CSTK_ASSERT_TRUE_FATAL(((g_bFlag1 == CSUDI_TRUE)&&(g_bFlag2 == CSUDI_TRUE)&&(g_bFlag3 == CSUDI_TRUE)),"��ȡ������д�����ݲ�һ��");
		
	CSTK_FATAL_POINT
	{
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

//@CASEGROUP:CSUDIEPRWrite  
//@CASEGROUP:CSUDIEPRRead 
//@DESCRIPTION:���Զ������ͬʱ��дEEPROMʱ�����
//@PRECONDITION:EEPROMģ���ѳɹ���ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:ÿһ���������Ԥ�ڽ��
//@REMARK:
//@EXECUTIONFLOW:1����������1��EEPROM�д���ʼ��ַ��д��1024���ֽڵ�����0x55
//@EXECUTIONFLOW:2����������2��EEPROM�д���ʼ��ַ+ 1024��д��1024���ֽڵ�����0xee
//@EXECUTIONFLOW:3����������3��EEPROM����ʼ��ַ���ȡ1024���ֽڵ����ݣ���������������ȫ��Ϊ0x55
//@EXECUTIONFLOW:4����������4��EEPROM����ʼ��ַ+1024���ȡ1024���ֽڵ����ݣ���������������ȫ��Ϊ0xee
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

	//���߳�д����
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread1", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread1Entry, &nParam, &hThreadhandle1),
			"��������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread7", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread7Entry, &nParam, &hThreadhandle2),
			"��������ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��");
	
	while((!g_bThreadEnd1) ||(!g_bThreadEnd7))
	{
		CSUDIOSThreadSleep(10);
	}

	//���̶߳�����
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread4", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread4Entry, &nParam, &hThreadhandle3),
			"��������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("EEPROM_TestThread6", nPriority , PANEL_TEST_STACK_SIZE, 
							 EEPROM_TestThread6Entry, &nParam, &hThreadhandle4),
			"��������ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle4,"�����ɹ����������������������Ϊ��");
	
	while((!g_bThreadEnd4) ||(!g_bThreadEnd6))
	{
		CSUDIOSThreadSleep(10);
	}
		
	CSTK_ASSERT_TRUE_FATAL(((g_bFlag1 == CSUDI_TRUE)&&(g_bFlag3 == CSUDI_TRUE)),"��ȡ������д�����ݲ�һ��");
		
	CSTK_FATAL_POINT
	{
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
		if (hThreadhandle4 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle4), "ɾ������ʧ��");
			   hThreadhandle4 = CSUDI_NULL;
		}
	}
	
	return CSUDI_TRUE;
}

