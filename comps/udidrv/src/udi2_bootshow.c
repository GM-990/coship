/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_bootshow.c
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

#include "udi2_bootshow.h"
 /**
@brief 获取booter的显示相关的能力
@param[out] psCapability 获取booter的显示能力，详见结构体定义
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 详见结构体CSUDIBOOTSHOWCapability定义
@note 该接口根据项目需要可选实现
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetCapability(CSUDIBOOTSHOWCapability* psOutCapability)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获得booter显示用到的已存储的每个显示单元的信息
@param[in] nIndex 存储显示单元的索引（取值为0--已存储显示单元个数-1）
@param[out] psOutInfo 显示单元信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetShowUnitInfo(int nIndex,CSUDIBOOTSHOWUnitInfo* psOutInfo)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief 获取booter用于存储可显示单元数据的flash剩余空间大小
@param[out] pnOutSize 剩余空间大小，单位为BYTE
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 可存储显示数据大小 = 剩余空间大小 - 1个显示单元信息头部大小
@note 剩余空间为flash地址对齐后的剩余可用空间大小。
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取boot已存储开机画面显示单元个数
@param[out] pnOutCount 已存储的显示单元个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetUnitCount(unsigned int* pnOutCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置booter显示某个指定的已存储开机显示单元
@param[in] nIndex 显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
@param[in] bNeedShow 是否需要显示
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 设置某个显示单元显示后，其他的所有显示单元均不显示，开机只显示一个显示单元
@note 如果对多个单元均调用了该接口，则以index最小的单元为准，即其他单元及时设置了显示也是无效的
*/
CSUDI_Error_Code CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置booter显示用到的每个显示单元的信息
@param[in] nIndex 显示单元的索引（取值为大于等于0且小于等于已存储显示单元个数）
@param[in] psInfo 显示单元信息
@param[in] pData  显示单元信息对应的数据区的起始地址
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 如果索引==已存储显示单元个数，则表示新增一个显示单元。
@note 如果修改后的显示单元大小与修改前的不一样，则需要进行flash整理重新分配等操作。
@note 某些显示单元的内容可能是固定的，不允许修改。此时，调用CSUDIBOOTSHOWSetShowUnitInfo返回错误。
*/
CSUDI_Error_Code CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo,void* pData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

