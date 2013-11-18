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
#include "cs_routertestcase.h"
#include "udi2_router.h"

#define MAX_NIC_LEN (16)
#define MAX_IP_LEN (16)

static struct
{
	char szWan[16];
	char szWanIp[16];
	char szLan[16];
	char szIp0[16];
	char szIp1[16];
	char szNetMask1[16];
	char szNetMask0[16];
	char szInvalidWan[16];
	char szInvalidLan[16];
	
}g_sRouterInfo;

static char g_szPcWanIp[16] = {0}; // ��wan��������pc��ip��ַ

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

static void Router_InitCfg(void)
{
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER", "PC_WAN_IP", g_szPcWanIp, MAX_IP_LEN))
	{
		CSTCPrint("PC_WAN_IP is %s\n", g_szPcWanIp);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","WAN_NIC",g_sRouterInfo.szWan,MAX_NIC_LEN))
	{
		CSTCPrint("WAN_NIC is %s\n",g_sRouterInfo.szWan);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","WAN_IP",g_sRouterInfo.szWanIp,MAX_NIC_LEN))
	{
		CSTCPrint("WAN_IP is %s\n",g_sRouterInfo.szWanIp);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","LAN_NIC",g_sRouterInfo.szLan,MAX_NIC_LEN))
	{
		CSTCPrint("LAN_NIC is %s\n",g_sRouterInfo.szLan);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","LAN_IP0",g_sRouterInfo.szIp0,MAX_NIC_LEN))
	{
		CSTCPrint("LAN_IP0 is %s\n",g_sRouterInfo.szIp0);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","LAN_NETMASK0",g_sRouterInfo.szNetMask0,MAX_NIC_LEN))
	{
		CSTCPrint("LAN_NETMASK0 is %s\n",g_sRouterInfo.szNetMask0);
	}

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","LAN_IP1",g_sRouterInfo.szIp1,MAX_NIC_LEN))
	{
		CSTCPrint("LAN_IP1 is %s\n",g_sRouterInfo.szIp1);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","LAN_NETMASK1",g_sRouterInfo.szNetMask1,MAX_NIC_LEN))
	{
		CSTCPrint("LAN_NETMASK1 is %s\n",g_sRouterInfo.szNetMask1);
	}
	
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","INVALID_WAN_NIC",g_sRouterInfo.szInvalidWan,MAX_NIC_LEN))
	{
		CSTCPrint("INVALID_WAN_NIC is %s\n",g_sRouterInfo.szInvalidWan);
	}
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("ROUTER","INVALID_LAN_NIC",g_sRouterInfo.szInvalidLan,MAX_NIC_LEN))
	{
		CSTCPrint("INVALID_LAN_NIC is %s\n",g_sRouterInfo.szInvalidLan);
	}	
}

CSUDI_BOOL CSTC_ROUTER_Init(void)
{
	Router_InitCfg();
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_ROUTER_UnInit(void)
{
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRTSetInterfaces&CSUDIRTGetInterfaces
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIRTSetInterfaces������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:���÷��ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��szWanΪNULL������CSUDIRTSetInterfaces���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szWanΪ���������ڣ�����CSUDIRTSetInterfaces���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szWanΪ������szLanΪNULL,����CSUDIRTSetInterfaces���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��szWanΪ������szLanΪ����������,����CSUDIRTSetInterfaces���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetInterfaces(CSUDI_NULL,g_sRouterInfo.szLan),
		"����1ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetInterfaces(g_sRouterInfo.szInvalidWan, g_sRouterInfo.szLan),
		"����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, CSUDI_NULL),
		"����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szInvalidLan),
		"����4ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTSetInterfaces&CSUDIRTGetInterfaces
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIRTGetInterfaces������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:���÷��ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��pcWan��pcLanΪNULL������CSUDIRTGetInterfaces���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��nWanLen<=0��nLanLen<=0��nWanLen<=0 ,����CSUDIRTGetInterfaces���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0002(void)
{
	char szWan[16];
	char szLan[16];
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetInterfaces(CSUDI_NULL, sizeof(szWan), CSUDI_NULL, sizeof(szLan)),
		"����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetInterfaces(szWan, 0, szLan, 0),
		"����2ʧ��");
				
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTSetInterfaces&CSUDIRTGetInterfaces
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIRTSetInterfaces&CSUDIRTGetInterfaces������ִ��Ч��
//@PRECONDITION:
//@INPUT:1�������������Ҫ��
//@EXPECTATION:�����ܹ���ȷִ��
//@REMARK:
//@EXECUTIONFLOW:1������szWan,szLan, szWan �� szlan��ͬ
//@EXECUTIONFLOW:2������CSUDIRTSetInterfaces�����ýӿڲ���ʧ��
//@EXECUTIONFLOW:3������szWan,szLan, szWan �� szlan����ͬ
//@EXECUTIONFLOW:4������CSUDIRTSetInterfaces���ɹ����ýӿڲ���
//@EXECUTIONFLOW:5������CSUDIRTGetInterfaces����ȡ���ӿڲ����������õĲ���һ��
//@EXECUTIONFLOW:6������szWan,szLan, szWan �� szlan����ͬ�����Һ���һ�β�һ��
//@EXECUTIONFLOW:7������CSUDIRTSetInterfaces���ɹ����ýӿڲ���
//@EXECUTIONFLOW:8������CSUDIRTGetInterfaces����ȡ���ӿڲ����������õĲ���һ��
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0003(void)
{
	char szWan[16];
	char szLan[16];
	int nRet = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS != CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szWan),
		"����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetInterfaces(szWan, sizeof(szWan), szLan, sizeof(szLan)),
		"����3ʧ��");
	nRet = strcmp(szWan, g_sRouterInfo.szWan);
	nRet |= strcmp(szLan, g_sRouterInfo.szLan);

	CSTK_ASSERT_TRUE_FATAL(0 == nRet, "����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szLan, g_sRouterInfo.szWan),
		"����5ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetInterfaces(szWan, sizeof(szWan), szLan, sizeof(szLan)), 
		"����6ʧ��");
	nRet = strcmp(szWan, g_sRouterInfo.szLan);
	nRet |= strcmp(szLan, g_sRouterInfo.szWan);

	CSTK_ASSERT_TRUE_FATAL(0 == nRet, "����7ʧ��");
	
	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}




//@CASEGROUP:CSUDIRTSetLanIp
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIRTSetLanIp������ִ��Ч��
//@PRECONDITION:
//@INPUT:1���������������Ҫ��
//@EXPECTATION:���÷��ط�CSUDI_SUCCESS
//@REMARK:�Ƿ�ip�� 172.30.1.256 
//@REMARK:		   172.30..2
//@REMARK:		   1723022
//@EXECUTIONFLOW:1��szLanΪNULL������CSUDIRTSetLanIp���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��szLanΪ���������ڣ�����CSUDIRTSetLanIp���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��szLanΪ�������ڣ�szIpΪNULL ,����CSUDIRTSetLanIp���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��szLanΪ�������ڣ�szIpΪ�Ƿ�IP ,����CSUDIRTSetLanIp���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��szLanΪ�������ڣ�szIpΪ���� ,szMaskΪNULL,����CSUDIRTSetLanIp���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetLanIp_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(CSUDI_NULL, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szInvalidLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, CSUDI_NULL, g_sRouterInfo.szNetMask0), 
		"����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, "172.30.2.256", g_sRouterInfo.szNetMask0), 
		"����5ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, "172.30.2.", g_sRouterInfo.szNetMask0), 
		"����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, "1723022", g_sRouterInfo.szNetMask0),
		"����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, CSUDI_NULL), 
		"����8ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTSetLanIp
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIRTSetLanIp������ִ��Ч��
//@PRECONDITION:
//@INPUT:1�������������Ҫ��
//@EXPECTATION:�ɹ�ִ��CSUDIRTSetLanIp
//@REMARK:
//@EXECUTIONFLOW:1��������ȷ��������
//@EXECUTIONFLOW:2������CSUDIRTSetLanIp��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��PC0��lan�����ӣ�����pc0 IP��ַ��lan ip��ͬһ��ַ��
//@EXECUTIONFLOW:4��PC0 ping lan��ip��ַ���ܹ�pingͨ
//@EXECUTIONFLOW:5��������ȷ�ĺ���һ�β�ͬ��������
//@EXECUTIONFLOW:6������CSUDIRTSetLanIp��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7��PC0��lan�����ӣ�����pc0 IP��ַ��lan ip��ͬһ��ַ��
//@EXECUTIONFLOW:8��PC0 ping lan��ip��ַ���ܹ�pingͨ
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetLanIp_0002(void)
{
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"����2ʧ��");

	CSTCPrint("����lan�ڵ�ip��ַ��ϢΪ ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0��lan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
    CSTCPrint("2:����pc0 mask��ַ��lan�� mask��ͬ\n");
	CSTCPrint("3:��pc0����ping lan��ip ��ַ?\n");
	CSTCPrint("pc0�����ܷ�pingͨlan��?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����3ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp1, g_sRouterInfo.szNetMask1), 
		"����4ʧ��");

	CSTCPrint("����lan�ڵ�ip��ַ��ϢΪ ip=%s,mask=%s\n", g_sRouterInfo.szIp1, g_sRouterInfo.szNetMask1);
	CSTCPrint("1:PC0��lan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
    CSTCPrint("2:����pc0 mask��ַ��lan�� mask��ͬ\n");
	CSTCPrint("3:��pc0����ping lan��ip ��ַ?\n");
	CSTCPrint("pc0�����ܷ�pingͨlan��?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ���ȡ��Ϣ����ȷ");

	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIRTStartService������ִ��Ч��
//@PRECONDITION:ROUTERģ�鱻������ʼ��
//@INPUT:1��eService, pvStartParam�������������Ҫ��
//@EXPECTATION:���е��÷��ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��eServiceΪ-1��pvStartParam�Ϸ�������CSUDIRTStartService���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��eServiceΪEM_RT_SERVICETYPE_MAX��pvStartParam�Ϸ�������CSUDIRTStartService���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��eServiceΪEM_RT_SERVICETYPE_MAX+1��pvStartParam�Ϸ�������CSUDIRTStartService���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��eServiceΪEM_RT_SERVICETYPE_DNS_PROXY��pvStartParamΪ�գ�����CSUDIRTStartService���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5������(eth1 ipΪ192.168.10.68)
//@EXECUTIONFLOW:6������CSUDIRTSetInterfaces����wan eth1,lan ra0
//@EXECUTIONFLOW:7��eServiceΪEM_RT_SERVICETYPE_DNS_PROXY��pvStartParam�е�DNS��ַΪ�գ�����CSUDIRTStartService���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0001(void)
{
	CSUDIRTDnsServers_S  sStartParam;
	char *pFirstDNS="202.10.10.88";
	char *pSecDNS="202.10.10.99";

	memcpy(sStartParam.acFirstDNS, pFirstDNS, CSUDI_APRT_IP_LEN);
	memcpy(sStartParam.acSecondDNS, pSecDNS, CSUDI_APRT_IP_LEN);
       
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStartService(-1, &sStartParam),
		"����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStartService(EM_RT_SERVICETYPE_MAX, &sStartParam),
		"����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStartService(EM_RT_SERVICETYPE_MAX+1, &sStartParam),
		"����3ʧ��");
    
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, CSUDI_NULL),
		"����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(g_sRouterInfo.szWan, g_sRouterInfo.szWanIp), 
		"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����6ʧ��");

    memset(sStartParam.acFirstDNS, '\0', CSUDI_APRT_IP_LEN);
    memset(sStartParam.acSecondDNS, '\0', CSUDI_APRT_IP_LEN);
	CSTK_ASSERT_TRUE_FATAL(
	CSUDI_FAILURE== CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, &sStartParam),
	"����7ʧ��");
        
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIRTStopService������ִ��Ч��
//@PRECONDITION:ROUTERģ�鱻������ʼ��
//@INPUT:1��eService�������������Ҫ��
//@EXPECTATION:���е��÷��ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��eServiceΪ-1������CSUDIRTStopService���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2��eServiceΪEM_RT_SERVICETYPE_MAX������CSUDIRTStopService���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��eServiceΪEM_RT_SERVICETYPE_MAX+1������CSUDIRTStopService���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStopService(-1),
		"����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStopService(EM_RT_SERVICETYPE_MAX),
		"����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStopService(EM_RT_SERVICETYPE_MAX+1),
		"����3ʧ��");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:����EM_RT_SERVICETYPE_DNS_PROXY�����������ֹͣ
//@PRECONDITION:
//@INPUT:1��eServiceΪEM_RT_SERVICETYPE_DNS_PROXY
//@EXPECTATION:�ܹ������õ�����������ȷʹ��DNS_PROXY����
//@REMARK:
//@EXECUTIONFLOW:1������(eth1 ipΪ192.168.10.68)
//@EXECUTIONFLOW:2������CSUDIRTSetInterfaces����wan eth1,lan ra0
//@EXECUTIONFLOW:3������CSUDIRTSetLanIp����lan ��IP��ַΪ172.30.2.1
//@EXECUTIONFLOW:4��lan�����ӵ�PC0 ��������(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5��wan�����ӵ�PC1 ��������(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6������CSUDIRTStartService��������DNS(nameserver 192.168.10.90)��������� 
//@EXECUTIONFLOW:7����pc1��������ethereal capture 192.168.10.90 �������ݰ�
//@EXECUTIONFLOW:8����pc0 ��������� www.baidu.com
//@EXECUTIONFLOW:9���鿴pc1 ethereal ���沶������ݰ��д���dns ����www.baidu.com �����ݰ�
//@EXECUTIONFLOW:10������CSUDIRTStopService�����ر�DNS���������
//@EXECUTIONFLOW:11����pc0 ��������� www.baidu.com (pc1 ������Ҫ����ʹ��ethereal ����192.168.10.90��������)
//@EXECUTIONFLOW:12���鿴pc1 ethereal ���沶������ݰ��в�����dns ����www.baidu.com �����ݰ�
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0003(void)
{
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIRTDnsServers_S  sStartParam;

	memcpy(sStartParam.acFirstDNS, g_szPcWanIp, CSUDI_APRT_IP_LEN);
	memcpy(sStartParam.acSecondDNS, g_szPcWanIp, CSUDI_APRT_IP_LEN);

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(g_sRouterInfo.szWan, g_sRouterInfo.szWanIp), 
		"����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"����3ʧ��");

	CSTCPrint("����lan�ڵ�ip��ַ��ϢΪ ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0��lan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
    CSTCPrint("2:����pc0 mask��ַ��lan�� mask��ͬ\n");
	CSTCPrint("3:����pc0 dns Ϊlan��ip��ַ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����4ʧ�ܣ�û����ȷ����pc0 ������Ϣ");

	CSTCPrint("����wan�ڵ�ip��ַ��ϢΪ ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1��wan������,����pc1 ip��ַ��wan��ip��ַ��ͬһ����\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����5ʧ�ܣ�û����ȷ����pc1 ������Ϣ\n");

/*begin modified by 904782 --ֱ���ڴ�����������DNS��ַ*/
#if 0
	CSTCPrint("�޸�STB /etc/resolv.conf�ļ������(nameserver ip) ip��ַΪpc1 ip��ַ\n");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����6ʧ�ܣ��޸�STB /etc/resolv.conf�ļ�ʧ��\n");
#endif

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, &sStartParam),
		"����6ʧ��");
	bStart = CSUDI_TRUE;
/*end modified by 904782 --ֱ���ڴ�����������DNS��ַ*/

	CSTCPrint("1:��pc1��������ethereal�����wan���������ڵ����ݰ�\n");
	CSTCPrint("pc1���Ƿ�����ethereal�������ݰ�\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����7ʧ�ܣ�û����ȷ����ethereal");

	CSTCPrint("1:��pc0 ��������� www.baidu.com\n");
	CSTCPrint("pc0 ������Ƿ���� www.baidu.com\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����8ʧ�ܣ�û�з���www.baidu.com");

	CSTCPrint("1:�鿴ethereal��������ݰ�\n");
	CSTCPrint("���ݰ����Ƿ����pc0����dns�����ݰ�?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����9ʧ�ܣ�������dns���ݰ�");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
		"����10ʧ��");
	bStart = CSUDI_FALSE;

	CSTCPrint("1:��pc1��������ethereal�����wan���������ڵ����ݰ�\n");
	CSTCPrint("pc1���Ƿ�����ethereal�������ݰ�\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����11ʧ�ܣ�û����ȷ����ethereal");

	CSTCPrint("1:��pc0 ��������� www.baidu.com\n");
	CSTCPrint("pc0 ������Ƿ���� www.baidu.com\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����12ʧ�ܣ�û�з���www.baidu.com");

	CSTCPrint("1:�鿴ethereal��������ݰ�\n");
	CSTCPrint("���ݰ����Ƿ����pc0����dns�����ݰ�?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����13ʧ�ܣ�������dns���ݰ�");

	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
			"����14ʧ��");
	}	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:����EM_RT_SERVICETYPE_IGMP_PROXY�����������ֹͣ
//@PRECONDITION:STB����·��ת��STB:echo 1 > /proc/sys/net/ipv4/ip_forward
//@INPUT:1��eServiceΪEM_RT_SERVICETYPE_IGMP_PROXY
//@EXPECTATION:�ܹ������õ�����������ȷʹ��IGMP_PROXY����
//@REMARK:
//@EXECUTIONFLOW:1������(eth1 ipΪ192.168.10.68)
//@EXECUTIONFLOW:2������CSUDIRTSetInterfaces����wan eth1,lan ra0
//@EXECUTIONFLOW:3������CSUDIRTSetLanIp����lan ��IP��ַΪ172.30.2.1
//@EXECUTIONFLOW:4��lan�����ӵ�PC0 ��������(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5��wan�����ӵ�PC1 ��������(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6������CSUDIRTExecuteIptableCmd����ִ��iptables -t nat -A POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE����
//@EXECUTIONFLOW:7�����ò���eService=EM_RT_SERVICETYPE_IGMP_PROXY
//@EXECUTIONFLOW:8������CSUDIRTStartService��������IGMP PROXY����
//@EXECUTIONFLOW:9��PC1���棬ʹ��WindSend���߲�������
//@EXECUTIONFLOW:10��PC0���棬ʹ��VLC���߲�������
//@EXECUTIONFLOW:11��PC0���棬�ܹ���������WindSend���沥�ŵ�����
//@EXECUTIONFLOW:12������CSUDIRTStopService�����ر�IGMP PROXY�������ɹ�
//@EXECUTIONFLOW:13��PC0���棬���ܲ���WindSend���沥�ŵ�����
//@EXECUTIONFLOW:14������CSUDIRTExecuteIptableCmd����ִ������iptables -t nat -D POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE����
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0004(void)
{
	char szIptableCmd[128];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIRTDnsServers_S  sStartParam;

	memset(sStartParam.acFirstDNS, '\0', CSUDI_APRT_IP_LEN);
	memset(sStartParam.acSecondDNS, '\0', CSUDI_APRT_IP_LEN);

	system("echo 1 > /proc/sys/net/ipv4/ip_forward");
	
	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(g_sRouterInfo.szWan, g_sRouterInfo.szWanIp), 
		"����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"����3ʧ��");

	CSTCPrint("����lan�ڵ�ip��ַ��ϢΪ ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0��lan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
    CSTCPrint("2:����pc0 mask��ַ��lan�� mask��ͬ\n");
	CSTCPrint("3:����pc0 dns Ϊlan��ip��ַ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����4ʧ�ܣ�û����ȷ����pc0 ������Ϣ");

	CSTCPrint("����wan�ڵ�ip��ַ��ϢΪ ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1��wan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����5ʧ�ܣ�û����ȷ����pc1 ������Ϣ");

	sprintf(szIptableCmd, "iptables -t nat -A POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_IGMP_PROXY, &sStartParam),
		"����7ʧ��");
	bStart = CSUDI_TRUE;

	CSTCPrint("1:��pc1��������WindSend��������\n");
	CSTCPrint("pc1���Ƿ�����WindSend��������\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����8ʧ�ܣ�û����ȷ����WindSend");

	CSTCPrint("1:��pc0 ʹ��VLC���߲�������\n");
	CSTCPrint("pc0 �Ƿ�ʹ��VLC���߲�������\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����9ʧ�ܣ�û��ʹ��VLC���߲�������");


	CSTCPrint("PC0���棬�ܷ���������WindSend���沥�ŵ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����10ʧ�ܣ�����������������");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == 
		CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),"����11ʧ��");
	bStart = CSUDI_FALSE;

	CSTCPrint("PC0���棬�ܷ���������WindSend���沥�ŵ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����12ʧ�ܣ���������������");

	sprintf(szIptableCmd, "iptables -t nat -D POSTROUTING -s %s/16 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"����13ʧ��");

	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
			"����14ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:����EM_RT_SERVICETYPE_UPNP�����������ֹͣ
//@PRECONDITION:1:STB����·��ת��STB:echo 1 > /proc/sys/net/ipv4/ip_forward
//@PRECONDITION:2:�ػ��豸�������� ifconfig lo up
//@INPUT:1��eServiceΪEM_RT_SERVICETYPE_UPNP
//@EXPECTATION:�ܹ������õ�����������ȷʹ��UPNP����
//@REMARK:
//@EXECUTIONFLOW:1������(eth1 ipΪ192.168.10.68)
//@EXECUTIONFLOW:2������CSUDIRTSetInterfaces����wan eth1,lan ra0
//@EXECUTIONFLOW:3������CSUDIRTSetLanIp����lan ��IP��ַΪ172.30.2.1
//@EXECUTIONFLOW:4��lan�����ӵ�PC0 ��������(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5��wan�����ӵ�PC1 ��������(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6������CSUDIRTExecuteIptableCmd����ִ��iptables -t nat -A POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE����
//@EXECUTIONFLOW:7�����ò���eService=EM_RT_SERVICETYPE_UPNP
//@EXECUTIONFLOW:8������CSUDIRTStartService��������UPNP����
//@EXECUTIONFLOW:9��PC0���棬ʹ��Xlight FTP�������������
//@EXECUTIONFLOW:10��PC0���棬�ܹ���������UPNP�������Ϣ
//@EXECUTIONFLOW:11������CSUDIRTStopService�����ر�UPNP�������ɹ�
//@EXECUTIONFLOW:12��PC0���棬���ܹ���������UPNP�������Ϣ
//@EXECUTIONFLOW:13������CSUDIRTExecuteIptableCmd����ִ��iptables -t nat -D POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE����
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0005(void)
{
	char szIptableCmd[128];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIRTDnsServers_S  sStartParam;

	memset(sStartParam.acFirstDNS, '\0', CSUDI_APRT_IP_LEN);
	memset(sStartParam.acSecondDNS, '\0', CSUDI_APRT_IP_LEN);

	system("echo 1 > /proc/sys/net/ipv4/ip_forward");
	
	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(g_sRouterInfo.szWan, g_sRouterInfo.szWanIp), 
		"����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"����3ʧ��");

	CSTCPrint("����lan�ڵ�ip��ַ��ϢΪ ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0��lan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
    CSTCPrint("2:����pc0 mask��ַ��lan�� mask��ͬ\n");
	CSTCPrint("3:����pc0 dns Ϊlan��ip��ַ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����4ʧ�ܣ�û����ȷ����pc0 ������Ϣ");

	CSTCPrint("����wan�ڵ�ip��ַ��ϢΪ ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1��wan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����5ʧ�ܣ�û����ȷ����pc1 ������Ϣ");

	sprintf(szIptableCmd, "iptables -t nat -A POSTROUTING -s %s/16 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_UPNP, &sStartParam),
		"����7ʧ��");
	bStart = CSUDI_TRUE;	

	CSTCPrint("1:��pc0 ʹ��Xlight FTP���߲���\n");
	CSTCPrint("pc0 �Ƿ��ܹ�����UPNP�������Ϣ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����8ʧ�ܣ�pc0 ���ܿ���UPNP�������Ϣ");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
		"����9ʧ��");
	bStart = CSUDI_FALSE;

	CSTCPrint("1:��pc0 ʹ��Xlight FTP���߲���\n");
	CSTCPrint("pc0 �Ƿ��ܹ�����UPNP�������Ϣ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����10ʧ�ܣ�pc0 �ܿ���UPNP�������Ϣ");

	sprintf(szIptableCmd, "iptables -t nat -D POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"����11ʧ��");
	
	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
			"����12ʧ��");
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:����EM_RT_SERVICETYPE_DNS_PROXY\EM_RT_SERVICETYPE_IGMP_PROXY\EM_RT_SERVICETYPE_UPNP����ͬʱ����
//@PRECONDITION:1:STB����·��ת��STB:echo 1 > /proc/sys/net/ipv4/ip_forward
//@PRECONDITION:2:�ػ��豸�������� ifconfig lo up
//@INPUT:1��eServiceΪEM_RT_SERVICETYPE_DNS_PROXY\EM_RT_SERVICETYPE_IGMP_PROXY\EM_RT_SERVICETYPE_UPNP
//@EXPECTATION:�ܹ������õ�����������ȷʹ�����еķ���
//@REMARK:
//@EXECUTIONFLOW:1������(eth1 ipΪ192.168.10.68)
//@EXECUTIONFLOW:2������CSUDIRTSetInterfaces����wan eth1,lan ra0
//@EXECUTIONFLOW:3������CSUDIRTSetLanIp����lan ��IP��ַΪ172.30.2.1
//@EXECUTIONFLOW:4��lan�����ӵ�PC0 ��������(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5��wan�����ӵ�PC1 ��������(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6������CSUDIRTExecuteIptableCmd����ִ��iptables -t nat -A POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE����
//@EXECUTIONFLOW:7������CSUDIRTStartService��������EM_RT_SERVICETYPE_DNS_PROXY(nameserver 192.168.10.90)����
//@EXECUTIONFLOW:8������CSUDIRTStartService��������EM_RT_SERVICETYPE_IGMP_PROXY����
//@EXECUTIONFLOW:9������CSUDIRTStartService��������EM_RT_SERVICETYPE_UPNP����
//@EXECUTIONFLOW:10����pc1��������ethereal capture 192.168.10.90 �������ݰ�
//@EXECUTIONFLOW:11����pc0 ��������� www.baidu.com
//@EXECUTIONFLOW:12���鿴pc1 ethereal ���沶������ݰ��д���dns ����www.baidu.com �����ݰ�
//@EXECUTIONFLOW:13��PC1����ʹ��WindSend����������PC2ʹ��VLC���߲�������
//@EXECUTIONFLOW:14��PC0��vlc�����ܹ�������������
//@EXECUTIONFLOW:15��PC0���棬ʹ��Xlight FTP�������������,�ܹ�����UPNP��Ϣ
//@EXECUTIONFLOW:16������CSUDIRTStopService�����ر�DNS����ɹ�
//@EXECUTIONFLOW:17������CSUDIRTStopService�����ر�IGMP Proxy����ɹ�
//@EXECUTIONFLOW:18������CSUDIRTStopService�����ر�UPNP����ɹ�
//@EXECUTIONFLOW:19������CSUDIRTExecuteIptableCmd����ִ��iptables -t nat -D POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE����
//@EXECUTIONFLOW:20����pc1��������ethereal capture 192.168.10.90 �������ݰ�
//@EXECUTIONFLOW:21����pc0 ��������� www.baidu.com
//@EXECUTIONFLOW:22���鿴pc1 ethereal ���沶������ݰ��в�����dns ����www.baidu.com �����ݰ�
//@EXECUTIONFLOW:23��PC0��vlc���治�ܹ�������������
//@EXECUTIONFLOW:24��PC0���棬ʹ��Xlight FTP�������������,���ܹ�����UPNP��Ϣ
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0006(void)
{	
	char szIptableCmd[128];
	CSUDI_BOOL bDNSProxyStart = CSUDI_FALSE;
	CSUDI_BOOL bIGMPProxyStart = CSUDI_FALSE;
	CSUDI_BOOL bUPNPStart = CSUDI_FALSE;
	CSUDIRTDnsServers_S  sStartParam;

	memcpy(sStartParam.acFirstDNS, g_szPcWanIp, CSUDI_APRT_IP_LEN);
	memcpy(sStartParam.acSecondDNS, g_szPcWanIp, CSUDI_APRT_IP_LEN);

	system("echo 1 > /proc/sys/net/ipv4/ip_forward");
	system("ifconfig lo up");

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(g_sRouterInfo.szWan, g_sRouterInfo.szWanIp), 
		"����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"����3ʧ��");

	CSTCPrint("����lan�ڵ�ip��ַ��ϢΪ ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0��lan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
    CSTCPrint("2:����pc0 mask��ַ��lan�� mask��ͬ\n");
	CSTCPrint("3:����pc0 dns Ϊlan��ip��ַ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����4ʧ�ܣ�û����ȷ����pc0 ������Ϣ");

	CSTCPrint("����wan�ڵ�ip��ַ��ϢΪ ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1��wan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����5ʧ�ܣ�û����ȷ����pc1 ������Ϣ");

	
	sprintf(szIptableCmd, "iptables -t nat -A POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, &sStartParam),
		"����7ʧ��");
        bDNSProxyStart = CSUDI_TRUE;
        
        memset(sStartParam.acFirstDNS, '\0', CSUDI_APRT_IP_LEN);
        memset(sStartParam.acSecondDNS, '\0', CSUDI_APRT_IP_LEN);
        
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_IGMP_PROXY, &bDNSProxyStart),
		"����8ʧ��");
	bIGMPProxyStart = CSUDI_TRUE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_UPNP, &bDNSProxyStart),
		"����9ʧ��");
	bUPNPStart = CSUDI_TRUE;

	CSTCPrint("1:��pc1��������ethereal�����wan���������ڵ����ݰ�\n");
	CSTCPrint("pc1���Ƿ�����ethereal�������ݰ�\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����10ʧ�ܣ�û����ȷ����ethereal");
	CSTCPrint("1:��pc0 ��������� www.baidu.com\n");
	CSTCPrint("pc0 ������Ƿ���� www.baidu.com\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����11ʧ�ܣ�û�з���www.baidu.com");
	CSTCPrint("1:�鿴ethereal��������ݰ�\n");
	CSTCPrint("���ݰ����Ƿ����pc0����dns�����ݰ�?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����12ʧ�ܣ�������dns���ݰ�");
	
	CSTCPrint("1:��pc1��������WindSend��������\n");
	CSTCPrint("pc1���Ƿ�����WindSend��������\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����13ʧ�ܣ�û����ȷ����WindSend");
	CSTCPrint("1:��pc0 ʹ��VLC���߲�������\n");
	CSTCPrint("pc0 �Ƿ�ʹ��VLC���߲�������\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����14ʧ�ܣ�û��ʹ��VLC���߲�������");
	CSTCPrint("PC0���棬�ܷ���������WindSend���沥�ŵ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����15ʧ�ܣ�����������������");

	CSTCPrint("1:��pc0 ʹ��Xlight FTP���߲���\n");
	CSTCPrint("pc0 �Ƿ��ܹ�����UPNP�������Ϣ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����16ʧ�ܣ�pc0 ���ܿ���UPNP�������Ϣ");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
		"����17ʧ��");
	bDNSProxyStart = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
		"����18ʧ��");
	bIGMPProxyStart = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
		"����19ʧ��");
	bUPNPStart = CSUDI_FALSE;

	CSTCPrint("1:��pc1��������ethereal�����wan���������ڵ����ݰ�\n");
	CSTCPrint("pc1���Ƿ�����ethereal�������ݰ�\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����20ʧ�ܣ�û����ȷ����ethereal");
	CSTCPrint("1:��pc0 ��������� www.baidu.com\n");
	CSTCPrint("pc0 ������Ƿ���� www.baidu.com\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����21ʧ�ܣ�û�з���www.baidu.com");
	CSTCPrint("1:�鿴ethereal��������ݰ�\n");
	CSTCPrint("���ݰ����Ƿ����pc0����dns�����ݰ�?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����22ʧ�ܣ�������dns���ݰ�");
	CSTCPrint("PC0���棬�ܷ���������WindSend���沥�ŵ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����23ʧ�ܣ���������������");
	CSTCPrint("1:��pc0 ʹ��Xlight FTP���߲���\n");
	CSTCPrint("pc0 �Ƿ��ܹ�����UPNP�������Ϣ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"����24ʧ�ܣ�pc0 �ܿ���UPNP�������Ϣ");

	sprintf(szIptableCmd, "iptables -t nat -D POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"����25ʧ��");

	CSTK_FATAL_POINT;
	if (bDNSProxyStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
			"����26ʧ��");
	}
	if (bIGMPProxyStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
			"����27ʧ��");		
	}
	if (bUPNPStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
			"����28ʧ��");		
	}

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIRTGetServiceStatus
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIRTGetServiceStatus������ִ��Ч��
//@PRECONDITION:ROUTERģ�鱻������ʼ��
//@INPUT:1��eServiceΪ�Ƿ�����
//@INPUT:2��pbIsRunningΪ�Ƿ�����
//@EXPECTATION:���е��÷��ط�CSNET_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��eServiceΪ-1�� ����CSUDIRTGetServiceStatus�������ã����ط�CSNET_SUCCESS
//@EXECUTIONFLOW:2��eServiceΪEM_RT_SERVICETYPE_MAX�� ����CSUDIRTGetServiceStatus�������ã����ط�CSNET_SUCCESS
//@EXECUTIONFLOW:3��eServiceΪEM_RT_SERVICETYPE_MAX+1�� ����CSUDIRTGetServiceStatus�������ã����ط�CSNET_SUCCESS
//@EXECUTIONFLOW:4��eServiceΪEM_RT_SERVICETYPE_IGMP_PROXY,pbIsRunningΪNULL,����CSUDIRTGetServiceStatus�������ã����ط�CSNET_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTGetServiceStatus_0001(void)
{
	CSUDI_BOOL bIsRunning = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetServiceStatus(-1, &bIsRunning),
		"����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_MAX, &bIsRunning),
		"����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_MAX+1, &bIsRunning),
		"����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_IGMP_PROXY, CSUDI_NULL),
		"����4ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTGetServiceStatus
//@DESCRIPTION:���Բ�����Ӧ�ó����ڷ��Ͻӿ����Ҫ��CSUDIRTGetServiceStatus������ִ��Ч��
//@PRECONDITION:ROUTERģ�鱻������ʼ��
//@PRECONDITION:STB����·��ת��STB:echo 1 > /proc/sys/net/ipv4/ip_forward
//@INPUT:1��eServiceΪEM_RT_SERVICETYPE_DNS_PROXY\EM_RT_SERVICETYPE_IGMP_PROXY\EM_RT_SERVICETYPE_UPNP
//@INPUT:2��pbIsRunningΪ�ǿղ���
//@EXPECTATION:CSNETGetServiceStatus�����ܹ���ȷ�Ļ�ȡ��������״̬
//@REMARK:
//@EXECUTIONFLOW:1������(eth1 ipΪ192.168.10.68)
//@EXECUTIONFLOW:2������CSUDIRTSetInterfaces����wan eth1,lan ra0
//@EXECUTIONFLOW:3������CSUDIRTSetLanIp����lan ��IP��ַΪ172.30.2.1
//@EXECUTIONFLOW:4��lan�����ӵ�PC0 ��������(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5��wan�����ӵ�PC1 ��������(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6������CSUDIRTExecuteIptableCmd����ִ��iptables -t nat -A POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE����
//@EXECUTIONFLOW:7������CSUDIRTStartService��������EM_RT_SERVICETYPE_DNS_PROXY((nameserver 192.168.10.90))����
//@EXECUTIONFLOW:8������CSUDIRTStartService��������EM_RT_SERVICETYPE_IGMP_PROXY����
//@EXECUTIONFLOW:9������CSUDIRTStartService��������EM_RT_SERVICETYPE_UPNP����
//@EXECUTIONFLOW:10������CSUDIRTGetServiceStatus������ȡEM_NET_DNS_PROXY����״̬ΪTRUE
//@EXECUTIONFLOW:11������CSUDIRTGetServiceStatus������ȡEM_NET_IGMP_PROXY����״̬ΪTRUE
//@EXECUTIONFLOW:12������CSUDIRTGetServiceStatus������ȡEM_RT_SERVICETYPE_UPNP����״̬ΪTRUE
//@EXECUTIONFLOW:13������CSUDIRTStopService����ֹͣEM_RT_SERVICETYPE_DNS_PROXY����
//@EXECUTIONFLOW:14������CSUDIRTStopService����ֹͣEM_RT_SERVICETYPE_IGMP_PROXY����
//@EXECUTIONFLOW:15������CSUDIRTStopService����ֹͣEM_RT_SERVICETYPE_UPNP����
//@EXECUTIONFLOW:16������CSUDIRTExecuteIptableCmd����ִ��iptables -t nat -D POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE����
//@EXECUTIONFLOW:17������CSUDIRTGetServiceStatus������ȡEM_RT_SERVICETYPE_DNS_PROXY����״̬ΪFALSE
//@EXECUTIONFLOW:18������CSUDIRTGetServiceStatus������ȡEM_RT_SERVICETYPE_IGMP_PROXY����״̬ΪFALSE
//@EXECUTIONFLOW:19������CSUDIRTGetServiceStatus������ȡEM_RT_SERVICETYPE_UPNP����״̬ΪFALSE
CSUDI_BOOL CSTC_ROUTER_CSUDIRTGetServiceStatus_0002(void)
{	
	char szIptableCmd[128];
	CSUDI_BOOL bDNSProxyStart = CSUDI_FALSE;
	CSUDI_BOOL bIGMPProxyStart = CSUDI_FALSE;
	CSUDI_BOOL bUPNPStart = CSUDI_FALSE;
	CSUDI_BOOL bIsRunning = CSUDI_FALSE;
	CSUDIRTDnsServers_S  sStartParam;

	memcpy(sStartParam.acFirstDNS, g_szPcWanIp, CSUDI_APRT_IP_LEN);
	memcpy(sStartParam.acSecondDNS, g_szPcWanIp, CSUDI_APRT_IP_LEN);

	system("echo 1 > /proc/sys/net/ipv4/ip_forward");
    	
	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(g_sRouterInfo.szWan, g_sRouterInfo.szWanIp), 
		"����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"����3ʧ��");

	CSTCPrint("����lan�ڵ�ip��ַ��ϢΪ ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0��lan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
    CSTCPrint("2:����pc0 mask��ַ��lan�� mask��ͬ\n");
	CSTCPrint("3:����pc0 dns Ϊlan��ip��ַ\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����4ʧ�ܣ�û����ȷ����pc0 ������Ϣ");

	CSTCPrint("����wan�ڵ�ip��ַ��ϢΪ ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1��wan������,����pc0 ip��ַ��lan��ip��ַ��ͬһ����\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"����5ʧ�ܣ�û����ȷ����pc1 ������Ϣ");
	
	sprintf(szIptableCmd, "iptables -t nat -A POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, &sStartParam),
		"����7ʧ��");
	bDNSProxyStart = CSUDI_TRUE;

        memset(sStartParam.acFirstDNS, '\0', CSUDI_APRT_IP_LEN);
        memset(sStartParam.acSecondDNS, '\0', CSUDI_APRT_IP_LEN);

        CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_IGMP_PROXY, &sStartParam),
		"����8ʧ��");
	bIGMPProxyStart = CSUDI_TRUE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_UPNP, &sStartParam),
		"����9ʧ��");
	bUPNPStart = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_DNS_PROXY, &bIsRunning),
		"����10ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == bIsRunning, "����10ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_IGMP_PROXY, &bIsRunning),
		"����11ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == bIsRunning, "����11ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_UPNP, &bIsRunning),
		"����12ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == bIsRunning, "����12ʧ��");

	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
		"����13ʧ��");
	bDNSProxyStart = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
		"����14ʧ��");
	bIGMPProxyStart = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
		"����15ʧ��");
	bUPNPStart = CSUDI_FALSE;

	sprintf(szIptableCmd, "iptables -t nat -D POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"����16ʧ��");


	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_DNS_PROXY, &bIsRunning),
		"����17ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bIsRunning, "����17ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_IGMP_PROXY, &bIsRunning),
		"����18ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bIsRunning, "����18ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_UPNP, &bIsRunning),
		"����19ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bIsRunning, "����19ʧ��");


	CSTK_FATAL_POINT;
	if (bDNSProxyStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
			"����20ʧ��");
	}
	if (bIGMPProxyStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
			"����21ʧ��");		
	}
	if (bUPNPStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
			"����22ʧ��");		
	}

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTExecuteIptableCmd
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIRTExecuteIptableCmd������ִ��Ч��
//@PRECONDITION:ROUTERģ�鱻������ʼ��
//@INPUT:1��pszcmd��������ȷ
//@EXPECTATION:���е��÷��ط�CSNET_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1��pszcmdΪ��,����CSUDIRTExecuteIptableCmd���������ط�CSNET_SUCCESS
CSUDI_BOOL  CSTC_ROUTER_CSUDIRTExecuteIptableCmd_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTExecuteIptableCmd(CSUDI_NULL),
		"����1ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


