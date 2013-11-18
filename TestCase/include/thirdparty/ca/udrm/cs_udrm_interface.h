/******************************************************************************
 *�ļ����ƣ�cs_udrm_interface.h
 *��Ȩ����(c) 2010�� ������ͬ�޵��ӹɷ����޹�˾��
 *
 *��ǰ�汾��Ver 1.0    
 *��    �ߣ������� (wansuanlin@coship.com)
 *
 *����ժҪ��
 *	�ļ�����˵��
 *
 *	�޸ļ�¼��
 *	�汾��		����		����			�޸�����
 *	--------	----------	----------		------------
 *	1.0			2010-06-24	������			����
*******************************************************************************/

/******************************************************************************
 *                                 ͷ�ļ�����                                 *
 ******************************************************************************/
#include "udiplus_typedef.h"
#include "cs_cainterface.h"

#ifndef _CS_UDRM_INTERFACE_H_
#define _CS_UDRM_INTERFACE_H_
#ifdef  __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 �ꡢ���塢�ṹ��                           *
 ******************************************************************************/

#define	CS_UDRM_MSG_BASE                (0)
#define	CS_UDRM_ALARM_MSG_BASE          (100)
#define CS_UDRM_INFO_MSG_BASE           (200)
#define	CS_UDRM_COMMAND_MSG_BASE        (300)

typedef enum _CaEventGroup
{
	CA_ALARM = CS_UDRM_MSG_BASE,//0
	CA_INFO,
	CA_COMMAND,
	CA_STATUS,
	CA_MMI,
	
	CA_EVENTGROUP_MAX
}CaEventGroup_E;

typedef enum _CaAlarm
{
	CA_ALARM_SERVFREE = CS_UDRM_ALARM_MSG_BASE, /*������Ŀ*/
	CA_ALARM_SECURE_CHIPSET_ERROR,              /*UTI��ȫоƬ����*/
	CA_ALARM_SECURE_CHIPSET_INVALID,            /*UTI��ȫоƬ��Ч*/
	CA_ALARM_NO_OPERTOR,                        /*�ն�û�д���Ӫ��*/
	CA_ALARM_NVRAM_ERROR,                       /*NVRAM����*/
	CA_ALARM_NO_SIGNAL,                         /*û���ź�*/
	
	CA_ALARM_PROGRAM_ABNORMITY,                 /*6 ��Ŀ�쳣������ϵ��Ӫ��*/
	CA_ALARM_NOENTITLE,                         /*��Ŀδ��Ȩ*/
	CA_ALARM_CANCEL,                            /*����ȡ����ǰ��Ϣ��ʾ*/
	CA_ALARM_PROGRAM_AREA_LIMIT,                /*��Ŀ�������Ʋ���*/
	CA_ALARM_WATCHLEVEL,                        /*��Ŀ���˼�������*/

	CA_ALARM_SHOW_FINGER,                       /*11 ָ����ʾ*/
	CA_ALARM_ORDER_LIMIT,                       /*������������*/
	CA_ALARM_PROGRAM_PREVIEW,                   /*��ĿԤ��*/
	CA_ALARM_PRODUCT_ORDERED,                   /*��Ʒ�Ѿ�����*/
	CA_ALARM_AUTHOR_NUMBER_MAX,                 /*��Ȩ��¼�Ѵﵽ�����Ŀ*/

	CA_ALARM_NOT_ACCOUNT,                       /*16 δ����*/
	CA_ALARM_BLACKOUT,                          /*��������*/
	CA_ALARM_PWD_RESET,                         /*����������*/
	CA_ALARM_LINK_SERVER_FAIL,                  /*���ӷ�����ʧ��*/
	CA_ALARM_PROGRAM_PROTECT_BYDRM,             /*����Ŀ��DRM����*/
	
	CA_ALARM_SECURE_CHIPSET_LOCK,               /*21 UTI��ȫоƬ����*/
	CA_ALARM_SECURE_CHIPSET_UNLOCK,             /*UTI��ȫоƬ�������*/
	CA_ALARM_SERVOTHER,							/*����CA���Ž�Ŀ*/
	
    CA_ALARM_MAX
}CaAlarm_E;

typedef enum _CaCommand
{
	CA_COMMAND_EMAIL = CS_UDRM_COMMAND_MSG_BASE,/*�ʼ�״̬����, dwParm2 = (DWORD)&CaMail_S*/
	CA_COMMAND_SHOW_FINGERPRINT,                /*��ʾָ��, dwParm2 = (DWORD)&CaFingerPrintInfo_S */
	CA_COMMAND_HIDE_FINGERPRINT,                /*����ָ��*/
	CA_COMMAND_SHOW_OSD,                        /*��ʾOSD��Ϣ, dwParm2 = (DWORD)&CaOsdInfo_S*/
	CA_COMMAND_HIDE_OSD,                        /*����OSD, dwParm2 = (WORD)CaOsdStyle_E*/
	CA_COMMAND_IPPV_NOTIFY,                     /*IPPV֪ͨ, dwParam2 = (DWORD)&CaIPPVTInfo_S*/
	CA_COMMAND_PPT_NOTIFY,                      /*PPT֪ͨ, dwParam2 = (DWORD)&CaIPPVTInfo_S*/
	CA_COMMAND_IPPV_CANCEL,                     /*���IPPV�Ի���*/
	CA_COMMAND_PPT_CANCEL,                      /*���PPT�Ի���*/
	CA_COMMAND_OTA_INFO,                        /*OTA������Ϣ, dwParam2 = (DWORD)&CaOtaInfo_S*/

	CA_COMMAND_MAX
}CaCommand_E;

#define MAX_EMAIL_TITLE_LEN	     (20)   /*�ʼ�����ĳ���*/
#define MAX_EMAIL_NUM            (100)  /*�����б��������ʼ�����*/
#define MAX_EMAIL_CONTENT_LEN    (255)  /*�ʼ����ݵĳ���*/
#define MAX_EVENT_INFO_LEN       (64)   /*Event�����Ϣ����*/
#define MAX_UTDRM_MAJORVER_LEN   (8)    /*UT���汾����*/
#define MAX_UTDRM_SUBVER_LEN     (8)    /*UT�Ӱ汾����*/
#define MAX_CAPORTING_VER_LEN    (32)   /*CA Portingģ��汾����*/
#define MAX_UTUSER_SERIALNUM_LEN (32)   /*UT�û����кų���*/
#define PIN_CODE_LEN             (6)    /*PIN�볤��*/
#define MAX_FINGERPRINT_LEN      (20)   /*ָ����Ϣ*/

typedef struct _CaInfo
{
	CHAR	m_acUTMajorVer[MAX_UTDRM_MAJORVER_LEN];        /*UT���汾��*/
	CHAR    m_acUTSubVer[MAX_UTDRM_SUBVER_LEN];            /*UT�Ӱ汾��*/
	CHAR	m_acCAVersion[MAX_CAPORTING_VER_LEN];          /*CA Portingģ��汾��*/
	CHAR    m_acUTUserSerialNum[MAX_UTUSER_SERIALNUM_LEN]; /*UT�û����к�*/
}CaInfo_S;

typedef enum
{
	EM_EMAIL_HIDE_ICON,                 /*�����ʼ�֪ͨͼ��*/
	EM_EMAIL_NEW_ICON,                  /*���ʼ�֪ͨ����ʾ���ʼ�ͼ��*/
	EM_EMAIL_FULL_ICON                  /*���̿ռ�������ͼ����˸*/	
}CaEmailIconType_EM;

typedef struct  _CaDate
{
	DWORD   m_dwYear;
	DWORD   m_dwMonth;
	DWORD   m_dwDay;
	DWORD   m_dwHour;
	DWORD   m_dwMinute;
	DWORD   m_dwSecond;
}CaDate_S;

typedef struct _CaMail
{
	DWORD    m_dwEmailID;                              /*Email ID*/
	BOOL     m_bReaded;                                /*�Ƿ��Ѷ���־:FALSE->��ʾδ�� TRUE->��ʾ�Ѷ�*/
	CHAR     m_acEmailHead[MAX_EMAIL_TITLE_LEN+1];     /*�ʼ����⣬�Ϊ20*/
	WORD     m_wImportance;                            /*0��ǿ����ʾ����������ͨ*/
	CaDate_S m_stCreateTime;                           /*EMAIL������ʱ��*/
	CHAR    m_acEmailContent[MAX_EMAIL_CONTENT_LEN+1];  /*Email������*/
}CaMail_S;/*��ռ312���ֽ�*/

typedef enum _CaOsdStyle
{
	EM_CA_OSD_TOP,					/*����Ļ�Ϸ���ʾ*/
	EM_CA_OSD_BOTTOM,               /*����Ļ�·���ʾ*/
	EM_CA_OSD_TOPBOTTOM             /*����Ļ�ϡ��·�ͬʱ��ʾ*/
}CaOsdStyle_EM;

typedef struct _CaOsdInfo
{
	CaOsdStyle_EM m_emOsdStyle;                        /*OSD��ʾ��ʽ*/
	WORD     m_wImportance;                            /*��Ҫ��, 0����ͨ��1����Ҫ */
	WORD     m_wDurtion;                               /*Ҫ����ʾ��ʱ�䳤�ȣ���λ����*/
	CaDate_S m_stCreateTime;                           /*OSD������ʱ��*/
	CHAR     m_acOsdContent[MAX_EMAIL_CONTENT_LEN+1];  /*OSD����*/
}CaOsdInfo_S;

typedef enum _IPPVTType
{
	EM_PPVTTYPE_PPV,
	EM_PPVTTYPE_IPPV,
	EM_PPVTTYPE_PPT
}IPPVTType_E;

typedef enum _CaReturnCode
{
	EM_RETURNCODE_FAIL = -1,
	EM_RETURNCODE_SUCCESS,
	EM_RETURNCODE_INVALID_PARAM,
	EM_RETURNCODE_NVRAM_FAIL,
	EM_RETURNCODE_PIN_WRONG,
	EM_CAINIT_NOTCOMPLETE
}CaReturnCode_E;

/***************************************************************
 *����CaIPPVTInfo_S�ṹ�壺
 *1.��Ӧ����������CaPorting�ӿڻ�ȡPPV/IPPV/PPTʱ���ýṹ���е�
 *  m_wDmxId��m_wServiceId�ֶ���Ч;�������ֲ�����, 
 *  1).�����PPV����,��m_aucEventInfo�ֶ���Ч����ΪEvent name,������������ʱ�����ֶ���Ч;
 *  2).�����PPV����,��m_dwPrice�ֶ���Ч,��������������Ч;
 *  3).m_dwPayUnit�ֶ�ֻ���PPT��Ч;
 *2.��CaPorting�����ϱ�CA_COMMAND_IPPV_NOTIFY��CA_COMMAND_PPT_NOTIFY
 *  ʱ��m_wDmxId��m_wServiceId�ֶ���Ч;
 */
typedef struct _IPPVTInfo
{
	BYTE     m_ucIppvType;  /*0:PPV, 1:IPPV, 2:PPT*/
	DWORD    m_dwOperatorIndex;
	DWORD    m_dwSectorIndex;
	WORD     m_wDmxId;
	WORD     m_wServiceId;
	DWORD    m_dwEventId;
	BYTE     m_aucEventInfo[MAX_EVENT_INFO_LEN];
	DWORD    m_dwPrice;
	DWORD    m_dwPayUnit;   /*���ֶ�ֻ����PPT��Ŀ����Ч*/
	CaDate_S m_stStartTime;
	CaDate_S m_stEndTime;
}CaIPPVTInfo_S;

typedef struct _ProductInfo
{
	DWORD    m_dwOperatorId;
	DWORD    m_dwSectorId;
	DWORD    m_dwProductId;
}CaProductInfo_S;

typedef struct _WalletInfo
{
	DWORD    m_dwOperatorId;
	DWORD    m_dwSectorId;
	DWORD    m_dwValue;
}CaWalletInfo_S;

typedef struct _FingerPrint
{
	CHAR     m_acFingerPrint[MAX_FINGERPRINT_LEN];
}CaFingerPrintInfo_S;

typedef struct _OtaInfo
{
	DWORD     m_dwManufactureId;              //����ID������ָ��
	DWORD     m_dwModuleId;                   // �豸ID������ָ��
	DWORD     m_dwLoaderVersion;              // loader�汾������ָ��
	DWORD     m_dwSoftwareVersion;            // ����汾
	DWORD     m_dwServiceId;                  // ҵ���
	DWORD     m_dwOperatorIdMatchValid;      // �Ƿ�ƥ����Ӫ�̵ı�־ 0 - ����ƥ�� 1- ��Ҫƥ��
	DWORD     m_dwOperatorId;                 // ��Ӫ��ID
	DWORD     m_dwAreaIdMatchValid;          // �Ƿ�ƥ������ŵı�־ 0 - ����ƥ�� 1 -��Ҫƥ��
	DWORD     m_dwAreaId;                    // �����
	DWORD     m_dwSoftwareVersionMatchValid; // �Ƿ���Ҫƥ���ض�����汾�ŵı�־ 0 - ����ƥ�� 1 - ��Ҫƥ��
	DWORD     m_dwCertainSoftwareVersion;     // �ض�����汾��
	DWORD     m_dwGlobalTriggerValid;         //�Ƿ���ȫ�ִ���
	DWORD     m_dwGlobalTriggerType;          //ȫ�ִ�������, 0��Ϊǿ��������1����ǿ������
	DWORD     m_dwEMMTriggerType; 		      //˽�д������ͣ�0��Ϊǿ��������1����ǿ������
}CaOtaInfo_S;

/******************************************************************************
 *                                 ��������                                   *
 *˵����
 *	����Ӧ����Ҫ��ȡ��Ϣ�����Ҫ�ռ䶼��Ӧ�÷��䣬���磬��ȡ����Ǯ����Ϣ��
 *		CaWalletInfo_S����ṹ�ռ���Ӧ�÷���
 ******************************************************************************/

/******************************************************************************
��������:	CSCAGetSNFromTestMode

��������:
		����ģʽ�»�ȡ��ʽSN�ţ���������ʱ���ɱ����ļ�

�������:
		pacSN : Ӧ�÷���һ����С��32���ֽڵ�Buffer�����ڴ洢SN��
		pnLen : Ӧ�ø�֮�����Buffer��Сֵ

�������:
		pacSN : ����SN�ţ��ַ�����ʽ
		pnLen : ������ʵ���ַ�������

�� �� ֵ:
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������

����˵��:
*******************************************************************************/
INT CSCAGetSNFromTestMode(CHAR *pacSN, INT *pnLen);

/******************************************************************************
��������:	CSCAGetUDRMAreaValue

��������:
		��ȡ����ֵ

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1
		pdwAreaValue    : ����һ��DWORD���͵�Buffer�����ڴ洢����ֵ

�������:
		pdwAreaValue : ��������ֵ

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetUDRMAreaValue(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwAreaValue);

/******************************************************************************
��������:	CSCAAppInfoSet

��������:
		CA��ʼ�����֮�󣬵��ô˺��������̴��ţ�����ID��Loader�汾����CA�ں�

�������:
		wManufactureId : ���̴���
		wModuleId      : �豸ID
		wLoaderVersion : Loader�汾

�������:
		��

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ����ʧ��
		EM_RETURNCODE_SUCCESS       : ���óɹ�
		
����˵��:
*******************************************************************************/
INT CSCAAppInfoSet(DWORD dwManufactureId, DWORD dwModuleId, DWORD dwLoaderVersion);

/******************************************************************************
��������:	CSCALoaderStatus

��������:
		�˺�������Ӧ�ÿ���ʵʱ�鿴�Ƿ���������Ϣ(��ע����������⵽��������Ϣ��
		CAֻ���ϱ�һ��CA_COMMAND_OTA_INFO��Ϣ)

�������:
		pstLoaderStatus : Ӧ�÷���ռ䣬���ڴ洢��ǰ��������Ϣ

�������:
		pstLoaderStatus : ���ظ�ֵ

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ��ǰû��������Ϣ
		EM_RETURNCODE_SUCCESS       : ��ǰ����ȷ��������Ϣ
		EM_RETURNCODE_INVALID_PARAM : ��ǰ��������������Ϣ����CA�ں�û�й��˵�������Ϣ
		
����˵��:
		�鿴��״̬ʱ��Ҫ������������ֵ������ʾ��Ϣ��
*******************************************************************************/
INT CSCALoaderStatus(CaOtaInfo_S *pstLoaderStatus);

/******************************************************************************
��������:	CSCAGetMailNum

��������:
		��ȡ�ʼ�������ʣ���ʼ��ռ�

�������:
		��

�������:
		pnTotalEmailNum  : �ʼ�����
		pnNoReadEmailNum : δ���ʼ�����

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��
		
����˵��:
*******************************************************************************/
INT CSCAGetMailNum(INT* pnTotalEmailNum, INT* pnNoReadEmailNum);

/******************************************************************************
��������:	CSCASetMailNum

��������:
		������Ҫ�洢���ʼ������������ĸ���Ϊ209��

�������:
		pnTotalEmailNum	����Ҫ�洢���ʼ�����

�������:
 		pnTotalEmailNum	��ʵ���赽CA�е���ȷ�ʼ�����


�� �� ֵ:
		SUCCESS:           ��   ��
		FAILURE:           ���뺯�����ǿ�ָ��
		
����˵��:
*******************************************************************************/
INT CSCASetMailNum(INT* pnTotalEmailNum);

/******************************************************************************
��������:	CSCAGetMailContentByIndex

��������:
		���������Ż�ȡ�ʼ�����

�������:
		nIndex:�ʼ�������(��0��ʼ)

�������:
		pstMailContent:�ʼ����ݣ���CaEmailContent_S

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetMailByIndex(INT nIndex, CaMail_S *pstMailInfo);

/******************************************************************************
��������:	CSCAGetMailContentByEmailId

��������:
		����EmailId��ȡ�ʼ�����

�������:
		dwEmailId:�ʼ�ID��

�������:
		pstMailInfo:�ʼ����ݣ���CaMail_S

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetMailByEmailId(DWORD dwEmailId, CaMail_S *pstMailInfo);

/******************************************************************************
��������:	CSCASetMailReadedFlag

��������:
		ͨ��EmailId�����ʼ��Ѷ���־

�������:
		dwEmailId : �ʼ���ʶ��
		bReadFlag : �Ѷ���־

�������:
		��

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCASetMailReadedFlag(DWORD dwEmailId, BOOL bReadFlag);

/******************************************************************************
��������:	CSCADelMail

��������:
		����������ɾ��һ���ʼ�

�������:
		dwEmailId:�ʼ���ʶ��

�������:
		��

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCADelMail(DWORD dwEmailId);

/******************************************************************************
��������:	CSCADelMailAll

��������:
		ɾ�������ʼ�

�������:
		��

�������:
		��

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

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
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetCaInfo(CaInfo_S *pstCaInfo);

/******************************************************************************
��������:	CSCAGetOperatorNumber

��������:
		��ȡ��Ӫ�̸���

�������:
		��

�������:
		pdwOperatorNum:���������Ӫ�̸���

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetOperatorNumber(DWORD *pdwOperatorNum);

/******************************************************************************
��������:	CSCAGetSectorNumber

��������:
		��ȡÿ����Ӫ����������(�����൱�ڽ�Ŀ�ṩ��)

�������:
		dwOperatorIndex:��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1

�������:
		pdwSectorNum:���ÿ����Ӫ����������

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetSectorNumber(DWORD dwOperatorIndex, DWORD *pdwSectorNum);

/******************************************************************************
��������:	CSCAGetProductNumber

��������:
		��ȡÿ�������Ĳ�Ʒ����

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1

�������:
		pdwProductNum:��Ʒ����

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetProductNumber(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwProductNum);

/******************************************************************************
��������:	CSCAGetProductInfo

��������:
		��ȡÿ����Ʒ����Ϣ

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1
		dwProductIndex  : ��Ʒ��ʶ�ţ���0��ʼ�����ֵΪ��Ʒ������1

�������:
		pstProductInfo:��Ʒ��Ϣ

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetProductInfo(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD dwProductIndex, CaProductInfo_S* pstProductInfo);

/******************************************************************************
��������:	CSCAGetIPPVTNumber

��������:
		1.�������emIppvtTypeΪ��EM_PPVTTYPE_PPV--��ȡÿ��������PPV(Pay Pre View���θ���)����
		2.�������emIppvtTypeΪ��EM_PPVTTYPE_IPPV--��ȡÿ��������IPPV(��ע�⣺Impulse Pay Pre View���˰��θ���)����
		3.�������emIppvtTypeΪ��EM_PPVTTYPE_PPT--��ȡÿ��������PPT(Pay Pre Time��ʱ����)����

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1
		emIppvtType     : PPV/IPPV/PPT����֮һ

�������:
		pdwIPPVTNum:�����Ӧ��PPV��IPPV��PPT����

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetIPPVTNumber(DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD *pdwIPPVTNum);

/******************************************************************************
��������:	CSCAGetIPPVTInfo

��������:
		1.�������emIppvtTypeΪ��EM_PPVTTYPE_PPV--��ȡÿ��PPV�¼���Ϣ
		2.�������emIppvtTypeΪ��EM_PPVTTYPE_IPPV--��ȡÿ��IPPV�¼���Ϣ
		3.�������emIppvtTypeΪ��EM_PPVTTYPE_PPT--��ȡÿ��PPT�¼���Ϣ

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1
		emIppvtType     : PPV/IPPV/PPT����֮һ
		dwIPPVTIndex    : PPV/IPPV/PPT��ʶ�ţ���0��ʼ�����ֵΪ������1

�������:
		pstIPPVTInfo:���PPV��IPPV��PPT��Ϣ

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetIPPVTInfo(DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD dwIPPVTIndex, CaIPPVTInfo_S* pstIPPVTInfo);

/******************************************************************************
��������:	CSCAOrderIPPVT

��������:
		1.�������emIppvtTypeΪ��EM_PPVTTYPE_IPPV--����IPPV
		2.�������emIppvtTypeΪ��EM_PPVTTYPE_PPT--����PPT

�������:
		bIsOrder        : TRUE��ʾ���� FALSE��ʾȡ������Ի���
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1
		emIppvtType     : PPV/IPPV/PPT����֮һ
		dwEventId       : IPPV�¼���

�������:
		��

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAOrderIPPVT(BOOL bIsOrder, DWORD dwOperatorIndex, DWORD dwSectorIndex, IPPVTType_E emIppvtType, DWORD dwEventId);

/******************************************************************************
��������:	CSCAGetViewLevel

��������:
		��ȡ�ۿ�����

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1

�������:
		pdwLevel:����ۿ�����

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��

����˵��:
*******************************************************************************/
INT CSCAGetViewLevel(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD *pdwLevel);

/******************************************************************************
��������:	CSCASetViewLevel

��������:
		���ùۿ�����

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1
		dwLevel         : ϣ�����õĹۿ�����
		paucPin         : PIN�룬��unsigned char�������飬����Ϊ6

�������:
		��

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��
		EM_RETURNCODE_PIN_WRONG     : PIN�����

����˵��:
*******************************************************************************/
INT CSCASetViewLevel(DWORD dwOperatorIndex, DWORD dwSectorIndex, DWORD dwLevel, BYTE *paucPin);

/******************************************************************************
��������:	CSCACheckPin

��������:
		У��PIN��

�������:
		paucPin         : PIN�룬��unsigned char�������飬����Ϊ6

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��
		EM_RETURNCODE_PIN_WRONG     : PIN�����

����˵��:
*******************************************************************************/
INT CSCACheckPin(DWORD dwOperatorIndex, DWORD dwSectorIndex, BYTE *paucPin);

/******************************************************************************
��������:	CSCASetPin

��������:
		�޸�PIN��

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1
		paucOldPin      : ��PIN�룬��unsigned char�������飬����Ϊ6
		paucNewPin      : ��Ҫ���µ�PIN�룬��unsigned char�������飬����Ϊ6

�������:
		��

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��
		EM_RETURNCODE_PIN_WRONG     : PIN�����

����˵��:
*******************************************************************************/
INT CSCASetPin(DWORD dwOperatorIndex, DWORD dwSectorIndex, BYTE *paucOldPin, BYTE *paucNewPin);

/******************************************************************************
��������:	CSCAGetWalletInfo

��������:
		��ȡ����Ǯ����Ϣ

�������:
		dwOperatorIndex : ��Ӫ�̱�ʶ�ţ���0��ʼ�����ֵΪ��Ӫ�̸�����1
		dwSectorIndex   : ������ʶ�ţ���0��ʼ�����ֵΪ����������1

�������:
		pstWalletInfo : �������Ǯ����Ϣ

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������
		EM_RETURNCODE_NVRAM_FAIL    : NVRAMʧ��
		EM_RETURNCODE_PIN_WRONG     : PIN�����

����˵��:
*******************************************************************************/
INT CSCAGetWalletInfo(DWORD dwOperatorIndex, DWORD dwSectorIndex, CaWalletInfo_S *pstWalletInfo);

/******************************************************************************
��������:	CSCAGetUDRMBuildInformation

��������:
		��ѯUDRM�汾������Ϣ

�������:
		paucBuff : �洢�汾������Ϣ��buf
		nBuffSize   : �洢�汾������Ϣ��buf�Ĵ�С������Ϊ128�ֽڣ����С��128�ֽڣ�����������EM_RETURNCODE_INVALID_PARAM

�������:
		paucBuff : �汾������Ϣ

�� �� ֵ:
		EM_RETURNCODE_FAIL          : ʧ��
		EM_RETURNCODE_SUCCESS       : �ɹ�
		EM_RETURNCODE_INVALID_PARAM : ��������	

����˵��:
*******************************************************************************/
INT CSCAGetUDRMBuildInformation(BYTE *paucBuff, int nBuffSize);

#ifdef  __cplusplus
}
#endif

#endif //_CS_UDRM_INTERFACE_H_
