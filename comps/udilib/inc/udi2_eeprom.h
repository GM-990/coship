/**@defgroup EEPROM EEPROM 主要定义了平台EEPROM存储相关接口，和Flash模块类似，它也同时包括EEPROM的操作及配置接口。
@brief EEPROM模块对应头文件<udi2_eeprom.h>，和Flash模块类似，它也同时包括EEPROM的操作及配置接口。

EEPROM同样适用GetInfo获取到的地址，同样从UDI获取配置信息。但它的配置信息简单得多，仅支持一个地址一个偏移 。

需要注意的是，EEPROM读写操作的地址都是相对地址。

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


/**@brief EEPROM 设备相关错误代码值*/
enum
{
	CSUDIEEPROM_ERROR_BAD_PARAMETER = CSUDI_EEPROM_ERROR_BASE,  ///< 参数错误
	CSUDIEEPROM_ERROR_FEATURE_NOT_SUPPORTED,					///< 操作不支持
	CSUDIEEPROM_ERROR_UNKNOWN_ERROR,							///< 一般错误
	CSUDIEEPROM_ERROR_INVALID_ADDR,								///< 非法地址
	CSUDIEEPROM_ERROR_I2C_ERROR									///< I2C通信错误
};

/**@brief eeprom空间信息结构体*/
typedef struct
 {
	CSUDI_UINT32 		m_dwStartAddress;	 ///< eeprom的起始地址
	int                 m_nSize;		     ///< 空间大小
}CSUDIEPRInfo_S;

/**
@brief 向EEPROM存储器件中写数据

@param[in] dwStartAddress 待写入存储区的起始地址。
@param[in] pucData 待写入的数据指针。
@param[in] uDataLength 待写入的数据长度(单位：字节)。
@return CSUDI_SUCCESS表示EEPROM器件写成功，CSUDI_FAILURE表示EEPROM器件写失败。
*/
CSUDI_Error_Code CSUDIEPRWrite(CSUDI_UINT32 dwStartAddress, const unsigned char * pucData, unsigned int  uDataLength);

/**
@brief 从EEPROM存储器件中读数据

@param[in] dwStartAddress 待读出存储区的起始地址。
@param[out] pucData 存放读出数据的数据指针。
@param[in] uDataLength 待读出数据的长度(单位：字节)。
@return CSUDI_SUCCESS表示EEPROM器件读成功，CSUDI_FAILURE表示EEPROM器件读失败。
*/
CSUDI_Error_Code CSUDIEPRRead(CSUDI_UINT32 dwStartAddress, unsigned char * pucData, unsigned int  uDataLength );

/**
@brief 获取EEPROM芯片相关信息

@param[out] psEPRInfo 待写入EEPROM信息的指针参数。
@return 操作成功返回 CSUDI_SUCCESS;否则返回 CSUDI_FAILURE   
@note 该函数用来获取应用数据EEPROM结构的信息，此信息在进行数据读写时将会用到。
@note本函数是在进行EEPROM实际操作之前所应调用的第二个函数（第一个函数为eeprom初始化函数。
@note在调用时，psEPRInfo由调用者分配空间，该函数只需将相应的项赋值即可
*/
CSUDI_Error_Code CSUDIEPRGetInfo(CSUDIEPRInfo_S * psEPRInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
