/**@def
@brief
*/
#ifndef _CS_AMSCCTRLADAPTER_H
#define _CS_AMSCCTRLADAPTER_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "cs_amsapplication.h"


/**
@brief 应用控制回调通知
@param[in] eAppState (EM_APP_STATE_ACTIVE、EM_APP_STATE_STOPED、EM_APP_STATE_PAUSED)
@param[in] pcAppId 应用标识
@param[in] pUserData 用户数据
*/
typedef void (*AMSCCTRLAppOnAction_Fn)(CSAMSTATE_E eAppState, const char *pcAppId, void *pUserData);




typedef struct _CSAMSCCTRLAppOpts_S
{
	int m_nSize;   ///< 本结构体的大小，必须赋值为sizeof(CSAMSCCTRLAppOpts_S)

	BOOL (*m_fnStartApp)(HCSHANDLE hApp, char *pcParam);
	BOOL (*m_fnStopApp)(HCSHANDLE hApp);
	BOOL (*m_fnPauseApp)(HCSHANDLE hApp);
	BOOL (*m_fnResumeApp)(HCSHANDLE hApp);
	BOOL (*m_fnAddListener)(AMSCCTRLAppOnAction_Fn fnCb, void *pvUserData);
	void (*m_fnRemoveListener)(AMSCCTRLAppOnAction_Fn fnCb);
}CSAMSCCTRLAppOpts_S;


/**
@brief 向amscctrladapter模块注册应用
@param[in] pcAppType 应用类型
@param[in] psAppOpts 应用操作集
@return TRUE成功，FALSE失败
*/
BOOL CSAMSCCTRLRegistAdapter(const char *pcAppType, CSAMSCCTRLAppOpts_S *psAppOpts);


#ifdef  __cplusplus
}
#endif

#endif

