/**@defgroup DEBUG_PLUS DEBUG Plus UDI DEBUG ģ��Ĳ���ģ�飬�ṩ�����ӡ���ƽӿ�

@brief ��ģ�����ڰ�ģ�����ؼ��ּ��������־��Ϣ��������Ϳ��ơ�
@brief ÿһ��ģ��(�ؼ���)��Ӧһ���������
ÿһ����־������Ҳ��Ӧһ����������һ��ģ����(�ؼ���)��
��������ļ��𲻵��ڶ�Ӧģ����������ʱ���������־��Ϣ��
CSASSERT��CSDEBUG��CSVDEBUG��ȫ��д�������ڷ�Debugģʽ�¹ر����Դ�ӡ��

@warning  ��ģ�����MMCP�ڲ�ʹ�ã���������ģ����κζ��塢�ӿ��ṩ��MMCP�����ģ��ʵ�ã�������������UDI�����ɽӿ���
@warning  ����������Ҫ��MMCP�ⲿ������ͷ�ļ��а�����ͷ�ļ�
 
@version 2009/09/16 ����UDI2.1�汾�Ķ��壬���µ�����ģ��
- ��ʼ�汾
@{
*/

#include <stdarg.h>
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_debug.h"
#include "udiplus_error.h"

#ifndef _UDIPLUS_DEBUG_H_
#define _UDIPLUS_DEBUG_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief Debug Plus ģ����ش������ֵ*/
enum
{
	CSUDIDEBUGPLUS_ERROR_BAD_PARAMETER = CSUDIPLUS_DEBUG_ERROR_BASE,	///< ��������
	CSUDIDEBUGPLUS_ERROR_FEATURE_NOT_SUPPORTED,							///< ��֧�ִ˲���
	CSUDIDEBUGPLUS_ERROR_UNKNOWN_ERROR,									///< һ�����
	CSUDIDEBUGPLUS_ERROR_NO_MEMORY,										///< ���ڴ���ô���
	
	CSUDIDEBUGPLUS_ERROR_MAX											///< �߽�ֵ
};

/**@brief ���Լ�*/
#define DEBUG_LEVEL		    (0)

/**@brief ���̵���ʾ��*/
#define INFO_LEVEL 		    (1)

/**@brief ���Ǵ��󵫿�����Ҫ����*/
#define NOTICE_LEVEL		(0x0FFFFFFF)

/**@brief �ɻָ����ϼ�*/
#define WARN_LEVEL 		    (0x1FFFFFFF)

/**@brief ��ȷ���Ƿ�ɻָ����ϼ�*/
#define ERROR_LEVEL		    (0x3FFFFFFF)

/**@brief �޷��ָ��Ĺ��ϼ�*/
#define FATAL_LEVEL		    (0x7FFFFFFD)

/**@brief command line interface��������shell�����ȱ����������־*/
#define CLI_LEVEL		    (0x7FFFFFFE)

/**@brief ��ֹ����ֻ������CSDebugSetDefaultLevel��CSDebugSet�����ýӿڣ���������CSDebug������ӿ�*/
#define DISABLE_LEVEL		(0x7FFFFFFF)

/**@brief CSDebugSetʱ��ʹ�øú��ʾ��������ģ�����־����*/
#define MODULE_NAME_ALL     ("*all")

/**@cond */
#ifdef _DEBUG
	#define CSASSERT(expression)  \
	do{ \
		if(!(expression)) \
		CSDebug("CSDebug",FATAL_LEVEL,"Assertion: \"%s\" failed, in file %s, line %d\n", \
				#expression, __FILE__, __LINE__); \
	}while(0)
	
	#define CSASSERT_FAILED(exp) (!(exp)? (CSDebug("CSDebug", FATAL_LEVEL, "Assertion \"%s\" failed, in file %s line %d\n", #exp, __FILE__,__LINE__), 1) : 0) 

	#define CSDEBUG    	CSDebug
	#define CSVDEBUG	CSVDebug
#else
	#define CSASSERT(x)
	#define CSASSERT_FAILED(exp) (!(exp))
/**@brief Ϊ����release�汾ɾ�����д�ӡ��Ϣ��MMCP_FORCE_DELETE_DEBUGINFO����menuconfig�ж���*/
#ifdef MMCP_RELEASE_DEL_LOG
	#define CSDEBUG		1?(void)0:CSDebugNULL
	#define CSVDEBUG	1?(void)0:CSVDebugNULL	
	#define CSDebug		1?(void)0:CSDebugNULL
	#define CSVDebug	1?(void)0:CSVDebugNULL
#else
	#define CSDEBUG		1?(void)0:CSDebug
	#define CSVDEBUG	1?(void)0:CSVDebug
#endif
#endif
/**@endcond*/

/**
@brief ��־��Ϣ����ص���������

��ʽ���õ���־��Ϣ���ջ����һ������ʽ�ĺ���������������Ӧ�������п�����־��Ϣ�������
��ɶ���һ������ʽ�ĺ�������CSDebugRegisterOutputCallback
@param[in] pStr ��ʽ���õ���־�ַ������������
@see CSDebugRegisterOutputCallback
*/
typedef  void ( *CSDebugOutPutString )( const char* pStr );

/**
@brief ģ���ʼ��������Ĭ�ϼ���

һ����˵��Ӧ�þ�����ĵط����г�ʼ�������ڿ����Ժ��ģ��Ĵ�ӡ
@param[in] nDefaultLevel Ĭ�ϼ���
@return ֮ǰ��Ĭ�ϼ����״ε��ø�ֵ��ȷ��
@note �״ε����κδ�ģ�����ĺ���ʱ��������ȷָ����ģ���ӡ���𣬷���ᱻ���ó�nDefaultLevel
@note ���к�������ģ����������ʹ�ø�ֵ���ɶ�ε��ã���ÿ�ε��ú���Ժ�������ģ��������
@note ���Լ���Ƚϲ�����ѧ�Ƚϣ��������� < 0 < ����
@note ����һ��Ӧ�����ýӿڣ�������MMCP�����е��ñ��ӿ����ô�ӡ����
*/
int CSDebugSetDefaultLevel(int nDefaultLevel);

/**
@brief ����ģ����־�������

�״����ü�Ϊע�ᣬģ����ģ�����ַ���Ϊ�ؼ��֣����ģ���Ѿ�ע�������ı�����־����
@param[in] pcModuleName ģ�����ؼ���
@param[in] nOutputLevel ģ�����ú����־����
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ����ģ����������ָ�룬�뱣֤ע��ʱģ��������Ч�ԣ�����ֱ��ʹ���ַ�����������
@note ʹ��MODULE_NAME_ALL����Ϊģ������������ʾ��������ģ�����־����
@note ����һ��Ӧ�����ýӿڣ�������MMCP�����е��ñ��ӿ����ô�ӡ����
*/
CSUDI_Error_Code CSDebugSet(const char* pcModuleName,int nOutputLevel);

/**
@brief ��ȡ������ע���ģ����

ģ���������������־�У����ppModules��nMaxCount��Ϊ0Ҳ��洢�����У�������CSUDI_NULL����
@param[out] ppModules ���ڴ洢ģ������ָ�����飬����ΪCSUDI_NULL
@param[in] nMaxCount ָ������Ĵ�С
@return ʵ��ģ��������ppModules��nMaxCount�޹�
@note ����ܴ洢nMaxCount-1��ģ������ppModules�У���Ϊ��β��NULL��ռ��һ����
@note ����ppModules��nMaxCount���ȡֵ������ģ���������������־�ļ��
@note ����һ��Ӧ�����ýӿڣ������ر��������򲻵���MMCP�����е��ñ��ӿڲ�ѯģ�����ơ�
*/
int CSDebugGetModuleInfo(const char** ppModules,int nMaxCount);

/**
@brief ע����־�������

������־���ݾ���ʽ�����ж��ͷ�Ҫ���������Ҫ������Ը�ʽ���õ��ַ������ñ�����
@param[in] OutputCallback ָ�����������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ע���֮ǰ��������������ٵ��ã����������û�е��ù�����ʹ��Ĭ����������������
@note ����������Ŀ�أ��罫��־�ض����ļ��У������Ƽ�ʹ�ñ���������ΪĬ�Ϻ�������ƽ̨�Ƽ����������
@note ���ر�ע�ⲻҪ�ڻص����ٴε���CSDebug������CSDebug�ĺ���(��������)��������ܻ��������޵ݹ����
@note Ĭ�ϵ���־�������ΪCSUDIDEBUGOutputString������Ҫ�ָ�Ĭ��ʱҲ����ʹ��CSUDIDEBUGOutputStringΪ�������ñ�����
@note ����һ��Ӧ�����ýӿڣ������ر��������򲻵���MMCP�����е��ñ��ӿ��ض�����־���
*/
CSUDI_Error_Code CSDebugRegisterOutputCallback(CSDebugOutPutString OutputCallback);

#if !defined(_DEBUG) && defined(MMCP_RELEASE_DEL_LOG) 
/**
@brief ����2������������Ϊ�˽���������⣻��ֱֹ�ӵ��ô˽ӿڣ�����Ҳ����Ҫʵ�ִ˽ӿڣ�
*/
void CSDebugNULL(const char * pcModuleName, int nOutputLevel, const char *pcFormat, ...);
void CSVDebugNULL(const char * pcModuleName, int nOutputLevel, const char *pcFormat,va_list arg);
#else
/**
@brief ���ض��������ĳ��ģ�����־��Ϣ

��nOutputLevel >= pcModuleName����־����ʱ�������־��Ϣ������ֱ�ӷ���
@param[in] pcModuleName ģ�����ؼ���
@param[in] nOutputLevel ��־�������
@param[in] pcFormat ��־�ַ�������ʽ��ͬprintf�Ķ���
@note ����ģ����������ָ�룬�뱣֤ע��ʱģ��������Ч�ԣ�����ֱ��ʹ���ַ�����������
@note nOutputLevel���ܵ���DISABLE_LEVEL������Զ�����ΪCLI_LEVEL
*/
void CSDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat, ...);

/**
@brief V�汾��CSDebug

��CSDebug�Ĺ�ϵ�൱��vprintf��printf�Ĺ�ϵ
@param[in] pcModuleName ģ�����ؼ���
@param[in] nOutputLevel ��־�������
@param[in] pcFormat ��־�ַ�������ʽ��ͬprintf�Ķ���
@param[in] arg ��־�ַ����еĲ����б�
@note ����ģ����������ָ�룬�뱣֤ע��ʱģ��������Ч�ԣ�����ֱ��ʹ���ַ�����������
@note nOutputLevel���ܵ���DISABLE_LEVEL������Զ�����ΪCLI_LEVEL
*/
void CSVDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat,va_list arg);
#endif

#ifdef	__cplusplus
}
#endif
/** @} */

#endif

