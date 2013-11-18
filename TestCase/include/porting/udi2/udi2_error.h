/**@defgroup ERROR ERROR �������ģ��Ĵ���������ֵ��ͨ�ó������

@brief Errorģ���Ӧͷ�ļ�<udi2_error.h>�������˸���ģ��Ĵ���������ֵ��ͨ��
�ɹ�����CSUDI_SUCCESS��ͨ�ó������CSUDI_FAILURE��û�нӿ���Ҫʵ�֡�

һ������£�UDI���еĺ��������ر�ģ�鶨���CSUDI_Error_Code���͵Ĵ������ֵ��
����������ֵ����������ɣ����16λΪģ����룬�ڱ�ģ���ж��壨������������
ֵ������16λΪ���������룬�ھ���ģ���ж��塣

@note ������ģ�鼰�ӿ��жԴ������ֵ��FEATURE_NOT_SUPPORTED��NO_MEMORY�Ƚ�����
�ر�˵��������������ֵ�����ڵ��ԣ�Eastwin�����ж������Ƿ�ɹ�����������ݴ�
�����ֵ������ͬ�ĺ���������
@version 2.0.1 2009/07/27 ����
@{
*/

#ifndef _UDI2_ERROR_H_
#define _UDI2_ERROR_H_

#include "udi2_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSUDI_SUCCESS   (0)     		///< ͨ�óɹ�����,����һ��ɹ����ø�ֵ��ʾ

#define CSUDI_FAILURE   (0xFFFFFFFF)    ///< һ��ʧ�ܴ���,δ����ĳ�������ʹ�ø�ֵ�������鷵��ʵ�ʵĴ������ֵ������Ӧ��ʹ�ø�ֵ

typedef CSUDI_UINT32    CSUDI_Error_Code;  ///< ƽ̨��������ֵ����

/**@brief ����ģ�����������ֵö�ٶ���

@note ÿ��ģ��Ĵ������ֵ��������ĸ������Ĵ����������:BAD_PARAMETER��NO_MEMORY,FEATURE_NOT_SUPPORTED,UNKNOWN_ERROR
*/
enum
{
	CSUDI_AOUT_ERROR_BASE			= 1 << 16,      ///< AOUTģ��Ĵ���������ֵ
	CSUDI_AUDIO_ERROR_BASE			= 2 << 16,		///< AUDIOģ��Ĵ���������ֵ
	CSUDI_VIDEO_ERROR_BASE			= 3 << 16,		///< VIDEOģ��Ĵ���������ֵ
	CSUDI_SCREEN_ERROR_BASE			= 4 << 16, 		///< SCREENģ��Ĵ���������ֵ
	CSUDI_DESCRAMBLE_ERROR_BASE		= 5 << 16,		///< DESCRAMBLEģ��Ĵ���������ֵ
	CSUDI_INJECT_ERROR_BASE			= 6 << 16,		///< INJECTERģ��Ĵ���������ֵ
	CSUDI_RECORD_ERROR_BASE			= 7 << 16,		///< RECORDģ��Ĵ���������ֵ
	CSUDI_OSG_ERROR_BASE			= 8 << 16,		///< OSGģ��Ĵ���������ֵ
	CSUDI_SECTION_ERROR_BASE		= 9 << 16,		///< SECTIONģ��Ĵ���������ֵ
	CSUDI_SMC_ERROR_BASE			= 10 << 16,		///< SMCģ��Ĵ���������ֵ
	CSUDI_OS_ERROR_BASE				= 11 << 16,		///< OSģ��Ĵ���������ֵ
	CSUDI_FS_ERROR_BASE				= 12 << 16,		///< FSģ��Ĵ���������ֵ
	CSUDI_PLAYER_ERROR_BASE			= 13 << 16,		///< PLAYERģ��Ĵ���������ֵ
	CSUDI_TUNER_ERROR_BASE			= 14 << 16,		///< TUNERģ��Ĵ���������ֵ
	CSUDI_FLASH_ERROR_BASE			= 15 << 16,		///< FLASHģ��Ĵ���������ֵ
	CSUDI_DEMUX_ERROR_BASE			= 16 << 16,		///< DEMUXģ��Ĵ���������ֵ
	CSUDI_EEPROM_ERROR_BASE			= 17 << 16,		///< EEPROMģ��Ĵ���������ֵ
	CSUDI_TOOLSET_ERROR_BASE		= 18 << 16,		///< TOOLSETģ��Ĵ���������ֵ
	CSUDI_PANEL_ERROR_BASE			= 19 << 16,		///< PANELģ��Ĵ���������ֵ
	CSUDI_INPUT_ERROR_BASE			= 20 << 16,		///< INPUTģ��Ĵ���������ֵ
	CSUDI_IPCFG_ERROR_BASE			= 21 << 16,		///< IPCFGģ��Ĵ���������ֵ
	CSUDI_OTA_ERROR_BASE			= 22 << 16,   	///< OTAģ��Ĵ���������ֵ
	CSUDI_EW200S_ERROR_BASE			= 23 << 16,		///< Eastwin200��������������ֵ
	CSUDI_VOUT_ERROR_BASE       	= 24 << 16,   	///< VOUTģ��Ĵ���������ֵ
	CSUDI_INVOKE_ERROR_BASE     	= 25 << 16,   	///< invokeNativeģ��Ĵ���������ֵ
	CSUDI_UART_ERROR_BASE       	= 26 << 16,   	///< Uartģ��Ĵ���������ֵ
	CSUDI_PPPOE_ERROR_BASE      	= 27 << 16,   	///<PPPOEģ��Ĵ���������ֵ
	CSUDI_BLUETOOTH_ERROR_BASE		= 28 << 16,  	///<bluetoothģ��Ĵ���������ֵ
	CSUDI_USB_ERROR_BASE       		= 29 << 16,  	///<usbģ��Ĵ���������ֵ
	CSUDI_BOOTSHOW_ERROR_BASE       = 30 << 16,  	///<bootshowģ��Ĵ���������ֵ
	CSUDI_MAX_ERROR_BASE							///< �߽�ֵ
};

#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_ERROR_H_

