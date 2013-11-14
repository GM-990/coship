/****************************************************************************/
/*
 * Filename:        cs_i2c_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 -------------------------------------------------------------------------------
 ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 -------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_i2c_plus.h"

/**
@brief  ��ʼ��I2C ģ�� 

@param[in] enmIndex I2C bus index
@param[in] enmSpeed: I2C bus speed
@return ��CSI2C_Error_t �Ķ���
@note  �������ȵ��ø�ģ��
*/
CSI2C_Error_t CSI2CInitialize (CSHDII2CIndex enmIndex, CSI2C_SPEED_t enmSpeed)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}



/**
@brief  ��ĳ��i2c �����ϵ�һ���豸������һ�������ͨ���þ����i2c �豸ͨѶ.

@param[in] enmIndex I2C bus index
@param[in] wSlaveAddress I2C device slave address
@param[out] phI2CHandle   I2C handle
@return ��CSI2C_Error_t �Ķ���
@note  �ڶ���д����ǰ������ø�ģ��
*/
CSI2C_Error_t CSI2COpen(CSHDII2CIndex enmIndex, WORD wSlaveAddress, HCSHANDLE *phI2CHandle)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief   �ر�һ��I2C���

@param[in] hI2CHandle  I2C ���
@return ��CSI2C_Error_t �Ķ���	
@note  �˹����������ر�һ�����ŵ��豸�����ͷ���Դ�� 
*/
CSI2C_Error_t CSI2CClose(HCSHANDLE hI2CHandle)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  ��ָ����i2c �豸дָ�����ȵ����ݣ�������ֹͣ�ź� ��

@param[in] hI2CHandle I2C ���
@param[in] pucBuffer ���ݻ�����
@param[in] dwDataLength  д�����ݳ���
@return ��CSI2C_Error_t �Ķ���		
@note 
*/
CSI2C_Error_t CSI2CWriteWithStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  ��ָ����I2C �豸��ȡָ�����ȵ����ݣ�������ֹͣ�ź� ��

@param[in] hI2CHandle I2C handle
@param[in] dwDataLength Data length to read
@param[out]  pucBuffer Data buffer
@return ��CSI2C_Error_t �Ķ���	
@note 
*/
CSI2C_Error_t CSI2CReadWithStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  ��ָ����i2c �豸дָ�����ȵ����ݣ���������ֹͣ�ź� ��

@param[in] hI2CHandle I2C handle
@param[in] pucBuffer  Data buffer
@param[in] dwDataLength: Data length to write
@return ��CSI2C_Error_t �Ķ���	
@note 
*/
CSI2C_Error_t CSI2CWriteWithoutStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  ��ָ����I2C �豸��ȡָ�����ȵ����ݣ���������ֹͣ�ź� ��

@param[in] hI2CHandle I2C handle
@param[in] dwDataLength  Data length to read
@param[out]  pucBuffer Data buffer
@return ��CSI2C_Error_t �Ķ���
@note 
*/
CSI2C_Error_t CSI2CReadWithoutStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**	   
@brief  �ýӿڴ�ָ����I2C�豸��ȡָ�����ȵ����ݣ��ú���������������:дҪ���ļĴ�����ַ+���Ĵ�������
@param[in] hI2CHandle I2C handle
@param[in/out]  psReadInfo ��������Ϣ�����CSI2CReadInfo_S����
@return ��CSI2C_Error_t �Ķ���
@note 
*/
CSI2C_Error_t CSI2CReadExt(HCSHANDLE hI2CHandle, CSI2CReadInfo_S *psReadInfo)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  ��������ָ��I2C ���ߵ����� ��

@param[in] enmIndex I2C index
@param[in] dwMilliseconds  Timeout in ms
@return ��CSI2C_Error_t �Ķ���	
@note 
*/
CSI2C_Error_t CSI2CRequestBus(CSHDII2CIndex enmIndex, DWORD dwMilliseconds)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  �ͷ�ָ��I2C ���ߵķ���

@param[in] enmIndex  I2C��������
@return   ��CSI2C_Error_t �Ķ���	
@note 
*/
CSI2C_Error_t CSI2CReleaseBus(CSHDII2CIndex enmIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  

@param[in]  nSize ����������
@param[out]  pcVer  ��Ű汾��Ϣ�Ļ������׵�ַ
@return  1> �����һ������pchVerΪ�գ����ذ汾��Ϣ��ʵ�ʳ���
  	     2> �����һ��������Ϊ�գ��Һ������óɹ����򷵻�ʵ
  	     �ʻ�ȡ�İ汾��Ϣ����
  	     3> ���ʧ�ܣ�����0	
@note 
*/
int CSI2CGetVersion(PCHAR pcVer, int nSize)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

