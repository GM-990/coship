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

/**@brief E2PROM 类型*/
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

//未使用？
typedef struct _EEPROMTYPE
{
	DWORD m_dwDeviceAddr[8];
	int   m_nType;
}EEPROMTYPE;

/*
@brief 执行串行EEPROM初始化。

主要工作是规定串行EEPROM的器件地址和器件型号
@param[in] nIndex 器件所在的I2C总线的索引
@param[in] dwEPRDevAddr 串行EEPROM的I2C器件地址(0b 1 0 1 0 x x x 0)(其中x由具体硬件决定)
@param[in] nEPRChipType 串行EEPROM的类型, 参见枚举CSEPRTYPE。
@return CSHDI_SUCCESS:初始化成功;CSHDI_FAILURE：初始化失败
@note 
- 这个函数只是在系统复位的时候被调用一次，其他时候不能再被调用。
- 该函数必须先于其他EEPROM API函数被使用之前调用。否则会造成不可预知的错误。
*/
CSHDI_Error_t CSEPRInitialize(  CSHDII2CIndex nIndex, BYTE bEPRDevAddr, CSEPRTYPE nEPRChipType ) ;

/*
@brief 获取EEPROM驱动版本号。

@param[in] pchVer 待写入存储区的起始地址。
@param[in] nSize 获取的字符串长度。
@return 实际得到的版本字符串长度
@note 格式："[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSEEPROMGetVersion(PCHAR pchVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
