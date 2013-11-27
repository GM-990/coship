#include <stdio.h>
#include "udiplus_debug.h"
#include "udiplus_typedef.h"
#include "udiplus_error.h"


void CSUDIDEBUGOutputString(const char * pStr )
{
	if(pStr)
	{
		printf("%s",pStr);
		fflush( stdout );
	}
}


void CSDebugDefaultOutputString(const char * pStr )
{
	if(pStr)
	{
		printf("%s",pStr);
		fflush( stdout );
	}
}

