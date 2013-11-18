#ifndef JAVASHELL_OTA_H
#define JAVASHELL_OTA_H
#include "mmcp_typedef.h"

void javashell_OtaStart();
void javashell_OtaUserWait();
int javashell_OtaSetStandByState(BOOL bStandby);
int javashell_OtaGetLoaderStatus();
void javashell_EnterManualOta(int nFre,int nSymbol,int nModulation);
char *javashell_getOtaName();
BOOL javashell_hasUpdateInfo();

#endif


