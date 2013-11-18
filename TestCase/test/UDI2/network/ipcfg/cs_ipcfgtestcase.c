/*
 *@version 1.0.0 2009/08/13 完成初始版本
 */
/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：CSTC_测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_ipcfgtestcase.h"
#include "udi2_ipcfg.h"
#include "cs_testkit.h"
#include "udi2_os.h"
#include "udi2_socket.h"

static char g_szDNSIp[16] = {0};
static CSUDIWLanConfig_S s_asLanCfg[10];//配置文件中有线网卡名称
static CSUDIWLanConfig_S s_asWLanCfg[10];//配置文件中无线网卡名称
static int s_nActLanCount = 0;//配置文件中有线网卡数量
static int s_nActWLanCount = 0;//配置文件中无线网卡数量

static CSUDI_BOOL IPCFG_iInitCfg()
{
	CSUDI_BOOL bRet = CSUDI_TRUE;
	char cIpCfgBuf[32];
	char cTestParam[64];
	int i;
	CSTKCfgError_E cfgRe;

	CSTCPrint("=============SetupIpcfg===========\n");
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("IPCFG","DNS_IP",g_szDNSIp,sizeof(g_szDNSIp)))
	{
		CSTCPrint("DNS IP is %s\n", g_szDNSIp);
	}
	else
	{
		bRet = CSUDI_FALSE;
		CSTCPrint("get [IPCFG] DNS_IP failed\n");
	}
	
	memset(cIpCfgBuf,'\0',sizeof(cIpCfgBuf));
	if(CSTKGetConfigInfo("IPCFG","CS_IPCFG_LAN_COUNT ",cIpCfgBuf,sizeof(cIpCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_nActLanCount = CSTKGetIntFromStr(cIpCfgBuf, 10);
		CSTCPrint("lan count is: %d\n", s_nActLanCount);
	}
	else
	{
		bRet = CSUDI_FALSE;
		CSTCPrint("get [IPCFG] CS_IPCFG_LAN_COUNT failed\n");
	}

	memset(cIpCfgBuf,'\0',sizeof(cIpCfgBuf));
	if(CSTKGetConfigInfo("IPCFG","CS_IPCFG_WLAN_COUNT ",cIpCfgBuf,sizeof(cIpCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_nActWLanCount = CSTKGetIntFromStr(cIpCfgBuf, 10);
		CSTCPrint("wlan count is: %d\n", s_nActWLanCount);
	}
	else
	{
		bRet = CSUDI_FALSE;
		CSTCPrint("get [IPCFG] CS_IPCFG_WLAN_COUNT failed\n");
	}

	memset(s_asLanCfg,0,sizeof(s_asLanCfg)/sizeof(s_asLanCfg[0]));
	for(i=0 ; i<s_nActLanCount ; i++)
	{
		memset(s_asLanCfg[i].szEthName,'\0',sizeof(s_asLanCfg[i].szEthName));
		memset(cTestParam,'\0',sizeof(cTestParam));
		sprintf(cTestParam,"%s%d%s","CS_LAN",i,"_NAME");
		cfgRe = CSTKGetConfigInfo("IPCFG",cTestParam,s_asLanCfg[i].szEthName,sizeof(s_asLanCfg[i].szEthName));
		if(cfgRe != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			CSTCPrint("get [IPCFG] CS_LAN%d_NAME failed\n", i);
		}
		else
		{
			CSTCPrint("lan%d name is: %s\n", i, s_asLanCfg[i].szEthName);
		}
	}
	
	memset(s_asWLanCfg,0,sizeof(s_asWLanCfg)/sizeof(s_asWLanCfg[0]));
	for(i=0 ; i<s_nActWLanCount ; i++)
	{
		memset(s_asWLanCfg[i].szEthName,'\0',sizeof(s_asWLanCfg[i].szEthName));
		memset(cTestParam,'\0',sizeof(cTestParam));
		sprintf(cTestParam,"%s%d%s","CS_WLAN",i,"_NAME");
		cfgRe = CSTKGetConfigInfo("IPCFG",cTestParam,s_asWLanCfg[i].szEthName,sizeof(s_asWLanCfg[i].szEthName));
		if(cfgRe != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			CSTCPrint("get [IPCFG] CS_WLAN%d_NAME failed\n", i);
		}
		else
		{
			CSTCPrint("wlan%d name is: %s\n", i, s_asWLanCfg[i].szEthName);
		}
	}
	
	return bRet;
}

CSUDI_BOOL CSTC_IPCFG_Init(void)
{
	//在本测试用例集执行前调用
	return IPCFG_iInitCfg();
}

CSUDI_BOOL CSTC_IPCFG_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}


//**********************************************************************************//
//******************************Begin Test  GetEtherCount*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetEtherCount
//@DESCRIPTION:成功获得网卡数
//@PRECONDITION: 协议栈已经初始化成功
//@EXPECTATION:  成功返回系统配置的网卡数
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherCount(void)操作，返回系统配置的网卡数
//@EXECUTIONFLOW: 2、比较获取的网卡数目和配置项是否一致
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherCount_0001(void)
{
	int nCount = 0 ;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "步骤1失败");
	CSTCPrint("网卡数:%d\n", nCount);
	CSTK_ASSERT_TRUE_FATAL(nCount == (s_nActLanCount+s_nActWLanCount), "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End Test GetEtherCount*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test  GetEtherName*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:成功获取存在的所有网卡设备名
//@PRECONDITION: 1、协议栈已经初始化成功
//@INPUT:1、nEthIndex = i
//@INPUT:2、szEthName[16]，用来存放获取的设备名
//@INPUT:3、nLen = sizeof(szEthName)
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherCount(&nCount)操作，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetEtherName(i, szEthName, nLen)操作，成功返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0001(void)
{
	int i = 0;
	char szEthName[16];
	int nCount = 0;
	int nLen = sizeof(szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "步骤1失败");
	CSTCPrint("网卡数:%d\n", nCount);

	for(i=0; i<nCount; i++)
	{
		memset(szEthName, 0, nLen);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherName(i, szEthName, nLen), "步骤2失败");
		CSTCPrint("网卡%d名为:%s\n", i, szEthName);
	}
		
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:索引号为非法值nCount时获取网卡设备名失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@INPUT:1、nEthIndex = nCount
//@INPUT:2、szEthName[16]，用来存放获取的设备名
//@INPUT:3、nLen = sizeof(szEthName)
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherCount(&nCount)操作，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetEtherName(nEthIndex,szEthName,nLen)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0002(void)
{
	char szEthName[16];
	int nCount = 0;
	int nLen = sizeof(szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "步骤1失败");
	memset(szEthName, 0, nLen);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherName(nCount, szEthName, nLen), "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:指针为非法值空时获取网卡设备名失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、设备索引号为0的网卡存在
//@INPUT:1、nEthIndex = 0
//@INPUT:2、pcEthName = CSUDI_NULL，指针置为空
//@INPUT:3、nLen = sizeof(szEthName)
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherCount(&nCount)操作，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetEtherName(nCount-1, CSUDI_NULL, 16)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0003(void)
{
	int nCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "步骤1失败");
	if(nCount > 0)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherName(0, CSUDI_NULL, 16), "步骤2失败");
	}	
	else
	{
		CSTCPrint("没有找到网卡\n");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:存储设备名缓存为0时获取网卡设备名失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、设备索引号为0的网卡存在
//@INPUT:1、nEthIndex = 0
//@INPUT:2、szEthName[16]，用来存放获取的设备名
//@INPUT:3、nLen = 0
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherCount(&nCount)操作，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetEtherName(0, szEthName, 0)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0004(void)
{
	int nCount = 0;
	char szEthName[16];
	int nLen = sizeof(szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "步骤1失败");
	if(nCount > 0)
	{
		memset(szEthName, 0, nLen);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherName(0, szEthName, 0), "步骤2失败");
	}	
	else
	{
		CSTCPrint("没有找到网卡\n");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:存储设备名缓存超过16字节时获取网卡设备名失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、设备索引号为0的网卡存在
//@INPUT:1、nEthIndex = 0
//@INPUT:2、szEthName[17]，用来存放获取的设备名
//@INPUT:3、nLen = sizeof(szEthName)
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherCount(&nCount)操作，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetEtherName(0, szEthName, 17)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0005(void)
{
	int nCount = 0;
	char szEthName[17];
	int nLen = sizeof(szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "步骤1失败");
	if(nCount > 0)
	{
		memset(szEthName, 0, nLen);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherName(0, szEthName, 17), "步骤2失败");
	}	
	else
	{
		CSTCPrint("没有找到网卡\n");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End Test GetEtherName*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test  GetEtherChipInfo*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:成功获得所有网卡芯片的信息
//@PRECONDITION: 1、协议栈已经初始化成功
//@INPUT:1、配置文件中存在的各个网卡名
//@INPUT:2、sChipInfo，存放网卡芯片信息的结构体
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherCount(&nCount)操作，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、比较获取的网卡数目和配置项是否一致
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGGetEtherChipInfo(s_asLanCfg[i].szEthName, &sChipInfo)操作，成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、执行CSUDIIPCFGGetEtherChipInfo(s_asLanCfg[i].szEthName, &sChipInfo)操作，成功返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0001(void)
{
	int i = 0;
	int nCount = 0;
	CSUDIEtherInfo_S sChipInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(nCount == (s_nActLanCount+s_nActWLanCount), "步骤2失败");
	for(i=0; i<s_nActLanCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS== CSUDIIPCFGGetEtherChipInfo(s_asLanCfg[i].szEthName, &sChipInfo), "步骤3失败");
	}
	for(i=0; i<s_nActWLanCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS== CSUDIIPCFGGetEtherChipInfo(s_asWLanCfg[i].szEthName, &sChipInfo), "步骤4失败");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:网卡名为非法值"eth5"时获取芯片信息失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中没有网卡名"eth5"的网卡
//@INPUT:1、szEthName[5] = "eth5"
//@INPUT:2、sChipInfo，存放网卡芯片信息的结构体
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherChipInfo(szEthName,&sChipInfo)操作
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0002(void)
{
	CSUDIEtherInfo_S sChipInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherChipInfo("eth5", &sChipInfo), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:网卡名为非法值"Eth0"时获取芯片信息失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中没有网卡名"Eth0"的网卡
//@INPUT:1、szEthName[5] = "Eth0"
//@INPUT:2、sChipInfo，存放网卡芯片信息的结构体
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherChipInfo(szEthName,&sChipInfo)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0003(void)
{
	CSUDIEtherInfo_S sChipInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherChipInfo("Eth0", &sChipInfo), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:结构体为非法值空时获取芯片信息失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中有有线网卡
//@INPUT:1、szEthName[5] = CS_LAN0_NAME
//@INPUT:2、pcChipInfo = CSUDI_NULL
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherChipInfo(szEthName,pcChipInfo)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0004(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherChipInfo(szEthName, CSUDI_NULL), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:网卡名为非法值空时获取芯片信息失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@INPUT:1、pcEthName = CSUDI_NULL，网卡设备名为空
//@INPUT:2、sChipInfo，存放网卡芯片信息的结构体
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherChipInfo(pcEthName,&sChipInfo)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0005(void)
{
	CSUDIEtherInfo_S sChipInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherChipInfo(CSUDI_NULL, &sChipInfo), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}
//**********************************************************************************//
//******************************End Test  GetEtherChipInfo*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test  GetMacByName*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:成功获得网卡名为CS_LAN0_NAME芯片的物理地址
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中存在有线网卡
//@INPUT:1、szEthName[5] = CS_LAN0_NAME
//@INPUT:2、szMacAddress[8]，存放MAC地址
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetMacByName(szEthName,szMacAddress)操作， 成功返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0001(void)
{
	unsigned char szMacAddress[8] = {0};
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetMacByName(szEthName, szMacAddress), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:网卡名为非法值"Eth0"时获取芯片物理地址失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中不存在网卡名"Eth0"的网卡
//@INPUT:1、szEthName[5] = "Eth0"
//@INPUT:2、szMacAddress[8]，存放MAC地址
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetMacByName(szEthName,szMacAddress)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0002(void)
{
	unsigned char szMacAddress[8] = {0};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetMacByName("Eth0", szMacAddress), "步骤1失败");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:网卡名为非法值"eth5"时获取芯片物理地址失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中不存在网卡名"eth5"的网卡
//@INPUT:1、szEthName[5] = "eth5"
//@INPUT:2、szMacAddress[8]，存放MAC地址
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetMacByName(szEthName,szMacAddress)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0003(void)
{
	unsigned char szMacAddress[8] = {0};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetMacByName("eth5", szMacAddress), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:存储MAC地址指针为非法值空时获取芯片物理地址失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中存在有线网卡
//@INPUT:1、szEthName[5] = CS_LAN0_NAME
//@INPUT:2、pcMacAddress = CSUDI_NULL
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetMacByName(szEthName,pcMacAddress)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0004(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetMacByName(szEthName, CSUDI_NULL), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:网卡名为非法值空时获取芯片物理地址失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@INPUT:1、pcEthName = CSUDI_NULL
//@INPUT:2、szMacAddress[8]，存放MAC地址
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetMacByName(pcEthName,szMacAddress)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0005(void)
{
	unsigned char szMacAddress[8] = {0};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetMacByName(CSUDI_NULL, szMacAddress), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP: mac地址测试
//@DESCRIPTION:  网卡初始化成功，获取协议栈mac地址，不为FFFFFFFF,也不为多播地址
//@PRECONDITION: 协议栈初始化成功
//@EXECUTIONFLOW: 获取mac地址不为FFFFFFFF,也不为多播地址
//@EXECUTIONFLOW: 1、调用CSUDIIPCFGGetMacByName获取网卡mac地址
//@EXECUTIONFLOW: 2、将获取的mac地址和FFFFFF及全0地址相比，如果不为FFFFFF或全0，则返回成功，否则返回失败
//@EXECUTIONFLOW: 3、将获取的mac地址和多播地址相比，如果不为多播地址，则返回成功，否则返回失败
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0006(void)
{
	unsigned char acMacAddress[6] = {0};
	int nRet = -1;
	char szEthName[16] = {0};

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	nRet = CSUDIIPCFGGetMacByName(szEthName, acMacAddress);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "步骤1失败，获取mac地址失败");

	if ((acMacAddress[0]== 0xff)&(acMacAddress[1]== 0xff)&(acMacAddress[2]== 0xff)&(acMacAddress[3]== 0xff)&(acMacAddress[4]== 0xff)&(acMacAddress[5]== 0xff))
	{
		nRet = -1;
	}

	if ((acMacAddress[0]== 0x00)&(acMacAddress[1]== 0x00)&(acMacAddress[2]== 0x00)&(acMacAddress[3]== 0x00)&(acMacAddress[4]== 0x00)&(acMacAddress[5]== 0x00))
	{
		nRet = -1;
	}
	CSTK_ASSERT_TRUE_FATAL(nRet != -1, "步骤2失败，获取mac地址是广播地址或者全0地址");

	CSTK_ASSERT_TRUE_FATAL((acMacAddress[0] & 0x01) == 0x00, "步骤3失败，mac地址是多播地址");

	CSTK_FATAL_POINT   

	return CSUDI_TRUE; 
}

//**********************************************************************************//
//******************************End Test  GetMacByName*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test  GetEtherStatus*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:成功获取有线网卡名为CS_LAN0_NAME的连接状态
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中存在有线网卡
//@PRECONDITION: 3、机顶盒不带switch时插上网线
//@INPUT:1、szEthName[5] = CS_LAN0_NAME
//@INPUT:2、ushLinkState， 保存网卡的连接状态值
//@EXPECTATION:  成功返回CSUDI_SUCCESS，ushLinkState & CSUDI_ETHMASK_UP_DOWN=0
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherStatus(szEthName,&ushLinkState)操作
//@EXECUTIONFLOW: 2、执行ushLinkState & CSUDI_ETHMASK_UP_DOWN，判断连接状态
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0001(void)
{
	unsigned short ushLinkState = 255;
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherStatus(szEthName, &ushLinkState), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(0 == (ushLinkState & CSUDI_ETHMASK_UP_DOWN), "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:有线网卡名为非法值"Eth0"时获取连接状态失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中不存在网卡名"Eth0"的有线网卡
//@PRECONDITION: 3、机顶盒不带switch时插上网线
//@INPUT:1、szEthName[5] = "Eth0"
//@INPUT:2、ushLinkState， 保存网卡的连接状态值
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherStatus(szEthName,&ushLinkState)操作，失败返回错误码
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0002(void)
{
	unsigned short ushLinkState = 255;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherStatus("Eth0", &ushLinkState), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:有线网卡名为非法值"eth5"时获取连接状态失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中不存在网卡名"eth5"的有线网卡
//@PRECONDITION: 3、机顶盒不带switch时插上网线
//@INPUT:1、szEthName[5] = "eth5"
//@INPUT:2、ushLinkState， 保存网卡的连接状态值
//@EXPECTATION: 失败返回错误代码值
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherStatus(szEthName,&ushLinkState)操作
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0003(void)
{
	unsigned short ushLinkState = 255;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherStatus("eth5", &ushLinkState), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:机顶盒不带switch拔掉网线时获取连接状态成功
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中存在有线网卡
//@PRECONDITION: 3、机顶盒不带switch时拔掉网线
//@INPUT:1、szEthName[5] = CS_LAN0_NAME
//@INPUT:2、ushLinkState， 保存网卡的连接状态值
//@EXPECTATION:  成功返回CSUDI_SUCCESS，ushLinkState & CSUDI_ETHMASK_UP_DOWN=1
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherStatus(pcEthName,ushLinkState)操作
//@EXECUTIONFLOW: 2、将ushLinkState & CSUDI_ETHMASK_UP_DOWN，判断连接状态。连接断开，返回1
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0004(void)
{
	unsigned short ushLinkState = 255;
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTCPrint("请拔掉网线\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤0失败");
	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherStatus(szEthName, &ushLinkState), "步骤1失败");

	CSTCPrint("请插上网线\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(1 == (ushLinkState & CSUDI_ETHMASK_UP_DOWN), "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:有线网卡名为非法值空时获取连接状态失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中网卡名为空
//@PRECONDITION: 3、机顶盒不带switch时插上网线
//@INPUT:1、pcEthName = CSUDI_NULL，网卡设备名被置为空
//@INPUT:2、ushLinkState， 保存网卡的连接状态值
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherStatus(pcEthName,&ushLinkState)操作,失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0005(void)
{
	unsigned short ushLinkState = 255;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherStatus(CSUDI_NULL, &ushLinkState), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:存储连接状态值指针为非法值空时获取网卡的连接状态失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中存在有线网卡
//@PRECONDITION: 3、机顶盒不带switch时插上网线
//@INPUT:1、szEthName[5] = CS_LAN0_NAME
//@INPUT:2、pushLinkState = CSUDI_NULL， 保存网卡的连接状态指针为空
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetEtherStatus(szEthName,pushLinkState)操作,失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0006(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherStatus(szEthName, CSUDI_NULL), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End Test  GetEtherStatus*******************************//
//**********************************************************************************//

#ifdef PLATFORM_BCM7401
//**********************************************************************************//
//******************************Begin Test  SearchAP*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:AP信息结构体为非法值空时搜索AP信息失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中存在网卡名"eth0"的无线网卡
//@INPUT:1、szEthName[5] = "eth0"
//@INPUT:2、psWlanApInfo = CSUDI_NULL，存储AP信息结构体为CSUDI_NULL
//@INPUT:3、nWlanApInfoNum = 5，结构体数组的个数,置为5
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,psWlanApInfo, &nWlanApInfoNum)操作，失败返回错误码
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0001(void)
{   	
	int nWlanApInfoNum = 5;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP(szEthName,CSUDI_NULL,&nWlanApInfoNum),"步骤1 失败");
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:结构体数组个数为非法值0时搜索AP信息失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中存在网卡名"eth0"的无线网卡
//@INPUT:1、szEthName[5] = "eth0"
//@INPUT:2、sWlanApInfo，存储AP信息结构体
//@INPUT:3、nWlanApInfoNum = 0，结构体数组的个数,置为0
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)操作，失败返回错误码
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0002(void)
{   	
	int nWlanApInfoNum = 0;
	CSUDIWLanApInfo_S sWlanApInfo;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:网卡名称为非法值空时搜索AP信息失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@INPUT:1、pcEthName = CSUDI_NULL
//@INPUT:2、sWlanApInfo，存储AP信息结构体
//@INPUT:3、nWlanApInfoNum = 5
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)操作，失败返回错误码
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0003(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP(CSUDI_NULL,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:搜索AP信息成功
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、存在网卡名"eth0"的无线网卡
//@PRECONDITION: 3、实际存在的AP个数为5
//@INPUT:1、szEthName[5] = "eth0"
//@INPUT:2、sWlanApInfo，存储AP信息结构体
//@INPUT:3、nWlanApInfoNum = 5
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)操作
//@EXECUTIONFLOW: 2、判断nWlanApInfoNum的值，若nWlanApInfoNum ==5，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0004(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	CSTK_ASSERT_TRUE_FATAL(nWlanApInfoNum ==5,"步骤2 失败");
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:存储AP信息的结构体个数为5，网络中AP个数为6时，搜索得到AP的个数为5
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、存在网卡名"eth0"的无线网卡
//@PRECONDITION: 3、实际存在的AP个数为6
//@INPUT:1、szEthName[5] = "eth0"
//@INPUT:2、sWlanApInfo，存储AP信息结构体
//@INPUT:3、nWlanApInfoNum = 5
//@EXPECTATION:  成功返回CSUDI_SUCCESS，返回AP个数为5
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)操作
//@EXECUTIONFLOW: 2、判断nWlanApInfoNum的值，若nWlanApInfoNum ==5，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0005(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");
	CSTK_ASSERT_TRUE_FATAL(nWlanApInfoNum ==5,"步骤2 失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:存储AP信息的结构体个数为6，网络中AP个数为5时，搜索得到AP的个数为5
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、存在网卡名"eth0"的无线网卡
//@PRECONDITION: 3、实际存在的AP个数为5
//@INPUT:1、szEthName[5] = "eth0"
//@INPUT:2、sWlanApInfo，存储AP信息结构体
//@INPUT:3、nWlanApInfoNum = 6
//@EXPECTATION:  成功返回CSUDI_SUCCESS，返回AP个数为5
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)操作
//@EXECUTIONFLOW: 2、判断nWlanApInfoNum的值，若nWlanApInfoNum ==5，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0006(void)
{   	
	int nWlanApInfoNum = 6;
	CSUDIWLanApInfo_S sWlanApInfo;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");
	CSTK_ASSERT_TRUE_FATAL(nWlanApInfoNum ==5,"步骤2 失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:网卡名为非法值"Eth0"时搜索AP失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中不存在网卡名"Eth0"的无线网卡
//@PRECONDITION: 3、实际存在的AP个数为5
//@INPUT:1、szEthName[5] = "Eth0"
//@INPUT:2、sWlanApInfo，存储AP信息结构体
//@INPUT:3、nWlanApInfoNum = 5，结构体数组的个数,置为5
//@EXPECTATION: 
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)操作，失败返回错误码
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0007(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP("Eth0",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:网卡名为非法值"eth5"时搜索AP失败
//@PRECONDITION: 1、协议栈已经初始化成功
//@PRECONDITION: 2、协议栈中不存在网卡名"eth5"的无线网卡
//@PRECONDITION: 3、实际存在的AP个数为5
//@INPUT:1、szEthName[5] = "eth5"
//@INPUT:2、sWlanApInfo，存储AP信息结构体
//@INPUT:3、nWlanApInfoNum = 5，结构体数组的个数,置为5
//@EXPECTATION: 失败返回错误码
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)操作
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0008(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP("eth5",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}
//**********************************************************************************//
//******************************End Test  SearchAP*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test ConnectAP*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:网卡名为非法值空时与AP认证失败
//@PRECONDITION: 1、测试环境中至少存在一个AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName="";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(pcEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDIIPCFG_ERROR_CONNECTAP_ERROR
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0001(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memset(sWLanConf.szEthName ,0,sizeof(sWLanConf.szEthName));
	memcpy(sWLanConf.szEssId,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGConnectAP(&sWLanConf),"步骤2 失败");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:密钥类型为EM_UDIWLAN_KEY_HEX时与AP认证成功
//@PRECONDITION: 1、测试环境中至少存在一个密钥类型为EM_UDIWLAN_KEY_HEX 的AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: 成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0002(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,szEthName,sizeof(szEthName));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"步骤2 失败");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤3 失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:密钥类型为EM_UDIWLAN_KEY_ANSI时与AP认证成功
//@PRECONDITION: 1、测试环境中至少存在一个密钥类型为EM_UDIWLAN_KEY_ANSI的AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_ANSI;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: 成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0003(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,szEthName,sizeof(szEthName));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_ANSI;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"步骤2 失败");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤3 失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:密钥类型为EM_UDIWLAN_KEY_STRING时与AP认证成功
//@PRECONDITION: 1、测试环境中至少存在一个密钥类型为EM_UDIWLAN_KEY_STRING的AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_STRING;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: 成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0004(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,szEthName,sizeof(szEthName));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_STRING;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"步骤2 失败");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤3 失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:密钥错误导致与AP认证失败
//@PRECONDITION: 1、测试环境中至少存在一个AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@INPUT:sWlanApInfo，存储AP信息的结构体;
//@INPUT:nWlanApInfoNum = 5;
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szInvalidKey错误的密钥(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_STRING;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDIIPCFG_ERROR_CONNECTAP_ERROR
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0005(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,szEthName,sizeof(szEthName));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szInvalidKey,sizeof(szInvalidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_STRING;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
	CSTK_ASSERT_TRUE_FATAL(CSUDIIPCFG_ERROR_CONNECTAP_ERROR == nRetConnectAP,"步骤2 失败");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤3 失败");
	}
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:非所搜索到session id 导致与AP认证失败
//@PRECONDITION: 1、测试环境中至少存在一个AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = 0;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_STRING;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDIIPCFG_ERROR_CONNECTAP_ERROR
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0006(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memset(sWLanConf.szEssId ,0,sizeof(sWLanConf.szEssId));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_STRING;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"步骤2 失败");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤3 失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:非所搜索到AP的认证加密类型导致与AP认证失败
//@PRECONDITION: 1、测试环境中至少存在一个AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = 10(非搜索得到的加密类型);
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_STRING;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDIIPCFG_ERROR_CONNECTAP_ERROR
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0007(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = 10;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_STRING;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"步骤2 失败");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤3 失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:重复连接导致与AP认证失败
//@PRECONDITION: 1、测试环境中至少存在一个AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDIIPCFG_ERROR_CONNECTAP_ERROR
//@EXECUTIONFLOW: 4、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0008(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;
	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"步骤2 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGConnectAP(&sWLanConf),"步骤3 失败");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤4 失败");
	}
	return CSUDI_TRUE;
}
//**********************************************************************************//
//******************************End Test ConnectAP*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test DisconnectAP*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:无线网卡"eth1"成功与AP断开连接
//@PRECONDITION: 1、测试环境中至少存在一个AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: 成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_DisconnectAP_0001(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"步骤2 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤3 失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:无线网卡名为非法值空时与AP断开连接失败
//@PRECONDITION: 1、测试环境中至少存在一个AP
//@PRECONDITION: 2、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo，存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:pcEthName = CSUDI_NULL;
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGConnectAP(&sWLanConf)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、执行CSUDIIPCFGDisconnectAP(pcEthName)操作，返回CSUDI_FAILURE
CSUDI_BOOL CSTC_IPCFG_IT_DisconnectAP_0002(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"步骤2 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGDisconnectAP(CSUDI_NULL),"步骤3 失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}
//**********************************************************************************//
//******************************End Test DisconnectAP*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test GetAPSignalQulityStrength*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetAPSignalQulityStrength
//@DESCRIPTION:成功获取获取AP信号的质量和强度
//@PRECONDITION:1、测试环境中至少存在一个AP
//@PRECONDITION:2、无线网卡可以连接成功
//@PRECONDITION:3、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo,存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGGetAPSignalQulityStrength 函数的输入参数:
//@INPUT:nWlanApSigQulity = 0， 存储AP信号的质量
//@INPUT:nWlanApSigStrength = 0，存储AP信号的强度
//@INPUT:4、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、执行CSUDIIPCFGSearchAP(pcEthName,psWlanApInfo,nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、执行CSUDIIPCFGConnectAP(psWLanConf)操作,返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、执行CSUDIIPCFGGetAPSignalQulityStrength(&nWlanApSigQulity,&nWlanApSigStrength)操作,返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
//@REMARK: 测试环境:无线网卡与AP相隔10米，无线网卡与AP相隔50米，无线网卡与AP之间相隔一堵墙，无线网卡接入不同的AP，这些情况下测试得到的信号质量与强度是否相等
CSUDI_BOOL CSTC_IPCFG_IT_GetAPSignalQulityStrength_0001(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nWlanApSigQulity = 0;
	int nWlanApSigStrength = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"步骤2 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetAPSignalQulityStrength(&nWlanApSigQulity,&nWlanApSigStrength),"步骤3 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤4失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetAPSignalQulityStrength
//@DESCRIPTION:AP信号的质量和强度为非法指针时获取AP信号失败
//@PRECONDITION:1、测试环境中至少存在一个AP
//@PRECONDITION:2、无线网卡可以连接成功
//@PRECONDITION:3、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo,存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGGetAPSignalQulityStrength 函数的输入参数:
//@INPUT:pnWlanApSigQulity = CSUDI_NULL， 存储AP信号的质量
//@INPUT:nWlanApSigStrength = 0，存储AP信号的强度
//@INPUT:4、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW:1、执行CSUDIIPCFGSearchAP(pcEthName,psWlanApInfo,nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、执行CSUDIIPCFGConnectAP(psWLanConf)操作,返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、执行CSUDIIPCFGGetAPSignalQulityStrength(pnWlanApSigQulity,&nWlanApSigStrength)操作,返回错误码
//@EXECUTIONFLOW:4、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetAPSignalQulityStrength_0002(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nWlanApSigStrength = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"步骤2 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetAPSignalQulityStrength(CSUDI_NULL,&nWlanApSigStrength),"步骤3 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤4失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetAPSignalQulityStrength
//@DESCRIPTION:AP信号的质量和强度为非法指针时获取AP信号失败
//@PRECONDITION:1、测试环境中至少存在一个AP
//@PRECONDITION:2、无线网卡可以连接成功
//@PRECONDITION:3、存在名为"eth1"的无线网卡
//@INPUT:1、CSUDIIPCFGSearchAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo,存储AP信息的结构体
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2、CSUDIIPCFGConnectAP 函数的输入参数:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(参考配置文件);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3、CSUDIIPCFGGetAPSignalQulityStrength 函数的输入参数:
//@INPUT:nWlanApSigQulity = 0， 存储AP信号的质量
//@INPUT:pnWlanApSigStrength = CSUDI_NULL，存储AP信号的强度
//@INPUT:4、CSUDIIPCFGDisconnectAP 函数的输入参数:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW:1、执行CSUDIIPCFGSearchAP(pcEthName,psWlanApInfo,nWlanApInfoNum)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、执行CSUDIIPCFGConnectAP(psWLanConf)操作,返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、执行CSUDIIPCFGGetAPSignalQulityStrength(&nWlanApSigQulity,pnWlanApSigStrength)操作,返回错误码
//@EXECUTIONFLOW:4、执行CSUDIIPCFGDisconnectAP(szEthName)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetAPSignalQulityStrength_0003(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nWlanApSigQulity = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"步骤1 失败");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"步骤2 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetAPSignalQulityStrength(&nWlanApSigQulity,CSUDI_NULL),"步骤3 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"步骤4失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}
//**********************************************************************************//
//******************************End Test GetAPSignalQulityStrength*******************************//
//**********************************************************************************//
#endif

//**********************************************************************************//
//******************************Begin Test SetDNSServer*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:成功设置DNS的地址
//@PRECONDITION: 1、协议栈初始化成功
//@PRECONDITION: 2、存在索引号为0的DNS
//@INPUT:nDNSIndex=0，DNS的索引号
//@INPUT:szDNS= g_szDNSIp(参考配置文件)  
//@INPUT:nLen=16， DNS的IP地址字符串长度
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，返回成功
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:索引号为非法值4时设置DNS的地址失败
//@PRECONDITION: 1、协议栈初始化成功
//@PRECONDITION: 2、不存在索引号为4的DNS
//@INPUT:nDNSIndex=4，DNS的索引号
//@INPUT:szDNS= g_szDNSIp(参考配置文件)  
//@INPUT:nLen=16， DNS的IP地址字符串长度
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，失败返回错误码
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(4, g_szDNSIp, 16), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:DNS IP地址置为非法值空时设置DNS地址失败
//@PRECONDITION: 1、协议栈初始化成功
//@PRECONDITION: 2、存在索引号为0的DNS
//@INPUT:nDNSIndex=0，DNS的索引号
//@INPUT:pcDNS= CSUDI_NULL ，DNS的IP地址，置为空
//@INPUT:nLen=16， DNS的IP地址字符串长度
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,pcDNS,nLen)操作，失败返回错误码
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0004(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(0, CSUDI_NULL, 16), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:DNS的IP地址字符串长度为15时设置DNS的地址失败
//@PRECONDITION: 1、协议栈初始化成功
//@PRECONDITION: 2、存在索引号为0的DNS
//@INPUT:nDNSIndex=0，DNS的索引号
//@INPUT:szDNS= g_szDNSIp(参考配置文件)  
//@INPUT:nLen=15， DNS的IP地址字符串长度，置为15
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，失败返回错误码
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0005(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 15), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer 
//@DESCRIPTION:IP地址非法时设置DNS的地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在DNS索引号为0
//@INPUT:nDNSIndex=0，DNS的索引号
//@INPUT:szDNS= "a0.b1.c0.d9" ，DNS的IP地址，置为a0.b1.c0.d9
//@INPUT:nLen=16， DNS的IP地址字符串长度
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，返回失败
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0006(void)
{
	char szDNSSet[] = "a0.b1.c0.d9";

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(0, szDNSSet, 16), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer 
//@DESCRIPTION:IP地址非法时设置DNS的地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在DNS索引号为0
//@INPUT:nDNSIndex=0，DNS的索引号
//@INPUT:szDNS= "10.10.98.256" ，DNS的IP地址，置为10.10.98.256
//@INPUT:nLen=16， DNS的IP地址字符串长度
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，返回失败
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0007(void)
{
	char szDNSSet[] = "10.10.98.256";

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(0, szDNSSet, 16), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:DNS的IP地址字符串长度置为17时设置DNS的地址成功
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在DNS索引号为0
//@INPUT:nDNSIndex=0，DNS的索引号
//@INPUT:szDNS= g_szDNSIp(参考配置文件)  
//@INPUT:nLen=17， DNS的IP地址字符串长度，置为17
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，返回成功
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0008(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 17), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End Test SetDNSServer*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test GetDNSServer*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:成功获得DNS地址
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在索引号为0的DNS
//@PRECONDITION:3、可以成功设置DNS的地址
//@INPUT:1、CSUDIIPCFGSetDNSServer 函数的参数说明:
//@INPUT:nDNSIndex=0
//@INPUT:szDNS= g_szDNSIp(参考配置文件)  
//@INPUT:nLen=16
//@INPUT:2、CSUDIIPCFGGetDNSServer 函数的参数说明:
//@INPUT:nDNSIndex = 0，DNS的索引号，置为0
//@INPUT:szDNS[16] ，用来存储DNS的IP地址
//@INPUT:nLen=sizeof(szDNS)， DNS的IP地址字符串长度
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetDNSServer(nDNSIndex,szDNS,nLen)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0001(void)
{
	char szDNS[16];

	memset(szDNS, 0, sizeof(szDNS));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetDNSServer(0, szDNS, 16), "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:索引号不匹配时获得DNS地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在索引号为0的DNS，不存在索引号为2的DNS
//@PRECONDITION:3、可以成功设置DNS的地址
//@INPUT:1、CSUDIIPCFGSetDNSServer 函数的参数说明:
//@INPUT:nDNSIndex=0
//@INPUT:szDNS= g_szDNSIp(参考配置文件)  
//@INPUT:nLen=16
//@INPUT:2、CSUDIIPCFGGetDNSServer 函数的参数说明:
//@INPUT:nDNSIndex = 2，DNS的索引号，置为2
//@INPUT:szDNS[16] ，用来存储DNS的IP地址
//@INPUT:nLen=sizeof(szDNS)， DNS的IP地址字符串长度
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetDNSServer(nDNSIndex,szDNS,nLen)操作，返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0002(void)
{
	char szDNS[16];

	memset(szDNS, 0, sizeof(szDNS));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetDNSServer(CSUDI_ETHER_DNS_MAX_SUPPORT, szDNS, 16), "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:DNS地址为非法值空时获得DNS地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在索引号为0的DNS
//@PRECONDITION:3、可以成功设置DNS的地址
//@INPUT:1、CSUDIIPCFGSetDNSServer 函数的参数说明:
//@INPUT:nDNSIndex=0
//@INPUT:szDNS= g_szDNSIp(参考配置文件)  
//@INPUT:nLen=16
//@INPUT:2、CSUDIIPCFGGetDNSServer 函数的参数说明:
//@INPUT:nDNSIndex = 0，DNS的索引号，置为0
//@INPUT:pcDNS = CSUDI_NULL 
//@INPUT:nLen=16
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetDNSServer(nDNSIndex,szDNS,nLen)操作，返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetDNSServer(0, CSUDI_NULL, 16), "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:DNS的IP地址字符串长度置为非法值15时获得DNS的地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在索引号为0的DNS
//@PRECONDITION:3、可以成功设置DNS的地址
//@INPUT:1、CSUDIIPCFGSetDNSServer 函数的参数说明:
//@INPUT:nDNSIndex=0
//@INPUT:szDNS= g_szDNSIp(参考配置文件)  
//@INPUT:nLen=16
//@INPUT:2、CSUDIIPCFGGetDNSServer 函数的参数说明:
//@INPUT:nDNSIndex = 0，DNS的索引号，置为0
//@INPUT:pcDNS 
//@INPUT:nLen=15， DNS的IP地址字符串长度
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetDNSServer(nDNSIndex,pcDNS,nLen)操作，返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0004(void)
{
	char szDNS[16];

	memset(szDNS, 0, sizeof(szDNS));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetDNSServer(0, szDNS, 15), "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:测试设置的dns和获取的dns是否一致
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在索引号为0的DNS
//@PRECONDITION:2、存在索引号为1的DNS
//@EXECUTIONFLOW: 1、调用CSUDIIPCFGSetDNSServer(0, "192.168.99.4", 16) 设置第一个dns1
//@EXECUTIONFLOW: 2、调用CSUDIIPCFGSetDNSServer(0, "192.168.99.5", 16) 设置第二个dns2
//@EXECUTIONFLOW: 3、调用CSUDIIPCFGGetDNSServer获取第一个dns1
//@EXECUTIONFLOW: 4、调用CSUDIIPCFGGetDNSServer获取第二个dns2
//@EXECUTIONFLOW: 5、比较第获取的第一个dns1和设置的第一个dns1是否相等
//@EXECUTIONFLOW: 6、比较第获取的第二个dns2和设置的第二个dns2是否相等
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0005(void)
{
	int nRet = -1;
	char acDNS1[16] = {0};
	char acDNS2[16] = {0};

	nRet = CSUDIIPCFGSetDNSServer(0, "192.168.99.4", 16);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "步骤1失败");

	nRet = CSUDIIPCFGSetDNSServer(1, "192.168.99.5", 16);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "步骤2失败");

	nRet = CSUDIIPCFGGetDNSServer(0, acDNS1, 16);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "步骤3失败");

	nRet = CSUDIIPCFGGetDNSServer(1, acDNS2, 16);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "步骤4失败");

	nRet = memcmp(acDNS1, "192.168.99.4", strlen("192.168.99.4"));
	CSTK_ASSERT_TRUE_FATAL(0 == nRet, "步骤5失败");

	nRet = memcmp(acDNS2, "192.168.99.5", strlen("192.168.99.5"));
	CSTK_ASSERT_TRUE_FATAL(0 == nRet, "步骤6失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;   
}

//**********************************************************************************//
//******************************End Test GetDNSServer*******************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test SetIpInfo *******************************//
//**********************************************************************************//
static CSUDI_BOOL IPCFG_iSetDefault(void)
{
	CSUDI_BOOL bRet = CSUDI_TRUE;
	char szIp[16];
	char szMask[16];
	char szGateWay[16];

	memset(szIp, 0, sizeof(szIp));
	memset(szMask, 0, sizeof(szMask));
	memset(szGateWay, 0, sizeof(szGateWay));

	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("SOCKET", "BOX_IP", szIp, sizeof(szIp)))
	{
		CSTCPrint("Get IP config failed!\n");
		bRet = CSUDI_FALSE;
	}
	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("SOCKET", "BOX_MASK", szMask, sizeof(szMask)))
	{
		CSTCPrint("Get mask config failed!\n");
		bRet = CSUDI_FALSE;
	}
	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("SOCKET", "BOX_GETEWAY", szGateWay, sizeof(szGateWay)))
	{
		CSTCPrint("Get gateway config failed!\n");
		bRet = CSUDI_FALSE;
	}

	if(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, szIp, szMask, szGateWay))
	{
		CSTCPrint("Set default IP failed!\n");
		bRet = CSUDI_FALSE;
	}

	return bRet;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:成功设置网卡的IP地址、子网掩码和网关地址
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2 、szIp="192.168.1.1" 
//@INPUT:3 、szMask="255.255.255.0"
//@INPUT:4 、szGateWay="192.168.1.254"
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,szGateWay)操作
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0001(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "步骤1失败");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:子网掩码非法时设置网卡的IP地址、子网掩码和网关地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2 、szIp="192.168.1.1" 
//@INPUT:3 、szMask="255.255.255.255"
//@INPUT:4 、szGateWay="192.168.1.254"
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,szGateWay)操作，失败返回错误代码值。
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0002(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", "255.255.255.255", "192.168.1.254"), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:IP地址非法时设置网卡的IP地址、子网掩码和网关地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2 、szIp="192.168.1.256" 
//@INPUT:3 、szMask="255.255.255.0"
//@INPUT:4 、szGateWay="192.168.1.254"
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,szGateWay)操作，失败返回错误代码值。
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0003(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.256", "255.255.255.0", "192.168.1.254"), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:网关非法时设置网卡的IP地址、子网掩码和网关地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2 、szIp="192.168.1.1" 
//@INPUT:3 、szMask="255.255.255.0"
//@INPUT:4 、szGateWay="192.168.1.256"
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,szGateWay)操作，成功返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0004(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.256"), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:网关为非法值空时设置网卡的IP地址、子网掩码和网关地址成功
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2 、szIp="192.168.1.1" 
//@INPUT:3 、szMask="255.255.255.0"
//@INPUT:4 、pcGateWay=CSUDI_NULL
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,pcGateWay)操作，成功返回CSUDI_SUCCESS。
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0005(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", "255.255.255.0", CSUDI_NULL), "步骤1失败");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:掩码为非法值空时设置网卡的IP地址、子网掩码和网关地址成功
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2 、szIp="192.168.1.1" 
//@INPUT:3 、pcMask=CSUDI_NULL
//@INPUT:4 、szGateWay="192.168.1.254"
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szEthName,szIp,pcMask,szGateWay)操作，成功返回CSUDI_SUCCESS。
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0006(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", CSUDI_NULL, "192.168.1.254"), "步骤1失败");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:IP为非法值空时设置网卡的IP地址、子网掩码和网关地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2 、pcIp=CSUDI_NULL
//@INPUT:3 、szMask="255.255.255.0"
//@INPUT:4 、szGateWay="192.168.1.254"
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szEthName,pcIp,szMask,szGateWay)操作，失败返回错误代码值。
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0007(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(szEthName, CSUDI_NULL, "255.255.255.0", "192.168.1.254"), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:网卡名为非法值空时设置网卡的IP地址、子网掩码和网关地址失败
//@PRECONDITION:1、协议栈初始化成功
//@INPUT:1、 pcEthName=CSUDI_NULL
//@INPUT:2 、szIp="192.168.1.1" 
//@INPUT:3 、szMask="255.255.255.0"
//@INPUT:4 、szGateWay="192.168.1.254"
//@EXPECTATION:  失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(pcEthName,szIp,szMask,szGateWay)操作，失败返回错误代码值。
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0008(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(CSUDI_NULL, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:IP地址，子网掩码设置成0.0.0.0时，测试用例通过
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 pcEthName=CS_LAN0_NAME
//@INPUT:2 、szIp="0.0.0.0" 
//@INPUT:3 、szMask="0.0.0.0"
//@INPUT:4 、szGateWay="192.168.1.254"
//@EXPECTATION:  失败返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(pcEthName,szIp,szMask,szGateWay)操作，成功返回CSUDI_SUCCESS。
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0009(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "0.0.0.0", "0.0.0.0", "192.168.1.254"), "步骤1失败");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:子网掩码设置成255.255.255.128时，测试用例通过
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@INPUT:1、 pcEthName=CS_LAN0_NAME
//@INPUT:2 、szIp="10.135.255.188" 
//@INPUT:3 、szMask="255.255.255.128"
//@INPUT:4 、szGateWay="10.135.255.129"
//@EXPECTATION:  失败返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(pcEthName,szIp,szMask,szGateWay)操作，成功返回CSUDI_SUCCESS。
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0010(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "10.135.255.188", "255.255.255.128", "10.135.255.129"), "步骤1失败");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End Test SetIpInfo *************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test  GetIpInfo*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:成功获取网卡的IP地址、子网掩码和网关地址
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@PRECONDITION:3、可以成功设置网卡的IP地址、子网掩码和网关地址
//@INPUT:1、CSUDIIPCFGSetIpInfo 函数的参数说明:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2、CSUDIIPCFGGetIpInfo 函数的参数说明:
//@INPUT:szGetEthName=CS_LAN0_NAME
//@INPUT:szGetIp[16]， 保存获取的IP地址
//@INPUT:szGetMask[16]，保存子网掩码
//@INPUT:szGetGateWay[16]，保存网关地址
//@EXPECTATION:  成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetIpInfo(szGetEthName,szGetIp,szGetMask,szGetGateWay)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0001(void)
{
	char szGetIp[16];
	char szGetMask[16];
	char szGetGateWay[16];

	memset(szGetIp, 0, sizeof(szGetIp));
	memset(szGetMask, 0, sizeof(szGetMask));
	memset(szGetGateWay, 0, sizeof(szGetGateWay));

	/* 改为 "192.168.010.1" 可以检测驱动是否做了8进制转换 */
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.010.1" , "255.255.255.0", "192.168.010.254"), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, szGetIp, szGetMask, szGetGateWay), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetIp, "192.168.10.1", sizeof("192.168.10.1")), "获得的IP与设置的不同");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetMask, "255.255.255.0", sizeof("255.255.255.0")), "获得的子网掩码与设置的不同");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetGateWay, "192.168.10.254", sizeof("192.168.10.254")), "获得的网关地址与设置的不同");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:网卡名为非法值空时获取网卡的IP地址、子网掩码和网关地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@PRECONDITION:3、可以成功设置网卡的IP地址、子网掩码和网关地址
//@INPUT:1、CSUDIIPCFGSetIpInfo 函数的参数说明:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2、CSUDIIPCFGGetIpInfo 函数的参数说明:
//@INPUT:pcGetEthName=CSUDI_NULL
//@INPUT:szGetIp[16]， 保存获取的IP地址
//@INPUT:szGetMask[16]，保存子网掩码
//@INPUT:szGetGateWay[16]，保存网关地址
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetIpInfo(pcGetEthName,szGetIp,szGetMask,szGetGateWay)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0002(void)
{
	char szGetIp[16];
	char szGetMask[16];
	char szGetGateWay[16];

	memset(szGetIp, 0, sizeof(szGetIp));
	memset(szGetMask, 0, sizeof(szGetMask));
	memset(szGetGateWay, 0, sizeof(szGetGateWay));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetIpInfo(CSUDI_NULL, szGetIp, szGetMask, szGetGateWay), "步骤2失败");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:指针为空时获取网卡的IP地址、子网掩码和网关地址失败
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@PRECONDITION:3、可以成功设置网卡的IP地址、子网掩码和网关地址
//@INPUT:1、CSUDIIPCFGSetIpInfo 函数的参数说明:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2、CSUDIIPCFGGetIpInfo 函数的参数说明:
//@INPUT:szGetEthName=CS_LAN0_NAME
//@INPUT:pcGetIp=CSUDI_NULL
//@INPUT:pcGetMask=CSUDI_NULL
//@INPUT:pcGetGateWay=CSUDI_NULL
//@EXPECTATION: 失败返回CSUDI_FAILURE
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetIpInfo(pcGetIp,szGetIp,pcGetMask,pcGetGateWay)操作，失败返回错误代码值
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, CSUDI_NULL, CSUDI_NULL, CSUDI_NULL), "步骤2失败");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:成功获取网卡的IP地址、子网掩码
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@PRECONDITION:3、可以成功设置网卡的IP地址、子网掩码和网关地址
//@INPUT:1、CSUDIIPCFGSetIpInfo 函数的参数说明:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2、CSUDIIPCFGGetIpInfo 函数的参数说明:
//@INPUT:szGetEthName=CS_LAN0_NAME
//@INPUT:szGetIp[16]， 保存获取的IP地址
//@INPUT:szGetMask[16]，保存子网掩码
//@INPUT:pcGetGateWay=CSUDI_NULL
//@EXPECTATION: 成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetIpInfo(szGetEthName,szGetIp,szGetMask,pcGetGateWay)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0004(void)
{
	char szGetIp[16];
	char szGetMask[16];

	memset(szGetIp, 0, sizeof(szGetIp));
	memset(szGetMask, 0, sizeof(szGetMask));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, szGetIp, szGetMask, CSUDI_NULL), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetIp, "192.168.1.1", sizeof("192.168.1.1")), "获得的IP与设置的不同");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetMask, "255.255.255.0", sizeof("255.255.255.0")), "获得的子网掩码与设置的不同");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:成功获取网卡的IP地址
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在有线网卡
//@PRECONDITION:3、可以成功设置网卡的IP地址、子网掩码和网关地址
//@INPUT:1、CSUDIIPCFGSetIpInfo 函数的参数说明:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2、CSUDIIPCFGGetIpInfo 函数的参数说明:
//@INPUT:szGetEthName=CS_LAN0_NAME
//@INPUT:szGetIp[16]， 保存获取的IP地址
//@INPUT:pcGetMask = CSUDI_NULL
//@INPUT:pcGetGateWay=CSUDI_NULL
//@EXPECTATION: 成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)操作，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、执行CSUDIIPCFGGetIpInfo(szGetEthName,szGetIp,szGetMask,pcGetGateWay)操作，返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0005(void)
{
	char szGetIp[16];

	memset(szGetIp, 0, sizeof(szGetIp));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, szGetIp, CSUDI_NULL, CSUDI_NULL), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetIp, "192.168.1.1", sizeof("192.168.1.1")), "获得的IP与设置的不同");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "恢复默认IP设置");

	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End  Test GetIpInfo *******************************//
//**********************************************************************************//

//**********************************************************************************//
//******************************Begin Test  GetStatInfo*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetStatInfo
//@DESCRIPTION:成功获取网卡统计信息
//@PRECONDITION:1、协议栈初始化成功
//@PRECONDITION:2、存在网卡
//@INPUT:szEthName=CS_LAN0_NAME
//@EXPECTATION: 成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、判断至少存在一张网卡
//@EXECUTIONFLOW: 2、循环执行CSUDIIPCFGGetStatInfo(szEthName,&sEtherStat)操作，期望成功获取有线网卡统计信息
//@EXECUTIONFLOW: 3、循环执行CSUDIIPCFGGetStatInfo(szEthName,&sEtherStat)操作，期望成功获取无线网卡统计信息
CSUDI_BOOL CSTC_IPCFG_IT_GetStatInfo_0001(void)
{
	int i = 0;
	CSUDIEtherStat_S sEtherStat;

	CSTK_ASSERT_TRUE_FATAL((s_nActLanCount+s_nActWLanCount) > 0, "没有网卡");

	for(i=0; i<s_nActLanCount; i++)
	{
		memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetStatInfo(s_asLanCfg[i].szEthName, &sEtherStat), "获取有线网卡统计信息失败");

		CSTCPrint("interface:%s, connect time:%d, send Byte:%d, recv Byte:%d, send Packet:%d, recv Packet:%d\n", 
			s_asLanCfg[i].szEthName, sEtherStat.uConnectTime, 
			sEtherStat.uTotalBytesSend, sEtherStat.uTotalBytesReceived, 
			sEtherStat.uTotalPacketSend, sEtherStat.uTotalPacketReceived);
	}

	for(i=0; i<s_nActWLanCount; i++)
	{
		memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetStatInfo(s_asWLanCfg[i].szEthName, &sEtherStat), "获取无线网卡统计信息失败");

		CSTCPrint("interface:%s, connect time:%d, send Bytes:%d, recv Bytes:%d, send Packets:%d, recv Packets:%d\n", 
			s_asWLanCfg[i].szEthName, sEtherStat.uConnectTime, 
			sEtherStat.uTotalBytesSend, sEtherStat.uTotalBytesReceived, 
			sEtherStat.uTotalPacketSend, sEtherStat.uTotalPacketReceived);
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetStatInfo
//@DESCRIPTION:网卡名为空，获取网卡统计信息失败
//@PRECONDITION:1、协议栈初始化成功
//@INPUT:网卡名为空:
//@EXPECTATION: 失败返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetStatInfo(CSUDI_NULL,&sEtherStat)操作，期望执行失败
CSUDI_BOOL CSTC_IPCFG_IT_GetStatInfo_0002(void)
{
	CSUDIEtherStat_S sEtherStat;

	memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetStatInfo(CSUDI_NULL, &sEtherStat), "获取有线网卡统计信息不应该成功");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetStatInfo
//@DESCRIPTION:网卡统计信息结构体为空，获取网卡统计信息失败
//@PRECONDITION:1、协议栈初始化成功
//@INPUT:有线网卡名:
//@EXPECTATION: 失败返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetStatInfo(szEthName, CSUDI_NULL)操作，期望执行失败
CSUDI_BOOL CSTC_IPCFG_IT_GetStatInfo_0003(void)
{
	CSUDIEtherStat_S sEtherStat;

	CSTK_ASSERT_TRUE_FATAL(s_nActLanCount > 0, "没有有线网卡");
	memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetStatInfo(s_asLanCfg[0].szEthName, CSUDI_NULL), "获取有线网卡统计信息不应该成功");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetStatInfo
//@DESCRIPTION:不存在的网卡名，获取网卡统计信息失败
//@PRECONDITION:1、协议栈初始化成功
//@INPUT:不存在的网卡名:
//@EXPECTATION: 失败返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、执行CSUDIIPCFGGetStatInfo("Eth0",&sEtherStat)操作，期望执行失败
CSUDI_BOOL CSTC_IPCFG_IT_GetStatInfo_0004(void)
{
	CSUDIEtherStat_S sEtherStat;

	memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetStatInfo("Eth0", &sEtherStat), "获取有线网卡统计信息不应该成功");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}
//**********************************************************************************//
//******************************End  Test GetStatInfo *******************************//
//**********************************************************************************//

//**********************************************************************************//
//******************************Begin Test RawPacket********************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIRawSendPacket
//@DESCRIPTION:从网卡驱动层直接发包成功
//@PRECONDITION:1、协议栈初始化成功
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2、 fd = -1
//@INPUT:3、 nFlag = 1
//@EXPECTATION: 成功返回0
//@EXECUTIONFLOW: 1、执行CSUDIRawSendPacket(szEthName, &nSockRet, (char *)(&acSendBuf), sizeof(acSendBuf), 1)操作
//@EXECUTIONFLOW: 2、判断返回值，期望为0，判断fd，期望不等于-1
//@EXECUTIONFLOW: 3、执行CSUDISOCKClose(nFd)期望返回0
CSUDI_BOOL CSTC_IPCFG_RAWSOCKET_0001(void)
{
	int nFd = -1;
	char szEthName[16];
	char acSendBuf[548] = {0};
	int nRet = -1;

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	nRet = CSUDIRawSendPacket(szEthName, &nFd, (char *)(&acSendBuf), sizeof(acSendBuf), 1);
	CSTK_ASSERT_TRUE_FATAL(nRet == 0, "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(nFd != -1, "步骤2失败");

	CSTK_FATAL_POINT
	if(nFd != -1 )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISOCKClose(nFd), "关闭套接字失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRawSendPacketByMode
//@DESCRIPTION:从网卡驱动层直接发包成功
//@PRECONDITION:1、协议栈初始化成功
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2、 fd = -1
//@INPUT:3、 nFlag = 1
//@INPUT:4、 eRawSendMode = EM_UDIRAWSENDTYPE_UNICAST
//@EXPECTATION: 成功返回0
//@EXECUTIONFLOW: 1、执行CSUDIRawSendPacketByMode(szEthName, &nSockRet, (char *)(&acSendBuf), sizeof(acSendBuf), 1, EM_UDIRAWSENDTYPE_UNICAST)操作
//@EXECUTIONFLOW: 2、判断返回值，期望为0，判断fd，期望不等于-1
//@EXECUTIONFLOW: 3、执行CSUDISOCKClose(nFd)期望返回0
CSUDI_BOOL CSTC_IPCFG_RAWSOCKET_0002(void)
{
#if 0
	int nFd = -1;
	char szEthName[16];
	char acSendBuf[548] = {0};
	int nRet = -1;

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	nRet = CSUDIRawSendPacketByMode(szEthName, &nFd, (char *)(&acSendBuf), sizeof(acSendBuf), 1, EM_UDIRAWSENDTYPE_UNICAST);
	CSTK_ASSERT_TRUE_FATAL(nRet == 0, "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(nFd != -1, "步骤2失败");

	CSTK_FATAL_POINT
	if(nFd != -1 )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISOCKClose(nFd), "关闭套接字失败");
	}
#endif
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRawRecvPacket
//@DESCRIPTION:从网卡驱动层直接接收包成功
//@PRECONDITION:1、协议栈初始化成功
//@INPUT:1、 szEthName=CS_LAN0_NAME
//@INPUT:2、 fd = -1
//@INPUT:3、 nFlag = 1
//@INPUT:4、 eRawSendMode = EM_UDIRAWSENDTYPE_UNICAST
//@EXPECTATION: 成功返回0
//@EXECUTIONFLOW: 1、提示从pc一直ping机顶盒
//@EXECUTIONFLOW: 2、执行CSUDIRawSendPacketByMode(szEthName, &nFd, (char *)(&acSendBuf), sizeof(acSendBuf), 1)操作
//@EXECUTIONFLOW: 3、判断返回值，期望为0，判断fd，期望不等于-1
//@EXECUTIONFLOW: 4、执行CSUDIRawRecvPacket(szEthName, &nFd, (char *)(&acRecvBuf), &nRecvLen, 1)操作
//@EXECUTIONFLOW: 5、判断返回值，期望大于0
//@EXECUTIONFLOW: 6、执行CSUDISOCKClose(nFd)期望返回0
CSUDI_BOOL CSTC_IPCFG_RAWSOCKET_0003(void)
{
	int nFd = -1;
	char szEthName[16];
	char acSendBuf[548] = {0};
	char acRecvBuf[548] = {0};
	int nRecvLen = 548;
	int nRet = -1;
	char acStbIP[16] = {0};

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("SOCKET", "BOX_IP", acStbIP, sizeof(acStbIP)),
			"从配置文件中获取机顶盒IP配置项失败")

	CSTCPrint("请使用pc一直ping机顶盒(example: ping 172.30.18.195 -t 保证能ping通)\n");
	CSTCPrint("配置文件中配置的机顶盒IP为：%s\n", acStbIP);
	CSTKWaitAnyKey();

	nRet = CSUDIRawSendPacket(szEthName, &nFd, (char *)(&acSendBuf), sizeof(acSendBuf), 1);
	CSTK_ASSERT_TRUE_FATAL(nRet == 0, "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(nFd != -1, "步骤2失败");

	nRet = CSUDIRawRecvPacket(szEthName, &nFd, (char *)(&acRecvBuf), &nRecvLen, 1);
	CSTCPrint("nRet = %d\n", nRet);
	CSTK_ASSERT_TRUE_FATAL(nRet > 0, "步骤3失败");

	CSTK_FATAL_POINT
	if(nFd != -1 )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISOCKClose(nFd), "关闭套接字失败");
	}
	CSTCPrint("请终止pc上的ping命令\n");
	CSTKWaitAnyKey();

	return CSUDI_TRUE;
}
//**********************************************************************************//
//******************************End  Test RawPacket ********************************//
//**********************************************************************************//


