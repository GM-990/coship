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
	�����ڴ��

	HANDLE CreateMemoryPool(
		PBYTE pbMemoryAddress,
		int nMemoryBufferLenght
	};	

Parameters
	
	pbMemoryAddress
		[in]�ڴ��Ŀ�ʼ��ַ

	nMemoryBufferLenght
		[in]�ڴ��ĳ���

Return Values
		1������ɹ������ڴ�ؾ��
		2�����ʧ�ܷ���ΪNULL

		��pbMemoryAddressΪ�ջ�nMemoryBufferLenght����С��1024��1K����nMemoryBufferLenght���ڵ���64Mʱ������������ɹ�

   }}}USR */
HCSHANDLE CSUSPCreateMemoryPool( PBYTE pbMemoryAddress, int nMemoryBufferLength );

/* {{{USR
CreateMemoryPoolEx
	�����ɿ��Լ����ڴ��,�����пɿ��Լ��ʱ,����ʹ�����ٶȽ���,������Debug�汾ʱʹ�øù���,����鵽�ڴ��ƻ�ʱ�����������

	HANDLE CreateMemoryPoolEx(
		PBYTE pbMemoryAddress,
		int nMemoryBufferLenght,
		BOOL bSecurityCheck
	};	

Parameters
	
	pbMemoryAddress
		[in]�ڴ��Ŀ�ʼ��ַ

	nMemoryBufferLenght
		[in]�ڴ��ĳ���

	bSecurityCheck
		[in]�Ƿ�����ȫ��鹦��,
			TRUE:	����
			FALSE:	��ֹ�ù���

Return Values
		1������ɹ������ڴ�ؾ��
		2�����ʧ�ܷ���ΪNULL

		��pbMemoryAddressΪ�ջ�nMemoryBufferLenght����С��1024��1K����nMemoryBufferLenght���ڵ���64Mʱ������������ɹ�

   }}}USR */
HCSHANDLE CSUSPCreateMemoryPoolEx( PBYTE pbMemoryAddress, int nMemoryBufferLength, BOOL bSecurityCheck );

/* {{{USR
CreateMemoryPoolEx2
�����ɿ��Լ��͸����д�ٶȵ��ڴ��,�����пɿ��Լ��ʱ,����ʹ�����ٶȽ���,������Debug�汾ʱʹ�øù���,����鵽�ڴ��ƻ�ʱ����������ԣ�
�����Ҫ������ٶȣ��뽫bFast����ΪTRUE����ʱ�ڳ����дMemoryPoolʱû�м������ٶȸ��졣��������ȷ�������ж���̶߳�дͬһ��MemoryPoolʱʹ�ô˹��ܡ�

HANDLE CreateMemoryPoolEx2(
PBYTE pbMemoryAddress,
int nMemoryBufferLenght,
BOOL bSecurityCheck,
BOOL bFast
};	

Parameters

pbMemoryAddress
[in]�ڴ��Ŀ�ʼ��ַ

nMemoryBufferLenght
[in]�ڴ��ĳ���

bSecurityCheck
[in]�Ƿ�����ȫ��鹦��,
TRUE:	����
FALSE:	��ֹ�ù���

bFast
[in]�Ƿ���Ҫ������ٶ�,
TRUE:	��
FALSE:	��

Return Values
1������ɹ������ڴ�ؾ��
2�����ʧ�ܷ���ΪNULL

��pbMemoryAddressΪ�ջ�nMemoryBufferLenght����С��1024��1K����nMemoryBufferLenght���ڵ���64Mʱ������������ɹ�

}}}USR */

HCSHANDLE CSUSPCreateMemoryPoolEx2( PBYTE pbMemoryAddress, int nMemoryBufferLength, BOOL bSecurityCheck, BOOL bFast );

/* {{{USR
CreateGroupMemory
	���ڴ���д��������ڴ���

	HANDLE CreateGroupMemory(
		HANDLE hMemoryPool,
		int nMemorySize,
		int	nLimitSize
	};	

Parameters
	
	hMemoryPool
		[in]�ڴ�ؾ��

	nMemorySize
		[in]�����ڴ�ĳ���

	nLimitSIze
		[in]�ܷ����ڴ���������

Return Values
		1������ɹ����ط����ڴ���
		2�����ʧ�ܷ���ΪNULL
		3) ������ CreateGroupMemory Call  �У������һ�͵ڶ�������ͬ��������������ͬ�������ɹ��� ���ڶ��� call �������һ����ͬ�ľ����
		���磺
		DWORD dwGroup2=CSUSPCreateGroupMemory(dwRe,0x300,0x400);
		DWORD dwGroup3=CSUSPCreateGroupMemory(dwRe,0x300,0x00);
		dwGroup2 �� dwGroup3 ָ����ͬ�� group
       

		��hMemoryPool��Ч��nMemorySize�����ڴ�س���ʱ��������ʧ��
		��nLimitSIze<=0,2��ʹ�ó��Ȳ������ƣ���nLimitSIze>nMemorySizeʱ����nMemorySizeΪ׼��
		���ڴ����û�������ڴ�ʱ��������ʧ��
   }}}USR */
HCSHANDLE CSUSPCreateGroupMemory( HCSHANDLE hMemoryPool, int nMemorySize, int nLimitSize );

/* {{{USR
CSUSPMallocMemoryFromPool
	���ڴ���з�������ʹ���ڴ�,���ڴ治�ܵ���CSUSPMallocMemory

	PVOID CSUSPMallocMemoryFromPool(
		HCSHANDLE hMemoryPool,
		int nMemorySize
	};	

Parameters
	
	hMemoryPool
		[in]�ڴ�ؾ��

	nMemorySize
		[in]�����ڴ�ĳ���

Return Values
		1������ɹ������ڴ�ָ��
		2�����ʧ�ܷ���ΪNULL

		��hMemoryPool��Чʱ������ʧ��
		��hMemoryPool��Ч�����������ڴ���û���㹻�ڴ�ʱ������ʧ��

   }}}USR */
PVOID CSUSPMallocMemoryFromPool( HCSHANDLE hMemoryPool, int nMemorySize );

/* {{{USR
CSUSPMallocMemory
	���ڴ���з����ڴ�

	PVOID CSUSPMallocMemory(
		HANDLE hGroupMemory
	};	

Parameters
	
	hGroupMemory
		[in]�����ڴ���

Return Values
		1������ɹ������ڴ�ָ��
		2�����ʧ�ܷ���ΪNULL

		��hGroupMemory��Чʱ������ʧ��
		��hGroupMemory��Ч��������еĿ����ڴ�Ϊ�գ����������ڴ�Ϊ��ʱ������ʧ��

   }}}USR */
PVOID CSUSPMallocMemory( HCSHANDLE hGroupMemory );

/* {{{USR
FreeMemory
	�ͷ����ڴ���з�����ڴ�ָ��

	PVOID FreeMemory(
		HANDLE hGroupMemory,
		PVOID pvMemoryAddr
	};	

Parameters
	
	hGroupMemory
		[in]�ò���δʹ��

	pvMemoryAddr
		[in]�ڴ�ָ��

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

		��hGroupMemory��Ч��pvMemoryAddr��Ч��pvMemoryAddr����ͷ�ʱ������ʧ��

   }}}USR */
BOOL CSUSPFreeMemory( HCSHANDLE hGroupMemory, PVOID pvMemoryAddr );

/* {{{USR
FreeMemory
	�ͷ����ڴ���з�����ڴ�ָ��

	PVOID FreeMemoryEx(
		PVOID pvMemoryAddr
	};	

Parameters
	
	pvMemoryAddr
		[in]�ڴ�ָ��

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

		��hGroupMemory��Ч��pvMemoryAddr��Ч��pvMemoryAddr����ͷ�ʱ������ʧ��

Remarks
		��CSUSPFreeMemoryֻ�ǵ��ýӿڲ�ͬ��
		ֻ����Debug�вŶ�hGroupMemory��Ч�Խ��м��
   }}}USR */
BOOL CSUSPFreeMemoryEx( PVOID pvMemoryAddr );

/* {{{USR
IsMemoryAddress
	����Ƿ�Ϊ�Ϸ����ڴ�ص�ַ

	BOOL IsMemoryAddress(
		HANDLE hMemoryPool,
		PVOID pvMemoryAddress,
		int nMemoryLen
	};	

Parameters
	
	hMemoryPool
		[in]�ò���δʹ��

	pvMemoryAddress
		[in]�ڴ�ָ��

	nMemoryLen
		[in]�ڴ�ָ�����Ч��ַ����

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

		��hMemoryPool��Чʱ������ʧ��
		��pvMemoryAddress��Чʱ������ʧ��
		��hMemoryPool��Ч��pvMemoryAddress��Ч��nMemoryLen�����ڴ��ʵ�ʳ���ʱ�����سɹ�
		��hMemoryPool��Ч��pvMemoryAddress��Ч��nMemoryLen�������ڴ��ʵ�ʳ���ʱ�����سɹ�
   }}}USR */
BOOL CSUSPIsMemoryAddress( HCSHANDLE hMemoryPool, PVOID pvMemoryAddress, int nMemoryLen );

/* {{{USR
IsMemoryAddress
	����Ƿ�Ϊ�Ϸ����ڴ�ص�ַ

	BOOL CSUSPIsMemoryAddressEx(
		PVOID pvMemoryAddress,
		int nMemoryLen
	};	

Parameters
	
	pvMemoryAddress
		[in]�ڴ�ָ��

	nMemoryLen
		[in]�ڴ�ָ�����Ч��ַ����

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

		��pvMemoryAddress��Чʱ������ʧ��
		��hMemoryPool��Ч��pvMemoryAddress��Ч��nMemoryLen�����ڴ��ʵ�ʳ���ʱ�����سɹ�
		��hMemoryPool��Ч��pvMemoryAddress��Ч��nMemoryLen�������ڴ��ʵ�ʳ���ʱ�����سɹ�
   }}}USR */
BOOL CSUSPIsMemoryAddressEx( PVOID pvMemoryAddress, int nMemoryLen );

/* {{{USR
CSUSPGetMemoryPoolAllAviSize
	��ȡ�ڴ�������ڱ�ʹ�õ��ڴ���ܴ�С

	BOOL CSUSPGetMemoryPoolAllAviSize(
		HCSHANDLE hMemoryPool,
	};	

Parameters
	
	hMemoryPool
		[in]�ڴ�ؾ��

Return Values
		1������ɹ����ؿ���ʹ�õĿ������ڴ��С
		2�����ʧ�ܷ���-1

		��hGroupMemory��Чʱ������-1
   }}}USR */
int CSUSPGetMemoryPoolAllAviSize( HCSHANDLE hMemoryPool );

/* {{{USR
CSUSPGetMemoryPoolFreeSize
	��ȡ����ʹ�õĿ����ڴ�

	BOOL CSUSPGetMemoryPoolFreeSize(
		HCSHANDLE hMemoryPool,
	};	

Parameters
	
	hMemoryPool
		[in]�ڴ�ؾ��

Return Values
		1������ɹ����ؿ���ʹ�õĿ������ڴ��С
		2�����ʧ�ܷ���-1

		��hGroupMemory��Чʱ������-1
   }}}USR */
int CSUSPGetMemoryPoolFreeSize( HCSHANDLE hMemoryPool );

/* {{{USR
CSUSPGetMemoryGroupFreeCount
	��ȡ����ʹ�õĿ����ڴ��ĸ���

	BOOL CSUSPGetMemoryGroupFreeCount(
		HCSHANDLE hGroupMemory
	};	

Parameters
	
	hGroupMemory
		[in]�����ڴ���

Return Values
		1������ɹ����ؿ���ʹ�õĿ������ڴ��ĸ���
		2�����ʧ�ܷ���-1

		��hGroupMemory��Чʱ������-1
   }}}USR */
int CSUSPGetMemoryGroupFreeCount( HCSHANDLE hGroupMemory );

/* {{{USR
CSUSPGetMemoryGroupAviCount
	��ȡһ���ڴ��������ڱ�ʹ�õ��ڴ��ĸ���

	BOOL CSUSPGetMemoryGroupAviCount(
		HCSHANDLE hGroupMemory
	};	

Parameters
	
	hGroupMemory
		[in]�����ڴ���

Return Values
		1������ɹ��������ڱ�ʹ�õ��ڴ��ĸ���
		2�����ʧ�ܷ���-1

		��hGroupMemory��Чʱ������-1
   }}}USR */
int CSUSPGetMemoryGroupAviCount( HCSHANDLE hGroupMemory );

/* {{{USR
CSUSPDestroyMemoryPool
	�ͷ�MemoryPoolռ����Դ

	BOOL CSUSPDestroyMemoryPool(
		HCSHANDLE hMemoryPool
	};	

Parameters
	
	hMemoryPool
		[in]�ڴ�ؾ��

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

		��hMemoryPool��Чʱ������FALSE
   }}}USR */
BOOL CSUSPDestroyMemoryPool( HCSHANDLE hMemoryPool );

/* {{{USR */
int CSUSPMemoryPoolGetVersionInfo( char * pcVersionInfo, int nSize );

#ifdef  __cplusplus
}
#endif
/* }}}USR */

#endif /* MemoryPoolInterface_H */
