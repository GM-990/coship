/**@defgroup SOCKET SOCKET ��ģ���SOCKET ģ�����õ����������ͺ����ݽ��ж��壬�Լ��Ե��ýӿں�������˵��
@brief  Socketģ���Ӧͷ�ļ�udi2_socket.h�������ݻ����൱��Linux��Socket�ӿڣ�
�����ʵ��ʱ���κ����⣬���ǽӿ��ĵ����ر�˵����������ɲο�Linux������ĵ���

@version 2.0.2 2009/08/24 �ۺ������İ汾
@version 2.0.1 2009/08/12 ��ʾ�汾
@{
*/
#ifndef _UDI2_SOCKET_H_
#define _UDI2_SOCKET_H_

#include "udi2_typedef.h"

#ifdef   __cplusplus
extern "C" {
#endif

#define  CSUDI_FD_MAX    						(1024)  					 ///< max file descriptor
#define  CSUDI_INVALID_SOCKET   					(-1)						 ///< invalid  socket
#define  CSUDI_SOCKET_ERROR    					(-1)						 ///< socket error
#define  CSUDI_IOCPARM_MASK   					(0x1fff)    				 ///< parameter length, at most 13 bits.

#define  CSUDI_FIONBIO        						(0x667e)                          	 ///< set/clear nonblocking io.
#define  CSUDI_FIONREAD      						(0x467f)                         	 ///< get # bytes to read.

#define  CSUDI_IPPROTO_IP         				 	(0)             				///< Dummy protocol for TCP. 
#define  CSUDI_IPPROTO_HOPOPTS 				(0)    					///< IPv6 Hop-by-Hop options.
#define  CSUDI_IPPROTO_ICMP       				(1)   	   				///< Internet Control Message Protocol.
#define  CSUDI_IPPROTO_IGMP        				(2)						///< Internet Group Management Protocol. 
#define  CSUDI_IPPROTO_IPIP        				(4)						///< IPIP tunnels (older KA9Q tunnels use 94).
#define  CSUDI_IPPROTO_TCP         				(6)						///< Transmission Control Protocol. 
#define  CSUDI_IPPROTO_EGP         				(8)						///< Exterior Gateway Protocol.
#define  CSUDI_IPPROTO_PUP        					(12)						///< PUP protocol.
#define  CSUDI_IPPROTO_UDP        					(17)						///< User Datagram Protocol.
#define  CSUDI_IPPROTO_IDP         					(22)						///< XNS IDP protocol.
#define  CSUDI_IPPROTO_TP          					(29)						///< SO Transport Protocol Class 4. 
#define  CSUDI_IPPROTO_IPV6       					(41)						///< IPv6 header.
#define  CSUDI_IPPROTO_ROUTING     				(43)						///< IPv6 routing header.
#define  CSUDI_IPPROTO_FRAGMENT   				(44)						///< IPv6 fragmentation header. 
#define  CSUDI_IPPROTO_RSVP        				(46)						///< Reservation Protocol.
#define  CSUDI_IPPROTO_GRE          				(47)						///< General Routing Encapsulation.
#define  CSUDI_IPPROTO_ESP          				(50)						///< encapsulating security payload.
#define  CSUDI_IPPROTO_AH            				(51)		 				///< authentication header. 
#define  CSUDI_IPPROTO_ICMPV6                              (58)                                    ///< ICMPv6. 
#define  CSUDI_IPPROTO_NONE         				(59)						///< IPv6 no next header. 
#define  CSUDI_IPPROTO_DSTOPTS    				(60)						///< IPv6 destination options.
#define  CSUDI_IPPROTO_MTP 		 				(92)						///< Multicast Transport Protocol. 
#define  CSUDI_IPPROTO_ENCAP       				(98)						///< Encapsulation Header. 
#define  CSUDI_IPPROTO_PIM           				(103)					///< Protocol Independent Multicast.
#define  CSUDI_IPPROTO_COMP        				(108)					///< Compression Header Protocol. 
#define  CSUDI_IPPROTO_RAW          				(255)					///< Raw IP packets. 


#define CSUDI_BTPROTO_L2CAP	0
#define CSUDI_BTPROTO_HCI	1
#define CSUDI_BTPROTO_SCO	2
#define CSUDI_BTPROTO_RFCOMM	3
#define CSUDI_BTPROTO_BNEP	4
#define CSUDI_BTPROTO_CMTP	5
#define CSUDI_BTPROTO_HIDP	6
#define CSUDI_BTPROTO_AVDTP	7


#define  CSUDI_IPPORT_ECHO             	   			(7)						///< Echo service. 
#define  CSUDI_IPPORT_DISCARD       	   			(9)						///< Discard transmissions service. 
#define  CSUDI_IPPORT_SYSTAT               			(11)						///< System status service.
#define  CSUDI_IPPORT_DAYTIME             			(13)						///< Time of day service.
#define  CSUDI_IPPORT_NETSTAT             			(15)						///< Network status service.
#define  CSUDI_IPPORT_FTP                      			(21)						///< File Transfer Protocol. 
#define  CSUDI_IPPORT_TELNET                			(23)						///< Telnet protocol.
#define  CSUDI_IPPORT_SMTP                   			(25)						///< Simple Mail Transfer Protocol.
#define  CSUDI_IPPORT_TIMESERVER        			(37)						///< Timeserver service. 
#define  CSUDI_IPPORT_NAMESERVER       			(42)						///< Domain Name Service. 
#define  CSUDI_IPPORT_WHOIS                 			(43)						///< Internet Whois service.
#define  CSUDI_IPPORT_MTP                      			(57)						///< Media Transfer Protocol 
#define  CSUDI_IPPORT_TFTP                     			(69)						///< Trivial File Transfer Protocol.
#define  CSUDI_IPPORT_RJE                       			(77)						///< Remote Job Entry 
#define  CSUDI_IPPORT_FINGER                 			(79)						///< Finger service.
#define  CSUDI_IPPORT_TTYLINK                			(87)						///< TTY link
#define  CSUDI_IPPORT_SUPDUP                 			(95)						///< SUPDUP protocol. 
#define  CSUDI_IPPORT_EXECSERVER 	 			(512)					///< execd service.  
#define  CSUDI_IPPORT_LOGINSERVER   			(513)					///< rlogind service.  
#define  CSUDI_IPPORT_CMDSERVER 	 			(514) 					///< command service. 
#define  CSUDI_IPPORT_EFSSERVER 	 			(520) 					///< EFS service. 

#define  CSUDI_IPPORT_BIFFUDP  		 			(512)					///< BIFF UDP service. 
#define  CSUDI_IPPORT_WHOSERVER 	 			(513)					///< WHO service. 
#define  CSUDI_IPPORT_ROUTESERVER   			(520)					///< ROUTE service. 
#define  CSUDI_IPPORT_RESERVED 	 			(1024)  					///< Ports less than this value are reserved for privileged processes.
#define  CSUDI_IPPORT_USERRESERVED 			(5000)   					///< Ports greater this value are reserved for (non-privileged) servers. 

#define  CSUDI_TCP_NODELAY	 					(1)						///< Don't delay send to coalesce packets.  
#define  CSUDI_TCP_MAXSEG						(2)						///< Set maximum segment size.  
#define  CSUDI_TCP_CORK	 						(3)						///< Control sending of partial frames.  
#define  CSUDI_TCP_KEEPIDLE	 					(4)						///< Start keeplives after this period. 
#define  CSUDI_TCP_KEEPINTVL					(5)						///< Interval between keepalives. 
#define  CSUDI_TCP_KEEPCNT	 					(6)						///< Number of keepalives before death. 
#define  CSUDI_TCP_SYNCNT	 					(7)						///< Number of SYN retransmits. 
#define  CSUDI_TCP_LINGER2						(8)						///< Life time of orphaned FIN-WAIT-2 state. 
#define  CSUDI_TCP_DEFER_ACCEPT				(9)						///< Wake up listener only when data arrive. 
#define  CSUDI_TCP_WINDOW_CLAMP 				(10)						///< Bound advertised window. 
#define  CSUDI_TCP_INFO	 						(11)						///< Information about this connection. 
#define  CSUDI_TCP_QUICKACK	 				(12)						///< Bock/reenable quick ACKs.  

#define  CSUDI_TCPOPT_EOL						(0)						///< end of option list 
#define  CSUDI_TCPOPT_NOP						(1)						///< no option
#define  CSUDI_TCPOPT_MAXSEG					(2)						///< max segment
#define  CSUDI_TCPOPT_WINDOW					(3)						///< wodow 
#define  CSUDI_TCPOPT_SACK_PERMITTED			(4)						///< Experimental. 
#define  CSUDI_TCPOPT_SACK						(5)		   				///< Experimental. 
#define  CSUDI_TCPOPT_TIMESTAMP				(8)						///< time stamp option
						
#define  CSUDI_TCPOLEN_SACK_PERMITTED			(2)						///< Experimental. 
#define  CSUDI_TCPOLEN_WINDOW					(3)                                      ///< length of wodow
#define  CSUDI_TCPOLEN_MAXSEG					(4)						///< length of max segment
#define  CSUDI_TCPOLEN_TIMESTAMP				(10)						///< length of time stamp
#define  CSUDI_TCPOLEN_TSTAMP_APPA			(CSUDI_TCPOLEN_TIMESTAMP+2)	 ///< appendix A 
#define  CSUDI_TCPOPT_TSTAMP_HDR	\
    (CSUDI_TCPOPT_NOP<<24|CSUDI_TCPOPT_NOP<<16|CSUDI_TCPOPT_TIMESTAMP<<8|CSUDI_TCPOLEN_TIMESTAMP)

#define  CSUDI_TCP_MSS							(512)					///< packet subsect
#define  CSUDI_TCP_MAXWIN						(65535)					///< largest value for (unscaled) window. 
#define  CSUDI_TCP_MAX_WINSHIFT				(14)						///< maximum window shift.  

#define  CSUDI_TCPI_OPT_TIMESTAMPS 			(1)						///< time stamp 
#define  CSUDI_TCPI_OPT_SACK					(2)						///< Selective Acknowledgment
#define  CSUDI_TCPI_OPT_WSCALE					(4)						///< Window Scale Option
#define  CSUDI_TCPI_OPT_ECN					(8)						///< explicit congestion notification

#define  CSUDI_PF_UNSPEC           					(0)						///< Unspecified.
#define  CSUDI_PF_LOCAL							(1)	 					///< Local to host (pipes and file-domain). 
#define  CSUDI_PF_UNIX							CSUDI_PF_LOCAL 			///< Old BSD name for CSUDI_PF_LOCAL.  
#define  CSUDI_PF_FILE							CSUDI_PF_LOCAL 			///< Another non-standard name for CSUDI_PF_LOCAL.  
#define  CSUDI_PF_INET                					(2)						///< IP protocol family.
#define  CSUDI_PF_AX25							(3)						///< Amateur Radio AX.25.  
#define  CSUDI_PF_IPX							(4)						///< Novell Internet Protocol.  
#define  CSUDI_PF_APPLETALK					(5)						///< Appletalk DDP.  
#define  CSUDI_PF_NETROM						(6)						///< Amateur radio NetROM.  
#define  CSUDI_PF_BRIDGE						(7)						///< Multiprotocol bridge.  
#define  CSUDI_PF_ATMPVC						(8)						///< ATM PVCs.  
#define  CSUDI_PF_X25							(9)						///< Reserved for X.25 project.  
#define  CSUDI_PF_INET6							(10)						///< IP version 6.  
#define  CSUDI_PF_ROSE							(11)						///< Amateur Radio X.25 PLP.  
#define  CSUDI_PF_DECnet						(12)						///< Reserved for DECnet project. 
#define  CSUDI_PF_NETBEUI						(13)						///< Reserved for 802.2LLC project.  
#define  CSUDI_PF_SECURITY						(14)						///< Security callback pseudo AF.  
#define  CSUDI_PF_KEY							(15)						///< PF_KEY key management API.  
#define  CSUDI_PF_NETLINK						(16)  					///< net link
#define  CSUDI_PF_ROUTE	 						CSUDI_PF_NETLINK 		///< Alias to emulate 4.4BSD.  
#define  CSUDI_PF_PACKET						(17)						///< Packet family.  
#define  CSUDI_PF_ASH							(18)						///< Ash.  
#define  CSUDI_PF_ECONET						(19)						///< Acorn Econet.  
#define  CSUDI_PF_ATMSVC						(20)						///< ATM SVCs.  
#define  CSUDI_PF_SNA							(22)						///< Linux SNA Project 
#define  CSUDI_PF_IRDA							(23)						///< IRDA sockets.  
#define  CSUDI_PF_PPPOX							(24)						///< PPPoX sockets.  
#define  CSUDI_PF_WANPIPE						(25)						///< Wanpipe API sockets.  
#define  CSUDI_PF_BLUETOOTH					(31)						///< Bluetooth sockets.  
#define  CSUDI_PF_MAX							(32)						///< For now.  

#define  CSUDI_AF_UNSPEC        					CSUDI_PF_UNSPEC      	 	///< Unspecified.
#define  CSUDI_AF_INET           	  				CSUDI_PF_INET      		///< IP address family.
#define  CSUDI_AF_LOCAL							CSUDI_PF_LOCAL			///< Local to host (pipes and file-domain).  
#define  CSUDI_AF_UNIX							CSUDI_PF_UNIX			///< Old BSD name for CSUDI_AF_LOCAL. 
#define  CSUDI_AF_FILE							CSUDI_PF_FILE			///< Another non-standard name for CSUDI_AF_LOCAL.  
#define  CSUDI_AF_AX25							CSUDI_PF_AX25			///< Amateur Radio AX.25.  
#define  CSUDI_AF_IPX							CSUDI_PF_IPX			///< Novell Internet Protocol.  
#define  CSUDI_AF_APPLETALK					CSUDI_PF_APPLETALK  		///< Appletalk DDP.  
#define  CSUDI_AF_NETROM						CSUDI_PF_NETROM		///< Amateur radio NetROM.  
#define  CSUDI_AF_BRIDGE						CSUDI_PF_BRIDGE		///< Multiprotocol bridge.  
#define  CSUDI_AF_ATMPVC						CSUDI_PF_ATMPVC		///< ATM PVCs.  
#define  CSUDI_AF_X25							CSUDI_PF_X25			///< Reserved for X.25 project.  
#define  CSUDI_AF_INET6							CSUDI_PF_INET6			///< IP version 6.  
#define  CSUDI_AF_ROSE							CSUDI_PF_ROSE			///< Amateur Radio X.25 PLP.  
#define  CSUDI_AF_DECnet						CSUDI_PF_DECnet			///< Reserved for DECnet project. 
#define  CSUDI_AF_NETBEUI						CSUDI_PF_NETBEUI		///< Reserved for 802.2LLC project.  
#define  CSUDI_AF_SECURITY						CSUDI_PF_SECURITY		///< Security callback pseudo AF.  
#define  CSUDI_AF_KEY							CSUDI_PF_KEY			///< AF_KEY key management API.  
#define  CSUDI_AF_NETLINK						CSUDI_PF_NETLINK		///< net link
#define  CSUDI_AF_ROUTE							CSUDI_PF_ROUTE			///< Alias to emulate 4.4BSD.  
#define  CSUDI_AF_PACKET						CSUDI_PF_PACKET		///< Packet family.  
#define  CSUDI_AF_ASH							CSUDI_PF_ASH			///< Ash.  
#define  CSUDI_AF_ECONET						CSUDI_PF_ECONET		///< Acorn Econet.  
#define  CSUDI_AF_ATMSVC						CSUDI_PF_ATMSVC		///< ATM SVCs.  
#define  CSUDI_AF_SNA							CSUDI_PF_SNA			///< Linux SNA Project 
#define  CSUDI_AF_IRDA							CSUDI_PF_IRDA			///< IRDA sockets.  
#define  CSUDI_AF_PPPOX							CSUDI_PF_PPPOX			///< PPPoX sockets.  
#define  CSUDI_AF_WANPIPE						CSUDI_PF_WANPIPE		///< Wanpipe API sockets.  
#define  CSUDI_AF_BLUETOOTH					CSUDI_PF_BLUETOOTH 		///< Bluetooth sockets.  
#define  CSUDI_AF_MAX							CSUDI_PF_MAX			///< For now. 

#define  CSUDI_SOCK_DGRAM          				(1)						///< Connectionless, unreliable datagrams of fixed maximum length. 
#define  CSUDI_SOCK_STREAM        				(2)						///< Sequenced, reliable, connection-based byte streams.  
#define  CSUDI_SOCK_RAW              				(3)						///< Raw protocol interface. 
#define  CSUDI_SOCK_RDM              				(4)						///< Reliably-delivered messages.  
#define  CSUDI_SOCK_SEQPACKET  	 				(5)						///< Sequenced, reliable, connection-based, datagrams of fixed maximum length.
#define  CSUDI_SOCK_PACKET  					(10)						///< Linux specific way of getting packets at the dev level.  For writing rarp and other similar things on the user level.

#define  CSUDI_ETH_P_ALL						(0x0003)					///< Every packet (be careful!!!)
#define  CSUDI_ETH_P_ARP						(0x0806)					///< Address Resolution packet	
#define  CSUDI_ETH_P_IP							(0x0800)					///< Internet Protocol packet	
#define  CSUDI_ETH_P_RARP      					(0x8035)					///< Reverse Addr Res packet	
#define  CSUDI_ETH_P_IPV6						(0x86DD)					///< IPv6 over bluebook		

#define  CSUDI_SOL_RAW							(255)					///< RAW layer
#define  CSUDI_SOL_DECNET      					(261)					///< DECNET layer
#define  CSUDI_SOL_X25        						(262)					///< X25 layer
#define  CSUDI_SOL_PACKET						(263)					///< PACKET layer
#define  CSUDI_SOL_ATM							(264)					///< ATM layer (cell level).  
#define  CSUDI_SOL_AAL							(265)					///< ATM Adaption Layer (packet level).  
#define  CSUDI_SOL_IRDA							(266)					///< IRDA layer
#define  CSUDI_SOL_SOCKET         					(0xffff)					///< SOCKET layer
#define  CSUDI_SOL_IP                 					(0) 						///< To select the IP level. 
#define  CSUDI_SOL_TCP               					(6) 						///< TCP level
#define  CSUDI_SOL_UDP              					(17)						///< sockopt level for UDP


#define CSUDI_SOL_HCI		0
#define CSUDI_SOL_L2CAP	    6
#define CSUDI_SOL_SCO		17
#define CSUDI_SOL_RFCOMM	18


#define  CSUDI_SO_DEBUG            					(0x0001)      				///< turn on debugging info recording. 
#define  CSUDI_SO_REUSEADDR      				(0x0004)  				///< Allow reuse of local addresses.
#define  CSUDI_SO_KEEPALIVE      					(0x0008)  				///< Keep connections alive and send SIGPIPE when they die. 
#define  CSUDI_SO_DONTROUTE      				(0x0010)  				///< Don't do local routing. 
#define  CSUDI_SO_BROADCAST      				(0x0020)  				///< Allow transmission of broadcast messages.
#define  CSUDI_SO_LINGER            					(0x0080)   	 			///< Block on close of a reliable socket to transmit pending data.
#define  CSUDI_SO_OOBINLINE      					(0x0100)  				///< Receive out-of-band data in-band.
#define  CSUDI_SO_SNDBUF          					(0x1001)    				///< send buffer size. 
#define  CSUDI_SO_RCVBUF          					(0x1002)    				///< receive buffer size. 
#define  CSUDI_SO_SNDLOWAT    					(0x1003)   				///< send low-water mark. 
#define  CSUDI_SO_RCVLOWAT     					(0x1004)    				///< receive low-water mark. 
#define  CSUDI_SO_SNDTIMEO      					(0x1005)    				///< send timeout.
#define  CSUDI_SO_RCVTIMEO      					(0x1006)    				///< receive timeout. 
#define  CSUDI_SO_ERROR         					(0x1007)     				///< get error status and clear (ro). 
#define  CSUDI_SO_TYPE          					(0x1008)     				///< get socket type (ro) ,Compatible name for SO_STYLE.
#define  CSUDI_SO_STYLE	                                          CSUDI_SO_TYPE	              ///< Synonym 
#define  CSUDI_SO_ACCEPTCONN     				(0x1009)  				///< accept connection


/**@brief linux-specific, might as well be the same as on i386 */
#define CSUDI_SO_NO_CHECK						(11)
#define CSUDI_SO_PRIORITY						(12)
#define CSUDI_SO_BSDCOMPAT					(14)
#define CSUDI_SO_PASSCRED						(17)
#define CSUDI_SO_PEERCRED						(18)

/**@brief Security levels - as per NRL IPv6 - don't actually do anything */
#define CSUDI_SO_SECURITY_AUTHENTICATION		(22)
#define CSUDI_SO_SECURITY_ENCRYPTION_TRANSPORT	(23)
#define CSUDI_SO_SECURITY_ENCRYPTION_NETWORK	(24)
#define CSUDI_SO_BINDTODEVICE					(25)

/**@brief Socket filtering */
#define CSUDI_SO_ATTACH_FILTER        			(26)
#define CSUDI_SO_DETACH_FILTER       			(27)
#define CSUDI_SO_PEERNAME             				(28)
#define CSUDI_SO_TIMESTAMP					(29)
#define CSUDI_SCM_TIMESTAMP					CSUDI_SO_TIMESTAMP
#define CSUDI_SO_PEERSEC						(30)


#define  CSUDI_IP_TOS               	 	 			(1)   					///< int; IP type of service and precedence.  
#define  CSUDI_IP_TTL               	 	 			(2)   					///< int; IP time to live.
#define  CSUDI_IP_HDRINCL           	 	 			(3)   					///< int; Header is included with data.  
#define  CSUDI_IP_OPTIONS         					(4)						///< ip_opts; IP per-packet options.  
#define  CSUDI_IP_ROUTER_ALERT    				(5)						///< bool 
#define  CSUDI_IP_RECVOPTS        					(6)						///< bool 
#define  CSUDI_IP_RETOPTS         					(7)						///< bool 
#define  CSUDI_IP_PKTINFO         					(8)						///< bool 
#define  CSUDI_IP_PKTOPTIONS      				(9)
#define  CSUDI_IP_PMTUDISC        					(10)						///< obsolete name
#define  CSUDI_IP_MTU_DISCOVER    				(10)						///< int; see below 
#define  CSUDI_IP_RECVERR         					(11)						///< bool 
#define  CSUDI_IP_RECVTTL         					(12)						///< bool 
#define  CSUDI_IP_RECVTOS         					(13)						///< bool 
#define  CSUDI_IP_MULTICAST_IF        				(32)   					///< set/get IP multicast i/f .
#define  CSUDI_IP_MULTICAST_TTL       			(33)   					///< set/get IP multicast ttl  .
#define  CSUDI_IP_MULTICAST_LOOP      			(34)   					///< set/get IP multicast loopback .
#define  CSUDI_IP_ADD_MEMBERSHIP      			(35)   					///< add an IP group membership.
#define  CSUDI_IP_DROP_MEMBERSHIP     			(36)   					///< drop an IP group membership .
#define  CSUDI_IP_UNBLOCK_SOURCE				(37)
#define  CSUDI_IP_BLOCK_SOURCE					(38)
#define  CSUDI_IP_ADD_SOURCE_MEMBERSHIP		(39)
#define  CSUDI_IP_DROP_SOURCE_MEMBERSHIP		(40)
#define  CSUDI_IP_MSFILTER						(41)
#define  CSUDI_MCAST_JOIN_GROUP		              (42)
#define  CSUDI_MCAST_BLOCK_SOURCE		              (43)
#define  CSUDI_MCAST_UNBLOCK_SOURCE		       (44)
#define  CSUDI_MCAST_LEAVE_GROUP		              (45)
#define  CSUDI_MCAST_JOIN_SOURCE_GROUP		(46)
#define  CSUDI_MCAST_LEAVE_SOURCE_GROUP	       (47)
#define  CSUDI_MCAST_MSFILTER			              (48)

#define CSUDI_MCAST_EXCLUDE					(0)
#define CSUDI_MCAST_INCLUDE					(1)


#define CSUDI_IP_RECVRETOPTS					CSUDI_IP_RETOPTS		///< For BSD compatibility.  

#define CSUDI_IP_PMTUDISC_DONT   				(0)						///< Never send DF frames.  
#define CSUDI_IP_PMTUDISC_WANT   				(1)						///< Use per route hints.  
#define CSUDI_IP_PMTUDISC_DO     				(2)						///< Always DF.  


#define CSUDI_IP_DEFAULT_MULTICAST_TTL        	(1)
#define CSUDI_IP_DEFAULT_MULTICAST_LOOP       	(1)
#define CSUDI_IP_MAX_MEMBERSHIPS              		(20)


#define  CSUDI_MSG_OOB           	  				(0x01)    					///< Process out-of-band data. 
#define  CSUDI_MSG_PEEK          	 				(0x02)					///< Peek at incoming messages.  
#define  CSUDI_MSG_DONTROUTE 	  				(0x04)     				///< Don't use local routing. 
#define  CSUDI_MSG_CTRUNC						(0x08)					///< Control data lost before delivery.  
#define  CSUDI_MSG_PROXY						(0x10)					///< Supply or ask second address.  
#define  CSUDI_MSG_TRUNC          	 				(0x20)     				///< rcvd data truncated .
#define  CSUDI_MSG_DONTWAIT	 				(0x40) 					///< Nonblocking IO.  
#define  CSUDI_MSG_EOR              	 				(0x80)    					///< end of record. NG
#define  CSUDI_MSG_TRYHARD						CSUDI_MSG_DONTROUTE
#define  CSUDI_MSG_WAITALL						(0x100)					///< Wait for a full request.  
#define  CSUDI_MSG_FIN							(0x200)					///< Finished
#define  CSUDI_MSG_SYN							(0x400)					///< Synchronize   
#define  CSUDI_MSG_CONFIRM						(0x800)					///< Confirm path validity.
#define  CSUDI_MSG_RST							(0x1000)					///< Reset
#define  CSUDI_MSG_ERRQUEUE					(0x2000)					///< Fetch message from error queue. 
#define  CSUDI_MSG_NOSIGNAL					(0x4000)					///< Do not generate SIGPIPE.
#define  CSUDI_MSG_MORE						(0x8000)					///< Sender will send more.

#define  CSUDI_F_GETFL         						(3)     					///< Get file status flags.
#define  CSUDI_F_SETFL         						(4)     					///< Set file status flags.
#define  CSUDI_O_NONBLOCK      					(0x0080)     				///< set/clear nonblocking io 
#define  CSUDI_O_NDELAY        					CSUDI_O_NONBLOCK		///< set/clear nonblocking io 

/**@brief the ip is class A  */
#define  CSUDI_IN_CLASSA(i)            				(((CSUDIInAddr_t)(i) & 0x80000000) == 0)
#define  CSUDI_IN_CLASSA_NET           				(0xff000000)
#define  CSUDI_IN_CLASSA_NSHIFT        			(24)
#define  CSUDI_IN_CLASSA_HOST          			(0x00ffffff)  
#define  CSUDI_IN_CLASSA_MAX           				(128)

/**@brief the ip is class B  */
#define  CSUDI_IN_CLASSB(i)            				(((CSUDIInAddr_t)(i) & 0xc0000000) == 0x80000000)
#define  CSUDI_IN_CLASSB_NET           				(0xffff0000)
#define  CSUDI_IN_CLASSB_NSHIFT        			(16)
#define  CSUDI_IN_CLASSB_HOST          			(0x0000ffff)
#define  CSUDI_IN_CLASSB_MAX           				(65536)

/**@brief the ip is class C  */
#define  CSUDI_IN_CLASSC(i)            				(((CSUDIInAddr_t)(i) & 0xe0000000) == 0xc0000000)
#define  CSUDI_IN_CLASSC_NET           				(0xffffff00)
#define  CSUDI_IN_CLASSC_NSHIFT        			(8)
#define  CSUDI_IN_CLASSC_HOST          			(0x000000ff)

/**@brief the ip is class D  */
#define  CSUDI_IN_CLASSD(i)            				(((CSUDIInAddr_t)(i) & 0xf0000000) == 0xe0000000)
#define  CSUDI_IN_MULTICAST(i)         				CSUDI_IN_CLASSD(i)

/**@brief the ip is class E  */
#define  CSUDI_IN_EXPERIMENTAL(i)      			(((CSUDIInAddr_t)(i) & 0xe0000000) == 0xe0000000)
#define  CSUDI_IN_BADCLASS(i)          				(((CSUDIInAddr_t)(i) & 0xf0000000) == 0xf0000000)

#define  CSUDI_IN_LOOPBACKNET					(127)						///< Network number for local host loopback.  			

#define  CSUDI_INADDR_ANY              				((CSUDIInAddr_t)0x00000000)    ///< Address to accept any incoming messages. 
#define  CSUDI_INADDR_LOOPBACK         			((CSUDIInAddr_t)0x7f000001)	///< Address to loopback in software to local host.  Inet 127.0.0.1. 
#define  CSUDI_INADDR_BROADCAST        			((CSUDIInAddr_t)0xffffffff)	///< Address to send to all hosts.
#define  CSUDI_INADDR_NONE             				((CSUDIInAddr_t)0xffffffff)	///< Address indicating an error return. 
#define  CSUDI_INADDR_UNSPEC_GROUP    	 		((CSUDIInAddr_t)0xe0000000)	///< 224.0.0.0
#define  CSUDI_INADDR_ALLHOSTS_GROUP  	 	((CSUDIInAddr_t)0xe0000001)	///< 224.0.0.1
#define  CSUDI_INADDR_ALLRTRS_GROUP      		((CSUDIInAddr_t)0xe0000002) 	//<  224.0.0.2 
#define  CSUDI_INADDR_MAX_LOCAL_GROUP 		((CSUDIInAddr_t)0xe00000ff)	///< 224.0.0.255

#define CSUDI_INET_ADDRSTRLEN 					(16)
#define CSUDI_INET6_ADDRSTRLEN 				(46)


#pragma pack(1)//1�ֽڶ���
/* BD Address */
typedef struct {
	unsigned char b[6];
} CSUDIBDAddrr_S;
#pragma pack()//�ָ�����״̬

#define CSUDI_BDADDR_ANY   (&(CSUDIBDAddrr_S) {{0, 0, 0, 0, 0, 0}})
#define CSUDI_BDADDR_ALL   (&(CSUDIBDAddrr_S) {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}})
#define CSUDI_BDADDR_LOCAL (&(CSUDIBDAddrr_S) {{0, 0, 0, 0xff, 0xff, 0xff}})


#define CSUDISockaddr_Storage_S CSUDIKernel_Sockaddr_Storage_S
#define CSUDIK_SS_MAXSIZE	128												///< Implementation specific max size 


#ifdef WIN32

#include <WinSock.h>
typedef unsigned long CSUDIInAddr_t;


/**@brief ����������׽��־�� */
#define CSUDIFdSet_S fd_set


/**@brief add a descripter to a set */
#define  CSUDI_FD_SET FD_SET

/**@brief remove a descripter from a set	*/
#define  CSUDI_FD_CLR FD_CLR

/**@brief test to see if a descriptor is part of the set	*/
#define  CSUDI_FD_ISSET FD_ISSET

/**@brief clear a set */
#define  CSUDI_FD_ZERO FD_ZERO


#define CSUDI_Caddr_tt caddr_tt

#ifdef CSUDI_FIONBIO
#undef CSUDI_FIONBIO
#endif
#define  CSUDI_FIONBIO        						FIONBIO               ///< set/clear nonblocking io.

#ifdef CSUDI_FIONREAD
#undef CSUDI_FIONREAD
#endif
#define  CSUDI_FIONREAD      						FIONREAD            ///< get # bytes to read.


/**@brief �ӳٹر�socket */
typedef struct linger CSUDILinger_S;


/**@brief ������������� */
typedef struct iovec CSUDIIovec_S;


/**@brief ��Ϣ���ͺͽ�����Ϣ�����ṹ�� */
typedef struct msghdr CSUDIMsghdr_S;


/**@brief ������Ϣ */
typedef struct hostent CSUDIHostent_S;


/**@brief �������Ϣ  */
typedef struct servent CSUDIServent_S;


/**@brief Э����Ϣ  */
typedef struct protoent CSUDIProtoent_S;


/**@brief ��ַ��Ϣ�ṹ��*/
typedef struct in_addr CSUDIInAddr_S;


/**@brief �����׽�����Ϣ�ṹ��

@note �˽ṹ�����������CSUDISockAddr_S�ṹ��ͬ�����CSUDISockAddr_Sע��
*/
typedef struct sockaddr_in CSUDISockAddr_IN_S;


/**@brief �����׽�����Ϣ�ṹ��

@note �û�ʹ��CSUDISockAddr_IN_S�ṹ���и�ֵ��ǿ��ת��ΪCSUDISockAddr_S�ṹ��ΪCSUDISOCKConnect�����Ĳ�������
*/
typedef struct sockaddr CSUDISockAddr_S;


/**@brief ������˳��鲥*/
typedef struct ip_mreq CSUDIIp_Mreq_S;


/**@brief ��ָ���ӿڼ�����˳��鲥 */
//#define CSUDIIp_Mreqn_S ip_mreqn


/**@brief ��ʱʱ��ֵ�ṹ��*/
typedef struct timeval CSUDITimeval_S;


//////////////////////////////////////////////////////////////////////////////////////////begin

/**@brief IPv6 ��ַ*/
typedef struct 
{
	union 
	{
		unsigned char		u6_addr8[16];
		unsigned short	u6_addr16[8];
		unsigned int		u6_addr32[4];
	} in6_u;
#define s6_addr			in6_u.u6_addr8
#define s6_addr16		in6_u.u6_addr16
#define s6_addr32		in6_u.u6_addr32
}CSUDISockIn6_Addr_S;

extern const CSUDISockIn6_Addr_S CSUDI_IN6ADDR_ANY;        						///<ipv6 addr_any
extern const CSUDISockIn6_Addr_S CSUDI_IN6ADDR_LOOPBACK;   					///<ipv6 loopback


/**@brief ipv6  �����׽�����Ϣ�ṹ��*/
typedef struct  
{
	unsigned short 	sin6_family;    			 ///<ipv6 ��ַ��
	unsigned short	sin6_port;     			 ///<ipv6 �˿ں�
	unsigned int		sin6_flowinfo;  			 ///<ipv6 ����Ϣ
	CSUDISockIn6_Addr_S		sin6_addr;     	 ///<ipv6 ��ַ
	unsigned int			sin6_scope_id;  		 ///<ipv6 socpe id
}CSUDISockAddr_IN6_S;

/**@brief ipv6 ������˳��鲥*/
typedef struct
{
	unsigned int		imr_multiaddr;			///<�鲥���ַ
	unsigned int		imr_interface;			///<����ӿ�IP��ַ
	unsigned int		imr_sourceaddr;			///<�鲥Դ��ַ
}CSUDIIp_Mreq_Source_S ;

/**@brief socket�ں˵�ַ�ṹ*/
typedef struct
{
	unsigned short	ss_family;								///<��ַ��
	char		data[CSUDIK_SS_MAXSIZE - sizeof(unsigned short)];	///<socket ��ַ��Ϣ
} CSUDIKernel_Sockaddr_Storage_S;

/**@brief �鲥������*/
typedef struct 
{
	unsigned int				 gr_interface;		///<����ӿ�����
	CSUDIKernel_Sockaddr_Storage_S gr_group;		///<�鲥���ַ
}CSUDIGroup_Req_S;

/**@brief�鲥����*/
typedef struct 
{
	unsigned int				 gf_interface;		///<����ӿ�����
	CSUDIKernel_Sockaddr_Storage_S gf_group;		///<�鲥���ַ
	unsigned int				 gf_fmode;			///<����ģʽ
	unsigned int				 gf_numsrc;			///<�鲥Դ����
	CSUDIKernel_Sockaddr_Storage_S gf_slist[1];		///<����ӿ�����
}CSUDIGroup_Filter_S;

/**@brief�鲥������*/
typedef struct 
{
	unsigned int				 gsr_interface;		///<����ӿ�����
	CSUDIKernel_Sockaddr_Storage_S gsr_group;	///<�鲥���ַ
	CSUDIKernel_Sockaddr_Storage_S gsr_source;	///<�鲥Դ��ַ
}CSUDIGroup_Source_Req_S;
////////////////////////////////////////////////////////////////////////////////////////////end



#else

/**@brief ����������׽��־�� */
typedef struct 
{
  long fds[( CSUDI_FD_MAX)/(8*sizeof(long))];
  #define CSUDI_FDS_BITS(set)  ((set)->fds)
} CSUDIFdSet_S;


/**@brief add a descripter to a set */
#define  CSUDI_FD_SET(fd, fdset)       (CSUDI_FDS_BITS (fdset)[(fd /(8*sizeof ( long)))] |= (1UL << ((fd) % (8*sizeof ( long)))))

/**@brief remove a descripter from a set	*/
#define  CSUDI_FD_CLR(fd, fdset)       (CSUDI_FDS_BITS (fdset)[(fd /(8*sizeof ( long)))] &= ~(1UL << ((fd) % (8*sizeof ( long)))))

/**@brief test to see if a descriptor is part of the set	*/
#define  CSUDI_FD_ISSET(fd, fdset)    ((CSUDI_FDS_BITS (fdset)[(fd /(8*sizeof ( long)))] & (1UL << ((fd) % (8*sizeof ( long))))) != 0)

/**@brief clear a set */
#define  CSUDI_FD_ZERO(fdset)  do \
{									      \
  unsigned int i;							      \
  CSUDIFdSet_S *arr = (fdset);						      \
  for (i = 0; i < sizeof (CSUDIFdSet_S) / sizeof ( long); ++i)	      \
  CSUDI_FDS_BITS (arr)[i] = 0;					      \
}while (0)



typedef char *  CSUDI_Caddr_tt;

/**@brief �ӳٹر�socket */
typedef struct
{
	int		l_onoff;               ///< �Ƿ��ӳٹر�		
	int		l_linger;	             ///< �ӳٹرյ�ʱ��

} CSUDILinger_S;


/**@brief ������������� */
typedef struct 
{
  void *         iov_base;	      					///< ����ָ��
  unsigned int iov_len;	            				 	///< ���ݳ���
}CSUDIIovec_S;


/**@brief ��Ϣ���ͺͽ�����Ϣ�����ṹ�� */
typedef struct 
{
  void * msg_name;		       				///< ���ͷ����Ŀ�ĵ�ַ�����շ���ŷ��͵�ַ
  unsigned int       msg_namelen;	       		///< ��ַ���ݳ���
  CSUDIIovec_S *msg_iov;						///< ���������������
  int      msg_iovlen;		       				///< ���������ݳ�Ա����
  void * msg_control;							///< ��������
  unsigned int  msg_controllen;	     	 			///< �������ݴ�С
  int      msg_flags;		       				///< �������ݱ�־
}CSUDIMsghdr_S;	


/**@brief ������Ϣ */
typedef  struct 
{
  char *   h_name;				  			///< �����Ĺ淶��
  char ** h_aliases;			  				///< �����ı���
  int        h_addrtype;				  			///< IP��ַ������
  int        h_length;				  			///< IP��ַ�ĳ���
  char ** h_addr_list;		         				///< ������IP ��ַ 
  #define	h_addr	h_addr_list[0]	  				///< ȡ��һ��IP ��ַ
}CSUDIHostent_S;


/**@brief �������Ϣ  */
typedef struct 
{
  char *  	s_name;							///< ��׼������
  char **	s_aliases;							///< �������
  int      	s_port;								///< �˿ں�
  char *	s_proto;								///< ʹ��Э��
}CSUDIServent_S;


/**@brief Э����Ϣ  */
typedef struct 
{
  char *	p_name;							///< Э������
  char **	p_aliases;							///< Э�����
  int 		p_proto;							///< Э����
}CSUDIProtoent_S;


typedef unsigned int  CSUDIInAddr_t;		///< ���Ͷ���

/**@brief ��ַ��Ϣ�ṹ��*/
typedef struct 
 {
   CSUDIInAddr_t s_addr;
 }CSUDIInAddr_S;


/**@brief �����׽�����Ϣ�ṹ��

@note �˽ṹ�����������CSUDISockAddr_S�ṹ��ͬ�����CSUDISockAddr_Sע��
*/
typedef struct 
{
    unsigned short 	sin_family;   				///< ��ַ��
    unsigned short  	sin_port;   					///< �˿ں�
    CSUDIInAddr_S 	sin_addr;   				//< IP��ַ
    unsigned char 	sin_zero[8];   				///< ���0�Ա�����struct CSUDISockAddr_Sͬ����С
}CSUDISockAddr_IN_S;


/**@brief �����׽�����Ϣ�ṹ��

@note �û�ʹ��CSUDISockAddr_IN_S�ṹ���и�ֵ��ǿ��ת��ΪCSUDISockAddr_S�ṹ��ΪCSUDISOCKConnect�����Ĳ�������
*/
typedef struct 
{
    unsigned short sa_family;   					///< ��ַ��
    char sa_data[14];   							///< 14�ֽڵ�ַ
}CSUDISockAddr_S;




/**@brief IPv6 ��ַ*/
typedef struct 
{
	union 
	{
		unsigned char		u6_addr8[16];
		unsigned short	u6_addr16[8];
		unsigned int		u6_addr32[4];
	} in6_u;
#define s6_addr			in6_u.u6_addr8
#define s6_addr16		in6_u.u6_addr16
#define s6_addr32		in6_u.u6_addr32
}CSUDISockIn6_Addr_S;


extern const CSUDISockIn6_Addr_S CSUDI_IN6ADDR_ANY;        						///<ipv6 addr_any
extern const CSUDISockIn6_Addr_S CSUDI_IN6ADDR_LOOPBACK;   					///<ipv6 loopback


/**@brief ipv6  �����׽�����Ϣ�ṹ��*/
typedef struct  
{
	unsigned short 	sin6_family;    			 ///<ipv6 ��ַ��
	unsigned short	sin6_port;     			 ///<ipv6 �˿ں�
	unsigned int		sin6_flowinfo;  			 ///<ipv6 ����Ϣ
	CSUDISockIn6_Addr_S		sin6_addr;     	 ///<ipv6 ��ַ
	unsigned int			sin6_scope_id;  		 ///<ipv6 socpe id
}CSUDISockAddr_IN6_S;


/**@brief ������˳��鲥*/
typedef struct  
{
     CSUDIInAddr_S  imr_multiaddr;  				///< �鲥��ַ
     CSUDIInAddr_S  imr_interface;  					///< ���ؽӿ�IP��ַ
}CSUDIIp_Mreq_S;


/**@brief ��ָ���ӿڼ�����˳��鲥 */
typedef struct 
{
    CSUDIInAddr_S  imr_multiaddr;				///< �鲥��ַ
    CSUDIInAddr_S  imr_address;				///< ���ؽӿ�IP��ַ
    int			imr_ifindex;					///< �ӿ�����
 }CSUDIIp_Mreqn_S;


/**@brief ipv6 ������˳��鲥*/
typedef struct
{
	unsigned int		imr_multiaddr;			///<�鲥���ַ
	unsigned int		imr_interface;			///<����ӿ�IP��ַ
	unsigned int		imr_sourceaddr;			///<�鲥Դ��ַ
}CSUDIIp_Mreq_Source_S ;


/**@brief socket�ں˵�ַ�ṹ*/
typedef struct
{
	unsigned short	ss_family;								///<��ַ��
	char		data[CSUDIK_SS_MAXSIZE - sizeof(unsigned short)];	///<socket ��ַ��Ϣ
} CSUDIKernel_Sockaddr_Storage_S;	


/**@brief �鲥������*/
typedef struct 
{
	unsigned int				 gr_interface;		///<����ӿ�����
	CSUDIKernel_Sockaddr_Storage_S gr_group;		///<�鲥���ַ
}CSUDIGroup_Req_S;


/**@brief�鲥������*/
typedef struct 
{
	unsigned int				 gsr_interface;		///<����ӿ�����
	CSUDIKernel_Sockaddr_Storage_S gsr_group;	///<�鲥���ַ
	CSUDIKernel_Sockaddr_Storage_S gsr_source;	///<�鲥Դ��ַ
}CSUDIGroup_Source_Req_S;


/**@brief�鲥����*/
typedef struct 
{
	unsigned int				 gf_interface;		///<����ӿ�����
	CSUDIKernel_Sockaddr_Storage_S gf_group;		///<�鲥���ַ
	unsigned int				 gf_fmode;			///<����ģʽ
	unsigned int				 gf_numsrc;			///<�鲥Դ����
	CSUDIKernel_Sockaddr_Storage_S gf_slist[1];		///<����ӿ�����
}CSUDIGroup_Filter_S;



/**@brief ��ʱʱ��ֵ�ṹ��*/
typedef struct 
 {
   long int tv_sec;								///< ��
   long int tv_usec;							///< ΢��,��һ�����֮һ��
 }CSUDITimeval_S;

/**@brief RFCOMM socket address */
typedef struct  {
		unsigned short	addressFamily;
		CSUDIBDAddrr_S	btAddr;
		unsigned char port;
}CSUDISockAddr_RC_S;
#endif

 /**
 @brief �����׽��֡�
 
 @param[in] nDomain ָ��Ҫ�������׽���Э����
 @param[in] nType ָ��Ҫ�������׽�������              
 @param[in] nProtocol ָ������Э�飬ͨ������Ϊ0
 @return �ɹ����ش������׽�����������ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�socket�ӿ�˵����
 */
 int CSUDISOCKSocket(int nDomain,int nType,int nProtocol);


 /**
 @brief ��ָ����������ַ���׽��ֽ��а󶨡�
 
 @param[in] nFd ��Ҫ�󶨵��׽���������
 @param[in] psAddr ���󶨵�������ַ�ṹ���ָ��
 @param[in] uAddrLen ��ַ�ṹ��CSUDISockAddr�ĳ���
 @return �ɹ�����0; ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�bind�ӿ�˵����
 */
 int CSUDISOCKBind(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen);
 

 /**
 @brief �����ӽ��м�����
 
 @param[in] nFd �׽���������
 @param[in] nNum ���ն��е���󳤶�
 @return �ɹ�����0; ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�listen�ӿ�˵����
 */
 int CSUDISOCKListen(int nFd,int nNum);



 /**
 @brief ��ָ����ַ�������ӡ�
 
 @param[in] nFd �׽���������
 @param[in] psAddr ָ������ӵ�ַ�ṹ���ָ��
 @param[in] uAddrLen �ṹ��CSUDISockAddr_S �ĳ���
 @return �ɹ�����0 ; ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�connect�ӿ�˵����
 */
 int CSUDISOCKConnect(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen);
 

  /**
 @brief ��ָ���׽����Ͻ���һ�����ӡ�
 
 @param[in] nFd ���ڼ���״̬���׽���������
 @param[out] psAddr ָ�򱣴����ӷ���ַ�Ľṹ��ָ��
 @param[out] puAddrLen �ṹ��CSUDISockAddr_S �ĳ���
 @return �ɹ��������׽�����������ʧ�ܷ���-1  ��
 @note ��ϸ�����μ�Linux�µ�accept�ӿ�˵����
 */
 int CSUDISOCKAccept(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen);


 /**
 @brief �������ݡ�
 
 @param[in] nFd ���Ͷ��׽���������
 @param[in] pvBuf ���������ݻ�����
 @param[in] uNum �����������ݳ���
 @param[in] nFlags ����ѡ�һ������Ϊ0
 @return �ɹ�����ʵ�ʷ��͵����ݳ���; ʧ�ܷ���-1  ��
 @note ��ϸ�����μ�Linux�µ�send�ӿ�˵����
 */
 int CSUDISOCKSend(int nFd,const void * pvBuf,unsigned int uNum,int nFlags);


 /**
 @brief �������ݡ�
 
 @param[in] nFd ���ն��׽���������
 @param[out] pvBuf ���ڽ������ݵĻ�����
 @param[in] uNum ����������
 @param[in] nFlags ����ѡ�һ����Ϊ0
 @return �ɹ�����ʵ�ʽ��յ��ֽ���; ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�recv�ӿ�˵����
 */
int CSUDISOCKRecv(int nFd,void * pvBuf,unsigned int uNum,int nFlags);


 /**
 @brief �������ݵ�ָ��Ŀ�ĵ�ַ��
 
 @param[in] nFd �׽���������
 @param[in] pvBuf ���������ݵĻ�����
 @param[in] uNum �����������ݵĳ���
 @param[in] nFlags ����ѡ�һ��Ϊ0
 @param[in] psAddr ָ��CSUDISockAddr_S�ṹ���ָ��
 @param[in] uAddrLen CSUDISockAddr_S �ĳ���				                                        
 @return �ɹ�����ʵ�ʷ��ͳ�ȥ�������ֽ���;ʧ�ܷ���-1  ��
 @note ��ϸ�����μ�Linux�µ�sendto�ӿ�˵����
 */
 int CSUDISOCKSendTo(int nFd,const void * pvBuf,unsigned int uNum,int nFlags,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen);



 /**
 @brief ��ָ��Դ��ַ�������ݡ�
 
 @param[in] nFd �׽���������
 @param[out] pvBuf ���ڽ������ݵĻ�����
 @param[in] uNum ����������
 @param[in] nFlags ����ѡ�һ����Ϊ0
 @param[out] psAddr ָ��CSUDISockAddr_S �ṹ���ָ��
 @param[in] puAddrLen CSUDISockAddr_S �ĳ��ȣ��˲�������Ϊ�������
 @return �ɹ����ض�ȡ���ֽ���;ʧ�ܷ���-1 ��
 @note ��ϸ�����μ�Linux�µ�recvfrom�ӿ�˵����
 */
int CSUDISOCKRecvFrom(int nFd,void * pvBuf,unsigned int uNum, int nFlags,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen);


 /**
 @brief ȷ��һ�������׽��ֵ�״̬���ж��׽������Ƿ������ݿɶ����ܷ����׽���д�����ݣ��Ƿ������������
 
 @param[in] nFds ��Ҫ���������׽�������һ
 @param[in] psReadnFds �������ɶ��Ե�һ���׽���������
 @param[in] psWritenFds ��������д�Ե�һ���׽���������
 @param[in] psExceptnFds �����������״̬���׽����ļ���������������������״̬
 @param[in] psTimeout �ȴ�ʱ��
 @return �ɹ�����׼���������׽���������ʱ����0��ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�select�ӿ�˵����
 */
 int CSUDISOCKSelect(int nFds,CSUDIFdSet_S * psReadnFds,CSUDIFdSet_S * psWritenFds,CSUDIFdSet_S * psExceptnFds,CSUDITimeval_S * psTimeout);


 /**
 @brief �����׽����������������׽��ֵ�����, �ο�BSD Socket ��׼���塣
 
 @param[in] nFd ������nCmd ������������
 @param[in] nCmd ��������ɹ�ʹ�õĲ�������μ���׼Linux���塣
 @param[in] nArg1 ����1
 @param[in] nArg2 ����2
 @return �ɹ����ڲ�ͬ�Ĳ��������в�ͬ�ķ���ֵ��ʧ�ܷ���-1��
 */
int CSUDISOCKFcntl(int nFd,int nCmd,int nArg1,int nArg2);


 /**
 @brief �豸���������ж��豸��I/Oͨ�����й����ο�BSD Socket ��׼���塣
 
 @param[in] nFd �׽���������
 @param[in] ulRequest �û��Գ���Ŀ�������ɹ�ʹ�õĲ�������μ���׼Linux���塣
 @param[in] pArg1 ����1
 @param[in] pArg2 ����2
 @return �ɹ����ڲ�ͬ�Ĳ��������в�ͬ�ķ���ֵ��ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�ioctl�ӿ�˵����
 */
 int CSUDISOCKIoctl(int nFd,unsigned int ulRequest,void * pArg1,void * pArg2);


 /**
 @brief ��ȡ�׽���ѡ�
 
 @param[in] nFd �׽���������
 @param[in] nLevel  ѡ���Ĳ�Σ���ȡֵ�� CSUDI_SOL_SOCKET��CSUDI_SOL_TCP��CSUDI_SOL_UDP�� CSUDI_SOL_IP
 @param[in] nOptName Ҫ��ȡ��ѡ����
 @param[out] pvOptVal ָ�򱣴�ѡ��ֵ�Ļ�����
 @param[in] puOptLen ����ѡ��ֵ�������ĳ��ȣ��˲�������Ϊ�������
 @return �ɹ�����0��ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�getsockopt�ӿ�˵����
 */
 int CSUDISOCKGetSockOpt(int nFd,int nLevel,int nOptName,void * pvOptVal,unsigned int * puOptLen);


 /**
 @brief �����׽���ѡ�
 
 @param[in] nFd �׽���������
 @param[in] nLevel  ѡ���Ĳ�Σ���ȡֵ�� CSUDI_SOL_SOCKET��CSUDI_SOL_TCP��CSUDI_SOL_UDP�� CSUDI_SOL_IP
 @param[in] nOptName ��Ҫ���õ�ѡ����
 @param[in] pvOptVal ָ�򱣴�ѡ��ֵ�Ļ�����
 @param[in] uOptLen  ����ѡ��ֵ�������ĳ���
 @return �ɹ�����0��ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�setsockopt�ӿ�˵����
 */
 int CSUDISOCKSetSockOpt(int nFd,int nLevel,int nOptName,const void * pvOptVal, unsigned int uOptLen);


 /**
 @brief ����Э�����ƻ�ȡЭ���������Ϣ��
 
 @param[in] pcProtoName Э������
 @return �ɹ�����Э��CSUDIProtoent_S�ṹ����Ϣ��ʧ�ܷ���CSUDI_NULL��
 @note ��ϸ�����μ�Linux�µ�getprotobyname�ӿ�˵����
 */
CSUDIProtoent_S * CSUDISOCKGetProtoByName(const char * pcProtoName);

 
 /**
 @brief ���ݷ�������Э������ȡ������Ϣ��
 
 @param[in] pcServName ��������
 @param[in] pcProtoName ����ʹ�õ�Э��
 @return �ɹ����ط���CSUDIServent_S �ṹ����Ϣ��ʧ�ܷ���CSUDI_NULL��
 @note ��ϸ�����μ�Linux�µ�getservbyname�ӿ�˵����
 */
CSUDIServent_S * CSUDISOCKGetServByName(const char * pcServName,const char * pcProtoName);


 /**
 @brief ��ȡ���׽�����ص�Զ�˵�ַ��

 @param[in] nFd ��ʶ�����ӵ��׽���������
 @param[out] psAddr �ṹ��CSUDISockAddr_S ��ָ�룬�������ӵ���һ������Ϣ
 @param[in] puAddrLen ָ��sizeof(CSUDISockAddr_S)��ָ�룬��ָ������Ϊ�������
 @return �ɹ�����0 ,ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�getpeername�ӿ�˵����
 */
 int CSUDISOCKGetPeerName(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen);


 /**
 @brief ��ȡ���׽�����صı��ص�ַ��
 
 @param[in] nFd �׽���������
 @param[out] psAddr ָ������׽��ֵ�ַ
 @param[in] puAddrLen ��Ϊ�������ʱ��Ϊ�ռ����󳤶ȡ���Ϊ�������ʱ��Ϊ��������ʵ�ʴ�С�����ֽ�Ϊ��λ��
 @return �ɹ�����0,ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�getsockname�ӿ�˵����
 */
 int CSUDISOCKGetSockName(int nFd,CSUDISockAddr_S * psAddr, unsigned int * puAddrLen);


 /**
 @brief ��ȡ������������������

 @param[out] pcHostName ���ձ����������Ļ�����
 @param[in] uAddrLen �������ĳ���
 @return �ɹ�����0��ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�gethostname�ӿ�˵����
 */
 int CSUDISOCKGetHostName(char * pcHostName,unsigned int uAddrLen);


 /**
 @brief ������������ȡ������������Ϣ��
 
 @param[in] pcHostName ������
 @return �ɹ����ػ�ȡ������CSUDIHostent_S�ṹ����Ϣ��ʧ�ܷ���CSUDI_NULL
 @note ��ϸ�����μ�Linux�µ�gethostbyname�ӿ�˵����
 @note �ýӿ��ѷ������м������ʹ��
 */
 CSUDIHostent_S * CSUDISOCKGetHostByName(const char * pcHostName);


/**
 @brief ������������ȡ������������Ϣ��
 
 @param[in] pcHostName ������
 @param[in] psResultBuf ָ��һ�������ṹ
 @param[in] pcBuf ָ��һ�����棬��ָ������Ϊ���
 @param[in] uBufLen ��������С���˲�������Ϊ���
 @param[out] psResult �����ɹ����ú�ָ���ȡ�������ṹCSUDIHostent_S
 @param[out] pnErrCode ������
 @return �ɹ�����0��ʧ�ܷ��ش���š�
 @note ��ϸ�����μ�Linux�µ�gethostbyname_r�ӿ�˵����
 @note �ýӿ��ѷ������м������ʹ��
 */
int CSUDISOCKGetHostByName_r(const char * pcHostName,CSUDIHostent_S * psResultBuf,char * pcBuf,unsigned int uBufLen,CSUDIHostent_S ** psResult,int * pnErrCode);


 /**
 @brief ���ݸ����ĵ�ַ��ȡ������������Ϣ��
 
 @param[in] pvAddr ������ַ(IP��ַ�����ֽ���)
 @param[in] uAddrLen ��ַ�ĳ���
 @param[in] nType ��ַ����
 @return �ɹ����ػ�ȡ������CSUDIHostent_S �ṹ����Ϣ��ʧ�ܷ���CSUDI_NULL
 @note ��ϸ�����μ�Linux�µ�gethostbyaddr�ӿ�˵����
 */
CSUDIHostent_S * CSUDISOCKGetHostByAddr(const void * pvAddr,unsigned int uAddrLen,int nType);


  /**
 @brief �ѵ��IP��ַת��Ϊ�����ֽ���������ַ��
 @param[in] pcIp ��Ҫת���ı�׼���IP��ַ�ַ���,����"192.168.0.1"
 @note ��IP��255.255.255.255ʱ������CSUDIInet_addr()����Ϊ���Ǹ���Ч��IP��ַ
 @return �ɹ������޷���������ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�inet_addr�ӿ�˵����
 */
CSUDIInAddr_t CSUDISOCKInet_addr(const char * pcIp);


 /**
 @brief �ѵ��IP��ַת��Ϊ�����ֽ���������ַ��
 
 @param[in] pcIp ��Ҫת���ĵ��IP��ַ
 @param[out] psIpNet IP��ַ�ṹ��
 @return �ɹ����ط�0��ʧ�ܷ���0��
 @note ��ϸ�����μ�Linux�µ�inet_aton�ӿ�˵����
 */
 CSUDIInAddr_t CSUDISOCKInet_aton(const char * pcIp, CSUDIInAddr_S * psIpNet);

 
 /**
 @brief �������ֽ����ַת��Ϊ���IP��ַ��
 
 @param[in] sIpNet �����ֽ����ַ
 @return ת����ĵ��IP ��ַ�ַ�����ʧ�ܷ���CSUDI_NULL��
 @note ��ϸ�����μ�Linux�µ�inet_ntoa�ӿ�˵����
 */
 char * CSUDISOCKInet_ntoa(CSUDIInAddr_S sIpNet);


 
 /**
 @brief ���ַ���ת���������ַ��
 
 @param[in] nAf ��ַ��
 @param[in] pSrc �ַ��͵�ַ
 @param[out] pDst ��ַת��Ϊin_addr�Ľṹ�壬��������pDst��
 @return �ɹ�����1��ʧ�ܷ���0
 @note ��ϸ�����μ�Linux�µ�inet_pton�ӿ�˵����
 */
 int CSUDISOCKInet_pton(int nAf,const char *pSrc, void *pDst);


  
 /**
 @brief ����������ƽṹ��ַת�����ַ�����ַ��
 
 @param[in] nAf ��ַ��
 @param[in] pSrc �����ַ
 @param[out] pDst �ַ��͵�ַ
 @param[in] nDsrLen pDst ������
 @return �ɹ�����ת������ַ�����ַ��ʧ�ܷ���CSUDI_NULL
 @note ��ϸ�����μ�Linux�µ�inet_ntop�ӿ�˵����
 */
 const char * CSUDISOCKInet_ntop(int nAf,const void *pSrc,char *pDst,int nDsrLen);



 /**
 @brief ��short�͵������ֽ���ת��Ϊ�����ֽ���
 
 @param[in] ushNet �����ֽ����16bit����
 @return �����ֽ���ֵ��
 @note ��ϸ�����μ�Linux�µ�ntohs�ӿ�˵����
 */
unsigned short CSUDISOCKNtohs(unsigned short ushNet);


 /**
 @brief ��short�͵������ֽ���ת��Ϊ�����ֽ���
 
 @param[in] ushHost �����ֽ����16bit����
 @return �����ֽ���ֵ
 @note ��ϸ�����μ�Linux�µ�htons�ӿ�˵����
 */
unsigned short CSUDISOCKHtons(unsigned short ushHost);
  

 /**
 @brief ��32λ�������ֽ���ת��Ϊ�����ֽ���
 
 @param[in] uNet �����ֽ����32bit����
 @return �����ֽ���ֵ
 @note ��ϸ�����μ�Linux�µ�ntohl�ӿ�˵����
 */
unsigned int CSUDISOCKNtohl(unsigned int uNet);
 

 /**
 @brief ��32λ�������ֽ���ת��Ϊ�����ֽ���
 
 @param[in] uHost �����ֽ����32bit����
 @return �����ֽ���ֵ
 @note ��ϸ�����μ�Linux�µ�htonl�ӿ�˵����
 */
unsigned int CSUDISOCKHtonl(unsigned int uHost);


 /**
 @brief  ��ָ���׽����Ͻ�ֹ���ݵĽ��ա����ͻ�ͬʱ��ֹ���ݽ��պͷ��͡�
 
 @param[in] nFd ��Ҫ�رյ��׽���������
 @param[in] nHow �رշ�ʽ 
 @return �ɹ�����0��ʧ�ܷ���-1��
 @note nHow=0:��ֹ���ݽ���
 @note nHow=1:��ֹ���ݷ���
 @note nHow=2:ͬʱ��ֹ���ݷ��ͺͽ���
 */
 int CSUDISOCKShutDown(int nFd,int nHow);


 /**
 @brief �ر�ָ�����׽��֡�
 
 @param[in] nFd �׽���������
 @return �ɹ�����0��ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�close�ӿ�˵����
 */
int CSUDISOCKClose(int nFd);


 /**@brief ��ȡ����š�

 @return ���ش����,���udi2_sockerror.hͷ�ļ����塣
 */
int CSUDISOCKGetLastError(void);


#ifdef __cplusplus
}
#endif
/** @} */
#endif

