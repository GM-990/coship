/*
 *@version 1.0.0 2009/08/13 ��ɳ�ʼ�汾
 */
/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.������������������CSTC_��������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_ipcfgtestcase.h"
#include "udi2_ipcfg.h"
#include "cs_testkit.h"
#include "udi2_os.h"
#include "udi2_socket.h"

static char g_szDNSIp[16] = {0};
static CSUDIWLanConfig_S s_asLanCfg[10];//�����ļ���������������
static CSUDIWLanConfig_S s_asWLanCfg[10];//�����ļ���������������
static int s_nActLanCount = 0;//�����ļ���������������
static int s_nActWLanCount = 0;//�����ļ���������������

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
	//�ڱ�����������ִ��ǰ����
	return IPCFG_iInitCfg();
}

CSUDI_BOOL CSTC_IPCFG_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}


//**********************************************************************************//
//******************************Begin Test  GetEtherCount*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetEtherCount
//@DESCRIPTION:�ɹ����������
//@PRECONDITION: Э��ջ�Ѿ���ʼ���ɹ�
//@EXPECTATION:  �ɹ�����ϵͳ���õ�������
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherCount(void)����������ϵͳ���õ�������
//@EXECUTIONFLOW: 2���Ƚϻ�ȡ��������Ŀ���������Ƿ�һ��
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherCount_0001(void)
{
	int nCount = 0 ;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "����1ʧ��");
	CSTCPrint("������:%d\n", nCount);
	CSTK_ASSERT_TRUE_FATAL(nCount == (s_nActLanCount+s_nActWLanCount), "����2ʧ��");

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
//@DESCRIPTION:�ɹ���ȡ���ڵ����������豸��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@INPUT:1��nEthIndex = i
//@INPUT:2��szEthName[16]��������Ż�ȡ���豸��
//@INPUT:3��nLen = sizeof(szEthName)
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherCount(&nCount)�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetEtherName(i, szEthName, nLen)�������ɹ�����CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0001(void)
{
	int i = 0;
	char szEthName[16];
	int nCount = 0;
	int nLen = sizeof(szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "����1ʧ��");
	CSTCPrint("������:%d\n", nCount);

	for(i=0; i<nCount; i++)
	{
		memset(szEthName, 0, nLen);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherName(i, szEthName, nLen), "����2ʧ��");
		CSTCPrint("����%d��Ϊ:%s\n", i, szEthName);
	}
		
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:������Ϊ�Ƿ�ֵnCountʱ��ȡ�����豸��ʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@INPUT:1��nEthIndex = nCount
//@INPUT:2��szEthName[16]��������Ż�ȡ���豸��
//@INPUT:3��nLen = sizeof(szEthName)
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherCount(&nCount)�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetEtherName(nEthIndex,szEthName,nLen)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0002(void)
{
	char szEthName[16];
	int nCount = 0;
	int nLen = sizeof(szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "����1ʧ��");
	memset(szEthName, 0, nLen);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherName(nCount, szEthName, nLen), "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:ָ��Ϊ�Ƿ�ֵ��ʱ��ȡ�����豸��ʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2���豸������Ϊ0����������
//@INPUT:1��nEthIndex = 0
//@INPUT:2��pcEthName = CSUDI_NULL��ָ����Ϊ��
//@INPUT:3��nLen = sizeof(szEthName)
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherCount(&nCount)�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetEtherName(nCount-1, CSUDI_NULL, 16)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0003(void)
{
	int nCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "����1ʧ��");
	if(nCount > 0)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherName(0, CSUDI_NULL, 16), "����2ʧ��");
	}	
	else
	{
		CSTCPrint("û���ҵ�����\n");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:�洢�豸������Ϊ0ʱ��ȡ�����豸��ʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2���豸������Ϊ0����������
//@INPUT:1��nEthIndex = 0
//@INPUT:2��szEthName[16]��������Ż�ȡ���豸��
//@INPUT:3��nLen = 0
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherCount(&nCount)�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetEtherName(0, szEthName, 0)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0004(void)
{
	int nCount = 0;
	char szEthName[16];
	int nLen = sizeof(szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "����1ʧ��");
	if(nCount > 0)
	{
		memset(szEthName, 0, nLen);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherName(0, szEthName, 0), "����2ʧ��");
	}	
	else
	{
		CSTCPrint("û���ҵ�����\n");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherName
//@DESCRIPTION:�洢�豸�����泬��16�ֽ�ʱ��ȡ�����豸��ʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2���豸������Ϊ0����������
//@INPUT:1��nEthIndex = 0
//@INPUT:2��szEthName[17]��������Ż�ȡ���豸��
//@INPUT:3��nLen = sizeof(szEthName)
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherCount(&nCount)�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetEtherName(0, szEthName, 17)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherName_0005(void)
{
	int nCount = 0;
	char szEthName[17];
	int nLen = sizeof(szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "����1ʧ��");
	if(nCount > 0)
	{
		memset(szEthName, 0, nLen);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherName(0, szEthName, 17), "����2ʧ��");
	}	
	else
	{
		CSTCPrint("û���ҵ�����\n");
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
//@DESCRIPTION:�ɹ������������оƬ����Ϣ
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@INPUT:1�������ļ��д��ڵĸ���������
//@INPUT:2��sChipInfo���������оƬ��Ϣ�Ľṹ��
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherCount(&nCount)�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Ƚϻ�ȡ��������Ŀ���������Ƿ�һ��
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGGetEtherChipInfo(s_asLanCfg[i].szEthName, &sChipInfo)�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 4��ִ��CSUDIIPCFGGetEtherChipInfo(s_asLanCfg[i].szEthName, &sChipInfo)�������ɹ�����CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0001(void)
{
	int i = 0;
	int nCount = 0;
	CSUDIEtherInfo_S sChipInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherCount(&nCount), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nCount == (s_nActLanCount+s_nActWLanCount), "����2ʧ��");
	for(i=0; i<s_nActLanCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS== CSUDIIPCFGGetEtherChipInfo(s_asLanCfg[i].szEthName, &sChipInfo), "����3ʧ��");
	}
	for(i=0; i<s_nActWLanCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS== CSUDIIPCFGGetEtherChipInfo(s_asWLanCfg[i].szEthName, &sChipInfo), "����4ʧ��");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:������Ϊ�Ƿ�ֵ"eth5"ʱ��ȡоƬ��Ϣʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ��û��������"eth5"������
//@INPUT:1��szEthName[5] = "eth5"
//@INPUT:2��sChipInfo���������оƬ��Ϣ�Ľṹ��
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherChipInfo(szEthName,&sChipInfo)����
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0002(void)
{
	CSUDIEtherInfo_S sChipInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherChipInfo("eth5", &sChipInfo), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:������Ϊ�Ƿ�ֵ"Eth0"ʱ��ȡоƬ��Ϣʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ��û��������"Eth0"������
//@INPUT:1��szEthName[5] = "Eth0"
//@INPUT:2��sChipInfo���������оƬ��Ϣ�Ľṹ��
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherChipInfo(szEthName,&sChipInfo)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0003(void)
{
	CSUDIEtherInfo_S sChipInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherChipInfo("Eth0", &sChipInfo), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:�ṹ��Ϊ�Ƿ�ֵ��ʱ��ȡоƬ��Ϣʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ������������
//@INPUT:1��szEthName[5] = CS_LAN0_NAME
//@INPUT:2��pcChipInfo = CSUDI_NULL
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherChipInfo(szEthName,pcChipInfo)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0004(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherChipInfo(szEthName, CSUDI_NULL), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherChipInfo
//@DESCRIPTION:������Ϊ�Ƿ�ֵ��ʱ��ȡоƬ��Ϣʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@INPUT:1��pcEthName = CSUDI_NULL�������豸��Ϊ��
//@INPUT:2��sChipInfo���������оƬ��Ϣ�Ľṹ��
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherChipInfo(pcEthName,&sChipInfo)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherChipInfo_0005(void)
{
	CSUDIEtherInfo_S sChipInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherChipInfo(CSUDI_NULL, &sChipInfo), "����1ʧ��");

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
//@DESCRIPTION:�ɹ����������ΪCS_LAN0_NAMEоƬ�������ַ
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�д�����������
//@INPUT:1��szEthName[5] = CS_LAN0_NAME
//@INPUT:2��szMacAddress[8]�����MAC��ַ
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetMacByName(szEthName,szMacAddress)������ �ɹ�����CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0001(void)
{
	unsigned char szMacAddress[8] = {0};
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetMacByName(szEthName, szMacAddress), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:������Ϊ�Ƿ�ֵ"Eth0"ʱ��ȡоƬ�����ַʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�в�����������"Eth0"������
//@INPUT:1��szEthName[5] = "Eth0"
//@INPUT:2��szMacAddress[8]�����MAC��ַ
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetMacByName(szEthName,szMacAddress)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0002(void)
{
	unsigned char szMacAddress[8] = {0};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetMacByName("Eth0", szMacAddress), "����1ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:������Ϊ�Ƿ�ֵ"eth5"ʱ��ȡоƬ�����ַʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�в�����������"eth5"������
//@INPUT:1��szEthName[5] = "eth5"
//@INPUT:2��szMacAddress[8]�����MAC��ַ
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetMacByName(szEthName,szMacAddress)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0003(void)
{
	unsigned char szMacAddress[8] = {0};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetMacByName("eth5", szMacAddress), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:�洢MAC��ַָ��Ϊ�Ƿ�ֵ��ʱ��ȡоƬ�����ַʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�д�����������
//@INPUT:1��szEthName[5] = CS_LAN0_NAME
//@INPUT:2��pcMacAddress = CSUDI_NULL
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetMacByName(szEthName,pcMacAddress)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0004(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetMacByName(szEthName, CSUDI_NULL), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetMacByName
//@DESCRIPTION:������Ϊ�Ƿ�ֵ��ʱ��ȡоƬ�����ַʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@INPUT:1��pcEthName = CSUDI_NULL
//@INPUT:2��szMacAddress[8]�����MAC��ַ
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetMacByName(pcEthName,szMacAddress)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0005(void)
{
	unsigned char szMacAddress[8] = {0};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetMacByName(CSUDI_NULL, szMacAddress), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP: mac��ַ����
//@DESCRIPTION:  ������ʼ���ɹ�����ȡЭ��ջmac��ַ����ΪFFFFFFFF,Ҳ��Ϊ�ಥ��ַ
//@PRECONDITION: Э��ջ��ʼ���ɹ�
//@EXECUTIONFLOW: ��ȡmac��ַ��ΪFFFFFFFF,Ҳ��Ϊ�ಥ��ַ
//@EXECUTIONFLOW: 1������CSUDIIPCFGGetMacByName��ȡ����mac��ַ
//@EXECUTIONFLOW: 2������ȡ��mac��ַ��FFFFFF��ȫ0��ַ��ȣ������ΪFFFFFF��ȫ0���򷵻سɹ������򷵻�ʧ��
//@EXECUTIONFLOW: 3������ȡ��mac��ַ�Ͷಥ��ַ��ȣ������Ϊ�ಥ��ַ���򷵻سɹ������򷵻�ʧ��
CSUDI_BOOL CSTC_IPCFG_IT_GetMacByName_0006(void)
{
	unsigned char acMacAddress[6] = {0};
	int nRet = -1;
	char szEthName[16] = {0};

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	nRet = CSUDIIPCFGGetMacByName(szEthName, acMacAddress);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "����1ʧ�ܣ���ȡmac��ַʧ��");

	if ((acMacAddress[0]== 0xff)&(acMacAddress[1]== 0xff)&(acMacAddress[2]== 0xff)&(acMacAddress[3]== 0xff)&(acMacAddress[4]== 0xff)&(acMacAddress[5]== 0xff))
	{
		nRet = -1;
	}

	if ((acMacAddress[0]== 0x00)&(acMacAddress[1]== 0x00)&(acMacAddress[2]== 0x00)&(acMacAddress[3]== 0x00)&(acMacAddress[4]== 0x00)&(acMacAddress[5]== 0x00))
	{
		nRet = -1;
	}
	CSTK_ASSERT_TRUE_FATAL(nRet != -1, "����2ʧ�ܣ���ȡmac��ַ�ǹ㲥��ַ����ȫ0��ַ");

	CSTK_ASSERT_TRUE_FATAL((acMacAddress[0] & 0x01) == 0x00, "����3ʧ�ܣ�mac��ַ�Ƕಥ��ַ");

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
//@DESCRIPTION:�ɹ���ȡ����������ΪCS_LAN0_NAME������״̬
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�д�����������
//@PRECONDITION: 3�������в���switchʱ��������
//@INPUT:1��szEthName[5] = CS_LAN0_NAME
//@INPUT:2��ushLinkState�� ��������������״ֵ̬
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS��ushLinkState & CSUDI_ETHMASK_UP_DOWN=0
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherStatus(szEthName,&ushLinkState)����
//@EXECUTIONFLOW: 2��ִ��ushLinkState & CSUDI_ETHMASK_UP_DOWN���ж�����״̬
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0001(void)
{
	unsigned short ushLinkState = 255;
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherStatus(szEthName, &ushLinkState), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == (ushLinkState & CSUDI_ETHMASK_UP_DOWN), "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:����������Ϊ�Ƿ�ֵ"Eth0"ʱ��ȡ����״̬ʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�в�����������"Eth0"����������
//@PRECONDITION: 3�������в���switchʱ��������
//@INPUT:1��szEthName[5] = "Eth0"
//@INPUT:2��ushLinkState�� ��������������״ֵ̬
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherStatus(szEthName,&ushLinkState)������ʧ�ܷ��ش�����
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0002(void)
{
	unsigned short ushLinkState = 255;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherStatus("Eth0", &ushLinkState), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:����������Ϊ�Ƿ�ֵ"eth5"ʱ��ȡ����״̬ʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�в�����������"eth5"����������
//@PRECONDITION: 3�������в���switchʱ��������
//@INPUT:1��szEthName[5] = "eth5"
//@INPUT:2��ushLinkState�� ��������������״ֵ̬
//@EXPECTATION: ʧ�ܷ��ش������ֵ
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherStatus(szEthName,&ushLinkState)����
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0003(void)
{
	unsigned short ushLinkState = 255;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherStatus("eth5", &ushLinkState), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:�����в���switch�ε�����ʱ��ȡ����״̬�ɹ�
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�д�����������
//@PRECONDITION: 3�������в���switchʱ�ε�����
//@INPUT:1��szEthName[5] = CS_LAN0_NAME
//@INPUT:2��ushLinkState�� ��������������״ֵ̬
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS��ushLinkState & CSUDI_ETHMASK_UP_DOWN=1
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherStatus(pcEthName,ushLinkState)����
//@EXECUTIONFLOW: 2����ushLinkState & CSUDI_ETHMASK_UP_DOWN���ж�����״̬�����ӶϿ�������1
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0004(void)
{
	unsigned short ushLinkState = 255;
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTCPrint("��ε�����\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����0ʧ��");
	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetEtherStatus(szEthName, &ushLinkState), "����1ʧ��");

	CSTCPrint("���������\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(1 == (ushLinkState & CSUDI_ETHMASK_UP_DOWN), "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:����������Ϊ�Ƿ�ֵ��ʱ��ȡ����״̬ʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ��������Ϊ��
//@PRECONDITION: 3�������в���switchʱ��������
//@INPUT:1��pcEthName = CSUDI_NULL�������豸������Ϊ��
//@INPUT:2��ushLinkState�� ��������������״ֵ̬
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherStatus(pcEthName,&ushLinkState)����,ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0005(void)
{
	unsigned short ushLinkState = 255;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherStatus(CSUDI_NULL, &ushLinkState), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetEtherStatus
//@DESCRIPTION:�洢����״ֵָ̬��Ϊ�Ƿ�ֵ��ʱ��ȡ����������״̬ʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�д�����������
//@PRECONDITION: 3�������в���switchʱ��������
//@INPUT:1��szEthName[5] = CS_LAN0_NAME
//@INPUT:2��pushLinkState = CSUDI_NULL�� ��������������״ָ̬��Ϊ��
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetEtherStatus(szEthName,pushLinkState)����,ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetEtherStatus_0006(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetEtherStatus(szEthName, CSUDI_NULL), "����1ʧ��");

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
//@DESCRIPTION:AP��Ϣ�ṹ��Ϊ�Ƿ�ֵ��ʱ����AP��Ϣʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�д���������"eth0"����������
//@INPUT:1��szEthName[5] = "eth0"
//@INPUT:2��psWlanApInfo = CSUDI_NULL���洢AP��Ϣ�ṹ��ΪCSUDI_NULL
//@INPUT:3��nWlanApInfoNum = 5���ṹ������ĸ���,��Ϊ5
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,psWlanApInfo, &nWlanApInfoNum)������ʧ�ܷ��ش�����
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0001(void)
{   	
	int nWlanApInfoNum = 5;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP(szEthName,CSUDI_NULL,&nWlanApInfoNum),"����1 ʧ��");
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:�ṹ���������Ϊ�Ƿ�ֵ0ʱ����AP��Ϣʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�д���������"eth0"����������
//@INPUT:1��szEthName[5] = "eth0"
//@INPUT:2��sWlanApInfo���洢AP��Ϣ�ṹ��
//@INPUT:3��nWlanApInfoNum = 0���ṹ������ĸ���,��Ϊ0
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)������ʧ�ܷ��ش�����
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0002(void)
{   	
	int nWlanApInfoNum = 0;
	CSUDIWLanApInfo_S sWlanApInfo;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:��������Ϊ�Ƿ�ֵ��ʱ����AP��Ϣʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@INPUT:1��pcEthName = CSUDI_NULL
//@INPUT:2��sWlanApInfo���洢AP��Ϣ�ṹ��
//@INPUT:3��nWlanApInfoNum = 5
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)������ʧ�ܷ��ش�����
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0003(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP(CSUDI_NULL,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:����AP��Ϣ�ɹ�
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2������������"eth0"����������
//@PRECONDITION: 3��ʵ�ʴ��ڵ�AP����Ϊ5
//@INPUT:1��szEthName[5] = "eth0"
//@INPUT:2��sWlanApInfo���洢AP��Ϣ�ṹ��
//@INPUT:3��nWlanApInfoNum = 5
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)����
//@EXECUTIONFLOW: 2���ж�nWlanApInfoNum��ֵ����nWlanApInfoNum ==5������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0004(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nWlanApInfoNum ==5,"����2 ʧ��");
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:�洢AP��Ϣ�Ľṹ�����Ϊ5��������AP����Ϊ6ʱ�������õ�AP�ĸ���Ϊ5
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2������������"eth0"����������
//@PRECONDITION: 3��ʵ�ʴ��ڵ�AP����Ϊ6
//@INPUT:1��szEthName[5] = "eth0"
//@INPUT:2��sWlanApInfo���洢AP��Ϣ�ṹ��
//@INPUT:3��nWlanApInfoNum = 5
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS������AP����Ϊ5
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)����
//@EXECUTIONFLOW: 2���ж�nWlanApInfoNum��ֵ����nWlanApInfoNum ==5������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0005(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nWlanApInfoNum ==5,"����2 ʧ��");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:�洢AP��Ϣ�Ľṹ�����Ϊ6��������AP����Ϊ5ʱ�������õ�AP�ĸ���Ϊ5
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2������������"eth0"����������
//@PRECONDITION: 3��ʵ�ʴ��ڵ�AP����Ϊ5
//@INPUT:1��szEthName[5] = "eth0"
//@INPUT:2��sWlanApInfo���洢AP��Ϣ�ṹ��
//@INPUT:3��nWlanApInfoNum = 6
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS������AP����Ϊ5
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)����
//@EXECUTIONFLOW: 2���ж�nWlanApInfoNum��ֵ����nWlanApInfoNum ==5������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0006(void)
{   	
	int nWlanApInfoNum = 6;
	CSUDIWLanApInfo_S sWlanApInfo;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[0].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nWlanApInfoNum ==5,"����2 ʧ��");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:������Ϊ�Ƿ�ֵ"Eth0"ʱ����APʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�в�����������"Eth0"����������
//@PRECONDITION: 3��ʵ�ʴ��ڵ�AP����Ϊ5
//@INPUT:1��szEthName[5] = "Eth0"
//@INPUT:2��sWlanApInfo���洢AP��Ϣ�ṹ��
//@INPUT:3��nWlanApInfoNum = 5���ṹ������ĸ���,��Ϊ5
//@EXPECTATION: 
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)������ʧ�ܷ��ش�����
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0007(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP("Eth0",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSearchAP
//@DESCRIPTION:������Ϊ�Ƿ�ֵ"eth5"ʱ����APʧ��
//@PRECONDITION: 1��Э��ջ�Ѿ���ʼ���ɹ�
//@PRECONDITION: 2��Э��ջ�в�����������"eth5"����������
//@PRECONDITION: 3��ʵ�ʴ��ڵ�AP����Ϊ5
//@INPUT:1��szEthName[5] = "eth5"
//@INPUT:2��sWlanApInfo���洢AP��Ϣ�ṹ��
//@INPUT:3��nWlanApInfoNum = 5���ṹ������ĸ���,��Ϊ5
//@EXPECTATION: ʧ�ܷ��ش�����
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo, &nWlanApInfoNum)����
CSUDI_BOOL CSTC_IPCFG_IT_SearchAP_0008(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSearchAP("eth5",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");
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
//@DESCRIPTION:������Ϊ�Ƿ�ֵ��ʱ��AP��֤ʧ��
//@PRECONDITION: 1�����Ի��������ٴ���һ��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName="";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(pcEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDIIPCFG_ERROR_CONNECTAP_ERROR
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0001(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memset(sWLanConf.szEthName ,0,sizeof(sWLanConf.szEthName));
	memcpy(sWLanConf.szEssId,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGConnectAP(&sWLanConf),"����2 ʧ��");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:��Կ����ΪEM_UDIWLAN_KEY_HEXʱ��AP��֤�ɹ�
//@PRECONDITION: 1�����Ի��������ٴ���һ����Կ����ΪEM_UDIWLAN_KEY_HEX ��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0002(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,szEthName,sizeof(szEthName));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"����2 ʧ��");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����3 ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:��Կ����ΪEM_UDIWLAN_KEY_ANSIʱ��AP��֤�ɹ�
//@PRECONDITION: 1�����Ի��������ٴ���һ����Կ����ΪEM_UDIWLAN_KEY_ANSI��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_ANSI;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0003(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,szEthName,sizeof(szEthName));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_ANSI;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"����2 ʧ��");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����3 ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:��Կ����ΪEM_UDIWLAN_KEY_STRINGʱ��AP��֤�ɹ�
//@PRECONDITION: 1�����Ի��������ٴ���һ����Կ����ΪEM_UDIWLAN_KEY_STRING��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_STRING;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0004(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,szEthName,sizeof(szEthName));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_STRING;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"����2 ʧ��");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����3 ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:��Կ��������AP��֤ʧ��
//@PRECONDITION: 1�����Ի��������ٴ���һ��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��;
//@INPUT:nWlanApInfoNum = 5;
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szInvalidKey�������Կ(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_STRING;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDIIPCFG_ERROR_CONNECTAP_ERROR
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0005(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;
	char szEthName[16];
	memset(szEthName,0,sizeof(szEthName));
	strcpy(szEthName, s_asWLanCfg[1].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,szEthName,sizeof(szEthName));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szInvalidKey,sizeof(szInvalidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_STRING;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
	CSTK_ASSERT_TRUE_FATAL(CSUDIIPCFG_ERROR_CONNECTAP_ERROR == nRetConnectAP,"����2 ʧ��");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����3 ʧ��");
	}
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:����������session id ������AP��֤ʧ��
//@PRECONDITION: 1�����Ի��������ٴ���һ��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = 0;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_STRING;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDIIPCFG_ERROR_CONNECTAP_ERROR
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0006(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memset(sWLanConf.szEssId ,0,sizeof(sWLanConf.szEssId));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_STRING;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"����2 ʧ��");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����3 ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:����������AP����֤�������͵�����AP��֤ʧ��
//@PRECONDITION: 1�����Ի��������ٴ���һ��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = 10(�������õ��ļ�������);
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_STRING;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDIIPCFG_ERROR_CONNECTAP_ERROR
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0007(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = 10;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_STRING;

	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"����2 ʧ��");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����3 ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:�ظ����ӵ�����AP��֤ʧ��
//@PRECONDITION: 1�����Ի��������ٴ���һ��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType = EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDIIPCFG_ERROR_CONNECTAP_ERROR
//@EXECUTIONFLOW: 4��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_ConnectAP_0008(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nRetConnectAP = -1;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;
	nRetConnectAP=CSUDIIPCFGConnectAP(&sWLanConf);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRetConnectAP,"����2 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGConnectAP(&sWLanConf),"����3 ʧ��");

	CSTK_FATAL_POINT
	if (CSUDI_SUCCESS == nRetConnectAP)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����4 ʧ��");
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
//@DESCRIPTION:��������"eth1"�ɹ���AP�Ͽ�����
//@PRECONDITION: 1�����Ի��������ٴ���һ��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION: �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_DisconnectAP_0001(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"����2 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����3 ʧ��");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGConnectAP
//@DESCRIPTION:����������Ϊ�Ƿ�ֵ��ʱ��AP�Ͽ�����ʧ��
//@PRECONDITION: 1�����Ի��������ٴ���һ��AP
//@PRECONDITION: 2��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo���洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:pcEthName = CSUDI_NULL;
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSearchAP(szEthName,&sWlanApInfo,&nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGConnectAP(&sWLanConf)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGDisconnectAP(pcEthName)����������CSUDI_FAILURE
CSUDI_BOOL CSTC_IPCFG_IT_DisconnectAP_0002(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"����2 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGDisconnectAP(CSUDI_NULL),"����3 ʧ��");
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
//@DESCRIPTION:�ɹ���ȡ��ȡAP�źŵ�������ǿ��
//@PRECONDITION:1�����Ի��������ٴ���һ��AP
//@PRECONDITION:2�����������������ӳɹ�
//@PRECONDITION:3��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo,�洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGGetAPSignalQulityStrength �������������:
//@INPUT:nWlanApSigQulity = 0�� �洢AP�źŵ�����
//@INPUT:nWlanApSigStrength = 0���洢AP�źŵ�ǿ��
//@INPUT:4��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:1��ִ��CSUDIIPCFGSearchAP(pcEthName,psWlanApInfo,nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��ִ��CSUDIIPCFGConnectAP(psWLanConf)����,����CSUDI_SUCCESS
//@EXECUTIONFLOW:3��ִ��CSUDIIPCFGGetAPSignalQulityStrength(&nWlanApSigQulity,&nWlanApSigStrength)����,����CSUDI_SUCCESS
//@EXECUTIONFLOW:4��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
//@REMARK: ���Ի���:����������AP���10�ף�����������AP���50�ף�����������AP֮�����һ��ǽ�������������벻ͬ��AP����Щ����²��Եõ����ź�������ǿ���Ƿ����
CSUDI_BOOL CSTC_IPCFG_IT_GetAPSignalQulityStrength_0001(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nWlanApSigQulity = 0;
	int nWlanApSigStrength = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"����2 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetAPSignalQulityStrength(&nWlanApSigQulity,&nWlanApSigStrength),"����3 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����4ʧ��");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetAPSignalQulityStrength
//@DESCRIPTION:AP�źŵ�������ǿ��Ϊ�Ƿ�ָ��ʱ��ȡAP�ź�ʧ��
//@PRECONDITION:1�����Ի��������ٴ���һ��AP
//@PRECONDITION:2�����������������ӳɹ�
//@PRECONDITION:3��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo,�洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGGetAPSignalQulityStrength �������������:
//@INPUT:pnWlanApSigQulity = CSUDI_NULL�� �洢AP�źŵ�����
//@INPUT:nWlanApSigStrength = 0���洢AP�źŵ�ǿ��
//@INPUT:4��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW:1��ִ��CSUDIIPCFGSearchAP(pcEthName,psWlanApInfo,nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��ִ��CSUDIIPCFGConnectAP(psWLanConf)����,����CSUDI_SUCCESS
//@EXECUTIONFLOW:3��ִ��CSUDIIPCFGGetAPSignalQulityStrength(pnWlanApSigQulity,&nWlanApSigStrength)����,���ش�����
//@EXECUTIONFLOW:4��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetAPSignalQulityStrength_0002(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nWlanApSigStrength = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"����2 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetAPSignalQulityStrength(CSUDI_NULL,&nWlanApSigStrength),"����3 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����4ʧ��");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetAPSignalQulityStrength
//@DESCRIPTION:AP�źŵ�������ǿ��Ϊ�Ƿ�ָ��ʱ��ȡAP�ź�ʧ��
//@PRECONDITION:1�����Ի��������ٴ���һ��AP
//@PRECONDITION:2�����������������ӳɹ�
//@PRECONDITION:3��������Ϊ"eth1"����������
//@INPUT:1��CSUDIIPCFGSearchAP �������������:
//@INPUT:szEthName[5] = "eth1"
//@INPUT:sWlanApInfo,�洢AP��Ϣ�Ľṹ��
//@INPUT:nWlanApInfoNum = 5
//@INPUT:2��CSUDIIPCFGConnectAP �������������:
//@INPUT:sWLanConf
//@INPUT:{	
//@INPUT:	szEthName[5] ="eth1";
//@INPUT:	szEssId = sWlanApInfo.szApName;
//@INPUT:	szKey = szValidKey(�ο������ļ�);
//@INPUT:	eEncType = sWlanApInfo.eApEncType;
//@INPUT:	eKeyType=EM_UDIWLAN_KEY_HEX;
//@INPUT:}
//@INPUT:3��CSUDIIPCFGGetAPSignalQulityStrength �������������:
//@INPUT:nWlanApSigQulity = 0�� �洢AP�źŵ�����
//@INPUT:pnWlanApSigStrength = CSUDI_NULL���洢AP�źŵ�ǿ��
//@INPUT:4��CSUDIIPCFGDisconnectAP �������������:
//@INPUT:szEthName[5] = "eth1";
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW:1��ִ��CSUDIIPCFGSearchAP(pcEthName,psWlanApInfo,nWlanApInfoNum)����������CSUDI_SUCCESS
//@EXECUTIONFLOW:2��ִ��CSUDIIPCFGConnectAP(psWLanConf)����,����CSUDI_SUCCESS
//@EXECUTIONFLOW:3��ִ��CSUDIIPCFGGetAPSignalQulityStrength(&nWlanApSigQulity,pnWlanApSigStrength)����,���ش�����
//@EXECUTIONFLOW:4��ִ��CSUDIIPCFGDisconnectAP(szEthName)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetAPSignalQulityStrength_0003(void)
{   	
	int nWlanApInfoNum = 5;
	CSUDIWLanApInfo_S sWlanApInfo;
	CSUDIWLanConfig_S sWLanConf;
	int nWlanApSigQulity = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSearchAP("eth1",&sWlanApInfo,&nWlanApInfoNum),"����1 ʧ��");

	memcpy(sWLanConf.szEthName ,"eth1",sizeof("eth1"));
	memcpy(sWLanConf.szEssId ,sWlanApInfo.szApName,sizeof(sWlanApInfo.szApName));
	memcpy(sWLanConf.szKey ,szValidKey,sizeof(szValidKey));
	sWLanConf.eEncType = sWlanApInfo.eApEncType;
	sWLanConf.eKeyType = EM_UDIWLAN_KEY_HEX;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGConnectAP(&sWLanConf),"����2 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetAPSignalQulityStrength(&nWlanApSigQulity,CSUDI_NULL),"����3 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGDisconnectAP("eth1"),"����4ʧ��");
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
//@DESCRIPTION:�ɹ�����DNS�ĵ�ַ
//@PRECONDITION: 1��Э��ջ��ʼ���ɹ�
//@PRECONDITION: 2������������Ϊ0��DNS
//@INPUT:nDNSIndex=0��DNS��������
//@INPUT:szDNS= g_szDNSIp(�ο������ļ�)  
//@INPUT:nLen=16�� DNS��IP��ַ�ַ�������
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)���������سɹ�
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:������Ϊ�Ƿ�ֵ4ʱ����DNS�ĵ�ַʧ��
//@PRECONDITION: 1��Э��ջ��ʼ���ɹ�
//@PRECONDITION: 2��������������Ϊ4��DNS
//@INPUT:nDNSIndex=4��DNS��������
//@INPUT:szDNS= g_szDNSIp(�ο������ļ�)  
//@INPUT:nLen=16�� DNS��IP��ַ�ַ�������
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)������ʧ�ܷ��ش�����
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(4, g_szDNSIp, 16), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:DNS IP��ַ��Ϊ�Ƿ�ֵ��ʱ����DNS��ַʧ��
//@PRECONDITION: 1��Э��ջ��ʼ���ɹ�
//@PRECONDITION: 2������������Ϊ0��DNS
//@INPUT:nDNSIndex=0��DNS��������
//@INPUT:pcDNS= CSUDI_NULL ��DNS��IP��ַ����Ϊ��
//@INPUT:nLen=16�� DNS��IP��ַ�ַ�������
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,pcDNS,nLen)������ʧ�ܷ��ش�����
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0004(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(0, CSUDI_NULL, 16), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:DNS��IP��ַ�ַ�������Ϊ15ʱ����DNS�ĵ�ַʧ��
//@PRECONDITION: 1��Э��ջ��ʼ���ɹ�
//@PRECONDITION: 2������������Ϊ0��DNS
//@INPUT:nDNSIndex=0��DNS��������
//@INPUT:szDNS= g_szDNSIp(�ο������ļ�)  
//@INPUT:nLen=15�� DNS��IP��ַ�ַ������ȣ���Ϊ15
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)������ʧ�ܷ��ش�����
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0005(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 15), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer 
//@DESCRIPTION:IP��ַ�Ƿ�ʱ����DNS�ĵ�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2������DNS������Ϊ0
//@INPUT:nDNSIndex=0��DNS��������
//@INPUT:szDNS= "a0.b1.c0.d9" ��DNS��IP��ַ����Ϊa0.b1.c0.d9
//@INPUT:nLen=16�� DNS��IP��ַ�ַ�������
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)����������ʧ��
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0006(void)
{
	char szDNSSet[] = "a0.b1.c0.d9";

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(0, szDNSSet, 16), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer 
//@DESCRIPTION:IP��ַ�Ƿ�ʱ����DNS�ĵ�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2������DNS������Ϊ0
//@INPUT:nDNSIndex=0��DNS��������
//@INPUT:szDNS= "10.10.98.256" ��DNS��IP��ַ����Ϊ10.10.98.256
//@INPUT:nLen=16�� DNS��IP��ַ�ַ�������
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)����������ʧ��
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0007(void)
{
	char szDNSSet[] = "10.10.98.256";

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetDNSServer(0, szDNSSet, 16), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetDNSServer
//@DESCRIPTION:DNS��IP��ַ�ַ���������Ϊ17ʱ����DNS�ĵ�ַ�ɹ�
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2������DNS������Ϊ0
//@INPUT:nDNSIndex=0��DNS��������
//@INPUT:szDNS= g_szDNSIp(�ο������ļ�)  
//@INPUT:nLen=17�� DNS��IP��ַ�ַ������ȣ���Ϊ17
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)���������سɹ�
CSUDI_BOOL CSTC_IPCFG_IT_SetDNSServer_0008(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 17), "����1ʧ��");

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
//@DESCRIPTION:�ɹ����DNS��ַ
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2������������Ϊ0��DNS
//@PRECONDITION:3�����Գɹ�����DNS�ĵ�ַ
//@INPUT:1��CSUDIIPCFGSetDNSServer �����Ĳ���˵��:
//@INPUT:nDNSIndex=0
//@INPUT:szDNS= g_szDNSIp(�ο������ļ�)  
//@INPUT:nLen=16
//@INPUT:2��CSUDIIPCFGGetDNSServer �����Ĳ���˵��:
//@INPUT:nDNSIndex = 0��DNS�������ţ���Ϊ0
//@INPUT:szDNS[16] �������洢DNS��IP��ַ
//@INPUT:nLen=sizeof(szDNS)�� DNS��IP��ַ�ַ�������
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetDNSServer(nDNSIndex,szDNS,nLen)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0001(void)
{
	char szDNS[16];

	memset(szDNS, 0, sizeof(szDNS));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetDNSServer(0, szDNS, 16), "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:�����Ų�ƥ��ʱ���DNS��ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2������������Ϊ0��DNS��������������Ϊ2��DNS
//@PRECONDITION:3�����Գɹ�����DNS�ĵ�ַ
//@INPUT:1��CSUDIIPCFGSetDNSServer �����Ĳ���˵��:
//@INPUT:nDNSIndex=0
//@INPUT:szDNS= g_szDNSIp(�ο������ļ�)  
//@INPUT:nLen=16
//@INPUT:2��CSUDIIPCFGGetDNSServer �����Ĳ���˵��:
//@INPUT:nDNSIndex = 2��DNS�������ţ���Ϊ2
//@INPUT:szDNS[16] �������洢DNS��IP��ַ
//@INPUT:nLen=sizeof(szDNS)�� DNS��IP��ַ�ַ�������
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetDNSServer(nDNSIndex,szDNS,nLen)���������ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0002(void)
{
	char szDNS[16];

	memset(szDNS, 0, sizeof(szDNS));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetDNSServer(CSUDI_ETHER_DNS_MAX_SUPPORT, szDNS, 16), "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:DNS��ַΪ�Ƿ�ֵ��ʱ���DNS��ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2������������Ϊ0��DNS
//@PRECONDITION:3�����Գɹ�����DNS�ĵ�ַ
//@INPUT:1��CSUDIIPCFGSetDNSServer �����Ĳ���˵��:
//@INPUT:nDNSIndex=0
//@INPUT:szDNS= g_szDNSIp(�ο������ļ�)  
//@INPUT:nLen=16
//@INPUT:2��CSUDIIPCFGGetDNSServer �����Ĳ���˵��:
//@INPUT:nDNSIndex = 0��DNS�������ţ���Ϊ0
//@INPUT:pcDNS = CSUDI_NULL 
//@INPUT:nLen=16
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetDNSServer(nDNSIndex,szDNS,nLen)���������ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetDNSServer(0, CSUDI_NULL, 16), "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:DNS��IP��ַ�ַ���������Ϊ�Ƿ�ֵ15ʱ���DNS�ĵ�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2������������Ϊ0��DNS
//@PRECONDITION:3�����Գɹ�����DNS�ĵ�ַ
//@INPUT:1��CSUDIIPCFGSetDNSServer �����Ĳ���˵��:
//@INPUT:nDNSIndex=0
//@INPUT:szDNS= g_szDNSIp(�ο������ļ�)  
//@INPUT:nLen=16
//@INPUT:2��CSUDIIPCFGGetDNSServer �����Ĳ���˵��:
//@INPUT:nDNSIndex = 0��DNS�������ţ���Ϊ0
//@INPUT:pcDNS 
//@INPUT:nLen=15�� DNS��IP��ַ�ַ�������
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetDNSServer(nDNSIndex,szDNS,nLen)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetDNSServer(nDNSIndex,pcDNS,nLen)���������ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0004(void)
{
	char szDNS[16];

	memset(szDNS, 0, sizeof(szDNS));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetDNSServer(0, g_szDNSIp, 16), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetDNSServer(0, szDNS, 15), "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetDNSServer
//@DESCRIPTION:�������õ�dns�ͻ�ȡ��dns�Ƿ�һ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2������������Ϊ0��DNS
//@PRECONDITION:2������������Ϊ1��DNS
//@EXECUTIONFLOW: 1������CSUDIIPCFGSetDNSServer(0, "192.168.99.4", 16) ���õ�һ��dns1
//@EXECUTIONFLOW: 2������CSUDIIPCFGSetDNSServer(0, "192.168.99.5", 16) ���õڶ���dns2
//@EXECUTIONFLOW: 3������CSUDIIPCFGGetDNSServer��ȡ��һ��dns1
//@EXECUTIONFLOW: 4������CSUDIIPCFGGetDNSServer��ȡ�ڶ���dns2
//@EXECUTIONFLOW: 5���Ƚϵڻ�ȡ�ĵ�һ��dns1�����õĵ�һ��dns1�Ƿ����
//@EXECUTIONFLOW: 6���Ƚϵڻ�ȡ�ĵڶ���dns2�����õĵڶ���dns2�Ƿ����
CSUDI_BOOL CSTC_IPCFG_IT_GetDNSServer_0005(void)
{
	int nRet = -1;
	char acDNS1[16] = {0};
	char acDNS2[16] = {0};

	nRet = CSUDIIPCFGSetDNSServer(0, "192.168.99.4", 16);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "����1ʧ��");

	nRet = CSUDIIPCFGSetDNSServer(1, "192.168.99.5", 16);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "����2ʧ��");

	nRet = CSUDIIPCFGGetDNSServer(0, acDNS1, 16);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "����3ʧ��");

	nRet = CSUDIIPCFGGetDNSServer(1, acDNS2, 16);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == nRet, "����4ʧ��");

	nRet = memcmp(acDNS1, "192.168.99.4", strlen("192.168.99.4"));
	CSTK_ASSERT_TRUE_FATAL(0 == nRet, "����5ʧ��");

	nRet = memcmp(acDNS2, "192.168.99.5", strlen("192.168.99.5"));
	CSTK_ASSERT_TRUE_FATAL(0 == nRet, "����6ʧ��");

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
//@DESCRIPTION:�ɹ�����������IP��ַ��������������ص�ַ
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2 ��szIp="192.168.1.1" 
//@INPUT:3 ��szMask="255.255.255.0"
//@INPUT:4 ��szGateWay="192.168.1.254"
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,szGateWay)����
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0001(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "����1ʧ��");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:��������Ƿ�ʱ����������IP��ַ��������������ص�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2 ��szIp="192.168.1.1" 
//@INPUT:3 ��szMask="255.255.255.255"
//@INPUT:4 ��szGateWay="192.168.1.254"
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,szGateWay)������ʧ�ܷ��ش������ֵ��
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0002(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", "255.255.255.255", "192.168.1.254"), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:IP��ַ�Ƿ�ʱ����������IP��ַ��������������ص�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2 ��szIp="192.168.1.256" 
//@INPUT:3 ��szMask="255.255.255.0"
//@INPUT:4 ��szGateWay="192.168.1.254"
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,szGateWay)������ʧ�ܷ��ش������ֵ��
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0003(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.256", "255.255.255.0", "192.168.1.254"), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:���طǷ�ʱ����������IP��ַ��������������ص�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2 ��szIp="192.168.1.1" 
//@INPUT:3 ��szMask="255.255.255.0"
//@INPUT:4 ��szGateWay="192.168.1.256"
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,szGateWay)�������ɹ�����CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0004(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.256"), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:����Ϊ�Ƿ�ֵ��ʱ����������IP��ַ��������������ص�ַ�ɹ�
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2 ��szIp="192.168.1.1" 
//@INPUT:3 ��szMask="255.255.255.0"
//@INPUT:4 ��pcGateWay=CSUDI_NULL
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szEthName,szIp,szMask,pcGateWay)�������ɹ�����CSUDI_SUCCESS��
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0005(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", "255.255.255.0", CSUDI_NULL), "����1ʧ��");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:����Ϊ�Ƿ�ֵ��ʱ����������IP��ַ��������������ص�ַ�ɹ�
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2 ��szIp="192.168.1.1" 
//@INPUT:3 ��pcMask=CSUDI_NULL
//@INPUT:4 ��szGateWay="192.168.1.254"
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szEthName,szIp,pcMask,szGateWay)�������ɹ�����CSUDI_SUCCESS��
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0006(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "192.168.1.1", CSUDI_NULL, "192.168.1.254"), "����1ʧ��");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:IPΪ�Ƿ�ֵ��ʱ����������IP��ַ��������������ص�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2 ��pcIp=CSUDI_NULL
//@INPUT:3 ��szMask="255.255.255.0"
//@INPUT:4 ��szGateWay="192.168.1.254"
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szEthName,pcIp,szMask,szGateWay)������ʧ�ܷ��ش������ֵ��
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0007(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(szEthName, CSUDI_NULL, "255.255.255.0", "192.168.1.254"), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:������Ϊ�Ƿ�ֵ��ʱ����������IP��ַ��������������ص�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@INPUT:1�� pcEthName=CSUDI_NULL
//@INPUT:2 ��szIp="192.168.1.1" 
//@INPUT:3 ��szMask="255.255.255.0"
//@INPUT:4 ��szGateWay="192.168.1.254"
//@EXPECTATION:  ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(pcEthName,szIp,szMask,szGateWay)������ʧ�ܷ��ش������ֵ��
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0008(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGSetIpInfo(CSUDI_NULL, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:IP��ַ�������������ó�0.0.0.0ʱ����������ͨ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� pcEthName=CS_LAN0_NAME
//@INPUT:2 ��szIp="0.0.0.0" 
//@INPUT:3 ��szMask="0.0.0.0"
//@INPUT:4 ��szGateWay="192.168.1.254"
//@EXPECTATION:  ʧ�ܷ���CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(pcEthName,szIp,szMask,szGateWay)�������ɹ�����CSUDI_SUCCESS��
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0009(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "0.0.0.0", "0.0.0.0", "192.168.1.254"), "����1ʧ��");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGSetIpInfo
//@DESCRIPTION:�����������ó�255.255.255.128ʱ����������ͨ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@INPUT:1�� pcEthName=CS_LAN0_NAME
//@INPUT:2 ��szIp="10.135.255.188" 
//@INPUT:3 ��szMask="255.255.255.128"
//@INPUT:4 ��szGateWay="10.135.255.129"
//@EXPECTATION:  ʧ�ܷ���CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(pcEthName,szIp,szMask,szGateWay)�������ɹ�����CSUDI_SUCCESS��
CSUDI_BOOL CSTC_IPCFG_IT_SetIpInfo_0010(void)
{
	char szEthName[16];

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(szEthName, "10.135.255.188", "255.255.255.128", "10.135.255.129"), "����1ʧ��");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End Test SetIpInfo *************************//
//**********************************************************************************//


//**********************************************************************************//
//******************************Begin Test  GetIpInfo*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:�ɹ���ȡ������IP��ַ��������������ص�ַ
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@PRECONDITION:3�����Գɹ�����������IP��ַ��������������ص�ַ
//@INPUT:1��CSUDIIPCFGSetIpInfo �����Ĳ���˵��:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2��CSUDIIPCFGGetIpInfo �����Ĳ���˵��:
//@INPUT:szGetEthName=CS_LAN0_NAME
//@INPUT:szGetIp[16]�� �����ȡ��IP��ַ
//@INPUT:szGetMask[16]��������������
//@INPUT:szGetGateWay[16]���������ص�ַ
//@EXPECTATION:  �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetIpInfo(szGetEthName,szGetIp,szGetMask,szGetGateWay)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0001(void)
{
	char szGetIp[16];
	char szGetMask[16];
	char szGetGateWay[16];

	memset(szGetIp, 0, sizeof(szGetIp));
	memset(szGetMask, 0, sizeof(szGetMask));
	memset(szGetGateWay, 0, sizeof(szGetGateWay));

	/* ��Ϊ "192.168.010.1" ���Լ�������Ƿ�����8����ת�� */
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.010.1" , "255.255.255.0", "192.168.010.254"), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, szGetIp, szGetMask, szGetGateWay), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetIp, "192.168.10.1", sizeof("192.168.10.1")), "��õ�IP�����õĲ�ͬ");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetMask, "255.255.255.0", sizeof("255.255.255.0")), "��õ��������������õĲ�ͬ");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetGateWay, "192.168.10.254", sizeof("192.168.10.254")), "��õ����ص�ַ�����õĲ�ͬ");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:������Ϊ�Ƿ�ֵ��ʱ��ȡ������IP��ַ��������������ص�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@PRECONDITION:3�����Գɹ�����������IP��ַ��������������ص�ַ
//@INPUT:1��CSUDIIPCFGSetIpInfo �����Ĳ���˵��:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2��CSUDIIPCFGGetIpInfo �����Ĳ���˵��:
//@INPUT:pcGetEthName=CSUDI_NULL
//@INPUT:szGetIp[16]�� �����ȡ��IP��ַ
//@INPUT:szGetMask[16]��������������
//@INPUT:szGetGateWay[16]���������ص�ַ
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetIpInfo(pcGetEthName,szGetIp,szGetMask,szGetGateWay)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0002(void)
{
	char szGetIp[16];
	char szGetMask[16];
	char szGetGateWay[16];

	memset(szGetIp, 0, sizeof(szGetIp));
	memset(szGetMask, 0, sizeof(szGetMask));
	memset(szGetGateWay, 0, sizeof(szGetGateWay));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetIpInfo(CSUDI_NULL, szGetIp, szGetMask, szGetGateWay), "����2ʧ��");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:ָ��Ϊ��ʱ��ȡ������IP��ַ��������������ص�ַʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@PRECONDITION:3�����Գɹ�����������IP��ַ��������������ص�ַ
//@INPUT:1��CSUDIIPCFGSetIpInfo �����Ĳ���˵��:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2��CSUDIIPCFGGetIpInfo �����Ĳ���˵��:
//@INPUT:szGetEthName=CS_LAN0_NAME
//@INPUT:pcGetIp=CSUDI_NULL
//@INPUT:pcGetMask=CSUDI_NULL
//@INPUT:pcGetGateWay=CSUDI_NULL
//@EXPECTATION: ʧ�ܷ���CSUDI_FAILURE
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetIpInfo(pcGetIp,szGetIp,pcGetMask,pcGetGateWay)������ʧ�ܷ��ش������ֵ
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, CSUDI_NULL, CSUDI_NULL, CSUDI_NULL), "����2ʧ��");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:�ɹ���ȡ������IP��ַ����������
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@PRECONDITION:3�����Գɹ�����������IP��ַ��������������ص�ַ
//@INPUT:1��CSUDIIPCFGSetIpInfo �����Ĳ���˵��:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2��CSUDIIPCFGGetIpInfo �����Ĳ���˵��:
//@INPUT:szGetEthName=CS_LAN0_NAME
//@INPUT:szGetIp[16]�� �����ȡ��IP��ַ
//@INPUT:szGetMask[16]��������������
//@INPUT:pcGetGateWay=CSUDI_NULL
//@EXPECTATION: �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetIpInfo(szGetEthName,szGetIp,szGetMask,pcGetGateWay)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0004(void)
{
	char szGetIp[16];
	char szGetMask[16];

	memset(szGetIp, 0, sizeof(szGetIp));
	memset(szGetMask, 0, sizeof(szGetMask));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, szGetIp, szGetMask, CSUDI_NULL), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetIp, "192.168.1.1", sizeof("192.168.1.1")), "��õ�IP�����õĲ�ͬ");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetMask, "255.255.255.0", sizeof("255.255.255.0")), "��õ��������������õĲ�ͬ");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetIpInfo
//@DESCRIPTION:�ɹ���ȡ������IP��ַ
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2��������������
//@PRECONDITION:3�����Գɹ�����������IP��ַ��������������ص�ַ
//@INPUT:1��CSUDIIPCFGSetIpInfo �����Ĳ���˵��:
//@INPUT:szSetEthName=CS_LAN0_NAME
//@INPUT:szSetIp="192.168.1.1"
//@INPUT:szSetMask="255.255.255.0"
//@INPUT:szSetGateWay="192.168.1.254"
//@INPUT:2��CSUDIIPCFGGetIpInfo �����Ĳ���˵��:
//@INPUT:szGetEthName=CS_LAN0_NAME
//@INPUT:szGetIp[16]�� �����ȡ��IP��ַ
//@INPUT:pcGetMask = CSUDI_NULL
//@INPUT:pcGetGateWay=CSUDI_NULL
//@EXPECTATION: �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGSetIpInfo(szSetEthName,szSetIp,szSetMask,szSetGateWay)����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetIpInfo(szGetEthName,szGetIp,szGetMask,pcGetGateWay)����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_IPCFG_IT_GetIpInfo_0005(void)
{
	char szGetIp[16];

	memset(szGetIp, 0, sizeof(szGetIp));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "192.168.1.1", "255.255.255.0", "192.168.1.254"), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, szGetIp, CSUDI_NULL, CSUDI_NULL), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == memcmp(szGetIp, "192.168.1.1", sizeof("192.168.1.1")), "��õ�IP�����õĲ�ͬ");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IPCFG_iSetDefault(), "�ָ�Ĭ��IP����");

	return CSUDI_TRUE;
}

//**********************************************************************************//
//******************************End  Test GetIpInfo *******************************//
//**********************************************************************************//

//**********************************************************************************//
//******************************Begin Test  GetStatInfo*******************************//
//**********************************************************************************//
//@CASEGROUP:CSUDIIPCFGGetStatInfo
//@DESCRIPTION:�ɹ���ȡ����ͳ����Ϣ
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@PRECONDITION:2����������
//@INPUT:szEthName=CS_LAN0_NAME
//@EXPECTATION: �ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1���ж����ٴ���һ������
//@EXECUTIONFLOW: 2��ѭ��ִ��CSUDIIPCFGGetStatInfo(szEthName,&sEtherStat)�����������ɹ���ȡ��������ͳ����Ϣ
//@EXECUTIONFLOW: 3��ѭ��ִ��CSUDIIPCFGGetStatInfo(szEthName,&sEtherStat)�����������ɹ���ȡ��������ͳ����Ϣ
CSUDI_BOOL CSTC_IPCFG_IT_GetStatInfo_0001(void)
{
	int i = 0;
	CSUDIEtherStat_S sEtherStat;

	CSTK_ASSERT_TRUE_FATAL((s_nActLanCount+s_nActWLanCount) > 0, "û������");

	for(i=0; i<s_nActLanCount; i++)
	{
		memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetStatInfo(s_asLanCfg[i].szEthName, &sEtherStat), "��ȡ��������ͳ����Ϣʧ��");

		CSTCPrint("interface:%s, connect time:%d, send Byte:%d, recv Byte:%d, send Packet:%d, recv Packet:%d\n", 
			s_asLanCfg[i].szEthName, sEtherStat.uConnectTime, 
			sEtherStat.uTotalBytesSend, sEtherStat.uTotalBytesReceived, 
			sEtherStat.uTotalPacketSend, sEtherStat.uTotalPacketReceived);
	}

	for(i=0; i<s_nActWLanCount; i++)
	{
		memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetStatInfo(s_asWLanCfg[i].szEthName, &sEtherStat), "��ȡ��������ͳ����Ϣʧ��");

		CSTCPrint("interface:%s, connect time:%d, send Bytes:%d, recv Bytes:%d, send Packets:%d, recv Packets:%d\n", 
			s_asWLanCfg[i].szEthName, sEtherStat.uConnectTime, 
			sEtherStat.uTotalBytesSend, sEtherStat.uTotalBytesReceived, 
			sEtherStat.uTotalPacketSend, sEtherStat.uTotalPacketReceived);
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetStatInfo
//@DESCRIPTION:������Ϊ�գ���ȡ����ͳ����Ϣʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@INPUT:������Ϊ��:
//@EXPECTATION: ʧ�ܷ��ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetStatInfo(CSUDI_NULL,&sEtherStat)����������ִ��ʧ��
CSUDI_BOOL CSTC_IPCFG_IT_GetStatInfo_0002(void)
{
	CSUDIEtherStat_S sEtherStat;

	memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetStatInfo(CSUDI_NULL, &sEtherStat), "��ȡ��������ͳ����Ϣ��Ӧ�óɹ�");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetStatInfo
//@DESCRIPTION:����ͳ����Ϣ�ṹ��Ϊ�գ���ȡ����ͳ����Ϣʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@INPUT:����������:
//@EXPECTATION: ʧ�ܷ��ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetStatInfo(szEthName, CSUDI_NULL)����������ִ��ʧ��
CSUDI_BOOL CSTC_IPCFG_IT_GetStatInfo_0003(void)
{
	CSUDIEtherStat_S sEtherStat;

	CSTK_ASSERT_TRUE_FATAL(s_nActLanCount > 0, "û����������");
	memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetStatInfo(s_asLanCfg[0].szEthName, CSUDI_NULL), "��ȡ��������ͳ����Ϣ��Ӧ�óɹ�");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIIPCFGGetStatInfo
//@DESCRIPTION:�����ڵ�����������ȡ����ͳ����Ϣʧ��
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@INPUT:�����ڵ�������:
//@EXPECTATION: ʧ�ܷ��ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ִ��CSUDIIPCFGGetStatInfo("Eth0",&sEtherStat)����������ִ��ʧ��
CSUDI_BOOL CSTC_IPCFG_IT_GetStatInfo_0004(void)
{
	CSUDIEtherStat_S sEtherStat;

	memset(&sEtherStat, 0x00, sizeof(CSUDIEtherStat_S));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIIPCFGGetStatInfo("Eth0", &sEtherStat), "��ȡ��������ͳ����Ϣ��Ӧ�óɹ�");

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
//@DESCRIPTION:������������ֱ�ӷ����ɹ�
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2�� fd = -1
//@INPUT:3�� nFlag = 1
//@EXPECTATION: �ɹ�����0
//@EXECUTIONFLOW: 1��ִ��CSUDIRawSendPacket(szEthName, &nSockRet, (char *)(&acSendBuf), sizeof(acSendBuf), 1)����
//@EXECUTIONFLOW: 2���жϷ���ֵ������Ϊ0���ж�fd������������-1
//@EXECUTIONFLOW: 3��ִ��CSUDISOCKClose(nFd)��������0
CSUDI_BOOL CSTC_IPCFG_RAWSOCKET_0001(void)
{
	int nFd = -1;
	char szEthName[16];
	char acSendBuf[548] = {0};
	int nRet = -1;

	memset(szEthName, 0, sizeof(szEthName));
	strcpy(szEthName, s_asLanCfg[0].szEthName);

	nRet = CSUDIRawSendPacket(szEthName, &nFd, (char *)(&acSendBuf), sizeof(acSendBuf), 1);
	CSTK_ASSERT_TRUE_FATAL(nRet == 0, "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nFd != -1, "����2ʧ��");

	CSTK_FATAL_POINT
	if(nFd != -1 )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISOCKClose(nFd), "�ر��׽���ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRawSendPacketByMode
//@DESCRIPTION:������������ֱ�ӷ����ɹ�
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2�� fd = -1
//@INPUT:3�� nFlag = 1
//@INPUT:4�� eRawSendMode = EM_UDIRAWSENDTYPE_UNICAST
//@EXPECTATION: �ɹ�����0
//@EXECUTIONFLOW: 1��ִ��CSUDIRawSendPacketByMode(szEthName, &nSockRet, (char *)(&acSendBuf), sizeof(acSendBuf), 1, EM_UDIRAWSENDTYPE_UNICAST)����
//@EXECUTIONFLOW: 2���жϷ���ֵ������Ϊ0���ж�fd������������-1
//@EXECUTIONFLOW: 3��ִ��CSUDISOCKClose(nFd)��������0
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
	CSTK_ASSERT_TRUE_FATAL(nRet == 0, "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nFd != -1, "����2ʧ��");

	CSTK_FATAL_POINT
	if(nFd != -1 )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISOCKClose(nFd), "�ر��׽���ʧ��");
	}
#endif
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRawRecvPacket
//@DESCRIPTION:������������ֱ�ӽ��հ��ɹ�
//@PRECONDITION:1��Э��ջ��ʼ���ɹ�
//@INPUT:1�� szEthName=CS_LAN0_NAME
//@INPUT:2�� fd = -1
//@INPUT:3�� nFlag = 1
//@INPUT:4�� eRawSendMode = EM_UDIRAWSENDTYPE_UNICAST
//@EXPECTATION: �ɹ�����0
//@EXECUTIONFLOW: 1����ʾ��pcһֱping������
//@EXECUTIONFLOW: 2��ִ��CSUDIRawSendPacketByMode(szEthName, &nFd, (char *)(&acSendBuf), sizeof(acSendBuf), 1)����
//@EXECUTIONFLOW: 3���жϷ���ֵ������Ϊ0���ж�fd������������-1
//@EXECUTIONFLOW: 4��ִ��CSUDIRawRecvPacket(szEthName, &nFd, (char *)(&acRecvBuf), &nRecvLen, 1)����
//@EXECUTIONFLOW: 5���жϷ���ֵ����������0
//@EXECUTIONFLOW: 6��ִ��CSUDISOCKClose(nFd)��������0
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
			"�������ļ��л�ȡ������IP������ʧ��")

	CSTCPrint("��ʹ��pcһֱping������(example: ping 172.30.18.195 -t ��֤��pingͨ)\n");
	CSTCPrint("�����ļ������õĻ�����IPΪ��%s\n", acStbIP);
	CSTKWaitAnyKey();

	nRet = CSUDIRawSendPacket(szEthName, &nFd, (char *)(&acSendBuf), sizeof(acSendBuf), 1);
	CSTK_ASSERT_TRUE_FATAL(nRet == 0, "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nFd != -1, "����2ʧ��");

	nRet = CSUDIRawRecvPacket(szEthName, &nFd, (char *)(&acRecvBuf), &nRecvLen, 1);
	CSTCPrint("nRet = %d\n", nRet);
	CSTK_ASSERT_TRUE_FATAL(nRet > 0, "����3ʧ��");

	CSTK_FATAL_POINT
	if(nFd != -1 )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISOCKClose(nFd), "�ر��׽���ʧ��");
	}
	CSTCPrint("����ֹpc�ϵ�ping����\n");
	CSTKWaitAnyKey();

	return CSUDI_TRUE;
}
//**********************************************************************************//
//******************************End  Test RawPacket ********************************//
//**********************************************************************************//


