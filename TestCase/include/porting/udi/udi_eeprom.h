/**@defgroup CSEeprom eeprom e2prom�洢��ؽӿ�
@brief e2prom�洢��ؽӿ�

@version 0.1 2008/10/23 ��ʼ�汾
@{
*/
#ifndef _UDI_EEPROM_H_
#define _UDI_EEPROM_H_

#include "udi_typedef.h"
#include "udi_hditypedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief eeprom�ռ���Ϣ���*/
typedef struct _CSEPRInfo
 {
		DWORD m_dwStartAddr;	/**< ��ʼ��ַ*/
		int m_nSize;				/**< �ռ��С*/
} CSEPRInfo_S;

/*
@brief ����EEPROM�洢������д����

@param[in] dwStartAddress ��д��洢������ʼ��ַ��
@param[in] pbData ��д�������ָ�롣
@param[in] dwDataLength ��д������ݳ���(��λ��BYTE)��
@return CSHDI_SUCCESS��ʾEEPROM����д�ɹ���CSHDI_FAILURE��ʾEEPROM����дʧ�ܡ�
*/
CSHDI_Error_t CSEPRWriteData(DWORD dwStartAddress, PBYTE pbData, DWORD dwDataLength ) ;

/*
@brief�Ӵ���EEPROM�洢�����ж�����

@param[in] dwStartAddress �������洢������ʼ��ַ��
@param[in] pbData ��Ŷ������ݵ�����ָ�롣
@param[in] dwDataLength ���������ݵĳ���(��λ��BYTE)��
@param[out] pbData ���������ݡ�
@return CSHDI_SUCCESS��ʾEEPROM�������ɹ���CSHDI_FAILURE��ʾEEPROM������ʧ�ܡ�
*/
CSHDI_Error_t CSEPRReadData(DWORD dwStartAddress, PBYTE pbData, DWORD dwDataLength );

/*
@brief ��ȡEEPROMоƬ�����Ϣ

@param[out]pEPRInfo��д��EEPROM��Ϣ��ָ�������
@return �����ɹ����� CSHDI_SUCCESS;���򷵻�  CSHDI_FAILURE   
@note �ú���������ȡӦ������EEPROM�ṹ����Ϣ������Ϣ�ڽ������ݶ�дʱ�����õ���
���������ڽ���EEPROMʵ�ʲ���֮ǰ��Ӧ���õĵڶ�����������һ������ΪCSVPEPRInitialize()��
�ڵ���ʱ��pEPRInfo�ɵ����߷���ռ䣬�ú���ֻ�轫��Ӧ���ֵ����
*/
CSHDI_Error_t CSEPRGetInfo(CSEPRInfo_S *pEPRInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
