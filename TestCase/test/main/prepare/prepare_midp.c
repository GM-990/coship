
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
 
#if defined(TKMODULE_midp)
void PrepareTestMidp(void)
{
	udi_init();
	CSShellFontInitArpFont(CSShellFontGetDefaultArpFont());
	CSWMInit();
	GsInitialize();
	CSFSInit(0x100,1024);
}
#endif
