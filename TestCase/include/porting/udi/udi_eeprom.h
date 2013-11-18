/**@defgroup CSEeprom eeprom e2prom存储相关接口
@brief e2prom存储相关接口

@version 0.1 2008/10/23 初始版本
@{
*/
#ifndef _UDI_EEPROM_H_
#define _UDI_EEPROM_H_

#include "udi_typedef.h"
#include "udi_hditypedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief eeprom空间信息结果*/
typedef struct _CSEPRInfo
 {
		DWORD m_dwStartAddr;	/**< 起始地址*/
		int m_nSize;				/**< 空间大小*/
} CSEPRInfo_S;

/*
@brief 向串行EEPROM存储器件中写数据

@param[in] dwStartAddress 待写入存储区的起始地址。
@param[in] pbData 待写入的数据指针。
@param[in] dwDataLength 待写入的数据长度(单位：BYTE)。
@return CSHDI_SUCCESS表示EEPROM器件写成功，CSHDI_FAILURE表示EEPROM器件写失败。
*/
CSHDI_Error_t CSEPRWriteData(DWORD dwStartAddress, PBYTE pbData, DWORD dwDataLength ) ;

/*
@brief从串行EEPROM存储器件中读数据

@param[in] dwStartAddress 待读出存储区的起始地址。
@param[in] pbData 存放读出数据的数据指针。
@param[in] dwDataLength 待读出数据的长度(单位：BYTE)。
@param[out] pbData 读出的数据。
@return CSHDI_SUCCESS表示EEPROM器件读成功，CSHDI_FAILURE表示EEPROM器件读失败。
*/
CSHDI_Error_t CSEPRReadData(DWORD dwStartAddress, PBYTE pbData, DWORD dwDataLength );

/*
@brief 获取EEPROM芯片相关信息

@param[out]pEPRInfo待写入EEPROM信息的指针参数。
@return 操作成功返回 CSHDI_SUCCESS;否则返回  CSHDI_FAILURE   
@note 该函数用来获取应用数据EEPROM结构的信息，此信息在进行数据读写时将会用到。
本函数是在进行EEPROM实际操作之前所应调用的第二个函数（第一个函数为CSVPEPRInitialize()。
在调用时，pEPRInfo由调用者分配空间，该函数只需将相应的项赋值即可
*/
CSHDI_Error_t CSEPRGetInfo(CSEPRInfo_S *pEPRInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
