/**@defgroup FS FS 定义平台文件系统相关的接口,主要是U盘、移动硬盘等的访问接口
@brief FS即文件系统模块，包括了移植层文件系统的读写、设备发现等接口，对应头文件<udi2_fs.h >。

本模块主要负责管理和操作外置存储设备，如U盘、移动硬盘、内置硬盘以及Flash上的文件系统。

当MMCP（CloudTV ）中间件平台启动或者初始化完成后，将可移动的存储设备插入到终端的USB口时，本模块通过注册的回调通知MMCP（CloudTV ）中间件平台。

当MMCP（CloudTV ）中间件平台还没有启动或者完成初始化之前，将可移动的存储设备插入到终端的USB口时，由于MMCP（CloudTV ）中间件平台会在第一次启动时通过GetAllDeviceId获取所有已经存在的设备，所以如果设备是永久存在的（如内置硬盘、Flash中的文件系统），则发现设备时不立即通知。

本模块处理的设备分为存储设备（U盘、Flash、硬盘等）和分卷设备（分区）两种。一个存储设备可能包含0至n个分卷设备，其中0个表示存储设备未格式化。

发现分区设备时，会将分区的MountPoint即根位置反馈给MMCP（CloudTV ）中间件平台，MMCP（CloudTV ）中间件平台在根位置上加上文件的相对路径，再通过本模块提供的文件操作接口进行读写等操作。
一个正常的U盘等设备插入后，消息抛出的流程应该为：Found(存储设备)'Ready(存储设备) 'Found(分卷1) 'Ready（分卷1）'Found(分卷2) 'Ready（分卷2）。如果存储设备上有多个分卷设备，其顺序是不确定的，允许不同分卷设备的Found和Ready之间存在交错。如果U盘需要自检，其分卷设备的Found和Ready消息之间，可插入若干个Scaning消息。如果U盘本身或者某个分卷无法识别（如未格式化，或存在不识别的文件系统格式等），则其Ready消息被Error消息替代。

一个正常的U盘等设备被拔出后，消息抛出的流程应该为：Plugout(分卷1) ' Plugout(分卷2) '……'Plugout(分卷n)'Plugout（存储设备)。即先拔出分卷设备，再拔出存储设备。

所有以上描述，均与RDI模块描述的规则保持一致，并在RDI基础上进行的细化。

原则上讲，USB设备的发现、访问和文件系统之间，不存在强耦合关系。在Linux等操作系统上，U盘等也是以块设备读写的形式提供驱动层接口，然后由操作系统完成文件系统的解析等工作。但考虑到大多数平台上，特别是Linux系统上，芯片往往以文件系统的形式提供了存储设备的访问接口，并且已经做得很好了。如果再严格的要求按照USB通用设备的形式提供块设备读写的形式的原始接口，一方面不容易实现，另一方面也造成MMCP（CloudTV ）中间件平台和驱动层存在两套文件系统解析代码，浪费空间。

@note 本模块文件操作相关接口详细操作说明请参考C99规范类似接口

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.6 2009/10/22 修正分卷的类型枚举定义不统一的问题
@version 2.0.5 2009/09/15 修正CSUDIFSGetAllDeviceId中pnActIdCnt可以为CSUDI_NULL的错误描述
@version 2.0.4 2009/09/08 修改可移动设备的发现规则使之符合RDI模块的定义
@version 2.0.3 2009/8/24 第一次综合评审后修改的版本
@version 2.0.2 2009/07/27 UDI2第二次评审版本
@version 2.0.1 2009/04/02 UDI2初稿，根据原Kernel模块中相关代码提取修改而来,经初步讨论的版本
@{
*/

#ifndef _UDI2_FS_H_
#define _UDI2_FS_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDIFS_CP_DEFAULT		(0)           ///< 默认编码方式,当移植层不确定文件系统使用的编码方式时使用该模式,这种情况下可能会产生乱码
#define CSUDIFS_CP_GB2312	    (936)         ///< GB2312编码
#define CSUDIFS_CP_BIG5			(950)         ///< big5编码
#define CSUDIFS_CP_UTF8			(65001)       ///< UTF8

#define CSUDIFS_ENCODE_BIG_ENDIAN     (0x10000)     ///< bigendian,即高位在前
#define CSUDIFS_ENCODE_LITTLE_ENDIAN  (0)           ///< littelendian,即低位在前

#define CSUDIFS_SEEK_SET 	(0)	///< 从文件开头处seek
#define CSUDIFS_SEEK_CUR 	(1)	///< 从文件当前位置seek
#define CSUDIFS_SEEK_END  	(2) 	///< 从文件结尾处seek

#define CSUDIFS_MODE_REG 	(0x1)		///< 普通文件，见CSUDIFSFileStat_S结构的m_lMode域
#define CSUDIFS_MODE_DIR	(0x1 << 1) 	///< 文件夹，CSUDIFSFileStat_S结构的m_lMode域

#define CSUDIFS_ISREG(m)  (((m) & CSUDIFS_MODE_REG) == CSUDIFS_MODE_REG)	///< 判断是否为普通文件,用于CSUDIFSFileStat_S结构的m_lMode域
#define CSUDIFS_ISDIR(m)  (((m) & CSUDIFS_MODE_DIR) == CSUDIFS_MODE_DIR)	///< 判断是否为文件夹,用于CSUDIFSFileStat_S结构的m_lMode域


/**@brief 设备操作相关错误代码值*/
enum
{
	CSUDIFS_ERROR_BAD_PARAMETER = CSUDI_FS_ERROR_BASE,    ///< 错误参数
	CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED,				  ///< 不支持操作
	CSUDIFS_ERROR_UNKNOWN_ERROR,				 	 	  ///< 一般错误
	CSUDIFS_ERROR_NO_MEMORY,							  ///< 无内存可用错误
	CSUDIFS_ERROR_INVALID_DEVICE_ID,	    			  ///< 非法设备ID
	CSUDIFS_ERROR_DEVICE_BUSY, 							  ///< 设备繁忙状态
	CSUDIFS_ERROR_NO_CALLBACK,                           ///<  没有相应的回调函数可以删除
	CSUDIFS_ERROR_ALREADY_ADDED							  ///< 该回调已经注册
};

/**@brief 分卷的类型*/
typedef enum 
{
	EM_UDIFS_PARTITION_AUTO = 0,    	///< 自动类型,由平台决定使用支持的最好的类型，从底层获取的文件系统类型不得为该值
	
	EM_UDIFS_PARTITION_READONLY_START,///< 只读文件系统边界，它与EM_UDIFS_PARTITION_READONLY_END之间的文件系统类型为只读文件系统，该值本身不是一种文件系统
	EM_UDIFS_PARTITION_CRAMFS,				///< CRAMFS类分区
	EM_UDIFS_PARTITION_ROMFS,				  ///< ROMFS类分区
	EM_UDIFS_PARTITION_READONLY_END = 0x100,///< 只读文件系统边界，它与EM_UDIFS_PARTITION_READONLY_START之间的文件系统类型为只读文件系统，该值本身不是一种文件系统
	
	EM_UDIFS_PARTITION_RW_START,		///< 可读写文件系统边界，它与EM_UDIFS_PARTITION_RW_END之间的文件系统类型为可读写的文件系统，该值本身不是一种文件系统
	EM_UDIFS_PARTITION_FAT32 ,      ///< FAT32类型分区
	EM_UDIFS_PARTITION_EXT2,        ///< E2FS类型分区
	EM_UDIFS_PARTITION_EXT3,			///< EXT3类型分区
	EM_UDIFS_PARTITION_JFFS2,			///< JFFS2类型分区
	EM_UDIFS_PARTITION_NTFS,        ///< NTFS类型分区
	EM_UDIFS_PARTITION_UBIFS,        ///< UBIFS类型分区
	EM_UDIFS_PARTITION_YAFFS2,        ///< YAFFS2类型分区
	EM_UDIFS_PARTITION_RW_END = 0x200///< 可读写文件系统边界，它与EM_UDIFS_PARTITION_RW_START之间的文件系统类型为可读写的文件系统，该值本身不是一种文件系统
}CSUDIFSPartitionType_E;

/**@brief 设备类型*/
typedef enum
{
    EM_UDIFS_DEVTYPE_UNKNOWN,      ///< 边界值
	EM_UDIFS_DEVTYPE_STORAGE,	   ///< 存储设备
    EM_UDIFS_DEVTYPE_VOLUME       ///< 分卷设备,如U盘的分区等(windows上的G盘,H盘概念),一个存储设备可能含有1-n个分卷设备须分别通知
}CSUDIFSDeviceType_E;

/**@brief 设备信息结构*/
typedef struct
{
    CSUDI_UINT32    	m_dwDeviceId;        ///< 设备Id,一次开机过程中设备Id唯一标识一个设备的一次插入,同一设备反复插入设备Id号发生变化	
	CSUDIFSDeviceType_E m_eDeviceType;       ///< 设备类型,可移动设备的设备类型必须要设备Id保持一致，固定设备的设备类型从该值获取
    char      	m_szDeviceName[128]; ///< 设备的唯一设备名称
    CSUDI_UINT64    	m_dwDeviceSize;      ///< 设备大小(字节为单位)
    CSUDI_UINT32    	m_dwParentDeviceId;  ///< 父设备Id,存储设备该值无效,分卷设备为其所属的存储设备的Id
    char m_szMountPoint[256];     	 ///< 如果是存储设备的该项为"";分卷设备表示该设备在UDIFS中的可访问路径,如果为""(不是为CSUDI_NULL，也不是为空格),表示这是一个不能识别的分卷
    								///<加载点不能以"/"结尾
}CSUDIFSDeviceInfo_S;	

/**@brief 文件属性

@note 和windows一样,m_lSize表示文件实际大小,m_lBlksize*m_lBlocks表示占用大小
*/
typedef struct
{
	unsigned long   m_lMode;		///< 文件类型，用于表示是文件还是文件夹等，请使用CSUDIFS_ISREG、CSUDIFS_ISDIR宏进行判断
	unsigned long   m_lSize;     	///< 文件实际大小
	unsigned long   m_lBlkSize;  	///< 块大小
	unsigned long   m_lBlkCount;    ///< 块数量
	unsigned long   m_lAtime;    	///< 访问时间,用1970年1月1日0时0分0秒至今的秒数表示
	unsigned long   m_lMtime;    	///< 修改时间,用1970年1月1日0时0分0秒至今的秒数表示
	unsigned long   m_lCtime;    	///< 创建时间,用1970年1月1日0时0分0秒至今的秒数表示
}CSUDIFSFileStat_S;

/**@brief 文件夹内容节点属性,一般代表一个文件夹或者文件*/
typedef struct 
{
	unsigned char	m_ucDirType;        ///< 节点类型0-非文件夹,1-文件夹
	char		m_cDirName[256];    ///< 节点名称
}CSUDIFSDirent_S;

/**@brief 文件系统属性*/
typedef struct
{
	        CSUDIFSPartitionType_E m_eFsType;  ///< 文件系统类型	        
	        long m_lBsize;                     ///< optimal transfer block size
	        long m_lBlocks;                     ///< total data blocks in file system
	        long m_lBfree;                  ///< free blocks in fs
	        long m_lBavail;                 ///< free blocks avail to non-superuser
	        long m_lFiles;                  ///< total file nodes in file system
	        long m_lFfree;                  ///< free file nodes in fs
	        long m_lNamelen;                ///< maximum length of filenames
}CSUDIFSStatFs_S;

/**
@brief 添加设备通知回调函数

详见RDI模块CSUDIRDIAddCallback_F接口定义，仅用于处理“STORAGE”、“VOLUME”类型的可移动设备
@param[in] fnCallback 回调函数,当设备状态发生变化时调用该函数
@param[in] pvUserData 用户数据,将作为回调函数的pUserData参数返回
@return 成功添加返回CSUDI_SUCCESS,失败返回错误代码值
@note 添加回调前产生的设备变化信息,不会补充通知.即若开机时这些设备就存在,调用本函数时不会得到通知.
@note fnNotify和pUserData一起唯一确定一个回调函数，当两次注册它们完全一样时，第二次将返回CSUDIFS_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief 删除设备通知回调函数

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“STORAGE”、“VOLUME”类型的可移动设备
@param[in] fnCallback 回调函数,之前注册的函数
@param[in] pvUserData 用户数据,必须与注册时的一致
@return 成功删除返回CSUDI_SUCCESS,失败返回错误代码值
@note fnNotify和pUserData一起唯一确定一个回调函数

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief 安全移除可移动存储设备

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“STORAGE”、“VOLUME”类型的可移动设备
@param[in] dwDeviceId 目标设备ID
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 只能删除可移动设备，试图删除其它设备类型则返回CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED，通过设备的设备Id可判断设备类型
@note 删除设备后不可再通过该设备Id获取设备信息
@note 用户调用该接口会触发EM_UDIRDI_PLUGOUT_SAFE类型的EM_UDIRDI_EVENT_PLUGOUT消息

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId);

/**
@brief 获取所有设备的Id

包括可移动的和固定的存储设备和分卷设备
@param[out] pdwIds 用于存储所有存在的设备的Id
@param[in] nMaxIdCnt pdwIds数组的大小,即最多能返回多少设备ID,一般来说有一个分卷的U盘会表示为2个设备,该值传入32基本能满足需求
@param[out] pnActIdCnt 实际的设备ID数，该值表示有多少设备，与拷贝到pdwIds中的个数相同
@return 成功返回CSUDI_SUCCESS，失败返回错误代码值
@note 该接口仅返回中间件需关注的设备,如U盘,移动硬盘,需在中间件中访问的cramfs设备等,其数量一般来说少于实际的设备数.
故UDI可能需要根据项目进行一些配置工作。
@note 为保持兼容，所有非可移动设备的设备号仍从0开始索引
@note 允许pdwIds为CSUDI_CSUDI_NULL，这种情况下表示应用仅关注有多少个存储设备，但不关心它们具体的Id

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt);

/**
@brief 根据设备ID获取设备信息

@param[in] dwDeviceId 目标设备ID,其值来源于CSUDIFSGetAllDeviceId或回调函数
@param[out] psDeviceInfo 指向返回设备信息内存的指针
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo);

/**
@brief 使设备进入待机状态

@param[in] dwDeviceId 目标设备ID
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 仅支持存储设备即可,分卷设备返回失败
@note 已经在待机状态下调用本接口仍返回CSUDI_SUCCESS

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId);

/**
@brief 唤醒设备

@param[in] dwDeviceId 目标设备ID
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 仅支持存储设备即可,分卷设备返回失败
@note 已经在唤醒状态下调用本接口仍返回CSUDI_SUCCESS

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId);

/**
@brief 格式化设备

@param[in] dwDeviceId 目标设备ID
@param[in] eType 拟格式化成的文件系统格式
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 传入分卷设备将格式化整个分卷
@note 传入存储设备将整个设备格式化为一个分卷,并产生原分卷(如果存在)PLUGOUT,新分卷创建的回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType);

/**
@brief 获取路径编码方式

@param[out] pnCodePage 编码字节顺序 | 编码方式,详见以上宏定义
通过该函数确定与UDIFS交互的路径的编码方式
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetCodePage(int * pnCodePage); 

/**
@brief 打开文件

@param[in] pcFileName 文件名
@param[in] pcMode 打开方式,详见C99标准
@return 文件句柄,失败返回CSUDI_NULL
@note 本函数定义类似C99标准fopen

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode);

/**
@brief 读文件

@param[in] hFile 文件句柄,CSUDIFS_Open的返回值
@param[in] pcBuf 指向存放读入数据的内存
@param[in] uCount 需读入的字节数
@return 实际读到的字节数,它可能少于uCount
@note uCount为0时返回0,文件状态不发生任何变化
@note 返回值<0表示出错或者文件到结尾了

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount);

/**
@brief 写文件

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] pcBuf 指向存放写入数据的内存
@param[in] uCount 需读入的字节数
@return 实际写入的字节数,它可能少于uCount
@note uCount为0时返回0,文件状态不发生任何变化

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount);

/**
@brief 关闭文件

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSClose(CSUDI_HANDLE hFile);

/**
@brief seek文件

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] lOffset 移动偏移
@param[in] uOrigin 移动起始位置,CSUDIFS_SEEK_SET,CSUDIFS_SEEK_CUR,CSUDIFS_SEEK_END
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin);

/**
@brief seek文件64位版

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] i64Offset 移动偏移
@param[in] uOrigin 移动起始位置,CSUDIFS_SEEK_SET,CSUDIFS_SEEK_CUR,CSUDIFS_SEEK_END
@return 成功返回0,其它情况返回-1
@note 即使文件系统不支持超过2G的文件也必须实现

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin);

/**
@brief 获取文件当前位置

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@return 成功返回文件当前距开始处的偏移,其它情况返回-1
@note 当前偏移大于long能表示的最大范围时(32位机上为2G),返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
long CSUDIFSTell(CSUDI_HANDLE hFile);

/**
@brief 获取文件当前位置64位版

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@return 成功返回文件当前距开始处的偏移,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile);

/**
@brief flush文件

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFlush(CSUDI_HANDLE hFile);

/**
@brief 删除文件

@param[in] pcFileName 文件名
@return 成功返回0,其它情况返回-1
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRemove(const char * pcFileName);

/**
@brief 重命名文件

@param[in] pcOldPath 旧文件名
@param[in] pcNewPath 新文件名
@return 成功返回0,其它情况返回-1
@note 该接口对文件夹有效
@note 该接口可以改变文件的目录,但不会创建新目录,即若pNewPath指定的目录不存在则返回失败
@note 如果pNewPath指向的文件已存在,结果是不确定的,情慎用该特性.

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath);

/**
@brief 截断文件

将一个文件截断为一定长度
@param[in] pcPathName 文件名
@param[in] ulLength 新文件长度
@return 成功返回0,其它情况返回-1
@note 如果新长度小于原文件长度,则多余的部分丢弃;如果新长度大于原文件长度,则新增部分用0填充
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength);

/**
@brief 截断文件描述符版

将一个文件截断为一定长度
@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] ulLength 新文件长度
@return 成功返回0,其它情况返回-1
@note 如果新长度小于原文件长度,则多余的部分丢弃;如果新长度大于原文件长度,则新增部分用0填充
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength);

/**
@brief 截断文件64位版

将一个文件截断为一定长度
@param[in] pcPathName 文件名
@param[in] u64Length 新文件长度
@return 成功返回0,其它情况返回-1
@note 如果新长度小于原文件长度,则多余的部分丢弃;如果新长度大于原文件长度,则新增部分用0填充
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length);

/**
@brief 截断文件描述符64位版

将一个文件截断为一定长度
@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] u64Length 新文件长度
@return 成功返回0,其它情况返回-1
@note 如果新长度小于原文件长度,则多余的部分丢弃;如果新长度大于原文件长度,则新增部分用0填充
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length);

/**
@brief 获取文件属性

@param[in] pcFileName 文件名
@param[out] psFileStat 输出文件属性
@return 成功返回0,其它情况返回-1
@note 该接口对文件夹有效
@note psFileStat->m_lMode 值必须正确。只可能是CSUDIFS_ISREG、CSUDIFS_ISDIR，不应该为其他值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat);

/**
@brief 获取文件属性文件描述符版

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[out] psFileStat 输出文件属性
@return 成功返回0,其它情况返回-1
@note 该接口对文件夹无效
@note psFileStat->m_lMode 值必须正确,即为CSUDIFS_ISREG。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat);

/**
@brief 创建文件夹

@param[in] pcPathName 文件夹名
@param[in] nReserved 保留字段,必须传0,否则须返回失败
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSMkdir(const char * pcPathName, int nReserved);

/**
@brief 删除文件夹

@param[in] pcPathName 文件夹名
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRmdir(const char * pcPathName);

/**
@brief 打开文件夹

@param[in] pcPathName 文件夹名
@return 成功返回文件夹句柄,失败返回CSUDI_NULL

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName);

/**
@brief 关闭文件夹

@param[in] hDir 文件夹句柄,CSUDIFSOpendir的返回值
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSCloseDir(CSUDI_HANDLE hDir);

/**
@brief 读取文件夹

一次读取该文件夹下的一个节点的信息,依次调用可获取该文件夹下所有文件信息
@param[in] hDir 文件夹句柄,CSUDIFSOpendir的返回值
@return 返回一个文件夹内容节点指针,没有更多节点时返回CSUDI_NULL
@note
- 该接口仅列出其子文件和文件夹，不递归
- 子文件夹包括"."和".."两个目录

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir);

/**
@brief 获取分卷信息

获取pcPathName所在分卷的分卷信息,如磁盘大小,空闲空间等
@param[in] pcPathName 分卷路径,所需获得信息的分卷的路径
@param[out] psFsStat 存储分卷信息内存指针
@return 成功返回0,其它情况返回-1
@note 该接口对文件、文件夹均有效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat);

/**
@brief 通过文件句柄获取分卷信息

获取hFile对应的文件所在分卷的分卷信息,如磁盘大小,空闲空间等
@param[in] hFile 文件句柄,即去获取该文件所在的分卷的信息
@param[out] psFsStat 存储分卷信息内存指针
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

