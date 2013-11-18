/**
@brief 本模块主要定义了JNI String 的相关接口

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
@brief 将jstring编码格式转换成本地码。 本函数由于内部申请了资源，
	需要和CSJNSReleaseStringPlatformChars 配对调用释放资源。
	本地码 即MMCP_SetDefaultCodePage设置的编码类型 @see cs_strings.h
@param[in] pEnv  JAVA 环境变量
@param[in] jStr  待转换的jstring对象
@return  char*  转换后的本地字符串  
*/
char * CSJNSGetStringPlatformChars(JNIEnv *pEnv, jstring jStr);

/**
@brief 释放CSJNSGetStringPlatformChars函数调用中申请的资源，
	和CSJNSGetStringPlatformChars配对，必须显式调用。
@param[in] pcStr  待释放的内存指针
*/
void CSJNSReleaseStringPlatformChars(char *pcStr);

/**
@brief 将本地字符串转换成jstring对象。
@param[in] pEnv  JAVA 环境变量
@param[in] nCodingType  pcBuf字符串编码类型  @see cs_strings.h
             可以传入 CP_DEFAULT  本地码 
              或 CP_UNICODE_BE    大端UNICODE 类型字符串
@param[in] pcBuf    存放本地字符串的buffer指针
@param[in] nBufLen  存放本地字符串的buffer长度
@return  jstring  转换后jstring对象 
*/
jstring CSJNSChars2JString(JNIEnv *pEnv, int nCodingType, const char *pcBuf, int nBufLen);


#ifdef  __cplusplus
}
#endif
/**@}*/
#endif
