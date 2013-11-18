#include "cs_testkit.h"
#include "cs_udi2testentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼
#include "os/cs_ostestentry.h"
#include "osg/cs_osgtestentry.h"
#include "input/cs_inputtestentry.h"
#include "section/cs_sectiontestentry.h"
#include "audio/cs_audiotestentry.h"
#include "player/cs_playertestentry.h"
#include "demux/cs_demuxtestentry.h"
#include "panel/cs_paneltestentry.h"
#include "smartcard/cs_smartcardtestentry.h"
#include "fs/cs_fstestentry.h"
#include "tuner/cs_tunertestentry.h"
#include "toolset/cs_toolsettestentry.h"
#include "debug/cs_debugtestentry.h"
#include "flash/cs_flashtestentry.h"
#include "eeprom/cs_eepromtestentry.h"
#include "screen/cs_screentestentry.h"
#include "video/cs_videotestentry.h"
#include "injecter/cs_injectertestentry.h"
#include "descramble/cs_descrambletestentry.h"
#include "aout/cs_aouttestentry.h"
#include "ittest/cs_ittesttestentry.h"
#include "network/cs_protocoludi2_testentry.h"
#include "performance/cs_performancetestentry.h"
#include "usb/cs_usbtestentry.h"
#include "record/cs_recordtestentry.h"
#include "bootshow/cs_bootshowtestentry.h"
#include "ota/cs_otatestentry.h"
#include "hdmi/cs_hdmitestentry.h"
#include "cs_udi2testcase.h"
#include "hid/cs_hidtestentry.h"


/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_UDI2_Init, CSTC_UDI2_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
   {CS_TK_MANUALCASE(CSTC_UDI2_TestTunerDemux_001)},
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSUDI2TESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��
	CSOSTESTGetObj,
	CSOSGTESTGetObj,
	CSINPUTTESTGetObj,
	CSAUDIOTESTGetObj,
	CSPLAYERTESTGetObj,
	CSSECTIONTESTGetObj,
	CSFSTESTGetObj,
	CSPANELTESTGetObj,
	CSSMARTCARDTESTGetObj,
	CSDEMUXTESTGetObj,
	CSTUNERTESTGetObj,
	CSDEBUGTESTGetObj,
	CSFLASHTESTGetObj,
	CSEEPROMTESTGetObj,
	CSSCREENTESTGetObj,
	CSVIDEOTESTGetObj,
	CSINJECTERTESTGetObj,
	CSDESCRAMBLETESTGetObj,
	CSAOUTTESTGetObj,
	CSITTESTTESTGetObj,
	CSPROTOCOLUDI2TESTGetObj,
	CSPERFORMANCETESTGetObj, 
	CSUSBTESTGetObj,
	CSRECORDTESTGetObj,	
	CSTOOLSETTESTGetObj,
	CSBOOTSHOWTESTGetObj,
	CSOTATESTGetObj,
	CSHDMITESTGetObj,
	CSHIDTESTGetObj    
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(UDI2)
/*end don't change*/





