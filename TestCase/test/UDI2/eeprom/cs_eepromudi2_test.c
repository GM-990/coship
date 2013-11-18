/**
   *@version 1.0.0 2009/09/24 �޸����ò���Ϊ
**/
/*Include head files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cs_testkit.h"
#include "cs_eepromudi2_test.h"
#include "udi2_error.h"


/*
*��ȡ�����ļ���Ϣ,��ʼ��eeprom ��ز���
*/

//eeprom����ʼ��ַ 
CSUDI_UINT32 g_dwStartAddress;

//eeprom�ռ��С 
int g_nEepromSize;

void CSTC_EEPROM_InitCfg()
{
	char cBuf[32];
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("EEPROM", "CS_EEPROMUDI2_START_ADDRESS", cBuf, sizeof(cBuf));
	//printf("30************%s******************\n",cBuf);	
	if(cBuf[0] != '\0')
	{
		g_dwStartAddress = CSTKGetIntFromStr(cBuf, 16);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("EEPROM", "CS_EEPROMUDI2_SIZE", cBuf, sizeof(cBuf));
	//printf("30************%s******************\n",cBuf);
	if(cBuf[0] != '\0')
	{
		g_nEepromSize  = CSTKGetIntFromStr(cBuf, 16);
	}
	
}


