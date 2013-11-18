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
	char szNicIp[DHCPS_NETWORK_SEGMENT_COUNT][MAX_IP_LEN]; //nic地址和网段相对应
	CSUDIDHCPSConfig_S sConfig[DHCPS_NETWORK_SEGMENT_COUNT];
	CSUDIDHCPSStaticIpCfg sStaticIpCfg; //保存一组静态Ip信息
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

	//获取静态ip mac对
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
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIDHCPSStart函数的执行效果
//@PRECONDITION:
//@INPUT:1、szIfName参数不符合设计要求
//@EXPECTATION:调用返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为NULL，调用CSUDIDHCPSStart，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szIfName为不存在的设备(如"eth100")，调用CSUDIDHCPSStart，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szIfName为回环设备名称(如"lo")，调用CSUDIDHCPSStart，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSStartStop_0001(void)
{
	char szIfName[16];

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStart(CSUDI_NULL),
		"步骤1失败");	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStart(g_sDhcpsInfo.szInvalidNic),
		"步骤2失败");
	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStart(szIfName),
		"步骤3失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSStart&CSUDIDHCPSStop
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIDHCPSStop函数的执行效果
//@PRECONDITION:
//@INPUT:1、szIfName参数不符合设计要求
//@EXPECTATION:调用返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为NULL，调用CSUDIDHCPSStop，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szIfName为不存在的设备(如"eth100")，调用CSUDIDHCPSStop，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szIfName为回环设备名称(如"lo")，调用CSUDIDHCPSStop，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSStartStop_0002(void)
{
	char szIfName[16];

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStop(CSUDI_NULL),
		"步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStop(g_sDhcpsInfo.szInvalidNic),
		"步骤2失败");
	
	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSStop(szIfName),
		"步骤3失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSStart&CSUDIDHCPSStop&CSUDIDHCPSConfig
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIDHCPSStart&CSUDIDHCPSStop函数的执行效果
//@PRECONDITION:
//@INPUT:1、szIfName参数符合设计要求
//@INPUT:2、psDHCPCfg参数符合设计要求
//@EXPECTATION:DHCPS更够正确的启动和停止
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为正确的网卡名称(如"eth1"， IP:172.30.2.1)
//@EXECUTIONFLOW:2、设置psDHCPCfg参数正确，(如acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3、调用CSUDIDHCPSConfig函数，设置DHCPS参数
//@EXECUTIONFLOW:4、调用CSUDIDHCPSStart函数，启动DHCPS
//@EXECUTIONFLOW:5、PC0和eth1连接，在PC0上面启动Ethereal抓包工具，捕获pc0上和eth1连接的网口
//@EXECUTIONFLOW:6、PC0动态获取IP地址，查看Ethereal捕获的数据包
//@EXECUTIONFLOW:7、比较Ethereal捕获的数据包和psDHCPCfg参数是否一致
//@EXECUTIONFLOW:8、调用CSUDIDHCPSStop函数，停止DHCPS
//@EXECUTIONFLOW:9、和eth1连接的PC0 不能够正常获取到IP地址
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSStartStop_0003(void)
{
	char szIfName[16];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIDHCPSConfig_S sDHCPCfg;
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	//设置ip地址
	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(szIfName, g_sDhcpsInfo.szNicIp[0]), 
		"步骤1失败");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPEnd);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName),
		"步骤3失败");
	bStart = CSUDI_TRUE;

	CSTCPrint("现在的DHCPS信息为:\nstartip=%s  \nendip=%s \nmask=%s  \ngataway=%s  \n", 
		g_sDhcpsInfo.sConfig[0].acIPStart, 
		g_sDhcpsInfo.sConfig[0].acIPEnd,
		g_sDhcpsInfo.sConfig[0].acMask, 
		g_sDhcpsInfo.sConfig[0].acGatway);
	
	CSTCPrint("1:PC0上面使用Ethereal抓包工具，捕获pc0上和eth1连接的网口\n");
    CSTCPrint("2:pc0上和eth1连接的网口，请求IP地址\n");
	CSTCPrint("Ethereal抓获的包是否和设置的DHCPS信息一致?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤4失败：获取信息不正确");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), 
		"步骤5失败");
	bStart = CSUDI_FALSE;

	CSTCPrint("DHCPS服务器已停止，请断开pc0与eth1的连接，重新请求ip地址\n");
    CSTCPrint("1:pc0上和eth1连接的网口，请求IP地址\n");
	CSTCPrint("pc0 是否分配到ip地址?\n");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤6失败：获取信息不正确");
	
	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),
			"步骤7失败");
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSStart&CSUDIDHCPSStop&CSUDIDHCPSConfig
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIDHCPSStart&CSUDIDHCPSStop函数的执行1000次效果
//@PRECONDITION:
//@INPUT:1、szIfName参数符合设计要求
//@INPUT:2、psDHCPCfg参数符合设计要求
//@EXPECTATION:DHCPS进行开关强度测试后能够正常工作
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为正确的网卡名称(如"eth1"， IP:172.30.2.1)
//@EXECUTIONFLOW:2、设置psDHCPCfg参数正确，(如acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3、调用CSUDIDHCPSConfig函数，设置DHCPS参数
//@EXECUTIONFLOW:4、调用CSUDIDHCPSStart、CSUDIDHCPSStop函数1000次
//@EXECUTIONFLOW:5、调用CSUDIDHCPSStart函数,启动DHCPS服务器
//@EXECUTIONFLOW:6、PC0和eth1连接，在PC0上面启动Ethereal抓包工具，捕获pc0上和eth1连接的网口
//@EXECUTIONFLOW:7、PC0动态获取IP地址，查看Ethereal捕获的数据包
//@EXECUTIONFLOW:8、比较Ethereal捕获的数据包和psDHCPCfg参数是否一致
//@EXECUTIONFLOW:9、调用CSUDIDHCPSStop函数，停止DHCPS
//@EXECUTIONFLOW:10、和eth1连接的PC0 不能够正常获取到IP地址
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSStartStop_0004(void)
{
	int nCount = 0;
	char szIfName[16];
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIDHCPSConfig_S sDHCPCfg;
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(szIfName, g_sDhcpsInfo.szNicIp[0]), 
		"步骤1失败");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPEnd);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"步骤2失败");

	CSTCPrint("开始进行强度测试\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤3失败：不进行强度测试");
	
	while(nCount < 1000)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
			"步骤4失败");
		bStart = CSUDI_TRUE;
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), 
			"步骤5失败");
		bStart = CSUDI_FALSE;
		nCount++;
	}
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
		"步骤6失败");
	bStart = CSUDI_TRUE;

	CSTCPrint("1:PC0上面使用Ethereal抓包工具，捕获pc0上和eth1连接的网口\n");
    CSTCPrint("2:pc0上和eth1连接的网口，请求IP地址\n");
	CSTCPrint("Ethereal抓获的包是否和设置的DHCPS信息一致?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤4失败：获取信息不正确");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), 
		"步骤7失败");
	bStart = CSUDI_FALSE;

	CSTCPrint("DHCPS服务器已停止，请断开pc0与eth1的连接，重新请求ip地址\n");
    CSTCPrint("1:pc0上和eth1连接的网口，请求IP地址\n");
	CSTCPrint("pc0 是否分配到ip地址?\n");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤8失败：获取信息不正确");
	
	CSTK_FATAL_POINT;
	
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),
			"步骤9失败");
	}
	
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIDHCPSConfig&CSUDIDHCPSGetConfig
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIDHCPSConfig函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:
//@REMARK:非法ip如 172.30.1.256 
//@REMARK:		   172.30..2
//@REMARK:		   1723022
//@EXECUTIONFLOW:1、szIfName为NULL，调用CSUDIDHCPSConfig，返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szIfName为不存在的设备(如"eth100")，调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szIfName为回环设备名称(如"lo")，调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、szIfName为正确的参数(如"eth1"),psDHCPCfg为空，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、szIfName为正确的参数(如"eth1"),psDHCPCfg不为空，acIPStart为非法IP,调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、szIfName为正确的参数(如"eth1"),psDHCPCfg不为空，acIPStart为正常IP，acIPEnd为非法IP,调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、szIfName为正确的参数(如"eth1"),psDHCPCfg不为空，acIPStart acIPEnd为正常IP，但不在同一网段,调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、szIfName为正确的参数(如"eth1"),psDHCPCfg不为空，acIPStart acIPEnd为正常IP，acMask为非法格式,调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、szIfName为正确的参数(如"eth1"),psDHCPCfg不为空，acIPStart acIPEnd为正常IP，acMask为正常格式，acGatway为非法IP,调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、szIfName为正确的参数(如"eth1"),psDHCPCfg不为空，acIPStart acIPEnd为正常IP，acMask为正常格式，acGatway为正常IP，但是acIPStart acIPEnd acGatway不在同一网段，调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:11、szIfName为正确的参数(如"eth1"),psDHCPCfg不为空，acIPStart acIPEnd为正常IP，acMask为正常格式，acGatway为正常IP，但是acIPStart acIPEnd acGatway在同一网段，acFirstDNS为空，调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、szIfName为正确的参数(如"eth1"),psDHCPCfg不为空，acIPStart acIPEnd为正常IP，acMask为正常格式，acGatway为正常IP，但是acIPStart acIPEnd acGatway在同一网段，acFirstDNS为非法IP，调用CSUDIDHCPSConfig，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSConfigGetConfig_0001(void)
{
	char szIfName[16];
	CSUDIDHCPSConfig_S sDHCPCfg;

	memset(&sDHCPCfg, 0x00, sizeof(CSUDIDHCPSConfig_S));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(CSUDI_NULL, &sDHCPCfg),
		"步骤1失败");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤2失败");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤3失败");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, CSUDI_NULL),
		"步骤4失败");


	strncpy(sDHCPCfg.acIPStart, "172.30.1.256",sizeof(sDHCPCfg.acIPStart));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤5失败");


	strncpy(sDHCPCfg.acIPStart, "172.30.2.2",sizeof(sDHCPCfg.acIPStart));	
	strncpy(sDHCPCfg.acIPEnd, "172.30.1.256",sizeof(sDHCPCfg.acIPEnd));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤6失败");

	strncpy(sDHCPCfg.acIPEnd, "172.30.1.254",sizeof(sDHCPCfg.acIPEnd));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤7失败");

	
	strncpy(sDHCPCfg.acIPEnd, "172.30.2.254",sizeof(sDHCPCfg.acIPEnd));	
	strncpy(sDHCPCfg.acMask, "172.30.1.256",sizeof(sDHCPCfg.acMask));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤8失败");

	strncpy(sDHCPCfg.acMask, "255.255.255.0",sizeof(sDHCPCfg.acMask));
	strncpy(sDHCPCfg.acGatway, "172.30.1.256",sizeof(sDHCPCfg.acGatway));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤9失败");
	strncpy(sDHCPCfg.acGatway, "172.30.1.1",sizeof(sDHCPCfg.acGatway));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤10失败");

	strncpy(sDHCPCfg.acGatway, "172.30.2.1",sizeof(sDHCPCfg.acGatway));
	memset(sDHCPCfg.acFirstDNS, 0x00, sizeof(sDHCPCfg.acFirstDNS));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤11失败");
	strncpy(sDHCPCfg.acFirstDNS, "172.30.2.256",sizeof(sDHCPCfg.acFirstDNS));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSConfig(szIfName, &sDHCPCfg),
		"步骤12失败");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSConfig&CSUDIDHCPSGetConfig
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIDHCPSGetConfig函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为NULL，调用CSUDIDHCPSGetConfig，返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szIfName为不存在的设备(如"eth100")，调用CSUDIDHCPSGetConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szIfName为回环设备名称(如"lo")，调用CSUDIDHCPSGetConfig，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、szIfName为正确的参数(如"eth1"),psDHCPCfg为空，调用CSUDIDHCPSGetConfig，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSConfigGetConfig_0002(void)
{
	char szIfName[16];
	CSUDIDHCPSConfig_S sDHCPCfg;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetConfig(CSUDI_NULL,&sDHCPCfg),
		"步骤1失败");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetConfig(szIfName,&sDHCPCfg),
		"步骤2失败");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetConfig(szIfName,&sDHCPCfg),
		"步骤3失败");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetConfig(szIfName,CSUDI_NULL),
		"步骤4失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIDHCPSConfig&CSUDIDHCPSGetConfig
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIDHCPSConfig&CSUDIDHCPSGetConfig函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数符合设计要求
//@EXPECTATION:DHCPS更够正确的设置和获取参数
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为正确的网卡名称(如"eth1"， IP:172.30.2.1)
//@EXECUTIONFLOW:2、设置psDHCPCfg参数正确，(如acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3、调用CSUDIDHCPSConfig函数，设置DHCPS参数
//@EXECUTIONFLOW:4、调用CSUDIDHCPSStart函数，启动DHCPS
//@EXECUTIONFLOW:5、和eth1连接的PC0 能够正常获取172.30.2.2~172.30.2.254段地址
//@EXECUTIONFLOW:6、调用CSUDIDHCPSGetConfig函数，获取的参数和设置的参数相同
//@EXECUTIONFLOW:7、调用CSUDIDHCPSStop函数，停止DHCPS
//@EXECUTIONFLOW:8、和eth1连接的PC0 不能够正常获取到IP地址
//@EXECUTIONFLOW:9、szIfName为正确的网卡名称(如"eth1"， IP:172.30.10.1)
//@EXECUTIONFLOW:10、设置psDHCPCfg参数正确，(如acIPStart:172.30.10.2,acIPEnd:172.30.10.254,acMask:255.255.0.0 acGatway:172.30.10.1) 
//@EXECUTIONFLOW:11、调用CSUDIDHCPSConfig函数，设置DHCPS参数
//@EXECUTIONFLOW:12、调用CSUDIDHCPSStart函数，启动DHCPS
//@EXECUTIONFLOW:13、和eth1连接的PC0 能够正常获取172.30.10.2~172.30.10.254段地址
//@EXECUTIONFLOW:14、调用CSUDIDHCPSGetConfig函数，获取的参数和第二次设置的参数相同
//@EXECUTIONFLOW:15、调用CSUDIDHCPSStop函数，停止DHCPS
//@EXECUTIONFLOW:16、和eth1连接的PC0 不能够正常获取到IP地址
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
			"步骤1失败");

		strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[i].acIPStart);
		strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[i].acIPEnd);
		strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[i].acMask);
		strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[i].acGatway);
		strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[i].acFirstDNS);
		strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[i].acSecondDNS);
		sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[i].uLeaseTime;

		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
			"步骤2失败");
		
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
			"步骤3失败");
		bStart = CSUDI_TRUE;

		CSTCPrint("1:PC0上面使用Ethereal抓包工具，捕获pc0上和eth1连接的网口\n");
	    CSTCPrint("2:pc0上和eth1连接的网口，请求IP地址\n");
		CSTCPrint("Ethereal抓获的包是否和设置的DHCPS信息一致?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败：获取信息不正确");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSGetConfig(szIfName,&sDHCPCfg1),
			"步骤4失败");

		nRet = strcmp(sDHCPCfg.acIPStart, sDHCPCfg1.acIPStart);
		nRet |= strcmp(sDHCPCfg.acIPEnd, sDHCPCfg1.acIPEnd);
		nRet |= strcmp(sDHCPCfg.acMask, sDHCPCfg1.acMask);
		nRet |= strcmp(sDHCPCfg.acGatway, sDHCPCfg1.acGatway);
		nRet |= strcmp(sDHCPCfg.acFirstDNS, sDHCPCfg1.acFirstDNS);
		nRet |= strcmp(sDHCPCfg.acSecondDNS, sDHCPCfg1.acSecondDNS);
		nRet |= (sDHCPCfg.uLeaseTime == sDHCPCfg1.uLeaseTime)?0:1;
		
		CSTK_ASSERT_TRUE_FATAL(0 == nRet,"步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), 
			"步骤6失败");
		bStart = CSUDI_FALSE;

		CSTCPrint("DHCPS服务器已停止，请断开pc0与eth1的连接，重新请求ip地址\n");
	    CSTCPrint("1:pc0上和eth1连接的网口，请求IP地址\n");
		CSTCPrint("pc0是否分配到ip地址?\n");
		
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_FALSE == CSTKWaitYes(),
			"步骤7失败：获取信息不正确");
	}

	CSTK_FATAL_POINT;
	
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),
			"步骤8失败");
	}
	
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIDHCPSAddStaticIPCfg函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为NULL，调用CSUDIDHCPSAddStaticIPCfg，返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szIfName为不存在的设备(如"eth100")，调用CSUDIDHCPSAddStaticIPCfg，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szIfName为回环设备名称(如"lo")，调用CSUDIDHCPSAddStaticIPCfg，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、szIfName为正确的参数(如"eth1"),psItem为空，调用CSUDIDHCPSAddStaticIPCfg，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、szIfName为正确的参数(如"eth1"),psItem不为空，nCount<=0，调用CSUDIDHCPSAddStaticIPCfg，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、szIfName为正确的参数(如"eth1"),psItem不为空，nCount>0，psItem->acIP非法调用CSUDIDHCPSAddStaticIPCfg，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0001(void)
{
	char szIfName[16];
	CSUDIDHCPSStaticIpCfg sItem;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(CSUDI_NULL,&sItem,1),
		"步骤1失败");

	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,1),
		"步骤2失败");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,1),
		"步骤3失败");
	
	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,CSUDI_NULL,1),
		"步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,0),
		"步骤5失败");

	strncpy(sItem.acIP,"0.0.0", sizeof(sItem.acIP));
	memcpy(sItem.aucMac, g_sDhcpsInfo.sStaticIpCfg.aucMac, sizeof(sItem.aucMac));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,1),
		"步骤6失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIDHCPSDelStaticIPCfg函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为NULL，调用CSUDIDHCPSDelStaticIPCfg，返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szIfName为不存在的设备(如"eth100")，调用CSUDIDHCPSDelStaticIPCfg，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szIfName为回环设备名称(如"lo")，调用CSUDIDHCPSDelStaticIPCfg，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、szIfName为正确的参数(如"eth1"),psItem为空，调用CSUDIDHCPSDelStaticIPCfg，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、szIfName为正确的参数(如"eth1"),psItem不为空，nCount<=0，调用CSUDIDHCPSDelStaticIPCfg，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、szIfName为正确的参数(如"eth1"),psItem不为空，nCount>0，psItem->acIP非法调用CSUDIDHCPSDelStaticIPCfg，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0002(void)
{
	char szIfName[16];
	CSUDIDHCPSStaticIpCfg sItem;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(CSUDI_NULL,&sItem,1),
		"步骤1失败");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,1),
		"步骤2失败");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,1),
		"步骤3失败");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,CSUDI_NULL,1),
		"步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,0),
		"步骤5失败");

	strncpy(sItem.acIP,"0.0.0", sizeof(sItem.acIP));
	memcpy(sItem.aucMac, g_sDhcpsInfo.sStaticIpCfg.aucMac, sizeof(sItem.aucMac));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,1),
		"步骤6失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIDHCPSGetStaticIPCfgs函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为NULL，调用CSUDIDHCPSGetStaticIPCfgs，返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szIfName为不存在的设备(如"eth100")，调用CSUDIDHCPSGetStaticIPCfgs，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szIfName为回环设备名称(如"lo")，调用CSUDIDHCPSGetStaticIPCfgs，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、szIfName为正确的参数(如"eth1"),psItem为空，调用CSUDIDHCPSGetStaticIPCfgs，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、szIfName为正确的参数(如"eth1"),psItem不为空，pnStaticCount为空，调用CSUDIDHCPSGetStaticIPCfgs，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、szIfName为正确的参数(如"eth1"),psItem不为空，*pnStaticCount<=0，调用CSUDIDHCPSGetStaticIPCfgs，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0003(void)
{
	char szIfName[16];
	CSUDIDHCPSStaticIpCfg sItem;
	int nStaticCount = 1;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(CSUDI_NULL,&sItem,&nStaticCount),
		"步骤1失败");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem,&nStaticCount),
		"步骤2失败");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem,&nStaticCount),
		"步骤3失败");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,CSUDI_NULL,&nStaticCount),
		"步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem,CSUDI_NULL),
		"步骤5失败");
	
	nStaticCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem,&nStaticCount),
		"步骤6失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}




//@CASEGROUP:CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIDHCPSAddStaticIPCfg&CSUDIDHCPSDelStaticIPCfg&CSUDIDHCPSGetStaticIPCfgs函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数符合设计要求
//@EXPECTATION:DHCPS更够正确的设置和获取参数
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为正确的网卡名称(如"eth1"， IP:172.30.2.1)
//@EXECUTIONFLOW:2、设置psDHCPCfg参数正确，(如acIPStart:172.30.2.2,acIPEnd:172.30.2.2,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3、调用CSUDIDHCPSConfig函数，设置DHCPS参数
//@EXECUTIONFLOW:4、调用CSUDIDHCPSAddStaticIPCfg函数，设置静态IP(172.30.2.2)与mac(0xaa,0xbb,0xcc,0xdd,0xee,0xff)
//@EXECUTIONFLOW:5、调用CSUDIDHCPSStart函数，启动DHCPS
//@EXECUTIONFLOW:6、和eth1连接的PC0 mac(0xaa,0xbb,0xcc,0xdd,0xee,0xff),能动态获取172.30.2.2
//@EXECUTIONFLOW:7、和eth1连接的PC1 mac不为(0xaa,0xbb,0xcc,0xdd,0xee,0xff),不能动态获取到IP地址
//@EXECUTIONFLOW:8、调用CSUDIDHCPSGetStaticIPCfgs函数，获取到的参数和设置的参数相同
//@EXECUTIONFLOW:9、调用CSUDIDHCPSStop函数，停止DHCPS
//@EXECUTIONFLOW:10、调用CSUDIDHCPSDelStaticIPCfg函数，删除静态IP和Mac
//@EXECUTIONFLOW:11、调用CSUDIDHCPSGetStaticIPCfgs函数，获取到的参数为空
//@EXECUTIONFLOW:12、调用CSUDIDHCPSStart函数，启动DHCPS
//@EXECUTIONFLOW:13、和eth1连接的PC0 mac(0xaa,0xbb,0xcc,0xdd,0xee,0xff),能动态获取172.30.2.2
//@EXECUTIONFLOW:14、和eth1连接的PC1 mac不为(0xaa,0xbb,0xcc,0xdd,0xee,0xff),能动态获取172.30.2.2
//@EXECUTIONFLOW:15、调用CSUDIDHCPSStop函数，停止DHCPS
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
		"步骤1失败");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"步骤2失败");

	strncpy(sItem.acIP,g_sDhcpsInfo.sStaticIpCfg.acIP, sizeof(sItem.acIP));
	memcpy(sItem.aucMac, g_sDhcpsInfo.sStaticIpCfg.aucMac, sizeof(sItem.aucMac));

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSAddStaticIPCfg(szIfName,&sItem,sizeof(sItem)/sizeof(CSUDIDHCPSStaticIpCfg)),
		"步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
		"步骤4失败");
	bStart = CSUDI_TRUE;

	CSTCPrint("pc0(mac地址和配置文件中一致) 上和eth1连接的网口，请求IP地址\n");
	CSTCPrint("pc0能否请求到IP地址?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤5失败：获取信息不正确");

	CSTCPrint("断开pc0与eth1的连接，使用pc1请求ip地址\n");
	CSTCPrint("pc1(mac地址和配置文件中不一致) 上和eth1连接的网口，请求IP地址\n");
	CSTCPrint("pc1能否请求到IP地址?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤6失败：获取信息不正确");

	nStaticCount = 1;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem1,&nStaticCount),
		"步骤7失败");
	if (nStaticCount != 1)
	{
		nRet = -1;
	}
	else
	{
		nRet = strcmp(sItem.acIP, sItem1.acIP);
		nRet |= memcmp(sItem.aucMac, sItem1.aucMac, sizeof(sItem.aucMac));
	}
	
	CSTK_ASSERT_TRUE_FATAL(0 == nRet,"步骤8失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), "步骤9失败");
	bStart = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSDelStaticIPCfg(szIfName,&sItem,sizeof(sItem1)/sizeof(CSUDIDHCPSStaticIpCfg)),
		"步骤10失败");
	nStaticCount = 1;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetStaticIPCfgs(szIfName,&sItem1,&nStaticCount),
		"步骤11失败");
	CSTK_ASSERT_TRUE_FATAL(0 == nStaticCount,"步骤12失败");


	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), "步骤13失败");
	bStart = CSUDI_TRUE;

	CSTCPrint("pc0(mac地址和配置文件中一致) 上和eth1连接的网口，请求IP地址\n");
	CSTCPrint("pc0能否请求到IP地址?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败：获取信息不正确");

	CSTCPrint("断开pc0与eth1的连接，使用pc1请求ip地址\n");
	CSTCPrint("pc1(mac地址和配置文件中不一致) 上和eth1连接的网口，请求IP地址\n");
	CSTCPrint("pc1能否请求到IP地址?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤15失败：获取信息不正确");

	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),"步骤16失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDHCPSGetLeaseInfo
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIDHCPSGetLeaseInfo函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1、szIfName为NULL，调用CSUDIDHCPSGetLeaseInfo，返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szIfName为不存在的设备(如"eth100")，调用CSUDIDHCPSGetLeaseInfo，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szIfName为回环设备名称(如"lo")，调用CSUDIDHCPSGetLeaseInfo，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、szIfName为正确的参数(如"eth1"),pstLeaseInfo为空，调用CSUDIDHCPSGetLeaseInfo，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、szIfName为正确的参数(如"eth1"),pstLeaseInfo不为空，pnCount为空，调用CSUDIDHCPSGetLeaseInfo，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、szIfName为正确的参数(如"eth1"),pstLeaseInfo不为空，*pnCount<=0，调用CSUDIDHCPSGetLeaseInfo，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DHCPS_CSUDIDHCPSGetLeaseInfo_0001(void)
{
	char szIfName[16];
	CSUDIDHCPSLeaseInfo stLeaseInfo;
	int nCount = 1;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(CSUDI_NULL,&stLeaseInfo, &nCount),
		"步骤1失败");
	
	strncpy(szIfName,g_sDhcpsInfo.szInvalidNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,&stLeaseInfo, &nCount),
		"步骤2失败");

	strncpy(szIfName,"lo", sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,&stLeaseInfo, &nCount),
		"步骤3失败");

	strncpy(szIfName,g_sDhcpsInfo.szNic, sizeof(szIfName));
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,CSUDI_NULL, &nCount),
		"步骤4失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,&stLeaseInfo, CSUDI_NULL),
		"步骤5失败");

	nCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIDHCPS_ERROR_BAD_PARAMETER == CSUDIDHCPSGetLeaseInfo(szIfName,&stLeaseInfo, &nCount),
		"步骤6失败");
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSGetLeaseInfo
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIDHCPSGetLeaseInfo函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数符合设计要求
//@EXPECTATION:
//@REMARK:如果eth1为无线接口，则多台pc可以直接与其相连接
//@REMARK:如果eth1为有线接口，则多台pc可以通过交换机与其相连接
//@EXECUTIONFLOW:1、szIfName为正确的网卡名称(如"eth1"， IP:172.30.2.1)
//@EXECUTIONFLOW:2、设置psDHCPCfg参数正确，(如acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3、调用CSUDIDHCPSConfig函数，设置DHCPS参数
//@EXECUTIONFLOW:4、调用CSUDIDHCPSStart函数，启动DHCPS
//@EXECUTIONFLOW:5、和eth1连接的PC0 能够正常获取172.30.2.2~172.30.2.254段地址
//@EXECUTIONFLOW:6、和eth1连接的PC1 能够正常获取172.30.2.2~172.30.2.254段地址
//@EXECUTIONFLOW:7、调用CSUDIDHCPSGetLeaseInfo函数，获取的信息和PC0，PC1信息相同
//@EXECUTIONFLOW:8、断开pc2和eth1的连接
//@EXECUTIONFLOW:9、调用CSUDIDHCPSGetLeaseInfo函数，获取的信息和PC0信息相同
//@EXECUTIONFLOW:10、调用CSUDIDHCPSStop函数，停止DHCPS
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
		"步骤1失败");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPEnd);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
		"步骤3失败");
	bStart = CSUDI_TRUE;
	
    CSTCPrint("pc0上和eth1连接的网口，能否请求到相应IP段地址\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败：不能正确连接");

	CSTCPrint("pc1上和eth1连接的网口，能否请求到相应IP段地址\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：不能正确连接");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetLeaseInfo(szIfName,satLeaseInfo, &nCount),
		"步骤6失败");

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

	CSTCPrint("获取的信息是否和pc0 pc1信息一致\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：获取信息不正确");

	CSTCPrint("断开pc1和eth1的连接\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤8失败：没有断开连接");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetLeaseInfo(szIfName,satLeaseInfo, &nCount),
		"步骤9失败");

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

	CSTCPrint("获取的信息是否和pc0信息一致\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败：获取信息不正确");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), "步骤11失败");
	bStart = CSUDI_FALSE;

	CSTK_FATAL_POINT;
	
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),"步骤12失败");
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDHCPSGetLeaseInfo
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIDHCPSGetLeaseInfo函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数符合设计要求
//@EXPECTATION:
//@REMARK:如果eth1为无线接口，则多台pc可以直接与其相连接
//@REMARK:如果eth1为有线接口，则多台pc可以通过交换机与其相连接
//@EXECUTIONFLOW:1、szIfName为正确的网卡名称(如"eth1"， IP:172.30.2.1)
//@EXECUTIONFLOW:2、设置psDHCPCfg参数正确，(如acIPStart:172.30.2.2,acIPEnd:172.30.2.254,acMask:255.255.0.0 acGatway:172.30.2.1) 
//@EXECUTIONFLOW:3、调用CSUDIDHCPSConfig函数，设置DHCPS参数
//@EXECUTIONFLOW:4、调用CSUDIDHCPSStart函数，启动DHCPS
//@EXECUTIONFLOW:5、和eth1连接的PC0 能够正常获取172.30.2.2~172.30.2.254段地址
//@EXECUTIONFLOW:6、和eth1连接的PC1 能够正常获取172.30.2.2~172.30.2.254段地址
//@EXECUTIONFLOW:7、*pnCount=1，调用CSUDIDHCPSGetLeaseInfo函数，能够正确获取到一个pc的信息
//@EXECUTIONFLOW:8、调用CSUDIDHCPSStop函数，停止DHCPS
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
		"步骤1失败");

	strcpy(sDHCPCfg.acIPStart, g_sDhcpsInfo.sConfig[0].acIPStart);
	strcpy(sDHCPCfg.acIPEnd, g_sDhcpsInfo.sConfig[0].acIPEnd);
	strcpy(sDHCPCfg.acMask, g_sDhcpsInfo.sConfig[0].acMask);
	strcpy(sDHCPCfg.acGatway, g_sDhcpsInfo.sConfig[0].acGatway);
	strcpy(sDHCPCfg.acFirstDNS, g_sDhcpsInfo.sConfig[0].acFirstDNS);
	strcpy(sDHCPCfg.acSecondDNS, g_sDhcpsInfo.sConfig[0].acSecondDNS);
	sDHCPCfg.uLeaseTime = g_sDhcpsInfo.sConfig[0].uLeaseTime;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSConfig(szIfName, &sDHCPCfg), 
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSStart(szIfName), 
		"步骤3失败");
	bStart = CSUDI_TRUE;
	
    CSTCPrint("pc0上和eth1连接的网口，能否请求到相应IP段地址\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败：获取信息不正确");

	CSTCPrint("pc1上和eth1连接的网口，能否请求到相应IP段地址\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：获取信息不正确");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIDHCPSGetLeaseInfo(szIfName,satLeaseInfo, &nCount),
		"步骤6失败");

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

	CSTCPrint("是否只显示了pc0或者pc1的信息，且信息正确无误\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：获取信息不正确");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName), "步骤8失败");
	bStart = CSUDI_FALSE;

	CSTK_FATAL_POINT;
	
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDHCPSStop(szIfName),"步骤9失败");
	}
	
	return CSUDI_TRUE;
}



