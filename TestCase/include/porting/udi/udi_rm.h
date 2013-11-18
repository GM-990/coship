/**@defgroup RM resource manager 设备管理
@brief 设备管理

提供基本的设备的功能查询、连接等功能。\n
管理各设备之间的连接情况，方便中间层及应用层模块直接按业务关注设备的使用，而无需关注具体的设备及设备间的共享\n
便于上层对各模块使用设备进行管理，避免多个模块同时使用造成的冲突
@version 0.2 2008/10/28 添加详细描述，分离剩余的初始化接口
@version 0.1 2008/10/21 初始版本
@{
*/

#ifndef _UDI_RM_H_
#define _UDI_RM_H_
#include "udi_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**@brief Demux设备支持直播*/
#define   DEMUX_SUPPORT_LIVE   				 (0x01)

/**@brief Demux设备支持录制*/
#define   DEMUX_SUPPORT_REC  				 (0x02)

/**@brief Demux设备支持回放*/
#define   DEMUX_SUPPORT_PLAYBACK   			 (0x04)	

typedef DWORD HPATH;

/**@brief 支持的设备类型枚举*/
typedef enum
{
	EM_RESOURCE_TUNER = 0x10,       /**< 高频头*/
	EM_RESOURCE_DEMUX,		        /**< 解复用器*/
	EM_RESOURCE_VIDEO,              /**< 视频解码器*/
	EM_RESOURCE_AUDIO,              /**< 音频解码器*/
	EM_RESOURCE_VIDLAYER,		    /**< 视频输出层*/
	EM_RESOURCE_LAN,		        /**< 网卡*/
	EM_RESOURCE_UNDEF
}CSRMResource_E;  

/**@brief 设备标识结构，可唯一标识一个设备*/
typedef struct _RM_Dev_Req_
{
      CSRMResource_E m_eResType;    /**< 设备类型*/
      int            m_nIndex;   	/**< 设备索引，从0开始递增*/
}CSRMDeviceReq_S;

/**@brief 高频头类型枚举*/
typedef enum
{
	EM_TUNER_CAB,			  /**< 有线高频头*/
	EM_TUNER_SAT,			  /**< 卫星高频头*/
	EM_TUNER_SAT2,     		  /**< 卫星S2高频头*/
	EM_TUNER_TER,			  /**< 地面高频头*/
	EM_TUNER_UNDEF
}CSRMTunerType_E;

/**@brief 高频头配置信息*/
typedef  struct  _Tuner_config
{  
    CSRMTunerType_E 	m_emTunerType;	/**< 高频头类型*/
}CSRMTunerConfig_S;

/**@brief Demux支持通道个数枚举，具体指示的通道类型根据具体调用不同*/
typedef enum
{
	EM_DEMUX_CHANNELCOUNT_0 = 0x0, 		/**< 0个通道*/
	EM_DEMUX_CHANNELCOUNT_1 = 0x1, 		/**< 1个通道*/
	EM_DEMUX_CHANNELCOUNT_N = 0x7fffffff 	/**< 未知个通道*/
}CSRMDemuxChannel_E;

/**@brief Demux配置信息*/
typedef   struct  _Demux_config
{ 
	int				m_nSupportType;                     /**< DEMUX_SUPPORT_LIVE|DEMUX_SUPPORT_REC|DEMUX_SUPPORT_PLAYBACK的组合*/
	CSRMDemuxChannel_E		m_emLiveChannel;            /**< Demux支持电视直播通道的个数，也即：demux可以连多少个decoder(A+V算一个)*/
	CSRMDemuxChannel_E		m_emRecordChannel;          /**< Demux支持录制通道的个数，也即：demux可以输出多少路ts流[到HDD]*/
	CSRMDemuxChannel_E		m_emPlaybackChannel;        /**< Demux支持电视回放的个数，也即：demux可以接收多少路ts流的注入[From FILE or IP]*/
    DWORD	 	        	m_nTunerLinkMark;	        /**< 该Demux可以连接Tuner的记录，
        								                    如果该Demux可以链接到第N个Tuner,则将m_nTunerMark的对应第N位置1,不能链接则该N位置0。
        								                    如果该Demux不能链接到任何Tuner，该数所有位全为0。*/
}CSRMDemuxConfig_S;

/**@brief 视频解码器类型枚举*/
typedef  enum    _VID_Dec_Type_
{
	EM_DECODER_OUTPUT_HD,       /**< 视频解码器仅支持高清流解码输出*/
	EM_DECODER_OUTPUT_SD,       /**< 视频解码器仅支持标清流解码输出*/
   	EM_DECODER_OUTPUT_HD_SD,    /**< 视频解码器支持高、标清流解码输出*/
	EM_DECODER_OUTPUT_UNDEF
} CSRMVIDDecType_E;

/**@brief 视频解码器配置信息*/
typedef  struct  _video_config
{  
    CSRMVIDDecType_E 	m_emDecoderType;  /**< 视频解码器类型*/
}CSRMVideoConfig_S;

/**@brief 视频输出层类型枚举*/
typedef  enum    VID_Layer_Type_
{
	EM_LAYTER_HD,       /**< 视频输出层支持高清(HD)格式输入（如:1024×768等）*/
	EM_LAYTER_SD,       /**< 视频输出层支持标清(SD)格式输入 （如：720×576等）*/
	EM_LAYTER_UNDEF
} CSRMVIDLayerTpye_E;

/**@brief 视频输出层配置信息*/
typedef  struct  _video_Layer_config
{  
    CSRMVIDLayerTpye_E 		m_emVIDLayer;	/**< 视频输出层类型*/
}CSRMVIDLayerConfig_S;

/**
@brief 申请指定的设备

一个设备在释放前，只能申请一次
@param[in] stDevice 要申请的设备的标识
@param[out] phDevice 设备句柄
@return 成功返回TRUE;失败返回FALSE
@note 如果某个平台有n个高频头，则其索引必须是0,1...n-1
*/
BOOL CSRMAlloc(const CSRMDeviceReq_S  stDevice, HCSHANDLE* const phDevice);

/**
@brief 释放分配的设备句柄

一个已被申请的设备，在释放后可被再次申请
@param[in] phDevice 要释放的设备句柄
@return 成功返回TRUE;失败返回FALSE
@note 本函数释放由CSRMAlloc分配的设备句柄，释放成功后，该Handle不能用来调用该设备的任何功能函数。
上层如果要使用设备，必须重新调用CSRMAlloc获得设备句柄。
*/
BOOL CSRMFree(const HCSHANDLE  phDevice);
 
/**
@brief 将多个设备组成一个设备链

该设备链可作为统一的句柄，用来操作设备链上的设备\n
例如将一个Tuner一个Demux一组Decoder连接起来，形成一个path，
要使用这些设备时使用这个path作为标示参数，传给相关接口\n
@param[in] hDevices 要组成设备链的设备句柄数组,这些设备句柄均为CSRMAlloc的输出
@param[in] nDeviceNums 设备数组中设备的数量
@return 设备链物理连接成功，则返回一个设备链路径句柄，来标志这组设备链，否则返回NULL
@note 如果该组设备逻辑上或者物理上无法连接，则返回NULL
@note 设备数组在该函数返回后可释放，函数实现会自行深拷贝数组中的信息
@note 对于已Connect形成hPath的设备句柄，如果对其进行Free操作，或者Free后再操作该hPath，这个行为是未定义的
@note 对于缺少某些唯一设备的设备链，是否可以正常工作是未定义的。如只有一个视频输出层时，设备链中没有EM_RESOURCE_VIDLAYER设备
*/
HPATH CSRMConnect(const HCSHANDLE hDevices[], int nDeviceNums);

/**
@brief 断开已连接的设备链

将通过CSRMConnect产生的设备链断开，断开后hPath句柄不再有效
@param[in] hPath 要断开的设备链句柄。
@return 成功返回TRUE;失败返回FALSE
@note 正常情况下，断开设备链后，才能Free设备链上的设备
*/
BOOL CSRMDisconnect(const HPATH hPath);

/**
@brief 返回设备句柄或者设备链对应类型的设备的设备索引

@param[in] hHandle 设备句柄或者设备链句柄
@param[in] emType 设备类型
@return 操作成功返回对应的索引；无效返回-1
@note 该接口主要用于设备分配模块对设备状态查询
@note hHandle可以是设备也可以是设备链
- 如果handel是一个设备句柄，则emType必须是该设备的类型，否则返回失败(-1)，这种情况下emType仅用于校验
- 如果handle是一个设备链句柄，则其中必须包含一个emType类型的设备，否则返回失败(-1)，这种情况下emType用来选择设备
*/
int CSRMGetIndex(const HCSHANDLE hHandle, CSRMResource_E  emType);

/**
@brief 查询指定的设备的空闲状态

如果一个设备通过CSRMAlloc接口分配出去，并且没有通过CSRMFree接口释放，则为非空闲状态；否则为空闲状态
@param[in] stDevice 待查询设备的标识
@return 目标空闲返回TRUE；否则返回FALSE
@note 该接口主要用于设备分配模块对设备状态查询
@note 对于不存在的设备返回FALSE
*/
BOOL CSRMIsFree(const CSRMDeviceReq_S stDevice);

/**
@brief 查询指定类型的设备的总数

查询设备某个类型的设备的总数，无论是否空闲或者已被分配
@param[in] emType 欲查询的设备类型
@return 该类型设备的总数。该平台没有该设备则返回0，未知的设备类型则返回-1
@note 该接口主要用于设备分配模块对设备状态查询
*/
int CSRMGetDeviceCounts(CSRMResource_E emType);

/**
@brief 查询指定Demux设备的配置

@param[in] nDmxIndex 待查询demux设备索引
@param[out] pstDemuxInfo 存储输出的demux配置信息，不可为NULL
@return 成功返回TRUE;失败返回FALSE
@note 该接口主要用于设备分配模块对设备状态查询
*/
BOOL CSRMGetDemuxConfig(int nDmxIndex,CSRMDemuxConfig_S* pstDemuxInfo);

/**
@brief 查询指定高频头设备的配置

@param[in] nTunerIndex 待查询Tuner设备索引
@param[out] pstTunerInfo 存储输出的Tuner配置信息，不可为NULL
@return 成功返回TRUE;失败返回FALSE
@note 该接口主要用于设备分配模块对设备状态查询
*/
BOOL CSRMGetTunerConfig(int nTunerIndex,CSRMTunerConfig_S* pstTunerInfo);

/**
@brief 查询指定视频解码器设备的配置

@param[in] nVideoIndex 待查询视频解码器设备索引
@param[out] pstVideoInfo 存储输出的视频解码器配置信息，不可为NULL
@return 成功返回TRUE;失败返回FALSE
@note 该接口主要用于设备分配模块对设备状态查询
*/
BOOL CSRMGetVideoConfig(int nVideoIndex,CSRMVideoConfig_S* pstVideoInfo);

/**
@brief 查询指定视频输出层设备的配置

@param[in] nVIDLayerIndex 待查询视频输出层设备索引
@param[out] pstVIDLayerInfo 存储输出的视频输出层配置信息，不可为NULL
@return 成功返回TRUE;失败返回FALSE
@note 该接口主要用于设备分配模块对设备状态查询
*/
BOOL CSRMGetVIDLayerConfig(int nVIDLayerIndex,CSRMVIDLayerConfig_S *pstVIDLayerInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
