/******************************************************************************
�ļ���:	CSIPPLAYPUMP.H
Copyright (c) 2007�� Shen Zhen Coship Electronics CO.,LTD.��

�汾:		Ver 1.0    
����:		Nevill (luoyangzhi@coship.com)

�ļ�����:
    Playpump�Ľӿ��ļ�����Ҫ��Mediaplayʹ�á�
	
    Playpump�Ƕ�IP�鲥�͵㲥��һ�ֳ��󣬽���������tuner���ƵĹ��ܣ�һ������
    connect�����������"��Ƶ"�����������ݾͻ����뵽Ӳ��DEMUX�С�Ȼ�������DEMUX
    ���ܹ�������ע���DEMUX,��STƽ̨��SWTS��BCMƽ̨��Playbackģʽ��
    ����DVB˫����RTSPģ�����ȡ������������Ҫ��Ƶ����Ϣ�����ϲ㸺�������Ƶ����
    ��Ȼ��������Ϊ����Ƶ�������ڴ���Ҳδ�����ɣ����Ҹ�����ͨ��tuner��

�޸ļ�¼:
�汾   	����		�޸���		˵��
--------	----------	--------	------------
1.0			2007-6-22	Nevill		����
1.1			2007-7-26	Tongfulei	���ӷ���ֵ����:EM_PUMP_NOT_SUPPORT
1.2         2008-10-30  ������      ���RTSP������ö������
*******************************************************************************/

#ifndef CSIPPLAYPUMP_H
#define CSIPPLAYPUMP_H

/******************************************************************************
 *                                 Header File Include                        *
 ******************************************************************************/
#include "mmcp_typedef.h"
#include "cs_player_message.h"
#include "cs_ipplugin.h"

/******************************************************************************
 *                                 Macro/Define/Structus                      *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif



#define IPPUMP_DEFAULT_THREAD_PRI   (125)		/*�������ȼ�*/	
#define IPPUMP_DEFAULT_THREAD_STACK (0x20000) /* 128 * 1024������ջ��С*/

#define IPUDP_ES_MODE		(0x44)
#define IPTV_MODE		(0x33)
#define IPQAM_VOD		(0x22)


typedef struct _CSIPPUMPDVBInfo_S
{
    int m_nServiceID;
    char *pcMineType;
    DWORD m_dwMaxBitRate;//KHZ
    int m_nHeight;
    int m_nWidth;
    int m_nFramePerSeconf;
    int m_nAspectRatio;
    int m_nStartTime; //ms
    DWORD m_dwDuration;
    int m_nPlayModes;
    int m_nNumTrickSpeed;
    int m_nTrickSpeed;
    int m_nTypeSpecificData;
    int m_nVideoPID;
    int m_nAudioPID;
    int m_nPCRPID;
    int m_nProgramNo;
    int m_nFrequency;
    int m_nModulation;//0 - Not Define,  1 - 16QAM,	2 - 32QAM,	3 - 64QAM, 4 - 128QAM, 5 - 256QAM
    int m_nSymbolRate;
    int m_nAnnex;
    int m_nChannelSpace;
}CSIPPUMPDVBInfo_S;

typedef  enum 
{
	EM_IPPumpStreamTrans_UNKNOWN,          /*δ֪����*/
	EM_IPPumpStreamTrans_TCP,            /*IPTV MODE*/
	EM_IPPumpStreamTrans_UDP,	        /*IPQAM MODE*/
}IPPumpStremTransMode_E;

typedef int (*FOnCSIPPUMPHaveMsgToUser)(DWORD nUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);

/******************************************************************************
 *                                 Global Function Declare                    *
 ******************************************************************************/
/******************************************************************************
������:     CSIPPUMPCreate

��������:
    ����һ��Playpump

����:
    dwThreadPriority:�������̵߳����ȼ���
    dwThreadStackSize:�������߳�ջ�Ĵ�С��

���:
    phPlaypump �� ���ɵ�Playpump��������Ժ���Ҫ���ñ�ģ����������ʱʹ��

����ֵ:
    EM_PUMP_SUCCESS �� �ɹ�

����:�߳����ȼ���ջ�Ĵ�С���������IPPUMP_DEFAULT_THREAD_PRI��IPPUMP_DEFAULT_THREAD_STACK 
*******************************************************************************/

int CSIPPUMPCreate(DWORD			dwThreadPriority,
					 	DWORD			dwThreadStackSize,
					 	HCSHANDLE 		*phPlaypump );


/******************************************************************************
������: 	CSIPPUMPDestroy

��������:
		����һ��Playpump

����:
hPlaypump �� Ҫ���ٵ�playpump���

���:
	��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:

*******************************************************************************/
int CSIPPUMPDestroy(HCSHANDLE hPlaypump);


/******************************************************************************
������: 	CSIPPUMPConnect

��������:
	����һ��ģ��tuner�豸(�˴���ģ����˵�������������ģ���tuner���ԣ�������
	˵tuner�����͵�������ģ������ź�)

����:
	hPlaypump �� playpump���
	hDevice �� ��Ӳ���豸�йصľ������DEMUX,DECODE��
	nIpMode �� ����ָ��IPģʽ
	pszURL �� ������ԴURL
	    �����鲥: igmp://224.0.0.1:5001
	    ���ڵ㲥: rtsp://192.168.1.1:644/vod.ts
	    ����dvb˫��: 
	nTimeout - ��ʱʱ��
	
���:
    ��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:

*******************************************************************************/
int CSIPPUMPConnect(HCSHANDLE hPlaypump, HCSHANDLE hDevice, int nIpMode, char *pszURL, DWORD dwTimeout);


/******************************************************************************
������: 	CSIPPUMPDisconnect

��������:
	�Ͽ�һ��ģ��tuner�豸

����:
	hPlaypump �� playpump���
	nNICID �� ����ID
	
���:
    ��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:

*******************************************************************************/
int CSIPPUMPDisconnect(HCSHANDLE hPlaypump, int nNICID);


/******************************************************************************
������: 	CSIPPUMPSetCallback

��������:
	�Ե�ǰPlaypump���ûص�����

����:
	hPlaypump �� playpump���
	fnCallback �� Ҫע��Ļص�����
	pUser �� �ص�Ҫ�õ����û����ݣ����ûص��Ǵ���
	
���:
	��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:

*******************************************************************************/
int CSIPPUMPSetCallback(HCSHANDLE hPlaypump, FOnCSIPPUMPHaveMsgToUser fnCallback, void *pUser);


/******************************************************************************
������: 	CSIPPUMPResetCallback

��������:
   �Ե�ǰPlaypumpɾ���ص�����

����:
	hPlaypump �� playpump���
    fnCallback �� Ҫɾ���Ļص�����
	
���:
    ��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:

*******************************************************************************/
int CSIPPUMPResetCallback(HCSHANDLE hPlaypump);


/******************************************************************************
������: 	CSIPPUMPPause

��������:
	��ͣһ��Playpump

����:
	hPlaypump �� playpump���
	
���:
    ��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    ��ͣ���Ե㲥��Ч
*******************************************************************************/
int CSIPPUMPPause(HCSHANDLE hPlaypump);


/******************************************************************************
������: 	CSIPPUMPResume

��������:
	�ָ�һ��Playpump�Ĳ���

����:
	hPlaypump �� playpump���
	
���:
	��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    �ָ����Ž��Ե㲥��Ч

*******************************************************************************/
int CSIPPUMPResume(HCSHANDLE hPlaypump);


/******************************************************************************
������: 	CSIPPUMPSetRate

��������:
	����һ��Playpump�Ĳ�������

����:
	hPlaypump �� playpump���
	nRate �� Ҫ���õĲ����ٶȡ�Ϊ��֧�������Ҳ�ʹ�ø������������������׼����
	�������µĹ�ʽ: nRate = RealRate * 1000
	
���:
	��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    ��������1000ʱΪ1����ǰ����2000Ϊ������ǰ����4000Ϊ�ı��٣�Ϊ500��ʾ������
    ���С��0˵��Ϊ���ˡ�
    �ú���Ϊ�첽������
*******************************************************************************/
int CSIPPUMPSetRate(HCSHANDLE hPlaypump, int nRate);


/******************************************************************************
������: 	CSIPPUMPSetTime

��������:
	����һ��Playpump�Ĳ���ʱ���

����:
	hPlaypump �� playpump���
	nTime �� Ҫ���õ�ʱ��㣬��λΪ����
	
���:
	��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    �ú���Ϊ�첽������
*******************************************************************************/
int CSIPPUMPSetTime(HCSHANDLE hPlaypump, DWORD dwTime);

/******************************************************************************
������: 	 CSIPPUMPSetTimeByType

��������:
	����һ��Playpump�Ĳ���ʱ���

����:
	hPlaypump �� playpump���
	nTime �� Ҫ���õ�ʱ��㣬��λΪ����
	nType -- Ϊ1ʱʹ��NPTʱ�䣬������ʹ��UTC ʱ�� 
���:
	��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    �ú���Ϊ�첽������
*******************************************************************************/
int  CSIPPUMPSetTimeByType(HCSHANDLE hPlaypump, DWORD dwTime,int nType);

/******************************************************************************
������: 	CSIPPUMPGetTime

��������:
	��ȡһ��Playpump�ĵ�ǰ����ʱ���

����:
	hPlaypump �� playpump���
	
���:
	pnTime �� ��ǰ����ʱ�䣬��λ��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    �ú���Ϊͬ��������
*******************************************************************************/
int CSIPPUMPGetTime(HCSHANDLE hPlaypump, int *pnTime);


/******************************************************************************
������: 	CSIPPUMPGetDuration

��������:
	��ȡһ����Ŀ�ĳ���

����:
	hPlaypump �� playpump���
	
���:
	pdwDuration �� ��Ŀ���ȣ���λ��

����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    �ú���Ϊͬ��������
*******************************************************************************/
int CSIPPUMPGetDuration(HCSHANDLE hPlaypump, DWORD *pdwDuration);

/******************************************************************************
������: 	CSIPPUMPCreateSendSocket

��������:
	�����ϴ���������Ҫ����Դ

����:
	hPlaypump �� playpump���
	pszIPAdress �� Ŀ������IP��ַ,��"192.168.95.65"
	wPort �� Ŀ���������ն˿�
���:
	��
����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    �ú���Ϊͬ��������
*******************************************************************************/
int CSIPPUMPCreateSendSocket(HCSHANDLE hPlaypump, char *pszIPAdress,WORD wPort,DWORD dwTimeout,int nTcpFlag);
/******************************************************************************
������: 	CSIPPUMPDestroySendSocket

��������:
	�ͷ��ϴ���������Ҫ����Դ

����:
	hPlaypump �� playpump���
���:
	��
����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    �ú���Ϊͬ��������
*******************************************************************************/
int CSIPPUMPDestroySendSocket(HCSHANDLE hPlaypump);
/******************************************************************************
������: 	CSIPPUMPSendStream

��������:
	�ϴ����ݵ�ָ��λ��

����:
	hPlaypump �� playpump���
	pucBuffer �� Ҫ�ϴ�������
	dwLen �� Ҫ�ϴ����ݵĳ���
	dwTimeout �� ���ͳ�ʱʱ��
���:
	��
����ֵ:
	EM_PUMP_SUCCESS �� �ɹ�

����:
    �ú���Ϊͬ��������
*******************************************************************************/
int CSIPPUMPSendStream(HCSHANDLE hPlaypump,BYTE *pucBuffer,DWORD dwLen ,DWORD dwTimeout);

/******************************************************************************
������: 	CSIPPUMPIsSupportTrickMode

��������:
	��ȡ��ǰ��Ŀ�ǹ�֧�ֿ������

����:
		hPlaypump��IPPUMP���

���:
	��
����ֵ:
	EM_PUMP_SUCCESS �� ֧��

����:
    �ú���Ϊͬ��������
*******************************************************************************/
int CSIPPUMPIsSupportTrickMode(HCSHANDLE hPlaypump);

/******************************************************************************
������: 	CSIPPUMPDisableBreakPointToSee

��������:
	����һ��Playpump�㲥����Ƿ�ȥ��ȥ�ϵ������Ĺ��ܣ�Ĭ��ΪFALSE����Ҫ����̨��
	��Ŀ��ǰ��ϵͳ���ݴ����ýӿں�֤��һ�β���ʱ������Ӧ�ô�������locator��ʼ
	����ʱ��Ϊ���٣��м����ǰ�˷��Ͳ��������ʱ���Ǵ�ͷ��ʼ���ţ����ҶԸ�ϵͳ��
	�Ľ����������������ݴ���

����:
	bDisable Ĭ��ΪFALSE��ֻ������ΪTRUE���ʾ�ǵ㲥���ȥ���ϵ������Ĺ���
	
���:
	��

����ֵ:
	��
*******************************************************************************/
void CSIPPUMPDisableBreakPointToSee(BOOL bDisable);

#ifdef __cplusplus
}
#endif

#endif  /* CSIPPLAYPUMP_H */

/* End of File */

