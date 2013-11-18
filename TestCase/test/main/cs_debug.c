#include <stdarg.h>
#include <cs_assert.h>
#include <stdio.h>
#include <string.h>


/******************************************
** Purpose:    Initialise debug level.
**
** Parameter:  nLevel : INFO_LEVEL,WARN_LEVE or ,ERROR_LEVEL
**
** Returns:    Nothing.
**
** Notes  :    user can change this level dyamatic
******************************************/
void CSTTPrint(char *format, ...)
{
	va_list arglist;
	va_start( arglist, format );
	
	CSVDebug("TestKitCase",CLI_LEVEL,format,arglist); /*test cases��������Ϣ����ͨ��CSDebugģ�����*/
	
	va_end(arglist);
}

