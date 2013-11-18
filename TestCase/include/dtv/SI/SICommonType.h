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

//定义NIT表搜索类型
#define SI_NIT_DEFAULT		0x00	//只搜索一个
#define SI_NIT_CABLE		0x01	//只搜索有线
#define SI_NIT_TERRESTRIAL	0x02	//只搜索地面
#define SI_NIT_SATELITE		0x04	//只搜索卫星
#define SI_NIT_ALL			0x07	//搜索全部

#define SI_TAG_MAX			8

#define SI_NIT_TAG		0x01
#define SI_SDT_TAG		0x02
#define SI_PMT_TAG		0x04


#define SI_PAT_TSID_MATCH					(0x00004000)	//请参考SearchUI.h中的定义SEARCH_PAT_TSID_MATCH
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

//SDeliverySys结构同CSUSPDMCommonType.h中的结构SatelliteDeliver完全一样,同cs_tuner.h中卫星频点定义不一样!
typedef struct _SDeliverySys
{
	DWORD m_dwFreq;
	DWORD m_dwSymbolRate:24;
	DWORD m_dwFECInner:4;
	DWORD m_dwPolarization:2;
	DWORD m_dwReserved:2;
}SDeliverySys;

//TDeliverSys同CSUSPDMCommonType.h中的结构TerrestrialDeliver完全一样,同cs_tuner.h中地面频点CSTUNTerrestrialDeliver_S定义也一样!
typedef struct _TDeliverySys
{
	DWORD m_dwCentreFrep;
	DWORD m_dwBandwidth:3;
	DWORD m_dwReserve:29;
}TDeliverySys;

//CdeliverySys同CSUSPDMCommonType.h中的结构CableDeliver完全一样,同cs_tuner.h中地面频点CSTUNCableDeliver_S定义也一样!
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

//该结构以前是内部结构,现在放到外边，用其中m_pvReserved用来保存nit表（之前没有使用）。
typedef struct _SINetworkInfoExt
{
	BYTE m_bType;
	WORD m_wNetworkId;
	BYTE m_bNetworkName;
	WORD m_wOriginalNetworkId;
	WORD m_wTransportStreamId;
	SIDelivery m_delivery;
	struct _SINetworkInfoExt* next;
	BYTE m_bModulation;//本来应该保存在SIDelivery中，但由于以前设计问题，SIDelivery需要拷贝到Search模块，故保存在这里。
	void * m_pvReserved;//m_pvReserved是SectionTableData_S结构的指针,
}SINetworkInfoExt,SINetworkExtNode;

typedef struct _SITSInfo
{
	BYTE m_bDeliverType;	//类似SI_DESCRIPTOR_SATELLITE_DELIVERY
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

//设定在解析文本字符时是否过滤控制字符。默认为TRUE，即过滤控制字符。
void CSUSPSISetFilterControlChar(BOOL bFilter);

#ifdef __cplusplus
}
#endif

// }}}USR

// {{{USR

// }}}USR

#endif /* SICommonType_H */
