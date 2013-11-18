/**@defgroup CSSCART [CSSCART]同洲项目专用SCART接口
@brief 本模块主要定义了同洲项目专用平台无关QAM驱动接口
@brief  SCART是欧洲强制要求用于完成卫星电视接收机、电视机、录象机和其他音视频设备的互连互通接口。标准的SCART接口为21针连接器,同时定义了音频和视频信号传输通道。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_SCART_H_
#define _CS_SCART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "CSHDI_typedef.h"
#include "cs_typedef.h"
#include "cs_assert.h"

/**@brief SCART的控制模式*/
typedef enum
{
	SCART_IC,	/**<IC控制*/
	SCART_HW	/**<硬件控制*/
}T_SCART_TYPE;



/**@brief GPIO的具体配置*/
typedef struct
{
	int	m_nGPIO16_9;			/**<设置16:9或4:3*/
	int	m_nGPIOTV_AV;			/**<设置tv av输出*/
	int	m_nGPIOSlow_SW;			/**<是否连接了VCR*/
	int	m_nGPIOFastBlank;		/**<输出为RGB或s缎子*/
	int	m_nGPIOVCR_SW;			/**<信号来源*/
}T_SCART_GPIO_CFG;

/**@brief GPIO的具体配置*/
typedef struct
{
	int	m_nGPIO16_9;			/**<设置16:9或4:3*/
	int	m_nGPIOTV_AV;			/**<设置tv av输出*/
	int	m_nGPIOSlow_SW;			/**<是否连接了VCR*/
	int	m_nGPIOFastBlank;		/**<输出为RGB或s缎子*/
	int	m_nGPIOVCR_SW;			/**<TV scart 信号来源, STB信号或VCR信号*/
	int   m_nGPIOTV_SW;                      /**<VCR scart 信号来源, STB信号或TV信号*/
	DWORD   m_dwReserved1;                         /**<保留位*/
	DWORD   m_dwReserved2;				/**<保留位*/
}T_SCART_GPIO_CFG_EX;

/**@brief Scart模式*/
typedef enum
{
	SCART_TV, 					/**<TV　Scart*/
	SCART_VCR					/**<VCR　Scart*/
}T_SCART_INDEX;

/**@brief 输出信号的来源*/
typedef enum
{
	SOURCE_VCR,			 		/**<信号来源于VCR*/
	SOURCE_ENCODER,				/**<信号来源于CPU*/
	SOURCE_AUX,					/**<信号来源于辅助设备*/
	SOURCE_TV                                      /**<信号来源于TV SCART*/
}T_SIGNAL_SOURCE;

/**@brief 显示模式*/
typedef enum
{
	DISPLAY_4_3,				/**<4:3模式*/
	DISPLAY_16_9				/**<16:9模式*/
}T_DISPLAY_MOD;

/**@brief 工作模式*/
typedef enum  
{
	MOD_AV,						/**<AV模式*/
	MOD_TV						/**<TV模式*/
}T_TV_AV_MOD;

/**@brief 颜色模式*/
typedef enum
{
	COLOR_RGB,					/**<RGB模式模式*/
	COLOR_CVBS					/**<CVBS模式*/
}T_COLOR_MOD;


typedef enum
{
	MSG_VCR_ON,	/**< 需要添加说明 */
	MSG_VCR_OFF,	/**< 需要添加说明 */
	MSG_TV_ON,		/**< 需要添加说明 */
	MSG_TV_OFF
}T_SCART_POWER_STATUS_MSG;

typedef enum
{
	VCR_OFF_TV_OFF,	/**< 需要添加说明 */
	VCR_OFF_TV_ON,		/**< 需要添加说明 */
	VCR_ON_TV_OFF,		/**< 需要添加说明 */
	VCR_ON_TV_ON		/**< 需要添加说明 */
}T_SCART_POWER_STATUS;

/**@brief VCR引脚状态*/
typedef enum
{
	H_FOR_VCR_ON,				/*GPIO为1表示VCR ON状态*/
	L_FOR_VCR_ON				/*GPIO为0表示VCR ON状态*/
}T_SCART_VCR_STATUS;

/**
@brief 配置Scart用到的i2c,该函数必须在初始化前调用

@param[in] I2CIndex I2c索引
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE
*/
CSHDI_Error_t CSScartConfigI2CIndex(CSHDII2CIndex I2CIndex);

/**
@brief 对Scart进行初始化

@param[in] type 控制类型, SCART_IC：用IC控制的scart,SCART_HW：用GPIO控制的scart
@param[in] PIOCfg  当Scart为GPIO控制时，该参数为控制GPIO的具体配置情况（如果类型为SCART_IC，该参数无效）
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE，如果参数错误返回CSHDI_ERROR_BAD_PARAMETER，如果如果已经初始化过返回CSHDI_ERROR_ALREADY_INITIALIZED
*/
CSHDI_Error_t CSScartInitialize( T_SCART_TYPE type, T_SCART_GPIO_CFG * PIOCfg );



/**
@brief 对Scart进行初始化

@param[in] type 控制类型, SCART_IC：用IC控制的scart,SCART_HW：用GPIO控制的scart
@param[in] pPIOCfgEx  当Scart为GPIO控制时，该参数为控制GPIO的具体配置情况,见T_SCART_GPIO_CFG_EX（如果类型为SCART_IC，该参数无效）
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE，如果参数错误返回CSHDI_ERROR_BAD_PARAMETER，如果如果已经初始化过返回CSHDI_ERROR_ALREADY_INITIALIZED
*/
CSHDI_Error_t CSScartInitialize_Ext( T_SCART_TYPE type, T_SCART_GPIO_CFG_EX* pPIOCfgEx );

/**
@brief 根据硬件设置表示VCR状态时的电平值

@param[in] type 控制类型,SCART_IC：用IC控制的scart,SCART_HW：用GPIO控制的scart
@param[in] enmVCRStatus VCR引脚状态
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE，如果参数错误返回CSHDI_ERROR_BAD_PARAMETER，如果控制类型未知返回CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartSetVCRStatus( T_SCART_TYPE type, T_SCART_VCR_STATUS enmVCRStatus );

/**
@brief 控制TV Scart 输出信号的来源

@param[in] SignalSource 显示模式，可选值
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE，如果参数错误返回CSHDI_ERROR_BAD_PARAMETER，如果控制类型未知返回CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartSignalSourceSwitch( T_SIGNAL_SOURCE SignalSource );

/**
@brief 控制Scart Scart 输出信号的来源

@param[in] Scart Scart模式，可选值
@param[in] SignalSource 显示模式，可选值
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE，如果参数错误返回CSHDI_ERROR_BAD_PARAMETER，如果控制类型未知返回CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartExtSignalSourceSwitch(  T_SCART_INDEX Scart, T_SIGNAL_SOURCE SignalSource );

/**
@brief 控制Scart Scart连接电视的显示模式

@param[in] Scart Scart模式，可选值
@param[in] DisplayMode 显示模式，可选值
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE，如果参数错误返回CSHDI_ERROR_BAD_PARAMETER，如果控制类型未知返回CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScart16_9Switch( T_SCART_INDEX Scart,  T_DISPLAY_MOD DisplayMode);

/**
@brief 控制Scart Scart连接电视的工作模式

@param[in] Scart Scart模式，可选值
@param[in] TVWorkMode 工作模式，可选值
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE，如果参数错误返回CSHDI_ERROR_BAD_PARAMETER，如果控制类型未知返回CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartTV_AVSwitch( T_SCART_INDEX Scart, T_TV_AV_MOD TVWorkMode);

/**
@brief 控制Scart Scart连接电视的颜色模式

@param[in] Scart Scart模式，可选值
@param[in] ColorMode 颜色模式，可选值
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE，如果参数错误返回CSHDI_ERROR_BAD_PARAMETER，如果控制类型未知返回CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartColorModeSwitch( T_SCART_INDEX Scart, T_COLOR_MOD ColorMode);

/**
@brief 注册回调函数，驱动通过该回调函数来通知应用是否有VCR接入

@param[in] function 要注册的回调函数
@return 如果成功返回CSHDI_SUCCESS;如果失败返回CSHDI_FAILURE
*/
CSHDI_Error_t CSScartRegistCallBack( CSHDICallback function);

/**
@brief 获取SCART的驱动版本号

@param[in] nSize 版本号长度
@param[out] pchVer 版本号存放地址
@return 如果pchVer为空或者调用失败则返回0，如果pchVer为非空返回版本号信息长度
*/
int CSSCARTGetVersion( PCHAR pchVer, int nSize );


#ifdef __cplusplus
}
#endif
/** @} */

#endif

