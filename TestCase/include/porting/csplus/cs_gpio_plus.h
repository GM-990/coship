/**@defgroup CSGPIO[CSGPIO]同洲项目专用GPIO驱动接口
@brief 本模块主要定义了同洲项目专用平台无关GPIO驱动接口
@brief GPIO的意思为通用输入输出口(general-purpose I/O port) GPIO (通用输入/输出)。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。

@note 定义0-0x7f为标准GPIO范围，由芯片提供，但实际有几个由芯片决定，一般不会有0x7f这么多
@note 定义0x80-0xfe为非标准GPIO范围，由外围硬件提供，但实际有几个有硬件板决定，一般不会到0xfe这么多
@note 定义0xff为特殊GPIO，所有相关操作都会直接返回成功

@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_GPIO_PLUS_H_
#define _CS_GPIO_PLUS_H_

#include  "udiplus_typedef.h"
#include  "cs_hdicommon_plus.h"

#ifdef __cplusplus
extern "C" {
#endif


/**@brief GPIO模块接口的返回值定义*/
typedef enum
{
	CS_GPIO_SUCCESS, 
	CS_GPIO_FAILURE,
	CS_GPIO_PARAMETERS_ERROR,
	CS_GPIO_NOT_INIT,	
	CS_GPIO_NOT_OPEN,
	CS_GPIO_BUSY	
} T_CSGPIOStatus;


/**@brief  Mode of GPIO Pins */
typedef enum
{
	CS_GPIO_PRIMARY_MODE, /* Configure for primary functionality */
	CS_GPIO_SECONDARY_MODE /* Configure for GPIO functionality */
} T_CSGPIOWorkMode;


/**@brief Input/Output Mode of GPIO Pins */
typedef enum
{	
	CS_GPIO_BIDIRECTIONAL,
	CS_GPIO_INPUT,
	CS_GPIO_OUPUT,
	CS_NOT_SPECIAL
} T_CSPinIOMode;


/**@brief  Interrupt Trig Mode of GPIO Pins */
typedef enum
{
	CS_GPIO_INT_HIGH,
	CS_GPIO_INT_LOW,
	CS_GPIO_INT_UP_DOWN
} T_CSPinInterruptTrigMode;


/**@brief  Extern GPIO Config of 74HC595 */
typedef struct
{	
	BYTE SH_CP_GPIO; /** @brief  clock pin */
	BYTE ST_CP_GPIO; /** @brief  enable pin */
	BYTE DS_GPIO; /** @brief data pin */
}S74HC595_PIO_CFG_t;


typedef struct _CSGPIOData_S
{
	int m_nLevel; /** @brief  Line state, High(1) or low(0) level. */
	int m_nTime;  /** @brief  Current level duration, unit is ms */
}CSGPIOData_S;


/**@brief Interrupt Handler of GPIO Pins */
typedef void(*T_CSGPIOInterruptHandler)(void);

/**
@brief Initialize the GPIO module 

@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO module initialized successfully.
 	CS_GPIO_FAILURE:
 		GPIO module initialization failed. 
@note This routine must be called first
*/
T_CSGPIOStatus CSGPIOInitialize(void);

/**
@brief Open a GPIO pin

@param[in] ucGPIOIndex GPIO pin index.
@param[in] enmPinIOMode IO mode of the GPIO pin.
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin openned successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_BUSY:
 		The GPIO pin has already been openned.	
 	CS_GPIO_FAILURE:
 		GPIO pin open failed. 
@note This routine only be used to open a closed GPIO.
*/
T_CSGPIOStatus CSGPIOOpen(BYTE ucGPIOIndex, T_CSPinIOMode enmPinIOMode);

/**
@brief Close a openned GPIO pin

@param[in] ucGPIOIndex GPIO pin index
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin closed successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_NOT_OPEN:
 		The GPIO pin hasn't been openned.	
 	CS_GPIO_FAILURE:
 		GPIO pin closed failed.	
@note This routine only be used to close a opened GPIO	.
*/
T_CSGPIOStatus CSGPIOClose(BYTE ucGPIOIndex);

/**
@brief Config the base address of extern GPIO (138+574)

@param[in]  nTeamIndex The team index of GPIO belong to (nTeamIndex = ucGPIOIndex/8).
@param[in]  pdwBaseAddress The base address of extern GPIO (138+574)
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		Extern GPIO pin config successfully.
@note None
*/
T_CSGPIOStatus CSConfigExtGpioBaseAddr(int nTeamIndex, DWORD *pdwBaseAddress);

/**
@brief Config the base address of extern GPIO (74HC595)

@param[in]  nTeamIndex The team index of GPIO belong to (nTeamIndex = ucGPIOIndex/8).
@param[in]  pstGPIOConfig The GPIO used to control 74HC595 (74HC595)
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		Extern GPIO pin config successfully.
@note None
*/
T_CSGPIOStatus CSConfigExtGpio74HC595(int nTeamIndex, S74HC595_PIO_CFG_t *pstGPIOConfig);

/**
@brief Read a GPIO pin

@param[in]  ucGPIOIndex Gpio to read.
@param[out] pucGPIOValue used to store the value read from gpio. 
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin read successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_NOT_OPEN:
 		The GPIO pin hasn't been openned.	
 	CS_GPIO_FAILURE:
 		GPIO pin read failed.
@note None
*/
T_CSGPIOStatus CSGPIORead(BYTE ucGPIOIndex, BYTE *pucGPIOValue);

/**
@brief write a GPIO pin

@param[in]  ucGPIOIndex  Gpio to write.
@param[in]  ucGPIOValue  Value to write to gpio.
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin write successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_NOT_OPEN:
 		The GPIO pin hasn't been openned.	
 	CS_GPIO_FAILURE:
 		GPIO pin write failed.
@note None
*/
T_CSGPIOStatus CSGPIOWrite(BYTE ucGPIOIndex, BYTE ucGPIOValue);

/**
@brief Set the I/O mode of a gpio.

@param[in]  ucGPIOIndex  Gpio to config.
@param[in]  enmPinIOMode The mode want to set.
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin mode set successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_NOT_OPEN:
 		The GPIO pin hasn't been openned.	
 	CS_GPIO_FAILURE:
 		GPIO pin mode set failed.
@note None
*/
T_CSGPIOStatus CSGPIOSetIOMode(BYTE ucGPIOIndex, T_CSPinIOMode enmPinIOMode);

/**
@brief Set the work mode of a gpio.

@param[in]  ucGPIOIndex Gpio to config.
@param[in]  enmPinWorkMode  The mode want to set.
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin mode set successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_NOT_OPEN:
 		The GPIO pin hasn't been openned.	
 	CS_GPIO_FAILURE:
 		GPIO pin mode set failed.
@note None
*/
T_CSGPIOStatus CSGPIOSetWorkMode(	BYTE ucGPIOIndex, T_CSGPIOWorkMode enmPinWorkMode);

/**
@brief enable the interrupt of a gpio.

@param[in]  ucGPIOIndex Gpio to enable the interrupt.
@param[in]  enmTrigMode The Trig mode of interrupt.
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin interrupt enable successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_NOT_OPEN:
 		The GPIO pin hasn't been openned.	
 	CS_GPIO_FAILURE:
 		GPIO pin interrupt enable failed.
@note None
*/
T_CSGPIOStatus CSGPIOEnableInterrupt(BYTE ucGPIOIndex, T_CSPinInterruptTrigMode enmTrigMode);

/**
@brief disable the interrupt of a gpio.

@param[in]  ucGPIOIndex Gpio to enable the interrupt.
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin interrupt disable successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_NOT_OPEN:
 		The GPIO pin hasn't been openned.	
 	CS_GPIO_FAILURE:
 		GPIO pin interrupt disable failed.
@note None
*/
T_CSGPIOStatus CSGPIODisableInterrupt(BYTE ucGPIOIndex);

/**
@brief 

@param[in]  nSize The size of Version Buffer.
@param[out] pcVer The top address of Version Buffer
@return <1>If [pcVer] is NULL,return the actual length of
	 	version information,including "\0".
	<2>If [pcVer] is not NULL,and the call is success,
	 	return the length of version information returned.
	<3>If the call is failure,return 0.	
@note This function is used to get version information.
 	Display format is:
 	"[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSGPIOGetVersion(PCHAR pcVer, int nSize);

/**
@brief 

@param[in]  nPIOPortIndex GPIO port index.
@return The name of the gpio port
@note None
*/
char *GetPIODeviceName(int nPIOPortIndex);

/**
@brief register a GPIO interrupt server routine.

@param[in]  ucGPIOIndex Gpio to enable the interrupt.
@param[in]  pfunISR routine to server the interrupt.
@param[in]  ucIntPriority   No use
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin interrupt enable successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_BUSY:
 		The GPIO pin hasn't been register a interrupt server routine at befor.	
 	CS_GPIO_FAILURE:
 		GPIO pin interrupt enable failed.
@note None
*/
T_CSGPIOStatus CSGPIORegisterISR(BYTE ucGPIOIndex, T_CSGPIOInterruptHandler pfunISR, BYTE ucIntPriority);

/**
@brief Unregister a Interrupt Serive Rountine.

@param[in]  ucGPIOIndex Gpio to enable the interrupt.
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO pin interrupt enable successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		GPIO Index out of range.
 	CS_GPIO_FAILURE:
 		GPIO pin interrupt enable failed.
@note None
*/
T_CSGPIOStatus CSGPIOUnRegisterISR(BYTE ucGPIOIndex);

/**
@brief Send data through gpio

@param[in]  ucGPIOIndex: gpio to send data.
@param[in]  psBuffer: data to be sent
@param[in]  u32DataNum: number of data elements
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		Data send successfully.
 	CS_GPIO_PARAMETERS_ERROR:
 		Some params are illegal .
 	CS_GPIO_FAILURE:
 		Data send failed.
@note This api is synchronous which means it will not return until all data have been sent out. In order to make the time precise, we use hardware timer to control sending.
@note But currently hardware timer is often used for panel display and we implement this api in panel module. So user shoud call panel initilization api before using CSGPIOSend.
*/
T_CSGPIOStatus CSGPIOSend(BYTE ucGPIOIndex, CSGPIOData_S *psBuffer, ULONG u32DataNum);

#ifdef __cplusplus
}
#endif

/** @} */

#endif


