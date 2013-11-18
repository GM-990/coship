
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
 
#if defined (TKMODULE_ca)
void PrepareForCaTest(void)
{
	//SetupPortingDriver();
	udi_init();
	CSWMInit();
	SetupDTV();

	SetupCa();

}//end prepareForTest()
#endif //end #if defined (TKMODULE_test)

