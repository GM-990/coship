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
@brief ����һ������ͨ�������ؽ���ͨ�����

@param[in] nDemuxIndex  demux�豸������
@param[out] phDSM ���ڽ��շ��ؽ���ͨ�����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����pHandle������δ����
@note ����Ѿ����������н���ͨ�����˽ӿڷ���CSUDIDSM_ERROR_NO_FREE_DESCRAMBLER��
@note Eastwin�ὫCSUDI_NULL����һ���Ƿ��ľ��������ʵ��ʱ�ر�ע������Handle��ȡֵ��Χ
*/
CSUDI_Error_Code CSUDIDSMAllocate(int nDemuxIndex, CSUDI_HANDLE * phDSM)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �ͷ�һ������ͨ��

@param[in] hDSM ����ͨ���ľ������CSUDIDSMAllocate���صõ�
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/ 
CSUDI_Error_Code CSUDIDSMFree(CSUDI_HANDLE hDSM )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief Ϊ���ŵ�ͨ��ָ�����˵�PID

@param[in] hDSM ����ͨ�����
@param[in] ushPID ����PID
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���ý���PIDǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵģ�ʹ��������Ҫע���˳������
*/ 
CSUDI_Error_Code CSUDIDSMSetPID(CSUDI_HANDLE hDSM,  unsigned short ushPID )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ս���ͨ��ָ���Ĺ���PID

@param[in] hDSM ����ͨ�����
@param[in] ushPID ����PID
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/ 
CSUDI_Error_Code CSUDIDSMClearPID(CSUDI_HANDLE hDSM, unsigned short ushPID )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ���ý���ͨ������Keyֵ

@param[in] hDSM ����ͨ�����
@param[in] pucOddKey ���ŵ���KEY
@param[in] ucOddLength Key����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��keyǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵ�
*/ 
CSUDI_Error_Code CSUDIDSMSetOddKey(CSUDI_HANDLE hDSM, const unsigned char * pucOddKey, unsigned char ucOddLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ���ý���ͨ����żKeyֵ

@param[in] hDSM ����ͨ�����
@param[in] pucEvenKey ���ŵ�żKEY
@param[in] ucEvenLength Key����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��keyǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵ�
*/ 
CSUDI_Error_Code CSUDIDSMSetEvenKey(CSUDI_HANDLE hDSM, const unsigned char * pucEvenKey, unsigned char ucEvenLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


