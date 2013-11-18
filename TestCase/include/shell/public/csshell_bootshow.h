/**@defgroup PublicShellBootShow PublicShellBootShow ����BootShow�Ķ���ӿ�

@{
*/
#ifndef CSSHELL_BOOTSHOW_H
#define CSSHELL_BOOTSHOW_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"

typedef enum CSShellSaveBootScreenMessageType_E
{
	EM_SAVEBOOTSCREEM_SUCCESS = 10602,
	EM_SAVEBOOTSCREEM_FAILURE	
}CSShellBootScreenMessageType_E;

/*=============================================
���ܣ�SaveBootScreen�Ļص�֪ͨ����
�������: nMsg ��ӦCSSaveBootScreenMessageType_E����Ķ���
                            dwPara1 ��δʹ�� 
                            dwPara2 ��δʹ�� 
=============================================*/

typedef void (*FOnJSSaveBootScreenCallback)(DWORD dwUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);


typedef struct
{
    int   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellBootShowInitParam_S)   
    WORD (*m_fnSaveBootScrean)(const char *pcUrl);  ///< ����ֵ��1��ʾ�ɹ���0��ʾʧ�ܣ�-1��ʾpcUrlΪ��
    BOOL (*m_fnSaveBootTag)(int nFlag);   ///< ����ֵ��1��ʾ�ɹ���0��ʾʧ��
}CSShellBootShowInitParam_S;

/*=============================================
���ܣ�����BootShow
=============================================*/
BOOL CSShellBootShowSetParam(const CSShellBootShowInitParam_S* psBootShow);

/*=============================================
���ܣ���ȡBootShow��Ĭ��ֵ
=============================================*/
const CSShellBootShowInitParam_S* CSShellBootShowGetDefaulParam(void);

BOOL CSShellSaveBootShowAddCallback(DWORD dwUserData, FOnJSSaveBootScreenCallback callback);
BOOL CSShellSaveBootShowSendMsg( int nMsg, DWORD dwPara1, DWORD dwPara2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


