#ifndef _CS_DEMUX_TEST_ENTRY_H_
#define _CS_DEMUX_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSDEMUXTESTEntry(void);
extern CSTestObj_S* CSDEMUXTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

