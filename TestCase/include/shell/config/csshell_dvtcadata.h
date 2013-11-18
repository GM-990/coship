
#ifndef CSSHELL_DVTCADATA_H
#define CSSHELL_DVTCADATA_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@brief DVT CA初始化参数 */
typedef struct _CSShellDvtCaInitParam
{
	int m_nSize;
	int m_nFrequency;
	DWORD m_dwCaAddr;
	DWORD m_dwCaAddrBak;
}CSShellDvtCaInitData_S;


/**
@brief 获取DVT CA初始化参数
@param[in] psDvtCaParam DVT CA初始化结构体指针
@param[in] pcFilePath 配置文件路径
@param[in] nFilePathLen 配置文件路径长度
@return 成功返回TRUE,失败返回FALSE
@notes 如果pcFilePath为NULL或nFilePathLen为0，则默认配置文件路径为ROOT_PATH"config/dvtcadata.properties"
@notes 调用此接口时psDvtCaParam参数的m_nSize需赋值为sizeof(CSShellDvtCaInitData_S)，否则可能获取不到数据
*/
BOOL CSShellGetDvtCaData(CSShellDvtCaInitData_S* psDvtCaParam, const char* pcFilePath, int nFilePathLen);

/**
@brief 获取DVT CA默认初始化参数，读取配置项时调用
@param[in] psDvtCaParam DVT CA初始化结构体指针
@return 成功返回TRUE,失败返回FALSE
@notes 调用此接口时psDvtCaParam参数的m_nSize需赋值为sizeof(CSShellDvtCaInitParam_S)，否则可能获取不到数据
*/
BOOL CSShellGetDefaultDvtCaData(CSShellDvtCaInitData_S* psDvtCaParam);

#ifdef __cplusplus
}
#endif

#endif  
