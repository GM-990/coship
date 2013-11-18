/**@defgroup CSDEMUX demux 解复用及数据注入
@brief 解复用及数据注入

关于数据请求的几个接口的关系请看下图:
@image HTML demuxstate.png

@version 0.3 2008/11/27 
- 增加CSDMXStart回调时机的详细说明
- 增加Filter只保证16字节深度的描述

@version 0.2 2008/10/29 添加详细描述
@version 0.1 2008/10/17 初始版本
@{
*/

#ifndef _UDI_DEMUX_H_
#define _UDI_DEMUX_H_

#include "udi_typedef.h"
#include "udi_public.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief filter长度，不包含section中的length字段*/
#define FILTER_LEN 16

/**@brief Demux回调数据类型枚举*/
typedef enum _USPSectionDemuxCallbackType
{
	USP_SECTION_DEMUX_DATA,         /**< 表示一个PSI、SI数据包*/
	USP_PES_DEMUX_DATA,             /**< 表示一个PES数据包*/
	USP_RAW_DEMUX_DATA              /**< 表示一个RAW数据包*/
}USPSectionDemuxCallbackType;

/**@brief Demux回调数据结构*/
typedef struct _USPSectionDemuxCallbackData
{
	PBYTE m_pucData;		/**< Section数据指针*/
	int m_nDataLen;			/**< Section数据长度*/
	PVOID m_pvAddData;		/**< 用户数据，与USPSectionDemuxRequest等调用时的pvAppData参数一致*/
}USPSectionDemuxCallbackData;

/**@brief 数据请求的优先级，高优先级的section会比低优先级的section提前分发，在Demux模块中无效*/
typedef enum  
{
    SECTION_PRIORITY_HIGH,		/**< 高优先级*/
    SECTION_PRIORITY_MIDDLE,	/**< 中优先级*/  
    SECTION_PRIORITY_LOW,		/**< 低优先级*/
    NUM_SECTION_PRIORITIES		/**< 必须在枚举的最后一位，表示Section优先级的个数，不可当优先级用*/
} SectionPriority ;

/**
@brief Filter信息数据结构

提供信息用于数据的过滤，其过滤规则为:
@code
IF ngate[i] == 0
{
	IF data[i] & mask[i] == match[i] & mask[i]
	{
		data[i] can be passed with the filter.
	}
	ELSE
	{
		Drop the data
	}
}
ELSE
{
	IF data[i] & mask[i] != match[i] & mask[i]
	{
		data[i] can be passed with the filter.
	}
ELSE
	{
		Drop the data
	}
}
@endcode
@note 当i从0到FILTER_LEN － 1的所有过滤条件都成立时，整个filter的过滤条件成立，section才会被返回
@note 因为一般来说，我们不会对section length字段做过滤处理，所有我们的接口要求不过滤section的第2，3两个字节数据。即如下对应关系：
- Data[0]    ←→   MatchMask[0] 
- Data[1]    ←→   无
- Data[2]    ←→   无
- Data[3]    ←→   MatchMask[1] 
- Data[4]    ←→   MatchMask[2]
- Data[5]    ←→   MatchMask[3]
- ……
@note 由于跳过两个字节，实际Filter深度可为FILTER_LEN+1字节，但实现只能保证前16字节的过滤，即第FILTER_LEN-2、FILTER_LEN-1字节可能无效
*/
typedef struct _MatchMask_S
{
    BYTE match[FILTER_LEN];	    /**< 匹配字段*/
    BYTE mask[FILTER_LEN];	    /**< 关注字段*/
    BYTE negate[FILTER_LEN];	/**< 取反字段*/
}MatchMask;

/**@brief Filter信息，该信息描述所要请求的数据的特征*/
typedef struct _RequestInfo_S
{	    
    DWORD dwPID;		        /**< 请求数据的pid*/
    DWORD dwDemuxID;		    /**< 用于请求数据的Demux设备句柄*/
    long lTimeout;		        /**< 超时时间，超过这个时间就会发送超时消息，对于Demux接口无效*/
    void* pvAppData;		    /**< 用户数据，当返回Section时，该数据也会原封不动的返回给用户，对于Demux接口无效*/	
    BOOL bCRCCheck;		        /**< 是否进行CRC检查*/
    SectionPriority enmPriority;/**< 该请求的优先级 ，对于Demux接口无效*/
    MatchMask MMFilter;		    /**< Filter 信息*/
	int m_nPES;					/**< SectionRequestType中定义的数值，对于Demux接口无效*/
	int m_nTaskIndex;           /**< 请求所在线程，对于Demux接口无效*/
} RequestInfo;

/**@brief 数据打包格式，同CSAUDStreamType和CSVIDStreamType描述的数据编码格式不是同一层次*/
typedef enum _CSSYSTransportType_E
{
	EM_TRANSPORT_TYPE_UNKNOW,	/**< unknown or not supported stream format */
	EM_TRANSPORT_TYPE_ES,		/**< Elementary stream */
	EM_TRANSPORT_TYPE_PES,		/**< Packetized elementary stream */
	EM_TRANSPORT_TYPE_TS,		/**< Transport stream */
	EM_TRANSPORT_TYPE_DSS_ES,
	EM_TRANSPORT_TYPE_DSS_PES,
	EM_TRANSPORT_TYPE_VOB,		/**< video object, used with DVD */
	EM_TRANSPORT_TYPE_ASF,		/**< Advanced Systems Format */
	EM_TRANSPORT_TYPE_AVI,		/**< Audio Video Interleave */
	EM_TRANSPORT_TYPE_MP4,	   	 /**< MP4 (MPEG-4 Part12) container */
	EM_TRANSPORT_TYPE_FLV		/**< Flash video container */
} CSSYSTransportType_E;

/**@brief  Descrambler 加扰类型定义，用于改变以及设置加扰类型**/
typedef enum _CSDescramblerType_E
{
    CS_DESCRAMBLER_TYPE_DVB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_ECB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_CBC_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_ECB_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_CBC_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_OFB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_CTS_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_FASTI_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_ECB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_CBC_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_ECB_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_CBC_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_OFB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_CTS_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_ECB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CBC_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_ECB_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CBC_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_OFB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CTS_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CTS_SCHNEIER_DESCRAMBLER,    
    CS_DESCRAMBLER_TYPE_AES_NSA_MDD_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_NSA_MDI_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_IPTV_CSA_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DVB_EAVS_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_ECB_LR_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CBC_LR_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_ECB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_CBC_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_ECB_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_CBC_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_OFB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_CTS_DESCRAMBLER
} CSDescramblerType_E;


/**@brief 只有当CSSYSTransportType_E 值为ES或PES等数据类型时,该枚举有效,用来指定是视频流还是音频流*/
typedef enum _CSSYSStreamType_E
{
	EM_STREAM_TYPE_UNKNOW,	/**< unknown or not supported stream type */
	EM_STREAM_TYPE_VIDEO,
	EM_STREAM_TYPE_AUDIO
} CSSYSStreamType_E;


/**@brief 打开一个数据注入时所需的参数，仅为将来预留，目前无效*/
typedef struct _CSDMXOpenParams_S
{
	DWORD m_dwBufferSize; 	/**< 注入缓冲区的大小，目前必须为0*/
	DWORD m_dwAlignment; 	/**< 注入缓冲区对其方式，表示为2^m_dwAlignment，即2表示2^2=4字节对齐;3表示2^3=8字节对齐；
					            0表示使用默认值，目前无效，必须为0*/
	CSSYSTransportType_E m_dwTransportType; /**< 数据打包格式*/
	DWORD m_dwResvered0;	/**< 当CSSYSTransportType_E 值为ES或PES等数据类型时，该参数用来指定流类型.
								 具体定义参见CSSYSStreamType_E，其它情况必须为0*/
	DWORD m_dwResvered1;	/**< 保留参数，必须为0*/
 }CSDMXOpenParams_S;

/**
@brief 数据注入回调函数

@param[in] pContext 用户数据，等于数据注入启动参数的m_pCallbackContext项
*/
typedef void (*CSDMXCallback)(void *pContext);

/**@brief 数据注入启动播放参数*/
typedef struct _CSDMXStartParams_S 
{
	CSDMXCallback m_fnWriteCallback;/**< 当注入缓冲区不满，可以进行注入时会使用该回调函数通知上层可以注入后续数据
						                    不推荐直接在该回调中获取Buffer进行注入，这可能会严重影响播放的效率
						                建议在该回调中通知其它线程进行注入*/
	void *m_pCallbackContext; 	    /**< 用户数据，回调函数的参数*/	
	DWORD m_dwResvered0;		    /**< 保留参数，必须为0*/
	DWORD m_dwResvered1;		    /**< 保留参数，必须为0*/
	DWORD m_dwResvered2;		    /**< 保留参数，必须为0*/
} CSDMXStartParams_S;

/**
@brief Demux请求回调函数

当请求的数据到来时，产生回调
@param[in] enmType 回调类型
@param[in] pParam 回调数据,目前的所有的回调类型均为为指向一个USPSectionDemuxCallbackData类型的指针
*/
typedef void( *USPSectionDemuxCallback ) ( USPSectionDemuxCallbackType enmType, void* pParam );

/*-------------数据获取接口----------------*/

/**
@brief 获取指定Demux设备的Filter总数

无论空闲或者被占用
@param[in] hHandle 设备或设备链句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@return 返回Filter总数,出错返回-1
*/
int USPSectionGetDemuxFilterCount( HCSHANDLE   hHandle );

/**
@brief 获取指定Demux设备的空闲Filter总数

@param[in] hHandle 设备或设备链句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@return 返回空闲的Filter总数,出错返回-1
*/
int USPSectionGetDemuxFreeFilterCount( HCSHANDLE   hHandle );

/**
@brief 请求Filter过滤数据

常用来过滤PSI、SI，也可用于过滤PES、RAW等数据
这是仅支持PSI、SI的版本
@param[in] pRequestInfo Filter参数
@param[in] pDmxCallback 回调函数，当有符合条件的数据到来时，调用本函数通知，所有回调必须在同一线程且不在调用线程发生
@param[in] pvAppData 用户数据，将在回调时赋给回调数据的m_pvAddData字段
@return 返回当此请求的句柄，如果请求失败则返回-1(0是有效的)
@note 几个数据请求接口之间的关系请参考本章的“详细描述”部分
@note RequestInfo中也有一个pvAppData字段，是上层模块使用的，不必理会
@note 详细的过滤规则，请参考RequestInfo结构的定义
@note 对应的设备句柄在RequestInfo结构中
@note Request成功后，即开始接受、过滤相应数据
*/
int USPSectionDemuxRequest( RequestInfo *pRequestInfo, USPSectionDemuxCallback pDmxCallback, PVOID pvAppData );

/**
@brief 请求Filter过滤数据

常用来过滤Section，也可用于过滤PES、RAW等数据
这是支持PES的版本
@param[in] bRequestPES 请求PES数据标记,TRUE表示请求PES，FALSE时等于USPSectionDemuxRequest
@param[in] pRequestInfo Filter参数
@param[in] pDmxCallback 回调函数，当有符合条件的数据到来时，调用本函数通知，所有回调必须在同一线程且不在调用线程发生
@param[in] pvAppData 用户数据，将在回调时赋给回调数据的m_pvAddData字段
@return 返回当此请求的句柄，如果请求失败则返回-1
@note 几个数据请求接口之间的关系请参考本章的“详细描述”部分
@note RequestInfo中也有一个pvAppData字段，是上层模块使用的，不必理会
@note 详细的过滤规则，请参考RequestInfo结构的定义
@note 对应的设备句柄在RequestInfo结构中
@note Request成功后，即开始接受、过滤相应数据
*/
int USPSectionDemuxRequestEx( BOOL bRequestPES, RequestInfo *pRequestInfo, USPSectionDemuxCallback pDmxCallback, PVOID pvAppData );

/**
@brief 启动Filter过滤数据

仅启动，资源之前已经申请好
@param[in] hHandle 数据请求句柄,USPSectionDemuxRequest或USPSectionDemuxAllocateSectionFilter的返回值
@param[in] nFilter Filter句柄(USPSectionDemuxRequest返回的有效句柄)
@return 成功返回TRUE,失败返回FALSE
@note 几个数据请求接口之间的关系请参考本章的“详细描述”部分
@note Start成功后，即开始接受、过滤相应数据
*/    
BOOL USPSectionDemuxStart( HCSHANDLE hHandle, int nFilter );

/**
@brief 停止Filter过滤数据,但并不释放Filter资源

仅停止，不释放资源
@param[in] hHandle 数据请求句柄,USPSectionDemuxRequest或USPSectionDemuxAllocateSectionFilter的返回值
@param[in] nFilter Filter句柄(USPSectionDemuxRequest返回的有效句柄)
@return 成功返回TRUE,失败返回FALSE
@note 几个数据请求接口之间的关系请参考本章的“详细描述”部分
@note Stop成功后，即停止接受、过滤相应数据
*/
BOOL USPSectionDemuxStop(HCSHANDLE   hHandle, int nFilter );

/**
@brief 停止Filter接收数据,并释放Filter资源

停止并且释放资源
@param[in] hHandle 数据请求句柄,USPSectionDemuxRequest或USPSectionDemuxAllocateSectionFilter的返回值
@param[in] nFilter Filter句柄(USPSectionDemuxRequest返回的有效句柄)
@return 成功返回TRUE,失败返回FALSE
@note 几个数据请求接口之间的关系请参考本章的“详细描述”部分
@note Cancel成功后，即停止接受、过滤相应数据
*/
BOOL USPSectionDemuxCancel(HCSHANDLE   hHandle, int nFilter );

/**
@brief 请求Filter资源，设置Fillter参数，但不开始获取相关数据

仅申请资源，不启动
@param[in] bRequestPES bRequestPES ,TRUE表示请求PES，FALSE时请求PSI、SI
@param[in] pRequestInfo Filter参数
@param[in] pDmxCallback 回调函数，当有符合条件的数据到来时，调用本函数通知，所有回调必须在同一线程且不在调用线程发生
@param[in] pvAppData 用户数据，将在回调时赋给回调数据的m_pvAddData字段
@return 返回当前请求的句柄，如果请求失败则返回-1
@note 几个数据请求接口之间的关系请参考本章的“详细描述”部分
@note RequestInfo中也有一个pvAppData字段，是上层模块使用的，不必理会
@note 详细的过滤规则，请参考RequestInfo结构的定义
@note 对应的设备句柄在RequestInfo结构中
@note Alloca成功后，并不开始接受、过滤相应数据
*/
int USPSectionDemuxAllocateSectionFilter( BOOL bRequestPES, RequestInfo *pRequestInfo, USPSectionDemuxCallback pDmxCallback, PVOID pvAppData );

/**
@brief 修改Fillter设置

必须在Stop状态下才能调用
@param[in] hHandle 数据请求句柄,USPSectionDemuxRequest或USPSectionDemuxAllocateSectionFilter的返回值
@param[in] nFilter Filter句柄(USPSectionDemuxRequest返回的有效句柄)
@param[in] pRequestInfo Filter参数 
@return 成功返回TRUE；失败返回FALSE
@note 设备句柄以hHandle参数为准，忽略RequestInfo中的句柄参数
@note RequestInfo中也有一个pvAppData字段，是上层模块使用的，不必理会，也即这里无法改变pvAppData
*/
BOOL USPSectionDemuxModifySectionFilter(HCSHANDLE hHandle, int nFilter, RequestInfo *pRequestInfo);

/**
@brief 设置Fillter的底层buffer大小

主要用于需要过滤大量数据,上层可能会来不及取走的情况
@param[in] hHandle 设备或设备链句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[in] nFilter Filte句柄，使用请求Filter时的返回如USPSectionDemuxAllocateSectionFilter
@param[in] nBufSize 要设置的底层buffer的大小，必须为2的n次方，最小1K，最大值根据各个平台差异不同
@return 如果成功则返回 TRUE,失败则返回 FALSE.
@note 修改Filter size，filter需处于Stop（pause）状态才能修改filter。	
*/
BOOL USPSectionDemuxSetFilterBufSize(HCSHANDLE hHandle,int nFilter,int nBufSize);

/*-------------数据注入接口----------------*/

/**
@brief 打开一个数据注入通道

本接口用于播放本地、IP等非Tuner通道获取的音视频数据\n
数据由上层直接注入到底层，由底层完成相应的解复用(可选)、解码(可选)、输出等操作
@param[in] dwPath 设备链句柄，必须包括播放待注入数据的所有需要的设备，但这里并不一定需要检查，缺少设备会在后续步骤中失败
@param[in] pOpenParams 数据注入通道参数，具体见CSDMXOpenParams_S结构定义，目前该参数无效，必须传NULL或者其内容均为指定值
@return 成功返回TRUE；失败返回FALSE
@note 数据通道建立好，启动后，并不马上开始播放，仍要上层对该设备链上的其它设备进行操作以启动播放
*/
BOOL CSDMXOpen(DWORD dwPath,const CSDMXOpenParams_S* pOpenParams);

/**
@brief 关闭一个数据注入通道

释放Demux资源，但该资源仍属于设备链所有，可供该设备链进行其它后续操作
@param[in] dwPath 设备链句柄，同CSDMXOpen的该参数
@return 成功返回TRUE;失败返回FALSE
@note 通道关闭后，仍需要上层对设备链上的其它设备进行相应的处理，如关闭等
*/
BOOL CSDMXClose(DWORD dwPath);

/**
@brief 开始数据注入

本接口成功返回后，即可开始启动数据注入
@param[in] dwPath 设备链句柄，同CSDMXOpen的该参数
@param[in] pStartParams 启动控制参数
@return 成功返回TRUE;失败返回FALSE
@note Start后并不会立刻产生回调通知可写，必须先至少写入一次数据后才有可能收到通知
@note 同时若某次通知发出后，如因意外原因未成功写入，也不会再次产生通知，需上层自行进行恢复处理
*/
BOOL CSDMXStart(DWORD dwPath,CSDMXStartParams_S* pStartParams);

/**
@brief 停止数据注入

@param[in] dwPath 设备链句柄，同CSDMXOpen的该参数
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSDMXStop(DWORD dwPath);

/**
@brief 数据注入复位

复位数据注入，清空已注入的数据
@param[in] dwPath 设备链句柄，同CSDMXOpen的该参数
@return 成功返回TRUE;失败返回FALSE
@note 数据注入出现异常或其它情况需要丢弃已注入数据时进行复位
*/
BOOL CSDMXReset(DWORD dwPath);

/**
@brief 获得数据注入缓冲区的内存和缓冲区空闲的大小

获取数据注入的缓冲区，供上层使用。如IPTV中将socket 口收到数据直接写到Demux的缓冲区中。
@param[in] dwPath 设备链句柄，同CSDMXOpen的该参数
@param[out] ppBuffer 返回缓冲区的写数据地址
@param[out] pSize 返回缓冲区空闲的大小
@return 得到缓冲区返回TRUE;否则返回FALSE
@note 必须与CSDMXWriteComplete成对调用，除非CSDMXGetFreeBuffer没有得到任何缓冲区
@note 该接口是不阻塞的，如果调用时没有可用空间则*ppBuffer 的值将会是NULL，并且*pSize =0
@note 当TS注入模块有空余空间时m_fnWriteCallback函数将被调用
@note 不推荐在通知回调中直接调用该函数进行注入，推荐在回调中发消息通知另外的线程开始注入
*/
BOOL  CSDMXGetFreeBuffer(DWORD dwPath, void **ppBuffer, int*pSize);

/**
@brief 通知底层本次注入操作写入的数据和跳过的数据大小，以修改Demux缓冲区指针

@param[in] dwPath 设备链句柄，同CSDMXOpen的该参数
@param[out] nSkipSize 本次注入操作忽略的缓冲区数据大小，即忽略上次获取到的缓冲区内存的前nSkipSize字节的内容
@param[out] nUsedSize 本次注入的实际数据的大小，即注入的有效数据的大小，该数据存放在上次获取的缓冲区起始地址偏移nSkipSize字节的位置
@return 成功返回TRUE;失败返回FALSE
@note 必须与CSDMXGetFreeBuffer成对调用，除非CSDMXGetFreeBuffer没有得到任何缓冲区
@note 0 <= nSkipSize+nUsedSize < CSDMXGetFreeBuffer到的pSize
*/
BOOL CSDMXWriteComplete(DWORD dwPath, int nSkipSize, int  nUsedSize);

/*-------------解扰接口----------------*/

/**
@brief 分配一个解扰的通道ID

@param[out] pdwDescrambleID 分配的解扰通道ID
@return 分配成功返回0;分配失败-1
*/
int CSDSMAllocateDescramble(DWORD *pdwDescrambleID);

/**
@brief 分配一个解扰的通道ID

在指定的Demux设备上分配
@param[in] hHandle Demux设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来
@param[out] pdwDescrambleID 分配的解扰通道ID
@return 分配成功返回0;分配失败-1
*/
int CSDSMAllocateDescrambleEX(HCSHANDLE hHandle ,DWORD *pdwDescrambleID);

/**
@brief 释放分配的解扰的通道ID

@param[in] dwDescrambleID 解扰通道ID,CSDSMAllocateDescramble的输出参数
@return 成功 0;失败 -1
*/ 
int CSDSMFreeDescramble(DWORD dwDescrambleID );

/**
@brief 为解扰的通道ID指定过滤的PID

@param[in] dwDescrambleID 解扰通道ID,CSDSMAllocateDescramble的输出参数
@param[in] wPID 解扰PID
@return 成功 0;失败 -1
@note 设置解扰PID前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的，使用中亦需要注意该顺序问题
*/ 
int CSDSMSetDescramblePID(DWORD dwDescrambleID,WORD wPID );

/**
@brief 清空解扰通道过滤的PID

@param[in] dwDescrambleID 解扰通道ID,CSDSMAllocateDescramble的输出参数
@param[in] wPID 解扰PID
@return 成功 0;失败 -1
*/ 
int CSDSMClearDescramblePID(DWORD dwDescrambleID,WORD wPID );

/**
@brief 设置解扰通道的奇Key值

@param[in] dwDescrambleID 解扰通道ID,CSDSMAllocateDescramble的输出参数
@param[in] pbOddKey 解扰的奇KEY
@param[in] bOddLength Key长度
@return 成功 0;失败 -1
@note 设key前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的
*/ 
int CSDSMDescrambleSetOddKeyValue(DWORD dwDescrambleID, BYTE* pbOddKey, BYTE bOddLength);

/**
@brief 设置解扰通道的偶Key值

@param[in] dwDescrambleID 解扰通道ID,CSDSMAllocateDescramble的输出参数
@param[in] pbEvenKey 解扰的偶KEY
@param[in] bEvenLength Key长度
@return 成功 0;失败 -1
@note 设key前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的
*/ 
int CSDSMDescrambleSetEvenKeyValue(DWORD  dwDescrambleID, BYTE *pbEvenKey, BYTE bEvenLength);


/**
@brief 设置解扰通道的解扰类型

@param[in] dwDescrambleID 解扰通道ID,CSDSMAllocateDescramble的输出参数
@param[in] eDescrambleType 码流加扰类型
@return 成功 0;失败 -1
@note 若不调用此接口，默认情况下Descramble Type为DVB
*/ 
int CSDSMSetDescrambleType(DWORD dwDescrambleID , CSDescramblerType_E  eDescrambleType);

/**
@brief 设置System Key,在Multi2解扰的时候要调用此接口。SystemKey可以看成是一个Demux的一个全局属性与通道无关。

@param[in] hDmxHandle Demux设备操作句柄，由应用通过调CSRMAlloc()或CSRMConnect()分配得来。与CSDSMAllocateDescrambleEX的第一个参数是一致的。
@param[in] pbSystemKey SystemKey的地址，必须为Multi2标准规定的32B
@return 成功 0;失败 -1
@note 该接口可以在任何解扰器被分配前调用被调用
	如果一个Demux有多个解扰器，而所有解扰器的SystemKey是一样的，
	这种情况下只需调一次CSDSMDescrambleSetSystemKey。
*/ 
int CSDSMDescrambleSetSystemKey(HCSHANDLE  hDmxHandle, BYTE * pbSystemKey);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif

