/**@defgroup PLAYER  PLAYER 定义了平台无关的音视频播放控制接口
@brief PLAYER，INJECT，RECORD三个模块分别对应头文件<udi2_player.h>、<udi2_inject.h>和<udi2_record.h>。它们利用多种设备共同提供播放、注入、录制服务的抽象。这些服务之间，通过设备id进行关联。例如一个注入类型的播放和一个TS流类型的注入如果要联合起来完成一个TS流注入播放工作（也只有他们联合起来才能完成这个工作），则两者会使用同一个Demux。通过这个Demux，驱动及MMCP（CloudTV ）中间件平台会把他们关联起来 。

下面将通过一些常见的场景，描述三个模块的使用方式，以助于理解这三个模块的实现。

- DVB直播
选取一个拥有EM_UDI_DEMUX_PLAY及EM_UDI_DEMUX_DMA能力的Demux，
播放窗口对应的解码器(主窗口播放为0，pip窗口为1)，
可用的Audio解码器(一般除了PCM“解码器”外只会有一个)，
创建一个EM_UDIPLAYER_LIVE类型的Player进行播放。

- I帧播放
选取一个拥有EM_UDI_VID_STREAM_MPEG2能力的Video，其它不选（均为-1)
创建类型为EM_INJECTER_ES注入内容为EM_UDIINJECTER_CONTENT_IFRAME的Inject
通过设置EM_UDIINJECTER_IFRAME_PARAMS方式把数据注入
然后用该Video创建Player进行播放

- TS注入（IPTV）
选取一个拥有EM_UDI_DEMUX_INJECT及EM_UDI_DEMUX_PLAY能力的Demux，创建Inject。
选取此Demux及相应Video、Audio(参加DVB直播)，创建一个EM_UDIPLAYER_INJECT型的Player进行播放。

- PCM注入：
选取一个Audio(一般来说会独立一个只能解PCM的，若不支持混音则只有唯一一个Audio)，创建Inject。
用同一个Audio创建一个EM_UDIPLAYER_INJECT型的Player进行播放。

- ES注入
分别用一个Audio及Video创建两个Inject，并用它们创建一个Player进行播放。

- 网络AVI文件注入播放
选取一个有支持EM_UDI_DEMUX_INJECT_AVI容器的拥有EM_UDI_DEMUX_INJECT及EM_UDI_DEMUX_PLAY
能力的Demux，创建Inject，将从网络获取的数据注入到此Inject中。
选取此Demux及相应Video、Audio创建一个EM_UDIPLAYER_INJECT类型Player，设置CSUDIPLAYERSetStream
为UNKNOWN的音视频，不设置回放参数进行播放。

- 本地AVI文件注入播放
选取一个有支持EM_UDI_DEMUX_INJECT_AVI容器的拥有EM_UDI_DEMUX_INJECT及EM_UDI_DEMUX_PLAY
能力的Demux，选取此Demux及相应Video、Audio创建一个EM_UDIPLAYER_FILE类型的Player，（不创建Inject）
设置CSUDIPLAYERSetStream为UNKNOWN的音视频，并设置回放参数为对应AVI文件的UDI_FS路径进行播放。

-UDI 文件播放（包括本地文件和网络文件）

(1)中间件首先调用CSUDIPLAYERProbe接口，传入URL检查是否支持该格式。
(2)若支持，调用CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer)接口时，
psPlayerChnl传空。ePlayerType传EM_UDIPLAYER_FILE，
(3)调用CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam)
接口时，psStreamInfo及nStreamCnt传0，psPlaybackParam->m_szFileName传入URL（file或http）。
(4)调用CSUDIPLAYERStart接口播放。
(5)Pause Resume Seek GetCurrentTime等控制接口与现有定义相同。
(6)文件播放使用的回调类型和事件类型分别为CSUDIFilePLAYERCallback_F和CSUDIFilePlayerEventType_E
(7)调用CSUDIPLAYERGetFileInfo 接口获取播放的文件信息
(8)调用CSUDIPLAYERSetFilePlayStream 接口设置文件播放的属性，包括programid, videoid, audioid, subtitleid

-PVR 录制文件播放
(1)选取一个支持EM_UDI_DEMUX_INJECT_TS容器且拥有EM_UDI_DEMUX_PLAY及EM_UDI_DEMUX_INJECT能力的Demux，
(2)调用CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer)接口时，
psPlayerChnl传入选定的Demux及对应的Audio、Video。ePlayerType传EM_UDIPLAYER_PVR，
(3)调用CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam)
接口时，psStreamInfo传入音视频相关信息，nStreamCnt传入对应的count，psPlaybackParam->m_szFileName传入PVR录制文件路径。
(4)调用CSUDIPLAYERStart接口播放。
(5)Pause Resume Seek GetCurrentTime等控制接口与现有定义相同。

-PVR 录制时移播放
(1)选取一个支持EM_UDI_DEMUX_INJECT_TS容器且拥有EM_UDI_DEMUX_PLAY及EM_UDI_DEMUX_INJECT能力的Demux，
(2)调用CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer)接口时，
psPlayerChnl传入选定的Demux及对应的Audio、Video。ePlayerType传EM_UDIPLAYER_TSHIFT，
(3)调用CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam)
接口时，psStreamInfo传入音视频相关信息，nStreamCnt传入对应的count，psPlaybackParam->m_szFileName传入PVR时移文件路径。
(4)调用CSUDIPLAYERStart接口播放。
(5)Pause Resume Seek GetCurrentTime等控制接口与现有定义相同。


@note 另外需特别注意:
-注入播放必须保证以下面接口调用顺序调用正常工作
1.开始注入播放： CSUDIINJECTEROpen -> CSUDIPLAYEROpen -> CSUDIINJECTERStart -> CSUDIPLAYERStart
2.停止注入播放： CSUDIPLAYERStop ->	CSUDIINJECTERStop -> CSUDIPLAYERClose -> CSUDIINJECTERClose
- 创建Player时应杜绝传入冗余设备，如播放I帧时不得传入Audio
- 重大改变需重新Open（参数一般放置在Open接口中）。
- 属性改变只需Set即可（参数一般放置在Set接口中。
- 原则上不允许通过Set接口改变服务类型，如不允许通过Set接口将一个PCM注入的Inject改变为TS的Inject，因为它们所涉及的设备完全不同。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.3 2010/11/24 增加注入播放顺序描述注释
@version 2.0.2 2010/11/1 播放回调通知类型增加了EM_UDIPLAYER_END_OF_STREAM和EM_UDIPLAYER_BEGIN_OF_STREAM
@version 2.0.2 2009/8/26 增加错误代码注释和视频源改变事件类型
@version 2.0.1 2009/8/17
@{
*/

#ifndef _UDI2_PLAYER_H_
#define _UDI2_PLAYER_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_record.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define EM_UDIFILEPLAYER_ERR_BASE        (1<<16) ///<文件播放错误类型基本值
#define EM_UDIFILEPLAYER_STATE_BASE    (2<<16)	///<文件播放状态类型基本值

/**@brief 播放相关错误代码值*/
enum
{
	CSUDIPLAYER_ERROR_BAD_PARAMETER = CSUDI_PLAYER_ERROR_BASE, ///< 参数错误
	CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED,  ///< 操作不支持
	CSUDIPLAYER_ERROR_UNKNOWN_ERROR,		  ///< 一般错误
	CSUDIPLAYER_ERROR_NO_MEMORY,			  ///< 无可用内存
	CSUDIPLAYER_ERROR_INVALID_DEVICE_ID,	  ///< 非法的设备ID，
	CSUDIPLAYER_ERROR_INVALID_HANDLE,		  ///< 非法句柄
	CSUDIPLAYER_ERROR_INVALID_STATUS,	      ///< 非法状态操作，比如在没有stop时进行ModifyStream，或是在stop时进行pause
	CSUDIPLAYER_ERROR_DEVICE_BUSY,			  ///< 设备繁忙
	CSUDIPLAYER_ERROR_DEVICE_DISORDER,        ///< 错误的设备ID，配置了错误的设备ID
	CSUDIPLAYER_ERROR_CALLBACK_FULL,          ///<注册的回调已经达到最大值
	CSUDIPLAYER_ERROR_CALLBACK_EXIST,         ///< 要注册的回调已经存在
	CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST,     ///< 要删除的回调不存在
	CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER,    ///< 音视频类型混乱，比如说有两个视频，两个音频
	CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR   ///< TRICKMODE状态错误，仅用于已经在TrickMode时再次进入TrickMode或者已经在非TrickMode时再次退出TrickMode
};

/**@brief 播放速率枚举定义*/
typedef enum 
{
	EM_UDIPLAYER_SPEED_NORMAL= 0,          	///< 1
	EM_UDIPLAYER_SPEED_FASTREWIND_32,	   	///< -32		
	EM_UDIPLAYER_SPEED_FASTREWIND_16,		///< -16	
	EM_UDIPLAYER_SPEED_FASTREWIND_8,		///< -8
	EM_UDIPLAYER_SPEED_FASTREWIND_4,		///< -4 	
	EM_UDIPLAYER_SPEED_FASTREWIND_2,		///< -2	
	EM_UDIPLAYER_SPEED_SLOWREWIND_2,	   	///< -1/2		
	EM_UDIPLAYER_SPEED_SLOWREWIND_4,	   	///< -1/4		
  	EM_UDIPLAYER_SPEED_SLOWREWIND_8,	   	///< -1/8		
  	EM_UDIPLAYER_SPEED_SLOWFORWARD_2,		///< 1/2
  	EM_UDIPLAYER_SPEED_SLOWFORWARD_4,		///< 1/4
  	EM_UDIPLAYER_SPEED_SLOWFORWARD_8,	 	///< 1/8
	EM_UDIPLAYER_SPEED_FASTFORWARD_2, 		///< 2
	EM_UDIPLAYER_SPEED_FASTFORWARD_4,     	///< 4
	EM_UDIPLAYER_SPEED_FASTFORWARD_8,		///< 8
	EM_UDIPLAYER_SPEED_FASTFORWARD_16, 	  	///< 16	
  	EM_UDIPLAYER_SPEED_FASTFORWARD_32,		///< 32	
	EM_UDIPLAYER_SPEED_MAX
}CSUDIPlayerSpeed_E;	

/**@brief 播放模式枚举定义*/
typedef enum
{
	EM_UDIPLAYER_LIVE,			///< 直播模式
	EM_UDIPLAYER_FILE,			///< 文件播放模式，包括本地文件和网络文件
	EM_UDIPLAYER_TSHIFT,		///< PVR录制时移播放模式
	EM_UDIPLAYER_INJECT,		///< 数据注入播放模式
	EM_UDIPLAYER_PVR,			///< PVR录制播放模式
	EM_UDIPLAYER_MAXPLAYERTYPE  ///< 类型最大值，不可设置为该类型
}CSUDIPLAYERType_E;

/**@brief 回放播放模式(EM_UDIPLAYER_FILE、EM_UDIPLAYER_PVR和EM_UDIPLAYER_TSHIFT)下的参数数据结构*/
typedef struct 
{
	int                         m_nSecondPos;    ///< 回放的起始位置
	CSUDIPlayerSpeed_E 			m_eSpeed;           ///< 回放的速度
	char                        m_szFileName[CSUDI_MAX_URL_LEN];     	///< 回放的流文件夹路径,只支持ASCII编码格式
    CSUDIStreamEncryptParam_S   m_stEncryptParam; ///<参见CSUDIStreamEncryptParam_S
}CSUDIPlaybackParam_S;

/**@brief 播放通道设备资源数据结构定义*/
typedef struct 
{
	int m_nDemux;			///< Demux设备的Index，CSUDI_INVALID_INDEX代表不存在Demux设备
	int m_nVideoDecoder;	///< 视频解码器的设备Index，CSUDI_INVALID_INDEX代表不存在视频解码器
	int m_nAudioDecoder;	///< 音频解码器的设备Index，CSUDI_INVALID_INDEX代表不存在音频解码器
}CSUDIPlayerChnl_S;

/**@brief 回放偏移起始位置枚举定义*/
typedef enum
{
	EM_UDIPLAYER_POSITION_FROM_HEAD = 0,  		///< 从文件头开始计算偏移
	EM_UDIPLAYER_POSITION_FROM_CURRENT,		///< 从当前位置计算偏移
	EM_UDIPLAYER_POSITION_FROM_END			///< 从文件尾计算偏移
}CSUDIPlayPosition_E;

/**@brief 非本地文件播放时，音视频播放事件枚举定义*/
typedef enum 
{
	
	EM_UDIPLAYER_VIDEO_FRAME_COMING,		///< video新帧事件 此事件应该保证在视频正常解码的情况下一直可以监听到，并不是播放节目开始播放时只向上抛1次
	EM_UDIPLAYER_AUDIO_FRAME_COMING,		///< audio新帧事件
	
	EM_UDIPLAYER_VIDEO_UNDERFLOW,		    ///< 视频解码器下溢事件
	EM_UDIPLAYER_AUDIO_UNDERFLOW,		    ///< 音频解码器下溢事件  
	
	EM_UDIPLAYER_VIDEO_OVERFLOW,		    ///< 视频解码器上溢事件
	EM_UDIPLAYER_AUDIO_OVERFLOW,		    ///< 音频解码器上溢事件

	EM_UDIPLAYER_VIDEO_SOURCE_CHANGE,		///< 视频源改变事件，例如width,height,framerate参数改变
	EM_UDIPLAYER_AUDIO_SOURCE_CHANGE,		///< 音频源改变事件，例如codec改变
	
	EM_UDIPLAYER_END_OF_STREAM,				///< 播放至文件末尾，仅当播放模式为EM_UDIPLAYER_FILE、EM_UDIPLAYER_PVR或者EM_UDIPLAYER_TSHIFT时才会有该事件发生
	EM_UDIPLAYER_BEGIN_OF_STREAM,			///< 播放至文件开头, 仅当播放模式为EM_UDIPLAYER_FILE、EM_UDIPLAYER_PVR或者EM_UDIPLAYER_TSHIFT且快退或慢退时才会有该事件发生

	EM_UDIPLAYER_MAXEVENTTYPE				///< 边界值
}CSUDIPlayerEventType_E; 

/**@brief 本地文件播放，音视频播放事件枚举定义，通过CSUDIFilePLAYERCallback_F 回调上来的值*/
typedef enum 
{
	
	EM_UDIFILEPLAYER_VIDEO_FRAME_COMING,		///< video新帧事件 此事件应该保证在视频正常解码的情况下一直可以监听到，并不是播放节目开始播放时只向上抛1次
	EM_UDIFILEPLAYER_AUDIO_FRAME_COMING,		///< audio新帧事件
	
	EM_UDIFILEPLAYER_VIDEO_UNDERFLOW,		    ///< 视频解码器下溢事件
	EM_UDIFILEPLAYER_AUDIO_UNDERFLOW,		    ///< 音频解码器下溢事件  
	
	EM_UDIFILEPLAYER_VIDEO_OVERFLOW,		    ///< 视频解码器上溢事件
	EM_UDIFILEPLAYER_AUDIO_OVERFLOW,		    ///< 音频解码器上溢事件

	EM_UDIFILEPLAYER_VIDEO_SOURCE_CHANGE,		///< 视频源改变事件，例如width,height,framerate参数改变
	EM_UDIFILEPLAYER_AUDIO_SOURCE_CHANGE,		///< 音频源改变事件，例如codec改变
	
	EM_UDIFILEPLAYER_END_OF_STREAM,				///< 播放至文件末尾，仅当播放模式为EM_UDIPLAYER_FILE时才会有该事件发生
	EM_UDIFILEPLAYER_BEGIN_OF_STREAM,			///< 播放至文件开头, 仅当播放模式为EM_UDIPLAYER_FILE且快退或慢退时才会有该事件发生

	EM_UDIFILEPLAYER_SHOW_SUBTITLE,             ///< 显示字幕，pvEventData为const char* 表示要显示的字幕ASCII编码的字符串
	EM_UDIFILEPLAYER_CLEAR_SUBTITLE,            ///< 清除当前显示的字幕

	EM_UDIFILEPLAYER_ERR_ABORTED = EM_UDIFILEPLAYER_ERR_BASE,                           ///< 用户中止操作，中止成功消息
	EM_UDIFILEPLAYER_ERR_DECODE,                                ///< 解码器出错
	EM_UDIFILEPLAYER_ERR_FORMAT,                                ///< 媒体文件不存在或文件格式出错
	EM_UDIFILEPLAYER_ERR_SEEK,                         ///< 播放定位出错时，抛一次消息
	EM_UDIFILEPLAYER_ERR_PAUSE,                      ///< 暂停操作失败时，抛一次消息
	EM_UDIFILEPLAYER_ERR_RESUME,                   ///< 状态恢复失败时，抛一次消息
	EM_UDIFILEPLAYER_ERR_SETSPEED,                ///< 设置速率失败时，抛一次消息
	EM_UDIFILEPLAYER_ERR_NETWORK,                               ///< 网络异常，m_pvData为int，值为HTTP 标准error code, 比如http错误为 200, pvEventData值为200；
	EM_UDIFILEPLAYER_ERR_TIMEOUT,                    ///< 读取数据超时 

	EM_UDIPFILELAYER_STATE_LOADING = EM_UDIFILEPLAYER_STATE_BASE,                             ///< 缓存开始,每次进入缓存状态都要抛一次
	EM_UDIPFILELAYER_STATE_LOADED,            ///< 缓存结束,有足够数据开始播放
	EM_UDIPFILELAYER_STATE_HAVEMETADATA,                        ///< 能够获取一些基本信息，如片长
	EM_UDIPFILELAYER_STATE_ENOUGHDATA_FORPLAY,            ///< 有足够的数据进行播放，如进行快进快退都操作
	EM_UDIPFILELAYER_STATE_DURATIONCHANGE,                       ///< 节目总时长发生变化，需要播放器更新时长，可以通过CSUDIPLAYERGetDuration 获取新的总时长
	EM_UDIPFILELAYER_STATE_RATECHANGE,                           ///< 节目总播放倍数发生变化，可以通过CSUDIPLAYERGetSpeed 获取新的播放速率

	EM_UDIFILEPLAYER_STATE_LOADING_PROGRESS,	///< 等待播放，缓存数据的进度值；进度值和基值或起来，m_pvData为int, 百分制，比如缓冲到25%,  pvEventData值为25；
	EM_UDIFILEPLAYER_STATE_LOADING_DOWNLOADRATE,	///< 下载速率, pvEventData为int, 单位:kbytes/s, 比如网络下载速度25kbits/s, pvData值为25；
	
	EM_UDIFILEPLAYER_STATE_STOP,                 ///< 停止播放器操作成功
	EM_UDIFILEPLAYER_STATE_START,                ///< 启动播放器操作成功
	EM_UDIFILEPLAYER_STATE_PAUSE,                ///< 暂停播放器操作成功
	EM_UDIFILEPLAYER_SEEK_FINISH,                 ///< 选时播放定位成功

	EM_UDIFILEPLAYER_MAXEVENTTYPE				///< 边界值
}CSUDIFilePlayerEventType_E; 

/**@brief 设置同步模式的类型*/
typedef enum
{
	EM_UDIPLAYER_AVSYNC_PCR_MASTER,			///< 以PCR作为同步模式 (默认状态)
	EM_UDIPLAYER_AVSYNC_AUDIO_MASTER,		///< 以AUDIO PTS 作为同步模式
	EM_UDIPLAYER_AVSYNC_VIDEO_MASTER,		///< 以VIDEO PTS作为同步模式
	EM_UDIPLAYER_AVSYNC_MAX_MODE 			///< 类型最大值，不可设置为该类型
}CSUDIPLAYERAVSyncMode_E;

#define CSUDI_PLAYER_MAX_FILE_NAME_LEN         (512)///<文件名称长度
#define CSUDI_PLAYER_TITLE_MAX_LEN             (512)///<文件标题、作者等最大字符数
#define CSUDI_PLAYER_TIME_LEN                  (8)///<年份字节数
#define CSUDI_PLAYER_MAX_PROGRAM_NUM           (5)///<最大节目个数，如ts媒体文件，节目个数如果超过5个，则播放器最多只能存储其中某5个节目信息，并播放其中的某一个
#define CSUDI_PLAYER_MAX_STREAM_NUM            (5)///< 流个数 
#define CSUDI_PLAYER_MAX_LANG_NUM              (5)///<支持的最多语言种类个数
#define CSUDI_PLAYER_LANG_LEN                  (4)///<语言字符数
#define CSUDI_PLAYER_SUB_TITLE_LEN             (8)///<字幕标题长度
 
/**@brief 文件流类型，播放的文件类型 */
typedef enum
{
	 EM_UDIFILEPLAYER_STREAM_ES = 0x0,	 ///< es流文件
	 EM_UDIFILEPLAYER_STREAM_TS, 		 ///< ts流文件
	 EM_UDIFILEPLAYER_STREAM_NORMAL, 	 ///< 普通文件，如idx,lrc,srt等字幕文件
	 EM_UDIFILEPLAYER_STREAM_NET,		 ///< 网络流文件
	 EM_UDIFILEPLAYER_STREAM_END
} CSUDIPlayerStreamType_E;

/**@brief 视频流信息 */
typedef struct
{
	 CSUDI_INT32 m_n32StreamIndex;		   ///< 流索引，对于ts流，该值为流pid
	 CSUDI_UINT32 m_u32Format;			   ///< 视频编码格式，值定义参考::CSUDIVIDStreamType_E ,驱动实现的时候要注意转换
	 CSUDI_UINT16 m_u16Width;			   ///< 宽度，单位像素
	 CSUDI_UINT16 m_u16Height;			   ///< 高度，单位像素
	 CSUDI_UINT16 m_u16FpsInteger;		   ///< 帧率，整数部分
	 CSUDI_UINT16 m_u16FpsDecimal;		   ///< 帧率，小数部分
	 CSUDI_UINT32 m_u32Bps; 			   ///< 视频码率，bits/s
	 CSUDI_UINT32 m_u32ExtradataSize;	   ///< 扩展数据长度
	 CSUDI_UINT8  *m_pu8Extradata;		   ///< 扩展数据
} CSUDIPlayerVidInfo_S;

/**@brief 音频流信息 */
typedef struct
{
	 CSUDI_INT32 m_n32StreamIndex;					  ///< 流索引，对于ts流，该值为流pid，非法值为::HI_FORMAT_INVALID_STREAM_ID
	 CSUDI_UINT32 m_u32Format;						  ///< 音频编码格式，值定义参考::CSUDIAUDStreamType_E ,驱动实现的时候要注意转换
	 CSUDI_UINT32 m_u32Version; 					  ///< 音频编码版本，仅对wma编码，0x160(WMAV1), 0x161 (WMAV2)
	 CSUDI_UINT32 m_u32SampleRate;					  ///< 8000,11025,441000,...
	 CSUDI_UINT16 m_u16BitPerSample;				  ///< 音频每个采样点所占的比特数，8bit,16bit
	 CSUDI_UINT16 m_u16Channels;					  ///< 声道数, 1 or 2
	 CSUDI_UINT32 m_u32BlockAlign;					  ///< packet包含的字节数
	 CSUDI_UINT32 m_u32Bps; 						  ///< 音频码率，bits/s
	 CSUDI_BOOL m_bBigEndian;					      ///< 大小端，仅pcm格式有效
	 char m_acAudLang[CSUDI_PLAYER_LANG_LEN];         ///< 音频流语言
	 CSUDI_UINT32 m_u32ExtradataSize;				  ///< 扩展数据长度
	 CSUDI_UINT8  *m_pu8Extradata;					  ///< 扩展数据
} CSUDIPlayerAudInfo_S;

/**@brief 字幕数据类型 */
typedef enum
{
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_TEXT = 0x0,	  ///< 字符串
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_BMP, 		  ///< bmp图片
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_ASS, 		  ///< ass字幕
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_HDMV_PGS,	  ///< pgs字幕
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_BMP_SUB, 	  ///< sub字幕
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_END
} CSUDIPlayerSubtitleDataType_E;


/**@brief 字幕信息 */
typedef struct
{
	 CSUDI_INT32  m_n32StreamIndex; 						    ///< 流索引，对于ts流，该值为流pid
	 CSUDI_BOOL m_bExtSubTitle; 							    ///< 是否为外挂字幕
	 CSUDIPlayerSubtitleDataType_E m_eSubtitileType;	        ///< 字幕格式，如bmp,string
	 char m_acSubTitleName[CSUDI_PLAYER_SUB_TITLE_LEN];         ///< 字幕标题
	 CSUDI_UINT16  m_u16OriginalFrameWidth; 					///< 原始图像宽度
	 CSUDI_UINT16  m_u16OriginalFrameHeight;					///< 原始图像高度
	 CSUDI_UINT16  m_u16HorScale;								///< 水平缩放比，0-100
	 CSUDI_UINT16  m_u16VerScale;								///< 垂直缩放比，0-100
	 CSUDI_UINT32  m_u32ExtradataSize;							///< 扩展数据长度
	 CSUDI_UINT8   *m_pu8Extradata; 							///< 扩展数据
} CSUDIPlayerSubtitleInfo_S;


/**@brief 节目信息，如果文件既带内置字幕也带外置字幕，则外置字幕信息追加在内置字幕后，
 注: 如果媒体文件不带视频流，需将videstreamindex设置为HI_FORMAT_INVALID_STREAM_ID */
typedef struct
{
	 CSUDIPlayerVidInfo_S m_stVidStream;								        ///< 视频流信息
	 CSUDI_UINT32 m_u32AudStreamNum;											///< 音频流个数
	 CSUDIPlayerAudInfo_S m_astAudStream[CSUDI_PLAYER_MAX_STREAM_NUM];		    ///< 音频流信息
	 CSUDI_UINT32 m_u32SubTitleNum; 											///< 字幕个数
	 CSUDIPlayerSubtitleInfo_S m_astSubTitle[CSUDI_PLAYER_MAX_LANG_NUM];	    ///< 字幕信息
} CSUDIPlayerProgramInfo_S;

/**@brief 媒体文件信息 */
typedef struct
{
	 CSUDIPlayerStreamType_E m_eStreamType; 			        ///< 文件流类型
	 CSUDI_INT64  m_n64FileSize;								///< 文件大小，单位字节
	 CSUDI_INT64  m_n64StartTime;								///< 文件播放起始时间，单位ms
	 CSUDI_INT64  m_n64Duration;								///< 文件总时长，单位ms
	 CSUDI_UINT32  m_u32Bps;									///< 文件码率，bits/s
	 char m_acFileName[CSUDI_PLAYER_MAX_FILE_NAME_LEN];         ///< 文件名称
	 char m_acAlbum[CSUDI_PLAYER_TITLE_MAX_LEN];		        ///< 专辑
	 char m_aczTitle[CSUDI_PLAYER_TITLE_MAX_LEN];		        ///< 标题
	 char m_acArtist[CSUDI_PLAYER_TITLE_MAX_LEN];		        ///< 艺术家，作者
	 char m_acGenre[CSUDI_PLAYER_TITLE_MAX_LEN];		        ///< 风格
	 char m_acComments[CSUDI_PLAYER_TITLE_MAX_LEN]; 	        ///< 附注
	 char m_acTime[CSUDI_PLAYER_TIME_LEN];				        ///< 创建年份
	 CSUDI_UINT32  m_u32ProgramNum; 							///< 实际节目个数
	 CSUDIPlayerProgramInfo_S m_astProgramInfo[CSUDI_PLAYER_MAX_PROGRAM_NUM];	 ///< 节目信息
} CSUDIPlayerFileInfo_S;

/**@brief 音视频、字幕ID切换属性 */
typedef struct
{
    CSUDI_UINT32 m_u32ProgramId;   ///< 节目id，值为::CSUDIPlayerFileInfo_S结构m_astProgramInfo数组下标
    CSUDI_UINT32 m_u32VStreamId;   ///< 视频流id，值与m_u32ProgramId相等
    CSUDI_UINT32 m_u32AStreamId;   ///< 音频流id，值为::CSUDIPlayerProgramInfo_S结构m_astAudStream数组下标
    CSUDI_UINT32 m_u32SubTitleId;  ///< 字幕id，值为::CSUDIPlayerProgramInfo_S结构m_astSubTitle数组下标
} CSUDIPlayerFileStreamId_S;

 /**
@brief 播放回调函数的定义

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] eEvent 回调消息类型，详见CSUDIPlayerEventType_E定义
@param[in] pvUserData  用户数据类型
*/
typedef  void ( *CSUDIPLAYERCallback_F  )(CSUDI_HANDLE hPlayer,CSUDIPlayerEventType_E eEvent,void * pvUserData);

  /**
@brief 文件播放状态回调函数定义

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] ePlayerEvent 回调消息类型，详见CSUDIFilePlayerEventType_E定义
@param[in] pvEventData  回调数据，根据CSUDIFilePlayerEventType_E类型不同，pvEventData代表的根据类型而不同，详见CSUDIFilePlayerEventType_E类型定义
@param[in] pvUserData  用户数据
*/
typedef  void ( *CSUDIFilePLAYERCallback_F  )(CSUDI_HANDLE hPlayer, CSUDIFilePlayerEventType_E ePlayerEvent, void *pvEventData, void * pvUserData);

  
/**
@brief UDI Player是否支持播放传入的URL指定的媒体文件.
 
@param[in] pcURL 需要探测的媒体文件URL.
@return 支持返回CSUDI_SUCCESS, 不支持返回CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED.
@note 参考udi2_player.h文件中对第三方媒体播放器流程相关的描述.
@note 该接口主要针对芯片能够直接支持本地媒体播放，如不支持请直接返回CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/	
CSUDI_Error_Code CSUDIPLAYERProbe(const char * pcURL);

 /**
@brief  注册指定事件类型的回调函数

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] fnPlayerCallback 回调函数指针，详见CSUDIPLAYERCallback_F定义
@param[in] eEvent 回调函数类型，详见CSUDIPlayerEventType_E
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 至少支持注册的回调函数个数为32个，最大不限
@note 不允许注册EM_UDIPLAYER_MAXEVENTTYPE类型的回调
@note 回调函数指针+回调函数类型+用户数据唯一标识一个回调,当两次注册它们完全一样时，第二次将返回CSUDIPLAYER_ERROR_CALLBACK_EXIST

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERAddPlayerCallback(CSUDI_HANDLE hPlayer,CSUDIPLAYERCallback_F fnPlayerCallback,CSUDIPlayerEventType_E eEvent,void * pvUserData );

 /**
@brief 删除指定事件类型的回调函数
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] fnPlayerCallback 回调函数指针，详见CSUDIPLAYERCallback_F定义
@param[in] eEvent 回调函数类型，详见CSUDIPlayerEventType_E
@param[in] pvUserData 同CSUDIPLAYERAddPLAYERCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
@note 允许在回调函数中删除回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERDelPlayerCallback(CSUDI_HANDLE hPlayer,CSUDIPLAYERCallback_F fnPlayerCallback,CSUDIPlayerEventType_E eEvent, void * pvUserData );

 /**
@brief  注册文件播放事件类型的回调函数

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] fnPlayerCallback 回调函数指针，详见CSUDIFilePLAYERCallback_F定义
@param[in] pvUserData 用户传入的自定义指针，用来通知驱动返回给用户的数据信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 至少支持注册的回调函数个数为32个，最大不限
@note 回调函数指针+用户数据唯一标识一个回调,当两次注册它们完全一样时，第二次将返回CSUDIPLAYER_ERROR_CALLBACK_EXIST
@note 只允许ePlayerType为EM_UDIPLAYER_FILE的hPlayer时，才能注册此回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERAddFilePlayerCallback(CSUDI_HANDLE hPlayer, CSUDIFilePLAYERCallback_F fnPlayerCallback, void * pvUserData );

 /**
@brief 删除文件播放事件类型的回调函数

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] fnPlayerCallback 回调函数指针，详见CSUDIFilePLAYERCallback_F定义
@param[in] pvUserData 同CSUDIPLAYERAddFilePlayerCallback的pvUserData
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
@note 允许在回调函数中删除回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERDelFilePlayerCallback(CSUDI_HANDLE hPlayer, CSUDIFilePLAYERCallback_F fnPlayerCallback, void * pvUserData );

/**
@brief  获取播放的文件信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[out] pstFileInfo 获取的文件信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetFileInfo(CSUDI_HANDLE hPlayer, CSUDIPlayerFileInfo_S * pstFileInfo);

/**
@brief  设置播放的文件信息，被设置的文件按此属性播放

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] pstFileStreamId 需要设置的文件播放信息
@note 通过CSUDIPLAYERGetFilePlayInfo 获取需要设置的信息的索引
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetFilePlayStream(CSUDI_HANDLE hPlayer, CSUDIPlayerFileStreamId_S * pstFileStreamId);
 
/**
@brief 创建一个播放器

@param[in] psPlayerChnl   播放器通道的数据结构指针，请参见CSUDIPlayerChnl_S。
@param[in] ePlayerType   播放器播放类型，请参见CSUDIPLAYERType_E。
@param[out] phPlayer  创建成功后生成的播放器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note psPlayerChnl 中的成员设备资源索引可以为CSUDI_INVALID_INDEX，CSUDI_INVALID_INDEX代表该设备资源不存在或者播放器不需要此设备实现播放操作。
@note 创建Player时应杜绝传入冗余设备，如播放I帧时不得传入Audio
@note 如果播放器的类型是直播，那一定要指定demux ID,且音视频不能同时为-1，否则返回CSUDIPLAYER_ERROR_DEVICE_DISORDER
@note 如果此时该视频解码器正在进行I帧解码(由于CSUDIOSGDecodeImage接口的使用)，并且本函数需要马上用到该解码器，则该接口应进行等待直到I帧解码完成为止
@note 如果以直播形式打开Player且所用Demux未与任何Tuner连接,仍能创建成功，但将无法播放出任何数据
@note 如果以Inject等不需要Tuner的形式打开Player且所用Demux已经连接了Tuner，则返回CSUDIPLAYER_ERROR_DEVICE_BUSY
@see CSUDIOSGDecodeImage

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer);

/**
@brief 销毁一个播放器

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 销毁一个播放器，与CSUDIPLAYEROpen成对使用。
@note 一个播放器被销毁后，其相关资源需一并自行销毁(如该播放器关联的回调函数)。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERClose(CSUDI_HANDLE hPlayer);

/**
@brief 用户设置Audio/video/pcr/sub/tt节目信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] psStreamInfo 输入节目信息，具体结构参看CSUDIStreamInfo_S 数据结构。
@param[in] nStreamCnt 要设置的节目信息个数
@param[in] psPlaybackParam 文件回放/时移播放参数信息；该参数可以为CSUDI_NULL,表示进行直播
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 试图设置两个音视频信息时返回CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER,即不能同时设置两个音视频PID 
@note 供用户设置Audio/video/pcr/sub/tt节目信息.具体请参见CSUDIStreamInfo_S的定义 
@note 试图设置解码器不支持的音视频类型时返回CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER
@note 该接口设置的音频格式与输出模式无关，如当前所有Aout设备均为DECODER模式，但当前Player中的Audio设备
仅支持AC3音频的BYPASS模式，使用本接口设置一个AC3音频时，仍返回成功，但实际输出会没有声音。
@note 播放参数中不设置不需要的属性PID，如一个有AV的Player来播放广播时不传入视频PID
@note 该接口及相关接口无需进行PID标准级的判断，即0,0x10等被其他PSI占用的PID仍为合法的PID，实际若上述PID上无对应音视频数据时，无法播放成功即可。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam);

/**
@brief 用户得到Audio/video/pcr/sub/tt节目信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[out] psStreamInfo 保存节目信息的地址，具体结构参看CSUDIStreamInfo_S数据结构。
@param[in,out] pnStreamCnt 获取的节目信息个数.输入参数时用作psStreamInfo中所有元素个数，输出参数为psStreamInfo有效节目元素个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 
- 此接口在创建播放器后就可以使用，如果还没有设置节目，则返回nStreamCnt =0，且psStreamInfo的内容不能够被改变
- 使用此接口获取到的流信息与CSUDIPLAYERSetStream设置的信息相对应，传入的StreamInfo数组必须传足够大(建议>=5)，否则可能无法取到需要的信息

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetStream(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S * psStreamInfo,int * pnStreamCnt);

/**
@brief 获取回放参数信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[out] psPlaybackParam 保存回放信息，具体结构参看CSUDIPlaybackParam_S数据结构。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetPlaybackParam(CSUDI_HANDLE hPlayer, CSUDIPlaybackParam_S * psPlaybackParam);

/**
@brief 用户修改Audio/video/pcr/sub/tt节目信息

@param[in] hPlayer 播放器句柄，由CSUDIPLAYEROpen()获得
@param[in] psStreamInfo 待修改的节目信息，具体情况请参见CSUDIStreamInfo_S数据结构。
@param[in] nStreamCnt  修改的节目信息个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 试图设置两个音视频信息时返回CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER  ,即不能同时设置两个音视频PID  
@note 试图设置解码器不支持的音视频类型时返回CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER    
@note 修改节目信息会将前一次的设置全部覆盖，建议调用本接口前先通过CSUDIPLAYERGetPlaybackParam获取当前状态，再进行修改。
@note 修改后的动作于修改前的动作保持一致。如果修改前处于播放状态，则修改后也处于播放状态。
@note 该接口设置的音频格式与输出模式无关，如当前所有Aout设备均为DECODER模式，但当前Player中的Audio设备
仅支持AC3音频的BYPASS模式，使用本接口设置一个AC3音频时，仍返回成功，但实际输出会没有声音。
@todo 状态机

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERModifyStream(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S * psStreamInfo,int nStreamCnt);

/**
@brief 启动一个播放器的工作 

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 按照CSUDIPLAYERSetInputStream 等函数设置的流信息以及其他相关参数播放一个媒体流。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERStart(CSUDI_HANDLE hPlayer);

/**
@brief 停止一个播放器的工作

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 停止一个播放器的工作，与CSUDIPLAYERStart成对使用。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERStop(CSUDI_HANDLE hPlayer);

/**
@brief 暂停与hPlayer相关的一个播放器的工作

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 
      - 成功返回CSUDI_SUCCESS；
      - 失败则返回错误代码值；
      - 不支持此操作返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 暂停与hPlayer相关的一个播放器的工作；只有正在播放某一节目（不论是只有音频还是视频）时调用才有效。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERPause(CSUDI_HANDLE hPlayer);

/**
@brief 恢复暂停模式 

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 不支持此操作返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 恢复暂停模式;与CSUDIPLAYERPause成对使用。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERResume(CSUDI_HANDLE hPlayer);

/**
@brief 清除播放器中缓存的数据

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@return 
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 不支持此操作返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 本接口不改变Player状态，仅清除缓冲数据，若仍在播放状态，则新数据会重新开始播放

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERClear(CSUDI_HANDLE hPlayer);

/**
@brief 设置播放速率

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] eSpeed 要设置的速率模式,详见CSUDIPlayerSpeed_E定义
@return 
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 该接口对直播节目无效，返回CSUDI_ERROR_PLAYER_NOTSUPPORT（不支持此操作）
@note 该参数修改后立即生效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetSpeed(CSUDI_HANDLE  hPlayer, CSUDIPlayerSpeed_E eSpeed);

/**
@brief 得到播放速率

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[out] peSpeed 要得到的速率模式,详见CSUDIPlayerSpeed_E定义
@return 
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 该接口对直播节目无效，返回CSUDI_ERROR_PLAYER_NOTSUPPORT（不支持此操作）

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetSpeed(CSUDI_HANDLE  hPlayer, CSUDIPlayerSpeed_E *peSpeed);

/**
@brief 设置播放位置

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] nPosInSec 偏移量
@param[in] ePlayPosFlag 偏移的相对位置
@return 	
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 只有回放文件和Tshift播放可以设置播放位置，Tshift播放的起始位置为有效可播放的起始位置（具体解释见CSUDIPLAYERGetCurPosInSec的注释）

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSeek(CSUDI_HANDLE  hPlayer,const int nPosInSec, const CSUDIPlayPosition_E ePlayPosFlag);

/**
@brief 获取回放文件/Tshift播放的当前播放位置的绝对时长。

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[out] pnPosInSec  存放当前播放位置(单位:秒)
@return  
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 在非"回放文件/Tshift播放"的情况下,该接口的行为未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetCurPosInSec(CSUDI_HANDLE  hPlayer, int * pnPosInSec);

/**
@brief 获取当前正在播放本地文件的节目总时长

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[out] pnSeconds 节目总时长,单位秒
@return  
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
         
@note 本接口只针对UDI播放视频文件有效,其它实时流或驱动不支持的视频文件pdwSeconds为0，返回值为CSUDI_ERROR_PLAYER_NOTSUPPORT
      对于普通的TS文件，如果是走Inject注入流程，也可以不用返回节目时长

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetDuration(CSUDI_HANDLE  hPlayer, int *pnSeconds);

/**
@brief 获取当前正在播放(显示)的码流PTS信息。

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] eContentType 流类型 详见CSUDIContentType_E相关定义
@param[out] psPTS 用于存放当前正在播放(显示)的码流PTS值,该值33bit。
@return  
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
@note 这里的PCR作为STC

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetCurPTS(CSUDI_HANDLE  hPlayer, CSUDIContentType_E eContentType, CSUDI_UINT64 *psPTS);

/**
@brief 获取当前正在播放节目指定流类型的packet个数。

@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] eContentType 当前正在播放的流类型详见CSUDIContentType_E
@param[out] puPacketCnt 存放packet包个数,指单个PID类型的包个数
@return  
         - 成功返回CSUDI_SUCCESS；
         - 失败则返回错误代码值；
         - 不支持此操作则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
         
@note 本接口主要用于判断前端码流是否停播

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetPacketCount(CSUDI_HANDLE  hPlayer, CSUDIContentType_E eContentType, unsigned int * puPacketCnt);

/**
@brief 进入退出TrickMode

通知驱动层后续音视频可能会按非正常速度进入解码器。
常用于VOD播放时的快退快进等，这种情况下Tuner过来的数据为非正常速度，
平台需做好准备以防止过度阶段及播放阶段的马赛克。
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] bTrickMode 为CSUDI_TRUE表示进入TrickMode,为CSUDI_FALSE表示退出TrickMode
@return
- 成功返回CSUDI_SUCCESS；
- 失败则返回错误代码值；
- 已经在TrickMode时再次进入TrickMode或者已经在非TrickMode时再次退出TrickMode返回CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
@note 回放文件和Tshift播放由于控制流程在底层处理，因此不需要调用本接口。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERTrickMode(CSUDI_HANDLE hPlayer,CSUDI_BOOL bTrickMode);

/**
@brief 进入退出快速输出模式
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] bQuickOutputMode 为CSUDI_TRUE表示进入QuickOutputMode,为CSUDI_FALSE表示退出QuickOutputMode
@return
	- 成功返回CSUDI_SUCCESS；
	- 失败则返回CSUDI_FAILURE；
@note 进入快速输出模式，解码器需要做特殊处理
          例如: 1、关闭隔行算法
                2、包溢出时进行丢弃
                3、按解码顺序输出
@note 该接口主要用于对低延时需求特别高的应用场景
          例如: 1、永新云游戏，云视频业务
@note 该接口需要在播放器非运行状态调用
          例如: 1、open之后start之前
                2、stop之后close之前

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERQuickOutputMode(CSUDI_HANDLE hPlayer,CSUDI_BOOL bQuickOutputMode);

/**
@brief 设置player的音视频同步模式
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[in] eAVSyncMode 为需要设置的同步模式，见CSUDIPLAYERAVSyncMode_E
@return
	- 成功返回CSUDI_SUCCESS；
	- 失败则返回CSUDI_FAILURE；
@note 如果不使用该接口，那么默认是采用PCR同步模式

@note 该接口需要在播放器非运行状态调用
          例如: 1、open之后start之前
                2、stop之后close之前

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIPLAYERSetAVSyncMode(CSUDI_HANDLE hPlayer,CSUDIPLAYERAVSyncMode_E eAVSyncMode);

/**
@brief 获取player的音视频同步模式
@param[in] hPlayer 播放器句柄，由函数CSUDIPLAYEROpen()获得
@param[out] peAVSyncMode 为保存同步模式的指针，见CSUDIPLAYERAVSyncMode_E
@return
	- 成功返回CSUDI_SUCCESS；
	- 失败则返回CSUDI_FAILURE；

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIPLAYERGetAVSyncMode(CSUDI_HANDLE hPlayer,CSUDIPLAYERAVSyncMode_E *peAVSyncMode);


#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

