#ifndef _CS_SYSAUDIO_H_
#define _CS_SYSAUDIO_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "udi_audio.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 静音*/
typedef enum _CSSYSAudioOpenType
{
	CS_MODE_AUD_UNMUTE,			/**< 取消静音*/
	CS_MODE_AUD_MUTE			/**< 静音*/
}CSSYSAudioOpenType;

/**@brief 音频事件类型 */
 typedef enum _CSSYSAUDEvent_E
{
	EM_AUD_NO_EVT			=0x00000000,	/**< 没有任何事发生*/
	EM_AUD_SOURCE_CHANGED_EVT	=0x00000001,	/**< 数据源发生变化*/
	EM_AUD_OVERFLOW_EVT		=0x00000002,	/**< 缓存溢出*/
	EM_AUD_UNDERFLOW_EVT		=0x00000004,	/**< 缓存数据耗尽，即没有数据播了*/
	EM_AUD_PAKET_START 		=0x000000008,	/**< todo 需添加注释*/
	EM_AUD_PAKET_STOP 		=0x000000010,	/**< todo 需添加注释*/
	EM_AUD_ALL_EVT			=0x0000001f	/**< 事件上限*/
}CSSYSAUDEvent_E;

/**
@brief audio模块回调函数

@param[in] dwEvent 发生的事件，CSSYSAUDEvent_E中的一个事件或组合
@param[in] pvParam 暂时保留，目前传NULL
@param[in] pvUserData 用户调用CSSYSAUDRegisterEvent注册时的自定义数据pvUserData
@note 在用户注册的事件发生是被调用
*/
typedef void (*FAudioCallback)(DWORD dwEvent,PVOID pvParam,PVOID pvUserData);


/**
@brief AudioDecoder需要数据的回调定义

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来。具体参看设备关联接口文档定义
@param[in] pvParam 暂时保留，目前必须为NULL
@note 该接口实际是音频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放			此函数无效。
@note		struct _OnAudioNeedData_S
@note		{ 
@note		int    nJumpTimeM;      nJumpTimeMs 跳跃间隔
@note	    UINT  uNeedDataSize;  uNeedDataSize 所需注入的数据量，BYTE为单位
@note       void *  pUserData;     pUserData 用户自定义参数
@note		}OnAudioNeedData_S 
@note		pvParam = &(OnAudioNeedData_S);
@note nJumpTimeMs是指上一次注入的数据尾距离本次注入数据头中间的时间间隔，对于回放等Trick操
@note 作将用该数据决定读取数据的位置，该为值为0表示是连续的注入。
*/
typedef void(*FOnAudioNeedData)(HCSHANDLE hHandle,  PVOID pvParam);

/**
@brief AudioDecoder产生一个新帧的回调定义

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来。具体参看设备关联接口文档定义。
@param[in] pUserData 用户自定义参数，目前必须为NULL
@note 在用户注册的事件发生是被调用
*/
typedef void(*FOnAudFrameCome)(HCSHANDLE hHandle, PVOID pUserData);



/*----------------------------------音频相关回调待讨论----------------------------------*/

/**
@brief 注册AudioDecoder新帧的回调。

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] fnCallback 回调
@param[in] hReserved 用户自定义参数
*/
void CSSYSAUDRegNewFrameCome(HCSHANDLE hHandle, FOnAudFrameCome fnCallback, HCSHANDLE hReserved);

/**
@brief 通过不同事件来控制音频开关

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] enmEventMsg 对应的事件类型。参看 SYS Event Msg 的定义。
@param[in] enmAudioOpenType  操作音频输出的类型。打开还是禁音
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSYSOpenAudioByEventMsg( HCSHANDLE hHandle, CSSYSEventMsg enmEventMsg, CSSYSAudioOpenType enmAudioOpenType );

/**
@brief 设置对audio开关起控制作用的事件的Mask，这个mask可看作是事件的属性

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] nEvent 对应的事件类型（异常或正常事件，如Tuner事件传入CSSYSEVENT_SIGNAL_LOCK或CSSYSEVENT_SIGNAL_LOST）。
@param[in] dwMask 事件的Mask（或者说是属性）
@return 成功返回TRUE,失败返回FALSE
@note 使用该接口可以实现禁用或启用一个Event类型对audio开关的控制，也可以设置该Event类型是否节目相关。
@note 一个Event类型禁用之后，如果再针对该Event调用CSSYSOpenAudioByEventMsg
@note 这些事件的状态仍然会被记录，但不参与音频的开关控制。
@note 当有一个事件禁用或启用，音频的开关状态并不会立即改变，
@note 直到CSSYSOpenAudioByEventMsg被调用。
*/
BOOL CSSYSOpenAudioSetEventMask(HCSHANDLE hHandle, int nEvent, DWORD dwMask); 

/**
@brief 获取对audio开关起控制作用的事件的Mask

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] nEvent 	对应的事件类型（异常或正常事件，如Tuner事件传入CSSYSEVENT_SIGNAL_LOCK或CSSYSEVENT_SIGNAL_LOST）。
@return  事件的Mask（或者说是属性）
*/
DWORD CSSYSOpenAudioGetEventMask(HCSHANDLE hHandle, int nEvent); 

/**
@brief 注册audio模块对应的回调函数

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] fnCallback 回调函数，可以为空
@param[in] pvUserData 用户自定义数据，会在fnCallback函数调用时用pvUserData返回给用户
@return 成功返回TRUE,失败返回FALSE
@note 可被多次调用，以最后一次调用结果为准，fnCallback为NULL为反注册
*/
BOOL CSSYSAUDRegisterEvent(HCSHANDLE hHandle,FAudioCallback fnCallback,PVOID pvUserData);

/**
@brief 设置Auideo模块监控的事件

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] dwEventMask 设置当前模块要监控的事件,为CSSYSAUDEvent_E中枚举变量的组合或单值
@return 成功返回TRUE,失败返回FALSE
@note 举例说明:如果dwEventMask=EM_AUD_SOURCE_CHANGED_EVT|EM_AUD_OVERFLOW_EVT,
@note 那么，当节目源发生变化或者audio buffer ovferlow，CSSYSAUDRegisterEvent函数注册的回调函数会被调用
*/
BOOL CSSYSAUIDSetEventMask(HCSHANDLE hHandle,DWORD dwEventMask);

/**
@brief 获取Audio模块事件监控掩码

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[out]  pdwEventMask 当前Audio模块监控的事件，为CSSYSAUDEvent_E中枚举变量的组合或单值
@return 成功返回TRUE,失败返回FALSE
@note 如果对应位为1，则说明系统正在监控该事件
*/
BOOL CSSYSAUDGetEventMask(HCSHANDLE hHandle,DWORD* pdwEventMask);






/**
@brief 初始化MP3软解码模块
@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来。具体参看设备关联接口文档定义。
@param[in]  userData 其他数据，扩展用
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSSYSAUDSoftMP3Init(HCSHANDLE hHandle,HCSHANDLE userData);

/*----------------------------------ES注入接口----------------------------------*/

/**
@brief 为Audio ES注入做初始化工作

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] pstESBufferInfo Audio ES注入的Buffer的相关的信息，例如音频Buffer的地址和大小等。参看Audio ES  Buffer Info 数据结构。 
@return 成功返回TRUE,失败返回FALSE
@note 初始化ES注入所需要的Audio Buffer的相关的信息，并分配buffe，大小通过初始化参数传入，默认音频位64k。
*/
BOOL CSAUDESInit(HCSHANDLE hHandle, CSESBufferInfo* pstESBufferInfo);

/**
@brief 终止AUD ES的注入

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来 
@return 成功返回TRUE,失败返回FALSE
@note 终止AUD ES的注入，并回收buffer。
*/
BOOL CSAUDESTerm(HCSHANDLE hHandle);

/**
@brief 获得ES注入AudioBuffer的空闲空间的大小

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] pnBufSize ES注入所需AudioBuffer的大小。
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSESGetAudioBufferFreeSize(HCSHANDLE hHandle, int * pnBufSize);

/**
@brief 获得注入空间的起始地址和大小

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] nBufSize 要获取的AudioBuffer的大小
@param[out] ppucBuffer ES注入所需的AudioBuffer的地址
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSESGetAvailAudioBuffer(HCSHANDLE hHandle, int  nBufSize, PBYTE *  ppucBuffer);

/**
@brief 清空ES注入的整个AudioBuffer

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSESAudioBufferReset(HCSHANDLE hHandle);

/**
@brief 往Audiobuffer中写入数据

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] pucBuffer 要进行ES注入的audio数据的存放的地址。
@param[in] nBufSize 要进行ES注入的audio数据的大小。
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSESAudioBufferPut(HCSHANDLE  hHandle,  PBYTE pucBuffer, int nBufSize);

/**
@brief 设置Audio数据注入的同步信息

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] nFirstInjectSize 第一次进行ES注入的Audio数据的大小
@param[in] nFirstMilliSeconds 第一次进行Audio数据ES注入的时间
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSESSetAudioBufferSyncInfo(HCSHANDLE hHandle,  int  nFirstInjectSize,int  nFirstMilliSeconds);

/**
@brief 获得注入的AV ES流PTS数据

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] syncInfo  返回的的包含Audio ES流的PTS数组。
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSESGetAudioCurrentPTS(HCSHANDLE hHandle, CSESSyncInfoEx* syncInfo);

/**
@brief 设置ES流注入的音频解码器的ＰＴＳ

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] un64PTS  [in]: 欲设置的以clock_t类型的音频流ＰＴＳ
@return 成功返回TRUE,失败返回FALSE
@note 该函数接口用于IPTV的ＥＳ流注入同步管理.
*/
BOOL CSESSetAudioPTSEx(HCSHANDLE hHandle, U64CLock_t un64PTS);

/**
@brief 获取当前注入的ES流中是否存在音频流

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@return 成功返回TRUE,失败返回FALSE
@note 该函数接口用于IPTV。
*/
BOOL CSESIsHaveAudio(HCSHANDLE hHandle );

/**
@brief 告知音频解码器本次注入的数据头与上次注入的数据尾是否是不连续注入

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] bDiscontinuity TRUE不连续，FALSE连续
@return 成功返回TRUE,失败返回FALSE
@note 该接口实际是音频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放此函数无效。
*/
BOOL CSSYSAUDDiscontinuity(HCSHANDLE hHandle, BOOL bDiscontinuity);

/**
@brief 在接收到FOnAudioNeedData回调注入数据后，调用该函数告知音频解码器注入完成。

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@return 成功返回TRUE,失败返回FALSE
@note 该接口实际是音频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放此函数无效。
*/
BOOL CSSYSAUDInjectComplete(HCSHANDLE hHandle);

/**
@brief 设置ES数据注入的音频回调函数。

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] pfnCallback 上层注册的用于获得decoder 信息回调函数指针。
@param[in] pvUserData 提供给回调函数的参数指针。
@return 成功返回TRUE,失败返回FALSE
@note 该回调函数用于IPTV应用，主要用来通知 STB IP端客户器ES流Decoder的数据状态，如数据上溢，
@note 下溢等，以便IP客户端与前端IPTV服务器进行协调，更新前端IP数据发送的速率，达到完美的播放效果。
*/
BOOL CSESSetAUDDataInjectCallback(HCSHANDLE hHandle, CSESDataInjectCallback pfnCallback, PVOID pvUserData);

/**
@brief 注册AudioDecoder需要数据的回调函数。

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] fnCallback 回调
@param[in] hReserved 用户自定义参数
@return 成功返回TRUE,失败返回FALSE
@note 该接口实际是音频进入Trick Mode时调用的函数，用于支持回放和时移。实时流播放此函数无效。
*/
void CSSYSAUDRegNeedDataCallback(HCSHANDLE hHandle, FOnAudioNeedData fnCallback, HCSHANDLE hReserved);

/**
@brief 设置音视输出设备

@param[in] hHandle 底层设备操作句柄，由应用通过调CSRMConnect()分配得来
@param[in] dwOutputDevice 音频输出端口，可为CSAUDOutputDevice中各项相或
@param[in] hReserved 保留参数，目前必须为NULL
@return 成功返回TRUE,失败返回FALSE
@note 供用户设定采用何种音频输出设备
*/
BOOL CSSYSAUDSetOutputDevice(HCSHANDLE hHandle, DWORD dwOutputDevice, HCSHANDLE hReserved);

/**@brief 清除Audio Decode Buffer中的数据

@param[in] hHandle[in]: 底层音频设备操作句柄，由应用通过调CSRMConnect() 分配得来。具体参看设备关联接口文档定义
@param[in] hReserved[in]: 该参数保留，以便日后的功能扩展，目前不使用，默认值设为NULL
@return 操作成功返回TRUE， 操作失败返回FALSE
*/
BOOL CSSYSAUDClearDecodeBuffer(HCSHANDLE hHandle, HCSHANDLE hReserved);

#ifdef  __cplusplus
}
#endif

#endif


