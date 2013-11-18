#ifndef CSSHELL_TELETEXT_PRIVATE_H
#define CSSHELL_TELETEXT_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_teletext.h"

/**
@brief 初始化teletext
@param[in] teletext callback
@return 成功返回teletext句柄,失败返回NULL
*/
HCSHANDLE CSShellTeletextInit(TELECallback callback);

/**
@brief  获取teletext初始化参数
@return 成功返回psTeletext 初始化参数结构体实例
@note 
*/
CSShellTeletextInitParam_S* CSShellTeletextGetParam();

#ifdef __cplusplus
}
#endif

#endif  /* CSSHELL_TELETEXT_PRIVATE_H */

