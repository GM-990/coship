#ifndef _WEBKIT_PLATFORM_H
#define _WEBKIT_PLATFORM_H

#ifndef QWEBKIT_EXPORT 
#define QWEBKIT_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif


// 返回1970-1-1 0:0:0 至今的秒数
typedef double (*getCurrentTime_F)(void);

// 返回时区偏移标准时间秒数
typedef double (*getUTCOffset_F)(void);

// 返回夏时令偏移时间秒数
typedef double (*getDSTOffset_F)(void);

// 返回开机到现在运行的毫秒数
typedef int (*getTickCount_F)(void);


// 浏览器打印接口
typedef void (*webkitPrint_F)(const char * pcModuleName, int nOutputLevel, const char *pcFormat, ...);

typedef void (*webkitNotifyPageSize_F)(int nWidth, int nHeight);

typedef void (*webkitNotifyContentSize_F)(int nWith, int nHeight);

/**
@brief  功能描述:		
	设置下载完成后的操作
@param[in] 
    下载完成的job的url
@return 		
	无
@exception
@note  
	无
*/
typedef void (*webkitFinishLoading_F)(char* pUrl);

/**
@brief  功能描述:		
	保存下载的数据
@param[in] 
    url连接
    pData下载完成的数据
    nLen要保存数据的长度
@return 		
	无
@exception
@note  
	无
*/
typedef void (*webkitSaveData_F)(char *purl, char *pData, int nLen);
typedef void (*webkitAddExtraMenu_F)(void* menu, void *page, void *parentWidget);
typedef void (*webkitDownloadSetting_F)();

typedef struct _WebKitSystemInfo_S{
	getCurrentTime_F m_fnGetCurrentTime;
	getUTCOffset_F m_fnGetUTCOffset;
	getDSTOffset_F m_fnGetDstOffset;
	getTickCount_F m_fnGetTickCount;
	webkitPrint_F m_fnPrint;
}webkitSystemInfo_S;


typedef enum {
	MPUnknownMediaStatus,
	MPPlayingState,
	MPNoMedia,
	MPLoadingMedia,
	MPLoadedMedia,
	MPBufferingMedia,
	MPStalledMedia,
	MPBufferedMedia,
	MPEndOfMedia,
	MPInvalidMedia
}MPStatus;

typedef enum {
	MPNoError,
	MPFormatError
}MPNetworkStatus;

typedef void *(*HTML5createMP_F)();
typedef void (*HTML5destroyMP_F)(void *mpHandle);
typedef bool(*HTML5getMpSupportMimeTypes_F)(char **mime, int maxTypesNum, int maxTypeByteLen);
typedef bool(*HTML5hasSupportTypes_F)(const char* mime, const char* codec);
typedef bool(*HTML5isVideoAvailable_F)(void *mpHandle);
typedef void (*HTML5setMedia_F)(void *mpHandle, char *url);
typedef void (*HTML5setMuted_F)(void *mpHandle, bool mute);
typedef void (*HTML5setVolume_F)(void *mpHandle, int vol);
typedef bool (*HTML5isMuted_F)(void *mpHandle);
typedef int (*HTML5getVolume_F)(void *mpHandle);
typedef void (*HTML5mpPlay_F)(void *mpHandle);
typedef void (*HTML5mpPause_F)(void *mpHandle);
typedef void (*HTML5mpsetPosition_F)(void *mpHandle, float pos);
typedef void (*HTML5mpstop_F)(void *mpHandle);
typedef void (*HTML5setPlaybackRate_F)(void *mpHandle, float rate);
typedef int (*HTML5totalBytes_F)(void *mpHandle);
typedef int (*HTML5bytesLoaded_F)(void *mpHandle);
typedef float (*HTML5mpCurrentTime_F)(void *mpHandle); // 单位为s
typedef float (*HTML5duration_F)(void *mpHandle); // 单位为s
typedef float (*HTML5maxTimeSeekAble_F)(void *mpHandle); // 单位为s
typedef MPStatus (*HTML5mpStatus_F)(void *mpHandle);
typedef MPNetworkStatus (*HTML5mpErr_F)(void *mpHandle);
typedef void (*HTML5setVideoPosition_F)(void *mpHandle, int x, int y, int w, int h);
typedef bool (*HTML5mpPaused_F)(void* mpHandle);

typedef struct _webKitMediaPlayerFuns {
	HTML5createMP_F m_createMP;
	HTML5destroyMP_F m_destroyMP;
	HTML5getMpSupportMimeTypes_F m_supportMimeTypes;
	HTML5hasSupportTypes_F m_hasSupportTypes;
	HTML5isVideoAvailable_F m_isVideoAvailable;
	HTML5setMedia_F m_setMedia;
	HTML5setMuted_F m_setMuted;
	HTML5setVolume_F m_setVolume;
	HTML5mpPlay_F m_play;
	HTML5mpPause_F m_pause;
	HTML5mpstop_F m_stop;
	HTML5mpsetPosition_F m_setPosition;
	HTML5setPlaybackRate_F m_setPlaybackRate;
	HTML5totalBytes_F m_totalBytes;
	HTML5bytesLoaded_F m_bytesLoaded;
	HTML5mpCurrentTime_F m_currentTime;
	HTML5duration_F m_duration;
	HTML5maxTimeSeekAble_F m_maxSeekAble;
	HTML5mpStatus_F m_status;
	HTML5mpErr_F m_err;
	HTML5isMuted_F m_isMuted;
	HTML5getVolume_F m_getVolume;
	HTML5setVideoPosition_F m_setVideoPosition;
	HTML5mpPaused_F m_paused;
}webKitMediaPlayerFuns;


QWEBKIT_EXPORT bool CSBrwRegMediaPlayerFus(webKitMediaPlayerFuns *mpfuns);
QWEBKIT_EXPORT void CSBrwRegSystemInfo(webkitSystemInfo_S *sysInfo);
QWEBKIT_EXPORT void CSBrwRegNotifyPageSize(webkitNotifyPageSize_F notifyPageSize);
QWEBKIT_EXPORT void CSBrwRegNotifyContentSize(webkitNotifyContentSize_F notifyContentSize);
QWEBKIT_EXPORT void CSBrwRegDownLoad(webkitFinishLoading_F fnDownload, webkitSaveData_F fnSaveData);
QWEBKIT_EXPORT void CSBrwRegExtraMenu(webkitAddExtraMenu_F fnAddExtraMenu);
QWEBKIT_EXPORT void CSBrwRegDownloadSetting(webkitDownloadSetting_F fnDownLoadSetting);
QWEBKIT_EXPORT void CSBrwSetSupportException(bool bEnable);
QWEBKIT_EXPORT void setDumpMemory();
QWEBKIT_EXPORT int getExternUsedMemorySize();

QWEBKIT_EXPORT void CSBrwSetDefaultBoxSize(int size); 
QWEBKIT_EXPORT void CSBrwSetSupportSmartEpg(bool bEnable);

#ifdef __cplusplus
}
#endif


#endif


