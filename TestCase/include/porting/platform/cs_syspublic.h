#ifndef _CS_SYSPUBLIC_H_
#define _CS_SYSPUBLIC_H_

#include "cs_typedef.h"
#include "cs_assert.h"
#include "cs_device.h"
#include "udi_public.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 同步方式*/
typedef enum _CSSYSSync_E
{
	EM_SYNC_NONE = 0, 	/**< 没有设置同步*/
	EM_SYNC_PCR, 		/**< 音视频与PCR同步*/
	EM_SYNC_VIDPTS,  	/**< 音视频与视频同步 */
	EM_SYNC_AUDPTS	    /**< 音视频与音频同步,暂不支持*/
}CSSYSSync_E;

/**@brief 需要添加说明*/
typedef enum
{
	DATA_VIDEO_OVERFLOW,	/**<  视频缓冲区上溢*/
	DATA_VIDEO_UNDERFLOW,	/**<  视频缓冲区下溢*/
	DATA_AUDIO_OVERFLOW,	/**<  音频缓冲区上溢*/
	DATA_AUDIO_UNDERFLOW	/**<  音频缓冲区下溢*/
}CSESDataInjectCallbackType;

/**@brief 需要添加说明*/
typedef struct _CSESBufferInfo
{
	PBYTE   pucVideoBuffer;			/**<  视频基本流缓冲区地址*/
	int     nVideoBufferSize;		/**<  视频基本流缓冲区长度*/
	int     nVideoPreInjectTime;	/**<  需要添加说明*/
	int     nVideoPreInjectSize;	/**<  需要添加说明*/

	PBYTE   pucAudioBuffer;			/**<  音频基本流缓冲区地址*/
	int     nAudioBufferSize;		/**<  音频基本流缓冲区长度*/
	int     nAudioPreInjectTime;	/**<  需要添加说明*/
	int     nAudioPreInjectSize;	/**<  需要添加说明*/
}CSESBufferInfo;

/**@brief 用来在32位机上表示64位数值的结构*/
typedef struct _U64_clock_t
{
	DWORD  dwHigh;		/**<  64位clock中的高32位*/
	DWORD  dwLow;		/**<  64位clock中的低32位*/
}U64CLock_t;

/**@brief 需要添加说明*/
typedef struct _CSESSyncInfoEx
{
	BOOL        isSync; 		/**<  音视频是否同步*/
	U64CLock_t  pts;			/**< 当前PTS(节目时间戳)*/
	U64CLock_t  video_pts; 		/**< video PTS*/
	U64CLock_t  audio_pts; 		/**< audio PTS*/
}CSESSyncInfoEx;

/**@brief 需要添加说明*/
typedef enum CSSYS_Memory_Type_e
{
    CSSYS_MEM_TYPE_SYSPART, /**< 对应于system partition*/
    CSSYS_MEM_TYPE_NONCACHE,/**< 对应于noncache partition*/
    CSSYS_MEM_TYPE_AVMEM, 	/**< 对应于av memory*/
    CSSYS_MEM_TYPE_OS,		/**< 操作系统使用的堆空间，一般malloc在此空间中分配*/
    CSSYS_MEM_TYPE_DRIVER,	/**< 片内/片外驱动所使用的堆空间*/
    CSSYS_MEM_TYPE_AVMEM_USER	/**< 供应用使用的AVMEM分区，Graphic，IFrame,GUI Buffer等等 */
} CSSYSMemoryType;

typedef struct _CSSYSMemoryRange
{
	CSSYSMemoryType m_eMemoryType;/**< 内存区域类型*/
	void* m_pStartAddr;/**< 内存区域起始地址*/
	DWORD m_dwSize;/**< 内存区域大小*/
}CSSYSMemoryRange;

/**@brief 需要添加说明*/
typedef enum _CSSYSCfg_E
{
	CSCFG_TUNER_TSIN, 		/**< 设置TUNER对应的TSIN, CSSYSChipInit之前设置，dwDevice为CSTUNERDeviceType，dwValue为CSTSINDeviceType,dwRserved目前必须为0 */
	CSCFG_TSIN_DMX,		/**< 设置TSIN对应的DMX, CSSYSChipInit之前设置，dwDevice为CSTSINDeviceType，dwValue为CSDMXDeviceType,dwRserved目前必须为0 */
	CSCFG_HDMI_I2C,		/**< 设置HDMI对应的I2C, CSSYSChipInit之前设置，dwDevice为CSHDMIDeviceType，dwValue为CSI2CDeviceType，dwRserved目前必须为0 */
	CSCFG_HDMI_PIO,		/**< 设置HDMI对应的PIO索引, CSSYSChipInit之前设置，dwDevice为CSHDMIDeviceType，dwValue为GPIO从0开始的index，dwRserved目前必须为0 */
	CSCFG_TUNER_MODE, 	/**< Tuner串行并行, CSSYSChipInit之前设置，dwDevice为CSTUNERDeviceType，dwValue为CSSYSTsMode_E，dwRserved目前必须为0 */
	CSCFG_TUNER_TYPE, 	 	/**< TUNER_C, TUNER_S, TUNER_T, CSSYSChipInit之前设置，dwDevice为CSTUNERDeviceType，dwValue为CSSYSTunerType_E，dwRserved目前必须为0 */
	CSCFG_SD_TIMING_MODE, 	/**< 设置平台初始化时的标清分辩率, CSSYSChipInit之前设置，dwDevice=> CSDEVICE_IGNORE, dwValue为CSVIDTimingMode，dwRserved目前必须为0 */
	CSCFG_HD_TIMING_MODE,		/**< 设置平台初始化时的高清分辩率, CSSYSChipInit之前设置，dwDevice=> CSDEVICE_IGNORE, dwValue为CSVIDTimingMode，dwRserved目前必须为0 */
	CSCFG_CSOS_USEPOSIX,		/**< 设置平台是否使用Posix,目前只对st7100平台有效，dwDevice=> CSDEVICE_IGNORE, dwValue为TRUE/FALSE，dwRserved目前必须为0 */
	CSCFG_HDMI_HDCP,                     /**<  设置平台初始化时是否打开HDCP 功能，dwDevice为CSHDMIDeviceType，dwValue为CSSYSHdcpMode_E，dwRserved目前必须为0 */
	CSCFG_OSD_ADJUST_BOUNDS, /**<  缩小OSD，st7100平台特殊需求，dwDevice => CSLAYERDeviceType, dwValue TRUE or FALSE, dwRserved 0*/
	CSCFG_MEM_PROFILE,   		/**< 设置内存配置，对应于数据结构CSSYSMemoryRange,目前只对st71xx平台有效，dwDevice=> CSDEVICE_IGNORE, dwValue为CSSYSMemoryRange结构指针，dwRserved目前必须为MemoryRange个数 */
	CSCFG_VID_OUTPUT_MODE, 	/**< 设置video output device的模式,dwDevice=> CSVIDOutputDevice, dwValue=> CSVIDOutputMode, dwRserved 0*/
	CSCFG_VID_REINIT_WHEN_PLAY, 		/**< 切台时针对特定stream type是否强制重新初始化video，dwDevice=>CSVIDDeviceType, dwValue=>CSVIDStreamType,dwRserved=>TRUE or FALSE, 目前仅7100平台有效 */
	CSCFG_OSD_REFRESH_ANTIALIASING, 	/**< 设置OSD是否支持去刷新锯齿,dwDevice=> CSDEVICE_IGNORE, dwValue=> TREU or FALSE, dwRserved 0, 系统默认是不支持，目前仅7100平台有效*/
	CSCFG_SET_CSAVSTANDINIT_API, 	/**< 设置原型为typedef  void ( *CSPlatAvStandInit)(HCSHANDLE)的API函数，目前仅7100平台有效*/
	CSCFG_VID_DECODE_FRAME_NUM, /**<  设置视频解码帧数,目前仅7100平台有效,dwDevice=>CSDEVICE_IGNORE,dwValue=>CSVIDStreamType目前只支持H264,dwRserved=>解码帧buffer数量,一般为6~8*/
	CSCFG_POWER_CTL_GPIO, /**< 设置待机时对指定gpio的控制--强制拉高、拉低或者不操作,目前仅Hisi3110e平台有效,dwDevice为要操作的gpio索引号，dwRserved为FALSE时表示强制不对该gpio进行操作(如果正常待机流程有对该gpio的操作)，此时dwValue无效，dwRserved为TRUE时表示待机时强制操作gpio，dwValue指定要进行的操作，0为拉低，1则拉高>*/
	CSCFG_TS_INJECT_SUPPORT, /**< 设置是否支持Ts注入功能，dwDevice=> CSDEVICE_IGNORE, dwValue为 TRUE or FALSE,dwRserved目前必须为0 */
	CSCFG_STORAGE_HDD_WA, /**< 设置是否打开对特殊移动硬盘的workaround，仅对st71xx平台有效, dwDevice=> CSDEVICE_IGNORE, dwValue为 TRUE or FALSE,TRUE为enable，dwRserved目前必须为0  */
	CSCFG_AUD_AUTOVOLUME, /**< 设置是否打开音量补偿，仅对st5197 A37平台有效, dwDevice为CSAUDDeviceType， dwValue为 TRUE or FALSE,TRUE为enable，dwRserved目前必须为0  */
	CSCFG_TSIN_SYNC_MODE/**< 设置PTI 模块接收TS 流的模式，DEMO传至DEMUX模块的包格式分为异步模式和同步模式两种，dwDevice=> CSTSINDeviceType, dwValue为 CSSYS_TS_PARALLEL 或者 CSSYS_TS_SERIAL,dwRserved目前必须为0 **/
}CSSYSCfg_E;

/**@brief 需要添加说明*/
typedef enum _CSSYSHdcpMode_E
{
        CSSYS_HDCP_OPEN,		/**< 需要添加说明*/
        CSSYS_HDCP_CLOSE		/**< 需要添加说明*/
}CSSYSHdcpMode_E;

/**@brief 需要添加说明*/
typedef enum _CSSYSTunerType_E
{
	CSSYS_TUNER_C, 	/**< 有线*/
	CSSYS_TUNER_S, 	/**< 卫星*/
	CSSYS_TUNER_T  	/**< 地面*/
}CSSYSTunerType_E;

/**@brief 用于注册音视频事件的注册项*/
typedef struct  _CSSYSEventItem
{
	int 		m_nNormalEvent;	    /**< 正常事件,如CSSYSEVENT_SIGNAL_LOCK*/
	int 		m_nAbnormalEvent;	/**< 异常时间,如CSSYSEVENT_SIGNAL_LOST*/
	BOOL 	    m_bServiceRelated;	/**< 事件是否Service相关*/
}CSSYSEventItem;

/**@brief 需要添加说明*/
typedef enum _CSSYSTsMode_E
{
	CSSYS_TS_PARALLEL,		/**< 需要添加说明*/
	CSSYS_TS_SERIAL		/**< 需要添加说明*/
}CSSYSTsMode_E;

/**@brief 需要添加说明*/
typedef enum _CSSYSEventMask
{
	CSSYSEVENT_MASK_ACTIVE = 0x01, 			/**< 表示该事件是否为Active*/
	CSSYSEVENT_MASK_SERVICE_RELATED = 0x02 	/**< 表示该事件是否为节目相关,即如果相关则执行换台操着后就无效了*/
}CSSYSEventMask;

/**@brief audio和video驱动中的基本流注入功能用到的Callback原型*/
typedef void (* CSESDataInjectCallback)(CSESDataInjectCallbackType enmType, PVOID pvParam );

#ifdef  __cplusplus
}
#endif

#endif


