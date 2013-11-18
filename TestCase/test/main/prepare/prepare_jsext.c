
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
#include "cs_selection.h"
#include "cs_env.h"
 
#if defined(TKMODULE_jsext)
void PrepareTestJsext(void)
{
	udi_init();
	CSWMInit();
	CSENVInit();
	SetupFS();
	SetupDTV();
    CSSELCreate(0,0);
}
#endif

