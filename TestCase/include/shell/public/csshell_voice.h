#ifndef CSSHELL_VOICE_H
#define CSSHELL_VOICE_H
#include "mmcp_typedef.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**@brief ��ǰ���������ͷ��෽ʽ*/
typedef enum _CSShellVoiceType
{
	VOICETYPE_NONE,            //�������๦��
	VOICETYPE_NO_PCM,  //��pcm����
	VOICETYPE_PCM  //pcm����
}CSShellVoiceType_E;

/*
*@brief voice��ʼ������,������һϵ�к���ָ�룬��Щ����ָ����Ӧ�õ�voiceʵ��(C����)��Ȼ��ע���shell��ÿ������ָ���Ӧmediaplayer����������;�����һ���ӿ�
@see mediaplayer*/
typedef struct _CSShellVoiceInitParam_S
{
	int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellvoiceInitParam_S)
	/**
	@brief  ��������ֵ
	@param[in] eType : ��ʾ��ǰ���ڲ��Ž�Ŀ�����ͣ��ο�CSShellVoiceType_E����;
	@param[in] nVolume : ����ֵ �û���Ҫ���������ֵ����[0,31]����������;
	@return �ɹ���0 ʧ�ܣ�-1
	*/
	int (* m_fnSetVolume)(CSShellVoiceType_E eType, int nVolume);

	/**
	@brief  �õ���ǰ����ʹ�õ�����ֵ
	@param[out] pnVolume:������С,��[0,31]����������;
    @return �ɹ���0 ʧ�ܣ�-1
	*/
	int (* m_fnGetVolume)(int *pnVolume);

	/**
	@brief ���þ���
	@param[in] eType : ��ʾ��ǰ���ڲ��Ž�Ŀ�����ͣ��ο�CSShellVoiceType_E����;
	@param[in] bIsMute ������1  ��������0
	@return �ɹ���0 ʧ�ܣ�-1
	*/
	int (* m_fnSetMuteFlag)(CSShellVoiceType_E eType, BOOL bIsMute);

	/**
	@brief  �õ���ǰ����ʹ�õľ�����־
	@param[out]:pbIsMute ����0:  ������
    @return �ɹ���0 ʧ�ܣ�-1
	*/
	int (* m_fnGetMuteFlag)(BOOL *pbIsMute);

}CSShellVoiceInitParam_S;


/**
@brief ���������;�����صĽӿ�
@param psParam ���������;���shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellVoiceSetParam(CSShellVoiceInitParam_S * psParam);


/**
@brief ��ȡ���������;��������ӿ�
@param ��
@return psParam ���������;���shell��Ҫʵ�ֵĽӿ�
*/
const CSShellVoiceInitParam_S* CSShellVoiceGetParam();


#ifdef __cplusplus
}
#endif

#endif

