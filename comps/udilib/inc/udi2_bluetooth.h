/**@defgroup 蓝牙接口，udi_bluetooth，模块简称：CSUDIBT
	本机的蓝牙设备称为蓝牙适配器(Adapter)，网络中其他蓝牙设备称为蓝牙设备(Device)
	
模块简称BT

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.1.0 2011/10/18 增加多个回调消息及接口,重新定义CSUDIBTGetDevices接口
@version 2.0.1 2011/06/21 定义了作为蓝牙client的接口
@{
*/
#ifndef _UDI2_BLUETOOTH_H_
#define _UDI2_BLUETOOTH_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_tuner.h"

#ifdef  __cplusplus
extern "C" {
#endif


#define CSUDIBT_STR_LEN 64 				///< 蓝牙模块使用的字符串长度（名称字符串与地址字符串）

typedef enum _CSUDIBTProfileType_E{
	CSUDIBT_PROFILE_A2DP = 0x01, ///< 蓝牙Advanced Audio Distribution Profile（高级音频传输模型，用于蓝牙传送立体声音乐），
	CSUDIBT_PROFILE_HFP = 0x02,  ///< 蓝牙Headset profile（手机模型，用于手机语音通话）
	CSUDIBT_PROFILE_HSP = 0x04,  ///< 蓝牙Hands Free Profile（免提模型，在HFP基础上扩展了某些功能，开始用于车载系统）
	CSUDIBT_PROFILE_HID = 0x08   ///< 蓝牙Human Interface Device Profile（鼠标、键盘等输入设备的通信进行无线化）
}CSUDIBTProfileType_E;

/**@brief 蓝牙适配器状态枚举值。*/
typedef enum _CSUDIBTAdapterStatus_E{
	EM_UDIBT_ADAPTER_STATUS_DOWN,		///< 关闭状态
	EM_UDIBT_ADAPTER_STATUS_UP			///< 开启状态
}CSUDIBTAdapterStatus_E;


/**@brief 蓝牙消息通知事件类型枚举值。*/
typedef enum _CSUDIBTEventType_E{
	EM_UDIBT_EVENT_TYPE_SCAN_OK,	///< 扫描设备成功,回调pvEventData为NULL
	EM_UDIBT_EVENT_TYPE_SCAN_FAILED,///< 扫描设备失败,回调pvEventData为NULL
	EM_UDIBT_EVENT_TYPE_CONNECT_OK,	///< 设备连接成功,回调pvEventData为CSUDIBTDeviceInfo_S指针
	EM_UDIBT_EVENT_TYPE_CONNECT_FAILED,	///< 设备连接失败,回调pvEventData为CSUDIBTDeviceInfo_S指针
	EM_UDIBT_EVENT_TYPE_CONNECT_LOST, ///< 连接丢失,回调pvEventData为CSUDIBTDeviceInfo_S指针
	EM_UDIBT_EVENT_TYPE_FOUND_DEVICE,	///< 发现设备,回调pvEventData为CSUDIBTDeviceInfo_S指针
	EM_UDIBT_EVENT_TYPE_PAIR_OK,	///< 配对成功,回调pvEventData为CSUDIBTDeviceInfo_S指针
	EM_UDIBT_EVENT_TYPE_PAIR_FAILED	///< 配对失败,回调pvEventData为CSUDIBTDeviceInfo_S指针
}CSUDIBTEventType_E;

/**@brief 蓝牙设备状态枚举值。*/
typedef enum _CSUDIBTDeviceStatus_E{
	EM_UDIBT_DEVICE_STATUS_NOTMATCH,		///< 未配对
	EM_UDIBT_DEVICE_STATUS_MATCHED,		///< 已配对但未连接
	EM_UDIBT_DEVICE_STATUS_CONNECTED,	///< 已连接
	EM_UDIBT_DEVICE_STATUS_NOTEXIST		///< 设备已不存在
}CSUDIBTDeviceStatus_E;


/**@brief 蓝牙适配器信息。*/
typedef struct _CSUDIBTAdapterInfo_S{   
    char szName[CSUDIBT_STR_LEN];		///< 蓝牙适配器名称
    char szAddress[CSUDIBT_STR_LEN];	///< 蓝牙适配器地址
}CSUDIBTAdapterInfo_S;

/**@brief 蓝牙设备信息。*/
typedef struct _CSUDIBTDeviceInfo_S{
	char szName[CSUDIBT_STR_LEN];		///< 蓝牙设备名称
    char szAddress[CSUDIBT_STR_LEN];	///< 蓝牙设备地址
    unsigned int nProfileMask;			///< 蓝牙设备profile属性，支持多种profile与运算
}CSUDIBTDeviceInfo_S;



/**@brief 蓝牙设备错误代码值*/
enum
{
	CSUDIBluetooth_ERROR_BAD_PARAMETER = CSUDI_BLUETOOTH_ERROR_BASE,    ///< 错误参数
	CSUDIBluetooth_ERROR_INVALID_DEVICE_ID,							///< 非法设备ID
	CSUDIBluetooth_ERROR_FEATURE_NOT_SUPPORTED,					///< 不支持此操作
	CSUDIBluetooth_ERROR_ADAPTER_NOT_UP,						///< 适配器未开启
	CSUDIBluetooth_ERROR_DEVICE_NOT_EXIST						///< 设备不存在
};
	
/**
@brief 蓝牙通知回调函数。
@param[in] hAdapter  事件通知的蓝牙适配器句柄。
@param[in] eEvent 事件类型
@param[in] pvEventData 事件数据
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 无。
*/
typedef void (*CSUDIBTCallback_F)(CSUDI_HANDLE hAdapter,CSUDIBTEventType_E eEvent,void* pvEventData,void * pvUserData);

/**
@brief 注册蓝牙事件监听函数
@param[in] fnCb  回调函数
@param[in] pUserData 用户自定义参数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTAddListener(CSUDIBTCallback_F fnCb,void* pUserData);

/**
@brief 获取所有蓝牙适配器
@param[out] ahOutHandleArray 输出蓝牙适配器句柄数组
@param[in] dwArraySize 数组大小
@param[out] pdwOutCount 输出蓝牙适配器个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 实际蓝牙设配器个数大于dwArraySize时，函数返回CSUDI_SUCCESS，且输出参数*pdwOutCount为dwArraySize
*/
CSUDI_Error_Code CSUDIBTGetAllAdapter(CSUDI_HANDLE *ahOutHandleArray,unsigned int dwArraySize,unsigned int* pdwOutCount);

/**
@brief 获取蓝牙适配器信息
@param[in] hAdapter 蓝牙适配器句柄
@param[out] psOutInfo 输出蓝牙适配器信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTGetAdapterInfo(CSUDI_HANDLE hAdapter,CSUDIBTAdapterInfo_S* psOutInfo);

/**
@brief 设置蓝牙适配器名称
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szName 设置的蓝牙适配器名称,字符串长度不能超过CSUDIBT_STR_LEN
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTSetAdapterName(CSUDI_HANDLE hAdapter, const char *szName);

/**
@brief 获取蓝牙适配器状态
@param[in] hAdapter 蓝牙适配器句柄
@param[out] peOutStatus 输出蓝牙适配器状态
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTGetAdapterStatus(CSUDI_HANDLE hAdapter,CSUDIBTAdapterStatus_E* peOutStatus);

/**
@brief 打开蓝牙适配器
@param[in] hAdapter 蓝牙适配器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTTurnOn(CSUDI_HANDLE hAdapter);

/**
@brief 关闭蓝牙适配器
@param[in] hAdapter 蓝牙适配器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTTurnOff(CSUDI_HANDLE hAdapter);


/**
@brief 获取所有蓝牙设备,包括曾经扫描到的设备
@param[in] hAdapter 蓝牙适配器句柄
@param[out] psOutDevicesArray 输出蓝牙设备信息数组
@param[in] dwArraySize 蓝牙设备信息结构体数组大小
@param[out] pdwOutCount 输出蓝牙设备个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 实际蓝牙设备个数大于nArraySize时，函数返回CSUDI_SUCCESS，且输出参数*pdwOutCount为dwArraySize
*/
CSUDI_Error_Code CSUDIBTGetDevices(CSUDI_HANDLE hAdapter,CSUDIBTDeviceInfo_S* psOutDevicesArray,unsigned int dwArraySize,unsigned int* pdwOutCount);


/**
@brief 删除蓝牙设备
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szDeviceAddress 蓝牙设备地址
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 成功删除的设备,其配对信息同步删除.必须重新扫描才可以通过CSUDIBTGetDevices接口获得。
*/
CSUDI_Error_Code CSUDIBTDeleteDevice(CSUDI_HANDLE hAdapter, const char* szDeviceAddress);


/**
@brief 启动扫描蓝牙设备
@param[in] hAdapter 蓝牙适配器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 扫描过程中，每扫描到一个蓝牙设备都会抛EM_UDIBT_EVENT_TYPE_FOUND_DEVICE消息，扫描完成或者扫描中止会抛EM_UDIBT_EVENT_TYPE_SCAN_OK消息。
*/
CSUDI_Error_Code CSUDIBTStartScan(CSUDI_HANDLE hAdapter);

/**
@brief 停止扫描蓝牙设备
@param[in] hAdapter 蓝牙适配器句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTStopScan(CSUDI_HANDLE hAdapter);


/**
@brief 启动蓝牙设备配对
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szDeviceAddress 需配对的蓝牙设备地址
@param[in] szPassword 配对密码字符串
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTStartPair(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,const char* szPassword);

/**
@brief 启动连接蓝牙设备
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szDeviceAddress 需连接的蓝牙设备地址
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 只有已配对的设备才能进行连接操作
*/
CSUDI_Error_Code CSUDIBTStartConnect(CSUDI_HANDLE hAdapter,const char* szDeviceAddress);

/**
@brief 获取蓝牙设备状态
@param[in] hAdapter 蓝牙适配器句柄
@param[in] szDeviceAddress 蓝牙设备地址
@param[out] peOutStatus 蓝牙设备状态
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTGetDeviceStatus(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,CSUDIBTDeviceStatus_E* peOutStatus);

/**
@brief 注册启用蓝牙服务sdp发现协议函数
@param[in] pcSerialID 序列号
@param[in] nPort 服务器的侦听端口
@param[in] pcName 服务名，与sdp服务的连接无关
@param[in] pcDsc 服务描述名，与sdp服务的连接无关
@param[in] pcProv 服务提供商名，与sdp服务的连接无关
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIBTSdpRegister(const char* pcSerialID, unsigned int dwPort, const char* pcName, const char* pcDsc, const char* pcProv);


#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



