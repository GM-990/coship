
#ifndef CSSHELL_NVODDATA_H
#define CSSHELL_NVODDATA_H

#include "mmcp_typedef.h"
#include "csshell_nvod.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 获取NVOD初始化参数
@param[in] psNvodParam NVOD初始化结构体指针
@param[in] pcFilePath 配置文件路径
@param[in] nFilePathLen 配置文件路径长度
@return 成功返回TRUE,失败返回FALSE
@notes 如果pcFilePath为NULL或nFilePathLen为0，则默认配置文件路径为ROOT_PATH"config/nvoddata.properties"
*/
BOOL CSShellGetNvodData(CSShellNvodInitParam_S* psNvodParam, const char* pcFilePath, int nFilePathLen);

/**
@brief 获取NVOD默认初始化参数，读取配置项时调用
@param[in] psNvodParam NVOD初始化结构体指针
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSShellGetDefaultNvodData(CSShellNvodInitParam_S* psNvodParam);

#ifdef __cplusplus
}
#endif

#endif  
