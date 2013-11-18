#include "setup.h"
#include "cs_ipcfg.h"
#include "cs_tcpip.h"
#include "cs_testkit.h"
#include "udi2_ipcfg.h"
#include "cs_lan_plus.h"
#include "cs_lan.h"

//#ifndef PLATFORM_X86
#define TCPIP_STACK_DATA_TASK_PRIORITY	(9 * 17)
#define TCPIP_STACK_INPUT_TASK_PRIORITY	(7 * 17)
#define TCPIP_STACK_TIMER_TASK_PRIORITY	(4 * 17)

//如果测试protocol模块请把PROTOCOLTEST定义为正整数  yuanjun 2008-12-30 modification
#define PROTOCOL 1
#define TCPIP_STACK_DATA_TASK_SIZE 		(32*1024)
#define TCPIP_STACK_INPUT_TASK_SIZE    	(32* 1024)
#define TCPIP_STACK_TIMER_TASK_SIZE		(32* 1024)

#define TCPIP_STACK_FLAGS					0x10		/*支持DHCP；*/

#define IPSTACK_BUFFERSIZE  				(1024*1024)

extern int tvcombo_main( int argc, char **argv );
#if PROTOCOL
#ifndef PLATFORM_X86
static char g_cIPStackBuffer[IPSTACK_BUFFERSIZE];

static LAN_CONFIG_INFO sLanConfigInfo = 
{
	0xa3e00300,  /*0xa3e00000*/
	4,
	64,
	7,
	0,
	TCPIP_STACK_DATA_TASK_SIZE,
	TCPIP_STACK_DATA_TASK_PRIORITY
};

/*网络初始化*/
void SetupProtocol(void)
{

	int nNicCount;
	unsigned char MacAddr[6] ={0x00,0x0E,0x09,0xef,0xee,0xbf};
	int i;
	int nRe;
	
	char acEthName[8] = {0};
	char acIP[20] = {0};
	char acMask[20] = {0};
	char acGateWay[20] = {0};
	char acDns1[20] = {0};
	char acDns2[20] = {0};
	char acMac[16] = {0};

	CSTTPrint("=============SetupProtocol===========\n");
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("PROTOCOL","ETHNAME",acEthName,sizeof(acEthName)))
	{
		CSTTPrint("eth name is %s\n",acEthName);
	}	
	
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("PROTOCOL","ETHMAC",acMac,sizeof(acMac)))
	{
		CSTTPrint("eth mac is %s\n",acMac);
	}
	
	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("PROTOCOL","ETHIP",acIP,sizeof(acIP)))
	{
		CSTTPrint("eth ip is %s\n",acIP);
	}

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("PROTOCOL","ETHMASK",acMask,sizeof(acMask)))
	{
		CSTTPrint("eth Mask is %s\n",acMask);
	}

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("PROTOCOL","ETHGATEWAY",acGateWay,sizeof(acGateWay)))
	{
		CSTTPrint("eth GateWay is %s\n",acGateWay);
	}

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("PROTOCOL","ETHDNS1",acDns1,sizeof(acDns1)))
	{
		CSTTPrint("eth dns1 is %s\n",acDns1);
	}

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("PROTOCOL","ETHDNS2",acDns2,sizeof(acDns2)))
	{
		CSTTPrint("eth dns2 is %s\n",acDns2);
	}
	/* 初始化网卡， 设置mac地址 */
	nRe = CSLanInitialize(&sLanConfigInfo, 1);
	CSDebug("dhcp",DEBUG_LEVEL, "******************CSLanInitialize is %d %d \n",nRe,__LINE__);

	CSASSERT(nRe == 0);

	nNicCount = CSLanGetCount();
	CSDebug("dhcp",DEBUG_LEVEL,"\n  nNicCount  =%d  %d \n",nNicCount,__LINE__);
	CSASSERT(nNicCount > 0);
	
	
	for (i=0; i<nNicCount; i++)
	{
		CSLanSetMacAddress( i, MacAddr, 6);
	}
	CSLanGetMacAddress(0, MacAddr);
	CSDebug("dhcp",DEBUG_LEVEL, "\n MAC Address is : %02x  %02x  %02x  %02x  %02x  %02x \n",MacAddr[0],MacAddr[1],MacAddr[2],MacAddr[3],MacAddr[4],MacAddr[5]);
#if 0
	for (i=0; i<nNicCount; i++)	
	{
		if (CSLanGetMacAddress(i, (BYTE*)&cTemp) == 0)
		{
			continue;
		}
	}
#endif 
	/* 初始协议栈， 设置ip 地址，子网掩码，网关，DNS服务器 */	
	nRe =  CSIPCFG_TCPSetup(g_cIPStackBuffer, IPSTACK_BUFFERSIZE,
		TCPIP_STACK_INPUT_TASK_PRIORITY, TCPIP_STACK_INPUT_TASK_SIZE,
		TCPIP_STACK_TIMER_TASK_PRIORITY, TCPIP_STACK_TIMER_TASK_SIZE,
		TCPIP_STACK_FLAGS);

	CSASSERT(nRe == 0);

	nRe =  CSUDIIPCFGSetIpInfo(acEthName,acIP,acMask,acGateWay);	
	CSDebug("dhcp",DEBUG_LEVEL,"\n  nRe  =%d  acEthName=%s  acIP=%s  acMask=%s  acGateWay=%s  \n",nRe,acEthName,acIP,acMask,acGateWay);
	CSASSERT(nRe == 0);
		
	CSUDIIPCFGSetDNSServer(0, acDns1, 20);
	CSUDIIPCFGSetDNSServer(1, acDns2, 20);

}
#else
void SetupProtocol(void)
{
	return;
}
#endif
#else
void SetupProtocol(void)
{
	return;
}
#endif
