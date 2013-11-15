/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

/** @addtogroup CSEeprom
@{
*/
#ifndef _CS_EEPROM_PLUS_H_
#define _CS_EEPROM_PLUS_H_

#include "udiplus_typedef.h"
#include "cs_hdicommon_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief E2PROM ����*/
typedef enum
{
	AT24C01,
	AT24C02,
	AT24C04,			
	AT24C08,
	AT24C16,
	AT24C32,
	AT24C64,
	AT24C128,
	AT24C256
}CSEPRTYPE;

//δʹ�ã�
typedef struct _EEPROMTYPE
{
	DWORD m_dwDeviceAddr[8];
	int   m_nType;
}EEPROMTYPE;

/*
@brief ִ�д���EEPROM��ʼ����

��Ҫ�����ǹ涨����EEPROM��������ַ�������ͺ�
@param[in] nIndex �������ڵ�I2C���ߵ�����
@param[in] dwEPRDevAddr ����EEPROM��I2C������ַ(0b 1 0 1 0 x x x 0)(����x�ɾ���Ӳ������)
@param[in] nEPRChipType ����EEPROM������, �μ�ö��CSEPRTYPE��
@return CSHDI_SUCCESS:��ʼ���ɹ�;CSHDI_FAILURE����ʼ��ʧ��
@note 
- �������ֻ����ϵͳ��λ��ʱ�򱻵���һ�Σ�����ʱ�����ٱ����á�
- �ú���������������EEPROM API������ʹ��֮ǰ���á��������ɲ���Ԥ֪�Ĵ���
*/
CSHDI_Error_t CSEPRInitialize(  CSHDII2CIndex nIndex, BYTE bEPRDevAddr, CSEPRTYPE nEPRChipType ) ;

/*
@brief ��ȡEEPROM�����汾�š�

@param[in] pchVer ��д��洢������ʼ��ַ��
@param[in] nSize ��ȡ���ַ������ȡ�
@return ʵ�ʵõ��İ汾�ַ�������
@note ��ʽ��"[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSEEPROMGetVersion(PCHAR pchVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
