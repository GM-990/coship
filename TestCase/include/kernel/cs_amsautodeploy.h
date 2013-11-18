

#ifndef _AMS_AUTODEPLOY_H_
#define _AMS_AUTODEPLOY_H_
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif





/**@brief 应用部署事件*/
typedef enum _CSAMSAutoDeployEvent_E
{
	//请求应用清单
	EM_AMS_DEPLOY_REQUEST_APPLIST_START=1,  //json 字符串为空
	EM_AMS_DEPLOY_REQUEST_APPLIST_OK=2,        //json字符串格式{"UpdateCount":123,"deleteCount":123,"dowloadCount":123};
	EM_AMS_DEPLOY_REQUEST_APPLIST_FAILED=3,  //json字符串格式{"resion":"network failed"};

	//更新应用
	EM_AMS_DEPLOY_UPDATE_START=4,     //json字符串格式{"appName":"portal","version":"1.5.1","UpdatedCount":123};
	EM_AMS_DEPLOY_UPDATE_OK=5,           //json字符串格式{"appName":"portal","version":"1.5.1","UpdatedCount":123};
	EM_AMS_DEPLOY_UPDATE_FAILED=6,    //json字符串格式{"appName":"portal","resion":"network failed"};

	//下载应用
	EM_AMS_DEPLOY_DOWNLOAD_START=7, //json字符串格式{"appName":"portal","version":"1.5.1","dowloadedCount":123};
	EM_AMS_DEPLOY_DOWNLOAD_OK=8,      //json字符串格式{"appName":"portal","version":"1.5.1","dowloadedCount":123};
	EM_AMS_DEPLOY_DOWNLOAD_FAILED=9,//json字符串格式{"appName":"portal","resion":"network failed"};

	//删除应用
	EM_AMS_DEPLOY_DELETE_START=10,   //json字符串格式{"appName":"portal","version":"1.5.1","deletedCount":123};
	EM_AMS_DEPLOY_DELETE_OK=11,         //json字符串格式{"appName":"portal","version":"1.5.1","deletedCount":123};
	EM_AMS_DEPLOY_DELETE_FAILED=12,   //json字符串格式{"appName":"portal","resion":"network failed"};

	//部署完毕
	EM_AMS_DEPLOY_FINISH=13              //json字符格式{"UpdatedCount":123,"deletedCount":123,"dowloadedCount":123};
}CSAMSAutoDeployEvent_e;


/**
@brief 应用部署事件回调通知
@param[in] eEvent  事件类型
@param[in] pcJson   json 字符串
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
*/
typedef void (*CSAMSAutoDeployOnAction_Fn) (CSAMSAutoDeployEvent_e eEvent, const char* pcJson, DWORD dwLen, void* pvUserData);



/**
@brief 启动部署
@return 启动成功返回TRUE，否则返回FALSE
*/
BOOL CSAMSAutoDeployInit();

/** 
@brief 停止部署 
@param[in] bReset 是否停止部署并reset部署状态，TRUE--停止部署并清空部署相关的信息;FALSE--停止部署但不清空部署相关的信息 
*/ 
void CSAMSAutoDeployStop(BOOL bReset); 


/**
@brief 注册监听回调
@param[in] fnCb 回调函数
@param[in] pUserData 用户自定义参数，在回调函数中将传给用户
@return 成功返回TRUE，否则返回FALSE
*/
BOOL CSAMSAutoDeployAddListener(CSAMSAutoDeployOnAction_Fn fnCb, void* pvUserData);

/**
@brief 移除监听回调
@param[in] fnCb 回调函数
*/
void CSAMSAutoDeployRemoveListener(CSAMSAutoDeployOnAction_Fn fnCb);

BOOL CSAMSAutoDeployStart(); 

BOOL CSAMSAutoDeployStartDeploy();

BOOL CSAMSAutoDeployCancelDeploy();


#ifdef __cplusplus
}
#endif

#endif


