
#ifndef _CS_SYSTEM_DAEMON_PROXY_H_
#define _CS_SYSTEM_DAEMON_PROXY_H_

#ifdef __cplusplus
extern "C"
{
#endif



/**
	LinuxϵͳAPI����system�� ������,����ִ��szCmdָ����ϵͳ����
*/

#define SYSTEM_DEAMON


#ifdef SYSTEM_DEAMON
int  CSSYSTEMPROXYRun(char * szCmd);
#define COMMAND_OPTION ""


#else
#define CSSYSTEMPROXYRun system


/*{{{for command in/output option*/
/*no stdin, but stdout and stderr; for Info*/
/*#define COMMAND_OPTION "0<&-"*/

/*no stdin , no stdout, just stderr; for debug*/
#define COMMAND_OPTION "0<&- 1>&-"

/*no in/output; for Release*/
/*#define COMMAND_OPTION "0<&- 1>&- 2>&-"*/
/*}}}for command in/output option*/
#endif



#ifdef __cplusplus
}
#endif

#endif

