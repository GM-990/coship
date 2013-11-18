/**@defgroup 应用管理器应用属性接口
@brief
*/
#ifndef _CS_AMSAPPLICATION_H
#define _CS_AMSAPPLICATION_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"



/**@brief 协议类型*/
typedef enum _CSAMSProtocolType_E
{
	EM_AMS_PROTOCOL_OC,   //数据广播(OC)
	EM_AMS_PROTOCOL_IP     //IP
}CSAMSProtocolType_e;


/**@brief 应用状态枚举定义*/
typedef enum _CSAMSTATE_E{
	EM_APP_STATE_ACTIVE = 0,	///<应用处于运行状态
	EM_APP_STATE_STOPED = 1,	///<应用处于停止状态
	EM_APP_STATE_PAUSED = 2,	///<应用处于暂停状态
	EM_APP_STATE_LOADING = 3,	///<应用启动过程中，正在加载的状态
	EM_APP_STATE_INVALID = 4	///<非法状态,获取状态错误或应用不存在
}CSAMSTATE_E;




//应用管理器系统属性
#define CSAMSPROP_APPS_ROOT_PATH    	"appsRootPath"	///<扩展应用存储根路径
#define CSAMSPROP_BASEAPP_PATH			"baseappPath"	///<内置应用(baseapp)存储根路径
#define CSAMSPROP_APPS_MEM_PATH    		"appsMemPath"	///<应用在内存中的路径


//应用属性
#define CSAPPPROP_LOCATOR               "locator"                 ///广播业务定位符,格式见GY/T xxx-xxxx附录J第J.2条"访问广播业务"
#define CSAPPPROP_ROOT_DIR  			"appRoot"		///<应用存储的根路径，该路径由应用管理器生成。
#define CSAPPPROP_ENTER_DIR  			"classPath"		///<应用文件的路径。如果是java应用，就是该应用的所有jar包,如app.jar；如果是HTML应用，就是该应用首页对应的文件夹，
#define CSAPPPROP_ENTER_FILE 			"bootPath"		///<应用启动的入口文件。java应用对应的是入口类（包名＋入口类名），如org.dtvmx.appmanager.AppEngineXlet；HTML应用对应的是首页的文件名，
#define CSAPPPROP_APP_TYPE  			"type"			///<应用的类型。有三种：JAVA_CDC, JAVA_CLDC, HTML
#define CSAPPPROP_APP_ID  				"appId"			///<应用的唯一标识符ID
#define CSAPPPROP_APP_NAME 				"name"			///<应用的名称
#define CSAPPPROP_APP_SIZE 				"size"			///<应用的大小。单位是byte
#define CSAPPPROP_APP_VERSION 			"version"		///<应用对外发布版本号。由应用提供者控制，是字符串，如：1.0.1
#define CSAPPPROP_AUTORUN_ORDER  		"autoRunOrder"	///<应用自启动顺序。如果不是自启动的，该值无效
#define CSAPPPROP_POWERON_RUNTYPE  		"powerOnRunType" ///<应用开机启动方式。0－自启动，1－手动启动
#define CSAPPPROP_UPGRADE_TYPE  		"upgradeType"	///<应用升级方式：0－自动升级，1－通知用户升级，2－用户确认升级
#define CSAPPPROP_DOWNLOAD_RUNTYPE		"downloadRunType"///<应用下载后的启动方式：0－自启动，1－通知用户启动，2－手动启动
#define CSAPPPROP_IS_VISIBLE  			"uiVisible"		///<应用是否可见。值为true或false（大小写均可）
#define CSAPPPROP_PUBLISH_TIME 			"publishTime"	///<应用发布时间
#define CSAPPPROP_CATEGORY 				"category"		///< 应用分类属性。应用管理器中应用分为两类：系统应用system，普通应用application
#define CSAPPPROP_RELATIONAL_PROTOCOL	"relationalProtocol"///<应用协议类型关联属性。
#define CSAPPPROP_REMOTE				"isRemote"		///<是否是远程应用
#define CSAPPPROP_PROVIDER				"provider"		///<应用提供者描述
#define CSAPPPROP_DESCRIPTION			"description"	///<应用描述
#define CSAPPPROP_TRANSPORT_TYPE		"TransportType"	///<若该值为OC-ServiceBound，则应用为节目关联应用
#define CSAPPPROP_TIMEZONE				"timeZone"		///<当前时区偏移>
#define CSAPPPROP_SOURCE_TYPE			"sourceType"	///<APP_SOURCETYPE应用来源类型：“ait”：节目关联型,“autodeploy”：自动部署型，“manualInstall”：手动型
#define CSAPPPROP_ORGID					"GID"			///<组织ID
#define CSAPPPROP_AID					"AID"			///<APPID
#define CSAPPPROP_VISIBILITY			"visibility"	///<是否可见
#define CSAPPPROP_PRIORITY				"priority"		///<标识应用存储的优先级，值越大优先级越高。若应用具有相同存储优先级，则应用存储行为由实现自行决定
#define CSAPPPROP_URLBASE				"fullUrl"		///<传输类型为HTTPTransportType时，URLBase指向某个zip包
#define CSAPPPROP_URLEXTENSION			"URLExtension"	///<应用下载扩展路径
#define CSAPPPROP_ORIGNETID				"OrigNetId"		///<传输类型为OCTransportType时,orgid
#define CSAPPPROP_TSID					"TSId"			///<传输类型为OCTransportType时,tsid
#define CSAPPPROP_SERVICEID				"ServiceId"		///<传输类型为OCTransportType时,serviceid
#define CSAPPPROP_COMPONENTTAG			"componentTag"	///<传输类型为OCTransportType时,componentTag
#define CSAPPPROP_APP_INCON_PATH        "appIconPath"    ///<应用图标路径

/**
@brief 获取应用属性
@param[in] hAppHandle 应用句柄
@param[in] pcKey 属性键值见CSAPPPROP_系列键值定义
@param[out] pcOutValue 属性取值
@param[in] nSize 传入取值字符串数组长度
@return 成功返回取值字符个数，失败返回-1
*/
int CSAMSAPPGetAppProperty(HCSHANDLE hAppHandle, const char* pcKey,char* pcOutValue, int nSize);

/**
@brief 获取应用状态
@param[in] hAppHandle 应用句柄
@return 应用状态
*/
CSAMSTATE_E CSAMSAPPGetAppState(HCSHANDLE hAppHandle);

/**
@brief 修改保存应用properties文件属性
@param[in] pcKey 属性对应的key
@param[in] pcValue 属性对应的值
*/
void CSAMSAPPSetAppPropery(HCSHANDLE hAppHandle,const char* pcKey,const char* pcValue);

#ifdef  __cplusplus
}
#endif

#endif

