/**@defgroup TYPEDEF_PLUS TYPEDEF Plus UDI TYPEDEF 模块的补充模块，提供兼容的数据定义

@brief 本模块仅为保持旧发布版本的兼容而定义，原则上不推荐在新模块中使用本模块中的定义。
本模块中的定义可以在MMCP内部模块及内部头文件中使用，也可以在基于GUI+USP模式的产品下使用，
但不得在Eastwin系列产品的对外头文件中使用

@warning 本模块仅供MMCP内部使用，不允许将本模块的任何定义、接口提供给MMCP以外的模块实用，包括但不限于UDI、集成接口中
@warning 不允许在需要对MMCP外部发布的头文件中包含本头文件
 
@version 2009/10/09 根据UDI2.1版本的定义，重新调整本模块
- 初始版本
@{
*/

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifndef _UDIPLUS_TYPEDEF_H_
#define _UDIPLUS_TYPEDEF_H_

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef WIN32
typedef char			 CHAR;
typedef double			 DOUBLE;

typedef CHAR			 *PCHAR;
typedef CHAR			 *LPCHAR;

typedef unsigned char    UCHAR;
typedef UCHAR			 *PUCHAR;
typedef UCHAR			 *LPUCHAR;

typedef short			 SHORT;
typedef SHORT			 *PSHORT;
typedef SHORT			 *LPSHORT;

typedef unsigned short   USHORT;
typedef USHORT			 *PUSHORT;
typedef USHORT			 *LPUSHORT;

typedef long			 LONG;
typedef LONG			 *PLONG;
typedef LONG			 *LPLONG;

typedef unsigned long	 ULONG;
typedef ULONG			 *PULONG;
typedef ULONG			 *LPULONG;

typedef int              INT;
typedef int              *PINT;
typedef int              *LPINT;

typedef unsigned int     UINT;
typedef unsigned int     *PUINT;
typedef unsigned int     *LPUINT;

typedef float            FLOAT;
typedef FLOAT            *PFLOAT;
typedef FLOAT            *LPFLOAT;

typedef void			 VOID;
typedef void			 *PVOID;
typedef void             *LPVOID;
typedef const void       *LPCVOID;

typedef CSUDI_INT8		 INT8;
typedef CSUDI_INT16		 INT16;
typedef CSUDI_INT32		 INT32;
typedef CSUDI_UINT8	     UINT8;
typedef CSUDI_UINT16 	 UINT16;
typedef CSUDI_UINT32	 UINT32;

#ifndef DEFINED_BOOL
typedef CSUDI_BOOL       BOOL;
#define  DEFINED_BOOL
#endif

typedef BOOL             *PBOOL;
typedef BOOL             *LPBOOL;

typedef CSUDI_UINT8		 BYTE;
typedef BYTE             *PBYTE;
typedef BYTE             *LPBYTE;

typedef CSUDI_UINT16	 WORD;
typedef WORD             *PWORD;
typedef WORD             *LPWORD;

typedef unsigned int     DWORD;
typedef DWORD            *PDWORD;
typedef DWORD            *LPDWORD;
#else
	#include <windows.h>
#endif

typedef DWORD       	 HCSHANDLE; 

/*Standard constants*/
#undef FALSE
#undef TRUE
#undef NULL

#define FALSE   		 (CSUDI_FALSE)
#define TRUE    		 (CSUDI_TRUE)
#define NULL    		 (0)


#ifndef SUCCESS
#define SUCCESS			 (CSUDI_SUCCESS)
#endif

#ifndef FAILURE
#define FAILURE			 (CSUDI_FAILURE) 
#endif

#ifdef	__cplusplus
}
#endif
/** @} */

#endif

