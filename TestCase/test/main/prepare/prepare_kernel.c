
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
 

//#if defined(TKMODULE_kernel)
void PrepareTestKernel(void)
{
	//SetupPortingDriver();
	udi_init();
	SetupFS();
	SetupDTV();	
}
//#endif

