#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include "cs_dhcpstestcase.h"
#include "udi2_dhcpserver.h"

#define MAX_NIC_LEN (16)
#define MAX_IP_LEN (16)
#define DHCPS_NETWORK_SEGMENT_COUNT (2)
static struct 
{
	char szNic[MAX_NIC_LEN];
	char szInvalidNic[MAX_NIC_LEN];
	char szNicIp[DHCPS_NETWORK_SEGMENT_COUNT][MAX_IP_LEN]; //nic��ַ���������Ӧ
	CSUDIDHCPSConfig_S sConfig[DHCPS_NETWORK_SEGMENT_COUNT];
	CSUDIDHCPSStaticIpCfg sStaticIpCfg; //����һ�龲̬Ip��Ϣ
}g_sDhcpsInfo;

static int  set_nic_ip(char *szNic,char *szIP)
{
	int nSockId = 0;
	struct ifreq ifr;
	struct   sockaddr_in   *sin; 

	memset(&ifr, 0x00, sizeof(ifr));
	strncpy(ifr.ifr_name, szNic, sizeof(ifr.ifr_name)-1);

	if ((nSockId = socket(AF_INET, SOCK_DGRAM,0)) < 0)
	{
		return -1;
	}
	sin = (struct   sockaddr_in*)&ifr.ifr_addr;
	sin->sin_addr.s_addr = inet_addr(szIP);
	sin->sin_family = AF_INET;

	if (ioctl(nSockId, SIOCSIFADDR, &ifr) < 0)
	{
		close(nSockId);
		return -1;
	}
	return 0;	
}

static int char2int(char ch)
{
	if (ch >='a' && ch <= 'f')
	{
		return ch - 'a' + 10; 
	}
	else if (ch >='A' && ch <= 'F')
	{
		return ch - 'A' + 10;
	}
	else
	{
		return ch - '0';
	}	
}

static void DHCPS_Init_Cfg(void)
{
	int i = 0;
	char szLeaseTime[32];
	char szMac[32];
	char szkey[32];
	char *pszTemp;
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS","DHCPS0_NIC",g_sDhcpsInfo.szNic,MAX_NIC_LEN))
	{
		CSTCPrint("DHCPS0_NIC is %s\n",g_sDhcpsInfo.szNic);
	}

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS","INVLID_NIC",g_sDhcpsInfo.szInvalidNic,MAX_NIC_LEN))
	{
		CSTCPrint("INVLID_NIC is %s\n",g_sDhcpsInfo.szInvalidNic);
	}
	
	for (i = 0; i < DHCPS_NETWORK_SEGMENT_COUNT;i++)
	{
		sprintf(szkey, "DHCPS%d_NIC_IP", i);
		if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS",szkey,g_sDhcpsInfo.szNicIp[i],MAX_IP_LEN))
		{
			CSTCPrint("%s is %s\n",szkey, g_sDhcpsInfo.szNicIp[i]);
		}

		sprintf(szkey, "DHCPS%d_IP_START", i);
		if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS",szkey,g_sDhcpsInfo.sConfig[i].acIPStart,MAX_IP_LEN))
		{
			CSTCPrint("%s is %s\n",szkey, g_sDhcpsInfo.sConfig[i].acIPStart);
		}
		
		sprintf(szkey, "DHCPS%d_IP_END", i);
		if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS",szkey,g_sDhcpsInfo.sConfig[i].acIPEnd,MAX_IP_LEN))
		{
			CSTCPrint("%s is %s\n",szkey, g_sDhcpsInfo.sConfig[i].acIPEnd);
		}

		sprintf(szkey, "DHCPS%d_MASK", i);
		if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS",szkey,g_sDhcpsInfo.sConfig[i].acMask,MAX_IP_LEN))
		{
			CSTCPrint("%s is %s\n",szkey, g_sDhcpsInfo.sConfig[i].acMask);
		}

		sprintf(szkey, "DHCPS%d_GATEWAY", i);
		if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS",szkey,g_sDhcpsInfo.sConfig[i].acGatway,MAX_IP_LEN))
		{
			CSTCPrint("%s is %s\n",szkey, g_sDhcpsInfo.sConfig[i].acGatway);
		}

		sprintf(szkey, "DHCPS%d_FIRSTDNS", i);
		if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS",szkey,g_sDhcpsInfo.sConfig[i].acFirstDNS,MAX_IP_LEN))
		{
			CSTCPrint("%s is %s\n",szkey, g_sDhcpsInfo.sConfig[i].acFirstDNS);
		}

		sprintf(szkey, "DHCPS%d_SECONDDNS", i);
		if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS",szkey,g_sDhcpsInfo.sConfig[i].acSecondDNS,MAX_IP_LEN))
		{
			CSTCPrint("%s is %s\n",szkey, g_sDhcpsInfo.sConfig[i].acSecondDNS);
		}

		sprintf(szkey, "DHCPS%d_LEASETIME", i);
		if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS",szkey,szLeaseTime,MAX_IP_LEN))
		{
			CSTCPrint("%s is %s\n",szkey, szLeaseTime);
		}
		g_sDhcpsInfo.sConfig[i].uLeaseTime = (unsigned int)CSTKGetIntFromStr(szLeaseTime, 10);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS","STATIC_IP",g_sDhcpsInfo.sStaticIpCfg.acIP,MAX_IP_LEN))
	{
		CSTCPrint("%s is %s\n",szkey, g_sDhcpsInfo.sStaticIpCfg.acIP);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("DHCPS","STATIC_MAC",szMac,sizeof(szMac)))
	{
		CSTCPrint("%s is %s\n",szkey, szMac);
	}

	//��ȡ��̬ip mac��
	i = 0;
	pszTemp = strtok(szMac, ":-");
	g_sDhcpsInfo.sStaticIpCfg.aucMac[i++] = 16*char2int(pszTemp[0]) + char2int(pszTemp[1]);	
	while(CSUDI_NULL != (pszTemp = strtok(NULL,":-")) && i < CSUDI_MAC_LEN)
	{
		g_sDhcpsInfo.sStaticIpCfg.aucMac[i++] = 16*char2int(pszTemp[0]) + char2int(pszTemp[1]);		
	}
}

CSUDI_BOOL CSTC_DHCPS_Init(void)
{
	DHCPS_Init_Cfg();
	return CSUDI_TRUE;
}
CSUDI_BOOL CSTC_DHCPS_UnInit(void)
{
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDHCPSStart&CSUDIDHCPSStop
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIDHCPSStart������ִ��Ч��
//@PRECONDITION:
//@INPUT:1��szIfName�������������Ҫ��
//@EXPECTATION:���÷��ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪNULL������CSUDIDHCPSStart���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szIfNameΪ�����ڵ��豸(��"eth100")������CSUDIDHCPSStart���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szIfNameΪ�ػ��豸����(��"lo")������CSUDIDHCPSStart���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSStartStop_0001(void)
{
	char szIfName[16];

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStart(CSUDI_NULL),
		"����1ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStart(g_sDhcpsInfo.szInvalidNic),
		"����2ʧ��");
	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStart(szIfName),
		"����3ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSStart&CSUDIDHCPSStop
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIDHCPSStop������ִ��Ч��
//@PRECONDITION:
//@INPUT:1��szIfName�������������Ҫ��
//@EXPECTATION:���÷��ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪNULL������CSUDIDHCPSStop���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szIfNameΪ�����ڵ��豸(��"eth100")������CSUDIDHCPSStop���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szIfNameΪ�ػ��豸����(��"lo")������CSUDIDHCPSStop���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSStartStop_0002(void)
{
	char szIfName[16];

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStop(CSUDI_NULL),
		"����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStop(g_sDhcpsInfo.szInvalidNic),
		"����2ʧ��");
	
	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStop(szIfName),
		"����3ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSStart&CSUDIDHCPSStop&CSUDIDHCPSConfig
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIDHCPSStart&CSUDIDHCPSStop������ִ��Ч��
//@PRECONDITION:
//@INPUT:1��szIfName�����������Ҫ��
//@INPUT:2��psDHCPCfg�����������Ҫ��
//@EXPECTATION:DHCPS������ȷ��������ֹͣ
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪ��ȷ����������(��"eth1"�� IP:172.30.2.1)
//@EXECUTIONFLOW:2������psDHCPCfg������ȷ��(��acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3������CSUDIDHCPSConfig����������DHCPS����
//@EXECUTIONFLOW:4������CSUDIDHCPSStart����������DHCPS
//@EXECUTIONFLOW:5��PC0��eth1���ӣ���PC0��������Etherealץ�����ߣ�����pc0�Ϻ�eth1���ӵ�����
//@EXECUTIONFLOW:6��PC0��̬��ȡIP��ַ���鿴Ethereal��������ݰ�
//@EXECUTIONFLOW:7���Ƚ�Ethereal��������ݰ���psDHCPCfg�����Ƿ�һ��
//@EXECUTIONFLOW:8������CSUDIDHCPSStop������ֹͣDHCPS
//@EXECUTIONFLOW:9����eth1���ӵ�PC0 ���ܹ�������ȡ��IP��ַ
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSStartStop_0003(void)
{
	char szIfName[16];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIDHCPSConfig_S sDHCPCfg;
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	//����ip��ַ
	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(szIfName, g_sDhcpsInfo.szNicIp[0]), 
		"����1ʧ��");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPEnd);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName),
		"����3ʧ��");
	bStart = CSUDI_TRUE;

	CSTCPrint("���ڵ�DHCPS��ϢΪ:\nstartip=%s  \nendip=%s \nmask=%s  \ngataway=%s  \n", 
		g_sDhcpsInfo.sConfig[0].acIPStart, 
		g_sDhcpsInfo.sConfig[0].acIPEnd,
		g_sDhcpsInfo.sConfig[0].acMask, 
		g_sDhcpsInfo.sConfig[0].acGatway);
	
	CSTCPrint("1:PC0����ʹ��Etherealץ�����ߣ�����pc0�Ϻ�eth1���ӵ�����\n");
    CSTCPrint("2:pc0�Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
	CSTCPrint("Etherealץ��İ��Ƿ�����õ�DHCPS��Ϣһ��?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����4ʧ�ܣ���ȡ��Ϣ����ȷ");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), 
		"����5ʧ��");
	bStart = CSUDI_FALSE;

	CSTCPrint("DHCPS��������ֹͣ����Ͽ�pc0��eth1�����ӣ���������ip��ַ\n");
    CSTCPrint("1:pc0�Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
	CSTCPrint("pc0 �Ƿ���䵽ip��ַ?\n");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����6ʧ�ܣ���ȡ��Ϣ����ȷ");
	
	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),
			"����7ʧ��");
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSStart&CSUDIDHCPSStop&CSUDIDHCPSConfig
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIDHCPSStart&CSUDIDHCPSStop������ִ��1000��Ч��
//@PRECONDITION:
//@INPUT:1��szIfName�����������Ҫ��
//@INPUT:2��psDHCPCfg�����������Ҫ��
//@EXPECTATION:DHCPS���п���ǿ�Ȳ��Ժ��ܹ���������
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪ��ȷ����������(��"eth1"�� IP:172.30.2.1)
//@EXECUTIONFLOW:2������psDHCPCfg������ȷ��(��acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3������CSUDIDHCPSConfig����������DHCPS����
//@EXECUTIONFLOW:4������CSUDIDHCPSStart��CSUDIDHCPSStop����1000��
//@EXECUTIONFLOW:5������CSUDIDHCPSStart����,����DHCPS������
//@EXECUTIONFLOW:6��PC0��eth1���ӣ���PC0��������Etherealץ�����ߣ�����pc0�Ϻ�eth1���ӵ�����
//@EXECUTIONFLOW:7��PC0��̬��ȡIP��ַ���鿴Ethereal��������ݰ�
//@EXECUTIONFLOW:8���Ƚ�Ethereal��������ݰ���psDHCPCfg�����Ƿ�һ��
//@EXECUTIONFLOW:9������CSUDIDHCPSStop������ֹͣDHCPS
//@EXECUTIONFLOW:10����eth1���ӵ�PC0 ���ܹ�������ȡ��IP��ַ
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSStartStop_0004(void)
{
	int nCount = 0;
	char szIfName[16];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIDHCPSConfig_S sDHCPCfg;
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(szIfName, g_sDhcpsInfo.szNicIp[0]), 
		"����1ʧ��");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPEnd);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"����2ʧ��");

	CSTCPrint("��ʼ����ǿ�Ȳ���\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����3ʧ�ܣ�������ǿ�Ȳ���");
	
	while(nCount < 1000)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
			"����4ʧ��");
		bStart = CSUDI_TRUE;
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), 
			"����5ʧ��");
		bStart = CSUDI_FALSE;
		nCount++;
	}
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
		"����6ʧ��");
	bStart = CSUDI_TRUE;

	CSTCPrint("1:PC0����ʹ��Etherealץ�����ߣ�����pc0�Ϻ�eth1���ӵ�����\n");
    CSTCPrint("2:pc0�Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
	CSTCPrint("Etherealץ��İ��Ƿ�����õ�DHCPS��Ϣһ��?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����4ʧ�ܣ���ȡ��Ϣ����ȷ");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), 
		"����7ʧ��");
	bStart = CSUDI_FALSE;

	CSTCPrint("DHCPS��������ֹͣ����Ͽ�pc0��eth1�����ӣ���������ip��ַ\n");
    CSTCPrint("1:pc0�Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
	CSTCPrint("pc0 �Ƿ���䵽ip��ַ?\n");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����8ʧ�ܣ���ȡ��Ϣ����ȷ");
	
	CSTK_FATAL_POINT;
	
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),
			"����9ʧ��");
	}
	
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIDHCPSConfig&CSUDIDHCPSGetConfig
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIDHCPSConfig������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:
//@REMARK:�Ƿ�ip�� 172.30.1.256 
//@REMARK:		   172.30..2
//@REMARK:		   1723022
//@EXECUTIONFLOW:1��szIfNameΪNULL������CSUDIDHCPSConfig�����ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szIfNameΪ�����ڵ��豸(��"eth100")������CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szIfNameΪ�ػ��豸����(��"lo")������CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfgΪ�գ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfg��Ϊ�գ�acIPStartΪ�Ƿ�IP,����CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfg��Ϊ�գ�acIPStartΪ����IP��acIPEndΪ�Ƿ�IP,����CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfg��Ϊ�գ�acIPStart acIPEndΪ����IP��������ͬһ����,����CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfg��Ϊ�գ�acIPStart acIPEndΪ����IP��acMaskΪ�Ƿ���ʽ,����CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfg��Ϊ�գ�acIPStart acIPEndΪ����IP��acMaskΪ������ʽ��acGatwayΪ�Ƿ�IP,����CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfg��Ϊ�գ�acIPStart acIPEndΪ����IP��acMaskΪ������ʽ��acGatwayΪ����IP������acIPStart acIPEnd acGatway����ͬһ���Σ�����CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:11��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfg��Ϊ�գ�acIPStart acIPEndΪ����IP��acMaskΪ������ʽ��acGatwayΪ����IP������acIPStart acIPEnd acGatway��ͬһ���Σ�acFirstDNSΪ�գ�����CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfg��Ϊ�գ�acIPStart acIPEndΪ����IP��acMaskΪ������ʽ��acGatwayΪ����IP������acIPStart acIPEnd acGatway��ͬһ���Σ�acFirstDNSΪ�Ƿ�IP������CSUDIDHCPSConfig���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSConfigGetConfig_0001(void)
{
	char szIfName[16];
	CSUDIDHCPSConfig_S sDHCPCfg;

	memset(&sDHCPCfg, 0x00, sizeof(CSUDIDHCPSConfig_S));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(CSUDI_NULL, &sDHCPCfg),
		"����1ʧ��");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����2ʧ��");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����3ʧ��");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, CSUDI_NULL),
		"����4ʧ��");


	strncpy(sDHCPCfg.acIPStart, "172.30.1.256",sizeof(sDHCPCfg.acIPStart));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����5ʧ��");


	strncpy(sDHCPCfg.acIPStart, "172.30.2.2",sizeof(sDHCPCfg.acIPStart));	
	strncpy(sDHCPCfg.acIPEnd, "172.30.1.256",sizeof(sDHCPCfg.acIPEnd));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����6ʧ��");

	strncpy(sDHCPCfg.acIPEnd, "172.30.1.254",sizeof(sDHCPCfg.acIPEnd));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����7ʧ��");

	
	strncpy(sDHCPCfg.acIPEnd, "172.30.2.254",sizeof(sDHCPCfg.acIPEnd));	
	strncpy(sDHCPCfg.acMask, "172.30.1.256",sizeof(sDHCPCfg.acMask));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����8ʧ��");

	strncpy(sDHCPCfg.acMask, "255.255.255.0",sizeof(sDHCPCfg.acMask));
	strncpy(sDHCPCfg.acGatway, "172.30.1.256",sizeof(sDHCPCfg.acGatway));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����9ʧ��");
	strncpy(sDHCPCfg.acGatway, "172.30.1.1",sizeof(sDHCPCfg.acGatway));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����10ʧ��");

	strncpy(sDHCPCfg.acGatway, "172.30.2.1",sizeof(sDHCPCfg.acGatway));
	memset(sDHCPCfg.acFirstDNS, 0x00, sizeof(sDHCPCfg.acFirstDNS));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����11ʧ��");
	strncpy(sDHCPCfg.acFirstDNS, "172.30.2.256",sizeof(sDHCPCfg.acFirstDNS));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"����12ʧ��");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSConfig&CSUDIDHCPSGetConfig
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIDHCPSGetConfig������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪNULL������CSUDIDHCPSGetConfig�����ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szIfNameΪ�����ڵ��豸(��"eth100")������CSUDIDHCPSGetConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szIfNameΪ�ػ��豸����(��"lo")������CSUDIDHCPSGetConfig���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psDHCPCfgΪ�գ�����CSUDIDHCPSGetConfig���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSConfigGetConfig_0002(void)
{
	char szIfName[16];
	CSUDIDHCPSConfig_S sDHCPCfg;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetConfig(CSUDI_NULL,&sDHCPCfg),
		"����1ʧ��");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetConfig(szIfName,&sDHCPCfg),
		"����2ʧ��");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetConfig(szIfName,&sDHCPCfg),
		"����3ʧ��");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetConfig(szIfName,CSUDI_NULL),
		"����4ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIDHCPSConfig&CSUDIDHCPSGetConfig
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIDHCPSConfig&CSUDIDHCPSGetConfig������ִ��Ч��
//@PRECONDITION:
//@INPUT:1�������������Ҫ��
//@EXPECTATION:DHCPS������ȷ�����úͻ�ȡ����
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪ��ȷ����������(��"eth1"�� IP:172.30.2.1)
//@EXECUTIONFLOW:2������psDHCPCfg������ȷ��(��acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3������CSUDIDHCPSConfig����������DHCPS����
//@EXECUTIONFLOW:4������CSUDIDHCPSStart����������DHCPS
//@EXECUTIONFLOW:5����eth1���ӵ�PC0 �ܹ�������ȡ172.30.2.2~172.30.2.254�ε�ַ
//@EXECUTIONFLOW:6������CSUDIDHCPSGetConfig��������ȡ�Ĳ��������õĲ�����ͬ
//@EXECUTIONFLOW:7������CSUDIDHCPSStop������ֹͣDHCPS
//@EXECUTIONFLOW:8����eth1���ӵ�PC0 ���ܹ�������ȡ��IP��ַ
//@EXECUTIONFLOW:9��szIfNameΪ��ȷ����������(��"eth1"�� IP:172.30.10.1)
//@EXECUTIONFLOW:10������psDHCPCfg������ȷ��(��acIPStart:172.30.10.2,acIPEnd:172.30.10.254,acMask:255.255.0.0 acGatway:172.30.10.1) 
//@EXECUTIONFLOW:11������CSUDIDHCPSConfig����������DHCPS����
//@EXECUTIONFLOW:12������CSUDIDHCPSStart����������DHCPS
//@EXECUTIONFLOW:13����eth1���ӵ�PC0 �ܹ�������ȡ172.30.10.2~172.30.10.254�ε�ַ
//@EXECUTIONFLOW:14������CSUDIDHCPSGetConfig��������ȡ�Ĳ����͵ڶ������õĲ�����ͬ
//@EXECUTIONFLOW:15������CSUDIDHCPSStop������ֹͣDHCPS
//@EXECUTIONFLOW:16����eth1���ӵ�PC0 ���ܹ�������ȡ��IP��ַ
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSConfigGetConfig_0003(void)
{
	char szIfName[16];
	int nRet = 0;
	int i = 0;
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIDHCPSConfig_S sDHCPCfg;
	CSUDIDHCPSConfig_S sDHCPCfg1;
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));

	for (i=0; i< 2; i++)
	{
		printf("szIfName = %s, ip=%s\n", szIfName, g_sDhcpsInfo.szNicIp[i]);
		CSTK_ASSERT_TRUE_FATAL(
			0 == set_nic_ip(szIfName, g_sDhcpsInfo.szNicIp[i]), 
			"����1ʧ��");

		strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[i].acIPStart);
		strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[i].acIPEnd);
		strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[i].acMask);
		strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[i].acGatway);
		strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[i].acFirstDNS);
		strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[i].acSecondDNS);
		sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[i].uLeaseTime;

		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
			"����2ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
			"����3ʧ��");
		bStart = CSUDI_TRUE;

		CSTCPrint("1:PC0����ʹ��Etherealץ�����ߣ�����pc0�Ϻ�eth1���ӵ�����\n");
	    CSTCPrint("2:pc0�Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
		CSTCPrint("Etherealץ��İ��Ƿ�����õ�DHCPS��Ϣһ��?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ�ܣ���ȡ��Ϣ����ȷ");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSGetConfig(szIfName,&sDHCPCfg1),
			"����4ʧ��");

		nRet = strcmp(sDHCPCfg.acIPStart, sDHCPCfg1.acIPStart);
		nRet |= strcmp(sDHCPCfg.acIPEnd, sDHCPCfg1.acIPEnd);
		nRet |= strcmp(sDHCPCfg.acMask, sDHCPCfg1.acMask);
		nRet |= strcmp(sDHCPCfg.acGatway, sDHCPCfg1.acGatway);
		nRet |= strcmp(sDHCPCfg.acFirstDNS, sDHCPCfg1.acFirstDNS);
		nRet |= strcmp(sDHCPCfg.acSecondDNS, sDHCPCfg1.acSecondDNS);
		nRet |= (sDHCPCfg.uLeaseTime == sDHCPCfg1.uLeaseTime)?0:1;
		
		CSTK_ASSERT_TRUE_FATAL(0 == nRet,"����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), 
			"����6ʧ��");
		bStart = CSUDI_FALSE;

		CSTCPrint("DHCPS��������ֹͣ����Ͽ�pc0��eth1�����ӣ���������ip��ַ\n");
	    CSTCPrint("1:pc0�Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
		CSTCPrint("pc0�Ƿ���䵽ip��ַ?\n");
		
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_FALSE == CSTKWaitYes(),
			"����7ʧ�ܣ���ȡ��Ϣ����ȷ");
	}

	CSTK_FATAL_POINT;
	
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),
			"����8ʧ��");
	}
	
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIDHCPSAddStaticIPCfg������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪNULL������CSUDIDHCPSAddStaticIPCfg�����ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szIfNameΪ�����ڵ��豸(��"eth100")������CSUDIDHCPSAddStaticIPCfg���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szIfNameΪ�ػ��豸����(��"lo")������CSUDIDHCPSAddStaticIPCfg���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItemΪ�գ�����CSUDIDHCPSAddStaticIPCfg���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItem��Ϊ�գ�nCount<=0������CSUDIDHCPSAddStaticIPCfg���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItem��Ϊ�գ�nCount>0��psItem->acIP�Ƿ�����CSUDIDHCPSAddStaticIPCfg���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0001(void)
{
	char szIfName[16];
	CSUDIDHCPSStaticIpCfg sItem;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(CSUDI_NULL,&sItem,1),
		"����1ʧ��");

	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,1),
		"����2ʧ��");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,1),
		"����3ʧ��");
	
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,CSUDI_NULL,1),
		"����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,0),
		"����5ʧ��");

	strncpy(sItem.acIP,"0.0.0", sizeof(sItem.acIP));
	memcpy(sItem.aucMac, g_sDhcpsInfo.sStaticIpCfg.aucMac, sizeof(sItem.aucMac));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,1),
		"����6ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIDHCPSDelStaticIPCfg������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪNULL������CSUDIDHCPSDelStaticIPCfg�����ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szIfNameΪ�����ڵ��豸(��"eth100")������CSUDIDHCPSDelStaticIPCfg���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szIfNameΪ�ػ��豸����(��"lo")������CSUDIDHCPSDelStaticIPCfg���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItemΪ�գ�����CSUDIDHCPSDelStaticIPCfg���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItem��Ϊ�գ�nCount<=0������CSUDIDHCPSDelStaticIPCfg���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItem��Ϊ�գ�nCount>0��psItem->acIP�Ƿ�����CSUDIDHCPSDelStaticIPCfg���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0002(void)
{
	char szIfName[16];
	CSUDIDHCPSStaticIpCfg sItem;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(CSUDI_NULL,&sItem,1),
		"����1ʧ��");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,1),
		"����2ʧ��");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,1),
		"����3ʧ��");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,CSUDI_NULL,1),
		"����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,0),
		"����5ʧ��");

	strncpy(sItem.acIP,"0.0.0", sizeof(sItem.acIP));
	memcpy(sItem.aucMac, g_sDhcpsInfo.sStaticIpCfg.aucMac, sizeof(sItem.aucMac));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,1),
		"����6ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIDHCPSGetStaticIPCfgs������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪNULL������CSUDIDHCPSGetStaticIPCfgs�����ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szIfNameΪ�����ڵ��豸(��"eth100")������CSUDIDHCPSGetStaticIPCfgs���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szIfNameΪ�ػ��豸����(��"lo")������CSUDIDHCPSGetStaticIPCfgs���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItemΪ�գ�����CSUDIDHCPSGetStaticIPCfgs���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItem��Ϊ�գ�pnStaticCountΪ�գ�����CSUDIDHCPSGetStaticIPCfgs���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��szIfNameΪ��ȷ�Ĳ���(��"eth1"),psItem��Ϊ�գ�*pnStaticCount<=0������CSUDIDHCPSGetStaticIPCfgs���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0003(void)
{
	char szIfName[16];
	CSUDIDHCPSStaticIpCfg sItem;
	int nStaticCount = 1;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(CSUDI_NULL,&sItem,&nStaticCount),
		"����1ʧ��");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem,&nStaticCount),
		"����2ʧ��");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem,&nStaticCount),
		"����3ʧ��");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,CSUDI_NULL,&nStaticCount),
		"����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem,CSUDI_NULL),
		"����5ʧ��");
	
	nStaticCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem,&nStaticCount),
		"����6ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}




//@CASEGROUP:CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs������ִ��Ч��
//@PRECONDITION:
//@INPUT:1�������������Ҫ��
//@EXPECTATION:DHCPS������ȷ�����úͻ�ȡ����
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪ��ȷ����������(��"eth1"�� IP:172.30.2.1)
//@EXECUTIONFLOW:2������psDHCPCfg������ȷ��(��acIPStart:172.30.2.2,acIPEnd:172.30.2.2,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3������CSUDIDHCPSConfig����������DHCPS����
//@EXECUTIONFLOW:4������CSUDIDHCPSAddStaticIPCfg���������þ�̬IP(172.30.2.2)��mac(0xaa,0xbb,0xcc,0xdd,0xee,0xff)
//@EXECUTIONFLOW:5������CSUDIDHCPSStart����������DHCPS
//@EXECUTIONFLOW:6����eth1���ӵ�PC0 mac(0xaa,0xbb,0xcc,0xdd,0xee,0xff),�ܶ�̬��ȡ172.30.2.2
//@EXECUTIONFLOW:7����eth1���ӵ�PC1 mac��Ϊ(0xaa,0xbb,0xcc,0xdd,0xee,0xff),���ܶ�̬��ȡ��IP��ַ
//@EXECUTIONFLOW:8������CSUDIDHCPSGetStaticIPCfgs��������ȡ���Ĳ��������õĲ�����ͬ
//@EXECUTIONFLOW:9������CSUDIDHCPSStop������ֹͣDHCPS
//@EXECUTIONFLOW:10������CSUDIDHCPSDelStaticIPCfg������ɾ����̬IP��Mac
//@EXECUTIONFLOW:11������CSUDIDHCPSGetStaticIPCfgs��������ȡ���Ĳ���Ϊ��
//@EXECUTIONFLOW:12������CSUDIDHCPSStart����������DHCPS
//@EXECUTIONFLOW:13����eth1���ӵ�PC0 mac(0xaa,0xbb,0xcc,0xdd,0xee,0xff),�ܶ�̬��ȡ172.30.2.2
//@EXECUTIONFLOW:14����eth1���ӵ�PC1 mac��Ϊ(0xaa,0xbb,0xcc,0xdd,0xee,0xff),�ܶ�̬��ȡ172.30.2.2
//@EXECUTIONFLOW:15������CSUDIDHCPSStop������ֹͣDHCPS
CSUDI_BOOL CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0004(void)
{
	char szIfName[16];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIDHCPSConfig_S sDHCPCfg;
	CSUDIDHCPSStaticIpCfg sItem;
	CSUDIDHCPSStaticIpCfg sItem1;
	int nRet = 0;
	int nStaticCount = 0;
	
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(szIfName, g_sDhcpsInfo.szNicIp[0]), 
		"����1ʧ��");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"����2ʧ��");

	strncpy(sItem.acIP,g_sDhcpsInfo.sStaticIpCfg.acIP, sizeof(sItem.acIP));
	memcpy(sItem.aucMac, g_sDhcpsInfo.sStaticIpCfg.aucMac, sizeof(sItem.aucMac));

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,sizeof(sItem)/sizeof(CSUDIDHCPSStaticIpCfg)),
		"����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
		"����4ʧ��");
	bStart = CSUDI_TRUE;

	CSTCPrint("pc0(mac��ַ�������ļ���һ��) �Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
	CSTCPrint("pc0�ܷ�����IP��ַ?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����5ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTCPrint("�Ͽ�pc0��eth1�����ӣ�ʹ��pc1����ip��ַ\n");
	CSTCPrint("pc1(mac��ַ�������ļ��в�һ��) �Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
	CSTCPrint("pc1�ܷ�����IP��ַ?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����6ʧ�ܣ���ȡ��Ϣ����ȷ");

	nStaticCount = 1;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem1,&nStaticCount),
		"����7ʧ��");
	if (nStaticCount != 1)
	{
		nRet = -1;
	}
	else
	{
		nRet = strcmp(sItem.acIP, sItem1.acIP);
		nRet |= memcmp(sItem.aucMac, sItem1.aucMac, sizeof(sItem.aucMac));
	}
	
	CSTK_ASSERT_TRUE_FATAL(0 == nRet,"����8ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), "����9ʧ��");
	bStart = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,sizeof(sItem1)/sizeof(CSUDIDHCPSStaticIpCfg)),
		"����10ʧ��");
	nStaticCount = 1;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem1,&nStaticCount),
		"����11ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0 == nStaticCount,"����12ʧ��");


	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), "����13ʧ��");
	bStart = CSUDI_TRUE;

	CSTCPrint("pc0(mac��ַ�������ļ���һ��) �Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
	CSTCPrint("pc0�ܷ�����IP��ַ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTCPrint("�Ͽ�pc0��eth1�����ӣ�ʹ��pc1����ip��ַ\n");
	CSTCPrint("pc1(mac��ַ�������ļ��в�һ��) �Ϻ�eth1���ӵ����ڣ�����IP��ַ\n");
	CSTCPrint("pc1�ܷ�����IP��ַ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����15ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),"����16ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDHCPSGetLeaseInfo
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIDHCPSGetLeaseInfo������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1��szIfNameΪNULL������CSUDIDHCPSGetLeaseInfo�����ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szIfNameΪ�����ڵ��豸(��"eth100")������CSUDIDHCPSGetLeaseInfo���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szIfNameΪ�ػ��豸����(��"lo")������CSUDIDHCPSGetLeaseInfo���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��szIfNameΪ��ȷ�Ĳ���(��"eth1"),pstLeaseInfoΪ�գ�����CSUDIDHCPSGetLeaseInfo���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��szIfNameΪ��ȷ�Ĳ���(��"eth1"),pstLeaseInfo��Ϊ�գ�pnCountΪ�գ�����CSUDIDHCPSGetLeaseInfo���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��szIfNameΪ��ȷ�Ĳ���(��"eth1"),pstLeaseInfo��Ϊ�գ�*pnCount<=0������CSUDIDHCPSGetLeaseInfo���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSGetLeaseInfo_0001(void)
{
	char szIfName[16];
	CSUDIDHCPSLeaseInfo stLeaseInfo;
	int nCount = 1;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(CSUDI_NULL,&stLeaseInfo, &nCount),
		"����1ʧ��");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,&stLeaseInfo, &nCount),
		"����2ʧ��");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,&stLeaseInfo, &nCount),
		"����3ʧ��");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,CSUDI_NULL, &nCount),
		"����4ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,&stLeaseInfo, CSUDI_NULL),
		"����5ʧ��");

	nCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,&stLeaseInfo, &nCount),
		"����6ʧ��");
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSGetLeaseInfo
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIDHCPSGetLeaseInfo������ִ��Ч��
//@PRECONDITION:
//@INPUT:1�������������Ҫ��
//@EXPECTATION:
//@REMARK:���eth1Ϊ���߽ӿڣ����̨pc����ֱ������������
//@REMARK:���eth1Ϊ���߽ӿڣ����̨pc����ͨ������������������
//@EXECUTIONFLOW:1��szIfNameΪ��ȷ����������(��"eth1"�� IP:172.30.2.1)
//@EXECUTIONFLOW:2������psDHCPCfg������ȷ��(��acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3������CSUDIDHCPSConfig����������DHCPS����
//@EXECUTIONFLOW:4������CSUDIDHCPSStart����������DHCPS
//@EXECUTIONFLOW:5����eth1���ӵ�PC0 �ܹ�������ȡ172.30.2.2~172.30.2.254�ε�ַ
//@EXECUTIONFLOW:6����eth1���ӵ�PC1 �ܹ�������ȡ172.30.2.2~172.30.2.254�ε�ַ
//@EXECUTIONFLOW:7������CSUDIDHCPSGetLeaseInfo��������ȡ����Ϣ��PC0��PC1��Ϣ��ͬ
//@EXECUTIONFLOW:8���Ͽ�pc2��eth1������
//@EXECUTIONFLOW:9������CSUDIDHCPSGetLeaseInfo��������ȡ����Ϣ��PC0��Ϣ��ͬ
//@EXECUTIONFLOW:10������CSUDIDHCPSStop������ֹͣDHCPS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSGetLeaseInfo_0002(void)
{
	char szIfName[16];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIDHCPSConfig_S sDHCPCfg;
	CSUDIDHCPSLeaseInfo satLeaseInfo[2];
	int nCount = 2;
	int i = 0;
	
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(szIfName, g_sDhcpsInfo.szNicIp[0]), 
		"����1ʧ��");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPEnd);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
		"����3ʧ��");
	bStart = CSUDI_TRUE;
	
    CSTCPrint("pc0�Ϻ�eth1���ӵ����ڣ��ܷ�������ӦIP�ε�ַ\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ�ܣ�������ȷ����");

	CSTCPrint("pc1�Ϻ�eth1���ӵ����ڣ��ܷ�������ӦIP�ε�ַ\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ�������ȷ����");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetLeaseInfo(szIfName,satLeaseInfo, &nCount),
		"����6ʧ��");

	CSTCPrint("-----------------LeaseInfo start-------------------\n");
	CSTCPrint("the leaseinfo count is %d \n", nCount);
	for (i = 0; i < nCount; i++)
	{
		CSTCPrint("the %d info is:\n");
		CSTCPrint("IP:  %s\n", satLeaseInfo[i].acIP);
		CSTCPrint("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			satLeaseInfo[i].aucMac[0],satLeaseInfo[i].aucMac[1],
			satLeaseInfo[i].aucMac[2],satLeaseInfo[i].aucMac[3],
			satLeaseInfo[i].aucMac[4],satLeaseInfo[i].aucMac[5]
		);	
		CSTCPrint("LeftTime:  %d\n", satLeaseInfo[i].nLeftTime);		
	}
	CSTCPrint("-----------------LeaseInfo end-------------------\n");

	CSTCPrint("��ȡ����Ϣ�Ƿ��pc0 pc1��Ϣһ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTCPrint("�Ͽ�pc1��eth1������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����8ʧ�ܣ�û�жϿ�����");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetLeaseInfo(szIfName,satLeaseInfo, &nCount),
		"����9ʧ��");

	CSTCPrint("-----------------LeaseInfo start-------------------\n");
	CSTCPrint("the leaseinfo count is %d \n", nCount);
	for (i = 0; i < nCount; i++)
	{
		CSTCPrint("the %d info is:\n");
		CSTCPrint("IP:  %s\n", satLeaseInfo[i].acIP);
		CSTCPrint("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			satLeaseInfo[i].aucMac[0],satLeaseInfo[i].aucMac[1],
			satLeaseInfo[i].aucMac[2],satLeaseInfo[i].aucMac[3],
			satLeaseInfo[i].aucMac[4],satLeaseInfo[i].aucMac[5]
		);	
		CSTCPrint("LeftTime:  %d\n", satLeaseInfo[i].nLeftTime);		
	}
	CSTCPrint("-----------------LeaseInfo end-------------------\n");

	CSTCPrint("��ȡ����Ϣ�Ƿ��pc0��Ϣһ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ�ܣ���ȡ��Ϣ����ȷ");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), "����11ʧ��");
	bStart = CSUDI_FALSE;

	CSTK_FATAL_POINT;
	
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),"����12ʧ��");
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSGetLeaseInfo
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIDHCPSGetLeaseInfo������ִ��Ч��
//@PRECONDITION:
//@INPUT:1�������������Ҫ��
//@EXPECTATION:
//@REMARK:���eth1Ϊ���߽ӿڣ����̨pc����ֱ������������
//@REMARK:���eth1Ϊ���߽ӿڣ����̨pc����ͨ������������������
//@EXECUTIONFLOW:1��szIfNameΪ��ȷ����������(��"eth1"�� IP:172.30.2.1)
//@EXECUTIONFLOW:2������psDHCPCfg������ȷ��(��acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3������CSUDIDHCPSConfig����������DHCPS����
//@EXECUTIONFLOW:4������CSUDIDHCPSStart����������DHCPS
//@EXECUTIONFLOW:5����eth1���ӵ�PC0 �ܹ�������ȡ172.30.2.2~172.30.2.254�ε�ַ
//@EXECUTIONFLOW:6����eth1���ӵ�PC1 �ܹ�������ȡ172.30.2.2~172.30.2.254�ε�ַ
//@EXECUTIONFLOW:7��*pnCount=1������CSUDIDHCPSGetLeaseInfo�������ܹ���ȷ��ȡ��һ��pc����Ϣ
//@EXECUTIONFLOW:8������CSUDIDHCPSStop������ֹͣDHCPS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSGetLeaseInfo_0003(void)
{
	char szIfName[16];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIDHCPSConfig_S sDHCPCfg;
	CSUDIDHCPSLeaseInfo satLeaseInfo[1];
	int nCount = 1;
	int i = 0;
	
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(szIfName, g_sDhcpsInfo.szNicIp[0]), 
		"����1ʧ��");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPEnd);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
		"����3ʧ��");
	bStart = CSUDI_TRUE;
	
    CSTCPrint("pc0�Ϻ�eth1���ӵ����ڣ��ܷ�������ӦIP�ε�ַ\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTCPrint("pc1�Ϻ�eth1���ӵ����ڣ��ܷ�������ӦIP�ε�ַ\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetLeaseInfo(szIfName,satLeaseInfo, &nCount),
		"����6ʧ��");

	CSTCPrint("-----------------LeaseInfo start-------------------\n");
	CSTCPrint("the leaseinfo count is %d \n", nCount);
	for (i = 0; i < nCount; i++)
	{
		CSTCPrint("the %d info is:\n");
		CSTCPrint("IP:  %s\n", satLeaseInfo[i].acIP);
		CSTCPrint("MAC: %x:%x:%x:%x:%x:%x\n",
			satLeaseInfo[i].aucMac[0],satLeaseInfo[i].aucMac[1],
			satLeaseInfo[i].aucMac[2],satLeaseInfo[i].aucMac[3],
			satLeaseInfo[i].aucMac[4],satLeaseInfo[i].aucMac[5]
		);	
		CSTCPrint("LeftTime:  %d\n", satLeaseInfo[i].nLeftTime);		
	}
	CSTCPrint("-----------------LeaseInfo end-------------------\n");

	CSTCPrint("�Ƿ�ֻ��ʾ��pc0����pc1����Ϣ������Ϣ��ȷ����\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), "����8ʧ��");
	bStart = CSUDI_FALSE;

	CSTK_FATAL_POINT;
	
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),"����9ʧ��");
	}
	
	return CSUDI_TRUE;
}



