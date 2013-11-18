#ifndef SHELL_EPGMANAGER_H
#define SHELL_EPGMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "EPGReceiverInterface.h"
#include "CSEPGSortInterface.h"

HCSHANDLE javashell_getEPGSortHandle();
HCSHANDLE javashell_getEPGHandle();
BOOL javashell_destroyEPG();


HCSHANDLE javashell_createEPGManager(EPGCallBack epgCallback , EPGLevelGCallBack sortCallback);
#ifdef __cplusplus
}
#endif

#endif  /* SHELL_EPGMANAGER_H */

