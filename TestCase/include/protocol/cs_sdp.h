/******************************************************************************
�ļ����ƣ�cs_sdp.h
��Ȩ����(c) 2006�� ������ͬ�޵��ӹɷ����޹�˾��

��ǰ�汾��Ver 1.0    
��    �ߣ�

����ժҪ��
	sdpģ���ṩ���ⲿ�ӿ��ļ���

�޸ļ�¼��
�汾��		����		����			�޸�����
--------	----------	----------		------------
1.0			2006-2-27	������	     �����޸Ľӿ������Ű�
1.1         2006-6-22   ������       ��ֲ��PORTING��
*******************************************************************************/

#ifndef __CS_SDP_H__
#define __CS_SDP_H__
/******************************************************************************
*                                 ͷ�ļ�����                                 *
******************************************************************************/
#include <stdio.h>		/* C��׼��ͷ�ļ�*/

#ifdef PLATFORM_ST40
#include <time.h>
#else
#ifdef PLATFORM_ST20
#include "cs_clocktimer.h"
#else
#ifndef WIN32
#include <sys/time.h>
#endif
#endif
#endif

#include "mmcp_typedef.h"
#include "cs_clocktimer.h"
/******************************************************************************
*                                 �ꡢ���塢�ṹ��                           *
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#if defined(PLATFORM_ST40) || defined(PLATFORM_ST20)
typedef unsigned long  SDPUINT64;
typedef long  SDPINT64;
#else
typedef unsigned long long  SDPUINT64;
typedef long long  SDPINT64;
#endif


#define SDP_MAX_REPEAT_OFFSETS 16

#ifndef SDP_NTP_TO_UNIX_TIME
#define SDP_NTP_TO_UNIX_TIME 2208988800UL
#endif

typedef enum CSSDPLogLevel_E
{
	EM_SDP_LEVEL_INFO = 1,   /*һ��*/
    EM_SDP_LEVEL_WARN,       /*����*/
    EM_SDP_LEVEL_ERROR      /*����*/
} CSSDPLogLevel_E; 


typedef enum CSSDPRet_E
{
	EM_SDP_RET_SUCCESS = 0,     	/*�ɹ�*/
    EM_SDP_RET_FAILURE,         	/*δ֪ԭ��ʧ��*/
    EM_SDP_RET_INVALPARA,       	/*������Ч*/
    EM_SDP_RET_NOMEM,           	/*�����ڴ�ʹ�����ƻ�ϵͳ�ڴ�������*/
    EM_SDP_RET_INVALVER,        	/*Э��汾��Ч*/
    EM_SDP_RET_UNKNOWN_LINE,    	/*SDP�������в���ʶ����*/
    EM_SDP_RET_INVALBANDWIDTH,  	/*��Ч����*/
	EM_SDP_RET_TIME,            	/* t= �����ֽ�������*/
	EM_SDP_RET_REPEAT,          	/* r= �����ֽ�������*/
	EM_SDP_RET_REPEAT_NOTIME,   	/*��ǰʱ��δ�� */
	EM_SDP_RET_TIME_ADJ,  	 		/* z= �����ֽ�������*/
	EM_SDP_RET_CONNECT,  			/* c= �����ֽ�������*/
	EM_SDP_RET_ORIGIN,  			/*o= �����ֽ������� */
	EM_SDP_RET_MEDIA,  				/*m= �����ֽ������� */
	EM_SDP_RET_KEY,  				/* k=  �����ֽ�������*/
	EM_SDP_RET_ATTRIBUTES_NO_COLON  /*�޷��ҵ�:�ָ�� */
} CSSDPRet_E;

/*��������ṹ*/
typedef struct CSSDPStringList_S
{
	struct CSSDPStringList_S *psNext;
	char   *pszStringVal;
} CSSDPStringList_S;

/*parses b=<modifier>:<value>*/
typedef enum CSSDPBandwidthModifier_E
{
	EM_SDP_BANDWIDTH_MODIFIER_NONE = 0,
	EM_SDP_BANDWIDTH_MODIFIER_CT,     
	EM_SDP_BANDWIDTH_MODIFIER_AS,     
	EM_SDP_BANDWIDTH_MODIFIER_USER   
} CSSDPBandwidthModifier_E;

/*
* These next 2 set of defines are used in CSSDPSessionDesc_t field conf_type
* and CSSDPMediaDesc_t field orient_type. They need to be in this order
* (0 for no value,user value at end),so the processing routine in
* sdp_decode.c can use an array lookup. See type_values,orient_values
* static variables and the check_value_list_or_user() routine.
* If you want to add a value,add before the user value,in the #define
* and in the static variable.
*/

#define CONFERENCE_TYPE_NONE 0
#define CONFERENCE_TYPE_BROADCAST 1
#define CONFERENCE_TYPE_MEETING 2
#define CONFERENCE_TYPE_MODERATED 3
#define CONFERENCE_TYPE_TEST 4
#define CONFERENCE_TYPE_H332 5
#define CONFERENCE_TYPE_OTHER 6

#define ORIENT_TYPE_NONE 0
#define ORIENT_TYPE_PORTRAIT 1
#define ORIENT_TYPE_LANDSCAPE 2
#define ORIENT_TYPE_SEASCAPE 3
#define ORIENT_TYPE_USER 4

typedef struct CSSDPBandwidth_S
{
	  struct CSSDPBandwidth_S *psNext;    /*������Ϣ������ָ��*/
	  CSSDPBandwidthModifier_E eModifier; /*������Ϣ�����࣬eg.CT��AS*/
	  double  Bandwidth;                  /*������Ϣ��ͨ����������*/
	  char    *pszUserBand;
} CSSDPBandwidth_S; 

typedef struct CSSDPCategoryList_S
{
	  struct CSSDPCategoryList_S *psNext;  /*�ỰĿ¼��Ϣ����ָ��*/
	  SDPUINT64 u64Category;                  /*�ỰĿ¼��Ϣ*/
} CSSDPCategoryList_S;


/*c=IN IP4 0.0.0.0 (���ӵ���Ϣ)*/
typedef struct CSSDPConnectDesc_S
{
	  char  *pszConnType;   /*"ip4"*/
	  char  *pszConnAddr;   /*conn_addr="0.0.0.0"*/
	  DWORD dwTTL;          /*ʹ���鲥��ַҲӦ��ָ��TTL������һ�������鲥���ݱ��ܹ����͵ķ�Χ��TTLֵ��0-255֮�䡣TTL��б�ߺ����*/
	  DWORD dwNumAddr;      /*��ʶ�鲥������*/
	  int   nIsUsed;        /*�Ƿ�ʹ�ñ�ʶ*/
} CSSDPConnectDesc_S;

/*��ʶ��Կ���������͸������߱�ʶ��ȡ��Կ���ⲿ����*/
typedef enum CSSDPKeyTypes_E
{
	EM_SDP_KEYTYPE_NONE = 0,  /*SDP�в�����Key�ֶ�*/
	EM_SDP_KEYTYPE_PROMPT,    /*Key�ֶβ�������Կ*/
	EM_SDP_KEYTYPE_CLEAR,     /*Key�ֶθ�����Կ����û���κα任*/
	EM_SDP_KEYTYPE_BASE64,    /*Key�ֶθ�����Կ������Base64����*/
	EM_SDP_KEYTYPE_URI        /*Key��������Կ����URI*/
} CSSDPKeyTypes_E;

typedef struct CSSDPKeyDesc_S
{
	CSSDPKeyTypes_E eKeyType; /*Key�ֶ�����*/
	char *pszKey;             /*Key�ֶ�ֵ*/
} CSSDPKeyDesc_S;

typedef struct CSSDPRtpmapDesc_S
{
	char  *pszEncodeName;     /*���뷽ʽ*/
	DWORD dwClockRate;        /*ʱ�Ӳ�����*/
	DWORD dwEncodeParam;      /*�����������Ƶû�н����������Ƶָ����������*/
} CSSDPRtpmapDesc_S ;


typedef struct CSSDPFormatList_S
{
	struct CSSDPFormatList_S *psNext;  /*���ý���ʽ������*/
	struct CSSDPMediaDesc_S  *psMedia; /*ָ��ṹCSSDPMediaDesc_S��ָ��*/
	char                     *pszFmt;  /*��ʽ����*/
	CSSDPRtpmapDesc_S        *psRtpmap;/*ִ�нṹCSSDPRtpmapDesc_S��ָ��*/
	char                     *pszFmtParam;/*eg:fmtp:96 profile-level-id=1; config=000001b003000001b50b��*/
                                          /*(����������96��profile������1����Ƶ��config��Ϣ)*/
}CSSDPFormatList_S;

/*eg:a=range:npt=0-4926.21294 (�Ự��0�뵽4926.21294��)*/
typedef struct CSSDPRangeDesc_S
{
	int     nHaveRange;       /*�����úú�ѱ��ֶ���Ϊtrue,ͬʱ���ֶα�ʶ����*/
							  /*��ֻ����һ�Σ������Ϊtrue�ˣ�������������򷵻�*/
	int     nRangeIsNpt;      /*�Ƿ�ʹ��NPT�����ʹ����Ϊture*/
	int     nRangeIsClock;	/*�Ƿ�ʹ��clock�����ʹ����Ϊture*/					  
	double  RangeStart;       /*��ʼʱ��*/
	double  RangeEnd;         /*������������Ŀ�Ľ���ʱ��*/
	WORD    wRangeSmpteFps;   /*ʹ��SMPTE��ʽ*/
	int     nRangeEndInfinite;/*���û�н���ʱ�䣬���ֶθ�ֵΪTRUE����ʾ���������*/
} CSSDPRangeDesc_S;

/* basic structure for definition of a media.*/
typedef struct  CSSDPMediaDesc_S
{
	struct CSSDPMediaDesc_S   *psNext;    /*�ж���ý��ʱ��������������Ƶ����Ƶ*/
	struct CSSDPSessionDesc_S *psParent;  /*ָ��ṹCSSDPSessionDesc_S��ָ��*/
	char *pszMediaName;                   /*media name*/
	char *pszProto;                       /*protocol used*/
	char *pszSdpLang;                      
	char *pszLang;
	char *pszMediaDesc;                   /*description string*/
	char *pszControlString;               /*rtsp control string*/
	CSSDPFormatList_S *psMediaFmt;        /*ý���ʽ������ж��֣��������ʶ*/
	CSSDPStringList_S *psUnparsedLines;   /*û�н���������Э�飬�����������*/
	int    nRecvonly;
	int    nSendrecv;
	int    nSendonly;
	WORD   wIpPort;                       /*ip port*/
	WORD   wNumOfPorts;                   /*number of ports*/
	DWORD  dwPtime;
	int    nPtimePresent;
	DWORD  dwQuality;
	int    nQualityPresent;
	double Framerate;
	int    nFrameratePresent;
	CSSDPConnectDesc_S sMediaConnect;
	CSSDPRangeDesc_S   sMediaRange;
	CSSDPBandwidth_S   *psMediaBandwidth;
	int                nOrientType;
	char               *pszOrientUserType;
	CSSDPKeyDesc_S     sKey;
	SDPUINT64 u64Xcreatedate;/*a=X-createdate:1123736310*/
	char   *pszXConttype; /*a=X-conttype:m2t*/
	char   *pszXClip;     /*a=:X-clipa1*/
	char   *pszXName;     /*a=X-name:/0811test2*/	
	double XDuration;     /*a=X-duration:2666.440*/	
	double XBitrate;      /*a=X-bitrate:3100000.000*/ 
} CSSDPMediaDesc_S;

typedef struct CSSDPTimeAdjDesc_S
{
	  struct CSSDPTimeAdjDesc_S *psNext;  
	  cstime_t adj_time;       				 /*��ʼת����ʱ���*/
	  int nOffset;           				 /*ʱ��ƫ����*/
} CSSDPTimeAdjDesc_S;

typedef struct CSSDPTimeRepeatDesc_S
{
	  struct CSSDPTimeRepeatDesc_S *psNext;  /*�ظ�ʱ��������Ϣ����ָ��*/
	  DWORD dwRepeatInterval;                /*�ظ�ʱ����*/
	  DWORD dwActiveDuration;                /*�Ự����ʱ��*/
	  DWORD dwOffsetCnt;                     /*ʱ��ƫ��ֵ�ĸ���*/
	  DWORD dwOffsets[SDP_MAX_REPEAT_OFFSETS];   /*ʱ��ƫ��ֵ����*/
} CSSDPTimeRepeatDesc_S;

/*t=0 0 (����Ự��ʼ��ʱ����0��������ʱ��Ҳ��0������һ����������)*/
typedef struct CSSDPSessionTimeDesc_S
{
	struct CSSDPSessionTimeDesc_S *psNext; /*ʱ����Ϣ����ָ��*/
	cstime_t start_time;                     /*��ʼʱ��*/
	cstime_t end_time;                       /*����ʱ��*/
	CSSDPTimeRepeatDesc_S *psRepeat;       /*�Ự�ظ�ʱ�������ṹָ��*/
} CSSDPSessionTimeDesc_S;


/*�Ự�����ṹ*/
typedef struct CSSDPSessionDesc_S
{
	struct  CSSDPSessionDesc_S *psNext;      /*session����ָ��*/
	/* o= fields */
	char   *pszOrigUsername;                 /*�û���¼��*/
	SDPUINT64 u64SessionId;                     /*�Ựid*/
	SDPUINT64 u64SessionVersion;                /*�Ự�汾��*/
	char   *pszCreateAddrType;               /*IP���ͣ�IP4/IP6*/
	char   *pszCreateAddr;                   /*�����Ự������IP��ַ */
	CSSDPCategoryList_S *psCategoryList;     /*�����б�û�б����*/
	/* s= field */
	char   *pszSessionName;                  /*�Ự����*/
	/* i= field */
	char   *pszSessionDesc;                  /*�Ự��Ϣ*/
	/* u = field */
	char   *pszURI;                          /*������URI*/
	/* Administrator Information */
	CSSDPStringList_S *psAdminPhone;         /*�绰����, �����ж��*/
	CSSDPStringList_S *psAdminEmail;         /*email��ַ, �����ж��*/
	/* connect info */
	CSSDPConnectDesc_S sSessionConnect;      /*�Ự������Ϣ*/
	CSSDPRangeDesc_S sSessionRange;         /*ӰƬ���ŷ�Χ*/
	CSSDPBandwidth_S *psSessionBandwidth;    /*�Ự������Ϣ*/
	/* key type */
	CSSDPKeyDesc_S sKey;                     /*Key�ֶ���Ϣ*/
	char *pszKeywds;                       	 /*�Ự�Ĺؼ���*/
	char *pszTool;                         	 /*�����Ự�����Ĺ��ߵ����ƺͰ汾*/
	char *pszCharset;                        /*��ʾ�Ự���ơ���Ϣ���ַ���*/
	char *pszSdplang;                        /*�Ự����������*/
	char *pszLang;                         	 /*�Ự��Ĭ������*/
	char *pszControlString;                  /*���Ե�URL*/ 
	char *pszEtag;                         
	/* start/end times,in an array form */ 
	CSSDPSessionTimeDesc_S *psTimeDesc;      /*��Ų��ŵĿ�ʼ�����ʱ��*/
	CSSDPTimeAdjDesc_S *psTimeAdjDesc;  
	/* media descriptions */
	CSSDPMediaDesc_S *psMedia;               /*���ý��������Ϣ*/
	/* unparsed lines */
	CSSDPStringList_S *psUnparsedLines;      /*���δ����������*/
	int  nConfType;                          /*�Ự������Ϣ����*/
	char *pszConfTypeUser;                   /**/
	int  nRecvonly;                          /*ֻ����*/
	int  nSendrecv; 						 /*���պͷ���*/
	int  nSendonly;                          /*ֻ����*/
} CSSDPSessionDesc_S;

//typedef struct sdp_decode_info_ CSSDPDecodeInfo_S;
typedef struct CSSDPDecodeInfo_S
{
	int   nMode;              /*�ļ���ʽΪ0�� �ڴ淽ʽΪ1���������봦��*/
	const char * pszMemptr;   /*ָ��SDP�����ڴ�*/
	FILE  *pFile;             /*�ļ�ָ��*/
} CSSDPDecodeInfo_S;

/******************************************************************************
��������:	CSSDPDecode
��������:
    ������ĽṹCSSDPDecodeInfo_S��ȡ��SDP���ݣ������н�����
    �����а�����RFC2326��Ҫ������������֡�
    
�������:
    psDecodeInfo������SDP��������ݽṹ��
    
�������:
    ppsSessionList������ĻỰ�б�
	pnSessionInfoNum���Ự�б��лỰ��Ϣ����

�� �� ֵ:
    �����ɹ�������EM_SDP_RET_SUCCESS��
	����ʧ�ܣ��������������롣
	
����˵��:

******************************************************************************/
int CSSDPDecode ( CSSDPDecodeInfo_S  *psDecodeInfo,
				  CSSDPSessionDesc_S **ppsSessionList,
							    int  *pnSessionInfoNum);

/******************************************************************************
��������:	CSSDPFreeSessionDesc
��������:
    �ͷ�SDP�Ự�ṹCSSDPSessionDesc�и��ֶ�������ڴ档
    
�������:
    psSessionDesc��SDP��������ݽṹ��CSSDPSessionDesc��ָ�롣
    
�������:
    ��
    
�� �� ֵ:
    ��
    
����˵��:
******************************************************************************/
void CSSDPFreeSessionDesc (CSSDPSessionDesc_S *psSessionDesc);

/******************************************************************************
��������:	CSSDPSetLogLevel
��������:
    ����SDPģ���ӡ�ȼ�,ֻ�д��ڵ���nLogLevel����־���ܱ���ӡ
    
�������:
    nLogLevel:��ӡ�ȼ�,�����CSSDPLogLevel_E

�������:
	��
	
�� �� ֵ:
	��
	
����˵��:
******************************************************************************/
void CSSDPSetLogLevel(int nLogLevel);


#ifdef __cplusplus
}
#endif

#endif 


