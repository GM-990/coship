/******************************************************************************
�ļ����ƣ�cs_rtsp_client.h
��Ȩ����(c) 2006�� ������ͬ�޵��ӹɷ����޹�˾��

��ǰ�汾��Ver 1.0    
��    �ߣ�

����ժҪ��
	rtspģ���ṩ���ⲿ�ӿ��ļ���

�޸ļ�¼��
�汾��		����		����			�޸�����
--------	----------	----------		------------
1.0			2006-2-11	������	�����޸Ľӿ������Ű�
1.1         2006-6-22   ������  ��ֲ��PORTING��
2.0			2007-6-11	١����	�����з����ı���淶���¶���ӿڣ��������
*******************************************************************************/

#ifndef __RTSP_CLIENT_H__
#define __RTSP_CLIENT_H__ 

/******************************************************************************
 *                                 ͷ�ļ�����                                 *
 ******************************************************************************/
#include "mmcp_typedef.h"
/******************************************************************************
 *                                 �ꡢ���塢�ṹ��                           *
 ******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif


#define RTSP_RECV_BUF_LEN 		(5120)	/*���շ�������Ϣ�Ļ�����*/
#define RTSP_SEND_BUF_LEN 		(2048)     /*��װRTSP�����·�������*/
#define RTSP_URL_MAX_LEN		(1024)		/*url����󳤶�*/
/*���������͹�����Ϣ������*/
typedef enum  
{
	EM_RTSP_SENDORDER=0,		/*������������Ϣ�ɹ�*/
	EM_RTSP_ANNOUNCE,			/*�����������·�ANNOUNCE*/
	EM_RTSP_OPTIONS,			/*�����������·�OPTIONS*/
	EM_RTSP_GETPARAMETER,		/*�����������·�GETPARAMETER */
	EM_RTSP_SETPARAMETER,		/*�����������·�SETPARAMETER */
	EM_RTSP_REDIRECT,			/*�����������·�REDIRECT */
	EM_RTSP_RTPDATA,			/*�������·�rtp ���� */
    EM_RTSP_SEND_ERROR_ORDER	/*������������Ϣ����*/
} CSRTSPServerResponse_E;

typedef enum 
{
	EM_RTSP_TIME_NOFORMAT= 0,  /*û�б�ʾʱ�䷶Χ���ֶ�*/
	EM_RTSP_TIME_NPT,			 /*����nptʱ���ʽ*/
	EM_RTSP_TIME_SMPTE			/*����smpteʱ���ʽ*/
} CSRTSPTimeFormat_E;

/*��־��������������ʾС��ָ�������������־��*/
typedef enum CSRTSPLogLevel_E
{
	EM_RTSP_LOG_DEBUG = 0,
	EM_RTSP_LOG_INFO,
	EM_RTSP_LOG_NOTICE,
	EM_RTSP_LOG_WARNING,
	EM_RTSP_LOG_ERR,
	EM_RTSP_LOG_CRIT,
	EM_RTSP_LOG_ALERT,
	EM_RTSP_LOG_EMERG
} CSRTSPLogLevel_E;

/*������װ������������RTSP��Ϣ�Ľṹ*/
typedef struct 
{
	char *pszAccept;           /*ָʾ�ͻ�������ʶ��ı�ʾ�������ݵ�����*/
	char *pszAcceptEncoding;  /*ʾ�ͻ�������ʶ������ݱ���ĸ�ʽ����compress�����������û��Accept-Encoding��Ϣͷ������������Ϊ�ͻ��˿��Խ����κ����ݱ���ĸ�ʽ*/
	char *pszAcceptLanguage;  /*ָʾ��֧�ֵı���������ԭ��������������*/
	char *pszAcceptCharset;/* Accept-Charset: UTF-8*/
	char *pszAcceptAuthentication;/* X-Accept-Authentication: Basic, NTLM, Digest*/
	char *pszAuthorization;	/*��Ȩ���Ͽ�*/
	DWORD uBandwidth;       /*�����˿ͻ��˹����ܵõ������еĴ���*/
	DWORD uBlockSize;       /*�Ǵӿͻ��˷��͵�ý��������ģ�ѯ�ʷ�����ý����Ĵ�С�������ǰ��ֽڵ�������������Ĵ�С���������ײ�Э��(IP,UDP,RTP)Э��İ�ͷ��С*/
	char *pszCacheControl;/*Cache-Control: must-revalidate*/
	char *pszConference;
	char *pszConnection;
	char *pszFrom;
	char *pszProxyAuth;
	char *pszProxyRequire;
	char *pszRange;
	char *pszReferer;
	double Scale;/*���������Ʋ���/��¼�����ʵġ�����Ϊ1ʱ����ʾ��������/��¼���ʡ�����Ϊ2ʱ����ʾ����������ʡ�����Ϊ0.5ʱ����ʾ0.5���ʵ�����ͷ������Ϊ-2ʱ����ʾ�����Ŀ������ʡ�*/
	char *pszSession;/*���ѡȡ�ı䳤������8���ֽ�,���ɷ���������*/
	double Speed;/*�����������������ָ���Ĵ������ʽ����ݴ��͵��ͻ���,�ӷ�������������ý��������Ҫ����*/
	char *pszTransport;/*����������ý��Ĵ���Э��ʹ������(��Ŀ�ĵ�ַ,Ŀ�Ķ˿�,�ಥTTL)*/
	char * pszDate;  /*��Ϣ����ʱ��ʱ�䣬��ʽΪyyyy-mm-dd hh:mm:ss */ 
	char *pszUseragent;
	char *pszUser;
	char *pszRequire;
	char *pszOVSExtensions;		/*ovs��������չ*/
	char *pszOVSMayForward;
	int  bHasMayForward;
	char *pszOVSPlayNow;
	int  bHasPlayNow;
	char *pszOVSPrepareAssets;
	int  bHasPrepareAssets;
	char *pszOVSMayNotify;
	int  bHasMayNotify;
	char *pszOVSNoFlush;
	int  bHasNoFlush;	
	char *pszClientId;       /*�ͻ���ID��,�����з����*/
	char *pszTransactionId;   /*�����*/
	char *pszContentLength;      /*��Ϣ�峤��*/
	char *pszContentType;        /*��Ϣ������*/  
	char *pszBody;              /*��Ϣ������*/
	char *pszCommandExpand;	/*��չ�ӿ�*/
	char *pszCSeq;  /*���к�*/
} CSRTSPCommandInfo_S;

	
/*����NPT��Ϣ�ṹ*/
typedef struct  
{
	CSRTSPTimeFormat_E eTimeFormat;	/*0��û��ʱ�䷶Χ��1������NPTʱ���ʽ��2������smpteʱ���ʽ*/
	double RangeStart;
	double RangeEnd;
	WORD wSmpteFps;  				/*SMPTE��ʽʱ����֡����*/

} CSRTSPRangeDesc_S;

/*�������ϱ�������RTSP��Ϣ�������ڽṹ CSRTSPDecode_S��*/
typedef struct 
{
	DWORD uCseq;
	int bHaveCseq;
	int bCloseConnection;
	char acRetCode[4];         /* 3 byte return code - \0 delimited */
	char *pszRetResp;
	char *pszBody;              /* Contains body returned */
	char *pszAccept;
	char *pszAcceptEncoding;
	char *pszAcceptLanguage;
	char *pszAllowPublic;          /*options ���ص�PUBLIC����*/
	char *pszAuthorization;
	char *pszBandwidth;
	char *pszBlocksize;
	char *pszCacheControl;
	char *pszContentBase;	/*���������ص�URL·��*/
	char *pszContentEncoding;/*Content-Encoding����Entity-Body�ı����ʽ�����磺Content-Encoding��gzip*/
	DWORD uContentLength; /*����������(message body)�ĳ���,��λ��octet*/
	char *pszContentLanguage;
	char *pszContentLocation;/*������ʵ�����ݵ�λ��.���һ����Դ�����˶��ʵ��,ÿ��ʵ������ڲ�ͬ��·����,��ʱConntent-Locationָ���˵���ʵ���·��*/
	char *pszContentType;/*������Entity-Body��ý�����ͣ����磺Content-Type��text/parameters*/
	char *pszCookie;
	char *pszDate;
	char *pszExpires;/*һ��ý����������ý�����Ĺ���ʱ��*/
	char *pszFrom;
	char *pszIfModifiedSince;
	char *pszLastModified;/*�÷���������ý����������ý�������һ�ε��޸�ʱ��*/
	char *pszLocation;           /*�ض�λ��Ϣ*/
	char *pszProxyAuthenticate;
	char *pszProxyRequire;
	char *pszRange;
	CSRTSPRangeDesc_S *psRangeDesc;/*����range�е�����*/
	char *pszReferer;
	char *pszRequire;
	char *pszRetryAfter;
	char *pszRtpInfo;
	char *pszScale;
	char *pszServer;
	char *pszSession;
    char *pszSpeed;
    char *pszTransport;
	char *pszUnsupported;
	char *pszUserAgent;
	char *pszVia;
	char *pszWWWAuthenticate;
	char *pszAnnounceUrl;	
	char *pszServerDate;
	char *pszOVSExtensions;
	char *pszNotice;	/*<USE>x-notice: 2101 "End-of-Stream Reached" event-date=19990706T215715.777Z*/
	char *pszControlAddress;
	char *pszAuthenticationInfo;
	char *pszOVSPlayNow;
	char *pszOVSNoFlush;
	char *pszServerTimeout;
	char *pszExtensionsInfo; /*x-Info:"EOS"*/
	double  Position;   //x-position������Ӧ���з���ʱ��  ��λ����
	int  nTimeShiftTV;/*��ֵΪ1ʱ��ʾ�л���ֱ���ŵ���xΪ0ʱ��ʾ�л���¼���ŵ�*/
	double VCRRange;/*��xxx.xxx��Ϊ0�����ʾ����һ��ʱ��Ƶ��������xxx.xxx��ʾ��ǰ����ʱ�Ƶ����ʱ�䣬��λS*/
	char *pszExpandCommand;/*��չ�����ֶ�*/	
} CSRTSPDecode_S;

/*�����ӿڷ���ֵ������*/
typedef enum 
{
	EM_RTSP_RESPONSE_SUCCESS = 0,				/*������ȷ*/
	EM_RTSP_RESPONSE_REDIRECT, 	/*�ض����������ϲ��Ѿ��ض���ʹ���µ�����*/
	EM_RTSP_RESPONSE_RECV_ERROR,  	/*�����������ݽ��մ��󣬶����ݳ�ʱ����SOCKET����,�ر�SOCKET*/
	EM_RTSP_RESPONSE_BAD,   	/*���������ش����룬���ر�SOCKET�����ϲ���������룬�����洦��*/
	EM_RTSP_RESPONSE_MISSING_OR_BAD_PARAM,   /*������֯������������װ�����·�����*/
	EM_RTSP_RESPONSE_BAD_URL,         		/*URL������װ�µ�URL*/
	EM_RTSP_RESPONSE_CLOSED_SOCKET, 		 /*SOCKET�Ŵ���*/
	EM_RTSP_RESPONSE_MALFORM_HEADER,  	/*�������������ݽ���ʱ����*/
	EM_RTSP_RESPONSE_RTP_MESSAGE,     /*��������������ΪRTP����*/
	EM_RTSP_SELECT_RECV_ERROR,         /*SELECT ������*/
	EM_RTSP_SELECT_TIMEOUT,            	/*SOCKET ��ʱ����*/
	EM_RTSP_REDIRECT_FAILED,           	/*�ض������½���SOCEKETʱʧ��*/
	EM_RTSP_SEND_FAILED,                	/*��������ʧ��*/
	EM_RTSP_RECV_NOT_COMPLETE,        /*���ν������ݲ�������*/
	EM_RTSP_PARAMETER_ERR,
	EM_RTSP_FAILURE,
	EM_RTSP_REBUILD			/*tcp����*/
} CSRTSPRet_E;

typedef enum 
{
	EM_RTSP_CALLBACK_RECVRTSP = 0,		/*���ϲ㱨����������͵�RTSP��Ϣ*/
	EM_RTSP_CALLBACK_RECVRTP			/*���ϲ㷢��RTP����*/
} CSRTSPCallback_E;

typedef void (*FonRtspRecvMsg)(
			CSRTSPCallback_E 			eCallbackType,/*�ص���������*/
			CSRTSPServerResponse_E 		eResponseType, /*��������Ӧ����*/
			CSRTSPDecode_S				*psResp,/* eCallbackType */
			char						*pcRtpBuf ,
			BYTE						ucRtpChannel,
			WORD 						wBufLen,
			char 						*pcUserData);

/******************************************************************************
*                                 ��������                                   *
******************************************************************************/

/******************************************************************************
��������:	CSRTSPCreateClient
��������:
    �����ͻ����������������
�������:
pszUrl		�����Ŀ��URI,pszUrl����'\0'�������ַ��������ܳ��Ȳ�����1024���ֽ�,
			�洢�ռ��ɵ��������룻
dwTimeout 	�������ӳ�ʱ����λ���룬dwTimeout�������0��

�������:
phClientHandle �ͻ��˵ľ����

�� �� ֵ:
    ����ɹ�������0�����򷵻ش���š�
����˵��:
******************************************************************************/	
int CSRTSPCreateClient(char *pszUrl, DWORD dwTimeout,HCSHANDLE *phClientHandle);

/******************************************************************************
��������:	CSRTSPDestroyClient
��������:
    ���ٿͻ��˾����
�������:
hClientHandle  �ͻ��˾����

�������:

�� �� ֵ:
��
����˵��:
******************************************************************************/	
int CSRTSPDestroyClient(HCSHANDLE hClientHandle);

/******************************************************************************
��������:	CSRTSPSendDescribe
��������:
    ���nSendOnlyΪ1����װDESCRIBE����ṹ,�����͵���������������Ϻ��������ء�
	���nSendOnlyΪ0����װDESCRIBE����ṹ���͵������������������������ص���Ϣ��
���������ppsDecodeResult������ͨ���˽ӿڿɻ�ý�Ŀ��ص�������Ϣ��

�������:
hClientHandle		�ӿ�3.1�����Ŀͻ��˾����
sCmd	 			��װRTSP�������Ϣ�ṹ���ɵ����߷����ڴ棻
dwRedirectUrlLen  	pszRedirectUrl�ĳ��ȣ�
nSendOnly			1�����ͺ��������أ�ppsDecodeResult��pszRedirectUrl��ΪNULL;
					0�����Ͳ�������Ϣ��ppsDecodeResult��pszRedirectUrl������ΪNULL��


�������:
ppsDecodeResult  	�ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ����ڴ������и��Ĳ�����Ҳ�������٣�
pszRedirectUrl  	���������ص��ض���url���ɵ����߷����ڴ棬������'\0'�������ַ�����

�� �� ֵ:
����ɹ�������0�����򷵻ش���š�
����˵��:
******************************************************************************/	
int CSRTSPSendDescribe(HCSHANDLE			hClientHandle,
						CSRTSPCommandInfo_S	 	sCmd, 
									DWORD		dwRedirectUrlLen,
									int		nSendOnly,
									char 		*pszRedirectUrl, 
							CSRTSPDecode_S 	    **ppsDecodeResult);

/******************************************************************************
��������:	CSRTSPSendSetup
��������:
	���nSendOnlyΪ1����װSETUP����ṹ,�����͵���������������Ϻ��������ء�
	���nSendOnlyΪ0����װSETUP����ṹ���͵������������������������ص���Ϣ��
���������ppsDecodeResult������ͨ���νӿڿ��������Э�̴�����صĲ�����
�������:
hClientHandle	�ӿ�3.1�����Ŀͻ��˾����
pszUrl			�轨�����ӵ�����Ӧ��"��"url, pszUrl����'\0'�������ַ��������ܳ��Ȳ�����1024���ֽڣ��洢�ռ��ɵ��������룻
sCmd	 		��װRTSP�������Ϣ�ṹ���ɵ����߷����ڴ棻
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��phSessionHandle��ΪNULL;
				0�����Ͳ�������Ϣ��ppsDecodeResult��phSessionHandle������ΪNULL��

�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ����ڴ������и��Ĳ�����Ҳ�������٣�
phSessionHandle  ��ȡ�Ự��Ϣ�洢�ľ����


�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
******************************************************************************/	
int CSRTSPSendSetup(HCSHANDLE				hClientHandle, 
		     					 char			*pszUrl, 
		     	CSRTSPCommandInfo_S 			sCmd, 
		 						int			nSendOnly,
		     			 HCSHANDLE			*phSessionHandle, 
					CSRTSPDecode_S 	  		**ppsDecodeResult);

/******************************************************************************
��������:	CSRTSPSendPlay
��������:
	���nSendOnlyΪ1����װPLAY����ṹ,�����͵���������������Ϻ��������ء�
	���nSendOnlyΪ0����װPLAY����ṹ���͵������������������������ص���Ϣ��
���������ppsDecodeResult������ͨ�����������Խ���Ŀ����������
�������:
hSessionHandle	�Ự�ľ�����·�CSRTSPSendSetupʱ������
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��
�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ�
					��ָ����ڴ������и��Ĳ�����Ҳ�������١�
����ɹ�������0�����򷵻ش����
����˵��:
	�������Ϣ��һ���·�������nSendOnly��0����Ϊ���ǵ�������������ȷ�յ���
	���ܽ�������Ĳ������������ͣ�����·����ţ�����nSendOnly��1����Ϊ����ʽ��
	����������ܻ�Ӱ����ϲ㴦���Ч�ʡ�
******************************************************************************/
int CSRTSPSendPlay(HCSHANDLE	   		hSessionHandle,
				CSRTSPCommandInfo_S 	   	sCmd,
							int			nSendOnly,
			            CSRTSPDecode_S     	**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendPlayByClientHandle
��������:
	���nSendOnlyΪ1����װPLAY����ṹ,�����͵���������������Ϻ��������ء�
	���nSendOnlyΪ0����װPLAY����ṹ���͵������������������������ص���Ϣ��
���������ppsDecodeResult������ͨ�����������Խ���Ŀ����������
�������:
hClientHandle	������RTSP�ͻ��˾����
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��
�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ�
					��ָ����ڴ������и��Ĳ�����Ҳ�������١�
����ɹ�������0�����򷵻ش����
����˵��:
	�������Ϣ��һ���·�������nSendOnly��0����Ϊ���ǵ�������������ȷ�յ���
	���ܽ�������Ĳ������������ͣ�����·����ţ�����nSendOnly��1����Ϊ����ʽ��
	����������ܻ�Ӱ����ϲ㴦���Ч�ʡ�
��CSRTSPSendPlay ��֮ͬ��:
	CSRTSPSendPlay ��һ������:���� CSRTSPSendSetup�������õĻỰ���
	CSRTSPSendPlayByClientHandle ��һ��������� :�Ǵ�����RTSP�ͻ��˵ľ��

******************************************************************************/

int CSRTSPSendPlayByClientHandle(HCSHANDLE	hClientHandle,
                   CSRTSPCommandInfo_S sCmd,
                   int	nSendOnly,
                   CSRTSPDecode_S **ppsDecodeResult);

/******************************************************************************
��������:	CSRTSPSendPause
��������:
	���nSendOnlyΪ1����װPAUSE����ṹ,�����͵���������������Ϻ��������ء�
	���nSendOnlyΪ0����װPAUSE����ṹ���͵������������������������ص���Ϣ��
���������ppsDecodeResult������ͨ�����������Խ����ŵĽ�Ŀ��ͣ���š�
�������:
hSessionHandle	�Ự�ľ�����·�CSRTSPSendSetupʱ������
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��
�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ�
				��ָ����ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�

******************************************************************************/
int CSRTSPSendPause(HCSHANDLE	   		hSessionHandle,
					CSRTSPCommandInfo_S 	   	sCmd,
								int			nSendOnly,
			            CSRTSPDecode_S     	**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendPauseByClientHandle
��������:
	���nSendOnlyΪ1����װPAUSE����ṹ,�����͵���������������Ϻ��������ء�
	���nSendOnlyΪ0����װPAUSE����ṹ���͵������������������������ص���Ϣ��
���������ppsDecodeResult������ͨ�����������Խ����ŵĽ�Ŀ��ͣ���š�
�������:
hSessionHandle	������RTSP�ͻ��˾����
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��
�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ�
				��ָ����ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�
��CSRTSPSendPause ��֮ͬ��:
	CSRTSPSendPause ��һ������:���� CSRTSPSendSetup�������õĻỰ���
	CSRTSPSendPauseByClientHandle ��һ��������� :�Ǵ�����RTSP�ͻ��˵ľ��
	
*/
int CSRTSPSendPauseByClientHandle(HCSHANDLE	hClientHandle,
                    CSRTSPCommandInfo_S sCmd,
                    int	nSendOnly,
                    CSRTSPDecode_S **ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendTeardown
��������:
	���nSendOnlyΪ1����װTEARDOWN����ṹ,�����͵���������������Ϻ��������ء�
	���nSendOnlyΪ0����װTEARDOWN����ṹ���͵������������������������ص���Ϣ��
���������ppsDecodeResult������ͨ�����������Խ��Ѳ��ŵĽ�Ŀֹͣ��
�������:
hSessionHandle	�Ự�ľ�����·�CSRTSPSendSetupʱ������
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��
�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ��
					���ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�
******************************************************************************/
int CSRTSPSendTeardown(HCSHANDLE	   	hSessionHandle,
				CSRTSPCommandInfo_S 	   		sCmd,
								int			nSendOnly,
			            CSRTSPDecode_S     		**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendGetParameter
��������:
	���nSendOnlyΪ1����װGETPARAMETER����ṹ,�����͵���������������Ϻ��������ء�
	���nSendOnlyΪ0����װGETPARAMETER����ṹ���͵������������������������ص���Ϣ��
���������ppsDecodeResult������
    
�������:
hClientHandle  �ͻ��˾����
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��


�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ���
				�ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�
	��������ݷ�����֧�ֵĲ�ͬ���в�ͬ��������װ�����磺kasenna����������֧������Ϣ
������װ"position"�õ���ǰ��Ŀ���ŵ�ʱ�䣬��װ"scale"���Եõ���ǰ������֧�ֵĿ����
�˵����ʡ��������������������������������������֡�
******************************************************************************/
int CSRTSPSendGetParameter(HCSHANDLE			hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendSetParameter
��������:
    �����������setparameter��������ؽ��������
    �еķ���������֧�ֱ��������װ�·�ʱ����Ҫ���˽�������Ƿ�֧��
    
�������:
hClientHandle  �ͻ��˾����
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��

�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ��
				���ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�
	���ݷ�����֧�ֵĲ�ͬ����Ҳ�ò�ͬ����;������΢�������ͨ����������߿ͻ�������
��ϣ����������˵Ŀ�ʼ����
******************************************************************************/
int CSRTSPSendSetParameter(HCSHANDLE			hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendOptions
��������:
    �����������options��������ؽ��������ͨ���·���������Եõ�������֧�ֵ�rtsp���
    �еķ���������֧�ֱ��������װ�·�ʱ����Ҫ���˽�������Ƿ�֧��
    
�������:
hClientHandle  �ͻ��˾����
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��

�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ��
				���ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�

******************************************************************************/
int CSRTSPSendOptions(HCSHANDLE					hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendAnnounce
��������:
    �����������announce��������ؽ����������ʵ��Ӧ���б������ɷ������������͵ıȽ϶࣬
    �����ڽ�Ŀ������Ϻ���߿��˵���ͷʱͨ��������֪ͨ�ϲ�
    
�������:
hClientHandle  �ͻ��˾����
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��

�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ��
				���ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�
	ANNOUNCE��������������;�����ӿͻ��˷��͵�������ʱ��ANNOUNCE�ύ������URIȷ��
��ý�����������������������ӷ��������͵��ͻ���ʱ��ANNOUNCEʵʱ��real-time����
���»Ự��������ʵ��Ӧ���б������ɷ������������͵ıȽ϶࣬�����ڽ�Ŀ������Ϻ��
�߿��˵���ͷʱͨ��������֪ͨ�ϲ㣨��kasenna����������
******************************************************************************/
int CSRTSPSendAnnounce(HCSHANDLE				hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendRecord
��������:
    �����������record��������ؽ����������ʵ��Ӧ���б������ɷ������������͵ıȽ϶࣬
    �����ڽ�Ŀ������Ϻ���߿��˵���ͷʱͨ��������֪ͨ�ϲ�
    
�������:
hClientHandle  �ͻ��˾����
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��

�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ��
				���ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�

******************************************************************************/
int CSRTSPSendRecord(HCSHANDLE					hClientHandle,
						  CSRTSPCommandInfo_S	 		sCmd,
                         				int			nSendOnly,
                        		CSRTSPDecode_S 			**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPRecvRtspMessage
��������:
	���ӿ�ʵ�ֵĹ��ܿ��Խ��շ��������͵��κ���Ϣ��������rtspӦ����Ϣ�������·�rtsp��Ϣ��
rtp over rtsp��Ϣ��
	�������Ĵ���ԭ���ǣ�����ģ������������ݺ󣬵��ñ������������ݣ�Ȼ������ݽ��н�����
������ԭ���ǣ��������һ�����������ݣ��Ͳ�������ֱ�ӷ��أ�����ԭ�е����ݽ��ջ���ȥ���㣬
�����һ�����������ݣ���������ƶ����ݻ�������ƫ��ָ�룬������������ݸ��ǵ���

  
�������:
hClientHandle      	�ͻ��˾��;
dwTimeout   		��ʱʱ�䣬��λ���롣
�������:
��
�� �� ֵ:
 ����ɹ�������0�����򷵻ش����
����˵��:
******************************************************************************/
int CSRTSPRecvRtspMessage(HCSHANDLE	hClientHandle,DWORD dwTimeout);
/******************************************************************************
��������:	CSRTSPGetRtspMessage
��������:
	���ӿ�ʵ�ֵĹ��ܿ���������ȡ���������͵����ݡ�
  
�������:
hClientHandle      	�ͻ��˾��; 
dwTimeout   		��ʱʱ�䣬��λ���롣

�������:
pcBuf				���rtp���ݵĻ�����,���û�����;
pdwBufLen			ʵ�ʴ�ŵ���������rtp���ݳ���;
pChannelId			rtp���ݵ�;
pnMsgType			������������Ϣ�����ͣ�
ppsDecodeResult		�ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���
					ָ����ڴ������и��Ĳ�����Ҳ�������١�

�� �� ֵ:
 ����ɹ�������0�����򷵻ش����
����˵��:
******************************************************************************/
int CSRTSPGetRtspMessage(HCSHANDLE 			hClientHandle,
									DWORD 			uTimeout,
									char 			*pcBuf,
									DWORD 			*puBufLen,
									BYTE			*pChannelId,
									int				*pnMsgType,
									CSRTSPDecode_S	**ppsDecodeResult);
/******************************************************************************
��������:	CSRTSPSendResponse
��������:
����Ӧ������������ϱ�����Ϣ����������������ϱ�����Ϣ��ҪӦ��ʱ��
���Ե��ñ��ӿڣ���װ��Ӧ����ϢӦ�������

  
�������:
hClientHandle   �ӿ�3.1�����Ŀͻ��˾��;
pszStatus	 	��Ӧ�ţ���������'\0'�������ַ���������"200"��
pszStatusDesc	��Ӧ��������������'\0'�������ַ���������"ok"��
pszSession		�Ự�ţ���������'\0'�������ַ���������"22456712566"��
sCmd    		RTSP������װ����Ϣ�ṹ��

�������:
��
�� �� ֵ:
 ����ɹ�������0�����򷵻ش����
����˵��:
******************************************************************************/
int CSRTSPSendResponse (HCSHANDLE	hClientHandle,
							char *pszStatus,
                            char *pszStatusDesc,
                            char *pszSession,
                            CSRTSPCommandInfo_S sCmd);
/******************************************************************************
��������:	CSRTSPSetCallbackFunc
��������:
���ûص�������
  
�������:
hClientHandle			�ͻ��˾��;
fnCallback				Ҫע��Ļص�����ָ��;
pcUserdata				�ص��������û�������


�������:
��
�� �� ֵ:
 ����ɹ�������0�����򷵻ش����
����˵��:
******************************************************************************/
int CSRTSPSetCallbackFunc (HCSHANDLE			hClientHandle,
							FonRtspRecvMsg			fnCallback,
									char			*pcUserdata); 

/******************************************************************************
��������:	CSRTSPSetLogLevel
��������:
    ������־��ӡ����
    
�������:
int nLogLevel  ��־��ӡ����

�������:
��
�� �� ֵ:
��
����˵��:
******************************************************************************/
void CSRTSPSetLogLevel(int nLogLevel);

/******************************************************************************
��������:	CSRTSPSetRecvTimeout
��������:
   ���ý������ݳ�ʱ
�������:
hClientHandle		�ͻ��˾��;
dwTimeout        	��ʱʱ�䣬��λ����.

�������:
	��
�� �� ֵ:
    �ɹ�0������������

����˵��:
******************************************************************************/
int CSRTSPSetRecvTimeout(HCSHANDLE	hClientHandle,DWORD dwTimeOut);
/******************************************************************************
��������:	CSRTSPSetSendTimeout
��������:
   ���÷������ݳ�ʱ
�������:
hClientHandle		�ͻ��˾��;
dwTimeout        	��ʱʱ�䣬��λ����.

�������:
	��
�� �� ֵ:
    �ɹ�0������������

����˵��:
******************************************************************************/
int CSRTSPSetSendTimeout(HCSHANDLE	hClientHandle,DWORD dwTimeOut);

/******************************************************************************
��������:	CSRTSPCloseServerSocket
��������:
    �رտ�������socket
�������:
hClientHandle		�ͻ��˾��;

�������:
	��
�� �� ֵ:
    �ɹ�0������������

����˵��:
******************************************************************************/

int CSRTSPCloseServerSocket(HCSHANDLE hClientHandle);

/* add by zch 
������������*/
/******************************************************************************
��������:	CSRTSPCloseServerSocket
��������:
	������������socket
�������:
hClientHandle		�ͻ��˾��;
dwTimeout           ��ʱʱ�䣬��λ����.

�������:
	��
�� �� ֵ:
	�ɹ�0������������

����˵��:
******************************************************************************/
int CSRTSPCreateSocket(HCSHANDLE hClientHandle,DWORD dwTimeout);


/******************************************************************************
��������:	CSRTSPSetCseqStrategy
��������:
	����RTSP����ʱ��������Ӳ���
�������:
hClientHandle		�ͻ��˾��;
nStrategy          0:�յ���������Ӧ�����
			   1:����ʱ����

�������:
	��
�� �� ֵ:
	�ɹ�0������������

����˵��:
******************************************************************************/

// Ĭ���յ���������Ӧ�������
// 0:�յ���������Ӧ�����
// 1:����ʱ����
int CSRTSPSetCseqStrategy(HCSHANDLE hClientHandle,int nStrategy);

/*
��������:	CSRTSPSendPing
��������:
    �����������PINH��������ؽ��������
    �еķ���������֧�ֱ��������װ�·�ʱ����Ҫ���˽�������Ƿ�֧��
    
�������:
hClientHandle  �ͻ��˾����
sCmd   			��װRTSP�������Ϣ�ṹ��
nSendOnly		1�����ͺ��������أ�ppsDecodeResult��ΪNULL��
				0�����Ͳ�������Ϣ��ppsDecodeResult����ΪNULL��

�������:
ppsDecodeResult  �ⲿģ�鴫���ָ�룬��ָ�����ݽ������ڴ棬���ⲿģ�鲻�ܶ���ָ��
				���ڴ������и��Ĳ�����Ҳ�������١�
�� �� ֵ:
����ɹ�������0�����򷵻ش����
����˵��:
	�·�������ʱ����nSendOnly��1����Ϊ����ʽ�Ľ���������ܻ�Ӱ����ϲ㴦���Ч�ʡ�
	���ݷ�����֧�ֵĲ�ͬ����Ҳ�ò�ͬ����;������΢�������ͨ����������߿ͻ�������
��ϣ����������˵Ŀ�ʼ����
    ���������������������
*/
int CSRTSPSendPing(HCSHANDLE    hSessionHandle,
                    CSRTSPCommandInfo_S sCmd,
                    int nSendOnly,
                    CSRTSPDecode_S **ppsDecodeResult);
                    
                    
/******************************************************************************
��������:	CSRTSPGetCurrentSeq
��������:
    ��ȡ��ǰRTSP ���������к�
�������:
hClientHandle		�ͻ��˾��;

�������:
	��
�� �� ֵ:
    �ɹ�0������������

����˵��:
******************************************************************************/
int CSRTSPGetCurrentSeq(HCSHANDLE hClientHandle);
/******************************************************************************
��������:	CSRTSPSetCurrentUrl
��������:
    ���õ�ǰ���͵�RTSP ����ͷ����URL
�������:
hClientHandle		�ͻ��˾��;
pcUrl  Ҫ���õ�url��
�������:
	��
�� �� ֵ:
    �ɹ�0������������

����˵��:
******************************************************************************/
int CSRTSPSetCurrentUrl(HCSHANDLE hClientHandle, char *pcUrl);

int CSRTSPModifySeesionUrl(HCSHANDLE	 hSessionHandle, char *pcUrl);

int CSRTSPModifyCurrentSeq(HCSHANDLE hClientHandle, int nOffSet);

#ifdef __cplusplus
}
#endif

#endif
