/**@defgroup PublicShellTF3CA PublicShellTF3CA 定义同方3 CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_TF3CA_PRIVATE_H
#define CSSHELL_TF3CA_PRIVATE_H

#include "csshell_tf3ca.h"
#include "thirdparty/ca/tongfang3/CS_TFCA_Interface.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
	/**
	@brief 获取tf3 CA实现接口
	@return 成功返回之前设置的tf3ca实现接口
	@see CSShellTf3CaGetDefaultParam
	*/
	const CSShellTf3CaInitParam_S* CSShellTf3CaGetParam(void);


/**
@brief 获取同方3CA的相关配置信息
@return 成功返回配置信息，失败返回NULL
@note 当应用没有调用CSShellTf3CaSetConfig()设置配置信息时，返回NULL
@see CSShellTf3CaSetConfig
*/
const CSShellTf3CaConfig_S* CSShellTf3CaGetConfig(void);


/******************************************************************************
@breif 根据ca的类型和消息，设置智能卡状态
@param dwType--dwParam3,对应cs_cainterface_prvext.h中 F_CBMsg中的参数
@return 成功返回TRUE,否则返回FALSE
@note 在接收到ca发出的，智能卡错误，请插入智能卡，加密节目，节目未授权等要停止音视频输出的消息时，需要将该原因告诉给mediaplayer。如果mediaplayer不知道
节目停播的原因，就会误发送节目停播的消息
*******************************************************************************/
BOOL CSShellTf3CaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


