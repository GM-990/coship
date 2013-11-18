/**@defgroup ITESTKIT ���弯�ɲ����������(C���Բ���)��ֲ��ӿ�

���ɲ����������(C���Բ���)�������ڱ�ģ�鶨��Ľӿ�ʵ�֡�
�����ֵ�ʵ�ִ��������¾�����ʹ��stdio.h�е�ͬ������ʵ�֣�
ֻ�ڼ���stdio.hδʵ�ֻ���ʵ�ֲ��õ�ƽ̨�ϣ���Ҫ��������ʽʵ��.


@version 2.0.0 2009/09/17 ����
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
@brief ���Կ����������ʾ����ӿ�

����Ϊ����ʽ��ӡ���
@note ����Ĵ�ӡ��Portingʵ���еĴ�ӡ��һ��Ҫ��һ���ط���
����STƽ̨��,��־��ӡһ���ڴ��ڣ������Կ�ܴ�ӡ�������ն���(��printf)
*/

/*******************I/O��ؽӿ�********************************/
int CSTKP_VPrintf(char *format,va_list arg);
/**
@brief �������豸�����̡�ң�����ȣ���ȡ���봮
@param[in] char *s   ���ڴ�������ַ�����������ַ
@param[in] int nSize   �����ַ������������� 
@param[out] 
@return  �ɹ���ȡ���뷵�� �����ַ�����������ַ
@return û�������ʧ�ܷ���CSUDI_NULL
*/
char* CSTKP_Gets(char *s,int size);
void CSTKP_Flush(void);
void CSTKP_Exit(void);
/********************�ļ�ϵͳ��ؽӿ�********************/
/**
@brief ���ļ�
@param[in] const char* filename    ���򿪵��ļ����ļ���
@param[in] const char* mode     �򿪷�ʽ,���C99��׼
@param[out] 
@return  �ļ����,ʧ�ܷ���CSUDI_NULL  
*/
CSUDI_HANDLE CSTKP_FOpen(const char* filename,const char* mode);
/**
@brief �ر��ļ�
@param[in] CSUDI_HANDLE hFile �ļ����,CSTKP_FOpen�ķ���ֵ
@param[out] 
@return �ɹ�����0
*/
int CSTKP_FClose(CSUDI_HANDLE hFile);
/**
@brief ���ļ�
@param[in] int nSize     ����Ԫ�ش�С���ֽڣ�
@param[in] int nMemb     Ҫ����Ԫ�ظ���
@param[in] CSUDI_HANDLE hFile	 �ļ����,CSTKP_FOpen�ķ���ֵ
@param[out] void * ptr  ָ���Ŷ������ݵ��ڴ�
@return �ɹ���ȡԪ�ظ���,����������nMemb��<0��ʾ��������ļ�����β�� 
*/
int CSTKP_FRead(void * ptr,int size,int nmemb,CSUDI_HANDLE hFile);
/**
@brief д�ļ�
@param[in] void * ptr ָ����д�����ݵ��ڴ�
@param[in] int nSize     ����Ԫ�ش�С���ֽڣ�
@param[in] int nMemb     Ҫд��Ԫ�ظ���
@param[in] CSUDI_HANDLE hFile	 �ļ����,CSTKP_FOpen�ķ���ֵ
@param[out] 
@return �ɹ�д���Ԫ�ظ��� 0:�ļ�״̬�������κα仯
*/
int CSTKP_FWrite(const void * ptr,int size,int nmemb,CSUDI_HANDLE hFile);
/**
@brief  flush�ļ�
@param[in] CSUDI_HANDLE hFile �ļ����,CSTKP_FOpen�ķ���ֵ
@param[out] 
@return �ɹ�����0
*/
int CSTKP_FFlush(CSUDI_HANDLE hFile);
/**
@brief seek�ļ�
@param[in] CSUDI_HANDLE hFile �ļ����,CSTKP_FOpen�ķ���ֵ
@param[in] long int offset    �ƶ�ƫ��
@param[in] int whence    �ƶ���ʼλ��,SEEK_SET,SEEK_CUR,SEEK_END
@param[out] 
@return �ɹ�����0,���������������ֵ  
*/
int CSTKP_FSeek(CSUDI_HANDLE hFIle,long int offset,int whence);
/**
@brief  ��ȡ�ļ���ǰλ��
@param[in] CSUDI_HANDLE hFile �ļ����,CSTKP_FOpen�ķ���ֵ
@param[out] 
@return �ɹ������ļ���ǰ�࿪ʼ����ƫ��      
*/
long CSTKP_FTell(CSUDI_HANDLE hFile);


/*****Ϊ���ֽ�����������ӵ�ע��ӿڴ˽ӿ�ֻ��testkitģ��ʹ��****/
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

/*********************���ֲ��������ļ�ϵͳע��ӿ�***********************/
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

//����ע��ӿڹ��ⲿʹ��
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


