/**@defgroup UDI2FLASH  FLASH ��ģ����Ҫ����������Ķ�д�͸�ʽ��(erase)��ؽӿ�
@brief FLASHģ���Ӧͷ�ļ�<udi2_flash.h>��������Flash�Ĳ��������ýӿ� ��

����Flash�ĵ�ַ��ͨ��GetInfo�ӿڻ�ȡ����MMCP��CloudTV ���м��ƽ̨����֪��Ҳ�����ľ����ṩ������Ի��Ǿ��Ե�ַ��GetInfo��Flash��д�����ӿ���Ϻü��ɡ�MMCP��CloudTV ���м��ƽ̨����ͨ����ֱַ�Ӷ�ȡFlash�����ݡ�

���˻�����Flash�����ӿڣ�Flashģ���������һ������--Flash���ýӿ�����Ŀ��صġ�ͨ���ýӿڣ��ɻ�ȡ�������MMCP��CloudTV ���м��ƽ̨����Flash����MMCP��CloudTV ���м��ƽ̨�������Ŀ�Ĳ�ͬ�ͷ���Flash����Ĳ�ͬ��������Щ����һ����˵�������MMCP��CloudTV ���м��ƽ̨������Ӧ�ó�����������¼����UDIFS����Booterloader����MAC��ַ�����к���¼����OTA��������ȡ�

MMCP��CloudTV ���м��ƽ̨ʹ��Region��RegionSuit������������ʾFlash���������

Region��һ����������ͼʾ����Flash�е�Region������ʾFlash��ʵ�ʴ��ڵģ������ģ���ͬ��С������Block��

@image HTML udi_flash_region.png
RegionSuit��һ��Flash�����ϵĸ������ʾ��Ŀ�ϣ���һ��Flash���ָ�ĳ�����ģ����MMCP��CloudTV ���м��ƽ̨ʹ�á�RegionSuit�����ǲ������ġ���ͼʾ����RegionSuit�ĸ��

@image HTML udi_flash_regionsuit.png

��Ҫע����ǣ�flash��д�����ĵ�ַ���Ǿ��Ե�ַ��

@version 2.0.2 2009/8/24 ��һ���ۺ�������޸ĵİ汾
@version 2.0.1 2009/07/24 ����
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


/**@brief FLASH �豸��ش������ֵ*/
enum
{
	CSUDIFLASH_ERROR_BAD_PARAMETER = CSUDI_FLASH_ERROR_BASE,   
	CSUDIFLASH_ERROR_INVALID_ADDR,
	CSUDIFLASH_ERROR_UNKNOWN
};


/**@brief Flash������Ϣ���ݽṹ

һ����������<B>����</B>����ͬ��С�����ɸ�Block
*/
typedef struct  
{
	CSUDI_UINT32     m_dwStartAddress;			 ///< ������ʼ��ַ,���ڶ�д����
	unsigned int     m_uBlockSize;		         ///< ����Block��С,����������Block�Ĵ�С����ͬ��
	unsigned int     m_uBlockCount;		         ///< ����Block����
}CSUDIFLASHRegion_S;

/**@brief Flash����������Ϣ���ݽṹ

@note Flash��������ʾ����Flash�������ɸ�������ɵļ���,�ṹ������ͳ�����ݾ��ڱ������еķ����в���
@note ���������е�����һ����������
@note ���������е�������ʼ��ַ����������
*/
typedef struct 
{
	unsigned int			  m_uRegionCount;			///< ���������з�������
	unsigned int	  	      m_uBlockCount;			///< �������ϰ�����Block����
	unsigned int 	          m_uSize;				///< �������ϵ��ܴ�С�����ֽ�Ϊ��λ
	unsigned int 	          m_uMaxBlockSize;		///< ����������,���Block�Ĵ�С,���ֽ�Ϊ��λ
	CSUDIFLASHRegion_S   	  m_asBlockRegion[CSUDI_FLASH_MAX_REGION_CNT];   ///< ���������е���������Ϣ�ṹ
}CSUDIFLASHRegionSuit_S;

/**@brief Flash������������

���ڱ�ʶ���ַ�������
@note ����һ����Ŀ��صķ���,��������Ŀ��ʼ��ʱ��������
*/
typedef enum
{
	EM_UDI_FLASH_ALL,			///< ����Flash��������,������������Flash���з����ķ�������
	EM_UDI_FLASH_MAX			///< �߽�ֵ
}CSUDIFLASHRegionSuitType_E;

/**
@brief ��Flash�ж�ȡ��������� 

@param[in] dwStartAddress �洢���ݵ�Flash�ռ���ʼ��ַ
@param[out] pucBuf ����������ݵĻ�������ʼ��ַ
@param[in] uDataLength Ҫ��ȡ�����ݳ���(���ֽ�Ϊ��λ)
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���CSUDI_FAILURE��ʾ��ȡʧ��
*/
CSUDI_Error_Code CSUDIFlashRead (CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf,  unsigned int uDataLength);

/**
@brief ��Flash��д����Ҫ��������� ,�ڽ�����

@param[in] dwStartAddress �洢���ݵ�Flash�ռ��ַ
@param[in] pucBuf Ҫд�����ݵĻ�������ʼ��ַ
@param[in] uDataLength Ҫд������ݳ���(���ֽ�Ϊ��λ)
@return CSUDI_SUCCESS��ʾд��ɹ���CSUDI_FAILURE��ʾд��ʧ��
@note �Ѿ��ڴ���״̬�µ��ñ��ӿ��Է���TRUE
*/
CSUDI_Error_Code CSUDIFlashWrite(CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf, unsigned int uDataLength);

/**
@brief ��ȡFlash����������Ϣ
@param[in] eSuitType ���ȡ��Flash������������
@param[out] psFlashSuit ָ��flash�����Ϣ�ṹ���ָ�룬��ϸ�����CSUDIFLASHRegionSuit_S
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���CSUDI_FAILURE��ʾ��ȡʧ��
@note ��ƽ̨��֧�ֻ�ȡ��ϸ��Block��Ϣ���ɽ�����Flash����Ϊͬһ��Block������������������
*/
CSUDI_Error_Code CSUDIFlashGetInfo(CSUDIFLASHRegionSuitType_E eSuitType,CSUDIFLASHRegionSuit_S * psFlashSuit);

/**
@brief ����ָ���ռ䡣

@param[in] dwStartAddress �������ռ����ʼ��ַ�����Ե�ַ��
@param[in] uSpaceSize �������ռ��С�����ֽ�Ϊ��λ��
@return CSUDI_SUCCESS��ʾ���������ɹ�,CSUDI_FAILURE��ʾ��������ʧ��
@note 
- ����Flash��Ҫ����Block���У�������ĵ�ַ����С������Block�����Է���CSUDIFLASH_ERROR_BAD_PARAMETER����Ҳ
��������ִ�У����������ִ�У��豣֤δָ���������ֵ����ݲ��ñ��ƻ���
- ��0-0x10000Ϊһ��Block����Ҫ�����0-0x8000��������Է���CSUDIFLASH_ERROR_BAD_PARAMETER���������Flash�洢
�����ݲ������κα仯��Ҳ��Ҳ����CSUDI_SUCCESS������Flash0-0x7fff�����ݱ�����,������Flash��������ݣ��ر���
0x8000-0x10000�����ݲ��÷����仯��
*/
CSUDI_Error_Code CSUDIFlashEraseSpace(CSUDI_UINT32 dwStartAddress, unsigned int uSpaceSize);

/**
@brief ��FLASH��д����Ҫ��������� ,Ϊ�����Ч��ʵ��ʱ��������

@param[in] dwStartAddress ��д��ռ����ʼ��ַ�����Ե�ַ��
@param[in] pucBuf Ҫд�����ݵĻ�������ʼ��ַ
@param[in] uDataLength ��д�����ݳ��ȣ����ֽ�Ϊ��λ��
@return CSUDI_SUCCESS ��ʾд�����ɹ���CSUDI_FAILURE��ʾд����ʧ��
@note 
- ����д����δ�������Ȳ������Ч��CSUDIFlashWrite
- �м������flash�����Ա�֤д���ַ�����ݶ��Ǵ�1->0��������ִ�0��Ϊ1�Ĳ�������˸ýӿ�ֻ��Ҫ��д��ĵط�ֱ�Ӱ�λ�޸����ݼ���
- ����:���flash��ĳ��ַ��Ӧ������: 0x31 0x39 0x38 0x34 дΪ: 0x31 0x39 0x38 0x30,ֻ��Ҫ��0x34�е�һλ1,��Ϊ0���ɡ�
*/
CSUDI_Error_Code CSUDIFlashWriteWithoutErase( CSUDI_UINT32 dwStartAddress, const unsigned char * pucBuf, unsigned int uDataLength);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
