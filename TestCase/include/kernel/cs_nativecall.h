#ifndef _CS_NATIVECALL_H_
#define _CS_NATIVECALL_H_

#include "mmcp_typedef.h"
#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

/**�Զ���native�������÷����ַ�������*/
#define CSNCALL_RET_VALUE_LEN 1024



/**
�û��Զ��庯���ص�
@param szFunction IN �ص���������(��ע��ʱ�ĺ�������һ��)
@param pszParams IN -�����б�
@param nParamCount IN -��������
@param szOutReturn OUT ����������÷���ֵ(�ַ�������ΪCSNCALL_RET_VALUE_LEN)
@return ���غ��������Ƿ�ɹ�(ֻ�з��غ������óɹ�ʱ������ֵszOutReturn����Ч)
@note ������������޷���ֵ���������szOutReturn Ϊ���ַ������߲����szOutReturn
*/
typedef BOOL (*CSNCALLNativeCall_Fn)(const char* szFunction,const char** pszParams,int nParamCount,char* szOutReturn);

/**
�û��Զ��庯���ص�
@param szFunction IN �ص���������(��ע��ʱ�ĺ�������һ��)
@param pszParams IN -�����б�
@param nParamCount IN -��������
@param szOutReturn OUT ����������÷���ֵ(�ַ�������ΪCSNCALL_RET_VALUE_LEN)
@return ���غ��������Ƿ�ɹ�(ֻ�з��غ������óɹ�ʱ������ֵszOutReturn����Ч)
@note ������������޷���ֵ���������szOutReturn Ϊ���ַ������߲����szOutReturn
*/
typedef jobjectArray (*CSNCALLNativeObjectCall_Fn)(JNIEnv *env, const char* szFunction,jobjectArray jParams);


/**
ע���û��Զ���native����ʵ�ֻص�
@param szFunction IN �ص���������
@param fnCallback IN -�Զ���native����ʵ��
@return ע���Ƿ�ɹ�
@note ͬһ�������Ƶĺ����ص�ֻ��ע��һ�Σ�����������ƵĻص���ע��������ٴ�ע�᷵��ʧ��
*/
BOOL CSNCALLRegist(const char* szFunction,CSNCALLNativeCall_Fn fnCallback);


/**
ע���û��Զ���native����ʵ�ֻص�
@param szFunction IN �ص���������
@param fnCallback IN -�Զ���native����ʵ��
@return ע���Ƿ�ɹ�
@note ͬһ�������Ƶĺ����ص�ֻ��ע��һ�Σ�����������ƵĻص���ע��������ٴ�ע�᷵��ʧ��
*/
BOOL CSNCALLRegistObjFun(const char* szFunction,CSNCALLNativeObjectCall_Fn fnCallback);

/**
ע���û��Զ���native����ʵ�ֻص�
@param szFunction IN �ص���������
*/
BOOL CSNCALLUnRegist(const char* szFunction);

#ifdef __cplusplus
}
#endif

#endif
