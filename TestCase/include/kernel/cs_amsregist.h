/**@def
@brief
*/
#ifndef _CS_AMS_H
#define _CS_AMS_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "cs_amsdatabase.h"
#include "cs_amscontroller.h"
#include "cs_amsapplication.h"
#include "cs_amsautodeploy.h"
#include "cs_amsassociateapp.h"



typedef struct _CSAMSFuns_S
{
	int m_nSize;                    ///< 本结构体的大小，必须赋值为sizeof(CSAMSFuns_t)

	BOOL (*m_fnAMSInit)(HCSHANDLE hAMSProp);
	BOOL (*m_fnSetProperty)(const char* pcKey,const char* pcValue);

	//database
	HCSHANDLE (*m_fnDBGetApp)(const char* pcAppid);
	int (*m_fnDBGetApps)(const char* pcFilter,HCSHANDLE* phOutAppHandleArray,int nArraySize);
	BOOL (*m_fnDBDeleteApp)(HCSHANDLE hAppHandle, BOOL bDeleteAppFile);
	HCSHANDLE (*m_fnDBAddApp)(const char* pcfilePath);
	BOOL (*m_fnDBAddListener)(CSAMSDBOnDbChanged_Fn fnCb,void* pvUserData);
	void (*m_fnDBRemoveListener)(CSAMSDBOnDbChanged_Fn fnCb);
	void (*m_fnDBInstallApp)(const char* pcAppUrl);
	void (*m_fnDBUpgradeApp)(const char* pcAppUrl, const char* pcAppId);
	HCSHANDLE (*m_fnDBGetMainApp)();
	
	//application
	int (*m_fnAPPGetAppProperty)(HCSHANDLE hAppHandle, const char* pcKey,char* pcOutValue, int nSize);
	CSAMSTATE_E (*m_fnAPPGetAppState)(HCSHANDLE hAppHandle);
	void (*m_fnAPPSetAppPropery)(HCSHANDLE hAppHandle,const char* pcKey,const char* pcValue);

	//controller
	BOOL (*m_fnCTRLStartApp)(const char* pcAppId,const char* pcParam);
	BOOL (*m_fnCTRLPauseApp)(const char* pcAppId);
	BOOL (*m_fnCTRLStopApp)(const char* pcAppId);
	BOOL (*m_fnCTRLStopApps)(const char** ppcAppIds,int nCount);
	BOOL (*m_fnCTRLResumeApp)(const char* pcAppId);
	HCSHANDLE (*m_fnCTRLGetActiveApp)();
	int (*m_fnCTRLGetRuningApps)(HCSHANDLE* phOutAppHandleArray,int nArraySize);
	BOOL (*m_fnCTRLStartRemoteApp)(const char* classPath,const char* mainClass,const char* entryMain,const char* pzAgus);
	BOOL (*m_fnCTRLAddListener)(CSAMSCTRLOnAction_Fn fnCb,void* pvUserData);
	void (*m_fnCTRLRemoveListener)(CSAMSCTRLOnAction_Fn fnCb);

	//autodeploy
	BOOL (*m_fnAutoDeployInit)();
	void (*m_fnAutoDeployStop)(BOOL bReset); 
	BOOL (*m_fnAutoDeployAddListener)(CSAMSAutoDeployOnAction_Fn fnCb, void* pvUserData);
	void (*m_fnAutoDeployRemoveListener)(CSAMSAutoDeployOnAction_Fn fnCb);
	BOOL (*m_fnAutoDeployStart)(); 
	BOOL (*m_fnAutoDeployStartDeploy)();
	BOOL (*m_fnAutoDeployCancelDeploy)();
	
	//associateapp
	BOOL (*m_fnAssociateAppInit)();
	BOOL (*m_fnAssociateAppAitManagerStart)(const char *pcLocator);
	void (*m_fnAssociateAppAitManagerStop)();
	BOOL (*m_fnAssociateAppAddListener)(CSAMSAssociateAppOnAction_Fn fnCb,void* pvUserData);
	void (*m_fnAssociateAppRemoveListener)(CSAMSAssociateAppOnAction_Fn fnCb);
}CSAMSFuns_S;



/**
@brief 获取C 应用管理器函数集
@return 函数集
*/
BOOL CSAMSRegistFuns(CSAMSFuns_S *pFuns);


#ifdef  __cplusplus
}
#endif

#endif

