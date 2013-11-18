#ifndef _CS_FSAPI_H_
#define _CS_FSAPI_H_

#include "mmcp_typedef.h"


#ifdef __cplusplus
extern "C" {
#endif
/*��Ŀ¼�µĶ���Ϊ��ϵͳ��Ŀ¼������Ҫ����ϵͳ�µ�"/home/usr000"Ŀ¼���봫��"/workdir/home/usr000"��·����*/
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

/*�������ߴ��ļ��ı�־����*/
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
/*linuxƽ̨��st_atime�����������ʱ��������/usr/include/bits/stat.h��
st_atime�������st_atim.tv_sec�������������ʱ�ᱨ�����յķ����ǸĶ�
CSFSFileStat_s�г�Ա���������֣����ǹ������󣬶�ʱ���ڸĲ��ã���������
ô�Ķ���֤linuxƽ̨�Ͽ��Ա���ͨ���������ֵ��������޽𻪸��𣬴���ǰ�ĺ�*/
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
	char cDeviceName[CSFS_DEVICENAME_LEN];		/*�豸��*/
	char cDeviceTypeName[CSFS_DEVICENAME_LEN];	/*�豸��Ӧ�ļ�ϵͳ����*/
	char cMountedDir[CSFS_MOUNTDIR_LEN];		 /*�豸����Ŀ¼*/
	int nDeviceType;					 /*�豸���ͣ��磬romfs 0x01*/
	int nDeviceID;					/*�豸ID�ţ��磬rom0 0x0100*/
};

struct CSFSRomfsInfo_s
{
	int 		m_nROMUnits;			/*address counts in m_ppcROMUnitsArray*/
	int 		m_nROMSizePerUnit;	/*length in bytes of m_ppcROMUnitsArray[i]*/
	char ** 	m_ppcROMUnitsArray;	/*ROM contents marix address*/
};

typedef enum _CSFS_Ramdisk_Type_E{
	CSFS_RAMDISK_TYPE_INNER = 			1,			/*fs�ڲ�����*/
	CSFS_RAMDISK_TYPE_OUTER = 			2			/*�ⲿ���䴫��*/
}CSFS_Ramdisk_Type_E;

struct CSFSRamfsMountOpt_s
{
	int 		m_nRamDiskType;		/*CSFS_RAMDISK_TYPE_INNERΪfs�ڲ����䣬CSFS_RAMDISK_TYPE_OUTERΪ�ⲿ���䴫�룬����ֵ��ʱΪ�Ƿ�*/
	int 		m_nRamDiskSizeKbytes;	/*m_ucpRamDisk�Ŀռ��С������Ҫ�ڲ�����Ĵ�С*/
	BYTE* 	m_ucpRamDisk;			/*m_nRamDiskTypeΪCSFS_RAMDISK_TYPE_OUTERʱramdisk��ָ��*/
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
	 int (*addFsRoot)(char* szFsRoot);		//�����ļ�ϵͳ���ʸ�·��,�ò���ָ�CSFSBindDir�ӿڵ���
	 int (*Path2DriverPath)(const char * szFsPath, int nDriverPathLen, char * szDriverPath);
};

typedef struct CSFSOperation_s  	CSFSOperation_t;

/**
@brief     	ע��ǩ����У���㷨�ӿڸ�fsģ�飬���ڴ򿪡�����д�Ȳ����ļ�ʱ��������У��ӿ�
*/
typedef struct
{
	BOOL (*m_fnSign)(char *pcFileName, unsigned char *pucData, int nLen);
    BOOL (*m_fnVerify)(char *pcFileName, unsigned char *pucData, int nLen);
    void (*m_fnNotifyUnlinkFile)(char *pcFileName);
}CSFSEncryptMethod_S;

void CSFSSetCWD(const char *pcCurrentPath);

/**
@brief     �ļ�ϵͳ��ʼ��
@param[in] nRootDevice  ���豸
@param[in] nRamFsKBytes �ڴ��ļ�ϵͳ��С(��λKB)
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
           int nRet = CSFSInit(0x100, 1024);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }

*/
int CSFSInit(int nRootDevice, int nRamFsKBytes);

/**
@brief     �ļ�ϵͳ��ʼ��(����UDIFS֧�֣����UDI2.0)
@param[in] nRootDevice  ���豸
@param[in] nRamFsKBytes �ڴ��ļ�ϵͳ��С(��λKB)
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
           int nRet = CSFSInitWithoutUDIFS(0x100, 1024);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }

*/
int CSFSInitWithoutUDIFS(int nRootDevice, int nRamFsKBytes);

/**
@brief mount  ���ط����ӿ�

@param[in] pcDeviceName Դ�豸
@param[in] pcDir ���ص�
@param[in] pcFilesystemType �ļ�ϵͳ����
@param[in] dwMountFlags ���ر�־
@param[in] pData ���ز���
	��mount jffs�ļ�ϵͳʱ:
	����-delete_no_parent_file	��ʾ����mount ʱ���ж�Դ�豸�����ļ�����;
*/

int CSFSMount( const char *pcDeviceName,
		                            const char * pcDir ,
		                            const char * pcFilesystemType, 
		                            DWORD  dwMountFlags , 
		                            const void * pData);



   
int CSFSUmount(const char *pcDir);

/*·����������ע*****************************************************

CS�ļ�ϵͳ: �м���Լ����ļ�ϵͳ��ֱ���Ծ����·��Ϊ·��������
            /test/datadir
            /jffs/data.txt
            
OS�ļ�ϵͳ: ����ϵͳ���ļ�ϵͳ����Ҫ�ھ���·��ǰ�����"/workdir"����
            /workdir/mnt/hd/HDD0/test/datadir
            /workdir/e:/test/data.txt

*********************************************************************/


/**
@brief     ��ָ��·������һ���ļ���
@param[in] pcPathName ȫ·������·�����������ǰ��˵��[·����������ע]
@param[in] nMode ģʽ��ȡֵ CSFS_O_MODE
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
           int nRet = CSFSMkdir("/example", CSFS_O_MODE);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }

*/
int CSFSMkdir(const char * pcPathName, int nMode);

/*Just function such as 'mkdir -p' on linux platform*/
int CSFSMkdirP(const char * pcPathName, int nMode);

/**
@brief     ��ָ��·������һ���ļ�
@param[in] pcPathName ȫ·������·�����������ǰ��˵��[·����������ע]
@param[in] nFlags �����ļ���־��ȡֵΪ
           CSFS_O_ACCMODE 
           CSFS_O_RDONLY   ֻ����ֻ�ܽ��ж�����������д��������ʧ��
           CSFS_O_WRONLY   ֻд��ֻ�ܽ���д���������ж���������ʧ��
           CSFS_O_RDWR     ��д���ɶ��ļ����ж�д����
           CSFS_O_CREAT    ����, ����ļ�������򿪣��������򴴽�
           CSFS_O_EXCL    
           CSFS_O_TRUNC    ����ļ������������
           CSFS_O_APPEND   �ļ�ĩβ׷��
           �ļ���־����������ϣ�������������¼���:
           CSFS_O_RDONLY  ��ֻ����ʽ���Ѵ��ڵ��ļ�
           CSFS_O_WRONLY  ��ֻд��ʽ���Ѵ��ڵ��ļ�
           CSFS_O_RDWR    �Զ�д��ʽ���Ѵ��ڵ��ļ�
           CSFS_O_CREAT | CSFS_O_WRONLY  ��ֻд��ʽ���ļ����ļ��������򴴽�
           CSFS_O_CREAT | CSFS_O_RDWR    �Զ�д��ʽ���ļ����ļ��������򴴽�
           CSFS_O_CREAT | CSFS_O_RDWR | CSFS_O_TRUNC  �Զ�д��ʽ���ļ����ļ��������򴴽������������
@param[in] nMode ģʽ��ȡֵ CSFS_O_MODE
@return    �����ļ������� nFile, nFile >= 0 ��Ч���ļ�������
                                 nFile < 0  ��Ч���ļ�������

@example
           int nFile = CSFSOpen("/example", CSFS_O_CREAT | CSFS_O_RDWR | CSFS_O_TRUNC, CSFS_O_MODE);
           if(nFile >= 0)
           {
               //TODO: ��Ч���ļ�������
           }
           else
           {
	           //TODO: ��Ч���ļ�������
           }

*/
int CSFSOpen(const char * pcPathName, int nFlags,int nMode);

/**
@brief     ��ָ��·������һ���ļ�
@param[in] pcPathName ȫ·������·�����������ǰ��˵��[·����������ע]
@param[in] pcMode �����ļ���־������C99��׼������, ȡֵΪ
           r   ��ֻ���ļ������ļ��������
           r+  �򿪿ɶ�д���ļ������ļ��������
           w   ��ֻд�ļ������ļ��������ļ�������Ϊ0�����ļ��������������ļ�
           w+  �򿪿ɶ�д�ļ������ļ��������ļ�������Ϊ�㣬���ļ��������������ļ�
           a   �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β
           a+  �Ը��ӷ�ʽ�򿪿ɶ�д���ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β��@param[in] nMode ģʽ��ȡֵ CSFS_O_MODE
@return    �����ļ������� nFile, nFile >= 0 ��Ч���ļ�������
                                 nFile < 0  ��Ч���ļ�������

@example
           int nFile = CSFSFopen("/example", "w+");
           if(nFile >= 0)
           {
               //TODO: ��Ч���ļ�������
           }
           else
           {
	           //TODO: ��Ч���ļ�������
           }

*/
int CSFSFopen(const char * pcPathName, const char * pcMode);

/**
@brief     ���ļ�
@param[in] nFileDescriptor �ļ�������(��CSFSOpen �� CSFSFopen �ӿڻ�ȡ����Чֵ >= 0)
@param[in] pcBuf  ��Ŵ��ļ���ȡ���ݵĻ�����
@param[in] uCount ���ļ���ȡ���ݳ��ȣ���ֵҪ <= pcBuf�ĳ���
@return    ����ʵ�ʶ�ȡ�����ݳ���nLen��nLen == uCount ��ȡ�ɹ�
                                       nLen < uCount && nLen >= 0 �ļ�����
                                       nLen < 0 ��ȡʧ��

@example
		   #define COUNT 512
		   char pcBuf[COUNT];
		   int nFile, nLen;
		   nFile = CSFSOpen("/example", CSFS_O_RDONLY, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // ���ļ�ʧ��
           }
           int nLen = CSFSRead(nFile, pcBuf, COUNT);
           if(nLen <= COUNT && nLen >= 0)
           {
               //TODO: ��ȡ�ɹ�
           }
           else
           {
	           //TODO: ��ȡʧ��
           }
           CSFSClose(nFile);

*/
int CSFSRead(int nFileDescriptor, char * pcBuf, unsigned int uCount);

/**
@brief     д�ļ�
@param[in] nFileDescriptor �ļ�������(��CSFSOpen �� CSFSFopen �ӿڻ�ȡ����Чֵ >= 0)
@param[in] pcBuf  Ҫд���ļ������ݻ���
@param[in] uCount Ҫд���ļ������ݳ��ȣ���ֵҪ <= pcBuf�ĳ���
@return    ����ʵ��д������ݳ���nLen��nLen == uCount д��ɹ�
                                       nLen < uCount  д��ʧ��

@example
		   #define COUNT 512
		   char pcBuf[COUNT]; // �������ڸû����Ѿ���ź��˴�д�������
		   int nFile, nLen;
		   nFile = CSFSOpen("/example", CSFS_O_CREAT | CSFS_O_RDWR | CSFS_O_TRUNC, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // ���ļ�ʧ��
           }
           int nLen = CSFSWrite(nFile, pcBuf, COUNT);
           if(nLen == COUNT)
           {
               //TODO: д��ɹ�
           }
           else
           {
	           //TODO: д��ʧ��
           }
           CSFSClose(nFile);

*/
int CSFSWrite(int nFileDescriptor, char * pcBuf, unsigned int uCount);

/**
@brief     �ر��ļ�
@param[in] nFileDescriptor �ļ�������(��CSFSOpen �� CSFSFopen �ӿڻ�ȡ����Чֵ >= 0)
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_CREAT | CSFS_O_RDWR | CSFS_O_TRUNC, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // ���ļ�ʧ��
           }
           nRet = CSFSClose(nFile);
           if(nRet < 0)
           {
               //TODO: �ر��ļ�ʧ��
           }
           else
           {
	           //TODO: �ر��ļ��ɹ�
           }
*/
int CSFSClose(int nFileDescriptor);

/**
@brief     �ƶ��ļ�ָ��
@param[in] nFileDescriptor �ļ�������(��CSFSOpen �� CSFSFopen �ӿڻ�ȡ����Чֵ >= 0)
@param[in] lOffset  �ƶ�ƫ����
@param[in] uOrigin  �ƶ���ʼλ�ã�ȡֵΪ
                    CSFS_SEEK_SET  �ļ���ͷ
                    CSFS_SEEK_CUR  ��ǰλ��
                    CSFS_SEEK_END  �ļ�ĩβ
@return    �����ƶ����ָ��λ�ã�>= 0 ��Чλ�ã�< 0 ��Чλ��

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_RDONLY, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // ���ļ�ʧ��
           }
           
           // �ƶ����ļ���ͷ
		   nRet = CSFSLseek(nFile, 0, CSFS_SEEK_SET);
           if(nRet < 0)
           {
               //TODO: �ƶ�ʧ��
           }
           else
           {
	           //TODO: �ƶ��ɹ�
           }

           // �ƶ����ļ�ĩβ(�жϷ���ֵʡ��)
		   nRet = CSFSLseek(nFile, 0, CSFS_SEEK_END);
           // ���ļ���ͷ����ƶ�10���ֽ�(�жϷ���ֵʡ��)
		   nRet = CSFSLseek(nFile, 10, CSFS_SEEK_SET);
 		   // ���ļ�ĩβ��ǰ�ƶ�10���ֽ�(�жϷ���ֵʡ��)
		   nRet = CSFSLseek(nFile, -10, CSFS_SEEK_END);
		   // ���ļ���ǰλ������ƶ�10���ֽ�(�жϷ���ֵʡ��)
		   nRet = CSFSLseek(nFile, 10, CSFS_SEEK_CUR);
		   // ���ļ���ǰλ����ǰ�ƶ�10���ֽ�(�жϷ���ֵʡ��)
		   nRet = CSFSLseek(nFile, -10, CSFS_SEEK_CUR);
           CSFSClose(nFile);
*/
int CSFSLseek(int nFileDescriptor, long lOffset, unsigned int uOrigin);

int CSFSLlseek(int nFileDescriptor, unsigned long  dwOffsetHigh,unsigned long  dwOffsetLow, long * plResult, unsigned int uOrigin);

/**
@brief     ��ȡ��ǰ�ļ�ָ��λ��
@param[in] nFileDescriptor �ļ�������(��CSFSOpen �� CSFSFopen �ӿڻ�ȡ����Чֵ >= 0)
@return    ���ص�ǰ�ļ�ָ��λ�ã�>= 0 ��Чλ�ã�< 0 ��Чλ��

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_CREAT | CSFS_O_RDWR, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // ���ļ�ʧ��
           }
           nRet = CSFSFtell(nFile);
           if(nRet < 0)
           {
               //TODO: ��Чλ��
           }
           else
           {
	           //TODO: ��Чλ��
           }
           CSFSClose(nFile);
*/
long CSFSFtell(int nFileDescriptor);

/**
@brief     �������е�����ˢ�µ�ʵ�ʴ洢�豸(��: Ӳ�̡�U��)
           Ϊ����߶�дЧ�ʣ�����ϵͳ�����������������д��ʵ�ʴ洢�豸����
           ���ڴ��еĻ����У��ȵ�һ������һ����д��ʵ�ʴ洢�豸
@param[in] nFileDescriptor �ļ�������(��CSFSOpen �� CSFSFopen �ӿڻ�ȡ����Чֵ >= 0)
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_RDONLY, CSFS_O_MODE);
           if(nFile < 0)
           {
               return; // ���ļ�ʧ��
           }
           nRet = CSFSFflush(nFile);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
           CSFSClose(nFile);
*/
int CSFSFflush(int nFileDescriptor);

/**
@brief     ɾ���ļ�
@param[in] pcPathName ȫ·������·�����������ǰ��˵��[·����������ע]
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
		   int nRet;
           nRet = CSFSUnlink(pcPathName);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
*/
int CSFSUnlink(const char * pcPathName);

/**
@brief     ɾ���ļ���
@param[in] pcPathName ȫ·������·�����������ǰ��˵��[·����������ע]
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
		   int nRet;
           nRet = CSFSRmdir(pcPathName);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
*/
int CSFSRmdir(const char * pcPathName);

/**
@brief     ɾ���ļ���
@param[in] pcOldPath �����������ļ�ȫ·����·�����������ǰ��˵��[·����������ע]
@param[in] pcNewPath ��������������֣�·�����������ǰ��˵��[·����������ע]
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
		   int nRet;
           nRet = CSFSRename(pcOldPath, pcNewPath);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
*/
int CSFSRename(const char * pcOldPath, const char * pcNewPath);

int CSFSChmod(const char * pcPathName, int nMode);

int CSFSAccess(const char *pcPathName,int i_Mode);

int CSFSFchmod(int nFileDescriptor, int nMode);

int CSFSChown(const char * pcPathName, int nUidOwner, int nGidGroup);

int CSFSFchown(int nFileDescriptor, int nUidOwner, int nGidGroup);

/**
@brief     ��ȡ�ļ�״̬����
@param[in] pcFileName �ļ���ȫ·����·�����������ǰ��˵��[·����������ע]
@param[in] psFileStat �����ļ����ԵĽṹ������
					  struct CSFSFileStat_t
					  {
					      USHORT st_dev;
						  USHORT st_ino;
						  USHORT st_mode;     �ļ����ͣ�CSFS_S_ISDIR(st_mode) != 0 ���ļ���һ���ļ���
						                                CSFS_S_ISREG(st_mode) != 0 ���ļ���һ����ͨ�ļ�
						  USHORT st_nlink;
						  USHORT st_uid;
						  USHORT st_gid;
						  USHORT st_rdev;
						  ULONG  st_size;     �ļ���С
						  ULONG  st_blksize;  �ļ�ϵͳ���С(��512�ֽ�)
						  ULONG  st_blocks;   ռ�õĿ����
						  ULONG  st_atime;
						  ULONG  st_mtime;
						  ULONG  st_ctime;
					  };
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��(�����ļ�������)

@example
		   int nRet;
		   CSFSFileStat_t psFileStat;
           nRet = CSFSStat(pcFileName, &psFileStat);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
*/
int CSFSStat(const char * pcFileName, CSFSFileStat_t *psFileStat);

/**
@brief     ��ȡ�ļ�״̬����
@param[in] nFileDescriptor �ļ�������(��CSFSOpen �� CSFSFopen �ӿڻ�ȡ����Чֵ >= 0)
@param[in] psFileStat �����ļ����ԵĽṹ����[CSFSStat]�ӿ�
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��(�����ļ�������)

@example
		   int nFile, nRet;
		   nFile = CSFSOpen("/example", CSFS_O_RDONLY, CSFS_O_MODE);
		   CSFSFileStat_t psFileStat;
           nRet = CSFSFstat(nFile, &psFileStat);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
*/
int CSFSFstat(int nFileDescriptor, CSFSFileStat_t * psFileStat);

int CSFSLstat(const char * pcFileName, CSFSFileStat_t *psFileStat);

/**
@brief     ��һ���ļ��У����ڶ�ȡ���ļ���������ļ������ļ���
@param[in] pcDirName  ȫ·������·�����������ǰ��˵��[·����������ע]
@return    �����ļ��еľ����== NULL ʧ��

@example
		   CSFSDir_t * pDir;
		   pDir = CSFSOpenDir("/example");
           if(pDir == NULL)
           {
               //TODO: ʧ��
           }
           else
           {
	           //TODO: �ɹ�
           }
*/
CSFSDir_t * CSFSOpenDir (const char *pcDirName);

/**
@brief     ��ȡ�ļ���������ļ������ļ��У�����һ�ζ�ȡһ�����ļ�
@param[in] psDir  ����ȡ�ļ��еľ��
@return    ���ض�ȡ���ļ���Ϣ��== NULL ʧ�ܣ�����
		   struct CSFSDirent_s
		   {
		       unsigned long     m_uiDirInod;
		       unsigned long     m_lDirOff;	       �ļ����ļ����µ����
		       unsigned short    m_usDirRecLen;    δʹ��
		       unsigned char     m_ucDirType;      �ļ����ͣ�0 Ϊ�ļ���1 Ϊ�ļ���
		       char				 m_cDirName[256];  �ļ����ļ�������
		   };

@example
		   CSFSDir_t * pDir;
		   CSFSDirent_t * pDirent;
		   pDir = CSFSOpenDir("/example");
		   pDirent = CSFSReadDir(pDir);
		   while(pDirent != NULL)
		   {
		       // �ɹ�
		       pDirent = CSFSReadDir(pDir); // ������ȡ�����ļ�
		   }
		   // whileѭ���˳���˵�����ļ���������ļ���������
*/
CSFSDirent_t * CSFSReadDir(CSFSDir_t *psDir);

/**
@brief     �ر�һ���ļ���
@param[in] psDir  �ļ��о��
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
		   int nRet;
		   CSFSDir_t * pDir;
		   pDir = CSFSOpenDir("/example");
		   nRet = CSFSCloseDir(pDir);
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
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
/*�õ��Ѿ����ص��豸�ĸ���*/
int CSFSGetDeviceCount( int * pnCount, int*  pnDeviceId);

/*�����豸�ŵõ��Ѿ����ص�ĳ���豸����Ϣ*/
int CSFSGetDeviceInfo( int nDeviceId,CSFSDeviceInfo_t *psDeviceInfo);
/*��ʽ��ĳ���豸Ϊ�ض����͵ķ���������Ӳ�̺����̵ȴ������洢�豸�����ڸ�ʽ����fat��e2fs�ĸ�ʽ��Ŀǰflash��ֻ�ܱ���ʽ����jffs��ʽ�ķ�����*/
int CSFSFormat(const char *pcDeviceName, const char *pcFSTypeName, void *pParam );

void CSFSBrowseDir(const char *pcDirName,int*pnFileCount,int * pnDirCount);

void CSFSBrowseDirWithLevel(const char *pcDirName,int*pnFileCount,int * pnDirCount,int nInfoLevel);

/**
@brief     �����ļ����ļ���
@param[in] pSrcPath  Դ�ļ�ȫ·������·�����������ǰ��˵��[·����������ע]
@param[in] pDestPath Ŀ���ļ�ȫ·������·�����������ǰ��˵��[·����������ע]
@param[in] pOptions  ����ֵΪ NULL ʱ���ýӿڿ���ʱ������ͬ���ֵ��ļ�ʱ�򷵻� -EEXIST
                     ����ֵΪ "Rf"(�����ִ�Сд)ʱ����������ͬ���ֵ��ļ��򸲸�
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
           // ���ļ�������������
		   int nRet = CSFSCopy("/example1/data1.txt", "/example2/data2.txt", "Rf");
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
           // ���ļ���������һ�ļ���
		   nRet = CSFSCopy("/example1/data1.txt", "/example2", "Rf");
		   // ���ļ��п�����������(example2 �ļ��в�����)
		   nRet = CSFSCopy("/example1", "/example2", "Rf");
		   // ���ļ��п�������һ���ļ�������(example2 �ļ��д���)
		   nRet = CSFSCopy("/example1", "/example2", "Rf");
*/

int CSFSCopy(const char *pSrcPath, const char *pDestPath, char *pOptions);

/**
@brief     �ƶ��ļ����ļ���
@param[in] pSrcPath  Դ�ļ�ȫ·������·�����������ǰ��˵��[·����������ע]
@param[in] pDestPath Ŀ���ļ�ȫ·������·�����������ǰ��˵��[·����������ע]
@param[in] pOptions  ����ֵΪ NULL ʱ���ýӿ��ƶ�ʱ������ͬ���ֵ��ļ�ʱ�򷵻� -EEXIST
                     ����ֵΪ "Rf"(�����ִ�Сд)ʱ����������ͬ���ֵ��ļ��򸲸�
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
           // ���ļ��ƶ���������
		   int nRet = CSFSMove("/example1/data1.txt", "/example2/data2.txt", "Rf");
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
           // ���ļ��ƶ�����һ�ļ���
		   nRet = CSFSMove("/example1/data1.txt", "/example2", "Rf");
		   // ���ļ����ƶ���������(example2 �ļ��в�����)
		   nRet = CSFSMove("/example1", "/example2", "Rf");
		   // ���ļ����ƶ�����һ���ļ�������(example2 �ļ��д���)
		   nRet = CSFSMove("/example1", "/example2", "Rf");
*/
int CSFSMove(const char *pSrcPath, const char *pDestPath, char *pOptions);

/**
@brief     �ݹ�ɾ���ļ���(ɾ���ļ��м������ļ������ļ���)
@param[in] pcPathName  �ļ���ȫ·������·�����������ǰ��˵��[·����������ע]
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
		   int nRet = CSFSRmdirRecrusive("/example");
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
*/
int CSFSRmdirRecrusive(const char * pcPathName);

/**
@brief     �ݹ�ɾ���ļ�������
@param[in] pcPathName  �ļ���ȫ·������·�����������ǰ��˵��[·����������ע]
@return    �����Ƿ�ɹ���== 0 �ɹ���< 0 ʧ��

@example
		   int nRet = CSFSRmdirContents("/example");
           if(nRet == 0)
           {
               //TODO: �ɹ�
           }
           else
           {
	           //TODO: ʧ��
           }
*/
int CSFSRmdirContents(const char * pcPathName);

int CSFSRegisterRomFs(CSFSRomfsInfo_t *pRomfsInfo);

/****************************************************************************  
*   CSFSAddFlashMemToFs														    
*   Function:		��ӹ�JFFS�õ�flash memory��,֧�����ò�ĵ���								    
*   Input:	pcDeviceName :Ҫ����Device���֣���mount��ʹ��	
*			pStartAddr: ĳһ�����ʼ��ַ
*			nSize:�ÿ�Ĵ�С	
*
*   Return:		����ɹ�������0�����򣬷��ش������.	    
.									    

****************************************************************************/ 
int CSFSAddFlashMemToFs( const char *pcDeviceName, void* pStartAddr, int nSize);

/*�����CSFSAddFlashMemToFs��ӵ�flash�ռ���Ϣ*/
int CSFSFreeFsDeviceMem( const char *pcDeviceName);
/**
* CSFS��ʶ���·��������ʶ���·����ת��
* Input : szCsfsPath : ��ҪCSFSʶ���·��
* Ountput: szDriverPath : ת���õ�������ʶ���·��
* Return : 0,�ɹ���-1������ʧ��
*/
int CSFSPath2DriverPath(const char * szCsfsPath,int nDriverNameLen,char *szDriverPath);

int CSFSRegisterFS(char* szFSName, CSFSOperation_t *psFSOps);
int CSFSDirBindFS(char * szDir,char * szFSName);

/**
@brief     	ע��ǩ����У���㷨�ӿڸ�fsģ�飬���ڴ򿪡�����д�Ȳ����ļ�ʱ��������У��ӿڣ��ɹ�����0��ʧ�ܷ���-1��
*/
int CSFSRegisterEncryptMethod(CSFSEncryptMethod_S *pfnEncrypt);

#ifdef __cplusplus
}
#endif

#endif

