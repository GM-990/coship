#ifndef _CS_NATIVECALL_H_
#define _CS_NATIVECALL_H_

#include "mmcp_typedef.h"
#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

/**自定义native方法调用返回字符串长度*/
#define CSNCALL_RET_VALUE_LEN 1024



/**
用户自定义函数回调
@param szFunction IN 回调函数名称(与注册时的函数名称一致)
@param pszParams IN -参数列表
@param nParamCount IN -参数个数
@param szOutReturn OUT 输出函数调用返回值(字符串长度为CSNCALL_RET_VALUE_LEN)
@return 返回函数调用是否成功(只有返回函数调用成功时，返回值szOutReturn才有效)
@note 如果函数调用无返回值，则请填充szOutReturn 为空字符串或者不填充szOutReturn
*/
typedef BOOL (*CSNCALLNativeCall_Fn)(const char* szFunction,const char** pszParams,int nParamCount,char* szOutReturn);

/**
用户自定义函数回调
@param szFunction IN 回调函数名称(与注册时的函数名称一致)
@param pszParams IN -参数列表
@param nParamCount IN -参数个数
@param szOutReturn OUT 输出函数调用返回值(字符串长度为CSNCALL_RET_VALUE_LEN)
@return 返回函数调用是否成功(只有返回函数调用成功时，返回值szOutReturn才有效)
@note 如果函数调用无返回值，则请填充szOutReturn 为空字符串或者不填充szOutReturn
*/
typedef jobjectArray (*CSNCALLNativeObjectCall_Fn)(JNIEnv *env, const char* szFunction,jobjectArray jParams);


/**
注册用户自定义native函数实现回调
@param szFunction IN 回调函数名称
@param fnCallback IN -自定义native函数实现
@return 注册是否成功
@note 同一函数名称的函数回调只能注册一次，如果函数名称的回调已注册过，则再次注册返回失败
*/
BOOL CSNCALLRegist(const char* szFunction,CSNCALLNativeCall_Fn fnCallback);


/**
注册用户自定义native函数实现回调
@param szFunction IN 回调函数名称
@param fnCallback IN -自定义native函数实现
@return 注册是否成功
@note 同一函数名称的函数回调只能注册一次，如果函数名称的回调已注册过，则再次注册返回失败
*/
BOOL CSNCALLRegistObjFun(const char* szFunction,CSNCALLNativeObjectCall_Fn fnCallback);

/**
注销用户自定义native函数实现回调
@param szFunction IN 回调函数名称
*/
BOOL CSNCALLUnRegist(const char* szFunction);

#ifdef __cplusplus
}
#endif

#endif
