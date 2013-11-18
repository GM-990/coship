/**@defgroup CSTOOLSET toolset 移植层系统初始化、配置和控制
@brief 移植层系统初始化、配置和控制接口
- 系统初始化，整个系统的运行需要的模块初始化都在这里进行
- 配置功能，系统初始化时可以提供给用户的配置接口，包括驱动特性选择
- 重启功能，提供reboot功能

@version 0.3 2009/8/5
- 明确模块功能
- 明确不需要实现的接口
- 增加系统流程和调用示例代码
@version 0.2 2008/10/29
- 添加部分详细描述
@version 0.1 2008/10/22
- 初始版本
@{
*/
#ifndef _UDI_TOOLSET_H_
#define _UDI_TOOLSET_H_

#include "udi_typedef.h"
#include "udi_public.h"
#include "udi_video.h"
#include "udi_audio.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief system status information*/
typedef enum _CSSYSStatusInfo	
{	
	CSSYS_STATUS_BOOT,				/**< boot the Kernel*/
	CSSYS_STATUS_RUN,				/**< STB system runing on normally mode*/
	CSSYS_STATUS_HALT,				/**< STB system halt ，目前不使用此状态*/
	CSSYS_STATUS_REBOOT,			/**< reboot the Kernel */
	CSSYS_STATUS_STANDBY			/**< Standby the STB system*/
}CSSYSStatusInfo;

/**@brief system information*/
typedef struct _CSSYSInfo
{
	CSSYSStatusInfo		SysStatusInfo;	/**< system status information: boot/run/halt/reboot*/
	CSSYSServiceType	SysServiceInfo;	/**< play status: DVB/IPTV*/

	CSSYSAUDInputInfo	InputAUDInfo;	/**< Audio input stream information*/
	CSSYSVIDInputInfo	InputVIDInfo; 	/**< Video input stream information*/
	CSSYSAUDOutputInfo 	OutputAUDInfo;	/**< Audio ouput data information*/
	CSSYSTVOutputInfo 	OuputTVInfo;	/**< Video output data information*/
} CSSYSInfo;

typedef struct _CSSYSStandbyParams_S
{
	BOOL	m_bReal; /**< 是否为真待机 */
	DWORD 	m_dwStandbyTime; /**< 定时唤醒时间，仅对真待机有效 */
	DWORD 	m_dwReserved1;  /**< 扩展参数，目前必须为0 */
	DWORD 	m_dwReserved2; /**< 扩展参数，目前必须为0 */
}CSSYSStandbyParams_S;

/**
@brief 系统重启，应为硬件的硬重启

@param[in] hReserved 该参数保留，以便日后的功能扩展，目前不使用，必须为NULL。
@return 如果调用成功，返回调用TRUE；如果调用失败则返回FALSE
@note 推荐使用watckdog方式重启
*/
BOOL CSSYSChipReboot(HCSHANDLE hReserved);

/**
@brief 实现系统的关机,目前需实现stub函数，

@param[in] hReserved 该参数保留，以便日后的功能扩展，目前不使用，必须为NULL。
@return 如果调用成功，返回调用TRUE；如果调用失败则返回FALSE
@note 目前不需实现，实现系统自动关机功能，需要硬件支持
*/
BOOL CSSYSChipHalt(HCSHANDLE hReserved);	

/**
@brief 使系统进入待机状态或者退出待机状态，

@param[in] bStandbyStatus TRUE:进入待机状态;FALSE:退出待机状态，为FALSE，pstStandbyParams参数无效，请设为NULL
@param[in] hStandbyParams 为CSSYSStandbyParams_S结构本指针，为NULL按假待机处理；具体见CSSYSStandbyParams_S定义
@return 如果调用成功，返回调用TRUE；如果调用失败则返回FALSE
@note 当要求为假待机时，关闭AV DAC和HDMI等各输出接口
*/
BOOL CSSYSChipStandby(BOOL bStandbyStatus, const CSSYSStandbyParams_S* pstStandbyParams);	

/**
@brief 获得系统当前的各种状态信息

包括ES,以及video，audio的各种信息
@param[in] hHandle 设备通道句柄，系统初始化得到
@param[out] pstSysInfo 系统当前的信息
@param[in] hReserved 该参数保留，以便日后的功能扩展，目前不使用，必须为NULL。
@return 如果调用成功，返回调用TRUE；如果调用失败则返回FALSE
*/
BOOL CSSYSGetInfo(HCSHANDLE hHandle, CSSYSInfo* pstSysInfo, HCSHANDLE hReserved);

/*-------------以下接口不属于平台驱动提供部分-------------*/

/**
@brief UDI初始化接口，接口不属于平台驱动提供部分，不需要实现

统一初始化UDI中定义的模块，调用后UDI可用
@return 成功返回TRUE；失败返回FALSE
@note CITV3.0初始化时会调用本函数进行初始化操作
@note 该接口的实现根据项目不同而不同，一般情况下由集成者根据项目具体情况调用平台驱动实现
@note 接口不属于平台驱动提供部分，不需要实现
*/
BOOL CSUDIInit(void);

/**
@brief 获取系统环境信息，接口不属于平台驱动提供部分，不需要实现

如MAC地址、IP地址、VOD服务器地址等信息，类似环境变量系统
@param[in] key 唯一标识所需信息的字符串关键字
@return 成功返回对应的字符串值，失败返回NULL
@todo 具体需要哪些信息待确定
@note 该接口的实现根据项目不同而不同，由集成者根据项目具体情况实现
@note 接口不属于平台驱动提供部分，不需要实现
*/
const char* CSUDIGetEnv(const char* key);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif

