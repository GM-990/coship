/**@defgroup PlayerAdapter PlayerAdapter_Register 定义了播放器适配模块中,
       调用注册相关的接口及结构。
@brief 本模块主要定义了播放器适配模块调用注册方面的相关接口。

@version 1.0.0 2011/01/11 
@{
*/

#ifndef	_CS_PLAYERADATPTER_REGISTER_H_
#define	_CS_PLAYERADATPTER_REGISTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"
#include "cs_selection.h"
#include "cs_mediaplayer.h"
#include "udi2_player.h"

/**@brief Player适配层操作函数结构体

定义Player适配层播放控制相关的回调函数原型.
*/
typedef struct _CSPlayerAdapterOpt_S {

	/**
	@brief 必须赋值为本结构体的大小(sizeof(CSPlayerAdapterOpt_S))，用来做有效性判断
	@note 
	*/ 
	int  nSize;
	/**
	@brief 判断指定的url 是否支持播放
	@param[in] pcURL 需要播放的URL地址
	@return 成功返回0, 失败返回-1
	@note 
	*/ 

	 int (*Probe)(const char * pcURL);
	/**
	@brief 根据指定的URL打开一个播放器.
	@param[in] pcURL 需要播放的URL地址
	@param[in] pUserData 需要保存的用户数据
	@param[out] phPlayer 返回创建的Player句柄
	@return 成功返回0, 失败返回-1
	@note 
	*/ 

	 int (*Open)(const char *pcURL,  HCSHANDLE hUserData, HCSHANDLE *phPlayer);

	/**
	@brief 开始播放.
	@param[in] hPlayer 播放器句柄
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	 int (*Start)(HCSHANDLE hPlayer);

	/**
	@brief 暂停播放.
	@param[in] hPlayer 播放器句柄
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	 int (*Pause)(HCSHANDLE hPlayer);

	/**
	@brief 恢复播放.
	@param[in] hPlayer 播放器句柄
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	 int (*Resume)(HCSHANDLE hPlayer);

	/**
	@brief 跳转到指定时间点播放.
	@param[in] hPlayer 播放器句柄
	@param[in] nTime 需要跳转到的播放时间时, 单位s
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	 int (*Seek)(HCSHANDLE hPlayer, DWORD nTime);

	/**
	@brief 设置播放速度
	@param[in] hPlayer 播放器句柄
	@param[in] nTime 需要跳转到的播放时间时, 单位s
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	 int (*SetSpeed)(HCSHANDLE hPlayer, int nRate);

	/**
	@brief 停止播放.
	@param[in] hPlayer 播放器句柄
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	 int (*Stop)(HCSHANDLE hPlayer);

	/**
	@brief 关闭播放器.
	@param[in] hPlayer 播放器句柄
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	 int (*Close)(HCSHANDLE hPlayer);

	/**
	@brief 获取当前播放的位置
	@param[in] hPlayer 播放器句柄
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	int (*GetCurrentPlayTime)(HCSHANDLE hPlayer, int*pdwTime);

		/**
	@brief 获取节目的总时长
	@param[in] hPlayer 播放器句柄
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	int (*GetDuration)(HCSHANDLE hPlayer, DWORD  *pdwDuration);


	/**
	@brief 增加播放控制消息回调.
	@param[in] hPlayer 播放器句柄
	@param[in] dwUserData 用户数据
	@param[in] pfCallback 回调函数指针
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	int (*AddCallBack)(HCSHANDLE hPlayer, DWORD dwUserData, FOnCSSELCallback fnCallback);

	/**
	@brief 删除播放控制消息回调.
	@param[in] hPlayer 播放器句柄
	@param[in] dwUserData 用户数据
	@param[in] pfCallback 回调函数指针
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	int (*RemoveCallBack)(HCSHANDLE hPlayer, DWORD dwUserData, FOnCSSELCallback fnCallback);

	/**
	@brief 获取播放器当前状态
	@param[in] hPlayer 播放器句柄
	@param[out] peState 播放器当前状态值
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	int (*GetCurrentState)(HCSHANDLE hPlayer, CSMPState_E *peState);

	/**
	@brief 设置播放的音频流。因为一路节目可能包含多个音频流，
        比如：国语流，粤语流，英语流等，可以根据用户的需求选择需要播出
        的音频流
	@param[in] hPlayer 播放器句柄
	@param[in] nProgramId 节目ID
	@param[in] nVideoPid 视频ID
	@param[in] nAudioPid 音频ID
	@param[in] nSubTitleId 字幕ID
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	int (*SwitchPidToPlay)(HCSHANDLE hPlayer, int nProgramId, int nVideoPid, int nAudioPid, int nSubTitleId);

	/**
	@brief 获取视频文件的属性
	@param[in] pcFileLocation 文件的路径
	@param[out] pstFileInfo 视频文件的属性值
	@return 成功返回0, 失败返回-1
	@note 
	*/ 
	int (*GetFileInfo)(const char *pcFileLocation, CSUDIPlayerFileInfo_S *pstFileInfo);

	/**
	@brief 获取播放器句柄
	@return Adapter Player句柄
	@note 
	*/ 
	HCSHANDLE (*GetAdapterPlayer)(void);
}CSPlayerAdapterOpt_S;

/**
@brief 获取ffmpeg的操作集
 

@return 成功返回ffmpeg 操作集结构体指针
@note 当工程需要使用ffmpeg适配播放器模块时, 调用该函数, 即将适配播放器模块
	  注册到工程中.
	  示例代码如下:
	  CSMPPlayerAdapterRegist(CSMPPlayerAdapterGetFfmpegOpts());
*/
const CSPlayerAdapterOpt_S * CSMPPlayerAdapterGetFfmpegOpts();
/**
@brief 获取uid2 播放的操作集
 

@return 成功返回uid2 操作集结构体指针
@note 当工程需要使用uid2适配播放器模块时, 调用该函数, 即将适配播放器模块
	  注册到工程中.
	  示例代码如下:
	  CSMPPlayerAdapterRegist(CSMPPlayerAdapterGetUdi2Opts());
*/
const CSPlayerAdapterOpt_S* CSMPPlayerAdapterGetUdi2Opts();

/**
@brief 调用适配播放器模块注册函数.
 
@param[in] pOpt  适配播放器处理结构体
@return 成功返回TRUE, 失败返回FALSE
@note 当工程需要使用适配播放器模块时, 调用该函数, 即将适配播放器模块
	  注册到工程中.
	  示例代码如下:
	  CSMPPlayerAdapterRegist(CSMPPlayerAdapterGetUdi2Opts);
*/
BOOL CSMPPlayerAdapterRegist( const CSPlayerAdapterOpt_S  * pOpt );

/**
@brief 获取播放文件信息.
 
@param[in] pszLocator  播放文件的url
@param[out] pstFileInfo  播放文件信息
@return 成功返回TRUE, 失败返回FALSE
*/
BOOL CSPlayerAdapterGetFileInfo(const char *pszLocator, CSUDIPlayerFileInfo_S *pstFileInfo);

#ifdef __cplusplus
	}
#endif

#endif /* _CS_PLAYERADATPTER_REGISTER_H_ */


