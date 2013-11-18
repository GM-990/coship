/**@defgroup OTA OTA 定义中间件与Loader交互接口

@brief OTA模块对应头文件udi2_ota.h，定义了中间件与Loader交互的接口，
中间件通过该模块获取OTA信息如硬件版本号，厂商ID等，告知Loader需要升级信息等

@note 由于本模块与应用层及BooterLoader关联较大，建议放在应用层或者Booterloader层实现

@note wifi升级包括两个部分wifi升级模块下载、wifi升级模块更新
其中wifi升级模块下载与项目中使用的OTA过程类似，需要OTA检测和升级
涉及到的接口:
CSUDI_Error_Code CSUDIOTAGetInfo(CSUDIOTAInfoType_E eType, void* value, int length);
CSUDI_Error_Code CSUDIOTASetInfo(CSUDIOTAInfoType_E eType, void* value, int length);
CSUDI_Error_Code CSUDIOTASetOta(const char* pUrl);

wifi升级模块更新
涉及到的接口:
CSUDI_Error_Code CSUDIOTAWIFIStart(void *pvUserData);
CSUDI_Error_Code CSUDIOTAWIFIAddCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData);
CSUDI_Error_Code CSUDIOTAWIFIDelCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData);
其中只有调用接口CSUDIOTAWIFIStart后中间件才能接收到wifi升级消息CSUDIOTAWIFIMsg_E

@version 2.0.0 2009/11/27 初稿
@{
*/

#ifndef _UDI2_OTA_H_
#define _UDI2_OTA_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief OTA错误代码值*/
enum
{
	CSUDIOTA_ERROR_BAD_PARAMETER = CSUDI_OTA_ERROR_BASE,  ///< 错误参数
	CSUDIOTA_ERROR_FEATURE_NOT_SUPPORTED,				  ///< 不支持此操作
	CSUDIOTA_ERROR_MEMORY_NOT_ENOUGH                      ///< 内存不足
};

/**@brief OTA相关信息类型枚举

定义了各种OTA方式需获取的信息，根据项目的不同，只需实现某一种即可。
若CA无特殊需求，一般实现NOR类；若为NDSCA则需要实现NDS类。
具体使用哪种OTA方式是项目相关的，需在集成中间件时根据运营商要求进行配置，
中间件会根据配置从UDI获取相关类型的OTA信息。
@note 说明中[R]代表该项可读，即会通过CSUDIOTAGetInfo接口读取该信息;[W]表示该项可写，即会通过CSUDIOTASetInfo接口写入;[RW]表示读写
@note NDS BCA,FDCA,FLASHHEAD结构及其含义等，请参考NDS相关文档。
*/
typedef enum
{
	EM_UDIOTA_NDS_BEGIN = 2,            ///< []NDS OTA上边界值，不会使用
	EM_UDIOTA_NDS_BCA,		            ///< [R]NDS BCA数据，CSUDI_UINT8*
	EM_UDIOTA_NDS_FDCA,                 ///< [RW]NDS FDCA数据，使用该参数进行写操作时需进行永久存储以备下次重启时决定是否进入Loader，CSUDI_UINT8*
	EM_UDIOTA_NDS_FLASHHEAD,            ///< [R]NDS FlashHead数据，CSUDI_UINT8*
	EM_UDIOTA_NDS_STANDBY_INFO,         ///< [RW]NDS Loader待机状态位,Loader及中间件直接需通过该接口交换当前是否为待机状态,用于满足NDS待机是OTA的需求,CSUDI_BOOL
	EM_UDIOTA_NDS_END = 0x100,          ///< []NDS OTA下边界值，不会使用

	EM_UDIOTA_NOR_BEGIN = 0x101,        ///< []普通OTA上边界值，不会使用
	EM_UDIOTA_NOR_MANUFACTURER_CODE,    ///< [R]普通OTA，厂商代码,int
	EM_UDIOTA_NOR_HARDWARE_CODE,        ///< [R]普通OTA，硬件版本号,int
	EM_UDIOTA_NOR_SOFTWARE_VERSION,     ///< [R]普通OTA，软件版本号,int
	EM_UDIOTA_NOR_HARDWARE_BATCHNO,		///< [R]普通OTA，硬件批次号,int
	EM_UDIOTA_NOR_END = 0x200,     		///< []普通OTA下边界值，不会使用

	EM_UDIOTA_IRDETO_BEGIN = 0x201,		///< []Irdeto OTA上边界值，不会使用
	EM_UDIOTA_IRDETO_CPCB,				/// [R] Irdeto loader 中CPCB结构字段， CSUDI_INT8*
	EM_UDIOTA_IRDETO_BBCB,				/// [R] Irdeto loader 中PBCB结构字段， CSUDI_INT8*
	EM_UDIOTA_IRDETO_KEYSTATUS,			/// [R] Irdeto loader 中KEYSTATUS结构字段， CSUDI_INT8*
	EM_UDIOTA_IRDETO_END = 0x300		///< []Irdeto OTA下边界值，不会使用
	
}CSUDIOTAInfoType_E;

typedef enum
{
	EM_UDIOTA_WIFI_SOFTWARE_START,  ///< WIFI模块升级开始，未定义，暂返回CSUDI_NULL
	EM_UDIOTA_WIFI_SOFTWARE_FINISH, ///< WIFI模块升级结束，未定义，暂返回CSUDI_NULL
	EM_UDIOTA_WIFI_SOFTWARE_FAILED, ///< WIFI模块升级失败，pMsgData为CSUDIOTAWIFIErrorType_E类型
	EM_UDIOTA_WIFI_CONFIG_START, ///< WIFI配置表更新开始，未定义，暂返回CSUDI_NULL
	EM_UDIOTA_WIFI_CONFIG_FINISH, ///< WIFI配置表更新结束，未定义，暂返回CSUDI_NULL
	EM_UDIOTA_WIFI_CONFIG_FAILED, ///< WIFI配置表更新失败，pMsgData为CSUDIOTAWIFIErrorType_E类型
	EM_UDIOTA_WIFI_ALL_FINISH, ///< WIFI升级完成，升级成功或失败都要发此消息，未定义，暂返回CSUDI_NULL
	EM_UDIOTA_WIFI_MAX ///< WIFI消息类型边界值
}CSUDIOTAWIFIMsg_E; 

typedef enum
{
	EM_UDIOTA_WIFI_ERROR_GET_URL_FAIL,  ///<获取URL失败
	EM_UDIOTA_WIFI_ERROR_URL_ERROR,  ///<URL错误
	EM_UDIOTA_WIFI_ERROR_HTTP_OPEN_FAIL,  ///<打开http服务器失败
	EM_UDIOTA_WIFI_ERROR_HTTP_LOAD_FAIL, ///< HTTP下载错误
	EM_UDIOTA_WIFI_ERROR_SHA256_CHECK_FAIL, ///< 下载数据sha256校验失败
	EM_UDIOTA_WIFI_ERROR_GETVERSION_FAIL, ///< 获取wifi版本失败
	EM_UDIOTA_WIFI_ERROR_VERSION_CHECK_FAIL, ///< wifi版本检查失败或是通过版本检测不需要升级
	EM_UDIOTA_WIFI_ERROR_READ_CONFIG_FAIL,  ///<读取wifi配置文件失败
	EM_UDIOTA_WIFI_ERROR_SET_RUN_TYPE_FAIL,  ///<设置wifi 为run type 失败
	EM_UDIOTA_WIFI_ERROR_SET_BACKUP_TYPE_FAIL,  ///<设置wifi为BACKUP  type 失败
	EM_UDIOTA_WIFI_ERROR_SET_CONFIG_FAIL,  ///<设置wifi配置文件失败
	EM_UDIOTA_WIFI_ERROR_CHECK_CONFIG_FAIL,  ///<检查wifi配置失败
	EM_UDIOTA_WIFI_ERROR_SEND_FAIL, ///< 向wifi发送数据失败
	EM_UDIOTA_WIFI_ERROR_MAX ///< wifi错误消息边界值
}CSUDIOTAWIFIErrorType_E; 


/**
@brief WIFI升级回调函数
@param[in] eMsg:详见CSUDIOTAWIFIMsg_E说明
@param[in] pvMsgData:根据不同的消息类型，回调具体的消息，详见CSUDIOTAWIFIMsg_E说明
@param[in] pvUserData: 用户传入自定义指针
@note:
*/
typedef void (*CSUDIOTAWIFICakllback_F)(CSUDIOTAWIFIMsg_E eMsg, void *pvMsgData, void *pvUserData);

/**
@brief 启动wifi升级
@param[out] pbWifiUpdate: CSUDI_TRUE:表示需要进行wifi升级，CSUDI_FALSE:表示不需要进行wifi升级
@return 成功返回CSUDI_SUCCESS，失败CSUDI_FAILURE
@note:只有调用此接口后中间件才能接收到wifi升级消息CSUDIOTAWIFIMsg_E
@note:使用场景，中间件启动后调用此接口，通过pbWifiUpdate判断wifi是否需要升级。
     若需要升级，启动后台新线程实现与wifi模块交互,更新wifi软件和配置文件。
     并在更新过程中将更新相关消息通过注册的回调函数抛给中间件。
*/
CSUDI_Error_Code CSUDIOTAWIFIStart(CSUDI_BOOL *pbWifiUpdate);

/**
@brief 注册wifi升级回调函数
@param[in] fnWifiCallback: 回调函数指针，详见：CSUDIOTAWIFICakllback_F
@param[in] pvUserData: 用户传入自定义指针，可以为NULL
@return 成功返回CSUDI_SUCCESS，失败CSUDI_FAILURE
@note:
*/
CSUDI_Error_Code CSUDIOTAWIFIAddCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData);

/**
@brief 删除wifi升级回调函数
@param[in] fnWifiCallback: 回调函数指针，详见：CSUDIOTAWIFICakllback_F
@param[in] pvUserData: 用户传入自定义指针，可以为NULL
@return 成功返回CSUDI_SUCCESS，失败CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIOTAWIFIDelCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData);

/**@brief 获取OTA信息接口

@param[in] eType 信息类型，详见CSUDIOTAInfoType_E
@param[out] value 指向存储获取信息的内存地址
@param[in] length value值的长度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note MMCP会根据运营商选择的OTA方式，调用本接口获取特定类型的OTA信息。

@code
//参考实现(未考虑缓冲区溢出等异常)
//注意:以下两种情况，value表示的类型及含义略有不同，请按参考代码实现/使用即可。
if(eType == EM_UDIOTA_NOR_MANUFACTURER_CODE)
{
	//EM_UDIOTA_NOR_MANUFACTURER_CODE对应数据类型是int的
	//value为一个指向int的指针，其长度应该为sizeof(int)
	//把对应值存储到value指向的buffer中
	*value = (int)100;
}
else if(eType == EM_UDIOTA_NDS_BCA)
{
	//EM_UDIOTA_NDS_BCA对应数据类型是CSUDI_UINT8数组
	//value一个指向CSUDI_UINT8数组，其长度为length
	//把对应的数据存储到value指向的buffer中
	memcpy(value,s_aBCAData,sizeof(s_aBCAData));
}
@endcode
*/
CSUDI_Error_Code CSUDIOTAGetInfo(CSUDIOTAInfoType_E eType, void* value, int length);

/**@brief 设置OTA信息接口

@param[in] eType 信息类型，详见CSUDIOTAInfoType_E
@param[in] value 指向存储获取信息的内存地址
@param[in] length value值的长度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note MMCP会根据运营商选择的OTA方式，调用本接口设置特定类型的OTA信息。该信息的时效性由具体选项决定。

@code
//参考实现(未考虑缓冲区溢出等异常)
//注意:以下两种情况，value表示的类型及含义略有不同，请按参考代码实现/使用即可。
if(eType == EM_UDIOTA_NOR_MANUFACTURER_CODE)
{
	//EM_UDIOTA_NOR_MANUFACTURER_CODE对应数据类型是int的
	//value为一个指向int的指针，其长度应该为sizeof(int)
	//从value指向的buffer中获取该值
	int manu_code = *(int*)value;
}
else if(eType == EM_UDIOTA_NDS_BCA)
{
	//EM_UDIOTA_NDS_BCA对应数据类型是CSUDI_UINT8数组
	//value一个指向CSUDI_UINT8数组，其长度为length
	//从value指向的buffer中获取对应内容
	memcpy(s_aBCAData,value,length);
}
@endcode
*/
CSUDI_Error_Code CSUDIOTASetInfo(CSUDIOTAInfoType_E eType, void* value, int length);

/**@brief 设置OTA信息.

普通OTA模式会通过该接口通知Loader需要启动OTA，并将必要信息存储，之后MMCP会随时重启终端，下次启动时直接进入Loader。
@param[in] pUrl 升级信息，描述升级的位置，如频点,IP地址等。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note NDS OTA模式时该接口无效，MMCP会自行完成NDS数据结构如FDCA等的修改，并在必要时重启，Booter需根据NDS规则在启动时判断这些结构
@note 目前支持的URL有：
- dvb://networkid.tsid.serviceid，如dvb://1000.100.10。
- dvbc://qamfreq.SymbolRate.Modulation，如dvbc://363000.6875.64
- http://ipurl/，如http://www.coship.com/otafile.bin。
- tftp://ipurl/，如tftp://ftp.coship.com/otafile.bin。
- wifi://upgrade?ipurl/, 如wifi://upgrade?http://www.downloadWifi.com/wifi.bin?ver=4.1.0.20&upgradeType=1
其中所有整数数据均为10进制。
@note URL后面还可能附带一些可选的升级参数，如dvbc://363000.6875.64?pid=100&tableid=254目前需支持的参数有：
- pid，表示升级数据所在pid，10进制整数，如pid=100。
- tableid，表示升级数据所在TableId,10进制整数，如tableid=254。
- md5,表示升级数据的md5校验值,10进制，如md5=2b166501e6b8a40911975c54d3c4297b。
- version,表示升级后的软件版本，10进制，如version=100。
- type，表示升级类型，字符串，如type=auto,type=manual。
@note 具体需要支持哪些类型的URL需根据项目及CA需求确定。
@note pUrl为CSUDI_NULL表示取消升级。
@note 该接口仅仅是设置OTA信息，完毕后并不执行重启动作。重启动作由MMCP应用显式调用
*/
CSUDI_Error_Code CSUDIOTASetOta(const char* pUrl);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

