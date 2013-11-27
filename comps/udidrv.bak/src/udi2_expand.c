/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_expand.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_expand.h"

/**
@brief 获取扩展网络设备的连接状态，例如: 硬AP，cable modem，EOC等

@param[in] pcEthName 与扩展网络设备相连的网卡名字
@param[out] pushLinkState 连接状态
@return 成功返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
@note 无线网卡的连接状态:
@note 1、pushLinkState & CSUDI_ETHMASK_UP_DOWN : 为0代表已经连接，为1代表未连接。
@note 2、集成厂商可以自行决定获取连接状态的方法，例如: SNMP 查询协议
*/
CSUDI_Error_Code CSUDIExpandGetStatus(char * pcEthName, unsigned short * pushLinkState)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

