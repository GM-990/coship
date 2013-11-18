
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
 
//#if defined(TKMODULE_pvr)
extern BOOL CSSYSSetPvrSupport(BOOL bSupport);

void PrepareTestPvr(void)
{
	CSSYSSetPvrSupport(TRUE);
	//SetupPortingDriver();
	udi_init();
	CSWMInit();
	SetupFS();
	SetupDTV();
#if defined(PLATFORM_ST40)
//	CSSTRGInit();
	SetupPVR();
#endif
#if defined(PLATFORM_BCM740X)
	SetupPVR();
#endif
}
//#endif

