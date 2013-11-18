#ifndef _CSUSPDMCOMMENTYPE_H_
#define _CSUSPDMCOMMENTYPE_H_

#include "mmcp_typedef.h"
#include "mmcp_debug.h"

//�ַ�����������
typedef enum _CSUSPDMCodingType
{
	USP_CODING_UNKNOWEN	//δ֪����
  , USP_CODING_ASC = 1	//ASC��
  , USP_CODING_UNICODE	//UNICODE��
}CSUSPDMCodingType;

//��Ŀ��FAV���ԣ����û��Զ��壬������û�д�����
typedef enum _CSUSPDMServiceFAVLevel
{
	USPDM_SERVICE_FAV_ALL
  , USPDM_SERVICE_FAV_1 = 1
  , USPDM_SERVICE_FAV_2
  , USPDM_SERVICE_FAV_3
  , USPDM_SERVICE_FAV_4
  , USPDM_SERVICE_FAV_5
  , USPDM_SERVICE_FAV_6
  , USPDM_SERVICE_FAV_7
  , USPDM_SERVICE_FAV_8
}CSUSPDMServiceFAVLevel;

//��Ŀ���͡������п��Եõ������庬����μ� ETSI 300 468
typedef enum _CSUSPDMServiceType
{
	USPDM_SERVICE_RESERVED
  , USPDM_SERVICE_TV = 1
  , USPDM_SERVICE_RADIO
  , USPDM_SERVICE_TELETEXT
  , USPDM_SERVICE_NVOD_REFERENCE
  , USPDM_SERVICE_NVOD_TIME_SHIFT
  , USPDM_SERVICE_MOSAIC
  , USPDM_SERVICE_PAL
  , USPDM_SERVICE_SECAM
  , USPDM_SERVICE_D_MAC
  , USPDM_SERVICE_FM_RADIO
  , USPDM_SERVICE_NTSC
  , USPDM_SERVICE_DATA_BROADCAST
  , USPDM_SERVICE_RCS_MAP = 14
  , USPDM_SERVICE_RCS_FLS
  , USPDM_SERVICE_DVB_MHP
  , USPDM_SERVICE_3D = 0x1c
}CSUSPDMServiceType;

//����Iterator����ʱ�������ֶ�
typedef enum _CSUSPDMSortKey
{
	USPDM_SORT_INDEX							//����Service ID ����
  , USPDM_SORT_NAME								//����Service Name ����
}CSUSPDMSortKey;

//����Iteratorʱ������ʽ
typedef enum _CSUSPDMSortType
{
	USPDM_SORT_ASC								//����
  , USPDM_SORT_DESC								//����
}CSUSPDMSortType;

typedef enum _CSUSPDMFLASHBLOCK
{
    USPDM_FLASHBLOCK_32K
   ,USPDM_FLASHBLOCK_64K
   ,USPDM_FLASHBLOCK_128K
   ,USPDM_FLASHBLOCK_256K
   ,USPDM_FLASHBLOCK_512K
}CSUSPDMFlashBlock;

//��Ŀ����
typedef struct _SERVICEPAR
{
	DWORD m_dwLanguage_Code_6			:6;		//��Ŀ����
	DWORD m_dwReservied_2				:1;		//����
	DWORD m_dwFreeCAMode				:1;		//���Ʊ�־��0��δ���ţ�1������
	DWORD m_dwAudio_Type_2				:2;		//��Ƶ����//�Ѿ�����
	DWORD m_dwAudio_Vol_6				:6;		//��Ŀ�������û��趨
	DWORD m_dwSkip_1					:1;		//������־���û��趨
	DWORD m_dwDelete_1					:1;		//ɾ����־���û��趨
	DWORD m_dwLockState_1				:1;		//��Ŀ����־���û��趨
	DWORD m_dwChannelID_13				:13;	//Ƶ��ID��������Ҫ�������л��û��Լ��趨
	DWORD m_dwAudioChannelType			:2;		//�����������ԣ��û��趨
	DWORD m_dwAudio_Type_8				:8;		//��Ƶ���ͣ���������עService������UniNum���ԵĹ���ʹ��
	DWORD m_dwReserved					:22;	
}SERVICEPAR;

//��Ŀ��PID����
typedef struct _ServicePID
{
	DWORD m_dwPMTPID_13					:13;	//PMT PID
	DWORD m_dwPCRPID_13					:13;	//PCR PID
	DWORD m_dwSubtitle_PID_Index_6		:6;		//�˴���Ч��ͨ���ض������ӿڷ���
	DWORD m_dwVideoPID_13:13;					//Video PID
	DWORD m_dwAudioPID_13:13;					//Audio PID
	DWORD m_dwAudio_PID_Index_6:6;				//�˴���Ч��ͨ���ض������ӿڷ���	
	DWORD m_dwSbutitlePID_13:13;				//��Ч�ֶΣ����ݹ����ڲ��޴��ֶ�
	DWORD m_dwTeletextPID_13:13;				//��Ч�ֶΣ����ݹ����ڲ��޴��ֶ�
	DWORD m_dwTeletext_PID_Index_6		:6;		//�˴���Ч��ͨ���ض������ӿڷ���	
}ServicePID;

//����Container�ĳ�ʼ�������庬��μ���ؽӿ�
typedef struct _DATACONTAINERPAR
{
	void* pVoid;
	int nLen;
	int nType;
}DATACONTAINERPAR;

//����Ƶ������
typedef struct _SatelliteDeliver
{
	DWORD m_dwFrequency;						//Ƶ�ʣ���λMHZ
	DWORD m_dwSymbolRate_24				:24;	//�����ʣ���λKHZ
	DWORD m_dwFECInner_4				:4;
	DWORD m_dwPolarization_2			:2;		//������ʽ,0 - ˮƽ, 1 - ��ֱ
	DWORD m_dwReserved					:2;
}SatelliteDeliver;

//��չ����Ƶ������
typedef struct _SatelliteDeliverExt
{
	DWORD m_dwFrequency;						//Ƶ�ʣ���λMHZ
	DWORD m_dwSymbolRate_24				:24;	//�����ʣ���λKHZ
	DWORD m_dwFECInner_5				:5;		
	DWORD m_dwPolarization_2			:2;		//������ʽ,0 - ˮƽ, 1 - ��ֱ
	DWORD m_dwPilot_1					:1;		//��ֵ��Ч����������ʵpilotֵ
}SatelliteDeliverExt;

//����Ƶ��
typedef struct _CableDeliver
{
	DWORD m_dwFrequency;						//Ƶ�ʣ���λKHZ
	DWORD m_dwSymbolRate_24				:24;	//�����ʣ���λKHZ
	DWORD m_dwModulation_8				:8;		//���Ʒ�ʽ��0 - Not Define,  1 - 16QAM,	2 - 32QAM,	3 - 64QAM, 4 - 128QAM, 5 - 256QAM
}CableDeliver;

//����Ƶ��
typedef struct _TerrestrialDeliver
{
	DWORD m_dwFrequency;						//Ƶ�ʣ���λKHZ
	DWORD m_dwBanwith_3					:3;		//���Σ�0 - 8M, 1 - 7M, 2 - 6M
	DWORD m_dwReserved_29				:29;   
}TerrestrialDeliver;

//Ƶ���������
typedef struct _SourceID
{
	WORD m_wNetwork_id;							//Ƶ�������ID�������л�ȡ
	WORD m_wOriginal_network_id;				//Ƶ���ԭʼ����ID�������л�ȡ
	WORD m_wTSID;								//Ƶ�����ID�������л�ȡ
}SourceID;

//Ƶ��ƫ��
typedef struct _TransponderOffset
{
	DWORD m_dwOffsetFrequency_23		:23;	//Ƶ�ʵ�ƫ��ֵ����λKHZ
	DWORD m_dwOffsetSymbolRate_8		:8;		//�����ʵ�ƫ��ֵ����λKHZ
	DWORD m_dwIQ_1						:1;		//0-Normal; 1-Invers
}TransponderOffset;

//Ƶ������
typedef union _Deliver
{
	CableDeliver		m_CableDeliver;			//��ǰ�涨��
	SatelliteDeliver	m_SatelliteDeliver;		//��ǰ�涨��
	TerrestrialDeliver	m_TerrestrialDeliver;	//��ǰ�涨��
}Deliver;

//��չƵ������
typedef union _DeliverExt
{
	CableDeliver		m_CableDeliver;
	SatelliteDeliverExt	m_SatelliteDeliverExt;
	TerrestrialDeliver	m_TerrestrialDeliver;
}DeliverExt;

typedef struct _DiSEqC1020
{
	DWORD m_dwToneburst					:2;		// Tone Burst����: 0 - TONEBURST_NONE, 1 - TONEBURST_A, 2 - TONEBURST_B
	DWORD m_dwCommitted					:4;		// DiSEqC1.0����:	0 - DISEQC10_OFF, 1 - PORT_4_A,	2 - PORT_4_B,	3 - PORT_4_C,	4 - PORT_4_D,	5 - PORT_2_A,	6 - PORT_2_B
	DWORD m_dwOption					:2;
	DWORD m_dwReserved					:24;
}DiSEqC1020;

typedef struct _DiSEqC1121
{
	DWORD m_dwUncommitted				:4;
	DWORD m_dwCommitted					:4;
	DWORD m_dwRepeatedTimes				:2;		//�ظ�������0 - NoRepeat, 1 - OnceRepeat, 2 - TwiceRepeat
	DWORD m_dwReserved					:22;
}DiSEqC1121;

typedef struct _DiSEqC1121Ext
{
	DWORD m_dwUncommitted				:5;
	DWORD m_dwCommitted					:4;
	DWORD m_dwRepeatedTimes				:2;
	DWORD m_dwReserved					:21;
}DiSEqC1121Ext;

typedef struct _DiSEqC1222
{
	DWORD m_dwUsals						:2;		//1 - ��תλ�ã�λ��ֵȡ��m_dwIndex 2 - ������û����õľ�γ�Ƚ�����Ӧת������
	DWORD m_dwIndex						:5;		//��ת����λ��ֵ��
	DWORD m_dwReserved					:25;
}DiSEqC1222;

typedef struct _LNB
{
	DWORD m_dwLowFrequency				:32;	//low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	DWORD m_dwHighFrequency				:32;	//high LNB frequency in Universal Mod   [unit:KHz]
	DWORD m_dwCenterFrequency			:32;	//middle LNB frequency in Universal Mod  [unit:KHz]
	DWORD m_dwLnbType					:4;		//��Ƶͷ���ͣ�0 - SINGLE, 1 - UNIVERSAL
	DWORD m_dwLnbPower					:2;		//0 - CSHDI_OFF, 1 - CSHDI_ON
	DWORD m_dwLnbVoltage				:2;
	DWORD m_dwReserved					:24;
}LNB;

typedef struct _SatellitePar
{
	DWORD m_dwOrbitalPosition			:16;	//�������ڵľ���λ��
	DWORD m_dwWestEastFlag				:1;		//�������ڵ�λ�õĶ�������־��0 - EASTERN, 1 - WESTERN
	DWORD m_dwReserved					:15;
}SatellitePar;

typedef struct _AntennaPar
{
	DWORD m_dwSwitch22k					:1;		//0 - CSHDI_OFF, 1 - CSHDI_ON
	DWORD m_dwSwith12v					:1;		//0 - SWITCH_0V, 1 - SWITCH_12V
	DWORD m_dwTunerMask					:4;		//0x01 - ����1��tuner��0x02 - ����2��tuner
	DWORD m_dwDisEqCType				:1;
	DWORD m_dwAntennaKey				:8;
	DWORD m_dwSatCrNum_3				:3;
	DWORD m_dwLnbNum_3					:3;
	DWORD m_dwReserved					:11;
}AntennaPar;

typedef struct _CSDTVDMBatInfo_S
{
	UINT	uBouquetID		:16;
	UINT	uVersion		:5;
	UINT	uReserved		:11;
	BYTE	aucBouquetName[32];
}CSDTVDMBatInfo_S;

typedef int (*USPDMCompareUserServiceData) ( const BYTE* pDataLhs, const BYTE* pDataRhs, int nDataLen );
typedef int (*USPDMCompareService) ( HCSHANDLE hServiceLhs, HCSHANDLE hServiceRhs );
typedef int (*USPDMCompareServiceEx) ( HCSHANDLE hServiceLhs, HCSHANDLE hServiceRhs, void* pUserData );
typedef BOOL (*USPDMFilterService) ( HCSHANDLE hService, void* pUserData );


#endif

