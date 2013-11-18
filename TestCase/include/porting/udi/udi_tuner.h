/**@defgroup CSTUNER tuner ������ͳһ��tuner�ӿڲ�������
@brief ��ģ����Ҫ������ͳһ��tuner�ӿڲ�������
@brief ͳһ��tuner�ӿ�֧��DVB-C��׼(QAM���Ʒ�ʽ),DVB-S/DVB-S2��׼(QPSK���Ʒ�ʽ),DVB-T��׼(COFDM��DMB-TH���Ʒ�ʽ),�Լ��ֻ�����CMMB��׼��tuner

@version 0.1 2008/10/23 ��ʼ�汾
@{
*/

#ifndef _UDI_TUNER_H_
#define _UDI_TUNER_H_

#include "udi_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif
	

/**@brief QPSK����TS��(������)ʱ�Ӽ��Զ���*/
typedef enum
{
	LOW_TO_HIGH,	/**< ������*/
	HIGH_TO_LOW	    /**< �½���*/
}T_ClockPolarity;

/**@brief QPSK����Diseqc1.0�˿ڶ���*/
typedef enum  
{
	DISEQC10_OFF,	/**< diseqc1.0ͨ��ѡ��ر�*/
	PORT_4_A,		/**< diseqc1.0��ѡһ���ص�Aͨ��*/
	PORT_4_B,		/**< diseqc1.0��ѡһ���ص�Bͨ��*/
	PORT_4_C,		/**< diseqc1.0��ѡһ���ص�Cͨ��*/
	PORT_4_D,		/**< diseqc1.0��ѡһ���ص�Dͨ��*/
	PORT_2_A,		/**< diseqc1.0��ѡһ���ص�Aͨ��*/
	PORT_2_B		/**< diseqc1.0��ѡһ���ص�Bͨ�� */
}Switch_Port;

/**@brief ����tuner����ͨ��Band����*/
typedef enum 
{
	BAND_LOW,		/**< �Ͳ���*/
	BAND_HIGH		/**< �߲���*/
}Channel_Band;

/**@brief QPSK�����źż���������*/
typedef enum 
{
	POLARISATION_H,		/**< �߼�������-ˮƽ����*/
	POLARISATION_V,		/**< �߼�������-��ֱ����*/
	POLARISATION_L,     /**< Բ��������-��������*/
	POLARISATION_R      /**< Բ��������-��������*/
}Channel_Polarisation;

/**@brief QPSK����Diseqc1.0�е�Tone burst(��ѡһͨ��ѡ�� �ļ���ģʽ,���ѡһ��Ͽ���֧��2*N��ͨ��ѡ��)����*/
typedef enum 
{
	TONEBURST_NONE,		/**< Tone burstѡ��ر�*/
	TONEBURST_A,		/**< Tone burstѡ��Aͨ��*/
	TONEBURST_B			/**< Tone burstѡ��Bͨ��*/
}Switch_ToneBurst;

/**@brief QPSK����Diseqc�е�ͨ���ظ�ѡ����,DiSEqC Repeat(ָ���ظ�)��һ���ʵ�õĹ���,��ÿ��ָ���ظ����Ͷ��,���Ա�֤�л��ɿ�������,�ظ�̫��Ҳû��˼,���ǲ�����ٶ۵�*/
typedef enum 
{
	NOREPEAT,		/**< û��DiSEqC Repeat */
	ONCEREPEAT,		/**< ��Ҫһ��DiSEqC Repeat*/
	TWICEREPEAT		/**< ��Ҫ����DiSEqC Repeat*/
}Repeat_Time;

/**@brief QPSK����Diseqc1.2������ת��������*/
typedef enum 
{
	EASTERN,	/**< ��ת��*/
	WESTERN		/**< ����ת��*/
}RotateDirection;
	
/**@brief QPSK����LNB(LNB�ֽи�Ƶͷ��Low Noise Block�����������±�Ƶ����\n
	�书���ǽ�����Դ���͵������źž����Ŵ���±�Ƶ����Ku��C��\n
	���źű��L���Σ���ͬ����´��͸����ǽ��ջ��� \n
	������΢���������Ŵ�����΢����Ƶ������һ�����\n
	��һ��Ƶǰ�÷Ŵ�����ɣ�һ��֣ò����õ�Cͷ,��ƫ��ʹ��\n
	��KUͷ,LNB�϶�����̽�룬��·�����̽���⵽������������\n
	�Ž��е������Ŵ���±�Ƶ��������950~2150MHZ����ĵ�һ��Ƶ\n
	�źž��������������ֵ�г�����)���Ͷ��壬Universal LNB��˫����\n
	ͨ��LNB*/
typedef enum 
{
	SINGLE,					/**< ������LNB */
	UNIVERSAL,				/**< ˫����ͨ��LNB*/
	UNIVERSAL_NegLogic		/**< ��Ҫ���˵�� */
}LNBType;

/**@brief QPSK����12����ѹѡ���LNB��ѹ����*/
typedef enum 
{
	SWITCH_0V,		/**< 0���ص�ѹѡ�� */
	SWITCH_12V		/**< 12���ص�ѹѡ��*/
}Switch0_12V;

/**@brief QPSK�����е�FECҲ��ǰ�����(Forward Error Correction)��\n
���������ݽ��д����߳���Ϣ�ķ���,�������г��ִ���\n
,������������ٽ����ݡ���7/8Ϊ��,��ʵ��������,��һ��\n
TS����,ֻ��7/8��������װ�н�Ŀ���ݵ�PES��,�������1/8��\n
��,����������������������������ľ����롣���������\n
����������,���������Ŀ�ķ�������ë�صĻ�,��7/8�Ľ�Ŀ\n
���ݺñ��Ǿ���,��1/8�ľ�������ǰ�װ���������*/
typedef enum
{
	FEC_AUTO,		/**< FEC��ʽ�Զ�ѡ�� */
	FEC1_2,			/**< 1/2��ʽ��FEC*/
	FEC2_3,			/**< 2/3��ʽ��FEC*/
	FEC3_4,			/**< 3/4��ʽ��FEC*/
	FEC5_6,			/**< 5/6��ʽ��FEC */
	FEC7_8,			/**< 7/8��ʽ��FEC*/
	FEC8_9,			/**< 8/9��ʽ��FEC*/
	FEC3_5,			/**< 3/5��ʽ��FEC */
	FEC4_5,			/**< 4/5��ʽ��FEC*/
	FEC9_10,		/**< 9/10��ʽ��FEC*/
	FEC6_7			/**< 6/7��ʽ��FEC */
}CSQPSK_FECRates;

/**@brief DVB-S2�еĵ�Ƶ���ض���*/
typedef enum
{
	PILOT_OFF,		/**< ��Ƶ��*/
    	PILOT_ON		/**< ��Ƶ�ر�*/
}CSQPSK_PIILOT;

/**@brief DVB-S/S2�г��õĵ��Ʒ�ʽ����*/
typedef enum
{
	CS_AUTO_PSK,
	CS_QPSK,	/**< Quadrature Phase Shift Keying */
	CS_8PSK		/**< Eight Phase Shift Keying*/
}CSQPSK_MODULATION;

/**@brief �������ֵ��ӵı�׼����*/
typedef enum
{
	DVBS_STANDARD,		/**< DVB-S��׼�ź� */
	DVBS2_STANDARD,		/**< DVB-S2��׼�ź�*/
	DVBS_AUTO_STANDARD    /** <DVB-S��DVB-S2�Զ�ѡ��>*/
}CSQPSK_STANDARD;

/**@brief  Diseqc1.1�еĿ�ѡͨ��ѡ��,������Diseqc1.0�е�Tone burst*/
typedef enum
{
	OPTION_NONE,		/**< û�п�ѡͨ�� */
	OPTION_A,			/**< ѡ���ѡͨ��A */
	OPTION_B			/**< ѡ���ѡͨ��B */
}CSQPSK_OPTION;

/**@brief QPSK�����е�LNB��ѹ״̬����*/
typedef enum
{
	LNB_OFF,		/**< LNB��ѹ�ر�*/
	LNB_AUTO,		/**< LNB��ѹѡ���Զ� */
	LNB_14V,		/**< LNB��ѹѡ��14����*/
	LNB_18V		    /**< LNB��ѹѡ��18����*/
}CSQPSK_LNB_Status;

/**@brief ���ǵ���tuner���ź����ݽṹ(ͬʱ����DVB-S/S2)*/
typedef struct _CSTUNSatelliteDeliver_S
{
	DWORD	m_dwFrequency;						/**< Ƶ�ʣ���λMHZ*/
	DWORD	m_dwSymbolRate_24			:24;	/**< �����ʣ���λKHZ*/
	DWORD	m_dwFECInner_4				:4;		/**< CSQPSK_FECRates����ö��*/
	DWORD	m_dwPolarization_2			:2;		/**< Channel_Polarisation����ö��*/
	DWORD	m_ucPilot;                          /**< CSQPSK_PIILOT����ö��--> add for s2*/
	DWORD	m_ucSatCRNo;                        /**< �໧���õ��ŵ�·����(SatCR)��ͨ��ѡ��*/
	DWORD	m_ucSatCRLNBNo;                     /**< �໧���õ��ŵ�·����(SatCR)��LNBѡ��*/
	DWORD	m_dwOptions                 :2;     /**<CSQPSK_OPTION����ö��*/
	DWORD	m_dwModulations             :2;		/**< CSQPSK_MODULATION����ö��*/
	DWORD  	m_dwstandards               :1;		/**< CSQPSK_STANDARD����ö��*/
	DWORD	m_dwreserved                :2;		/**< ����ʹ��*/
}CSTUNSatelliteDeliver_S;


/**@brief QAM_�����ӿڷ���ֵ����*/
typedef enum
{
	CS_NOT_DEFINED,	    /**< δ�����QAM������һ������±�ʾ��Ч����*/
	CS_QAM16,			/**< 16QAM����*/
	CS_QAM32,			/**< 32QAM����*/
	CS_QAM64,			/**< 64QAM����*/
	CS_QAM128,			/**< 128QAM����*/
	CS_QAM256			/**< 256QAM���� */
}T_QAMMode;

/**@brief �������ֵ���(DVB-C)tuner���źŲ������ݽṹ*/
typedef struct _CSTUNCableDeliver_S
{
	DWORD m_dwFrequency;					/**< Ƶ�ʣ���λKHZ*/
	DWORD m_dwSymbolRate_24				:24;/**< �����ʣ���λKHZ*/
	DWORD m_dwModulation_8				:8;	/**< T_QAMMode����ö��*/
}CSTUNCableDeliver_S;


/**@brief cofdm �źŴ�����*/
typedef enum _T_BANDWIDTH							
{
	BAND_8M,    /**<����8M*/
	BAND_7M,    /**<����7M*/
	BAND_6M     /**<����6M*/
}T_BANDWIDTH;

/**@brief �������ֵ���(DVB-T)tuner���źŲ������ݽṹ)*/
typedef struct _CSTUNTerrestrialDeliver_S
{
	DWORD m_dwFrequency;					 /**< Ƶ�ʣ���λKHZ*/
	DWORD m_dwBanwith_3					:3;	 /**< T_BANDWIDTH����ö��*/
	DWORD m_dwFlag_2					:2;		/**< DVB-T/DVB-T2��־:00-T;01-T2;10,11����*/
	DWORD m_dwPlpID_8					:8;     /**< T2�洢PLPID*/
	DWORD m_dwReserved_19				:19;  	/**< ����ʹ��*/
}CSTUNTerrestrialDeliver_S;

/**@brief �������ֵ���(DVB-S/S2)tuner��DiSEqC1.0��׼�Ĳ���*/
typedef struct _CSTUNDiSEqC1020_S
{
	DWORD m_dwToneburst					:2;		/**< Switch_ToneBurst����ö��*/
	DWORD m_dwCommitted					:4;		/**< Switch_Port����ö��*/
	DWORD m_dwReserved					:26;	/**< ����ʹ��*/
}CSTUNDiSEqC1020_S;


/**@brief �������ֵ���(DVB-S/S2)tuner��DiSEqC1.1��׼�Ĳ���*/
typedef struct _CSTUNDiSEqC1121_S
{
	DWORD m_dwUncommitted					:4;	/**< �μ�DiSEqC1.1��׼*/
	DWORD m_dwCommitted					:4;		/**< �μ�DiSEqC1.1��׼*/
	DWORD m_dwRepeatedTimes					:2;	/**< �ظ�����Repeat_Time����ö��*/
	DWORD m_dwReserved					:22;	/**< ����ʹ��*/
}CSTUNDiSEqC1121_S;

/**@brief �������ֵ���(DVB-S/S2)tuner��DiSEqC1.2��׼�Ĳ���*/
typedef struct _CSTUNDiSEqC1222_S
{
	DWORD m_dwUsals						:2;		/**< ��תλ�ã�λ��ֵȡ��m_dwIndex 2 -�����û����õľ�γ�Ƚ�����Ӧת������*/
	DWORD m_dwIndex						:5;		/**< ��ת����λ��ֵ��*/
	DWORD m_dwReserved					:25;	/**< ����ʹ��*/
}CSTUNDiSEqC1222_S;

/**@brief �������ֵ���(DVB-S/S2)tuner��LNB�������ݽṹ*/
typedef struct _CSTUNLNB_S
{
	DWORD m_dwLowFrequency;				    /**< low LNB frequency in Universal Mod or 
													LNB frequency in Single Mod [unit:KHz]*/
	DWORD m_dwHighFrequency;				/**< high LNB frequency in Universal Mod [unit:KHz]*/
	DWORD m_dwCenterFrequency;			    /**< middle LNB frequency in Universal Mod [unit:KHz]*/
	DWORD m_dwLnbType					:4;	/**< ��Ƶͷ����LNBType����ö��*/
	DWORD m_dwLnbPower					:2;	/**< CSHDISwitch����ö��*/
	DWORD m_dwLnbVoltage				:2;	/**< CSQPSK_LNB_Status����ö��*/
	DWORD m_dwReserved					:24;/**< ����ʹ��*/
}CSTUNLNB_S;

/**@brief �������ֵ���(DVB-S/S2)tuner������λ�ò����ṹ(ͬʱ����DVB-S/S2)*/
typedef struct _CSTUNSatellitePar_S
{
	DWORD m_dwOrbitalPosition			:16;	/**< �������ڵľ���λ��*/
	DWORD m_dwWestEastFlag				:1;		/**< �������ڵ�λ�õĶ�������־��0 -EASTERN, 1 - WESTERN*/
	DWORD m_dwReserved					:15;	/**< ����ʹ��*/
}CSTUNSatellitePar_S;

/**@brief �������ֵ���(DVB-S/S2)tuner�����߲����ݽṹ(ͬʱ����DVB-S/S2)*/
typedef struct _CSTUNAntennaPar_S
{
	DWORD m_dwSwitch22k					:1;		/**< CSHDISwitch����ö*/
	DWORD m_dwSwith12v					:1;		/**< Switch0_12V����ö��*/
	DWORD m_dwTunerMask					:4;		/**< 0x01 - ����1��tuner��0x02 - ����2��tuner*/
	DWORD m_dwDisEqCType				:1;		/**< �����ע��*/
	DWORD m_dwAntennaKey				:8;		/**< �����ע��*/
	DWORD m_dwSatCrNum_3				:3;		/**< �����ע��*/
	DWORD m_dwLnbNum_3					:3;		/**< �����ע��*/
	DWORD m_dwReserved					:11;	/**< ����ʹ��*/
}CSTUNAntennaPar_S;

/**@brief tunerģ��ͳһ���ź�Ƶ�ʡ�������ƫ�����ݽṹ*/
typedef struct _CSTUNTransponderOffset_S
{
	DWORD m_dwOffsetFrequency_23		:23;	/**< Ƶ�ʵ�ƫ��ֵ����λKHZ*/
	DWORD m_dwOffsetSymbolRate_8		:8;		/**< �����ʵ�ƫ��ֵ����λKHZ*/
	DWORD m_dwIQ_1						:1;		/**< 0-Normal; 1-Invers*/
}CSTUNTransponderOffset_S;


/**@brief ���ǵ���tuner�Ĵ�disqec��Ϣ���ź����ݽṹ(ͬʱ����DVB-S/S2)*/
typedef struct _CSTUNSRCSatelliteDeliver_S
{
	CSTUNSatelliteDeliver_S m_SatelliteDeliver;		/**< ���ǻ�Ƶ����Ϣ�ṹ��*/
	CSTUNLNB_S				m_LNB;					/**< ��Ƶͷ��Ϣ�ṹ��*/
	CSTUNDiSEqC1020_S		m_DiSEqC1020;           /**< diseq1.0��Ϣ�ṹ��*/
	CSTUNDiSEqC1121_S		m_DiSEqC1121;           /**< diseq1.1��Ϣ�ṹ��*/
	CSTUNDiSEqC1222_S		m_DiSEqC1222;           /**< diseq1.2��Ϣ�ṹ��*/
	CSTUNSatellitePar_S		m_SatellitePar;			/**< ���ǲ�����Ϣ�ṹ��*/
	CSTUNAntennaPar_S		m_AntennaPar;           /**< ���߲�����Ϣ�ṹ��*/
	CSTUNTransponderOffset_S	m_TransponderOffset;/**< Ƶ��ƫ��ֵ��Ϣ�ṹ��*/
}CSTUNSRCSatelliteDeliver_S;

/**@brief tunerģ��ͳһ�źŲ�������*/
typedef union _CSTUNSRCDeliver_U
{
	CSTUNCableDeliver_S		    m_CableDeliver;             /**< ���߻�Ƶ����Ϣ�ṹ��*/
	CSTUNSRCSatelliteDeliver_S	m_SourceSatelliteDeliver;	/**< �����涨��*/
	CSTUNTerrestrialDeliver_S	m_TerrestrialDeliver;       /**< �����Ƶ����Ϣ�ṹ��*/
}CSTUNSRCDeliver_U;



/**@brief tuner�ص��������Ͷ��壬������*/
typedef enum
{
	CTUN_CALLBACK_QPSK=2,		/**<  �ص��е��źŲ�����QPSK���Ʒ�ʽ��*/
	CTUN_CALLBACK_COFDM,	    /**< �ص��е��źŲ�����COFDM���Ʒ�ʽ��*/
	CTUN_CALLBACK_QAM		    /**<  �ص��е��źŲ�����QAM���Ʒ�ʽ��*/
}CSTUNCallbackType_E;

/**@brief tuner�ص���Ϣ���Ͷ���*/
typedef enum
{
      TUN_SIGNAL_CONNECTED,          	/**< Ƶ�������ɹ�*/
      TUN_SIGNAL_DROPPED,             	/**< ȡ����Ƶʱ���ʹ���Ϣ*/
      TUN_SIGNAL_LOST,                	/**< ������תΪʧ��ʱ���ʹ���Ϣ*/
      TUN_SIGNAL_SEARCHING,           	/**< ��������Ƶ�㣬�������ʧ�ܻ�һֱ���͸���Ϣ*/
      TUN_SIGNAL_NOSIGNAL				/**< ���ź�,ģ�����ȷ������ʧ��ʱ����һ�θ���Ϣ*/
}CSTUNCallbackEvents_E;

/**@brief tuner�ص���Ϣ���ݽṹ����*/
typedef struct _CSTUNCallbackMessage_S
{
 CSTUNCallbackEvents_E m_SourceCallbackEvent;   /*��ǰ�涨��*/
 DWORD dwFrequency;                             /**< �ص���Ϣ��Ӧ��Ƶ�㣬��λKHZ*/
 DWORD dwSymbolRate;                            /**< �ص���Ϣ��Ӧ�ķ����ʣ���λKbps*/
 DWORD dwBandWidth;                             /**< �ص���Ϣ��Ӧ�Ĳ��Σ�ʵ�ʶ�ӦHDI��
 										��T_BANDWIDTHö�����ͣ�ֵ���壺0----8M, 1----7M, 2----6M��*/
 DWORD dwQAMMode;                               /**< �ص���Ϣ��Ӧ�ĵ��Ʒ�ʽ��ʵ�ʶ�ӦHDI�е�T_QAMModeö�����ͣ�ֵ���壺
 										0----Not Define, 1----16QAM, 2----32QAM, 3----64QAM, 4----128QAM, 5----256QAM��*/
 DWORD dwTunerIndex;                            /**< �ص���Ϣ��Ӧ��Tuner�����ţ�ʵ�ʶ�ӦHDI�е�CSHDITunerIndexö�����ͣ�
 										ֵ���壺0----tuner 1, 1----tuner 2, 2----���tuner����*/
 void *pvUserData;                             /**< �û���ע��ص�����ʱ�����ָ��*/
}CSTUNCallbackMessage_S;

/**@brief tuner�¼��ص�����ԭ�Ͷ���*/
typedef  void ( *CSTUNCallback  )( CSTUNCallbackType_E nSourceCallbackType, CSTUNCallbackMessage_S* pSourceCallbackMessage);

/**@brief tuner�ص���Ϣ���ݽṹ����*/
typedef struct _CSTUNSignalInfo_S
{
	int 		nQuality;	/**< �ź�������0��100��*/
	int		nStrength;	    /**< �ź�ǿ�ȣ�0 ��100��*/
	int		nErrorRate;	    /**< �����ʣ�1*107������Ԫ�д�����Ԫ�ĸ�����(����107�͵õ������ʣ�*/
	int		nCurrentFreq;	/**< ���������ȡ���ź�Ƶ��*/
	int		nReserved[3];	/**< ��������*/
}CSTUNSignalInfo_S;

/**
@brief ����ָ����Ƶ�������Ƶ����.

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc������ CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@param[in] pDeliver �û����Ƶ����Ϣ�����ݿ�ĵ�ַ
@return ����ɹ�����TRUE,���ʧ�ܷ���FALSE
*/
BOOL CSTunerConnectSource(HCSHANDLE hHandle, const CSTUNSRCDeliver_U* pDeliver);

 /**
@brief  ע��ָ���Ļص�����

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc������ CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@param[in] callback �ص��������
@param[in] pData �û�������Զ���ָ��
@return ����ɹ�����TRUE,���ʧ�ܷ���FALSE
@note
- �ص����������ע�����Ϊ20��������20��������FALSE
- ע��ʱ����tuner����callbackΪ��ֵ��
*/
BOOL CSTunerAddCallback( HCSHANDLE hHandle, CSTUNCallback  callback, void * pData );

 /**
@brief ɾ��ָ���Ļص�������ע�ᡣ

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc������ CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@param[in] callback �ص��������
@return ����ûص�����������û��ע�ᣬ������FALSE
*/
BOOL CSTunerDelCallback( HCSHANDLE hHandle, CSTUNCallback  callback );

 /**
@brief ȡ�õ�ǰ�����������ź���Ϣ

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc������ CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@param[out] pSignalInfo ���������ȡ���ź���Ϣ
@return ����ź�������Ӧ�÷���TRUE,����FALSE
*/
BOOL CSTunerGetSignalInfo( HCSHANDLE hHandle, CSTUNSignalInfo_S *pSignalInfo);

/**
@brief ��ȡһ��PlpID,PlpIDΪ0~255������

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc()�� CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@param[out] pbPlpIds ���ڷ�PLP ID�����飬�����СӦ������256����������ڿ��õ�PLP ID�������������С�����ش���
@param[in] nPlpIdsCount pbPlpIdsָ��������С
@param[out] pnRetIdCount ���ڷ���ʵ�ʱ��浽pbPlpIds�е�PlpID����
@return 
- ����ɹ�����TRUE��
- ���ʧ�ܷ���FALSE
@note ���ӿ�ΪDVB-T2ר�á�
*/
BOOL CSTunerT2GetPlpInfo(HCSHANDLE hHandle, PBYTE pbPlpIds, int nPlpIdsCount, int* pnRetIdCount);

#ifdef __cplusplus
}
#endif

/** @} */

#endif 
