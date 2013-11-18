
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
 
static BOOL bInit=FALSE;
void PrepareWin32Test(void)
{
	if(bInit)
		return;
	bInit = TRUE;
	udi_init();
	CSWMInit();
	SetupFS();
	SetupDTV();	

	SetupProtocol();

    //SetupJSEXT();

	//SetupPVR();

}//end prepareForTest()


