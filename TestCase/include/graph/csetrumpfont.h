#ifndef _CSETRUMPFONT_H
#define _CSETRUMPFONT_H

#include "mwfont.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
@brief  功能描述:
获取到弈创字库的注册指针

@param[in]
无

@param[out] 
无

@return
弈创字库的注册指针
@exception
@note
*/
GR_FONT_Driver_Proc *  GrFont_GetEtrumProc(void);

#ifdef __cplusplus
}
#endif
#endif

