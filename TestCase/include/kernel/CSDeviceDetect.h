
#ifndef _CS_DEVICE_DETECT_H_
#define _CS_DEVICE_DETECT_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**�豸����*/
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


/**�豸�¼�*/
typedef enum 
{
	EM_DEVICE_EVENT_UNKONWN = 0,
	EM_DEVICE_EVENT_CREATED , ///�豸�ķ����¼�
	EM_DEVICE_EVENT_DELETED    ///�豸��ɾ���¼�
} CSDeviceEvent_E;



/**�豸֪ͨ�ṹ*/
typedef struct _CSDeviceNotify_S{
	char   * m_szDeviceName;   ///�豸��
	CSDeviceType_E m_eDeviceType;  ///�豸����
} CSDeviceNotify_S;


/**�豸֪ͨ�¼�*/
typedef void (*CSDeviceNotify_F)(
	CSDeviceEvent_E eDeviceEvent,
	CSDeviceNotify_S * psNotify,
	void * pUserData
	);

/**ע���豸֪ͨ�¼�*/
int CSDeviceNotifyRegiste (CSDeviceNotify_F fnDeviceNotify, void * pUserData);


/**ע���豸֪ͨ�¼�*/
int CSDeviceNotifyUnregiste (CSDeviceNotify_F fnDeviceNotify, void * pUserData);

#ifdef __cplusplus
}
#endif

#endif

