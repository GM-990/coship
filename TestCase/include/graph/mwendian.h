#ifndef _GRAENDIAN_H
#define _GRAENDIAN_H

#include <stdio.h>
#include <mwtypes.h>
#include <mwrwops.h>


#ifdef __cplusplus
extern "C" {
#endif

#define GRA_LIL_ENDIAN	1234
#define GRA_BIG_ENDIAN	4321

#ifndef GRA_BYTEORDER
#define GRA_BYTEORDER GRA_LIL_ENDIAN
#else 
#define GRA_CHECK_ENDIAN (GRA_BYTEORDER==GRA_BIG_ENDIAN)?GRA_BIG_ENDIAN:GRA_LIL_ENDIAN
#undef GRA_BYTEORDER 
#define GRA_BYTEORDER GRA_CHECK_ENDIAN	
#endif 	

extern Uint16 GRA_Swap16(Uint16 D);
extern Uint32 GRA_Swap32(Uint32 D);
extern Uint64 GRA_Swap64(Uint64 val);

extern Uint16 GRA_ReadLE16(GRA_RWops * src);
extern Uint16 GRA_ReadBE16(GRA_RWops * src);
extern Uint32 GRA_ReadLE32(GRA_RWops * src);
extern Uint32 GRA_ReadBE32(GRA_RWops * src);
extern Uint64 GRA_ReadLE64(GRA_RWops * src);
extern Uint64 GRA_ReadBE64(GRA_RWops * src);


extern int GRA_WriteLE16(GRA_RWops * dst, Uint16 value);
extern int GRA_WriteBE16(GRA_RWops * dst, Uint16 value);
extern int GRA_WriteLE32(GRA_RWops * dst, Uint32 value);
extern int GRA_WriteBE32(GRA_RWops * dst, Uint32 value);
extern int GRA_WriteLE64(GRA_RWops * dst, Uint64 value);
extern int GRA_WriteBE64(GRA_RWops * dst, Uint64 value);


#if GRA_BYTEORDER == GRA_LIL_ENDIAN

#define GRA_SwapLE16(X)	(X)
#define GRA_SwapLE32(X)	(X)
#define GRA_SwapLE64(X)	(X)
#define GRA_SwapBE16(X)	GRA_Swap16(X)
#define GRA_SwapBE32(X)	GRA_Swap32(X)
#define GRA_SwapBE64(X)	GRA_Swap64(X)

#else

#define GRA_SwapLE16(X)	GRA_Swap16(X)
#define GRA_SwapLE32(X)	GRA_Swap32(X)
#define GRA_SwapLE64(X)	GRA_Swap64(X)
#define GRA_SwapBE16(X)	(X)
#define GRA_SwapBE32(X)	(X)
#define GRA_SwapBE64(X)	(X)

#endif
    
#ifdef __cplusplus
}
#endif
#endif				/* _GRAENDIAN_H */
