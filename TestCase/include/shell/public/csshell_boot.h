#ifndef CSSHELL_BOOT_H
#define CSSHELL_BOOT_H

#include "mmcp_typedef.h"
#include "SearchUI.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
本结构定义了Boot插件对外结构体信息
*/
typedef struct _CSShellCommonBoot_S
{
    int                    m_nSize;                ///<本结构体的大小，必须赋值为sizeof(CommonBoot_S)
    void*                  m_pvUserData;           ///<用户数据，此参数将随着本结构中的函数指针一超传递给函数实现

    /*
        @brief 获取Boot插件主流程的状态
        @return TURE表示Boot插件主流程正处于运行状态，FALSE表示Boot插件主流程正处于非运行状态
        */
    int (* m_fnBootGetBootState)();
}CSShellCommonBoot_S;

/**
@brief 设置CSShellCommonBoot_S参数
@param psParam Boot Shell层要实现的接口
@成功返回TRUE，失败返回FALSE
*/
BOOL CSShellCommonBootSetParam(CSShellCommonBoot_S *psParam);

/**
@brief 获取CSShellCommonBoot_S参数
@param 无
@成功返回CSShellCommonBoot_S，失败返回NULL
*/
const CSShellCommonBoot_S* CSShellCommonBootGetParam();

#ifdef __cplusplus
}
#endif
/** @} */
#endif

