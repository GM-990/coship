
#include  "mmcp_typedef.h"


#ifndef CS_ENVIRONMENT_H
#define CS_ENVIRONMENT_H

#ifdef __cplusplus
extern "C" {
#endif



/**@brief MMCP��������*/



/**@brief ���ܿ�/CA�ṩ������*/
#define CSENV_SMARTCARD_PROVIDER                    ("SmartCard.provider")

/**@brief ���ܿ�����*/
#define CSENV_SMARTCARD_SN                    ("SmartCard.serialNumber")

/**@brief ϵͳ��оƬ�ͺ�*/
#define CSENV_SOC_MODEL                                       ("SOC.model")

/**@brief ϵͳ��оƬ�ṩ��*/
#define CSENV_SOC_PROVIDER                                 ("SOC.provider")

/**@brief �ڴ��С����λMB*/
#define CSENV_RAM_SIZE                                          ("RAM.size")

/**@brief �ڴ湤��Ƶ�ʣ���λMHz*/
#define CSENV_RAM_FREQUENCY                               ("RAM.frequency")

/**@brief Flash��С����λMB*/
#define CSENV_FLASH_SIZE                                       ("Flash.size")



/**@brief �������ṩ��*/
#define CSENV_STB_PROVIDER                                  ("STB.provider")

/**@brief ����������
"DVB-C"  "OneWay" "DualWay"  "SD"  "HD"����ϡ�*/
#define CSENV_STB_TYPE                                          ("STB.type")

/**@brief ������Ʒ��*/
#define CSENV_STB_BRAND                                        ("STB.brand")

/**@brief �������ͺ�*/
#define CSENV_STB_MODEL                                        ("STB.model")

/**@brief ���������к�*/
#define CSENV_STB_SERIAL_NUMBER                         ("STB.serialNumber")

/**@brief ������MAC����*/
#define CSENV_STB_MAC_COUNT                                  ("STB.macCount")

/**@brief ������MAC��ַ��Ĭ��Ϊ��һ�ţ�����ͨ��STB.macAddress_0, STB.macAddress_1�ȷ�ʽȡ��N��������ַ*/
#define CSENV_STB_MAC_ADDRESS                                  ("STB.macAddress")

/**@brief ��Ŀ�б�汾*/
#define CSENV_STB_PROGRAMS_VERSION                  ("STB.programsVersion")

/**@brief ϵͳ·����ͨ��������������ļ���
�൱��windows��system32Ŀ¼����Ŀ¼��д��*/
#define CSENV_STB_SYSTEM_PATH                             ("STB.systemPath")

/**@brief ����·����ͨ���������ֻ��������
�ļ�����Щ�ļ������޸ġ���Щ�����ļ�
�������������豸�У���STB.systemPath�µ�����
�ļ��������ǣ�ʹ�ø�Ŀ¼�µ�ͬ���·����
ͬ���ļ���Ϊ�����ļ���*/
#define CSENV_STB_INNER_PATH                               ("STB.innerPath")

/**@brief �������豸���ͣ���ʱ�����ַ���""*/
#define CSENV_STB_DEVICE_TYPE                              ("STB.deviceType")

/**@brief �����зֱ��ʣ�"SD"-���壻"HD"-����*/
#define CSENV_STB_DEFINITION                                ("STB.definition")



/**@brief Loader����*/
#define CSENV_LOADER_NAME                                    ("Loader.name")

/**@brief Loader�ṩ��*/
#define CSENV_LOADER_PROVIDER                             ("Loader.provider")

/**@brief Loader�汾*/
#define CSENV_LOADER_VERSION                               ("Loader.version")

/**@brief Loaderϵͳ��С����λ��KB*/
#define CSENV_LOADER_SIZE                                      ("Loader.size")



/**@brief �м����Ȩ������*/
#define CSENV_MW_COPYRIGHT                                 ("Middleware.copyright")

/**@brief �м������*/
#define CSENV_MW_NAME                                           ("Middleware.name")

/**@brief �м���汾*/
#define CSENV_MW_VERSION                                      ("Middleware.version")

/**@brief �м����������*/
#define CSENV_MW_RELEASE_DATE                            ("Middleware.releaseDate")

/**@brief �Ƿ�ǿ�������м�����õ�BaseApp��
"true"or"false"��δ����Ϊ"false"*/
#define CSENV_MW_START_BASEAPP_FORCIBLY         ("Middleware.startBaseAppForcibly")

/**@brief Ӧ�ù���ϵͳ��ʹ�õĻ�����
���ͱ�ţ��ǳ���Ҫ����������NSP����:5101��
��LOADER���ø���:5102��Զ�̸���5191����
�ñ������Ӫ�̷��䣬ͨ����������ͺŰ�
����ͨ��CAģ�����õĳ��̺š�Ӳ���š�ģ��ţ�*/
#define CSENV_MW_STB_TYPE_CODE                          ("Middleware.stbTypeCode")

/**@brief Ӧ�ù�����ʹ��UDP������
ǰ��Ӧ�ò���ϵͳ�е�Ӧ�õİ汾��ʱ��
ʧ�ܺ����Ե�ʱ������*/
#define CSENV_MW_CHECK_APPSVERSION_INTERVAL ("Middleware.checkAppsVersionInterval")

/**@brief �м����ԵĿͻ���*/
#define CSENV_MW_CUSTOMER                                   ("Middleware.customer")



/**@brief ����汾��*/
#define CSENV_SW_VERSION                                       ("Software.version")

/**@brief Ӳ����Ϣ*/
#define CSENV_HW_INFO                                             ("Hardware.info")

/**@brief Ӳ���汾��*/
#define CSENV_HW_VERSION                                       ("Hardware.version")


#define CSENV_HW_VIDEO_TYPE                                 ("Hardware.videoType")

/**@brief ���ݸ�ʱ����ҳ��ʱ��Ƶ�����ݣ�ȡ����*/
#define CSENV_TIMESHIFT_CHANNEL_INFO                  ("TimeshiftChannelInfo")


/**@brief ��������ģʽ����ͳһ("UNIQUE")���Ƿ�ɢ("MULTIPLE")*/
#define CSENV_SYS_VOLUME_MODE                             ("SYS.VolumeMode")

/**@brief ��������ģʽ����ͳһ("UNIQUE")���Ƿ�ɢ("MULTIPLE")*/
#define CSENV_SYS_AUDIO_TRACK_MODE                    ("SYS.AudioTrackMode")
	
/**�Ƿ�֧�ְ�ȫУ��,"true" ��"false", ���û�����ã�Ĭ��Ϊ"false"��֧�ְ�ȫУ��*/
#define CSENV_MW_SECURITY_SUPPORT   ( "Middleware.securitySupport")

/**������Ƶ����Ϣ���磺deliver://522000000.6875.64?type=qam */
#define CSENV_MW_DEPLOY_DELIVER    ("Middleware.AutodeployDELIVER")

/**Ӧ�ò���ʽ��"oc" �� "ip"  �� "auto",���ִ�Сд�����û�����ã�Ĭ��Ϊ"ip"˫����*/
#define CSENV_MW_DEPLOY_TYPE    ("Middleware.stbDeployType")

/**������������*/
#define CSENV_MW_AREA_CODE  ("Middleware.AreaCode")

/**����������Ⱥ����Ϣ*/
#define CSENV_MW_GROUP_ID  ("Middleware.GroupID")	

/**˫��Ӧ�ò���ʱ�Ƿ�ʹ��http�����ļ�ϵͳֱ�ӽ�ѹǰ��Ӧ��zip��*/
#define CSENV_DEPLOY_USE_HTTPFS ("Middleware.deployUseHttpFS")

/**�������Ƿ�ɻָ�,Ĭ�ϲ��ɻָ�*/
#define CSENV_DEPLOY_OC_REVERABLE ("Middleware.isOCReverable")


/**
@brief  ����������ʼ��

@param[in] ��
@return   �ɹ�����TRUE;���򷵻�FALSE
*/

BOOL CSENVInit(void);

/**
���������仯֪ͨ
@param szKey - ��������.
@param szValue - �仯���ֵ
@param pUserData �û��Զ������(ע��ص�����ʱ����Ĳ���)
*/
typedef void (*CSOnEnvChanged_F)(const char* szKey,const char* szValue,void* pUserData);

/**
ע�ỷ�������仯������
@param szKey - ��Ҫ�����Ļ���������(NULL��ʾ�������л��������仯)
@param fnCb - �����߻ص�����
@param pUserData �û��Զ������(�ص�֪ͨʱ�ش����ص�����)
*/
BOOL CSENVAddListner(const char* szKey,CSOnEnvChanged_F fnCb,void* pUserData);

/**
�Ƴ�������
@param fnCb - �����߻ص�����
*/
void CSENVRemoveListner(CSOnEnvChanged_F fnCb);

/**
�Ƴ�������
@param szKey - �����߼����Ļ������� [������Ϊ��]
@param fnCb - ���������ڴ���ָ�����������Ļص�����[������Ϊ��]
*/
void CSENVRemoveKeyListner(const char* szKey, CSOnEnvChanged_F fnCb);

/*!
���û�������
@param szKey - ��������.������ΪNULL,���򷵻�ΪNULL
@param szValue - Ҫ���õ�ֵ
@return : ���óɹ�����TRUE;���򷵻�FALSE
*/
BOOL CSENVSetValue(const char *szKey,const char *szValue);

/*!
@param szKey - Ҫ���ȡ�Ļ�������,������ΪNULL,���򷵻�NULL
@return : ����������ֵ.�����򷵻ػ���������ֵ�����򷵻�NULL
*/
const char * CSENVGetValue(const char * szKey);

/**
@brief ���ٻ�������ģ��

@param[in] ��
@return �ɹ�����TRUE;���򷵻�FALSE
*/

BOOL CSENVClearUp(void);


/**
@brief ��ӡ������������Ϣ��������ʹ�ã����"env"�Ĵ�ӡ

@param[in] ��
*/
void CSENVPrintAllNode(void);

#ifdef __cplusplus
}
#endif

#endif


