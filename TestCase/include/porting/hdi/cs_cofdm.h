/**@defgroup CSCOFDM [CSCOFDM] DVB-T��׼�� ��������Ƶ�ָ���(coded orthogonal frequency division multiplexing)������
@brief ����cofdm��������Ƶ��ȡ���ź���Ϣ�ȡ�
 
@version 0.3 2008/11/5  ��һ��ϸ���汾
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/


#ifndef _CS_COFDM_H_
#define _CS_COFDM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cs_typedef.h"
#include "CSHDI_typedef.h"
#include "udi_tuner.h"

/**@brief cofdm �ӿڷ���ֵ����*/
typedef enum
{
	CSCOFDM_SUCCESS,						/**< �����ɹ�*/
	CSCOFDM_FAILURE,						/**< ����ʧ��*/
	CSCOFDM_HAS_INITIALIZED,		/**< ģ���Ѿ�����ʼ����һ��*/
	CSCOFDM_INVALID_PARAM, 			/**< ��������*/
	CSCOFDM_DEVICE_UNKNOWN,			/**< δ֪�豸��������Ч��CSHDITunerIndex*/
	CSCOFDM_I2C_ERROR,					/**< ����ʱI2C���ʳ���*/
	CSCOFDM_GPIO_ERROR,					/**< ����ʱgpio���ʳ���*/
	CSCOFDM_SYSTEM_ERROR,				/**< δ֪��ϵͳ����*/
	CSCOFDM_DROPPED,						/**< �źŶ�ʧ*/
	CSCOFDM_STOP,								/**< ����ֹͣ*/
	CSCOFDM_NOT_INITIALIZED			/**< ��Ӧ��CSHDITunerIndexδ��ʼ��*/
}CSCOFDM_Error_t; 

/**@brief cofdm �źŴ���ģʽö��*/
typedef enum _T_TransMode
{
	FFT_2K,		 			/**< FFT 2K����ģʽ*/
	FFT_8K,		  		/**< FFT 8K����ģʽ*/
	FFT_AUTO  			/**< FFT �Զ�����*/
}T_TransMode;

/**@brief cofdm �ź�Guardģʽö��*/
typedef enum _T_GuardMode
{
	GUARD_1_32,						/**< GUARD ģʽΪ1/32*/
	GUARD_1_16,						/**< GUARD ģʽΪ1/16*/
	GUARD_1_8,						/**< GUARD ģʽΪ1/8*/
	GUARD_1_4,						/**< GUARD ģʽΪ1/4*/
	GUARD_AUTO   					/**< GUARD �Զ�����*/
}T_GuardMode;

/**@brief cofdm �ź�I/Qģʽö��*/
typedef enum _T_IQMode
{
	IQ_NORMAL,   					/**< ģʽΪ I/Q*/
	IQ_CHANGED, 	 				/**< ģʽΪ I/-Q*/
	IQ_AUTO     					/**< I/Q�Զ�����*/
}T_IQMode;

/**@brief cofdm �ź�CodeRateö��*/
typedef enum _T_CodeRate
{
	CODERATE_1_2, 				/**< CodeRateΪ1/2*/
	CODERATE_2_3, 				/**< CodeRateΪ2/3*/
	CODERATE_3_4, 				/**< CodeRateΪ/3/4*/
	CODERATE_5_6, 				/**< CodeRateΪ5/6*/
	CODERATE_7_8, 				/**< CodeRateΪ7/8*/
	CODERATE_AUTO 				/**< CODERATE�Զ�����*/
}T_CodeRate;

/**@brief cofdm �ź�����ͼConstellationö��*/
typedef enum _T_Constellation
{
	QPSK_MODE,   					/**< ����ͼΪQPSKģʽ*/
	QAM16_MODE,  					/**< ����ͼΪQAM16ģʽ*/
	QAM64_MODE,  					/**< ����ͼQAM64ģʽ*/
	MODULATION_AUTO	 			/**< ����ͼ�Զ�����*/
}T_Constellation;

/**@brief cofdm �����ص��¼�����ö��*/
typedef enum							
{
  COFDM_CALLBACK_SIGNAL_CONNECTED,         	/**< �ź�����*/
  COFDM_CALLBACK_SIGNAL_DROPPED,          	/**< ��Ϊʹ�źŶ�ʧ*/
  COFDM_CALLBACK_SIGNAL_LOST,             	/**< �źŶ�ʧ*/
  COFDM_CALLBACK_SIGNAL_SEARCHING						/**< ��������*/	
}T_COFDMCallbackEvents;

/**@brief Heirarchyö��*/
typedef enum _T_Heirarchy
{
	ALPHA_NONE,									/**< ������*/	
	ALPHA_1,										/**< ������*/	
	ALPHA_2,										/**< ������*/	
	ALPHA_4,										/**< ������*/	
	ALPHA_AUTO									/**< ������*/	
}T_Heirarchy;

/**@brief ���ȼ�ö��*/
typedef enum _T_HPLP
{
	PRIORITY_HIGH,  	/**< ������*/
	PRIORITY_LOW,   	/**< ������*/
	PRIORITY_AUTO  		/**< �Զ�����*/
}T_HPLP;

/**@brief ��������ö��*/
typedef enum _T_SeachType
{
	PLAY_SIGNAL, 			/**< ����ģʽ����*/
	SCAN_SIGNAL  			/**< �Զ�ģʽ����*/
}T_SearchType;


/**@brief �����źŵ��Ʒ�ʽö��*/
typedef enum
{
	EM_DMB_TH_MODE_AUTO, 					/**< �Զ�ģʽ*/
	EM_DMB_TH_MODE_4QAM, 					/**< 4QAM*/
	EM_DMB_TH_MODE_4QAM_NR, 			/**< 4QAM_NR*/
	EM_DMB_TH_MODE_16QAM, 				/**< 16QAM*/
	EM_DMB_TH_MODE_32QAM, 				/**< 32QAM*/
	EM_DMB_TH_MODE_64QAM 					/**< 64QAM*/
}DMB_TH_MODE_E;

/**@brief DMB�ز���ʽö��*/
typedef enum
{
	EM_DMB_TH_SM_AUTO,    				/**<	�Զ�*/
	EM_DMB_TH_SM_SINGLE, 					/**<	���ز�*/
	EM_DMB_TH_SM_MULTI  					/**<	���ز�*/
}DMB_TH_SM_E;

/**@brief DMB PN��ʽö��*/
typedef enum
{
	EM_DMB_TH_PN_AUTO,    				/**<	�Զ�*/
	EM_DMB_TH_PN_420,    					/**<	420*/
	EM_DMB_TH_PN_595,    					/**<	595*/
	EM_DMB_TH_PN_945    					/**<	945*/
}DMB_TH_PN_E;

/**@brief DMB PNFLIP��ʽö��*/
typedef enum
{
	EM_DMB_TH_PNFLIP_AUTO,    			/**<  �Զ�*/
	EM_DMB_TH_PNFLIP_VPN,    				/**<	VPN */
	EM_DMB_TH_PNFLIP_CPN    				/**<	CPN */
}DMB_TH_PNFLIP_E;

/**@brief Ƶ����Ϣ�����ṹ��*/
typedef struct _DMB_TH_Para_S
{
	DWORD dwDMB_TH_Mode_3		:3;  			/**<  ������*/
	DWORD	dwDMB_TH_SM_2			:2;  			/**<  ������*/
	DWORD	dwDMB_TH_PN_2			:2;  			/**<  ������*/
	DWORD	dwDMB_TH_PNFLIP_2	:2;  			/**<  ������*/
	DWORD	dwReserved				:23;  		/**<  ������*/
}DMB_TH_Para_S;

/**@brief Ƶ��������չ�ṹ*/
typedef struct _COFDM_ParamExt_S
{
	T_BANDWIDTH bBandWidth ;  					/**<  ����*/
	DWORD dwFrequency ;  								/**<  Ƶ��(kHz)*/
	T_TransMode bTransMode ;  					/**<  ����ģʽ*/
	T_GuardMode bGuard ;  							/**<  Guardģʽ*/
	T_IQMode	bIQ;  										/**<  IQģʽ*/
	T_Constellation bConstellation ;  	/**<  ����ͼ*/
	T_CodeRate bHPCode ;  							/**<  ��λ����*/
	T_CodeRate bLPCode ;  							/**<  ��λ����*/
	T_Heirarchy bHeirarchy ;  					/**<  bHeirarchy����*/
	T_HPLP bHPLP;  											/**<  ���ȼ�*/ 
	DMB_TH_Para_S sDMBTHPara;  					/**<  �������*/
}COFDM_ParamExt_S;

/**@brief Ƶ�������ṹ*/
typedef struct _T_COFDM_Param
{
	T_BANDWIDTH bBandWidth ;  					/**<  ����*/
	DWORD dwFrequency ; 								/**<  Ƶ��*/
	T_TransMode bTransMode ;  					/**<  ����ģʽ*/
	T_GuardMode bGuard ;  							/**<  Guardģʽ*/
	T_IQMode	bIQ;  										/**<  IQģʽ*/
	T_Constellation bConstellation ; 		/**<  ����ͼ*/
	T_CodeRate bHPCode ;  							/**<  ��λ����*/
	T_CodeRate bLPCode ;  							/**<  ��λ����*/
	T_Heirarchy bHeirarchy ;  					/**<  T_Heirarchy����*/ 
	T_HPLP bHPLP;  											/**<  ���ȼ�*/ 
}T_COFDM_Param;

/**@brief �ص������ṹ*/
typedef struct _COFDMCallbackMessage
{
	T_COFDMCallbackEvents dwNIMEvent;							/**<  �ص��¼�*/
	DWORD dwFrequency;														/**<  Ƶ��*/
	DWORD dwBandWidth;														/**<  ����*/
	CSHDITunerIndex bTunerIndex;									/**<  tuner����*/
}COFDMCallbackMessage;


/**
@brief ��ʼ��COFDMǰ��

@param[in] enmTunerIndex Ҫ��ʼ��tuner������:Tuner_0��ʾ0��tuner��Tuner_1��ʾ1��tuner
@param[in] nPriority ����Ҫ�����ȼ���
@param[in] enmTSOutputMode ����������ӿ�ģʽ(����/����): TS_PARALLEL��ʾ����; TS_SERIAL��ʾ����
@param[in] nResetPinNum ���ӽ��оƬӲ��λ��GPIO������index.����ϣ��Ӳ��λ������Ϊ0xff
@return CSCOFDM_SUCCESS ��ʾǰ�˳�ʼ���ɹ���CSCOFDM_FAILURE��ʾ��ʼ��ʧ�ܡ�
@note �ú����������������������ã��Ҷ���ÿ��tunerֻ����ϵͳ��λʱ���ã��Σ��������ɲ���Ԥ֪�Ĵ���
*/
CSCOFDM_Error_t CSCOFDMInitialize(CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutputMode, int nResetPinNum);


/**
@brief ��ʼ��COFDMǰ��

@param[in] enmTunerIndex Ҫ��ʼ��tuner������:Tuner_0��ʾ0��tuner��Tuner_1��ʾ1��tuner
@param[in] eI2cIndex tuner���ڵ�I2C������
@param[in] nPriority ����Ҫ�����ȼ���
@param[in] enmTSOutputMode ����������ӿ�ģʽ(����/����): TS_PARALLEL��ʾ����; TS_SERIAL��ʾ����
@param[in] nResetPinNum ���ӽ��оƬӲ��λ��GPIO������index.����ϣ��Ӳ��λ������Ϊ0xff
@return CSCOFDM_SUCCESS ��ʾǰ�˳�ʼ���ɹ���CSCOFDM_FAILURE��ʾ��ʼ��ʧ�ܡ�
@note �ú����������������������ã��Ҷ���ÿ��tunerֻ����ϵͳ��λʱ���ã��Σ��������ɲ���Ԥ֪�Ĵ���
*/
CSCOFDM_Error_t CSCOFDMInitializeEx(CSHDITunerIndex enmTunerIndex, CSHDII2CIndex eI2cIndex, int nPriority, CSHDITsMode enmTSOutputMode, int nResetPinNum);

/**
@brief ע��ص���������������״̬֪ͨ

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[in] pfunCOFDMCallback ע��Ļص�����
@note ����ÿ��tuner�ú�����������Ÿ��Ե�CSCOFDMInitialize ���ã���������������֮ǰ���ã���������ȷ����֪ͨ
*/
void CSCOFDMRegisterCallback( CSHDITunerIndex enmTunerIndex, CSHDICallback pfunCOFDMCallback) ;

/**
@brief �����źŲ���,�����ź�,To send a request to lock a specified frequency point.

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[in] pstSignal �źŲ����ṹָ�루�μ��ṹ˵����T_COFDM_Param��˵����
@return CSCOFDM_SUCCESS��ʾ���ò����ɹ���CSCOFDM_FAILURE��ʾ���ò���ʧ��
*/
CSCOFDM_Error_t CSCOFDMConnectSignal(CSHDITunerIndex enmTunerIndex, T_COFDM_Param *pstSignal);

 /**
@brief ��ѯ�ź�������

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[in] pnSignalQuality �ź�����,��ΧΪ0��100,ֵԽ��,����Խ��
@return CSCOFDM_SUCCESS��ʾ��ѯ�����ɹ���CSCOFDM_FAILURE��ʾ��ѯ����ʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMQuerySignalQuality(CSHDITunerIndex enmTunerIndex,int *pnSignalQuality);

 /**
@brief ��ѯ�ź�ǿ��

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[out] pnSignalStrength �ź�ǿ�ȣ���ΧΪ0��100,ֵԽ���ź�Խǿ
@return CSCOFDM_SUCCESS��ʾ��ѯ�ź�ǿ�ȳɹ���CSCOFDM_FAILURE��ʾ��ѯ�ź�ǿ��ʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMQuerySignalStrength(CSHDITunerIndex enmTunerIndex, int *pnSignalStrength);
 
 /**
@brief ��ѯ�źŵ�ƽ

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[out] pnSignalRFLevel �źŵ�ƽ��С
@return CSCOFDM_SUCCESS��ʾ��ѯ�ɹ���CSCOFDM_FAILURE��ʾ��ѯʧ�ܡ�
*/
 CSCOFDM_Error_t CSCOFDMQuerySignalLeveldBm(CSHDITunerIndex enmTunerIndex, int *pnSignalRFLevel);	

 
 /**
@brief ��ѯ�ź������(C/N)

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[out] pnSignalCN �ź�����ȴ�С
@return CSCOFDM_SUCCESS��ʾ��ѯ�ɹ���CSCOFDM_FAILURE��ʾ��ѯʧ�ܡ�
*/
 CSCOFDM_Error_t CSCOFDMQuerySignalCNdB(CSHDITunerIndex enmTunerIndex, int *pnSignalCN);

/** 
@brief ��ѯ�ź�������

@param[in]  enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[out] pdwBER_1E7 1*10^-7����Ԫ�д�����Ԫ�ĸ�����(����10^-7�͵õ�������)
@return CSCOFDM_SUCCESS��ʾ��ѯ�����ʳɹ���CSCOFDM_FAILURE��ʾ��ѯ������ʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMQueryErrorRate(CSHDITunerIndex enmTunerIndex, DWORD *pdwBER_1E7);

/**
@brief ������ǰ�����źţ�ֱ�����µ���Ƶ����

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@return CSCOFDM_SUCCESS��ʾ���óɹ���CSCOFDM_FAILURE��ʾ����ʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMDropSignal(CSHDITunerIndex enmTunerIndex);

/**
@brief ��ѯ�ź�����״��

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[out] pnLockStatus ����״̬��TRUEΪ������FALSEΪδ������
@return CSCOFDM_SUCCESS��ʾ��ѯ�ɹ���CSCOFDM_FAILURE��ʾ��ѯʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMCheckNIMLock(CSHDITunerIndex enmTunerIndex, int *pnLockStatus);

/**
@brief ��ȡ��ȷ���źŲ���

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[out] pstTPSParam �źŲ����ṹ���μ��ṹ˵����T_COFDM_Param��˵����
@return CSCOFDM_SUCCESS��ʾ��ȡ�źŲ����ɹ���CSCOFDM_FAILURE��ʾ��ȡ�źŲ���ʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMGetTPSParam(CSHDITunerIndex enmTunerIndex, T_COFDM_Param *pstTPSParam);

/**
@brief ����LNB��Դ���ص�GPIO��

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[in] nGPIOIndex ����LNB��Դ���ص�GPIO�ڵ�����
@return CSCOFDM_SUCCESS��ʾ���óɹ���CSCOFDM_FAILURE��ʾ����ʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMConfigPinLNBPower (CSHDITunerIndex enmTunerIndex, int nGPIOIndex);

/**
@brief  ����LNB��Դ���ص�GPIO��

@param[in] enmTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[in] enmSwitchStatus CSHDI_OFF ��ʾ�ر�LNB��Դ;CSHDI_ON��ʾ��LNB��Դ
@return CSCOFDM_SUCCESS��ʾ����LNB��Դ�ɹ���CSCOFDM_FAILURE��ʾ����LNB��Դʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMSwitchLNBPower (CSHDITunerIndex enmTunerIndex, CSHDISwitch enmSwitchStatus);

 /**
@brief �����źŲ���,�����ź�,To send a request to lock a specified frequency point.

@param[in] eTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[in] psCOFDMSignalParam �źŲ����ṹָ�루�μ��ṹ˵����COFDM_ParamExt_S��˵����
@return CSCOFDM_SUCCESS��ʾ���ò����ɹ���CSCOFDM_FAILURE��ʾ���ò���ʧ��
*/
CSCOFDM_Error_t CSCOFDMConnectSignalExt(CSHDITunerIndex eTunerIndex, COFDM_ParamExt_S *psCOFDMSignalParam);

/**
@brief ��ȡ��ȷ���źŲ���

@param[in] eTunerIndex tuner������:Tuner_0��ʾ0��tuner; Tuner_1��ʾ1��tuner
@param[out] psCOFDMSignalParam �źŲ����ṹ���μ��ṹ˵����COFDM_ParamExt_S��˵����
@return CSCOFDM_SUCCESS��ʾ��ȡ�źŲ����ɹ���CSCOFDM_FAILURE��ʾ��ȡ�źŲ���ʧ�ܡ�
*/
CSCOFDM_Error_t CSCOFDMGetTPSParamExt(CSHDITunerIndex eTunerIndex, COFDM_ParamExt_S *psCOFDMSignalParam);

/*
@brief ��ȡ��ǰ������汾��

@param[in]nSize The size of Version Buffer. Must be less than 256.
@param[out]pchVer The top address of Version Buffer
@return 
- 1>If [pchVer] is NULL,return the actual length of version information,including '\0'.
- 2>If [pchVer] is not NULL,and the call is success,return the length of version information returned.
- 3>If the call is failure,return 0.   
*/
int CSCOFDMGetVersion(PCHAR pcVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif


