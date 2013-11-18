/**@defgroup FS 定义了统一的FS初始化相关接口操作函数
@brief 本模块主要定义了同洲项目专用平台无关FS驱动接口
@brief 基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
@version 1.0.0 2009/11/18 初稿
@{
*/

#ifndef _CS_FS_PLUS_H_
#define _CS_FS_PLUS_H_

#include  "udiplus_typedef.h"
#include "udi2_fs.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
@brief FS初始化

@return
- CSHDI_ERROR_ALREADY_INITIALIZED：FS之前已经被初始化过了。
- CSHDI_FAILURE：FS初始化失败。
- CSHDI_SUCCESS：FS初始化成功。
*/
CSUDI_Error_Code CSUDIFSInit(void);


/**
@brief 向FS中增加一个非热插拔分区,以使FS自动挂载该分区

@param[in] szPartition 分区名 
@param[in] eFsType  分区的类型
@return 
- CSHDI_FAILURE：向FS中增加此分区失败
- CSHDI_SUCCESS：向FS中增加此分区成功。
- CSHDI_ERROR_FEATURE_NOT_SUPPORTED : 不支持的文件系统类型
@note 该函数在CSUDIFSInit后调用，用于告知FS自动挂载指定的分区设备。
   通常FS对于该种设备无法检测到或者无法决定使用。
   例如向FS中加入jffs、cramfs等类型的设备。
*/
CSUDI_Error_Code CSUDIFSAddPartition(const char * szPartition, CSUDIFSPartitionType_E eFsType);

#ifdef __cplusplus
}
#endif

/** @} */

#endif



