/**@defgroup CSUDIOSG PLUS�����ӿ�
@brief ��ģ����Ҫ������OSG PLUS �ӿ�

@version 0.1 2010/5/29 �����ĵ�һ���汾
@{
*/

#ifndef  _CS_UDIOSGPLUS_H
#define  _CS_UDIOSGPLUS_H

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_screen.h"

#ifdef  __cplusplus
extern "C" {
#endif


/**@brief BLITʱע��Ĺ��Ӻ����ĵ���ģʽ */
typedef enum {
	EM_UDIOSG_BLITHOOK_CALL_FIRST, ///< �ȵ��ù��Ӻ������ٵ���blit����������
	EM_UDIOSG_BLITHOOK_CALL_LAST  ///< �ȵ���blit�������������ٵ��ù��Ӻ���
} CSUDIOSGBlitCallMode_E;


/**
@brief Osg Blit�Ĺ��Ӻ�������
@param[in] pPram ���Ӻ����Ĳ�������ʱû���壬�����Ժ���չ
@return CSUDI_SUCCESS ��ʾ���Ӻ���ִ�гɹ���CSUDI_FAILURE��ʾ���Ӻ���ִ��ʧ��
*/
typedef CSUDI_Error_Code (* CSUDIOSGBlitHook_F)(void *pPram);


/**
@brief ����OSD�����Ļ

����OSD�����Ļ,�Ǹ��廹�Ǳ���
@param[in] unScreen �����ĻFlag
@param[in] pvData ������ֵ,�����ݼ�CSUDIHDMIPropertyType_Eö���жԸ������Ե�����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
@note unScreenĿǰ����ΪEM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD��EM_UDI_VOUT_DEVICE_SD|EM_UDI_VOUT_DEVICE_HD
*/
CSUDI_Error_Code CSUDIOSGSetOutputScreen(unsigned int unScreen);


/**
@brief ע��blit�����Ĺ��Ӻ���

@param[in] eMode ���Ӻ����ĵ���ģʽ����CSUDIOSGBlitCallMode_E���塣
@param[in] fHook ���Ӻ�������CSUDIOSGBlitHook_F���塣
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ ��
@note 
- ֻ����ע��һ�����Ӻ���
- ͨ��ע�ṳ�Ӻ���ΪNULL����ע����ע��Ĺ��Ӻ���
*/
CSUDI_Error_Code CSUDIOSGSetBlitHook(CSUDIOSGBlitCallMode_E eMode, CSUDIOSGBlitHook_F fHook);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif  /* _CS_UDIOSGPLUS_H */


