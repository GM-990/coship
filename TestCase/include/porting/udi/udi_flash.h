/**@defgroup CSFLASH flash ��ģ����Ҫ����������Ķ�д�͸�ʽ��(erase)�ӿ�
@brief ��ģ����Ҫ����������Ķ�д�͸�ʽ��(erase)�ӿ�
@brief ������һ�ַǻӷ��Եİ뵼��洢оƬ,�����������,�ӽṹ�Ϸ���Ҫ��AND��NAND��NOR��DiNOR��,����NAND��NOR��Ŀǰ��Ϊ����������

@version 0.1 2008/10/23 ��ʼ�汾
@{
*/

#ifndef _UDI_FLASH_H_
#define _UDI_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "udiplus_typedef.h"


/**@brief flashоƬ��Ϣ���ݽṹ*/
typedef struct 
{
	int   		nFlashManuFactory ;	/**< the manufacturer ID of the flash used,such as AMD(0x01),Intel(0x89),ST(0x20),etc. */
	int	  	nFlashType ;			/**< The specific type of a chip Vendor.such as SST39VFXX(0x00~0x19),AMD29LVXX(0x20~0x39) etc*/
	DWORD 	dwCapacity;			/**< Flash��ʵ�ʿռ��С,���ֽ�Ϊ��λ*/
	int	  	nSectorNumber ;		/**< Total sector number*/
	DWORD 	dwSize;				/**< ʵ�ʿ��õ�flash�ܵĿռ��С�����ֽ�Ϊ��λ*/
	DWORD 	dwMaxSectorSize;		/**< ����block(section)�Ĵ�С�����ֽ�Ϊ��λ*/
	DWORD 	*pMemoryStart;		/**< ÿ��block����ʼ��ַ���飬�˵�ֱַ�����ڶ�дflash�����鳤�Ȳ�С��nSectorNumber*/
}T_CHIPInfo;

/**
@brief ��FLASH�ж�ȡ��������� 

@param[in] dwStartAddress �洢���ݵ�FLASH�ռ��ַ
@param[in] pbValue ����������ݵ��ڴ�ռ��ַ
@param[in] dwLength Ҫ��ȡ�����ݳ���(���ֽ�Ϊ��λ)
@return CSHDI_SUCCESS��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ��
*/
CSHDI_Error_t CSFLSReadFlash (DWORD dwStartAddress, PBYTE pbValue, DWORD dwLength);

/**
@brief ��FLASH��д����Ҫ��������� ,�ڽ�����

@param[in] dwStartAddress �洢���ݵ�FLASH�ռ��ַ
@param[in] pbValue Ҫд�����ݵ��ڴ�ռ��ַ
@param[in] dwLength Ҫд������ݳ���(���ֽ�Ϊ��λ)
@return CSHDI_SUCCESS��ʾд��ɹ���CSHDI_FAILURE��ʾд��ʧ��
*/
CSHDI_Error_t CSFLSWriteFlash (DWORD dwStartAddress, PBYTE pbValue, DWORD dwLength);

/**
@brief ȡ�����õ�FLASH��оƬ��

@return ���õ�FLASH��оƬ��
*/
int CSFLSGetFLSCount(void) ;

/**
@brief ��ʼ�����ȡflash����Ϣ�� 

@param[in] nChipIndex ��ǰFLASH��ţ��ӣ���ʼ���߽磺 nFlashIndex >= 0 && nFlashIndex < ���õ�flashƬ��
@param[out] tChipInfo ָ��flash�����Ϣ�ṹ���ָ�룬��ṹ��ͷ�ļ��ж��壺
@return CSHDI_SUCCESS��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ��
*/
CSHDI_Error_t CSFlashGetChipInfo (int nChipIndex, T_CHIPInfo *tChipInfo);

/**
@brief ����ָ���ռ䡣

@param[in] dwStartAddr �������ռ����ʼ��ַ�����Ե�ַ��
@param[in] dwSpaceSize �������ռ��С����λ��BYTE��
@return CSHDI_SUCCESS ��ʾ���������ɹ���CSHDI_FAILURE��ʾ��������ʧ�ܡ�
*/
CSHDI_Error_t CSFLSEraseSpace(DWORD dwStartAddr, DWORD dwSpaceSize);

/**
@brief ��FLASH��д����Ҫ��������� ,Ϊ�����Ч��ʵ��ʱ��������

@param[in] pbBuffer ��д������ָ��
@param[in] dwDataLength ��д�����ݳ��ȣ���λ��BYTE��
@param[in] dwFlashAddr ��д��ռ����ʼ��ַ�����Ե�ַ��
@return CSHDI_SUCCESS ��ʾд�����ɹ���CSHDI_FAILURE��ʾд����ʧ�ܡ�
*/
CSHDI_Error_t CSFLSWriteDataWithoutErase(PBYTE pbBuffer, DWORD dwDataLength, DWORD dwFlashAddr);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
