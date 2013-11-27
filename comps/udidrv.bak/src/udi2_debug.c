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
 @brief ��־�������
 
 @param[in] pStr ��ʽ���õ���־����
 @code 
 printf("%s",pStr)//�ο�ʵ��
 @endcode
 @note 
 - �����������ض�ģ����ã����Ƽ��ⲿֱ����������������г�ֵ����ɣ����������ϲ�����ƹ��ϲ��ӡ���ƽӿ�ֱ��ʹ�øú��������\n
 - EastWin�ᴦ�����־����еĲ���������ֲ��������ġ�
 
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


