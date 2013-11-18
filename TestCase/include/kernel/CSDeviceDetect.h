
#ifndef _CS_DEVICE_DETECT_H_
#define _CS_DEVICE_DETECT_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**设备类型*/
typedef enum
{
	EM_DEVICE_TYPE_INVALID = -1,
	EM_DEVICE_TYPE_UNKNOWN = 0,
	EM_DEVICE_TYPE_ATA_CONTROLLER,
	EM_DEVICE_TYPE_CABLE_MODEM,
	EM_DEVICE_TYPE_ETHERNET,
	EM_DEVICE_TYPE_FIXED_STORAGE,
	EM_DEVICE_TYPE_JOYSTICK,
	EM_DEVICE_TYPE_KEYBOARD,
	EM_DEVICE_TYPE_MODEM,
	EM_DEVICE_TYPE_MOUSE,
	EM_DEVICE_TYPE_PRINTER,
	EM_DEVICE_TYPE_RAMDISK_STORAGE,
	EM_DEVICE_TYPE_REMOVABLE_STORAGE, 
	EM_DEVICE_TYPE_SATA_CONTROLLER,
	EM_DEVICE_TYPE_SERIAL,
	EM_DEVICE_TYPE_USB_COMM,
	EM_DEVICE_TYPE_USB_CONTROLLER,
	EM_DEVICE_TYPE_USB_GADGET,
	EM_DEVICE_TYPE_USB_H2H,
	EM_DEVICE_TYPE_USB_HID,
	EM_DEVICE_TYPE_USB_HUB,
	EM_DEVICE_TYPE_USB_UNKNOWN,
	EM_DEVICE_TYPE_VOLUME
}CSDeviceType_E;


/**设备事件*/
typedef enum 
{
	EM_DEVICE_EVENT_UNKONWN = 0,
	EM_DEVICE_EVENT_CREATED , ///设备的发现事件
	EM_DEVICE_EVENT_DELETED    ///设备的删除事件
} CSDeviceEvent_E;



/**设备通知结构*/
typedef struct _CSDeviceNotify_S{
	char   * m_szDeviceName;   ///设备名
	CSDeviceType_E m_eDeviceType;  ///设备类型
} CSDeviceNotify_S;


/**设备通知事件*/
typedef void (*CSDeviceNotify_F)(
	CSDeviceEvent_E eDeviceEvent,
	CSDeviceNotify_S * psNotify,
	void * pUserData
	);

/**注册设备通知事件*/
int CSDeviceNotifyRegiste (CSDeviceNotify_F fnDeviceNotify, void * pUserData);


/**注销设备通知事件*/
int CSDeviceNotifyUnregiste (CSDeviceNotify_F fnDeviceNotify, void * pUserData);

#ifdef __cplusplus
}
#endif

#endif

