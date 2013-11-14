/**@defgroup DEBUG DEBUG ��־��Ϣ���

@brief Debugģ���Ӧͷ�ļ�<udi2_debug.h>�������˻�����־����ӿڡ��ֶԸ�ģ���������˵����

- ���齫��־��ӡ��������ڡ�\n
- ʵ��ʱ���ر�ע������ַ����п��ܰ�����%s���ȵ������\n
- EastWin�ᴦ�����־����еĲ���������ֲ��������ġ�\n
- ���ڵ����������־��Ϣ�϶࣬��ģ���ʵ�����ر�ע��Ч�����⡣\n
- ע�⵱����߳�ͬʱ��������ӿ�ʱ��������ݲ��ɽ��桢������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.0 2009/8/22 �������߼�����,��ֲ���������������ӿ�,���˵��ͬʱ����
@version 1.0.1 2008/8/10 ����FAQ
@version 1.0.0 2008/7/25 ��ʼ�汾
@{
*/

#ifndef _UDI2_DEBUG_H_
#define _UDI2_DEBUG_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
@brief ��־�������

@param[in] pStr ��ʽ���õ���־����
@code 
printf("%s",pStr)//�ο�ʵ��
@endcode
@note 
- �����������ض�ģ����ã����Ƽ��ⲿֱ����������������г�ֵ����ɣ����������ϲ�����ƹ��ϲ��ӡ���ƽӿ�ֱ��ʹ�øú��������\n
- EastWin�ᴦ�����־����еĲ���������ֲ��������ġ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
void CSDebugDefaultOutputString(const char * pStr );


#ifdef	__cplusplus
}
#endif

/**@}*/

#endif 

