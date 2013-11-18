/******************************************************************

@defgroup  ��ȡstb�����м��ݵ�Ӳ����Ϣ


@brief �����˲�ͬӲ�����õĽӿ�
�ֶԸ�ģ���������˵����


(1)����flash���ü���, �ǵ�nor ? ��nand? ����nor+nand?   
(2)����ram�ļ���
(3)ǰ������͡���led? lcd? gpio? ������ǰ���?
(4)booter�����Ĳ���ϵͳ��ecos?  OS21?  ucos?  linux?  


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


//stb  cfg ͷ�������ļ�
#define M_SYS_STBCFG_TAG_LENGTH       		(16)
#define M_SYS_STBCFG_TAG_MAGIC_ASCII		"COSHIPBLSTBCFG"
#define M_SYS_MAX_STBCFG_SIZE				(1024)


/**@brief flash�������Ͷ���*/
typedef enum
{
	EM_BL_CFG_FLASH_NOR, ///��nor flash
	EM_BL_CFG_FLASH_NAND,///��NAND flash
	EM_BL_CFG_FLASH_NOR_NAND,///nor flash + NAND flash
	EM_BL_CFG_FLASH_NUM ///����
} CSBLCFGFlashType_E;



/**@brief ����ϵͳ�������Ͷ���*/
typedef enum
{
	EM_BL_CFG_OS_LINUX, /// 
	EM_BL_CFG_OS_UCOSII,/// 
	EM_BL_CFG_OS_OS21,/// 	
	EM_BL_CFG_OS_ECOS,/// 	
	EM_BL_CFG_OS_NUM ///����
} CSBLCFGOSType_E;


/************************************************************
@brief    STB���ýṹ��(Ŀǰ�̶�M_SYS_MAX_STBCFG_SIZE  �ֽ�(2011.10.14))
@note:
***********************************************************/
typedef struct
{
	char m_aTag[M_SYS_STBCFG_TAG_LENGTH]; /***��־=== M_SYS_STBCFG_TAG_MAGIC_ASCII ***/

	unsigned int m_dwCrc32;	/***mpegcrc32, �����ݽṹ��CRC��������m_aTag, m_dwCrc32,m_dwLength�ֶ�,��m_adwReserve�ֶο�ʼ***/
	unsigned int m_dwLength;	/***mpegcrc32, ���㳤��Ϊm_dwLength ***/
	unsigned int  m_adwReserve[2]; /*****����****/

	CSBLCFGFlashType_E m_eFlashType; ///flash��������
	unsigned int m_dwRamSize;		///RAM��С
	CSUDIPanelType_E m_ePanelType;   ///ǰ�������, EM_PANEL_TYPE_MAX��ʾû��ǰ���
	CSBLCFGOSType_E m_eAppOSType;      ///app��Ӧ�Ĳ���ϵͳ

	CSBLCFGOSType_E m_eLoaderOSType;      ///loader��Ӧ�Ĳ���ϵͳ
	unsigned int m_dwSTB[256-13]; /*****����****/
} CSBLSTBcfgInfo_S;


/************************************************************
@brief ��flash�����ȡӲ�����õȵȲ���

@dwAdrr[in]: 		stb_cfg.bin�ļ���flash�ĵ�ַ
@pStbCfg[out]:  	��������ýṹ��


@return �ɹ�����CSUDI_SUCCESS, ʧ�ܷ�������
************************************************************/
CSUDI_Error_Code  CSBLGetSTBCfg(DWORD dwAdrr, CSBLSTBcfgInfo_S * pStbCfg);



#ifdef	__cplusplus
}
#endif


#endif


