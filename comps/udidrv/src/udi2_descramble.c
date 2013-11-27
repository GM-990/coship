/******************************************************************************/
/*
* Filename:        udi2_descrambler.c
*
*
* Description:     Demux Descrambler API implementation for COSHIP UDI layer.
*
*
* Author:          Trident Design-In Team
*
*******************************************************************************/

#include "generic_include.h"
#include "udi2_descramble.h"

#define MODULE_NAME "CS_DESC"
#define DESC_KEY_LENGTH_DVB_CSA 8

extern CS_TM_PIPE_OBJECTS gTmPipeObject;

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

int tm_get_free_DescID(void)
{
    int i = 0;

    for(i = 0; i< UDI_MAX_DESC ; i++)
    {
       if (gDescHandle[i].bAllocDesc == FALSE)
       {
          return i;
       }
    }

    return -1;
}

CSUDI_Error_Code CSUDIDSMAllocate(int nDemuxIndex, CSUDI_HANDLE * phDSM)
{
    int DescId = -1;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    
    if((nDemuxIndex >= MAX_DEMUX_OBJECTS)||(nDemuxIndex < 0)||(phDSM == NULL))
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL,"NULL Desc\n");
       return CSUDIDSM_ERROR_BAD_PARAMETER;
    }
    if ( cnxt_kal_sem_get(gDescSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is busy ....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }
    do
    {
         DescId = tm_get_free_DescID();
         if(DescId < 0)
         {
            CSDebug(MODULE_NAME, ERROR_LEVEL, "No Desc Is free to Use ....\n");
            retCode = CSUDI_FAILURE;
            break;
         }
         gDescHandle[DescId].DescId      = DescId;
         gDescHandle[DescId].uDescPid    = 0x1FFF;
         gDescHandle[DescId].bAllocDesc  = TRUE;
         gDescHandle[DescId].demux_unit  = nDemuxIndex;

        *phDSM = (CSUDI_HANDLE)&(gDescHandle[DescId].DescId);

         retCode = CSUDI_SUCCESS;
    }while(0);

    //printf("DESC:Alloc - DescId(%d)demux_unit(%d)\n",gDescHandle[DescId].DescId,gDescHandle[DescId].demux_unit);

    cnxt_kal_sem_put(gDescSem);
    return retCode;
}

CSUDI_Error_Code CSUDIDSMFree(CSUDI_HANDLE hDSM )
{
    int DescID;
    int *Id;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;

    
    if(hDSM == NULL)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"NULL Desc\n");
        return CSUDIDSM_ERROR_INVALID_HANDLE;
    }

    Id     = (int *)hDSM;
    DescID = *Id;
    //printf("DESC: %s() - DescId(%d)\n",__FUNCTION__,DescID);
    
    
    if (gDescHandle[DescID].bAllocDesc == FALSE)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is already free....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }
    
    if ( cnxt_kal_sem_get(gDescSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is busy ....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }
    
    do
    {
       gDescHandle[DescID].DescId      = 0xFFFF;
       gDescHandle[DescID].uDescPid    = 0x1FFF;
       gDescHandle[DescID].bAllocDesc  = FALSE;
       gDescHandle[DescID].demux_unit  = 0xFF;
       
       retCode = CSUDI_SUCCESS;
        
    }while(0);

    cnxt_kal_sem_put(gDescSem);

    return retCode;
}


CSUDI_Error_Code CSUDIDSMSetPID(CSUDI_HANDLE hDSM,  unsigned short ushPID )
{
    int DescID;
    int *Id;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;

    if(hDSM == NULL)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIDSMSetPID: NULL Desc\n");
        return CSUDIDSM_ERROR_INVALID_HANDLE;
    }

    Id     = (int *)hDSM;
    DescID = *Id;
    //printf("DESC: %s() - DescId(%d)\n",__FUNCTION__,DescID);

    if (gDescHandle[DescID].bAllocDesc == FALSE)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is not Allocated....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }
    
    if(ushPID >= 0x1FFF)
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL, "Setting Invalid PID 0x%x \n", ushPID);
       return CSUDIDSM_ERROR_BAD_PARAMETER;
    }
    
    if ( cnxt_kal_sem_get(gDescSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is busy ....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }

    do
    {
       gDescHandle[DescID].uDescPid = ushPID;

       retCode = CSUDI_SUCCESS;
       
    }while(0);

    cnxt_kal_sem_put(gDescSem);

    return retCode;
}

CSUDI_Error_Code CSUDIDSMClearPID(CSUDI_HANDLE hDSM, unsigned short ushPID )
{
    int DescID;
    int *Id;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;

    if(hDSM == NULL)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIDSMSetPID: NULL Desc\n");
        return CSUDIDSM_ERROR_INVALID_HANDLE;
    }

    Id     = (int *)hDSM;
    DescID = *Id;
    //printf("DESC: %s() - DescId(%d)\n",__FUNCTION__,DescID);

    if (gDescHandle[DescID].bAllocDesc == FALSE)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is not Allocated....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }
    
    if(ushPID > 0x1FFF )
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL, "Setting Invalid PID 0x%x \n", ushPID);
       return CSUDIDSM_ERROR_BAD_PARAMETER;
    }
    
    if ( cnxt_kal_sem_get(gDescSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is busy ....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }

    do
    {
       if(gDescHandle[DescID].uDescPid != ushPID)
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL, "Wrong Pid for clear:uDescPid(%d) ushPID(%d)\n",gDescHandle[DescID].uDescPid,ushPID);
          retCode = CSUDI_FAILURE;
          break;
       }
       
       gDescHandle[DescID].uDescPid = 0x1FFF;

       retCode = CSUDI_SUCCESS;
       
    }while(0);

    cnxt_kal_sem_put(gDescSem);

    return retCode;
}

CSUDI_Error_Code CSUDIDSMSetOddKey(CSUDI_HANDLE         hDSM, 
                                   const unsigned char *pucOddKey,
                                   unsigned char        ucOddLength)
{
    int DescID;
    int *Id;
    u_int8 DemuxID;
    PIPE_DEMUX_DESCRAMBLER_KEY_CFG DescKey;
    u_int8 uKey[8] = {0};
    int i;
    u_int32 status;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;

    if(hDSM==NULL || !pucOddKey || ucOddLength != DESC_KEY_LENGTH_DVB_CSA)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIDSMFree: NULL Desc\n");
        return CSUDIDSM_ERROR_BAD_PARAMETER;
    }

    Id     = (int *)hDSM;
    DescID = *Id;
    //printf("DESC: %s() - DescId(%d)\n",__FUNCTION__,DescID);

    if (gDescHandle[DescID].bAllocDesc == FALSE)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is not Allocated....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }

    if(gDescHandle[DescID].uDescPid > 0x1FFF )
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL, "Pid Not Set PID 0x%x \n", gDescHandle[DescID].uDescPid);
       return CSUDIDSM_ERROR_BAD_PARAMETER;
    }
    
    if ( cnxt_kal_sem_get(gDescSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is busy ....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }


    do
    {
        //printf("Odd Key[%d]:",ucOddLength);
        for (i = 0; i < ucOddLength ; i++)
        {
            uKey[i] = *(pucOddKey + i );
            //printf("uKey[%d]= 0x%x \t",i,uKey[i]);
        }
        //printf("\n");
        
        DemuxID          = gDescHandle[DescID].demux_unit;
        
        DescKey.uPid     = gDescHandle[DescID].uDescPid;
        DescKey.pData    = uKey;
        DescKey.uLength  = ucOddLength;
        DescKey.Flag     = PIPE_DEMUX_DESCRAMBLER_ODD_KEY_OR_IV;
        DescKey.DataType = PIPE_DEMUX_PID_DESCRAMBLER_DATA_TYPE_KEY;
        DescKey.DescType = PIPE_DEMUX_DESCRAMBLE_DVB;
        DescKey.uLevel   = PIPE_DEMUX_PID_DESCRAMBLER_DATA_LEVEL_0;

        status = gTmPipeObject.hDemuxObj[DemuxID]->set_descrambler_key( \
                                             gTmPipeObject.hDemuxObj[DemuxID],
                                             &DescKey);
        if (status != PIPE_STATUS_OK)
        {
           printf( "DESC : Failed to Set Odd Key In PIPE status(%d)\n",status);
           retCode = CSUDI_FAILURE;
           break;
        }

        retCode = CSUDI_SUCCESS;
        
    }while(0);

    cnxt_kal_sem_put(gDescSem);
    return retCode;
}

CSUDI_Error_Code CSUDIDSMSetEvenKey(CSUDI_HANDLE         hDSM, 
                                    const unsigned char *pucEvenKey,
                                    unsigned char        ucEvenLength)
{
    int DescID;
    int *Id;
    u_int8 DemuxID;
    PIPE_DEMUX_DESCRAMBLER_KEY_CFG DescKey;
    u_int8 uKey[8] = {0};
    int i;
    u_int32 status;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;

    if(hDSM==NULL || !pucEvenKey || ucEvenLength != DESC_KEY_LENGTH_DVB_CSA)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIDSMFree: NULL Desc\n");
        return CSUDIDSM_ERROR_BAD_PARAMETER;
    }

    Id     = (int *)hDSM;
    DescID = *Id;
    //printf("DESC: %s() - DescId(%d)\n",__FUNCTION__,DescID);

    if (gDescHandle[DescID].bAllocDesc == FALSE)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is not Allocated....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }

    if(gDescHandle[DescID].uDescPid >= 0x1FFF )
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL, "Pid Not Set PID 0x%x \n", gDescHandle[DescID].uDescPid);
       return CSUDIDSM_ERROR_BAD_PARAMETER;
    }
    
    if ( cnxt_kal_sem_get(gDescSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Desc Device is busy ....\n");
        return CSUDIDSM_ERROR_INVALID_STATUS;
    }


    do
    {
        //printf("Even Key[%d]:",ucEvenLength);
        for (i = 0; i < ucEvenLength ; i++)
        {
            uKey[i] = *(pucEvenKey + i );
            //printf("uKey[%d]= 0x%x \t",i,uKey[i]);
        }
        //printf("\n");
        
        DemuxID          = gDescHandle[DescID].demux_unit;
        
        DescKey.uPid     = gDescHandle[DescID].uDescPid;
        DescKey.pData    = uKey;
        DescKey.uLength  = ucEvenLength;
        DescKey.Flag     = PIPE_DEMUX_DESCRAMBLER_EVEN_KEY_OR_IV;
        DescKey.DataType = PIPE_DEMUX_PID_DESCRAMBLER_DATA_TYPE_KEY;
        DescKey.DescType = PIPE_DEMUX_DESCRAMBLE_DVB;
        DescKey.uLevel   = PIPE_DEMUX_PID_DESCRAMBLER_DATA_LEVEL_0;

        status = gTmPipeObject.hDemuxObj[DemuxID]->set_descrambler_key( \
                                             gTmPipeObject.hDemuxObj[DemuxID],
                                             &DescKey);
        if (status != PIPE_STATUS_OK)
        {
           printf( "DESC : Failed to Set Even Key In PIPE status(%d)\n",status);
           retCode = CSUDI_FAILURE;
           break;
        }

        retCode = CSUDI_SUCCESS;
        
    }while(0);

    cnxt_kal_sem_put(gDescSem);
    return retCode;
}


