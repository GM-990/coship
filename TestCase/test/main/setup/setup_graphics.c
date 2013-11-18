#include "setup.h"

#ifdef MMCP_UDI2
#include "udi2_osg.h"
#else
#include "cs_osg.h"
#endif
/*#include "CSAppRC.c"*/
/*#include "Font.c"*/
#ifdef TKMODULE_graphics
 
#include "CS_GUIStartUp.h"
#define GDIDATALEN			(0x90000)
#define OSD_WIDTH			720
#define OSD_HEIGHT			576
#define OSD_BITS				32

static BYTE 		g_aucGDIData[GDIDATALEN];

INT_PTR CALLBACK MainWndProc(HCSWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
BOOL CSInitWinApp(HCSINSTANCE hInstance, int nCmdShow)
{
	HCSWND hWndMain;
	CSWNDCLASSEX wcex;

	wcex.cbSize			= sizeof(wcex);
	wcex.style			= CS_DBLCLKS;
	wcex.lpfnWndProc	= (CSWNDPROC)MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= (HCSICON)NULL;
	wcex.hCursor		= (HCSCURSOR)NULL;
	wcex.hbrBackground	= CSCreateSolidBrush(RGB(255,0,0));
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "GUIAppClass";
	wcex.hIconSm		= (HCSICON)NULL;

	if(!CSRegisterClassEx(&wcex))
		return FALSE;

	hWndMain = CSCreateWindow("GUIAppClass", "GUIApp", WS_VISIBLE, 0, 0, 720, 576, 0, 0, hInstance, 0);
	if(hWndMain == (HCSWND)NULL)
		return FALSE;

	return TRUE;
}

int CSAPPMain(HCSINSTANCE hInstance, HCSINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
	CSMSG msg;

	if(CSInitWinApp(hInstance, nCmdShow) == FALSE)
		return 0;

	while(CSGetMessage(&msg, (HCSWND)NULL, 0, 0))
	{
		if(!CSIsDialogMessage(msg.hwnd,&msg))
		{
			CSDispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

static DWORD GUI_OSDBufferExchange(CSRECT* pRect)
{
	CSOSGRect rect;
	
	rect.left = pRect->left;
	rect.top = pRect->top;
	rect.Width = pRect->right- pRect->left;
	rect.Height = pRect->bottom-pRect->top;
	
	CSOSGBufferExchange(CSLAYER_GFX1, 0, &rect);
	return 0;
}
extern unsigned char CSAppRC_h[];
extern unsigned char Font_h[];
extern int CSAppRC_h_size;
extern int Font_h_size;
void SetupGraphics(void)
{
	static BOOL bRet = FALSE;
	
	int nRCData[2];
	int nDataType[2];
	int dwRcDataLen[2];

	if (bRet)
		return;

	nDataType[0] = 1;
	nDataType[1] = 2;

	dwRcDataLen[0] = CSAppRC_h_size;
	nRCData[0] = (int)CSAppRC_h;
	
	dwRcDataLen[1] = Font_h_size;
	nRCData[1] = (int)Font_h;

	//g_pucScreenBitmap = (BYTE*)OSDUtils_GetOSDBitmap();
	SetScreenHardProperty((BYTE*)CSOSGGetScreenAddress(CSLAYER_GFX2, 0), OSD_WIDTH, OSD_HEIGHT, OSD_BITS);
	SetScreenChangeNotify(GUI_OSDBufferExchange);

	SetScreenPixelFormat(PixelFormat32bppARGB);
	//SetScreenPixelFormat(PixelFormat16bppARGB1555);
	CSTRACE(3,"\n==## now InitNewCOSHIPGUI!\n");
	bRet = InitNewCOSHIPGUI(g_aucGDIData, GDIDATALEN, (DWORD*)nRCData, (DWORD*)dwRcDataLen, (DWORD*)nDataType, 2);

	if (bRet)
	{
	
		StartCOSHIGUIAppEx(CSAPPMain, 2*17,  256*1024);
	}
	else
	{
	}	

	CSASSERT(bRet);
}
#endif

