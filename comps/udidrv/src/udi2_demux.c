/****************************************************************************/
/*
* Filename:        udi2_demux.h
*
*
* Description:     API implementation for COSHIP interface layer .
*
*
* Author:          Trident Design-In Team
*
****************************************************************************/

#include "generic_include.h"
#include "udidrv_log.h"

#define MODULE_NAME ("CS_DEMUX")

extern CS_TM_PIPE_OBJECTS gTmPipeObject;

static int gTunerConnectedPipeID = CSUDIDEMUX_NOT_CONNECTED_ID;

CSUDI_Error_Code CSUDIDEMUXGetCount(int *pnDemuxCount)
{
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "START: CSUDIDEMUXGetCount 0x%x\n",pnDemuxCount);
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	

    if(pnDemuxCount == NULL)
    {
    	UDIDRV_LOGI("%s exit line=%d \n", __FUNCTION__, __LINE__);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }
    *pnDemuxCount = MAX_DEMUX_OBJECTS;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);  

    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"End: CSUDIDEMUXGetCount %d\n",*pnDemuxCount);
    return CSUDI_SUCCESS;
}

CSUDI_Error_Code CSUDIDEMUXGetCapability(int nDemuxIndex,CSUDIDEMUXCapability_S *psCapabilityInfo)
{
   CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "START: CSUDIDEMUXGetCapability dmxId %d\n",nDemuxIndex);

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
    return CSUDI_SUCCESS;
}

CSUDI_Error_Code CSUDIDEMUXConnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex)
{
    CSUDI_Error_Code retCode = CSUDI_FAILURE;

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

    return retCode;
}

CSUDI_Error_Code CSUDIDEMUXDisconnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex)
{
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

    return retCode;
}

CSUDI_Error_Code CSUDIDEMUXGetTsPacketCount(int nDemuxIndex, unsigned int * puTsPacketCount)
{

    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "CSUDIDEMUXGetTsPacketCount %d\n", nDemuxIndex);
    if((nDemuxIndex >= MAX_DEMUX_OBJECTS)||(nDemuxIndex < 0)||(puTsPacketCount == NULL))
    {
        CSDebug(MODULE_NAME, DEBUG_LEVEL, "FAIL: CSUDIDEMUXGetTsPacketCount Wrong DmxID %d\n", nDemuxIndex);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    /* No Driver API is available in PIPILINE to get TS Packet Count*/
    *puTsPacketCount = 0;

    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"end: cnxt_dmx_get_ts_packet_count %d\n", *puTsPacketCount);
    return CSUDI_SUCCESS;
}


