/** @addtogroup CSOS
@{
*/

#ifndef _CS_MEMLEAK_H_
#define _CS_MEMLEAK_H_

#include "mmcp_typedef.h"
#include "mmcp_debug.h"

#ifdef  __cplusplus
extern "C"
{
#endif

#define CHK_FREED 1
#define CHK_ALLOC 2
#define CHK_ALL (CHK_FREED | CHK_ALLOC)



#if !defined(MEMLEAK_TYPE)
#define MEMLEAK_TYPE 0
#endif

extern void *dbg_malloc(int nModule,char* pFile,int nLine,DWORD size);
extern void *dbg_realloc(int nModule,char* pFile,int nLine,void *ptr, DWORD size);
extern void *dbg_calloc(int nModule,char* pFile,int nLine,DWORD num, DWORD size);
extern BOOL dbg_free(int nModule ,char* pFile,int nLine,void *ptr);

/**中间件各模块在memleak中分配的模块号
*  模块名参见cs_memory.h 
*/



/**
@brief memleak初始化函数

@return 无
@note 调用memleak的函数之前一定要进行初始化
*/
void CSMemLeakInit();

/**
@brief 打印系统中所有模块的memleak情况，包括所有未释放内存及分配时的位置
@return 无
@note 该函数等同于CSMemLeakModule(-1)
*/
void CSMemLeakDump();

/**
@brief 打印系统中所有模块内存使用情况，包括峰值和当前值
@return 无
*/
void CSMemModuleDump();

/**
@brief 打印某模块的memleak 情况
@param[in] nModule 按模块查看memleak情况；-1表示所有模块
@return nModule为-1时等同于CSMemLeakDump
*/
void CSMemLeakModule(int nModule);

/**
@brief 得到指定模块当前占用内存大小
@param[in] nModule 按模块查看memleak情况
@return nModule模块占用的内存大小
*/
int CSMemLeakGetModuleMemSize(int nModule);


#define CSMemDump() (dbg_heap_dump(NULL))


//#define CSUDIOSMalloc(s) (dbg_malloc(MEMLEAK_TYPE,__FILE__,__LINE__,(s)))
//#define CSUDIOSRealloc(p,s) (dbg_realloc(MEMLEAK_TYPE,__FILE__,__LINE__,(p), (s)))
//#define CSUDIOSCalloc(n,s) (dbg_calloc(MEMLEAK_TYPE,__FILE__,__LINE__,(n), (s)))
//#define CSUDIOSFree(p) dbg_free(MEMLEAK_TYPE,__FILE__,__LINE__,(p))




#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* CS_OS_H */
