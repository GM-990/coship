/**@defgroup PublicShellTTS PublicShellTTS ����TTS����ӿ�

@version 1.0.0 2011/9/7 ����
@{
*/
#ifndef CSSHELL_TTS_H
#define CSSHELL_TTS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "CSTTSUserInterface.h"
#include "CSTTSIFlyTek.h"

/**
@brief TTS��ʼ������
@see CSShellTTSSetParam CSShellTTSGetDefaultParam
*/
typedef struct _CSShellTTSInitParam
{
    int m_nSize;    ///<���ṹ��С�����븳ֵΪsizeof(CSShellTTSInitParam_S)

    /**
	@brief �ͷ�����������Դ 
	@param[in] hTTS ����������Դ���
	@return �ɹ�ʱӦ����TRUE;ʧ��ʱӦ����FALSE
	*/
	BOOL (*m_fnDestory)(HCSHANDLE hTTS);

	/**
	@brief ��ȡ���з�������Ϣ
	@param[in] hTTS ����������Դ���
	@param[out] psSpeakerInfos ���з�������Ϣ
	@return �ɹ�ʱӦ����TRUE;ʧ��ʱӦ����FALSE
	*/
	BOOL (*m_fnGetAllSpeakerInfos)(HCSHANDLE hTTS, CSTTSAllSpeakerInfos_S * psSpeakerInfos);

	/**
	@brief ���ò���
	@param[in] hTTS ����������Դ���
	@param[in] eOption Ҫ���ò���ѡ��μ�CSTTSSetParamOption
	@param[in] nValue Ҫ���ò���ֵ������ȡֵ����:
		eOptionȡEM_TTS_OPTION_SPEAKER: ����������
		eOptionȡEM_TTS_OPTION_VOLUME: ȡֵ�μ�CSTTSVolumeLevel
		eOptionȡEM_TTS_OPTION_SPEED: ȡֵ�μ�CSTTSVoiceSpeedLevel
		eOptionȡEM_TTS_OPTION_CODEPAGE: ȡֵ�μ�CSTTSCodePageType
		eOptionȡEM_TTS_OPTION_LANGUAGE: ȡֵ�μ�CSTTSLanguageType
	@return �ɹ�ʱӦ����TRUE,ʧ��ʱӦ����FALSE
	*/
	BOOL (*m_fnSetParam)(HCSHANDLE hTTS, CSTTSSetParamOption_E eOption, int nValue);

	/**
	@brief ��ȡ����ֵ
	@param[in] hTTS ����������Դ���
	@param[in] eOption Ҫ��ȡ����ѡ��μ�CSTTSSetParamOption
	@return Ҫ��ȡ������ȡֵ,ʧ��ʱӦ����-1
	*/
	int (*m_fnGetParam)(HCSHANDLE hTTS, CSTTSSetParamOption_E eOption);

	/**
	@brief ��ָ���ı��ϳɡ�����
	@param[in] hTTS ����������Դ���
	@param[in] pcText Ҫ�ϳɲ������ı�buffer 
	@param[in] dwTextBufferSize Ҫ�ϳɲ������ı���Ҫ�Ļ�������С
	@return �ɹ�ʱӦ����TRUE; ʧ��ʱӦ����FALSE
	*/
	BOOL (*m_fnPlay)(HCSHANDLE hTTS, const char * pcText, DWORD dwTextBufferSize);

	/**
	@brief ��ͣ�ı��ϳɡ�����
	@param[in] hTTS ����������Դ���
	@return �ɹ�ʱӦ����TRUE; ʧ��ʱӦ����FALSE
	*/
	BOOL (*m_fnPause)(HCSHANDLE hTTS);

	/**
	@brief �ָ��ı��ϳɡ�����
	@param[in] hTTS ����������Դ���
	@return �ɹ�ʱӦ����TRUE; ʧ��ʱӦ����FALSE
	*/
	BOOL (*m_fnResume)(HCSHANDLE hTTS);

	/**
	@brief ֹͣ�ı��ϳɡ�����
	@param[in] hTTS ����������Դ���
	@return �ɹ�ʱӦ����TRUE; ʧ��ʱӦ����FALSE
	*/
	BOOL (*m_fnStop)(HCSHANDLE hTTS);

	/**
	@brief ���ÿƴ�Ѷ����������������Դ����Ĭ��·��
	@param[in] pcDefResPath ��������������Դ·��
	@return �ɹ�ʱӦ����TRUE; ʧ��ʱӦ����FALSE
	*/
	BOOL (*m_fnSetDefaultResPath)(const char * pcDefResPath, DWORD dwResPathLen);

	/**
	@brief ����һ���ƴ�Ѷ������������Դ���ĵ�·��
	@param[in] pcResPath ��������������Դ·��
	@param[in] dwResPathLen pcResPath�ĳ���
	@return �ɹ�ʱӦ����TRUE; ʧ��ʱӦ����FALSE
	@remark	1)����һ���Ѵ��ڵ���Դ��·����ֱ�ӷ��سɹ���
			2)��ͨ��m_fnDeleteResPath��ɾ�����õ���Դ��·����
			3)�ýӿڲ�֧�ֶ��̲߳�����
	*/
	BOOL (*m_fnAddResPath)(const char * pcResPath, DWORD dwResPathLen);

	/**
	@brief ɾ���ƴ�Ѷ������������Դ���ĵ�·��
	@param[in] pcResPath ��������������Դ·��
	@param[in] dwResPathLen pcResPath�ĳ���
	@return �ɹ�ʱӦ����TRUE; ʧ��ʱӦ����FALSE
	@remark	1)���ӿ�ֻɾ����m_fnAddResPath���õ���Դ��·����
			2)������pcResPath����NULLʱ��ɾ����m_fnAddResPath���õ�ȫ����Դ·����
			3)ɾ��һ�������ڵ���Դ·��ʱ������ʧ�ܣ�
			4)�ýӿڲ�֧�ֶ��̲߳�����
	*/
	BOOL (*m_fnDeleteResPath)(const char * pcResPath, DWORD dwResPathLen);

	/**
	@brief �����ƴ�Ѷ������������Դ���
	@param[in] pResPath ��������������Դ·��
	@return �ɹ�ʱӦ������Դ���;ʧ��ʱӦ����NULL
	@remark 1)��pResPath��ΪNULL����ʵ��ʹ��pResPath + m_fnAddResPath���õ�·����
			2)��pResPathΪNULL����ʵ��ʹ��m_fnSetDefaultResPath���õ�·�� + m_fnAddResPath���õ�·����
			3)���������m_fnSetDefaultResPath����Ĭ��·����Ҳ������m_fnAddResPath���·������pcResPath���ܸ�ΪNULL��
	*/
 	HCSHANDLE (*m_fnCreateIFlyTek)(const char * pcResPath);

}CSShellTTSInitParam_S;

/**
@brief ����TTS��ز���
@param[in] psTTSParam TTS��ʼ������
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note ��Ŀʹ��Ĭ��ʵ�ּ�����������
@code
CSShellTTSSetParam(CSShellTTSGetDefaultParam());
@endcode
*/
BOOL CSShellTTSSetParam(const CSShellTTSInitParam_S * psTTSParam);

/**
@brief ��ȡ��ʼ������TTS��ز���
@param ��
@return �ɹ�����֮ǰ���õ�TTSʵ�ֽӿ�
@see CSShellTTSGetDefaultParam
*/
const CSShellTTSInitParam_S * CSShellTTSGetParam(void);
	
/**
@brief ��ȡTTSĬ��ʵ�ֽӿ�
@return �ɹ�ʱӦ����TTS Shell�ӿڵ�Ĭ��ʵ��
@note 
*/
const CSShellTTSInitParam_S * CSShellTTSGetDefaultParam(void);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

