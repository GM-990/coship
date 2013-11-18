/**@defgroup TUNER TUNER ������ͳһ��tuner�ӿڲ�������
@brief Tunerģ���Ӧͷ�ļ�<udi2_tuner.h>��������Tuner�ĳ��ò����ӿڡ�֧��DVB-C��
׼(QAM���Ʒ�ʽ),DVB-S/DVB-S2��׼(QPSK,8PSK���Ʒ�ʽ),DVB-T��׼(COFDM��DMB-TH���Ʒ�ʽ)��

Tuner��Ϊ�̶�Tuner�Ϳ��ƶ�Tuner���֣�ͨ��GetAllDeviceId�ɻ��ÿ��Tuner��Id�����й�
��Tuner��ID��0��ʼ�������ۼӣ����ƶ�Tuner��ID�����RDIģ��������

���ƶ�Tuner�ڱ����ֺ�ΪInit״̬���̶�Tuner�ڳ�ʼ�����ΪInit״̬��
Tuner����һЩ�еĽӿڵ��û����״̬�ı仯������Щ�仯�Ĺ����У��ص���ϢӦ����ͼ��
ʾ��������������У�\n
- Init ָtuner��ʼ�����֮��û�н�����Ƶ֮ǰ��״̬��
- Locked ָtuner������Ƶ������������Ƶ�ɹ�֮���״̬��
- Unlocked ָtunerû����Ƶ�ɹ���״̬��
- Init -> Locked ��ָtuner���е�һ����Ƶ��������Ƶ�ɹ�֮����һ��CONNECTED��Ϣ
- Init -> Unlocked ��ָtuner���е�һ����Ƶ��������Ƶʧ��֮�󣬲��ϳ��Խ�����Ƶ
�������ڼ�᲻�Ϸ���SEARCHING��Ϣֱ����Ƶ�ɹ���searching��Ϣ���͵�Ƶ����ÿ1sһ�Ρ�
- Unlocked -> Locked ��ָtuner����Ƶʧ��״̬֮���ٴ���Ƶ�ɹ����ɹ�֮����CONNECTED
��Ϣ��ͬʱֹͣ����searching��Ϣ
- Locked->Unlocked ��ָtuner��Ƶʧ�ܺ󣬷���һ��LOST��Ϣ�������Ͻ�����Ƶ�������ڼ�
�ᷢ��SEARCHING��Ϣֱ����Ƶ�ɹ���\n

@section Tuner״̬�ı�����ص�ͼ
@image HTML udi_tuner_event.jpg

�����ݵĴ���Ƕȱ�ģ���ֽ�Tuner��Ϊ���ࣺ
- һ��ΪDMA���͵�Tuner�����̶�Tuner��ͨ����Tuner��Ƶ���TS���ݻ�ֱ��DMA��������
�ӵ�Demux���������豸�ϡ�
- һ��ΪSOFT���͵�Tuner�������ƶ�Tuner��ͨ����Tuner��Ƶ���TS������Ҫͨ��IORead
�ӿڶ���Buff�ϣ�����Eastwin���������������ע�뵽Demux�н��в��ţ����ߴ洢��
Ӳ�̵ȡ�

���ָ�Ƶͷ��֧��PID���˹��ܣ����ָ�Ƶͷ������һЩ���ƶ��豸��DVB-TH��CMMB
�ȡ�����Ƶͷ��m_bPIDFilterSupport����ΪCSUDI_TRUEʱ���ù�����Ч���������������
Ƶ��󣬸�Ƶͷ���Ὣ�κ����ݴ��͸����������ģ�飬����������ЩPid������ʱ϶��
�����ݻ�ͨ���󣬷�������ͨ������ʵ���൱�ڸ���Tunerһ���򵥵�Demux���ܡ����
ϵͳ�в�����������Tuner���ɲ�ʵ�ִ˽ӿڣ�ֱ�ӷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
���ɡ�

@version 2.2.0 2011/12/31 �޸�CSUDITunerTerrestrialDeliver_S�ṹ�壬����T2��ؽӿ�
@version 2.1.2 2009/09/15 �޸�Tuner�¼��ص��������ͣ�����һЩ����淶����
@version 2.1.1 2009/09/11 �޸Ļ�ȡTuner�ź�����ӿڣ��ýӿ����ڿ�ѡ��Ļ�ȡ������Ϣ������һ����ȫ����ȡ
@version 2.1.0 2009/09/08 ���ӿ��ƶ�Tuner�豸��ض���
@version 2.0.2 2009/08/28 ����ע��ص�����ʱָ��tuner�豸�źŵĲ���
@version 2.0.1 2009/08/25 ��һ���ۺ������޸ĺ�İ汾
@{
*/

#ifndef _UDI2_TUNER_H_
#define _UDI2_TUNER_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"
#ifdef __cplusplus
extern "C" 
{
#endif
	
/**@brief tunerģ����ش������ֵ*/
enum
{
	CSUDITUNER_ERROR_BAD_PARAMETER = CSUDI_TUNER_ERROR_BASE,	///< ��������
	CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED,						///< ��֧�ִ˲���
	CSUDITUNER_ERROR_UNKNOWN,									///< һ�����
	CSUDITUNER_ERROR_NO_MEMORY,									///< ���ڴ���ô���
	CSUDITUNER_ERROR_INVALID_DEVICE_ID,							///< �Ƿ��豸ID
	CSUDITUNER_ERROR_I2C_ERROR,								    ///< I2C���ߴ���
	CSUDITUNER_ERROR_NO_CALLBACK,                               ///< û����Ӧ�Ļص���������ɾ��
	CSUDITUNER_ERROR_NO_DATA,									///< û�����ݿ��Զ�ȡ
	CSUDITUNER_ERROR_ALREADY_ADDED								///< �ûص��Ѿ���ӹ�
};

/**@brief QAM ��������ö�ٶ���*/
typedef enum
{
	EM_UDITUNER_NOT_DEFINED,	///< δ�����QAM������һ������±�����Ч����
	EM_UDITUNER_QAM16,			///< 16QAM����
	EM_UDITUNER_QAM32,			///< 32QAM����
	EM_UDITUNER_QAM64,			///< 64QAM����
	EM_UDITUNER_QAM128,			///< 128QAM����
	EM_UDITUNER_QAM256			///< 256QAM���� 
}CSUDITunerQAMMode_E;

/**@brief �������ֵ���(DVB-C)tuner���źŲ������ݽṹ*/
typedef struct 
{
	unsigned int m_uFrequency;					///< Ƶ�ʣ���λKHz
	unsigned int m_uSymbolRate_24			:24;    	///< �����ʣ���λKbps
	unsigned int m_uModulation_8			:8;	 ///< CSUDITunerQAMMode_E����ö��
}CSUDITunerCableDeliver_S;

/**@brief tunerģ�鿪������ö�ٶ���*/
typedef enum
{
	EM_UDITUNER_SWITCH_OFF,	///< ���عر�
	EM_UDITUNER_SWITCH_ON	///< ���ش�
}CSUDITunerSwitchStatus_E;


/**@brief cofdm �źŴ�������ö�ٶ���*/
typedef enum 							
{
	EM_UDITUNER_BAND_8M,    ///<����8M 
	EM_UDITUNER_BAND_7M,    ///<����7M
	EM_UDITUNER_BAND_6M     ///<����6M
}CSUDITunerBanuidth_E;

/**@brief �������ֵ���(DVB-T)tuner���źŲ������ݽṹ*/
typedef struct 
{
	unsigned int m_uFrequency;					          ///< Ƶ�ʣ���λKHz
	unsigned int m_uBanuidth_3				:3;	 	///< CSUDITunerBanuidth_E����ö��
	unsigned int m_uFlag_2					:2;		///DVB-T/DVB-T2��־:00-T;01-T2;10,11����
	unsigned int m_uPlpID_8					:8;     ///T2�洢PLPID
	unsigned int m_uReserved_19				:19;  	///< ����ʹ�� 
}CSUDITunerTerrestrialDeliver_S;

/**@brief QPSK����Diseqc1.0�˿�����ö�ٶ���*/
typedef enum  
{
	EM_UDITUNER_DISEQC10_OFF,	///< diseqc1.0ͨ��ѡ��ر�
	EM_UDITUNER_PORT_4_A,		///< diseqc1.0��ѡһ���ص�Aͨ��
	EM_UDITUNER_PORT_4_B,		///< diseqc1.0��ѡһ���ص�Bͨ��
	EM_UDITUNER_PORT_4_C,		///< diseqc1.0��ѡһ���ص�Cͨ��
	EM_UDITUNER_PORT_4_D,		///< diseqc1.0��ѡһ���ص�Dͨ��
	EM_UDITUNER_PORT_2_A,		///< diseqc1.0��ѡһ���ص�Aͨ��
	EM_UDITUNER_PORT_2_B		///< diseqc1.0��ѡһ���ص�Bͨ��
}CSUDITunerSwitchDisqec10Port_E;  

/**@brief QPSK�����źż�������ö�ٶ���*/
typedef enum 
{
	EM_UDITUNER_POLARISATION_H,		///< �߼�������-ˮƽ����
	EM_UDITUNER_POLARISATION_V,		///< �߼�������-��ֱ����
	EM_UDITUNER_POLARISATION_L,     	///< Բ��������-��������
	EM_UDITUNER_POLARISATION_R      	///< Բ��������-��������
}CSUDITunerChannelPolar_E;

/**@brief QPSK����Tone burst ����*/
typedef enum 
{
	EM_UDITUNER_TONEBURST_NONE,		///< Tone burstѡ��ر�
	EM_UDITUNER_TONEBURST_A,		///< Tone burstѡ��Aͨ��
	EM_UDITUNER_TONEBURST_B			///< Tone burstѡ��Bͨ��
}CSUDITunerSwitchToneBurst_E;

/**@brief QPSK����Diseqc�����ظ���������*/
typedef enum 
{
	EM_UDITUNER_NOREPEAT,		///< û��DiSEqC Repeat 
	EM_UDITUNER_ONCEREPEAT,		///< ��Ҫһ��DiSEqC Repeat
	EM_UDITUNER_TWICEREPEAT		///< ��Ҫ����DiSEqC Repeat
}CSUDITunerRepeatTime_E;

/**@brief QPSK����Diseqc1.2������ת��������*/
typedef enum 
{
	EM_UDITUNER_EASTERN,		///< ��ת��
	EM_UDITUNER_WESTERN		///< ����ת��
}CSUDITunerRotateDirection_E;
	
/**@brief QPSK����LNB���Ͷ���*/
typedef enum 
{
	EM_UDITUNER_SINGLE,				///< ������LNB 
	EM_UDITUNER_UNIVERSAL,				///< ˫����ͨ��LNB:  �߱���22K�򿪣��ͱ���22K�ر�
	EM_UDITUNER_UNIVERSAL_NEGLOGIC			///< ˫����LNB: �߱���22K�رգ��ͱ���22K��
}CSUDITunerLNBType_E;

/**@brief QPSK����12����ѹѡ���LNB��ѹ����*/
typedef enum 
{
	EM_UDITUNER_SWITCH_0V,		///< 0���ص�ѹѡ�� 
	EM_UDITUNER_SWITCH_12V		///< 12���ص�ѹѡ��
}CSUDITunerSwitch012V_E;

/**@brief QPSK�����е�FEC�����Ͷ���*/
typedef enum
{
	EM_UDITUNER_FEC_AUTO,			///< FEC��ʽ�Զ�ѡ�� 
	EM_UDITUNER_FEC1_2,			///< 1/2��ʽ��FEC
	EM_UDITUNER_FEC2_3,			///< 2/3��ʽ��FEC
	EM_UDITUNER_FEC3_4,			///< 3/4��ʽ��FEC
	EM_UDITUNER_FEC5_6,			///< 5/6��ʽ��FEC 
	EM_UDITUNER_FEC7_8,			///< 7/8��ʽ��FEC
	EM_UDITUNER_FEC8_9,			///< 8/9��ʽ��FEC
	EM_UDITUNER_FEC3_5,			///< 3/5��ʽ��FEC 
	EM_UDITUNER_FEC4_5,			///< 4/5��ʽ��FEC
	EM_UDITUNER_FEC9_10,		     	///< 9/10��ʽ��FEC
	EM_UDITUNER_FEC6_7			///< 6/7��ʽ��FEC 
}CSUDITunerQPSKFECRates_E;

/**@brief DVB-S/S2�г��õĵ��Ʒ�ʽ����*/
typedef enum
{
	EM_UDITUNER_AUTO_PSK,		///< QPSK��8PSK�Զ�ʶ��ʽ
	EM_UDITUNER_QPSK,	        ///< QPSK���Ʒ�ʽ
	EM_UDITUNER_8PSK		///< 8PSK���Ʒ�ʽ
}CSUDITunerQPSKModulation_E;

/**@brief �������ֵ��ӵı�׼����*/
typedef enum
{
	EM_UDITUNER_DVBS_STANDARD,		///< DVB-S��׼�ź� 
	EM_UDITUNER_DVBS2_STANDARD,		///< DVB-S2��׼�ź�
	EM_UDITUNER_DVBS_AUTO_STANDARD  	///< DVB-S��DVB-S2�Զ�ʶ��
}CSUDITunerQPSKStandard_E;


/**@brief QPSK����Unicable�е��������ѡ��(һ��Unicable LNB ͬʱ���֧��2������, ����A������B)*/
typedef enum
{
	EM_UDITUNER_OPTION_NONE,		///< û�п�ѡ���� 
	EM_UDITUNER_OPTION_A,			///< Unicableѡ������A
	EM_UDITUNER_OPTION_B			///< Unicableѡ������B
}CSUDITunerQPSKOption_E;

/**@brief QPSK����Unicable�е�������������ѡ��һ��Unicable LNB ���֧��8������*/
typedef enum 
{
	EM_UDITUNER_UNICABLE_NONE,			///< Unicable LNB δ����
	EM_UDITUNER_UNICABLE_A_LB_VP,			///< Unicable LNB ѡ��:satellite A, Low Band, Vertical Polarization
	EM_UDITUNER_UNICABLE_A_HB_VP,			///< Unicable LNB ѡ��:satellite A, High Band, Vertical Polarization
	EM_UDITUNER_UNICABLE_A_LB_HP,			///< Unicable LNB ѡ��:satellite A, Low Band, Horizontal Polarization
	EM_UDITUNER_UNICABLE_A_HB_HP,			///< Unicable LNB ѡ��:satellite A, High  Band, Horizontal Polarization
	EM_UDITUNER_UNICABLE_B_LB_VP,			///< Unicable LNB ѡ��:satellite B, Low Band,  Vertical Polarization
	EM_UDITUNER_UNICABLE_B_HB_VP,			///< Unicable LNB ѡ��:satellite B, High  Band, Vertical Polarization
	EM_UDITUNER_UNICABLE_B_LB_HP,			///< Unicable LNB ѡ��:satellite B, Low Band, Horizontal Polarization
	EM_UDITUNER_UNICABLE_B_HB_HP			///< Unicable LNB ѡ��:satellite B, High  Band, Horizontal Polarization
}CSUDITunerQPSKUnicableLNBType_E;

/**@brief QPSK�����е�LNB��ѹ״̬����*/
typedef enum
{
	EM_UDITUNER_LNB_OFF,		///< LNB��ѹ�ر�
	EM_UDITUNER_LNB_AUTO,		///< LNB��ѹѡ���Զ� 
	EM_UDITUNER_LNB_14V,		///< LNB��ѹѡ��14����
	EM_UDITUNER_LNB_18V		///< LNB��ѹѡ��18����
}CSUDITunerQPSKLNBStatus_E;

/**@brief QPSK����������DVB-S2�еĵ�Ƶ���ض���*/
typedef enum
{
	EM_UDITUNER_PILOT_OFF,		///< ��Ƶ�� 
    	EM_UDITUNER_PILOT_ON		///< ��Ƶ�ر� 
}CSUDITunerQPSKPilot_E;

/**@brief ���ǵ���tuner���ź����ݽṹ(ͬʱ����DVB-S/S2)*/
typedef struct 
{
	unsigned int	        m_uFrequency;					///< Ƶ�ʣ���λKHz
	unsigned int	m_uSymbolRate_24			:24;	///< �����ʣ���λKbps
	unsigned int	m_uFECInner_4				:4;	///< CSUDITunerQPSKFECRates_E ����ö��
	unsigned int	m_uPolar_4				:4;	///< CSUDITunerChannelPolar_E ����ö��
	unsigned int	        m_uPilot;                          		///< CSUDITunerQPSKPilot_E ����ö��,DVB_S2�ź�ר��
	unsigned int	m_uModulations_2             :2;			///< CSUDITunerQPSKModulation_E ����ö��
	unsigned int    m_uStandards_2               :2;			///< CSUDITunerQPSKStandard_E ����ö�� 
	unsigned int	m_uReserved_28               :28;		///< ����ʹ��
}CSUDITunerSatelliteDeliver_S;

/**@brief �������ֵ���(DVB-S/S2)tuner��DiSEqC1.0��׼�Ĳ���*/
typedef struct 
{
	unsigned int m_uCommitted					:4;		///< CSUDITunerSwitchDisqec10Port_E ����ö��
	unsigned int m_uToneburst					:2;		///< CSUDITunerSwitchToneBurst_E ����ö��
	unsigned int m_uRepeatedTimes				:2;		///< �ظ�����CSUDITunerRepeatTime_E ����ö��
	unsigned int m_uReserved					       :24;		///< ����ʹ��
}CSUDITunerDiSEqC1020_S;

/**@brief �������ֵ���(DVB-S/S2)tuner��DiSEqC1.1��׼�Ĳ���*/
typedef struct 
{
	unsigned int m_uCommitted					:4;	///< DiSEqC1.0�˿�,��CSUDITunerSwitchDisqec10Port_E ����ö��
	unsigned int m_uUncommitted					:4;	///< DiSEqC1.1�˿�, 1----16
	unsigned int m_uToneburst					:2;     ///< CSUDITunerSwitchToneBurst_E ����ö��
	unsigned int m_uRepeatedTimes					:2;	///< �ظ�����CSUDITunerRepeatTime_E ����ö��
	unsigned int m_uReserved					:20;	///< ����ʹ��
}CSUDITunerDiSEqC1121_S;

/**@brief �������ֵ���(DVB-S/S2)tuner��DiSEqC1.2��׼�Ĳ���*/
typedef struct 
{
	unsigned int m_uUsals						:2;		///< ��תλ�ã�λ��ֵȡ��m_uIndex 2 -�����û����õľ�γ�Ƚ�����Ӧת������
	unsigned int m_uIndex						:5;		///< ��ת����λ��ֵ��
	unsigned int m_uReserved					:25;		///< ����ʹ��
}CSUDITunerDiSEqC1222_S;


/**@brief �������ֵ���(DVB-S/S2)tuner������λ�ò����ṹ(ͬʱ����DVB-S/S2)*/
typedef struct 
{
	double m_dLocalLongitude;					///< ���صľ���λ��(����ֵ*10,��DTV�ж���һ��)
	double m_dLatitude;						///< ���ص�γ��λ��(γ��ֵ*10,��DTV�ж���һ��)
	double m_dSatLongitude;						///< �������ڵľ���λ��(����ֵ*10,��DTV�ж���һ��)
	unsigned int m_uWestEastFlag			:1;		///< �������ڵ�λ�õĶ�������־��0 -EM_UDITUNER_EASTERN, 1 - EM_UDITUNER_WESTERN
	unsigned int m_uReserved			:31;		///< ����ʹ��
}CSUDITunerSatellitePar_S;

/**@brief �������ֵ���(DVB-S/S2)tuner��LNB�������ݽṹ*/
typedef struct 
{
	unsigned int m_uLowFrequency;				///< low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	unsigned int m_uHighFrequency;				///< high LNB frequency in Universal Mod [unit:KHz]
	unsigned int m_uCenterFrequency;			///< middle LNB frequency in Universal Mod [unit:KHz]
	unsigned int m_uSatCRNo			:16;    ///< �໧���õ��ŵ�·����(SatCR)��ͨ��ѡ��, 0---7
	unsigned int m_uSatCRLNBNo			:16;	///< �໧���õ��ŵ�·����(SatCR)��LNBѡ��, CSUDITunerQPSKUnicableLNBType_Eö�����Ͷ���
	unsigned int m_uLnbType			:4;	///< ��Ƶͷ����CSUDITunerLNBType_E ����ö��
	unsigned int m_uLnbPower			:2;	///< CSUDITunerSwitchStatus_E ����ö��
	unsigned int m_uOptions                 	:2;     ///<�໧���õ��ŵ�·����(SatCR)������ѡ��, CSUDITunerQPSKOption_Eö�����Ͷ���
	unsigned int m_uReserved		 	:24;	///< ����ʹ��
}CSUDITunerLNB_S;

/**@brief �������ֵ���(DVB-S/S2)tuner�����߲����ݽṹ(ͬʱ����DVB-S/S2)*/
typedef struct 
{
	unsigned int m_uSwitch22k					:1;		///< CSUDITunerSwitchStatus_E ����ö��
	unsigned int m_uSwith12v					:1;		///< CSUDITunerSwitch012V_E����ö��
	unsigned int m_uDisEqCType					:1;		///< ��չʹ��
	unsigned int m_uAntennaKey					:8;		///< ��չʹ��
	unsigned int m_uReserved					:21;	        ///< ����ʹ��
}CSUDITunerAntennaPar_S;

/**@brief tunerģ��ͳһ���ź�Ƶ�ʡ�������ƫ�����ݽṹ*/
typedef struct 
{
	unsigned int m_uOffsetFrequency_23		:23;	///< Ƶ�ʵ�ƫ��ֵ����λKHz
	unsigned int m_uOffsetSymbolRate_8		:8;	///< �����ʵ�ƫ��ֵ����λKbps
	unsigned int m_uIQ_1				:1;	///< 0-Normal; 1-Inverse
}CSUDITunerTransponderOffset_S;

/**@brief ���ǵ���tuner�Ĵ�disqec��Ϣ���ź����ݽṹ(ͬʱ����DVB-S/S2)*/
typedef struct 
{
	CSUDITunerSatelliteDeliver_S m_sSatelliteDeliver;	///< ���ǻ�Ƶ����Ϣ�ṹ��
	CSUDITunerLNB_S			m_sLNB;			///< ��Ƶͷ��Ϣ�ṹ��
	CSUDITunerDiSEqC1020_S		m_sDiSEqC1020;           ///< diseq1.0��Ϣ�ṹ��
	CSUDITunerDiSEqC1121_S		m_sDiSEqC1121;           ///< diseq1.1��Ϣ�ṹ��
	CSUDITunerDiSEqC1222_S		m_sDiSEqC1222;           ///< diseq1.2��Ϣ�ṹ��
	CSUDITunerSatellitePar_S	m_sSatellitePar;	///< ���ǲ�����Ϣ�ṹ��
	CSUDITunerAntennaPar_S		m_sAntennaPar;           ///< ���߲�����Ϣ�ṹ��
	CSUDITunerTransponderOffset_S	m_sTransponderOffset;	///< Ƶ��ƫ��ֵ��Ϣ�ṹ��
}CSUDITunerSRCSatelliteDeliver_S;


/**@brief tunerģ��ͳһ�źŲ�������*/
typedef union 
{
	CSUDITunerCableDeliver_S	m_sCableDeliver;                ///< ���߻�Ƶ����Ϣ�ṹ��
	CSUDITunerSRCSatelliteDeliver_S	m_sSourceSatelliteDeliver;	///< ���ǻ�Ƶ����Ϣ�ṹ��
	CSUDITunerTerrestrialDeliver_S	m_sTerrestrialDeliver;          ///< �����Ƶ����Ϣ�ṹ��
}CSUDITunerSRCDeliver_U;

/**@brief tuner ���Ͷ���*/
typedef enum
{
	EM_UDITUNER_TYPE_UNKNOWN,
	EM_UDITUNER_TYPE_QAM,		///<  ����tuner����,��Ҫ����QAM���Ʒ�ʽ
	EM_UDITUNER_TYPE_COFDM,	   	///<  ����tuner����,��Ҫ����COFDM���Ʒ�ʽ
	EM_UDITUNER_TYPE_QPSK		///<  ����tuner����,��Ҫ����QPSK,8PSK���Ʒ�ʽ
}CSUDITunerType_E;

/**@brief tuner���ݴ������Ͷ���*/
typedef enum
{
	EM_UDITUNER_DATATRANSFER_UNKNOWN,	///< �Ƿ�ֵ
	EM_UDITUNER_DATATRANSFER_DMA,		///< DMA���ݴ���ģʽ��Ƶͷ����������DMA��ʽֱ�Ӵ��䵽Demux�������������Ҫʹ��DMA���͵�Demux���к���
	EM_UDITUNER_DATATRANSFER_SOFT		///< ��ע�����ݴ���ģʽ�ĸ�Ƶͷ���������������ʽע�뵽Demux�������������Ҫʹ��INJECT���͵�Demux���к��������н�����ע�뵽Demux
}CSUDITunerDataTransferType_E;

/**@brief Tuner�ź���Ϣ����ö�ٶ��� */
typedef enum
{
	EM_UDITUNER_SIGNAL_QUALITY      = 1 << 0,       ///<�ź�����
	EM_UDITUNER_SIGNAL_STRENGTH     = 1 << 1,       ///< �ź�ǿ�ȣ��ȼ�
	EM_UDITUNER_ERROR_RATE          = 1 << 2,       ///< ������
	EM_UDITUNER_SIGNAL_LEVEL        = 1 << 3,       ///< �ź������ƽ
	EM_UDITUNER_SIGNAL_CN           = 1 << 4,       ///<�ź������
	EM_UDITUNER_CUR_FREQU           = 1 << 5        ///<�ź�Ƶ��
}CSUDITunerSignalInfoType_E;

/**@brief tuner�豸������Ϣ����*/
typedef struct
{
	CSUDITunerType_E m_eType;	   ///< Tuner����
	CSUDITunerDataTransferType_E m_eTransferType;	///< Tuner���ݴ���ģʽ
	CSUDI_BOOL m_bPIDFilterSupport;		///< �Ƿ�֧��PID���ˣ����ָ�Ƶͷ��֧�ֽ�������PID������������Լ��ٴ�������ע����ʽ�ĸ�Ƶͷ��DVB-TH��CMMB����ʽ�ĸ�Ƶͷ�ر�����
}CSUDITunerInfo_S;

/**@brief tuner�ص���Ϣ���Ͷ���

Tuner��Ƶ״̬�仯������¼���ο���ͼ:
@note 
	- Init ָtuner��ʼ�����֮��û�н�����Ƶ֮ǰ��״̬
	- Locked ָtuner������Ƶ������������Ƶ�ɹ�֮���״̬
	- Unlocked ָtunerû����Ƶ�ɹ���״̬
	- Init -> Locked ��ָtuner���е�һ����Ƶ��������Ƶ�ɹ�֮����CONNECTED��Ϣ
	- Init -> Unlocked ��ָtuner���е�һ����Ƶ��������Ƶʧ��֮�󣬲��ϳ��Խ�����Ƶ�������ڼ�ᷢ��SEARCHING��Ϣֱ����Ƶ�ɹ�
	- Unlocked -> Locked ��ָtuner����Ƶʧ��״̬֮���ٴ���Ƶ�ɹ����ɹ�֮����CONNECTED��Ϣ
	- Locked->Unlocked ��ָtuner��Ƶʧ�ܺ󣬷���һ��LOST��Ϣ�������Ͻ�����Ƶ�������ڼ�ᷢ��SEARCHING��Ϣֱ����Ƶ�ɹ�
@image HTML udi_tuner_event.jpg
*/
typedef enum
{
      EM_UDITUNER_SIGNAL_CONNECTED,   ///< Ƶ�������ɹ�
      EM_UDITUNER_SIGNAL_LOST,        ///< ������תΪʧ��ʱ���ʹ���Ϣ
      EM_UDITUNER_SIGNAL_SEARCHING    ///< ����Ϣ��ʾ��Ƶʧ�ܣ��������������Ƶ��ֱ����Ƶ�ɹ��������������������������Ƶ����л�������Ҫ������Ϣ��
}CSUDITunerCallbackEvent_E;

/**@brief tuner�ص���Ϣ���ݽṹ����*/
typedef struct 
{  										
	 unsigned int  m_dwTunerId;                    ///< �ص���Ϣ��Ӧ��Tuner�豸��
	 CSUDITunerCallbackEvent_E m_eEvent;      		///< ��ǰ��CSUDITunerCallbackEvents_E���� 
	 unsigned int  m_uFrequency;                    ///< �ص���Ϣ��Ӧ��Ƶ�㣬��λKHz
	 unsigned int  m_uSymbolRate;                   ///< �ص���Ϣ��Ӧ�ķ����ʣ���λKbps
	 unsigned int  m_uBandWidth;                    ///< �ص���Ϣ��Ӧ�Ĳ��Σ���CSUDITunerBanuidth_E����
	 unsigned int  m_uQAMMode;                      ///< �ص���Ϣ��Ӧ�ĵ��Ʒ�ʽ����CSUDITunerQAMMode_E����
	 void *        m_pvUserData;                    ///< �û���ע��ص�����ʱ�����ָ��
}CSUDITunerCallbackMessage_S;

/**
@brief tuner�¼��ص�����ԭ�Ͷ���

@param[in] eType �����ص��ĸ�Ƶͷ����
@param[in] psSourceCallbackMessage �ص���Ϣ���ݣ����CSUDITunerCallbackMessage_S���塣
*/
typedef  void ( *CSUDITunerCallback  )( CSUDITunerType_E eType, CSUDITunerCallbackMessage_S* psMessage);

/**@brief tuner�ź���Ϣ���ݽṹ����*/
typedef struct 
{
	int 		 m_nQuality;	    ///< �ź�������0��100��
	int		     m_nStrength;	    ///< �ź�ǿ�ȣ�0 ��100��
	int		     m_nErrorRate;	    ///< �����ʣ�1*10^7����Ԫ�д�����Ԫ�ĸ�����(����10^7�͵õ������ʣ�,ʧ��ʱ��������Ϊ1*10^7����100%���롣	
	unsigned int m_nCurrentFreq;	///< ���������ȡ���ź�Ƶ��,unit[KHz]
	double 		 m_dSignalLevel;    ///< �źŵ�ƽ,��ȷ��С�����һλ,unit(QAM: dBuv; COFDM: dBm; QPSK:dBm)	
	double 		 m_dSignalCN;	    ///< �����,��ȷ��С�����һλ, unit[dB]
	int		     m_nReserved[3];	///< ��������
}CSUDITunerSignalInfo_S;

/**
@brief ����ָ����Ƶ�������Ƶ����.

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] punDeliver �û����Ƶ����Ϣ�����ݿ�ĵ�ַ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����롣\n
-- CSUDITUNER_ERROR_BAD_PARAMETER����������\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID���Ƿ�ID��\n
-- CSUDITUNER_ERROR_I2C_ERR��I2C���ߴ���\n
@note ���ӿ��첽ִ�У����سɹ�����ʾ�����ѽ��գ�����ʾ����ִ�н����
@note ���ӿڷ��غ������������Ƶ�����ϴβ�ͬ�����������ֹͣ���ϸ�Ƶ���demux���ݽ���
*/
CSUDI_Error_Code CSUDITunerConnect(CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punDeliver);

 /**
@brief ע��Tuner��Ƶ��Ϣ�ص�

@param[in] dwTunerId tuner�豸�š�
@param[in] fnTunerCallback �ص����������
@param[in] pvUserData �û�������Զ���ָ��,����֪ͨ�������ظ��û���������Ϣ��
@return ����ɹ�����CSUDI_SUCCESS, ʧ�ܷ��ش������
@note
- �ص�������ע���������֧��8�������Ӧ��ע��ص�����������֧�ָ���������CSUDITUNER_ERROR_NO_MEMORY
- һ��tuner id,callback,userdata��ͬ����һ��Tuner��Ƶ��Ϣ�ص���������ע��������ȫһ��ʱ���ڶ��ν�����CSUDITUNER_ERROR_ALREADY_ADDED
*/
CSUDI_Error_Code CSUDITunerAddCallback(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData );

 /**
@brief ɾ��ָ������Ƶ�ص�������ע��

@param[in] dwTunerId tuner�豸�š�
@param[in] fnTunerCallback �ص����������
@param[in] pvUserData ͬCSUDITunerAddCallback��pvUserData
@return�ɹ�����CSUDI_SUCCESS ����ûص�����������û��ע�ᣬ������CSUDI_FAILURE
@note ���ڿ��ƶ���Tuner�����䱻�γ�������Ҫ����ɾ�����Ӧ�Ļص�ע�ᣬ�����Ի�ռ�ò�����Դ����Ҫ���ڴ棩
*/
CSUDI_Error_Code CSUDITunerDelCallback(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData);

 /**
@brief ȡ�õ�ǰ�����������ź���Ϣ

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸��
@param[in] u32SignalInfoMask ���ȡ���ź���Ϣ����,ȡֵΪCSUDITunerSignalInfoType_E�е�ö��ֵ��ö��������ֵ��򣬶�ӦλΪ1�򷵻ظ���Ϣ��Ϊ0�򲻷��أ���ʱ��Ӧ�ṹ��ֵ�ǲ�ȷ���ġ�
@param[out] psSignalInfo ���������ȡ���ź���Ϣ��
@return 
- �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
- ���ʧ�ܷ������´������:\n
-- CSUDITUNER_ERROR_BAD_PARAMETER ����������\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID���Ƿ��豸ID��\n 
-- CSUDITUNER_ERROR_I2C_ERROR��I2C���ߴ���\n 
@note ��ȡ������Щ��Ϣ���ܻ��漰һЩ��ͬ��Tuner������Ϊ���Ч�ʣ��뾡������ȡ���ע����Ϣ��������ȫ������Ϊ���������£���Ҫ����ϢԽ�࣬�ýӿ���Ҫ��ʱ��Խ��
@note �ýӿ�һ��ҪѸ�ٷ��أ����ػ�ȡ�ǳ���ȷ����Ϣ���������ڸýӿ���ֱ�ӵ������������ӿڲ�ѯ��������udi�㶨ʱ��ѯ�����������Ϣ��ȡ���������ñ��ӿ�ʱֱ�ӻ�ȡ���ɡ�
*/
CSUDI_Error_Code CSUDITunerGetSignalInfo( CSUDI_UINT32 dwTunerId, CSUDI_UINT32 u32SignalInfoMask, CSUDITunerSignalInfo_S *psSignalInfo);

 /**
@brief ȡ��Tuner�豸��������Ϣ

@param[in] dwTunerId ���ڱ��Tuner�豸���豸��
@param[out] psTunerInfo ���������ȡ��Tuner������Ϣ
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDITunerGetDeviceInfo( CSUDI_UINT32 dwTunerId, CSUDITunerInfo_S *psTunerInfo);

/**
@brief ��ȡ�����豸��Id

�������ƶ���Tuner�豸�͹̶����豸
@param[out] pdwIds ���ڴ洢���д��ڵ��豸��Id
@param[in] nMaxIdCnt pdwIds����Ĵ�С,������ܷ��ض����豸Id
@param[out] pnActIdCnt ʵ�ʵ��豸ID��
@return �ɹ�����CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ
@note
- Ϊ���ּ��ݣ����зǿ��ƶ��豸���豸���Դ�0��ʼ����
- ����pdwIdsΪCSUDI_NULL����������±�ʾӦ�ý���ע�ж��ٸ�tuner�豸�������������Ǿ����Id
*/
CSUDI_Error_Code CSUDITunerGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt);

/**
@brief ����TunerӲ����Pid

- ����m_bPIDFilterSupportΪCSUDI_TRUE��Tuner������ͨ���ýӿ�����Ӳ����Pid�󣬷��ɻ�ȡ���ݡ�
- ����m_bPIDFilterSupportΪCSUDI_FALSE��Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED�����Ҳ���Ҫ���øýӿڼ��ɻ�ȡ��������
@param[in] dwTunerId Tuner�豸��
@param[in] nPID ����ӹ��˵�����Pid����Ӻ��Pid�����ݿ�ͨ�������Ӧ�ĺ���ģ�飨һ��ΪDemux��
@return �ɹ�����CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ
@note ��ʼ״̬�£�Tuner��ͨ���κ�Pid������
*/
CSUDI_Error_Code CSUDITunerAddPID (CSUDI_UINT32 dwTunerId ,int nPID);

/**
@brief ɾ��TunerӲ����Pid

- ����m_bPIDFilterSupportΪCSUDI_TRUE��Tuner������ͨ���ýӿ�ɾ��CSUDITunerAddPID�ӿ����ӵ�Ӳ����Pid����ֹ��ȡ��ӦPID�����ݡ�
- ����m_bPIDFilterSupportΪCSUDI_FALSE��Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED�����Ҳ���Ҫ���øýӿڼ��ɻ�ȡ��������
@param[in] dwTunerId Tuner�豸��
@param[in] nPID ��ɾ�����˵�����Pid��ɾ�����Pid�����ݲ���ͨ�������Ӧ�ĺ���ģ�飨һ��ΪDemux��
@return �ɹ�����CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ
@note nPIDΪ~0��ʾɾ����Tuner�µ�����pid���ˣ����˺��޷�ͨ����Tuner��ȡ�κ����ݣ������ٴε���CSUDITunerAddPID��
@note ��ʼ״̬�£�Tuner��ͨ���κ�Pid������
@see CSUDITunerAddPID
*/
CSUDI_Error_Code CSUDITunerRemovePID (CSUDI_UINT32 dwTunerId ,int nPID);

/*-----------------���½ӿڶ�����ƶ�Tuner����ؽӿ�---------------------*/
/**
@brief ����豸֪ͨ�ص�����

���RDIģ��CSUDIRDIAddCallback_F�ӿڶ��壬�����ڴ���TUNER�����͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,���豸״̬�����仯ʱ���øú���
@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
@return �ɹ���ӷ���CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��ӻص�ǰ�������豸�仯��Ϣ,���Ჹ��֪ͨ.��������ʱ��Щ�豸�ʹ���,���ñ�����ʱ����õ�֪ͨ.
@note ��ƽ̨����Ҫ֧���κο��ƶ�Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDITunerRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief ɾ���豸֪ͨ�ص�����

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ���TUNER�����͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,֮ǰע��ĺ���
@param[in] pvUserData �û�����,������ע��ʱ��һ��
@return �ɹ�ɾ������CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note fnCallback��pUserDataһ��Ψһȷ��һ���ص�����
@note ��ƽ̨����Ҫ֧���κο��ƶ�Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDITunerRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief ��ȫ�Ƴ����ƶ�Tuner�豸

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ���TUNER�����͵Ŀ��ƶ��豸
@param[in] dwTunerId Ŀ���豸ID
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note
- ֻ��ɾ�����ƶ��豸����ͼɾ�������豸�����򷵻�CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED��ͨ���豸���豸Id���ж��豸���ͣ�ɾ���豸�󲻿���ͨ�����豸Id��ȡ�豸��Ϣ���û����øýӿڻᴥ��EM_UDIRDI_PLUGOUT_SAFE���͵�PLUGOUT�¼��� 
- ��ƽ̨����Ҫ֧���κο��ƶ�Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDITunerRDIRemoveDev(CSUDI_UINT32 dwTunerId);

/**
@brief �ӿ��ƶ�Tuner��ȡ����

�ɹ���Ƶ�����ù���PID(�����Ҫ)�󣬿�ͨ���ýӿڶ�ȡ��TS������
@param[in] dwTunerId Ŀ���豸Id
@param[in] pvData ָ��������ݵ�Buffer
@param[in] nBuffLength pvData�ĳ���
@param[out] pnReadLength ʵ�ʶ��������ݳ���
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note �ӿ�Ӧ���̷��أ������ܶ����κ����ݣ���*pnReadLength==0�����Է��سɹ�
@note ���RDIģ����CSUDIRDIIORead_Fԭ��
@note ��ƽ̨����Ҫ֧���κο��ƶ�Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDITunerIORead(CSUDI_UINT32 dwTunerId,void* pvData,int nBuffLength,int * pnReadLength);


/**
@brief ��ȡһ��PlpID,PlpIDΪ0~255������

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[out] pbPlpIds ���ڷ�PLP ID�����飬�����СӦ������256����������ڿ��õ�PLP ID�������������С�����ش���
@param[in] nPlpIdsCount pbPlpIdsָ��������С
@param[out] pnRetIdCount ���ڷ���ʵ�ʱ��浽pbPlpIds�е�PlpID����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����롣\n
-- CSUDITUNER_ERROR_BAD_PARAMETER����������\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID���Ƿ�ID��\n
-- CSUDITUNER_ERROR_I2C_ERR��I2C���ߴ���\n
@note ���ӿ�ΪDVB-T2ר�á�
*/
CSUDI_Error_Code CSUDITunerT2GetPlpInfo(CSUDI_UINT32 dwTunerId, CSUDI_UINT8* pbPlpIds, int nPlpIdsCount, int* pnRetIdCount);

#ifdef __cplusplus
}
#endif

/** @} */

#endif 

