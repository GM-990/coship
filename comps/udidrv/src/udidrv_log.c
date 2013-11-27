/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_os.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> 

#include "udidrv_log.h"

//#ifdef UDIDRV_LOG
#if 0
void CSDEBUG(char *string, int level, ...)
{
	if(level >= 0)
	{
		int iLen = 0;
		char        pszString [512];
		va_list     arg;
		va_start(arg,string);
		iLen = sprintf(pszString, "[UDI2 LOG]:");
		vsprintf(&pszString[iLen], string, arg);
		printf(pszString);
		va_end(arg);
	}
}
#endif 