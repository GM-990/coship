/**
@addtogroup cs_adssys
@{
*/
#ifndef _CS_ADSDATA_H_
#define _CS_ADSDATA_H_

#include "mmcp_typedef.h"
#include "mmcp_os.h"
#include "CSUSPCommonType.h"
#include "CSUSPDMCommonType.h"
#include "udi2_tuner.h"

#define CS_ADSSYS_TYPE_COUNT 32 ///<���ж����ֹ��λ����

#define CS_ADSSYS_CHANNEL_BAR_BIT (0x01) ///<bit0, channel bar/epg banner�Ĺ��λ����
#define CS_ADSSYS_CHANNEL_BAR_DETAIL_BIT (0x02) ///<bit1, channel bar/epg banner ����Ĺ��λ����
#define CS_ADSSYS_OK_LIST_BIT (0x04) ///<bit2, Ƶ���б�Ĺ��λ����
#define CS_ADSSYS_OK_LIST_DETAIL_BIT (0x08) ///<bit3, Ƶ���б�����Ĺ��λ����
#define CS_ADSSYS_EPG211_172_BIT (0x10) ///<bit4, EPG����С���ι��λ���룬ͨ�������ұ�
#define CS_ADSSYS_EPG666_81_BIT (0x20) ///<bit5, EPG���泤���ι��λ���룬ͨ�������·�
#define CS_ADSSYS_NVOD_BIT (0x40) ///<bit6, NVOD���λ����
#define CS_ADSSYS_RADIO_BIT (0x80) ///<bit7, ��ƵƵ���������λ����
#define CS_ADSSYS_POWER_ON_BIT (0x0100) ///<bit8, �����������λ����
#define CS_ADSSYS_MAINMENU_VIDEO_BIT (0x0200) ///<bit9, ���˵�С��Ƶ�������Ĺ��λ����
#define CS_ADSSYS_MAINMENU_LOG_BIT (0x0400) ///<bit10, ���˵�logo���Ĺ��λ����
#define CS_ADSSYS_FLOAT_TEXT_BIT (0x0800)  ///<bit11, ����ƹ��λ����
#define CS_ADSSYS_FLOAT_IMAGE_BIT (0x1000)  ///<bit12, �ҽ�ͼƬ���λ���룬ͬ�ҽ�ͼƬ1
#define CS_ADSSYS_FLOAT_IMAGE1_BIT (0x1000)  ///<bit12, �ҽ�ͼƬ1���λ����
#define CS_ADSSYS_FLOAT_IMAGE2_BIT (0x2000)  ///<bit13, �ҽ�ͼƬ2���λ����
#define CS_ADSSYS_FLOAT_IMAGE3_BIT (0x4000)  ///<bit14, �ҽ�ͼƬ3���λ����
#define CS_ADSSYS_FLOAT_IMAGE4_BIT (0x8000) ///<bit15, �ҽ�ͼƬ4���λ����
#define CS_ADSSYS_RECOMMENDATION_BIT (0x10000) ///<bit16, �Ƽ����λ����
#define CS_ADSSYS_SOUND_BAR_BIT (0x20000) ///<bit17,���������λ����

#define CS_ADSSYS_STB_SYSTEM_PATH ("STB.systemPath")
#define CS_ADSSYS_STB_INNER_PATH ("STB.innerPath")
#define CS_ADSSYS_STB_SMARTCARD_ID ("SmartCard.serialNumber")
#define CS_ADSSYS_STB_DEFINITION ("STB.definition")
#define CS_ADSSYS_STB_HIGH_TV ("HD")
#define CS_ADSSYS_STB_STANDARD_TV ("SD")

#define CS_ADSSYS_DIRECTORY ("config")
#define CS_ADSSYS_SUBDIRECTORY ("adsres")
#define CS_ADSSYS_RESOURCE_FILENAME ("resource.properties")
#define CS_ADSSYS_ADS_DATA_FILENAME ("AdsData.properties")
#define CS_ADSSYS_ADS_PRIVATE_DATA_FILENAME ("AdsPrivate.properties")

#define CS_ADSSYS_DEFINITION ("Ads.definition")
#define CS_ADSSYS_PID ("Ads.pid")
#define CS_ADSSYS_TABLEID ("Ads.tableId")
#define CS_ADSSYS_TIMEOUT ("Ads.timeout")
#define CS_ADSSYS_FREQUENCY ("Ads.frequency")
#define CS_ADSSYS_SYMBOLRATE ("Ads.symbolRate")
#define CS_ADSSYS_MODULATION ("Ads.modulation")

#define CS_ADSSYS_TER_FREQUENCY ("Ads.terrestrial.frequency")
#define CS_ADSSYS_TER_BANDWIDTH ("Ads.terrestrial.bandwidth")

#define CS_ADSSYS_SAT_DELIVERY ("Ads.satelliteDelivery")

//#define CS_ADSSYS_MATCH_BG_DEFINITION ("Ads. matchBackgroundDefinition")
#define CS_ADSSYS_STBID ("Ads.stbId")
#define CS_ADSSYS_ADS_SWITCH ("Ads.Switch")
#define CS_ADSSYS_ADS_SCAN_DISABLE_MASK ("Ads.scanDisableMask")
#define CS_ADSSYS_ADS_STORE_DISABLE_MASK ("Ads.storeDisableMask")

#define CS_ADSSYS_STORE_PIC_NAME ("AdsImage")
#define CS_ADSSYS_STORE_START_BG ("STB.startBackgroundFile")
#define CS_ADSSYS_STORE_RADIO_BG ("STB.radioBackgroundFile")


#define CS_ADSSYS_SERVICE_COUNT  64///<һ��Ƶ��������ຬ��64��Ƶ

#define CSADS_APP_NAME_LEN 256

///���λ���/��Ŷ���
enum _CSADSSYS_IMAGE_TYPE_INDEX_CONST 
{
	CS_ADSSYS_CHANNEL_BAR = 0, ///<channel bar/epg banner
	CS_ADSSYS_CHANNEL_BAR_DETAIL = 1, ///<channel bar / epg banner����
	CS_ADSSYS_OK_LIST = 2, ///<Ƶ���б�
	CS_ADSSYS_OK_LIST_DETAIL = 3, ///<Ƶ���б�����
	CS_ADSSYS_EPG211_172 = 4, ///<EPG����С���ι��λ���룬ͨ�������ұ�
	CS_ADSSYS_EPG666_81 = 5, ///<EPG���泤���ι��λ���룬ͨ�������·�
	CS_ADSSYS_NVOD = 6, ///<NVOD
	CS_ADSSYS_RADIO = 7, ///<��ƵƵ����������λ�õ����ݿ�����ΪIFrame
	CS_ADSSYS_POWER_ON = 8, ///<�������棬��λ�õ����ݿ�����ΪIFrame
	CS_ADSSYS_MAINMENU_VIDEO = 9, ///<���˵���ƵС���������������Ƶ��棬��������AV PID��Ƶ��ָ��
	CS_ADSSYS_MAINMENU_LOG = 10, ///<���˵�logoλ��
	CS_ADSSYS_FLOAT_TEXT = 11, ///<����Ƶ���������
	CS_ADSSYS_FLOAT_IMAGE = 12, ///<����Ƶ���ڹҽǣ�ͬ�ҽ�1
	CS_ADSSYS_FLOAT_IMAGE1 = 12, ///<����Ƶ���ڹҽ�1
	CS_ADSSYS_FLOAT_IMAGE2 = 13, ///<����Ƶ���ڹҽ�2
	CS_ADSSYS_FLOAT_IMAGE3 = 14, ///<����Ƶ���ڹҽ�3
	CS_ADSSYS_FLOAT_IMAGE4 = 15, ///<����Ƶ���ڹҽ�4
	CS_ADSSYS_RECOMMENDATION = 16, ///<�Ƽ����λ������Ƶ��δ��Ȩ����ʾ��
	CS_ADSSYS_SOUND_BAR = 17 ///<���������λ
};

///�߱������ͣ�����ն�ϵͳֻ�������ָ���ĸ߱������͵�����
typedef enum
{
	EM_ADSSYS_HD_RESOLUTION = 0, ///<����
	EM_ADSSYS_SD_RESOLUTION ///< ����
}CSAdsSysResolution_E;

///����Ӧ�����ͣ�һ������Ӧ�ÿ����ж�����λ@see CSAdsSysAppDispTimeoutDscrpt_S
enum _CS_ADSSYS_APP_CONST 
{
	CS_ADSSYS_APP_CHANNEL_BAR = 1, 	///< channel bar���棬��CS_ADSSYS_CHANNEL_BAR��CS_ADSSYS_CHANNEL_BAR_DETAIL
	CS_ADSSYS_APP_CHANNEL_LIST = 2, 	///< Ƶ���б����,��CS_ADSSYS_OK_LIST, CS_ADSSYS_OK_LIST_DETAIL
	CS_ADSSYS_APP_FLOAT_IMAGE = 3, 	///< ���еĹҽ�ͼƬ
	CS_ADSSYS_APP_FLOAT_TEXT = 4, 	///< ���������
	CS_ADSSYS_APP_VOLUME_BAR = 5,   /// ������
	CS_ADSSYS_APP_COUNT = 17 			///������֧�ֵĽ�ĿӦ����Ŀ
};

///tag 0xa1.��������Ӧ�ó�ʱ,�ﵽ��ʱʱ�䣬�����Զ���ʧ@see _CS_ADSSYS_APP_CONST
typedef struct 
{
	BYTE aucDisplayTimeout[CS_ADSSYS_APP_COUNT];///<����Ӧ�ó�ʱʱ�䣬0Ϊ��Զ��ʾ��������Ųμ�_CS_ADSSYS_APP_CONST
}CSAdsSysAppDispTimeoutDscrpt_S; 

///tag 0xa2,ʹ��λ���뷽ʽ������Ҫ�رյĹ��λ
typedef struct 
{
	/**
	Ϊ1��bit��Ӧ�Ĺ��λ��Ҫ�رա�
	@code
	if(dwImgDispLocationMask & (1<<CS_ADSSYS_OK_LIST))
	{
	    �ر�CS_ADSSYS_OK_LIST��Ӧ�Ĺ��λ
	}
	@endcode
	*/
	DWORD dwImgDispLocationMask; 
}CSAdsSysInvalidationDscrpt_S; 

///deliver����
typedef enum
{
	EM_ADSSYS_DELIVER_CABLE = 0, ///<����
	EM_ADSSYS_DELIVER_TERRESTRIA = 1, ///<����
	EM_ADSSYS_DELIVER_SATELLITE = 2 ///<����
}CSAdsSysDeliverType_E;


///Deliver����
typedef struct
{
	CSAdsSysDeliverType_E eDeliverType; ///<Deliver����
	CSUDITunerSRCDeliver_U unDeliver;  //Deliverֵ
}CSAdsSysDeliver_S; 

///tag 0xb1, �����������������˶�������ɫ��
typedef struct 
{
	BYTE ucDisplayDirection; //<0x01: Left to Right; 0x02: Right to Left; 0x03: Up to Down; 0x04: Down to Up
	BYTE ucFlyMode; //< 0x01: flying; 0x02: freeze
	BYTE ucFlyInterval; //< In ms, meaning: every fly interval time to move in flying model and in second in freeze model
	BYTE ucFlyDistance; //< In pixel, meaning: distance in pixel for each move
	BYTE ucFontSize; //< ������Ҫ��ʾ�Ĵ�С
	
	BYTE ucForegroundColorA; //<��������ɫalpha����
	BYTE ucForegroundColorR; //<������ɫred����
	BYTE ucForegroundColorG; //<������ɫgreen����
	BYTE ucForegroundColorB; //<������ɫblue����
	
	BYTE ucBackgroundColorA; //<����Ʊ�������ɫalpha����
	BYTE ucBackgroundColorR; //<����Ʊ�������ɫred����
	BYTE ucBackgroundColorG; //<����Ʊ�������ɫgreen����
	BYTE ucBackgroundColorB; //<����Ʊ�������ɫblue����
}CSAdsSysTextDisplayDscrpt_S; ///�����������ʾ����

///tag 0xb2, ���λ��������Ӧ�õ����� ,�μ� <CSDS-AMS������ݹ淶>
typedef struct 
{
	WORD wActiveKeyCode; ///<����Ӧ�õļ������
	BYTE ucNameLength; ///<������Ӧ�����Ƴ���
	BYTE pucApplicationName[CSADS_APP_NAME_LEN]; ///<������Ӧ����
	DWORD dwMask; ///<�����Ĺ��λ����
}CSAdsSysActivationDscrpt_S; 

///tag 0xb3, ���λ��λ��������Ӧ����Ҫ���������ߣ���ҽǹ��,�μ� <CSDS-AMS������ݹ淶>
typedef struct 
{
	DWORD dwMask; ///<ʹ�ø�λ�õĹ��λ���룬Ӧ�ò�����
	WORD wXPos; ///<x����
	WORD wYPos; ///<y����
	WORD wWidth; ///<���
	WORD wHeight; ///<�߶�
	BYTE *pucBuf;  ///<���ݻ��棬Ӧ�ò�����
	BOOL bToBeStored; ///<�洢����
	BYTE ucTranparency; ///<͸��������
}CSAdsSysLocationDscrpt_S; 

///tag 0xb5 ,��Ƶ�������
typedef struct
{
	CSAdsSysDeliver_S sDeliver; ///< deliver
	WORD wVideoPid;  ///< ��ƵPID
	WORD wAudioPid; ///< ��ƵPID	 
	WORD wPcrPid;	///< PCR PID
	/**
	 ��Ƶ�������ʽ:
	- 0: MPEG-1 Video (ISO/IEC 11172-2) 
	- 1: MPEG-2 Video (ISO/IEC 13818-2) 
	- 2: MPEG-4 Part 2 Video 
	- 3: H.264 (ITU-T) or ISO/IEC 14496-10/MPEG-4 AVC 
	- 4: H.263 Video. The value of the enum is not based on PSI standards.
	*/
	BYTE ucVideoType; 
	/**
	��Ƶ�������ʽ
	- 0: Dolby Digital AC3 audio 
	- 1: DTS
	- 2: MPEG1
	- 3: MPEG2
	- 4: CDDA
	- 5: PCM			
	- 6: LPCM			
  	- 7: MP3
  	- 8: AAC(Advanced audio coding. Part of MPEG-4 )  
  	*/
	BYTE ucAudioType; 
}CSAdsSysVideoDscrpt_S; 

///tag 0xb7, ����������,����ʱ�䳤�ȼ�CSAdsSysAppDispTimeoutDscrpt_S
typedef struct
{
	BYTE ucFlag; ///<��������,1 - ���ٷ��Ӵ���һ�Σ�2 - һСʱ�������ٴ�
	DWORD dwValue; ///<������ֵ
}CSAdsSysTrigerDscrpt_S;

#define CS_ADSSYS_STBID_SIZE (256) ///������ID������ֽ���
#define MAX_FILE_NAME_LEN (256) //�ļ���󳤶�
#define MAX_COMP_IMAGE_DATA_LEN (256)
#define ADS_MASK_SIZE (10)

typedef enum
{
	EM_ADSDATABASEIMP_RADIO,
	EM_ADSDATABASEIMP_START,
	EM_ADSDATABASEIMP_MAX
}CSAdsDataBaseImpPicture_E;//���ͼƬ����

///����Ƶ����Ҫ��
typedef struct
{
	WORD wOnId; ///< ԭʼ����ID
	WORD wTsId; ///< TS ID
	WORD wServiceId; ///< Service ID
}CSAdsSysService_S; 

///���ݿ�Ļ������ݽṹ�����������ݿ鶼Ҫ���������ݿ�
typedef struct _CSAdsSysBlock_S
{
	int nRefCount;
	DWORD dwRefFlag;
	DWORD dwId; ///< block��ID
	BYTE ucTag;
	void *pBlockDataMem; 
	int nBlockSize;
	DWORD dwStored;	
	BYTE *pucDscrpt;
	int nDscrptSize;
	struct _CSAdsSysBlock_S *psBaseBlock;
}CSAdsSysBlock_S;

///tag 0xb4, ��������ʾ��Ϣ
typedef struct 
{
	CSUDIPLUSOSSysTime_S sStartDisplayTime; ///< ��ʼ��ʾʱ��
	CSUDIPLUSOSSysTime_S sEndDisplayTime; ///< ������ʾʱ��
	CSAdsSysService_S sService; ///< ������Ƶ��
	DWORD dwLocationMask;	 ///< ��ʾ�ĵĹ��λ����������ʽ��ʾ������λ
	CSAdsSysBlock_S *psBlock; ///< ���������ݿ�
	
	int nTrigerSpace; ///<�����ļ��������0����
	DWORD dwStartSec; ///<���2000������ֵ
	DWORD dwEndSec; ///<���2000������ֵ

	DWORD dwLastSec; // ����ʱ��
	BYTE* pucDscrpt; ///<�����ں�����������ڴ���ʼ��ַ
	int nDscrptLen; ///�����ں���һ������������ǰ�ĳ���
}CSAdsSysDisplayDscrpt_S;

///tag 0xBA �������������Ϣ
typedef struct
{
	DWORD dwGroupId;///<���������������еĹ����������ID����������λ���飬������λ��ID��ȣ���ʾ����������λ����
	DWORD dwDetail_ad_id;///<�������Ψһid �����Detail_ad_id =0 ˵���ù��λû��������(�������)
	DWORD dwServiceGroupId;///<������������id
	CSUDIPLUSOSSysTime_S sStartDisplayTime;///<��濪ʼʱ�� (DVB UTC[5])
	CSUDIPLUSOSSysTime_S sEndDisplayTime; ///<������ʱ�� (DVB UTC[5])
}CSAdsSysGroupDescriptor_S;

///tag 0xBB ����Ψһid 
typedef struct
{
    DWORD dwIdentificationid ; ///<����Ψһid
}CSAdsSysIdentificationDescriptor_S;
	

///tag 0xBC, Ƶ���������Ϣ
typedef struct 
{
	DWORD dwGroupId; ///<������
	CSAdsSysService_S asService[CS_ADSSYS_SERVICE_COUNT];///<�����еĽ�Ŀ�б�
}CSAdsSysServiceGroupDescriptor_S;

///һ��imageblock���������Ĺ�������
typedef struct
{
   CSAdsSysIdentificationDescriptor_S sIdentificationId;///<����Ψһid
   CSAdsSysGroupDescriptor_S sGroupDescriptor;///<һ��imageblock���ܰ�����AD_Group_Descriptor
   CSAdsSysBlock_S *psBlock; ///< ���������ݿ�

   int nTrigerSpace; ///<�����ļ��������0���ԣ���ʱδ��	
   DWORD dwLastSec; // ����ʱ�䣬��ʱδ��
}CSAdsSysShareDescriptor_S;

///��ʱû��ʹ�ã�ĳ���λ��Ӧ�����й��������ݿ��id��
typedef struct
{
   DWORD dwGroupId;///���λid��@see_CSADSSYS_IMAGE_TYPE_INDEX_CONST
   DWORD dwServiceGroupId;///��Ŀ�����id
   DWORD adwBlockId[128];///< ���ݿ�ID
   int nValidCount; ///< ��Ҫ�洢�����ݿ����
} CSAdsSysBlockShareId_S;


///�����еĲ��������ݣ�������Ƶ��/����Ӧ�ó�ʱʱ��/���ι��λ/Ƶ���������Ϣ
typedef struct
{
	BOOL bExistMainDeliver; ///< �Ƿ������Ƶ����Ϣ
	CSAdsSysDeliver_S sMainDeliver; ///< ��Ƶ��

	BOOL bExistAppDispTimeout; ///< �Ƿ���ڽ���Ӧ�ó�ʱʱ������
	CSAdsSysAppDispTimeoutDscrpt_S sAppDispTimeout; ///< ����Ӧ�ó�ʱʱ��

	BOOL bExistInvalidation; ///< �Ƿ�������ι��λ������
	CSAdsSysInvalidationDscrpt_S sInvalidation;	///< ��Ҫ���εĹ��λ

	
	BOOL bExistServiceGroup;///�Ƿ����Ƶ��������Ϣ������
	int nServiceGroupCount;
	CSAdsSysServiceGroupDescriptor_S *psServiceGroup;///Ƶ������������
	
}CSAdsSysDescriptorLoop_S;

///�������ݿ�,�ӹ����������tag 0xc1��block�н�����,�μ� <CSDS-AMS������ݹ淶>
typedef struct
{
	CSAdsSysBlock_S sBlock; ///<���ݿ������Ϣ
	
	WORD wTextLength;   ///< ���ֳ���
	BYTE *pucTextData;	///< ��������

	BOOL bExistTextDisplay;    ///< �Ƿ����Text Display��Ϣ����
	CSAdsSysTextDisplayDscrpt_S sTextDisplay; ///Text Display��Ϣ
}CSAdsSysTextBlock_S;

///ͼƬ���ݿ�,�ӹ����������tag 0xc2��block�н�����
typedef struct
{
	CSAdsSysBlock_S sBlock; ///< ���ݿ������Ϣ
	
	BYTE *pucImageName; ///< ͼƬ����
	BYTE ucImageNameLength; ///< ͼƬ�����ַ�������

	BYTE *pucImageData;  ///< ͼƬ����
	UINT uImageDataLength;	///< ͼƬ���ݳ���
}CSAdsSysImageBlock_S;

///��Ƶ���ݿ�,�ӹ����������tag 0xc3��block�н�����
typedef struct
{
	CSAdsSysBlock_S sBlock; ///< ���ݿ������Ϣ
	BOOL bExistVideo; ///< �Ƿ������Ƶ����
	CSAdsSysVideoDscrpt_S sVideo; ///< ��Ƶ����
}CSAdsSysVideoBlock_S;


///���ϵͳ�ն�ģ��Ļص��¼����ͣ���ת���¼����ͻ���Ӧ���Լ����߳�����������@see CSAdsSys_RegisterCallback
typedef enum
{
	/**
	��������еĹ��ϵͳ�������е�������Ϣ,�˺�ɵ���CSAdsSys_GetDescriptorLoopInfo��ȡ��Ϣ
	@see CSAdsSys_GetDescriptorLoopInfo
	*/
	EM_ADSSYS_DESCRIPTOR_LOOP_COMPLETE = 0,

	/**
	��ǰ��Ŀ��ĳ���λ ���ݱ仯,��Ч����:
	dwBlockId - ���λ��Ӧ��block id,0Ϊ�Ƿ�,��ʾblock������(������ͨ���ò���)
	ucTypeIndex - ���λ���
	ucBlockTag - �������	
	@note ֮����Ե���CSAdsSys_TakeXXXBlockϵ�к�����ȡ��������
	*/
	EM_ADSSYS_CURRENT_CONTENT_CHANGE_EVENT = 1,

	/**
	�������������ϡ�
	������Ϻ󣬿��Ե���CSAdsSys_GetStoredBlockIds��ȡ��Ҫ�洢�����ݿ�
	@see CSAdsSys_GetStoredBlockIds
	*/
	EM_ADSSYS_SCAN_COMPLETE = 2,

	/**
	�������û�STBƥ��Ŀ�����Ϣ,��Ч����:
	bOpenAds - ���û��Ƿ���Ҫ�رչ��,TRUE�򿪣�FALSE�ر�	
	@note �û�ƥ���STB ID��CSAdsSys_SetStbId����
	@see CSAdsSys_SetStbId
	@code
	case EM_ADSSYS_FIND_STB_CTRL_INFO	:
	{
		USPPrint(ADS_DEMO_LEVEL, "find stb id, open=%d\n", sEvent.bOpenAds);
	}
	break;	
	@endcode
	*/
	EM_ADSSYS_FIND_STB_CTRL_INFO = 3
}CSAdsSysEventType_E;

///���ϵͳ�ն�ģ����¼�����
typedef struct
{
	CSAdsSysEventType_E eEventType;  ///<�¼�����
	DWORD dwBlockId; ///<�¼�����:��ص�BlockId	
	BYTE ucTypeIndex; ///<�¼�����:���λ���@see _CSADS_IMAGE_TYPE_INDEX_CONST
	BYTE ucBlockTag; ///<�¼�����:���λ��������@see CS_ADSSYS_TAGS
	BOOL bOpenAds; ///<�¼�����:TRUE�ɴ򿪹�棬FALSEȫ���ر�
	DWORD dwOldBlockId; ///<�¼�����: ��һ�ε�BlockId
}CSAdsSysEvent_S;

///��Ҫ�洢�����ݿ�,EM_ADSSYS_SCAN_COMPLETE��ɵ���@see EM_ADSSYS_SCAN_COMPLETE CSAdsSys_GetStoredBlockIds
typedef struct
{
	DWORD adwBlockId[CS_ADSSYS_TYPE_COUNT];  ///< ���ݿ�ID
	BYTE aucTypeIndex[CS_ADSSYS_TYPE_COUNT]; ///< ���λ
	int nValidCount; ///< ��Ҫ�洢�����ݿ����
}CSAdsSysBlockStoredId_S;



#define CS_ADSSYS_TEXT_BLOCK_TAG 0xc1 ///< �ı�������ݿ�tag
#define CS_ADSSYS_IMAGE_BLOCK_TAG 0xc2 ///< ͼƬ������ݿ�tag
#define CS_ADSSYS_VIDEO_BLOCK_TAG 0xc3 ///< ��Ƶ������ݿ�tag
///������ݿ�tag����
const static BYTE CS_ADSSYS_TAGS[] = 
{
	CS_ADSSYS_TEXT_BLOCK_TAG, 
	CS_ADSSYS_IMAGE_BLOCK_TAG, 
	CS_ADSSYS_VIDEO_BLOCK_TAG
};
#define CS_ADSSYS_BLOCK_TAG_COUNT (sizeof(CS_ADSSYS_TAGS)) ///< ����ʾ�Ĺ�����ݿ�����

#define CS_ADSSYS_CONTROL_INFO_BLOCK_TAG 0xc4 ///< ����STB ID���������߹������ݿ�tag
#define CS_ADSSYS_PARAMETER_BLOCK_TAG 0xc5 ///< ���ϵͳ�������ݿ�tag

///��ʾ����@see CSAdsSys_TakeTextBlock CSAdsSys_TakeImageBlock CSAdsSys_TakeVideoBlock
typedef struct
{
	CSAdsSysService_S sService;	///< Ƶ��
	CSUDIPLUSOSSysTime_S sTime; ///< ʱ��
}CSAdsSysDisplay_S;


typedef void (*CSOnAdsSysGetDateTime_F)(BOOL bOffset, CSUDIPLUSOSSysTime_S *psDateTime);


#endif
/**
@}
*/
