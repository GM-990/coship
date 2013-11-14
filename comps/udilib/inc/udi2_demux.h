/**@defgroup DEMUX DEMUX 定义了平台无关的解复用设备驱动接口
@brief DEMUX模块对应头文件<udi2_demux.h>，包括对Demux设备能力和状态的描述接口。

能独立工作的解复用设备抽象为一个Demux，与硬件的Demux保持一致即可。并且主Demux的索引必须排在前面。

Demux在使用前必须与Tuner进行连接，开机后在没有调用任何连接断开接口之前，Tuner0与Demux0连接，Tuner1与Demux1连接，保证了在没有这个模块的MMCP（CloudTV ）中间件平台系列产品中，驱动也能正常工作。所以要将主Demux索引放在前面的原因之一。

Demux的能力（是否可以Inject，Record）决定一个Demux是否可以进行边录边播，或者只能进行普通的播放，播放时必须另外找一个Demux去进行录制。

假设在能力允许范围内，一个Demux的输出能力可以同时工作，但输入能力不能同时工作。即若一个Demux既有录制能力也有播放能力，则假设它可以同时录、播一路TS流中的不同节目；但一个Demux即使同时拥有DMA和Inject的能力，也不能同时即从Tuner获取数据又从Inject获取数据。总之，不能有两个数据源同时连接同一个demux

本模块还可以用于获取已从Tuner注入的TS包的数量，用于检查前端码流停播。它与pid停播的区别在于pid停播的信息可以从Player获得。是否可以获取软注入的TS包不做要求，UDI的实现也不需要刻意回避，MMCP（CloudTV ）中间件平台不会使用该接口获取软注入的TS包信息。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.4 2009/09/14 添加Tuner,Demux连接状态查询功能；添加Tuner和Demux不可连接的错误代码说明
@version 2.0.3 2009/09/08 添加EM_UDI_DEMUX_DMA以将输出能力和数据获取能力分开，因为他们本来就是独立的
@version 2.0.2 2009/08/26 增加是否支持容器注入的能力类型
@version 2.0.2 2009/08/26 去掉demux能力信息中的通道数和端口数，增加是否支持直播的能力
@version 2.0.2 2009/08/25 增加了Demux设备抽象规则的说明
@version 2.0.1 2009/08/17 初稿
@{
*/

#ifndef _UDI2_DEMUX_H_
#define _UDI2_DEMUX_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDIDEMUX_NOT_CONNECTED_ID			(0xFFFFFFFF)			///< 未和任何Demux连接的TunerId

/**@brief DEMUX 设备相关错误代码值*/
enum
{
	CSUDIDEMUX_ERROR_BAD_PARAMETER = CSUDI_DEMUX_ERROR_BASE,		///< 错误参数
	CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED,							///< 不支持此操作
	CSUDIDEMUX_ERROR_UNKNOWN_ERROR,									///< 一般错误
	CSUDIDEMUX_ERROR_DEVICE_BUSY,									///< 设备忙,比如Demux已经连接过了
	CSUDIDEMUX_ERROR_NO_MEMORY									///< 无可用内存
};

/**@brief DEMUX支持的容器注入类型

@note 这里所谓的“注入”指数据进入，而不管方式，即无论是通过Tuner DMA过来的，还是从Buffer软注入的。
*/
typedef enum
{
	EM_UDI_DEMUX_INJECT_AVI 	= 1 << 0,       ///< 支持AVI类型数据注入
	EM_UDI_DEMUX_INJECT_RM		= 1 << 1,		///< 支持RM类型数据注入
	EM_UDI_DEMUX_INJECT_RMVB 	= 1 << 2,		///< 支持RMVB类型的数据注入
	EM_UDI_DEMUX_INJECT_TS      = 1 << 3        ///< 支持TS类型的数据注入
	
}CSUDIDEMUXContainerType_E;

/**@brief DEMUX支持工作类型*/
typedef enum
{
	EM_UDI_DEMUX_PLAY 	  		= 1 << 0,	 ///< 支持播放
	EM_UDI_DEMUX_RECORD   		= 1 << 1,    ///< 支持录制
	EM_UDI_DEMUX_DMA			= 1 << 2,	 ///< 支持以DMA形式获取Tuner的数据，即Tuner和Demux可以通过硬件方式直接连接，这种Demux通过CSUDIDEMUXConnectTuner和Tuner连接后无需任何操作即可获取Tuner的数据，绝大多数可以从Tuner获取数据的Demux都是拥有这个属性
	EM_UDI_DEMUX_INJECT 		= 1 << 3     ///< 回放数据的注入，包括PVR类型的TS注入，以及IP通道类型的TS的数据注入，软注入Tuner形式的数据注入等	
}CSUDIDEMUXWorkType_E;

/**@brief DEMUX设备能力信息*/
typedef struct  
{
	int 			m_nFilterNum;                      ///< 过滤器个数 
	int 			m_nkeyNum;                         ///< 密钥区(descramble)个数
	CSUDI_UINT32	m_dwWorkTypeMask;                  ///< 支持的TS源注入类型，可以是CSUDIDEMUXWorkType_E中的(EM_UDI_DEMUX_PLAY | EM_UDI_DEMUX_INJECT | EM_UDI_DEMUX_RECORD ) 的各种组合
	CSUDI_UINT32    m_dwConTypeMask;				   ///< 支持的容器数据注入类型，可以是CSUDIDEMUXContainerType_E中的各种组合
	CSUDI_UINT32	m_dwDMAConnectedTunerId;		   ///< 当前与该Demux连接上的DMA TunerId，如果不是DMA Demux或者DMA Demux未和任何Tuner相连，则该值为CSUDIDEMUX_NOT_CONNECTED_ID
	CSUDI_UINT32	m_dwDMAConnectSupportTunersMask;   ///< 可以与该Demux连接的TunerId列表，其中第n位表示TunerId为n的Tuner可以与该Demux相连，暂不考虑可移动Tuner与Demux硬连接的情况
} CSUDIDEMUXCapability_S;

/**
@brief 获取Demux设备的总数

@param[out] pnDemuxCount 系统中存在demux设备的个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pnDemuxCount的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetCount(int * pnDemuxCount);

/**
@brief 获取指定Demux设备的能力

无论空闲或者被占用
@param[in] nDemuxIndex demux设备的索引
@param[out] psCapabilityInfo demux设备能力信息，详见CSUDIDEMUXCapability_S定义
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且psCapabilityInfo的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetCapability(int nDemuxIndex,CSUDIDEMUXCapability_S * psCapabilityInfo);

/**
@brief 用指定demux连接指定tuner

@param[in] dwTunerId  与指定demux连接的tuner设备Id
@param[in] nDemuxIndex  与指定tuner连接的demux索引号
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
@note 
	- 所有需要tuner和demux设备的服务在启动之前都必须调用该接口进行tuner与demux的连接
	- 如果处在工作状态的demux具有直播和录制功能，在启动直播之后，再去启动录制时，可以不用调用此接口再次连接tuner和demux，其它类似服务按照此规定。
@note 开机后Tuner和Demux的默认连接规则为0-0，1-1以此类推。因此，请尽量将主Demux的索引排在前面。
@note 一个Tuner可以和多个Demux相连,比如边录边播的情况;但一个Demux只能同时连接一个Tuner
@note 若同时将一个Demux连接到多个Tuner上，返回CSUDIDEMUX_ERROR_DEVICE_BUSY
@note Demux必须先与Tuner断开连接，才能用于创建Inject，即一个Demux不能同时拥有两个源设备
@note 同样，被Inject占用的Demux设备不能与Tuner连接，这种情况下将返回CSUDIDEMUX_ERROR_DEVICE_BUSY
@note 若所连接的Tuner和Demux无法连接，比如将内置Tuner与不支持Live的Inject的Demux连接，返回CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED
@note 只有DMA类型的Tuner才需要使用该接口进行连接,否则请返回CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED
@note 若对应Tuner与Demux不可连接，返回CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED，这种情况是基于某些平台的特性或者简化实现而定义的
@note 暂不考虑DMA类型的可移动Tuner设备

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXConnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex);

/**
@brief 断开指定demux与指定tuner的连接

@param[in] dwTunerId 与指定demux连接的tuner设备Id
@param[in] nDemuxIndex 与指定tuner连接的demux索引号
@return 成功断开则返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXDisconnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex);

/**
@brief 获取指定Demux设备的空闲Filter总数

@param[in] nDemuxIndex demux设备的索引
@param[out] pnFreeFilterCount 当前存在的空闲filter个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pnFreeFilterCount的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetFreeFilterCount(int nDemuxIndex, int * pnFreeFilterCount);

/**
@brief 获取指定Demux设备当前ts包个数

获取已经注入(从Tuner硬注入)的TS包的数量
@param[in] nDemuxIndex demux设备的索引
@param[out] puTsPacketCount 存放当前dumux中ts包个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且puTsPacketCount的内容未定义
@note 本接口主要与CSUDIPLAYERGetPacketCount接口配合使用判断前端码流是否停播
@note 本接口仅要求从对从Tuner来的数据有效，是否可以获取软注入的TS包不做要求，UDI2.0的实现也不需要刻意回避，Eastwin不允许使用该接口获取软注入的TS包信息。
@note 在部分平台上，该接口无法实现。此时须返回CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED。 Eastwin不会在不支持的平台上使用此接口判断码流停播。
@deprecated 此接口中间件未使用，可不实现

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
		but Entropic Pipeline Manager doesn't support to get the TS packet count,
		so we always reture 0 of TS packet count.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetTsPacketCount(int nDemuxIndex, unsigned int * puTsPacketCount);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif

