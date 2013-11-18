#ifndef _GRACOMMON_H
#define _GRACOMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "mwtypes.h"

#include "mmcp_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MODULE_MICROWIN	"MicroWin"

/**��ʾ�йص�(�������ڵ�map unmap raise��ͼƬ ͼԪ��show,�ü���Ϣ)���Դ�ӡʹ�ô�ģ���
 * ��ӡ���� DEBUG_LEVEL
 */
#define MWSHOW          "MicroWin-show"

/**���ܷ���Ĵ�ӡ���������ͼƬʱ�䡢��ʾͼƬʱ��ȣ�ʹ�ô�ģ���
 * ��ӡ���� ʹ�� DEBUG_LEVEL
 */
#define MWPerFormance       "MicroWin-Performace"

/**��ȥ��ʾ�����(������Ϣ�����󾯸��ӡ��)������ӡʹ�ô�ģ���
 * ��ӡ���� �����������INFO_LEVEL ERROR_LEVEL WARN_LEVEL
 */
#define MICROWIN        "MicroWin"

/**ͼƬ�����ͷŴ�ӡ��Ϣ������ͼƬ�Ƿ��ͷŵ��ڴ�й¶
 * ��ӡ���� �����������INFO_LEVEL ERROR_LEVEL WARN_LEVEL
 */
#define MWIMG        "MicroWin-Image"

#define GRTRACE CSDEBUG


/**����ͳ�ƺ�
 * MWPERFORMANCE ������������֮�� 
 */
#ifdef _DEBUG

#define MWPERFORMANCE  \
		int nMWPStartTime, nMWPEndTime, nMWPStartLineNum;

#define MWPERFORMANCESTART \
		nMWPStartTime = CSUDIPlusOSGetTickCount();    \
		nMWPStartLineNum = __LINE__;

#define MWPERFORMANCEEND		\
	nMWPEndTime = CSUDIPlusOSGetTickCount();	\
	GRTRACE(MWPerFormance, DEBUG_LEVEL, "%s:line %d <-->%d use time: %d \n", __FILE__, nMWPStartLineNum, __LINE__, nMWPEndTime-nMWPStartTime);


#else
#define MWPERFORMANCE 
#define MWPERFORMANCESTART
#define MWPERFORMANCEEND
#endif	

typedef enum {
	GRA_ENOMEM,
	GRA_EFREAD,
	GRA_EFWRITE,
	GRA_EFSEEK,
	GRA_LASTERROR
} GRA_errorcode;

extern void GRA_PrintfNull(const char *fmt, ...);
extern void GRA_PrintfError(const char *fmt, ...);
extern void GRA_PrintfCommon(const char *fmt, ...);
extern void GRA_Error(GRA_errorcode code);
extern void *GRA_Malloc(Uint32 size);
extern void GRA_Free(void *fp);
extern void* GRA_Realloc(void* pf,Uint32 size);
extern void* GRA_Calloc(Uint32 elememts, Uint32 size);

#define GRA_NOUSE(x) (x=x)
#define GRA_Max(x,y) (((x)>(y))?(x):(y))
#define GRA_Min(x,y) (((x)>(y))?(y):(x))

#define GRA_Memcmp(s1,s2,n) memcmp(s1,s2,n)
#define GRA_Memcpy(dst,src,len) memcpy(dst,src,len)    
#define GRA_Memset(s,c,n) memset(s,c,n)

#define GRA_OutOfMemory()       GRA_Error(GRA_ENOMEM)

#ifdef DEBUG
#define GRA_Trace(statement) do{statement;}while(0)
#define GRA_EchoWhere() do{CSDEBUG(MODULE_MICROWIN, INFO_LEVEL,"(_)(%s)(%d)(%s) :\n",__FILE__,__LINE__,__FILE__);}while(0)
#define GRA_SetError GRA_PrintfError
#define GRA_Printf GRA_PrintfCommon

#else
#define GRA_Trace(statement)   statement
#define GRA_EchoWhere()
#define GRA_SetError GRA_PrintfError
#define GRA_Printf GRA_PrintfNull 
#endif

#ifdef __cplusplus
}
#endif
#endif				/* _GRACOMMON_H */
