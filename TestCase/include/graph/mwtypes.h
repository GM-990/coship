#ifndef _GRATYPES_H
#define _GRATYPES_H

#ifdef __cplusplus
extern "C" {
#endif



#include <mmcp_osg.h>
#include <mmcp_os.h>
#include <mwscreen.h>

#include <pthread.h>
#define BYTEALIGN 1



//{{add by xiachao
#ifndef LOAD_BMP
#define LOAD_BMP
#endif

#ifndef _AUTOGIF_
#define _AUTOGIF_ 
#endif

#ifdef PLATFORM_ST40 
#ifndef ST_POSIX
#define ST_POSIX (1)
#endif
#endif
//}}

#define GRA_HW_SCREEN 0x00000001L
#define GRA_SW_SCREEN 0x00000002L
#define GRA_SCREEN_MASK 0x00000003L

#define GRA_COLORKEY_FLAG 0x00000001L
#define GRA_ALPHABLEND_FLAG 0x00000002L

#define GRA_ALPHA_OPAQUE 255
#define GRA_ALPHA_TRANSPARENT 0

#define ALIGN4(width) ((width)+3)&~3

typedef unsigned char Uint8;
typedef signed char Sint8;
typedef unsigned short Uint16;
typedef signed short Sint16;
typedef unsigned int Uint32;
typedef signed int Sint32;

typedef struct _CSRGBColor GRA_Color;

typedef struct {
	Uint32 hi;
	Uint32 lo;
} Uint64, Sint64;

typedef enum {
	GRA_FALSE = 0,
	GRA_TRUE = 1
} GRA_bool;

#ifdef _AUTOGIF_
typedef struct _GIFInfo_
{
    unsigned char *gif_buff;
    int bufLen;
    int cur_file_p; 
    struct GifFileType *giffile;
    struct layout_rect *cut;
}GIFInfo;
#endif


#ifndef _RECTANGLE_
#define _RECTANGLE_
struct layout_rect{
  short int x_position;
  short int y_position;
  short int width;
  short int height;
  char *buffer;
  unsigned char *file_data;
  #ifdef _AUTOGIF_
  GIFInfo *gif_info;
  #endif
};
typedef struct layout_rect GAR_layout_rect;
#endif

typedef struct _GRA_List {	/* LIST must be first decl in struct */
	struct _GRA_List *next;	/* next item */
	struct _GRA_List *prev;	/* previous item */
    } GRA_List;

typedef struct _GRA_List GRALIST, *PGRALIST;

typedef struct _GRA_Listhead {
	struct _GRA_List *head;	/* first item */
	struct _GRA_List *tail;	/* last item */
} GRA_ListHead;

typedef struct _GRA_Listhead GRALISTHEAD, *PGRALISTHEAD;

typedef struct _GRA_Rect {
	Sint16 x, y;
	Sint16 w, h;
} GRA_Rect;

typedef struct {
	INT x;		/* x coordinate of top left corner */
	INT y;		/* y coordinate of top left corner */
	INT width;		/* width of rectangle */
	INT height;		/* height of rectangle */
}GRACLIPRECT;

typedef struct {
	INT left;
	INT top;
	INT right;
	INT bottom;
} GRARECT;

typedef struct {
	int size;		/* malloc'd # of rectangles */
	int numRects;		/* # rectangles in use */
	int type;		/* region type */
	GRARECT *rects;		/* rectangle array */
	GRARECT extents;		/* bounding box of region */
} GRACLIPREGION;


typedef enum {
	EM_ANIMATION_LINEAR,     ///< 线性 速度从v -- v
	EM_ANIMATION_EASEIN,   ///<  初速为0的加速  速度从0 -- v
	EM_ANIMATION_EASEOUT,   ///<  初速不为0的减速  速度从v -- 0
	EM_ANIMATION_EASEINOUT, ///<  先加速后减速 速度从0 -- v  ---0
	EM_ANIMATION_NUM
} CSANIMATIONSTYLE_E;

/* region types */
#define GRAREGION_ERROR		0
#define GRAREGION_NULL		1
#define GRAREGION_SIMPLE		2
#define GRAREGION_COMPLEX	3

/* GdRectInRegion return codes*/
#define GRARECT_OUT	0	/* rectangle not in region */
#define GRARECT_ALLIN	1	/* rectangle all in region */
#define GRARECT_PARTIN	2	/* rectangle partly in region */

#define GRA_STYLE_NORMAL        0x00
#define GRA_STYLE_BOLD          0x01
#define GRA_STYLE_ITALIC        0x02
#define GRA_STYLE_UNDERLINE     0x04
#define GRA_STYLE_FIXEDWIDTH   (0x08) //lixiaowang.
#define GRA_STYLE_MASK     0x0F

#define GRA_FONT_CODE_MASK	0xFF
#define GRA_FONT_CODE_ASCII      0x01
#define GRA_FONT_CODE_UTF8       0x02
#define GRA_FONT_CODE_UNICODE    0x04

#define MAX_CLIPRECTS 256
#define MAX_FONTSIZE  256    
#define USE_FONTBITMAP 1
#define GRA_MAX_FILENAME_LEN 128

#define PNGCOMPRESSONFLAGE              0x7F
#define JPG_YUV_COMPRESSSION_FLAG   0X7d
#define BMPCOMPRESSSIONFLAG          0X7C

/*定义图片格式宏*/
//正常的单帧图片格式(bmp/signel gif)得到的bitmap
#define GRA_BITMAP_SOURCE_NORMAL    0
//JPEG格式解码得到的bitmap
#define GRA_BITMAP_SOURCE_JPEG       1
//多帧GIF图片格式解码得到的bitmap
#define GRA_BITMAP_SOURCE_AUTOGIF   2
//APNG格式解码得到的bitmap
#define GRA_BITMAP_SOURCE_APNG      3
//PNG格式解码得到的bitmap
#define GRA_BITMAP_SOURCE_PNG       4

/*
*Bitmap 结构体，仅在MicroWin内部使用 */
typedef struct _GRA_Bitmap {
	USHORT w;		/* image width in pixels */
	USHORT h;		/* image height in pixels */
	UCHAR planes;		/* # image planes */
	USHORT pitch;		/* bytes per line */
	UCHAR compression;	/* compression algorithm */
	CSGraphPixelFormat format;	/* bits per pixel(1,4 or 8,16,24,32) */
	CSGraphPalette *palette;
	UCHAR *pixels;	/* image bits (dword right aligned) */
	BOOL colorkeyflag;
	BOOL alphaflag;
    unsigned int  timer;
	struct layout_rect geometry;
	float delay_in_deciseconds;	/*  多帧图片当前帧持续时间*/
	int  currentFrame;			/*  多帧图片当前帧次 */
	int  frameCount;			/*  多帧图片总帧数 */
	unsigned long   playCount;	/*  多帧图片循环次数 */
	void* pPrivate_info;		/*  多帧图片的私有信息 用于得到下一帧时的解码*/
    int nSourceType;            /* 图片类型*/
} GRA_Bitmap;

/*
* GR_IMAGE_INFO 对应的结构体，Bitmap的必要部分*/
typedef struct _mw_ImageInfo {
	USHORT w;		            /* image width in pixels */
	USHORT h;		            /* image height in pixels */
	UCHAR planes;		        /* # image planes */
	USHORT pitch;		        /* bytes per line */
	UCHAR compression;      	/* compression algorithm */
	CSGraphPixelFormat format;	/* bits per pixel(1,4 or 8,16,24,32) */
	CSGraphPalette *palette;
	UCHAR *pixels;	            /* image bits (dword right aligned) */
	BOOL colorkeyflag;
	BOOL alphaflag;
    
	int nSourceType;               /* 图片类型 */
	float delay_in_deciseconds;	/* apng 多帧图片当前帧持续时间,单位毫秒,其他格式则无意义*/
	int  currentFrame;			    /* apng 多帧图片当前帧次,其他格式则无意义 */
	int  frameCount;			    /* apng 多帧图片总帧数,其他格式则无意义 */
	unsigned long   playCount;	    /* apng 多帧图片循环次数,其他格式则无意义 */
} MWIMAGEINFO;


/*
* GR_IMAGE_HDR 对应的结构体，Bitmap的必要部分*/
typedef struct _mw_imageHead {
	USHORT w;		/* image width in pixels */
	USHORT h;		/* image height in pixels */
	UCHAR planes;		/* # image planes */
	USHORT pitch;		/* bytes per line */
	UCHAR compression;	/* compression algorithm */
	CSGraphPixelFormat format;	/* bits per pixel(1,4 or 8,16,24,32) */
	CSGraphPalette *palette;
	UCHAR *pixels;	/* image bits (dword right aligned) */
	BOOL colorkeyflag;
	BOOL alphaflag;
    BOOL isautogif;        /* 在此结构体中无意义，为了兼容之前的结构*/
    unsigned int  timer;    /* 在此结构体中无意义，为了兼容之前的结构*/
	struct layout_rect geometry;
} MWIMAGEHDR;


typedef void* (*GR_AllocBufferCallback_F)(int nRgbDataSize, void *pUserData);

typedef struct _GRA_Display {
	GRA_List list;
	GRA_ListHead screenhead;
	CSGraphRegion *region;

	INT clipminx;	/* minimum x value of cache rectangle */
	INT clipminy;	/* minimum y value of cache rectangle */
	INT clipmaxx;	/* maximum x value of cache rectangle */
	INT clipmaxy;	/* maximum y value of cache rectangle */
	BOOL clipresult;	/* whether clip rectangle is plottable */

	int clipcount;		/* number of clip rectangles */
	GRACLIPRECT cliprects[MAX_CLIPRECTS];	/* clip rectangles */
#if USE_FONTBITMAP	
	GRA_Bitmap * fontbitmap;
#endif	
    } GRA_Display;		/*region */

typedef struct _GRA_Screen {
	GRA_List list;
	CSGraphSurface *surface;
	GRA_bool activeflag;
	GRA_Display *pdisplay;
    CSGraphSurface *surface_inactive;
    GRA_bool isDoubleBuffer; 
	int flag;
	int size;
	void *addr;
	Uint16 pitch;
	Uint16 width;
	Uint16 height;
	CSGraphPixelFormat format;
	CSGraphPalette *palette;
} GRA_Screen, *PSD;

typedef struct _CSRegion GRA_ScreenInfo;

typedef struct _GRA_BlitInfo {
	Uint8 *s_pixels;
	int s_width;
	int s_height;
	int s_skip;
	Uint8 *d_pixels;
	int d_width;
	int d_height;
	int d_skip;
	CSGraphPixelFormat *src;
	CSGraphPixelFormat *dst;
	CSGraphPalette *srcpal;
	CSGraphPalette *dstpal;
	Uint8 *table;		/*palette map table */
} GRA_BlitInfo;


typedef struct _GRA_FontInfo {
	char fontname[GRA_MAX_FILENAME_LEN];
	int size;
	int style;
	int height;
	int ascent;
	int descent;
	long face;
	int fixwidth;
	char *facename;
	char *stylename;
} GRA_FontInfo, GRAFONTINFO, *PGRAFONTINFO;

typedef struct _GRA_Image {
	GRA_List list;
	int id;
	GRA_Bitmap *bitmap;
	PSD psd;

} GRA_Image, GRAIMAGEHDR, *PGRAIMAGEHDR;

typedef enum _MW_ICON_TYPE_E
{
	MW_ICONTYPE_TEXT = 0,     //Text
	MW_ICONTYPE_IMAGE = 1,    //Image
	MW_ICONTYPE_IMAGEFILE = 2,//Image from file
	MW_ICONTYPE_RECT = 3 		//Rect
}MW_ICON_TYPE_E;


typedef struct _MW_ICON_TEXT_S
{
	char *pucText; //Text string
	int nTextLength; //Text Length
	int nTextSize;  //Text Size
	int nTextColor; //Text Color
}MW_ICON_TEXT_S;

typedef struct _MW_ICON_IMAGE_S
{
	char *pucBuffer;  //Image Data Buffer
	int nBufferSize;  //Image buffer length
	int nPicWidth;    //Picture width
	int nPicHeight;   //Picture height
	int nDstWidth;    //Dest Area width
	int nDstheight;   //Dest Area height
}MW_ICON_IMAGE_S;

typedef struct _MW_ICON_IMAGEFILE_S
{
	char *pucFileName;  //Image file name
	int nPicWidth;    	//Picture width
	int nPicHeight;   	//Picture height
	int nDstWidth;    	//Dest Area width
	int nDstheight;   	//Dest Area height
}MW_ICON_IMAGEFILE_S;


typedef struct _MW_ICON_RECT_S
{
	int nRectWidth;    //Rect width
	int nRectHeight;   //Rect height
	int nRectColor;    //Rect Fill Color
	BOOL bFillFlag;    //true fill, false not fill
}MW_ICON_RECT_S;

typedef struct _MW_ICON_S
{
	int nX;   //Dest Rect X
	int nY;   //Dest Rect Y
	MW_ICON_TYPE_E eIconType; //Icon Type
	union
	{
		MW_ICON_TEXT_S sText;   			//Text type ICON
		MW_ICON_IMAGE_S sImage; 			//Image type ICON
		MW_ICON_IMAGEFILE_S sImageFile; 	//Image type ICON create from file
		MW_ICON_RECT_S sRect;   			//Rect type ICON
	}
	uIconMemo;
}MW_ICON_S;

/** Define the function's return value */
typedef enum _MW_ICONBASE_RETURNTYPE_E
{
	MW_ICONBASE_RETURN_OK = 0,
	MW_ICONBASE_RETURN_ERROR = -1
} MW_ICONBASE_RETURNTYPE_E;


typedef struct _GRA_Font GRA_Font;
typedef struct _GRA_FontProcs GRA_FontProcs;
typedef struct _GRA_Timer GRATIMER;
typedef unsigned long GRATIMEOUT;

struct _GRA_FontProcs {
	int (*CreateFont) (GRA_Font * font, const char *file, int ptsize, long index);
      	int (*CreateFontFromMem) (GRA_Font * font, const char *file, int ptsize, long index,unsigned char *fontdata,int dsize);
	int (*GetFontInfo) (GRA_Font * font, GRA_FontInfo * fontinfo);
	int (*GetTextSize) (GRA_Font * font, const void *str, Uint16 count, int *w, int *h, long codeflag);
	int (*DrawText) (PSD psd, GRA_Font * font, int x, int y, const void *text, int count, long codeflag, CSGraphGC * gc);
        int (*DrawTextEx) (PSD psd,GRA_Bitmap*bitmap, GRA_Font * font, int x, int y, const void *text, int count, long codeflag, CSGraphGC *gc);
	int (*SetFontSize) (GRA_Font * font, Uint8 fontsize);
	int (*SetFontAttr) (GRA_Font * font, int setflags);
	void (*DestroyFont) (GRA_Font * font);
	
/*******************************************************************************/
	int (*DrawText2)(PSD psd, GRA_Font * font, CSGraphPoint *charPos, const void * text, 
							int count, long codeflag, CSGraphGC * gc);
/*******************************************************************************/

};

struct _GRA_Font {
	GRA_List list;
	int id;
	int codeflag;
	Uint16 size;
	char name[GRA_MAX_FILENAME_LEN];
	GRA_FontProcs *fontproc;
	void *font;		/*for separate font engine */
	int nFamilyType;
	long lFaceIndex;
};

extern GRA_ListHead display_head;
extern GRA_ListHead font_head;
extern GRA_ListHead image_head;
extern GRA_ListHead timer_head;

#ifdef __cplusplus
}
#endif

#endif /*_MWTYPES_H*/
