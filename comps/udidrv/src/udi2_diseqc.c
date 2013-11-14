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
@brief ��ָ������ת��ָ����ʱ��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] eDirection ����ת������
@param[in] ucTime (s) ����ת��ʱ��(����)
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief ��ָ������ת��ָ���Ļ���

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] eDirection ָ������
@param[in] ucCycles ת������
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief ����ǰλ�ñ���Ϊ��XX������λ��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] ucPosition ��ǰλ��
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief ת�����ǵĵ�XX��λ

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] ucPosition ��Ҫת������λ��
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief ת���̶�Ϊ0��λ

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief ���ת��ֱ�����(������)����λ�û���յ�STOP�����ֹͣת��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] ucDirection �޶�λ��
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief  ֹͣ���ת��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief  ȡ��ת����Χ������(��ߺ�������)

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief  ȡ��ת����Χ������(��ߺ�������)

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] bDirection ���õ�ǰλ��Ϊ���ת�������/����������
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief ���ݵ�XX�����ǵ�ƫ��ͬ���������ǵ�λ��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] bPosition ��ǰ����ƫ��λ��
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief ת����ָ���Ƕ�(For STAB USALS Motor)

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] dlAngle ���߷�λ��(>0 East, <0 West)
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief  ת����ָ���Ƕ�(For STAB USALS Motor)

@param[in]  dlSatLongitude ���Ǿ���
@param[in]  dlMyLongitude  ���ؾ���
@param[in]  dlMyLatitude   ����γ��
@param[out] pdlAngle       �洢����ת���Ƕ�
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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
@brief  ������ﵽָ���ľ�γ��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] psSatLocationInfo,�������ָ���ľ�γ����Ϣ
@return  CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������

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

