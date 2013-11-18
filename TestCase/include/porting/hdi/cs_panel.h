/**@defgroup CSPANEL panel
@brief �����ʾ��������ؽӿ�

@version 0.1 2008/10/23 ��ʼ�汾
@{
*/

#ifndef _CS_PANEL_H_
#define _CS_PANEL_H_

#include "CSHDI_typedef.h"
#include "udi_input.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
**                          Data structures		                                      
**********************************************************************/
/**@brief ������Ͷ���*/
typedef enum _CSPanelCntTye_t
{
	CSPANEL_CNT_164_LED,      	/**< ������ܣ���164����*/
	CSPANEL_CNT_164_LOCK,    	/**< û������ܣ���LED����164����*/
	CSPANEL_CNT_164_NO_LED,		/**< û������ܣ���164����*/
	CSPANEL_CNT_GPIO_NO_LED,    /**< ʹ��GPIO�����¿�����Ŀ�н���ʹ��ѡ��CSPANEL_CNT_GPIO_MATRIX*/
	CSPANEL_CNT_MCU,			/**< ʹ�õ�Ƭ���������*/
	CSPANEL_CNT_GPIO_MATRIX,	/**< ������gpio��ɰ���ɨ�����*/
	CSPANEL_CNT_SPI,            /**< ʹ��pt6964,�������湦��,����SPIЭ��>*/
	CSPANEL_CNT_ALL_GPIO,       /**< ʹ��GPIO��û��164����led��led��gpio����>*/
	CSPANEL_MAX_TYPE
}CSPanelCntTye_t;

/**@brief �尴LED�Ͱ���ɨ���IO������*/
typedef struct _CSPanelPinCfg_t
{
	BYTE m_pucComPinGroup[6]; 	/**< ��ͨ���: �������λѡ�ߵ�IO����Դ���ο�Ӳ�����*/	
	                            /**<CSPANEL_CNT_SPI:                              >*/
	                            /**0: m_nGPIOSTB ; 1:(�ϡ��¡����ҡ�menu��ok)�ж�; 2:power�ж�;3:power �жϴ�����ʽ��־λ����Ϊ GPIO_LOW_TO_HIGHģʽ����Ϊ0xfe >*/ 
								/**<CSPANEL_CNT_MCU:*/
								/**0:��������Ҫ����ʾʱ���򸳳�ֵ'y'����'Y'*/
	                            /**������� >*/
								/**�ص��ע: ��ʹ�õ�pin����,�븳��ֵΪ0xff >*/
								
	BYTE m_pucSegPinGroup[2];	/**< ��ͨ���: �������74HC164оƬ��Data��Clock��Ӧ��IO�ڣ�m_pnSegPinGroup[0]��ӦData�ߣ�m_pnSegPinGroup[1]��ӦClock��*/
	                            /**<CSPANEL_CNT_SPI: 0:m_nGPIOData; 1:m_nGPIOCLK >*/  
								/**�ص��ע: ��ʹ�õ�pin����,�븳��ֵΪ0xff >*/

	BYTE m_ucKeyScanPin; 		/**< ��ͨ���: 1.�ٴ������˲���Ϊ����ɨ���Ӧ��IO��.2.��������˲���Ϊ��оƬ�뵥Ƭ��ͨ��ʹ�õĴ�������*/
	                            /**<CSPANEL_CNT_SPI: ���� >*/
}CSPanelPinCfg_t;

typedef struct _CSLedPinCfg_t
{	
	/**
	- m_LedSegPinGroup[0]---- A
	- m_LedSegPinGroup[1]---- B
	- m_LedSegPinGroup[2]---- C
	- m_LedSegPinGroup[3]---- D
	- m_LedSegPinGroup[4]---- E
	- m_LedSegPinGroup[5]---- F
	- m_LedSegPinGroup[6]---- G
	- m_LedSegPinGroup[7]---- DP
	- m_LedComPinGroup[0]---- LED1
	- m_LedComPinGroup[1]---- LED2
	- m_LedComPinGroup[2]---- LED_COL
	- m_LedComPinGroup[3]---- LED3
	- m_LedComPinGroup[4]---- LED4
	- m_ucLedStandbyPin  ---- STANDBY
	- �ص��ע: ��ʹ�õ�pin����,�븳��ֵΪ0xff
	*/
	BYTE m_LedSegPinGroup[8];
	BYTE m_LedComPinGroup[7];
	BYTE m_ucLedStandbyPin;
}CSLedPinCfg_t;

/**@brief �����尴����GPIO���ã���Ӧ��ѡ��CSPANEL_CNT_GPIO_NO_LED*/
typedef struct _CSPanelKeyCfg_t
{
	/**
	- pKeyPinGroup[0]---- CH-UP����
	- pKeyPinGroup[1]---- CH-DOWN����
	- pKeyPinGroup[2]---- POWER����
	- pKeyPinGroup[3]---- ENTER����
	- pKeyPinGroup[4]---- CH-LEFT����
	- pKeyPinGroup[5]---- CH-RIGHT����
	- pKeyPinGroup[6]---- MUTE����
	- pKeyPinGroup[7]---- �ϡ��¡����ҡ�ok��menu�������жϿ�
	- �ص��ע: ��ʹ�õ�pin����,�븳��ֵΪ0xff
	*/
	BYTE m_aucKeyPinGroup[8];
	DWORD m_dwKeyNum;
}CSPanelKeyPinCfg_t;


/**@brief ��Ӧ��ѡ��CSPANEL_CNT_GPIO_MATRIX*/
typedef struct _CSPanelKeyMatrixCfg_S
{
	BYTE  *m_pucRowGpioIndex;	/**< ָ����GPIO������,����0��ʼ*/
	BYTE  *m_pucColGpioIndex;	/**< ָ����GPIO������,����0��ʼ*/
	DWORD m_dwRowNum;	/**< ���������*/
	DWORD m_dwColNum;	/**< ���������*/
}CSPanelKeyMatrixCfg_S;


/**@brief ��̬������ò���*/
typedef struct _CSStaticPanelPinCfg_S    
{
	BYTE m_ucPCLK;       		 /**< 74595 SH CLK����*/
	BYTE m_ucPSTB;       		 /**< 74595 OE��ST CLK���� ��ͨ����������������һ��*/
	BYTE m_ucPDATA;      		 /**< 74595 ������������*/
	BYTE m_ucSENSE0;     		 /**< �����ض�����*/
	BYTE m_ucSENSE1;     		 /**< �����ض�����*/
	BYTE m_ucSENSE2;     		 /**< �����ض�����*/
	BYTE m_ucSCAN0;      		 /**< ��鰴�����ţ�����δ����ʱ����������״̬*/
	BYTE m_ucSCAN1;      		 /**< ��鰴�����ţ�����δ����ʱ����������״̬*/	
}CSStaticPanelCfg_S;

/**@brief �����ʾ��ʱ�����*/
typedef struct _CSPanelTimeCfg_t
{
	BYTE m_ucHour;   		 /**< �������ʱ����ʾ��Сʱ*/
	BYTE m_ucMinute; 		 /**< �������ʱ����ʾ�ķ���*/
}CSPanelTimeCfg_t;

/**@brief �����LED��ʾ״̬����*/
typedef enum _CSPanelLEDStatus_t
{
	CSPANEL_LED_OFF,			/**<  LED����*/
	CSPANEL_LED_ON,			/**<  LED����	*/
	CSPANEL_LED_FL			/**<  LED����˸*/
}CSPanelLEDStatus_t;

/**@brief ����Ϸ��Ͱ���ʱ�İ���״̬����*/
typedef enum _CSPanelKeyStatus_t
{
	CSPANEL_KEY_PRESS,		/**<  ��ʾ��������*/
	CSPANEL_KEY_REPEAT,		/**<  ��ʾ�����ظ�*/
	CSPANEL_KEY_RELEASE		/**<  ��ʾ��������*/
}CSPanelKeyStatus_t;

typedef struct _CSPanelKeyParam_t
{
	CSPanelKeyStatus_t	m_enmPanelKeyStatus;  	/**<  ����״̬����ʾ�������»���*/
	BYTE			m_ucPanelKeyCode;			/**<  �����ļ�ֵ*/
}CSPanelKeyParam_t;

typedef struct _CSPanelDecode_S
{
	int m_nKeyValue;	/**< ת��֮��İ�����ֵ*/
	int m_nLogicValue;	/**< ԭʼ����ֵ*/
	BOOL m_bKeyRepeat;	/**< �Ƿ�֧��repeat*/
}CSPanelDecode_S;

typedef enum _CSPanelLEDType_t
{
	CSPANEL_LED_MAIL,     	/**< �������Mail��*/
	CSPANEL_LED_SIGNAL,  	/**<  ������� Signal��*/
	CSPANEL_LED_POWER,  	/**< �������Power��*/
	CSPANEL_LED_AUDIO, 	    /**< �������Audio��*/
	CSPANEL_LED_CMRS,   	/**< Cable Modem �շ�״̬��*/
	CSPANEL_LED_CMONLINE,   /**< Cable Modem ����ָʾ��*/
	CSPANEL_LED_CMUPLINK,	/**< Cable Modem ����״ָ̬ʾ��*/
	CSPANEL_LED_CMDOWNLINK,  /**< Cable Modem ����״ָ̬ʾ��*/
	CSPANEL_LED_CLOCK    	/**< �������ʱ���ƣ��������м��":" */  
}CSPanelLEDType_t;

/**@deprecated CSPANEL_LED_EXT0�ѱ�CSPANEL_LED_CMRSȡ���������ڴ˽�Ϊ���ּ��ݣ��µİ汾���᳹���Ƴ� */
#define CSPANEL_LED_EXT0 CSPANEL_LED_CMRS

/**@deprecated CSPANEL_LED_EXT1�ѱ�CSPANEL_LED_CMONLINEȡ���������ڴ˽�Ϊ���ּ��ݣ��µİ汾���᳹���Ƴ� */
#define CSPANEL_LED_EXT1 CSPANEL_LED_CMONLINE

/**@deprecated CSPANEL_LED_EXT2�ѱ�CSPANEL_LED_CMUPLINKȡ���������ڴ˽�Ϊ���ּ��ݣ��µİ汾���᳹���Ƴ� */
#define CSPANEL_LED_EXT2 CSPANEL_LED_CMUPLINK

/**@deprecated CSPANEL_LED_EXT3�ѱ�CSPANEL_LED_CMDOWNLINKȡ���������ڴ˽�Ϊ���ּ��ݣ��µİ汾���᳹���Ƴ� */
#define CSPANEL_LED_EXT3 CSPANEL_LED_CMDOWNLINK

typedef enum _CSPanelBootType_E
{
	CSPANEL_BOOT_BY_KEY,   		/**< ����ң������ǰ��尴������*/
	CSPANEL_BOOT_BY_TIME,      		/**< ��ʱ����*/
	CSPANEL_BOOT_BY_POWER_ON,		/**<�������ϵ�����*/
	CSPANEL_BOOT_TYPE_NOT_DEF  	/**< δ���������*/
}CSPanelBootType_E;

typedef struct _CSPanelIR_S
{
	DWORD	 	m_dwIRProtocol;  /**< ң����Э��,ʹ��CSIRProtocol_tö������*/
	DWORD		m_dwUserID;		/**< �û���*/
	DWORD		m_dwScanKeyCode;	/**< ɨ����*/
	DWORD		m_dwLogicKeyCode;	/**< reserved*/
}CSPanelIRSpecialKey_S;

/**
@brief �����ʾHook
@param[in] penmLEDType    ��ʾ������ö��
@param[in] penmLEDStatus  ��ʾ������״̬
@param[in] pbSkipDefault �Ƿ����Ĭ�ϲ�����TRUE��ʾִֻ��Hook��������Ĳ�����FALSE��ʾͬʱִ��CSPanelSetDisplayLEDĬ�ϲ���
@return CSUDI_SUCCESS ��ʾ�ɹ���ʧ���򷵻ش������ֵ
*/
typedef CSHDI_Error_t (*CSPanelDisplayLedHookFunc_F)(CSPanelLEDType_t* eLedType, CSPanelLEDStatus_t* eLedStatus, BOOL *pbSkipDefault);

/**
@brief ����ǰ��������

���������CSPanelCntTye_t�Ķ��壬�ڳ�ʼ��֮ǰ����
@param[in] enmPanelCntType ������࣬�μ�CSPanelCntTye_t
@return CSHDI_SUCCESS��ʾ�����������ɹ���CSHDI_FAILURE��ʾ�����������ʧ��
@note �����øú�����Ĭ��ֵΪCSPANEL_CNT_164_LED
*/
CSHDI_Error_t CSPanelSetType(CSPanelCntTye_t enmPanelCntType);

/**
@brief ���������ж����ȼ�

@param[in] nIntLevel ����ж����ȼ���ȡֵ��ΧΪ0-255
@return CSHDI_SUCCESS��ʾ����жϼ������óɹ���CSHDI_FAILURE��ʾ�������жϼ�������ʧ��
*/
CSHDI_Error_t CSPanelSetInterruptLevel( int nIntLevel );

/**
@brief ȫgpio����ʼ�����������İ����������ȫ��Ϊgpioֱ����ʽ

@param[in] nIntLevel ����ж����ȼ���ȡֵ��ΧΪ0-255
@param[in] pKeyPinGroup �����尴����GPIO����
@param[in] pLedPinGroup led����ܵĹܽ�����
@return CSHDI_SUCCESS��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ����ʼ��ʧ�ܡ�
*/
CSHDI_Error_t CSPanelAllGPIOInitialize(int nPriority, CSPanelKeyPinCfg_t *psKeyPinGroup, CSLedPinCfg_t * psLedPinGroup);

/**
@brief ����ʼ��

�����������LSIƽ̨�����жϣ�STƽ̨�����ȼ���IO����Դ
@param[in] nPriority �������LSIƽ̨�����жϣ�STƽ̨�����ȼ���ȡֵ��ΧΪ0-255
@param[in] pPinGroup ���IO����Դ����
@return CSHDI_SUCCESS��ʾ����ʼ���ɹ���CSHDI_FAILURE��ʾ����ʼ��ʧ��
@note �������ȼ�Ϊ0-255��ֵԽ�����ȼ�Խ�ߡ��Ƽ����ȼ�Ϊ200-230��������ȼ�̫�Ϳ��ܻ������˸������
*/
CSHDI_Error_t CSPanelInitialize( int nPriority, CSPanelPinCfg_t *pPinGroup );

/**
@brief ����ʼ������Ӧ��ѡ��CSPANEL_CNT_GPIO_NO_LED�ĳ�ʼ������

��������GPIO��ⰴ����û��164��û������ܵ����
@param[in] nPriority �������LSIƽ̨�����жϣ�STƽ̨�����ȼ���ȡֵ��ΧΪ0-255
@param[in] pKeyPinGroup �����尴����GPIO����
@return CSHDI_SUCCESS��ʾ����ʼ���ɹ���CSHDI_FAILURE��ʾ����ʼ��ʧ��
@note �������ȼ�Ϊ0-255��ֵԽ�����ȼ�Խ�ߡ��Ƽ����ȼ�Ϊ200-230��������ȼ�̫�Ϳ��ܻ������˸������
*/
CSHDI_Error_t CSPanelInitialize_GPIO( int nPriority, CSPanelKeyPinCfg_t *pKeyPinGroup );


/**
@brief ����ʼ������Ӧ��ѡ��CSPANEL_CNT_GPIO_MATRIX�ĳ�ʼ������

��������GPIO��ⰴ����û��164��û������ܵ����
@param[in] nPriority �������LSIƽ̨�����жϣ�STƽ̨�����ȼ���ȡֵ��ΧΪ0-255
@param[in] pKeyPinGroup �����尴����GPIO����
@return CSHDI_SUCCESS��ʾ����ʼ���ɹ���CSHDI_FAILURE��ʾ����ʼ��ʧ��
@note �������ȼ�Ϊ0-255��ֵԽ�����ȼ�Խ�ߡ��Ƽ����ȼ�Ϊ200-230��������ȼ�̫�Ϳ��ܻ������˸������
*/
CSHDI_Error_t CSPanelInitializeGpioMatrix( int nPriority, CSPanelKeyMatrixCfg_S *pKeyPinGroup );


/**
@brief ����ʼ��

����ʼ���������������ջ��С���ʺ��ڲ�ʹ��GPIO���õ����
@param[in] nPriority �������LSIƽ̨�����жϣ�STƽ̨�����ȼ���ȡֵ��ΧΪ0-255
@param[in] nStackSize ��������ջ��С
@param[in] pPinGroup ���IO����Դ����
@return CSHDI_SUCCESS��ʾ����ʼ���ɹ���CSHDI_FAILURE��ʾ����ʼ��ʧ��
@note �������ȼ�Ϊ0-255��ֵԽ�����ȼ�Խ�ߡ��Ƽ����ȼ�Ϊ200-230��������ȼ�̫�Ϳ��ܻ������˸������
*/
CSHDI_Error_t CSPanelInitialize_Ext( int nPriority,int nStackSize,CSPanelPinCfg_t *pPinGroup );

/**
@brief ����ʼ��

�����������ջ��С���ʺ���ʹ��GPIO��ⰴ����������164�����
@param[in] nPriority �������LSIƽ̨�����жϣ�STƽ̨�����ȼ���ȡֵ��ΧΪ0-255
@param[in] nStackSize ��������ջ��С
@param[in] pKeyPinGroup ��尴��IO����Դ����
@return CSHDI_SUCCESS��ʾ����ʼ���ɹ���CSHDI_FAILURE��ʾ����ʼ��ʧ��
@note �������ȼ�Ϊ0-255��ֵԽ�����ȼ�Խ�ߡ��Ƽ����ȼ�Ϊ200-230��������ȼ�̫�Ϳ��ܻ������˸������
*/
CSHDI_Error_t CSPanelInitialize_Ext_GPIO( int nPriority,int nStackSize,CSPanelKeyPinCfg_t *pKeyPinGroup);

/**
@brief ����ʼ��

�����������ջ��С�������ھ�̬���
@param[in] nPriority �������LSIƽ̨�����жϣ�STƽ̨�����ȼ���ȡֵ��ΧΪ0-255
@param[in] psPinGroup ��尴��IO����Դ����
@return CSHDI_SUCCESS��ʾ����ʼ���ɹ���CSHDI_FAILURE��ʾ����ʼ��ʧ��
@note �������ȼ�Ϊ0-255��ֵԽ�����ȼ�Խ�ߡ��Ƽ����ȼ�Ϊ200-230��������ȼ�̫�Ϳ��ܻ������˸������
*/
CSHDI_Error_t CSStaticPanelInitialize( int nPriority, CSStaticPanelCfg_S *psPinGroup);

/**
@brief ����ʼ��

ʹ��pt6964оƬ���������湦�ܣ����水��ֵ
@param[in] nPriority �������LSIƽ̨�����жϣ�STƽ̨�����ȼ���ȡֵ��ΧΪ0-255
@param[in] psPinGroup ��尴��IO����Դ����
@return CSHDI_SUCCESS��ʾ����ʼ���ɹ���CSHDI_FAILURE��ʾ����ʼ��ʧ��
*/
CSHDI_Error_t CSPanelSPIInit( int nPriority, CSPanelPinCfg_t *pPinGroup );

/**
@brief ע����尴���ص�����

���������⵽�������»���ʱͨ��ע��Ļص�����֪ͨӦ�ò�
@param[in] function �ص��������û�ͨ���ص������õ�������Դ��״̬�ͼ�ֵ
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelRegisterKeySendCallback( CSHDICallback function );

/**
@brief �������ʾ�����Կ����ĸ��߶ιܺ�����Բ�����ʾ

@param[in] pucSegVal
- pucSegVal[0]�������һ���߶ιܵĶ�ѡ
- pucSegVal[1]������ڶ����߶ιܵĶ�ѡ
- pucSegVal[2]��Բ��Ķ�ѡ
- pucSegVal[3]������������߶ιܵĶ�ѡ
- pucSegVal[4]��������ĸ��߶ιܵĶ�ѡ
@param[in] ucSize Ҫ��ʾ���߶ιܸ�����nSize���Ϊ5����nSizeС��5����������뽫����ʾ
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelDisplaySegment( BYTE *pucSegVal, BYTE ucSize );

/**
@brief ������尴���Ƿ���ظ�

@param[in] ucSingleKeyValue Ҫ���õİ����ļ�ֵ
@param[in] bISRepeat 
- FALSE�����������ظ���ֻ�ڰ������º͵���ʱ�����ص�����״̬�ǰ�����Ĭ��״̬
- TRUE���������ظ����ڰ������²���ʱ���������������ص�
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelSetSingleKeyRepeat( BYTE ucSingleKeyValue,BOOL bISRepeat );

/**
@brief ���ɨ��

�����޲���ϵͳƽ̨���������ʾ�Ͱ���ɨ�衣
�û���Ҫ�ڵ��øú���ǰ����Ҫ��ʾ�����ݣ����ڵ��øú���ʱ����ɨ�������ÿ��ɨ�����ʱʱ�䡣
@param[in] dwScanTime ���ɨ����������ڿ����������ʾ�Ͱ���ɨ��ĳ���ʱ��
@param[in] dwDelayTime ���ڿ���ÿ��ɨ�����ʱʱ�䣬��λ��1��ʱ������
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelScan (DWORD dwScanTime, DWORD dwDelayTime);

/**
@brief �����޲���ϵͳ�ĵ����������ʾ

@param[in] ucSegValue ����ܵĶ�ѡ
@param[in] ucComIndex �����λѡ��0�������һ���߶ιܣ�1������ڶ����߶ιܣ�2��Բ�㣬3������������߶ιܣ�4��������ĸ��߶ι�
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelSetSingleSegment( BYTE ucSegValue, BYTE ucComIndex );

/**
@brief �����޲���ϵͳ���������ʾ��ͬ����

@param[in] ucSegValue ����ܵĶ�ѡ
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelSetAllSegment( BYTE ucSegValue );

/**
@brief ����ð�ŵ���˸Ƶ��
@param[in] nTimeDiv ���õ�ֵԽС����˸Խ�죬ֵԽ����˸Խ��
@return CSHDI_SUCCESS��ʾ��ʾ�ɹ���CSHDI_FAILURE��ʾ��ʾʧ�ܡ�
*/
CSHDI_Error_t	CSPanelConfigDisplayTimeDiv(int nTimeDiv);

/**
@brief ����LED�Ƶ���˸Ƶ��

@param[in] nLEDDiv ���õ�ֵԽС����˸Խ�죬ֵԽ����˸Խ��
@return CSHDI_SUCCESS��ʾ��ʾ�ɹ���CSHDI_FAILURE��ʾ��ʾʧ�ܡ�
*/
CSHDI_Error_t	CSPanelConfigLEDFlashDiv(int nLEDDiv);

/**
@brief ��ȡ����İ汾��

@param[out] pchVer ������Ű汾��Ϣ
@param[in] nSize pchVer�ĳ���
@return �汾��Ϣ��ʵ�ʳ���
@note This function is used to get version information.Display format is:\n
"[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSPANELGetVersion( PCHAR pchVer, int nSize );

/**
@brief ��ʾʱ��

@param[in] pCSTime ʱ��
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelDisplayTime( CSPanelTimeCfg_t *pCSTime );

/**
@brief ��ʾ�ź�ǿ��

@param[in] ucStrength ��ʾ�ź�ǿ�ȣ�ȡֵ��Χ��0-10
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelDisplaySignalStrength( BYTE ucStrength );

/**
@brief �������LED����ʾ

@param[in] enmLEDType ѡ��Ҫ��ʾ��LED��
@param[in] enmLEDStatus LED�Ƶ���ʾ״̬
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelDisplayLED( CSPanelLEDType_t enmLEDType, CSPanelLEDStatus_t enmLEDStatus );

/**
@brief �������ǰ���7�ι���ʾ

���ú�7�ι�Ϩ�𣬵���Ӱ��Mail,Siganl,Power,Aduio��
*/
void CSPanelClear( void );

/**
@brief ����ǰ��尴����ʵ��ֵ���߼�ֵ֮��Ĺ�ϵ

@param[in] psKeyList ����ʵ��ֵ���߼�ֵ��Ӧ��ϵ��ָ�롣���CSPanelDecode_S���塣
@param[in] nKeyNumber ǰ��尴���������ò�������psKeyList����ĳ��ȡ�
@return CSHDI_SUCCESS��ʾ��ʾ�ɹ���CSHDI_FAILURE��ʾ��ʾʧ�ܡ�
*/
CSHDI_Error_t  CSPanelAddEquipment( const CSPanelDecode_S *psKeyList, int nKeyNumber);
	
/**
@brief ������ַ���ʾ��

���в���ϵͳ��ƽ̨���������ʾ�ڵ��øú������Զ����£�
�޲���ϵͳ��ƽ̨��Ҫ�ڵ��øú��������CSPanelScan�����������������ʾ��
@param[in] pcText Ҫ���������ʾ���ַ���������������ܿ���ʾ���ַ�����������ܽ�����ʾ��������ʾ�ĸ��ַ���
@param[in] ucSize Ҫ��ʾ���ַ��������Ϊ4����nSizeС��4���ַ��������ʾ
@return CSHDI_SUCCESS��ʾ�ɹ���CSHDI_FAILURE��ʾʧ��
*/
CSHDI_Error_t CSPanelDisplayText( char *pcText, BYTE ucSize );


/**
@brief ���û����д���ʱ�䣬�����õ�ʱ�䵽��������Զ�����

@param[in] psStandTime ����ʱ�䣬Ŀǰֻ֧��Сʱ�����ӡ�������3Сʱ��20���ӣ���ػ�3Сʱ20���Ӻ󣬻�����������
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSPanelSetStandbyTime (const CSPanelTimeCfg_t *psStandTime);


/**
@brief �����д��������øú����Ժ󣬵�Ƭ�������ж������Դ��
@param[in] ���������Ӧ��ң���������б�ң������Ŀ
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSPanelStandby(const CSPanelIRSpecialKey_S *psIRSpecialKeyList,BYTE ucSize);



/**
@brief���õ�Ƭ���ܽŵ�ƽ������ͨ����Ƭ�����Ƶ����

@param[in] ucPinIndex Ҫ���õ�ƽ�ĵ�Ƭ���ܽŵ�����,51��Ƭ���Ĺܽž��ܱ�ʾΪPm.n����ʽ,��Pm.n��Ӧ��������Ϊm*8+n,��P1.6�ܽŵ�����Ϊ1*8+6=14
@param[in] ucLevel Ҫ���óɵĵ�ƽֵ,0��1
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSPanelSetPinLevel(BYTE ucPinIndex,  BYTE ucLevel);


/**
@brief��ȡ������ʽ����

@param[out] peBootType ������ʽ��ŵ�ַ
@return CSHDI_SUCCESS��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ�ܡ�
*/
CSHDI_Error_t CSPanelGetBootType(CSPanelBootType_E  *peBootType);

/**
@brief ���ע��Hook����
@param[in] fnHook   Ҫע���Hook���� 
@return CSUDI_SUCCESS ��ʾע��ɹ���ʧ����ע��ʧ��
*/
CSHDI_Error_t CSPanelSetDisplayLedHook(CSPanelDisplayLedHookFunc_F fnHook);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
