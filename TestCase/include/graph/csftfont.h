#ifndef _CSFTFONT_H_
#define _CSFTFONT_H_

#include "mwfont.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MODULE_FREETYPE "freetype"
#ifndef CSTTF_SetError
#define CSTTF_SetError  CSDEBUG
#endif

#define CSTTF_RET_ERROR (-1)
#define CSTTF_RET_OK  0

#define CSTTF_Memset memset
#define CSTTF_Memcpy memcpy

GR_FONT_Driver_Proc *GrFont_GetFreetypeProc(void);

#ifdef __cplusplus
}
#endif
#endif


