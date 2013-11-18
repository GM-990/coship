
#ifndef SHELL_EITLOADER_H
#define SHELL_EITLOADER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "EPGReceiverInterface.h"
#include "SectionInterface2C.h"
#include "MemoryPoolInterface.h"

HCSHANDLE javashell_createEITLoader(EPGCallBack );
HCSHANDLE javashell_getPFInfoHandle();
#ifdef __cplusplus
}
#endif

#endif  /* SHELL_EITLOADER_H */




