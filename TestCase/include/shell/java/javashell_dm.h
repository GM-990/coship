

#ifndef SHELL_DM_H
#define SHELL_DM_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"

BOOL shellAudioDMInit();
void shellSaveVolInfo(int serviceKey, BYTE info);
BYTE shellGetVolInfo(int serviceKey) ;


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_DM_H */




