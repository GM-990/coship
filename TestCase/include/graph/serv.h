#ifndef	_SERV_H
#define	_SERV_H
/*
 * Copyright (c) 2000 Greg Haerr <greg@censoft.com>
 * Copyright (c) 2000 Alex Holden <alex@linuxhacker.org>
 * Copyright (c) 1991 David I. Bell
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 *
 * Private definitions for the graphics server.
 * These definitions are not to be used by clients.
 */
//#include "mwsys.h"//xiachao
#include <pthread.h>
#include <mwtypes.h>
#include <mwinput.h>
//#include <nano-X.h>//xiachao
#include <mwapi.h>//xiachao
#include "mwengine.h"

/*
#define DYNAMICREGIONS  1 
*/
/* =1 to use GRACLIPREGIONS*/
/*
 * Drawing types.
 */
typedef	int	GR_DRAW_TYPE;

#define	GR_DRAW_TYPE_NONE	0	/* none or error */
#define	GR_DRAW_TYPE_WINDOW	1	/* windows */
#define	GR_DRAW_TYPE_PIXMAP	2	/* pixmaps */

#define	GR_MAX_MODE		CS_MODE_ALPHA

//add 2010.09.16
#define GR_WINDOW_TYPE_ROOT     1  /* root */
#define GR_WINDOW_TYPE_TOP   2  /* sreen */
#define GR_WINDOW_TYPE_ORDINARY 3  /* ordinary */
typedef struct gr_client GR_CLIENT;

/*
 * List of elements for events.
 */
typedef struct gr_event_list GR_EVENT_LIST;
struct gr_event_list {
	GR_EVENT_LIST	*next;		/* next element in list */
	GR_EVENT	event;		/* event */
};
/*
 * Data structure to keep track of state of clients.
 */
typedef struct _PSDLIST {
	struct _GRA_Screen *psd;
	struct _PSDLIST *next;
} PSDLIST, *PPSDLIST;


/*
 * Structure to remember clients associated with events.
 */
typedef	struct gr_event_client	GR_EVENT_CLIENT;
struct gr_event_client	{
	UCHAR 		used;
	GR_EVENT_MASK	eventmask;	/* events client wants to see */
	GR_CLIENT	*client;	/* client who is interested */
};
/*
 * Structure to remember graphics contexts.
 */
typedef	struct gr_gc	GR_GC;
struct gr_gc {
	CSGraphGC mwgc;
	GR_REGION_ID	regionid;	/* current clipping region */
 	int             xoff;           /* X offset for the clip region */
	int             yoff;           /* Y offset for the clip region */
	GR_BOOL		usebackground;	/* actually display the background */
	GR_BOOL		changed;	/* graphics context has been changed */
	GR_CLIENT 	*owner;		/* client that created it */
	GR_GC		*next;		/* next graphics context */
};

/*
 * Structure to remember regions.
 */
typedef struct gr_region	GR_REGION;
struct gr_region {
	GRACLIPREGION *	rgn;
	GR_REGION_ID	id;
	GR_CLIENT *	owner;		/* client that created it */
	GR_REGION *	next;
};
 
/*
 * Structure to remember fonts.
 */
typedef struct gr_font	GR_FONT;
struct gr_font {
	int		fontsize;	/* font size*/
	GR_FONT_ID	id;		/* font id*/
	GR_CLIENT *	owner;		/* client that created it */
	GR_FONT *	next;		/* next font*/
};

/*
 * Structure to remember images.
 */
typedef struct gr_image	GR_IMAGE;
struct gr_image {
	GR_IMAGE_ID	id;
	GR_CLIENT *	owner;		/* client that created it */
	GR_IMAGE *	next;
/*for PICMERGEBMP*/
	GR_IMAGE_ID	id2;
	GRA_Bitmap *pbitmap;
};
 
/*
 * Cursor structure.
 */
typedef struct gr_cursor GR_CURSOR;
struct gr_cursor {
	GR_CURSOR_ID	id;		/* cursor id*/
	GR_CLIENT	*owner;		/* client that created it*/
	GR_CURSOR	*next;
	GRACURSOR	cursor;		/* mwin engine cursor structure*/
};

/*
 * Structure to remember selection owner and mime types it can produce.
 */
typedef struct {
	GR_WINDOW_ID wid;
	GR_CHAR *typelist;
} GR_SELECTIONOWNER;


/*
 * Structure to remember timers.
 */
typedef struct gr_timer GR_TIMER;
struct gr_timer 
{
    GR_TIMER_ID    id;       /* This instances ID */
    GR_CLIENT     *owner;    /* client that created it */
    GR_WINDOW_ID   wid;
    GRA_Timer 		*timer;    /* Device independent layer timer */
    GR_TIMER      *next;
};

/*
 * Drawable structure.  This structure must be the first
 * elements in a GR_WINDOW or GR_PIXMAP, as GrPrepareWindow
 * returns a GR_DRAWABLE.  This structure includes
 * only those items that routines that use GrPrepareWindow
 * might dereference, and must be included in windows or pixmaps.
 */
typedef struct gr_drawable {
	GR_COORD	x;		/* x position (0)*/
	GR_COORD	y;		/* y position (0)*/
	GR_SIZE		width;		/* width */
	GR_SIZE		height;		/* height */
	/*
        *change the type of psd pointer from _mwscreen to _GRA_Screen struct
        *by shchen --- 02-11-26
        */
        struct _GRA_Screen *psd;    /* associated screen device */
       GR_WINDOW_ID	id;		/* window/pixmap id */
} GR_DRAWABLE;

/*
 * Window structure
 * Note: first elements must match GR_DRAWABLE
 */
#define MAX_NUM_EVENT_CLIENTS 10
typedef struct gr_pixmap GR_PIXMAP;
typedef struct gr_window GR_WINDOW;
struct gr_window {
  GR_COORD	x;		/* absolute x position */
  GR_COORD	y;		/* absolute y position */
  GR_SIZE		width;		/* width */
  GR_SIZE		height;		/* height */
  /*
        *change the type of psd pointer from _mwscreen to _GRA_Screen struct
        *by shchen --- 02-11-26
        */
  struct _GRA_Screen *psd; /*the surface structure pointer that is associated to the window*/
  GR_WINDOW_ID	id;		/* window id */
  /* end of GR_DRAWABLE common members*/
  
  GR_WINDOW	*next;		/* next window in complete list */
  GR_CLIENT	*owner;		/* client that created it */
  GR_WINDOW	*parent;	/* parent window */
  GR_WINDOW	*children;	/* first child window */
  GR_WINDOW	*siblings;	/* next sibling window */
  GR_SIZE		bordersize;	/* size of border */
  GR_COLOR	bordercolor;	/* color of border */
  GR_COLOR	background;	/* background color */
  GR_PIXMAP	*bgpixmap;	/* background pixmap */
  PSD	str_bgpixmap;	/* background pixmap psd for stretch mode */
  int		bgpixmapflags;	/* center, tile etc. */
  GR_EVENT_MASK	nopropmask;	/* events not to be propagated */
  GR_EVENT_CLIENT	eventclients[MAX_NUM_EVENT_CLIENTS];	/* clients interested in events */
  GR_CURSOR_ID	cursorid;	/* cursor for this window */
  GR_BOOL		mapped;		/* TRUE if explicitly mapped */
  GR_COUNT	unmapcount;	/* count of reasons not really mapped */
  GR_BOOL		output;		/* TRUE if window can do output */
  GR_WM_PROPS	props;		/* window properties*/
  GR_CHAR		*title;		/* window title*/
  int 		type; /* root=1, screen=2, ordinary=3 */
};

/*
 * Pixmap structure
 * Note: first elements must match GR_DRAWABLE
 */
struct gr_pixmap {
	GR_COORD	x;		/* x position (0)*/
	GR_COORD	y;		/* y position (0)*/
	GR_SIZE		width;		/* width */
	GR_SIZE		height;		/* height */
        struct _GRA_Screen *psd;    /* associated screen device */
	GR_WINDOW_ID	id;		/* pixmap id */

	GR_PIXMAP	*next;		/* next pixmap in list */
	GR_CLIENT	*owner;		/* client that created it */
};

struct gr_client_private {
GR_TIMER_ID     cache_timer_id;         /* cached timer ID */
GR_TIMER        *cache_timer;           /* cached timer */
GR_TIMER        *list_timer ;            /* list of all timers */
GR_WINDOW_ID    cachepixmapid;         /* cached pixmap id */

GR_GC_ID	cachegcid;		/* cached graphics context id */
GR_GC		*cachegcp ;		/* cached graphics context */
GR_GC		*curgcp ;		/* currently enabled gc */
GR_GC		*listgcp ;		/* list of all gc */

GR_PIXMAP       *cachepp ;               /* cached pixmap */
GR_PIXMAP       *listpp ;                /* List of all pixmaps */
GR_REGION	*listregionp ;		/* list of all regions */
GR_FONT		*listfontp ;		/* list of all fonts */
GR_IMAGE	*listimagep ;		/* list of all images */

GR_CURSOR	*listcursorp ;		/* list of all cursors */
GR_CURSOR	*curcursor ;		/* currently enabled cursor */

GR_EVENT_LIST	*eventfree;		/* list of free events */

};
typedef struct gr_client_private GR_CLIENT_PRIVATE;

struct gr_client {
	int		id;		/* client id and socket descriptor */
	GR_EVENT_LIST	*eventhead;	/* head of event chain (or NULL) */
	GR_EVENT_LIST	*eventtail;	/* tail of event chain (or NULL) */
	GR_CLIENT	*next;		/* the next client in the list */
	GR_CLIENT	*prev;		/* the previous client in the list */
	char		*shm_cmds;
	int		shm_cmds_size;
	int		shm_cmds_shmid;
	
	pthread_t threadid;	/**<the threadid of the client*/
	GRA_Mutex *evt_list_mutex;	/**<mutex of client event list*/
	GR_CLIENT_PRIVATE private_data; /*client private data*/

	GRA_Display *display;
	GRA_Screen *psd;
	
	pthread_mutex_t event_mutex;	
	pthread_cond_t event_cond;
};

/*
 * Graphics server routines.
 */
int		GsInitialize(void);
void		GsClose(int client_id);
int             GsCreateScreen(int xoff, int yoff, int width, int height);
void            GsDestroyScreen(int id);
void		GsSelect(GR_TIMEOUT timeout);
void		GsTerminate(void);
GR_TIMEOUT	GsGetTickCount(void);
void		GsRedrawScreen(int client_id);
void		GsError(int client_id,GR_ERROR code, GR_ID id);
GR_BOOL		GsCheckMouseEvent(void);
GR_BOOL		GsCheckKeyboardEvent(void);
int		GsReadKeyboard(GR_CHAR *buf, int *modifiers);
int		GsOpenKeyboard(void);
void		GsGetButtonInfo(int *buttons);
void		GsGetModifierInfo(int *modifiers);
void		GsCloseKeyboard(void);
void		GsExposeArea(GR_WINDOW *wp, GR_COORD rootx, GR_COORD rooty,
			GR_SIZE width, GR_SIZE height, GR_WINDOW *stopwp);
void		GsCheckCursor(void);
void		GsWpSetFocus(GR_WINDOW *wp);
void		GsWpDrawBackgroundPixmap(GR_WINDOW *wp, GR_PIXMAP *pm,
			GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height);
void		GsWpTileBackgroundPixmap(GR_WINDOW *wp, GR_PIXMAP *pm,
			GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height);

/** Modified by jiangfeng 2008.5.15 */
/** Add bFillFlag paramter */
void		GsWpClearWindow(GR_WINDOW *wp, GR_COORD x, GR_COORD y,
			GR_SIZE width, GR_SIZE height, GR_BOOL exposeflag, GR_BOOL bFillFlag);

void		GsWpUnmapWindow(GR_WINDOW *wp, GR_BOOL temp_unmap);
void		GsWpMapWindow(GR_WINDOW *wp, GR_BOOL temp);
void		GsWpDestroyWindow(GR_WINDOW *wp);
void		GsSetPortraitMode(int mode);
void		GsSetPortraitModeFromXY(GR_COORD rootx, GR_COORD rooty);
void		GsSetClipWindow(GR_WINDOW *wp, GRACLIPREGION *userregion,
			int flags);
void GsHandleMouseStatus(GR_COORD newx, GR_COORD newy, int nEventType, int newbuttons, GRAKEYMOD modifiers /* latest modifiers */);
void		GsFreePositionEvent(GR_CLIENT *client, GR_WINDOW_ID wid,
			GR_WINDOW_ID subwid);
void		GsDeliverButtonEvent(GR_EVENT_TYPE type, int buttons,
			int changebuttons, int modifiers);
void		GsDeliverMotionEvent(GR_EVENT_TYPE type, int buttons,
			GRAKEYMOD modifiers);
void		GsDeliverKeyboardEvent(GR_WINDOW_ID wid, GR_EVENT_TYPE type,
			GR_KEY keyvalue, GR_KEYMOD modifiers,
			GR_SCANCODE scancode);
void		GsDeliverExposureEvent(GR_WINDOW *wp, GR_COORD x, GR_COORD y,
			GR_SIZE width, GR_SIZE height);
void		GsFreeExposureEvent(GR_CLIENT *client, GR_WINDOW_ID wid,
			GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height);
void		GsDeliverUpdateEvent(GR_WINDOW *wp, GR_UPDATE_TYPE utype,
			GR_COORD x, GR_COORD y, GR_SIZE width, GR_SIZE height);
void		GsDeliverGeneralEvent(GR_WINDOW *wp, GR_EVENT_TYPE type,
			GR_WINDOW *other);
void		GsDeliverPortraitChangedEvent(void);
void		GsDeliverScreenSaverEvent(GR_BOOL activate);
void		GsDeliverClientDataReqEvent(GR_WINDOW_ID wid, GR_WINDOW_ID rid,
			GR_SERIALNO serial, GR_MIMETYPE mimetype);
void		GsDeliverClientDataEvent(GR_WINDOW_ID wid, GR_WINDOW_ID rid,
			GR_SERIALNO serial, GR_LENGTH len, GR_LENGTH thislen,
			void *data);
void		GsDeliverSelectionChangedEvent(GR_WINDOW_ID old_owner,
			GR_WINDOW_ID new_owner);
void		GsDeliverTimerEvent(GR_CLIENT *client, GR_WINDOW_ID wid,
			GR_TIMER_ID tid);

void        GsDeliverResolutionChangedEvent(int clientid, int x, int y, 
            int w, int h);

void		GsCheckMouseWindow(void);
void		GsCheckFocusWindow(void);
GR_DRAW_TYPE	GsPrepareDrawing(int client_id,GR_DRAW_ID id, GR_GC_ID gcid,
			GR_DRAWABLE **retdp);
GR_BOOL		GsCheckOverlap(GR_WINDOW *topwp, GR_WINDOW *botwp);
GR_EVENT	*GsAllocEvent(GR_CLIENT *client);
GR_WINDOW	*GsFindWindow(GR_WINDOW_ID id);
GR_PIXMAP 	*GsFindPixmap(int client_id,GR_WINDOW_ID id);
GR_GC		*GsFindGC(int client_id,GR_GC_ID gcid);
GR_REGION	*GsFindRegion(int client_id,GR_REGION_ID regionid);
GR_FONT 	*GsFindFont(int client_id,GR_FONT_ID fontid);
GR_TIMER	*GsFindTimer(int client_id,GR_TIMER_ID timer_id);
GR_CURSOR 	*GsFindCursor(int client_id,GR_CURSOR_ID cursorid);

GR_PIXMAP 	*GsFindClientPixmap(GR_CLIENT* cp,GR_WINDOW_ID id);
GR_GC		*GsFindClientGC(GR_CLIENT* cp,GR_GC_ID gcid);
GR_REGION	*GsFindClientRegion(GR_CLIENT* cp,GR_REGION_ID regionid);
GR_FONT 	*GsFindClientFont(GR_CLIENT* cp,GR_FONT_ID fontid);
GR_TIMER	*GsFindClientTimer(GR_CLIENT* cp,GR_TIMER_ID timer_id);
GR_CURSOR 	*GsFindClientCursor(GR_CLIENT* cp,GR_CURSOR_ID cursorid);

GR_WINDOW	*GsPrepareWindow(GR_WINDOW_ID wid);
GR_WINDOW	*GsFindVisibleWindow(GR_COORD x, GR_COORD y);
void		GsDrawBorder(GR_WINDOW *wp);
int		GsCurrentVt(void);
void		GsRedrawVt(int t);
int		GsOpenSocket(void);
void		GsCloseSocket(void);
void		GsAcceptClient(void);
int		GsAcceptClientFd(int i, GRA_Display *display, GRA_Screen *psd);
int		GsPutCh(int fd, unsigned char c);
GR_CLIENT	*GsFindClient(int client_id);
void		GsDestroyClientResources(GR_CLIENT * client);
void		GsDropClient(int client_id);
int		GsRead(int fd, void *buf, int c);
int		GsWrite(int fd, void *buf, int c);
void		GsHandleClient(int fd);
void		GsResetScreenSaver(void);
void		GsActivateScreenSaver(void *arg);
void		GrGetNextEventWrapperFinish(int);
void        GsTimerCB(void *arg);
void 	GsRaiseWindow(GR_WINDOW *wp);
void 	GsFreeImage(GR_CLIENT *clientp, GR_IMAGE_ID id);
void 	GsDestroyCursor(GR_CLIENT * clientp, GR_CURSOR * cursorp);
void 	GsDestroyFont(GR_CLIENT * clientp, GR_FONT * fontp);
void 	GsDestroyGC(GR_CLIENT * clientp, GR_GC * gcp);
void 	GsDestroyPixmap(GR_CLIENT * clientp, GR_PIXMAP * pp);
void 	GsDestroyRegion(GR_CLIENT * clientp, GR_REGION * regionp);
void 	GsDestroyTimer(GR_CLIENT * clientp, GR_TIMER * timer);
void 	GsFreeImage(GR_CLIENT * clientp, GR_IMAGE_ID id);

void 	GsDefaultGCAttrb(GR_GC *gcp);
void 	GsSetCursor(GRACURSOR cursor) ;
void GsLowerWindowRefresh(GR_WINDOW *expwp, GR_WINDOW *wp) ;
void GsClearBackgroundPixmap(GR_WINDOW_ID wid);
GR_CURSOR* GsGetCurrentCursor(void);
void GsSetCurrentCursor(GR_CURSOR *curcursor);
void GsMoveCursor(int newx, int newy);
void GsShowCursor(GR_CLIENT *clientp, GR_CURSOR *psCursor);

/*
 * External data definitions.
 */
extern GR_WINDOW	 *rootwp;
extern GR_WINDOW_ID	  cachewindowid;		/* cached window id */
extern GR_WINDOW	*cachewp ;		/* cached window pointer */
extern GR_WINDOW	*listwp ;		/* list of all windows */
extern GR_WINDOW	*clipwp ;		/* window clipping is set for */
extern GR_WINDOW	*focuswp ;		/* focus window for keyboard */
extern GR_WINDOW	*mousewp  ;		/* window mouse is currently in */
extern GR_WINDOW	*grabbuttonwp  ;		/* window grabbed by button */

extern int		current_fd;		/* the fd of the client talking to */
extern int		connectcount ;	/* number of connections to server */

/*client list pointers*/
extern GR_CLIENT	*root_client  ;		/* root entry of the client table */

extern int		keyb_fd;		/* the keyboard file descriptor */
extern int		mouse_fd;		/* the mouse file descriptor */
extern char		*curfunc  ;		/* the name of the current server func*/

extern GR_TIMEOUT	screensaver_delay;	/* time before screensaver activates */
extern GR_BOOL		screensaver_active;	/* time before screensaver activates */
extern GR_SELECTIONOWNER selection_owner;	/* the selection owner and typelist */
extern int		autoportrait ;	/* auto portrait mode switching*/

extern GR_CURSOR	*stdcursor  ;		/* root window cursor */
extern GR_COORD	cursorx  ;		/* current x position of cursor */
extern GR_COORD	cursory ;		/* current y position of cursor */
extern int		curbuttons;		/* current state of buttons */

extern GR_BOOL		focusfixed;    /* TRUE if focus is fixed on a window */
extern char		*progname  ;		/* Name of this program.. */
extern GR_GC gr_default_gc ;
extern GR_FONT gr_default_font;
extern int screen_saver_timer_id;

/*
 * The filename to use for the named socket. If we ever support multiple
 * servers on one machine, the last digit will be that of the FB used for it.
 */
#define GR_NAMED_SOCKET "/tmp/.mw"
#define GR_NUMB_SOCKET 79

#ifndef M_PI

#define M_PI            3.14159265358979323846
#define M_PI_2          1.57079632679489661923
#define M_PI_4          0.78539816339744830962
#define M_1_PI          0.31830988618379067154
#define M_2_PI          0.63661977236758134308
#define M_SQRT2         1.41421356237309504880
#define M_SQRT1_2       0.70710678118654752440

#endif


#endif /* _SERV_H*/
