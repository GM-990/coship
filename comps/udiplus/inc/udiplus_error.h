/**@defgroup ERROR_PLUS ERROR Plus ����UDIPlus�и���ģ��Ĵ������

��ģ������MMCP������ģ���еĴ���������ֵ���壬���й�����UDI ERRORģ����ͬ��������ο�udi2_error.h

@warning  ��ģ�����MMCP�ڲ�ʹ�ã���������ģ����κζ��塢�ӿ��ṩ��MMCP�����ģ��ʵ�ã�������������UDI�����ɽӿ���
@warning  ����������Ҫ��MMCP�ⲿ������ͷ�ļ��а�����ͷ�ļ�

@version 2.0.0 2009/09/16 ����
@{
*/

#ifndef _UDIPLUS_ERROR_H_
#define _UDIPLUS_ERROR_H_

#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief ��ģ���������λ��ʼֵö��*/
enum
{
	EM_ERROR_START_UDI		= 0,		///< UDIģ���������λ��ʼֵ
	EM_ERROR_START_UDIPLUS	= 100,		///< UDIPlusģ���������λ��ʼֵ�����ر�ע���ֵ����udi2_plus.h�еĶ��屣�ֲ���ͻ
	
	EM_ERROR_START_MAX								///< �߽�ֵ
};

/**@brief ����ģ�����������ֵö�ٶ���

@note ÿ��ģ��Ĵ������ֵ��������ĸ������Ĵ����������:BAD_PARAMETER��NO_MEMORY,FEATURE_NOT_SUPPORTED,UNKNOWN_ERROR
*/
enum
{
	CSUDIPLUS_OS_ERROR_BASE			= (EM_ERROR_START_UDIPLUS+1) << 16,	///< OS Plusģ��Ĵ���������ֵ
	CSUDIPLUS_DEBUG_ERROR_BASE		= (EM_ERROR_START_UDIPLUS+2) << 16,	///< Debug Plusģ��Ĵ���������ֵ
	
	CSUDIPLUS_MAX_ERROR_BASE
};

#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_ERROR_H_

