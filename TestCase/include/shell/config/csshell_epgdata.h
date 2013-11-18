
#ifndef CSSHELL_EPGDATA_H
#define CSSHELL_EPGDATA_H

#include "mmcp_typedef.h"
#include "csshell_epg.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 获取EPG初始化参数
@param[in] psPFaram PF EPG初始化结构体指针
@param[in] psScheduleParam Schedule EPG初始化结构体指针
@param[in] pcFilePath 配置文件路径
@param[in] nFilePathLen 配置文件路径长度
@return 成功返回TRUE,失败返回FALSE
@notes 如果pcFilePath为NULL或nFilePathLen为0，则默认配置文件路径为ROOT_PATH"config/epgdata.properties"
@notes 调用此接口时CSShellEpgInitParam_S结构体参数的m_nSize要赋值为sizeof(CSShellEpgInitParam_S)，否则可能导致失败
*/
BOOL CSShellGetEpgData(CSShellEpgInitParam_S* psPFParam, CSShellEpgInitParam_S* psScheduleParam, const char* pcFilePath, int nFilePathLen);

/**
@brief 获取EPG默认初始化参数，读取配置项时调用
@param[in] psPFaram PF EPG初始化结构体指针
@param[in] psScheduleParam Schedule EPG初始化结构体指针
@return 成功返回TRUE,失败返回FALSE
@notes 调用此接口时CSShellEpgInitParam_S结构体参数的m_nSize要赋值为sizeof(CSShellEpgInitParam_S)，否则可能导致失败
*/
BOOL CSShellGetDefaultEpgData(CSShellEpgInitParam_S* psPFaram, CSShellEpgInitParam_S* psScheduleParam);

#ifdef __cplusplus
}
#endif

#endif  
