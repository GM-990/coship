/**@defgroup TOOLSET_PLUS TOOLSET_PLUS主要定义平台初始化相关接口
@brief 定义平台初始化相关接口

@version 2.0.1 2009/10/21 初稿
@{
*/

#ifndef UDI2_TOOLSET_PLUS_H
#define UDI2_TOOLSET_PLUS_H

#include "udi2_toolset.h"
#include "cs_ir_plus.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef enum
{
        CSUDISYS_HDCP_OPEN,		/**< 打开HDCP */
        CSUDISYS_HDCP_CLOSE		/**< 关闭HDCP */
}CSUDISYSHdcpMode_E;

typedef enum
{
	CSUDISYS_MEM_PARTITION_HEAP,	/**< 系统堆分区 */
	CSUDISYS_MEM_PARTITION_AVMEM	/**< AVMEM分区，用于分配Surface等 */
}CSUDISYSMemPartition_E;

typedef enum
{
	/** CSUDICFG_TUNER_MODE
	Tuner串行并行, CSUDIPlatformInit之前设置
	CSUDISYSSetCfg参数说明:
		eCfgType:		CSUDICFG_TUNER_MODE
		nDeviceIndex: 	tuner index
		unValue1:		CSHDITsMode枚举
		unValue1:		0
	*/
	CSUDICFG_TUNER_MODE ,	

	/** CSUDICFG_TUNER_TSIN
	设置TUNER对应的TSIN, CSUDIPlatformInit之前设置
	CSUDISYSSetCfg参数说明:
		eCfgType:		CSUDICFG_TUNER_TSIN
		nDeviceIndex: 	tuner index
		unValue1:		TSIN index
		unValue1:		0
	*/
	CSUDICFG_TUNER_TSIN, 
	
	/** CSUDICFG_TSIN_DMX
	设置TSIN对应的DMX, CSUDIPlatformInit之前设置
	CSUDISYSSetCfg参数说明:
		eCfgType:		CSUDICFG_TSIN_DMX
		nDeviceIndex: 	TSIN index
		unValue1:		Demux index
		unValue1:		0
	*/
	CSUDICFG_TSIN_DMX,

	/** CSUDICFG_HDMI_I2C
	设置HDMI对应的I2C, CSUDIPlatformInit之前设置
	CSUDISYSSetCfg参数说明:
		eCfgType:		CSUDICFG_HDMI_I2C
		nDeviceIndex: 	HDMI index, 一般为0
		unValue1:		I2C index
		unValue1:		0
	*/
	CSUDICFG_HDMI_I2C,

	/** CSUDICFG_HDMI_PIO
	设置HDMI对应的I2C, CSUDIPlatformInit之前设置
	CSUDISYSSetCfg参数说明:
		eCfgType:		CSUDICFG_HDMI_PIO
		nDeviceIndex: 	HDMI index, 一般为0
		unValue1:		GPIO index
		unValue1:		0
	*/
	CSUDICFG_HDMI_PIO,

	/** CSUDICFG_HDMI_HDCP
	设置平台初始化时是否打开HDCP 功能, CSUDIPlatformInit之前设置
	CSUDISYSSetCfg参数说明:
		eCfgType:		CSUDICFG_HDMI_HDCP
		nDeviceIndex: 	HDMI index, 一般为0
		unValue1:		CSUDISYSHdcpMode_E,详情见该枚举的说明
		unValue1:		0
	*/
	CSUDICFG_HDMI_HDCP,

	/** CSUDICFG_SCREEN_RESOLUTION
	设置平台初始化时的分辩率,  CSUDIPlatformInit之前设置
	CSUDISYSSetCfg参数说明:
		eCfgType:		CSUDICFG_SCREEN_RESOLUTION
		nDeviceIndex: 	CSUDISCREENType_E, 具体见该枚举的说明
		unValue1:		CSUDISCREENResolution_E, 具体见该枚举的说明
		unValue1:		0
	*/
	CSUDICFG_SCREEN_RESOLUTION,

	/*CSUDICFG_MEMORY_PARTATION
	设置内存分区
	CSUDISYSSetCfg 参数说明：
	eCfgType	：CSUDICFG_MEMORY_PARTATION
	nDeviceIndex	：CSUDISYSMemPartition_E枚举
	unValue1	：分区起始地址
	unValue1	：分区长度
	*/
	CSUDICFG_MEMORY_PARTATION,

	/*CSUDICFG_PIXEL_FORMAT
	设置显存颜色模式
	CSUDISYSSetCfg 参数说明：
	eCfgType	：CSUDICFG_PIXEL_FORMAT
	nDeviceIndex	：默认为0
	unValue1	：CSUDIOSGPixelFormat_E，具体见该枚举的说明
	unValue1	：0
	*/
	CSUDICFG_PIXEL_FORMAT,
	
	/*CSUDICFG_BOARD_NO
	设置主板号
	CSUDISYSSetCfg 参数说明：
	eCfgType	：CSUDICFG_BOARD_NO
	nDeviceIndex	：不关注，无意义
	unValue1	：主板号字符串指针
	unValue2	：不关注，无意义
	*/
	CSUDICFG_BOARD_NO,

	CSUDICFG_MAX_NUM
}CSUDISYSCfg_E;

/**@brief 定义真待机唤醒模式*/
typedef enum 
{
	CSUDISYS_WAKEUPE_MODE_IR /**< 通过遥控器唤醒*/
}CSUDISYSWakeUpMode_E;

/**@brief 定义遥控器键值结构*/
typedef struct
{
	CSIRProtocol_t	m_enmIRProtocol; 	/**< 协议 */
	unsigned int 		m_nUserID; 				/**< 遥控器厂家码，用于识别遥控器类型 */
	unsigned int 		m_nKeyCode;			/**< 遥控器按键的电气编码 */
}CSUDISYSWakeUpKey_S;

#define CSUDI_MAX_WAKEUP_KEY_NUM 16

/**@brief 定义真待机唤醒参数*/
typedef struct
{
	CSUDISYSWakeUpMode_E m_eWakeUpMode; 	/**< 唤醒模式 */
	BOOL 	m_bModeEnable;	/**< 是否启用这种模式 */
	union 
	{
		struct
		{
			CSUDISYSWakeUpKey_S m_astKeys[CSUDI_MAX_WAKEUP_KEY_NUM];
			int m_nKeyNum;
		} 
		m_stIRKeys; /**< IR唤醒模式对应的键值参数 */
	} 
	m_uWakeupParams; /**< 唤醒模式对应的参数， m_bModeEnable为FALSE时，忽略这些参数*/
}CSUDISYSWakeupParams_S;

/**
@brief 移植层配置接口，调用时机由各eCfgType决定,具体见CSSYSCfg_E定义

该接口可被调用多次，同一类型的配置以最后一次的调用为准
@param[in] eCfgType 配置类型,具体见CSSYSCfg_E定义;
@param[in] nDeviceIndex 所要配置的设备index,如果该eCfgType无对应的设备时,传入-1;
@param[in] unValue1	配置参数1,具体含义由eCfgType决定;
@param[in] unValue2 配置参数2,具体含义由eCfgType决定;
@return 配置成功返回CSUDI_SUCCESS ，配置失败返回错误值。
*/
CSUDI_Error_Code CSUDISYSSetCfg(CSUDISYSCfg_E eCfgType, int nDeviceIndex, CSUDI_UINT32 unValue1, CSUDI_UINT32 unValue2);

/**
@brief 移植层系统初始化接口，实现系统的分区划分，并启动操作系统，系统启动时第一个被调用

@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误值。
*/
CSUDI_Error_Code CSUDISYSInit(void);

/**
@brief 移植层系统初始化接口

该接口只可被调用一次，必须在CSUDISYSInit后调用
@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误值。
@note 
- 初始化机顶盒主芯片相关设备驱动，如AV decoder，Demux等等，并初始化相应的移植层模块，
	如Player,Inject,Audio,Video,Screen,Demux,Section等，初始化之后调用.用户不需要关心各设备的初始化顺序。
	初始化的配置参数使用CSUDISYSSetCfg设置，如果没有设置将按默认值初始化。
*/
CSUDI_Error_Code CSUDIPlatformInit(void);

/**
@brief 设置真待机唤醒参数

@param[in] pstStandbyParams 见CSUDISYSStandbyParams_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDISYSSetWakeupParams(const CSUDISYSWakeupParams_S* pstWakeupParams);

/**
@brief 芯片待机的钩子函数定义
@param[in] bStandbyStatus 	TRUE:进入待机状态;FALSE:退出待机状态
@param[in] nStandbyTime  待机时间
@param[out] pbSkipDefault 是否忽略默认操作，TRUE表示只执行Hook函数定义的操作，FALSE表示同时执行CSUDISYSChipStandby原有操作，默认为TRUE
@return CSUDI_SUCCESS 表示钩子函数执行成功，其它为返回的错误代码值。
*/
typedef CSUDI_Error_Code (*CSUDIChipStandbyHookFunc_F)(CSUDI_BOOL bStandby,int nStandbyTime, CSUDI_BOOL *pbSkipDefault);

/**
@brief 设置芯片待机的钩子函数

@param[in] fnHook 钩子函数，见CSUDIChipStandbyHookFunc_F定义
@return 如果调用成功，返回调用CSUDI_SUCCESS；如果调用失败则返回错误代码值。
@note fnHook可以为NULL，表示取消钩子函数
*/
CSUDI_Error_Code CSUDISYSSetChipStandbyHook(CSUDIChipStandbyHookFunc_F fnHook); 

/**
@brief 设置引导信息

根据引导类型设置额外的引导信息，用于在CSUDIToolsetExec时使用相同的目标代码完成不同的工作。
如loader和app可以使用相同Linux内核的两个拷贝，但根据本接口设置的引导参数加载不同的rootfs。
@param[in] eType 引导信息
@param[in] pExecParam 引导参数，具体意义由CSUDIToolsetExec自行决定，并告知初始化模块设置，调用者必须保证所指向内存一直有效。
@return 调用成功返回CSUDI_SUCCESS ，调用失败返回错误代码
@see CSUDIToolsetExec
*/
CSUDI_Error_Code CSUDIToolsetSetExecInfo(CSUDIToolsetExecType_E eType,void* pExecParam);

/**
@brief 设置目标机器的序列号和Key，与UDI接口CSUDIGetSerialKey对应

@param[in] psSerialKey 指向存放获取到的序列号和Key的结构体指针，序列号和Key均是以0结尾的字符串，请参见CSUDISerialKey_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 由于在coship内部，CSUDIGetSerialKey接口驱动部门实现，但是驱动并不知道如何获取序列号和Key，所以需要有一个set接口，供应用在初始化时调用
*/
CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif  /* UDI2_TOOLSET_PLUS_H */

/* End of File */

