/**@defgroup ABV CA interface
@brief ABVCA��Ӧ���ṩͷ�ļ�
@note  Copyright Shenzhen Coship Electronics Co., Ltd., 

@version 1.0  2011/12/31  Yuhongdan/Yuanqinlong
@{
*/

#ifndef _CS_ABVCA_INTERFACE_H_
#define _CS_ABVCA_INTERFACE_H_

#include "udiplus_typedef.h"
#include "cs_cainterface.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief CA Message������*/
typedef enum _CaMessage
{
	CA_COMMAND,
	CA_ALARM,
	CA_STATUS
}CaMessage_E;

/**@brief ALARM��Ϣ
@note ���յ���Ŀ���ܹۿ��Ĵ�����Ϣʱ������Ϣ�ص��ĵ�3����������
@ֹͣ��Ƶ����Ƶ��������Ƶ����������Ƶ�������ŵ����*/
typedef enum _CaAlarm
{
	CA_ALARM_CA_NO_ERR,                ///< 0�����ʾ
	CA_ALARM_STB_SC_NO_PAIRED, 	       ///< 1������ƥ��(��Ŀ�����)
	CA_ALARM_SC_ERROR, 			       ///< 2��������ܿ�(��Ŀ�����)
	CA_ALARM_NO_SC, 			       ///< 3��������ܿ�(��Ŀ�����)
	CA_ALARM_READ_WRITER_ERR, 	       ///< 4������ʧ��(��Ŀ�����)
	CA_ALARM_NOT_IN_WATCH_TIME,        ///< 5�������õĽ�Ŀ�ۿ�ʱ��֮��(��Ŀ�����)
	CA_ALARM_CARD_OUT_OF_DATE, 	       ///< 6���ܿ�����(��Ŀ���)
	CA_ALARM_NO_ENTITLE, 		       ///< 7û����Ȩ(��Ŀ���)
	CA_ALARM_STB_AREA_RESTRECT,        ///< 8��������(��Ŀ���)
	CA_ALARM_CA_NO_PAIRED, 		       ///< 9��Ҫ�������(��Ŀ���)
	CA_ALARM_ECM_UNKNOWEN_ERROR,       ///< 10����Ľ�Ŀ��Ϣ(��Ŀ���)
	CA_ALARM_NOT_CONTACT_MASTER,       ///< 11û��ĸ��(��Ŀ���) 
	CA_ALARM_IPPV_PRO_NO_ENOUGH_TOKEN, ///< 12IPPV ��Ŀ,����(��Ŀ���)
	CA_ALARM_WAIT_ENTITLE,	           ///< 13�ȴ�������Ȩ(��Ŀ���)
	CA_ALARM_SRV_FREE,                 ///< 14������Ŀ
	CA_ALARM_SRV_OTHER,		           ///< 15����CA���Ž�Ŀ
	CA_ALARM_SRV_ABV,                  ///< 16��CA���Ž�Ŀ
	CA_ALARM_MAX
	
}CaAlarm_E;

/**@brief COMMAND��Ϣ*/
typedef enum  _CaCommand
{
    CA_COMMAND_NEW_EMAIL,		  ///< 0(��Ŀ�����)��ʾ���������ʼ�ͼ��dwParm2=1��ʾ��dwParm2=2����
 	CA_COMMAND_FULL_EMAIL,		  ///< 1(��Ŀ�����)��ʾ���������ʼ�ͼ��dwParm2=1��ʾ��dwParm2=2����
	CA_COMMAND_ECM_FINGERPRINT,	  ///< 2(��Ŀ���)ECMָ�� dwParm2 = (DWORD)&ECM_FINGERPRINT_S
	CA_COMMAND_ECM_FP_CANCEL,	  ///< 3(��Ŀ���)����ECMָ����ʾ
	CA_COMMAND_EMM_FINGERPRINT,	  ///< 4(��Ŀ�����)EMMָ�� dwParm2 = (DWORD)&EMM_FINGERPRINT_S
	CA_COMMAND_EMM_FP_CANCEL,	  ///< 5(��Ŀ�����)����EMMָ����ʾ��dwParm2 =1ȡ������emmpf, dwParm2=2ȡ��channel��emmpf
	CA_COMMAND_SHOW_SUBTITLE,	  ///< 6(��Ŀ�����)��ʾSubtitle��Ϣ, dwParm2 = (DWORD)&CaSubtitleInfo_S
	CA_COMMAND_OTA,				  ///< 7(��Ŀ�����)OTA��Ϣ dwParm2 = (DWORD)&CaOtaInfo_S
	CA_COMMAND_IPPV_NOTIFY,		  ///< 8(��Ŀ���)IPPV������Ϣ ,��Ҫ����CAIPPVProInfo_S[2]����ȡ��Ϣ����2��CAIPPVProInfo_S��С���ݽṹ
	                              ///< Ӧ����Ҫ����1������2��ppv�Ĺ���򣬾���˵������
	            				  /**@note ��������
	            				     @CAIPPVProInfo_S sCAIPPVProInfo[2];
				                     @memset(sCAIPPVProInfo, 0, sizeof(CAIPPVProInfo_S)*2);
				                     @sCAIPPVProInfo[0] = *(CAIPPVProInfo_S*)dwParm2;                          //��ǰ��ppv
				                     @sCAIPPVProInfo[1] = *(CAIPPVProInfo_S*)(dwParm2+sizeof(CAIPPVProInfo_S));//��һ��ppv
				                     @�����ȡ����sCAIPPVProInfo[i].m_wIPPVID == 0xffff �������ppv�ǲ����ڵģ�Ӧ�ò���Ҫ��ʾ����
				                     @������󣬿��Թ���(����CSCAConfirmBuyIPPV)��ȡ��(����CSCACancelWatchProgram)
				                     @�������˵����ʽ�μ��ӿ�˵�� 
				                     */
	CA_COMMAND_CLEAR_IPPV_SHOW,   ///< 9(��Ŀ���)ȡ��ippv�Ի���
	CA_COMMAND_MATURE_NOTIFY,     ///< 10(��Ŀ���)���˼���Ŀ��
	                             /**@note Ӧ����Ҫ����ʾ�����û�����pin��
	                                @ȷ��pin�벢��������ۿ�����CSCAConfirmWatchMrProgram
	                                @ȡ���ۿ�����CSCACancelWatchProgram
	                              	*/
	CA_COMMAND_CHANNEL_EMM_FP,	 ///< 11(��Ŀ���)��Ŀ�û�ָ�� dwParm2 = (DWORD)&ChannelEMM_FINGERPRINT_S
	CA_COMMAND_MAX
}CaCommand_E;

/**@brief STATUS��Ϣ*/
typedef enum _CaStatus
{
	CA_STATUS_CARD_IN_BEGIN, 			///< 0(��Ŀ�����)����ʼ��ʼ��(�����Ϣ���Բ�����ʾ)
	CA_STATUS_CARD_IN_END_SUCCESS,      ///< 1(��Ŀ�����)����ʼ���ɹ�(�����Ϣ3s�������ʧ)
	CA_STATUS_CARD_IN_END_FAIL, 		///< 2(��Ŀ�����)����ʼ��ʧ��
	CA_STATUS_SC_PIN_RESET, 			///< 3(��Ŀ�����)�������óɹ�(�����Ϣ3s�������ʧ)
	CA_STATUS_STB_SC_PAIRED,			///< 4(��Ŀ�����)������Գɹ�(�����Ϣ3s�������ʧ)
	CA_STATUS_STB_SC_FREE_PAIRED, 	    ///< 5(��Ŀ�����)���������Գɹ�(�����Ϣ3s�������ʧ)
	CA_STATUS_STB_SC_PAIRED_FAILED, 	///< 6(��Ŀ�����)�������ʧ��
	CA_STATUS_MAX
}CaStatus_E;

/**@note ��Ϣ����ʾ˵��
@AbvCa����Ϣ����ʾ��Ҫ��serviceid��ϵ������
@Ӧ����Ҫ���ýӿ�INT CSCARegisterMsgCallback(DWORD dwUserData, F_CBMsg fcbMsg)ע��ص�
@Ӧ�ÿ���ͨ���ص��õ����ݵ����һ������dwparm4�õ���Ŀid:	
@DWORD serviceId;
@serviceId =  (*(FuncbParam_S*)dwparm4).m_hService; 
@��serviceId = 0ʱ��ʾ�ͽ�Ŀ�޹أ����н�Ŀ��Ҫ��ʾ;
@��serviceId != 0ʱҪ�͵�ǰ�Ľ�Ŀid�Ƚϣ��뵱ǰ��ĿID��ͬ����ʾ
*/

typedef enum
{
	EM_ABVCA_STOPAV,      ///< 0.���ܹۿ���ĿʱCA_ALARM��Ϣ��Ҫͣ����Ƶ������Ϣ�ص��ĵ�3����������
	EM_ABVCA_STOPAUDIO,   ///< 1.���ܹۿ���ĿʱCA_ALARM��Ϣ��Ҫͣ��Ƶ������Ϣ�ص��ĵ�3����������
	EM_ABVCA_STOPVIDEO    ///< 2.���ܹۿ���ĿʱCA_ALARM��Ϣ��Ҫͣ��Ƶ������Ϣ�ص��ĵ�3����������
}ABVCAAVSTOP_E;

/**@brief ʱ��ṹ��*/
typedef struct  _CaTime
{
	WORD		m_wYear;
	BYTE		m_ucMonth;
	BYTE		m_ucDay;
	BYTE		m_ucHour;
	BYTE		m_ucMinute;
}CATime_S;

#define CA_MAX_ECM_SIZE_FINGERPRINT      (17)
#define CA_MAX_EMM_SIZE_FINGERPRINT      (1024)

/**@brief ECM(��Ŀ)ָ����Ϣ*/
typedef struct _CaEcmFingerprint 
{
	BYTE    	m_acMsg[CA_MAX_ECM_SIZE_FINGERPRINT]; ///< ָ������,�ַ�������
	WORD		m_wLocationX;	                      ///< ������λ��
	WORD		m_wLocationY;                        ///< ������λ��
	BOOL        m_ucType;                             ///< ����:TRUE������Ҫ����ȡ����������ʾ;FALSE�Լ��涨��ʾλ��
}ECM_FINGERPRINT_S;

/**@brief EMM(�û�)ָ����Ϣ
@note Emm����ͱ�����ɫ˵����Χ(0x0~0xf)
@0 {0xff, 0xff, 0xff}, WHITE ��
@1 {0xc0, 0xc0, 0xc0}, LTGRAY �Ұ�
@2 {0x00, 0xff, 0xff}, LTCYAN ��
@3 {0x00, 0x80, 0x80}, CYAN ����
@4 {0xff, 0x00, 0xff}, LTMAGENTA Ʒ��
@5 {0x80, 0x00, 0x80}, MAGENTA ��
@6 {0x00, 0x00, 0xff}, LTBLUE ��
@7 {0x00, 0x00, 0x80}, BLUE ����
@8 {0xff, 0xff, 0x00}, YELLOW ��
@9 {0x80, 0x80, 0x00}, BROWN �����
@a {0x00, 0xff, 0x00}, LTGREEN ��
@b {0x00, 0x80, 0x00}, GREEN ����
@c {0xff, 0x00, 0x00}, LTRED ��
@d {0x80, 0x00, 0x00}, RED ���
@e {0x80, 0x80, 0x80}, GRAY ���
@f {0x00, 0x00, 0x00}, BLACK ��--͸��ɫ
@�����ȡ����������ʾ����
*/
typedef struct _CaEmmFingerprint 
{
	BYTE    	m_acMsg[CA_MAX_EMM_SIZE_FINGERPRINT]; ///< ָ������,�ַ�������
	WORD		m_wcLocationX;	                      ///< ������λ��
    WORD		m_wcLocationY;                        ///< ������λ��
	WORD     	m_wDuration;  	                      ///< ָ����ʾ�ĳ���ʱ��,��λ��.
	WORD     	m_wRepetition; 	                      ///< ָ����ʾ���ظ�����
	WORD     	m_wInterval; 	                      ///< ����ָ����ʾ���ʱ����
	BYTE		m_ucFontColour;                       ///< ������ɫ
	BYTE		m_ucBgColour;	                      ///< ������ɫ
	BOOL        m_bfontFlash;                         ///< �����Ƿ���˸,����˸ΪTRUEʱ
	BYTE        m_ucStyle;                            ///< ���뷽ʽ-----���忴�ʼ���Ϣ��˵��
}EMM_FINGERPRINT_S;

typedef struct _CaChannelEmmFingerprint 
{
	BYTE		m_acMsg[CA_MAX_EMM_SIZE_FINGERPRINT+17]; ///< ָ������,�ַ�������
	WORD		m_wcLocationX;						  ///< ������λ��
	WORD		m_wcLocationY;						  ///< ������λ��
	WORD		m_wDuration;						  ///< ָ����ʾ�ĳ���ʱ��,��λ��.
	WORD		m_wRepetition;						  ///< ָ����ʾ���ظ�����
	WORD		m_wInterval;						  ///< ����ָ����ʾ���ʱ����
	BYTE		m_ucFontColour; 					  ///< ������ɫ
	BYTE		m_ucBgColour;						  ///< ������ɫ
	BOOL		m_bfontFlash;						  ///< �����Ƿ���˸,����˸ΪTRUEʱ
	BYTE		m_ucStyle;							  ///< ���뷽ʽ-----���忴�ʼ���Ϣ��˵��
}ChannelEMM_FINGERPRINT_S;


#define MAX_EMAIL_LEN        (1024)
#define MAX_SENDERNAME_LEN   (20)  
#define MAX_MAILTITLE_LEN    (50)
#define MAX_EMAIL_NUM        (100)

/**@brief �ʼ�������Ϣ*/
typedef struct _CaMailContentInfo_S
{
	BYTE        m_byStyle;///< ��������
	                      ///< 0x00(ASCII)        ֧������:English,Indonesian
	                      ///< 0x05(ISO_8859_9)   ֧������:French,German,Turkish,Spanish,Dutch,Italian,Portuguese
	                      ///< 0x03(ISO_8859_7)   ֧������:Greek
	                      ///< 0x01(ISO_8859_5)   ֧������:Russian
	                      ///< 0x02(ISO_8859_6)   ֧������:Arabic,persian
	                      ///< 0x06(ISO_8859_2)   ֧������:polish,Czech
	                      ///< 0x07(ISO_8859_11)  ֧������:Thai
	                      ///< 0x12(GB2312)       ֧������:Chinese
	WORD        m_acEmailLen;
	BYTE		m_acEmailContent[MAX_EMAIL_LEN+1];	///< Email������ 
}CaMailContentInfo_S;

/**@brief �ʼ�ͷ��Ϣ*/
typedef struct _CaMailHeadInfo_S                       
{
	BOOL			m_bReadFlag;                        ///< �ʼ�״̬��־0:δ����1:�Ѷ�
	WORD			m_wEmailID;                         ///< �ʼ� ID 
	CATime_S		m_sMailTime;                        ///< �ʼ�����ʱ��
	BYTE     		m_ucSenderLen;                      ///< ��������������
	BYTE     		m_acSenderName[MAX_SENDERNAME_LEN+1]; ///< ����������
	BYTE    		m_ucTitleLen;                 	    ///< ���ⳤ��
	BYTE     		m_acTitle[MAX_MAILTITLE_LEN+1];       ///< ����
}CaMailHeadInfo_S;

#define MAX_SUBDATA_LEN (1024)

/**@brief Subtile--������Ϣ*/
typedef struct _CaSubtitleInfo 
{
	WORD	m_wSubLen;                    ///< �ܳ��ȣ�Ӧ�ÿ��Ժ���
	WORD	m_wSubID;                     ///< Subtitle ID  
	WORD	m_wDuration;  	              ///< Subtitle��ʾ�ĳ���ʱ��,��λ��.
	WORD	m_wRepetition; 	              ///< Subtitle��ʾ���ظ�����
	WORD	m_wInterval; 		          ///< Subtitleָ����ʾ���ʱ����
	WORD	m_wFlashLen;                  ///< ����
	BYTE 	m_acSubdata[MAX_SUBDATA_LEN]; ///< Subtitle����
	BYTE    m_ucStyle;                    ///< ���뷽ʽ
}CaSubtitleInfo_S;

#define CA_MAX_LABLE_LEN  (8)
#define CA_MAX_SMC_NUMBER_LEN  (16)
#define CA_MAX_SMC_NAME_LEN  (5)


/**@brief ���ܿ�������Ϣ*/
typedef struct _SmcInfo
{
	BYTE      	m_acLabel[CA_MAX_LABLE_LEN+1];           ///< ��Ӧ������
	BYTE      	m_acSmcNumber[CA_MAX_SMC_NUMBER_LEN+1];  ///< ���ܿ���
	BYTE      	m_acSmclName[CA_MAX_SMC_NAME_LEN+1];     ///< ���ܿ�����
	BYTE	    m_ucVersion;		                     ///< ���ܿ��汾��
	BYTE		m_ucAdultlevel;	                         ///< ���ܿ����˼���
	BYTE		m_ucAreal;		                         ///< ���ܿ�������
	BOOL		m_bPaired;		                         ///< ���ܿ��Ƿ���� TRUE:�����  FALSE:δ���
	BYTE		m_acStartHour;                           ///< �ۿ�ʱ�俪ʼʱ���Сʱλ,
	BYTE		m_acStartMinute;                         ///< �ۿ�ʱ�俪ʼʱ��ķ���λ,
	BYTE		m_acEndHour;                             ///< �ۿ�ʱ�����ʱ���Сʱλ,
	BYTE		m_acEndMinute;                           ///< �ۿ�ʱ�����ʱ��ķ���λ,
	BOOL		m_bIsChildSC;	                         ///< ���ܿ��Ƿ����ӿ� TRUE:�ӿ�  FALSE:ĸ��
	BYTE      	m_acMasterNum[CA_MAX_SMC_NUMBER_LEN+1];  ///< ĸ������
}CaSmcInfo_S;

#define MAX_OPERATOR_NUM         (4)       ///< ������Ӫ�̸���
#define MAX_OPERATORPPID_LEN     (6)       ///< �����Ӫ��PPID
#define MAX_OPERATORLABLE_LEN    (8)       ///< �����Ӫ��LABLE

/**@brief ��Ӫ�̵Ļ�����Ϣ�ṹ��*/
typedef struct _CaOperatorInfo
{
	BYTE		m_acOperatorPPID[MAX_OPERATORPPID_LEN+1];     ///< ��Ӫ��PPID,��ʾ��ʽ%s*/
	BYTE		m_acOperatorLable[MAX_OPERATORLABLE_LEN+1];   ///< ��Ӫ��Lable,��ʾ��ʽ%s*/
	DWORD		m_dwBalance;                                  ///< ��Ӫ�����
}CaOperatorInfo_S;

/**@brief ��Ȩ����ö��*/
typedef enum
{
	E_PPID_FREE_ENTITLE, ///< FREE ��Ȩ,��ʾʱֻ��ʾlable ��start/endʱ�伴�ɣ�����ʱ�䲻��Ҫ��ʾСʱ�ͷ���
	E_PPID_PPC_ENTITLE,	 ///< PPC  ��Ȩ,�ṹ�е���Ϣ����Ҫ��ʾ*/
	E_PPID_PPV_ENTITLE	 ///< PPV  ��Ȩ,�ṹ�е���Ϣ����Ҫ��ʾ*/
} CaPPIDEntitleType_E;

#define  MAX_LABEL_LEN   (8)

/**@brief ��Ȩ��ϸ��Ϣ*/
typedef struct 
{
	WORD 		m_wLinkID;	                ///< Packet ID
	BYTE 		m_acLabel[MAX_LABEL_LEN+1]; ///< Packet name
	BYTE 		m_ucPayState;               ///< 0:Already paid	 1,2:Paid by time  3:Paid by prog  4:Have not paid
	CATime_S  	m_sNDate;					///< Send time
	CATime_S 	m_sSDate;					///< Start time
	CATime_S  	m_sEDate;					///< End time
}CaEntitlementInfo_S;

#define  MAX_DATA_LEN    (5)

/**@brief OTA��Ϣ�ṹ��*/
typedef struct  _CaOtaInfo
{
	WORD 		m_wOtalen;			        ///< OTA���ݵ��ܳ���
	DWORD 		m_dwOtaMesgid;	            ///< OTA��Ϣ��MessageID
	BYTE		m_acOtadate[MAX_DATA_LEN];	///< OTA���ݵķ���ʱ��
	WORD		m_wOtapid;			        ///< OTA���ݵ�PID
	BYTE		m_ucDvbrecievetype;         ///< ���ջ�������IRD Type :DVB-C   DVB-S DVB-T
	BYTE		m_ucDvbmanufactor;	        ///< ���̴���*/
	DWORD		m_dwDvbhardvesion;	        ///< Ӳ���汾��
	DWORD		m_dwDvbsoftversion;	        ///< ����汾��
	BYTE		m_ucDvbModulation;	        ///< ���Ʒ�ʽ
	DWORD		m_dwDvbfreq;		        ///< frequencyOTA�������ڵķ���Ƶ���Ƶ��
	DWORD		m_dwDvbsymb;		        ///< symbolrateOTA�������ڵķ���Ƶ��ķ�����
	BYTE		m_ucDvbpolarization;	    ///< ������ʽ
	BYTE		m_ucDvbFEC;		            ///< DVB-C��DVB-S��ʹ�õ�ǰ��������
	BYTE		m_ucDvbGuardinterval;       ///< �������
	BYTE		m_ucDvbTransmission_mode;   ///< ����ģʽ
	BYTE		m_ucDvbcoderate;	        ///< ���ȼ�����
	BYTE		m_ucDvbBandWidth;	        ///< ����
	BYTE		m_ucDvbConstellation;       ///< �������ڵľ���
}CaOtaInfo_S;

#define MAX_IPPV_LEN   (24)

/**@brief PPV��Ϣ�ṹ��
@note  
- ��m_ucIPPVType = 1ʱ��˵��ppv�ǰ�ʱ�Ʒѵģ�����ʽΪ "��ʱ��Ʒ�"m_dwIPPVTimePrice��Ч��m_dwIPPVViewPrice��Ч
- ��m_ucIPPVType = 2ʱ��˵��ppv�ǰ��������ѣ�����ʽΪ "����������"m_dwIPPVTimePrice��Ч��m_dwIPPVViewPrice��Ч
- ��m_ucIPPVType = 0ʱ��Ҫ�ṩ2�ַ�ʽ���û�ѡ���û�ѡ�����־Ͱ����ַ�ʽȷ��m_dwIPPVTimePrice��m_dwIPPVViewPrice����Ч
*/
typedef struct  _CAIPPVProInfo_S
{
	BYTE	  m_ucIPPVType;                ///< 1:Pay per minute  2:Pay per View  0:Both types can be chosen by users
	DWORD	  m_dwIPPVTimePrice;           ///< price of pay per minute --long int---
	DWORD	  m_dwIPPVViewPrice;           ///< price of pay per view ---long int---
	BYTE	  m_ucIPPVName[MAX_IPPV_LEN+1];///< Program instrction
	CATime_S  m_sIPPVStartTime;            ///< Start time
	CATime_S  m_sIPPVEndTime;              ///< Finish time
	WORD      m_wIPPVID;                   ///< ppvID  �� m_wIPPVID=0xffff  ʱ������ǰ��ppv������
}CAIPPVProInfo_S;

/**@brief ������ʷ*/
typedef struct _IPPVHistoryInfo_S
{
	CATime_S  m_sIPPVEntitletime;    ///< ���ѻ��ֵʱ��
	LONG	  m_dwIPPVEntitletokens; ///< ���ѻ��ֵ����
}IPPVHistoryInfo_S;

#define MAX_UPDATEDATA_LEN  (5)

/**@brief CA�ķ���ֵ*/
#define CA_RTN_FAIL                 		(0xff)  ///< ����ʧ��
#define	CA_RTN_OK                   		(0x00)  ///< �����ɹ�
#define	CA_RTN_PIN_ERROR           			(0x01)  ///< PIN�����
#define	CA_RTN_PIN_LOCK						(0x02)	///< PIN������
#define	CA_RTN_PIN_NO_MATCH					(0x03)  ///< ��PIN���ȷ���벻һ��
#define	CA_RTN_MR_ERROR						(0x04)  ///< ���˼����Ŀ����
#define	CA_RTN_WT_ERROR						(0x05)  ///< ����д����
#define	CA_RTN_IPPV_ERROR					(0x06)  ///< IPPV�������
#define	CA_RTN_NO_ENOUGH_TOKENS				(0x07)  ///< ����
#define	CA_RTN_CARDOUT_ERROR				(0x08)  ///< �޿����������ܿ�
#define	CA_RTN_TYPE_ERROR					(0x09)  ///< ���ܿ����ʹ���
#define	CA_RTN_OTHER_ERROR					(0x0A)  ///< ��������

/**
@brief ��ȡ���ܿ���Ϣ

@param[out] *pstSmcInfo��ȡ�Ŀ���Ϣ
@return  CA_RTN_OK    �ɹ� 
@		 CA_RTN_FAIL  ʧ��
*/
INT CSCAGetSmcInfo(CaSmcInfo_S * pstSmcInfo);

/**
@brief ��ȡ��������Ϣ

@param[in] ucBuffLen aucInfo�ĳ���(���ڵ���MAX_UPDATEDATA_LEN+1)
@param[out]aucInfo MAX_UPDATEDATA_LEN���ֽڵĳ�����Ϣ
@return �ɹ�:CA_RTN_OK  ʧ��:CA_RTN_FAIL
*/
INT CSCAGetSmcUpdataInfo(BYTE* aucInfo, BYTE ucBuffLen);

/**
@brief �޸����ܿ�����

@param[in] pucOldPin      ������
@param[in] ucOldPinLen    �����볤��
@param[in] pucNewPin      ������
@param[in] ucNewPinLen    �����볤��
@param[in] pucMatchPin    ȷ������
@param[in] ucMatchPinLen  ȷ�����볤��
@param[out]pucRetryTimes  ���������ԵĴ���
@return  CA_RTN_OK   	      �޸�����ɹ�
@	     CA_RTN_PIN_ERROR     �������
@	     CA_RTN_PIN_LOCK	  PIN���ѱ�����
@	     CA_RTN_PIN_NO_MATCH  �����벻ƥ��
@	     CA_RTN_OUT_ERROR	  �޿���������ܿ�	
@	     CA_RTN_OTHER_ERROR   ��������
@note �û�����1234����ôpucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4,  ucPinLen=4
*/
INT CSCAChangePin(BYTE *pucOldPin, 
                  BYTE ucOldPinLen,
                  BYTE *pucNewPin, 
                  BYTE ucNewPinLen,
				  BYTE *pucMatchPin,
				  BYTE ucMatchPinLen, 
				  BYTE *pucRetryTimes);

/**
@brief ���ó��˼���(6--21��Ч)

@param[in] pucPinCode      ����
@param[in] ucPinLen        ���볤��
@param[in] ucRating        ���˼���
@param[out]pucRetryTimes   ���������ԵĴ���
@return  CA_RTN_OK   	    ���óɹ�
@	     CA_RTN_ERROR       �������
@	     CA_RTN_PIN_LOCK    PIN���ѱ�����
@	     CA_RTN_OUT_ERROR   �޿���������ܿ�	
@	     CA_RTN_OTHER_ERROR ��������
@note �û�����1234����ôpucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4, ucPinLen=4
*/
INT CSCASetRating(BYTE *pucPinCode, 
                  BYTE ucPinLen,
                  BYTE ucRating, 
                  BYTE *pucRetryTimes);

/**
@brief ���ù���ʱ��(00:00-23:59)

@param[in] pucPinCode      ����
@param[in] ucPinLen        ���볤��
@param[in] ucStartTime_h   ��ʼʱ��Сʱ
@param[in] ucStartTime_m   ��ʼʱ�����
@param[in] ucEndTime_h     ����ʱ��Сʱ
@param[in] ucEndTime_m     ����ʱ�����
@param[out]pucRetryTimes   ���������ԵĴ���
@return  CA_RTN_OK   	    ���óɹ�
@	     CA_RTN_ERROR       �������
@	     CA_RTN_PIN_LOCK    PIN���ѱ�����
@	     CA_RTN_OUT_ERROR   �޿���������ܿ�	
@	     CA_RTN_OTHER_ERROR ��������
@note �û�����1234����ôpucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4, ucPinLen=4
*/
INT CSCASetWorkTime(BYTE *pucPinCode, 
                    BYTE ucPinLen,
                    BYTE ucStartTime_h, 
                    BYTE ucStartTime_m, 
                    BYTE ucEndTime_h, 
                    BYTE ucEndTime_m, 
                    BYTE *pucRetryTimes);

/**
@brief ��ȡ��Ӫ������

@param[out] *ucNumofOperator  ����
@return  CA_RTN_OK   	      �ɹ�
@	     CA_RTN_FAIL          ʧ��
*/
INT CSCAGetOperatorTotal(WORD * ucNumofOperator);

/**
@brief  ��ȡĳ����Ӫ����Ϣ

@param[in]  ucPPIDNO          ��Ӫ������,��0��ʼ
@param[out] *pstOperatorInfo  ��Ӫ����Ϣ
@return  CA_RTN_OK   	      �ɹ�
@	     CA_RTN_FAIL          ʧ��
*/
INT CSCAGetOperatorInfo(BYTE ucPPIDNO, CaOperatorInfo_S* pstOperatorInfo);

/**
@brief  ��ȡ��Ӫ��ĳ����Ȩ��Ϣ������

@param[in]  ucPPIDNO          ��Ӫ������,��0��ʼ
@param[in]  ePPIDEntitleType  ��Ȩ��Ϣ����:free or ppc or ppv
@param[out] *wNum             ��ȡ������Ȩ��Ϣ����      
@return  CA_RTN_OK   	      �ɹ�
@	     CA_RTN_FAIL          ʧ��
*/
INT CSCAGetServiceEntitlesTotal(BYTE ucPPIDNO, 
                                CaPPIDEntitleType_E ePPIDEntitleType, 
                                WORD* wNum);

/**
@brief  ��ȡĳ����Ӫ��ĳ����Ȩ����ϸ��Ϣ

@param[in]  ucPPIDNO             ��Ӫ������,��0��ʼ
@param[in]  wEntitleINO          ��Ҫ��ȡ�ڼ�����Ȩ��Ϣ,��0��ʼ
@param[in]  ePPIDEntitleType     ��Ȩ��Ϣ����:free or ppc or ppv
@param[out] *pstServiceEntitles  ��Ȩ��ϸ��Ϣ    
@return  CA_RTN_OK   	      �ɹ�
@	     CA_RTN_FAIL          ʧ��
*/
INT CSCAGetServiceEntitles(BYTE ucPPIDNO, 
                           WORD wEntitleINO, 
                           CaPPIDEntitleType_E ePPIDEntitleType, 
                           CaEntitlementInfo_S* pstServiceEntitles);

/**
@brief  ��ȡĳ����Ӫ�����ѻ��ֵ��ʷ����

@param[in]  ucPPIDNO       ��Ӫ������,��0��ʼ
@param[out] *wNumofRecord  ���ֵ��ʷ���� 
@return  CA_RTN_OK   	   �ɹ�
@	     CA_RTN_FAIL       ʧ��
*/
INT CSCAGetHistRecordTotal(BYTE ucPPIDNO, WORD *wNumofRecord);

/**
@brief  ��ȡĳ����Ӫ��ĳ�����ѻ��ֵ��ʷ����ϸ��Ϣ
     
@param[in]  ucPPIDNO       ��Ӫ������,��0��ʼ
@param[in]  wRecordNO      ��ʷ��¼���(0��ʼ)
@param[out] *psIPPVHistoryInfo  ������ʷ���ֵ��ϸ��Ϣ
@return  CA_RTN_OK   	      �ɹ�
@	     CA_RTN_FAIL          ʧ��
*/
INT CSCAGetHistRecordInfo(BYTE ucPPIDNO, 
                          WORD wRecordNO, 
                          IPPVHistoryInfo_S *psIPPVHistoryInfo);

/**
@brief  �����浯��������û�ȷ��Ҫ����IPPVʱ���ô˺���
     
@param[in]  pucPin       ����
@param[in]  ucPinLen     ���볤��
@param[in]  ucPayType    1:��ʱ�丶�ѣ�2:����������
@param[in]  ucIndex      0:��ǰ��Ŀ��1:��һ����Ŀ
@param[out] *ucRetryNum  ���뻹�������Լ��Σ����5��
@return  CA_RTN_OK   	      �ɹ�
@	     CA_RTN_FAIL          ʧ��
@note �û�����1234����ôpucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4, ucPinLen=4
*/
INT CSCAConfirmBuyIPPV(BYTE* pucPin, 
                       BYTE ucPinLen,
                       BYTE ucPayType, 
                       BYTE ucIndex, 
                       BYTE* ucRetryNum);

/**
@brief  �����浯�����˼����Ŀʱ���û�ȷ��Ҫ�ۿ��˽�Ŀʱ���ô˺���
     
@param[in]  pucPin       ����
@param[in]  ucPinLen     ���볤��
@param[out] *ucRetryNum  ���뻹�������Լ��Σ����5��
@return  CA_RTN_OK   	       �ɹ�
@        CA_RTN_CARDOUT_ERROR  �޿������ܿ�����
@        CA_RTN_PIN_ERROR      �������
@        CA_RTN_OTHER_ERROR    ����ippv����
@        CA_RTN_PIN_LOCK       ���ܿ�����
@note �û�����1234����ôpucPin[0]=0x1,pucPin[0]=0x2,pucPin[0]=0x3,pucPin[0]=0x4, ucPinLen=4
*/
INT CSCAConfirmWatchMrProgram(BYTE* ucPin, BYTE ucPinLen, BYTE* ucRetryNum);

/**
@brief  �����浯��IPPV/���˼����Ŀ��ʾʱ���û�����ʧ�ܻ�ȡ������ʱ����
     
@param[in]  ucType      TRUE ȡ��������������룬�Ժ��ٵ���IPPV�����˼�����ʾ   
@						FALSE��Ҫ���򣬵��ǹ���ʧ�ܣ��Ժ󻹻���IPPV�����˼�����ʾ
@return  CA_RTN_OK   	�ɹ�
@	     CA_RTN_FAIL    ʧ��
*/
INT CSCACancelWatchProgram(BOOL ucType);

/**
@brief  ��ȡ�ʼ�������
     
@param[out]  *ucNum   �ʼ�����
@return  CA_RTN_OK    �ɹ�
@	     CA_RTN_FAIL  ʧ��
*/
INT CSCAGetMailTotalNum(BYTE *ucNum);

/**
@brief  ��ȡδ���ʼ�������
     
@param[out]  *ucNum   δ���ʼ�����
@return  CA_RTN_OK    �ɹ�
@	     CA_RTN_FAIL  ʧ��
*/
INT CSCAGetNewMailTotalNum(BYTE *ucNum);

/**
@brief  ��ȡһ���ʼ���ͷ��Ϣ
     
@param[in]  ucIndex      �ʼ�����;0��ʼ
@param[out]  *pMailHead   ��Ӧ�����µ��ʼ�ͷ��Ϣ
@return  CA_RTN_OK    �ɹ�
@	     CA_RTN_FAIL  ʧ��
*/
INT CSCAGetMailHead(BYTE ucIndex, CaMailHeadInfo_S *pMailHead);

/**
@brief  ��ȡһ���ʼ�������
     
@param[in]  ucIndex           �ʼ�����;0��ʼ
@param[out]  *psMailContent   ��Ӧ�����µ��ʼ�����
@return  CA_RTN_OK    �ɹ�
@	     CA_RTN_FAIL  ʧ��
*/
INT CSCAGetMailContent(BYTE ucIndex, CaMailContentInfo_S *psMailContent);

/**
@brief  ɾ��һ���ʼ�
     
@param[in]  ucIndex   �ʼ�����;0��ʼ
@return  CA_RTN_OK    �ɹ�
@	     CA_RTN_FAIL  ʧ��
*/
INT CSCADelOneMail(BYTE ucIndex);

/**
@brief  ɾ�������ʼ�
     
@return  CA_RTN_OK    �ɹ�
@	     CA_RTN_FAIL  ʧ��
*/
INT CSCADelAllMail(void);

/**
@brief  ����pmtpid

@param[in]  wDemuxId    demuxid
@param[in]  wOriNetId   ԭʼ����id
@param[in]  wTsID       ts��id
@param[in]  wServiceID  ��Ŀid
@param[in]  wPmtPid     pmtpid 
@return  CA_RTN_OK    �ɹ�
@	     CA_RTN_FAIL  ʧ��
*/
void CSCASetSerivcePmtPid(WORD wDemuxId, 
                          WORD wOriNetId, 
                          WORD wTsID, 
                          WORD wServiceID, 
                          WORD wPmtPid);

/**
@brief  ��ȡABV CA  ��������Flash ��ַ(����������д��һ��������ͬһ��Block)

@param[in]  dwSize   ����flash�Ĵ�С
@return     һ������Block (64K) ����ʼ��ַ
*/
extern DWORD CSCAGetFlashAddress(DWORD dwSize);

/**
@brief  ��ȡABV CA  ������������Flash ��ַ(����������д��һ��������ͬһ��Block)

@param[in]  dwSize  ����flash�Ĵ�С
@return     һ������Block (64K) ����ʼ��ַ
*/
extern DWORD CSCAGetBackupFlashAddress(DWORD dwSize);

/**
@brief ��Ӧ�û��CA����Ҫ����Ϣ

@param[out]  *ucManucode    STB���̴���(abv������Ƿ����������)
@param[out]  *pdwHardVer    Ӳ���汾
@param[out]  *pdwSoftVer    ����汾
*/
extern void CSCAGetSTBInfoFromApp(BYTE *ucManucode, DWORD *pdwHardVer, DWORD *pdwSoftVer);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif //_CS_ABVCA_INTERFACE_H_

