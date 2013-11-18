/******************************************************************************
�ļ����ƣ�CSFINESUMCAINTERFACE.H
��Ȩ����(c) 2010�� ������ͬ�޵��ӹɷ����޹�˾��

��ǰ�汾��Ver 1.0    
��    �ߣ���Զ�� (huyuanfeng@coship.com)

����ժҪ��
		�ļ�����˵��

�޸ļ�¼��
�汾��		����		����			�޸�����
--------	----------	----------		------------
1.0			2010-5-11	��Զ��	����
*******************************************************************************/
#include "cs_cainterface.h"
#include "cs_cainterface_pvrext.h"

#ifndef CSFINESUMCAINTERFACE_H
#define CSFINESUMCAINTERFACE_H
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 ͷ�ļ�����                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 �ꡢ���塢�ṹ��                           *
 ******************************************************************************/
#define	CS_FSCA_MSG_BASE    			(0)
#define	CS_FSCA_ALARM_MSG_BASE    		(100)
#define	CS_FSCA_INFO_MSG_BASE    		(200)
#define	CS_FSCA_COMMAND_MSG_BASE    	(300)
#define	CS_FSCA_STATUS_MSG_BASE    		(400)
#define	CS_FSCA_MMI_MSG_BASE    		(500)

#define MAX_CAINFO_LEN   	   			(10)		/*CA������Ϣ��󳤶�*/
#define MAX_CAVERSION_LEN	   			(10)	 	/*CAģ��汾�ų���*/
#define	MAX_OSD_LEN						(184)		/*OSD���ݵ���󳤶�*/
#define CARDID_LEN						(8)			/*���ų���*/
#define ZONEID_LEN         				(4)			/*������ID*/

#define MAX_EMAIL_TITLE_LEN	     		(20)		/*�ʼ�����ĳ���*/
#define MAX_EMAIL_NUM            		(100)		/*�����б��������ʼ�����*/
#define MAX_EMAIL_CONTENT_LEN    		(180)		/*�ʼ����ݵĳ���*/

typedef enum _CaEventGroup
{
	CA_ALARM = CS_FSCA_MSG_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	
	CA_EVENTGROUP_MAX
}CaEventGroup_E;

typedef enum _CaAlarm
{
	CA_ALARM_CANCEL = CS_FSCA_ALARM_MSG_BASE,	/*ȡ����ǰ��Ϣ��ʾ*/
	CA_ALARM_FAIL, 								/*����ʧ��*/
	CA_ALARM_BADCARD, 							/*�޷�ʶ��Ŀ�������ʹ��*/
	CA_ALARM_NOCARD, 							/*��Ƭû�в���*/
	CA_ALARM_BADVERSION, 						/*���ݸ�ʽ�汾��֧��*/
	CA_ALARM_BADCAS, 							/*��֧�ֵ�CAϵͳ*/
	
	CA_ALARM_BADSP, 							/* 6 ��֧�ֵ�������Ӫ��*/
	CA_ALARM_BADFILTER, 						/*����Ĺ��˹���*/
	CA_ALARM_SERVSTART, 						/*�������ò����ɹ�*/
	CA_ALARM_SERVPAUSE, 						/*���񶳽�����ɹ�*/
	CA_ALARM_PAIRING, 							/*��Ŀ��Ҫ������Ӧ*/
	
	CA_ALARM_PAUSED, 							/* 11 �����Ѿ�������*/
	CA_ALARM_CARDDELETE, 						/*��ɾ�������ɹ�*/
	CA_ALARM_FILESYSBAD, 						/*�ļ�ϵͳ����*/
	CA_ALARM_LEVELERROR, 						/*�����տ��Ľ�Ŀ����*/
	CA_ALARM_DECRYPTFAIL, 						/*���ܳ���*/
	
	CA_ALARM_DATAERROR, 						/* 16 ��������ݸ�ʽ*/
	CA_ALARM_NEAREND, 							/*��Ȩ�������ڣ��뼰ʱ��ֵ*/
	CA_ALARM_PAIRINGFAIL,						/*�������ʧ��*/
	CA_ALARM_NOENTITLE,							/*û����Ȩ*/
	CA_ALARM_NEWEMAIL,							/*���ʼ�֪ͨ*/
	
	CA_ALARM_EMAILFULL,							/* 21 �ʼ�����*/
	CA_ALARM_NOMONEY,							/*����Ǯ������*/
	CA_ALARM_ISSONCARD,							/*��ǰ��ƬΪ�ӿ����޷���ȡι������*/
	CA_ALARM_NEEDECM,							/*δ֪�ĵ�ǰʱ�䣬��Ƭ��Ҫ�Ȼ�ȡECM����*/
	CA_ALARM_ISMOTHERCARD,						/*��ǰ��ƬΪĸ��������Ҫ��ȡι������*/
	
	CA_ALARM_BADDATA,							/* 26 �����ι������*/
	CA_ALARM_NOTINIT,							/*��Ƭδ��ɳ�ʼ��*/
	CA_ALARM_NOOVERTIME,						/*��ǰ��Ƭ��ĸ�������ӿ�����ʱ��*/
	CA_ALARM_SONCARDEXRIE,						/*�ӿ����ڣ���Ҫ����ĸ��*/
	CA_ALARM_NITCHECKERROR,						/*NIT��������֤ʧ��*/
	
	CA_ALARM_NITZONEERROR,						/* 31 ���ܿ�δͨ��������֤����Ҫ������ȷ��NIT��*/
	CA_ALARM_HIDEINFOERROR,						/*��ȡ��Ŀ������Ϣ����*/
	CA_ALARM_UPDATEFILTER,						/*���¹��˹���*/
	CA_ALARM_GETMOTHERCARDDATA,					/*��ȡĸ�����ݳɹ�*/
	CA_ALARM_SETSONCARDDATA,					/*�ӿ��������ݳɹ�*/

	CA_ALARM_SERVFREE,							/* 36 ��ѽ�Ŀ*/
	CA_ALARM_SERVOTHER,							/*����CA���Ž�Ŀ*/
	
    CA_ALARM_MAX
}CaAlarm_E;

typedef enum _CaCommand
{
	//�ʼ�״̬����
	CA_COMMAND_EMAIL = CS_FSCA_COMMAND_MSG_BASE,/*dwParm2 = (DWORD)CaEmailIconType_E*/
	//ָ��
	CA_COMMAND_FINGERPRINT,						/*dwParm2 = (DWORD)CardId, if equal to zero,means cancel fingerprint */
	//��ʾOSD��Ϣ
	CA_COMMAND_SHOW_OSD,						/*dwParm2 = (DWORD)&CaOsdInfo_S*/
	//����OSD��Ϣ
	CA_COMMAND_HIDE_OSD,						/*dwParm2 = (WORD)CaOsdStyle_E*/

	CA_COMMAND_MAX
}CaCommand_E;

typedef enum _CaStatus
{
	CA_STATUS_SMC_IN = CS_FSCA_STATUS_MSG_BASE,
	CA_STATUS_SMC_OUT,
	CA_STATUS_SMC_OK,
	CA_STATUS_SMC_ERROR,
	
	CA_STATUS_MAX
}CaStatus_E;

typedef enum _CaEmailIconType
{
	CA_EMAIL_HIDE_ICON,				/*�����ʼ�֪ͨͼ��*/
	CA_EMAIL_NEW_ICON,				/*���ʼ�֪ͨ����ʾ���ʼ�ͼ��*/
	CA_EMAIL_FULL_ICON				/*���̿ռ�������ͼ����˸��*/	
}CaEmailIconType_E;

typedef enum _CaOsdStyle
{
	CA_OSD_TOP,						/*����Ļ�Ϸ���ʾ*/
	CA_OSD_BOTTOM					/*����Ļ�·���ʾ*/
}CaOsdStyle_E;

typedef enum _CA_ReturnValue
{
	CA_RETURN_OK, 					/*�����ɹ�*/
	CA_RETURN_FAIL, 				/*����ʧ��*/
	CA_RETURN_INVALIDCARD, 			/*IC����Ч����IC������*/
	CA_RETURN_CARDNOTPAIRING,		/* IC��û�ж�Ӧ�κλ�����*/
	CA_RETURN_CARDPAIRED,			/* IC���Ѷ�Ӧ����������*/
	CA_RETURN_VERSIONERROR,			/*��֧�ֵ��ں˰汾*/
	CA_RETURN_POINTINVALID,			/*ָ��Ϊ��*/
	CA_RETURN_DATANOTFOUND,			/*����û���ҵ�*/
	CA_RETURN_UNKOWNCAID,			/*δ֪CAID*/
	CA_RETURN_ISSONCARD,			/*��ǰ��ƬΪ�ӿ����޷���ȡι������*/	
	CA_RETURN_NEEDECM,				/*δ֪�ĵ�ǰʱ�䣬��Ƭ��Ҫ�Ȼ�ȡECM����*/
	CA_RETURN_ISMOTHERCARD,			/*��ǰ��ƬΪĸ��������Ҫ��ȡι������*/
	CA_RETURN_BADDATA,				/*�����ι������*/
	CA_RETURN_INSERT_SONCARD,		/*������ӿ�*/
	CA_RETURN_NO_ZONE_CONTROL,		/*�ÿ�Ƭ���������*/
	CA_RETURN_HIDE_INFO_ERROR		/*��ȡ��Ŀ������Ϣ����*/
}CA_ReturnValue_E;

typedef struct _CaOsdInfo
{
	CaOsdStyle_E 		m_eStyle;
	CHAR				m_acMessage[MAX_OSD_LEN];
}CaOsdInfo_S;

typedef struct _CaInfo
{
	CHAR	m_acCAProvider[MAX_CAINFO_LEN +1];				/*CA�ṩ������*/
	CHAR	m_acCAVersion[MAX_CAVERSION_LEN +1];			/*CAģ��汾��*/
	BYTE	m_ucServiceProviderID;							/*��Ӫ��ID*/
	BYTE    m_ucCardControlLevel;                       	/*���ܿ���Ŀ�ȼ�*/
	BYTE    m_aucCardId[CARDID_LEN];						/*���ܿ�����*/
	BYTE    m_aucZoneId[ZONEID_LEN];						/*���ܿ�����ID*/
}CaInfo_S;

typedef struct  _CaDate
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
}CaDate_S;

typedef struct _CaEmailContent
{
	CHAR		m_acEmailContent[MAX_EMAIL_CONTENT_LEN+1];	/*Email������*/
}CaEmailContent_S;

typedef struct _CaEmailTitle                       
{
	DWORD		m_dwEmailID;                                /*Email ID  */
	BYTE		m_ucNewEmail;                               /*0 �������ʼ� 1�����ʼ� */
	CHAR   		m_acEmailHead[MAX_EMAIL_TITLE_LEN+1];  		/*�ʼ����⣬�Ϊ30 */
	WORD 		m_wImportance;								/*��Ҫ��, 0����ͨ��1����Ҫ */
	CaDate_S	m_stCreateTime;                             /*EMAIL������ʱ��*/
}CaEmailHead_S;

typedef enum _CaQAMType
{
	CA_QAMTYPE_QPSK,
	CA_QAMTYPE_8QAM,
	CA_QAMTYPE_16QAM,
	CA_QAMTYPE_32QAM,
	CA_QAMTYPE_64QAM,
	CA_QAMTYPE_128QAM
}CaQAMType_E;

typedef struct _CaNITFrequenceInfo
{
	DWORD		m_dwFrequenceKHz;		//Ƶ��KHz
	DWORD		m_dwSymbolKBps;			//������KBps
	CaQAMType_E	m_dwQamType;			//���Է�ʽ
}CaNITFrequenceInfo_S;

/******************************************************************************
 *                                 ��������                                   *
 ******************************************************************************/

/******************************************************************************
��������:	CSCAGetMailNum

��������:
		��ȡ�ʼ�������ʣ���ʼ��ռ�

�������:
		��

�������:
		pnEmailNum:�ʼ�����
		pnEmptyNum:ʣ���ʼ��ռ�

�� �� ֵ:
		0 : SUCCESS
		-1: FAILURE
		
����˵��:
*******************************************************************************/
INT CSCAGetMailNum(INT* pnEmailNum, INT* pnEmptyNum);

/******************************************************************************
��������:	CSCAGetMailHead

��������:
		��ȡ�ʼ�������Ϣ

�������:
		nIndex:�ʼ�������(��1��ʼ)

�������:
		pstMailHead:�ʼ��������ݣ���CaEmailHead_S

�� �� ֵ:
		0 : SUCCESS
		-1: FAILURE
		
����˵��:
*******************************************************************************/
INT CSCAGetMailHead(INT nIndex, CaEmailHead_S *pstMailHead);

/******************************************************************************
��������:	CSCAGetMailContentByIndex

��������:
		���������Ż�ȡ�ʼ�����

�������:
		nIndex:�ʼ�������(��1��ʼ)

�������:
		pstMailContent:�ʼ����ݣ���CaEmailContent_S

�� �� ֵ:
		0 : SUCCESS
		-1: FAILURE

����˵��:
*******************************************************************************/
INT CSCAGetMailContentByIndex(INT nIndex, CaEmailContent_S *pstMailContent);

/******************************************************************************
��������:	CSCAGetMailContentByEmailId

��������:
		����EmailId��ȡ�ʼ�����

�������:
		wEmailId:�ʼ�ID��

�������:
		pstMailContent:�ʼ����ݣ���CaEmailContent_S

�� �� ֵ:
		0 : SUCCESS
		-1: FAILURE

����˵��:
*******************************************************************************/
INT CSCAGetMailContentByEmailId(WORD wEmailId, CaEmailContent_S *pstMailContent);

/******************************************************************************
��������:	CSCADelMail

��������:
		����������ɾ��һ���ʼ�

�������:
		nIndex:�ʼ�������(��1��ʼ)

�������:
		��

�� �� ֵ:
		0 : SUCCESS
		-1: FAILURE

����˵��:
*******************************************************************************/
INT CSCADelMail(INT nIndex);

/******************************************************************************
��������:	CSCADelMailAll

��������:
		ɾ�������ʼ�

�������:
		��

�������:
		��

�� �� ֵ:
		0 : SUCCESS
		-1: FAILURE

����˵��:
*******************************************************************************/
INT CSCADelMailAll(void);

/******************************************************************************
��������:	CSCAGetCaInfo

��������:
		��ȡCA������Ϣ

�������:
		��

�������:
		pstCaInfo:CA������Ϣ����CaInfo_S

�� �� ֵ:
		0 : SUCCESS
		-1: FAILURE

����˵��:
*******************************************************************************/
INT CSCAGetCaInfo(CaInfo_S *pstCaInfo);

/******************************************************************************
��������:	CSCAGetPairstatus

��������:
		��ȡ����������

�������:
		��

�������:
		��

�� �� ֵ:
		CA_RETURN_OK			--������Գɹ�						
		CA_RETURN_CARDNOTPAIRING--IC��û�ж�Ӧ�κλ�����	
		CA_RETURN_CARDPAIRED	--IC���Ѷ�Ӧ����������		
		CA_RETURN_INVALIDCARD	--����Ч��δ����	
		
����˵��:
*******************************************************************************/
CA_ReturnValue_E CSCAGetPairstatus(void);

/******************************************************************************
��������:	CSCANotifyShowOSDOver

��������:
		֪ͨCAֹͣ��ʾOSD��Ϣ

�������:
		��

�������:
		��

�� �� ֵ:
		��

����˵��:
		�ڽ��յ�CA_COMMAND_SHOW_OSD��Ϣ��OSD��������һ�ܺ�
		��������ӿ���֪ͨCAֹͣOSD��ʾ
*******************************************************************************/
void CSCANotifyShowOSDOver(void);

/******************************************************************************
��������:	CSCAControlZone

��������:
		��ȡ��Ƭ�����������

�������:
		������ֵΪCA_RETURN_NO_ZONE_CONTROLʱ������������
		stNitFreInfo:������Ҫ���յ�NIT���Ƶ����Ϣ	
�������:
		��

�� �� ֵ:
		CA_RETURN_FAIL				--����ʧ��
		CA_RETURN_OK				--�����ɹ�						
		CA_RETURN_INVALIDCARD		--IC��Ч��Ƭ	
		CA_RETURN_NO_ZONE_CONTROL	--�ÿ�Ƭ���������	
		
����˵��:
*******************************************************************************/
CA_ReturnValue_E CSCAControlZone(CaNITFrequenceInfo_S* pstNitFreInfo);

/******************************************************************************
��������:	CSCAGetControlLevel

��������:
		��ȡ��Ƭ�ȼ�	

�������:
		ucLevel:���ؿ�Ƭ�Ľ�Ŀ���صȼ�		
�������:
		��

�� �� ֵ:
		CA_RETURN_UNKOWNCAID		--δ֪CAID
		CA_RETURN_FAIL				--����ʧ��
		CA_RETURN_OK				--�����ɹ�						
		CA_RETURN_INVALIDCARD		--IC��Ч��Ƭ	
		FSCA_RETURN_HIDE_INFO_ERROR	--��ȡ��Ŀ������Ϣ����
		
����˵��:
*******************************************************************************/
CA_ReturnValue_E CSCAGetControlLevel(BYTE* ucLevel);

/******************************************************************************
��������:	CSCAGetCardID

��������:
		��ȡ��ƬID	

�������:
		paucCardID:���ؿ�Ƭ��ID	(8 �ֽ�)		
�������:
		��

�� �� ֵ:
		CA_RETURN_FAIL				--����ʧ��
		CA_RETURN_OK				--�����ɹ�						
		CA_RETURN_INVALIDCARD		--IC��Ч��Ƭ	
		
����˵��:
*******************************************************************************/
CA_ReturnValue_E CSCAGetCardID(BYTE* paucCardID);

/******************************************************************************
��������:	CSCAGetZoneID

��������:
		��ȡ��Ƭ����ID	
		
�������:
		paucZoneID:���ؿ�Ƭ����ID	(4�ֽ�)	
�������:
		��

�� �� ֵ:
		CA_RETURN_FAIL				--����ʧ��
		CA_RETURN_OK				--�����ɹ�						
		CA_RETURN_INVALIDCARD		--IC��Ч��Ƭ	
		
����˵��:
*******************************************************************************/
CA_ReturnValue_E CSCAGetZoneID(BYTE* paucZoneID);

#ifdef __cplusplus
}
#endif
#endif  /* CSFINESUMCAINTERFACE_H */

/* �ļ����� */

