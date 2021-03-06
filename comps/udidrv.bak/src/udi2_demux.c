/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_demux.c
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


#include "udi2_demux.h"
#include "generic_include.h"

#define MODULE_NAME ("CS_DEMUX")

static CNXT_SEM_ID  gSectionSem;
extern CS_TM_PIPE_OBJECTS gTmPipeObject;
extern u_int32 guSectionFilterCount[MAX_NUM_TSI_DEMUX+1];

static int gTunerConnectedPipeID = CSUDIDEMUX_NOT_CONNECTED_ID;
/**
@brief 获取Demux设备的总数

@param[out] pnDemuxCount 系统中存在demux设备的个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pnDemuxCount的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetCount(int * pnDemuxCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	if(pnDemuxCount == NULL)
	{
		return CSUDIDEMUX_ERROR_BAD_PARAMETER;
	}
	*pnDemuxCount = MAX_DEMUX_OBJECTS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 获取指定Demux设备的能力

无论空闲或者被占用
@param[in] nDemuxIndex demux设备的索引
@param[out] psCapabilityInfo demux设备能力信息，详见CSUDIDEMUXCapability_S定义
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且psCapabilityInfo的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetCapability(int nDemuxIndex,CSUDIDEMUXCapability_S * psCapabilityInfo)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	if((psCapabilityInfo == NULL)||((nDemuxIndex >= MAX_DEMUX_OBJECTS)||(nDemuxIndex < 0)))
    {
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    if (nDemuxIndex == 0 || nDemuxIndex == 1)
    {
		psCapabilityInfo->m_dwWorkTypeMask = (EM_UDI_DEMUX_PLAY   | \
		                                     EM_UDI_DEMUX_RECORD | \
		                                     EM_UDI_DEMUX_DMA);
		psCapabilityInfo->m_dwDMAConnectedTunerId  = gTunerConnectedPipeID;
		psCapabilityInfo->m_nFilterNum = 26;//(MAX_SECTION_FILTERS/MAX_NUM_TSI_DEMUX);
		psCapabilityInfo->m_nkeyNum    = 32; /*Changed Once descrambler implemented */
		psCapabilityInfo->m_dwDMAConnectSupportTunersMask = 1;
		psCapabilityInfo->m_dwConTypeMask            = 0;
    }
    else if (nDemuxIndex == 2)
    {
        psCapabilityInfo->m_dwWorkTypeMask = ( EM_UDI_DEMUX_INJECT | \
                                               EM_UDI_DEMUX_PLAY);
        psCapabilityInfo->m_dwDMAConnectedTunerId  = CSUDIDEMUX_NOT_CONNECTED_ID;
        psCapabilityInfo->m_nFilterNum = 12;//0;
        psCapabilityInfo->m_nkeyNum    = 0;
        psCapabilityInfo->m_dwDMAConnectSupportTunersMask = 0;
        psCapabilityInfo->m_dwConTypeMask          = EM_UDI_DEMUX_INJECT_TS;
    }


    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FilterNum %d KeyNum %d WorkTye %d ConType %d DMAConnected %d \n", 
    psCapabilityInfo->m_nFilterNum,psCapabilityInfo->m_nkeyNum, psCapabilityInfo->m_dwWorkTypeMask, 
    psCapabilityInfo->m_dwConTypeMask, psCapabilityInfo->m_dwDMAConnectedTunerId);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 用指定demux连接指定tuner

@param[in] dwTunerId  与指定demux连接的tuner设备Id
@param[in] nDemuxIndex  与指定tuner连接的demux索引号
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
@note 
	- 所有需要tuner和demux设备的服务在启动之前都必须调用该接口进行tuner与demux的连接
	- 如果处在工作状态的demux具有直播和录制功能，在启动直播之后，再去启动录制时，可以不用调用此接口再次连接tuner和demux，其它类似服务按照此规定。
@note 开机后Tuner和Demux的默认连接规则为0-0，1-1以此类推。因此，请尽量将主Demux的索引排在前面。
@note 一个Tuner可以和多个Demux相连,比如边录边播的情况;但一个Demux只能同时连接一个Tuner
@note 若同时将一个Demux连接到多个Tuner上，返回CSUDIDEMUX_ERROR_DEVICE_BUSY
@note Demux必须先与Tuner断开连接，才能用于创建Inject，即一个Demux不能同时拥有两个源设备
@note 同样，被Inject占用的Demux设备不能与Tuner连接，这种情况下将返回CSUDIDEMUX_ERROR_DEVICE_BUSY
@note 若所连接的Tuner和Demux无法连接，比如将内置Tuner与不支持Live的Inject的Demux连接，返回CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED
@note 只有DMA类型的Tuner才需要使用该接口进行连接,否则请返回CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED
@note 若对应Tuner与Demux不可连接，返回CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED，这种情况是基于某些平台的特性或者简化实现而定义的
@note 暂不考虑DMA类型的可移动Tuner设备

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXConnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
	
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int       Index          = 0;
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "START: CSUDIDEMUXConnectTuner Demux%d <-> Tuner%d\n", nDemuxIndex, dwTunerId);

    if((nDemuxIndex >= MAX_NUM_TSI_DEMUX )||(nDemuxIndex < 0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXConnectTuner Wrong DmxID %d\n", nDemuxIndex);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }
    if((dwTunerId >= MAX_TUNER_OBJECTS)||(dwTunerId < 0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXConnectTuner Wrong TunerID %d\n", dwTunerId);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    printf("\ndwTunerId(%d)::nDemuxIndex(%d)\n",dwTunerId,nDemuxIndex);

    #if 0
    if (gTunerConnectedPipeID == dwTunerId)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXConnectTuner Tuner Already Connected with PIPE(%d)\n", gTunerConnectedPipeID);
        return CSUDI_SUCCESS;
    }
    #endif
    
    /* Add tuner device According to Demux Number,Note: we Have only one tuner Device*/
    Index = nDemuxIndex;

    do
    {
        printf("Add Tuner in PIPE No(%d)\n",Index);
        if ( gTmPipeObject.hPipeObj[Index]->add_device(gTmPipeObject.hPipeObj[Index], (PIPE_OBJ_HANDLE)gTmPipeObject.hTunerObj[0])!= PIPE_STATUS_OK )
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Cannot Add Tuner To pipeline\n" );
        }

      gTunerConnectedPipeID = 0;
      retCode = CSUDI_SUCCESS;

    }while(0);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 断开指定demux与指定tuner的连接

@param[in] dwTunerId 与指定demux连接的tuner设备Id
@param[in] nDemuxIndex 与指定tuner连接的demux索引号
@return 成功断开则返回CSUDI_SUCCESS；失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXDisconnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CSUDI_Error_Code retCode = CSUDI_FAILURE;
	int Index = 0;
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "START: CSUDIDEMUXDisconnectTuner Demux%d <-> Tuner%d\n", nDemuxIndex, dwTunerId);

    if((nDemuxIndex >= MAX_NUM_TSI_DEMUX )||(nDemuxIndex < 0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXDisconnectTuner Wrong DmxID %d\n", nDemuxIndex);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }
    if((dwTunerId >= MAX_TUNER_OBJECTS)||(dwTunerId < 0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXDisconnectTuner Wrong TunerID %d\n", dwTunerId);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    #if 0
    if (gTunerConnectedPipeID != dwTunerId)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXDisconnectTuner Wrong PipeID %d\n", gTunerConnectedPipeID);
        return CSUDI_SUCCESS;
    }
    #endif
    
    /* Remove tuner device According to Demux Number,Note: we Have only one tuner Device*/
    Index = nDemuxIndex;

    do
    {
        printf("Remove Tuner(%d) From PIPE No(%d)\n",dwTunerId,Index);
        
        if ( gTmPipeObject.hPipeObj[Index]->delete_device(gTmPipeObject.hPipeObj[Index], (PIPE_OBJ_HANDLE)gTmPipeObject.hTunerObj[0])!= PIPE_STATUS_OK )
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Cannot Delete Tuner to pipeline\n" );
        }
        
        gTunerConnectedPipeID = CSUDIDEMUX_NOT_CONNECTED_ID;
        retCode = CSUDI_SUCCESS;
        
    }while(0);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 获取指定Demux设备的空闲Filter总数

@param[in] nDemuxIndex demux设备的索引
@param[out] pnFreeFilterCount 当前存在的空闲filter个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pnFreeFilterCount的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetFreeFilterCount(int nDemuxIndex, int * pnFreeFilterCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	if((nDemuxIndex >= MAX_DEMUX_OBJECTS)||(nDemuxIndex < 0)||(pnFreeFilterCount == NULL))
	{
	   CSDebug(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXGetFreeFilterCount Wrong DmxID %d\n", nDemuxIndex);
	   return CSUDIDEMUX_ERROR_BAD_PARAMETER;
	}

	if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
	{
	   CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
	   return CSUDISECTION_ERROR_UNKNOWN_ERROR;
	}

	do 
	{
	   if(nDemuxIndex < MAX_NUM_TSI_DEMUX)
	   {
	       *pnFreeFilterCount = ( 26 -guSectionFilterCount[nDemuxIndex]);
	   }
	   else if(nDemuxIndex == 2)
	   {
	        *pnFreeFilterCount = ( 12 -guSectionFilterCount[nDemuxIndex]);
	   }
	   else
	   {
	       *pnFreeFilterCount = 0;
	   }
	   
	}while(0);

	cnxt_kal_sem_put(gSectionSem);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief 获取指定Demux设备当前ts包个数

获取已经注入(从Tuner硬注入)的TS包的数量
@param[in] nDemuxIndex demux设备的索引
@param[out] puTsPacketCount 存放当前dumux中ts包个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且puTsPacketCount的内容未定义
@note 本接口主要与CSUDIPLAYERGetPacketCount接口配合使用判断前端码流是否停播
@note 本接口仅要求从对从Tuner来的数据有效，是否可以获取软注入的TS包不做要求，UDI2.0的实现也不需要刻意回避，Eastwin不允许使用该接口获取软注入的TS包信息。
@note 在部分平台上，该接口无法实现。此时须返回CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED。 Eastwin不会在不支持的平台上使用此接口判断码流停播。
@deprecated 此接口中间件未使用，可不实现

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
		but Entropic Pipeline Manager doesn't support to get the TS packet count,
		so we always reture 0 of TS packet count.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetTsPacketCount(int nDemuxIndex, unsigned int * puTsPacketCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	if((nDemuxIndex >= MAX_DEMUX_OBJECTS)||(nDemuxIndex < 0)||(puTsPacketCount == NULL))
    {
        CSDebug(MODULE_NAME, DEBUG_LEVEL, "FAIL: CSUDIDEMUXGetTsPacketCount Wrong DmxID %d\n", nDemuxIndex);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    /* No Driver API is available in PIPILINE to get TS Packet Count*/
    *puTsPacketCount = 0;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

