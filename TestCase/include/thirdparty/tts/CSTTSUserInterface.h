#if !defined(_CSTTSUSERINTERFACE_H)
#define _CSTTSUSERINTERFACE_H

#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**@brief ��������ɫö�ٶ���*/
typedef enum
{
	EM_SPEAKER_UNKNOW,	///<δ֪
	EM_SPEAKER_MAN,		///<����
	EM_SPEAKER_WOMAN,	///<Ů��
	EM_SPEAKER_BOY,		///<�к���
	EM_SPEAKER_GIRL		///<Ů����
}CSTTSSpeakerTamber_E;

/**@brief ����������(����)ö�ٶ���*/
typedef enum
{
	EM_SPEAKER_LUNKNOW,	
	EM_SPEAKER_USENG,		///<��ʽӢ��
	EM_SPEAKER_BRIENG,	///<ӢʽӢ��
	EM_SPEAKER_CHI,		///<������ͨ��
	EM_SPEAKER_CHISZ,		///<�Ĵ���
	EM_SPEAKER_CHINE,		///������
	EM_SPEAKER_CHICA		///�㶫��
}CSTTSSpeakerLocalism_E;

#define MAX_SPEAKER_COUNT 30

/**@brief ��������Ϣ���ݽṹ����*/
typedef struct
{
	int nId;			///<������ID
	char pcName[32];			///<����������
	DWORD dwTamber;		///< ������ɫ
	DWORD dwLocalism;		///<��������		
}CSTTSSpeakerInfo_S;

/**@brief ���з�������Ϣ���ݽṹ����*/
typedef struct
{
	int nSpkCount;		///<����������
	CSTTSSpeakerInfo_S asSpkInfos[MAX_SPEAKER_COUNT];
}CSTTSAllSpeakerInfos_S;

/**@brief ��������ö�ٶ���*/
typedef enum
{
	EM_TTS_VOLUME_NORMAL,		///< ��������
	EM_TTS_VOLUME_MIN,			///< ��С����(����)
	EM_TTS_VOLUME_MAX			///< �������
}CSTTSVolumeLevel_E;

/**@brief ��������ö�ٶ���*/
typedef enum
{
	EM_TTS_SPEED_NORMAL,		///< ��������
	EM_TTS_SPEED_MIN,			///< ��������
	EM_TTS_SPEED_MAX		///< ��������
}CSTTSVoiceSpeedLevel_E;

/**@brief �ı�����ҳö�ٶ���*/
typedef enum
{
	EM_TTS_CODEPAGE_ASCII,		///< ASCII
	EM_TTS_CODEPAGE_GBK,			///< GBK (default)
	EM_TTS_CODEPAGE_BIG5,			///< Big5 
	EM_TTS_CODEPAGE_UTF16LE,		///< UTF-16 little-endian
	EM_TTS_CODEPAGE_UTF16BE,		///< UTF-16 big-endian
	EM_TTS_CODEPAGE_UTF8			///< UTF-8 
}CSTTSCodePageType_E;

/**@brief �ı�����ö�ٶ���*/
typedef enum
{
	EM_TTS_LANGUAGE_CHINESE,		///< Chinese
	EM_TTS_LANGUAGE_ENGLISH		///< English
}CSTTSLanguageType_E;

/**@brief ���ò���ѡ��ö�ٶ���*/
typedef enum
{
	EM_TTS_OPTION_SPEAKER,		///< ���÷�����
	EM_TTS_OPTION_VOLUME,		///< ��������
	EM_TTS_OPTION_SPEED,			///< ���÷�������
	EM_TTS_OPTION_CODEPAGE,		///< ���������ı�����ҳ
	EM_TTS_OPTION_LANGUAGE		///< ���������ı�����
}CSTTSSetParamOption_E;

/**
@brief �ͷ�����������Դ 

@param[in] hTTS ����������Դ���
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSTTSDestory(HCSHANDLE hTTS);

/**
@brief ��ȡ���з�������Ϣ

@param[in] hTTS ����������Դ���
@param[out] psSpeakerInfos ���з�������Ϣ
@return �ɹ�����TRUE, ʧ�ܷ���FALSE
*/
BOOL CSTTSGetAllSpeakerInfos(HCSHANDLE hTTS, CSTTSAllSpeakerInfos_S* psSpeakerInfos);

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
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSTTSSetParam(HCSHANDLE hTTS, CSTTSSetParamOption_E eOption, int nValue);

/**
@brief��ȡ����ֵ

@param[in] hTTS ����������Դ���
@param[in] eOption Ҫ��ȡ����ѡ��μ�CSTTSSetParamOption
@return Ҫ��ȡ������ȡֵ,ʧ�ܷ���-1
*/
int CSTTSGetParam(HCSHANDLE hTTS, CSTTSSetParamOption_E eOption);

/**
@brief ��ָ���ı��ϳɡ�����

@param[in] hTTS ����������Դ���
@param[in] pcText Ҫ�ϳɲ������ı�buffer 
@param[in] dwTextBufferSize Ҫ�ϳɲ������ı���Ҫ�Ļ�������С
@return �ɹ�����TRUE; ʧ�ܷ���FALSE
*/
BOOL CSTTSPlay(HCSHANDLE hTTS, const char* pcText, DWORD dwTextBufferSize);

/**
@brief ��ͣ�ı��ϳɡ�����

@param[in] hTTS ����������Դ���
@return �ɹ�����TRUE; ʧ�ܷ���FALSE
*/
BOOL CSTTSPause(HCSHANDLE hTTS);

/**
@brief �ָ��ı��ϳɡ�����

@param[in] hTTS ����������Դ���
@return �ɹ�����TRUE; ʧ�ܷ���FALSE
*/
BOOL CSTTSResume(HCSHANDLE hTTS);

/**
@brief ֹͣ�ı��ϳɡ�����

@param[in] hTTS ����������Դ���
@return �ɹ�����TRUE; ʧ�ܷ���FALSE
*/
BOOL CSTTSStop(HCSHANDLE hTTS);


#ifdef __cplusplus
}
#endif

#endif

