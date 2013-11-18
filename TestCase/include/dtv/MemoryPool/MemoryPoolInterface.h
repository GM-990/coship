#ifndef MemoryPoolInterface_H
#define MemoryPoolInterface_H

#ifdef PRAGMA
#pragma interface "MemoryPoolInterface.h"
#endif

#include "mmcp_typedef.h"

/* {{{USR */
#ifdef  __cplusplus
extern "C" {
#endif
/* }}}USR */

/* {{{USR
CreateMemoryPool
	创建内存池

	HANDLE CreateMemoryPool(
		PBYTE pbMemoryAddress,
		int nMemoryBufferLenght
	};	

Parameters
	
	pbMemoryAddress
		[in]内存块的开始地址

	nMemoryBufferLenght
		[in]内存块的长度

Return Values
		1）如果成功返回内存池句柄
		2）如果失败返回为NULL

		当pbMemoryAddress为空或nMemoryBufferLenght长度小于1024（1K）或nMemoryBufferLenght大于等于64M时，创建将不会成功

   }}}USR */
HCSHANDLE CSUSPCreateMemoryPool( PBYTE pbMemoryAddress, int nMemoryBufferLength );

/* {{{USR
CreateMemoryPoolEx
	创建可靠性检查的内存池,当进行可靠性检查时,将会使运行速度降低,建议在Debug版本时使用该功能,当检查到内存破坏时，将输出断言

	HANDLE CreateMemoryPoolEx(
		PBYTE pbMemoryAddress,
		int nMemoryBufferLenght,
		BOOL bSecurityCheck
	};	

Parameters
	
	pbMemoryAddress
		[in]内存块的开始地址

	nMemoryBufferLenght
		[in]内存块的长度

	bSecurityCheck
		[in]是否开启安全检查功能,
			TRUE:	开启
			FALSE:	静止该功能

Return Values
		1）如果成功返回内存池句柄
		2）如果失败返回为NULL

		当pbMemoryAddress为空或nMemoryBufferLenght长度小于1024（1K）或nMemoryBufferLenght大于等于64M时，创建将不会成功

   }}}USR */
HCSHANDLE CSUSPCreateMemoryPoolEx( PBYTE pbMemoryAddress, int nMemoryBufferLength, BOOL bSecurityCheck );

/* {{{USR
CreateMemoryPoolEx2
创建可靠性检查和更快读写速度的内存池,当进行可靠性检查时,将会使运行速度降低,建议在Debug版本时使用该功能,当检查到内存破坏时，将输出断言；
如果需要更快的速度，请将bFast设置为TRUE，此时在程序读写MemoryPool时没有加锁，速度更快。建议在能确保不会有多个线程读写同一个MemoryPool时使用此功能。

HANDLE CreateMemoryPoolEx2(
PBYTE pbMemoryAddress,
int nMemoryBufferLenght,
BOOL bSecurityCheck,
BOOL bFast
};	

Parameters

pbMemoryAddress
[in]内存块的开始地址

nMemoryBufferLenght
[in]内存块的长度

bSecurityCheck
[in]是否开启安全检查功能,
TRUE:	开启
FALSE:	静止该功能

bFast
[in]是否需要更快的速度,
TRUE:	是
FALSE:	否

Return Values
1）如果成功返回内存池句柄
2）如果失败返回为NULL

当pbMemoryAddress为空或nMemoryBufferLenght长度小于1024（1K）或nMemoryBufferLenght大于等于64M时，创建将不会成功

}}}USR */

HCSHANDLE CSUSPCreateMemoryPoolEx2( PBYTE pbMemoryAddress, int nMemoryBufferLength, BOOL bSecurityCheck, BOOL bFast );

/* {{{USR
CreateGroupMemory
	在内存池中创建分组内存句柄

	HANDLE CreateGroupMemory(
		HANDLE hMemoryPool,
		int nMemorySize,
		int	nLimitSize
	};	

Parameters
	
	hMemoryPool
		[in]内存池句柄

	nMemorySize
		[in]分组内存的长度

	nLimitSIze
		[in]总分组内存容量限制

Return Values
		1）如果成功返回分组内存句柄
		2）如果失败返回为NULL
		3) 在两次 CreateGroupMemory Call  中，如果第一和第二参数相同、第三个参数不同，创建成功， 但第二次 call 返回与第一次相同的句柄。
		例如：
		DWORD dwGroup2=CSUSPCreateGroupMemory(dwRe,0x300,0x400);
		DWORD dwGroup3=CSUSPCreateGroupMemory(dwRe,0x300,0x00);
		dwGroup2 和 dwGroup3 指向相同的 group
       

		当hMemoryPool无效或nMemorySize超过内存池长度时，创建会失败
		当nLimitSIze<=0,2其使用长度不受限制，当nLimitSIze>nMemorySize时，以nMemorySize为准。
		当内存池中没有永久内存时，创建会失败
   }}}USR */
HCSHANDLE CSUSPCreateGroupMemory( HCSHANDLE hMemoryPool, int nMemorySize, int nLimitSize );

/* {{{USR
CSUSPMallocMemoryFromPool
	在内存池中分配永久使用内存,该内存不能调用CSUSPMallocMemory

	PVOID CSUSPMallocMemoryFromPool(
		HCSHANDLE hMemoryPool,
		int nMemorySize
	};	

Parameters
	
	hMemoryPool
		[in]内存池句柄

	nMemorySize
		[in]分配内存的长度

Return Values
		1）如果成功返回内存指针
		2）如果失败返回为NULL

		当hMemoryPool无效时，分配失败
		当hMemoryPool有效，并且永久内存中没有足够内存时，分配失败

   }}}USR */
PVOID CSUSPMallocMemoryFromPool( HCSHANDLE hMemoryPool, int nMemorySize );

/* {{{USR
CSUSPMallocMemory
	在内存池中分配内存

	PVOID CSUSPMallocMemory(
		HANDLE hGroupMemory
	};	

Parameters
	
	hGroupMemory
		[in]分组内存句柄

Return Values
		1）如果成功返回内存指针
		2）如果失败返回为NULL

		当hGroupMemory无效时，分配失败
		当hGroupMemory有效，如果组中的空闲内存为空，并且永久内存为空时，分配失败

   }}}USR */
PVOID CSUSPMallocMemory( HCSHANDLE hGroupMemory );

/* {{{USR
FreeMemory
	释放在内存池中分配的内存指针

	PVOID FreeMemory(
		HANDLE hGroupMemory,
		PVOID pvMemoryAddr
	};	

Parameters
	
	hGroupMemory
		[in]该参数未使用

	pvMemoryAddr
		[in]内存指针

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

		当hGroupMemory无效或pvMemoryAddr无效或pvMemoryAddr多次释放时，分配失败

   }}}USR */
BOOL CSUSPFreeMemory( HCSHANDLE hGroupMemory, PVOID pvMemoryAddr );

/* {{{USR
FreeMemory
	释放在内存池中分配的内存指针

	PVOID FreeMemoryEx(
		PVOID pvMemoryAddr
	};	

Parameters
	
	pvMemoryAddr
		[in]内存指针

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

		当hGroupMemory无效或pvMemoryAddr无效或pvMemoryAddr多次释放时，分配失败

Remarks
		和CSUSPFreeMemory只是调用接口不同。
		只有在Debug中才对hGroupMemory有效性进行检查
   }}}USR */
BOOL CSUSPFreeMemoryEx( PVOID pvMemoryAddr );

/* {{{USR
IsMemoryAddress
	检查是否为合法的内存池地址

	BOOL IsMemoryAddress(
		HANDLE hMemoryPool,
		PVOID pvMemoryAddress,
		int nMemoryLen
	};	

Parameters
	
	hMemoryPool
		[in]该参数未使用

	pvMemoryAddress
		[in]内存指针

	nMemoryLen
		[in]内存指针的有效地址长度

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

		当hMemoryPool无效时，返回失败
		当pvMemoryAddress无效时，返回失败
		当hMemoryPool有效、pvMemoryAddress有效、nMemoryLen等于内存的实际长度时，返回成功
		当hMemoryPool有效、pvMemoryAddress有效、nMemoryLen不等于内存的实际长度时，返回成功
   }}}USR */
BOOL CSUSPIsMemoryAddress( HCSHANDLE hMemoryPool, PVOID pvMemoryAddress, int nMemoryLen );

/* {{{USR
IsMemoryAddress
	检查是否为合法的内存池地址

	BOOL CSUSPIsMemoryAddressEx(
		PVOID pvMemoryAddress,
		int nMemoryLen
	};	

Parameters
	
	pvMemoryAddress
		[in]内存指针

	nMemoryLen
		[in]内存指针的有效地址长度

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

		当pvMemoryAddress无效时，返回失败
		当hMemoryPool有效、pvMemoryAddress有效、nMemoryLen等于内存的实际长度时，返回成功
		当hMemoryPool有效、pvMemoryAddress有效、nMemoryLen不等于内存的实际长度时，返回成功
   }}}USR */
BOOL CSUSPIsMemoryAddressEx( PVOID pvMemoryAddress, int nMemoryLen );

/* {{{USR
CSUSPGetMemoryPoolAllAviSize
	获取内存池中正在被使用的内存的总大小

	BOOL CSUSPGetMemoryPoolAllAviSize(
		HCSHANDLE hMemoryPool,
	};	

Parameters
	
	hMemoryPool
		[in]内存池句柄

Return Values
		1）如果成功返回可以使用的空闲组内存大小
		2）如果失败返回-1

		当hGroupMemory无效时，返回-1
   }}}USR */
int CSUSPGetMemoryPoolAllAviSize( HCSHANDLE hMemoryPool );

/* {{{USR
CSUSPGetMemoryPoolFreeSize
	获取可以使用的空闲内存

	BOOL CSUSPGetMemoryPoolFreeSize(
		HCSHANDLE hMemoryPool,
	};	

Parameters
	
	hMemoryPool
		[in]内存池句柄

Return Values
		1）如果成功返回可以使用的空闲组内存大小
		2）如果失败返回-1

		当hGroupMemory无效时，返回-1
   }}}USR */
int CSUSPGetMemoryPoolFreeSize( HCSHANDLE hMemoryPool );

/* {{{USR
CSUSPGetMemoryGroupFreeCount
	获取可以使用的空闲内存块的个数

	BOOL CSUSPGetMemoryGroupFreeCount(
		HCSHANDLE hGroupMemory
	};	

Parameters
	
	hGroupMemory
		[in]分组内存句柄

Return Values
		1）如果成功返回可以使用的空闲组内存块的个数
		2）如果失败返回-1

		当hGroupMemory无效时，返回-1
   }}}USR */
int CSUSPGetMemoryGroupFreeCount( HCSHANDLE hGroupMemory );

/* {{{USR
CSUSPGetMemoryGroupAviCount
	获取一个内存组中正在被使用的内存块的个数

	BOOL CSUSPGetMemoryGroupAviCount(
		HCSHANDLE hGroupMemory
	};	

Parameters
	
	hGroupMemory
		[in]分组内存句柄

Return Values
		1）如果成功返回正在被使用的内存块的个数
		2）如果失败返回-1

		当hGroupMemory无效时，返回-1
   }}}USR */
int CSUSPGetMemoryGroupAviCount( HCSHANDLE hGroupMemory );

/* {{{USR
CSUSPDestroyMemoryPool
	释放MemoryPool占用资源

	BOOL CSUSPDestroyMemoryPool(
		HCSHANDLE hMemoryPool
	};	

Parameters
	
	hMemoryPool
		[in]内存池句柄

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

		当hMemoryPool无效时，返回FALSE
   }}}USR */
BOOL CSUSPDestroyMemoryPool( HCSHANDLE hMemoryPool );

/* {{{USR */
int CSUSPMemoryPoolGetVersionInfo( char * pcVersionInfo, int nSize );

#ifdef  __cplusplus
}
#endif
/* }}}USR */

#endif /* MemoryPoolInterface_H */
