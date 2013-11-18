
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
#include "csshell_font.h"
#ifdef MMCP_QT
#include "prepare_qt.h"
#endif

#if 0 
void PrepareTestGraphics(void)
{
#ifdef TKMODULE_graphics	
    
	//SetupPortingDriver();
	udi_init();
	SetupGraphics();
#endif
    
}
#endif

void PrepareTestGraph(void)
{
#ifndef TKMODULE_graphics	
	//SetupPortingDriver();
	udi_init();
#ifdef MMCP_QT
    CSTKInitQT();
    CSTKExecQT();
#endif
	CSShellFontInitStfFont(CSShellFontGetDefaultStfFont());
	CSWMInit();
	GsInitialize();
#endif
}

void PrepareTestJvm(int argc, char *argv[])
{ 	
#ifndef TKMODULE_graphics		
	
	/* ��ʼ������*/
	//SetupPortingDriver();
	udi_init();

	CSShellFontInitArpFont(CSShellFontGetDefaultArpFont());
	/* ��ʼ��ͼ������*/
	GsInitialize();

	/* ��ʼ���ļ�ϵͳ*/
	CSFSInit(0x100,1024);

#endif
}

void PrepareTestGuitool(void)
{
	PrepareTestGraph();
}
