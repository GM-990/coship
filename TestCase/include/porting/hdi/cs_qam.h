/**@defgroup CSQAM [CSQAM]ͬ����Ŀר��QAM tuner �ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨�޹�QAM�����ӿ�
@brief QAM: Quadrature Amplitude Modulation(��������) ��������(QAM)��������Ϣ��Я������λ�к͵����ز������һ����ơ�����udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
 
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/

#ifndef _UDI_QAM_H_
#define _UDI_QAM_H_

#include "CSHDI_typedef.h"
#include "udi_tuner.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief QAM_�����ӿڷ���ֵ����*/
typedef enum
{
	CSQAM_SUCCESS,			/**< �ɹ�*/
	CSQAM_FAILURE,				/**< ʧ��*/
	CSQAM_HAS_INITIALIZED,	/**< �Ѿ���ʼ�� */
	CSQAM_INVALID_PARAM, 		/**< ��Ч����*/
	CSQAM_DEVICE_UNKNOWN,	/**< δ֪�豸,Ŀǰû����*/
	CSQAM_I2C_ERROR,			/**< I2C����,Ŀǰû����*/
	CSQAM_GPIO_ERROR,			/**< GPIO����,Ŀǰû���� */
	CSQAM_SYSTEM_ERROR,		/**< ϵͳ����,Ŀǰû���� */
	CSQAM_DROPPED,			/**< �źŶ�ʧ,Ŀǰû����*/
	CSQAM_STOP,				/**< ֹͣ��Ƶ,Ŀǰû���� */
	CSQAM_AGC_NOT_LOCK,		/**< AGC����ס */
	CSQAM_NOT_INITIALIZED,	/**< δ��ʼ�� */
	CSQAM_LEVEL_TOO_HIGH,	/**< �����ƽ̫��,*/
	CSQAM_LEVEL_TOO_LOW		/**< �����ƽ̫��*/
}CSQAM_Error_t; 


/**@brief Ƶ�������ṹ��*/
typedef struct 		
{
  DWORD		dwFrequencyKHz;				/**<	Ƶ�� ( unit: KHz )  Ƶ�ʷ�Χ: ( 48000 - 866000 )*/
  DWORD		dwSymbolRateKHz;			/**< ������ ( unit: Kbps )*/  
  T_QAMMode	bQAMMode;				/**< 0 Not Define  QAM���� ( 0 Not Define   1 - 16QAM	  2 - 32QAM	2 - 64QAM	  3- 128QAM	4 - 256QAM)*/
  int		nFrequencyOffsetKHz;		/**< Ƶƫ(unit:KHz),û���õ� */
  int		nSymbolRateOffset;			/**< ������ƫ��(unit:Kbps),û���õ� */
  BYTE		bIQ;					/**< 0��ʾƵ�ײ���ת,1��ʾƵ�׷�ת */
} T_QAMParameters;

/**@brief QAM_�����ӿڷ���ֵ����*/
typedef enum
{
	QAM_QUICKLOCK,		/**< û���õ�*/
	QAM_SEARCHING			/**< û���õ� */
}T_QAMSearchingMode;

/**@brief QAM_�����ӿڷ���ֵ����*/
typedef enum _DVBMCNSMode_t
{
	DVBMCNS_DVB,						/**< DVB ģʽ*/
	DVBMCNS_MCNS						/**< ANNEX_Bģʽ*/
}DVBMCNSMode_t;

/**@brief �ص����� ����ֵ*/
typedef enum								
{
	QAM_CALLBACK_SIGNAL_CONNECTED,        /**< 	�ź�����֪ͨ   */
	QAM_CALLBACK_SIGNAL_DROPPED,          /**<	������̨֪ͨ*/
	QAM_CALLBACK_SIGNAL_LOST,             /**< �źŶ�ʧ֪ͨ*/
	QAM_CALLBACK_SIGNAL_SEARCHING	      /**< �ź�δ����֪ͨ*/
}T_QAMCallbackEvents;
 
/**@brief QAM_�����ӿڷ���ֵ����*/
typedef struct 
{
	T_QAMCallbackEvents dwNIMEvent;		/**< �ص��������¼� */
	DWORD 	dwFrequency;				/**< �ص���Ƶ��(unit:KHz) */
	DWORD 	dwSymbolRate;				/**< �ص��ķ�����:(unit:Kbps) */	
	T_QAMMode dwQAMMode;				/**< 0 Not Define  QAM���� ( 0 Not Define   1 - 16QAM	  2 - 32QAM	2 - 64QAM	  3- 128QAM	4 - 256QAM)*/
	CSHDITunerIndex bTunerIndex;		/**< �ص���tuner����,��0��ʼ */
}T_QAMCallbackMessage;


/**@brief QAM_ͬƵ����ģʽ����*/
typedef enum
{
	EM_DIGITAL_INTERFERE_GB,   /**< ͬƵ����Ҫ��:С��27 */
	EM_DIGITAL_INTERFERE_8,    /**< ͬƵ����Ҫ��:С��8�����人 */
	EM_DIGITAL_INTERFERE_MAX
}CSQAMDigitalInterfere_E;

/**
@brief �������õ�ǰ�����źŵĴ����ڳ�ʼ��֮ǰ���ã���������ô˽ӿڣ�Ĭ��Ϊ8M����Ҫ��������Щ���������ĸ�Ƶͷ����TDA18252

@param[in] bTunerIndex   tuner����,�ýӿ���tuner����Ϊdemodulator���ڵ�I2C����
@param[in] bBandWidth  ��ǰ�����źŵĴ���6M����8M
@return    CSQAM_SUCCESS �������óɹ�  CSQAM_FAILURE��������ʧ��
*/
CSQAM_Error_t CSQAMSetBandWidth(CSHDITunerIndex bTunerIndex, T_BANDWIDTH eBandWidth); 

/**
@brief QAMģ���ʼ��

@param[in] bTunerIndex   tuner����,�ýӿ���tuner����Ϊdemodulator���ڵ�I2C����
@param[in] nPriority     �̵߳����ȼ�, 0---255
@param[in] TSOutMode     TS�������ʽ,TS_PARALLEL:������� TS_SERIAL:�������
@param[in] nResetPinNum  Ӳ�����ö�Ӧ��GPIO����,û������0xff
@return    CSQAM_SUCCESS ��ʼ���ɹ�  CSQAM_FAILURE��ʼ��ʧ��
*/
CSQAM_Error_t CSQAMInitialize( CSHDITunerIndex bTunerIndex, int nPriority, CSHDITsMode TSOutMode,  int nResetPinNum ); 

/**
@brief QAMģ���ʼ��,��CSQAMInitialize����չ����

@param[in] bTunerIndex tuner������,Tuner_0 0��tuner, Tuner_1 1��tuner,��Tuner_0��ʼ���
@param[in] eI2cIndex tuner��Ӧ��I2C��������
@param[in] nPriority �߳����ȼ�, 0--255
@param[in] TSOutMode TS�������ʽ,TS_PARALLEL:������� TS_SERIAL:�������
@param[in] nResetPinNum  Ӳ�����ö�Ӧ��GPIO����,û������0xff
@return    CSQAM_SUCCESS ��ʼ���ɹ�  CSQAM_FAILURE��ʼ��ʧ��
*/
CSQAM_Error_t CSQAMInitializeEx( CSHDITunerIndex bTunerIndex,CSHDII2CIndex eI2cIndex, int nPriority, CSHDITsMode TSOutMode,  int nResetPinNum ); 

/**
@brief ע��������ʾ�û���Ƶͷ����״̬�Ļص�����,�����ڳ�ʼ�������

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] function ע��Ļص�����,����Ϊ��
@return    ��
*/
void CSQAMRegisterCallback( CSHDITunerIndex bTunerIndex, CSHDICallback function );

/**
@brief ע��������ʾ�û���Ƶͷ����״̬�Ļص�����

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] function ����functionΪ��,���������ʾ,����Ӱ�캯������
@return    ��
*/
void CSQAMUnRegisterCallback( CSHDITunerIndex bTunerIndex, CSHDICallback function );

/**
@brief �����ָ����Ƶ�������Ƶ,����ǰ��������ȷ��Ƶ��,���øú�������Զ��Դ�Ƶ�������Ƶ,ʧ����Ҳ�����Զ�����ֱ����Ƶ�ɹ�

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] signalPtr Ƶ����Ϣ,���������T_QAMParameters����
@return    CSQAM_SUCCESS ������Ƶ����ɹ� CSQAM_FAILURE������Ƶ����ʧ�� 
@	   CSQAM_INVALID_PARAM ����������� CSQAM_NOT_INITIALIZEDδ��ʼ��
*/
CSQAM_Error_t CSQAMConnectSignal( CSHDITunerIndex bTunerIndex, T_QAMParameters *signalPtr );

/**
@brief �����ָ����Ƶ�������Ƶ,����ǰ��������ȷ��Ƶ��,���øú�������Զ��Դ�Ƶ�������Ƶ,ʧ����Ҳ�����Զ�����ֱ����Ƶ�ɹ�

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] signalPtr Ƶ����Ϣ,���������T_QAMParameters����
@param[in] bFlag �ݲ���
@return    CSQAM_SUCCESS ������Ƶ����ɹ� CSQAM_FAILURE������Ƶ����ʧ�� 
@	   CSQAM_INVALID_PARAM ����������� CSQAM_NOT_INITIALIZEDδ��ʼ��
*/
CSQAM_Error_t CSQAMConnectSignal_EXT( CSHDITunerIndex bTunerIndex, T_QAMParameters *signalPtr,T_QAMSearchingMode bFlag );

/**
@brief ��ȡ��ǰ��Ƶͷ����Ƶ״̬

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[out] pnLockStatus 1��ʾ��Ƶ 0��ʾʧ��
@return    CSQAM_SUCCESS ��ȡ��Ƶ״̬�ɹ� CSQAM_FAILURE��ȡ��Ƶ״̬ʧ�� 
@	   CSQAM_INVALID_PARAM ����������� CSQAM_NOT_INITIALIZEDδ��ʼ��
*/
CSQAM_Error_t CSQAMCheckNIMLock(CSHDITunerIndex bTunerIndex, int *pnLockStatus);

/**
@brief ��ȡָ��tuner���ź�����,��CSQAMConnectSignal�����

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[out] pnSignalQuality  ���ص���0--100������,���ڼ���Quality�Ĺ�ʽ��û��ͳһ�ı�׼��
@                            ���ǿ����ṩ�ο�ֵ����64QAM, С��18dBΪ0������35dBΪ100���м�Ϊ���ԣ�
@			     256QAM��С��26dBΪ0������38dBΪ100���м����ԡ����⣬������ס������£��ź�����Ϊ0��
@return    CSQAM_SUCCESS ��ȡ�ź������ɹ� CSQAM_FAILURE��ȡ�ź�����ʧ�� 
@	   CSQAM_INVALID_PARAM ����������� CSQAM_NOT_INITIALIZEDδ��ʼ��
*/
CSQAM_Error_t CSQAMQuerySignalQuality( CSHDITunerIndex bTunerIndex, int *pnSignalQuality );	

/**
@brief ��ȡָ��tuner���ź�ǿ��,��CSQAMConnectSignal�����

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[out] pnSignalStrength ���ص���0---100��ֵ������SignalStrengt�ļ���Ŀǰû��ͳһ�ı�׼, 
@			     ���ǿ����ṩ�ο�ֵ, ��64QAM��С��32dBuV������Ϊ�ź�ǿ��Ϊ0������120dBuV������Ϊ�ź�ǿ��Ϊ100���м�����;
@			     ����256QAM��С��36dB������Ϊ�ź�ǿ��Ϊ0������115dB������Ϊ�ź�ǿ��Ϊ100���м�����;
@			     ������ס������£���Ȼ�������ź�ǿ�ȡ�
@return    CSQAM_SUCCESS ��ȡ�ź�ǿ�ȳɹ� CSQAM_FAILURE��ȡ�ź�ǿ��ʧ�� 
@	   CSQAM_INVALID_PARAM ����������� CSQAM_NOT_INITIALIZEDδ��ʼ��
*/
CSQAM_Error_t CSQAMQuerySignalStrength( CSHDITunerIndex bTunerIndex, int *pnSignalStrength );	

/**
@brief ��ȡָ��tuner��������, ��CSQAMConnectSignal�����

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[out] pdwErrorRate ���ص���R-S�����Ĵ���ı�����(Bit error count), ����סʱΪ10000000��
@return    CSQAM_SUCCESS ��ȡ�����ʳɹ� CSQAM_FAILURE��ȡ������ʧ�� 
@	   CSQAM_INVALID_PARAM ����������� CSQAM_NOT_INITIALIZEDδ��ʼ��
*/
CSQAM_Error_t CSQAMQueryErrorRate(CSHDITunerIndex bTunerIndex, DWORD *pdwErrorRate);		

/**
@brief ��ȡָ��tuner���źŵ�ƽǿ��, ��CSQAMConnectSignal�����

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[out] pnSignalLevel ���ص��������źŵĵ�ƽǿ��,��Χ30-114dBuV(unit:dBuV)
@return    CSQAM_SUCCESS ��ȡ�źŵ�ƽ�ɹ� CSQAM_FAILURE��ȡ�źŵ�ƽʧ�� 
@	   CSQAM_INVALID_PARAM ����������� CSQAM_NOT_INITIALIZEDδ��ʼ��
*/
CSQAM_Error_t CSQAMQuerySignalLeveldBuV(CSHDITunerIndex bTunerIndex, int *pnSignalLevel);

/**
@brief ��ȡָ��tuner�������(C/Nֵ), ��CSQAMConnectSignal�����

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] pnSignalCN  ���ص�ֵ���źŵ������(unit:dB),��36.1dB���ص���36dB
@return    CSQAM_SUCCESS ��ȡ�ź�����ȳɹ� CSQAM_FAILURE��ȡ�ź������ʧ�� 
@	   CSQAM_INVALID_PARAM ����������� CSQAM_NOT_INITIALIZEDδ��ʼ��
*/
CSQAM_Error_t CSQAMQuerySignalCNdB(CSHDITunerIndex bTunerIndex, int *pnSignalCN);

/**
@brief ��ȡָ��tuner��ƫ�Ʋ���

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] ptrQamChannel  �������T_QAMParameters����,�����в�û�б���Ƶƫ�ͷ�����ƫ��ֵ,����nFrequencyOffsetKHz��nSymbolRateOffset��ֵΪ0
@return    CSQAM_SUCCESS ��ȡƫ�Ʋ����ɹ� CSQAM_FAILURE��ȡƫ�Ʋ���ʧ�� 
@	   CSQAM_INVALID_PARAM ����������� 
*/
CSQAM_Error_t CSQAMGetOffsetPAR(CSHDITunerIndex bTunerIndex, T_QAMParameters *ptrQamChannel);

/**
@brief �����ӿڣ�����QAM��ʼ��ǰ���ã�����ѡ��ͬ�ķ�������,�����ô˺���Ĭ�ϵķ�������ΪDVBMCNS_DVBģʽ

@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] bModeDvbMcns  �������DVBMCNSMode_t˵�� 
@return    CSQAM_SUCCESS ���÷������ͳɹ�, CSQAM_FAILURE ���÷�������ʧ��
@	   CSQAM_INVALID_PARAM ����������� CSQAM_HAS_INITIALIZED�Ѿ���ʼ��
*/
CSQAM_Error_t CSQAMSetDVBMCNSMode( CSHDITunerIndex bTunerIndex, DVBMCNSMode_t bModeDvbMcns );

/**
@brief ��ȡģ��汾��Ϣ����ʾ��ʽΪ:
  [ģ����]-[ģ������]-[�汾��],��CS_QAM-T-01.00.00, CS_QAM��ʾģ����,T��ʾtest�汾,01.00.00Ϊ�汾��

@param[out] pchVer ��Ű汾��Ϣ�Ļ������׵�ַ
@param[in] nSize  ����������
@return 
   1> �����һ������pchVerΪ�գ����ذ汾��Ϣ��ʵ�ʳ���
   2> �����һ��������Ϊ�գ��Һ������óɹ����򷵻�ʵ
        �ʻ�ȡ�İ汾��Ϣ����
   3> ���ʧ�ܣ�����0

*/
int CSQAMGetVersion( PCHAR pchVer, int nSize );


/**
@brief ����ע�ử�㺯������������Ļ�ϻ�������ͼ,����CSQAMShowConstellation��ʾ

@note  ���㺯���ĺ���ԭ��Ϊ: void SetPixel(int x, int y, DWORD color);
 	���У�x, yΪ��Ļ���꣬color��͸���Ⱥ�RGB��ɫ�Ĺ�ϵΪ:
 	color=(alpha<<24)|(B<<16)|(G<<8)|R. ��720x576��Ϊ�ο�
@return ��
*/
void CSQAMRegisterFuncSetPixel( void (*function)(int, int, DWORD)  );

/**
@brief ����ע���ı���ʾ��������������Ļ����ʾ������,�ź���������Ϣ������CSQAMShowConstellation��ʾ
@note �ı���ʾ�����ĺ���ԭ��Ϊ: void DrawText(char * strMsg, int xStart, int yStart,  DWORD color);
 	���У�xStart, yStartΪ��ʾ��ʼ���꣬color��͸���Ⱥ�RGB��ɫ�Ĺ�ϵΪ:
 	color=(alpha<<24)|(B<<16)|(G<<8)|R.
@return ��
*/

void CSQAMRegisterFuncDrawText( void (*function)(char *, int, int,  DWORD)  )	;

/**
@brief ��������Ļ�ϻ�������ͼ��Ŀǰֻ֧��64QAM������
	ʹ��CSQAMRegisterFuncSetPixel����ע��Ļ��㺯����
	���ƹ���Ϊ�����������ú����Ƚ���ĻOSD�������
	Ȼ���ȡ�ź�IQֵ������Ļ�ϴ�㡣�����Ҫ�ظ�����
	�ýӿ��ѻ�ȡʵʱ������ͼ��
	��ʾ�����ݰ���:�ź�����(0-100);CNֵ(unit:dB);�ź�ǿ��(0-100);���������
	
@param[in] bTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@return ��
*/
void CSQAMShowConstellation( CSHDITunerIndex bTunerIndex );

/**
@brief �ýӿ�������Ƶʱ��Ƶƫ�ͷ�����ƫ�Ʋ�����ƫ�Ʋ���Ϊ:
		����������Ƶ�ʺ����ķ����ʽ�����Ƶ�������Ƶʧ�ܣ�
		�����趨�Ĳ�����ƫ�ƴ������������ҽ���ƫ�ơ�
		ע�����õ�ƫ�ƴ����������ܵ�ƫ�ƴ�����ƫ��˳����������ҡ�
		�ú����������κ�ʱ����á�
		����û������øú�����QAM�����Ĭ�ϵĲ���������Ƶ���ơ�
		�����Ǹ���Ƶͷ��Ĭ�Ͽ��Ʋ���:
		STV0297/STV0297J:
			nFreqSearchTime: 1
			nFreqStepKHz: 500
			nSymSearchTime: 0
			nSRateStepKSps: 2
		TDA10021/TDA10023:
			nFreqSearchTime: 0
			nFreqStepKHz: 500
			nSymSearchTime: 0
			nSRateStepKSps: 2

@param[in] nFreqSearchTime ������Ƶʱ����Ƶƫ�Ĵ���
@param[in] nFreqStepKHz ������Ƶ��Ƶƫ��������KHzΪ��λ
@param[in] nSymSearchTime ������Ƶʱ���ҷ�����ƫ�ƵĴ���
@param[in] nSRateStepKSps ������Ƶʱ������ƫ�ƵĲ�������KSpsΪ��λ
@return    CSQAM_SUCCESS ����Ƶƫ�����ɹ�, CSQAM_FAILURE ����Ƶƫ����ʧ��
@	   CSQAM_INVALID_PARAM ����������� 
*/
CSQAM_Error_t CSQAMSetSearchingPar(int nFreqSearchTime, int nFreqStepKHz, int nSymSearchTime, int nSRateStepKSps);

/**
@brief ���ڼ��� MT2060�ĵ�һ��Ƶ 

@param[in] eTunerIndex   tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] pQAMParameters  ����Ƶ����Ϣ,���������T_QAMParameters˵��
@param[in] dwIFout  tuner����Ƶֵ, Output IF(unit:KHz)(Range 0~60000)
@param[out] dwFIF1  ���صĵ�һ��Ƶ��ֵ, MT2060 First IF(unit:Hz)(Range 1160,000,000~1270,000,000)
@param[out] dwPMax  ���ص������ƽֵ(unit:*100dB), Input Power Level	(less than 0x1400->Input Level is low)(larger than 0x1900->Input Level is high)
@return 
-CSQAM_SUCCESS - ������Ƶֵ�ɹ�   
-CSQAM_FAILURE	 - ������Ƶֵʧ��
-CSQAM_INVALID_PARAM	- ��������

*/
CSQAM_Error_t CSQAMLocateIF(CSHDITunerIndex eTunerIndex, T_QAMParameters* pQAMParameters, DWORD dwIFout, DWORD* dwFIF1, DWORD* dwPMax);

/**
@brief This function Get  the value of MT2060's First IF from App
@	�ú���������Silicon tuner MT2060�Ĵ��ݵ�һ��Ƶ��
@	��֮ǰ�ѳɹ�У�����һ��Ƶ֮�󣬲������ݴ���FLASH�У��˺󿪻�ʱ������֤������ֻ����ô˺������Ѿ��洢�����ݴ���MT2060


@param[in] eTunerIndex tuner���� Tuner_0 0��tuner, Tuner_1 1��tuner
@param[in] dwFreqIF1 �ɹ�У�����ROM�еĵ�һ��Ƶֵ MT2060 First IF from App read in flash(Hz)(Range 1160,000,000~1270,000,000)
@return  
-CSQAM_SUCCESS	- ��ȡ�ɹ�
-CSQAM_FAILURE	 - 	��ȡʧ��
-CSQAM_INVALID_PARAM	- �������� 

*/
CSQAM_Error_t CSQAMGetIF(CSHDITunerIndex eTunerIndex, DWORD dwFreqIF1);



/**
@brief 
@	�ú�������ͬƵ����ָ�꣬Ĭ��Ϊ����ģʽ
@	�ú����ڳ�ʼ��֮ǰ���ã������ظ�����
@param[in] eMode ͬƵ����ģʽ
@return  
-CSQAM_SUCCESS	- ���óɹ�
-CSQAM_FAILURE	 - 	����ʧ��
-CSQAM_INVALID_PARAM	- �������� 

*/
CSQAM_Error_t CSQAMSetDigitalInterfereMode(CSQAMDigitalInterfere_E eMode);

#ifdef __cplusplus
}
#endif
/** @} */
#endif
