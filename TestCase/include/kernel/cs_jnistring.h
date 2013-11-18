/**
@brief ��ģ����Ҫ������JNI String ����ؽӿ�

@note This file provide several universal string convert functions .

@version 1.0 Douchengquan       Create
@{
*/
#ifndef _CS_JNI_STRING_H_
#define _CS_JNI_STRING_H_
#include "jni.h"
#include "cs_strings.h"
#ifdef  __cplusplus
extern "C" {
#endif


/**
@brief ��jstring�����ʽת���ɱ����롣 �����������ڲ���������Դ��
	��Ҫ��CSJNSReleaseStringPlatformChars ��Ե����ͷ���Դ��
	������ ��MMCP_SetDefaultCodePage���õı������� @see cs_strings.h
@param[in] pEnv  JAVA ��������
@param[in] jStr  ��ת����jstring����
@return  char*  ת����ı����ַ���  
*/
char * CSJNSGetStringPlatformChars(JNIEnv *pEnv, jstring jStr);

/**
@brief �ͷ�CSJNSGetStringPlatformChars�����������������Դ��
	��CSJNSGetStringPlatformChars��ԣ�������ʽ���á�
@param[in] pcStr  ���ͷŵ��ڴ�ָ��
*/
void CSJNSReleaseStringPlatformChars(char *pcStr);

/**
@brief �������ַ���ת����jstring����
@param[in] pEnv  JAVA ��������
@param[in] nCodingType  pcBuf�ַ�����������  @see cs_strings.h
             ���Դ��� CP_DEFAULT  ������ 
              �� CP_UNICODE_BE    ���UNICODE �����ַ���
@param[in] pcBuf    ��ű����ַ�����bufferָ��
@param[in] nBufLen  ��ű����ַ�����buffer����
@return  jstring  ת����jstring���� 
*/
jstring CSJNSChars2JString(JNIEnv *pEnv, int nCodingType, const char *pcBuf, int nBufLen);


#ifdef  __cplusplus
}
#endif
/**@}*/
#endif
