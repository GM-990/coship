/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_diseqc.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_diseqc.h"
/**
@brief 按指定方向转动指定的时间

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] eDirection 设置转动方向
@param[in] ucTime (s) 设置转动时间(秒钟)
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12RotateTimes(CSUDI_UINT32 dwTunerId, CSUDIRotateDirection_E eDirection, CSUDI_UINT8 ucTime)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 按指定方向转动指定的弧度

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] eDirection 指定方向
@param[in] ucCycles 转动弧度
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12RotateCycles (CSUDI_UINT32 dwTunerId, CSUDIRotateDirection_E eDirection,CSUDI_UINT8 ucCycles)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}
/**
@brief 将当前位置保存为第XX个卫星位置

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] ucPosition 当前位置
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12StorePosition (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 转到卫星的第XX个位

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] ucPosition 将要转动到的位置
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12GotoPosition (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}
/**
@brief 转到刻度为0的位

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12ResetPosition (CSUDI_UINT32 dwTunerId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 电机转动直到最东边(最西边)限制位置或接收到STOP命令才停止转动

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] ucDirection 限定位置
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12RotateWithoutStop (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucDirection)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief  停止电机转动

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12RotateStop (CSUDI_UINT32 dwTunerId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief  取消转动范围的限制(最东边和最西边)

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12DisableLimit (CSUDI_UINT32 dwTunerId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief  取消转动范围的限制(最东边和最西边)

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] bDirection 设置当前位置为电机转动的最东边/最西边限制
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12SetLimit (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucDirection)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 根据第XX个卫星的偏移同步所有卫星的位置

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] bPosition 当前卫星偏移位置
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12AdjustTAB (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 转动到指定角度(For STAB USALS Motor)

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] dlAngle 天线方位角(>0 East, <0 West)
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12GotoSTABPosition(CSUDI_UINT32 dwTunerId, double dlAngle)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief  转动到指定角度(For STAB USALS Motor)

@param[in]  dlSatLongitude 卫星经度
@param[in]  dlMyLongitude  本地经度
@param[in]  dlMyLatitude   本地纬度
@param[out] pdlAngle       存储天线转动角度
@return CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12GetAntennaAngle(double dlSatLongitude, double dlMyLongitude, double dlMyLatitude, double *pdlAngle)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief  调整马达到指定的经纬度

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] psSatLocationInfo,设置马达指定的经纬度信息
@return  CSUDI_SUCCESS：成功；失败返回错误代码

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDSC12TurnSateWithUsals(CSUDI_UINT32 dwTunerId, const CSUDISatLocationInfo_S *psSatLocationInfo)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

