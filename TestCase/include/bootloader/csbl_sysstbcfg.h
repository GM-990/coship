/******************************************************************

@defgroup  获取stb机顶盒兼容的硬件信息


@brief 定义了不同硬件配置的接口
现对该模块进行如下说明：


(1)多种flash配置兼容, 是单nor ? 单nand? 还是nor+nand?   
(2)多种ram的兼容
(3)前面板类型。是led? lcd? gpio? 还是无前面板?
(4)booter引导的操作系统是ecos?  OS21?  ucos?  linux?  


@version    1.0.0   bootloader     2011/9/14
@version    2.0.0   bootloader     2011/10/13
***********************************************************************/

#ifndef _CSBL_SYSSTBCFG_H_
#define _CSBL_SYSSTBCFG_H_

#include "udiplus_debug.h"
#include "udi2_error.h"
#include "cs_panel_plus.h"


#ifdef	__cplusplus
extern "C" {
#endif


//stb  cfg 头部配置文件
#define M_SYS_STBCFG_TAG_LENGTH       		(16)
#define M_SYS_STBCFG_TAG_MAGIC_ASCII		"COSHIPBLSTBCFG"
#define M_SYS_MAX_STBCFG_SIZE				(1024)


/**@brief flash配置类型定义*/
typedef enum
{
	EM_BL_CFG_FLASH_NOR, ///单nor flash
	EM_BL_CFG_FLASH_NAND,///单NAND flash
	EM_BL_CFG_FLASH_NOR_NAND,///nor flash + NAND flash
	EM_BL_CFG_FLASH_NUM ///上限
} CSBLCFGFlashType_E;



/**@brief 操作系统配置类型定义*/
typedef enum
{
	EM_BL_CFG_OS_LINUX, /// 
	EM_BL_CFG_OS_UCOSII,/// 
	EM_BL_CFG_OS_OS21,/// 	
	EM_BL_CFG_OS_ECOS,/// 	
	EM_BL_CFG_OS_NUM ///上限
} CSBLCFGOSType_E;


/************************************************************
@brief    STB配置结构体(目前固定M_SYS_MAX_STBCFG_SIZE  字节(2011.10.14))
@note:
***********************************************************/
typedef struct
{
	char m_aTag[M_SYS_STBCFG_TAG_LENGTH]; /***标志=== M_SYS_STBCFG_TAG_MAGIC_ASCII ***/

	unsigned int m_dwCrc32;	/***mpegcrc32, 本数据结构的CRC，不包括m_aTag, m_dwCrc32,m_dwLength字段,从m_adwReserve字段开始***/
	unsigned int m_dwLength;	/***mpegcrc32, 计算长度为m_dwLength ***/
	unsigned int  m_adwReserve[2]; /*****保留****/

	CSBLCFGFlashType_E m_eFlashType; ///flash配置类型
	unsigned int m_dwRamSize;		///RAM大小
	CSUDIPanelType_E m_ePanelType;   ///前面板配置, EM_PANEL_TYPE_MAX表示没有前面板
	CSBLCFGOSType_E m_eAppOSType;      ///app对应的操作系统

	CSBLCFGOSType_E m_eLoaderOSType;      ///loader对应的操作系统
	unsigned int m_dwSTB[256-13]; /*****保留****/
} CSBLSTBcfgInfo_S;


/************************************************************
@brief 从flash里面获取硬件配置等等参数

@dwAdrr[in]: 		stb_cfg.bin文件在flash的地址
@pStbCfg[out]:  	输出的配置结构体


@return 成功返回CSUDI_SUCCESS, 失败返回其他
************************************************************/
CSUDI_Error_Code  CSBLGetSTBCfg(DWORD dwAdrr, CSBLSTBcfgInfo_S * pStbCfg);



#ifdef	__cplusplus
}
#endif


#endif


