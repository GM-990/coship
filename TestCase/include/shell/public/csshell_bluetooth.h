/**@defgroup 蓝牙接口，csshell_bluetooth，模块简称：CSSHELLBT
	本机的蓝牙设备称为蓝牙适配器(Adapter)，网络中其他蓝牙设备称为蓝牙设备(Device)
	
模块简称BT
@version 2.0.1 2011/09/14 为了保证中间件的统一性，定义了作为蓝牙client的接口,这一套接口的定义使得
即使蓝牙的udi接口在某些平台上没有实现，也不会存在链接不过的问题。
@{
*/
#ifndef _CSSHELL_BLUETOOTH_H_
#define _CSSHELL_BLUETOOTH_H_

#include "udi2_bluetooth.h"
#include "mmcp_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct 
{
	int m_nSize;	///<本结构大小，一定等于sizeof(CSShellBTInitParam_S)


	/**
	@brief 注册蓝牙事件监听函数
	@param[in] fnCb  回调函数
	@param[in] pUserData 用户自定义参数
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	// 可以注册多回调
	CSUDI_Error_Code (* m_fnAddListener)(CSUDIBTCallback_F  fnCb, void* pUserData);
	
	/**
	@brief	 删除蓝牙事件回调函数。
	@param[in] fnCb  回调函数
	@param[in] pUserData 用户自定义参数
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnRemoveListener)(CSUDIBTCallback_F fnCb,void* pUserData);

	/**
	@brief 获取所有蓝牙适配器
	@param[out] ahOutHandleArray 输出蓝牙适配器句柄数组
	@param[in] arraySize 数组大小
	@param[out] pnOutCount 输出蓝牙适配器个数
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note 实际蓝牙设配器个数大于arraySize时，函数返回CSUDI_SUCCESS，且输出参数*pnOutCount为arraySize
	*/
	CSUDI_Error_Code (* m_fnGetAllAdapter)(CSUDI_HANDLE *ahOutHandleArray,unsigned int arraySize,unsigned int* pnOutCount);
	
	/**
	@brief 获取蓝牙适配器信息
	@param[in] hAdapter 蓝牙适配器句柄
	@param[out] psOutInfo 输出蓝牙适配器信息
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnGetAdapterInfo)(CSUDI_HANDLE hAdapter,CSUDIBTAdapterInfo_S* psOutInfo);

	/**
	@brief 获取蓝牙适配器状态
	@param[in] hAdapter 蓝牙适配器句柄
	@param[out] peOutStatus 输出蓝牙适配器状态
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnGetAdapterStatus)(CSUDI_HANDLE hAdapter ,  CSUDIBTAdapterStatus_E* peOutStatus);
	
	/**
	@brief 打开蓝牙适配器
	@param[in] hAdapter 蓝牙适配器句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnTurnOn)(CSUDI_HANDLE hAdapter);
	
	/**
	@brief 关闭蓝牙适配器
	@param[in] hAdapter 蓝牙适配器句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnTurnOff)(CSUDI_HANDLE hAdapter);
	
	/**
	@brief 获取已扫描到的所有蓝牙设备
	@param[in] hAdapter 蓝牙适配器句柄
	@param[out] asOutDevicesArray 输出蓝牙设备信息数组
	@param[in] nArrayCount 蓝牙设备信息结构体数组大小
	@param[out] pnOutCount 输出蓝牙设备个数
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note 实际蓝牙设备个数大于nArraySize时，函数返回CSUDI_SUCCESS，且输出参数*pnOutCount为arraySize
	*/
	CSUDI_Error_Code (* m_fnGetDevices)(CSUDI_HANDLE hAdapter,CSUDIBTDeviceInfo_S* asOutDevicesArray,unsigned int nArraySize,unsigned int* pnOutCount);
	
	/**
	@brief 启动扫描蓝牙设备,同步接口
	@param[in] hAdapter 蓝牙适配器句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回CSUDIBluetooth_ERROR_SCAN_FAILED
	*/
	CSUDI_Error_Code (* m_fnStartScan)(CSUDI_HANDLE hAdapter);
	
	/**
	@brief 停止扫描蓝牙设备
	@param[in] hAdapter 蓝牙适配器句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnStopScan)(CSUDI_HANDLE hAdapter);

	/**
	@brief 启动蓝牙设备配对
	@param[in] hAdapter 蓝牙适配器句柄
	@param[in] szDeviceAddress 需配对的蓝牙设备地址
	@param[in] szPassword 配对密码字符串
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnStartPair)(CSUDI_HANDLE  hAdapter,const char* szDeviceAddress,const char* szPassword);

	/**
	@brief 启动连接蓝牙设备
	@param[in] hAdapter 蓝牙适配器句柄
	@param[in] szDeviceAddress 需连接的蓝牙设备地址
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note 只有已配对的设备才能进行连接操作
	*/
	CSUDI_Error_Code (* m_fnStartConnect)(CSUDI_HANDLE  hAdapter,  const char* szDeviceAddress);

	/**
	@brief 删除蓝牙设备
	@param[in] hAdapter 蓝牙适配器句柄
	@param[in] szDeviceAddress 蓝牙设备地址
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note 成功删除的设备,其配对信息同步删除.必须重新扫描才可以通过CSUDIBTGetDevices接口获得。
	*/
	CSUDI_Error_Code (* m_fnStartDelete)(CSUDI_HANDLE hAdapter, const char* szDeviceAddress);

	/**
	@brief 获取蓝牙设备状态
	@param[in] hAdapter 蓝牙适配器句柄
	@param[in] szDeviceAddress 蓝牙设备地址
	@param[out] peOutStatus 蓝牙设备状态
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnGetDeviceStatus)(CSUDI_HANDLE hAdapter,const char* szDeviceAddress,CSUDIBTDeviceStatus_E* peOutStatus);
	
	/**
	@brief 注册启用蓝牙服务sdp发现协议函数
	@param[in] pcSerialID 序列号
	@param[in] nPort 服务器的侦听端口
	@param[in] pcName 服务名，与sdp服务的连接无关
	@param[in] pcDsc 服务描述名，与sdp服务的连接无关
	@param[in] pcProv 服务提供商名，与sdp服务的连接无关
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnSdpRegister) (const char* pcSerialID, unsigned int nPort, const char* pcName, const char* pcDsc, const char* pcProv);

}CSShellBTInitParam_S;

/**
@brief 设置 BLUETOOTH 接口参数
@param psParam  BLUETOOTH  Shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellBTSetParam(CSShellBTInitParam_S * psParam);

/**
@brief 获取 BLUETOOTH 接口参数
@param 无
@成功返回CSShellBTInitParam_S,失败返回NULL
*/
const CSShellBTInitParam_S* CSShellBTgetParam();

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



