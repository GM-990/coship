/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_descramble.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------


 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_descramble.h"
#include "generic_include.h"

#define MODULE_NAME ("CS_DESC")

#define UDI_MAX_DESC 32        /* Check with PIPE team */

typedef struct udi_desc_handle
{
    int      DescId;
    u_int16  uDescPid;
    bool     bAllocDesc;
    int      demux_unit;
    
} UDI_DESC_HANDLE;

static UDI_DESC_HANDLE         gDescHandle[UDI_MAX_DESC];

static CNXT_SEM_ID      gDescSem;

CSUDI_Error_Code tm_desc_init(void)
{
    u_int16 i;
    int Result;
    
    for(i = 0; i < UDI_MAX_DESC; i++ )
    {
       gDescHandle[i].DescId      = 0xFFFF;
       gDescHandle[i].uDescPid    = 0x1FFF;
       gDescHandle[i].bAllocDesc  = FALSE;
       gDescHandle[i].demux_unit  = ~0;
    }

    Result = cnxt_kal_sem_create(1, NULL, &gDescSem);
    if(CNXT_STATUS_OK != Result)
    {
         CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Create Semaphore\n");
         return  CSUDI_FAILURE;
    }
    
    return CSUDI_SUCCESS;
}
//frank.zhou---------------------------------------------------------------------------------------------------
/**
@brief 分配一个解扰通道，返回解扰通道句柄

@param[in] nDemuxIndex  demux设备的索引
@param[out] phDSM 用于接收返回解扰通道句柄
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pHandle的内容未定义
@note 如果已经分配完所有解扰通道，此接口返回CSUDIDSM_ERROR_NO_FREE_DESCRAMBLER。
@note Eastwin会将CSUDI_NULL当做一个非法的句柄，请在实现时特别注意所有Handle的取值范围
*/
CSUDI_Error_Code CSUDIDSMAllocate(int nDemuxIndex, CSUDI_HANDLE * phDSM)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 释放一个解扰通道

@param[in] hDSM 解扰通道的句柄，由CSUDIDSMAllocate返回得到
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
*/ 
CSUDI_Error_Code CSUDIDSMFree(CSUDI_HANDLE hDSM )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 为解扰的通道指定过滤的PID

@param[in] hDSM 解扰通道句柄
@param[in] ushPID 解扰PID
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置解扰PID前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的，使用中亦需要注意该顺序问题
*/ 
CSUDI_Error_Code CSUDIDSMSetPID(CSUDI_HANDLE hDSM,  unsigned short ushPID )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 清空解扰通道指定的过滤PID

@param[in] hDSM 解扰通道句柄
@param[in] ushPID 解扰PID
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
*/ 
CSUDI_Error_Code CSUDIDSMClearPID(CSUDI_HANDLE hDSM, unsigned short ushPID )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置解扰通道的奇Key值

@param[in] hDSM 解扰通道句柄
@param[in] pucOddKey 解扰的奇KEY
@param[in] ucOddLength Key长度
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设key前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的
*/ 
CSUDI_Error_Code CSUDIDSMSetOddKey(CSUDI_HANDLE hDSM, const unsigned char * pucOddKey, unsigned char ucOddLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置解扰通道的偶Key值

@param[in] hDSM 解扰通道句柄
@param[in] pucEvenKey 解扰的偶KEY
@param[in] ucEvenLength Key长度
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设key前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的
*/ 
CSUDI_Error_Code CSUDIDSMSetEvenKey(CSUDI_HANDLE hDSM, const unsigned char * pucEvenKey, unsigned char ucEvenLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


