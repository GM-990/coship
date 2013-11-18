
#ifndef CSSHELL_SEARCHDATA_H
#define CSSHELL_SEARCHDATA_H

#include "mmcp_typedef.h"
#include "csshell_search.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 获取Search初始化参数
@param[in] psSearchParam Search初始化结构体指针
@param[in] pcFilePath 配置文件路径
@param[in] nFilePathLen 配置文件路径长度
@return 成功返回TRUE,失败返回FALSE
@notes 如果pcFilePath为NULL或nFilePathLen为0，则默认配置文件路径为ROOT_PATH"config/searchdata.properties"
@notes 调用此接口时psSearchParam参数的m_nSize需赋值为sizeof(CSShellCommonSearch_S)，否则可能获取不到数据
*/
BOOL CSShellGetSearchData(CSShellCommonSearch_S* psSearchParam, const char* pcFilePath, int nFilePathLen);

/**
@brief 获取Search默认初始化参数，读取配置项时调用
@param[in] psSearchParam Search初始化结构体指针
@return 成功返回TRUE,失败返回FALSE
@notes 调用此接口时psSearchParam参数的m_nSize需赋值为sizeof(CSShellCommonSearch_S)，否则可能获取不到数据
*/
BOOL CSShellGetDefaultSearchData(CSShellCommonSearch_S* psSearchParam);

#ifdef __cplusplus
}
#endif

#endif  
