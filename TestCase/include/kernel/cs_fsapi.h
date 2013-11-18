#ifndef _CS_FSAPI_H_
#define _CS_FSAPI_H_

#include "mmcp_typedef.h"


#ifdef __cplusplus
extern "C" {
#endif
/*此目录下的都认为是系统的目录，比如要访问系统下的"/home/usr000"目录必须传入"/workdir/home/usr000"的路径名*/
#define CSFS_LOCALROOT 	"/workdir" 
#define CSFS_USBROOT    "/usb"
#define CSFS_FLASHROOT  "/flash"
#define CSFS_ROMROOT	"/rom"
#define CSFS_HTTPROOT	"/http"  //example: /http/172.30.50.235/bw50/portal/app.zip

#define CSFS_FS_TYPE_DSM		"dsmfs"
#define CSFS_FS_TYPE_RAMFS		"ramfs"
#define CSFS_FS_TYPE_JFFS		"jffs"
#define CSFS_FS_TYPE_ROMFS		"romfs"

#define CSFS_FS_TYPE_FAT32		"fat32"
#define CSFS_FS_TYPE_EXT2		"ext2"
#define CSFS_FS_TYPE_EXT3		"ext3"
#define CSFS_FS_TYPE_JFFS2		"jffs2"
#define CSFS_FS_TYPE_NTFS		"ntfs"
#define CSFS_FS_TYPE_CRAMFS	"cramfs"
#define CSFS_FS_TYPE_YAFFS2      "yaffs2"

/*创建或者打开文件的标志定义*/
#define CSFS_O_ACCMODE        0x00000003 
#define CSFS_O_RDONLY         0x00000001
#define CSFS_O_WRONLY         0x00000002
#define CSFS_O_RDWR           0x00000003
#define CSFS_O_CREAT          0x00000008
#define CSFS_O_EXCL           0x00000010
#define CSFS_O_TRUNC          0x00000020
#define CSFS_O_APPEND         0x00000004

#define CSFS_O_MODE	00777

#define CSFS_SEEK_SET 	0	/* Seek from beginning of file.  */
#define CSFS_SEEK_CUR 	1	/* Seek from current position.  */
#define CSFS_SEEK_END  	2 	/* Set file pointer to EOF plus "offset" */

#define CSFS_S_IFMT  00170000
#define CSFS_S_IFSOCK 0140000
#define CSFS_S_IFLNK	0120000
#define CSFS_S_IFREG  0100000
#define CSFS_S_IFBLK  0060000
#define CSFS_S_IFDIR  0040000
#define CSFS_S_IFCHR  0020000
#define CSFS_S_IFIFO  0010000
#define CSFS_S_ISUID  0004000
#define CSFS_S_ISGID  0002000
#define CSFS_S_ISVTX  0001000
#define CSFS_S_ISLNK(m)	(((m) & CSFS_S_IFMT) == CSFS_S_IFLNK)
#define CSFS_S_ISREG(m)	(((m) & CSFS_S_IFMT) == CSFS_S_IFREG)
#define CSFS_S_ISDIR(m)	(((m) & CSFS_S_IFMT) == CSFS_S_IFDIR)
#define CSFS_S_ISCHR(m)	(((m) & CSFS_S_IFMT) == CSFS_S_IFCHR)
#define CSFS_S_ISBLK(m)	(((m) & CSFS_S_IFMT) == CSFS_S_IFBLK)
#define CSFS_S_ISFIFO(m)	(((m) & CSFS_S_IFMT) == CSFS_S_IFIFO)
#define CSFS_S_ISSOCK(m)	(((m) & CSFS_S_IFMT) == CSFS_S_IFSOCK)

#define CSFS_F_DUPFD		0	/* dup */
#define CSFS_F_GETFD		1	/* get f_flags */
#define CSFS_F_SETFD		2	/* set f_flags */


#define CSFS_DEVICENAME_LEN 20
#define CSFS_MOUNTDIR_LEN 256
typedef void  CSFSDir_t;

struct CSFSDirent_s
{
	unsigned long 		m_uiDirInod;
	unsigned long 		m_lDirOff;	/*The index of this dirent*/
	unsigned short		m_usDirRecLen;/*No use temp*/
	unsigned char	 	m_ucDirType;    /*0,not a dir; 1,this is a dir*/
	char				m_cDirName[256];           /* We must not include limits.h! */
};


/**
like file in PC(windows) :st_size is the actual size of file,
and  st_blksize*st_blocks is the allocated size in filesystem;
use CSFS_S_ISXXX to st_mode
*/
struct CSFSFileStat_s
{
	USHORT st_dev;
	USHORT st_ino;
	USHORT st_mode;
	USHORT st_nlink;
	USHORT st_uid;
	USHORT st_gid;
	USHORT st_rdev;
	ULONG  st_size;         /*actual size of file*/
	ULONG    st_blksize;  /* blocksize for filesystem I/O */
	ULONG    st_blocks;   /* number of blocks allocated, st_blksize*st_blocks is the allocated size in filesystem*/
//#if defined (PLATFORM_X86)
/*linux平台上st_atime编译问题的临时方案，在/usr/include/bits/stat.h中
st_atime定义成了st_atim.tv_sec，所以这里编译时会报错，最终的方案是改动
CSFSFileStat_s中成员变量的名字，但是工作量大，短时间内改不好，所以先这
么改动保证linux平台上可以编译通过，改名字的事情由罗金华负责，春节前改好*/
	#ifdef st_atime
	#undef st_atime
	#endif
	#ifdef st_mtime
	#undef st_mtime
	#endif
	#ifdef st_ctime
	#undef st_ctime
	#endif
//#endif
	ULONG    st_atime;
	ULONG    st_mtime;
	ULONG    st_ctime;
};


struct CSFSStatFs_s 
{
	        LONG f_type;
	        LONG f_bsize;
	        LONG f_blocks;
	        LONG f_bfree;
	        LONG f_bavail;
	        LONG f_files;
	        LONG f_ffree;
	        LONG f_fsid;
	        LONG f_namelen;
	        LONG f_spare[6];
};

struct CSFSDeviceInfo_s
{
	char cDeviceName[CSFS_DEVICENAME_LEN];		/*设备名*/
	char cDeviceTypeName[CSFS_DEVICENAME_LEN];	/*设备对应文件系统的名*/
	char cMountedDir[CSFS_MOUNTDIR_LEN];		 /*设备挂载目录*/
	int nDeviceType;					 /*设备类型，如，romfs 0x01*/
	int nDeviceID;					/*设备ID号，如，rom0 0x0100*/
};

struct CSFSRomfsInfo_s
{
	int 		m_nROMUnits;			/*address counts in m_ppcROMUnitsArray*/
	int 		m_nROMSizePerUnit;	/*length in bytes of m_ppcROMUnitsArray[i]*/
	char ** 	m_ppcROMUnitsArray;	/*ROM contents marix address*/
};

typedef enum _CSFS_Ramdisk_Type_E{
	CSFS_RAMDISK_TYPE_INNER = 			1,			/*fs内部分配*/
	CSFS_RAMDISK_TYPE_OUTER = 			2			/*外部分配传入*/
}CSFS_Ramdisk_Type_E;

struct CSFSRamfsMountOpt_s
{
	int 		m_nRamDiskType;		/*CSFS_RAMDISK_TYPE_INNER为fs内部分配，CSFS_RAMDISK_TYPE_OUTER为外部分配传入，其它值暂时为非法*/
	int 		m_nRamDiskSizeKbytes;	/*m_ucpRamDisk的空间大小或者需要内部分配的大小*/
	BYTE* 	m_ucpRamDisk;			/*m_nRamDiskType为CSFS_RAMDISK_TYPE_OUTER时ramdisk的指针*/
};

typedef struct CSFSStatFs_s 		CSFSStatFs_t;
typedef struct CSFSFileStat_s        	CSFSFileStat_t;
typedef struct CSFSDirent_s		CSFSDirent_t;
typedef struct CSFSDeviceInfo_s  	CSFSDeviceInfo_t;
typedef struct CSFSRomfsInfo_s  	CSFSRomfsInfo_t;
typedef struct CSFSRamfsMountOpt_s  	CSFSRamfsMountOpt_t;

struct CSFSOperation_s {
	 int (*Init)(void);
	 int (*Mount)(const char *szDevice, const char *szMountPoint, const char *szFileSystem, int nFlag , const void *pvData);
	 int (*Umount)(const char *szMountPoint);
	 int (*Mkdir)(const char *szDir, int nMode);
	 int (*Open)(const char *szFile, int nFlag, int nMode);
	 int (*Read)(int nFile, char *pcBuf, unsigned int uCount);
	 int (*Write)(int nFile, char *pcBuf, unsigned int uCount);
	 int (*Close)(int nFile);
	 int (*Lseek)(int nFile, long lOffset, unsigned int uOrigin);
	 int (*Llseek)(int nFile,  long nOffsetHigh,unsigned long nOffsetLow, long * plResult, unsigned int uOrigin);
	 long (*Ftell)(int nFile);
	 int (*Fflush)(int nFile);
	 int (*Unlink)(const char * szFile);
	 int (*Rmdir)(const char * szDir);
	 int (*Rename)(const char *szOldPath, const char *szNewPath);
	 int (*Chmod)(const char * szPath, int nMode);
	 int (*Fchmod)(int nFile, int nMode);
	 int (*Chown)(const char * szPath, int nOwner, int nGroup);
	 int (*Fchown)(int nFile, int nOwner, int nGroup);
	 int (*Stat)(const char *szPath, CSFSFileStat_t *psFileStat);
	 int (*Fstat)(int nFile, CSFSFileStat_t *psFileStat);
	 int (*Lstat)(const char *szPath, CSFSFileStat_t *psFileStat);
	 void* (*OpenDir) (const char *szDir);
	 CSFSDirent_t* (*ReadDir)(void *psDir);
	 int (*CloseDir) (void *psDir);
	 int (*Truncate)(const char *szFile, long lLength);
	 int (*Ftruncate)(int nFile, long lLength);
	 int (*Statfs)(const char *szPath, CSFSStatFs_t *psFsStat);
	 int (*Fstatfs)(int nFile, CSFSStatFs_t *psFsStat);
	 int (*Dup)(int nFile);
	 int (*Dup2)(int nOldFile, int nNewFile);
	 int (*LockFile)(int nFile, int nCmd, long lLength);/*cmd=1 lock;cmd=0 unlock*/
	 int (*Mkstemp)(char * szTmpName);
	 int (*Format)(const char *szDevice, const char *szFileSystem, void *pParam );
	 int (*addFsRoot)(char* szFsRoot);		//设置文件系统访问根路径,该操作指令被CSFSBindDir接口调用
	 int (*Path2DriverPath)(const char * szFsPath, int nDriverPathLen, char * szDriverPath);
};

typedef struct CSFSOperation_s  	CSFSOperation_t;

/**
@brief     	注册签名和校验算法接口给fs模块，用于打开、读、写等操作文件时做完整性校验接口
*/
typedef struct
{
	BOOL (*m_fnSign)(char *pcFileName, unsigned char *pucData, int nLen);
    BOOL (*m_fnVerify)(char *pcFileName, unsigned char *pucData, int nLen);
    void (*m_fnNotifyUnlinkFile)(char *pcFileName);
}CSFSEncryptMethod_S;

void CSFSSetCWD(const char *pcCurrentPath);

/**
@brief     文件系统初始化
@param[in] nRootDevice  根设备
@param[in] nRamFsKBytes 内存文件系统大小(单位KB)
@return    返回是否成功，== 0 成功，< 0 失败

@example
           int nRet = CSFSInit(0x100, 1024);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }

*/
int CSFSInit(int nRootDevice, int nRamFsKBytes);

/**
@brief     文件系统初始化(不带UDIFS支持，针对UDI2.0)
@param[in] nRootDevice  根设备
@param[in] nRamFsKBytes 内存文件系统大小(单位KB)
@return    返回是否成功，== 0 成功，< 0 失败

@example
           int nRet = CSFSInitWithoutUDIFS(0x100, 1024);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }

*/
int CSFSInitWithoutUDIFS(int nRootDevice, int nRamFsKBytes);

/**
@brief mount  挂载分区接口

@param[in] pcDeviceName 源设备
@param[in] pcDir 挂载点
@param[in] pcFilesystemType 文件系统类型
@param[in] dwMountFlags 挂载标志
@param[in] pData 挂载参数
	在mount jffs文件系统时:
	参数-delete_no_parent_file	表示会在mount 时进行对源设备进行文件清理;
*/

int CSFSMount( const char *pcDeviceName,
		                            const char * pcDir ,
		                            const char * pcFilesystemType, 
		                            DWORD  dwMountFlags , 
		                            const void * pData);



   
int CSFSUmount(const char *pcDir);

/*路径命名规则注*****************************************************

CS文件系统: 中间件自己的文件系统，直接以具体的路径为路径名，如
            /test/datadir
            /jffs/data.txt
            
OS文件系统: 操作系统的文件系统，需要在具体路径前面加上"/workdir"，如
            /workdir/mnt/hd/HDD0/test/datadir
            /workdir/e:/test/data.txt

*********************************************************************/


/**
@brief     在指定路径创建一个文件夹
@param[in] pcPathName 全路径名，路径命名规则见前面说明[路径命名规则注]
@param[in] nMode 模式，取值 CSFS_O_MODE
@return    返回是否成功，== 0 成功，< 0 失败

@example
           int nRet = CSFSMkdir("/example", CSFS_O_MODE);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }

*/
int CSFSMkdir(const char * pcPathName, int nMode);

/*Just function such as 'mkdir -p' on linux platform*/
int CSFSMkdirP(const char * pcPathName, int nMode);

/**
@brief     在指定路径创建一个文件
@param[in] pcPathName 全路径名，路径命名规则见前面说明[路径命名规则注]
@param[in] nFlags 创建文件标志，取值为
           CSFS_O_ACCMODE 
           CSFS_O_RDONLY   只读，只能进行读操作，进行写操作返回失败
           CSFS_O_WRONLY   只写，只能进行写操作，进行读操作返回失败
           CSFS_O_RDWR     读写，可对文件进行读写操作
           CSFS_O_CREAT    创建, 如果文件存在则打开，不存在则创建
           CSFS_O_EXCL    
           CSFS_O_TRUNC    如果文件存在则将其清空
           CSFS_O_APPEND   文件末尾追加
           文件标志可以自由组合，常用组合有以下几种:
           CSFS_O_RDONLY  以只读方式打开已存在的文件
           CSFS_O_WRONLY  以只写方式打开已存在的文件
           CSFS_O_RDWR    以读写方式打开已存在的文件
           CSFS_O_CREAT | CSFS_O_WRONLY  以只写方式打开文件，文件不存在则创建
           CSFS_O_CREAT | CSFS_O_RDWR    以读写方式打开文件，文件不存在则创建
           CSFS_O_CREAT | CSFS_O_RDWR | CSFS_O_TRUNC  以读写方式打开文件，文件不存在则创建，存在则清空
@param[in] nMode 模式，取值 CSFS_O_MODE
@return    返回文件描述符 nFile, nFile >= 0 有效的文件描述符
                                 nFile < 0  无效的文件描述符

@example
           int nFile = CSFSOpen("/example", CSFS_O_CREAT | CSFS_O_RDWR | CSFS_O_TRUNC, CSFS_O_MODE);
           if(nFile >= 0)
           {
               //TODO: 有效的文件描述符
           }
           else
           {
	           //TODO: 无效的文件描述符
           }

*/
int CSFSOpen(const char * pcPathName, int nFlags,int nMode);

/**
@brief     在指定路径创建一个文件
@param[in] pcPathName 全路径名，路径命名规则见前面说明[路径命名规则注]
@param[in] pcMode 创建文件标志，采用C99标准的意义, 取值为
           r   打开只读文件，该文件必须存在
           r+  打开可读写的文件，该文件必须存在
           w   打开只写文件，若文件存在则文件长度清为0，若文件不存在则建立该文件
           w+  打开可读写文件，若文件存在则文件长度清为零，若文件不存在则建立该文件
           a   以附加的方式打开只写文件，若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾
           a+  以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后@param[in] nMode 模式，取值 CSFS_O_MODE
@return    返回文件描述符 nFile, nFile >= 0 有效的文件描述符
                                 nFile < 0  无效的文件描述符

@example
           int nFile = CSFSFopen("/example", "w+");
           if(nFile >= 0)
           {
               //TODO: 有效的文件描述符
           }
           else
           {
	           //TODO: 无效的文件描述符
           }

*/
int CSFSFopen(const char * pcPathName, const char * pcMode);

/**
@brief     读文件
@param[in] nFileDescriptor 文件描述符(由CSFSOpen 或 CSFSFopen 接口获取，有效值 >= 0)
@param[in] pcBuf  存放从文件读取数据的缓冲区
@param[in] uCount 从文件读取数据长度，该值要 <= pcBuf的长度
@return    返回实际读取的数据长度nLen，nLen == uCount 读取成功
                                       nLen < uCount && nLen >= 0 文件读完
                                       nLen < 0 读取失败

@example
		   #define COUNT 512
		   char pcBuf[COUNT];
		   int nFile, nLen;
		   nFile = CSFSOpen("/example", CSFS_O_RDONLY, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // 打开文件失败
           }
           int nLen = CSFSRead(nFile, pcBuf, COUNT);
           if(nLen <= COUNT && nLen >= 0)
           {
               //TODO: 读取成功
           }
           else
           {
	           //TODO: 读取失败
           }
           CSFSClose(nFile);

*/
int CSFSRead(int nFileDescriptor, char * pcBuf, unsigned int uCount);

/**
@brief     写文件
@param[in] nFileDescriptor 文件描述符(由CSFSOpen 或 CSFSFopen 接口获取，有效值 >= 0)
@param[in] pcBuf  要写入文件的数据缓存
@param[in] uCount 要写入文件的数据长度，该值要 <= pcBuf的长度
@return    返回实际写入的数据长度nLen，nLen == uCount 写入成功
                                       nLen < uCount  写入失败

@example
		   #define COUNT 512
		   char pcBuf[COUNT]; // 假设现在该缓存已经存放好了待写入的数据
		   int nFile, nLen;
		   nFile = CSFSOpen("/example", CSFS_O_CREAT | CSFS_O_RDWR | CSFS_O_TRUNC, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // 打开文件失败
           }
           int nLen = CSFSWrite(nFile, pcBuf, COUNT);
           if(nLen == COUNT)
           {
               //TODO: 写入成功
           }
           else
           {
	           //TODO: 写入失败
           }
           CSFSClose(nFile);

*/
int CSFSWrite(int nFileDescriptor, char * pcBuf, unsigned int uCount);

/**
@brief     关闭文件
@param[in] nFileDescriptor 文件描述符(由CSFSOpen 或 CSFSFopen 接口获取，有效值 >= 0)
@return    返回是否成功，== 0 成功，< 0 失败

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_CREAT | CSFS_O_RDWR | CSFS_O_TRUNC, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // 打开文件失败
           }
           nRet = CSFSClose(nFile);
           if(nRet < 0)
           {
               //TODO: 关闭文件失败
           }
           else
           {
	           //TODO: 关闭文件成功
           }
*/
int CSFSClose(int nFileDescriptor);

/**
@brief     移动文件指针
@param[in] nFileDescriptor 文件描述符(由CSFSOpen 或 CSFSFopen 接口获取，有效值 >= 0)
@param[in] lOffset  移动偏移量
@param[in] uOrigin  移动起始位置，取值为
                    CSFS_SEEK_SET  文件开头
                    CSFS_SEEK_CUR  当前位置
                    CSFS_SEEK_END  文件末尾
@return    返回移动后的指针位置，>= 0 有效位置，< 0 无效位置

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_RDONLY, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // 打开文件失败
           }
           
           // 移动到文件开头
		   nRet = CSFSLseek(nFile, 0, CSFS_SEEK_SET);
           if(nRet < 0)
           {
               //TODO: 移动失败
           }
           else
           {
	           //TODO: 移动成功
           }

           // 移动到文件末尾(判断返回值省略)
		   nRet = CSFSLseek(nFile, 0, CSFS_SEEK_END);
           // 从文件开头向后移动10个字节(判断返回值省略)
		   nRet = CSFSLseek(nFile, 10, CSFS_SEEK_SET);
 		   // 从文件末尾向前移动10个字节(判断返回值省略)
		   nRet = CSFSLseek(nFile, -10, CSFS_SEEK_END);
		   // 从文件当前位置向后移动10个字节(判断返回值省略)
		   nRet = CSFSLseek(nFile, 10, CSFS_SEEK_CUR);
		   // 从文件当前位置向前移动10个字节(判断返回值省略)
		   nRet = CSFSLseek(nFile, -10, CSFS_SEEK_CUR);
           CSFSClose(nFile);
*/
int CSFSLseek(int nFileDescriptor, long lOffset, unsigned int uOrigin);

int CSFSLlseek(int nFileDescriptor, unsigned long  dwOffsetHigh,unsigned long  dwOffsetLow, long * plResult, unsigned int uOrigin);

/**
@brief     获取当前文件指针位置
@param[in] nFileDescriptor 文件描述符(由CSFSOpen 或 CSFSFopen 接口获取，有效值 >= 0)
@return    返回当前文件指针位置，>= 0 有效位置，< 0 无效位置

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_CREAT | CSFS_O_RDWR, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // 打开文件失败
           }
           nRet = CSFSFtell(nFile);
           if(nRet < 0)
           {
               //TODO: 无效位置
           }
           else
           {
	           //TODO: 有效位置
           }
           CSFSClose(nFile);
*/
long CSFSFtell(int nFileDescriptor);

/**
@brief     将缓存中的内容刷新到实际存储设备(如: 硬盘、U盘)
           为了提高读写效率，操作系统往往不会把数据立刻写入实际存储设备而是
           在内存中的缓存中，等到一定量再一次性写入实际存储设备
@param[in] nFileDescriptor 文件描述符(由CSFSOpen 或 CSFSFopen 接口获取，有效值 >= 0)
@return    返回是否成功，== 0 成功，< 0 失败

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_RDONLY, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // 打开文件失败
           }
           nRet = CSFSFflush(nFile);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
           CSFSClose(nFile);
*/
int CSFSFflush(int nFileDescriptor);

/**
@brief     删除文件
@param[in] pcPathName 全路径名，路径命名规则见前面说明[路径命名规则注]
@return    返回是否成功，== 0 成功，< 0 失败

@example
		   int nRet;
           nRet = CSFSUnlink(pcPathName);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
*/
int CSFSUnlink(const char * pcPathName);

/**
@brief     删除文件夹
@param[in] pcPathName 全路径名，路径命名规则见前面说明[路径命名规则注]
@return    返回是否成功，== 0 成功，< 0 失败

@example
		   int nRet;
           nRet = CSFSRmdir(pcPathName);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
*/
int CSFSRmdir(const char * pcPathName);

/**
@brief     删除文件夹
@param[in] pcOldPath 待重命名的文件全路径，路径命名规则见前面说明[路径命名规则注]
@param[in] pcNewPath 重命名后的新名字，路径命名规则见前面说明[路径命名规则注]
@return    返回是否成功，== 0 成功，< 0 失败

@example
		   int nRet;
           nRet = CSFSRename(pcOldPath, pcNewPath);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
*/
int CSFSRename(const char * pcOldPath, const char * pcNewPath);

int CSFSChmod(const char * pcPathName, int nMode);

int CSFSAccess(const char *pcPathName,int i_Mode);

int CSFSFchmod(int nFileDescriptor, int nMode);

int CSFSChown(const char * pcPathName, int nUidOwner, int nGidGroup);

int CSFSFchown(int nFileDescriptor, int nUidOwner, int nGidGroup);

/**
@brief     获取文件状态属性
@param[in] pcFileName 文件名全路径，路径命名规则见前面说明[路径命名规则注]
@param[in] psFileStat 保存文件属性的结构，如下
					  struct CSFSFileStat_t
					  {
					      USHORT st_dev;
						  USHORT st_ino;
						  USHORT st_mode;     文件类型，CSFS_S_ISDIR(st_mode) != 0 则文件是一个文件夹
						                                CSFS_S_ISREG(st_mode) != 0 则文件是一个普通文件
						  USHORT st_nlink;
						  USHORT st_uid;
						  USHORT st_gid;
						  USHORT st_rdev;
						  ULONG  st_size;     文件大小
						  ULONG  st_blksize;  文件系统块大小(如512字节)
						  ULONG  st_blocks;   占用的块个数
						  ULONG  st_atime;
						  ULONG  st_mtime;
						  ULONG  st_ctime;
					  };
@return    返回是否成功，== 0 成功，< 0 失败(可能文件不存在)

@example
		   int nRet;
		   CSFSFileStat_t psFileStat;
           nRet = CSFSStat(pcFileName, &psFileStat);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
*/
int CSFSStat(const char * pcFileName, CSFSFileStat_t *psFileStat);

/**
@brief     获取文件状态属性
@param[in] nFileDescriptor 文件描述符(由CSFSOpen 或 CSFSFopen 接口获取，有效值 >= 0)
@param[in] psFileStat 保存文件属性的结构，见[CSFSStat]接口
@return    返回是否成功，== 0 成功，< 0 失败(可能文件不存在)

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_RDONLY, CSFS_O_MODE);
		   CSFSFileStat_t psFileStat;
           nRet = CSFSFstat(nFile, &psFileStat);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
*/
int CSFSFstat(int nFileDescriptor, CSFSFileStat_t * psFileStat);

int CSFSLstat(const char * pcFileName, CSFSFileStat_t *psFileStat);

/**
@brief     打开一个文件夹，用于读取该文件夹下面的文件或子文件夹
@param[in] pcDirName  全路径名，路径命名规则见前面说明[路径命名规则注]
@return    返回文件夹的句柄，== NULL 失败

@example
		   CSFSDir_t * pDir;
		   pDir = CSFSOpenDir("/example");
           if(pDir == NULL)
           {
               //TODO: 失败
           }
           else
           {
	           //TODO: 成功
           }
*/
CSFSDir_t * CSFSOpenDir (const char *pcDirName);

/**
@brief     读取文件夹下面的文件或子文件夹，调用一次读取一个子文件
@param[in] psDir  待读取文件夹的句柄
@return    返回读取的文件信息，== NULL 失败，如下
		   struct CSFSDirent_s
		   {
		       unsigned long     m_uiDirInod;
		       unsigned long     m_lDirOff;	       文件在文件夹下的序号
		       unsigned short    m_usDirRecLen;    未使用
		       unsigned char     m_ucDirType;      文件类型，0 为文件，1 为文件夹
		       char				 m_cDirName[256];  文件或文件夹名字
		   };

@example
		   CSFSDir_t * pDir;
		   CSFSDirent_t * pDirent;
		   pDir = CSFSOpenDir("/example");
		   pDirent = CSFSReadDir(pDir);
		   while(pDirent != NULL)
		   {
		       // 成功
		       pDirent = CSFSReadDir(pDir); // 继续读取其他文件
		   }
		   // while循环退出，说明该文件夹下面的文件遍历完了
*/
CSFSDirent_t * CSFSReadDir(CSFSDir_t *psDir);

/**
@brief     关闭一个文件夹
@param[in] psDir  文件夹句柄
@return    返回是否成功，== 0 成功，< 0 失败

@example
		   int nRet;
		   CSFSDir_t * pDir;
		   pDir = CSFSOpenDir("/example");
		   nRet = CSFSCloseDir(pDir);
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
*/
int CSFSCloseDir (CSFSDir_t *psDir);

int CSFSTruncate(const char * pcPathName, long lLength);

int CSFSFtruncate(int nFileDescriptor, long lLength);

int CSFSStatfs(const char * pcPathName, CSFSStatFs_t * psFsStat);

int CSFSFstatfs(int nFileDescriptor, CSFSStatFs_t * psFsStat);

int CSFSDup(int nOldFileDescriptor);

int CSFSDup2(int nOldFileDescriptor, int nNewFileDescriptor);

int CSFSLockFile(int nFileDescriptor, int nCmd, long lLength);/*cmd=1 lock;cmd=0 unlock*/

int CSFSMemUnmap(char * pcAddr, int nLength);

void * CSFSMemMap(void * pvStart, int length, int nProt, int nFlags, int nFileDescriptor, long lOffset);

int CSFSMkstemp(char * pcTempName);
/*得到已经挂载的设备的个数*/
int CSFSGetDeviceCount( int * pnCount, int*  pnDeviceId);

/*根据设备号得到已经挂载的某个设备的信息*/
int CSFSGetDeviceInfo( int nDeviceId,CSFSDeviceInfo_t *psDeviceInfo);
/*格式化某个设备为特定类型的分区，对于硬盘和闪盘等大容量存储设备适用于格式化成fat和e2fs的格式，目前flash上只能被格式化成jffs格式的分区。*/
int CSFSFormat(const char *pcDeviceName, const char *pcFSTypeName, void *pParam );

void CSFSBrowseDir(const char *pcDirName,int*pnFileCount,int * pnDirCount);

void CSFSBrowseDirWithLevel(const char *pcDirName,int*pnFileCount,int * pnDirCount,int nInfoLevel);

/**
@brief     拷贝文件或文件夹
@param[in] pSrcPath  源文件全路径名，路径命名规则见前面说明[路径命名规则注]
@param[in] pDestPath 目标文件全路径名，路径命名规则见前面说明[路径命名规则注]
@param[in] pOptions  当该值为 NULL 时，该接口拷贝时遇到相同名字的文件时则返回 -EEXIST
                     当该值为 "Rf"(不区分大小写)时，则遇到相同名字的文件则覆盖
@return    返回是否成功，== 0 成功，< 0 失败

@example
           // 将文件拷贝并重命名
		   int nRet = CSFSCopy("/example1/data1.txt", "/example2/data2.txt", "Rf");
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
           // 将文件拷贝到另一文件夹
		   nRet = CSFSCopy("/example1/data1.txt", "/example2", "Rf");
		   // 将文件夹拷贝并重命名(example2 文件夹不存在)
		   nRet = CSFSCopy("/example1", "/example2", "Rf");
		   // 将文件夹拷贝到另一个文件夹下面(example2 文件夹存在)
		   nRet = CSFSCopy("/example1", "/example2", "Rf");
*/

int CSFSCopy(const char *pSrcPath, const char *pDestPath, char *pOptions);

/**
@brief     移动文件或文件夹
@param[in] pSrcPath  源文件全路径名，路径命名规则见前面说明[路径命名规则注]
@param[in] pDestPath 目标文件全路径名，路径命名规则见前面说明[路径命名规则注]
@param[in] pOptions  当该值为 NULL 时，该接口移动时遇到相同名字的文件时则返回 -EEXIST
                     当该值为 "Rf"(不区分大小写)时，则遇到相同名字的文件则覆盖
@return    返回是否成功，== 0 成功，< 0 失败

@example
           // 将文件移动并重命名
		   int nRet = CSFSMove("/example1/data1.txt", "/example2/data2.txt", "Rf");
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
           // 将文件移动到另一文件夹
		   nRet = CSFSMove("/example1/data1.txt", "/example2", "Rf");
		   // 将文件夹移动并重命名(example2 文件夹不存在)
		   nRet = CSFSMove("/example1", "/example2", "Rf");
		   // 将文件夹移动到另一个文件夹下面(example2 文件夹存在)
		   nRet = CSFSMove("/example1", "/example2", "Rf");
*/
int CSFSMove(const char *pSrcPath, const char *pDestPath, char *pOptions);

/**
@brief     递归删除文件夹(删除文件夹及其子文件和子文件夹)
@param[in] pcPathName  文件夹全路径名，路径命名规则见前面说明[路径命名规则注]
@return    返回是否成功，== 0 成功，< 0 失败

@example
		   int nRet = CSFSRmdirRecrusive("/example");
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
*/
int CSFSRmdirRecrusive(const char * pcPathName);

/**
@brief     递归删除文件夹内容
@param[in] pcPathName  文件夹全路径名，路径命名规则见前面说明[路径命名规则注]
@return    返回是否成功，== 0 成功，< 0 失败

@example
		   int nRet = CSFSRmdirContents("/example");
           if(nRet == 0)
           {
               //TODO: 成功
           }
           else
           {
	           //TODO: 失败
           }
*/
int CSFSRmdirContents(const char * pcPathName);

int CSFSRegisterRomFs(CSFSRomfsInfo_t *pRomfsInfo);

/****************************************************************************  
*   CSFSAddFlashMemToFs														    
*   Function:		添加供JFFS用的flash memory块,支持配置层的调用								    
*   Input:	pcDeviceName :要增加Device名字，供mount中使用	
*			pStartAddr: 某一块的起始地址
*			nSize:该块的大小	
*
*   Return:		如果成功，返回0；否则，返回错误代码.	    
.									    

****************************************************************************/ 
int CSFSAddFlashMemToFs( const char *pcDeviceName, void* pStartAddr, int nSize);

/*清空由CSFSAddFlashMemToFs添加的flash空间信息*/
int CSFSFreeFsDeviceMem( const char *pcDeviceName);
/**
* CSFS中识别的路径向驱动识别的路径的转换
* Input : szCsfsPath : 需要CSFS识别的路径
* Ountput: szDriverPath : 转换得到的驱动识别的路径
* Return : 0,成功；-1，返回失败
*/
int CSFSPath2DriverPath(const char * szCsfsPath,int nDriverNameLen,char *szDriverPath);

int CSFSRegisterFS(char* szFSName, CSFSOperation_t *psFSOps);
int CSFSDirBindFS(char * szDir,char * szFSName);

/**
@brief     	注册签名和校验算法接口给fs模块，用于打开、读、写等操作文件时做完整性校验接口，成功返回0，失败返回-1；
*/
int CSFSRegisterEncryptMethod(CSFSEncryptMethod_S *pfnEncrypt);

#ifdef __cplusplus
}
#endif

#endif

