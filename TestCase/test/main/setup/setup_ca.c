#include "setup.h"
#include "udi_assert.h"

extern void CSCATestInit(void);

void SetupCa(void)
{
	USPPrint(0xff, "\n[SetupCa]---test start\n");
	CSCATestInit();
}

