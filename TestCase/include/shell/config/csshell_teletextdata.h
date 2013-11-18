
#ifndef CSSHELL_TELETEXTDATA_H
#define CSSHELL_TELETEXTDATA_H

#include "mmcp_typedef.h"
#include "csshell_teletext.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 获取teletext初始化参数
@param[out] psSubParam teletext初始化结构体指针
@param[in] pcFilePath 配置文件路径
@param[in] nFilePathLen 配置文件路径长度
@return 成功返回TRUE,失败返回FALSE
@notes 如果pcFilePath为NULL或nFilePathLen为0，则默认配置文件路径为ROOT_PATH"config/teletextdata.properties"
@notes 调用此接口时psSubParam参数的m_nSize需赋值为sizeof(CSShellTeletextInitParam_S)，否则可能获取不到数据
@notes 此接口仅可获取配置文件中已配置的参数，其它参数必须在具体项目中自行赋值
*/
BOOL CSShellGetTeletextData(CSShellTeletextInitParam_S* psSubParam, const char* pcFilePath, int nFilePathLen);

/**
@brief 获取teletext默认初始化参数，读取配置项时调用
@param[out] psSubParam teletext初始化结构体指针
@return 成功返回TRUE,失败返回FALSE
@notes 调用此接口时psSubParam参数的m_nSize需赋值为sizeof(CSShellTeletextInitParam_S)，否则可能获取不到数据
@notes 此接口仅可获取配置文件中已配置的参数，其它参数必须在具体项目中自行赋值
*/
BOOL CSShellGetDefaultTeletextData(CSShellTeletextInitParam_S* psSubParam);

#ifdef __cplusplus
}
#endif

#endif  
