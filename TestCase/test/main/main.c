#include "setup.h"

#include "irdecode.h"
#include "cs_testkit.h"
#include "udiplus_debug.h"

extern void test_main(void);

int main(int argc, char *argv[])
{

	CSDebugSetDefaultLevel(DEBUG_LEVEL);

	printf("\n------always open MP_TEST OC_TEST debug-----\n");
	CSDebugSet("mptest", 0);   
	CSDebugSet("OC_TEST", 0);  
	printf("[%s].%d\n",__FILE__,__LINE__);
#if 0  //please call those function after chip initialize.
	CSDebugSet("CSGraphics",NOTICE_LEVEL);
	CSDebugSet("EventManager",CLI_LEVEL);
	CSDebugSet("CoshipFS",WARN_LEVEL);
	CSDebugSet("SiCache", WARN_LEVEL);
	CSDebugSet("CoshipFS-DSM", WARN_LEVEL);
	CSDebugSet("TestMain", DEBUG_LEVEL);
	CSDebugSet("TestKitCase", CLI_LEVEL);
	CSDebugSet("TSMONITOR",INFO_LEVEL);
	CSDebugSet("CSGraphics",ERROR_LEVEL);

	CSDebugSet("dhcp",0);
	CSDebugSet("memleak",INFO_LEVEL);
	CSDebugSet("mediaplayer",INFO_LEVEL);
	CSDebug("TestMain",DEBUG_LEVEL, "[main]: TestKit Starting.............\n");
#endif

#if defined(TKMODULE_dtvm) //java
	PrepareTestDtvm(argc,argv);
#else	//use test framework

#if defined(TKMODULE_test)
	PrepareForAllTest();
#elif defined(TKMODULE_porting)
	PrepareTestPorting();
#elif defined(TKMODULE_codec)	
	PrepareTestCodec();
#elif defined(TKMODULE_kernel)
	PrepareTestKernel();
#elif defined(TKMODULE_protocol)
	PrepareTestProtocol();
#elif defined(TKMODULE_mediaplayer)
	printf("\n[%s].%d: mp\n", __FUNCTION__, __LINE__);
	PrepareTestMediaplayer();
#elif defined(TKMODULE_dtv)
	PrepareTestDtv();
	CSDebug("TestMain",DEBUG_LEVEL, "[main]: dtv.\n");
#elif defined(TKMODULE_jvm)
	PrepareTestJvm(argc, argv);
#elif defined(TKMODULE_pvr)
	PrepareTestPvr();
#elif defined(TKMODULE_graphics)
//	PrepareTestGraphics();
#elif defined(TKMODULE_graph)
	PrepareTestGraph();
	CSDebug("TestMain",DEBUG_LEVEL, "[main]: graph.\n");
#elif defined(TKMODULE_midp)
	PrepareTestMidp();
	CSDebug("TestMain",DEBUG_LEVEL, "[main]: midp. \n");
#elif defined(TKMODULE_guitool)
	PrepareTestGuitool();
#elif defined(TKMODULE_udi)
    SetupUdi();
#elif defined(TKMODULE_ca)
	PrepareForCaTest();
#elif defined(TKMODULE_shell)
    //PrepareTestPorting();
#elif defined(TKMODULE_udi2)
    PrepareTestPorting();
#elif defined(TKMODULE_jsext)
	PrepareTestJsext();
	CSDebug("TestMain",DEBUG_LEVEL, "[main]: dtv.\n");
#else
	#error Cannot compile main.c.
#endif
	printf("[%s].%d\n",__FILE__,__LINE__);

	CSDebugSet("cs_qam",ERROR_LEVEL);

	CSDebugSet("mp", 0);
    CSDebugSet("mp.avctrl", 0);
	
	CSDebug("TestMain",DEBUG_LEVEL, "[main]: Test begin.\n");

	//UI_IRInitialize(128);

	test_main();
	CSDebug("TestMain",DEBUG_LEVEL, "[main]: Test end.\n");

#endif

//	{
//		HCSHANDLE hSuspend = CSCreateEvent("Suspend Root", 0);
//   		CSWaitForSingleEvent(hSuspend, 0xFFFFFFFF);
//	}

	return 0;
}
