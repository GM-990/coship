
#ifndef CSSHELL_SUBTITLEDATA_H
#define CSSHELL_SUBTITLEDATA_H

#include "mmcp_typedef.h"
#include "csshell_subtitle.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 获取subtitle初始化参数
@param[in] psSubParam subtitle初始化结构体指针
@param[in] pcFilePath 配置文件路径
@param[in] nFilePathLen 配置文件路径长度
@return 成功返回TRUE,失败返回FALSE
@notes 如果pcFilePath为NULL或nFilePathLen为0，则默认配置文件路径为ROOT_PATH"config/subtidata.properties"
@notes 调用此接口时psSubParam参数的m_nSize需赋值为sizeof(CSShellSubtitleInitParam_S)，否则可能获取不到数据
*/
BOOL CSShellGetSubtitleData(CSShellSubtitleInitParam_S* psSubParam, const char* pcFilePath, int nFilePathLen);

/**
@brief 获取subtitle默认初始化参数，读取配置项时调用
@param[in] psSubParam subtitle初始化结构体指针
@return 成功返回TRUE,失败返回FALSE
@notes 调用此接口时psSubParam参数的m_nSize需赋值为sizeof(CSShellSubtitleInitParam_S)，否则可能获取不到数据
*/
BOOL CSShellGetDefaultSubtitleData(CSShellSubtitleInitParam_S* psSubParam);

#ifdef __cplusplus
}
#endif

#endif  
