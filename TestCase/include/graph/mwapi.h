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

//����Ϊ14������java��ʹ�õĸ��������õ�
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
	GRA_Bitmap *psCursorBitmap;  //�洢��Icon֮ǰ��ǰ�����ϸ����������, �������, ��ΪNULL
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
@brief  ��������:
		��ȡclient id
 
@param[in]
		��
 
@param[out] 
		��
 
@return
 		����Ѿ�������client���򷵻ص�һ��client��id�����򷵻�-1
@exception
@note
*/
int GrGetUsedClientId(void);

/**
@brief  ��������:
		ע�ᵱǰpixmapΪ�ö�pixmap
 
@param[in]  
                     client_id : clientid��
                     pixmapId :   Ҫע���pixmap��IDֵ
                     x              :�ö�pixmap��������Ļ����ʾλֵ��X����
                     y              :�ö�pixmap��������Ļ����ʾλֵ��X����
                     pixmapZOrder:��ǰpixmap��ʾ��z�����ö������ú�(Ŀǰ��ʱδ�õ�)����ֵ��ȡGR_TOP_PIXMAP_TOP��GR_TOP_PIXMAP_BOTTOM
@param[out] 
		��
 
@return
 		�ɹ�����0�����򷵻�-1
@exception
@note
*/

int GrRegistTopPixmap(int client_id, GR_WINDOW_ID pixmapId, int x, int y, int  pixmapZOrder);
/**
@brief  ��������:
		ע������ǰ�ö�pixmap
 
@param[in]  
                     client_id : clientid��
                     pixmapId :   Ҫע����pixmap��IDֵ
                    
@param[out] 
		��
 
@return
 		�ɹ�����0�����򷵻�-1
@exception
@note
*/
int  GrUnRegistTopPixmap(int client_id, GR_WINDOW_ID pixmapId);
int GrRegisterFont(char *fontfile); 
int GrUnregisterFont(char *facename,char *fontfile);

//ȫ����Ч�ĺ���ָ��
typedef void (*CSEFTExcuteEftCallback)(int client_id,  GR_WINDOW_ID wid,  GR_GC_ID gc, GR_WINDOW_INFO info, int m_prepixmap, int m_curpixmap);
typedef int  (*CSEFTGetGlobalTypeCallback)();

/**
@brief ע��ȫ����Ч����

@param[in] nType : ע��ĺ������͡�
@param[in] pfnCallback :       ע��ĺ���ָ�롣

@return �ɹ�����0
@note   
*/
int GrRegisterGlobalEftFun(int nFunType, void * pfnCallback);

/**
@brief ���ȫ����Ч�ı�־

@param[in] client_id : clientid��
@param[in] m_prepixmap :   ��һ��Ӧ�õ�pixmap��
@param[in] m_curpixmap :   ��ǰӦ�õ�pixmap��

@return �ɹ�����0
@note   ��
*/
int GrReSetGlobalEftFlag(int client_id, int m_prepixmap, int m_curpixmap);

/**
@brief ����ȫ����Ч��������־

@param[in] nFlag : ����

@return �ɹ�����0
@note   ��
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
@brief  ��������:		
		����GC������

@param[in] 
		int client_id   client��� 

@param[in] 
		GR_GC_ID gc ��ͼ�����ľ��

@param[in] 
		GR_FONT_ID font ������

@return 		
		��	

@exception
@note
		ֻ�е����˸ýӿڣ��������������Ż��ڻ�ͼʱ��Ч
*/
void GrSetGCFont(int client_id,GR_GC_ID gc, GR_FONT_ID font);

/**
@brief  ��������:		
		��ȡ�ַ�����ʾ���ؿ��

@param[in] 
		int client_id   client��� 

@param[in] 
		GR_GC_ID gc ��ͼ�����ľ��

@param[in] 
		void *str ��ʾ���ַ������ݻ�����

@param[in] 
		int count ��ʾ���ַ�������

@param[in] 
		int flags �ַ����ı��뷽ʽ����ҪȡֵΪ��
		GRA_FONT_CODE_ASCII      0x01
GRA_FONT_CODE_UTF8       0x02
GRA_FONT_CODE_UNICODE    0x04

@param[out] 		
int *retwidth ��ʾ���ַ������ؿ�� 

@param[out] 		
int *retheight ��ʾ���ַ������ظ߶ȶ�

@return 		
		��	

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
@brief  ��������:		
		��������

@param[in] 
		int client_id   client��� 

@param[in] 
		GR_CHAR * facename �������������ƣ�����NULL����ʹ��Ĭ��"default"���ƣ����е�����������Ҫʹ�õ��ò�������Ϊ���ָ��������Ψһ��ʶ

@param[in] 
		GR_COORD height �����������С��ȡֵ��ΧΪ5~100

@param[in] 
		char *fontfile �ֿ��ļ�·���ַ���������ΪNULL����facename δ�ҵ�������£�ʹ�øñ������ƶ��������ֿ����ı�ʶ 

@param[out] 		
�� 

@return 		
		�ɹ�����������������������������GRA_INVALID_FONT_ID	

@exception
@note
*/
GR_FONT_ID GrCreateFont(int client_id,GR_CHAR * name, GR_COORD height, char *fontfile);

/**
@brief  ��������:		
		���������С

@param[in] 
		int client_id   client��� 

@param[in] 
		GR_FONT_ID fontid ������

@param[in] 
		GR_COORD size �����С��ȡֵ��ΧΪ5~100

@param[out] 		
�� 

@return 		
		��	

@exception
@note
*/
void 	GrSetFontSize(int client_id,GR_FONT_ID fontid, GR_COORD size);

void 	GrSetFontRotation(int client_id,GR_FONT_ID fontid, int tenthsdegrees);

/**
@brief  ��������:		
		������������

@param[in] 
		int client_id   client��� 

@param[in] 
		GR_FONT_ID fontid ������

@param[in] 
		int flags �������ԣ���ҪȡֵΪ��
GRA_STYLE_NORMAL        0x00
GRA_STYLE_BOLD          0x01
GRA_STYLE_ITALIC        0x02
GRA_STYLE_UNDERLINE     0x04
GRA_STYLE_FIXEDWIDTH   (0x08)
GRA_STYLE_MASK     0x0F

@param[out] 		
�� 

@return 		
		��	

@exception
@note
		���������Ƿ���Ч��������Ҫ�ֿ�����֧�֣����Ķ��ֿ����ڻ���֧�����ø�����
*/
void 	GrSetFontAttr(int client_id,GR_FONT_ID fontid, int flags);

/**
@brief  ��������:		
		����������

@param[in] 
		int client_id   client��� 

@param[in] 
		GR_FONT_ID fontid ������

@param[out] 		
		�� 

@return 		
		��	

@exception
@note
*/
void 	GrDestroyFont(int client_id,GR_FONT_ID fontid);

/**
@brief  ��������:		
		��ȡ������Ϣ

@param[in] 
		int client_id   client��� 

@param[in] 
		GR_FONT_ID fontid ������

@param[out] 		
		GR_FONT_INFO * fip ������Ϣ�ṹ�� 

@return 		
		��	

@exception
@note
*/
void 	GrGetFontInfo(int client_id,GR_FONT_ID font, GR_FONT_INFO * fip);
char * GrValidFont(char * fontface,int * validflag);

/**
@brief  ��������:		
		ע���ֿ�����

@param[in] 
		GR_FONT_Driver_Proc  *pfontproc ������ʵ�ֵ��ֿ�ӿڣ��绪�Ŀ���ʹ��GrFont_GetSTFProc()�ӿڻ�ȡ���� 

@param[out] 		
		�� 

@return 		
		�ɹ�ע�᷵��0�����򷵻ط�0ֵ	

@exception
@note
		ע���ֿ�����ǰ��������ע����Ӧ���ֿ����ݣ�����ע��ʧ�ܡ�
*/
int GrRegisterFontEngine(GR_FONT_Driver_Proc  *psFontProc);

/**
@brief  ��������:		
		ʹ���ڴ��е�����ע����߸����ֿ�����

@param[in] 
		const char *pcFontdata �ֿ����ݻ����� 

@param[in] 
		int nDataLen �ֿ����ݻ��������ȣ�������ʵ�ʵĳ��ȣ�������ܵ�������

@param[in] 
		GR_CHAR * pucFaceName ע�����ݵ��������ƣ��ò����������ã��������ΪNULL�������"default"���档

@param[out] 		
		�� 

@return 		
		�ɹ�ע�᷵��0�����򷵻ط�0ֵ	

@exception
@note
		�ýӿ�ʵ���������ܣ�
		1����pucFaceNameδע��ʱ����Ϊע������ʹ�ã�
		2��pucFaceName��ע�ᣬ������Ӧ��facename�����ݣ�
*/
int GrRegisterFontData(const char *pcFontdata, int nDataLen, GR_CHAR * pucFaceName);

/**
@brief  ��������:		
		ʹ���ļ�ע����߸����ֿ�����

@param[in] 
		const char *pcFileName �ļ�·���ַ��� 

@param[in] 
		GR_CHAR * pucFaceName ע�����ݵ��������ƣ��ò����������ã��������ΪNULL�������"default"���档

@param[out] 		
		�� 

@return 		
		�ɹ�ע�᷵��0�����򷵻ط�0ֵ	

@exception
@note
		�ýӿ�ʵ���������ܣ�
		1����pucFaceNameδע��ʱ����Ϊע������ʹ�ã�
		2��pucFaceName��ע�ᣬ������Ӧ��facename�����ݣ�
*/
int GrRegisterFontDataFromFile(const char *pcFileName, GR_CHAR * pucFaceName);

/**
@brief  ��������:		
		��ȡ��ע����ֿ����ݵĸ���

@param[in] 
		��

@param[out] 		
		��

@return 		
		�ɹ���ȡ����ʵ�ʵ��ֿ����ݸ��������򷵻�0	

@exception
@note
*/
int GrGetAvailableFontCnt(void);

/**
@brief  ��������:		
		��ȡָ���������ֿ����ݵ�facename

@param[in] 
		int nFontNameLen�������ĳ���

@param[in] 
		int nIndex ָ��������ֵ����ֵ��ȡֵ��ΧΪ0~GrGetAvailableFontCnt��ȡ���������

@param[out] 		
		char * pcFontName�ò���������������facename��������������ϲ��Ѿ�����ռ�

@return 		
		�ɹ���ȡ����0�����򷵻ط�0ֵ	

@exception
@note
		�ú�������GrGetAvailableFontCnt�ӿ����ʹ�ã���ʹ��GrGetAvailableFontCnt��ȡ���������
		Ȼ���ٻ�ȡÿһ�����������
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
@brief ͨ������ͼƬ�ļ��ĵ�ַ����һ��ͼƬ��ָ����BUFFER

@param[in] pimage:	�û������bitmap��ַ
@param[in] pcFileName : 	ͼƬ�ļ���ַ
@param[in] fnAllocBuffercallback :     �û�����Ļص�����
@param[in] pUserData :     �û�����Ĵ�Ž������ݵ�ַ

@return �ɹ����ؽ������ݵ�ַ,ʧ�ܷ��� NULL
@note  bitmap��Ҫ��Ž�����ȡ��ͼƬ����Ϣ 
*/
unsigned char *GrLoadImgFromFileToBuffer(GR_IMAGE_HDR *pimage, char *pcFileName, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);


/**
@brief ͨ������ͼƬͼƬ���ݵ�ַ�ռ�����һ��ͼƬ��ָ����BUFFER

@param[in] pimage:	�û������bitmap��ַ
@param[in] buffer : 	ͼƬ�ļ��ڴ��ַ
@param[in] size : 	ͼƬ�ļ�����
@param[in] fnAllocBuffercallback :     �û�����Ļص�����
@param[in] pUserData :     �û�����Ĵ�Ž������ݵ�ַ

@return �ɹ����ؽ������ݵ�ַ,ʧ�ܷ��� NULL
@note    bitmap��Ҫ��Ž�����ȡ��ͼƬ����Ϣ 
*/
unsigned char *GrLoadImgFromBufferToBuffer(GR_IMAGE_HDR *pimage,void *buffer, int size, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);


/**
@brief ͨ������ͼƬ�ļ��ĵ�ַ��ָ�������ſ������һ��ͼƬ��ָ����BUFFER

@param[in] pimage:	�û������bitmap��ַ
@param[in] pcFileName : 	ͼƬ�ļ���ַ
@param[in] nWidth :      ����֮���ͼƬ��ȡ�
@param[in] nHeight :     ���� ֮���ͼƬ�߶ȡ�
@param[in] fnAllocBuffercallback :     �û�����Ļص�����
@param[in] pUserData :     �û�����Ĵ�Ž������ݵ�ַ

@return �ɹ����ؽ������ݵ�ַ,ʧ�ܷ��� NULL
@note    bitmap��Ҫ��Ž�����ȡ��ͼƬ����Ϣ 
*/
unsigned char *GrLoadImgFromFileScaledToBuffer(GR_IMAGE_HDR *pimage,char *pcFileName, int nWidth, int nHeight, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);



/**
@brief ͨ������ͼƬ���ݵ�ַ�ռ��ָ�������ſ������һ��ͼƬ��ָ����BUFFER

@param[in] pimage:	�û������bitmap��ַ
@param[in] buffer : 	ͼƬ�ļ��ڴ��ַ
@param[in] size : 	ͼƬ�ļ�����
@param[in] nWidth :      ����֮���ͼƬ��ȡ�
@param[in] nHeight :     ���� ֮���ͼƬ�߶ȡ�
@param[in] fnAllocBuffercallback :     �û�����Ļص�����
@param[in] pUserData :     �û�����Ĵ�Ž������ݵ�ַ

@return �ɹ����ؽ������ݵ�ַ,ʧ�ܷ��� NULL
@note    bitmap��Ҫ��Ž�����ȡ��ͼƬ����Ϣ 
*/
unsigned char *GrLoadImgFromBufferScaledToBuffer(GR_IMAGE_HDR *pimage,void *buffer, int size, int nWidth, int nHeight, GR_AllocBufferCallback_F fnAllocBuffercallback,void  *pUserData);

/**
@brief ͨ������ͼƬ�ļ��ĵ�ַ��ָ�������ſ������һ��ͼƬ

@param[in] pcFileName : ͼƬ�ļ���ַ��
@param[in] nWidth :      ����֮���ͼƬ��ȡ�
@param[in] nHeight :     ���� ֮���ͼƬ�߶ȡ�
@param[in] flags :     ��ʱû�����ã����鴫 0

@return �ɹ����ش���0��ͼƬID,ʧ�ܷ��� -1
@note   ��ͼƬΪJPEG��ʽʱ��nWidth ��nHeight Ϊ�����ͼƬ��RGB���ݿ�͸� ��������nWidth>=0;  nHeight>=0
*/
GR_IMAGE_ID GrLoadImageFromFileScaled(int client_id,  char* pcFileName, int nWidth, int nHeight, int  flag);

/**
@brief �õ���֡APNGͼƬ�ĵ���һ֡����

@param[in] client_id : client id
@param[in] imgid :     ͨ������ӿڵõ���ͼƬId


@return �ɹ����ظ�֡�ĳ���ʱ�䣬ʧ�ܷ��� -1
@note   Ŀǰ��֧��APNG��֡ͼƬ
*/
int GrLoadNextImageFrame(int client_id, GR_IMAGE_ID imgid);

GR_IMAGE_ID GrRotateImage(int client_id, GR_IMAGE_ID imageid, int angle);
GR_IMAGE_ID GrMirrorImage(int client_id, GR_IMAGE_ID imageid);

void GrDrawImageFromBuffer(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
			   GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height, 
			   void *buffer, int size, int flags);


int GrClientSync(int client_id, GR_DRAW_ID id);
/**
@brief ����gif��timer

@param[in] client_id  client_id
@param[in] id     ͼƬ�ľ��
@param[in] bStopFlag       true ��ʾֹͣgif��timer, false��ʾ����gif��timer

@return ��
@note   
*/

void GrControlGifTimer(int client_id, GR_IMAGE_ID id, BOOL bStopFlag);


/**
@brief ��ȡ��ͼƽ�������

@param[in] client_id  client_id
@param[in] nIndex  0 ��ʾ���� 1 ��ʾ����
@param[out] ��
@return pixmapID
@note  ��ȡ��ͼƽ�������,�û���Ҫ�ͷ�pixmapid��
*/

GR_WINDOW_ID GrGetScreenDisplay(int client_id,int index);

/**
	@brief ���Դ���д����

	@param[in] client_id  client_id
	@param[in] id  ���ھ��
	@param[in] gc  gc���
	@param[in] x  x����
	@param[in] y  y����
	@param[in] width  ���
	@param[in] height  �߶�
	@return û�з���ֵ
	@note  ���Դ���д����	
*/
void GrReStoreScreenDisplay(int client_id,GR_DRAW_ID id,GR_GC_ID gc,GR_DRAW_ID source);


/**
@brief ����ͼƬ���ݹ���BMPͼƬ

@param[in] pBitmapMemorySrc ͼƬ���ݵ�ַ�ռ�
@param[in] nMemoryCount     ͼƬ���ݵĴ�С
@param[in] colorDepth       ͼƬ����ɫ��ȣ�1 2 4 8 16 24 32
@param[in] pPalette         ͼƬ��ɫ�����ݵ�ַ����colorDepthС��8ʱ������ΪNULL,��������ΪNULL
@param[in] nPaletteCount    ��ɫ�����
@param[in] w                ͼƬ���
@param[in] h                ͼƬ�߶�
@param[out] nPaletteCount   ����������������BMPBuf�Ĵ�С

@return �ɹ����ع�����BMPBuf�����򷵻�NULL
@note   �ýӿڹ�����BMPBuffer����ʹ�ú�������GrReleaseBMPBuffer�ͷţ���������ڴ�й©
*/
void *GrBuildBMPBuffer(const unsigned char *pBitmapMemorySrc, int nMemoryCount, 
                    unsigned int colorDepth, 
                    const unsigned char *pPalette, int nPaletteCount, int w, int h, int *pnBmpLen);
/**
@brief �ͷ�����ͼƬ���ݹ���BMPͼƬ

@param[in] buffer Ҫ�ͷŵ�ͼƬbuffer

@return ��
*/
void GrReleaseBMPBuffer(void *buffer);


GR_IMAGE_ID GrLoadImageFromBuffer(int client_id,void *buffer, int size, int flags);


/**
@brief ͨ�������ͼƬ����,���Ž���

@param[in] pcImageData : ͼƬ���ݵ��׵�ַ��
@param[in] size :       ͼƬ���ݵĳ��ȡ�
@param[in] nWidth :      ����֮���ͼƬ��ȡ�
@param[in] nHeight :     ���� ֮���ͼƬ�߶ȡ�
@param[in] flags :     ��ʱû�����ã����鴫 0

@return �ɹ����ش���0��ͼƬID,ʧ�ܷ��� -1
@note   ��ͼƬΪJPEG��ʽʱ��nWidth ��nHeight Ϊ�����ͼƬ��RGB���ݿ�͸� ��������nWidth>=0;  nHeight>=0
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
@brief ���ƶ�����������̬gif����̬APNG�ȶ�֡ͼƬ

@param[in] client_id : client ID,GrOpen����GrOpenExt�ķ���ֵ
@param[in] id :        ���Ƶ�Ŀ�괰�ھ����GrNewWindow����GrNewPixmap����ֵ
@param[in] gc :       gc���
@param[in] srcx :     ͼƬ��Ҫ��ʾ�����x����
@param[in] srcy :     ͼƬ��Ҫ��ʾ�����y����
@param[in] srcw :     ͼƬ��Ҫ��ʾ����Ŀ��
@param[in] srch :     ͼƬ��Ҫ��ʾ����ĸ߶�
@param[in] x :     	  Ŀ������x����
@param[in] y :     	  Ŀ������y����
@param[in] width :    Ŀ������Ŀ��
@param[in] height :   Ŀ������ĸ߶�
@param[in] imageid :  Ҫ���ƵĶ�֡ͼƬID,  
@param[in] flag :  	  �Ƿ�Ҫǿ��ˢ�£����鴫0����awt�����⣬�˲�����Ϊ�˼�����ǰ��JavaӦ���еĶ�̬gif

@return ��
@note �ýӿ�ͨ��timer���ƶ�֡ͼƬ����ʾ��ֱ�ӻ����Դ档
@note Ŀǰֻ֧�ֶ�֡gif��ʾ�����APNG��ҪҲ��������ʵ�֣������������
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
@brief  ��������:		
		��ָ����λ����ʾָ�����ַ���

@param[in] 
		int client_id   client��� 

@param[in]
GR_DRAW_ID id ��ʾ���ڴ���ID

@param[in] 
		GR_GC_ID gc ��ͼ�����ľ��

@param[in] 
		GR_COORD x ��ʾ����ʼ���Ͻ�x����

@param[in] 
		GR_COORD y ��ʾ����ʼ���Ͻ�y����

@param[in] 
		void *str ��ʾ���ַ������ݻ�����

@param[in] 
		int count ��ʾ���ַ�������

@param[in] 
		int flags �ַ����ı��뷽ʽ����ҪȡֵΪ��
		GRA_FONT_CODE_ASCII      0x01
		GRA_FONT_CODE_UTF8       0x02
		GRA_FONT_CODE_UNICODE    0x04

@param[out] 		
��

@return 		
		��	

@exception
@note
*/
void GrText(int client_id,GR_DRAW_ID id, GR_GC_ID gc, 
	    GR_COORD x, GR_COORD y, 
	    void *str, GR_COUNT count, int flags);
/*cursor APIs*/

/*************************************************/
/**
@brief  ��������:		
		��ָ����λ����ʾָ�����ַ���

@param[in] 
		int client_id   client��� 

@param[in]
GR_DRAW_ID id ��ʾ���ڴ���ID

@param[in] 
		GR_GC_ID gc ��ͼ�����ľ��

@param[in] 
		GR_POINT *charPos ÿ���ַ�������������

@param[in] 
		void *str ��ʾ���ַ������ݻ�����

@param[in] 
		int count ��ʾ���ַ�������

@param[in] 
		int flags �ַ����ı��뷽ʽ����ҪȡֵΪ��
		GRA_FONT_CODE_ASCII      0x01
		GRA_FONT_CODE_UTF8       0x02
		GRA_FONT_CODE_UNICODE    0x04

@param[out] 		
��

@return 		
		��	

@exception
@note
		�ýӿ���GrText������������ڣ��ýӿڱ���ָ��ÿ���ַ������꣬��������ʵ���Ű�Ķ����ԡ���Ҫ�������ʹ�ã����뷨�ĺ�ѡ��Ҳ�ǵ��øýӿڽ�����ʾ�ġ�
*/
int GrTextEx(int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_POINT *charPos, void *str, GR_COUNT count, int flags);

/**
@brief  ��������:
		��ȡָ���ַ�����ģ���ݣ���ģ����Ϊ�Ҷȼ������ݣ���ÿ��������1���ֽڱ�ʾ
 
@param[in]
		int client_id client���

 @param[in]
		GR_FONT_ID fontid ������

 @param[in]
		unsigned short usCharCode �ַ��ı��룬������UNICODE���뷽ʽ
		
 @param[in]
		int nMemSize������ڴ滺������Сֵ����ֵ����ΪpFontDataMem��ʵ�ʴ�С
 
@param[out] 
		UCHAR *pFontDataMem �����ڴ滺���������ڱ����ȡ�����ַ�������ģ���ݣ�
		��黺���������и�ʽ:
		1��ÿ�е��ֽڸ���ΪGC������������õĴ�С��
		2��������ΪGC������������õĴ�С��
		3����黺�����Ĵ�С������ڵ��� (fontsize * fontsize);
		�����Ϊ�գ��򲻻�ȡ��ģ����

@param[out] 
 		int * pnWidth ��ȡ�ַ���ʵ�ʿ�ȣ������Ϊ�գ��򲻻�ȡ��ֵ

 @param[out] 
 		int * pnHeight ��ȡ�ַ���ʵ�ʸ߶ȣ������Ϊ�գ��򲻻�ȡ��ֵ
		
@return
 		��
@exception
@note
*/
void GrGetCharFontData(int client_id, GR_FONT_ID fontid, unsigned short usCharCode, UCHAR *pFontDataMem, int nMemSize, int * pnWidth, int  * pnHeight);
/*************************************************/

/**
@brief  ��������:
		����OSG�����ʾ����
 
@param[in]
		int nType ����osg������
		����ȡֵ����Ϊ:
		GRA_DISPLAYOPTION_COLOR  ΪOSG��ʾ���
		GRA_DISPLAYOPTION_3D ΪOSG֧��3DTV
		

 @param[in]
		int nValue ���õĶ�Ӧ����nType��ֵ
		��nTypeΪGRA_DISPLAYOPTION_COLORʱ��
			nValue ֵΪ1,��ʾ����OSG���
			nValue ֵΪ0,��ʾ����OSG�ָ�����
			
		��nTypeΪGRA_DISPLAYOPTION_3Dʱ��
			nValue ֵΪ0,��ʾOSG������ʾ
			nValue ֵΪ1,��ʾ��OSGһ��Ϊ��������ʾ
			nValue ֵΪ2,��ʾ��OSGһ��Ϊ��������ʾ

 @param[out] 
 		0��ʾ�ɹ���-1��ʾʧ��
		
@return
 		��
@exception
@note
*/
int GrSetDisplayOption(int client_id, int nType, int nValue);

/**
@brief ����Cursor��Դ�����ļ���

@param[in] client_id :  client���
@param[in] pcDirPath : Ҫ������Դ�ļ���·������"/workdir/arrow/SkyWorks/"

@return �ɹ����÷���0�����򷵻�-1
@note   �˺��������ڼ���curosr��Դ֮ǰ���ã�������Ч
		client_id��ʱδ�õ������Բ�����
*/
int GrSetCursorResPath(int client_id, const char * pcDirPath);

/**
@brief �������

@param[in] client_id :  client���
@param[in] width ���Ϊ32
@param[in] height �߶�Ϊ32
@param[in] hotx �ȵ��x����
@param[in] hoty �ȵ��y����
@param[in] pCursorBits �����ͼƬ����

@return �ɹ����÷���0�����򷵻�-1
@note
*/
GR_CURSOR_ID GrNewCursor(int client_id,GR_SIZE width, GR_SIZE height, 
			 GR_COORD hotx, GR_COORD hoty, 
			 unsigned char  * pCursorBits);

/**
@brief ��ʾ���

@param[in] xoffset x����ƫ��(������ϴ���ʾλ��)
@param[in] yoffset y����ƫ��(������ϴ���ʾλ��)
            
@return 
        ��
@note
*/
void GrShowCursor(int xoffset, int yoffset);
/**
@brief  ��������:
		���ٹ��
 
@param[in]
		client_id: client���
		cid: ���ID
 
@param[out] 
 
@return
 		��
@exception
@note   ���ͷŹ����Դbitmap
*/
void GrDestroyCursor(int client_id,GR_CURSOR_ID cid);

/**
@brief  ��������:
		Ϊ��ǰ�������ù��
 
@param[in]
		client_id: client���
		wid: ���ھ��
		cid: ���ID
		���cid�Ƿ�����ʹ��Ĭ�Ϲ����ʽ
 
@param[out] 
 
@return
 		��
@exception
@note
*/
void GrSetWindowCursor(int client_id,GR_WINDOW_ID wid, GR_CURSOR_ID cid);

/**
@brief  ��������:
		�ƶ���굽Ŀ��λ��
 
@param[in]
		x: Ŀ��x����
		y: Ŀ��y����
		���x,y������Ļ��Χ��������x,y���ƶ�������ʾ�ڱ߽�λ��
 
@param[out] 
 
@return
 		��
@exception
@note
*/
void GrMoveCursor(int client_id,GR_COORD x, GR_COORD y);

/**
@brief  ��������:
		��ȡCursorId
 
@param[in]
        client_id: client���
		index: �����������
		���index >= E_MAX_COUNT_CUROSR ����index = 0��Ӧ�Ĺ��ID
		index < E_MAX_COUNT_CUROSR      ����index��Ӧ�Ĺ��ID
 
@param[out] 
 
@return
 		�ɹ�����CursorId, ʧ�ܷ���-1
@exception
@note
*/
int GrGetCursorId(int client_id,int index);

/**
@brief  ��������:
		�����꣬ʹ��ɼ�����δʵ��
 
@param[in]
 
@param[out] 
 
@return

@exception
@note
*/
void GrActivateCursor(void);

/**
@brief  ��������:
		�������꣬���䲻�ɼ�����δʵ��
 
@param[in]
 
@param[out] 
 
@return

@exception
@note
*/
void GrDeactivateCursor(void);

/**
@brief  ��������:
        ���ù�걣����ʾ��ʱ�䣨�Զ����ع�깦��Ĭ�Ͽ���,�û��ɴ���-1ȡ���Զ����أ�

@param[in]
int client_id  client���
int nTime      ��걣����ʾ��ʱ�䣬��λΪms,nTimeΪ-1�����Զ����ع��

@return
BOOL   �ɹ�����TRUE
       ʧ�ܷ���FALSE

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
@brief ����ĳ��������Ϊ��ռ����

@param[in] client_id :  �ͻ���ID
@param[in] psGrapRect : Ҫ���õ�����ΪNULLʱ��ʾȡ������

@return ��
@note   �˺���ֻ�Զ�֡gif��ˢ��������
*/
void GrSetHogRect(int client_id, GR_RECT *psGrapRect);
void GrVersion(char* version, int len);


/**
*  ��������:
*		ICON��ģ���ʼ��, ��AddIcon֮ǰ, ��Ҫ�ӳ�ʼ��Iconģ��
*
* \param[in]
*		��
*
* \param[out] 
*		��
*
* \return
*		��ֵ : ִ�гɹ�
*         -1 : ִ��ʧ��
*  \exception
*  \note
*/
int GrIconInitial();

/**
*  ��������:
*		����ICON��ģ��.
*
* \param[in]
*		��
*
* \param[out] 
*		��
*
* \return
*		��ֵ : ִ�гɹ�
*         -1 : ִ��ʧ��
*  \exception
*  \note
*/
int GrIconRelease();

/**
*  ��������:
*		��������ICON���ݼ��ص�ϵͳ��.
*
* \param[in]
*		MW_ICON_S sIcon   : ��Ҫ���ص�ICON�ṹ������
*
* \param[out] 
*		��
*
* \return
*		��ֵ : ICON ID
*         -1 : ִ��ʧ��
*  \exception
*  \note
*/
int GrAddIcon(MW_ICON_S *psIcon);

/**
*  ��������:
*		��ϵͳ����ȥָ��ID��ICON.
*
* \param[in]
*		int nIconId 		: ��Ҫ��ȥ��ICON ID
*
* \param[out] 
*		��
*
* \return
*		   0 : ִ�гɹ�
*         -1 : ִ��ʧ��
*  \exception
*  \note
*/
int GrRemoveIcon(int nIconId);

/**
*  ��������:
*		��ʾָ��ID��ICON.
*
* \param[in]
*		int nIconId 		: ��Ҫ��ʾ��ICON ID
*
* \param[out] 
*		��
*
* \return
*		   0 : ִ�гɹ�
*         -1 : ִ��ʧ��
*  \exception
*  \note
*/
int GrShowIcon(int nIconId);


/**
*  ��������:
*		����ָ��ID��ICON.
*
* \param[in]
*		int nIconId 		: ��Ҫ���ص�ICON ID
*
* \param[out] 
*		��
*
* \return
*		   0 : ִ�гɹ�
*         -1 : ִ��ʧ��
*  \exception
*  \note
*/
int GrHideIcon(int nIconId);

/**
*  ��������:
*		�����Ƿ��3D��֧�м���ͼƬ.
*
* \param[in]
*		bool b_LoadImgFrom3DFlag		: trueΪ�Ǵ�3D��֧�м���ͼƬ;false Ϊ��
*
* \param[out] 
*		��
*
* \return
*		��  
*  \exception
*  \note
*   Ӧ�ó���:1. ���3D�к�ɫ����ɫ��ɫ�����
*                         2. ����֮ǰ����bLoadImgFrom3DFlagΪtrue
*                         3. ���ý���ӿ�
*                         4. ����֮������bLoadImgFrom3DFlagΪfalse
*/
void GrSetLoadImgFrom3DFlag(BOOL bLoadImgFrom3DFlag);

/**
@brief  ��������:
		��ͼ�����ݱ�����û�ָ���ĸ�ʽ
 
@param[in]
		pstImage  				  ��ת����ԭʼͼ�����ݣ���ϸ��μ�mwtypes.h�еĶ���,����: 
								w/h/pitch=w*4/format/pixels=argb_buffer_addr
		nDstImageType				  �û���Ҫת����Ŀ��ͼƬ��ʽ����֧��GRA_BITMAP_SOURCE_JPEG���Ժ������չ֧��
								GRA_BITMAP_SOURCE_AUTOGIF��GRA_BITMAP_SOURCE_APNG��GRA_BITMAP_SOURCE_PNG��������μ�mwtypes.h��
 
@param[out] 
		nDstImgFileHandle			  ����ļ����,���ת�����ͼƬ����
 
@return
 		�����ɹ����򷵻�0�����򷵻�-1
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
