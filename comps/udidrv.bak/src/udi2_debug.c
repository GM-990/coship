/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_debug.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP API
	 2013/11/06
		 This API is required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------


 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_debug.h"
 /**
 @brief 日志输出函数
 
 @param[in] pStr 格式化好的日志内容
 @code 
 printf("%s",pStr)//参考实现
 @endcode
 @note 
 - 本函数仅被特定模块调用，不推荐外部直接用于输出，除非有充分的理由，否则不允许上层代码绕过上层打印控制接口直接使用该函数输出。\n
 - EastWin会处理好日志输出中的参数规则，移植层无需关心。
 
 -------------------------------------------------------------------------------
 ENTROPIC COMMENTS ON COSHIP API
	 2013/11/06
		 This API is required for Android DVB-S2 plus OTT project. 
 -------------------------------------------------------------------------------
 
 */
void CSDebugDefaultOutputString(const char * pStr )
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    UDIDRV_LOGI("%s end\n", __FUNCTION__);    
}


