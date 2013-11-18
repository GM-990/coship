#ifndef _CSUSB_H_
#define _CSUSB_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "mmcp_typedef.h"

#define MAX_DEVICE_NAME_LEN 	32
#define MAX_TYPE_NAME_LEN    	32
typedef struct device_msg_s 
{
	int  event;
	char          /***/name[MAX_DEVICE_NAME_LEN];
	char          /***/type[MAX_TYPE_NAME_LEN];
	int             volume_number;
} device_msg_t;

typedef struct CSUSB_NotifyStruct
{
	const char   *device_name;
	const char   *type_name;
	int               type;
}CSUSB_NotifyStruct_t;

/******************************************************************************
 * Device message size
 *****************************************************************************/
#define DEVICE_MSG_MAX 32
#define CS_USB_DEVICE_MAX 2

/*
 * Standard device type names
 */
#define CSUSB_TYPE_ATA_CONTROLLER    "ATA controller"
#define CSUSB_TYPE_CABLE_MODEM       "cable modem"
#define CSUSB_TYPE_ETHERNET          "ethernet"
#define CSUSB_TYPE_FIXED_STORAGE     "mass storage"
#define CSUSB_TYPE_JOYSTICK          "joystick"
#define CSUSB_TYPE_KEYBOARD          "keyboard"
#define CSUSB_TYPE_MODEM             "modem"
#define CSUSB_TYPE_MOUSE             "mouse"
#define CSUSB_TYPE_PRINTER           "printer"
#define CSUSB_TYPE_RAMDISK_STORAGE   "RAM disk storage"
#define CSUSB_TYPE_REMOVABLE_STORAGE "removable media storage"
#define CSUSB_TYPE_SATA_CONTROLLER   "SATA controller"
#define CSUSB_TYPE_SERIAL            "serial"
#define CSUSB_TYPE_USB_COMM          "USB comm"
#define CSUSB_TYPE_USB_CONTROLLER    "USB controller"
#define CSUSB_TYPE_USB_GADGET        "USB gadget"
#define CSUSB_TYPE_USB_H2H           "USB host-to-host"
#define CSUSB_TYPE_USB_HID           "USB HID"
#define CSUSB_TYPE_USB_HUB           "USB hub"
#define CSUSB_TYPE_USB_UNKNOWN       "USB unknown device"
#define CSUSB_TYPE_VOLUME            "logical volume"




/******************************************************************************
 * Block device ioctl to query the number of addressable blocks
 *****************************************************************************/
#define CSUSB_IOCTL_NUM_BLOCKS   0x05b75000
/******************************************************************************
 * Block device ioctl to query the device addressable block size
 *****************************************************************************/

#define CSUSB_IOCTL_BLOCK_SIZE    0x05b75001
/******************************************************************************
 * Block device ioctl to query the state of the media in the device
 *****************************************************************************/


#define CSUSB_IOCTL_CHECK_MEDIA     0x05b75002
#define CSUSB_MEDIA_PRESENT            0x00000000
#define CSUSB_MEDIA_NOT_PRESENT   0x00000001
#define CSUSB_MEDIA_CHANGED          0x00000002
/******************************************************************************
 * Block device ioctl to query if the media in the device is write protected
 *****************************************************************************/

#define CSUSB_IOCTL_WR_PROTECTED  0x05b75003
/******************************************************************************
 * Ioctls to get/set a read/write call timeout
 *****************************************************************************/

#define CSUSB_IOCTL_GET_RD_TIMEOUT  0x05b75004
#define CSUSB_IOCTL_SET_RD_TIMEOUT   0x05b75005
#define CSUSB_IOCTL_GET_WR_TIMEOUT  0x05b75006
#define CSUSB_IOCTL_SET_WR_TIMEOUT   0x05b75007
/******************************************************************************
 * Ioctls to get/set a read/write call mode
 *****************************************************************************/

#define CSUSB_IOCTL_GET_RD_MODE   0x05b75008
#define CSUSB_IOCTL_SET_RD_MODE    0x05b75009
#define CSUSB_IOCTL_GET_WR_MODE   0x05b7500a
#define CSUSB_IOCTL_SET_WR_MODE   0x05b7500b
#define CSUSB_MODE_NONBLOCKING    0x00000000
#define CSUSB_MODE_BLOCKING           0x00000001
/******************************************************************************
 * Ioctls to abort a read/write call
 *****************************************************************************/

#define CSUSB_IOCTL_ABORT_RD     0x05b7500c
#define CSUSB_IOCTL_ABORT_WR    0x05b7500d
/******************************************************************************
 * Ioctls to get/clear error statistics (statistics dependent on device)
 *****************************************************************************/

#define CSUSB_IOCTL_GET_ERRORS 0x05b7500e
#define CSUSB_IOCTL_CLR_ERROR    0x05b7500f
/******************************************************************************
 * Ioctls to get/clear device status information (dependent on device)
 *****************************************************************************/

#define CSUSB_IOCTL_GET_STATUS   0x05b75010
#define CSUSB_IOCTL_CLR_STATUS    0x05b75011
/******************************************************************************
 * Ioctls to flush/purge a device
 *****************************************************************************/

#define CSUSB_IOCTL_FLUSH      0x05b75012
#define CSUSB_IOCTL_PURGE      0x05b75013
#define CSUSB_READ_BUFFER     0x00000000
#define CSUSB_WRITE_BUFFER   0x00000001
/******************************************************************************
 * Ioctl to block waiting on a device to receive an event
 *****************************************************************************/

#define CSUSB_IOCTL_WAIT   0x05b75014
/******************************************************************************
 * Ioctls to get Vendor/Product IDs
 *****************************************************************************/

#define CSUSB_IOCTL_GET_VID    0x05b75015
#define CSUSB_IOCTL_GET_PID    0x05b75016
/******************************************************************************
 * Ioctls to get Vendor/Product/Serial strings from a device (if supported)
 *****************************************************************************/


#define CSUSB_IOCTL_GET_VID_STR   0x05b75017
#define CSUSB_IOCTL_GET_PID_STR   0x05b75018
#define CSUSB_IOCTL_GET_SERIAL     0x05b75019
/******************************************************************************
 * Ioctls to get device features
 *****************************************************************************/

#define CSUSB_IOCTL_GET_FEATURES  0x05b75020
/******************************************************************************/


/* Open mode flags */
#define CSUSB_OPEN_RDONLY 0x00000001
#define CSUSB_OPEN_WRONLY 0x00000002
#define CSUSB_OPEN_RDWR (CSUSB_OPEN_RDONLY| CSUSB_OPEN_WRONLY)
#define CSUSB_OPEN_ACCMODE (CSUSB_OPEN_RDWR)

/* time out definition */
#define CSUSB_TIMEOUT_IMMEDIATE  0
#define CSUSB_TIMEOUT_INFINITY      0xffffffff


#define CSUSB_DEVICE_CREATED 0x00
#define CSUSB_DEVICE_DELETED 0x01


#define CSUSB_SUCCESS   0
#define CSUSB_FAULE   -1

#define CSUSB_EVENT_TYPE 0x7fffffff
/******************************************************************************
 * Device message queue
 *****************************************************************************/
typedef void  (*CSUSBDeviceCallbackFn)(int  event,CSUSB_NotifyStruct_t *notifications[],int total,void *cookie);

int CSUSBInit(void);
void *CSUSBCreate(void *param);
void CSUSBDelete (void *instance);
void* CSUSBOpen(void *instance, int mode);
int CSUSBClose (unsigned int  handle);
int CSUSBStart(const char*name);
int CSUSBStopWithTimeout(const char *name,DWORD timeout);
int CSUSBStop(const char*name);
int CSUSBRead(unsigned int handle, void *buffer,unsigned int size);
int CSUSBWrite (unsigned int handle, const void *buffer, unsigned int  size);
int CSUSBReadInBlock(unsigned int handle, void *buffer, unsigned int  lba, unsigned int size);
int CSUSBWriteInBlock(unsigned int handle, const void *buffer, unsigned int  lba, unsigned int size);
int CSUSBIoctl(unsigned int handle, int function, void *param);
void CSUSBRegisterCallback(CSUSBDeviceCallbackFn function,int *error);
void CSUSBUnregisterCallback(CSUSBDeviceCallbackFn function);
#ifdef __cplusplus
}
#endif


#endif /* _CSUSB_H_ */

