/**@defgroup SOCKET SOCKET 本模块对SOCKET 模块中用到的数据类型和数据进行定义，以及对调用接口函数进行说明
@brief  Socket模块对应头文件udi2_socket.h，其内容基本相当于Linux的Socket接口，
如果在实现时有任何问题，除非接口文档中特别说明，否则均可参考Linux的相关文档。

@version 2.0.2 2009/08/24 综合评审后的版本
@version 2.0.1 2009/08/12 演示版本
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


#pragma pack(1)//1字节对齐
/* BD Address */
typedef struct {
	unsigned char b[6];
} CSUDIBDAddrr_S;
#pragma pack()//恢复对齐状态

#define CSUDI_BDADDR_ANY   (&(CSUDIBDAddrr_S) {{0, 0, 0, 0, 0, 0}})
#define CSUDI_BDADDR_ALL   (&(CSUDIBDAddrr_S) {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}})
#define CSUDI_BDADDR_LOCAL (&(CSUDIBDAddrr_S) {{0, 0, 0, 0xff, 0xff, 0xff}})


#define CSUDISockaddr_Storage_S CSUDIKernel_Sockaddr_Storage_S
#define CSUDIK_SS_MAXSIZE	128												///< Implementation specific max size 


#ifdef WIN32

#include <WinSock.h>
typedef unsigned long CSUDIInAddr_t;


/**@brief 保存待检测的套接字句柄 */
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


/**@brief 延迟关闭socket */
typedef struct linger CSUDILinger_S;


/**@brief 输入输出缓存区 */
typedef struct iovec CSUDIIovec_S;


/**@brief 消息发送和接收信息描述结构体 */
typedef struct msghdr CSUDIMsghdr_S;


/**@brief 主机信息 */
typedef struct hostent CSUDIHostent_S;


/**@brief 服务端信息  */
typedef struct servent CSUDIServent_S;


/**@brief 协议信息  */
typedef struct protoent CSUDIProtoent_S;


/**@brief 地址信息结构体*/
typedef struct in_addr CSUDIInAddr_S;


/**@brief 保存套接字信息结构体

@note 此结构保存的数据与CSUDISockAddr_S结构相同，详见CSUDISockAddr_S注释
*/
typedef struct sockaddr_in CSUDISockAddr_IN_S;


/**@brief 保存套接字信息结构体

@note 用户使用CSUDISockAddr_IN_S结构进行赋值后，强制转换为CSUDISockAddr_S结构作为CSUDISOCKConnect函数的参数传入
*/
typedef struct sockaddr CSUDISockAddr_S;


/**@brief 加入或退出组播*/
typedef struct ip_mreq CSUDIIp_Mreq_S;


/**@brief 在指定接口加入或退出组播 */
//#define CSUDIIp_Mreqn_S ip_mreqn


/**@brief 超时时间值结构体*/
typedef struct timeval CSUDITimeval_S;


//////////////////////////////////////////////////////////////////////////////////////////begin

/**@brief IPv6 地址*/
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


/**@brief ipv6  保存套接字信息结构体*/
typedef struct  
{
	unsigned short 	sin6_family;    			 ///<ipv6 地址族
	unsigned short	sin6_port;     			 ///<ipv6 端口号
	unsigned int		sin6_flowinfo;  			 ///<ipv6 流信息
	CSUDISockIn6_Addr_S		sin6_addr;     	 ///<ipv6 地址
	unsigned int			sin6_scope_id;  		 ///<ipv6 socpe id
}CSUDISockAddr_IN6_S;

/**@brief ipv6 加入或退出组播*/
typedef struct
{
	unsigned int		imr_multiaddr;			///<组播组地址
	unsigned int		imr_interface;			///<网络接口IP地址
	unsigned int		imr_sourceaddr;			///<组播源地址
}CSUDIIp_Mreq_Source_S ;

/**@brief socket内核地址结构*/
typedef struct
{
	unsigned short	ss_family;								///<地址族
	char		data[CSUDIK_SS_MAXSIZE - sizeof(unsigned short)];	///<socket 地址信息
} CSUDIKernel_Sockaddr_Storage_S;

/**@brief 组播组请求*/
typedef struct 
{
	unsigned int				 gr_interface;		///<网络接口索引
	CSUDIKernel_Sockaddr_Storage_S gr_group;		///<组播组地址
}CSUDIGroup_Req_S;

/**@brief组播过滤*/
typedef struct 
{
	unsigned int				 gf_interface;		///<网络接口索引
	CSUDIKernel_Sockaddr_Storage_S gf_group;		///<组播组地址
	unsigned int				 gf_fmode;			///<过滤模式
	unsigned int				 gf_numsrc;			///<组播源个数
	CSUDIKernel_Sockaddr_Storage_S gf_slist[1];		///<网络接口索引
}CSUDIGroup_Filter_S;

/**@brief组播组请求*/
typedef struct 
{
	unsigned int				 gsr_interface;		///<网络接口索引
	CSUDIKernel_Sockaddr_Storage_S gsr_group;	///<组播组地址
	CSUDIKernel_Sockaddr_Storage_S gsr_source;	///<组播源地址
}CSUDIGroup_Source_Req_S;
////////////////////////////////////////////////////////////////////////////////////////////end



#else

/**@brief 保存待检测的套接字句柄 */
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

/**@brief 延迟关闭socket */
typedef struct
{
	int		l_onoff;               ///< 是否延迟关闭		
	int		l_linger;	             ///< 延迟关闭的时间

} CSUDILinger_S;


/**@brief 输入输出缓存区 */
typedef struct 
{
  void *         iov_base;	      					///< 数据指针
  unsigned int iov_len;	            				 	///< 数据长度
}CSUDIIovec_S;


/**@brief 消息发送和接收信息描述结构体 */
typedef struct 
{
  void * msg_name;		       				///< 发送方存放目的地址，接收方存放发送地址
  unsigned int       msg_namelen;	       		///< 地址数据长度
  CSUDIIovec_S *msg_iov;						///< 输入输出缓存数据
  int      msg_iovlen;		       				///< 缓存中数据成员个数
  void * msg_control;							///< 辅助数据
  unsigned int  msg_controllen;	     	 			///< 辅助数据大小
  int      msg_flags;		       				///< 接收数据标志
}CSUDIMsghdr_S;	


/**@brief 主机信息 */
typedef  struct 
{
  char *   h_name;				  			///< 主机的规范名
  char ** h_aliases;			  				///< 主机的别名
  int        h_addrtype;				  			///< IP地址的类型
  int        h_length;				  			///< IP地址的长度
  char ** h_addr_list;		         				///< 主机的IP 地址 
  #define	h_addr	h_addr_list[0]	  				///< 取第一个IP 地址
}CSUDIHostent_S;


/**@brief 服务端信息  */
typedef struct 
{
  char *  	s_name;							///< 标准服务名
  char **	s_aliases;							///< 服务别名
  int      	s_port;								///< 端口号
  char *	s_proto;								///< 使用协议
}CSUDIServent_S;


/**@brief 协议信息  */
typedef struct 
{
  char *	p_name;							///< 协议名称
  char **	p_aliases;							///< 协议别名
  int 		p_proto;							///< 协议编号
}CSUDIProtoent_S;


typedef unsigned int  CSUDIInAddr_t;		///< 类型定义

/**@brief 地址信息结构体*/
typedef struct 
 {
   CSUDIInAddr_t s_addr;
 }CSUDIInAddr_S;


/**@brief 保存套接字信息结构体

@note 此结构保存的数据与CSUDISockAddr_S结构相同，详见CSUDISockAddr_S注释
*/
typedef struct 
{
    unsigned short 	sin_family;   				///< 地址族
    unsigned short  	sin_port;   					///< 端口号
    CSUDIInAddr_S 	sin_addr;   				//< IP地址
    unsigned char 	sin_zero[8];   				///< 填充0以保持与struct CSUDISockAddr_S同样大小
}CSUDISockAddr_IN_S;


/**@brief 保存套接字信息结构体

@note 用户使用CSUDISockAddr_IN_S结构进行赋值后，强制转换为CSUDISockAddr_S结构作为CSUDISOCKConnect函数的参数传入
*/
typedef struct 
{
    unsigned short sa_family;   					///< 地址族
    char sa_data[14];   							///< 14字节地址
}CSUDISockAddr_S;




/**@brief IPv6 地址*/
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


/**@brief ipv6  保存套接字信息结构体*/
typedef struct  
{
	unsigned short 	sin6_family;    			 ///<ipv6 地址族
	unsigned short	sin6_port;     			 ///<ipv6 端口号
	unsigned int		sin6_flowinfo;  			 ///<ipv6 流信息
	CSUDISockIn6_Addr_S		sin6_addr;     	 ///<ipv6 地址
	unsigned int			sin6_scope_id;  		 ///<ipv6 socpe id
}CSUDISockAddr_IN6_S;


/**@brief 加入或退出组播*/
typedef struct  
{
     CSUDIInAddr_S  imr_multiaddr;  				///< 组播地址
     CSUDIInAddr_S  imr_interface;  					///< 本地接口IP地址
}CSUDIIp_Mreq_S;


/**@brief 在指定接口加入或退出组播 */
typedef struct 
{
    CSUDIInAddr_S  imr_multiaddr;				///< 组播地址
    CSUDIInAddr_S  imr_address;				///< 本地接口IP地址
    int			imr_ifindex;					///< 接口索引
 }CSUDIIp_Mreqn_S;


/**@brief ipv6 加入或退出组播*/
typedef struct
{
	unsigned int		imr_multiaddr;			///<组播组地址
	unsigned int		imr_interface;			///<网络接口IP地址
	unsigned int		imr_sourceaddr;			///<组播源地址
}CSUDIIp_Mreq_Source_S ;


/**@brief socket内核地址结构*/
typedef struct
{
	unsigned short	ss_family;								///<地址族
	char		data[CSUDIK_SS_MAXSIZE - sizeof(unsigned short)];	///<socket 地址信息
} CSUDIKernel_Sockaddr_Storage_S;	


/**@brief 组播组请求*/
typedef struct 
{
	unsigned int				 gr_interface;		///<网络接口索引
	CSUDIKernel_Sockaddr_Storage_S gr_group;		///<组播组地址
}CSUDIGroup_Req_S;


/**@brief组播组请求*/
typedef struct 
{
	unsigned int				 gsr_interface;		///<网络接口索引
	CSUDIKernel_Sockaddr_Storage_S gsr_group;	///<组播组地址
	CSUDIKernel_Sockaddr_Storage_S gsr_source;	///<组播源地址
}CSUDIGroup_Source_Req_S;


/**@brief组播过滤*/
typedef struct 
{
	unsigned int				 gf_interface;		///<网络接口索引
	CSUDIKernel_Sockaddr_Storage_S gf_group;		///<组播组地址
	unsigned int				 gf_fmode;			///<过滤模式
	unsigned int				 gf_numsrc;			///<组播源个数
	CSUDIKernel_Sockaddr_Storage_S gf_slist[1];		///<网络接口索引
}CSUDIGroup_Filter_S;



/**@brief 超时时间值结构体*/
typedef struct 
 {
   long int tv_sec;								///< 秒
   long int tv_usec;							///< 微秒,即一百万分之一秒
 }CSUDITimeval_S;

/**@brief RFCOMM socket address */
typedef struct  {
		unsigned short	addressFamily;
		CSUDIBDAddrr_S	btAddr;
		unsigned char port;
}CSUDISockAddr_RC_S;
#endif

 /**
 @brief 创建套接字。
 
 @param[in] nDomain 指定要创建的套接字协议族
 @param[in] nType 指定要创建的套接字类型              
 @param[in] nProtocol 指定哪种协议，通常设置为0
 @return 成功返回创建的套接字描述符；失败返回-1。
 @note 详细描述参见Linux下的socket接口说明。
 */
 int CSUDISOCKSocket(int nDomain,int nType,int nProtocol);


 /**
 @brief 将指定的主机地址与套接字进行绑定。
 
 @param[in] nFd 需要绑定的套接字描述符
 @param[in] psAddr 待绑定的主机地址结构体的指针
 @param[in] uAddrLen 地址结构体CSUDISockAddr的长度
 @return 成功返回0; 失败返回-1。
 @note 详细描述参见Linux下的bind接口说明。
 */
 int CSUDISOCKBind(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen);
 

 /**
 @brief 对连接进行监听。
 
 @param[in] nFd 套接字描述符
 @param[in] nNum 接收队列的最大长度
 @return 成功返回0; 失败返回-1。
 @note 详细描述参见Linux下的listen接口说明。
 */
 int CSUDISOCKListen(int nFd,int nNum);



 /**
 @brief 向指定地址发起连接。
 
 @param[in] nFd 套接字描述符
 @param[in] psAddr 指向待连接地址结构体的指针
 @param[in] uAddrLen 结构体CSUDISockAddr_S 的长度
 @return 成功返回0 ; 失败返回-1。
 @note 详细描述参见Linux下的connect接口说明。
 */
 int CSUDISOCKConnect(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen);
 

  /**
 @brief 在指定套接字上接收一个连接。
 
 @param[in] nFd 处于监听状态的套接字描述符
 @param[out] psAddr 指向保存连接方地址的结构体指针
 @param[out] puAddrLen 结构体CSUDISockAddr_S 的长度
 @return 成功返回新套接字描述符；失败返回-1  。
 @note 详细描述参见Linux下的accept接口说明。
 */
 int CSUDISOCKAccept(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen);


 /**
 @brief 发送数据。
 
 @param[in] nFd 发送端套接字描述符
 @param[in] pvBuf 待发送数据缓存区
 @param[in] uNum 缓存区中数据长度
 @param[in] nFlags 发送选项，一般设置为0
 @return 成功返回实际发送的数据长度; 失败返回-1  。
 @note 详细描述参见Linux下的send接口说明。
 */
 int CSUDISOCKSend(int nFd,const void * pvBuf,unsigned int uNum,int nFlags);


 /**
 @brief 接收数据。
 
 @param[in] nFd 接收端套接字描述符
 @param[out] pvBuf 用于接收数据的缓存区
 @param[in] uNum 缓存区长度
 @param[in] nFlags 操作选项，一般置为0
 @return 成功返回实际接收的字节数; 失败返回-1。
 @note 详细描述参见Linux下的recv接口说明。
 */
int CSUDISOCKRecv(int nFd,void * pvBuf,unsigned int uNum,int nFlags);


 /**
 @brief 发送数据到指定目的地址。
 
 @param[in] nFd 套接字描述符
 @param[in] pvBuf 待发送数据的缓存区
 @param[in] uNum 缓存区中数据的长度
 @param[in] nFlags 操作选项，一般为0
 @param[in] psAddr 指向CSUDISockAddr_S结构体的指针
 @param[in] uAddrLen CSUDISockAddr_S 的长度				                                        
 @return 成功返回实际发送出去的数据字节数;失败返回-1  。
 @note 详细描述参见Linux下的sendto接口说明。
 */
 int CSUDISOCKSendTo(int nFd,const void * pvBuf,unsigned int uNum,int nFlags,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen);



 /**
 @brief 从指定源地址接收数据。
 
 @param[in] nFd 套接字描述符
 @param[out] pvBuf 用于接收数据的缓存区
 @param[in] uNum 缓存区长度
 @param[in] nFlags 操作选项，一般置为0
 @param[out] psAddr 指向CSUDISockAddr_S 结构体的指针
 @param[in] puAddrLen CSUDISockAddr_S 的长度，此参数亦作为输出参数
 @return 成功返回读取的字节数;失败返回-1 。
 @note 详细描述参见Linux下的recvfrom接口说明。
 */
int CSUDISOCKRecvFrom(int nFd,void * pvBuf,unsigned int uNum, int nFlags,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen);


 /**
 @brief 确定一个或多个套接字的状态，判断套接字上是否有数据可读，能否向套接字写入数据，是否有意外情况。
 
 @param[in] nFds 需要管理的最大套接字数加一
 @param[in] psReadnFds 用来检查可读性的一组套接字描述符
 @param[in] psWritenFds 用来检查可写性的一组套接字描述符
 @param[in] psExceptnFds 用来检查意外状态的套接字文件描述符，错误不属于意外状态
 @param[in] psTimeout 等待时间
 @return 成功返回准备就绪的套接字数；超时返回0；失败返回错误码。
 @note 详细描述参见Linux下的select接口说明。
 */
 int CSUDISOCKSelect(int nFds,CSUDIFdSet_S * psReadnFds,CSUDIFdSet_S * psWritenFds,CSUDIFdSet_S * psExceptnFds,CSUDITimeval_S * psTimeout);


 /**
 @brief 根据套接字描述符来操作套接字的属性, 参考BSD Socket 标准定义。
 
 @param[in] nFd 被参数nCmd 操作的描述符
 @param[in] nCmd 操作命令，可供使用的操作命令参见标准Linux定义。
 @param[in] nArg1 参数1
 @param[in] nArg2 参数2
 @return 成功对于不同的操作命令有不同的返回值；失败返回-1。
 */
int CSUDISOCKFcntl(int nFd,int nCmd,int nArg1,int nArg2);


 /**
 @brief 设备驱动程序中对设备的I/O通道进行管理，参考BSD Socket 标准定义。
 
 @param[in] nFd 套接字描述符
 @param[in] ulRequest 用户对程序的控制命令，可供使用的操作命令参见标准Linux定义。
 @param[in] pArg1 参数1
 @param[in] pArg2 参数2
 @return 成功对于不同的操作命令有不同的返回值；失败返回-1。
 @note 详细描述参见Linux下的ioctl接口说明。
 */
 int CSUDISOCKIoctl(int nFd,unsigned int ulRequest,void * pArg1,void * pArg2);


 /**
 @brief 获取套接字选项。
 
 @param[in] nFd 套接字描述符
 @param[in] nLevel  选项定义的层次，可取值有 CSUDI_SOL_SOCKET，CSUDI_SOL_TCP，CSUDI_SOL_UDP， CSUDI_SOL_IP
 @param[in] nOptName 要获取的选项名
 @param[out] pvOptVal 指向保存选项值的缓存区
 @param[in] puOptLen 保存选项值缓存区的长度，此参数亦作为输出参数
 @return 成功返回0；失败返回错误码。
 @note 详细描述参见Linux下的getsockopt接口说明。
 */
 int CSUDISOCKGetSockOpt(int nFd,int nLevel,int nOptName,void * pvOptVal,unsigned int * puOptLen);


 /**
 @brief 设置套接字选项。
 
 @param[in] nFd 套接字描述符
 @param[in] nLevel  选项定义的层次，可取值有 CSUDI_SOL_SOCKET，CSUDI_SOL_TCP，CSUDI_SOL_UDP， CSUDI_SOL_IP
 @param[in] nOptName 需要设置的选项名
 @param[in] pvOptVal 指向保存选项值的缓存区
 @param[in] uOptLen  保存选项值缓存区的长度
 @return 成功返回0；失败返回错误码。
 @note 详细描述参见Linux下的setsockopt接口说明。
 */
 int CSUDISOCKSetSockOpt(int nFd,int nLevel,int nOptName,const void * pvOptVal, unsigned int uOptLen);


 /**
 @brief 根据协议名称获取协议的其它信息。
 
 @param[in] pcProtoName 协议名称
 @return 成功返回协议CSUDIProtoent_S结构体信息；失败返回CSUDI_NULL。
 @note 详细描述参见Linux下的getprotobyname接口说明。
 */
CSUDIProtoent_S * CSUDISOCKGetProtoByName(const char * pcProtoName);

 
 /**
 @brief 根据服务名和协议名获取服务信息。
 
 @param[in] pcServName 服务名称
 @param[in] pcProtoName 服务使用的协议
 @return 成功返回服务CSUDIServent_S 结构体信息；失败返回CSUDI_NULL。
 @note 详细描述参见Linux下的getservbyname接口说明。
 */
CSUDIServent_S * CSUDISOCKGetServByName(const char * pcServName,const char * pcProtoName);


 /**
 @brief 获取与套接字相关的远端地址。

 @param[in] nFd 标识已连接的套接字描述符
 @param[out] psAddr 结构体CSUDISockAddr_S 的指针，保存连接的另一方的信息
 @param[in] puAddrLen 指向sizeof(CSUDISockAddr_S)的指针，此指针亦作为输出参数
 @return 成功返回0 ,失败返回错误码。
 @note 详细描述参见Linux下的getpeername接口说明。
 */
 int CSUDISOCKGetPeerName(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen);


 /**
 @brief 获取与套接字相关的本地地址。
 
 @param[in] nFd 套接字描述符
 @param[out] psAddr 指向接收套接字地址
 @param[in] puAddrLen 作为输入参数时，为空间的最大长度。作为输出参数时，为缓存区的实际大小，以字节为单位。
 @return 成功返回0,失败返回错误码。
 @note 详细描述参见Linux下的getsockname接口说明。
 */
 int CSUDISOCKGetSockName(int nFd,CSUDISockAddr_S * psAddr, unsigned int * puAddrLen);


 /**
 @brief 获取本地主机的主机名。

 @param[out] pcHostName 接收本地主机名的缓存区
 @param[in] uAddrLen 缓存区的长度
 @return 成功返回0；失败返回错误码。
 @note 详细描述参见Linux下的gethostname接口说明。
 */
 int CSUDISOCKGetHostName(char * pcHostName,unsigned int uAddrLen);


 /**
 @brief 根据主机名获取主机的其它信息。
 
 @param[in] pcHostName 主机名
 @return 成功返回获取的主机CSUDIHostent_S结构体信息；失败返回CSUDI_NULL
 @note 详细描述参见Linux下的gethostbyname接口说明。
 @note 该接口已废弃，中间件不再使用
 */
 CSUDIHostent_S * CSUDISOCKGetHostByName(const char * pcHostName);


/**
 @brief 根据主机名获取主机的其它信息。
 
 @param[in] pcHostName 主机名
 @param[in] psResultBuf 指向一个主机结构
 @param[in] pcBuf 指向一个缓存，此指针亦作为输出
 @param[in] uBufLen 缓存区大小，此参数亦作为输出
 @param[out] psResult 函数成功调用后指向获取的主机结构CSUDIHostent_S
 @param[out] pnErrCode 错误码
 @return 成功返回0；失败返回错误号。
 @note 详细描述参见Linux下的gethostbyname_r接口说明。
 @note 该接口已废弃，中间件不再使用
 */
int CSUDISOCKGetHostByName_r(const char * pcHostName,CSUDIHostent_S * psResultBuf,char * pcBuf,unsigned int uBufLen,CSUDIHostent_S ** psResult,int * pnErrCode);


 /**
 @brief 根据给定的地址获取主机的其它信息。
 
 @param[in] pvAddr 主机地址(IP地址网络字节序)
 @param[in] uAddrLen 地址的长度
 @param[in] nType 地址类型
 @return 成功返回获取的主机CSUDIHostent_S 结构体信息；失败返回CSUDI_NULL
 @note 详细描述参见Linux下的gethostbyaddr接口说明。
 */
CSUDIHostent_S * CSUDISOCKGetHostByAddr(const void * pvAddr,unsigned int uAddrLen,int nType);


  /**
 @brief 把点分IP地址转换为网络字节序的网络地址。
 @param[in] pcIp 需要转化的标准点分IP地址字符串,例如"192.168.0.1"
 @note 当IP是255.255.255.255时，函数CSUDIInet_addr()会认为这是个无效的IP地址
 @return 成功返回无符号整数；失败返回-1。
 @note 详细描述参见Linux下的inet_addr接口说明。
 */
CSUDIInAddr_t CSUDISOCKInet_addr(const char * pcIp);


 /**
 @brief 把点分IP地址转换为网络字节序的网络地址。
 
 @param[in] pcIp 需要转化的点分IP地址
 @param[out] psIpNet IP地址结构体
 @return 成功返回非0；失败返回0。
 @note 详细描述参见Linux下的inet_aton接口说明。
 */
 CSUDIInAddr_t CSUDISOCKInet_aton(const char * pcIp, CSUDIInAddr_S * psIpNet);

 
 /**
 @brief 把网络字节序地址转换为点分IP地址。
 
 @param[in] sIpNet 网络字节序地址
 @return 转换后的点分IP 地址字符串；失败返回CSUDI_NULL。
 @note 详细描述参见Linux下的inet_ntoa接口说明。
 */
 char * CSUDISOCKInet_ntoa(CSUDIInAddr_S sIpNet);


 
 /**
 @brief 把字符串转换成网络地址。
 
 @param[in] nAf 地址族
 @param[in] pSrc 字符型地址
 @param[out] pDst 地址转换为in_addr的结构体，并复制在pDst中
 @return 成功返回1；失败返回0
 @note 详细描述参见Linux下的inet_pton接口说明。
 */
 int CSUDISOCKInet_pton(int nAf,const char *pSrc, void *pDst);


  
 /**
 @brief 把网络二进制结构地址转换成字符串地址。
 
 @param[in] nAf 地址族
 @param[in] pSrc 网络地址
 @param[out] pDst 字符型地址
 @param[in] nDsrLen pDst 缓存区
 @return 成功返回转换后的字符串地址；失败返回CSUDI_NULL
 @note 详细描述参见Linux下的inet_ntop接口说明。
 */
 const char * CSUDISOCKInet_ntop(int nAf,const void *pSrc,char *pDst,int nDsrLen);



 /**
 @brief 把short型的网络字节序转化为主机字节序。
 
 @param[in] ushNet 网络字节序的16bit数据
 @return 主机字节序值。
 @note 详细描述参见Linux下的ntohs接口说明。
 */
unsigned short CSUDISOCKNtohs(unsigned short ushNet);


 /**
 @brief 把short型的主机字节序转换为网络字节序。
 
 @param[in] ushHost 主机字节序的16bit数据
 @return 网络字节序值
 @note 详细描述参见Linux下的htons接口说明。
 */
unsigned short CSUDISOCKHtons(unsigned short ushHost);
  

 /**
 @brief 把32位的网络字节序转化为主机字节序。
 
 @param[in] uNet 网络字节序的32bit数据
 @return 主机字节序值
 @note 详细描述参见Linux下的ntohl接口说明。
 */
unsigned int CSUDISOCKNtohl(unsigned int uNet);
 

 /**
 @brief 把32位的主机字节序转换为网络字节序。
 
 @param[in] uHost 主机字节序的32bit数据
 @return 网络字节序值
 @note 详细描述参见Linux下的htonl接口说明。
 */
unsigned int CSUDISOCKHtonl(unsigned int uHost);


 /**
 @brief  在指定套接字上禁止数据的接收、发送或同时禁止数据接收和发送。
 
 @param[in] nFd 需要关闭的套接字描述符
 @param[in] nHow 关闭方式 
 @return 成功返回0；失败返回-1。
 @note nHow=0:禁止数据接收
 @note nHow=1:禁止数据发送
 @note nHow=2:同时禁止数据发送和接收
 */
 int CSUDISOCKShutDown(int nFd,int nHow);


 /**
 @brief 关闭指定的套接字。
 
 @param[in] nFd 套接字描述符
 @return 成功返回0；失败返回-1。
 @note 详细描述参见Linux下的close接口说明。
 */
int CSUDISOCKClose(int nFd);


 /**@brief 获取错误号。

 @return 返回错误号,详见udi2_sockerror.h头文件定义。
 */
int CSUDISOCKGetLastError(void);


#ifdef __cplusplus
}
#endif
/** @} */
#endif

