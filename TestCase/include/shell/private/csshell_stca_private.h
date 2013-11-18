/**@defgroup PublicShellSTCA PublicShellSTCA 定义算通 CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_STCA_PRIVATE_H
#define CSSHELL_STCA_PRIVATE_H

#include "csshell_stca.h"
#include "thirdparty/ca/suantong/CS_SUANTONGCA_Interface.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	@brief 获取St CA实现参数
	@return 成功返回之前设置的stca实现参数
	@see CSShellStCaGetDefaultParam
	*/
	const CSShellStCaInitParam_S* CSShellStCaGetParam(void);

/**
@brief 获取算通CA的相关配置信息
@return 成功返回配置信息，失败返回NULL
@note 当应用没有调用CSShellStCaSetConfig()设置配置信息时，返回NULL
@see CSShellStCaConfig_S
*/
const CSShellStCaConfig_S* CSShellStCaGetConfig(void);

/******************************************************************************
@breif 根据ca的类型和消息，设置智能卡状态
@param dwType--dwParam3,对应cs_cainterface_prvext.h中 F_CBMsg中的参数
@return 成功返回TRUE,否则返回FALSE
@note 在接收到ca发出的，智能卡错误，请插入智能卡，加密节目，节目未授权等要停止音视频输出的消息时，需要将该原因告诉给mediaplayer。如果mediaplayer不知道
节目停播的原因，就会误发送节目停播的消息
*******************************************************************************/
BOOL CSShellStCaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif

