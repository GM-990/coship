/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_bluetooth.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_bluetooth.h"
/**
@brief 注册蓝牙事件监听函数
@param[in] fnCb  回调函数
@param[in] pUserData 用户自定义参数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTAddListener(CSUDIBTCallback_F fnCb,void* pUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief 获取所有蓝牙适配器
@param[out] ahOutHandleArray 输出蓝牙适配器句柄数组
@param[in] dwArraySize 数组大小
@param[out] pdwOutCount 输出蓝牙适配器个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 实际蓝牙设配器个数大于dwArraySize时，函数返回CSUDI_SUCCESS，且输出参数*pdwOutCount为dwArraySize
*/
CSUDI_Error_Code CSUDIBTGetAllAdapter(CSUDI_HANDLE *ahOutHandleArray,unsigned int dwArraySize,unsigned int* pdwOutCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief 获取蓝牙适配器信息
@param[in] hAdapter 蓝牙适配器句柄
@param[out] psOutInfo 输出蓝牙适配器信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTGetAdapterInfo(CSUDI_HANDLE hAdapter,CSUDIBTAdapterInfo_S* psOutInfo)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 设置蓝牙适配器名称
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szName 设置的蓝牙适配器名称,字符串长度不能超过CSUDIBT_STR_LEN
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTSetAdapterName(CSUDI_HANDLE hAdapter, const char *szName)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}
/**
@brief 获取蓝牙适配器状态
@param[in] hAdapter 蓝牙适配器句柄
@param[out] peOutStatus 输出蓝牙适配器状态
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTGetAdapterStatus(CSUDI_HANDLE hAdapter,CSUDIBTAdapterStatus_E* peOutStatus)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 打开蓝牙适配器
@param[in] hAdapter 蓝牙适配器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTTurnOn(CSUDI_HANDLE hAdapter)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 关闭蓝牙适配器
@param[in] hAdapter 蓝牙适配器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTTurnOff(CSUDI_HANDLE hAdapter)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取所有蓝牙设备,包括曾经扫描到的设备
@param[in] hAdapter 蓝牙适配器句柄
@param[out] psOutDevicesArray 输出蓝牙设备信息数组
@param[in] dwArraySize 蓝牙设备信息结构体数组大小
@param[out] pdwOutCount 输出蓝牙设备个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 实际蓝牙设备个数大于nArraySize时，函数返回CSUDI_SUCCESS，且输出参数*pdwOutCount为dwArraySize
*/
CSUDI_Error_Code CSUDIBTGetDevices(CSUDI_HANDLE hAdapter,CSUDIBTDeviceInfo_S* psOutDevicesArray,unsigned int dwArraySize,unsigned int* pdwOutCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 删除蓝牙设备
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szDeviceAddress 蓝牙设备地址
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 成功删除的设备,其配对信息同步删除.必须重新扫描才可以通过CSUDIBTGetDevices接口获得。
*/
CSUDI_Error_Code CSUDIBTDeleteDevice(CSUDI_HANDLE hAdapter, const char* szDeviceAddress)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 启动扫描蓝牙设备
@param[in] hAdapter 蓝牙适配器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 扫描过程中，每扫描到一个蓝牙设备都会抛EM_UDIBT_EVENT_TYPE_FOUND_DEVICE消息，扫描完成或者扫描中止会抛EM_UDIBT_EVENT_TYPE_SCAN_OK消息。
*/
CSUDI_Error_Code CSUDIBTStartScan(CSUDI_HANDLE hAdapter)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 停止扫描蓝牙设备
@param[in] hAdapter 蓝牙适配器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTStopScan(CSUDI_HANDLE hAdapter)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 启动蓝牙设备配对
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szDeviceAddress 需配对的蓝牙设备地址
@param[in] szPassword 配对密码字符串
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTStartPair(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,const char* szPassword)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 启动连接蓝牙设备
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szDeviceAddress 需连接的蓝牙设备地址
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 只有已配对的设备才能进行连接操作
*/
CSUDI_Error_Code CSUDIBTStartConnect(CSUDI_HANDLE hAdapter,const char* szDeviceAddress)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取蓝牙设备状态
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szDeviceAddress 蓝牙设备地址
@param[out] peOutStatus 蓝牙设备状态
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTGetDeviceStatus(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,CSUDIBTDeviceStatus_E* peOutStatus)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 注册启用蓝牙服务sdp发现协议函数
@param[in] pcSerialID 序列号
@param[in] nPort 服务器的侦听端口
@param[in] pcName 服务名，与sdp服务的连接无关
@param[in] pcDsc 服务描述名，与sdp服务的连接无关
@param[in] pcProv 服务提供商名，与sdp服务的连接无关
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTSdpRegister(const char* pcSerialID, unsigned int dwPort, const char* pcName, const char* pcDsc, const char* pcProv)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


