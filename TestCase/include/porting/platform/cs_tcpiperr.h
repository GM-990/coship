
#ifndef _CS_TCPIPERR_H_
#define _CS_TCPIPERR_H_

#if defined (PLATFORM_ST40)|| defined (PLATFORM_ST20) || defined (PLATFORM_ZORAN)/*(以下只针对NG_STACK有效)*/
#define CSTCPIP_EOK               0  /* no error */
#define CSTCPIP_E2BIG            -1  /* argument list too long */
#define CSTCPIP_EACCES           -2  /* permission denied */
#define CSTCPIP_EADDRINUSE       -3  /* address already in use */
#define CSTCPIP_EADDRNOTAVAIL    -4  /* address not available from this host */
#define CSTCPIP_EAFNOSUPPORT     -5  /* address family not supported */
#define CSTCPIP_EAGAIN           -6  /* resource temporarily unavailable */
#define CSTCPIP_EALREADY         -7  /* operation already in progress */
#define CSTCPIP_EAUTH            -8  /* authentication error */
#define CSTCPIP_EBADF            -9  /* bad descriptor */
#define CSTCPIP_EBUSY            -10 /* resource busy */
#define CSTCPIP_ECHILD           -11 /* no child process */
#define CSTCPIP_ECONNABORTED     -12 /* connection aborted by user */
#define CSTCPIP_ECONNREFUSED     -13 /* connection refused */
#define CSTCPIP_ECONNRESET       -14 /* connection reseted by peer */
#define CSTCPIP_EDEADLK          -15 /* resource deadlock avoided */
#define CSTCPIP_EDESTADDRREQ     -16 /* destination address required */
#define CSTCPIP_EDOM             -17 /* value out of range */
#define CSTCPIP_EEXIST           -18 /* file exists */
#define CSTCPIP_EFAULT           -19 /* bad address */
#define CSTCPIP_EFBIG            -20 /* file too big */
#define CSTCPIP_EHOSTDOWN        -21 /* host down */
#define CSTCPIP_EHOSTUNREACH     -22 /* host unreachable */
#define CSTCPIP_EIDRM            -23 /* identifier removed */
#define CSTCPIP_EINPROGRESS      -24 /* operation now in progress */
#define CSTCPIP_EINTR            -25 /* interrupted function call */
#define CSTCPIP_EINVAL           -26 /* invalid argument */
#define CSTCPIP_EIO              -27 /* input/output error */
#define CSTCPIP_EISCONN          -28 /* already connected */
#define CSTCPIP_EISDIR           -29 /* is a directory */
#define CSTCPIP_ELOOP            -30 /* too many levels of symbolic links */
#define CSTCPIP_EMFILE           -31 /* out of descriptors */
#define CSTCPIP_EMLINK           -32 /* too many links */
#define CSTCPIP_EMSGSIZE         -33 /* message too big */
#define CSTCPIP_ENEEDAUTH        -34 /* need authenticator */
#define CSTCPIP_ENETDOWN         -35 /* network down */
#define CSTCPIP_ENETRESET        -36 /* network dropped connection on reset */
#define CSTCPIP_ENETUNREACH      -37 /* network unreachable */
#define CSTCPIP_ENFILE           -38 /* too many open files in system */
#define CSTCPIP_ENOBUFS          -39 /* out of resources */
#define CSTCPIP_ENODEV           -40 /* device not found */
#define CSTCPIP_ENOENT           -41 /* no such file or directory */
#define CSTCPIP_ENOEXEC          -42 /* exec format error */
#define CSTCPIP_ENOLCK           -43 /* no locks available */
#define CSTCPIP_ENOMEM           -44 /* cannot allocate memory */
#define CSTCPIP_ENOMSG           -45 /* no message of desired type */
#define CSTCPIP_ENOPROTOOPT      -46 /* not a valid protocol option */
#define CSTCPIP_ENOSPC           -47 /* not enougth space to create object */
#define CSTCPIP_ENOSYS           -48 /* function not implemented */
#define CSTCPIP_ENOTBLK          -49 /* block device required */
#define CSTCPIP_ENOTCONN         -50 /* socket not connected */
#define CSTCPIP_ENOTDIR          -51 /* not a directory */
#define CSTCPIP_ENOTEMPTY        -52 /* directory not empty */
#define CSTCPIP_ENOTSOCK         -53 /* not a socket descriptor */
#define CSTCPIP_ENOTTY           -54 /* not a tty */
#define CSTCPIP_ENXIO            -55 /* device not configured */
#define CSTCPIP_EOPNOTSUPP       -56 /* operation not supported */
#define CSTCPIP_EPERM            -57 /* operation not permitted */
#define CSTCPIP_EPFNOSUPPORT     -58 /* protocol family not supported */
#define CSTCPIP_EPIPE            -59 /* pipe broken */
#define CSTCPIP_EPROTONOSUPPORT  -60 /* protocol not supported */
#define CSTCPIP_EPROTOTYPE       -61 /* wrong protocol type */
#define CSTCPIP_ERANGE           -62 /* result too large */
#define CSTCPIP_EROFS            -63 /* read-only file system */
#define CSTCPIP_ESHUTDOWN        -64 /* can't send after shutdown */
#define CSTCPIP_ESOCKTNOSUPPORT  -65 /* socket type not supported */
#define CSTCPIP_ESPIPE           -66 /* illegal seek */
#define CSTCPIP_ESRCH            -67 /* no such process */
#define CSTCPIP_ETIMEDOUT        -68 /* connection timed out */
#define CSTCPIP_ETOOMANYREFS     -69 /* too many references */
#define CSTCPIP_EWOULDBLOCK      CSTCPIP_EAGAIN /* operation would block */
#define CSTCPIP_EXDEV            -70 /* cross-device link */
#define CSTCPIP_ENAMETOOLONG     -71 /* component of the path name too long */
#define CSTCPIP_EASSERT          -80 /*ASSERT ERROR*/  

#elif defined (PLATFORM_X86) || defined(PLATFORM_BCMMIPSEL)||defined(PLATFORM_HI3110E) ||defined (PLATFORM_HI3716M)|| defined(PLATFORM_CX24501)||defined(PLATFORM_MSD7853)
#include <errno.h>

#define  CSTCPIP_EINPROGRESS      -EINPROGRESS
#define  CSTCPIP_EALREADY      	-EALREADY
#define  CSTCPIP_EWOULDBLOCK      -EWOULDBLOCK
#define  CSTCPIP_EAGAIN           -EAGAIN
#define  CSTCPIP_ENOMEM           -ENOMEM
#define  CSTCPIP_ENOSPC          -ENOSPC
#define  CSTCPIP_EACCES          -EACCES
#define  CSTCPIP_ENOMEM          -ENOMEM
#define  CSTCPIP_EINTR           -EINTR
#define  CSTCPIP_EINVAL          -EINVAL
#define  CSTCPIP_EBADF           -EBADF
#define  CSTCPIP_EPIPE		 -EPIPE
#define  CSTCPIP_ECONNRESET	-ECONNRESET
#define CSTCPIP_EADDRINUSE       -EADDRINUSE

#elif defined (WIN32)
#include <winerror.h>

#define CSTCPIP_EINPROGRESS      -WSAEINPROGRESS
#define CSTCPIP_EALREADY      	-WSAEALREADY
#define CSTCPIP_EWOULDBLOCK      -WSAEWOULDBLOCK
#define CSTCPIP_EACCES          -WSAEACCES
#define CSTCPIP_EINTR           -WSAEINTR
#define CSTCPIP_EINVAL          -WSAEINVAL
#define CSTCPIP_EBADF           -WSAEBADF
#define CSTCPIP_EPROTO		-WSAEPFNOSUPPORT
#define CSTCPIP_ECONNREFUSED	-WSAECONNREFUSED
#define CSTCPIP_EHOSTUNREACH	-WSAEHOSTUNREACH
#define CSTCPIP_EADDRINUSE		-WSAEADDRINUSE
#define CSTCPIP_EADDRNOTAVAIL	-WSAEADDRNOTAVAIL


#endif
#endif
