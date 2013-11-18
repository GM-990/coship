/**
*FileName:  APPCONFIG.H
*Copyright (c) 2008， coship。
*
* \file
*        File Description, Please add you description here.
*
*Modify History:
*Version     Date        Author          Description
*--------    ----------  --------        ------------

*/

#ifndef APPCONFIG_H
#define APPCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define  FLASH_BLOCK_SIZE		(128*1024)

#if defined(PLATFORM_X86)/**/
#elif defined(PLATFORM_BCM7309)/**/
#define FLASH_BASE_ADDRESS		(0xbf000000)
#define FLASH_BUFFER_SIZE		(0x1000000)	//16M
//=================== 哈尔滨FLASH BLOCK PARTITION Start:===============================
//for sysinfo
#define FLSADDR_SYSINFO				( 0x1FC00 ) 
//for STB S/N
#define FLSADDR_SN				    ( FLASH_BASE_ADDRESS + 0x1FE00 )
//for mac
#define FLSADDR_MAC				    ( FLASH_BASE_ADDRESS + 0x1FF00 ) 

//dm的数据区
#define FLASH_DM_DATA_OFF		(0x00f40000)
#define DM_DATA_SIZE				(FLASH_BLOCK_SIZE)
//commondata的数据区
#define FLASH_COMMON_DATA_OFF		(0x00F60000)
#define COMMON_DATA_SIZE			(64*1024)

//systemParam的数据区
#define FLASH_SYSPARAM_DATA_OFF		(0x00F80000)
#define SYSPARAM_DATA_SIZE			(64*1024)

//ca
#define FLASH_CA_OFF				(0x00FA0000)
#define CA_DATA_SIZE				(64*1024)

//for loaderpt
#define FLSADDR_LOADER_PT			( FLASH_BASE_ADDRESS + 0x00FC0000)
//for syspartition
#define FLSADDR_SYSPARTITION 		( FLASH_BASE_ADDRESS + 0x00FC8000)
//for sysconfig
#define FLSADDR_SYSCONFIG 			( FLASH_BASE_ADDRESS + 0x00FCC000)

//for syspartitionback
#define FLSADDR_SYSPARTITIONBACK 	( FLASH_BASE_ADDRESS + 0x00FE0000)
//for sysconfigback
#define FLSADDR_SYSCONFIGBACK 		( FLASH_BASE_ADDRESS + 0x00FE4000)


#elif defined(PLATFORM_BCM740X)
/*
mtd0: 0008000 00020000 "LA_Booter"			1e 000 000 
mtd1: 0014000 00020000 "loader_kernel"		1E 080 000                                     
mtd2: 0050000 00020000 "loader_rootfs"		1E 1C0 000                                     
mtd3: 0004000 00020000 "BCA_MAC_HDCP_FH"	1E 6C0 000                                     
mtd4: 0038000 00020000 "APP_kernel"			1E 700 000                                     
mtd5: 013e000 00020000 "APP_rootfs"			1E A80 000                                     
mtd6: 0010000 00020000 "APP_DATA"			1F E60 000                                     
mtd7: 000a000 00020000 "BL_DATA"			1F F60 000 //1M的可写空间，共16个block,供csfs用
						20 000 000 //共10个block  
*/					

//===================================================================================
//====== 内存空间资源分配 =====
//扇区
#define FLASH_BASE_ADDRESS              (0X1E000000)//哈尔滨应该是0x1E000000
#define FLASH_BUFFER_SIZE				(0x2000000) //32M

//=================== 哈尔滨可写的FLASH BLOCK PARTITION Start:，用来保存邮件===============================
//=================== 哈尔滨FLASH BLOCK PARTITION Start:===============================
//for sysinfo
#define FLSADDR_SYSINFO				( 0x00040000 ) 
//for STB S/N
#define FLSADDR_SN				    ( FLASH_BASE_ADDRESS + 0x00680000 )
//for mac
#define FLSADDR_MAC				    ( FLASH_BASE_ADDRESS + 0x00688000) 

//dm的数据区
#define FLASH_DM_DATA_OFF			(0x01EE0000)	
#define DM_DATA_SIZE				(FLASH_BLOCK_SIZE)
//commondata的数据区
#define FLASH_COMMON_DATA_OFF		(0x01F00000)
#define COMMON_DATA_SIZE			(FLASH_BLOCK_SIZE)

//systemParam的数据区
#define FLASH_SYSPARAM_DATA_OFF		(0x01F20000)
#define SYSPARAM_DATA_SIZE			(FLASH_BLOCK_SIZE)

//ca
#define FLASH_CA_OFF				(0x01F40000)
#define CA_DATA_SIZE				(FLASH_BLOCK_SIZE)

//for loaderpt
#define FLSADDR_LOADER_PT			( FLASH_BASE_ADDRESS + 0x01F60000)
//for syspartition
#define FLSADDR_SYSPARTITION 		( FLASH_BASE_ADDRESS + 0x01F80000)
//for sysconfig
#define FLSADDR_SYSCONFIG 			( FLASH_BASE_ADDRESS + 0x01FA0000)

//for syspartitionback
#define FLSADDR_SYSPARTITIONBACK 	( FLASH_BASE_ADDRESS + 0x01FC0000)
//for sysconfigback
#define FLSADDR_SYSCONFIGBACK 		( FLASH_BASE_ADDRESS + 0x01FE0000)


#define  PVR_DATA_START_ADDR     (FLASH_BASE_ADDRESS+0x01EC0000)
#define  PVR_DATA_SIZE               (2*1024)


/*目前最后一个分区还有前8个block空闲*/

#elif defined(PLATFORM_ST40)/**/
	#define  FLASH_BUFFER_BASE				(0x1E000000)
	#define FLASH_BASE_ADDRESS              (0x1E000000)
	#define  FLASH_BUFFER_SIZE				(0x2000000) //32M
	#define APP_DATA_SIZE     (0x100000)
	#define FLASH_DM_DATA_OFF   (0x1E60000)
	#define FLSADDR_SYSINFO				( 0x0) 
#endif


#ifdef __cplusplus
}
#endif

#endif  /* APPCONFIG_H */

/* End of File */




