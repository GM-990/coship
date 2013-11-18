#ifndef _CS_VOLUME_H_
#define _CS_VOLUME_H_

#include "mmcp_typedef.h"
#include  "udi2_fs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@非法的逻辑卷数目*/
#define CSVLM_INVALID_COUNT (-1)
/**@逻辑卷名称最大长度*/
#define CSVLM_MAX_NAME_LEN (256)

#define CSVLM_MAX_PARTITION_NAME (128)

/**@brief 逻辑卷设备事件*/
typedef enum
{
	EM_VLM_FOUND,          /**<卷发现事件,pEventData为CSVLMInfo_S，nEventData未定义*/
	EM_VLM_UNMOUNT,     /**<卸载事件,pEventData为CSVLMInfo_S，nEventData未定义*/
	EM_VLM_MOUNT_FAIL, /**<加载失败,pEventData为CSVLMInfo_S，nEventData未定义*/
	EM_VLM_MOUTED,          /**<加载事件,pEventData为CSVLMInfo_S，nEventData未定义*/
	EM_VLM_FORMAD	/**<磁盘被格式化事件,pEventData为CSVLMInfo_S，nEventData未定义*/
}CSVLMEvent_E;

/**@brief 逻辑卷的分区类型*/
typedef enum
{
	EM_VLM_PARTITION_UNKNOW = 0,/**< 不识别的类型*/
	EM_VLM_PARTITION_FAT32 ,/**< FAT32类型分区*/
	EM_VLM_PARTITION_EXT2,   /**< EXT2类型分区*/
	EM_VLM_PARTITION_NTFS,    /**< NTFS类型分区*/
	EM_VLM_PARTITION_EXT3,   /**< EXT3类型分区*/
	EM_VLM_PARTITION_JFFS,   /**< JFFS类型分区*/
	EM_VLM_PARTITION_JFFS2,  /**< JFFS2类型分区*/
	EM_VLM_PARTITION_RAMFS,  /**< RAMFS类型分区*/
	EM_VLM_PARTITION_YAFFS2,  /**<YAFFS2类型分区*/

	
	EM_VLM_PARTITION_CRAMFS, /**< CRAMFS类型分区*/
	EM_VLM_PARTITION_ROMFS,	/**< ROMFS类型分区*/
	EM_VLM_PARTITION_UBIFS /**< UBIFS类型分区*/
}CSVLMPartitionType_E;

typedef int(*CSVLMNotify_F)(const CSVLMEvent_E eVlmEvent, const void * pEventData, const int nEventData, void *pUserData);


/**@brief 逻辑卷信息*/
typedef struct  
{
	char m_szPartitionName[CSVLM_MAX_PARTITION_NAME];/**< 逻辑卷所在的分区名*/
	char m_szMountPoint[CSVLM_MAX_NAME_LEN];/**< 逻辑卷挂载的路径名*/
	CSVLMPartitionType_E m_ePartitionType;      /**< 逻辑卷的分区类型*/
	BOOL m_bMounted;   /**< 逻辑卷是否已经记载到文件系统*/
	int m_nStorageId;     /**< 逻辑卷所在的存储设备标识*/
	int m_nDeviceId; /**< 逻辑卷的设备的ID 标识*/
}CSVLMInfo_S;

/**
@brief 逻辑卷模块初始化

@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSVLMInit(void);

/**
@brief 用户注册逻辑卷事件

@param[in] fnNotify 注册逻辑卷事件的回调函数
@param[in] pUserData 注册的用户私有数据

@return 成功返回TRUE,失败返回FALSE
@note fnNotify不能为空，fnNotify和pUserData唯一表示一个注册
*/
BOOL CSVLMRegNotify(const CSVLMNotify_F fnNotify, const void *pUserData);

/**
@brief 用户注销逻辑卷事件

@param[in] fnNotify 注册逻辑卷事件时传递的回调函数
@param[in] pUserData 注册时传入的用户私有数据

@return 成功返回TRUE,失败返回FALSE
@note fnNotify和pUserData唯一表示一个注册
*/
BOOL CSVLMUnregNotify(const CSVLMNotify_F fnNotify, const void *pUserData);

/**
@brief 获取系统中已挂载的逻辑卷数量

@return 失败返回CSVLM_INVALID_COUNT；否则返回已挂载逻辑卷的个数
*/
int CSVLMGetVolumeCount(void);

/**
@brief 根据索引获取逻辑卷信息

@param[in] nIndex 指定的逻辑卷索引
@param[out] psVolumeInfo 存储逻辑卷信息的地址

@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSVLMGetVolumeInfo(const int nIndex,CSVLMInfo_S * psVolumeInfo);

/**
@brief 逻辑卷的格式化

@param[in] szMountPoint 逻辑卷的挂载点
@param[out] eVlmFormat 用来格式化逻辑卷的格式

@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSVLMFormat(char * szMountPoint, CSVLMPartitionType_E eVlmFormat);

/**
@brief 卸载分区

@paramp[in] szMountPoint 逻辑卷的挂载点

@return 成功返回TRUE,失败返回FALSE
@note 卸载分区时，模块内会发送umount事件
*/
BOOL CSVLMUmount(char* szMountPoint);

#ifdef __cplusplus
}
#endif

#endif

