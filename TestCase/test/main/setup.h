#ifndef MMCP_UDI2
#include "CS_OS.h"
#include "cs_assert.h"
#include "cs_typedef.h"

#include "cs_qam.h"
#include "cs_cofdm.h"
#include "cs_panel.h"
#include "cs_uart.h"
#include "cs_gpio.h"
#include "cs_i2c.h"
#include "cs_panel.h"
#include "cs_ir.h"
#include "cs_vfd.h"
#include "cs_toolset.h"
#include "cs_eeprom.h"
#include "cs_ipcfg.h"
#include "cs_lan.h"
#include "cs_osg.h"
#include "cs_spi.h"
#include "cs_flash.h"
#include "udi_rm.h"
#else
#include "udi2_tuner.h"

#endif
#ifndef TKMODULE_graphics
#include "cs_saapi.h"
#endif





#include "cs_dhcpc.h"

#include "cs_fsapi.h"
#include "tcpip_setup.h"

//#include "irdecode.h"

#include "MemoryPoolInterface.h"
#include "SectionInterface2C.h"
#include "CSUSPDMCreateContainer.h"
#include "CSUSPDMParseData.h"
#include "CSUSPDMCommonType.h"
#include "CSUSPDMCreateIterate.h"
#include "CSUSPDMFlashStorage.h"
#include "SourceInterface.h"
#include "SearchUI.h"
#include "cs_workermanager.h"
#include "cs_sicache.h"

#include "cs_gtools.h"

#include "cs_event.h"
//#include "appconfig.h"
#include "mwapi.h"

#include "cspm_config.h"


#include<unistd.h>
#include<signal.h>



//void SetupPortingDriver(void);
void SetupProtocol(void);
void SetupFS(void);
void SetupDTV(void);
void SetupPVR(void);
void SetupGUITool(void);
void SetupJVM(int argc, char* argv[]);
void SetupGraphics(void);
void SetupUdi(void);
void SetupCa(void);
void SetupJSEXT(void);

void PrepareForAllTest(void);
void PrepareTestPorting(void);
void PrepareTestKernel(void);
void PrepareTestPvr(void);
void PrepareTestMediaplayer(void);
void PrepareTestProtocol(void);
void PrepareTestDtv(void);
//void PrepareTestGraphics(void);
void PrepareTestGraph(void);
void PrepareTestMidp(void);
void PrepareTestJvm(int argc, char *argv[]);
void PrepareTestGuitool(void);
void PrepareTestDtvm(int argc, char *argv[]);
void PrepareTestCodec(void);
void PrepareTestJsext(void);
void PrepareForCaTest(void);

void PrepareWin32Test(void);

