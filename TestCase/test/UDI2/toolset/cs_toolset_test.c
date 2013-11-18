/**
   *2009/09/24  �������ò���
 **/
 
/*Include head files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cs_testkit.h"
#include "cs_toolset_test.h"
#include "udi2_error.h"

//��оƬ��ID����󳤶�
#define  CSUDI_CHIPID_MAX_LENGTH   (32) 
 
//ϵͳ���кŵ���󳤶�  
#define  CSUDI_SERIAL_MAX_LENGTH   (64) 

//У����Կ����󳤶�  
//#define  CSUDI_KEY_MAX_LENGTH   (128) 

 //оƬ֧�ֻ�ȡChipID��
CSUDI_BOOL g_bSupportGetChipID;

//��оƬ��ID��Ϣ
CSUDICHIPID_S g_sCHIPID;

//���������кź�Keyֵ
CSUDISerialKey_S g_sSerialKey;

//ϵͳ֧�ֲ�֧��halt����
CSUDI_BOOL g_bSupportSysHalt;

void CSTC_TOOLSET_InitCfg()
{
	char cBuf[CSUDI_KEY_MAX_LENGTH];

	//ƽ̨֧��֧�ֻ�ȡChipID��
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("TOOLSET", "CSUDI_TOOLSET_SUPPORT_GET_CHIPID", cBuf, 2);
	if(cBuf[0] == '1')
	{
		g_bSupportGetChipID = CSUDI_TRUE;
	}

	//��оƬ��ID��Ϣ
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("TOOLSET", "CSUDI_TOOLSET_CHIPID_LENGTH", cBuf, 4);
	if(cBuf[0] != '\0')
	{
		g_sCHIPID.m_nLength = CSTKGetIntFromStr(cBuf, 10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("TOOLSET", "CSUDI_TOOLSET_CHIPID_TEXT", cBuf, CSUDI_CHIPID_MAX_LENGTH);
	if(cBuf[0] != '\0')
	{
		memcpy(g_sCHIPID.m_szChipID,cBuf,g_sCHIPID.m_nLength);
	}

	//���������кź�Keyֵ��Ϣ
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("TOOLSET", "CSUDI_TOOLSET_SERIAL", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		memcpy(g_sSerialKey.m_szSerial,cBuf,strlen(cBuf));
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("TOOLSET", "CSUDI_TOOLSET_KEY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		memcpy(g_sSerialKey.m_szPairsKey,cBuf,strlen(cBuf));

	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("TOOLSET", "CSUDI_TOOLSET_SUPPORT_HALT", cBuf, sizeof(cBuf));
	if(cBuf[0] != '0')
	{
		g_bSupportSysHalt = CSUDI_TRUE;
	}
}


