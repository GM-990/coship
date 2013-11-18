
#ifndef CVM_DEF_H
#define CVM_DEF_H

#define  JAVAVM_UNISTD

#include <cs_types.h>
#include <cs_memory.h>

#define MMCP_CVM_MALLOC(a)        MMCP_MemMalloc(MEM_TYPE_JAVAVM_JVM, a)
#define MMCP_CVM_CALLOC(a, b)     MMCP_MemCalloc(MEM_TYPE_JAVAVM_JVM, a, b)
#define MMCP_CVM_REALLOC(a, b)    MMCP_MemRealloc(MEM_TYPE_JAVAVM_JVM, a, b)
#define MMCP_CVM_FREE(a)          MMCP_MemFree(MEM_TYPE_JAVAVM_JVM, a)

#ifdef MMCP_SUPPORT_FS

#include <cs_types.h>
#include <cs_fsapi.h>

#define MMCP_CVM_ACCESS(a,b)   CSFSAccess(a,b) 
#define MMCP_CVM_MKDIR(a,b)    CSFSMkdir(a, b)
#define MMCP_CVM_OPEN(a,b,c)   CSFSOpen(a,b,c)
#define MMCP_CVM_READ(a,b,c)   CSFSRead(a,b,c)
#define MMCP_CVM_WRITE(a,b,c)  CSFSWrite(a, b,c)
#define MMCP_CVM_FLUSH(a)		CSFSFflush(a)
#define MMCP_CVM_CLOSE(a)      CSFSClose(a)
#define MMCP_CVM_LSEEK(a,b,c)  CSFSLseek(a, b, c)
#define MMCP_CVM_LLSEEK(a,b,c,d,e)  CSFSLlseek(a, b,c, d,e)


#define MMCP_CVM_UNLINK(a)    CSFSUnlink(a)
#define MMCP_CVM_RMDIR(a)     CSFSRmdir(a)
#define MMCP_CVM_RENAME(a,b)  CSFSRename(a, b)

#define MMCP_CVM_CHMOD(a,b)   CSFSChmod(a, b)
#define MMCP_CVM_FCHMOD(a,b)  CSFSFchmod(a, b)

#define MMCP_CVM_CHOWN(a,b,c) CSFSChown(a, b, c)
#define MMCP_CVM_FCHOWN(a,b,c) CSFSFchown(a, b, c)

#define MMCP_CVM_STAT(a,b)  CSFSStat(a, b)
#define MMCP_CVM_FSTAT(a,b) CSFSFstat(a, b)
#define MMCP_CVM_LSTAT(a,b) CSFSLstat(a, b)

#define MMCP_CVM_OPENDIR(a)  CSFSOpenDir(a)
#define MMCP_CVM_READDIR(a)  CSFSReadDir(a)
#define MMCP_CVM_CLOSEDIR(a)  CSFSCloseDir(a)

#define MMCP_CVM_TRUNCATE(a,b) CSFSTruncate(a, b)
#define MMCP_CVM_FTRUNCATE(a,b) CSFSFtruncate(a, b)

#define MMCP_CVM_STATFS(a,b)  CSFSStatfs(a, b)
#define MMCP_CVM_FSTATFS(a,b)  CSFSFstatfs(a, b)

#define MMCP_CVM_DUP(a)           CSFSDup(a)
#define MMCP_CVM_DUP2(a,b)        CSFSDup2(a, b)

#define MMCP_CVM_LOCKF(a,b,c)     CSFSLockFile(a, b, c)

#define MMCP_CVM_FOPEN(a,b)       CSFSFopen(a,b)
#define MMCP_CVM_FREAD(a,b,c,d)   CSFSRead(a, b, c*d)
#define MMCP_CVM_FCLOSE(a)        CSFSClose((int)a)
#define MMCP_CVM_FSEEK(a,b,c)     CSFSLseek(a, b,c)
#define MMCP_CVM_FWRITE(a,b,c,d)   CSFSWrite(a, b, c*d)


#define MMCP_CVM_MUNMAP(a,b)         CSFSMemUnmap(a, b)
#define MMCP_CVM_MMAP(a,b,c,d,e,f)   CSFSMemMap(a, b, c, d, e, f)

#define MMCP_CVM_MKSTEMP(a)          CSFSMkstemp(a)

#define CVM_FILE_STAT        CSFSFileStat_s
#define CVM_FS_STATFS      CSFSStatFs_s
#define CVM_DIR		   CSFSDirStream_s
#define CVM_DIRENT	   CSFSDirent_s
#define CVM_FILE           INT

#else /* NO MMCP_SUPPORT_FS */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <dirent.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <limits.h>
#include <alloca.h>
#include <sys/time.h>
#include <sys/param.h>
#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>

#define MMCP_CVM_MKDIR(a,b)    access(a,b)
#define MMCP_CVM_MKDIR(a,b)    mkdir(a, b)
#define MMCP_CVM_OPEN(a,b,c)   open(a,b,c)
#define MMCP_CVM_READ(a,b,c)   read(a,b,c)
#define MMCP_CVM_WRITE(a,b,c)  write(a, b,c)
#define MMCP_CVM_FLUSH(a)		flush(a)
#define MMCP_CVM_CLOSE(a)      close(a)
#define MMCP_CVM_LSEEK(a,b,c)  lseek(a, b, c)
#define MMCP_CVM_LLSEEK(a,b,c,d,e)  llseek(a, b,c, d,e)


#define MMCP_CVM_UNLINK(a)    unlink(a)
#define MMCP_CVM_RMDIR(a)     rmdir(a)
#define MMCP_CVM_RENAME(a,b)  rename(a, b)

#define MMCP_CVM_CHMOD(a,b)   chmod(a, b)
#define MMCP_CVM_FCHMOD(a,b)  fchmod(a, b)

#define MMCP_CVM_CHOWN(a,b,c) chown(a, b, c)
#define MMCP_CVM_FCHOWN(a,b,c) fchown(a, b, c)

#define MMCP_CVM_STAT(a,b)  stat(a, b)
#define MMCP_CVM_FSTAT(a,b) fstat(a, b)
#define MMCP_CVM_LSTAT(a,b) lstat(a, b)

#define MMCP_CVM_OPENDIR(a)  opendir(a)
#define MMCP_CVM_READDIR(a)  readdir(a)
#define MMCP_CVM_CLOSEDIR(a)  closedir (a)

#define MMCP_CVM_TRUNCATE(a,b) truncate(a, b)
#define MMCP_CVM_FTRUNCATE(a,b) ftruncate(a, b)

#define MMCP_CVM_STATFS(a,b)  statfs(a, b)
#define MMCP_CVM_FSTATFS(a,b)  fstatfs(a, b)

#define MMCP_CVM_DUP(a)           dup(a)
#define MMCP_CVM_DUP2(a,b)        dup2(a, b)

#define MMCP_CVM_LOCKF(a,b,c)     lockf(a, b, c)

#define MMCP_CVM_FOPEN(a,b)       fopen (a, b)
#define MMCP_CVM_FREAD(a,b,c,d)   fread (a, b, c, d)
#define MMCP_CVM_FCLOSE(a)        fclose(a)
#define MMCP_CVM_FSEEK(a,b,c)     fseek(a, b,c)
#define MMCP_CVM_FWRITE(a,b,c,d)  fwrite(a, b, c, d)


#define MMCP_CVM_MUNMAP(a,b)         munmap(a, b)
#define MMCP_CVM_MMAP(a,b,c,d,e,f)   mmap(a, b, c, d, e, f)

#define MMCP_CVM_MKSTEMP(a)          mkstemp(a)

#define CVM_FILE_STAT     stat
#define CVM_FS_STATFS   statfs
#define CVM_DIR		DIR
#define CVM_DIRENT	dirent
#define CVM_FILE        FILE

#endif /* MMCP_SUPPORT_FS */

#if defined(MMCP_SUPPORT_IP) || defined(MMCP_SUPPORT_DVB_IP)

#define MMCP_CVM_ACCEPT		accept
#define MMCP_CVM_BIND		bind
#define MMCP_CVM_CONNECT	connect
#define MMCP_CVM_LISTEN		listen
#define MMCP_CVM_RECV		recv
#define MMCP_CVM_RECVFROM	recvfrom
#define MMCP_CVM_SEND		send
#define MMCP_CVM_SENDTO		sendto
#define MMCP_CVM_SOCKET		socket
#define MMCP_CVM_SETSOCKOPT	setsockopt
#define MMCP_CVM_SHUTDOWN	shutdown
#define MMCP_CVM_CLOSESOCKET	closesocket
#define MMCP_CVM_SELECT		selectsocket
#define MMCP_CVM_GETSOCKOPT	getsockopt
#define MMCP_CVM_GETSOCKNAME	getsockname
#define MMCP_CVM_GETHOSTNAME	gethostname
#define MMCP_CVM_GETPROTOBYNAME	getprotobyname
#define MMCP_CVM_IOCTLSOCKET    ioctlsocket

#endif /* defined(MMCP_SUPPORT_IP) || defined(MMCP_SUPPORT_DVB_IP) */

#endif


