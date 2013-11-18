/**@defgroup UTI ��Universal Transport Interface��������̫�͹�˾���ý���оƬ����
SmartCard�ṩ��ͳһ�ӿڣ����ڶ�UTIоƬ�ļĴ������ж�д����

@brief ��ģ����Ҫ������ͳһ��UTI��ʼ���뷴��ʼ���ӿڲ�������

@{
*/

#ifndef _CS_UTI_H_
#define _CS_UTI_H_

#ifdef __cplusplus
extern "C" 
{
#endif

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "cs_hdicommon_plus.h"

typedef enum
{
    EM_UTI_TYPE_1201,
    EM_UTI_TYPE_1203,
    EM_UTI_TYPE_MAX
}CSUTIType_E;


typedef struct 
{
	CSUTIType_E				m_eUtiType;			///< UTI�ͺ�
	unsigned int			m_uI2CIndex;		///< UTI I2C����
	unsigned int			m_uResetGPIONum;	///< UTI reset gpio ����
	CSHDITsMode				m_eTSOutMode;		///< TS�����ģʽ
	unsigned int			m_dwReserved0;		///< ��������0
	unsigned int			m_dwReserved1;		///< ��������1
}CSUTIInitParam_S;

/**
@brief UTIģ���ʼ��

@param[in] uUtiIndex  UTI�豸����
@param[in] psUtiInitParam UTI��ʼ����ز�������
@return 
	- �ɹ����ظ���ʼ������CSUDI_SUCCESS
	- ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUTIInit(unsigned int uUtiIndex, const CSUTIInitParam_S *psUtiInitParam);


 /**
@brief UTIģ�鷴��ʼ��

@param[in] uUtiIndex  UTI�豸����
@return 
	- �ɹ����ظ�����ʼ������CSUDI_SUCCESS
	- ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUTITerm(unsigned int uUtiIndex);


#ifdef __cplusplus
}
#endif

/** @} */

#endif




