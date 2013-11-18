/**@defgroup GUITOOL 
@brief 
@note Copyright (c) 2008 Coship
@version 1.0  2008-03-04  hjl             Create
@{
*/

#ifndef CSMULTIGTOOLS_H
#define CSMULTIGTOOLS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WEBKIT_EXPORTS
#define WEBKIT_API __declspec(dllexport)
#else
#define WEBKIT_API 
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include "mmcp_typedef.h"
#include "cs_gtools.h"
/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/
typedef struct _CSGTInitConfig
{
	char * pBufferAddr;			
	int nBufferSize;			// 初始化浏览器内存池大小		
	int nCacheBufferSize;		//  初始化浏览器缓存大小	
}CSGTInitConfig_S;

/**
@brief 浏览器初始化接口
return 0  		初始化失败
return > 0 	初始化成功
*/
HCSHANDLE CSGTMultiInit(CSGTInitConfig_S* pstInitCfg);

/**
@brief 设置浏览器可选参数
@param[in] HCSHANDLE hHandle = 0;
@param[in] CSGTOption_E enmOption  参见CSGTOption_E 说明
@param[in] void *pvValue
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE。
@note 无。
*/
CSGTRet_E  CSGTMultiSetOpt(HCSHANDLE hHandle,CSGTOption_E enmOption, void *pvValue);

/**
@brief 指定设置给浏览器的窗口
@param[in] HCSHANDLE hWid
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE。
@note 无。
*/
CSGTRet_E CSGTMultiSetWinId(HCSHANDLE hWid);

/**
@brief 应用向浏览器发送事件消息。
@param[in] HCSHANDLE hGtPageHandle hGtPageHandle = 0 ,发送消息给当前窗口 
								   hGtPageHandle > 0 ,发送消息给指定窗口
@param[in] sEvent 按键消息。
@return 成功时返回GT_RET_SUCCESS,失败返回GT_RET_FAILURE。
@note 参考例子:
@note 按键消息:
@note CSBRWEvent sEvent;
@note sEvent.m_dwPara0 = nKeyCode; // 键值
@note sEvent.m_dwPara1 = nEventType; // 按键类型
@note sEvent.m_dwPara2 = nEventAction; // 按键动作
@note sEvent.m_dwPara3 = 0;
@note sEvent.m_dwPara4 = 0;
@note CSGTMultiPostMsg(sEvent);

@note 功能消息:
@note CSBRWEvent sEvent;
@note sEvent.m_dwPara0 = BRW_SYSTEM_TYPE_GOBACK;
@note sEvent.m_dwPara1 = BRW_EVENT_TYPE_SYSTEM;
@note sEvent.m_dwPara2 = 0;
@note sEvent.m_dwPara3 = 0;
@note sEvent.m_dwPara4 = 0;
@note CSGTMultiPostMsg(sEvent);
*/
CSGTRet_E CSGTMultiPostMsg(HCSHANDLE hGtPageHandle, CSBRWEvent sEvent);

/**
@brief 指定创建 page
@return     HCSHANDLE 创建page的句柄

@note 功能消息:
@note CSBRWRect_S sRect;
@note sRect.nX = 0;
@note sRect.nY = 0;
@note sRect.nWidth = 1280;
@note sRect.nHeight = 720;
@note 设置的区域必须是平台支持可以创建的
*/
HCSHANDLE  CSGTMultiCreatePage(CSBRWRect_S sRect);

/**
@brief 同步创建 page
@return     HCSHANDLE 创建page的句柄

@note 在浏览器线程才可以调用此接口
@note CSBRWRect_S sRect;
@note sRect.nX = 0;
@note sRect.nY = 0;
@note sRect.nWidth = 1280;
@note sRect.nHeight = 720;
@note 设置的区域必须是平台支持可以创建的
*/
HCSHANDLE  CSGTMultiCreatePage_S(CSBRWRect_S sRect);

/**
@brief 打开一个网页 。
@param[in] HCSHANDLE hGtPageHandle hGtPageHandle == 0 在当前窗口打开网页 
								   hGtPageHandle >    0 (必须是CSGTMultiCreatePage创建page),在指定窗口打开网页
@param[in] pcUrl  可以访问的网页 pcUrl( ascall 码)。
@param[in] nLen pcUrl 的长度。
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE。
@note 无。
*/
CSGTRet_E CSGTMultiOpenURL(HCSHANDLE hGtPageHandle, const char *pcUrl,int nLen);	

/**
@brief 动态调整浏览器的窗口，主要在进入退出进入浏览器的时候需要适当的调整窗口
@param[in] HCSHANDLE hClient
@param[in] HCSHANDLE hWindow
@param[in] int nScreenWidth
@param[in] int nScreenHeight
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE。
*/
CSGTRet_E  CSGTMultiAdjustWin(HCSHANDLE hClient, HCSHANDLE hWindow, int nScreenWidth, int nScreenHeight); 
/**
@brief 获取当前浏览器所有的pageCount
*/
int CSGTMultiGetPageCount();

/**
@brief 获取当前浏览器激活的page
*/
HCSHANDLE   CSGTMultiGetCurActivePage();

/**
@brief 隐藏指定的page
*/
CSGTRet_E    CSGTMultiHidePage(HCSHANDLE hGtPageHandle);

/**
@brief 隐藏指定的page,并且停止接收消息
*/
CSGTRet_E    CSGTMultiStopPage(HCSHANDLE hGtPageHandle);

/**
@brief 激活指定的page
*/
CSGTRet_E   CSGTMultiActivePage(HCSHANDLE hGtPageHandle);	

/**
@brief 销毁指定的page
*/
CSGTRet_E   CSGTMultiDestroyPage(HCSHANDLE hGtPageHandle);		

/**
@brief 销毁浏览器
*/
CSGTRet_E   CSGTMultiDestroy();

#ifdef __cplusplus
}
#endif
#endif

