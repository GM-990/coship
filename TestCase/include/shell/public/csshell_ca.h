/**@defgroup PublicShellCA PublicShellCA 定义ShellCA公共对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_CA_H
#define CSSHELL_CA_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@brief CA类型.目前已定义mmcp经常用到的ca。对于一些过时ca，比如同方2未做定义。dtvm和shell也未对过时的ca做默认支持*/
typedef enum
{
    EM_CSSHELL_CA_TYPE_FTA = 0,
    EM_CSSHELL_CA_TYPE_NDS,
    EM_CSSHELL_CA_TYPE_TF3,
    EM_CSSHELL_CA_TYPE_DVT,
    EM_CSSHELL_CA_TYPE_SUANTONG,
    EM_CSSHELL_CA_TYPE_IRDETO3,
    EM_CSSHELL_CA_TYPE_NAGRA,
    EM_CSSHELL_CA_TYPE_TOP,
    EM_CSSHELL_CA_TYPE_IRDETO2,
    EM_CSSHELL_CA_TYPE_FINESUM,
    EM_CSSEHLL_CA_TYPE_UDRM,
    EM_CSSHELL_CA_TYPE_IRDETOIUC,
    EM_CSSHELL_CA_TYPE_CONAX,
    EM_CSSHELL_CA_TYPE_DVN,
    EM_CSSHELL_CA_TYPE_COMVENIENT,
    EM_CSSHELL_CA_TYPE_SUANTONGCTI,
    EM_CSSHELL_CA_TYPE_TR
}CSShellCaType_E;


/**
@brief 获取智能卡号接口定义
本接口同步获取智能卡号，若无卡或者无法识别则返回失败。
@param[out] pcCardId 存储获取到的智能卡卡号
@param[in/out] pnLen 传入参数时表示pcCardId指向的buffer长度，做传出参数是是 pcCardId中卡号的实际长度
@return 成功返回0，失败-1
@note 同步接口，若ca底层(即各具体ca)无法提供同步接口需实现自行缓存，以保证该接口的同步
*/
typedef int (*CSShellCaGetSmartCardID_F)(char* pcCardId, int *pnLen);

/**
@brief 设置是否允许dtvm ca native关闭或打开音视频
@param[in] bCanControl TRUE,允许控制音视频;否则不允许
@return 无
@note 该接口是为了兼容天威项目之前在dtvm ca native中控制音视频的特性而添加；默认为 FALSE
@note 仅建议天威项目使用，设为TRUE;其他项目建议不要调用该接口
@note 目前该设置仅对nds ca有效
*/
void CSShellCaSetControlAV(BOOL bCanControl);


#ifdef __cplusplus
}
#endif
/** @} */
#endif   



