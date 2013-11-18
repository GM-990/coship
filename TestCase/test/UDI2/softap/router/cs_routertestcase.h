#ifndef _CS_ROUTER_TEST_CASE_H_
#define _CS_ROUTER_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_ROUTER_Init(void);
CSUDI_BOOL CSTC_ROUTER_UnInit(void);

CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0001(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0002(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetGetInterfaces_0003(void);

CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetLanIp_0001(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTSetLanIp_0002(void);

CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0001(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0002(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0003(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0004(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0005(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTStartStopService_0006(void);

CSUDI_BOOL CSTC_ROUTER_CSUDIRTGetServiceStatus_0001(void);
CSUDI_BOOL CSTC_ROUTER_CSUDIRTGetServiceStatus_0002(void);

CSUDI_BOOL CSTC_ROUTER_CSUDIRTExecuteIptableCmd_0001(void);




#ifdef __cplusplus
}
#endif

#endif