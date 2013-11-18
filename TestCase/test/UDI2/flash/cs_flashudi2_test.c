/**
   *@version 1.0.0 2009/09/24  ����flashģ������ò���
**/
/*Include head files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs_testkit.h"
#include "cs_flashudi2_test.h"
#include "udi2_error.h"
#include "cs_flash.h"

//flash�ռ����ʼ��ַ
CSUDI_UINT32 g_uStartAddress = 0;

//�û����ÿռ����ʼ��ַ
CSUDI_UINT32 g_uTestStartAddress = 0;

//�û����ÿռ���ܴ�С
unsigned int g_uTestFlashSize = 0;

//�û����õ�����Block��С
unsigned int g_uTestBlockSize = 0;

//flash����������Ϣ
unsigned int  g_uALLRegionCount = 0;
unsigned int  g_uALLRegionSize = 0;
unsigned int  g_uALLBlockCount = 0;
unsigned int  g_uALLMaxBlockSize = 0;

/*
*��ȡ�����ļ���Ϣ,��ʼ��flash�е���ز���
*/
void CSTC_FLASH_InitCfg()
{
	char cBuf[64];
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_START_ADDRESS", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_uStartAddress = CSTKGetIntFromStr(cBuf, 16);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_TEST_START_ADDRESS", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_uTestStartAddress = CSTKGetIntFromStr(cBuf, 16);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_TEST_FLASH_SIZE", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_uTestFlashSize = CSTKGetIntFromStr(cBuf, 16);
	}

	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_TEST_BLOCK_MAX_SIZE", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_uTestBlockSize = CSTKGetIntFromStr(cBuf, 16);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_ALL_REGION_COUNT", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_uALLRegionCount = CSTKGetIntFromStr(cBuf, 10);
	}

	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_ALL_REGION_SIZE", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_uALLRegionSize = CSTKGetIntFromStr(cBuf, 16);
	}

	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_ALL_REGION_BLOCK_COUNT", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_uALLBlockCount = CSTKGetIntFromStr(cBuf, 10);
	}

	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_ALL_REGION_BLOCK_SIZE", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_uALLMaxBlockSize = CSTKGetIntFromStr(cBuf, 16);
	}
	
}

