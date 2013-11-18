/**@defgroup CSFLASH flash 本模块主要定义了闪存的读写和格式化(erase)接口
@brief 本模块主要定义了闪存的读写和格式化(erase)接口
@brief 闪存是一种非挥发性的半导体存储芯片,有许多种类型,从结构上分主要有AND、NAND、NOR、DiNOR等,其中NAND和NOR是目前最为常见的类型

@version 0.1 2008/10/23 初始版本
@{
*/

#ifndef _UDI_FLASH_H_
#define _UDI_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "udiplus_typedef.h"


/**@brief flash芯片信息数据结构*/
typedef struct 
{
	int   		nFlashManuFactory ;	/**< the manufacturer ID of the flash used,such as AMD(0x01),Intel(0x89),ST(0x20),etc. */
	int	  	nFlashType ;			/**< The specific type of a chip Vendor.such as SST39VFXX(0x00~0x19),AMD29LVXX(0x20~0x39) etc*/
	DWORD 	dwCapacity;			/**< Flash的实际空间大小,以字节为单位*/
	int	  	nSectorNumber ;		/**< Total sector number*/
	DWORD 	dwSize;				/**< 实际可用的flash总的空间大小，以字节为单位*/
	DWORD 	dwMaxSectorSize;		/**< 最大的block(section)的大小，以字节为单位*/
	DWORD 	*pMemoryStart;		/**< 每个block的起始地址数组，此地址直接用于读写flash，数组长度不小于nSectorNumber*/
}T_CHIPInfo;

/**
@brief 从FLASH中读取保存的数据 

@param[in] dwStartAddress 存储数据的FLASH空间地址
@param[in] pbValue 保存读出数据的内存空间地址
@param[in] dwLength 要读取的数据长度(以字节为单位)
@return CSHDI_SUCCESS表示读取成功，CSHDI_FAILURE表示读取失败
*/
CSHDI_Error_t CSFLSReadFlash (DWORD dwStartAddress, PBYTE pbValue, DWORD dwLength);

/**
@brief 往FLASH中写入需要保存的数据 ,内建擦除

@param[in] dwStartAddress 存储数据的FLASH空间地址
@param[in] pbValue 要写入数据的内存空间地址
@param[in] dwLength 要写入的数据长度(以字节为单位)
@return CSHDI_SUCCESS表示写入成功，CSHDI_FAILURE表示写入失败
*/
CSHDI_Error_t CSFLSWriteFlash (DWORD dwStartAddress, PBYTE pbValue, DWORD dwLength);

/**
@brief 取得配置的FLASH的芯片数

@return 配置的FLASH的芯片数
*/
int CSFLSGetFLSCount(void) ;

/**
@brief 初始化后读取flash的信息。 

@param[in] nChipIndex 当前FLASH序号（从０开始）边界： nFlashIndex >= 0 && nFlashIndex < 配置的flash片数
@param[out] tChipInfo 指向flash相关信息结构体的指针，其结构在头文件中定义：
@return CSHDI_SUCCESS表示读取成功，CSHDI_FAILURE表示读取失败
*/
CSHDI_Error_t CSFlashGetChipInfo (int nChipIndex, T_CHIPInfo *tChipInfo);

/**
@brief 擦除指定空间。

@param[in] dwStartAddr 待擦除空间的起始地址（绝对地址）
@param[in] dwSpaceSize 待擦除空间大小（单位：BYTE）
@return CSHDI_SUCCESS 表示擦除操作成功，CSHDI_FAILURE表示擦除操作失败。
*/
CSHDI_Error_t CSFLSEraseSpace(DWORD dwStartAddr, DWORD dwSpaceSize);

/**
@brief 往FLASH中写入需要保存的数据 ,为了提高效率实现时不带擦除

@param[in] pbBuffer 待写入数据指针
@param[in] dwDataLength 待写入数据长度（单位：BYTE）
@param[in] dwFlashAddr 待写入空间的起始地址（绝对地址）
@return CSHDI_SUCCESS 表示写操作成功，CSHDI_FAILURE表示写操作失败。
*/
CSHDI_Error_t CSFLSWriteDataWithoutErase(PBYTE pbBuffer, DWORD dwDataLength, DWORD dwFlashAddr);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
