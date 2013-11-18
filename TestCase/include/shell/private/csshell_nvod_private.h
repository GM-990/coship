#ifndef CSSHELL_NVOD_PRIVATE_H
#define CSSHELL_NVOD_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csshell_nvod.h"

/**
@brief 初始化Nvod
@param[in] 回调函数,在dtvmx native中实现
@return 成功则返回初始化过的Nvod句柄，失败则返回NULL
@note 如果已经初始化过epg，则直接返回上次初始化过的epg句柄
*/
HCSHANDLE CSShellNvodInit(NVODCallBack callback );

/**
@brief 获取初始化Nvod参数
@param 无
@return 之前设置的nvod参数
*/
const CSShellNvodInitParam_S* CSShellNvodGetParam();

/**
@brief 获取NVOD实现接口
@return 成功返回之前设置的NVOD实现接口
@see CSShellNvodGetDefaulFunc
*/
const CSShellNvodFunc_S* CSShellNvodGetFunc(void);

/**
@brief 销毁Nvod
@param 无
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSShellNvodDestroy();


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_SUBTITLE_H */

