/**@defgroup COSHIPLIB COSHIPLIB �ṩӦ�ó�����coship�淶BootloaderͼƬ����������Ƚӿڡ�
@brief BOOTSHOWģ���Ӧͷ�ļ�<csbl_bootshow.h>��

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



CSBOOTADPicHeader   ͼƬ��Ϣͷ,����ͼƬ����,crcֵ,����.
(ע��Ӧ����booter�б�֤����ͷ�ı��ݻ���,����block)

				     
---------------------------
stInfo[0]    ����ͼƬ0,ֻ������,����block
............
stInfo[31]    ����ͼƬ31,ֻ������,����block
----------------------------

*********************************************************/

#define MAX_BOOTPIC_COUNT			(32)      ///booter���֧�ֶ�����ͼƬ
#define MAX_USER_DEFINE_LENGTH	(256) ///����CSUDIBOOTSHOWUnitInfo�ṹ�峤��
#define PRESENT_FLAG			(0xaabbccdd)  ///��ʾ��flash�ռ���ͼƬ�Ƿ����
#define DATA_CRC_LENGTH		(256)  ///ȱʡ״̬��ͼƬ������CRC�ĳ���, ����Ϊȫ������
#define BOOT_SHOW_EXT_MAGIC  (0xA7B7C7D7) ///CSBOOTADPicHeader.dwMagic�ֶ�, �����ṹ��汾��ħ����,�������־��Ϊ�µĽṹ��
#define DEFAULT_BOOTPIC_SIZE    (0x20000)   ///ͼƬĬ�ϴ�С����Ҫ�ı�ʱ����CSBLSetBOOTSHOWPicHeadSize�޸�
#define SD_720_576_24BIT_SIZE   (195000) ///����ͼƬ�����ֵ
#define HD_1280_720_24BIT_SIZE (0x80000) ///����ͼƬ�����ֵ
/**
@brief :  ÿ�ſ���ͼƬ��������
*/
typedef struct 
{
    DWORD dwPresentFlag;//ͼƬ�Ƿ����    PRESENT_FLAG��ʾ����
    DWORD dwCompressSize;//�洢��flash�ռ��ͼƬ����
    DWORD dwCRC;         //�洢��flash�ռ��ͼƬCRCЧ��ֵ,�п��ܲ���ȫУ�飬����ֻУ��һ����
    DWORD dwCRCLength;   //ѹ��ͼƬCRCЧ�鳤��,���ǵ�CRCЧ�����ʾ�ٶȵ�Ӱ��,ֻЧ��ѹ��ͼƬ��dwCRCLength���ֽ�
    DWORD dwBootPicStartAddress; ///����ͼƬ��ŵ�flash��ַ
    DWORD adwUserDefine[MAX_USER_DEFINE_LENGTH/sizeof(DWORD)]; ///CSUDIBOOTSHOWUnitInfo��Ϣ�ṹ���Ա
}CSBOOTADPicInfo;


/**
@brief :  booter����ͼƬ������ܽṹ��
*/
typedef struct 
{
    DWORD bShowIndex;  ///Ϊ����booter��Ҫ��ʾ��ͼƬ����
    CSUDIBOOTSHOWCapability sCapability; ///Booter����ʾ��������� 
    CSBOOTADPicInfo stInfo[MAX_BOOTPIC_COUNT]; ///Ϊÿ��ͼƬ��������
    DWORD dwMagic;    ///����ṹ���ħ��ֵ,�̶�0xA7B7C7D7
    DWORD dwLength; ///�˽ṹ��crc�ĳ���, ���ֶ�֮ǰ������
    DWORD dwCrc; ///�νṹ��crcֵ
}CSBOOTADPicHeader;


/**
@brief ͼƬ��ʾ��ʼ��

@param[in] dwFlashAddressΪͼƬͷ��Ϣ��ʼ��ַ
@param[in] dwFlashSizeΪͼƬ�����������flash�ܴ�С, (����ͼƬ������Ϣͷ0x20000�ֽ�)
@param[in] dwMaxBootPicSizeΪÿ��ͼƬ�Ĵ�С(��������Ϣͷ),Ϊflash�ռ����Ĵ�С.
@return ����CSUDI_Error_Code�ж���Ĵ���

@note �˽ӿڱ����������ӿ�֮ǰ����
*/
CSUDI_Error_Code CSUDIBOOTSHOWShowInit(DWORD dwFlashAddress,DWORD dwFlashSize,DWORD dwMaxBootPicSize);


/**
@brief : booter����

booter������ʱ�򣬻�ȡ��ǰ��Ҫ��ʾ��ͼƬ��Ӧ�ĵ�ַ����С


pdwAddr:[out],   ��ȡͼƬ��flash�еĵ�ַ
pdwSize:[out],   ��ȡͼƬ�Ĵ�С
*/
CSUDI_Error_Code CSBLGetBootLogoInfo(unsigned int * pdwAddr, unsigned int * pdwSize);


/**
@brief ����ͼƬͷ

@param[in] dwShowIndexΪ��������ͼƬͷ�Ժ���Ҫ��ʾ��ͼƬ��index
@param[in] dwFlashAddressΪͼƬͷ��Ϣ��ʼ��ַ
@param[in] dwMaxBootPicSizeΪÿ��ͼƬ�Ĵ�С(��������Ϣͷ),Ϊflash�ռ����Ĵ�С.
@return ����CSUDI_Error_Code�ж���Ĵ���

@note �˽ӿڱ����������ӿ�֮ǰ����
*/
CSUDI_Error_Code CSUDIBOOTSHOWRecoveryPicHead(DWORD dwShowIndex, DWORD dwFlashAddress,DWORD dwMaxBootPicSize);

#ifdef __cplusplus
}
#endif

#endif

