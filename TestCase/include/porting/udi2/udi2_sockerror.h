/**@defgroup SOCKETERROR SOCKETERROR 定义socket中需要的错误代码值

@brief 本模块定义socket中需要的错误代码值。\n
详细描述参见Linux下/usr/include/asm-generic/errno-base.h  和 /usr/include/asm-generic/errno.h
@version 2.0.1 2009/08/24 初稿
@{
*/

#ifndef _UDI2_SOCKETERROR_H_
#define _UDI2_SOCKETERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define 		CSUDI_EPERM            			(1)      			///< Operation not permitted
#define 		CSUDI_ENOENT           		(2)     			///< No such file or directory
#define 		CSUDI_ESRCH            			(3)      			///< No such process
#define 		CSUDI_EINTR            			(4)      			///< Interrupted system call
#define 		CSUDI_EIO              			(5)      			///< I/O error
#define 		CSUDI_ENXIO            			(6)      			///< No such device or address
#define 		CSUDI_E2BIG            			(7)      			///< Argument list too long
#define 		CSUDI_ENOEXEC          		(8)      			///< Exec format error
#define 		CSUDI_EBADF            			(9)      			///< Bad file number
#define 		CSUDI_ECHILD          			(10)     	 		///< No child processes
#define 		CSUDI_EAGAIN          			(11)     	 		///< Try again
#define 		CSUDI_ENOMEM          		(12)     	 		///< Out of memory
#define 		CSUDI_EACCES          			(13)     	 		///< Permission denied 
#define 		CSUDI_EFAULT          			(14)    	 		///< Bad address
#define 		CSUDI_ENOTBLK         		(15)     			///< Block device required 
#define 		CSUDI_EBUSY           			(16)     	 		///< Device or resource busy
#define 		CSUDI_EEXIST          			(17)     	 		///< File exists 
#define 		CSUDI_EXDEV           			(18)     	 		///< Cross-device link 
#define 		CSUDI_ENODEV          			(19)     	 		///< No such device
#define 		CSUDI_ENOTDIR         		(20)     	 		///< Not a directory
#define 		CSUDI_EISDIR          			(21)     	 		///< Is a directory 
#define 		CSUDI_EINVAL          			(22)      	 		///< Invalid argument
#define 		CSUDI_ENFILE          			(23)     	 		///< File table overflow 
#define 		CSUDI_EMFILE          			(24)      			///< Too many open files
#define 		CSUDI_ENOTTY          			(25)      	 		///< Not a typewriter 
#define 		CSUDI_ETXTBSY         		(26)      	 		///< Text file busy
#define 		CSUDI_EFBIG           			(27)      	 		///< File too large
#define 		CSUDI_ENOSPC          			(28)      			///< No space left on device
#define 		CSUDI_ESPIPE          			(29)   	 		///< Illegal seek
#define 		CSUDI_EROFS           			(30)    	 		///< Read-only file system
#define 		CSUDI_EMLINK          			(31)     	 		///< Too many links
#define 		CSUDI_EPIPE           			(32)     	 		///< Broken pipe 
#define 		CSUDI_EDOM            			(33)     	 		///< Math argument out of domain of func 
#define 		CSUDI_ERANGE          			(34)     	 		///< Math result not representable 

#define		CSUDI_ENOMSG				(35)				///<  No message of desired type 
#define		CSUDI_EIDRM				(36)				///<  Identifier removed 
#define		CSUDI_ECHRNG				(37)				///<  Channel number out of range 
#define		CSUDI_EL2NSYNC			(38)				///<  Level 2 not synchronized 
#define		CSUDI_EL3HLT				(39)				///<  Level 3 halted 
#define		CSUDI_EL3RST				(40)				///<  Level 3 reset 
#define		CSUDI_ELNRNG				(41)				///<  Link number out of range 
#define		CSUDI_EUNATCH				(42)				///<  Protocol driver not attached 
#define		CSUDI_ENOCSI				(43)				///<  No CSI structure available 
#define		CSUDI_EL2HLT				(44)				///<  Level 2 halted 
#define		CSUDI_EDEADLK				(45)				///<  Resource deadlock would occur 
#define		CSUDI_ENOLCK				(46)				///<  No record locks available 
#define		CSUDI_EBADE				(50)				///<  Invalid exchange 
#define		CSUDI_EBADR				(51)				///<  Invalid request descriptor 
#define		CSUDI_EXFULL				(52)				///<  Exchange full 
#define		CSUDI_ENOANO				(53)				///<  No anode 
#define		CSUDI_EBADRQC				(54)				///<  Invalid request code 
#define		CSUDI_EBADSLT				(55)				///<  Invalid slot 
#define		CSUDI_EDEADLOCK			(56)				///<  File locking deadlock error 
#define		CSUDI_EBFONT				(59)				///<  Bad font file format 
#define		CSUDI_ENOSTR				(60)				///<  Device not a stream 
#define		CSUDI_ENODATA				(61)				///<  No data available 
#define		CSUDI_ETIME				(62)				///<  Timer expired 
#define		CSUDI_ENOSR				(63)				///<  Out of streams resources 
#define		CSUDI_ENONET				(64)				///<  Machine is not on the network 
#define		CSUDI_ENOPKG				(65)				///<  Package not installed 
#define		CSUDI_EREMOTE				(66)				///<  Object is remote 
#define		CSUDI_ENOLINK				(67)				///<  Link has been severed 
#define		CSUDI_EADV					(68)				///<  Advertise error 
#define		CSUDI_ESRMNT				(69)				///<  Srmount error 
#define		CSUDI_ECOMM				(70)				///<  Communication error on send 
#define		CSUDI_EPROTO				(71)				///<  Protocol error 
#define		CSUDI_EDOTDOT				(73)				///<  RFS specific error 
#define		CSUDI_EMULTIHOP			(74)				///<  Multihop attempted 
#define		CSUDI_EBADMSG				(77)				///<  Not a data message 
#define		CSUDI_ENAMETOOLONG		(78)				///<  File name too long 
#define		CSUDI_EOVERFLOW			(79)				///<  Value too large for defined data type 
#define		CSUDI_ENOTUNIQ			(80)				///<  Name not unique on network 
#define		CSUDI_EBADFD				(81)				///<  File descriptor in bad state 
#define		CSUDI_EREMCHG				(82)				///<  Remote address changed 
#define		CSUDI_ELIBACC				(83)				///<  Can not access a needed shared library 
#define		CSUDI_ELIBBAD				(84)				///<  Accessing a corrupted shared library 
#define		CSUDI_ELIBSCN				(85)				///<  .lib section in a.out corrupted 
#define		CSUDI_ELIBMAX				(86)				///<  Attempting to link in too many shared libraries 
#define		CSUDI_ELIBEXEC				(87)				///<  Cannot exec a shared library directly 
#define		CSUDI_EILSEQ				(88)				///<  Illegal byte sequence 
#define		CSUDI_ENOSYS				(89)				///<  Function not implemented 
#define		CSUDI_ELOOP				(90)				///<  Too many symbolic links encountered 
#define		CSUDI_ERESTART			(91)				///<  Interrupted system call should be restarted 
#define		CSUDI_ESTRPIPE				(92)				///<  Streams pipe error 
#define		CSUDI_ENOTEMPTY			(93)				///<  Directory not empty 
#define		CSUDI_EUSERS				(94)				///<  Too many users 
#define		CSUDI_ENOTSOCK			(95)				///<  Socket operation on non-socket 
#define		CSUDI_EDESTADDRREQ		(96)				///<  Destination address required 
#define		CSUDI_EMSGSIZE			(97)				///<  Message too long 
#define		CSUDI_EPROTOTYPE			(98)				///<  Protocol wrong type for socket 
#define		CSUDI_ENOPROTOOPT		(99)				///<  Protocol not available 

#define		CSUDI_EPROTONOSUPPORT	(120)			///<  Protocol not supported 
#define		CSUDI_ESOCKTNOSUPPORT	(121)			///<  Socket type not supported 
#define		CSUDI_EOPNOTSUPP			(122)			///<  Operation not supported on transport endpoint 
#define		CSUDI_EPFNOSUPPORT		(123)			///<  Protocol family not supported 
#define		CSUDI_EAFNOSUPPORT		(124)			///<  Address family not supported by protocol 
#define		CSUDI_EADDRINUSE			(125)			///<  Address already in use 
#define		CSUDI_EADDRNOTAVAIL		(126)			///<  Cannot assign requested address 
#define		CSUDI_ENETDOWN			(127)			///<  Network is down 
#define		CSUDI_ENETUNREACH		(128)			///<  Network is unreachable 
#define		CSUDI_ENETRESET			(129)			///<  Network dropped connection because of reset 
#define		CSUDI_ECONNABORTED		(130)			///<  Software caused connection abort 
#define		CSUDI_ECONNRESET			(131)			///<  Connection reset by peer 
#define		CSUDI_ENOBUFS				(132)			///<  No buffer space available 
#define		CSUDI_EISCONN				(133)			///<  Transport endpoint is already connected 
#define		CSUDI_ENOTCONN			(134)			///<  Transport endpoint is not connected 
#define		CSUDI_EUCLEAN				(135)			///<  Structure needs cleaning 
#define		CSUDI_ENOTNAM				(137)			///<  Not a XENIX named type file 
#define		CSUDI_ENAVAIL				(138)			///<  No XENIX semaphores available 
#define		CSUDI_EISNAM				(139)			///<  Is a named type file 
#define		CSUDI_EREMOTEIO			(140)			///<  Remote I/O error 
#define 		CSUDI_EINIT				(141)			///<  Reserved 
#define 		CSUDI_EREMDEV				(142)			///<  Error 142 
#define		CSUDI_ESHUTDOWN			(143)			///<  Cannot send after transport endpoint shutdown 
#define		CSUDI_ETOOMANYREFS		(144)			///<  Too many references: cannot splice 
#define		CSUDI_ETIMEDOUT			(145)			///<  Connection timed out 
#define		CSUDI_ECONNREFUSED		(146)			///<  Connection refused 
#define		CSUDI_EHOSTDOWN			(147)			///<  Host is down 
#define		CSUDI_EHOSTUNREACH		(148)			///<  No route to host 
#define		CSUDI_EWOULDBLOCK	CSUDI_EAGAIN		///<  Operation would block 
#define		CSUDI_EALREADY			(149)			///<  Operation already in progress 
#define		CSUDI_EINPROGRESS			(150)			///<  Operation now in progress 
#define		CSUDI_ESTALE				(151)			///<  Stale NFS file handle 
#define 		CSUDI_ECANCELED			(158)			///<  AIO operation canceled 

/*
 * These error are Linux extensions.
 */
#define 		CSUDI_ENOMEDIUM			(159)			///<  No medium found 
#define 		CSUDI_EMEDIUMTYPE		(160)			///<  Wrong medium type 
#define		CSUDI_ENOKEY				(161)			///<  Required key not available 
#define		CSUDI_EKEYEXPIRED			(162)			///<  Key has expired 
#define		CSUDI_EKEYREVOKED			(163)			///<  Key has been revoked 
#define		CSUDI_EKEYREJECTED		(164)			///<  Key was rejected by service 

/* for robust mutexes */
#define    	CSUDI_EOWNERDEAD 	 			(165)			///<  Owner died 
#define    	CSUDI_ENOTRECOVERABLE 			(166) 			///<  State not recoverable 
#define 		CSUDI_EDQUOT					(1133)			///<  Quota exceeded ble 



#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_SOCKETERROR_H_

