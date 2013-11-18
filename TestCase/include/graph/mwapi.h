#ifndef	_MWAPI_H_
#define	_MWAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <mwtypes.h>
#include <mwinput.h>
#include <pthread.h>
#include <cs_strings.h>
#include "mwfont.h"

#define  DEFAULT_SCREEN_WIDTH 720
#define DEFAULT_SCREEN_HEIGHT 576

#define GRA_DEFAULT_GC_ID	0
#define GRA_DEFAULT_FONT_ID	gr_default_font_id
#define _NANOX_DEFAULT_FONT_FILE "fonts/default.fnt"

#ifdef PLATFORM_X86
#define GRA_FONTS_CONFIG_FILE "fonts.info"
#else
#define GRA_FONTS_CONFIG_FILE "/flash/fonts/fonts.info"	
#endif

#define GRA_INVALID_GC_ID -1
#define GRA_INVALID_FONT_ID -1
#define GRA_INVALID_CURSOR_ID	-1
#define GRA_INVALID_IMAGE_ID	-1
#define GRA_INVALID_REGION_ID	-1
#define GRA_INVALID_WINDOW_ID	-1
#define GRA_INVALID_TIMER_ID -1


#define GRA_DISPLAYOPTION_COLOR 0
#define GRA_DISPLAYOPTION_3D 1

//定义为14，根据java中使用的个数来设置的
typedef enum{
    E_DEFAULT_CURSOR = 0,
    E_CROSSHAIR_CURSOR,
    E_TEXT_CURSOR,
    E_WAIT_CURSOR,
    E_SW_RESIZE_CURSOR,
    E_SE_RESIZE_CURSOR,
    E_NW_RESIZE_CURSOR,
    E_NE_RESIZE_CURSOR,
    E_N_RESIZE_CURSOR,
    E_S_RESIZE_CURSOR,
    E_W_RESIZE_CURSOR,
    E_E_RESIZE_CURSOR,
    E_HAND_CURSOR,
    E_MOVE_CURSOR,
    E_MAX_COUNT_CUROSR
}GR_CURSOR_TYPE;

#define	GRAMAX_CURSOR_SIZE 32	
  typedef UINT GRACOLORVAL;
  typedef GRACOLORVAL GRAPIXELVAL;
  typedef struct {
    int		width;		
    int		height;		
    int		hotx;	
    int		hoty;	
	GRA_Bitmap *psCursorBitmap;  //存储画Icon之前当前窗口上该区域的数据, 如果不用, 则为NULL
	int id;
  } GRACURSOR, *PGRACURSOR;

#define SYSFONTSNUMS	4
  typedef struct _FontFace{
    char facename[128];	
    Uint16 faceindex;	
    char filename[256];	
    Uint16 facecounts;	
    Uint16 filecounts;	
    Uint32 size;
    unsigned char *fontdata;
  }Gr_FontFace;
  typedef struct _FontFaceList {
    Gr_FontFace element;
    struct _FontFaceList *next;
  }Gr_FontFaceList;

  typedef MW_ICON_TYPE_E GR_ICON_TYPE_E;
  typedef MW_ICON_TEXT_S GR_ICON_TEXT_S;
  typedef MW_ICON_IMAGE_S GR_ICON_IMAGE_S;
  typedef MW_ICON_IMAGEFILE_S GR_ICON_IMAGEFILE_S;
  typedef MW_ICON_RECT_S GR_ICON_RECT_S;
  typedef MW_ICON_S GR_ICON_S;
  typedef MW_ICONBASE_RETURNTYPE_E GR_ICONBASE_RETURNTYPE_E;
  

  typedef unsigned short	GRAIMAGEBITS;	/* bitmap image unit size*/

  typedef SHORT GR_COORD;	/* coordinate value */
  typedef USHORT GR_SIZE;	/* size value */
  typedef UINT GR_COLOR;	/* full color value */
  typedef GRAPIXELVAL GR_PIXELVAL;	/* hw pixel value */
  typedef GRAIMAGEBITS GR_BITMAP;	/* bitmap unit */
  typedef UCHAR GR_CHAR;	/* filename, window title */
  typedef GRAKEY GR_KEY;	/* keystroke value */
  typedef GRASCANCODE GR_SCANCODE;	/* oem keystroke scancode value */
  typedef GRAKEYMOD GR_KEYMOD;	/* keystroke modifiers */
  typedef GRA_ScreenInfo GRASCREENINFO, *PGRASCREENINFO;
  typedef GRASCREENINFO GR_SCREEN_INFO;	/* screen information */
  typedef GRAFONTINFO GR_FONT_INFO;	/* font information */
  typedef MWIMAGEINFO GR_IMAGE_INFO;	/* image information */
  typedef MWIMAGEHDR GR_IMAGE_HDR;	/* multicolor image representation */
  typedef CSGraphPalette GR_PALENTRY;	/* palette entry */
  typedef CSGraphPoint GR_POINT;	/* definition of a point */
  typedef GRATIMEOUT GR_TIMEOUT;	/* timeout value */
    

  typedef int GR_COUNT;	/* number of items */
  typedef unsigned char GR_CHAR_WIDTH;	/* width of character */
  typedef INT GR_ID;	/* resource ids */
  typedef GR_ID GR_DRAW_ID;	/* drawable id */
  typedef GR_DRAW_ID GR_WINDOW_ID;	/* window or pixmap id */
  typedef GR_ID GR_GC_ID;	/* graphics context id */
  typedef GR_ID GR_REGION_ID;	/* region id */
  typedef GR_ID GR_FONT_ID;	/* font id */
  typedef GR_ID GR_IMAGE_ID;	/* image id */
  typedef GR_ID GR_TIMER_ID;	/* timer id */
  typedef GR_ID GR_CURSOR_ID;	/* cursor id */
  typedef unsigned short GR_BOOL;	/* boolean value */
  typedef int GR_ERROR;	/* error types */
  typedef int GR_EVENT_TYPE;	/* event types */
  typedef int GR_UPDATE_TYPE;	/* window update types */
  typedef unsigned long GR_EVENT_MASK;	/* event masks */
  typedef char GR_FUNC_NAME[25];	/* function name */
  typedef unsigned long GR_WM_PROPS;	/* window property flags */
  typedef unsigned long GR_SERIALNO;	/* Selection request ID number */
  typedef unsigned short GR_MIMETYPE;	/* Index into mime type list */
  typedef unsigned long GR_LENGTH;	/* Length of a block of data */
  typedef unsigned int GR_BUTTON;	/* mouse button value */


  typedef struct {
    GR_COORD x;
    GR_COORD y;
    GR_SIZE width;
    GR_SIZE height;
  } GR_RECT;

  /* The root window id. */
#define	GR_ROOT_WINDOW_ID	((GR_WINDOW_ID) 1)


#define GRARGB(r,g,b) GRARGBA(r,g,b,0xff)
#define GRARGBA(r,g,b,a)	((GR_COLOR)(((unsigned long)a<<24)|(((unsigned char)(b)|\
				((unsigned short)((unsigned char)(g))<<8))|\
				(((unsigned long)(unsigned char)(r))<<16))))

#define GRAF_PALINDEX	0x01000000
#define GRAPALINDEX(x)	((GR_COLOR)GRAF_PALINDEX | (x))

/* GR_COLOR color constructor*/
#define GR_RGB(r,g,b)		GRARGB(r,g,b)
#define GR_RGBA(r,g,b,a)  GRARGBA(r,g,b,a)

/* Drawing modes for GrSetGCMode*/
#define	GR_MODE_COPY		CS_MODE_COPY/* src */
#define	GR_MODE_SET		CS_MODE_SET/* obsolete, use GR_MODE_COPY */
#define	GR_MODE_XOR		CS_MODE_XOR/* src ^ dst */
#define	GR_MODE_OR		CS_MODE_OR	/* src | dst */
#define	GR_MODE_AND		CS_MODE_AND	/* src & dst */
#define	GR_MODE_CLEAR 		CS_MODE_CLEAR	/* 0 */
#define	GR_MODE_SETTO1		CS_MODE_SET		/* 11111111*/	/* will be GR_MODE_SET */
#define	GR_MODE_EQUIV		CS_MODE_EQUIV	/* ~(src ^ dst) */
#define	GR_MODE_NOR		CS_MODE_NOR	/* ~(src | dst) */
#define	GR_MODE_NAND		CS_MODE_NAND	/* ~(src & dst) */
#define	GR_MODE_INVERT		CS_MODE_INVERT	/* ~dst */
#define	GR_MODE_COPYINVERTED	CS_MODE_COPYINVERTED	/* ~src */
#define	GR_MODE_ORINVERTED	CS_MODE_ORINVERTED	/* ~src | dst */
#define	GR_MODE_ANDINVERTED	CS_MODE_ANDINVERTED	/* ~src & dst */
#define 	GR_MODE_ORREVERSE	CS_MODE_ORREVERSE	/* src | ~dst */
#define	GR_MODE_ANDREVERSE	CS_MODE_ANDREVERSE	/* src & ~dst */
#define	GR_MODE_NOOP		CS_MODE_NOOP	/* dst */
#define 	GR_MODE_ALPHA		CS_MODE_ALPHA	/* alpha */

#define GR_MODE_DRAWMASK	0x00FF
#define GR_MODE_EXCLUDECHILDREN	0x0100	/* exclude children on clip */

/* builtin font std names*/
#define GRAFONT_SYSTEM_VAR	"System"	/* winSystem 14x16 (ansi)*/
#define GRAFONT_GUI_VAR		"Helvetica"	/* winMSSansSerif 11x13 (ansi)*/
#define GRAFONT_OEM_FIXED	"Terminal"	/* rom8x16 (oem)*/
#define GRAFONT_SYSTEM_FIXED	"SystemFixed"	/* X6x13 (should be ansi)*/

				    /*	lurenfu, add two ttf fonts	*/
#define	GRAFONT_SIMKAI		"simkai.ttf"
#define	GRAFONT_STSONG		"stsong.ttf"

/* encoding */
				    /* Text/GetTextSize encoding flags*/
#define GRATF_ASCII	0x0000	/* 8 bit packing, ascii*/
#define GRATF_UTF8	0x0001	/* 8 bit packing, utf8*/
#define GRATF_UC16	0x0002	/* 16 bit packing, unicode 16*/
#define GRATF_UC32	0x0004	/* 32 bit packing, unicode 32*/
#define GRATF_PACKMASK	0x0007	/* packing mask*/
#define	GRATF_GB2312	GRATF_ASCII

				    /* Text alignment flags*/
#define GRATF_TOP	0x0010	/* align on top*/
#define GRATF_BASELINE	0x0020	/* align on baseline*/
#define GRATF_BOTTOM	0x0040	/* align on bottom*/

				    /* SetFontAttr flags*/
#define GRATF_KERNING	0x1000	/* font kerning*/
#define GRATF_ANTIALIAS	0x2000	/* antialiased output*/
#define GRATF_UNDERLINE	0x4000	/* draw underline*/

#define	GRATF_ITALIC	0x0100
#define	GRATF_BOLD	0x0200


/* builtin font std names*/
#define GR_FONT_SYSTEM_VAR	GRAFONT_SYSTEM_VAR
#define GR_FONT_GUI_VAR		GRAFONT_GUI_VAR
#define GR_FONT_OEM_FIXED	GRAFONT_OEM_FIXED
#define GR_FONT_SYSTEM_FIXED	GRAFONT_SYSTEM_FIXED

/* lurenfu, add two font enum */
#define	GR_FONT_SIMKAI		GRAFONT_SIMKAI
#define	GR_FONT_STSONG		GRAFONT_STSONG

/* GrText/GrGetTextSize encoding flags*/
#define GR_TFASCII		GRA_FONT_CODE_ASCII
#define GR_TFUTF8		GRA_FONT_CODE_UTF8
#define GR_TFUC16		GRA_FONT_CODE_UNICODE
#define GR_TFPACKMASK		GRA_FONT_CODE_MASK

/* GrText alignment flags*/
#define GR_TFTOP		GRATF_TOP
#define GR_TFBASELINE		GRATF_BASELINE
#define GR_TFBOTTOM		GRATF_BOTTOM

/* GrSetFontAttr flags*/
#define GR_TFKERNING		GRATF_KERNING
#define GR_TFANTIALIAS		GRATF_ANTIALIAS

/* add two font style */
#define	GR_TFITALIC		GRA_STYLE_ITALIC
#define	GR_TFBOLD			GRA_STYLE_BOLD 
#define	GR_TFUNDERLINE 	GRA_STYLE_UNDERLINE
#define	GR_TFNORMAL		GRA_STYLE_NORMAL

/*
 * outline and filled arc and pie types
  */
#define GRAARC		0x0001	/* arc*/
#define GRAOUTLINE	0x0002
#define GRAARCOUTLINE	/*0x0004*/0x0003	/* arc + outline*/
#define GRAPIE		0x0008	/* pie (filled)*/
#define GRAELLIPSE	0x0010	/* ellipse outline*/
#define GRAELLIPSEFILL	0x0020	/* ellipse filled*/
#define GRAFILLARC	0x0100

/* GrArc, GrArcAngle types*/
#define GR_ARC		GRAARC	/* arc only 0x0001*/
#define GR_ARCOUTLINE	GRAARCOUTLINE	/* arc + outline 0x0003*/
#define GR_PIE		GRAPIE	/* pie (filled) 0x0004*/
#define GR_FILL_ARC	GRAFILLARC /*0x0100*/

/* Booleans */
#define	GR_FALSE		0
#define	GR_TRUE			1

/* Loadable Image support definition */
#define GR_IMAGE_MAX_SIZE	(-1)

				    /* Button flags */
#define	GR_BUTTON_R	GRABUTTON_R	/* right button */
#define	GR_BUTTON_M	GRABUTTON_M	/* middle button */
#define	GR_BUTTON_L	GRABUTTON_L	/* left button */
#define	GR_BUTTON_ANY	(GRABUTTON_R|GRABUTTON_M|GRABUTTON_L)	/* any */

				    /* GrSetBackgroundPixmap flags */
#define GR_BACKGROUND_TILE	0	/* Tile across the window */
#define GR_BACKGROUND_CENTER	1	/* Draw in center of window */
#define GR_BACKGROUND_TOPLEFT	2	/* Draw at top left of window */
#define GR_BACKGROUND_STRETCH	4	/* Stretch image to fit window */
#define GR_BACKGROUND_TRANS	8	/* Don't fill in gaps */

				    /* GrNewPixmapFromData flags*/
#define GR_BMDATA_BYTEREVERSE	01	/* byte-reverse bitmap data */
#define GR_BMDATA_BYTESWAP	02	/* byte-swap bitmap data */

/* Window properties*/
#define GR_WM_PROPS_NOBACKGROUND 0x00000001L	/* window background */
#define GR_WM_PROPS_NOFOCUS	 0x00000002L	/* focus to this window */
#define GR_WM_PROPS_NOMOVE	 0x00000004L	/* user move window */
#define GR_WM_PROPS_NORAISE	 0x00000008L	/* user raise window */
#define GR_WM_PROPS_NODECORATE	 0x00000010L	/* redecorate window */
#define GR_WM_PROPS_NOAUTOMOVE	 0x00000020L	/* move window on 1st map */
#define GR_WM_PROPS_NOAUTORESIZE 0x00000040L	/* resize window on 1st map */

/* default decoration style*/
#define GR_WM_PROPS_APPWINDOW	0x00000000L	/* Leave appearance to WM */
#define GR_WM_PROPS_APPMASK	0xF0000000L	/* Appearance mask */
#define GR_WM_PROPS_BORDER	0x80000000L	/* Single line border */
#define GR_WM_PROPS_APPFRAME	0x40000000L	/* 3D app frame */
#define GR_WM_PROPS_CAPTION	0x20000000L	/* Title bar */
#define GR_WM_PROPS_CLOSEBOX	0x10000000L	/* Close box */
#define GR_WM_PROPS_MAXIMIZE	0x08000000L	/* Application is maximized */

/* Flags for indicating valid bits in GrSetWMProperties call*/
#define GR_WM_FLAGS_PROPS	0x0001	/* Properties */
#define GR_WM_FLAGS_TITLE	0x0002	/* Title */
#define GR_WM_FLAGS_BACKGROUND	0x0004	/* Background color */
#define GR_WM_FLAGS_BORDERSIZE	0x0008	/* Border size */
#define GR_WM_FLAGS_BORDERCOLOR	0x0010	/* Border color */


 typedef struct {
  GR_WM_PROPS flags;	/* Which properties valid in struct for set */
  GR_WM_PROPS props;	/* Window property bits */
  GR_CHAR *title;		/* Window title */
  GR_COLOR background;	/* Window background color */
  GR_SIZE bordersize;	/* Window border size */
  GR_COLOR bordercolor;	/* Window border color */
} GR_WM_PROPERTIES;


typedef struct {
  GR_WINDOW_ID wid;	/* window id (or 0 if no such window) */
  GR_WINDOW_ID parent;	/* parent window id */
  GR_WINDOW_ID child;	/* first child window id (or 0) */
  GR_WINDOW_ID sibling;	/* next sibling window id (or 0) */
  GR_BOOL inputonly;	/* TRUE if window is input only */
  GR_BOOL mapped;		/* TRUE if window is mapped */
  GR_COUNT unmapcount;	/* reasons why window is unmapped */
  GR_COORD x;		/* absolute x position of window */
  GR_COORD y;		/* absolute y position of window */
  GR_SIZE width;		/* width of window */
  GR_SIZE height;		/* height of window */
  GR_SIZE bordersize;	/* size of border */
  GR_COLOR bordercolor;	/* color of border */
  GR_COLOR background;	/* background color */
  GR_EVENT_MASK eventmask;	/* current event mask for this client */
  GR_WM_PROPS props;	/* window properties */
  GR_CURSOR_ID cursor;	/* cursor id */
  pthread_t threadid;	/* thread id of owner */
  CSGraphRegion *pregion;  /* region pointer which the window belongs to */
  CSGraphSurface *psurface; /* surface pointer which the window belongs to */
} GR_WINDOW_INFO;

typedef struct {
  CSGraphGC mwgc;
  GR_REGION_ID	regionid;	/* current clipping region */
  int             xoff;           /* X offset for the clip region */
  int             yoff;           /* Y offset for the clip region */
  GR_BOOL  usebackground;	/* actually display the background */
} GR_GC_INFO;
				    
/* color palette*/
typedef GR_PALENTRY GR_PALETTE;
   	

/* Error codes */
#define	GR_ERROR_BAD_WINDOW_ID		1
#define	GR_ERROR_BAD_GC_ID		2
#define	GR_ERROR_BAD_CURSOR_SIZE	3
#define	GR_ERROR_MALLOC_FAILED		4
#define	GR_ERROR_BAD_WINDOW_SIZE	5
#define	GR_ERROR_KEYBOARD_ERROR		6
#define	GR_ERROR_MOUSE_ERROR		7
#define	GR_ERROR_INPUT_ONLY_WINDOW	8
#define	GR_ERROR_ILLEGAL_ON_ROOT_WINDOW	9
#define	GR_ERROR_TOO_MUCH_CLIPPING	10
#define	GR_ERROR_SCREEN_ERROR		11
#define	GR_ERROR_UNMAPPED_FOCUS_WINDOW	12
#define	GR_ERROR_BAD_DRAWING_MODE	13
#define GR_ERROR_BAD_CLIENT_THREAD      14
#define GR_ERROR_BAD_WINDOW_POSITION    15

/* Event types. */
#define	GR_EVENT_TYPE_ERROR		(-1)
#define	GR_EVENT_TYPE_NONE		0
#define	GR_EVENT_TYPE_EXPOSURE		1
#define	GR_EVENT_TYPE_BUTTON_DOWN	2
#define	GR_EVENT_TYPE_BUTTON_UP		3
#define	GR_EVENT_TYPE_MOUSE_ENTER	4
#define	GR_EVENT_TYPE_MOUSE_EXIT	5
#define	GR_EVENT_TYPE_MOUSE_MOTION	6
#define	GR_EVENT_TYPE_MOUSE_POSITION	7
#define	GR_EVENT_TYPE_KEY_DOWN		8
#define	GR_EVENT_TYPE_KEY_UP		9
#define	GR_EVENT_TYPE_FOCUS_IN		10
#define	GR_EVENT_TYPE_FOCUS_OUT		11
#define GR_EVENT_TYPE_FDINPUT		12
#define GR_EVENT_TYPE_UPDATE		13
#define GR_EVENT_TYPE_CHLD_UPDATE	14
#define GR_EVENT_TYPE_CLOSE_REQ		15
#define GR_EVENT_TYPE_TIMEOUT		16
#define GR_EVENT_TYPE_SCREENSAVER	17
#define GR_EVENT_TYPE_CLIENT_DATA_REQ	18
#define GR_EVENT_TYPE_CLIENT_DATA	19
#define GR_EVENT_TYPE_SELECTION_CHANGED 20
#define GR_EVENT_TYPE_TIMER             21
#define GR_EVENT_TYPE_PORTRAIT_CHANGED  22
#define GR_EVENT_TYPE_RESOLUTION_CHANGED    23
#define GR_EVENT_TYPE_MOUSE_WHEEL	24

/* Event masks */
#define	GR_EVENTMASK(n)			(((GR_EVENT_MASK) 1) << (n))

#define	GR_EVENT_MASK_NONE		GR_EVENTMASK(GR_EVENT_TYPE_NONE)
#define	GR_EVENT_MASK_ERROR		0x80000000L
#define	GR_EVENT_MASK_EXPOSURE		GR_EVENTMASK(GR_EVENT_TYPE_EXPOSURE)
#define	GR_EVENT_MASK_BUTTON_DOWN	GR_EVENTMASK(GR_EVENT_TYPE_BUTTON_DOWN)
#define	GR_EVENT_MASK_BUTTON_UP		GR_EVENTMASK(GR_EVENT_TYPE_BUTTON_UP)
#define	GR_EVENT_MASK_MOUSE_ENTER	GR_EVENTMASK(GR_EVENT_TYPE_MOUSE_ENTER)
#define	GR_EVENT_MASK_MOUSE_EXIT	GR_EVENTMASK(GR_EVENT_TYPE_MOUSE_EXIT)
#define	GR_EVENT_MASK_MOUSE_MOTION   GR_EVENTMASK(GR_EVENT_TYPE_MOUSE_MOTION)
#define	GR_EVENT_MASK_MOUSE_POSITION GR_EVENTMASK(GR_EVENT_TYPE_MOUSE_POSITION)
#define	GR_EVENT_MASK_KEY_DOWN		GR_EVENTMASK(GR_EVENT_TYPE_KEY_DOWN)
#define	GR_EVENT_MASK_KEY_UP		GR_EVENTMASK(GR_EVENT_TYPE_KEY_UP)
#define	GR_EVENT_MASK_FOCUS_IN		GR_EVENTMASK(GR_EVENT_TYPE_FOCUS_IN)
#define	GR_EVENT_MASK_FOCUS_OUT		GR_EVENTMASK(GR_EVENT_TYPE_FOCUS_OUT)
#define	GR_EVENT_MASK_FDINPUT		GR_EVENTMASK(GR_EVENT_TYPE_FDINPUT)
#define	GR_EVENT_MASK_UPDATE		GR_EVENTMASK(GR_EVENT_TYPE_UPDATE)
#define	GR_EVENT_MASK_CHLD_UPDATE	GR_EVENTMASK(GR_EVENT_TYPE_CHLD_UPDATE)
#define	GR_EVENT_MASK_CLOSE_REQ		GR_EVENTMASK(GR_EVENT_TYPE_CLOSE_REQ)
#define	GR_EVENT_MASK_TIMEOUT		GR_EVENTMASK(GR_EVENT_TYPE_TIMEOUT)
#define GR_EVENT_MASK_SCREENSAVER	GR_EVENTMASK(GR_EVENT_TYPE_SCREENSAVER)
#define GR_EVENT_MASK_CLIENT_DATA_REQ	GR_EVENTMASK(GR_EVENT_TYPE_CLIENT_DATA_REQ)
#define GR_EVENT_MASK_CLIENT_DATA	GR_EVENTMASK(GR_EVENT_TYPE_CLIENT_DATA)
#define GR_EVENT_MASK_SELECTION_CHANGED GR_EVENTMASK(GR_EVENT_TYPE_SELECTION_CHANGED)
#define GR_EVENT_MASK_TIMER             GR_EVENTMASK(GR_EVENT_TYPE_TIMER)
#define GR_EVENT_MASK_PORTRAIT_CHANGED  GR_EVENTMASK(GR_EVENT_TYPE_PORTRAIT_CHANGED)
#define GR_EVENT_MASK_RESOLUTION_CHANGED  GR_EVENTMASK(GR_EVENT_TYPE_RESOLUTION_CHANGED)
#define GR_EVENT_MASK_MOUSE_WHEEL  GR_EVENTMASK(GR_EVENT_TYPE_MOUSE_WHEEL)
#define	GR_EVENT_MASK_ALL		((GR_EVENT_MASK) -1L)

/* update event types */
#define GR_UPDATE_MAP		1
#define GR_UPDATE_UNMAP		2
#define GR_UPDATE_MOVE		3
#define GR_UPDATE_SIZE		4
#define GR_UPDATE_UNMAPTEMP	5	/* unmap during window move/resize */
#define GR_UPDATE_ACTIVATE	6	/* toplevel window [de]activate */
#define GR_UPDATE_DESTROY	7

/* Event for errors detected by the server. */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_FUNC_NAME name;	/* function name which failed */
  GR_ERROR code;		/* error code */
  GR_ID id;		/* resource id (maybe useless) */
} GR_EVENT_ERROR;
				    
/* Event for a mouse button pressed down or released. */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID wid;	/* window id event delivered to */
  GR_WINDOW_ID subwid;	/* sub-window id (pointer was in) */
  GR_COORD rootx;		/* root window x coordinate */
  GR_COORD rooty;		/* root window y coordinate */
  GR_COORD x;		/* window x coordinate of mouse */
  GR_COORD y;		/* window y coordinate of mouse */
  GR_BUTTON buttons;	/* current state of all buttons */
  GR_BUTTON changebuttons;	/* buttons which went down or up */
  GR_KEYMOD modifiers;	/* modifiers (GRAKMOD_SHIFT, etc) */
  GR_TIMEOUT time;	/* tickcount time value */
} GR_EVENT_BUTTON;
				    
/* Event for a keystroke typed for the window with has focus. */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID wid;	/* window id event delived to */
  GR_WINDOW_ID subwid;	/* sub-window id (pointer was in) */
  GR_COORD rootx;		/* root window x coordinate */
  GR_COORD rooty;		/* root window y coordinate */
  GR_COORD x;		/* window x coordinate of mouse */
  GR_COORD y;		/* window y coordinate of mouse */
  GR_BUTTON buttons;	/* current state of buttons */
  GR_KEYMOD modifiers;	/* modifiers (GRAKMOD_SHIFT, etc) */
  GR_KEY ch;		/* 16-bit unicode key value, GRAKEY_xxx */
  GR_SCANCODE scancode;	/* OEM scancode value if available */
} GR_EVENT_KEYSTROKE;
				    
/* Event for exposure for a region of a window. */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID wid;	/* window id */
  GR_COORD x;		/* window x coordinate of exposure */
  GR_COORD y;		/* window y coordinate of exposure */
  GR_SIZE width;		/* width of exposure */
  GR_SIZE height;		/* height of exposure */
} GR_EVENT_EXPOSURE;
				    
/* General events for focus in or focus out for a window, or mouse enter
 * or mouse exit from a window, or window unmapping or mapping, etc.
 * Server portrait mode changes are also sent using this event to
 * all windows that request it.
 */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID wid;	/* window id */
  GR_WINDOW_ID otherid;	/* new/old focus id for focus events */
} GR_EVENT_GENERAL;
				    
/* Events for mouse motion or mouse position. */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID wid;	/* window id event delivered to */
  GR_WINDOW_ID subwid;	/* sub-window id (pointer was in) */
  GR_COORD rootx;		/* root window x coordinate */
  GR_COORD rooty;		/* root window y coordinate */
  GR_COORD x;		/* window x coordinate of mouse */
  GR_COORD y;		/* window y coordinate of mouse */
  GR_BUTTON buttons;	/* current state of buttons */
  GR_KEYMOD modifiers;	/* modifiers (GRAKMOD_SHIFT, etc) */
} GR_EVENT_MOUSE;
				    
/* GrRegisterInput event*/
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  int fd;			/* input fd */
} GR_EVENT_FDINPUT;
				    
/* GR_EVENT_TYPE_UPDATE */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID wid;	/* select window id */
  GR_WINDOW_ID subwid;	/* update window id (=wid for UPDATE event) */
  GR_COORD x;		/* new window x coordinate */
  GR_COORD y;		/* new window y coordinate */
  GR_SIZE width;		/* new width */
  GR_SIZE height;		/* new height */
  GR_UPDATE_TYPE utype;	/* update_type */
} GR_EVENT_UPDATE;
				    
/* GR_EVENT_TYPE_SCREENSAVER */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_BOOL activate;	/* true = activate, false = deactivate */
} GR_EVENT_SCREENSAVER;
				    
/* GR_EVENT_TYPE_CLIENT_DATA_REQ */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID wid;	/* ID of requested window */
  GR_WINDOW_ID rid;	/* ID of window to send data to */
  GR_SERIALNO serial;	/* Serial number of transaction */
  GR_MIMETYPE mimetype;	/* Type to supply data as */
} GR_EVENT_CLIENT_DATA_REQ;
				    
/* GR_EVENT_TYPE_CLIENT_DATA */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID wid;	/* ID of window data is destined for */
  GR_WINDOW_ID rid;	/* ID of window data is from */
  GR_SERIALNO serial;	/* Serial number of transaction */
  unsigned long len;	/* Total length of data */
  unsigned long datalen;	/* Length of following data */
  void *data;		/* Pointer to data (filled in on client side) */
} GR_EVENT_CLIENT_DATA;
				    
/* GR_EVENT_TYPE_SELECTION_CHANGED */
typedef struct {
  GR_EVENT_TYPE type;	/* event type */
  GR_WINDOW_ID new_owner;	/* ID of new selection owner */
} GR_EVENT_SELECTION_CHANGED;
				    
/* GR_EVENT_TYPE_TIMER */
typedef struct {
  GR_EVENT_TYPE type;	/* event type, GR_EVENT_TYPE_TIMER */
  GR_WINDOW_ID wid;	/* ID of window timer is destined for */
  GR_TIMER_ID tid;	/* ID of expired timer */
} GR_EVENT_TIMER;

/* GR_EVENT_TYPE_RESOLUTION_CHANGED */
typedef struct {
  GR_EVENT_TYPE type;	/* event type, GR_EVENT_TYPE_RESOLUTION_CHANGED */
  int clientid;         /* id of which client changed */
  GR_COORD x;		    /* new client x */
  GR_COORD y;		    /* new client y */
  GR_SIZE width;		/* new width */
  GR_SIZE height;		/* new height */
} GR_EVENT_RESOLUTION_CHANGED;
				    
/*
 * Union of all possible event structures.
 * This is the structure returned by the GrGetNextEvent and similar routines.
 */
typedef union {
  GR_EVENT_TYPE type;			/* event type */
  GR_EVENT_ERROR error;			/* error event */
  GR_EVENT_GENERAL general;		/* general window events */
  GR_EVENT_BUTTON button;		/* button events */
  GR_EVENT_KEYSTROKE keystroke;		/* keystroke events */
  GR_EVENT_EXPOSURE exposure;		/* exposure events */
  GR_EVENT_MOUSE mouse;			/* mouse motion events */
  GR_EVENT_FDINPUT fdinput;		/* fd input events*/
  GR_EVENT_UPDATE update;		/* window update events */
  GR_EVENT_SCREENSAVER screensaver; 	/* Screen saver events */
  GR_EVENT_CLIENT_DATA_REQ clientdatareq; /* Request for client data events */
  GR_EVENT_CLIENT_DATA clientdata;	/* Client data events */
  GR_EVENT_SELECTION_CHANGED selectionchanged; /* Selection owner changed */
  GR_EVENT_TIMER timer;  
  GR_EVENT_RESOLUTION_CHANGED resolutionchanged;
} GR_EVENT;

typedef void (*GR_FNCALLBACKEVENT) (int ,GR_EVENT *);			    


#define GR_TOP_PIXMAP_TOP     0
#define GR_TOP_PIXMAP_BOTTOM   1


/* Pixel packings within words. */
#define	GR_BITMAPBITS	(sizeof(GR_BITMAP) * 8)
#define	GR_ZEROBITS	((GR_BITMAP) 0x0000)
#define	GR_ONEBITS	((GR_BITMAP) 0xffff)
#define	GR_FIRSTBIT	((GR_BITMAP) 0x8000)
#define	GR_LASTBIT	((GR_BITMAP) 0x0001)
#define	GR_BITVALUE(n)	((GR_BITMAP) (((GR_BITMAP) 1) << (n)))
#define	GR_SHIFTBIT(m)	((GR_BITMAP) ((m) << 1))
#define	GR_NEXTBIT(m)	((GR_BITMAP) ((m) >> 1))
#define	GR_TESTBIT(m)	(((m) & GR_FIRSTBIT) != 0)
				    
/* Size of bitmaps. */
#define	GR_BITMAP_SIZE(width, height)	((height) * \
  (((width) + sizeof(GR_BITMAP) * 8 - 1) / (sizeof(GR_BITMAP) * 8)))
				    
#define	GR_MAX_BITMAP_SIZE \
  GR_BITMAP_SIZE(GRAMAX_CURSOR_SIZE, GRAMAX_CURSOR_SIZE)

/* GrGetSysColor colors*/
/* desktop background*/
#define GR_COLOR_DESKTOP           0
				    
/* caption colors*/
#define GR_COLOR_ACTIVECAPTION     1
#define GR_COLOR_ACTIVECAPTIONTEXT 2
#define GR_COLOR_INACTIVECAPTION   3
#define GR_COLOR_INACTIVECAPTIONTEXT 4

/* 3d border shades*/
#define GR_COLOR_WINDOWFRAME       5
#define GR_COLOR_BTNSHADOW         6
#define GR_COLOR_3DLIGHT           7
#define GR_COLOR_BTNHIGHLIGHT      8

/* top level application window backgrounds/text*/
#define GR_COLOR_APPWINDOW         9
#define GR_COLOR_APPTEXT           10

/* button control backgrounds/text (usually same as app window colors)*/
#define GR_COLOR_BTNFACE           11
#define GR_COLOR_BTNTEXT           12

/* edit/listbox control backgrounds/text, selected highlights*/
#define GR_COLOR_WINDOW            13
#define GR_COLOR_WINDOWTEXT        14
#define GR_COLOR_HIGHLIGHT         15
#define GR_COLOR_HIGHLIGHTTEXT     16
#define GR_COLOR_GRAYTEXT          17

/* menu backgrounds/text*/
#define GR_COLOR_MENUTEXT          18
#define GR_COLOR_MENU              19
#define GRAH_PIXEL_FORMAT_32(format)  \
{\
   format.Aloss = 0; \
   format.Bloss = 0; \
   format.Gloss = 0; \
   format.Rloss = 0; \
   format.Rshift = 0; \
   format.Gshift = 8; \
   format.Bshift = 16; \
   format.Ashift = 24;\
   format.Rmask = 0xff; \
   format.Amask = 0xff000000; \
   format.Gmask = 0x0000ff00; \
   format.Bmask = 0x00ff0000; \
   format.BitsPerPixel  = 32; \
   format.BytesPerPixel = 4;\
}
#define GRAH_PIXEL_FORMA_24(format)  \
{\
   format.Aloss = 8; \
   format.Bloss = 0; \
   format.Gloss = 0; \
   format.Rloss = 0; \
   format.Rshift = 0; \
   format.Gshift = 8; \
   format.Bshift = 16; \
   format.Ashift = 0;\
   format.Rmask = 0xff; \
   format.Amask = 0; \
   format.Gmask = 0x0000ff00; \
   format.Bmask = 0x00ff0000; \
   format.BitsPerPixel  = 24; \
   format.BytesPerPixel = 3;\
}

#define GRAH_PIXEL_FORMA_8(format)  \
{\
   format.Aloss = 8; \
   format.Bloss = 8; \
   format.Gloss = 8; \
   format.Rloss = 8; \
   format.Rshift = 0; \
   format.Gshift = 0; \
   format.Bshift = 0; \
   format.Ashift = 0;\
   format.Rmask = 0; \
   format.Amask = 0; \
   format.Gmask = 0; \
   format.Bmask = 0; \
   format.BitsPerPixel  = 8; \
   format.BytesPerPixel = 1;\
}


/* Error strings per error number*/
#define GR_ERROR_STRINGS		\
	"",				\
	"Bad window id: %d\n",		\
	"Bad graphics context: %d\n",	\
	"Bad cursor size\n",		\
	"Out of server memory\n",	\
	"Bad window size: %d\n",	\
	"Keyboard error\n",		\
	"Mouse error\n",		\
	"Input only window: %d\n",	\
	"Illegal on root window: %d\n",	\
	"Clipping overflow\n",		\
	"Screen error\n",		\
	"Unmapped focus window: %d\n",	\
	"Bad drawing mode gc: %d\n"
				    
extern char *nxErrorStrings[];
				    
/* Public graphics routines. */
/*srvmain.c*/
void 	GrFlush(void);
int 	GrOpen(void);
int GrReOpen(int clientid, GR_COORD x, GR_COORD y, GR_SIZE w, GR_SIZE h);
void 	GrClose(int client_id);
void 	GrDelay(GR_TIMEOUT msecs);
extern int GsInitialize(void);
extern int GrOpenExt(GR_COORD x,GR_COORD y,
		     GR_SIZE w,GR_SIZE h, 
		     CSGraphPixelFormat *format, 
		     GR_BOOL flag,GR_RECT *retRect);

/**
@brief  功能描述:
		获取client id
 
@param[in]
		无
 
@param[out] 
		无
 
@return
 		如果已经创建过client，则返回第一个client的id，否则返回-1
@exception
@note
*/
int GrGetUsedClientId(void);

/**
@brief  功能描述:
		注册当前pixmap为置顶pixmap
 
@param[in]  
                     client_id : clientid。
                     pixmapId :   要注册的pixmap的ID值
                     x              :置顶pixmap在整个屏幕中显示位值的X坐标
                     y              :置顶pixmap在整个屏幕中显示位值的X坐标
                     pixmapZOrder:当前pixmap显示的z方向即置顶或者置后(目前暂时未用到)，该值可取GR_TOP_PIXMAP_TOP，GR_TOP_PIXMAP_BOTTOM
@param[out] 
		无
 
@return
 		成功返回0，否则返回-1
@exception
@note
*/

int GrRegistTopPixmap(int client_id, GR_WINDOW_ID pixmapId, int x, int y, int  pixmapZOrder);
/**
@brief  功能描述:
		注销掉当前置顶pixmap
 
@param[in]  
                     client_id : clientid。
                     pixmapId :   要注销的pixmap的ID值
                    
@param[out] 
		无
 
@return
 		成功返回0，否则返回-1
@exception
@note
*/
int  GrUnRegistTopPixmap(int client_id, GR_WINDOW_ID pixmapId);
int GrRegisterFont(char *fontfile); 
int GrUnregisterFont(char *facename,char *fontfile);

//全局特效的函数指针
typedef void (*CSEFTExcuteEftCallback)(int client_id,  GR_WINDOW_ID wid,  GR_GC_ID gc, GR_WINDOW_INFO info, int m_prepixmap, int m_curpixmap);
typedef int  (*CSEFTGetGlobalTypeCallback)();

/**
@brief 注册全局特效函数

@param[in] nType : 注册的函数类型。
@param[in] pfnCallback :       注册的函数指针。

@return 成功返回0
@note   
*/
int GrRegisterGlobalEftFun(int nFunType, void * pfnCallback);

/**
@brief 清除全局特效的标志

@param[in] client_id : clientid。
@param[in] m_prepixmap :   上一个应用的pixmap。
@param[in] m_curpixmap :   当前应用的pixmap。

@return 成功返回0
@note   无
*/
int GrReSetGlobalEftFlag(int client_id, int m_prepixmap, int m_curpixmap);

/**
@brief 设置全局特效的启动标志

@param[in] nFlag : 保留

@return 成功返回0
@note   无
*/
int GrSetGlobalEftFlag(int nFlag);

extern UINT GsGetStartPictureID(int *picId);

/*srvfunc.c*/    
void 		GrGetScreenInfo(int client_id,GR_SCREEN_INFO * sip);
/**
 * Set antiflickering flag
 */
extern void GrSetAntiFlickering(int client_id, GR_WINDOW_ID wid, int anti);

void GrSetDoubleBuffer(int client_id, GR_WINDOW_ID wid, int doubleBuffer,
		       int x, int y, int w, int h);
				    
/*window APIs*/
GR_WINDOW_ID 	GrNewWindow(int client_id,GR_WINDOW_ID parent, GR_COORD x, 
			    GR_COORD y,
			    GR_SIZE width, GR_SIZE height, GR_SIZE bordersize, 
			    GR_COLOR background, GR_COLOR bordercolor);
GR_WINDOW_ID 	GrNewPixmap(int client_id,GR_SIZE width, GR_SIZE height, 
			    void *addr);
GR_WINDOW_ID 	GrNewInputWindow(int client_id,GR_WINDOW_ID parent, GR_COORD x,
				 GR_COORD y, GR_SIZE width, GR_SIZE height);
void 		GrDestroyWindow(int client_id,GR_WINDOW_ID wid);
void GrCloseWindow(int client_id,GR_WINDOW_ID wid);
void GrKillWindow(int client_id,GR_WINDOW_ID wid);
void GrMapWindow(int client_id,GR_WINDOW_ID wid);
void GrUnmapWindow(int client_id,GR_WINDOW_ID wid);
void GrRaiseWindow(int client_id,GR_WINDOW_ID wid);
void GrLowerWindow(int client_id,GR_WINDOW_ID wid);
void GrMoveWindow(int client_id,GR_WINDOW_ID wid, GR_COORD x, GR_COORD y);
void GrResizeWindow(int client_id,GR_WINDOW_ID wid, 
		    GR_SIZE width, GR_SIZE height);
void GrReparentWindow(int client_id,GR_WINDOW_ID wid, 
		      GR_WINDOW_ID pwid, GR_COORD x, GR_COORD y);
void GrGetWindowInfo(int client_id,GR_WINDOW_ID wid, 
		     GR_WINDOW_INFO * infoptr);
void GrSetWMProperties(int client_id,GR_WINDOW_ID wid, 
		       GR_WM_PROPERTIES * props);
GR_WINDOW_ID GrGetFocus(int client_id);
void GrSetFocus(int client_id,GR_WINDOW_ID wid);
void GrSetAlwaysOnTopWindow(int client_id,GR_WINDOW_ID wid,GR_BOOL flag);

/*GC APIs*/
GR_GC_ID GrNewGC(int client_id);
GR_GC_ID GrCopyGC(int client_id,GR_GC_ID gc);
void GrGetGCInfo(int client_id,GR_GC_ID gc, GR_GC_INFO * gcip);
void GrSetGCRegion(int client_id,GR_GC_ID gc, GR_REGION_ID region);
void GrSetGCClipOrigin(int client_id,GR_GC_ID gc, int x, int y);
void GrDestroyGC(int client_id,GR_GC_ID gc);
void GrSetGCForeground(int client_id,GR_GC_ID gc, GR_COLOR foreground);
void GrSetGCBackground(int client_id,GR_GC_ID gc, GR_COLOR background);
void GrSetGCUseBackground(int client_id,GR_GC_ID gc, GR_BOOL flag);
void GrSetGCMode(int client_id,GR_GC_ID gc, int mode);

/**
@brief  功能描述:		
		设置GC的字体

@param[in] 
		int client_id   client句柄 

@param[in] 
		GR_GC_ID gc 绘图上下文句柄

@param[in] 
		GR_FONT_ID font 字体句柄

@return 		
		无	

@exception
@note
		只有调用了该接口，创建的字体句柄才会在绘图时有效
*/
void GrSetGCFont(int client_id,GR_GC_ID gc, GR_FONT_ID font);

/**
@brief  功能描述:		
		获取字符串显示像素宽度

@param[in] 
		int client_id   client句柄 

@param[in] 
		GR_GC_ID gc 绘图上下文句柄

@param[in] 
		void *str 显示的字符串内容缓冲区

@param[in] 
		int count 显示的字符串个数

@param[in] 
		int flags 字符串的编码方式，主要取值为：
		GRA_FONT_CODE_ASCII      0x01
GRA_FONT_CODE_UTF8       0x02
GRA_FONT_CODE_UNICODE    0x04

@param[out] 		
int *retwidth 显示的字符串像素宽度 

@param[out] 		
int *retheight 显示的字符串像素高度度

@return 		
		无	

@exception
@note
*/
void GrGetGCTextSize(int client_id,GR_GC_ID gc, void *str, int count, 
		     int flags, int* retwidth, int* retheight);

/*new functions for gc attribute set*/
void GrSetGCPixmapMask(int client_id,GR_GC_ID gc, CSGraphPixmap *pixmap_mask);
void GrSetGCCapStyle(int client_id,GR_GC_ID gc, UCHAR cap_style);
void GrSetGCDash(int client_id,GR_GC_ID gc, UINT dash_length, UCHAR * dash_format);
void GrSetGCJoinStyle(int client_id,GR_GC_ID gc, UCHAR join_style);
void GrSetGCPenStyle(int client_id,GR_GC_ID gc, UINT pen_style);
void GrSetGCPenWidth(int client_id,GR_GC_ID gc, UINT pen_width);
void GrSetGCPFillStyle(int client_id,GR_GC_ID gc, UCHAR poly_fill_style);
void GrSetGCArcStyle(int client_id,GR_GC_ID gc, UCHAR arc_style);
void GrSetGCPlaneMask(int client_id,GR_GC_ID gc, UINT plane_mask);
void GrSetGCBlendingFactor(int client_id,GR_GC_ID gc, UCHAR blending_factor);

/*region APIs*/
GR_REGION_ID GrNewRegion(int client_id);
GR_REGION_ID GrNewPolygonRegion(int client_id,int mode, GR_COUNT count, GR_POINT * points);
GR_BOOL GrPointInRegion(int client_id,GR_REGION_ID region, GR_COORD x, GR_COORD y);
void GrDestroyRegion(int client_id,GR_REGION_ID region);
void GrUnionRectWithRegion(int client_id,GR_REGION_ID region, GR_RECT * rect);
void GrSetRectWithRegion(int client_id, GR_REGION_ID region, GR_RECT * rect);
void GrUnionRegion(int client_id,GR_REGION_ID dst_rgn, GR_REGION_ID src_rgn1, GR_REGION_ID src_rgn2);
void GrIntersectRegion(int client_id,GR_REGION_ID dst_rgn, GR_REGION_ID src_rgn1, GR_REGION_ID src_rgn2);
void GrSubtractRegion(int client_id,GR_REGION_ID dst_rgn, GR_REGION_ID src_rgn1, GR_REGION_ID src_rgn2);
void GrXorRegion(int client_id,GR_REGION_ID dst_rgn, GR_REGION_ID src_rgn1, GR_REGION_ID src_rgn2);
int GrRectInRegion(int client_id,GR_REGION_ID region, GR_COORD x, GR_COORD y, GR_COORD w, GR_COORD h);
GR_BOOL GrEmptyRegion(int client_id,GR_REGION_ID region);
GR_BOOL GrEqualRegion(int client_id,GR_REGION_ID rgn1, GR_REGION_ID rgn2);
void GrOffsetRegion(int client_id,GR_REGION_ID region, GR_SIZE dx, GR_SIZE dy);
int GrGetRegionBox(int client_id,GR_REGION_ID region, GR_RECT * rect);

/*font APIs*/
/**
@brief  功能描述:		
		创建字体

@param[in] 
		int client_id   client句柄 

@param[in] 
		GR_CHAR * facename 创建的字体名称，传入NULL，则使用默认"default"名称，所有的字体句柄都需要使用到该参数，作为区分各种字体的唯一标识

@param[in] 
		GR_COORD height 创建的字体大小，取值范围为5~100

@param[in] 
		char *fontfile 字库文件路径字符串，不能为NULL，在facename 未找到的情况下，使用该变量来制定创建的字库句柄的标识 

@param[out] 		
无 

@return 		
		成功创建字体句柄，返回字体句柄，否则GRA_INVALID_FONT_ID	

@exception
@note
*/
GR_FONT_ID GrCreateFont(int client_id,GR_CHAR * name, GR_COORD height, char *fontfile);

/**
@brief  功能描述:		
		设置字体大小

@param[in] 
		int client_id   client句柄 

@param[in] 
		GR_FONT_ID fontid 字体句柄

@param[in] 
		GR_COORD size 字体大小，取值范围为5~100

@param[out] 		
无 

@return 		
		无	

@exception
@note
*/
void 	GrSetFontSize(int client_id,GR_FONT_ID fontid, GR_COORD size);

void 	GrSetFontRotation(int client_id,GR_FONT_ID fontid, int tenthsdegrees);

/**
@brief  功能描述:		
		设置字体属性

@param[in] 
		int client_id   client句柄 

@param[in] 
		GR_FONT_ID fontid 字体句柄

@param[in] 
		int flags 字体属性，主要取值为：
GRA_STYLE_NORMAL        0x00
GRA_STYLE_BOLD          0x01
GRA_STYLE_ITALIC        0x02
GRA_STYLE_UNDERLINE     0x04
GRA_STYLE_FIXEDWIDTH   (0x08)
GRA_STYLE_MASK     0x0F

@param[out] 		
无 

@return 		
		无	

@exception
@note
		属性设置是否有效，还必须要字库引擎支持，像文鼎字库现在还不支持设置该属性
*/
void 	GrSetFontAttr(int client_id,GR_FONT_ID fontid, int flags);

/**
@brief  功能描述:		
		销毁字体句柄

@param[in] 
		int client_id   client句柄 

@param[in] 
		GR_FONT_ID fontid 字体句柄

@param[out] 		
		无 

@return 		
		无	

@exception
@note
*/
void 	GrDestroyFont(int client_id,GR_FONT_ID fontid);

/**
@brief  功能描述:		
		获取字体信息

@param[in] 
		int client_id   client句柄 

@param[in] 
		GR_FONT_ID fontid 字体句柄

@param[out] 		
		GR_FONT_INFO * fip 字体信息结构体 

@return 		
		无	

@exception
@note
*/
void 	GrGetFontInfo(int client_id,GR_FONT_ID font, GR_FONT_INFO * fip);
char * GrValidFont(char * fontface,int * validflag);

/**
@brief  功能描述:		
		注册字库引擎

@param[in] 
		GR_FONT_Driver_Proc  *pfontproc 第三方实现的字库接口，如华文可以使用GrFont_GetSTFProc()接口获取到。 

@param[out] 		
		无 

@return 		
		成功注册返回0，否则返回非0值	

@exception
@note
		注册字库引擎前，必须先注册相应的字库数据，否则注册失败。
*/
int GrRegisterFontEngine(GR_FONT_Driver_Proc  *psFontProc);

/**
@brief  功能描述:		
		使用内存中的数据注册或者更新字库数据

@param[in] 
		const char *pcFontdata 字库数据缓冲区 

@param[in] 
		int nDataLen 字库数据缓冲区长度，必须是实际的长度，否则可能导致问题

@param[in] 
		GR_CHAR * pucFaceName 注册数据的字体名称，该参数必须设置，如果传入为NULL，则会用"default"代替。

@param[out] 		
		无 

@return 		
		成功注册返回0，否则返回非0值	

@exception
@note
		该接口实现两个功能：
		1、在pucFaceName未注册时，作为注册数据使用；
		2、pucFaceName已注册，更新相应的facename的数据；
*/
int GrRegisterFontData(const char *pcFontdata, int nDataLen, GR_CHAR * pucFaceName);

/**
@brief  功能描述:		
		使用文件注册或者更新字库数据

@param[in] 
		const char *pcFileName 文件路径字符串 

@param[in] 
		GR_CHAR * pucFaceName 注册数据的字体名称，该参数必须设置，如果传入为NULL，则会用"default"代替。

@param[out] 		
		无 

@return 		
		成功注册返回0，否则返回非0值	

@exception
@note
		该接口实现两个功能：
		1、在pucFaceName未注册时，作为注册数据使用；
		2、pucFaceName已注册，更新相应的facename的数据；
*/
int GrRegisterFontDataFromFile(const char *pcFileName, GR_CHAR * pucFaceName);

/**
@brief  功能描述:		
		获取已注册的字库数据的个数

@param[in] 
		无

@param[out] 		
		无

@return 		
		成功获取返回实际的字库数据个数，否则返回0	

@exception
@note
*/
int GrGetAvailableFontCnt(void);

/**
@brief  功能描述:		
		获取指定索引的字库数据的facename

@param[in] 
		int nFontNameLen缓冲区的长度

@param[in] 
		int nIndex 指定的索引值，该值的取值范围为0~GrGetAvailableFontCnt获取的字体个数

@param[out] 		
		char * pcFontName该参数保存的是字体的facename，该数组必须在上层已经分配空间

@return 		
		成功获取返回0，否则返回非0值	

@exception
@note
		该函数请与GrGetAvailableFontCnt接口组合使用，先使用GrGetAvailableFontCnt获取字体个数，
		然后再获取每一种字体的名称
*/
int GrGetAvailableFontName(char * pcFontName, int nFontNameLen, int nIndex);

/*event APIs*/
void 	GrSelectEvents(int client_id,GR_WINDOW_ID wid, GR_EVENT_MASK eventmask);
void 	GrGetNextEvent(int client_id,GR_EVENT * ep);
void 	GrGetNextEventTimeout(int client_id,GR_EVENT * ep, GR_TIMEOUT timeout);
void 	GrCheckNextEvent(int client_id,GR_EVENT * ep);
int 	GrPeekEvent(int client_id,GR_EVENT * ep);
void 	GrPeekWaitEvent(int client_id,GR_EVENT * ep);
void 	GrInjectPointerEvent(GR_COORD x, GR_COORD y, int button, int visible);
void 	GrInjectKeyboardEvent(GR_WINDOW_ID wid, 
			      GR_KEY keyvalue, GR_KEYMOD modifiers, GR_SCANCODE scancode, GR_BOOL pressed);
void GrInjectGeneralEvent(GR_WINDOW_ID wid, GR_EVENT_TYPE eventType, GR_WINDOW_ID otherid);

/*graphics APIs*/
void GrLine(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
GR_COORD ux1, GR_COORD uy1, GR_COORD ux2, GR_COORD uy2);
void GrDrawLines(int client_id,GR_DRAW_ID w, GR_GC_ID gc, GR_POINT * points, GR_COUNT count);
void GrPoint(int client_id,GR_DRAW_ID id, GR_GC_ID gc, GR_COORD x, GR_COORD y);
void GrPoints(int client_id,GR_DRAW_ID id, GR_GC_ID gc, GR_COUNT count, GR_POINT * pointtable);
void GrRect(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
	    GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height);
void GrFillRect(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
		GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height);
void GrPoly(int client_id,GR_DRAW_ID id, GR_GC_ID gc, GR_COUNT count, GR_POINT * pointtable);
void GrFillPoly(int client_id,GR_DRAW_ID id, GR_GC_ID gc, GR_COUNT count, GR_POINT * pointtable);
void GrEllipse(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
GR_COORD x, GR_COORD y, GR_SIZE rx, GR_SIZE ry);
void GrFillEllipse(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
		   GR_COORD x, GR_COORD y, GR_SIZE rx, GR_SIZE ry);
void GrArc(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
	   GR_COORD x, GR_COORD y, GR_SIZE rx, GR_SIZE ry, 
	   GR_COORD ax, GR_COORD ay, GR_COORD bx, GR_COORD by, int type);
void GrArcAngle(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
		GR_COORD x, GR_COORD y, GR_SIZE rx, GR_SIZE ry, 
		GR_COORD angle1, GR_COORD angle2, int type);

/*new functions for extend ellipse drawing*/
void GrEllipseExt(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
		  GR_COORD x, GR_COORD y, 
		  GR_SIZE rx, GR_SIZE ry, GR_COORD angle);
void GrFillEllipseExt(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
		      GR_COORD x, GR_COORD y, 
		      GR_SIZE rx, GR_SIZE ry, GR_COORD angle);

/*area APIs*/
void GrReadArea(int client_id,GR_DRAW_ID id, 
		GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
		GR_IMAGE_HDR *image);
void GrArea(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
	    GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
	    GRA_Bitmap *pbitmap);
void GrCopyArea(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
		GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
		GR_DRAW_ID srcid, GR_COORD srcx, GR_COORD srcy, int op);
void GrCopyAreaToFit(int client_id, GR_DRAW_ID id, GR_GC_ID gc, 
		GR_RECT *dstRect, GR_DRAW_ID source, GR_RECT *srcRect, int op);
void GrClearArea(int client_id,GR_WINDOW_ID wid, 
		 GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
		 GR_BOOL exposeflag);
void GrDestroyBitmap(int client_id,GR_IMAGE_HDR *bitmap) ;
GR_IMAGE_HDR *GrNewBitmap(int client_id,GR_SIZE width,GR_SIZE height);
				    /*image APIs*/
GR_IMAGE_ID GrLoadImageFromId(int client_id, GR_IMAGE_ID imgid);
GR_IMAGE_ID GrLoadImageFromIdEx(int client_id, GR_IMAGE_ID imgid, GR_RECT * rect);
GR_IMAGE_ID GrLoadImageFromBits(int client_id, GR_IMAGE_HDR * pimage);

void GrBitmap(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
	      GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
	      GR_BITMAP * imagebits);
void GrDrawImageBits(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
		     GR_COORD x, GR_COORD y, 
		     GR_IMAGE_HDR * pimage);
void GrDrawImageFromFile(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
			 GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
			 char *path, int flags);
GR_IMAGE_ID GrLoadImageFromFile(int client_id,char *path, int flags);

/**
@brief 通过传入图片文件的地址载入一幅图片到指定的BUFFER

@param[in] pimage:	用户传入的bitmap地址
@param[in] pcFileName : 	图片文件地址
@param[in] fnAllocBuffercallback :     用户传入的回调函数
@param[in] pUserData :     用户传入的存放解码数据地址

@return 成功返回解码数据地址,失败返回 NULL
@note  bitmap主要存放解码后获取的图片的信息 
*/
unsigned char *GrLoadImgFromFileToBuffer(GR_IMAGE_HDR *pimage, char *pcFileName, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);


/**
@brief 通过传入图片图片数据地址空间载入一幅图片到指定的BUFFER

@param[in] pimage:	用户传入的bitmap地址
@param[in] buffer : 	图片文件内存地址
@param[in] size : 	图片文件长度
@param[in] fnAllocBuffercallback :     用户传入的回调函数
@param[in] pUserData :     用户传入的存放解码数据地址

@return 成功返回解码数据地址,失败返回 NULL
@note    bitmap主要存放解码后获取的图片的信息 
*/
unsigned char *GrLoadImgFromBufferToBuffer(GR_IMAGE_HDR *pimage,void *buffer, int size, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);


/**
@brief 通过传入图片文件的地址和指定的缩放宽高载入一幅图片到指定的BUFFER

@param[in] pimage:	用户传入的bitmap地址
@param[in] pcFileName : 	图片文件地址
@param[in] nWidth :      解码之后的图片宽度。
@param[in] nHeight :     解码 之后的图片高度。
@param[in] fnAllocBuffercallback :     用户传入的回调函数
@param[in] pUserData :     用户传入的存放解码数据地址

@return 成功返回解码数据地址,失败返回 NULL
@note    bitmap主要存放解码后获取的图片的信息 
*/
unsigned char *GrLoadImgFromFileScaledToBuffer(GR_IMAGE_HDR *pimage,char *pcFileName, int nWidth, int nHeight, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);



/**
@brief 通过传入图片数据地址空间和指定的缩放宽高载入一幅图片到指定的BUFFER

@param[in] pimage:	用户传入的bitmap地址
@param[in] buffer : 	图片文件内存地址
@param[in] size : 	图片文件长度
@param[in] nWidth :      解码之后的图片宽度。
@param[in] nHeight :     解码 之后的图片高度。
@param[in] fnAllocBuffercallback :     用户传入的回调函数
@param[in] pUserData :     用户传入的存放解码数据地址

@return 成功返回解码数据地址,失败返回 NULL
@note    bitmap主要存放解码后获取的图片的信息 
*/
unsigned char *GrLoadImgFromBufferScaledToBuffer(GR_IMAGE_HDR *pimage,void *buffer, int size, int nWidth, int nHeight, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);

/**
@brief 通过传入图片文件的地址和指定的缩放宽高载入一幅图片

@param[in] pcFileName : 图片文件地址。
@param[in] nWidth :      解码之后的图片宽度。
@param[in] nHeight :     解码 之后的图片高度。
@param[in] flags :     暂时没有作用，建议传 0

@return 成功返回大于0的图片ID,失败返回 -1
@note   当图片为JPEG格式时，nWidth ，nHeight 为解码后图片的RGB数据宽和高 参数满足nWidth>=0;  nHeight>=0
*/
GR_IMAGE_ID GrLoadImageFromFileScaled(int client_id,  char* pcFileName, int nWidth, int nHeight, int  flag);

/**
@brief 得到多帧APNG图片的的下一帧数据

@param[in] client_id : client id
@param[in] imgid :     通过解码接口得到的图片Id


@return 成功返回该帧的持续时间，失败返回 -1
@note   目前仅支持APNG多帧图片
*/
int GrLoadNextImageFrame(int client_id, GR_IMAGE_ID imgid);

GR_IMAGE_ID GrRotateImage(int client_id, GR_IMAGE_ID imageid, int angle);
GR_IMAGE_ID GrMirrorImage(int client_id, GR_IMAGE_ID imageid);

void GrDrawImageFromBuffer(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
			   GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
			   void *buffer, int size, int flags);


int GrClientSync(int client_id, GR_DRAW_ID id);
/**
@brief 控制gif的timer

@param[in] client_id  client_id
@param[in] id     图片的句柄
@param[in] bStopFlag       true 表示停止gif的timer, false表示启动gif的timer

@return 无
@note   
*/

void GrControlGifTimer(int client_id, GR_IMAGE_ID id, BOOL bStopFlag);


/**
@brief 获取绘图平面的数据

@param[in] client_id  client_id
@param[in] nIndex  0 表示标清 1 表示高清
@param[out] 无
@return pixmapID
@note  获取绘图平面的数据,用户需要释放pixmapid。
*/

GR_WINDOW_ID GrGetScreenDisplay(int client_id,int index);

/**
	@brief 往显存上写数据

	@param[in] client_id  client_id
	@param[in] id  窗口句柄
	@param[in] gc  gc句柄
	@param[in] x  x坐标
	@param[in] y  y坐标
	@param[in] width  宽度
	@param[in] height  高度
	@return 没有返回值
	@note  往显存上写数据	
*/
void GrReStoreScreenDisplay(int client_id,GR_DRAW_ID id,GR_GC_ID gc,GR_DRAW_ID source);


/**
@brief 利用图片数据构建BMP图片

@param[in] pBitmapMemorySrc 图片数据地址空间
@param[in] nMemoryCount     图片数据的大小
@param[in] colorDepth       图片的颜色深度，1 2 4 8 16 24 32
@param[in] pPalette         图片调色板数据地址，当colorDepth小于8时，不能为NULL,其他可以为NULL
@param[in] nPaletteCount    调色板个数
@param[in] w                图片宽度
@param[in] h                图片高度
@param[out] nPaletteCount   输出参数，构建后的BMPBuf的大小

@return 成功返回构建后BMPBuf，否则返回NULL
@note   该接口构建的BMPBuffer，在使用后必须调用GrReleaseBMPBuffer释放，否则会有内存泄漏
*/
void *GrBuildBMPBuffer(const unsigned char *pBitmapMemorySrc, int nMemoryCount, 
                    unsigned int colorDepth, 
                    const unsigned char *pPalette, int nPaletteCount, int w, int h, int *pnBmpLen);
/**
@brief 释放利用图片数据构建BMP图片

@param[in] buffer 要释放的图片buffer

@return 无
*/
void GrReleaseBMPBuffer(void *buffer);


GR_IMAGE_ID GrLoadImageFromBuffer(int client_id,void *buffer, int size, int flags);


/**
@brief 通过传入的图片数据,缩放解码

@param[in] pcImageData : 图片数据的首地址。
@param[in] size :       图片数据的长度。
@param[in] nWidth :      解码之后的图片宽度。
@param[in] nHeight :     解码 之后的图片高度。
@param[in] flags :     暂时没有作用，建议传 0

@return 成功返回大于0的图片ID,失败返回 -1
@note   当图片为JPEG格式时，nWidth ，nHeight 为解码后图片的RGB数据宽和高 参数满足nWidth>=0;  nHeight>=0
*/
GR_IMAGE_ID GrLoadImageFromBufferScaled(int client_id,  char* pcImageData, int size, int nWidth, int nHeight, int  flags);
void GrDrawImageToFit(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
		      GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
		      GR_IMAGE_ID imageid);
void GrDrawImageToFitEx(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
			int srcx, int srcy, int srcw, int srch, 
			GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
			GR_IMAGE_ID imageid);

/**
@brief 绘制动画，包括动态gif，动态APNG等多帧图片

@param[in] client_id : client ID,GrOpen或者GrOpenExt的返回值
@param[in] id :        绘制的目标窗口句柄，GrNewWindow或者GrNewPixmap返回值
@param[in] gc :       gc句柄
@param[in] srcx :     图片上要显示区域的x坐标
@param[in] srcy :     图片上要显示区域的y坐标
@param[in] srcw :     图片上要显示区域的宽度
@param[in] srch :     图片上要显示区域的高度
@param[in] x :     	  目标区域x坐标
@param[in] y :     	  目标区域y坐标
@param[in] width :    目标区域的宽度
@param[in] height :   目标区域的高度
@param[in] imageid :  要绘制的多帧图片ID,  
@param[in] flag :  	  是否要强制刷新，建议传0，除awt调用外，此参数是为了兼容以前的Java应用中的动态gif

@return 无
@note 该接口通过timer控制多帧图片的显示，直接绘制显存。
@note 目前只支持多帧gif显示，如果APNG需要也可以类似实现，但不建议如此
*/
void GrDrawAnimation(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
			int srcx, int srcy, int srcw, int srch, 
			GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
			GR_IMAGE_ID imageid, int flag);

void GrFreeImage(int client_id,GR_IMAGE_ID id);
void GrGetImageInfo(int client_id,GR_IMAGE_ID id, GR_IMAGE_INFO * iip);
void GrDrawImage(int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *dst,GR_RECT *src, GR_IMAGE_ID imageid);
/*Draw Text API*/

/**
@brief  功能描述:		
		在指定的位置显示指定的字符串

@param[in] 
		int client_id   client句柄 

@param[in]
GR_DRAW_ID id 显示所在窗口ID

@param[in] 
		GR_GC_ID gc 绘图上下文句柄

@param[in] 
		GR_COORD x 显示的起始左上角x坐标

@param[in] 
		GR_COORD y 显示的起始左上角y坐标

@param[in] 
		void *str 显示的字符串内容缓冲区

@param[in] 
		int count 显示的字符串个数

@param[in] 
		int flags 字符串的编码方式，主要取值为：
		GRA_FONT_CODE_ASCII      0x01
		GRA_FONT_CODE_UTF8       0x02
		GRA_FONT_CODE_UNICODE    0x04

@param[out] 		
无

@return 		
		无	

@exception
@note
*/
void GrText(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
	    GR_COORD x, GR_COORD y, 
	    void *str, GR_COUNT count, int flags);
/*cursor APIs*/

/*************************************************/
/**
@brief  功能描述:		
		在指定的位置显示指定的字符串

@param[in] 
		int client_id   client句柄 

@param[in]
GR_DRAW_ID id 显示所在窗口ID

@param[in] 
		GR_GC_ID gc 绘图上下文句柄

@param[in] 
		GR_POINT *charPos 每个字符串的坐标数组

@param[in] 
		void *str 显示的字符串内容缓冲区

@param[in] 
		int count 显示的字符串个数

@param[in] 
		int flags 字符串的编码方式，主要取值为：
		GRA_FONT_CODE_ASCII      0x01
		GRA_FONT_CODE_UTF8       0x02
		GRA_FONT_CODE_UNICODE    0x04

@param[out] 		
无

@return 		
		无	

@exception
@note
		该接口与GrText的最大区别在于，该接口必须指定每个字符的坐标，这样可以实现排版的多样性。主要被浏览器使用，输入法的候选字也是调用该接口进行显示的。
*/
int GrTextEx(int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_POINT *charPos, void *str, GR_COUNT count, int flags);

/**
@brief  功能描述:
		获取指定字符的字模数据，字模数据为灰度级别数据，即每个像素由1个字节表示
 
@param[in]
		int client_id client句柄

 @param[in]
		GR_FONT_ID fontid 字体句柄

 @param[in]
		unsigned short usCharCode 字符的编码，必须是UNICODE编码方式
		
 @param[in]
		int nMemSize传入的内存缓冲区大小值，该值必须为pFontDataMem的实际大小
 
@param[out] 
		UCHAR *pFontDataMem 传入内存缓冲区，用于保存获取到的字符串的字模数据，
		这块缓冲区的排列格式:
		1、每行的字节个数为GC句柄中字体设置的大小；
		2、总行数为GC句柄中字体设置的大小；
		3、这块缓冲区的大小必须大于等于 (fontsize * fontsize);
		如果置为空，则不获取字模数据

@param[out] 
 		int * pnWidth 获取字符的实际宽度，如果置为空，则不获取该值

 @param[out] 
 		int * pnHeight 获取字符的实际高度，如果置为空，则不获取该值
		
@return
 		无
@exception
@note
*/
void GrGetCharFontData(int client_id, GR_FONT_ID fontid, unsigned short usCharCode, UCHAR *pFontDataMem, int nMemSize, int * pnWidth, int  * pnHeight);
/*************************************************/

/**
@brief  功能描述:
		设置OSG层的显示类型
 
@param[in]
		int nType 设置osg的类型
		类型取值可以为:
		GRA_DISPLAYOPTION_COLOR  为OSG显示变灰
		GRA_DISPLAYOPTION_3D 为OSG支持3DTV
		

 @param[in]
		int nValue 设置的对应类型nType的值
		当nType为GRA_DISPLAYOPTION_COLOR时，
			nValue 值为1,表示设置OSG变灰
			nValue 值为0,表示设置OSG恢复正常
			
		当nType为GRA_DISPLAYOPTION_3D时，
			nValue 值为0,表示OSG正常显示
			nValue 值为1,表示将OSG一分为二左右显示
			nValue 值为2,表示将OSG一分为二上下显示

 @param[out] 
 		0表示成功，-1表示失败
		
@return
 		无
@exception
@note
*/
int GrSetDisplayOption(int client_id, int nType, int nValue);

/**
@brief 设置Cursor资源所在文件夹

@param[in] client_id :  client句柄
@param[in] pcDirPath : 要设置资源文件夹路径，如"/workdir/arrow/SkyWorks/"

@return 成功设置返回0，否则返回-1
@note   此函数必须在加载curosr资源之前设置，否则无效
		client_id暂时未用到，可以不设置
*/
int GrSetCursorResPath(int client_id, const char * pcDirPath);

/**
@brief 创建光标

@param[in] client_id :  client句柄
@param[in] width 宽度为32
@param[in] height 高度为32
@param[in] hotx 热点的x坐标
@param[in] hoty 热点的y坐标
@param[in] pCursorBits 传入的图片数据

@return 成功设置返回0，否则返回-1
@note
*/
GR_CURSOR_ID GrNewCursor(int client_id,GR_SIZE width, GR_SIZE height, 
			 GR_COORD hotx, GR_COORD hoty, 
			 unsigned char  * pCursorBits);

/**
@brief 显示光标

@param[in] xoffset x方向偏移(相对于上次显示位置)
@param[in] yoffset y方向偏移(相对于上次显示位置)
            
@return 
        无
@note
*/
void GrShowCursor(int xoffset, int yoffset);
/**
@brief  功能描述:
		销毁光标
 
@param[in]
		client_id: client句柄
		cid: 光标ID
 
@param[out] 
 
@return
 		无
@exception
@note   会释放光标资源bitmap
*/
void GrDestroyCursor(int client_id,GR_CURSOR_ID cid);

/**
@brief  功能描述:
		为当前窗口设置光标
 
@param[in]
		client_id: client句柄
		wid: 窗口句柄
		cid: 光标ID
		如果cid非法，则使用默认光标样式
 
@param[out] 
 
@return
 		无
@exception
@note
*/
void GrSetWindowCursor(int client_id,GR_WINDOW_ID wid, GR_CURSOR_ID cid);

/**
@brief  功能描述:
		移动光标到目标位置
 
@param[in]
		x: 目标x坐标
		y: 目标y坐标
		如果x,y超出屏幕范围，则修正x,y，移动后光标显示在边界位置
 
@param[out] 
 
@return
 		无
@exception
@note
*/
void GrMoveCursor(int client_id,GR_COORD x, GR_COORD y);

/**
@brief  功能描述:
		获取CursorId
 
@param[in]
        client_id: client句柄
		index: 光标数组索引
		如果index >= E_MAX_COUNT_CUROSR 返回index = 0对应的光标ID
		index < E_MAX_COUNT_CUROSR      返回index对应的光标ID
 
@param[out] 
 
@return
 		成功返回CursorId, 失败返回-1
@exception
@note
*/
int GrGetCursorId(int client_id,int index);

/**
@brief  功能描述:
		激活光标，使其可见，暂未实现
 
@param[in]
 
@param[out] 
 
@return

@exception
@note
*/
void GrActivateCursor(void);

/**
@brief  功能描述:
		反激活光标，是其不可见，暂未实现
 
@param[in]
 
@param[out] 
 
@return

@exception
@note
*/
void GrDeactivateCursor(void);

/**
@brief  功能描述:
        设置光标保持显示的时间（自动隐藏光标功能默认开启,用户可传入-1取消自动隐藏）

@param[in]
int client_id  client句柄
int nTime      光标保持显示的时间，单位为ms,nTime为-1，则不自动隐藏光标

@return
BOOL   成功返回TRUE
       失败返回FALSE

@exception
@note
*/
BOOL GrSetCursorKeepShowTime(int client_id, int nTime);

/*color APIs*/
void GrGetSystemPalette(int client_id,GR_PALETTE * pal);
void GrSetSystemPalette(int client_id,GR_COUNT first, GR_PALETTE * pal);
void GrFindColor(int client_id,GR_COLOR c, GR_PIXELVAL * retpixel);
/*other */
void GrSetSelectionOwner(int client_id,GR_WINDOW_ID wid, GR_CHAR * typelist);
GR_WINDOW_ID GrGetSelectionOwner(int client_id,GR_CHAR ** typelist);

void GrRequestClientData(int client_id,GR_WINDOW_ID wid, GR_WINDOW_ID rid,
			 GR_SERIALNO serial, GR_MIMETYPE mimetype);
void GrSendClientData(int client_id,GR_WINDOW_ID wid, GR_WINDOW_ID did, 
		      GR_SERIALNO serial, GR_LENGTH len, GR_LENGTH thislen, void *data);
void GrBell(void);
void GrSetBackgroundPixmap(int client_id,GR_WINDOW_ID wid, GR_WINDOW_ID pixmap, int flags);
void GrQueryTree(int client_id,GR_WINDOW_ID wid, GR_WINDOW_ID * parentid, 
		 GR_WINDOW_ID ** children, GR_COUNT * nchildren);

/*Timer APIs*/
GR_TIMER_ID GrCreateTimer(int client_id,GR_WINDOW_ID wid, GR_TIMEOUT period);
void GrDestroyTimer(int client_id,GR_TIMER_ID tid);

/*Error handler APIs*/
GR_FNCALLBACKEVENT GrSetErrorHandler(GR_FNCALLBACKEVENT fncb);
void GrDefaultErrorHandler(int client_id,GR_EVENT * ep);

/* Picture functions */
INT GrDrawPictureFromBuffer(int client_id, GR_WINDOW_ID id, GR_GC_ID gc,
			    GR_COORD srcx, GR_COORD srcy, 
			    GR_SIZE srcw, GR_SIZE srch, 
			    GR_COORD x, GR_COORD y, 
			    GR_SIZE w, GR_SIZE h, 
			    char *srcBuff, int nLen, 
			    int flags);
INT GrCreatePictureFromBuffer(int client_id, 
				GR_COORD srcx, GR_COORD srcy, 
				GR_SIZE srcw, GR_SIZE srch, 
				UCHAR *srcBuff, int nLen,
				int flags);
INT GrDrawPictureFromFile(int client_id,GR_WINDOW_ID id,GR_GC_ID gc,
			    GR_COORD srcx,GR_COORD srcy,
			    GR_SIZE srcw, GR_SIZE srch,
			    GR_COORD x,GR_COORD y,
			    GR_SIZE w, GR_SIZE h,
			    char *fname,int flags) ;
INT GrCreatePictureFromFile(int client_id, GR_COORD srcx,
			      GR_COORD srcy,
			      GR_SIZE srcw, GR_SIZE srch,
			      char *fname,
			      int flags);
INT GrCreatePictureFromBitmap(int client_id, GR_COORD srcx, GR_COORD srcy, 
				GR_SIZE srcw, GR_SIZE srch, 
				GRA_Bitmap * bitmap, int size);

INT GrDrawPicture(int client_id,GR_WINDOW_ID id,GR_GC_ID gc,
		    GR_COORD x,GR_COORD y,GR_SIZE w, GR_SIZE h,
		    INT pictureId,int flags);
INT GrHidePicture(int client_id, INT picture_id);
void GrFillPictureRect(int client_id, INT picture_id, GR_COORD x, GR_COORD y, GR_SIZE w, GR_SIZE h);
void GrDestroyPicture(int client_id,INT picture_id) ;
void GrClosePicture(void) ;
GR_WINDOW_ID GrNewPixmapFromData(int client_id, GR_SIZE width, GR_SIZE height, GR_COLOR foreground,
				 GR_COLOR background, void *bits, int flags);
GR_BITMAP* GrNewBitmapFromData(int client_id, GR_SIZE width, GR_SIZE height, GR_SIZE bits_width,
			       GR_SIZE bits_height, void *bits, int flags);

void GrSetMixWeight(int client_id,UCHAR alpha) ;
void GrMixOut(int client_id) ;
void GrMixIn(int client_id) ;

/**
@brief 设置某块区域作为独占区域

@param[in] client_id :  客户端ID
@param[in] psGrapRect : 要设置的区域，为NULL时表示取消设置

@return 无
@note   此函数只对多帧gif的刷新起作用
*/
void GrSetHogRect(int client_id, GR_RECT *psGrapRect);
void GrVersion(char* version, int len);


/**
*  功能描述:
*		ICON层模块初始化, 在AddIcon之前, 需要加初始化Icon模块
*
* \param[in]
*		无
*
* \param[out] 
*		无
*
* \return
*		正值 : 执行成功
*         -1 : 执行失败
*  \exception
*  \note
*/
int GrIconInitial();

/**
*  功能描述:
*		销毁ICON层模块.
*
* \param[in]
*		无
*
* \param[out] 
*		无
*
* \return
*		正值 : 执行成功
*         -1 : 执行失败
*  \exception
*  \note
*/
int GrIconRelease();

/**
*  功能描述:
*		将给定的ICON数据加载到系统中.
*
* \param[in]
*		MW_ICON_S sIcon   : 需要加载的ICON结构体数据
*
* \param[out] 
*		无
*
* \return
*		正值 : ICON ID
*         -1 : 执行失败
*  \exception
*  \note
*/
int GrAddIcon(MW_ICON_S *psIcon);

/**
*  功能描述:
*		从系统中移去指定ID的ICON.
*
* \param[in]
*		int nIconId 		: 需要移去的ICON ID
*
* \param[out] 
*		无
*
* \return
*		   0 : 执行成功
*         -1 : 执行失败
*  \exception
*  \note
*/
int GrRemoveIcon(int nIconId);

/**
*  功能描述:
*		显示指定ID的ICON.
*
* \param[in]
*		int nIconId 		: 需要显示的ICON ID
*
* \param[out] 
*		无
*
* \return
*		   0 : 执行成功
*         -1 : 执行失败
*  \exception
*  \note
*/
int GrShowIcon(int nIconId);


/**
*  功能描述:
*		隐藏指定ID的ICON.
*
* \param[in]
*		int nIconId 		: 需要隐藏的ICON ID
*
* \param[out] 
*		无
*
* \return
*		   0 : 执行成功
*         -1 : 执行失败
*  \exception
*  \note
*/
int GrHideIcon(int nIconId);

/**
*  功能描述:
*		设置是否从3D分支中加载图片.
*
* \param[in]
*		bool b_LoadImgFrom3DFlag		: true为是从3D分支中加载图片;false 为否
*
* \param[out] 
*		无
*
* \return
*		无  
*  \exception
*  \note
*   应用场景:1. 针对3D中红色和蓝色反色的情况
*                         2. 解码之前设置bLoadImgFrom3DFlag为true
*                         3. 调用解码接口
*                         4. 解码之后设置bLoadImgFrom3DFlag为false
*/
void GrSetLoadImgFrom3DFlag(BOOL bLoadImgFrom3DFlag);

/**
@brief  功能描述:
		将图像数据保存成用户指定的格式
 
@param[in]
		pstImage  				  被转换的原始图形数据，详细请参见mwtypes.h中的定义,如下: 
								w/h/pitch=w*4/format/pixels=argb_buffer_addr
		nDstImageType				  用户需要转换的目标图片格式，可支持GRA_BITMAP_SOURCE_JPEG；以后可以扩展支持
								GRA_BITMAP_SOURCE_AUTOGIF，GRA_BITMAP_SOURCE_APNG，GRA_BITMAP_SOURCE_PNG（定义请参见mwtypes.h）
 
@param[out] 
		nDstImgFileHandle			  输出文件句柄,存放转换后的图片数据
 
@return
 		创建成功，则返回0，否则返回-1
@exception
@note
*/
int GrSaveImage( const GR_IMAGE_HDR * pstImage, int nDstImageType, int nDstImgFileHandle );

#define GrSetBorderColor  GrSetWindowBorderColor
#define GrClearWindow(client_id,wid,exposeflag)	GrClearArea(client_id,wid,0,0,0,0,exposeflag)	

/* useful function macros*/
#define GrSetWindowBackgroundColor(client_id,wid,color) \
	{	GR_WM_PROPERTIES props;	\
	        props.flags = GR_WM_FLAGS_BACKGROUND; \
		props.background = color; \
		GrSetWMProperties(client_id,wid, &props); \
	}
#define GrSetWindowBorderSize(client_id,wid,width) \
	{	GR_WM_PROPERTIES props;	\
		props.flags = GR_WM_FLAGS_BORDERSIZE; \
		props.bordersize = width; \
		GrSetWMProperties(client_id,wid, &props); \
	}
#define GrSetWindowBorderColor(client_id,wid,color) \
	{	GR_WM_PROPERTIES props;	\
		props.flags = GR_WM_FLAGS_BORDERCOLOR; \
		props.bordercolor = color; \
		GrSetWMProperties(client_id,wid, &props); \
	}
#define GrSetWindowTitle(client_id,wid,name) \
	{	GR_WM_PROPERTIES props;	\
		props.flags = GR_WM_FLAGS_TITLE; \
		props.title = (GR_CHAR *)name; \
		GrSetWMProperties(client_id,wid, &props); \
	}

#define GrDefaultGC() GRA_DEFAULT_GC_ID
#define GrDefaultFont() GRA_DEFAULT_FONT_ID

#ifdef __cplusplus
	}
#endif


typedef struct event_list EVENT_LIST;
struct event_list {
  EVENT_LIST *next;
  GR_EVENT event;
};

#define ACCESS_PER_THREAD_DATA()

#define BLACK		GRARGB( 0  , 0  , 0   )
#define BLUE		GRARGB( 0  , 0  , 128 )
#define GREEN		GRARGB( 0  , 128, 0   )
#define CYAN		GRARGB( 0  , 128, 128 )
#define RED		GRARGB( 128, 0  , 0   )
#define MAGENTA		GRARGB( 128, 0  , 128 )
#define BROWN		GRARGB( 128, 64 , 0   )
#define LTGRAY		GRARGB( 192, 192, 192 )
#define GRAY		GRARGB( 128, 128, 128 )
#define LTBLUE		GRARGB( 0  , 0  , 255 )
#define LTGREEN		GRARGB( 0  , 255, 0   )
#define LTCYAN		GRARGB( 0  , 255, 255 )
#define LTRED		GRARGB( 255, 0  , 0   )
#define LTMAGENTA	GRARGB( 255, 0  , 255 )
#define YELLOW		GRARGB( 255, 255, 0   )
#define WHITE		GRARGB( 255, 255, 255 )

/* other common colors*/
#define DKGRAY		GRARGB( 32,  32,  32)

/* 
 * ROP blitter opcodes (extensions < 0x10000000 are GRAMODE_xxx blit ops)
 */
#define GRAROP_EXTENSION		0xff000000L	/* rop extension bits*/

/* copy src -> dst except for transparent color in src*/
#define GRAROP_SRCTRANSCOPY	0x11000000L

/* alpha blend src -> dst with constant alpha, alpha value in low 8 bits*/
#define GRAROP_BLENDCONSTANT	0x12000000L

/* alpha blend fg/bg color -> dst with src as alpha channel*/
#define GRAROP_BLENDFGBG		0x13000000L

/* alpha blend src -> dst with separate per pixel alpha channel*/
#define GRAROP_BLENDCHANNEL	0x14000000L

/* stretch src -> dst*/
#define GRAROP_STRETCH		0x15000000L

/* blits rops based on src/dst binary operations*/
#define GRAROP_COPY		(CS_MODE_COPY << 24L)
#define	GRAROP_XOR		(CS_MODE_XOR << 24L)
#define	GRAROP_OR		(CS_MODE_OR << 24L)
#define GRAROP_AND		(CS_MODE_AND << 24L)
#define	GRAROP_CLEAR		(CS_MODE_CLEAR << 24L)
#define	GRAROP_SET		(CS_MODE_SET << 24L)
#define	GRAROP_EQUIV		(CS_MODE_EQUIV << 24L)
#define	GRAROP_NOR		(CS_MODE_NOR << 24L)
#define	GRAROP_NAND		(CS_MODE_NAND << 24L)
#define	GRAROP_INVERT		(CS_MODE_INVERT << 24L)
#define	GRAROP_COPYINVERTED	(CS_MODE_COPYINVERTED << 24L)
#define	GRAROP_ORINVERTED	(CS_MODE_ORINVERTED << 24L)
#define	GRAROP_ANDINVERTED	(CS_MODE_ANDINVERTED << 24L)
#define GRAROP_ORREVERSE		(CS_MODE_ORREVERSE << 24L)
#define	GRAROP_ANDREVERSE	(CS_MODE_ANDREVERSE << 24L)
#define	GRAROP_NOOP		(CS_MODE_NOOP << 24L)

#define GRAROP_SRCCOPY		GRAROP_COPY	/* obsolete*/
#define GRAROP_SRCAND		GRAROP_AND	/* obsolete*/
#define GRAROP_SRCINVERT		GRAROP_XOR	/* obsolete*/
#define GRAROP_BLACKNESS     	GRAROP_CLEAR	/* obsolete*/

/* convert an GRAROP to drawing mode GRAMODE value*/
#define GRAROP_TO_MODE(op)	((op) >> 24)
extern GR_FONT_ID gr_default_font_id;

#endif /* _MWAPI_H */
