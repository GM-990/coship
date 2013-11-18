
#ifndef SHELL_PANEL_H
#define SHELL_PANEL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"

#define  PANEL_ALIGN_CENTER  (0)   ///����
#define  PANEL_ALIGN_LEFT (1)    ///�����
#define  PANEL_ALIGN_RIGHT (2)  ///�Ҷ���	
#define  PANEL_ALIGN_TOP (3)  ///����
#define  PANEL_ALIGN_BOTTOM (4)///�ײ�

BOOL CSShellPanelDisplayTextAlign(char *pcText,int nAlign);
BOOL CSShellPanelDisplayLED(int status);


#ifdef __cplusplus
}
#endif

#endif  /* CSMP_H */
