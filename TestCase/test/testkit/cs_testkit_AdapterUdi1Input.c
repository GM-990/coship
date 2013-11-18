#include "cs_testkit_porting.h"
#include "stdio.h"
#include <string.h>


#include "cs_uart.h"//使用UDI1的接口

/****************UARTUDI1*******************************/
static char* iUartUDI1Gets(char *s,int size)
{
	int nLen = 0;
	if(s && size > 0)
	{
		for (;;)
		{

			#ifndef MMCP_UDI2
			CSHDIUartReadWithTimeout((unsigned char*)&s[nLen], 1, 0xFFFFFFFF);	
			CSHDIUartWrite((unsigned char*)&s[nLen], 1);
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

static int iUartUDI1VPrintf(char *format,va_list arg)
{
	int nLen = 0;
	int nOutLen;
	char strOutBuff[1024];		
	//use vsnprintf instand of vsprintf to solve overflow problem,
	//if any platform not support thsi api,pls rollback to vsprintf by platform marco
	#if 0 //todo add platform not suport C99 here
	vsprintf(strOutBuff + nLen, pcFormat, arg);
	#else
	nOutLen = vsnprintf(strOutBuff + (unsigned int)nLen, sizeof(strOutBuff) - (unsigned int)nLen,format, arg);
	#endif
	printf("%s", strOutBuff);	
	nLen = (int)strlen(strOutBuff);
	return nLen;
}

static void iUartUDI1Flush(void)
{
	//nothing
}

static void iUartUDI1Exit(void)
{
	exit(-1);
}

void CSTKPRegisterUartUDI1Input(void)
{

	CSTKPInputOperations_S* psTKPInput=CSUDI_NULL;
	
	if(!CSTKPGetInputRegisterState())
	{				
		psTKPInput = CSTKPInputGetOps();
	
		psTKPInput->Gets      = iUartUDI1Gets;
		psTKPInput->VPrintf   = iUartUDI1VPrintf;
		psTKPInput->Flush     = iUartUDI1Flush;
		psTKPInput->Exit       = iUartUDI1Exit;
		
		CSTKPSetInputRegisterState(CSUDI_TRUE);
		
		CSTCPrint( "Register UDI1Uart for input Success!\n");
	}
}

