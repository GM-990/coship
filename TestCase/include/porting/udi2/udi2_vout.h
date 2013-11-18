/**@defgroup VOUT VOUT ������Ƶ����豸���ƽӿ�
@brief ��ģ����Ҫ��������Ƶ����豸���ƽӿ�

@version 2.0.0 2009/12/29 ����
@{
*/
#ifndef _UDI2_VOUT_H_
#define _UDI2_VOUT_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief VOUT�������ֵ*/
enum
{
	CSUDIVOUT_ERROR_BAD_PARAMETER = CSUDI_VOUT_ERROR_BASE,  ///< �������
	CSUDIVOUT_ERROR_INVALID_DEVICE_ID,						///< �Ƿ��豸ID
	CSUDIVOUT_ERROR_FEATURE_NOT_SUPPORTED					///< ��֧�ִ˲���
};

/**@brief ��Ƶ����豸����*/
typedef enum
{
	EM_UDIVOUT_CVBS,	/**< CSBVS��Ƶ����ӿ� */
	EM_UDIVOUT_YC,	/**< S-Video ��Ƶ����ӿ� */
	EM_UDIVOUT_YUV, /**< ������Ƶ����ӿ� */
	EM_UDIVOUT_RF, /**< ��Ƶ�ź���Ƶ����ӿ� */
	EM_UDIVOUT_SCART, /**< SCART��Ƶ����ӿ� */
	EM_UDIVOUT_HDMI, /**< HDMI��Ƶ����ӿ� */
	EM_UDIVOUT_DVO,  /**< ������Ƶ���*/
	EM_UDIVOUT_NUM /**< ��Ƶ�ӿڸ��� */
}CSUDIVOUTDevices_E;

/**@brief ��Ƶ����˿ڵ���ɫ���� */
typedef enum
{
	EM_UDIVOUT_OUTPUT_TYPE_YUV, /**< YUVģʽ */
	EM_UDIVOUT_OUTPUT_TYPE_RGB, 	/**< RGBģʽ */
	EM_UDIVOUT_OUTPUT_TYPE_NUM	 /**< ģʽ���� */
}CSUDIVOUTOutputType_E;

/**
@brief ��ָ������Ƶ����豸

@param[in] eVoutDevice  ��Ƶ����豸��������μ�CSUDIVOUTDevices_E
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVOUTEnable(CSUDIVOUTDevices_E eVoutDevice);

/**
@brief �ر�ָ������Ƶ����豸

@param[in] eVoutDevice	��Ƶ����豸��������μ�CSUDIVOUTDevices_E
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVOUTDisable(CSUDIVOUTDevices_E eVoutDevice);

/**
@brief ������Ƶ����˿ڵ���ɫ����

@param[in] eVoutDevice ��Ƶ����豸,ֻ֧�ַ���(EM_UDIVOUT_YUV)/HDMI(EM_UDIVOUT_HDMI)
@param[in] eType �������ɫ���ͣ�RGB����YUV
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����������Ч
*/
CSUDI_Error_Code CSUDIVOUTSetOutputType(CSUDIVOUTDevices_E eVoutDevice, CSUDIVOUTOutputType_E eType);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

