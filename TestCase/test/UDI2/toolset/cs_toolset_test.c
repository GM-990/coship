/**
   *2009/09/24  设置配置参数
 **/
 
/*Include head files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cs_testkit.h"
#include "cs_toolset_test.h"
#include "udi2_error.h"

//主芯片的ID的最大长度
#define  CSUDI_CHIPID_MAX_LENGTH   (32) 
 
//系统序列号的最大长度  
#define  CSUDI_SERIAL_MAX_LENGTH   (64) 

//校验密钥的最大长度  
//#define  CSUDI_KEY_MAX_LENGTH   (128) 

 //芯片支持获取ChipID否
CSUDI_BOOL g_bSupportGetChipID;

//主芯片的ID信息
CSUDICHIPID_S g_sCHIPID;

//机器的序列号和Key值
CSUDISerialKey_S g_sSerialKey;

//系统支持不支持halt功能
CSUDI_BOOL g_bSupportSysHalt;

void CSTC_TOOLSET_InitCfg()
{
	char cBuf[CSUDI_KEY_MAX_LENGTH];

	//平台支持支持获取ChipID否
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("TOOLSET", "CSUDI_TOOLSET_SUPPORT_GET_CHIPID", cBuf, 2);
	if(cBuf[0] == '1')
	{
		g_bSupportGetChipID = CSUDI_TRUE;
	}

	//主芯片的ID信息
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

	//机器的序列号和Key值信息
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


