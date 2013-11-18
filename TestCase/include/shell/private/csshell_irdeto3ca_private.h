/**@defgroup PublicShellIrdeto3CA PublicShellIrdeto3CA 定义Irdeto3 CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_IRDETO3CA_PRIVATE_H
#define CSSHELL_IRDETO3CA_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "csshell_irdeto3ca.h"
		
/**
@brief 获取irdeto3 CA实现接口
@return 成功返回之前设置的irdeto3实现接口
@see CSShellIrdeto3CaGetDefaultParam
*/
const CSShellIrdeto3CaInitParam_S* CSShellIrdeto3CaGetParam(void);

/**
@brief 获取Irdeto3 CA的相关配置信息
@return 成功返回配置信息，失败返回NULL
@note 当应用没有调用CSShellIrdeto3CaSetConfig()设置配置信息时，返回NULL
@see CSShellIrdeto3CaSetConfig
*/
const CSShellIrdeto3CaConfig_S* CSShellIrdeto3CaGetConfig(void);

/******************************************************************************
@breif 根据ca的类型和消息，设置智能卡状态
@param dwType--dwParam3,对应cs_cainterface_prvext.h中 F_CBMsg中的参数
@return 成功返回TRUE,否则返回FALSE
@note 在接收到ca发出的，智能卡错误，请插入智能卡，加密节目，节目未授权等要停止音视频输出的消息时，需要将该原因告诉给mediaplayer。如果mediaplayer不知道
节目停播的原因，就会误发送节目停播的消息
*******************************************************************************/
BOOL CSShellIrdeto3CaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


