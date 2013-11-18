#ifndef _CSUSPDMCOMMENTYPE_H_
#define _CSUSPDMCOMMENTYPE_H_

#include "mmcp_typedef.h"
#include "mmcp_debug.h"

//字符串编码类型
typedef enum _CSUSPDMCodingType
{
	USP_CODING_UNKNOWEN	//未知类型
  , USP_CODING_ASC = 1	//ASC码
  , USP_CODING_UNICODE	//UNICODE码
}CSUSPDMCodingType;

//节目的FAV属性，需用户自定义，码流中没有此属性
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

//节目类型。码流中可以得到。具体含义请参见 ETSI 300 468
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

//创建Iterator排序时的排序字段
typedef enum _CSUSPDMSortKey
{
	USPDM_SORT_INDEX							//按照Service ID 排序
  , USPDM_SORT_NAME								//按照Service Name 排序
}CSUSPDMSortKey;

//创建Iterator时的排序方式
typedef enum _CSUSPDMSortType
{
	USPDM_SORT_ASC								//升序
  , USPDM_SORT_DESC								//降序
}CSUSPDMSortType;

typedef enum _CSUSPDMFLASHBLOCK
{
    USPDM_FLASHBLOCK_32K
   ,USPDM_FLASHBLOCK_64K
   ,USPDM_FLASHBLOCK_128K
   ,USPDM_FLASHBLOCK_256K
   ,USPDM_FLASHBLOCK_512K
}CSUSPDMFlashBlock;

//节目属性
typedef struct _SERVICEPAR
{
	DWORD m_dwLanguage_Code_6			:6;		//节目语言
	DWORD m_dwReservied_2				:1;		//保留
	DWORD m_dwFreeCAMode				:1;		//加绕标志：0：未加扰，1，加绕
	DWORD m_dwAudio_Type_2				:2;		//音频类型//已经废弃
	DWORD m_dwAudio_Vol_6				:6;		//节目音量，用户设定
	DWORD m_dwSkip_1					:1;		//跳过标志，用户设定
	DWORD m_dwDelete_1					:1;		//删除标志，用户设定
	DWORD m_dwLockState_1				:1;		//节目锁标志，用户设定
	DWORD m_dwChannelID_13				:13;	//频道ID，根据需要，从流中或用户自己设定
	DWORD m_dwAudioChannelType			:2;		//左右声道属性，用户设定
	DWORD m_dwAudio_Type_8				:8;		//音频类型，仅供不关注Service属性中UniNum属性的工程使用
	DWORD m_dwReserved					:22;	
}SERVICEPAR;

//节目的PID属性
typedef struct _ServicePID
{
	DWORD m_dwPMTPID_13					:13;	//PMT PID
	DWORD m_dwPCRPID_13					:13;	//PCR PID
	DWORD m_dwSubtitle_PID_Index_6		:6;		//此处无效，通过特定函数接口访问
	DWORD m_dwVideoPID_13:13;					//Video PID
	DWORD m_dwAudioPID_13:13;					//Audio PID
	DWORD m_dwAudio_PID_Index_6:6;				//此处无效，通过特定函数接口访问	
	DWORD m_dwSbutitlePID_13:13;				//无效字段，数据管理内部无此字段
	DWORD m_dwTeletextPID_13:13;				//无效字段，数据管理内部无此字段
	DWORD m_dwTeletext_PID_Index_6		:6;		//此处无效，通过特定函数接口访问	
}ServicePID;

//用于Container的初始化，具体含义参见相关接口
typedef struct _DATACONTAINERPAR
{
	void* pVoid;
	int nLen;
	int nType;
}DATACONTAINERPAR;

//卫星频点数据
typedef struct _SatelliteDeliver
{
	DWORD m_dwFrequency;						//频率，单位MHZ
	DWORD m_dwSymbolRate_24				:24;	//符号率，单位KHZ
	DWORD m_dwFECInner_4				:4;
	DWORD m_dwPolarization_2			:2;		//极化方式,0 - 水平, 1 - 垂直
	DWORD m_dwReserved					:2;
}SatelliteDeliver;

//扩展卫星频点数据
typedef struct _SatelliteDeliverExt
{
	DWORD m_dwFrequency;						//频率，单位MHZ
	DWORD m_dwSymbolRate_24				:24;	//符号率，单位KHZ
	DWORD m_dwFECInner_5				:5;		
	DWORD m_dwPolarization_2			:2;		//极化方式,0 - 水平, 1 - 垂直
	DWORD m_dwPilot_1					:1;		//此值无效，不可做真实pilot值
}SatelliteDeliverExt;

//有线频点
typedef struct _CableDeliver
{
	DWORD m_dwFrequency;						//频率，单位KHZ
	DWORD m_dwSymbolRate_24				:24;	//符号率，单位KHZ
	DWORD m_dwModulation_8				:8;		//调制方式：0 - Not Define,  1 - 16QAM,	2 - 32QAM,	3 - 64QAM, 4 - 128QAM, 5 - 256QAM
}CableDeliver;

//地面频点
typedef struct _TerrestrialDeliver
{
	DWORD m_dwFrequency;						//频率，单位KHZ
	DWORD m_dwBanwith_3					:3;		//波段：0 - 8M, 1 - 7M, 2 - 6M
	DWORD m_dwReserved_29				:29;   
}TerrestrialDeliver;

//频点的流属性
typedef struct _SourceID
{
	WORD m_wNetwork_id;							//频点的网络ID，从流中获取
	WORD m_wOriginal_network_id;				//频点的原始网络ID，从流中获取
	WORD m_wTSID;								//频点的流ID，从流中获取
}SourceID;

//频点偏移
typedef struct _TransponderOffset
{
	DWORD m_dwOffsetFrequency_23		:23;	//频率的偏移值，单位KHZ
	DWORD m_dwOffsetSymbolRate_8		:8;		//符号率的偏移值，单位KHZ
	DWORD m_dwIQ_1						:1;		//0-Normal; 1-Invers
}TransponderOffset;

//频点属性
typedef union _Deliver
{
	CableDeliver		m_CableDeliver;			//见前面定义
	SatelliteDeliver	m_SatelliteDeliver;		//见前面定义
	TerrestrialDeliver	m_TerrestrialDeliver;	//见前面定义
}Deliver;

//扩展频点属性
typedef union _DeliverExt
{
	CableDeliver		m_CableDeliver;
	SatelliteDeliverExt	m_SatelliteDeliverExt;
	TerrestrialDeliver	m_TerrestrialDeliver;
}DeliverExt;

typedef struct _DiSEqC1020
{
	DWORD m_dwToneburst					:2;		// Tone Burst开关: 0 - TONEBURST_NONE, 1 - TONEBURST_A, 2 - TONEBURST_B
	DWORD m_dwCommitted					:4;		// DiSEqC1.0开关:	0 - DISEQC10_OFF, 1 - PORT_4_A,	2 - PORT_4_B,	3 - PORT_4_C,	4 - PORT_4_D,	5 - PORT_2_A,	6 - PORT_2_B
	DWORD m_dwOption					:2;
	DWORD m_dwReserved					:24;
}DiSEqC1020;

typedef struct _DiSEqC1121
{
	DWORD m_dwUncommitted				:4;
	DWORD m_dwCommitted					:4;
	DWORD m_dwRepeatedTimes				:2;		//重复次数：0 - NoRepeat, 1 - OnceRepeat, 2 - TwiceRepeat
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
	DWORD m_dwUsals						:2;		//1 - 需转位置，位置值取自m_dwIndex 2 - 需根据用户设置的经纬度进行相应转锅操作
	DWORD m_dwIndex						:5;		//需转到的位置值。
	DWORD m_dwReserved					:25;
}DiSEqC1222;

typedef struct _LNB
{
	DWORD m_dwLowFrequency				:32;	//low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	DWORD m_dwHighFrequency				:32;	//high LNB frequency in Universal Mod   [unit:KHz]
	DWORD m_dwCenterFrequency			:32;	//middle LNB frequency in Universal Mod  [unit:KHz]
	DWORD m_dwLnbType					:4;		//高频头类型：0 - SINGLE, 1 - UNIVERSAL
	DWORD m_dwLnbPower					:2;		//0 - CSHDI_OFF, 1 - CSHDI_ON
	DWORD m_dwLnbVoltage				:2;
	DWORD m_dwReserved					:24;
}LNB;

typedef struct _SatellitePar
{
	DWORD m_dwOrbitalPosition			:16;	//卫星所在的经度位置
	DWORD m_dwWestEastFlag				:1;		//卫星所在的位置的东西方标志：0 - EASTERN, 1 - WESTERN
	DWORD m_dwReserved					:15;
}SatellitePar;

typedef struct _AntennaPar
{
	DWORD m_dwSwitch22k					:1;		//0 - CSHDI_OFF, 1 - CSHDI_ON
	DWORD m_dwSwith12v					:1;		//0 - SWITCH_0V, 1 - SWITCH_12V
	DWORD m_dwTunerMask					:4;		//0x01 - 代表1号tuner，0x02 - 代表2号tuner
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

