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

#define CS_ADSSYS_TYPE_COUNT 32 ///<共有多少种广告位类型

#define CS_ADSSYS_CHANNEL_BAR_BIT (0x01) ///<bit0, channel bar/epg banner的广告位掩码
#define CS_ADSSYS_CHANNEL_BAR_DETAIL_BIT (0x02) ///<bit1, channel bar/epg banner 详情的广告位掩码
#define CS_ADSSYS_OK_LIST_BIT (0x04) ///<bit2, 频道列表的广告位掩码
#define CS_ADSSYS_OK_LIST_DETAIL_BIT (0x08) ///<bit3, 频道列表详情的广告位掩码
#define CS_ADSSYS_EPG211_172_BIT (0x10) ///<bit4, EPG界面小块形广告位掩码，通常放在右边
#define CS_ADSSYS_EPG666_81_BIT (0x20) ///<bit5, EPG界面长条形广告位掩码，通常放在下方
#define CS_ADSSYS_NVOD_BIT (0x40) ///<bit6, NVOD广告位掩码
#define CS_ADSSYS_RADIO_BIT (0x80) ///<bit7, 音频频道背景广告位掩码
#define CS_ADSSYS_POWER_ON_BIT (0x0100) ///<bit8, 开机背景广告位掩码
#define CS_ADSSYS_MAINMENU_VIDEO_BIT (0x0200) ///<bit9, 主菜单小视频窗口区的广告位掩码
#define CS_ADSSYS_MAINMENU_LOG_BIT (0x0400) ///<bit10, 主菜单logo区的广告位掩码
#define CS_ADSSYS_FLOAT_TEXT_BIT (0x0800)  ///<bit11, 走马灯广告位掩码
#define CS_ADSSYS_FLOAT_IMAGE_BIT (0x1000)  ///<bit12, 挂角图片广告位掩码，同挂角图片1
#define CS_ADSSYS_FLOAT_IMAGE1_BIT (0x1000)  ///<bit12, 挂角图片1广告位掩码
#define CS_ADSSYS_FLOAT_IMAGE2_BIT (0x2000)  ///<bit13, 挂角图片2广告位掩码
#define CS_ADSSYS_FLOAT_IMAGE3_BIT (0x4000)  ///<bit14, 挂角图片3广告位掩码
#define CS_ADSSYS_FLOAT_IMAGE4_BIT (0x8000) ///<bit15, 挂角图片4广告位掩码
#define CS_ADSSYS_RECOMMENDATION_BIT (0x10000) ///<bit16, 推荐广告位掩码
#define CS_ADSSYS_SOUND_BAR_BIT (0x20000) ///<bit17,音量条广告位掩码

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


#define CS_ADSSYS_SERVICE_COUNT  64///<一个频道分组最多含有64个频

#define CSADS_APP_NAME_LEN 256

///广告位编号/序号定义
enum _CSADSSYS_IMAGE_TYPE_INDEX_CONST 
{
	CS_ADSSYS_CHANNEL_BAR = 0, ///<channel bar/epg banner
	CS_ADSSYS_CHANNEL_BAR_DETAIL = 1, ///<channel bar / epg banner详情
	CS_ADSSYS_OK_LIST = 2, ///<频道列表
	CS_ADSSYS_OK_LIST_DETAIL = 3, ///<频道列表详情
	CS_ADSSYS_EPG211_172 = 4, ///<EPG界面小块形广告位掩码，通常放在右边
	CS_ADSSYS_EPG666_81 = 5, ///<EPG界面长条形广告位掩码，通常放在下方
	CS_ADSSYS_NVOD = 6, ///<NVOD
	CS_ADSSYS_RADIO = 7, ///<音频频道背景，该位置的数据块内容为IFrame
	CS_ADSSYS_POWER_ON = 8, ///<开机画面，该位置的数据块内容为IFrame
	CS_ADSSYS_MAINMENU_VIDEO = 9, ///<主菜单视频小窗口区，如果是视频广告，则内容由AV PID与频点指定
	CS_ADSSYS_MAINMENU_LOG = 10, ///<主菜单logo位置
	CS_ADSSYS_FLOAT_TEXT = 11, ///<主视频窗口走马灯
	CS_ADSSYS_FLOAT_IMAGE = 12, ///<主视频窗口挂角，同挂角1
	CS_ADSSYS_FLOAT_IMAGE1 = 12, ///<主视频窗口挂角1
	CS_ADSSYS_FLOAT_IMAGE2 = 13, ///<主视频窗口挂角2
	CS_ADSSYS_FLOAT_IMAGE3 = 14, ///<主视频窗口挂角3
	CS_ADSSYS_FLOAT_IMAGE4 = 15, ///<主视频窗口挂角4
	CS_ADSSYS_RECOMMENDATION = 16, ///<推荐广告位，用于频道未授权的提示框
	CS_ADSSYS_SOUND_BAR = 17 ///<音量条广告位
};

///高标清类型，广告终端系统只会接收所指定的高标清类型的数据
typedef enum
{
	EM_ADSSYS_HD_RESOLUTION = 0, ///<高清
	EM_ADSSYS_SD_RESOLUTION ///< 标清
}CSAdsSysResolution_E;

///界面应用类型，一个界面应用可以有多个广告位@see CSAdsSysAppDispTimeoutDscrpt_S
enum _CS_ADSSYS_APP_CONST 
{
	CS_ADSSYS_APP_CHANNEL_BAR = 1, 	///< channel bar界面，含CS_ADSSYS_CHANNEL_BAR与CS_ADSSYS_CHANNEL_BAR_DETAIL
	CS_ADSSYS_APP_CHANNEL_LIST = 2, 	///< 频道列表界面,含CS_ADSSYS_OK_LIST, CS_ADSSYS_OK_LIST_DETAIL
	CS_ADSSYS_APP_FLOAT_IMAGE = 3, 	///< 所有的挂角图片
	CS_ADSSYS_APP_FLOAT_TEXT = 4, 	///< 走马灯文字
	CS_ADSSYS_APP_VOLUME_BAR = 5,   /// 音量条
	CS_ADSSYS_APP_COUNT = 17 			///最大可以支持的节目应用数目
};

///tag 0xa1.描述界面应用超时,达到超时时间，界面自动消失@see _CS_ADSSYS_APP_CONST
typedef struct 
{
	BYTE aucDisplayTimeout[CS_ADSSYS_APP_COUNT];///<界面应用超时时间，0为永远显示，数组序号参见_CS_ADSSYS_APP_CONST
}CSAdsSysAppDispTimeoutDscrpt_S; 

///tag 0xa2,使用位掩码方式描述需要关闭的广告位
typedef struct 
{
	/**
	为1的bit对应的广告位需要关闭。
	@code
	if(dwImgDispLocationMask & (1<<CS_ADSSYS_OK_LIST))
	{
	    关闭CS_ADSSYS_OK_LIST对应的广告位
	}
	@endcode
	*/
	DWORD dwImgDispLocationMask; 
}CSAdsSysInvalidationDscrpt_S; 

///deliver类型
typedef enum
{
	EM_ADSSYS_DELIVER_CABLE = 0, ///<有线
	EM_ADSSYS_DELIVER_TERRESTRIA = 1, ///<地面
	EM_ADSSYS_DELIVER_SATELLITE = 2 ///<卫星
}CSAdsSysDeliverType_E;


///Deliver定义
typedef struct
{
	CSAdsSysDeliverType_E eDeliverType; ///<Deliver类型
	CSUDITunerSRCDeliver_U unDeliver;  //Deliver值
}CSAdsSysDeliver_S; 

///tag 0xb1, 文字属性描述，如运动方向，颜色等
typedef struct 
{
	BYTE ucDisplayDirection; //<0x01: Left to Right; 0x02: Right to Left; 0x03: Up to Down; 0x04: Down to Up
	BYTE ucFlyMode; //< 0x01: flying; 0x02: freeze
	BYTE ucFlyInterval; //< In ms, meaning: every fly interval time to move in flying model and in second in freeze model
	BYTE ucFlyDistance; //< In pixel, meaning: distance in pixel for each move
	BYTE ucFontSize; //< 字体需要显示的大小
	
	BYTE ucForegroundColorA; //<文字体颜色alpha分量
	BYTE ucForegroundColorR; //<文字颜色red分量
	BYTE ucForegroundColorG; //<文字颜色green分量
	BYTE ucForegroundColorB; //<文字颜色blue分量
	
	BYTE ucBackgroundColorA; //<走马灯背景条颜色alpha分量
	BYTE ucBackgroundColorR; //<走马灯背景条颜色red分量
	BYTE ucBackgroundColorG; //<走马灯背景条颜色green分量
	BYTE ucBackgroundColorB; //<走马灯背景条颜色blue分量
}CSAdsSysTextDisplayDscrpt_S; ///走马灯文字显示属性

///tag 0xb2, 广告位所关联的应用的名称 ,参见 <CSDS-AMS广告数据规范>
typedef struct 
{
	WORD wActiveKeyCode; ///<关联应用的激活按键码
	BYTE ucNameLength; ///<关联的应用名称长度
	BYTE pucApplicationName[CSADS_APP_NAME_LEN]; ///<关联的应用名
	DWORD dwMask; ///<关联的广告位掩码
}CSAdsSysActivationDscrpt_S; 

///tag 0xb3, 广告位的位置描述，应用主要关心坐标宽高，如挂角广告,参见 <CSDS-AMS广告数据规范>
typedef struct 
{
	DWORD dwMask; ///<使用该位置的广告位掩码，应用不关心
	WORD wXPos; ///<x坐标
	WORD wYPos; ///<y坐标
	WORD wWidth; ///<宽度
	WORD wHeight; ///<高度
	BYTE *pucBuf;  ///<数据缓存，应用不关心
	BOOL bToBeStored; ///<存储描述
	BYTE ucTranparency; ///<透明度描述
}CSAdsSysLocationDscrpt_S; 

///tag 0xb5 ,视频广告描述
typedef struct
{
	CSAdsSysDeliver_S sDeliver; ///< deliver
	WORD wVideoPid;  ///< 视频PID
	WORD wAudioPid; ///< 音频PID	 
	WORD wPcrPid;	///< PCR PID
	/**
	 视频流编码格式:
	- 0: MPEG-1 Video (ISO/IEC 11172-2) 
	- 1: MPEG-2 Video (ISO/IEC 13818-2) 
	- 2: MPEG-4 Part 2 Video 
	- 3: H.264 (ITU-T) or ISO/IEC 14496-10/MPEG-4 AVC 
	- 4: H.263 Video. The value of the enum is not based on PSI standards.
	*/
	BYTE ucVideoType; 
	/**
	音频流编码格式
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

///tag 0xb7, 触发描述符,持续时间长度见CSAdsSysAppDispTimeoutDscrpt_S
typedef struct
{
	BYTE ucFlag; ///<触发类型,1 - 多少分钟触发一次，2 - 一小时触发多少次
	DWORD dwValue; ///<触发的值
}CSAdsSysTrigerDscrpt_S;

#define CS_ADSSYS_STBID_SIZE (256) ///机顶盒ID的最大字节数
#define MAX_FILE_NAME_LEN (256) //文件最大长度
#define MAX_COMP_IMAGE_DATA_LEN (256)
#define ADS_MASK_SIZE (10)

typedef enum
{
	EM_ADSDATABASEIMP_RADIO,
	EM_ADSDATABASEIMP_START,
	EM_ADSDATABASEIMP_MAX
}CSAdsDataBaseImpPicture_E;//广告图片名字

///电视频道三要素
typedef struct
{
	WORD wOnId; ///< 原始网络ID
	WORD wTsId; ///< TS ID
	WORD wServiceId; ///< Service ID
}CSAdsSysService_S; 

///数据块的基本数据结构，其它的数据块都要包含本数据块
typedef struct _CSAdsSysBlock_S
{
	int nRefCount;
	DWORD dwRefFlag;
	DWORD dwId; ///< block块ID
	BYTE ucTag;
	void *pBlockDataMem; 
	int nBlockSize;
	DWORD dwStored;	
	BYTE *pucDscrpt;
	int nDscrptSize;
	struct _CSAdsSysBlock_S *psBaseBlock;
}CSAdsSysBlock_S;

///tag 0xb4, 广告编排显示信息
typedef struct 
{
	CSUDIPLUSOSSysTime_S sStartDisplayTime; ///< 开始显示时间
	CSUDIPLUSOSSysTime_S sEndDisplayTime; ///< 结束显示时间
	CSAdsSysService_S sService; ///< 关联的频道
	DWORD dwLocationMask;	 ///< 显示的的广告位，以掩码形式表示多个广告位
	CSAdsSysBlock_S *psBlock; ///< 关联的数据块
	
	int nTrigerSpace; ///<触发的间隔秒数，0忽略
	DWORD dwStartSec; ///<相对2000年的秒差值
	DWORD dwEndSec; ///<相对2000年的秒差值

	DWORD dwLastSec; // 激活时间
	BYTE* pucDscrpt; ///<该排期后的描述符的内存起始地址
	int nDscrptLen; ///该排期后到下一个排期描述符前的长度
}CSAdsSysDisplayDscrpt_S;

///tag 0xBA 共享广告的描述信息
typedef struct
{
	DWORD dwGroupId;///<描述共享随机广告中的广告所属的组ID，如果按广告位分组，则与广告位的ID相等，表示属于这个广告位的组
	DWORD dwDetail_ad_id;///<详情广告的唯一id ，如果Detail_ad_id =0 说明该广告位没有详情广告(二级广告)
	DWORD dwServiceGroupId;///<广告所属组的组id
	CSUDIPLUSOSSysTime_S sStartDisplayTime;///<广告开始时间 (DVB UTC[5])
	CSUDIPLUSOSSysTime_S sEndDisplayTime; ///<广告结束时间 (DVB UTC[5])
}CSAdsSysGroupDescriptor_S;

///tag 0xBB 广告的唯一id 
typedef struct
{
    DWORD dwIdentificationid ; ///<广告的唯一id
}CSAdsSysIdentificationDescriptor_S;
	

///tag 0xBC, 频道分组的信息
typedef struct 
{
	DWORD dwGroupId; ///<分组编号
	CSAdsSysService_S asService[CS_ADSSYS_SERVICE_COUNT];///<分组中的节目列表
}CSAdsSysServiceGroupDescriptor_S;

///一个imageblock与它所属的共享广告组
typedef struct
{
   CSAdsSysIdentificationDescriptor_S sIdentificationId;///<广告的唯一id
   CSAdsSysGroupDescriptor_S sGroupDescriptor;///<一个imageblock可能包含的AD_Group_Descriptor
   CSAdsSysBlock_S *psBlock; ///< 关联的数据块

   int nTrigerSpace; ///<触发的间隔秒数，0忽略，暂时未用	
   DWORD dwLastSec; // 激活时间，暂时未用
}CSAdsSysShareDescriptor_S;

///暂时没有使用，某广告位对应的所有共享广告数据块的id，
typedef struct
{
   DWORD dwGroupId;///广告位id，@see_CSADSSYS_IMAGE_TYPE_INDEX_CONST
   DWORD dwServiceGroupId;///节目分组的id
   DWORD adwBlockId[128];///< 数据块ID
   int nValidCount; ///< 需要存储的数据块个数
} CSAdsSysBlockShareId_S;


///广告表中的参数块数据，包含主频点/界面应用超时时间/屏蔽广告位/频道分组等信息
typedef struct
{
	BOOL bExistMainDeliver; ///< 是否存在主频点信息
	CSAdsSysDeliver_S sMainDeliver; ///< 主频点

	BOOL bExistAppDispTimeout; ///< 是否存在界面应用超时时间描述
	CSAdsSysAppDispTimeoutDscrpt_S sAppDispTimeout; ///< 界面应用超时时间

	BOOL bExistInvalidation; ///< 是否存在屏蔽广告位的描述
	CSAdsSysInvalidationDscrpt_S sInvalidation;	///< 需要屏蔽的广告位

	
	BOOL bExistServiceGroup;///是否存在频道分组信息的描述
	int nServiceGroupCount;
	CSAdsSysServiceGroupDescriptor_S *psServiceGroup;///频道分组描述符
	
}CSAdsSysDescriptorLoop_S;

///文字数据块,从广告码流数据tag 0xc1的block中解析出,参见 <CSDS-AMS广告数据规范>
typedef struct
{
	CSAdsSysBlock_S sBlock; ///<数据块基本信息
	
	WORD wTextLength;   ///< 文字长度
	BYTE *pucTextData;	///< 文字内容

	BOOL bExistTextDisplay;    ///< 是否存在Text Display信息描述
	CSAdsSysTextDisplayDscrpt_S sTextDisplay; ///Text Display信息
}CSAdsSysTextBlock_S;

///图片数据块,从广告码流数据tag 0xc2的block中解析出
typedef struct
{
	CSAdsSysBlock_S sBlock; ///< 数据块基本信息
	
	BYTE *pucImageName; ///< 图片名称
	BYTE ucImageNameLength; ///< 图片名称字符串长度

	BYTE *pucImageData;  ///< 图片数据
	UINT uImageDataLength;	///< 图片数据长度
}CSAdsSysImageBlock_S;

///视频数据块,从广告码流数据tag 0xc3的block中解析出
typedef struct
{
	CSAdsSysBlock_S sBlock; ///< 数据块基本信息
	BOOL bExistVideo; ///< 是否存在视频描述
	CSAdsSysVideoDscrpt_S sVideo; ///< 视频描述
}CSAdsSysVideoBlock_S;


///广告系统终端模块的回调事件类型，请转发事件到客户端应用自己的线程做后续处理@see CSAdsSys_RegisterCallback
typedef enum
{
	/**
	获得码流中的广告系统参数块中的描述信息,此后可调用CSAdsSys_GetDescriptorLoopInfo获取信息
	@see CSAdsSys_GetDescriptorLoopInfo
	*/
	EM_ADSSYS_DESCRIPTOR_LOOP_COMPLETE = 0,

	/**
	当前节目的某广告位 内容变化,有效参数:
	dwBlockId - 广告位对应的block id,0为非法,表示block不存在(本数据通常用不着)
	ucTypeIndex - 广告位编号
	ucBlockTag - 广告类型	
	@note 之后可以调用CSAdsSys_TakeXXXBlock系列函数获取具体内容
	*/
	EM_ADSSYS_CURRENT_CONTENT_CHANGE_EVENT = 1,

	/**
	广告数据搜索完毕。
	搜索完毕后，可以调用CSAdsSys_GetStoredBlockIds获取需要存储的数据块
	@see CSAdsSys_GetStoredBlockIds
	*/
	EM_ADSSYS_SCAN_COMPLETE = 2,

	/**
	发现与用户STB匹配的控制信息,有效数据:
	bOpenAds - 该用户是否需要关闭广告,TRUE打开，FALSE关闭	
	@note 用户匹配的STB ID由CSAdsSys_SetStbId设置
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

///广告系统终端模块的事件数据
typedef struct
{
	CSAdsSysEventType_E eEventType;  ///<事件类型
	DWORD dwBlockId; ///<事件参数:相关的BlockId	
	BYTE ucTypeIndex; ///<事件参数:广告位编号@see _CSADS_IMAGE_TYPE_INDEX_CONST
	BYTE ucBlockTag; ///<事件参数:广告位数据类型@see CS_ADSSYS_TAGS
	BOOL bOpenAds; ///<事件参数:TRUE可打开广告，FALSE全部关闭
	DWORD dwOldBlockId; ///<事件参数: 上一次的BlockId
}CSAdsSysEvent_S;

///需要存储的数据块,EM_ADSSYS_SCAN_COMPLETE后可调用@see EM_ADSSYS_SCAN_COMPLETE CSAdsSys_GetStoredBlockIds
typedef struct
{
	DWORD adwBlockId[CS_ADSSYS_TYPE_COUNT];  ///< 数据块ID
	BYTE aucTypeIndex[CS_ADSSYS_TYPE_COUNT]; ///< 广告位
	int nValidCount; ///< 需要存储的数据块个数
}CSAdsSysBlockStoredId_S;



#define CS_ADSSYS_TEXT_BLOCK_TAG 0xc1 ///< 文本广告数据块tag
#define CS_ADSSYS_IMAGE_BLOCK_TAG 0xc2 ///< 图片广告数据块tag
#define CS_ADSSYS_VIDEO_BLOCK_TAG 0xc3 ///< 视频广告数据块tag
///广告数据块tag数组
const static BYTE CS_ADSSYS_TAGS[] = 
{
	CS_ADSSYS_TEXT_BLOCK_TAG, 
	CS_ADSSYS_IMAGE_BLOCK_TAG, 
	CS_ADSSYS_VIDEO_BLOCK_TAG
};
#define CS_ADSSYS_BLOCK_TAG_COUNT (sizeof(CS_ADSSYS_TAGS)) ///< 可显示的广告数据块数量

#define CS_ADSSYS_CONTROL_INFO_BLOCK_TAG 0xc4 ///< 根据STB ID开闭消费者广告的数据块tag
#define CS_ADSSYS_PARAMETER_BLOCK_TAG 0xc5 ///< 广告系统参数数据块tag

///显示参数@see CSAdsSys_TakeTextBlock CSAdsSys_TakeImageBlock CSAdsSys_TakeVideoBlock
typedef struct
{
	CSAdsSysService_S sService;	///< 频道
	CSUDIPLUSOSSysTime_S sTime; ///< 时间
}CSAdsSysDisplay_S;


typedef void (*CSOnAdsSysGetDateTime_F)(BOOL bOffset, CSUDIPLUSOSSysTime_S *psDateTime);


#endif
/**
@}
*/
