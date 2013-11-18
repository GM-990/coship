/**@defgroup UDI2FLASH  FLASH 本模块主要定义了闪存的读写和格式化(erase)相关接口
@brief FLASH模块对应头文件<udi2_flash.h>，包括了Flash的操作及配置接口 。

操作Flash的地址均通过GetInfo接口获取，即MMCP（CloudTV ）中间件平台并不知道也不关心究竟提供的是相对还是绝对地址，GetInfo和Flash读写操作接口配合好即可。MMCP（CloudTV ）中间件平台不会通过地址直接读取Flash的内容。

除了基本的Flash操作接口，Flash模块包括的另一个功能--Flash配置接口是项目相关的。通过该接口，可获取到分配给MMCP（CloudTV ）中间件平台的总Flash区域。MMCP（CloudTV ）中间件平台会根据项目的不同和分配Flash区域的不同，分配这些区域。一般来说，分配给MMCP（CloudTV ）中间件平台的区域应该除开主程序烧录区，UDIFS区域，Booterloader区域，MAC地址、序列号烧录区、OTA数据区域等。

MMCP（CloudTV ）中间件平台使用Region和RegionSuit两个概念来表示Flash分配情况。

Region是一个物理概念，下图示例了Flash中的Region。它表示Flash中实际存在的，连续的，相同大小的若干Block。

@image HTML udi_flash_region.png
RegionSuit是一个Flash分配上的概念，它表示项目上，将一块Flash划分给某个软件模块如MMCP（CloudTV ）中间件平台使用。RegionSuit可能是不连续的。下图示例了RegionSuit的概念。

@image HTML udi_flash_regionsuit.png

需要注意的是，flash读写操作的地址都是绝对地址。

@version 2.0.2 2009/8/24 第一次综合评审后修改的版本
@version 2.0.1 2009/07/24 创建
@{
*/

#ifndef _UDI2_FLASH_H_
#define _UDI2_FLASH_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSUDI_FLASH_MAX_REGION_CNT		(32)


/**@brief FLASH 设备相关错误代码值*/
enum
{
	CSUDIFLASH_ERROR_BAD_PARAMETER = CSUDI_FLASH_ERROR_BASE,   
	CSUDIFLASH_ERROR_INVALID_ADDR,
	CSUDIFLASH_ERROR_UNKNOWN
};


/**@brief Flash分区信息数据结构

一个分区包括<B>连续</B>的相同大小的若干个Block
*/
typedef struct  
{
	CSUDI_UINT32     m_dwStartAddress;			 ///< 分区起始地址,用于读写访问
	unsigned int     m_uBlockSize;		         ///< 分区Block大小,分区中所有Block的大小是相同的
	unsigned int     m_uBlockCount;		         ///< 分区Block数量
}CSUDIFLASHRegion_S;

/**@brief Flash分区集合信息数据结构

@note Flash分区集表示整个Flash中由若干个分区组成的集合,结构中所有统计数据均在本集合中的分区中产生
@note 分区集合中的区域不一定是连续的
@note 分区集合中的区域起始地址按升序排列
*/
typedef struct 
{
	unsigned int			  m_uRegionCount;			///< 分区集合中分区个数
	unsigned int	  	      m_uBlockCount;			///< 分区集合包括的Block总数
	unsigned int 	          m_uSize;				///< 分区集合的总大小，以字节为单位
	unsigned int 	          m_uMaxBlockSize;		///< 分区集合中,最大Block的大小,以字节为单位
	CSUDIFLASHRegion_S   	  m_asBlockRegion[CSUDI_FLASH_MAX_REGION_CNT];   ///< 分区集合中单个分区信息结构
}CSUDIFLASHRegionSuit_S;

/**@brief Flash分区集合类型

用于标识各种分区集合
@note 这是一个项目相关的分配,建议在项目初始化时进行配置
*/
typedef enum
{
	EM_UDI_FLASH_ALL,			///< 整块Flash分区集合,即包括了整块Flash所有分区的分区集合
	EM_UDI_FLASH_MAX			///< 边界值
}CSUDIFLASHRegionSuitType_E;

/**
@brief 从Flash中读取保存的数据 

@param[in] dwStartAddress 存储数据的Flash空间起始地址
@param[out] pucBuf 保存读出数据的缓冲区起始地址
@param[in] uDataLength 要读取的数据长度(以字节为单位)
@return CSUDI_SUCCESS表示读取成功，CSUDI_FAILURE表示读取失败
*/
CSUDI_Error_Code CSUDIFlashRead (CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf,  unsigned int uDataLength);

/**
@brief 往Flash中写入需要保存的数据 ,内建擦除

@param[in] dwStartAddress 存储数据的Flash空间地址
@param[in] pucBuf 要写入数据的缓冲区起始地址
@param[in] uDataLength 要写入的数据长度(以字节为单位)
@return CSUDI_SUCCESS表示写入成功，CSUDI_FAILURE表示写入失败
@note 已经在待机状态下调用本接口仍返回TRUE
*/
CSUDI_Error_Code CSUDIFlashWrite(CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf, unsigned int uDataLength);

/**
@brief 获取Flash分区集合信息
@param[in] eSuitType 需获取的Flash分区集合类型
@param[out] psFlashSuit 指向flash相关信息结构体的指针，详细定义见CSUDIFLASHRegionSuit_S
@return CSUDI_SUCCESS表示读取成功，CSUDI_FAILURE表示读取失败
@note 若平台不支持获取详细的Block信息，可将所有Flash划分为同一个Block，但不建议这样做。
*/
CSUDI_Error_Code CSUDIFlashGetInfo(CSUDIFLASHRegionSuitType_E eSuitType,CSUDIFLASHRegionSuit_S * psFlashSuit);

/**
@brief 擦除指定空间。

@param[in] dwStartAddress 待擦除空间的起始地址（绝对地址）
@param[in] uSpaceSize 待擦除空间大小（以字节为单位）
@return CSUDI_SUCCESS表示擦除操作成功,CSUDI_FAILURE表示擦除操作失败
@note 
- 擦除Flash仅要求按整Block进行，若传入的地址及大小不是整Block，可以返回CSUDIFLASH_ERROR_BAD_PARAMETER，但也
可以正常执行，但如果正常执行，需保证未指定擦除部分的内容不得被破坏。
- 如0-0x10000为一个Block，若要求擦除0-0x8000区域则可以返回CSUDIFLASH_ERROR_BAD_PARAMETER这种情况下Flash存储
的内容不发生任何变化；也可也返回CSUDI_SUCCESS，并且Flash0-0x7fff的内容被擦除,但其它Flash区域的内容，特别是
0x8000-0x10000的内容不得发生变化。
*/
CSUDI_Error_Code CSUDIFlashEraseSpace(CSUDI_UINT32 dwStartAddress, unsigned int uSpaceSize);

/**
@brief 往FLASH中写入需要保存的数据 ,为了提高效率实现时不带擦除

@param[in] dwStartAddress 待写入空间的起始地址（绝对地址）
@param[in] pucBuf 要写入数据的缓冲区起始地址
@param[in] uDataLength 待写入数据长度（以字节为单位）
@return CSUDI_SUCCESS 表示写操作成功，CSUDI_FAILURE表示写操作失败
@note 
- 若所写区域未进行事先擦除则等效于CSUDIFlashWrite
- 中间件根据flash的特性保证写入地址的数据都是从1->0而不会出现从0变为1的操作，因此该接口只需要在写入的地方直接按位修改数据即可
- 例如:会把flash中某地址对应的数据: 0x31 0x39 0x38 0x34 写为: 0x31 0x39 0x38 0x30,只需要把0x34中的一位1,变为0即可。
*/
CSUDI_Error_Code CSUDIFlashWriteWithoutErase( CSUDI_UINT32 dwStartAddress, const unsigned char * pucBuf, unsigned int uDataLength);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
