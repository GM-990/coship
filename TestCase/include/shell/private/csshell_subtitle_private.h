#ifndef CSSHELL_SUBTITLE_PRIVATE_H
#define CSSHELL_SUBTITLE_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_subtitle.h"

/**
@brief 初始化subtitle
@param[in] subtitle callback
@return 成功返回Subtitle句柄,失败返回false
*/
HCSHANDLE CSShellSubtitleInit(SubtitleCallBack callback);

/**
@brief 初始化subtitle,允许调用多次
@param[in] 
	subtitle callback
@param[in] 
	subtitle 类型
@return 成功返回Subtitle句柄,失败返回false
*/
HCSHANDLE CSShellSubtitleInitEx(SubtitleCallBack callback, CSUSPSBTLType_E emType);

/**
@brief  获取subtitle初始化参数
@return 成功返回psSubtitle 初始化参数结构体实例
@note 
*/
CSShellSubtitleInitParam_S* CSShellSubtitleGetParam();

#ifdef __cplusplus
}
#endif

#endif  /* CSSHELL_SUBTITLE_PRIVATE_H */

