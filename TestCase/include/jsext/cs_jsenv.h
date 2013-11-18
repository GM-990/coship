
#ifndef CS_JSENV_H
#define CS_JSENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"

/******************************************************************************
Function Name:    CSJSLoadPropertyFile
	
Description:
	根据文件名字加载property文件
Input:
    pcFileName 指定的文件名

Output:
    无

Return Value:
    property的句柄
Other:
    在调用此方法之前，必须配置好环境变量"STB.systemPath"与"STB.innerPath"
*******************************************************************************/
HCSHANDLE CSJSLoadPropertyFile(const char *pcFileName);

/******************************************************************************
Function Name:    JSGetNonVolatileEnv
	
Description:
	获取浏览器的环境变量。
Input:
    pcFileName 指定的文件名
	pcKey 环境变量的键值
	nLength 输出参数pcValue的长度

Output:
    pcValue 与key对应的值

Return Value:
	环境变量的值，如果key没有对应的环境变量，返回空字符串。
Other:
*******************************************************************************/
int JSGetNonVolatileEnv(const char *pcFileName, const char * pcKey, char *pcValue, int nLength);

/******************************************************************************
Function Name:    JSGetNonVolatileEnvValueLen
	
Description:
	获取环境变量的长度。
Input:
    pcFileName 指定的文件名
	pcKey 环境变量的键值

Output:
    

Return Value:
	环境变量的长度，
	如果key没有对应的环境变量，返回0；
	错误、文件不存在、key不存在，返回-1；
Other:
*******************************************************************************/
int JSGetNonVolatileEnvValueLen(const char *pcFileName, const char * pcKey);

/******************************************************************************
Function Name:    JSSetNonVolatileEnv
	
Description:
	设置浏览器的环境变量。
Input:
    pcFileName 指定的文件名
	pcKey 环境变量的键值
	pcValue  环境变量的值 
Output:
    None

Return Value:
	
Other:
*******************************************************************************/
int JSSetNonVolatileEnv(const char *pcFileName, const char * pcKey, const char * pcValue);

/******************************************************************************
Function Name:    JSSaveEnv
	
Description:
	保存可关机保存的环境变量
Input:
	无

Output:
    None

Return Value:
	成功返回0，失败返回其他
Other:
*******************************************************************************/
int JSSaveEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSRevertEnv
	
Description:
	从存储设备中恢复环境变量
Input:
	无
Output:
    None

Return Value:
	成功返回0，失败返回其他
Other:
*******************************************************************************/
int JSRevertEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSSetDefaultEnv
	
Description:
	把环境变量恢复出厂设置
Input:
	无
Output:
    None

Return Value:
	成功返回0，失败返回其他
Other:
*******************************************************************************/
int JSSetDefaultEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSDeleteEnv
	
Description:
	从存储设备中删除对应pcKey的环境变量
Input:
	无
Output:
    None

Return Value:
	成功返回0，失败返回其他
Other:
*******************************************************************************/
int JSDeleteEnv(const char* pcFileName, char *pcKey);

/******************************************************************************
Function Name:    JSDeleteAllEnv
	
Description:
	从存储设备中删除所有环境变量
Input:
	无
Output:
    None

Return Value:
	成功返回0，失败返回其他
Other:
*******************************************************************************/
int JSDeleteAllEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSGetNonVolatileEnvCount

Description:
获取环境变量个数。
Input:
pcFileName 指定的文件名
Output:
None

Return Value:
	指定配置文件中的key总个数
Other:
*******************************************************************************/
int JSGetNonVolatileEnvCount(const char *pcFileName);

/******************************************************************************
Function Name:    JSGetNonVolatileEnvAt

Description:
获取指定序号的环境变量的key。
Input:
pcFileName 指定的文件名
nIndex 序号
pcKey 环境变量的key值，输出参数
dwKeyLen  环境变量的key值的buffer长度 
Output:
None

Return Value:
	获取指定配置文件中对应index的key的值
Other:
*******************************************************************************/
int JSGetNonVolatileEnvAt(const char *pcFileName, int nIndex, char * pcKey, DWORD dwKeyLen);

/******************************************************************************
Function Name: CSJSLoadPropertyFileFromEeprom

Description:
    根据文件名字从eeprom中加载property文件
    在应用上再根据此文件名获取内容的时候，实际上都是从eeprom中获取的了
Input:
    pcFileName - 指定的文件名(注:只能填文件名不能填路径)
    dwFlashAddress - 存储此文件的eeprom的起始地址
    dwSize - 存储此文件的eeprom的空间大小

Output:
    无

Return Value:
    property的句柄
Other:
    调用此接口时要保证此机顶盒是有eeprom存储器的
    如果指定的文件的内容太长，超出了限制，则超出的部分会被丢弃
*******************************************************************************/
HCSHANDLE CSJSLoadPropertyFileFromEeprom(const char *pcFileName, DWORD dwFlashAddress, DWORD dwSize);

/******************************************************************************
Function Name: CSJSClearPropertyFileEeprom

Description:
    根据文件名字清除校验位，以便达到恢复出厂设置的效果
Input:
    pcFileName - 指定的文件名(注:只能填文件名不能填路径)

Output:
    无

Return Value:
    FALSE表示失败，TRUE表示成功
Other:
    调用此接口时要保证此机顶盒是有eeprom存储器的
*******************************************************************************/
BOOL CSJSClearPropertyFileEeprom(const char *pcFileName);

/******************************************************************************
Function Name:    JSAddPropertyFile

Description:
	将property文件添加到结构体中去
Input:
	pcFilePath:文件全路径
	nPropertyType:文件类型
Output:
None

Return Value:
	成功添加或已添加过返回对应的HCSHANDLE，失败返回0
Other:
*******************************************************************************/
HCSHANDLE JSAddPropertyFile(char *pcFilePath, int nPropertyType);

/**
广东 环境变量变化通知
@param szKey - 环境变量.
@param szValue - 变化后的值
@param pUserData 用户自定义参数(注册回调函数时传入的参数)
*/
typedef void (*JSGDOnEnvChanged_F)(const char* szKey,const char* szValue,void* pUserData);

/******************************************************************************
Function Name:    JSGDSetEnv
	
Description:
	设置广东项目浏览器的环境变量。
Input:
    pcFileName 指定的文件名
	pcKey 环境变量的键值
	pcValue  环境变量的值 
Output:
    None

Return Value:
	
Other:
*******************************************************************************/
BOOL JSGDSetEnv(const char *pcFileName, const char *pcKey, const char *pcValue);


/******************************************************************************
Function Name:    JSGDGetEnv
	
Description:
	获取广东项目浏览器的环境变量。
Input:
    pcFileName 指定的文件名
	pcKey 环境变量的键值
	pcValue  环境变量的值 
	nLength  pcValue的长度
Output:
    pcValue

Return Value:
	
Other:
*******************************************************************************/
BOOL JSGDGetEnv(const char *pcFileName, const char * pcKey, char *pcValue, int nLength);

/**
注册广东环境变量变化监听者
@param szKey - 需要监听的环境变量，(NULL表示监听所有环境变量变化)
@param fnCb - 监听者回调函数
@param pUserData 用户自定义参数(回调通知时回传给回调函数)
*/
BOOL JSGDENVAddListner(const char* szKey,JSGDOnEnvChanged_F fnCb,void* pUserData);

/**
移除广东环境变量监听者
@param fnCb - 监听者回调函数
*/
void JSGDENVRemoveListner(JSGDOnEnvChanged_F fnCb);

/**
移除广东环境变量监听者
@param szKey - 需要监听的环境变量，(NULL表示监听所有环境变量变化)
@param fnCb - 监听者回调函数
*/
void JSGDENVRemoveKeyListner(const char* szKey, JSGDOnEnvChanged_F fnCb);


/******************************************************************************
Function Name:    JSDestoryAllEnv
	
Description:
	从内存中删除某个配置文件(或数据表)相关内容
Input:
	文件名称
Output:
    None

Return Value:
	成功返回0，失败返回其他
Other:
*******************************************************************************/
int JSDestoryAllEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSRemoveAllEnv
	
Description:
	从存储设备中删除配置文件，并从内存中删除某个配置文件(或数据表)相关内容
Input:
	文件名称
Output:
    None

Return Value:
	成功返回0，失败返回其他
Other:
*******************************************************************************/
int JSRemoveAllEnv(const char* pcFileName);

BOOL CSNGBSetEnv(const char *pcKey, const char *pcValue);

BOOL CSNGBGetEnv(const char *pcKey, char *pcValue, int nLength);


#ifdef __cplusplus
}
#endif

#endif  /* CS_JSENV_H */




