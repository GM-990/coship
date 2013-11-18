
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
 
#if defined (TKMODULE_test)
void PrepareForAllTest(void)
{
	//SetupPortingDriver();
	udi_init();
	SetupProtocol();
	UI_IRInitialize(5*17);		
	CSWMInit();
	GsInitialize();
	SetupFS();
	SetupDTV();
	
#if defined(PLATFORM_BCM740X)
	SetupPVR();
#endif

}//end prepareForTest()

#endif //end #if defined (TKMODULE_test)

