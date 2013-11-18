/**@defgroup CSSCART [CSSCART]ͬ����Ŀר��SCART�ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨�޹�QAM�����ӿ�
@brief  SCART��ŷ��ǿ��Ҫ������������ǵ��ӽ��ջ������ӻ���¼�������������Ƶ�豸�Ļ�����ͨ�ӿڡ���׼��SCART�ӿ�Ϊ21��������,ͬʱ��������Ƶ����Ƶ�źŴ���ͨ��������udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
 
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/

#ifndef _CS_SCART_H_
#define _CS_SCART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "CSHDI_typedef.h"
#include "cs_typedef.h"
#include "cs_assert.h"

/**@brief SCART�Ŀ���ģʽ*/
typedef enum
{
	SCART_IC,	/**<IC����*/
	SCART_HW	/**<Ӳ������*/
}T_SCART_TYPE;



/**@brief GPIO�ľ�������*/
typedef struct
{
	int	m_nGPIO16_9;			/**<����16:9��4:3*/
	int	m_nGPIOTV_AV;			/**<����tv av���*/
	int	m_nGPIOSlow_SW;			/**<�Ƿ�������VCR*/
	int	m_nGPIOFastBlank;		/**<���ΪRGB��s����*/
	int	m_nGPIOVCR_SW;			/**<�ź���Դ*/
}T_SCART_GPIO_CFG;

/**@brief GPIO�ľ�������*/
typedef struct
{
	int	m_nGPIO16_9;			/**<����16:9��4:3*/
	int	m_nGPIOTV_AV;			/**<����tv av���*/
	int	m_nGPIOSlow_SW;			/**<�Ƿ�������VCR*/
	int	m_nGPIOFastBlank;		/**<���ΪRGB��s����*/
	int	m_nGPIOVCR_SW;			/**<TV scart �ź���Դ, STB�źŻ�VCR�ź�*/
	int   m_nGPIOTV_SW;                      /**<VCR scart �ź���Դ, STB�źŻ�TV�ź�*/
	DWORD   m_dwReserved1;                         /**<����λ*/
	DWORD   m_dwReserved2;				/**<����λ*/
}T_SCART_GPIO_CFG_EX;

/**@brief Scartģʽ*/
typedef enum
{
	SCART_TV, 					/**<TV��Scart*/
	SCART_VCR					/**<VCR��Scart*/
}T_SCART_INDEX;

/**@brief ����źŵ���Դ*/
typedef enum
{
	SOURCE_VCR,			 		/**<�ź���Դ��VCR*/
	SOURCE_ENCODER,				/**<�ź���Դ��CPU*/
	SOURCE_AUX,					/**<�ź���Դ�ڸ����豸*/
	SOURCE_TV                                      /**<�ź���Դ��TV SCART*/
}T_SIGNAL_SOURCE;

/**@brief ��ʾģʽ*/
typedef enum
{
	DISPLAY_4_3,				/**<4:3ģʽ*/
	DISPLAY_16_9				/**<16:9ģʽ*/
}T_DISPLAY_MOD;

/**@brief ����ģʽ*/
typedef enum  
{
	MOD_AV,						/**<AVģʽ*/
	MOD_TV						/**<TVģʽ*/
}T_TV_AV_MOD;

/**@brief ��ɫģʽ*/
typedef enum
{
	COLOR_RGB,					/**<RGBģʽģʽ*/
	COLOR_CVBS					/**<CVBSģʽ*/
}T_COLOR_MOD;


typedef enum
{
	MSG_VCR_ON,	/**< ��Ҫ���˵�� */
	MSG_VCR_OFF,	/**< ��Ҫ���˵�� */
	MSG_TV_ON,		/**< ��Ҫ���˵�� */
	MSG_TV_OFF
}T_SCART_POWER_STATUS_MSG;

typedef enum
{
	VCR_OFF_TV_OFF,	/**< ��Ҫ���˵�� */
	VCR_OFF_TV_ON,		/**< ��Ҫ���˵�� */
	VCR_ON_TV_OFF,		/**< ��Ҫ���˵�� */
	VCR_ON_TV_ON		/**< ��Ҫ���˵�� */
}T_SCART_POWER_STATUS;

/**@brief VCR����״̬*/
typedef enum
{
	H_FOR_VCR_ON,				/*GPIOΪ1��ʾVCR ON״̬*/
	L_FOR_VCR_ON				/*GPIOΪ0��ʾVCR ON״̬*/
}T_SCART_VCR_STATUS;

/**
@brief ����Scart�õ���i2c,�ú��������ڳ�ʼ��ǰ����

@param[in] I2CIndex I2c����
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE
*/
CSHDI_Error_t CSScartConfigI2CIndex(CSHDII2CIndex I2CIndex);

/**
@brief ��Scart���г�ʼ��

@param[in] type ��������, SCART_IC����IC���Ƶ�scart,SCART_HW����GPIO���Ƶ�scart
@param[in] PIOCfg  ��ScartΪGPIO����ʱ���ò���Ϊ����GPIO�ľ�������������������ΪSCART_IC���ò�����Ч��
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE������������󷵻�CSHDI_ERROR_BAD_PARAMETER���������Ѿ���ʼ��������CSHDI_ERROR_ALREADY_INITIALIZED
*/
CSHDI_Error_t CSScartInitialize( T_SCART_TYPE type, T_SCART_GPIO_CFG * PIOCfg );



/**
@brief ��Scart���г�ʼ��

@param[in] type ��������, SCART_IC����IC���Ƶ�scart,SCART_HW����GPIO���Ƶ�scart
@param[in] pPIOCfgEx  ��ScartΪGPIO����ʱ���ò���Ϊ����GPIO�ľ����������,��T_SCART_GPIO_CFG_EX���������ΪSCART_IC���ò�����Ч��
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE������������󷵻�CSHDI_ERROR_BAD_PARAMETER���������Ѿ���ʼ��������CSHDI_ERROR_ALREADY_INITIALIZED
*/
CSHDI_Error_t CSScartInitialize_Ext( T_SCART_TYPE type, T_SCART_GPIO_CFG_EX* pPIOCfgEx );

/**
@brief ����Ӳ�����ñ�ʾVCR״̬ʱ�ĵ�ƽֵ

@param[in] type ��������,SCART_IC����IC���Ƶ�scart,SCART_HW����GPIO���Ƶ�scart
@param[in] enmVCRStatus VCR����״̬
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE������������󷵻�CSHDI_ERROR_BAD_PARAMETER�������������δ֪����CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartSetVCRStatus( T_SCART_TYPE type, T_SCART_VCR_STATUS enmVCRStatus );

/**
@brief ����TV Scart ����źŵ���Դ

@param[in] SignalSource ��ʾģʽ����ѡֵ
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE������������󷵻�CSHDI_ERROR_BAD_PARAMETER�������������δ֪����CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartSignalSourceSwitch( T_SIGNAL_SOURCE SignalSource );

/**
@brief ����Scart Scart ����źŵ���Դ

@param[in] Scart Scartģʽ����ѡֵ
@param[in] SignalSource ��ʾģʽ����ѡֵ
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE������������󷵻�CSHDI_ERROR_BAD_PARAMETER�������������δ֪����CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartExtSignalSourceSwitch(  T_SCART_INDEX Scart, T_SIGNAL_SOURCE SignalSource );

/**
@brief ����Scart Scart���ӵ��ӵ���ʾģʽ

@param[in] Scart Scartģʽ����ѡֵ
@param[in] DisplayMode ��ʾģʽ����ѡֵ
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE������������󷵻�CSHDI_ERROR_BAD_PARAMETER�������������δ֪����CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScart16_9Switch( T_SCART_INDEX Scart,  T_DISPLAY_MOD DisplayMode);

/**
@brief ����Scart Scart���ӵ��ӵĹ���ģʽ

@param[in] Scart Scartģʽ����ѡֵ
@param[in] TVWorkMode ����ģʽ����ѡֵ
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE������������󷵻�CSHDI_ERROR_BAD_PARAMETER�������������δ֪����CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartTV_AVSwitch( T_SCART_INDEX Scart, T_TV_AV_MOD TVWorkMode);

/**
@brief ����Scart Scart���ӵ��ӵ���ɫģʽ

@param[in] Scart Scartģʽ����ѡֵ
@param[in] ColorMode ��ɫģʽ����ѡֵ
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE������������󷵻�CSHDI_ERROR_BAD_PARAMETER�������������δ֪����CSHDI_ERROR_UNKNOWN_DEVICE
*/
CSHDI_Error_t CSScartColorModeSwitch( T_SCART_INDEX Scart, T_COLOR_MOD ColorMode);

/**
@brief ע��ص�����������ͨ���ûص�������֪ͨӦ���Ƿ���VCR����

@param[in] function Ҫע��Ļص�����
@return ����ɹ�����CSHDI_SUCCESS;���ʧ�ܷ���CSHDI_FAILURE
*/
CSHDI_Error_t CSScartRegistCallBack( CSHDICallback function);

/**
@brief ��ȡSCART�������汾��

@param[in] nSize �汾�ų���
@param[out] pchVer �汾�Ŵ�ŵ�ַ
@return ���pchVerΪ�ջ��ߵ���ʧ���򷵻�0�����pchVerΪ�ǿշ��ذ汾����Ϣ����
*/
int CSSCARTGetVersion( PCHAR pchVer, int nSize );


#ifdef __cplusplus
}
#endif
/** @} */

#endif

