
#ifndef CSSHELL_CONFIG_H
#define CSSHELL_CONFIG_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define KEYNAME_LEN 63

#define CONFIGFILE_PATH_LEN 256
#define PLUGIN_FILE_NAME "plugindata.properties"
#define PRINT_FILE_NAME "printinit.properties"
#define SEARCH_FILE_NAME "searchdata.properties"
#define EPG_FILE_NAME "epgdata.properties"
#define NVOD_FILE_NAME "nvoddata.properties"
#define ADS_FILE_NAME "Adsdata.properties"
#define DVTCA_FILE_NAME "dvtcadata.properties"
#define SAU_FILE_NAME "saudata.properties"
#define SUBTITLE_FILE_NAME "subtitledata.properties"
#define TELETEXT_FILE_NAME "teletextdata.properties"
#define SYSTEM_FILE_NAME "systemdata.properties"
#define ENV_FILE_NAME "env.properties"
#define RESOURCE_FILE_NAME "resource.properties"
#define ERROR_FILE_NAME "error.properties"
#define BROWSER_FILE_NAME "browserparam.properties"
#define NETWORK_FILE_NAME "NetworkConfig.properties"
#define DM_FILE_NAME "dmdata.properties"
#define SECTION_FILE_NAME "sectiondata.properties"
#define SYSTEMSETTING_FILE_NAME "Systemsetting.properties"
#define JVM_FILE_NAME "jvmdata.properties"
#define SN_FILE_NAME "sndata.properties"
#define MAC_FILE_NAME "macdata.properties"
#define VOIP_FILE_NAME "voipconfig.properties"
#define SAITION_FILE_NAME "saitionconfig.properties"
#define BROWSER_600_FILE_NAME "browser600param.properties"

/**@brief 配置文件中配置项的Value类型*/
typedef enum
{
    EM_CSSHELL_CONFIG_DATATYPE_GROUP,  ///< 数值形式的字符串，预示其下面将是一个循环
    EM_CSSHELL_CONFIG_DATATYPE_INT,  ///< 整型数值形式的字符串
	EM_CSSHELL_CONFIG_DATATYPE_FLOAT,  ///< 浮点型数值形式的字符串
    EM_CSSHELL_CONFIG_DATATYPE_STR,     ///< 字符串形式
    EM_CSSHELL_CONFIG_DATATYPE_BOOL,     ///< BOOL形式字符串

    EM_CSSHELL_CONFIG_STRUCT_START,     ///< 结构体开始标识
	EM_CSSHELL_CONFIG_STRUCT_END     ///< 结构体结束标识
}CSShellConfigDataType_E;

/**@brief 配置文件中配置项与实际的初始化结构体映射关系结构体*/
typedef struct _CSShellConfigData
{
	char m_acKeyName[KEYNAME_LEN];			//配置文件中配置项的名字
	CSShellConfigDataType_E m_eDataType;		//配置文件中配置项的Value类型
	DWORD m_dwDataAddr;					//实际初始化参数的地址
	int m_nDataLen;							//实际初始化参数的长度

}CSShellConfigData_S;

/**
@brief获取配置文件路径，由项目实现，Shell调用
@return 配置文件路径字符串
*/
char* CSShellGetConfigFilePath(void);

/**
@brief从配置文件中获取初始化参数值
@param[in] pcCfgFilePath 配置文件路径
@param[in/out] psCfgDataArr 配置文件中配置项与初始化参数映射数组
@param[in] nArrSize 映射数据大小
@return TRUE 成功，FALSE 失败
*/
BOOL CSShellGetConfigData(const char* pcCfgFilePath, int nCfgFilePathLen, CSShellConfigData_S* psCfgDataArr, int nArrSize);

#ifdef __cplusplus
}
#endif

#endif 
