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
@brief Ӧ�ÿ��ƻص�֪ͨ
@param[in] eAppState (EM_APP_STATE_ACTIVE��EM_APP_STATE_STOPED��EM_APP_STATE_PAUSED)
@param[in] pcAppId Ӧ�ñ�ʶ
@param[in] pUserData �û�����
*/
typedef void (*AMSCCTRLAppOnAction_Fn)(CSAMSTATE_E eAppState, const char *pcAppId, void *pUserData);




typedef struct _CSAMSCCTRLAppOpts_S
{
	int m_nSize;   ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSAMSCCTRLAppOpts_S)

	BOOL (*m_fnStartApp)(HCSHANDLE hApp, char *pcParam);
	BOOL (*m_fnStopApp)(HCSHANDLE hApp);
	BOOL (*m_fnPauseApp)(HCSHANDLE hApp);
	BOOL (*m_fnResumeApp)(HCSHANDLE hApp);
	BOOL (*m_fnAddListener)(AMSCCTRLAppOnAction_Fn fnCb, void *pvUserData);
	void (*m_fnRemoveListener)(AMSCCTRLAppOnAction_Fn fnCb);
}CSAMSCCTRLAppOpts_S;


/**
@brief ��amscctrladapterģ��ע��Ӧ��
@param[in] pcAppType Ӧ������
@param[in] psAppOpts Ӧ�ò�����
@return TRUE�ɹ���FALSEʧ��
*/
BOOL CSAMSCCTRLRegistAdapter(const char *pcAppType, CSAMSCCTRLAppOpts_S *psAppOpts);


#ifdef  __cplusplus
}
#endif

#endif

