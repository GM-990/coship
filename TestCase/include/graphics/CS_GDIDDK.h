
#ifndef __CS_GDIDDK_H
#define __CS_GDIDDK_H

#ifdef  __cplusplus
extern "C" {
#endif
#define	HANDLE			HCSHANDLE
#define	HWND			HCSWND
#define	HDC				HCSDC
#define	HBITMAP		HCSBITMAP
#define	SIZE			CSSIZE
#define	RECTL			CSRECTL
#define	RECT			CSRECT
#define	PRECTL			PCSRECTL
#define	SIZEL			CSSIZEL
#define	LOGFONTW		CSLOGFONT
#define	HPALETTE		HCSPALETTE
#define	POINTL			CSPOINT
#define	HBITMAP		HCSBITMAP
#define	PALETTEENTRY	CSPALETTEENTRY
#define	HLOCAL			HCSHANDLE

//y_junjie 2004.10.8 for PixelFormat
typedef DWORD ARGB;

#define	ALPHA_SHIFT	24
#define	RED_SHIFT		16
#define	GREEN_SHIFT	8
#define	BLUE_SHIFT		0
#define	ALPHA_MASK	((ARGB) 0xff << ALPHA_SHIFT)

// In-memory pixel data formats:
// bits 0-7 = format index
// bits 8-15 = pixel size (in bits)
// bits 16-23 = flags
// bits 24-31 = reserved

typedef INT PixelFormat;

#define	PixelFormatIndexed		0x00010000 // Indexes into a palette
#define	PixelFormatGDI			0x00020000 // Is a GDI-supported format
#define	PixelFormatAlpha		0x00040000 // Has an alpha component
#define	PixelFormatPAlpha		0x00080000 // Pre-multiplied alpha
#define	PixelFormatExtended	0x00100000 // Extended color 16 bits/channel
#define	PixelFormatCanonical	0x00200000 

#define	PixelFormatUndefined	0
#define	PixelFormatDontCare		0

#define	PixelFormat1bppIndexed		(1 | ( 1 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define	PixelFormat4bppIndexed		(2 | ( 4 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define	PixelFormat8bppIndexed		(3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define	PixelFormat16bppGrayScale	(4 | (16 << 8) | PixelFormatExtended)
#define	PixelFormat16bppRGB555	(5 | (16 << 8) | PixelFormatGDI)
#define	PixelFormat16bppRGB565	(6 | (16 << 8) | PixelFormatGDI)
#define	PixelFormat16bppARGB1555	(7 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI)
#define	PixelFormat16bppARGB4444	(8 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI) //y_junjie added for 4444 mode
#define	PixelFormat16bppBGR565	(9 | (16 << 8) | PixelFormatGDI) //zyw added for 4444 mode
#define	PixelFormat16bppABGR1555	(10 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI) //zyw added for 4444 mode
#define	PixelFormat16bppABGR4444	(11 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI) //zyw added for 4444 mode
#define	PixelFormat24bppRGB		(12 | (24 << 8) | PixelFormatGDI)
#define	PixelFormat32bppRGB		(13 | (32 << 8) | PixelFormatGDI)
#define	PixelFormat32bppARGB		(14 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical)
#define	PixelFormat32bppPARGB		(15 | (32 << 8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI)
#define	PixelFormat48bppRGB		(16 | (48 << 8) | PixelFormatExtended)
#define	PixelFormat64bppARGB		(17 | (64 << 8) | PixelFormatAlpha  | PixelFormatCanonical | PixelFormatExtended)
#define	PixelFormat64bppPARGB		(18 | (64 << 8) | PixelFormatAlpha  | PixelFormatPAlpha | PixelFormatExtended)
#define	PixelFormat16bppRGBA4444	(19 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI) //y_junjie added for 4444 mode
#define	PixelFormatMax				19

#ifdef  __cplusplus
}
#endif

#endif

