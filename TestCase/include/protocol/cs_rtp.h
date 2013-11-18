/******************************************************************************
�ļ����ƣ�CS_RTP.H
��Ȩ����(c) 2007�� ������ͬ�޵��ӹɷ����޹�˾��

��ǰ�汾��Ver 1.0    
��    �ߣ������� (duanxubin@coship.com)

����ժҪ��
		�ļ�����˵��

�޸ļ�¼��
�汾��		����		����			�޸�����
--------	----------	----------		------------
1.0			2007-5-28	������	����
1.1			2009-4-24	��      ��   	�޸�
*******************************************************************************/

#ifndef CSRTP_H
#define CSRTP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "udi2_socket.h"
#include "mmcp_typedef.h"
/******************************************************************************
 *                                 ͷ�ļ�����                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 �ꡢ���塢�ṹ��                           *
 ******************************************************************************/

/******************************************************************************
 *                                 ��������                                   *
 ******************************************************************************/

/*RTP������,����RTP�����ʵ��ϸ��*/
typedef	VOID*		HRTPHANDLE;

/*RTPģ�鷵�ش�����,0��ʾִ�гɹ�,������ʾʧ��*/
typedef enum
{
	EM_RTP_ERROR = -100, 	/*����ſ�ʼ*/
	EM_RTP_PARAERR,		/*�����������*/
	EM_RTP_NOMEMORY, 		/*�ڴ�û�����뵽*/
	EM_RTP_GOOD = 0		/*����ִ�гɹ�*/
}CSRTPErrorCode_E;


/*RTP�ص�����, ʹ�÷����μ�CSRTPCallback��ʹ�÷���*/
typedef enum 
{
        EM_RX_RTP, 		/*RTP���ݰ�*/
        EM_RX_SR, 		/*SR��*/
        EM_RX_RR, 		/*RR��*/
        EM_RX_SDES,		/*SDES ��Ŀ*/
        EM_RX_BYE,         	/* Source is leaving the session, database entry is still valid                           */
        EM_SOURCE_CREATED, /*һ��Դ������*/
        EM_SOURCE_DELETED, /* Source has been removed from the database                                              */
        EM_RX_RR_EMPTY,    	/* We've received an empty reception report block */
        EM_RR_TIMEOUT, 	/*Դ��ʱ*/
        EM_RX_APP 			/*APP��*/
} CSRTPEventType_E;

/*RTP�ص��¼��ṹ, ʹ�÷����μ�CSRTPCallback��ʹ�÷���*/
typedef struct 
{
	DWORD		 	dwSSRC; /*ͬ��Դ*/
	CSRTPEventType_E  	eType; 	/*������*/
	VOID			*pData; 	/*����*/
	CSUDITimeval_S	 	*psTS; 	/*��ǰʱ��*/
} CSRTPEvent_S;


/*RTPģ�鷵�ش�����,0��ʾִ�гɹ�,������ʾʧ��*/
typedef enum
{
	EM_RTP_TYPE  = 0, 	/*RTP ����*/
	EM_RTCP_TYPE 	 	/*RTCP����*/
}CSRTPPacketType_E;

/*APP��������Ļص��ṹ, ʹ�÷����μ�CSRTPCallback��ʹ�÷���,.ͬʱҲ�Ƿ���APP���Ľṹ*/
typedef struct
{
	BYTE 	ucVersion; 		/*�汾*/
	BYTE 	ucPadding;		/*���λ*/
	BYTE 	ucSubType;		/*Ӧ��ʹ��*/
	BYTE 	ucPayloadType;	/*��������*/
	WORD  	wLength;			/* ���ݳ���  ����pcData �ĳ���       */
	DWORD   dwSSRC;
	CHAR    acName[4];       		/* four ASCII characters  */
	CHAR    *pcData;        		/* variable length field */
}CSRTPRtcpAPP_S;

/*RTPģ����¼��ص�,�¼����Ͳμ�rtp_event_type*/
typedef void (*FOnRTPEvent)( HRTPHANDLE hRTPSession, 
								CSRTPEvent_S *psEvent,
								BYTE *pucUserdata);

/*����RTCP���ݰ�ʱ,�û�����APP���ݵĻص�,����û�����ú���,��RTCP���н�������APP����.*/
typedef CSRTPRtcpAPP_S * (*FOnRTCPApp)(HRTPHANDLE hRTPSession, 
												DWORD dwSsrc, 
												INT nMaxSize);


HRTPHANDLE  CSRTPCreate(BYTE *pucUserdata, FOnRTPEvent fnCallback,DWORD dwMemoryLen);

INT CSRTPDestroy(HRTPHANDLE hRTPSession);

INT CSRTPParseData(HRTPHANDLE hRTPSession,
						CSRTPPacketType_E eType,
						INT nBuffeLen,
						BYTE * pucBuffer,DWORD dwTimeout);

INT CSRTPGetPayloadData(HRTPHANDLE hRTPSession,DWORD dwTimeout,
								BYTE *pcDemuxBuffer, 
								DWORD *nNeedDataLen);
INT CSRTPBuildRtcpPacket(HRTPHANDLE hRTPSession, 
								DWORD dwRtpTS,
								INT nBufLen,
								BYTE *pucBuffer,
								FOnRTCPApp fnAppCallback);
#ifdef __cplusplus
}
#endif

#endif  /* CSRTPINTERFACE_H */

/* �ļ����� */



