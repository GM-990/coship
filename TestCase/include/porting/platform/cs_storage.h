/**@defgroup storage 存储设备的发现、管理及查询接口
@brief 存储设备的发现管理及查询接口

@version 1.0 2008/12/12 初始版本
to do : SetExistHdd 因为有些项目不含有磁盘，磁盘的初始化很耗时
@{
*/
#ifndef _CS_STORAGE_H_
#define _CS_STORAGE_H_

#include "udi_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@存储设备名称最大长度*/
#define CSSTRG_NAME_SIZE (128)
/**@存储设备块的大小*/
#define CSSTRG_BLOCK_SIZE  (512)  
/**@存储设备非法索引值*/
#define CSSTRG_INVALID_INDEX (-1)
/**@存储设备非法数目*/
#define CSSTRG_INVALID_COUNT (-1)

/**@brief 存储设备的类型*/
typedef enum
{
	EM_STRG_TYPE_ATA, /**< ATA设备，包括SATA,PATA,CDROM等*/
	EM_STRG_TYPE_USB  /**< USB设备，包括U盘和USB移动硬盘等*/
}CSSTRGType_E;

/**@brief 存储设备中分区的类型*/
typedef enum 
{
	EM_STRG_PARTITION_UNKNOW = 0,/**< 不识别的类型*/
	EM_STRG_PARTITION_FAT32 ,/**< FAT32类型分区*/
	EM_STRG_PARTITION_E2FS,   /**< E2FS类型分区*/
	EM_STRG_PARTITION_NTFS,    /**< NTFS类型分区*/
	EM_STRG_PARTITION_E3FS   	/**< E3FS类型分区*/
}CSSTRGPartitionType_E;

/**@brief 存储设备信息*/
typedef struct
{
	char m_szStorage[CSSTRG_NAME_SIZE];/**< 存储设备的设备名*/
	int m_nIndex;/**< 存储设备的索引值*/
	int m_nId;      /**< 存储设备的ID标识*/
	CSSTRGType_E m_eType; /**< 存储设备的类型*/
}CSSTRGInfo_S;

/**@brief 存储设备中分区的信息*/
typedef struct
{
	char m_szPartition[CSSTRG_NAME_SIZE];/**< 分区的分区名*/
	CSSTRGPartitionType_E m_ePartitionType;	/**< 分区的类型*/
	DWORD m_dwBlockCount;/**< 分区的块数，以512字节为单位*/
}CSSTRGPartitionInfo_S;

/**@brief 存储设备的事件*/
typedef enum
{
	EM_STRG_EVT_CREATE, /**<设备发现,pEventData为CSSTRGInfo_S，nEventData未定义*/
	EM_STRG_EVT_DELETE   /**<设备拔出,pEventData为CSSTRGInfo_S，nEventData未定义*/
}CSSTRGEvent_E;



typedef int(*CSSTRGNotify_F)(const CSSTRGEvent_E eDevEvent, const void * pEventData, const int nEventData, void *pUserData);

/**
@brief 参数配置

@param[in] bIsInitSata 是否初始化SATA硬盘

@note 如果系统不存在SATA硬盘，则初始SATA硬盘会花很长时间
@note 请在初始化Storage之前调用本接口
*/
void CSSTRGConfig(BOOL bIsInitSata);

/**
@brief 本模块初始化

@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSTRGInit(void);

/**
@brief 用户注册存储设备事件

@param[in] fnNotify 注册存储设备事件的回调函数
@param[in] pUserData 注册的用户私有数据

@return 成功返回TRUE,失败返回FALSE
@note fnNotify不能为空，fnNotify和pUserData唯一表示一个注册
*/
BOOL CSSTRGRegNotify(const CSSTRGNotify_F fnNotify, const void *pUserData);

/**
@brief 用户注销存储设备事件

@param[in] fnNotify 注册存储设备事件时传递的回调函数
@param[in] pUserData 注册时传入的用户私有数据

@return 成功返回TRUE,失败返回FALSE
@note fnNotify和pUserData唯一表示一个注册
*/
BOOL CSSTRGUnregNotify(const CSSTRGNotify_F fnNotify, const void *pUserData);


/**
@brief 获取系统中存储设备的数量

@return 失败返回CSSTRG_INVALID_COUNT；否则返回系统中存储设备个数
*/
int CSSTRGGetStorageCount(void);

/**
@brief 根据ID标识获取存储设备信息

@param[in] nStorageId 存储设备ID标识
@param[out] psStorageInfo 存储设备信息的地址

@return 成功返回TRUE,失败返回FALSE
@note 一旦存储设备被系统检测到后，它的ID标识就唯一的标识了该设备。而设备的索引不是唯一的，随着设备的插入和拔出，它有可能会发生改变。
*/
BOOL CSSTRGGetStorageInfoById(const int nStorageId, CSSTRGInfo_S *psStorageInfo);

/**
@brief 根据索引获取存储设备信息

@param[in] nStorageIndex 存储设备的索引
@param[out] psStorageInfo 存储设备信息的地址

@return 成功返回TRUE,失败返回FALSE
@note 一旦存储设备被系统检测到后，它的ID标识就唯一的标识了该设备。而设备的索引不是唯一的，随着设备的插入和拔出，它有可能会发生改变。
*/
BOOL CSSTRGGetStorageInfoByIndex(const int nStorageIndex, CSSTRGInfo_S *psStorageInfo);

/**
@brief 获取存储设备的分区个数

@param[in] szStorage 指定存储设备名称

@return 失败返回CSSTRG_INVALID_COUNT；否则返回指定存储设备的个数
*/
int CSSTRGGetPartitionCount(const char* szStorage);

/**
@brief 获取存储器上分区信息

@param[in] szStorage 指定的存储设备名称
@param[in] nPartionIndex 指定的分区索引
@param[out] psPartitionInfo 输出分区信息

@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSTRGGetPartionInfo(const char* szStorage, const int nPartionIndex, CSSTRGPartitionInfo_S *psPartitionInfo);

/**
@brief 存储设备待机

@param[in] szStorage 指定的存储设备名称

@return 成功返回TRUE,失败返回FALSE
@note 执行存储设备的待机或唤醒可能会阻塞执行
*/
BOOL CSSTRGStandby(const char * szStorage);

/**
@brief 存储设备唤醒

@param[in] szStorage 指定的存储设备名称

@return 成功返回TRUE,失败返回FALSE
@note 执行存储设备的待机或唤醒可能会阻塞执行
*/
BOOL CSSTRGWakeup (const char * szStorage);

/**
@brief 对整个 存储设备进行格式化

@param[in] szStorage 指定的存储设备名称
@param[in] eType       指定的格式化的类型

@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSTRGFormat(const char * szStorage,CSSTRGPartitionType_E eType);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /*_CS_STORAGE_H_*/

