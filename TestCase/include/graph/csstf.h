/**
�ļ����� :   csstf.h
��Ȩ����(c) 2009�� ͬ�޵��ӡ�
 
@version  Ver 1.0    
@author   ��ʽ�� (longshirong@coship.com)
 
@brief
 		  �ļ�����˵��
  
@history 
�汾��		����		����		�޸�����
---------	     --------	     --------  	-------------
    1.0		   2009-11-20	     ��ʽ��	  	����
*/

#ifndef _CSSTF_H
#define _CSSTF_H

/******************************************************************************
*                                 Header File Include                         *
*******************************************************************************/
#include "mwfont.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*				      Extern Function Declare              				*
*******************************************************************************/
GR_FONT_Driver_Proc *GrFont_GetSTFProc(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of file */
