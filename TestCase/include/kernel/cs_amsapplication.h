/**@defgroup Ӧ�ù�����Ӧ�����Խӿ�
@brief
*/
#ifndef _CS_AMSAPPLICATION_H
#define _CS_AMSAPPLICATION_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"



/**@brief Э������*/
typedef enum _CSAMSProtocolType_E
{
	EM_AMS_PROTOCOL_OC,   //���ݹ㲥(OC)
	EM_AMS_PROTOCOL_IP     //IP
}CSAMSProtocolType_e;


/**@brief Ӧ��״̬ö�ٶ���*/
typedef enum _CSAMSTATE_E{
	EM_APP_STATE_ACTIVE = 0,	///<Ӧ�ô�������״̬
	EM_APP_STATE_STOPED = 1,	///<Ӧ�ô���ֹͣ״̬
	EM_APP_STATE_PAUSED = 2,	///<Ӧ�ô�����ͣ״̬
	EM_APP_STATE_LOADING = 3,	///<Ӧ�����������У����ڼ��ص�״̬
	EM_APP_STATE_INVALID = 4	///<�Ƿ�״̬,��ȡ״̬�����Ӧ�ò�����
}CSAMSTATE_E;




//Ӧ�ù�����ϵͳ����
#define CSAMSPROP_APPS_ROOT_PATH    	"appsRootPath"	///<��չӦ�ô洢��·��
#define CSAMSPROP_BASEAPP_PATH			"baseappPath"	///<����Ӧ��(baseapp)�洢��·��
#define CSAMSPROP_APPS_MEM_PATH    		"appsMemPath"	///<Ӧ�����ڴ��е�·��


//Ӧ������
#define CSAPPPROP_LOCATOR               "locator"                 ///�㲥ҵ��λ��,��ʽ��GY/T xxx-xxxx��¼J��J.2��"���ʹ㲥ҵ��"
#define CSAPPPROP_ROOT_DIR  			"appRoot"		///<Ӧ�ô洢�ĸ�·������·����Ӧ�ù��������ɡ�
#define CSAPPPROP_ENTER_DIR  			"classPath"		///<Ӧ���ļ���·���������javaӦ�ã����Ǹ�Ӧ�õ�����jar��,��app.jar�������HTMLӦ�ã����Ǹ�Ӧ����ҳ��Ӧ���ļ��У�
#define CSAPPPROP_ENTER_FILE 			"bootPath"		///<Ӧ������������ļ���javaӦ�ö�Ӧ��������ࣨ�������������������org.dtvmx.appmanager.AppEngineXlet��HTMLӦ�ö�Ӧ������ҳ���ļ�����
#define CSAPPPROP_APP_TYPE  			"type"			///<Ӧ�õ����͡������֣�JAVA_CDC, JAVA_CLDC, HTML
#define CSAPPPROP_APP_ID  				"appId"			///<Ӧ�õ�Ψһ��ʶ��ID
#define CSAPPPROP_APP_NAME 				"name"			///<Ӧ�õ�����
#define CSAPPPROP_APP_SIZE 				"size"			///<Ӧ�õĴ�С����λ��byte
#define CSAPPPROP_APP_VERSION 			"version"		///<Ӧ�ö��ⷢ���汾�š���Ӧ���ṩ�߿��ƣ����ַ������磺1.0.1
#define CSAPPPROP_AUTORUN_ORDER  		"autoRunOrder"	///<Ӧ��������˳����������������ģ���ֵ��Ч
#define CSAPPPROP_POWERON_RUNTYPE  		"powerOnRunType" ///<Ӧ�ÿ���������ʽ��0����������1���ֶ�����
#define CSAPPPROP_UPGRADE_TYPE  		"upgradeType"	///<Ӧ��������ʽ��0���Զ�������1��֪ͨ�û�������2���û�ȷ������
#define CSAPPPROP_DOWNLOAD_RUNTYPE		"downloadRunType"///<Ӧ�����غ��������ʽ��0����������1��֪ͨ�û�������2���ֶ�����
#define CSAPPPROP_IS_VISIBLE  			"uiVisible"		///<Ӧ���Ƿ�ɼ���ֵΪtrue��false����Сд���ɣ�
#define CSAPPPROP_PUBLISH_TIME 			"publishTime"	///<Ӧ�÷���ʱ��
#define CSAPPPROP_CATEGORY 				"category"		///< Ӧ�÷������ԡ�Ӧ�ù�������Ӧ�÷�Ϊ���ࣺϵͳӦ��system����ͨӦ��application
#define CSAPPPROP_RELATIONAL_PROTOCOL	"relationalProtocol"///<Ӧ��Э�����͹������ԡ�
#define CSAPPPROP_REMOTE				"isRemote"		///<�Ƿ���Զ��Ӧ��
#define CSAPPPROP_PROVIDER				"provider"		///<Ӧ���ṩ������
#define CSAPPPROP_DESCRIPTION			"description"	///<Ӧ������
#define CSAPPPROP_TRANSPORT_TYPE		"TransportType"	///<����ֵΪOC-ServiceBound����Ӧ��Ϊ��Ŀ����Ӧ��
#define CSAPPPROP_TIMEZONE				"timeZone"		///<��ǰʱ��ƫ��>
#define CSAPPPROP_SOURCE_TYPE			"sourceType"	///<APP_SOURCETYPEӦ����Դ���ͣ���ait������Ŀ������,��autodeploy�����Զ������ͣ���manualInstall�����ֶ���
#define CSAPPPROP_ORGID					"GID"			///<��֯ID
#define CSAPPPROP_AID					"AID"			///<APPID
#define CSAPPPROP_VISIBILITY			"visibility"	///<�Ƿ�ɼ�
#define CSAPPPROP_PRIORITY				"priority"		///<��ʶӦ�ô洢�����ȼ���ֵԽ�����ȼ�Խ�ߡ���Ӧ�þ�����ͬ�洢���ȼ�����Ӧ�ô洢��Ϊ��ʵ�����о���
#define CSAPPPROP_URLBASE				"fullUrl"		///<��������ΪHTTPTransportTypeʱ��URLBaseָ��ĳ��zip��
#define CSAPPPROP_URLEXTENSION			"URLExtension"	///<Ӧ��������չ·��
#define CSAPPPROP_ORIGNETID				"OrigNetId"		///<��������ΪOCTransportTypeʱ,orgid
#define CSAPPPROP_TSID					"TSId"			///<��������ΪOCTransportTypeʱ,tsid
#define CSAPPPROP_SERVICEID				"ServiceId"		///<��������ΪOCTransportTypeʱ,serviceid
#define CSAPPPROP_COMPONENTTAG			"componentTag"	///<��������ΪOCTransportTypeʱ,componentTag
#define CSAPPPROP_APP_INCON_PATH        "appIconPath"    ///<Ӧ��ͼ��·��

/**
@brief ��ȡӦ������
@param[in] hAppHandle Ӧ�þ��
@param[in] pcKey ���Լ�ֵ��CSAPPPROP_ϵ�м�ֵ����
@param[out] pcOutValue ����ȡֵ
@param[in] nSize ����ȡֵ�ַ������鳤��
@return �ɹ�����ȡֵ�ַ�������ʧ�ܷ���-1
*/
int CSAMSAPPGetAppProperty(HCSHANDLE hAppHandle, const char* pcKey,char* pcOutValue, int nSize);

/**
@brief ��ȡӦ��״̬
@param[in] hAppHandle Ӧ�þ��
@return Ӧ��״̬
*/
CSAMSTATE_E CSAMSAPPGetAppState(HCSHANDLE hAppHandle);

/**
@brief �޸ı���Ӧ��properties�ļ�����
@param[in] pcKey ���Զ�Ӧ��key
@param[in] pcValue ���Զ�Ӧ��ֵ
*/
void CSAMSAPPSetAppPropery(HCSHANDLE hAppHandle,const char* pcKey,const char* pcValue);

#ifdef  __cplusplus
}
#endif

#endif

