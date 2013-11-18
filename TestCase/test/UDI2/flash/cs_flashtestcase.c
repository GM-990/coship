/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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

//flash�ռ����ʼ��ַ
extern CSUDI_UINT32 g_uStartAddress;

//�û����ÿռ����ʼ��ַ
extern CSUDI_UINT32 g_uTestStartAddress;

//�û����ÿռ���ܴ�С
extern unsigned int g_uTestFlashSize;

//�û����õ�����Block��С
extern unsigned int g_uTestBlockSize;

//flash����������Ϣ
extern unsigned int  g_uALLRegionCount;
extern unsigned int  g_uALLRegionSize;
extern unsigned int  g_uALLBlockCount;
extern unsigned int  g_uALLMaxBlockSize;


//���������ʱ�õ���ȫ�ֱ���
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

//�����дflash����
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
	//�ڱ�����������ִ��ǰ����
	 CSTC_FLASH_InitCfg();

	//���flashϵͳ������flash��Ϊһ��block�������ò���������block��СΪ128K
	//�������ò���������block��СΪflash����block��С
	if(g_uALLBlockCount > 1)
	{
		g_uTestBlockSize = g_uALLMaxBlockSize;
	}
	
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_FLASH_UnInit(void)
{
	//�ڱ�����������ִ�к����
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
		CSTCPrint("����1дflashʧ��");
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
		CSTCPrint("����1��flashʧ��");
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
		CSTCPrint("����1дflashʧ��");
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
		CSTCPrint("����1��flashʧ��");
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
		CSTCPrint("����3дflashʧ��");
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
		CSTCPrint("����4дflashʧ��");
		g_bFlagWrite4 = CSUDI_TRUE;
	}
		
	g_bFlashWrite4 = CSUDI_TRUE;
}

//	CSUDIFlashEraseSpace(0x6800000,0x7760000);
//@CASEGROUP:CSUDIFlashGetInfo 
//@DESCRIPTION:���Բ���eSuitType���Ϸ�ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:eSuitType = EM_UDI_FLASH_MAX
//@INPUT:psFlashSuit�ǿ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIFlashGetInfo(EM_UDI_FLASH_MAX, psFlashSuit)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0001(void)
{
	CSUDIFLASHRegionSuit_S sFlashRegionSuit;
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashGetInfo(EM_UDI_FLASH_MAX,&sFlashRegionSuit),"eSuitType = EM_UDI_FLASH_MAXʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashGetInfo 
//@DESCRIPTION:���Բ���psFlashSuit = CSUDI_NULLʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:eSuitTypeֵ�Ϸ�
//@INPUT:psFlashSuit = CSUDI_NULL
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIFlashGetInfo(EM_UDI_FLASH_ALL,CSUDI_NULL)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0002(void)
{
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashGetInfo(EM_UDI_FLASH_ALL,CSUDI_NULL),"psFlashSuit = CSUDI_NULLʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashGetInfo 
//@DESCRIPTION:�����ܷ���ȷ�õ�����������Ϣʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:eSuitTypeֵ�Ϸ�
//@INPUT:psFlashSuitֵ�Ϸ�
//@EXPECTATION:����CSUDI_SUCCESS���õ��ķ���������Ϣ�������ļ��е����
//@REMARK:
//@EXECUTIONFLOW:1��eSuitType=EM_UDI_FLASH_ALLʱ����CSUDIFlashGetInfo��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�������õ��ķ���������Ϣ�������ļ��е���ͬ
CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0003(void)
{
	CSUDIFLASHRegionSuit_S sFlashRegionSuit;

	int i;

	unsigned int uSize = 0;

	unsigned int uBlockCount = 0;
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashGetInfo(EM_UDI_FLASH_ALL,&sFlashRegionSuit),"��һ������ʧ��");

	CSTCPrint("\n********m_uRegintCount:%u,\n\tm_uSize:%0x,\n\tm_uMaxBlockSize:%0x,\n\tm_uBlockCount:%u\n",
	sFlashRegionSuit.m_uRegionCount,sFlashRegionSuit.m_uSize,sFlashRegionSuit.m_uMaxBlockSize,sFlashRegionSuit.m_uBlockCount);
	CSTCPrint("g_uALLRegionSize=0x%x,sFlashRegionSuit.m_uSize=0x%x, g_uALLBlockCount=%d\r\n ", g_uALLRegionSize, sFlashRegionSuit.m_uSize, g_uALLBlockCount);
	CSTCPrint("g_uStartAddress=0x%x,  sFlashRegionSuit.m_asBlockRegion[0].m_dwStartAddress=0x%x\r\n", g_uStartAddress, sFlashRegionSuit.m_asBlockRegion[0].m_dwStartAddress);
	
	CSTK_ASSERT_TRUE_FATAL(g_uALLRegionCount == sFlashRegionSuit.m_uRegionCount, "�ڶ����õ�flash count�������ļ�����ͬ");

	CSTK_ASSERT_TRUE_FATAL(g_uALLRegionSize == sFlashRegionSuit.m_uSize, "�ڶ����õ�flash��С�������ļ�����ͬ");

	CSTK_ASSERT_TRUE_FATAL(g_uALLBlockCount== sFlashRegionSuit.m_uBlockCount, "�ڶ����õ�flash block count�������ļ�����ͬ");
	
	CSTK_ASSERT_TRUE_FATAL(g_uALLMaxBlockSize == sFlashRegionSuit.m_uMaxBlockSize, "�ڶ����õ�flash max block size�������ļ�����ͬ");
	
	CSTK_ASSERT_TRUE_FATAL(g_uStartAddress == sFlashRegionSuit.m_asBlockRegion[0].m_dwStartAddress,"�ڶ����õ�flash����ʼ��ַ�������ļ�����ͬ");
	CSTK_ASSERT_TRUE_FATAL(0 == (g_uTestBlockSize%sFlashRegionSuit.m_uMaxBlockSize),"�ڶ���TestBlockSize ����BlockSize��������");

	for(i = 0; i < g_uALLRegionCount; ++i)
	{
		uBlockCount += sFlashRegionSuit.m_asBlockRegion[i].m_uBlockCount;
	
		uSize += (sFlashRegionSuit.m_asBlockRegion[i].m_uBlockCount)*(sFlashRegionSuit.m_asBlockRegion[i].m_uBlockSize); 
	}

	CSTK_ASSERT_TRUE_FATAL(sFlashRegionSuit.m_uSize == uSize ,"�ڶ����õ����������ܴ�С�����з���Block�ܴ�С����ͬ");
	
	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:���Բ���dwStartAddress���Ϸ����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uStartAddress - 0x1000
//@INPUT:pucBufֵ�Ϸ�
//@INPUT:uDataLengthֵ�Ϸ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uStartAddressΪflash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashRead(g_uStartAddress - 0x1000,pucBuf,16)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0001(void)
{
	CSUDI_UINT8 pucBuf[16];
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashRead(g_uStartAddress - 0x1000, pucBuf, 16),"dwStartAddress���Ϸ�ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:���Բ���pucBuf = CSUDI_NULL�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf = CSUDI_NULL
//@INPUT:uDataLength = 1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashRead(g_uTestStartAddress, CSUDI_NULL, 1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashRead(g_uTestStartAddress, CSUDI_NULL, 1024),"pucBuf = CSUDI_NULLʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:���Բ���uDataLength = 0ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBufֵ�Ϸ�
//@INPUT:uDataLength = 0
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashRead(g_uTestStartAddress, pucBuf, 0)��������CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0003(void)
{
	CSUDI_UINT8 pucBuf[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 0),"uDataLength = 0ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:���Բ���uDataLength���Ϸ������
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBufֵ�Ϸ�
//@INPUT:uDataLength = 1024*1024*1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashRead(g_uTestStartAddress, pucBuf, 1024*1024*1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashRead_0004(void)
{
	CSUDI_UINT8 pucBuf[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashRead(g_uTestStartAddress, pucBuf, 1024*1024*1024),"uDataLength���Ϸ�ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead
//@DESCRIPTION:����dwStartAddress + uDataLength���ڿɶ�д��flash�ռ�ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uStartAddress+1024
//@INPUT:pucBufֵ�Ϸ�
//@INPUT:uDataLength = g_uALLRegionSize
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uStartAddressΪflash�ռ���ʼ��ַ��g_uALLRegionSizeΪflash�������ϵĴ�С
//@EXECUTIONFLOW:1������CSUDIFlashRead(g_uStartAddress+1024, pucBuf, g_uALLRegionSize)�������ط�CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashRead(g_uStartAddress+1024, pucBuf, g_uALLRegionSize),"����ʧ��");

	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");	
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite 
//@DESCRIPTION:���Բ���dwStartAddress���Ϸ����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uStartAddress - 0x1000
//@INPUT:pucBufֵ�ǿ�
//@INPUT:uDataLength = 1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uStartAddressΪflash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashWrite(g_uStartAddress - 0x1000, pucBuf, 1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0001(void)
{
	CSUDI_UINT8 pucBuf[1024];
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWrite(g_uStartAddress - 0x1000, pucBuf, 1024),"dwStartAddress���Ϸ�ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite 
//@DESCRIPTION:���Բ���pucBuf = CSUDI_NULL�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf = CSUDI_NULL
//@INPUT:uDataLength = 1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash��ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashWrite(g_uTestStartAddress, CSUDI_NULL, 16)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWrite(g_uTestStartAddress, CSUDI_NULL, 16),"pucBuf = CSUDI_NULLʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite 
//@DESCRIPTION:���Բ���uDataLength = 0ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�ǿ�
//@INPUT:uDataLength = 0
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashWrite(g_uTestStartAddress, pucBuf, 0)��������CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0003(void)
{
	CSUDI_UINT8 pucBuf[1024];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, 0),"uDataLength = 0ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite 
//@DESCRIPTION:���Բ���uDataLength���Ϸ������
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�ǿ�
//@INPUT:uDataLength = 1024*1024*1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashWrite(g_uTestStartAddress, pucBuf, 1024*1024*1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWrite_0004(void)
{
	CSUDI_UINT8 pucBuf[1024];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWrite(g_uTestStartAddress, pucBuf, 1024*1024*1024),"uDataLength���Ϸ�ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:����dwStartAddress + uDataLength���ڿɶ�д��flash�ռ�ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uStartAddress + 1024
//@INPUT:pucBufֵ�Ϸ�
//@INPUT:uDataLength = g_uALLRegionSize
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uStartAddressΪflash�ռ���ʼ��ַ��g_uALLRegionSizeΪflash�������ϵĴ�С
//@EXECUTIONFLOW:1������CSUDIFlashWrite(g_uStartAddress+1024, pucBuf, g_uALLRegionSize)�������ط�CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWrite(g_uStartAddress+1024, pucBuf, g_uALLRegionSize),"����ʧ��");

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:���Բ���dwStartAddress���Ϸ����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uStartAddress-0x1000
//@INPUT:uSpaceSize = 1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uStartAddressΪflash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace(g_uStartAddress - 0x1000, 1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0001(void)
{
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashEraseSpace(g_uStartAddress - 0x1000, 1024),"dwStartAddress���Ϸ�ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:���Բ���uSpaceSize = 0ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = 0 
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace(g_uTestStartAddress, 0)��������CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 0),"uSpaceSize = 0ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:���Բ���uSpaceSize���Ϸ������
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = 1024*1024*1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace(g_uTestStartAddress, 1024*1024*1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashEraseSpace(g_uTestStartAddress, 1024*1024*1024),"uSpaceSize���Ϸ�ʱ����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:����dwStartAddress + uSpaceSize���ڿɶ�д��flash�ռ�ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uStartAddress + 1024
//@INPUT:uSpaceSize = g_uALLRegionSize
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uStartAddressΪflash�ռ���ʼ��ַ��g_uALLRegionSizeΪflash�������ϵĴ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace(g_uStartAddress + 1024, g_uALLRegionSize)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0004(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashEraseSpace(g_uStartAddress + 1024, g_uALLRegionSize),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:���Բ����������ڲ��Ե�flash�ռ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = g_uTestFlashSize
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestFlashSizeΪ�����ڲ��Ե�flash�ռ��С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashRead��ȡflash�е����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:3����������������ȫΪ0xff
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTCPrint("���������У���ȴ�!!\n");

	CSTCPrint("���ڲ���Flash����ȴ�!!\n");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize),"��һ�������������Ե�FLASH�ռ�ʧ��");

	CSTCPrint("���ڶ�ȡ���ݣ���ȴ�!!\n");
	while (unLeftSzie > 0)
	{
		unReadSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr, pucBuf, unReadSize),"�ڶ�����ȡ����ʧ��");

		for(i=0; i < unReadSize; ++i)
		{
			if (pucBuf[i] != 0xff)
			{
				CSTCPrint("pucBuf[%d]=0x%x, unReadSize=0x%x\r\n", i, pucBuf[i], unReadSize);
			}
			CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"����������У��ʧ��");
		}

		unTestStartAddr += unReadSize;
		unLeftSzie -= unReadSize;
	}

	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:���Բ���һ��Blockʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = g_uTestBlockSize
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block��С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashRead��ȡ������flash�и�block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:3����������������ȫΪ0xff
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0006(void)
{
	int i;

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������һ��Blockʱʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "�ڶ�����ȡ����ʧ��");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"����������У��ʧ��");
	}
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:���Բ������Blockʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = 3*g_uTestBlockSize
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block��С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace(g_uTestStartAddress, 3*g_uTestBlockSize)��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashRead��ȡ������flash�е����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:3����������������ȫΪ0xff
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0007(void)
{
	int i;

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(3*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 3*g_uTestBlockSize),"��һ���������Blockʱʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 3*g_uTestBlockSize), "�ڶ�����ȡ����ʧ��");

	for(i=0; i < 3*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"����������У��ʧ��");
	}
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:����Ҫ��������ʼ��ַ������ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress + 0x0003
//@INPUT:uSpaceSize = g_uTestBlockSize -1024
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block��С
//@EXECUTIONFLOW:1������CSUDIFlashWrite�ѵ�һ��block����ȫ��дΪ0x55��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��ErrCode = CSUDIFlashEraseSpace(g_uTestStartAddress + 0x0003, g_uTestBlockSize -1024)
//@EXECUTIONFLOW:3�����ErrCode == CSUDI_SUCCESS��ִ��4����5��
//@EXECUTIONFLOW:4������CSUDIFlashRead��ȡ��һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������������block�����ݴӵ��������g_uTestBlockSize -1024������Ϊ0xff�����������Ϊ0x55
//@EXECUTIONFLOW:6�����ErrCode == CSUDIFLASH_ERROR_BAD_PARAMETER��ִ�в�7����8��
//@EXECUTIONFLOW:7������CSUDIFlashRead��ȡ��һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8����������������ȫΪ0x55
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0008(void)
{
	int i;

	CSUDI_BOOL ErrCode;

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	memset(pucBuf, 0x55, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "��һ����������ʧ��");

	ErrCode = CSUDIFlashEraseSpace(g_uTestStartAddress + 0x0003, g_uTestBlockSize -1024);

	if(ErrCode == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "���Ĳ���ȡ����ʧ��");

		for(i=0; i < 0x0003; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"���岽����У��ʧ��");
		}
		for( ; i < g_uTestBlockSize -1024 + 3; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"���岽����У��ʧ��");
		}
		for( ; i < g_uTestBlockSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"���岽����У��ʧ��");
		}
	}
	else
		if(ErrCode == CSUDIFLASH_ERROR_BAD_PARAMETER)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "���߲���ȡ����ʧ��");
	
			for(i = 0 ; i < g_uTestBlockSize; ++i)
			{
				CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"�ڰ˲�����У��ʧ��");
			}
		}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashEraseSpace 
//@DESCRIPTION:����Ҫ�����Ĵ�С������ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:uSpaceSize = 1023
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block��С
//@EXECUTIONFLOW:1������CSUDIFlashWrite�ѵ�һ��block����ȫ��дΪ0x55��������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��ErrCode = CSUDIFlashEraseSpace(g_uTestStartAddress, 1023)
//@EXECUTIONFLOW:3�����ErrCode == CSUDI_SUCCESS��ִ��4����5��
//@EXECUTIONFLOW:4������CSUDIFlashRead��ȡ��һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������������block������ǰ1023������Ϊ0xff�����������Ϊ0x55
//@EXECUTIONFLOW:6�����ErrCode == CSUDIFLASH_ERROR_BAD_PARAMETER��ִ�в�7����8��
//@EXECUTIONFLOW:7������CSUDIFlashRead��ȡ��һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8����������������ȫΪ0x55
CSUDI_BOOL  CSTC_FLASH_IT_EraseSpace_0009(void)
{
	int i;

	CSUDI_BOOL ErrCode;

	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	memset(pucBuf, 0x55, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "��һ������ʧ��");

	ErrCode = CSUDIFlashEraseSpace(g_uTestStartAddress, 1023);

	if(ErrCode == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "���Ĳ�����ʧ��");

		for(i=0; i < 1023 ; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i],"���岽����ʧ��");
		}
		
		for( ; i < g_uTestBlockSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"���岽����ʧ��");
		}
	}
	else
		if(ErrCode == CSUDIFLASH_ERROR_BAD_PARAMETER)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize), "���߲�����ʧ��");
	
			for(i = 0 ; i < g_uTestBlockSize; ++i)
			{
				CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i],"�ڰ˲�����ʧ��");
			}
		}
		
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:����dwStartAddress���Ϸ����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uStartAddress-0x1000
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uStartAddressΪflash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashWriteWithoutErase(g_uStartAddress - 0x1000, pucBuf, 1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0001(void)
{
	CSUDI_UINT8 pucBuf[1024];
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWriteWithoutErase(g_uStartAddress - 0x1000, pucBuf, 1024),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:����pucBuf = CSUDI_NULL�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf = CSUDI_NULL
//@INPUT:uDataLength = 1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashWriteWithoutErase(g_uTestStartAddress, CSUDI_NULL, 1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWriteWithoutErase(g_uTestStartAddress, CSUDI_NULL, 1024),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:����uDataLength = 0ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 0
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 0)��������CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0003(void)
{
	CSUDI_UINT8 pucBuf[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 0),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:����uDataLength���Ϸ������
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1024*1024*1024
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 1024*1024*1024)�������ط�CSUDI_SUCCESS
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutErase_0004(void)
{
	CSUDI_UINT8 pucBuf[16];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 1024*1024*1024),"����ʧ��");

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashWriteWithoutErase  
//@DESCRIPTION:����dwStartAddress + uDataLength���ڿɶ�д��flash�ռ�ʱ�����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uStartAddress + 1024
//@INPUT:pucBufֵ�Ϸ�
//@INPUT:uDataLength = g_uALLRegionSize
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:g_uStartAddressΪflash�ռ���ʼ��ַ��g_uALLRegionSizeΪflash�������ϵĴ�С
//@EXECUTIONFLOW:1������CSUDIFlashWriteWithoutErase�������ط�CSUDI_SUCCESS
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIFlashWriteWithoutErase(g_uStartAddress + 1024, pucBuf, g_uALLRegionSize),"����ʧ��");

	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");	
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:���������Ե�FLASH�ռ���ж�дʱ�Ķ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestFlashSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestFlashSizeΪ�����ڲ��Ե�flash�ռ��С
//@EXECUTIONFLOW:1������CSUDIFlashWrite������g_uTestStartAddress��ַд��g_uTestFlashSize���ֽ�0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestFlashSize���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:3���������������ݶ���0x55
//@EXECUTIONFLOW:4������CSUDIFlashWrite������g_uTestStartAddress��ַд��g_uTestFlashSize���ֽ�0xbb����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestFlashSize���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���������������ݶ���0xbb
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");

	

	CSTCPrint("���������У���ȴ�!!\n");

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	while (unLeftSzie > 0)
	{
		memset(pucBuf, 0x55, unMallocSzie);
	 	unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
	 	//CSTCPrint("CSUDIFlashWrite unTestStartAddr=0x%x,pucBuf=0x%x,unOperSize=0x%x, g_uALLMaxBlockSize=0x%xr\n", unTestStartAddr,pucBuf,unOperSize, g_uALLMaxBlockSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(unTestStartAddr,pucBuf,unOperSize),"��һ������ʧ��");
		
		memset(pucBuf, 0x00, unMallocSzie);
		//CSTCPrint("CSUDIFlashRead unTestStartAddr=0x%x,pucBuf=0x%x,unOperSize=0x%xr\n", unTestStartAddr,pucBuf,unOperSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"�ڶ�������ʧ��");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "����������У�����ʧ��");
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(unTestStartAddr,pucBuf,unOperSize),"���Ĳ�����ʧ��");
		memset(pucBuf, 0x00, unMallocSzie);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"���岽����ʧ��");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "����������У�����ʧ��");
		}

		unTestStartAddr += unOperSize;
		unLeftSzie -= unOperSize;
	}

 	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:���ڶ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 16
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ,g_uTestBlockSizeΪ���ڲ��Ե�block��С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite������g_uTestStartAddress��ַд��16���ֽ�pubBufָ������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����16���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݺ�д���������ͬ
//@EXECUTIONFLOW:5������CSUDIFlashRead������g_uTestStartAddress��ַ������һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6����������������ǰ16���ֽ�������pubBufָ���������ͬ�������ȫΪ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0002(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[16];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");

	for(i=0; i < 16; ++i)
	{
		pucBuf[i] = i;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf,16),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,16),"����������ʧ��");

	for(i=0; i < 16; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucData, '\0', g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucData,g_uTestBlockSize),"���岽����ʧ��");

	for(i=0; i < 16; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i == pucData[i], "����������У�����ʧ��");
	}
	
	for(; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "����������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:�����д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace��������ǰ����block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite�������һ��block��д��g_uTestBlockSize���ֽڵ�����0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead�����ӵ�һ��block�ڶ���g_uTestBlockSize���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashWrite�������һ��block��д��g_uTestBlockSize���ֽڵ�0x00����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashRead�����ӵ�һ��block�ڶ���g_uTestBlockSize���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������������ݶ���0x00
//@EXECUTIONFLOW:8������CSUDIFlashRead������g_uTestStartAddress��ַ����ǰ����block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���������������ݵ�һ��block�ڵ�����Ϊ0x00���ڶ���block�ڵ�����Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0003(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"��һ������ʧ��");
	 
	memset(pucBuf, 0x55, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0x00, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x00 == pucBuf[i], "���߲�����У�����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, 2*g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x00== pucData[i], "�ھŲ�����У�����ʧ��");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"������Դʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:����д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddressֵ�Ϸ�
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize + 512
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace��������ǰ����block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite������g_uTestStartAddress��ַд��g_uTestBlockSize + 512��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize + 512�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashWrite������g_uTestStartAddress��ַд��g_uTestBlockSize + 512��0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize + 512�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������������ݶ���0xaa
//@EXECUTIONFLOW:8������CSUDIFlashRead������g_uTestStartAddress��ַ����ǰ����block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����������������ǰg_uTestBlockSize + 512��0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0004(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize + 512);

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, g_uTestBlockSize + 512);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, g_uTestBlockSize + 512),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize + 512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, g_uTestBlockSize + 512);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf,g_uTestBlockSize + 512),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize + 512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "���߲�����У�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, 2*g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "�ھŲ�����У�����ʧ��");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:�Ҳ�block���룬���block������ʱ�Ķ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress + 512
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize - 512
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite������g_uTestStartAddress + 512��ַд��g_uTestBlockSize - 512���ֽ�����0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 512��ַ����g_uTestBlockSize - 512���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashWrite������g_uTestStartAddress + 512��ַд��g_uTestBlockSize - 512���ֽ�����0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashRead������g_uTestStartAddress + 512��ַ����g_uTestBlockSize - 512���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������������ݶ���0xaa
//@EXECUTIONFLOW:8������CSUDIFlashRead������g_uTestStartAddress��ַ������һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����������������ǰ512���ֽ���0xff�������ȫ��Ϊ0xaa
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0005(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 512, pucBuf, g_uTestBlockSize -512),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 512, pucBuf, g_uTestBlockSize -512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 512,pucBuf,g_uTestBlockSize - 512),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 512,pucBuf,g_uTestBlockSize - 512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "���߲�����У�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=0; i < 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "�ھŲ�����У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "�ھŲ�����У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:���block���룬�Ҳ�block������ʱ�Ķ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize - 512
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite������g_uTestStartAddress��ַд��g_uTestBlockSize - 512���ֽڵ�0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize - 512���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashWrite������g_uTestStartAddress��ַд��g_uTestBlockSize - 512���ֽڵ�0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize - 512���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������������ݶ���0xaa
//@EXECUTIONFLOW:8������CSUDIFlashRead������g_uTestStartAddress��ַ������һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����������������ǰg_uTestBlockSize - 512���ֽ���0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0006(void)
{
	int i;
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucData, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucData, g_uTestBlockSize - 512),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize -512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucData[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucData, 0xaa, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucData,g_uTestBlockSize - 512),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucData,g_uTestBlockSize - 512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "���߲�����У�����ʧ��");
	}
	
	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "�ھŲ�����У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:��ʼ��ַΪ���ַ����д����Ϊ����ʱ�Ķ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress�ֱ�ȡg_uTestStartAddress + 0x0001
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1023
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite������g_uTestStartAddress + 0x0001��ַд��1023��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1023�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashWrite������g_uTestStartAddress + 0x0001��ַд��1023��0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1023�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������������ݶ���0xaa
//@EXECUTIONFLOW:8������CSUDIFlashRead������ȡ��һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���������������ݴӵڶ�����ʼ��1023������Ϊ0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0007(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1023];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, 1023);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0001, pucBuf, 1023),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001, pucBuf, 1023),"����������ʧ��");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, 1023);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0001,pucBuf,1023),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001,pucBuf,1023),"����������ʧ��");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "���߲�����У�����ʧ��");
	}

	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=1; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "�ھŲ�����У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:��ʼ��ַΪ���ַ����д����Ϊż��ʱ�Ķ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress�ֱ�ȡg_uTestStartAddress + 0x0001
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1024
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite������g_uTestStartAddress + 0x0001��ַд��1024���ֽ�����0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1024���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashWrite������g_uTestStartAddress + 0x0001��ַд��1024���ֽ�����0x88����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1024���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������������ݶ���0x88
//@EXECUTIONFLOW:8������CSUDIFlashRead������ȡ��һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���������������ݴӵڶ�����ʼ��1024������Ϊ0x88�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0008(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1024];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, 1024);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0001, pucBuf, 1024),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001, pucBuf, 1024),"����������ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0x88, 1024);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0001,pucBuf,1024),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001,pucBuf,1024),"����������ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x88 == pucBuf[i], "���߲�����У�����ʧ��");
	}
	
	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=1; i < 1025; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x88 == pucData[i], "�ھŲ�����У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:��ʼ��ַΪż��ַ����д����Ϊ����ʱ�Ķ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddressȡg_uTestStartAddress + 0x0006
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1023
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite������g_uTestStartAddress + 0x0006��ַд��1023��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 0x0006��ַ����1023�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashWrite������g_uTestStartAddress + 0x0006��ַд��1023��0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashRead������g_uTestStartAddress + 0x0006��ַ����1023�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������������ݶ���0xaa
//@EXECUTIONFLOW:8������CSUDIFlashRead������ȡ��һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���������������ݴӵ��߸���ʼ��1023������Ϊ0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0009(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1023];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, 1023);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0006, pucBuf, 1023),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006, pucBuf, 1023),"����������ʧ��");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, 1023);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0006,pucBuf,1023),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006,pucBuf,1023),"����������ʧ��");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "���߲�����У�����ʧ��");
	}

	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=6; i < 1029; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "�ھŲ�����У�����ʧ��");
	}

	for( ;  i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:��ʼ��ַΪż��ַ����д����Ϊż��ʱ�Ķ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddressȡg_uTestStartAddress + 0x0006
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1024
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite������g_uTestStartAddress + 0x0006��ַд��1024��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 0x0006��ַ����1024�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashWrite������g_uTestStartAddress + 0x0006��ַд��1024��0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashRead������g_uTestStartAddress + 0x0006��ַ����1024�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���������������ݶ���0xaa
//@EXECUTIONFLOW:8������CSUDIFlashRead������ȡ��һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���������������ݴӵ��߸���ʼ��1024������Ϊ0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0010(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1024];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, 1024);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0006, pucBuf, 1024),"�ڶ�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006, pucBuf, 1024),"����������ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, 1024);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress + 0x0006,pucBuf,1024),"���岽����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006,pucBuf,1024),"����������ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "���߲�����У�����ʧ��");
	}
	
	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=6; i < 1030; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucData[i], "�ھŲ�����У�����ʧ��");
	}

	for( ;  i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:���Զ�����ͬʱ��дflash�Ĳ�ͬblockʱ�����
//@PRECONDITION:Flash�ѳɹ���ʼ��
//@INPUT:��������Ϸ�  
//@EXPECTATION:ÿһ�����ﵽԤ�ڽ��
//@EXECUTIONFLOW:1������д����1��������Flash��1��Blockд��1024�ֽڵ�����0xaa
//@EXECUTIONFLOW:2������д����2��������Flash��2��Blockд��1024�ֽڵ�����0x88
//@EXECUTIONFLOW:3������������1���������Flash�ĵ�1��Block����1024�ֽڵ����ݣ������õ������ݶ�Ϊ0xaa
//@EXECUTIONFLOW:4������������2���������Flash�ĵ�2��Block����1024�ֽڵ����ݣ������õ������ݶ�Ϊ0x88
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0011(void)
{
        int nPriority = 128;
	int nParam = 0;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle3 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle4 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread1", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashWrite1, &nParam, &hThreadhandle1),"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread2", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashWrite2, &nParam, &hThreadhandle2),"��������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��");
		
	CSUDIOSThreadSleep(5000);

	while(!g_bFlashWrite1)
	{
		;
	}
	while(!g_bFlashWrite2)
	{
		;
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE != g_bFlagWrite1)&&(CSUDI_TRUE != g_bFlagWrite2),"����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread3", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashRead1, &nParam, &hThreadhandle3),"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle3,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread4", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashRead2, &nParam, &hThreadhandle4),"��������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle4,"�����ɹ����������������������Ϊ��");
	
	CSUDIOSThreadSleep(5000);

	while(!g_bFlashRead1)
	{
		CSUDIOSThreadSleep(2000);
	}
	while(!g_bFlashRead2)
	{
		CSUDIOSThreadSleep(2000);
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE != g_bFlagRead1)&&(CSUDI_TRUE != g_bFlagRead2),"����ʧ��");
			
	CSTK_FATAL_POINT
	{
		if (hThreadhandle1 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������1ʧ��\n");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������2ʧ��\n");
			   hThreadhandle2 = CSUDI_NULL;
		}
		if (hThreadhandle3 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle3), "ɾ������3ʧ��\n");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle4 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle4), "ɾ������4ʧ��\n");
			   hThreadhandle2 = CSUDI_NULL;
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:���Զ�����ͬʱ��ͬһblock���ж�дʱ�Ĺ��ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:��������Ϸ�
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:
//@EXECUTIONFLOW:1��CSUDIFlashWrite Flash ĳ��Block������Ϊ0x55��ϣ���ɹ�
//@EXECUTIONFLOW:2����������1��Flash ĳ��Block�ĺ�1024���ֽ�����дΪ0x88
//@EXECUTIONFLOW:3����������2��Flash ĳ��Block�ĺ�1024���ֽ�����дΪ0xaa
//@EXECUTIONFLOW:4������CSUDIFlashRead��ȡ��ǰд��Block �ĺ�1024�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5����������������Ϊ0x88����Ϊ0xaa
//@EXECUTIONFLOW:6������CSUDIFlashRead��ȡ��ǰд��Block ��0---(BolckSize-1024) �����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����������������Ϊ0x55
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0012(void)
{
	int i;
	CSUDI_UINT8 *pucBuf = CSUDI_NULL;
	
      	int nPriority = 128;
		
	int nParam = 0;
	
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;

	pucBuf = CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL((pucBuf != CSUDI_NULL),"����ռ�ʧ��\n");
	
	memset(pucBuf, 0x55, g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress+g_uTestFlashSize-g_uTestBlockSize, pucBuf, g_uTestBlockSize)), "Step fail");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread1", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashWrite3, &nParam, &hThreadhandle1),"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread2", nPriority , FLASH_TEST_STACK_SIZE, 
							 TestFlashWrite4, &nParam, &hThreadhandle2),"��������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��");

	CSUDIOSThreadSleep(5000);
	
	while(!g_bFlashWrite3)
	{
		CSUDIOSThreadSleep(10);
	}
	while(!g_bFlashWrite4)
	{
		CSUDIOSThreadSleep(10);
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE != g_bFlagWrite3)&&(CSUDI_TRUE != g_bFlagWrite4),"����ִ���ڲ�ʧ��");

	//Ϊȷ���Ƚ�����Ϊ�ڴ��ж���
	memset(pucBuf, 0, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + g_uTestFlashSize - g_uTestBlockSize, pucBuf, g_uTestBlockSize), "����������ʧ��");

	for(i=(g_uTestBlockSize-1024); i < 1024; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((pucBuf[i] == 0x88)||(pucBuf[i] == 0xaa),"���岽����У�����ʧ��");
	}

	for(i=0; i < (g_uTestBlockSize-1024); i++)
	{
		CSTK_ASSERT_TRUE_FATAL(pucBuf[i] == 0x55,"���߲�����У�����ʧ��");
	}
	
	CSTK_FATAL_POINT;
		
	if (pucBuf != CSUDI_NULL)
	{
		CSUDIOSFree(pucBuf);
		pucBuf = CSUDI_NULL;
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

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION: ��ͬһ��ַ�������ж�дʱ��ǿ�Ȳ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1024
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite��FLASH��д��1024���ֽڵ�����,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead��FLASH�ж�ȡ1024���ֽڵ�����,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4�������õ���������ڶ���д���������ͬ
//@EXECUTIONFLOW:5���ظ�2--4��REPERT_COUNT�Σ�����ÿ�ζ��ﵽԤ�ڽ��
//@EXECUTIONFLOW:6������CSUDIFlashRead��FLASH�ж�ȡ��һ��block������,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7�������õ�������ǰ1024����д���������ͬ�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0013(void)
{
	int i;
	CSUDI_UINT8 pucBuf[1024];
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	for(i=0; i < 1024; ++i)
	{
		pucBuf[i] = i%256;
	}
	
	for(i=0; i < REPERT_COUNT; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf, 1024),"�ڶ�������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf, 1024),"����������ʧ��");

		for(i=0; i < 1024; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(i%256 == pucBuf[i], "���Ĳ�����У�����ʧ��");
		}
	}
	memset(pucData, '\0', g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"�ڰ˲�����ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i%256 == pucData[i], "�ھŲ�����У�����ʧ��");
	}

	for( ;  i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION: дȫΪ0xff���ݲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWrite��FLASH��һ������ȫΪ0xff������,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead��FLASH�ж�ȡһ������������,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4�������õ���������ڶ���д���������ͬ
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0014(void)
{
	int i;
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");
	
	memset(pucData, 0xff, g_uTestBlockSize);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucData, g_uTestBlockSize),"�ڶ�������ʧ��");
	
	memset(pucData, 0x00, g_uTestBlockSize);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucData, g_uTestBlockSize),"����������ʧ��");

	for( i=0;  i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "�ھŲ�����У�����ʧ��");
	}
	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:���������Ե�FLASH�ռ���в�������д����ʱ�Ķ�д���ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestFlashSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestFlashSizeΪ�����ڲ��Ե�flash�ռ��С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace���������������Ե�FLASH�ռ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��g_uTestFlashSize���ֽ�0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestFlashSize���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace���������������Ե�FLASH�ռ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��g_uTestFlashSize���ֽ�0xbb����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestFlashSize���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xbb
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
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTCPrint("���������У���ȴ�!!\n");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize),"��һ������ʧ��");

	CSTCPrint("��һ�����Գɹ�!!\n");	
	
	unLeftSzie = g_uTestFlashSize;
	unTestStartAddr = g_uTestStartAddress;
	while (unLeftSzie > 0)
	{
		memset(pucBuf, 0x55, unMallocSzie);
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(unTestStartAddr,pucBuf,unOperSize),"�ڶ�������ʧ��");

		memset(pucBuf, '\0', unMallocSzie);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"����������ʧ��");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
		}

		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize),"���岽����ʧ��");
	unLeftSzie = g_uTestFlashSize;
	unTestStartAddr = g_uTestStartAddress;
	
	while (unLeftSzie > 0)
	{
		memset(pucBuf, 0xbb, unMallocSzie);

		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(unTestStartAddr,pucBuf, unOperSize),"����������ʧ��");

		memset(pucBuf, '\0', unOperSize);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"���߲�����ʧ��");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "�ڰ˲�����У�����ʧ��");
		}

		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}

 	CSTK_FATAL_POINT

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:���ڲ�������д�������ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 16
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��16���ֽ�pubBufָ������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����16���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݺ�д���������ͬ
//@EXECUTIONFLOW:5������CSUDIFlashRead������g_uTestStartAddress��ַ������һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6����������������ǰ16���ֽ�������pubBufָ���������ͬ�������ȫΪ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0002(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[16];
	
	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");
	
	for(i = 0; i < 16; ++i)
	{
		pucBuf[i] = i;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, 16),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', 16);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 16),"����������ʧ��");

	for(i=0; i < 16; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}
	
	memset(pucData, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"���岽����ʧ��");

	for(i=0; i < 16; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(i == pucData[i], "����������У�����ʧ��");
	}
	
	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "����������У�����ʧ��");
	}

 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:���鲻������д�����ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace��������ǰ����block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase�������һ��block��д��g_uTestBlockSize���ֽڵ�����0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��g_uTestBlockSize���ֽڵ�0xee����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xee
//@EXECUTIONFLOW:9������CSUDIFlashRead������g_uTestStartAddress��ַ����ǰ����block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10���������������ݵ�һ��block������Ϊ0xee���ڶ���block������Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0003(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, 2*g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xee, 2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"���岽����ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"����������ʧ��");

	memset(pucBuf, '\0',2* g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"���߲�����ʧ��");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xee == pucBuf[i], "�ڰ˲�����У�����ʧ��");
	}
	
	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 2*g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xee== pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "��ʮ������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:��鲻������д�������ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddressֵ�Ϸ�
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize + 512
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace��������ǰ����block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��g_uTestBlockSize + 512��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize + 512�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace��������ǰ����block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��g_uTestBlockSize + 512��0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize + 512�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xaa
//@EXECUTIONFLOW:9������CSUDIFlashRead������g_uTestStartAddress��ַ����ǰ����block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10����������������ǰg_uTestBlockSize + 512��0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0004(void)
{
	
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, 2*g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, g_uTestBlockSize + 512),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize + 512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, 2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"���岽����ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,g_uTestBlockSize + 512),"����������ʧ��");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize + 512),"���߲�����ʧ��");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "�ڰ˲�����У�����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 2*g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=0; i < g_uTestBlockSize + 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "��ʮ������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:�Ҳ�block���룬���block������ʱ��������д�������ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress + 512
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize - 512
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 512��ַд��g_uTestBlockSize - 512���ֽ�����0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 512��ַ����g_uTestBlockSize - 512���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 512��ַд��g_uTestBlockSize - 512���ֽ�����0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress + 512��ַ����g_uTestBlockSize - 512���ֽ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xaa
//@EXECUTIONFLOW:9������CSUDIFlashRead������g_uTestStartAddress��ַ������һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10����������������ǰ512���ֽ���0xff�������ȫ��Ϊ0xaa
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0005(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"��һ������ʧ��");


	memset(pucBuf, 0x55, 2*g_uTestBlockSize);

	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 512, pucBuf, g_uTestBlockSize - 512),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 512, pucBuf, g_uTestBlockSize - 512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, 2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"���岽����ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 512,pucBuf,g_uTestBlockSize - 512),"����������ʧ��");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 512,pucBuf,g_uTestBlockSize - 512),"���߲�����ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "�ڰ˲�����У�����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 2*g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=0; i < 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff== pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for(i=512; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "��ʮ������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:���block���룬�Ҳ�block������ʱ��������д�������ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize - 512
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��g_uTestBlockSize - 512���ֽڵ�0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize - 512���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��g_uTestBlockSize - 512���ֽڵ�0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize - 512���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xaa
//@EXECUTIONFLOW:9������CSUDIFlashRead������g_uTestStartAddress��ַ������һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10����������������ǰg_uTestBlockSize - 512���ֽ���0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0006(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, 2*g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress, pucBuf, g_uTestBlockSize - 512),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize - 512),"����������ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, 2*g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 2*g_uTestBlockSize),"���岽����ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,g_uTestBlockSize - 512),"����������ʧ��");

	memset(pucBuf, '\0', 2*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize - 512),"���߲�����ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "�ڰ˲�����У�����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 2*g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=0; i < g_uTestBlockSize - 512; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for( ; i < 2*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "��ʮ������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");

	return CSUDI_TRUE;	
}


//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:��ʼ��ַΪ���ַ����д����Ϊ����ʱ��������д�������ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress�ֱ�ȡg_uTestStartAddress + 0x0001
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1023
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 0x0001��ַд��1023��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1023�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 0x0001��ַд��1023��0xbb����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1023�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xbb
//@EXECUTIONFLOW:9������CSUDIFlashRead������ȡ��һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10���������������ݴӵڶ�����ʼ��1023������Ϊ0x88�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0007(void)
{
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0001, pucBuf, 1023),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001, pucBuf, 1023),"����������ʧ��");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xbb, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"���岽����ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0001,pucBuf,1023),"����������ʧ��");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001,pucBuf,1023),"���߲�����ʧ��");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "�ڰ˲�����У�����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=1; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xbb== pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "��ʮ������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:��ʼ��ַΪ���ַ����д����Ϊż��ʱ��������д�������ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress�ֱ�ȡg_uTestStartAddress + 0x0001
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1024
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 0x0001��ַд��1024���ֽ�����0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1024���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 0x0001��ַд��1024���ֽ�����0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1024���ֽڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xaa
//@EXECUTIONFLOW:9������CSUDIFlashRead������ȡ��һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10���������������ݴӵڶ�����ʼ��1024������Ϊ0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0008(void)
{	
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0001, pucBuf, 1024),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001, pucBuf, 1024),"����������ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"���岽����ʧ��");

	memset(pucBuf, 0xaa, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0001,pucBuf,1024),"����������ʧ��");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0001,pucBuf,1024),"���߲�����ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "�ڰ˲�����У�����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=1; i < 1025; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "��ʮ������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:��ʼ��ַΪż��ַ����д����Ϊ����ʱ��������д�������ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddressȡg_uTestStartAddress + 0x0006
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1023
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 0x0006��ַд��1023��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 0x0001��ַ����1023�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 0x0006��ַд��1023��0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress + 0x0006��ַ����1023�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xaa
//@EXECUTIONFLOW:9������CSUDIFlashRead������ȡ��һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10���������������ݴӵ��߸���ʼ��1023������Ϊ0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0009(void)
{	
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0006, pucBuf, 1023),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006, pucBuf, 1023),"����������ʧ��");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(pucBuf, 0xaa, g_uTestBlockSize);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"���岽����ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0006,pucBuf,1023),"����������ʧ��");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006,pucBuf,1023),"���߲�����ʧ��");

	for(i=0; i < 1023; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "�ڰ˲�����У�����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=6; i < 1029; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "��ʮ������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION:��ʼ��ַΪż��ַ����д����Ϊż��ʱ��������д�������ܲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddressȡg_uTestStartAddress + 0x0006
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1024
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 0x0006��ַд��1024��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress + 0x0006��ַ����1024�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������������ݶ���0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIFlashWriteWithoutErase������g_uTestStartAddress + 0x0006��ַд��1024��0xaa����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIFlashRead������g_uTestStartAddress + 0x0006��ַ����1024�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���������������ݶ���0xaa
//@EXECUTIONFLOW:9������CSUDIFlashRead������ȡ��һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10���������������ݴӵ��߸���ʼ��1024������Ϊ0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0010(void)
{	
	int i;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

	memset(pucBuf, 0x55, g_uTestBlockSize);
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0006, pucBuf, 1024),"�ڶ�������ʧ��");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006, pucBuf, 1024),"����������ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"���岽����ʧ��");

	memset(pucBuf, 0xaa, g_uTestBlockSize);
 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress + 0x0006,pucBuf,1024),"����������ʧ��");

	memset(pucBuf, '\0', g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress + 0x0006,pucBuf,1024),"���߲�����ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "�ڰ˲�����У�����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=6; i < 1030; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucBuf[i], "��ʮ������У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucBuf[i], "��ʮ������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase   
//@DESCRIPTION: ��ͬһ��ַ�������в�������д����ʱ��ǿ�Ȳ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 1024
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace����������һ��block�ڵ����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase��FLASH��д��1024���ֽڵ�����0xaa,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIFlashRead��FLASH�ж�ȡ1024���ֽڵ�����,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4�������õ�������ȫ��Ϊ0xaa
//@EXECUTIONFLOW:5���ظ�1--4��REPERT_COUNT�Σ�����ÿ�ζ��ﵽԤ�ڽ��
//@EXECUTIONFLOW:6������CSUDIFlashRead��FLASH�ж�ȡ��һ��block������,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7�������õ�������ǰ1024��Ϊ0xaa�������ȫ��Ϊ0xff
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0011(void)
{
	int i;
	
	CSUDI_UINT8 pucBuf[1024];

	CSUDI_UINT8 *pucData = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucData,"�����ڴ�ʧ��");

	memset(pucBuf, 0xaa, sizeof(pucBuf));
	
	for(i=0; i < REPERT_COUNT; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"��һ������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf, 1024),"�ڶ�������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf, 1024),"����������ʧ��");

		for(i=0; i < 1024; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0xaa == pucBuf[i], "���Ĳ�����У�����ʧ��");
		}
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucData, g_uTestBlockSize),"���岽����ʧ��");

	for(i=0; i < 1024; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xaa== pucData[i], "����������У�����ʧ��");
	}

	for( ; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xff == pucData[i], "����������У�����ʧ��");
	}
	
 	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree (pucData),"������Դʧ��");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashEraseSpace
//@CASEGROUP:CSUDIFlashWriteWithoutErase 
//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:��������������д������д�����������Ե�FLASH�ռ�ʱ��ʱ��Ч�ʲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestFlashSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestFlashSizeΪ�����ڲ��Ե�flash��С
//@EXECUTIONFLOW:1������CSUDIFlashWrite������g_uTestStartAddress��ַд��g_uTestFlashSize��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�������õ���ɲ���д�������õ�ʱ��
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestFlashSize�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���õ���ɶ��������õ�ʱ�䣬�������õ�������ȫ��Ϊ0x55
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace������������flash���ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6�������õ���ɲ����������õ�ʱ��
//@EXECUTIONFLOW:7������CSUDIFlashWriteWithoutErase������g_uTestStartAddress��ַд��g_uTestFlashSize��0x55����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8�������õ���ɲ�������д�������õ�ʱ��
//@EXECUTIONFLOW:9������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestFlashSize�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10�������õ�������ȫ��Ϊ0x55
//@EXECUTIONFLOW:11����ӡ���ϸ��ֶ�д�����ķѵ�ʱ��
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0001(void)
{
	int i = 0;
	
	//��¼����д���õ�ʱ��(��λ����)
	int nTemp1 = 0;
	//��¼�����õ�ʱ��(��λ����)
	int nTemp2 = 0;
	//��¼�������õ�ʱ��(��λ����)
	int  nTemp3 = 0;
	//��¼��������д���õ�ʱ��	(��λ����)
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	CSTCPrint("���������У���ȴ�!!\n");

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	memset(pucBuf, 0x55, unMallocSzie);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");

	while (unLeftSzie)
	{
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(unTestStartAddr,pucBuf,unOperSize),"��һ������ʧ��");
		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");

	nTemp1 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");

	while (unLeftSzie)
	{
		memset(pucBuf, '\0', unMallocSzie);
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr, pucBuf, unOperSize),"����������ʧ��");
		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "���Ĳ�����У�����ʧ��");
		}
	}

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp2 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize),"���岽����ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp3 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	memset(pucBuf, 0x55, unMallocSzie);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");

	while (unLeftSzie)
	{
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(unTestStartAddr,pucBuf,unOperSize),"���߲�����ʧ��");
		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp4 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	unTestStartAddr = g_uTestStartAddress;
	unLeftSzie = g_uTestFlashSize;
	while (unLeftSzie)
	{
		memset(pucBuf, 0x00, unMallocSzie);
		unOperSize = unLeftSzie > unMallocSzie ? unMallocSzie : unLeftSzie;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(unTestStartAddr,pucBuf,unOperSize),"�ھŲ�����ʧ��");

		for(i=0; i < unOperSize; ++i)
		{
			CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[i], "��ʮ������У�����ʧ��");
		}

		unLeftSzie -= unOperSize;
		unTestStartAddr += unOperSize;
	}

	CSTCPrint("\n********�������Ե�FLASH�ռ��СΪ0x%x(�ֽ�)\n", g_uTestFlashSize);
	CSTCPrint("\n********����д�������Ե�FLASH�ռ����õ�ʱ��:%d(����)\n********��������д�������Ե�FLASH�ռ����õ�ʱ��:%d(����)", nTemp1, nTemp4);
	CSTCPrint("\n********���������Ե�FLASH�ռ����õ�ʱ��:%d(����)\n********�����������Ե�FLASH�ռ����õ�ʱ��:%d(����)\n", nTemp2, nTemp3);

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWriteWithoutErase 
//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:��������������д������д����һ��Blockʱ��ʱ��Ч�ʲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestBlockSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashWrite�����ѵ�һ��block������ȫ��дΪ0xbb����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�������õ���ɲ���дһ��block���õ�ʱ��
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����g_uTestBlockSize�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���õ���ɶ�һ��block�������õ�ʱ�䣬�������õ�������ȫ��Ϊ0xbb
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace����������һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6�������õ���ɲ���һ��block���õ�ʱ��
//@EXECUTIONFLOW:7������CSUDIFlashWriteWithoutErase�����ѵ�һ��block������ȫ��дΪ0xbb����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8�������õ���ɲ�������дһ��block���õ�ʱ��
//@EXECUTIONFLOW:9������CSUDIFlashRead������g_uTestStartAddress��ַ������һ��block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10�������õ�������ȫ��Ϊ0xbb
//@EXECUTIONFLOW:11����ӡ���ϸ��ֶ�д�����ķѵ�ʱ��
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0002(void)
{
	int i = 0;
	
	//��¼����д���õ�ʱ��(��λ����)
	int nTemp1 = 0;
	//��¼�����õ�ʱ��(��λ����)
	int nTemp2 = 0;
	//��¼�������õ�ʱ��(��λ����)
	int  nTemp3 = 0;
	//��¼��������д���õ�ʱ��	(��λ����)
	int nTemp4 = 0;
	
	CSUDIOSTimeVal_S sSysTime1;
	CSUDIOSTimeVal_S sSysTime2;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
		
	memset(pucBuf, 0xbb, g_uTestBlockSize);

	CSTCPrint("���������У���ȴ�!!\n");

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"��һ������ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp1 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	memset(pucBuf, '\0', g_uTestBlockSize);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, g_uTestBlockSize),"����������ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp2 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);
	
	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestBlockSize),"���岽����ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp3 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	memset(pucBuf, 0xbb, g_uTestBlockSize);	

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"���߲�����ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp4 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=0; i < g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[i], "��ʮ������У�����ʧ��");
	}

	CSTCPrint("\n********һ��Block��СΪ0x%x(�ֽ�)\n", g_uTestBlockSize);
	CSTCPrint("\n********����дһ��Block���õ�ʱ��:%d(����)\n********��������дһ��Block���õ�ʱ��:%d(����)", nTemp1, nTemp4);
	CSTCPrint("\n********��һ��Block���õ�ʱ��:%d(����)\n********����һ��Block���õ�ʱ��:%d(����)\n", nTemp2, nTemp3);

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWriteWithoutErase 
//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:��������������д������д�������Blockʱ��ʱ��Ч�ʲ���
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 3*g_uTestBlockSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@EXECUTIONFLOW:1������CSUDIFlashWrite������ǰ����block������ȫ��дΪ0x66����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2�������õ���ɲ���д����block���õ�ʱ��
//@EXECUTIONFLOW:3������CSUDIFlashRead������g_uTestStartAddress��ַ����3*g_uTestBlockSize�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���õ���ɶ�����block�������õ�ʱ�䣬�������õ�������ȫ��Ϊ0x66
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace��������ǰ����block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6�������õ���ɲ�������block���õ�ʱ��
//@EXECUTIONFLOW:7������CSUDIFlashWriteWithoutErase������ǰ����block������ȫ��дΪ0x66����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8�������õ���ɲ�������д����block���õ�ʱ��
//@EXECUTIONFLOW:9������CSUDIFlashRead������g_uTestStartAddress��ַ����ǰ����block�����ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10�������õ�������ȫ��Ϊ0x66
//@EXECUTIONFLOW:11����ӡ���ϸ��ֶ�д�����ķѵ�ʱ��
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0003(void)
{
	int i = 0;
	
	//��¼����д���õ�ʱ��(��λ����)
	int nTemp1 = 0;
	//��¼�����õ�ʱ��(��λ����)
	int nTemp2 = 0;
	//��¼�������õ�ʱ��(��λ����)
	int  nTemp3 = 0;
	//��¼��������д���õ�ʱ��	(��λ����)
	int nTemp4 = 0;
	
	CSUDIOSTimeVal_S sSysTime1;
	CSUDIOSTimeVal_S sSysTime2;
	
	CSUDI_UINT8 *pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(3*g_uTestBlockSize);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
		
	memset(pucBuf, 0x66, 3*g_uTestBlockSize);

	CSTCPrint("���������У���ȴ�!!\n");

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf,3*g_uTestBlockSize),"��һ������ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp1 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);
	
	memset(pucBuf, '\0', 3*g_uTestBlockSize);

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, 3*g_uTestBlockSize),"����������ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp2 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);
	
	for(i=0; i < 3*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x66 == pucBuf[i], "���Ĳ�����У�����ʧ��");
	}

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, 3*g_uTestBlockSize),"���岽����ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp3 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	memset(pucBuf, 0x66, 3*g_uTestBlockSize);	

	memset(&sSysTime1, 0, sizeof(sSysTime1));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime1),"��ȡϵͳʱ��ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWriteWithoutErase(g_uTestStartAddress,pucBuf,3*g_uTestBlockSize),"���߲�����ʧ��");

	memset(&sSysTime2, 0, sizeof(sSysTime2));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime2),"��ȡϵͳʱ��ʧ��");
	
	nTemp4 = (sSysTime2.m_nSecond*1000+sSysTime2.m_nMiliSecond) - (sSysTime1.m_nSecond*1000+sSysTime1.m_nMiliSecond);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress,pucBuf,3*g_uTestBlockSize),"�ھŲ�����ʧ��");

	for(i=0; i < 3*g_uTestBlockSize; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(0x66 == pucBuf[i], "��ʮ������У�����ʧ��");
	}

	CSTCPrint("\n********һ��Block��СΪ0x%x(�ֽ�)\n", g_uTestBlockSize);
	CSTCPrint("\n********����д����Block���õ�ʱ��:%d(����)\n********��������д����Block���õ�ʱ��:%d(����)", nTemp1, nTemp4);
	CSTCPrint("\n********������Block���õ�ʱ��:%d(����)\n********��������Block���õ�ʱ��:%d(����)\n", nTemp2, nTemp3);

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"������Դʧ��");

	return CSUDI_TRUE;
}

/*lichanghua modify:���������ʱ64��1000,��ɲ���ʱ�䳤�Ｘ��Сʱ,û�б�Ҫ*/
#define BLOCK_DIVIDE_FACTOR  (8)

#define LOOP_COUNT             (2)

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWriteWithoutErase 
//@CASEGROUP:CSUDIFlashWrite
//@DESCRIPTION:flash��д����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = 3*g_uTestBlockSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1��������flash��ÿ����д�����ݣ��ٶ�ȡУ��
//@EXECUTIONFLOW:2������1ѭ��ִ��100�飬ÿ����֤�����Ƿ�һ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestBlockSizeΪ�����ڲ��Ե�block�Ĵ�С
//@REMARK: �κβ���(������ɾ������д)ʧ�ܽ�����CSUDI_FAILURE,����ӡ������Ϣ: "error occur at i = ? while call [XXX] func!"\r\n 
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
		CSTK_ASSERT_TRUE_FATAL(0,"�������������ڴ�ʧ��\n");
	}
	
	pBuffer2 = CSUDIOSMalloc(g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);	
	if(pBuffer2 == NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(0,"У�黺���������ڴ�ʧ��\n");
	}

	for(i = 0; i < LOOP_COUNT; i++)
	{
		for(j = 0; j < g_uTestFlashSize/g_uTestBlockSize;  j++)
		{
			addr = g_uTestStartAddress + j*g_uTestBlockSize;


			//step1: erase a 64*1024 block.
			eErrorCode = CSUDIFlashEraseSpace(addr, g_uTestBlockSize);
			CSTK_ASSERT_TRUE_FATAL(eErrorCode == 0,"CSUDIFlashEraseSpace ʧ��\n");

			//step2: write data to 8*(8*1024) bytes region in every block.
			for(k = 0; k < BLOCK_DIVIDE_FACTOR; k++)
			{
				addr1 = addr + k*(g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				memset(pBuffer1, i + j + k, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				eErrorCode = CSUDIFlashWriteWithoutErase(addr1,pBuffer1, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				CSTK_ASSERT_TRUE_FATAL(eErrorCode == 0,"CSUDIFlashWriteWithoutErase ʧ��\n");
			}

			//step3: check data of  8*(8*1024) bytes region in every block.
			for(k = 0; k < BLOCK_DIVIDE_FACTOR; k++)
			{
				addr1 = addr + k*(g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				memset(pBuffer1, i + j + k, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				memset(pBuffer2, 0, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				eErrorCode = CSUDIFlashRead(addr1, pBuffer2, g_uTestBlockSize/BLOCK_DIVIDE_FACTOR);
				
				CSTK_ASSERT_TRUE_FATAL(eErrorCode == 0,"CSUDIFlashRead ʧ��\n");
				
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
//@DESCRIPTION:����ϵͳ��FLASH�ռ���ж�дʱ��ʱ��ͳ��
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:dwStartAddress = g_uTestStartAddress
//@INPUT:pucBuf ֵ�Ϸ�
//@INPUT:uDataLength = g_uTestFlashSize
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@REMARK:g_uTestStartAddressΪ�����ڲ��Ե�flash�ռ���ʼ��ַ��g_uTestFlashSizeΪ�����ڲ��Ե�flash�ռ��С
//@EXECUTIONFLOW:1���Ƚ����ڲ��Ե�Flash��С�Ƿ�С�ڿɹ��û����Ե�Flash��С������(�ɹ��û�����ʹ�õ�Flash>1M)
//@EXECUTIONFLOW:2������CSUDIOSMalloc()����1M�ڴ��С���ڲ��ԣ�������ֵ0x55
//@EXECUTIONFLOW:3����ȡ��ǰʱ�䣬����CSUDIFlashWrite()д��1M�����ݣ�ѭ��10�Σ���������Ӧ��д���ٶ�
//@EXECUTIONFLOW:4����ȡ��ǰʱ�䣬����CSUDIFlashRead()����1M�����ݣ�ѭ��10�Σ���������Ӧ�Ķ����ٶȣ�����ֵΪ0x55
//@EXECUTIONFLOW:5�����ڴ����ݸ�ֵΪ0xbb
//@EXECUTIONFLOW:6����ȡ��ǰʱ�䣬����CSUDIFlashWrite()д��10K�����ݣ�ѭ��100�Σ���������Ӧ��д���ٶ�
//@EXECUTIONFLOW:7����ȡ��ǰʱ�䣬����CSUDIFlashRead()����10K�����ݣ�ѭ��100�Σ���������Ӧ�Ķ����ٶȣ�����ֵΪ0xbb
//@EXECUTIONFLOW:7����ӡ�������������ʹ�õ��ڴ���Դ
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
	
	CSTK_ASSERT_TRUE_FATAL( g_uTestFlashSize >= nMemorySize1M ,"��һ������ʧ��");
	
	pucBuf = (CSUDI_UINT8 *)CSUDIOSMalloc( nMemorySize1M );
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucBuf,"�����ڴ�ʧ��");
	
	memset(pucBuf, 0x55, nMemorySize1M );

	CSTCPrint("���������У���ȴ�!!\n");
	
	//д��1M�����ݣ�ѭ��10�Σ���������Ӧ��д���ٶ�

	CSTCPrint("���ڽ���д��1M,ѭ��10�ε�flash�ٶȲ���... \n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp1),"��������ȡϵͳʱ��ʧ��");
	
	for( i=0; i < 10 ; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress, pucBuf, nMemorySize1M),"����������ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"��������ȡϵͳʱ��ʧ��");
		
	temp1.m_nSecond = sSysTime.m_nSecond - temp1.m_nSecond;
	temp1.m_nMiliSecond = sSysTime.m_nMiliSecond - temp1.m_nMiliSecond;
	
	//����1M�����ݣ�ѭ��10�Σ���������Ӧ�Ķ����ٶ�

	CSTCPrint("���ڽ��ж���1M,ѭ��10�ε�flash�ٶȲ���... \n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp2),"���Ĳ���ȡϵͳʱ��ʧ��");
	
	for( i=0; i < 10 ; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, nMemorySize1M ),"���Ĳ�����ʧ��");

		for( j=0; j< nMemorySize1M ; ++j )
		{
		    CSTK_ASSERT_TRUE_FATAL(0x55 == pucBuf[j], "���Ĳ�����У�����ʧ��");
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"���Ĳ���ȡϵͳʱ��ʧ��");
	
	temp2.m_nSecond = sSysTime.m_nSecond - temp2.m_nSecond;
	temp2.m_nMiliSecond = sSysTime.m_nMiliSecond - temp2.m_nMiliSecond;
	
	memset(pucBuf, 0xbb, nMemorySize1M );
	
	//д��10K�����ݣ�ѭ��100�Σ���������Ӧ�Ķ����ٶ�

	CSTCPrint("���ڽ���д��10K,ѭ��100�ε�flash�ٶȲ���... \n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp3),"��������ȡϵͳʱ��ʧ��");
	
	for( i=0; i < 100 ; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(g_uTestStartAddress,pucBuf, nMemorySize10K ),"����������ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"��������ȡϵͳʱ��ʧ��");
		
	temp3.m_nSecond = sSysTime.m_nSecond - temp3.m_nSecond;
	temp3.m_nMiliSecond = sSysTime.m_nMiliSecond - temp3.m_nMiliSecond;
	
	//����10K�����ݣ�ѭ��100�Σ���������Ӧ�Ķ����ٶȣ�

	CSTCPrint("���ڽ��ж���10K,ѭ��100�ε�flash�ٶȲ���... \n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&temp4),"���߲���ȡϵͳʱ��ʧ��");
	
	for( i=0; i < 100 ; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(g_uTestStartAddress, pucBuf, nMemorySize10K ),"���߲�����ʧ��");
		
		for( j=0; j< nMemorySize10K ; ++j )
		{
		    CSTK_ASSERT_TRUE_FATAL(0xbb == pucBuf[j], "���Ĳ�����У�����ʧ��");
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&sSysTime),"���߲���ȡϵͳʱ��ʧ��");
	
	temp4.m_nSecond = sSysTime.m_nSecond - temp4.m_nSecond;
	temp4.m_nMiliSecond = sSysTime.m_nMiliSecond - temp4.m_nMiliSecond;
	
	CSTCPrint("ÿ��д��1MB���ݣ�ѭ��10�ε�����Ϊ��%0.2f M/s \n" , (1.0 * 10) / ((temp1.m_nSecond*1000) + temp1.m_nMiliSecond ) * 1000 );
	CSTCPrint("ÿ�ζ���1MB���ݣ�ѭ��10�ε�����Ϊ��%0.2f M/s \n" , (1.0 * 10) / ((temp2.m_nSecond*1000) + temp2.m_nMiliSecond ) * 1000 );
	CSTCPrint("ÿ��д��10K���ݣ�ѭ��100�ε�����Ϊ��%0.2f M/s \n" , (10.0 * 100) / ((temp3.m_nSecond*1000) + temp3.m_nMiliSecond ) * 1000 / 1024);
	CSTCPrint("ÿ�ζ���10K���ݣ�ѭ��100�ε�����Ϊ��%0.2f M/s \n" , (10.0 * 100) / ((temp4.m_nSecond*1000) + temp4.m_nMiliSecond ) * 1000 / 1024);
	
 	CSTK_FATAL_POINT
 	{

	   CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucBuf),"������Դʧ��");
    }
	
	return CSUDI_TRUE;
}


//����(min)��(max-1)֮��������
static int GetRandom(int min, int max)
{ 
	if(min > max)
	{
		CSTCPrint("����������Сֵ����,min=%d,max=%d\n", min, max);
		return CSUDI_FALSE;
	}	
	int nRet = ((rand()%((max) - (min))) + (min)); 
	return ( nRet == max ? (nRet-1) : nRet); 
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:�����ַ�����С��д����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:flash���Ե���ʼ��ַ
//@INPUT:flash���ԵĴ�С
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1����ȡflash������Ϣ����������С
//@EXECUTIONFLOW:2���������flash����ʼ��ַ��д��С��д�����ݲ�����CSUDIFlashWriteд������
//@EXECUTIONFLOW:3������CSUDIFlashRead��ȡ����
//@EXECUTIONFLOW:4��У������
//@EXECUTIONFLOW:5������CSUDIFlashEraseSpace���������С
//@EXECUTIONFLOW:6��У������
//@EXECUTIONFLOW:6���ظ�����2, 3, 4, 5, 6    100��
//@EXECUTIONFLOW:7��������Դ
CSUDI_BOOL  CSTC_FLASH_IT_Performance_0005(void)
{
	CSUDIFLASHRegionSuit_S sFlashSuit;
	DWORD unBlkSize, dwTestAddr, dwTestArea, dwTestSize, dwWriteSize;
	BYTE ucWriteValue = 0;
	PBYTE pWriteBuf = NULL;
	int ii, nTestTime;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashGetInfo(EM_UDI_FLASH_ALL, &sFlashSuit), "��һ����ȡflash��Ϣʧ��");
	unBlkSize = sFlashSuit.m_uMaxBlockSize;
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (2 *unBlkSize), "�û��ɲ��Կռ�̫С");
	
	pWriteBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*unBlkSize);
	CSTK_ASSERT_TRUE_FATAL(pWriteBuf != NULL, "����ռ�ʧ��");

	nTestTime = 100;
	
	while (nTestTime--)
	{
		dwTestAddr = GetRandom(g_uTestStartAddress, g_uTestStartAddress + g_uTestFlashSize-1);
		dwTestArea = g_uTestStartAddress + g_uTestFlashSize - dwTestAddr;
		dwTestSize = dwTestArea > (2*unBlkSize) ? (2*unBlkSize) : dwTestArea;
		dwWriteSize = GetRandom(1, dwTestSize);

		ucWriteValue = GetRandom(1, 255);
		memset(pWriteBuf, ucWriteValue, 2 * unBlkSize);
		CSTCPrint("flash д��ʼdwTestAddr=0x%x, dwWriteSize=0x%x, unBlkSize=0x%x\n", dwTestAddr, dwWriteSize, unBlkSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashWrite(dwTestAddr, pWriteBuf, dwWriteSize), "�ڶ���дflashʧ��");	
		CSTCPrint("flash д���\n");

		CSTCPrint("flash ����ʼdwTestAddr=0x%x, dwWriteSize=0x%x\n", dwTestAddr, dwWriteSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(dwTestAddr, pWriteBuf, dwWriteSize), "��������flashʧ��");
		CSTCPrint("flash �����\n");
		for (ii=0; ii<dwWriteSize; ii++)
		{
			CSTK_ASSERT_TRUE_FATAL(pWriteBuf[ii] == ucWriteValue, "���Ĳ�У��flashʧ��");
		}

		CSTCPrint("flash ����ʼ\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashEraseSpace(g_uTestStartAddress, g_uTestFlashSize), "���岽��flashʧ��");	

		CSTCPrint("flash ����ʼdwTestAddr=0x%x, dwWriteSize=0x%x\n", dwTestAddr, dwWriteSize);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFlashRead(dwTestAddr, pWriteBuf, dwWriteSize), "��������flashʧ��");

		for (ii=0; ii<dwWriteSize; ii++)
		{
			CSTK_ASSERT_TRUE_FATAL(pWriteBuf[ii] == 0xff, "���߲�������У��ʧ��");
		}

		CSTCPrint("У���������\n");
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pWriteBuf), "������Դʧ��");

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
		CSTCPrint("��һ����ȡflash��Ϣʧ��\n");
		bRandRunFlag1 = FALSE;
		goto done;
	}
	
	unBlkSize = sFlashSuit.m_uMaxBlockSize;
	if (g_uTestFlashSize < (4 *unBlkSize))
	{
		CSTCPrint("�û��ɲ��Կռ�̫С\n");
		bRandRunFlag1 = FALSE;
		goto done;
	}
	
	pWriteBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*unBlkSize);
	if (pWriteBuf == NULL)
	{
		CSTCPrint("����ռ�ʧ��\n");
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
		CSTCPrint("flash д��ʼdwTestAddr=0x%x, dwWriteSize=0x%x, unBlkSize=0x%x\n", dwTestAddr, dwWriteSize, unBlkSize);
		if (CSUDI_SUCCESS != CSUDIFlashWrite(dwTestAddr, pWriteBuf, dwWriteSize))
		{
			CSTCPrint("�ڶ���дflashʧ��\n");
			bRandRunFlag1 = FALSE;
			break;
		}
		
		CSTCPrint("flash д���\n");

		CSTCPrint("flash ����ʼdwTestAddr=0x%x, dwWriteSize=0x%x\n", dwTestAddr, dwWriteSize);
		if (CSUDI_SUCCESS != CSUDIFlashRead(dwTestAddr, pWriteBuf, dwWriteSize))
		{
			CSTCPrint("������дflashʧ��\n");
			bRandRunFlag1 = FALSE;
			break;
		}
		
		CSTCPrint("flash �����\n");
		for (ii=0; ii<dwWriteSize; ii++)
		{
			if (pWriteBuf[ii] != ucWriteValue)
			{
				CSTCPrint("���Ĳ�У��flashʧ��");
				bRandRunFlag1 = FALSE;
				break;
			}
		}

		if (!bRandRunFlag1)
		{
			break;
		}
		CSTCPrint("У���������\n");
	}

	if (CSUDI_SUCCESS != CSUDIOSFree(pWriteBuf))
	{
		CSTCPrint("������Դʧ��\n");
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
		CSTCPrint("��һ����ȡflash��Ϣʧ��\n");
		bRandRunFlag2 = FALSE;
		goto done;
	}
	
	unBlkSize = sFlashSuit.m_uMaxBlockSize;
	if (g_uTestFlashSize < (4 *unBlkSize))
	{
		CSTCPrint("�û��ɲ��Կռ�̫С\n");
		bRandRunFlag2 = FALSE;
		goto done;
	}
	
	pWriteBuf = (CSUDI_UINT8 *)CSUDIOSMalloc(2*unBlkSize);
	if (pWriteBuf == NULL)
	{
		CSTCPrint("����ռ�ʧ��\n");
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
		CSTCPrint("flash д��ʼdwTestAddr=0x%x, dwWriteSize=0x%x, unBlkSize=0x%x\n", dwTestAddr, dwWriteSize, unBlkSize);
		if (CSUDI_SUCCESS != CSUDIFlashWrite(dwTestAddr, pWriteBuf, dwWriteSize))
		{
			CSTCPrint("�ڶ���дflashʧ��\n");
			bRandRunFlag1 = FALSE;
			break;
		}
		
		CSTCPrint("flash д���\n");

		CSTCPrint("flash ����ʼdwTestAddr=0x%x, dwWriteSize=0x%x\n", dwTestAddr, dwWriteSize);
		if (CSUDI_SUCCESS != CSUDIFlashRead(dwTestAddr, pWriteBuf, dwWriteSize))
		{
			CSTCPrint("������дflashʧ��\n");
			bRandRunFlag1 = FALSE;
			break;
		}
		
		CSTCPrint("flash �����\n");
		for (ii=0; ii<dwWriteSize; ii++)
		{
			if (pWriteBuf[ii] != ucWriteValue)
			{
				CSTCPrint("���Ĳ�У��flashʧ��");
				bRandRunFlag2 = FALSE;
				break;
			}
		}

		if (!bRandRunFlag2)
		{
			break;
		}
		CSTCPrint("У���������\n");
	}

	if (CSUDI_SUCCESS != CSUDIOSFree(pWriteBuf))
	{
		CSTCPrint("������Դʧ��\n");
	}

done:
	RandThread2Done = TRUE;
}

//@CASEGROUP:CSUDIFlashRead  
//@CASEGROUP:CSUDIFlashWrite   
//@DESCRIPTION:���߳������ַ�����С��д����
//@PRECONDITION:FLASHģ��֮ǰ�ѳɹ���ʼ��
//@INPUT:flash���Ե���ʼ��ַ
//@INPUT:flash���ԵĴ�С
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1����ȡ�û����õķ�����Ϣ
//@EXECUTIONFLOW:2����ȡflash������Ϣ����������С
//@EXECUTIONFLOW:3�����������߳������дflash
//@EXECUTIONFLOW:3���������flash����ʼ��ַ��д��С��д������
//@EXECUTIONFLOW:4������CSUDIFlashWriteд������
//@EXECUTIONFLOW:5������CSUDIFlashRead��ȡ����
//@EXECUTIONFLOW:6��У������
//@EXECUTIONFLOW:7���ظ�����3, 4, 5, 6    10��
//@EXECUTIONFLOW:8��������Դ
CSUDI_BOOL  CSTC_FLASH_IT_Performance_0006(void)
{
	CSUDI_HANDLE hTestFlash1, hTestFlash2;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIOSThreadCreate("WriteFlash1", 17*10, 4096, (CSUDIOSThreadEntry_F)RandWriteFlashThread1, NULL, &hTestFlash1), 
	"�����߳�1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("WriteFlash2", 17*10, 4096, (CSUDIOSThreadEntry_F)RandWriteFlashThread2, NULL, &hTestFlash2),
	"�����߳�2ʧ��");

	while (!RandThread1Done || !RandThread2Done)
	{
		CSUDIOSThreadSleep(1000);
	}

	CSTK_FATAL_POINT;

	if (hTestFlash1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hTestFlash1), "ɾ������1ʧ��\n");
		hTestFlash1 = CSUDI_NULL;
	}

	if (hTestFlash2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hTestFlash2), "ɾ������1ʧ��\n");
		hTestFlash2 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//ͨ����ȡ�����ļ���ѯ�ɹ����Ե�Flash����
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
			CSTCPrint("[%s] ������ɵĵ�ַ����dwTestAddr=0x%x, dwTestSize=0x%x\n", __FUNCTION__,dwTestAddr, dwTestSize);
			s_bFlashTaskFlag = FALSE;
			goto exit;
		}

		memset(pDatabuf, testcode, dwTestSize);

		eErrCode = CSUDIFlashWrite(dwTestAddr, pDatabuf, dwTestSize);
		if(eErrCode != CSUDI_SUCCESS)
		{
			CSTCPrint("[%s] дflashʧ��dwTestAddr = 0x%x, dwTestSize = 0x%x\n", __FUNCTION__,dwTestAddr, dwTestSize);
			s_bFlashTaskFlag = FALSE;
			goto exit;
		}

		memset(pDatabuf, 0, dwTestSize);
		eErrCode = CSUDIFlashRead(dwTestAddr, pDatabuf, dwTestSize);
		if(eErrCode != CSUDI_SUCCESS)
		{
			CSTCPrint("[%s] ��flash ʧ��!dwTestAddr = 0x%x, dwTestSize = 0x%x\n", __FUNCTION__, dwTestAddr, dwTestSize);
			s_bFlashTaskFlag = FALSE;
			goto exit;
		}

		for(i=0; i<dwTestSize; i++)
		{
			if(pDatabuf[i] != testcode)
			{
				CSTCPrint("[%s] У������ʧ��!testcode = 0x%x, readdata = 0x%x\n", __FUNCTION__, testcode, pDatabuf[i]);
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
		CSTCPrint("��ȡ�豸idʧ��\n");
		s_bLocalFsTaskFlag = FALSE;
		goto exit;
	}

	//������Ϊû�пɲ����豸����������ͨ��
	if(nActIdCnt < 1)
	{
		CSTCPrint("��ȡ�豸����ʧ��\n");
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
			CSTCPrint("û�й̶��豸�ɲ���\n");
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
				 	CSTCPrint("��ȡ�־���Ϣʧ��\n");
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
						CSTCPrint("���������ļ�ʧ��\n");
						s_bLocalFsTaskFlag = FALSE;
				 		goto exit;
					}
					
					if (nWriteSize !=  CSUDIFSWrite(hFile, (char*)pDataBuf, nWriteSize))
					{
						CSTCPrint("д���ļ�ʧ��\n");
						s_bLocalFsTaskFlag = FALSE;
						CSUDIFSClose(hFile); 
				 		goto exit;
					}
					
					CSUDIFSClose(hFile);

					memset(pDataBuf,0,nWriteSize);
					hFile = CSUDIFSOpen(szFilePath, "r");
					if (NULL == hFile)
					{
						CSTCPrint("�������򿪶��ļ�ʧ��\n");
						s_bLocalFsTaskFlag = FALSE;
				 		goto exit;
					}
					if (nWriteSize != CSUDIFSRead(hFile, (char*)pDataBuf, nWriteSize))
					{
						CSTCPrint("��ȡ����ʧ��\n");
						s_bLocalFsTaskFlag = FALSE;
						CSUDIFSClose(hFile); 
				 		goto exit;
				 	}

					for(i=0; i<nWriteSize; i++)
					{
						if (pDataBuf[i] != testcode)
						{
							CSTCPrint("��ȡ��������д��Ĳ�һ��\n");
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
//@DESCRIPTION: ���ԶԱ����ļ�ϵͳ���ж�д������ͬʱ����Flash����ͬʱ��Flash���ж�д����
//@PRECONDITION:FS��FLASHģ��֮ǰ�ѳɹ���ʼ��
//@PRECONDITION: ϵͳ�д��ڷ־��豸
//@INPUT: pcPathName = ����·�� + "LocalFsWriteRead_0001"
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIFSGetAllDeviceId()�õ������ѹ��صķ־��豸id	
//@EXECUTIONFLOW:2�����ú��ж��豸�Ƿ�Ϊ���ƶ��豸
//@EXECUTIONFLOW:3��������ǿ��ƶ��豸����CSUDIFSGetDeviceInfo��÷־�Ĺ���·��
//@EXECUTIONFLOW:4������CSUDIFSStatfs��ȡ�־���Ϣ���ж��ļ�ϵͳ����
//@EXECUTIONFLOW:5������ļ�ϵͳ��ֻ�����������CSUDIFSOpen(szFilePath, "r"))�򿪵�ǰ·���ļ�,Ҫ��ɹ�
//@EXECUTIONFLOW:6������CSUDIFSRead��ȡ�ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������CSUDIFSClose�ر��ļ���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:8������ļ�ϵͳ�ǿɶ�д���������CSUDIFSOpen(szFilePath, "w+"\r\n ))�������򿪵�ǰ·���ļ�,Ҫ��ɹ�
//@EXECUTIONFLOW:9������CSUDIFSWrite(hFile, szTest, sizeof(szTest))���ļ���д���ַ�����Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10������CSUDIFSClose(hFile)�ر��ļ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:11������CSUDIFSOpen(szFilePath, "r"))���ļ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:12������CSUDIFSRead(hFile, szBuf, sizeof(szTest))�����ַ�������д��ıȽ�Ҫ��һ��
//@EXECUTIONFLOW:13������CSUDIFSClose(hFile)�ر��ļ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:14�������з־��豸�ظ�����2~13
CSUDI_BOOL CSTC_FLASH_IT_Performance_0007(void)
{

	void *hFlashTestTask = NULL;
	void * hLocalFsWriteReadTask = NULL;
	char *pFlashDatabuf = NULL;
	char *pLocalFsDatabuf = NULL;
	unsigned int testcnt = 36000; /* 10 hours*/
	unsigned int testsleeptime = 1000; /* 1000ms*/
	unsigned int cnt = testcnt; /* 10 hours*/

	CSTCPrint("[UDI2FSTEST]:��������Ҫ10 Сʱ�\r\n");

	GetFlashTestArea(&s_dwTestAreaStart, &s_dwTestTestAreaSize);

	CSTCPrint("Flash ��д������ʼ��ַ:0x%x\n", s_dwTestAreaStart);
	CSTCPrint("Flash ��д���������С:0x%x\n", s_dwTestTestAreaSize);
	
	pFlashDatabuf = CSUDIOSMalloc(DATA_BUF_SIZE);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pFlashDatabuf,"�����ڴ�ʧ��");

	pLocalFsDatabuf = CSUDIOSMalloc(DATA_BUF_SIZE);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pLocalFsDatabuf,"�����ڴ�ʧ��");

	s_bFlashTaskRun = TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FSFlashTestTask", 4*17, 1024*64, FSFlashTestTask, (void*)pFlashDatabuf, &hFlashTestTask),
	"����FSFlashTestTaskʧ��");

	s_bLocalFsWriteReadTaskRun = TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FSLocalFsWriteReadTask", 4*17, 1024*64, FSLocalFsWriteReadTask, (void*)pLocalFsDatabuf, &hFlashTestTask),
	"����FSLocalFsWriteReadTaskʧ��");

	while((cnt > 0) && s_bFlashTaskFlag && s_bLocalFsTaskFlag)
	{
		CSUDIOSThreadSleep(testsleeptime);
		cnt--;
	}

	if(cnt <= 0)
	{
		CSTCPrint("�������!!!!\n");
		CSTCPrint("����ʱ��Ϊ%dS,δ���ִ���!!!!\n", testsleeptime*testcnt/1000);
	}

	CSTK_ASSERT_TRUE_FATAL(s_bFlashTaskFlag, "Flash��д�̲߳���ʧ��");
	CSTK_ASSERT_TRUE_FATAL(s_bLocalFsTaskFlag, "�ļ�ϵͳ��д�̲߳���ʧ��");

	CSTK_FATAL_POINT;

	/* ���ñ�ǲ��ȴ��߳��˳� */
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
//��ע ����CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0001
//         ~CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0004
//�ܷ�ʱ,�����������дһ��byte��ʱ��Ϊ20ms(�����cfi flash��)����ô����
//һ��4MBytes��С��flash,��Ҫ23Сʱ��,�ʽ����Գ��ȸ�Ϊ1MBytes

//@CASEGROUP:���һ��block�ĵ��ֽ�дǿ�Ȳ���
//@DESCRIPTION:��flash���ֽ�д��0xaa,ÿдһ�ζ�����������У��
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ������Ƭflash��������
//@EXPECTATION:����д������������CSHDI_SUCCESS������������д�������һ��
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0001(void)
{
	BYTE ucWriteData = 0xaa,ucReadData = 0;
	unsigned int ii,kk=0;
	DWORD dwCurAddr = g_uTestStartAddress;
	DWORD dwLength = 1;
	DWORD dwTestSize = g_uTestBlockSize;

	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= g_uTestBlockSize, "�û��ɲ��Կռ�̫С");
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

			// ÿ��1KB�ʹ�ӡһ��
			if(((ii << 22) == 0))
			{
				CSTCPrint("%3d(KB)/%d(KB) test OK!\n", (ii>>10)+1, g_uTestBlockSize>>10);
			}
		}
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);

	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:���һ��block�Ķ��ֽ�дǿ�Ȳ���
//@DESCRIPTION:��flash���д�����ֽ�0xaa,ÿдһ�ζ�����������У��
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ������Ƭflash��������
//@EXPECTATION:����д������������CSHDI_SUCCESS������������д�������һ��
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0002(void)
{
	BYTE ucWriteData[10] = {0},ucReadData[10] = {0};
	int ii,kk=0;
	DWORD dwCurAddr = 0;
	DWORD dwLength = 2;
	DWORD dwTestSize = g_uTestBlockSize;
	
	dwCurAddr = g_uTestStartAddress+g_uTestBlockSize;
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (2*g_uTestBlockSize), "�û��ɲ��Կռ�̫С");
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

			// ÿ��1KB�ʹ�ӡһ��
			if(((ii << 22) == 0))
			{
				CSTCPrint("%3d(KB)/%d(KB) test OK!\n", (ii>>10)+1, g_uTestBlockSize>>10);
			}
		}
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);

	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:���һ��block�����ֽ�дǿ�Ȳ���
//@DESCRIPTION:��flash���д�����ֽ�0xaa,ÿдһ�ζ�����������У��
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ������Ƭflash��������
//@EXPECTATION:����д������������CSHDI_SUCCESS������������д�������һ��
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0003(void)
{
	BYTE ucWriteData[10] = {0},ucReadData[10] = {0};
	int ii,kk=0;
	DWORD dwCurAddr = 0;
	DWORD dwLength = 3;
	DWORD dwTestSize = g_uTestBlockSize;
	
	dwCurAddr = g_uTestStartAddress+2*g_uTestBlockSize;
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (3*g_uTestBlockSize), "�û��ɲ��Կռ�̫С");
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

			// ÿ��1KB�ʹ�ӡһ�Σ���Ҫ���Ǳ�3����������
			if(((ii << 22) == 0) || ((((ii-1) << 22) == 0)) || ((((ii-2) << 22) == 0)))
			{
				CSTCPrint("%3d(KB)/%d(KB) test OK!\n", (ii>>10)+1, g_uTestBlockSize>>10);
			}
		}
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);

	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:���һ��block�����ֽ�дǿ�Ȳ���
//@DESCRIPTION:��flash���д�����ֽ�0xaa,ÿдһ�ζ�����������У��
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ������Ƭflash��������
//@EXPECTATION:����д������������CSHDI_SUCCESS������������д�������һ��
CSUDI_BOOL CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0004(void)
{
	BYTE ucWriteData[10] = {0},ucReadData[10] = {0};
	int ii,kk=0;
	DWORD dwCurAddr = 0;
	DWORD dwLength = 4;
	DWORD dwTestSize = g_uTestBlockSize;
	
	dwCurAddr = g_uTestStartAddress+3*g_uTestBlockSize;

	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (4*g_uTestBlockSize), "�û��ɲ��Կռ�̫С");
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

			// ÿ��1KB�ʹ�ӡһ��
			if(((ii << 22) == 0))
			{
				CSTCPrint("%3d(KB)/%d(KB) test OK!\n", (ii>>10)+1, g_uTestBlockSize>>10);
			}
		}
	}while(kk<FLASH_FEW_BYTES_TEST_CNT);
	
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}
//@CASEGROUP:���һ��block�ĵ��ֽڶ�ǿ�Ȳ���
//@DESCRIPTION:��flashд��0-255������,ÿ�ζ�һ���ֽڽ���У������ظ����
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ��������block �Ĳ�������
//@EXPECTATION:����д������������CSHDI_SUCCESS������������д�������һ��
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucWriteData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (5*g_uTestBlockSize), "�û��ɲ��Կռ�̫С");

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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucWriteData),"������Դʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:���һ��block�Ķ��ֽڶ�ǿ�Ȳ���
//@DESCRIPTION:��flashд��0-255������,ÿ�ζ������ֽڽ���У������ظ����
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ��������block �Ĳ�������
//@EXPECTATION:����д������������CSHDI_SUCCESS������������д�������һ��
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucWriteData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (6*g_uTestBlockSize), "�û��ɲ��Կռ�̫С");

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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucWriteData),"������Դʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:���һ��block�����ֽڶ�ǿ�Ȳ���
//@DESCRIPTION:��flashд��0-255������,ÿ�ζ������ֽڽ���У������ظ����
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ��������block �Ĳ�������
//@EXPECTATION:����д������������CSHDI_SUCCESS������������д�������һ��
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucWriteData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (7*g_uTestBlockSize), "�û��ɲ��Կռ�̫С");

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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucWriteData),"������Դʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:���һ��block�����ֽڶ�ǿ�Ȳ���
//@DESCRIPTION:��flashд��0-255������,ÿ�ζ��ĸ��ֽڽ���У������ظ����
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ��������block �Ĳ�������
//@EXPECTATION:����д������������CSHDI_SUCCESS������������д�������һ��
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pucWriteData,"�����ڴ�ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (8*g_uTestBlockSize), "�û��ɲ��Կռ�̫С");

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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSFree(pucWriteData),"������Դʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:�������block���п�block��д����
//@DESCRIPTION:���һ��block����������ֽں����ڵڶ���block��ǰ�����ֽ�д�����ݣ��ж϶�д�����Ƿ�һ��
//@PRECONDITION:Flash��ʼ���ɹ�����д��ǰ��������block �Ĳ������������Կռ�����Ϊ����block
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDIFlashEraseSpace()��������block�ռ����ݣ������������سɹ�
//@EXECUTIONFLOW:2������CSUDIFlashWriteWithoutErase��������������block�е�һ��block��������ֽں͵ڶ���block��ǰ�����ֽ�д������
//@EXECUTIONFLOW:3������CSUDIFlashRead��������2��д����ĸ��ֽڵ����ݣ�Ҫ���ȡ���ݳɹ�
//@EXECUTIONFLOW:4��������3�������ĸ��ֽ������벽��2��д����ĸ��ֽ�����ֵ���жԱȣ�����������ֵ��д���һ��
CSUDI_BOOL CSTC_FLASH_IT_FlashNeighborBlockWriteNoErase(void)
{
	BYTE ucWriteData[4],ucReadData[4];
	int ii=0;
	DWORD dwStartAddr = g_uTestStartAddress;
	
	CSTK_ASSERT_TRUE_FATAL(g_uTestFlashSize >= (2*g_uTestBlockSize), "�û��ɲ��Կռ�̫С");

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

