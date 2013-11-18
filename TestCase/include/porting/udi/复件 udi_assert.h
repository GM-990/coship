/**@defgroup CSASSERT print ��־��Ϣ���
@brief ��ģ�����ڰ�ģ�����ؼ��ּ��������־��Ϣ��������Ϳ��ơ�
@brief ÿһ��ģ��(�ؼ���)��Ӧһ���������\n
ÿһ����־������Ҳ��Ӧһ����������һ��ģ����(�ؼ���)\n
��������ļ��𲻵��ڶ�Ӧģ����������ʱ���������־��Ϣ\n
CSASSERT��CSDEBUG��CSVDEBUG��ȫ��д�������ڷ�Debugģʽ�¹ر����Դ�ӡ\n
��ģ����ṩʵ�ֲο�Դ����
 
@version UDI_ASSERT_0.1
- ��ʼ�汾
@{
*/
#ifndef _UDI_ASSERT_H_
#define _UDI_ASSERT_H_

#include "udiplus_debug.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
	#include <assert.h>
	#define CSTRACE    USPPrint
#else
	#ifdef _DEBUG
		#define CSTRACE USPPrint
	#else
		#define CSTRACE 1?(void)0:USPPrint
	#endif

#endif

void USPPrint(int nLevel,const char *format, ...);


#ifdef	__cplusplus
}
#endif

/** @} */

#endif 
