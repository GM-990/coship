/**@defgroup CSSPI[CSSPI]ͬ����Ŀר��SPI �����ӿ�
@version 0.1 
@{
*/

#ifndef _CS_SPI_H_
#define _CS_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "CSHDI_typedef.h"


/**@brief SPI ģ��ӿڵķ���ֵ����*/
typedef enum
{
	CSSPI_SUCCESS,					/**< �ɹ�*/
	CSSPI_FAILURE,					/**< ʧ��*/
	CSSPI_BUSY,						/**< ��æ*/
	CSSPI_NOACK,					/**< ��ȷ��*/
	CSSPI_NOT_SUPPORT,				/**< ��֧��*/
	CSSPI_TIMOUT,					/**< ��ʱ*/
	CSSPI_BAD_PARAM,				/**< �������*/
	CSSPI_MAX_HANDLE,				/**< �����ע��*/
	CSSPI_ALREADY_INIT,				/**< �Ѿ���ʼ��*/
	CSSPI_NOT_INIT					/**< δ��ʼ��*/
}CSSPIError;

/**@brief SPI ���ߵ�ʱ��Ƶ�ʶ���*/
typedef enum
{
	CSSPI_SPEED_LOW,				/**< SPI���ߵ�ʱ��Ƶ�ʵ���40KHz*/
	CSSPI_SPEED_NORMAL,			/**< SPI���ߵ�ʱ��Ƶ�ʵ���100KHz*/
	CSSPI_SPEED_HIGHT				/**< SPI���ߵ�ʱ��Ƶ�ʵ���10MHz*/
}CSSPISpeed;

/**@brief SPI ���ߵ����Ŷ���*/
typedef struct
{
	int 	m_nCSIndex;					/**< SPI���ߵ�Ƭѡ��Ӧ��IO ��*/
	int 	m_nCLKIndex;				/**< SPI���ߵ�ʱ�Ӷ�Ӧ��IO ��*/
	int 	m_nDataInIndex;				/**< SPI���ߵ����������Ӧ��IO ��*/
	int 	m_nDataOutIndex;				/**< SPI���ߵ����������Ӧ��IO ��*/
	int 	m_nReserved;	
}CSSPIPinIndex;

/**@brief SPI ���ߵĲ�������*/
typedef struct
{
	 BOOL       m_bMSBFirst;		/*���ô��/С��ģʽ.If set to TRUE MSB is sent first else LSB is sent.*/
   	 BOOL       m_bClkPhase;		/*��ʱ����λ�����ù涨�˴������ں�ʱ���ͺͽ�������,If set to TRUE pulse will be in the first half cycle else in the 2nd half cycle.*/
   	 BOOL       m_bPolarity;		/*��ʱ�Ӽ��Ե����þ����˾�ֹʱSPI����ʱ�ӵ���Ч״̬ ,If set to TRUE clock idles at logic 1 else at logic 0.*//*��λ�ͼ�����Ϊ����Ӧ��ͬ�������·����Ҫ����Ϊ��Щ������ʱ���źŵ��������������ݣ�����Щ��������ʱ���źŵ��½����������ݡ�*/
   	 DWORD   m_dwDataWidth;	/*��������λ��.Set the width of Tx and Rx buffer for data transfer.*/
    	 DWORD   m_dwBaudRate;		/*���ò�����.Baud rate of the SPI link.*/
    	 DWORD   m_dwGlitchWidth;	/*Value in microseconds specifying the maximum width of noise pulses to be suppressed. User can disable this option by writing '0' .*/
}CSSPIParams;


/**
@brief  ��ʼ����SPI ģ�� 

@param[in] enmIndex SPI bus index
@param[in] nChipSelectIndex: SPI chipSelect GPIO index 
@param[in] enmSpeed: SPI bus speed
@return ��CSSPIError �Ķ���
@note  �������ȵ��ø�ģ��
*/
CSSPIError CSSPIInitialize(CSHDISPIIndex enmIndex, CSSPIPinIndex sCSSPIPinIndex, CSSPISpeed enmSpeed);


/**
@brief   ��һ��SPI���

@param[in] hSPIHandle  SPI ���
@return ��CSSPIError �Ķ���	
@note  �˹�����������һ�����ŵ��豸���������Ӧ����� 
*/
CSSPIError CSSPIOpen( CSHDISPIIndex enmIndex, HCSHANDLE *phSPIHandle );


/**
@brief   �ر�һ��SPI���

@param[in] hSPIHandle  SPI ���
@return ��CSSPIError �Ķ���	
@note  �˹����������ر�һ�����ŵ��豸�����ͷ���Դ�� 
*/
CSSPIError CSSPIClose( HCSHANDLE hSPIHandle );


/**
@brief  Wirte data to SPI device 

@param[in] hSPIHandle SPI ���
@param[in] pwBuffer ���ݻ�����
@param[in] dwDataLength  д�����ݳ���
@return ��CSSPIError �Ķ���		
@note 
*/
CSSPIError CSSPIWriteData( HCSHANDLE hSPIHandle, WORD *pwBuffer,DWORD dwDataLength );


/**
@brief  Read data  from SPI device 

@param[in] hSPIHandle SPI handle
@param[in] dwDataLength Data length to read
@param[out]  pwBuffer Data buffer
@return ��CSSPIError �Ķ���	
@note 
*/
CSSPIError CSSPIReadData( HCSHANDLE hSPIHandle, WORD *pwBuffer,DWORD dwDataLength );


/**
@brief  set params to the SPI bus

@param[in] hSPIHandle SPI handle
@param[in] pSetParams  Data buffer
@return ��CSSPIError �Ķ���	
@note 
*/
CSSPIError CSSPISetParams( HCSHANDLE hSPIHandle, CSSPIParams  *pSetParams);


/**
@brief  get current params from the SPI bus

@param[in] hSPIHandle SPI handle
@param[out]  pGetParams Data buffer
@return ��CSSPIError �Ķ���
@note 
*/
CSSPIError CSSPIGetParams( HCSHANDLE hSPIHandle, CSSPIParams *pGetParams);


/**
@brief  puts an inited spi device into a stable shut down state and deletes its associated control structures.

@param[in] hSPIHandle SPI handle
@param[in] flag used to terminate the SPI bus forcibly
@return ��CSSPIError �Ķ���
@note 
*/
CSSPIError CSSPITerm(CSHDISPIIndex enmIndex, BOOL bForceTerm);


/**
@brief  Request SPI bus

@param[in] enmIndex SPI index
@param[in] dwMilliseconds  Timeout in ms
@return ��CSSPIError �Ķ���	
@note 
*/
CSSPIError CSSPIRequestBus(CSHDISPIIndex enmIndex, DWORD dwMilliseconds);


/**
@brief  �ͷ�SPI���ߵķ���

@param[in] enmIndex  SPI��������
@return   ��CSSPIError �Ķ���	
@note 
*/
CSSPIError CSSPIReleaseBus(CSHDISPIIndex enmIndex);


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
int CSSPIGetVersion(PCHAR pcVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

