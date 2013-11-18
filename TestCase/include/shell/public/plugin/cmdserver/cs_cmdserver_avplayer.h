#ifndef _CS_CMDSERVER_AVPLAYER_H_
#define _CS_CMDSERVER_AVPLAYER_H_

#include "cs_cmdserver.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief 得到AVPlayer(音视频)插件的实例

@param[in] 无
@return 成功返回对应插件实例的结构体，详见CSCmdInterface_S，失败返回NULL
@note 为方便扩展该模块其他功能插件，可按照如下示例代码实现\n
-static CSCmdInterface_S sPluginCmd;	\n
-sPluginCmd.m_szModuleName = "PluginName";	\n
-sPluginCmd.m_fnOnCmdServerAddCLI = CSPluginCmd_iProcess;	\n
-sPluginCmd.m_szModuleCmdFormat = "AVPlayer channelType  setType  setInfo  filename  playType/volInfo  videotop\n";	\n
-sPluginCmd.m_fnOnCmdServerAddInitPlugin = CSPluginCmd_iInitPlugin;	\n
-return &sAVPlayerCmd;	\n
-其中m_szModuleCmdFormat为提示信息，告知该插件的客户端发送的数据格式必须按照这种顺序发送	\n
-对于AVPlayer插件，客户端发送数据格式形如AVPlayer 0 1 0 \"file://name\" 0 0\n，注意每次发送数据必须以\n结束作为标识符\n
-其他可参见CSCmdInterface_S结构体说明\n
*/
CSCmdInterface_S* CSCMDServerGetAVPlayerInstance( void );

/**
@brief初始化AVPlayer(音视频)插件

@param[in] 无
@return 成功返回1，失败返回0
@note 该接口由项目调用，在调用CSCMDServerInit初始化cmd服务器后，调用该接口初始化AVPlayer插件\n
-若还需要支持其他插件功能，需要实现对应的插件，在项目中初始化该插件\n
-初始化插件的接口统一为CSPluginCMDServer_PluginNameInit，PluginName表示插件名\n
- 
*/
int CSPluginCMDServer_AVPlayerInit(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

