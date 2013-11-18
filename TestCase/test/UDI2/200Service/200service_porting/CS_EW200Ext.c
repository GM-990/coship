/*

本文件主要实现EW200需要应用需要实现的接口

Autor:sunpengqiang.

*/
#include <string.h>
#include <stdio.h>
#include "CSEWVod.h"
#include "../include/porting/udi2/udi2_EW200Service.h"
#include "udi2_video.h"
#include "udi2_audio.h"
#include "udi2_error.h"
#include "udi2_screen.h"
#include "udi2_player.h"
#include "udiplus_typedef.h"
#include "udiplus_debug.h"
#include "udi2_os.h"
#include "udi2_public.h"
#include "udi2_demux.h"


#include "SectionInterface2C.h"
#include "SourceInterface.h"
#include "CSUSPDMCreateIterate.h"
#include "SectionInterface2C.h"

#include "CSUSPDMCommonType.h"
#include "CSUSPDMParseData.h"


#define TEST_UDI_EW200SERVICE TEST_UDI_EW200SERVICE
#define ERROR_LEVEL 3

#define ENABLE 1
#define DISABLE 0
#define PAT_PID 0x0000
#define PAT_TABLE_ID 0x00
#define PMT_TABLE_ID 0x02
#define MAX_AUDIO_NUM 3

static int g_nVolume =0;
static int g_nMute = 0;
static CSUDIVIDEOStopMode_E g_nStopMode = 0;


#define    BLOCK                (0x10000)
#define    KBYTE                (1024)
#define    MBYTE                (0x100000)


typedef struct
{
	WORD 	aud;
	int 	audiotype; 
	char 	language[3];
}PROGRAMAUDIOINFO;

typedef struct
{
	WORD 	ServiceID;
	WORD 	videoID;
	int 	Videotype; 
	PROGRAMAUDIOINFO audio[MAX_AUDIO_NUM];
	int 	nAudioNum;
	WORD pcr;
}VODPROGRAMINFO;

typedef  struct
{
	DWORD udwMsg;
	WORD udwlParam;
	WORD udwwParam;
}EW200MSGINFO;

#define MPEG1_VIDEO_STREAM 0x01
#define MPEG2_VIDEO_STREAM 0x02
#define MPEG1_AUDIO_STREAM 0x03
#define MPEG2_AUDIO_STREAM 0x04
#define H264_VIDEO_STREAM  0x1b
#define MPEG4_STREAM_VIDEO  0x10
#define MPEG21_STREAM_VIDEO 0x1e

#define AC3_AUDIO_STREAM 0X81
#define AAC_AUDIO_STREAM   0x0f
#define AC3_6_AUDIO_STREAM  0x06


#define WM_SVS_MSG  0x1400
#define MSG_EW200_START_PAT	(WM_SVS_MSG + 170)//EW200请求PAT、并且PAT回调处理
#define MSG_EW200_START_PMT	(WM_SVS_MSG + 171)//EW200请求PMT、并且PMT回调处理
#define MSG_EW200_START_PLAY_VOD (WM_SVS_MSG + 172)//开始播放VOD


typedef struct _EW200CAllBack
{
    CSUDIEW200SCallback_F fnCallback;
    DWORD dwUserData;
}EW200CAllBack;


#define INVALID_SECTION_HANDLE NULL
#define INVALID_PID 0x1fff
#define MAX_REQUST_PAT_PMT_COUNT 6

//获取QamDomainInfo
static CSUDI_HANDLE g_hSemIpQamDomainInfo = 0;
static int g_nTunerRetryCount;
static BOOL g_nDomainInfoLength = 0;
static BYTE g_aucIpQamDomainInfo[16];
static BOOL g_bWorking = FALSE;
static WORD g_wPid;
static BYTE g_ucTableId;
static char g_chUserID[32] = {0};
static char g_chToken[32] = {0};
static BOOL g_BInitEWExt = FALSE;

static CSUDI_HANDLE g_hTask = NULL;
static CSUDI_HANDLE g_hMsgQueue = NULL;
static CSUDI_HANDLE g_hMutex = NULL;

static CSUDI_HANDLE g_hPlayer = NULL;

static HCSHANDLE g_PAThandle = (DWORD)INVALID_SECTION_HANDLE;
static HCSHANDLE g_PMThandle = (DWORD)INVALID_SECTION_HANDLE;
static EW200CAllBack g_fnEW200Register={0};
static VODPROGRAMINFO g_program;
static CSUDI_HANDLE g_hSemVodCallback = (DWORD)NULL;
static int g_nReqPatCount = 0;

extern void doSearch_DTV(void);
extern HCSHANDLE getDMContainer();
extern  HCSHANDLE USPHANDLE_SOURCE(void);


/***********************/
static void CSQamDomainParseInfo(BYTE *data, int length);
static void CSQamDomainNotifyInfo(SectionEventType event, Section* pSection,void* pUserData, HCSHANDLE hRequest);
static void CSQamDomain_Callback( CSSourceCallbackType nSourceCallbackType, SourceCallbackMessage *pSourceCallbackMessage );



static void SourceCallback( CSSourceCallbackType nSourceCallbackType, SourceCallbackMessage *pSourceCallbackMessage );
static int CSGTVODStartPAT(void);
static void CSGTVODPatNotify(SectionEventType event, Section *pSection, void *pUserData,HCSHANDLE hRequest);
static void CSGTVODPmtNotify(SectionEventType event, Section *pSection, void *pUserData, HCSHANDLE hRequest);
static int CSParsePat(BYTE *data, WORD ServiceID);
static int CSParsePmt(BYTE *data);
static void CSCancelPatSection(void);
static void CSCancelPMTSection(void);
static void CSStartplayVOD();
static void CSEW200PlayVODThread(void);
static void CSGTVODStartPMT(WORD ServiceID, WORD PMTPID);

void CSEW200_Init(void);

/***********************/
/**
@brief 获取终端信息接口

Eastwin200将通过该接口获取终端的序列号,智能卡号等信息,或获取网页中设置的信息
@param[in] eType 信息类型
@param[in] value eType类型对应的值
@param[in] length value值的长度
@return 返回实际信息的长度,出错返回-1
@note 需要设置的信息见CSUDIEW200SClientInfoType_E枚举中的[r]标识
@note 支持Coship对象、Enreach对象时需要实现该接口

@code
//参考实现(未考虑缓冲区溢出等异常)
//注意:以下两种情况，value表示的类型及含义略有不同，请按参考代码实现/使用即可。
if(eType == EM_UDIEW200S_AUDIOCHANNEL)
{
	//EM_UDIEW200S_AUDIOCHANNEL对应数据类型是int的
	//value为int的变量，其长度应该为sizeof(int)
	
	value = EM_UDIEW200S_AUD_CHANNEL_STEREO;
	CSUDIEW200SReadClientInfo(eType , (void*)&value, sizeof(int));

}
else if(eType == EM_UDIEW200S_SN)
{
	//EM_UDIEW200S_SN对应数据类型是CSUDI_UINT8数组
	//value一个指向CSUDI_UINT8数组，其长度为length
	//从value指向的buffer中获取对应内容
	value[32];
	CSUDIEW200SReadClientInfo(eType , (void*)value, sizeof(value));

}
@endcode
*/

int CSUDIEW200SReadClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length)
{
    int nLength = 0;
    if((eType <EM_UDIEW200S_CARDID) || (eType >EM_UDIEW200S_PROGRAMEINFO))
    {
        CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] eType is out of defined type \r\n");
        CSASSERT((eType >=EM_UDIEW200S_CARDID) && (eType <=EM_UDIEW200S_PROGRAMEINFO));
	return -1;
    }
    if(value == NULL || length <=0)
    {
        CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] value is null or length is invalid \r\n");
        CSASSERT((value != NULL)&&(length >0));
	return -1;
    }
    switch(eType)
    {
    case EM_UDIEW200S_CARDID:          ///< [r]智能卡卡号,char*
        nLength = strlen("710092200010");
        if(length>=nLength)
        {
            strncpy(value,"710092200010",nLength);
        }
        break;
	case EM_UDIEW200S_SN:              ///< [r]序列号,char*
	    nLength = strlen("710092200010");
	    if(length>=nLength)
        {
            strncpy(value,"710092200010",nLength);
        }
        break;
	case EM_UDIEW200S_PROVIDER:        ///< [r]终端制造厂商,char*
	    nLength = strlen("coship");
        if(length>=nLength)
        {
            strncpy(value,"coship",nLength);
        }
        break;
	case EM_UDIEW200S_ALIAS:           ///< [r]终端别名,char*
        nLength = strlen("710092200010");
	    if(length>=nLength)
        {
            strncpy(value,"710092200010",nLength);
        }
        break;
    case EM_UDIEW200S_MODULE:          ///< [r]终端型号,char*
        nLength = strlen("N5200");
	    if(length>=nLength)
        {
            strncpy(value,"N5200",nLength);
        }
        break;

    case EM_UDIEW200S_LOCAL_IP:        ///< [r]本机IP地址,char*,读取时格式为"192.168.1.1"	
        nLength = strlen("192.168.1.101");
	    if(length>=nLength)
        {
            strncpy(value,"192.168.1.101",nLength);
        }
        break;

    case EM_UDIEW200S_LOCAL_MAC:       ///< [r]本机Mac地址,char*,如"00AABBCCDDEE"
         nLength = strlen("00AABBCCDDEE");
	    if(length>=nLength)
        {
            strncpy(value,"00AABBCCDDEE",nLength);
        }
        break;

    case EM_UDIEW200S_CITV_PSD:        ///< [r]CITV VOD用户密码,char*
        nLength = strlen("111111");
	    if(length>=nLength)
        {
            strncpy(value,"111111",nLength);
        }
        break;

    case EM_UDIEW200S_CITV_USERID:     ///< [w][r]CITV VOD用户号,第一次获取传全0,通过认证后CITV3会反馈该值,请将其存储在ROM中,以备下次使用,char*
        nLength = strlen(g_chUserID);
        if(length>=nLength)
        {
            strncpy(value,g_chUserID,nLength);
        }      
        break;

    case EM_UDIEW200S_CITV_TOKEN:      ///< [w][r]CITV 令牌,第一次获取传全0,通过认证后CITV3会反馈该值,请将其存储在ROM中,以备下次使用,char*
        nLength = strlen(g_chToken);
        if(length>=nLength)
        {
            strncpy(value,g_chToken,nLength);
        }      
        break;

    case EM_UDIEW200S_CITV_SERVERIP:   ///< [r]CITV 认证服务器IP地址,char*
        nLength = strlen("172.30.42.1");
	    if(length>=nLength)
        {
            strncpy(value,"172.30.42.1",nLength);
        }
        break;

    case EM_UDIEW200S_CITV_SERVERPORT: ///< [r]CITV 认证服务器端口号,int,如8080
        {
            int port = 8080;
            CSASSERT(length >= sizeof(int));
            memcpy(value,&port,sizeof(int));
            nLength = sizeof(int);
        }
        break;

    case EM_UDIEW200S_CITV_VIDEOTYPE:  ///< [r]支持音视频解码格式MPEG-2标清:"MPEG-2-SD" ;MPEG-2高清:"MPEG-2-HD";H.264标清: "H.264-SD";H.264高清: "H.264-HD" ; 缺省为MPEG-2-SD;同一格式高清向下兼容标清，可以不用再声明;终端可解码格式，可运行多个，用“/”分隔。
        nLength = strlen("MPEG-2-SD");
        if(length>=nLength)
        {
            strncpy(value,"MPEG-2-SD",nLength);
        }        
        break;

    case EM_UDIEW200S_CITV_BROWSERVER: ///< [r]浏览器的版本号，应用根据需求与前端协商
	 nLength = strlen("coship1.0");
        if(length>=nLength)
        {
            strncpy(value,"coship1.0",nLength);
        }        
		break;

    case EM_UDIEW200S_ISMUTE:          ///< [r][w]静音状态,CSUDI_BOOL,CSUDI_TRUE表示当前为静音状态,CSUDI_FALSE表示为非静音状态
        {       
            memcpy(value,&g_nMute,sizeof(int));
            nLength = sizeof(int);
        }
        break;

    case EM_UDIEW200S_VOLUME:          ///< [r][w]音量,int,[0,31],0为无声,31最大
        {	
            memcpy(value,&g_nVolume,sizeof(int));
            nLength = sizeof(int);
        }
        break;

    case EM_UDIEW200S_AUDIOPID:        ///< [r][w]音频pid,int,
    	{
         memcpy(value,&g_program.audio[0].aud,sizeof(WORD));
         nLength = sizeof(int);
    	}
        break;
    case EM_UDIEW200S_MATCHMETHOD:     ///< [r][w]窗口的pan scan等格式,CSUDIEW200SMatchMethod_E
        {
	 CSUDIVIDEOMatchMethod_E eMatchmethod;
	 CSUDIVIDEOGetMatchMethod (0,0, &eMatchmethod);
         memcpy(value,&eMatchmethod,sizeof(WORD));
         nLength = sizeof(int);
    	}  
        break;

    case EM_UDIEW200S_STOPMODE:        ///< [r][w]换台时视频的处理类型:CSUDIEW200SStopMode_E
        {
	    if(CSUDI_SUCCESS == CSUDIVIDEOGetStopMode (0, &g_nStopMode))
	    	{
	    		memcpy(value,&g_nStopMode,sizeof(int));
            		nLength = sizeof(int); 
	    	}                       
        }
        break;

    case EM_UDIEW200S_PROGRAMEINFO:    ///< [r] 节目信息,CSUDIEW200SProgrameInfo_S
        break;

        default:
        CSASSERT(0);
        break;

    }

    return nLength;
}

/**
@brief 设置终端信息接口

Eastwin200将通过该接口将一些用户信息设置到终端永久存储或者应用自己存储起来
@param[in] eType 信息类型
@param[in] value eType类型对应的值
@param[in] length value值的长度
@return 返回实际存储信息的长度,出错返回-1
@note 需要设置的信息见CSUDIEW200SClientInfoType_E枚举中的[w]标识
@note 支持Coship对象、Enreach对象时需要实现该接口
@note 参考实现同CSUDIEW200SReadClientInfo
*/
int CSUDIEW200SWriteClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length)
{
    int nLength = 0;
    if((eType <EM_UDIEW200S_CARDID) || (eType >EM_UDIEW200S_PROGRAMEINFO))
    {
       CSDebug("TEST_UDI_EW200SERVICE", ERROR_LEVEL,"[CS_EW200Ext] eType is out of defined type \r\n");
        CSASSERT((eType >=EM_UDIEW200S_CARDID) && (eType <=EM_UDIEW200S_PROGRAMEINFO));
		return -1;
    }
    if(value == NULL || length <=0)
    {
        CSDebug("TEST_UDI_EW200SERVICE", ERROR_LEVEL,"[CS_EW200Ext] value is NULL or length is invalid \r\n");
        CSASSERT((value != NULL)&&(length >0));
		return -1;
    }
    switch(eType)
    {
    case EM_UDIEW200S_CITV_USERID:     ///< [w][r]CITV VOD用户号,第一次获取传全0,通过认证后CITV3会反馈该值,请将其存储在ROM中,以备下次使用,char*
        if(length<sizeof(g_chUserID))
        {
            strncpy(g_chUserID,value,length);
        }      
        nLength = length;
        break;

    case EM_UDIEW200S_CITV_TOKEN:      ///< [w][r]CITV 令牌,第一次获取传全0,通过认证后CITV3会反馈该值,请将其存储在ROM中,以备下次使用,char*
        if(length<sizeof(g_chToken))
        {
            strncpy(g_chToken,value,length);
        }      
        nLength = length;
        break;

    case EM_UDIEW200S_CITV_INMUSICSHOW:///< [w][r]是否进入音秀模式,"TRUE":音秀模式 "FALSE":非音秀模式.EW200接收到"ITVRTSPURL"关键字时,进入音秀模式;接收到"ITVControl"关键字,值为"ITVStop"时退出音秀模式
        break;

    case EM_UDIEW200S_ISMUTE:          ///< [r][w]静音状态,CSUDI_BOOL,CSUDI_TRUE表示当前为静音状态,CSUDI_FALSE表示为非静音状态
     	{
		g_nMute =  *(int *)value;
		if(g_nMute == CSUDI_TRUE)
			CSUDIAUDIOMute (0);
		else
			CSUDIAUDIOUnmute (0);
		nLength = sizeof(int);   
    	}		
        break;

    case EM_UDIEW200S_VOLUME:          ///< [r][w]音量,int,[0,31],0为无声,31最大
        {   
         g_nVolume = *(int *)value ;
	 CSUDIAUDIOSetVolume(0, g_nVolume);
            nLength = sizeof(int);        
        }   
        break;
    case EM_UDIEW200S_AUDIOCHANNEL:    ///< [w]声道,CSUDIEW200SAUDChannelType_E
        {
            int nTrack = *(int *)value;
            CSUDIAUDIOSetChannel (0, nTrack);
            nLength = sizeof(int);
        }
        break;
    case EM_UDIEW200S_AUDIOPID:        ///< [r][w]音频pid,int,
        g_program.audio[0].aud = *(int *)value;            
        nLength = sizeof(int);     
        break;

    case EM_UDIEW200S_VIDEOALPHA:      ///< [w] 窗口的透明度，int,[0,100],0为完全透明，100为不透明
        {
            int nOSDAlpha = *(int *)value;   
            if(nOSDAlpha >=0&&nOSDAlpha<=100)
            {
                CSUDISCREENSetOSDTransparency (nOSDAlpha);
                nLength = sizeof(int);
            }
        }
        break;

    case EM_UDIEW200S_BRIGHTNESS:      ///< [w]视频亮度，int,[0,100],0为全黑，100为亮度最高
        {
            int nOSDBrightness = *(int *)value;             
            if(nOSDBrightness >=0&&nOSDBrightness<=100)
            {
                CSUDISCREENSetBrightness (nOSDBrightness);
                nLength = sizeof(int);
            }
        }
        break;

    case EM_UDIEW200S_CONTRAST:        ///< [w]视频对比度，int,[0,100],0为全黑，100为对比度最高
        {
            int nOSDContrast = *(int *)value;             
            if(nOSDContrast >=0&&nOSDContrast<=100)
            {
                CSUDISCREENSetContrast (nOSDContrast);
                nLength = sizeof(int);
            }
        }        
        break;

    case EM_UDIEW200S_SATURATION:      ///< [w]视频饱和度，int,[0,100],0为全黑，100为饱和度最高
        {
            int nOSDChroma = *(int *)value;             
            if(nOSDChroma >=0&&nOSDChroma<=100)
            {
                CSUDISCREENSetSaturation (nOSDChroma);
                nLength = sizeof(int);
            }
        } 
        break;

    case EM_UDIEW200S_ASPECTRATIO:     ///< [w]输出宽高比,CSUDIEW200SAspectRatio_E
        {
            int nAspectRatio = *(int *)value;  
            CSUDIVIDEOSetAspectRatio (0, 0, nAspectRatio);
            nLength = sizeof(int);
        } 
        break;

    case EM_UDIEW200S_MATCHMETHOD:     ///< [r][w]窗口的pan scan等格式,CSUDIEW200SMatchMethod_E
        {
            int nAutoApdat = *(int *)value;  
                CSUDIVIDEOSetMatchMethod (0, 0, nAutoApdat);
                nLength = sizeof(int);
        }
        break;

    case EM_UDIEW200S_VIDEOFORMAT:     ///< [w]视频输出制式，CSUDIEW200SVdisVideoFormat_E   
        //中山强制Pal;
        nLength = sizeof(int);
        break;

    case EM_UDIEW200S_STOPMODE:        ///< [r][w]换台时视频的处理类型:CSUDIEW200SStopMode_E
        //中山强制黑屏;
        g_nStopMode = *(int *)value;
       if (CSUDI_SUCCESS == CSUDIVIDEOSetStopMode (0, g_nStopMode))
       	{
       		 nLength = sizeof(int);
       	}
        break; 

        default:
        CSASSERT(0);
        break;
		
  	
	}
	return nLength;
	
    }





/**
@brief 通过DVB3要素查询数据源信息

包括获取该3要素对应信息时所需使用的TunerId,DemuxId,及频点
@param[in] nOrNetId org network id
@param[in] nTsId ts id
@param[in] nServiceId service id
@param[out] psSourceParam 用于存储输出的数据源信息
@return 如果成功返回CSUDI_SUCCESS, 失败返回错误代码
@note EW200通过该接口查询一个Service的对应的频点及高频头信息，若平台有多个高频头，这个接口的实现要特别小心，因为对应的数据可能在不同的高频头上，否则简单查询数据管理即可
*/
CSUDI_Error_Code CSUDIEW200SGetTuneParam(int nOrNetId,int nTsId,int nServiceId,CSUDIEW200SSourceParam_S * psSourceParam)
{
    CSUDI_Error_Code eRet = CSUDI_FAILURE;
    if(psSourceParam == NULL)
    {
        CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] psSourceParam is NULL \r\n");
        CSASSERT(psSourceParam != NULL);        
    }
    else
    {
        HCSHANDLE hIterate = (DWORD)NULL;
        HCSHANDLE hService = (DWORD)NULL;
	doSearch_DTV();
       hIterate = CSUSPDMCreateIterateAllService(getDMContainer(),0xFF);
        if(hIterate != (DWORD)NULL)
        {
            int nServiceID = 0;
            SourceID stSourceID = {0};
            HCSHANDLE hTp = (DWORD)NULL;
            hService = CSUSPDMGetHeadDataHandle(hIterate);
            while(hService != (DWORD)NULL)
            {
                CSUSPDMGetService_id(&nServiceID,hService);
                if(nServiceID == nServiceId)
                {
                    hTp = CSUSPDMGetTransponderByService(hService,getDMContainer());
                    if(hTp != (DWORD)NULL)
                    {
                        CSUSPDMGetSourceID(&stSourceID,hTp);
                        if(stSourceID.m_wTSID == nTsId && stSourceID.m_wOriginal_network_id == nOrNetId)
                        {
                            CableDeliver stCableDeliver ;
                            memset(psSourceParam,0,sizeof(CSUDIEW200SSourceParam_S));
                            CSUSPDMGetCableDeliver(&stCableDeliver,hTp);
                            psSourceParam->m_punDeliver.m_sCableDeliver.m_uFrequency = stCableDeliver.m_dwFrequency;
                            psSourceParam->m_punDeliver.m_sCableDeliver.m_uModulation_8 = stCableDeliver.m_dwModulation_8;
                            psSourceParam->m_punDeliver.m_sCableDeliver.m_uSymbolRate_24 = stCableDeliver.m_dwSymbolRate_24;                            
                            eRet = CSUDI_SUCCESS;
                            break;
                        }
                    }
                    else
                    {
                        CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] hTp is NULL \r\n");
                        CSASSERT(hTp != (DWORD)NULL);
                        break;
                    }
                }
                hService = CSUSPDMGetNextDataHandle(hIterate);
            }
            CSUSPDMFreeIteratorHandle(hIterate,getDMContainer());
        }
    }    
    return eRet;
}

/**
@brief 获取IPQam域信息接口

常用语VOD模块
认证模块通过调用终端实现的该接口得到IPQam的域信息
- CITV3.x版本的VOD目前取参数指定Section的第4、5字节(即data[3],data[4])
@param[in] punDeliverList 存放信息所在频点列表数组
@param[in] nDeliverCount pDeliverList数组中的频点个数
@param[in] nPid IPQam域信息所在Pid
@param[in] ucTableId IPQam域信息所在TableId
@param[out] pIpQamDataBuffer 用于存放IPQam域信息的内存指针
@param[in] nBufferLength pIpQamDataBuffer的长度
@param[in] pvReserved 保留字段,目前必须为NULL
@return 成功获取返回TRUE,失败返回FALSE
*/
CSUDI_BOOL CSUDIEW200SGetIPQamDomainInfo(const CSUDITunerSRCDeliver_U* punDeliverList,
										int nDeliverCount,
										int nPid,
										unsigned char ucTableId,
										char* pIpQamDataBuffer,
										int nBufferLength,
										void* pvReserved
									   )
{
    BOOL bRet = FALSE;
    int i;

    CSDebug("TEST_UDI_EW200SERVICE",INFO_LEVEL,"[CS_EW200Ext] nPid =%d ucTableId = %d nBufferLength = %d "
        ,nPid,ucTableId,nBufferLength);

    if (g_hSemIpQamDomainInfo == 0)
    {
        CSUDIOSSemCreate("MMCPSaForDomainInfo", 0, 1, &g_hSemIpQamDomainInfo);
        if (g_hSemIpQamDomainInfo == 0)
        {
            CSASSERT(g_hSemIpQamDomainInfo != 0);
            return FALSE;
        }
    }
    g_wPid = nPid;
    g_ucTableId = ucTableId;
    g_bWorking = TRUE;

    CSUSPSourceAddCallback(USPHANDLE_SOURCE(),CSQamDomain_Callback, NULL );
    
    CSASSERT(bRet);

     CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] Domain deliver count is %d ",nDeliverCount);

    for (i = 0;i < nDeliverCount;i++)
    {
        SourceDeliver stSourceDeliver;
        g_nTunerRetryCount = 0;
        g_nDomainInfoLength = 0;

        stSourceDeliver.m_CableDeliver.m_dwFrequency = punDeliverList[i].m_sCableDeliver.m_uFrequency;
        stSourceDeliver.m_CableDeliver.m_dwModulation_8 = punDeliverList[i].m_sCableDeliver.m_uModulation_8;
        stSourceDeliver.m_CableDeliver.m_dwSymbolRate_24 = punDeliverList[i].m_sCableDeliver.m_uSymbolRate_24;
         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] the %d deliver ( %d,%d,%d) \r\n"
            ,i
            ,stSourceDeliver.m_CableDeliver.m_dwFrequency
            ,stSourceDeliver.m_CableDeliver.m_dwSymbolRate_24
            ,stSourceDeliver.m_CableDeliver.m_dwModulation_8);
        
        bRet = ConnectSource(USPHANDLE_SOURCE(),&stSourceDeliver);
        if (!bRet)
        {
             CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext]Connect Source failed.\r\n");
            bRet = FALSE;
        }
        else
        {
            bRet = FALSE;//CSTunerConnectSource是一个异步的过程
             CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext]Connect Source bRet:%d.\n",bRet);
            CSUDIOSSemWait(g_hSemIpQamDomainInfo, 10*1000);
             CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] g_nDomainInfoLength = %d\n", g_nDomainInfoLength);
    
            if (g_nDomainInfoLength > 0)
            {
                bRet = TRUE;
                 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] g_aucIpQamDomainInfo = %s\n", g_aucIpQamDomainInfo);
                CSASSERT(nBufferLength >= g_nDomainInfoLength);
                memcpy(pIpQamDataBuffer,g_aucIpQamDomainInfo,g_nDomainInfoLength);
                break;
            }
        }
    }
    CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] Connect Source  bRet:%d.\n",bRet);
    g_bWorking = FALSE;
    CSUSPSourceDelCallback(USPHANDLE_SOURCE(),CSQamDomain_Callback);
    return bRet;
}


static void CSQamDomainParseInfo(BYTE *data, int length)
{
    g_aucIpQamDomainInfo[0] = data[3];
    g_aucIpQamDomainInfo[1] = data[4];
    g_nDomainInfoLength = 2;
}

static void CSQamDomainNotifyInfo(SectionEventType event, Section* pSection,void* pUserData, HCSHANDLE hRequest)
{
    Section2CSectionCancel(hRequest);
    if (!g_bWorking)
    {
         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext][MMCPSANotifyAreacode]not in working \r\n");
    }

    if (event != SECTION_TIMEOUT) //超时后CancelLoad
    {
        if (pSection)
        {
            CSQamDomainParseInfo(pSection->m_pucDataBuf,pSection->m_nDataLen);
        }
    }
    CSUDIOSSemRelease(g_hSemIpQamDomainInfo);
    return;
}

static void CSQamDomain_Callback( CSSourceCallbackType nSourceCallbackType, SourceCallbackMessage *pSourceCallbackMessage )
{
    RequestInfo sRequestInfo;

     CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext][MMCPSAQAMCallback](pemMessage->m_SourceCallbackEvent)=%d\r\n",pSourceCallbackMessage->m_SourceCallbackEvent);

    if (NULL == pSourceCallbackMessage)
    {
         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext][MMCPSAQAMCallback]Input parameter invalid\r\n");
        return ;
    }

    if (!g_bWorking)
    {
         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext][MMCPSAQAMCallback]not in working\r\n");
        return;
    }

    switch (pSourceCallbackMessage->m_SourceCallbackEvent)
    {
    case SOURCE_SIGNAL_CONNECTED:
         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext][MMCPSAQAMCallback]Signal connect\r\n");
        memset(&sRequestInfo,0,sizeof(sRequestInfo));
        sRequestInfo.dwPID = g_wPid;
        sRequestInfo.MMFilter.match[0] = g_ucTableId;
        sRequestInfo.MMFilter.mask[0] = 0xff;
        sRequestInfo.lTimeout = 5000;

        g_nTunerRetryCount = 0;
        Section2CSectionRequest(&sRequestInfo,CSQamDomainNotifyInfo);
        break;

    default:
        if (g_nTunerRetryCount < 3)
        {
             CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext][MMCPSAQAMCallback] g_sITVInfo.nTuneCount = %d\r\n",g_nTunerRetryCount );
            g_nTunerRetryCount++;
        }
        else
        {
            CSUDIOSSemRelease(g_hSemIpQamDomainInfo);
            g_nTunerRetryCount = 0;
        }
         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext][MMCPSAQAMCallback]Lock failure, searching");
        break;
    }
    return ;
}

									   
/**
@brief 播放节目接口

根据制定URL的格式进行播放

@param[in] pcURL 播放URL
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@code 播放URL的格式有以下几种，应用可以根据不同的URL格式进行播放处理：
(1)dvb://OriginalNetworkId.TS_Id.ServiceId  具体参考《数字电视中间件技术规范》第3部分附录I的1.2.1小节。例如:dvb://1.2.5(默认十六进制)
(2)dvbc://Frequency.SymbolRate.Modulation.ServiceId
		Frequency:频率.单位MHz
		SymbolRate:符号率，单位是Kbps
		Modulation:调制模式
		ServiceId:节目Id
		例如: deliver://490000000:6875:64:10(ServiceId为十六进制)
(3)avpid://pcrPid.emm.videoPid.audioPid.audio_ecm_pid.video_ecm_pid(十六进制)	
@endcode
*/
void CSUDIEW200SPlayStream(char* pcURL, int nPlayId)
{    
    SourceDeliver stSourceDeliver;   
    int nSerivceId = -1;
	int nFreq = 0;
	int nSymbol = 0;
	int nModulation = 0;
    	g_nReqPatCount = 0;
	
     CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext][CSUDIEW200SPlayStream]URL = %s \n",pcURL);
    sscanf( pcURL ,"dvbc://%d.%d.%d.%x", &nFreq, &nSymbol , &nModulation , &nSerivceId);

    g_program.ServiceID = nSerivceId;
    stSourceDeliver.m_CableDeliver.m_dwFrequency = nFreq/1000;
    stSourceDeliver.m_CableDeliver.m_dwModulation_8 = EM_UDITUNER_QAM64;
    stSourceDeliver.m_CableDeliver.m_dwSymbolRate_24 = nSymbol;
  
    CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] %d %d %d %d\n",  \
		stSourceDeliver.m_CableDeliver.m_dwFrequency,stSourceDeliver.m_CableDeliver.m_dwModulation_8,stSourceDeliver.m_CableDeliver.m_dwSymbolRate_24,nSerivceId);

    CSUSPSourceAddCallback( USPHANDLE_SOURCE(), SourceCallback, NULL );
    ConnectSource(USPHANDLE_SOURCE(),&stSourceDeliver);
    return ;
}

/**
@brief 停止节目播放接口

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
*/
void CSUDIEW200SStopPlay(int nPlayId)
{
    CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] STOP Play \r\n");
    CSUDIPLAYERStop(g_hPlayer);
    CSUDIPLAYERClose(g_hPlayer);
    return ;
}

/**
@brief 设置视频播放窗口大小接口

@param[in] psRect 视频矩形大小,为NULL表示全屏播放
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
*/
void CSUDIEW200SSetPlayRect(CSUDIWinRect_S* psRect,int nPlayId)
{
    CSUDIWinRect_S stRect;
    if(psRect == NULL)
    {
        stRect.m_nX = 0;
        stRect.m_nWidth = 720;
        stRect.m_nY = 0;
        stRect.m_nHeight = 576;
        CSASSERT(psRect != NULL);
        CSUDIVIDEOSetWindowSize (0, 0, &stRect);
        return ;
    }
    if((psRect->m_nX < 0) || (psRect->m_nY < 0) || (psRect->m_nX > 720) || (psRect->m_nY > 576)
        ||(psRect->m_nWidth <0 ) || (psRect->m_nHeight < 0 )
        || ((psRect->m_nX + psRect->m_nWidth) > 720) || ((psRect->m_nY + psRect->m_nWidth)>576))
    {
       CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] Rect  is Invalid \r\n");
    }
    else
    {                
        CSUDIWinRect_S stRect;
        stRect.m_nX= psRect->m_nX;
        stRect.m_nY = psRect->m_nY;
        stRect.m_nWidth = psRect->m_nWidth;
        stRect.m_nHeight = psRect->m_nHeight;
        CSUDIVIDEOSetWindowSize(0, 0, &stRect);
    }
    return ;
}

/**
@brief 注册UDI层消息回调函数

注册后UDI层相关的消息会通过该接口通知到应用
@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@param[in] pUserData 用户数据,会在回调中原样返回
@return 成功返回CSUDI_TRUE,失败返回CSUDI_TRUE
*/
CSUDI_Error_Code CSUDIEW200SAddCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata)
{    
    CSUDI_Error_Code nRet = CSUDI_FAILURE;

    if(fncallback == NULL)
    {
         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CS_EW200Ext] The callback function to be added is NULL !");
        CSASSERT(fncallback!=NULL);
    }
    else if(g_fnEW200Register.fnCallback== NULL)
    {
        CSUDIOSSemWait((CSUDI_HANDLE)g_hSemVodCallback, 2*1000);
        g_fnEW200Register.fnCallback=fncallback;
        g_fnEW200Register.dwUserData = (DWORD)pUserdata;
        CSUDIOSSemRelease((CSUDI_HANDLE)g_hSemVodCallback);
        nRet = CSUDI_SUCCESS;
    }
    else
    {
         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CS_EW200Ext] The callback has added");
    }
    return nRet;
}

 /**
@brief 删除UDI层回调函数的注册

@param[in] nPlayId 参见Eastwin200VOD中的nPlayId定义,在支持多Demux,多Tuner,Pip等功能的机器上移植层可以用该参数配合应用确定使用哪个设备，其它情况可忽略该参数
@param[in] fnTunerCallback 回调函数句柄
@param[in] pUserData 同CSUDIEW200SAddCallback中的pUserData
@return成功返回CSUDI_SUCCESS 如果该回调函数根本就没有注册，将返回CSUDIEW200S_ERROR_NO_CALLBACK
*/
CSUDI_Error_Code CSUDIEW200SDelCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata)
{
	return CSUDI_SUCCESS;
}

/****************************************************/
//                  
/**
锁频回调
*/
static void SourceCallback( CSSourceCallbackType nSourceCallbackType, SourceCallbackMessage *pSourceCallbackMessage )
{
printf("going-----pSourceCallbackMessage->m_SourceCallbackEvent:%d---\n" ,pSourceCallbackMessage->m_SourceCallbackEvent);
   // CSUSPSourceDelCallback(USPHANDLE_SOURCE(),SourceCallback);
	switch( pSourceCallbackMessage->m_SourceCallbackEvent )
	{
	case SOURCE_SIGNAL_CONNECTED:
		{
             CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CS_EW200Ext] SourceCallback connect \n" );
            if(g_fnEW200Register.fnCallback != NULL)
            {
                CSUDIOSSemWait((CSUDI_HANDLE)g_hSemVodCallback, 2*1000);
                g_fnEW200Register.fnCallback(EM_EW_MSG_VOD_CONNECT_SUCCESS, NULL, (void *)g_fnEW200Register.dwUserData);
                CSUDIOSSemRelease((CSUDI_HANDLE)g_hSemVodCallback);
            }
            
            EW200MSGINFO szEW200MsgInfo = {0};
			szEW200MsgInfo.udwMsg = MSG_EW200_START_PAT;
			CSUDIOSMsgQueueSend(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0);	
	    }
		break;

    case SOURCE_SIGNAL_LOST:
        {
             CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CS_EW200Ext] SourceCallback lost \n" );
            if(g_fnEW200Register.fnCallback != NULL)
            {
                CSUDIOSSemWait((CSUDI_HANDLE)g_hSemVodCallback, 2*1000);
                g_fnEW200Register.fnCallback(EM_EW_MSG_VOD_CONNECT_FAILED, NULL, (void *)g_fnEW200Register.dwUserData);
		 CSUDIOSSemRelease((CSUDI_HANDLE)g_hSemVodCallback);
            }
	    }
        break;
		
	default:
		break;
	}
	printf("------SourceCallback end -----------\n");
}

/**
请求PAT
*/

static int CSGTVODStartPAT(void)
{
	RequestInfo	patinfo;

	CSCancelPatSection();
	memset(&patinfo, 0, sizeof(patinfo));
	patinfo.bCRCCheck = ENABLE;
	patinfo.dwPID = PAT_PID;
	patinfo.enmPriority = SECTION_PRIORITY_HIGH;
	patinfo.lTimeout = 6;
	patinfo.MMFilter.match[0] = PAT_TABLE_ID;
	patinfo.MMFilter.mask[0] = 0xff;
	patinfo.MMFilter.negate[0] = 0;

	g_PAThandle = Section2CSectionRequest(&patinfo, CSGTVODPatNotify);
	if(g_PAThandle == (DWORD)INVALID_SECTION_HANDLE)
	{
		 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"\r\n[CSVODStartPlay]Request section failure.\r\n");
		return FAILURE;
	}
    g_nReqPatCount ++;
	return SUCCESS;
}

/**
PAT 回调
*/

static void CSGTVODPatNotify(SectionEventType event, Section *pSection, void *pUserData,
						   HCSHANDLE hRequest)
{
	BYTE   *data;
	WORD	length;
    int     nRet;
    EW200MSGINFO szEW200MsgInfo = {0};

	if (pSection == (DWORD)NULL || hRequest == (DWORD)NULL )
	{
		CSASSERT(pSection != (DWORD)NULL);
		CSASSERT(hRequest != (DWORD)NULL);
		szEW200MsgInfo.udwMsg = MSG_EW200_START_PAT;
		CSUDIOSMsgQueueSend(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0);	

		return;
	}
    if ((pSection->m_pucDataBuf == (DWORD)NULL)
        || (0 >= pSection->m_nDataLen))
    {
		CSASSERT(pSection->m_pucDataBuf != (DWORD)NULL);
		return;        
    }
	switch(event)
	{
	case SECTION_REPEATED:
		{
			return;
		}
		break;
	    
	case SECTION_AVAIL:
		{
			 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CSPatNotify]SECTION_AVAIL:handle = %d, event = %d.\r\n",
						hRequest, event);
		}
		break;

	default:
		break;
	}

	data = pSection->m_pucDataBuf;
	length = (WORD) pSection->m_nDataLen;
    
    if(length >0)
    {
        nRet = CSParsePat(data, g_program.ServiceID);
        if(nRet == FAILURE)
        {
			szEW200MsgInfo.udwMsg = MSG_EW200_START_PAT;
			CSUDIOSMsgQueueSend(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0);	
        }
    }

    if ((SECTION_TIMEOUT == event) || (SECTION_COMPLETE == event))
    {
        CSCancelPatSection();
    }
    
    return;
}

/**
PMT 回调
*/

static void CSGTVODPmtNotify(SectionEventType event, Section *pSection, void *pUserData,
						   HCSHANDLE hRequest)
{
	BYTE   *data;
	DWORD	length;
    EW200MSGINFO szEW200MsgInfo = {0};
    
    memset(&szEW200MsgInfo,0,sizeof(EW200MSGINFO));
	if (pSection == (DWORD)NULL || hRequest == (DWORD)NULL)
	{
		CSASSERT(pSection != (DWORD)NULL);
		CSASSERT(hRequest != (DWORD)NULL);	
        szEW200MsgInfo.udwMsg = MSG_EW200_START_PAT;
    	CSUDIOSMsgQueueSend(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0);	
		return;
	}
    if(pSection->m_pucDataBuf == (DWORD)NULL)
    {
        szEW200MsgInfo.udwMsg = MSG_EW200_START_PAT;
    	CSUDIOSMsgQueueSend(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0);	
        CSASSERT(pSection->m_pucDataBuf != (DWORD)NULL);
        return;
    }

	switch(event)
	{
    	case SECTION_REPEATED:
    		return;

    	case SECTION_AVAIL:
    		{
    			 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CSVODPmtNotify]event: handle = %d, event = %d.\r\n",
    						hRequest, event);
    		}
    		break;

    	default:
    		break;
	}


	data = pSection->m_pucDataBuf;
	length = pSection->m_nDataLen;
    
    if ((0 < length) && (SUCCESS == CSParsePmt(data)))
    {
	    if(g_fnEW200Register.fnCallback != NULL)
        {
            CSUDIOSSemWait((CSUDI_HANDLE)g_hSemVodCallback, 2*1000);
            g_fnEW200Register.fnCallback(EM_EW_MSG_VOD_LOAD_SUCCESS, NULL, (void *)g_fnEW200Register.dwUserData);
            CSUDIOSSemRelease((CSUDI_HANDLE)g_hSemVodCallback);
        }
        szEW200MsgInfo.udwMsg = MSG_EW200_START_PLAY_VOD;
	}
	else 
	{
        if(g_fnEW200Register.fnCallback != NULL && g_nReqPatCount >= MAX_REQUST_PAT_PMT_COUNT)
        {
            CSUDIOSSemWait((CSUDI_HANDLE)g_hSemVodCallback, 2*1000);
            g_fnEW200Register.fnCallback(EM_EW_MSG_VOD_NO_PROGRAME, NULL, (void *)g_fnEW200Register.dwUserData);
            CSUDIOSSemRelease((CSUDI_HANDLE)g_hSemVodCallback);
        }
        else
        {
            
            szEW200MsgInfo.udwMsg = MSG_EW200_START_PAT;
        }
	}	

	if((SECTION_TIMEOUT == event) || (SECTION_COMPLETE == event))
    {
        CSCancelPMTSection();
    }
    if(szEW200MsgInfo.udwMsg != 0)
    {
        CSUDIOSMsgQueueSend(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0);  
    }
    
    return;
}

/**
PAT解析
*/

static int CSParsePat(BYTE *data, WORD ServiceID)
{
	char	last;
	WORD	CurTableServiceID	= INVALID_PID;
	WORD	PMTPID				= INVALID_PID;
	WORD	len					= 0;
     CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"\r\n   CSParsePat ");

	if(data[0] != PAT_TABLE_ID)
	{
		 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,
					"\r\n[CSParsePat]Table id(in buf) is incorrect, returned table id=%x.\r\n",
					data[0]);
		return FAILURE;
	}

	len = ((data[1] << 8) & 0xf00) | data[2];
	last = data[7];
	data += 8;
	len -= 9;

	while(len >= 4)
	{
		CurTableServiceID = ((data[0] << 8) & 0xff00) | data[1];
		PMTPID = (WORD) (((data[2] << 8) & 0x1f00) | data[3]);
		 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CSParsePat]CurTableServiceID = %d, PMTPID:%d. ServiceID should be %d \r\n",
					CurTableServiceID, PMTPID,ServiceID);

		if(CurTableServiceID == ServiceID)
		{
		    EW200MSGINFO szEW200MsgInfo = {0};
		    //发送请求PMT的消息
        	szEW200MsgInfo.udwMsg = MSG_EW200_START_PMT;
        	szEW200MsgInfo.udwlParam = ServiceID;
        	szEW200MsgInfo.udwwParam = PMTPID;
        	CSUDIOSMsgQueueSend(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0);				
			return SUCCESS;
		}

		data += 4;
		len -= 4;
	}
	 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[ParsePat]Can't find specified serviceid.\r\n");

	return FAILURE;
}
/**
PMT解析
*/

static int CSParsePmt(BYTE *data)
{
	WORD	CurTableServiceID;
	WORD	section_length;
	WORD	len;
	int     i = 0;

     CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"\r\n   CSParsePmt ");
	if(data[0] != PMT_TABLE_ID)
	{
		 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,
					"\r\n[CSParsePmt]Table id(in buf) is incorrect. returned table id=%x\r\n",
					data[0]);
		return FAILURE;
	}

	section_length = ((data[1] << 8) & 0xf00) | data[2];
	CurTableServiceID = ((data[3] << 8) & 0xff00) | data[4];

	if(CurTableServiceID != g_program.ServiceID)
	{
		 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CSParsePmt]Service id incorrect.\r\n");
		return  FAILURE;
	}

	g_program.pcr = ((data[8] << 8) & 0x1f00) | data[9];
	len = ((data[10] << 8) & 0xf00) | data[11];
	data += 12;
	data += len;
	section_length -= len;
	section_length -= 13;

	while(section_length > 0)
	{
		BYTE	type;
		WORD	pid;

		type = data[0];
		pid = ((data[1] << 8) & 0x1f00) | data[2];

		switch(type)
		{
		case H264_VIDEO_STREAM:
		case MPEG1_VIDEO_STREAM :
		case MPEG2_VIDEO_STREAM :
		case MPEG4_STREAM_VIDEO :
		case MPEG21_STREAM_VIDEO :
			g_program.videoID = pid;
			g_program.Videotype = type;
			break;

		case AC3_6_AUDIO_STREAM:
		case AAC_AUDIO_STREAM:
		case MPEG1_AUDIO_STREAM :
		case MPEG2_AUDIO_STREAM :
			g_program.audio[i].aud = pid;
			g_program.audio[i].audiotype = type;//MPEG_AUDIO;
			i++;
			break;

		case AC3_AUDIO_STREAM:
			g_program.audio[i].aud = pid;
			g_program.audio[i].audiotype = type;//AC3_AUDIO;
			i++;
			break;

		default :
			break;
		}

		if (i >= MAX_AUDIO_NUM)
			break;

		len = ((data[3] << 8) & 0xf00) | data[4];
		data += 5;

		data += len;
		section_length -= 5;
		section_length -= len;
	}
	
	g_program.nAudioNum = i;

	 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[CSParsePmt]Vpid:%d, Apid: %d, Pcrpid: %d\r\n", g_program.videoID,
				g_program.audio[0].aud, g_program.pcr);

	return SUCCESS;
}


static void CSCancelPatSection(void)
{
	if(g_PAThandle != (DWORD)INVALID_SECTION_HANDLE)
	{
		Section2CSectionCancel(g_PAThandle);
		g_PAThandle = (DWORD)INVALID_SECTION_HANDLE;
	}
}


static void CSCancelPMTSection(void)
{
	if(g_PMThandle != (DWORD)INVALID_SECTION_HANDLE)
	{
		Section2CSectionCancel(g_PMThandle);
		g_PMThandle = (DWORD)INVALID_SECTION_HANDLE;
	}
}

static CSUDI_BOOL iIsAudioSupportDecodeType(int nAudioIndex,int nType)
{
	int nSupportIndex;
	CSUDIAUDIOCapability_S sCapablity;
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	if(CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity) != CSUDI_SUCCESS)
	{
		printf("Get Audio (%d) capability failure !!\n",nAudioIndex);
		return CSUDI_FALSE;
	}
	
	for(nSupportIndex = 0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportDecode[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
	{
		if(sCapablity.m_sSupportDecode[nSupportIndex] == nType)
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}

static CSUDI_BOOL iIsAudioSupportBypassType(int nAudioIndex,int nType)
{
	int nSupportIndex;
	CSUDIAUDIOCapability_S sCapablity;
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	if(CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity) != CSUDI_SUCCESS)
	{
		CSTCPrint("Get Audio (%d) capability failure !!\n",nAudioIndex);
		return CSUDI_FALSE;
	}
	
	for(nSupportIndex = 0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportByPass[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
	{
		if(sCapablity.m_sSupportByPass[nSupportIndex] == nType)
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}

int PLAYER_searchAudioID_Y(int nAudioType)
{
	int nAudioCount = 0;
	int nAudioID = -1;
	int nAudioIndex = 0;
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //先取得音频解码器的数量
	{
		for (nAudioIndex = 0; nAudioIndex<nAudioCount; nAudioIndex++)
		{
			if(nAudioType == EM_UDI_AUD_STREAM_AC3)//AC3才检测bypass模式
			{
				if (iIsAudioSupportBypassType(nAudioIndex,nAudioType))
				{					
					nAudioID = nAudioIndex;
					
					break;
				}
			}
			else
			{
				if (iIsAudioSupportDecodeType(nAudioIndex,nAudioType))
				{					
					nAudioID = nAudioIndex;
					
					break;
				}
			}
		}
	}	
	printf("nAudioID nAudioID=%d\n\r",nAudioID);
	return nAudioID;
}


//video

static CSUDI_BOOL iIsVideoSupportDecodeType(int nVideoIndex,int nType)
{
	int nSupportIndex;
	CSUDIVIDEOCapability_S  sCapability;
	
	memset(&sCapability,EM_UDI_VID_STREAM_UNKNOWN,sizeof(CSUDIVIDEOCapability_S));
	if(CSUDIVIDEOGetCapability(nVideoIndex, &sCapability) != CSUDI_SUCCESS)
	{
		printf("Get Video (%d) capability failure !!\n",nVideoIndex);
		return CSUDI_FALSE;
	}
	
	for (nSupportIndex=0; 
			nSupportIndex < EM_UDI_VID_STREAMTYPE_NUM && sCapability.m_eStreamType[nSupportIndex]!= EM_UDI_VID_STREAM_UNKNOWN;
				nSupportIndex++)
	{
		if((sCapability.m_eStreamType[nSupportIndex] == nType))
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}

int PLAYER_searchVideoID_Y(int nVideoType)
{	
	int nVideoCount = 0;
	int nVidioID = -1;
	int nVideoIndex = 0;
			
	if (CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)) //先取数量
	{
		for (nVideoIndex = 0; nVideoIndex<nVideoCount; nVideoIndex++)
		{
			if(iIsVideoSupportDecodeType(nVideoIndex,nVideoType))
			{					
				nVidioID = nVideoIndex;
				break;
			}
		}
	}
	printf("nVidioID nVidioID=%d\n\r",nVidioID);
	return nVidioID;
}


//demux
int PLAYER_searchDemuxID_Y(int nDemxType)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;
	
	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //先取得demux的数量
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //取相对应ID=i 的demux的能力
			{
				if((sDemuxCapabilityInfo.m_dwWorkTypeMask & nDemxType ) != 0)
				{
					nDemuxID=i;
					
					break;
				}
			}	
		}
	}
	printf("nDemuxID nDemuxID=%d\n\r",nDemuxID);
	return  nDemuxID;
}



static void CSStartplayVOD()
{
	int nDemuxID = 0;
	int nAudioID = 0;
	int nVideoID = 0;
	int nStreamCnt;
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  asStreamInfo[10];
	memset(&sPlayerChnl, 0, sizeof(sPlayerChnl));
	CSUDI_Error_Code nRe  = CSUDI_FAILURE;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

//设置节目信息
	memset(asStreamInfo,0,sizeof(asStreamInfo));

	asStreamInfo[0].m_nPid=0xd1;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_nPid=0xd3;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;	

	asStreamInfo[2].m_nPid=0xca;
	asStreamInfo[2].m_eContentType=EM_UDI_CONTENT_PCR;

	nStreamCnt=3;

	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);
	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_MPEG2);

	sPlayerChnl.m_nAudioDecoder = nAudioID;
	sPlayerChnl.m_nVideoDecoder = nVideoID;
	sPlayerChnl.m_nDemux = nDemuxID;
	
	nRe =  CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &g_hPlayer);
	CSUDIPLAYERSetStream (g_hPlayer, asStreamInfo, nStreamCnt, CSUDI_NULL);
	nRe = CSUDIPLAYERStart(g_hPlayer);

    if(g_fnEW200Register.fnCallback != NULL)
    {
        CSUDIOSSemWait((CSUDI_HANDLE)g_hSemVodCallback, 2*1000);
        g_fnEW200Register.fnCallback(EM_EW_MSG_VOD_PLAYSTATE_READY, NULL, (void *)g_fnEW200Register.dwUserData);
        CSUDIOSSemRelease((CSUDI_HANDLE)g_hSemVodCallback);
    }
}

BOOL CSEW200ExtApp_Init()
{
    if(!g_BInitEWExt)
    {
        g_fnEW200Register.dwUserData = 0;
        g_fnEW200Register.fnCallback = 0;
        memset(g_chUserID,0,sizeof(g_chUserID));
        memset(g_chToken,0,sizeof(g_chToken));
        g_fnEW200Register.dwUserData = 0;
        g_fnEW200Register.fnCallback = 0;
        g_BInitEWExt = TRUE;
        g_nReqPatCount = 0;
        if(g_hSemVodCallback == (DWORD)NULL)
        {
           // g_hSemVodCallback = CSCreateSemaphore("200vodCallback",0,1);
           CSUDIOSSemCreate("200vodCallback",0,1, &g_hSemVodCallback);
            CSASSERT(g_hSemVodCallback != (DWORD)NULL);
        }
    }
	return TRUE;
}

/*****************************************************************************
* 函 数	名:	CSEW200_Init
* 功能描述:	创建EW200锁频消息队列、互斥量、任务
* 作	者:	
* 创建日期:	
* 输入参数:	void
* 输出参数:	无	
* 返 回	值: void
* 修改记录: SongLuYu(904595),2010.09.10
*****************************************************************************/
void CSEW200_Init(void)
{
	 CSUDI_Error_Code   hEW200MsgQueue = NULL;
	CSUDI_Error_Code   hEW200Mutex = NULL;
	CSUDI_Error_Code   hTask = NULL;
    //创建消息队列
	hEW200MsgQueue = CSUDIOSMsgQueueCreate("EW200MsgQueue", 64, sizeof(EW200MSGINFO), &g_hMsgQueue);
	CSASSERT(CSUDI_SUCCESS == hEW200MsgQueue);

    //创建互斥量，不立即加锁
	hEW200Mutex = CSUDIOSMutexCreate("EW200Mutex", 0, &g_hMutex);
	CSASSERT(CSUDI_SUCCESS ==  hEW200Mutex);

    //创建任务
	hTask = CSUDIOSThreadCreate("EW200Thread", 5*17, 1024*10, (CSUDIOSThreadEntry_F)CSEW200PlayVODThread, NULL, &g_hTask);
	CSASSERT(CSUDI_SUCCESS ==  hTask);
}



/*****************************************************************************
* 函 数	名:	CSEW200PlayVODThread
* 功能描述:	创建EW200锁频任务时的回调函数处理
* 作	者:	
* 创建日期:	
* 输入参数:	void
* 输出参数:	无	
* 返 回	值: void
* 修改记录: SongLuYu(904595),2010.09.10
*****************************************************************************/
static void CSEW200PlayVODThread(void)
{
	EW200MSGINFO szEW200MsgInfo = {0};
    
	while(1) 
	{
	    //判断从消息队列接收一条消息是否成功
		if(CSUDI_SUCCESS == CSUDIOSMsgQueueReceive(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0xffffffff))
		{
			switch(szEW200MsgInfo.udwMsg)
			{
    			case MSG_EW200_START_PAT:
    				{    
                         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext]------------ start PAT  ------------\r\n");
                        if(g_nReqPatCount < MAX_REQUST_PAT_PMT_COUNT)
                        {
    				        CSGTVODStartPAT();
                        }
    				}
    				break;

    			case MSG_EW200_START_PMT:
    				{
                        //等待互斥量
                         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext]------------ start PMT  ------------\r\n");
    					CSUDIOSMutexWait(g_hMutex, 0xffffffff);
                        CSGTVODStartPMT(szEW200MsgInfo.udwlParam, szEW200MsgInfo.udwwParam);

                        //释放互斥量
    					CSUDIOSMutexRelease(g_hMutex);
    				}
    				break;

    			case MSG_EW200_START_PLAY_VOD:
    				{
                        //等待互斥量
                         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext]------------ start PLAY VOD  ------------\r\n");
    					CSUDIOSMutexWait(g_hMutex, 0xffffffff);
                      CSStartplayVOD();
                        
                        //释放互斥量
    					CSUDIOSMutexRelease(g_hMutex);
    				}
				CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext]------------ start PLAY VOD  end------------\r\n");
    				break;

    			default:
    				break;
			}
		}
		else
		{
			CSUDIOSThreadSleep(200);
		}
	}
	
	return;
}


/*****************************************************************************
* 函 数	名:	CSGTVODStartPMT
* 功能描述:	请求PMT
* 作	者:	
* 创建日期:	
* 输入参数:	WORD ServiceID
* 输出参数:	无	
* 返 回	值: void
* 修改记录: SongLuYu(904595),2010.09.10
*****************************************************************************/
static void CSGTVODStartPMT(WORD ServiceID, WORD PMTPID)
{
	RequestInfo	pmtinfo;	
    
    CSCancelPMTSection();

	memset(&pmtinfo, 0, sizeof(pmtinfo));
	pmtinfo.dwPID = PMTPID;
	pmtinfo.bCRCCheck = ENABLE;
	pmtinfo.enmPriority = SECTION_PRIORITY_HIGH;
	pmtinfo.lTimeout = 2000;
	pmtinfo.MMFilter.match[0] = PMT_TABLE_ID;
	pmtinfo.MMFilter.mask[0] = 0xff;
	pmtinfo.MMFilter.match[1] = (BYTE) (ServiceID >> 8) & 0x00ff;
	pmtinfo.MMFilter.mask[1] = 0xff;
	pmtinfo.MMFilter.match[2] = (BYTE) (ServiceID & 0x00ff);
	pmtinfo.MMFilter.mask[2] = 0xff;
	g_PMThandle = Section2CSectionRequest(&pmtinfo, CSGTVODPmtNotify);
	if(g_PMThandle == (DWORD)NULL)
	{
		 CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL, "\r\n[ParsePat]Request PMT section failure.\r\n");
        EW200MSGINFO szEW200MsgInfo = {0};
		szEW200MsgInfo.udwMsg = MSG_EW200_START_PAT;
		CSUDIOSMsgQueueSend(g_hMsgQueue, &szEW200MsgInfo, sizeof(EW200MSGINFO), 0);	
	}
	return;
}

void CSEW200CardChange(DWORD dwCardNumber,int nCardStatus)
{
    if(g_fnEW200Register.fnCallback != NULL)
    {
        CSUDIOSSemWait((CSUDI_HANDLE)g_hSemVodCallback, 2*1000);
        g_fnEW200Register.fnCallback(EM_EW_MSG_VOD_CARD_VALID, NULL, (void *)g_fnEW200Register.dwUserData);
        CSUDIOSSemRelease((CSUDI_HANDLE)g_hSemVodCallback);
    }
}

