
#ifndef CSSHELL_EPG_PRIVATE_H
#define CSSHELL_EPG_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_epg.h"
#include "mmcp_typedef.h"
#include "CSEPGSortInterface.h"

/*********************设置和获取pf epg *******************************/
/**
@brief 获取设置的EPG参数
@param 无
@return EPG初始化参数
@note 该函数应该由dtvm native调用.
*/
const CSShellEpgInitParam_S* CSShellEpgGetPFParam();

/**
@brief 初始化epg
@param[in] callback epg回调，该回调在dtvm native中实现
@return 成功则返回之前初始化过的epg句柄，失败则返回NULL
@note 如果已经初始化过epg，则直接返回上次初始化过的epg句柄。
*/
HCSHANDLE CSShellEpgPFInit(EPGCallBack callback );

/*********************end 设置和获取pf epg *******************************/


/*********************设置和获取schedule epg *******************************/
/**
@brief 获取设置的EPG参数
@param 无
@return EPG初始化参数
@note 该函数应该由dtvm native调用.
*/
const CSShellEpgInitParam_S* CSShellEpgGetScheduleParam();

/**
@brief 初始化epg
@param[in] callback epg回调，该回调在dtvm native中实现
@return 成功则返回之前初始化过的epg句柄，失败则返回NULL
@note 如果已经初始化过epg，则直接返回上次初始化过的epg句柄。
*/
HCSHANDLE CSShellEpgScheduleInit(EPGCallBack callback ,EPGLevelGCallBack sortCallback);

/*********************end 设置和获取schedule epg *******************************/


/**
@brief 销毁schedule epg (pf的占用空间小，不用销毁)
@param 无
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSShellEpgScheduleDestroy();



#ifdef __cplusplus
}
#endif

#endif  /* SHELL_EITLOADER_H */




