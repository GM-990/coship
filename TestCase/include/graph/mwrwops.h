#ifndef _GRARWOPS_H_
#define _GRARWOPS_H_

#include <stdio.h>
#include <mwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
//#include <sys/stat.h>//xiachao,complict with cs_fsapi.h
//#include <cs_fcntl.h>//xiachao
	
typedef struct GRA_RWops {
 int (*seek) (struct GRA_RWops * context, int offset, int whence);
 int (*read) (struct GRA_RWops * context, void *ptr, int size, int maxnum);
 int (*write) (struct GRA_RWops * context, const void *ptr, int size, int num);
 int (*close) (struct GRA_RWops * context);

 Uint32 type;
	union {
	    struct {
		int autoclose;
		int fp;
	    } stdio;
	    struct {
		Uint8 *base;
		Uint8 *here;
		Uint8 *stop;
	    } mem;
	    struct {
		void *data1;
	    } unknown;
	} hidden;

    } GRA_RWops;


extern GRA_RWops *GRA_RWFromFile(const char *file, int mode);
extern GRA_RWops *GRA_RWFromFP(int fp, int autoclose);
extern GRA_RWops *GRA_RWFromMem(void *mem, int size);

extern GRA_RWops *GRA_AllocRW(void);
extern void GRA_FreeRW(GRA_RWops * area);

#define GRA_RWseek(ctx, offset, whence)	(ctx)->seek(ctx, offset, whence)
#define GRA_RWtell(ctx)			(ctx)->seek(ctx, 0, SEEK_CUR)
#define GRA_RWread(ctx, ptr, size, n)	(ctx)->read(ctx, ptr, size, n)
#define GRA_RWwrite(ctx, ptr, size, n)	(ctx)->write(ctx, ptr, size, n)
#define GRA_RWclose(ctx)			(ctx)->close(ctx)

#ifdef __cplusplus
}
#endif

#endif	
