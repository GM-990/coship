
#ifndef SHELL_PANEL_H
#define SHELL_PANEL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"

#define  PANEL_ALIGN_CENTER  (0)   ///居中
#define  PANEL_ALIGN_LEFT (1)    ///左对齐
#define  PANEL_ALIGN_RIGHT (2)  ///右对齐	
#define  PANEL_ALIGN_TOP (3)  ///顶部
#define  PANEL_ALIGN_BOTTOM (4)///底部

BOOL CSShellPanelDisplayTextAlign(char *pcText,int nAlign);
BOOL CSShellPanelDisplayLED(int status);


#ifdef __cplusplus
}
#endif

#endif  /* CSMP_H */
