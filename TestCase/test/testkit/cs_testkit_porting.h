/**@defgroup ITESTKIT 定义集成测试用例框架(C语言部分)移植层接口

集成测试用例框架(C语言部分)仅依赖于本模块定义的接口实现。
本部分的实现大多是情况下均可以使用stdio.h中的同名方法实现，
只在极少stdio.h未实现或者实现不好的平台上，需要用其它方式实现.


@version 2.0.0 2009/09/17 初稿
@{
*/
#ifndef _ITESTKIT_PORTING_H_
#define _ITESTKIT_PORTING_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h> 

#include "cs_testkit.h"

#include "udi2_typedef.h"
#include "udiplus_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum
{
	EM_INPUT_UART_SYS=0,
	EM_INPUT_UART_UDI2,
	EM_INPUT_UART_UDI1,
	EM_INPUT_REMOTE
}CSTKPInputType_E;

typedef enum
{
	EM_TKP_FS_SYS=0,
	EM_TKP_FS_ROM,
	EM_TKP_FS_USB
}CSTKPFSType_E;

/**
@brief 测试框架命令行提示输出接口

必须为交互式打印输出
@note 这里的打印与Porting实现中的打印不一定要在一个地方，
比如ST平台上,日志打印一般在串口，而测试框架打印可以在终端上(用printf)
*/

/*******************I/O相关接口********************************/
int CSTKP_VPrintf(char *format,va_list arg);
/**
@brief 从输入设备（键盘、遥控器等）获取输入串
@param[in] char *s   用于存放输入字符串缓冲区首址
@param[in] int nSize   输入字符串缓冲区长度 
@param[out] 
@return  成功获取输入返回 输入字符串缓冲区首址
@return 没有输入或失败返回CSUDI_NULL
*/
char* CSTKP_Gets(char *s,int size);
void CSTKP_Flush(void);
void CSTKP_Exit(void);
/********************文件系统相关接口********************/
/**
@brief 打开文件
@param[in] const char* filename    待打开的文件的文件名
@param[in] const char* mode     打开方式,详见C99标准
@param[out] 
@return  文件句柄,失败返回CSUDI_NULL  
*/
CSUDI_HANDLE CSTKP_FOpen(const char* filename,const char* mode);
/**
@brief 关闭文件
@param[in] CSUDI_HANDLE hFile 文件句柄,CSTKP_FOpen的返回值
@param[out] 
@return 成功返回0
*/
int CSTKP_FClose(CSUDI_HANDLE hFile);
/**
@brief 读文件
@param[in] int nSize     单个元素大小（字节）
@param[in] int nMemb     要读出元素个数
@param[in] CSUDI_HANDLE hFile	 文件句柄,CSTKP_FOpen的返回值
@param[out] void * ptr  指向存放读出数据的内存
@return 成功读取元素个数,它可能少于nMemb，<0表示出错或者文件到结尾了 
*/
int CSTKP_FRead(void * ptr,int size,int nmemb,CSUDI_HANDLE hFile);
/**
@brief 写文件
@param[in] void * ptr 指向存放写入数据的内存
@param[in] int nSize     单个元素大小（字节）
@param[in] int nMemb     要写入元素个数
@param[in] CSUDI_HANDLE hFile	 文件句柄,CSTKP_FOpen的返回值
@param[out] 
@return 成功写入的元素个数 0:文件状态不发生任何变化
*/
int CSTKP_FWrite(const void * ptr,int size,int nmemb,CSUDI_HANDLE hFile);
/**
@brief  flush文件
@param[in] CSUDI_HANDLE hFile 文件句柄,CSTKP_FOpen的返回值
@param[out] 
@return 成功返回0
*/
int CSTKP_FFlush(CSUDI_HANDLE hFile);
/**
@brief seek文件
@param[in] CSUDI_HANDLE hFile 文件句柄,CSTKP_FOpen的返回值
@param[in] long int offset    移动偏移
@param[in] int whence    移动起始位置,SEEK_SET,SEEK_CUR,SEEK_END
@param[out] 
@return 成功返回0,其它情况返回其它值  
*/
int CSTKP_FSeek(CSUDI_HANDLE hFIle,long int offset,int whence);
/**
@brief  获取文件当前位置
@param[in] CSUDI_HANDLE hFile 文件句柄,CSTKP_FOpen的返回值
@param[out] 
@return 成功返回文件当前距开始处的偏移      
*/
long CSTKP_FTell(CSUDI_HANDLE hFile);


/*****为各种交互输入而增加的注册接口此接口只供testkit模块使用****/
typedef struct _CSTKPInputOperations_S
{
	char* (*Gets) (char *s,int size);
	int (*VPrintf) (char *format,va_list arg);
	void (*Flush) (void);
	void (*Exit) (void);
}CSTKPInputOperations_S;

CSTKPInputOperations_S* CSTKPInputGetOps(void);
CSUDI_BOOL CSTKPGetInputRegisterState(void);
CSUDI_BOOL CSTKPSetInputRegisterState(CSUDI_BOOL bState);

void CSTKPRegisterUartSysInput(void);
void CSTKPRegisterUartUDI1Input(void);
void CSTKPRegisterUartUDI2Input(void);
void CSTKPRegisterRemoteInput(void);
void CSTKPRegisterInput(CSTKPInputType_E nType);
/*************************declare end******************************************************/

/*********************各种测试依赖文件系统注册接口***********************/
typedef struct _CSTKPFSOperations_S
{
 	CSUDI_HANDLE (* Open) (const char* filename,const char* mode);
 	int (* Close) (CSUDI_HANDLE hFile);
	int (* Read) (void * ptr,int size,int nmemb,CSUDI_HANDLE hFile);
	int (* Write) (const void * ptr,int size,int nmemb,CSUDI_HANDLE hFile);
	int (* Flush) (CSUDI_HANDLE hFile);
	int (* Seek) (CSUDI_HANDLE hFile,long int offset,int whence);
	int (* Tell) (CSUDI_HANDLE hFile);
}CSTKPFSOperations_S;

CSTKPFSOperations_S* CSTKPFsGetOps(void);

CSUDI_BOOL CSTKPSetFsRegisterState(CSUDI_BOOL bState);
CSUDI_BOOL CSTKPGetFsRegisterState(void);

//以下注册接口供外部使用
void CSTKPRegisterUsbFs(void);
void CSTKPRegisterRomFs(void);
void CSTKPRegisterSysFs(void);

CSUDI_BOOL CSTKPSetFSType(CSTKPFSType_E eType);
CSTKPFSType_E CSTKPGetFSType(void);

/**************************declare end******************************************************/

#ifdef __cplusplus
}
#endif

/** @} */

#endif


