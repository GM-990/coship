/**@defgroup FS ������ͳһ��FS��ʼ����ؽӿڲ�������
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨�޹�FS�����ӿ�
@brief ����udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
@version 1.0.0 2009/11/18 ����
@{
*/

#ifndef _CS_FS_PLUS_H_
#define _CS_FS_PLUS_H_

#include  "udiplus_typedef.h"
#include "udi2_fs.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
@brief FS��ʼ��

@return
- CSHDI_ERROR_ALREADY_INITIALIZED��FS֮ǰ�Ѿ�����ʼ�����ˡ�
- CSHDI_FAILURE��FS��ʼ��ʧ�ܡ�
- CSHDI_SUCCESS��FS��ʼ���ɹ���
*/
CSUDI_Error_Code CSUDIFSInit(void);


/**
@brief ��FS������һ�����Ȳ�η���,��ʹFS�Զ����ظ÷���

@param[in] szPartition ������ 
@param[in] eFsType  ����������
@return 
- CSHDI_FAILURE����FS�����Ӵ˷���ʧ��
- CSHDI_SUCCESS����FS�����Ӵ˷����ɹ���
- CSHDI_ERROR_FEATURE_NOT_SUPPORTED : ��֧�ֵ��ļ�ϵͳ����
@note �ú�����CSUDIFSInit����ã����ڸ�֪FS�Զ�����ָ���ķ����豸��
   ͨ��FS���ڸ����豸�޷���⵽�����޷�����ʹ�á�
   ������FS�м���jffs��cramfs�����͵��豸��
*/
CSUDI_Error_Code CSUDIFSAddPartition(const char * szPartition, CSUDIFSPartitionType_E eFsType);

#ifdef __cplusplus
}
#endif

/** @} */

#endif



