#ifndef CS_MULTIPAGECTRL_H
#define CS_MULTIPAGECTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

typedef enum
{
    EM_MPC_MAIN_PAGE,
    EM_MPC_VOIP_PAGE,
    EM_MPC_MOTO_PAGE,
    EM_MPC_MSG_PAGE,
    EM_MPC_VOICEINPUT_PAGE,
    EM_MPC_PAGE_COUNT
} CSJSMPCPages_E;

typedef enum
{
    EM_MPC_INIT,
    EM_MPC_VISIBLE,
    EM_MPC_HIDE,
    EM_MPC_UNKNOWN
}MPCPageState_E;

/**
@brief 	初始化MultiPageCtrl模块

@return TRUE表示成功；FALSE表示失败
@note:
*/
BOOL CSJSMPCInit(void);

/**
@brief 	隐藏指定的page

@param[in] ePage:指定打开的page的类型
@return TRUE表示成功；FALSE表示失败
@note:
*/
BOOL CSJSMPCCreate(CSJSMPCPages_E ePage, const char* pcUrl);

/**
@brief 	隐藏指定的page

@param[in] ePage:指定打开的page的类型
@return TRUE表示成功；FALSE表示失败
@note:
*/
BOOL CSJSMPCDestroy(CSJSMPCPages_E ePage);

/**
@brief 	使指定的page显示出来，并且打开传入的url

@param[in] ePage:指定打开的page的类型
@param[in] pcUrl:要打开的目标页面，如果传空，表示打开指定page的首页
@param[in] nLength:pcUrl的长度
@return TRUE表示成功；FALSE表示失败
@note:
*/
BOOL CSJSMPCShow(CSJSMPCPages_E ePage, const char *pcUrl, int nLength);

/**
@brief 	隐藏指定的page

@param[in] ePage:指定打开的page的类型
@return TRUE表示成功；FALSE表示失败
@note:
*/
BOOL CSJSMPCHide(CSJSMPCPages_E ePage);

/**
@brief 	获取指定的page

@param[in] ePage:指定的page的类型
@return handle
@note:
*/
HCSHANDLE CSJSMPCGetPageHandle(CSJSMPCPages_E ePage);

/**
@brief 	获取当前显示的page

@return 当前page
@note:
*/
CSJSMPCPages_E CSJSMPCGetCurPage();

/**
@brief 	获取page的状态

@return page的状态
@note:
*/
MPCPageState_E CSJSMPCGetPageState(CSJSMPCPages_E ePage);

/**
@brief 	获取page的name

@param[in] ePage:指定打开的page的类型
@param[out] pcPageName:获取指定page的pageName
@param[in] nPageNameLen:传入的pageName字符串长度
@return TRUE表示成功；FALSE表示失败
@note:
*/
BOOL CSJSMPCGetPageName(CSJSMPCPages_E ePage, char *pcPageName, int nPageNameLen);

/**
@brief 	获取page的pageUrl

@param[in] ePage:指定打开的page的类型
@param[out] pcPageName:获取指定page的pageUrl
@param[in] nPageNameLen:传入的pageUrl字符串长度
@return TRUE表示成功；FALSE表示失败
@note:
*/
BOOL CSJSMPCGetPageUrl(CSJSMPCPages_E ePage, char *pcPageUrl, int nPageUrlLen);

/**
@brief 	设置page的pageUrl

@param[in] ePage:指定打开的page的类型
@param[out] pcPageName:获取指定page的pageUrl
@param[in] nPageNameLen:传入的pageUrl字符串长度
@return TRUE表示成功；FALSE表示失败
@note:
*/
BOOL CSJSMPCSetPageUrl(CSJSMPCPages_E ePage, char *pcPageUrl, int nPageUrlLen);

#ifdef __cplusplus
}
#endif

#endif 

