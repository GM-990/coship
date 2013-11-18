#ifndef _CS_CMDSERVER_AVPLAYER_H_
#define _CS_CMDSERVER_AVPLAYER_H_

#include "cs_cmdserver.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief �õ�AVPlayer(����Ƶ)�����ʵ��

@param[in] ��
@return �ɹ����ض�Ӧ���ʵ���Ľṹ�壬���CSCmdInterface_S��ʧ�ܷ���NULL
@note Ϊ������չ��ģ���������ܲ�����ɰ�������ʾ������ʵ��\n
-static CSCmdInterface_S sPluginCmd;	\n
-sPluginCmd.m_szModuleName = "PluginName";	\n
-sPluginCmd.m_fnOnCmdServerAddCLI = CSPluginCmd_iProcess;	\n
-sPluginCmd.m_szModuleCmdFormat = "AVPlayer channelType  setType  setInfo  filename  playType/volInfo  videotop\n";	\n
-sPluginCmd.m_fnOnCmdServerAddInitPlugin = CSPluginCmd_iInitPlugin;	\n
-return &sAVPlayerCmd;	\n
-����m_szModuleCmdFormatΪ��ʾ��Ϣ����֪�ò���Ŀͻ��˷��͵����ݸ�ʽ���밴������˳����	\n
-����AVPlayer������ͻ��˷������ݸ�ʽ����AVPlayer 0 1 0 \"file://name\" 0 0\n��ע��ÿ�η������ݱ�����\n������Ϊ��ʶ��\n
-�����ɲμ�CSCmdInterface_S�ṹ��˵��\n
*/
CSCmdInterface_S* CSCMDServerGetAVPlayerInstance( void );

/**
@brief��ʼ��AVPlayer(����Ƶ)���

@param[in] ��
@return �ɹ�����1��ʧ�ܷ���0
@note �ýӿ�����Ŀ���ã��ڵ���CSCMDServerInit��ʼ��cmd�������󣬵��øýӿڳ�ʼ��AVPlayer���\n
-������Ҫ֧������������ܣ���Ҫʵ�ֶ�Ӧ�Ĳ��������Ŀ�г�ʼ���ò��\n
-��ʼ������Ľӿ�ͳһΪCSPluginCMDServer_PluginNameInit��PluginName��ʾ�����\n
- 
*/
int CSPluginCMDServer_AVPlayerInit(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

