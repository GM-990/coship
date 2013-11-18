/**@defgroup COSHIPLIB COSHIPLIB 提供应用程序与coship规范Bootloader图片升级、保存等接口。
@brief BOOTSHOW模块对应头文件<csbl_bootshow.h>。

@version 1.0.0 2011/7/10  
@{
*/
#ifndef __CSBL_BOOTSHOW_H__
#define __CSBL_BOOTSHOW_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "udiplus_typedef.h"
#include "udi2_bootshow.h"

#define MODULE_NAME "BOOTPIC"


/*********************************************************


------------------------------------------



CSBOOTADPicHeader   图片信息头,包括图片张数,crc值,长度.
(注意应该在booter中保证数据头的备份机制,独立block)

				     
---------------------------
stInfo[0]    开机图片0,只有数据,独立block
............
stInfo[31]    开机图片31,只有数据,独立block
----------------------------

*********************************************************/

#define MAX_BOOTPIC_COUNT			(32)      ///booter最多支持多少张图片
#define MAX_USER_DEFINE_LENGTH	(256) ///大于CSUDIBOOTSHOWUnitInfo结构体长度
#define PRESENT_FLAG			(0xaabbccdd)  ///表示此flash空间中图片是否存在
#define DATA_CRC_LENGTH		(256)  ///缺省状态下图片数据做CRC的长度, 可以为全部长度
#define BOOT_SHOW_EXT_MAGIC  (0xA7B7C7D7) ///CSBOOTADPicHeader.dwMagic字段, 扩充后结构体版本的魔法数,有这个标志则为新的结构体
#define DEFAULT_BOOTPIC_SIZE    (0x20000)   ///图片默认大小，需要改变时可用CSBLSetBOOTSHOWPicHeadSize修改
#define SD_720_576_24BIT_SIZE   (195000) ///标清图片的最大值
#define HD_1280_720_24BIT_SIZE (0x80000) ///高清图片的最大值
/**
@brief :  每张开机图片的描述符
*/
typedef struct 
{
    DWORD dwPresentFlag;//图片是否存在    PRESENT_FLAG表示存在
    DWORD dwCompressSize;//存储在flash空间的图片长度
    DWORD dwCRC;         //存储在flash空间的图片CRC效验值,有可能不是全校验，或许只校验一部分
    DWORD dwCRCLength;   //压缩图片CRC效验长度,考虑到CRC效验对显示速度的影响,只效验压缩图片的dwCRCLength个字节
    DWORD dwBootPicStartAddress; ///本张图片存放的flash地址
    DWORD adwUserDefine[MAX_USER_DEFINE_LENGTH/sizeof(DWORD)]; ///CSUDIBOOTSHOWUnitInfo信息结构体成员
}CSBOOTADPicInfo;


/**
@brief :  booter开机图片管理的总结构体
*/
typedef struct 
{
    DWORD bShowIndex;  ///为开机booter需要显示的图片索引
    CSUDIBOOTSHOWCapability sCapability; ///Booter在显示方面的能力 
    CSBOOTADPicInfo stInfo[MAX_BOOTPIC_COUNT]; ///为每张图片的描述符
    DWORD dwMagic;    ///扩充结构体的魔法值,固定0xA7B7C7D7
    DWORD dwLength; ///此结构体crc的长度, 此字段之前的所有
    DWORD dwCrc; ///次结构体crc值
}CSBOOTADPicHeader;


/**
@brief 图片显示初始化

@param[in] dwFlashAddress为图片头信息起始地址
@param[in] dwFlashSize为图片区域所分配的flash总大小, (包括图片管理信息头0x20000字节)
@param[in] dwMaxBootPicSize为每张图片的大小(不包括信息头),为flash空间分配的大小.
@return 返回CSUDI_Error_Code中定义的错误

@note 此接口必须在其它接口之前调用
*/
CSUDI_Error_Code CSUDIBOOTSHOWShowInit(DWORD dwFlashAddress,DWORD dwFlashSize,DWORD dwMaxBootPicSize);


/**
@brief : booter调用

booter启动的时候，获取当前需要显示的图片对应的地址，大小


pdwAddr:[out],   获取图片在flash中的地址
pdwSize:[out],   获取图片的大小
*/
CSUDI_Error_Code CSBLGetBootLogoInfo(unsigned int * pdwAddr, unsigned int * pdwSize);


/**
@brief 设置图片头

@param[in] dwShowIndex为重新设置图片头以后，需要显示的图片的index
@param[in] dwFlashAddress为图片头信息起始地址
@param[in] dwMaxBootPicSize为每张图片的大小(不包括信息头),为flash空间分配的大小.
@return 返回CSUDI_Error_Code中定义的错误

@note 此接口必须在其它接口之前调用
*/
CSUDI_Error_Code CSUDIBOOTSHOWRecoveryPicHead(DWORD dwShowIndex, DWORD dwFlashAddress,DWORD dwMaxBootPicSize);

#ifdef __cplusplus
}
#endif

#endif

