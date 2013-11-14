/**@defgroup EEPROM EEPROM ��Ҫ������ƽ̨EEPROM�洢��ؽӿڣ���Flashģ�����ƣ���Ҳͬʱ����EEPROM�Ĳ��������ýӿڡ�
@brief EEPROMģ���Ӧͷ�ļ�<udi2_eeprom.h>����Flashģ�����ƣ���Ҳͬʱ����EEPROM�Ĳ��������ýӿڡ�

EEPROMͬ������GetInfo��ȡ���ĵ�ַ��ͬ����UDI��ȡ������Ϣ��������������Ϣ�򵥵ö࣬��֧��һ����ַһ��ƫ�� ��

��Ҫע����ǣ�EEPROM��д�����ĵ�ַ������Ե�ַ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		Because there is no EEPROM device in the STB system.
-------------------------------------------------------------------------------

@version 2.0.1 2009/07/24
@{
*/
#ifndef _UDI2_EEPROM_H_
#define _UDI2_EEPROM_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**@brief EEPROM �豸��ش������ֵ*/
enum
{
	CSUDIEEPROM_ERROR_BAD_PARAMETER = CSUDI_EEPROM_ERROR_BASE,  ///< ��������
	CSUDIEEPROM_ERROR_FEATURE_NOT_SUPPORTED,					///< ������֧��
	CSUDIEEPROM_ERROR_UNKNOWN_ERROR,							///< һ�����
	CSUDIEEPROM_ERROR_INVALID_ADDR,								///< �Ƿ���ַ
	CSUDIEEPROM_ERROR_I2C_ERROR									///< I2Cͨ�Ŵ���
};

/**@brief eeprom�ռ���Ϣ�ṹ��*/
typedef struct
 {
	CSUDI_UINT32 		m_dwStartAddress;	 ///< eeprom����ʼ��ַ
	int                 m_nSize;		     ///< �ռ��С
}CSUDIEPRInfo_S;

/**
@brief ��EEPROM�洢������д����

@param[in] dwStartAddress ��д��洢������ʼ��ַ��
@param[in] pucData ��д�������ָ�롣
@param[in] uDataLength ��д������ݳ���(��λ���ֽ�)��
@return CSUDI_SUCCESS��ʾEEPROM����д�ɹ���CSUDI_FAILURE��ʾEEPROM����дʧ�ܡ�
*/
CSUDI_Error_Code CSUDIEPRWrite(CSUDI_UINT32 dwStartAddress, const unsigned char * pucData, unsigned int  uDataLength);

/**
@brief ��EEPROM�洢�����ж�����

@param[in] dwStartAddress �������洢������ʼ��ַ��
@param[out] pucData ��Ŷ������ݵ�����ָ�롣
@param[in] uDataLength ���������ݵĳ���(��λ���ֽ�)��
@return CSUDI_SUCCESS��ʾEEPROM�������ɹ���CSUDI_FAILURE��ʾEEPROM������ʧ�ܡ�
*/
CSUDI_Error_Code CSUDIEPRRead(CSUDI_UINT32 dwStartAddress, unsigned char * pucData, unsigned int  uDataLength );

/**
@brief ��ȡEEPROMоƬ�����Ϣ

@param[out] psEPRInfo ��д��EEPROM��Ϣ��ָ�������
@return �����ɹ����� CSUDI_SUCCESS;���򷵻� CSUDI_FAILURE   
@note �ú���������ȡӦ������EEPROM�ṹ����Ϣ������Ϣ�ڽ������ݶ�дʱ�����õ���
@note���������ڽ���EEPROMʵ�ʲ���֮ǰ��Ӧ���õĵڶ�����������һ������Ϊeeprom��ʼ��������
@note�ڵ���ʱ��psEPRInfo�ɵ����߷���ռ䣬�ú���ֻ�轫��Ӧ���ֵ����
*/
CSUDI_Error_Code CSUDIEPRGetInfo(CSUDIEPRInfo_S * psEPRInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
