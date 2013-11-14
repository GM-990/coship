/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

#ifndef _UDI2_PANEL_PLUS_H_
#define _UDI2_PANEL_PLUS_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "udi2_typedef.h"


#define	UDIPANEL_LED_MAX_COM_PIN 6

/**@brief Panelģ��Panel����ö�ٶ���*/
typedef enum
{
	EM_PANEL_TYPE_COMMON_LED = 0, ///<��ͨLED���
	EM_PANEL_TYPE_MCU_LED,	///<ʹ�õ�Ƭ�����Ƶ�LED���
	EM_PANEL_TYPE_COMMON_VFD, ///<��ͨVFD���
	EM_PANEL_TYPE_MCU_VFD,	///<ʹ�õ�Ƭ�����Ƶ�VFD���
	EM_PANEL_TYPE_SPI_LED,  ///<���ʹ��pt6964,��������湦�ܣ�����SPIЭ��
	EM_PANEL_TYPE_COMMON_NO_LED,  ///<��ͨ���ʹ��74HC164����LED
	EM_PANEL_TYPE_GPIO_LED, ///<��ͨ��壬ȫ��gpio���ƣ�û��74H164����LED
	EM_PANEL_TYPE_GPIO_NO_LED,///<��ͨ��壬gpio���ư�����û��led��ʹ��164��
	EM_PANEL_TYPE_MAX
}CSUDIPanelType_E;


/**@brief Panelģ����ͨLED���Ӳ�����ýṹ��(���Ÿ���)*/
typedef struct
{
	unsigned char m_aucGpioComPin[UDIPANEL_LED_MAX_COM_PIN]; 	///< LED���λѡ�ߵ�GPIO�ڣ��ο�Ӳ�����
	                             ///<pt6964���  0: m_nGPIOSTB, 1: �ж�����1, 2: �ж�����2
	                             ///<���౸��
	                             ///<ע��:��ʹ�õ�pin�����븳ֵΪ0xFF
	unsigned char m_ucGpioData;	 ///< �������164оƬ��Data���ŵ�GPIO
	                             ///<pt6964���  m_nGPIOData
	                             ///<ע��:��ʹ�õ�pin�����븳ֵΪ0xFF
	unsigned char m_ucGpioClk;	 ///< �������164оƬ��clock���ŵ�GPIO
	                             ///<pt6964���  m_nGPIOCLK
	                             ///<ע��:��ʹ�õ�pin�����븳ֵΪ0xFF
	unsigned char m_ucGpioKey; 	 ///< ���ɨ�谴���� GPIO
	                             ///<pt6964���  ����
	                             ///<ע��:��ʹ�õ�pin�����븳ֵΪ0xFF
	unsigned char m_aucReserved[3];///<pt6964���  ����
								   ///<ע��:��ʹ�õ�pin�����븳ֵΪ0xFF
	                            
}CSUDIPanelCommonLedCfg_S;


/**@brief Panelģ����ͨVFD���Ӳ�����ýṹ��*/
typedef struct
{
	unsigned char m_ucGpioSTB;		///< ʹ�ܿ��ƽţ���ʹ��ʱһ����Ϊ0xff
	unsigned char m_ucGpioDIn; 		///< ��������ţ�������ͨ�������Ŵ�PT6315 �����ݣ���ʹ��ʱһ����Ϊ0xff
	unsigned char m_ucGpioDout; 	///< ��������ţ�������ͨ����������PT6315д���� ����ʹ��ʱһ����Ϊ0xff
	unsigned char m_ucGpioClk; 		///< ʱ�� �ߣ���ʹ��ʱһ����Ϊ0xff
}CSUDIPanelCommonVfdCfg_S;


/**@brief Panelģ��MCU���Ƶ����Ӳ�����ýṹ�壬��Ƭ�����Ƶ�LED��VFD����ʹ�ô����ݽṹ*/
typedef struct
{
	int m_nUartIndex;	///<��оƬ�뵥Ƭ��ͨ���õĴ�������
	unsigned int m_unBaudrate;		///<����ʹ�õĲ�����
}CSUDIPanelMcuCfg_S;

/**@brief Panelģ�飬��ͨ��壬��gpio���Ƶ����ʹ�ô����ݽṹ
����ΪEM_PANEL_TYPE_GPIO_NO_LED�����ʹ�øýṹ���m_ulKeyNum��m_ucKeyPinGroup[8]������Ա������ĳ�Ա��Ϊ0xff��
����ΪEM_PANEL_TYPE_GPIO_LED�����ýṹ������г�Ա����Ҫ���ã���ʹ�õ�gpio����Ϊ0xff.*/
typedef struct
{
	unsigned char m_ucLedSegPinGroup[8];//<�����seg�ܽţ���ʹ��ʱһ����Ϊ0xff
	unsigned char m_ucLedComPinGroup[7];//<�����com�ܽ�,��ʹ��ʱһ����Ϊ0xff
	unsigned char m_ucLedStandby;//<�����standby�ܽţ���ʹ��ʱһ����Ϊ0xff
	unsigned int  m_ulKeyNum;////������������ʹ��ʱһ����Ϊ0xff
	unsigned char m_ucKeyPinGroup[8];//����gpio���ã���ʹ��ʱһ����Ϊ0xff
}CSUDIPanelGpioCfg_S;

/**@brief Panelģ��Ӳ����Ϣ�ṹ�嶨��*/
typedef struct
{
	CSUDIPanelCommonLedCfg_S m_sLEDCfg; ///<��ͨLED���Ӳ�����ã���Ӧ��EM_PANEL_TYPE_COMMON_LED
	CSUDIPanelCommonVfdCfg_S m_sVFDCfg; ///<��ͨVFD���Ӳ�����ã���Ӧ��EM_PANEL_TYPE_COMMON_VFD
	CSUDIPanelMcuCfg_S m_sMCUCfg; ///<MCU���Ƶ����Ӳ������,��Ӧ��EM_PANEL_TYPE_MCU_LED �� EM_PANEL_TYPE_MCU_VFD
}CSUDIPanelHWCfg_S;

/**@brief panel initilize param structure*/
typedef struct _CSUDIPanelParams_S
{
	int m_nMySize; ///<�˽ṹ���С��sizeof(CSUDIPanelParams_S)
	CSUDIPanelType_E m_ePanelType;	///<panel����
	CSUDIPanelHWCfg_S m_ePanelHwCfg;///<�������͵�panelӲ������
}CSUDIPanelParams_S;

/**
@brief Panelģ���ʼ��
@param[in] nIndex panel����
@param[in] psParms ��ʼ��Panelģ������Ҫ�Ĳ���
@return CSUDI_SUCCESS��ʾ�ɹ�,�����豸�ŷ���CSUDIPANEL_ERROR_INVALID_DEVICE_ID,��ʼ��ʧ�ܷ���CSUDI_FAILURE
@note: �ýӿ��Ѳ��Ƽ�ʹ��,��ʹ��CSUDIPanelInitExt
*/
CSUDI_Error_Code CSUDIPanelInit(int nIndex, CSUDIPanelParams_S *psParms);

/**
@brief Panelģ���ʼ��
@param[in] nIndex panel����
@param[in] ePanelType panel����
@param[in] pParms �ò�������ePanelTypeָ���Ĳ�ͬpanel���ͣ���ָ��ͬ�����ݽṹ����ϸ���£�
	EM_PANEL_TYPE_COMMON_LED��pParmsΪָ��CSUDIPanelCommonLedCfg_S��ָ��
	EM_PANEL_TYPE_SPI_LED��pParmsΪָ��CSUDIPanelCommonLedCfg_S��ָ��
	EM_PANEL_TYPE_COMMON_NO_LED��pParmsΪָ��CSUDIPanelCommonLedCfg_S��ָ��
	EM_PANEL_TYPE_MCU_LED��pParmsΪָ��CSUDIPanelMcuCfg_S��ָ��
	EM_PANEL_TYPE_GPIO_LED��pParmsΪָ��CSUDIPanelGpioCfg_S��ָ��
	EM_PANEL_TYPE_GPIO_NO_LED��pParmsΪָ��CSUDIPanelGpioCfg_S��ָ��
@return 
	-�ɹ����� CSUDI_SUCCESS
	-�����豸�ŷ��� CSUDIPANEL_ERROR_INVALID_DEVICE_ID
	-�������󷵻� CSUDIPANEL_ERROR_BAD_PARAMETER
	-��ʼ��ʧ�ܷ��� CSUDI_FAILURE
@note 
*/
CSUDI_Error_Code CSUDIPanelInitExt(int nIndex,CSUDIPanelType_E ePanelType,void *pParms);

/**
@brief ��ͨLED����ʼ��,���ô˽ӿڳ�ʼ�����������ɼ�С���ӿ�Ĵ�С
*/
CSUDI_Error_Code CSUDIPanelInitCommonLed(int nIndex, CSUDIPanelParams_S *psParms);

/**
@brief ��ͨVFD����ʼ��,���ô˽ӿڳ�ʼ�����������ɼ�С���ӿ�Ĵ�С
*/
CSUDI_Error_Code CSUDIPanelInitCommonVfd(int nIndex, CSUDIPanelParams_S *psParms);

/**
@brief ��Ƭ�����Ƶ�LED����ʼ��,���ó�ʼ�����������ɼ�С���ӿ�Ĵ�С
*/
CSUDI_Error_Code CSUDIPanelInitMCULed(int nIndex, CSUDIPanelParams_S *psParms);

/**
@brief ��Ƭ�����Ƶ�VFD����ʼ��,���ó�ʼ�����������ɼ�С���ӿ�Ĵ�С
*/
CSUDI_Error_Code CSUDIPanelInitMCUVfd(int nIndex, CSUDIPanelParams_S *psParms);


#ifdef __cplusplus
}
#endif

#endif

