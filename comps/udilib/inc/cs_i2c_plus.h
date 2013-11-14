/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

/**@defgroup CSI2C[CSI2C]ͬ����Ŀר��I2C�����ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��I2C�����ӿ�
@brief I2C(Inter-Integrated Circuit)������һ����PHILIPS��˾����������ʽ�������ߣ���������΢������������Χ�豸��I2C���߲�������80��������Ϊ��Ƶ����Ƶ�豸�����������Ҫ�ڷ�����������ʹ�ã����а����������״̬��ͨ�š�����udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
 
@version 0.3 2008/11/5  ��һ��ϸ���汾
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/

#ifndef _CS_I2C_PLUS_H_
#define _CS_I2C_PLUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "udiplus_typedef.h"
#include  "cs_hdicommon_plus.h"

/**@brief I2Cģ��ӿڵķ���ֵ����*/
typedef enum
{
	CSI2C_SUCCESS,						/**< �ɹ�*/
	CSI2C_FAILURE,						/**< ʧ��*/
	CSI2C_BUSY,							/**< I2C ���߷�æ*/
	CSI2C_NOACK,						/**< û��Ӧ��*/
	CSI2C_NOT_SUPPORT,					/**< ��֧��*/
	CSI2C_TIMOUT,						/**<���� ��ʱ*/
	CSI2C_BAD_PARAM,					/**< �������*/
	CSI2C_MAX_HANDLE,					/**< ��ʱûʹ��*/
	CSI2C_ALREADY_INIT,					/**< �Ѿ���ʼ��*/
	CSI2C_NOT_INIT						/**< δ��ʼ��*/
}CSI2C_Error_t;


/**@brief I2C���ߵ�ʱ��Ƶ�ʶ���*/
typedef enum
{
	CSI2C_SPEED_LOW,				/**< I2C���ߵ�ʱ��Ƶ�ʵ���40KHz*/
	CSI2C_SPEED_NORMAL,			/**< I2C���ߵ�ʱ��Ƶ�ʵ���100KHz*/
	CSI2C_SPEED_HIGHT,				/**< I2C���ߵ�ʱ��Ƶ�ʵ���400KHz*/
	CSI2C_SPEED_MIDDLE				/**< I2C���ߵ�ʱ��Ƶ�ʵ���300KHz*/
}CSI2C_SPEED_t;

/**@brief I2C���ӿڲ�������*/
typedef struct
{
	BYTE *pucRegAddrBuf;       /**< ��Ŷ��Ĵ�����ַbuffer*/
	DWORD dwAddrLen;	   /**< �Ĵ�����ַbuffer����(�ֽ���),��pucRegAddrBuf��Ч���ݳ���*/ 
	BYTE *pucRdBuf;		   /**<��żĴ�������buffer,��buffer���ٰ���dwDataLengthָ���Ĵ�С�ڴ�*/
	DWORD dwDataLength;	   /**< Ҫ��ȡ�Ĵ������ݵĳ���(�ֽ���)*/ 
	BOOL bWrtStop;	   	   /**< д�����Ƿ����ֹͣλ*/ 
	BOOL bRdStop;	       /**< �������Ƿ����ֹͣλ*/ 
}CSI2CReadInfo_S;

/**
@brief  ��ʼ��I2C ģ�� 

@param[in] enmIndex I2C bus index
@param[in] enmSpeed: I2C bus speed
@return ��CSI2C_Error_t �Ķ���
@note  �������ȵ��ø�ģ��
*/
CSI2C_Error_t CSI2CInitialize (CSHDII2CIndex enmIndex, CSI2C_SPEED_t enmSpeed);



/**
@brief  ��ĳ��i2c �����ϵ�һ���豸������һ�������ͨ���þ����i2c �豸ͨѶ.

@param[in] enmIndex I2C bus index
@param[in] wSlaveAddress I2C device slave address
@param[out] phI2CHandle   I2C handle
@return ��CSI2C_Error_t �Ķ���
@note  �ڶ���д����ǰ������ø�ģ��
*/
CSI2C_Error_t CSI2COpen(CSHDII2CIndex enmIndex, WORD wSlaveAddress, HCSHANDLE *phI2CHandle);

/**
@brief   �ر�һ��I2C���

@param[in] hI2CHandle  I2C ���
@return ��CSI2C_Error_t �Ķ���	
@note  �˹����������ر�һ�����ŵ��豸�����ͷ���Դ�� 
*/
CSI2C_Error_t CSI2CClose(HCSHANDLE hI2CHandle);

/**
@brief  ��ָ����i2c �豸дָ�����ȵ����ݣ�������ֹͣ�ź� ��

@param[in] hI2CHandle I2C ���
@param[in] pucBuffer ���ݻ�����
@param[in] dwDataLength  д�����ݳ���
@return ��CSI2C_Error_t �Ķ���		
@note 
*/
CSI2C_Error_t CSI2CWriteWithStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength);

/**
@brief  ��ָ����I2C �豸��ȡָ�����ȵ����ݣ�������ֹͣ�ź� ��

@param[in] hI2CHandle I2C handle
@param[in] dwDataLength Data length to read
@param[out]  pucBuffer Data buffer
@return ��CSI2C_Error_t �Ķ���	
@note 
*/
CSI2C_Error_t CSI2CReadWithStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength);

/**
@brief  ��ָ����i2c �豸дָ�����ȵ����ݣ���������ֹͣ�ź� ��

@param[in] hI2CHandle I2C handle
@param[in] pucBuffer  Data buffer
@param[in] dwDataLength: Data length to write
@return ��CSI2C_Error_t �Ķ���	
@note 
*/
CSI2C_Error_t CSI2CWriteWithoutStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength);

/**
@brief  ��ָ����I2C �豸��ȡָ�����ȵ����ݣ���������ֹͣ�ź� ��

@param[in] hI2CHandle I2C handle
@param[in] dwDataLength  Data length to read
@param[out]  pucBuffer Data buffer
@return ��CSI2C_Error_t �Ķ���
@note 
*/
CSI2C_Error_t CSI2CReadWithoutStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength);

/**	   
@brief  �ýӿڴ�ָ����I2C�豸��ȡָ�����ȵ����ݣ��ú���������������:дҪ���ļĴ�����ַ+���Ĵ�������
@param[in] hI2CHandle I2C handle
@param[in/out]  psReadInfo ��������Ϣ�����CSI2CReadInfo_S����
@return ��CSI2C_Error_t �Ķ���
@note 
*/
CSI2C_Error_t CSI2CReadExt(HCSHANDLE hI2CHandle, CSI2CReadInfo_S *psReadInfo);

/**
@brief  ��������ָ��I2C ���ߵ����� ��

@param[in] enmIndex I2C index
@param[in] dwMilliseconds  Timeout in ms
@return ��CSI2C_Error_t �Ķ���	
@note 
*/
CSI2C_Error_t CSI2CRequestBus(CSHDII2CIndex enmIndex, DWORD dwMilliseconds);

/**
@brief  �ͷ�ָ��I2C ���ߵķ���

@param[in] enmIndex  I2C��������
@return   ��CSI2C_Error_t �Ķ���	
@note 
*/
CSI2C_Error_t CSI2CReleaseBus(CSHDII2CIndex enmIndex);

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
int CSI2CGetVersion(PCHAR pcVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

