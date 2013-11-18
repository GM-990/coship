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

/**�м����ģ����memleak�з����ģ���
*  ģ�����μ�cs_memory.h 
*/



/**
@brief memleak��ʼ������

@return ��
@note ����memleak�ĺ���֮ǰһ��Ҫ���г�ʼ��
*/
void CSMemLeakInit();

/**
@brief ��ӡϵͳ������ģ���memleak�������������δ�ͷ��ڴ漰����ʱ��λ��
@return ��
@note �ú�����ͬ��CSMemLeakModule(-1)
*/
void CSMemLeakDump();

/**
@brief ��ӡϵͳ������ģ���ڴ�ʹ�������������ֵ�͵�ǰֵ
@return ��
*/
void CSMemModuleDump();

/**
@brief ��ӡĳģ���memleak ���
@param[in] nModule ��ģ��鿴memleak�����-1��ʾ����ģ��
@return nModuleΪ-1ʱ��ͬ��CSMemLeakDump
*/
void CSMemLeakModule(int nModule);

/**
@brief �õ�ָ��ģ�鵱ǰռ���ڴ��С
@param[in] nModule ��ģ��鿴memleak���
@return nModuleģ��ռ�õ��ڴ��С
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
