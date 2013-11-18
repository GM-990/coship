
#include <QString>
#include <QApplication>
#include "udi2_os.h"

static int s_argc = 0;
static char *ps_argv[5];
static QApplication *ps_gAppInstance;
static int iInitArgs()
{
	s_argc = 5;
	ps_argv[0] =  "whatever";
	ps_argv[1] = "-qws";
        ps_argv[2] = "-display";
        ps_argv[3] = "directfb";
        ps_argv[4] = "--dfb:mode=1280x720,depth=32,pixelformat=ARGB,bg-color=00000000,module-dir=/home/local/lib/directfb-1.4-0,no-cursor";
        
	return 0;
}

static void iExecQT(void * pvParam)
{
     ps_gAppInstance->exec();
}

extern "C"
{
int CSTKExecQT()
{
    CSUDI_HANDLE hThread;
    CSUDIOSThreadCreate("qtthread", 1*17, 16*1024, (CSUDIOSThreadEntry_F)iExecQT, NULL, (CSUDI_HANDLE*)&hThread);
    return 0;
}

int CSTKInitQT()
{
    iInitArgs();

    static QApplication a(s_argc, ps_argv);
    ps_gAppInstance = &a;

    return 0;
}
}
