/**@defgroup CSPower power 为应用层提供统一的电源控制接口
@version 0.1 2009/04/13 初始版本
@{
*/

#ifndef _UDI_POWER_H_
#define _UDI_POWER_H_

/* Includes ----------------------------------------- */
#include "udi_typedef.h"
#include "udi_public.h"
#include "cs_ir.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef DWORD CSPOWER_Handle_t ;

/* Revision structure */
typedef const char * CSPOWER_Revision_t;

/**@brief 设备默认的非法句柄*/
#define CSPOWER_INVALID_HANDLE (CSPOWER_Handle_t) 0xFFFFFFFF

/**@brief CSPOWER 模块的ID号，此ID在所有的coship driver模块中应该是唯一的，目前暂定为9*/
#define CSPOWER_DRIVER_ID       9

/**@brief Base of CSPOWER Driver*/
#define CSPOWER_DRIVER_BASE     (CSPOWER_DRIVER_ID << 16)

/**@brief CSPOWER模块起始错误号*/
#define CSPOWER_ERROR_BASE     (CSPOWER_DRIVER_BASE)

/**@brief 每一个注册地profile最大支持的条目数*/
#define MAX_ENTRIES_IN_LIST 100

/**@brief 通过open接口最多可获得的handle数*/
#define MAX_POWER_HANDLER (1)

/**@brief porting层的模块ID起始值*/
#define MODULE_ID_PORTING_BASE (0)

/**@brief中间层的模块ID起始值*/
#define MODULE_ID_MIDDLE_BASE    (1 << 8)

/**@brief应用层的模块ID起始值*/
#define MODULE_ID_APPLICATION_BASE    (1 << 16)

/**@brief 错误定义*/
typedef enum CSPOWER_ErrorCode_e
{
    CSPOWER_NO_ERROR = CSPOWER_DRIVER_BASE,          /**< 0 无错误 */
    CSPOWER_ERROR_BAD_PARAMETER,                          /**< 1 错误的参数*/
    CSPOWER_ERROR_NO_MEMORY,                                /**< 2 内存分配失败   */
    CSPOWER_ERROR_UNKNOWN_DEVICE,                       /**< 3 未知的设备        */
    CSPOWER_ERROR_ALREADY_INITIALIZED,                  /**< 4 设备已初始化 */
    CSPOWER_ERROR_NO_FREE_HANDLES,                       /**< 5 没有空闲的设备句柄  */
    CSPOWER_ERROR_OPEN_HANDLE,                              /**< 6 无法打开设备 */
    CSPOWER_ERROR_INVALID_HANDLE,                          /**< 7 非法的句柄       */
    CSPOWER_ERROR_FEATURE_NOT_SUPPORTED,            /**< 8 不支持指定的特性        */
    CSPOWER_ERROR_INTERRUPT_INSTALL,                        /**< 9 安装中断失败  */
    CSPOWER_ERROR_INTERRUPT_UNINSTALL,                   /**< 10 卸载中断失败 */
    CSPOWER_ERROR_TIMEOUT,                                       /**< 11 超时            */
    CSPOWER_ERROR_DEVICE_BUSY,                                 /**< 12 设备忙,暂时无法访问  */
    CSPOWER_ERROR_DEVICE_PRESENT,			             /**< 13 设备已运行 */
    CSPOWER_ERROR_DEVICE_NOT_PRESENT,		         /**< 14 设备未运行 */
    CSPOWER_ERROR_PROFILE_NOT_REGISTER,			 /**< 15 未注册的profile */
    CSPOWER_ERROR_DRIVER_NOT_INITIALIZED,	            /**< 16 设备未初始化 */
    CSPOWER_ERROR_UNKNOWN                                       /**< 17 未知错误*/
}CSPOWER_ErrorCode_E;

/**@brief 定义模块ID*/
typedef enum CSPOWER_ModuleID_e
{
    CSPOWER_MODULE_ID_OS = MODULE_ID_PORTING_BASE,		/**< OS模块ID*/
    CSPOWER_MODULE_ID_DEMUX,
    CSPOWER_MODULE_ID_VIDEO,
    CSPOWER_MODULE_ID_AUDIO,
    CSPOWER_MODULE_ID_OSG,
    CSPOWER_MODULE_ID_IFRAME,
    CSPOWER_MODULE_ID_SMC,
    CSPOWER_MODULE_ID_STORAGE,
    CSPOWER_MODULE_ID_QAM,
    CSPOWER_MODULE_ID_COFDM,
    CSPOWER_MODULE_ID_QPSK,
    CSPOWER_MODULE_ID_DISEQC,
    CSPOWER_MODULE_ID_PANEL,
    CSPOWER_MODULE_ID_VFD,
    CSPOWER_MODULE_ID_LCD,
    CSPOWER_MODULE_ID_GPIO,
    CSPOWER_MODULE_ID_I2C,
    CSPOWER_MODULE_ID_SPI,
    CSPOWER_MODULE_ID_UART,
    CSPOWER_MODULE_ID_EEPROM,
    CSPOWER_MODULE_ID_FLASH,
    CSPOWER_MODULE_ID_ENCODER,
    CSPOWER_MODULE_ID_RFMOD,
    CSPOWER_MODULE_ID_SCART,
    CSPOWER_MODULE_ID_IR,
    /*其它porting层模块的ID在此定义*/
    CSPOWER_MODULE_ID_PROTOCOL = MODULE_ID_MIDDLE_BASE,		/**< PROTOCOL模块ID*/
    /*其它中间层模块的ID在此定义*/
    CSPOWER_MODULE_ID_EPG = MODULE_ID_APPLICATION_BASE,		/**< EPG模块ID*/
    CSPOWER_MODULE_ID_USER,
    /*其它应用层模块的ID在此定义*/
    CSPOWER_MODULE_ID_MAX = 0x7FFFFFFF		                    /**< 模块ID的最大值，只用于边界测试*/
} CSPOWER_ModuleID_E;


/**@brief 定义设备类型,不同的模块设备在电源管理方面可能有不同的特性，但是目前还不清楚如何来分类，所以统一定义为CSPOWER_DEVICE*/
typedef enum CSPOWER_ModuleType_e
{
    CSPOWER_DEVICE					/**< 0 支持CSPOWER的设备 */
} CSPOWER_ModuleType_E;


/**@brief 定义模块状态*/
typedef enum CSPOWER_State_e
{
    CSPOWER_DEVICE_STATE_D0=1<<1,	/**< D0 运行状态*/
    CSPOWER_DEVICE_STATE_D1=1<<2	/**< D1 待机状态 */
} CSPOWER_State_E;


/**@brief 定义模块的电源管理函数接口. 
各个模块应该实现自己的电源管理函数并将其注册到CSPOWER 模块。
当应用程序请求将某个模块切换到指定的状态时，
CSPOWER模块将回调指定模块注册的函数。
参数定义如下:
@param[in] State 指定模块要切换到何种状态
@param[in] ModuleData_p 模块注册时传递的参数，回调时作为参数传回给相应的模块
*/
typedef CSPOWER_ErrorCode_E (*CSPOWER_Func_t) (CSPOWER_State_E State ,const void * ModuleData_p);


/**@brief CSPOWER的初始化参数*/
typedef struct CSPOWER_InitParams_s
{
    DWORD m_dwReserved; /* 保留参数 */
} CSPOWER_InitParams_S;

/**@brief CSPOWER的open 参数*/
typedef struct CSPOWER_OpenParams_S
{
    DWORD m_dwReserved; /* 保留参数 */
} CSPOWER_OpenParams_S;

/**@brief CSPOWER的term 参数*/
typedef struct CSPOWER_TermParams_s
{
   BOOL bForceTerminate;                   /*强制终止，详细说明见CSPOWERTerm*/
} CSPOWER_TermParams_S;

/**@brief This define CPU states*/
typedef enum CSPOWER_CPUState_e
{
    CSPOWER_CPU_STATE_NORMAL,	/**< Normal状态，正常运行状态 */
    CSPOWER_CPU_STATE_SLEEP,	       /**< Sleep状态， 低功耗状态(部分硬件可能不支持)*/
    CSPOWER_CPU_STATE_STANDBY	/**< Standby状态，待机状态 */
}
CSPOWER_CPUState_E;

/**@brief 唤醒原因, CPU因为何种原因从Sleep或者Standby状态恢复过来*/
typedef enum CSPOWER_WakeUpReason_e
{
    CSPOWER_WAKEUP_UNKNOWN 		= 0,		/**< 未知的唤醒原因*/
    CSPOWER_WAKEUP_NMI 			= 1<<0,		/**< 通过Tuner唤醒*/
    CSPOWER_WAKEUP_IR			      = 1<<1,		/**< 通过遥控器按键唤醒 */
    CSPOWER_WAKEUP_FRONTPANEL	= 1<<2,		/**< 通过前面板按键唤醒 */
    CSPOWER_WAKEUP_ETHETNET		= 1<<3,		/**< 通过网口唤醒 */
    CSPOWER_WAKEUP_TIMER		      = 1<<4,		/**< 定时器唤醒 */
    CSPOWER_WAKEUP_ASC			= 1<<5		/**< 通过串口唤醒 */
} CSPOWER_WakeUpReason_E;

/**@brief 定义遥控器键值结构*/
typedef struct CSPOWER_Key_s
{
        CSIRProtocol_t	        m_enmIRProtocol; /**< 协议*/
        DWORD			m_dwUserID;			/**< 遥控器厂家码，用于识别遥控器类型 */
	DWORD			m_dwKeyCode;		/**< 遥控器按键的电气编码 */
}CSPOWER_Key_S;

/**@brief 定义唤醒键值结构,支持多键值唤醒*/
typedef struct CSPOWER_WakeUpKey_s
{
        CSPOWER_Key_S   *pstWakeUpKey;          /**< 遥控器唤醒按键值数组指针*/
        DWORD                   m_dwWakeUpNum;               /**< 遥控器唤醒按键数量,该成员定义pstWakeUpKey指向键值数组的大小*/       
}CSPOWER_WakeUpKey_S;

/**@brief 定义设置wake up reason时的userdate内容*/
typedef struct CSPOWER_WakeUpUserData_s
{
    PVOID      pNmiUserData;                   /**< 通过Tuner唤醒时的用户数据指针，指向的数据暂未定义*/
    PDWORD   pdwIrUserData;                /**< 通过遥控器唤醒时的用户数据指针，指向CSPOWER_WakeUpKey_S型数据，该数据为指定遥控器按键的物理键值*/
    PDWORD   pdwFrontPanelUserData;    /**< 通过前面板唤醒时的用户数据指针，指向unsigned int型数据，该数据为指定前面板按键的物理键值*/
    PVOID      pEthetnetUserData;           /**< 通过网口唤醒时的用户数据指针，指向的数据暂未定义*/
    PDWORD   pdwTimerUserData;           /**< 通过定时器唤醒时的用户数据指针,指向unsigned int型数据，该数据为CPU的待机时长,单位为秒*/
    PVOID      pAscUserData;                  /**< 通过串口唤醒时的用户数据指针,指向的数据暂未定义*/
} CSPOWER_WakeUpUserData_S;


/**@brief 定义POWER Profile的条目内容*/
typedef struct CSPOWER_ProfileEntry_s
{
    CSPOWER_ModuleID_E eModuleID;	        /**< 模块ID*/
    CSPOWER_ModuleType_E eDeviceType;	/**< 模块的类型 */
    CSPOWER_State_E eModuleState;		/**< 指定模块切换到的状态*/
} CSPOWER_ProfileEntry_S;





/*Functions -------------------------------------------------------------*/

/**
@brief 初始化CSPOWER模块

@param[in] psInitParams 初始化参数指针
@return CSPOWER_NO_ERROR 初始化成功
@return CSPOWER_ERROR_ALREADY_INITIALIZED CSPOWER已经初始化
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_NO_MEMORY 没有足够的内存空间.
@note 此函数应当在系统初始化的时候首先调用
*/
CSPOWER_ErrorCode_E CSPOWERInit(CSPOWER_InitParams_S *psInitParams);

/**
@brief 打开CSPOWER,创建可用的句柄

@param[in] psOpenParams open参数.
@param[out] pHandle open成功后返回可用的句柄.
@return CSPOWER_NO_ERROR 成功.
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_NO_FREE_HANDLES open已达最大数，无法再创建新的句柄.
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED 模块没有初始化.
@note 此函数应当在CSPOWERInit之后CSPOWERClose之前调用
*/
CSPOWER_ErrorCode_E CSPOWEROpen(const CSPOWER_OpenParams_S *psOpenParams, CSPOWER_Handle_t *pHandle);


/**
@brief 销毁CSPOWEROpen创建的句柄.

@param[in] Handle CSPOWER驱动句柄，通过CSPOWEROpen获得.
@return CSPOWER_NO_ERROR  成功.
@return CSPOWER_ERROR_INVALID_HANDLE 传入的句柄非法.
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED 模块没有初始化.
@note 此函数成功返回之后，之前传入的合法句柄将变为非法，不再可用
*/
CSPOWER_ErrorCode_E CSPOWERClose(const CSPOWER_Handle_t Handle);

/**
@brief 终止CSPOWER驱动.

@param[in] psTermParams 终止参数的指针.
@return CSPOWER_NO_ERROR 成功.
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_OPEN_HANDLE 有未被销毁的CSPOWER驱动句柄存在，不允许终止.
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWER模块没有初始化.
@note 此函数终止CSPOWER，并且清除所有的内部状态以及用户注册的函数和profile
@note 如果 ForceTerminate 为 TRUE ，即使有未被销毁的CSPOWER驱动句柄存在，仍然强制终止. 
@note 如果 ForceTerminate 为 FALSE，当有未被销毁的CSPOWER驱动句柄存在时，返回CSPOWER_ERROR_OPEN_HANDLE
*/
CSPOWER_ErrorCode_E CSPOWERTerm ( const CSPOWER_TermParams_S *  psTermParams);

/**
@brief 获取CSPOWER模块版本号

@return 字符串形式的CSPOWER版本号.
*/
CSPOWER_Revision_t CSPOWERGetRevision(void);

/**
@brief 注册profile

@param[in] Handle CSPOWER驱动句柄，通过CSPOWEROpen获得.
@param[in] psProfile profile列表指针，指向CSPOWER_ProfileEntry_t类型的数组
@param[in] dwNumEntries profile列表中包含的条目数，即有多少个profile Entry
@param[out] pdwProfileID 当注册成功时，返回内部分配的profileID
@return CSPOWER_NO_ERROR 成功.
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_NO_MEMORY 没有足够的空间
@return CSPOWER_ERROR_UNKNOWN_DEVICE ProfileEntry中指定的设备不存在，或者存在但是没有向CSPOWER注册电源管理函数
@return CSPOWER_ERROR_FEATURE_NOT_SUPPORTED profile中指定的某个模块的的state该模块不支持
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWER模块没有初始化.
@note CSPOWERRegisterProfile只是将各个模块的信息保存起来，
@note 只有调用CSPOWERSetProfile时才会通过调用各个模块注册的回调函数执行待机或者唤醒动作
@note 对同一个profile只需注册一次即可，可以多次set
*/
CSPOWER_ErrorCode_E CSPOWERRegisterProfile(const CSPOWER_Handle_t Handle,
                                    CSPOWER_ProfileEntry_S *psProfile,
                                    DWORD dwNumEntries, DWORD *pdwProfileID);

/**
@brief 执行profile

@param[in] Handle CSPOWER驱动句柄，通过CSPOWEROpen获得.
@param[in] dwProfileID Profile ID，通过CSPOWERRegisterProfile获得
@return CSPOWER_NO_ERROR  成功.
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_INVALID_HANDLE 传入非法的handle
@return CSPOWER_ERROR_PROFILE_NOT_REGISTER 指定的profile没有注册
@return 其它错误由回调函数返回的错误
@note CSPOWERSetProfile会调用各个模块注册的回调函数执行实际的动作，
@note 其调用的顺序严格按照各个模块在profile中出现的顺序执行
*/
CSPOWER_ErrorCode_E CSPOWERSetProfile(const CSPOWER_Handle_t Handle, DWORD dwProfileID);

/**
@brief 注册设备(模块的电源管理函数)

@param[in] eDeviceID 模块编号.
@param[in] fnPowerFunc 电源管理函数。
@param[in] nSupportedState 该模块支持的电源状态，见CSPOWER_State_t，可支持多种
@param[in] pModuleData 模块的私有数据，调用注册的函数时，会将DeviceData_p作为参数传回
@return CSPOWER_NO_ERROR  成功.
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_DEVICE_PRESENT 指定ID的模块已经注册了电源管理函数
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWER模块没有初始化.
@return CSPOWER_ERROR_NO_MEMORY 没有足够的内存空间
@note 每一个模块都必须在自己的初始化函数中调用此接口注册自己的电源管理函数.
@note 每个模块的ID都应是全局可见的并且是全局唯一的
*/
CSPOWER_ErrorCode_E CSPOWERRegisterPowerFunction(const CSPOWER_ModuleID_E eDeviceID,
                                CSPOWER_Func_t fnPowerFunc, INT nSupportedState, void *pModuleData);

/**
@brief 移除已注册的电源管理函数

@param[in] eDeviceID  模块编号
@return CSPOWER_NO_ERROR 成功
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_DEVICE_NOT_PRESENT 指定ID的模块没有注册电源管理函数
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWER模块没有初始化
@note 每一个模块都必须在自己的邋Term 函数中调用此接口注销自己的电源管理函数.
*/
CSPOWER_ErrorCode_E CSPOWERUnRegisterPowerFunction(const CSPOWER_ModuleID_E eDeviceID);

/**
@brief 获取CPU支持的唤醒方式 .

@param[in] Handle CSPOWER驱动句柄，通过CSPOWEROpen获得.
@param[out] pnWakeUpReason 返回CPU支持的唤醒方式
@return CSPOWER_NO_ERROR 成功
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_FEATURE_NOT_SUPPORTED 不支持的特性
*/
CSPOWER_ErrorCode_E CSPOWERGetCPUSupportWakeUpReason(const CSPOWER_Handle_t Handle, INT *pnWakeUpReason);

/**
@brief 设置CPU状态，将CPU切换到指定的状态

@param[in] Handle CSPOWER驱动句柄，通过CSPOWEROpen获得.
@param[in] eCPUState 指定CPU要切换的状态
@param[in] nWakeUpReasonSelect用户设置的唤醒方式,必须是CPU支持的方式，可以多种方式组合
@param[out]  peWakeUpReason 函数运行结束时返回CPU被唤醒的原因(即CPU是以何种方式被唤醒的)
@param[in] pUserData 定义如下:
WakeUpReason_p为CSPOWER_WAKEUP_TIMER(即定时唤醒)时，UserData指向unsigned int型数据，该数据为CPU的待机时长,单位为秒
WakeUpReason_p为CSPOWER_WAKEUP_IR(遥控器唤醒)时，UserData指向unsigned int型数据，该数据为指定遥控器按键的物理键值
WakeUpReason_p为CSPOWER_WAKEUP_FRONTPANEL(前面板唤醒)时，UserData指向unsigned int型数据，该数据为指定前面板按键的物理键值
@return CSPOWER_NO_ERROR 成功
@return CSPOWER_ERROR_BAD_PARAMETER 传入的参数错误.
@return CSPOWER_ERROR_INVALID_HANDLE 非法的handle
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWER模块没有初始化
@return CSPOWER_ERROR_FEATURE_NOT_SUPPORTED 不支持的特性，可能是CPU不支持切换到指定的状态或者不支持指定的唤醒方式
*/

CSPOWER_ErrorCode_E CSPOWERSetCPUState(const CSPOWER_Handle_t Handle, CSPOWER_CPUState_E eCPUState, 
										INT nWakeUpReasonSelect, CSPOWER_WakeUpReason_E *peWakeUpReason, CSPOWER_WakeUpUserData_S* psUserData);



#ifdef __cplusplus
}
#endif

#endif /* #ifndef _UDI_POWER_H_ */





