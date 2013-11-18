#ifndef  _CS_MEMORY_H_
#define  _CS_MEMORY_H_

#include "mmcp_typedef.h"
#include "mmcp_os.h"
#include "mmcp_debug.h"
#include "cs_pthread.h"
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif


struct _MMCP_Mutex {
	pthread_mutex_t id;
	short recursive;
	short inited;
	pthread_t owner;
};

typedef struct _MMCP_Mutex MMCP_Mutex;

/**** Functions of Memory Vector  ****/



/*使用于非多线程的情况，相当于MMCP_MemCreateVectorEx的sync参数为FALSE*/
void* MMCP_MemCreateVector(int  elementSize,
					  int  elementNumber,
					  int  incrementNumber);
/*sync参数表示是否后续的操作是否是同步的,TRUE表示同步。
FALSE表示非同步，相当于 MMCP_MemCreateVector*/
void * MMCP_MemCreateVectorEx(int  elementSize, 
					int  elementNumber, 
					int incrementNumber,
					BOOL sync);
int MMCP_MemDestroyVector(void *vector);
void* MMCP_MemAllocateElement(void *vector);
int MMCP_MemFreeElement(void *vector,	void *element);

void* MMCP_MemGetElementAt(void *vector,int index);
int MMCP_MemGetElementIndex(void *vector, void *element);

int MMCP_MemInit(unsigned int nMMCPTotalMem);

int __MMCP_Mutex_Init(MMCP_Mutex *mutex);
int __MMCP_Mutex_Destroy(MMCP_Mutex *mutex);
int __MMCP_Mutex_Lock(MMCP_Mutex *mutex);
int __MMCP_Mutex_Unlock(MMCP_Mutex *mutex);
/******************** memory type **********************/
#define MEM_TYPE_GENERAL	    0
/* DSMCC */
#define MEM_TYPE_DSMCC		    1
/* MPI DLOAD */
#define MEM_TYPE_MMCP_DLOAD	    2
/* MPI OSD */
#define MEM_TYPE_MMCP_OSD	    3
/* coresys dtvfs */
#define MEM_TYPE_FS		        4
/* MPI TFCA */
#define MEM_TYPE_TFCA		    5
/* coresys EPG */
#define MEM_TYPE_EPG		    6	
/* coresys Event Manage */
#define MEM_TYPE_EVT_MANAGER	7	
/* coresys dtvfs */
#define MEM_TYPE_DTVFS		    8
/* coresys/navitv  */	
#define MEM_TYPE_NATIVE		    9	
/* coresys/dtv/sidatabase  */
#define MEM_TYPE_SIDB		    10	
/* graphics microwin */
#define MEM_TYPE_MICROWIN	    11	
/* graphics jpg */
#define MEM_TYPE_JPG		    12	
/* graphics png */
#define MEM_TYPE_PNG		    13
/* javavm/src/share/basis/native */	
#define MEM_TYPE_JAVAVM_BASIS   14 
/* javavm/src/share/native, javavm/src/stos20/native */
#define MEM_TYPE_JAVAVM_COMMON  15 
 /* javavm/src/portlib */
#define MEM_TYPE_JAVAVM_PORTLIB 16
/* javavm/src/share/javavm */
#define MEM_TYPE_JAVAVM_JVM     17 
/*kernel/fs/fat/fat_inode.c*/
#define MEM_TYPE_DTVFS_FAT	    18
/*kernel/fs/e2fs/e2fs_link.c*/
#define MEM_TYPE_DTVFS_E2FS	    19
/*dtv/si_cache/cssi_cache.c*/
#define MEM_TYPE_SI_CACHE	    20
/*midp*/
#define MEM_TYPE_MIDP	        21
/*freetype*/
#define MEM_TYPE_FREETYPE	    22
/*codec*/
#define MEM_TYPE_CODEC  	    23
/*protocol*/
#define MEM_TYPE_PROTOCOL    	24
/*mediaplayer*/
#define MEM_TYPE_MEDIAPLAYER  	25
/*GTOOL*/
#define MEM_TYPE_GTOOLS  	    26
/*dtvmx*/
#define MEM_TYPE_DTVMX  	    27
/*dtvmx*/
#define MEM_TYPE_DTV  	        28
/*JSEXT*/
#define MEM_TYPE_JSEXT  	    29
/*Shell*/
#define MEM_TYPE_SHELL  	    30
/*nes*/
#define MEM_TYPE_NES 	        31
/*lockcheck*/
#define MEM_TYPE_LOCKCHECK      32
/*appmanager*/
#define MEM_TYPE_APPMANAGER		33
/*PVR*/
#define MEM_TYPE_PVR            34
/*filter analyze*/
#define MEM_TYPE_COND_ANALYZER	35
/*******************************************************/
#define MMCP_TYPE_COUNT			36
/*******************************************************/


//#define MMCP_MUTEX_DEBUG
//#define MMCP_MALLOC_USE_CSMALLOC
//#define  MMCP_MALLOC_USE_CSMALLOC
#if defined(MMCP_MALLOC_USE_CSMALLOC)

#define  MMCP_MemMalloc(x,y) CSUDIOSMalloc(y)
#define  MMCP_MemCalloc(x,y,z) CSUDIOSCalloc(y,z)
#define  MMCP_MemRealloc(x,y,z) CSUDIOSRealloc(y,z)
#define  MMCP_MemFree(x,y) (CSUDI_SUCCESS == CSUDIOSFree(y))

#elif defined(MMCP_MEMLEAK_SUPPORT)
#include "cs_memleak.h"
#define  MMCP_MemMalloc(x,y) (dbg_malloc((x),__FILE__,__LINE__,(y)))
#define  MMCP_MemCalloc(x,y,z) (dbg_calloc((x),__FILE__,__LINE__,(y), (z)))
#define  MMCP_MemRealloc(x,y,z) (dbg_realloc((x),__FILE__,__LINE__,(y), (z)))
#define  MMCP_MemFree(x,y) dbg_free((x),__FILE__,__LINE__,(y))

#elif defined(MMCP_BOUNDCHECK_SUPPORT)
#include "cs_boundcheck.h"
#define  MMCP_MemMalloc(x,y) (bc_malloc(y))
#define  MMCP_MemCalloc(x,y,z) (bc_calloc((y), (z)))
#define  MMCP_MemRealloc(x,y,z) (bc_realloc((y), (z)))
#define  MMCP_MemFree(x,y) (0 == bc_free((y)))

#else

void* MMCP_MemMalloc(WORD type,DWORD size);
void* MMCP_MemCalloc(WORD type, DWORD nmemb, DWORD size);
void* MMCP_MemRealloc(WORD type,void* pold,DWORD size);
BOOL  MMCP_MemFree(WORD type, void* addr);

#endif

DWORD MMCP_MemGetUsedSize(WORD type);
int MMCP_MemShowInfo(void);
int MMCP_MemDumpUsed(void);
int  MMCP_MemUnInit(void);

typedef enum
{
	EM_MEM_MODE_START = 0x100,  // 枚举边界
	EM_MEM_NORMAL,              // 直接调用udi接口，可限制内存使用大小
	EM_MEM_DEBUG,               // 含有调试信息
	EM_MEM_DLMALLOC,            // dlmalloc 
	EM_MEM_MODE_END             // 枚举边界
} CSMemMgrMode_E;

/**
@brief      返回已注册的内存分配方式个数
@param[in]  void
@return     已注册的内存分配方式个数
*/
int CSFSMEMGetModeCount(void);

/**
@brief      返回已注册的内存分配方式
@param[in]  arraySize 已注册的内存分配方式个数
@return     peOutModes 已注册的内存分配方式的数组
*/
int CSFSMEMGetModes(CSMemMgrMode_E *peOutModes,int arraySize);

/**
@brief     设置内存分配方式(只能设置一次，如果没有调用设置缺省按MMCP_MEM_NORMAL方式)
@param[in] eMemMgrType  内存分配方式
@return    返回是否成功，== TRUE 成功，== FALSE 失败
*/
BOOL CSFSMemSetMgrMode(CSMemMgrMode_E eMemMgrMode);

#define CSFS_Malloc(size)		MMCP_MemMalloc(MEM_TYPE_DTVFS,size)
#define	CSFS_Calloc(count,size)	MMCP_MemCalloc(MEM_TYPE_DTVFS,(count),(size))
#define	CSFS_Realloc(old,size)	MMCP_MemRealloc(MEM_TYPE_DTVFS,(old),(size))
#define	CSFS_Free(ptr)		    MMCP_MemFree(MEM_TYPE_DTVFS,(ptr))

/*****************************************************/
/**         for mutex debug                         **/
/*****************************************************/
#define MUTEX_TYPE_MIN             0
#define MUTEX_TYPE_GENERAL	       0
/* DSMCC: kernel/dsm, kernel/oc */
#define MUTEX_TYPE_DSMCC	       1
/* dtvfs: kernel/dtvfs */
#define MUTEX_TYPE_DTVFS	       2
/* event manager: kernel/event */
#define MUTEX_TYPE_EVENT           3
/* osplus: not finished */
#define MUTEX_TYPE_OSPLUS	       4
/* graphics: MMCP/graphics */
#define MUTEX_TYPE_GRAPHICS        5
/* MPI: only st5516 */
#define MUTEX_TYPE_MPI		       6	
/* javavm */
#define MUTEX_TYPE_JAVAVM	       7	
#define MUTEX_TYPE_MAX             7

#ifdef MMCP_MUTEX_DEBUG
int _MMCP_Mutex_Init(int type, pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
int _MMCP_Mutex_Destroy(int type, pthread_mutex_t *mutex);
#define MMCP_MutexInit(type, mutex, attr)  _MMCP_Mutex_Init(type, mutex, attr)
#define MMCP_MutexDestroy(type, mutex)     _MMCP_Mutex_Destroy(type, mutex)
#else
#define MMCP_MutexInit(type, mutex, attr)  pthread_mutex_init(mutex, attr)
#define MMCP_MutexDestroy(type, mutex)     pthread_mutex_destroy(mutex)

#endif
#define CSFS_MutexInit(type, mutex, attr)  __MMCP_Mutex_Init(mutex)
#define CSFS_MutexDestroy(type, mutex)     __MMCP_Mutex_Destroy(mutex)
#define CSFS_MutexLock(mutex)              __MMCP_Mutex_Lock(mutex)
#define CSFS_MutexUnlock(mutex)            __MMCP_Mutex_Unlock(mutex)

#ifdef __cplusplus
}
#endif
  
#endif /*_MMCP_MEMORY_H_*/

