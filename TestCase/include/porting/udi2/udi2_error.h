/**@defgroup ERROR ERROR 定义各个模块的错误代码基本值及通用出错代码

@brief Error模块对应头文件<udi2_error.h>，定义了各个模块的错误代码基本值、通用
成功代码CSUDI_SUCCESS及通用出错代码CSUDI_FAILURE，没有接口需要实现。

一般情况下，UDI所有的函数均返回本模块定义的CSUDI_Error_Code类型的错误代码值，
该类错误代码值由两部分组成，其高16位为模块代码，在本模块中定义（即错误代码基本
值）；低16位为具体错误代码，在具体模块中定义。

@note 除非在模块及接口中对错误代码值如FEATURE_NOT_SUPPORTED、NO_MEMORY等进行了
特别说明，否则错误代码值仅用于调试，Eastwin仅会判断其结果是否成功，而不会根据错
误代码值做出不同的后续操作。
@version 2.0.1 2009/07/27 初稿
@{
*/

#ifndef _UDI2_ERROR_H_
#define _UDI2_ERROR_H_

#include "udi2_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSUDI_SUCCESS   (0)     		///< 通用成功代码,所有一般成功均用该值表示

#define CSUDI_FAILURE   (0xFFFFFFFF)    ///< 一般失败代码,未定义的出错代码可使用该值，但建议返回实际的错误代码值，而不应该使用该值

typedef CSUDI_UINT32    CSUDI_Error_Code;  ///< 平台公共返回值类型

/**@brief 各个模块错误代码基本值枚举定义

@note 每个模块的错误代码值都会包含四个基本的错误代码类型:BAD_PARAMETER，NO_MEMORY,FEATURE_NOT_SUPPORTED,UNKNOWN_ERROR
*/
enum
{
	CSUDI_AOUT_ERROR_BASE			= 1 << 16,      ///< AOUT模块的错误代码基本值
	CSUDI_AUDIO_ERROR_BASE			= 2 << 16,		///< AUDIO模块的错误代码基本值
	CSUDI_VIDEO_ERROR_BASE			= 3 << 16,		///< VIDEO模块的错误代码基本值
	CSUDI_SCREEN_ERROR_BASE			= 4 << 16, 		///< SCREEN模块的错误代码基本值
	CSUDI_DESCRAMBLE_ERROR_BASE		= 5 << 16,		///< DESCRAMBLE模块的错误代码基本值
	CSUDI_INJECT_ERROR_BASE			= 6 << 16,		///< INJECTER模块的错误代码基本值
	CSUDI_RECORD_ERROR_BASE			= 7 << 16,		///< RECORD模块的错误代码基本值
	CSUDI_OSG_ERROR_BASE			= 8 << 16,		///< OSG模块的错误代码基本值
	CSUDI_SECTION_ERROR_BASE		= 9 << 16,		///< SECTION模块的错误代码基本值
	CSUDI_SMC_ERROR_BASE			= 10 << 16,		///< SMC模块的错误代码基本值
	CSUDI_OS_ERROR_BASE				= 11 << 16,		///< OS模块的错误代码基本值
	CSUDI_FS_ERROR_BASE				= 12 << 16,		///< FS模块的错误代码基本值
	CSUDI_PLAYER_ERROR_BASE			= 13 << 16,		///< PLAYER模块的错误代码基本值
	CSUDI_TUNER_ERROR_BASE			= 14 << 16,		///< TUNER模块的错误代码基本值
	CSUDI_FLASH_ERROR_BASE			= 15 << 16,		///< FLASH模块的错误代码基本值
	CSUDI_DEMUX_ERROR_BASE			= 16 << 16,		///< DEMUX模块的错误代码基本值
	CSUDI_EEPROM_ERROR_BASE			= 17 << 16,		///< EEPROM模块的错误代码基本值
	CSUDI_TOOLSET_ERROR_BASE		= 18 << 16,		///< TOOLSET模块的错误代码基本值
	CSUDI_PANEL_ERROR_BASE			= 19 << 16,		///< PANEL模块的错误代码基本值
	CSUDI_INPUT_ERROR_BASE			= 20 << 16,		///< INPUT模块的错误代码基本值
	CSUDI_IPCFG_ERROR_BASE			= 21 << 16,		///< IPCFG模块的错误代码基本值
	CSUDI_OTA_ERROR_BASE			= 22 << 16,   	///< OTA模块的错误代码基本值
	CSUDI_EW200S_ERROR_BASE			= 23 << 16,		///< Eastwin200服务层错误代码基本值
	CSUDI_VOUT_ERROR_BASE       	= 24 << 16,   	///< VOUT模块的错误代码基本值
	CSUDI_INVOKE_ERROR_BASE     	= 25 << 16,   	///< invokeNative模块的错误代码基本值
	CSUDI_UART_ERROR_BASE       	= 26 << 16,   	///< Uart模块的错误代码基本值
	CSUDI_PPPOE_ERROR_BASE      	= 27 << 16,   	///<PPPOE模块的错误代码基本值
	CSUDI_BLUETOOTH_ERROR_BASE		= 28 << 16,  	///<bluetooth模块的错误代码基本值
	CSUDI_USB_ERROR_BASE       		= 29 << 16,  	///<usb模块的错误代码基本值
	CSUDI_BOOTSHOW_ERROR_BASE       = 30 << 16,  	///<bootshow模块的错误代码基本值
	CSUDI_MAX_ERROR_BASE							///< 边界值
};

#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_ERROR_H_

