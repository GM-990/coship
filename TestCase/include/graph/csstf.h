/**
文件名称 :   csstf.h
版权所有(c) 2009， 同洲电子。
 
@version  Ver 1.0    
@author   龙式荣 (longshirong@coship.com)
 
@brief
 		  文件功能说明
  
@history 
版本号		日期		作者		修改内容
---------	     --------	     --------  	-------------
    1.0		   2009-11-20	     龙式荣	  	创建
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
