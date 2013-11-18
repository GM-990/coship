/**@defgroup INPUT INPUT �ṩ�����豸�����ӿ�
@brief INPUTģ���Ӧͷ�ļ�<udi2_input.h>�����������������豸�Ŀ��ƽӿڡ��ֶԸ�ģ��
��������˵����

- �����ĸ�ģ��İ�����ң������ǰ���ȣ���ͨ����ģ��֪ͨ��Eastwin��
- ң������ʵ�ʰ����빦�ܼ��Ķ�Ӧ��ϵ������Ŀ�Ĳ�ͬ����Ӫ�̡�COSHIP��ͬȷ�������磺
ĳ��Ŀ��Ҫ�ڰ�F1��ʱ�������뷨������Ҫ���յ�F1��ʱ����CS_VK_INPUT��ֵ��
- ��ģ��δ����ļ�ֵһ���͸����Ӧ�ò㣬��JavaScript����JavaӦ���С���Eastwin�ڲ�
���в��ְ������߼����Ӧ����CSUDI_VK_OK��Ӧ���볬���ӵȣ���������ȫ͸�������Բ���
����Eastwin����ֻ��͸���������иı���Щ��ֵ��

@{
*/

#ifndef _CSSHELL_IRCARD_H_
#define _CSSHELL_IRCARD_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "JSMessage.h"

#define  IRCARD_BASE					( JSCALL_USERDEF_MSG_START+100 )//��Ŀ����ʹ������
//IRCARD ��Ϣ
#define  JSCALL_IRCARD_EMPTY					( IRCARD_BASE+ 1 )//���ڳ�ʼ��
#define  JSCALL_IRCARD_DEVIC_ID					( IRCARD_BASE+ 2 )//�豸ID
#define  JSCALL_IRCARD_KEYDOWN				( IRCARD_BASE+ 3 )//����֪ͨ
#define  JSCALL_IRCARD_BRUSH_OK				( IRCARD_BASE+ 4 )//ˢ���ɹ�
#define  JSCALL_IRCARD_BRUSH_FAILED			( IRCARD_BASE+ 5 )//ˢ��ʧ��
#define  JSCALL_IRCARD_BRUSH_TIMEOUT			( IRCARD_BASE+ 6 )//ˢ����ʱ
#define  JSCALL_IRCARD_DATA_ERROR				( IRCARD_BASE+ 7 )//���ݴ���
#define  JSCALL_IRCARD_NO_BRUSH				( IRCARD_BASE+ 8)//���ݴ���
#define  JSCALL_IRCARD_UNDER_VOLTAGE			( IRCARD_BASE+ 9)//���Ƿѹ
#define  JSCALL_IRCARD_PASSWORD_NOT_ENOUGH	( IRCARD_BASE+ 10)//����δ��������벻��
#define  JSCALL_IRCARD_PASSWORD_OVERFLOW	( IRCARD_BASE+ 11)//���볬��6λ
#define  JSCALL_IRCARD_PASSWORD_SHOW			( IRCARD_BASE+ 12)//��ʾ����
#define  JSCALL_IRCARD_SEND_ENCRYPT_DATA		( IRCARD_BASE+ 13)//������������
#define  JSCALL_IRCARD_SEND_FREE_DATA			( IRCARD_BASE+ 14)//������������

#define  JSCALL_IRCARD_UNKNOWN				( IRCARD_BASE+ 15)//δ֪֪ͨ

#ifdef __cplusplus
extern "C" {
#endif



CSUDI_Error_Code CSShellIrCardInit(void);
CSUDI_Error_Code CSShellIrCardDestroy(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif  



