
#ifndef CSSHELL_SAUDATA_H
#define CSSHELL_SAUDATA_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@brief SAU初始化参数 */
typedef struct _CSShellSauInitParam
{
	int m_nSize;
	BOOL m_bStatus;
	int m_nFrequency;
	int m_nSymbolRate;			
	int m_nModulation;
}CSShellSauInitParam_S;


/**
@brief 获取SAU初始化参数
@param[in] psSauParam SAU初始化结构体指针
@param[in] pcFilePath 配置文件路径
@param[in] nFilePathLen 配置文件路径长度
@return 成功返回TRUE,失败返回FALSE
@notes 如果pcFilePath为NULL或nFilePathLen为0，则默认配置文件路径为ROOT_PATH"config/saudata.properties"
@notes 调用此接口时psSauParam参数的m_nSize需赋值为sizeof(CSShellSauInitParam_S)，否则可能获取不到数据
*/
BOOL CSShellGetSauCaData(CSShellSauInitParam_S* psSauParam, const char* pcFilePath, int nFilePathLen);

/**
@brief 获取SAU默认初始化参数，读取配置项时调用
@param[in] psSauParam SAU初始化结构体指针
@return 成功返回TRUE,失败返回FALSE
@notes 调用此接口时psSauParam参数的m_nSize需赋值为sizeof(CSShellSauInitParam_S)，否则可能获取不到数据
*/
BOOL CSShellGetDefaultSauData(CSShellSauInitParam_S* psSauParam);

#ifdef __cplusplus
}
#endif

#endif  
