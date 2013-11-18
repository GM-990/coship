/*
 *@version 1.0.0 2012/07/28 ��ɳ�ʼ�汾
 */
/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.������������������CSTC_��������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_dhcptestcase.h"
#include "udi2_ipcfg.h"
#include "cs_testkit.h"
#include "udi2_os.h"
#include "udi2_socket.h"
#include "cs_dhcpc.h"
#include "udiplus_os.h"
#include "udi2_os.h"

static char                g_szDNSIp[16] = {0};
static CSUDIWLanConfig_S   s_asLanCfg[10];//�����ļ���������������
static CSUDIWLanConfig_S   s_asWLanCfg[10];//�����ļ���������������
static int                 s_nActLanCount = 0;//�����ļ���������������
static int                 s_nActWLanCount = 0;//�����ļ���������������

static int	               g_nDhcpSocket=-1;
static dhcpMessage         g_sDhcpMsgSend;
static dhcpMessage         g_sDhcpMsgRecv;
static unsigned char       aucMacAddress[ETH_ALEN];
static dhcpOptions         g_sDhcpOptions;
static unsigned char       g_DhcpserverIp[4] = {0,0,0,0};
static const unsigned char g_aucMagicCookie[4] = { 99, 130, 83, 99 };
static unsigned short  	   g_wDhcpMsgSize = 0;
static unsigned  int  	   g_unleaseTime   = DEFAULT_LEASETIME;
static dhcpOptions         s_dhcpoptions;


int peekfd(int s,unsigned long tv_usec)
{
	CSUDIFdSet_S sFs;
	CSUDITimeval_S sTmValue;
    int nErr=0;
	int nErrlen=0;

	CSUDI_FD_ZERO(&sFs);
	CSUDI_FD_SET(s,&sFs);
	sTmValue.tv_sec=tv_usec/1000000;
	sTmValue.tv_usec=tv_usec%1000000;
	if (s >=0)
 	{
		if ( CSUDISOCKSelect(s+1,&sFs,CSUDI_NULL,CSUDI_NULL,&sTmValue) <=0 )
		{
			CSTCPrint("CSUDISOCKSelectʧ�� \n");	
			return -1;
		}
		if ( CSUDI_FD_ISSET(s,&sFs) ) 
		{
			CSUDISOCKGetSockOpt(s, CSUDI_SOL_SOCKET, CSUDI_SO_ERROR, (void*)&nErr, (unsigned int*)&nErrlen);
			if (nErr != 0)
			{	
				CSTCPrint("CSUDISOCKGetSockOptʧ��, ����ֵΪ: %d\n", nErr);	
				return -1;
			}
			return 0;
		}
		else
		{
			CSTCPrint("CSUDI_FD_ISSETʧ�� \n");	
		}
 	
		return -1;
	}
	else
	{
		CSTCPrint("CSUDI_FD_SETʧ�� \n");	
	}
	return -1;
}

void buildDhcpDiscover(unsigned int xid)
{	
	register unsigned char *pucPoint = (&g_sDhcpMsgSend)->options + 4;
	
	(&g_sDhcpMsgSend)->op	=	DHCP_BOOTREQUEST;/*DHCP��������,requestΪ1,replyΪ2*/
	(&g_sDhcpMsgSend)->htype=	ARPHRD_ETHER;/*Ӳ������, ethernetΪ1*/
	(&g_sDhcpMsgSend)->hlen	=	ETH_ALEN;/*Ӳ����ַ����Ϊ6*/
	(&g_sDhcpMsgSend)->xid 	= 	xid;/*����ID,������*/
	
	(&g_sDhcpMsgSend)->secs	=       CSUDISOCKHtons(3);

	(&g_sDhcpMsgSend)->flags=	CSUDISOCKHtons(BROADCAST_FLAG);

	
	memcpy((&g_sDhcpMsgSend)->chaddr,aucMacAddress,ETH_ALEN);
	  
	/*&magic_cookie  -->magic_cookie    modified by zch,because the defination modified */
	memcpy((&g_sDhcpMsgSend)->options,g_aucMagicCookie,4);
 
	*pucPoint++ = dhcpMessageType;
	*pucPoint++ = 1;
	*pucPoint++ = DHCP_DISCOVER;
	
	*pucPoint++ = dhcpMaxMsgSize;
	*pucPoint++ = 2;
	memcpy(pucPoint,&g_wDhcpMsgSize,2);
	pucPoint += 2;

	*pucPoint++ = dhcpIPaddrLeaseTime;
	*pucPoint++ = 4;
	memcpy(pucPoint,&g_unleaseTime,4);
	pucPoint += 4;

	*pucPoint++ = dhcpParamRequest;

	*pucPoint++ = 14;

	*pucPoint++ = subnetMask;
	*pucPoint++ = routersOnSubnet;
	*pucPoint++ = dns;
	*pucPoint++ = hostName;
	*pucPoint++ = domainName;
	*pucPoint++ = rootPath;
	*pucPoint++ = defaultIPTTL;
	*pucPoint++ = broadcastAddr;
	*pucPoint++ = performMaskDiscovery;
	*pucPoint++ = performRouterDiscovery;
	*pucPoint++ = staticRoute;
	*pucPoint++ = nisDomainName;
	*pucPoint++ = nisServers;
	*pucPoint++ = ntpServers;


	*pucPoint = endOption;

}

void buildDhcpRequest(unsigned xid)
{
	register unsigned char *pucPoint = (&g_sDhcpMsgSend)->options + 4;
	
	(&g_sDhcpMsgSend)->op	=	DHCP_BOOTREQUEST;
	(&g_sDhcpMsgSend)->htype=	ARPHRD_ETHER;
	(&g_sDhcpMsgSend)->hlen	=	ETH_ALEN;
	(&g_sDhcpMsgSend)->xid	=	xid;
	(&g_sDhcpMsgSend)->secs	=	CSUDISOCKHtons(3);

	(&g_sDhcpMsgSend)->flags=	CSUDISOCKHtons(BROADCAST_FLAG);

	memcpy((&g_sDhcpMsgSend)->chaddr,aucMacAddress,ETH_ALEN);

   
	memcpy((&g_sDhcpMsgSend)->options,g_aucMagicCookie,4);
	*pucPoint++ = dhcpMessageType;
	*pucPoint++ = 1;
	*pucPoint++ = DHCP_REQUEST;
	
	*pucPoint++ = dhcpMaxMsgSize;
	*pucPoint++ = 2;
	memcpy(pucPoint,&g_wDhcpMsgSize,2);
	pucPoint += 2;

	*pucPoint++ = dhcpServerIdentifier;
	*pucPoint++ = 4;

	if (g_sDhcpOptions.len[dhcpServerIdentifier]!=0)
	{
		memcpy(pucPoint,g_sDhcpOptions.val[dhcpServerIdentifier],4);
	}

	pucPoint += 4;

	*pucPoint++ = dhcpRequestedIPaddr;
	*pucPoint++ = 4;
	memcpy(pucPoint,&((&g_sDhcpMsgRecv)->yiaddr),4);
	pucPoint += 4;

	*pucPoint++ = dhcpParamRequest;



	*pucPoint++ = 14;

	*pucPoint++ = subnetMask;
	*pucPoint++ = routersOnSubnet;
	*pucPoint++ = dns;
	*pucPoint++ = hostName;
	*pucPoint++ = domainName;
	*pucPoint++ = rootPath;
	*pucPoint++ = defaultIPTTL;
	*pucPoint++ = broadcastAddr;
	*pucPoint++ = performMaskDiscovery;
	*pucPoint++ = performRouterDiscovery;
	*pucPoint++ = staticRoute;
	*pucPoint++ = nisDomainName;
	*pucPoint++ = nisServers;
	*pucPoint++ = ntpServers;
      
	*pucPoint = endOption;
	
}

/*****************************************************************************/
#define IFNAMSIZ 16

int parseDhcpMsgRecv()  /* this routine parses dhcp message received */
{
    register unsigned char	 *pucPoint = (&g_sDhcpMsgRecv)->options+4;
    unsigned char *pucEnd = (&g_sDhcpMsgRecv)->options+sizeof((&g_sDhcpMsgRecv)->options);
    memset((unsigned char *)&s_dhcpoptions,0,sizeof(dhcpOptions));

    while ( pucPoint < pucEnd )
    {
	//CSTCPrint("[parseDhcpMsgRecv ]:*pucPoint:%d, 0x%x\n", *pucPoint, pucPoint[1]);
    	switch ( *pucPoint )
    	{
    	case endOption: 
    				goto swend;
    	case padOption: 
    				pucPoint++; 
    				break;
    	default:
      			/* Handle a bug in the redhat 6.2 bootp server
       			* which causes it to incorrectly pad the end option */
      			if ( ((pucPoint - (&g_sDhcpMsgRecv)->options)&1) && (pucPoint[1] == endOption) )
        			goto swend;
      			if ( pucPoint[1] )
        		{
          			if ( s_dhcpoptions.len[*pucPoint] == pucPoint[1] )
            		{
            			memcpy(s_dhcpoptions.val[*pucPoint],pucPoint+2,pucPoint[1]);
          			}
          			else
            		{
    	  				s_dhcpoptions.len[*pucPoint] = pucPoint[1];
    					s_dhcpoptions.num++;
      	  				memcpy(s_dhcpoptions.val[*pucPoint],pucPoint+2,pucPoint[1]);
            		}
        		}
      			pucPoint+=pucPoint[1]+2;
    
    	}
    }
	
    swend:
    if ( ! (&g_sDhcpMsgRecv)->yiaddr ) 
    {
	    (&g_sDhcpMsgRecv)->yiaddr=(&g_sDhcpMsgSend)->ciaddr;
    }

    if (s_dhcpoptions.len[subnetMask]==0)
    {
	    ((unsigned char *)s_dhcpoptions.val[subnetMask])[0] = 255;
    
    	if ( ((unsigned char *)&(&g_sDhcpMsgRecv)->yiaddr)[0] < 128 )
    	{
    	    ((unsigned char *)s_dhcpoptions.val[subnetMask])[1] = 0; /* class A */
    	}
    	else
        {
      	    ((unsigned char *)s_dhcpoptions.val[subnetMask])[1] = 255;
    	    if ( ((unsigned char *)&(&g_sDhcpMsgRecv)->yiaddr)[0] < 192 )
    	    {
    		    ((unsigned char *)s_dhcpoptions.val[subnetMask])[2] = 0;/* class B */
    	    } 
    	    else
    	    {
    		    ((unsigned char *)s_dhcpoptions.val[subnetMask])[2] = 255;/* class C */
    	    }
         }

    	 ((unsigned char *)s_dhcpoptions.val[subnetMask])[3] = 0;
    	 s_dhcpoptions.len[subnetMask] = 4;
    	 s_dhcpoptions.num++;
    }

    if (s_dhcpoptions.len[broadcastAddr]==0)/* did not get broadcastAddr */		
    {	
        int br = (&g_sDhcpMsgRecv)->yiaddr | ~*((int *)s_dhcpoptions.val[subnetMask]);
    	memcpy(s_dhcpoptions.val[broadcastAddr],&br,4);
    	s_dhcpoptions.len[broadcastAddr] = 4;
    	s_dhcpoptions.num++;
    }

    if (s_dhcpoptions.len[routersOnSubnet] ==0)
    { 		
        if ( (&g_sDhcpMsgRecv)->giaddr )
        {
	        memcpy(s_dhcpoptions.val[routersOnSubnet],&(&g_sDhcpMsgRecv)->giaddr,4);
        }
        else
        {
            memcpy(s_dhcpoptions.val[routersOnSubnet],s_dhcpoptions.val[dhcpServerIdentifier],4);
        }
        s_dhcpoptions.len[routersOnSubnet] = 4;
        s_dhcpoptions.num++;	 
    }
    if ( (*(unsigned char *)s_dhcpoptions.val[dhcpMessageType]==DHCPOFFER)||(*(unsigned char *)s_dhcpoptions.val[dhcpMessageType]==DHCPACK))
    {			
        if (((g_DhcpserverIp[0]|g_DhcpserverIp[1]|g_DhcpserverIp[2]|g_DhcpserverIp[3])!=0)&&
           (memcmp( s_dhcpoptions.val[dhcpServerIdentifier],g_DhcpserverIp,4)!=0))
        {	
            return 0;
        }
		
        memcpy((unsigned char *)&g_sDhcpOptions,(unsigned char *)&s_dhcpoptions,sizeof(dhcpOptions));
	
        if (g_sDhcpOptions.len[dhcpServerIdentifier]!=0)
        {
            memcpy(g_DhcpserverIp, g_sDhcpOptions.val[dhcpServerIdentifier],4);
        }
		
        if (g_sDhcpOptions.len[dhcpMessageType] !=0)
        {
	        return *(unsigned char *)g_sDhcpOptions.val[dhcpMessageType];
        }
        else
        {
	        return 0;
        }
    }    
    else if(g_sDhcpOptions.len[dhcpMessageType] !=0 )
    {	
        memcpy((unsigned char *)&g_sDhcpOptions,(unsigned char *)&s_dhcpoptions,sizeof(dhcpOptions));
	
        if (g_sDhcpOptions.len[dhcpMessageType]!=0)
        {
            return *(unsigned char *)g_sDhcpOptions.val[dhcpMessageType];
        }
        else
        {
	        return 0;
        }
    }
    return 0;
}

int dhcpSendAndRecv(char* pcEthName, unsigned xid,unsigned msg,
                                  void (*buildUdpIpMsg)(unsigned))
{
    unsigned int	dwBegin = 0, dwCurrent = 0, dwDiff = 0;
    int 	len;
    int 	j=DHCP_INITIAL_RTO/2;
    int 	nTimeout = 0;
    int 	nRetryCount=6;
    int 	nMesstype;
    int    nRet;
    
    if(NULL == pcEthName)
    {
	    CSTCPrint("\n [dhcpSendAndRecv]pcEthName is null\n");
	    return -1;
    }

    do
    {
        j+=j;
        if (j > DHCP_MAX_RTO)
  	    {
	        j = DHCP_MAX_RTO;
  	    }

  	    buildUdpIpMsg(xid);/*���DHCPDISCOVER��DHCPREQUEST*/
 
  	    if ((buildUdpIpMsg == buildDhcpRequest)||(buildUdpIpMsg == buildDhcpDiscover) ) 
	    {
	        CSTCPrint("CSUDIRawSendPacket����ʧ��%d�Σ����ʣ��%d������\n", 6-nRetryCount, nRetryCount);	
	        nRetryCount--;
  	    }

	    nRet = CSUDIRawSendPacket(pcEthName, &g_nDhcpSocket, (char *)(&g_sDhcpMsgSend), sizeof(g_sDhcpMsgSend), EM_UDIRAWSENDTYPE_BROADCAST);	
	    if(nRet == -1)
	    {
	        CSTCPrint("CSUDIRawSendPacket ����ʧ�ܣ�����ֵ:%d\n", nRet);	
	        return -1;
	    }
		
  	    dwBegin = CSUDIPlusOSGetTickCount();
	    dwBegin = dwBegin/1000;

  	    if (( (buildUdpIpMsg == buildDhcpRequest)||
	        (buildUdpIpMsg == buildDhcpDiscover)) && 
	        (nRetryCount <= 0))
  	    {
	        break;
  	    }
		
	    CSUDIOSThreadSleep(10);
    }while ( peekfd(g_nDhcpSocket,j/2) );   

    do
    {		
  	    CSUDIOSThreadSleep(10);
	    len = sizeof(dhcpMessage);
	    nRet = CSUDIRawRecvPacket(pcEthName, &g_nDhcpSocket, (char *)(&g_sDhcpMsgRecv), &len, EM_UDIRAWSENDTYPE_BROADCAST);	
	    if(nRet == -1)
	    {
	        CSTCPrint("CSUDIRawRecvPacket ���հ�ʧ�ܣ�����ֵ:%d\n", nRet);	
	        return -1;
	    }	
		
    	dwCurrent = CSUDIPlusOSGetTickCount( );
      	dwCurrent = dwCurrent/1000;
      	dwDiff = dwCurrent-dwBegin;
      	nTimeout = j/2 - dwDiff*1000000  /*+ rand()%200000*/;
      	
      	if ( (&g_sDhcpMsgRecv)->htype != ARPHRD_ETHER ) 
      	{	
    	    continue; 
      	}
      	
      	if ( (&g_sDhcpMsgRecv)->xid != xid )
      	{		
    	    continue;
      	}
    
      	if ( (&g_sDhcpMsgRecv)->op != DHCP_BOOTREPLY ) 
    	{	
    	    continue;
      	}
    
    	if (memcmp(g_sDhcpMsgRecv.chaddr,aucMacAddress,ETH_ALEN)!=0)
    	{
    	    CSTCPrint("��ȡ���İ��е�Client MAC��ַ��ͻ��˵�MAC��ַ��ƥ�� \n");
            continue;
    	}
    	
      	if ( parseDhcpMsgRecv() == msg ) 
    	{		
            return 0;
      	}
      	
    	if (g_sDhcpOptions.len[dhcpMessageType]==0)
    	{	
    	    continue;
    	}
    
    	if (g_sDhcpOptions.len[dhcpMessageType]!=0)
    	{			
    	     nMesstype= *(unsigned char *)g_sDhcpOptions.val[dhcpMessageType];
    	}
    			
      	if ((g_sDhcpOptions.len[dhcpServerIdentifier]!=0)&&(*(unsigned char *)g_sDhcpOptions.val[dhcpMessageType] == DHCP_NAK)&&
    		(memcmp(g_DhcpserverIp, g_sDhcpOptions.val[dhcpServerIdentifier],4)==0))
    	{
      	    if ( g_sDhcpOptions.val[dhcpMsg] )
      	    {		
         		CSTCPrint("DHCP_NAK server response received: %s\n",(char *)g_sDhcpOptions.val[dhcpMsg]);
      	    }
      	    else
            {		
                CSTCPrint("DHCP_NAK server response received\n");
            }
                
            return 1;
         }
    }while ( nTimeout > 0 && peekfd(g_nDhcpSocket, nTimeout) == 0 );
	
    return 1;
}

int dhcpConfig(char* pcEthName)
{
    unsigned int unIpAddr;
    unsigned int unNetMask;
    unsigned int unGateway;
    char * pcTemp;
    char  acIpAddr[24],acNetMask[24],acGateway[24];

    if(NULL == pcEthName)
    {
        CSTCPrint("\n [dhcpSendAndRecv]pcEthName is null\n");
        return -1;
    }
 
    unIpAddr = (&g_sDhcpMsgRecv)->yiaddr;

    memcpy(&unNetMask,g_sDhcpOptions.val[subnetMask],4);
    memcpy(&unGateway,	((char *)g_sDhcpOptions.val[routersOnSubnet]),4);
	
    pcTemp = CSUDISOCKInet_ntoa(*(CSUDIInAddr_S*)&unIpAddr);
    memcpy(acIpAddr , pcTemp,sizeof("255.255.255.255"));
    CSTCPrint("\n CSUDISOCKInet_ntoa��ipת��Ϊ %s\n",acIpAddr);

    pcTemp = CSUDISOCKInet_ntoa(*(CSUDIInAddr_S*)&unNetMask);
    memcpy(acNetMask , pcTemp,sizeof("255.255.255.255"));
    CSTCPrint("\n CSUDISOCKInet_ntoa��maskת��Ϊ  %s\n",acNetMask);

    pcTemp = CSUDISOCKInet_ntoa(*(CSUDIInAddr_S*)&unGateway);
    memcpy(acGateway , pcTemp,sizeof("255.255.255.255"));
    CSTCPrint("\n SUDISOCKInet_ntoa��gatewayת��Ϊ  %s\n",acGateway);

    if (CSUDIIPCFGSetIpInfo(pcEthName, acIpAddr, acNetMask,acGateway)!=CSUDI_SUCCESS)
    {		
        CSTCPrint("\n CSUDIIPCFGSetIpInfo����ip, mask, gateway ʧ��\n");
        return -1;
    }
	
    return 0;	
}

static CSUDI_BOOL DHCP_iInitCfg()
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

static CSUDI_BOOL DHCP_iSetDefault(void)
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

CSUDI_BOOL CSTC_DHCP_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	CSUDI_BOOL bRet = CSUDI_FALSE;
	bRet = DHCP_iInitCfg();

	g_wDhcpMsgSize = CSUDISOCKHtons(sizeof(dhcpMessage));
	
	return bRet;
}

CSUDI_BOOL CSTC_DHCP_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}


//**********************************************************************************//
//******************************Begin Test  dhcp*******************************//
//**********************************************************************************//
//@CASEGROUP:dhcp
//@DESCRIPTION:�ɹ��õ�dhcp�����������ip��ַ��ֻ�����ڻ�ȡ������DHCP
//@PRECONDITION: �����ӵ�����dhcp������������
//@EXPECTATION:  �ɹ����غ�dhcp��������ͬ���ε�IP��ַ
//@EXECUTIONFLOW: 1��ѯ�ʲ�����Ա�������������ӵ�����DHCP�����������磬"Y"�������²��裬"N"��������ʧ��
//@EXECUTIONFLOW: 2��ִ��CSUDIIPCFGGetMacByName��ȡ����MAC��ַ��Ӧ�÷���CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW: 3��ִ��CSUDIIPCFGSetIpInfo����Ӧ������ip, mask, gateway���ó�"0.0.0.0"��Ӧ�÷���CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW: 4����װdiscover����ִ��CSUDISOCKHtons
//@EXECUTIONFLOW: 5��ִ��CSUDIRawSendPacket��discover����Ӧ�÷���0�������������ʧ��
//@EXECUTIONFLOW: 6����ೢ�Է���6��,ͨ���������½ӿ�(CSUDI_FD_ZERO;CSUDI_FD_SET;CSUDISOCKSelect,CSUDI_FD_ISSET,CSUDISOCKGetSockOpt)
//@EXECUTIONFLOW: 7��ִ��CSUDIRawRecvPacket����OFFER����Ӧ�÷���0�������������ʧ��
//@EXECUTIONFLOW: 8��ͨ���������½ӿ�(CSUDI_FD_ZERO;CSUDI_FD_SET;SUDISOCKSelect,CSUDI_FD_ISSET,CSUDISOCKGetSockOpt)���ж��Ƿ������������OFFER���������յ�OFFER�������ִ������Ĳ��裬�����������ֱ����ʱ���Ѿ�����ʱʱ�仹û���յ���Ҫ��OFFER������������ʧ��
//@EXECUTIONFLOW: 9����װrequest����ִ��CSUDISOCKHtons
//@EXECUTIONFLOW: 10��ִ��CSUDIRawSendPacket��request����Ӧ�÷���0�������������ʧ��
//@EXECUTIONFLOW: 11����ೢ�Է���6��,ͨ���������½ӿ�(CSUDI_FD_ZERO;CSUDI_FD_SET;CSUDISOCKSelect,CSUDI_FD_ISSET,CSUDISOCKGetSockOpt)
//@EXECUTIONFLOW: 12��ִ��CSUDIRawRecvPacket����ACK����Ӧ�÷���0�������������ʧ��
//@EXECUTIONFLOW: 13��ͨ���������½ӿ�(CSUDI_FD_ZERO;CSUDI_FD_SET;CSUDISOCKSelect,CSUDI_FD_ISSET,CSUDISOCKGetSockOpt)�ж��Ƿ����������ACK���������յ�ACK�������ִ������Ĳ��裬�����������ֱ����ʱ���Ѿ�����ʱʱ�仹û���յ���Ҫ��ACK������������ʧ��
//@EXECUTIONFLOW: 14��ִ��CSUDISOCKInet_ntoa����ȡ����ip, mask, gatewayת�ɵ��IP��ַ
//@EXECUTIONFLOW: 15��ִ��CSUDIIPCFGSetIpInfo��ת�����ip, mask, gateway���õ�������Ӧ�÷���CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW: 16��ִ��CSUDIIPCFGGetIpInfo��ȡ������ip��Ӧ�÷���CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW: 17��ѯ�ʲ�����Ա����ȡ����IP�������ӵĻ����Ƿ���ͬһ�����Σ�������"Y"    
//@EXECUTIONFLOW: 18��ִ��CSUDISOCKClose�رռ����׽��֣��ָ�����
CSUDI_BOOL CSTC_DHCP_IT_0001(void)
{
	char szGetIp[16];
	char szGetMask[16];
	char szGetGateWay[16];

	memset(szGetIp, 0, sizeof(szGetIp));
	memset(szGetMask, 0, sizeof(szGetMask));
	memset(szGetGateWay, 0, sizeof(szGetGateWay));

	if (g_nDhcpSocket != -1 )
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDISOCKClose(g_nDhcpSocket), "�رռ����׽���ʧ��");
		g_nDhcpSocket = -1;
	}
	
	CSTCPrint("��ȷ�������Ի������Ѿ����ӵ�����DHCP�����������磬Y���������ӣ�N����δ����\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N�����Ի���δ׼����");  

	memset(aucMacAddress, 0, sizeof(aucMacAddress));
	memset(&g_sDhcpMsgSend, 0, sizeof(g_sDhcpMsgSend));
	memset(&g_sDhcpMsgRecv, 0, sizeof(g_sDhcpMsgRecv));
	memset(&s_dhcpoptions, 0, sizeof(s_dhcpoptions));
	memset(&g_DhcpserverIp, 0, sizeof(g_DhcpserverIp));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetMacByName(s_asLanCfg[0].szEthName, aucMacAddress), "CSUDIIPCFGGetMacByName��ȡ��һ��������MACʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGSetIpInfo(s_asLanCfg[0].szEthName, "0.0.0.0", "0.0.0.0", "0.0.0.0"), "CSUDIIPCFGSetIpInfo����IPΪ0ʧ��");

	CSTK_ASSERT_TRUE_FATAL(0 == dhcpSendAndRecv(s_asLanCfg[0].szEthName, rand(),DHCP_OFFER,&buildDhcpDiscover), "���ͽ���discoverʧ��"); 

  	CSTK_ASSERT_TRUE_FATAL(0 == dhcpSendAndRecv(s_asLanCfg[0].szEthName, (&g_sDhcpMsgRecv)->xid,DHCP_ACK,&buildDhcpRequest), "���ͽ���requestʧ��");

	CSTK_ASSERT_TRUE_FATAL(0 == dhcpConfig(s_asLanCfg[0].szEthName), "������������IPʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIIPCFGGetIpInfo(s_asLanCfg[0].szEthName, szGetIp, szGetMask, szGetGateWay), "CSUDIIPCFGGetIpInfo��ȡIPʧ��");
	
	CSTCPrint("\n��DHCP��������ȡ����IPΪ  %s\n",szGetIp);
	
	CSTCPrint("��ȷ�ϻ�ȡ����IP�Ƿ�����ӵ�������ͬһ������\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "������N����ȡ����IP���Ǻ����ӵ�����ͬһ������"); 

	CSTK_FATAL_POINT
	if (g_nDhcpSocket != -1 )
	{
		CSTK_ASSERT_TRUE_FATAL(0 == CSUDISOCKClose(g_nDhcpSocket), "�رռ����׽���ʧ��");
		g_nDhcpSocket = -1;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == DHCP_iSetDefault(), "�ָ�Ĭ��IP����");
	
	return CSUDI_TRUE;
}

