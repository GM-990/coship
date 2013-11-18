/*cs_tcpip.h*/



#ifndef _CS_TCPIP_H_
#define _CS_TCPIP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

#define  CS_FD_MAX 32

#ifndef  SOCKET
#define  SOCKET  int
#endif

#define INVALID_SOCKET    -1/*should be different according platform*/
#define SOCKET_ERROR      -1/*should be different according platform*/

#if defined (PLATFORM_ST40)|| defined (PLATFORM_ST20) || defined (PLATFORM_ZORAN)//(����ֻ���NG_STACK��Ч)
/* standart types */
typedef char *caddr_tt;


/* Protocol configuration options */

#ifndef TCP_NODELAY
#define TCP_NODELAY         0x0001
#endif

#ifndef _IOCTLSOCKET_
#define _IOCTLSOCKET_

#define NG_SELECT_FD_MAX    ( CS_FD_MAX/2) /*NG_SELECT_FD_MAX i.e. Winsock's FD_SETSIZE*/
/* commands for fcntlsocket() */
#define F_GETFL         3     /* get flags */
#define F_SETFL         4     /* set flags */
#if defined (PLATFORM_ST40) || defined (PLATFORM_ST20)
#include <sys/fcntl.h>
#endif

#ifndef O_NONBLOCK
#define O_NONBLOCK      0x0004 
#endif

/* commands for ioctlsocket() */
#define IOC_OUT         0x40000000L     /* copy out parameters */
#define IOC_IN          0x80000000L     /* copy in parameters */
#define IOCPARM_MASK    0x1fff          /* parameter length, at most 13 bits */
#define _IOC(inout,group,num,len) \
        (inout | (((long)len & IOCPARM_MASK) << 16) | ((group) << 8) | (num))
#define _IOR(g,n,t)     _IOC(IOC_OUT,   (g), (n), sizeof(t))
#define _IOW(g,n,t)     _IOC(IOC_IN,    (g), (n), sizeof(t))

#define FIONBIO         _IOW('f', 126, int)     /* set/clear nonblocking io */
#define FIONREAD        _IOR('f', 127, int)     /* get # bytes to read */
#define FIONWRITE       _IOR('f', 128, int)     /* get # bytes to write */
#define SIOCATMARK      _IOR('s',  7, int)      /* at oob mark? */

#endif


/* protocols */
#define IPPROTO_IP          0
#define IPPROTO_HOPOPTS     0
#define IPPROTO_ICMP        1
#define IPPROTO_IGMP        2
#define IPPROTO_GGP         3
#define IPPROTO_IPV4        4
#define IPPROTO_IPIP        4
#define IPPROTO_TCP         6
#define IPPROTO_EGP         8
#define IPPROTO_PUP         12
#define IPPROTO_UDP         17
#define IPPROTO_IDP         22
#define IPPROTO_TP          29
#define IPPROTO_IPV6        41
#define IPPROTO_ROUTING     43
#define IPPROTO_FRAGMENT    44
#define IPPROTO_RSVP        46
#define IPPROTO_GRE         47
#define IPPROTO_ESP         50
#define IPPROTO_AH          51
#define IPPROTO_EON         80
#define IPPROTO_MOBILE      55
#define IPPROTO_IPV6_ICMP   58
#define IPPROTO_ICMPV6      58
#define IPPROTO_NONE        59
#define IPPROTO_DSTOPTS     60
#define IPPROTO_ENCAP       98
#define IPPROTO_PIM         103
#define IPPROTO_IPCOMP      108
#define IPPROTO_VRRP        112
#define IPPROTO_RAW         255
#define IPPROTO_MAX         256

/* udp and tcp reserved ports */
#define IPPORT_RESERVED         1024
#define IPPORT_USERRESERVED     5000

#define IPPORT_ECHO             7
#define IPPORT_DISCARD          9
#define IPPORT_SYSTAT           11
#define IPPORT_DAYTIME          13
#define IPPORT_NETSTAT          15
#define IPPORT_FTP              21
#define IPPORT_TELNET           23
#define IPPORT_SMTP             25
#define IPPORT_TIMESERVER       37
#define IPPORT_NAMESERVER       42
#define IPPORT_WHOIS            43
#define IPPORT_MTP              57
#define IPPORT_BOOTP            67
#define IPPORT_BOOTPC           68
#define IPPORT_TFTP             69
#define IPPORT_RJE              77
#define IPPORT_FINGER           79
#define IPPORT_TTYLINK          87
#define IPPORT_SUPDUP           95

/*
 * Public
 */

/*
 * BSD compatible names
 * this should be in cs_tcpip.h
 */

/* address family */
#define  AF_UNSPEC        0       /* unspecified */
#define  AF_INET          2       /* internetwork: UDP, TCP, etc. */

/* protocol family (== address family) */
#define  PF_UNSPEC           AF_UNSPEC
#define  PF_INET             AF_INET

/* type of socket */
#define  SOCK_STREAM      1
#define  SOCK_DGRAM       2
#define  SOCK_RAW         3

/* level of option */
#define  SOL_SOCKET          0xffffU
#define  SOL_IP              IPPROTO_IP 
#define  SOL_TCP             IPPROTO_TCP
#define  SOL_UDP             IPPROTO_UDP

/* set/get operation */
#define  SO_SET           0x8000U   /* set option */
#define  SO_GET           0x0000U   /* get option */

/* socket level options */
#define  SO_DEBUG          0x0001  /* turn on debugging info recording */
#define  SO_ACCEPTCONN     0x0002  /* socket has had listen() */
#define  SO_REUSEADDR      0x0004  /* allow local address reuse */
#define  SO_KEEPALIVE      0x0008  /* keep connections alive */
#define  SO_DONTROUTE      0x0010  /* just use interface addresses */
#define  SO_BROADCAST      0x0020  /* permit sending of broadcast msgs */
#define  SO_RECVDSTADDR    0x0040  /* receive dest addr in control data */
#define  SO_LINGER         0x0080  /* linger on close if data present */
#define  SO_OOBINLINE      0x0100  /* leave received OOB data in line */
#define  SO_REUSEPORT      0x0200  /* allow local address & port reuse */
#define  SO_HDRINCL        0x1000  /* include IP header (raw sockets) */
#define  SO_MCASTLOOP      0x2000  /* enable loopback of outgoing mcasts */
#define  SO_DONTFRAG       0x4000  /* set DF flag on outgoing datagrams */


/* socket level options */
#define  SO_SNDBUF        0x1001    /* send buffer size */
#define  SO_RCVBUF        0x1002    /* receive buffer size */
#define  SO_SNDLOWAT      0x1003    /* send low-water mark */
#define  SO_RCVLOWAT      0x1004    /* receive low-water mark */
#define  SO_SNDTIMEO      0x1005    /* send timeout */
#define  SO_RCVTIMEO      0x1006    /* receive timeout */
#define  SO_ERROR         0x1007    /* get error status and clear (ro) */
#define  SO_TYPE          0x1008    /* get socket type (ro) */
#define  SO_MERGE       0x1009    /*udp zubao*//*chenlin add 07.07.26*/
/* CS Sock specific options */
#define  SO_NBIO          0x1f01    /* set nonblocking mode (CS_SS_NBIO flag) */
#define  SO_NREAD         0x1f02    /* nb of bytes in rcv buffer (ro) */
#define  SO_ATMARK        0x1f03    /* returns SS_RCVATMARK flag (ro) */
#define  SO_SOCKNAME      0x1f04    /* get socket name (ro) */
#define  SO_PEERNAME      0x1f05    /* get peer name (ro) */
#define  SO_NWRITE        0x1f06    /* nb of free bytes in snd buffer (ro) */

/* IP level options */
#define  IP_HDRINCL             0x0002  /* header is included with data (raw) */
#define  IP_TOS                 0x0003  /* IP type of service and precedence */
#define  IP_TTL                 0x0004  /* IP time to live */
#define  IP_MULTICAST_IF        0x0005  /* Interface for outgoing multicasts */
#define  IP_MULTICAST_TTL       0x0006  /* time to live for outgoing multicasts */
#define  IP_MULTICAST_IFP       0x0007  /* interface for outgoing multicasts */
#define  IP_ADD_MEMBERSHIP      0x0008  /* add a mcast group membership */
#define  IP_ADD_MEMBERSHIP_IFP  0x0009
#define  IP_DROP_MEMBERSHIP     0x000a  /* drop a mcast group membership */
#define  IP_DROP_MEMBERSHIP_IFP 0x000b
#define  IP_MULTICAST_LOOP      0x000c  /* enable loopback of outgoing mcasts */
#define  IP_DONTFRAG            0x000d  /* set DF bit on outgoing datagrams */
#define  IP_RECVDSTADDR         0x000e  /* recv dest address in control data */

/* flags for recv/send operations */
#define  IO_DONTROUTE     0x0010    /* bypass routing table */
#define  IO_EOR           0x0020    /* end of record */
#define  IO_TRUNC         0x0040    /* rcvd data truncated */
#define  IO_CTRUNC        0x0080    /* rcvd control data truncated */
#define  IO_BCAST         0x0100    /* rcvd link-layer bcast */
#define  IO_MCAST         0x0200    /* rcvd link-layer mcast */
#define  IO_WORD          0x0400    /* DSPs: use word transferts */

/* recv/send flags */
#define MSG_OOB             0x0004
#define MSG_PEEK            0x0008

#define MSG_DONTROUTE       0x0010    /* bypass routing table */
#define MSG_EOR             0x0020    /* end of record */
#define MSG_TRUNC           0x0040    /* rcvd data truncated */
#define MSG_CTRUNC          0x0080    /* rcvd control data truncated */
#define MSG_BCAST           0x0100    /* rcvd link-layer bcast */
#define MSG_MCAST           0x0200    /* rcvd link-layer mcast */
#define MSG_WORD            0x0400    /* DSPs: use word transferts */

/* Definitions of bits in internet address */
/* (!) addresses are in host-byte order (!) */

#define IN_CLASSA(i)            (((unsigned int)(i) & 0x80000000) == 0)
#define IN_CLASSA_NET           0xff000000
#define IN_CLASSA_NSHIFT        24
#define IN_CLASSA_HOST          0x00ffffff
#define IN_CLASSA_MAX           128

#define IN_CLASSB(i)            (((unsigned int)(i) & 0xc0000000) == 0x80000000)
#define IN_CLASSB_NET           0xffff0000
#define IN_CLASSB_NSHIFT        16
#define IN_CLASSB_HOST          0x0000ffff
#define IN_CLASSB_MAX           65536

#define IN_CLASSC(i)            (((unsigned int)(i) & 0xe0000000) == 0xc0000000)
#define IN_CLASSC_NET           0xffffff00
#define IN_CLASSC_NSHIFT        8
#define IN_CLASSC_HOST          0x000000ff

#define IN_CLASSD(i)            (((unsigned int)(i) & 0xf0000000) == 0xe0000000)
#define IN_MULTICAST(i)         IN_CLASSD(i)
#define IN_MULTICAST_H(i)       (((unsigned int)(i) & 0xf0) == 0xe0)

#define IN_EXPERIMENTAL(i)      (((unsigned int)(i) & 0xe0000000) == 0xe0000000)
#define IN_BADCLASS(i)          (((unsigned int)(i) & 0xf0000000) == 0xf0000000)

#define INADDR_ANY              (unsigned int)0x00000000
#define INADDR_LOOPBACK         0x7f000001
#define INADDR_BROADCAST        (unsigned int)0xffffffff
#define INADDR_NONE             0xffffffff

#define INADDR_UNSPEC_GROUP     0xe0000000
#define INADDR_ALLHOSTS_GROUP   0xe0000001
#define INADDR_MAX_LOCAL_GROUP  0xe00000ff




/* generic socket address */
struct sockaddr 
{
    unsigned char     sa_len;         /* total length */
    unsigned char     sa_family;      /* address family */
    unsigned char     sa_data[6];     /* address value */
};

/* socket address in internet domain */
struct in_addr 
{
    unsigned int      s_addr;         /* 32 bits IP address, net byte order */
};

struct sockaddr_in 
{
    unsigned char     sin_len;        /* total length (16) */
    unsigned char     sin_family;     /* AF_INET */
    unsigned short    sin_port;       /* 16 bits port number, net byte order */
    struct in_addr    sin_addr;       /* 32 bits IP address */
};

/* structure used by readv/writev/recvmsg/writemsg */
struct iovec 
{
    void   *iov_base;
    int     iov_len;
};

/* used by SO_SNDTIMEO/SO_RCVTIMEO socket options */
#if defined (PLATFORM_ST40) 

#include <sys/time.h>

#else
#ifdef PLATFORM_ST20
#include <sys/timeval.h>
#else
struct timeval 
{
    unsigned long    tv_sec;
    unsigned long    tv_usec;
};
#endif
#endif

/* used by SO_LINGER socket option */
struct linger 
{
    int     l_onoff;
    int     l_linger;
};

/* used by IP_ADD_MEMBERSHIP/IP_DROP_MEMBERSHIP socket options */
struct ip_mreq 
{
    struct in_addr  imr_multiaddr;  /* multicast group address */
    struct in_addr  imr_interface;  /* local interface address */
};
  
/* message structure for sendmsg/recvmsg */
struct msghdr 
{
    caddr_tt      msg_name;       /* optional address */
    unsigned int  msg_namelen;    /* size of address */
    struct iovec *msg_iov;        /* scatter/gather array */
    unsigned int  msg_iovlen;     /* # elements in msg_iov */
    caddr_tt      msg_control;    /* ancillary data */
    unsigned int  msg_controllen; /* ancillary data buffer len */
    int           msg_flags;      /* flags on received message */
};

struct cmsghdr 
{
    unsigned int cmsg_len;       /* data byte count, including hdr */
    int          cmsg_level;     /* originating protocol */
    int          cmsg_type;      /* protocol-specific type */
};

struct hostent 
{
    char   *h_name;         /* host name */
    char  **h_aliases;      /* list of aliases */
    int     h_addrtype;     /* type of address (always AF_INET) */
    int     h_length;       /* length of address */
    char  **h_addr_list;    /* list of addresses */
};

struct protoent 
{
    char   *p_name;       /* official name of protocol */
    char  **p_aliases;    /* alias list (not implemented) */
    int     p_proto;      /* protocol number */
};

struct  servent 
{
    char   *s_name;       /* official name of service */
    char  **s_aliases;    /* alias list */
    int     s_port;       /* port service resides at */
    char   *s_proto;      /* protocol to use */
};


/* definitions for selectsocket() */
#ifndef fd_set
typedef struct {
    unsigned long fds[( CS_FD_MAX+31)/32];
} ng_fd_set;
#define fd_set ng_fd_set

#define FD_ZERO( fdset) \
  { int i; for( i=0; i<(( CS_FD_MAX+31)/32); i++) (fdset)->fds[i] = 0; }

#define FD_SET( fd, fdset) \
  ((fdset)->fds[(fd)>>5] |= 1UL<<((fd) & 0x1f))

#define FD_CLR( fd, fdset) \
  ((fdset)->fds[(fd)>>5] &= ~(1UL<<((fd) & 0x1f)))

#define FD_ISSET( fd, fdset) \
  (((fdset)->fds[(fd)>>5] & (1UL<<((fd) & 0x1f))) != 0UL)

#ifndef FD_SETSIZE
#define FD_SETSIZE CS_FD_MAX
#endif

#endif

#ifndef socklen_t 
#define socklen_t  int
#endif

SOCKET	socket( int domain, int type, int protocol);

int bind( SOCKET s, const struct sockaddr *name, int namelen);
int	listen( SOCKET s, int backlog);
int	accept( SOCKET s, struct sockaddr *addr, socklen_t *addrlen);
int connect( SOCKET s, const struct sockaddr *name, int namelen);

int	getpeername( SOCKET s, struct sockaddr *name, socklen_t *namelen);
int	getsockname( SOCKET s, struct sockaddr *name, socklen_t *namelen);

int getsockopt( SOCKET s, int level, int optname, void *optval, socklen_t  *optlen);
int setsockopt( SOCKET s, int level, int optname, const void *optval, int optlen);
int ioctlsocket( SOCKET s, long cmd, int *arg);
int fcntlsocket( SOCKET s, int cmd, ...);

int readvsocket( SOCKET s, const struct iovec *iov, int iovcnt);

int recvfrom( SOCKET s, void *buf, int len, int flags,
                  struct sockaddr *from, socklen_t  *fromlen);

int	recvmsg( SOCKET s, struct msghdr *msg, int flags);

int selectsocket( int fdmax, fd_set *readfds, fd_set *writefds,
                        fd_set *exceptfds, struct timeval *timeout);

int sendto( SOCKET s, const void *buf, int len, int flags,
                const struct sockaddr *to, int tolen);

int sendmsg( SOCKET s, const struct msghdr *msg, int flags);
int writevsocket( SOCKET s, const struct iovec *iov, int iovcnt);

#define recv( s, buf, len, flags)   recvfrom( s, buf, len, flags, NULL, NULL)
#define readsocket( s, buf, len)   recvfrom( s, buf, len, 0, NULL, NULL)
#define send( s, buf, len, flags)   sendto( s, buf, len, flags, NULL, 0)
#define writesocket( s, buf, len)  sendto( s, buf, len, 0, NULL, 0)
    
int shutdown( SOCKET s, int how);
int closesocket( SOCKET s);

// Get DNS option through CoShip interface, update by zxh
int CSResolvGetOption( int opt, void * optval);

// Set DNS option through CoShip interface, update by zxh
int CSResolvSetOption( int opt, const void * optval);

struct hostent * gethostbyname( const char *name);

struct hostent * gethostbyaddr( const char *addr, int len, int type );

struct hostent * gethostbyname_r( const char *name, struct hostent *result,
                                        void *buf, int buflen, int *err);

struct hostent * gethostbyaddr_r( const char *addr, int len, int type,
                                       struct hostent *result, void *buf, int buflen, int *err);

int gethostname( char *name, int namelen);
int sethostname( const char *name, int namelen);

struct protoent * getprotobyname( const char *name);
struct protoent * getprotobynumber( int proto);
struct servent * getservbyname( const char *name, const char *proto);
struct servent * getservbyport( int port, const char *proto);

/* internet address manipulation routines */
unsigned long  inet_addr( const char *);
int            inet_aton(const char *, struct in_addr *);
char          *inet_ntoa( struct in_addr);

unsigned short ntohs(unsigned short val);
unsigned short htons(unsigned short val);
unsigned  ntohl(unsigned  val);
unsigned  htonl(unsigned  val);



/***********************************************************
* Function: CSNgipGetVersion
*
* Input:  nSize: The size of Version Buffer.
* Output:  pchVer: The top address of Version Buffer
* Return: 
*   1>If [pchVer] is NULL,return the actual length of
*     version information,including '\0'.
*   2>If [pchVer] is not NULL,and the call is success,
*     return the length of version information returned.
*   3>If the call is failure,return 0.   
*   
* Note: This function is used to get version information.
*  Display format is:
*  "[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
************************************************************/
int  CSNgipGetVersion( char *pchVer, int nSize );
int  CSNgosGetVersion( char *pchVer, int nSize );

//�Ͻӿ�
int  arp_sniffer(const char * ifname, const unsigned inaddr, const unsigned timeout);
int  del_arp_entry(unsigned inaddr);
void clear_arp_table(void);



/***********************************************************
* ����: CSTCPIP_EnableRespondPing
* ����: �����Ƿ�����Э��ջ��Ӧping���ֻ��NGЭ��ջ����Ч
* ����:   
*     bEnableping ����������TRUE����FALSE��
*                   ��ΪTRUEʱ����Э��ջ��Ӧping���
*                   ��ΪFALSEʱ������Э��ջ��Ӧping����
            
* ���: 
*
* ����ֵ: 
*     
************************************************************/
void CSTCPIP_EnableRespondPing(BOOL bEnableping);

/***********************************************************
* ����: CSTCPIP_IsEnableRespondPing
* ����: �鿴�Ƿ�����Э��ջ��Ӧping���ֻ��NGЭ��ջ����Ч
* ����:   
*                 
* ���: 
*
* ����ֵ: 
*       ����TRUE��FALSE��
*       TRUE������Э��ջ��Ӧping����
*     
************************************************************/
BOOL CSTCPIP_IsEnableRespondPing(void);

/*����linuxƽ̨����ƽ̨makeϵͳ�ж���PLATFORM_LINUX,�������ڴ����ƽ̨��*/
#elif defined (PLATFORM_X86) || defined(PLATFORM_LINUX) || defined(PLATFORM_BCMMIPSEL)||defined(PLATFORM_HI3110E)||defined(PLATFORM_CX24501)||defined(PLATFORM_HI3716H)||defined (PLATFORM_HI3716M)||defined(PLATFORM_PNX8473)||defined(PLATFORM_MSD7853)||defined(PLATFORM_MTK8652) ||defined(PLATFORM_ST40_LINUX)
#include <sys/select.h>
#include <sys/socket.h>
#include <fcntl.h> 
#include <errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define TCPIPDEBUG 

#define SO_REUSEPORT SO_REUSEADDR

#define fcntlsocket  fcntl


int selectsocket( int fdmax, fd_set *readfds, fd_set *writefds,
                        fd_set *exceptfds, struct timeval *timeout);

int closesocket( SOCKET s);
int ioctlsocket( SOCKET s, long cmd, int *arg) ;

#elif defined (WIN32)
//#include <winsock2.h>
#define F_GETFL         3     /* get flags */
#define F_SETFL         4     /* set flags */
#define O_NONBLOCK      0x0004       /* set/clear nonblocking io */
#ifndef socklen_t 
#define socklen_t  int
#endif

#ifdef fcntlsocket
#undef fcntlsocket
#endif

#ifdef selectsocket
#undef selectsocket
#endif

int selectsocket( int fdmax, fd_set *readfds, fd_set *writefds,
                        fd_set *exceptfds, struct timeval *timeout);

#define     CSGETHOSTBYNAME     gethostbyname

#endif


typedef enum
{
	EM_RAWSENDTYPE_UNICAST = 0,//����
	EM_RAWSENDTYPE_BROADCAST,//�㲥
	EM_RAWSENDTYPE_UNDEFINED
} CSRawSendType_E;

/*���ش�����,������ľ������������cstcpip_err.h */
int CSTCPIPGetLastError(void);


/***********************************************************
* ����: CSPing
* ����: ����ping������һ̨�����Ƿ񵽴�˺�����NGЭ��ջ��LinuxЭ��ջ�²�������������ͬ��
* ����:  
*	    pPingStr����ʾping �����У��磺"ping -n 8  192.168.110.90"��
*                 ��NGЭ��ջ�£������ν�����������
*                 ��LinuxЭ��ջ�£�Ŀǰֻ����ping�����е�IP��ַ,�����������ԣ�
*       Backstrlen�������ַ����ĳ��ȣ���С��512�ֽڡ�
*	    Sleeptime����NGЭ��ջ�£� ��ʾ����ping��֮��ļ��ʱ�䡣��λms
*                  ��LinuxЭ��ջ�£���ʾping�������󣬵ȴ�Ӧ���ʱ��,��λms
*                   
* ���: 
*       pBackStr���������ping�Ľ�����ռ����û����룬��С��512�ֽڡ�
*                 ����磺"Reply from 192.168.1.1,  ��.. time<5ms  "
*
* ����ֵ: 
*       �ɹ���0
*       ʧ�ܣ�-1
*   
************************************************************/
int CSPing(char *pPingStr,char*pBackStr,unsigned int backstrlen,unsigned int  sleeptime);

/***********************************************************
* ����: CSArpSniffer
* ����: ��鱾��IP��ַ�Ƿ������������г�ͻ
* ����:  
*      pEthName �������豸�����磺"eth0"��  
*	   wInaddr�������IP��ַ�������ֽ����ַ������ͨ��inet_addr()���ַ���ת��Ϊ�����ֽ����ַ ��   
*      wTimeOut:�ȴ�arpӦ���ʱ��,��λ��ms
*                   
* ���: 
*
* ����ֵ: 
*     IP��ַû�г�ͻ������0��
*     IP��ַ�г�ͻ����NGЭ��ջ�£�����NG_EADDRINUSE����linuxЭ��ջ�£�����1.  ��μ�cs_tcpiperr.h��
*     ���򷵻�����ֵ
*   
************************************************************/
int  CSArpSniffer(const char * pEthName, const unsigned wInaddr, const unsigned wTimeOut);


/***********************************************************
* ����: CSDelArpEntry
* ����: ɾ��IP��ַ��Ӧ��arp��¼��
* ����:  
*	   wInaddr��IP��ַ�������ֽ����ַ������ͨ��inet_addr()���ַ���ת��Ϊ�����ֽ����ַ ��   
*                   
* ���: 
*
* ����ֵ: 
*     �ɹ�������0��
*     ���򷵻ش����
*   
************************************************************/
int  CSDelArpEntry(unsigned wInaddr);


/***********************************************************
* ����: CSClearArpTable
* ����: ɾ������IP��ַ��Ӧ��arp��¼��
* ����:   
*                   
* ���: 
*
* ����ֵ: 
*     
************************************************************/
void CSClearArpTable(void);


/***********************************************************
* ����: CSRawSendPacket
* ���ܣ���Ӧ�ò㴫�����İ��������������������㷢�ͳ�ȥ��
* ���������
*    	pszEthName�������豸����
*	fd��        socket��������(*fd==-1��ʱ���ڴ˺����ڴ���socket,������sosket�޹�)
*	pcBuf��    Ӧ�ò�����/��̫��������
*	nBufLen��   buffer�ĳ���
*	nFlag��    �Ƿ���Ҫ�˺������udp/ip/ethernetͷ 
*	nFlag =1,��Ҫ��ӣ�
*	nFlag =0,ֱ�ӷ���
* �������:
* 	��
* ����ֵ: 
*  	�ɹ����� CSDHCP_SUCCESS
*	ʧ�ܷ��� CSDHCP_FAILURE
*   
* 
************************************************************/
int  CSRawSendPacket(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag);

/***********************************************************
* ����: CSRawSendPacketByMode
* ���ܣ���Ӧ�ò㴫�����İ��������������������㷢�ͳ�ȥ��
* ���������
*    	pszEthName�������豸����
*	fd��        socket��������(*fd==-1��ʱ���ڴ˺����ڴ���socket,������sosket�޹�)
*	pcBuf��    Ӧ�ò�����/��̫��������
*	nBufLen��   buffer�ĳ���
*	nFlag��    �Ƿ���Ҫ�˺������udp/ip/ethernetͷ 
*	nFlag =1,��Ҫ��ӣ�
*	nFlag =0,ֱ�ӷ���
*	eRawSendType=EM_UDIRAWSENDTYPE_UNICAST, ���͵����ݰ�Ϊ������
*	eRawSendType=EM_UDIRAWSENDTYPE_BROADCAST,���͵����ݰ�Ϊ�㲥��
* �������:
* 	��
* ����ֵ: 
*  	�ɹ����� CSDHCP_SUCCESS
*	ʧ�ܷ��� CSDHCP_FAILURE
*   
* 
************************************************************/
int  CSRawSendPacketByMode(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag, CSRawSendType_E eRawSendMode);

/***********************************************************
* ����: CSRawRecvPacket
* ���ܣ���Ӧ�ò㴫�����İ��������������������㷢�ͳ�ȥ��
* ���������
*    	pszEthName��  �����豸����
*	fd��          socket�����������Դ���
*	pnBufLen:      ��Ҫ���յ����ݳ���
*	nFlag ��       �Ƿ���Ҫ�˺���ȥ��udp/ip/ethernetͷ 
*	nFlag =1,��Ҫȡ����
*	nFlag =0,ֱ�ӽ�����һ��
* �������:
* 	pcBuf��     Ӧ�ò�����/��̫��������
*	pnBufLen��    ʵ�ʽ��յĳ���
* ����ֵ: 
*  	�ɹ����� CSDHCP_SUCCESS
*	ʧ�ܷ��� CSDHCP_FAILURE
* 
************************************************************/
int CSRawRecvPacket (char *pszEthName, int  *fd, const char *pchBuf, int* pnBufLen, int nFlag);

/***********************************************************
* ����: CSDNSSetDefaultTimeout
* ����: ����CSDNSGetHostByName������Ĭ�ϳ�ʱʱ�䣬��λms
* ����:   
*     dwTimeout : CSDNSGetHostByName������Ĭ�ϳ�ʱʱ�䣬��λms
*                        �ɸ��ݻ�����ʵ��������ú���ʱʱ��
*     ���  dwTimeout = 0, ����Ĭ�ϳ�ʱ��������-1
* ע��:
*           �ڲ� Ĭ�ϳ�ʱΪ2000ms    
* ���: 
*
* ����ֵ: 
*      ���óɹ��򷵻�0
*      ���  dwTimeout = 0, �����ڲ�Ĭ�ϳ�ʱ��������-1
*     
************************************************************/
int CSDNSSetDefaultTimeout(unsigned int dwTimeout) ;



/***********************************************************
* ����: CSDNSGetHostByNameWithTimeout
* ����: ������������gethostbyname��ͬ���ǿ������ó�ʱ������
*              ÿ������������
* ����:   
*     pcName :  Ҫ����������, ��"www.baidu.com",Ҳ������IP��ַ
*     dwTimeout : ������ʱʱ��, ��λms  
*     
*  ע��:
*     dwTimeout Ϊ�ȴ�һ��DNS������Ӧ���ʱ�䣬���
*     ������n��DNS�������������ȴ���ʱ��Ϊn*dwTimeout(ms)
*          
* ���: 
*
* ����ֵ: 
*       �����ɹ��򷵻ؽ��struct hostent*
*       ���򷵻�NULL
*     
************************************************************/
struct hostent *  CSDNSGetHostByNameWithTimeout(const char* pcName,unsigned int dwTimeout);

/***********************************************************
* ����: CSDNSGetHostByName
* ����: ����������ʹ��CSDNSSetDefaultTimeout ���õ�Ĭ�ϳ�ʱ
* ����:   
*     pcName :  Ҫ����������, ��"www.baidu.com",Ҳ������IP��ַ
*     
*  ע��:
*     ���˳�ʱʱ��ʹ��Ĭ�ϳ�ʱ���⣬����ͬCSDNSGetHostByNameWithTimeout
*          
* ���: 
*
* ����ֵ: 
*       �����ɹ��򷵻ؽ��struct hostent*
*       ���򷵻�NULL
*     
************************************************************/
struct hostent *  CSDNSGetHostByName(const char* pcName); 
/***********************************************************
* ����: CSDNSGetHostByNameWithTimeoutR
* ����: ����������֧�ֶ���������,��Ӧ��׼����gethostbyname_r
* ����:   
*     name :  Ҫ����������, ��"www.baidu.com",Ҳ������IP��ַ
*     buf:   ָ��һ��buf�ռ�
*     buflen: buf ��С 
*     dwTimeout : ������ʱʱ��, ��λms  
*     
*  ע��:
*     dwTimeout Ϊ�ȴ�һ��DNS������Ӧ���ʱ�䣬���
*     ������n��DNS�������������ȴ���ʱ��Ϊn*dwTimeout(ms)
*          
* ���: 
*     result_buf: ָ��һ�������ṹ������������ݴ���buf�У�ָ��buf
*     result:������Ľ������ָ��ָ��buf
*     h_errnop:�������
* ˵��:
*     result_buf��result������Ϊ���������result_bufָ��buf��
*     ��result��ָ��ָ��buf��������ʽ����ͬ��Ϊ�û��ṩ�����������ʽ
* ����ֵ: 
*       �����ɹ�0
*       ���򷵻ش������
*     
************************************************************/

int CSDNSGetHostByNameWithTimeoutR(const char * name,
                           struct hostent * result_buf,
                           char * buf, unsigned int buflen,
                           struct hostent ** result,
                           int * h_errnop,unsigned int dwTimeout);


/***********************************************************
* ����: CSDNSGetHostByNameR
* ����: ʹ��CSDNSSetDefaultTimeout ���õ�Ĭ�ϳ�ʱ
* ����:   
*     name :  Ҫ����������, ��"www.baidu.com",Ҳ������IP��ַ
*     
*     buf:   ָ��һ��buf�ռ䣬����������ݴ����buf��
*     buflen: buf ��С 
* 
*  ע��:
*     ������n��DNS�������������ȴ���ʱ��Ϊn*�����õ�Ĭ�ϳ�ʱʱ��
*     ����ʱʱ���⣬������ͬCSDNSGetHostByNameWithTimeoutR
*          
* ���: 
*     result_buf: ָ��һ�������ṹ,��Ϊ�������
*     result:������Ľ������ȡ�������ṹ��ָ�룬��Ϊ�������
*     h_errnop:������롣
* ˵��:
*     result_buf��result������Ϊ���������result_bufָ��buf��
*     ��result��ָ��ָ��buf��������ʽ����ͬ��Ϊ�û��ṩ�����������ʽ
* ����ֵ: 
*       �����ɹ�0
*       ���򷵻ش������
*     
************************************************************/

int CSDNSGetHostByNameR(const char * name,
                           struct hostent * result_buf,
                           char * buf, unsigned int buflen,
                           struct hostent ** result,
                           int * h_errnop);




#define     CSGETHOSTBYNAME     CSDNSGetHostByName

#ifdef __cplusplus
}
#endif

#endif

