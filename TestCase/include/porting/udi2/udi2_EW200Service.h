/**@defgroup EW200S [EW200Only]EW200 Service 提供EW200专用的应用层服务接口，以及EW210和EW510要用到的部分认证相关接口
@brief EW200由于本身不包括DVB应用控制部分，许多工作需要应用层辅助完成，比如对高频头资源的占用，数据管理查询，节目播放等
本模块即定义此部分接口

@note 本模块接口与与Eastwin其它系列产品的部分UDI接口会有少量重复，请根据具体集成的产品决定使用哪个模块
@note 上层亦需要并根据具体使用的产品决定使用哪些接口，原则上，上层除了EW200的配置接口及专用模块，其它模块均不得调用本模块接口

@note EW200产品需实现本模块所有接口；EW210和EW510只需要实现CSUDIEW200SReadClientInfo和CSUDIEW200SWriteClientInfo，并且只需要支持CSUDIEW200SClientInfoType_E中EM_UDIEW200S_CARDID~EM_UDIEW200S_VOLUME项，主要用于认证
@note 本模块定义的接口更偏向应用层，建议由应用层实现

@version 2.0.0 2009/09/09 初稿
@{
*/

#ifndef _UDI2_EW200_SERVICE_H_
#define _UDI2_EW200_SERVICE_H_

#include "udi2_typedef.h"
#include "udi2_tuner.h"
#include "udi2_public.h"
#include "udi2_error.h"
#ifdef __cplusplus
extern "C" {
#endif

/**@brief EW200 Service相关错误代码值*/
typedef enum
{
	CSUDIEW200S_ERROR_BAD_PARAMETER = CSUDI_EW200S_ERROR_BASE, ///< 参数错误
	CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED,                   ///< 操作不支持
	CSUDIEW200S_ERROR_UNKNOWN_ERROR,		           ///< 一般错误
	CSUDIEW200S_ERROR_NO_MEMORY,			           ///< 无可用内存
	CSUDIEW200S_ERROR_NO_CALLBACK,				 ///< 没有相应的回调函数可以删除
	CSUDIEW200S_ERROR_ALREADY_ADDED,			///< 该回调已经添加过
	CSUDIEW200S_ERROR_TRICKMODE_STATE_ERROR   ///< TRICKMODE状态错误，仅用于已经在TrickMode时再次进入TrickMode或者已经在非TrickMode时再次退出TrickMode
}CSUDIEW200S_ERRORCODE_E;


#define CSUDIEW200S_VIDEOCOUNT_PER_PROG 	(30)    ///< 每个节目的视频数
#define CSUDIEW200S_AUDIOCOUNT_PER_PROG 	(30)	///< 每个节目的音频数
#define CSUDIEW200S_SUBTITLECOUNT_PER_PROG 	(8)	///< 每个节目的subtitile数
#define CSUDIEW200S_TELETEXTCOUNT_PER_PROG 	(1)	///< 每个节目的teletext数
#define CSUDIEW200S_SUBTITLE_PER_STREAM         (20)	///< 每个流的subtitile数
#define CSUDIEW200S_TELETEXT_PER_STREAM         (20)	///< 每个流的teletext数

/**
@brief 定义EW200 Service通用消息参数结构

具体参数根据消息类型确定,
在消息类型中会以:简称=功能(类型)的形式加以描述
除非特别说明,所有指针指向的内容在消息返回后不再有效.
*/
typedef struct
{
	void* pPara0;                         ///< 参数0,简称p0
	void* pPara1;                         ///< 参数1,简称p1
	void* pPara2;                         ///< 参数2,简称p2
	void* pPara3;                         ///< 参数3,简称p3
} CSUDIEW200SParam_S;

/**

@brief 定义EW200 Service消息通知回调函数

EW200 Service将通过该回调获取底层播放相关的一些状态及信息
@param[in] dwMsg 消息类型,可以取CSEWVODMsgType_E中的EM_EW_MSG_VOD_CONNECT_SUCCESS，EM_EW_MSG_VOD_CONNECT_FAILED，EM_EW_MSG_VOD_NO_PROGRAME，EM_EW_MSG_VOD_NOT_SUPPORT，EM_EW_MSG_VOD_FRONTTS_STOP,EM_EW_MSG_VOD_FRONTTS_RUN,
@param[in] psParam 消息参数,具体参数类型见消息类型定义
@param[in] pUserData 用户数据,等于回调注册时的相应参数
@return 成功处理该消息返回1,否则返回0
@note 除非特别说明,MMCP会尽快从回调函数中返回
*/
typedef int (*CSUDIEW200SCallback_F)(unsigned int dwMsg, CSUDIEW200SParam_S *psParam, void* pUserData);


/**@brief EW200 所需数据源参数结构*/
typedef struct
{
	CSUDI_UINT32 m_dwTunerId;		///< EW200使用的Tuner设备Id
	int m_nDemuxID;				///< EW200使用的Demux设备Id
	CSUDITunerSRCDeliver_U m_punDeliver;	///< EW200使用的频点
}CSUDIEW200SSourceParam_S;

/**
@brief 定义VOD等需要的终端提供的信息类型

各种信息所需参数详细描述及类型见具体枚举项
@note CITV认证相关的字段详细格式均由运营商指定,JSExtern的信息类型由网页、中间件及应用共同决定
@note 带有[w]标识的字段会进行CSUDIEW200SWriteClientInfo操作,带有[r]标识的字段会进行CSUDIEW200SReadClientInfo操作
*/
typedef enum
{
	//CITV认证相关的枚举值
	EM_UDIEW200S_CARDID,          ///< [r]智能卡卡号,char*
	EM_UDIEW200S_SN,              ///< [r]序列号,char*
	EM_UDIEW200S_PROVIDER,        ///< [r]终端制造厂商,char*
	EM_UDIEW200S_ALIAS,           ///< [r]终端别名,char*
	EM_UDIEW200S_MODULE,          ///< [r]终端型号,char*
	EM_UDIEW200S_LOCAL_IP,        ///< [r]本机IP地址,char*,读取时格式为"192.168.1.1"	
	EM_UDIEW200S_LOCAL_MAC,       ///< [r]本机Mac地址,char*,如"00AABBCCDDEE"
	EM_UDIEW200S_CITV_PSD,        ///< [r]CITV VOD用户密码,char*
	EM_UDIEW200S_CITV_USERID,     ///< [w][r]CITV VOD用户号,第一次获取传全0,通过认证后CITV3会反馈该值,请将其存储在ROM中,以备下次使用,char*
	EM_UDIEW200S_CITV_TOKEN,      ///< [w][r]CITV 令牌,第一次获取传全0,通过认证后CITV3会反馈该值,请将其存储在ROM中,以备下次使用,char*
	EM_UDIEW200S_CITV_SERVERIP,   ///< [r]CITV 认证服务器IP地址,char*
	EM_UDIEW200S_CITV_SERVERPORT, ///< [r]CITV 认证服务器端口号,int,如8080
	EM_UDIEW200S_CITV_VIDEOTYPE,  ///< [r]支持音视频解码格式MPEG-2标清:"MPEG-2-SD" ;MPEG-2高清:"MPEG-2-HD";H.264标清: "H.264-SD";H.264高清: "H.264-HD" ; 缺省为MPEG-2-SD;同一格式高清向下兼容标清，可以不用再声明;终端可解码格式，可运行多个，用“/”分隔。
	EM_UDIEW200S_CITV_INMUSICSHOW,///< [w][r]是否进入音秀模式,"TRUE":音秀模式 "FALSE":非音秀模式.EW200接收到"ITVRTSPURL"关键字时,进入音秀模式;接收到"ITVControl"关键字,值为"ITVStop"时退出音秀模式
	EM_UDIEW200S_CITV_BROWSERVER, ///< [r]浏览器的版本号，应用根据需求与前端协商
	EM_UDIEW200S_ISMUTE,          ///< [r][w]静音状态,CSUDI_BOOL,CSUDI_TRUE表示当前为静音状态,CSUDI_FALSE表示为非静音状态
	EM_UDIEW200S_VOLUME,          ///< [r][w]音量,int,[0,31],0为无声,31最大

	EM_UDIEW200S_AUDIOCHANNEL,    ///< [w]声道,CSUDIEW200SAUDChannelType_E
	EM_UDIEW200S_AUDIOPID,        ///< [r][w]音频pid,int,
	EM_UDIEW200S_VIDEOALPHA,      ///< [w] 窗口的透明度，int,[0,100],0为完全透明，100为不透明
	EM_UDIEW200S_BRIGHTNESS,      ///< [w]视频亮度，int,[0,100],0为全黑，100为亮度最高
	EM_UDIEW200S_CONTRAST,        ///< [w]视频对比度，int,[0,100],0为全黑，100为对比度最高
	EM_UDIEW200S_SATURATION,      ///< [w]视频饱和度，int,[0,100],0为全黑，100为饱和度最高
	EM_UDIEW200S_ASPECTRATIO,     ///< [w]输出宽高比,CSUDIEW200SAspectRatio_E
	EM_UDIEW200S_MATCHMETHOD,     ///< [r][w]窗口的pan scan等格式,CSUDIEW200SMatchMethod_E
	EM_UDIEW200S_VIDEOFORMAT,     ///< [w]视频输出制式，CSUDIEW200SVdisVideoFormat_E	
	EM_UDIEW200S_STOPMODE,        ///< [r][w]换台时视频的处理类型:CSUDIEW200SStopMode_E
	EM_UDIEW200S_PROGRAMEINFO ,    ///< [r] 节目信息,CSUDIEW200SProgrameInfo_S
	EM_UDIEW200S_MIDDLEWAREVER,  //中间件版本号最大长度32字节。
	EM_UDIEW200S_SOFTWAREVER,   //软件本本号最大长度16字节
	EM_UDIEW200S_TVCODE,         //电视号最大长度16字节。
	EM_UDIEW200S_RESOLUTION,	 //获取视频分辨率, int。0-EM_ITV_RESOLUTION_720_576, 1-EM_ITV_RESOLUTION_1280_720
	EW_UDIEW200S_LOGINKEY       //[r]加密因子,char *
}CSUDIEW200SClientInfoType_E;

/**@brief 换台时视频的处理类型 */
typedef enum 
{	
	EM_UDIEW200S_STOPMODE_BLACK,		///< 黑屏
	EM_UDIEW200S_STOPMODE_FREEZE,		///< 静帧	
	EM_UDIEW200S_STOPMODE_FIFO		///< 淡入淡出
}CSUDIEW200SStopMode_E;

/**@brief 声道类型 */
typedef enum 
{
  	EM_UDIEW200S_AUD_CHANNEL_STEREO,     ///< 立体声
  	EM_UDIEW200S_AUD_CHANNEL_LEFT,       ///< 左声道    
  	EM_UDIEW200S_AUD_CHANNEL_RIGHT,      ///< 右声道 
  	EM_UDIEW200S_AUD_CHANNEL_MIXED_MONO  ///< 混合声
}CSUDIEW200SAUDChannelType_E;	

/**@brief 窗口格式 */
typedef enum 
{
	EM_UDIEW200S_LETTER_BOX,	///< Letter Box
	EM_UDIEW200S_PAN_SCAN,	        ///< Pan Scan
	EM_UDIEW200S_COMBINED,	        ///< ComBined
	EM_UDIEW200S_IGNORE,		///< Ignore
	EM_UDIEW200S_NUM			///< Number, not use
}CSUDIEW200SMatchMethod_E;

/**@brief 视频类型 */
typedef enum 
{
	EM_UDIEW200S_VIDEO_FMT_NTSC = 0,          ///< 480i, NSTC-M for North America 
	EM_UDIEW200S_VIDEO_FMT_NTSC_J,            ///< 480i (Japan) 
	EM_UDIEW200S_VIDEO_FMT_PAL_B,             ///< Australia 
	EM_UDIEW200S_VIDEO_FMT_PAL_B1,            ///< Hungary
	EM_UDIEW200S_VIDEO_FMT_PAL_D,             ///< China 
	EM_UDIEW200S_VIDEO_FMT_PAL_D1,            ///< Poland 
	EM_UDIEW200S_VIDEO_FMT_PAL_G,             ///< Europe 
	EM_UDIEW200S_VIDEO_FMT_PAL_H,             ///< Europe 
	EM_UDIEW200S_VIDEO_FMT_PAL_K,             ///< Europe 
	EM_UDIEW200S_VIDEO_FMT_PAL_I,             ///< U.K. 
	EM_UDIEW200S_VIDEO_FMT_PAL_M,             ///< 525-lines (Brazil)
	EM_UDIEW200S_VIDEO_FMT_PAL_N,             ///< Jamaica, Uruguay 
	EM_UDIEW200S_VIDEO_FMT_PAL_NC,            ///< N combination (Argentina) 
	EM_UDIEW200S_VIDEO_FMT_SECAM,             ///< LDK/SECAM (France,Russia) 
	EM_UDIEW200S_VIDEO_FMT_1080i,               ///< HD 1080i 
	EM_UDIEW200S_VIDEO_FMT_1080p,             ///< HD 1080p 60/59.94Hz, SMPTE 274M-1998
	EM_UDIEW200S_VIDEO_FMT_720p,               ///< HD 720p 
	EM_UDIEW200S_VIDEO_FMT_480p,               ///< HD 480p 
	EM_UDIEW200S_VIDEO_FMT_1080i_50Hz,        ///< HD 1080i 50Hz, 1125 sample per line, SMPTE 274M 
	EM_UDIEW200S_VIDEO_FMT_1080p_24Hz,        ///< HD 1080p 24Hz, 2750 sample per line, SMPTE 274M-1998 
	EM_UDIEW200S_VIDEO_FMT_1080p_25Hz,        ///<HD 1080p 25Hz, 2640 sample per line, SMPTE 274M-1998 
	EM_UDIEW200S_VIDEO_FMT_1080p_30Hz,       ///< HD 1080p 30Hz, 2200 sample per line, SMPTE 274M-1998 
	EM_UDIEW200S_VIDEO_FMT_1250i_50Hz,       ///< HD 1250i 50Hz, another 1080i_50hz standard SMPTE 295M 
	EM_UDIEW200S_VIDEO_FMT_720p_24Hz,        ///< HD 720p 23.976/24Hz, 750 line, SMPTE 296M 
	EM_UDIEW200S_VIDEO_FMT_720p_50Hz,         ///< HD 720p 50Hz (Australia) 
	EM_UDIEW200S_VIDEO_FMT_576p_50Hz,         ///< HD 576p 50Hz (Australia)
	EM_UDIEW200S_VIDEO_FMT_CUSTOM_1440x240p_60Hz,  ///< 240p 60Hz 7411 custom format. 
	EM_UDIEW200S_VIDEO_FMT_CUSTOM_1440x288p_50Hz,  ///< 288p 50Hz 7411 custom format. 
	EM_UDIEW200S_VIDEO_FMT_CUSTOM_1366x768p,       ///< Custom 1366x768 mode 
	EM_UDIEW200S_VIDEO_FMT_CUSTOM_1366x768p_50Hz, ///< Custom 1366x768 50Hz mode 
	EM_UDIEW200S_VIDEO_FMT_DVI_640x480p,       ///< DVI Safe mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_800x600p,       ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1024x768p,     ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1280x768p,      ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1280x720p_50Hz, ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1280x720p,      ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1280x720p_ReducedBlank, ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_MaxCount            ///< Counter. Do not use! 
}CSUDIEW200SVdisVideoFormat_E;

/**@brief 窗口输出宽高比 */
typedef enum 
{
	EM_UDIEW200S_WIN_ASPECT_RATIO_4_3 = 0,   ///< 4：3 
	EM_UDIEW200S_WIN_ASPECT_RATIO_16_9       ///< 16：9 
}CSUDIEW200SAspectRatio_E;

/**@brief 视频参数 */
typedef struct 
{
	CSUDI_UINT16  wVideoPID;              ///< 视频PID
	CSUDI_UINT8   ucVideoFormat;        ///< 视频格式
}CSUDIEW200SVideoParams_S;

/**@brief 音频参数 */
typedef struct 
{
	CSUDI_UINT16  wAudioPID;                ///< 音频PID
	CSUDI_UINT8    ucAudioFormat;           ///< 音频格式
	CSUDI_UINT8    ucLanguageCode[4];       ///< 语言描述符,字符串Chinese、English等 。
}CSUDIEW200SAudioParams_S; 

/**@brief 字幕实体 */
typedef struct 
{
	CSUDI_UINT8     aucLanguage[3];         ///< ISO639 字符语言编码
	CSUDI_UINT8     ucSubtitleType;         ///< 字幕类型
	CSUDI_UINT16   wCompPageID;		///< 页标识符的组合
	CSUDI_UINT16   wAnciPageID;		///< 辅助页面标识
}CSUDIEW200SSubtitleEntity_S;

/**@brief 图文电视实体 */
typedef struct 
{
	CSUDI_UINT8    aucLanguage[3];         ///< ISO639 字符语言编码
	CSUDI_UINT8    ucTeletextType;	       ///< 图文电视类型
	CSUDI_UINT8    ucTeleMagNum;	       ///< 图文电视杂志号码
	CSUDI_UINT8    ucTelePageNum;          ///< 图文电视页码
}CSUDIEW200STeletextEntity_S;

/**@brief 字幕参数 */
typedef struct 
{
	CSUDI_UINT8    ucStreamType;           ///< 描述子类型
	CSUDI_UINT16    wSubtitlePID;          ///< subtitile PID
	CSUDI_UINT8    ucDescriptorCount;      ///< subtitile个数
	CSUDIEW200SSubtitleEntity_S sDescriptor[CSUDIEW200S_SUBTITLE_PER_STREAM]; ///< subtile描述
}CSUDIEW200SSubtitileParams_S;

/**@brief 图文电视参数 */
typedef struct 
{
	CSUDI_UINT8    ucStreamType;    ///< 描述子类型
	CSUDI_UINT16  wTeletextPID;      ///< 图文电视PID
	CSUDI_UINT8    ucDescriptorCount; ///< 图文电视个数
	CSUDIEW200STeletextEntity_S sDescriptor[CSUDIEW200S_TELETEXT_PER_STREAM];  ///< 图文电视描述
}CSUDIEW200STeletextParams_S;

/**@brief 节目信息 */
typedef struct 
{
	CSUDI_UINT16	wPCRPID;              ///< PCR PID
	CSUDI_UINT16	wVideoCount;          ///< 视频数目
	CSUDI_UINT16    wVideoIndex;          ///< 视频索引
	CSUDIEW200SVideoParams_S	asVideo[CSUDIEW200S_VIDEOCOUNT_PER_PROG]; ///< 一个节目中的所有视频信息
	CSUDI_UINT16	wAudioCount;     ///< 音频数目
	CSUDI_UINT16 wAudioIndex;        ///< 音频索引
	CSUDIEW200SAudioParams_S	asAudio[CSUDIEW200S_AUDIOCOUNT_PER_PROG];///< 一个节目中的所有音频信息
	CSUDI_UINT16	wSubtitleCount;  ///< subtitle数目
	CSUDI_UINT16 wSubtitleIndex;	///< subtitle索引
	CSUDIEW200SSubtitileParams_S asSubtitle[CSUDIEW200S_SUBTITLECOUNT_PER_PROG];///< 一个节目中的所有subtitle信息
	CSUDI_UINT16	wTeletextCount;  ///< 图文电视数目
	CSUDI_UINT16 wTeletextIndex;	 ///< 图文电视索引
	CSUDIEW200STeletextParams_S asTeletext[CSUDIEW200S_TELETEXTCOUNT_PER_PROG];///< 一个节目中的所有图文电视信息
}CSUDIEW200SProgrameInfo_S;


/**
@brief 获取终端信息接口

Eastwin200将通过该接口获取终端的序列号,智能卡号等信息,或获取网页中设置的信息
@param[in] eType 信息类型
@param[in] value eType类型对应的值
@param[in] length value值的长度
@return 返回实际信息的长度,出错返回-1
@note 需要设置的信息见CSUDIEW200SClientInfoType_E枚举中的[r]标识
@note 支持Coship对象、Enreach对象时需要实现该接口

@code
//参考实现(未考虑缓冲区溢出等异常)
//注意:以下两种情况，value表示的类型及含义略有不同，请按参考代码实现/使用即可。
if(eType == EM_UDIEW200S_AUDIOCHANNEL)
{
	//EM_UDIEW200S_AUDIOCHANNEL对应数据类型是int的
	//value为int的变量，其长度应该为sizeof(int)
	
	value = EM_UDIEW200S_AUD_CHANNEL_STEREO;
	CSUDIEW200SReadClientInfo(eType , (void*)&value, sizeof(int));

}
else if(eType == EM_UDIEW200S_SN)
{
	//EM_UDIEW200S_SN对应数据类型是CSUDI_UINT8数组
	//value一个指向CSUDI_UINT8数组，其长度为length
	//从value指向的buffer中获取对应内容
	value[32];
	CSUDIEW200SReadClientInfo(eType , (void*)value, sizeof(value));

}
@endcode
*/
int CSUDIEW200SReadClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length);

/**
@brief 设置终端信息接口

Eastwin200将通过该接口将一些用户信息设置到终端永久存储或者应用自己存储起来
@param[in] eType 信息类型
@param[in] value eType类型对应的值
@param[in] length value值的长度
@return 返回实际存储信息的长度,出错返回-1
@note 需要设置的信息见CSUDIEW200SClientInfoType_E枚举中的[w]标识
@note 支持Coship对象、Enreach对象时需要实现该接口
@note 参考实现同CSUDIEW200SReadClientInfo
*/
int CSUDIEW200SWriteClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length);

/**
@brief 通过DVB3要素查询数据源信息

包括获取该3要素对应信息时所需使用的TunerId,DemuxId,及频点
@param[in] nOrNetId org network id
@param[in] nTsId ts id
@param[in] nServiceId service id
@param[out] psSourceParam 用于存储输出的数据源信息
@return 如果成功返回CSUDI_SUCCESS, 失败返回错误代码
@note EW200通过该接口查询一个Service的对应的频点及高频头信息，若平台有多个高频头，这个接口的实现要特别小心，因为对应的数据可能在不同的高频头上，否则简单查询数据管理即可
*/
CSUDI_Error_Code CSUDIEW200SGetTuneParam(int nOrNetId,int nTsId,int nServiceId,CSUDIEW200SSourceParam_S * psSourceParam);

/**
@brief 获取IPQam域信息接口

常用语VOD模块
认证模块通过调用终端实现的该接口得到IPQam的域信息
- CITV3.x版本的VOD目前取参数指定Section的第4、5字节(即data[3],data[4])
@param[in] punDeliverList 存放信息所在频点列表数组
@param[in] nDeliverCount pDeliverList数组中的频点个数
@param[in] nPid IPQam域信息所在Pid
@param[in] ucTableId IPQam域信息所在TableId
@param[out] pIpQamDataBuffer 用于存放IPQam域信息的内存指针
@param[in] nBufferLength pIpQamDataBuffer的长度
@param[in] pvReserved 保留字段,目前必须为NULL
@return 成功获取返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSUDIEW200SGetIPQamDomainInfo(const CSUDITunerSRCDeliver_U* punDeliverList,
										int nDeliverCount,
										int nPid,
										unsigned char ucTableId,
										char* pIpQamDataBuffer,
										int nBufferLength,
										void* pvReserved
									   );
									   
/**
@brief 播放节目接口

根据制定URL的格式进行播放

@param[in] pcURL 播放URL
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@code 播放URL的格式有以下几种，应用可以根据不同的URL格式进行播放处理：
(1)dvb://OriginalNetworkId.TS_Id.ServiceId  具体参考《数字电视中间件技术规范》第3部分附录I的1.2.1小节。
		OriginalNetworkId: original_network_id
		TS_Id: transport_stream_id
		ServiceId: service_id
		例如: dvb://1.2.5(十六进制)
(2)dvbc://Frequency.SymbolRate.Modulation.ServiceId
		Frequency:频率.单位MHz
		SymbolRate:符号率，单位是Kbps
		Modulation:调制模式
		ServiceId:节目Id
		例如: deliver://490000000:6875:64:10(ServiceId为十六进制)
(3)avpid://pcrPid.emm.videoPid.audioPid.audio_ecm_pid.video_ecm_pid(十六进制)	
@endcode
*/
void CSUDIEW200SPlayStream(char* pcURL, int nPlayId);

/**
@brief 停止节目播放接口

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
*/
void CSUDIEW200SStopPlay(int nPlayId);

/**
@brief 设置视频播放窗口大小接口

@param[in] psRect 视频矩形大小,为NULL表示全屏播放
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
*/
void CSUDIEW200SSetPlayRect(CSUDIWinRect_S* psRect,int nPlayId);

/**
@brief 恢复暂停模式 

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数

@return 
- 成功返回CSUDI_SUCCESS；
- 失败返回其他
不支持此操作返回CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
@note 恢复暂停模式;与CSUDIEW200SPause成对使用。
*/
CSUDI_Error_Code CSUDIEW200SResume(int nPlayId);

/**
@brief 暂停与hPlayer相关的一个播放器的工作

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数

@return 
- 成功返回CSUDI_SUCCESS;
- 失败返回其他
不支持此操作返回CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
@note 暂停与hPlayer相关的一个播放器的工作；只有正在播放某一节目（不论是只有音频还是视频）时调用才有效。
*/
CSUDI_Error_Code CSUDIEW200SPause(int nPlayId);

/**
@brief 进入退出TrickMode

通知驱动层后续音视频可能会按非正常速度进入解码器。
常用于VOD播放时的快退快进等，这种情况下Tuner过来的数据为非正常速度，
平台需做好准备以防止过度阶段及播放阶段的马赛克。
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@param[in] bTrickMode 为CSUDI_TRUE表示进入TrickMode,为CSUDI_FALSE表示退出TrickMode
@return 
- 成功返回CSUDI_SUCCESS；
不支持此操作返回CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
已经处于此状态返回CSUDIEW200S_ERROR_TRICKMODE_STATE_ERROR
*/
CSUDI_Error_Code CSUDIEW200STrickMode(int nPlayId,CSUDI_BOOL bTrickMode);


/**
@brief 注册UDI层消息回调函数

注册后UDI层相关的消息会通过该接口通知到应用
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@param[in] pUserData 用户数据,会在回调中原样返回
@return 成功返回CSUDI_SUCCESS,失败返回错误代码见CSUDIEW200S_ERRORCODE_E定义
@note
- 回调函数的注册个数至少支持5个，如果应用注册回调个数超过所支持个数将返回CSUDIEW200S_ERROR_NO_MEMORY
- 一组nPlayId,fncallback,pUserdata共同决定一个回调，当两次注册它们完全一样时，第二次将返回CSUDIEW200S_ERROR_ALREADY_ADDED
*/
CSUDI_Error_Code CSUDIEW200SAddCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata);

 /**
@brief 删除UDI层回调函数的注册

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@param[in] fnTunerCallback 回调函数句柄
@param[in] pUserData 同CSUDIEW200SAddCallback中的pUserData
@return成功返回CSUDI_SUCCESS 失败错误返回见CSUDIEW200S_ERRORCODE_E定义
*/
CSUDI_Error_Code CSUDIEW200SDelCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata);

#ifdef __cplusplus
}
#endif

/** @} */

#endif   


