#ifndef _CS_HDICOMMON_PLUS_H_
#define _CS_HDICOMMON_PLUS_H_

#include  "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSHDI_TIMEOUT_INFINITY		(DWORD)0xffffffff
#define CSHDI_TIMEOUT_IMMEDIATE	0

#define CSHDI_GPIO_NO_USE			0xff

#define CSHDI_USB_TUNER_BASE 	0x08

/**@brief Common driver error constants */
#define CSHDI_DRIVER_ID   0
#define CSHDI_DRIVER_BASE (CSHDI_DRIVER_ID << 16)
typedef enum
{
    CSHDI_SUCCESS = CSHDI_DRIVER_BASE,
    CSHDI_FAILURE,
    CSHDI_ERROR_BAD_PARAMETER,             /**< Bad parameter passed       */
    CSHDI_ERROR_NO_MEMORY,                 /**< Memory allocation failed   */
    CSHDI_ERROR_UNKNOWN_DEVICE,            /**< Unknown device name        */
    CSHDI_ERROR_ALREADY_INITIALIZED,       /**< Device already initialized */
    CSHDI_ERROR_NO_FREE_HANDLES,           /**< Cannot open device again   */
    CSHDI_ERROR_OPEN_HANDLE,               /**< At least one open handle   */
    CSHDI_ERROR_INVALID_HANDLE,            /**< Handle is not valid        */
    CSHDI_ERROR_FEATURE_NOT_SUPPORTED,     /**< Feature unavailable        */
    CSHDI_ERROR_INTERRUPT_INSTALL,         /**< Interrupt install failed   */
    CSHDI_ERROR_INTERRUPT_UNINSTALL,       /**< Interrupt uninstall failed */
    CSHDI_ERROR_TIMEOUT,                   /**< Timeout occured            */
    CSHDI_ERROR_DEVICE_BUSY,               /**< Device is currently busy   */
    CSHDI_ERROR_DEVICE_NOT_INITIALIZED
}CSHDIComErrType;

typedef enum
{
	CSHDI_OFF,		/**< HDI device off*/
	CSHDI_ON		/**< HDI device on*/
}CSHDISwitch;

typedef enum
{
	I2C_0,			/**< i2c bus index 0*/
	I2C_1,			/**< i2c bus index 1*/
	I2C_2,			/**< i2c bus index 2*/
	I2C_3,			/**< i2c bus index 3*/
	I2C_4, 			/**< i2c bus index 4*/
	MAX_I2C_NUM	/**< maximun number of i2c bus */
}CSHDII2CIndex;

typedef enum
{
	SPI_0,			/**< spi bus index 0*/
	SPI_1,			/**< spi bus index 1*/
	SPI_2,			/**< spi bus index 2*/
	SPI_3,			/**< spi bus index 3*/
	MAX_SPI_NUM	/**< maximun number of spi bus */
}CSHDISPIIndex;

typedef enum
{
	Tuner_0,							/**< tuner index 0*/
	Tuner_1,							/**< tuner index 1*/
	Tuner_2,							/**< tuner index 2*/
	Tuner_3,							/**< tuner index 3*/
	Tuner_Mobile_0 = CSHDI_USB_TUNER_BASE, /**< usb tuner index 0 */
	Tuner_Mobile_1,						/**< usb tuner index 1*/
	Tuner_Mobile_2,						/**< usb tuner index 2*/
	Tuner_Mobile_3,						/**< usb tuner index 3*/
	MAX_TUNER_NUM					/**< maximun number of tuners */
}CSHDITunerIndex;

typedef enum
{
	AVIN_0,			/**< AVIN index 0*/
	AVIN_1,			/**< AVIN index 1*/
	AVIN_2,			/**< AVIN index 2*/
	MAX_AVIN_NUM	/**< max number of avin devices */
}CSHDIAVINIndex_E;


typedef enum 
{
	TS_PARALLEL,		/**<TS output mode:parallel*/
	TS_SERIAL			/**<TS output mode:serial*/
}CSHDITsMode;


typedef enum
{
	CSHDI_CALLBACK_IR,			/**< callback from cs_ir */
	CSHDI_CALLBACK_PANEL,		/**< callback from cs_panel */
	CSHDI_CALLBACK_QPSK,		/**< callback from cs_qpsk */
	CSHDI_CALLBACK_COFDM,		/**< callback from cs_cofdm */
	CSHDI_CALLBACK_QAM,		/**< callback from cs_qam */
	CSHDI_CALLBACK_SCART,
	CSHDI_CALLBACK_MOUSE,	/**< callback from cs_input_device */
	CSHDI_CALLBACK_KEYBOARD,	/**< callback from cs_input_device */
	CSHDI_CALLBACK_JOYSTICK	/**< callback from cs_input_device */
} CSHDICallbackType;


typedef DWORD CSHDI_Error_t;

typedef  void ( *CSHDICallback )( CSHDICallbackType , void * );

#ifdef __cplusplus
}
#endif

#endif
