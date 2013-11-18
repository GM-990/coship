#ifndef SICommonType_H
#define SICommonType_H

#ifdef PRAGMA
#pragma interface "SICommonType.h"
#endif



// {{{USR
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

//����NIT����������
#define SI_NIT_DEFAULT		0x00	//ֻ����һ��
#define SI_NIT_CABLE		0x01	//ֻ��������
#define SI_NIT_TERRESTRIAL	0x02	//ֻ��������
#define SI_NIT_SATELITE		0x04	//ֻ��������
#define SI_NIT_ALL			0x07	//����ȫ��

#define SI_TAG_MAX			8

#define SI_NIT_TAG		0x01
#define SI_SDT_TAG		0x02
#define SI_PMT_TAG		0x04


#define SI_PAT_TSID_MATCH					(0x00004000)	//��ο�SearchUI.h�еĶ���SEARCH_PAT_TSID_MATCH
#define SI_SDT_TSID_MATCH				(0x00008000)	
#define SI_SDT_ORINETWORKID_MATCH		(0x00010000)

typedef struct _SIDescriptorData
{
	BYTE* m_pData;
	int m_nLength;

}SIDescriptorData;

typedef struct _SIDescriptorListData
{
	BYTE* m_pData;
	int m_nLength;

}SIDescriptorListData;

typedef  SIDescriptorListData* SIDescriptorListHandle;

typedef SIDescriptorData*  SIDescriptorHandle;

#define DES_BUF_LEN 256

typedef struct _SITeletext
{
	char ISO_639_language_code[4];

	unsigned int teletext_type:5;
	unsigned int teletext_magazine_number:3; 
	unsigned int teletext_page_number:8;
	unsigned int PID:13;
	unsigned int nReserved:3;

	struct _SITeletext* next;
} SITeletext;

typedef struct _SISubtitle
{
	char ISO_639_language_code[4];

	unsigned int composition_page_id:16;
	unsigned int ancillary_page_id:16;
	unsigned int subtitling_type:8;
	unsigned int PID:13;
	unsigned int nReserved:11;

	struct _SISubtitle* next;
} SISubtitle;


typedef struct _SICA
{
	WORD m_wCASystemID;
	WORD m_wCAPid;

	struct _SICA* next;
} SICA;

#define LAN_BUF_LEN 4
#define LAN_SORT_MAX 16

typedef struct _SIMpegInfo
{
	BYTE m_bElemType;
	WORD m_wElemPID;
	
	SITeletext* pTeletextHead;
	SISubtitle* pSubtitleHead;
	SICA* pCAHead;

	BYTE aDescriptors[DES_BUF_LEN];
	BYTE bDesLength;

	struct _SIMpegInfo* next;

	char m_caLanguageCode[LAN_SORT_MAX][LAN_BUF_LEN];
	int m_nLanguageCount;
}SIMpegInfo,SIMpegNode;

typedef struct _SITAGINFO
{
	WORD m_wNetWorkID;
	WORD m_wOriNetWorkID;
	WORD m_wTSID;
	BYTE* m_pucTagInfo;
	DWORD m_dwTagLength;

	struct _SITAGINFO* next;
}TTSITagInfo;

typedef struct _SIServiceInfo
{
	BYTE m_bSdtVersion;
	WORD m_wOriginalNetworkId;
	WORD m_wTransprotStreamId;
	WORD m_wServiceId;
	char m_bServiceName[256];
	BYTE m_bServiceType;
	BYTE m_bRunningStatus;
	BYTE m_bFreeCA;
	WORD m_wPcrPid;
	BYTE m_bPmtVersion;
	SIMpegInfo* m_pStreams;
	BYTE m_bPatVersion;
	WORD m_wPmtPID;
	struct _SIServiceInfo* next;

	char m_cProvideName[256];
}SIServiceInfo,SIServiceNode;

typedef struct _SIServiceInfoExt
{
	BYTE m_bSdtVersion;
	WORD m_wOriginalNetworkId;
	WORD m_wTransprotStreamId;
	WORD m_wServiceId;
	char m_bServiceName[256];
	BYTE m_bServiceType;
	BYTE m_bRunningStatus;
	BYTE m_bFreeCA;
	WORD m_wPcrPid;
	BYTE m_bPmtVersion;
	SIMpegInfo* m_pStreams;
	BYTE m_bPatVersion;
	WORD m_wPmtPID;
	struct _SIServiceInfoExt* next;
	char m_cProvideName[256];
	BYTE m_bEitScheduleFlag;
	BYTE m_bEitPFFlag;
	WORD m_wServiceNameLen;
}SIServiceInfoExt, SIServiceExtNode;
void SIServiceInfoInit( SIServiceInfo* pServiceInfo );
void SIServiceInfoExtInit(SIServiceInfoExt* pServiceInfoExt );

typedef struct _SISectionData
{
	BYTE* m_pData;
	int m_nLength;
}SISectionData;
typedef SISectionData* SISectionHandle;

//SDeliverySys�ṹͬCSUSPDMCommonType.h�еĽṹSatelliteDeliver��ȫһ��,ͬcs_tuner.h������Ƶ�㶨�岻һ��!
typedef struct _SDeliverySys
{
	DWORD m_dwFreq;
	DWORD m_dwSymbolRate:24;
	DWORD m_dwFECInner:4;
	DWORD m_dwPolarization:2;
	DWORD m_dwReserved:2;
}SDeliverySys;

//TDeliverSysͬCSUSPDMCommonType.h�еĽṹTerrestrialDeliver��ȫһ��,ͬcs_tuner.h�е���Ƶ��CSTUNTerrestrialDeliver_S����Ҳһ��!
typedef struct _TDeliverySys
{
	DWORD m_dwCentreFrep;
	DWORD m_dwBandwidth:3;
	DWORD m_dwReserve:29;
}TDeliverySys;

//CdeliverySysͬCSUSPDMCommonType.h�еĽṹCableDeliver��ȫһ��,ͬcs_tuner.h�е���Ƶ��CSTUNCableDeliver_S����Ҳһ��!
typedef struct _CDeliverySys
{
	DWORD m_dwFreq;
	DWORD m_dwSymbolRate:24;
	DWORD m_dwModulation:8;
}CDeliverySys;

typedef union _SIDelivery
{
	SDeliverySys    satellite;
	CDeliverySys    cable;
	TDeliverySys	terrestrial;
}SIDelivery;

typedef struct _SINetworkInfo
{
	BYTE m_bType;
	WORD m_wNetworkId;
	BYTE m_bNetworkName;
	WORD m_wOriginalNetworkId;
	WORD m_wTransportStreamId;
	SIDelivery m_delivery;
	struct _SINetworkInfo* next;
}SINetworkInfo,SINetworkNode;

//�ýṹ��ǰ���ڲ��ṹ,���ڷŵ���ߣ�������m_pvReserved��������nit��֮ǰû��ʹ�ã���
typedef struct _SINetworkInfoExt
{
	BYTE m_bType;
	WORD m_wNetworkId;
	BYTE m_bNetworkName;
	WORD m_wOriginalNetworkId;
	WORD m_wTransportStreamId;
	SIDelivery m_delivery;
	struct _SINetworkInfoExt* next;
	BYTE m_bModulation;//����Ӧ�ñ�����SIDelivery�У���������ǰ������⣬SIDelivery��Ҫ������Searchģ�飬�ʱ��������
	void * m_pvReserved;//m_pvReserved��SectionTableData_S�ṹ��ָ��,
}SINetworkInfoExt,SINetworkExtNode;

typedef struct _SITSInfo
{
	BYTE m_bDeliverType;	//����SI_DESCRIPTOR_SATELLITE_DELIVERY
	WORD m_wNetworkId;
	WORD m_wOriginalNetworkId;
	WORD m_wTransportStreamId;
	SIDelivery m_sDeliver;
	HCSHANDLE m_hTP;

	PBYTE m_pucTagData;
	DWORD m_dwTagLength;
}SITSInfo;


typedef enum _NOTIFY_TYPE { COMPLETE, TIMEOUT, ABORT ,VERSION_CHANGED}NOTIFY_TYPE;

typedef struct _SIOtherSdtFlag
{
	DWORD m_dwON_TS;
	BYTE m_bFlag;
}SIOtherSdtFlag;

#define MAX_TS_NUM 256

enum{
 SI_DESCRIPTOR_VIDEO_STREAM = 0x02 ,       
 SI_DESCRIPTOR_AUDIO_STREAM = 0x03 ,       
 SI_DESCRIPTOR_HIERACHY = 0x04 ,       
 SI_DESCRIPTOR_REGISTRATION = 0x05 ,       
 SI_DESCRIPTOR_DATA_STREAM_ALIGNMENT = 0x06 ,       
 SI_DESCRIPTOR_TARGET_BACKGROUND_GRID = 0x07 ,       
 SI_DESCRIPTOR_VIDEO_WINDOW = 0x08 ,       
 SI_DESCRIPTOR_CA = 0x09 ,       
 SI_DESCRIPTOR_LANGUAGE = 0x0A ,       
 SI_DESCRIPTOR_SYSTEM_CLOCK = 0x0B ,       
 SI_DESCRIPTOR_MULTIPLEX_BUFFER_USAGE = 0x0C ,       
 SI_DESCRIPTOR_COPYRIGHT = 0x0D ,       
 SI_DESCRIPTOR_MAXIMUM_BITRATE = 0x0E ,       
 SI_DESCRIPTOR_PRIVATE_DATA_INDICATOR = 0x0F ,       
 SI_DESCRIPTOR_SMOOTHING_BUFFER = 0x10 ,       
 SI_DESCRIPTOR_STD = 0x11 ,       
 SI_DESCRIPTOR_IBP = 0x12 ,       
 SI_DESCRIPTOR_CAROUSEL_IDENTIFIER = 0x13,
 SI_DESCRIPTOR_ASSOCIATION_TAG = 0x14,
 SI_DESCRIPTOR_DERERRED_TAG = 0x15,
 SI_DESCRIPTOR_NETWORK_NAME = 0x40 ,       
 SI_DESCRIPTOR_SERVICE_LIST = 0x41 ,       
 SI_DESCRIPTOR_STUFFING = 0x42 ,       
 SI_DESCRIPTOR_SATELLITE_DELIVERY = 0x43 ,       
 SI_DESCRIPTOR_CABLE_DELIVERY = 0x44 ,       
 SI_DESCRIPTOR_BOUQUET_NAME = 0x47 ,       
 SI_DESCRIPTOR_SERVICE = 0x48 ,       
 SI_DESCRIPTOR_COUNTRY_AVAILABILITY = 0x49 ,       
 SI_DESCRIPTOR_LINKAGE = 0x4A ,       
 SI_DESCRIPTOR_NVOD_REFERENCE = 0x4B ,       
 SI_DESCRIPTOR_TIME_SHIFTED_SERVICE = 0x4C ,       
 SI_DESCRIPTOR_SHORT_EVENT = 0x4D ,       
 SI_DESCRIPTOR_EXTENDED_EVENT = 0x4E ,       
 SI_DESCRIPTOR_TIME_SHIFTED_EVENT = 0x4F ,       
 SI_DESCRIPTOR_COMPONENT = 0x50 ,       
 SI_DESCRIPTOR_MOSAIC = 0x51 ,       
 SI_DESCRIPTOR_STREAM_IDENTIFIER = 0x52 ,       
 SI_DESCRIPTOR_CA_IDENTIFIER = 0x53 ,       
 SI_DESCRIPTOR_CONTENT = 0x54 ,       
 SI_DESCRIPTOR_PARENTAL_RATING = 0x55 ,       
 SI_DESCRIPTOR_TELETEXT = 0x56 ,       
 SI_DESCRIPTOR_TELEPHONE = 0x57 ,       
 SI_DESCRIPTOR_LOCAL_TIME_OFFSET = 0x58 ,       
 SI_DESCRIPTOR_SUBTITLING = 0x59 ,       
 SI_DESCRIPTOR_TERRESTRIAL_DELIVERY = 0x5A ,       
 SI_DESCRIPTOR_MULTILINGUAL_NETWORK_NAME = 0x5B ,       
 SI_DESCRIPTOR_MULTILINGUAL_BOUQUET_NAME = 0x5C ,       
 SI_DESCRIPTOR_MULTILINGUAL_SERVICE_NAME = 0x5D ,       
 SI_DESCRIPTOR_MULTILINGUAL_COMPONENT = 0x5E ,       
 SI_DESCRIPTOR_PRIVATE_DATA_SPECIFIER = 0x5F ,       
 SI_DESCRIPTOR_SERVICE_MOVE = 0x60 ,       
 SI_DESCRIPTOR_SHORT_SMOOTHING_BUFFER = 0x61 ,       
 SI_DESCRIPTOR_FREQUENCY_LIST = 0x62 ,       
 SI_DESCRIPTOR_PARTIAL_TRANSPORT_STREAM = 0x63 ,       
 SI_DESCRIPTOR_DATA_BROADCAST = 0x64 ,       
 SI_DESCRIPTOR_CA_SYSTEM = 0x65 ,       
 SI_DESCRIPTOR_DATA_BROADCAST_ID = 0x66 ,       
 SI_DESCRIPTOR_APPLICATION_SIGNALLING = 0x6f         
};

int CSUSPSIGetVersionInfo( char * pcVersionInfo, int nSize );

BOOL CSUSPSIGetMemInfo ( int * pnMin, int * pnMax );

//�趨�ڽ����ı��ַ�ʱ�Ƿ���˿����ַ���Ĭ��ΪTRUE�������˿����ַ���
void CSUSPSISetFilterControlChar(BOOL bFilter);

#ifdef __cplusplus
}
#endif

// }}}USR

// {{{USR

// }}}USR

#endif /* SICommonType_H */
