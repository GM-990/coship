
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

/**@brief �����ļ����������Value����*/
typedef enum
{
    EM_CSSHELL_CONFIG_DATATYPE_GROUP,  ///< ��ֵ��ʽ���ַ�����Ԥʾ�����潫��һ��ѭ��
    EM_CSSHELL_CONFIG_DATATYPE_INT,  ///< ������ֵ��ʽ���ַ���
	EM_CSSHELL_CONFIG_DATATYPE_FLOAT,  ///< ��������ֵ��ʽ���ַ���
    EM_CSSHELL_CONFIG_DATATYPE_STR,     ///< �ַ�����ʽ
    EM_CSSHELL_CONFIG_DATATYPE_BOOL,     ///< BOOL��ʽ�ַ���

    EM_CSSHELL_CONFIG_STRUCT_START,     ///< �ṹ�忪ʼ��ʶ
	EM_CSSHELL_CONFIG_STRUCT_END     ///< �ṹ�������ʶ
}CSShellConfigDataType_E;

/**@brief �����ļ�����������ʵ�ʵĳ�ʼ���ṹ��ӳ���ϵ�ṹ��*/
typedef struct _CSShellConfigData
{
	char m_acKeyName[KEYNAME_LEN];			//�����ļ��������������
	CSShellConfigDataType_E m_eDataType;		//�����ļ����������Value����
	DWORD m_dwDataAddr;					//ʵ�ʳ�ʼ�������ĵ�ַ
	int m_nDataLen;							//ʵ�ʳ�ʼ�������ĳ���

}CSShellConfigData_S;

/**
@brief��ȡ�����ļ�·��������Ŀʵ�֣�Shell����
@return �����ļ�·���ַ���
*/
char* CSShellGetConfigFilePath(void);

/**
@brief�������ļ��л�ȡ��ʼ������ֵ
@param[in] pcCfgFilePath �����ļ�·��
@param[in/out] psCfgDataArr �����ļ������������ʼ������ӳ������
@param[in] nArrSize ӳ�����ݴ�С
@return TRUE �ɹ���FALSE ʧ��
*/
BOOL CSShellGetConfigData(const char* pcCfgFilePath, int nCfgFilePathLen, CSShellConfigData_S* psCfgDataArr, int nArrSize);

#ifdef __cplusplus
}
#endif

#endif 
