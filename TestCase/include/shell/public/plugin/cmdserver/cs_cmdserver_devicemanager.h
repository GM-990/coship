#ifndef _CS_CMDSERVER_DEVICEMANAGER_H_
#define _CS_CMDSERVER_DEVICEMANAGER_H_

#include "cs_cmdserver.h"
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
@brief 设备类型
*/
typedef enum 
{
	EM_CMDSERVER_DEVMNG_USB,
	EM_CMDSERVER_DEVMNG_ETH
}CSCMDSERVERDeviceType_E;

/**
@brief 设备状态类型
*/
typedef enum
{
	EM_CMDSERVER_DEVMNG_USB_FREE,           /* USB is working normally */
	EM_CMDSERVER_DEVMNG_USB_RUNNING,             /* USB is not inserted */
	EM_CMDSERVER_DEVMNG_ETH_FREE,        /* Network is working normally */
	EM_CMDSERVER_DEVMNG_ETH_RUNNING        /* Network cable is not inserted*/
} CSCMDSERVERDeviceState_E;

/**
@brief 设备信息定义
*/
typedef struct _CSDEVICEInfo
{
	char      	m_szDeviceName[128]; ///< 设备的唯一设备名称，如果是网口设备应说明网口名etn0或eth1
	char 	m_szMountPoint[256];  ///挂载点，对于网口设备没有挂载点
	CSCMDSERVERDeviceType_E m_eDeviceType;
	CSCMDSERVERDeviceState_E m_eDeviceState;
} CSCMDServerDeviceInfo_S;

/**
@brief 定义设备状态通知函数原形

@param[in] sMsgType 要获取设备信息的设备类型,详细定义见CSCMDSERVERDeviceType_E说明
@param[in] ppsDeviceInfo 指定设备类型的所有状态信息,详细定义见CSCMDServerDeviceInfo_S说明
@param[in/out] pnDeviceCount 设备个数，usb接口个数或是eth个数，默认是4个
@param[out] pvUserData 用户数据,调用CSCMDServerDevMngAddCallBack注册该回调时传入的pvUserData。
@return 获取设备状态成功返回1,失败返回0，如果pnDeviceCount设备个数不足返回-1
@note
-该回调函数由具备能获取设备状态的相关模块实现，当收到通知后，根据sMsgType的类型\n
-获取设备能力信息，将设备能力信息通过psDeviceInfo返回，具体的设备能力信息见CSCMDServerDeviceInfo_S	\n
*/
typedef int (*CSOnCMDServerDeviceManager_F)(CSCMDSERVERDeviceType_E sMsgType, CSCMDServerDeviceInfo_S** ppsDeviceInfo, int* pnDeviceCount, void* pvUserData);

 /**
@brief 注册获取设备状态改变消息回调

@param[in] fnDevMngCallBack回调函数,由具备能获取设备状态的相关模块实现
@param[in] pvUserData用户数据,将作为回调函数的pUserData参数返回 
@return 如果成功返回TRUE,否则返回FALSE
@note
- 
*/
BOOL CSCMDServerDevMngAddCallBack(CSOnCMDServerDeviceManager_F fnDevMngCallBack, void* pvUserData);

 /**
@brief 删除注册的fnDevMngCallBack回调函数

@param[in] fnDevMngCallBack回调函数
@param[in] pvUserData用户数据,将作为回调函数的pUserData参数返回 
@return 如果成功返回TRUE,否则返回FALSE
@note
- 
*/
BOOL CSCMDServerDevMngDelCallBack(CSOnCMDServerDeviceManager_F fnDevMngCallBack, void* pvUserData);
 
   /**
@brief 当设备状态有变化时主动通知DeviceManager模块

@param[in] sMsgType 设备类型
@param[in] psDeviceInfo 设备状态信息
@param[in] pvUserData 私有参数，做为保留 
@return 如果成功返回TRUE,否则返回FALSE
@note 该接口由应用主动调用，或有能获取设备状态变化的模块调用
- 
*/
BOOL  CSCMDServerDeviceStateNotify(CSCMDSERVERDeviceType_E sMsgType, CSCMDServerDeviceInfo_S* psDeviceInfo, void* pvUserData);
   
/**
@brief 得到设备管理模块的实例

@param[in] 无
@return 成功返回对应插件实例的结构体，详见CSCmdInterface_S，失败返回NULL
@note 
- 详细描述可参见cs_cmdserver_avplayer.h
*/
CSCmdInterface_S* CSCMDServerGetDeviceManagerInstance( void );

/**
@brief初始化设备管理插件

@param[in] 无
@return 成功返回1，失败返回0
@note 
- 详细描述可参见cs_cmdserver_avplayer.h
*/
int CSPluginCMDServer_DeviceManagerInit(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

