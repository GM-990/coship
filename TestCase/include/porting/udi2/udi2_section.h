/**@defgroup SECTION SECTION 数据过滤相关接口定义
@brief Section模块对应头文件<udi2_section.h>，定义获取DVB PSI、SI、PES、RAW数据的接口。

本模块以FILTER过滤器为控制单位，每在一个Demux上启动一个过滤器，所有经过该Demux并
且符合过滤条件的数据都会通过Allocate时设置的回调函数抛上来，所有的回调必须在同一
个线程中产生。\n
过滤器的生命周期分为Ready、Running和Cancel三个状态，过滤器通过本模块提供的几个接
口在这些状态间切换，其对应关系如下图所示：
- 过滤器在过滤EM_UDI_REQ_SECTION_DATA类型的数据时，会跳过第二个字节Data[1]和第三
个字节Data[2]的数据，即Filter[0]与Data[0]对应，Filter[1]与Data[3]对应，Filter[2]
与Data[4]对应，Filter[3]与Data[5]，依此类推。因为Data[1]和Data[2]这两个字节是
Section的长度，所以不进行过滤。
- 过滤器在过滤其他类型的数据时，过滤器的CSUDIFILTERMatchMask_S参数无效。
@image HTML udi_section_state.png

@brief 在许多平台上，相关功能都有一个通道的概念。即一个Demux上可以挂多个通道（对应
一个PID），一个通道上再挂多个Filter，下图描述了这种模型。这种情况下，本模块仅关
注Filter，其实现需处理好UDI与通用过滤器模型间的转换。即当申请一个新Filter时，若实现
发现对应的通道未被创建则需要首先创建之，若已经创建则在其上添加Filter即可。同样当需
要删除一个Filter时，实现需关注若该通道上已没有其它任何Filter则需同时关闭通道，反之则
保留通道。
@image HTML udi_section_channel.png

@note
- 因为NDS CA使用ICAM模块获取Section数据，所以需要特别注意ICAM模块与Section模块之间的
协调，防止资源存在冲突。
- 不同的过滤器之间，可能存在部分数据的重叠，这种情况下，数据应通知到所有过滤器申请时
对应的回调函数，而不是仅仅通知第一个。

@version 2.0.4 2009/09/15 去掉CSUDISECTIONCallbackData_S结构体中多余的m_pvAddData
@version 2.0.3 2009/09/14 增加在非法状态调用接口返回值的具体说明
@version 2.0.2 2009/08/25 增加了过滤器通道的说明,增加了过滤器状态转换说明
@version 2.0.1 2009/08/17 初稿
@{
*/

#ifndef _UDI2_SECTION_H_
#define _UDI2_SECTION_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif
 

/**@brief 解复用相关错误代码值*/
enum 
{
	CSUDISECTION_ERROR_BAD_PARAMETER = CSUDI_SECTION_ERROR_BASE,      ///< 参数错误
	CSUDISECTION_ERROR_FEATURE_NOT_SUPPORTED,	///< 操作不支持
	CSUDISECTION_ERROR_UNKNOWN_ERROR,			///< 一般错误
	CSUDISECTION_ERROR_NO_MEMORY,				///< 无可用内存
	CSUDISECTION_ERROR_INVALID_HANDLE,			///< 非法句柄
	CSUDISECTION_ERROR_INVALID_STATUS,  		///< 无效状态，比如在没有stop时进行modify
	CSUDISECTION_ERROR_NO_FREE_FILTER			///< 没有可用的filter
};


/**@brief filter长度，不包含section中的length字段*/
#define CSUDISECTION_FILTER_LEN 16

/**@brief Section回调数据类型枚举*/
typedef enum
{
	EM_UDI_REQ_SECTION_DATA,		///< 表示一个PSI、SI数据包
	EM_UDI_REQ_PES_DATA,			///< 表示一个PES数据包(除音视频外要求整包数据)
	EM_UDI_REQ_RAW_DATA			///< 表示一个RAW数据包
}CSUDISECTIONRequestType_E;


/**@brief Section回调数据结构*/
typedef struct 
{
	unsigned char* m_pucData;		///< Section数据指针
	int            	  m_nDataLen;			///< Section数据长度
}CSUDISECTIONCallbackData_S;

/**@brief Filter信息数据结构

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
@note 当i从0到CSUDISECTION_FILTER_LEN － 1的所有过滤条件都成立时，整个filter的过滤条件成立，section才会被返回
@note 因为一般来说，我们不会对section length字段做过滤处理，所有我们的接口要求不过滤section的第2，3两个字节数据。即如下对应关系：
- Data[0]    ←→   MatchMask[0] 
- Data[1]    ←→   无
- Data[2]    ←→   无
- Data[3]    ←→   MatchMask[1] 
- Data[4]    ←→   MatchMask[2]
- Data[5]    ←→   MatchMask[3]
- ……
@note 由于跳过两个字节，实际Filter深度可为CSUDISECTION_FILTER_LEN+1字节，但实现只能保证前16字节的过滤，即第CSUDISECTION_FILTER_LEN-2、CSUDISECTION_FILTER_LEN-1字节可能无效

@note Filter只对EM_UDI_REQ_SECTION_DATA有效，其它类型无效
*/
typedef struct 
{
    unsigned char match[CSUDISECTION_FILTER_LEN];	    ///< 匹配字段
    unsigned char mask[CSUDISECTION_FILTER_LEN];	    ///< 关注字段
    unsigned char negate[CSUDISECTION_FILTER_LEN];	    ///< 取反字段
}CSUDIFILTERMatchMask_S;


/**@brief Filter信息，该信息描述所要请求的数据的特征*/
typedef struct 
{	   
    CSUDISECTIONRequestType_E		m_eReqType;	    ///< 指定请求类型，请参见CSUDISECTIONRequestType_E
    int 							m_nDemuxID;	    ///<指定Demux Index
    int                          	             m_nPID;		    ///< 指定请求数据的pid
    CSUDI_BOOL                               m_bCRCCheck;	    ///< 是否进行CRC检查，注意：对于待申请数据(PSI/SI/私有数据)在数字电视相关标准文档中已明确没有CRC字段的情况，要求中间件设置该项为CSUDI_FALSE，并且实现者也无需关注该项为CSUDI_TRUE情况下的实现机制；
    CSUDIFILTERMatchMask_S     		 m_sMMFilter;	    ///< Filter 信息
} CSUDISECTIONRequestInfo_S;



/**
@brief Demux请求回调函数

当请求的数据到来时，产生回调
@param[in] eType 回调类型，详情请参见CSUDISECTIONRequestType_E
@param[in] pvParam 回调数据，目前的所有的回调类型均为为指向一个CSUDISECTIONCallbackData_S类型的指针
@param[in] pvUserData 用户数据
*/
typedef void ( *CSUDISECTIONCallback_F ) ( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData);


/**
@brief 请求Filter过滤数据

常用来过滤PSI、SI，也可用于过滤PES、RAW等数据,需要与CSUDIFILTERStart配合使用
@param[in] psRequestInfo Filter参数，详情请参见CSUDISECTIONRequestInfo_S
@param[in] fnSectionCallback 回调函数，当有符合条件的数据到来时，调用本函数通知，所有回调必须在同一线程且建议不要在调用线程中执行太多操作，详情请参见CSUDISECTIONCallback_F
@param[in] pvUserData 用户数据，可以为CSUDI_NULL
@param[out] phFilterHandle	用于接收返回的Filter句柄，返回值为0认为非法
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 
- 详细的过滤规则，请参考CSUDISECTIONRequestInfo_S结构的定义
- 对应的设备句柄在CSUDISECTIONRequestInfo_S结构中
- CSUDIFILTERAlloc成功后，调用CSUDIFILTERStart开始接收数据
- CSUDIFILTERAlloc可以多次调用，直到Filter通道数全部分配完为止
*/
CSUDI_Error_Code CSUDIFILTERAllocate(const CSUDISECTIONRequestInfo_S * psRequestInfo, CSUDISECTIONCallback_F fnSectionCallback, const void * pvUserData ,CSUDI_HANDLE * phFilterHandle);

/**
@brief 启动Filter过滤数据

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Start成功后，即开始接收、过滤相应数据
- 第一次调用后，再次(多次)调用将返回失败
*/    
CSUDI_Error_Code CSUDIFILTERStart(CSUDI_HANDLE hFilterHandle);

/**
@brief 停止Filter过滤数据,但并不释放Filter资源

仅停止，不释放资源
@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Stop成功后，即停止接收、过滤相应数据
- 试图去停止一个未启动的Filter返回CSUDISECTION_ERROR_INVALID_STATUS
- 第一次调用后，再次(多次)调用将返回失败
*/
CSUDI_Error_Code CSUDIFILTERStop(CSUDI_HANDLE hFilterHandle );

/**
@brief 停止Filter接收数据,并释放Filter资源

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Free成功后，即停止接收、过滤相应数据
- 第一次调用后，再次(多次)调用将返回失败
*/
CSUDI_Error_Code CSUDIFILTERFree(CSUDI_HANDLE hFilterHandle);

/**
@brief 修改Fillter设置

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@param[in] psMMFilter Filter 参数详见 CSUDIFILTERMatchMask_S定义,若为CSUDI_NULL表示不更改此项
@param[in] bCRCCheck 是否检测CRC
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- 必须在Stop状态下才能修改filter参数，否则返回CSUDISECTION_ERROR_INVALID_STATUS
- 第一次调用后，再次(多次)调用将返回失败
*/
CSUDI_Error_Code CSUDIFILTERModify(CSUDI_HANDLE hFilterHandle, const CSUDIFILTERMatchMask_S * psMMFilter,  CSUDI_BOOL bCRCCheck);

/**
@brief 设置Fillter的底层buffer大小

主要用于需要过滤大量数据,上层可能会来不及取走的情况
@param[in] hFilterHandle 由CSUDIFILTERAlloc分配的过滤器句柄
@param[in] nBufSize 要设置的底层buffer的大小，必须为2的n次方，最小1K，最大值根据各个平台差异不同
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置Fillter的底层buffer大小时必须让Fillter处于stop状态，否则返回CSUDISECTION_ERROR_INVALID_STATUS	
*/
CSUDI_Error_Code CSUDIFILTERSetBufSize(CSUDI_HANDLE hFilterHandle,  int nBufSize);


#ifdef  __cplusplus
}
#endif

/** @} */

#endif


