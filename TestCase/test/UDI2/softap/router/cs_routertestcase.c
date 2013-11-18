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

static char g_szPcWanIp[16] = {0}; // 与wan口相连的pc的ip地址

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
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIRTSetInterfaces函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:调用返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、szWan为NULL，调用CSUDIRTSetInterfaces，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szWan为不存在网口，调用CSUDIRTSetInterfaces，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szWan为正常，szLan为NULL,调用CSUDIRTSetInterfaces，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、szWan为正常，szLan为不存在网口,调用CSUDIRTSetInterfaces，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetInterfaces(CSUDI_NULL,g_sRouterInfo.szLan),
		"步骤1失败");	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetInterfaces(g_sRouterInfo.szInvalidWan, g_sRouterInfo.szLan),
		"步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, CSUDI_NULL),
		"步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szInvalidLan),
		"步骤4失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTSetInterfaces&CSUDIRTGetInterfaces
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIRTGetInterfaces函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:调用返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、pcWan和pcLan为NULL，调用CSUDIRTGetInterfaces，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、nWanLen<=0和nLanLen<=0，nWanLen<=0 ,调用CSUDIRTGetInterfaces，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0002(void)
{
	char szWan[16];
	char szLan[16];
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetInterfaces(CSUDI_NULL, sizeof(szWan), CSUDI_NULL, sizeof(szLan)),
		"步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetInterfaces(szWan, 0, szLan, 0),
		"步骤2失败");
				
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTSetInterfaces&CSUDIRTGetInterfaces
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIRTSetInterfaces&CSUDIRTGetInterfaces函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数符合设计要求
//@EXPECTATION:函数能够正确执行
//@REMARK:
//@EXECUTIONFLOW:1、设置szWan,szLan, szWan 和 szlan相同
//@EXECUTIONFLOW:2、调用CSUDIRTSetInterfaces，设置接口参数失败
//@EXECUTIONFLOW:3、设置szWan,szLan, szWan 和 szlan不相同
//@EXECUTIONFLOW:4、调用CSUDIRTSetInterfaces，成功设置接口参数
//@EXECUTIONFLOW:5、调用CSUDIRTGetInterfaces，获取到接口参数，和设置的参数一致
//@EXECUTIONFLOW:6、设置szWan,szLan, szWan 和 szlan不相同，并且和上一次不一样
//@EXECUTIONFLOW:7、调用CSUDIRTSetInterfaces，成功设置接口参数
//@EXECUTIONFLOW:8、调用CSUDIRTGetInterfaces，获取到接口参数，和设置的参数一致
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0003(void)
{
	char szWan[16];
	char szLan[16];
	int nRet = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS != CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szWan),
		"步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetInterfaces(szWan, sizeof(szWan), szLan, sizeof(szLan)),
		"步骤3失败");
	nRet = strcmp(szWan, g_sRouterInfo.szWan);
	nRet |= strcmp(szLan, g_sRouterInfo.szLan);

	CSTK_ASSERT_TRUE_FATAL(0 == nRet, "步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szLan, g_sRouterInfo.szWan),
		"步骤5失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetInterfaces(szWan, sizeof(szWan), szLan, sizeof(szLan)), 
		"步骤6失败");
	nRet = strcmp(szWan, g_sRouterInfo.szLan);
	nRet |= strcmp(szLan, g_sRouterInfo.szWan);

	CSTK_ASSERT_TRUE_FATAL(0 == nRet, "步骤7失败");
	
	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}




//@CASEGROUP:CSUDIRTSetLanIp
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIRTSetLanIp函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:调用返回非CSUDI_SUCCESS
//@REMARK:非法ip如 172.30.1.256 
//@REMARK:		   172.30..2
//@REMARK:		   1723022
//@EXECUTIONFLOW:1、szLan为NULL，调用CSUDIRTSetLanIp，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、szLan为不存在网口，调用CSUDIRTSetLanIp，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、szLan为正常网口，szIp为NULL ,调用CSUDIRTSetLanIp，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、szLan为正常网口，szIp为非法IP ,调用CSUDIRTSetLanIp，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、szLan为正常网口，szIp为正常 ,szMask为NULL,调用CSUDIRTSetLanIp，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetLanIp_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(CSUDI_NULL, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szInvalidLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, CSUDI_NULL, g_sRouterInfo.szNetMask0), 
		"步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, "172.30.2.256", g_sRouterInfo.szNetMask0), 
		"步骤5失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, "172.30.2.", g_sRouterInfo.szNetMask0), 
		"步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, "1723022", g_sRouterInfo.szNetMask0),
		"步骤7失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, CSUDI_NULL), 
		"步骤8失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTSetLanIp
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIRTSetLanIp函数的执行效果
//@PRECONDITION:
//@INPUT:1、参数符合设计要求
//@EXPECTATION:成功执行CSUDIRTSetLanIp
//@REMARK:
//@EXECUTIONFLOW:1、设置正确函数参数
//@EXECUTIONFLOW:2、调用CSUDIRTSetLanIp函数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、PC0和lan口连接，设置pc0 IP地址和lan ip在同一地址段
//@EXECUTIONFLOW:4、PC0 ping lan口ip地址，能够ping通
//@EXECUTIONFLOW:5、设置正确的和上一次不同函数参数
//@EXECUTIONFLOW:6、调用CSUDIRTSetLanIp函数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、PC0和lan口连接，设置pc0 IP地址和lan ip在同一地址段
//@EXECUTIONFLOW:8、PC0 ping lan口ip地址，能够ping通
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetLanIp_0002(void)
{
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"步骤2失败");

	CSTCPrint("现在lan口的ip地址信息为 ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0和lan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
    CSTCPrint("2:设置pc0 mask地址和lan口 mask相同\n");
	CSTCPrint("3:在pc0上面ping lan口ip 地址?\n");
	CSTCPrint("pc0上面能否ping通lan口?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤3失败：获取信息不正确");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp1, g_sRouterInfo.szNetMask1), 
		"步骤4失败");

	CSTCPrint("现在lan口的ip地址信息为 ip=%s,mask=%s\n", g_sRouterInfo.szIp1, g_sRouterInfo.szNetMask1);
	CSTCPrint("1:PC0和lan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
    CSTCPrint("2:设置pc0 mask地址和lan口 mask相同\n");
	CSTCPrint("3:在pc0上面ping lan口ip 地址?\n");
	CSTCPrint("pc0上面能否ping通lan口?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：获取信息不正确");

	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIRTStartService函数的执行效果
//@PRECONDITION:ROUTER模块被正常初始化
//@INPUT:1、eService, pvStartParam参数不符合设计要求
//@EXPECTATION:所有调用返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、eService为-1，pvStartParam合法，调用CSUDIRTStartService，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、eService为EM_RT_SERVICETYPE_MAX，pvStartParam合法，调用CSUDIRTStartService，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、eService为EM_RT_SERVICETYPE_MAX+1，pvStartParam合法，调用CSUDIRTStartService，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、eService为EM_RT_SERVICETYPE_DNS_PROXY，pvStartParam为空，调用CSUDIRTStartService，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、设置(eth1 ip为192.168.10.68)
//@EXECUTIONFLOW:6、调用CSUDIRTSetInterfaces设置wan eth1,lan ra0
//@EXECUTIONFLOW:7、eService为EM_RT_SERVICETYPE_DNS_PROXY，pvStartParam中的DNS地址为空，调用CSUDIRTStartService，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0001(void)
{
	CSUDIRTDnsServers_S  sStartParam;
	char *pFirstDNS="202.10.10.88";
	char *pSecDNS="202.10.10.99";

	memcpy(sStartParam.acFirstDNS, pFirstDNS, CSUDI_APRT_IP_LEN);
	memcpy(sStartParam.acSecondDNS, pSecDNS, CSUDI_APRT_IP_LEN);
       
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStartService(-1, &sStartParam),
		"步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStartService(EM_RT_SERVICETYPE_MAX, &sStartParam),
		"步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStartService(EM_RT_SERVICETYPE_MAX+1, &sStartParam),
		"步骤3失败");
    
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, CSUDI_NULL),
		"步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(g_sRouterInfo.szWan, g_sRouterInfo.szWanIp), 
		"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤6失败");

    memset(sStartParam.acFirstDNS, '\0', CSUDI_APRT_IP_LEN);
    memset(sStartParam.acSecondDNS, '\0', CSUDI_APRT_IP_LEN);
	CSTK_ASSERT_TRUE_FATAL(
	CSUDI_FAILURE== CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, &sStartParam),
	"步骤7失败");
        
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIRTStopService函数的执行效果
//@PRECONDITION:ROUTER模块被正常初始化
//@INPUT:1、eService参数不符合设计要求
//@EXPECTATION:所有调用返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、eService为-1，调用CSUDIRTStopService，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、eService为EM_RT_SERVICETYPE_MAX，调用CSUDIRTStopService，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、eService为EM_RT_SERVICETYPE_MAX+1，调用CSUDIRTStopService，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStopService(-1),
		"步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStopService(EM_RT_SERVICETYPE_MAX),
		"步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTStopService(EM_RT_SERVICETYPE_MAX+1),
		"步骤3失败");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:测试EM_RT_SERVICETYPE_DNS_PROXY服务的启动与停止
//@PRECONDITION:
//@INPUT:1、eService为EM_RT_SERVICETYPE_DNS_PROXY
//@EXPECTATION:能够在配置的网口上面正确使用DNS_PROXY服务
//@REMARK:
//@EXECUTIONFLOW:1、设置(eth1 ip为192.168.10.68)
//@EXECUTIONFLOW:2、调用CSUDIRTSetInterfaces设置wan eth1,lan ra0
//@EXECUTIONFLOW:3、调用CSUDIRTSetLanIp设置lan 口IP地址为172.30.2.1
//@EXECUTIONFLOW:4、lan口连接的PC0 设置如下(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5、wan口连接的PC1 设置如下(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6、调用CSUDIRTStartService函数启动DNS(nameserver 192.168.10.90)代理服务器 
//@EXECUTIONFLOW:7、在pc1上面启动ethereal capture 192.168.10.90 网口数据包
//@EXECUTIONFLOW:8、在pc0 浏览器访问 www.baidu.com
//@EXECUTIONFLOW:9、查看pc1 ethereal 上面捕获的数据包中存在dns 请求www.baidu.com 的数据包
//@EXECUTIONFLOW:10、调用CSUDIRTStopService函数关闭DNS代理服务器
//@EXECUTIONFLOW:11、在pc0 浏览器访问 www.baidu.com (pc1 上面需要继续使用ethereal 捕获192.168.10.90网口数据)
//@EXECUTIONFLOW:12、查看pc1 ethereal 上面捕获的数据包中不存在dns 请求www.baidu.com 的数据包
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0003(void)
{
	CSUDI_BOOL bStart = CSUDI_FALSE;
	CSUDIRTDnsServers_S  sStartParam;

	memcpy(sStartParam.acFirstDNS, g_szPcWanIp, CSUDI_APRT_IP_LEN);
	memcpy(sStartParam.acSecondDNS, g_szPcWanIp, CSUDI_APRT_IP_LEN);

	CSTK_ASSERT_TRUE_FATAL(
		0 == set_nic_ip(g_sRouterInfo.szWan, g_sRouterInfo.szWanIp), 
		"步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"步骤3失败");

	CSTCPrint("现在lan口的ip地址信息为 ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0和lan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
    CSTCPrint("2:设置pc0 mask地址和lan口 mask相同\n");
	CSTCPrint("3:设置pc0 dns 为lan口ip地址\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤4失败：没有正确设置pc0 网络信息");

	CSTCPrint("现在wan口的ip地址信息为 ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1和wan口连接,设置pc1 ip地址和wan口ip地址在同一网段\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤5失败：没有正确设置pc1 网络信息\n");

/*begin modified by 904782 --直接在代码里面设置DNS地址*/
#if 0
	CSTCPrint("修改STB /etc/resolv.conf文件，添加(nameserver ip) ip地址为pc1 ip地址\n");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤6失败：修改STB /etc/resolv.conf文件失败\n");
#endif

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, &sStartParam),
		"步骤6失败");
	bStart = CSUDI_TRUE;
/*end modified by 904782 --直接在代码里面设置DNS地址*/

	CSTCPrint("1:在pc1上面启动ethereal捕获和wan口连接网口的数据包\n");
	CSTCPrint("pc1上是否启动ethereal捕获数据包\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤7失败：没有正确启动ethereal");

	CSTCPrint("1:在pc0 浏览器访问 www.baidu.com\n");
	CSTCPrint("pc0 浏览器是否访问 www.baidu.com\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤8失败：没有访问www.baidu.com");

	CSTCPrint("1:查看ethereal捕获的数据包\n");
	CSTCPrint("数据包中是否存在pc0请求dns的数据包?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤9失败：不存在dns数据包");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
		"步骤10失败");
	bStart = CSUDI_FALSE;

	CSTCPrint("1:在pc1上面启动ethereal捕获和wan口连接网口的数据包\n");
	CSTCPrint("pc1上是否启动ethereal捕获数据包\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤11失败：没有正确启动ethereal");

	CSTCPrint("1:在pc0 浏览器访问 www.baidu.com\n");
	CSTCPrint("pc0 浏览器是否访问 www.baidu.com\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤12失败：没有访问www.baidu.com");

	CSTCPrint("1:查看ethereal捕获的数据包\n");
	CSTCPrint("数据包中是否存在pc0请求dns的数据包?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤13失败：不存在dns数据包");

	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
			"步骤14失败");
	}	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:测试EM_RT_SERVICETYPE_IGMP_PROXY服务的启动与停止
//@PRECONDITION:STB启动路由转发STB:echo 1 > /proc/sys/net/ipv4/ip_forward
//@INPUT:1、eService为EM_RT_SERVICETYPE_IGMP_PROXY
//@EXPECTATION:能够在配置的网口上面正确使用IGMP_PROXY服务
//@REMARK:
//@EXECUTIONFLOW:1、设置(eth1 ip为192.168.10.68)
//@EXECUTIONFLOW:2、调用CSUDIRTSetInterfaces设置wan eth1,lan ra0
//@EXECUTIONFLOW:3、调用CSUDIRTSetLanIp设置lan 口IP地址为172.30.2.1
//@EXECUTIONFLOW:4、lan口连接的PC0 设置如下(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5、wan口连接的PC1 设置如下(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6、调用CSUDIRTExecuteIptableCmd函数执行iptables -t nat -A POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE命令
//@EXECUTIONFLOW:7、设置参数eService=EM_RT_SERVICETYPE_IGMP_PROXY
//@EXECUTIONFLOW:8、调用CSUDIRTStartService函数启动IGMP PROXY服务
//@EXECUTIONFLOW:9、PC1上面，使用WindSend工具播放码流
//@EXECUTIONFLOW:10、PC0上面，使用VLC工具播放码流
//@EXECUTIONFLOW:11、PC0上面，能够正常播放WindSend上面播放的码流
//@EXECUTIONFLOW:12、调用CSUDIRTStopService函数关闭IGMP PROXY服务器成功
//@EXECUTIONFLOW:13、PC0上面，不能播放WindSend上面播放的码流
//@EXECUTIONFLOW:14、调用CSUDIRTExecuteIptableCmd函数执行命令iptables -t nat -D POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE命令
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
		"步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"步骤3失败");

	CSTCPrint("现在lan口的ip地址信息为 ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0和lan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
    CSTCPrint("2:设置pc0 mask地址和lan口 mask相同\n");
	CSTCPrint("3:设置pc0 dns 为lan口ip地址\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤4失败：没有正确设置pc0 网络信息");

	CSTCPrint("现在wan口的ip地址信息为 ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1和wan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤5失败：没有正确设置pc1 网络信息");

	sprintf(szIptableCmd, "iptables -t nat -A POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_IGMP_PROXY, &sStartParam),
		"步骤7失败");
	bStart = CSUDI_TRUE;

	CSTCPrint("1:在pc1上面启动WindSend播放码流\n");
	CSTCPrint("pc1上是否启动WindSend播放码流\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤8失败：没有正确启动WindSend");

	CSTCPrint("1:在pc0 使用VLC工具播放码流\n");
	CSTCPrint("pc0 是否使用VLC工具播放码流\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤9失败：没有使用VLC工具播放码流");


	CSTCPrint("PC0上面，能否正常播放WindSend上面播放的码流?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤10失败：不能正常播放码流");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == 
		CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),"步骤11失败");
	bStart = CSUDI_FALSE;

	CSTCPrint("PC0上面，能否正常播放WindSend上面播放的码流?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤12失败：能正常播放码流");

	sprintf(szIptableCmd, "iptables -t nat -D POSTROUTING -s %s/16 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"步骤13失败");

	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
			"步骤14失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:测试EM_RT_SERVICETYPE_UPNP服务的启动与停止
//@PRECONDITION:1:STB启动路由转发STB:echo 1 > /proc/sys/net/ipv4/ip_forward
//@PRECONDITION:2:回环设备必须启动 ifconfig lo up
//@INPUT:1、eService为EM_RT_SERVICETYPE_UPNP
//@EXPECTATION:能够在配置的网口上面正确使用UPNP服务
//@REMARK:
//@EXECUTIONFLOW:1、设置(eth1 ip为192.168.10.68)
//@EXECUTIONFLOW:2、调用CSUDIRTSetInterfaces设置wan eth1,lan ra0
//@EXECUTIONFLOW:3、调用CSUDIRTSetLanIp设置lan 口IP地址为172.30.2.1
//@EXECUTIONFLOW:4、lan口连接的PC0 设置如下(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5、wan口连接的PC1 设置如下(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6、调用CSUDIRTExecuteIptableCmd函数执行iptables -t nat -A POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE命令
//@EXECUTIONFLOW:7、设置参数eService=EM_RT_SERVICETYPE_UPNP
//@EXECUTIONFLOW:8、调用CSUDIRTStartService函数启动UPNP服务
//@EXECUTIONFLOW:9、PC0上面，使用Xlight FTP服务器软件测试
//@EXECUTIONFLOW:10、PC0上面，能够正常看到UPNP服务的信息
//@EXECUTIONFLOW:11、调用CSUDIRTStopService函数关闭UPNP服务器成功
//@EXECUTIONFLOW:12、PC0上面，不能够正常看到UPNP服务的信息
//@EXECUTIONFLOW:13、调用CSUDIRTExecuteIptableCmd函数执行iptables -t nat -D POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE命令
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
		"步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"步骤3失败");

	CSTCPrint("现在lan口的ip地址信息为 ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0和lan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
    CSTCPrint("2:设置pc0 mask地址和lan口 mask相同\n");
	CSTCPrint("3:设置pc0 dns 为lan口ip地址\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤4失败：没有正确设置pc0 网络信息");

	CSTCPrint("现在wan口的ip地址信息为 ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1和wan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤5失败：没有正确设置pc1 网络信息");

	sprintf(szIptableCmd, "iptables -t nat -A POSTROUTING -s %s/16 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_UPNP, &sStartParam),
		"步骤7失败");
	bStart = CSUDI_TRUE;	

	CSTCPrint("1:在pc0 使用Xlight FTP工具测试\n");
	CSTCPrint("pc0 是否能够看到UPNP服务的信息\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤8失败：pc0 不能看到UPNP服务的信息");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
		"步骤9失败");
	bStart = CSUDI_FALSE;

	CSTCPrint("1:在pc0 使用Xlight FTP工具测试\n");
	CSTCPrint("pc0 是否能够看到UPNP服务的信息\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤10失败：pc0 能看到UPNP服务的信息");

	sprintf(szIptableCmd, "iptables -t nat -D POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"步骤11失败");
	
	CSTK_FATAL_POINT;
	if (bStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
			"步骤12失败");
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTStartService&CSUDIRTStopService
//@DESCRIPTION:测试EM_RT_SERVICETYPE_DNS_PROXY\EM_RT_SERVICETYPE_IGMP_PROXY\EM_RT_SERVICETYPE_UPNP服务同时启动
//@PRECONDITION:1:STB启动路由转发STB:echo 1 > /proc/sys/net/ipv4/ip_forward
//@PRECONDITION:2:回环设备必须启动 ifconfig lo up
//@INPUT:1、eService为EM_RT_SERVICETYPE_DNS_PROXY\EM_RT_SERVICETYPE_IGMP_PROXY\EM_RT_SERVICETYPE_UPNP
//@EXPECTATION:能够在配置的网口上面正确使用所有的服务
//@REMARK:
//@EXECUTIONFLOW:1、设置(eth1 ip为192.168.10.68)
//@EXECUTIONFLOW:2、调用CSUDIRTSetInterfaces设置wan eth1,lan ra0
//@EXECUTIONFLOW:3、调用CSUDIRTSetLanIp设置lan 口IP地址为172.30.2.1
//@EXECUTIONFLOW:4、lan口连接的PC0 设置如下(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5、wan口连接的PC1 设置如下(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6、调用CSUDIRTExecuteIptableCmd函数执行iptables -t nat -A POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE命令
//@EXECUTIONFLOW:7、调用CSUDIRTStartService函数启动EM_RT_SERVICETYPE_DNS_PROXY(nameserver 192.168.10.90)服务
//@EXECUTIONFLOW:8、调用CSUDIRTStartService函数启动EM_RT_SERVICETYPE_IGMP_PROXY服务
//@EXECUTIONFLOW:9、调用CSUDIRTStartService函数启动EM_RT_SERVICETYPE_UPNP服务
//@EXECUTIONFLOW:10、在pc1上面启动ethereal capture 192.168.10.90 网口数据包
//@EXECUTIONFLOW:11、在pc0 浏览器访问 www.baidu.com
//@EXECUTIONFLOW:12、查看pc1 ethereal 上面捕获的数据包中存在dns 请求www.baidu.com 的数据包
//@EXECUTIONFLOW:13、PC1上面使用WindSend播放码流，PC2使用VLC工具播放码流
//@EXECUTIONFLOW:14、PC0在vlc上面能够正常播放码流
//@EXECUTIONFLOW:15、PC0上面，使用Xlight FTP服务器软件测试,能够看到UPNP信息
//@EXECUTIONFLOW:16、调用CSUDIRTStopService函数关闭DNS服务成功
//@EXECUTIONFLOW:17、调用CSUDIRTStopService函数关闭IGMP Proxy服务成功
//@EXECUTIONFLOW:18、调用CSUDIRTStopService函数关闭UPNP服务成功
//@EXECUTIONFLOW:19、调用CSUDIRTExecuteIptableCmd函数执行iptables -t nat -D POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE命令
//@EXECUTIONFLOW:20、在pc1上面启动ethereal capture 192.168.10.90 网口数据包
//@EXECUTIONFLOW:21、在pc0 浏览器访问 www.baidu.com
//@EXECUTIONFLOW:22、查看pc1 ethereal 上面捕获的数据包中不存在dns 请求www.baidu.com 的数据包
//@EXECUTIONFLOW:23、PC0在vlc上面不能够正常播放码流
//@EXECUTIONFLOW:24、PC0上面，使用Xlight FTP服务器软件测试,不能够看到UPNP信息
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
		"步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"步骤3失败");

	CSTCPrint("现在lan口的ip地址信息为 ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0和lan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
    CSTCPrint("2:设置pc0 mask地址和lan口 mask相同\n");
	CSTCPrint("3:设置pc0 dns 为lan口ip地址\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤4失败：没有正确设置pc0 网络信息");

	CSTCPrint("现在wan口的ip地址信息为 ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1和wan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤5失败：没有正确设置pc1 网络信息");

	
	sprintf(szIptableCmd, "iptables -t nat -A POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, &sStartParam),
		"步骤7失败");
        bDNSProxyStart = CSUDI_TRUE;
        
        memset(sStartParam.acFirstDNS, '\0', CSUDI_APRT_IP_LEN);
        memset(sStartParam.acSecondDNS, '\0', CSUDI_APRT_IP_LEN);
        
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_IGMP_PROXY, &bDNSProxyStart),
		"步骤8失败");
	bIGMPProxyStart = CSUDI_TRUE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_UPNP, &bDNSProxyStart),
		"步骤9失败");
	bUPNPStart = CSUDI_TRUE;

	CSTCPrint("1:在pc1上面启动ethereal捕获和wan口连接网口的数据包\n");
	CSTCPrint("pc1上是否启动ethereal捕获数据包\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤10失败：没有正确启动ethereal");
	CSTCPrint("1:在pc0 浏览器访问 www.baidu.com\n");
	CSTCPrint("pc0 浏览器是否访问 www.baidu.com\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤11失败：没有访问www.baidu.com");
	CSTCPrint("1:查看ethereal捕获的数据包\n");
	CSTCPrint("数据包中是否存在pc0请求dns的数据包?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤12失败：不存在dns数据包");
	
	CSTCPrint("1:在pc1上面启动WindSend播放码流\n");
	CSTCPrint("pc1上是否启动WindSend播放码流\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤13失败：没有正确启动WindSend");
	CSTCPrint("1:在pc0 使用VLC工具播放码流\n");
	CSTCPrint("pc0 是否使用VLC工具播放码流\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤14失败：没有使用VLC工具播放码流");
	CSTCPrint("PC0上面，能否正常播放WindSend上面播放的码流?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤15失败：不能正常播放码流");

	CSTCPrint("1:在pc0 使用Xlight FTP工具测试\n");
	CSTCPrint("pc0 是否能够看到UPNP服务的信息\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤16失败：pc0 不能看到UPNP服务的信息");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
		"步骤17失败");
	bDNSProxyStart = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
		"步骤18失败");
	bIGMPProxyStart = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
		"步骤19失败");
	bUPNPStart = CSUDI_FALSE;

	CSTCPrint("1:在pc1上面启动ethereal捕获和wan口连接网口的数据包\n");
	CSTCPrint("pc1上是否启动ethereal捕获数据包\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤20失败：没有正确启动ethereal");
	CSTCPrint("1:在pc0 浏览器访问 www.baidu.com\n");
	CSTCPrint("pc0 浏览器是否访问 www.baidu.com\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤21失败：没有访问www.baidu.com");
	CSTCPrint("1:查看ethereal捕获的数据包\n");
	CSTCPrint("数据包中是否存在pc0请求dns的数据包?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤22失败：不存在dns数据包");
	CSTCPrint("PC0上面，能否正常播放WindSend上面播放的码流?\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤23失败：能正常播放码流");
	CSTCPrint("1:在pc0 使用Xlight FTP工具测试\n");
	CSTCPrint("pc0 是否能够看到UPNP服务的信息\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_FALSE == CSTKWaitYes(),
		"步骤24失败：pc0 能看到UPNP服务的信息");

	sprintf(szIptableCmd, "iptables -t nat -D POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"步骤25失败");

	CSTK_FATAL_POINT;
	if (bDNSProxyStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
			"步骤26失败");
	}
	if (bIGMPProxyStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
			"步骤27失败");		
	}
	if (bUPNPStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
			"步骤28失败");		
	}

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIRTGetServiceStatus
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIRTGetServiceStatus函数的执行效果
//@PRECONDITION:ROUTER模块被正常初始化
//@INPUT:1、eService为非法参数
//@INPUT:2、pbIsRunning为非法参数
//@EXPECTATION:所有调用返回非CSNET_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、eService为-1， 调用CSUDIRTGetServiceStatus函数调用，返回非CSNET_SUCCESS
//@EXECUTIONFLOW:2、eService为EM_RT_SERVICETYPE_MAX， 调用CSUDIRTGetServiceStatus函数调用，返回非CSNET_SUCCESS
//@EXECUTIONFLOW:3、eService为EM_RT_SERVICETYPE_MAX+1， 调用CSUDIRTGetServiceStatus函数调用，返回非CSNET_SUCCESS
//@EXECUTIONFLOW:4、eService为EM_RT_SERVICETYPE_IGMP_PROXY,pbIsRunning为NULL,调用CSUDIRTGetServiceStatus函数调用，返回非CSNET_SUCCESS
CSUDI_BOOL CSTC_ROUTER_CSUDIRTGetServiceStatus_0001(void)
{
	CSUDI_BOOL bIsRunning = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetServiceStatus(-1, &bIsRunning),
		"步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_MAX, &bIsRunning),
		"步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_MAX+1, &bIsRunning),
		"步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_IGMP_PROXY, CSUDI_NULL),
		"步骤4失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTGetServiceStatus
//@DESCRIPTION:测试参数或应用场景在符合接口设计要求，CSUDIRTGetServiceStatus函数的执行效果
//@PRECONDITION:ROUTER模块被正常初始化
//@PRECONDITION:STB启动路由转发STB:echo 1 > /proc/sys/net/ipv4/ip_forward
//@INPUT:1、eService为EM_RT_SERVICETYPE_DNS_PROXY\EM_RT_SERVICETYPE_IGMP_PROXY\EM_RT_SERVICETYPE_UPNP
//@INPUT:2、pbIsRunning为非空参数
//@EXPECTATION:CSNETGetServiceStatus函数能够正确的获取各个服务状态
//@REMARK:
//@EXECUTIONFLOW:1、设置(eth1 ip为192.168.10.68)
//@EXECUTIONFLOW:2、调用CSUDIRTSetInterfaces设置wan eth1,lan ra0
//@EXECUTIONFLOW:3、调用CSUDIRTSetLanIp设置lan 口IP地址为172.30.2.1
//@EXECUTIONFLOW:4、lan口连接的PC0 设置如下(ip 172.30.2.2 mask 255.255.0.0 dns 172.30.2.1)
//@EXECUTIONFLOW:5、wan口连接的PC1 设置如下(ip 192.168.10.90 mask 255.255.255.0)
//@EXECUTIONFLOW:6、调用CSUDIRTExecuteIptableCmd函数执行iptables -t nat -A POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE命令
//@EXECUTIONFLOW:7、调用CSUDIRTStartService函数启动EM_RT_SERVICETYPE_DNS_PROXY((nameserver 192.168.10.90))服务
//@EXECUTIONFLOW:8、调用CSUDIRTStartService函数启动EM_RT_SERVICETYPE_IGMP_PROXY服务
//@EXECUTIONFLOW:9、调用CSUDIRTStartService函数启动EM_RT_SERVICETYPE_UPNP服务
//@EXECUTIONFLOW:10、调用CSUDIRTGetServiceStatus函数获取EM_NET_DNS_PROXY服务状态为TRUE
//@EXECUTIONFLOW:11、调用CSUDIRTGetServiceStatus函数获取EM_NET_IGMP_PROXY服务状态为TRUE
//@EXECUTIONFLOW:12、调用CSUDIRTGetServiceStatus函数获取EM_RT_SERVICETYPE_UPNP服务状态为TRUE
//@EXECUTIONFLOW:13、调用CSUDIRTStopService函数停止EM_RT_SERVICETYPE_DNS_PROXY服务
//@EXECUTIONFLOW:14、调用CSUDIRTStopService函数停止EM_RT_SERVICETYPE_IGMP_PROXY服务
//@EXECUTIONFLOW:15、调用CSUDIRTStopService函数停止EM_RT_SERVICETYPE_UPNP服务
//@EXECUTIONFLOW:16、调用CSUDIRTExecuteIptableCmd函数执行iptables -t nat -D POSTROUTING -s 172.30.2.0/24 -o eth1 -j MASQUERADE命令
//@EXECUTIONFLOW:17、调用CSUDIRTGetServiceStatus函数获取EM_RT_SERVICETYPE_DNS_PROXY服务状态为FALSE
//@EXECUTIONFLOW:18、调用CSUDIRTGetServiceStatus函数获取EM_RT_SERVICETYPE_IGMP_PROXY服务状态为FALSE
//@EXECUTIONFLOW:19、调用CSUDIRTGetServiceStatus函数获取EM_RT_SERVICETYPE_UPNP服务状态为FALSE
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
		"步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetInterfaces(g_sRouterInfo.szWan, g_sRouterInfo.szLan),
		"步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTSetLanIp(g_sRouterInfo.szLan, g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0), 
		"步骤3失败");

	CSTCPrint("现在lan口的ip地址信息为 ip=%s,mask=%s\n", g_sRouterInfo.szIp0, g_sRouterInfo.szNetMask0);
	CSTCPrint("1:PC0和lan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
    CSTCPrint("2:设置pc0 mask地址和lan口 mask相同\n");
	CSTCPrint("3:设置pc0 dns 为lan口ip地址\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤4失败：没有正确设置pc0 网络信息");

	CSTCPrint("现在wan口的ip地址信息为 ip=%s\n", g_sRouterInfo.szWanIp);
	CSTCPrint("1:PC1和wan口连接,设置pc0 ip地址和lan口ip地址在同一网段\n");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE == CSTKWaitYes(),
		"步骤5失败：没有正确设置pc1 网络信息");
	
	sprintf(szIptableCmd, "iptables -t nat -A POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_DNS_PROXY, &sStartParam),
		"步骤7失败");
	bDNSProxyStart = CSUDI_TRUE;

        memset(sStartParam.acFirstDNS, '\0', CSUDI_APRT_IP_LEN);
        memset(sStartParam.acSecondDNS, '\0', CSUDI_APRT_IP_LEN);

        CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_IGMP_PROXY, &sStartParam),
		"步骤8失败");
	bIGMPProxyStart = CSUDI_TRUE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStartService(EM_RT_SERVICETYPE_UPNP, &sStartParam),
		"步骤9失败");
	bUPNPStart = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_DNS_PROXY, &bIsRunning),
		"步骤10失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == bIsRunning, "步骤10失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_IGMP_PROXY, &bIsRunning),
		"步骤11失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == bIsRunning, "步骤11失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_UPNP, &bIsRunning),
		"步骤12失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == bIsRunning, "步骤12失败");

	
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
		"步骤13失败");
	bDNSProxyStart = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
		"步骤14失败");
	bIGMPProxyStart = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
		"步骤15失败");
	bUPNPStart = CSUDI_FALSE;

	sprintf(szIptableCmd, "iptables -t nat -D POSTROUTING -s %s/24 -o %s -j MASQUERADE", g_sRouterInfo.szIp0, g_sRouterInfo.szWan);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTExecuteIptableCmd(szIptableCmd), 
		"步骤16失败");


	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_DNS_PROXY, &bIsRunning),
		"步骤17失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bIsRunning, "步骤17失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_IGMP_PROXY, &bIsRunning),
		"步骤18失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bIsRunning, "步骤18失败");
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_SUCCESS == CSUDIRTGetServiceStatus(EM_RT_SERVICETYPE_UPNP, &bIsRunning),
		"步骤19失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bIsRunning, "步骤19失败");


	CSTK_FATAL_POINT;
	if (bDNSProxyStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_DNS_PROXY),
			"步骤20失败");
	}
	if (bIGMPProxyStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_IGMP_PROXY),
			"步骤21失败");		
	}
	if (bUPNPStart)
	{
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIRTStopService(EM_RT_SERVICETYPE_UPNP),
			"步骤22失败");		
	}

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIRTExecuteIptableCmd
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIRTExecuteIptableCmd函数的执行效果
//@PRECONDITION:ROUTER模块被正常初始化
//@INPUT:1、pszcmd参数不正确
//@EXPECTATION:所有调用返回非CSNET_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、pszcmd为空,调用CSUDIRTExecuteIptableCmd函数，返回非CSNET_SUCCESS
CSUDI_BOOL  CSTC_ROUTER_CSUDIRTExecuteIptableCmd_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
		CSUDIRT_ERROR_BAD_PARAMETER == CSUDIRTExecuteIptableCmd(CSUDI_NULL),
		"步骤1失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


