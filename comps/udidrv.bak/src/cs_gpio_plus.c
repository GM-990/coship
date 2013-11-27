/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        cs_gpio_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
		 This module is used for the production test application. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_gpio_plus.h"

/**
@brief Initialize the GPIO module 

@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		GPIO module initialized successfully.
 	CS_GPIO_FAILURE:
 		GPIO module initialization failed. 
@note This routine must be called first
*/
T_CSGPIOStatus CSGPIOInitialize(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIOOpen(BYTE ucGPIOIndex, T_CSPinIOMode enmPinIOMode)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIOClose(BYTE ucGPIOIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief Config the base address of extern GPIO (138+574)

@param[in]  nTeamIndex The team index of GPIO belong to (nTeamIndex = ucGPIOIndex/8).
@param[in]  pdwBaseAddress The base address of extern GPIO (138+574)
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		Extern GPIO pin config successfully.
@note None
*/
T_CSGPIOStatus CSConfigExtGpioBaseAddr(int nTeamIndex, DWORD *pdwBaseAddress)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief Config the base address of extern GPIO (74HC595)

@param[in]  nTeamIndex The team index of GPIO belong to (nTeamIndex = ucGPIOIndex/8).
@param[in]  pstGPIOConfig The GPIO used to control 74HC595 (74HC595)
@return Operation Return Status	 
 	CS_GPIO_SUCCESS:
 		Extern GPIO pin config successfully.
@note None
*/
T_CSGPIOStatus CSConfigExtGpio74HC595(int nTeamIndex, S74HC595_PIO_CFG_t *pstGPIOConfig)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIORead(BYTE ucGPIOIndex, BYTE *pucGPIOValue)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIOWrite(BYTE ucGPIOIndex, BYTE ucGPIOValue)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIOSetIOMode(BYTE ucGPIOIndex, T_CSPinIOMode enmPinIOMode)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIOSetWorkMode(	BYTE ucGPIOIndex, T_CSGPIOWorkMode enmPinWorkMode)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIOEnableInterrupt(BYTE ucGPIOIndex, T_CSPinInterruptTrigMode enmTrigMode)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIODisableInterrupt(BYTE ucGPIOIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
int CSGPIOGetVersion(PCHAR pcVer, int nSize)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 

@param[in]  nPIOPortIndex GPIO port index.
@return The name of the gpio port
@note None
*/
char *GetPIODeviceName(int nPIOPortIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    char* Retcode = "GPIO";	
    UDIDRV_LOGI("%s (Retcode =%s)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIORegisterISR(BYTE ucGPIOIndex, T_CSGPIOInterruptHandler pfunISR, BYTE ucIntPriority)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIOUnRegisterISR(BYTE ucGPIOIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

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
T_CSGPIOStatus CSGPIOSend(BYTE ucGPIOIndex, CSGPIOData_S *psBuffer, ULONG u32DataNum)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    T_CSGPIOStatus Retcode = CS_GPIO_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}


