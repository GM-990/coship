#ifndef _CS_EEPROM_TEST_ENTRY_H_
#define _CS_EEPROM_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSEEPROMTESTEntry(void);
extern CSTestObj_S* CSEEPROMTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

