/**@defgroup CSSYSAV [CSSYSAV]同洲项目专用AV驱动接口
@brief 本模块主要定义了同洲项目专用平台无AV驱动接口
@version 0.1 2008/10/23 演示版本
@{
*/
#ifndef _CS_SYSAV_H_
#define _CS_SYSAV_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "cs_sysvideo.h"
#include "cs_sysaudio.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SMS_AUIDO_TYPE_UNKNOWN									(0) 	/**< 音频类型：未知类型*/
#define SMS_AUIDO_TYPE_NORMAL										(1)	  /**< 音频类型：普通,0x03/0x04*/
#define SMS_AUIDO_TYPE_AC3											(2)	  /**< 音频类型：AC3*/

#define SMS_AUIDO_TYPE_DTS        							(3) 	/**< 音频类型：DTS */
#define SMS_AUIDO_TYPE_MPEG1     								(4) 	/**< 音频类型：MPEG1*/
#define SMS_AUIDO_TYPE_MPEG2      							(5)		/**< 音频类型：MPEG2*/
#define SMS_AUIDO_TYPE_CDDA       							(6) 	/**< 音频类型：CDDA*/
#define SMS_AUIDO_TYPE_PCM       	 							(7) 	/**< 音频类型：PCM*/
#define SMS_AUIDO_TYPE_LPCM       							(8) 	/**< 音频类型：LPCM*/
#define SMS_AUIDO_TYPE_PINK_NOISE 							(9) 	/**< 音频类型：PINK_NOISE*/
#define SMS_AUIDO_TYPE_MP3        							(10) 	/**< 音频类型：MP3 */
#define SMS_AUIDO_TYPE_MLP        							(11) 	/**< 音频类型：MLP */
#define SMS_AUIDO_TYPE_BEEP_TONE  							(12) 	/**< 音频类型：EEP_TONE*/
#define SMS_AUIDO_TYPE_MPEG_AAC   							(13)	/**< 音频类型：Advanced audio coding. Part of MPEG-4 */
#define SMS_AUIDO_TYPE_NULL       							(14)
#define SMS_AUIDO_TYPE_MPEG_AAC_PLUS  					(15) 	/**< 音频类型：AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE) */
#define SMS_AUIDO_TYPE_MPEG_AAC_PLUS_ADTS			 	(16) 	/**< 音频类型：AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format) */
#define SMS_AUIDO_TYPE_MPEG_AAC_PLUS_LOAS 			(17) 	/**< 音频类型：AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream) */
#define SMS_AUIDO_TYPE_AC3_PLUS   							(18) 	/**< 音频类型：Dolby Digital Plus (AC3+ or DDP) audio */
#define SMS_AUIDO_TYPE_HE_AAC   								(19) 	/**< 需要添加说明*/
#define SMS_AUIDO_TYPE_RAW_AAC   								(20) 	/**< 需要添加说明*/
#define SMS_AUIDO_TYPE_DRA        							(21) 	/**< 音频类型：DRA */

#define SMS_VIDEO_TYPE_UNKNOWN									(0)		/**< 视频类型：未知类型*/

#define SMS_VIDEO_TYPE_MPEG2  									(0) 	/**< 视频类型：MPEG2*/
#define SMS_VIDEO_TYPE_H264    									(1) 	/**< 视频类型：H264*/
#define SMS_VIDEO_TYPE_MPEG4 										(2) 	/**< 视频类型：MPEG4*/
#define SMS_VIDEO_TYPE_MPEG1          					(3) 	/**< 视频类型：MPEG1*/
#define SMS_VIDEO_TYPE_H263            					(4) 	/**< 视频类型：H263*/
#define SMS_VIDEO_TYPE_NULL       							(500)	/**< 视频类型：未知类型*/

#define SMS_AUIDO_OUTPUTMODE_UNKNOWN						(0)		/**< 音频输出类型：未知*/
#define SMS_AUIDO_OUTPUTMODE_PCM								(1)		/**< 音频输出类型：PCM*/
#define SMS_AUIDO_OUTPUTMODE_DIGITAL						(2)		/**< 音频输出类型：Digital*/

#define SMS_VIDEO_OUTPUTMODE_UNKNOWN						(0)		/**< 视频输出类型：未知*/

#define SMS_SER_SWITCH_TYPE_UNKNOWN							(0)		/**< 换台时音视频的处理类型：未定义*/
#define SMS_SER_SWITCH_TYPE_SCREEN_BLACK				(1)		/**< 换台时音视频的处理类型：黑屏*/
#define SMS_SER_SWITCH_TYPE_SCREEN_IFRAME				(2)		/**< 换台时音视频的处理类型：静桢*/
#define SMS_SER_SWITCH_TYPE_SCREEN_FADE_IN_OUT	(3)		/**< 换台时音视频的处理类型：淡入淡出*/

typedef struct _SMSAVStreamType
{
	DWORD			m_dwAudioType:8;					/**< SMS_AUIDO_TYPE_UNKNOWN SMS_AUIDO_TYPE_NORMAL SMS_AUIDO_TYPE_AC3*/
	DWORD			m_dwVideoType:8;					/**< SMS_VIDEO_TYPE_UNKNOWN*/
	DWORD			m_dwAudioOutMode:8;				/**< SMS_AUIDO_OUTPUTMODE_UNKNOWN SMS_AUIDO_OUTPUTMODE_PCM SMS_AUIDO_TYPE_AC3*/
	DWORD			m_dwVideoOutMode:8;				/**< SMS_VIDEO_OUTPUTMODE_UNKNOWN*/

	DWORD			m_dwCode:8;								/**< 如果为1，则说明所在结果体类型为SMSAVStreamType；其他未定义*/
	DWORD			m_dwServiceSwitchType:8;	/**< 换台时音视频的处理类型:黑屏 静桢 淡入淡出*/
	DWORD					:16;									/**< 保留*/
}SMSAVStreamType;
/**@brief 以上定义用于描述音视频播放时描述的类型，主要用于函数 CSUSPSMSPlayStream*/
typedef enum _CSSYSAVSync_E
{
	EM_AV_SYNC_NONE = 0, 					/**< 没有设置同步*/
	EM_AV_SYNC_PCR,		 						/**< 音视频与PCR同步*/
	EM_AV_SYNC_VID,  							/**< 音视频与视频同步 */
	EM_AV_SYNC_AUD 								/**< 音视频与音频同步,暂不支持*/
}CSSYSAVSync_E;

/**
@brief 用户设置Audio/video/pcr节目信息

@param[in] hHandle 设备通道句柄，通过CSRMConnect()分配得到的设备句柄。
@param[in] pstAUDInfo 输入节目的Audio信息，具体结构参看CSSYSAUDInputInfo数据结构。
@param[in] pstVIDInfo 输入节目的Video信息，具体结构参看CSSYSVIDInputInfo数据结构。
@param[in] pstPCRInfo pstPCRInfo输入节目的PCR信息，参看3.2.1 PCR Input Info数据结构
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，默认值设为NULL。
@return  如果操作成功则返回TRUE，如果操作失败则返回FALSE
@note供用户设置Audio/video/pcr节目信息，包括流的类型，格式，pid等，如果不设置诸如视频类型，音频类型，则按照默认类型操作；
*/
BOOL CSSYSSMSetInputStream(HCSHANDLE hHandle, CSSYSAUDInputInfo *pstAUDInfo, 
			   CSSYSVIDInputInfo *pstVIDInfo,CSSYSPCRInputInfo* pstPCRInfo,HCSHANDLE hUserData );


/**
@brief 启动音视频的播放 

@param[in] hHandle 通过函数CSSYSChipInit（）返回得到
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，默认值设为NULL
@return  如果成功则返回为TRUE，如果失败则返回为FALSE
@note 按照CSSYSSMSetInputStream（）等设置函数设置音视频pid，以及音视频的相关特性参数启动音视频的播放；如果没有设置视频pid，则只有声音输出，反之只有视频输出
*/
BOOL CSSYSSMPlay(HCSHANDLE hHandle, HCSHANDLE hUserData);


/**
@brief 停止音视频的播放

@param[in] hHandle 通过函数CSSYSChipInit（）返回得到。
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，默认值设为NULL。
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@note 停止音视频的播放；与CSSYSSMPlay成对使用。
*/
BOOL CSSYSSMStop(HCSHANDLE hHandle, HCSHANDLE hUserData);


/**
@brief 暂停与hHandle相关的音视频的播放

@param[in] hHandle 通过函数CSSYSChipInit（）返回得到
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，默认值设为NULL。
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@note 暂停与hHandle相关的音视频的播放；只有正在播放某一节目（不论是只有音频还是视频）时调用才有效。
*/
BOOL CSSYSSMPause(HCSHANDLE hHandle, HCSHANDLE hUserData);



/**
@brief 恢复暂停模式 

@param[in] hHandle 通过函数CSSYSChipInit（）返回得到
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，默认值设为NULL。
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@note 恢复暂停模式;与CSSYSSMPause成对使用。
*/
BOOL CSSYSSMResume(HCSHANDLE hHandle, HCSHANDLE hUserData);

/**
@brief 进入快进,快退,定位模式

@param[in] hHandle 通过函数CSSYSChipInit（）返回得到
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，默认值设为NULL。
@return  如果成功则返回为TRUE，如果失败则返回为FALSE  
@note 进入快进,快退,定位模式。
*/
BOOL CSSYSSMTrickModeStart(HCSHANDLE hHandle,HCSHANDLE hUserData);


/**
@brief 退出快进,快退,定位模式

@param[in] hHandle 通过函数CSSYSChipInit（）返回得到
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，默认值设为NULL。
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@note 退出快进,快退,定位模式。
*/
BOOL CSSYSSMTrickModeStop(HCSHANDLE  hHandle, HCSHANDLE  hUserData);


/**
@brief ES流注入初始化

@param[in] hHandle 通过函数CSSYSChipInit（）返回得到 hHandle通过函数CSSYSChipInit（）返回得到
@param[in] pstESBufferInfo 注入的Buffer的相关的信息，例如音视频Buffer的地址和大小等。参看ES  Buffer Info 数据结构。 
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@note 终止ES的注入，并回收buffer。
*/
BOOL CSESInit(HCSHANDLE  hHandle, CSESBufferInfo* pstESBufferInfo);


/**
@brief 终止ES的注入，并回收buffer

@param[in] hHandle 通过函数CSSYSChipInit（）返回得到
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@note 终止ES的注入，并回收buffer。
*/
BOOL CSESTerm(HCSHANDLE hHandle);


/**
@brief 获得注入的AV ES流PTS信息 

@param[in] hHandle 设备通道句柄，通过CSRMConnect()分配得到的设备句柄
@param[out] syncInfo 返回的的包含 ES流的PTS数据。
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
*/
BOOL CSESGetCurrentPTSEx(HCSHANDLE hHandle, CSESSyncInfoEx* syncInfo);
/**
@brief  启动AV同步

@param[in] hPcrDmx - 解析PCR所用的Demux设备Handle， EM_AV_SYNC_PCR模式有效
@param[in] wPcrPid - PCR PID, EM_AV_SYNC_PCR模式有效
@param[in] hVid  - Video设备句柄
@param[in] hAud  - Audio设备句柄
@param[in] eSync  - 同步方式
@return  如果成功返回同步句柄，如果失败返回NULL。
*/
HCSHANDLE  CSSYSAVSyncStart (HCSHANDLE hPcrDmx,WORD wPcrPid, HCSHANDLE hVid, HCSHANDLE hAud,CSSYSAVSync_E eSync);

/**
@brief 停止同步，停止後hAvSync无效，同一个hAvSync只能Stop一次

@param[in]  hAvSync  - CSAVSyncStart返回的同步句柄
@return  如果成功返回同步句柄，如果失败返回NULL。
*/
BOOL  CSSYSAVSyncStop (HCSHANDLE hAvSync);

/**
@brief 置音视频同步模式，在下次播放时生效

@param[in] hHandle  -通过函数CSSYSChipInit（）返回得到。
@param[in] eMode  -同步模式
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@note 在下次播放时生效
*/
BOOL CSSYSAVSetSyncMode(HCSHANDLE hHandle, CSSYSSync_E eMode);


/**
@brief 获取音视频同步模式 

@param[in] hHandle -通过函数CSSYSChipInit（）返回得到。
@param[out] peMode -同步模式
@return  如果成功则返回为TRUE，如果失败则返回为FALSE  
*/

BOOL CSSYSAVGetSyncMode(HCSHANDLE hHandle, CSSYSSync_E* peMode);
/**
@brief 启动音视频解码器,对于SM 01.05.15 及其以后版本，hUserData被解释成 &SMSAVStreamType

@param[in] hDemux 设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的Demux设备索引号。
						对于单Demux设备，该参数无意义，直接赋0.
@param[in] wVideoPID  待解码流的视频PID，范围0-0x1ffe
@param[in] wAudioPID  待解码流的音频PID，范围0-0x1ffe
@param[in] wPCRPID  待解码流的PCR PID，范围0-0x1ffe
@param[in] hUserData  用户自定义数据， 目前USP对该参数的使用为指向SMSAVStreamType的指针。
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数只是启动解码器，音视频的打开关闭应该与此函数无关，要求此函数调用前后不改变音视频输出状态，
 当即使wVideoPID wAudioPID wPCRPID都为0仍然返回TRUE
*/
BOOL CSUSPSMSPlayStream( HCSHANDLE hDemux, WORD wVideoPID, WORD wAudioPID, WORD wPCRPID, HCSHANDLE hUserData );

/**
@brief 停止音视频解码器

@param[in] hDemux 设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] hUserData  用户自定义数据，目前USP对该参数的使用为指向SMSAVStreamType的指针
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数负责停止音视频解码器，音视频的打开关闭应该与此函数无关，要求此函数调用前后不改变音视频输出状态。
*/
BOOL CSUSPSMSStopPlay(HCSHANDLE hDemux, HCSHANDLE hUserData);

/**
@brief 打开音频输出

@param[in] hDemux   设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] hUserData  用户自定义数据，保留
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数只负责音频输出的打开，与解码器的启动关闭无关。
*/
BOOL CSUSPSMSEnableAudio( HCSHANDLE hDemux, HCSHANDLE hUserData );
/**
@brief 关闭音频输出

@param[in] hDemux  设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] hUserData  用户自定义数据，保留
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数只负责音频输出的关闭，与解码器的启动关闭无关.
*/
BOOL CSUSPSMSDisableAudio( HCSHANDLE hDemux, HCSHANDLE hUserData );

/**
@brief 打开视频输出

@param[in] hDemux   设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] hUserData  用户自定义数据，保留
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数只负责视频输出的打开，与解码器的启动关闭无关。
*/
BOOL CSUSPSMSEnableVideo( HCSHANDLE hDemux, HCSHANDLE hUserData );

/**
@brief 关闭视频输出

@关闭视频输出，该接口只提供给UPS 的SMS模块使用。
 如果当前系统模式为PVR模式，则直接返回TRUE，当前系统模式为PVR模式通过平台CSSYSAdvancedParamEx接口进行初始化设置，
 也可以通过CSSYSSetPvrSupport接口修改
@param[in] hDemux  设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] hUserData  用户自定义数据，保留
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数至负责视频输出的关闭，与解码器的启动关闭无关
*/
BOOL CSUSPSMSDisableVideo( HCSHANDLE hDemux, HCSHANDLE hUserData );

/**
@brief 以下5个函数用于设定音视频解码器Audio，Video PID,而不涉及音视频输出的打开和关闭
设定解码器的Audio PID
@该接口只提供给UPS 的SMS模块使用。如果当前系统模式为PVR模式，则直接返回TRUE，
 当前系统模式为PVR模式通过平台CSSYSAdvancedParamEx接口进行初始化设置，也可以通过CSSYSSetPvrSupport接口修改。
@param[in] hDemux  设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] dwAudioPID   音频PID
@param[in] hUserData  用户自定义数据，保留
@param[in] nType   Audio 类型
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数用于重新设定解码器的Audio PID，而不涉及音视频输出的打开和关闭。
 此函数的调用发生在解码器启动之后，发现需要改变音频PID的地方（例如PMT版本号变化）。
*/
BOOL CSUSPSMSSetAudioPID( HCSHANDLE hDemux, DWORD dwAudioPID, int nType, HCSHANDLE hUserData );
/**
@brief 设定解码器的Audio PID

@该接口只提供给USP 的SMS模块使用，如果当前系统模式为PVR模式，则直接返回TRUE，
 当前系统模式为PVR模式通过平台CSSYSAdvancedParamEx接口进行初始化设置，也可以通过CSSYSSetPvrSupport接口修改。
@param[in] hDemux  设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] wAudioPID   音频PID
@param[in] hUserData   用户自定义数据，保留
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数用于重新设定解码器的Audio PID，而不涉及音视频输出的打开和关闭。
 此函数的调用发生在解码器启动之后，发现需要更改AudioPID的地方（例如PMT版本号变化）。
*/
BOOL CSUSPSMSResetAudioPID( HCSHANDLE hDemux, WORD wAudioPID, HCSHANDLE hUserData );

/**
@brief 设定解码器的Video PID

@该接口只提供给UPS 的SMS模块使用，如果当前系统模式为PVR模式，则直接返回TRUE，
 当前系统模式为PVR模式通过平台CSSYSAdvancedParamEx接口进行初始化设置，也可以通过CSSYSSetPvrSupport接口修改。
@param[in] hDemux  设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] wVideoPID   视频PID
@param[in] hUserData   用户自定义数据，保留
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数用于重新设定解码器的Video PID，而不涉及音视频输出的打开和关闭。
 此函数的调用发生在解码器启动之后，发现需要改变视频PID的地方（例如PMT版本号变化）。
*/
BOOL CSUSPSMSResetVideoPID( HCSHANDLE hDemux, WORD wVideoPID, HCSHANDLE hUserData );

/**
@brief 设定解码器的PCR PID

@该接口只提供给UPS 的SMS模块使用。如果当前系统模式为PVR模式，则直接返回TRUE，
 当前系统模式为PVR模式通过平台CSSYSAdvancedParamEx接口进行初始化设置，也可以通过CSSYSSetPvrSupport接口修改。
@param[in] hDemux   设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] wPCRPID   PCR PID
@param[in] hUserData   用户自定义数据，保留
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数用于重新设定解码器的PCR PID，而不涉及音视频输出的打开和关闭。
 此函数的调用发生在解码器启动之后，发现需要改变PCR PID的地方（例如PMT版本号发生变化）。
*/
BOOL CSUSPSMSResetPCRPID(HCSHANDLE hDemux, WORD wPCRPID, HCSHANDLE hUserData );

/**
@brief 设定解码器的AC3 Audio PID

@该接口只提供给UPS 的SMS模块使用。如果当前系统模式为PVR模式，则直接返回TRUE，
当前系统模式为PVR模式通过平台CSSYSAdvancedParamEx接口进行初始化设置，也可以通过CSSYSSetPvrSupport接口修改。
@param[in] hDemux   设备句柄。该参数针对多Demux设备情况，直接对应底层可分配的
									Demux设备索引号。对于单Demux设备，该参数无意义，直接赋0.
@param[in] wAC3PID   AC3PID
@param[in] hUserData   用户自定义数据，保留
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
@此函数用于重新设定解码器的AC3 PID，而不涉及音视频输出的打开和关闭。
 此函数的调用发生在解码器启动之后，发现需要改变切换音频PID为AC3PID的地方（例如PMT搜索完成）。
*/
BOOL CSUSPSMSResetAC3PID( HCSHANDLE hDemux, WORD wAC3PID, HCSHANDLE hUserData );

/**
@brief 启动CA

@param[in] hDemux    需要添加说明
@param[in] nOriNetworkID   需要添加说明
@param[in] nTSID   需要添加说明
@param[in] nServiceID   需要添加说明
@param[in] nVideoPID   需要添加说明
@param[in] nAudioPID   需要添加说明
@param[in] hUserData   需要添加说明
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
*/
BOOL CSUSPSMSStartCA(HCSHANDLE hDemux, int nOriNetworkID, int nTSID, int nServiceID, int nVideoPID, int nAudioPID, HCSHANDLE hUserData );

/**
@brief 停止CA

@param[in] hDemux    需要添加说明
@param[in] nVideoPID   需要添加说明
@param[in] nAudioPID   需要添加说明
@param[in]  hUserData  需要添加说明
@return  如果成功则返回为TRUE，如果失败则返回为FALSE 
*/
BOOL CSUSPSMSStopCA( HCSHANDLE hDemux, int nVideoPID, int nAudioPID, HCSHANDLE hUserData );

/**
@brief 停止CA扩展接口,支持多路输出

@param[in] hDemux    需要添加说明
@param[in] nOriNetworkID   需要添加说明
@param[in] nTSID   需要添加说明
@param[in]  nServiceID  需要添加说明
@param[in]  nVideoPID  需要添加说明
@param[in]  nAudioPID  需要添加说明
@param[in]  hUserData   需要添加说明
@return  如果成功则返回为TRUE，如果失败则返回为FALSE
*/
BOOL CSUSPSMSStopCAEx( HCSHANDLE hDemux, int nOriNetworkID, int nTSID, int nServiceID, int nVideoPID, int nAudioPID, HCSHANDLE hUserData );
/**
@brief 抛出PMT原始数据

@param[in] hDemux    需要添加说明
@param[in] pbPMTData   需要添加说明
@param[in] nDataLength   需要添加说明
@param[in]  hUserData  需要添加说明
@return  如果成功则返回为TRUE，如果失败则返回为FALSE
*/
BOOL CSUSPSMSInjectPMT( HCSHANDLE hDemux, PBYTE pbPMTData, int nDataLength, HCSHANDLE hUserData );

/**
@brief 抛出CAT原始数据

@param[in] hDemux    需要添加说明
@param[in] pbCATData   需要添加说明
@param[in] nDataLength   需要添加说明
@param[in]  hUserData  需要添加说明
@return  如果成功则返回为TRUE，如果失败则返回为FALSE
*/
BOOL CSUSPSMSInjectCAT( HCSHANDLE hDemux, PBYTE pbCATData, int nDataLength, HCSHANDLE hUserData );


/**
@brief 抛出BAT原始数据

@param[in] hDemux 需要添加说明
@param[in] pbBATData 需要添加说明
@param[in] nDataLength   需要添加说明
@param[in] nSectionEventType: 		1: section available		2: section complete
@param[in] hUserData 需要添加说明
@return  如果成功则返回为TRUE，如果失败则返回为FALSE
*/
BOOL CSUSPSMSInjectBAT( HCSHANDLE hDemux, PBYTE pbBATData, int nDataLength, int nSectionEventType, HCSHANDLE hUserData );

/**
@brief 抛出NIT原始数据

@param[in] hDemux 需要添加说明
@param[in] pbNITData 需要添加说明
@param[in] nDataLength   需要添加说明
@param[in] nSectionEventType: 		1: section available		2: section complete
@param[in] hUserData 需要添加说明
@return  如果成功则返回为TRUE，如果失败则返回为FALSE
*/
BOOL CSUSPSMSInjectNIT( HCSHANDLE hDemux, PBYTE pbNITData, int nDataLength, int nSectionEventType, HCSHANDLE hUserData );

/**
@brief 抛出SDT原始数据

@param[in] hDemux 需要添加说明
@param[in] pbSDTData 需要添加说明
@param[in] nDataLength   需要添加说明
@param[in] nSectionEventType 1: section available		2: section complete
@param[in] hUserData 需要添加说明
@return  如果成功则返回为TRUE，如果失败则返回为FALSE
*/
BOOL CSUSPSMSInjectSDT( HCSHANDLE hDemux, PBYTE pbSDTData, int nDataLength, int nSectionEventType, HCSHANDLE hUserData );

/**
@brief get user defined pg_rate

@return 需要添加说明
*/
int CSUSPSMSGetUserPGRate(void);


typedef  void ( *CSPlatAvStandInit)(HCSHANDLE);

#if 0 /*removed CSAvStandInit*/
/*CSAvStandInit通过CSSYSSetCfg()传入CSPlatAvStandInit类型的接口，移植层调用
具体用法为:
在CSSYSChipBoot()之前调用
	CSSYSSetCfg(CSCFG_SET_CSAVSTANDINIT_API, CSDEVICE_IGNORE, CSAvStandInit, 0);
*/

BOOL CSAvStandInit(HCSHANDLE hUserData);

/********************************************************************************
接口名称： int CSAvStandGetVersion( char* pcVersionInfo, int nSize)
接口用途：获取此模块的版本信息
                          此函数移植层只提供代码，不链接到发布的库中去
参数说明：
*********************************************************************************/
int CSAvStandGetVersion( char* pcVersionInfo, int nSize);

#endif

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

