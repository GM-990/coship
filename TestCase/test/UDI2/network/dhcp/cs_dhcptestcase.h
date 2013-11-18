#ifndef _CS_DHCP_TEST_CASE_H_
#define _CS_DHCP_TEST_CASE_H_

#include "cs_testkit.h"
#include "udi2_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DHCP message type */
#define	DHCP_DISCOVER		1
#define DHCP_OFFER			2
#define	DHCP_REQUEST		3
#define	DHCP_ACK			5
#define DHCP_NAK			6

#define DHCP_INITIAL_RTO    ( 2*1000000)
#define DHCP_MAX_RTO        ( 32*1000000)
#define ARPHRD_ETHER		1

/* DHCP message OP code */
#define DHCP_BOOTREQUEST	1
#define DHCP_BOOTREPLY		2

#define   dhcpMessageType               TAG_DHCP_MESS_TYPE 
#define   dhcpServerIdentifier          TAG_DHCP_SERVER_ID  
#define   dhcpMsg                     	TAG_DHCP_TEXT_MESSAGE  
#define   dhcpMaxMsgSize                TAG_DHCP_MAX_MSGSZ 


#define DEFAULT_LEASETIME	0xffffffff	/* infinite lease time */
#define BROADCAST_FLAG		0x8000
#define UNICAST_FLAG		0x0000

/* UDP port numbers for DHCP */
#define	DHCP_SERVER_PORT	67	/* from client to server */
#define DHCP_CLIENT_PORT	68	/* from server to client */

#define ETH_ALEN	6	

/*======zch add begin=============*/
#define STRING_SIZE  (64)

#define   padOption                   	TAG_PAD 
#define   subnetMask                	TAG_SUBNET_MASK    
#define   timerOffset                 	TAG_TIME_OFFSET      
#define   routersOnSubnet        		TAG_GATEWAY           
#define   timeServer                 	TAG_TIME_SERVER     
#define   nameServer                	TAG_NAME_SERVER     
#define   dns                           TAG_DOMAIN_SERVER 
#define   logServer                     TAG_LOG_SERVER       
#define   cookieServer              	TAG_COOKIE_SERVER   
#define   lprServer                    	TAG_LPR_SERVER        
#define   impressServer            		TAG_IMPRESS_SERVER           
#define   resourceLocationServer 		TAG_RLP_SERVER     
#define   hostName                     	TAG_HOST_NAME  
#define   bootFileSize                	TAG_BOOT_SIZE 

#define  meritDumpFile              	TAG_DUMP_FILE
#define  domainName                		TAG_DOMAIN_NAME
#define  vendSpecific                		TAG_VEND_SPECIFIC// sunyongchao edit
#define  swapServer                 	TAG_SWAP_SERVER
#define  rootPath   					TAG_ROOT_PATH
#define  extentionsPath      			TAG_EXTEN_FILE
#define  IPforwarding     				TAG_IP_FORWARD
#define  nonLocalSourceRouting 			TAG_IP_NLSR
#define  policyFilter        			TAG_IP_POLICY_FILTER
#define  maxDgramReasmSize    			TAG_IP_MAX_DRS
#define  defaultIPTTL       		 	TAG_IP_TTL
#define  pathMTUagingTimeout            TAG_IP_MTU_AGE
#define  pathMTUplateauTable      		TAG_IP_MTU_PLAT
#define  ifMTU				 			TAG_IP_MTU
#define  allSubnetsLocal    	 		TAG_IP_SNARL
#define  broadcastAddr					TAG_IP_BROADCAST

#define  performMaskDiscovery           TAG_IP_SMASKDISC
#define  maskSupplier                	TAG_IP_SMASKSUPP
#define  performRouterDiscovery         TAG_IP_ROUTERDISC
#define  routerSolicitationAddr   		TAG_IP_ROUTER_SOL_ADDR
#define  staticRoute      				TAG_IP_STATIC_ROUTES
#define  trailerEncapsulation     		TAG_IP_TRAILER_ENC
#define  arpCacheTimeout 				TAG_ARP_TIMEOUT
#define  ethernetEncapsulation        	TAG_ETHER_IEEE
#define  tcpDefaultTTL   		 		TAG_IP_TCP_TTL
#define  tcpKeepaliveInterval           TAG_IP_TCP_KA_INT
#define  tcpKeepaliveGarbage            TAG_IP_TCP_KA_GARBAGE
#define  nisDomainName      			TAG_NIS_DOMAIN
#define  nisServers				 		TAG_NIS_SERVER
#define  ntpServers    	 				TAG_NTP_SERVER
#define  vendorSpecificInfo				TAG_VEND_SPECIFIC

#define   netBIOSnameServer             TAG_NBNS_SERVER 
#define   netBIOSdgramDistServer        TAG_NBDD_SERVER    
#define   netBIOSnodeType               TAG_NBOTCP_OTPION      
#define   netBIOSscope        			TAG_NB_SCOPE           
#define   xFontServer                 	TAG_XFONT_SERVER     
#define   xDisplayManager               TAG_XDISPLAY_SERVER     
#define   dhcpRequestedIPaddr           TAG_DHCP_REQ_IP 
#define   dhcpIPaddrLeaseTime           TAG_DHCP_LEASE_TIME       
#define   dhcpOptionOverload            TAG_DHCP_OPTOVER   
#define   dhcpMessageType               TAG_DHCP_MESS_TYPE        
#define   dhcpServerIdentifier          TAG_DHCP_SERVER_ID           
#define   dhcpParamRequest 		    	TAG_DHCP_REQ_LIST     
#define   dhcpMsg                     	TAG_DHCP_TEXT_MESSAGE  
#define   dhcpMaxMsgSize                TAG_DHCP_MAX_MSGSZ 

#define   dhcpT1value                   TAG_DHCP_RENEWAL_TIME 
#define   dhcpT2value                	TAG_DHCP_REBIND_TIME    
#define   dhcpClassIdentifier           TAG_DHCP_CLASSID     
#define   dhcpMitel125           TAG_DHCP_MITEL125      
#define   dhcpClientIdentifier        	TAG_DHCP_CLIENTID           
#define   endOption                 	TAG_END     

/*=============zch add end ==========*/

typedef struct dhcpMessage
{
  unsigned char  op;		/* message type */
  unsigned char  htype;	/* hardware address type */
  unsigned char  hlen;		/* hardware address length */
  unsigned char  hops;		/* should be zero in client's message */
  unsigned int	   xid;		/* transaction id */
  unsigned short secs;		/* elapsed time in sec. from trying to boot */
  unsigned short flags;
  unsigned int	   ciaddr;	/* (previously allocated) client IP address */
  unsigned int		  yiaddr;	/* 'your' client IP address */
  unsigned int		  siaddr;	/* should be zero in client's messages */
  unsigned int		  giaddr;	/* should be zero in client's messages */
  unsigned char  chaddr[16];	/* client's hardware address */
  unsigned char  sname[64];	/* server host name, null terminated string */
  unsigned char  file[128];	/* boot file name, null terminated string */
  unsigned char  options[312];	/* message options */
} dhcpMessage;

typedef struct dhcpOptions
{
  unsigned int num;
  unsigned char len[256];
  unsigned char val[256][64];/*最好设置为  unsigned char val[256][256]*/

} dhcpOptions;


CSUDI_BOOL CSTC_DHCP_Init(void);
CSUDI_BOOL CSTC_DHCP_UnInit(void);
CSUDI_BOOL CSTC_DHCP_IT_0001(void);


#ifdef __cplusplus
}
#endif

#endif

