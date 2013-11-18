#include "setup.h"
#include "java.h"

static int global_argc=6;
static char **	global_argv ;


#if defined(PLATFORM_ST40) ||defined(PLATFORM_ST20)
#if defined(TKMODULE_dtvm) 

extern int MMCP_ROMSizePerUnit ; 
extern int MMCP_ROMUnits; 
extern char * MMCP_ROMArray[];
#endif
#endif	



static void StartJVM(void* pvParam)
{
#if defined(TKMODULE_graphics)  || defined(TKMODULE_dtvm) || defined(TKMODULE_jvm)
	ansiJavaMain(global_argc, (const char**)global_argv);
#endif
}

/*st7100平台必须用pthread创建，否则创建过程中会死机。
使用pthread 不会影响bcm平台--YanQiao, 2008.10.14*/
#define CREATE_BY_PTHREAD
#ifndef CREATE_BY_PTHREAD
static char thread_name_jvm[] = "StartJVM";
#endif


void SetupJVM(int argc, char* argv[])
{
#ifdef CREATE_BY_PTHREAD
	pthread_t jvmThreadId; 
	int nError = 0;	
	int i=0;

	CSDebug("TestMain", DEBUG_LEVEL, "[%s.%d] setup JVM, argc=%d\n",__FILE__, __LINE__, argc);
	for(i=0; i<argc; i++)
	{
		CSDebug("TestMain", DEBUG_LEVEL, "argv[%d]=%s\n", i, argv[i]);
	}	
		
	global_argc = argc;
	global_argv = argv;
	
	nError = pthread_create(&jvmThreadId, NULL, (void *(*)(void*))StartJVM, NULL);
	CSASSERT(nError == 0);
	CSDebug("TestMain", DEBUG_LEVEL, "[%s.%d] nError=%d, jvmThreadId=0x%x\n", 
		__FILE__, __LINE__, nError, (DWORD)jvmThreadId);
	
#else
	HCSHANDLE hThreadJVM = 0;
	
	global_argc = argc;
	global_argv = argv;
			
	hThreadJVM = CSCreateThread(thread_name_jvm, 1*17, 1024 * 128, (ThreadEntry*)StartJVM, NULL, NULL);

	CSASSERT(hThreadJVM != 0);
#endif
}

