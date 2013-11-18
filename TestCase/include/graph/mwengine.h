#ifndef _GRAENGINE_H_
#define _GRAENGINE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <mwtypes.h>
#include <mwcommon.h>
#include <mwrwops.h>
#include <mwendian.h>
#include <mwsys.h>
#include <mwtimer.h>
#include "cs_fsapi.h"
#include "cs_memory.h"


#include "mmcp_osg.h"
#include "mwfont.h"

typedef struct _CSGC *PGRA_GC;

/*macro define */
#define	MIN_GRACOORD	((INT) -32768)
#define	MAX_GRACOORD	((INT) 32767)	
#define CLIP_VISIBLE            0
#define CLIP_INVISIBLE          1
#define CLIP_PARTIAL            2

//facename 字符串长度
#define GRA_MAX_FACENAME_LEN 128
#define FP			(3.141592653989)
#define GRASWAP(a,b) do { a ^= b; b ^= a; a ^= b; } while (0)

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef FALSE
#define FALSE                   0
#endif

#define GRAMIN(a,b)              ((a) < (b) ? (a) : (b))
#define GRAMAX(a,b)              ((a) > (b) ? (a) : (b))

/*macro define*/

/*the outside color value pack 0xAARRGGBB and use high bits first*/
#define BLUEVALUE(argb)  ((argb) & 0xff)
#define GREENVALUE(argb) (((argb) >> 8) & 0xff)
#define REDVALUE(argb) (((argb) >> 16) & 0xff)
#define ALPHAVALUE(argb) (((argb) >> 24) & 0xff)
/*#AARRGGBB*/
#define GETPALENTRY(pal,index) ((unsigned long)(pal[index].b |\
	 (pal[index].g << 8) | (pal[index].r << 16) | (pal[index].a << 24)))


#define FORMAT_EQUAL(A, B)						\
    ((A)->BitsPerPixel == (B)->BitsPerPixel				\
     && ((A)->Rmask == (B)->Rmask) && ((A)->Amask == (B)->Amask))


#define FORMAT_EQUALEX(dst, src) FORMAT_EQUAL(&((dst)->format),&((src)->format))? (((dst)->format.BitsPerPixel <= 8)?((GdCmpPalette((dst)->palette, (src)->palette))?0:1):1):0

#define RGB_FROM_PIXEL(pixel, fmt, r, g, b)				\
{									\
	r = (((pixel&fmt->Rmask)>>fmt->Rshift)<<fmt->Rloss); 		\
	g = (((pixel&fmt->Gmask)>>fmt->Gshift)<<fmt->Gloss); 		\
	b = (((pixel&fmt->Bmask)>>fmt->Bshift)<<fmt->Bloss); 		\
}

#define RETRIEVE_RGB_PIXEL(buf, bpp, pixel)				   \
do {									   \
	switch (bpp) {							   \
		case 2:							   \
			pixel = *((Uint16 *)(buf));			   \
		break;							   \
									   \
		case 3: {						   \
		        Uint8 *B = (Uint8 *)(buf);			   \
			if(GRA_BYTEORDER == GRA_LIL_ENDIAN) {		   \
			        pixel = B[0] + (B[1] << 8) + (B[2] << 16); \
			} else {					   \
			        pixel = (B[0] << 16) + (B[1] << 8) + B[2]; \
			}						   \
		}							   \
		break;							   \
									   \
		case 4:							   \
			pixel = *((Uint32 *)(buf));			   \
		break;							   \
									   \
		default:						   \
			pixel = 0; /* appease gcc */			   \
		break;							   \
	}								   \
} while(0)

#define DISEMBLE_RGB(buf, bpp, fmt, pixel, r, g, b)			   \
do {									   \
	switch (bpp) {							   \
		case 2:							   \
			pixel = *((Uint16 *)(char*)(buf));			   \
		break;							   \
									   \
		case 3: {						   \
		        Uint8 *B = (Uint8 *)buf;			   \
			if(GRA_BYTEORDER == GRA_LIL_ENDIAN) {		   \
			        pixel = B[0] + (B[1] << 8) + (B[2] << 16); \
			} else {					   \
			        pixel = (B[0] << 16) + (B[1] << 8) + B[2]; \
			}						   \
		}							   \
		break;							   \
									   \
		case 4:							   \
			pixel = *((Uint32 *)(buf));			   \
		break;							   \
									   \
	        default:						   \
		        pixel = 0;	/* prevent gcc from complaining */ \
		break;							   \
	}								   \
	RGB_FROM_PIXEL(pixel, fmt, r, g, b);				   \
} while(0)


#define PIXEL_FROM_RGB(pixel, fmt, r, g, b)				\
{									\
	pixel = ((r>>fmt->Rloss)<<fmt->Rshift)|				\
		((g>>fmt->Gloss)<<fmt->Gshift)|				\
		((b>>fmt->Bloss)<<fmt->Bshift);				\
}
#define RGB565_FROM_RGB(pixel, r, g, b)					\
{									\
	pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);			\
}
#define RGB555_FROM_RGB(pixel, r, g, b)					\
{									\
	pixel = ((r>>3)<<10)|((g>>3)<<5)|(b>>3);			\
}
#define RGB888_FROM_RGB(pixel, r, g, b)					\
{									\
	pixel = (r<<16)|(g<<8)|b;					\
}
#define RGBA_FROM_ARGB8888(pixel, r, g, b, a)                           \
{                                                                       \
	r = ((pixel>>16)&0xFF);                                         \
	g = ((pixel>>8)&0xFF);                                          \
	b = (pixel&0xFF);                                               \
	a = (pixel>>24);                                                \
}
#define ASSEMBLE_RGB(buf, bpp, fmt, r, g, b) 				\
{									\
	switch (bpp) {							\
		case 2: {						\
			Uint16 pixel;					\
									\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);		\
			*((Uint16 *)(buf)) = pixel;			\
		}							\
		break;							\
									\
		case 3: {						\
                        if(GRA_BYTEORDER == GRA_LIL_ENDIAN) {		\
			        *((buf)+fmt->Rshift/8) = r;		\
				*((buf)+fmt->Gshift/8) = g;		\
				*((buf)+fmt->Bshift/8) = b;		\
			} else {					\
			        *((buf)+2-fmt->Rshift/8) = r;		\
				*((buf)+2-fmt->Gshift/8) = g;		\
				*((buf)+2-fmt->Bshift/8) = b;		\
			}						\
		}							\
		break;							\
									\
		case 4: {						\
			Uint32 pixel;					\
									\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);		\
			*((Uint32 *)(buf)) = pixel;			\
		}							\
		break;							\
	}								\
}
#define ASSEMBLE_RGB_AMASK(buf, bpp, fmt, r, g, b, Amask)		\
{									\
	switch (bpp) {							\
		case 2: {						\
			Uint16 *bufp;					\
			Uint16 pixel;					\
									\
			bufp = (Uint16 *)buf;				\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);		\
			*bufp = pixel | (*bufp & Amask);		\
		}							\
		break;							\
									\
		case 3: {						\
                        if(GRA_BYTEORDER == GRA_LIL_ENDIAN) {		\
			        *((buf)+fmt->Rshift/8) = r;		\
				*((buf)+fmt->Gshift/8) = g;		\
				*((buf)+fmt->Bshift/8) = b;		\
			} else {					\
			        *((buf)+2-fmt->Rshift/8) = r;		\
				*((buf)+2-fmt->Gshift/8) = g;		\
				*((buf)+2-fmt->Bshift/8) = b;		\
			}						\
		}							\
		break;							\
									\
		case 4: {						\
			Uint32 *bufp;					\
			Uint32 pixel;					\
									\
			bufp = (Uint32 *)buf;				\
			PIXEL_FROM_RGB(pixel, fmt, r, g, b);		\
			*bufp = pixel | (*bufp & Amask);		\
		}							\
		break;							\
	}								\
}

#define RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a)				\
{									\
	r = ((pixel&fmt->Rmask)>>fmt->Rshift)<<fmt->Rloss; 		\
	g = ((pixel&fmt->Gmask)>>fmt->Gshift)<<fmt->Gloss; 		\
	b = ((pixel&fmt->Bmask)>>fmt->Bshift)<<fmt->Bloss; 		\
	a = ((pixel&fmt->Amask)>>fmt->Ashift)<<fmt->Aloss;	 	\
}

#define DISEMBLE_RGBA(buf, bpp, fmt, pixel, r, g, b, a)			   \
do {									   \
	switch (bpp) {							   \
		case 2:							   \
			pixel = *((Uint16 *)(char*)(buf));			   \
		break;							   \
									   \
		case 3:	{                                		   \
		        Uint8 *b = (Uint8 *)buf;			   \
			if(GRA_BYTEORDER == GRA_LIL_ENDIAN) {		   \
			        pixel = b[0] + (b[1] << 8) + (b[2] << 16); \
			} else {					   \
			        pixel = (b[0] << 16) + (b[1] << 8) + b[2]; \
			}						   \
		}							   \
		break;							   \
									   \
		case 4:							   \
			pixel = *((Uint32 *)(buf));			   \
		break;							   \
									   \
		default:						   \
		        pixel = 0; 		                           \
		break;							   \
	}								   \
	RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a);			   \
} while(0)


#define PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a)				\
{									\
	pixel = ((r>>fmt->Rloss)<<fmt->Rshift)|				\
		((g>>fmt->Gloss)<<fmt->Gshift)|				\
		((b>>fmt->Bloss)<<fmt->Bshift)|				\
		((a>>fmt->Aloss)<<fmt->Ashift);				\
}
#define ASSEMBLE_RGBA(buf, bpp, fmt, r, g, b, a)			\
{									\
	switch (bpp) {							\
		case 2: {						\
			Uint16 pixel;					\
									\
			PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a);	\
			*((Uint16 *)(char*)(buf)) = pixel;			\
		}							\
		break;							\
									\
		case 3: { /* FIXME: broken code (no alpha) */		\
                        if(GRA_BYTEORDER == GRA_LIL_ENDIAN) {		\
			        *((buf)+fmt->Rshift/8) = r;		\
				*((buf)+fmt->Gshift/8) = g;		\
				*((buf)+fmt->Bshift/8) = b;		\
			} else {					\
			        *((buf)+2-fmt->Rshift/8) = r;		\
				*((buf)+2-fmt->Gshift/8) = g;		\
				*((buf)+2-fmt->Bshift/8) = b;		\
			}						\
		}							\
		break;							\
									\
		case 4: {						\
			Uint32 pixel;					\
									\
			PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a);	\
			*((Uint32 *)(buf)) = pixel;			\
		}							\
		break;							\
	}								\
}


#define ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB)	\
do {						\
	dR = (((sR-dR)*(A))>>8)+dR;		\
	dG = (((sG-dG)*(A))>>8)+dG;		\
	dB = (((sB-dB)*(A))>>8)+dB;		\
} while(0)

#define USE_DUFFS_LOOP
#ifdef USE_DUFFS_LOOP

#define DUFFS_LOOP8(pixel_copy_increment, width)			\
{ int n = (width+7)/8;							\
	switch (width & 7) {						\
	case 0: do {	pixel_copy_increment;				\
	case 7:		pixel_copy_increment;				\
	case 6:		pixel_copy_increment;				\
	case 5:		pixel_copy_increment;				\
	case 4:		pixel_copy_increment;				\
	case 3:		pixel_copy_increment;				\
	case 2:		pixel_copy_increment;				\
	case 1:		pixel_copy_increment;				\
		} while ( --n > 0 );					\
	}								\
}

#define DUFFS_LOOP4(pixel_copy_increment, width)			\
{ int n = (width+3)/4;							\
	switch (width & 3) {						\
	case 0: do {	pixel_copy_increment;				\
	case 3:		pixel_copy_increment;				\
	case 2:		pixel_copy_increment;				\
	case 1:		pixel_copy_increment;				\
		} while ( --n > 0 );					\
	}								\
}

#define DUFFS_LOOP(pixel_copy_increment, width)				\
	DUFFS_LOOP8(pixel_copy_increment, width)

#else

#define DUFFS_LOOP(pixel_copy_increment, width)				\
{ int n;								\
	for ( n=width; n > 0; --n ) {					\
		pixel_copy_increment;					\
	}								\
}
#define DUFFS_LOOP8(pixel_copy_increment, width)			\
	DUFFS_LOOP(pixel_copy_increment, width)
#define DUFFS_LOOP4(pixel_copy_increment, width)			\
	DUFFS_LOOP(pixel_copy_increment, width)

#endif 

/*
* 使用IMAGEHDR 构造GRA_Bitmap结构 */
#define GRA_BITMAP_FROM_PIMAGEHDR(sBitmap, pIMAGEHDR)    \
do {						\
    sBitmap.w = pIMAGEHDR->w;   \
    sBitmap.h = pIMAGEHDR->h;   \
    sBitmap.planes = pIMAGEHDR->planes;   \
    sBitmap.pitch = pIMAGEHDR->pitch;   \
    sBitmap.compression = pIMAGEHDR->compression;   \
    sBitmap.format = pIMAGEHDR->format;   \
    sBitmap.palette = pIMAGEHDR->palette;   \
    sBitmap.pixels = pIMAGEHDR->pixels;   \
    sBitmap.colorkeyflag = pIMAGEHDR->colorkeyflag;   \
    sBitmap.alphaflag = pIMAGEHDR->alphaflag;   \
    sBitmap.geometry = pIMAGEHDR->geometry;   \
    sBitmap.delay_in_deciseconds = 0;   \
    sBitmap.currentFrame = 0;   \
    sBitmap.frameCount = 0;   \
    sBitmap.playCount = 0;   \
    sBitmap.pPrivate_info = NULL;   \
    sBitmap.nSourceType = 0;   \
    sBitmap.timer = 0;   \
} while(0)

/*
* 使用GRA_Bitmap  构造IMAGEHDR结构 */
#define GRA_IMAGEHDR_FROM_BITMAP(IMAGEHDR, psBitmap)    \
do {						\
    IMAGEHDR.w = psBitmap->w;   \
    IMAGEHDR.h = psBitmap->h;   \
    IMAGEHDR.planes = psBitmap->planes;   \
    IMAGEHDR.pitch = psBitmap->pitch;   \
    IMAGEHDR.compression = psBitmap->compression;   \
    IMAGEHDR.format = psBitmap->format;   \
    IMAGEHDR.palette = psBitmap->palette;   \
    IMAGEHDR.pixels = psBitmap->pixels;   \
    IMAGEHDR.colorkeyflag = psBitmap->colorkeyflag;   \
    IMAGEHDR.alphaflag = psBitmap->alphaflag;   \
    IMAGEHDR.geometry = psBitmap->geometry;   \
} while(0)

/*
* 使用IMAGEINFO  构造IMAGEHDR结构 */
#define GRA_IMAGEHDR_FROM_IMAGEINFO(IMAGEHDR, psImageInfo)    \
do {						\
    IMAGEHDR.w = psImageInfo->w;   \
    IMAGEHDR.h = psImageInfo->h;   \
    IMAGEHDR.planes = psImageInfo->planes;   \
    IMAGEHDR.pitch = psImageInfo->pitch;   \
    IMAGEHDR.compression = psImageInfo->compression;   \
    IMAGEHDR.format = psImageInfo->format;   \
    IMAGEHDR.palette = psImageInfo->palette;   \
    IMAGEHDR.pixels = psImageInfo->pixels;   \
    IMAGEHDR.colorkeyflag = psImageInfo->colorkeyflag;   \
    IMAGEHDR.alphaflag = psImageInfo->alphaflag;   \
} while(0)


#define GRAITEM_OFFSET(type, field)    ((long)&(((type *)0)->field))

#define GdItemAlloc(size) GRA_Malloc(size)

void GdListAdd(PGRALISTHEAD pHead, PGRALIST pItem);
void GdListInsert(PGRALISTHEAD pHead, PGRALIST pItem);
void GdListRemove(PGRALISTHEAD pHead, PGRALIST pItem);

#define GdItemNew(type)	((type *)GdItemAlloc(sizeof(type)))
#define GdItemFree(ptr)	GRA_Free(ptr)

#define GdItemAddr(p,type,list)	((type *)((long)p - GRAITEM_OFFSET(type,list)))

int GdInit(void);
int GdExit(void);

extern void GdSetAntiFlickering(GRA_Display * display, int anti);

GRA_Display *GdOpenDisplay(GRA_Rect * rect, CSGraphPixelFormat * format);
int GdCloseDisplay(GRA_Display * display);

int GdHideDisplay(GRA_Display * display);
int GdShowDisplay(GRA_Display * display);
int GdMoveDisplay(GRA_Display * display, INT x, INT y);
int GdSetAlphaMode(GRA_Display * display, UCHAR mode);
int GdSetTransparence(GRA_Display * display, UCHAR alpha);

PSD GdOpenScreen(GRA_Display * display);
int GdActiveScreen(PSD psd);
int GdCloseScreen(PSD psd);
int GdGetScreenInfo(PSD psd, GRA_ScreenInfo * info);

PSD GdOpenMemScreen(GRA_Display * display);
PSD GdOpenMemScreenEx(GRA_Display * display, Uint16 w, Uint16 h);
int GdCloseMemScreen(PSD psd);

GRA_Bitmap *GdAllocBitmap(Uint32 width, Uint32 height, Uint8 bpp, Uint32 rmask, Uint32 gmask, Uint32 bmask, Uint32 amask);
int GdGetBitmapPitch( Uint32 width, Uint32 bpp,Uint32 BytesPerPixel);

void GdFreeBitmap(GRA_Bitmap * bitmap);
void GdSetColorKey(GRA_Bitmap * bitmap, Uint32 colorkey);
void GdClearColorKey(GRA_Bitmap * bitmap);
void GdSetAlpha(GRA_Bitmap * bitmap, Uint8 alpha);
void GdClearAlpha(GRA_Bitmap * bitmap);

int GdSaveBMP(GRA_Bitmap * saveme, GRA_RWops * dst, int freedst);

void *GdBuildBMPBuf(const Uint8 *pBitmapMemorySrc, int nMemoryCount, Uint32 colorDepth, 
                                    const Uint8 *pPalette, int nPaletteCount, int w, int h, int *pnBmpLen);
int GdReleaseBMPBuf(void *pBMPBuf);

GRA_Bitmap *GdLoadBMP(GRA_RWops * src, int freesrc);

#define GdFreeFormat(fmt) GRA_Free(frm)
CSGraphPixelFormat *GdAllocFormat(int bpp, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
CSGraphPalette *GdAllocPalette(Uint8 bppcount, int type);
CSGraphPalette *GdAllocPaletteEx(Uint16 colorcount, int type);
void GdFreePalette(CSGraphPalette * palette);
int GdSetRGBPalette(PSD psd, Uint8 first, Uint16 count, CSGraphPalette * palette);
int GdSetYUVPalette(PSD psd, Uint8 first, Uint16 count, CSGraphPalette * palette);

int GdGetPalette(PSD psd, CSGraphPalette * palette);
CSGraphPalette *GdGetPaletteEx(PSD psd);

Uint32 GdFindColor(PSD psd, Uint32 colorval);
Uint32 GdFindColorByRGBA(PSD psd, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
Uint32 GdFindColorByRGBAEx(GRA_Bitmap * bitmap, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
Uint8 GdFindColorByPalette(CSGraphPalette * palette, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
Uint8 GdFindNearestColor(CSGraphPalette * palette, Uint32 colorval);
BOOL GdColorInPalette(CSGraphPalette * palette, Uint32 colorval);
void GdMakePaletteConversionMap(CSGraphPalette * dstpal, CSGraphPalette * srcpal, Uint8 * map);
int GdCmpPalette(CSGraphPalette * dstpal, CSGraphPalette * srcpal);

void GdPoint(PSD psd, Sint16 x, Sint16 y, PGRA_GC gc);
void GdLine(PSD psd, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, PGRA_GC gc);

void GdRect(PSD psd, Sint16 x, Sint16 y, Uint16 w, Uint16 h, PGRA_GC gc);
void GdFillRect(PSD psd, Sint16 x, Sint16 y, Uint16 w, Uint16 h, PGRA_GC gc);

void GdCircle(PSD psd, Sint16 x0, Sint16 y0, Uint16 r, PGRA_GC gc);
void GdFillCircle(PSD psd, Sint16 x0, Sint16 y0, Uint16 r, PGRA_GC gc);

void GdPoly(PSD psd, Uint16 numbers, CSGraphPoint * points, PGRA_GC gc);
void GdFillPoly(PSD psd, Uint16 numbers, CSGraphPoint * points, PGRA_GC gc);

void GdArc(PSD psd, Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint16 salpha, Uint16 ealpha, PGRA_GC gc);
void GdFillArc(PSD psd, Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint16 salpha, Uint16 ealpha, PGRA_GC gc);

void GdEllipse(PSD psd, Sint16 x, Sint16 y, Uint16 xr, Uint16 yr, Uint16 alpha, PGRA_GC gc);
void GdFillEllipse(PSD psd, Sint16 x, Sint16 y, Uint16 xr, Uint16 yr, Uint16 alpha, PGRA_GC gc);

int GdBlit(PSD srcpsd, GRA_Rect * srcrect, PSD dstpsd, GRA_Rect * dstrect, PGRA_GC gc);
int GdCopyAreaToFit(PSD srcpsd, GRA_Rect * src_rect, PSD dstpsd, GRA_Rect * dst_rect, PGRA_GC sgc);
int GdBlitBitmap(GRA_Bitmap * srcbitmap, GRA_Rect * srcrect, GRA_Bitmap * dstbitmap, GRA_Rect * dstrect);
int GdStretchBlitBitmap(GRA_Bitmap * src, GRA_Rect * srcrect, GRA_Bitmap * dst, GRA_Rect * dstrect);
GRA_Bitmap *GdStretchBitmap(GRA_Bitmap * src, GRA_Rect * srcrect, Uint16 width, Uint16 height);
int GdDrawBitmap(PSD psd, GRA_Rect * dstrect, GRA_Rect * srcrect, GRA_Bitmap * bitmap, CSGraphGC * gc);
int GdDrawBitmapToFit(PSD psd, GRA_Rect * dstrect, GRA_Rect * srcrect, GRA_Bitmap * bitmap, CSGraphGC * gc);

int GdCopyAreaToBitmap(PSD psd, GRA_Rect * rect, GRA_Bitmap * bitmap);
GRA_Bitmap *GdCopyArea(PSD psd, GRA_Rect * rect);

#define MAXTEXTLEN 256
int GdSetFontSize(int id, Uint8 fontsize);
int GdSetFontAttr(int id, int style);
int GdCreateFont(char *fontname, int fontsize, long index);
int GdGetFontInfo(int id, GRA_FontInfo * info);
int GdDestroyFont(int id);
int GdGetTextSize(int id, const void *str, Uint16 count, int *w, int *h, long codeflag);
int GdText(PSD psd, INT x, INT y, const void *str, int count, long codeflag, PGRA_GC gc);
int GdTextEx(PSD psd, GRA_Bitmap * bitmap,INT x, INT y, const void *str, int count, long codeflag, PGRA_GC gc);
int GdCreateFontFromMem(char *fontname, int fontsize, long index,unsigned char *fontdata,unsigned int dsize);
int  GdUpdateFontData(char * filename, Uint8 * pucNewData, int nNewDataSize, long lFaceIndex);
int GdAddFontEngine(GR_FONT_Driver_Proc  *pFontProc);
int GdAddFontDataKeyWord(const char * pcFontData, int nDataLen);
int GdGetFontType(unsigned char *pucFontdata, unsigned int nSize);
int GdGetCharFontData(int id, unsigned short usCharCode, UCHAR *pFontDataMem, int nMemSize, int * pnWidth, int  * pnHeight);

BOOL GdClipPoint(PSD psd, INT x, INT y);
int GdClipArea(PSD psd, INT x1, INT y1, INT x2, INT y2);
void GdSetPSDClipRects(PSD psd, int count, GRACLIPRECT * table);
void GdSetClipRects(GRA_Display * display, int count, GRACLIPRECT * table);
BOOL GdPtInRegion(GRACLIPREGION * rgn, INT x, INT y);
int GdRectInRegion(GRACLIPREGION * rgn, const GRARECT * rect);
BOOL GdEqualRegion(GRACLIPREGION * r1, GRACLIPREGION * r2);
BOOL GdEmptyRegion(GRACLIPREGION * rgn);
GRACLIPREGION *GdAllocRegion(void);
GRACLIPREGION *GdAllocRectRegion(INT left, INT top, INT right, INT bottom);
GRACLIPREGION *GdAllocRectRegionIndirect(GRARECT * prc);
void GdSetRectRegion(GRACLIPREGION * rgn, INT left, INT top, INT right, INT bottom);
void GdSetRectRegionIndirect(GRACLIPREGION * rgn, GRARECT * prc);
void GdDestroyRegion(GRACLIPREGION * rgn);
void GdOffsetRegion(GRACLIPREGION * rgn, INT x, INT y);
int GdGetRegionBox(GRACLIPREGION * rgn, GRARECT * prc);
void GdUnionRectWithRegion(const GRARECT * rect, GRACLIPREGION * rgn);
void GdSubtractRectFromRegion(const GRARECT * rect, GRACLIPREGION * rgn);
void GdCopyRegion(GRACLIPREGION * d, GRACLIPREGION * s);
void GdIntersectRegion(GRACLIPREGION * d, GRACLIPREGION * s1, GRACLIPREGION * s2);
void GdUnionRegion(GRACLIPREGION * d, GRACLIPREGION * s1, GRACLIPREGION * s2);
void GdSubtractRegion(GRACLIPREGION * d, GRACLIPREGION * s1, GRACLIPREGION * s2);
void GdXorRegion(GRACLIPREGION * d, GRACLIPREGION * s1, GRACLIPREGION * s2);

int GdLoadImageFromBuffer(PSD psd, void *buffer, int size, GRA_Bitmap ** poutbitmap, int flags);
int GdLoadImageFromBufferEx(PSD psd, void *buffer, int size, GRA_Bitmap ** poutbitmap, int flags);
void GdDrawImageFromBuffer(PSD psd, INT x, INT y, INT width, INT height, void *buffer, int size, int flags, PGRA_GC gc);
int GdLoadImageFromBufferScaled(PSD psd, void *buffer, int size, GRA_Bitmap ** poutbitmap, GRA_Rect *rect, int flag);
char *GdLoadImgFromBufferToBuffer(GRA_Bitmap *pimage, void *buffer, int size, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);
char *GdLoadImgFromBufferScaledToBuffer(GRA_Bitmap *pimage, void *buffer, int size, GRA_Rect *rect, GR_AllocBufferCallback_F fnAllocBuffercallback, void  *pUserData);	
void GdDrawImageFromFile(PSD psd, INT x, INT y, INT width, INT height, char *file, PGRA_GC gc);
int GdLoadImageFromFile(PSD psd, char *file, GRA_Bitmap ** poutbitmap);
int GdLoadImageFromFileScaled(PSD psd, char *file, GRA_Bitmap ** poutbitmap, GRA_Rect *rect, int flag);
char *GdLoadImgFromFileScaledToBuffer(GRA_Bitmap *pimage, char *pcFileName, GRA_Rect *rect, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);
char *GdLoadImgFromFileToBuffer(GRA_Bitmap *pimage, char *pcFileName, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);
int GdLoadImageFromFileEx(PSD psd, char *file, GRA_Bitmap ** poutbitmap);
int GdDrawImageToFit(PSD psd, GRA_Rect * dstrect, GRA_Rect * srcrect, int id, PGRA_GC gc);
int GdDrawImage(PSD psd, GRA_Rect * dstrect, GRA_Rect * srcrect, int id, PGRA_GC gc);
void GdFreeImage(int id);
int GdLoadImageFromId(PSD psd, int id,GRA_Rect * rect, GRA_Bitmap ** poutbitmap);
GRA_Bitmap *GdTranslateBitmap(GRA_Bitmap * dstbitmap, GRA_Rect * rect, GRA_Bitmap * srcbitmap, int flag);
GRA_Bitmap *GdTranslateBitmapEx(PSD psd, GRA_Rect * rect, GRA_Bitmap * srcbitmap);


PGRAIMAGEHDR GdGetImageInfo(int id);
GRA_Bitmap *GdLoadImage(const char *file);
GRA_Bitmap *GdLoadImageEx(PSD psd, const char *file);
int GdRotateImage(PSD psd, GRA_Bitmap** outbitmap, GRA_Bitmap* bitmap, INT nRotateAngle);
int GdMirrorImage(PSD psd, GRA_Bitmap** outbitmap, GRA_Bitmap* bitmap);

int GdTimerInit(void);
void GdTimerQuit(void);
void GdDestroyTimer(GRA_Timer * timer);
void GdFreeTimer(int id);
GRA_Timer *GdFindTimer(int id);
double GRA_GetTime(void);
void GdFreeGiffile(GIFInfo *gif_info);

int GdLoadImageFromBits(PSD psd, GRA_Bitmap *  pimage, GRA_Bitmap ** poutbitmap);
GRA_Bitmap *GdGetPixeldataFromImage(GRA_Bitmap *pSrcBitmap, GRA_Rect * srcrect);
//add for multiFrame Image such as apng gif and so on
GRA_Bitmap *GdGetNextImageFrame(GRA_Bitmap *pBitmap);
void GdFreeApngBitmap(GRA_Bitmap * pBitmap);

int GdShowAllIcons(int nClient, PSD psd);
int GdRestoreIconBackground(int nClient, PSD psd);
int GdInterchangeScreenSurface(PSD psd);

int GdIconInitial(void);
int GdIconRelease(void);
int GdAddIcon(MW_ICON_S *psIcon);
int GdRemoveIcon(int nIconId);
int GdShowIcon(int nIconId);
int GdHideIcon(int nIconId);
BOOL GdGetLoadImgFrom3DFlag(void);
void GdCheckCursor(PSD psd, int x1, int y1, int x2,int y2);
void GdFixCursor(PSD psd);

#if SUPPORT_MUTIL_THREAD
#define GdAddTimer(timerout,callback,arg) GRA_AddTimer(GRA_TIMER_ONESHOT,timerout,callback,arg,pthread_self());
#define GdAddPeriodicTimer(timerout,callback,arg) GRA_AddTimer(GRA_TIMER_PERIODIC,timerout,callback,arg,pthread_self());
#else
#define GdAddTimer(timerout,callback,arg) GRA_AddTimer(GRA_TIMER_ONESHOT,timerout,callback,arg,0);
#define GdAddPeriodicTimer(timerout,callback,arg) GRA_AddTimer(GRA_TIMER_PERIODIC,timerout,callback,arg,0);
#endif
#define GdTimeout() GRA_TimerCheck()
#define GdGetTicks() GRA_GetTicks()
#define GdGetNextTimeout(a,b) FALSE

extern GRA_Mutex *timer_mutex;
extern GRA_Mutex *client_mutex;
extern GRA_Mutex *do_mutex;
extern GRA_Mutex *gif_mutex;
extern CSGraphDisplayDevice *display_device;
extern CSGraphGC default_gc;
extern GRA_Mutex *destroy_win_mutex;
extern PSD g_psd;
extern UINT g_mwclockspersecond;

#ifdef __cplusplus
}
#endif
#endif /*_GRAENGINE_H_*/
