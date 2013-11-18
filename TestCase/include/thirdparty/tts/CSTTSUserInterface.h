#if !defined(_CSTTSUSERINTERFACE_H)
#define _CSTTSUSERINTERFACE_H

#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**@brief 发音人音色枚举定义*/
typedef enum
{
	EM_SPEAKER_UNKNOW,	///<未知
	EM_SPEAKER_MAN,		///<男音
	EM_SPEAKER_WOMAN,	///<女音
	EM_SPEAKER_BOY,		///<男孩音
	EM_SPEAKER_GIRL		///<女孩音
}CSTTSSpeakerTamber_E;

/**@brief 发音人语种(方言)枚举定义*/
typedef enum
{
	EM_SPEAKER_LUNKNOW,	
	EM_SPEAKER_USENG,		///<美式英语
	EM_SPEAKER_BRIENG,	///<英式英语
	EM_SPEAKER_CHI,		///<汉语普通话
	EM_SPEAKER_CHISZ,		///<四川话
	EM_SPEAKER_CHINE,		///东北话
	EM_SPEAKER_CHICA		///广东话
}CSTTSSpeakerLocalism_E;

#define MAX_SPEAKER_COUNT 30

/**@brief 发音人信息数据结构定义*/
typedef struct
{
	int nId;			///<发音人ID
	char pcName[32];			///<发音人名字
	DWORD dwTamber;		///< 发音音色
	DWORD dwLocalism;		///<发音方言		
}CSTTSSpeakerInfo_S;

/**@brief 所有发音人信息数据结构定义*/
typedef struct
{
	int nSpkCount;		///<发音人总数
	CSTTSSpeakerInfo_S asSpkInfos[MAX_SPEAKER_COUNT];
}CSTTSAllSpeakerInfos_S;

/**@brief 发音音量枚举定义*/
typedef enum
{
	EM_TTS_VOLUME_NORMAL,		///< 常规音量
	EM_TTS_VOLUME_MIN,			///< 最小音量(静音)
	EM_TTS_VOLUME_MAX			///< 最大音量
}CSTTSVolumeLevel_E;

/**@brief 发音语速枚举定义*/
typedef enum
{
	EM_TTS_SPEED_NORMAL,		///< 常规语速
	EM_TTS_SPEED_MIN,			///< 最慢语速
	EM_TTS_SPEED_MAX		///< 最慢语速
}CSTTSVoiceSpeedLevel_E;

/**@brief 文本代码页枚举定义*/
typedef enum
{
	EM_TTS_CODEPAGE_ASCII,		///< ASCII
	EM_TTS_CODEPAGE_GBK,			///< GBK (default)
	EM_TTS_CODEPAGE_BIG5,			///< Big5 
	EM_TTS_CODEPAGE_UTF16LE,		///< UTF-16 little-endian
	EM_TTS_CODEPAGE_UTF16BE,		///< UTF-16 big-endian
	EM_TTS_CODEPAGE_UTF8			///< UTF-8 
}CSTTSCodePageType_E;

/**@brief 文本语言枚举定义*/
typedef enum
{
	EM_TTS_LANGUAGE_CHINESE,		///< Chinese
	EM_TTS_LANGUAGE_ENGLISH		///< English
}CSTTSLanguageType_E;

/**@brief 设置参数选项枚举定义*/
typedef enum
{
	EM_TTS_OPTION_SPEAKER,		///< 设置发音人
	EM_TTS_OPTION_VOLUME,		///< 设置音量
	EM_TTS_OPTION_SPEED,			///< 设置发音语速
	EM_TTS_OPTION_CODEPAGE,		///< 设置输入文本代码页
	EM_TTS_OPTION_LANGUAGE		///< 设置输入文本语言
}CSTTSSetParamOption_E;

/**
@brief 释放语音引擎资源 

@param[in] hTTS 语音引擎资源句柄
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSTTSDestory(HCSHANDLE hTTS);

/**
@brief 获取所有发音人信息

@param[in] hTTS 语音引擎资源句柄
@param[out] psSpeakerInfos 所有发音人信息
@return 成功返回TRUE, 失败返回FALSE
*/
BOOL CSTTSGetAllSpeakerInfos(HCSHANDLE hTTS, CSTTSAllSpeakerInfos_S* psSpeakerInfos);

/**
@brief 设置参数

@param[in] hTTS 语音引擎资源句柄
@param[in] eOption 要设置参数选项，参见CSTTSSetParamOption
@param[in] nValue 要设置参数值，具体取值如下:
	eOption取EM_TTS_OPTION_SPEAKER: 发音人索引
	eOption取EM_TTS_OPTION_VOLUME: 取值参见CSTTSVolumeLevel
	eOption取EM_TTS_OPTION_SPEED: 取值参见CSTTSVoiceSpeedLevel
	eOption取EM_TTS_OPTION_CODEPAGE: 取值参见CSTTSCodePageType
	eOption取EM_TTS_OPTION_LANGUAGE: 取值参见CSTTSLanguageType
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSTTSSetParam(HCSHANDLE hTTS, CSTTSSetParamOption_E eOption, int nValue);

/**
@brief获取参数值

@param[in] hTTS 语音引擎资源句柄
@param[in] eOption 要获取参数选项，参见CSTTSSetParamOption
@return 要获取参数的取值,失败返回-1
*/
int CSTTSGetParam(HCSHANDLE hTTS, CSTTSSetParamOption_E eOption);

/**
@brief 将指定文本合成、播音

@param[in] hTTS 语音引擎资源句柄
@param[in] pcText 要合成播音的文本buffer 
@param[in] dwTextBufferSize 要合成播音的文本需要的缓冲区大小
@return 成功返回TRUE; 失败返回FALSE
*/
BOOL CSTTSPlay(HCSHANDLE hTTS, const char* pcText, DWORD dwTextBufferSize);

/**
@brief 暂停文本合成、播音

@param[in] hTTS 语音引擎资源句柄
@return 成功返回TRUE; 失败返回FALSE
*/
BOOL CSTTSPause(HCSHANDLE hTTS);

/**
@brief 恢复文本合成、播音

@param[in] hTTS 语音引擎资源句柄
@return 成功返回TRUE; 失败返回FALSE
*/
BOOL CSTTSResume(HCSHANDLE hTTS);

/**
@brief 停止文本合成、播音

@param[in] hTTS 语音引擎资源句柄
@return 成功返回TRUE; 失败返回FALSE
*/
BOOL CSTTSStop(HCSHANDLE hTTS);


#ifdef __cplusplus
}
#endif

#endif

