
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
 
void PrepareTestDtv(void)
{
	CSTTPrint("[%s.%d] 1\n",__FILE__, __LINE__);
	//SetupPortingDriver();
	udi_init();
	CSTTPrint("[%s.%d] 1\n",__FILE__, __LINE__);
	CSWMInit();
	CSTTPrint("[%s.%d] 1\n",__FILE__, __LINE__);
	SetupFS();
	CSTTPrint("[%s.%d] 1\n",__FILE__, __LINE__);
	SetupDTV();
	CSTTPrint("[%s.%d] 1\n",__FILE__, __LINE__);
}


