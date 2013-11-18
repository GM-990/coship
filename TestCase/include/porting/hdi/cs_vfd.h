/**@defgroup CSVFD[CSVFD]ͬ����Ŀר��VFD�����ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨�޹�VFD�����ӿ�
@brief ���ӫ����ʾ��VFD(Vacuum Fluorescent Display)������udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
 
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/

#ifndef _CS_VFD_H_
#define _CS_VFD_H_


#include "cs_typedef.h"
#include "cs_assert.h"
#include "CSHDI_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief VFD��ʾ��������*/
typedef enum
{
	VFD_LED_PLAY, 		/**< ���� */ 
	VFD_LED_PAUSE, 	/**<��ͣ */ 
	VFD_LED_STOP, 		/**< ֹͣ */ 
	VFD_LED_HDTV,		/**< ���� */ 
	VFD_LED_BACKWARD, /**< HDTV */ 
	VFD_LED_FORWARD, 	/**< ��� */ 
	VFD_LED_BALL, 		/**< Բ*/ 
	VFD_LED_DUBY, 		/**< �ű� */ 
	VFD_LED_PREVIOUS, 	/**< ǰһ��*/ 
	VFD_LED_NEXT, 		/**< ��һ��*/ 
	VFD_LED_IR, 		/**< ң����*/ 
	VFD_LED_USB, 		/**< USB*/ 
	VFD_LED_N,			/**< N*/ 
	VFD_LED_VOD, 		/**< VOD */
	VFD_LED_POWER, 	/**< ��Դ*/ 
	VFD_LED_TV, 		/**< ����*/ 
	VFD_LED_NETWORK, 	/**< ����*/ 
	VFD_LED_RADIO, 	/**< �㲥*/ 
	VFD_LED_CLOCK,		/**< ʱ��*/ 
	VFD_LED_MODEM,	/**< MODEM */ 
	VFD_LED_REC		/**< ¼�� */ 
}T_CS_VFD_LEDS;
	

/**@brief VFD��ʾģʽ����*/
typedef enum
{
	VFD_DIGIT_4_SEG_24,	/**< ��Ҫ���˵�� */
	VFD_DIGIT_5_SEG_23,	/**< ��Ҫ���˵�� */
	VFD_DIGIT_6_SEG_22,	/**< ��Ҫ���˵�� */
	VFD_DIGIT_7_SEG_21,	/**< ��Ҫ���˵�� */
	VFD_DIGIT_8_SEG_20,	/**< ��Ҫ���˵�� */
	VFD_DIGIT_9_SEG_19,	/**< ��Ҫ���˵�� */
	VFD_DIGIT_10_SEG_18,	/**< ��Ҫ���˵�� */
	VFD_DIGIT_11_SEG_17,	/**< ��Ҫ���˵�� */
	VFD_DIGIT_12_SEG_16	/**< ��Ҫ���˵�� */
}T_CS_VFD_DIS_MODE;

/**@brief VFD��ʾ���ȶ���*/
typedef enum
{
	VFD_WIDTH_1_16, 		/**< ���� */
	VFD_WIDTH_2_16, 		/**< ��Ҫ���˵�� */
	VFD_WIDTH_4_16, 		/**< ��Ҫ���˵�� */
	VFD_WIDTH_10_16, 		/**< ��Ҫ���˵�� */
	VFD_WIDTH_11_16, 		/**< ��Ҫ���˵�� */
	VFD_WIDTH_12_16, 		/**< ��Ҫ���˵�� */
	VFD_WIDTH_13_16, 		/**< ��Ҫ���˵�� */
	VFD_WIDTH_14_16 		/**< ��ǿ */
}T_CS_VFD_DIMMING; 

/**@brief VFD��ʾ���ض���*/
typedef enum
{
	VFD_LED_OFF,		/**< VFD��ʾ�ر�*/
	VFD_LED_ON			/**< VFD��ʾ��*/
}T_CS_VFD_LED_STATUS;

/**@brief VFD��ʾ���ض���*/
typedef enum
{
	VFD_COL_ON,		/**< ��Ҫ���˵�� */
	VFD_COL_FLS,		/**< ��Ҫ���˵�� */
	VFD_COL_OFF		/**< ��Ҫ���˵�� */
}T_CS_VFD_COL_STATUS;

/**@brief VFD ����״̬����*/
typedef enum 
{
	CSVFDPANEL_KEY_PRESS, 		/**< ��ʾ�������� */
	CSVFDPANEL_KEY_REPEAT, 		/**< ��ʾ�����ظ� */
	CSVFDPANEL_KEY_RELEASE 		/**< ��ʾ�������� */
}T_CS_VFDPanel_Key_Status; 		

/**@brief VFD ��ʾʱ�ӵ����㷽������*/
typedef enum
{
	TWO_POINT_FIRST, /**< ��ߵ����� */
	TWO_POINT_SECOND /**< �ұߵ����� */
}T_CS_VFDPanel_Two_Points;

/**@brief VFD �豸������������*/
typedef enum
{
	BOOT_BY_POWER,   		/**< ����ң������ǰ���Ŀ���������*/
	BOOT_BY_TIME,      		/**< ��ʱ����*/
	BOOT_BY_RESTART,		/**< �ϵ��������µ�����*/
	BOOT_TYPE_NOT_DEF	/**< δ�����������ʽ*/
}CS_VFD_BOOT_TYPE_E;

/**@brief VFD �����ص���������*/
typedef struct 
{
	T_CS_VFDPanel_Key_Status enmVFDPanelKeyStatus; 	/**< ����״̬ */
	BYTE ucVFDPanelKeyCode;						/**< ������ֵ */
}T_CS_VFDPanel_KeyParam; 				

/**@brief VFD ������ʼ����������*/
typedef struct
{
	int nGpioSTB; 		/**< ʹ�ܿ��ƽ� */
	int nGpioDIn; 		/**< ��������ţ�������ͨ�������Ŵ�PT6315 ������*/
	int nGpioDout; 		/**< ��������ţ�������ͨ����������PT6315д���� */
	int nGpioClk; 		/**< ʱ�� */
	T_CS_VFD_DIS_MODE enmDisplayMode; 	/**< VFD����ʾģʽ��һ��ΪVFD_DIGIT_10_SEG_18 */
	T_CS_VFD_DIMMING enmDimming;			/**< ��Ҫ���˵�� */
	BOOL bKeyScan; 		/**< ��ʾ�Ƿ���Ҫ֧�ּ���ɨ��,TRUE:֧��,FALSE:��֧�� */
}T_CS_VFDPanel_InitParam; 


/**@brief VFD ��������*/
typedef struct _CSVFDKeyDef_t
{
	BYTE	m_ucKeyValue;		/**< ��Ҫ���˵�� */
	BOOL	m_bRepeatEnable;	/**< ��Ҫ���˵�� */
}CSVFDKeyDef_t;


/**@brief VFD ��ʾʱ�䶨��*/
typedef struct _CSVFDTimeCfg_S
{
	WORD m_wYear;			/**< ��Ҫ���˵�� */
	BYTE m_ucMonth;		/**< ��Ҫ���˵�� */
	BYTE m_ucDay;			/**< ��Ҫ���˵�� */
	BYTE m_ucHour;			/**< ��Ҫ���˵�� */
	BYTE m_ucMinute;		/**< ��Ҫ���˵�� */
	BYTE m_ucSecond;		/**< ��Ҫ���˵�� */
}CSVFDTimeCfg_S;

/**@brief VFD ����ģʽ����*/
typedef enum
{
	CS_VFD_GPIO,		/**< ��Ҫ���˵�� */
	CS_VFD_SCM			/**< ��Ҫ���˵�� */
}CSVFDMode;

typedef struct _CSVFDIRSpecialKey_S
{
	DWORD	 	m_dwIRProtocol;  /**< ң����Э��,ʹ��CSIRProtocol_tö������*/
	DWORD		m_dwUserID;		/**< �û���*/
	DWORD		m_dwScanKeyCode;	/**< ɨ����*/
	DWORD		m_dwLogicKeyCode;	/**< reserved*/
}CSVFDIRSpecialKey_S;

/**
@brief VFD��ʼ��������VFD��ʼ��ʱ��IO������Ϣ��PT6315��������Ϣ��

@param[in] stVFDInitparam VFD IO����Դ�����PT6315������Ϣ�����T_CS_VFDPanel_InitParam�ṹ����
@return CSHDI_SUCCESS��ʾVFD��ʼ���ɹ���CSHDI_FAILURE��ʾVFD��ʼ��ʧ��
*/
CSHDI_Error_t CSVFDInit(T_CS_VFDPanel_InitParam stVFDInitparam);

/**
@brief VFD�ַ���ʾ�����в���ϵͳ��ƽ̨���������ʾ�ڵ��øú������Զ����£��޲���ϵͳ��ƽ̨��Ҫ�ڵ��øú��������CSVFDScan����������VFD��ʾ��

@param[in] pucData Ҫ��VFD����ʾ���ַ�����������VFD����ʾ���ַ����ڣ�����VFDֻ��ʾ���ַ�����Χ�ڵ��ַ���������ʾ�˸��ַ���
@param[in] nLength Ҫ��ʾ���ַ��������Ϊ8����nLengthС��8���ַ��������ʾ��
@return CSHDI_SUCCESS��ʾ��ʾ�ɹ���CSHDI_FAILURE��ʾ��ʾʧ��
*/
CSHDI_Error_t CSVFDDisplayText(BYTE *pucData, int nLength);

/**
@brief ��ʾʱ��

@param[in] nhour ��ʾСʱ�����ֵΪ99Сʱ����nhourС��0������ʾСʱ��
@param[in] nminute ��ʾ���ӣ����ֵΪ99���ӡ���nminuteС��0������ʾ���ӡ�
@param[in] nSecond ��ʾ�룬���ֵΪ99�롣��nSecondС��0������ʾ�롣
@return CSHDI_SUCCESS��ʾ��ʾ�ɹ���CSHDI_FAILURE��ʾ��ʾʧ��
*/
CSHDI_Error_t CSVFDDisplayTime(int nhour, int nminute, int nSecond);

/**
@brief ������оƬ�뵥Ƭ��ͨ�ŵĴ�������

@param[in] nUartIndex ��оƬ�뵥Ƭ��ͨ�ŵĴ��ںţ�Ĭ��ֵΪ0���ú��������ڳ�ʼ��ǰ
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSVFDSetUartIndex (int nUartIndex);

/**
@brief ���û����д���ʱ�䣬�����õ�ʱ�䵽��������Զ�����

@param[in] psVFDTimeCfg ����ʱ�䣬Ŀǰֻ֧��Сʱ�����ӣ��롣������3Сʱ��20���ӣ�20�룬��3:20:20��ȥ�󣬻�����������
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ������ͳɹ������յ���Ƭ��Ӧ���Ժ󣬷��سɹ������򷵻�ʧ�ܡ�
*/
CSHDI_Error_t CSVFDSetStandbyTime (CSVFDTimeCfg_S *psVFDTimeCfg);

/**
@brief �����д��������øú����Ժ󣬵�Ƭ�������ж������Դ��

@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSVFDStandby(void);

/**
@brief��ȡ������ʽ����

@param[out] peBootType ������ʽ��ŵ�ַ
@return CSHDI_SUCCESS��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ�ܡ�
*/
CSHDI_Error_t CSVFDGetBootType(CS_VFD_BOOT_TYPE_E *peBootType);

/**
@brief ����VFD��LED�Ƶ���ʾ�����T_CS_VFD_LEDS���͡�

@param[in] enmLED  Ҫ��ʾ��LED��
@param[in] enmStatus LED�Ƶ�������������T_CS_VFD_LED_STATUS
@return CSHDI_SUCCESS��ʾ��ʾ�ɹ���CSHDI_FAILURE��ʾ��ʾʧ�ܡ�
*/
CSHDI_Error_t CSVFDDisplayLED(T_CS_VFD_LEDS enmLED, T_CS_VFD_LED_STATUS enmStatus);

/**
@brief ����VFD������ð�ŵ������������˸Ƶ�ʡ�

@param[in] enmPointType ��ʾð�����ͣ�ǰ��ð�Ż��ߺ���ð�š�
@param[in] enmColStatus ��ʾð����ʾ״̬�����������˸��
@param[in] nFlsTimeDiv ��ʾð����˸��Ƶ�ʣ�����1��ʾ30ms������һ�Ρ�
@return CSHDI_SUCCESS��ʾ��ʾ�ɹ���CSHDI_FAILURE��ʾ��ʾʧ�ܡ�
*/
CSHDI_Error_t CSVFDConfigDisplayTimeDiv(T_CS_VFDPanel_Two_Points enmPointType, 
                                                                               T_CS_VFD_COL_STATUS enmColStatus, int nFlsTimeDiv);

/**
@brief ע��VFD��尴���ص�������VFD������⵽�������»���ʱͨ��ע��Ļص�����֪ͨӦ�ò㡣

@param[in] pfunCallBack �ص��������û�ͨ���ص������õ�������Դ��״̬�ͼ�ֵ��
@return CSHDI_SUCCESS��ʾע��ɹ���CSHDI_FAILURE��ʾע��ʧ�ܡ�
*/
CSHDI_Error_t CSVFDPanelRegisterKeySendCallback(CSHDICallback pfunCallBack);

/**
@brief ����VFD��尴���Ƿ���ظ���

@param[in] ucSingleKeyValue Ҫ���õİ����ļ�ֵ
@param[in] bISRepeat FALSE�����������ظ���ֻ�ڰ������º͵���ʱ�����ص�����״̬�ǰ�����Ĭ��״̬��TRUE���������ظ����ڰ������²���ʱ���������������ص�
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSVFDSetSingleKeyRepeat( BYTE ucSingleKeyValue, BOOL bISRepeat );

/**
@brief ����VFD��ʼ��ʱ������������ȼ���ջ��С

@param[in] nPriority  VFD��ʼ��������������ȼ�
@param[in] nStackSize VFD��ʼ�����������ջ��С
@return None
*/
void CSVFDSetTaskParam(int nPriority, int nStackSize);

/**
@brief ����VFD����ʾ����

@param[in] eDimming VFD�����ȣ����T_CS_VFD_DIMMING����
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
CSHDI_Error_t CSVFDSetDimming(T_CS_VFD_DIMMING eDimming);

/**
@brief VFD���ɨ�裬�����޲���ϵͳƽ̨��VFD��ʾ�Ͱ���ɨ�衣�û���Ҫ�ڵ��øú���ǰ����Ҫ��ʾ�����ݣ����ڵ��øú���ʱ����ɨ�������ÿ��ɨ�����ʱʱ�䡣

@param[in] dwScanTime ���ɨ����������ڿ���VFD��ʾ�Ͱ���ɨ��ĳ���ʱ��
@param[in] dwDelayTime ���ڿ���ÿ��ɨ�����ʱʱ�䡣
@return CSHDI_SUCCESS��ʾɨ��ɹ���CSHDI_FAILURE��ʾɨ��ʧ�ܡ�
*/
CSHDI_Error_t CSVFDScan (DWORD dwScanTime,DWORD dwDelayTime);

/**
@brief��ȡ�汾��Ϣ��

@param[in] nSize pchVer�ĳ���
@param[out] pcVer ������Ű汾��Ϣ
@return CSHDI_SUCCESS��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
*/
int CSVFDGetVersion(PCHAR pcVer, int nSize);

/**
@brief ѡ��VFD����ģʽ

@param[in] eVfdMode VFD����ģʽ
@return CSHDI_SUCCESS��ʾѡ��ɹ���CSHDI_FAILURE��ʾѡ��ʧ�ܡ�
*/
CSHDI_Error_t CSVFDSeleteMode(CSVFDMode eVfdMode);


#ifdef __cplusplus
}
#endif

/** @} */

#endif






