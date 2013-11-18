/**@defgroup PublicShellBootShow PublicShellBootShow 定义BootShow的对外接口

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
功能：SaveBootScreen的回调通知函数
输入参数: nMsg 对应CSSaveBootScreenMessageType_E里面的定义
                            dwPara1 暂未使用 
                            dwPara2 暂未使用 
=============================================*/

typedef void (*FOnJSSaveBootScreenCallback)(DWORD dwUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);


typedef struct
{
    int   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellBootShowInitParam_S)   
    WORD (*m_fnSaveBootScrean)(const char *pcUrl);  ///< 返回值：1表示成功；0表示失败；-1表示pcUrl为空
    BOOL (*m_fnSaveBootTag)(int nFlag);   ///< 返回值：1表示成功；0表示失败
}CSShellBootShowInitParam_S;

/*=============================================
功能：设置BootShow
=============================================*/
BOOL CSShellBootShowSetParam(const CSShellBootShowInitParam_S* psBootShow);

/*=============================================
功能：获取BootShow的默认值
=============================================*/
const CSShellBootShowInitParam_S* CSShellBootShowGetDefaulParam(void);

BOOL CSShellSaveBootShowAddCallback(DWORD dwUserData, FOnJSSaveBootScreenCallback callback);
BOOL CSShellSaveBootShowSendMsg( int nMsg, DWORD dwPara1, DWORD dwPara2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


