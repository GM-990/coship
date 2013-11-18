/*

���ļ���Ҫʵ��EW200��ҪӦ����Ҫʵ�ֵĽӿ�

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
#define MSG_EW200_START_PAT	(WM_SVS_MSG + 170)//EW200����PAT������PAT�ص�����
#define MSG_EW200_START_PMT	(WM_SVS_MSG + 171)//EW200����PMT������PMT�ص�����
#define MSG_EW200_START_PLAY_VOD (WM_SVS_MSG + 172)//��ʼ����VOD


typedef struct _EW200CAllBack
{
    CSUDIEW200SCallback_F fnCallback;
    DWORD dwUserData;
}EW200CAllBack;


#define INVALID_SECTION_HANDLE NULL
#define INVALID_PID 0x1fff
#define MAX_REQUST_PAT_PMT_COUNT 6

//��ȡQamDomainInfo
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
@brief ��ȡ�ն���Ϣ�ӿ�

Eastwin200��ͨ���ýӿڻ�ȡ�ն˵����к�,���ܿ��ŵ���Ϣ,���ȡ��ҳ�����õ���Ϣ
@param[in] eType ��Ϣ����
@param[in] value eType���Ͷ�Ӧ��ֵ
@param[in] length valueֵ�ĳ���
@return ����ʵ����Ϣ�ĳ���,������-1
@note ��Ҫ���õ���Ϣ��CSUDIEW200SClientInfoType_Eö���е�[r]��ʶ
@note ֧��Coship����Enreach����ʱ��Ҫʵ�ָýӿ�

@code
//�ο�ʵ��(δ���ǻ�����������쳣)
//ע��:�������������value��ʾ�����ͼ��������в�ͬ���밴�ο�����ʵ��/ʹ�ü��ɡ�
if(eType == EM_UDIEW200S_AUDIOCHANNEL)
{
	//EM_UDIEW200S_AUDIOCHANNEL��Ӧ����������int��
	//valueΪint�ı������䳤��Ӧ��Ϊsizeof(int)
	
	value = EM_UDIEW200S_AUD_CHANNEL_STEREO;
	CSUDIEW200SReadClientInfo(eType , (void*)&value, sizeof(int));

}
else if(eType == EM_UDIEW200S_SN)
{
	//EM_UDIEW200S_SN��Ӧ����������CSUDI_UINT8����
	//valueһ��ָ��CSUDI_UINT8���飬�䳤��Ϊlength
	//��valueָ���buffer�л�ȡ��Ӧ����
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
    case EM_UDIEW200S_CARDID:          ///< [r]���ܿ�����,char*
        nLength = strlen("710092200010");
        if(length>=nLength)
        {
            strncpy(value,"710092200010",nLength);
        }
        break;
	case EM_UDIEW200S_SN:              ///< [r]���к�,char*
	    nLength = strlen("710092200010");
	    if(length>=nLength)
        {
            strncpy(value,"710092200010",nLength);
        }
        break;
	case EM_UDIEW200S_PROVIDER:        ///< [r]�ն����쳧��,char*
	    nLength = strlen("coship");
        if(length>=nLength)
        {
            strncpy(value,"coship",nLength);
        }
        break;
	case EM_UDIEW200S_ALIAS:           ///< [r]�ն˱���,char*
        nLength = strlen("710092200010");
	    if(length>=nLength)
        {
            strncpy(value,"710092200010",nLength);
        }
        break;
    case EM_UDIEW200S_MODULE:          ///< [r]�ն��ͺ�,char*
        nLength = strlen("N5200");
	    if(length>=nLength)
        {
            strncpy(value,"N5200",nLength);
        }
        break;

    case EM_UDIEW200S_LOCAL_IP:        ///< [r]����IP��ַ,char*,��ȡʱ��ʽΪ"192.168.1.1"	
        nLength = strlen("192.168.1.101");
	    if(length>=nLength)
        {
            strncpy(value,"192.168.1.101",nLength);
        }
        break;

    case EM_UDIEW200S_LOCAL_MAC:       ///< [r]����Mac��ַ,char*,��"00AABBCCDDEE"
         nLength = strlen("00AABBCCDDEE");
	    if(length>=nLength)
        {
            strncpy(value,"00AABBCCDDEE",nLength);
        }
        break;

    case EM_UDIEW200S_CITV_PSD:        ///< [r]CITV VOD�û�����,char*
        nLength = strlen("111111");
	    if(length>=nLength)
        {
            strncpy(value,"111111",nLength);
        }
        break;

    case EM_UDIEW200S_CITV_USERID:     ///< [w][r]CITV VOD�û���,��һ�λ�ȡ��ȫ0,ͨ����֤��CITV3�ᷴ����ֵ,�뽫��洢��ROM��,�Ա��´�ʹ��,char*
        nLength = strlen(g_chUserID);
        if(length>=nLength)
        {
            strncpy(value,g_chUserID,nLength);
        }      
        break;

    case EM_UDIEW200S_CITV_TOKEN:      ///< [w][r]CITV ����,��һ�λ�ȡ��ȫ0,ͨ����֤��CITV3�ᷴ����ֵ,�뽫��洢��ROM��,�Ա��´�ʹ��,char*
        nLength = strlen(g_chToken);
        if(length>=nLength)
        {
            strncpy(value,g_chToken,nLength);
        }      
        break;

    case EM_UDIEW200S_CITV_SERVERIP:   ///< [r]CITV ��֤������IP��ַ,char*
        nLength = strlen("172.30.42.1");
	    if(length>=nLength)
        {
            strncpy(value,"172.30.42.1",nLength);
        }
        break;

    case EM_UDIEW200S_CITV_SERVERPORT: ///< [r]CITV ��֤�������˿ں�,int,��8080
        {
            int port = 8080;
            CSASSERT(length >= sizeof(int));
            memcpy(value,&port,sizeof(int));
            nLength = sizeof(int);
        }
        break;

    case EM_UDIEW200S_CITV_VIDEOTYPE:  ///< [r]֧������Ƶ�����ʽMPEG-2����:"MPEG-2-SD" ;MPEG-2����:"MPEG-2-HD";H.264����: "H.264-SD";H.264����: "H.264-HD" ; ȱʡΪMPEG-2-SD;ͬһ��ʽ�������¼��ݱ��壬���Բ���������;�ն˿ɽ����ʽ�������ж�����á�/���ָ���
        nLength = strlen("MPEG-2-SD");
        if(length>=nLength)
        {
            strncpy(value,"MPEG-2-SD",nLength);
        }        
        break;

    case EM_UDIEW200S_CITV_BROWSERVER: ///< [r]������İ汾�ţ�Ӧ�ø���������ǰ��Э��
	 nLength = strlen("coship1.0");
        if(length>=nLength)
        {
            strncpy(value,"coship1.0",nLength);
        }        
		break;

    case EM_UDIEW200S_ISMUTE:          ///< [r][w]����״̬,CSUDI_BOOL,CSUDI_TRUE��ʾ��ǰΪ����״̬,CSUDI_FALSE��ʾΪ�Ǿ���״̬
        {       
            memcpy(value,&g_nMute,sizeof(int));
            nLength = sizeof(int);
        }
        break;

    case EM_UDIEW200S_VOLUME:          ///< [r][w]����,int,[0,31],0Ϊ����,31���
        {	
            memcpy(value,&g_nVolume,sizeof(int));
            nLength = sizeof(int);
        }
        break;

    case EM_UDIEW200S_AUDIOPID:        ///< [r][w]��Ƶpid,int,
    	{
         memcpy(value,&g_program.audio[0].aud,sizeof(WORD));
         nLength = sizeof(int);
    	}
        break;
    case EM_UDIEW200S_MATCHMETHOD:     ///< [r][w]���ڵ�pan scan�ȸ�ʽ,CSUDIEW200SMatchMethod_E
        {
	 CSUDIVIDEOMatchMethod_E eMatchmethod;
	 CSUDIVIDEOGetMatchMethod (0,0, &eMatchmethod);
         memcpy(value,&eMatchmethod,sizeof(WORD));
         nLength = sizeof(int);
    	}  
        break;

    case EM_UDIEW200S_STOPMODE:        ///< [r][w]��̨ʱ��Ƶ�Ĵ�������:CSUDIEW200SStopMode_E
        {
	    if(CSUDI_SUCCESS == CSUDIVIDEOGetStopMode (0, &g_nStopMode))
	    	{
	    		memcpy(value,&g_nStopMode,sizeof(int));
            		nLength = sizeof(int); 
	    	}                       
        }
        break;

    case EM_UDIEW200S_PROGRAMEINFO:    ///< [r] ��Ŀ��Ϣ,CSUDIEW200SProgrameInfo_S
        break;

        default:
        CSASSERT(0);
        break;

    }

    return nLength;
}

/**
@brief �����ն���Ϣ�ӿ�

Eastwin200��ͨ���ýӿڽ�һЩ�û���Ϣ���õ��ն����ô洢����Ӧ���Լ��洢����
@param[in] eType ��Ϣ����
@param[in] value eType���Ͷ�Ӧ��ֵ
@param[in] length valueֵ�ĳ���
@return ����ʵ�ʴ洢��Ϣ�ĳ���,������-1
@note ��Ҫ���õ���Ϣ��CSUDIEW200SClientInfoType_Eö���е�[w]��ʶ
@note ֧��Coship����Enreach����ʱ��Ҫʵ�ָýӿ�
@note �ο�ʵ��ͬCSUDIEW200SReadClientInfo
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
    case EM_UDIEW200S_CITV_USERID:     ///< [w][r]CITV VOD�û���,��һ�λ�ȡ��ȫ0,ͨ����֤��CITV3�ᷴ����ֵ,�뽫��洢��ROM��,�Ա��´�ʹ��,char*
        if(length<sizeof(g_chUserID))
        {
            strncpy(g_chUserID,value,length);
        }      
        nLength = length;
        break;

    case EM_UDIEW200S_CITV_TOKEN:      ///< [w][r]CITV ����,��һ�λ�ȡ��ȫ0,ͨ����֤��CITV3�ᷴ����ֵ,�뽫��洢��ROM��,�Ա��´�ʹ��,char*
        if(length<sizeof(g_chToken))
        {
            strncpy(g_chToken,value,length);
        }      
        nLength = length;
        break;

    case EM_UDIEW200S_CITV_INMUSICSHOW:///< [w][r]�Ƿ��������ģʽ,"TRUE":����ģʽ "FALSE":������ģʽ.EW200���յ�"ITVRTSPURL"�ؼ���ʱ,��������ģʽ;���յ�"ITVControl"�ؼ���,ֵΪ"ITVStop"ʱ�˳�����ģʽ
        break;

    case EM_UDIEW200S_ISMUTE:          ///< [r][w]����״̬,CSUDI_BOOL,CSUDI_TRUE��ʾ��ǰΪ����״̬,CSUDI_FALSE��ʾΪ�Ǿ���״̬
     	{
		g_nMute =  *(int *)value;
		if(g_nMute == CSUDI_TRUE)
			CSUDIAUDIOMute (0);
		else
			CSUDIAUDIOUnmute (0);
		nLength = sizeof(int);   
    	}		
        break;

    case EM_UDIEW200S_VOLUME:          ///< [r][w]����,int,[0,31],0Ϊ����,31���
        {   
         g_nVolume = *(int *)value ;
	 CSUDIAUDIOSetVolume(0, g_nVolume);
            nLength = sizeof(int);        
        }   
        break;
    case EM_UDIEW200S_AUDIOCHANNEL:    ///< [w]����,CSUDIEW200SAUDChannelType_E
        {
            int nTrack = *(int *)value;
            CSUDIAUDIOSetChannel (0, nTrack);
            nLength = sizeof(int);
        }
        break;
    case EM_UDIEW200S_AUDIOPID:        ///< [r][w]��Ƶpid,int,
        g_program.audio[0].aud = *(int *)value;            
        nLength = sizeof(int);     
        break;

    case EM_UDIEW200S_VIDEOALPHA:      ///< [w] ���ڵ�͸���ȣ�int,[0,100],0Ϊ��ȫ͸����100Ϊ��͸��
        {
            int nOSDAlpha = *(int *)value;   
            if(nOSDAlpha >=0&&nOSDAlpha<=100)
            {
                CSUDISCREENSetOSDTransparency (nOSDAlpha);
                nLength = sizeof(int);
            }
        }
        break;

    case EM_UDIEW200S_BRIGHTNESS:      ///< [w]��Ƶ���ȣ�int,[0,100],0Ϊȫ�ڣ�100Ϊ�������
        {
            int nOSDBrightness = *(int *)value;             
            if(nOSDBrightness >=0&&nOSDBrightness<=100)
            {
                CSUDISCREENSetBrightness (nOSDBrightness);
                nLength = sizeof(int);
            }
        }
        break;

    case EM_UDIEW200S_CONTRAST:        ///< [w]��Ƶ�Աȶȣ�int,[0,100],0Ϊȫ�ڣ�100Ϊ�Աȶ����
        {
            int nOSDContrast = *(int *)value;             
            if(nOSDContrast >=0&&nOSDContrast<=100)
            {
                CSUDISCREENSetContrast (nOSDContrast);
                nLength = sizeof(int);
            }
        }        
        break;

    case EM_UDIEW200S_SATURATION:      ///< [w]��Ƶ���Ͷȣ�int,[0,100],0Ϊȫ�ڣ�100Ϊ���Ͷ����
        {
            int nOSDChroma = *(int *)value;             
            if(nOSDChroma >=0&&nOSDChroma<=100)
            {
                CSUDISCREENSetSaturation (nOSDChroma);
                nLength = sizeof(int);
            }
        } 
        break;

    case EM_UDIEW200S_ASPECTRATIO:     ///< [w]�����߱�,CSUDIEW200SAspectRatio_E
        {
            int nAspectRatio = *(int *)value;  
            CSUDIVIDEOSetAspectRatio (0, 0, nAspectRatio);
            nLength = sizeof(int);
        } 
        break;

    case EM_UDIEW200S_MATCHMETHOD:     ///< [r][w]���ڵ�pan scan�ȸ�ʽ,CSUDIEW200SMatchMethod_E
        {
            int nAutoApdat = *(int *)value;  
                CSUDIVIDEOSetMatchMethod (0, 0, nAutoApdat);
                nLength = sizeof(int);
        }
        break;

    case EM_UDIEW200S_VIDEOFORMAT:     ///< [w]��Ƶ�����ʽ��CSUDIEW200SVdisVideoFormat_E   
        //��ɽǿ��Pal;
        nLength = sizeof(int);
        break;

    case EM_UDIEW200S_STOPMODE:        ///< [r][w]��̨ʱ��Ƶ�Ĵ�������:CSUDIEW200SStopMode_E
        //��ɽǿ�ƺ���;
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
@brief ͨ��DVB3Ҫ�ز�ѯ����Դ��Ϣ

������ȡ��3Ҫ�ض�Ӧ��Ϣʱ����ʹ�õ�TunerId,DemuxId,��Ƶ��
@param[in] nOrNetId org network id
@param[in] nTsId ts id
@param[in] nServiceId service id
@param[out] psSourceParam ���ڴ洢���������Դ��Ϣ
@return ����ɹ�����CSUDI_SUCCESS, ʧ�ܷ��ش������
@note EW200ͨ���ýӿڲ�ѯһ��Service�Ķ�Ӧ��Ƶ�㼰��Ƶͷ��Ϣ����ƽ̨�ж����Ƶͷ������ӿڵ�ʵ��Ҫ�ر�С�ģ���Ϊ��Ӧ�����ݿ����ڲ�ͬ�ĸ�Ƶͷ�ϣ�����򵥲�ѯ���ݹ�����
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
@brief ��ȡIPQam����Ϣ�ӿ�

������VODģ��
��֤ģ��ͨ�������ն�ʵ�ֵĸýӿڵõ�IPQam������Ϣ
- CITV3.x�汾��VODĿǰȡ����ָ��Section�ĵ�4��5�ֽ�(��data[3],data[4])
@param[in] punDeliverList �����Ϣ����Ƶ���б�����
@param[in] nDeliverCount pDeliverList�����е�Ƶ�����
@param[in] nPid IPQam����Ϣ����Pid
@param[in] ucTableId IPQam����Ϣ����TableId
@param[out] pIpQamDataBuffer ���ڴ��IPQam����Ϣ���ڴ�ָ��
@param[in] nBufferLength pIpQamDataBuffer�ĳ���
@param[in] pvReserved �����ֶ�,Ŀǰ����ΪNULL
@return �ɹ���ȡ����TRUE,ʧ�ܷ���FALSE
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
            bRet = FALSE;//CSTunerConnectSource��һ���첽�Ĺ���
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

    if (event != SECTION_TIMEOUT) //��ʱ��CancelLoad
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
@brief ���Ž�Ŀ�ӿ�

�����ƶ�URL�ĸ�ʽ���в���

@param[in] pcURL ����URL
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@code ����URL�ĸ�ʽ�����¼��֣�Ӧ�ÿ��Ը��ݲ�ͬ��URL��ʽ���в��Ŵ���
(1)dvb://OriginalNetworkId.TS_Id.ServiceId  ����ο������ֵ����м�������淶����3���ָ�¼I��1.2.1С�ڡ�����:dvb://1.2.5(Ĭ��ʮ������)
(2)dvbc://Frequency.SymbolRate.Modulation.ServiceId
		Frequency:Ƶ��.��λMHz
		SymbolRate:�����ʣ���λ��Kbps
		Modulation:����ģʽ
		ServiceId:��ĿId
		����: deliver://490000000:6875:64:10(ServiceIdΪʮ������)
(3)avpid://pcrPid.emm.videoPid.audioPid.audio_ecm_pid.video_ecm_pid(ʮ������)	
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
@brief ֹͣ��Ŀ���Žӿ�

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
*/
void CSUDIEW200SStopPlay(int nPlayId)
{
    CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext] STOP Play \r\n");
    CSUDIPLAYERStop(g_hPlayer);
    CSUDIPLAYERClose(g_hPlayer);
    return ;
}

/**
@brief ������Ƶ���Ŵ��ڴ�С�ӿ�

@param[in] psRect ��Ƶ���δ�С,ΪNULL��ʾȫ������
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
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
@brief ע��UDI����Ϣ�ص�����

ע���UDI����ص���Ϣ��ͨ���ýӿ�֪ͨ��Ӧ��
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@param[in] pUserData �û�����,���ڻص���ԭ������
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_TRUE
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
@brief ɾ��UDI��ص�������ע��

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@param[in] fnTunerCallback �ص��������
@param[in] pUserData ͬCSUDIEW200SAddCallback�е�pUserData
@return�ɹ�����CSUDI_SUCCESS ����ûص�����������û��ע�ᣬ������CSUDIEW200S_ERROR_NO_CALLBACK
*/
CSUDI_Error_Code CSUDIEW200SDelCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata)
{
	return CSUDI_SUCCESS;
}

/****************************************************/
//                  
/**
��Ƶ�ص�
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
����PAT
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
PAT �ص�
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
PMT �ص�
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
PAT����
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
		    //��������PMT����Ϣ
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
PMT����
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
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //��ȡ����Ƶ������������
	{
		for (nAudioIndex = 0; nAudioIndex<nAudioCount; nAudioIndex++)
		{
			if(nAudioType == EM_UDI_AUD_STREAM_AC3)//AC3�ż��bypassģʽ
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
			
	if (CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)) //��ȡ����
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
	
	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //��ȡ��demux������
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //ȡ���ӦID=i ��demux������
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

//���ý�Ŀ��Ϣ
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
* �� ��	��:	CSEW200_Init
* ��������:	����EW200��Ƶ��Ϣ���С�������������
* ��	��:	
* ��������:	
* �������:	void
* �������:	��	
* �� ��	ֵ: void
* �޸ļ�¼: SongLuYu(904595),2010.09.10
*****************************************************************************/
void CSEW200_Init(void)
{
	 CSUDI_Error_Code   hEW200MsgQueue = NULL;
	CSUDI_Error_Code   hEW200Mutex = NULL;
	CSUDI_Error_Code   hTask = NULL;
    //������Ϣ����
	hEW200MsgQueue = CSUDIOSMsgQueueCreate("EW200MsgQueue", 64, sizeof(EW200MSGINFO), &g_hMsgQueue);
	CSASSERT(CSUDI_SUCCESS == hEW200MsgQueue);

    //����������������������
	hEW200Mutex = CSUDIOSMutexCreate("EW200Mutex", 0, &g_hMutex);
	CSASSERT(CSUDI_SUCCESS ==  hEW200Mutex);

    //��������
	hTask = CSUDIOSThreadCreate("EW200Thread", 5*17, 1024*10, (CSUDIOSThreadEntry_F)CSEW200PlayVODThread, NULL, &g_hTask);
	CSASSERT(CSUDI_SUCCESS ==  hTask);
}



/*****************************************************************************
* �� ��	��:	CSEW200PlayVODThread
* ��������:	����EW200��Ƶ����ʱ�Ļص���������
* ��	��:	
* ��������:	
* �������:	void
* �������:	��	
* �� ��	ֵ: void
* �޸ļ�¼: SongLuYu(904595),2010.09.10
*****************************************************************************/
static void CSEW200PlayVODThread(void)
{
	EW200MSGINFO szEW200MsgInfo = {0};
    
	while(1) 
	{
	    //�жϴ���Ϣ���н���һ����Ϣ�Ƿ�ɹ�
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
                        //�ȴ�������
                         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext]------------ start PMT  ------------\r\n");
    					CSUDIOSMutexWait(g_hMutex, 0xffffffff);
                        CSGTVODStartPMT(szEW200MsgInfo.udwlParam, szEW200MsgInfo.udwwParam);

                        //�ͷŻ�����
    					CSUDIOSMutexRelease(g_hMutex);
    				}
    				break;

    			case MSG_EW200_START_PLAY_VOD:
    				{
                        //�ȴ�������
                         CSDebug("TEST_UDI_EW200SERVICE",ERROR_LEVEL,"[CS_EW200Ext]------------ start PLAY VOD  ------------\r\n");
    					CSUDIOSMutexWait(g_hMutex, 0xffffffff);
                      CSStartplayVOD();
                        
                        //�ͷŻ�����
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
* �� ��	��:	CSGTVODStartPMT
* ��������:	����PMT
* ��	��:	
* ��������:	
* �������:	WORD ServiceID
* �������:	��	
* �� ��	ֵ: void
* �޸ļ�¼: SongLuYu(904595),2010.09.10
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

