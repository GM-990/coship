#ifndef _CS_SYSDEMUX_H_
#define _CS_SYSDEMUX_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "udi_demux.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
@brief Section或PES包的环形Buffer大小的钩子函数

@param[in] dwDemuxID Demux ID
@param[in] nRequestDataType 表示请求的数据类型如:PES、SECTION、TS包,见USPSectionDemuxCallbackType
@param[in] pRequestInfo 用户用于请求数据的Filter参数 
@return 返0表示用默认值，否则按这个返回值设置环形Buffer大小
@note 由用户按实际需求实现
*/
typedef int (*USPSectionDemuxBufSizeHook_F)(DWORD dwDemuxID, USPSectionDemuxCallbackType eRequestDataType, const RequestInfo *pRequestInfo);

/**
@brief 初始化Demux设备

@param[in] dwDemuxID Demux标识;当系统中只有一个Demux时,该参数可以省略；该参数也可以是一个Demux设备或者设备链
@return 成功返回TRUE;失败返回FALSE
*/
BOOL InitUSPSectionDemux( DWORD dwDemuxID );

/**
@brief 链接指定的Demux与Tuner设备

目前该函数无用
@param[in] dwDemuxID 指定的Demux设备句柄
@param[in] dwTsInID 设备指定的Tuner设备句柄
@param[in] hReserved 保留参数
@return 成功返回TRUE;失败返回FALSE
*/ 
BOOL USPTsSourceConnect( DWORD dwDemuxID, DWORD dwTsInID, HCSHANDLE hReserved );

/**
@brief 设置Section或PES包的环形Buffer大小的钩子函数

@param[in] fnBufSizeHook
@return 成功返回TRUE；失败返回FALSE
@note 项目中可以通过钩子函数返回值来设定平台底层用于存放Section或PES包的环形Buffer大小，没有设置则进行默认值来实现
*/
BOOL USPSectionDemuxSetBufSizeHook(USPSectionDemuxBufSizeHook_F fnBufSizeHook);


#ifdef  __cplusplus
}
#endif

#endif

