#ifndef CSSHELL_VOICE_H
#define CSSHELL_VOICE_H
#include "mmcp_typedef.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**@brief 当前声音的类型分类方式*/
typedef enum _CSShellVoiceType
{
	VOICETYPE_NONE,            //不含分类功能
	VOICETYPE_NO_PCM,  //非pcm类型
	VOICETYPE_PCM  //pcm类型
}CSShellVoiceType_E;

/*
*@brief voice初始化参数,定义了一系列函数指针，这些函数指针由应用的voice实现(C代码)，然后注册给shell。每个函数指针对应mediaplayer对象的音量和静音中一个接口
@see mediaplayer*/
typedef struct _CSShellVoiceInitParam_S
{
	int m_nSize;	///<本结构大小，一定等于sizeof(CSShellvoiceInitParam_S)
	/**
	@brief  设置音量值
	@param[in] eType : 表示当前正在播放节目的类型，参考CSShellVoiceType_E定义;
	@param[in] nVolume : 音量值 用户需要输出的音量值，按[0,31]阶音量计算;
	@return 成功：0 失败：-1
	*/
	int (* m_fnSetVolume)(CSShellVoiceType_E eType, int nVolume);

	/**
	@brief  得到当前正在使用的音量值
	@param[out] pnVolume:音量大小,按[0,31]阶音量计算;
    @return 成功：0 失败：-1
	*/
	int (* m_fnGetVolume)(int *pnVolume);

	/**
	@brief 设置静音
	@param[in] eType : 表示当前正在播放节目的类型，参考CSShellVoiceType_E定义;
	@param[in] bIsMute 静音：1  不静音：0
	@return 成功：0 失败：-1
	*/
	int (* m_fnSetMuteFlag)(CSShellVoiceType_E eType, BOOL bIsMute);

	/**
	@brief  得到当前正在使用的静音标志
	@param[out]:pbIsMute 静音0:  不静音
    @return 成功：0 失败：-1
	*/
	int (* m_fnGetMuteFlag)(BOOL *pbIsMute);

}CSShellVoiceInitParam_S;


/**
@brief 设置音量和静音相关的接口
@param psParam 设置音量和静音shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellVoiceSetParam(CSShellVoiceInitParam_S * psParam);


/**
@brief 获取设置音量和静音函数接口
@param 无
@return psParam 设置音量和静音shell层要实现的接口
*/
const CSShellVoiceInitParam_S* CSShellVoiceGetParam();


#ifdef __cplusplus
}
#endif

#endif

