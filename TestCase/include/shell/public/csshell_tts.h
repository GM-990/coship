/**@defgroup PublicShellTTS PublicShellTTS 定义TTS对外接口

@version 1.0.0 2011/9/7 初稿
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
@brief TTS初始化参数
@see CSShellTTSSetParam CSShellTTSGetDefaultParam
*/
typedef struct _CSShellTTSInitParam
{
    int m_nSize;    ///<本结构大小，必须赋值为sizeof(CSShellTTSInitParam_S)

    /**
	@brief 释放语音引擎资源 
	@param[in] hTTS 语音引擎资源句柄
	@return 成功时应返回TRUE;失败时应返回FALSE
	*/
	BOOL (*m_fnDestory)(HCSHANDLE hTTS);

	/**
	@brief 获取所有发音人信息
	@param[in] hTTS 语音引擎资源句柄
	@param[out] psSpeakerInfos 所有发音人信息
	@return 成功时应返回TRUE;失败时应返回FALSE
	*/
	BOOL (*m_fnGetAllSpeakerInfos)(HCSHANDLE hTTS, CSTTSAllSpeakerInfos_S * psSpeakerInfos);

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
	@return 成功时应返回TRUE,失败时应返回FALSE
	*/
	BOOL (*m_fnSetParam)(HCSHANDLE hTTS, CSTTSSetParamOption_E eOption, int nValue);

	/**
	@brief 获取参数值
	@param[in] hTTS 语音引擎资源句柄
	@param[in] eOption 要获取参数选项，参见CSTTSSetParamOption
	@return 要获取参数的取值,失败时应返回-1
	*/
	int (*m_fnGetParam)(HCSHANDLE hTTS, CSTTSSetParamOption_E eOption);

	/**
	@brief 将指定文本合成、播音
	@param[in] hTTS 语音引擎资源句柄
	@param[in] pcText 要合成播音的文本buffer 
	@param[in] dwTextBufferSize 要合成播音的文本需要的缓冲区大小
	@return 成功时应返回TRUE; 失败时应返回FALSE
	*/
	BOOL (*m_fnPlay)(HCSHANDLE hTTS, const char * pcText, DWORD dwTextBufferSize);

	/**
	@brief 暂停文本合成、播音
	@param[in] hTTS 语音引擎资源句柄
	@return 成功时应返回TRUE; 失败时应返回FALSE
	*/
	BOOL (*m_fnPause)(HCSHANDLE hTTS);

	/**
	@brief 恢复文本合成、播音
	@param[in] hTTS 语音引擎资源句柄
	@return 成功时应返回TRUE; 失败时应返回FALSE
	*/
	BOOL (*m_fnResume)(HCSHANDLE hTTS);

	/**
	@brief 停止文本合成、播音
	@param[in] hTTS 语音引擎资源句柄
	@return 成功时应返回TRUE; 失败时应返回FALSE
	*/
	BOOL (*m_fnStop)(HCSHANDLE hTTS);

	/**
	@brief 设置科大讯飞语音引擎所需资源包的默认路径
	@param[in] pcDefResPath 语音引擎所需资源路径
	@return 成功时应返回TRUE; 失败时应返回FALSE
	*/
	BOOL (*m_fnSetDefaultResPath)(const char * pcDefResPath, DWORD dwResPathLen);

	/**
	@brief 增加一个科大讯飞引擎所需资源包的的路径
	@param[in] pcResPath 语音引擎所需资源路径
	@param[in] dwResPathLen pcResPath的长度
	@return 成功时应返回TRUE; 失败时应返回FALSE
	@remark	1)增加一个已存在的资源包路径，直接返回成功；
			2)可通过m_fnDeleteResPath来删除设置的资源包路径；
			3)该接口不支持多线程操作；
	*/
	BOOL (*m_fnAddResPath)(const char * pcResPath, DWORD dwResPathLen);

	/**
	@brief 删除科大讯飞引擎所需资源包的的路径
	@param[in] pcResPath 语音引擎所需资源路径
	@param[in] dwResPathLen pcResPath的长度
	@return 成功时应返回TRUE; 失败时应返回FALSE
	@remark	1)本接口只删除由m_fnAddResPath设置的资源包路径；
			2)当参数pcResPath传入NULL时，删除由m_fnAddResPath设置的全部资源路径；
			3)删除一个不存在的资源路径时，返回失败；
			4)该接口不支持多线程操作；
	*/
	BOOL (*m_fnDeleteResPath)(const char * pcResPath, DWORD dwResPathLen);

	/**
	@brief 创建科大讯飞语音引擎资源句柄
	@param[in] pResPath 语音引擎所需资源路径
	@return 成功时应返回资源句柄;失败时应返回NULL
	@remark 1)若pResPath不为NULL，则实际使用pResPath + m_fnAddResPath设置的路径；
			2)若pResPath为NULL，则实际使用m_fnSetDefaultResPath设置的路径 + m_fnAddResPath设置的路径；
			3)如果不调用m_fnSetDefaultResPath设置默认路径，也不调用m_fnAddResPath添加路径，则pcResPath不能赋为NULL；
	*/
 	HCSHANDLE (*m_fnCreateIFlyTek)(const char * pcResPath);

}CSShellTTSInitParam_S;

/**
@brief 设置TTS相关参数
@param[in] psTTSParam TTS初始化参数
@return 成功返回TRUE，失败返回FALSE
@note 项目使用默认实现即可满足需求
@code
CSShellTTSSetParam(CSShellTTSGetDefaultParam());
@endcode
*/
BOOL CSShellTTSSetParam(const CSShellTTSInitParam_S * psTTSParam);

/**
@brief 获取初始化过的TTS相关参数
@param 无
@return 成功返回之前设置的TTS实现接口
@see CSShellTTSGetDefaultParam
*/
const CSShellTTSInitParam_S * CSShellTTSGetParam(void);
	
/**
@brief 获取TTS默认实现接口
@return 成功时应返回TTS Shell接口的默认实现
@note 
*/
const CSShellTTSInitParam_S * CSShellTTSGetDefaultParam(void);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

