#ifndef CSSHELL_BL_PRIVATE_H
#define CSSHELL_BL_PRIVATE_H
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum 
{
	EM_CSSHELL_BL_SUCCESS = 0,	///<�ɹ���ȡ�򱣴���Ϣ
	EM_CSSHELL_BL_TYPE_ERROR,	///<Ҫ��ȡ�򱣴����Ϣ���Ͳ�����
	EM_CSSHELL_BL_LENGTH_ERROR, ///<������Ϣ���������ͳ��ȴ���
	EM_CSSHELL_BL_NULL_POINT,	///<������Ϣ�Ľṹ��Ϊ��
	EM_CSSHELL_BL_INVALID_VALUE,///<Ҫд�����Ϣ�Ƿ���
	EM_CSSHELL_BL_READ_ERROR,	///<��ȡflashʱ����
	EM_CSSHELL_BL_WRITE_ERROR	///<��Ϣд��flashʱ����һ����дflashʧ��
}CSShelllBLErroCode_E;

/**@brief ��ȡbootloader��Ϣ�ӿ�.bootloader��Ϣ�����ڽ�������ʾ��Ҳ������ota����ʱ�õ�

@param[in] pcType ��Ϣ���ͣ��ɾ���shellȷ����
@param[in] pvValue ָ��洢��ȡ��Ϣ���ڴ��ַ�����صĿ����ǵ���ֵ��Ҳ������һ�����ݽṹ
@param[in] nLength valueֵ�ĳ���
@return �ɹ�����EM_CSSHELL_BL_SUCCESS��ʧ���򷵻ش������ֵ
@note ���ݸ������bootloader��ͬ����ʵ��Ҳ��ͬ��
*/
CSShelllBLErroCode_E CSShellBLGetSysInfo(char* pcType ,void* pvValue, int nlength);

/**@brief ����ota��Ϣ�ӿ�.
@param[in] pcType ��Ϣ���ͣ��ɾ���shellȷ����
@param[in] pvValue ָ��洢��ȡ��Ϣ���ڴ��ַ��
@param[in] nLength valueֵ�ĳ���
@return �ɹ�����EM_CSSHELL_BL_SUCCESS��ʧ���򷵻ش������ֵ
@note ���ݸ������bootloader��ͬ����ʵ��Ҳ��ͬ��
*/

CSShelllBLErroCode_E CSShellBLSetSysInfo(char* pcType ,void* pvValue, int nlength);


#ifdef __cplusplus
}
#endif

#endif

