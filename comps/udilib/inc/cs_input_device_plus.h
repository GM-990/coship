/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

#ifndef __CS_INPUT_DEVICE_PLUS_H__
#define __CS_INPUT_DEVICE_PLUS_H__

#include "cs_hdicommon_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief 输入设备类型枚举定义 */
typedef enum {
	CS_INPUT_DEVICE_MOUSE=1<<0,                //鼠标
	CS_INPUT_DEVICE_KEYBOARD=1<<1,		//键盘
	CS_INPUT_DEVICE_JOYSTICK=1<<2,		//游戏手柄
	CS_INPUT_DEVICE_MAX	=1<<3
}CSInputDeviceType_E;

/**@brief 输入设备数据结构定义 */
typedef struct  {
	char	m_caName[64];	 //输入设备名称
	CSInputDeviceType_E	m_eInputType; //输入设备类型
	char m_caEventName[64]; //输入设备对应的事件名称，包含路径，用于打开该设备，例如/dev/input/mice
	int m_nVendorId;	//输入设备的生产厂商ID
	int m_nProductId; // 输入设备的产品ID
	int m_nDevFd; //linux系统输入设备对应的文件句柄，通过nDevfd =open(caEvenNamet) 获取，后续对该设备的操作都需要该句柄	
}InputDevice_S;

 /**@brief 键盘鼠标游戏手柄时间定义 */
typedef struct {
     unsigned long    tv_sec;
     unsigned long    tv_usec;
 }CSInputDeviceTimeval_S;

/**@brief 键盘鼠标游戏手柄所产生的事件的数据类型定义 */
typedef struct {
    unsigned int m_uDeviceIndex;   ///<事件发生的设备编号，用于区分同一类型的多个设备
    CSInputDeviceTimeval_S m_stTime;      ///<事件发生的时间
    unsigned short m_ushEventType;      ///< 事件类型
    unsigned short m_ushCode;      ///< 事件对应的数据
    signed int m_nValue;///<事件携带的状态数据. 根据m_ushEventType的不同而有所差异
}CSInputDeviceParam_S ;

/**
@brief Input device module 初始化。
@return CSHDI_SUCCESS 表示初始化成功，CSHDI_FAILURE表示初始化失败。
*/
CSHDI_Error_t CSInputDeviceInit(void);

/**
@brief注册鼠标、键盘、手柄等input device 回调函数，input device 驱动将按键、坐标位置等信息通过回调函数传递给应用

@param[in] fnCallback 指向回调函数的函数指针，用户通过回调函数得到input device 句柄、键值和坐标位置等信息，详见CSHDICallback定义。
@return CSHDI_SUCCESS：注册成功;CSHDI_ERROR_BAD_PARAMETER：输入参数错误。
@note
*/
CSHDI_Error_t CSInputDeviceRegisterKeySendCallback(CSHDICallback fnCallback);


#ifdef __cplusplus
}
#endif

#endif

