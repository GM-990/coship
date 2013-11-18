/***************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_TRCA_Interface.h  
* Author:      qiaozhenghui
* Date:        Jan 2013
* Description: TR CA Interface 
*            
****************************************************************************************************/
#ifndef __CS_TRCA_INTERFACE_H__
#define __CS_TRCA_INTERFACE_H__

#include "udiplus_typedef.h"
#include "cs_cainterface.h"

#ifdef  __cplusplus
extern "C" {
#endif

/******************************************************************** 
  �ṹ������:  ����������CA�׸�Ӧ�õ����е���Ϣ����
  
  ����Լ��:    CA_ALARM:����CA lib���׵���ʾ��Ϣ,������ο�TRCaAlarm_E
               CA_STATUS:���׿���״̬,������ʾ�Ͷ�����Ӧ�ö���
               CA_COMMAND:����ҵ����Ϣ,���һ����������ҵ����Ϣ�ĵ�ַ
********************************************************************/
typedef enum _TRCaEventGroup
{
	CA_ALARM = 0,                    //ALARM��Ϣ 
	CA_STATUS,                       //STATUS��Ϣ 
	CA_COMMAND,                      //COMMAND��Ϣ   
	CA_INFO,                         //reserve for future
	CA_MMI,                          //reserve for future
    CA_MAX                           //��Ч�ֶ�  
}TRCaEventGroup_E;

/********************************************************************  
  �ṹ������:  ����������CA�׸�Ӧ�õ����е�ҵ����Ϣ
  
  ����Լ��:    ANNOUNCEMENTΪ�����¼�,�û��յ���Ӧ��������ʾ,�����˳���ɾ��,
           ���빫������Բ鿴��ɾ��
               NOTIFICATIONΪ�ɷ�֪ͨ,�û��յ���Ӧ������CSCACheckEntitleInfo�����ж��Ƿ���Ҫ��ʾ
               MAIL:Ϊ��ͨ�Ż���Ϣ,�û��յ�����Ҫ����Ļ����ʾ��ʾ��Ϣ,
           ����������Բ鿴��ɾ�� 
               COMMAND��Ϣ�ڻص�������һ������Ϊ��Ӧ�ı���Ϣ�ĵ�ַ��ForcePlayͬʱ�����н�Ŀ��Ҫ��
********************************************************************/
typedef enum _TRCaCommand
{
    CA_COMMAND_MAIL  =  100,                             //�ʼ�
    CA_COMMAND_ANNOUNCEMENT,                             //����
    CA_COMMAND_NOTIFICATION,                             //֪ͨ
    CA_COMMAND_FINGERPRINT,                              //ָ��,�յ�����Ϣ����Ҫ������ȡ����,����Ҫ��ʾ5-10��,���ܹ����������渲����ʾ
    CA_COMMAND_FORCEPLAY,                                //ǿ����̨
    CA_COMMAND_MAX                                       //��Ч�ֶ�
}TRCaCommand_E;

/******************************************************************** 
  �ṹ������:  ����������CA�׸�Ӧ�õ����п���Ϣ
  
  ����Լ��:    ��ʾ�ֶκͶ�������Ӧ�����о���
********************************************************************/
typedef enum  _TRCaStatus 
{ 
	CA_STATUS_CARD_INSERT = 200,                        //�����벢��λ�ɹ�,
	CA_STATUS_CARD_REMOVE,                              //���γ�,
	CA_STATUS_CARD_ERROR,                               //�����뵫��λ���ɹ�,����,
	CA_STATUS_CARD_UNKNOWN,                             //δʶ��,
	CA_STATUS_CARD_MAX                                  //��Ч�ֶ�  
}TRCaStatus_E; 

/********************************************************************  
  �ṹ������:  ����������CA�׸�Ӧ�õ�����Alarm��Ϣ,��ҪӦ������Ļ����ʾ
  
  ����Լ��:    ��ʾ�ֶ�Ϊ����Ҫ��,���ɸ���;
               ALARM��Ϣ��ʾ��ʱ����Ҫ�ر�����Ƶ,
               CA_ALARM_CANCEL��Ҫ������Ƶ
********************************************************************/
typedef enum _TRCaAlarm
{
	CA_ALARM_INPUT_CA_MOUDLE = 300,             //��ʾ "E01 �����CAģ��"
	CA_ALARM_CA_EEPROM_FAILURE,                 //��ʾ "E02 CAģ��EEPROMʧ��" 
	CA_ALARM_CA_FAILURE1,                       //��ʾ "E03 CAģ��ʧ��" 
	CA_ALARM_SC_INPUT,                          //��ʾ "E04 ��������ܿ�"
	CA_ALARM_SC_UNKNOWN ,                       //��ʾ "E05 ϵͳ��ʶ��˿�"
	CA_ALARM_SC_FAILURE,                        //��ʾ "E06 ���ܿ�ʧ��"
	CA_ALARM_SC_CHECKING,                       //��ʾ "E07 ���ڼ�����ܿ�"
	CA_ALARM_CA_FAILURE2,                       //��ʾ "E08 CAģ��ʧ��"
	CA_ALARM_SC_EEPROM_FAILURE,                 //��ʾ "E09 ���ܿ�EEPROMʧ��"
	CA_ALARM_PMT_FORMAT_ERROR ,                 //��ʾ "E10 PMT��ʽ����"
	CA_ALARM_SC_NOT_MATCHED,                    //��ʾ "E11 ������ƥ��"	
	CA_ALARM_SC_FEED ,                          //��ʾ "E12 ��ι���ӿ�"
	CA_ALARM_SERVICE_NOT_AVIALABLE,             //��ʾ "E13 û����Ч��Ŀ"
	CA_ALARM_SERVICE_NOT_AUTHORIZED,            //��ʾ "E14 ��Ŀû����Ȩ"
	CA_ALARM_SERVICE_AUTHORIZED ,               //��ʾ "E15 ���յ���Ȩ"
	CA_ALARM_SERVICE_IS_SCRAMBLED,              //��ʾ "E16 ��ǰ��Ŀ�Ѽ���"
	CA_ALARM_SERVICE_CURRENTLY_SCRAMBLED,       //��ʾ "E17 ��ǰ��Ŀ�Ѽ���"
	CA_ALARM_NOT_ALLOWED_IN_BROADCASTER1,       //��ʾ "E18"
	CA_ALARM_NOT_ALLOWED_IN_BROADCASTER2,       //��ʾ "E19"
	CA_ALARM_NOT_ALLOWED_IN_BROADCASTER3 ,      //��ʾ "E20 ��Ӫ������Ȩ"
	CA_ALARM_SERVICE_IS_DESCRAMBLED1,           //��ʾ "E21"    
	CA_ALARM_SERVICE_IS_DESCRAMBLED2,           //��ʾ "E22"
	CA_ALARM_NOT_ALLOWED_IN_REGION,             //��ʾ "E24 �ޱ�������Ȩ"
	CA_ALARM_SC_NOT_COMPATIABLE ,               //��ʾ "E25 ���ܿ�������"
	CA_ALARM_SERVICE_UNKONWN,                   //��ʾ "E26 ϵͳ��ʶ��ǰ��Ŀ"
	CA_ALARM_SERVICE_NOT_RUNNING,               //��ʾ "E27 ��ǰ��Ŀ��������"   
	CA_ALARM_SC_LOCKED,                         //��ʾ "E28 ���ܿ�������"
	CA_ALARM_PARENTAL_LOCKED1,                  //��ʾ "E29"
	CA_ALARM_PARENTAL_LOCKED2 ,                 //��ʾ "E30"
	CA_ALARM_PARENTAL_LOCKED3,                  //��ʾ "E31 ��ĸ������"
	CA_ALARM_COUNTRY_NOT_ALLOWED,               //��ʾ "E32 ���Ҵ���û����Ȩ" 
	CA_ALARM_NO_AUTHORIZE_DATA,                 //��ʾ "E33 û����Ȩ����"
	CA_ALARM_ILLEGAL_BOX,                       //��ʾ "E34 ����Ȩ������"
	CA_ALARM_NO_SIGNAL ,                        //��ʾ "E35 û���ź�"
	CA_ALARM_CANCEL,                            //������������������,ȡ����ʾ
	CA_ALARM_MAX                                //��Ч�ֶ�
}TRCaAlarm_E;

/********************************************************************
  ö������:  ����������CA�׸�Ӧ�õĵ�ALARM��Ϣ������������ֵ
  
  ����Լ��:  ��ʾALARM��Ϣʱ,Ӧ����Ҫ���ݵ�����������ȷ���Ƿ�ر�����Ƶ
********************************************************************/
typedef enum _TRCaAVControl
{
	EM_TRCA_STOPAV,     //�ر�����Ƶ
	EM_TRCA_STOPAUDIO,  //�ر���Ƶ
	EM_TRCA_STOPVIDEO,  //�ر���Ƶ
	EM_TRCA_OPENAV,     //������Ƶ
	EM_TRCA_OPENAUDIO,  //����Ƶ
	EM_TRCA_OPENVIDEO,  //����Ƶ
	EM_TRCA_MAX         //��Ч�ֶ�
}TRCaAVControl_E;

/********************************************************************
  ö������:  ����������CA�׸�Ӧ�õ��ı���Ϣ����������
  
  ����Լ��:    ANNOUNCEMENTΪ�����¼�,�û��յ���Ӧ��������ʾ,�����˳���ɾ��,
           ���빫������Բ鿴��ɾ��
               NOTIFICATIONΪ�ɷ�֪ͨ,�û��յ���Ӧ��������ʾ,����ɾ��
               MAIL:Ϊ��ͨ�Ż���Ϣ,�û��յ�����Ҫ����Ļ����ʾ��ʾ��Ϣ,
           ����������Բ鿴��ɾ�� 
********************************************************************/
typedef enum _TRCaMsgType
{
	CA_MSG_MAIL,        //�ʼ�
	CA_MSG_ANNOUNCE,    //����
	CA_MSG_NOTIFICATION //֪ͨ
}TRCaMsgType_E;

/******************************************************************** 
  ö������:  ����������CA�׸�Ӧ�õ��ı���Ϣ��������ʾģʽ
  
  ����Լ��: CA_MSG_NORMAL,��Ҫ֪ͨ�û�,�û����Խ�������򹫸����н��в쿴
            CA_MSG_TIMED ,��Ҫ������ʾ,ʱ�����ı���Ϣ�ṹ���� 
            CA_MSG_FORCED,��Ҫ����ǿ����ʾ
********************************************************************/
typedef enum _TRCaMsgClass
{
	CA_MSG_NORMAL,
	CA_MSG_TIMED,      /* Automatic erase after wRemainTime seconds. */
	CA_MSG_FORCED      /* Forced display message. */
}TRCaMsgClass_E;

/********************************************************************  
  ö������:  ����CA Flash��ص�ַ����ֵ
  
  ����Լ��:   
********************************************************************/
typedef enum _TRCaFlashAddrErr
{
	EM_FLASH_FATAL_ERROR = -4,        
    EM_FLASH_PARAM_ERROR = -3,
	EM_FLASH_LENGTH_ERROR = -2,
	EM_OTHER_FAILURE = -1,
	EM_FLASH_SET_SUCCESS = 0
}TRCaFlashAddrErr_E;

/********************************************************************  
  ö������:  ��ȡ��Ȩ��Ϣ����ֵ
  
  ����Լ��:   
********************************************************************/
typedef enum _TRCaEntitleRet
{
    EM_ENTITLE_VALID  = 0,          //��Ч��Ȩ
    EM_ENTITLE_PARA_ERR,            //��������
    EM_ENTITLE_NO_ENTITLE,          //û����Ȩ
    EM_ENTITLE_VALID_LIMITED,       //��Ч��Ȩ,��������
    EM_ENTITLE_CLOSED,              //��Ȩ�ر�
    EM_ENTITLE_OPEN                 //��ͣ��Ȩ
}TRCaEntitleRet_E;

/********************************************************************  
  ö������:  ����CSCAReadDataFromMaster����ֵ
  
  ����Լ��:   
********************************************************************/
typedef enum _TRCaReadMasterRet
{
    EM_READ_MASTER_OK = 0,              //��ȡ�ɹ�
    EM_READ_MASTER_SC_ERROR = 1,        //���ܿ�����
    EM_READ_MASTER_DATA_ERROR = 3,      //û�л�ȡ����Ч����
    EM_READ_MASTER_NOT_SUPPORT = 5,     //���ܿ����Դ���
    EM_READ_MASTER_STATE_ERROR = 6,     //û�л�ȡ����Ч����
    EM_READ_MASTER_UNKNOW_ERROR = 9     //δ֪����
}TRCaReadMasterRet_E;

/********************************************************************  
  ö������:  ����CSCAWriteDataToSlaver����ֵ
  
  ����Լ��:   
********************************************************************/
typedef enum _TRCaWriteSlaverRet
{
    EM_WRITE_SLAVER_OK = 0,             //�����ɹ�
    EM_WRITE_SLAVER_NOT_OK = 1,         //���ܿ�ʧ�ܻ�ι������ʧ��
    EM_WRITE_SLAVER_DATA_ERROR = 3,     //ι������ʧ��
    EM_WRITE_SLAVER_WRITE_ERROR = 4,    //д���ܿ�ʧ��
    EM_WRITE_SLAVER_NOT_SUPPORT = 5,    //���ܿ����Դ���
    EM_WRITE_SLAVER_STATE_ERROR = 6,    //û�л�ȡ����Ч����
    EM_WRITE_SLAVER_MATCH_ERROR = 7,    //��ĸ����ƥ��
    EM_WRITE_SLAVER_UNKNOE_ERROR = 9    //δ֪����
}TRCaWriteSlaverRet_E;

/******************************************************************** 
  �ṹ������:  ForcePlay�����Ϣ
  
  ����Լ��:   ����ң����֮�������ְ취�������: 1.���������У�2.ǰ�˷�m_ucLockedFlag=0��ָ��
                ���в��Ľ�Ŀ���ڽ�Ŀ�б��У�Ӧ�ú��Դ���Ϣ
********************************************************************/
typedef struct _TRCaForcePlay
{
    BYTE    m_ucLockedFlag;  //TRUE:ң�����������������û���̨��FALSE:�����û���̨

    WORD    m_wNetWorkId;   //��Ŀ��Ҫ��
    WORD    m_wTsId;
    WORD    m_wServiceId;
    
    WORD    m_wContentLen;  //reserved
    BYTE*   m_pucContent;   //reserved
}TRCaForcePlay_S;

/******************************************************************** 
  �ṹ������:  CA Flash��ַ��Ϣ
  
  ����Լ��:   
********************************************************************/
typedef struct _TRCaFlashAddress
{
    BOOL    m_bCaFlashAddressFlag;  //TRUE: ��ʽapp;FALSE: TestApp
    DWORD   m_dwMainFlashAddr;      //��Flash ��ַ
    DWORD   m_dwMainFlashLength;    //��Flash ���� 
    DWORD   m_dwBackupFlashAddr;    //����Flash  ��ַ����ʹ��ֱ�Ӵ���0
    DWORD   m_dwBackupFlashLength;  //����Flash  ��������ʹ��ֱ�Ӵ���0
    
    DWORD   m_dwTxtMsgFlashAddr;    //�ı���ϢFlash��ַ
    DWORD   m_dwTxtMsgFlashLenth;   //�ı���ϢFlash����
    DWORD   m_dwReserve3;           //���������ĳЩCA ��������ĵ�ַ
    DWORD   m_dwReserve4;           //���������ĳЩCA ��������ĵ�ַ
}TRCaFlashAddress_S;

/********************************************************************  
  �ṹ������:  ����������CA�׸�Ӧ�õ��ı���Ϣ������ʱ���
  
  ����Լ��:   
********************************************************************/
typedef struct _TRCaTimeStamp
{
	WORD    m_wYear;         // for example 1998,  m_wYear = 1998
	BYTE    m_ucMonth;       // 1 to 12 
	BYTE    m_ucDay;         // 1 to 31 
	BYTE    m_ucHour;        // 0 to 23 
	BYTE    m_ucMinute;      // 0 to 59 
	BYTE    m_ucSecond;      // 0 to 59 
}TRCaTimeStamp_S;

/******************************************************************** 
  �ṹ������:  ����������CA�׸�Ӧ�õ��ı���Ϣ�ṹ,�����ʼ�,����,֪ͨ
  
  ����Լ��:    m_wPeriod����֪ͨ���͵��ı���Ч
********************************************************************/
typedef struct _TRCaTxtMsg
{
   WORD             m_wIndex;                           //����,�ʼ��͹�����и��Ե�����
   BYTE             m_ucPriority;                       //reserve
   WORD             m_wPeriod;                          //

   TRCaMsgClass_E   m_eClass;                           //����,һ��,��ʱ��ʾ,ǿ����ʾ
   TRCaMsgType_E    m_eType;                            //��Ϣ����,�ʼ�,�������֪ͨ
   
   TRCaTimeStamp_S  m_sMsgTime;                         //reserve
   TRCaTimeStamp_S  m_sCreateTime;                      //����ʱ��
   
   WORD             m_wTitleLen;                        //���ⳤ��
   BYTE             *m_pucMsgTitl;                      //��������
   WORD             m_wDataLen;                         //���ݳ���
   BYTE             *m_pucTxtMsgData;                   //��Ϣ����
}TRCaTxtMsg_S;

/******************************************************************** 
  �ṹ������:  ��Ȩ��Ϣ
  
  ����Լ��:   �� 
********************************************************************/
typedef struct _TREntitleInfo
{
    WORD                 m_wProductId;      //��ƷID,0--319

    TRCaEntitleRet_E     m_eEntitleStatus;  //�˲�Ʒ����Ȩ״̬

    TRCaTimeStamp_S      m_sStartTime;      //��ʼʱ��
    TRCaTimeStamp_S      m_sEndTime;        //����ʱ��
}TREntitleInfo_S;

/******************************************************************** 
  �ṹ������:  ָ����Ϣ�Ľṹ
  
  ����Լ��:    ��
********************************************************************/
typedef struct _TRFingerPrintInfo
{
    BYTE    m_ucNumLen;         //ָ�����ݳ���
    BYTE   *m_pucNumData;       //ָ������
    BYTE    m_ucContentLen;     //reserved
    BYTE   *m_puccontentData;   //reserved
}TRFingerPrintInfo_S;

/**************************************************************************
*Function Name: CSCAGetSmartCardVersion
*   ��ȡ���ܿ��汾��Ϣ
*
*Parameters
*   pucVerBuf[out]:ָ��洢CA�汾��Ϣ�Ŀռ�
*   ucBufSize[in]:����һ����󳤶�ֵ,���ܹ�С��7���ֽ�
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
*
*Note:  ������Ҫ��ʾ���ܿ��İ汾��ʱ����
**************************************************************************/
INT CSCAGetSmartCardVersion(BYTE* pucVerBuf,  BYTE ucBufSize);

/**************************************************************************
* Function Name: CSCAGetSmartCardID
*   ��ȡ���ܿ��Ŀ���
*
*Parameters	
*    pcCardId[out]:���ַ�������ʽ�������ܿ����ţ�
*    ucCardIdBuff[in]:Ϊ���ܿ�������׼����Buff��С,����Ϊ������27���ֽ�
*         
*Return Values
*	0 : SUCCESS 
*	-1: FAILURE
*Note:�û���Ҫ��ȡ����ʱ����
**************************************************************************/
INT	CSCAGetSmartCardID(char* pcCardId, BYTE ucCardIdBuff);


/**************************************************************************
*Function Name: CSCASetDebugMode
*  ���ô�ӡģʽ,����Ӧ�õ���
*
*Parameters 
*	dwMode[in]:ÿ��ģ�鶼����Ӧ�Ĵ�ӡ����		
*   
*Note:����ʱ��CA��ʼ��֮ǰ����
**************************************************************************/
void CSCASetDebugMode(DWORD dwMode);

/**************************************************************************
* Function Name: CSCADelOneTxtMsg
*   ɾ���ʼ��򹫸��ĳһ���ı���Ϣ
*
*Parameters	
*  eTxtMsgType[in]:ɾ����Ϣ������,������ο�TRCaMsgType_E,
* wTxtMsgIndex[in]:��Ҫɾ����Ϣ������,�ʼ�0--31������0--29��
*         
*Return Values
*	0 : SUCCESS 
*	-1: FAILURE
* Note:�û�ɾ��ĳһ���͵�ĳһ��Ϣ��ʱ�����,����ɾ���ʼ��б�ĵ�һ���ʼ�
*   ��eTxtMsgType = CA_MSG_MAIL, wTxtMsgIndex = 0
**************************************************************************/
INT CSCADelOneTxtMsg(TRCaMsgType_E eTxtMsgType, WORD wTxtMsgIndex);

/**************************************************************************
* Function Name: CSCADelTxtMsg
*   ɾ���ʼ��򹫸��µ������ı���Ϣ
*
*Parameters	
*    eTxtMsgType[in]:ɾ����Ϣ������,������ο�TRCaMsgType_E,�����ʼ��͹�����Ч
*         
*Return Values
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:�û�ɾ���ʼ��򹫸����е���Ϣʱ����,�����������,eTxtMsgType = CA_MSG_MAIL
**************************************************************************/
INT CSCADelAllTxtMsg(TRCaMsgType_E eTxtMsgType);

/**************************************************************************
* Function Name: CSCAGetTxtMsgInfo
*   ��ȡ��Ϣ����
*
*Parameters	
*    eTxtMsgType[in]:��ȡ��Ϣ������
*        ucIndex[in]:����Ҫ��ȡ��Ϣ������,��0��ʼ������0--31����32�⣻����0--29����30�⣻֪ͨΪ0
*      psTxtMsg[out]:����Ϣ�洢����psMsgΪ��ʼ��һ�οռ�
*Return Values
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:�û�������ȡ�ʼ��򹫸��ʱ�����,����ʼ��򹫸��֪ͨ��Ϣ
**************************************************************************/
INT CSCAGetTxtMsgInfo(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, TRCaTxtMsg_S *psTxtMsg);

/**************************************************************************
*Function Name:CSCAGetTxtMsgNum
* ��ȡ�ʼ��򹫸���Ϣ������ʣ��ɴ洢�ĸ���
*
*Parameters 
*	eMsgType[in]:��Ϣ����,
*   pucTotalNum[out]:�ı���Ϣ����,�ʼ�1--32;����1--30
*   pucEmptyNum[out]:ʣ��ռ���Ŀ,�ʼ�1--32;����1--30
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:�û����빫������ʼ���,��Ҫ��ʾ�洢��Ϣʱ�����
**************************************************************************/
INT CSCAGetTxtMsgNum(TRCaMsgType_E eTxtMsgType, BYTE *pucTotalNum, BYTE *pucEmptyNum);

/**************************************************************************
*Function Name:CSCASetTxtMsgReadFlag
* ����ĳһ���ĳһ����Ϣ�Ƿ��Ѷ���־
*
*Parameters 
*   eMsgType[in]:��Ϣ����
*	ucIndex[in]:ĳһ����Ϣ�洢���±꣬��0��ʼ,����0--31����32��; ����0--29����30��.
*	bRead[in]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:�û��Ե�������򹫸�������õ�ʱ��ʹ��
**************************************************************************/
INT CSCASetTxtMsgReadFlag(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, BOOL bRead);

/**************************************************************************
*Function Name:CSCASetTxtMsgReadFlag
* ��ѯĳһ���ĳһ���ı���Ϣ�Ƿ��Ѷ�
*
*Parameters 
*   eMsgType[in]:��Ϣ����
*	ucIndex[in]:ĳһ����Ϣ�洢���±꣬��0��ʼ,����0--31����32��; ����0--29����30��.
*	pbRead[OUT]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:�û���������򹫸���鿴�ı���Ϣʱ����
**************************************************************************/
INT CSCAGetTxtMsgReadFlag(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, BOOL *pbRead);

/**************************************************************************
*Function Name:CSCAHaveNewTxtMsgNum
* ���ص�ǰĳһ������Ϣ���ж��ٷ�δ����Ϣ
*
*Parameters 
*	eMsgType[in]:��Ϣ����
*   pucNotReadNum[out]:δ����Ϣ��Ŀ,��ΧΪ:�ʼ�1--32; ����1--30
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:�û���ѯ����򹫸�洢��Ϣʱ�����
**************************************************************************/
INT CSCAHaveNewTxtMsgNum(TRCaMsgType_E eTxtMsgType, BYTE *pucNotReadNum);

/**************************************************************************
*Function Name:CSCAGetEntitleTotalNum
* ��ȡ��Ȩ��Ϣ��������
*
*Parameters 
*	pwTotalNum[out]:��ȡ��Ȩ��Ϣ��������,320
*   
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:�û�������Ȩ��Ϣ����鿴��Ȩ��Ϣʱ,�ȵ��ô˺�����ȡ��Ȩ��Ϣ����
**************************************************************************/
INT CSCAGetEntitleTotalNum(WORD *pwTotalNum);

/**************************************************************************
*Function Name:CSCAGetEntitleInfo
*   ��ȡ������Ȩ��Ϣ
*
*Parameters 
*	wIndex[in]:��Ȩ��Ϣ������,���ֵͨ��INT 
*       CSCAGetEntitleTotalNum(WORD *pwTotalNum)��ȡ, ��ΧΪ������ΧΪ0--319
*   psEntitleInfo[out]:ָ������ΪwIndex����Ȩ��Ϣ�Ĵ洢�ռ�
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       �û�������Ȩ����鿴��Ȩ��Ϣʱ����
**************************************************************************/
INT CSCAGetEntitleInfo(WORD wIndex, TREntitleInfo_S *psEntitleInfo);

/**************************************************************************
*Function Name:CSCAGetRating
*   ��ȡ���ܿ��Ĺۿ����������Ϣ,����Ϊ0--10
*
*Parameters 
*	pucRating[out]:���ظ�Ӧ������ѯ�ļ�����Ϣ,����Χ0--10
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       �û���ѯ�ۿ�����ʱ����
**************************************************************************/
INT CSCAGetRating(BYTE *pucRating);

/**************************************************************************
*Function Name:CSCASetRating
*   �������ܿ��Ĺۿ����������Ϣ,����Ϊ0--10
*
*Parameters 
*	        pucPin[in]:���ùۿ����������6λpin����׵�ַ
*           ucPinLen[in]:����Pin��ĳ��ȣ�Ϊ6���������򷵻�ʧ��
*   
*           ucRating[out]:����Ҫ���õĹۿ�����,��ΧΪ0--10
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       �û����ùۿ�����ʱ����
*       ����������ȷ��PIN��֮����ܹ����óɹ�
**************************************************************************/
INT CSCASetRating(BYTE* pucPin, BYTE ucPinLen, BYTE ucRating);

/**************************************************************************
*Function Name:CSCAOpenParentalControl
*   �����и�ĸ�����ƵĽ�Ŀ
*
*Parameters 
*	pucPin
        [in]:���븸ĸ�����ƽ�Ŀʱ����λ��pin����׵�ַ
*   ucPinLen
        [in]:PIN�볤�ȱ���Ϊ6λ,���򷵻�ʧ��
*   
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       �û����븸ĸ���ƵĽ�Ŀ֮ǰ����
*       ����������ȷ��PIN��֮����ܹ������Ŀ,����CA lib������н���,����ʧ��
**************************************************************************/
INT CSCAOpenParentalControl(BYTE* pucPin, BYTE ucPinLen);

/**************************************************************************
*Function Name:CSCAChangePin
*   �������ܿ���PIN��
*
*Parameters 
*	aucOldPin
        [in]:�ɵ�pin��,��������򷵻�ʧ��,PIN�볤��,����Ϊ6λ
*   pucNewPin
        [in]:�µ�PIN��,����Ϊ6λ,���򷵻�ʧ��
*   ucPinLen
        [in]:�������Pin��;�Pin��ĳ��ȣ�����Ϊ6Ϊ,���򷵻�ʧ��
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       �û����븸ĸ���ƵĽ�Ŀ֮ǰ����
*       ����������ȷ��PIN��֮����ܹ������Ŀ,����CA lib������н���,����ʧ��
**************************************************************************/
INT CSCAChangePin(BYTE* pucOldPin, BYTE* pucNewPin, BYTE ucPinLen);

/**************************************************************************
*Function Name:CSCAReadDataFromMaster
*   ��ĸ���л�ȡι������
*
*Parameters 
*   
*Return Values: 
*	��� TRCaReadMasterRet_E
*Note:
*       ��ĸ��ι��ʱ����ĸ����ȡ����ʱ����
**************************************************************************/
TRCaReadMasterRet_E CSCAReadDataFromMaster(void);

/**************************************************************************
*Function Name:CSCAWriteDataToSlaver
*   ��ĸ���л�ȡ��ι������ͨ���˽ӿ�д������Ե��ӿ���
*
*Parameters 
*	
*Return Values: 
*	��� TRCaWriteSlaverRet_E
*Note:
*       ��ĸ��ι��ʱ����ĸ����ȡ������д���ӿ���ʱ����
**************************************************************************/
TRCaWriteSlaverRet_E CSCAWriteDataToSlaver(void);

/**************************************************************************
*Function Name:CSCASetFlashAddress
* ����CA �ں˴洢���������FLASH ��ַ
*
*Parameters
*  pstFlashAddress: CA Flash �����Ϣ
                                
pstFlashAddress->m_bCaFlashAddressFlag:1 ����ģʽ,������дFLash
                                        0 TESTAPP ģʽ,���ݲ�д��flash

pstFlashAddress->m_dwMainFlashAddr: �����CA�ں�����������ַ,����Ϊһ������ֵ
pstFlashAddress->m_dwMainFlashLength: CA�ں�����������С,Ϊ64K

pstFlashAddress->m_dwBackupFlashAddr:�����CA�ں˱�����������ַ
pstFlashAddress->m_dwBackupFlashLength:CA�ں˱�����������С,Ϊ64K 

pstFlashAddress->m_dwTxtMsgFlashAddr:�����CA �ı���Ϣ�����������ַ
pstFlashAddress->m_dwTxtMsgFlashLenth:�ı���Ϣ��������ĳ���,Ϊ64k.
*                     
*Return Values: 
*	��CaFlashAddrErr_E����

Note:
        ��CA��ʼ��֮ǰ���ô˺�������Flash��ַ����  
        ���ú������ش��󣬽�����CA��ʼ��ʧ��!!
**************************************************************************/
TRCaFlashAddrErr_E CSCASetFlashAddress(TRCaFlashAddress_S *pstFlashAddress);

#ifdef  __cplusplus
}
#endif

#endif //__CS_TRCA_INTERFACE_H__

