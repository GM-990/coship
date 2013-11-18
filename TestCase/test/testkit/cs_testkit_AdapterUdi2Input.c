#include "cs_testkit_porting.h"
#include <stdio.h>
#include <string.h>
#include "udi2_uart.h"//使用UDI2的接口

/***************************UARTUDI2*******************************/
static char* iUartUDI2Gets(char *s,int size)
{
	int nLen = 0;
	if(s && size > 0)
	{
		for (;;)
		{
			#ifdef MMCP_UDI2
//			CSUDIUartRead(-1 ,(unsigned char*)&s[nLen], 1,0xFFFFFFFF);
//			CSUDIUartWrite(-1,(unsigned char*)&s[nLen], 1, 1);
			#else
			return CSUDI_NULL;
			#endif
			if (s[nLen] == '\n' || s[nLen] == '\r')
			{
				s[nLen] = 0;
				return s;
			}

			nLen++;
			if (nLen >= size - 1)
			{
				s[nLen] = 0;
				return s;
			}
		}
	}

	return CSUDI_NULL;
}

static int iUartUDI2VPrintf(char *format,va_list arg)
{
	return vprintf(format,arg);
}

static void iUartUDI2Flush(void)
{
	fflush(stdout);
}

static void iUartUDI2Exit(void)
{
	exit(-1);
}

void CSTKPRegisterUartUDI2Input(void)
{
	CSTKPInputOperations_S* psTKPInput=CSUDI_NULL;
	
	if(!CSTKPGetInputRegisterState())
	{
		psTKPInput= CSTKPInputGetOps();
	
		psTKPInput->Gets      = iUartUDI2Gets;
		psTKPInput->VPrintf   = iUartUDI2VPrintf;
		psTKPInput->Flush     = iUartUDI2Flush;
		psTKPInput->Exit       = iUartUDI2Exit;

		CSTKPSetInputRegisterState(CSUDI_TRUE);
		
		CSTCPrint( "Register UDI2Uart for input Success!\n");
	}
}


