/**@defgroup CSENCODER[CSENCODER]ͬ����Ŀר��ENCODER�����ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨�޹�ENCODER�����ӿ�
@brief ��֪���Ǹ�ʲô����������udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
 
@version 0.3 2008/11/5  ��һ��ϸ���汾
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/


#ifndef _CS_ENCODER_H_
#define _CS_ENCODER_H_

#include "cs_typedef.h"
#include "CSHDI_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


/**@brief ENCODER Format����*/
typedef enum
{
	CSVENC_PAL_BDGHI,					/**< ��Ҫ���˵�� */
	CSVENC_PAL_COMBINATION_N,	/**< ��Ҫ���˵�� */
	CSVENC_PAL_M,							/**< ��Ҫ���˵�� */
	CSVENC_PAL_60,						/**< ��Ҫ���˵�� */
	CSVENC_NTSC_M,						/**< ��Ҫ���˵�� */
	CSVENC_NTSC_443,					/**< ��Ҫ���˵�� */
	CSVENC_SECAM_BGLK1				/**< ��Ҫ���˵�� */
} T_CSVENC_FORMAT;

/**
@brief ִ��ENCODER��ʼ����

@param[in] enmI2CIndex �������ڵ�I2C���ߵ�����
@param[in] bTTXFlag �Ƿ�֧��ͼ�ĵ��ӹ���
@return
- CSHDI_SUCCESS:	��ʼ���ɹ�
- CSHDI_FAILURE��	��ʼ��ʧ��
*/
CSHDI_Error_t CSENCInit(CSHDII2CIndex enmI2CIndex, BOOL bTTXFlag);

/**
@brief ����ENCODER�豸��

@return CSHDI_SUCCESS��ʾ����ɹ��� CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSENCEnable(void);

/**
@brief ����ENCODER�����ʽ��

@param[in] enmVENCFormat ENCODER��������ĸ�ʽ������
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSENCSetEncoderFormat(T_CSVENC_FORMAT  enmVENCFormat);

/**
@brief ��ͼ�ĵ��ӹ���

@return CSHDI_SUCCESS��ʾ�򿪳ɹ���CSHDI_FAILURE��ʾ��ʧ�ܡ�
*/
CSHDI_Error_t CSENCOpenTeletext(void);

/**
@brief �ر�ͼ�ĵ��ӹ���

@return  success:CSHDI_SUCCESS��ʾ�رճɹ���fail:CSHDI_FAILURE��ʾ�ر�ʧ�ܡ�
*/
CSHDI_Error_t CSENCCloseTeletext(void);

/*
@brief ����ENCDOER����standby ģʽ

@return
- success:CSHDI_SUCCESS��ʾ���óɹ���
- fail:CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSENCStandBy (void);

/**
@brief �ر�ENCODER

@return 
- success:CSHDI_SUCCESS��ʾ�رճɹ���
- fail:CSHDI_FAILURE��ʾ�ر�ʧ�ܡ�
*/
CSHDI_Error_t CSENCDisable(void);

/**
@brief ������ɫ������

@return
- success:CSHDI_SUCCESS��ʾ�����ɹ���
- fail:CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSENCEnableColorBar(void);

/**
@brief �ر���ɫ������

@return
- success:CSHDI_SUCCESS��ʾ�رճɹ���
- fail:CSHDI_FAILURE��ʾ�ر�ʧ�ܡ�
*/
CSHDI_Error_t CSENCDisableColorBar(void);

/**
@brief ȡ�ø�ģ�������İ汾��

@param[out] pcVer The top address of Version Buffer
@param[in] nSize �Ƿ�֧��ͼ�ĵ��ӹ���
@return  success:CSHDI_SUCCESS��ʾ�رճɹ��� fail:CSHDI_FAILURE��ʾ�ر�ʧ�ܡ�
*/
int CSENCGetVersion(PCHAR pcVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

