/*****************************************************************************
 * cs_dhcpc.h
 *
 *****************************************************************************/

#ifndef __DHCPC_H_INCLUDED__
#define __DHCPC_H_INCLUDED__

#if defined(__cplusplus) || defined(__CPLUSPLUS)
extern "C" {
#endif 
#include "mmcp_typedef.h"

#define DHCPC_RUNNING     	0x00  /* Running DHCP client */
#define DHCPC_SLEEPING    	0x01  /* Sleeping DHCP client */
#define DHCPC_MATCHMASK   	0x02  /* Bit used for server selecting policy */
#define DHCPC_MATCHFIRST  	(0<<1) /* Takes first answering lease server */
#define DHCPC_MATCHLEASE  	(1<<1) /* Selects longest lease time */
#define DHCPC_INFORMSENT  	0x04  /* DHCPINFORM sent (for client with IP addr) */
#define DHCPC_SET_GW      	0x08  /* Set default gateway if provided by DHCP */
#define DHCPC_SET_RSLV    	0x10  /* Set resolver if provided by DHCP */

typedef struct DhcpClientInfo_S 
{
    unsigned char  	dc_state;     /* Internal DHCP client state */
    unsigned char  	dc_mode;      /* DHCP client mode */
    unsigned int   	dc_xid;       /* Client transaction ID */
    unsigned int   	dc_saddr;     /* IP address of selected lease server */
    unsigned int   	dc_caddr;     /* Client IP address */
    unsigned long  	dc_T1;        /* Remaining time before renewing (ticks) */
    unsigned long  	dc_T2;        /* Remaining time before rebinding (ticks) */
    unsigned long  	dc_lease;     /* Lease time for client IP address (ticks) */    
    unsigned int  	*dc_reject;    /* IP addresses of indesirable lease servers */
    const char    	*dc_hostname;  /* local host name */
    const char    	*dc_fqdn;      /* fully-qualified domain name */
    unsigned char  	dc_ethid[7];  /* default client id (ethernet address) */ 
} DhcpClientInfo;


/*
 *event      callback context (UP, DOWN, INFORM)
 *incoming   bit mask for incoming option 
 *client    calling DHCP client info
 */

#define CSDHCP_SUCCESS   0           /*���巵��ֵ���ɹ�����CSDHCP_SUCCESS */
#define CSDHCP_FAILURE  -1           /*���巵��ֵ��ʧ�ܷ���CSDHCP _FAILURE */
#define DHCP_MAX_CALLBACK_COUNT   (20)
typedef void  (*DhcpCallBackFun_p)( int event, unsigned int incoming, DhcpClientInfo *client);
/*��Ӷ�ص����������Ժ����CSDHCPAddCallback�����������������͵�callback*/
typedef void  (*fnDhcpCallBack_p)( int event, unsigned int incoming, DhcpClientInfo *client,DWORD dwUserData);
/* ���ע���ȡ������Ψһ��ʶ������*/
typedef unsigned int(*fnGetIdentify_F)();

typedef enum
{
    EM_DHCPC_RENEWTYPE_UNICAST = 0,//����
    EM_DHCPC_RENEWTYPE_BROADCAST,//�㲥
    EM_DHCPC_RENEWTYPE_UNDEFINED
} CSDHCPCRenwType_E;

typedef enum
{
    EM_DHCPC_UNICAST = 0,//����
    EM_DHCPC_BROADCAST,//�㲥
    EM_DHCPC_UNDEFINED
} CSDHCPCType_E;

typedef enum CSDHCP_CONFIG_e
{
	/* DHCP configurating options (argument type is commented) */
	/* Setting timer for selecting state (global DHCP option) */
	 DHCPCO_TIMERSELECT ,     /* unsigned long */

	/* Set network interface on which DHCP should run on , by name*/
	 DHCPCO_IF_NAME ,          /* Netif name* */

	/* Set DHCP client criteria to select lease */ 
	 DHCPCO_MODE ,            /* unsigned char * */

	/* IP address of expected (requested) lease server */
	 DHCPCO_SADDR,            /* unsigned int * */

	/* Suggested client IP address */
	 DHCPCO_CADDR,            /* unsigned int* */

	/* Default time before renewing (for a virtual lease time of 1000) */
	 DHCPCO_T1,               /* int* */

	/* Default time before rebinding (for a virtual lease time of 1000) */
	 DHCPCO_T2,               /* int * */

	/* Suggested lease time for IP address */
	 DHCPCO_LEASE ,            /* unsigned long * */

	/* Null terminated list of IP address of lease servers to reject */
	 DHCPCO_REJ_SERVERS ,      /* unsigned int* * */

	/* Requested option list (sequence of tag/size/value, ended with TAG_END) */
	 DHCPCO_OPTLIST,          /* NGdhcpoption* */

	/* User callback function */
	 DHCPCO_CALLBACK ,         /*DhcpCallBackFun_p *  */

	/* Setting DHCP random seed */
	 DHCPCO_RANDOMSEED,       /* unsigned long * */

	/* Setting DHCP client id */
	 DHCPCO_CLIENTID,         /* unsigned char ** */

	 DHCPCO_CLASSID,              /* unsigned char ** */
	 DHCPCO_FOREIGN_CLASSID,     /* unsigned char ** */

	 DHCPCO_FOREIGN_MITEL125,

	 DHCPCO_CLASSID_LEN,              /* unsigned char ** */
	 DHCPCO_FOREIGN_CLASSID_LEN,     /* unsigned char ** */

	 DHCPCO_FOREIGN_MITEL125_LEN,
	
	/* Setting DHCP client id length */
	 DHCPCO_CLIENTID_LEN,     /* int * */

	/* Enabling/disabling configuration of default route */
	 DHCPCO_SET_GATEWAY,      /* CFG_TRUE or CFG_FALSE */

	/* Enabling/disabling configuration of resolver */
	 DHCPCO_SET_RESOLVER ,     /* CFG_TRUE or CFG_FALSE */

	/* Setting local host name */
	 DHCPCO_HOSTNAME,         /* const char** */

	/* Setting fully-qualified domain name */
	 DHCPCO_FQDN ,             /* const char** */

	/* State of DHCP client */
	 DHCPCO_CUR_STATE ,        /* unsigned int  (read-only) */

	/* IP address of client */
	 DHCPCO_CUR_CADDR,        /* unsigned int (read-only) */

	/* IP address of lease server */
	 DHCPCO_CUR_SADDR ,       /* unsigned int (read-only) */

	/* Time left before renewing (in seconds) */
	 DHCPCO_CUR_T1,           /* unsigned long (read-only) */

	/* Time left before rebinding (in seconds) */
	 DHCPCO_CUR_T2,           /* unsigned long (read-only) */

	/* Lease time for IP address (in seconds) */
	 DHCPCO_CUR_LEASE  ,      /* unsigned long (read-only) */

	/*  the domain name that client should use when   resolving hostnames via the Domain Name System */
	 DHCPCO_DOMAIN_NAME ,     /* unsigned char */

       /* This option is used by clients and servers to exchange vendor-specific information. */
        DHCPCO_VEND_SPECIFIC         /* unsigned char */

	
	
}DhcpConfig_E;

/* Macros for user callback function */
/*   Event */
typedef enum CSDHCP_EVENT_e
{
	 UP,                 	 /* interface has just entered UP state */
	 DOWN,                 /* interface has just entered DOWN state */
	 INFORM,              /* some user requested opt have just arrived */
	 RENEW,              	/*interface renew begin*//*chenlin add 06.09.28*/
	 RENEWED,            /*interface renew success*//*chenlin add 06.09.28*/
	 RELEASED
}DhcpEvent_E;


typedef enum
{
	EM_DHCPOPT_CHECK = 0,/*������opt60������£��ͻ�����Ҫ�ϸ���֤server classid��ֵ����֤ͨ���������¸����ݰ�*/
	EM_DHCPOPT_NOCHECK   /*������opt60������£��յ��İ����������server classid,�ͻ��˿���ֱ������*/
}CSDHCPOptCheckMode_E;


/*
 * Tag values used to specify what information is being supplied in
 * the vendor (options) data area of the packet.
 */
/* RFC 1048 */
/* End of cookie */
#define TAG_END            ((unsigned char) 255)
/* padding for alignment */
#define TAG_PAD            ((unsigned char)   0)
/* Subnet mask */
#define TAG_SUBNET_MASK    ((unsigned char)   1)
/* Time offset from UTC for this system */
#define TAG_TIME_OFFSET    ((unsigned char)   2)
/* List of routers on this subnet */
#define TAG_GATEWAY        ((unsigned char)   3)
/* List of rfc868 time servers available to client */
#define TAG_TIME_SERVER    ((unsigned char)   4)
/* List of IEN 116 name servers */
#define TAG_NAME_SERVER    ((unsigned char)   5)
/* List of DNS name servers */
#define TAG_DOMAIN_SERVER  ((unsigned char)   6)
/* List of MIT-LCS UDL log servers */
#define TAG_LOG_SERVER     ((unsigned char)   7)
/* List of rfc865 cookie servers */
#define TAG_COOKIE_SERVER  ((unsigned char)   8)
/* List of rfc1179 printer servers (in order to try) */
#define TAG_LPR_SERVER     ((unsigned char)   9)
/* List of Imagen Impress servers (in prefered order) */
#define TAG_IMPRESS_SERVER ((unsigned char)  10)
/* List of rfc887 Resourse Location servers */
#define TAG_RLP_SERVER     ((unsigned char)  11)
/* Hostname of client */
#define TAG_HOST_NAME      ((unsigned char)  12)
/* boot file size */
#define TAG_BOOT_SIZE      ((unsigned char)  13)
/* RFC 1395 */
/* path to dump to in case of crash */
#define TAG_DUMP_FILE      ((unsigned char)  14)
/* domain name for use with the DNS */
#define TAG_DOMAIN_NAME    ((unsigned char)  15)
/* IP address of the swap server for this machine */
#define TAG_SWAP_SERVER    ((unsigned char)  16)
/* The path name to the root filesystem for this machine */
#define TAG_ROOT_PATH      ((unsigned char)  17)
/* RFC 1497 */
/* filename to tftp with more options in it */
#define TAG_EXTEN_FILE     ((unsigned char)  18)
/* RFC 1533 */
/* The following are in rfc1533 and may be used by BOOTP/DHCP */
/* IP forwarding enable/disable */
#define TAG_IP_FORWARD          ((unsigned char)  19)
/* Non-Local source routing enable/disable */
#define TAG_IP_NLSR             ((unsigned char)  20)
/* List of pairs of addresses/masks to allow non-local source routing to */
#define TAG_IP_POLICY_FILTER    ((unsigned char)  21)
/* Maximum size of datagrams client should be prepared to reassemble */
#define TAG_IP_MAX_DRS          ((unsigned char)  22)
/* Default IP TTL */
#define TAG_IP_TTL              ((unsigned char)  23)
/* Timeout in seconds to age path MTU values found with rfc1191 */
#define TAG_IP_MTU_AGE          ((unsigned char)  24)
/* Table of MTU sizes to use when doing rfc1191 MTU discovery */
#define TAG_IP_MTU_PLAT         ((unsigned char)  25)
/* MTU to use on this interface */
#define TAG_IP_MTU              ((unsigned char)  26)
/* All subnets are local option */
#define TAG_IP_SNARL            ((unsigned char)  27)
/* broadcast address */
#define TAG_IP_BROADCAST        ((unsigned char)  28)
/* perform subnet mask discovery using ICMP */
#define TAG_IP_SMASKDISC        ((unsigned char)  29)
/* act as a subnet mask server using ICMP */
#define TAG_IP_SMASKSUPP        ((unsigned char)  30)
/* perform rfc1256 router discovery */
#define TAG_IP_ROUTERDISC       ((unsigned char)  31)
/* address to send router solicitation requests */
#define TAG_IP_ROUTER_SOL_ADDR  ((unsigned char)  32)
/* list of static routes to addresses (addr, router) pairs */
#define TAG_IP_STATIC_ROUTES    ((unsigned char)  33)
/* use trailers (rfc893) when using ARP */
#define TAG_IP_TRAILER_ENC      ((unsigned char)  34)
/* timeout in seconds for ARP cache entries */
#define TAG_ARP_TIMEOUT         ((unsigned char)  35)
/* use either Ethernet version 2 (rfc894) or IEEE 802.3 (rfc1042) */
#define TAG_ETHER_IEEE          ((unsigned char)  36)
/* default TCP TTL when sending TCP segments */
#define TAG_IP_TCP_TTL          ((unsigned char)  37)
/* time for client to wait before sending a keepalive on a TCP connection */
#define TAG_IP_TCP_KA_INT       ((unsigned char)  38)
/* don't send keepalive with an octet of garbage for compatability */
#define TAG_IP_TCP_KA_GARBAGE   ((unsigned char)  39)
/* NIS domainname */
#define TAG_NIS_DOMAIN          ((unsigned char)  40)
/* list of NIS servers */
#define TAG_NIS_SERVER          ((unsigned char)  41)
/* list of NTP servers */
#define TAG_NTP_SERVER          ((unsigned char)  42)
/* and stuff vendors may want to add */
#define TAG_VEND_SPECIFIC       ((unsigned char)  43)
/* NetBios over TCP/IP name server */
#define TAG_NBNS_SERVER         ((unsigned char)  44)
/* NetBios over TCP/IP NBDD servers (rfc1001/1002) */
#define TAG_NBDD_SERVER         ((unsigned char)  45)
/* NetBios over TCP/IP node type option for use with above */
#define TAG_NBOTCP_OTPION       ((unsigned char)  46)
/* NetBios over TCP/IP scopt option for use with above */
#define TAG_NB_SCOPE            ((unsigned char)  47)
/* list of X Window system font servers */
#define TAG_XFONT_SERVER        ((unsigned char)  48)
/* list of systems running X Display Manager (xdm) available to this client */
#define TAG_XDISPLAY_SERVER     ((unsigned char)  49)

/* DHCP specific option tags */
/* requested IP address */
#define TAG_DHCP_REQ_IP         ((unsigned char)  50)
/* IP address lease time */
#define TAG_DHCP_LEASE_TIME     ((unsigned char)  51)
/* options overload (1:file, 2:sname, 3:both) */
#define TAG_DHCP_OPTOVER        ((unsigned char)  52)
/* type of DHCP message */
#define TAG_DHCP_MESS_TYPE      ((unsigned char)  53)
/* server identification */
#define TAG_DHCP_SERVER_ID      ((unsigned char)  54)
/* ordered list of requested option */
#define TAG_DHCP_REQ_LIST       ((unsigned char)  55)
/* reply message */
#define TAG_DHCP_TEXT_MESSAGE   ((unsigned char)  56)
/* largest DHCP message client will accept */
#define TAG_DHCP_MAX_MSGSZ      ((unsigned char)  57)
/* time 'til client needs to renew (T1) */
#define TAG_DHCP_RENEWAL_TIME   ((unsigned char)  58)
/* time 'til client needs to rebind (T2) */
#define TAG_DHCP_REBIND_TIME    ((unsigned char)  59)

/* vendor class identifier */
#define TAG_DHCP_CLASSID        ((unsigned char)  60)

/* client identifier (unique on client's subnet) */
#define TAG_DHCP_CLIENTID       ((unsigned char)  61)

/* some option missing ??? */

/* name of NIS domain */
#define TAG_DHCP_NIS_DOMAIN     ((unsigned char)  64)
/* IP addresses of NIS servers */
#define TAG_DHCP_NIS_SERVERS    ((unsigned char)  65)
/* name of TFTP server (if sname has been used for options) */
#define TAG_DHCP_TFTP_SERVER    ((unsigned char)  66)
/* bootfile name (if sname has been used for options) */
#define TAG_DHCP_BOOTFILE       ((unsigned char)  67)
/* IP addresses of mobile IP home agents */
#define TAG_DHCP_MOB_AGENTS     ((unsigned char)  68)
/* IP addresses of SMTP servers */
#define TAG_DHCP_SMTP_SERVERS   ((unsigned char)  69)
/* IP addresses of POP3 servers */
#define TAG_DHCP_POP3_SERVERS   ((unsigned char)  70)
/* IP addresses of NNTP servers */
#define TAG_DHCP_NNTP_SERVERS   ((unsigned char)  71)
/* IP addresses of WWW servers */
#define TAG_DHCP_WWW_SERVERS    ((unsigned char)  72)
/* IP addresses of finger servers */
#define TAG_DHCP_FING_SERVERS   ((unsigned char)  73)
/* IP addresses of IRC servers */
#define TAG_DHCP_IRC_SERVERS    ((unsigned char)  74)
/* IP addresses of StreetTalk servers */
#define TAG_DHCP_ST_SERVERS     ((unsigned char)  75)
/* IP addresses of StreetTalk Directory Assistance servers */
#define TAG_DHCP_STDA_SERVERS   ((unsigned char)  76)
/* Fully-Qualified Domain Name */
#define TAG_DHCP_FQDN           ((unsigned char)  81)
#define TAG_DHCP_MITEL125		((unsigned char)  125)	
/* Please add any new tags here */

/* DHCP option values */

/* DHCP message types (see TAG_DHCP_MESS_TYPE) */
#define DHCPDISCOVER    1
#define DHCPOFFER       2
#define DHCPREQUEST     3
#define DHCPDECLINE     4
#define DHCPACK         5
#define DHCPNACK        6
#define DHCPRELEASE     7
#define DHCPINFORM      8

/* Overload option tag values (see TAG_DHCP_OPTOVER) */
#define DHCP_OPTOVER_FILE  1 /* Only file is filled with options */
#define DHCP_OPTOVER_SNAME 2 /* Only sname is filled with options */
#define DHCP_OPTOVER_BOTH  3 /* Both file and sname are filled with options */

/* structure to allow to request specific options */
typedef struct Dhcpoption_s
{
	  unsigned char 	do_tag;          /* tag (cf. header files dhcp.h bp_tags.h )*/
	  unsigned char 	do_size;         /* length of the value (bytes count) */
	  unsigned char	*do_data;       /* location of data */
} Dhcpoption;


/*************************************************************************
 ����: DhcpcStart 
 ��������: 
 	����dhcp
 �������: 
 	pchNetifName: �����豸��,��"eth0"
 �������: 
 	�� 
 ����ֵ: 
 	-26 invalid parameter
 	-41 no such file or directory
 	0     success	
 ��ע: 
 	��
*************************************************************************/
int DhcpcStart( char* pchNetifName);

/*************************************************************************
 ����: DhcpcRelease 
 ��������: 
 	�ͷ�֮ǰdhcp��ȡ����IP��ַ  
 �������: 
 	pchNetifName: �����豸��,��"eth0"
 �������: 
 	��  
 ����ֵ: 
 	-26 invalid parameter
 	-39 out of resources
 	0     success	
 ��ע: 
 	��
*************************************************************************/
int DhcpcRelease( char* pchNetifName);
/*************************************************************************
 ����: DhcpcSetOption 
 ��������: 
 	����DHCP�ͻ���ѡ�� �� 
 �������: 
 	pchNetifName: �����豸��,��"eth0"
 	opt:�����õ�dhcp�ͻ���ѡ��,һ����Ҫ���õĲ����У�DHCPCO_CALLBACK?
 	    �DHCPCO_SET_RESOLVER��DHCPCO_SET_GATEWAY(���궨��DHCP������Ϣ)��ֵ
 	optval:ָ��ѡ�������ֵ
 �������: 
 	��  
 ����ֵ: 
 	-26 invalid parameter
 	-41 no such file or directory
 	0     success
 	
 ��ע: 
*************************************************************************/
int DhcpcSetOption( char* pchNetifName,  int opt, void* optval);


/*************************************************************************
 ����: DhcpcGetOption 
 ��������: 
 	��ȡDHCPѡ���ֵ  
 �������: 
 	pchNetifName: �����豸��,��"eth0"
 	opt:����ȡ��dhcp�ͻ���ѡ��	
 �������: 
 	optval:��������ѡ�������ֵ����δ���ã������ΪĬ��ֵ
 ����ֵ: 
 	-26 invalid parameter
 	-41 no such file or directory
 	0     success
 ��ע: 
 	None
*************************************************************************/
int DhcpcGetOption( char* pchNetifName,  int opt, void* optval);


/*************************************************************************
 ����: CSDhcpcSetOptClassID 
 ��������: 
 	����OPT60(Classid)��ֵ
 �������: 
 	pchNetifName: �����豸��,��"eth0"
 	pcClientClassID:�ͻ��˵�classid
 	pcServerClassID:�������˵�classid
 �������: 
 	��  
 ����ֵ: 
 	��
 ��ע: 
*************************************************************************/

int CSDhcpcSetOptClassID(
	const  char* pcNetIfName, 
	const  char* pcClientClassID, 
	const  char* pcServerClassID );

/*************************************************************************
 ����: DhcpcSendInform 
 ��������: 
 	�ͻ��˹㲥һ��DHCPINFORM������˻�ø��������ѡ��ֵ
 Input: 
 	pchNetifName: �����豸��,��"eth0"
 	saddr:DHCP��������IP��ַ
 	req_opt:Dhcpoption �ṹ��ĵ�ַ
 Output: 
 	��  
 Return: 
 	   NG_EOK       success
       else failure
 Note: 
 	None
*************************************************************************/
int DhcpcSendInform( char* pchNetifName, unsigned int saddr, Dhcpoption* req_opt);


/*************************************************************************
 Function: CSSuspendSendMsgThread 
 Description: 
 	����DHCP�߳�
 Input: 
 	��
 Output: 
 	��  
 Return: 
 	��
 Note: 
 	��
*************************************************************************/
void CSSuspendSendMsgThread(void);
/*************************************************************************
 Function: CSSuspendSendMsgThread 
 Description: 
 	�ָ�DHCP�߳�
 Input: 
 	��
 Output: 
 	��  
 Return: 
 	��
 Note: 
 	��
*************************************************************************/
void CSResumeSendMsgThread(void);


/*************************************************************************
 ����: CSDHCPInit 
 ��������: 
 	��ʼ��DHCPģ���е�ȫ�ֱ��� ,���ȱ�����
 �������: 
 	pchNetifName: �����豸��,��"eth0"
 �������: 
 	�� 
 ����ֵ: 
    �����ɹ���CSDHCP_SUCCESS
    ����ʧ�ܣ�CSDHCP _FAILURE
	
 ��ע: 
 	��
*************************************************************************/
int CSDHCPInit( char * pcEthName);


/*************************************************************************
 ����: CSDHCPSetRenewUnicastMode 
 ��������: 
 	����dhcp��renew״̬ʱ���͹㲥�����ǵ���������Ҫ����Ϊ����
 	����������dhcp����ǰ���ô˽ӿڡ�
 �������: 
 	eRenewType: ��CSDHCPCRenwType_E  ������Ĭ��Ϊ���͹㲥��
 �������: 
 	�� 
 ����ֵ: 
    ���óɹ���CSDHCP_SUCCESS
    ����ʧ�ܣ�CSDHCP _FAILURE
	
 ��ע: 
 	��
*************************************************************************/
int CSDHCPSetRenewUnicastMode(CSDHCPCRenwType_E eRenewType);

/*************************************************************************
 ����: CSDHCPSetThreeFourthsUnicast 
 ��������: 
 	����dhcp���ķ�֮�����ڵ�ʱ���͵�������������
 �������: 
 	bMode: ����ΪTRUE���ʾ��dhcp 1/2���ɹ���ʱ�򣬼������ķ�
 	֮�����ڷ���һ������������������ΪFALSE����Ҫ�ڴ�
 	ʱ�̷��͵�����
	Ĭ�ϲ����ͣ�����dhcp��׼�淶
 
 �������: 
 	�� 
 ����ֵ: 
    ���óɹ���CSDHCP_SUCCESS
    ����ʧ�ܣ�CSDHCP _FAILURE
	
 ��ע: 
 	��
*************************************************************************/
int CSDHCPSetThreeFourthsUnicast(BOOL bMode);

/*************************************************************************
 ����: CSDHCPCleanup 
 ��������: 
 	�ͷ���CSDHCPInit�������Դ��������ʹ�øú��� 
 �������: 
 	��
 �������: 
 	��  
 ����ֵ: 
    ���ٳɹ���CSDHCP_SUCCESS
    ����ʧ�ܣ�CSDHCP _FAILURE
	
 ��ע: 
 	None
*************************************************************************/

int CSDHCPCleanup(void);

/*************************************************************************
 ����: CSDHCPSetOption 
 ��������: 
 	����DHCP�ͻ���ѡ�� �� 
 �������: 
 	pchNetifName: �����豸��,��"eth0"
 	opt:�����õ�dhcp�ͻ���ѡ��,һ����Ҫ���õĲ����У�DHCPCO_CALLBACK?
 	    �DHCPCO_SET_RESOLVER��DHCPCO_SET_GATEWAY(���궨��DHCP������Ϣ)��ֵ
 	optval:ָ��ѡ�������ֵ,������ַ�������������"\0"�������ַ��������򳤶� Ϊ0
 �������: 
 	��  
 ����ֵ: 
    ���óɹ���CSDHCP_SUCCESS
	����ʧ�ܣ�CSDHCP _FAILURE
	
 ��ע: 
 	��
*************************************************************************/
int CSDHCPSetOption( char * pcEthName,  int opt, void *optval);


/*************************************************************************
 ����: CSDHCPGetOption 
 ��������: 
 	��ȡDHCPѡ���ֵ  
 �������: 
 	pchNetifName: �����豸��,��"eth0"
 	opt:����ȡ��dhcp�ͻ���ѡ��	
 �������: 
 	optval:��������ѡ�������ֵ����δ���ã������ΪĬ��ֵ
 ����ֵ: 
    ��ȡ�ɹ���CSDHCP_SUCCESS
	��ȡʧ�ܣ�CSDHCP _FAILURE
	
 ��ע: 
 	None
*************************************************************************/
int CSDHCPGetOption( char * pcEthName,  int opt, void *optval);

/*************************************************************************
 ����: CSDHCPStart 
 ��������: 
 	����DHCP��CSDHCPInit֮�����CSDHCPStop����(����)����dhcp,�ɵ��øýӿ�
 �������: 
 	��
 �������: 
 	��   
 ����ֵ: 
 	�����ɹ���CSDHCP_SUCCESS
	����ʧ�ܣ�CSDHCP _FAILURE

 ��ע: 
 	��
*************************************************************************/
int  CSDHCPStart(void);

/*************************************************************************
 ����: CSDHCPStop 
 ��������: 
 	ֹͣDHCP,����CSDHCPInit֮������Ҫ��ֹdhcp�ɵ��øýӿ�
 �������: 
 	pcEthName:�����豸��,��"eth0"
 �������: 
 	��   
 ����ֵ: 
 	ֹͣ�ɹ���CSDHCP_SUCCESS
	ֹͣʧ�ܣ�CSDHCP _FAILURE

 ��ע: 
 	��
*************************************************************************/
int CSDHCPStop(char * pcEthName);


/*************************************************************************
 ����: CSDHCPSetSerName 
 ��������: 
 	���÷������������ֶ�.���ֶ�����"\0"Ϊ���������ַ������������ã���Ҫ��
 	CSDHCPStart֮ǰ����
 �������: 
 	pucSerName: �û����õķ������������ֶε�ֵ�����Ȳ�����64�ֽ�
 	dwLen: �û����õķ������������ֶεĳ���
 �������: 
 	��  
 ����ֵ: 
 	���óɹ���CSDHCP_SUCCESS
	����ʧ�ܣ�CSDHCP _FAILURE
 ��ע: 
 	���ֶ��ɷ�������д������û����ô��ֶΣ����ڿͻ����յ�DHCPOFFERʱ�����
 	�ϴ��ֶΣ�������ϲŻ���ܣ����򲻽���.
*************************************************************************/
int CSDHCPSetSerName(BYTE * pucSerName, DWORD dwLen );


/*************************************************************************
 ����: CSDHCPGetSerName 
 ��������: 
 	��ȡ��������Ӧ����еķ������������ֶΡ�
 �������: 
 	pucSerName: �����û���ȡ�ķ������������ֶεĵ�ַ����ռ����û�����
 	dwLen: �û�Ҫ��ȡ�ķ������������ֶεĳ��ȣ�������64
 �������: 
 	pucSerName :  �����û���ȡ�ķ������������ֶεĵ�ַ
 ����ֵ: 
 	��ȡ�ɹ���CSDHCP_SUCCESS
	��ȡʧ�ܣ�CSDHCP _FAILURE
 ��ע: 
 	�ͻ����յ�Ӧ����󣬽������еķ������������ֶΣ��û�����ͨ�����ô˽ӿ�
 	�õ����ֶε�ֵ��
*************************************************************************/
int CSDHCPGetSerName(BYTE *pucSerName,DWORD dwLen );



/*************************************************************************
 ����: CSDHCPClearSerName 
 ��������: 
 	���servername�ֶ�
 �������: 
        ��
�������: 
        ��
 ����ֵ: 
 	��ȡ�ɹ���CSDHCP_SUCCESS
	��ȡʧ�ܣ�CSDHCP _FAILURE
 ��ע: 

���֮ǰ���õ�servername��ʹ���ͳ�ȥ�İ�������servername�ֶ�
 
*************************************************************************/
int CSDHCPClearSerName(void);



/*************************************************************************
 ����: CSDHCPClearOption 
 ��������: 
 	�������õ�ѡ�����,ʹ��ѡ��ָ���Ĭ��ֵ
 �������: 
 	opt: ��Ҫ�����ѡ��
 �������: 
 	��
 ����ֵ: 
 	��ȡ�ɹ���CSDHCP_SUCCESS
	��ȡʧ�ܣ�CSDHCP _FAILURE
 ��ע: 
*************************************************************************/

int CSDHCPClearOption( int opt);

/*************************************************************************
 ����: CSDhcpcGetVersion
 ��������:
 	��ȡdhcpģ��İ汾��Ϣ
 �������: 	
 	nSize: ����汾�ŵ�buffer��С.
 �������: 	
 	pcVer: ����汾�ŵ�buffer�׵�ַ
 ����ֵ: 
	<1>��� [pcVer] �� NULL,���ذ汾��Ϣ��ʵ�ʳ��ȣ����� '\0'.
	<2>��� [pcVer] ��Ϊ NULL,�ҵ��óɹ�,�򷵻ذ汾��Ϣ�ĳ��ȡ�
	 	return the length of version information returned.
	<3>�������ʧ�ܣ�����0.	
	
 ��ע: 

*************************************************************************/
int CSDHCPGetVersion( char *pchVer, int nSize );



/*************************************************************************
 ����: CSDHCPAddCallback
 ��������:
 	���һ��dhcp�ص�
 �������: 	
 	dwUserData: �����û�������Ϣ
 	pCallback:  Ҫ��ӵĻص�
 �������: 	
 ����ֵ: 
	�ɹ�����0
	ʧ�ܣ�0
	
 ��ע: ������Ӷ���ص�  
*************************************************************************/


BOOL CSDHCPAddCallback(DWORD dwUserData, fnDhcpCallBack_p pCallback);


/*************************************************************************
 ����: CSDHCPRemoveCallback
 ��������:
 	�Ƴ�һ���ص�
 �������: 	
 	dwUserData: �û�������Ϣ
 	pCallback:  Ҫ�Ƴ��Ļص�
 �������: 	
 ����ֵ: 
	�ɹ�����0
	ʧ�ܣ�0
	
 ��ע: 

*************************************************************************/

BOOL CSDHCPRemoveCallback(DWORD dwUserData, fnDhcpCallBack_p pCallback);

/*************************************************************************
 ����: CSRegisterDhcpGetSTBID
 ��������:
 	ע���ȡ������Ψһ��ʶ���Ļص�����
 �������: 	
 	fnNotify: ���û�ʵ�֣���ȡ�����е�Ψһ��ʶ��
 �������: 	��
 ����ֵ: 
	��
	
 ��ע: 

*************************************************************************/

void CSRegisterDhcpGetSTBID(fnGetIdentify_F fnNotify);



/*************************************************************************
 ����: CSDHCPSetTmpIp
 ��������:
 	��cm���ߵ�ʱ��ֹͣdhcp������Ϊ����������һ����ʱ��ip��ַ
 �������: 	
 	ethName: �����豸����
 	nLen:�����豸������
 �������: 	��
 ����ֵ: 
	��	
 ��ע: 

*************************************************************************/

void  CSDHCPSetTmpIp(char *pchEthName,int nLen);


/*************************************************************************
 ����: CSDHCPSetOptionMode
 ��������:
 	����option60ѡ�����֤��ʽ
 �������: 	
 	eCheckMode: ��֤��ʽ����eCheckModeΪEM_DHCPOPT_CHECKʱ�����ϸ���֤server classid��ֵ����eCheckModeΪEM_DHCPOPT_NOCHECK
 ʱ������Ҫ��֤��ѡ�ֱ�����¸����ݰ���Ŀǰֻ�������ͱ�����Ŀ����EM_DHCPOPT_NOCHECK��ʽ����������Ŀ������EM_DHCPOPT_CHECK��ʽ
 �������: 	��
 ����ֵ: 
	��	
 ��ע: 

*************************************************************************/

void CSDHCPSetOptionMode(CSDHCPOptCheckMode_E eCheckMode );



/*************************************************************************
 ����: CSDHCPGetOptionMode
 ��������:
 	��ȡoption60ѡ�����֤��ʽ
 �������: 	
     ��
 �������: 	��
 ����ֵ: 
	����EM_DHCPOPT_CHECK����EM_DHCPOPT_NOCHECK������֤��ʽ
 ��ע: 

*************************************************************************/

CSDHCPOptCheckMode_E  CSDHCPGetOptionMode();

/*************************************************************************
 ����: CSDHCPSetBroadcastMode
 ��������:
 	����DHCP��������Ӧ������㲥��
 �������: 	
   eBroadcastResp:Ҫ��������ط�������㲥��
 �������: 	��
 ����ֵ: 
	
 ��ע: ϵͳĬ��Ϊ�㲥��

*************************************************************************/

void CSDHCPSetBroadcastMode(CSDHCPCType_E eBroadcastResp);

#if defined(__cplusplus) || defined(__CPLUSPLUS)
}
#endif

#endif 

