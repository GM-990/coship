
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
 
#if 0
#ifndef TKMODULE_graphics
BOOL AddHardInputEventMessageQueueMsg(DWORD dwKey)
{
	dwKey = dwKey;
	CSDebug("TestMain",FATAL_LEVEL, "[Setup]: not support AddHardInputEventMessageQueueMsg!!!!!!!!!!!!!!!!!!\n");
	
	return FALSE;
}
#endif
#endif

void SetupUdi(void)
{
	//SetupPortingDriver();
	udi_init();
}



