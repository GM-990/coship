#ifndef _CS_DLNA_MEDIA_RENDER_H_
#define _CS_DLNA_MEDIA_RENDER_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum 
{
    EM_MEDIARENDER_PLAY = 0,				///<播放指令       ，消息值(pPara):播放文件的URL,其长度可通过strlen((char *)pvPara)来计算出来
    EM_MEDIARENDER_PAUSE,					///<暂停指令       ，消息值:无         
    EM_MEDIARENDER_STOP,					///<停止指令       ，消息值:无         
    EM_MEDIARENDER_SEEK,					///<查找指令       ，消息值(pPara):开始播放的位置，单位为秒      
    EM_MEDIARENDER_NEXT,					///<下一个曲目指令       ，消息值(pPara):上一曲目URL
    EM_MEDIARENDER_PREVIOUS,				///<上一个曲目指令       ，消息值(pPara):下一曲目URL        
    EM_MEDIARENDER_SETMUTE,				///<静音指令       ，消息值:无         
    EM_MEDIARENDER_SETVOLUME,				///<音量指令       ，消息值(pPara):音量值 用户需要输出的音量值，按[0,31]阶音量计算；
    EM_MEDIARENDER_UNKNOWN = 1000			///<未知指令
}CSDLNAMediaRenderMsg_E;


/**
@brief    回调函数。
@param[in] hController  Controller句柄
@param[in] eMsg 消息类型
@param[in] pPara 消息值，各种不同的消息所带的消息值不同，详见CSDLNAMediaRenderMsg_E定义
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
@return 无。
*/
typedef int (*CSOnDLNAMediaRenderAction_F)(HCSHANDLE hRender, CSDLNAMediaRenderMsg_E eMsg, void* pPara, void* pUserData);

/**
@brief 创建一个Renderer
@param[in] pcRenderName Render的名称
@return 
- 成功返回Renderer句柄。
- 失败返回NULL。
*/
HCSHANDLE CSDLNAMediaRenderCreate(char * pcRenderName);

/**
@brief 销毁一个Render
@param[in] hRender Render的句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaRenderDestroy(HCSHANDLE hRender);

/**
@brief 启动Render
@param[in] hRender Render句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaRenderStart(HCSHANDLE hRender);

/**
@brief 停止Render
@param[in] hRender Render句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaRenderStop(HCSHANDLE hRender);

/**
@brief 停止Render
@param[in] hRender Render句柄
@param[in] pcName Render名称，字符串
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaRenderSetName(HCSHANDLE hRender, char* pcName);

/**
@brief	 注册通知回调函数。
@param[in] fnCb 回调处理函数
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaRenderAddCallback(CSOnDLNAMediaRenderAction_F fnCb, void* pUserData);

/**
@brief	删除回调函数。
@param[in] fnCb 回调处理函数
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaRenderRemoveCallback(CSOnDLNAMediaRenderAction_F fnCb, void* pUserData);


#ifdef __cplusplus
}
#endif

#endif

