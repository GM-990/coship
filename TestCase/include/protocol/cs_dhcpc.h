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

#define CSDHCP_SUCCESS   0           /*定义返回值：成功返回CSDHCP_SUCCESS */
#define CSDHCP_FAILURE  -1           /*定义返回值：失败返回CSDHCP _FAILURE */
#define DHCP_MAX_CALLBACK_COUNT   (20)
typedef void  (*DhcpCallBackFun_p)( int event, unsigned int incoming, DhcpClientInfo *client);
/*添加多回调处理函数，以后调用CSDHCPAddCallback将传递下面这种类型的callback*/
typedef void  (*fnDhcpCallBack_p)( int event, unsigned int incoming, DhcpClientInfo *client,DWORD dwUserData);
/* 添加注册获取机顶盒唯一标识符函数*/
typedef unsigned int(*fnGetIdentify_F)();

typedef enum
{
    EM_DHCPC_RENEWTYPE_UNICAST = 0,//单播
    EM_DHCPC_RENEWTYPE_BROADCAST,//广播
    EM_DHCPC_RENEWTYPE_UNDEFINED
} CSDHCPCRenwType_E;

typedef enum
{
    EM_DHCPC_UNICAST = 0,//单播
    EM_DHCPC_BROADCAST,//广播
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
	EM_DHCPOPT_CHECK = 0,/*在设置opt60的情况下，客户端需要严格验证server classid的值，验证通过才能收下该数据包*/
	EM_DHCPOPT_NOCHECK   /*在设置opt60的情况下，收到的包如果不带有server classid,客户端可以直接收下*/
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
 函数: DhcpcStart 
 功能描述: 
 	启动dhcp
 输入参数: 
 	pchNetifName: 网络设备名,如"eth0"
 输出参数: 
 	无 
 返回值: 
 	-26 invalid parameter
 	-41 no such file or directory
 	0     success	
 备注: 
 	无
*************************************************************************/
int DhcpcStart( char* pchNetifName);

/*************************************************************************
 函数: DhcpcRelease 
 功能描述: 
 	释放之前dhcp获取到的IP地址  
 输入参数: 
 	pchNetifName: 网络设备名,如"eth0"
 输出参数: 
 	无  
 返回值: 
 	-26 invalid parameter
 	-39 out of resources
 	0     success	
 备注: 
 	无
*************************************************************************/
int DhcpcRelease( char* pchNetifName);
/*************************************************************************
 函数: DhcpcSetOption 
 功能描述: 
 	配置DHCP客户端选项 ， 
 输入参数: 
 	pchNetifName: 网络设备名,如"eth0"
 	opt:被设置的dhcp客户端选项,一般需要设置的参数有：DHCPCO_CALLBACK?
 	    珼HCPCO_SET_RESOLVER，DHCPCO_SET_GATEWAY(见宏定义DHCP配置信息)等值
 	optval:指向选项的设置值
 输出参数: 
 	无  
 返回值: 
 	-26 invalid parameter
 	-41 no such file or directory
 	0     success
 	
 备注: 
*************************************************************************/
int DhcpcSetOption( char* pchNetifName,  int opt, void* optval);


/*************************************************************************
 函数: DhcpcGetOption 
 功能描述: 
 	获取DHCP选项的值  
 输入参数: 
 	pchNetifName: 网络设备名,如"eth0"
 	opt:欲获取的dhcp客户端选项	
 输出参数: 
 	optval:用来保存选项的设置值，若未设置，则输出为默认值
 返回值: 
 	-26 invalid parameter
 	-41 no such file or directory
 	0     success
 备注: 
 	None
*************************************************************************/
int DhcpcGetOption( char* pchNetifName,  int opt, void* optval);


/*************************************************************************
 函数: CSDhcpcSetOptClassID 
 功能描述: 
 	设置OPT60(Classid)的值
 输入参数: 
 	pchNetifName: 网络设备名,如"eth0"
 	pcClientClassID:客户端的classid
 	pcServerClassID:服务器端的classid
 输出参数: 
 	无  
 返回值: 
 	无
 备注: 
*************************************************************************/

int CSDhcpcSetOptClassID(
	const  char* pcNetIfName, 
	const  char* pcClientClassID, 
	const  char* pcServerClassID );

/*************************************************************************
 函数: DhcpcSendInform 
 功能描述: 
 	客户端广播一个DHCPINFORM包，借此获得更多的配置选项值
 Input: 
 	pchNetifName: 网络设备名,如"eth0"
 	saddr:DHCP服务器的IP地址
 	req_opt:Dhcpoption 结构体的地址
 Output: 
 	无  
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
 	挂起DHCP线程
 Input: 
 	无
 Output: 
 	无  
 Return: 
 	无
 Note: 
 	无
*************************************************************************/
void CSSuspendSendMsgThread(void);
/*************************************************************************
 Function: CSSuspendSendMsgThread 
 Description: 
 	恢复DHCP线程
 Input: 
 	无
 Output: 
 	无  
 Return: 
 	无
 Note: 
 	无
*************************************************************************/
void CSResumeSendMsgThread(void);


/*************************************************************************
 函数: CSDHCPInit 
 功能描述: 
 	初始化DHCP模块中的全局变量 ,首先被调用
 输入参数: 
 	pchNetifName: 网络设备名,如"eth0"
 输出参数: 
 	无 
 返回值: 
    启动成功：CSDHCP_SUCCESS
    启动失败：CSDHCP _FAILURE
	
 备注: 
 	无
*************************************************************************/
int CSDHCPInit( char * pcEthName);


/*************************************************************************
 函数: CSDHCPSetRenewUnicastMode 
 功能描述: 
 	设置dhcp在renew状态时发送广播包还是单播包，如要设置为单播
 	包，则须在dhcp启动前调用此接口。
 输入参数: 
 	eRenewType: 见CSDHCPCRenwType_E  不设置默认为发送广播包
 输出参数: 
 	无 
 返回值: 
    设置成功：CSDHCP_SUCCESS
    设置失败：CSDHCP _FAILURE
	
 备注: 
 	无
*************************************************************************/
int CSDHCPSetRenewUnicastMode(CSDHCPCRenwType_E eRenewType);

/*************************************************************************
 函数: CSDHCPSetThreeFourthsUnicast 
 功能描述: 
 	设置dhcp在四分之三租期的时候发送单播包到服务器
 输入参数: 
 	bMode: 设置为TRUE则表示在dhcp 1/2不成功的时候，继续在四分
 	之三租期发送一个单播包到服务器，为FALSE则不需要在此
 	时刻发送单播，
	默认不发送，符合dhcp标准规范
 
 输出参数: 
 	无 
 返回值: 
    设置成功：CSDHCP_SUCCESS
    设置失败：CSDHCP _FAILURE
	
 备注: 
 	无
*************************************************************************/
int CSDHCPSetThreeFourthsUnicast(BOOL bMode);

/*************************************************************************
 函数: CSDHCPCleanup 
 功能描述: 
 	释放由CSDHCPInit分配的资源，不建议使用该函数 
 输入参数: 
 	无
 输出参数: 
 	无  
 返回值: 
    销毁成功：CSDHCP_SUCCESS
    销毁失败：CSDHCP _FAILURE
	
 备注: 
 	None
*************************************************************************/

int CSDHCPCleanup(void);

/*************************************************************************
 函数: CSDHCPSetOption 
 功能描述: 
 	配置DHCP客户端选项 ， 
 输入参数: 
 	pchNetifName: 网络设备名,如"eth0"
 	opt:被设置的dhcp客户端选项,一般需要设置的参数有：DHCPCO_CALLBACK?
 	    珼HCPCO_SET_RESOLVER，DHCPCO_SET_GATEWAY(见宏定义DHCP配置信息)等值
 	optval:指向选项的设置值,如果是字符串，必须是以"\0"结束的字符串，否则长度 为0
 输出参数: 
 	无  
 返回值: 
    设置成功：CSDHCP_SUCCESS
	设置失败：CSDHCP _FAILURE
	
 备注: 
 	无
*************************************************************************/
int CSDHCPSetOption( char * pcEthName,  int opt, void *optval);


/*************************************************************************
 函数: CSDHCPGetOption 
 功能描述: 
 	获取DHCP选项的值  
 输入参数: 
 	pchNetifName: 网络设备名,如"eth0"
 	opt:欲获取的dhcp客户端选项	
 输出参数: 
 	optval:用来保存选项的设置值，若未设置，则输出为默认值
 返回值: 
    获取成功：CSDHCP_SUCCESS
	获取失败：CSDHCP _FAILURE
	
 备注: 
 	None
*************************************************************************/
int CSDHCPGetOption( char * pcEthName,  int opt, void *optval);

/*************************************************************************
 函数: CSDHCPStart 
 功能描述: 
 	启动DHCP，CSDHCPInit之后或者CSDHCPStop后想(继续)运行dhcp,可调用该接口
 输入参数: 
 	无
 输出参数: 
 	无   
 返回值: 
 	启动成功：CSDHCP_SUCCESS
	启动失败：CSDHCP _FAILURE

 备注: 
 	无
*************************************************************************/
int  CSDHCPStart(void);

/*************************************************************************
 函数: CSDHCPStop 
 功能描述: 
 	停止DHCP,调用CSDHCPInit之后，如需要中止dhcp可调用该接口
 输入参数: 
 	pcEthName:网络设备名,如"eth0"
 输出参数: 
 	无   
 返回值: 
 	停止成功：CSDHCP_SUCCESS
	停止失败：CSDHCP _FAILURE

 备注: 
 	无
*************************************************************************/
int CSDHCPStop(char * pcEthName);


/*************************************************************************
 函数: CSDHCPSetSerName 
 功能描述: 
 	设置服务器主机名字段.此字段是以"\0"为结束符的字符串，如需设置，需要在
 	CSDHCPStart之前调用
 输入参数: 
 	pucSerName: 用户设置的服务器主机名字段的值，长度不超过64字节
 	dwLen: 用户设置的服务器主机名字段的长度
 输出参数: 
 	无  
 返回值: 
 	设置成功：CSDHCP_SUCCESS
	设置失败：CSDHCP _FAILURE
 备注: 
 	本字段由服务器填写。如果用户设置此字段，则在客户端收到DHCPOFFER时，会比
 	较此字段，如果符合才会接受，否则不接受.
*************************************************************************/
int CSDHCPSetSerName(BYTE * pucSerName, DWORD dwLen );


/*************************************************************************
 函数: CSDHCPGetSerName 
 功能描述: 
 	获取服务器端应答包中的服务器主机名字段。
 输入参数: 
 	pucSerName: 保存用户获取的服务器主机名字段的地址，其空间由用户申请
 	dwLen: 用户要获取的服务器主机名字段的长度，不超过64
 输出参数: 
 	pucSerName :  保存用户获取的服务器主机名字段的地址
 返回值: 
 	获取成功：CSDHCP_SUCCESS
	获取失败：CSDHCP _FAILURE
 备注: 
 	客户端收到应答包后，解析其中的服务器主机名字段，用户可以通过调用此接口
 	得到该字段的值。
*************************************************************************/
int CSDHCPGetSerName(BYTE *pucSerName,DWORD dwLen );



/*************************************************************************
 函数: CSDHCPClearSerName 
 功能描述: 
 	清除servername字段
 输入参数: 
        无
输出参数: 
        无
 返回值: 
 	获取成功：CSDHCP_SUCCESS
	获取失败：CSDHCP _FAILURE
 备注: 

清除之前设置的servername，使发送出去的包不带有servername字段
 
*************************************************************************/
int CSDHCPClearSerName(void);



/*************************************************************************
 函数: CSDHCPClearOption 
 功能描述: 
 	将所设置的选项清除,使得选项恢复到默认值
 输入参数: 
 	opt: 将要清除的选项
 输出参数: 
 	无
 返回值: 
 	获取成功：CSDHCP_SUCCESS
	获取失败：CSDHCP _FAILURE
 备注: 
*************************************************************************/

int CSDHCPClearOption( int opt);

/*************************************************************************
 函数: CSDhcpcGetVersion
 功能描述:
 	获取dhcp模块的版本信息
 输入参数: 	
 	nSize: 保存版本号的buffer大小.
 输出参数: 	
 	pcVer: 保存版本号的buffer首地址
 返回值: 
	<1>如果 [pcVer] 是 NULL,返回版本信息的实际长度，包括 '\0'.
	<2>如果 [pcVer] 不为 NULL,且调用成功,则返回版本信息的长度。
	 	return the length of version information returned.
	<3>如果调用失败，返回0.	
	
 备注: 

*************************************************************************/
int CSDHCPGetVersion( char *pchVer, int nSize );



/*************************************************************************
 函数: CSDHCPAddCallback
 功能描述:
 	添加一个dhcp回调
 输入参数: 	
 	dwUserData: 保存用户数据信息
 	pCallback:  要添加的回调
 输出参数: 	
 返回值: 
	成功：非0
	失败：0
	
 备注: 可以添加多个回调  
*************************************************************************/


BOOL CSDHCPAddCallback(DWORD dwUserData, fnDhcpCallBack_p pCallback);


/*************************************************************************
 函数: CSDHCPRemoveCallback
 功能描述:
 	移除一个回调
 输入参数: 	
 	dwUserData: 用户数据信息
 	pCallback:  要移除的回调
 输出参数: 	
 返回值: 
	成功：非0
	失败：0
	
 备注: 

*************************************************************************/

BOOL CSDHCPRemoveCallback(DWORD dwUserData, fnDhcpCallBack_p pCallback);

/*************************************************************************
 函数: CSRegisterDhcpGetSTBID
 功能描述:
 	注册获取机顶盒唯一标识符的回调函数
 输入参数: 	
 	fnNotify: 由用户实现，获取机顶盒的唯一标识符
 输出参数: 	无
 返回值: 
	无
	
 备注: 

*************************************************************************/

void CSRegisterDhcpGetSTBID(fnGetIdentify_F fnNotify);



/*************************************************************************
 函数: CSDHCPSetTmpIp
 功能描述:
 	当cm掉线的时候，停止dhcp，并且为机顶盒设置一个临时的ip地址
 输入参数: 	
 	ethName: 网卡设备名称
 	nLen:网卡设备名长度
 输出参数: 	无
 返回值: 
	无	
 备注: 

*************************************************************************/

void  CSDHCPSetTmpIp(char *pchEthName,int nLen);


/*************************************************************************
 函数: CSDHCPSetOptionMode
 功能描述:
 	设置option60选项的验证方式
 输入参数: 	
 	eCheckMode: 验证方式，当eCheckMode为EM_DHCPOPT_CHECK时，会严格验证server classid的值，当eCheckMode为EM_DHCPOPT_NOCHECK
 时，不需要验证该选项，直接收下该数据包，目前只有天威和宝龙项目采用EM_DHCPOPT_NOCHECK方式，其他的项目均采用EM_DHCPOPT_CHECK方式
 输出参数: 	无
 返回值: 
	无	
 备注: 

*************************************************************************/

void CSDHCPSetOptionMode(CSDHCPOptCheckMode_E eCheckMode );



/*************************************************************************
 函数: CSDHCPGetOptionMode
 功能描述:
 	获取option60选项的验证方式
 输入参数: 	
     无
 输出参数: 	无
 返回值: 
	返回EM_DHCPOPT_CHECK或者EM_DHCPOPT_NOCHECK两种验证方式
 备注: 

*************************************************************************/

CSDHCPOptCheckMode_E  CSDHCPGetOptionMode();

/*************************************************************************
 函数: CSDHCPSetBroadcastMode
 功能描述:
 	设置DHCP服务器回应单播或广播包
 输入参数: 	
   eBroadcastResp:要求服务器回发单播或广播包
 输出参数: 	无
 返回值: 
	
 备注: 系统默认为广播包

*************************************************************************/

void CSDHCPSetBroadcastMode(CSDHCPCType_E eBroadcastResp);

#if defined(__cplusplus) || defined(__CPLUSPLUS)
}
#endif

#endif 

